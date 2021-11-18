#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ostream.h>

#include <algorithm>
#include <chrono>
#include <future>
//#include <iostream>
#include <random>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

std::string getShortThreadId()
{
  const auto id       = std::this_thread::get_id();  // requires fmt/ostream.h
  const auto f_id     = fmt::format("{}", id);
  auto       short_id = f_id.substr(f_id.size() - 5);

  return short_id;
}

template<typename Type>
constexpr auto toUnderlyingType(Type enumerator) noexcept
{
  return static_cast<std::underlying_type_t<Type>>(enumerator);
}

template<typename Type>
constexpr auto  // fmt::color
toFmtColor(Type underlying_type_value)
{
  return static_cast<fmt::color>(underlying_type_value);
}

void doAsyncWork()
{
  std::random_device                      random_device;
  std::mt19937                            generator(random_device());
  std::uniform_int_distribution<uint32_t> distribution(toUnderlyingType(fmt::color::red),
                                                       toUnderlyingType(fmt::color::blue));

  auto generated = distribution(generator);
  auto color     = toFmtColor(generated);

  fmt::print(fg(color),
             "Doing async work for thread {}",
             fmt::format(fmt::emphasis::bold, "{}\n", getShortThreadId()));
  std::this_thread::sleep_for(10us);
  fmt::print(fg(color),
             "Async work done  for thread {}",
             fmt::format(fmt::emphasis::bold, "{}\n", getShortThreadId()));
}

void spawnAsync(uint32_t n)
{
  auto lambda = [=]() { doAsyncWork(); };

  std::vector<std::future<void>> futures;
  futures.reserve(n);

  for (int i = 1; i <= n; ++i)
  {
    futures.emplace_back(std::async(std::launch::async | std::launch::deferred, lambda));
  }

  //run them
  auto start{std::chrono::high_resolution_clock::now()};

  for (auto&& it : futures)
  {
    it.get();
  }

  auto       end{std::chrono::high_resolution_clock::now()};
  const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  fmt::print("time used in range-based for loop: {} us\n", elapsed.count());
}

int main()
{
  fmt::print("Spawned vector of std::future\n");

  auto number = std::thread::hardware_concurrency();
  auto m      = std::max<uint32_t>(1, number - 1);
  fmt::print("Threads available: {}\n", m);

  spawnAsync(m);

  fmt::print("End");

  return 0;
}
