#include <thread>
#include <iostream>
#include <vector>

using std::cout;
using std::thread;

void doSomething(int id)
{
    cout << id << "\n";
}

/**
 * Spawns n threads
 */
void spawnThreads(int n)
{
    std::vector<thread> threads(n);

    // spawn n threads:
    for (int i = 0; i < n; i++)
    {
        threads[i] = thread(doSomething, i + 1);
    }

    for (auto& th : threads)
    {
        th.join();
    }
}

int main()
{
    spawnThreads(10);
}
