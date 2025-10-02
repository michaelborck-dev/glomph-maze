# CMake vs. Simple Makefile - Decision Guide

## Current Situation

### The Complex Makefile
- **5,437 lines** of GNU Make metaprogramming
- Supports **234+ game variants** (myman, pac-man, ghost, etc.)
- Supports **237 level files** in `lvl/`
- Supports **38 tilesets** in `chr/`
- **20+ minute builds** due to complexity
- Cross-platform: DOS, Windows, Mac, Unix, EFI, embedded systems

### The Simple Makefile
- **116 lines** of straightforward Make
- Supports **4 size variants** (huge, big, small, square)
- Supports **1 game variant** (myman/glomph-maze)
- **1-2 second builds**
- Cross-platform: Any Unix-like with curses

### The Question
**Do we need 234 game variants, or can we simplify?**

## What You Gain with CMake

### 1. **Incremental Builds** (Primary Benefit)
```bash
# First build: ~30 seconds (vs 20 minutes with old Makefile)
cmake -B build && cmake --build build

# Change one file: ~1-2 seconds
touch src/utils.c
cmake --build build  # Only recompiles utils.c
```

CMake tracks dependencies properly, so changing `utils.c` doesn't rebuild everything.

### 2. **Modern Tooling**
- `compile_commands.json` for IDE integration (VSCode, CLion)
- Built-in test framework (`ctest`)
- Sanitizer support (`-DENABLE_ASAN=ON`)
- Cross-platform without shell scripts

### 3. **Code Quality Integration**
```bash
cmake --build build --target format    # clang-format
cmake --build build --target lint      # clang-tidy
ctest                                   # Run tests
```

### 4. **Better Portability**
CMake handles platform differences:
- Finds curses/ncurses/pdcurses automatically
- Detects SDL_mixer if available
- Works on Windows without Cygwin/MinGW hacks
- Generates Xcode/Visual Studio projects

### 5. **Professional Development Workflow**
```bash
# Debug build
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Release build
cmake -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release

# With sanitizers
cmake -B build-asan -DENABLE_ASAN=ON
cmake --build build-asan && ./build-asan/glomph-maze
```

## What You DON'T Gain

### 1. **Multiple Game Variants**
The old Makefile builds 234 different Pac-Man clones:
- `myman` (original)
- `pacman`, `mspacman`, `ghostman`, `catchum`, etc.

**CMake won't magically preserve these.** You'd need to:
- Manually enumerate them (tedious)
- OR generate them with a script (defeats simplicity)
- OR drop them entirely

### 2. **Exotic Platform Support**
The old Makefile supports:
- DOS (DJGPP, EMX, Cygwin)
- Windows (MinGW, native)
- Mac Classic (pre-OSX)
- EFI
- Embedded systems

**Question:** Do you need these in 2025?

### 3. **Binary Distribution Generation**
The old Makefile can create:
- `.tar.gz` source releases
- `.zip` Windows binaries
- `.dmg` Mac disk images
- Debian packages

CMake has `CPack` for this, but you'd need to set it up.

## The Real Question: Variants

You have **234 game variants** defined. Examples:
```
myman       - Original
pacman      - Classic Pac-Man
mspacman    - Ms. Pac-Man
ghost       - Ghost mode
catchum     - Catch'em
chomp       - Chomp
pac64       - C64 style
...
```

### Option A: Keep All Variants (Complex)
**Pros:**
- Preserves the game's unique feature (100+ Pac-Man clones!)
- Historical completeness
- Users can play any variant

**Cons:**
- CMake becomes more complex
- Need to enumerate or generate 234 targets
- Longer initial builds (still faster than old Makefile)
- More testing needed

**Implementation:**
```cmake
# Either manually list all 234:
add_executable(myman src/myman.c src/utils.c)
add_executable(pacman src/myman.c src/utils.c)
# ... 232 more ...

# OR generate with a loop:
set(VARIANTS myman pacman mspacman ghost ...)
foreach(variant ${VARIANTS})
    add_executable(${variant} src/myman.c src/utils.c)
    target_compile_definitions(${variant} PRIVATE VARIANT="${variant}")
endforeach()
```

### Option B: Keep Only Core Variants (Simple)
**Pros:**
- Clean, maintainable CMake
- Fast builds (1-2 seconds)
- Easy to understand
- Can add more later if needed

**Cons:**
- Loses unique feature
- Users can't play all the variants
- Need to pick which ones to keep

**Implementation:**
```cmake
# Keep the most popular:
add_executable(glomph-maze-huge src/myman.c src/utils.c)  # Huge tiles
add_executable(glomph-maze-big src/myman.c src/utils.c)   # Big tiles
add_executable(glomph-maze src/myman.c src/utils.c)       # Normal
add_executable(glomph-maze-mini src/myman.c src/utils.c)  # Tiny

# Configure with defines:
target_compile_definitions(glomph-maze-huge PRIVATE SIZE="huge")
```

