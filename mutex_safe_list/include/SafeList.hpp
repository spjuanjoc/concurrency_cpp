//
// Created by juan.castellanos on 26/04/21.
//
#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

template<typename T>
class SafeList
{
public:
  void add(T new_value)
  {
    std::lock_guard guard(mutex_);
    fmt::print("Adding {} to list\n", new_value);
    std::this_thread::sleep_for(5ms);
    list_.push_back(new_value);
  }

  bool contains(T value_to_find)
  {
    std::lock_guard<std::mutex> guard(mutex_);
    fmt::print("Checking if list contains {}\n", value_to_find);
    return std::find(std::begin(list_), std::end(list_), value_to_find) != list_.end();
  }

  void print() const
  {
    fmt::print("All the values: ");

    for (const auto& it : list_)
    {
      fmt::print("{} ", it);
    }
    fmt::print("\n");
  }

private:
  std::mutex   mutex_;
  std::list<T> list_;
};
