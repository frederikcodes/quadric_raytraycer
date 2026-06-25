#pragma once

class AABB;
struct HitRecord;
struct Ray;

class Hittable
{
public:
    virtual ~Hittable();

    virtual bool hit(const Ray &ray,
                     double tMin,
                     double tMax,
                     HitRecord &rec) const = 0;

    virtual bool boundingBox(AABB &box) const = 0;
    virtual bool anyHit(const Ray &ray,
                        double tMin,
                        double tMax) const = 0;
};
