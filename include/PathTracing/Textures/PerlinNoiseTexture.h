#pragma once
#include <glm/vec3.hpp>

#include "PathTracing/Texture.h"
#include "Util/PerlinNoise.h"

namespace PathTracing {
namespace Textures {
class PerlinNoiseTexture : public Texture {
 public:
  PerlinNoiseTexture(unsigned int seed) : noise(seed) {}
  glm::vec3 sample(float u, float v, const glm::vec3& p) const override {
    return glm::vec3{1} * noise.sample(p);
  }

 private:
  Util::PerlinNoise noise;
};

}  // namespace Textures
}  // namespace PathTracing