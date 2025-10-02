# Phase 2 Extraction Plan - Dependency-Based Order

## Analysis Summary

**File:** `src/myman.c` - 7,999 lines  
**Strategy:** Extract leaf nodes first (functions with minimal dependencies)  
**Approach:** Conservative - copy functions as-is, keep original names

---

## Extraction Order (Least → Most Dependencies)

### Wave 1: Pure Utility Functions (LEAF NODES)
**Lines ~600-2000: Character encoding, locale, signal handling**

These functions have NO dependencies on game state or curses:

1. **`ucs_to_wchar()`** (line 629)
   - Unicode to wide char conversion
   - Pure function, no external dependencies
   - → Extract to: `src/core/config.c`

2. **`locale_is_utf8()`** (line 829)
   - Checks if locale is UTF-8
   - Only depends on system calls
   - → Extract to: `src/core/config.c`

3. **`sigwinch_handler()`** (line 721)
   - Signal handler for window resize
   - Only depends on system signals
   - → Extract to: `src/render/screen.c`

4. **`cp437_to_ascii()`** (line 1897)
   - Character translation
   - Pure lookup table function
   - → Extract to: `src/core/config.c`

5. **`my_wcwidth()`** (line 1927)
   - Wide character width calculation
   - Depends only on wcwidth() system call
   - → Extract to: `src/render/screen.c`

**Risk Level:** ⭐ VERY LOW  
**Dependencies:** System calls only, no game state  
**Test Strategy:** Unit tests for each function

---

### Wave 2: Curses Wrapper Functions  
**Lines ~2500-3800: Low-level screen operations**

These wrap ncurses but don't depend on game logic:

**Target Module:** `src/render/screen.c`

Functions to extract (in this order):
1. `snapshot_*()` functions - HTML/text snapshot (if exist)
2. `my_attrset()` - Attribute setting (line 3728)
3. `my_real_attrset()` - Real attribute setting
4. Color management functions
5. Window management functions

**Risk Level:** ⭐⭐ LOW  
**Dependencies:** Only curses library, no game state  
**Test Strategy:** Create test harness that initializes curses

---

### Wave 3: Pager System
**Lines ~4000-5000: Text display and help system**

The pager displays scrolling text (help, high scores):

**Target Module:** `src/render/pager.c`

Functions to extract:
1. `pager_init()` - Initialize pager state
2. `pager_addstr()` - Add string to pager
3. `pager()` - Main pager loop
4. `gamehelp()` - Help display (depends on pager)

**Risk Level:** ⭐⭐ LOW-MEDIUM  
**Dependencies:** Curses wrappers (from Wave 2)  
**Test Strategy:** Can test with static text

---

### Wave 4: Game State Management
**Lines ~5000-6000: Scoring, lives, level tracking**

Pure state management without rendering:

**Target Module:** `src/core/game_state.c`

Functions to extract:
1. Score tracking functions
2. Lives/death management  
3. Level progression
4. Power pellet timer
5. Fruit bonus logic

**Risk Level:** ⭐⭐⭐ MEDIUM  
**Dependencies:** No rendering, but lots of global state  
**Test Strategy:** Mock game scenarios

---

### Wave 5: Collision Detection
**Lines ~6000-6500: Hit detection**

Ghost collisions, pellet eating, wall checking:

**Target Module:** `src/core/collision.c`

Functions to extract:
1. Player/ghost collision
2. Player/pellet collision
3. Wall collision checking
4. Tunnel wrapping logic

**Risk Level:** ⭐⭐⭐ MEDIUM  
**Dependencies:** Game state (from Wave 4), maze data  
**Test Strategy:** Unit tests with known positions

---

### Wave 6: Input Handling
**Lines scattered: Keyboard input and control mapping**

**Target Module:** `src/input/keyboard.c`

Functions to extract:
1. Key reading/buffering
2. Control mapping (arrow keys, HJKL, etc.)
3. Pause/quit handling
4. Menu navigation

**Risk Level:** ⭐⭐⭐ MEDIUM  
**Dependencies:** Curses input, game state  
**Test Strategy:** Can simulate key presses

---

### Wave 7: Maze Loading
**Lines ~??: Level file parsing**

**Target Module:** `src/data/maze_loader.c`

Functions to extract:
1. Maze file reading
2. Maze parsing
3. Tile/sprite loading
4. Variant configuration

**Risk Level:** ⭐⭐⭐⭐ MEDIUM-HIGH  
**Dependencies:** File I/O, maze data structures  
**Test Strategy:** Load known test mazes

---

### Wave 8: Main Game Loop
**Lines ~7000-7999: Core game loop**

**Keep in:** `src/myman.c` (significantly smaller now)

