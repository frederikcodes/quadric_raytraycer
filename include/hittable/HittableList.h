#pragma once

#include <memory>
#include <vector>

#include "acceleration/AABB.h"
#include "core/Ray.h"
#include "hittable/Hittable.h"
#include "hittable/HitRecord.h"

struct HittableList : public Hittable
{
    struct ObjectEntry
    {
        std::shared_ptr<Hittable> object;
        AABB box;
        bool hasBox;
    };

    std::vector<ObjectEntry> objects;

    void add(std::shared_ptr<Hittable> object);

    void clear();

    const std::vector<ObjectEntry> &entries() const;

    bool hit(const Ray &ray,
             double tMin,
             double tMax,
             HitRecord &rec) const override;

    bool anyHit(const Ray &ray,
                double tMin,
                double tMax) const override;

    bool boundingBox(AABB &box) const override;
};
