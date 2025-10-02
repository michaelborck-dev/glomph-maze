# Refactoring Strategy - Naming and Modernization

## TL;DR - Recommended Approach

**Phase 2 (Current): Conservative Extraction**
- Keep original function names (`my_*`, `pager_*`, etc.)
- Keep original comments
- Focus on MOVING code, not rewriting it
- Minimal changes = minimal risk

**Phase 3 (Future): Rename & Modernize**
- After extraction complete and all tests passing
- Rename in bulk: `my_*` → `glomph_screen_*`
- Update comments: "MyMan" → "Glomph Maze"
- Modernize idioms safely

**Rationale:** Don't try to do everything at once!

---

## The Two Schools of Thought

### Option 1: Conservative (RECOMMENDED)

**Extract first, rename later**

```c
// Phase 2: Extract as-is
// src/render/screen.c
static void my_init(void) {
    // Original code, unchanged
    initscr();
    // ...
}
```

Then later:

```c
// Phase 3: Rename in bulk
// src/render/screen.c
void glomph_screen_init(void) {
    // Same code, new name
    initscr();
    // ...
}
```

**Pros:**
- ✅ Minimal changes = easier to verify correctness
- ✅ Can diff old vs new easily
- ✅ If something breaks, you know it's the extraction, not a rename
- ✅ Faster to complete Phase 2
- ✅ Can test old/new builds are identical

**Cons:**
- ⚠️ Still has "MyMan" references during Phase 2
- ⚠️ Two passes through the code

### Option 2: Rename as You Go

**Extract and rename simultaneously**

```c
// Phase 2: Extract and rename
// src/render/screen.c
void glomph_screen_init(void) {
    // Renamed during extraction
    initscr();
    // ...
}
```

**Pros:**
- ✅ Done in one pass
- ✅ Clean "Glomph" naming from the start

**Cons:**
- ❌ Harder to verify correctness (is bug from move or rename?)
- ❌ Can't easily diff against original
- ❌ More cognitive load during extraction
- ❌ Higher risk of introducing bugs
- ❌ Harder to maintain parallel builds

---

## Recommendation: Conservative Approach

### Phase 2: Extract with Minimal Changes

1. **Copy functions exactly as-is**
   - Keep function names: `my_init()`, `pager()`, `gamehelp()`
   - Keep variable names: `reinit_requested`, `pager_remaining`
   - Keep comments unchanged
   
2. **Only change what's necessary**
   - Add proper header guards
   - Fix include paths
   - Add module-level comments
   
3. **Focus on structural changes only**
   - Moving code between files
   - Creating clean interfaces
   - Ensuring compilation

### Phase 3: Rename & Modernize (After extraction complete)

Once all modules are extracted and working:

1. **Bulk rename functions** (tool-assisted)
   ```bash
   # Example: Rename all my_* functions
   sed -i 's/my_init/glomph_screen_init/g' src/render/screen.c
   ```

2. **Update comments**
   ```bash
   # Replace MyMan with Glomph Maze
   find src -name "*.c" -o -name "*.h" | xargs sed -i 's/MyMan/Glomph Maze/g'
   ```

3. **Modernize idioms** (carefully, one at a time)

---

## Naming Conventions for New Code

When you DO rename (Phase 3), use these conventions:

### Module Prefixes
```c
// render/screen.c
glomph_screen_init()
glomph_screen_clear()
glomph_screen_refresh()

// render/pager.c
glomph_pager_show()
glomph_pager_add_line()

// core/game_state.c
glomph_game_init()
glomph_game_get_score()
glomph_game_add_life()

// core/collision.c
glomph_collision_check()
glomph_collision_player_ghost()
```

### Naming Style
- **Functions**: `module_verb_noun()` (snake_case)
- **Structs**: `GlomphModuleName` (PascalCase)
- **Enums**: `GLOMPH_MODULE_CONSTANT` (UPPER_CASE)
- **Static functions**: `module_internal_function()` (snake_case, not exported)

### Example Header
```c
// render/screen.h
#ifndef GLOMPH_SCREEN_H
#define GLOMPH_SCREEN_H

#include <curses.h>

// Public API
void glomph_screen_init(void);
void glomph_screen_cleanup(void);
void glomph_screen_clear(void);
void glomph_screen_refresh(void);

// Color management
void glomph_screen_init_colors(void);
int glomph_screen_has_colors(void);

#endif
```

---

## Modern C Practices

### What C11 Gives You (We're Already Using It!)

Current: `set(CMAKE_C_STANDARD 11)`

**Available features:**
- `_Static_assert()` - Compile-time assertions
- Anonymous structs/unions
- `_Noreturn` function attribute
- `<stdatomic.h>` for threading
- `<threads.h>` for portable threads
- `<stdalign.h>` for alignment
- Better Unicode support

### What to Modernize (During Extraction)

1. **Replace magic numbers with enums**
   ```c
   // Old
   #define GHOST_STATE_CHASE 0
   #define GHOST_STATE_SCATTER 1
   
   // New
   typedef enum {
       GLOMPH_GHOST_CHASE = 0,
       GLOMPH_GHOST_SCATTER = 1,
       GLOMPH_GHOST_FRIGHTENED = 2
   } GlomphGhostState;
   ```

