#include "Materials/Lambertian.h"
#include "Hitable.h"
#include "Ray.h"
#include "Util/MyRandom.h"

Lambertian::Lambertian(const glm::vec3& a) : albedo(a) {}

bool Lambertian::scatter(const Ray& rayIn, const HitRecord& rec,
                         glm::vec3& attenuation, Ray& scattered) const {
  glm::vec3 target = rec.point + rec.normal + MyRandom::randInUnitSphere();
  scattered = Ray{rec.point, target - rec.point};
  attenuation = albedo;
  return true;
}
