# Cardio — Algorithms & Data Structures

Cardio is a personal learning project focused on exploring classic and modern algorithms and data structures,
implemented from scratch in multiple programming languages.

This repository serves as both a learning log and a practical reference for how core concepts are expressed across
different ecosystems.

# Current Implementations

| Language         | 	Status          | 	Location	 | Testing                     |
|------------------|------------------|------------|-----------------------------|
| C / C++          | ✅ Active         | /cpp       | Unit tests with Google Test |
| .NET / .NET Core | ✅ Active         | /dotnet    | xUnit test suite            |
| PHP              | 🕐 (coming soon) | /php       | TBD                         |
| Java             | 🕐 (coming soon) | /java      | JUnit (planned)             |

# Project Goals

- Learn and reinforce algorithmic thinking through practical coding.
- Compare performance and syntax differences between languages.
- Build a reusable collection of algorithm implementations for reference and practice.
- Automate build and test pipelines for each language.

# Folder Structure

    .cardio/
    ├── cpp/           # C/C++ implementation + tests
    ├── dotnet/        # .NET implementation + xUnit tests
    ├── php/           # (coming soon)
    ├── java/          # (coming soon)
    └── .github/
        └── workflows/ # Language-specific CI pipelines

# Running Tests

Each language folder includes its own build and test instructions.

## C/C++

```bash
cd cpp
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

See [cpp/README.md](cpp/README.md) for running a single test, the interactive demo, and sanitizer builds.

## .NET

```bash
cd dotnet
dotnet test
```

# Running the Application

## C/C++

The C++ build produces `Algorithms`, an interactive menu that demonstrates each data structure. Build it first (see [Running Tests](#cc)), then from `cpp/`:

```bash
./build/Algorithms
```

Choose a structure by number, then type its input:

| # | Structure                   | # | Structure   |
|---|-----------------------------|---|-------------|
| 1 | Array rotation              | 6 | Array Stack |
| 2 | Singly Linked list          | 7 | List Stack  |
| 3 | Doubly Linked list          | 8 | Array Queue |
| 4 | Circular Singly Linked list | 9 | List Queue  |
| 5 | Circular Doubly Linked list |   |             |

To skip typing, pipe the input in. The stack and queue folders include sample input in `testcases.txt`; put the menu number in front of it:

```bash
(echo 6; cat DataStructures/Stack/testcases.txt) | ./build/Algorithms   # Array Stack
(echo 8; cat DataStructures/Queue/testcases.txt) | ./build/Algorithms   # Array Queue
```

Array rotation (option 1) reads the number of test cases, then `n d` for each case, then the `n` values:

```bash
printf '1\n1\n12 3\n1 2 3 4 5 6 7 8 9 10 11 12\n' | ./build/Algorithms
```

## .NET

The .NET projects are class libraries with no executable. Run them through the test suite with `dotnet test`.

# Contributing

Contributions are welcome!
You can help improve Cardio by:

- 🐞 Reporting bugs: Open an issue with details and reproduction steps.
- 💡 Proposing new algorithms: Use the Feature Request issue template.
- 🧑‍💻 Reviewing pull requests: Provide feedback, improvements, or optimizations.
- 🪜 Improving documentation: Fix typos, clarify concepts, or enhance structure.

Please check existing issues or open a new one before submitting a PR.
All contributions should follow the folder naming and testing conventions per language.

# License

This project is licensed under the [MIT License](LICENSE)