What remains:
1. `myman()` main function
2. Game loop coordination
3. State machine (attract mode, playing, game over)
4. Integration of all modules

**Risk Level:** ⭐⭐⭐⭐⭐ HIGH  
**This is the last thing to touch!**

---

## Recommended Start: Wave 1, Function 1

### First Extraction: `locale_is_utf8()`

**Why this function first:**
1. ✅ Only 30-40 lines
2. ✅ Zero game dependencies
3. ✅ Zero curses dependencies  
4. ✅ Easy to verify (compare output)
5. ✅ Perfect practice for the extraction process

**Steps:**
1. Copy `locale_is_utf8()` to `src/core/config.c`
2. Add declaration to `src/core/config.h`
3. Guard original in myman.c with `#ifndef USE_NEW_CONFIG`
4. Update CMakeLists.txt to compile with `USE_NEW_CONFIG=ON`
5. Test both builds produce identical output
6. Commit

**Estimated Time:** 30-60 minutes  
**Success Criteria:** Both old and new builds work identically

---

## Alternative Start: Wave 2, Curses Wrappers

If you prefer to extract a larger, more visible module first:

### Extract: `my_attrset()` and related functions

**Why:**
1. Clear boundaries (only wraps curses)
2. Used everywhere, so good test of extraction
3. ~100-200 lines total
4. No game logic mixed in

**Risk:** Slightly higher than Wave 1, but still low

---

## Extraction Process Template

For each function extraction:

```
1. READ: Understand function (5-10 min)
   - What does it do?
   - What does it call?
   - What calls it?
   - What globals does it use?

2. COPY: Extract to new module (10-20 min)
   - Copy function exactly as-is
   - Add to .c file
   - Declare in .h file
   - Handle any globals needed

3. GUARD: Protect original (5 min)
   - Wrap in #ifndef USE_NEW_MODULE
   - Keep old code intact

4. BUILD: Compile both ways (5 min)
   - Build with old code (default)
   - Build with new code (-DUSE_NEW_MODULE=ON)
   - Both must compile

5. TEST: Verify equivalence (10-20 min)
   - Run game with old build
   - Run game with new build
   - Compare behavior (should be identical)
   - Run test suite

6. COMMIT: Save work (2 min)
   - Clear commit message
   - Reference what was extracted
   - Note: "Parallel builds working"

Total per function: 30-60 minutes
```

---

## Progress Tracking

Use this checklist as you go:

### Wave 1: Utility Functions
- [ ] locale_is_utf8()
- [ ] cp437_to_ascii()
- [ ] ucs_to_wchar()
- [ ] my_wcwidth()
- [ ] sigwinch_handler()

### Wave 2: Curses Wrappers
- [ ] my_attrset() and family
- [ ] Color management
- [ ] Window management
- [ ] cursor movement

### Wave 3: Pager
- [ ] pager_init()
- [ ] pager_addstr()
- [ ] pager()
- [ ] gamehelp()

### Wave 4: Game State
- [ ] Score functions
- [ ] Lives management
- [ ] Level progression

### Wave 5: Collision
- [ ] Ghost collision
- [ ] Pellet collision
- [ ] Wall checking

### Wave 6: Input
- [ ] Key reading
- [ ] Control mapping

### Wave 7: Maze Loading
- [ ] File parsing
- [ ] Level data

### Wave 8: Main Loop
- [ ] Refactor remaining myman()

---

## Success Metrics

After each wave:
- ✅ Old build still works (default)
- ✅ New build works identically  
- ✅ Tests pass (all 4)
- ✅ Code compiles without warnings
- ✅ No crashes or hangs

---

## Rollback Strategy

If anything goes wrong:

```bash
# Rollback to before this extraction
git reset --hard HEAD~1

# Or switch to old build
cmake -B build -DUSE_NEW_MODULE=OFF
cmake --build build
```

You can always go back to the pre-refactor tag:
```bash
git checkout pre-refactor-20251002
```

---

## Recommended First Session

**Goal:** Extract first 2-3 utility functions from Wave 1

**Time:** 2-3 hours

**Steps:**
1. Start with `locale_is_utf8()` (easiest)
2. Then `cp437_to_ascii()` (also easy)
3. If going well, try `my_wcwidth()` (slightly harder)

**Outcome:** 
- Build system proven to work
- Extraction process validated
- Confidence built
- Ready for larger extractions

---

## Notes

- **Don't rush**: Better to extract 1 function correctly than 10 functions with bugs
- **Test frequently**: After every extraction, test both builds
- **Commit often**: Each extraction should be its own commit
- **Ask for help**: If stuck, stop and ask questions
- **Celebrate wins**: Each function extracted is progress!

---

*Generated: 2025-10-02*  
*Status: Ready to begin Wave 1*
