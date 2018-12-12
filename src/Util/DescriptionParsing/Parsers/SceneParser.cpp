#include <type_traits>

#include "Util/DescriptionParsing/Parsers/SceneParser.h"
#include "PathTracing/Hitables/MovingSphere.h"
#include "PathTracing/Hitables/Sphere.h"
#include "PathTracing/Materials/Dielectric.h"
#include "PathTracing/Materials/Lambertian.h"
#include "PathTracing/Materials/Metal.h"
#include "PathTracing/Textures/CheckerTexture.h"
#include "PathTracing/Textures/ConstantTexture.h"
#include "Util/DescriptionParsing/Parsers/SceneElementParser.h"

namespace Util {
namespace DescriptionParsing {
namespace Parsers {
namespace detail {
using Json = SceneParser::Json;

template <class BaseType, class T, class... Ts>
void tryParseSceneElementOfType(const Json& jSceneElement,
                                PathTracing::Scene& scene) {
  static_assert(std::is_base_of_v<BaseType, T>);

  if (jSceneElement["type"] == SceneElementParser<T>::getTypeString()) {
    SceneElementParser<T> elemParser(scene);
    elemParser.parse(jSceneElement);
    return;
  }

  if constexpr (sizeof...(Ts) > 0) {
    tryParseSceneElementOfType<BaseType, Ts...>(jSceneElement, scene);
  } else {
    throw std::runtime_error("Unknown type: " + jSceneElement["type"]);
  }
}

template <class BaseType, class... Ts>
void parseSceneGroupOfType(const Json& jSceneGroup, PathTracing::Scene& scene) {
  static_assert(sizeof...(Ts) > 0);

  for (auto& jSceneElement : jSceneGroup)
    tryParseSceneElementOfType<BaseType, Ts...>(jSceneElement, scene);
}
}  // namespace detail

void SceneParser::parse(const Json& jScene) {
  using namespace PathTracing;

#define TEXTURE_TYPES Textures::ConstantTexture, Textures::CheckerTexture
#define MATERIAL_TYPES \
  Materials::Dielectric, Materials::Lambertian, Materials::Metal
#define HITABLE_TYPES Hitables::Sphere, Hitables::MovingSphere

  try {
    detail::parseSceneGroupOfType<Texture, TEXTURE_TYPES>(jScene["textures"],
                                                          scene);
  } catch (const std::exception& e) {
    throw std::runtime_error(
        std::string("Failed to parse textures from JSON file. Details: ") +
        e.what());
  }

  try {
    detail::parseSceneGroupOfType<Material, MATERIAL_TYPES>(jScene["materials"],
                                                            scene);
  } catch (const std::exception& e) {
    throw std::runtime_error(
        std::string("Failed to parse materials from JSON file. Details: ") +
        e.what());
  }

  try {
    detail::parseSceneGroupOfType<Hitable, HITABLE_TYPES>(jScene["hitables"],
                                                          scene);
  } catch (const std::exception& e) {
    throw std::runtime_error(
        std::string("Failed to parse hitables from JSON file. Details: ") +
        e.what());
  }
}

}  // namespace Parsers
}  // namespace DescriptionParsing
}  // namespace Util