#ifndef HANAEXPR_HPP
#define HANAEXPR_HPP

struct Computer {
  virtual ~Computer() {}
  virtual unsigned compute(unsigned a, unsigned b) const = 0;
};

struct AdditionComputer : public Computer {
  virtual unsigned compute(unsigned a, unsigned b) const final;
};

struct SubtractionComputer : public Computer {
  virtual unsigned compute(unsigned a, unsigned b) const final;
};

// Note, this does not inherit from Computer, so it cannot be cast to Computer.
struct MultiplicationComputer {
  unsigned compute(unsigned a, unsigned b) const;
};

#endif // HANAEXPR_HPP
