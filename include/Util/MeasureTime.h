#pragma once
#include <chrono>

using Seconds = std::chrono::duration<float>;

template <class Func>
auto measureTime(Func&& f) {
  auto before = std::chrono::high_resolution_clock::now();
  f();
  auto after = std::chrono::high_resolution_clock::now();

  Seconds duration = after - before;
  return duration.count();
}