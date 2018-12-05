#include <algorithm>

#include "PathTracing/Hitables/BVH.h"
#include "Util/Random.h"

namespace PathTracing {
namespace Hitables {
BVH::BVH(Hitable** begin, Hitable** end, float time0, float time1) {
  // reasonable estimate for used bvh generation algorithm
  nodes.reserve(2 * (end - begin) + 1);
  BVHNode* unused = createNode(begin, end, time0, time1);
}
bool BVH::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
  return nodes.size() > 0 ? nodes[0].hit(r, tMin, tMax, rec) : false;
}
bool BVH::boundingBox(float t0, float t1, AABB& box) const {
  return nodes.size() > 0 ? nodes[0].boundingBox(t0, t1, box) : false;
}
bool BVH::BVHNode::hit(const Ray& r, float tMin, float tMax,
                       HitRecord& rec) const {
  if (box.hit(r, tMin, tMax)) {
    HitRecord leftRec, rightRec;
    bool hitLeft = left->hit(r, tMin, tMax, leftRec);
    bool hitRight = right->hit(r, tMin, tMax, rightRec);
    if (hitLeft && hitRight) {
      rec = leftRec.t < rightRec.t ? leftRec : rightRec;
      return true;
    } else if (hitLeft) {
      rec = leftRec;
      return true;
    } else if (hitRight) {
      rec = rightRec;
      return true;
    }
  }
  return false;
}
bool BVH::BVHNode::boundingBox(float t0, float t1, AABB& box) const {
  box = this->box;
  return true;
}
BVH::BVHNode* BVH::createNode(Hitable** begin, Hitable** end, float time0,
                              float time1) {
  nodes.emplace_back();
  BVHNode& ret = nodes.back();
  const auto n = end - begin;

  int axis = int(3 * Util::Random::randf()) % 3;
  std::sort(begin, end,
            [time0, time1, axis](const Hitable* l, const Hitable* r) {
              AABB boxLeft{}, boxRight{};
              bool leftHasBox = l->boundingBox(time0, time1, boxLeft);
              bool rightHasBox = r->boundingBox(time0, time1, boxRight);
              // NOTE: no error checking happens here
              // what if hitables don't have bounding boxes?
              return boxLeft.min[axis] < boxRight.min[axis];
            });

  if (n == 1) {
    ret.left = begin[0];
    ret.right = begin[0];
  } else if (n == 2) {
    ret.left = begin[0];
    ret.right = begin[1];
  } else {
    auto median = begin + n / 2;
    ret.left = createNode(begin, median, time0, time1);
    ret.right = createNode(median, end, time0, time1);
  }

  AABB boxLeft{}, boxRight{};
  bool leftHasBox = ret.left->boundingBox(time0, time1, boxLeft);
  bool rightHasBox = ret.right->boundingBox(time0, time1, boxRight);
  // NOTE: no error checking happens here
  // what if hitables don't have bounding boxes?
  ret.box = AABB::surroundingBox(boxLeft, boxRight);

  return &ret;
}
}  // namespace Hitables
}  // namespace PathTracing