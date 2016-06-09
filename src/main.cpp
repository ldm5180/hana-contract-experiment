#include <iostream>
#include <memory>
#include <vector>

#include "hanaexpr.hpp"

int main() {
  std::vector<std::unique_ptr<Computer>> computers;
  computers.emplace_back(std::make_unique<AdditionComputer>());
  computers.emplace_back(std::make_unique<SubtractionComputer>());

  for (const auto& x : computers) {
    std::cout << x->compute(1,1) << "\n";
  }

  return 0;
}

