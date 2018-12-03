#pragma once
#include <glm/vec3.hpp>

#include "Scene/Hitables/Hitable.h"

class Sphere : public Hitable {
 public:
  Sphere() = default;
  Sphere(const glm::vec3& center, float radius, const Material& material);

  bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;

  glm::vec3 center;
  float radius;
  const Material* material;
};