//#include "AsyncPool.h"
#include "fmt/core.h"
#include "SimpleThreadPool.hpp"
#include "WaitableThreadPool.hpp"
#include "Utils/TimeUtils.h"
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using std::cout;
using std::endl;
using std::fstream;
using std::future;
using std::lock_guard;
using std::mutex;
using std::string;
using std::vector;

using namespace Utils;

//void runAsyncPool()
//{
//  AsyncPool pool(3 /*std::thread::hardware_concurrency()*/);
//
//  for (auto i = 0; i < 10; ++i)
//  {
//    pool.queueWork([i = i]() { fmt::print("Do something for {}\n", i); });
//  }
//
//  pool.run();
//  std::this_thread::sleep_for(5s);
//  pool.stop();
//}

void runSimpleThreadPool()
{
  tpns::SimpleThreadPool pool1;
  tpns::SimpleThreadPool pool2;

  for (int i = 0; i < 10; ++i)
  {
    pool1.addToQueue([i = i](){ fmt::print("Some work {}\n", i);});
    pool2.addToQueue([i = i](){ fmt::print("Some other work {}\n", 10-i);});
    std::this_thread::sleep_for(1s);
  }
}

void runWaitableThreadPool()
{
  tpns::WaitableThreadPool pool3;

  for (int i = 0; i < 10; ++i)
  {
    auto result = pool3.addToQueue(
      [i = i]()
      {
        fmt::print("Some work {}\n", i);
        return 1;
      });
    fmt::print("Result: {}\n", result.get());

    std::this_thread::sleep_for(200ms);
  }
}

int main()
{
  const auto start       = count();
  const auto max_threads = std::thread::hardware_concurrency();
  fmt::print("Max threads supported {}\n", max_threads);

//  runSimpleThreadPool();
  runWaitableThreadPool();

  const auto end = count();
  fmt::print("Total elapsed time {} ms\n", elapsedTime(start, end));

  return 0;
}
