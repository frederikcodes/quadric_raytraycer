#include "hittable/HittableList.h"

void HittableList::add(std::shared_ptr<Hittable> object)
{
    AABB box;
    bool hasBox = object->boundingBox(box);

    objects.push_back({object, box, hasBox});
}

void HittableList::clear()
{
    objects.clear();
}

const std::vector<HittableList::ObjectEntry> &HittableList::entries() const
{
    return objects;
}

bool HittableList::hit(const Ray &ray,
                       double tMin,
                       double tMax,
                       HitRecord &rec) const
{
    HitRecord tempRec;
    bool hitAnything = false;
    double closestSoFar = tMax;

    for (const auto &entry : objects)
    {
        if (entry.hasBox)
        {
            if (!entry.box.hit(ray, tMin, closestSoFar))
            {
                continue;
            }
        }

        if (entry.object->hit(ray, tMin, closestSoFar, tempRec))
        {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}

bool HittableList::anyHit(const Ray &ray,
                          double tMin,
                          double tMax) const
{
    HitRecord tempRec;

    for (const auto &entry : objects)
    {
        if (entry.hasBox)
        {
            if (!entry.box.hit(ray, tMin, tMax))
            {
                continue;
            }
        }

        if (entry.object->anyHit(ray, tMin, tMax))
        {
            return true;
        }
    }

    return false;
}

bool HittableList::boundingBox(AABB &box) const
{
    bool firstBox = true;

    for (const auto &entry : objects)
    {
        if (!entry.hasBox)
        {
            return false;
        }

        if (firstBox)
        {
            box = entry.box;
            firstBox = false;
        }
        else
        {
            box = surroundingBox(box, entry.box);
        }
    }

    return !firstBox;
}
