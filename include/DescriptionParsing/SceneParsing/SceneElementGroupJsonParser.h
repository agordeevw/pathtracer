#pragma once
#include <type_traits>

#include "DescriptionParsing/JsonParser.h"
#include "DescriptionParsing/SceneParsing/SceneElementJsonParser.h"

namespace PathTracing {
class Scene;
}

namespace DescriptionParsing {
template <class T>
class SceneElementGroupJsonParser : JsonParser<PathTracing::Scene> {
 public:
  static void parse(const Json& jSceneElementGroup, PathTracing::Scene& scene);

 private:
  template <class U, class... Us>
  struct Helper {
    static_assert(std::is_base_of<T, U>::value, "");

    static void tryParseSceneElement(const Json& jSceneElement,
                                     PathTracing::Scene& scene) {
      if (SceneElementJsonParser<U>::name() == jSceneElement["type"]) {
        SceneElementJsonParser<U>::parse(jSceneElement, scene);
      } else {
        Helper<Us...>::tryParseSceneElement(jSceneElement, scene);
      }
    }
  };

  template <class U>
  struct Helper<U> {
    static_assert(std::is_base_of<T, U>::value, "");

    static void tryParseSceneElement(const Json& jSceneElement,
                                     PathTracing::Scene& scene) {
      if (SceneElementJsonParser<U>::name() == jSceneElement["type"]) {
        SceneElementJsonParser<U>::parse(jSceneElement, scene);
      } else {
        throw std::runtime_error(
            "Failed to parse scene element, unknown type: " +
            jSceneElement["type"]);
      }
    }
  };
};
}  // namespace DescriptionParsing