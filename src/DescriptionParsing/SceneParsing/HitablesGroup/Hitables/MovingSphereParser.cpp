#include <glm/vec3.hpp>

#include "DescriptionParsing/SceneElementJsonParser.h"
#include "PathTracing/Hitables/MovingSphere.h"
#include "PathTracing/Scene.h"

namespace DescriptionParsing {
using Type = PathTracing::Hitables::MovingSphere;

const char* SceneElementJsonParser<Type>::name() { return "moving sphere"; }

void SceneElementJsonParser<Type>::parse(const Json& jHitable,
                                         PathTracing::Scene& scene) {
  glm::vec3 startCenter = parseVec3(jHitable["start center"]);
  glm::vec3 finishCenter = parseVec3(jHitable["finish center"]);
  float startTime = jHitable["start time"].get<float>();
  float finishTime = jHitable["finish time"].get<float>();
  float radius = jHitable["radius"].get<float>();
  int materialId = jHitable["material"].get<int>();
  scene.createHitable<Type>(startCenter, finishCenter, startTime, finishTime,
                            radius, scene.getMaterialById(materialId));
}
}  // namespace DescriptionParsing
