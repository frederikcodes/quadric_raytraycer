#pragma once

struct Vec3
{
    double x;
    double y;
    double z;

    Vec3();

    Vec3(double x, double y, double z);

    Vec3 operator+(const Vec3 &other) const;

    Vec3 operator-(const Vec3 &other) const;

    Vec3 operator*(double scalar) const;

    Vec3 operator/(double scalar) const;

    Vec3 operator-() const;

    double length() const;

    Vec3 normalized() const;
};

double dot(const Vec3 &a, const Vec3 &b);

Vec3 cross(const Vec3 &a, const Vec3 &b);

Vec3 reflect(const Vec3 &direction, const Vec3 &normal);
