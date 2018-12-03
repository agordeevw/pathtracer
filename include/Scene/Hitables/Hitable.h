#pragma once
#include <glm/vec3.hpp>

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
  virtual bool hit(const Ray& r, float tMin, float tMax,
                   HitRecord& rec) const = 0;
};