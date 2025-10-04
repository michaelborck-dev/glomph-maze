/*
 * globals.h - Global game state consolidation
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
 * @file globals.h
 * @brief Global game state - single source of truth
 * 
 * This header consolidates all global variables from utils.h.
 * During refactoring, modules will gradually replace these globals
 * with proper encapsulation.
 * 
 * @note This is a TRANSITION file - will be replaced by module-specific
 *       state in future phases of the refactoring plan
 * 
 * Total global variables: 210+
 * Organized into sections:
 * - String constants and prefixes
 * - Unicode/character encoding tables
 * - Function declarations (parsing, I/O)
 * - Maze data and state
 * - Tile and sprite data
 * - Ghost AI state
 * - Sprite registers
 * - Game state (score, lives, level)
 * - Input and timing
 * - Display and rendering
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <getopt.h>

/*============================================================================
 * CONSTANTS - Will move to constants.h in Phase 3
 *===========================================================================*/

#ifndef MAXGHOSTS
#define MAXGHOSTS 16
#endif

#ifndef SPRITE_REGISTERS
#define SPRITE_REGISTERS 57
#endif

#ifndef LIVES
#define LIVES 3
#endif

#ifndef NPENS
#define NPENS 256
#endif

/*============================================================================
 * COMPILE-TIME ASSERTIONS (C17)
 *===========================================================================*/

/* Verify critical constants at compile time to catch configuration errors early */

_Static_assert(MAXGHOSTS == 16, 
               "MAXGHOSTS must be 16 - ghost arrays hardcoded to this size");

_Static_assert(SPRITE_REGISTERS == 57, 
               "SPRITE_REGISTERS must be 57 - calculated from MAXGHOSTS and sprite layout");

_Static_assert(LIVES >= 0 && LIVES <= 99, 
               "LIVES must be between 0 and 99 - display assumes two digits");

_Static_assert(NPENS == 256, 
               "NPENS must be 256 - color palette requires full 8-bit range");

_Static_assert(sizeof(int) >= 4, 
               "Need at least 32-bit integers for score and coordinate calculations");

_Static_assert(sizeof(void*) >= 4, 
               "Need at least 32-bit pointers for maze data structures");

/*============================================================================
 * STRING CONSTANTS AND PREFIXES
 *===========================================================================*/

extern const char *maze_ABOUT_prefix;
extern const char *maze_FIXME_prefix;
extern const char *maze_NOTE_prefix;
extern const char *tile_ABOUT_prefix;
extern const char *tile_FIXME_prefix;
extern const char *tile_NOTE_prefix;
extern const char *sprite_ABOUT_prefix;
extern const char *sprite_FIXME_prefix;
extern const char *sprite_NOTE_prefix;
extern const char *MYMANKEYS_prefix;
extern const char *MOREMESSAGE;
extern const char *DONEMESSAGE;
extern const char *MYMANLEGALNOTICE;
extern const char *MYMANKEYS;
extern const char *short_options;
extern struct option *long_options;

/*============================================================================
 * PROGRAM STATE
 *===========================================================================*/

extern const char *progname;

/*============================================================================
 * UNICODE/CP437 CHARACTER ENCODING TABLES
 *===========================================================================*/

extern const unsigned long uni_cp437_halfwidth[256];
extern const unsigned long uni_cp437_fullwidth[256];
extern const unsigned char cp437_fullwidth_rhs[256];
extern unsigned char reflect_sprite[256];
extern unsigned char cp437_sprite[256];
extern const unsigned char udlr[256];
extern unsigned char fallback_cp437[256];
extern unsigned char reflect_cp437[256];
extern const unsigned long *uni_cp437;

/*============================================================================
 * FUNCTION DECLARATIONS - Parsing and I/O
 *===========================================================================*/

extern short mille_to_scale(short n, short scale);
extern int fgetc_cp437_utf8(FILE *stream);
extern int fputc_utf8(unsigned long u, FILE *stream);
extern int fputc_utf8_cp437(int c, FILE *stream);
extern int ungetc_cp437_utf8(int c, FILE *stream);
extern char *strword(const char *from, const char **endp, size_t *lenp);
extern long *strtollist(const char *from, const char **endp, size_t *lenp);
extern double *strtodlist(const char *from, const char **endp, size_t *lenp);
extern long *strtollist_word(const char *from, const char **endp, size_t *lenp);
extern double *strtodlist_word(const char *from, const char **endp, size_t *lenp);
extern void mymanescape(const char *s, int len);
extern char *mystrdup(const char *s);

/*============================================================================
 * FONT/TILE/SPRITE LOADING
 *===========================================================================*/

extern int readfont(const char *fontfile,
                    int *w,
                    int *h,
                    const char **font,
                    int *used,
                    int *flags,
                    int *color,
                    const char **args);

