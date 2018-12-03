#include <fstream>
#include <iostream>

#pragma warning(disable : 4201)  // glm hits this warning a lot
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "Camera.h"
#include "PathTracing.h"
#include "Scene.h"
#include "Util/ParseCommandLineArgs.h"
#include "Util/SceneGeneration.h"

int main(int argc, char** argv) {
  Util::ParseArgs::ProgramOptions opts;
  if (!Util::ParseArgs::parseArgs(argc, argv, opts)) {
    return 1;
  }

  std::ifstream inputFile(opts.inputPath);
  if (!inputFile) {
    std::cerr << "Failed to open file \"" << opts.inputPath << "\"\n";
    return 1;
  }

  using namespace PathTracing;

  TracingParameters tracingParams{};
  CameraParameters cameraParams{};

  try {
    using Json = nlohmann::json;

    Json j;
    inputFile >> j;

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
    std::cerr << "Error: failed to parse input JSON file.\nDetails:\n"
              << e.what();
    return 1;
  }
  tracingParams.threadsCount = opts.threadCount;

  PathTracing::Scene scene = Util::SceneGeneration::randomSpheres();
  PathTracing::Camera camera{cameraParams};

  std::cout << "Tracing...\n";
  Util::Image image = PathTracing::traceScene(scene, camera, tracingParams);

  std::cout << "Done.\n";
  image.writeToFile(opts.outputPath.c_str());

  std::cout << "Output written to file \"" << opts.outputPath << "\"\n";
  return 0;
}
