#pragma once
#include <memory>
#include <vector>

#include "Hitable.h"
#include "Hitables/HitableList.h"
#include "Material.h"

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
    world.append(*hitablesStorage.back());
    return *hitablesStorage.back();
  }

  const HitableList& getWorld() const { return world; }

 private:
  HitableList world;
  std::vector<std::unique_ptr<Material>> materialsStorage;
  std::vector<std::unique_ptr<Hitable>> hitablesStorage;
};