"""Tests for asset loaders."""

from pathlib import Path

import pytest

from glomph.loaders import MazeLoader, SpriteLoader, TileLoader


@pytest.fixture
def assets_dir():
    """Get assets directory path."""
    return Path(__file__).parent.parent / "assets"


def test_maze_loader_lists_mazes(assets_dir):
    """Test that maze loader can list available mazes."""
    loader = MazeLoader(assets_dir)
    mazes = loader.list_mazes()
    assert len(mazes) > 0
    assert "pac" in mazes or "maze" in mazes


def test_maze_loader_loads_maze(assets_dir):
    """Test that maze loader can load a maze file."""
    loader = MazeLoader(assets_dir)
    mazes = loader.list_mazes()
    if mazes:
        maze = loader.load(mazes[0])
        assert maze.width > 0
        assert maze.height > 0
        assert len(maze.content) > 0


def test_tile_loader_loads_tiles(assets_dir):
    """Test that tile loader can load tile files."""
    loader = TileLoader(assets_dir)
    tiles_dir = assets_dir / "tiles"
    if tiles_dir.exists():
        tile_files = list(tiles_dir.glob("*.txt")) + list(tiles_dir.glob("*.asc"))
        if tile_files:
            tile_name = tile_files[0].stem
            tiles = loader.load(tile_name)
            assert tiles.width > 0
            assert tiles.height > 0


def test_sprite_loader_loads_sprites(assets_dir):
    """Test that sprite loader can load sprite files."""
    loader = SpriteLoader(assets_dir)
    sprites_dir = assets_dir / "sprites"
    if sprites_dir.exists():
        sprite_files = list(sprites_dir.glob("*.txt")) + list(sprites_dir.glob("*.asc"))
        if sprite_files:
            sprite_name = sprite_files[0].stem
            sprites = loader.load(sprite_name)
            assert sprites.width > 0
            assert sprites.height > 0
