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

  auto interfaceEnforcer = [](auto &computer, const auto &a, const auto &b,
                              ...) { return computer.compute(a, b); };

  // Standard way of doing it. Cannot use MultiplicationComputer because it is
  // not derived from Computer.
  // Note, the special caller is needed because x is a shared_ptr.
  auto ptrInterfaceEnforcer = [](auto &computer, const auto &a, const auto &b,
                                 ...) { return computer->compute(a, b); };
  std::cout << "Inheritance: ";
  {
    Computers computers;
    computers.emplace_back(std::make_shared<AdditionComputer>());
    computers.emplace_back(std::make_shared<SubtractionComputer>());
    computers.emplace_back(std::make_shared<MultiplicationComputer>());
    computers.emplace_back(std::make_shared<AccumulateFirstArgComputer>());

    auto e = NewEngine(std::move(computers));
    e.benchmark(ptrInterfaceEnforcer, numloops, ar, br);
  }

  // Using Hana with the inherited types.
  std::cout << "Hana (w/ inheritance): ";
  {
    auto computers = hana::make_tuple(AdditionComputer{}, SubtractionComputer{},
                                      MultiplicationComputer{},
                                      AccumulateFirstArgComputer{});
    BOOST_HANA_CONSTANT_CHECK(hana::length(computers) == hana::size_c<4>);

    auto e = NewEngine(std::move(computers));
    e.benchmark(interfaceEnforcer, numloops, ar, br);
  }

  // Using Hana with the unrelated types.
  std::cout << "Hana: ";
  {
    auto computers = hana::make_tuple(Addition{}, Subtraction{},
                                      Multiplication{}, AccumulateFirstArg{});
    BOOST_HANA_CONSTANT_CHECK(hana::length(computers) == hana::size_c<4>);

    auto e = NewEngine(std::move(computers));
    e.benchmark(interfaceEnforcer, numloops, ar, br);

    // Show off the ability to use Engine::run() with variable number of args.
    // The accumulator will be different between the 2 runs (4th number) because
    // it is stateful.
    auto args2 = e.run(interfaceEnforcer, ar[1], br[1]);
    boost::hana::for_each(args2, [](const auto &x) { std::cout << x << " "; });
    std::cout << std::endl;

    auto args3 = e.run(interfaceEnforcer, ar[1], br[1], ar[2]);
    boost::hana::for_each(args3, [](const auto &x) { std::cout << x << " "; });
    std::cout << std::endl;
  }

  return 0;
}
