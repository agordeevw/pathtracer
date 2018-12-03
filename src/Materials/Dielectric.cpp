#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "Hitable.h"
#include "Materials/Dielectric.h"
#include "Ray.h"
#include "Util/Random.h"

namespace {
float schlick(float cosine, float reflectiveIdx) {
  float r0 = (1 - reflectiveIdx) / (1 + reflectiveIdx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * powf(1 - cosine, 5);
}
}  // namespace

namespace PathTracing {
namespace Materials {
Dielectric::Dielectric(float ri) : reflectiveIdx{ri} {}

bool Dielectric::scatter(const Ray& rayIn, const HitRecord& rec,
                         glm::vec3& attenuation, Ray& scattered) const {
  using Util::Random::randf;

  attenuation = glm::vec3{1.0f, 1.0f, 1.0f};
  glm::vec3 outwardNormal;
  float niOverNt;
  float cosine =
      glm::dot(rayIn.direction, rec.normal) / glm::length(rayIn.direction);
  if (glm::dot(rayIn.direction, rec.normal) > 0.0f) {
    outwardNormal = -rec.normal;
    niOverNt = reflectiveIdx;
    cosine *= reflectiveIdx;
  } else {
    outwardNormal = rec.normal;
    niOverNt = 1.0f / reflectiveIdx;
    cosine *= -1;
  }

  glm::vec3 reflected = glm::reflect(rayIn.direction, rec.normal);
  glm::vec3 refracted =
      glm::refract(glm::normalize(rayIn.direction), outwardNormal, niOverNt);

  if (refracted != glm::vec3{0.0f}) {
    float reflectionProba = schlick(cosine, reflectiveIdx);
    if (randf() < reflectionProba) {
      scattered = Ray{rec.point, reflected};
    } else {
      scattered = Ray{rec.point, refracted};
    }
  } else {
    scattered = Ray{rec.point, reflected};
  }
  return true;
}
}  // namespace Materials
}  // namespace PathTracing