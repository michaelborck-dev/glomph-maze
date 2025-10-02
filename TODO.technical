# Technical Implementation Notes

**Note:** This document contains technical specifications from the original MyMan project (2003-2009). Many of these ideas were exploratory and may no longer be applicable after refactoring or due to removal of legacy platform support (DOS, VMS, EFI, etc.). These notes are preserved for historical reference and potential inspiration for future enhancements.

## Core Feature Ideas

### Input Recording and Replay
**Purpose:** Enable automated screenshot generation and regression testing

Allow recording of player input (keyboard/controller) during gameplay, then replay it exactly to reproduce game state. This would enable:
- Automated screenshot generation for releases
- Regression testing to ensure game logic remains consistent
- Demo mode playback

### Save/Restore Game State
**Purpose:** Allow players to save progress and resume later

Implement serialization of complete game state (player position, ghost states, score, level, pellets remaining, timers) to disk, allowing the game to be paused and resumed later.

### Runtime Maze Switching
**Purpose:** Change levels without restarting the game

Add an in-game "command line" editor that allows switching to different maze files on-the-fly. Might be implemented via exec'ing a new process with different parameters, or by reloading maze data.

### Level Editor
**Purpose:** Create custom mazes without editing text files

Build a graphical or text-based tool for designing custom maze layouts, placing walls, dots, power pellets, and spawn points interactively.

---

## Rendering System Improvements

### Bicolor Maze Enhancement
**Purpose:** Better visual distinction for special maze variant

For mazes with the "~4" flag (bicolor mode), use proximity to edges/gates rather than wall width to determine color. This would provide more intuitive visual cues about maze geometry.

### Display Optimization
**Purpose:** Reduce screen flicker and improve performance

- **Double buffering:** Render to off-screen buffer first, then swap to display
- **Frame-skipping:** Drop frames when falling behind to maintain gameplay speed
- **Redraw avoidance:** Track which screen areas changed and only update those regions
- **Smart status line:** Determine early if plain-text status line will display and avoid scrolling extra line if not needed

### Flip Scrolling
**Purpose:** Keep player centered on screen

Divide screen into 9 regions (nonants). Scroll in large steps to keep player character in the center region, providing better visibility of surroundings.

### Transparency and Background
**Purpose:** Improve visual appearance

- Make black background the default
- Make transparency optional instead of default
- This would reduce visual noise and improve contrast

---

## Code Architecture Refactoring

### Backend Modularization
**Purpose:** Separate rendering code from game logic

**Current issue:** Game logic and rendering (curses/SDL/etc.) are tightly coupled in monolithic source files.

**Proposed solution:**
1. Extract backend-independent code (game logic, maze parsing, collision detection) into separate files
2. Move each backend implementation (ncurses, SDL, PDCurses, etc.) into its own file
3. Compile backends conditionally based on available libraries
4. Define clean interface between game logic and rendering

This would make the codebase more maintainable and make it easier to add new rendering backends.

### Bitmap Renderer Integration
**Purpose:** Simplify build system

Fold the bitmap renderer code directly into the main codebase rather than keeping it as a separate module. This would reduce complexity and make the code easier to understand.

### Memory Leak Fixes
**Purpose:** Prevent memory leaks during long play sessions

Fix memory leaks primarily found in file parsers (maze loading, tile loading, sprite loading). These leaks are minor for short sessions but could accumulate during extended play or repeated level loading.

---

## Advanced Bitmap Rendering Modes

**Note:** These are highly technical specifications for terminal-based bitmap rendering using Unicode block characters. May not be relevant after refactoring to modern graphics.

### Rendering Mode Matrix

The original author explored various ways to pack pixels into terminal cells using Unicode characters:

| Mode | Description | Pixels per Cell | Unicode Chars Used |
|------|-------------|-----------------|-------------------|
| **Standard** | Normal character cells | 1x2 | Standard ASCII |
| **Quadrant** | 2x2 blocks per cell | 2x2 | U+2596-U+259F quadrant blocks |
| **Braille** | Braille patterns | 2x4 | U+2800-U+28FF braille patterns |
| **Half-block** | Vertical/horizontal splits | 2x1 or 1x2 | U+2580-U+2590 block elements |
| **Double** | Compressed horizontal | varies | Combination of above |

