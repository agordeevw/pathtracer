#pragma once
#include <vector>

#include "PathTracing/Hitable.h"

namespace PathTracing {
namespace Hitables {
class HitableList : public Hitable {
 public:
  ~HitableList() override = default;

  void append(const Hitable& hitable);

  bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;
  bool boundingBox(float t0, float t1, AABB& box) const override;

 private:
  std::vector<const Hitable*> hitables;
};
}  // namespace Hitables
}  // namespace PathTracing