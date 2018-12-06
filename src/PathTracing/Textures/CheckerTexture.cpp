#include "PathTracing/Textures/CheckerTexture.h"

namespace PathTracing {
namespace Textures {
CheckerTexture::CheckerTexture(const Texture& even, const Texture& odd)
    : even(even), odd(odd) {}
glm::vec3 CheckerTexture::sample(float u, float v, const glm::vec3& p) const {
  float sines =
      std::sinf(10.0f * p.x) * std::sinf(10.0f * p.y) * std::sinf(10.0f * p.z);
  return sines < 0.0f ? odd.sample(u, v, p) : even.sample(u, v, p);
}
}  // namespace Textures
}  // namespace PathTracing