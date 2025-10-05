# Glomph Maze Codebase Analysis - Next Steps

## Current Status After Refactoring

### Completed Extractions ✓
1. **input_state.c** (44 lines) - Timing utilities
2. **render_state.c** (39 lines) - Curses wrappers
3. **gamecycle()** → game_state.c (orchestration)
4. **paint_walls()** → render.c (rendering)

### File Inventory
```
src/
├── main.c (378 lines) - NEEDS CLEANUP
├── logic.c (920 lines) - GOOD AS IS
├── render.c (996 lines) - Focused
├── game_state.c (1,307 lines) - Focused
├── utils.c (1,899 lines) - Still large
├── myman.c (5,514 lines) - DELAY EXTRACTION
```

---

## PRIORITY 1: Clean Up main.c (378 lines)

### Current Issues
**Over-engineered for our simple use case:**
- Multi-variant launcher with execve() calls
- BUILTIN_SIZES/BUILTIN_VARIANTS macros (unused)
- PRIVATELIBEXECDIR external executable support (unused)
- XCURSES/Allegro/SDL compatibility (we only use ncurses)
- Mac OS X -psn_ parameter handling (obsolete)
- Custom getopt wrapper support (we use standard getopt)

### What We Actually Need
We build **4 independent binaries** via CMake:
- `glomph` (standard size)
- `glomph-xlarge` (huge size)
- `glomph-small` (small size)
- `glomph-tiny` (tiny size)

Each binary has MYMANSIZE defined at compile time. No runtime variant switching needed.

### Proposed Cleanup
**Replace main.c with simplified version:**
```c
// main.c - Entry point for Glomph Maze
#include <getopt.h>
#include "utils.h"
#include "globals.h"

extern int myman_main(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    return myman_main(argc, argv);
}
```

**Move argument parsing to myman.c** where it's actually used.

---

## PRIORITY 2: logic.c Analysis (920 lines)

### Purpose
Single public function: `int gamelogic(void)` - Core game update loop

### What It Does
- Hero movement & collision detection
- Ghost AI (chase/scatter/frightened modes)
- Dot/pellet collection
- Power pellet timer
- Bonus fruit spawning
- Level completion
- Bonus life awards
- Death/respawn logic

### Verdict: **LEAVE AS IS** ✓
- Well-focused single responsibility
- Good size (920 lines)
- Clear domain (game rules)
- Minimal coupling (uses globals but doesn't modify state structure)

---

## PRIORITY 3: utils.c Remaining Functions (1,899 lines)

### Need Analysis
10 remaining functions after paint_walls/gamecycle extraction.

**Likely candidates for extraction:**
1. UTF-8/CP437 character conversion functions → `encoding.c`?
2. File I/O helpers (fopen_datafile, ignore_bom_utf8) → `file_io.c`?
3. String parsing (strword, strtollist, strtodlist) → `parsing.c`?
4. Environment variable handling (myman_setenv, myman_getenv) → Keep or simplify?

**Need to audit:**
- Which functions are legacy cruft?
- Which are essential?
- Can we delete any unused code?

---

## PRIORITY 4: myman.c Deep Dive (DELAYED)

### Why Delay?
- 5,514 lines of complex rendering/input/audio code
- High global state coupling
- Many curses-specific functions
- Risk of breaking rendering pipeline

### Strategy When Ready
1. Extract obvious wrappers (my_move, my_addch, etc.)
2. Create `input.c` for keyboard handling
3. Create `audio.c` for sound effects
4. Leave core rendering loop in place

---

## RECOMMENDED ACTION PLAN

### Phase 1: Simplify main.c (HIGH PRIORITY)
**Why:** Removes 300+ lines of dead code, clarifies entry point
**Risk:** Low - just moving arg parsing
**Steps:**
1. Move argument parsing from main.c to myman.c
2. Replace main.c with minimal wrapper
3. Remove BUILTIN_*/PRIVATELIBEXECDIR/XCURSES cruft
4. Test all 4 variants

### Phase 2: Audit utils.c Functions (MEDIUM PRIORITY)
**Why:** Still 1,899 lines - likely contains extractable/deletable code
**Risk:** Low-Medium - need careful analysis
**Steps:**
1. List all 10 remaining functions
2. Check usage across codebase
3. Extract to focused modules (encoding, parsing, file_io)
4. Delete any unused legacy code

### Phase 3: Final Cleanup (OPTIONAL)
- Document module boundaries
- Update README with new architecture
- Consider renaming myman.c → game_core.c

---

## QUESTIONS TO ANSWER

1. **main.c:** Can we delete all BUILTIN_SIZES/VARIANTS code? (YES - CMake handles this)
2. **Environment vars:** Is myman_setenv/getenv still needed? (CHECK USAGE)
3. **UTF-8/CP437:** Is character encoding still relevant? (CHECK if used)
4. **File I/O helpers:** Can we use standard fopen? (CHECK portability needs)

---

## FINAL RECOMMENDATION

**Start with main.c cleanup** - it's the easiest win:
- Remove 300+ lines of dead code
- Clarify program entry point
- Make build system more obvious
- Low risk, high clarity gain

**Then audit utils.c** - identify extraction/deletion candidates.

**Delay myman.c** - too risky without better test coverage.
