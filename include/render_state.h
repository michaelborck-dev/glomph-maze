/*
 * render_state.h - Rendering and display state management
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
 * @file render_state.h
 * @brief Rendering and display state management
 *
 * Encapsulates all rendering-related state including:
 * - Tile data (dimensions, flags, pixel data)
 * - Screen dimensions and layout
 * - Color and palette management
 * - Display buffers and pager state
 * - Frame tracking and performance
 * - Graphics reflection and transformation
 *
 * @note Part of Phase 3 modularization - extracted from globals.h
 */

#ifndef RENDER_STATE_H
#define RENDER_STATE_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifndef NPENS
#define NPENS 256
#endif

extern int         tile_w;
extern int         tile_h;
extern int         tile_flags;
extern const char* tile_args;
extern const char* tile[256];
extern int         tile_used[256];
extern int         tile_color[256];

extern const char* tile_ABOUT_prefix;
extern const char* tile_FIXME_prefix;
extern const char* tile_NOTE_prefix;
extern const char* tile_ABOUT;
extern const char* tile_FIXME;
extern const char* tile_NOTE;

extern int readfont(const char* fontfile, int* w, int* h, const char** font,
                    int* used, int* flags, int* color, const char** args);

extern void writefont(const char* file, const char* prefix, int w, int h,
                      const char** font, int* used, int flags, int* color,
                      const char* args);

extern int parse_tile_args(const char* tilefile, const char* tile_args);

extern uint8_t gfx2(uint8_t c);
extern size_t  gfx1(const char** font, unsigned char c, int y, int x, int w);
extern uint8_t gfx0(uint8_t c, uint8_t* m);

extern int reflect;
extern int gfx_reflect;

extern FILE*       snapshot;
extern FILE*       snapshot_txt;
extern int         xoff_received;
extern double      td;
extern const char* pager_notice;
extern const char* pager_remaining;
extern int         pager_arrow_magic;
extern int         reinit_requested;

extern int           old_lines;
extern int           old_cols;
extern int           ignore_delay;
extern long          frameskip;
extern long          frameskip0;
extern long          frameskip1;
extern long          scrolling;
extern long          frames;
extern unsigned long mymandelay;
extern unsigned long mindelay;

extern int use_underline;
extern int use_color;

extern int  my_clear(void);
extern void my_clearok(int ok);

extern int showlives;
extern int visible_frame;

extern int debug;

extern unsigned long myman_sfx;

extern char* tmp_notice;

extern const char* msg_READY;
extern const char* msg_GAMEOVER;
extern const char* msg_PLAYER1;
extern const char* msg_PLAYER2;

#endif /* RENDER_STATE_H */
