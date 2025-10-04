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
- [ ] Phase 1: C17 Upgrade
- [ ] Phase 2: Type Modernization
- [ ] Phase 3: Header Reorganization
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

## Phase 1: C17 Upgrade

**Goal:** Update to C17 standard with compile-time safety

### Task 1.1: Update CMakeLists.txt
```cmake
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
```

### Task 1.2: Add Static Assertions
```c
// In globals.c
_Static_assert(MAXGHOSTS == 16, "Ghost array size hardcoded as 16");
_Static_assert(SPRITE_REGISTERS == 46, "Sprite register count changed");
_Static_assert(sizeof(int) >= 4, "Need 32-bit integers");
```

### Task 1.3: Test & Commit
```bash
rm -rf build
cmake -B build
cmake --build build
./build/glomph
git commit -m "Phase 1: Upgrade to C17 with static assertions"
```

**Time estimate:** 1-2 hours

---

## Phase 2: Type Modernization

**Goal:** Use C17 stdint.h and stdbool.h types

### Task 2.1: Update globals.h Types
```c
// Replace:
unsigned char → uint8_t
unsigned short → uint16_t
unsigned int → uint32_t
int flags → bool (where appropriate)
```

### Task 2.2: Update globals.c Definitions
Match types from globals.h

### Task 2.3: Fix Compilation Errors
Build and fix one file at a time

### Task 2.4: Test & Commit
```bash
cmake --build build
./build/glomph  # Play full game
git commit -m "Phase 2: Modernize types to stdint/stdbool"
```

**Time estimate:** 6-8 hours

---

## Phase 3: Header Reorganization

**Goal:** Split globals.h into logical modules

### Module Headers:
```
include/
├── globals.h          (becomes aggregator)
├── game_state.h       (score, lives, level)
├── sprite_state.h     (sprite registers)
├── maze_state.h       (maze data, dimensions)
├── render_state.h     (screen, colors, tiles)
└── input_state.h      (keyboard, controls)
```

### Process:
1. Create module header (e.g., game_state.h)
2. Move relevant externs from globals.h
3. Update globals.h to include module header
4. Test build
5. Commit
6. Repeat for next module

**Time estimate:** 8-12 hours

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
