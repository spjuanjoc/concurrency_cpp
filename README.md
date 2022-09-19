# Concurrency cpp

Some examples of how to use `std::thread` and `std::async`, `std::mutex`
with `std::lock_guard`, create a safe `std::list`, break a `lock_guard` by
passing a malicious function.

Create a map of thread names to have named ids for each thread.

Create a basic thread pool, then an awaitable thread pool using a safe queue.

Concurrency != parallelism

## Requires

These projects use Conan to handle the dependencies.

Conan version > `1.51.1`

The libraries used include:

- fmt
- catch2
- libsigcpp

## Compile

```shell
cmake -DCMAKE_MODULE_PATH=$PWD -DCMAKE_BUILD_TYPE=Debug -G "Ninja" ..
cmake --build .
```
