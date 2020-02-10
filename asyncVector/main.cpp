#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <fstream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

using std::cout;
using std::endl;
using std::fstream;
using std::future;
using std::lock_guard;
using std::mutex;
using std::string;
using std::vector;
using namespace std::literals::chrono_literals;

void WriteFile(const string& s)
{
    mutex mt;
    lock_guard<mutex> lk (mt);
    cout << "do sequential work" << endl;
    std::ofstream out_file {"../output.txt", std::ios::app};
    if (!out_file) 
    {
        std::cerr << "Error creating file" << endl;
        return;
    }
    for(const auto& it : s)
    {
        out_file << it;
    }
    out_file << endl;
    
    out_file.close();
}
bool removeSpaces(char c)
{
    if(isspace(c))
    {
        cout << "thread [" << std::this_thread::get_id() << "] paused" << endl;
        std::this_thread::sleep_for(1s);
        return true;
    }
    else if(isdigit(c))
    {
        return false;
    }
    else
    {
        return false;
    }
}
void doAsyncWork(string s)
{
    cout << "Doing async work" << endl;
    s.erase(std::remove_if(s.begin(),s.end(), removeSpaces ), s.end() );
//    cout << "cleaned line: " << s << endl;
    std::sort(s.begin(), s.end());
//    cout << "sorted line: " << s << endl;
    //do other stuff like sort
//    cout << s << endl;
    WriteFile(s);
}
void spawnAsync(const int& n, std::ifstream& file)
{
    string s;
    auto lambda = [](string s)
                    {
                        doAsyncWork(s);
                    };
    vector<future<void>> fv;
    fv.reserve(n);
    for (int i = 1; i <= n; ++i)
    {
        if(!file.eof() )
        {
            
            std::getline(file, s);
            cout << "line: " << s << endl;
            
            fv.push_back(std::async(std::launch::deferred | std::launch::async, 
                                    lambda, 
                                    s ));
        }
    }
    //run them
    auto startForRB{std::chrono::high_resolution_clock::now()};
    for (auto&& it : fv)
    {
        it.get();
    }
    auto endForRB{std::chrono::high_resolution_clock::now()};
    auto timeUsedRB {endForRB - startForRB};
//    cout << "duration: "
//         << std::chrono::duration_cast<std::chrono::microseconds>(timeUsedRB).count() 
//         << " us" 
//         << endl;
}

int main(int argc, char* argv[])
{
    std::cout << "Hello world" << std::endl;
    std::ifstream inputFile;
    inputFile.open("../input.txt");
    std::vector<char> vec {};
    if(inputFile)
    {
        std::string sLine{};

        while (!inputFile.eof() ) 
        {
            spawnAsync(10, inputFile);
        }
        std::cout << std::endl;
        inputFile.close();
    }else{
        std::cerr << "Error reading file" << std::endl;
        return 1;
    }

	return 0;
}
