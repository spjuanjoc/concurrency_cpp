#include "fmt/core.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

using std::cout;
using namespace std::literals::chrono_literals;

void readNumbers(const std::string& line)
{
  std::vector<unsigned char> numbers;

  for (const auto& c : line)
  {
    if (isdigit(c))
    {
      cout << c;
    }
  }
  cout << std::endl;
}

int main()
{
  fmt::print("Read from a file and print only the numbers\n");

  std::ifstream inputFile;
  inputFile.open("input.txt");

  if (inputFile)
  {
    std::string line{};

    while (std::getline(inputFile, line))
    {
      std::thread first(readNumbers, line);
      first.join();
    }

    inputFile.close();
  }

  return 0;
}
