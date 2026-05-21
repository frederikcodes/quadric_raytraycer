#pragma once

#include "Vec3.h"

struct Material
{
    Vec3 color;

    double ambient;
    double diffuse;

    double specular;
    double shininess;

    Material(
        const Vec3 &color = Vec3(1, 1, 1),
        double ambient = 0.1,
        double diffuse = 1.0,
        double specular = 0.0,
        double shininess = 32.0)
        : color(color),
          ambient(ambient),
          diffuse(diffuse),
          specular(specular),
          shininess(shininess)
    {
    }
};