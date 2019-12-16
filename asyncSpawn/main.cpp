#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

using std::cout;
using std::endl;
using std::thread;
using std::vector;

void doAsyncWork()
{
    cout << "Doing async work" << endl;
}
/**
 * Spawns n threads
 */

void spawnAsync(int n)
{
    auto lambda = [ = ]()
    {
        doAsyncWork();
    };
    std::vector<std::future<void>> fv;
    fv.reserve(n);

    for (int i = 1; i <= n; ++i)
    {
        //        std::future<void> f = std::async(std::launch::deferred /*|
        //                                  std::launch::deferred*/,
        //                                         doAsyncWork);
        fv.push_back(std::async(std::launch::deferred, lambda));
    }

    //run them
    auto startForRB{std::chrono::high_resolution_clock::now()};

    for (auto&& it : fv)
    {
        it.get();
    }

    auto endForRB{std::chrono::high_resolution_clock::now()};
    auto timeUsedRB {endForRB - startForRB};
    cout << "time used in range-based for loop: "
         << std::chrono::duration_cast<std::chrono::microseconds>(timeUsedRB).count() << endl;
    //
    //    auto startForFE{std::chrono::high_resolution_clock::now()};
    //    for_each(fv.begin(), fv.end(), [](future<void>& f)
    //    {
    //        f.get();
    //    }
    //            );
    //    auto endForFE{std::chrono::high_resolution_clock::now()};
    //    auto timeUsedFE {startForFE - endForFE};
    //    cout << "time used in range-based for loop"
    //         << std::chrono::duration_cast<std::chrono::microseconds>(timeUsedFE).count() << endl;
}

int main()
{
    spawnAsync(10);
}
