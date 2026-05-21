#pragma once

#include "core/Vec3.h"

struct Ray
{
    Vec3 origin;
    Vec3 direction;

    Ray() : origin(), direction(0, 0, 1) {}

    Ray(const Vec3 &origin, const Vec3 &direction)
        : origin(origin), direction(direction) {}

    Vec3 at(double t) const
    {
        return origin + direction * t;
    }
};