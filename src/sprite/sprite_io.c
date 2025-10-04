/* sprite_io.c - Sprite and font file loading
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
 * @brief Load font/tileset/sprite graphics from file
 *
 * Reads graphics file in custom ASCII art format containing:
 * - Header: "WxH" (character width and height in pixels)
 * - Optional flags: "~F" for rendering flags
 * - Optional metadata: "ABOUT:", "FIXME:", "NOTE:" directives
 * - Character definitions: 256 possible characters, each as WxH grid of ASCII
 * art
 * - Optional color map: Per-pixel color codes
 *
 * Used for loading tiles, sprites, and character sets. Supports CP437/UTF-8.
 *
 * @param fontfile Path to graphics file (searched in DATADIR if relative)
 * @param w Output: character width in pixels
 * @param h Output: character height in pixels
 * @param font Output: array of 256 character pointers (allocated, caller must
 * free)
 * @param used Output: array of 256 flags indicating which characters are
 * defined
 * @param flags Output: rendering flags
 * @param color Output: array of 256 per-character color values
 * @param args Output: pointer to metadata string (static, do not free)
 *
 * @return 0 on success, 1 on error
 *
 * @note Allocates memory for each character bitmap using malloc
 * @note Used for tiles (readfont for tile[]), sprites (readfont for sprite[])
 * @see writefont, parse_tile_args, parse_sprite_args
 */
