/**
* @brief
*
* @author  spjuanjoc
* @date    2021-04-21
*/

#ifndef ASYNC_POOL_H
#define ASYNC_POOL_H

#include <fmt/std.h>

#include <cstdint>
#include <future>
#include <queue>

class IAsyncPool
{
};

class AsyncPool : IAsyncPool
{
public:
  using work_item_t = std::function<void(void)>;

  explicit AsyncPool(std::uint32_t max_threads)
  : max_threads_(max_threads)
  {
    if (max_threads_ > std::thread::hardware_concurrency())
    {
      //      throw error?
    }
  }

  virtual ~AsyncPool()
  {
    fmt::print("Finish\n");
    // Release ?
    //    for (auto&& it : vector_)
    //    {
    //      it.get();
    //    }
  }

  AsyncPool(const AsyncPool&)            = delete;
  AsyncPool(AsyncPool&&)                 = delete;
  AsyncPool& operator=(const AsyncPool&) = delete;
  AsyncPool& operator=(AsyncPool&&)      = delete;

  void queueWork(const work_item_t& item)
  {
    std::lock_guard lock(mutex_);
    fmt::print("Work queued\n");
    queue_.push(item);
    // notify ?
  }

  void run()
  {
    fmt::print("Run\n");
    auto lambda_work_item = [this]()
    {
      doAsyncWork();
    };
    std::promise<void> runner_started;
    auto               runnerValue = runner_started.get_future();

    for (uint32_t i = 1; i <= max_threads_; ++i)
    {
      std::future<void> result;
      result  = std::async(std::launch::deferred /*| std::launch::async*/,
                           lambda_work_item/*,
                          std::move(runner_started)*/);

      vector_.emplace_back(std::move(result));
    }

    fmt::print("async pool size: {}\n", vector_.size());

    while (!queue_.empty() | !stop_)
    {
      fmt::print("running\n");
      for (auto&& async_item : vector_)
      {
        fmt::print("running item\n");
        if (async_item.valid())
        {
          async_item.get();
        }
      }
    }
  }

  void stop() { stop_ = true; }

protected:
  AsyncPool() { fmt::print("Init the pool\n"); };

  void doAsyncWork()
  {
    std::lock_guard lock(mutex_);
    fmt::print("About to do some async work in thread {}, yay!\n", std::this_thread::get_id());

    if (auto front = queue_.front())
    {
      fmt::print("There is work to do\n");
      front();
      queue_.pop();
    }
    else
    {
      fmt::print("Nothing to do here, keep moving\n");
    }
  }

private:
  std::uint32_t                  max_threads_ { 1 };
  std::queue<work_item_t>        queue_;
  std::vector<std::future<void>> vector_;
  bool                           stop_ { false };
  std::mutex                     mutex_;
};

#endif  //ASYNC_POOL_H
