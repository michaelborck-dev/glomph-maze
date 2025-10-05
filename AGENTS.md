# Glomph Maze - Agent Guidelines

## Build Commands

### CMake
First, install CMake: `brew install cmake`

- **Configure**: `cmake -B build -DCMAKE_BUILD_TYPE=Debug`
- **Configure with audio**: `cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_AUDIO=ON`
- **Build**: `cmake --build build` (first: ~30s, incremental: 1-2s)
- **Test**: `./build/glomph --help`
- **Test audio**: `./build/glomph -b` (with -DENABLE_AUDIO=ON)
- **Clean**: `rm -rf build`
- **Format**: `cmake --build build --target format`
- **Lint**: `cmake --build build --target lint`
- **Run tests**: `cd build && ctest`

See `docs/CMAKE_SETUP.md` for details.

## Lint/Test Commands
- **Format check**: `clang-format --dry-run --Werror src/*.c include/*.h`
- **Format fix**: `clang-format -i src/*.c include/*.h`
- **Lint**: `clang-tidy --fix --quiet src/*.c include/*.h`
- **Static analysis**: `cppcheck --enable=all --inconclusive --suppress=missingInclude src/ include/`
- **Test**: `./build/glomph --help` or `./build/glomph-xlarge --help` (basic smoke test)

## Audio Commands
- **Build with audio**: `cmake -B build -DENABLE_AUDIO=ON && cmake --build build`
- **Test audio**: `./build/glomph -b` (must build with -DENABLE_AUDIO=ON first)
- **Test script**: `scripts/test_audio.sh`

## Documentation Commands
- **Generate docs**: `./scripts/generate_docs_script.sh` (Doxygen + cflow)
- **View HTML docs**: `open docs/generated/html/index.html`
- **cflow outputs**: `docs/cflow/callgraph-*.txt`

## Changelog Commands
- **Generate changelog**: `./scripts/generate_changelog.sh` (regenerates CHANGELOG.md from git history)

## Current Refactoring Status

**Branch:** phase-5-render-module  
**Phase:** Phase 1 Cleanup Complete ✅

### What Changed:
- ✅ Phase 0-5: Modular extraction (9 source files created)
- ✅ SDL audio support added (optional CMake flag)
- ✅ Phase 1 Cleanup: Removed 275 lines of dead platform code
  - Windows/DOS/VMS support removed
  - Ancient compiler support removed
  - BUILTIN_* embedded build flags removed
  - NEED_* dead code removed
  - Feature detection simplified to constants

### Current File Sizes:
```
src/
├── input_state.c (44 lines)
├── render_state.c (39 lines)
├── sprite_io.c (528 lines)
├── maze_io.c (609 lines)
├── logic.c (920 lines) ✓
├── render.c (996 lines)
├── game_state.c (1,307 lines)
├── utils.c (1,899 lines) ✓
└── myman.c (5,224 lines) - down from 5,499
```

### Architecture:
```
include/
├── globals.h        (aggregator - includes all modules)
├── game_state.h     (score, lives, level, player state)
├── sprite_state.h   (sprite registers, ghost AI)
├── maze_state.h     (maze data, dimensions, loading)
├── render_state.h   (tiles, screen, colors, pager)
├── input_state.h    (keyboard, controls, timing)
└── utils.h          (macros, constants, function prototypes)

src/
├── myman.c          (5,224 lines - core game loop)
├── game_state.c     (game state management)
├── sprite_io.c      (sprite loading)
├── maze_io.c        (maze loading)
├── render.c         (rendering helpers)
├── render_state.c   (curses wrappers)
├── input_state.c    (timing utilities)
├── logic.c          (game rules/AI)
└── utils.c          (utilities)
```

**Next Phase:** Extract curses wrappers (~800 lines) OR continue #ifdef cleanup (~50-100 lines)

See `docs/REFACTORING_STATUS.md` for full details.

## Known Issues
None currently. All builds passing, tests green.

## Code Style Guidelines

### Formatting (.clang-format)
- **Base style**: LLVM with customizations
- **Indentation**: 4 spaces
- **Line length**: 80 columns maximum
- **Pointer alignment**: Left (e.g., `int* ptr`)
- **Include sorting**: Enabled
- **Braces**: Attach style
- **Standard**: C17

### Linting (.clang-tidy)
- **Enabled checks**: clang-analyzer-*, modernize-*, cppcoreguidelines-*
- **Disabled checks**: modernize-use-trailing-return-type, cppcoreguidelines-avoid-magic-numbers

### General Conventions
- **Language**: C17 (modern standard)
- **Platforms**: macOS, Linux (modern Unix only)
- **Curses**: ncurses only
- **Audio**: SDL2_mixer (optional) or terminal beep
- **Headers**: BSD license blocks at top of each file
- **Includes**: System headers first, then local headers
- **Error handling**: Standard C errno-based
- **Naming**: Standard C conventions (snake_case)
- **No emojis or decorative elements**

## Audio Support

### Build with Audio
```bash
brew install sdl2 sdl2_mixer              # macOS
sudo apt install libsdl2-dev libsdl2-mixer-dev  # Ubuntu

cmake -B build -DENABLE_AUDIO=ON
cmake --build build
./build/glomph -b  # -b enables audio
```

### Audio Backends
1. **SDL2_mixer** (-DENABLE_AUDIO=ON): MIDI/XM music playback
2. **Terminal beep** (default): Simple beep() from ncurses
3. **miniaudio** (future): Zero-dependency WAV/OGG playback

Press 'S' in-game to toggle sound on/off.

## Next Session Prompt

```
Resume Glomph Maze refactoring from Phase 1 cleanup completion.

Current state:
- myman.c reduced from 5,499 to 5,224 lines (-275 lines, -5%)
- Dead platform code removed (Windows, DOS, VMS, ancient compilers)
- BUILTIN_* and NEED_* flags removed
- SDL audio support added and working
- All tests passing (100%)

Next options:
1. Extract curses wrappers to src/curses_wrapper.c (~800 line reduction)
2. Continue #ifdef cleanup (~50-100 line reduction, safer)
3. Address minor warnings (unused variables, operator precedence)

What would you like to do next?
```
