#include <glm/vec3.hpp>

#include "PathTracing/Materials/Dielectric.h"
#include "PathTracing/Scene.h"
#include "Util/DescriptionParsing/Parsers/SceneElementParser.h"

namespace Util {
namespace DescriptionParsing {
namespace Parsers {
using Type = PathTracing::Materials::Dielectric;

const char* SceneElementParser<Type>::getTypeString() {
  return "dielectric";
}

void SceneElementParser<Type>::parse(const Json& jMaterial) {
  float reflectiveIdx = jMaterial["reflectiveIdx"].get<float>();
  scene.createMaterial<Type>(reflectiveIdx);
}
}  // namespace Parsers
}  // namespace DescriptionParsing
}  // namespace Util