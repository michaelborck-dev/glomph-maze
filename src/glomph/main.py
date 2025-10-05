"""Main entry point for Glomph Maze game."""

import sys
from pathlib import Path

from glomph.game import Game
from glomph.loaders import MazeLoader
from glomph.terminal import get_terminal


def main() -> int:
    """Run the game."""
    terminal = get_terminal()

    # Get assets directory
    assets_dir = Path(__file__).parent.parent.parent / "assets"

    # Load a sample maze
    loader = MazeLoader(assets_dir)
    try:
        mazes = loader.list_mazes()
        if not mazes:
            print("No mazes found!")
            return 1

        maze = loader.load(mazes[0])
        print(f"Loaded maze: {mazes[0]} ({maze.width}x{maze.height})")

        # Initialize game
        game = Game(maze.width, maze.height)

        # Demo game loop
        with terminal.screen():
            while True:
                terminal.clear()

                # Draw game state
                terminal.draw_text(0, 0, f"Glomph Maze - Level {game.level}")
                terminal.draw_text(1, 0, f"Score: {game.player.score}  Lives: {game.player.lives}")
                terminal.draw_text(2, 0, f"Position: ({game.player.position.x}, {game.player.position.y})")
                terminal.draw_text(3, 0, f"Direction: {game.player.direction}")
                terminal.draw_text(5, 0, "Arrow keys: move  P: pause  ESC: quit")

                # Draw player
                terminal.draw_char(7, 10, game.player.char, game.player.color)

                # Draw ghosts
                ghost_positions = [(8, 8), (8, 12), (10, 8), (10, 12)]
                for i, ghost in enumerate(game.ghosts):
                    if i < len(ghost_positions):
                        y, x = ghost_positions[i]
                        terminal.draw_char(y, x, ghost.char, ghost.color)

                terminal.refresh()

                # Handle input
                key = terminal.wait_for_keypress()
                if key == "escape":
                    break
                elif key == "p":
                    game.pause_game()
                elif key in ["up", "down", "left", "right"]:
                    game.move_player(key)
                    game.update()

    except Exception as e:
        print(f"Error: {e}")
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
