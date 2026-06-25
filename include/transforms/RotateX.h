#pragma once

#include <memory>

#include "acceleration/AABB.h"
#include "core/Ray.h"
#include "hittable/Hittable.h"
#include "hittable/HitRecord.h"

class RotateX : public Hittable
{
public:
    std::shared_ptr<Hittable> object;

    double sinTheta;
    double cosTheta;

    RotateX(std::shared_ptr<Hittable> object,
            double angle);

    bool anyHit(const Ray &ray,
                double tMin,
                double tMax) const override;

    bool hit(const Ray &ray,
             double tMin,
             double tMax,
             HitRecord &rec) const override;

    bool boundingBox(AABB &box) const override;
};
