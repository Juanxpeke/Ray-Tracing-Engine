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

  double vFov            = 90;              // Vertical view angle (field of view)
  Point3 lookFrom        = Point3(0,0,0);   // Point camera is looking from
  Point3 lookAt          = Point3(0,0,-1);  // Point camera is looking at
  Vec3   vUp             = Vec3(0,1,0);     // Camera-relative "up" direction

  double defocusAngle = 0;  // Variation angle of rays through each pixel
  double focusDist    = 10; // Distance from camera lookFrom point to plane of perfect focus

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
  Vec3   u, v, w;              // Camera frame basis vectors
  Vec3   defocusDiskU;         // Defocus disk horizontal radius
  Vec3   defocusDiskV;         // Defocus disk vertical radius

  void Initialize()
  {
    imageHeight = int(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    pixelSamplesScale = 1.0 / samplesPerPixel;

    center = lookFrom;

    // Determine viewport dimensions
    auto theta = DegToRad(vFov);
    auto h = std::tan(theta / 2);
    auto viewportHeight = 2 * h * focusDist;
    auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame
    w = Normalized(lookFrom - lookAt);
    u = Normalized(Cross(vUp, w));
    v = Cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    Vec3 viewportU = viewportWidth * u;    // Vector across viewport horizontal edge
    Vec3 viewportV = viewportHeight * -v;  // Vector down viewport vertical edge

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    pixelDeltaU = viewportU / imageWidth;
    pixelDeltaV = viewportV / imageHeight;

    // Calculate the location of the upper left pixel
    auto viewportUpperLeft = center - (focusDist * w) - viewportU / 2 - viewportV / 2;
    pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

    // Calculate the camera defocus disk basis vectors
    auto defocusRadius = focusDist * std::tan(DegToRad(defocusAngle / 2));
    defocusDiskU = u * defocusRadius;
    defocusDiskV = v * defocusRadius;
  }

  Ray GetRay(int i, int j) const
  {
    // Construct a camera ray originating from the defocus disk and directed at a randomly
    // sampled point around the pixel location i, j

    auto offset = SampleSquare();
    auto pixelSample = pixel00Loc
    + ((i + offset.X()) * pixelDeltaU)
    + ((j + offset.Y()) * pixelDeltaV);

    auto rayOrigin = (defocusAngle <= 0) ? center : DefocusDiskSample();;
    auto rayDirection = pixelSample - rayOrigin;

    return Ray(rayOrigin, rayDirection);
  }

  Vec3 SampleSquare() const
  {
    // Returns the vector to a random point in the [-.5, -.5] - [+.5, +.5] unit square
    // TODO: Replace 0.5 with a pixel spacing variable (?)
    return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
  }

  Point3 DefocusDiskSample() const
  {
    // Returns a random point in the camera defocus disk
    auto p = RandomNormalized2DVector();
    return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
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