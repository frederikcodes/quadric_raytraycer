#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

#include "Ellipsoid.h"
#include "Camera.h"
#include "Cylinder.h"
#include "HittableList.h"
#include "Material.h"
#include "Ray.h"
#include "Sphere.h"
#include "Vec3.h"

const Vec3 lightPos(3, 5, 3);
const double infinity = std::numeric_limits<double>::infinity();

Vec3 rayColor(const Ray &ray, const Hittable &world)
{
    HitRecord rec;

    if (world.hit(ray, 0.001, infinity, rec))
    {
        Vec3 lightDir = lightPos - rec.point;
        double lightDistance = lightDir.length();
        lightDir = lightDir / lightDistance;

        Ray shadowRay(
            rec.point + rec.normal * 0.001,
            lightDir);

        HitRecord shadowRec;

        bool inShadow = world.hit(
            shadowRay,
            0.001,
            lightDistance - 0.001,
            shadowRec);

        Vec3 ambient =
            rec.material.color * rec.material.ambient;

        Vec3 diffuseColor(0.0, 0.0, 0.0);
        Vec3 specularColor(0.0, 0.0, 0.0);

        if (!inShadow)
        {
            double lambert =
                std::max(0.0, dot(rec.normal, lightDir));

            diffuseColor =
                rec.material.color *
                rec.material.diffuse *
                lambert;

            Vec3 viewDir =
                (ray.direction * -1.0).normalized();

            Vec3 reflectDir =
                (rec.normal * 2.0 * dot(rec.normal, lightDir) - lightDir).normalized();

            double spec =
                std::pow(
                    std::max(0.0, dot(viewDir, reflectDir)),
                    rec.material.shininess);

            specularColor =
                Vec3(1.0, 1.0, 1.0) *
                rec.material.specular *
                spec;
        }

        return ambient + diffuseColor + specularColor;
    }

    Vec3 unitDir = ray.direction.normalized();
    double t = 0.5 * (unitDir.y + 1.0);

    return Vec3(1.0, 1.0, 1.0) * (1.0 - t) + Vec3(0.5, 0.7, 1.0) * t;
}

int main()
{
    const int imageWidth = 800;
    const int imageHeight = 450;

    const int samplesPerPixel = 64;

    Camera cam(
        Vec3(2, 2, 4),
        Vec3(0, 0, 0),
        Vec3(0, 1, 0),
        45.0,
        double(imageWidth) / imageHeight);

    Material blueMat(
        Vec3(0.2, 0.4, 1.0),
        0.1,
        0.9,
        0.6,
        64.0);

    Material greenMat(
        Vec3(0.2, 1.0, 0.3),
        0.1,
        1.0,
        0.5,
        32.0);

    Material shinyRedMat(
        Vec3(1.0, 0.15, 0.12),
        0.05,
        0.85,
        1.2,
        160.0);

    Material matteGreenMat(
        Vec3(0.2, 0.8, 0.3),
        0.15,
        0.9,
        0.1,
        12.0);

    Material shinyYellowMat(
        Vec3(1.0, 0.85, 0.15),
        0.05,
        0.9,
        1.4,
        220.0);

    Material glossyPurpleMat(
        Vec3(0.6, 0.25, 0.9),
        0.08,
        0.85,
        1.0,
        120.0);

    Material groundMat(
        Vec3(0.8, 0.8, 0.8),
        0.12,
        0.9,
        0.05,
        8.0);

    HittableList world;

    world.add(
        std::make_shared<Cylinder>(
            Vec3(0.0, 0.0, 0.0),
            0.5,
            1.0,
            blueMat));

    world.add(std::make_shared<Ellipsoid>(
        Vec3(-1.5, 0.0, -3.0),
        Vec3(0.5, 1.0, 0.5),
        greenMat));

    world.add(
        std::make_shared<Sphere>(
            Vec3(-1.2, 0.0, -0.8),
            0.45,
            shinyRedMat));

    world.add(
        std::make_shared<Sphere>(
            Vec3(1.2, 0.0, -1.0),
            0.45,
            matteGreenMat));

    world.add(
        std::make_shared<Cylinder>(
            Vec3(-1.8, 0.0, -2.4),
            0.35,
            0.9,
            shinyYellowMat));

    world.add(
        std::make_shared<Sphere>(
            Vec3(1.8, 0.2, -2.6),
            0.6,
            glossyPurpleMat));

    world.add(
        std::make_shared<Sphere>(
            Vec3(0.0, 0.15, -3.5),
            0.75,
            shinyYellowMat));

    world.add(
        std::make_shared<Sphere>(
            Vec3(0, -100.5, 0),
            100.0,
            groundMat));

    std::ofstream out("../images/output.ppm");

    out << "P3\n";
    out << imageWidth << " "
        << imageHeight << "\n";

    out << "255\n";

    std::mt19937 rng(std::random_device{}());

    std::uniform_real_distribution<double>
        dist(0.0, 1.0);

    for (int y = imageHeight - 1;
         y >= 0;
         --y)
    {
        std::cout
            << "\rScanlines remaining: "
            << y
            << " "
            << std::flush;

        for (int x = 0;
             x < imageWidth;
             ++x)
        {
            Vec3 color(0, 0, 0);

            for (int s = 0;
                 s < samplesPerPixel;
                 ++s)
            {
                double u =
                    (x + dist(rng)) / (imageWidth - 1);

                double v =
                    (y + dist(rng)) / (imageHeight - 1);

                Ray ray =
                    cam.getRay(u, v);

                color =
                    color + rayColor(ray, world);
            }

            color =
                color / samplesPerPixel;

            color.x =
                std::clamp(color.x, 0.0, 1.0);

            color.y =
                std::clamp(color.y, 0.0, 1.0);

            color.z =
                std::clamp(color.z, 0.0, 1.0);

            int r =
                int(255.999 * color.x);

            int g =
                int(255.999 * color.y);

            int b =
                int(255.999 * color.z);

            out << r << " "
                << g << " "
                << b << "\n";
        }
    }

    std::cout
        << "\nRendered ../images/output.ppm\n";

    return 0;
}