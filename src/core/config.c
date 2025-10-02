/*
 * config.c - Configuration management implementation
 * 
 * New module created during Phase 2 refactoring.
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

#include "config.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

#ifdef CODESET
#include <langinfo.h>
#endif

#if HAVE_ICONV_H
#include <iconv.h>
#endif

#ifdef LC_CTYPE
#ifndef uint32_t
#if HAVE_STDINT_H
#include <stdint.h>
#endif
#endif
#endif

/* myman_getenv is in utils.c - we need to declare it here */
extern char *myman_getenv(const char *name);

/* ascii_cp437 array is in myman.c - we need to declare it here */
extern chtype ascii_cp437[256];

/* iconv descriptors for ucs_to_wchar - static state */
#if HAVE_ICONV_H
static iconv_t cd_to_wchar = (iconv_t) -1;
static iconv_t cd_to_uni = (iconv_t) -1;
#endif

/* Extracted from myman.c line 829 */
int locale_is_utf8(void)
{
    const char *my_locale;
    char *my_locale_lower;
    int is_utf8 = 0;
    int i;

#ifdef LC_CTYPE
    my_locale = setlocale(LC_CTYPE, "");
    if (my_locale)
    {
        my_locale_lower = strdup(my_locale);
        if (my_locale_lower)
        {
            for (i = 0; my_locale_lower[i]; i ++)
            {
                my_locale_lower[i] = tolower(my_locale_lower[i]);
            }
            if (strstr(my_locale_lower, "utf8")
                ||
                strstr(my_locale_lower, "utf-8"))
            {
                is_utf8 = 1;
            }
            free((void *) my_locale_lower);
            my_locale_lower = NULL;
        }
#ifdef CODESET
        {
            char *codeset, *codeset_lower;

            codeset = nl_langinfo(CODESET);
            if (codeset)
            {
                codeset_lower = strdup(codeset);
                if (codeset_lower)
                {
                    for (i = 0; codeset_lower[i]; i ++)
                    {
                        codeset_lower[i] = tolower(codeset_lower[i]);
                    }
                    if ((! strcmp(codeset_lower, "utf8"))
                        ||
                        (! strcmp(codeset_lower, "utf-8")))
                    {
                        is_utf8 = 1;
                    }
                    free((void *) codeset_lower);
                    codeset_lower = NULL;
                }
            }
        }
#endif /* defined(CODESET) */
        setlocale(LC_CTYPE, my_locale);
    }
#endif /* defined(LC_CTYPE) */
    /* for broken systems that do not yet support UTF-8 locales
     * (Cygwin comes to mind) */
    my_locale = myman_getenv("LC_CTYPE");
    if (! my_locale) my_locale = myman_getenv("LC_ALL");
    if (! my_locale) my_locale = myman_getenv("LANG");
    if (my_locale)
    {
        my_locale_lower = strdup(my_locale);
        if (my_locale_lower)
        {
            for (i = 0; my_locale_lower[i]; i ++)
            {
                my_locale_lower[i] = tolower(my_locale_lower[i]);
            }
            if (strstr(my_locale_lower, "utf8")
                ||
                strstr(my_locale_lower, "utf-8"))
            {
                is_utf8 = 1;
            }
            free((void *) my_locale_lower);
            my_locale_lower = NULL;
        }
    }
    return is_utf8;
}

/* Extracted from myman.c line 1905 */
chtype cp437_to_ascii(unsigned char ch)
{
    return ascii_cp437[(ch & 0xff)];
}

/* Extracted from myman.c line 629 */
#if HAVE_ICONV_H
wchar_t ucs_to_wchar(unsigned long ucs)
{
    wchar_t wcbuf[2];
#ifdef LC_CTYPE
    uint32_t ucsbuf[2];
    const char *ibuf;
    char *obuf;
    size_t ibl;
    size_t obl;
    const char *my_locale;

    do
    {
        if ((! (my_locale = setlocale(LC_CTYPE, "")))
            ||
            (! *my_locale)
            ||
            (! strcmp(my_locale, "C"))
            ||
            (! strcmp(my_locale, "POSIX")))
        {
            wcbuf[0] = 0;
            break;
        }
        if ((cd_to_wchar == (iconv_t) -1)
            &&
            ((cd_to_wchar = iconv_open("wchar_t//IGNORE", "UCS-4-INTERNAL")) == (iconv_t) -1))
        {
            wcbuf[0] = 0;
            break;
        }
        ucsbuf[0] = ucs;
        ucsbuf[1] = 0;
        wcbuf[0] = 0;
        wcbuf[1] = 0;
        ibuf = (char *) (void *) ucsbuf;
        obuf = (char *) (void *) wcbuf;
        ibl = sizeof(ucsbuf);
        obl = sizeof(wcbuf);
        if ((! iconv(cd_to_wchar, &ibuf, &ibl, &obuf, &obl))
            ||
            wcbuf[1]
            ||
            (! wcbuf[0]))
        {
            wcbuf[0] = 0;
            break;
        }
        if (cd_to_uni == (iconv_t) -1)
        {
            cd_to_uni = iconv_open("UCS-4-INTERNAL//IGNORE", "wchar_t");
        }
        ucsbuf[0] = 0;
        ibuf = (char *) (void *) wcbuf;
        obuf = (char *) (void *) ucsbuf;
        ibl = sizeof(wcbuf);
        obl = sizeof(ucsbuf);
        if ((cd_to_uni != (iconv_t) -1)
            &&
            (iconv(cd_to_uni, &ibuf, &ibl, &obuf, &obl))
            &&
            (ucsbuf[0] != ucs))
        {
            /* does not round-trip, probably a broken character */
            wcbuf[0] = 0;
            break;
        }
    }
    while (0);
    if (my_locale)
    {
        setlocale(LC_CTYPE, my_locale);
    }
#else /* ! defined(LC_CTYPE) */
    wcbuf[0] = 0;
#endif /* ! defined(LC_CTYPE) */
    return wcbuf[0] ? wcbuf[0] : (((ucs >= 0x20) && (ucs <= 0x7e)) ? ((wchar_t) ucs) : 0);
}
#else
/* No iconv support - use simple macro */
#define ucs_to_wchar(ucs) ((((unsigned long) (wchar_t) (unsigned long) (ucs)) == ((unsigned long) (ucs))) ? ((wchar_t) (unsigned long) (ucs)) : ((wchar_t) 0))
#endif
