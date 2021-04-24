//
// Created by juan.castellanos on 23/04/21.
//
#pragma once

#include <chrono>

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::chrono::system_clock;
using std::chrono::time_point;

using namespace std::chrono_literals;

using high_res_time_point = time_point<system_clock, duration<uint64_t, std::ratio<1, 1000000000>>>;

namespace Utils
{

/**
 * Gets the now time with the max possible precision
 * @return The high resolution time point
 */
high_res_time_point count()
{
  return high_resolution_clock::now();
}

/**
 * Calculates the elapsed microseconds since begin to end
 * @param begin The starting time point
 * @param end The ending time point
 * @return The microseconds elapsed
 */
auto elapsedTime(const high_res_time_point& begin, const high_res_time_point& end)
{
  return duration_cast<milliseconds>(end - begin).count();
}

}  // namespace Utils