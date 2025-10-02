# Roadmap

This document outlines potential future improvements for Glomph Maze, to be considered after the initial refactoring is complete.

## High Priority

### Core Gameplay
- [ ] **Save/Load game state** - Allow players to save progress and resume later
- [ ] **Input recording and replay** - Enable demo playback and regression testing
- [ ] **Runtime maze switching** - Switch mazes without restarting the game

### Code Quality
- [ ] **Complete refactoring** - See REFACTOR_PLAN.md for detailed phases
- [ ] **Fix memory leaks** - Address leaks in file parsers
- [ ] **Modernize command-line parsing** - Replace mygetopt with getopt_long()

## Medium Priority

### Features
- [ ] **Level editor** - Create and edit custom mazes
- [ ] **Configuration dialog** - In-game settings menu
- [ ] **Enhanced rendering options** - Double buffering, frame-skipping, redraw optimization

### Architecture
- [ ] **Separate backend code** - Extract backend-independent code to separate files
- [ ] **Modular backend compilation** - Compile backend implementations conditionally
- [ ] **Native bitmap support** - Improve graphics rendering

## Low Priority

### Visual Improvements
- [ ] **Per-character/tile coloring** - ANSI escape support in data files
- [ ] **Advanced bitmap renderer** - Braille patterns, quadrant blocks (see technical notes)
- [ ] **Black background default** - With optional transparency
- [ ] **Smart status line** - Determine early if status line will display

### Maze Features
- [ ] **Bicolor maze improvements** - Use edge/gate proximity for "~4" flag
- [ ] **Level printer enhancements** - Numeric suffix when ambiguous
- [ ] **Flip scrolling** - Keep focus centered using nonant-based scrolling

### Data Management
- [ ] **External v/s lists** - Move variant/size lists to separate files

## Technical Notes

The original TODO contained detailed technical specifications for:
- Bitmap rendering modes (double, doubleheight, braille, quadrant)
- Palette allocation algorithms
- Unicode block characters for rendering

These specifications are preserved in `TODO.technical` for reference during implementation.

## Contributing

When working on these features:
1. Review REFACTOR_PLAN.md first - many improvements require refactoring
2. Check TODO.technical for low-level implementation details
3. Follow the code style guidelines in AGENTS.md
4. Test with multiple maze variants and tile sizes
