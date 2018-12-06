#pragma once
#include "PathTracing/Material.h"
#include "PathTracing/Texture.h"

namespace PathTracing {
namespace Materials {
class Lambertian : public Material {
 public:
  Lambertian(const Texture& albedo);
  ~Lambertian() override = default;

  bool scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation,
               Ray& scattered) const override;

  const Texture& albedo;
};
}  // namespace Materials
}  // namespace PathTracing