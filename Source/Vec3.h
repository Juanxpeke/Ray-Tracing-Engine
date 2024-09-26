#pragma once

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class Vec3
{
public:
  double e[3];

  Vec3() : e{0,0,0} {}
  Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

  double X() const { return e[0]; }
  double Y() const { return e[1]; }
  double Z() const { return e[2]; }

  Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
  double operator[](int i) const { return e[i]; }
  double& operator[](int i) { return e[i]; }

  Vec3& operator+=(const Vec3& v)
  {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  Vec3& operator*=(double t)
  {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  Vec3& operator/=(double t)
  {
    return *this *= 1 / t;
  }

  double Length() const
  {
    return std::sqrt(LengthSquared());
  }

  double LengthSquared() const
  {
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
  }

  bool NearZero() const
  {
    // Return true if the vector is close to zero in all dimensions
    auto s = 1e-8;
    return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
  }

  static Vec3 Random()
  {
    return Vec3(RandomDouble(), RandomDouble(), RandomDouble());
  }

  static Vec3 Random(double min, double max)
  {
    return Vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
  }

};

// Point3 is just an alias for vec3, but useful for geometric clarity in the code
using Point3 = Vec3;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
  return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
  return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
  return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3& v)
{
  return Vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vec3 operator*(const Vec3& v, double t)
{
  return t * v;
}

inline Vec3 operator/(const Vec3& v, double t)
{
  return (1 / t) * v;
}

inline double Dot(const Vec3& u, const Vec3& v)
{
  return u.e[0] * v.e[0]
       + u.e[1] * v.e[1]
       + u.e[2] * v.e[2];
}

inline Vec3 Cross(const Vec3& u, const Vec3& v)
{
  return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 Normalized(const Vec3& v)
{
  return v / v.Length();
}

inline Vec3 RandomNormalized2DVector()
{
  while (true)
  {
    auto p = Vec3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);

    if (p.LengthSquared() < 1)
    {
      return p;
    }
  }
}

inline Vec3 RandomNormalizedVector()
{
  while (true)
  {
    auto p = Vec3::Random(-1, 1);
    auto lenSq = p.LengthSquared();

    if (1e-160 < lenSq && lenSq <= 1)
    {
      return p / sqrt(lenSq);
    }
  }
}

inline Vec3 RandomOnHemisphere(const Vec3& normal)
{
  Vec3 onUnitSphere = RandomNormalizedVector();

  if (Dot(onUnitSphere, normal) > 0.0) // In the same hemisphere as the normal
  {
    return onUnitSphere;
  }
  else
  {
    return -onUnitSphere;
  }
}

inline Vec3 LambertianSphere(const Vec3& normal)
{
  return normal + RandomNormalizedVector(); // P + normal + random - P
}

inline Vec3 Reflect(const Vec3& vector, const Vec3& normal)
{
  return vector - 2 * Dot(vector, normal) * normal;
}

inline Vec3 Refract(const Vec3& normalizedVector, const Vec3& normal, double etaOverEtaPrime)
{
  auto cosTheta = std::fmin(Dot(-normalizedVector, normal), 1.0);
  Vec3 rOutPerp =  etaOverEtaPrime * (normalizedVector + cosTheta * normal);
  Vec3 rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerp.LengthSquared())) * normal;
  return rOutPerp + rOutParallel;
}

#endif // VEC3_H