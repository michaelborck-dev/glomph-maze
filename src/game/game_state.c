/* game_state.c - Game state management functions
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

/**
 * @brief Display maze/tile/sprite metadata information
 *
 * Collects ABOUT, FIXME, and NOTE messages from maze, tile, and sprite
 * files and displays them as a pager notice. Used for showing level
 * information and developer notes embedded in asset files.
 *
 * @note Allocates tmp_notice buffer (freed on next call)
 * @note Sets pager_notice and reinit_requested globals
 */
void gameinfo(void) {
    if (maze_ABOUT || maze_FIXME || maze_NOTE || tile_ABOUT || tile_FIXME ||
        tile_NOTE || sprite_ABOUT || sprite_FIXME || sprite_NOTE) {
        if (tmp_notice) {
            free((void*)tmp_notice);
            tmp_notice = 0;
        }
        tmp_notice = (char*)malloc(
            ((maze_ABOUT
                  ? ((maze_ABOUT_prefix ? strlen(maze_ABOUT_prefix) : 0) +
                     strlen(maze_ABOUT))
                  : 0) +
             (maze_FIXME
                  ? ((maze_FIXME_prefix ? strlen(maze_FIXME_prefix) : 0) +
                     strlen(maze_FIXME))
                  : 0) +
             (maze_NOTE ? ((maze_NOTE_prefix ? strlen(maze_NOTE_prefix) : 0) +
                           strlen(maze_NOTE))
                        : 0) +
             (tile_ABOUT
                  ? ((tile_ABOUT_prefix ? strlen(tile_ABOUT_prefix) : 0) +
                     strlen(tile_ABOUT))
                  : 0) +
             (tile_FIXME
                  ? ((tile_FIXME_prefix ? strlen(tile_FIXME_prefix) : 0) +
                     strlen(tile_FIXME))
                  : 0) +
             (tile_NOTE ? ((tile_NOTE_prefix ? strlen(tile_NOTE_prefix) : 0) +
                           strlen(tile_NOTE))
                        : 0) +
             (sprite_ABOUT
                  ? ((sprite_ABOUT_prefix ? strlen(sprite_ABOUT_prefix) : 0) +
                     strlen(sprite_ABOUT))
                  : 0) +
             (sprite_FIXME
                  ? ((sprite_FIXME_prefix ? strlen(sprite_FIXME_prefix) : 0) +
                     strlen(sprite_FIXME))
                  : 0) +
             (sprite_NOTE
                  ? ((sprite_NOTE_prefix ? strlen(sprite_NOTE_prefix) : 0) +
                     strlen(sprite_NOTE))
                  : 0)) +
            1 + 1);
        if (tmp_notice) {
            *tmp_notice = '\0';
            if (maze_ABOUT)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (maze_ABOUT_prefix ? maze_ABOUT_prefix : ""),
                        maze_ABOUT);
            if (maze_FIXME)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (maze_FIXME_prefix ? maze_FIXME_prefix : ""),
                        maze_FIXME);
            if (maze_NOTE)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (maze_NOTE_prefix ? maze_NOTE_prefix : ""), maze_NOTE);
            if (tile_ABOUT)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (tile_ABOUT_prefix ? tile_ABOUT_prefix : ""),
                        tile_ABOUT);
            if (tile_FIXME)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (tile_FIXME_prefix ? tile_FIXME_prefix : ""),
                        tile_FIXME);
            if (tile_NOTE)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (tile_NOTE_prefix ? tile_NOTE_prefix : ""), tile_NOTE);
            if (sprite_ABOUT)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (sprite_ABOUT_prefix ? sprite_ABOUT_prefix : ""),
                        sprite_ABOUT);
            if (sprite_FIXME)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (sprite_FIXME_prefix ? sprite_FIXME_prefix : ""),
                        sprite_FIXME);
            if (sprite_NOTE)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (sprite_NOTE_prefix ? sprite_NOTE_prefix : ""),
                        sprite_NOTE);
            sprintf(tmp_notice + strlen(tmp_notice), "\n");
            pager_notice     = tmp_notice;
            reinit_requested = 1;
        }
    }
}

/**
 * @brief Display game help and control information
 *
 * Shows keyboard controls (MYMANKEYS) combined with asset metadata.
 * Creates a pager notice with control scheme and game information.
 *
 * @note Allocates tmp_notice buffer (freed on next call)
 * @note Sets pager_notice and reinit_requested globals
 * @see gameinfo
 */
