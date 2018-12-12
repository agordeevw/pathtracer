#include <stdexcept>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Util/Image.h"

namespace Util {
Image::Image(int width, int height)
    : data(sizeof(glm::u8vec3) * width * height),
      width(width),
      height(height) {}

Image::Image(const std::string& path) {
  int x, y, channels;
  unsigned char* texData = stbi_load(path.c_str(), &x, &y, &channels, 0);
  if (!texData)
    throw std::runtime_error("Failed to load image " + path);
  if (channels != 3)
    throw std::runtime_error("Expected exactly 3 channels in the image " +
                             path);
  width = x;
  height = y;
  data.resize(x * y * channels);
  memcpy(data.data(), texData, x * y * channels);
  free(texData);
}

void Image::setPixel(int x, int y, glm::u8vec3 color) {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    data[x + y * width] = color;
  }
}

bool Image::writeToFile(const char* filename) {
  if (width > 0 && height > 0 && data.size() == width * height * 3) {
    stbi_flip_vertically_on_write(1);
    return stbi_write_bmp(filename, width, height, 3, data.data());
  } else {
    return false;
  }
}
glm::u8vec3 Image::getPixel(int x, int y) const {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    return data[x + y * width];
  } else {
    return {};
  }
}
}  // namespace Util