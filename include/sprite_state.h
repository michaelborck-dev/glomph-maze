/*
 * sprite_state.h - Sprite and ghost state management
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
 * @file sprite_state.h
 * @brief Sprite and ghost AI state management
 *
 * Encapsulates all sprite-related state including:
 * - Sprite data (dimensions, flags, pixel data)
 * - Sprite registers (positions, animation frames)
 * - Ghost AI state (directions, memory, timers)
 * - Hero movement (direction, position)
 * - Collision detection
 *
 * @note Part of Phase 3 modularization - extracted from globals.h
 */

#ifndef SPRITE_STATE_H
#define SPRITE_STATE_H

#include <stddef.h>
#include <stdint.h>

#ifndef MAXGHOSTS
#define MAXGHOSTS 16
#endif

#ifndef SPRITE_REGISTERS
#define SPRITE_REGISTERS 57
#endif

extern int         sprite_w;
extern int         sprite_h;
extern int         sprite_flags;
extern const char* sprite_args;
extern const char* sprite[256];
extern int         sprite_used[256];
extern int         sprite_color[256];

extern const char* sprite_ABOUT_prefix;
extern const char* sprite_FIXME_prefix;
extern const char* sprite_NOTE_prefix;
extern const char* sprite_ABOUT;
extern const char* sprite_FIXME;
extern const char* sprite_NOTE;

extern int parse_sprite_args(const char* spritefile, const char* sprite_args);

extern int      ghost_dir[MAXGHOSTS];
extern int      ghost_mem[MAXGHOSTS];
extern int      ghost_man[MAXGHOSTS];
extern int      ghost_timer[MAXGHOSTS];
extern uint8_t* home_dir;

extern uint8_t sprite_register[SPRITE_REGISTERS];
extern int     sprite_register_frame[SPRITE_REGISTERS];
extern int     sprite_register_x[SPRITE_REGISTERS];
extern int     sprite_register_y[SPRITE_REGISTERS];
extern int     sprite_register_used[SPRITE_REGISTERS];
extern int     sprite_register_timer[SPRITE_REGISTERS];
extern int     sprite_register_color[SPRITE_REGISTERS];

extern void mark_sprite_register(int s);

extern uint8_t reflect_sprite[256];
extern uint8_t cp437_sprite[256];

extern int dirhero;
extern int hero_dir;

extern int check_collision(int eyes, int mean, int blue);
extern int find_home_dir(int s, int r, int c);

extern int ghosts_p;

extern long*   maze_GHOSTS;
extern size_t  maze_GHOSTS_len;
extern double* maze_RGHOST;
extern size_t  maze_RGHOST_len;
extern double* maze_CGHOST;
extern size_t  maze_CGHOST_len;
extern double* maze_ROGHOST;
extern size_t  maze_ROGHOST_len;
extern double* maze_COGHOST;
extern size_t  maze_COGHOST_len;
extern double* maze_RFRUIT;
extern size_t  maze_RFRUIT_len;
extern double* maze_CFRUIT;
extern size_t  maze_CFRUIT_len;
extern double* maze_RTOP;
extern size_t  maze_RTOP_len;
extern double* maze_RHERO;
extern size_t  maze_RHERO_len;
extern double* maze_CHERO;
extern size_t  maze_CHERO_len;

#endif /* SPRITE_STATE_H */
