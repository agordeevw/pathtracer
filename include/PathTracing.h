#pragma once
#include "Util/Image.h"

class Scene;
class Camera;

namespace PathTracing {
Image traceScene(const Scene& scene, const Camera& camera, int imageWidth,
                 int imageHeight, int samplesPerPixel);
}  // namespace PathTracing