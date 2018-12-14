#pragma once
#include "PathTracing/Material.h"
#include "PathTracing/Texture.h"

namespace PathTracing {
namespace Materials {
class DiffuseLight : public Material {
 public:
  DiffuseLight(const Texture& emission);
  ~DiffuseLight() override = default;

  bool scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation,
               Ray& scattered) const override;

  glm::vec3 emit(float u, float v, const glm::vec3& p) const override;

  const Texture& emission;
};
}  // namespace Materials
}  // namespace PathTracing