#include <glm/geometric.hpp>

#include "Ray.h"
#include "Scene/Hitables/Sphere.h"

Sphere::Sphere(const glm::vec3& center, float radius, const Material& material)
    : center(center), radius(radius), material(&material) {}

bool Sphere::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
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
