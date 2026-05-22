#pragma once

#include <memory>

#include "hittable/Hittable.h"
#include "core/Ray.h"
#include "core/Vec3.h"

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
    bool anyHit(const Ray &ray,
                double tMin,
                double tMax) const override
    {
        HitRecord rec;
        return hit(ray, tMin, tMax, rec);
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

    bool boundingBox(AABB &box) const override
    {
        if (!object->boundingBox(box))
        {
            return false;
        }

        box = AABB(box.minimum + offset, box.maximum + offset);

        return true;
    }
};
