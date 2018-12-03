#include <cassert>
#define _USE_MATH_DEFINES
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include <glm/glm.hpp>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "MyRandom.h"

float schlick(float cosine, float reflectiveIdx) {
  float r0 = (1 - reflectiveIdx) / (1 + reflectiveIdx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * powf(1 - cosine, 5);
}

struct Ray {
  glm::vec3 pointAtT(float t) const { return origin + t * direction; }

  glm::vec3 origin;
  glm::vec3 direction;
};

class Material;

struct HitRecord {
  float t;
  glm::vec3 point;
  glm::vec3 normal;
  const Material* material;
};

class Hitable {
 public:
  virtual bool hit(const Ray& r, float tMin, float tMax,
                   HitRecord& rec) const = 0;
};

class HitableList : public Hitable {
 public:
  void append(const Hitable& hitable) { hitables.push_back(&hitable); }

  bool hit(const Ray& r, float tMin, float tMax,
           HitRecord& rec) const override {
    HitRecord tempRec;
    bool hitAnything = false;
    float closestHit = tMax;

    for (const auto& hitable : hitables) {
      if (hitable->hit(r, tMin, closestHit, tempRec)) {
        hitAnything = true;
        closestHit = tempRec.t;
        rec = tempRec;
      }
    }
    return hitAnything;
  }

 private:
  std::vector<const Hitable*> hitables;
};

class Sphere : public Hitable {
 public:
  Sphere() = default;
  Sphere(const glm::vec3& center, float radius, const Material& material)
      : center(center), radius(radius), material(&material) {}

  bool hit(const Ray& r, float tMin, float tMax,
           HitRecord& rec) const override {
    glm::vec3 oc = r.origin - center;
    float a = glm::dot(r.direction, r.direction);
    float b = glm::dot(oc, r.direction);
    float c = glm::dot(oc, oc) - radius * radius;
    float discr = b * b - a * c;
    if (discr > 0) {
      float temp = (-b - sqrtf(discr)) / a;
      if (temp < tMax && temp > tMin) {
        rec.t = temp;
        rec.point = r.pointAtT(rec.t);
        rec.normal = (rec.point - center) / radius;
        rec.material = material;
        return true;
      }
      temp = (-b + sqrtf(discr)) / a;
      if (temp < tMax && temp > tMin) {
        rec.t = temp;
        rec.point = r.pointAtT(rec.t);
        rec.normal = (rec.point - center) / radius;
        rec.material = material;
        return true;
      }
    }
    return false;
  }

  glm::vec3 center;
  float radius;
  const Material* material;
};

class Material {
 public:
  virtual bool scatter(const Ray& rayIn, const HitRecord& rec,
                       glm::vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
 public:
  Lambertian(const glm::vec3& a) : albedo(a) {}

  bool scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation,
               Ray& scattered) const override {
    using MyRandom::randInUnitSphere;

    glm::vec3 target = rec.point + rec.normal + randInUnitSphere();
    scattered = Ray{rec.point, target - rec.point};
    attenuation = albedo;
    return true;
  }

  glm::vec3 albedo;
};

class Metal : public Material {
 public:
  Metal(const glm::vec3& a, float f) : albedo(a) {
    if (f < 1.0f)
      fuzz = f;
    else
      fuzz = 1.0f;
  }

  bool scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation,
               Ray& scattered) const override {
    using MyRandom::randInUnitSphere;

    glm::vec3 reflected =
        glm::reflect(glm::normalize(rayIn.direction), rec.normal);
    scattered = Ray{rec.point, reflected + fuzz * randInUnitSphere()};
    attenuation = albedo;
    return glm::dot(scattered.direction, rec.normal) > 0.0f;
  }

  glm::vec3 albedo;
  float fuzz;
};

class Dielectric : public Material {
 public:
  Dielectric(float ri) : reflectiveIdx{ri} {}

