#include <glm/vec3.hpp>

#include "DescriptionParsing/Parsers/SceneElementParser.h"
#include "PathTracing/Materials/Dielectric.h"
#include "PathTracing/Scene.h"

namespace DescriptionParsing {
namespace Parsers {
using Type = PathTracing::Materials::Dielectric;

const char* SceneElementParser<Type>::getTypeString() { return "dielectric"; }

void SceneElementParser<Type>::parse(const Json& jMaterial) {
  float reflectiveIdx = jMaterial["reflectiveIdx"].get<float>();
  scene.createMaterial<Type>(reflectiveIdx);
}
}  // namespace Parsers
}  // namespace DescriptionParsing
