#include "fmt/core.h"
#include "Utils.hpp"
#include <algorithm>
#include <list>
#include <numeric>
#include <vector>

int main()
{
  fmt::print("Measure algorithms elapsed time\n");

  std::vector<double> v(6'000'000, 0.5);

  auto acc_l = [&v] { return std::accumulate(v.begin(), v.end(), 0.0); };
  measure("std::accumulate", acc_l)();

  auto find_l = [&v]
  {
    auto res = std::find(v.begin(), v.end(), 0.6);
    return res == std::end(v) ? 0.0 : 1.0;
  };
  measure("std::find", find_l)();

  auto find_l_2 = [&v]
  {
    auto res = std::find(v.begin(), v.end(), 0.5);
    return res == std::end(v) ? 0.0 : 1.0;
  };
  runAndMeasure("std::find", find_l_2);

  return 0;
}
