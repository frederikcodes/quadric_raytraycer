#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

#include "Camera.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Ellipsoid.h"
#include "HittableList.h"
#include "Material.h"
#include "Ray.h"
#include "RotateX.h"
#include "RotateZ.h"
#include "Sphere.h"
#include "Translate.h"
#include "Vec3.h"

const Vec3 lightPos(3, 6, 4);

const double infinity =
    std::numeric_limits<double>::infinity();

Vec3 rayColor(const Ray &ray,
              const Hittable &world)
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

        bool inShadow =
            world.hit(
                shadowRay,
                0.001,
                lightDistance - 0.001,
                shadowRec);

        Vec3 ambient =
            rec.material.color *
            rec.material.ambient;

        Vec3 diffuseColor(0, 0, 0);
        Vec3 specularColor(0, 0, 0);

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
                (rec.normal *
                     2.0 *
                     dot(rec.normal, lightDir) -
                 lightDir)
                    .normalized();

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

    return Vec3(1.0, 1.0, 1.0) * (1.0 - t) +
           Vec3(0.5, 0.7, 1.0) * t;
}

int main()
{
    const int imageWidth = 800;
    const int imageHeight = 450;
    const int samplesPerPixel = 64;

    Camera cam(
        Vec3(3.0, 2.0, 5.0),
        Vec3(0.0, 0.0, -1.6),
        Vec3(0, 1, 0),
        42.0,
        double(imageWidth) / imageHeight);

    Material blueMat(Vec3(0.15, 0.35, 1.0), 0.08, 0.9, 0.7, 96.0);
    Material redMat(Vec3(1.0, 0.12, 0.08), 0.06, 0.85, 1.1, 160.0);
    Material greenMat(Vec3(0.2, 0.9, 0.35), 0.10, 0.9, 0.35, 32.0);
    Material yellowMat(Vec3(1.0, 0.85, 0.12), 0.06, 0.9, 1.2, 200.0);
    Material purpleMat(Vec3(0.55, 0.20, 0.9), 0.08, 0.85, 0.9, 120.0);
    Material orangeMat(Vec3(1.0, 0.45, 0.08), 0.08, 0.9, 0.8, 96.0);
    Material cyanMat(Vec3(0.1, 0.85, 1.0), 0.08, 0.9, 0.75, 96.0);
    Material groundMat(Vec3(0.78, 0.78, 0.78), 0.14, 0.9, 0.04, 8.0);

    HittableList world;

    world.add(
        std::make_shared<Cylinder>(
            Vec3(0.0, -0.5, -1.4),
            0.45,
            1.6,
            blueMat));

    world.add(
        std::make_shared<Translate>(
            std::make_shared<RotateZ>(
                std::make_shared<Cone>(
                    Vec3(0.0, -0.5, 0.0),
                    1.3,
                    0.42,
                    orangeMat),
                -55.0),
            Vec3(-0.9, 0.45, -1.9)));

    world.add(
        std::make_shared<Translate>(
            std::make_shared<RotateX>(
                std::make_shared<Cylinder>(
                    Vec3(0.0, -0.75, 0.0),
                    0.28,
                    1.5,
                    cyanMat),
                85.0),
            Vec3(1.1, 0.35, -2.3)));

    world.add(
        std::make_shared<Ellipsoid>(
            Vec3(-1.55, 0.05, -3.1),
            Vec3(0.45, 1.05, 0.55),
            greenMat));

    world.add(
        std::make_shared<Sphere>(
            Vec3(-1.15, -0.05, -0.65),
            0.45,
            redMat));

    world.add(
        std::make_shared<Sphere>(
            Vec3(1.35, -0.05, -0.9),
            0.42,
            purpleMat));

    world.add(
        std::make_shared<Sphere>(
            Vec3(0.05, 0.10, -3.7),
            0.70,
            yellowMat));

    world.add(
        std::make_shared<Translate>(
            std::make_shared<RotateZ>(
                std::make_shared<Cylinder>(
                    Vec3(0.0, -0.5, 0.0),
                    0.25,
                    1.0,
                    yellowMat),
                35.0),
            Vec3(-1.9, 0.30, -2.2)));

    world.add(
        std::make_shared<Translate>(
            std::make_shared<RotateX>(
                std::make_shared<Cone>(
                    Vec3(0.0, -0.5, 0.0),
                    1.0,
                    0.35,
                    redMat),
                -35.0),
            Vec3(1.75, 0.35, -3.0)));

    world.add(
        std::make_shared<Sphere>(
            Vec3(0, -100.5, 0),
            100.0,
            groundMat));

    std::ofstream out("../images/output.ppm");

    out << "P3\n";
    out << imageWidth << " " << imageHeight << "\n";
    out << "255\n";

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int y = imageHeight - 1; y >= 0; --y)
    {
        std::cout
            << "\rScanlines remaining: "
            << y
            << " "
            << std::flush;

        for (int x = 0; x < imageWidth; ++x)
        {
            Vec3 color(0, 0, 0);

            for (int s = 0; s < samplesPerPixel; ++s)
            {
                double u =
                    (x + dist(rng)) /
                    (imageWidth - 1);

                double v =
                    (y + dist(rng)) /
                    (imageHeight - 1);

                Ray ray = cam.getRay(u, v);

                color =
                    color +
                    rayColor(ray, world);
            }

            color = color / samplesPerPixel;

            color.x = std::clamp(color.x, 0.0, 1.0);
            color.y = std::clamp(color.y, 0.0, 1.0);
            color.z = std::clamp(color.z, 0.0, 1.0);

            int r = int(255.999 * color.x);
            int g = int(255.999 * color.y);
            int b = int(255.999 * color.z);

            out << r << " "
                << g << " "
                << b << "\n";
        }
    }

    std::cout
        << "\nRendered ../images/output.ppm\n";

    return 0;
}