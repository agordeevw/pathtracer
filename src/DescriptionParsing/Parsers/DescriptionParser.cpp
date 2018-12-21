#include "DescriptionParsing/Parsers/DescriptionParser.h"
#include "DescriptionParsing/Parsers/CameraParametersParser.h"
#include "DescriptionParsing/Parsers/SceneParser.h"
#include "DescriptionParsing/Parsers/TracingParametersParser.h"

namespace DescriptionParsing {
namespace Parsers {
void DescriptionParser::parse(const Json& jDescription) {
  CameraParametersParser cpParser(description.cameraParams);
  cpParser.parse(jDescription["cameraParameters"]);

  TracingParametersParser tpParser(description.tracingParams);
  tpParser.parse(jDescription["tracingParameters"]);

  SceneParser sParser(description.scene);
  sParser.parse(jDescription["scene"]);
}
}  // namespace Parsers
}  // namespace DescriptionParsing
