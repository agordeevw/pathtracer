#include <glm/vec3.hpp>

#include "DescriptionParsing/Parsers/SceneElementParser.h"
#include "PathTracing/Materials/DiffuseLight.h"
#include "PathTracing/Scene.h"

namespace DescriptionParsing {
namespace Parsers {
using Type = PathTracing::Materials::DiffuseLight;

const char* SceneElementParser<Type>::getTypeString() {
  return "diffuse light";
}

void SceneElementParser<Type>::parse(const Json& jMaterial) {
  int emissionTextureId = jMaterial["emission"].get<int>();
  scene.createMaterial<Type>(scene.getTextureById(emissionTextureId));
}
}  // namespace Parsers
}  // namespace DescriptionParsing
