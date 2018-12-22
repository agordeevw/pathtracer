#pragma once
#include "DescriptionParsing/JsonParser.h"

namespace PathTracing {
class Scene;
}

namespace DescriptionParsing {
template <class T>
class SceneElementJsonParser : JsonParser<PathTracing::Scene> {
 public:
  static void parse(const Json& jSceneElement, PathTracing::Scene& scene);
  static const char* name();
};
}  // namespace DescriptionParsing