// Hittable.h
#pragma once

#include "Ray.h"
#include "HitRecord.h"

struct Hittable
{
    virtual ~Hittable() = default;

    virtual bool hit(const Ray &ray, double tMin, double tMax, HitRecord &rec) const = 0;
};