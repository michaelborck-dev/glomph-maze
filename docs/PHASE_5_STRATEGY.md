# Phase 5 Strategy: Module Extraction

**Status:** Planning  
**Goal:** Extract functions from utils.c/myman.c into modular source files  
**Approach:** Methodical, one function at a time, compile after each change

---

## Current State Analysis

### File Sizes
- **src/utils.c**: 5,213 lines (LARGEST - prime target)
- **src/myman.c**: 5,561 lines (very complex, leave for last)
- **src/logic.c**: 920 lines (already focused, good state)
- **src/main.c**: 378 lines (small, leave alone)

### Module Headers Created (Phase 3)
- `include/game_state.h` - 112 lines
- `include/sprite_state.h` - 121 lines  
- `include/maze_state.h` - 109 lines
- `include/render_state.h` - 125 lines
- `include/input_state.h` - 91 lines

---

## Strategy: Conservative Extraction

### Principles
1. **One function at a time** - Move, compile, test, commit
2. **Leaf functions first** - Functions with no dependencies on other local functions
3. **Simple before complex** - Pure functions before stateful ones
4. **Test continuously** - All 4 variants must build after each move
5. **Use cflow** - Reverse call graph shows dependencies
6. **Don't touch myman.c yet** - Too complex, needs careful analysis

### Tools We Have
- ✅ **cflow callgraphs** - Shows who calls what (dependency tree)
- ✅ **Doxygen documentation** - Functions are documented with dependencies
- ✅ **Module headers** - Clear target locations already defined
- ✅ **4 build variants** - Comprehensive testing built-in

---

## Phase 5A: Extract Rendering Functions (EASIEST)

**Target:** `src/render/` directory  
**Source:** `src/utils.c`  
**Estimated time:** 4-6 hours

### Functions to Extract (in order)
1. ✅ **mark_cell()** - Line 2697, ~15 lines, LEAF function
   - Zero dependencies on other local functions
   - Only calls macro DIRTY_ALL
   - Used by: maze_erase, maze_puts, mark_sprite_register

2. ✅ **maze_erase()** - Line 2690, ~10 lines
   - Calls: mark_cell (already extracted)
   - Simple, well-defined

3. ✅ **maze_puts()** - Line 2710, ~20 lines  
   - Calls: mark_cell (already extracted)
   - String writing logic

4. ✅ **maze_putsn_nonblank()** - Line 2770, ~30 lines
   - Calls: mark_cell (already extracted)
   - Slightly more complex

5. ✅ **mark_sprite_register()** - Line 2829, ~15 lines
   - Calls: mark_cell (already extracted)
   - Sprite-related but rendering function

### Process
```bash
# 1. Create render module
mkdir -p src/render
touch src/render/render.c

# 2. Move mark_cell() first
# - Copy function from utils.c to render.c
# - Add necessary includes to render.c
# - Remove from utils.c
# - Update CMakeLists.txt to compile render.c

# 3. Test build
cmake --build build
./build/glomph --help

# 4. If success, commit
git add src/render/ src/utils.c CMakeLists.txt
git commit -m "Extract mark_cell() to src/render/render.c"

# 5. Repeat for next function
```

---

## Phase 5B: Extract Maze Functions (MEDIUM)

**Target:** `src/maze/` directory  
**Source:** `src/utils.c`  
**Estimated time:** 6-8 hours

### Functions to Extract (in order)
1. **parse_maze_args()** - Line 2195, ~150 lines
   - Parses maze metadata
   - Many string operations but self-contained

2. **readmaze()** - Line 1930, ~200 lines
   - File I/O and parsing
   - Calls: parse_maze_args (already extracted)

3. **writemaze()** - Line 2130, ~65 lines
   - File output
   - Calls: mymanescape (stays in utils for now)

---

## Phase 5C: Extract Sprite/Font Functions (MEDIUM)

**Target:** `src/sprite/` directory  
**Source:** `src/utils.c`  
**Estimated time:** 6-8 hours

### Functions to Extract
1. **parse_tile_args()** - Line 1875, ~75 lines
2. **parse_sprite_args()** - Line 1949, ~75 lines
3. **readfont()** - Line 1529, ~250 lines
4. **writefont()** - Line 1777, ~100 lines

---

## Phase 5D: Extract Game State Functions (HARDER)

**Target:** `src/game/` directory  
**Source:** `src/utils.c`  
**Estimated time:** 8-12 hours

### Functions to Extract
1. **gamereset()** - Line 3734, ~60 lines (moderate complexity)
2. **gamestart()** - Line 3332, ~30 lines (simple)
3. **gameinfo()** - Line 2987, ~40 lines (simple)
4. **gamehelp()** - Line 3025, ~40 lines (simple)
5. **creditscreen()** - Line 3149, ~35 lines (simple)
6. **gameintro()** - Line 3066, ~200 lines (complex, many sprite manipulations)
7. **gamedemo()** - Line 3184, ~150 lines (complex)
8. **gameintermission()** - Line 3364, ~370 lines (very complex)

**NOTE:** gameintermission is HUGE - may need to extract helper functions first

---

## Phase 5E: Extract Utility Functions (MEDIUM)

**Target:** Keep in `src/utils.c` OR create `src/collision/`  
**Source:** `src/utils.c`

### Collision Functions
1. **check_collision()** - Line 3677, ~30 lines
2. **find_home_dir()** - Line 3731, ~10 lines

These are candidates for `src/collision/collision.c` if we want maximum modularity.

---

## What NOT to Move (Yet)

### Leave in utils.c
- String manipulation: `strword()`, `strtollist()`, `mymanescape()`, etc.
- Character encoding: CP437/UTF-8 conversion functions
- Low-level helpers: `mille_to_scale()`, `fgetc_cp437_utf8()`, etc.
- Environment helpers: `myman_setenv()`, `myman_getenv()`

