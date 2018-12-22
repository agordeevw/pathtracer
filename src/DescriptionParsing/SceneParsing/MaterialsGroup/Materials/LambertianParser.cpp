#include <glm/vec3.hpp>

#include "DescriptionParsing/SceneElementJsonParser.h"
#include "PathTracing/Materials/Lambertian.h"
#include "PathTracing/Scene.h"

namespace DescriptionParsing {
using Type = PathTracing::Materials::Lambertian;

const char* SceneElementJsonParser<Type>::name() { return "lambertian"; }

void SceneElementJsonParser<Type>::parse(const Json& jMaterial,
                                         PathTracing::Scene& scene) {
  int albedoTextureId = jMaterial["albedo"].get<int>();
  scene.createMaterial<Type>(scene.getTextureById(albedoTextureId));
}
}  // namespace DescriptionParsing
