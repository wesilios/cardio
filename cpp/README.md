# Cardio — C++

Header-only data structures (C++14, namespace `dsa`) with an interactive demo program and GoogleTest unit tests.

## Requirements

- CMake 3.20+
- A C++14 compiler (g++ or clang++)
- Internet access on the first configure, which downloads GoogleTest

## Build

Run these from the `cpp/` folder:

```bash
cmake -S . -B build
cmake --build build --parallel
```

This produces two executables:

| Binary             | Purpose                                 |
| ------------------ | --------------------------------------- |
| `build/Algorithms` | Interactive menu demo of each structure |
| `build/runTests`   | GoogleTest unit tests                   |

To build only the demo, without downloading GoogleTest:

```bash
cmake -S . -B build -DCARDIO_BUILD_TESTS=OFF
```

## Run the unit tests

```bash
ctest --test-dir build --output-on-failure      # all tests
./build/runTests                                # all tests, GoogleTest output
./build/runTests --gtest_filter='ArrayStack.*'  # one test suite
./build/runTests --gtest_filter='ArrayStack.PopOnEmptyThrows'  # one test
./build/runTests --gtest_list_tests             # list available tests
```

Rebuild with `cmake --build build` after changing code; you don't need to configure again.

### Adding a test file

1. Create `tests/<Name>Tests.cpp` and include the header, e.g. `#include "DataStructures/Stack/ArrayStack.h"`.
2. Add the file to `add_executable(runTests ...)` in `CMakeLists.txt`.

## Run the demo

```bash
./build/Algorithms
```

Pick a structure by number, then type its input. Each structure's `testcases.txt` has sample input that doesn't include the menu number, so add it in front:

```bash
(echo 6; cat DataStructures/Stack/testcases.txt) | ./build/Algorithms   # Array Stack
(echo 7; cat DataStructures/Stack/testcases.txt) | ./build/Algorithms   # List Stack
(echo 8; cat DataStructures/Queue/testcases.txt) | ./build/Algorithms   # Array Queue
(echo 9; cat DataStructures/Queue/testcases.txt) | ./build/Algorithms   # List Queue
```

## Sanitizer build

CI builds with these flags. This build catches memory errors and undefined behaviour:

```bash
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer"
cmake --build build-asan --parallel
ctest --test-dir build-asan --output-on-failure
```
