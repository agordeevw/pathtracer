#include <glm/geometric.hpp>

#include "PathTracing/AABB.h"

namespace PathTracing {
AABB::AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

bool AABB::hit(const Ray& r, float tmin, float tmax) const {
  for (int i = 0; i < 3; i++) {
    if (std::abs(r.direction[i]) < 1e-5) {
      if (r.origin[i] < min[i] || r.origin[i] > max[i]) return false;
    } else {
      float ood = 1.0f / r.direction[i];
      float t0 = (min[i] - r.origin[i]) * ood;
      float t1 = (max[i] - r.origin[i]) * ood;
      if (t0 > t1) {
        float tmp = t0;
        t0 = t1;
        t1 = tmp;
      }
      tmin = t0 > tmin ? t0 : tmin;
      tmax = t1 < tmax ? t1 : tmax;
      if (tmax <= tmin) return false;
    }
  }
  return true;
}

AABB AABB::surroundingBox(const AABB& a, const AABB& b) {
  return {glm::min(a.min, b.min), glm::max(a.max, b.max)};
}
}  // namespace PathTracing