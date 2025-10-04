# Platform Detection Analysis

## How Platforms Are Selected

**KEY FINDING:** Platforms are **NOT configured by CMake or build system**.

Instead, they are **auto-detected at compile time** by checking symbols defined by:
1. **The compiler** (e.g., `__MSDOS__`, `_WIN32`, `__VMS`)
2. **The curses library headers** (e.g., `__PDCURSES__`, `SLCURSES`, `NCURSES_VERSION`)

## Our System

```c
NCURSES_VERSION defined: 6.0
__PDCURSES__ NOT defined
SLCURSES NOT defined  
OLDCURSES NOT defined
MACCURSES NOT defined
```

**Conclusion:** Our system uses **ncurses**. All other platform code is **dead code**.

## Dead Platform Code (Safe to Remove)

| Platform | Symbol | Lines | Status |
|----------|--------|-------|--------|
| S-Lang curses | `SLCURSES` | ~50 | ✅ Removed |
| PDCurses (Windows) | `__PDCURSES__` | ~51 | ⏳ Partially removed |
| Old BSD curses | `OLDCURSES` | ~149 | ⏳ Needs removal |
| Mac Classic | `MACCURSES` / `macintosh` | ~140 | ⏳ Needs removal |
| VMS | `_VMS_CURSES` | ~75 | ✅ Removed |
| BSD 4.4 | `_BSD44_CURSES` | ~9 | ✅ Removed |

**Estimated total removable:** ~400+ lines remaining

## Why This Design Is Problematic

❌ **No isolation** - Platform code scattered throughout  
❌ **Deep nesting** - Up to 7 levels of conditionals  
❌ **No configuration** - Can't easily target single platform  
❌ **Hard to maintain** - Changes affect multiple platforms  

## Better Design (for reference)

```c
// platform.h - ONE file per platform
#ifdef USE_NCURSES
  #include "platform_ncurses.h"
#elif defined(USE_PDCURSES)
  #include "platform_pdcurses.h"
#endif

// Common code uses platform abstraction
platform_init();
platform_draw(x, y, ch);
```

## Simplification Strategy

Since we only target **modern ncurses systems**:

1. ✅ Remove all VMS/BSD code
2. ✅ Remove SLCURSES code  
3. ⏳ Remove PDCURSES code
4. ⏳ Remove OLDCURSES code
5. ⏳ Remove MACCURSES code
6. ⏳ Assume ncurses features throughout

**Goal:** Reduce from 7,701 → ~4,000-5,000 lines

## guess.h Analysis

**File:** `include/guess.h` (409 lines)
**Purpose:** Platform/feature detection for DOS, Windows, VMS, Mac Classic, etc.

### Symbols Defined (29 total)
- WIN32, WIN16, __MSDOS__ - Platform markers
- HAVE_FCNTL_H, HAVE_IO_H, HAVE_IOCTL_H, etc. - Feature availability
- MAIN_NO_ENVP - main() signature variation

### Usage in Current Code
Only 4 symbols actually used:
1. `MYMAN_GUESS_H_INCLUDED` - include guard (not functional)
2. `WIN32` - Used 5 times in utils.c (path separator handling)
3. `__MSDOS__` - Used 4 times in utils.c (path separator handling)  
4. `MAIN_NO_ENVP` - Used in myman.c (envp parameter)

### Platform Test Results
On our ncurses system (Mac/Linux):
- ✅ WIN32: **NOT defined** (Windows-only)
- ✅ __MSDOS__: **NOT defined** (DOS-only)
- ✅ MAIN_NO_ENVP: **NOT defined** (Pelles C only)

### Conclusion
**guess.h is 99% dead code** on Unix/Mac systems!

**Removal Plan:**
1. Remove WIN32/__MSDOS__ conditionals from utils.c (keep Unix code)
2. Remove MAIN_NO_ENVP from myman.c (keep envp parameter)
3. Remove #include "guess.h" from all source files
4. Delete include/guess.h

**Estimated savings:** ~430 lines (guess.h itself + dead conditionals)

**Status:** ⏳ To be implemented in future cleanup
