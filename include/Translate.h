#pragma once

#include <memory>

#include "Hittable.h"
#include "Ray.h"
#include "Vec3.h"

class Translate : public Hittable
{
public:
    std::shared_ptr<Hittable> object;
    Vec3 offset;

    Translate(std::shared_ptr<Hittable> object,
              const Vec3 &offset)
        : object(object),
          offset(offset)
    {
    }

    bool hit(const Ray &ray,
             double tMin,
             double tMax,
             HitRecord &rec) const override
    {
        Ray movedRay(
            ray.origin - offset,
            ray.direction);

        if (!object->hit(movedRay, tMin, tMax, rec))
        {
            return false;
        }

        rec.point = rec.point + offset;

        return true;
    }
};