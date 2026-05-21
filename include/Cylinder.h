#pragma once

#include <cmath>

#include "Hittable.h"
#include "Material.h"

struct Cylinder : public Hittable
{
    Vec3 center;
    double radius;
    double height;

    Material material;

    Cylinder(
        const Vec3 &center,
        double radius,
        double height,
        const Material &material)
        : center(center),
          radius(radius),
          height(height),
          material(material)
    {
    }

    bool hit(
        const Ray &ray,
        double tMin,
        double tMax,
        HitRecord &rec) const override
    {
        const double epsilon = 1e-12;
        const double yMin = center.y - height / 2.0;
        const double yMax = center.y + height / 2.0;

        bool hitAnything = false;
        double closestSoFar = tMax;
        HitRecord tempRec;

        Vec3 oc = ray.origin - center;

        double dx = ray.direction.x;
        double dz = ray.direction.z;

        double a = dx * dx + dz * dz;
        double b = 2.0 * (oc.x * dx + oc.z * dz);
        double c = oc.x * oc.x + oc.z * oc.z - radius * radius;

        // Mantel
        if (a > epsilon)
        {
            double discriminant = b * b - 4.0 * a * c;

            if (discriminant >= 0.0)
            {
                double sqrtD = std::sqrt(discriminant);

                double root = (-b - sqrtD) / (2.0 * a);

                if (root >= tMin && root < closestSoFar)
                {
                    Vec3 hitPoint = ray.at(root);

                    if (hitPoint.y >= yMin && hitPoint.y <= yMax)
                    {
                        tempRec.t = root;
                        tempRec.point = hitPoint;
                        tempRec.normal =
                            Vec3(
                                hitPoint.x - center.x,
                                0,
                                hitPoint.z - center.z)
                                .normalized();

                        tempRec.material = material;

                        rec = tempRec;
                        closestSoFar = root;
                        hitAnything = true;
                    }
                }

                root = (-b + sqrtD) / (2.0 * a);

                if (root >= tMin && root < closestSoFar)
                {
                    Vec3 hitPoint = ray.at(root);

                    if (hitPoint.y >= yMin && hitPoint.y <= yMax)
                    {
                        tempRec.t = root;
                        tempRec.point = hitPoint;
                        tempRec.normal =
                            Vec3(
                                hitPoint.x - center.x,
                                0,
                                hitPoint.z - center.z)
                                .normalized();

                        tempRec.material = material;

                        rec = tempRec;
                        closestSoFar = root;
                        hitAnything = true;
                    }
                }
            }
        }

        // Deckel
        if (std::fabs(ray.direction.y) > epsilon)
        {
            double t = (yMin - ray.origin.y) / ray.direction.y;

            if (t >= tMin && t < closestSoFar)
            {
                Vec3 hitPoint = ray.at(t);

                double capDx = hitPoint.x - center.x;
                double capDz = hitPoint.z - center.z;

                if (capDx * capDx + capDz * capDz <= radius * radius)
                {
                    tempRec.t = t;
                    tempRec.point = hitPoint;
                    tempRec.normal = Vec3(0, -1, 0);
                    tempRec.material = material;

                    rec = tempRec;
                    closestSoFar = t;
                    hitAnything = true;
                }
            }

            t = (yMax - ray.origin.y) / ray.direction.y;

            if (t >= tMin && t < closestSoFar)
            {
                Vec3 hitPoint = ray.at(t);

                double capDx = hitPoint.x - center.x;
                double capDz = hitPoint.z - center.z;

                if (capDx * capDx + capDz * capDz <= radius * radius)
                {
                    tempRec.t = t;
                    tempRec.point = hitPoint;
                    tempRec.normal = Vec3(0, 1, 0);
                    tempRec.material = material;

                    rec = tempRec;
                    closestSoFar = t;
                    hitAnything = true;
                }
            }
        }

        return hitAnything;
    }
};