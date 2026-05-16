#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>

#include "Camera.h"
#include "HittableList.h"
#include "Ray.h"
#include "Sphere.h"
#include "Vec3.h"

Vec3 rayColor(const Ray &ray, const Hittable &world)
{
    HitRecord rec;

    if (world.hit(ray, 0.001, 1e30, rec))
    {
        Vec3 lightPos(3, 5, 3);

        Vec3 lightDir =
            (lightPos - rec.point).normalized();

        double intensity =
            std::max(
                0.0,
                dot(rec.normal, lightDir));

        return rec.color * intensity;
    }

    Vec3 unitDir =
        ray.direction.normalized();

    double t =
        0.5 * (unitDir.y + 1.0);

    Vec3 white(1.0, 1.0, 1.0);
    Vec3 blue(0.5, 0.7, 1.0);

    return white * (1.0 - t) + blue * t;
}

int main()
{
    const int imageWidth = 800;
    const int imageHeight = 450;

    const int samplesPerPixel = 64;

    Camera cam(
        Vec3(0, 0, 3),
        Vec3(0, 0, 0),
        Vec3(0, 1, 0),
        90.0,
        double(imageWidth) / imageHeight);

    HittableList world;

    world.add(
        std::make_shared<Sphere>(
            Vec3(0, 0, 0),
            0.5,
            Vec3(1, 0.2, 0.2)));

    world.add(
        std::make_shared<Sphere>(
            Vec3(0, -100.5, 0),
            100.0,
            Vec3(0.8, 0.8, 0.8)));

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