"""Game entities (player, ghosts, etc.)."""

from enum import Enum
from typing import NamedTuple


class Position(NamedTuple):
    """2D position coordinates."""
    x: int
    y: int


class Velocity(NamedTuple):
    """Movement velocity."""
    dx: int
    dy: int


class Entity:
    """Base class for game entities."""

    def __init__(self, position: Position, char: str = "?", color: int = 0) -> None:
        """Initialize entity."""
        self.position = position
        self.char = char
        self.color = color
        self.velocity = Velocity(0, 0)

    def move(self, dx: int = 0, dy: int = 0) -> None:
        """Move entity by delta."""
        self.position = Position(
            self.position.x + dx,
            self.position.y + dy
        )

    def set_velocity(self, dx: int, dy: int) -> None:
        """Set movement velocity."""
        self.velocity = Velocity(dx, dy)

    def update(self) -> None:
        """Update entity state."""
        self.move(self.velocity.dx, self.velocity.dy)


class Player(Entity):
    """Player entity (Pac-Man)."""

    def __init__(self, position: Position) -> None:
        """Initialize player."""
        super().__init__(position, "@", 2)  # Red @ symbol
        self.lives = 3
        self.score = 0
        self.direction = "left"  # Current facing direction

    def set_direction(self, direction: str) -> None:
        """Set player direction and update velocity."""
        self.direction = direction
        if direction == "up":
            self.set_velocity(0, -1)
        elif direction == "down":
            self.set_velocity(0, 1)
        elif direction == "left":
            self.set_velocity(-1, 0)
        elif direction == "right":
            self.set_velocity(1, 0)
        else:
            self.set_velocity(0, 0)

    def lose_life(self) -> None:
        """Player loses a life."""
        self.lives -= 1

    def add_score(self, points: int) -> None:
        """Add points to score."""
        self.score += points


class GhostMode(Enum):
    """Ghost behavior modes."""
    SCATTER = "scatter"
    CHASE = "chase"
    FRIGHTENED = "frightened"


class Ghost(Entity):
    """Ghost entity."""

    def __init__(self, position: Position, name: str, color: int) -> None:
        """Initialize ghost."""
        super().__init__(position, "M", color)  # M for ghost
        self.name = name
        self.mode = GhostMode.SCATTER
        self.target = position  # Position to move towards
        self.home_corner = position  # Scatter mode target corner

    def set_mode(self, mode: GhostMode) -> None:
        """Set ghost behavior mode."""
        self.mode = mode
        if mode == GhostMode.FRIGHTENED:
            self.char = "Ø"  # Different appearance when frightened
            self.color = 5  # Magenta when frightened
        else:
            self.char = "M"
            # Reset to normal color (would be per-ghost)

    def set_target(self, target: Position) -> None:
        """Set movement target position."""
        self.target = target

    def update_ai(self, player_pos: Position) -> None:
        """Update ghost AI based on current mode."""
        if self.mode == GhostMode.SCATTER:
            self.set_target(self.home_corner)
        elif self.mode == GhostMode.CHASE:
            # Different chase behaviors per ghost (simplified for now)
            self.set_target(player_pos)
        elif self.mode == GhostMode.FRIGHTENED:
            # Random movement when frightened (simplified)
            pass

    def calculate_direction(self) -> str:
        """Calculate best direction to move towards target."""
        # Simple pathfinding: move towards target
        dx = self.target.x - self.position.x
        dy = self.target.y - self.position.y

        # Prefer larger axis difference
        if abs(dx) > abs(dy):
            return "right" if dx > 0 else "left"
        else:
            return "down" if dy > 0 else "up"


def create_ghosts() -> list[Ghost]:
    """Create the four classic ghosts."""
    return [
        Ghost(Position(14, 11), "Blinky", 2),  # Red, targets player directly
        Ghost(Position(14, 13), "Pinky", 5),   # Pink, ambushes ahead of player
        Ghost(Position(12, 13), "Inky", 4),    # Cyan, uses complex targeting
        Ghost(Position(16, 13), "Clyde", 3),   # Orange, random when close
    ]
