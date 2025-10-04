/*
 * input_state.h - Input and timing state management
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
 * @file input_state.h
 * @brief Input handling and timing management
 *
 * Encapsulates all input and timing-related state including:
 * - Keyboard input (key_buffer, key constants)
 * - Control mapping (MYMANKEYS)
 * - Timing utilities (doubletime, my_usleep)
 * - Character encoding (CP437, UTF-8, Unicode)
 *
 * @note Part of Phase 3 modularization - extracted from globals.h
 */

#ifndef INPUT_STATE_H
#define INPUT_STATE_H

#include <getopt.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

extern int key_buffer;
extern int key_buffer_ERR;

extern const char*    MYMANKEYS_prefix;
extern const char*    MOREMESSAGE;
extern const char*    DONEMESSAGE;
extern const char*    MYMANLEGALNOTICE;
extern const char*    MYMANKEYS;
extern const char*    short_options;
extern struct option* long_options;

extern const char* progname;

extern const unsigned long  uni_cp437_halfwidth[256];
extern const unsigned long  uni_cp437_fullwidth[256];
extern const unsigned char  cp437_fullwidth_rhs[256];
extern uint8_t              fallback_cp437[256];
extern uint8_t              reflect_cp437[256];
extern const unsigned long* uni_cp437;

extern const uint8_t udlr[256];

extern short   mille_to_scale(short n, short scale);
extern int     fgetc_cp437_utf8(FILE* stream);
extern int     fputc_utf8(unsigned long u, FILE* stream);
extern int     fputc_utf8_cp437(int c, FILE* stream);
extern int     ungetc_cp437_utf8(int c, FILE* stream);
extern char*   strword(const char* from, const char** endp, size_t* lenp);
extern long*   strtollist(const char* from, const char** endp, size_t* lenp);
extern double* strtodlist(const char* from, const char** endp, size_t* lenp);
extern long* strtollist_word(const char* from, const char** endp, size_t* lenp);
extern double* strtodlist_word(const char* from, const char** endp,
                               size_t* lenp);
extern void    mymanescape(const char* s, int len);
extern char*   mystrdup(const char* s);

extern double doubletime(void);
extern void   my_usleep(long usecs);

extern int   myman_setenv(const char* name, const char* value);
extern char* myman_getenv(const char* name);

#endif /* INPUT_STATE_H */
