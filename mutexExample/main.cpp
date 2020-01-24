#include <algorithm>
#include <iostream>
#include <list>
#include <mutex>

template<typename T>
class SafeList
{
private:
    std::list<T> some_list;
    std::mutex   some_mutex;

public:
    void add_to_list( T new_value )
    {
        std::lock_guard<std::mutex> guard( some_mutex );
//            std::lock_guard guard(some_mutex); // since C++17
        some_list.push_back( new_value );
    }
    bool list_contains( T value_to_find )
    {
        std::lock_guard<std::mutex> guard( some_mutex );
        return std::find(some_list.begin(), some_list.end(), value_to_find ) != some_list.end();
    }
    void print() const
    {
        for (const auto& it : some_list)
        {
            std::cout << it << " ";
        }
        std::cout << "\n";
    }
};

int main()
{
    std::cout << "Hello, Concurrency!" << std::endl;

    SafeList<int> sl;
    sl.add_to_list( 1 );

    int  find  = 1;
    auto found = sl.list_contains( find );
    std::cout << std::boolalpha << find << " exists?: " << found << std::endl;
    int  find2  = 2;
    auto found2 = sl.list_contains( find2 );
    std::cout << find2 << " exists?: " << found2 << std::endl;

    sl.print();
    return 0;
}