void gamehelp(void) {
    if (MYMANKEYS || maze_ABOUT || maze_FIXME || maze_NOTE || tile_ABOUT ||
        tile_FIXME || tile_NOTE || sprite_ABOUT || sprite_FIXME ||
        sprite_NOTE) {
        if (tmp_notice) {
            free((void*)tmp_notice);
            tmp_notice = 0;
        }
        tmp_notice = (char*)malloc(
            ((MYMANKEYS ? ((MYMANKEYS_prefix ? strlen(MYMANKEYS_prefix) : 0) +
                           strlen(MYMANKEYS))
                        : 0) +
             (maze_ABOUT
                  ? ((maze_ABOUT_prefix ? strlen(maze_ABOUT_prefix) : 0) +
                     strlen(maze_ABOUT))
                  : 0) +
             (maze_FIXME
                  ? ((maze_FIXME_prefix ? strlen(maze_FIXME_prefix) : 0) +
                     strlen(maze_FIXME))
                  : 0) +
             (maze_NOTE ? ((maze_NOTE_prefix ? strlen(maze_NOTE_prefix) : 0) +
                           strlen(maze_NOTE))
                        : 0) +
             (tile_ABOUT
                  ? ((tile_ABOUT_prefix ? strlen(tile_ABOUT_prefix) : 0) +
                     strlen(tile_ABOUT))
                  : 0) +
             (tile_FIXME
                  ? ((tile_FIXME_prefix ? strlen(tile_FIXME_prefix) : 0) +
                     strlen(tile_FIXME))
                  : 0) +
             (tile_NOTE ? ((tile_NOTE_prefix ? strlen(tile_NOTE_prefix) : 0) +
                           strlen(tile_NOTE))
                        : 0) +
             (sprite_ABOUT
                  ? ((sprite_ABOUT_prefix ? strlen(sprite_ABOUT_prefix) : 0) +
                     strlen(sprite_ABOUT))
                  : 0) +
             (sprite_FIXME
                  ? ((sprite_FIXME_prefix ? strlen(sprite_FIXME_prefix) : 0) +
                     strlen(sprite_FIXME))
                  : 0) +
             (sprite_NOTE
                  ? ((sprite_NOTE_prefix ? strlen(sprite_NOTE_prefix) : 0) +
                     strlen(sprite_NOTE))
                  : 0)) +
            1 + 1);
        if (tmp_notice) {
            *tmp_notice = '\0';
            if (MYMANKEYS)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (MYMANKEYS_prefix ? MYMANKEYS_prefix : ""), MYMANKEYS);
            if (maze_ABOUT)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (maze_ABOUT_prefix ? maze_ABOUT_prefix : ""),
                        maze_ABOUT);
            if (maze_FIXME)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (maze_FIXME_prefix ? maze_FIXME_prefix : ""),
                        maze_FIXME);
            if (maze_NOTE)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (maze_NOTE_prefix ? maze_NOTE_prefix : ""), maze_NOTE);
            if (tile_ABOUT)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (tile_ABOUT_prefix ? tile_ABOUT_prefix : ""),
                        tile_ABOUT);
            if (tile_FIXME)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (tile_FIXME_prefix ? tile_FIXME_prefix : ""),
                        tile_FIXME);
            if (tile_NOTE)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (tile_NOTE_prefix ? tile_NOTE_prefix : ""), tile_NOTE);
            if (sprite_ABOUT)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (sprite_ABOUT_prefix ? sprite_ABOUT_prefix : ""),
                        sprite_ABOUT);
            if (sprite_FIXME)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (sprite_FIXME_prefix ? sprite_FIXME_prefix : ""),
                        sprite_FIXME);
            if (sprite_NOTE)
                sprintf(tmp_notice + strlen(tmp_notice), "%s%s",
                        (sprite_NOTE_prefix ? sprite_NOTE_prefix : ""),
                        sprite_NOTE);
            sprintf(tmp_notice + strlen(tmp_notice), "\n");
            pager_notice     = tmp_notice;
            reinit_requested = 1;
        }
    }
}

/**
 * @brief Initialize a new game session
 *
 * Resets all game state for a fresh game: level counter, score, lives,
 * maze state, and sprite positions. Called when player starts a new game
 * from the title screen or after game over.
 *
 * @note Sets player = 1, lives = LIVES, score = 0
 * @note Clears maze and resets all timers
 * @see gamereset, gameintro
 */
void gamestart(void) {
    level              = 0;
    maze_level         = 0;
    intermission       = 0;
    intermission_shown = 0;
    maze_erase();
    ghost_eaten_timer = 0;
    winning           = 1;
    oldplayer         = 0;
    player            = 1;
    pellet_timer      = 0;
    pellet_time       = PELLET_ADJUST(7 * ONESEC);
    cycles            = 0;
    score             = 0;
    dots              = 0;
    dead              = 0;
    deadpan           = 0;
    dying             = 0;
    myman_start       = 0;
}

/**
 * @brief Reset game state for a new level or life
 *
 * Initializes all sprite registers, positions, and states for the hero and
 * ghosts. Sets up fruit/bonus items, resets pellet timer, and configures
 * sprite colors based on ghost personalities.
 *
 * @note Modifies global sprite_register arrays and ghost state
 * @see gamestart, gameintermission
 */
