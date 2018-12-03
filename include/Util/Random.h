#pragma once
#include <random>

#include <glm/vec3.hpp>

namespace Util {
namespace Random {
float randf(float min = 0.0f, float max = 1.0f);
glm::vec3 randInUnitSphere();
glm::vec3 randInUnitDisk();
}  // namespace Random
}  // namespace Util