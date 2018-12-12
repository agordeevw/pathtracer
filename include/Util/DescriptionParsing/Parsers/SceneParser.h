#pragma once
#include "PathTracing/Scene.h"
#include "Util/DescriptionParsing/JsonParser.h"

namespace Util {
namespace DescriptionParsing {
namespace Parsers {
class SceneParser : public JsonParser {
 public:
  SceneParser(PathTracing::Scene& scene) : scene(scene) {}
  void parse(const Json& jScene);

 private:
  PathTracing::Scene& scene;
};
}  // namespace Parsers
}  // namespace DescriptionParsing
}  // namespace Util