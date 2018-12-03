#pragma once

#include <glm/vec3.hpp>

namespace PathTracing {
class Ray {
 public:
  glm::vec3 pointAtT(float t) const { return origin + t * direction; }

  glm::vec3 origin;
  glm::vec3 direction;
};
}  // namespace PathTracing