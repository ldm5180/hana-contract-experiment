#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <algorithm>
#include <boost/hana.hpp>

template <typename T> struct Engine {
  void computers(T &&c) { computers_ = c; }

  template <typename U = T>
  std::vector<unsigned>
  run(unsigned a, unsigned b,
      typename std::enable_if<boost::hana::Foldable<U>::value>::type * = 0) {
    std::vector<unsigned> ret;
    boost::hana::for_each(computers_, [&ret, &a, &b](auto &&x) {
      ret.push_back(x.compute(a, b));
    });
    return ret;
  }

  template <typename U = T>
  std::vector<unsigned>
  run(unsigned a, unsigned b,
      typename std::enable_if<!boost::hana::Foldable<U>::value>::type * = 0) {
    std::vector<unsigned> ret;
    std::for_each(
        computers_.begin(), computers_.end(),
        [&ret, &a, &b](auto &&x) { ret.push_back(x->compute(a, b)); });
    return ret;
  }

private:
  T computers_;
};

#endif // ENGINE_HPP
