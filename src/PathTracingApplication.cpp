#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

#include "PathTracing/Scene.h"
#include "PathTracingApplication.h"
#include "Util/SceneGeneration.h"

PathTracingApplication::PathTracingApplication(const std::string& inputFilePath,
                                               int threadCount) {
  tracingParams.threadsCount = threadCount;

  Json j;
  loadJsonFromFile(inputFilePath, j);
  parseInputJson(j);

  validateParameters();
}

void PathTracingApplication::run(const std::string& outputFile) {
  PathTracing::Scene scene = Util::SceneGeneration::randomSpheres();
  PathTracing::Camera camera{cameraParams};

  std::cout << "Tracing...\n";
  Util::Image image = PathTracing::traceScene(scene, camera, tracingParams);

  std::cout << "Done.\n";
  if (!image.writeToFile(outputFile.c_str())) {
    throw std::runtime_error("Failed to write image to file \"" + outputFile);
  }

  std::cout << "Output written to file \"" << outputFile << "\"\n";
}

void PathTracingApplication::loadJsonFromFile(const std::string& path,
                                              Json& j) {
  std::ifstream inputFile(path);
  if (!inputFile) {
    throw std::runtime_error("Failed to open file \"" + path + "\"\n");
  }

  try {
    inputFile >> j;
  } catch (const std::exception& e) {
    throw std::runtime_error("Failed to load JSON from file \"" + path +
                             "\".\nDetails: " + e.what());
  }
}

void PathTracingApplication::parseInputJson(const Json& j) {
  try {
    auto& jCameraParams = j["cameraParameters"];
    for (int i = 0; i < 3; i++)
      cameraParams.lookFrom[i] = jCameraParams["lookFrom"][i].get<float>();
    for (int i = 0; i < 3; i++)
      cameraParams.lookAt[i] = jCameraParams["lookAt"][i].get<float>();
    for (int i = 0; i < 3; i++)
      cameraParams.up[i] = jCameraParams["up"][i].get<float>();
    cameraParams.fov = jCameraParams["fov"].get<float>();
    cameraParams.aspectRatio = jCameraParams["aspectRatio"].get<float>();
    cameraParams.aperture = jCameraParams["aperture"].get<float>();
    cameraParams.focusDist = jCameraParams["focusDist"].get<float>();

    auto& jTracingParams = j["tracingParameters"];
    tracingParams.imageWidth = jTracingParams["imageWidth"].get<int>();
    tracingParams.imageHeight = jTracingParams["imageHeight"].get<int>();
    tracingParams.samplesPerPixel =
        jTracingParams["samplesPerPixel"].get<int>();
  } catch (const std::exception& e) {
    throw std::runtime_error(
        std::string(
            "Failed to parse parameters from input JSON file.\nDetails: ") +
        e.what());
  }
}

void PathTracingApplication::validateParameters() {
  if (tracingParams.imageHeight <= 0)
    throw std::runtime_error("Image height <= 0");
  if (tracingParams.imageWidth <= 0)
    throw std::runtime_error("Image width <= 0");
  if (tracingParams.samplesPerPixel <= 0)
    throw std::runtime_error("Samples per pixel <= 0");
  if (tracingParams.threadsCount < 1)
    throw std::runtime_error("Threads count < 1");

  if (cameraParams.aperture <= 0.0f)
    throw std::runtime_error("Camera aperture is zero or negative");
  if (cameraParams.aspectRatio <= 0.0f)
    throw std::runtime_error("Camera aspect ratio is zero or negative");
  if (cameraParams.focusDist <= 0.0f)
    throw std::runtime_error("Camera focus distance is zero or negative");
  if (cameraParams.fov <= 0.0f || cameraParams.fov >= 180.0f)
    throw std::runtime_error(
        "Camera vertical fov is not in interval (0.0, 180.0)");
}
