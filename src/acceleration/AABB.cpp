#include "acceleration/AABB.h"

#include <algorithm>

AABB::AABB() = default;

AABB::AABB(const Vec3 &minimum,
           const Vec3 &maximum)
    : minimum(minimum),
      maximum(maximum)
{
}

Vec3 AABB::center() const
{
    return (minimum + maximum) * 0.5;
}

bool AABB::overlaps(const AABB &other) const
{
    return minimum.x <= other.maximum.x &&
           maximum.x >= other.minimum.x &&

           minimum.y <= other.maximum.y &&
           maximum.y >= other.minimum.y &&

           minimum.z <= other.maximum.z &&
           maximum.z >= other.minimum.z;
}

bool AABB::hit(const Ray &ray,
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

AABB surroundingBox(const AABB &a,
                    const AABB &b)
{
    Vec3 small(
        std::min(a.minimum.x, b.minimum.x),
        std::min(a.minimum.y, b.minimum.y),
        std::min(a.minimum.z, b.minimum.z));

    Vec3 big(
        std::max(a.maximum.x, b.maximum.x),
        std::max(a.maximum.y, b.maximum.y),
        std::max(a.maximum.z, b.maximum.z));

    return AABB(small, big);
}
