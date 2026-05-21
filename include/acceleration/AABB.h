#pragma once

#include <algorithm>

#include "core/Ray.h"
#include "core/Vec3.h"

class AABB
{
public:
    Vec3 minimum;
    Vec3 maximum;

    AABB() = default;

    AABB(const Vec3 &minimum,
         const Vec3 &maximum)
        : minimum(minimum),
          maximum(maximum)
    {
    }

    bool hit(const Ray &ray,
             double tMin,
             double tMax) const
    {
        for (int axis = 0; axis < 3; axis++)
        {
            double origin;
            double direction;
            double minValue;
            double maxValue;

            if (axis == 0)
            {
                origin = ray.origin.x;
                direction = ray.direction.x;
                minValue = minimum.x;
                maxValue = maximum.x;
            }
            else if (axis == 1)
            {
                origin = ray.origin.y;
                direction = ray.direction.y;
                minValue = minimum.y;
                maxValue = maximum.y;
            }
            else
            {
                origin = ray.origin.z;
                direction = ray.direction.z;
                minValue = minimum.z;
                maxValue = maximum.z;
            }

            double invD = 1.0 / direction;

            double t0 = (minValue - origin) * invD;
            double t1 = (maxValue - origin) * invD;

            if (invD < 0.0)
            {
                std::swap(t0, t1);
            }

            tMin = std::max(tMin, t0);
            tMax = std::min(tMax, t1);

            if (tMax <= tMin)
            {
                return false;
            }
        }

        return true;
    }
};