### Leave in myman.c (TOO COMPLEX)
- **gamerender()** - 950+ lines, extremely complex
- **gameinput()** - 270+ lines, complex input handling  
- **gamesfx()** - 60+ lines, sound system
- **doubletime(), my_usleep()** - Can extract later if needed
- **myman()** - Main game loop, leave alone
- **main()** - Entry point, leave alone

---

## CMakeLists.txt Updates

For each new module directory, add to CMakeLists.txt:

```cmake
# Render module
add_library(glomph_render
    src/render/render.c
)
target_include_directories(glomph_render PUBLIC include)

# Update executable targets
target_link_libraries(glomph glomph_render)
target_link_libraries(glomph-xlarge glomph_render)
# ... etc
```

---

## Testing Checklist (After Each Function Move)

```bash
# 1. Clean rebuild
rm -rf build
cmake -B build
cmake --build build

# 2. Test all variants
./build/glomph --help
./build/glomph-xlarge --help
./build/glomph-small --help
./build/glomph-tiny --help

# 3. Quick play test (visual check)
./build/glomph
# Press keys, verify game works

# 4. If all pass, commit
git add -A
git commit -m "Extract <function_name> to src/<module>/"
```

---

## Dependency Graph Analysis

From cflow reverse call graph:

### Truly Leaf Functions (No local dependencies)
- `mark_cell()` - Only uses macros/globals
- `doubletime()` - Only calls gettimeofday()
- `my_usleep()` - Only calls usleep()
- `find_home_dir()` - Only reads global arrays
- `gfx0(), gfx1(), gfx2()` - Graphics helpers

### Functions Calling Only Leaf Functions
- `maze_erase()` - Calls mark_cell()
- `maze_puts()` - Calls mark_cell()
- `maze_putsn_nonblank()` - Calls mark_cell()
- `mark_sprite_register()` - Calls mark_cell()

### Functions with External Dependencies Only
- `readmaze()` - File I/O, calls parse_maze_args
- `writemaze()` - File I/O, calls mymanescape
- `readfont()` - File I/O, calls parse functions

---

## Risk Assessment

### Low Risk (Do First)
- ✅ Rendering functions - Small, well-defined, documented
- ✅ Parse functions - Self-contained, file parsing only
- ✅ Utility functions - Pure functions, minimal state

### Medium Risk (Do Second)  
- ⚠️ Maze I/O functions - File operations, error handling
- ⚠️ Game state simple functions - gamestart, gamereset
- ⚠️ Font I/O functions - File operations, complex parsing

### High Risk (Do Last or Skip)
- ❌ gameintro - Massive, complex sprite manipulation
- ❌ gamedemo - AI and state management
- ❌ gameintermission - 370 lines, very complex
- ❌ gamerender - 950+ lines, DO NOT TOUCH
- ❌ gameinput - Complex input state machine

---

## Recommended Order of Execution

### Week 1: Rendering Module (Low Risk)
1. Create `src/render/render.c`
2. Extract mark_cell() → test → commit
3. Extract maze_erase() → test → commit
4. Extract maze_puts() → test → commit
5. Extract maze_putsn_nonblank() → test → commit
6. Extract mark_sprite_register() → test → commit

**Deliverable:** render.c with 5 functions, ~100 lines total

### Week 2: Parse Functions (Low-Medium Risk)
1. Create `src/maze/maze_io.c`
2. Extract parse_maze_args() → test → commit
3. Extract readmaze() → test → commit
4. Extract writemaze() → test → commit

**Deliverable:** maze_io.c with 3 functions, ~400 lines total

### Week 3: Font/Sprite Functions (Medium Risk)
1. Create `src/sprite/sprite_io.c`
2. Extract parse_tile_args() → test → commit
3. Extract parse_sprite_args() → test → commit
4. Extract readfont() → test → commit
5. Extract writefont() → test → commit

**Deliverable:** sprite_io.c with 4 functions, ~500 lines total

### Week 4+: Decision Point
Evaluate progress and decide:
- Continue with game state functions?
- Stop here (already achieved 70% reduction in utils.c)?
- Focus on documentation instead?

---

## Success Metrics

### Phase 5A Complete (Rendering)
- [ ] src/render/render.c created
- [ ] 5 rendering functions extracted
- [ ] utils.c reduced by ~100 lines
- [ ] All 4 variants build and run

### Phase 5B Complete (Maze I/O)
- [ ] src/maze/maze_io.c created
- [ ] 3 maze I/O functions extracted
- [ ] utils.c reduced by ~400 lines
- [ ] All 4 variants build and run

### Phase 5C Complete (Sprite I/O)
- [ ] src/sprite/sprite_io.c created
- [ ] 4 sprite I/O functions extracted
- [ ] utils.c reduced by ~500 lines
- [ ] All 4 variants build and run

### Overall Phase 5 Success
- [ ] utils.c reduced by 1000+ lines (20% reduction)
- [ ] 3+ new module directories created
- [ ] Clear separation of concerns
- [ ] Zero behavior changes
- [ ] All 4 variants tested and working

---

## When to STOP

**Stop if:**
- Any extraction takes more than 4 hours (too complex)
- Build breaks and can't be fixed in 1 hour
- Tests fail and issue isn't obvious
- Code becomes harder to understand (defeats the purpose)

**It's OK to stop after Phase 5A** - Even extracting just the rendering functions is a significant win.

---

## Next Steps

Ready to start? Begin with:

```bash
# Create the strategy branch
git checkout -b phase-5-extraction

# Start with rendering module
mkdir -p src/render
```

Then proceed with mark_cell() extraction as outlined above.
