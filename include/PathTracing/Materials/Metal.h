#pragma once
#include <glm/vec3.hpp>

#include "PathTracing/Material.h"
#include "PathTracing/Texture.h"

namespace PathTracing {
namespace Materials {
class Metal : public Material {
 public:
  Metal(const Texture& albedo, float fuzziness);
  ~Metal() override = default;

  bool scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation,
               Ray& scattered) const override;

  const Texture& albedo;
  float fuzziness;
};
}  // namespace Materials
}  // namespace PathTracing