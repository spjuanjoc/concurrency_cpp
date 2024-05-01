#include <fmt/std.h>

#include <clocale>
#include <map>
#include <mutex>
#include <thread>

static constexpr auto ID_LENGTH = 5;

std::mutex                             g_mutex;
std::map<std::thread::id, std::string> g_thread_names {};

/**
 * @brief Sets the thread name to a map instead of the POSIX function
 *        pthread_setname_np(th.native_handle(), name.c_str());
 *
 * @param tid  The thread numeric identifier.
 * @param name The name to set.
 */
void
setThreadName(const std::thread::id& tid, const std::string& name)
{
  std::lock_guard lock(g_mutex);
  g_thread_names[tid] = name;
}

/**
 * @brief Gets the thread name from the map instead of the POSIX function
 *        pthread_getname_np(th.native_handle(), name. size);
 *
 * @param tid The thread numeric identifier.
 *
 * @return The name of the thread.
 */
std::string
getThreadName(const std::thread::id& tid)
{
  std::lock_guard lock(g_mutex);

  std::string result;
  const auto  it = g_thread_names.find(tid);

  if (it != g_thread_names.end())
  {
    result = it->second;
  }
  else
  {
    const std::string thread_name = fmt::format("{}", tid);

    result = thread_name.substr(thread_name.length() - ID_LENGTH);
  }

  return result;
}

void
foo()
{
  std::string name = getThreadName(std::this_thread::get_id());
  fmt::print("Foo para el hilo [{}]:{}\n", name, std::this_thread::get_id());
}

void
bar(int value)
{
  std::string name = getThreadName(std::this_thread::get_id());
  fmt::print("Bar: {}, para el hilo:[{}]:{}\n", value, name, std::this_thread::get_id());
}

int
main()
{
  std::setlocale(LC_ALL, "es_ES.UTF-8");

  {
    // inicia hilo que llama a foo()
    std::thread first(foo);
    setThreadName(first.get_id(), "hilo_1_foo");

    // inicia hilo que llama a bar(0)
    std::thread second(bar, 0);
    fmt::println("Hilo sin nombre: {}", getThreadName(second.get_id()));
    setThreadName(second.get_id(), "hilo_2_bar");

    fmt::print("La ejecución concurrente de las funciones foo y bar va a comenzar\n");

    // sincronizar los hilos
    if (first.joinable())
      first.join();

    if (second.joinable())
      second.join();
  }

  fmt::print("Ejecución finalizada\n");

  fmt::print("Hilos con nombre:\n");

  for (const auto& [id, name] : g_thread_names)
  {
    fmt::print("{}: {}\n", name, id);
  }

  return 0;
}
