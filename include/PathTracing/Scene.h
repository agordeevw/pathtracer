#pragma once
#include <memory>
#include <vector>

#include "PathTracing/Hitable.h"
#include "PathTracing/Hitables/BVH.h"
#include "PathTracing/Hitables/HitableList.h"
#include "PathTracing/Material.h"
#include "PathTracing/Texture.h"

namespace PathTracing {
class Scene {
 public:
  template <class T, class... Args>
  const Material& createMaterial(Args&&... args) {
    materialsStorage.push_back(
        std::make_unique<T>(std::forward<Args>(args)...));
    return *materialsStorage.back();
  }

  const Material& getMaterialById(int id) {
    if (id >= 0 && id < materialsStorage.size())
      return *materialsStorage[id].get();
    else
      throw std::runtime_error("Invalid material id");
  }

  template <class T, class... Args>
  const Texture& createTexture(Args&&... args) {
    texturesStorage.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    return *texturesStorage.back();
  }

  const Texture& getTextureById(int id) {
    if (id >= 0 && id < texturesStorage.size())
      return *texturesStorage[id].get();
    else
      throw std::runtime_error("Invalid texture id");
  }

  template <class T, class... Args>
  const Hitable& createHitable(Args&&... args) {
    hitablesStorage.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    return *hitablesStorage.back();
  }

  void buildBVH(float time0, float time1) const {
    std::vector<Hitable*> hitables;
    for (auto& hitablePtr : hitablesStorage) {
      hitables.push_back(hitablePtr.get());
    }
    if (hitables.size() > 0) {
      bvh = Hitables::BVH{hitables.data(), hitables.data() + hitables.size(),
                          time0, time1};
    }
  }

  const Hitable& getWorld() const { return bvh; }

 private:
  std::vector<std::unique_ptr<Texture>> texturesStorage;
  std::vector<std::unique_ptr<Material>> materialsStorage;
  std::vector<std::unique_ptr<Hitable>> hitablesStorage;
  mutable Hitables::BVH bvh;
};
}  // namespace PathTracing