#ifndef __UI_OBJ_H__
#define __UI_OBJ_H__

#include "tee_tui_api.h"
#include <sre_typedef.h>
#include "tui_api.h"

#define FB_SIZE (8*1024*1024)

//now only support ARGB8888
#define BPP_DEFAULT 4

/* some concepts:
 *   win:  the window for the whole screen;
 *   view: part of the window, mapped to a rectangle (or a ui_obj);
 *   vbox: a ui_obj container where its children are aligned vertically;
 *   hbox: a ui_obj container where its children are aligned horizontally;
 *   fb: foreground;
 *   bg: background;
 * */

/* -------------------------------------------- */

#define KEY_BackSpace	0xff08
#define KEY_Delete		0xffff
#define KEY_Left 		0xff51
#define KEY_Right		0xff53

#define KEY_Del			0x8


#define MAX_ADVANCES	64

#define DEFAULT_CARET_HEIGHT 	12
#define DEFAULT_CARET_BLINK_MS	400

struct ui_obj_t;
/* -------------------------------------------- */
struct screen_init_info {
	struct tui_rect  rect_keyboard;
	int type;
	int screen;
	int reserve2;
};

struct caret_info_t {
	int32_t caret_x;
	int32_t caret_y;
	int32_t to_show_caret;
	int32_t caret_height;
	int32_t caret_blink_ms;
};

/* -------------------------------------------- */
/* global vars */
extern struct win_info  g_screen_win;

/* Following global vars should be put to file tee_tui_api.h */
extern int32_t g_font_size;  /* only for test (e.g __gp)! */
extern int32_t g_spacing;    /* only for test (e.g __gp)! */


EXPORTED void set_screen_label(TEE_TUIScreenLabel *new_label);
EXPORTED void init_event_handlers(struct ui_obj_t *obj, int32_t to_null);
EXPORTED void init_button_handlers(struct ui_obj_t **uis, int32_t ui_obj_count, int32_t to_null);



/* -------------------------------------------- */
#define COLOR_PREFIX       '#'

#define PERCENT_SUFFIX     '%'
#define PERMIL_SUFFIX      '$'
#define PERCENT_MASK       (0x40000000)
#define PERMIL_MASK        (0x20000000)
#define RATIO_MASK         (PERCENT_MASK | PERMIL_MASK)
#define TO_ABS(x)          ( (x) & (~RATIO_MASK)   )
#define TO_PERCENT(x)      ( (x) |   PERCENT_MASK  )
#define TO_PERMIL(x)       ( (x) |   PERMIL_MASK   )
#define IS_PERCENT(x)      ( (x) &   PERCENT_MASK  )
#define IS_PERMIL(x)       ( (x) &   PERMIL_MASK   )
#define UN_RATIO(v,base)     \
    do { if(IS_PERCENT(v)) \
        (v) = (base)*(TO_ABS(v))/100; \
        else if(IS_PERMIL(v)) \
        (v) = (base)*(TO_ABS(v))/1000; \
    } while(0)


#define PT_IN_RECT(x,y,rect) ( (x)>=(rect).left && (x)<=(rect).right \
        && (y)>=(rect).top  && (y)<=(rect).bottom)
#define RECT_ONTOP_OF_RECT(r1,r2) (\
        PT_IN_RECT((r1).left,  (r1).top,    (r2)) \
        || PT_IN_RECT((r1).right, (r1).top,    (r2)) \
        || PT_IN_RECT((r1).left,  (r1).bottom, (r2)) \
        || PT_IN_RECT((r1).left,  (r1).bottom, (r2)) \
        )
#define IS_INTERSECTED(r1,r2) (RECT_ONTOP_OF_RECT(r1,r2) || RECT_ONTOP_OF_RECT(r2,r1))


enum special_dim_t
{ MATCH_PARENT = -3, WRAP_CONTENT = -2 };

enum special_family_t
{ NO_OBJ = -9, NO_PARENT = -8, NO_SIBLING = -7, NO_CHILD = -6 };



/*
 *   OT_FRAME: all its children are put on its top-left corner and are stacked.
 *             It is useful to build a soft keyboard.
 * */