  bool scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation,
               Ray& scattered) const override {
    using MyRandom::randf;

    attenuation = glm::vec3{1.0f, 1.0f, 1.0f};
    glm::vec3 outwardNormal;
    float niOverNt;
    float cosine =
        glm::dot(rayIn.direction, rec.normal) / glm::length(rayIn.direction);
    if (glm::dot(rayIn.direction, rec.normal) > 0.0f) {
      outwardNormal = -rec.normal;
      niOverNt = reflectiveIdx;
      cosine *= reflectiveIdx;
    } else {
      outwardNormal = rec.normal;
      niOverNt = 1.0f / reflectiveIdx;
      cosine *= -1;
    }

    glm::vec3 reflected = glm::reflect(rayIn.direction, rec.normal);
    glm::vec3 refracted =
        glm::refract(glm::normalize(rayIn.direction), outwardNormal, niOverNt);

    if (refracted != glm::vec3{0.0f}) {
      float reflectionProba = schlick(cosine, reflectiveIdx);
      if (randf() < reflectionProba) {
        scattered = Ray{rec.point, reflected};
      } else {
        scattered = Ray{rec.point, refracted};
      }
    } else {
      scattered = Ray{rec.point, reflected};
    }
    return true;
  }

  float reflectiveIdx;
};

class Camera {
 public:
  Camera(const glm::vec3& lookFrom, const glm::vec3& lookAt,
         const glm::vec3& up, float verticalFov, float aspectRatio,
         float aperture, float focusDist) {
    lensRadius = aperture * 0.5f;
    float theta = verticalFov * float(M_PI) / 180.0f;
    float halfHeight = tanf(theta * 0.5f);
    float halfWidth = aspectRatio * halfHeight;
    origin = lookFrom;
    w = glm::normalize(lookFrom - lookAt);
    u = glm::normalize(glm::cross(up, w));
    v = glm::cross(w, u);
    lowerLeftCorner = origin - focusDist * (halfWidth * u + halfHeight * v + w);
    horizontal = 2.0f * halfWidth * focusDist * u;
    vertical = 2.0f * halfHeight * focusDist * v;
  }

  Ray getRay(float s, float t) const {
    using MyRandom::randInUnitDisk;

    glm::vec3 rd = lensRadius * randInUnitDisk();
    glm::vec3 offset = u * rd.x + v * rd.y;
    return {origin + offset, lowerLeftCorner + s * horizontal + t * vertical -
                                 (origin + offset)};
  }

 private:
  glm::vec3 origin;
  glm::vec3 lowerLeftCorner;
  glm::vec3 horizontal;
  glm::vec3 vertical;
  glm::vec3 u, v, w;
  float lensRadius;
};

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

struct SceneResources {
  std::vector<std::unique_ptr<Material>> materials;
  std::vector<Sphere> spheres;
};

HitableList randomScene(SceneResources& resources) {
  using MyRandom::randf;

  int n = 500;
  resources.spheres.reserve(n + 1);

  resources.materials.push_back(
      std::make_unique<Lambertian>(glm::vec3{0.5f, 0.5f, 0.5f}));
  resources.spheres.emplace_back(glm::vec3{0.0f, -1000.0f, 0.0f}, 1000.0f,
                                 *resources.materials.back());

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float chooseMat = randf();
      glm::vec3 center(a + 0.9f * randf(), 0.2f, b + 0.9f * randf());
      if (glm::length(center - glm::vec3{4.0f, 0.2f, 0.0f}) > 0.9f) {
        if (chooseMat < 0.8f) {  // diffuse
          resources.materials.push_back(std::make_unique<Lambertian>(glm::vec3{
              randf() * randf(), randf() * randf(), randf() * randf()}));
          resources.spheres.emplace_back(center, 0.2f,
                                         *resources.materials.back());
        } else if (chooseMat < 0.95f) {  // metal
          resources.materials.push_back(std::make_unique<Metal>(
              glm::vec3{0.5f * (1.0f + randf()), 0.5f * (1.0f + randf()),
                        0.5f * (1.0f + randf())},
              0.5f * randf()));
          resources.spheres.emplace_back(center, 0.2f,
                                         *resources.materials.back());
        } else {  // glass
          resources.materials.push_back(std::make_unique<Dielectric>(1.5f));
          resources.spheres.emplace_back(center, 0.2f,
                                         *resources.materials.back());
        }
      }
    }

    resources.materials.push_back(std::make_unique<Dielectric>(1.5f));
    resources.spheres.emplace_back(glm::vec3{0.0f, 1.0f, 0.0f}, 1.0f,
                                   *resources.materials.back());

    resources.materials.push_back(
        std::make_unique<Lambertian>(glm::vec3{0.4f, 0.2f, 0.1f}));
    resources.spheres.emplace_back(glm::vec3{-4.0f, 1.0f, 0.0f}, 1.0f,
                                   *resources.materials.back());

    resources.materials.push_back(
        std::make_unique<Metal>(glm::vec3{0.7f, 0.6f, 0.5f}, 0.0f));
    resources.spheres.emplace_back(glm::vec3{4.0f, 1.0f, 0.0f}, 1.0f,
                                   *resources.materials.back());
  }

  HitableList list;
  for (const auto& sphere : resources.spheres) {
    list.append(sphere);
  }
  return list;
}

