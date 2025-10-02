# Glomph Maze - Agent Guidelines

## Build Commands

### CMake
First, install CMake: `brew install cmake`

- **Configure**: `cmake -B build -DCMAKE_BUILD_TYPE=Debug`
- **Build**: `cmake --build build` (first: ~30s, incremental: 1-2s)
- **Test**: `./build/bigman --help`
- **Clean**: `rm -rf build`
- **Format**: `cmake --build build --target format`
- **Lint**: `cmake --build build --target lint`
- **Run tests**: `cd build && ctest`

See `CMAKE_SETUP.md` for details.

## Lint/Test Commands
- **Format check**: `clang-format --dry-run --Werror src/*.c inc/*.h`
- **Format fix**: `clang-format -i src/*.c inc/*.h`
- **Lint**: `clang-tidy --fix --quiet src/*.c inc/*.h`
- **Static analysis**: `cppcheck --enable=all --inconclusive --suppress=missingInclude src/ inc/`
- **Test**: `./build/bigman --help` or `./build/hugeman --help` (basic smoke test)

## Changelog Commands
- **Generate changelog**: `./scripts/generate_changelog.sh` (regenerates CHANGELOG.md from git history)

## Known Issues
None currently. The help system has been re-enabled and is working correctly.

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

### General Conventions
- **Language**: ANSI C with C11 features
- **Headers**: BSD license blocks at top of each file
- **Includes**: System headers first, then local headers with guards
- **Conditional compilation**: Extensive use for portability across platforms
- **Error handling**: Standard C errno-based error handling
- **Naming**: Standard C conventions (snake_case for functions/variables)
- **Comments**: Block comments for file headers, inline comments as needed
- **No emojis or decorative elements**