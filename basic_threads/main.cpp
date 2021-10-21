/**
 * @brief
 *
 * @author  juan.castellanos
 * @date    2021-10-21
 */


#include <thread>
#include <fmt/core.h>

using namespace std::chrono_literals;
using secs = std::chrono::seconds;

void foo(int n)
{
  fmt::print("running thread {}\n", n);
  std::this_thread::sleep_for(secs(n));
  fmt::print("done thread {}\n", n);
}


int main()
{
  unsigned int n = std::thread::hardware_concurrency();

  fmt::print("threads available {}\n", n);

  std::thread t1 ( [](){ foo(2); });
  std::thread t2 ( [](){ foo(1); });

  t2.detach();
  t1.join();

  fmt::print("main\n");

  std::this_thread::sleep_for(2s);

  return 0;
}
