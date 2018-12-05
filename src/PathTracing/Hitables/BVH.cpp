#include <algorithm>

#include "PathTracing/Hitables/BVH.h"
#include "Util/Random.h"

namespace PathTracing {
namespace Hitables {
BVH::BVH(Hitable** begin, Hitable** end, float time0, float time1) {
  // reasonable estimate for used bvh generation algorithm
  nodes.reserve(2 * (end - begin) + 1);
  root = createNode(begin, end, time0, time1);
}
bool BVH::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
  return root->hit(r, tMin, tMax, rec);
}
bool BVH::boundingBox(float t0, float t1, AABB& box) const {
  return root->boundingBox(t0, t1, box);
}
BVH::BVHNode::BVHNode(Hitable* left, Hitable* right, float time0, float time1)
    : left(left), right(right) {
  AABB boxLeft{}, boxRight{};
  bool leftHasBox = left->boundingBox(time0, time1, boxLeft);
  bool rightHasBox = right->boundingBox(time0, time1, boxRight);
  box = AABB::surroundingBox(boxLeft, boxRight);
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
    nodes.emplace_back(begin[0], begin[0], time0, time1);
  } else if (n == 2) {
    nodes.emplace_back(begin[0], begin[1], time0, time1);
  } else {
    auto median = begin + n / 2;
    BVH::BVHNode* leftNode = createNode(begin, median, time0, time1);
    BVH::BVHNode* rightNode = createNode(median, end, time0, time1);
    nodes.emplace_back(leftNode, rightNode, time0, time1);
  }

  return &nodes.back();
}
}  // namespace Hitables
}  // namespace PathTracing