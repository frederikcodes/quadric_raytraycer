#include <algorithm>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <string>

#include "acceleration/Octree.h"

#include "core/Camera.h"
#include "core/Material.h"
#include "core/Ray.h"
#include "core/Vec3.h"

#include "hittable/HittableList.h"

#include "primitives/Cone.h"
#include "primitives/Cylinder.h"
#include "primitives/Ellipsoid.h"
#include "primitives/Sphere.h"

#include "transforms/RotateX.h"
#include "transforms/RotateZ.h"
#include "transforms/Translate.h"

const Vec3 lightPos(3, 6, 4);

const double infinity =
    std::numeric_limits<double>::infinity();

const std::string outputDir =
    "../results/reflections_no_parallelisation/";

Vec3 rayColor(const Ray &ray,
              const Hittable &world,
              const Hittable &ground,
              int depth)
{
    if (depth <= 0)
    {
        return Vec3(0.0, 0.0, 0.0);
    }

    HitRecord rec;
    HitRecord tempRec;

    bool hitAnything = false;
    double closestSoFar = infinity;

    if (world.hit(ray, 0.001, closestSoFar, tempRec))
    {
        hitAnything = true;
        closestSoFar = tempRec.t;
        rec = tempRec;
    }

    if (ground.hit(ray, 0.001, closestSoFar, tempRec))
    {
        hitAnything = true;
        closestSoFar = tempRec.t;
        rec = tempRec;
    }

    if (hitAnything)
    {
        Vec3 lightDir = lightPos - rec.point;
        double lightDistance = lightDir.length();
        lightDir = lightDir / lightDistance;

        Ray shadowRay(
            rec.point + rec.normal * 0.001,
            lightDir);

        bool inShadow =
            world.anyHit(
                shadowRay,
                0.001,
                lightDistance) ||
            ground.anyHit(
                shadowRay,
                0.001,
                lightDistance);

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
                reflect(lightDir * -1.0, rec.normal).normalized();

            double spec =
                std::pow(
                    std::max(0.0, dot(viewDir, reflectDir)),
                    rec.material.shininess);

            specularColor =
                Vec3(1.0, 1.0, 1.0) *
                rec.material.specular *
                spec;
        }

        Vec3 localColor =
            ambient + diffuseColor + specularColor;

        double reflectivity =
            std::max(
                0.0,
                std::min(1.0, rec.material.reflectivity));

        if (reflectivity > 0.0)
        {
            Vec3 reflectedDir =
                reflect(
                    ray.direction.normalized(),
                    rec.normal)
                    .normalized();

            Ray reflectedRay(
                rec.point + rec.normal * 0.001,
                reflectedDir);

            Vec3 reflectedColor =
                rayColor(
                    reflectedRay,
                    world,
                    ground,
                    depth - 1);

            return localColor +
                   reflectedColor * reflectivity * 0.5;
        }

        return localColor;
    }

    Vec3 unitDir = ray.direction.normalized();
    double t = 0.5 * (unitDir.y + 1.0);

    return Vec3(1.0, 1.0, 1.0) * (1.0 - t) +
           Vec3(0.5, 0.7, 1.0) * t;
}

double renderImage(const std::string &label,
                   const std::string &filename,
                   const Hittable &world,
                   const Hittable &ground,
                   const Camera &cam,
                   int imageWidth,
                   int imageHeight,
                   int samplesPerPixel,
                   int maxDepth)
{
    std::filesystem::create_directories(outputDir);

    std::ofstream out(outputDir + filename, std::ios::binary);

    out << "P6\n";
    out << imageWidth << " " << imageHeight << "\n";
    out << "255\n";

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    auto startTime =
        std::chrono::high_resolution_clock::now();

    for (int y = imageHeight - 1; y >= 0; --y)
    {
        std::cout
            << "\r[" << label << "] Scanlines remaining: "
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
                    rayColor(
                        ray,
                        world,
                        ground,
                        maxDepth);
            }

            color = color / samplesPerPixel;

            color.x = std::clamp(color.x, 0.0, 1.0);
            color.y = std::clamp(color.y, 0.0, 1.0);
            color.z = std::clamp(color.z, 0.0, 1.0);

            int r = int(255.999 * color.x);
            int g = int(255.999 * color.y);
            int b = int(255.999 * color.z);

            unsigned char pixel[3] = {
                static_cast<unsigned char>(r),
                static_cast<unsigned char>(g),
                static_cast<unsigned char>(b)};

            out.write(reinterpret_cast<char *>(pixel), 3);
        }
    }

    auto endTime =
        std::chrono::high_resolution_clock::now();

    double seconds =
        std::chrono::duration<double>(
            endTime - startTime)
            .count();

    std::cout
        << "\n[" << label << "] Render time: "
        << seconds
        << " seconds\n";

    std::cout
        << "[" << label << "] Rendered "
        << outputDir + filename
        << "\n";

    return seconds;
}

