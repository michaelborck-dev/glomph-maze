/* maze_io.c - Maze file loading and parsing
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

/**
 * @brief Load maze layout from file
 *
 * Reads maze file in custom ASCII format containing:
 * - Header: "N WxH" (number of levels, width, height)
 * - Optional flags: "~F" for maze rendering flags
 * - Optional metadata: "ABOUT:", "FIXME:", "NOTE:" directives
 * - Maze grid: Character-based map ('.' = dots, 'o' = pellets, '#' = walls,
 * etc.)
 * - Optional color map: Per-cell color codes
 *
 * Supports multiple maze levels in single file. Handles CP437/UTF-8 encoding.
 *
 * @param mazefile Path to maze file (searched in DATADIR if relative)
 * @param levels Output: number of maze levels in file
 * @param w Output: maze width in characters
 * @param h Output: maze height in characters
 * @param maze Output: allocated maze data buffer (caller must free)
 * @param flags Output: maze rendering flags
 * @param color Output: allocated color map buffer (caller must free, may be
 * NULL)
 * @param args Output: pointer to maze metadata string (static, do not free)
 *
 * @return 0 on success, 1 on error
 *
 * @note Allocates memory for maze and color buffers using malloc
 * @note Sets global variables: maze_ABOUT, maze_FIXME, maze_NOTE
 * @see writemaze, parse_maze_args, fopen_datafile
 */
int readmaze(const char* mazefile, int* levels, int* w, int* h, char** maze,
             int* flags, char** color, const char** args) {
    char  X;
    int   c = EOF, i, j;
    int   n;
    FILE* infile = NULL;

    infile = fopen_datafile(mazefile, "rb");
    if (!infile) {
        perror(mazefile);
        return 1;
    }
    ignore_bom_utf8(infile);
    {
        int rn, rw, rh;

        if ((fscanf(infile, "%d%d%c%d", &rn, &rw, &X, &rh) != 4) ||
            (tolower(X) != 'x')) {
            fprintf(stderr, "%s: can't find a dimension specification N WxH\n",
                    mazefile);
            return 1;
        }
        if ((rw < 1) || (rh < 1) || (rn < 1)) {
            fprintf(stderr,
                    "%s: dimension specification %d %dx%d is too small\n",
                    mazefile, rn, rw, rh);
            return 1;
        }
        *levels = rn;
        *h      = rh;
        *w      = rw;
    }
    *flags = 0;
    *args  = NULL;
    if (!feof(infile)) {
        c = fgetc_cp437_utf8(infile);
        if (c == '~') {
            if (fscanf(infile, "%d", flags) != 1) {
                fprintf(stderr,
                        "%s: can't find flags ~F after dimension specification "
                        "%d %dx%d\n",
                        mazefile, *levels, *w, *h);
                return 1;
            }
        } else {
            ungetc_cp437_utf8(c, infile);
        }
        c = fgetc_cp437_utf8(infile);
        if ((c == ' ') || (c == '\t')) {
            char*  args_tmp     = NULL;
            size_t args_tmp_len = 0;
            int    escaped      = 0;

            args_tmp = strdup("");
            if (!args_tmp) {
                perror("strdup");
                return 1;
            }
            while (((c = fgetc_cp437_utf8(infile)) != EOF) && (c != '\v') &&
                   (c != '\f') && (c != '\n') && (c != '\r')) {
                if (c == '\\') {
                    int c2;

                    escaped = !escaped;
                    if (escaped) {
                        c2 = fgetc_cp437_utf8(infile);
                        if (c2 == '\r') {
                            int c3;

                            c3 = fgetc_cp437_utf8(infile);
                            if (c3 == '\n') {
                                c2 = c3;
                            } else {
                                ungetc_cp437_utf8(c3, infile);
                            }
                        }
                        if ((c2 == '\v') || (c2 == '\f') || (c2 == '\n') ||
                            (c2 == '\r')) {
                            escaped = 0;
                            continue;
                        }
                        ungetc_cp437_utf8(c2, infile);
                    }
                }
                args_tmp = (char*)realloc(
                    (void*)args_tmp, args_tmp_len + 1 + ((c == 0) ? 4 : 1));
                if (!args_tmp) {
                    perror("realloc");
                    return 1;
                }
                if (c == 0) {
                    if (!escaped) {
                        args_tmp[args_tmp_len] = '\\';
                        args_tmp_len++;
                    }
                    args_tmp[args_tmp_len] = 'x';
                    args_tmp_len++;
                    args_tmp[args_tmp_len] = '0';
                    args_tmp_len++;
                    args_tmp[args_tmp_len] = '0';
                    args_tmp_len++;
                    args_tmp[args_tmp_len] = '\0';
                } else {
                    args_tmp[args_tmp_len] = c;
                    args_tmp_len++;
                    args_tmp[args_tmp_len] = '\0';
                }
                if (escaped && (c != '\\')) {
                    escaped = 0;
                }
            }
            if (args_tmp_len) {
                *args = args_tmp;
            } else {
                free((void*)args_tmp);
                args_tmp = NULL;
            }
        }
        if ((c == '\n') || (c == '\r') || (c == '\v') || (c == '\f')) {
            ungetc(c, infile);
        } else {
            ungetc_cp437_utf8(c, infile);
        }
    }
    if ((!feof(infile)) && (((X = fgetc_cp437_utf8(infile)) != '\n') &&
                            (X != '\r') && (X != '\v') && (X != '\f'))) {
        fprintf(
            stderr,
            "%s: garbage after dimension specification %d %dx%d~%d%s%s %X\n",
            mazefile, *levels, *w, *h, *flags, *args ? " " : "",
            *args ? *args : "", X);
        return 1;
    }
    *maze = (char*)malloc(*levels * *h * (*w + 1) * sizeof(**maze));
    if (!*maze) {
        perror("malloc");
        return 1;
    }
    memset((void*)*maze, 0, *levels * *h * (*w + 1) * sizeof(**maze));
    *color = (char*)malloc(*levels * *h * (*w + 1) * sizeof(**color));
    if (!*color) {
        perror("malloc");
        return 1;
    }
    memset((void*)*color, 0, *levels * *h * (*w + 1) * sizeof(**color));
    for (n = 0; n < *levels; n++) {
        for (i = 0; i < *h; i++) {
            for (j = 0; j < *w; j++) {
                if ((c = fgetc_cp437_utf8(infile)) == EOF) {
                    if (feof(infile))
                        fprintf(stderr, "%s: premature EOF\n", mazefile);
                    else
                        perror(mazefile);
                    return 1;
                } else if ((c == '\r') || (c == '\n') || (c == '\f') ||
                           (c == '\v'))
                    j--;
                else
                    (*maze)[(n * *h + i) * (*w + 1) + j] =
                        (char)(unsigned char)c;
            }
            if (ISPELLET(c) || ISDOT(c)) {
                c = ' ';
            }
            (*maze)[(n * *h + i) * (*w + 1) + *w] = (char)(unsigned char)c;
        }
    }
    fclose(infile);
    return 0;
}

