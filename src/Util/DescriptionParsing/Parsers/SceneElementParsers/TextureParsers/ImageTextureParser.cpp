#include <string>

#include "PathTracing/Scene.h"
#include "PathTracing/Textures/ImageTexture.h"
#include "Util/DescriptionParsing/Parsers/SceneElementParser.h"

namespace Util {
namespace DescriptionParsing {
namespace Parsers {
using Type = PathTracing::Textures::ImageTexture;

const char* SceneElementParser<Type>::getTypeString() { return "image"; }

void SceneElementParser<Type>::parse(const Json& jTexture) {
  std::string path = jTexture["path"].get<std::string>();
  scene.createTexture<Type>(path);
}
}  // namespace Parsers
}  // namespace DescriptionParsing
}  // namespace Util