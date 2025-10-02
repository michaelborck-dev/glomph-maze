# Glomph Maze - Agent Guidelines

## Build Commands
- **Build**: `make` or `make all`
- **Clean**: `make clean`
- **Install**: `make install`
- **Configure**: `./configure` (optional, run `./configure --help` for options)
- **Generate tags**: `make tags` (ctags) or `make TAGS` (etags)

## Lint/Test Commands
- **Format check**: `clang-format --dry-run --Werror src/*.c inc/*.h`
- **Format fix**: `clang-format -i src/*.c inc/*.h`
- **Lint**: `clang-tidy --fix --quiet src/*.c inc/*.h`
- **Static analysis**: `cppcheck --enable=all --inconclusive --suppress=missingInclude src/ inc/`
- **Pre-commit checks**: `pre-commit run --all-files`
- **Test**: `./glomph-maze --help` (basic smoke test)

## Changelog Commands
- **Generate changelog**: `./generate_changelog.sh` (regenerates CHANGELOG.md from git history)

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