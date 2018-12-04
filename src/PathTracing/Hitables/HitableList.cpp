#include "PathTracing/Hitables/HitableList.h"
#include "PathTracing/AABB.h"

namespace PathTracing {
namespace Hitables {
void HitableList::append(const Hitable& hitable) {
  hitables.push_back(&hitable);
}

bool HitableList::hit(const Ray& r, float tMin, float tMax,
                      HitRecord& rec) const {
  HitRecord tempRec;
  bool hitAnything = false;
  float closestHit = tMax;

  for (const auto& hitable : hitables) {
    if (hitable->hit(r, tMin, closestHit, tempRec)) {
      hitAnything = true;
      closestHit = tempRec.t;
      rec = tempRec;
    }
  }
  return hitAnything;
}

bool HitableList::boundingBox(float t0, float t1, AABB& box) const {
  if (hitables.empty()) return false;
  AABB tempBox;
  if (hitables[0]->boundingBox(t0, t1, tempBox))
    box = tempBox;
  else
    return false;
  for (int i = 1; i < hitables.size(); i++) {
    if (hitables[i]->boundingBox(t0, t1, tempBox))
      box = AABB::surroundingBox(box, tempBox);
    else
      return false;
  }
  return true;
}
}  // namespace Hitables
}  // namespace PathTracing