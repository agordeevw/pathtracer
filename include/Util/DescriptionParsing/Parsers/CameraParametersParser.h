#pragma once
#include "PathTracing/Camera.h"
#include "Util/DescriptionParsing/JsonParser.h"

namespace Util {
namespace DescriptionParsing {
namespace Parsers {
class CameraParametersParser : public JsonParser {
 public:
  CameraParametersParser(PathTracing::CameraParameters& cameraParameters)
      : cameraParameters(cameraParameters) {}
  void parse(const Json& jCameraParameters);

 private:
  PathTracing::CameraParameters& cameraParameters;
};
}  // namespace Parsers
}  // namespace DescriptionParsing
}  // namespace Util