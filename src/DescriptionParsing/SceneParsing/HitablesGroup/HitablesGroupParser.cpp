#include "DescriptionParsing/SceneParsing/SceneElementGroupJsonParser.h"
#include "PathTracing/Hitable.h"
#include "PathTracing/Hitables/MovingSphere.h"
#include "PathTracing/Hitables/Sphere.h"

namespace DescriptionParsing {
void SceneElementGroupJsonParser<PathTracing::Hitable>::parse(
    const Json& jSceneElementGroup, PathTracing::Scene& scene) {
  using namespace PathTracing;

  for (auto& jSceneElement : jSceneElementGroup) {
    Helper<Hitables::MovingSphere, Hitables::Sphere>::tryParseSceneElement(
        jSceneElement, scene);
  }
}
}  // namespace DescriptionParsing