### Unicode Block Characters Reference

**Half Blocks (CP437 compatible):**
```
▀ U+2580 UPPER HALF BLOCK
▄ U+2584 LOWER HALF BLOCK
█ U+2588 FULL BLOCK
▌ U+258C LEFT HALF BLOCK
▐ U+2590 RIGHT HALF BLOCK
```

**Quadrant Blocks (2x2 pixels per cell):**
```
▖ U+2596 QUADRANT LOWER LEFT
▗ U+2597 QUADRANT LOWER RIGHT
▘ U+2598 QUADRANT UPPER LEFT
▙ U+2599 QUADRANT UPPER LEFT AND LOWER LEFT AND LOWER RIGHT
▚ U+259A QUADRANT UPPER LEFT AND LOWER RIGHT
▛ U+259B QUADRANT UPPER LEFT AND UPPER RIGHT AND LOWER LEFT
▜ U+259C QUADRANT UPPER LEFT AND UPPER RIGHT AND LOWER RIGHT
▝ U+259D QUADRANT UPPER RIGHT
▞ U+259E QUADRANT UPPER RIGHT AND LOWER LEFT
▟ U+259F QUADRANT UPPER RIGHT AND LOWER LEFT AND LOWER RIGHT
```

**Braille Patterns (2x4 pixels per cell):**
```
⊻ U+22BB (example)
Braille pattern dots: U+2800 ... U+28FF
Dot positions:
1 4
2 5
3 6
7 8
```

These characters allow packing multiple pixels into a single terminal cell, enabling higher-resolution graphics in text mode. Modern terminals with good Unicode support could render reasonably detailed sprites using these techniques.

---

## Color Palette Management

**Note:** This section contains complex palette allocation algorithms for terminals with limited color support. Modern terminals typically support 256 or true color, making much of this obsolete.

### Purpose
The original game needed to work on terminals with only 8 or 16 colors. These algorithms determined how to map game elements (walls, pellets, ghosts, player) to available colors, with fallback strategies when colors were unavailable.

### Notation
```
XY = foreground X on background Y
!XY = reverse video (swap fg/bg)
^XY = bold/bright
vXY = dim
```

### Color Codes
```
0=black  1=red     2=green  3=yellow
4=blue   5=magenta 6=cyan   7=white
8-F = bright versions
```

### Allocation Strategy
The cryptic notation like "XB -> !BX" represents fallback rules:
- If we need foreground X on background B but it's not available
- Try reverse video (background B, foreground X)
- If that doesn't work, try dim or bold variants
- Keep falling back until we find a working combination

This ensured the game was playable even on very limited terminals, though colors might not match the intended design.

**Modern approach:** With 256-color or true-color terminal support, these complex fallbacks are unnecessary. A simpler palette system targeting modern terminals would be more appropriate.

---

## Other Ideas

### Level Printer Numeric Suffix
**Purpose:** Disambiguate similar maze names

When displaying maze names and multiple mazes have similar names, append a number suffix if there's enough screen space. Example: "maze-1", "maze-2" instead of just "maze".

### External Variant/Size Lists
**Purpose:** Reduce binary size and improve maintainability

Move the hard-coded lists of game variants (pacman, quackman, etc.) and tile sizes (8x8, 16x16, etc.) from compiled code into external data files. This would make it easier to add new variants without recompiling.

### ANSI Escape Support in Data Files
**Purpose:** Richer tile and sprite coloring

Allow maze/tile/sprite data files to include ANSI escape codes for per-character coloring, enabling gradients, highlights, and more detailed graphics without changing the rendering engine.

### Optional Bright Background via Blinking
**Purpose:** Work around terminal color limitations

Some old terminals couldn't display bright background colors but could display blinking text. This option would use the blink attribute to simulate bright backgrounds. (Likely not useful on modern terminals.)

---

## Conclusion

Many of these ideas reflect the constraints of 2000s-era terminal technology and cross-platform compatibility requirements that no longer apply. After refactoring, prioritize:

1. Modern terminal support (256-color, true color)
2. Clean separation of game logic and rendering
3. Maintainable, well-documented code
4. Features that enhance gameplay rather than work around platform limitations

Preserve the spirit of terminal-based gameplay while embracing modern development practices.
