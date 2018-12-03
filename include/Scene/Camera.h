#pragma once
#include <glm/vec3.hpp>

#include "Ray.h"

class Camera {
 public:
  Camera(const glm::vec3& lookFrom, const glm::vec3& lookAt,
         const glm::vec3& up, float verticalFov, float aspectRatio,
         float aperture, float focusDist);

  Ray getRay(float s, float t) const;

 private:
  glm::vec3 origin;
  glm::vec3 lowerLeftCorner;
  glm::vec3 horizontal;
  glm::vec3 vertical;
  glm::vec3 u, v, w;
  float lensRadius;
};