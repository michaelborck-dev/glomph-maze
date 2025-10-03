# Quick Start - Get Building Fast!

## TL;DR

```bash
# Fast builds RIGHT NOW (no setup)
make -f simple.mk
./glomph

# Better builds (30 min setup, worth it)
brew install cmake
cmake -B build
cmake --build build
./build/glomph
```

## The Situation

- ❌ Main `Makefile`: 20+ minutes (AVOID!)
- ✅ `simple.mk`: 1 second (WORKS!)
- ✅ CMake: 1-2 seconds incremental (BEST!)

## Just Want to Play the Game?

```bash
cd /Users/michael/Projects/glomph-maze
make -f simple.mk
./glomph
# Arrow keys to move, Q to quit, ? for help
```

## Want to Develop?

### Option 1: Quick & Dirty (Use simple.mk)

```bash
# Build
make -f simple.mk

# Test
./glomph --help

# Edit code
vim src/myman.c

# Rebuild (always takes 1-2 sec)
make -f simple.mk

# Repeat
```

**Pros:** No setup, works immediately  
**Cons:** Rebuilds everything every time (even if only 1 file changed)

### Option 2: Professional Setup (Use CMake)

```bash
# One-time setup
brew install cmake

# Configure (first time only)
cmake -B build

# Build (first time: ~30 sec)
cmake --build build

# Test
./build/glomph --help

# Edit code
vim src/myman.c

# Rebuild (ONLY rebuilds changed files! ~1-2 sec)
cmake --build build

# Run tests
cd build && ctest

# Format code
cmake --build build --target format

# Find bugs with sanitizer
cmake -B build-asan -DENABLE_ASAN=ON
cmake --build build-asan
./build-asan/glomph
```

**Pros:** Fast incremental builds, testing, IDE support, sanitizers  
**Cons:** Need to install CMake once

## Which Should You Choose?

**Use simple.mk if:**
- You just want to try the game
- You're making quick one-off changes
- You don't want to install anything

**Use CMake if:**
- You're doing serious development
- You're refactoring code
- You want IDE integration (VSCode, CLion)
- You want to catch bugs early (sanitizers)
- You want professional workflow

## Files You'll Build

All three options create these binaries:

- `glomph-xlarge` - Huge ASCII art tiles
- `glomph` - Big ASCII art tiles (default)
- `glomph-small` - Small 2x2 character tiles
- `glomph-tiny` - Tiny 1x1 character tiles

## Next Steps

1. **Try both** and see which you prefer
2. **Read** `CMAKE_VS_MAKEFILE.md` for detailed comparison
3. **Start** refactoring with confidence!

## Help! Something Broke!

### "cmake: command not found"
```bash
brew install cmake
```

### "make: command not found"
```bash
# You need Xcode Command Line Tools
xcode-select --install
```

### "curses.h: No such file"
```bash
# On macOS, curses is built-in
# On Linux: sudo apt-get install libncurses-dev
```

### Builds are still slow!
```bash
# Make sure you're NOT using the main Makefile:
make -f simple.mk    # ✅ Fast
cmake --build build  # ✅ Fast  
make                 # ❌ SLOW (20+ minutes)
```

## Done!

Now go forth and build fast! 🚀
