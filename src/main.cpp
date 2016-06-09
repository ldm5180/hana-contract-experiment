#include <iostream>
#include <memory>
#include <vector>

#include "computer.hpp"
#include "engine.hpp"

namespace hana = boost::hana;

using Computers = std::vector<std::shared_ptr<Computer>>;

int main() {
  // Standard way of doing it. Cannot use MultiplicationComputer because it is
  // not derived from Computer.
  std::cout << "Inheritance:\n";
  Computers computers;
  computers.emplace_back(std::make_shared<AdditionComputer>());
  computers.emplace_back(std::make_shared<SubtractionComputer>());
  // computers.emplace_back(std::make_shared<MultiplicationComputer>());

  {
    Engine<decltype(computers)> e;
    e.computers(std::move(computers));
    auto results = e.run(3, 2);
    for (const auto &x : results) {
      std::cout << x << std::endl;
    }
  }

  // Using Hana to optionally add MultiplicationComputer if it exists.
  std::cout << "\nHana:\n";
  auto allComputers =
      hana::make_tuple(AdditionComputer{}, SubtractionComputer{},
                       MultiplicationComputer{}, AccumulateFirstArg{});

  BOOST_HANA_CONSTANT_CHECK(hana::length(allComputers) == hana::size_c<4>);
  {
    Engine<decltype(allComputers)> e;
    e.computers(std::move(allComputers));
    auto results = e.run(3, 2);
    for (const auto &x : results) {
      std::cout << x << std::endl;
    }
  }

  // You can even filter the computers at compile time. Put something which
  // doesn't have the compute() function into all computers, such as
  // 'std::vector<int>{}'. Then you can filter it out using something like this.
  auto validComputers = hana::filter(allComputers, [](auto &&computer) {
    auto hasCompute =
        hana::is_valid([](auto &&obj) -> decltype(obj.compute(1, 1)) {});
    return hasCompute(computer);
  });

  return 0;
}