enum ui_obj_type_t {
	OT_FRAME, OT_BOX, OT_LABEL, OT_BUTTON, OT_EDITTEXT, OT_MAX = OT_EDITTEXT
};
#define IS_UI_CONTAINER(x) ((x)==OT_FRAME || (x)==OT_BOX)
#define CONTAIN_TEXT(type) ((type)==OT_LABEL || (type)==OT_BUTTON || (type)==OT_EDITTEXT)


// button status:
enum pushed_down_status_t { BTN_DOWN, BTN_UP, BTN_NONE };


enum focus_t
{ FOCUSED, UNFOCUSED };

enum box_orientation_t
{ BO_HORIZ, BO_VERT, FIRST_FRAME };

enum ui_gravity_t {
	GRVT_TOP = (1 << 0), GRVT_BOTTOM = (1 << 1),
	GRVT_CENTER_VERT  = GRVT_TOP  | GRVT_BOTTOM,
	GRVT_LEFT = (1 << 2), GRVT_RIGHT = (1 << 3),
	GRVT_CENTER_HORIZ = GRVT_LEFT | GRVT_RIGHT,
	GRVT_CENTER = GRVT_CENTER_VERT | GRVT_CENTER_HORIZ,
	GRVT_DEFAULT = GRVT_TOP | GRVT_LEFT,
	GRVT_MASK    = GRVT_TOP | GRVT_LEFT | GRVT_BOTTOM | GRVT_RIGHT,
};

enum ui_event_type_t {ET_QUIT_LOOP, ET_INVALIDATE,
		      ET_KEY, ET_TOUCH, ET_TIMER, ET_FOCUS
		     };
enum ui_key_action_t {KA_DOWN = 1 << 0, KA_UP = 1 << 1};
enum ui_touch_action_t {TA_PRESS = 1 << 0, TA_RELEASE = 1 << 1, TA_MOVE = 1 << 2};

#define TP_TOUCH        64
#define TP_RELEASE      32

struct ui_key_event_t {
	int32_t key_code;
	int32_t action;
	int32_t flag;   // alt,ctrl,shift
};

struct ui_touch_event_t {
	int32_t x, y;
	int32_t action;
};

struct ui_timer_event_t {
	int32_t timer_id;
};

struct ui_event_t {
	int32_t type;
	UINT64 time;
	union {
		struct ui_key_event_t key;
		struct ui_touch_event_t touch;
		struct ui_timer_event_t timer;
	};
};


struct ui_obj_t;
typedef int32_t (*on_press_t)(void *env, struct ui_obj_t *obj);
typedef int32_t (*on_move_t)(void *env, struct ui_obj_t *obj, struct ui_event_t *event);
typedef int32_t (*on_release_t)(void *env, struct ui_obj_t *obj);
typedef int32_t (*on_click_t)(void *env, struct ui_obj_t *obj);
typedef int32_t (*on_key_t)(void *env, struct ui_obj_t *obj, struct ui_event_t *event);
typedef int32_t (*on_focus_change_t)(void *env, struct ui_obj_t *obj, int32_t focused);

/* -------------------------------------------- */
/* -------------------------------------------- */
struct point_t {
	int32_t x, y;
};

struct rect_t {
	int32_t top;
	int32_t bottom;
	int32_t left;
	int32_t  right;
	// 0 <= top  < bottom <= HEIGHT
	// 0 <= left < right  <= WIDTH
};


/* -------------------------------------------- */
struct win_info {
	int32_t orientation;
	int32_t screen_width;
	int32_t screen_height;
	int32_t bits_per_pixel;
	int32_t line_length;
	int32_t smem_length;
	int32_t font_type;
};

typedef struct screen_s {
	int width;
	int height;
	char *addr;
	int bits_per_pixel;
} screen_t;
screen_t *tui_screen_info_get(screen_t *si);

/* only single-line is supported! */
struct ui_text_t {
	uint32_t gravity;             /* justify */
	uint32_t rotate;              /* always 0 degree ? */
	uint32_t font_size;
	uint32_t spacing;             /* gap between chars */
	uint32_t writable;            /* bool: does the text point to writable area? */
	uint32_t need_release;        /* bool: does the text need to release? */
	char *text;                /* for soft_kbd, it points to short_text. */
	uint8_t  short_text[4];       /* for switching lower/upper. '4' is enough. */
	uint8_t color[3];
};


