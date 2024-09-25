#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable
{
public:
  Sphere(const Point3& center, double radius) : center(center), radius(std::fmax(0, radius)) {}

  bool Hit(const Ray& r, Interval rayT, HitRecord& rec) const override
  {
    Vec3 oc = center - r.Origin();
    auto a = r.Direction().LengthSquared();
    auto h = Dot(r.Direction(), oc);
    auto c = oc.LengthSquared() - radius*radius;

    auto discriminant = h * h - a * c;

    if (discriminant < 0)
    {
      return false;
    }

    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;

    if (!rayT.Surrounds(root))
    {
      root = (h + sqrtd) / a;
  
      if (!rayT.Surrounds(root))
      {
        return false;
      }
    }

    rec.t = root;
    rec.p = r.At(rec.t);
    Vec3 outwardNormal = (rec.p - center) / radius;
    rec.SetFaceNormal(r, outwardNormal);

    return true;
  }

private:
  Point3 center;
  double radius;
};

#endif // SPHERE_H