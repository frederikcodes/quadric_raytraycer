
#include "acceleration/Octree.h"

Octree::Octree(const HittableList &world,
               int maxDepth,
               int maxObjectsPerLeaf)
    : maxDepth(maxDepth),
      maxObjectsPerLeaf(maxObjectsPerLeaf)
{
    std::vector<OctreeObject> octreeObjects;

    bool firstBox = true;

    for (const auto &entry : world.entries())
    {
        if (!entry.hasBox)
        {
            continue;
        }

        OctreeObject obj;
        obj.object = entry.object;
        obj.box = entry.box;

        octreeObjects.push_back(obj);

        if (firstBox)
        {
            rootBounds = entry.box;
            firstBox = false;
        }
        else
        {
            rootBounds = surroundingBox(rootBounds, entry.box);
        }
    }

    hasRootBounds = !firstBox;

    if (hasRootBounds)
    {
        root = buildNode(rootBounds, octreeObjects, 0);
    }
}

bool Octree::boundingBox(AABB &outputBox) const
{
    if (!hasRootBounds)
    {
        return false;
    }

    outputBox = rootBounds;
    return true;
}

bool Octree::hit(const Ray &ray,
                 double tMin,
                 double tMax,
                 HitRecord &rec) const
{
    if (!root)
    {
        return false;
    }

    return hitNode(root.get(), ray, tMin, tMax, rec);
}

bool Octree::anyHit(const Ray &ray,
                    double tMin,
                    double tMax) const
{
    if (!root)
    {
        return false;
    }

    return anyHitNode(root.get(), ray, tMin, tMax);
}

std::unique_ptr<OctreeNode> Octree::buildNode(
    const AABB &bounds,
    const std::vector<OctreeObject> &objects,
    int depth) const
{
    auto node = std::make_unique<OctreeNode>();

    node->bounds = bounds;

    if (depth >= maxDepth ||
        static_cast<int>(objects.size()) <= maxObjectsPerLeaf)
    {
        node->objects = objects;
        return node;
    }

    AABB childBounds[8];
    createChildBounds(bounds, childBounds);

    std::vector<OctreeObject> childObjects[8];

    for (const auto &obj : objects)
    {
        for (int i = 0; i < 8; i++)
        {
            if (obj.box.overlaps(childBounds[i]))
            {
                childObjects[i].push_back(obj);
            }
        }
    }

    bool wasSplitUseful = false;

    for (int i = 0; i < 8; i++)
    {
        if (!childObjects[i].empty() &&
            childObjects[i].size() < objects.size())
        {
            wasSplitUseful = true;
            break;
        }
    }

    if (!wasSplitUseful)
    {
        node->objects = objects;
        return node;
    }

    for (int i = 0; i < 8; i++)
    {
        if (!childObjects[i].empty())
        {
            node->children[i] =
                buildNode(childBounds[i],
                          childObjects[i],
                          depth + 1);
        }
    }

    return node;
}

void Octree::createChildBounds(
    const AABB &bounds,
    AABB childBounds[8]) const
{
    Vec3 min = bounds.minimum;
    Vec3 max = bounds.maximum;
    Vec3 mid = bounds.center();

    int index = 0;

    for (int x = 0; x < 2; x++)
    {
        for (int y = 0; y < 2; y++)
        {
            for (int z = 0; z < 2; z++)
            {
                Vec3 childMin(
                    x == 0 ? min.x : mid.x,
                    y == 0 ? min.y : mid.y,
                    z == 0 ? min.z : mid.z);

                Vec3 childMax(
                    x == 0 ? mid.x : max.x,
                    y == 0 ? mid.y : max.y,
                    z == 0 ? mid.z : max.z);

                childBounds[index] = AABB(childMin, childMax);
                index++;
            }
        }
    }
}

bool Octree::hitNode(
    const OctreeNode *node,
    const Ray &ray,
    double tMin,
    double tMax,
    HitRecord &rec) const
{
    if (!node->bounds.hit(ray, tMin, tMax))
    {
        return false;
    }

    HitRecord tempRec;
    bool hitAnything = false;
    double closestSoFar = tMax;

    if (node->isLeaf())
    {
        for (const auto &obj : node->objects)
        {
            if (!obj.box.hit(ray, tMin, closestSoFar))
            {
                continue;
            }

            if (obj.object->hit(ray, tMin, closestSoFar, tempRec))
            {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }

    for (int i = 0; i < 8; i++)
    {
        if (!node->children[i])
        {
            continue;
        }

        if (hitNode(node->children[i].get(),
                    ray,
                    tMin,
                    closestSoFar,
                    tempRec))
        {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}

bool Octree::anyHitNode(
    const OctreeNode *node,
    const Ray &ray,
    double tMin,
    double tMax) const
{
    if (!node->bounds.hit(ray, tMin, tMax))
    {
        return false;
    }

    if (node->isLeaf())
    {
        for (const auto &obj : node->objects)
        {
            if (!obj.box.hit(ray, tMin, tMax))
            {
                continue;
            }

            if (obj.object->anyHit(ray, tMin, tMax))
            {
                return true;
            }
        }

        return false;
    }

    for (int i = 0; i < 8; i++)
    {
        if (!node->children[i])
        {
            continue;
        }

        if (anyHitNode(node->children[i].get(),
                       ray,
                       tMin,
                       tMax))
        {
            return true;
        }
    }

    return false;
}