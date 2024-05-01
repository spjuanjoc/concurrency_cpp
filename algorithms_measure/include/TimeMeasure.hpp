/**
 * @brief Decorators to measure the time spent running a callable function.
 *
 * @author spjuanjoc
 * @date   2021-04-24
 */

#ifndef TIMEMEASURE_H
#define TIMEMEASURE_H

#include "fmt/core.h"

#include <chrono>

using std::chrono::duration;
using std::chrono::high_resolution_clock;
using duration_ratio = std::milli;

namespace TimeMeasure
{

/**
 * @brief Decorator as a running function template
 *
 * @tparam Callable The type of the callable.
 *
 * @param title    The title of the callable to show in console.
 * @param callable The callable.
 */
template <typename Callable>
void
runAndMeasure(std::string_view title, Callable callable)
{
  const auto start           = high_resolution_clock::now();
  auto       function_return = callable();
  const auto end             = high_resolution_clock::now();
  const auto elapsed         = duration<double, duration_ratio>(end - start);

  fmt::print("|{:<15}|{:<10}|{:<10}|\n", title, elapsed.count(), function_return);
}

/**
 * @brief Decorator as a template functor, needs a helper function.
 *
 * @tparam Callable The type of the callable.
 */
template <typename Callable>
class DecoratorTemplate
{
public:
  DecoratorTemplate(const Callable& callable, const std::string_view title)
  : m_callable(callable)
  , m_title(title)
  {
  }

  void operator()() const
  {
    const auto start           = high_resolution_clock::now();
    auto       function_return = m_callable();
    const auto end             = high_resolution_clock::now();
    const auto elapsed         = duration<double, duration_ratio>(end - start);

    fmt::print("|{:<15}|{:<10}|{:<10}|\n", m_title, elapsed.count(), function_return);
  }

private:
  Callable         m_callable;
  std::string_view m_title;
};

/**
 * @brief The helper template for the decorator to infer the type.
 *
 * @tparam Callable The type of the callable.
 *
 * @param title    The title of the callable to show in console.
 * @param callable The callable.
 *
 * @return The decorated functor.
 */
template <typename Callable>
auto
measure(const std::string_view title, Callable callable)
{
  return DecoratorTemplate<Callable> { callable, title };  // To infer the type
};

}  // namespace TimeMeasure

#endif  //TIMEMEASURE_H
