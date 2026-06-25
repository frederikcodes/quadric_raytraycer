#pragma once

#include "acceleration/AABB.h"
#include "core/Material.h"
#include "core/Ray.h"
#include "hittable/Hittable.h"
#include "hittable/HitRecord.h"

class Ellipsoid : public Hittable
{
public:
    Vec3 center;
    Vec3 radii;

    Material material;

    Ellipsoid(const Vec3 &center,
              const Vec3 &radii,
              const Material &material);

    bool hit(const Ray &ray,
             double tMin,
             double tMax,
             HitRecord &rec) const override;

    bool boundingBox(AABB &box) const override;

    bool anyHit(const Ray &ray,
                double tMin,
                double tMax) const override;
};