/* prefix for all ui_objs */
struct ui_obj_t {
	/* basic info: */
	char *name;                   /* fixed field #1: only used by XML parser */
	int32_t type;                 /* fixed field #2: enum ui_obj_type_t */
	/* ui info */
	int32_t index;                /* runtime value: index into array of all ui_objs, can be regardedd as ID */
	int32_t width, height;        /* REQUIRED: always set in xml. default to 0 (invisible) */
	int32_t xoffset, yoffset;     /* used to relocate an obj in its parent. default to 0 */
	/* skill: to adjust an obj to right, just put an empty box on its left! */
	struct rect_t rect;           /* runtime value: abs value. defaul to 0 (invisible) */
	uint32_t margin;              /* abs value: 0 for most cases if ignored */
	uint32_t padding;             /* abs value: 0 for most cases if ignored */
	TEE_TUIImage bg;              /* background: default to TEE_TUI_NO_SOURCE=0) */
	uint32_t bg_color;            /* for bg in pure color: default to BLACK=0 */
	uint32_t fg_color;            /* for text: default to BLACK=0 */
	/* obj family: */
	int32_t level;
	int32_t parent;
	int32_t prev_sibling;
	int32_t next_sibling;
	int32_t child;
	/* runtime attr: event handlers */
	int32_t on_top;    /* runtime boolean: if NO, no need to show it. default to YES. */
	void   *tag;       /* for any data bound. cast it to 'long' for I386/IA64. */
	on_press_t on_press;
	on_move_t on_move;
	on_release_t on_release;
	on_click_t on_click;
	on_key_t on_key;
	on_focus_change_t on_focus_change;
	int32_t IM_flag;
};

/* rect & text_rect should be measured simultanesously: */
#define IS_UNMEASURED(r)  ((r).top==UNMEASURED || (r).left==UNMEASURED || ((r).top==0 && (r).bottom==0))


/* -------------------------------------------- */
struct ui_frame_t {
	struct ui_obj_t common;       /* fixed field #1 */
	int32_t top_child;            /* runtime attr: no child is ontop if this is NO_CHILD. */
};

struct ui_box_t {
	struct ui_obj_t common;
	int32_t orientation;
};

/* each of the text has to be displayed, otherwise the display will be rejected. */
struct ui_label_t {
	struct ui_obj_t common;
	struct ui_text_t text;        /* text_def_line_1 */
	struct rect_t text_rect;      /* text_def_line_2, based on text alignment. relative to its parent. XXX */
	struct rect_t img_rect;    /* add for img  */
};

/* these offsets are used to move text when button is down or up. */
struct ui_button_t {
	struct ui_obj_t common;
	struct ui_text_t text;        /* text_def_line_1 */
	struct rect_t text_rect;      /* text_def_line_2, based on text alignment. relative to its parent. XXX */
	int32_t IM_next;              /* fixed, the next IM tab for a char set(subset) */
	int32_t key_val;              /* fixed, for soft keyboard. */
	uint32_t pushed_down;          /* runtime attr: drift (down/up) is needed */
	uint32_t disable_flag;
};

#define TUI_KEY_FLAG_PUSH 0
#define TUI_KEY_FLAG_CAPL 1
#define TUI_KEY_FLAG_TEMP 2

#define key_get_flag(val, bit)  ((val >> bit) & 0x1)
#define key_set_flag(val, bit, value) \
do { \
	if (value) \
		val = val | (value << bit); \
	else \
		val = val & ~(1 << bit); \
} while (0)

struct ui_edittext_t
{
	struct ui_obj_t common;
	struct ui_text_t text;        /* text_def_line_1 */
	struct rect_t text_rect;      /* text_def_line_2, based on text alignment. relative to its parent. XXX */
	TEE_TUIEntryFieldMode text_mode;
	TEE_TUIEntryFieldType input_type;
	uint32_t maxlen;              /* */
	int32_t caret_pos;            /* runtime attr: the index of the UTF-8 char after. */
	int32_t characterNumber;
	int32_t horiAdvances[MAX_ADVANCES];
	int32_t advancesTotal;
	int32_t temp_flag;
	int32_t event_flag;
	int32_t old_len;
	bool focus;
	bool cursor_flag;
	bool cursor_status;
};