using Seconds = std::chrono::duration<float>;

template <class Func>
auto measure(Func&& f) {
  auto before = std::chrono::high_resolution_clock::now();
  f();
  auto after = std::chrono::high_resolution_clock::now();

  Seconds duration = after - before;
  return duration.count();
}

int main() {
  using MyRandom::randf;

  const int width = 1024, height = 768;
  const int samplesPerPixel = 256;

  SceneResources res;
  HitableList world = randomScene(res);

  glm::vec3 lookFrom{7.0f, 1.5f, 2.5f};
  glm::vec3 lookAt{0.0f, 0.0f, 0.0f};
  float focusDist = glm::length(lookFrom - lookAt);
  float aperture = 0.1f;
  Camera camera{lookFrom,
                lookAt,
                {0.0f, 1.0f, 0.0f},
                45.0f,
                float(width) / float(height),
                aperture,
                focusDist};

  std::vector<glm::u8vec3> image(width * height * 3);

  auto renderTile = [&](int startX, int startY, int sizeX, int sizeY) {
    for (int y = startY; y < std::min(startY + sizeY, height); y++) {
      for (int x = startX; x < std::min(startX + sizeX, width); x++) {
        glm::vec3 col{};
        for (int sample = 0; sample < samplesPerPixel; sample++) {
          float u = (float(x) + randf() - 0.5f) / float(width);
          float v = (float(y) + randf() - 0.5f) / float(height);

          col += color(camera.getRay(u, v), world, 0) / float(samplesPerPixel);
        }
        // gamma-correction
        col = glm::sqrt(col);

        image[x + y * width] = 255.99f * col;
      }
    }
  };

  const int groupSizeX = 32, groupSizeY = 32;

  // one thread runs
  auto render = [&](int group) {
    for (int groupY = 0; groupY < height / groupSizeY + 1; groupY++) {
      for (int groupX = 0; groupX < width / groupSizeX + 1; groupX++) {
        if ((groupX + groupY * height / groupSizeY) % 4 == group) {
          renderTile(groupX * groupSizeX, groupY * groupSizeY, groupSizeX,
                     groupSizeY);
        }
      }
    }
  };

  float renderingTimeInSeconds = measure([&]() {
    constexpr int threadsCount = 4;

    std::thread renderThreads[threadsCount];
    for (int i = 0; i < threadsCount; i++) {
      renderThreads[i] = std::thread(render, i);
    }
    for (int i = 0; i < threadsCount; i++) {
      renderThreads[i].join();
    }
  });

  std::cout << "Rendering time: " << renderingTimeInSeconds << "s\n";

  stbi_flip_vertically_on_write(1);
  stbi_write_bmp("sample.bmp", width, height, 3, image.data());

  return 0;
}