### Option C: Runtime Variant Selection (Best of Both?)
**Pros:**
- Single binary
- All variants available via command-line flags
- Simplest CMake
- Users get all features

**Cons:**
- Requires code refactoring
- Data files loaded at runtime (slower startup)
- More complex game logic

**Implementation:**
```bash
# Instead of 234 binaries:
./glomph-maze --variant=pacman --size=big
./glomph-maze --variant=ghost --size=small

# OR use symlinks:
ln -s glomph-maze pacman
./pacman  # Detects name, loads pacman variant
```

This is **actually what `src/main.c` does** - it's a variant selector!

## My Recommendation

### Phase 1: Start Simple, Add Complexity Later

**Step 1: Create minimal CMake (TODAY - 2 hours)**
```cmake
cmake_minimum_required(VERSION 3.15)
project(glomph-maze VERSION 0.7.0 LANGUAGES C)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR} inc mygetopt)

# Single binary with all variants (via runtime selection)
add_executable(glomph-maze
    src/myman.c
    src/utils.c
    src/logic.c
    mygetopt/mygetopt.c
)
target_link_libraries(glomph-maze ${CURSES_LIBRARIES})

# Plus size variants (if you want separate binaries)
foreach(size huge big small square)
    add_executable(${size}man
        src/myman.c
        src/utils.c
        src/logic.c
        mygetopt/mygetopt.c
    )
    target_compile_definitions(${size}man PRIVATE MYMANSIZE="${size}")
    target_link_libraries(${size}man ${CURSES_LIBRARIES})
endforeach()
```

**Benefits:**
- ✅ Fast builds (1-2 seconds)
- ✅ Incremental compilation
- ✅ Modern tooling
- ✅ Can add variants later
- ✅ Compatible with refactoring plan

**Step 2: After Code Refactoring**
Once you've modularized the code (Phase 2), revisit variants:
- Data-driven variant loading (best)
- OR CMake generator script
- OR keep it simple with 4-5 main variants

### Comparison Table

| Feature | Old Makefile | simple.mk | CMake (Minimal) | CMake (Full) |
|---------|--------------|-----------|-----------------|--------------|
| Build time (clean) | 20+ min | 1.2 sec | 30 sec | 2 min |
| Build time (incremental) | 20+ min | 1.2 sec | 1-2 sec | 5-10 sec |
| Game variants | 234 | 1 | 1-4 | 234 |
| Size variants | 10+ | 4 | 4 | 10+ |
| Platforms | All | Unix | All modern | All modern |
| Maintainability | ❌ Terrible | ✅ Good | ✅ Excellent | ⚠️ Medium |
| IDE support | ❌ No | ❌ No | ✅ Yes | ✅ Yes |
| Testing | ❌ No | ❌ No | ✅ Built-in | ✅ Built-in |
| Code quality | ❌ No | ❌ No | ✅ Integrated | ✅ Integrated |

## My Specific Recommendation

**For your situation:**

1. **Create a simple CMake** that matches `simple.mk` functionality
   - 4 size variants (huge, big, small, square)
   - 1 game variant (glomph-maze)
   - Fast builds
   - Modern tooling

2. **Forget the 234 variants for now**
   - They're compiled-in data files
   - After refactoring, make them runtime-loadable
   - Then one binary can access all variants

3. **Keep old Makefile as reference**
   - Don't delete it
   - Mark it deprecated
   - Use for generating release tarballs if needed

4. **Focus on code quality**
   - CMake enables testing, sanitizers, linting
   - These are MORE valuable than 234 variants
   - Better code → easier to add variants later

## Action Plan

```bash
# TODAY (2 hours)
# 1. Create minimal CMakeLists.txt (I'll help)
# 2. Test it works
cmake -B build
cmake --build build
./build/bigman

# 3. Update AGENTS.md
# 4. Commit

# LATER (after code refactoring)
# 5. Make variants runtime-selectable
# 6. Single binary + data files = all variants
# 7. Remove old Makefile
```

## Bottom Line

**Use CMake, but keep it simple:**
- 4-5 size variants MAX
- Runtime variant selection
- Focus on code quality & maintainability
- Add complexity ONLY if needed

The old Makefile's 234 variants were a product of its time (2009):
- Disk space was expensive → compile everything in
- Memory was limited → separate binaries
- Data files were hard to distribute

**In 2025:**
- Use runtime data loading
- One binary, unlimited variants
- Much simpler build system

**Start with CMake that matches simple.mk, then evolve it as you refactor the code.**
