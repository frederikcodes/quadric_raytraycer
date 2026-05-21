#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "hittable/Hittable.h"

struct HittableList : public Hittable
{
    std::vector<std::shared_ptr<Hittable>> objects;

    void add(std::shared_ptr<Hittable> object)
    {
        objects.push_back(object);
    }

    void clear()
    {
        objects.clear();
    }

    bool hit(const Ray &ray, double tMin, double tMax, HitRecord &rec) const override
    {
        HitRecord tempRec;
        bool hitAnything = false;
        double closestSoFar = tMax;

        for (const auto &object : objects)
        {
            if (object->hit(ray, tMin, closestSoFar, tempRec))
            {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }

    bool boundingBox(AABB &box) const override
    {
        if (objects.empty())
        {
            return false;
        }

        AABB tempBox;
        bool firstBox = true;

        for (const auto &object : objects)
        {
            if (!object->boundingBox(tempBox))
            {
                return false;
            }

            if (firstBox)
            {
                box = tempBox;
                firstBox = false;
            }
            else
            {
                box = AABB(
                    Vec3(
                        std::min(box.minimum.x, tempBox.minimum.x),
                        std::min(box.minimum.y, tempBox.minimum.y),
                        std::min(box.minimum.z, tempBox.minimum.z)),
                    Vec3(
                        std::max(box.maximum.x, tempBox.maximum.x),
                        std::max(box.maximum.y, tempBox.maximum.y),
                        std::max(box.maximum.z, tempBox.maximum.z)));
            }
        }

        return true;
    }
};
