#include <algorithm>
#include <random>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "Util/PerlinNoise.h"

namespace Util {
PerlinNoise::PerlinNoise(unsigned int seed)
    : values(256), permutationX(256), permutationY(256), permutationZ(256) {
  std::uniform_real_distribution<float> distr{-1.0f, 1.0f};
  std::default_random_engine randomEngine(seed);

  for (int i = 0; i < 256; i++) {
    values[i] = {distr(randomEngine), distr(randomEngine), distr(randomEngine)};
    permutationX[i] = i;
    permutationY[i] = i;
    permutationZ[i] = i;
  }

  std::shuffle(permutationX.begin(), permutationX.end(), randomEngine);
  std::shuffle(permutationY.begin(), permutationY.end(), randomEngine);
  std::shuffle(permutationZ.begin(), permutationZ.end(), randomEngine);
}

inline float perlinInterpolation(glm::vec3 c[2][2][2], const glm::vec3& uvw) {
  glm::vec3 smoothedUvw = uvw * uvw * (3.0f - 2.0f * uvw);
  float accum = 0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        glm::vec3 weights{uvw[0] - i, uvw[1] - j, uvw[2] - k};
        glm::vec3 tmp =
            glm::mix(1.0f - smoothedUvw, smoothedUvw, glm::vec3{i, j, k});
        accum += tmp.x * tmp.y * tmp.z * glm::dot(c[i][j][k], weights);
      }
    }
  }
  return accum;
}

float PerlinNoise::sample(const glm::vec3& p) const {
  glm::ivec3 ijk{glm::floor(p)};
  glm::vec3 c[2][2][2];
  for (int di = 0; di < 2; di++) {
    for (int dj = 0; dj < 2; dj++) {
      for (int dk = 0; dk < 2; dk++) {
        c[di][dj][dk] = values[permutationX[(ijk[0] + di) & 255] ^
                               permutationY[(ijk[1] + dj) & 255] ^
                               permutationZ[(ijk[2] + dk) & 255]];
      }
    }
  }
  glm::vec3 uvw = p - glm::floor(p);
  return perlinInterpolation(c, uvw);
}
}  // namespace Util
