#include <type_traits>

#include "DescriptionParsing/Parsers/SceneElementParser.h"
#include "DescriptionParsing/Parsers/SceneParser.h"
#include "PathTracing/Hitables/MovingSphere.h"
#include "PathTracing/Hitables/Sphere.h"
#include "PathTracing/Materials/Dielectric.h"
#include "PathTracing/Materials/DiffuseLight.h"
#include "PathTracing/Materials/Lambertian.h"
#include "PathTracing/Materials/Metal.h"
#include "PathTracing/Textures/CheckerTexture.h"
#include "PathTracing/Textures/ConstantTexture.h"
#include "PathTracing/Textures/ImageTexture.h"
#include "PathTracing/Textures/PerlinNoiseTexture.h"

#define TEXTURE_TYPES                            \
  PathTracing::Textures::ConstantTexture,        \
      PathTracing::Textures::CheckerTexture,     \
      PathTracing::Textures::PerlinNoiseTexture, \
      PathTracing::Textures::ImageTexture
#define MATERIAL_TYPES                                                    \
  PathTracing::Materials::Dielectric, PathTracing::Materials::Lambertian, \
      PathTracing::Materials::Metal, PathTracing::Materials::DiffuseLight
#define HITABLE_TYPES \
  PathTracing::Hitables::Sphere, PathTracing::Hitables::MovingSphere

namespace DescriptionParsing {
namespace Parsers {
namespace detail {
using Json = SceneParser::Json;

template <class BaseType, class T, class... Ts>
struct ParseSceneGroupHelper {
  static_assert(std::is_base_of_v<BaseType, T>,
                "Provided type is not a subtype of base scene element type");

  static void tryParseSceneElement(const Json& jSceneElement,
                                   PathTracing::Scene& scene) {
    if (jSceneElement["type"] == SceneElementParser<T>::getTypeString()) {
      SceneElementParser<T> elemParser(scene);
      elemParser.parse(jSceneElement);
      return;
    }

    ParseSceneGroupHelper<BaseType, Ts...>::tryParseSceneElement(jSceneElement,
                                                                 scene);
  }
};

template <class BaseType, class T>
struct ParseSceneGroupHelper<BaseType, T> {
  static_assert(std::is_base_of_v<BaseType, T>,
                "Provided type is not a subtype of base scene element type");

  static void tryParseSceneElement(const Json& jSceneElement,
                                   PathTracing::Scene& scene) {
    if (jSceneElement["type"] == SceneElementParser<T>::getTypeString()) {
      SceneElementParser<T> elemParser(scene);
      elemParser.parse(jSceneElement);
      return;
    }

    throw std::runtime_error("Unknown type: " +
                             jSceneElement["type"].get<std::string>());
  }
};

template <class BaseType, class... Ts>
void parseSceneGroupOfType(const Json& jSceneGroup, PathTracing::Scene& scene) {
  static_assert(sizeof...(Ts) > 0, "No types specified");

  for (auto& jSceneElement : jSceneGroup)
    ParseSceneGroupHelper<BaseType, Ts...>::tryParseSceneElement(jSceneElement,
                                                                 scene);
}
}  // namespace detail

void SceneParser::parse(const Json& jScene) {
  try {
    detail::parseSceneGroupOfType<PathTracing::Texture, TEXTURE_TYPES>(
        jScene["textures"], scene);
  } catch (const std::exception& e) {
    throw std::runtime_error(
        std::string("Failed to parse textures from JSON file. Details: ") +
        e.what());
  }

  try {
    detail::parseSceneGroupOfType<PathTracing::Material, MATERIAL_TYPES>(
        jScene["materials"], scene);
  } catch (const std::exception& e) {
    throw std::runtime_error(
        std::string("Failed to parse materials from JSON file. Details: ") +
        e.what());
  }

  try {
    detail::parseSceneGroupOfType<PathTracing::Hitable, HITABLE_TYPES>(
        jScene["hitables"], scene);
  } catch (const std::exception& e) {
    throw std::runtime_error(
        std::string("Failed to parse hitables from JSON file. Details: ") +
        e.what());
  }
}

}  // namespace Parsers
}  // namespace DescriptionParsing
