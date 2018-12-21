#include <algorithm>
#include <iostream>

#include "PathTracing/Hitables/BVH.h"
#include "Util/Random.h"

namespace PathTracing {
namespace Hitables {
BVH::BVH() : root(nullptr) {}
BVH::BVH(Hitable** begin, Hitable** end, float time0, float time1) {
  // reasonable estimate for used bvh generation algorithm
  nodes.reserve(2 * (end - begin) + 1);
  root = createNode(begin, end, time0, time1);
}
bool BVH::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
  return root ? root->hit(r, tMin, tMax, rec) : false;
}
bool BVH::boundingBox(float t0, float t1, AABB& box) const {
  return root ? root->boundingBox(t0, t1, box) : false;
}
BVH::BVHNode::BVHNode(Hitable* left, Hitable* right, float time0, float time1)
    : left(left), right(right) {
  AABB boxLeft{}, boxRight{};
  bool leftHasBox = left->boundingBox(time0, time1, boxLeft);
  bool rightHasBox = right->boundingBox(time0, time1, boxRight);
  if (leftHasBox && rightHasBox) {
    box = AABB::surroundingBox(boxLeft, boxRight);
  } else {
    std::cerr << "Hitable doesn't have a bounding box\n";
    if (leftHasBox) {
      box = boxLeft;
    } else if (rightHasBox) {
      box = boxRight;
    } else {
      // box is reduced to a point, shouldn't be hit by most rays
      box = {};
    }
  }
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
bool BVH::BVHNode::boundingBox(float, float, AABB& box) const {
  // BVH is static in time, therefore t0 and t1 are unused
  box = this->box;
  return true;
}
BVH::BVHNode* BVH::createNode(Hitable** begin, Hitable** end, float time0,
                              float time1) {
  const auto n = end - begin;

  if (n <= 0) {
    throw std::logic_error("Incorrect hitables range provided to BVH");
  }

  int axis = int(3 * Util::Random::randf()) % 3;
  std::sort(begin, end,
            [time0, time1, axis](const Hitable* l, const Hitable* r) {
              AABB boxLeft{}, boxRight{};
              bool leftHasBox = l->boundingBox(time0, time1, boxLeft);
              bool rightHasBox = r->boundingBox(time0, time1, boxRight);
              // Will check for missing boxes at BVHNode ctor
              // while creating the BVH tree
              return boxLeft.min[axis] < boxRight.min[axis];
            });

  if (n < 3) {
    // n == 1 -> n/2 = 0
    // n == 2 -> n/2 = 1
    nodes.emplace_back(begin[0], begin[n / 2], time0, time1);
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