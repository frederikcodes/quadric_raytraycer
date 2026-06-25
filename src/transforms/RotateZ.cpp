#include "transforms/RotateZ.h"

#include <algorithm>
#include <cmath>
#include <limits>

RotateZ::RotateZ(std::shared_ptr<Hittable> object,
                 double angle)
    : object(object)
{
    const double pi = 3.1415926535897932385;
    double radians = angle * pi / 180.0;

    sinTheta = std::sin(radians);
    cosTheta = std::cos(radians);
}

bool RotateZ::anyHit(const Ray &ray,
                     double tMin,
                     double tMax) const
{
    HitRecord rec;
    return hit(ray, tMin, tMax, rec);
}

bool RotateZ::hit(const Ray &ray,
                  double tMin,
                  double tMax,
                  HitRecord &rec) const
{
    Vec3 origin = ray.origin;
    Vec3 direction = ray.direction;

    // Inverse rotation around z-axis
    double ox = ray.origin.x;
    double oy = ray.origin.y;

    origin.x = cosTheta * ox - sinTheta * oy;
    origin.y = sinTheta * ox + cosTheta * oy;

    double dx = ray.direction.x;
    double dy = ray.direction.y;

    direction.x = cosTheta * dx - sinTheta * dy;
    direction.y = sinTheta * dx + cosTheta * dy;

    Ray rotatedRay(origin, direction);

    if (!object->hit(rotatedRay, tMin, tMax, rec))
    {
        return false;
    }

    Vec3 point = rec.point;
    Vec3 normal = rec.normal;

    // Forward rotation around z-axis
    double px = rec.point.x;
    double py = rec.point.y;

    point.x = cosTheta * px + sinTheta * py;
    point.y = -sinTheta * px + cosTheta * py;

    double nx = rec.normal.x;
    double ny = rec.normal.y;

    normal.x = cosTheta * nx + sinTheta * ny;
    normal.y = -sinTheta * nx + cosTheta * ny;

    rec.point = point;
    rec.normal = normal.normalized();

    return true;
}

bool RotateZ::boundingBox(AABB &box) const
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

                double cx = corner.x;
                double cy = corner.y;

                corner.x = cosTheta * cx + sinTheta * cy;
                corner.y = -sinTheta * cx + cosTheta * cy;

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
