#include "primitives/Cone.h"

#include <cmath>

Cone::Cone(const Vec3 &baseCenter,
           double height,
           double radius,
           const Material &material)
    : baseCenter(baseCenter),
      height(height),
      radius(radius),
      material(material)
{
}

bool Cone::hit(const Ray &ray,
               double tMin,
               double tMax,
               HitRecord &rec) const
{
    Vec3 oc = ray.origin - baseCenter;

    const double k = radius / height;
    const double k2 = k * k;

    const double dx = ray.direction.x;
    const double dy = ray.direction.y;
    const double dz = ray.direction.z;

    const double ox = oc.x;
    const double oy = oc.y;
    const double oz = oc.z;

    /*
        Cone equation:

        x² + z² = k²(height - y)²
    */

    double a =
        dx * dx +
        dz * dz -
        k2 * dy * dy;

    double b =
        2.0 * (ox * dx + oz * dz) +
        2.0 * k2 * (height - oy) * dy;

    double c =
        ox * ox +
        oz * oz -
        k2 * (height - oy) * (height - oy);

    double discriminant = b * b - 4.0 * a * c;

    bool hitAnything = false;

    double closestT = tMax;

    HitRecord tempRec;

    // =========================
    // Mantel
    // =========================

    if (discriminant >= 0.0)
    {
        double sqrtD = std::sqrt(discriminant);

        double t1 = (-b - sqrtD) / (2.0 * a);
        double t2 = (-b + sqrtD) / (2.0 * a);

        double candidates[2] = {t1, t2};

        for (double t : candidates)
        {
            if (t < tMin || t > closestT)
            {
                continue;
            }

            Vec3 p = ray.at(t);

            Vec3 local = p - baseCenter;

            // innerhalb der Höhe?
            if (local.y < 0.0 || local.y > height)
            {
                continue;
            }

            tempRec.t = t;
            tempRec.point = p;

            /*
                Gradient von:

                F(x,y,z) =
                x² + z² - k²(height - y)²
            */

            Vec3 normal(
                local.x,
                k2 * (height - local.y),
                local.z);

            tempRec.normal = normal.normalized();

            tempRec.material = material;

            closestT = t;
            hitAnything = true;
        }
    }

    // =========================
    // Boden
    // =========================

    if (std::abs(ray.direction.y) > 1e-8)
    {
        double t =
            (baseCenter.y - ray.origin.y) / ray.direction.y;

        if (t >= tMin && t <= closestT)
        {
            Vec3 p = ray.at(t);

            Vec3 d = p - baseCenter;

            double dist2 =
                d.x * d.x +
                d.z * d.z;

            if (dist2 <= radius * radius)
            {
                tempRec.t = t;
                tempRec.point = p;

                tempRec.normal = Vec3(0, -1, 0);

                tempRec.material = material;

                closestT = t;
                hitAnything = true;
            }
        }
    }

    if (hitAnything)
    {
        rec = tempRec;
    }

    return hitAnything;
}

bool Cone::boundingBox(AABB &box) const
{
    Vec3 minimum(
        baseCenter.x - radius,
        baseCenter.y,
        baseCenter.z - radius);

    Vec3 maximum(
        baseCenter.x + radius,
        baseCenter.y + height,
        baseCenter.z + radius);

    box = AABB(minimum, maximum);

    return true;
}

bool Cone::anyHit(const Ray &ray,
                  double tMin,
                  double tMax) const
{
    HitRecord rec;
    return hit(ray, tMin, tMax, rec);
}
