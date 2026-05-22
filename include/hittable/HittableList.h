#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "acceleration/AABB.h"
#include "hittable/Hittable.h"

struct HittableList : public Hittable
{
    struct ObjectEntry
    {
        std::shared_ptr<Hittable> object;
        AABB box;
        bool hasBox;
    };

    std::vector<ObjectEntry> objects;

    void add(std::shared_ptr<Hittable> object)
    {
        AABB box;
        bool hasBox = object->boundingBox(box);

        objects.push_back({object, box, hasBox});
    }

    void clear()
    {
        objects.clear();
    }

    bool hit(const Ray &ray,
             double tMin,
             double tMax,
             HitRecord &rec) const override
    {
        HitRecord tempRec;
        bool hitAnything = false;
        double closestSoFar = tMax;

        for (const auto &entry : objects)
        {
            if (entry.hasBox)
            {
                if (!entry.box.hit(ray, tMin, closestSoFar))
                {
                    continue;
                }
            }

            if (entry.object->hit(ray, tMin, closestSoFar, tempRec))
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

        bool firstBox = true;

        for (const auto &entry : objects)
        {
            if (!entry.hasBox)
            {
                return false;
            }

            if (firstBox)
            {
                box = entry.box;
                firstBox = false;
            }
            else
            {
                box = AABB(
                    Vec3(
                        std::min(box.minimum.x, entry.box.minimum.x),
                        std::min(box.minimum.y, entry.box.minimum.y),
                        std::min(box.minimum.z, entry.box.minimum.z)),
                    Vec3(
                        std::max(box.maximum.x, entry.box.maximum.x),
                        std::max(box.maximum.y, entry.box.maximum.y),
                        std::max(box.maximum.z, entry.box.maximum.z)));
            }
        }

        return true;
    }
    bool anyHit(const Ray &ray,
                double tMin,
                double tMax) const override
    {
        HitRecord tempRec;

        for (const auto &entry : objects)
        {
            if (entry.hasBox)
            {
                if (!entry.box.hit(ray, tMin, tMax))
                {
                    continue;
                }
            }

            if (entry.object->hit(ray, tMin, tMax, tempRec))
            {
                return true;
            }
        }

        return false;
    }
};