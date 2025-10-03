/*
 * globals.h - Global state declarations for Glomph Maze
 * 
 * This header consolidates global variables that are shared across modules
 * during the Phase 2 refactoring. While globals are not ideal design, this
 * approach allows incremental extraction of modules from the monolithic
 * myman.c without requiring a complete rewrite.
 * 
 * TODO Phase 4: Replace with context structs for better encapsulation
 * 
 * Copyright 1997-2009, Benjamin C. Wiley Sittler <bsittler@gmail.com>
 * Copyright 2025, Michael Borck <michael@borck.dev>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <curses.h>
#include <stdio.h>

/* Maximum number of color pairs/pens */
#ifndef NPENS
#define NPENS 256
#endif

/* Character text attribute mask (curses compatibility) */
#ifndef MY_A_CHARTEXT
#ifdef A_CHARTEXT
#define MY_A_CHARTEXT A_CHARTEXT
#endif
#endif

#ifndef MY_A_CHARTEXT
#ifdef MY_A_ATTRIBUTES
#define MY_A_CHARTEXT ~MY_A_ATTRIBUTES
#else
#define MY_A_CHARTEXT 0xFF
#endif
#endif

/*
 * Character Set Arrays
 * Used by: render/screen.c for character rendering and snapshots
 */

/* Note: Type varies based on USE_WIDEC_SUPPORT */
extern chtype altcharset_cp437[256];
extern chtype ascii_cp437[256];

/*
 * Color and Rendering Globals
 * Used by: render/screen.c, render/pager.c, main rendering loop
 */

/* Color palette - maps attributes to curses color pairs */
extern chtype pen[NPENS];

/* RGB color values for palette (used for HTML snapshots and color init) */
extern const short pen_pal[16][3];

/* Rendering mode flags */
extern int use_acs;        /* Use alternate character set (line drawing) */
extern int use_raw;        /* Use raw 8-bit mode */
extern int use_raw_ucs;    /* Use raw Unicode mode */

/*
 * HTML/Text Snapshot System
 * Used by: render/screen.c for capturing game output
 */

/* Snapshot output files */
extern FILE *snapshot;      /* HTML output */
extern FILE *snapshot_txt;  /* Plain text output */

/* Snapshot cursor tracking */
extern int snapshot_x;      /* Current column */
extern int snapshot_y;      /* Current row */

/* Snapshot attribute tracking */
extern chtype snapshot_attrs;        /* Current attributes to apply */
extern chtype snapshot_attrs_active; /* Currently active attributes */
extern int snapshot_use_color;       /* Whether to use color in snapshots */

/*
 * Screen State Tracking
 * Used by: render/screen.c for cursor position management
 */

/* Cursor position tracking (for working around curses bugs) */
extern int location_is_suspect;  /* Cursor position may be wrong */
extern int last_valid_line;      /* Last known valid row */
extern int last_valid_col;       /* Last known valid column */

#endif /* GLOBALS_H */
