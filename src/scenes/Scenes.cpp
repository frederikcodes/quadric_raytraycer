#include "scenes/Scenes.h"

#include <memory>
#include <random>

#include "core/Material.h"
#include "primitives/Cone.h"
#include "primitives/Cylinder.h"
#include "primitives/Ellipsoid.h"
#include "primitives/Sphere.h"
#include "transforms/RotateX.h"
#include "transforms/RotateZ.h"
#include "transforms/Translate.h"

Scene createMainScene(int imageWidth, int imageHeight)
{
    Scene scene{
        "main_scene",
        Camera(
            Vec3(3.0, 2.0, 5.0),
            Vec3(0.0, 0.0, -1.6),
            Vec3(0, 1, 0),
            42.0,
            double(imageWidth) / imageHeight),
        HittableList(),
        nullptr};

    scene.name = "main_scene";

    scene.camera = Camera(
        Vec3(3.0, 2.0, 5.0),
        Vec3(0.0, 0.0, -1.6),
        Vec3(0, 1, 0),
        42.0,
        double(imageWidth) / imageHeight);

    Material blueMat(Vec3(0.15, 0.35, 1.0), 0.06, 0.9, 1.0, 128.0, 0.35);
    Material redMat(Vec3(1.0, 0.12, 0.08), 0.05, 0.85, 1.2, 180.0, 0.45);
    Material greenMat(Vec3(0.2, 0.9, 0.35), 0.08, 0.9, 0.5, 48.0, 0.15);
    Material yellowMat(Vec3(1.0, 0.85, 0.12), 0.04, 0.9, 1.4, 256.0, 0.60);
    Material purpleMat(Vec3(0.55, 0.20, 0.9), 0.05, 0.85, 1.0, 160.0, 0.50);
    Material orangeMat(Vec3(1.0, 0.45, 0.08), 0.06, 0.9, 0.9, 128.0, 0.25);
    Material cyanMat(Vec3(0.1, 0.85, 1.0), 0.05, 0.9, 1.1, 180.0, 0.55);
    Material groundMat(Vec3(0.78, 0.78, 0.78), 0.12, 0.9, 0.15, 16.0, 0.08);

    scene.world.add(std::make_shared<Cylinder>(
        Vec3(0.0, -0.5, -1.4), 0.45, 1.6, blueMat));

    scene.world.add(std::make_shared<Translate>(
        std::make_shared<RotateZ>(
            std::make_shared<Cone>(
                Vec3(0.0, -0.5, 0.0), 1.3, 0.42, orangeMat),
            -55.0),
        Vec3(-0.9, 0.45, -1.9)));

    scene.world.add(std::make_shared<Translate>(
        std::make_shared<RotateX>(
            std::make_shared<Cylinder>(
                Vec3(0.0, -0.75, 0.0), 0.28, 1.5, cyanMat),
            85.0),
        Vec3(1.1, 0.35, -2.3)));

    scene.world.add(std::make_shared<Ellipsoid>(
        Vec3(-1.55, 0.55, -3.1),
        Vec3(0.45, 1.05, 0.55),
        greenMat));

    scene.world.add(std::make_shared<Sphere>(
        Vec3(-1.15, -0.05, -0.65), 0.45, redMat));

    scene.world.add(std::make_shared<Sphere>(
        Vec3(1.35, -0.05, -0.9), 0.42, purpleMat));

    scene.world.add(std::make_shared<Sphere>(
        Vec3(0.05, 0.10, -3.7), 0.70, yellowMat));

    scene.world.add(std::make_shared<Translate>(
        std::make_shared<RotateZ>(
            std::make_shared<Cylinder>(
                Vec3(0.0, -0.5, 0.0), 0.25, 1.0, yellowMat),
            35.0),
        Vec3(-1.9, 0.30, -2.2)));

    scene.world.add(std::make_shared<Translate>(
        std::make_shared<RotateX>(
            std::make_shared<Cone>(
                Vec3(0.0, -0.5, 0.0), 1.0, 0.35, redMat),
            -35.0),
        Vec3(1.75, 0.35, -3.0)));

    scene.ground = std::make_shared<Sphere>(
        Vec3(0, -100.5, 0),
        100.0,
        groundMat);

    return scene;
}
Scene createManySpheresScene(int imageWidth, int imageHeight)
{
    Scene scene{
        "main_scene",
        Camera(
            Vec3(3.0, 2.0, 5.0),
            Vec3(0.0, 0.0, -1.6),
            Vec3(0, 1, 0),
            42.0,
            double(imageWidth) / imageHeight),
        HittableList(),
        nullptr};

    scene.name = "many_spheres";

    scene.camera = Camera(
        Vec3(4.0, 3.0, 7.0),
        Vec3(0.0, 0.0, -2.5),
        Vec3(0, 1, 0),
        45.0,
        double(imageWidth) / imageHeight);

    Material groundMat(Vec3(0.78, 0.78, 0.78), 0.12, 0.9, 0.15, 16.0, 0.05);

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> posX(-3.0, 3.0);
    std::uniform_real_distribution<double> posZ(-5.5, -0.8);
    std::uniform_real_distribution<double> radiusDist(0.08, 0.18);
    std::uniform_real_distribution<double> colorDist(0.1, 1.0);
    std::uniform_real_distribution<double> reflectDist(0.05, 0.35);

    for (int i = 0; i < 250; ++i)
    {
        double radius = radiusDist(rng);

        Material mat(
            Vec3(colorDist(rng), colorDist(rng), colorDist(rng)),
            0.05,
            0.85,
            0.6,
            64.0,
            reflectDist(rng));

        scene.world.add(std::make_shared<Sphere>(
            Vec3(posX(rng), -0.45 + radius, posZ(rng)),
            radius,
            mat));
    }

    scene.ground = std::make_shared<Sphere>(
        Vec3(0, -100.5, 0),
        100.0,
        groundMat);

    return scene;
}
Scene createManySmallObjectsScene(int imageWidth, int imageHeight)
{
    Scene scene{
        "main_scene",
        Camera(
            Vec3(3.0, 2.0, 5.0),
            Vec3(0.0, 0.0, -1.6),
            Vec3(0, 1, 0),
            42.0,
            double(imageWidth) / imageHeight),
        HittableList(),
        nullptr};

    scene.name = "many_small_objects";

    scene.camera = Camera(
        Vec3(4.5, 3.0, 7.0),
        Vec3(0.0, 0.0, -2.8),
        Vec3(0, 1, 0),
        45.0,
        double(imageWidth) / imageHeight);

    Material groundMat(Vec3(0.78, 0.78, 0.78), 0.12, 0.9, 0.15, 16.0, 0.05);

    std::mt19937 rng(123);
    std::uniform_real_distribution<double> posX(-3.5, 3.5);
    std::uniform_real_distribution<double> posZ(-6.0, -1.0);
    std::uniform_real_distribution<double> colorDist(0.1, 1.0);
    std::uniform_real_distribution<double> reflectDist(0.05, 0.45);
    std::uniform_int_distribution<int> typeDist(0, 2);

    for (int i = 0; i < 180; ++i)
    {
        Material mat(
            Vec3(colorDist(rng), colorDist(rng), colorDist(rng)),
            0.05,
            0.85,
            0.8,
            96.0,
            reflectDist(rng));

        Vec3 center(posX(rng), -0.35, posZ(rng));

        int type = typeDist(rng);

        if (type == 0)
        {
            scene.world.add(std::make_shared<Sphere>(
                center,
                0.14,
                mat));
        }
        else if (type == 1)
        {
            scene.world.add(std::make_shared<Cylinder>(
                center,
                0.10,
                0.45,
                mat));
        }
        else
        {
            scene.world.add(std::make_shared<Cone>(
                center,
                0.45,
                0.13,
                mat));
        }
    }

    scene.ground = std::make_shared<Sphere>(
        Vec3(0, -100.5, 0),
        100.0,
        groundMat);

    return scene;
}
Scene createReflectiveStressScene(int imageWidth, int imageHeight)
{
    Scene scene{
        "reflective_stress",
        Camera(
            Vec3(4.5, 3.0, 7.0),
            Vec3(0.0, 0.0, -3.0),
            Vec3(0, 1, 0),
            45.0,
            double(imageWidth) / imageHeight),
        HittableList(),
        nullptr};

    Material groundMat(
        Vec3(0.72, 0.72, 0.72),
        0.08,
        0.85,
        0.35,
        64.0,
        0.25);

    std::mt19937 rng(777);

    std::uniform_real_distribution<double> posX(-3.2, 3.2);
    std::uniform_real_distribution<double> posZ(-6.2, -1.0);
    std::uniform_real_distribution<double> radiusDist(0.10, 0.24);
    std::uniform_real_distribution<double> colorDist(0.25, 1.0);
    std::uniform_real_distribution<double> reflectDist(0.65, 0.95);

    for (int i = 0; i < 220; ++i)
    {
        double radius = radiusDist(rng);

        Material mirrorMat(
            Vec3(
                colorDist(rng),
                colorDist(rng),
                colorDist(rng)),
            0.03,
            0.55,
            1.4,
            256.0,
            reflectDist(rng));

        scene.world.add(
            std::make_shared<Sphere>(
                Vec3(
                    posX(rng),
                    -0.45 + radius,
                    posZ(rng)),
                radius,
                mirrorMat));
    }

    scene.ground =
        std::make_shared<Sphere>(
            Vec3(0, -100.5, 0),
            100.0,
            groundMat);

    return scene;
}