#pragma once
#include <glm/vec3.hpp>

struct HitRecord;
class Ray;

class Material {
public:
  virtual bool scatter(const Ray& rayIn, const HitRecord& rec,
    glm::vec3& attenuation, Ray& scattered) const = 0;
};