#pragma once
#include <glm/vec3.hpp>

#include "Hitable.h"

class Sphere : public Hitable {
 public:
  Sphere(const glm::vec3& center, float radius, const Material& material);
  ~Sphere() override = default;

  bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;

  glm::vec3 center;
  float radius;
  const Material* material;
};