#include "Scene/Hitables/HitableList.h"

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
