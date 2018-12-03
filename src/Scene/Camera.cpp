#define _USE_MATH_DEFINES
#include <cmath>

#include <glm/geometric.hpp>

#include "Scene/Camera.h"
#include "Util/MyRandom.h"

Camera::Camera(const glm::vec3& lookFrom, const glm::vec3& lookAt,
               const glm::vec3& up, float verticalFov, float aspectRatio,
               float aperture, float focusDist) {
  lensRadius = aperture * 0.5f;
  float theta = verticalFov * float(M_PI) / 180.0f;
  float halfHeight = tanf(theta * 0.5f);
  float halfWidth = aspectRatio * halfHeight;
  origin = lookFrom;
  w = glm::normalize(lookFrom - lookAt);
  u = glm::normalize(glm::cross(up, w));
  v = glm::cross(w, u);
  lowerLeftCorner = origin - focusDist * (halfWidth * u + halfHeight * v + w);
  horizontal = 2.0f * halfWidth * focusDist * u;
  vertical = 2.0f * halfHeight * focusDist * v;
}

Ray Camera::getRay(float s, float t) const {
  glm::vec3 rd = lensRadius * MyRandom::randInUnitDisk();
  glm::vec3 offset = u * rd.x + v * rd.y;
  return {origin + offset,
          lowerLeftCorner + s * horizontal + t * vertical - (origin + offset)};
}
