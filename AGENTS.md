# Glomph Maze - Agent Guidelines (Python Rewrite)

## Project Overview

**Branch:** python-rewrite  
**Language:** Python 3.11+  
**Status:** Active development (v0.1.0)

This is a modern Python rewrite of Glomph Maze, preserving all 655 game assets (mazes, tiles, sprites, sounds) from the original C version while providing clean, maintainable code.

## Environment Setup

### Prerequisites
```bash
# Install uv (modern Python package manager)
curl -LsSf https://astral.sh/uv/install.sh | sh

# Or via Homebrew
brew install uv
```

### Initial Setup
```bash
# Create virtual environment and install dependencies
uv venv
source .venv/bin/activate  # On Windows: .venv\Scripts\activate
uv pip install -e ".[dev]"
```

## Development Commands

### Running the Game
```bash
# Run from source
python -m glomph.main

# Or using the installed entry point
glomph
```

### Code Quality

#### Formatting (Ruff)
```bash
# Check formatting
ruff format --check .

# Apply formatting
ruff format .
```

#### Linting (Ruff)
```bash
# Run linter
ruff check .

# Fix auto-fixable issues
ruff check --fix .
```

#### Type Checking (mypy)
```bash
# Run type checker
mypy src/glomph
```

#### All Quality Checks
```bash
# Run all checks at once
ruff format --check . && ruff check . && mypy src/glomph
```

### Testing (pytest)
```bash
# Run all tests
pytest

# Run with coverage
pytest --cov

# Run specific test file
pytest tests/test_loaders.py

# Run with verbose output
pytest -v

# Run tests matching pattern
pytest -k "loader"
```

### Package Building
```bash
# Build source distribution and wheel
python -m build

# Install in development mode
uv pip install -e .
```

### Distribution
```bash
# Build for PyPI
python -m build

# Upload to PyPI (requires twine)
twine upload dist/*

# Upload to TestPyPI first
twine upload --repository testpypi dist/*
```

## Project Structure

```
glomph-maze/
├── src/
│   └── glomph/
│       ├── __init__.py      # Package initialization
│       ├── main.py          # Entry point
│       ├── loaders.py       # Asset loaders (mazes, tiles, sprites)
│       ├── terminal.py      # Terminal/curses abstraction (TODO)
│       ├── game.py          # Game logic (TODO)
│       ├── entities.py      # Player, ghosts, etc. (TODO)
│       └── renderer.py      # Rendering engine (TODO)
├── tests/
│   ├── __init__.py
│   └── test_loaders.py      # Loader tests
├── assets/                  # 655 game assets (shared with C version)
│   ├── mazes/              # 259 maze files
│   ├── tiles/              # 44 tile sets
│   ├── sprites/            # 44 sprite sheets
│   └── sounds/             # 19 MIDI files
├── docs/                    # Documentation
├── pyproject.toml           # Project configuration
├── README.md                # User documentation
└── AGENTS.md                # This file
```

## Code Style Guidelines

### General Principles
- **Type hints**: Use strict typing (enforced by mypy)
- **Docstrings**: Google style for all public functions/classes
- **Line length**: 88 characters (Black/Ruff default)
- **Imports**: Sorted by ruff (isort compatible)
- **Naming**: snake_case for functions/variables, PascalCase for classes

### Example Code Style
```python
from pathlib import Path
from typing import NamedTuple


class GameState(NamedTuple):
    """Represents the current game state."""

    score: int
    lives: int
    level: int


def load_maze(path: Path) -> str:
    """Load maze content from file.
    
    Args:
        path: Path to maze file
        
    Returns:
        Maze content as string
        
    Raises:
        FileNotFoundError: If maze file doesn't exist
    """
    with open(path, encoding="utf-8") as f:
        return f.read()
```

## Asset Information

All 655 assets are **CC0 Public Domain** (confirmed in README_ORIG.md):
- **259 mazes** - UTF-8 text files (.txt, .asc)
- **44 tile sets** - UTF-8 text files
- **44 sprite sheets** - UTF-8 text files  
- **19 sound files** - Standard MIDI (.mid)

Assets are plain text/standard formats and work identically with Python.

## Development Workflow

### 1. Make Changes
```bash
# Edit code in src/glomph/
# Add tests in tests/
```

### 2. Run Quality Checks
```bash
ruff format .
ruff check --fix .
mypy src/glomph
```

### 3. Run Tests
```bash
pytest --cov
```

### 4. Commit
```bash
git add .
git commit -m "feat: add feature description"
```

## Current Status

### ✅ Completed
- [x] Project structure (pyproject.toml, src layout)
- [x] Asset loaders (MazeLoader, TileLoader, SpriteLoader)
- [x] Basic tests for loaders
- [x] Development tooling (ruff, mypy, pytest)

### 🚧 In Progress
- [ ] Terminal abstraction layer (curses wrapper)
- [ ] Game state management
- [ ] Entity system (player, ghosts)
- [ ] Rendering engine
- [ ] Input handling
- [ ] Audio support (pygame or similar)

### 📋 Planned
- [ ] Complete game loop implementation
- [ ] AI for ghost behavior
- [ ] Collision detection
- [ ] Scoring system
- [ ] PyInstaller packaging for standalone binary
- [ ] Homebrew formula for distribution

## Comparison: C vs Python

| Feature | C Version | Python Version |
|---------|-----------|----------------|
| Lines of Code | 11,399 | ~1,500 (target) |
| Build Time | ~30s | Instant |
| Binary Size | 200KB | ~15MB (PyInstaller) |
| Startup Time | Instant | ~1s |
| Maintainability | Challenging | High |
| Type Safety | Manual | mypy enforced |
| Testing | Manual | pytest + coverage |
| Distribution | Binary | PyInstaller / PyPI |

## Resources

- **Python Docs**: https://docs.python.org/3/
- **uv Docs**: https://github.com/astral-sh/uv
- **Ruff Docs**: https://docs.astral.sh/ruff/
- **mypy Docs**: https://mypy.readthedocs.io/
- **pytest Docs**: https://docs.pytest.org/

## License

- **Python Code**: MIT License
- **Assets**: CC0 Public Domain (from original MyMan project)
- **C Version**: BSD 2-Clause (preserved on main branch)