void gamereset(void) {
    int i;

    pellet_time                  = PELLET_ADJUST(7 * ONESEC);
    sprite_register[FRUIT]       = SPRITE_FRUIT;
    sprite_register[FRUIT_SCORE] = SPRITE_FRUIT_SCORE;
    sprite_register_frame[FRUIT] = sprite_register_frame[FRUIT_SCORE] =
        BONUS(level);
    sprite_register_x[FRUIT] = sprite_register_x[FRUIT_SCORE] = (int)XFRUIT;
    sprite_register_y[FRUIT] = sprite_register_y[FRUIT_SCORE] = (int)YFRUIT;

    sprite_register[GHOST_SCORE]       = SPRITE_200;
    sprite_register_frame[GHOST_SCORE] = 0;

    hero_dir                    = dirhero;
    sprite_register[HERO]       = SPRITE_HERO + ((hero_dir == MYMAN_LEFT) ? 4
                                                 : (hero_dir == MYMAN_RIGHT) ? 12
                                                 : (hero_dir == MYMAN_DOWN) ? 16
                                                                            : 0);
    sprite_register_frame[HERO] = 0;
    sprite_register_x[HERO]     = (int)XHERO;
    sprite_register_y[HERO]     = (int)YHERO;
    sprite_register_used[HERO]  = 0;

    sprite_register_color[HERO]       = 0xE;
    sprite_register_color[BIGHERO_UL] = 0xE;
    sprite_register_color[BIGHERO_UR] = 0xE;
    sprite_register_color[BIGHERO_LL] = 0xE;
    sprite_register_color[BIGHERO_LR] = 0xE;

    for (i = 0; i < MAXGHOSTS; i++) {
        int         eyes, mean, blue;
        const char* extra_ghost_colors = EXTRA_GHOST_COLORS;

        eyes                            = GHOSTEYES(i);
        mean                            = MEANGHOST(i);
        blue                            = BLUEGHOST(i);
        sprite_register[eyes]           = SPRITE_EYES;
        sprite_register[mean]           = SPRITE_MEAN;
        sprite_register[blue]           = SPRITE_BLUE;
        sprite_register_used[eyes]      = sprite_register_used[mean] =
            sprite_register_used[blue]  = 0;
        sprite_register_frame[eyes]     = sprite_register_frame[mean] =
            sprite_register_frame[blue] = 0;
        ghost_mem[i]                    = 0;
        ghost_timer[i]                  = TWOSECS;
        ghost_man[i]                    = 0;
        sprite_register_color[eyes]     = 0xF;
        sprite_register_color[blue]     = 0x9;
        sprite_register_color[mean] =
            extra_ghost_colors[(i % strlen(extra_ghost_colors))];
    }

    if (GHOST0 < MAXGHOSTS)
        sprite_register_color[MEANGHOST(GHOST0)] = 0xB;
    if (GHOST1 < MAXGHOSTS)
        sprite_register_color[MEANGHOST(GHOST1)] = 0xC;
    if (GHOST2 < MAXGHOSTS)
        sprite_register_color[MEANGHOST(GHOST2)] = 0xD;
    if (GHOST3 < MAXGHOSTS)
        sprite_register_color[MEANGHOST(GHOST3)] = 0x6;
}

/**
 * @brief Display credit screen with game info and bonus text
 *
 * Shows copyright notice, bonus life threshold, credit information,
 * and "PUSH START BUTTON" message. Optionally displays debug character
 * grid when debug mode is enabled.
 *
 * @note Positions text based on maze dimensions for proper centering
 */
void creditscreen(void) {
    int s;
    int r_off;

    r_off = maze_h - 16;
    if (r_off < 0) {
        r_off = 0;
    }
    maze_puts(r_off + 12, ((int)maze_w - (int)strlen(MYMANNOTICE)) / 2, 0xD,
              MYMANNOTICE);
    maze_puts(r_off + 8, ((int)maze_w - (int)strlen(BONUSHEROTEXT)) / 2, 0x7,
              BONUSHEROTEXT);
    maze_puts(r_off + 4, ((int)maze_w - (int)strlen(CREDIT(1))) / 2, MSG2_COLOR,
              CREDIT(1));
    maze_puts(r_off, ((int)maze_w - (int)strlen(START)) / 2, 0x6, START);
    if (debug) {
        for (s = 0; s < 256; s++) {
            if ((s / 16) < maze_h)
                if ((s % 16) < maze_w)
                    maze_puts(maze_h - 1 - (s / 16), s % 16, s, "*");
        }
    }
    sprite_register_x[HERO] = maze_w * gfx_w / 2;
    sprite_register_y[HERO] = r_off * gfx_h;
}

/**
 * @brief Run attract mode demo gameplay
 *
 * Automated demo mode showing gameplay when no one is playing. Initializes
 * game state, sets up automatic navigation for hero, and runs simplified
 * game logic without score tracking. Hero follows walls autonomously using
 * home_dir pathfinding.
 *
 * @note Sets myman_demo flag and player = 1 (no scoring)
 * @note Cycles through levels automatically for variety
 * @see gamestart, find_home_dir
 */
