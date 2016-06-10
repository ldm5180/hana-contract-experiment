# hana-contract-experiment
Trying out implementing an interface contract using Hana instead of a virtual interface for type erasure. This is useful for eliminating the need for an interface class and virtual inheritance, but the drawback is that you can't change the types later.

Benefits:
- No need to store pointers to abstract types in containers.
- No virtual function call dispatch.
- No inheritance.

# Building

Use the Vagrantfile to build an environment for building. Otherwise install the dependencies: clang, cmake, boost.

```bash
cd hana-contract-experiment
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
./bin/hanaexpr-test
```
