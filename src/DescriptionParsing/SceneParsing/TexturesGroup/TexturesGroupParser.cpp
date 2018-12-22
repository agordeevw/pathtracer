#include "DescriptionParsing/SceneParsing/SceneElementGroupJsonParser.h"
#include "PathTracing/Texture.h"
#include "PathTracing/Textures/CheckerTexture.h"
#include "PathTracing/Textures/ConstantTexture.h"
#include "PathTracing/Textures/ImageTexture.h"
#include "PathTracing/Textures/PerlinNoiseTexture.h"

namespace DescriptionParsing {
using namespace PathTracing;

void SceneElementGroupJsonParser<PathTracing::Texture>::parse(
    const Json& jSceneElementGroup, PathTracing::Scene& scene) {
  for (auto& jSceneElement : jSceneElementGroup) {
    Helper<Textures::CheckerTexture, Textures::ConstantTexture,
           Textures::ImageTexture,
           Textures::PerlinNoiseTexture>::tryParseSceneElement(jSceneElement,
                                                               scene);
  }
}
}  // namespace DescriptionParsing