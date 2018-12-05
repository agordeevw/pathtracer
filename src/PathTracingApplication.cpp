#include <fstream>
#include <iostream>

#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>

#include "PathTracing/Hitables/MovingSphere.h"
#include "PathTracing/Hitables/Sphere.h"
#include "PathTracing/Materials/Dielectric.h"
#include "PathTracing/Materials/Lambertian.h"
#include "PathTracing/Materials/Metal.h"
#include "PathTracing/Scene.h"
#include "PathTracingApplication.h"
#include "Util/SceneGeneration.h"
#include "Util/TimeMeasure.h"

using Json = nlohmann::json;

namespace {
void loadJsonFromFile(const std::string& path, Json& j) {
  std::ifstream inputFile(path);
  if (!inputFile) {
    throw std::runtime_error("Failed to open input file \"" + path + "\"\n");
  }

  try {
    inputFile >> j;
  } catch (const std::exception& e) {
    throw std::runtime_error("Failed to load JSON from file \"" + path +
                             "\".\nDetails: " + e.what());
  }
}

void parseCameraParams(const Json& j,
                       PathTracing::CameraParameters& cameraParams) {
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
    cameraParams.shutterOpenTime =
        jCameraParams["shutterOpenTime"].get<float>();
    cameraParams.shutterCloseTime =
        jCameraParams["shutterCloseTime"].get<float>();
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Failed to parse camera parameters "
                                         "from input JSON file.\nDetails: ") +
                             e.what());
  }

  if (cameraParams.aperture < 0.0f)
    throw std::runtime_error("Camera aperture is negative");
  if (cameraParams.aspectRatio <= 0.0f)
    throw std::runtime_error("Camera aspect ratio is zero or negative");
  if (cameraParams.focusDist <= 0.0f)
    throw std::runtime_error("Camera focus distance is zero or negative");
  if (cameraParams.fov <= 0.0f || cameraParams.fov >= 180.0f)
    throw std::runtime_error(
        "Camera vertical fov is not in interval (0.0, 180.0)");
  if (cameraParams.shutterCloseTime < cameraParams.shutterOpenTime)
    throw std::runtime_error(
        "Camera shutter close time is earlier that open time");
}

void parseTracingParams(const Json& j,
                        PathTracing::TracingParameters& tracingParams) {
  try {
    auto& jTracingParams = j["tracingParameters"];
    tracingParams.imageWidth = jTracingParams["imageWidth"].get<int>();
    tracingParams.imageHeight = jTracingParams["imageHeight"].get<int>();
    tracingParams.samplesPerPixel =
        jTracingParams["samplesPerPixel"].get<int>();
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Failed to parse tracing parameters "
                                         "from input JSON file.\nDetails: ") +
                             e.what());
  }

  if (tracingParams.imageHeight <= 0)
    throw std::runtime_error("Image height <= 0");
  if (tracingParams.imageWidth <= 0)
    throw std::runtime_error("Image width <= 0");
  if (tracingParams.samplesPerPixel <= 0)
    throw std::runtime_error("Samples per pixel <= 0");
  if (tracingParams.threadsCount < 1)
    throw std::runtime_error("Threads count < 1");
}

void parseScene(const Json& j, PathTracing::Scene& scene) {
  try {
    using namespace PathTracing::Materials;
    using namespace PathTracing::Hitables;

    auto& jScene = j["scene"];
    auto& jMaterials = jScene["materials"];
    for (auto& jMaterial : jMaterials) {
      std::string type = jMaterial["type"].get<std::string>();
      if (type == "lambertian") {
        glm::vec3 albedo;
        for (int i = 0; i < 3; i++)
          albedo[i] = jMaterial["albedo"][i].get<float>();
        scene.createMaterial<Lambertian>(albedo);
      } else if (type == "metal") {
        glm::vec3 albedo;
        float fuzziness;
        for (int i = 0; i < 3; i++)
          albedo[i] = jMaterial["albedo"][i].get<float>();
        fuzziness = jMaterial["fuzziness"].get<float>();
        scene.createMaterial<Metal>(albedo, fuzziness);
      } else if (type == "dielectric") {
        float reflectiveIdx;
        reflectiveIdx = jMaterial["reflectiveIdx"].get<float>();
        scene.createMaterial<Dielectric>(reflectiveIdx);
      } else {
        throw std::runtime_error("Unknown material type: " + type);
      }
    }
    auto& jHitables = jScene["hitables"];
    for (auto& jHitable : jHitables) {
      std::string type = jHitable["type"].get<std::string>();
      if (type == "sphere") {
        glm::vec3 center;
        float radius;
        int materialId;
        for (auto i = 0; i < 3; i++)
          center[i] = jHitable["center"][i].get<float>();
        radius = jHitable["radius"].get<float>();
        materialId = jHitable["material"].get<int>();
        scene.createHitable<Sphere>(center, radius,
                                    scene.getMaterialById(materialId));
      } else if (type == "moving sphere") {
        glm::vec3 startCenter;
        glm::vec3 finishCenter;
        float startTime;
        float finishTime;
        float radius;
        int materialId;
        for (auto i = 0; i < 3; i++)
          startCenter[i] = jHitable["start center"][i].get<float>();
        for (auto i = 0; i < 3; i++)
          finishCenter[i] = jHitable["finish center"][i].get<float>();
        startTime = jHitable["start time"].get<float>();
        finishTime = jHitable["finish time"].get<float>();
        radius = jHitable["radius"].get<float>();
        materialId = jHitable["material"].get<int>();
        scene.createHitable<MovingSphere>(startCenter, finishCenter, startTime,
                                          finishTime, radius,
                                          scene.getMaterialById(materialId));
      } else {
        throw std::runtime_error("Unknown hitable type: " + type);
      }
    }
  } catch (const std::exception& e) {
    throw std::runtime_error(
        std::string("Failed to parse scene from input JSON file.\nDetails: ") +
        e.what());
  }
}
}  // namespace

PathTracingApplication::PathTracingApplication(const std::string& inputFilePath,
                                               int threadCount) {
  tracingParams.threadsCount = threadCount;
  loadDescription(inputFilePath);
}

void PathTracingApplication::run(const std::string& outputFile) {
  PathTracing::Camera camera{cameraParams};
  Util::Image image{};

  std::cout << "Tracing...\n";
  float traceTime = Util::measureExecutionTime([this, &camera, &image]() {
    image = PathTracing::traceScene(scene, camera, tracingParams);
  });
  std::cout << "Done after " << traceTime << " s.\n";

  if (!image.writeToFile(outputFile.c_str())) {
    throw std::runtime_error("Failed to write image to file \"" + outputFile);
  }

  std::cout << "Output written to file \"" << outputFile << "\"\n";
}

void PathTracingApplication::loadDescription(const std::string& path) {
  Json j;
  loadJsonFromFile(path, j);
  parseCameraParams(j, cameraParams);
  parseTracingParams(j, tracingParams);
  parseScene(j, scene);
}
