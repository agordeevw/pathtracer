#pragma once
#include "Util/Image.h"

namespace PathTracing {
struct TracingParameters {
  int imageWidth;
  int imageHeight;
  int samplesPerPixel;
  int threadsCount;
};

class Scene;
class Camera;

Util::Image traceScene(const Scene& scene, const Camera& camera, const TracingParameters& params);
}  // namespace PathTracing