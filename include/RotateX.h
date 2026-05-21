#pragma once

#include <cmath>
#include <memory>

#include "Hittable.h"
#include "Ray.h"
#include "Vec3.h"

class RotateX : public Hittable
{
public:
    std::shared_ptr<Hittable> object;

    double sinTheta;
    double cosTheta;

    RotateX(std::shared_ptr<Hittable> object,
            double angle)
        : object(object)
    {
        const double pi =
            3.1415926535897932385;

        double radians =
            angle * pi / 180.0;

        sinTheta = std::sin(radians);
        cosTheta = std::cos(radians);
    }

    bool hit(const Ray &ray,
             double tMin,
             double tMax,
             HitRecord &rec) const override
    {
        Vec3 origin = ray.origin;
        Vec3 direction = ray.direction;

        // inverse rotation

        origin.y =
            cosTheta * ray.origin.y -
            sinTheta * ray.origin.z;

        origin.z =
            sinTheta * ray.origin.y +
            cosTheta * ray.origin.z;

        direction.y =
            cosTheta * ray.direction.y -
            sinTheta * ray.direction.z;

        direction.z =
            sinTheta * ray.direction.y +
            cosTheta * ray.direction.z;

        Ray rotatedRay(origin, direction);

        if (!object->hit(rotatedRay,
                         tMin,
                         tMax,
                         rec))
        {
            return false;
        }

        Vec3 point = rec.point;
        Vec3 normal = rec.normal;

        // forward rotation

        point.y =
            cosTheta * rec.point.y +
            sinTheta * rec.point.z;

        point.z =
            -sinTheta * rec.point.y +
            cosTheta * rec.point.z;

        normal.y =
            cosTheta * rec.normal.y +
            sinTheta * rec.normal.z;

        normal.z =
            -sinTheta * rec.normal.y +
            cosTheta * rec.normal.z;

        rec.point = point;
        rec.normal = normal.normalized();

        return true;
    }
};