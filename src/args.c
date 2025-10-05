/* args.c - Command-line argument parsing for Glomph Maze
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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "globals.h"
#include "utils.h"

/* Build configuration - MYMANSIZE and file paths are set by CMake per variant
 */

#ifndef MYMANSIZE
#define MYMANSIZE "big"
#endif
const char* MYMANSIZE_str = MYMANSIZE;
#undef MYMANSIZE
#define MYMANSIZE MYMANSIZE_str

#ifndef TILEFILE
#define TILEFILE TILEDIR "/chr5x2.txt"
#endif
const char TILEFILE_str[] = TILEFILE;
#undef TILEFILE
#define TILEFILE TILEFILE_str
#define builtin_tilefile TILEFILE

#ifndef SPRITEFILE
#define SPRITEFILE SPRITEDIR "/spr7x3.txt"
#endif
const char SPRITEFILE_str[] = SPRITEFILE;
#undef SPRITEFILE
#define SPRITEFILE SPRITEFILE_str
#define builtin_spritefile SPRITEFILE

#ifndef MYMANVARIANT
#define MYMANVARIANT "myman"
#endif
const char* MYMANVARIANT_str = MYMANVARIANT;
#undef MYMANVARIANT
#define MYMANVARIANT MYMANVARIANT_str

#ifndef MAZEFILE
#define MAZEFILE MAZEDIR "/maze.txt"
#endif
char MAZEFILE_str[] = MAZEFILE;
#undef MAZEFILE
#define MAZEFILE MAZEFILE_str
#define builtin_mazefile MAZEFILE

/* Usage summary macro */
#ifndef XCURSES_USAGE
#define XCURSES_USAGE
#endif

#define SUMMARY(progname)                                                      \
    "Usage: %s [-h] [options]" XCURSES_USAGE "\n", progname

static void handle_display_options(int opt) {
    switch (opt) {
    case 'r':
        use_raw = 1;
        break;
    case 'R':
        use_raw = 0;
        break;
    case 'e':
        use_raw_ucs = 1;
        break;
    case 'E':
        use_raw_ucs = 0;
        break;
    case 'a':
        use_acs   = 0;
        use_acs_p = 1;
        break;
    case 'A':
        use_acs   = 1;
        use_acs_p = 1;
        break;
    case 'x':
        reflect = 1;
        break;
    case 'X':
        reflect = 0;
        break;
    case 'n':
        use_color_p = 1;
        use_color   = 0;
        break;
    case 'c':
        use_color_p = 1;
        use_color   = 1;
        break;
    case 'B':
        use_dim_and_bright   = 1;
        use_dim_and_bright_p = 1;
        break;
    case 'N':
        use_dim_and_bright   = 0;
        use_dim_and_bright_p = 1;
        break;
    case 'o':
        use_bullet_for_dots   = 1;
        use_bullet_for_dots_p = 1;
        break;
    case 'p':
        use_bullet_for_dots   = 0;
        use_bullet_for_dots_p = 1;
        break;
    case '1':
        use_fullwidth = 0;
        break;
    case '2':
        use_fullwidth = 1;
        break;
    case 'u':
        use_underline = 1;
        break;
    case 'U':
        use_underline = 0;
        break;
    case 'i':
        use_idlok = 0;
        break;
    case 'I':
        use_idlok = 1;
        break;
    }
}

/* Handle audio options (-b, -q) */
static void handle_audio_options(int opt) {
    switch (opt) {
    case 'b':
        use_sound = 1;
        break;
    case 'q':
        use_sound = 0;
        break;
    }
}

/* Handle file path options (-m, -s, -t) */
static void handle_file_options(int opt, const char** mazefile,
                                const char** spritefile,
                                const char** tilefile) {
    switch (opt) {
    case 'm':
        *mazefile = optarg;
        break;
    case 's':
        *spritefile = optarg;
        break;
    case 't':
        *tilefile = optarg;
        break;
    }
}

/* Handle dump/debug options (-M, -S, -T, -f, -F) */
static void handle_dump_options(int opt, int* dump_maze, int* dump_sprite,
                                int* dump_tile) {
    switch (opt) {
    case 'M':
        *dump_maze = 1;
        nogame     = 1;
        break;
    case 'S':
        *dump_sprite = 1;
        nogame       = 1;
        break;
    case 'T':
        *dump_tile = 1;
        nogame     = 1;
        break;
    case 'f':
        if (freopen(optarg, "a", stdout) == NULL) {
            perror(optarg);
            fflush(stderr), exit(1);
        }
        break;
    case 'F':
        if (freopen(optarg, "w", stdout) == NULL) {
            perror(optarg);
            fflush(stderr), exit(1);
        }
        break;
    }
}

