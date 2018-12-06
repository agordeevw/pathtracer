#pragma once
#include <glm/vec3.hpp>

namespace PathTracing {
class Texture {
 public:
  virtual ~Texture() = default;

  virtual glm::vec3 sample(float u, float v, const glm::vec3& p) const = 0;
};
}  // namespace PathTracing