#pragma once
#include "PathTracing/Scene.h"
#include "Util/DescriptionParsing/JsonParser.h"

namespace Util {
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
}  // namespace Util