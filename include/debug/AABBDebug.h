#pragma once

#include <memory>

#include "acceleration/AABB.h"
#include "hittable/HittableList.h"
#include "primitives/Sphere.h"

inline void addAABBCorners(HittableList &world,
                           const AABB &box,
                           const Material &mat,
                           double radius = 0.035)
{
    for (int x = 0; x < 2; x++)
    {
        for (int y = 0; y < 2; y++)
        {
            for (int z = 0; z < 2; z++)
            {
                Vec3 p(
                    x ? box.maximum.x : box.minimum.x,
                    y ? box.maximum.y : box.minimum.y,
                    z ? box.maximum.z : box.minimum.z);

                world.add(
                    std::make_shared<Sphere>(
                        p,
                        radius,
                        mat));
            }
        }
    }
}