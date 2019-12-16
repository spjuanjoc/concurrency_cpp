#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>

using namespace std::literals::chrono_literals;
using std::cout;
using std::endl;

void foo(const std::string& s)
{
    for(const auto& c: s)
    {
        if(isspace(c))
        {
            std::this_thread::sleep_for(1us);
        }
        else if(isdigit(c))
        {
            cout << c ;
//            vec.emplace_back(c);
        }
    }
    cout << endl;
}
int main(int argc, char* argv[])
{
    std::cout << "Hello world" << std::endl;
    std::ifstream inputFile;
    inputFile.open("../input.txt");
    std::vector<char> vec {};
    const std::uint16_t max_threads = 4;
    std::array<std::thread, max_threads> tArr;
    if(inputFile)
    {
        std::stringstream ssLine{};
        std::string sLine{};


        while (std::getline(inputFile,sLine) )
        {
            std::thread first(foo, sLine);
            first.join();
        }
        std::cout << std::endl;
        inputFile.close();
    }

    std::ofstream out_file {"../output.txt", std::ios::app};
    if (!out_file)
    {
        std::cerr << "Error creating file" << std::endl;
        return 1;
    }
    std::string line;
    for(const auto& it : vec)
    {
        out_file << it ;
    }

    out_file.close();

    return 0;
}
