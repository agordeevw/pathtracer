#include <glm/vec3.hpp>

#include "DescriptionParsing/SceneParsing/SceneElementJsonParser.h"
#include "PathTracing/Scene.h"
#include "PathTracing/Textures/CheckerTexture.h"

namespace DescriptionParsing {
using Type = PathTracing::Textures::CheckerTexture;

const char* SceneElementJsonParser<Type>::name() { return "checker"; }

void SceneElementJsonParser<Type>::parse(const Json& jTexture,
                                         PathTracing::Scene& scene) {
  int evenTextureId = jTexture["even"].get<int>();
  int oddTextureId = jTexture["odd"].get<int>();
  // Major problem here: loader can only reference loaded textures
  // Possible error in description file - reference loops
  // How to prevent?
  scene.createTexture<Type>(scene.getTextureById(evenTextureId),
                            scene.getTextureById(oddTextureId));
}
}  // namespace DescriptionParsing
