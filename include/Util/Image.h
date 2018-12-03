#pragma once
#include <vector>

#include <glm/glm.hpp>

class Image {
 public:
  Image(int width, int height);

  void setPixel(int x, int y, glm::u8vec3 color);

  void writeToFile(const char* filename);

 private:
  std::vector<glm::u8vec3> data;
  int width;
  int height;
};