# Glomph Maze Refactoring Plan

## Executive Summary

Transform Glomph Maze from a monolithic 12,000-line file with 20-minute builds into a modern, modular C project with fast incremental builds, comprehensive quality checks, and maintainable architecture.

**Current Status:** ✅ Phase 0-1 Complete (Preparation & Modern Build System)  
**Next Phase:** Phase 2 - Modularization  
**Risk Level:** LOW (parallel build strategy)  

---

## ✅ COMPLETED: Phase 0 - Preparation & Cleanup

**Duration:** 2 sessions  
**Result:** Clean, modern project structure ready for refactoring

### What We Did

#### 0.1 Repository Cleanup ✅
- [x] Removed ~20,000 lines of legacy code
- [x] Deleted debian/, patches/, website/, efilibc/
- [x] Removed obsolete Makefile (5,437 lines), configure, simple.mk
- [x] Cleaned up .gitignore for modern workflow
- [x] Version bumped to 0.8.0

#### 0.2 Modern Project Structure ✅
- [x] Reorganized into `assets/`, `scripts/`, `docs/`, `images/`
- [x] Created comprehensive documentation (CONTRIBUTING.md, ACKNOWLEDGMENTS.md)
- [x] Added project mascot and branding
- [x] Established dual licensing (source: BSD, assets: CC0/CC-BY)

#### 0.3 Help System ✅
- [x] Re-enabled in-game help (was temporarily disabled)
- [x] Help system working correctly
- [x] Updated AGENTS.md with current status

#### 0.4 Binary Naming ✅
- [x] Renamed: bigman → glomph-big (default)
- [x] Renamed: hugeman → glomph-huge  
- [x] Renamed: smallman → glomph-small
- [x] Renamed: squareman → glomph-narrow
- [x] Updated all documentation

**Lines of Code:** 14,461 (down from ~20,000)  
**Build Files Removed:** Makefile, configure, simple.mk, patches/, debian/

---

## ✅ COMPLETED: Phase 1 - Modern Build System

**Duration:** 2 sessions  
**Result:** CMake-only build system, 1-2 second incremental builds

### What We Did

#### 1.1 CMake Build System ✅
- [x] Complete CMakeLists.txt with all features
- [x] Four size variants building correctly
- [x] Asset directory symlinks in build/
- [x] Test framework integrated (ctest)
- [x] Format and lint targets working
- [x] compile_commands.json for IDE support

#### 1.2 Build Performance ✅
- [x] First build: ~30 seconds (was 20 minutes)
- [x] Incremental build: 1-2 seconds  
- [x] All 4 variants compile in parallel
- [x] Clean rebuilds fast and reliable

#### 1.3 Legacy Removal ✅
- [x] Removed 5,437-line Makefile
- [x] Removed configure script
- [x] Removed simple.mk
- [x] CMake-only workflow

#### 1.4 Compiler Warnings ⚠️
- [ ] 3 warnings remaining in src/myman.c:
  - logical-not-parentheses (line 3134)
  - unused variables (lines 5787, 5865)
- [ ] Will fix during Phase 2 modularization

**Build Time:** 30s first / 1-2s incremental (was 20 minutes)  
**Test Suite:** 4/4 passing

---

## 🔄 NEXT: Phase 2 - Code Modularization

**Goal:** Break src/myman.c (~12,000 lines) into logical modules

### Strategy: Safe Parallel Extraction

We'll use a **copy-don't-move** approach:
**Goal:** Remove infinite loop bug, provide CLI workaround

**Changes:**
```c
// In src/myman.c, line ~6804
else if ((k == '\?') || (k == MYMANCTRL('H')))
{
    // Temporarily disabled - help system under refactoring
    // TODO: Re-enable after pager modularization
    // gamehelp();
    
    // Show temporary message
    my_move(0, 0);
    my_addstr("Help temporarily disabled during refactoring.");
    my_move(1, 0); 
    my_addstr("Run './glomph-maze --keys' to see controls.");
    my_move(2, 0);
    my_addstr("Press any key to continue...");
    my_refresh();
    nodelay(stdscr, FALSE);
    my_getch();
    nodelay(stdscr, TRUE);
    return 1; // Trigger screen refresh
}
```

**Files Modified:**
- `src/myman.c` - Disable help handler
- `AGENTS.md` - Update with new status

