#pragma once

#include "acceleration/AABB.h"
#include "core/Material.h"
#include "core/Ray.h"
#include "core/Vec3.h"

struct HitRecord
{
    Vec3 point;
    Vec3 normal;
    Material material;
    double t;
};

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray &ray,
                     double tMin,
                     double tMax,
                     HitRecord &rec) const = 0;

    virtual bool boundingBox(AABB &box) const = 0;
    virtual bool anyHit(const Ray &ray,
                        double tMin,
                        double tMax) const = 0;
};