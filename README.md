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
cmake .
make
./tests/runTests
```

## .NET

```bash
cd dotnet
dotnet test
```

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
