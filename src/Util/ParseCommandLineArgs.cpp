#include <cxxopts.hpp>

#include "Util/ParseCommandLineArgs.h"

namespace Util {
namespace ParseArgs {
bool parseArgs(int argc, char** argv, ProgramOptions& opts) {
  cxxopts::Options options(argv[0]);
  auto& addOption = options.add_options();
  addOption("i,input", "Path to path tracer settings JSON file",
            cxxopts::value<std::string>(), "PATH");
  addOption("o,output", "Path to resulting image saved in bmp format",
            cxxopts::value<std::string>()->default_value("trace.bmp"), "PATH");
  addOption("t,thread-count", "Number of threads for pathtracer to run",
            cxxopts::value<int>()->default_value("4"));
  addOption("help", "Print this help");

  try {
    auto result = options.parse(argc, argv);

    if (result.count("help") > 0) {
      std::cout << options.help() << std::endl;
      return false;
    }

    if (!result.count("input")) {
      std::cerr << "Error: no input file specified.\n";
      return false;
    }

    if (result.count("thread-count") && result["thread-count"].as<int>() <= 0) {
      std::cerr << "Error: thread-count cannot be nonpositive\n";
      return false;
    }

    ProgramOptions tempOpts{};
    tempOpts.inputPath = result["input"].as<std::string>();
    tempOpts.outputPath = result["output"].as<std::string>();
    tempOpts.threadCount = result["thread-count"].as<int>();

    opts = std::move(tempOpts);
  } catch (const cxxopts::OptionException& e) {
    std::cerr << "Error: " << e.what();
    return false;
  }

  return true;
}
}  // namespace ParseArgs
}  // namespace Util