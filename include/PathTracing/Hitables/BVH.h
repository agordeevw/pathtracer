#pragma once
#include <vector>

#include "PathTracing/Hitable.h"
#include "PathTracing/Hitables/BVHNode.h"

namespace PathTracing {
class AABB;

namespace Hitables {
class BVH : public Hitable {
 public:
  BVH() = default;
  BVH(const BVH&) = default;
  BVH(BVH&&) = default;
  BVH(Hitable** begin, Hitable** end, float time0, float time1);
  ~BVH() override = default;
  BVH& operator=(const BVH&) = default;
  BVH& operator=(BVH&&) = default;

  bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;
  bool boundingBox(float t0, float t1, AABB& box) const override;

 private:
  BVHNode* createNode(Hitable** begin, Hitable** end, float time0, float time1);

  std::vector<BVHNode> nodes;
};
}  // namespace Hitables
}  // namespace PathTracing