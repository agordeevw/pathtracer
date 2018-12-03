#pragma once
#include <random>

#include <glm/vec3.hpp>

namespace MyRandom {
// Returns random float in interval [0.0, 1.0)
float randf(float min = 0.0f, float max = 1.0f);

// Returns random point in unit 3D-sphere centered at (0, 0, 0)
glm::vec3 randInUnitSphere();

// Returns random point in unit disk centered at (0, 0, 0)
// and lying within plane z=0
glm::vec3 randInUnitDisk();

}  // namespace MyRandom