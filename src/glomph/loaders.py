"""Asset loaders for mazes, sprites, and tiles."""

from pathlib import Path
from typing import NamedTuple


class MazeData(NamedTuple):
    """Maze file data."""

    width: int
    height: int
    flags: int
    content: str
    metadata: dict[str, str]


class TileData(NamedTuple):
    """Tile file data."""

    width: int
    height: int
    flags: int
    tiles: dict[str, list[str]]
    metadata: dict[str, str]


class SpriteData(NamedTuple):
    """Sprite file data."""

    width: int
    height: int
    flags: int
    frames: dict[str, list[str]]
    metadata: dict[str, str]


class MazeLoader:
    """Load maze files from assets/mazes/."""

    def __init__(self, assets_dir: Path) -> None:
        """Initialize maze loader."""
        self.mazes_dir = assets_dir / "mazes"

    def load(self, name: str) -> MazeData:
        """Load a maze file by name."""
        path = self.mazes_dir / f"{name}.txt"
        if not path.exists():
            path = self.mazes_dir / f"{name}.asc"

        if not path.exists():
            raise FileNotFoundError(f"Maze not found: {name}")

        with open(path, encoding="utf-8") as f:
            lines = f.readlines()

        # Parse first line: "N WxH" or "N WxH~F arg1=val1 ..."
        header = lines[0].strip().split()
        dimensions = header[1].split("x")
        width = int(dimensions[0])

        # Height and flags
        height_and_flags = dimensions[1].split("~")
        height = int(height_and_flags[0])
        flags = int(height_and_flags[1]) if len(height_and_flags) > 1 else 0

        # Parse metadata arguments
        metadata: dict[str, str] = {}
        if len(header) > 2:
            for arg in header[2:]:
                if "=" in arg:
                    key, value = arg.split("=", 1)
                    metadata[key] = value

        # Content is everything after first line
        content = "".join(lines[1:])

        return MazeData(width, height, flags, content, metadata)

    def list_mazes(self) -> list[str]:
        """List all available maze names."""
        mazes = []
        for path in self.mazes_dir.glob("*.txt"):
            mazes.append(path.stem)
        for path in self.mazes_dir.glob("*.asc"):
            if path.stem not in mazes:
                mazes.append(path.stem)
        return sorted(mazes)


class TileLoader:
    """Load tile files from assets/tiles/."""

    def __init__(self, assets_dir: Path) -> None:
        """Initialize tile loader."""
        self.tiles_dir = assets_dir / "tiles"

    def load(self, name: str) -> TileData:
        """Load a tile file by name."""
        path = self.tiles_dir / f"{name}.txt"
        if not path.exists():
            path = self.tiles_dir / f"{name}.asc"

        if not path.exists():
            raise FileNotFoundError(f"Tile file not found: {name}")

        with open(path, encoding="utf-8-sig") as f:
            content = f.read()

        # Parse header: "WxH" or "WxH~F arg1=val1 ..."
        lines = content.split("\n")
        header = lines[0].strip().split()
        dimensions = header[0].split("x")
        width = int(dimensions[0])

        height_and_flags = dimensions[1].split("~")
        height = int(height_and_flags[0])
        flags = int(height_and_flags[1]) if len(height_and_flags) > 1 else 0

        # Parse metadata
        metadata: dict[str, str] = {}
        if len(header) > 1:
            for arg in header[1:]:
                if "=" in arg:
                    key, value = arg.split("=", 1)
                    metadata[key] = value

        # Parse tiles
        tiles: dict[str, list[str]] = {}
        i = 1
        while i < len(lines):
            line = lines[i].strip()
            if not line:
                i += 1
                continue

            # Tile header: "XX~Y" or "XX"
            tile_code = line.split("~")[0]

            # Read tile lines
            tile_lines = []
            for j in range(height):
                if i + 1 + j < len(lines):
                    tile_line = lines[i + 1 + j]
                    if tile_line.startswith(":"):
                        tile_lines.append(tile_line[1 : width + 1])

            tiles[tile_code] = tile_lines
            i += height + 1

        return TileData(width, height, flags, tiles, metadata)


class SpriteLoader:
    """Load sprite files from assets/sprites/."""

    def __init__(self, assets_dir: Path) -> None:
        """Initialize sprite loader."""
        self.sprites_dir = assets_dir / "sprites"

    def load(self, name: str) -> SpriteData:
        """Load a sprite file by name."""
        path = self.sprites_dir / f"{name}.txt"
        if not path.exists():
            path = self.sprites_dir / f"{name}.asc"

        if not path.exists():
            raise FileNotFoundError(f"Sprite file not found: {name}")

        with open(path, encoding="utf-8-sig") as f:
            content = f.read()

        # Parse similar to tiles
        lines = content.split("\n")
        header = lines[0].strip().split()
        dimensions = header[0].split("x")
        width = int(dimensions[0])

        height_and_flags = dimensions[1].split("~")
        height = int(height_and_flags[0])
        flags = int(height_and_flags[1]) if len(height_and_flags) > 1 else 0

        metadata: dict[str, str] = {}
        if len(header) > 1:
            for arg in header[1:]:
                if "=" in arg:
                    key, value = arg.split("=", 1)
                    metadata[key] = value

        # Parse sprite frames (same format as tiles)
        frames: dict[str, list[str]] = {}
        i = 1
        while i < len(lines):
            line = lines[i].strip()
            if not line:
                i += 1
                continue

            frame_code = line.split("~")[0]
            frame_lines = []
            for j in range(height):
                if i + 1 + j < len(lines):
                    frame_line = lines[i + 1 + j]
                    if frame_line.startswith(":"):
                        frame_lines.append(frame_line[1 : width + 1])

            frames[frame_code] = frame_lines
            i += height + 1

        return SpriteData(width, height, flags, frames, metadata)
