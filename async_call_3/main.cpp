#include "fmt/core.h"
#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

using std::cout;
using std::endl;
using std::future;
using std::vector;
using namespace std::chrono_literals;

void test1()
{
  vector<double> numbers = {0, 1.1, 2.2, 3.3, 4.4, 5.5};

  auto lambda = [=](double factor)
  {
    double result = 0;

    for (const auto& number : numbers)
    {
      result += number * number * factor;
    }

    return result;
  };

  vector<future<double>> futures;

  for (double i : {1, 2, 3, 4, 5, 6, 7, 8, 9})
  {
    futures.push_back(std::async(lambda, i));
  }

  for_each(futures.begin(), futures.end(), [](future<double>& x) { fmt::print("{} ", x.get()); });
  fmt::print("\n");
}

// option 2 : store shared_futures which allow passing copies
void test2()
{
  vector<double> numbers = {0, 1.1, 2.2, 3.3, 4.4, 5.5};

  auto lambda = [=](double factor)
  {
    double result = 0;

    for (const auto number : numbers)
    {
      result += number * number * factor;
    }

    return result;
  };
  vector<std::shared_future<double>> futures;

  for (double i : {1, 2, 3, 4, 5, 6, 7, 8, 9})
  {
    futures.emplace_back(std::async(lambda, i));
  }

  for_each(futures.begin(),
           futures.end(),
           [](const std::shared_future<double>& x) { fmt::print("{} ", x.get()); });
  fmt::print("\n");
}

void doAsyncWork()
{
  fmt::print("Doing async work\n");
}

void spawnAsync(int n)
{
  auto lambda = [=]() { doAsyncWork(); };

  std::vector<std::future<void>> futures;
  futures.reserve(n);

  for (int i = 1; i <= n; ++i)
  {
    futures.emplace_back(std::async(std::launch::deferred, lambda));
  }

  //run them
  auto startForRB{std::chrono::high_resolution_clock::now()};

  for (auto&& it : futures)
  {
    it.get();
  }

  auto       endForRB{std::chrono::high_resolution_clock::now()};
  auto       timeUsedRB{endForRB - startForRB};
  const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(timeUsedRB).count();
  fmt::print("time used in range-based for loop: {} us\n", elapsed);
}

int main()
{
  fmt::print("Shared futures\n");
  {
    std::thread t(doAsyncWork);  //thread-based
    t.join();
  }
  std::future<void> f = std::async(std::launch::async | std::launch::deferred,
                                   doAsyncWork);  //task-based is preferred to thread-based
  f.get();
  spawnAsync(2);
  test1();
  test2();

  return 0;
}
