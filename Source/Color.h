#pragma once

#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "Vec3.h"

using Color = Vec3;

void WriteColor(std::ostream& out, const Color& pixelColor)
{
  auto r = pixelColor.X();
  auto g = pixelColor.Y();
  auto b = pixelColor.Z();

  // Translate the [0,1] component values to the byte range [0,255]
  int rByte = int(255.999 * r);
  int gByte = int(255.999 * g);
  int bByte = int(255.999 * b);

  // Write out the pixel color components
  out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}

#endif // COLOR_H