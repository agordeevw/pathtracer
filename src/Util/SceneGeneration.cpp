#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "PathTracing/Hitables/MovingSphere.h"
#include "PathTracing/Hitables/Sphere.h"
#include "PathTracing/Materials/Dielectric.h"
#include "PathTracing/Materials/Lambertian.h"
#include "PathTracing/Materials/Metal.h"
#include "PathTracing/Scene.h"
#include "Util/Random.h"
#include "Util/SceneGeneration.h"

namespace Util {
namespace SceneGeneration {
namespace {
PathTracing::Scene debugScene() {
  using namespace PathTracing;

  Scene scene;

  const auto& red =
      scene.createMaterial<Materials::Lambertian>(glm::vec3{1.0f, 0.0f, 0.0f});
  const auto& blue =
      scene.createMaterial<Materials::Lambertian>(glm::vec3{0.0f, 0.0f, 1.0f});
  const auto& metal =
      scene.createMaterial<Materials::Metal>(glm::vec3{0.7f, 0.6f, 0.5f}, 0.1f);

  scene.createHitable<Hitables::Sphere>(glm::vec3{-2.0f, 0.0f, 0.0f}, 2.0f,
                                        metal);
  scene.createHitable<Hitables::Sphere>(glm::vec3{4.0f, 0.0f, 0.0f}, 1.0f,
                                        blue);
  scene.createHitable<Hitables::Sphere>(glm::vec3{2.0f, -0.5f, 0.0f}, 0.5f,
                                        red);

  return scene;
}

PathTracing::Scene randomSpheres() {
  using Util::Random::randf;
  using namespace PathTracing;

  Scene scene;

  scene.createHitable<Hitables::Sphere>(
      glm::vec3{0.0f, -1000.0f, 0.0f}, 1000.0f,
      scene.createMaterial<Materials::Lambertian>(glm::vec3{0.5f, 0.5f, 0.5f}));

  const Material& glass = scene.createMaterial<Materials::Dielectric>(1.5f);

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float chooseMat = randf();
      glm::vec3 center(a + 0.9f * randf(), 0.2f, b + 0.9f * randf());
      if (glm::length(center - glm::vec3{4.0f, 0.2f, 0.0f}) > 0.9f) {
        if (chooseMat < 0.8f) {  // diffuse
          const Material& lambertian =
              scene.createMaterial<Materials::Lambertian>(glm::vec3{
                  randf() * randf(), randf() * randf(), randf() * randf()});
          scene.createHitable<Hitables::Sphere>(center, 0.2f, lambertian);
        } else if (chooseMat < 0.95f) {  // metal
          const Material& metal = scene.createMaterial<Materials::Metal>(
              0.5f * (glm::vec3{randf(), randf(), randf()} + 1.0f),
              0.5f * randf());
          scene.createHitable<Hitables::Sphere>(center, 0.2f, metal);
        } else {  // glass
          scene.createHitable<Hitables::Sphere>(center, 0.2f, glass);
        }
      }
    }

    scene.createHitable<Hitables::Sphere>(glm::vec3{0.0f, 1.0f, 0.0f}, 1.0f,
                                          glass);

    scene.createHitable<Hitables::Sphere>(
        glm::vec3{-4.0f, 1.0f, 0.0f}, 1.0f,
        scene.createMaterial<Materials::Lambertian>(
            glm::vec3{0.4f, 0.2f, 0.1f}));

    scene.createHitable<Hitables::Sphere>(
        glm::vec3{4.0f, 1.0f, 0.0f}, 1.0f,
        scene.createMaterial<Materials::Metal>(glm::vec3{0.7f, 0.6f, 0.5f},
                                               0.0f));
  }

  return scene;
}

PathTracing::Scene randomMovingSpheres() {
  using Util::Random::randf;
  using namespace PathTracing;

  Scene scene;

  scene.createHitable<Hitables::Sphere>(
      glm::vec3{0.0f, -1000.0f, 0.0f}, 1000.0f,
      scene.createMaterial<Materials::Lambertian>(glm::vec3{0.5f, 0.5f, 0.5f}));

  const Material& glass = scene.createMaterial<Materials::Dielectric>(1.5f);

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float chooseMat = randf();
      glm::vec3 center(a + 0.9f * randf(), 0.2f, b + 0.9f * randf());
      if (glm::length(center - glm::vec3{4.0f, 0.2f, 0.0f}) > 0.9f) {
        if (chooseMat < 0.8f) {  // diffuse
          const Material& lambertian =
              scene.createMaterial<Materials::Lambertian>(glm::vec3{
                  randf() * randf(), randf() * randf(), randf() * randf()});
          scene.createHitable<Hitables::MovingSphere>(
              center, center + glm::vec3{0.0f, 0.5f * randf(), 0.0f}, 0.0f,
              1.0f, 0.2f, lambertian);
        } else if (chooseMat < 0.95f) {  // metal
          const Material& metal = scene.createMaterial<Materials::Metal>(
              0.5f * (glm::vec3{randf(), randf(), randf()} + 1.0f),
              0.5f * randf());
          scene.createHitable<Hitables::Sphere>(center, 0.2f, metal);
        } else {  // glass
          scene.createHitable<Hitables::Sphere>(center, 0.2f, glass);
        }
      }
    }

    scene.createHitable<Hitables::Sphere>(glm::vec3{0.0f, 1.0f, 0.0f}, 1.0f,
                                          glass);

    scene.createHitable<Hitables::Sphere>(
        glm::vec3{-4.0f, 1.0f, 0.0f}, 1.0f,
        scene.createMaterial<Materials::Lambertian>(
            glm::vec3{0.4f, 0.2f, 0.1f}));

    scene.createHitable<Hitables::Sphere>(
        glm::vec3{4.0f, 1.0f, 0.0f}, 1.0f,
        scene.createMaterial<Materials::Metal>(glm::vec3{0.7f, 0.6f, 0.5f},
                                               0.0f));
  }

  return scene;
}
}  // namespace

PathTracing::Scene generateScene(const std::string& name) {
  if (name == "debugScene") {
    return debugScene();
  } else if (name == "randomSpheres") {
    return randomSpheres();
  } else if (name == "randomMovingSpheres") {
    return randomMovingSpheres();
  } else {
    throw std::logic_error("Unknown scene name");
  }
}

}  // namespace SceneGeneration
}  // namespace Util