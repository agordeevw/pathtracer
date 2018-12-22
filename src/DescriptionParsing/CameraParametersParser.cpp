#include "DescriptionParsing/JsonParser.h"
#include "PathTracing/Camera.h"

namespace DescriptionParsing {
using Type = PathTracing::CameraParameters;

void JsonParser<Type>::parse(const Json& jCameraParameters,
                             Type& cameraParameters) {
  try {
    cameraParameters.lookFrom = parseVec3(jCameraParameters["lookFrom"]);
    cameraParameters.lookAt = parseVec3(jCameraParameters["lookAt"]);
    cameraParameters.up = parseVec3(jCameraParameters["up"]);
    cameraParameters.fov = jCameraParameters["fov"].get<float>();
    cameraParameters.aperture = jCameraParameters["aperture"].get<float>();
    cameraParameters.focusDist = jCameraParameters["focusDist"].get<float>();
    cameraParameters.shutterOpenTime =
        jCameraParameters["shutterOpenTime"].get<float>();
    cameraParameters.shutterCloseTime =
        jCameraParameters["shutterCloseTime"].get<float>();
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Failed to parse camera parameters "
                                         "from input JSON file.\nDetails: ") +
                             e.what());
  }

  if (cameraParameters.aperture < 0.0f)
    throw std::runtime_error("Camera aperture is negative");
  if (cameraParameters.focusDist <= 0.0f)
    throw std::runtime_error("Camera focus distance is zero or negative");
  if (cameraParameters.fov <= 0.0f || cameraParameters.fov >= 180.0f)
    throw std::runtime_error(
        "Camera vertical fov is not in interval (0.0, 180.0)");
  if (cameraParameters.shutterCloseTime < cameraParameters.shutterOpenTime)
    throw std::runtime_error(
        "Camera shutter close time is earlier that open time");
}
}  // namespace DescriptionParsing
