#pragma once

#include "core/Vec3.h"

class Material
{
public:
    Vec3 color;

    double ambient;
    double diffuse;
    double specular;
    double shininess;

    double reflectivity;

    Material()
        : color(1.0, 1.0, 1.0),
          ambient(0.1),
          diffuse(1.0),
          specular(0.0),
          shininess(32.0),
          reflectivity(0.0)
    {
    }

    Material(const Vec3 &color,
             double ambient,
             double diffuse,
             double specular,
             double shininess,
             double reflectivity = 0.0)
        : color(color),
          ambient(ambient),
          diffuse(diffuse),
          specular(specular),
          shininess(shininess),
          reflectivity(reflectivity)
    {
    }
};