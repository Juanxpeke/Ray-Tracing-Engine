#include "Engine.h"

#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"

int main()
{
  HittableList world;

  world.Add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
  world.Add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

  Camera cam;

  cam.aspectRatio = 16.0 / 9.0;
  cam.imageWidth  = 640;
  cam.samplesPerPixel = 100;
  cam.maxDepth = 50;

  cam.Render(world);
}