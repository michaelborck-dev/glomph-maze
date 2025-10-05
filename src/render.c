/* render.c - Maze rendering and dirty cell tracking
 * Copyright 1997-2009, Benjamin C. Wiley Sittler <bsittler@gmail.com>
 * Copyright 2025, Michael Borck <michael@borck.dev>
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy,
 *  modify, merge, publish, distribute, sublicense, and/or sell copies
 *  of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 */

#include <string.h>

#include "globals.h"
#include "utils.h"

/**
 * @brief Mark maze cell as needing redraw
 *
 * Sets dirty bit for specified cell in dirty_cell bitmap. Cell will be
 * redrawn on next render pass. Bounds-checked to prevent invalid access.
 *
 * @param x Cell X coordinate (tile units)
 * @param y Cell Y coordinate (tile units)
 *
 * @note No-op if all_dirty is true (full redraw already pending)
 * @note No-op if coordinates are out of bounds
 * @see maze_erase, DIRTY_ALL, IS_CELL_DIRTY
 */
void mark_cell(int x, int y) {
    if ((!all_dirty) && ((((long)(x)) >= 0) && (((long)(y)) >= 0) &&
                         ((x) <= maze_w) && ((y) < maze_h))) {
        int idx;

        idx = ((y)) * ((maze_w + 1 + 7) >> 3) + ((x) >> 3);
        dirty_cell[idx] =
            (1U << ((x) & 7)) | (unsigned)(unsigned char)dirty_cell[idx];
    }
}

/**
 * @brief Clear current maze level
 *
 * Erases all maze data and color data for the current level (maze_level).
 * Marks all cells as dirty to trigger complete screen redraw.
 *
 * @note Modifies global maze and maze_color arrays
 * @note Sets all_dirty flag via DIRTY_ALL macro
 * @see mark_cell, DIRTY_ALL
 */
void maze_erase(void) {
    memset((void*)(maze + maze_level * maze_h * (maze_w + 1)), 0,
           (maze_w + 1) * maze_h);
    memset((void*)(maze_color + maze_level * maze_h * (maze_w + 1)), 0,
           (maze_w + 1) * maze_h);
    DIRTY_ALL();
}

/**
 * @brief Write string to maze at specified position
 *
 * Writes null-terminated string to maze grid with given color. Each character
 * overwrites maze cell and marks it dirty. Automatically wraps X coordinates.
 * Clips to maze bounds (ignores out-of-bounds characters).
 *
 * Used for displaying messages like "READY!", "GAME OVER", scores, etc.
 *
 * @param y Row position (tile coordinates)
 * @param x Starting column position (tile coordinates)
 * @param color Color code for all characters in string
 * @param s Null-terminated string to write
 *
 * @note Modifies global maze and maze_color arrays
 * @note Marks affected cells dirty via mark_cell
 * @see maze_putsn_nonblank, mark_cell, XWRAP, YWRAP
 */
void maze_puts(int y, int x, int color, const char* s) {
    int           i;
    unsigned char c;

    if (y < 0)
        return;
    if (y >= maze_h)
        return;
    for (i = 0; 0 != (int)(unsigned char)(c = (unsigned)(unsigned char)(s)[i]);
         i++) {
        if (((x + i) >= 0) && ((x + i) < maze_w)) {
            maze[(maze_level * maze_h + YWRAP(y)) * (maze_w + 1) +
                 XWRAP(x + i)]       = c;
            maze_color[(maze_level * maze_h + YWRAP(y)) * (maze_w + 1) +
                       XWRAP(x + i)] = (char)(unsigned char)color;
            mark_cell(XWRAP(x + i), YWRAP(y));
        }
    }
}

/**
 * @brief Write string to maze, preserving blank spaces
 *
 * Similar to maze_puts but with special handling for spaces:
 * - Non-space characters: Write to maze with given color
 * - Space characters: Copy from blank_maze (preserves original maze design)
 *
 * Useful for overlaying text while keeping background intact. Limited to
 * writing up to n characters.
 *
 * @param y Row position (tile coordinates)
 * @param x Starting column position (tile coordinates)
 * @param color Color code for non-blank characters
 * @param s String to write (may contain spaces)
 * @param n Maximum number of characters to write
 *
 * @note Modifies global maze and maze_color arrays
 * @note Preserves original maze design for space characters
 * @see maze_puts, blank_maze, mark_cell
 */
