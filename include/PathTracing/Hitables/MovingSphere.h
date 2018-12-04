#pragma once
#include <glm/vec3.hpp>

#include "PathTracing/Hitable.h"

namespace PathTracing {
namespace Hitables {
class MovingSphere : public Hitable {
 public:
  MovingSphere(const glm::vec3& startCenter, const glm::vec3& finishCenter,
               float startTime, float finishTime, float radius,
               const Material& material);
  ~MovingSphere() = default;

  bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;

  glm::vec3 centerAtTime(float time) const;

  glm::vec3 startCenter;
  glm::vec3 finishCenter;
  float startTime;
  float finishTime;
  float radius;
  const Material* material;
};
}  // namespace Hitables
}  // namespace PathTracing