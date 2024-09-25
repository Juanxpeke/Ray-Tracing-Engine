#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double DegToRad(double degrees)
{
  return degrees * pi / 180.0;
}

inline double RadToDeg(double radians)
{
  return radians * 180.0 / pi;
}

inline double RandomDouble()
{
  // Returns a random real in [0,1)
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double RandomDouble(double min, double max)
{
  // Returns a random real in [min, max)
  return min + (max - min) * RandomDouble();
}

// Common Headers

#include "Color.h"
#include "Interval.h"
#include "Ray.h"
#include "Vec3.h"

#endif // ENGINE_H