#pragma once
#include <glm/geometric.hpp>
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
    return glm::vec3{0.5} *
           (1.0f + sinf(p.z * scale + 10.0f * turbulence(p * scale)));
  }

 private:
  float turbulence(const glm::vec3& p, int depth = 7) const {
    float accum = 0;
    glm::vec3 tmp = p;
    float weight = 1.0f;
    for (int i = 0; i < depth; i++) {
      accum += weight * noise.sample(tmp);
      weight *= 0.5f;
      tmp *= 2.0f;
    }
    return fabsf(accum);
  }

  Util::PerlinNoise noise;
  float scale;
};

}  // namespace Textures
}  // namespace PathTracing