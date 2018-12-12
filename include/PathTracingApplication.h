#pragma once
#include <string>

#include "PathTracing/Description.h"

class PathTracingApplication {
 public:
  PathTracingApplication(const std::string& descriptionFilePath,
                         int threadCount);
  void run(const std::string& outputFile);

 private:
  void loadDescription(const std::string& descriptionFilePath);

  PathTracing::Description description;
};