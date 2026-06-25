#pragma once

#include "core/Ray.h"
#include "core/Vec3.h"

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
           double aspectRatio);

    Ray getRay(double s, double t) const;
};
