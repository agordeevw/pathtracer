#include <glm/vec3.hpp>

#include "DescriptionParsing/SceneElementJsonParser.h"
#include "PathTracing/Scene.h"
#include "PathTracing/Textures/PerlinNoiseTexture.h"

namespace DescriptionParsing {
using Type = PathTracing::Textures::PerlinNoiseTexture;

const char* SceneElementJsonParser<Type>::name() { return "perlin noise"; }

void SceneElementJsonParser<Type>::parse(const Json& jTexture,
                                         PathTracing::Scene& scene) {
  unsigned int seed = jTexture["seed"].get<unsigned int>();
  float scale = jTexture["scale"].get<float>();
  scene.createTexture<Type>(seed, scale);
}
}  // namespace DescriptionParsing