void maze_putsn_nonblank(int y, int x, int color, const char* s, int n) {
    int           i;
    unsigned char c;

    if (y < 0)
        return;
    if (y >= maze_h)
        return;
    for (i = 0;
         (i < (n)) &&
         (0 != (int)(unsigned char)(c = (unsigned)(unsigned char)(s)[i]));
         i++) {
        int cc;

        cc = color;
        if (((x + i) >= 0) && ((x + i) < maze_w)) {
            if (' ' == (char)c) {
                c = blank_maze[(maze_level * maze_h + YWRAP(y)) * (maze_w + 1) +
                               XWRAP(x + i)];
                cc = blank_maze_color[(maze_level * maze_h + YWRAP(y)) *
                                          (maze_w + 1) +
                                      XWRAP(x + i)];
            }
            maze[(maze_level * maze_h + YWRAP(y)) * (maze_w + 1) +
                 XWRAP(x + i)]       = c;
            maze_color[(maze_level * maze_h + YWRAP(y)) * (maze_w + 1) +
                       XWRAP(x + i)] = (char)(unsigned char)cc;
            mark_cell(XWRAP(x + i), YWRAP(y));
        }
    }
}

/**
 * @brief Mark maze cells occupied by sprite as needing redraw
 *
 * Calculates which maze cells are covered by a sprite and marks them all
 * dirty. Accounts for sprite size (may be larger than tile size) and centers
 * sprite over its position.
 *
 * @param s Sprite register index (0-56, see SPRITE_REGISTERS)
 *
 * @note Marks rectangular region based on gfx_w/gfx_h and sgfx_w/sgfx_h
 * @note Called when sprite moves or changes
 * @see mark_cell, sprite_register_x, sprite_register_y
 */
