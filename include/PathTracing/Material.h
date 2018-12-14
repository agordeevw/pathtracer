#pragma once
#include <glm/vec3.hpp>

namespace PathTracing {
struct HitRecord;
class Ray;

class Material {
 public:
  virtual ~Material() = default;

  virtual bool scatter(const Ray& rayIn, const HitRecord& rec,
                       glm::vec3& attenuation, Ray& scattered) const = 0;

  virtual glm::vec3 emit(float u, float v, const glm::vec3& p) const {
    return {};
  }
};
}  // namespace PathTracing