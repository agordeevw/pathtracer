#include <glm/vec3.hpp>

#include "DescriptionParsing/SceneParsing/SceneElementJsonParser.h"
#include "PathTracing/Scene.h"
#include "PathTracing/Textures/ConstantTexture.h"

namespace DescriptionParsing {
using Type = PathTracing::Textures::ConstantTexture;

const char* SceneElementJsonParser<Type>::name() { return "constant"; }

void SceneElementJsonParser<Type>::parse(const Json& jTexture,
                                         PathTracing::Scene& scene) {
  glm::vec3 color = parseVec3(jTexture["color"]);
  scene.createTexture<Type>(color);
}
}  // namespace DescriptionParsing
