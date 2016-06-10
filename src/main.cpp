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

constexpr unsigned numloops = 100000000;

int main() {
  std::srand(std::time(0)); // use current time as seed for random generator
  // Add 1 to avoid the boring case where there is a 0.
  unsigned randA = std::rand() % 10 + 1;
  unsigned randB = std::rand() % 10 + 1;

  // Standard way of doing it. Cannot use MultiplicationComputer because it is
  // not derived from Computer.
  std::cout << "Inheritance: ";
  {
    Computers computers;
    computers.emplace_back(std::make_shared<AdditionComputer>());
    computers.emplace_back(std::make_shared<SubtractionComputer>());
    computers.emplace_back(std::make_shared<MultiplicationComputer>());
    computers.emplace_back(std::make_shared<AccumulateFirstArgComputer>());
    Engine<decltype(computers)> e;
    e.computers(std::move(computers));
    e.benchmark(numloops, randA, randB);
  }

  // Using Hana with the inherited types.
  std::cout << "Hana (w/ inheritance): ";
  {
    auto computers = hana::make_tuple(AdditionComputer{}, SubtractionComputer{},
                                      MultiplicationComputer{},
                                      AccumulateFirstArgComputer{});

    BOOST_HANA_CONSTANT_CHECK(hana::length(computers) == hana::size_c<4>);
    Engine<decltype(computers)> e;
    e.computers(std::move(computers));
    e.benchmark(numloops, randA, randB);
  }

  // Using Hana with the unrelated types.
  std::cout << "Hana: ";
  {
    auto computers = hana::make_tuple(Addition{}, Subtraction{},
                                      Multiplication{}, AccumulateFirstArg{});

    BOOST_HANA_CONSTANT_CHECK(hana::length(computers) == hana::size_c<4>);
    Engine<decltype(computers)> e;
    e.computers(std::move(computers));
    e.benchmark(numloops, randA, randB);
  }

  return 0;
}