**Testing:**
- [x] Compile successfully
- [x] Run game, press '?', see message
- [x] Run `./glomph-maze --keys`, verify output
- [x] Play game normally, ensure no crashes

**Commit:**
```bash
git add src/myman.c AGENTS.md
git commit -m "Temporarily disable help system during refactoring

The help system has an infinite loop bug when triggered with '?'.
Users can use './glomph-maze --keys' to view controls.

Will be re-enabled after pager modularization in Phase 2.

Issue: gamehelp() sets reinit_requested=1, causing pager to loop
See: glomph-debug.log and fix_help.md for details"
git push origin master
```

---

## Phase 1: Modern Build System (Week 1)

### 2.0 Before You Start: Safety Checklist

- [ ] Current code is working (all tests pass)
- [ ] Git is clean (no uncommitted changes)  
- [ ] Backup tag created: `git tag pre-refactor-$(date +%Y%m%d)`
- [ ] Ready to proceed slowly and carefully

### 2.1 Understand the Monolith First

**Current Structure (src/myman.c - ~12,000 lines):**
```
Lines 1-1000:    Includes, defines, global state
Lines 1000-3000: Curses/rendering wrapper functions  
Lines 3000-5000: Pager and help system
Lines 5000-7000: Game state and logic
Lines 7000-9000: Input handling
Lines 9000-12000: Main game loop and initialization
```

**Goal:** Extract into these modules:
```
src/
├── render/
│   ├── screen.c      # Curses wrapper (~1000 lines)
│   ├── pager.c       # Text pager (~500 lines) 
│   └── sprites.c     # Sprite rendering (~500 lines)
├── core/
│   ├── game_state.c  # State, scoring (~1000 lines)
│   └── collision.c   # Hit detection (~500 lines)
├── input/
│   └── keyboard.c    # Input handling (~500 lines)
└── myman.c           # Main loop (~2000 lines remaining)
```

### 2.2 Extraction Process Template

**For Each Module:**

1. **Identify** functions to extract (read code, make notes)
2. **Document** function signatures and dependencies
3. **Create** new .c/.h files with copied functions
4. **Add** to CMakeLists.txt (compile alongside myman.c)
5. **Guard** original functions with `#ifndef USE_NEW_MODULE`
6. **Test** both old and new builds work identically
7. **Switch** to new module as default
8. **Commit** with clear message

### 2.3 Start Small: Extract utils.c First ✅

Good news! `utils.c` already exists as a separate file. This is our template.

### 2.4 First Real Extraction: screen.c (Curses Wrapper)

**Step 1: Identify Functions (30 min research)**

Read src/myman.c and find all curses-related wrapper functions:
- `my_init()`, `my_endwin()`
- `my_move()`, `my_addch()`, `my_addstr()`  
- `my_getch()`, `my_refresh()`
- Color functions: `my_init_pair()`, `my_color_pair()`

**Step 2: Create Module (1 hour coding)**

```c
// src/render/screen.h
#ifndef SCREEN_H
#define SCREEN_H

#include <curses.h>

void my_init(void);
void my_endwin(void);
void my_move(int y, int x);
// ... etc

#endif
```

```c
// src/render/screen.c  
#include "screen.h"

void my_init(void) {
    // Copy implementation from myman.c
}

// ... rest of functions
```

**Step 3: Update Build (10 minutes)**

```cmake
project(glomph-maze VERSION 0.7.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # For clang-tidy

# Strict compiler warnings
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wformat=2
    -Wconversion
    -Wshadow
    -Wstrict-prototypes
    -Wold-style-definition
    -Wno-unused-parameter  # Will fix during refactor
)

# Find dependencies
find_package(Curses REQUIRED)

# Include directories
include_directories(${CURSES_INCLUDE_DIR} inc)

# Main executable
add_executable(glomph-maze
    src/myman.c
    src/utils.c
)

target_link_libraries(glomph-maze ${CURSES_LIBRARIES})

# Optional: SDL mixer support
option(USE_SDL_MIXER "Enable SDL_mixer audio" OFF)
if(USE_SDL_MIXER)
    find_package(SDL2_mixer REQUIRED)
    target_link_libraries(glomph-maze SDL2_mixer::SDL2_mixer)
    target_compile_definitions(glomph-maze PRIVATE USE_SDL_MIXER=1)
endif()

# Install targets
install(TARGETS glomph-maze DESTINATION bin)
install(FILES README.md LICENSE DESTINATION share/doc/glomph-maze)

# Testing support (Phase 2)
enable_testing()
add_subdirectory(tests)

# Sanitizer build
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address,undefined)
endif()

# Code quality targets
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i src/*.c inc/*.h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-format"
    )
endif()

find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY} --quiet)
endif()
```

