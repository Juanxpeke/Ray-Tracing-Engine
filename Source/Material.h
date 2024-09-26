#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Hittable.h"

class Material
{
public:
  virtual ~Material() = default;

  virtual bool Scatter(
      const Ray& rIn,
      const HitRecord& rec,
      Color& attenuation,
      Ray& scattered) const
  {
    return false;
  }
};

class Lambertian : public Material
{
public:
  Lambertian(const Color& albedo) : albedo(albedo) {}

  bool Scatter(
      const Ray& rIn,
      const HitRecord& rec,
      Color& attenuation,
      Ray& scattered) const override
  {
    auto scatterDirection = LambertianSphere(rec.normal);

    // Catch degenerate scatter direction
    if (scatterDirection.NearZero())
    {
      scatterDirection = rec.normal;
    }

    scattered = Ray(rec.p, scatterDirection);
    attenuation = albedo;
    return true;
  }

private:
  Color albedo;
};

class Metal : public Material
{
public:
  Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

  bool Scatter(
      const Ray& rIn,
      const HitRecord& rec,
      Color& attenuation,
      Ray& scattered) const override
  {
    Vec3 reflected = Reflect(rIn.Direction(), rec.normal);

    // In order for the fuzz sphere to make sense, it needs to be consistently scaled
    // compared to the reflection vector, which can vary in length arbitrarily
    reflected = Normalized(reflected) + (fuzz * RandomNormalizedVector());

    scattered = Ray(rec.p, reflected);
    attenuation = albedo;
    
    return (Dot(scattered.Direction(), rec.normal) > 0);
  }

private:
  Color albedo;
  double fuzz;
};

#endif // MATERIAL_H