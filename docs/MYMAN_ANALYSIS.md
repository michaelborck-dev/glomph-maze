# myman.c Refactoring Strategy - PLAN MODE ANALYSIS

## Current State

**File:** `src/myman.c` (5,499 lines)
**Preprocessor directives:** 788 `#if/#ifdef/#ifndef/#else/#endif` lines (14% of file!)
**Static functions:** 28 functions
**Public functions:** 5 (gamesfx, gamerender, gameinput, usage, main)

## CMake Compile Flags Analysis

**Only 2 flags are defined by CMake:**
1. `MYMANSIZE` - Size variant ("xlarge", "standard", "small", "tiny")
2. `USE_SDL_MIXER=1` - Audio support (when `-DENABLE_AUDIO=ON`)

**Plus directory paths:**
- `TILEDIR`, `SPRITEDIR`, `MAZEDIR`, `SOUNDDIR`
- `TILEFILE`, `SPRITEFILE`

**That's it!** Everything else is legacy cruft.

---

## Legacy Platform #ifdefs to Remove

### ❌ Platform/Compiler Support (REMOVE)
- `__PACIFIC__`, `HI_TECH_C`, `SMALL_C` - Ancient DOS compilers
- `UNICODE`, `_UNICODE` - Win32 wide character support
- `HAVE_CONFIG_H` - autoconf support (we use CMake)
- `LIT64` - 64-bit literal support (C17 has this built-in)
- `HAVE_RAISE` - signal() support detection
- `SIG_ERR` - signal error detection
- `F_OK` - file access constant (always defined in modern systems)

### ❌ Curses Library Variants (REMOVE)
We only use **ncurses**, not:
- `NCURSES_XOPEN_HACK` - X/Open ncurses quirks
- `MY_CURSES_H` - Custom curses header selector
- `HAVE_SETATTR` - Old BSD curses compatibility
- `HAVE_NODELAY` - Curses nodelay() detection
- `USE_KEYPAD` - Keypad support detection
- `USE_A_CHARTEXT` - Attribute text extraction
- `USE_DIM_AND_BRIGHT` - Dim/bright attribute support
- `A_BOLD` - Bold attribute detection

### ❌ Character Encoding (SIMPLIFY)
- `USE_ICONV` - Character set conversion (do we need this?)
- `wcwidth` - Wide character width (locale-specific)
- `CODESET` - Locale codeset detection
- `LC_CTYPE` - Locale type detection

### ❌ Terminal Resize Detection (KEEP)
- `USE_IOCTL` - Terminal size via ioctl (KEEP - useful)
- `USE_SIGWINCH` - Window change signal (KEEP - useful)
- `TIOCGWINSZ` - Get window size (KEEP - useful)

### ✅ Audio Support (KEEP)
- `USE_SDL_MIXER` - SDL audio (KEEP - we use this)
- `USE_BEEP` - Terminal beep fallback (KEEP - fallback)

---

## Function Breakdown

### Curses Wrappers (~800 lines)
**Functions:**
- `my_erase()`, `my_refresh()`, `my_move()`, `my_attrset()`, `my_addch()`, `my_addstr()`, `my_getch()`
- `snapshot_*()` functions for HTML screenshots

**Strategy:** 
- Extract to `src/curses_wrapper.c`
- Simplify by removing legacy curses compatibility
- Keep only ncurses-specific code

### Pager System (~400 lines)
**Functions:**
- `pager()`, `pager_move()`, `pager_addch()`, `pager_addstr()`

**Strategy:**
- Extract to `src/pager.c`
- Used for help screens and legal notices
- Self-contained subsystem

### Game Core (~3000 lines)
**Functions:**
- `gamesfx()` - Sound effects (already clean)
- `gamerender()` - Main rendering loop
- `gameinput()` - Input handling
- `myman()` - Main game loop

**Strategy:**
- Leave `myman()` as orchestrator
- Extract `gamerender()` → `src/render.c` (move to existing file)
- Extract `gameinput()` → `src/input.c` (new file)
- Keep `gamesfx()` in myman.c (already well-abstracted)

### Initialization (~1000 lines)
**Functions:**
- `init_trans()` - Character translation tables
- `init_pen()`, `destroy_pen()` - Color palette
- `sigwinch_handler()` - Window resize handler
- `locale_is_utf8()` - UTF-8 detection

**Strategy:**
- Extract to `src/init.c`
- Remove legacy locale detection code
- Simplify to ncurses-only

### Argument Parsing (~300 lines)
**Functions:**
- `parse_myman_args()` - Command-line parsing
- `usage()` - Help text

