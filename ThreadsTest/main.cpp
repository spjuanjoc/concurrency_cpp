#include <clocale>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <thread>

using std::cout;
using std::endl;
using std::thread;

std::mutex m;

std::map<std::thread::id, std::string> threadNames{};

void setThreadName(const std::thread::id& tid, const std::string& name)
{
  std::lock_guard<std::mutex> lock(m);
  threadNames[tid] = name;
//  pthread_setname_np(th.native_handle(), name.c_str());
}

std::string getThreadName(const std::thread::id& tid)
{
  std::lock_guard<std::mutex> lock(m);
//  pthread_getname_np(th.native_handle(), name. size);
    std::string result;
    const auto it = threadNames.find(tid);
    if (it != threadNames.end())
    {
        result = it->second;
    }
    else
    {
        std::stringstream ss;
        ss << tid;
        result = ss.str();
    }
    return result;
}

void foo()
{
    auto tid = getThreadName(std::this_thread::get_id());
    cout << "Do some foo stuff ["  << tid << "]\n";
}

void bar(int x)
{
    auto tid = getThreadName(std::this_thread::get_id());
    cout << "Do some bar: " << x << " stuff ["  << tid  << "]\n";
}

int main()
{
    //setlocale("")
    std::setlocale(LC_ALL, "es_ES.UTF-8");
//    std::locale::global(std::locale("ES"));
//    cout << "Locale: " << std::locale("ES").name().c_str() << endl;

    thread first(foo);//inicia hilo que llama a foo()
    setThreadName(first.get_id(), "foo");


    thread second(bar, 0); //inicia hilo que llama a bar(0)
    setThreadName(second.get_id(), "bar");

    cout << "La ejecución concurrente de las funciones foo y bar va a comenzar " << endl;

    //sincronizar los hilos
    first.join();
    second.join();
    cout << "Ejecución finalizada" << endl;

//    for(const auto& it: threadNames)
//    {
//      cout << it.first << ":" << it.second << "\n";
//    }
    return 0;
}
