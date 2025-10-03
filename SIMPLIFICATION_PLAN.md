# Modern Simplification Plan

**Goal:** Strip 25-year-old platform compatibility code, focus on modern ncurses

**Strategy:** Incremental - keep builds working after each step

---

## Phase 1: Platform Simplification

### ✅ Step 1: Remove Alternate Curses Implementations (DONE)
- Deleted 16 alternate curses headers (30,345 lines!)
- Deleted mygetopt/ (use standard getopt)
- Result: Headers cleaned

### Step 2: Update CMakeLists.txt
- [x] Remove mygetopt from build
- [x] Assume ncurses available
- [x] Test build: `cmake --build build`

### ✅ Step 3: Delete Disabled Platform Code (DONE)
Removed all `#if 0` blocks for legacy platforms:
- [x] Deleted OLD_XCURSES (5 blocks)
- [x] Deleted OLD_CACACURSES (1 block)
- [x] Deleted OLD_GTKCURSES (2 blocks)
- [x] Deleted OLD_FLTKCURSES (1 block)
- [x] Deleted OLD_ALLEGROCURSES (2 blocks)
- [x] Deleted OLD_SLANG_VERSION (2 blocks)
- [x] Test build: ✅ All 4 variants working
- **Result:** 7,957 → 7,860 lines (removed 97 lines)

### Step 4: Remove Remaining Platform Conditionals (Next)
For each platform, remove conditionals and test:

Round 1 - DOS/Windows 95:
- [ ] Remove `#ifdef __MSDOS__` blocks
- [ ] Remove `#ifdef WIN32` (old Windows) blocks
- [ ] Test build

Round 2 - VMS:
- [ ] Remove `#ifdef __VMS` blocks  
- [ ] Remove `vmscurses_getch()` function
- [ ] Test build

Round 3 - Obscure Unix:
- [ ] Remove `#ifdef __amigaos__` blocks
- [ ] Remove `#ifdef __BEOS__` blocks
- [ ] Remove `#ifdef __APPLE__` (old pre-OSX) blocks
- [ ] Test build

Round 4 - Alternate Graphics (Keep SDL_MIXER!):
- [ ] Remove old SDL graphics code (not SDL_MIXER audio!)
- [ ] Keep modern SDL_MIXER sound system
- [ ] Keep only ncurses for display
- [ ] Test build

### Expected Intermediate Results
After each round:
- ✅ All 4 builds compile (huge, big, small, narrow)
- ✅ Game runs: `./build/glomph-big`
- ✅ Line count decreases

### Current Progress
- **Starting point:** 7,999 lines (before any simplification)
- **After header removal:** 7,957 lines (42 lines from disabled graphics blocks)
- **After #if 0 deletion:** 7,860 lines (97 lines of dead platform code)
- **Target:** ~4,000-5,000 lines (estimated)
- **Remaining:** ~3,860 lines to remove

---

## Phase 2: Document & Understand (Option D)
After simplification complete:
- [ ] Document remaining functions (what each does)
- [ ] Identify: game logic vs rendering vs I/O
- [ ] Draw architecture diagram

## Phase 3: Extract With Understanding
- [ ] Extract to proper modules (with understanding!)
- [ ] Possibly rewrite complex parts

---

## Progress Tracking

### Platform Code Removed:
- ✅ Headers: 16 alternate curses implementations (30,345 lines)
- ✅ mygetopt: 710 lines
- ✅ Disabled graphics blocks: 42 lines
- ✅ #if 0 dead code: 97 lines (XCURSES, CACA, GTK, FLTK, ALLEGRO, SLANG)
- [ ] DOS/Windows 95 code in myman.c
- [ ] VMS code in myman.c
- [ ] Obscure Unix code in myman.c
- [ ] Alternate graphics code in myman.c

### Builds Status:
- ✅ hugeman compiles & runs
- ✅ bigman compiles & runs
- ✅ smallman compiles & runs
- ✅ squareman compiles & runs

---
**Branch:** `modern-simplify`
**Base:** v0.8.0
**Date:** 2025-10-03
**Strategy:** Incremental simplification, keep builds working

---

## REVISION: Sound System Strategy

**Discovery:** SDL_MIXER is the MODERN sound system, not legacy!

### Sound System Architecture:
1. **SDL_MIXER** (2013+) - Modern, cross-platform ✅ **KEEP**
2. **ALLEGRO** (1990s) - DOS-era library ❌ **DELETE** 
3. **USE_BEEP** (ancient) - Terminal beep ⚠️ **KEEP as fallback**
4. **Silent** - No sound fallback ✅ **KEEP**

### Revised Approach:
- ✅ Keep SDL_MIXER blocks (modern sound system)
- ❌ Remove ALLEGRO blocks (already disabled with #if 0)
- ✅ Keep USE_BEEP as simple fallback
- 📦 Later: Extract sound code to `src/audio/sound.c`

**Modern Linux/Mac:** Use SDL_MIXER  
**Minimal systems:** Fall back to beep()  
**No audio lib:** Silent mode

This means SDL_MIXER should be **extracted to an audio module**, not deleted!

