/*
 * screen.c - Curses wrapper implementation
 * 
 * Extracted from myman.c during Phase 2 refactoring.
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
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/* Need feature detection first */
#ifndef MYMAN_GUESS_H_INCLUDED
#include "guess.h"
#endif

#include "screen.h"
#include "globals.h"

/* Extracted from myman.c line 3366 */
void
snapshot_attrset(chtype attrs)
{
    snapshot_attrs = attrs;
}

/* Extracted from myman.c line 3267 */
/* simulate a subset of curses attributes in HTML; note that this
 * generates presentational markup (<font color="...">, <u>, <b>,
 * etc.) which is considered deprecated in modern HTML; however there
 * is really no acceptable alternative since this markup needs to look
 * colorful even in older browsers */
void
snapshot_attrset_active(chtype attrs)
{
    if (! snapshot)
    {
        return;
    }
    if (snapshot_attrs_active != attrs)
    {
        int i = 16;

        if (snapshot_use_color)
        {
            for (i = 0; i < (int) (sizeof(pen)/sizeof(*pen)); i ++)
            {
                if (pen[i] &&
                    snapshot_attrs_active == pen[i])
                {
                    fprintf(snapshot,
                            "</font>");
                    break;
                }
            }
        }
#ifdef MY_A_BOLD
        if (i == 16)
        {
            if (snapshot_attrs_active & MY_A_BOLD)
            {
                fprintf(snapshot,
                        "</b>");
            }
        }
#endif
#ifdef MY_A_UNDERLINE
        if (i == 16)
        {
            if (snapshot_attrs_active & MY_A_UNDERLINE)
            {
                fprintf(snapshot,
                        "</u>");
            }
        }
#endif
        snapshot_attrs_active = attrs;
        if (snapshot_use_color)
        {
            int iodd;

            for (iodd = 0; iodd < (int) (sizeof(pen)/sizeof(*pen)); iodd ++)
            {
                i = (((iodd & 1) ? 8 : 0) | ((iodd & 14) >> 1) | (iodd & ~15)) ^ 7;
                if (pen[i] &&
                    snapshot_attrs_active == pen[i])
                {
                    unsigned long r, g, b;

                    r = (255 * pen_pal[i % 16][0]) / 1000;
                    g = (255 * pen_pal[i % 16][1]) / 1000;
                    b = (255 * pen_pal[i % 16][2]) / 1000;
                    fprintf(snapshot,
                            "<font color=\"#%2.2lX%2.2lX%2.2lX\"",
                            r & 0xffUL, g & 0xffUL, b & 0xffUL);
                    if (i / 16)
                    {
                        r = (255 * pen_pal[i / 16][0]) / 1000;
                        g = (255 * pen_pal[i / 16][1]) / 1000;
                        b = (255 * pen_pal[i / 16][2]) / 1000;
                        fprintf(snapshot,
                                " style=\"%sbackground:#%2.2lX%2.2lX%2.2lX\"",
                                ((i / 16) == (i % 16)) ? "color: #000000; " : "",
                                r & 0xffUL, g & 0xffUL, b & 0xffUL);
                    }
                    fprintf(snapshot,
                            ">");
                    break;
                }
            }
        }
#ifdef MY_A_UNDERLINE
        if (i == 16)
        {
            if (snapshot_attrs_active & MY_A_UNDERLINE)
            {
                fprintf(snapshot,
                        "<u>");
            }
        }
#endif
#ifdef MY_A_BOLD
        if (i == 16)
        {
            if (snapshot_attrs_active & MY_A_BOLD)
            {
                fprintf(snapshot,
                        "<b>");
            }
        }
#endif
    }
    fflush(snapshot);
}

/* Extracted from myman.c line 3696 */
/* Note: Always compiled in screen.c, guards remain in myman.c */
#ifndef DANGEROUS_ATTRS
#define DANGEROUS_ATTRS 0
#endif

#if DANGEROUS_ATTRS
static chtype my_attrs = 0;
#endif

