#include "DescriptionParsing/JsonParser.h"
#include "PathTracing/Description.h"

namespace DescriptionParsing {
using Type = PathTracing::Description;

void JsonParser<Type>::parse(const Json& jDescription, Type& description) {
  JsonParser<PathTracing::CameraParameters>::parse(
      jDescription["cameraParameters"], description.cameraParams);
  JsonParser<PathTracing::TracingParameters>::parse(
      jDescription["tracingParameters"], description.tracingParams);
  JsonParser<PathTracing::Scene>::parse(jDescription["scene"],
                                        description.scene);
}
}  // namespace DescriptionParsing
