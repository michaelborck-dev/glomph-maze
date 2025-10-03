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
#include "utils.h"

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
/* Extracted from myman.c line 3791 */
void
snapshot_addch(short inbyte)
{

#undef SNAPSHOT_ADDCH__NARROWC
#if USE_WIDEC_SUPPORT
#define SNAPSHOT_ADDCH__NARROWC(c) NULL
#else
#define SNAPSHOT_ADDCH__NARROWC(c) (c)
#endif

    if (snapshot || snapshot_txt)
    {
        unsigned long codepoint;

        if (use_acs)
        {
            codepoint = ((use_raw && use_raw_ucs) ? uni_cp437 : uni_cp437_halfwidth)[inbyte];
            if (CJK_MODE && use_raw && use_raw_ucs)
            {
                snapshot_x ++;
            }
            else if (! use_raw)
            {
                switch (inbyte)
                {
                case 201:
                    if (altcharset_cp437[201] != altcharset_cp437[218])
                    {
                        codepoint = (altcharset_cp437[201] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[201])
                            ) ? uni_cp437_halfwidth[201] : (ascii_cp437[201] & MY_A_CHARTEXT);
                        break;
                    }
                case 218:
                    codepoint = (altcharset_cp437[218] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[218])
                        ) ? uni_cp437_halfwidth[218] : (ascii_cp437[218] & MY_A_CHARTEXT);
                    break;
                case 200:
                    if (altcharset_cp437[200] != altcharset_cp437[192])
                    {
                        codepoint = (altcharset_cp437[200] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[200])
                            ) ? uni_cp437_halfwidth[200] : (ascii_cp437[200] & MY_A_CHARTEXT);
                        break;
                    }
                case 192:
                    codepoint = (altcharset_cp437[192] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[192])
                        ) ? uni_cp437_halfwidth[192] : (ascii_cp437[192] & MY_A_CHARTEXT);
                    break;
                case 187:
                    if (altcharset_cp437[187] != altcharset_cp437[191])
                    {
                        codepoint = (altcharset_cp437[187] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[187])
                            ) ? uni_cp437_halfwidth[187] : (ascii_cp437[187] & MY_A_CHARTEXT);
                        break;
                    }
                case 191:
                    codepoint = (altcharset_cp437[191] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[191])
                        ) ? uni_cp437_halfwidth[191] : (ascii_cp437[191] & MY_A_CHARTEXT);
                    break;
                case 188:
                    if (altcharset_cp437[188] != altcharset_cp437[217])
                    {
                        codepoint = (altcharset_cp437[188] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[188])
                            ) ? uni_cp437_halfwidth[188] : (ascii_cp437[188] & MY_A_CHARTEXT);
                        break;
                    }
                case 217:
                    codepoint = (altcharset_cp437[217] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[217])
                        ) ? uni_cp437_halfwidth[217] : (ascii_cp437[217] & MY_A_CHARTEXT);
                    break;
                case 185:
                    if (altcharset_cp437[185] != altcharset_cp437[181])
                    {
                        codepoint = (altcharset_cp437[185] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[185])
                            ) ? uni_cp437_halfwidth[185] : (ascii_cp437[185] & MY_A_CHARTEXT);
                        break;
                    }
                case 181:
                    if (altcharset_cp437[181] != altcharset_cp437[182])
                    {
                        codepoint = (altcharset_cp437[181] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[181])
                            ) ? uni_cp437_halfwidth[181] : (ascii_cp437[181] & MY_A_CHARTEXT);
                        break;
                    }
                case 182:
                    if (altcharset_cp437[182] != altcharset_cp437[180])
                    {
                        codepoint = (altcharset_cp437[182] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[182])
                            ) ? uni_cp437_halfwidth[182] : (ascii_cp437[182] & MY_A_CHARTEXT);
                        break;
                    }
                case 180:
                    codepoint = (altcharset_cp437[180] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[180])
                        ) ? uni_cp437_halfwidth[180] : (ascii_cp437[180] & MY_A_CHARTEXT);
                    break;
                case 204:
                    if (altcharset_cp437[204] != altcharset_cp437[198])
                    {
                        codepoint = (altcharset_cp437[204] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[204])
                            ) ? uni_cp437_halfwidth[204] : (ascii_cp437[204] & MY_A_CHARTEXT);
                        break;
                    }
                case 198:
                    if (altcharset_cp437[198] != altcharset_cp437[199])
                    {
                        codepoint = (altcharset_cp437[198] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[198])
                            ) ? uni_cp437_halfwidth[198] : (ascii_cp437[198] & MY_A_CHARTEXT);
                        break;
                    }
                case 199:
                    if (altcharset_cp437[199] != altcharset_cp437[195])
                    {
                        codepoint = (altcharset_cp437[199] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[199])
                            ) ? uni_cp437_halfwidth[199] : (ascii_cp437[199] & MY_A_CHARTEXT);
                        break;
                    }
                case 195:
                    codepoint = (altcharset_cp437[195] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[195])
                        ) ? uni_cp437_halfwidth[195] : (ascii_cp437[195] & MY_A_CHARTEXT);
                    break;
                case 202:
                    if (altcharset_cp437[202] != altcharset_cp437[207])
                    {
                        codepoint = (altcharset_cp437[202] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[202])
                            ) ? uni_cp437_halfwidth[202] : (ascii_cp437[202] & MY_A_CHARTEXT);
                        break;
                    }
                case 207:
                    if (altcharset_cp437[207] != altcharset_cp437[208])
                    {
                        codepoint = (altcharset_cp437[207] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[207])
                            ) ? uni_cp437_halfwidth[207] : (ascii_cp437[207] & MY_A_CHARTEXT);
                        break;
                    }
                case 208:
                    if (altcharset_cp437[208] != altcharset_cp437[193])
                    {
                        codepoint = (altcharset_cp437[208] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[208])
                            ) ? uni_cp437_halfwidth[208] : (ascii_cp437[208] & MY_A_CHARTEXT);
                        break;
                    }
                case 193:
                    codepoint = (altcharset_cp437[193] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[193])
                        ) ? uni_cp437_halfwidth[193] : (ascii_cp437[193] & MY_A_CHARTEXT);
                    break;
                case 203:
                    if (altcharset_cp437[203] != altcharset_cp437[209])
                    {
                        codepoint = (altcharset_cp437[203] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[203])
                            ) ? uni_cp437_halfwidth[203] : (ascii_cp437[203] & MY_A_CHARTEXT);
                        break;
                    }
                case 209:
                    if (altcharset_cp437[209] != altcharset_cp437[210])
                    {
                        codepoint = (altcharset_cp437[209] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[209])
                            ) ? uni_cp437_halfwidth[209] : (ascii_cp437[209] & MY_A_CHARTEXT);
                        break;
                    }
                case 210:
                    if (altcharset_cp437[210] != altcharset_cp437[194])
                    {
                        codepoint = (altcharset_cp437[210] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[210])
                            ) ? uni_cp437_halfwidth[210] : (ascii_cp437[210] & MY_A_CHARTEXT);
                        break;
                    }
                case 194:
                    codepoint = (altcharset_cp437[194] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[194])
                        ) ? uni_cp437_halfwidth[194] : (ascii_cp437[194] & MY_A_CHARTEXT);
                    break;
                case 213:
                    codepoint = (altcharset_cp437[194] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[194])
                        ) ? uni_cp437_halfwidth[194] : (ascii_cp437[194] & MY_A_CHARTEXT);
                    break;
                case 214:
                    codepoint = (altcharset_cp437[195] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[195])
                        ) ? uni_cp437_halfwidth[195] : (ascii_cp437[195] & MY_A_CHARTEXT);
                    break;
                case 212:
                    codepoint = (altcharset_cp437[193] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[193])
                        ) ? uni_cp437_halfwidth[193] : (ascii_cp437[193] & MY_A_CHARTEXT);
                    break;
                case 211:
                    codepoint = (altcharset_cp437[195] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[195])
                        ) ? uni_cp437_halfwidth[195] : (ascii_cp437[195] & MY_A_CHARTEXT);
                    break;
                case 184:
                    codepoint = (altcharset_cp437[194] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[194])
                        ) ? uni_cp437_halfwidth[194] : (ascii_cp437[194] & MY_A_CHARTEXT);
                    break;
                case 183:
                    codepoint = (altcharset_cp437[180] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[180])
                        ) ? uni_cp437_halfwidth[180] : (ascii_cp437[180] & MY_A_CHARTEXT);
                    break;
                case 190:
                    codepoint = (altcharset_cp437[193] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[193])
                        ) ? uni_cp437_halfwidth[193] : (ascii_cp437[193] & MY_A_CHARTEXT);
                    break;
                case 189:
                    codepoint = (altcharset_cp437[180] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[180])
                        ) ? uni_cp437_halfwidth[180] : (ascii_cp437[180] & MY_A_CHARTEXT);
                    break;
                case 205:
                    if (altcharset_cp437[205] != altcharset_cp437[196])
                    {
                        codepoint = (altcharset_cp437[205] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[205])
                            ) ? uni_cp437_halfwidth[205] : (ascii_cp437[205] & MY_A_CHARTEXT);
                        break;
                    }
                case 196:
                    codepoint = (altcharset_cp437[196] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[196])
                        ) ? uni_cp437_halfwidth[196] : (ascii_cp437[196] & MY_A_CHARTEXT);
                    break;
                case 186:
                    if (altcharset_cp437[186] != altcharset_cp437[179])
                    {
                        codepoint = (altcharset_cp437[186] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[186])
                            ) ? uni_cp437_halfwidth[186] : (ascii_cp437[186] & MY_A_CHARTEXT);
                        break;
                    }
                case 179:
                    codepoint = (altcharset_cp437[179] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[179])
                        ) ? uni_cp437_halfwidth[179] : (ascii_cp437[179] & MY_A_CHARTEXT);
                    break;
                case 206:
                    if (altcharset_cp437[206] != altcharset_cp437[215])
                    {
                        codepoint = (altcharset_cp437[206] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[206])
                            ) ? uni_cp437_halfwidth[206] : (ascii_cp437[206] & MY_A_CHARTEXT);
                        break;
                    }
                case 215:
                    if (altcharset_cp437[215] != altcharset_cp437[216])
                    {
                        codepoint = (altcharset_cp437[215] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[215])
                            ) ? uni_cp437_halfwidth[215] : (ascii_cp437[215] & MY_A_CHARTEXT);
                        break;
                    }
                case 216:
                    if (altcharset_cp437[216] != altcharset_cp437[197])
                    {
                        codepoint = (altcharset_cp437[216] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[216])
                            ) ? uni_cp437_halfwidth[216] : (ascii_cp437[216] & MY_A_CHARTEXT);
                        break;
                    }
                case 197:
                    codepoint = (altcharset_cp437[197] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[197])
                        ) ? uni_cp437_halfwidth[197] : (ascii_cp437[197] & MY_A_CHARTEXT);
                    break;
                case 15:
                    if (altcharset_cp437[15] != altcharset_cp437[176])
                    {
                        codepoint = (altcharset_cp437[15] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[15])
                            ) ? uni_cp437_halfwidth[15] : (ascii_cp437[15] & MY_A_CHARTEXT);
                        break;
                    }
                case 176:
                    if (altcharset_cp437[176] != altcharset_cp437[177])
                    {
                        codepoint = (altcharset_cp437[176] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[176])
                            ) ? uni_cp437_halfwidth[176] : (ascii_cp437[176] & MY_A_CHARTEXT);
                        break;
                    }
                case 177:
                    if (altcharset_cp437[177] != altcharset_cp437[178])
                    {
                        codepoint = (altcharset_cp437[177] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[177])
                            ) ? uni_cp437_halfwidth[177] : (ascii_cp437[177] & MY_A_CHARTEXT);
                        break;
                    }
                case 178:
                    if (altcharset_cp437[178] != altcharset_cp437[10])
                    {
                        codepoint = (altcharset_cp437[178] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[178])
                            ) ? uni_cp437_halfwidth[178] : (ascii_cp437[178] & MY_A_CHARTEXT);
                        break;
                    }
                case 10:
                    if (altcharset_cp437[10] != altcharset_cp437[219])
                    {
                        codepoint = (altcharset_cp437[10] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[10])
                            ) ? uni_cp437_halfwidth[10] : (ascii_cp437[10] & MY_A_CHARTEXT);
                        break;
                    }
                case 219:
                    codepoint = (altcharset_cp437[219] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[219])
                        ) ? uni_cp437_halfwidth[219] : (ascii_cp437[219] & MY_A_CHARTEXT);
                    break;
                case 27:
                    if (altcharset_cp437[27] != altcharset_cp437[17])
                    {
                        codepoint = (altcharset_cp437[27] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[27])
                            ) ? uni_cp437_halfwidth[27] : (ascii_cp437[27] & MY_A_CHARTEXT);
                        break;
                    }
                case 17:
                    if (altcharset_cp437[17] != altcharset_cp437[174])
                    {
                        codepoint = (altcharset_cp437[17] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[17])
                            ) ? uni_cp437_halfwidth[17] : (ascii_cp437[17] & MY_A_CHARTEXT);
                        break;
                    }
                case 174:
                    if (altcharset_cp437[174] != altcharset_cp437[243])
                    {
                        codepoint = (altcharset_cp437[174] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[174])
                            ) ? uni_cp437_halfwidth[174] : (ascii_cp437[174] & MY_A_CHARTEXT);
                        break;
                    }
                case 243:
                    codepoint = (altcharset_cp437[243] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[243])
                        ) ? uni_cp437_halfwidth[243] : (ascii_cp437[243] & MY_A_CHARTEXT);
                    break;
                case 26:
                    if (altcharset_cp437[26] != altcharset_cp437[16])
                    {
                        codepoint = (altcharset_cp437[26] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[26])
                            ) ? uni_cp437_halfwidth[26] : (ascii_cp437[26] & MY_A_CHARTEXT);
                        break;
                    }
                case 16:
                    if (altcharset_cp437[16] != altcharset_cp437[175])
                    {
                        codepoint = (altcharset_cp437[16] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[16])
                            ) ? uni_cp437_halfwidth[16] : (ascii_cp437[16] & MY_A_CHARTEXT);
                        break;
                    }
                case 175:
                    if (altcharset_cp437[175] != altcharset_cp437[242])
                    {
                        codepoint = (altcharset_cp437[175] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[175])
                            ) ? uni_cp437_halfwidth[175] : (ascii_cp437[175] & MY_A_CHARTEXT);
                        break;
                    }
                case 242:
                    codepoint = (altcharset_cp437[242] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[242])
                        ) ? uni_cp437_halfwidth[242] : (ascii_cp437[242] & MY_A_CHARTEXT);
                    break;
                case 7:
                    if (altcharset_cp437[7] != altcharset_cp437[9])
                    {
                        codepoint = (altcharset_cp437[7] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[7])
                            ) ? uni_cp437_halfwidth[7] : (ascii_cp437[7] & MY_A_CHARTEXT);
                        break;
                    }
                case 9:
                    if (altcharset_cp437[9] != altcharset_cp437[254])
                    {
                        codepoint = (altcharset_cp437[9] !=
                                     SNAPSHOT_ADDCH__NARROWC(ascii_cp437[9])
                            ) ? uni_cp437_halfwidth[9] : (ascii_cp437[9] & MY_A_CHARTEXT);
                        break;
                    }
                case 8:
                    codepoint = (altcharset_cp437[8] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[8])
                        ) ? uni_cp437_halfwidth[8] : (ascii_cp437[8] & MY_A_CHARTEXT);
                    break;
                case 4:
                case 25:
                case 31:
                case 24:
                case 30:
                case 248:
                case 241:
                case 227:
                case 156:
                case 249:
                case 250:
                case 254:
                    codepoint = (altcharset_cp437[inbyte] !=
                                 SNAPSHOT_ADDCH__NARROWC(ascii_cp437[inbyte])
                        ) ? uni_cp437_halfwidth[inbyte] : (ascii_cp437[inbyte] & MY_A_CHARTEXT);
                    break;
                default:
                    inbyte = (int) (unsigned char)
                        (ascii_cp437[inbyte] & MY_A_CHARTEXT);
                    codepoint = inbyte;
                }
            }
        }
        else
        {
            inbyte = (int) (unsigned char)
                (ascii_cp437[inbyte] & MY_A_CHARTEXT);
            codepoint = inbyte;
        }
        if (snapshot)
        {
            snapshot_attrset_active(snapshot_attrs);
            if (codepoint == '&')
            {
                fprintf(snapshot,
                        "&amp;");
            }
            else if (codepoint == '<')
            {
                fprintf(snapshot,
                        "&lt;");
            }
            else if (codepoint == '>')
            {
                fprintf(snapshot,
                        "&gt;");
            }
            else if (codepoint == '\"')
            {
                fprintf(snapshot,
                        "&quot;");
            }
            else if (use_acs && use_raw && ! use_raw_ucs)
            {
                fprintf(snapshot,
                        "%c",
                        (char) inbyte);
            }
            else if ((codepoint >= 0x20) && (codepoint <= 0x7E))
            {
                fprintf(snapshot,
                        "%c",
                        (char) codepoint);
            }
            else
            {
                fprintf(snapshot,
                        "&#%lu;",
                        codepoint);
            }
            fflush(snapshot);
        }
        if (snapshot_txt)
        {
#if USE_ATTR
#ifdef MY_A_BOLD
            if (snapshot_attrs_active & MY_A_BOLD)
            {
#ifdef MY_A_UNDERLINE
                if (snapshot_attrs_active & MY_A_UNDERLINE)
                {
                    fputs("_\b", snapshot_txt);
                }
#endif
                fputc_utf8(codepoint, snapshot_txt);
                fputc('\b', snapshot_txt);
            }
#endif
#ifdef MY_A_UNDERLINE
            if (snapshot_attrs_active & MY_A_UNDERLINE)
            {
                fputs("_\b", snapshot_txt);
            }
#endif
#endif
            fputc_utf8(codepoint, snapshot_txt);
            fflush(snapshot_txt);
        }
        snapshot_x ++;
    }
}

