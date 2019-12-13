#include <chrono>
#include <future>
#include <iostream>

//using namespace std::chrono_literals;

void firstTask()
{
    std::cout << "First message before sleep." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(15));
    std::cout << "Second message after sleep." << std::endl;
}
void called_from_async(std::promise<void> started)
{
    std::cout << "called_from_async init" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "called_from_async end" << std::endl;
}

int main()
{
    std::promise<void> runnerStarted;
    auto runnerValue = runnerStarted.get_future();
    std::future<void> result;
    result = std::async(
                 std::launch::async,
                 called_from_async,
                 move(runnerStarted));
    auto timeout = std::chrono::seconds(5);

    if(result.valid() )
    {
        if(result.wait_for(timeout) == std::future_status::ready)
        {
            std::cout << "ready" << std::endl;
        }

        if(result.wait_for(timeout) == std::future_status::timeout)
        {
            std::cout << "timeout" << std::endl;
            //
        }
        else
        {
            std::cout << "Task done" << std::endl;
        }
    }

    return 0;
}
