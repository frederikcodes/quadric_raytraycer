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

    Material();

    Material(const Vec3 &color,
             double ambient,
             double diffuse,
             double specular,
             double shininess,
             double reflectivity = 0.0);
};
