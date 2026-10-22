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

Each linked list has its own self-contained test file, `tests/<Name>LinkedListTests.cpp`. The four files repeat the same test cases on purpose, so each one reads on its own. Each file has its own `HasContents()` helper, which checks every link for that list type. If you add a test that applies to every list, add it to all four files. To run one list's tests:

```bash
./build/runTests --gtest_filter='CircularSinglyLinkedList.*'
ctest --test-dir build -R '^CircularSinglyLinkedList\.'
```

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

## Sanitizer build (ASan, UBSan, leak detection)

The linked lists manage memory by hand with `new`/`delete`. A memory bug there often doesn't fail a normal build: the test just passes by luck. Build with the sanitizers to catch these bugs. CI runs every test this way.

| Sanitizer | Catches |
| --- | --- |
| AddressSanitizer (ASan) | use after free, double free, out-of-bounds reads and writes, mismatched `new`/`delete` |
| LeakSanitizer (part of ASan) | memory that is never freed, such as nodes a destructor or `clear()` skipped |
| UndefinedBehaviorSanitizer (UBSan) | null pointer dereference, signed overflow, invalid shifts, and similar |

### 1. Configure and build

Use a separate build folder so the normal `build/` stays fast:

```bash
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer"
cmake --build build-asan --parallel
```

`-DCMAKE_BUILD_TYPE=Debug` and `-fno-omit-frame-pointer` give readable stack traces with file and line numbers. To use clang instead of gcc, add `-DCMAKE_CXX_COMPILER=clang++`.

### 2. Run the tests

```bash
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
  ctest --test-dir build-asan --output-on-failure --timeout 60
```

- `detect_leaks=1` turns on leak detection. It's the default on Linux, but setting it makes the check explicit.
- `halt_on_error=1` makes the first error fail the test. Without it, UBSan only prints a warning and the test still passes.
- `print_stacktrace=1` shows where a UBSan error happened.
- `--timeout 60` stops a test stuck in an infinite loop, such as a broken circular link.

Prefer `ctest` over running `./build-asan/runTests` directly. ctest runs each test in its own process, so a failure is reported under the name of the test that caused it.

To check one list or one test:

```bash
export ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1
ctest --test-dir build-asan --output-on-failure -R '^DoublyLinkedList\.'
./build-asan/runTests --gtest_filter='DoublyLinkedList.ClearOnSingleNode'
```

### 3. Check the demo for leaks

The demo binary is built with the same flags, so you can run it on sample input:

```bash
(echo 7; cat DataStructures/Stack/testcases.txt) | \
  ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 ./build-asan/Algorithms > /dev/null
echo $?   # 0 = clean; non-zero = see the report printed on stderr
```

### Reading a report

- **`ERROR: AddressSanitizer: heap-use-after-free`** (or `double-free`): the report has three stack traces. The first shows where the bad access happened, the second where the memory was freed, and the third where it was allocated.
- **`ERROR: LeakSanitizer: detected memory leaks`**: each `Direct leak` entry shows where the leaked memory was allocated. Look for the first frame in your own code, e.g. `addLast(...) CircularDoublyLinkedList.h:52`, then work out which `delete` is missing.
- **`runtime error: member access within null pointer`**: this one comes from UBSan. The file and line on that output line is where the null pointer was used.

LeakSanitizer reports only when the process exits, after every test in it has finished. The test itself still prints `[ OK ]`, and then the process fails with exit code 1. If `runTests` prints only `[ OK ]` lines but still fails, check the end of the output for a LeakSanitizer report.

Leak detection works on Linux with gcc or clang. Apple's clang on macOS doesn't support `detect_leaks`, so use Linux, WSL or a Docker container, or rely on CI.
