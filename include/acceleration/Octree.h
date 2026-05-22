#pragma once

#include <memory>
#include <vector>

#include "acceleration/AABB.h"
#include "acceleration/OctreeNode.h"
#include "core/Ray.h"
#include "hittable/Hittable.h"
#include "hittable/HittableList.h"

class Octree : public Hittable
{
public:
    Octree(const HittableList &world,
           int maxDepth = 8,
           int maxObjectsPerLeaf = 4);

    bool hit(const Ray &ray,
             double tMin,
             double tMax,
             HitRecord &rec) const override;

    bool anyHit(const Ray &ray,
                double tMin,
                double tMax) const override;

    bool boundingBox(AABB &outputBox) const override;

private:
    std::unique_ptr<OctreeNode> root;

    int maxDepth;
    int maxObjectsPerLeaf;

    AABB rootBounds;
    bool hasRootBounds = false;

    std::unique_ptr<OctreeNode> buildNode(
        const AABB &bounds,
        const std::vector<OctreeObject> &objects,
        int depth) const;

    void createChildBounds(
        const AABB &bounds,
        AABB childBounds[8]) const;

    bool hitNode(
        const OctreeNode *node,
        const Ray &ray,
        double tMin,
        double tMax,
        HitRecord &rec) const;

    bool anyHitNode(
        const OctreeNode *node,
        const Ray &ray,
        double tMin,
        double tMax) const;
};