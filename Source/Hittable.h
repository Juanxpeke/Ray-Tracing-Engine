#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"

class Material;

struct HitRecord
{
  Point3 p;
  Vec3 normal;
  std::shared_ptr<Material> material;
  double t;
  bool frontFace;

  void SetFaceNormal(const Ray& r, const Vec3& outwardNormal)
  {
    // Sets the hit record normal vector
    // NOTE: the parameter `outward_normal` is assumed to have unit length

    frontFace = Dot(r.Direction(), outwardNormal) < 0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};

class Hittable
{
public:
  virtual ~Hittable() = default;

  virtual bool Hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;
};

#endif // HITTABLE_H