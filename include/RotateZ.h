#pragma once

#include <cmath>
#include <memory>

#include "Hittable.h"
#include "Ray.h"
#include "Vec3.h"

class RotateZ : public Hittable
{
public:
    std::shared_ptr<Hittable> object;

    double sinTheta;
    double cosTheta;

    RotateZ(std::shared_ptr<Hittable> object,
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

        origin.x =
            cosTheta * ray.origin.x -
            sinTheta * ray.origin.y;

        origin.y =
            sinTheta * ray.origin.x +
            cosTheta * ray.origin.y;

        direction.x =
            cosTheta * ray.direction.x -
            sinTheta * ray.direction.y;

        direction.y =
            sinTheta * ray.direction.x +
            cosTheta * ray.direction.y;

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

        point.x =
            cosTheta * rec.point.x +
            sinTheta * rec.point.y;

        point.y =
            -sinTheta * rec.point.x +
            cosTheta * rec.point.y;

        normal.x =
            cosTheta * rec.normal.x +
            sinTheta * rec.normal.y;

        normal.y =
            -sinTheta * rec.normal.x +
            cosTheta * rec.normal.y;

        rec.point = point;
        rec.normal = normal.normalized();

        return true;
    }
};