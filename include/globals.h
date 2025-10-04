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
 * @brief Global game state - modular organization
 *
 * This header includes all module-specific state headers:
 * - game_state.h: Game state (score, lives, level)
 * - sprite_state.h: Sprite and ghost AI state
 * - maze_state.h: Maze data and dimensions
 * - render_state.h: Rendering and display state
 * - input_state.h: Input handling and timing
 *
 * @note Phase 3 modularization complete - globals.h now acts as
 *       a central aggregator for all module-specific headers
 *
 * Total global variables: 210+
 * Now organized into domain-specific modules for better maintainability
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "game_state.h"
#include "input_state.h"
#include "maze_state.h"
#include "render_state.h"
#include "sprite_state.h"

/*============================================================================
 * CONSTANTS
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

_Static_assert(MAXGHOSTS == 16,
               "MAXGHOSTS must be 16 - ghost arrays hardcoded to this size");

_Static_assert(SPRITE_REGISTERS == 57,
               "SPRITE_REGISTERS must be 57 - calculated from MAXGHOSTS and "
               "sprite layout");

_Static_assert(LIVES >= 0 && LIVES <= 99,
               "LIVES must be between 0 and 99 - display assumes two digits");

_Static_assert(NPENS == 256,
               "NPENS must be 256 - color palette requires full 8-bit range");

_Static_assert(
    sizeof(int) >= 4,
    "Need at least 32-bit integers for score and coordinate calculations");

_Static_assert(sizeof(void*) >= 4,
               "Need at least 32-bit pointers for maze data structures");

#endif /* GLOBALS_H */
