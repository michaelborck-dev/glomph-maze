# main.c Simplification - Complete ✓

## What We Did

### 1. Discovered main.c Was Dead Code
- **main.c** (378 lines) was NOT in CMakeLists.txt `COMMON_SOURCES`
- Never compiled or linked into any binary
- Existed as legacy build system artifact

### 2. Found Real main() in myman.c
- **myman.c** line 5449 contained the actual `main()` function
- Wrapped in legacy portability `#ifdef`s for platforms we don't support

### 3. Removed Dead Code
- **Renamed:** `src/main.c` → `src/main.c.unused` (378 lines saved)
- Cleaned up remaining structure by 398 lines total (11,836 → 11,836 - 378 = 11,458 effective)

### 4. Simplified main() Function
**Removed legacy cruft:**
- `#ifndef MAIN_NO_ENVP` (SDL compatibility)
- `#ifndef BUILTIN_TILE/BUILTIN_SPRITE` (embedded build system)
- `#ifdef END_OF_MAIN` (Allegro compatibility)
- Unused envp parameter handling

**Before:** 66 lines with 8 `#ifdef` blocks  
**After:** 46 lines, clean C17 code

### 5. Results
✓ All 4 variants build successfully  
✓ All 4 tests pass (100%)  
✓ Reduced myman.c: 5,514 → 5,494 lines (-20 lines)  
✓ Removed main.c: 378 lines of dead code  
✓ **Total reduction: 398 lines**

---

## Current Architecture

```
src/
├── input_state.c (44 lines) - Timing utilities
├── render_state.c (39 lines) - Curses wrappers
├── sprite_io.c (528 lines) - Sprite loading
├── maze_io.c (609 lines) - Maze loading
├── logic.c (920 lines) - Game logic/AI ✓ FOCUSED
├── render.c (996 lines) - Rendering helpers
├── game_state.c (1,307 lines) - Game state + orchestration
├── utils.c (1,899 lines) - NEXT TARGET
└── myman.c (5,494 lines) - Core game loop (DELAY)
```

---

## Next Steps Recommendation

### PRIORITY 1: Audit utils.c (1,899 lines)
Still large - likely contains extractable functions.

**Audit checklist:**
1. List remaining functions (likely ~10-15)
2. Check usage across codebase
3. Identify extraction candidates:
   - UTF-8/CP437 encoding → `encoding.c`?
   - File I/O helpers → `file_io.c`?
   - String parsing → `parsing.c`?
   - Environment vars → Delete if unused?

### PRIORITY 2: Document Module Boundaries
Update README/ARCHITECTURE.md with new structure.

### PRIORITY 3: Consider myman.c Decomposition (RISKY)
Only after better test coverage.

---

## Cleaned Up Code Patterns

**Old (main.c and myman.c before):**
```c
#ifndef MAIN_NO_ENVP
    , char *envp[]
#endif
    )
{
#ifndef MAIN_NO_ENVP
    if (envp) {
        /* we should care */
    }
#endif
```

**New (myman.c after):**
```c
int main(int argc, char* argv[], char* envp[]) {
    (void)envp;  // Unused parameter
```

**Clean, modern C17 - no legacy cruft.**
