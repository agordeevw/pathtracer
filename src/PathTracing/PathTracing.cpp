#include <thread>
#include <vector>

#include <glm/glm.hpp>

#include "PathTracing/Camera.h"
#include "PathTracing/Hitable.h"
#include "PathTracing/Material.h"
#include "PathTracing/PathTracing.h"
#include "PathTracing/Ray.h"
#include "PathTracing/Scene.h"
#include "Util/Image.h"
#include "Util/Random.h"

namespace PathTracing {

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

Util::Image traceScene(const Scene& scene, const Camera& camera,
                       const TracingParameters& params) {
  using Util::Random::randf;

  scene.buildBVH(camera.getShutterOpenTime(), camera.getShutterCloseTime());

  Util::Image image(params.imageWidth, params.imageHeight);

  auto renderTile = [&scene, &camera, &image, &params](int startX, int startY,
                                                       int sizeX, int sizeY) {
    for (int y = startY; y < std::min(startY + sizeY, params.imageHeight);
         y++) {
      for (int x = startX; x < std::min(startX + sizeX, params.imageWidth);
           x++) {
        glm::vec3 col{};
        for (int sample = 0; sample < params.samplesPerPixel; sample++) {
          float u = (float(x) + randf() - 0.5f) / float(params.imageWidth);
          float v = (float(y) + randf() - 0.5f) / float(params.imageHeight);

          col += color(camera.getRay(u, v), scene.getWorld(), 0) /
                 float(params.samplesPerPixel);
        }
        // gamma-correction
        col = glm::sqrt(col);

        image.setPixel(x, y, 255.99f * col);
      }
    }
  };

  const int groupSizeX = 32, groupSizeY = 32;

  auto renderThreadTask = [&params, &renderTile, groupSizeX,
                           groupSizeY](int threadId) {
    for (int gY = 0; gY < params.imageHeight / groupSizeY + 1; gY++) {
      for (int gX = 0; gX < params.imageWidth / groupSizeX + 1; gX++) {
        int gId = gX + gY * params.imageHeight / groupSizeY;
        if (gId % params.threadsCount == threadId) {
          renderTile(gX * groupSizeX, gY * groupSizeY, groupSizeX, groupSizeY);
        }
      }
    }
  };

  if (params.threadsCount > 1) {
    std::vector<std::thread> renderThreads(params.threadsCount);
    for (int i = 0; i < params.threadsCount; i++) {
      renderThreads[i] = std::thread(renderThreadTask, i);
    }
    for (int i = 0; i < params.threadsCount; i++) {
      renderThreads[i].join();
    }
  } else {
    renderTile(0, 0, params.imageWidth, params.imageHeight);
  }

  return image;
}
}  // namespace PathTracing