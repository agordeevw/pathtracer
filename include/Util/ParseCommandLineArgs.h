#pragma once
#include <string>

namespace Util {
struct ProgramOptions {
  std::string inputPath;
  std::string outputPath;
  int threadCount;
};

bool parseArgs(int argc, char** argv, ProgramOptions& opts);
}  // namespace Util
