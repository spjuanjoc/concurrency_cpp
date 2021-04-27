//
// Created by juan.castellanos on 24/01/20.
//
#pragma once

#include <mutex>
#include <string>

class SomeData
{
public:
  void doSomething()
  {
    fmt::print("Do something with {} {}\n", value, name);
    ++value;
    name.append(std::to_string(value));
    fmt::print("Updated {} {}\n", value, name);
  };

private:
  std::int32_t value{};
  std::string  name{"name"};
};

class DataWrapper
{
public:
  template<typename Function>
  void processData(Function func)
  {
    std::lock_guard lock(mutex_);
    fmt::print("Process data inside lock_guard\n");
    func(data_);  // Pass “protected” data to user-supplied function, but it's not really protected
  }

private:
  SomeData   data_;
  std::mutex mutex_;
};

SomeData* unprotected_data;

void maliciousFunction(SomeData& protected_data)
{
  fmt::print("Malicious function. Data's not thread safe\n");
  unprotected_data = &protected_data;
}
