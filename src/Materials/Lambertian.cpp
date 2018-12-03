#include "Materials/Lambertian.h"
#include "Scene/Hitables/Hitable.h"
#include "Util/MyRandom.h"
#include "Ray.h"

Lambertian::Lambertian(const glm::vec3& a) : albedo(a) {}

bool Lambertian::scatter(const Ray& rayIn, const HitRecord& rec,
                         glm::vec3& attenuation, Ray& scattered) const {
  glm::vec3 target = rec.point + rec.normal + MyRandom::randInUnitSphere();
  scattered = Ray{rec.point, target - rec.point};
  attenuation = albedo;
  return true;
}
