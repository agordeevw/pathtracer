#pragma once
#include <string>

#include <nlohmann/json_fwd.hpp>

#include "PathTracing/Camera.h"
#include "PathTracing/PathTracing.h"

class PathTracingApplication {
  using Json = nlohmann::json;

 public:
  PathTracingApplication(const std::string& inputFilePath, int threadCount);
  void run(const std::string& outputFile);

 private:
  void loadJsonFromFile(const std::string& path, Json& json);
  void parseInputJson(const Json& j);
  void validateParameters();

  PathTracing::TracingParameters tracingParams;
  PathTracing::CameraParameters cameraParams;
};