int
my_real_attrset(chtype attrs)
{
#if DANGEROUS_ATTRS
    if (attrs)
    {
        int cur_x, cur_y;

        getyx(stdscr, cur_y, cur_x);
        /* classic BSD curses has an annoying bug which causes it to
         * hang if attributes are used in the last writable screen
         * cell */
        if ((cur_x >= (COLS - (CJK_MODE ? 1 : 0) - 2 * (cur_y == (LINES - 1)))))
        {
            return 1;
        }
    }
#endif
#if HAVE_ATTRSET
    attrset(attrs);
#else
    {
#ifdef MY_A_STANDOUT
        if (attrs & MY_A_STANDOUT) standout();
        else standend();
#endif
#if HAVE_SETATTR
        {
#ifdef MY_A_BLINK
#ifdef _BLINK
            if (attrs & MY_A_BLINK) setattr(_BLINK);
            else clrattr(_BLINK);
#endif
#endif
#ifdef MY_A_BOLD
#ifdef _BOLD
            if (attrs & MY_A_BOLD) setattr(_BOLD);
            else clrattr(_BOLD);
#endif
#endif
#ifdef MY_A_REVERSE
#ifdef _REVERSE
            if (attrs & MY_A_REVERSE) setattr(_REVERSE);
            else clrattr(_REVERSE);
#endif
#endif
#ifdef MY_A_UNDERLINE
#ifdef _UNDERLINE
            if (attrs & MY_A_UNDERLINE) setattr(_UNDERLINE);
            else clrattr(_UNDERLINE);
#endif
#endif
        }
#endif
    }
#endif
    return 1;
}

/* Extracted from myman.c line 3765 */
int
my_attrset(chtype attrs)
{
    snapshot_attrset(attrs);
#if USE_ATTR || USE_COLOR
    attrs ^= (snapshot || snapshot_txt) ?
#ifdef MY_A_REVERSE
      MY_A_REVERSE
#else
      0
#endif
      : 0;
#if DANGEROUS_ATTRS
    my_attrs = attrs;
#else
    my_real_attrset(attrs);
#endif
#endif /* USE_ATTR || USE_COLOR */
    return 1;
}

#ifndef CRLF
#define CRLF "\r\n"
#endif

/* Extracted from myman.c line 3595 */
void
my_move(int y, int x)
{
    if ((y < 0) || (x < 0) || (y > LINES) || (x > COLS))
    {
        return;
    }
    if ((snapshot || snapshot_txt)
        &&
        ((x != snapshot_x) || (y != snapshot_y)))
    {
        snapshot_attrset_active(0);
        if ((snapshot || snapshot_txt)
            &&
            (y < snapshot_y))
        {
            if (snapshot)
            {
                fprintf(snapshot, "<!-- cuu%d -->", snapshot_y - y);
                fflush(snapshot);
            }
            snapshot_y = y;
        }
        if (snapshot && (x < snapshot_x) && (y == snapshot_y))
        {
            fprintf(snapshot, "<!-- cub%d -->", snapshot_x - x);
            fflush(snapshot);
        }
        while ((y > snapshot_y) || (x < snapshot_x))
        {
            snapshot_y ++;
            snapshot_x = 0;
            if (snapshot)
            {
                fprintf(snapshot,
                        CRLF);
                fflush(snapshot);
            }
            if (snapshot_txt)
            {
                fprintf(snapshot_txt,
                        CRLF);
                fflush(snapshot_txt);
            }
        }
        while (x > snapshot_x)
        {
            if (snapshot)
            {
                fprintf(snapshot,
                        " ");
                fflush(snapshot);
            }
            if (snapshot_txt)
            {
                fprintf(snapshot_txt,
                        " ");
                fflush(snapshot_txt);
            }
            snapshot_x ++;
        }
    }

    do
    {
        int cur_y, cur_x;

        getyx(stdscr, cur_y, cur_x);
        if (location_is_suspect)
        {
            if (last_valid_col == (COLS - 1))
            {
                last_valid_col = -1;
                last_valid_line ++;
            }
            while (y > last_valid_line)
            {
                move(last_valid_line, last_valid_col + 1);
                clrtoeol();
                last_valid_line ++;
                last_valid_col = -1;
            }
            while ((y == last_valid_line)
                   &&
                   (x > (last_valid_col + 1)))
            {
                move(last_valid_line, ++ last_valid_col);
                addch(' ');
            }
        }
        getyx(stdscr, cur_y, cur_x);
        if ((y != cur_y) || (x != cur_x))
        {
            move(y, x);
        }
    }
    while(0);
}
