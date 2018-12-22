#include <glm/vec3.hpp>

#include "DescriptionParsing/SceneElementJsonParser.h"
#include "PathTracing/Materials/Dielectric.h"
#include "PathTracing/Scene.h"

namespace DescriptionParsing {
using Type = PathTracing::Materials::Dielectric;

const char* SceneElementJsonParser<Type>::name() { return "dielectric"; }

void SceneElementJsonParser<Type>::parse(const Json& jMaterial,
                                         PathTracing::Scene& scene) {
  float reflectiveIdx = jMaterial["reflectiveIdx"].get<float>();
  scene.createMaterial<Type>(reflectiveIdx);
}
}  // namespace DescriptionParsing
