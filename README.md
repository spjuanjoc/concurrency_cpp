# Concurrency cpp

Some examples of how to use std::thread and std::async, std::mutex
with std::lock_guard, create a safe std::list, break a lock_guard
by passing a malicious function.

Create a map of thread names to have named ids for each thread.

Create a basic thread pool, then an awaitable thread pool using a 
safe queue.

Concurrency != parallelism
