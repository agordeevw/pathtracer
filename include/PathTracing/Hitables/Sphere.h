#pragma once
#include <glm/vec3.hpp>

#include "PathTracing/Hitable.h"

namespace PathTracing {
namespace Hitables {
class Sphere : public Hitable {
 public:
  Sphere(const glm::vec3& center, float radius, const Material& material);
  ~Sphere() override = default;

  bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;
  bool boundingBox(float t0, float t1, AABB& box) const override;

  glm::vec3 center;
  float radius;
  const Material* material;
};
}  // namespace Hitables
}  // namespace PathTracing