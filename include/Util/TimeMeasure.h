#pragma once
#include <chrono>

namespace Util {
  template <class F>
  float measureExecutionTime(F&& f) {
    using Seconds = std::chrono::duration<float>;

    auto timeBeforeCall = std::chrono::high_resolution_clock::now();
    f();
    auto timeAfterCall = std::chrono::high_resolution_clock::now();
    Seconds totalTime = timeAfterCall - timeBeforeCall;

    return totalTime.count();
  }
}  // namespace Util