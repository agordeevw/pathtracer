#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include "PathTracing/Hitables/MovingSphere.h"
#include "PathTracing/Ray.h"

namespace PathTracing {
namespace Hitables {
MovingSphere::MovingSphere(const glm::vec3& startCenter,
                           const glm::vec3& finishCenter, float startTime,
                           float finishTime, float radius,
                           const Material& material)
    : startCenter(startCenter),
      finishCenter(finishCenter),
      startTime(startTime),
      finishTime(finishTime),
      radius(radius),
      material(&material) {}

bool MovingSphere::hit(const Ray& r, float tMin, float tMax,
                       HitRecord& rec) const {
  glm::vec3 oc = r.origin - centerAtTime(r.time);
  float a = glm::dot(r.direction, r.direction);
  float b = glm::dot(oc, r.direction);
  float c = glm::dot(oc, oc) - radius * radius;
  float discr = b * b - a * c;
  if (discr > 0) {
    float temp = (-b - sqrtf(discr)) / a;
    if (temp < tMax && temp > tMin) {
      rec.t = temp;
      rec.point = r.pointAtT(rec.t);
      rec.normal = (rec.point - centerAtTime(r.time)) / radius;
      rec.material = material;
      return true;
    }
    temp = (-b + sqrtf(discr)) / a;
    if (temp < tMax && temp > tMin) {
      rec.t = temp;
      rec.point = r.pointAtT(rec.t);
      rec.normal = (rec.point - centerAtTime(r.time)) / radius;
      rec.material = material;
      return true;
    }
  }
  return false;
}

glm::vec3 MovingSphere::centerAtTime(float time) const {
  return startCenter + (time - startTime) / (finishTime - startTime) *
                           (finishCenter - startCenter);
}
}  // namespace Hitables
}  // namespace PathTracing