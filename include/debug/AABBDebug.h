#pragma once

#include "acceleration/AABB.h"
#include "core/Material.h"
#include "hittable/HittableList.h"

void addAABBCorners(HittableList &world,
                    const AABB &box,
                    const Material &mat,
                    double radius = 0.035);
