/**
 * @brief
 *
 * @author  spjuanjoc
 * @date    2021-11-18
 */

#include <fmt/core.h>

#include "Observable.h"

#include <thread>

using namespace Observable;

const std::chrono::milliseconds DISCONNECTION_SLEEP = 2s;

int
main()
{
  fmt::print(">>main\n");
  NameProvider provider;
  std::string  computed_name { "unknown" };

  auto onFieldChanged = provider.signalFieldChanged.connect(
    [](NameProvider& name_provider, const std::string& field_name)
    {
      fmt::print("on field changed\n");
      fmt::print("NameProvider::{} has changed\n", field_name);
    });

  auto onComputeFinished = provider.signalFinished.connect(
    [&computed_name](NameProvider& name_provider, const std::string& field_name)
    {
      fmt::print("NameProvider::{} has been computed\n", field_name);
      computed_name = name_provider.getName();
      fmt::print("on finished computed name: {}\n", computed_name);
    });

  fmt::print("Provider initial name: '{}'\n", provider.getName());
  fmt::print("Initial computed name: {}\n", computed_name);
  provider.setName("John");
  fmt::print("changing provider name...\n");
  fmt::print("changed provider name: {}\n", provider.getName());
  fmt::print("computed name: {}\n", computed_name);

  std::this_thread::sleep_for(DISCONNECTION_SLEEP);

  onFieldChanged.disconnect();
  onComputeFinished.disconnect();

  fmt::print("signals disconnected\n");
  fmt::print("<<main\n");

  return 0;
}
