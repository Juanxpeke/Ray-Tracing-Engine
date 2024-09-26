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

class Dielectric : public Material
{
public:
  Dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

  bool Scatter(
      const Ray& rIn,
      const HitRecord& rec,
      Color& attenuation,
      Ray& scattered) const override
  {
    attenuation = Color(1.0, 1.0, 1.0);
    double ri = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

    Vec3 normalizedDirection = Normalized(rIn.Direction());
    double cosTheta = std::fmin(Dot(-normalizedDirection, rec.normal), 1.0);
    double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

    bool cannotRefract = ri * sinTheta > 1.0;
    Vec3 direction;

    if (cannotRefract || Reflectance(cosTheta, ri) > RandomDouble())
    {
      direction = Reflect(normalizedDirection, rec.normal);
    }
    else
    {
      direction = Refract(normalizedDirection, rec.normal, ri);
    }

    scattered = Ray(rec.p, direction);
    return true;
  }

private:
  // Refractive index in vacuum or air, or the ratio of the material's refractive index over
  // the refractive index of the enclosing media
  double refractionIndex;

  static double Reflectance(double cosine, double refractionIndex)
  {
    // Use Schlick's approximation for reflectance
    auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
  }
};

#endif // MATERIAL_H