extern void writefont(const char *file,
                      const char *prefix,
                      int w, int h,
                      const char **font,
                      int *used,
                      int flags,
                      int *color,
                      const char *args);

extern int parse_tile_args(const char *tilefile, const char *tile_args);
extern int parse_sprite_args(const char *spritefile, const char *sprite_args);

/*============================================================================
 * MAZE LOADING
 *===========================================================================*/

extern int readmaze(const char *mazefile,
                    int *levels,
                    int *w,
                    int *h,
                    char **maze,
                    int *flags,
                    char **color,
                    const char **args);

extern void writemaze(const char *mazefile);
extern int parse_maze_args(const char *mazefile, const char *maze_args);

/*============================================================================
 * MAZE DATA AND STATE
 *===========================================================================*/

extern char *maze;
extern char *maze_color;
extern char *blank_maze;
extern char *blank_maze_color;
extern unsigned char *dirty_cell;
extern int all_dirty;

extern int nogame;

extern int maze_n;
extern int maze_w;
extern int maze_h;
extern int maze_flags;
extern int maze_level;
extern const char *maze_args;

/*============================================================================
 * TILE DATA
 *===========================================================================*/

extern int tile_w;
extern int tile_h;
extern int tile_flags;
extern const char *tile_args;
extern const char *tile[256];
extern int tile_used[256];
extern int tile_color[256];

/*============================================================================
 * SPRITE DATA
 *===========================================================================*/

extern int sprite_w;
extern int sprite_h;
extern int sprite_flags;
extern const char *sprite_args;
extern const char *sprite[256];
extern int sprite_used[256];
extern int sprite_color[256];

/*============================================================================
 * MAZE RENDERING
 *===========================================================================*/

extern void maze_erase(void);
extern void mark_cell(int x, int y);
extern void maze_puts(int y, int x, int color, const char *s);
extern void maze_putsn_nonblank(int y, int x, int color, const char *s, int n);

/*============================================================================
 * GHOST AI STATE
 *===========================================================================*/

extern int ghost_dir[MAXGHOSTS];
extern int ghost_mem[MAXGHOSTS];
extern int ghost_man[MAXGHOSTS];
extern int ghost_timer[MAXGHOSTS];
extern unsigned char *home_dir;

/*============================================================================
 * SPRITE REGISTERS
 *===========================================================================*/

extern unsigned char sprite_register[SPRITE_REGISTERS];
extern int sprite_register_frame[SPRITE_REGISTERS];
extern int sprite_register_x[SPRITE_REGISTERS];
extern int sprite_register_y[SPRITE_REGISTERS];
extern int sprite_register_used[SPRITE_REGISTERS];
extern int sprite_register_timer[SPRITE_REGISTERS];
extern int sprite_register_color[SPRITE_REGISTERS];

extern void mark_sprite_register(int s);

/*============================================================================
 * GRAPHICS AND RENDERING
 *===========================================================================*/

extern unsigned char gfx2(unsigned char c);
extern size_t gfx1(const char **font, unsigned char c, int y, int x, int w);
extern unsigned char gfx0(unsigned char c, unsigned char *m);

extern int reflect;
extern int gfx_reflect;

/*============================================================================
 * TIMING AND PERFORMANCE
 *===========================================================================*/

extern double doubletime(void);
extern void my_usleep(long usecs);

/*============================================================================
 * GAME LOGIC FUNCTIONS
 *===========================================================================*/

extern void gameintro(void);
extern void gamedemo(void);
extern void gamestart(void);
extern void gameintermission(void);
extern void gamehelp(void);
extern void gameinfo(void);
extern int gamelogic(void);
extern void gamesfx(void);
extern void gamereset(void);
extern void gamerender(void);
extern int gameinput(void);
extern int gamecycle(int lines, int cols);

extern void creditscreen(void);
extern void paint_walls(int verbose);

/*============================================================================
 * GAME STATE - Score, Lives, Level
 *===========================================================================*/

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

/*============================================================================
 * INPUT STATE
 *===========================================================================*/

extern int key_buffer;
extern int key_buffer_ERR;

/*============================================================================
 * PELLET/POWER-UP TIMERS
 *===========================================================================*/

extern long pellet_timer;
extern long pellet_time;

/*============================================================================
 * DISPLAY STATE
 *===========================================================================*/

extern unsigned short *inside_wall;
extern FILE *snapshot;
extern FILE *snapshot_txt;
extern int xoff_received;
extern double td;
extern const char *pager_notice;
extern const char *pager_remaining;
extern int pager_arrow_magic;
extern int reinit_requested;

/*============================================================================
 * GAME PHASE TRACKING
 *===========================================================================*/

extern long myman_intro;
extern unsigned long myman_start;
extern unsigned long myman_demo;
extern int munched;

/*============================================================================
 * PERFORMANCE AND RENDERING
 *===========================================================================*/

