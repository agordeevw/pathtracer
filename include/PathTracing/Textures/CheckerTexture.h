#pragma once
#include "PathTracing/Texture.h"

namespace PathTracing {
namespace Textures {
class CheckerTexture : public Texture {
 public:
  CheckerTexture(const Texture& even, const Texture& odd);
  ~CheckerTexture() override = default;

  glm::vec3 sample(float u, float v, const glm::vec3& p) const override;

 private:
  const Texture& even;
  const Texture& odd;
};
}  // namespace Textures
}  // namespace PathTracing