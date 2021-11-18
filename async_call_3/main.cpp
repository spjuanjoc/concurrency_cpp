#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

using std::cout;
using std::endl;
using std::future;
using std::vector;
using namespace std::chrono_literals;


void regularFutures()
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
void sharedFutures()
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
  fmt::print("Doing async work");
}


int main()
{
  fmt::print("Shared futures spawn\n");
  auto number = std::thread::hardware_concurrency();
  auto m      = std::max<uint32_t>(1, number - 1);
  fmt::print("Threads available: {}\n", m);

  regularFutures();
  sharedFutures();

  {
    std::thread t(doAsyncWork);  //thread-based
    t.join();
  }

  std::future<void> f = std::async(std::launch::async | std::launch::deferred,
                                   doAsyncWork);  //task-based is preferred to thread-based
  f.get();

  fmt::print("End");

  return 0;
}
