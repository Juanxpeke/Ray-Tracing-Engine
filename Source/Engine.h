#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include <cmath>
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

// Common Headers

#include "Color.h"
#include "Interval.h"
#include "Ray.h"
#include "Vec3.h"

#endif // ENGINE_H