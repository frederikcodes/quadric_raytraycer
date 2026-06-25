#pragma once

#include "core/Ray.h"
#include "core/Vec3.h"

class AABB
{
public:
    Vec3 minimum;
    Vec3 maximum;

    AABB();

    AABB(const Vec3 &minimum,
         const Vec3 &maximum);

    Vec3 center() const;

    bool overlaps(const AABB &other) const;

    bool hit(const Ray &ray,
             double tMin,
             double tMax) const;
};

AABB surroundingBox(const AABB &a,
                    const AABB &b);
