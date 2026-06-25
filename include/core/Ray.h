#pragma once

#include "core/Vec3.h"

struct Ray
{
    Vec3 origin;
    Vec3 direction;

    Ray();

    Ray(const Vec3 &origin, const Vec3 &direction);

    Vec3 at(double t) const;
};
