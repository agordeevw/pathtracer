#pragma once
#include "DescriptionParsing/JsonParser.h"
#include "PathTracing/Scene.h"

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
