#pragma once

#include <memory>
#include <vector>

#include "Hittable.h"

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
};