/**
 * @brief Export maze data as C source code
 *
 * Generates C code that can be compiled into the binary as a built-in maze.
 * Outputs variable declarations and string literals for maze data and metadata.
 * Useful for creating standalone builds without external maze files.
 *
 * Output includes:
 * - maze_n, maze_w, maze_h (dimensions)
 * - maze_flags (rendering flags)
 * - maze_args (metadata string)
 * - maze_data (escaped maze grid data)
 * - maze_color_data (optional color map)
 *
 * @param mazefile Original maze filename (embedded in output as
 * builtin_mazefile)
 *
 * @note Prints to stdout - redirect to capture output
 * @note Uses mymanescape() to properly escape special characters
 * @see readmaze, mymanescape
 */
void writemaze(const char* mazefile) {
    int n;
    int i;

    printf("int maze_n = %d;\n", maze_n);
    printf("int maze_w = %d;\n", maze_w);
    printf("int maze_h = %d;\n", maze_h);
    printf("int maze_flags = %d;\n", maze_flags);
    printf("const char *maze_args = ");
    if (maze_args) {
        printf("\"");
        mymanescape(maze_args, strlen(maze_args));
        printf("\"");
    } else {
        printf("0");
    }
    printf(";\n");
    printf("static const char builtin_mazefile_str[] = \"");
    mymanescape(mazefile, strlen(mazefile));
    printf("\";\n");
    printf("const char *builtin_mazefile = builtin_mazefile_str;\n");
    printf("const char *maze_data = \n");
    for (n = 0; n < maze_n; n++) {
        printf("/* 0x%X */\n", n);
        for (i = 0; i < maze_h; i++) {
            printf("  \"");
            mymanescape(maze + ((n * maze_h) + i) * (maze_w + 1), maze_w + 1);
            printf("\"\n");
        }
    }
    printf(";\n");
    printf("const char *maze_color_data = \n");
    for (n = 0; n < maze_n; n++) {
        printf("/* 0x%X */\n", n);
        for (i = 0; i < maze_h; i++) {
            printf("  \"");
            mymanescape(maze_color + ((n * maze_h) + i) * (maze_w + 1),
                        maze_w + 1);
            printf("\"\n");
        }
    }
    printf(";\n");
}