void mark_sprite_register(int s) {
    int _dirty_sprite_register_x, _dirty_sprite_register_y;

    for (_dirty_sprite_register_y = 0;
         _dirty_sprite_register_y < ((gfx_h > sgfx_h) ? gfx_h : sgfx_h);
         _dirty_sprite_register_y++)
        for (_dirty_sprite_register_x = 0;
             _dirty_sprite_register_x < ((gfx_w > sgfx_w) ? gfx_w : sgfx_w);
             _dirty_sprite_register_x++) {
            mark_cell(XTILE(sprite_register_x[s] + _dirty_sprite_register_x -
                            ((gfx_w > sgfx_w) ? gfx_w : sgfx_w) / 2),
                      YTILE(sprite_register_y[s] + _dirty_sprite_register_y -
                            ((gfx_h > sgfx_h) ? gfx_h : sgfx_h) / 2));
        }
}
void paint_walls(int verbose) {
    int    n;
    double tdt, tdt2 = 0.0L;
    int    tdt_used = 0;

    memset((void*)inside_wall, '\0', sizeof(inside_wall));
    tdt      = doubletime();
    tdt_used = 0;
    for (n = 0; n < maze_n; n++) {
        int phase;

        total_dots[n] = 0;
        pellets[n]    = 0;
        for (phase = 0; phase <= 4; phase++) {
            int phase_done;

            do {
                int i;

                phase_done = 1;
                for (i = 0; i < maze_h; i++) {
                    int j;

                    if ((!nogame) && verbose) {
                        tdt2 = doubletime();
                        if ((tdt2 - tdt) >= 1.0) {
                            tdt      = tdt2;
                            tdt_used = 1;
                            fprintf(stderr, "%3d%%\r",
                                    (int)(((((float)n) * 5.0 + (float)phase) *
                                               maze_h +
                                           (float)i) *
                                              100.0 /
                                              (((float)maze_n) * 5.0 *
                                               ((float)maze_h)) +
                                          0.5));
                            fflush(stderr);
                        }
                    }
                    for (j = 0; j <= maze_w; j++) {
                        long c;

                        c = maze_visual(n, i, j);
                        switch (phase) {
                        case 0:
                            if (ISPELLET(c) || ISDOT(c)) {
                                total_dots[n]++;
                                if (ISPELLET(c)) {
                                    pellets[n]++;
                                }
                            }
                            if (ISNONINVERTABLE(c)) {
                                inside_wall[(n * maze_h + i) * (maze_w + 1) +
                                            j] =
                                    ((unsigned)inside_wall[(n * maze_h + i) *
                                                               (maze_w + 1) +
                                                           j]) |
                                    INSIDE_WALL_NON_INVERTABLE;
                            }
                            break;
                        case 1:
                            if ((!(((unsigned)inside_wall
                                        [(n * maze_h + i) * (maze_w + 1) + j]) &
                                   INSIDE_WALL_NON_INVERTABLE)) &&
                                (!((unsigned)udlr[c])) &&
                                ((((unsigned)
                                       inside_wall[(n * maze_h + YWRAP(i - 1)) *
                                                       (maze_w + 1) +
                                                   j]) &
                                  INSIDE_WALL_NON_INVERTABLE) ||
                                 (((unsigned)
                                       inside_wall[(n * maze_h + YWRAP(i + 1)) *
                                                       (maze_w + 1) +
                                                   j]) &
                                  INSIDE_WALL_NON_INVERTABLE) ||
                                 (((unsigned)inside_wall[(n * maze_h + i) *
                                                             (maze_w + 1) +
                                                         XWRAP2(j - 1)]) &
                                  INSIDE_WALL_NON_INVERTABLE) ||
                                 (((unsigned)inside_wall[(n * maze_h + i) *
                                                             (maze_w + 1) +
                                                         XWRAP2(j + 1)]) &
                                  INSIDE_WALL_NON_INVERTABLE))) {
                                inside_wall[(n * maze_h + i) * (maze_w + 1) +
                                            j] =
                                    ((unsigned)inside_wall[(n * maze_h + i) *
                                                               (maze_w + 1) +
                                                           j]) |
                                    INSIDE_WALL_NON_INVERTABLE;
                                phase_done = 0;
                            }
                            break;
                        case 2:
                        case 3:
                            if ((!(((unsigned)inside_wall
                                        [(n * maze_h + i) * (maze_w + 1) + j]) &
                                   (INSIDE_WALL_NON_INVERTABLE |
                                    INSIDE_WALL_PROVISIONAL | INSIDE_WALL_YES |
                                    INSIDE_WALL_NO |
                                    ((phase == 2) ? INSIDE_WALL_PHASE2
                                                  : INSIDE_WALL_PHASE3)))) &&
                                ((phase == 3) ||
                                 ((((((unsigned)inside_wall[(n * maze_h +
                                                             YWRAP(i - 1)) *
                                                                (maze_w + 1) +
                                                            j]) &
                                     INSIDE_WALL_NON_INVERTABLE) ^
                                    (((unsigned)inside_wall[(n * maze_h +
                                                             YWRAP(i + 1)) *
                                                                (maze_w + 1) +
                                                            j]) &
                                     INSIDE_WALL_NON_INVERTABLE)) &&
                                   ((((unsigned)udlr[c]) & 0x05) == 0x05)) ||
                                  (((((unsigned)inside_wall[(n * maze_h + i) *
                                                                (maze_w + 1) +
                                                            XWRAP2(j - 1)]) &
                                     INSIDE_WALL_NON_INVERTABLE) ^
                                    (((unsigned)inside_wall[(n * maze_h + i) *
                                                                (maze_w + 1) +
                                                            XWRAP2(j + 1)]) &
                                     INSIDE_WALL_NON_INVERTABLE)) &&
                                   ((((unsigned)udlr[c]) & 0x50) == 0x50))))) {
                                int painting_done;

                                inside_wall[(n * maze_h + i) * (maze_w + 1) +
                                            j] =
                                    ((unsigned)inside_wall[(n * maze_h + i) *
                                                               (maze_w + 1) +
                                                           j]) |
                                    INSIDE_WALL_PROVISIONAL;
                                if ((((unsigned)inside_wall[(n * maze_h +
                                                             YWRAP(i - 1)) *
                                                                (maze_w + 1) +
                                                            j]) &
                                     INSIDE_WALL_NON_INVERTABLE) &&
                                    ((((unsigned)udlr[c]) & 0x05) == 0x05)) {
                                    inside_wall[(n * maze_h + i) *
                                                    (maze_w + 1) +
                                                j] =
                                        ((unsigned)
                                             inside_wall[(n * maze_h + i) *
                                                             (maze_w + 1) +
                                                         j]) |
                                        INSIDE_WALL_NO;
                                    if (!(((unsigned)
                                               inside_wall[(n * maze_h +
                                                            YWRAP(i + 1)) *
                                                               (maze_w + 1) +
                                                           j]) &
                                          (INSIDE_WALL_NON_INVERTABLE |
                                           INSIDE_WALL_PROVISIONAL |
                                           INSIDE_WALL_YES | INSIDE_WALL_NO))) {
                                        inside_wall[(n * maze_h +
                                                     YWRAP(i + 1)) *
                                                        (maze_w + 1) +
                                                    j] =
                                            ((unsigned)
                                                 inside_wall[(n * maze_h +
                                                              YWRAP(i + 1)) *
                                                                 (maze_w + 1) +
                                                             j]) |
                                            (INSIDE_WALL_YES |
                                             INSIDE_WALL_PROVISIONAL);
                                    }
                                } else if ((((unsigned)
                                                 inside_wall[(n * maze_h + i) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j - 1)]) &
                                            INSIDE_WALL_NON_INVERTABLE) &&
                                           ((((unsigned)udlr[c]) & 0x50) ==
                                            0x50)) {
                                    inside_wall[(n * maze_h + i) *
                                                    (maze_w + 1) +
                                                j] =
                                        ((unsigned)
                                             inside_wall[(n * maze_h + i) *
                                                             (maze_w + 1) +
                                                         j]) |
                                        INSIDE_WALL_NO;
                                    if (!(((unsigned)
                                               inside_wall[(n * maze_h + i) *
                                                               (maze_w + 1) +
                                                           XWRAP2(j + 1)]) &
                                          (INSIDE_WALL_NON_INVERTABLE |
                                           INSIDE_WALL_PROVISIONAL |
                                           INSIDE_WALL_YES | INSIDE_WALL_NO))) {
                                        inside_wall[(n * maze_h + i) *
                                                        (maze_w + 1) +
                                                    XWRAP2(j + 1)] =
                                            ((unsigned)
                                                 inside_wall[(n * maze_h + i) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j + 1)]) |
                                            (INSIDE_WALL_YES |
                                             INSIDE_WALL_PROVISIONAL);
                                    }
                                } else {
                                    inside_wall[(n * maze_h + i) *
                                                    (maze_w + 1) +
                                                j] =
                                        ((unsigned)
                                             inside_wall[(n * maze_h + i) *
                                                             (maze_w + 1) +
                                                         j]) |
                                        INSIDE_WALL_YES;
                                }
                                do {
                                    int i2, j2;
                                    int undo = 0;

                                    painting_done = 1;
                                    for (i2 = 0; i2 < maze_h; i2++)
                                        for (j2 = 0; j2 <= maze_w; j2++) {
                                            long c2;

                                            c2 = maze_visual(n, i2, j2);
                                            if (!undo) {
                                                if (((!(((unsigned)udlr[c2]) &
                                                        0x04)) &&
                                                     (((unsigned)inside_wall
                                                           [(n * maze_h +
                                                             YWRAP(i2 + 1)) *
                                                                (maze_w + 1) +
                                                            j2]) &
                                                      INSIDE_WALL_YES)) ||
                                                    ((!(((unsigned)udlr[c2]) &
                                                        0x40)) &&
                                                     (((unsigned)inside_wall
                                                           [(n * maze_h + i2) *
                                                                (maze_w + 1) +
                                                            XWRAP2(j2 + 1)]) &
                                                      INSIDE_WALL_YES))) {
                                                    if (((unsigned)inside_wall
                                                             [(n * maze_h +
                                                               i2) *
                                                                  (maze_w + 1) +
                                                              j2]) &
                                                        (INSIDE_WALL_NON_INVERTABLE |
                                                         INSIDE_WALL_NO)) {
                                                        undo          = 1;
                                                        i2            = 0;
                                                        j2            = 0;
                                                        painting_done = 1;
                                                    } else {
                                                        if (!(((unsigned)inside_wall
                                                                   [(n * maze_h +
                                                                     i2) *
                                                                        (maze_w +
                                                                         1) +
                                                                    j2]) &
                                                              INSIDE_WALL_YES)) {
                                                            inside_wall[(n * maze_h +
                                                                         i2) *
                                                                            (maze_w +
                                                                             1) +
                                                                        j2] =
                                                                ((unsigned)inside_wall
                                                                     [(n * maze_h +
                                                                       i2) *
                                                                          (maze_w +
                                                                           1) +
                                                                      j2]) |
                                                                INSIDE_WALL_PROVISIONAL |
                                                                INSIDE_WALL_YES;
                                                            painting_done = 0;
                                                        }
                                                    }
                                                } else if (
                                                    ((((unsigned)udlr[c2]) &
                                                      0x04) &&
                                                     ((((unsigned)inside_wall
                                                            [(n * maze_h +
                                                              YWRAP(i2 + 1)) *
                                                                 (maze_w + 1) +
                                                             j2]) &
                                                       (INSIDE_WALL_PROVISIONAL |
                                                        INSIDE_WALL_YES)) ==
                                                      ((INSIDE_WALL_PROVISIONAL |
                                                        INSIDE_WALL_YES) ^
                                                       (((unsigned)inside_wall
                                                             [(n * maze_h +
                                                               i2) *
                                                                  (maze_w + 1) +
                                                              j2]) &
                                                        INSIDE_WALL_PROVISIONAL)))) ||
                                                    ((((unsigned)udlr[c2]) &
                                                      0x40) &&
                                                     ((((unsigned)inside_wall
                                                            [(n * maze_h + i2) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j2 + 1)]) &
                                                       (INSIDE_WALL_PROVISIONAL |
                                                        INSIDE_WALL_YES)) ==
                                                      ((INSIDE_WALL_PROVISIONAL |
                                                        INSIDE_WALL_YES) ^
                                                       (((unsigned)inside_wall
                                                             [(n * maze_h +
                                                               i2) *
                                                                  (maze_w + 1) +
                                                              j2]) &
                                                        INSIDE_WALL_PROVISIONAL))))) {
                                                    if ((((unsigned)inside_wall
                                                              [(n * maze_h +
                                                                i2) *
                                                                   (maze_w +
                                                                    1) +
                                                               j2]) &
                                                         INSIDE_WALL_YES) ==
                                                        INSIDE_WALL_YES) {
                                                        undo          = 1;
                                                        i2            = 0;
                                                        j2            = 0;
                                                        painting_done = 1;
                                                    }
                                                }
                                            }
                                            if (!undo) {
                                                if ((((unsigned)inside_wall
                                                          [(n * maze_h + i2) *
                                                               (maze_w + 1) +
                                                           j2]) &
                                                     INSIDE_WALL_YES) &&
                                                    (!(((unsigned)udlr[c2]) &
                                                       0x04))) {
                                                    if (((unsigned)inside_wall
                                                             [(n * maze_h +
                                                               YWRAP(i2 + 1)) *
                                                                  (maze_w + 1) +
                                                              j2]) &
                                                        (INSIDE_WALL_NON_INVERTABLE |
                                                         INSIDE_WALL_NO)) {
                                                        undo          = 1;
                                                        i2            = 0;
                                                        j2            = 0;
                                                        painting_done = 1;
                                                    } else {
                                                        if (!(((unsigned)inside_wall
                                                                   [(n * maze_h +
                                                                     YWRAP(i2 +
                                                                           1)) *
                                                                        (maze_w +
                                                                         1) +
                                                                    j2]) &
                                                              INSIDE_WALL_YES)) {
                                                            inside_wall[(n * maze_h +
                                                                         YWRAP(
                                                                             i2 +
                                                                             1)) *
                                                                            (maze_w +
                                                                             1) +
                                                                        j2] =
                                                                ((unsigned)inside_wall
                                                                     [(n * maze_h +
                                                                       YWRAP(
                                                                           i2 +
                                                                           1)) *
                                                                          (maze_w +
                                                                           1) +
                                                                      j2]) |
                                                                INSIDE_WALL_PROVISIONAL |
                                                                INSIDE_WALL_YES;
                                                            painting_done = 0;
                                                        }
                                                    }
                                                }
                                            }
                                            if (!undo) {
                                                if ((((unsigned)inside_wall
                                                          [(n * maze_h + i2) *
                                                               (maze_w + 1) +
                                                           j2]) &
                                                     INSIDE_WALL_YES) &&
                                                    (!(((unsigned)udlr[c2]) &
                                                       0x40))) {
                                                    if (((unsigned)inside_wall
                                                             [(n * maze_h +
                                                               i2) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j2 + 1)]) &
                                                        (INSIDE_WALL_NON_INVERTABLE |
                                                         INSIDE_WALL_NO)) {
                                                        undo          = 1;
                                                        i2            = 0;
                                                        j2            = 0;
                                                        painting_done = 1;
                                                    } else {
                                                        if (!(((unsigned)inside_wall
                                                                   [(n * maze_h +
                                                                     i2) *
                                                                        (maze_w +
                                                                         1) +
                                                                    XWRAP2(
                                                                        j2 +
                                                                        1)]) &
                                                              INSIDE_WALL_YES)) {
                                                            inside_wall[(n * maze_h +
                                                                         i2) *
                                                                            (maze_w +
                                                                             1) +
                                                                        XWRAP2(
                                                                            j2 +
                                                                            1)] =
                                                                ((unsigned)inside_wall
                                                                     [(n * maze_h +
                                                                       i2) *
                                                                          (maze_w +
                                                                           1) +
                                                                      XWRAP2(
                                                                          j2 +
                                                                          1)]) |
                                                                INSIDE_WALL_PROVISIONAL |
                                                                INSIDE_WALL_YES;
                                                            painting_done = 0;
                                                        }
                                                    }
                                                }
                                            }
                                            if (undo) {
                                                if (((unsigned)inside_wall
                                                         [(n * maze_h + i2) *
                                                              (maze_w + 1) +
                                                          j2]) &
                                                    INSIDE_WALL_PROVISIONAL) {
                                                    inside_wall[(n * maze_h +
                                                                 i2) *
                                                                    (maze_w +
                                                                     1) +
                                                                j2] =
                                                        ((unsigned)inside_wall
                                                             [(n * maze_h +
                                                               i2) *
                                                                  (maze_w + 1) +
                                                              j2]) &
                                                        ~(INSIDE_WALL_PROVISIONAL |
                                                          INSIDE_WALL_YES |
                                                          INSIDE_WALL_NO);
                                                }
                                            }
                                        }
                                } while (!painting_done);
                                {
                                    int i2, j2;

                                    for (i2 = 0; i2 < maze_h; i2++)
                                        for (j2 = 0; j2 <= maze_w; j2++) {
                                            if (((unsigned)inside_wall
                                                     [(n * maze_h + i2) *
                                                          (maze_w + 1) +
                                                      j2]) &
                                                INSIDE_WALL_PROVISIONAL) {
                                                inside_wall[(n * maze_h + i2) *
                                                                (maze_w + 1) +
                                                            j2] =
                                                    ((unsigned)inside_wall
                                                         [(n * maze_h + i2) *
                                                              (maze_w + 1) +
                                                          j2]) &
                                                    ~INSIDE_WALL_PROVISIONAL;
                                            }
                                        }
                                }
                                phase_done = 0;
                                inside_wall[(n * maze_h + i) * (maze_w + 1) +
                                            j] =
                                    ((unsigned)inside_wall[(n * maze_h + i) *
                                                               (maze_w + 1) +
                                                           j]) |
                                    ((phase == 2) ? INSIDE_WALL_PHASE2
                                                  : INSIDE_WALL_PHASE3);
                            }
                            break;
                        case 4:
                            if (!(((unsigned)inside_wall
                                       [(n * maze_h + i) * (maze_w + 1) + j]) &
                                  INSIDE_WALL_NON_INVERTABLE)) {
                                int ul, ll, ur, lr;

                                ul =
                                    !!(((unsigned)inside_wall[(n * maze_h + i) *
                                                                  (maze_w + 1) +
                                                              j]) &
                                       INSIDE_WALL_YES);
                                ll =
                                    !!(((unsigned)inside_wall[(n * maze_h +
                                                               YWRAP(i + 1)) *
                                                                  (maze_w + 1) +
                                                              j]) &
                                       INSIDE_WALL_YES);
                                ur =
                                    !!(((unsigned)inside_wall[(n * maze_h + i) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j + 1)]) &
                                       INSIDE_WALL_YES);
                                lr =
                                    !!(((unsigned)inside_wall[(n * maze_h +
                                                               YWRAP(i + 1)) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j + 1)]) &
                                       INSIDE_WALL_YES);
                                if ((ul + ll + ur + lr) == 0) {
                                    if ((((unsigned)udlr[c]) & 0x05) == 0x05) {
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h +
                                                              YWRAP(i - 1)) *
                                                                 (maze_w + 1) +
                                                             j]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ll = 1;
                                            lr = 1;
                                        }
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h +
                                                              YWRAP(i + 1)) *
                                                                 (maze_w + 1) +
                                                             j]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ul = 1;
                                            ur = 1;
                                        }
                                    }
                                    if ((((unsigned)udlr[c]) & 0x50) == 0x50) {
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h + i) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j + 1)]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ul = 1;
                                            ll = 1;
                                        }
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h + i) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j - 1)]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ur = 1;
                                            lr = 1;
                                        }
                                    }
                                    if ((((unsigned)udlr[c]) & 0x55) == 0x44) {
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h +
                                                              YWRAP(i - 1)) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j - 1)]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ur = 1;
                                            ll = 1;
                                            lr = 1;
                                        }
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h +
                                                              YWRAP(i + 1)) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j + 1)]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ul = 1;
                                        }
                                    }
                                    if ((((unsigned)udlr[c]) & 0x55) == 0x41) {
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h +
                                                              YWRAP(i - 1)) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j + 1)]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ul = 1;
                                            ll = 1;
                                            lr = 1;
                                        }
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h +
                                                              YWRAP(i + 1)) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j - 1)]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ur = 1;
                                        }
                                    }
                                    if ((((unsigned)udlr[c]) & 0x55) == 0x14) {
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h +
                                                              YWRAP(i + 1)) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j - 1)]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ul = 1;
                                            ur = 1;
                                            lr = 1;
                                        }
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h +
                                                              YWRAP(i - 1)) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j + 1)]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ll = 1;
                                        }
                                    }
                                    if ((((unsigned)udlr[c]) & 0x55) == 0x11) {
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h +
                                                              YWRAP(i + 1)) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j + 1)]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ul = 1;
                                            ur = 1;
                                            ll = 1;
                                        }
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h +
                                                              YWRAP(i - 1)) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j - 1)]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            lr = 1;
                                        }
                                    }
                                    if ((((unsigned)udlr[c]) & 0x55) == 0x15) {
                                        if ((((unsigned)
                                                  inside_wall[(n * maze_h +
                                                               YWRAP(i + 1)) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j - 1)]) &
                                             INSIDE_WALL_NON_INVERTABLE) ||
                                            (((unsigned)
                                                  inside_wall[(n * maze_h +
                                                               YWRAP(i + 1)) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j + 1)]) &
                                             INSIDE_WALL_NON_INVERTABLE)) {
                                            ul = 1;
                                            ur = 1;
                                        }
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h +
                                                              YWRAP(i - 1)) *
                                                                 (maze_w + 1) +
                                                             j]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ll = 1;
                                            lr = 1;
                                        }
                                    }
                                    if ((((unsigned)udlr[c]) & 0x55) == 0x45) {
                                        if ((((unsigned)
                                                  inside_wall[(n * maze_h +
                                                               YWRAP(i - 1)) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j - 1)]) &
                                             INSIDE_WALL_NON_INVERTABLE) ||
                                            (((unsigned)
                                                  inside_wall[(n * maze_h +
                                                               YWRAP(i - 1)) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j + 1)]) &
                                             INSIDE_WALL_NON_INVERTABLE)) {
                                            ll = 1;
                                            lr = 1;
                                        }
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h +
                                                              YWRAP(i + 1)) *
                                                                 (maze_w + 1) +
                                                             j]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ul = 1;
                                            ur = 1;
                                        }
                                    }
                                    if ((((unsigned)udlr[c]) & 0x55) == 0x51) {
                                        if ((((unsigned)
                                                  inside_wall[(n * maze_h +
                                                               YWRAP(i - 1)) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j + 1)]) &
                                             INSIDE_WALL_NON_INVERTABLE) ||
                                            (((unsigned)
                                                  inside_wall[(n * maze_h +
                                                               YWRAP(i + 1)) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j + 1)]) &
                                             INSIDE_WALL_NON_INVERTABLE)) {
                                            ul = 1;
                                            ll = 1;
                                        }
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h + i) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j - 1)]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ur = 1;
                                            lr = 1;
                                        }
                                    }
                                    if ((((unsigned)udlr[c]) & 0x55) == 0x54) {
                                        if ((((unsigned)
                                                  inside_wall[(n * maze_h +
                                                               YWRAP(i - 1)) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j - 1)]) &
                                             INSIDE_WALL_NON_INVERTABLE) ||
                                            (((unsigned)
                                                  inside_wall[(n * maze_h +
                                                               YWRAP(i + 1)) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j - 1)]) &
                                             INSIDE_WALL_NON_INVERTABLE)) {
                                            ur = 1;
                                            lr = 1;
                                        }
                                        if (((unsigned)
                                                 inside_wall[(n * maze_h + i) *
                                                                 (maze_w + 1) +
                                                             XWRAP2(j + 1)]) &
                                            INSIDE_WALL_NON_INVERTABLE) {
                                            ul = 1;
                                            ll = 1;
                                        }
                                    }
                                    if ((((unsigned)udlr[c]) & 0x55) == 0x55) {
                                        if ((((unsigned)
                                                  inside_wall[(n * maze_h +
                                                               YWRAP(i - 1)) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j - 1)]) &
                                             INSIDE_WALL_NON_INVERTABLE) ||
                                            (((unsigned)
                                                  inside_wall[(n * maze_h +
                                                               YWRAP(i + 1)) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j + 1)]) &
                                             INSIDE_WALL_NON_INVERTABLE)) {
                                            ur = 1;
                                            ll = 1;
                                        }
                                        if ((((unsigned)
                                                  inside_wall[(n * maze_h +
                                                               YWRAP(i - 1)) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j + 1)]) &
                                             INSIDE_WALL_NON_INVERTABLE) ||
                                            (((unsigned)
                                                  inside_wall[(n * maze_h +
                                                               YWRAP(i + 1)) *
                                                                  (maze_w + 1) +
                                                              XWRAP2(j - 1)]) &
                                             INSIDE_WALL_NON_INVERTABLE)) {
                                            ul = 1;
                                            lr = 1;
                                        }
                                    }
                                    if ((ul + ur + ll + lr) == 4) {
                                        ul = 0;
                                        ur = 0;
                                        ll = 0;
                                        lr = 0;
                                        inside_wall[(n * maze_h + i) *
                                                        (maze_w + 1) +
                                                    j] =
                                            ((unsigned)
                                                 inside_wall[(n * maze_h + i) *
                                                                 (maze_w + 1) +
                                                             j]) |
                                            INSIDE_WALL_FULLY_NON_INVERTED;
                                    }
                                }
                                if (((ul + ll + ur + lr) > 2) ||
                                    (((ul + ll + ur + lr) == 2) && ul)) {
                                    inside_wall[(n * maze_h + i) *
                                                    (maze_w + 1) +
                                                j] =
                                        ((unsigned)
                                             inside_wall[(n * maze_h + i) *
                                                             (maze_w + 1) +
                                                         j]) |
                                        INSIDE_WALL_INVERTED;
                                }
                                if (((ul + ll + ur + lr) == 4) &&
                                    (((!!(((unsigned)udlr[c]) & 0x40)) +
                                      (!!(((unsigned)udlr[c]) & 0x10)) +
                                      (!!(((unsigned)udlr[c]) & 0x04)) +
                                      (!!(((unsigned)udlr[c]) & 0x01))) > 1)) {
                                    inside_wall[(n * maze_h + i) *
                                                    (maze_w + 1) +
                                                j] =
                                        ((unsigned)
                                             inside_wall[(n * maze_h + i) *
                                                             (maze_w + 1) +
                                                         j]) |
                                        INSIDE_WALL_FULLY_INVERTED;
                                }
                            }
                            break;
                        }
                    }
                }
            } while (!phase_done);
        }
    }
    if (tdt_used) {
        fprintf(stderr, "    \r");
        fflush(stderr);
        tdt_used = 0;
    }
}
