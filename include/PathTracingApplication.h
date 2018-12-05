#pragma once
#include <string>

#include "PathTracing/Camera.h"
#include "PathTracing/PathTracing.h"
#include "PathTracing/Scene.h"

class PathTracingApplication {
 public:
  PathTracingApplication(const std::string& inputFilePath, int threadCount);
  void run(const std::string& outputFile);

 private:
  void loadDescription(const std::string& path);

  PathTracing::Scene scene;
  PathTracing::CameraParameters cameraParams;
  PathTracing::TracingParameters tracingParams;
};