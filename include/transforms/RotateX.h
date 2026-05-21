#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>

#include "hittable/Hittable.h"
#include "core/Ray.h"
#include "core/Vec3.h"

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
        const double pi = 3.1415926535897932385;
        double radians = angle * pi / 180.0;

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

        // Inverse rotation around x-axis
        double oy = ray.origin.y;
        double oz = ray.origin.z;

        origin.y = cosTheta * oy - sinTheta * oz;
        origin.z = sinTheta * oy + cosTheta * oz;

        double dy = ray.direction.y;
        double dz = ray.direction.z;

        direction.y = cosTheta * dy - sinTheta * dz;
        direction.z = sinTheta * dy + cosTheta * dz;

        Ray rotatedRay(origin, direction);

        if (!object->hit(rotatedRay, tMin, tMax, rec))
        {
            return false;
        }

        Vec3 point = rec.point;
        Vec3 normal = rec.normal;

        // Forward rotation around x-axis
        double py = rec.point.y;
        double pz = rec.point.z;

        point.y = cosTheta * py + sinTheta * pz;
        point.z = -sinTheta * py + cosTheta * pz;

        double ny = rec.normal.y;
        double nz = rec.normal.z;

        normal.y = cosTheta * ny + sinTheta * nz;
        normal.z = -sinTheta * ny + cosTheta * nz;

        rec.point = point;
        rec.normal = normal.normalized();

        return true;
    }

    bool boundingBox(AABB &box) const override
    {
        AABB childBox;

        if (!object->boundingBox(childBox))
        {
            return false;
        }

        Vec3 minimum(
            std::numeric_limits<double>::infinity(),
            std::numeric_limits<double>::infinity(),
            std::numeric_limits<double>::infinity());

        Vec3 maximum(
            -std::numeric_limits<double>::infinity(),
            -std::numeric_limits<double>::infinity(),
            -std::numeric_limits<double>::infinity());

        for (int x = 0; x < 2; x++)
        {
            for (int y = 0; y < 2; y++)
            {
                for (int z = 0; z < 2; z++)
                {
                    Vec3 corner(
                        x ? childBox.maximum.x : childBox.minimum.x,
                        y ? childBox.maximum.y : childBox.minimum.y,
                        z ? childBox.maximum.z : childBox.minimum.z);

                    double cy = corner.y;
                    double cz = corner.z;

                    corner.y = cosTheta * cy + sinTheta * cz;
                    corner.z = -sinTheta * cy + cosTheta * cz;

                    minimum.x = std::min(minimum.x, corner.x);
                    minimum.y = std::min(minimum.y, corner.y);
                    minimum.z = std::min(minimum.z, corner.z);

                    maximum.x = std::max(maximum.x, corner.x);
                    maximum.y = std::max(maximum.y, corner.y);
                    maximum.z = std::max(maximum.z, corner.z);
                }
            }
        }

        box = AABB(minimum, maximum);

        return true;
    }
};
