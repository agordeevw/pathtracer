#include <glm/vec3.hpp>

#include "DescriptionParsing/SceneElementJsonParser.h"
#include "PathTracing/Materials/DiffuseLight.h"
#include "PathTracing/Scene.h"

namespace DescriptionParsing {
using Type = PathTracing::Materials::DiffuseLight;

const char* SceneElementJsonParser<Type>::name() { return "diffuse light"; }

void SceneElementJsonParser<Type>::parse(const Json& jMaterial,
                                         PathTracing::Scene& scene) {
  int emissionTextureId = jMaterial["emission"].get<int>();
  scene.createMaterial<Type>(scene.getTextureById(emissionTextureId));
}
}  // namespace DescriptionParsing
