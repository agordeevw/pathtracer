#pragma once
#include "Util/Image.h"

class Scene;
class Camera;

namespace PathTracing {
struct TracingParameters {
  int imageWidth;
  int imageHeight;
  int samplesPerPixel;
  int threadsCount;
};

Image traceScene(const Scene& scene, const Camera& camera, const TracingParameters& params);
}  // namespace PathTracing