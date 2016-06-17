#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <algorithm>
#include <boost/hana.hpp>

template <typename T, typename F> struct Engine {
  Engine(const F &f) : func_(f) {}
  void computers(T &&c) { computers_ = c; }

  template <typename... Args, typename U = T,
            typename std::enable_if<boost::hana::Foldable<U>::value>::type * =
                nullptr>
  auto run(const Args &... args) {
    auto ret = boost::hana::transform(
        computers_, [this, &args...](auto &&x) { return func_(x, args...); });
    return ret;
  }

  template <typename... Args, typename U = T,
            typename std::enable_if<!boost::hana::Foldable<U>::value>::type * =
                nullptr>
  auto run(const Args &... args) {
    std::vector<unsigned> ret;
    ret.reserve(computers_.size());
    std::transform(computers_.begin(), computers_.end(),
                   std::back_inserter(ret),
                   [this, &args...](auto &&x) { return func_(x, args...); });
    return ret;
  }

  template <typename U = T,
            typename std::enable_if<!boost::hana::Foldable<U>::value>::type * =
                nullptr>
  void benchmark(unsigned loops, std::vector<unsigned> a,
                 std::vector<unsigned> b) {
    unsigned val = 0;
    std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();
    for (unsigned i = 0; i < loops; ++i) {
      {
        auto results = run(a[i], b[i]);
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
  template <typename... Args, typename U = T,
            typename std::enable_if<boost::hana::Foldable<U>::value>::type * =
                nullptr>
  void benchmark(unsigned loops, const std::vector<unsigned> &a,
                 const std::vector<unsigned> &b) {
    unsigned val = 0;
    std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();
    for (unsigned i = 0; i < loops; ++i) {
      {
        auto results = run(a[i], b[i]);
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
  F func_;
};

#endif // ENGINE_HPP
