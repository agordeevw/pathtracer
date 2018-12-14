#include <glm/vec3.hpp>

#include "PathTracing/Materials/DiffuseLight.h"
#include "PathTracing/Scene.h"
#include "Util/DescriptionParsing/Parsers/SceneElementParser.h"

namespace Util {
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
}  // namespace Util