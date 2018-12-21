#pragma once
#include "DescriptionParsing/JsonParser.h"
#include "PathTracing/Camera.h"

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
