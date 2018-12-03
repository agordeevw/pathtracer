#pragma warning(disable : 4201)  // glm hits this warning a lot
#include <glm/glm.hpp>

#include "Camera.h"
#include "PathTracing.h"
#include "Scene.h"
#include "Util/SceneGeneration.h"

constexpr int width = 200, height = 100, samplesPerPixel = 64;

int main() {
  using namespace PathTracing;

  Scene scene = SceneGeneration::randomSpheres();

  glm::vec3 lookFrom{7.0f, 1.5f, 2.5f};
  glm::vec3 lookAt{0.0f, 0.0f, 0.0f};
  glm::vec3 up{0.0f, 1.0f, 0.0f};
  float fov = 45.0f;
  float aspectRatio = float(width) / float(height);
  float aperture = 0.1f;
  float focusDist = glm::length(lookFrom - lookAt);
  Camera camera{lookFrom, lookAt, up, fov, aspectRatio, aperture, focusDist};

  Image image = traceScene(scene, camera, width, height, samplesPerPixel);
  image.writeToFile("sample.bmp");

  return 0;
}
