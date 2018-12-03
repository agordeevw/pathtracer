#pragma once
#include <glm/vec3.hpp>

namespace PathTracing {
class Ray;
class Material;

struct HitRecord {
  float t;
  glm::vec3 point;
  glm::vec3 normal;
  const Material* material;
};

class Hitable {
 public:
  virtual ~Hitable() = default;

  virtual bool hit(const Ray& r, float tMin, float tMax,
                   HitRecord& rec) const = 0;
};
}  // namespace PathTracing