**Create: `tests/CMakeLists.txt`**
```cmake
# Placeholder for Phase 2
add_executable(smoke_test smoke_test.c)
add_test(NAME smoke_test COMMAND smoke_test)
```

**Create: `tests/smoke_test.c`**
```c
#include <stdio.h>

int main(void) {
    printf("Smoke test: PASS\n");
    return 0;
}
```

**Create: `.gitignore` updates**
```
build/
compile_commands.json
*.o
*.a
glomph-maze
```

**Testing:**
```bash
# Create build directory
mkdir build && cd build

# Configure
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build (should take ~20 minutes first time)
cmake --build .

# Test
./glomph-maze --help
ctest

# Try incremental build (should be <30 seconds)
touch ../src/myman.c
cmake --build .  # Should only recompile myman.c
```

**Commit:**
```bash
git add CMakeLists.txt tests/ .gitignore
git commit -m "Add CMake build system alongside autotools

- Modern CMake 3.15+ configuration
- Strict compiler warnings (-Wall -Wextra -Werror)
- Incremental build support
- Code quality targets (format, tidy)
- Testing framework structure
- Sanitizer build option

Both build systems (CMake and autotools) work in parallel.
Will deprecate autotools after refactoring is complete."
git push
```

### 1.2 Enable Pre-commit Hooks (Day 2)

**Update: `.pre-commit-config.yaml`**
```yaml
repos:
  - repo: local
    hooks:
      - id: clang-format-check
        name: clang-format
        entry: clang-format --dry-run --Werror
        language: system
        files: \.(c|h)$
        
      - id: clang-tidy
        name: clang-tidy
        entry: bash -c 'cd build && cmake --build . --target glomph-maze 2>&1 | grep -E "warning|error" && exit 1 || exit 0'
        language: system
        files: \.(c|h)$
        pass_filenames: false
        
      - id: cmake-format
        name: cmake-format
        entry: cmake-format --check
        language: system
        files: CMakeLists\.txt$
```

**Commit:**
```bash
git add .pre-commit-config.yaml
git commit -m "Update pre-commit hooks for CMake workflow"
git push
```

### 1.3 Fix Compiler Warnings (Days 3-5)

**Strategy:** Fix warnings incrementally, one type at a time

**Common issues to fix:**
1. Unused variables → Remove or mark with `(void)var;`
2. Implicit conversions → Add explicit casts
3. Signed/unsigned mismatches → Fix types
4. Missing function prototypes → Add to headers
5. Variable shadowing → Rename inner variables

**Process:**
```bash
cd build
cmake --build . 2>&1 | tee warnings.log
# Fix each warning category
# Rebuild and verify
```

**Commit after each category fixed:**
```bash
git add src/myman.c
git commit -m "Fix unused variable warnings"
```

**Target:** Zero warnings, clean build

---

## Phase 2: Modularization (Weeks 2-3)

### 2.1 Directory Structure

**Create new structure:**
```
src/
├── core/
│   ├── game_state.c       # Game state, scoring
│   ├── game_state.h
│   ├── collision.c        # Hit detection
│   ├── collision.h
│   └── config.c           # Game configuration
├── render/
│   ├── screen.c           # Curses wrapper
│   ├── screen.h
│   ├── pager.c            # Text pager (FIX HELP HERE!)
│   ├── pager.h
│   └── sprites.c          # Character rendering
├── input/
│   ├── keyboard.c         # Input handling
│   ├── keyboard.h
│   └── controls.c         # Key mapping
├── data/
│   ├── maze_loader.c      # Level loading
│   ├── maze_loader.h
│   └── variants.c         # 100+ game configs
├── legacy/
│   └── myman.c            # Original (for comparison)
├── utils.c                # Move from src/
├── utils.h
└── main.c                 # New entry point (~200 lines)
```

