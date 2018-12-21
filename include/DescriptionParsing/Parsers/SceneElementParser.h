#pragma once
#include "DescriptionParsing/JsonParser.h"
#include "PathTracing/Scene.h"

namespace DescriptionParsing {
namespace Parsers {
template <class T>
class SceneElementParser : public JsonParser {
 public:
  SceneElementParser(PathTracing::Scene& scene) : scene(scene) {}
  void parse(const Json& jSceneElement);
  static const char* getTypeString();

 private:
  PathTracing::Scene& scene;
};
}  // namespace Parsers
}  // namespace DescriptionParsing