extern int old_lines;
extern int old_cols;
extern int old_score;
extern int old_showlives;
extern int old_level;
extern int ignore_delay;
extern long frameskip;
extern long frameskip0;
extern long frameskip1;
extern long scrolling;
extern long frames;
extern long winning;
extern unsigned long mymandelay;
extern unsigned long mindelay;

/*============================================================================
 * COLLISION AND INTERACTION
 *===========================================================================*/

extern int ghost_eaten_timer;

/*============================================================================
 * PAUSE STATE
 *===========================================================================*/

extern int paused;

/*============================================================================
 * INTERMISSION STATE
 *===========================================================================*/

extern long intermission_running;
extern unsigned long myman_demo_setup;

/*============================================================================
 * RESET FLAGS
 *===========================================================================*/

extern int need_reset;

/*============================================================================
 * DISPLAY HELPERS
 *===========================================================================*/

extern char *tmp_notice;
extern const char *maze_ABOUT;
extern const char *maze_FIXME;
extern const char *maze_NOTE;
extern const char *tile_ABOUT;
extern const char *tile_FIXME;
extern const char *tile_NOTE;
extern const char *sprite_ABOUT;
extern const char *sprite_FIXME;
extern const char *sprite_NOTE;

/*============================================================================
 * MESSAGE TEXT
 *===========================================================================*/

extern const char *msg_READY;
extern const char *msg_GAMEOVER;
extern const char *msg_PLAYER1;
extern const char *msg_PLAYER2;

/*============================================================================
 * COLOR CONFIGURATION
 *===========================================================================*/

extern const char *maze_WALL_COLORS;
extern size_t maze_WALL_COLORS_len;
extern const char *maze_DOT_COLORS;
extern size_t maze_DOT_COLORS_len;
extern const char *maze_PELLET_COLORS;
extern size_t maze_PELLET_COLORS_len;
extern const char *maze_MORTAR_COLORS;
extern size_t maze_MORTAR_COLORS_len;

/*============================================================================
 * LEVEL CONFIGURATION
 *===========================================================================*/

extern long *maze_GHOSTS;
extern size_t maze_GHOSTS_len;
extern double *maze_RGHOST;
extern size_t maze_RGHOST_len;
extern double *maze_CGHOST;
extern size_t maze_CGHOST_len;
extern double *maze_ROGHOST;
extern size_t maze_ROGHOST_len;
extern double *maze_COGHOST;
extern size_t maze_COGHOST_len;
extern double *maze_RFRUIT;
extern size_t maze_RFRUIT_len;
extern double *maze_CFRUIT;
extern size_t maze_CFRUIT_len;
extern double *maze_RTOP;
extern size_t maze_RTOP_len;
extern double *maze_RHERO;
extern size_t maze_RHERO_len;
extern double *maze_CHERO;
extern size_t maze_CHERO_len;
extern long *maze_RMSG;
extern size_t maze_RMSG_len;
extern long *maze_CMSG;
extern size_t maze_CMSG_len;
extern long *maze_RMSG2;
extern size_t maze_RMSG2_len;
extern long *maze_CMSG2;
extern size_t maze_CMSG2_len;

/*============================================================================
 * HERO/MOVEMENT STATE
 *===========================================================================*/

extern int dirhero;
extern long scroll_offset_x0;
extern long scroll_offset_y0;
extern int msglen;
extern int hero_dir;

/*============================================================================
 * LEVEL STATISTICS
 *===========================================================================*/

extern int *total_dots;
extern int *pellets;

/*============================================================================
 * LEVEL TRANSITIONS
 *===========================================================================*/

extern long flip_to;

/*============================================================================
 * DEBUG FLAGS
 *===========================================================================*/

extern int debug;
extern int ghosts_p;

/*============================================================================
 * SOUND EFFECTS
 *===========================================================================*/

extern unsigned long myman_sfx;

/*============================================================================
 * DISPLAY FLAGS
 *===========================================================================*/

extern int showlives;
extern int visible_frame;

/*============================================================================
 * CURSES WRAPPERS
 *===========================================================================*/

extern int my_clear(void);
extern void my_clearok(int ok);

/*============================================================================
 * BONUS SCORING
 *===========================================================================*/

extern int use_underline;
extern int use_color;
extern int bonus_score[8];

/*============================================================================
 * COLLISION DETECTION
 *===========================================================================*/

extern int check_collision(int eyes, int mean, int blue);
extern int find_home_dir(int s, int r, int c);

/*============================================================================
 * MAZE HEURISTICS
 *===========================================================================*/

extern long maze_visual(int n, int i, int j);

/*============================================================================
 * ENVIRONMENT HELPERS
 *===========================================================================*/

extern int myman_setenv(const char *name, const char *value);
extern char *myman_getenv(const char *name);

#endif /* GLOBALS_H */
