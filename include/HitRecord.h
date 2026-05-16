// HitRecord.h
#pragma once

#include "Vec3.h"

struct HitRecord
{
    Vec3 point;
    Vec3 normal;
    Vec3 color;

    double t;
};