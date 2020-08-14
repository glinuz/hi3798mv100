/* \file vp_debug_colors.h
 * vp_debug_colors.h
 *
 * This file contains ANSI X3.64 codes for displaying colors.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 5264 $
 * $LastChangedDate: 2009-07-06 10:22:54 -0500 (Mon, 06 Jul 2009) $
 */

#ifndef VP_DEBUG_COLORS_H
#define VP_DEBUG_COLORS_H

#include "vp_api_cfg.h"

#ifdef VP_DEBUG_COLOR

#define COLORS_begin    "\x1b["
#define COLORS_delim    ";"
#define COLORS_end      "m"
#define COLORS_reset    "0"
#define COLORS_bold     "1"
#define COLORS_nonbold  "22"
#define COLORS_fore     "3"
#define COLORS_back     "4"

#define COLORS_black    "0"
#define COLORS_red      "1"
#define COLORS_green    "2"
#define COLORS_yellow   "3"
#define COLORS_blue     "4"
#define COLORS_magenta  "5"
#define COLORS_cyan     "6"
#define COLORS_white    "7"

#define COLORS_bright_black     COLORS_black    COLORS_delim COLORS_bold
#define COLORS_bright_red       COLORS_red      COLORS_delim COLORS_bold
#define COLORS_bright_green     COLORS_green    COLORS_delim COLORS_bold
#define COLORS_bright_yellow    COLORS_yellow   COLORS_delim COLORS_bold
#define COLORS_bright_blue      COLORS_blue     COLORS_delim COLORS_bold
#define COLORS_bright_magenta   COLORS_magenta  COLORS_delim COLORS_bold
#define COLORS_bright_cyan      COLORS_cyan     COLORS_delim COLORS_bold
#define COLORS_bright_white     COLORS_white    COLORS_delim COLORS_bold

#define COLORS_dark_black       COLORS_black    COLORS_delim COLORS_nonbold
#define COLORS_dark_red         COLORS_red      COLORS_delim COLORS_nonbold
#define COLORS_dark_green       COLORS_green    COLORS_delim COLORS_nonbold
#define COLORS_dark_yellow      COLORS_yellow   COLORS_delim COLORS_nonbold
#define COLORS_dark_blue        COLORS_blue     COLORS_delim COLORS_nonbold
#define COLORS_dark_magenta     COLORS_magenta  COLORS_delim COLORS_nonbold
#define COLORS_dark_cyan        COLORS_cyan     COLORS_delim COLORS_nonbold
#define COLORS_dark_white       COLORS_white    COLORS_delim COLORS_nonbold

/*
 * Usage example:
 * printf( color_reset             "this is white-on-black text"
 *         color(red, blue)        "this is red-on-blue text"
 *         color_fg(green)         "this is green-on-blue text"
 *         color_fg(bright_white)  "this is bright-white-on-blue text"
 *         color_bg(black)         "this is bright-white-on-black text"
 *         color_reset             "this is white on black\n"
 * );
 * Note: Bright colors are only allowed in the foreground.
 */

#define color_fg(c)             COLORS_begin COLORS_fore COLORS_ ## c COLORS_end
#define color_bg(c)             COLORS_begin COLORS_back COLORS_ ## c COLORS_end
#define color(fore_c, back_c)   COLORS_begin COLORS_fore COLORS_ ## fore_c COLORS_delim COLORS_back COLORS_ ## back_c COLORS_end
#define color_reset             COLORS_begin COLORS_reset COLORS_end

#else /* !defined(VP_DEBUG_COLOR) */

#define color_fg(c)             ""
#define color_bg(c)             ""
#define color(fore_c, back_c)   ""
#define color_reset             ""

#endif

#endif /* VP_DEBUG_COLORS_H */
