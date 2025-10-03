# Modern Simplification Plan

**Goal:** Strip 25-year-old platform compatibility code, focus on modern ncurses

## Phase 1: Platform Simplification

### Step 1: Identify Modern Target Platforms
- ✅ Linux (ncurses)
- ✅ macOS (ncurses via Homebrew)  
- ✅ Windows WSL (ncurses)
- ❌ DOS, VMS, OS/2, BeOS, AmigaOS
- ❌ aalib, caca, SDL, GTK, FLTK (14+ curses implementations!)

### Step 2: Remove Ancient Platform Code
Delete these curses implementations:
- [ ] `inc/aacurses.h` - aalib (ASCII art)
- [ ] `inc/cacacurs.h` - libcaca
- [ ] `inc/sdlcurs.h` - SDL
- [ ] `inc/gtkcurs.h` - GTK
- [ ] `inc/fltkcurs.h` - FLTK
- [ ] `inc/newtcurs.h` - newt
- [ ] `inc/twcurses.h` - twin
- [ ] `inc/dispcurs.h` - display
- [ ] `inc/ggicurs.h` - GGI
- [ ] `inc/allegcur.h` - Allegro
- [ ] `inc/graphcur.h` - graph
- [ ] `inc/rawcurs.h` - raw terminal
- [ ] `inc/eficurs.h` - EFI
- [ ] `inc/coniocur.h` - DOS conio

Keep only:
- ✅ `inc/mycurses.h` - curses detection
- ✅ Standard ncurses

### Step 3: Remove Ancient Workarounds
- [ ] Delete `mygetopt/` - use standard getopt
- [ ] Remove VMS-specific code
- [ ] Remove DOS-specific code
- [ ] Remove Windows 95/98 code

### Step 4: Simplify Build System
- [ ] Remove conditional compilation for 14 platforms
- [ ] Assume ncurses always available
- [ ] Remove autoconf-style feature detection (use modern CMake)

## Expected Result
- From: 7,999 lines
- To: ~4,000 lines (estimated)
- Cleaner, modern codebase ready for Option D (understand & extract)

## Phase 2: Document & Understand (Option D)
- [ ] Document remaining functions
- [ ] Identify: game logic vs rendering vs I/O
- [ ] Draw architecture diagram

## Phase 3: Extract With Understanding
- [ ] Extract to proper modules
- [ ] Possibly rewrite complex parts

---
**Branch:** `modern-simplify`
**Base:** v0.8.0
**Date:** 2025-10-03
