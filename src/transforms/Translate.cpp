#include "transforms/Translate.h"

Translate::Translate(std::shared_ptr<Hittable> object,
                     const Vec3 &offset)
    : object(object),
      offset(offset)
{
}

bool Translate::anyHit(const Ray &ray,
                       double tMin,
                       double tMax) const
{
    HitRecord rec;
    return hit(ray, tMin, tMax, rec);
}

bool Translate::hit(const Ray &ray,
                    double tMin,
                    double tMax,
                    HitRecord &rec) const
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

bool Translate::boundingBox(AABB &box) const
{
    if (!object->boundingBox(box))
    {
        return false;
    }

    box = AABB(box.minimum + offset, box.maximum + offset);

    return true;
}
