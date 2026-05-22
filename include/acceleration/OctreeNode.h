#pragma once

#include <memory>
#include <vector>

#include "acceleration/AABB.h"
#include "hittable/Hittable.h"

struct OctreeObject
{
    std::shared_ptr<Hittable> object;
    AABB box;
};

struct OctreeNode
{
    AABB bounds;

    std::vector<OctreeObject> objects;

    std::unique_ptr<OctreeNode> children[8];

    bool isLeaf() const
    {
        for (int i = 0; i < 8; i++)
        {
            if (children[i])
            {
                return false;
            }
        }

        return true;
    }
};