void gamedemo(void) {
    int s;
    int xtile, ytile;
    int x_off, y_off;

    xtile = XTILE(sprite_register_x[HERO]);
    ytile = YTILE(sprite_register_y[HERO]);
    x_off = sprite_register_x[HERO] % gfx_w;
    y_off = sprite_register_y[HERO] % gfx_h;
    if ((myman_demo == 1) && (!myman_demo_setup)) {
        level              = 0;
        maze_level         = 0;
        intermission       = 0;
        intermission_shown = 0;
        for (s = 0; s < SPRITE_REGISTERS; s++) {
            sprite_register_used[s]  = 0;
            sprite_register_timer[s] = 0;
            sprite_register_frame[s] = 0;
        }
        maze_erase();
        ghost_eaten_timer = 0;
        winning           = 1;
        oldplayer         = 0;
        player            = 1;
        pellet_timer      = 0;
        pellet_time       = PELLET_ADJUST(7 * ONESEC);
        for (s = 0; s < frames % 8; s++) {
            pellet_time -= PELLET_ADJUST(7 * ONESEC);
            if (level && (FLIP_ALWAYS || INTERMISSION(level))) {
                ++maze_level;
                maze_level %= maze_n;
                if (!maze_level) {
                    maze_level = flip_to % maze_n;
                }
                if (FLIP_LOCK && !maze_level) {
                    maze_level = maze_n - 1;
                }
            }
            ++level;
            sprite_register_frame[FRUIT] = sprite_register_frame[FRUIT_SCORE] =
                BONUS(level);
            pellet_time += PELLET_ADJUST(7 * ONESEC);
            if (pellet_time > PELLET_ADJUST(ONESEC))
                pellet_time -= PELLET_ADJUST(ONESEC);
            else
                pellet_time = 0;
        }
        cycles  = 0;
        dots    = 0;
        dead    = 0;
        deadpan = 0;
        dying   = 0;
        myman_demo_setup =
            1 + (15UL * (maze_h * maze_w) * TWOSECS / (28 * 31)) / 2;
    }
    if (!(winning || dying || (dead && !ghost_eaten_timer))) {
        if (!(frames % ((TWOSECS / 20) + 1))) {
            unsigned char mleft, mdown, mright, mup;
            mleft = (unsigned)(unsigned char)
                maze[(maze_level * maze_h + ytile) * (maze_w + 1) +
                     XWRAP(xtile - NOTRIGHT(x_off))];
            mdown = (unsigned)(unsigned char)
                maze[(maze_level * maze_h + YWRAP(ytile + NOTTOP(y_off))) *
                         (maze_w + 1) +
                     xtile];
            mright = (unsigned)(unsigned char)
                maze[(maze_level * maze_h + ytile) * (maze_w + 1) +
                     XWRAP(xtile + NOTLEFT(x_off))];
            mup = (unsigned)(unsigned char)
                maze[(maze_level * maze_h + YWRAP(ytile - NOTBOTTOM(y_off))) *
                         (maze_w + 1) +
                     xtile];
            if (ISOPEN((unsigned)mleft) && ISPELLET((unsigned)mleft)) {
                hero_dir              = MYMAN_LEFT;
                sprite_register[HERO] = SPRITE_HERO + 4;
            } else if (ISOPEN((unsigned)mdown) && ISPELLET((unsigned)mdown)) {
                hero_dir              = MYMAN_DOWN;
                sprite_register[HERO] = SPRITE_HERO + 16;
            } else if (ISOPEN((unsigned)mright) && ISPELLET((unsigned)mright)) {
                hero_dir              = MYMAN_RIGHT;
                sprite_register[HERO] = SPRITE_HERO + 12;
            } else if (ISOPEN((unsigned)mup) && ISPELLET((unsigned)mup)) {
                hero_dir              = MYMAN_UP;
                sprite_register[HERO] = SPRITE_HERO;
            } else if (ISOPEN((unsigned)mup) && ISDOT((unsigned)mup)) {
                hero_dir              = MYMAN_UP;
                sprite_register[HERO] = SPRITE_HERO;
            } else if (ISOPEN((unsigned)mleft) && ISDOT((unsigned)mleft)) {
                hero_dir              = MYMAN_LEFT;
                sprite_register[HERO] = SPRITE_HERO + 4;
            } else if (ISOPEN((unsigned)mdown) && ISDOT((unsigned)mdown)) {
                hero_dir              = MYMAN_DOWN;
                sprite_register[HERO] = SPRITE_HERO + 16;
            } else if (ISOPEN((unsigned)mright) && ISDOT((unsigned)mright)) {
                hero_dir              = MYMAN_RIGHT;
                sprite_register[HERO] = SPRITE_HERO + 12;
            } else if (ISOPEN((unsigned)mleft) && (hero_dir != MYMAN_RIGHT)) {
                hero_dir              = MYMAN_LEFT;
                sprite_register[HERO] = SPRITE_HERO + 4;
            } else if (ISOPEN((unsigned)mup) && (hero_dir != MYMAN_DOWN)) {
                hero_dir              = MYMAN_UP;
                sprite_register[HERO] = SPRITE_HERO;
            } else if (ISOPEN((unsigned)mright) && (hero_dir != MYMAN_LEFT)) {
                hero_dir              = MYMAN_RIGHT;
                sprite_register[HERO] = SPRITE_HERO + 12;
            } else if (ISOPEN((unsigned)mdown) && (hero_dir != MYMAN_UP)) {
                hero_dir              = MYMAN_DOWN;
                sprite_register[HERO] = SPRITE_HERO + 16;
            } else if (!(ISOPEN((unsigned)mleft) || ISOPEN((unsigned)mright) ||
                         ISOPEN((unsigned)mdown))) {
                hero_dir              = MYMAN_UP;
                sprite_register[HERO] = SPRITE_HERO;
            } else if (!(ISOPEN((unsigned)mleft) || ISOPEN((unsigned)mright) ||
                         ISOPEN((unsigned)mup))) {
                hero_dir              = MYMAN_DOWN;
                sprite_register[HERO] = SPRITE_HERO + 16;
            } else if (!(ISOPEN((unsigned)mright) || ISOPEN((unsigned)mdown) ||
                         ISOPEN((unsigned)mup))) {
                hero_dir              = MYMAN_LEFT;
                sprite_register[HERO] = SPRITE_HERO + 4;
            } else if (!(ISOPEN((unsigned)mleft) || ISOPEN((unsigned)mdown) ||
                         ISOPEN((unsigned)mup))) {
                hero_dir              = MYMAN_RIGHT;
                sprite_register[HERO] = SPRITE_HERO + 12;
            }
        }
    }
    if (myman_demo_setup) {
        myman_demo_setup--;
    }
    if (!myman_demo_setup) {
        myman_demo++;
    }
    maze_puts(rmsg, cmsg, MSG_COLOR, msg_GAMEOVER);
}

