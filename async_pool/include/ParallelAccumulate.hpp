//
// Created by juan.castellanos on 23/04/21.
//
#pragma once
#include "ThreadPoolHelper.hpp"
#include "WaitableThreadPool.hpp"
#include <future>

using namespace tpns;

template<typename Iterator, typename T>
struct AccumulateBlock
{
  T operator()(Iterator first, Iterator last) { return std::accumulate(first, last, T()); }
};

template<typename Iterator, typename T>
T parallelAccumulate(Iterator first, Iterator last, T init)
{
  const unsigned long length = std::distance(first, last);

  if (!length)
    return init;

  constexpr unsigned long      block_size = 25;
  const unsigned long          num_blocks = (length + block_size - 1) / block_size;
  std::vector<std::future<T> > futures(num_blocks - 1);
  WaitableThreadPool           pool;
  Iterator                     block_start = first;

  for (unsigned long i = 0; i < (num_blocks - 1); ++i)
  {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);

    futures[i]
      = pool.addToQueue([=]() { return AccumulateBlock<Iterator, T>()(block_start, block_end); });

    block_start = block_end;
  }

  T last_result = AccumulateBlock<Iterator, T>()(block_start, last);
  T result      = init;

  for (unsigned long i = 0; i < (num_blocks - 1); ++i)
  {
    result += futures[i].get();
  }

  result += last_result;

  return result;
}