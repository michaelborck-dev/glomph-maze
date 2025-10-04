# SIMPLIFIED C17 Refactoring Plan
## Single Branch Strategy + Globals Consolidation

**Status:** Active  
**Branch:** refactor  
**Timeline:** 5-6 weeks @ 10 hours/week  
**Last Updated:** 2025-10-04

---

## Strategy

### ✅ What We're Doing:
1. **Single refactor branch** - no parallel builds, no `#ifdef` hell
2. **globals.h first** - consolidate all externs in one place
3. **Incremental, working builds** - each commit compiles and runs
4. **Clear dependencies** - understand what depends on what

### ❌ What We're NOT Doing:
- No parallel build systems
- No complex CMake options
- No "it'll work later" broken states
- No premature modularization

---

## Progress Tracking

- [x] Phase 0: globals.h Foundation ✅ COMPLETE
- [x] Phase 1: C17 Upgrade ✅ COMPLETE
- [x] Phase 2: Type Modernization ✅ COMPLETE
- [x] Phase 3: Header Reorganization ✅ COMPLETE
- [ ] Phase 4: Function Documentation (Ongoing)
- [ ] Phase 5: Extract First Module (Optional)

---

## Phase 0: globals.h Foundation ✅ COMPLETE

**Goal:** Create single source of truth for all global state

**Status:** COMPLETE (Commit: c136511)

### Tasks:
- [x] Create include/globals.h with all 210 extern declarations
- [x] Update source files to include globals.h  
- [x] Test all 4 build variants (all working)
- [x] Commit: "Phase 0: Consolidate globals"

**Note:** We did NOT create src/globals.c - variable definitions remain in their original source files (utils.c, myman.c, etc.). This is standard C practice and avoids massive refactoring at this stage.

**Time actual:** 2 hours

**Benefits:**
- ✅ See ALL global state at once
- ✅ Easy to find dependencies (grep for variable names)
- ✅ Foundation for later encapsulation
- ✅ Documents current architecture

---

## Phase 1: C17 Upgrade ✅ COMPLETE

**Goal:** Update to C17 standard with compile-time safety

**Status:** COMPLETE

### Task 1.1: Update CMakeLists.txt ✅
```cmake
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
```

### Task 1.2: Add Static Assertions ✅
```c
// In globals.h
_Static_assert(MAXGHOSTS == 16, "Ghost array size hardcoded as 16");
_Static_assert(SPRITE_REGISTERS == 57, "Sprite register count");
_Static_assert(LIVES >= 0 && LIVES <= 99, "Lives range check");
_Static_assert(NPENS == 256, "Color palette size");
_Static_assert(sizeof(int) >= 4, "Need 32-bit integers");
_Static_assert(sizeof(void*) >= 4, "Need 32-bit pointers");
```

### Task 1.3: Test & Commit ✅
All 4 variants build and run successfully with C17.

**Time actual:** 1 hour

**Corrections made:**
- Fixed SPRITE_REGISTERS constant: was 46, actually 57 (calculated from MAXGHOSTS=16)
- All static assertions pass at compile time

---

## Phase 2: Type Modernization ✅ COMPLETE

**Goal:** Use C17 stdint.h and stdbool.h types

**Status:** COMPLETE

### Task 2.1: Update globals.h Types ✅
```c
// Replaced:
unsigned char → uint8_t (arrays, pointers, functions)
unsigned short → uint16_t (inside_wall)
int flags → bool (all_dirty, nogame, paused)
```

### Task 2.2: Update utils.h Types ✅
- Added <stdio.h>, <stdint.h>, <stdbool.h>
- Updated all type declarations to match globals.h

### Task 2.3: Update utils.c Definitions ✅
- Updated variable definitions
- Updated function signatures
- Updated function bodies

### Task 2.4: Test & Commit ✅
All 4 variants build and run successfully.

**Time actual:** 2 hours

**Types Modernized:**
- `uint8_t`: sprite_register, dirty_cell, home_dir, reflection arrays, gfx functions
- `uint16_t`: inside_wall pointer
- `bool`: all_dirty, nogame, paused

---

## Phase 3: Header Reorganization ✅ COMPLETE

**Goal:** Split globals.h into logical modules

**Status:** COMPLETE

