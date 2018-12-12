#pragma once
#include "PathTracing/Camera.h"
#include "PathTracing/PathTracing.h"
#include "PathTracing/Scene.h"

namespace PathTracing {
struct Description {
  Scene scene;
  CameraParameters cameraParams;
  TracingParameters tracingParams;
};
}  // namespace PathTracing