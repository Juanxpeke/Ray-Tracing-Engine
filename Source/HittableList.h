#pragma once

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>
#include "Hittable.h"

class HittableList : public Hittable
{
public:
  std::vector<std::shared_ptr<Hittable>> objects;

  HittableList() {}
  HittableList(std::shared_ptr<Hittable> object) { Add(object); }

  void Clear() { objects.clear(); }

  void Add(std::shared_ptr<Hittable> object)
  {
    objects.push_back(object);
  }

  bool Hit(const Ray& r, Interval rayT, HitRecord& rec) const override
  {
    HitRecord tempRec;
    bool hitAnything = false;
    auto closestSoFar = rayT.max;

    for (const auto& object : objects)
    {
      if (object->Hit(r, Interval(rayT.min, closestSoFar), tempRec))
      {
        hitAnything = true;
        closestSoFar = tempRec.t;
        rec = tempRec;
      }
    }

    return hitAnything;
  }
};

#endif // HITTABLE_LIST_H