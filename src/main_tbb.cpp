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
#include <vector>

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

#include "acceleration/Octree.h"

#include "core/Camera.h"
#include "core/Ray.h"
#include "core/Vec3.h"

#include "hittable/Hittable.h"

#include "scenes/Scenes.h"

const std::string outputDir =
    "../results/multiple_scenes_tbb/";

const Vec3 lightPos(3, 6, 4);

const double infinity =
    std::numeric_limits<double>::infinity();

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
            world.anyHit(shadowRay, 0.001, lightDistance) ||
            ground.anyHit(shadowRay, 0.001, lightDistance);

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

    std::vector<unsigned char> framebuffer(
        imageWidth * imageHeight * 3);

    auto startTime =
        std::chrono::high_resolution_clock::now();

    tbb::parallel_for(
        tbb::blocked_range<int>(0, imageHeight),
        [&](const tbb::blocked_range<int> &range)
        {
            for (int row = range.begin(); row < range.end(); ++row)
            {
                int y = imageHeight - 1 - row;

                std::mt19937 rng(42 + row);
                std::uniform_real_distribution<double> dist(0.0, 1.0);

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

                    std::size_t index =
                        (row * imageWidth + x) * 3;

                    framebuffer[index + 0] =
                        static_cast<unsigned char>(r);

                    framebuffer[index + 1] =
                        static_cast<unsigned char>(g);

                    framebuffer[index + 2] =
                        static_cast<unsigned char>(b);
                }
            }
        });

    auto endTime =
        std::chrono::high_resolution_clock::now();

    std::ofstream out(outputDir + filename, std::ios::binary);

    out << "P6\n";
    out << imageWidth << " " << imageHeight << "\n";
    out << "255\n";

    out.write(
        reinterpret_cast<char *>(framebuffer.data()),
        framebuffer.size());

    double seconds =
        std::chrono::duration<double>(
            endTime - startTime)
            .count();

    std::cout
        << "[" << label << "] Render time: "
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
    const int imageWidth = 1280;
    const int imageHeight = 720;
    const int samplesPerPixel = 4;
    const int maxDepth = 4;

    std::vector<Scene> scenes;

    scenes.push_back(
        createMainScene(
            imageWidth,
            imageHeight));

    scenes.push_back(
        createManySpheresScene(
            imageWidth,
            imageHeight));

    scenes.push_back(
        createManySmallObjectsScene(
            imageWidth,
            imageHeight));

    scenes.push_back(
        createReflectiveStressScene(
            imageWidth,
            imageHeight));

    std::ofstream benchmarkFile(
        outputDir + "benchmark_tbb.txt");

    benchmarkFile
        << "=== Multi-Scene Benchmark with oneTBB ===\n";

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

    for (const auto &scene : scenes)
    {
        auto octreeBuildStart =
            std::chrono::high_resolution_clock::now();

        Octree octree(scene.world);

        auto octreeBuildEnd =
            std::chrono::high_resolution_clock::now();

        double octreeBuildSeconds =
            std::chrono::duration<double>(
                octreeBuildEnd - octreeBuildStart)
                .count();

        double listTime =
            renderImage(
                scene.name + "_HittableList_TBB",
                scene.name + "_list_tbb.ppm",
                scene.world,
                *scene.ground,
                scene.camera,
                imageWidth,
                imageHeight,
                samplesPerPixel,
                maxDepth);

        double octreeTime =
            renderImage(
                scene.name + "_Octree_TBB",
                scene.name + "_octree_tbb.ppm",
                octree,
                *scene.ground,
                scene.camera,
                imageWidth,
                imageHeight,
                samplesPerPixel,
                maxDepth);

        double totalOctreeTime =
            octreeBuildSeconds + octreeTime;

        benchmarkFile
            << "=== Scene: "
            << scene.name
            << " ===\n";

        benchmarkFile
            << "HittableList TBB render time: "
            << listTime
            << " seconds\n";

        benchmarkFile
            << "Octree build time: "
            << octreeBuildSeconds
            << " seconds\n";

        benchmarkFile
            << "Octree TBB render time: "
            << octreeTime
            << " seconds\n";

        benchmarkFile
            << "Total octree time: "
            << totalOctreeTime
            << " seconds\n";

        if (octreeTime > 0.0)
        {
            benchmarkFile
                << "Render speedup list/octree: "
                << listTime / octreeTime
                << "x\n";
        }

        benchmarkFile << "\n";

        std::cout
            << "\n=== Scene: "
            << scene.name
            << " ===\n";

        std::cout
            << "Output directory: "
            << outputDir
            << "\n";

        std::cout
            << "HittableList TBB render time: "
            << listTime
            << " seconds\n";

        std::cout
            << "Octree build time: "
            << octreeBuildSeconds
            << " seconds\n";

        std::cout
            << "Octree TBB render time: "
            << octreeTime
            << " seconds\n";

        std::cout
            << "Total octree time: "
            << totalOctreeTime
            << " seconds\n";

        if (octreeTime > 0.0)
        {
            std::cout
                << "Render speedup list/octree: "
                << listTime / octreeTime
                << "x\n";
        }
    }

    return 0;
}