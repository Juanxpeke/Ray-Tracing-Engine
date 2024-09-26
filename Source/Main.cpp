#include "Engine.h"

#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Material.h"
#include "Sphere.h"

int main()
{
  HittableList world;

  auto groundMaterial = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

  for (int a = -11; a < 11; a++)
  {
    for (int b = -11; b < 11; b++)
    {
      auto chooseMat = RandomDouble();
      Point3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

      if ((center - Point3(4, 0.2, 0)).Length() > 0.9)
      {
        std::shared_ptr<Material> sphereMaterial;

        if (chooseMat < 0.8)
        {
          // Diffuse
          auto albedo = Color::Random() * Color::Random();
          sphereMaterial = std::make_shared<Lambertian>(albedo);
          world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
        } 
        else if (chooseMat < 0.95)
        {
          // Metal
          auto albedo = Color::Random(0.5, 1);
          auto fuzz = RandomDouble(0, 0.5);
          sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
          world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
        }
        else
        {
          // Glass
          sphereMaterial = std::make_shared<Dielectric>(1.5);
          world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
        }
      }
    }
  }

  auto material1 = std::make_shared<Dielectric>(1.5);
  world.Add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
  world.Add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  world.Add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  Camera cam;

  cam.aspectRatio      = 16.0 / 9.0;
  cam.imageWidth       = 360;
  cam.samplesPerPixel  = 10;
  cam.maxDepth         = 30;

  cam.vFov     = 20;
  cam.lookFrom = Point3(13,2,3);
  cam.lookAt   = Point3(0,0,0);
  cam.vUp      = Vec3(0,1,0);

  cam.defocusAngle = 0.6;
  cam.focusDist    = 10.0;

  cam.Render(world);
}