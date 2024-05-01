#include <fmt/std.h>  // for << std::thread::get_id()

#include <chrono>
#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

using namespace std::literals::chrono_literals;

std::mutex               g_mutex;
std::vector<std::string> g_ordered_lines;

void
writeFile(const std::string& line)
{
  std::lock_guard lock(g_mutex);

  fmt::print("do sequential work\n");
  std::ofstream file_stream { "output.txt", std::ios::app };

  if (file_stream)
  {
    for (const auto& it : line)
    {
      file_stream << it;
    }

    file_stream << '\n';

    file_stream.close();
  }
}

void
saveValues(const std::string& file_line)
{
  std::lock_guard lock(g_mutex);

  fmt::print("do sequential work\n");
  g_ordered_lines.emplace_back(file_line);  // race condition if not sequential
}

void
doAsyncWork(std::string file_line)
{
  fmt::print("Doing async work\n");

  auto lambda = [](unsigned char c)
  {
    bool result = false;
    if (isspace(c))
    {
      auto tid = std::this_thread::get_id();
      fmt::print("Space found. Thread [{}] paused\n", tid);
      std::this_thread::sleep_for(10ms);
      result = true;
    }
    return result;
  };

  file_line.erase(std::remove_if(file_line.begin(), file_line.end(), lambda), file_line.end());

  std::sort(file_line.begin(), file_line.end());
  //do other stuff like sort
  saveValues(file_line);
}

/**
 * Spawns one thread per line to the number_of_threads
 * @param file
 * @param number_of_threads
 */
void
spawnAsync(std::ifstream& file, const std::uint32_t& number_of_threads)
{
  std::string                    line;
  std::vector<std::future<void>> futures;
  futures.reserve(number_of_threads);

  auto lambda = [](std::string s)
  {
    doAsyncWork(std::move(s));
  };

  for (std::size_t i = 1; i <= number_of_threads; ++i)
  {
    if (!file.eof())
    {
      std::getline(file, line);
      fmt::print("line: {}\n", line);

      //      futures.push_back(std::async(std::launch::deferred | std::launch::async, lambda, line));
      futures.push_back(std::async(std::launch::deferred, lambda, line));
    }
  }

  //run them
  auto startForRB { std::chrono::high_resolution_clock::now() };
  for (auto&& it : futures)
  {
    it.get();
  }

  auto       endForRB { std::chrono::high_resolution_clock::now() };
  auto       timeUsedRB { endForRB - startForRB };
  const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(timeUsedRB).count();
  fmt::print("Elapsed: {} us\n", elapsed);
}

int main(/*int argc, char* argv[]*/)
{
  fmt::print("Async write file, read from a file and order its chars\n");

  std::ifstream input_stream;
  const auto    number_of_threads = std::thread::hardware_concurrency() - 1;

  input_stream.open("input.txt");

  if (input_stream)
  {
    while (!input_stream.eof())
    {
      spawnAsync(input_stream, number_of_threads);
    }

    input_stream.close();

    for (const auto& line : g_ordered_lines)
    {
      writeFile(line);
    }
  }
  else
  {
    std::cerr << "Error reading file\n";

    return 1;
  }

  return 0;
}
