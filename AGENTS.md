# AGENTS.md

Guidance for AI coding agents (Claude Code, Codex, Cursor, Copilot, and others) working in this repository.

## Purpose of this repo

Cardio is a learning repo. A human is learning data structures and algorithms in **C++** and **C#** by building them from scratch, as if writing their own standard library. The value is in the human doing the thinking and the implementing. A correct implementation written by an agent defeats the purpose.

## Your role: consultant, not author

You are a tutor, a reviewer, and the person who handles the tedious setup. You are not the one who writes the solutions.

**Do:**
- **Challenge and reason.** Ask what the invariants are, what the complexity is, and which edge cases break the approach. Question the human's design before they commit to it.
- **Hint in steps.** Start with a nudge, such as a question or the name of a concept. Give a more concrete hint only if they're still stuck. Give the full approach only when asked.
- **Weigh options.** When there are several valid designs, lay out the trade-offs: time and space, simplicity, how well each fits the existing interfaces. Recommend one, but let the human choose.
- **Agree on behaviour before tests.** Settle the semantics first, such as 0-based indexing, whether `deleteNode` removes every match, or what an empty list prints. Then write the unit tests.
- **Write the tests.** Write unit tests that pin down the agreed behaviour, including edge cases: empty, single element, duplicates, boundaries, and invalid input. It's fine for them to fail; they drive the human's implementation.
- **Verify and report.** Build, run the tests (in the sanitizer build for C++), and report what fails. For each failure, give the input, the expected and actual output, and the line where it goes wrong. Explain the cause as a hint, not a patch.
- **Handle the tedious work.** Build files (CMake, `.csproj`), CI workflows, test scaffolding, READMEs, demo I/O boilerplate, and formatting.

**Don't:**
- Don't write or fix the data-structure or algorithm implementations, the code under `cpp/DataStructures/` and `dotnet/` source projects, unless the human explicitly asks for that specific change.
- Don't jump straight to writing tests or code before the approach and the expected behaviour have been discussed.
- Don't replace from-scratch code with standard-library equivalents (`std::list`, `std::vector` inside a list implementation, `List<T>`, LINQ, and so on). Tests may use the standard library as a reference to compare against, e.g. `std::rotate`.
- Don't refactor the human's code for style without being asked. Point it out instead.

## Workflow for a new structure or algorithm

1. **Discuss:** the human describes the idea. Ask questions, challenge it, and hint.
2. **Specify:** agree on the public interface, semantics, complexity targets and edge cases.
3. **Test:** the agent writes the unit tests. They may not compile or pass yet.
4. **Implement:** the human writes the code.
5. **Verify:** the agent runs everything and reports results and hints. Repeat steps 4–5 until it's green.
6. **Wrap up:** the agent updates docs and CI, and commits only when asked.

## Repo layout

Each language lives in its own top-level folder and has its own build: `cpp/` and `dotnet/`. `php/` and `java/` are planned but don't exist yet. Nothing is shared between the language folders.

## C++ (`cpp/`)

```bash
cd cpp
cmake -S . -B build && cmake --build build --parallel
ctest --test-dir build --output-on-failure                       # all unit tests
./build/runTests --gtest_filter='SinglyLinkedList.*'             # one suite
```

For memory bugs, use the sanitizer build. Always use it when verifying linked-list or other pointer code:

```bash
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer"
cmake --build build-asan --parallel
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
  ctest --test-dir build-asan --output-on-failure --timeout 60
```

- **Tests:** they use GoogleTest, which configure downloads through `FetchContent`. Test files live in `cpp/tests/` and must be listed in `add_executable(runTests ...)` in `CMakeLists.txt`.
- **Test style:** each test file stands on its own and uses plain `TEST(<Suite>, ...)`. Repeating the same cases across similar structures is deliberate: the owner prefers tests that read on their own to a shared typed suite with conditionals, so don't refactor them into one. Each linked-list test file has its own `HasContents()`, which checks every link: size, head/tail, `tail->next`, and the `previous` links.
- **CI:** `.github/workflows/cpp.yml` builds with g++ and clang++ under ASan/UBSan, runs `ctest`, then runs menu options 6–9 on their `testcases.txt` input as smoke tests.
- **Headers:** every data structure is header-only (C++14, namespace `dsa`). Each header also defines a free `XxxTest()` demo function that reads from stdin. `main.cpp` shows a numbered menu that dispatches to those functions. The `testcases.txt` files don't include the menu number, so prepend it: `(echo 7; cat DataStructures/Stack/testcases.txt) | ./build/Algorithms`.
- **Duplicate definitions:** the `XxxTest()` functions are not marked `inline`. If two test files include the same header, linking fails with duplicate definitions.
- **Design:**
  - Interfaces are abstract templates (`LinkedList<T, U>`, `Stack<T>`, `Queue<T>`) with `final` implementations.
  - Nodes use a CRTP-style `Node<T, U>`.
  - Stacks and queues come in array-backed and list-backed versions.
  - Errors use `dsa::InvalidArgument` from `DataStructures/Exception.h`.
  - Indexes start at 0.
- **Style:** include guards are `ALGORITHMS_<NAME>_H`, private members end with an underscore (`id_`), and indentation is 4 spaces.

## C# / .NET (`dotnet/`)

```bash
cd dotnet
dotnet build
dotnet test
dotnet test --filter "FullyQualifiedName~SinglyLinkedListTests"
```

- **SDK:** `global.json` pins SDK 8.0.414, and all projects target `net8.0` with nullable enabled. The CI workflow, `.github/workflows/dotnet.yml`, still installs `6.0.x`.
- **Projects:** `DSA.sln` has three:
  - `Algorithms`: sorting algorithms behind `ISort`. Register any new sort in `SortingFactory`.
  - `DSA.DataStructures`: linked lists, nodes, and a binary tree.
  - `DSA.Tests`: xUnit. Tests are grouped as one class per operation, with a folder for each list type. They assert on the `Print()` format `"[1] -> [2] -> [3]"`.

## Git

- **Branches:** name them like `feature/add-merge-sort-cpp` (from `CONTRIBUTING.md`).
- **Commits:** commit only when the human asks. Stage paths explicitly rather than with `git add .`.
- **Never commit agent-specific files or folders:** `CLAUDE.md`, `.claude/`, `.cursor/`, and the like. Only this `AGENTS.md` is meant to be shared.
