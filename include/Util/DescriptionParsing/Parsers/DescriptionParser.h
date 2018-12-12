#pragma once
#include "PathTracing/Description.h"
#include "Util/DescriptionParsing/JsonParser.h"

namespace Util {
namespace DescriptionParsing {
namespace Parsers {
class DescriptionParser : public JsonParser {
 public:
  DescriptionParser(PathTracing::Description& description)
      : description(description) {}
  void parse(const Json& jDescription);

 private:
  PathTracing::Description& description;
};
}  // namespace Parsers
}  // namespace DescriptionParsing
}  // namespace Util