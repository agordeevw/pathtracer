#pragma once
#include <vector>

#include <glm/vec3.hpp>

#include "Util/Random.h"

namespace Util {
class PerlinNoise {
 public:
  PerlinNoise(unsigned int seed);
  float sample(const glm::vec3& p) const;

private:
  std::vector<glm::vec3> values;
  std::vector<int> permutationX;
  std::vector<int> permutationY;
  std::vector<int> permutationZ;
};
}  // namespace Util
