"""Game state and logic for Glomph Maze."""

from enum import Enum

from glomph.entities import Player, Position, create_ghosts
from glomph.loaders import MazeData


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

        # Maze collision data
        self.maze_data: MazeData | None = None
        self.collision_map: list[list[str]] = []
        self.dot_map: list[list[str]] = []

    def load_maze(self, maze_data: MazeData) -> None:
        """Load maze data and build collision maps."""
        self.maze_data = maze_data

        # Parse maze content into 2D arrays
        lines = maze_data.content.split('\n')
        self.collision_map = []
        self.dot_map = []
        self.dots_remaining = 0

        for line in lines:
            if not line.strip():
                continue

            collision_row = []
            dot_row = []

            for char in line:
                if char == '#':
                    # Wall - collision
                    collision_row.append('#')
                    dot_row.append(' ')
                elif char == '.':
                    # Dot - no collision, collectible
                    collision_row.append(' ')
                    dot_row.append('.')
                    self.dots_remaining += 1
                elif char == 'o':
                    # Power pellet - no collision, collectible
                    collision_row.append(' ')
                    dot_row.append('o')
                    self.dots_remaining += 1
                else:
                    # Empty space or other characters
                    collision_row.append(' ')
                    dot_row.append(' ')

            self.collision_map.append(collision_row)
            self.dot_map.append(dot_row)

        # Ensure all rows have the same width
        max_width = max(len(row) for row in self.collision_map) if self.collision_map else 0
        for row in self.collision_map:
            while len(row) < max_width:
                row.append(' ')
        for row in self.dot_map:
            while len(row) < max_width:
                row.append(' ')

    def is_collision(self, x: int, y: int) -> bool:
        """Check if position (x, y) has a collision."""
        if not self.collision_map or y < 0 or y >= len(self.collision_map):
            return True  # Out of bounds = collision

        row = self.collision_map[y]
        if x < 0 or x >= len(row):
            return True  # Out of bounds = collision

        return row[x] == '#'

    def collect_dot(self, x: int, y: int) -> bool:
        """Collect dot/power pellet at position (x, y). Returns True if collected."""
        if not self.dot_map or y < 0 or y >= len(self.dot_map):
            return False

        row = self.dot_map[y]
        if x < 0 or x >= len(row):
            return False

        if row[x] in ('.', 'o'):
            # Collect the dot
            row[x] = ' '  # Remove from map
            self.dots_remaining -= 1

            # Award points
            points = 10 if row[x] == '.' else 50  # Power pellet worth more
            self.player.add_score(points)

            return True

        return False

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

        # Check collision at new position
        if not self.is_collision(x, y):
            self.player.position = Position(x, y)

            # Check for dot collection
            self.collect_dot(x, y)

            # Check win condition
            if self.dots_remaining == 0:
                self.state = GameState.VICTORY

    def update(self) -> None:
        """Update game state (called each frame)."""
        if self.state != GameState.PLAYING:
            return

        # Update ghosts
        self.update_ghosts()

        # Check ghost-player collisions
        self.check_ghost_collisions()

    def update_ghosts(self) -> None:
        """Update all ghost positions and AI."""
        for ghost in self.ghosts:
            # Update ghost AI based on mode
            ghost.update_ai(self.player.position)

            # Calculate movement based on target
            direction = ghost.calculate_direction()

            # Calculate new position
            x, y = ghost.position.x, ghost.position.y
            if direction == "up":
                y -= 1
            elif direction == "down":
                y += 1
            elif direction == "left":
                x -= 1
            elif direction == "right":
                x += 1

            # Check collision at new position
            if not self.is_collision(x, y):
                ghost.position = Position(x, y)

    def check_ghost_collisions(self) -> None:
        """Check for collisions between player and ghosts."""
        for ghost in self.ghosts:
            if (self.player.position.x == ghost.position.x and
                self.player.position.y == ghost.position.y):
                # Player hit a ghost
                self.lose_life()
                break

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