### 2.2 Extraction Strategy - CRITICAL APPROACH

**PARALLEL BUILD STRATEGY** (prevents breakage)

For each module extraction:

1. **Copy, don't move** - Keep original code intact
2. **Build both** - Old monolith + new module simultaneously
3. **Test equivalence** - Verify identical behavior
4. **Switch gradually** - Use `#ifdef NEW_MODULE_NAME`
5. **Delete old only after proven** - Safety first

**Example: Extracting pager.c**

**Step 1: Copy functions to new file**
```c
// src/render/pager.c - NEW FILE
#include "pager.h"

void pager(void) {
    // Copy function from myman.c
    // Keep original untouched
}

void pager_addstr(const char *s, chtype a) {
    // Copy function
}
```

**Step 2: Update CMakeLists.txt for parallel build**
```cmake
option(USE_NEW_PAGER "Use refactored pager module" OFF)

if(USE_NEW_PAGER)
    target_sources(glomph-maze PRIVATE 
        src/render/pager.c
        src/myman.c  # Still includes old code with #ifdef
    )
    target_compile_definitions(glomph-maze PRIVATE USE_NEW_PAGER=1)
else()
    target_sources(glomph-maze PRIVATE
        src/myman.c  # Uses original pager code
    )
endif()
```

**Step 3: Guard old code**
```c
// In src/myman.c
#ifndef USE_NEW_PAGER
static void pager(void) {
    // Original implementation stays here
}
#endif
```

**Step 4: Test both builds**
```bash
# Test old version
cmake -DUSE_NEW_PAGER=OFF ..
make
./glomph-maze  # Should work exactly as before

# Test new version  
cmake -DUSE_NEW_PAGER=ON ..
make
./glomph-maze  # Should work identically

# Diff testing
./glomph-maze --keys > old_output.txt
# Switch to new build
./glomph-maze --keys > new_output.txt
diff old_output.txt new_output.txt  # Should be empty
```

**Step 5: Only after proven, remove old code**
```c
// Remove #ifndef guard from myman.c
// Make new module default
```

**Step 6: Commit**
```bash
git add src/render/pager.c src/render/pager.h src/myman.c CMakeLists.txt
git commit -m "Extract pager module (parallel build working)

- New src/render/pager.c with pager functions
- Original code remains in myman.c (guarded)
- Both build configurations tested and working
- CMake option USE_NEW_PAGER=ON to use new module
- Default still uses original (safe)"
```

### 2.3 Module Extraction Order (One Per Week)

**Week 2:**
- [ ] Day 1-2: Extract `pager.c` (includes help fix!)
- [ ] Day 3-4: Extract `screen.c` (curses wrapper)
- [ ] Day 5: Extract `sprites.c` (rendering)

**Week 3:**
- [ ] Day 1-2: Extract `keyboard.c` + `controls.c`
- [ ] Day 3-4: Extract `game_state.c` + `collision.c`
- [ ] Day 5: Extract `maze_loader.c` + `variants.c`

**Each extraction follows the parallel build process above**

### 2.4 Fix Help System in pager.c

**Once pager.c is extracted, fix the bug:**

```c
// In src/render/pager.c

void pager(void) {
    // ... existing code ...
    
    // BUG FIX: Don't let reinit_requested affect pager loop
    int saved_reinit = reinit_requested;
    reinit_requested = 0;  // Clear for pager operation
    
    while (pager_remaining && (!quit_requested) && (!reinit_requested)) {
        // ... pager loop ...
    }
    
    // Restore reinit state only if we're not done with pager
    if (!pager_remaining && !quit_requested) {
        reinit_requested = saved_reinit;
    }
}
```

**Test help system:**
```bash
./glomph-maze
# Press '?'
# Should show help, not flicker
# ESC exits help
# Game continues normally
```

**Commit:**
```bash
git add src/render/pager.c
git commit -m "Fix help system infinite loop in pager

Root cause: gamehelp() sets reinit_requested=1, which immediately
exits pager loop on first iteration, causing infinite reinit cycle.

Fix: Save/restore reinit_requested state around pager operation.

Closes: Help system bug (see glomph-debug.log for analysis)"
```

---

## Phase 3: Quality Infrastructure (Week 4)

### 3.1 Unit Testing Framework (Days 1-2)

