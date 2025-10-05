"""Terminal abstraction layer using curses."""

import curses
from collections.abc import Generator
from contextlib import contextmanager


class Terminal:
    """Curses-based terminal abstraction for the game."""

    def __init__(self) -> None:
        """Initialize terminal interface."""
        self.stdscr: curses.window | None = None
        self.colors_initialized = False

    def init_colors(self) -> None:
        """Initialize color pairs if terminal supports colors."""
        if not curses.has_colors():
            return

        curses.start_color()
        curses.use_default_colors()

        # Define common color pairs (foreground, background)
        curses.init_pair(1, curses.COLOR_BLACK, -1)      # Black on default
        curses.init_pair(2, curses.COLOR_RED, -1)        # Red on default
        curses.init_pair(3, curses.COLOR_GREEN, -1)      # Green on default
        curses.init_pair(4, curses.COLOR_YELLOW, -1)     # Yellow on default
        curses.init_pair(5, curses.COLOR_BLUE, -1)       # Blue on default
        curses.init_pair(6, curses.COLOR_MAGENTA, -1)    # Magenta on default
        curses.init_pair(7, curses.COLOR_CYAN, -1)       # Cyan on default
        curses.init_pair(8, curses.COLOR_WHITE, -1)      # White on default

        self.colors_initialized = True

    @contextmanager
    def screen(self) -> Generator[curses.window, None, None]:
        """Context manager for curses screen initialization."""
        try:
            self.stdscr = curses.initscr()
            curses.noecho()           # Don't echo keys
            curses.cbreak()           # React to keys instantly
            self.stdscr.keypad(True) # Enable special keys
            curses.curs_set(0)       # Hide cursor

            self.init_colors()

            yield self.stdscr

        finally:
            if self.stdscr:
                self.stdscr.keypad(False)
            curses.echo()
            curses.nocbreak()
            curses.endwin()

    def get_screen_size(self) -> tuple[int, int]:
        """Get terminal screen size (height, width)."""
        if not self.stdscr:
            return (24, 80)  # Default fallback
        height, width = self.stdscr.getmaxyx()
        return height, width

    def draw_char(self, y: int, x: int, char: str, color_pair: int = 0) -> None:
        """Draw a character at position (y, x) with optional color."""
        if not self.stdscr:
            return

        try:
            if self.colors_initialized and color_pair > 0:
                self.stdscr.addch(y, x, char, curses.color_pair(color_pair))
            else:
                self.stdscr.addch(y, x, char)
        except curses.error:
            # Ignore drawing errors (out of bounds, etc.)
            pass

    def draw_text(self, y: int, x: int, text: str, color_pair: int = 0) -> None:
        """Draw text at position (y, x) with optional color."""
        if not self.stdscr:
            return

        try:
            if self.colors_initialized and color_pair > 0:
                self.stdscr.addstr(y, x, text, curses.color_pair(color_pair))
            else:
                self.stdscr.addstr(y, x, text)
        except curses.error:
            # Ignore drawing errors
            pass

    def clear(self) -> None:
        """Clear the screen."""
        if self.stdscr:
            self.stdscr.clear()

    def refresh(self) -> None:
        """Refresh the screen to show changes."""
        if self.stdscr:
            self.stdscr.refresh()

    def get_input(self, timeout_ms: int | None = None) -> int | None:
        """Get input character, optionally with timeout."""
        if not self.stdscr:
            return None

        if timeout_ms is not None:
            self.stdscr.timeout(timeout_ms)
        else:
            self.stdscr.timeout(-1)  # Blocking

        try:
            return self.stdscr.getch()
        except (curses.error, KeyboardInterrupt):
            return None

    def wait_for_key(self) -> int:
        """Wait for and return a key press."""
        while True:
            key = self.get_input()
            if key is not None:
                return key

    @staticmethod
    def key_name(key: int) -> str:
        """Get the name of a key code."""
        return curses.keyname(key).decode('utf-8')

    def get_key(self) -> str | None:
        """Get a key press and return a normalized key name."""
        key = self.get_input()
        if key is None:
            return None

        # Handle special keys
        if key == curses.KEY_UP:
            return "up"
        elif key == curses.KEY_DOWN:
            return "down"
        elif key == curses.KEY_LEFT:
            return "left"
        elif key == curses.KEY_RIGHT:
            return "right"
        elif key == curses.KEY_ENTER or key == 10 or key == 13:
            return "enter"
        elif key == 27:  # ESC
            return "escape"
        elif key == 32:  # Space
            return "space"
        elif key == curses.KEY_BACKSPACE or key == 127:
            return "backspace"
        elif key >= 32 and key <= 126:  # Printable ASCII
            return chr(key)
        else:
            return f"key_{key}"

    def wait_for_keypress(self) -> str:
        """Wait for a key press and return normalized key name."""
        while True:
            key = self.get_key()
            if key is not None:
                return key


# Global terminal instance
_terminal = Terminal()


def get_terminal() -> Terminal:
    """Get the global terminal instance."""
    return _terminal
