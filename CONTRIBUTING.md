# Contributing to Glomph Maze

Thank you for your interest in contributing to Glomph Maze! This document provides guidelines for contributing to the project.

## Getting Started

### Prerequisites
- CMake 3.10 or higher
- C compiler (GCC or Clang)
- ncurses library
- Python 3 (for utility scripts)
- Git

### Building the Project

```bash
# Clone the repository
git clone git@github.com:michaelborck-dev/glomph-maze.git
cd glomph-maze

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Run the game
./build/glomph-big
```

See [docs/CMAKE_SETUP.md](docs/CMAKE_SETUP.md) for detailed build instructions and [docs/QUICKSTART.md](docs/QUICKSTART.md) for a quick reference.

## Development Workflow

### 1. Code Style
We follow strict C code style guidelines:

- **Standard**: C11 with ANSI C compatibility
- **Indentation**: 4 spaces (no tabs)
- **Line length**: 80 characters maximum
- **Naming**: `snake_case` for functions and variables
- **Comments**: Explain *why*, not *what*
- **No emojis or decorative elements**

See [AGENTS.md](AGENTS.md) for complete style guidelines.

### 2. Before Committing

Run formatting and linting:

```bash
# Format code
clang-format -i src/*.c inc/*.h

# Lint code
clang-tidy --fix --quiet src/*.c inc/*.h

# Build and test
cmake --build build
./build/glomph-big --help
```

### 3. Commit Messages

Write clear, concise commit messages:

```
Brief summary (50 chars or less)

More detailed explanation if needed. Wrap at 72 characters.
Explain what changed and why, not how.
```

Examples:
- ✅ `Fix ghost collision detection in narrow corridors`
- ✅ `Add support for custom maze file loading`
- ❌ `Update code`
- ❌ `Fix bug`

### 4. Pull Requests

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature-name`
3. Make your changes following the code style
4. Test your changes thoroughly
5. Commit with clear messages
6. Push to your fork: `git push origin feature/your-feature-name`
7. Open a Pull Request with a clear description

**PR Description should include:**
- What problem does this solve?
- How does it solve it?
- Any breaking changes?
- Testing performed

## Project Structure

```
glomph-maze/
├── src/           # Source code
├── inc/           # Header files
├── assets/        # Game assets
│   ├── tiles/     # Character/tile data
│   ├── mazes/     # Maze layouts
│   ├── sprites/   # Sprite graphics
│   └── sounds/    # Sound effects
├── scripts/       # Utility scripts (Python & shell)
├── build/         # Build output (generated)
└── docs/          # Documentation
```

## Types of Contributions

### Bug Reports
- Use GitHub Issues
- Include: OS, build commands, expected vs actual behavior
- Provide minimal reproduction steps

### Feature Requests
- Check [docs/ROADMAP.md](docs/ROADMAP.md) first
- Discuss in GitHub Issues before implementing
- Consider if it aligns with project goals (terminal-based gameplay)

### Code Contributions

**High Priority:**
- Bug fixes
- Memory leak fixes
- Code refactoring (see [docs/REFACTOR_PLAN.md](docs/REFACTOR_PLAN.md))
- Documentation improvements

**Welcome:**
- New maze layouts (in lvl/)
- Custom tile/sprite sets (in chr/spr/)
- Performance improvements
- Cross-platform compatibility fixes

**Discuss First:**
- Major architectural changes
- New dependencies
- GUI or graphics beyond terminal rendering
- Changes to game mechanics

## Refactoring

This project is undergoing a major refactoring effort. See [docs/REFACTOR_PLAN.md](docs/REFACTOR_PLAN.md) for:
- Current phase and progress
- Areas that need work
- How to contribute to refactoring

Key principles:
1. Separate game logic from rendering
2. Modularize monolithic files
3. Improve maintainability
4. Preserve terminal-based gameplay

## Testing

Currently, testing is manual:
- Run the game with different maze variants
- Test with different tile sizes (glomph-big, glomph-huge, glomph-small, glomph-narrow)
- Verify controls work correctly
- Check for memory leaks with valgrind (if available)

Automated testing is planned for the future (see docs/ROADMAP.md).

## Custom Mazes and Tiles

Want to create custom content?

**Maze files (assets/mazes/):**
- ASCII art format
- Use Python utility: `scripts/asc2txt.py --help`

**Tile/sprite files (assets/tiles/, assets/sprites/):**
- ASCII art format
- Use Python utilities: `scripts/bitmap2.py`, `scripts/bitmap4.py`

See existing files for examples.

## Questions?

- Check [README.md](README.md) first
- Review [docs/](docs/) for detailed documentation
- Open a GitHub Issue for discussion
- Reference [docs/TECHNICAL_NOTES.md](docs/TECHNICAL_NOTES.md) for implementation ideas

## License

By contributing, you agree that your contributions will be licensed under the same BSD 2-Clause License that covers the project. See [LICENSE](LICENSE) for details.

---

Thank you for contributing to Glomph Maze! 🎮
