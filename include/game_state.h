/*
 * game_state.h - Game state management (score, lives, level)
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
 * @file game_state.h
 * @brief Game state management
 *
 * Encapsulates all game state variables including:
 * - Score tracking (score, points, earned)
 * - Player lives (lives, lives_used)
 * - Level progression (level, intermission)
 * - Game phases (dying, dead, winning)
 * - Player state (player, oldplayer)
 * - Collectibles (dots, pellets, total_dots)
 *
 * @note Part of Phase 3 modularization - extracted from globals.h
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>

extern int level;
extern int intermission;
extern int intermission_shown;
extern int cycles;
extern int score;
extern int dots;
extern int points;
extern int lives;
extern int lives_used;
extern int earned;
extern int dying;
extern int dead;
extern int deadpan;
extern int myman_lines;
extern int myman_columns;
extern int oldplayer;
extern int player;

extern bool nogame;

extern long pellet_timer;
extern long pellet_time;

extern long          myman_intro;
extern unsigned long myman_start;
extern unsigned long myman_demo;
extern int           munched;

extern int  old_score;
extern int  old_showlives;
extern int  old_level;
extern long winning;

extern int ghost_eaten_timer;

extern bool paused;

extern long          intermission_running;
extern unsigned long myman_demo_setup;

extern int need_reset;

extern int* total_dots;
extern int* pellets;

extern long flip_to;

extern int bonus_score[8];

extern void gameintro(void);
extern void gamedemo(void);
extern void gamestart(void);
extern void gameintermission(void);
extern void gamehelp(void);
extern void gameinfo(void);
extern int  gamelogic(void);
extern void gamesfx(void);
extern void gamereset(void);
extern void gamerender(void);
extern int  gameinput(void);
extern int  gamecycle(int lines, int cols);

extern void creditscreen(void);

#endif /* GAME_STATE_H */
