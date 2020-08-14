/*******************************************************************************
* All rights reserved, Copyright (C) huawei LIMITED 2015
*------------------------------------------------------------------------------
* File Name   : penpaint_api.c
* Description : pen and paint api imp header file
* Author      : wangdedong@huawei.com
* Date        : 2015.1.26
* Notes       :
*
*------------------------------------------------------------------------------
* Modifications:
*   Date        Author          Modifications
*******************************************************************************/
/*******************************************************************************
 * This source code has been made available to you by HUAWEI on an
 * AS-IS basis. Anyone receiving this source code is licensed under HUAWEI
 * copyrights to use it in any way he or she deems fit, including copying it,
 * modifying it, compiling it, and redistributing it either with or without
 * modifications. Any person who transfers this source code or any derivative
 * work must include the HUAWEI copyright notice and this paragraph in
 * the transferred software.
*******************************************************************************/

#ifndef __PENPAINT_H__
#define __PENPAINT_H__

typedef struct canvas_s {
	int width;
	int height;
	int bpp;
	char *addr;
	int fd;
	int type;
} canvas_t;

typedef struct color_s {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} color_t;

/*################################ gdi ##################################### */
void draw_point(canvas_t *s, int x, int y, color_t color);
void draw_line(canvas_t *screen, int x1, int y1, int x2, int y2, color_t color);
int canvas_blit(canvas_t *from, canvas_t *to, int x, int y);

/*################################ paint ################################### */
//canvas_t *create_picture_canvas(char *filename);
//void destroy_picture_canvas(canvas_t *canvas);
//canvas_t *create_canvas(int width, int height, int bpp, char *addr, char *dev_file);
//void destroy_canvas(canvas_t *canvas);
int tui_draw_picture(canvas_t *canvas, char *filename, int x, int y);
int tui_draw_picture_ext(canvas_t *canvas, char *filename, int x, int y, int r, int adj);
int tui_draw_pic(canvas_t *canvas, char *foo, int len, int file0_stream1, int x, int y);
int tui_draw_pic_ext(canvas_t *canvas, char *foo, int len, int file0_stream1, int x, int y, int r, int adj);
int tui_draw_pic_ext_scale(canvas_t *canvas, char *foo, int len, int file0_stream1, int x, int y, int dst_width, int dst_height, int r, int adj);

/*################################ dev ##################################### */
canvas_t *create_screen_canvas(void);
void destroy_screen_canvas(canvas_t *canvas);

/*################################ pen ##################################### */
#ifndef TUI_SELECT_VECTOR
#define TUI_SELECT_VECTOR 1
#endif

typedef enum {
#if TUI_SELECT_VECTOR
	TUI_VPEN,
	TUI_VPEN_SONG, /* APPLE_ROMAN and unic */
	TUI_VPEN_KAI,
	TUI_VPEN_ZHONGSONG, /* APPLE_ROMAN and unic and none */
	TUI_RASTER_ASCII,
	TUI_RASTER_ASCII_SMOOTH,
	TUI_VPEN_HEI /* only support unic */
#else
	TUI_PPEN,
	TUI_PPEN_SONG_16,
	TUI_PPEN_SONG_24,
	TUI_PPEN_SONG_40,
	TUI_PPEN_SONG_48
#endif
} tui_pen_type_t;

typedef struct {
	tui_pen_type_t type;
	int height;
	char *filename;
	void *priv;
} tui_pen_t;

typedef enum {
	TUI_GDI_ROUND_RECT,
} tui_gdi_type_t;

typedef struct {
	tui_gdi_type_t type;
	void *priv;
} tui_gdi_t;

tui_gdi_t *create_gdi(tui_gdi_type_t type);
void destroy_gdi(tui_gdi_t *gdi);
void direct_draw_round_rect(tui_gdi_t *gdi, int x, int y, int width, int height, int r,
		int bold, int filled, color_t inner, color_t border);

#if TUI_SELECT_VECTOR
/* if you use ascii library alone,plz set the following option to 1.
 * and you MUST modify your font glyph library name to : DroidSansMono.ttf
 * When will u use ascii library alone?
 * If the ascii glyph of your vector font library is too ugly, u can use this.
 */
#define EXT_ASCII_FONT_LIB 1
#endif

#define TUI_LINE_DISTANCE 10
#define TUI_FONT_DISTANCE 4

tui_pen_t *create_pen(tui_pen_type_t type, int height);
void destroy_pen(tui_pen_t *pen);
int tui_draw_string(canvas_t *screen, tui_pen_t *pen, char *str, int x, int y, color_t color);
int tui_draw_string_ext(canvas_t *screen, tui_pen_t *pen, char *str, int x, int y, color_t color, struct rect_t *rect);
int tui_check_text_format(char *text, tui_pen_t *pen, int *width, int *height, int *lastindex);

#endif

