#pragma warning(disable : 4201)  // glm hits this warning a lot
#include <glm/glm.hpp>

#include "Camera.h"
#include "PathTracing.h"
#include "Scene.h"
#include "Util/SceneGeneration.h"

constexpr int width = 800, height = 600, samplesPerPixel = 512, threadsCount = 4;

Camera setupCamera() {
  glm::vec3 lookFrom{7.0f, 1.5f, 2.5f};
  glm::vec3 lookAt{0.0f, 0.0f, 0.0f};
  glm::vec3 up{0.0f, 1.0f, 0.0f};
  float fov = 45.0f;
  float aspectRatio = float(width) / float(height);
  float aperture = 0.1f;
  float focusDist = glm::length(lookFrom - lookAt);
  return {lookFrom, lookAt, up, fov, aspectRatio, aperture, focusDist};
}

int main() {
  using namespace PathTracing;

  Scene scene = SceneGeneration::randomSpheres();
  Camera camera = setupCamera();

  TracingParameters params;
  params.imageWidth = width;
  params.imageHeight = height;
  params.samplesPerPixel = samplesPerPixel;
  params.threadsCount = threadsCount;

  Image image = traceScene(scene, camera, params);
  image.writeToFile("pathtrace.bmp");

  return 0;
}