int main()
{
    const int imageWidth = 1920;
    const int imageHeight = 1080;
    const int samplesPerPixel = 64;
    const int maxDepth = 4;

    Camera cam(
        Vec3(3.0, 2.0, 5.0),
        Vec3(0.0, 0.0, -1.6),
        Vec3(0, 1, 0),
        42.0,
        double(imageWidth) / imageHeight);

    Material blueMat(
        Vec3(0.15, 0.35, 1.0),
        0.06,
        0.9,
        1.0,
        128.0,
        0.35);

    Material redMat(
        Vec3(1.0, 0.12, 0.08),
        0.05,
        0.85,
        1.2,
        180.0,
        0.45);

    Material greenMat(
        Vec3(0.2, 0.9, 0.35),
        0.08,
        0.9,
        0.5,
        48.0,
        0.15);

    Material yellowMat(
        Vec3(1.0, 0.85, 0.12),
        0.04,
        0.9,
        1.4,
        256.0,
        0.60);

    Material purpleMat(
        Vec3(0.55, 0.20, 0.9),
        0.05,
        0.85,
        1.0,
        160.0,
        0.50);

    Material orangeMat(
        Vec3(1.0, 0.45, 0.08),
        0.06,
        0.9,
        0.9,
        128.0,
        0.25);

    Material cyanMat(
        Vec3(0.1, 0.85, 1.0),
        0.05,
        0.9,
        1.1,
        180.0,
        0.55);

    Material groundMat(
        Vec3(0.78, 0.78, 0.78),
        0.12,
        0.9,
        0.15,
        16.0,
        0.08);

    HittableList accelWorld;

    accelWorld.add(
        std::make_shared<Cylinder>(
            Vec3(0.0, -0.5, -1.4),
            0.45,
            1.6,
            blueMat));

    accelWorld.add(
        std::make_shared<Translate>(
            std::make_shared<RotateZ>(
                std::make_shared<Cone>(
                    Vec3(0.0, -0.5, 0.0),
                    1.3,
                    0.42,
                    orangeMat),
                -55.0),
            Vec3(-0.9, 0.45, -1.9)));

    accelWorld.add(
        std::make_shared<Translate>(
            std::make_shared<RotateX>(
                std::make_shared<Cylinder>(
                    Vec3(0.0, -0.75, 0.0),
                    0.28,
                    1.5,
                    cyanMat),
                85.0),
            Vec3(1.1, 0.35, -2.3)));

    accelWorld.add(
        std::make_shared<Ellipsoid>(
            Vec3(-1.55, 0.55, -3.1),
            Vec3(0.45, 1.05, 0.55),
            greenMat));

    accelWorld.add(
        std::make_shared<Sphere>(
            Vec3(-1.15, -0.05, -0.65),
            0.45,
            redMat));

    accelWorld.add(
        std::make_shared<Sphere>(
            Vec3(1.35, -0.05, -0.9),
            0.42,
            purpleMat));

    accelWorld.add(
        std::make_shared<Sphere>(
            Vec3(0.05, 0.10, -3.7),
            0.70,
            yellowMat));

    accelWorld.add(
        std::make_shared<Translate>(
            std::make_shared<RotateZ>(
                std::make_shared<Cylinder>(
                    Vec3(0.0, -0.5, 0.0),
                    0.25,
                    1.0,
                    yellowMat),
                35.0),
            Vec3(-1.9, 0.30, -2.2)));

    accelWorld.add(
        std::make_shared<Translate>(
            std::make_shared<RotateX>(
                std::make_shared<Cone>(
                    Vec3(0.0, -0.5, 0.0),
                    1.0,
                    0.35,
                    redMat),
                -35.0),
            Vec3(1.75, 0.35, -3.0)));

    auto octreeBuildStart =
        std::chrono::high_resolution_clock::now();

    Octree octree(accelWorld);

    auto octreeBuildEnd =
        std::chrono::high_resolution_clock::now();

    double octreeBuildSeconds =
        std::chrono::duration<double>(
            octreeBuildEnd - octreeBuildStart)
            .count();

    Sphere ground(
        Vec3(0, -100.5, 0),
        100.0,
        groundMat);

    double listTime =
        renderImage(
            "HittableList",
            "output_list_reflections.ppm",
            accelWorld,
            ground,
            cam,
            imageWidth,
            imageHeight,
            samplesPerPixel,
            maxDepth);

    double octreeTime =
        renderImage(
            "Octree",
            "output_octree_reflections.ppm",
            octree,
            ground,
            cam,
            imageWidth,
            imageHeight,
            samplesPerPixel,
            maxDepth);

    std::ofstream benchmarkFile(
        outputDir + "benchmark.txt");

    benchmarkFile
        << "=== Reflection Benchmark ===\n";

    benchmarkFile
        << "Resolution: "
        << imageWidth
        << "x"
        << imageHeight
        << "\n";

    benchmarkFile
        << "Samples per pixel: "
        << samplesPerPixel
        << "\n";

    benchmarkFile
        << "Max recursion depth: "
        << maxDepth
        << "\n\n";

    benchmarkFile
        << "HittableList render time: "
        << listTime
        << " seconds\n";

    benchmarkFile
        << "Octree build time: "
        << octreeBuildSeconds
        << " seconds\n";

    benchmarkFile
        << "Octree render time: "
        << octreeTime
        << " seconds\n";

    if (octreeTime > 0.0)
    {
        benchmarkFile
            << "Render speedup: "
            << listTime / octreeTime
            << "x\n";
    }

    std::cout
        << "\n=== Reflection Benchmark ===\n";

    std::cout
        << "Output directory: "
        << outputDir
        << "\n";

    std::cout
        << "HittableList render time: "
        << listTime
        << " seconds\n";

    std::cout
        << "Octree build time: "
        << octreeBuildSeconds
        << " seconds\n";

    std::cout
        << "Octree render time: "
        << octreeTime
        << " seconds\n";

    if (octreeTime > 0.0)
    {
        std::cout
            << "Render speedup: "
            << listTime / octreeTime
            << "x\n";
    }

    return 0;
}