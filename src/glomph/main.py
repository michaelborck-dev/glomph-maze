"""Main entry point for Glomph Maze game."""

import sys
from pathlib import Path

from glomph.game import Game
from glomph.loaders import MazeLoader
from glomph.renderer import get_renderer
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

        # Initialize game and renderer
        game = Game(maze.width, maze.height)
        renderer = get_renderer()

        # Demo game loop
        with terminal.screen():
            height, width = terminal.get_screen_size()
            renderer.set_viewport(width, height)

            while True:
                renderer.clear()

                # Render maze
                renderer.render_maze(maze)

                # Render entities
                renderer.render_entity(game.player)
                renderer.render_entities(game.ghosts)

                # Render UI
                game_state = {
                    'level': game.level,
                    'score': game.player.score,
                    'lives': game.player.lives
                }
                renderer.render_ui(game_state)

                # Draw instructions
                terminal.draw_text(height - 2, 0, "Arrow keys: move  P: pause  ESC: quit", 7)

                renderer.present()

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
