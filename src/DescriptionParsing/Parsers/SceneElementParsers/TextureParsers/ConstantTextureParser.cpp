#include <glm/vec3.hpp>

#include "DescriptionParsing/Parsers/SceneElementParser.h"
#include "PathTracing/Scene.h"
#include "PathTracing/Textures/ConstantTexture.h"

namespace DescriptionParsing {
namespace Parsers {
using Type = PathTracing::Textures::ConstantTexture;

const char* SceneElementParser<Type>::getTypeString() { return "constant"; }

void SceneElementParser<Type>::parse(const Json& jTexture) {
  glm::vec3 color = parseVec3(jTexture["color"]);
  scene.createTexture<Type>(color);
}
}  // namespace Parsers
}  // namespace DescriptionParsing