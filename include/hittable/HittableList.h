#pragma once

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

    const std::vector<ObjectEntry> &entries() const
    {
        return objects;
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

            if (entry.object->anyHit(ray, tMin, tMax))
            {
                return true;
            }
        }

        return false;
    }

    bool boundingBox(AABB &box) const override
    {
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
                box = surroundingBox(box, entry.box);
            }
        }

        return !firstBox;
    }
};