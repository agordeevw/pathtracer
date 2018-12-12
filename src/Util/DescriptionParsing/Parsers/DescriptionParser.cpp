#include "Util/DescriptionParsing/Parsers/DescriptionParser.h"
#include "Util/DescriptionParsing/Parsers/CameraParametersParser.h"
#include "Util/DescriptionParsing/Parsers/SceneParser.h"
#include "Util/DescriptionParsing/Parsers/TracingParametersParser.h"

namespace Util {
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
}  // namespace Util