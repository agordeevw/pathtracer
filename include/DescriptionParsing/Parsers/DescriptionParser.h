#pragma once
#include "DescriptionParsing/JsonParser.h"
#include "PathTracing/Description.h"

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
