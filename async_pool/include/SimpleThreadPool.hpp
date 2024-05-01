/**
 * @brief
 *
 * @author  spjuanjoc
 * @date    2021-04-21
 */

#ifndef SIMPLE_THREADPOOL_H
#define SIMPLE_THREADPOOL_H

#include "ThreadPoolHelper.hpp"

namespace tpns
{

class SimpleThreadPool
{
public:
  SimpleThreadPool()
  : done_(false)
  , joiner_(threads_)
  {
    fmt::print("ctor\n");
    const std::uint32_t thread_count = std::max(2u, 1u);
    //    const std::uint32_t thread_count = std::max(std::thread::hardware_concurrency() - 1, 1u);

    try
    {
      for (unsigned i = 0; i < thread_count; ++i)
      {
        threads_.emplace_back(&SimpleThreadPool::runWorkerThread, this);
      }
    }
    catch (...)
    {
      done_ = true;
      throw;  // is it ok to throw from ctor?
    }
    fmt::print("ctor done\n");
  }

  ~SimpleThreadPool()
  {
    fmt::print("dtor\n");
    done_ = true;
  }

  template <typename FunctionType>
  void addToQueue(FunctionType f)
  {
    fmt::print("Add to queue\n");
    work_queue_.push(std::function<void()>(f));
  }

private:
  //! Strict order
  std::atomic_bool                       done_;
  ThreadSafeQueue<std::function<void()>> work_queue_;
  std::vector<std::thread>               threads_;
  ThreadsJoiner                          joiner_;

  void runWorkerThread()
  {
    while (!done_)
    {
      std::function<void()> task;
      if (work_queue_.tryPop(task))
      {
        fmt::print("Doing work from tid:{}\n", std::this_thread::get_id());
        task();
        fmt::print("work done for tid:{}\n", std::this_thread::get_id());
      }
      else
      {
        //        fmt::print("idle tid:{}\n", std::this_thread::get_id());
        std::this_thread::yield();
      }
    }
  }
};

}  // namespace tpns

#endif  //SIMPLE_THREADPOOL_H
