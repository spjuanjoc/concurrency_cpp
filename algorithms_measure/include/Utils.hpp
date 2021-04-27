//
// Created by juan.castellanos on 24/04/21.
//
#pragma once

#include "fmt/core.h"
#include <chrono>

using std::chrono::duration;
using std::chrono::high_resolution_clock;
using dur_ratio = std::milli;

// Decorator as a running function template
template<typename TFunc>
void runAndMeasure(std::string_view title, TFunc func)
{
  const auto start           = high_resolution_clock::now();
  auto       function_return = func();
  const auto end             = high_resolution_clock::now();
  const auto elapsed         = duration<double, dur_ratio>(end - start);
  fmt::print("{} : time: {} ms, result: {} \n", title, elapsed.count(), function_return);
}

// Decorator as a template functor, needs a helper function
template<typename Func>
struct DecoratorTemplate
{
  Func             func_;
  std::string_view name_;

  DecoratorTemplate(const Func& func, const std::string_view name) : func_(func), name_(name) {}

  void operator()() const
  {
    const auto start           = high_resolution_clock::now();
    auto       function_return = func_();
    const auto end             = high_resolution_clock::now();
    const auto elapsed         = duration<double, dur_ratio>(end - start);
    fmt::print("{} : time: {} ms, result: {} \n", name_, elapsed.count(), function_return);
  }
};

// The helper function
template<typename Func>
auto measure(const std::string_view name, Func func)
{
  return DecoratorTemplate<Func>{func, name};  // To infer the type
};
