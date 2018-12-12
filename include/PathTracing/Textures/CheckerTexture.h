#pragma once
#include "PathTracing/Texture.h"

namespace PathTracing {
namespace Textures {
class CheckerTexture : public Texture {
 public:
  CheckerTexture(const Texture& even, const Texture& odd)
      : even(even), odd(odd) {}
  ~CheckerTexture() override = default;

  glm::vec3 sample(float u, float v, const glm::vec3& p) const override {
    float sines = std::sinf(10.0f * p.x) * std::sinf(10.0f * p.y) *
                  std::sinf(10.0f * p.z);
    return sines < 0.0f ? odd.sample(u, v, p) : even.sample(u, v, p);
  }

 private:
  const Texture& even;
  const Texture& odd;
};
}  // namespace Textures
}  // namespace PathTracing