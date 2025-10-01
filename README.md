# Glomph Maze

A revived and renamed fork of [MyMan](http://myman.sourceforge.net/), an unofficial text-based clone of the classic Pac-Man game. Designed for terminal and console environments, with no GUI support. Integrated into a collection of minimal text-based projects.

## Description

Glomph Maze is a fast-paced, curses-based game where you navigate mazes, collect dots, and avoid ghosts. It supports multiple platforms including modern Unix-like systems, DOS, and VMS, emphasizing portability and legacy compatibility.

Original project last updated in 2009; this fork adds modern maintenance while preserving the text-based ethos.

### History
- **1998**: Original MyMan by Benjamin Sittler—public domain, basic ncurses Pac-Man clone (742 lines, large tiles, maze/color support).
- **1998-2003**: Expanded portability (cygwin/DOS/VMS), small tiles, variants (e.g., pacmanic). Data files public domain.
- **Nov 2003 (v0.4/0.5)**: Switched to BSD 2-Clause license for attribution.
- **2003-2009**: Added backends (PDCurses/SDL/GTK/Allegro/libcaca/EFI/Mac Carbon), sizes/variants (quackman/small/square), UX (pager/help/snapshots/MIDI). Last update: 2009 (DOS fixes). v0.7.0 final.
- **2025 Fork (Glomph Maze)**: Renamed/revived; Makefile/UX tweaks (license in help, no startup prompt); focus on text portability. Data/original code under BSD; mazes/tiles public domain.

## Features

- Text-mode rendering using ncurses or alternatives (PDCurses, SDL, etc.).
- Modular data files for mazes, tiles, and sprites (customizable variants).
- Color and attribute support (toggleable).
- Legacy platform support (DOS via CONIO, VMS curses, EFI).
- MIDI sound (optional; future updates may explore SDL_mixer or alternatives).
- Command-line options for mazes, sizes, ghosts, etc. (e.g., `glomph-maze -m pac` for Pac-Man layout).

## Installation

### Prerequisites
- ANSI C compiler (e.g., GCC).
- Curses library (e.g., ncurses on Unix, PDCurses on Windows).
- GNU Make (version 3.80+).
- Optional: SDL2 for enhanced backends.

### Build from Source
1. Clone the repo:
   ```
   git clone git@github.com:michaelborck-dev/glomph-maze.git
   cd glomph-maze
   ```
2. Configure (optional):
   ```
   ./configure --help  # See options, e.g., --with-ncurses
   ```
3. Build:
   ```
   make
   ```
4. Install (as root, optional):
   ```
   make install  # Defaults to /usr/local
   ```

For binary distributions or other platforms, see the [INSTALL](INSTALL) file.

## Usage

Run the game:
```
glomph-maze  # Or 'myman' if not renamed in binary
```

Controls (case-insensitive):
- Arrow keys / HJKL: Move (left/down/up/right).
- Q: Quit.
- P/ESC: Pause.
- C: Toggle color.
- ?: Help.

Full options: `glomph-maze -h`.

Environment variables for fine-tuning rendering (see original README notes).

## Building and Renaming Note

This fork renames binaries/docs from \"MyMan\" to \"Glomph Maze\". Update `Makefile` if needed (e.g., change MYMAN to GLOMPHMAZE).

## Future Plans

- Update curses support to modern ncurses.
- Explore MIDI alternatives without breaking legacy.
- Refactoring for clarity while maintaining portability.
- No GUI development; focus on text environments.

## License

Modified from original BSD license. See [LICENSE](LICENSE) for details.

Original author: Benjamin C. Wiley Sittler <bsittler@gmail.com>. Fork maintainer: Michael Borck.

## Acknowledgements

This project is a fork of the original MyMan game, originally developed by Benjamin C. Wiley Sittler.

Original Sources:
- Original author's homepage: [https://xent.com/~bsittler/geocities/#myman](https://xent.com/~bsittler/geocities/#myman)
- SourceForge project: [https://sourceforge.net/projects/myman/](https://sourceforge.net/projects/myman/)
- GitHub mirror (CVS to Git conversion): [https://github.com/kragen/myman](https://github.com/kragen/myman)

Based on MyMan (public domain/BSD). Inspired by Pac-Man (Namco). Thanks to contributors for ports and variants.