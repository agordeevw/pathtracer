#include <glm/geometric.hpp>

#include "PathTracing/Hitable.h"
#include "PathTracing/Materials/Metal.h"
#include "PathTracing/Ray.h"
#include "Util/Random.h"

namespace PathTracing {
namespace Materials {
Metal::Metal(const Texture& albedo, float fuzziness)
    : albedo(albedo), fuzziness(std::min(std::max(fuzziness, 0.0f), 1.0f)) {}

bool Metal::scatter(const Ray& rayIn, const HitRecord& rec,
                    glm::vec3& attenuation, Ray& scattered) const {
  glm::vec3 reflected =
      glm::reflect(glm::normalize(rayIn.direction), rec.normal);
  scattered =
      Ray{rec.point, reflected + fuzziness * Util::Random::randInUnitSphere(),
          rayIn.time};
  attenuation = albedo.sample(rec.u, rec.v, rec.point);
  return glm::dot(scattered.direction, rec.normal) > 0.0f;
}
}  // namespace Materials
}  // namespace PathTracing