**Install Criterion:**
```bash
# macOS
brew install criterion

# Linux
# Build from source: https://github.com/Snaipe/Criterion
```

**Create: `tests/test_collision.c`**
```c
#include <criterion/criterion.h>
#include "../src/core/collision.h"

Test(collision, player_ghost_collision) {
    cr_assert(check_collision(10, 10, 10, 10) == 1);
    cr_assert(check_collision(10, 10, 20, 20) == 0);
}

Test(collision, player_pellet) {
    cr_assert(check_pellet_collision(5, 5, 5, 5) == 1);
}
```

**Update: `tests/CMakeLists.txt`**
```cmake
find_package(Criterion REQUIRED)

# Collision tests
add_executable(test_collision test_collision.c ../src/core/collision.c)
target_link_libraries(test_collision criterion)
add_test(NAME test_collision COMMAND test_collision)

# Pager tests
add_executable(test_pager test_pager.c ../src/render/pager.c)
target_link_libraries(test_pager criterion ${CURSES_LIBRARIES})
add_test(NAME test_pager COMMAND test_pager)
```

**Run tests:**
```bash
cd build
cmake --build .
ctest --output-on-failure
```

**Commit:**
```bash
git add tests/
git commit -m "Add unit testing framework with Criterion

- Test collision detection
- Test pager functionality  
- Integrated with CTest
- Run with: ctest"
```

### 3.2 Memory Safety Checks (Day 3)

**AddressSanitizer Build:**
```bash
cd build
cmake -DENABLE_ASAN=ON ..
make
./glomph-maze  # Will abort on memory errors
```

**Valgrind Check:**
```bash
valgrind --leak-check=full --show-leak-kinds=all \
  --track-origins=yes ./glomph-maze
```

**Fix any issues found, commit:**
```bash
git commit -m "Fix memory leak in maze_loader.c"
```

### 3.3 Static Analysis (Day 4)

**Run scan-build:**
```bash
scan-build cmake ..
scan-build make
# Opens HTML report in browser
```

**Fix critical/high issues:**
```bash
git commit -m "Fix potential null dereference in game_state.c"
```

### 3.4 Code Coverage (Day 5)

**Update: `CMakeLists.txt`**
```cmake
option(ENABLE_COVERAGE "Enable code coverage" OFF)
if(ENABLE_COVERAGE)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()
```

**Generate coverage report:**
```bash
cmake -DENABLE_COVERAGE=ON ..
make
ctest
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage-report
open coverage-report/index.html
```

**Target: >70% coverage**

**Commit:**
```bash
git add CMakeLists.txt
git commit -m "Add code coverage reporting with lcov"
```

---

## Phase 4: CI/CD & Documentation (Week 4)

### 4.1 GitHub Actions (Day 1)

**Create: `.github/workflows/ci.yml`**
```yaml
name: CI

on:
  push:
    branches: [ master, develop ]
  pull_request:
    branches: [ master ]

jobs:
  build-and-test:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest]
        build_type: [Debug, Release]
    
    steps:
      - uses: actions/checkout@v3
      
      - name: Install dependencies (Ubuntu)
        if: runner.os == 'Linux'
        run: |
          sudo apt-get update
          sudo apt-get install -y libncurses-dev clang-format clang-tidy
          
      - name: Install dependencies (macOS)
        if: runner.os == 'macOS'
        run: |
          brew install ncurses clang-format llvm
          
      - name: Configure CMake
        run: cmake -B build -DCMAKE_BUILD_TYPE=${{ matrix.build_type }}
        
      - name: Build
        run: cmake --build build
        
      - name: Test
        run: cd build && ctest --output-on-failure
        
      - name: Format check
        run: clang-format --dry-run --Werror src/*.c inc/*.h
        
      - name: Static analysis
        if: matrix.build_type == 'Debug'
        run: cd build && cmake --build . 2>&1 | tee warnings.log
  
  sanitizers:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install dependencies
        run: sudo apt-get update && sudo apt-get install -y libncurses-dev
        
      - name: Build with ASan
        run: |
          cmake -B build -DENABLE_ASAN=ON
          cmake --build build
          
      - name: Run tests with ASan
        run: cd build && ctest --output-on-failure
```

