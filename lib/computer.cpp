#include "computer.hpp"

unsigned AdditionComputer::compute(unsigned a, unsigned b) const {
  return a + b;
}

unsigned SubtractionComputer::compute(unsigned a, unsigned b) const {
  return a - b;
}

unsigned MultiplicationComputer::compute(unsigned a, unsigned b) const {
  return a * b;
}

unsigned AccumulateFirstArg::compute(unsigned a, unsigned b) {
  (void)b;
  return (accum_ += a);
}
