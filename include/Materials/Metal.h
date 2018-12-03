#pragma once
#include <glm/vec3.hpp>

#include "Material.h"

class Metal : public Material {
 public:
  Metal(const glm::vec3& albedo, float fuzziness);
  ~Metal() override = default;

  bool scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation,
               Ray& scattered) const override;

  glm::vec3 albedo;
  float fuzziness;
};