int readfont(const char* fontfile, int* w, int* h, const char** font, int* used,
             int* flags, int* color, const char** args) {
    FILE* infile;
    int   c, i, j, k;
    int   rw, rh;
    char  X;
    char* font_dynamic[256];

    *args  = NULL;
    *flags = 0;
    for (i = 0; i < 256; i++) {
        used[i]  = 0;
        color[i] = 0;
    }
    infile = fopen_datafile(fontfile, "rb");
    if (!infile) {
        perror(fontfile);
        return 1;
    }
    ignore_bom_utf8(infile);
    if ((fscanf(infile, "%d%c%d", &rw, &X, &rh) != 3) || (tolower(X) != 'x')) {
        fprintf(stderr, "%s: can't find a dimension specification WxH\n",
                fontfile);
        return 1;
    }
    *w = rw;
    *h = rh;
    for (i = 0; i < 256; i++) {
        font[i] = NULL;
    }
    for (i = 0; i < 256; i++) {
        font_dynamic[i] = (char*)malloc(rh * rw);
        if (!font_dynamic[i]) {
            perror("malloc");
            for (j = 0; j < i; j++) {
                free((void*)font_dynamic[j]);
            }
            return 1;
        }
    }
    memcpy((void*)font, (void*)font_dynamic, sizeof(font_dynamic));
    if (!feof(infile)) {
        c = fgetc_cp437_utf8(infile);
        if (c == '~') {
            if (fscanf(infile, "%d", flags) != 1) {
                fprintf(stderr,
                        "%s: can't find flags ~F after dimension specification "
                        "%dx%d\n",
                        fontfile, rw, rh);
                return 1;
            }
        } else {
            ungetc_cp437_utf8(c, infile);
        }
    }
    if (!feof(infile)) {
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
    while (!feof(infile)) {
        if (fscanf(infile, "%x", &i) != 1) {
            if (!feof(infile)) {
                long offset;

                offset = ftell(infile);
                fprintf(stderr,
                        "%s: can't find an index at byte %ld near "
                        "\'\\u%4.4lx\' or equivalent\n",
                        fontfile, offset,
                        uni_cp437[(unsigned int)(unsigned char)fgetc_cp437_utf8(
                            infile)]);
                return 1;
            }
            continue;
        }
        if ((i < 0) || (i > 255)) {
            fprintf(stderr, "%s: invalid index %2.2X ignored\n", fontfile, i);
            continue;
        }
        if (used[i])
            fprintf(stderr, "%s: duplicate definition for %2.2X\n", fontfile,
                    i);
        used[i] = 1;
        if ((c = fgetc_cp437_utf8(infile)) == '~') {
            if (fscanf(infile, "%x", &c) != 1) {
                if (!feof(infile)) {
                    fprintf(stderr, "%s: can't find a color for index %2.2X\n",
                            fontfile, i);
                    return 1;
                }
                continue;
            }
            if ((c < 0) || (c >= NPENS))
                fprintf(stderr, "%s: invalid color %2.2X ignored\n", fontfile,
                        c);
            else
                color[i] = c;
        } else
            ungetc_cp437_utf8(c, infile);
        for (j = 0; j < rh; j++)
            for (k = 0; k < rw; k++)
                font_dynamic[i][j * rw + k] = ' ';
        for (j = 0; j < rh; j++) {
            while ((c = fgetc_cp437_utf8(infile)) != ':')
                if (c == EOF) {
                    if (!feof(infile))
                        perror(fontfile);
                    else
                        fprintf(stderr, "%s: premature EOF in index %2.2X\n",
                                fontfile, i);
                    return 1;
                }
            if ((c = fgetc_cp437_utf8(infile)) == EOF)
                if (!feof(infile)) {
                    perror(fontfile);
                    return 1;
                }
            for (k = 0; (k < rw) && (c != '\v') && (c != '\f') && (c != '\n') &&
                        (c != '\r') && !feof(infile);
                 k++) {
                font_dynamic[i][j * rw + k] = c;
                if ((c = fgetc_cp437_utf8(infile)) == EOF) {
                    if (feof(infile))
                        continue;
                    perror(fontfile);
                    return 1;
                }
            }
            while ((c != '\v') && (c != '\f') && (c != '\n') && (c != '\r') &&
                   !feof(infile)) {
                if (c == EOF) {
                    if (feof(infile))
                        continue;
                    perror(fontfile);
                    return 1;
                }
                c = fgetc_cp437_utf8(infile);
            }
        }
    }
    fclose(infile);
    return 0;
}

/**
 * @brief Export font/tileset/sprite data as C source code
 *
 * Generates C code that can be compiled into binary as built-in graphics.
 * Outputs variable declarations and character data arrays for tiles or sprites.
 * Useful for creating standalone builds without external asset files.
 *
 * Output includes:
 * - builtin_<prefix>file: Original filename
 * - <prefix>_w, <prefix>_h: Character dimensions
 * - <prefix>_flags: Rendering flags
 * - <prefix>_args: Metadata string
 * - <prefix>_data: Array of character bitmaps
 * - <prefix>_used: Which characters are defined
 * - <prefix>_color: Per-character color data
 *
 * @param file Original filename (embedded in output)
 * @param prefix Variable name prefix ("tile" or "sprite")
 * @param w Character width in pixels
 * @param h Character height in pixels
 * @param font Array of 256 character bitmaps
 * @param used Array of 256 usage flags
 * @param flags Rendering flags
 * @param color Array of 256 color values
 * @param args Metadata string
 *
 * @note Prints to stdout - redirect to capture output
 * @note Uses mymanescape() to properly escape special characters
 * @see readfont, writemaze, mymanescape
 */
extern void writefont(const char* file, const char* prefix, int w, int h,
                      const char** font, int* used, int flags, int* color,
                      const char* args) {
    int c, i, j;

    printf("static const char builtin_%sfile_str[] = \"", prefix);
    mymanescape(file, strlen(file));
    printf("\";\n");
    printf("const char *builtin_%sfile = builtin_%sfile_str;\n", prefix,
           prefix);
    printf("int %s_flags = %d;\n", prefix, flags);
    printf("int %s_w = %d;\n", prefix, w);
    printf("int %s_h = %d;\n", prefix, h);
    printf("const char *%s_args = ", prefix);
    if (args) {
        printf("\"");
        mymanescape(args, strlen(args));
        printf("\"");
    } else {
        printf("0");
    }
    printf(";\n");
    for (c = 0; c < 256; c++) {
        printf("static const char builtin_%s_data_%d[%d * %d] = {", prefix, c,
               h, w);
        for (i = 0; i < h; i++) {
            printf("\n ");
            for (j = 0; j < w; j++) {
                char k;

                printf(" \'");
                k = font[c][i * w + j];
                mymanescape(&k, 1);
                printf("\'");
                if (((i + 1) < h) || ((j + 1) < w))
                    printf(",");
            }
        }
        printf("};\n");
    }
    printf("const char *%s[256] = {", prefix);
    for (c = 0; c < 256; c++) {
        printf("\n ");
        printf("builtin_%s_data_%d", prefix, c);
        if ((c + 1) < 256)
            printf(",");
    }
    printf("};\n");
    printf("int %s_used[256] = {\n", prefix);
    for (c = 0; c < 256; c++) {
        if (c && !(c & 3))
            printf(",\n");
        else if (c)
            printf(", ");
        printf("/* 0x%2.2X */ %d", c, used[c]);
    }
    printf("};\n");
    printf("int %s_color[256] = {\n", prefix);
    for (c = 0; c < 256; c++) {
        if (c && !(c & 3))
            printf(",\n");
        else if (c)
            printf(", ");
        printf("/* 0x%2.2X */ 0x%X", c, color[c]);
    }
    printf("};\n");
}

/**
 * @brief Parse tile/font metadata arguments from file header
 *
 * Processes key=value pairs from tile file metadata. Supports:
 * - ABOUT="text": Description shown in help/info screens
 * - NOTE="text": Developer notes
 * - FIXME="text": Known issues or todos
 *
 * @param tilefile Tile filename (for error messages)
 * @param tile_args Argument string from tile file header
 *
 * @return 0 on success, 1 on parse error
 *
 * @note Sets global variables: tile_ABOUT, tile_NOTE, tile_FIXME
 * @note Allocates memory using strword (caller should not free)
 * @see readfont, parse_sprite_args, strword
 */
int parse_tile_args(const char* tilefile, const char* tile_args) {
    const char* argp = tile_args;
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
            if (!strncmp(argp, "ABOUT", endp - argp)) {
                argp       = endp + 1;
                tile_ABOUT = strword(argp, &endp, 0);
                if (!tile_ABOUT) {
                    perror("ABOUT");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "NOTE", endp - argp)) {
                argp      = endp + 1;
                tile_NOTE = strword(argp, &endp, 0);
                if (!tile_NOTE) {
                    perror("NOTE");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "FIXME", endp - argp)) {
                argp       = endp + 1;
                tile_FIXME = strword(argp, &endp, 0);
                if (!tile_FIXME) {
                    perror("FIXME");
                    return 1;
                }
                argp = endp;
            } else {
                fprintf(stderr, "%s: unrecognized tile argument: ", tilefile);
                fflush(stderr);
                fwrite((void*)argp, 1, endp - argp, stderr);
                fflush(stderr);
                fprintf(stderr, "\n");
                fflush(stderr);
                return 1;
            }
        } else {
            fprintf(stderr, "%s: unrecognized tile arguments: %s\n", tilefile,
                    argp);
            fflush(stderr);
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Parse sprite metadata arguments from file header
 *
 * Processes key=value pairs from sprite file metadata. Supports same
 * arguments as parse_tile_args:
 * - ABOUT="text": Description shown in help/info screens
 * - NOTE="text": Developer notes
 * - FIXME="text": Known issues or todos
 *
 * @param spritefile Sprite filename (for error messages)
 * @param sprite_args Argument string from sprite file header
 *
 * @return 0 on success, 1 on parse error
 *
 * @note Sets global variables: sprite_ABOUT, sprite_NOTE, sprite_FIXME
 * @note Allocates memory using strword (caller should not free)
 * @see readfont, parse_tile_args, strword
 */
int parse_sprite_args(const char* spritefile, const char* sprite_args) {
    const char* argp = sprite_args;
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
            if (!strncmp(argp, "ABOUT", endp - argp)) {
                argp         = endp + 1;
                sprite_ABOUT = strword(argp, &endp, 0);
                if (!sprite_ABOUT) {
                    perror("ABOUT");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "NOTE", endp - argp)) {
                argp        = endp + 1;
                sprite_NOTE = strword(argp, &endp, 0);
                if (!sprite_NOTE) {
                    perror("NOTE");
                    return 1;
                }
                argp = endp;
            } else if (!strncmp(argp, "FIXME", endp - argp)) {
                argp         = endp + 1;
                sprite_FIXME = strword(argp, &endp, 0);
                if (!sprite_FIXME) {
                    perror("FIXME");
                    return 1;
                }
                argp = endp;
            } else {
                fprintf(stderr,
                        "%s: unrecognized sprite argument: ", spritefile);
                fflush(stderr);
                fwrite((void*)argp, 1, endp - argp, stderr);
                fflush(stderr);
                fprintf(stderr, "\n");
                fflush(stderr);
                return 1;
            }
        } else {
            fprintf(stderr, "%s: unrecognized sprite arguments: %s\n",
                    spritefile, argp);
            fflush(stderr);
            return 1;
        }
    }
    return 0;
}
