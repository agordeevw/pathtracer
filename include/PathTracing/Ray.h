#pragma once

#include <glm/vec3.hpp>

namespace PathTracing {
class Ray {
 public:
   Ray() = default;
   Ray(const glm::vec3& origin, const glm::vec3& direction, float time = 0.0f) :
     origin(origin), direction(direction), time(time) {}

  glm::vec3 pointAtT(float t) const { return origin + t * direction; }

  glm::vec3 origin;
  glm::vec3 direction;
  float time;
};
}  // namespace PathTracing