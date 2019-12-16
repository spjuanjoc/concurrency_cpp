#include <chrono>
#include <future>
#include <iostream>

//using namespace std::chrono_literals;
std::promise<void> runnerTimeout;
void firstTask()
{
    std::cout << "First message before sleep." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(15));
    std::cout << "Second message after sleep." << std::endl;
}
void called_from_async(std::promise<void> started)
{
    std::cout << "Third message Async call" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(20));
//    auto abortValue = runnerTimeout.get_future();
//    started.set_value();
//    bool needToContinue{true};
//    while(needToContinue)
//    {
//        if(abortValue.wait_for(std::chrono::seconds(5)) == std::future_status::timeout)
//        {
//            std::cout << "inside wait for" << std::endl;
//            needToContinue = false;
//        }
//        else
//        {
//            std::cout << "outside wait for" << std::endl;
//        }
//    }
}

int main()
{
    //called_from_async launched in a separate thread if possible
    std::promise<void> runnerStarted;
    auto runnerValue = runnerStarted.get_future();
    std::future<void> result;
    result = std::async(
                 std::launch::async,
                 called_from_async,
                 move(runnerStarted));
////    firstTask();
//    std::cout << "after async" << std::endl;
//    runnerTimeout.set_value();
//    std::cout << "after set_value" << std::endl;
    //ensure that called_from_async is launched synchronously if it wasn't already launched
    auto timeout = std::chrono::seconds(5);

//    while(result.valid() )
//    {
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

//    }
    return 0;
}
