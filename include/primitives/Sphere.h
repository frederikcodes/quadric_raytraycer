#pragma once

#include "acceleration/AABB.h"
#include "core/Material.h"
#include "core/Ray.h"
#include "hittable/Hittable.h"
#include "hittable/HitRecord.h"

struct Sphere : public Hittable
{
    Vec3 center;
    double radius;

    Material material;

    Sphere(
        const Vec3 &center,
        double radius,
        const Material &material);

    bool hit(
        const Ray &ray,
        double tMin,
        double tMax,
        HitRecord &rec) const override;

    bool boundingBox(AABB &box) const override;

    bool anyHit(const Ray &ray,
                double tMin,
                double tMax) const override;
};
