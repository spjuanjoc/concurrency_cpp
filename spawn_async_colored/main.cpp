/**
 * @brief Creates a collection of asynchronous tasks to log a message in a colored output.
 *
 * @author spjuanjoc
 * @date   2021-11-17
 */

#include <fmt/color.h>
#include <fmt/std.h>

#include <chrono>
#include <future>
#include <random>
#include <thread>
#include <utility>
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
  const auto& thread_id    = std::this_thread::get_id();  // requires fmt/std.h
  const auto  formatted_id = fmt::format("{}", thread_id);
  std::string short_id     = formatted_id.substr(formatted_id.size() - 5);

  return short_id;
}

// template <typename Type>
// constexpr auto
// toUnderlyingType(Type enumerator) noexcept
// {
//   return static_cast<std::underlying_type_t<Type>>(enumerator);
// }

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
  // std::mt19937                            generator(random_device());
  std::uniform_int_distribution<uint32_t> distribution(
    std::to_underlying(fmt::color::red),
    std::to_underlying(fmt::color::blue));

  const auto colors_distribution = distribution(random_device);
  const auto text_color          = toFmtColor(colors_distribution);

  fmt::print(
    fg(text_color),
    "Doing async work for thread {}\n",
    fmt::format(fmt::emphasis::bold, "{}", getShortThreadId()));

  std::this_thread::sleep_for(TASK_SLEEP_TIME);  // The async work.

  fmt::print(
    fg(text_color),
    "Async work done  for thread {}\n",
    fmt::format(fmt::emphasis::bold, "{}", getShortThreadId()));
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
spawnAsyncTasks(std::uint32_t available_threads)
{
  std::vector<std::future<void>> tasks;

  tasks.reserve(available_threads);

  for (std::size_t i = 1; i <= available_threads; ++i)
  {
    tasks.emplace_back(std::async(std::launch::async | std::launch::deferred, &doAsyncWork));
  }

  return tasks;
}

int
main()
{
  fmt::println("Spawn async colored: A collection of std::future");

  const auto hardware_threads  = std::thread::hardware_concurrency();
  const auto available_threads = std::max<uint32_t>(1, hardware_threads - 1);

  fmt::println("Threads available: {}", available_threads);

  auto tasks = spawnAsyncTasks(available_threads);

  auto start { std::chrono::high_resolution_clock::now() };
  runTasks(tasks);
  auto end { std::chrono::high_resolution_clock::now() };

  const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  fmt::println("Time used running the tasks: {} us", elapsed.count());

  fmt::println("End");

  return 0;
}
