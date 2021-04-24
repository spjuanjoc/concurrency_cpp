#include <algorithm>
#include <iostream>
#include <list>
#include <mutex>
#include <numeric>
#include <vector>

template<typename TFunc>
void RunAndMeasure( const char* title, TFunc func )
{
    const auto start = std::chrono::steady_clock::now();
    auto       ret   = func();
    const auto end   = std::chrono::steady_clock::now();
    std::cout << title
              << " time: " << std::chrono::duration<double, std::milli>( end - start ).count()
              << " ms, result: " << ret << "\n";
}

int main()
{
    std::cout << "Hello, Concurrency!" << std::endl;

    //    std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::vector<double> v( 6000000, 0.5 );

    RunAndMeasure( "std::accumulate", [&v] { return std::accumulate( v.begin(), v.end(), 0.0 ); } );

    RunAndMeasure( "std::find", [&v] {
        auto res = std::find( v.begin(), v.end(), 0.6 );
        return res == std::end( v ) ? 0.0 : 1.0;
    } );

    return 0;
}
