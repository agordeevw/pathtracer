#include <fstream>
#include <iostream>

#include "DescriptionParsing/JsonParser.h"
#include "PathTracing/Camera.h"
#include "PathTracingApplication.h"
#include "Util/TimeMeasure.h"

PathTracingApplication::PathTracingApplication(
    const std::string& descriptionFilePath, int threadCount) {
  loadDescription(descriptionFilePath);

  description.tracingParams.threadsCount = threadCount;
  description.cameraParams.aspectRatio =
      (float)description.tracingParams.imageWidth /
      (float)description.tracingParams.imageHeight;
}

void PathTracingApplication::run(const std::string& outputFile) {
  PathTracing::Camera camera{description.cameraParams};
  Util::Image image{};

  std::cout << "Tracing...\n";
  float traceTime = Util::measureExecutionTime([this, &camera, &image]() {
    image = PathTracing::traceScene(description.scene, camera,
                                    description.tracingParams);
  });
  std::cout << "Done after " << traceTime << " s.\n";

  if (!image.writeToFile(outputFile.c_str())) {
    throw std::runtime_error("Failed to write image to file \"" + outputFile);
  }

  std::cout << "Output written to file \"" << outputFile << "\"\n";
}

void PathTracingApplication::loadDescription(
    const std::string& descriptionFilePath) {
  std::ifstream is(descriptionFilePath);

  using DescriptionJsonParser = DescriptionParsing::JsonParser<PathTracing::Description>;
  DescriptionJsonParser::Json jDescription;
  is >> jDescription;

  DescriptionJsonParser::parse(jDescription, description);
}
