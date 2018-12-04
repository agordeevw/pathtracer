#pragma once
#include "PathTracing/AABB.h"
#include "PathTracing/Hitable.h"

namespace PathTracing {
namespace Hitables {
class BVHNode : public Hitable {
 public:
  ~BVHNode() override = default;
  bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;
  bool boundingBox(float t0, float t1, AABB& box) const override;

  Hitable* left;
  Hitable* right;
  AABB box;
};

}  // namespace Hitables
}  // namespace PathTracing