/**
 * @brief Parse maze metadata arguments from file header
 *
 * Processes key=value pairs from maze file metadata line. Supports:
 * - FLIP_TO=N: Set maze level to flip to after completing all levels
 * - WALL_COLORS=list: Custom wall color sequence
 * - DOT_COLORS=list: Custom dot color sequence
 * - PELLET_COLORS=list: Custom power pellet color sequence
 * - MORTAR_COLORS=list: Custom mortar/border color sequence
 * - GHOSTS=list: Ghost spawn configuration
 * - Position lists: RGHOST, CGHOST, ROGHOST, COGHOST, RFRUIT, CFRUIT, etc.
 *
 * Lists use comma/space separation and support both integers and floating
 * point.
 *
 * @param mazefile Maze filename (for error messages)
 * @param maze_args Argument string from maze file header
 *
 * @return 0 on success, 1 on parse error
 *
 * @note Sets global configuration variables (flip_to, maze_WALL_COLORS, etc.)
 * @note Uses strword, strtollist, strtodlist for parsing
 * @see readmaze, strword, strtollist, strtodlist
 */
int parse_maze_args(const char* mazefile, const char* maze_args) {
    const char* argp = maze_args;
    int         c;

    while (1) {
        const char* endp;

        c = *argp;
        if (!c)
            break;
        if (isspace(c)) {
            argp++;
            continue;
        } else if ((endp = strchr(argp, '=')) != 0) {
            if (!strncmp(argp, "FLIP_TO", endp - argp)) {
                argp    = endp + 1;
                flip_to = strtol(argp, (char**)&endp, 0);
                if (endp == argp) {
                    perror("strtol: FLIP_TO");
                    return 1;
                }
                if ((*endp) && !isspace(*endp)) {
                    fprintf(stderr, "%s: FLIP_TO: garbage after argument: %s\n",
                            mazefile, endp);
                    fflush(stderr);
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "GHOSTS", endp - argp)) {
                long*  tmp_ghosts     = NULL;
                size_t tmp_ghosts_len = 0;

                argp       = endp + 1;
                tmp_ghosts = strtollist_word(argp, &endp, &tmp_ghosts_len);
                if (!tmp_ghosts) {
                    perror("GHOSTS");
                    return 1;
                }
                argp = endp;
                if (!ghosts_p) {
                    maze_GHOSTS     = tmp_ghosts;
                    maze_GHOSTS_len = tmp_ghosts_len;
                } else {
                    free((void*)tmp_ghosts);
                    tmp_ghosts     = NULL;
                    tmp_ghosts_len = 0;
                }
            } else if (!strncmp(argp, "RGHOST", endp - argp)) {
                argp        = endp + 1;
                maze_RGHOST = strtodlist_word(argp, &endp, &maze_RGHOST_len);
                if (!maze_RGHOST) {
                    perror("RGHOST");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "CGHOST", endp - argp)) {
                argp        = endp + 1;
                maze_CGHOST = strtodlist_word(argp, &endp, &maze_CGHOST_len);
                if (!maze_CGHOST) {
                    perror("CGHOST");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "ROGHOST", endp - argp)) {
                argp         = endp + 1;
                maze_ROGHOST = strtodlist_word(argp, &endp, &maze_ROGHOST_len);
                if (!maze_ROGHOST) {
                    perror("ROGHOST");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "COGHOST", endp - argp)) {
                argp         = endp + 1;
                maze_COGHOST = strtodlist_word(argp, &endp, &maze_COGHOST_len);
                if (!maze_COGHOST) {
                    perror("COGHOST");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "RFRUIT", endp - argp)) {
                argp        = endp + 1;
                maze_RFRUIT = strtodlist_word(argp, &endp, &maze_RFRUIT_len);
                if (!maze_RFRUIT) {
                    perror("RFRUIT");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "CFRUIT", endp - argp)) {
                argp        = endp + 1;
                maze_CFRUIT = strtodlist_word(argp, &endp, &maze_CFRUIT_len);
                if (!maze_CFRUIT) {
                    perror("CFRUIT");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "RTOP", endp - argp)) {
                argp      = endp + 1;
                maze_RTOP = strtodlist_word(argp, &endp, &maze_RTOP_len);
                if (!maze_RTOP) {
                    perror("RTOP");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "RHERO", endp - argp)) {
                argp       = endp + 1;
                maze_RHERO = strtodlist_word(argp, &endp, &maze_RHERO_len);
                if (!maze_RHERO) {
                    perror("RHERO");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "CHERO", endp - argp)) {
                argp       = endp + 1;
                maze_CHERO = strtodlist_word(argp, &endp, &maze_CHERO_len);
                if (!maze_CHERO) {
                    perror("CHERO");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "RMSG", endp - argp)) {
                argp      = endp + 1;
                maze_RMSG = strtollist_word(argp, &endp, &maze_RMSG_len);
                if (!maze_RMSG) {
                    perror("RMSG");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "CMSG", endp - argp)) {
                argp      = endp + 1;
                maze_CMSG = strtollist_word(argp, &endp, &maze_CMSG_len);
                if (!maze_CMSG) {
                    perror("CMSG");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "RMSG2", endp - argp)) {
                argp       = endp + 1;
                maze_RMSG2 = strtollist_word(argp, &endp, &maze_RMSG2_len);
                if (!maze_RMSG2) {
                    perror("RMSG2");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "CMSG2", endp - argp)) {
                argp       = endp + 1;
                maze_CMSG2 = strtollist_word(argp, &endp, &maze_CMSG2_len);
                if (!maze_CMSG2) {
                    perror("CMSG2");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "ABOUT", endp - argp)) {
                argp       = endp + 1;
                maze_ABOUT = strword(argp, &endp, 0);
                if (!maze_ABOUT) {
                    perror("ABOUT");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "NOTE", endp - argp)) {
                argp      = endp + 1;
                maze_NOTE = strword(argp, &endp, 0);
                if (!maze_NOTE) {
                    perror("NOTE");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "FIXME", endp - argp)) {
                argp       = endp + 1;
                maze_FIXME = strword(argp, &endp, 0);
                if (!maze_FIXME) {
                    perror("FIXME");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "DIRHERO", endp - argp)) {
                char* dirhero_tmp = NULL;

                argp        = endp + 1;
                dirhero_tmp = strword(argp, &endp, 0);
                if (!dirhero_tmp) {
                    perror("DIRHERO");
                    return 1;
                }
                argp = endp;
                if (!strcmp(dirhero_tmp, "UP")) {
                    dirhero = MYMAN_UP;
                } else if (!strcmp(dirhero_tmp, "DOWN")) {
                    dirhero = MYMAN_DOWN;
                } else if (!strcmp(dirhero_tmp, "LEFT")) {
                    dirhero = MYMAN_LEFT;
                } else if (!strcmp(dirhero_tmp, "RIGHT")) {
                    dirhero = MYMAN_RIGHT;
                } else {
                    fprintf(stderr,
                            "%s: DIRHERO: must be one of UP, DOWN, LEFT or "
                            "RIGHT; got \"%s\" instead\n",
                            mazefile, dirhero_tmp);
                    fflush(stderr);
                    return 1;
                }
                free((void*)dirhero_tmp);
                dirhero_tmp = NULL;
            } else if (!strncmp(argp, "GAMEOVER", endp - argp)) {
                argp         = endp + 1;
                msg_GAMEOVER = strword(argp, &endp, 0);
                if (!msg_GAMEOVER) {
                    perror("GAMEOVER");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "PLAYER1", endp - argp)) {
                argp        = endp + 1;
                msg_PLAYER1 = strword(argp, &endp, 0);
                if (!msg_PLAYER1) {
                    perror("PLAYER1");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "PLAYER2", endp - argp)) {
                argp        = endp + 1;
                msg_PLAYER2 = strword(argp, &endp, 0);
                if (!msg_PLAYER2) {
                    perror("PLAYER2");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "READY", endp - argp)) {
                argp      = endp + 1;
                msg_READY = strword(argp, &endp, 0);
                if (!msg_READY) {
                    perror("READY");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "WALL_COLORS", endp - argp)) {
                argp             = endp + 1;
                maze_WALL_COLORS = strword(argp, &endp, &maze_WALL_COLORS_len);
                if (!maze_WALL_COLORS) {
                    perror("WALL_COLORS");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "DOT_COLORS", endp - argp)) {
                argp            = endp + 1;
                maze_DOT_COLORS = strword(argp, &endp, &maze_DOT_COLORS_len);
                if (!maze_MORTAR_COLORS) {
                    perror("DOT_COLORS");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "PELLET_COLORS", endp - argp)) {
                argp = endp + 1;
                maze_PELLET_COLORS =
                    strword(argp, &endp, &maze_PELLET_COLORS_len);
                if (!maze_MORTAR_COLORS) {
                    perror("PELLET_COLORS");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "MORTAR_COLORS", endp - argp)) {
                argp = endp + 1;
                maze_MORTAR_COLORS =
                    strword(argp, &endp, &maze_MORTAR_COLORS_len);
                if (!maze_MORTAR_COLORS) {
                    perror("MORTAR_COLORS");
                    return 1;
                }
                argp = endp;
            } else {
                fprintf(stderr, "%s: unrecognized maze argument: ", mazefile);
                fflush(stderr);
                fwrite((void*)argp, 1, endp - argp, stderr);
                fflush(stderr);
                fprintf(stderr, "\n");
                fflush(stderr);
                return 1;
            }
        } else {
            fprintf(stderr, "%s: unrecognized maze arguments: %s\n", mazefile,
                    argp);
            fflush(stderr);
            return 1;
        }
    }
    return 0;
}
