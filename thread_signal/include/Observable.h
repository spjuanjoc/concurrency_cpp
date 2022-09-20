/**
 * @brief
 *
 * @author  juan.castellanos
 * @date    2021-11-18
 */

#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <fmt/core.h>

#include <thread>

#include <sigc++/sigc++.h>

namespace Observable
{
using namespace std::chrono_literals;

constexpr std::chrono::milliseconds COMPUTING_SLEEP = 500ms;

template <typename Type>
class ObservableSignal
{
public:
  virtual ~ObservableSignal() = default;
  sigc::signal<void(Type&, const std::string&)> signalFieldChanged;
  sigc::signal<void(Type&, const std::string&)> signalFinished;
};

class NameProvider : public ObservableSignal<NameProvider>
{
public:
  NameProvider() = default;

  ~NameProvider() override
  {
    if (m_thread.joinable())
    {
      fmt::print("join");
      m_thread.join();
    }
  }

  void setName(std::string_view value)
  {
    m_name = value;
    signalFieldChanged(*this, "value");
    m_thread = std::thread(&NameProvider::compute, this);
  }

  void compute()
  {
    fmt::print("Computing...\n");
    m_name = "Jane";
    std::this_thread::sleep_for(COMPUTING_SLEEP);
    fmt::print("Done computing\n");
    signalFinished(*this, "compute");
  }

  std::string getName() const { return m_name; }

private:
  std::string m_name;
  std::thread m_thread;
};

}  // namespace Observable

#endif /* OBSERVABLE_H */
