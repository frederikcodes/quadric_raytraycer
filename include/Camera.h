#pragma once

#include <cmath>

#include "Ray.h"
#include "Vec3.h"

struct Camera
{
    Vec3 origin;
    Vec3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;

    Camera(const Vec3 &lookFrom,
           const Vec3 &lookAt,
           const Vec3 &viewUp,
           double verticalFovDegrees,
           double aspectRatio)
    {
        const double pi = 3.1415926535897932385;
        const double theta = verticalFovDegrees * pi / 180.0;
        const double viewportHeight = 2.0 * std::tan(theta / 2.0);
        const double viewportWidth = aspectRatio * viewportHeight;

        const Vec3 w = (lookFrom - lookAt).normalized();
        const Vec3 u = cross(viewUp, w).normalized();
        const Vec3 v = cross(w, u);

        origin = lookFrom;
        horizontal = u * viewportWidth;
        vertical = v * viewportHeight;
        lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - w;
    }

    Ray getRay(double s, double t) const
    {
        Vec3 direction =
            (lowerLeftCorner + horizontal * s + vertical * t - origin).normalized();

        return Ray(origin, direction);
    }
};
