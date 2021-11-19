/**
 * @brief
 *
 * @author  juan.castellanos
 * @date    2021-11-18
 */

#include "Observable.h"
#include <fmt/core.h>
#include <thread>

using namespace Observable;

int main()
{
  fmt::print(">>main\n");
  NameProvider provider;

  auto slot1 = provider.signalFieldChanged.connect(
    [](NameProvider& p, const std::string& fieldName)
    {
      fmt::print("{} has changed\n", fieldName);
    });

  std::string value{"unknown"};

  fmt::print("1 value: {}\n", value);
  fmt::print("2 value: {}\n", value);

  auto slot2 = provider.signalFinished.connect(
    [&value](NameProvider& p, const std::string& fieldName)
    {
      fmt::print("{} has been computed\n", fieldName);
      value = p.getValue();
      fmt::print("lambda value: {}\n", value);
    });


  provider.setValue("30");
  fmt::print("3 value: {}\n", value);
  slot1.disconnect();
//  slot2.disconnect();

  fmt::print("4 value: {}\n", value);

  std::this_thread::sleep_for(5s);

  fmt::print("5 value: {}\n", value);

  fmt::print("<<main\n");

  return 0;
}
