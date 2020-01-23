#include <algorithm>
#include <iostream>
#include <list>
#include <mutex>

class SafeList
{
private:
    std::list<int> some_list;
    std::mutex     some_mutex;

public:
    void add_to_list( int new_value )
    {
        std::lock_guard<std::mutex> guard( some_mutex );
        //    std::lock_guard guard(some_mutex); // since C++17
        some_list.push_back( new_value );
    }
    bool list_contains( int value_to_find )
    {
        std::lock_guard<std::mutex> guard( some_mutex );
        return std::find( some_list.begin(), some_list.end(), value_to_find ) != some_list.end();
    }
};

int main()
{
    std::cout << "Hello, Concurrency!" << std::endl;

    SafeList sl;
    sl.add_to_list( 1 );

    int  find  = 1;
    auto found = sl.list_contains( find );

    std::cout << std::boolalpha << find << " exists?: " << found << std::endl;

    return 0;
}
