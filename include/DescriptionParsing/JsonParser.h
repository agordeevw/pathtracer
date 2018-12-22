#pragma once
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>

namespace DescriptionParsing {
template <class T>
class JsonParser {
 public:
  using Json = nlohmann::json;

  static void parse(const Json& j, T& instance);

 protected:
  static inline glm::vec3 parseVec3(const Json& jVec3) {
    glm::vec3 ret;
    for (int i = 0; i < 3; i++) ret[i] = jVec3[i].get<float>();
    return ret;
  }
};
}  // namespace DescriptionParsing