/* -------------------------------------------- */
extern struct ui_label_t  g_default_SI;

EXPORTED void
configure_security_indicator(struct ui_label_t *si, int32_t width, int32_t height,
			     const TEE_TUIImage *image, const char *text /* no copy */, int32_t padding /* negative ignored*/);
EXPORTED void
relocate_security_indicator(struct ui_label_t *si, const struct win_info *win, const struct rect_t *rect);
EXPORTED int32_t is_in_security_indicator(struct ui_label_t *si, int32_t x, int32_t y);
#define draw_security_indicator(p)  do { ui_draw_label(p); } while(0)

int32_t get_event(struct ui_event_t *event);  // required. return>=0: OK; otherwise: err code.

#define EXPORTED

/* -------------------------------------------- */
/* public utils: */
int32_t  ui_show();
EXPORTED void ui_bitmap(int32_t x1, int32_t y1, char *bitmap, int32_t width, int32_t height);
EXPORTED int get_win_info(struct win_info *win);
EXPORTED void *mem_alloc(uint32_t size);
EXPORTED void mem_free(void *addr);
EXPORTED void ui_draw_point(int32_t x1, int32_t y1, int32_t type, int32_t color);
EXPORTED void ui_draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t type, int32_t color);
EXPORTED void
ui_draw_rect(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
	     int32_t fill_color, int32_t line_color);
EXPORTED void
ui_draw_round_rect(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		   int32_t rx, int32_t ry, int32_t color, int32_t type);
EXPORTED void
ui_draw_round_rect_bitmap(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
			  int32_t rx, int32_t ry, char *src, int32_t srcW, int32_t type);
EXPORTED void
ui_draw_bitmap(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const TEE_TUIImage *img, int32_t bg_color);
EXPORTED void ui_draw_frame(struct ui_frame_t *frame);
EXPORTED void ui_draw_box(struct ui_box_t *box);
EXPORTED void ui_draw_label(struct ui_label_t *label);
EXPORTED void ui_draw_button(struct ui_button_t *button);
EXPORTED void ui_draw_edittext(struct ui_edittext_t *edit);


EXPORTED void ui_show_sole_obj(struct ui_obj_t *obj, int32_t verbose);

EXPORTED void ui_close_objs(struct ui_obj_t **uis, int32_t ui_obj_count);
EXPORTED void ui_free_objs(struct ui_obj_t **uis, int32_t ui_obj_count);
EXPORTED void start_ui_screen(struct ui_obj_t **uis, int32_t ui_obj_count, int32_t if_redraw_def_SI);
EXPORTED void ui_draw_obj(struct ui_obj_t **uis, int32_t ui_obj_count, struct ui_obj_t *top_obj);

EXPORTED void ui_set_obj_ontop(struct ui_obj_t **uis, int32_t ui_obj_count, struct ui_obj_t *top_obj, int32_t if_ontop);


EXPORTED int32_t ui_locate_obj(int32_t x, int32_t y, struct ui_obj_t **uis, int32_t ui_obj_count, int32_t ontop_required);
EXPORTED void try_redraw_SI(struct ui_label_t *si, struct ui_obj_t *covering_obj);

EXPORTED void IM_make_writable_button_texts(struct ui_obj_t **im_uis, int32_t im_obj_count);


EXPORTED struct ui_obj_t *ui_get_focus(void);
EXPORTED void ui_set_focus(struct ui_obj_t *obj, uint32_t focused);

EXPORTED int32_t ui_system_open();	/* called before main event loop */
EXPORTED int32_t ui_system_close();
EXPORTED int32_t ui_open(int flag);
EXPORTED int32_t ui_close();
EXPORTED int32_t ui_copy();
int ui_set_info(int type);
char *ui_get_screen_addr(void);
int keyboard_font_init(int flag);
void keyboard_font_exit();
int ui_set(struct screen_init_info *key_board);
int font_init();
int font_close();
/* -------------------------------------------- */
#endif

