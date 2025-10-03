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
