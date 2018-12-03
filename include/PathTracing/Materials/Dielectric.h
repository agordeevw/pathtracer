#include "PathTracing/Material.h"

namespace PathTracing {
namespace Materials {
class Dielectric : public Material {
 public:
  Dielectric(float ri);
  ~Dielectric() override = default;

  bool scatter(const Ray& rayIn, const HitRecord& rec, glm::vec3& attenuation,
               Ray& scattered) const override;

  float reflectiveIdx;
};
}  // namespace Materials
}  // namespace PathTracing
