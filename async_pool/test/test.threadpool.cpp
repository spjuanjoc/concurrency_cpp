#include "catch2/catch.hpp"
#include "ParallelAccumulate.hpp"

TEST_CASE("should accumulate a vector's values", "[std::accumulate]")
{
  using std::vector;
  using std::begin;
  using std::end;

  vector values{1,2,3,4,5,6,7,8,9};

  auto result = AccumulateBlock<vector<int>::iterator, int>()(begin(values), end(values));

  REQUIRE( result == 45);
}

TEST_CASE("should accumulate using a waitable thread pool", "[thread pool][accumulate]")
{
  REQUIRE( 1 == 1);
  std::optional<int> var;

  std::vector<int> values;
  uint32_t number_of_values = 1000;
  values.reserve(number_of_values);

  for (uint32_t i = 0; i < number_of_values; ++i)
  {
    values.push_back(1);
  }

  auto result = parallelAccumulate(std::begin(values), std::end(values), 0);

  REQUIRE(result == number_of_values);
}
