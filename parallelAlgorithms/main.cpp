#include "fmt/core.h"
#include <algorithm>
#include <list>
#include <mutex>
#include <numeric>
#include <vector>

template<typename TFunc>
void runAndMeasure(std::string_view title, TFunc func)
{
  using std::chrono::duration;
  using std::chrono::high_resolution_clock;
  using dur_ratio = std::milli;

  const auto start   = high_resolution_clock::now();
  auto       function_return = func();
  const auto end     = high_resolution_clock::now();
  const auto elapsed = duration<double, dur_ratio>(end - start);
  fmt::print("{} : time: {} ms, result: {} \n", title, elapsed.count(), function_return);
}

int main()
{
  fmt::print("Hello, Concurrency!\n");

  std::vector<double> v(6'000'000, 0.5);

  auto acc_l = [&v] { return std::accumulate(v.begin(), v.end(), 0.0); };
  runAndMeasure("std::accumulate", acc_l);

  auto find_l = [&v]
  {
    auto res = std::find(v.begin(), v.end(), 0.6);
    return res == std::end(v) ? 0.0 : 1.0;
  };
  runAndMeasure("std::find", find_l);

  auto find_l_2 = [&v]
  {
    auto res = std::find(v.begin(), v.end(), 0.5);
    return res == std::end(v) ? 0.0 : 1.0;
  };
  runAndMeasure("std::find", find_l_2);

  return 0;
}
