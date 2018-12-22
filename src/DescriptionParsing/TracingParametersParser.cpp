#include "DescriptionParsing/JsonParser.h"
#include "PathTracing/PathTracing.h"

namespace DescriptionParsing {
using Type = PathTracing::TracingParameters;

void JsonParser<Type>::parse(const Json& jTracingParameters,
                             Type& tracingParameters) {
  try {
    tracingParameters.imageWidth = jTracingParameters["imageWidth"].get<int>();
    tracingParameters.imageHeight =
        jTracingParameters["imageHeight"].get<int>();
    tracingParameters.samplesPerPixel =
        jTracingParameters["samplesPerPixel"].get<int>();
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Failed to parse tracing parameters "
                                         "from input JSON file.\nDetails: ") +
                             e.what());
  }

  if (tracingParameters.imageHeight <= 0)
    throw std::runtime_error("Image height <= 0");
  if (tracingParameters.imageWidth <= 0)
    throw std::runtime_error("Image width <= 0");
  if (tracingParameters.samplesPerPixel <= 0)
    throw std::runtime_error("Samples per pixel <= 0");
}
}  // namespace DescriptionParsing
