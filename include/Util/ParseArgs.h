#pragma once
#include <string>

namespace Util {
namespace ParseArgs {
struct ProgramOptions {
  std::string inputPath;
  std::string outputPath;
  int threadCount;
};

bool parseArgs(int argc, char** argv, ProgramOptions& opts);
}  // namespace ParseArgs
}  // namespace Util
