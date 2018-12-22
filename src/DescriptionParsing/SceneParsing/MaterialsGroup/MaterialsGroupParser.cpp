#include "DescriptionParsing/SceneParsing/SceneElementGroupJsonParser.h"
#include "PathTracing/Material.h"
#include "PathTracing/Materials/Dielectric.h"
#include "PathTracing/Materials/DiffuseLight.h"
#include "PathTracing/Materials/Lambertian.h"
#include "PathTracing/Materials/Metal.h"

namespace DescriptionParsing {
void SceneElementGroupJsonParser<PathTracing::Material>::parse(
    const Json& jSceneElementGroup, PathTracing::Scene& scene) {
  using namespace PathTracing;

  for (auto& jSceneElement : jSceneElementGroup) {
    Helper<Materials::Dielectric, Materials::DiffuseLight,
           Materials::Lambertian,
           Materials::Metal>::tryParseSceneElement(jSceneElement, scene);
  }
}
}  // namespace DescriptionParsing