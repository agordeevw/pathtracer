#include "DescriptionParsing/SceneParsing/SceneElementGroupJsonParser.h"

namespace PathTracing {
class Texture;
class Material;
class Hitable;
}  // namespace PathTracing

namespace DescriptionParsing {
using Type = PathTracing::Scene;

void JsonParser<Type>::parse(const Json& jScene, Type& scene) {
  SceneElementGroupJsonParser<PathTracing::Texture> textureParser;
  textureParser.parse(jScene["textures"], scene);

  SceneElementGroupJsonParser<PathTracing::Material> materialParser;
  materialParser.parse(jScene["materials"], scene);

  SceneElementGroupJsonParser<PathTracing::Hitable> hitableParser;
  hitableParser.parse(jScene["hitables"], scene);
}
}  // namespace DescriptionParsing
