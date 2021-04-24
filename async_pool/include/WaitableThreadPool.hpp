//
// Created by juan.castellanos on 22/04/21.
//
#pragma once

#include "fmt/core.h"
#include "fmt/ostream.h"
#include "ThreadPoolHelper.hpp"
#include <atomic>
#include <future>

namespace tpns
{

class WaitableThreadPool
{
public:
  WaitableThreadPool() : done_(false), joiner_(threads_)
  {
    fmt::print("ctor\n");
//    const std::uint32_t thread_count = std::max(2u, 1u);
    const std::uint32_t thread_count = std::max(std::thread::hardware_concurrency() - 1, 1u);

    try
    {
      for (unsigned i = 0; i < thread_count; ++i)
      {
        threads_.emplace_back(&WaitableThreadPool::runWorkerThread, this);
      }
    }
    catch (...)
    {
      done_ = true;
      throw;  // is it ok to throw from ctor?
    }
    fmt::print("ctor done\n");
  }

  ~WaitableThreadPool()
  {
    fmt::print("dtor\n");
    done_ = true;
  }

  template<typename FunctionType>
  std::future<typename std::result_of<FunctionType()>::type>
  addToQueue(FunctionType task_function)
  {
    fmt::print("Add to queue\n");
    using result_type = typename std::result_of<FunctionType()>::type;
//    typedef typename std::result_of<FunctionType()>::type result_type;

    std::packaged_task<result_type()> task(std::move(task_function));
    std::future<result_type>          result(task.get_future());

    work_queue_.push(std::move(task));

    return result;
  }

private:
  //! Strict order
  std::atomic_bool                 done_;
  ThreadSafeQueue<FunctionWrapper> work_queue_;
  std::vector<std::thread>         threads_;
  ThreadsJoiner                    joiner_;

  void runWorkerThread()
  {
    while (!done_)
    {
      FunctionWrapper task;
      if (work_queue_.tryPop(task))
      {
        fmt::print("Doing work from tid:{}\n", std::this_thread::get_id());
        task();
        fmt::print("work done for tid:{}\n", std::this_thread::get_id());
      }
      else
      {
        std::this_thread::yield();
      }
    }
  }
};

}  // namespace tpns
