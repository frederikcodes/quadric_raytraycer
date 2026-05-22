#pragma once

#include <cmath>

#include "hittable/Hittable.h"
#include "core/Material.h"

struct Sphere : public Hittable
{
    Vec3 center;
    double radius;

    Material material;

    Sphere(
        const Vec3 &center,
        double radius,
        const Material &material)
        : center(center),
          radius(radius),
          material(material)
    {
    }

    bool hit(
        const Ray &ray,
        double tMin,
        double tMax,
        HitRecord &rec) const override
    {
        Vec3 oc = ray.origin - center;

        double a = dot(ray.direction, ray.direction);

        double b =
            2.0 * dot(oc, ray.direction);

        double c =
            dot(oc, oc) - radius * radius;

        double discriminant =
            b * b - 4.0 * a * c;

        if (discriminant < 0.0)
        {
            return false;
        }

        double sqrtD = std::sqrt(discriminant);

        double root =
            (-b - sqrtD) / (2.0 * a);

        if (root < tMin || root > tMax)
        {
            root =
                (-b + sqrtD) / (2.0 * a);

            if (root < tMin || root > tMax)
            {
                return false;
            }
        }

        rec.t = root;

        rec.point =
            ray.at(rec.t);

        rec.normal =
            (rec.point - center) / radius;

        rec.material = material;

        return true;
    }

    bool boundingBox(AABB &box) const override
    {
        Vec3 radiusVec(radius, radius, radius);
        box = AABB(center - radiusVec, center + radiusVec);

        return true;
    }
    bool anyHit(const Ray &ray,
                double tMin,
                double tMax) const override
    {
        HitRecord rec;
        return hit(ray, tMin, tMax, rec);
    }
};
