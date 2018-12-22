#include <glm/vec3.hpp>

#include "DescriptionParsing/SceneParsing/SceneElementJsonParser.h"
#include "PathTracing/Hitables/Sphere.h"
#include "PathTracing/Scene.h"

namespace DescriptionParsing {
using Type = PathTracing::Hitables::Sphere;

const char* SceneElementJsonParser<Type>::name() { return "sphere"; }

void SceneElementJsonParser<Type>::parse(const Json& jHitable,
                                         PathTracing::Scene& scene) {
  glm::vec3 center = parseVec3(jHitable["center"]);
  float radius = jHitable["radius"].get<float>();
  int materialId = jHitable["material"].get<int>();
  scene.createHitable<Type>(center, radius, scene.getMaterialById(materialId));
}
}  // namespace DescriptionParsing
