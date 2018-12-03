#include <iostream>

#ifdef _MSC_VER
#pragma warning(disable : 4201)  // glm hits this warning a lot
#endif

#include "PathTracingApplication.h"
#include "Util/ParseArgs.h"

int main(int argc, char** argv) {
  Util::ParseArgs::ProgramOptions options;
  if (!Util::ParseArgs::parseArgs(argc, argv, options)) {
    return 1;
  }

  try {
    PathTracingApplication app{options.inputPath, options.threadCount};
    app.run(options.outputPath);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
