#pragma once
#include <glm/vec3.hpp>

#include "PathTracing/Texture.h"
#include "Util/PerlinNoise.h"

namespace PathTracing {
namespace Textures {
class PerlinNoiseTexture : public Texture {
 public:
  PerlinNoiseTexture(unsigned int seed, float scale)
      : noise(seed), scale(scale) {}
  glm::vec3 sample(float u, float v, const glm::vec3& p) const override {
    return glm::vec3{1} * noise.sample(p * scale);
  }

 private:
  Util::PerlinNoise noise;
  float scale;
};

}  // namespace Textures
}  // namespace PathTracing