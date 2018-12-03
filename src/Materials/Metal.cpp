#include <glm/geometric.hpp>

#include "Materials/Metal.h"
#include "Hitables/Hitable.h"
#include "Util/MyRandom.h"
#include "Ray.h"

Metal::Metal(const glm::vec3& albedo, float fuzziness) : albedo(albedo) {
  if (fuzziness < 1.0f)
    fuzziness = fuzziness;
  else
    fuzziness = 1.0f;
}

bool Metal::scatter(const Ray& rayIn, const HitRecord& rec,
                    glm::vec3& attenuation, Ray& scattered) const {
  glm::vec3 reflected =
      glm::reflect(glm::normalize(rayIn.direction), rec.normal);
  scattered =
      Ray{rec.point, reflected + fuzziness * MyRandom::randInUnitSphere()};
  attenuation = albedo;
  return glm::dot(scattered.direction, rec.normal) > 0.0f;
}