2. **Use `const` for read-only pointers**
   ```c
   // Old
   void pager_addstr(char *s, chtype a)
   
   // New
   void glomph_pager_add_str(const char *s, chtype a)
   ```

3. **Use `static` for internal functions**
   ```c
   // Functions not in header = static
   static void screen_internal_helper(void) {
       // Only visible in this file
   }
   ```

4. **Use `bool` instead of `int` for booleans**
   ```c
   #include <stdbool.h>
   
   // Old
   int has_colors = 0;
   
   // New
   bool has_colors = false;
   ```

5. **Use `size_t` for sizes/counts**
   ```c
   // Old
   int len = strlen(str);
   
   // New
   size_t len = strlen(str);
   ```

### What NOT to Change (Yet)

1. **Keep existing curses idioms**
   - `chtype`, `WINDOW*`, etc. are standard
   - Don't invent wrappers around wrappers

2. **Keep existing data structures**
   - Don't redesign while extracting
   - That's a separate refactoring

3. **Keep existing algorithms**
   - Don't optimize while moving code
   - Optimization is a separate phase

---

## Practical Example: Extracting screen.c

### Step 1: Phase 2 - Extract with minimal changes

```c
// src/render/screen.c
/*
 * screen.c - Curses wrapper for Glomph Maze
 * Copyright 2025, Michael Borck
 * 
 * Extracted from myman.c (original by Benjamin Sittler)
 */

#include "screen.h"

// KEEP ORIGINAL FUNCTION NAMES during extraction
static void my_init(void) {
    // Exact copy from myman.c
    initscr();
    nonl();
    cbreak();
    noecho();
    // ...
}

static void my_endwin(void) {
    // Exact copy from myman.c
    endwin();
}
```

```c
// src/render/screen.h
#ifndef SCREEN_H  // Will rename to GLOMPH_SCREEN_H in Phase 3
#define SCREEN_H

#include <curses.h>

// KEEP ORIGINAL NAMES - just declaring them here
void my_init(void);
void my_endwin(void);
void my_clear(void);
void my_refresh(void);
// ...

#endif
```

### Step 2: Guard old code in myman.c

```c
// src/myman.c
#ifndef USE_NEW_SCREEN
// Original implementations stay here
static void my_init(void) {
    initscr();
    // ...
}
#endif
```

### Step 3: Test both builds

```bash
# Old way (default)
cmake -B build
cmake --build build
./build/glomph-big --help

# New way (testing extraction)
cmake -B build-new -DUSE_NEW_SCREEN=ON
cmake --build build-new
./build-new/glomph-big --help

# Compare outputs - should be identical!
diff <(./build/glomph-big --help) <(./build-new/glomph-big --help)
```

### Step 4: Only after proven, rename (Phase 3)

```bash
# Bulk rename in screen.c
sed -i 's/my_init/glomph_screen_init/g' src/render/screen.c src/render/screen.h
sed -i 's/my_endwin/glomph_screen_cleanup/g' src/render/screen.c src/render/screen.h
# ... etc
```

---

## Decision Matrix

| Scenario | Recommendation |
|----------|----------------|
| **Extracting existing function** | Keep original name |
| **Creating new helper function** | Use new naming convention |
| **Fixing obvious bug** | Fix it (document in commit) |
| **Refactoring algorithm** | Do NOT refactor during extraction |
| **Updating comment** | Keep original unless factually wrong |
| **Improving variable name** | Keep original unless extremely confusing |
| **Adding const/bool** | Add them (low risk, high value) |
| **Changing data structure** | Do NOT change during extraction |

---

## Summary

### DO During Extraction (Phase 2):
- ✅ Copy functions exactly as written
- ✅ Keep original names and comments
- ✅ Add proper header guards and includes
- ✅ Use `const`, `bool`, `size_t` for new code
- ✅ Fix obvious bugs (document them!)
- ✅ Make it compile and pass tests

### DON'T During Extraction:
- ❌ Rename functions/variables
- ❌ Rewrite algorithms
- ❌ Change data structures
- ❌ Update all comments to say "Glomph"
- ❌ Optimize code
- ❌ Add new features

### DO After Extraction (Phase 3):
- ✅ Bulk rename with tools (sed, IDE refactoring)
- ✅ Update all comments
- ✅ Modernize idioms systematically
- ✅ Run full test suite after each category of changes

---

## Final Recommendation

**For Phase 2: Be conservative. Extract first, rename later.**

Reasoning:
1. You can verify correctness by comparing old vs new builds
2. If something breaks, you know it's structural, not a typo in a rename
3. You can complete Phase 2 faster
4. Renaming is EASY after extraction (tools + find/replace)
5. Lower cognitive load = fewer mistakes

**The goal of Phase 2 is modularization, not beautification.**

Save the beautification for Phase 3 when the structure is solid!

---

*Generated: 2025-10-02*  
*For: Glomph Maze Phase 2 Refactoring*
