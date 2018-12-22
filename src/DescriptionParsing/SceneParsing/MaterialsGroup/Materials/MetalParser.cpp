#include <glm/vec3.hpp>

#include "DescriptionParsing/SceneElementJsonParser.h"
#include "PathTracing/Materials/Metal.h"
#include "PathTracing/Scene.h"

namespace DescriptionParsing {
using Type = PathTracing::Materials::Metal;

const char* SceneElementJsonParser<Type>::name() { return "metal"; }

void SceneElementJsonParser<Type>::parse(const Json& jMaterial,
                                         PathTracing::Scene& scene) {
  int albedoTextureId = jMaterial["albedo"].get<int>();
  float fuzziness = jMaterial["fuzziness"].get<float>();
  scene.createMaterial<Type>(scene.getTextureById(albedoTextureId), fuzziness);
}
}  // namespace DescriptionParsing
