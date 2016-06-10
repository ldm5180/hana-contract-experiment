#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

#include "computer.hpp"
#include "engine.hpp"

namespace hana = boost::hana;

using Computers = std::vector<std::shared_ptr<Computer>>;

int main() {
  std::srand(std::time(0)); // use current time as seed for random generator
  unsigned randA = std::rand() % 10;
  unsigned randB = std::rand() % 10;

  // Standard way of doing it. Cannot use MultiplicationComputer because it is
  // not derived from Computer.
  Computers computers;
  computers.emplace_back(std::make_shared<AdditionComputer>());
  computers.emplace_back(std::make_shared<SubtractionComputer>());
  computers.emplace_back(std::make_shared<MultiplicationComputer>());
  computers.emplace_back(std::make_shared<AccumulateFirstArgComputer>());
    Engine<decltype(computers)> e;
    e.computers(std::move(computers));

    auto allComputers = hana::make_tuple(
        AdditionComputer{}, SubtractionComputer{}, MultiplicationComputer{},
        AccumulateFirstArgComputer{});

    BOOST_HANA_CONSTANT_CHECK(hana::length(allComputers) == hana::size_c<4>);
    Engine<decltype(allComputers)> eAll;
    eAll.computers(std::move(allComputers));

    auto rawComputers = hana::make_tuple(
        Addition{}, Subtraction{}, Multiplication{}, AccumulateFirstArg{});

    BOOST_HANA_CONSTANT_CHECK(hana::length(rawComputers) == hana::size_c<4>);
    Engine<decltype(rawComputers)> eRaw;
    eRaw.computers(std::move(rawComputers));

    {
      std::cout << "Inheritance: ";
      unsigned val = 0;
      std::clock_t c_start = std::clock();
      auto t_start = std::chrono::high_resolution_clock::now();
      for (unsigned i = 0; i < 100000000; ++i) {
        {
          auto results = e.run(randA, randB);
          val = std::accumulate(results.begin(), results.end(), val);
        }
    }
    std::clock_t c_end = std::clock();
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
              << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
              << "Wall clock time passed: "
              << std::chrono::duration<double, std::milli>(t_end - t_start)
                     .count() << " ms\n";
    std::cout << val << "\n\n";
  }

  {
    // Using Hana with the inherited types.
    std::cout << "Hana (w/ inheritance): ";
    unsigned val = 0;
    std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();
    for (unsigned i = 0; i < 100000000; ++i) {
      {
        auto results = eAll.run(randA, randB);
        val = hana::fold_left(results, val,
                              [](unsigned state, auto v) { return state + v; });
      }
    }
    std::clock_t c_end = std::clock();
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
              << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
              << "Wall clock time passed: "
              << std::chrono::duration<double, std::milli>(t_end - t_start)
                     .count() << " ms\n";
    std::cout << val << "\n\n";
  }

  {
    // Using Hana with the raw types.
    std::cout << "Hana: ";
    unsigned val = 0;
    std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();
    for (unsigned i = 0; i < 100000000; ++i) {
      {
        auto results = eRaw.run(randA, randB);
        val = hana::fold_left(results, val,
                              [](unsigned state, auto v) { return state + v; });
      }
    }
    std::clock_t c_end = std::clock();
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
              << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
              << "Wall clock time passed: "
              << std::chrono::duration<double, std::milli>(t_end - t_start)
                     .count() << " ms\n";
    std::cout << val << "\n\n";
  }

  return 0;
}
