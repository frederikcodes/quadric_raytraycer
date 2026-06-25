#include "primitives/Ellipsoid.h"

#include <cmath>

Ellipsoid::Ellipsoid(const Vec3 &center,
                     const Vec3 &radii,
                     const Material &material)
    : center(center),
      radii(radii),
      material(material)
{
}

bool Ellipsoid::hit(const Ray &ray,
                    double tMin,
                    double tMax,
                    HitRecord &rec) const
{
    Vec3 oc = ray.origin - center;

    double rx2 = radii.x * radii.x;
    double ry2 = radii.y * radii.y;
    double rz2 = radii.z * radii.z;

    double a =
        (ray.direction.x * ray.direction.x) / rx2 +
        (ray.direction.y * ray.direction.y) / ry2 +
        (ray.direction.z * ray.direction.z) / rz2;

    double b =
        2.0 * ((oc.x * ray.direction.x) / rx2 +
               (oc.y * ray.direction.y) / ry2 +
               (oc.z * ray.direction.z) / rz2);

    double c =
        (oc.x * oc.x) / rx2 +
        (oc.y * oc.y) / ry2 +
        (oc.z * oc.z) / rz2 -
        1.0;

    double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0.0)
    {
        return false;
    }

    double sqrtD = std::sqrt(discriminant);

    double t =
        (-b - sqrtD) / (2.0 * a);

    if (t < tMin || t > tMax)
    {
        t =
            (-b + sqrtD) / (2.0 * a);

        if (t < tMin || t > tMax)
        {
            return false;
        }
    }

    rec.t = t;

    rec.point = ray.at(t);

    Vec3 p = rec.point - center;

    Vec3 outwardNormal(
        p.x / rx2,
        p.y / ry2,
        p.z / rz2);

    rec.normal = outwardNormal.normalized();

    rec.material = material;

    return true;
}

bool Ellipsoid::boundingBox(AABB &box) const
{
    box = AABB(center - radii, center + radii);

    return true;
}

bool Ellipsoid::anyHit(const Ray &ray,
                       double tMin,
                       double tMax) const
{
    HitRecord rec;
    return hit(ray, tMin, tMax, rec);
}
