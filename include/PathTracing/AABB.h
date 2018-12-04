#pragma once
#include <glm/vec3.hpp>

#include "PathTracing/Ray.h"

namespace PathTracing {
class AABB {
 public:
  AABB() = default;
  AABB(const glm::vec3& min, const glm::vec3& max);

  bool hit(const Ray& r, float tmin, float tmax) const;

  static AABB surroundingBox(const AABB& a, const AABB& b);

  glm::vec3 min;
  glm::vec3 max;
};

}  // namespace PathTracing