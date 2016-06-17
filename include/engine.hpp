#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <algorithm>
#include <boost/hana.hpp>
#include <utility>

template <typename T> struct Engine {
  Engine(T &&computers) : computers_(std::move(computers)) {}

  template <typename Func, typename... Args, typename U = T,
            typename std::enable_if<boost::hana::Foldable<U>::value>::type * =
                nullptr>
  auto run(const Func &f, const Args &... args) {
    auto ret = boost::hana::transform(
        computers_, [this, &f, &args...](auto &&x) { return f(x, args...); });
    return ret;
  }

  template <typename Func, typename... Args, typename U = T,
            typename std::enable_if<!boost::hana::Foldable<U>::value>::type * =
                nullptr>
  auto run(const Func &f, const Args &... args) {
    std::vector<unsigned> ret;
    ret.reserve(computers_.size());
    std::transform(computers_.begin(), computers_.end(),
                   std::back_inserter(ret),
                   [this, &f, &args...](auto &&x) { return f(x, args...); });
    return ret;
  }

  template <typename Func, typename U = T,
            typename std::enable_if<!boost::hana::Foldable<U>::value>::type * =
                nullptr>
  void benchmark(const Func &f, unsigned loops, const std::vector<unsigned> a,
                 const std::vector<unsigned> b) {
    unsigned val = 0;
    std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();
    for (unsigned i = 0; i < loops; ++i) {
      {
        auto results = run(f, a[i], b[i]);
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

  template <typename Func, typename... Args, typename U = T,
            typename std::enable_if<boost::hana::Foldable<U>::value>::type * =
                nullptr>
  void benchmark(const Func &f, unsigned loops, const std::vector<unsigned> &a,
                 const std::vector<unsigned> &b) {
    unsigned val = 0;
    std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();
    for (unsigned i = 0; i < loops; ++i) {
      {
        auto results = run(f, a[i], b[i]);
        val = boost::hana::fold_left(
            results, val, [](unsigned state, auto v) { return state + v; });
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

private:
  T computers_;
};

template <typename Computers> Engine<Computers> NewEngine(Computers &&c) {
  return Engine<Computers>(std::forward<Computers>(c));
}

#endif // ENGINE_HPP
