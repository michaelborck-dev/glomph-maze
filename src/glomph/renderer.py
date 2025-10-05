"""Rendering engine for Glomph Maze."""

from glomph.entities import Entity
from typing import Sequence
from glomph.loaders import MazeData
from glomph.terminal import get_terminal


class Renderer:
    """Handles rendering of the game world."""

    def __init__(self) -> None:
        """Initialize the renderer."""
        self.terminal = get_terminal()
        self.viewport_width = 0
        self.viewport_height = 0
        self.camera_x = 0
        self.camera_y = 0

    def set_viewport(self, width: int, height: int) -> None:
        """Set the viewport dimensions."""
        self.viewport_width = width
        self.viewport_height = height

    def set_camera(self, x: int, y: int) -> None:
        """Set camera position for scrolling."""
        self.camera_x = x
        self.camera_y = y

    def world_to_screen(self, world_x: int, world_y: int) -> tuple[int, int]:
        """Convert world coordinates to screen coordinates."""
        screen_x = world_x - self.camera_x
        screen_y = world_y - self.camera_y
        return screen_x, screen_y

    def is_visible(self, world_x: int, world_y: int) -> bool:
        """Check if a world position is visible in the current viewport."""
        screen_x, screen_y = self.world_to_screen(world_x, world_y)
        return (0 <= screen_x < self.viewport_width and
                0 <= screen_y < self.viewport_height)

    def render_maze(self, maze: MazeData) -> None:
        """Render a maze to the screen."""
        # For now, render the maze content as plain text
        # TODO: Use tile system for proper graphics
        lines = maze.content.split('\n')

        for y, line in enumerate(lines):
            if y >= self.viewport_height:
                break

            for x, char in enumerate(line):
                if x >= self.viewport_width:
                    break

                # Skip empty characters
                if char == ' ':
                    continue

                # Map maze characters to colors
                color = 0  # Default
                if char == '#':  # Wall
                    color = 4  # Blue
                elif char in ('.', 'o'):  # Dot or Power pellet
                    color = 3  # Yellow

                self.terminal.draw_char(y, x, char, color)

    def render_entity(self, entity: Entity) -> None:
        """Render a game entity."""
        if not self.is_visible(entity.position.x, entity.position.y):
            return

        screen_x, screen_y = self.world_to_screen(entity.position.x, entity.position.y)
        self.terminal.draw_char(screen_y, screen_x, entity.char, entity.color)

    def render_entities(self, entities: Sequence[Entity]) -> None:
        """Render multiple entities."""
        for entity in entities:
            self.render_entity(entity)

    def render_ui(self, game_state: dict[str, int]) -> None:
        """Render UI elements (score, lives, etc.)."""
        # Render at the top of the screen
        ui_y = 0

        # Clear UI area
        for x in range(self.viewport_width):
            self.terminal.draw_char(ui_y, x, ' ')

        # Render game info
        level = game_state.get('level', 1)
        score = game_state.get('score', 0)
        lives = game_state.get('lives', 3)

        ui_text = f"Level: {level}  Score: {score}  Lives: {lives}"
        self.terminal.draw_text(ui_y, 0, ui_text, 7)  # White text

    def clear(self) -> None:
        """Clear the screen."""
        self.terminal.clear()

    def present(self) -> None:
        """Present the rendered frame."""
        self.terminal.refresh()


# Global renderer instance
_renderer = Renderer()


def get_renderer() -> Renderer:
    """Get the global renderer instance."""
    return _renderer
