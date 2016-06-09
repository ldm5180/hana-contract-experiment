#include <iostream>
#include <memory>
#include <vector>

#include <boost/hana.hpp>

#include "hanaexpr.hpp"

namespace hana = boost::hana;

using Computers = std::vector<std::shared_ptr<Computer>>;

int main() {
  // Standard way of doing it. Cannot use MultiplicationComputer because it is not derived from Computer.
  Computers computers;
  computers.emplace_back(std::make_shared<AdditionComputer>());
  computers.emplace_back(std::make_shared<SubtractionComputer>());
  //computers.emplace_back(std::make_shared<MultiplicationComputer>());

  std::cout << "Inheritance:\n";
  for (const auto& x : computers) {
    std::cout << x->compute(2, 2) << "\n";
  }

  // Using Hana to optionally add MultiplicationComputer if it exists.
  std::cout << "\nHana:\n";
  auto potentialComputers =
    hana::make_tuple(AdditionComputer{},
		     SubtractionComputer{},
		     MultiplicationComputer{},
		     std::vector<int>{});
  BOOST_HANA_CONSTANT_CHECK(hana::length(potentialComputers) == hana::size_c<4>);

  auto validComputers = hana::filter(potentialComputers, [](auto&& computer) {
      auto hasCompute =
      hana::is_valid([](auto&& obj) -> decltype(obj.compute(1, 1)) { });
      return hasCompute(computer);
    });
  BOOST_HANA_CONSTANT_CHECK(hana::length(validComputers) == hana::size_c<3>);

  hana::for_each(validComputers, [](auto&& x) {
      std::cout<< x.compute(2, 2) << "\n";
    });

  return 0;
}

