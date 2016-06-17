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

  // Using Hana with the unrelated types.
  std::cout << "Hana: ";
  {

    // Show off the ability to use Engine::run() with variable number of args.
    // The accumulator will be different between the 2 runs (4th number) because
    // it is stateful.
    boost::hana::for_each(args2, [](const auto &x) { std::cout << x << " "; });
    std::cout << std::endl;

    auto args3 = e.run(ar[1], br[1], ar[2]);
    boost::hana::for_each(args3, [](const auto &x) { std::cout << x << " "; });
    std::cout << std::endl;
  }

  return 0;
}

struct ScaleToParentSize {
  bool apply(Widget &w, const auto &coefficient, const auto &parents,
             const auto &bounds) {
    w.scaleTo(parents.size());
  }
};
struct SetColorToParentColor {
  bool apply(Widget &w, const auto &coefficient, const auto &parents,
             const auto &bounds) {
    w.colorIs(parents.color());
  }
};

template <typename Widget, typename... Args>
void gainFocusCallback(Widget &w, Args &&... args) {

  auto gainFocus =
      hana::make_tuple(ScaleToParentSize{}, SetColorToParentColor{});

  auto gainFocuserIntf = [](auto &action, auto &widget, const auto &coefficient,
                            const auto &parents, const auto &bounds, ...) {
    return action.apply(widget, coeffienct, parents, bounds);
  };

  auto gainFocusEngine = NewEngine(gainFocuserIntf, gainFocus);

  auto results = gainFocusEngine.run(widget, args...);
}
