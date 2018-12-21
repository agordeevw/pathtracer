#include <glm/vec3.hpp>

#include "DescriptionParsing/Parsers/SceneElementParser.h"
#include "PathTracing/Scene.h"
#include "PathTracing/Textures/CheckerTexture.h"

namespace DescriptionParsing {
namespace Parsers {
using Type = PathTracing::Textures::CheckerTexture;

const char* SceneElementParser<Type>::getTypeString() { return "checker"; }

void SceneElementParser<Type>::parse(const Json& jTexture) {
  int evenTextureId = jTexture["even"].get<int>();
  int oddTextureId = jTexture["odd"].get<int>();
  // Major problem here: loader can only reference loaded textures
  // Possible error in description file - reference loops
  // How to prevent?
  scene.createTexture<Type>(scene.getTextureById(evenTextureId),
                            scene.getTextureById(oddTextureId));
}
}  // namespace Parsers
}  // namespace DescriptionParsing
