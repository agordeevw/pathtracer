#include "PathTracing/Materials/Lambertian.h"
#include "PathTracing/Hitable.h"
#include "PathTracing/Ray.h"
#include "Util/Random.h"

namespace PathTracing {
namespace Materials {
Lambertian::Lambertian(const Texture& albedo) : albedo(albedo) {}
bool Lambertian::scatter(const Ray& rayIn, const HitRecord& rec,
                         glm::vec3& attenuation, Ray& scattered) const {
  glm::vec3 target = rec.point + rec.normal + Util::Random::randInUnitSphere();
  scattered = Ray{rec.point, target - rec.point, rayIn.time};
  attenuation = albedo.sample(rec.u, rec.v, rec.point);
  return true;
}
}  // namespace Materials
}  // namespace PathTracing