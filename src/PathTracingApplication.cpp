#include <fstream>
#include <iostream>
#include <type_traits>

#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>

#include "PathTracing/Hitables/MovingSphere.h"
#include "PathTracing/Hitables/Sphere.h"
#include "PathTracing/Materials/Dielectric.h"
#include "PathTracing/Materials/Lambertian.h"
#include "PathTracing/Materials/Metal.h"
#include "PathTracing/Scene.h"
#include "PathTracing/Textures/CheckerTexture.h"
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

inline glm::vec3 parseVec3(const Json& jVec3) {
  glm::vec3 ret;
  for (int i = 0; i < 3; i++) ret[i] = jVec3[i].get<float>();
  return ret;
}

void parseCameraParams(const Json& jCameraParams,
                       PathTracing::CameraParameters& cameraParams) {
  try {
    cameraParams.lookFrom = parseVec3(jCameraParams["lookFrom"]);
    cameraParams.lookAt = parseVec3(jCameraParams["lookAt"]);
    cameraParams.up = parseVec3(jCameraParams["up"]);
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

void parseTracingParams(const Json& jTracingParams,
                        PathTracing::TracingParameters& tracingParams) {
  try {
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

template <class T>
constexpr char* getTypeString();

template <>
constexpr char* getTypeString<PathTracing::Textures::ConstantTexture>() {
  return "constant";
}

template <>
constexpr char* getTypeString<PathTracing::Textures::CheckerTexture>() {
  return "checker";
}

template <class T>
void parseTexture(const Json& jTexture, PathTracing::Scene& scene);

template <>
void parseTexture<PathTracing::Textures::ConstantTexture>(
    const Json& jTexture, PathTracing::Scene& scene) {
  glm::vec3 color = parseVec3(jTexture["color"]);
  scene.createTexture<PathTracing::Textures::ConstantTexture>(color);
}

template <>
void parseTexture<PathTracing::Textures::CheckerTexture>(
    const Json& jTexture, PathTracing::Scene& scene) {
  int evenTextureId = jTexture["even"].get<int>();
  int oddTextureId = jTexture["odd"].get<int>();
  // Major problem here: loader can only reference loaded textures
  // Possible error in description file - reference loops
  // How to prevent?
  scene.createTexture<PathTracing::Textures::CheckerTexture>(
      scene.getTextureById(evenTextureId), scene.getTextureById(oddTextureId));
}

void parseSceneTextures(const Json& jTextures, PathTracing::Scene& scene) {
  try {
    using namespace PathTracing::Textures;

    for (auto& jTexture : jTextures) {
      std::string type = jTexture["type"].get<std::string>();

      if (type == getTypeString<ConstantTexture>()) {
        parseTexture<ConstantTexture>(jTexture, scene);
        continue;
      }

      if (type == getTypeString<CheckerTexture>()) {
        parseTexture<CheckerTexture>(jTexture, scene);
        continue;
      }

      throw std::runtime_error("Unknown texture type: " + type);
    }
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Failed to parse scene textures from "
                                         "input JSON file.\nDetails: ") +
                             e.what());
  }
}

template <>
constexpr char* getTypeString<PathTracing::Materials::Dielectric>() {
  return "dielectric";
}

template <>
constexpr char* getTypeString<PathTracing::Materials::Metal>() {
  return "metal";
}

template <>
constexpr char* getTypeString<PathTracing::Materials::Lambertian>() {
  return "lambertian";
}

template <class T>
void parseMaterial(const Json& jMaterial, PathTracing::Scene& scene);

template <>
void parseMaterial<PathTracing::Materials::Dielectric>(
    const Json& jMaterial, PathTracing::Scene& scene) {
  float reflectiveIdx = jMaterial["reflectiveIdx"].get<float>();
  scene.createMaterial<PathTracing::Materials::Dielectric>(reflectiveIdx);
}

template <>
void parseMaterial<PathTracing::Materials::Metal>(const Json& jMaterial,
                                                  PathTracing::Scene& scene) {
  int albedoTextureId = jMaterial["albedo"].get<int>();
  float fuzziness = jMaterial["fuzziness"].get<float>();
  scene.createMaterial<PathTracing::Materials::Metal>(
      scene.getTextureById(albedoTextureId), fuzziness);
}

template <>
void parseMaterial<PathTracing::Materials::Lambertian>(
    const Json& jMaterial, PathTracing::Scene& scene) {
  int albedoTextureId = jMaterial["albedo"].get<int>();
  scene.createMaterial<PathTracing::Materials::Lambertian>(
      scene.getTextureById(albedoTextureId));
}

void parseSceneMaterials(const Json& jMaterials, PathTracing::Scene& scene) {
  try {
    using namespace PathTracing::Materials;

    for (auto& jMaterial : jMaterials) {
      std::string type = jMaterial["type"].get<std::string>();

      if (type == getTypeString<Lambertian>()) {
        parseMaterial<Lambertian>(jMaterial, scene);
        continue;
      }

      if (type == getTypeString<Metal>()) {
        parseMaterial<Metal>(jMaterial, scene);
        continue;
      }

      if (type == getTypeString<Dielectric>()) {
        parseMaterial<Dielectric>(jMaterial, scene);
        continue;
      }

      throw std::runtime_error("Unknown material type: " + type);
    }
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Failed to parse scene materials from "
                                         "input JSON file.\nDetails: ") +
                             e.what());
  }
}

