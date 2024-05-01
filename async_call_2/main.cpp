#include <fmt/core.h>

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

void
firstTask()
{
  fmt::print("First message before sleep.\n");
  std::this_thread::sleep_for(2s);
  fmt::print("Second message after sleep.\n");
}

void
called_from_async(std::promise<void> started)
{
  fmt::print("called_from_async init.\n");
  std::this_thread::sleep_for(5s);
  fmt::print("called_from_async end.\n");
}

int
main()
{
  std::promise<void> runnerStarted;
  std::future<void>  runnerValue = runnerStarted.get_future();
  std::future<void>  result;

  result = std::async(std::launch::async | std::launch::deferred, called_from_async, std::move(runnerStarted));

  constexpr auto timeout = 10s;

  if (result.valid())
  {
    if (result.wait_for(timeout) == std::future_status::ready)
    {
      fmt::print("ready\n");
    }

    if (result.wait_for(timeout) == std::future_status::timeout)
    {
      fmt::print("timeout\n");
    }
    else
    {
      fmt::print("Task done\n");
    }
  }

  firstTask();

  return 0;
}
