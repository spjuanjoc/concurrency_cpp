/**
 * @brief
 *
 * @author  juan.castellanos
 * @date    2021-11-18
 */

#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <fmt/core.h>
#include <sigc++/sigc++.h>
#include <thread>

namespace Observable
{
using namespace std::chrono_literals;

template<typename T>
struct SignalObservable
{
  sigc::signal<void(T&, const std::string&)> signalFieldChanged;
  sigc::signal<void(T&, const std::string&)> signalFinished;
};

class NameProvider : public SignalObservable<NameProvider>
{
public:
  NameProvider() = default;

  virtual ~NameProvider()
  {
    if (m_thread.joinable())
    {
      m_thread.join();
    }
  }

  void setValue(std::string_view value)
  {
    m_value = value;
    signalFieldChanged(*this, "setValue");
    m_thread = std::thread(&NameProvider::compute, this);
  }

  void compute()
  {
    fmt::print("Computing...\n");
    m_value = "new value";
    std::this_thread::sleep_for(1s);
    fmt::print("Done\n");
    signalFinished(*this, "compute");
  }

  std::string getValue() const
  {
    return m_value;
  }

private:
  std::string m_value;
  std::thread m_thread;
};
}  // namespace Observable

#endif /* OBSERVABLE_H */
