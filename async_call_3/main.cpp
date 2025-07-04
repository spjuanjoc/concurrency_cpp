#include <fmt/core.h>

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


void
regularFutures()
{
  const vector<double> numbers = { 0, 1.1, 2.2, 3.3, 4.4, 5.5 };

  auto lambda = [=](const double factor)
  {
    double result = 0;

    for (const auto& number : numbers)
    {
      result += number * number * factor;
    }

    return result;
  };

  vector<future<double>> futures;

  for (double i : { 1, 2, 3, 4, 5, 6, 7, 8, 9 })
  {
    futures.push_back(std::async(lambda, i));
  }

  for_each(
    futures.begin(),
    futures.end(),
    [](future<double>& x)
    {
      fmt::print("{} ", x.get());
    });

  fmt::print("\n");
}

// option 2 : store shared_futures which allow passing copies
void
sharedFutures()
{
  const vector<double> numbers = { 0, 1.1, 2.2, 3.3, 4.4, 5.5 };

  auto lambda = [=](const double factor)
  {
    double result = 0;

    for (const auto& number : numbers)
    {
      result += number * number * factor;
    }

    return result;
  };

  vector<std::shared_future<double>> futures;

  for (double i : { 1, 2, 3, 4, 5, 6, 7, 8, 9 })
  {
    futures.emplace_back(std::async(lambda, i));
  }

  for_each(
    futures.begin(),
    futures.end(),
    [](const std::shared_future<double>& x)
    {
      fmt::print("{} ", x.get());
    });
  fmt::print("\n");
}


void
doAsyncWork()
{
  fmt::print("Doing async work");
}

int
main()
{
  fmt::print("Shared futures spawn\n");
  const auto number_of_threads = std::thread::hardware_concurrency();
  const auto available_threads = std::max<uint32_t>(1, number_of_threads - 1);
  fmt::print("Threads available: {}\n", available_threads);

  regularFutures();
  sharedFutures();

  {
    std::thread thread_1(doAsyncWork);  //thread-based
    thread_1.join();
  }

  std::future<void> future_1 = std::async(
    std::launch::async | std::launch::deferred,
    doAsyncWork);  //task-based is preferred to thread-based
  future_1.get();

  fmt::print("End");

  return 0;
}
