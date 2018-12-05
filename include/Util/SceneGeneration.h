#pragma once
#include <string>

#include "PathTracing/Scene.h"

namespace Util {
namespace SceneGeneration {
PathTracing::Scene generateScene(const std::string& name);
}  // namespace SceneGeneration
}  // namespace Util