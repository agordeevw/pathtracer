#pragma once
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace Util {
class Image {
 public:
  Image() = default;
  Image(int width, int height);
  Image(const std::string& path);

  void setPixel(int x, int y, glm::u8vec3 color);

  bool writeToFile(const char* filename);

  int getWidth() const { return width; }
  int getHeight() const { return height; }
  glm::u8vec3 getPixel(int x, int y) const;

 private:
  std::vector<glm::u8vec3> data;
  int width;
  int height;
};
}  // namespace Util