# Phase 0 Summary: globals.h Foundation

**Date:** 2025-10-04  
**Branch:** refactor  
**Status:** ✅ COMPLETE

---

## What Was Done

### Created `include/globals.h`
Single header file consolidating all 210+ global variable declarations from `utils.h`.

**Structure:**
- 30+ organized sections
- Full Doxygen documentation
- Clear categorization by subsystem

**Key Sections:**
- Game state (score, lives, level)
- Maze data and rendering  
- Sprite registers and animation
- Ghost AI state
- Input and timing
- Display and colors
- Function declarations

### Updated Source Files
All source files now include `globals.h` after `utils.h`:
- `src/myman.c`
- `src/utils.c`
- `src/logic.c`
- `src/main.c`

### Build Verification
All 4 size variants build and run correctly:
- ✅ `glomph` (standard 4x4)
- ✅ `glomph-xlarge` (5x3 ASCII art)
- ✅ `glomph-small` (2x1 Unicode)
- ✅ `glomph-tiny` (1x1 minimal)

---

## Benefits

### Immediate
1. **Visibility** - All global state in one place
2. **Searchability** - Easy to grep for variable usage
3. **Documentation** - Clear categorization of globals
4. **Stability** - No behavior changes, pure organization

### Long-term
1. **Modularization** - Foundation for extracting modules
2. **Refactoring** - Clear dependencies before restructuring
3. **Understanding** - Documents current architecture
4. **Type migration** - Easy to update types in one place

---

## Commits

```
3bd669d - Update AGENTS.md with Phase 0 refactoring status
ffbdfe5 - Update REFACTOR_PLAN.md - mark Phase 0 complete  
c136511 - Phase 0: Consolidate globals - create globals.h
```

---

## Testing Performed

### Build Testing
```bash
cmake -B build
cmake --build build
# Result: All 4 variants compiled successfully
```

### Runtime Testing
```bash
./build/glomph --help
./build/glomph-xlarge --help
./build/glomph-small --help
./build/glomph-tiny --help
# Result: All variants display help correctly
```

### Manual Play Testing
- Game starts correctly
- Sprites render properly
- Collision detection works
- Score tracking functions
- Lives system operational
- Help system functional

---

## Key Design Decisions

### Why NOT create src/globals.c?
**Decision:** Keep variable definitions in their original source files (utils.c, myman.c, etc.)

**Rationale:**
1. **Standard C practice** - `extern` in headers, definitions in source
2. **Minimal disruption** - Don't move code unnecessarily
3. **Incremental approach** - Organization first, restructuring later
4. **Avoid breakage** - Large moves increase risk of errors

This is the standard pattern used in most C projects.

### Why include globals.h in all files?
**Decision:** Add `#include "globals.h"` to all source files

**Rationale:**
1. **Explicit dependencies** - Makes global usage visible
2. **Future-proof** - Easier to track when extracting modules
3. **Documentation** - Clear what each file depends on
4. **Consistency** - Same pattern in all files

---

## File Statistics

### globals.h
- **Lines:** 682
- **Sections:** 30+
- **Extern declarations:** 210+
- **Function declarations:** 50+

### Source Changes
- **Files modified:** 4 (myman.c, utils.c, logic.c, main.c)
- **Lines added:** ~5 (one include per file)
- **Behavior changes:** 0

---

## Next Phase Preview

### Phase 1: C17 Upgrade

**Estimated time:** 1-2 hours

**Tasks:**
1. Update CMakeLists.txt to C17
2. Add static assertions
3. Test build
4. Commit

**Changes:**
```cmake
# CMakeLists.txt
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
```

**Static assertions to add:**
```c
_Static_assert(MAXGHOSTS == 16, "Ghost array size must be 16");
_Static_assert(SPRITE_REGISTERS == 46, "Sprite register count");
_Static_assert(sizeof(int) >= 4, "Need 32-bit integers");
```

---

## Lessons Learned

### What Worked Well
1. **Incremental approach** - Small, testable steps
2. **No parallel builds** - Single branch, clean history
3. **Test after each change** - Caught issues immediately
4. **Clear documentation** - Easy to understand what changed

### What To Watch
1. **Build warnings** - Still have 3 warnings in myman.c (pre-existing)
2. **Global count** - 210+ globals is a lot, needs future reduction
3. **Include order** - globals.h must come after utils.h

### What's Next
1. **C17 features** - Can now use modern safety features
2. **Type cleanup** - Prepare for stdint.h migration
3. **Module headers** - Split globals.h into domain headers
4. **Encapsulation** - Eventually reduce global usage

---

## References

- **Main plan:** `docs/REFACTOR_PLAN.md`
- **Build guide:** `docs/CMAKE_SETUP.md`  
- **Agent guide:** `AGENTS.md`
- **License:** `LICENSE` (BSD-style, original + 2025 additions)

---

## Quick Reference

### Build Commands
```bash
# Clean rebuild
rm -rf build && cmake -B build && cmake --build build

# Quick rebuild
cmake --build build

# Test
./build/glomph --help
```

### Find Global Usage
```bash
# Find where a global is used
grep -r "score" src/

# Find where a global is defined
grep "^int score" src/*.c

# List all globals
grep "^extern" include/globals.h
```

### Git Commands
```bash
# View changes
git log --oneline -5
git show c136511

# Compare to main
git diff main..refactor
```

---

**End of Phase 0 Summary**
