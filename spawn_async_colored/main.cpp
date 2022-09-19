/**
 * @brief Creates a collection of asynchronous tasks to log a message in a colored output.
 *
 * @author spjuanjoc
 * @date   2021-11-17
 */

#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ostream.h>

#include <algorithm>
#include <chrono>
#include <future>
#include <random>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

constexpr std::chrono::microseconds TASK_SLEEP_TIME = 10us;

void
runTasks(std::vector<std::future<void>>& tasks);

std::vector<std::future<void>>
spawnAsyncTasks(uint32_t available_threads);

std::string
getShortThreadId()
{
  const auto thread_id    = std::this_thread::get_id();  // requires fmt/ostream.h
  const auto formatted_id = fmt::format("{}", thread_id);
  auto       short_id     = formatted_id.substr(formatted_id.size() - 5);

  return short_id;
}

template <typename Type>
constexpr auto
toUnderlyingType(Type enumerator) noexcept
{
  return static_cast<std::underlying_type_t<Type>>(enumerator);
}

template <typename Type>
constexpr auto  // fmt::color
toFmtColor(Type underlying_type_value)
{
  return static_cast<fmt::color>(underlying_type_value);
}

void
doAsyncWork()
{
  std::random_device                      random_device;
  std::mt19937                            generator(random_device());
  std::uniform_int_distribution<uint32_t> distribution(
    toUnderlyingType(fmt::color::red),
    toUnderlyingType(fmt::color::blue));

  const auto colors_distribution = distribution(generator);
  const auto text_color          = toFmtColor(colors_distribution);

  fmt::print(
    fg(text_color),
    "Doing async work for thread {}",
    fmt::format(fmt::emphasis::bold, "{}\n", getShortThreadId()));

  std::this_thread::sleep_for(TASK_SLEEP_TIME);  // The async work.

  fmt::print(
    fg(text_color),
    "Async work done  for thread {}",
    fmt::format(fmt::emphasis::bold, "{}\n", getShortThreadId()));
}

void
runTasks(std::vector<std::future<void>>& tasks)
{
  for (auto&& task : tasks)
  {
    task.get();
  }
}

std::vector<std::future<void>>
spawnAsyncTasks(uint32_t available_threads)
{
  std::vector<std::future<void>> tasks;
  auto                           task = [=]() { doAsyncWork(); };

  tasks.reserve(available_threads);

  for (int i = 1; i <= available_threads; ++i)
  {
    tasks.emplace_back(std::async(std::launch::async | std::launch::deferred, task));
  }

  return tasks;
}

int
main()
{
  fmt::print("Spawn async colored: A collection of std::future\n");

  const auto hardware_threads  = std::thread::hardware_concurrency();
  const auto available_threads = std::max<uint32_t>(1, hardware_threads - 1);

  fmt::print("Threads available: {}\n", available_threads);

  auto tasks = spawnAsyncTasks(available_threads);

  auto start{ std::chrono::high_resolution_clock::now() };
  runTasks(tasks);
  auto end{ std::chrono::high_resolution_clock::now() };

  const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  fmt::print("Time used running the tasks: {} us\n", elapsed.count());

  fmt::print("End");

  return 0;
}
