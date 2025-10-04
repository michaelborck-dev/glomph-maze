/*
 * maze_state.h - Maze data and state management
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
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/**
 * @file maze_state.h
 * @brief Maze data and state management
 *
 * Encapsulates all maze-related state including:
 * - Maze data arrays (maze, blank_maze, color data)
 * - Maze dimensions (width, height, levels)
 * - Maze metadata (flags, args)
 * - Maze loading and parsing
 * - Wall and path information
 *
 * @note Part of Phase 3 modularization - extracted from globals.h
 */

#ifndef MAZE_STATE_H
#define MAZE_STATE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern char*    maze;
extern char*    maze_color;
extern char*    blank_maze;
extern char*    blank_maze_color;
extern uint8_t* dirty_cell;
extern bool     all_dirty;

extern int         maze_n;
extern int         maze_w;
extern int         maze_h;
extern int         maze_flags;
extern int         maze_level;
extern const char* maze_args;

extern const char* maze_ABOUT_prefix;
extern const char* maze_FIXME_prefix;
extern const char* maze_NOTE_prefix;
extern const char* maze_ABOUT;
extern const char* maze_FIXME;
extern const char* maze_NOTE;

extern int readmaze(const char* mazefile, int* levels, int* w, int* h,
                    char** maze, int* flags, char** color, const char** args);

extern void writemaze(const char* mazefile);
extern int  parse_maze_args(const char* mazefile, const char* maze_args);

extern void maze_erase(void);
extern void mark_cell(int x, int y);
extern void maze_puts(int y, int x, int color, const char* s);
extern void maze_putsn_nonblank(int y, int x, int color, const char* s, int n);

extern void paint_walls(int verbose);

extern uint16_t* inside_wall;

extern long maze_visual(int n, int i, int j);

extern const char* maze_WALL_COLORS;
extern size_t      maze_WALL_COLORS_len;
extern const char* maze_DOT_COLORS;
extern size_t      maze_DOT_COLORS_len;
extern const char* maze_PELLET_COLORS;
extern size_t      maze_PELLET_COLORS_len;
extern const char* maze_MORTAR_COLORS;
extern size_t      maze_MORTAR_COLORS_len;

extern long*  maze_RMSG;
extern size_t maze_RMSG_len;
extern long*  maze_CMSG;
extern size_t maze_CMSG_len;
extern long*  maze_RMSG2;
extern size_t maze_RMSG2_len;
extern long*  maze_CMSG2;
extern size_t maze_CMSG2_len;

extern long scroll_offset_x0;
extern long scroll_offset_y0;
extern int  msglen;

#endif /* MAZE_STATE_H */