int my_addch(unsigned long b, chtype attrs)
{
    int ret = 0;
    chtype c = '\?';
    int old_y, old_x;
    int new_y, new_x;

    if (! b) b = ' ';
    getyx(stdscr, old_y, old_x);
    if ((old_y == last_valid_line)
        &&
        (old_x == (last_valid_col + 1)))
    {
        last_valid_col += CJK_MODE ? 2 : 1;
    }
    my_attrset(attrs);
    snapshot_addch(b);
    if (CJK_MODE && ! (use_acs && use_raw && use_raw_ucs))
    {
        unsigned char rhs;

        rhs = cp437_fullwidth_rhs[b];
        if ((int) (unsigned char) rhs)
        {
            snapshot_addch(rhs);
        }
    }
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
    my_real_attrset(my_attrs);
#endif
#endif
    do
    {
        if (use_acs && use_raw && ! use_raw_ucs)
        {
            char buf[2];
            buf[0] = (char) (unsigned char) (b & 0xFF);
            buf[1] = '\0';
            ret = addstr(buf);
            getyx(stdscr, new_y, new_x);
            if ((old_x != new_x) || (old_y != new_y))
            {
                if (CJK_MODE && ((new_x % COLS) != ((old_x + 2) % COLS)))
                {
                    unsigned char rhs;

                    rhs = cp437_fullwidth_rhs[b];
                    if ((int) (unsigned char) rhs)
                    {
                        buf[0] = (char) (unsigned char) (0xFFU & (unsigned) rhs);
                        addstr(buf);
                    }
                }
                break;
            }
        }
        if (b <= 0xFF)
        {
            if (use_acs)
            {
#if USE_WIDEC_SUPPORT
                if (use_raw && use_raw_ucs)
                {
                    unsigned long my_ucs;
                    wchar_t my_wch;
                    int my_wcw;

                    my_ucs = uni_cp437[b];
                    my_wch = ucs_to_wchar(my_ucs);
                    my_wcw = my_wch ? my_wcwidth(my_wch) : 0;
                    if ((my_wcw > 0) && (my_wcw <= ((CJK_MODE) ? 2 : 1)))
                    {
                        ret = addnwstr(&my_wch, 1);
                        getyx(stdscr, new_y, new_x);
                        if ((old_x != new_x) || (old_y != new_y))
                        {
                            if (CJK_MODE && (my_wcw == 1))
                            {
                                unsigned char rhs;

                                rhs = cp437_fullwidth_rhs[b];
                                if ((int) (unsigned char) rhs)
                                {
                                    wchar_t wrhs;

                                    wrhs = ucs_to_wchar(uni_cp437_fullwidth[(int) (unsigned char) rhs]);
                                    my_wcw += wrhs ? my_wcwidth(wrhs) : 0;
                                    if (my_wcw == 2)
                                    {
                                        addnwstr(&wrhs, 1);
                                    }
                                }
                            }
                            else if (CJK_MODE && (my_wcw == 2)
                                     &&
                                     (((old_x + 1) % COLS) == (new_x % COLS)))
                            {
                                location_is_suspect = 1;
                                leaveok(stdscr, FALSE);
                                move(0, 0);
                                refresh();
                                leaveok(stdscr, TRUE);
                                move((old_y + ((old_x + 2) / COLS)) % LINES, (old_x + 2) % COLS);
                            }
                            break;
                        }
                        if (my_ucs == 0x30fb)
                        {
                            my_wch = ucs_to_wchar(0xff0e);
                            my_wcw = my_wch ? my_wcwidth(my_wch) : 0;
                            ret = addnwstr(&my_wch, 1);
                            getyx(stdscr, new_y, new_x);
                            if ((old_x != new_x) || (old_y != new_y))
                            {
                                if (CJK_MODE && (my_wcw == 2)
                                    &&
                                    (((old_x + 1) % COLS) == (new_x % COLS)))
                                {
                                }
                                break;
                            }
                        }
                        if (my_ucs == 0x301c)
                        {
                            my_wch = ucs_to_wchar(0xff5e);
                            my_wcw = my_wch ? my_wcwidth(my_wch) : 0;
                            ret = addnwstr(&my_wch, 1);
                            getyx(stdscr, new_y, new_x);
                            if ((old_x != new_x) || (old_y != new_y))
                            {
                                if (CJK_MODE && (my_wcw == 2)
                                    &&
                                    (((old_x + 1) % COLS) == (new_x % COLS)))
                                {
                                }
                                break;
                            }
                        }
                    }
                }
                if (altcharset_cp437[b])
                {
                    wchar_t my_wchbuf[CCHARW_MAX];
                    attr_t my_acs_attrs;
                    short my_color_pair;
                    attr_t my_current_attrs;
                    int my_len;

                    my_len =
                        getcchar(
                            MY_WACS_PTR altcharset_cp437[b],
                            NULL,
                            &my_acs_attrs,
                            &my_color_pair,
                            NULL);
                    if (my_len &&
                        (getcchar(
                            MY_WACS_PTR altcharset_cp437[b],
                            my_wchbuf,
                            &my_acs_attrs,
                            &my_color_pair,
                            NULL) != ERR) &&
                        (my_wcswidth(my_wchbuf, my_len) == 1))
                    {
#ifdef _XOPEN_SOURCE_EXTENDED
                        attr_get(
                            & my_current_attrs,
                            & my_color_pair,
                            NULL);
                        attr_set(
                            my_current_attrs | my_acs_attrs,
                            my_color_pair,
                            NULL);
#else
                        my_current_attrs = attr_get();
                        attr_set(my_current_attrs | my_acs_attrs);
#endif
                        ret = addnwstr(
                            my_wchbuf,
                            my_len);
                        getyx(stdscr, new_y, new_x);
                        if (CJK_MODE && ((old_x != new_x) || (old_y != new_y)))
                        {
                            unsigned char rhs;
                        
                            rhs = cp437_fullwidth_rhs[b];
                            if ((int) (unsigned char) rhs)
                            {
                                if (altcharset_cp437[(int) (unsigned char) rhs])
                                {
                                    my_len =
                                        getcchar(
                                            MY_WACS_PTR altcharset_cp437[(int) (unsigned char) rhs],
                                            NULL,
                                            &my_acs_attrs,
                                            &my_color_pair,
                                            NULL);
                                    if (my_len &&
                                        (getcchar(
                                            MY_WACS_PTR altcharset_cp437[(int) (unsigned char) rhs],
                                            my_wchbuf,
                                            &my_acs_attrs,
                                            &my_color_pair,
                                            NULL) != ERR) &&
                                        (my_wcswidth(my_wchbuf, my_len) == 1))
                                    {
                                        addnwstr(
                                            my_wchbuf,
                                            my_len);
                                    }
                                }
                                else
                                {
                                    addch(ascii_cp437[(int) (unsigned char) rhs]);
                                }
                            }
                        }
#ifdef _XOPEN_SOURCE_EXTENDED
                        attr_set(
                            my_current_attrs,
                            my_color_pair,
                            NULL);
#else
                        attr_set(my_current_attrs);
#endif
                        if ((old_x != new_x) || (old_y != new_y))
                        {
                            break;
                        }
                    }
                }
#else
                if (use_raw && use_raw_ucs)
                {
                    c = uni_cp437[b];
                    ret = addch(c);
                    getyx(stdscr, new_y, new_x);
                    if ((old_x != new_x) || (old_y != new_y))
                    {
#if USE_WCWIDTH
                        if (CJK_MODE)
                        {
                            wchar_t my_wch;

                            my_wch = ucs_to_wchar(c);
                            if (my_wch && (my_wcwidth(my_wch) < 2))
                            {
                                unsigned char rhs;
                    
                                rhs = cp437_fullwidth_rhs[b];
                                if ((int) (unsigned char) rhs)
                                {
                                    c = uni_cp437[(int) (unsigned char) rhs];
                                    my_wch = ucs_to_wchar(c);
                                    getyx(stdscr, old_y, old_x);
                                    if (my_wch && (my_wcwidth(my_wch) < 2))
                                    {
                                        addch(c);
                                        getyx(stdscr, new_y, new_x);
                                        if ((old_x != new_x) || (old_y != new_y))
                                        {
                                            break;
                                        }
                                    }
                                    c = altcharset_cp437[(int) (unsigned char) rhs];
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                                    if (c & ~A_CHARTEXT)
                                    {
                                        my_attrset(attrs);
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
                                        my_real_attrset(my_attrs);
#endif
#endif
                                    }
#endif
#endif
                                    addch(c);
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                                    if (c & ~A_CHARTEXT)
                                    {
                                        my_attrset(attrs);
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
                                        my_real_attrset(my_attrs);
#endif
#endif
                                    }
#endif
#endif
                                    getyx(stdscr, new_y, new_x);
                                    if ((old_x != new_x) || (old_y != new_y))
                                    {
                                        break;
                                    }
                                    addch(ascii_cp437[(int) (unsigned char) rhs]);
                                }
                            }
                        }
#endif
                        break;
                    }
                }
                c = altcharset_cp437[b];
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                if (c & ~A_CHARTEXT)
                {
                    my_attrset(attrs | (c & ~A_CHARTEXT));
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
                    my_real_attrset(my_attrs);
#endif
#endif
                }
#endif
#endif
                ret = addch(c);
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                if (c & ~A_CHARTEXT)
                {
                    my_attrset(attrs);
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
                    my_real_attrset(my_attrs);
#endif
#endif
                }
#endif
#endif
                getyx(stdscr, new_y, new_x);
                if ((old_x != new_x) || (old_y != new_y))
                {
                    if (CJK_MODE && ((new_x % COLS) != ((old_x + 2) % COLS)))
                    {
                        unsigned char rhs;
                    
                        rhs = (unsigned char) (unsigned) (chtype) cp437_fullwidth_rhs[b];
                        if ((int) (unsigned char) rhs)
                        {
                            c = altcharset_cp437[(int) (unsigned char) rhs];
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                            if (c & ~A_CHARTEXT)
                            {
                                my_attrset(attrs | (c & ~A_CHARTEXT));
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
                                my_real_attrset(my_attrs);
#endif
#endif
                            }
#endif
#endif
                            addch(c);
#if USE_A_CHARTEXT
#ifdef A_CHARTEXT
                            if (c & ~A_CHARTEXT)
                            {
                                my_attrset(attrs);
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
                                my_real_attrset(my_attrs);
#endif
#endif
                            }
#endif
#endif
                        }
                    }
                    break;
                }
#endif
            }
        }
        c = ascii_cp437[b];
        getyx(stdscr, old_y, old_x);
        ret = addch(c);
        getyx(stdscr, new_y, new_x);
        if (CJK_MODE && ((new_x % COLS) != ((old_x + 2) % COLS)))
        {
            unsigned char rhs;

            rhs = cp437_fullwidth_rhs[b];
            if ((int) (unsigned char) rhs)
            {
                addch(ascii_cp437[(int) (unsigned char) rhs]);
            }
        }
    }
    while (0);
#if USE_ATTR || USE_COLOR
#if DANGEROUS_ATTRS
    if (my_attrs) my_real_attrset(0);
#endif
#endif
    return ret;
}
