#include "Util/Random.h"
#include <glm/geometric.hpp>

namespace Util {
namespace Random {
std::default_random_engine g_randomEngine;

float randf(float min, float max) {
  std::uniform_real_distribution<float> distribution(min, max);
  return distribution(g_randomEngine);
}

glm::vec3 randInUnitSphere() {
  glm::vec3 p;
  do {
    p = 2.0f * glm::vec3{randf(), randf(), randf()} - 1.0f;
  } while (glm::dot(p, p) >= 1.0f);
  return p;
}

glm::vec3 randInUnitDisk() {
  glm::vec3 p;
  do {
    p = 2.0f * glm::vec3{randf(), randf(), 0.0f} - glm::vec3{1.0f, 1.0f, 0.0f};
  } while (glm::dot(p, p) >= 1.0f);
  return p;
}
}  // namespace Random
}  // namespace Util