#include <glm/vec3.hpp>

#include "PathTracing/Scene.h"
#include "PathTracing/Textures/PerlinNoiseTexture.h"
#include "Util/DescriptionParsing/Parsers/SceneElementParser.h"

namespace Util {
namespace DescriptionParsing {
namespace Parsers {
using Type = PathTracing::Textures::PerlinNoiseTexture;

const char* SceneElementParser<Type>::getTypeString() { return "perlin noise"; }

void SceneElementParser<Type>::parse(const Json& jTexture) {
  unsigned int seed = jTexture["seed"].get<unsigned int>();
  float scale = jTexture["scale"].get<float>();
  scene.createTexture<Type>(seed, scale);
}
}  // namespace Parsers
}  // namespace DescriptionParsing
}  // namespace Util