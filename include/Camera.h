#pragma once
#include <glm/vec3.hpp>

#include "Ray.h"

namespace PathTracing {
struct CameraParameters {
  glm::vec3 lookFrom;
  glm::vec3 lookAt;
  glm::vec3 up;
  float fov;
  float aspectRatio;
  float aperture;
  float focusDist;
};

class Camera {
 public:
  Camera(const CameraParameters& params);

  Ray getRay(float s, float t) const;

 private:
  glm::vec3 origin;
  glm::vec3 lowerLeftCorner;
  glm::vec3 horizontal;
  glm::vec3 vertical;
  glm::vec3 u, v, w;
  float lensRadius;
};
}  // namespace PathTracing