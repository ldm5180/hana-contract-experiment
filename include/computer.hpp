#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include <boost/lexical_cast.hpp>

struct Computer {
  virtual ~Computer() {}
  virtual unsigned compute(unsigned a, unsigned b) = 0;
};

struct AdditionComputer : public Computer {
  virtual unsigned compute(unsigned a, unsigned b) final { return a + b; }
};

struct SubtractionComputer : public Computer {
  virtual unsigned compute(unsigned a, unsigned b) final { return a - b; }
};

// Note, this does not inherit from Computer, so it cannot be cast to Computer.
struct MultiplicationComputer : public Computer {
  virtual unsigned compute(unsigned a, unsigned b) final { return a * b; }
};

struct AccumulateFirstArgComputer : public Computer {
  virtual unsigned compute(unsigned a, unsigned b) final {
    (void)b;
    return (accum_ += a);
  }

private:
  unsigned accum_ = 0;
};

struct Addition {
  unsigned compute(unsigned a, unsigned b) const { return a + b; }
};

struct Subtraction {
  unsigned compute(unsigned a, unsigned b) const { return a - b; }
};

// Note, this does not inherit from Computer, so it cannot be cast to Computer.
struct Multiplication {
  unsigned compute(unsigned a, unsigned b) const { return a * b; }
};

struct AccumulateFirstArg {
  unsigned compute(unsigned a, unsigned b) {
    (void)b;
    return (accum_ += a);
  }

private:
  unsigned accum_ = 0;
};

struct StringConcatComputer : public Computer {
  unsigned compute(unsigned a, unsigned b) {
    return boost::lexical_cast<unsigned>(boost::lexical_cast<std::string>(a) +
                                         boost::lexical_cast<std::string>(b));
  }
};

#endif // COMPUTER_HPP
