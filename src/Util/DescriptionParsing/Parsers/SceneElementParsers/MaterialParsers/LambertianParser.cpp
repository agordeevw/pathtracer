#include <glm/vec3.hpp>

#include "PathTracing/Materials/Lambertian.h"
#include "PathTracing/Scene.h"
#include "Util/DescriptionParsing/Parsers/SceneElementParser.h"

namespace Util {
namespace DescriptionParsing {
namespace Parsers {
using Type = PathTracing::Materials::Lambertian;

const char* SceneElementParser<Type>::getTypeString() {
  return "lambertian";
}

void SceneElementParser<Type>::parse(const Json& jMaterial) {
  int albedoTextureId = jMaterial["albedo"].get<int>();
  scene.createMaterial<Type>(scene.getTextureById(albedoTextureId));
}
}  // namespace Parsers
}  // namespace DescriptionParsing
}  // namespace Util