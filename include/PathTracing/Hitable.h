#pragma once
#include <glm/vec3.hpp>

namespace PathTracing {
class AABB;
class Material;
class Ray;

struct HitRecord {
  float t;
  glm::vec3 point;
  glm::vec3 normal;
  float u;
  float v;
  const Material* material;
};

class Hitable {
 public:
  virtual ~Hitable() = default;

  virtual bool hit(const Ray& r, float tMin, float tMax,
                   HitRecord& rec) const = 0;
  virtual bool boundingBox(float t0, float t1, AABB& box) const = 0;
};
}  // namespace PathTracing