/**
 * @brief Display game introduction sequence showing ghost personalities
 *
 * Shows the classic Pac-Man style intro with ghost nicknames and names:
 * - "SHADOW" / "BLINKY" (red ghost - aggressive chaser)
 * - "SPEEDY" / "PINKY" (pink ghost - ambusher)
 * - "BASHFUL" / "INKY" (cyan ghost - unpredictable)
 * - "POKEY" / "CLYDE" (orange ghost - wanderer)
 *
 * Displays animated ghosts with their personalities and point values.
 *
 * @note Uses global myman_intro counter to track animation state
 * @note Clears sprite registers on first frame (myman_intro == 1)
 */
void gameintro(void) {
    int s;

    if (myman_intro == 1) {
        cycles = -1;
        for (s = 0; s < SPRITE_REGISTERS; s++) {
            sprite_register_used[s]  = 0;
            sprite_register_timer[s] = 0;
        }
        maze_erase();
        ghost_eaten_timer = 0;
    }
    for (s = 0; s < 4; s++) {
        int         eyes;
        int         mean;
        int         blue;
        int         ghost;
        int         nick_col, nick_row;
        int         name_col, name_row;
        const char* ghost_name;
        const char* ghost_nick;

        ghost      = (s == 0)   ? GHOST1
                     : (s == 1) ? GHOST2
                     : (s == 2) ? GHOST0
                     : (s == 3) ? GHOST3
                                : s;
        ghost_nick = (ghost == GHOST0)   ? GHOST0_NICK
                     : (ghost == GHOST2) ? GHOST2_NICK
                     : (ghost == GHOST1) ? GHOST1_NICK
                     : (ghost == GHOST3) ? GHOST3_NICK
                                         : "";
        ghost_name = (ghost == GHOST0)   ? GHOST0_NAME
                     : (ghost == GHOST2) ? GHOST2_NAME
                     : (ghost == GHOST1) ? GHOST1_NAME
                     : (ghost == GHOST3) ? GHOST3_NAME
                                         : "-?????";
        name_row   = 3 * (s + 1) + 1;
        if (name_row < 2)
            name_row = 2;
        name_col = 7 + (maze_w - 28) / 2;
        if (name_col < (sgfx_w + gfx_w - 1) / gfx_w)
            name_col = (sgfx_w + gfx_w - 1) / gfx_w;
        nick_row = name_row;
        nick_col = name_col + (int)strlen(NAME_HEADER);
        if (nick_col + (int)strlen(NICK_HEADER) > maze_w) {
            nick_col = name_col + 1;
            nick_row = name_row + 1;
        }
        if ((!s) && (myman_intro == 1)) {
            maze_puts(name_row - 2, name_col, 0xF, NAME_HEADER);
            maze_puts(nick_row - 2, nick_col, 0xF, NICK_HEADER);
        }
        if (ghost >= ghosts)
            continue;
        eyes = GHOSTEYES(ghost);
        mean = MEANGHOST(ghost);
        blue = BLUEGHOST(ghost);
        if (myman_intro == (1 + s * 3 * TWOSECS / 4)) {
            int hero_x;

            hero_x = (9 + maze_w - 28) * gfx_w / 2;
            if (hero_x < sgfx_w / 2) {
                hero_x = XPIXWRAP(sgfx_w / 2);
            }
            sprite_register_used[eyes]  = VISIBLE_EYES;
            sprite_register_used[mean]  = 1;
            sprite_register_used[blue]  = 0;
            sprite_register_frame[mean] = 0;
            sprite_register_x[HERO]     = sprite_register_x[eyes] =
                sprite_register_x[mean] = hero_x;
            sprite_register_y[HERO]     = sprite_register_y[eyes] =
                sprite_register_y[mean] = (6 * s + 9) * gfx_h / 2;
            if (sprite_register_y[HERO] < sgfx_h / 2) {
                sprite_register_y[HERO]     = sprite_register_y[eyes] =
                    sprite_register_y[mean] = YPIXWRAP(sgfx_h / 2);
            }
            deadpan                     = 0;
            sprite_register_frame[eyes] = MYMAN_RIGHT - 1;
        } else if ((myman_intro > (1 + s * 3 * TWOSECS / 4)) &&
                   (myman_intro < (1 + (s * 3 + 1) * TWOSECS / 4))) {
            sprite_register_x[HERO] -=
                ((myman_intro - 1) - (1 + s * 3 * TWOSECS / 4)) *
                (name_col * gfx_w - sprite_register_x[mean]) /
                (1 + TWOSECS / 4);
            sprite_register_x[HERO] +=
                (myman_intro - (1 + s * 3 * TWOSECS / 4)) *
                (name_col * gfx_w - sprite_register_x[mean]) /
                (1 + TWOSECS / 4);
        }
        if (myman_intro == (1 + (s * 3 + 1) * TWOSECS / 4)) {
#define MEANCOLOR (use_color ? sprite_register_color[mean] : 0xF)
            sprite_register_x[HERO] = name_col * gfx_w + gfx_w / 2;
            maze_puts(name_row, name_col,
                      sprite_color[((unsigned)sprite_register[mean]) +
                                   sprite_register_frame[mean]]
                          ? sprite_color[((unsigned)sprite_register[mean]) +
                                         sprite_register_frame[mean]]
                          : MEANCOLOR,
                      ghost_name);
        } else if ((myman_intro > (1 + (s * 3 + 1) * TWOSECS / 4)) &&
                   (myman_intro < (1 + (s * 3 + 2) * TWOSECS / 4))) {
            sprite_register_x[HERO] -=
                ((myman_intro - 1) - (1 + (s * 3 + 1) * TWOSECS / 4)) *
                ((int)strlen(NAME_HEADER) * gfx_w) / (1 + TWOSECS / 4);
            sprite_register_x[HERO] +=
                (myman_intro - (1 + (s * 3 + 1) * TWOSECS / 4)) *
                ((int)strlen(NAME_HEADER) * gfx_w) / (1 + TWOSECS / 4);
        }
        if (myman_intro == (1 + (s * 3 + 2) * TWOSECS / 4)) {
            sprite_register_x[HERO] = nick_col * gfx_w + gfx_w / 2;
            maze_puts(nick_row, nick_col,
                      sprite_color[((unsigned)sprite_register[mean]) +
                                   sprite_register_frame[mean]]
                          ? sprite_color[((unsigned)sprite_register[mean]) +
                                         sprite_register_frame[mean]]
                          : MEANCOLOR,
                      ghost_nick);
        } else if ((myman_intro > (1 + (s * 3 + 2) * TWOSECS / 4)) &&
                   (myman_intro < (1 + ((s + 1) * 3) * TWOSECS / 4))) {
            sprite_register_x[HERO] -=
                ((myman_intro - 1) - (1 + (s * 3 + 2) * TWOSECS / 4)) *
                ((int)strlen(NICK_HEADER) * gfx_w) / (1 + TWOSECS / 4);
            sprite_register_x[HERO] +=
                (myman_intro - (1 + (s * 3 + 2) * TWOSECS / 4)) *
                ((int)strlen(NICK_HEADER) * gfx_w) / (1 + TWOSECS / 4);
        }
        if (myman_intro > (4 * 3 * TWOSECS / 4)) {
            sprite_register_frame[mean] =
                ((myman_intro / MYMANFIFTH) & 1) ? 1 : 0;
        }
    }
    if (myman_intro == (1 + 4 * 3 * TWOSECS / 4)) {
        sprite_register_x[HERO] = gfx_w * (maze_w + 1) - sgfx_w;
        sprite_register_y[HERO] = ((17 * 2 + 1) * gfx_h) / 2;
        if ((YTILE(sprite_register_y[HERO]) + 6) >= maze_h) {
            maze_erase();
            for (s = 0; s < SPRITE_REGISTERS; s++) {
                sprite_register_used[s]  = 0;
                sprite_register_timer[s] = 0;
                sprite_register_frame[s] = 0;
            }
            sprite_register_y[HERO] = 3 * gfx_h / 2;
        }
        sprite_register_frame[HERO] = 0;
        sprite_register[HERO]       = SPRITE_HERO + 4;
        sprite_register_used[HERO]  = 1;
        if ((YTILE(sprite_register_y[HERO]) + 11) < maze_h) {
            maze_puts(YWRAP(YTILE(sprite_register_y[HERO]) + 11),
                      ((int)maze_w - (int)strlen(MYMANNOTICE)) / 2, 0xD,
                      MYMANNOTICE);
        } else {
            maze_puts(YWRAP(maze_h - 1),
                      ((int)maze_w - (int)strlen(MYMANNOTICE)) / 2, 0xD,
                      MYMANNOTICE);
        }
        maze_puts(YTILE(sprite_register_y[HERO]) + 6, 12 + (maze_w - 28) / 2,
                  0xF, "50 \x9es");
        maze_puts(YTILE(sprite_register_y[HERO]) + 4, 12 + (maze_w - 28) / 2,
                  0xF, "10 \x9es");
        maze_puts(YTILE(sprite_register_y[HERO]) + 6, 10 + (maze_w - 28) / 2,
                  0x7, "\xfe");
        maze_puts(YTILE(sprite_register_y[HERO]) + 4, 10 + (maze_w - 28) / 2,
                  0x7, "\xf9");
        maze_puts(YTILE(sprite_register_y[HERO]), 4 + (maze_w - 28) / 2, 0x7,
                  "\xfe");
    } else if (sprite_register_used[HERO] || ghost_eaten_timer) {
        if (ghost_eaten_timer) {
            ghost_eaten_timer--;
            if (!ghost_eaten_timer) {
                sprite_register_used[HERO]        = 1;
                sprite_register_used[GHOST_SCORE] = 0;
            }
        } else {
            sprite_register_frame[HERO] =
                (myman_intro / (1 + (MYMANFIFTH / 2))) % 4;
            if ((((unsigned)sprite_register[HERO]) == (SPRITE_HERO + 4)) &&
                (sprite_register_x[HERO] == gfx_w * (4 + (maze_w - 28) / 2))) {
                maze_puts(YTILE(sprite_register_y[HERO]), 4 + (maze_w - 28) / 2,
                          0x7, " ");
                sprite_register[HERO]       = SPRITE_HERO + 12;
                sprite_register_frame[HERO] = 0;
            } else if (((unsigned)sprite_register[HERO]) == (SPRITE_HERO + 4)) {
                sprite_register_x[HERO]--;
            } else {
                sprite_register_x[HERO]++;
            }
        }
        for (s = 0; s < 4; s++) {
            int         eyes;
            int         mean;
            int         blue;
            int         ghost;
            const char* extra_ghost_colors = EXTRA_GHOST_COLORS;

            ghost = MAXGHOSTS - 4 +
                    ((s == 0)   ? GHOST1
                     : (s == 1) ? GHOST2
                     : (s == 2) ? GHOST0
                     : (s == 3) ? GHOST3
                                : s);
            if ((ghost < ghosts) || (ghost >= MAXGHOSTS))
                continue;
            eyes                        = GHOSTEYES(ghost);
            mean                        = MEANGHOST(ghost);
            blue                        = BLUEGHOST(ghost);
            sprite_register[eyes]       = SPRITE_EYES;
            sprite_register[mean]       = SPRITE_MEAN;
            sprite_register[blue]       = SPRITE_BLUE;
            sprite_register_y[eyes]     = sprite_register_y[mean] =
                sprite_register_y[blue] = sprite_register_y[HERO];
            sprite_register_frame[eyes] = MYMAN_RIGHT - 1;
            sprite_register_frame[blue] =
                (sprite_register_frame[mean] =
                     ((myman_intro / MYMANFIFTH) & 1) ? 1 : 0);
            if ((((unsigned)sprite_register[HERO]) == (SPRITE_HERO + 4)) &&
                !ghost_eaten_timer) {
                sprite_register_frame[eyes] = MYMAN_LEFT - 1;
                sprite_register_used[eyes]  = VISIBLE_EYES;
                sprite_register_used[mean]  = 1;
                sprite_register_used[blue]  = 0;
                sprite_register_x[eyes]     = sprite_register_x[mean] =
                    sprite_register_x[blue] =
                        3 *
                            (sprite_register_x[HERO] -
                             gfx_w * (3 + (maze_w - 28) / 2)) /
                            2 +
                        sgfx_w * s + gfx_w * (4 + (maze_w - 28) / 2);
            } else if (sprite_register_used[mean] && !ghost_eaten_timer) {
                sprite_register_used[eyes] = sprite_register_used[mean] = 0;
                sprite_register_used[blue]                              = 1;
            } else if (sprite_register_used[eyes]) {
                sprite_register_x[eyes] = sprite_register_x[mean] =
                    sprite_register_x[blue] += 2;
            } else if (sprite_register_used[blue] && collide(blue, HERO) &&
                       !ghost_eaten_timer) {
                sprite_register_used[blue]         = 0;
                sprite_register_used[eyes]         = 1;
                ghost_eaten_timer                  = ONESEC;
                sprite_register_frame[HERO]        = 0;
                sprite_register_used[HERO]         = 0;
                sprite_register_used[GHOST_SCORE]  = 1;
                sprite_register_x[GHOST_SCORE]     = sprite_register_x[blue];
                sprite_register_y[GHOST_SCORE]     = sprite_register_y[blue];
                sprite_register_frame[GHOST_SCORE] = s;
            } else if (sprite_register_used[blue] && (myman_intro & 1) &&
                       !ghost_eaten_timer) {
                sprite_register_x[eyes] = sprite_register_x[mean] =
                    sprite_register_x[blue]++;
            }
            sprite_register_color[eyes] = 0xF;
            sprite_register_color[blue] = 0x9;
            sprite_register_color[mean] =
                extra_ghost_colors[(s % strlen(extra_ghost_colors))];
            if (s == 2)
                sprite_register_color[mean] = 0xB;
            if (s == 0)
                sprite_register_color[mean] = 0xC;
            if (s == 1)
                sprite_register_color[mean] = 0xD;
            if (s == 3)
                sprite_register_color[mean] = 0x6;
        }
    }
    if (!ghost_eaten_timer)
        for (s = 0; s < SPRITE_REGISTERS; s++) {
            if (sprite_register_used[s] && sprite_register_timer[s])
                if (!--sprite_register_timer[s])
                    sprite_register_used[s] = 0;
        }
    myman_intro++;
    cycles++;
}

