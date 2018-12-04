#include "PathTracing\Hitables\BVHNode.h"

namespace PathTracing {
namespace Hitables {
bool BVHNode::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
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
bool BVHNode::boundingBox(float t0, float t1, AABB& box) const {
  box = this->box;
  return true;
}
}  // namespace Hitables
}  // namespace PathTracing