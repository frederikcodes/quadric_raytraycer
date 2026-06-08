#pragma once

#include <memory>
#include <string>

#include "core/Camera.h"
#include "hittable/Hittable.h"
#include "hittable/HittableList.h"

struct Scene
{
    std::string name;
    Camera camera;
    HittableList world;
    std::shared_ptr<Hittable> ground;
};