#include <fmt/core.h>

#include "SafeList.hpp"
#include "UnProtectedMutex.h"

int
main()
{
  fmt::print("Safe list using mutex\n");

  SafeList<int> safe_list;
  safe_list.add(1);

  int  find  = 1;
  auto found = safe_list.contains(find);
  fmt::print("Value: {} exists?: {}\n", find, found);

  int  find2  = 2;
  auto found2 = safe_list.contains(find2);
  fmt::print("Value: {} exists?: {}\n", find2, found2);

  safe_list.print();

  fmt::print("\n");
  fmt::print("Unsafe class even using mutex\n");

  DataWrapper x;
  x.processData(maliciousFunction);  //Pass in a malicious function
  unprotected_data->doSomething();   //Unprotected access to protected data

  return 0;
}
