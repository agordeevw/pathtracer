#include <string>

#include "DescriptionParsing/SceneParsing/SceneElementJsonParser.h"
#include "PathTracing/Scene.h"
#include "PathTracing/Textures/ImageTexture.h"

namespace DescriptionParsing {
using Type = PathTracing::Textures::ImageTexture;

const char* SceneElementJsonParser<Type>::name() { return "image"; }

void SceneElementJsonParser<Type>::parse(const Json& jTexture,
                                         PathTracing::Scene& scene) {
  std::string path = jTexture["path"].get<std::string>();
  scene.createTexture<Type>(path);
}
}  // namespace DescriptionParsing
