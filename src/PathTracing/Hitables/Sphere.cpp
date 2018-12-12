#define _USE_MATH_DEFINES
#include <cmath>

#include <glm/geometric.hpp>

#include "PathTracing/AABB.h"
#include "PathTracing/Hitables/Sphere.h"
#include "PathTracing/Ray.h"

namespace PathTracing {
namespace Hitables {
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
      glm::vec3 localPoint = (rec.point - center) / radius;
      rec.u = float(1.0f - (atan2f(localPoint.z, localPoint.x) + M_PI) * 0.5f / M_PI);
      rec.v = float((asinf(localPoint.y) + 0.5f * M_PI) / M_PI);
      return true;
    }
    temp = (-b + sqrtf(discr)) / a;
    if (temp < tMax && temp > tMin) {
      rec.t = temp;
      rec.point = r.pointAtT(rec.t);
      rec.normal = (rec.point - center) / radius;
      rec.material = material;
      glm::vec3 localPoint = (rec.point - center) / radius;
      rec.u = float(1.0f - (atan2f(localPoint.z, localPoint.x) + M_PI) * 0.5f / M_PI);
      rec.v = float((asinf(localPoint.y) + 0.5f * M_PI) / M_PI);
      return true;
    }
  }
  return false;
}

bool Sphere::boundingBox(float t0, float t1, AABB& box) const {
  box = AABB{center - glm::vec3{radius, radius, radius},
             center + glm::vec3{radius, radius, radius}};
  return true;
}
}  // namespace Hitables
}  // namespace PathTracing