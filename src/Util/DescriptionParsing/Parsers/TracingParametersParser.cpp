#include "Util/DescriptionParsing/Parsers/TracingParametersParser.h"

namespace Util {
namespace DescriptionParsing {
namespace Parsers {
void TracingParametersParser::parse(const Json& jTracingParameters) {
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

}  // namespace Parsers
}  // namespace DescriptionParsing
}  // namespace Util