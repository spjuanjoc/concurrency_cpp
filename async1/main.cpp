#include "fmt/ostream.h"
#include <fmt/core.h>
#include <chrono>
#include <future>
#include <iostream>

using namespace std::chrono_literals;
std::promise<void> runner_timeout;

void firstTask()
{
  constexpr auto sleep_time = 5s;
  fmt::print("First message, before sleep {}s {:^4} tid:{:<5}\n",
             sleep_time.count(),
             "",
             std::this_thread::get_id());
  std::this_thread::sleep_for(sleep_time);
  fmt::print("Second message, after sleep {}s {:^4} tid:{}\n",
             sleep_time.count(),
             "",
             std::this_thread::get_id());
}

void called_from_async(std::promise<void> started)
{
  constexpr auto sleep_time = 5s;
  constexpr auto timeout    = 5s;
  bool           needs_to_continue{true};

  fmt::print("Third message, before async call {}s tid:{}\n",
             sleep_time.count(),
             std::this_thread::get_id());
  std::this_thread::sleep_for(sleep_time);

  auto abort_value = runner_timeout.get_future();
  started.set_value();

  while (needs_to_continue)
  {
    if (abort_value.wait_for(timeout) == std::future_status::ready)
    {
      fmt::print("future ready\n");
      std::this_thread::sleep_for(sleep_time);
      needs_to_continue = false;
    }
    else if (abort_value.wait_for(timeout) == std::future_status::timeout)
    {
      fmt::print("future timed out\n");
      std::this_thread::sleep_for(sleep_time);
      needs_to_continue = false;
    }
    else
    {
      fmt::print("not ready nor timed out\n");
      std::this_thread::sleep_for(sleep_time);
    }
  }
}

int main()
{
  //called_from_async launched in a separate thread if possible
  std::promise<void> runnerStarted;
  auto               runnerValue = runnerStarted.get_future();
  std::future<void>  result;
  result = std::async(std::launch::async, called_from_async, move(runnerStarted));

  firstTask();
  fmt::print("after async\n");
  runner_timeout.set_value();
  fmt::print("after set_value\n");
  //ensure that called_from_async is launched synchronously if it wasn't already launched
  constexpr auto timeout = 25s;

  if (result.valid())  //    while (result.valid())
  {
    if (result.wait_for(timeout) == std::future_status::ready)
    {
      fmt::print("async ready\n");
    }
    else if (result.wait_for(timeout) == std::future_status::timeout)
    {
      fmt::print("async timed out\n");
    }
    else
    {
      fmt::print("async task done\n");
    }
  }
  return 0;
}
