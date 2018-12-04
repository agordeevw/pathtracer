#pragma once
#include <memory>
#include <vector>

#include "PathTracing/Hitable.h"
#include "PathTracing/Hitables/BVH.h"
#include "PathTracing/Hitables/HitableList.h"
#include "PathTracing/Material.h"

namespace PathTracing {
class Material;
class Hitable;

class Scene {
 public:
  template <class T, class... Args>
  const Material& createMaterial(Args&&... args) {
    materialsStorage.emplace_back(
        std::make_unique<T>(std::forward<Args>(args)...));
    return *materialsStorage.back();
  }

  template <class T, class... Args>
  const Hitable& createHitable(Args&&... args) {
    hitablesStorage.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    hitables.push_back(hitablesStorage.back().get());
    return *hitablesStorage.back();
  }

  void buildBVH(float time0, float time1) const {
    bvh = Hitables::BVH{hitables.data(), hitables.data() + hitables.size(),
                        time0, time1};
  }

  const Hitable& getWorld() const { return bvh; }

 private:
  std::vector<std::unique_ptr<Material>> materialsStorage;
  std::vector<std::unique_ptr<Hitable>> hitablesStorage;
  mutable std::vector<Hitable*> hitables;
  mutable Hitables::BVH bvh;
};
}  // namespace PathTracing