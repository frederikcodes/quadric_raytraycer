#include "primitives/Sphere.h"

#include <cmath>

Sphere::Sphere(
    const Vec3 &center,
    double radius,
    const Material &material)
    : center(center),
      radius(radius),
      material(material)
{
}

bool Sphere::hit(
    const Ray &ray,
    double tMin,
    double tMax,
    HitRecord &rec) const
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

bool Sphere::boundingBox(AABB &box) const
{
    Vec3 radiusVec(radius, radius, radius);
    box = AABB(center - radiusVec, center + radiusVec);

    return true;
}

bool Sphere::anyHit(const Ray &ray,
                    double tMin,
                    double tMax) const
{
    HitRecord rec;
    return hit(ray, tMin, tMax, rec);
}
