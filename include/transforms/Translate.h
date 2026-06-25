#pragma once

#include <memory>

#include "acceleration/AABB.h"
#include "core/Ray.h"
#include "core/Vec3.h"
#include "hittable/Hittable.h"
#include "hittable/HitRecord.h"

class Translate : public Hittable
{
public:
    std::shared_ptr<Hittable> object;
    Vec3 offset;

    Translate(std::shared_ptr<Hittable> object,
              const Vec3 &offset);

    bool anyHit(const Ray &ray,
                double tMin,
                double tMax) const override;

    bool hit(const Ray &ray,
             double tMin,
             double tMax,
             HitRecord &rec) const override;

    bool boundingBox(AABB &box) const override;
};
