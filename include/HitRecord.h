#pragma once

#include "Vec3.h"
#include "Material.h"

struct HitRecord
{
    Vec3 point;
    Vec3 normal;

    Material material;

    double t;
};