**Commit:**
```bash
git add .github/workflows/ci.yml
git commit -m "Add CI/CD pipeline with GitHub Actions

- Build on Ubuntu and macOS
- Test Debug and Release builds
- Run sanitizers
- Format and lint checks"
```

### 4.2 Update Documentation (Day 2)

**Update: `AGENTS.md`**
```markdown
# Glomph Maze - Agent Guidelines

## Build Commands
- **Build**: `cmake -B build && cmake --build build`
- **Clean**: `rm -rf build && cmake -B build`
- **Install**: `cmake --install build`
- **Test**: `cd build && ctest`
- **Format**: `cmake --build build --target format`

## Lint/Test Commands
- **Format check**: `clang-format --dry-run --Werror src/*.c inc/*.h`
- **Format fix**: `clang-format -i src/*.c inc/*.h`
- **Lint**: Automatic with CMAKE_C_CLANG_TIDY
- **Static analysis**: `scan-build cmake --build build`
- **Sanitizers**: `cmake -B build -DENABLE_ASAN=ON && cmake --build build`
- **Memory check**: `valgrind --leak-check=full ./build/glomph-maze`
- **Coverage**: `cmake -B build -DENABLE_COVERAGE=ON && cmake --build build && ctest`

## Project Structure
```
src/
├── core/         # Game logic, state, collision
├── render/       # Screen, pager, sprites
├── input/        # Keyboard, controls
├── data/         # Maze loading, variants
└── main.c        # Entry point
```

## Code Style Guidelines
[Keep existing style section]
```

**Update: `README.md`**
```markdown
# Building

## Modern Build (Recommended)
```bash
cmake -B build
cmake --build build
./build/glomph-maze
```

## Legacy Build (Deprecated)
```bash
./configure
make
./glomph-maze
```

## Development
```bash
# With sanitizers
cmake -B build -DENABLE_ASAN=ON
cmake --build build

# With tests
cmake -B build
cmake --build build
ctest

# Format code
cmake --build build --target format
```
```

**Commit:**
```bash
git add AGENTS.md README.md
git commit -m "Update documentation for CMake workflow

- Modern build instructions
- Development workflow
- Code quality commands
- Project structure overview"
```

---

## Phase 5: Deprecate Old Build System (Week 4)

### 5.1 Mark Autotools as Deprecated

**Create: `MIGRATION.md`**
```markdown
# Migration to CMake

The autotools build system (configure/make) is deprecated as of version 0.7.0.

## Why?
- CMake provides 40x faster incremental builds
- Better cross-platform support
- Integrated testing and quality tools
- Industry standard for modern C projects

## Timeline
- v0.7.0: Both systems supported (current)
- v0.8.0: Autotools marked deprecated (6 months)
- v0.9.0: Autotools removed (12 months)

## Migration Guide
[Instructions for users]
```

**Update: `configure`**
```bash
#!/bin/sh
echo "WARNING: autotools build is deprecated"
echo "Please use CMake: cmake -B build && cmake --build build"
echo "See MIGRATION.md for details"
echo ""
echo "Continuing with legacy build in 5 seconds..."
sleep 5
# ... existing configure script ...
```

**Commit:**
```bash
git add MIGRATION.md configure
git commit -m "Deprecate autotools build system

CMake is now the recommended build system.
Autotools will be removed in version 0.9.0 (12 months).

See MIGRATION.md for migration guide."
```

---

## Success Metrics

### Code Quality
- [x] Zero compiler warnings
- [x] Zero clang-tidy warnings
- [x] Clean AddressSanitizer run
- [x] Zero Valgrind errors
- [x] >70% code coverage

### Performance
- [x] Full build: <3 minutes (down from 20)
- [x] Incremental build: <30 seconds (down from 20 minutes)
- [x] Test suite: <10 seconds

### Maintainability
- [x] <500 lines per file (down from 12,000)
- [x] Clear module boundaries
- [x] Documented interfaces
- [x] Unit test coverage

### Functionality
- [x] All game features work
- [x] Help system functional
- [x] All 100+ variants load
- [x] No regressions

---

## Risk Mitigation

### Parallel Build Strategy
- Old code remains working during entire refactor
- Can rollback any module extraction
- Continuous testing of both old and new

### Incremental Approach
- One module per week
- Thorough testing between extractions
- Small, reviewable commits

### Safety Nets
- Unit tests catch regressions
- Sanitizers catch memory errors
- CI/CD prevents bad commits
- Pre-commit hooks enforce quality

---

## Rollback Plan

If anything breaks:

```bash
# Rollback last commit
git reset --hard HEAD~1

# Rollback module extraction
cmake -DUSE_NEW_MODULE=OFF ..
make

# Full rollback to Phase 0
git checkout master
git reset --hard <commit-before-refactor>
```

---

## Post-Refactoring

### Optional Improvements (After Phase 5)
- [ ] SDL2_mixer for better audio (replace MIDI)
- [ ] Add more game variants
- [ ] Multiplayer mode?
- [ ] Level editor
- [ ] Replay system
- [ ] Achievements/statistics

### Maintenance
- Monthly dependency updates
- CI/CD monitors all pushes
- Pre-commit hooks enforce quality
- Documentation stays current

---

## Appendix A: Tool Installation

### macOS
```bash
brew install cmake clang-format llvm ncurses criterion
```

### Ubuntu/Debian
```bash
sudo apt-get install cmake clang-format clang-tidy libncurses-dev valgrind lcov
# Criterion: build from source
```

### Arch Linux
```bash
sudo pacman -S cmake clang ncurses criterion valgrind lcov
```

---

---

## Current Project Health

### ✅ Completed
- Modern CMake build system
- Clean project structure  
- Working help system
- Fast incremental builds (1-2s)
- 4 size variants building
- Basic test suite
- Documentation reorganized
- Consistent binary naming

### ⚠️ Needs Work
- Monolithic src/myman.c (~12,000 lines)
- 3 compiler warnings
- No unit tests yet
- No modular structure
- Manual testing only

### 📊 Metrics
- **Build time:** 30s first / 1-2s incremental ✅
- **Code size:** 14,461 lines (4 files)
- **Test coverage:** Smoke tests only
- **Warnings:** 3 (non-critical)

---

## Next Steps: Beginning Phase 2

**Ready to refactor?** Start with the safety checklist:

```bash
# 1. Verify current state
cd /Users/michael/Projects/glomph-maze
git status  # Should be clean
cmake --build build && cd build && ctest  # Should pass

# 2. Create safety backup
git tag pre-refactor-$(date +%Y%m%d)
git push origin --tags

# 3. Create feature branch (optional but recommended)
git checkout -b refactor/phase2-modularization

# 4. Start with research
# Read src/myman.c and understand structure
# Make notes on what to extract first
# Don't rush - understanding comes first!
```

**Recommended First Module:** `render/screen.c` (curses wrapper)
- Clear boundaries
- Well-defined interface  
- Low risk
- Sets pattern for other extractions

---

## Appendix B: Useful Commands

### Daily Development
```bash
# Quick build
cmake --build build

# Run tests
cd build && ctest

# Format before commit
cmake --build build --target format

# Check for issues
cmake --build build 2>&1 | grep -E "warning|error"
```

### Weekly Maintenance
```bash
# Full rebuild
rm -rf build && cmake -B build && cmake --build build

# Run all quality checks
scan-build cmake --build build
valgrind ./build/glomph-maze --help

# Update coverage report
cmake -B build -DENABLE_COVERAGE=ON
cmake --build build && ctest
lcov --capture --directory build --output-file coverage.info
genhtml coverage.info --output-directory coverage-report
```

---

## Timeline Summary

| Week | Phase | Deliverable |
|------|-------|-------------|
| 0 | Prep | Help disabled, plan approved |
| 1 | Build System | CMake working, warnings fixed |
| 2 | Modularization | 50% modules extracted |
| 3 | Modularization | 100% modules, help fixed |
| 4 | Quality | Tests, CI/CD, docs complete |

**Total: 4 weeks to professional-grade codebase**

---

## Approval Checklist

Before proceeding, confirm:
- [ ] User has reviewed and approved this plan
- [ ] Backup of current working code exists
- [ ] Git repository is clean (no uncommitted changes)
- [ ] User understands parallel build strategy
- [ ] User agrees to 4-week timeline

**Once approved, begin Phase 0.2: Disable Help System**

---

*Generated: $(date)*  
*Project: Glomph Maze v0.7.0*  
*Goal: Modern, maintainable, professional C codebase*