/* Handle info/help options (-V, -h, -k, -L) - these exit immediately */
static void handle_info_options(int opt, const char* mazefile,
                                const char* spritefile, const char* tilefile) {
    switch (opt) {
    case 'V':
        printf("%s-%s (%s) %s\n%s\n", MYMANVARIANT, MYMANSIZE, MYMAN,
               MYMANVERSION, MYMANCOPYRIGHT);
        fflush(stdout), fflush(stderr), exit(0);
    case 'h':
        usage(mazefile, spritefile, tilefile);
        fflush(stdout), fflush(stderr), exit(0);
    case 'k':
        printf("%s", MYMANKEYS);
        fflush(stdout), fflush(stderr), exit(0);
    case 'L':
        printf("%s", MYMANLEGALNOTICE);
        fflush(stdout), fflush(stderr), exit(0);
    }
}

void parse_myman_args(int argc, char** argv) {
    int           i;
    int           dump_maze = 0, dump_sprite = 0, dump_tile = 0;
    const char*   tilefile   = TILEFILE;
    const char*   spritefile = SPRITEFILE;
    const char*   mazefile   = MAZEFILE;
    int           option_index;
    const char*   defvariant = MYMANVARIANT;
    const char*   defsize    = MYMANSIZE;
    unsigned long uli;

    while ((i = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1)
        /* Delegate to helper functions for simple option categories */
        handle_info_options(i, mazefile, spritefile, tilefile);
    handle_display_options(i);
    handle_audio_options(i);
    handle_file_options(i, &mazefile, &spritefile, &tilefile);
    handle_dump_options(i, &dump_maze, &dump_sprite, &dump_tile);

    /* Handle complex options that need validation or local state */
    switch (i) {
    case 'v':
        defvariant = optarg;
        break;
    case 'z':
        defsize = optarg;
        break;
    case 'd': {
        char garbage;

        if (sscanf(optarg, "%lu%c", &uli, &garbage) != 1) {
            fprintf(stderr,
                    "%s: argument to -d must be an unsigned long integer.\n",
                    progname);
            fflush(stderr), exit(1);
        }
        mymandelay = uli;
        mindelay   = mymandelay / 2;
        break;
    }
    case 'D': {
        char*       name;
        const char* value;

        value = "1";
        name  = strdup(optarg);
        if (!name) {
            perror("strdup");
            fflush(stderr), exit(1);
        }
        if (strchr(name, '=')) {
            *(strchr(name, '=')) = '\0';
            value                = name + strlen(name) + 1;
        }
        if (myman_setenv(name, value)) {
            perror("setenv");
            fflush(stderr), exit(1);
        }
        {
            const char* check_value;

            check_value = myman_getenv(name);
            if (check_value ? strcmp(check_value, value) : *value) {
                fprintf(
                    stderr, "setenv: did not preserve value, %s=%s vs %s=%s\n",
                    name, value, name, check_value ? check_value : "(null)");
                fflush(stderr), exit(1);
            }
        }
        free((void*)name);
        break;
    }
    case 'g': {
        const char* tmp_ghosts_endp = NULL;

        maze_GHOSTS = strtollist(optarg, &tmp_ghosts_endp, &maze_GHOSTS_len);
        if (!maze_GHOSTS) {
            perror("-g");
            fflush(stderr), exit(1);
        } else if (tmp_ghosts_endp && *tmp_ghosts_endp) {
            fprintf(stderr, "%s: -g: garbage after argument: %s\n", progname,
                    tmp_ghosts_endp);
            fflush(stderr), exit(1);
        }
        ghosts_p = 1;
        break;
    }
    case 'l': {
        char garbage;

        if (sscanf(optarg, "%lu%c", &uli, &garbage) != 1) {
            fprintf(stderr, "%s: argument to -l must be an unsigned integer.\n",
                    progname);
            fflush(stderr), exit(1);
        }
        lives = (int)uli;
        break;
    }
    default:
        fprintf(stderr, SUMMARY(progname));
        fflush(stderr), exit(2);
    }

    if (myman_getenv("MYMAN_DEBUG") && *(myman_getenv("MYMAN_DEBUG")) &&
        strcmp(myman_getenv("MYMAN_DEBUG"), "0")) {
        debug = atoi(myman_getenv("MYMAN_DEBUG"));
        debug = debug ? debug : 1;
    }
    if (optind < argc) {
        fprintf(stderr, SUMMARY(progname));
        fflush(stderr), exit(2);
    }
    if (strcmp(defvariant, MYMANVARIANT)) {
        fprintf(stderr, "%s: game variant `%s' not included in this binary\n",
                progname, defvariant);
        exit(2);
    }
    if (strcmp(defsize, MYMANSIZE)) {
        fprintf(stderr, "%s: game size `%s' not included in this binary\n",
                progname, defsize);
        exit(2);
    }

    if ((tilefile && readfont(tilefile, &tile_w, &tile_h, tile, tile_used,
                              &tile_flags, tile_color, &tile_args)) ||
        (spritefile &&
         readfont(spritefile, &sprite_w, &sprite_h, sprite, sprite_used,
                  &sprite_flags, sprite_color, &sprite_args)))
        exit(1);

    if (tile_args)
        if (parse_tile_args(tilefile ? tilefile : builtin_tilefile,
                            tile_args)) {
            exit(1);
        }

    if (sprite_args)
        if (parse_sprite_args(spritefile ? spritefile : builtin_spritefile,
                              sprite_args)) {
            exit(1);
        }

    gfx_reflect = reflect && !REFLECT_LARGE;

#if !MYMANDELAY
    if (mymandelay) {
        fprintf(stderr,
                "%s: compile with -DMYMANDELAY=1 to enable the -d option.\n",
                progname);
        fflush(stderr), exit(1);
    }
#else
    mymandelay = mymandelay / MYMANFIFTH;
    mindelay   = mymandelay / 2;
#endif

#ifdef BUILTIN_MAZE
    if (!mazefile) {
        maze = (char*)malloc(maze_n * maze_h * (maze_w + 1) * sizeof(*maze));
        if (!maze) {
            perror("malloc");
            exit(1);
        }
        memcpy((void*)maze, (void*)maze_data, maze_n * maze_h * (maze_w + 1));
        maze_color =
            (char*)malloc(maze_n * maze_h * (maze_w + 1) * sizeof(*maze_color));
        if (!maze_color) {
            perror("malloc");
            exit(1);
        }
        memcpy((void*)maze_color, (void*)maze_color_data,
               maze_n * maze_h * (maze_w + 1));
    }
#endif /* defined(BUILTIN_MAZE) */
    if (mazefile && readmaze(mazefile, &maze_n, &maze_w, &maze_h, &maze,
                             &maze_flags, &maze_color, &maze_args)) {
        exit(1);
    }
    if (maze_args)
        if (parse_maze_args(mazefile ? mazefile : builtin_mazefile,
                            maze_args)) {
            exit(1);
        }

    msglen     = MAX(MAX(strlen(msg_PLAYER1), strlen(msg_PLAYER2)),
                     MAX(strlen(msg_READY), strlen(msg_GAMEOVER)));
    total_dots = (int*)malloc(maze_n * sizeof(*total_dots));
    if (!total_dots) {
        perror("malloc");
        exit(1);
    }
    memset((void*)total_dots, 0, maze_n * sizeof(*total_dots));
    pellets = (int*)malloc(maze_n * sizeof(*pellets));
    if (!pellets) {
        perror("malloc");
        exit(1);
    }
    memset((void*)pellets, 0, maze_n * sizeof(*pellets));
    blank_maze =
        (char*)malloc(maze_n * maze_h * (maze_w + 1) * sizeof(*blank_maze));
    if (!blank_maze) {
        perror("malloc");
        exit(1);
    }
    memset((void*)blank_maze, 0,
           maze_n * maze_h * (maze_w + 1) * sizeof(*blank_maze));
    blank_maze_color = (char*)malloc(maze_n * maze_h * (maze_w + 1) *
                                     sizeof(*blank_maze_color));
    if (!blank_maze_color) {
        perror("malloc");
        exit(1);
    }
    memset((void*)blank_maze_color, 0,
           maze_n * maze_h * (maze_w + 1) * sizeof(*blank_maze_color));
    inside_wall = (unsigned short*)malloc(maze_n * maze_h * (maze_w + 1) *
                                          sizeof(*inside_wall));
    if (!inside_wall) {
        perror("malloc");
        exit(1);
    }
    memset((void*)inside_wall, 0,
           maze_n * maze_h * (maze_w + 1) * sizeof(*inside_wall));
    dirty_cell = (unsigned char*)malloc(maze_h * ((maze_w + 1 + 7) >> 3) *
                                        sizeof(*dirty_cell));
    if (!dirty_cell) {
        perror("malloc");
        exit(1);
    }
    memset((void*)dirty_cell, 0,
           maze_h * ((maze_w + 1 + 7) >> 3) * sizeof(*dirty_cell));
    home_dir = (unsigned char*)malloc(MAXGHOSTS * maze_h * (maze_w + 1) *
                                      sizeof(*home_dir));
    if (!home_dir) {
        perror("malloc");
        exit(1);
    }
    memset((void*)home_dir, 0,
           MAXGHOSTS * maze_h * (maze_w + 1) * sizeof(*home_dir));
    memcpy((void*)blank_maze, (void*)maze,
           (maze_w + 1) * maze_h * maze_n * sizeof(unsigned char));
    memcpy((void*)blank_maze_color, (void*)maze_color,
           (maze_w + 1) * maze_h * maze_n * sizeof(unsigned char));

    CLEAN_ALL();
    paint_walls(isatty(fileno(stderr)));
    gamereset();

    if (dump_maze)
        writemaze(mazefile ? mazefile : builtin_mazefile);

    if (dump_sprite)
        writefont(spritefile ? spritefile : builtin_spritefile, "sprite",
                  sprite_w, sprite_h, sprite, sprite_used, sprite_flags,
                  sprite_color, sprite_args);
    if (dump_tile)
        writefont(tilefile ? tilefile : builtin_tilefile, "tile", tile_w,
                  tile_h, tile, tile_used, tile_flags, tile_color, tile_args);
}
