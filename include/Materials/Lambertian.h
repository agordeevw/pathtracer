#pragma once
#include "Material.h"

namespace PathTracing {
namespace Materials {
class Lambertian : public Material {
 public:
  Lambertian(const glm::vec3& a);
  ~Lambertian() override = default;

  bool scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation,
               Ray& scattered) const override;

  glm::vec3 albedo;
};
}  // namespace Materials
}  // namespace PathTracing