#pragma once
#include "DescriptionParsing/JsonParser.h"
#include "PathTracing/PathTracing.h"

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
