#include <algorithm>

#include "PathTracing/Hitables/BVH.h"
#include "Util/Random.h"

namespace PathTracing {
namespace Hitables {
BVH::BVH(Hitable** begin, Hitable** end, float time0, float time1) {
  // please don't mess up the estimates...
  nodes.reserve(2 * (end - begin) + 1);
  BVHNode* dummy = createNode(begin, end, time0, time1);
}
bool BVH::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
  if (nodes.size() > 0) {
    return nodes[0].hit(r, tMin, tMax, rec);
  }
  return false;
}
bool BVH::boundingBox(float t0, float t1, AABB& box) const {
  if (nodes.size() > 0) {
    return nodes[0].boundingBox(t0, t1, box);
  }
  return false;
}
BVHNode* BVH::createNode(Hitable** begin, Hitable** end, float time0,
                         float time1) {
  nodes.emplace_back();
  BVHNode& ret = nodes.back();
  const auto n = end - begin;

  if (n == 1) {
    ret.left = ret.right = *begin;
  } else {
    int axis = int(3 * Util::Random::randf());
    if (axis == 3) axis = 2;  // if randf() returns 1.0f
    std::sort(begin, end,
              [time0, time1, axis](const Hitable* l, const Hitable* r) {
                AABB boxLeft{}, boxRight{};
                bool leftHasBox = l->boundingBox(time0, time1, boxLeft);
                bool rightHasBox = r->boundingBox(time0, time1, boxRight);
                // NOTE: no error checking happens here
                // what if hitables don't have bounding boxes?
                return boxLeft.min[axis] < boxRight.min[axis];
              });

    if (n == 2) {
      ret.left = begin[0];
      ret.right = begin[1];
    } else {
      ret.left = createNode(begin, begin + n / 2, time0, time1);
      ret.right = createNode(begin + n / 2, end, time0, time1);
    }
  }

  AABB boxLeft, boxRight;
  bool leftHasBox = ret.left->boundingBox(time0, time1, boxLeft);
  bool rightHasBox = ret.right->boundingBox(time0, time1, boxRight);
  // NOTE: no error checking happens here
  // what if hitables don't have bounding boxes?
  ret.box = AABB::surroundingBox(boxLeft, boxRight);

  return &ret;
}
}  // namespace Hitables
}  // namespace PathTracing