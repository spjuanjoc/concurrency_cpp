/**
 * @brief Defines the screen boxes.
 *
 * @author  spjuanjoc
 * @date    2022-04-22
 */

#ifndef THREADPOOLHELPER_H
#define THREADPOOLHELPER_H

#include <queue>
#include <thread>

#include <condition_variable>

namespace tpns
{

template <typename T>
class ThreadSafeQueue
{
private:
  mutable std::mutex      mutex_;
  std::queue<T>           data_queue_;
  std::condition_variable data_condition_v_;

public:
  ThreadSafeQueue() = default;

  void push(T new_value)
  {
    std::lock_guard lock(mutex_);
    data_queue_.push(std::move(new_value));
    data_condition_v_.notify_one();
  }

  void waitAndPop(T& value)
  {
    std::unique_lock lock(mutex_);
    data_condition_v_.wait(
      lock,
      [this]
      {
        return !data_queue_.empty();
      });
    value = std::move(data_queue_.front());
    data_queue_.pop();
  }

  std::shared_ptr<T> waitAndPop()
  {
    std::unique_lock lock(mutex_);
    data_condition_v_.wait(
      lock,
      [this]
      {
        return !data_queue_.empty();
      });
    std::shared_ptr<T> result = std::make_shared<T>(std::move(data_queue_.front()));
    data_queue_.pop();

    return result;
  }

  bool tryPop(T& value)
  {
    std::lock_guard lock(mutex_);
    bool            result = false;

    if (!data_queue_.empty())
    {
      value = std::move(data_queue_.front());
      data_queue_.pop();
      result = true;
    }

    return result;
  }

  std::shared_ptr<T> tryPop()
  {
    std::lock_guard    lock(mutex_);
    std::shared_ptr<T> result {};

    if (!data_queue_.empty())
    {
      result = std::make_shared<T>(std::move(data_queue_.front()));
      data_queue_.pop();
    }

    return result;
  }

  bool isEmpty() const
  {
    std::lock_guard lock(mutex_);
    return data_queue_.empty();
  }
};

class ThreadsJoiner
{
  std::vector<std::thread>& threads_;

public:
  explicit ThreadsJoiner(std::vector<std::thread>& threads)
  : threads_(threads)
  {
  }

  ~ThreadsJoiner()
  {
    for (auto& thread : threads_)
    {
      if (thread.joinable())
        thread.join();
    }
  }
};

class FunctionWrapper
{
  struct ImplBase
  {
    virtual void call() = 0;
    virtual ~ImplBase() = default;
  };
  std::unique_ptr<ImplBase> impl_;

  template <typename F>
  struct ImplType : ImplBase
  {
    F f;
    explicit ImplType(F&& func)
    : f(std::move(func))
    {
    }
    void call() override { f(); }
  };

public:
  template <typename F>
  FunctionWrapper(F&& f)
  : impl_(new ImplType<F>(std::forward<F>(f)))  //! No explicit
  {
  }

  void operator()() { impl_->call(); }

  FunctionWrapper() = default;

  FunctionWrapper(FunctionWrapper&& other) noexcept
  : impl_(std::move(other.impl_))
  {
  }

  FunctionWrapper& operator=(FunctionWrapper&& other) noexcept
  {
    impl_ = std::move(other.impl_);
    return *this;
  }

  FunctionWrapper(const FunctionWrapper&)            = delete;
  FunctionWrapper(FunctionWrapper&)                  = delete;
  FunctionWrapper& operator=(const FunctionWrapper&) = delete;
};

}  // namespace tpns

#endif  //THREADPOOLHELPER_H