**Strategy:**
- Already in myman.c, could extract to `src/args.c`
- Not urgent - relatively clean

---

## Refactoring Strategy (Prioritized)

### Phase 1: Remove Dead Platform Code (HIGH PRIORITY)
**Impact:** ~200 lines removed, readability ++

**Actions:**
1. Remove Win32 UNICODE handling (lines 37-48)
2. Remove ancient compiler support (lines 58-71)
3. Remove autoconf support (`HAVE_CONFIG_H`)
4. Remove custom curses header logic (`MY_CURSES_H`)
5. Set defaults for always-true flags:
   - `HAVE_NODELAY=1` (always true for ncurses)
   - `HAVE_SETATTR=0` (never needed)
   - `USE_KEYPAD=1` (always true)

**Risk:** Low - these are never used in CMake builds

---

### Phase 2: Extract Curses Wrappers (MEDIUM PRIORITY)
**Impact:** ~800 lines moved, separation of concerns

**Create:** `src/curses_wrapper.c` + `include/curses_wrapper.h`

**Move functions:**
- All `my_*()` curses wrappers
- All `snapshot_*()` functions
- Remove legacy curses compatibility code

**Keep in myman.c:**
- `myman()` main loop
- High-level game logic

**Risk:** Medium - curses code is intertwined with game logic

---

### Phase 3: Extract Pager System (LOW PRIORITY)
**Impact:** ~400 lines moved

**Create:** `src/pager.c` + `include/pager.h`

**Move functions:**
- `pager()`, `pager_move()`, `pager_addch()`, `pager_addstr()`

**Risk:** Low - self-contained subsystem

---

### Phase 4: Extract Game Rendering (OPTIONAL)
**Impact:** ~1200 lines moved

**Move to existing `src/render.c`:**
- `gamerender()` function
- Rendering helper functions

**Risk:** High - core game logic, needs careful testing

---

### Phase 5: Extract Initialization (OPTIONAL)
**Impact:** ~600 lines moved

**Create:** `src/init.c` + `include/init.h`

**Move functions:**
- `init_trans()`, `init_pen()`, `destroy_pen()`
- `locale_is_utf8()`, `my_wcwidth()`
- Signal handlers

**Risk:** Medium - initialization order matters

---

## Recommended Action Plan

**For this session:**
1. ✅ **Start with Phase 1** - Remove dead platform code
   - Lowest risk
   - Immediate readability improvement
   - ~200 lines removed
   - No functional changes

**Future sessions:**
2. Phase 2 - Extract curses wrappers (separate PR)
3. Phase 3 - Extract pager (separate PR)
4. Phases 4-5 - Optional, only if needed

---

## Questions to Answer

1. **Do we need `USE_ICONV`?** 
   - Check if any users need character set conversion
   - Likely NO for modern UTF-8 systems

2. **Keep `USE_SIGWINCH`?**
   - YES - useful for terminal resize detection
   - Already defaults correctly

3. **Keep `USE_IOCTL`?**
   - YES - needed for terminal size detection
   - Already defaults correctly

4. **Keep `USE_BEEP`?**
   - YES - fallback when SDL not available
   - Already defaults correctly

---

## Expected Results After Phase 1

**Before:**
- 5,499 lines
- 788 preprocessor directives (14%)
- Supports: DOS, Win32, ancient Unix, modern Unix

**After:**
- ~5,300 lines (-200 lines)
- ~600 preprocessor directives (-24%)
- Supports: modern Unix only (ncurses + optional SDL)

**Maintainability:** +++
**Risk:** Low
**Test Impact:** None (dead code removal)

---

## Implementation Notes

**DO NOT:**
- Remove `USE_SDL_MIXER` (we use it)
- Remove `USE_BEEP` (fallback audio)
- Remove `USE_SIGWINCH` (terminal resize)
- Remove `USE_IOCTL` (terminal size)
- Change game logic

**DO:**
- Remove Windows-specific code
- Remove DOS compiler support
- Remove autoconf support
- Simplify curses compatibility to ncurses-only
- Set always-true flags to constants

**Test after each change:**
```bash
cmake -B build -DENABLE_AUDIO=ON
cmake --build build
./build/glomph --help
```

---

## Final Recommendation

**Execute Phase 1 now:**
- Remove dead platform code
- Low risk, high readability gain
- ~200 lines removed
- No behavior changes
- Easy to review

**Defer Phases 2-5:**
- Larger changes
- Need careful planning
- Separate PRs for each
- Can be done incrementally

Ready to execute Phase 1 when plan mode ends! 🎯
