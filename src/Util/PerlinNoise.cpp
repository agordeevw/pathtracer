#include <algorithm>
#include <random>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "Util/PerlinNoise.h"

namespace Util {
PerlinNoise::PerlinNoise(unsigned int seed)
    : values(256), permutationX(256), permutationY(256), permutationZ(256) {
  std::uniform_real_distribution<float> distr;
  std::default_random_engine randomEngine(seed);

  for (int i = 0; i < 256; i++) {
    values[i] = distr(randomEngine);
    permutationX[i] = i;
    permutationY[i] = i;
    permutationZ[i] = i;
  }

  std::shuffle(permutationX.begin(), permutationX.end(), randomEngine);
  std::shuffle(permutationY.begin(), permutationY.end(), randomEngine);
  std::shuffle(permutationZ.begin(), permutationZ.end(), randomEngine);
}

float PerlinNoise::sample(const glm::vec3& p) const {
  glm::vec3 uvw = p - glm::floor(p);
  glm::ivec3 ijk{4.0f * p};
  ijk &= 255;
  return values[permutationX[ijk[0]] ^ permutationY[ijk[1]] ^ permutationZ[ijk[2]]];
}
}  // namespace Util
