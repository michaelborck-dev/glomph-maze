# Scripts

Utility scripts for Glomph Maze development and asset creation.

## Python Scripts (Asset Creation)

### asc2txt.py
**Purpose**: Convert plain ASCII maze layouts to Unicode box-drawing format

**Usage**:
```bash
python3 scripts/asc2txt.py [options] [input_file]

# Convert maze with default settings
python3 scripts/asc2txt.py < my_maze.asc > assets/mazes/my_maze.txt

# Use single-thickness lines
python3 scripts/asc2txt.py --single < maze.asc > output.txt

# Use double-thickness lines
python3 scripts/asc2txt.py --double < maze.asc > output.txt
```

**Options**:
- `-h, --help`: Show help message
- `-1, --single`: Use single-thickness lines exclusively
- `-2, --double`: Use double-thickness lines exclusively
- `-3, --both`: Use both single and double-thickness (default)
- `-t, --traditional`: Use double outer maze walls
- `-m, --modern`: Use single outer maze walls
- `-f, --fast`: Skip expensive filling heuristic
- `-s, --slow`: Use expensive filling heuristic (default)
- `-o FILE, --output=FILE`: Write output to FILE instead of stdout

**Input Format**:
- Plain ASCII art using `#` for walls, spaces for paths
- Automatically converts to Unicode box-drawing characters (┌ ┐ └ ┘ ─ │ ├ ┤ ┬ ┴ ┼)
- Fills in dots and power pellets automatically

### bitmap2.py
**Purpose**: Vertical tile compression (2 rows → 1 using half-block characters)

**Usage**:
```bash
python3 scripts/bitmap2.py < input.txt > output.txt
```

**How it works**:
- Combines two rows of bitmap data into one using Unicode half-block characters (▀ ▄)
- Reduces vertical resolution by half
- Useful for creating compact tile sets

### bitmap4.py
**Purpose**: Horizontal tile compression (2 columns → 1 using half-block characters)

**Usage**:
```bash
python3 scripts/bitmap4.py < input.txt > output.txt
```

**How it works**:
- Combines two columns of bitmap data into one using Unicode half-block characters (▌ ▐)
- Reduces horizontal resolution by half
- Useful for creating narrow tile sets

## Shell Scripts (Maintenance)

### generate_changelog.sh
**Purpose**: Generate CHANGELOG.md from git commit history

**Usage**:
```bash
./scripts/generate_changelog.sh
```

**How it works**:
- Reads git commit history
- Groups commits by version/tag
- Formats as Markdown changelog
- Overwrites `CHANGELOG.md` in project root

**When to use**:
- Before creating a new release
- After significant development milestones
- When updating project documentation

## Requirements

**Python scripts**:
- Python 3.6+ (no external dependencies)
- Standard library only (sys, os, getopt)

**Shell scripts**:
- Bash-compatible shell
- Git (for generate_changelog.sh)

## Examples

### Creating a Custom Maze

1. Draw your maze in plain ASCII:
```
#################
#...............#
#.##.###.###.##.#
#.##.........##.#
#...............#
#################
```

2. Convert to game format:
```bash
python3 scripts/asc2txt.py < my_maze.asc > assets/mazes/my_maze.txt
```

3. Test in game:
```bash
cd build
./bigman  # Your maze will be available in the maze selection
```

### Compressing Tile Graphics

If you have large tile graphics and want to save space:

```bash
# Compress vertically (good for tall tiles)
python3 scripts/bitmap2.py < assets/tiles/my_tiles.txt > assets/tiles/my_tiles_compact.txt

# Compress horizontally (good for wide tiles)
python3 scripts/bitmap4.py < assets/tiles/my_tiles.txt > assets/tiles/my_tiles_narrow.txt
```

## Contributing

When adding new scripts:
1. Follow existing naming conventions
2. Include `--help` option for command-line tools
3. Update this README with usage instructions
4. Add appropriate shebang line (`#!/usr/bin/env python3` or `#!/bin/bash`)
5. Make scripts executable: `chmod +x scripts/your_script.sh`

## C Utilities (Font Tools)

### fnt2bdf.c
**Purpose**: Convert 8x8 .fnt console font files to BDF (Bitmap Distribution Format)

**Compilation**:
```bash
gcc -o fnt2bdf scripts/fnt2bdf.c
```

**Usage**:
```bash
./fnt2bdf fontfile.fnt > output.bdf

# Or from stdin
cat fontfile.fnt | ./fnt2bdf - > output.bdf
```

**How it works**:
- Reads binary .fnt console font files (8x8 character bitmaps)
- Converts to BDF format for use with X11 and other systems
- Useful for font development and conversion

**When to use**:
- Converting console fonts to X11-compatible format
- Creating custom font assets for the game
- Font development and testing
