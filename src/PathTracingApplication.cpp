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
#include "PathTracing/Textures/ConstantTexture.h"
#include "PathTracingApplication.h"
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

inline void parseVec3(const Json& jVec3, glm::vec3& v) {
  for (int i = 0; i < 3; i++) v[i] = jVec3[i].get<float>();
}

void parseCameraParams(const Json& j,
                       PathTracing::CameraParameters& cameraParams) {
  try {
    auto& jCameraParams = j["cameraParameters"];
    parseVec3(jCameraParams["lookFrom"], cameraParams.lookFrom);
    parseVec3(jCameraParams["lookAt"], cameraParams.lookAt);
    parseVec3(jCameraParams["up"], cameraParams.up);
    cameraParams.fov = jCameraParams["fov"].get<float>();
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

void parseSceneTextures(const Json& jScene, PathTracing::Scene& scene) {
  try {
    using namespace PathTracing::Textures;

    auto& jTextures = jScene["textures"];
    for (auto& jTexture : jTextures) {
      std::string type = jTexture["type"].get<std::string>();
      if (type == "constant") {
        glm::vec3 color;
        parseVec3(jTexture["color"], color);
        scene.createTexture<ConstantTexture>(color);
      } else {
        throw std::runtime_error("Unknown texture type: " + type);
      }
    }
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Failed to parse scene textures from "
                                         "input JSON file.\nDetails: ") +
                             e.what());
  }
}

void parseSceneMaterials(const Json& jScene, PathTracing::Scene& scene) {
  try {
    using namespace PathTracing::Materials;

    auto& jMaterials = jScene["materials"];
    for (auto& jMaterial : jMaterials) {
      std::string type = jMaterial["type"].get<std::string>();
      if (type == "lambertian") {
        int albedoTextureId;
        albedoTextureId = jMaterial["albedo"].get<int>();
        scene.createMaterial<Lambertian>(scene.getTextureById(albedoTextureId));
      } else if (type == "metal") {
        int albedoTextureId;
        float fuzziness;
        albedoTextureId = jMaterial["albedo"].get<int>();
        fuzziness = jMaterial["fuzziness"].get<float>();
        scene.createMaterial<Metal>(scene.getTextureById(albedoTextureId),
                                    fuzziness);
      } else if (type == "dielectric") {
        float reflectiveIdx;
        reflectiveIdx = jMaterial["reflectiveIdx"].get<float>();
        scene.createMaterial<Dielectric>(reflectiveIdx);
      } else {
        throw std::runtime_error("Unknown material type: " + type);
      }
    }
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Failed to parse scene materials from "
                                         "input JSON file.\nDetails: ") +
                             e.what());
  }
}

void parseSceneHitables(const Json& jScene, PathTracing::Scene& scene) {
  try {
    using namespace PathTracing::Hitables;

    auto& jHitables = jScene["hitables"];
    for (auto& jHitable : jHitables) {
      std::string type = jHitable["type"].get<std::string>();
      if (type == "sphere") {
        glm::vec3 center;
        float radius;
        int materialId;
        parseVec3(jHitable["center"], center);
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
        parseVec3(jHitable["start center"], startCenter);
        parseVec3(jHitable["finish center"], finishCenter);
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
    throw std::runtime_error(std::string("Failed to parse scene hitables from "
                                         "input JSON file.\nDetails: ") +
                             e.what());
  }
}

void parseScene(const Json& j, PathTracing::Scene& scene) {
  parseSceneTextures(j["scene"], scene);
  parseSceneMaterials(j["scene"], scene);
  parseSceneHitables(j["scene"], scene);
}
}  // namespace

PathTracingApplication::PathTracingApplication(const std::string& inputFilePath,
                                               int threadCount) {
  tracingParams.threadsCount = threadCount;
  loadDescription(inputFilePath);
  cameraParams.aspectRatio =
      (float)tracingParams.imageWidth / (float)tracingParams.imageHeight;
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