### Module Headers Created:
```
include/
├── globals.h          (aggregator - includes all modules)
├── game_state.h       (score, lives, level, player state) - 114 lines
├── sprite_state.h     (sprite registers, ghost AI) - 123 lines
├── maze_state.h       (maze data, dimensions, loading) - 114 lines
├── render_state.h     (screen, colors, tiles, pager) - 139 lines
└── input_state.h      (keyboard, controls, timing) - 91 lines
```

### What Changed:
1. Created 5 domain-specific header files
2. Moved 210+ declarations from globals.h into appropriate modules
3. globals.h now acts as central aggregator
4. All headers include proper guards and documentation
5. Updated .clang-format to c++17 standard
6. Applied formatting to all headers

### Verification:
- ✅ All 4 variants build successfully
- ✅ Help system tested and working
- ✅ No behavior changes
- ✅ Code formatted and clean

**Time actual:** 3 hours

### Benefits:
- Better organization by functional domain
- Easier to navigate and find declarations
- Each module has clear purpose and documentation
- Foundation for future module extraction
- No breaking changes - backward compatible

---

## Phase 4: Function Documentation (Ongoing)

**Goal:** Add Doxygen comments to complex functions

### Documentation Template:
```c
/**
 * @brief One-line summary
 * 
 * Detailed description explaining algorithm and purpose.
 * 
 * @param name Parameter description
 * @return Return value meaning
 * 
 * @note Side effects, global state usage
 * @warning Thread safety, limitations
 * @see Related functions
 */
```

### Focus Areas:
- Complex algorithms (ghost AI, collision)
- Non-obvious behavior
- Global state dependencies
- Side effects

**Time estimate:** Ongoing (2-3 hours/week)

---

## Phase 5: Extract First Module (Optional)

**Goal:** Prove modularization approach works

### Best First Module: Collision Detection
- Small (~100 lines)
- Well-defined interface
- Easy to test
- Minimal dependencies

### Structure:
```
src/core/
├── collision.h
└── collision.c
```

**Time estimate:** 12-16 hours

**Decision Point:** Only proceed if Phases 0-4 went smoothly

---

## Timeline

| Week | Phase | Hours | Deliverable |
|------|-------|-------|-------------|
| 1 | Phase 0 | 3h | globals.h created |
| 1 | Phase 1 | 2h | C17 upgrade |
| 2 | Phase 2 | 8h | Modern types |
| 3 | Phase 3 | 12h | Module headers |
| 4-5 | Phase 4 | Ongoing | Documentation |
| 6+ | Phase 5 | 12h | Optional: extract module |

**Total: 5-6 weeks**

---

## Success Metrics

### After Phase 0 (globals.h):
- [ ] All 210 globals in globals.h
- [ ] Clear list of what exists
- [ ] Can grep for usage patterns
- [ ] Game runs identically

### After Phase 1 (C17):
- [ ] Compiles with C17
- [ ] Static assertions added
- [ ] No behavior changes

### After Phase 2 (Types):
- [ ] Using stdint.h types
- [ ] Using stdbool for flags
- [ ] Type safety improved

### After Phase 3 (Headers):
- [ ] 5+ module headers exist
- [ ] globals.h is just includes
- [ ] Clear module boundaries

### After Phase 4 (Docs):
- [ ] Top 20 functions documented
- [ ] Complex algorithms explained
- [ ] Dependencies documented

---

## Rollback Plan

If anything breaks:
```bash
# See what changed
git diff HEAD

# Undo last commit
git reset --hard HEAD~1

# Full reset to start of phase
git log --oneline
git reset --hard <commit-hash>
```

---

## Why This Works

| Previous Problem | New Solution |
|------------------|--------------|
| Too complex | Simple linear progression |
| Parallel builds | Single refactor branch |
| Unclear dependencies | globals.h makes everything visible |
| Breaking changes | Each phase builds and runs |
| Over-engineering | Pragmatic, incremental steps |

---

## License Header for New Files

```c
/*
 * filename.c - Brief description
 * 
 * Copyright 1997-2009, Benjamin C. Wiley Sittler <bsittler@gmail.com>
 * Copyright 2025, Michael Borck <michael@borck.dev>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
```

---

**Next Action:** Begin Phase 0 - Create globals.h
