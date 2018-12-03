#include <thread>
#include <vector>

#include <glm/glm.hpp>

#include "Camera.h"
#include "Hitable.h"
#include "Util/Image.h"
#include "Material.h"
#include "PathTracing.h"
#include "Ray.h"
#include "Scene.h"
#include "Util/MyRandom.h"

namespace {
glm::vec3 noHitColor(const Ray& r) {
  float param = (glm::normalize(r.direction).y + 1.0f) * 0.5f;
  return glm::mix(glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec3{0.1f, 0.3f, 0.5f},
                  param);
}

glm::vec3 color(const Ray& r, const Hitable& hitable, int depth) {
  HitRecord rec;
  if (hitable.hit(r, 0.001f, FLT_MAX, rec)) {
    Ray scattered;
    glm::vec3 attenuation;
    if (depth < 50 && rec.material->scatter(r, rec, attenuation, scattered)) {
      return attenuation * color(scattered, hitable, depth + 1);
    } else {
      return {};
    }
  } else {
    return noHitColor(r);
  }
}
}  // namespace

Image PathTracing::traceScene(const Scene& scene, const Camera& camera,
                              int imageWidth, int imageHeight,
                              int samplesPerPixel) {
  using MyRandom::randf;

  Image image(imageWidth, imageHeight);

  auto renderTile = [&](int startX, int startY, int sizeX, int sizeY) {
    for (int y = startY; y < std::min(startY + sizeY, imageHeight); y++) {
      for (int x = startX; x < std::min(startX + sizeX, imageWidth); x++) {
        glm::vec3 col{};
        for (int sample = 0; sample < samplesPerPixel; sample++) {
          float u = (float(x) + randf() - 0.5f) / float(imageWidth);
          float v = (float(y) + randf() - 0.5f) / float(imageHeight);

          col += color(camera.getRay(u, v), scene.getWorld(), 0) /
                 float(samplesPerPixel);
        }
        // gamma-correction
        col = glm::sqrt(col);

        image.setPixel(x, y, 255.99f * col);
      }
    }
  };

  const int groupSizeX = 32, groupSizeY = 32;

  auto render = [&](int group) {
    for (int groupY = 0; groupY < imageHeight / groupSizeY + 1; groupY++) {
      for (int groupX = 0; groupX < imageWidth / groupSizeX + 1; groupX++) {
        if ((groupX + groupY * imageHeight / groupSizeY) % 4 == group) {
          renderTile(groupX * groupSizeX, groupY * groupSizeY, groupSizeX,
                     groupSizeY);
        }
      }
    }
  };

  constexpr int threadsCount = 2;

  std::thread renderThreads[threadsCount];
  for (int i = 0; i < threadsCount; i++) {
    renderThreads[i] = std::thread(render, i);
  }
  for (int i = 0; i < threadsCount; i++) {
    renderThreads[i].join();
  }

  return image;
}
