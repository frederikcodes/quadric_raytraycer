#pragma once

#include "core/Vec3.h"
#include "core/Material.h"

struct HitRecord
{
    Vec3 point;
    Vec3 normal;

    Material material;

    double t;
};