#pragma once
#include <random>

#include <glm/vec3.hpp>

namespace MyRandom {
float randf(float min = 0.0f, float max = 1.0f);

glm::vec3 randInUnitSphere();

glm::vec3 randInUnitDisk();

}  // namespace MyRandom