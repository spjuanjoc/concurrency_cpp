#include <fmt/core.h>

#include <chrono>
#include <future>
#include <vector>

using namespace std::chrono_literals;

void
generaTareas(const std::uint32_t numero_de_hilos)
{
  std::vector<std::future<size_t>> futuros;
  futuros.reserve(numero_de_hilos);

  for (std::size_t i = 1; i <= numero_de_hilos; ++i)
  {
    futuros.emplace_back(
      std::async(
        std::launch::async,
        [=]()
        {
          //        std::this_thread::sleep_for(std::chrono::milliseconds(100));

          return i;
        }));
  }

  const auto inicio { std::chrono::high_resolution_clock::now() };
  for (auto&& futuro : futuros)
  {
    auto valor = futuro.get();
    fmt::println("{}", valor);
  }
  const auto fin { std::chrono::high_resolution_clock::now() };
  const auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
  fmt::print("Duración: {} us\n", duracion);
}

int
main()
{
  const auto numero_de_hilos   = std::thread::hardware_concurrency();
  const auto hilos_disponibles = std::max<uint32_t>(1, numero_de_hilos - 1);
  fmt::println("Hilos disponibles: {}", hilos_disponibles);

  generaTareas(hilos_disponibles);

  return 0;
}
