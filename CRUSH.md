# CRUSH.md - Glomph Maze Development Guide

## Build Commands
```bash
# Configure the build (optional, autodetects most settings)
./configure

# Build the project
make

# Clean build artifacts
make clean

# Install to /usr/local (requires sudo)
make install

# Uninstall
make uninstall

# Run basic smoke test
./glomph-maze --help
```

## Lint/Test Commands
```bash
# Format C/C++ files with clang-format
find src/ inc/ -name '*.c' -o -name '*.h' | xargs clang-format -i

# Lint with clang-tidy (auto-fix enabled)
find src/ inc/ -name '*.c' -o -name '*.h' | xargs clang-tidy --fix --quiet

# Static analysis with cppcheck
cppcheck --enable=all --inconclusive --suppress=missingInclude src/ inc/

# Check Makefile syntax
make -n --dry-run all

# Run pre-commit hooks
pre-commit run --all-files
```

## Code Style Guidelines

### Formatting (clang-format)
- Based on LLVM style with modifications (.clang-format)
- Indent width: 4 spaces
- Column limit: 80 characters
- Pointer alignment: Left (`int* ptr`)
- Sort includes automatically
- Align consecutive assignments and declarations
- Break before braces: Attach style
- Standard: C11

### Naming Conventions
- **Functions**: `snake_case` with lowercase (e.g., `check_level_transition`)
- **Variables**: `snake_case` with lowercase (e.g., `program_name`)
- **Constants/Macros**: `UPPER_CASE` with underscores
- **Types/Structs**: `PascalCase` (rare in this codebase)
- **Files**: `snake_case.c/.h`

### Imports/Includes
- System headers first (e.g., `<stdio.h>`, `<stdlib.h>`)
- Conditional includes with `#if HAVE_*` guards for portability
- Local headers in double quotes (e.g., `"utils.h"`)
- Sort includes alphabetically (enforced by clang-format)

### Error Handling
- Use standard C error handling patterns
- Return error codes from functions
- Check system call results (errno)
- Memory allocation checks (NULL returns)

### Code Structure
- Functions defined with clear single-responsibility
- Complex logic broken into helper functions
- Comment blocks for major sections
- Portable code with conditional compilation for different platforms</content>
<parameter name="file_path">CRUSH.md