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
  std::vector<unsigned> ar(numloops);
  std::vector<unsigned> br(numloops);
  std::generate(ar.begin(), ar.end(), std::rand);
  std::generate(br.begin(), br.end(), std::rand);

  auto pcaller =
      [](auto &x, const auto &a, const auto &b) { return x->compute(a, b); };
  auto caller =
      [](auto &x, const auto &a, const auto &b) { return x.compute(a, b); };

  // Standard way of doing it. Cannot use MultiplicationComputer because it is
  // not derived from Computer.
  std::cout << "Inheritance: ";
  {
    Computers computers;
    computers.emplace_back(std::make_shared<AdditionComputer>());
    computers.emplace_back(std::make_shared<SubtractionComputer>());
    computers.emplace_back(std::make_shared<MultiplicationComputer>());
    computers.emplace_back(std::make_shared<AccumulateFirstArgComputer>());
    Engine<decltype(computers), decltype(pcaller)> e(pcaller);
    e.computers(std::move(computers));
    e.benchmark(numloops, ar, br);
  }

  // Using Hana with the inherited types.
  std::cout << "Hana (w/ inheritance): ";
  {
    auto computers = hana::make_tuple(AdditionComputer{}, SubtractionComputer{},
                                      MultiplicationComputer{},
                                      AccumulateFirstArgComputer{});
    BOOST_HANA_CONSTANT_CHECK(hana::length(computers) == hana::size_c<4>);

    Engine<decltype(computers), decltype(caller)> e(caller);
    e.computers(std::move(computers));
    e.benchmark(numloops, ar, br);
  }

  // Using Hana with the unrelated types.
  std::cout << "Hana: ";
  {
    auto computers = hana::make_tuple(Addition{}, Subtraction{},
                                      Multiplication{}, AccumulateFirstArg{});
    BOOST_HANA_CONSTANT_CHECK(hana::length(computers) == hana::size_c<4>);

    Engine<decltype(computers), decltype(caller)> e(caller);
    e.computers(std::move(computers));
    e.benchmark(numloops, ar, br);
  }

  return 0;
}
