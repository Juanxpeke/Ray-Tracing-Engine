#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "Hittable.h"

class Camera
{
public:
  double aspectRatio = 1.0;  // Ratio of image width over height
  int    imageWidth  = 100;  // Rendered image width in pixel count

  void Render(const Hittable& world)
  {
    Initialize();

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; j++)
    {
      std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;

      for (int i = 0; i < imageWidth; i++)
      {
        auto pixel_center = pixel00Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
        auto ray_direction = pixel_center - center;
        Ray r(center, ray_direction);

        Color pixel_color = RayColor(r, world);
        WriteColor(std::cout, pixel_color);
      }
    }

    std::clog << "\rDone.                 \n";
  }

private:
  int    imageHeight; // Rendered image height
  Point3 center;      // Camera center
  Point3 pixel00Loc;  // Location of pixel 0, 0
  Vec3   pixelDeltaU; // Offset to pixel to the right
  Vec3   pixelDeltaV; // Offset to pixel below

  void Initialize()
  {
    imageHeight = int(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    center = Point3(0, 0, 0);

    // Determine viewport dimensions.
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(imageWidth) / imageHeight);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewportU = Vec3(viewport_width, 0, 0);
    auto viewportV = Vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixelDeltaU = viewportU / imageWidth;
    pixelDeltaV = viewportV / imageHeight;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left =
    center - Vec3(0, 0, focal_length) - viewportU / 2 - viewportV / 2;
    pixel00Loc = viewport_upper_left + 0.5 * (pixelDeltaU + pixelDeltaV);
  }

  Color RayColor(const Ray& r, const Hittable& world) const
  {
    HitRecord rec;

    if (world.Hit(r, Interval(0, infinity), rec))
    {
      return 0.5 * (rec.normal + Color(1,1,1));
    }

    Vec3 normalizedDirection = Normalize(r.Direction());
    auto a = 0.5*(normalizedDirection.Y() + 1.0);

    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
  }
};

#endif // CAMERA_H