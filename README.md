# Concurrency cpp

Some examples of how to use `std::thread` and `std::async`, `std::mutex`
with `std::lock_guard`, create a safe `std::list`, break a `lock_guard` by
passing a malicious function.

Create a map of thread names to have named ids for each thread.

Create a basic thread pool, then an awaitable thread pool using a safe queue.

Concurrency is not exactly equals to parallelism.

## Concurrency

Two or more separate activities happening at the same time, but not necessarily
synced, maybe switching tasks

Multiple processors
Processor with multiple cores: hardware concurrency.

Two approaches to concurrency:

- Multiple single-threaded processes
- Multiple threads on a single process

Multiple threads on the same CPU. Maybe virtual threads, not physical threads.

## Parallelism

It is performance-oriented.
Take advantage of the available hardware.
Concurrency for performance: task parallelism

Split a single task into subtasks that can be executed at the same time, in
physical threads.

## Parallel execution

Making progress on more than one task with some synchronicity, not switching
tasks.

Multiple physical threads on different CPUs

## Parallel concurrent execution

Multiple virtual threads, and multiple CPUs.

## Threads

Logical threads and physical threads.
Logical for concurrency with task switching.
Physical for parallelism.

## Requires

These projects use Conan to handle the dependencies.

Conan version `1.62.0`

The libraries used include:

- fmt
- catch2
- libsigcpp

## Compile

Inside any of the subprojects:

```shell
mkdir build && cd build
cmake -DCMAKE_MODULE_PATH=$PWD -DCMAKE_BUILD_TYPE=Debug -G "Ninja" ..
cmake --build .
```
