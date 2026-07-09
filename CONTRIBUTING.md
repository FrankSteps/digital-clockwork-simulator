# Contributing

First of all, thank you for taking the time to contribute to **A Digital Clockwork Simulator**.

This project exists primarily as an educational environment for studying digital electronics, hardware simulation, and low-level software design. Contributions are welcome as long as they preserve the project's goals and coding style.

## Before Opening a Pull Request

Please make sure that:

* The project builds successfully.
* Your changes do not introduce compiler warnings or obvious regressions.
* New code follows the existing architecture whenever possible.
* Comments and documentation follow the language conventions described below.

## Code Style

The project intentionally follows a relatively simple C++ style:

* Prefer indexed `for` loops over range-based loops when working with low-level data structures.
* Use explicit types instead of `auto`.
* Always use braces on conditionals and loops, even for single-line bodies.
* Avoid unnecessary abstraction; the codebase favors clarity over cleverness.

### Naming

Try to follow the naming already used throughout the project instead of introducing a different convention.
Consistency is more important than personal preference.

### Comments

#### Regular comments

Inline comments should always be written in **English**.

Example:

```cpp
// update the clock on each rising edge
```

#### File overview comments

Large documentation blocks placed at the beginning of a source file may be bilingual.
Use the following structure:

```cpp
/*
[PT-BR]
    ...
[EN-US]
    ...
*/
```

The Portuguese section should describe the implementation for Brazilian readers, while the English section should provide the equivalent documentation for international contributors.

## Testing

There is no mandatory testing framework for contributions.
However, every Pull Request should indicate which checks were performed.

Use the checklist below when opening a PR.

```text
## Tests performed
- [ ] Project builds successfully
- [ ] Existing tests still pass
- [ ] New functionality was tested manually
- [ ] Cross-platform behavior verified (if applicable)

Additional notes:
...
```

Not every item must necessarily be checked.
For example, if your contribution only changes documentation, all boxes may remain unchecked.
The important part is honestly documenting what was (or was not) verified.

## Commit Messages

Commits follow the [Conventional Commits](https://www.conventionalcommits.org/) convention, written in English.

Example:

```bash
feat: add cascaded comparator for alarm day matching
fix: correct UTF-8 encoding on Windows console output
```

## Pull Requests

When submitting a Pull Request:

* Keep changes focused on a single topic whenever possible.
* Explain why the change is necessary.
* Include screenshots only if the contribution affects user-visible output.
* If the change modifies the behavior of the simulator, briefly describe the expected behavior.

## Platform-specific Code

Keyboard and audio implementations are intentionally separated by operating system.
If your change affects one platform implementation (`Linux`, `Windows`, or `macOS`), please verify whether equivalent changes are also required in the other platform-specific source files.
Keeping the interfaces synchronized across platforms is an important part of maintaining the project.

## Questions

If you are unsure about a design decision, feel free to open an issue before implementing a large change.
Discussion is always preferred over large pull requests that significantly alter the project's architecture.
