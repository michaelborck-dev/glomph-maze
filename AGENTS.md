# Glomph Maze - Agent Guidelines

## Build Commands

### CMake
First, install CMake: `brew install cmake`

- **Configure**: `cmake -B build -DCMAKE_BUILD_TYPE=Debug`
- **Build**: `cmake --build build` (first: ~30s, incremental: 1-2s)
- **Test**: `./build/glomph --help`
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
- **Test**: `./build/glomph --help` or `./build/glomph-xlarge --help` (basic smoke test)

## Documentation Commands
- **Generate docs**: `./scripts/generate_docs_script.sh` (Doxygen + cflow)
- **View HTML docs**: `open docs/generated/html/index.html`
- **cflow outputs**: `docs/cflow/callgraph-*.txt`

## Changelog Commands
- **Generate changelog**: `./scripts/generate_changelog.sh` (regenerates CHANGELOG.md from git history)

## Current Refactoring Status

**Branch:** refactor  
**Phase:** 3 Complete - Header modularization

### What Changed:
- ✅ Phase 0: Created `include/globals.h` - consolidates all 210+ global variable declarations
- ✅ Phase 0: All source files include `globals.h` after `utils.h`
- ✅ Phase 1: Upgraded to C17 standard (from C89/C90)
- ✅ Phase 1: Added 6 compile-time static assertions for safety
- ✅ Phase 1: Fixed SPRITE_REGISTERS constant (46 → 57)
- ✅ Phase 2: Modernized types using stdint.h and stdbool.h
- ✅ Phase 2: Changed unsigned char → uint8_t, unsigned short → uint16_t
- ✅ Phase 2: Changed int boolean flags → bool (all_dirty, nogame, paused)
- ✅ Phase 3: Split globals.h into 5 domain-specific modules
- ✅ Phase 3: Created game_state.h, sprite_state.h, maze_state.h, render_state.h, input_state.h
- ✅ Phase 3: globals.h now acts as central aggregator
- ✅ No behavior changes - pure organizational refactoring and modernization

### Architecture:
```
include/
├── globals.h        (aggregator - includes all modules)
├── game_state.h     (score, lives, level, player state)
├── sprite_state.h   (sprite registers, ghost AI)
├── maze_state.h     (maze data, dimensions, loading)
├── render_state.h   (tiles, screen, colors, pager)
├── input_state.h    (keyboard, controls, timing)
└── utils.h          (macros, constants, function prototypes)

src/
├── myman.c    (includes globals.h)
├── utils.c    (includes globals.h - defines most globals)
├── logic.c    (includes globals.h)
└── main.c     (includes globals.h)
```

**Next Phase:** Function extraction and encapsulation

See `docs/REFACTOR_PLAN.md` for full plan.

## Known Issues
None currently. Help system is working correctly.

## Code Style Guidelines

### Formatting (.clang-format)
- **Base style**: LLVM with customizations
- **Indentation**: 4 spaces
- **Line length**: 80 columns maximum
- **Pointer alignment**: Left (e.g., `int* ptr`)
- **Include sorting**: Enabled
- **Braces**: Attach style
- **Standard**: C17 (upgraded from C11)

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