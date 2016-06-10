#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <algorithm>
#include <boost/hana.hpp>

template <typename T> struct Engine {
  void computers(T &&c) { computers_ = c; }

  template <typename U = T>
  auto run(
      unsigned a, unsigned b,
      typename std::enable_if<boost::hana::Foldable<U>::value>::type * = 0) {
    auto ret = boost::hana::transform(
        computers_, [&a, &b](auto &&x) { return x.compute(a, b); });
    return ret;
  }

  template <typename U = T>
  auto run(
      unsigned a, unsigned b,
      typename std::enable_if<!boost::hana::Foldable<U>::value>::type * = 0) {
    std::vector<unsigned> ret;
    ret.reserve(computers_.size());
    std::transform(computers_.begin(), computers_.end(),
                   std::back_inserter(ret),
                   [&a, &b](auto &&x) { return x->compute(a, b); });
    return ret;
  }

private:
  T computers_;
};

#endif // ENGINE_HPP
