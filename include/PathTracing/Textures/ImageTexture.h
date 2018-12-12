#pragma once
#include "PathTracing/Texture.h"
#include "Util/Image.h"

namespace PathTracing {
namespace Textures {
class ImageTexture : public Texture {
 public:
  ImageTexture(const std::string& filepath) : image(filepath) {}
  ~ImageTexture() override = default;

  glm::vec3 sample(float u, float v, const glm::vec3&) const override {
    int i = int(u * image.getWidth());
    int j = int((1 - v) * image.getHeight() - 0.001f);
    if (i < 0) i = 0;
    if (j < 0) j = 0;
    if (i > image.getWidth() - 1) i = image.getWidth() - 1;
    if (j > image.getHeight() - 1) j = image.getHeight() - 1;
    return glm::vec3{image.getPixel(i, j)} / 255.0f;
  }

 private:
  Util::Image image;
};
}  // namespace Textures
}  // namespace PathTracing