/**
 * @brief
 *
 * @author  spjuanjoc
 * @date    2021-04-26
 */

#ifndef SAFE_LIST_HPP
#define SAFE_LIST_HPP

#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

template <typename T>
class SafeList
{
public:
  void add(T new_value)
  {
    std::lock_guard guard(m_mutex);

    fmt::print("Adding {} to list\n", new_value);
    std::this_thread::sleep_for(5ms);
    m_values.push_back(new_value);
  }

  bool contains(T value_to_find)
  {
    std::lock_guard<std::mutex> guard(m_mutex);

    fmt::print("Checking if list contains {}\n", value_to_find);

    return std::find(std::begin(m_values), std::end(m_values), value_to_find) != m_values.end();
  }

  void print() const
  {
    fmt::print("All the values: ");

    for (const auto& it : m_values)
      fmt::print("{} ", it);

    fmt::print("\n");
  }

private:
  std::mutex   m_mutex;
  std::list<T> m_values;
};

#endif  //
