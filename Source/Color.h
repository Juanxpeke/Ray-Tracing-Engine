#pragma once

#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "Interval.h"
#include "Vec3.h"

using Color = Vec3;

void WriteColor(std::ostream& out, const Color& pixelColor)
{
  auto r = pixelColor.X();
  auto g = pixelColor.Y();
  auto b = pixelColor.Z();

  // Translate the [0,1] component values to the byte range [0,255]
  static const Interval intensity(0.000, 0.999);
  int rByte = int(256 * intensity.Clamp(r));
  int gByte = int(256 * intensity.Clamp(g));
  int bByte = int(256 * intensity.Clamp(b));

  // Write out the pixel color components
  out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}

#endif // COLOR_H