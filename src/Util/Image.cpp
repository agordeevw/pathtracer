#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "Image.h"

Image::Image(int width, int height)
    : data(sizeof(glm::u8vec3) * width * height) {}

void Image::setPixel(int x, int y, glm::u8vec3 color) {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    data[x + y * width] = color;
  }
}

void Image::writeToFile(const char* filename) {
  stbi_flip_vertically_on_write(1);
  stbi_write_bmp(filename, width, height, 3, data.data());
}
