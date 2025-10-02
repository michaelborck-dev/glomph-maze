# Assets

This directory contains all game runtime data files used by Glomph Maze.

## Structure

```
assets/
├── tiles/      - Character/tile graphics (walls, dots, pellets)
├── mazes/      - Level layouts and maze designs
├── sprites/    - Player and ghost sprite graphics
├── sounds/     - Sound effects (MIDI files)
└── graphics/   - Source graphics files (XCF, GIF, SVG)
```

## File Formats

### Mazes (mazes/)
- **Format**: ASCII art with Unicode box-drawing characters
- **Extension**: `.asc` (source), `.txt` (converted)
- **Structure**: Grid layout with walls, paths, dots, power pellets, and spawn points

### Tiles (tiles/)
- **Format**: ASCII art representing tile graphics
- **Extension**: `.asc` (source), `.txt` (converted)
- **Sizes**: Various tile sizes (4x4, 8x8, 16x16, 32x32 pixels)
- **Variants**: Plain (`chr*`), half-width (`*h`), bitmap (`*b`)

### Sprites (sprites/)
- **Format**: ASCII art for animated sprites
- **Extension**: `.asc` (source), `.txt` (converted)
- **Sizes**: Various sprite sizes (8x8, 12x12, 16x16, 32x32, 64x64 pixels)
- **Content**: Player character, ghosts, fruit, animations

### Sounds (sounds/)
- **Format**: MIDI files (`.mid`), XM music (`.xm`)
- **Content**: Game sound effects (pellet eating, ghost sounds, level complete, etc.)

### Graphics (graphics/)
- **Format**: Various source formats (GIMP XCF, GIF, PNG, SVG)
- **Purpose**: Original artwork files for icons, logos, and marketing materials
- **Note**: Not loaded by the game - for development/branding only

## Creating Custom Content

### Custom Mazes

1. Create plain ASCII maze using standard characters (`#` for walls, `.` for dots, etc.)
2. Convert to Unicode box-drawing format:
   ```bash
   python3 scripts/asc2txt.py --help  # See options
   python3 scripts/asc2txt.py < my_maze.asc > mazes/my_maze.txt
   ```
3. Test in game: `./build/bigman` (game will auto-detect new mazes)

### Custom Tiles

1. Create ASCII art tiles at desired resolution
2. Optionally compress using bitmap scripts:
   ```bash
   # Vertical compression (2 rows → 1)
   python3 scripts/bitmap2.py < my_tiles.txt > tiles/my_tiles_compact.txt
   
   # Horizontal compression (2 columns → 1)
   python3 scripts/bitmap4.py < my_tiles.txt > tiles/my_tiles_wide.txt
   ```

### Custom Sprites

Follow same process as tiles. Sprites represent player/ghost graphics at various animation frames and sizes.

## Naming Conventions

- **Tiles**: `chr{size}[variant].{asc|txt}`
  - Example: `chr16.txt` (16x16 tiles), `chr8h.txt` (8x8 half-width)
  
- **Sprites**: `spr{size}[variant].{asc|txt}`
  - Example: `spr12.txt` (12x12 sprites), `spr16h.txt` (16x16 half-width)
  
- **Mazes**: `{variant_name}.{asc|txt}`
  - Example: `pacman.txt`, `mspacman.txt`, `custom.txt`

## Examples

Look at existing files for format examples:
- **Simple maze**: `mazes/tiny.txt`
- **Standard tiles**: `tiles/chr8.txt`
- **Standard sprites**: `sprites/spr12.txt`

## Notes

- All maze layouts are in the public domain
- Original game code and framework are BSD 2-Clause licensed
- ASCII art files use UTF-8 encoding with Unicode box-drawing characters
- The game loads these files at runtime from hardcoded paths (`chr/`, `lvl/`, `spr/`)
- CMake build creates symlinks in `build/` directory for development
