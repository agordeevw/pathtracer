#pragma once
#include "PathTracing/PathTracing.h"
#include "Util/DescriptionParsing/JsonParser.h"

namespace Util {
namespace DescriptionParsing {
namespace Parsers {
class TracingParametersParser : public JsonParser {
 public:
  TracingParametersParser(PathTracing::TracingParameters& tracingParameters)
      : tracingParameters(tracingParameters) {}
  void parse(const Json& jTracingParameters);

 private:
  PathTracing::TracingParameters& tracingParameters;
};
}  // namespace Parsers
}  // namespace DescriptionParsing
}  // namespace Util