/**
 * @brief Run level intermission cutscene animation
 *
 * Plays the intermission cutscene after completing certain levels (classic
 * Pac-Man style). Shows animated sequence of hero chasing/being chased by
 * ghosts across the screen. Two different cutscenes alternate:
 * - intermission == 0: Hero chases ghost (ghost loses costume)
 * - intermission == 1: Giant hero chases ghost off screen
 *
 * @note Uses intermission_running counter for animation timing
 * @note Plays myman_sfx_intermission sound effect
 * @see INTERMISSION macro, INTERMISSION_TIME constant
 */
void gameintermission(void) {
    int s;

    if (intermission_running == (1 + INTERMISSION_TIME)) {
        for (s = 0; s < SPRITE_REGISTERS; s++) {
            sprite_register_used[s]  = 0;
            sprite_register_timer[s] = 0;
            sprite_register_frame[s] = 0;
        }
        maze_erase();
        myman_sfx |= myman_sfx_intermission;
    }
    intermission_running--;
    if (intermission == 0) {
        if ((INTERMISSION_TIME - intermission_running) <= PIX_W) {
            sprite_register_used[HERO] = 1;
            sprite_register_y[HERO]    = (maze_h / 2 - 2) * gfx_h;
            sprite_register_x[HERO] =
                PIX_W - gfx_w / 2 - (INTERMISSION_TIME - intermission_running);
            sprite_register[HERO] = SPRITE_HERO + 4;
            sprite_register_frame[HERO] =
                ((INTERMISSION_TIME - intermission_running) /
                 (1 + (MYMANFIFTH / 2))) %
                4;
            if (GHOST1 < ghosts) {
                int mean;
                int eyes;
                int blue;

                mean                    = MEANGHOST(GHOST1);
                eyes                    = GHOSTEYES(GHOST1);
                blue                    = BLUEGHOST(GHOST1);
                sprite_register_x[mean] = sprite_register_x[eyes] =
                    sprite_register_x[blue] =
                        2 * sprite_register_x[HERO] + sgfx_w;
                sprite_register_y[mean]     = sprite_register_y[eyes] =
                    sprite_register_y[blue] = sprite_register_y[HERO];
                sprite_register[mean]       = SPRITE_MEAN;
                sprite_register[eyes]       = SPRITE_EYES;
                sprite_register[blue]       = SPRITE_BLUE;
                sprite_register_frame[eyes] = MYMAN_LEFT - 1;
                sprite_register_frame[blue] =
                    (sprite_register_frame[mean] =
                         (((INTERMISSION_TIME - intermission_running) /
                           MYMANFIFTH) &
                          1)
                             ? 1
                             : 0);
                sprite_register_used[mean] = 1;
                sprite_register_used[eyes] = VISIBLE_EYES;
                sprite_register_used[blue] = 0;
            }
        } else if ((intermission_running <= (2 * PIX_W)) &&
                   intermission_running) {
            /* TODO: need a giant hero here */
            sprite_register_used[HERO]       = 1;
            sprite_register_used[BIGHERO_UL] = 0;
            sprite_register_used[BIGHERO_UR] = 0;
            sprite_register_used[BIGHERO_LL] = 0;
            sprite_register_used[BIGHERO_LR] = 0;
            if (sprite_used[SPRITE_BIGHERO_UL] &&
                sprite_used[SPRITE_BIGHERO_UR] &&
                sprite_used[SPRITE_BIGHERO_LL] &&
                sprite_used[SPRITE_BIGHERO_LR]) {
                sprite_register_used[HERO]       = 0;
                sprite_register_used[BIGHERO_UL] = 1;
                sprite_register_used[BIGHERO_UR] = 1;
                sprite_register_used[BIGHERO_LL] = 1;
                sprite_register_used[BIGHERO_LR] = 1;
            }
            sprite_register_y[HERO] = (maze_h / 2 + 2) * gfx_h;
            sprite_register_x[HERO] = PIX_W - gfx_w / 2 - intermission_running;
            sprite_register_y[BIGHERO_UL] = sprite_register_y[HERO] - sgfx_h;
            sprite_register_y[BIGHERO_UR] = sprite_register_y[HERO] - sgfx_h;
            sprite_register_y[BIGHERO_LL] = sprite_register_y[HERO];
            sprite_register_y[BIGHERO_LR] = sprite_register_y[HERO];
            sprite_register_x[BIGHERO_UL] = sprite_register_x[HERO] - sgfx_w;
            sprite_register_x[BIGHERO_UR] = sprite_register_x[HERO];
            sprite_register_x[BIGHERO_LL] = sprite_register_x[HERO] - sgfx_w;
            sprite_register_x[BIGHERO_LR] = sprite_register_x[HERO];
            sprite_register[HERO]         = SPRITE_HERO + 12;
            sprite_register[BIGHERO_UL]   = SPRITE_BIGHERO_UL;
            sprite_register[BIGHERO_UR]   = SPRITE_BIGHERO_UR;
            sprite_register[BIGHERO_LL]   = SPRITE_BIGHERO_LL;
            sprite_register[BIGHERO_LR]   = SPRITE_BIGHERO_LR;
            sprite_register_frame[HERO] =
                ((INTERMISSION_TIME - intermission_running) /
                 (1 + (MYMANFIFTH / 2))) %
                4;
            sprite_register_frame[BIGHERO_UL] = sprite_register_frame[HERO];
            sprite_register_frame[BIGHERO_UR] = sprite_register_frame[HERO];
            sprite_register_frame[BIGHERO_LL] = sprite_register_frame[HERO];
            sprite_register_frame[BIGHERO_LR] = sprite_register_frame[HERO];
            if (GHOST1 < ghosts) {
                int mean;
                int eyes;
                int blue;

                mean                    = MEANGHOST(GHOST1);
                eyes                    = GHOSTEYES(GHOST1);
                blue                    = BLUEGHOST(GHOST1);
                sprite_register_x[mean] = sprite_register_x[eyes] =
                    sprite_register_x[blue] =
                        PIX_W - gfx_w / 2 -
                        (PIX_W - gfx_w / 2 - sprite_register_x[HERO]) / 2 +
                        sgfx_w;
                sprite_register_y[mean]     = sprite_register_y[eyes] =
                    sprite_register_y[blue] = sprite_register_y[HERO];
                sprite_register[mean]       = SPRITE_MEAN;
                sprite_register[eyes]       = SPRITE_EYES;
                sprite_register[blue]       = SPRITE_BLUE;
                sprite_register_frame[eyes] = MYMAN_LEFT - 1;
                sprite_register_frame[blue] =
                    (sprite_register_frame[mean] =
                         (((INTERMISSION_TIME - intermission_running) /
                           MYMANFIFTH) &
                          1)
                             ? 1
                             : 0);
                sprite_register_used[mean] = 0;
                sprite_register_used[eyes] = 0;
                sprite_register_used[blue] = 1;
            }
        } else {
            for (s = 0; s < SPRITE_REGISTERS; s++) {
                sprite_register_used[s]  = 0;
                sprite_register_timer[s] = 0;
                sprite_register_frame[s] = 0;
            }
        }
    } else {
        maze_erase();
        sprite_register_y[HERO] =
            gfx_h * (maze_h * intermission_running / (1 + INTERMISSION_TIME));
        sprite_register_x[HERO] = gfx_w * maze_w / 2;
        maze_puts(YTILE(sprite_register_y[HERO]),
                  (maze_w - (int)strlen("COFFEE BREAK")) / 2,
                  1 + 0xE * intermission_running / (1 + INTERMISSION_TIME),
                  "COFFEE BREAK");
    }
}
