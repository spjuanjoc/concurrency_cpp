#include "fmt/core.h"
#include "fmt/ostream.h"
#include <clocale>
#include <map>
#include <mutex>
#include <sstream>
#include <thread>

using std::thread;

std::mutex                             g_mutex;
std::map<std::thread::id, std::string> threadNames{};

/**
 * Sets the thread name to a map instead of the POSIX function
 *    pthread_setname_np(th.native_handle(), name.c_str());
 * @param tid
 * @param name
 */
void setThreadName(const std::thread::id& tid, const std::string& name)
{
  std::lock_guard lock(g_mutex);
  threadNames[tid] = name;
}

/**
 * Gets the thread name from the map instead of the POSIX function
 *    pthread_getname_np(th.native_handle(), name. size);
 * @param tid
 * @return
 */
std::string getThreadName(const std::thread::id& tid)
{
  std::lock_guard lock(g_mutex);

  std::string result;
  const auto  it = threadNames.find(tid);

  if (it != threadNames.end())
  {
    result = it->second;
  }
  else
  {
    std::stringstream ss;
    ss << tid;
    result = ss.str();
    result = result.substr(result.length() - 5);
  }
  return result;
}

void foo()
{
  auto tid = getThreadName(std::this_thread::get_id());
  fmt::print("Do some foo stuff [{}]\n", tid);
}

void bar(int x)
{
  auto tid = getThreadName(std::this_thread::get_id());
  fmt::print("Do some bar: {} stuff tid:[{}]\n", x, tid);
  fmt::print("thread fullid: [{}]\n", std::this_thread::get_id());
}

int main()
{
  std::setlocale(LC_ALL, "es_ES.UTF-8");

  {
    // inicia hilo que llama a foo()
    thread first(foo);
    setThreadName(first.get_id(), "foo");

    // inicia hilo que llama a bar(0)
    thread second(bar, 0);
    //  setThreadName(second.get_id(), "bar");

    fmt::print("La ejecución concurrente de las funciones foo y bar va a comenzar\n");

    // sincronizar los hilos
    if (first.joinable())
      first.join();

    if (second.joinable())
      second.join();
  }
  fmt::print("Ejecución finalizada\n");

  fmt::print("All the saved threads names:\n");

  for (const auto& it : threadNames)
  {
    fmt::print("{} : {}\n", it.first, it.second);
  }
  return 0;
}