template <>
constexpr char* getTypeString<PathTracing::Hitables::Sphere>() {
  return "sphere";
}

template <>
constexpr char* getTypeString<PathTracing::Hitables::MovingSphere>() {
  return "moving sphere";
}

template <class T>
void parseHitable(const Json& jHitable, PathTracing::Scene& scene);

template <>
void parseHitable<PathTracing::Hitables::Sphere>(const Json& jHitable,
                                                 PathTracing::Scene& scene) {
  glm::vec3 center = parseVec3(jHitable["center"]);
  float radius = jHitable["radius"].get<float>();
  int materialId = jHitable["material"].get<int>();
  scene.createHitable<PathTracing::Hitables::Sphere>(
      center, radius, scene.getMaterialById(materialId));
}

template <>
void parseHitable<PathTracing::Hitables::MovingSphere>(
    const Json& jHitable, PathTracing::Scene& scene) {
  glm::vec3 startCenter = parseVec3(jHitable["start center"]);
  glm::vec3 finishCenter = parseVec3(jHitable["finish center"]);
  float startTime = jHitable["start time"].get<float>();
  float finishTime = jHitable["finish time"].get<float>();
  float radius = jHitable["radius"].get<float>();
  int materialId = jHitable["material"].get<int>();
  scene.createHitable<PathTracing::Hitables::MovingSphere>(
      startCenter, finishCenter, startTime, finishTime, radius,
      scene.getMaterialById(materialId));
}

void parseSceneHitables(const Json& jHitables, PathTracing::Scene& scene) {
  try {
    using namespace PathTracing::Hitables;

    for (auto& jHitable : jHitables) {
      std::string type = jHitable["type"].get<std::string>();

      if (type == getTypeString<Sphere>()) {
        parseHitable<Sphere>(jHitable, scene);
        continue;
      }

      if (type == getTypeString<MovingSphere>()) {
        parseHitable<MovingSphere>(jHitable, scene);
        continue;
      }

      throw std::runtime_error("Unknown hitable type: " + type);
    }
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Failed to parse scene hitables from "
                                         "input JSON file.\nDetails: ") +
                             e.what());
  }
}

void parseScene(const Json& jScene, PathTracing::Scene& scene) {
  parseSceneTextures(jScene["textures"], scene);
  parseSceneMaterials(jScene["materials"], scene);
  parseSceneHitables(jScene["hitables"], scene);
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
  parseCameraParams(j["cameraParameters"], cameraParams);
  parseTracingParams(j["tracingParameters"], tracingParams);
  parseScene(j["scene"], scene);
}
