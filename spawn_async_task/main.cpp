#include <fmt/core.h>

#include <chrono>
#include <future>
#include <vector>

void
doAsyncWork()
{
  fmt::print("Doing async work\n");
}

/**
 * Spawns number_of_threads threads
 */
void
spawnAsync(std::uint32_t number_of_threads)
{
  std::vector<std::future<void>> futures;

  futures.reserve(number_of_threads);

  for (std::size_t i = 1; i <= number_of_threads; ++i)
  {
    futures.emplace_back(std::async(std::launch::deferred, &doAsyncWork));
  }

  //run them
  auto start { std::chrono::high_resolution_clock::now() };

  for (auto&& future : futures)
  {
    future.get();
  }

  auto       end { std::chrono::high_resolution_clock::now() };
  const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  fmt::print("time used in range-based for loop: {} us\n", elapsed);
}

int
main()
{
  fmt::print("Spawn async tasks\n");
  spawnAsync(10);
}
