#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::future;
using namespace std::chrono_literals;

void test1()
{
    vector<double> v = { 0, 1.1, 2.2, 3.3, 4.4, 5.5 };
    auto K = [ = ](double z)
    {
        double y = 0;

        for (const auto x : v)
        {
            y += x * x * z;
        }

        return y;
    };
    vector<future<double>> VF;

    for (double i :
            {
                1, 2, 3, 4, 5, 6, 7, 8, 9
            })
        VF.push_back(std::async(K, i));
    for_each(VF.begin(), VF.end(), [](future<double>& x)
    {
        cout << x.get() << " ";
    });
}

// option 2 : store shared_futures which allow passing copies
void test2()
{
    vector<double> v = { 0, 1.1, 2.2, 3.3, 4.4, 5.5 };
    auto K = [ = ](double z)
    {
        double y = 0;

        for (const auto x : v)
        {
            y += x * x * z;
        }

        return y;
    };
    vector<std::shared_future<double>> VF;

    for (double i :
            {
                1, 2, 3, 4, 5, 6, 7, 8, 9
            })
        VF.push_back(std::async(K, i));
    for_each(VF.begin(), VF.end(), [](std::shared_future<double> x)
    {
        cout << x.get() << " ";
    });
}
void doAsyncWork()
{
    cout << "Doing async work" << endl;
}
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
    auto timeUsedRB {startForRB - endForRB};
    cout << "time used in range-based for loop"
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
    cout << "Hello World!" << endl;
//    std::thread t(doAsyncWork); //thread-based
////    t.join();
//    std::future<void> f = std::async(std::launch::async /*|
//                              std::launch::deferred*/,
//                                     doAsyncWork);//task-based prefered to thread-based
//    f.get();
//    t.join();
    spawnAsync(2);
//    test1();
    return 0;
}
