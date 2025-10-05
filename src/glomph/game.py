"""Game state and logic for Glomph Maze."""

from enum import Enum

from glomph.entities import Player, Position, create_ghosts


class GameState(Enum):
    """Current state of the game."""
    MENU = "menu"
    PLAYING = "playing"
    PAUSED = "paused"
    GAME_OVER = "game_over"
    VICTORY = "victory"


class Game:
    """Main game state manager."""

    def __init__(self, maze_width: int = 28, maze_height: int = 31) -> None:
        """Initialize game state."""
        self.state = GameState.MENU
        self.level = 1
        self.maze_width = maze_width
        self.maze_height = maze_height

        # Initialize player at center-bottom of maze
        start_x = maze_width // 2
        start_y = maze_height - 2
        self.player = Player(Position(start_x, start_y))

        self.dots_remaining = 0
        self.ghosts = create_ghosts()

    def start_game(self) -> None:
        """Start a new game."""
        self.state = GameState.PLAYING
        self.level = 1
        self.player.lives = 3
        self.player.score = 0

    def pause_game(self) -> None:
        """Pause/unpause the game."""
        if self.state == GameState.PLAYING:
            self.state = GameState.PAUSED
        elif self.state == GameState.PAUSED:
            self.state = GameState.PLAYING

    def game_over(self) -> None:
        """End the game."""
        self.state = GameState.GAME_OVER

    def move_player(self, direction: str) -> None:
        """Move player in specified direction."""
        if self.state != GameState.PLAYING:
            return

        # Update player direction and velocity
        self.player.set_direction(direction)

        # Calculate new position
        x, y = self.player.position.x, self.player.position.y
        if direction == "up":
            y -= 1
        elif direction == "down":
            y += 1
        elif direction == "left":
            x -= 1
        elif direction == "right":
            x += 1

        # Keep player within bounds (basic collision detection)
        x = max(0, min(x, self.maze_width - 1))
        y = max(0, min(y, self.maze_height - 1))

        self.player.position = Position(x, y)

    def update(self) -> None:
        """Update game state (called each frame)."""
        if self.state != GameState.PLAYING:
            return

        # TODO: Update ghosts
        # TODO: Check collisions
        # TODO: Check win condition
        pass

    def is_game_won(self) -> bool:
        """Check if the current level is won."""
        return self.dots_remaining == 0

    def next_level(self) -> None:
        """Advance to the next level."""
        self.level += 1
        # Reset player position
        start_x = self.maze_width // 2
        start_y = self.maze_height - 2
        self.player.position = Position(start_x, start_y)
        # TODO: Load new maze
        # TODO: Reset dots
        # TODO: Reset ghosts

    def lose_life(self) -> None:
        """Player loses a life."""
        self.player.lose_life()

        if self.player.lives <= 0:
            self.game_over()
        else:
            # Reset player position
            start_x = self.maze_width // 2
            start_y = self.maze_height - 2
            self.player.position = Position(start_x, start_y)
