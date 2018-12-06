#include "PathTracing/Texture.h"

namespace PathTracing {
namespace Textures {
class ConstantTexture : public Texture {
 public:
  ConstantTexture() = default;
  ConstantTexture(const glm::vec3& color) : color(color) {}
  ~ConstantTexture() override = default;

  glm::vec3 sample(float u, float v, const glm::vec3& p) const override {
    return color;
  }

 private:
  glm::vec3 color;
};
}  // namespace Textures
}  // namespace PathTracing