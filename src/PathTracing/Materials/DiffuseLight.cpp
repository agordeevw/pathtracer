#include "PathTracing/Materials/DiffuseLight.h"
#include "PathTracing/Hitable.h"
#include "PathTracing/Ray.h"
#include "Util/Random.h"

namespace PathTracing {
namespace Materials {
DiffuseLight::DiffuseLight(const Texture& emission) : emission(emission) {}
bool DiffuseLight::scatter(const Ray& rayIn, const HitRecord& rec,
                           glm::vec3& attenuation, Ray& scattered) const {
  return false;
}
glm::vec3 DiffuseLight::emit(float u, float v, const glm::vec3& p) const {
  return emission.sample(u, v, p);
}
}  // namespace Materials
}  // namespace PathTracing