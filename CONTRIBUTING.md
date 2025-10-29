# Contributing to Cardio

Thank you for your interest in contributing to **Cardio**!  
This project is about learning, experimenting, and improving algorithm implementations across multiple languages.

We welcome **all kinds of contributions** — from reporting bugs to implementing algorithms or improving documentation.

---

# How to Contribute

### 1. Fork and Clone
Fork the repository to your account and clone it locally:

```bash
git clone https://github.com/<your-username>/cardio.git
cd cardio
```

### 2. Create a Branch
Use a descriptive branch name:

```bash
git checkout -b feature/add-merge-sort-cpp
```

### 3. Make Changes

- Keep algorithm implementations under their respective language folder. 
- Include unit tests whenever possible.
- Ensure naming and structure follow existing patterns.

### 4. Run tests
Make sure all tests pass before submitting:

```bash
# Example: for .NET
cd dotnet
dotnet test
```

### 5. Commit and Push

Write clear commit messages:

```bash
git add .
git commit -m "Add merge sort implementation in C++"
git push origin feature/add-merge-sort-cpp
```

### 6.Open a Pull Request

- Open a PR against the main branch.
- Provide a short description of what your change does.
- The GitHub workflow will automatically run tests and label your PR based on folder paths.

# Reporting Bugs

If you find an issue:

1. Go to Issues
2. Choose Bug Report from the issue templates.
3. Describe:
   - Steps to reproduce 
   - Expected behavior
   - Actual behavior
   - Affected language (cpp, dotnet, etc.)

# Requesting Features
Have an idea for a new algorithm or structure?
Open an Enhancement issue using the template and specify the target language.

# Code Review

When reviewing a PR:

- Focus on clarity, correctness, and style consistency.
- Suggest improvements politely.
- Use inline comments for small fixes or optimizations.

# Licensing
All contributions are made under the MIT License.
By submitting a pull request, you agree that your code will be distributed under the same terms.

Thanks again for helping make Cardio better!