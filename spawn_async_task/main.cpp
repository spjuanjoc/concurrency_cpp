#include "fmt/core.h"
#include <chrono>
#include <future>
#include <vector>

void doAsyncWork()
{
  fmt::print("Doing async work\n");
}

/**
 * Spawns number_of_threads threads
 */
void spawnAsync(int number_of_threads)
{
  auto lambda = [=]() { doAsyncWork(); };

  std::vector<std::future<void>> futures;
  futures.reserve(number_of_threads);

  for (int i = 1; i <= number_of_threads; ++i)
  {
    futures.push_back(std::async(std::launch::deferred, lambda));
  }

  //run them
  auto startForRB{std::chrono::high_resolution_clock::now()};

  for (auto&& it : futures)
  {
    it.get();
  }

  auto endForRB{std::chrono::high_resolution_clock::now()};
  auto timeUsedRB{endForRB - startForRB};
  const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(timeUsedRB).count();
  fmt::print("time used in range-based for loop: {} us\n", elapsed);
}

int main()
{
  fmt::print("Spawn async tasks\n");
  spawnAsync(10);
}
