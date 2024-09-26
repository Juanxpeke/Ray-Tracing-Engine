#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "Hittable.h"
#include "Material.h"

class Camera
{
public:
  double aspectRatio     = 1.0;  // Ratio of image width over height
  int    imageWidth      = 100;  // Rendered image width in pixel count
  int    samplesPerPixel = 10;   // Count of random samples for each pixel
  int    maxDepth        = 10;   // Maximum number of ray bounces into scene

  void Render(const Hittable& world)
  {
    Initialize();

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; j++)
    {
      std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;

      for (int i = 0; i < imageWidth; i++)
      {
        Color pixelColor(0,0,0);
        for (int sample = 0; sample < samplesPerPixel; sample++)
        {
          Ray r = GetRay(i, j);
          pixelColor += RayColor(r, maxDepth, world);
        }

        WriteColor(std::cout, pixelSamplesScale * pixelColor);
      }
    }

    std::clog << "\rDone.                 \n";
  }

private:
  int    imageHeight;          // Rendered image height
  double pixelSamplesScale;    // Color scale factor for a sum of pixel samples
  Point3 center;               // Camera center
  Point3 pixel00Loc;           // Location of pixel 0, 0
  Vec3   pixelDeltaU;          // Offset to pixel to the right
  Vec3   pixelDeltaV;          // Offset to pixel below

  void Initialize()
  {
    imageHeight = int(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    pixelSamplesScale = 1.0 / samplesPerPixel;

    center = Point3(0, 0, 0);

    // Determine viewport dimensions
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(imageWidth) / imageHeight);

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    auto viewportU = Vec3(viewport_width, 0, 0);
    auto viewportV = Vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    pixelDeltaU = viewportU / imageWidth;
    pixelDeltaV = viewportV / imageHeight;

    // Calculate the location of the upper left pixel
    auto viewport_upper_left =
    center - Vec3(0, 0, focal_length) - viewportU / 2 - viewportV / 2;
    pixel00Loc = viewport_upper_left + 0.5 * (pixelDeltaU + pixelDeltaV);
  }

  Ray GetRay(int i, int j) const
  {
    // Construct a camera ray originating from the origin and directed at randomly sampled
    // point around the pixel location i, j

    auto offset = SampleSquare();
    auto pixelSample = pixel00Loc
    + ((i + offset.X()) * pixelDeltaU)
    + ((j + offset.Y()) * pixelDeltaV);

    auto rayOrigin = center;
    auto rayDirection = pixelSample - rayOrigin;

    return Ray(rayOrigin, rayDirection);
  }

  Vec3 SampleSquare() const
  {
    // Returns the vector to a random point in the [-.5, -.5] - [+.5, +.5] unit square
    // TODO: Replace 0.5 with a pixel spacing variable
    return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
  }

  Color RayColor(const Ray& r, int depth, const Hittable& world) const
  {
    // If we've exceeded the ray bounce limit, no more light is gathered
    if (depth <= 0)
    {
      return Color(0, 0, 0);
    }

    HitRecord rec;

    // Choose 0.001 as minimum t value to solve shadow acne
    if (world.Hit(r, Interval(0.001, infinity), rec))
    {
      Ray scattered;
      Color attenuation;
      if (rec.material->Scatter(r, rec, attenuation, scattered))
      {
        return attenuation * RayColor(scattered, depth - 1, world);
      }
      return Color(0, 0, 0);
    }

    Vec3 normalizedDirection = Normalized(r.Direction());
    auto a = 0.5 * (normalizedDirection.Y() + 1.0);

    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
  }
};

#endif // CAMERA_H