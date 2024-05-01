/**
 * @brief
 *
 * @author  spjuanjoc
 * @date    2020-01-24
 */

#ifndef UNPROTECTED_MUTEX_H
#define UNPROTECTED_MUTEX_H

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
  std::int32_t value {};
  std::string  name { "name" };
};

class DataWrapper
{
public:
  template <typename Function>
  void processData(Function func)
  {
    std::lock_guard lock(m_mutex);
    fmt::print("Process data inside lock_guard\n");
    func(m_data);  // Pass “protected” data to user-supplied function, but it's not really protected
  }

private:
  SomeData   m_data;
  std::mutex m_mutex;
};

SomeData* unprotected_data;

void
maliciousFunction(SomeData& protected_data)
{
  fmt::print("Malicious function. Data's not thread safe\n");
  unprotected_data = &protected_data;
}

#endif //
