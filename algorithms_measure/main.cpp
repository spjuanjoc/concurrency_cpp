/**
 * @brief Measure the elapsed time of callables using decorators.
 *
 * @author spjuanjoc
 * @date   2020-01-24
 */

#include <fmt/core.h>

#include "TimeMeasure.hpp"

#include <list>
#include <numeric>
#include <vector>

using namespace TimeMeasure;

int
main()
{
  fmt::println("Measure algorithms elapsed time");
  fmt::println("|{:^15}|{:^10}|{:^10}|", "Algorithm", "Time (ms)", "Result");
  fmt::println("{:-^39}","");

  std::vector<double> values(6'000'000, 0.5);

  auto measureAccumulate = [&values]
  {
    const auto initial_value = 0.0;
    return std::accumulate(values.begin(), values.end(), initial_value);
  };
  measure("std::accumulate", measureAccumulate)();

  auto wasFound = [&values](double value)
  {
    const auto result = std::find(values.begin(), values.end(), value);

    return !(result == std::end(values));
  };

  constexpr auto value_in_collection     = 0.5;
  constexpr auto value_not_in_collection = 0.6;

  auto wasFound1 = [&]
  {
    return wasFound(value_not_in_collection);
  };
  auto wasFound2 = [&]
  {
    return wasFound(value_in_collection);
  };

  measure("std::find", wasFound1)();
  runAndMeasure("std::find", wasFound2);

  return 0;
}
