# Glomph Maze - Agent Guidelines

## Build Commands

### CMake (Recommended)
First, install CMake: `brew install cmake`

- **Configure**: `cmake -B build -DCMAKE_BUILD_TYPE=Debug`
- **Build**: `cmake --build build` (first: ~30s, incremental: 1-2s)
- **Test**: `./build/bigman --help`
- **Clean**: `rm -rf build`
- **Format**: `cmake --build build --target format`
- **Lint**: `cmake --build build --target lint`
- **Run tests**: `cd build && ctest`

### Simple Makefile (Alternative)
- **Fast build**: `make -f simple.mk` (builds in 1-2 seconds)
- **Clean**: `make -f simple.mk clean`

### Legacy Makefile (Avoid)
- **Slow build**: `make` (20+ minutes - avoid unless necessary)
- **Configure**: `./configure` (only for legacy build)

### Build System Status
- ✅ **CMake**: Modern, fast incremental builds, testing, IDE support
- ✅ **simple.mk**: Fast but no incremental builds (always rebuilds everything)
- ❌ **Main Makefile**: 5,437 lines of GNU Make metaprogramming - extremely slow
- See `BUILD_SYSTEM_FIX.md` and `CMAKE_SETUP.md` for details

## Lint/Test Commands
- **Format check**: `clang-format --dry-run --Werror src/*.c inc/*.h`
- **Format fix**: `clang-format -i src/*.c inc/*.h`
- **Lint**: `clang-tidy --fix --quiet src/*.c inc/*.h`
- **Static analysis**: `cppcheck --enable=all --inconclusive --suppress=missingInclude src/ inc/`
- **Pre-commit checks**: `pre-commit run --all-files`
- **Test**: `./bigman --help` or `./hugeman --help` (basic smoke test after simple.mk build)

## Changelog Commands
- **Generate changelog**: `./generate_changelog.sh` (regenerates CHANGELOG.md from git history)

## Known Issues
- **Build System**: Main `Makefile` is extremely slow (20+ minutes)
  - **Solution**: Use `make -f simple.mk` for fast builds (1-2 seconds)
  - See BUILD_SYSTEM_FIX.md for details
- **Help System**: Temporarily disabled to prevent infinite loop bug
  - Pressing `?` or `Ctrl-H` shows a message: "Help temporarily disabled during refactoring"
  - Use `./bigman --keys` to view controls
  - Will be re-enabled after pager modularization (Phase 2.4 of REFACTOR_PLAN.md)

## Code Style Guidelines

### Formatting (.clang-format)
- **Base style**: LLVM with customizations
- **Indentation**: 4 spaces
- **Line length**: 80 columns maximum
- **Pointer alignment**: Left (e.g., `int* ptr`)
- **Include sorting**: Enabled
- **Braces**: Attach style
- **Standard**: C11

### Linting (.clang-tidy)
- **Enabled checks**: clang-analyzer-*, modernize-*, cppcoreguidelines-*
- **Disabled checks**: modernize-use-trailing-return-type, cppcoreguidelines-avoid-magic-numbers, cppcoreguidelines-non-private-member-variables-in-classes

### General Conventions
- **Language**: ANSI C with C11 features
- **Headers**: BSD license blocks at top of each file
- **Includes**: System headers first, then local headers with guards
- **Conditional compilation**: Extensive use for portability across platforms
- **Error handling**: Standard C errno-based error handling
- **Naming**: Standard C conventions (snake_case for functions/variables)
- **Comments**: Block comments for file headers, inline comments as needed
- **No emojis or decorative elements**

### Pre-commit Hooks
- Trailing whitespace removal
- End-of-file fixes
- YAML validation
- Large file checks
- clang-format enforcement
- clang-tidy fixes
- cppcheck static analysis
- Makefile syntax validation