#include <fmt/core.h>

#include <iostream>
#include <thread>
#include <vector>

void
doSomething(int id)
{
  fmt::print("ID: {}\n", id);
}

/**
 * Spawns n threads
 */
void
spawnThreads(std::uint32_t number_of_threads)
{
  std::vector<std::thread> threads;
  threads.reserve(number_of_threads);

  // spawn n threads:
  for (std::size_t i = 0; i < number_of_threads; ++i)
  {
    threads.emplace_back(doSomething, i + 1);
  }

  for (auto& it : threads)
  {
    it.join();
  }
}

int
main()
{
  const auto number_of_threads = std::thread::hardware_concurrency() - 1;
  spawnThreads(number_of_threads);
}
