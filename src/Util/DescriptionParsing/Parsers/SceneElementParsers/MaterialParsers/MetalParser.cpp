#include <glm/vec3.hpp>

#include "PathTracing/Materials/Metal.h"
#include "PathTracing/Scene.h"
#include "Util/DescriptionParsing/Parsers/SceneElementParser.h"

namespace Util {
namespace DescriptionParsing {
namespace Parsers {
using Type = PathTracing::Materials::Metal;

const char* SceneElementParser<Type>::getTypeString() { return "metal"; }

void SceneElementParser<Type>::parse(const Json& jMaterial) {
  int albedoTextureId = jMaterial["albedo"].get<int>();
  float fuzziness = jMaterial["fuzziness"].get<float>();
  scene.createMaterial<Type>(scene.getTextureById(albedoTextureId), fuzziness);
}
}  // namespace Parsers
}  // namespace DescriptionParsing
}  // namespace Util