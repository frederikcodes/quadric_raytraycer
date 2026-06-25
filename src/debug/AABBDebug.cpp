#include "debug/AABBDebug.h"

#include <memory>

#include "primitives/Sphere.h"

void addAABBCorners(HittableList &world,
                    const AABB &box,
                    const Material &mat,
                    double radius)
{
    for (int x = 0; x < 2; x++)
    {
        for (int y = 0; y < 2; y++)
        {
            for (int z = 0; z < 2; z++)
            {
                Vec3 p(
                    x ? box.maximum.x : box.minimum.x,
                    y ? box.maximum.y : box.minimum.y,
                    z ? box.maximum.z : box.minimum.z);

                world.add(
                    std::make_shared<Sphere>(
                        p,
                        radius,
                        mat));
            }
        }
    }
}
