#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "tee_internal_api.h"
#include "tee_log.h"


#if defined(STATIC)
#undef STATIC
#endif
#define STATIC static

#if defined(EXPORTED)
#undef EXPORTED
#endif
#define EXPORTED
#define DEBUG_TAG "[debug]"
//#define TUI_DEBUG
#ifdef TUI_DEBUG
#define LOG(fmt, args...)  ta_debug("%s %s: " fmt "", DEBUG_TAG, __FUNCTION__, ## args)
#else
#define LOG(fmt, args...)
#endif
#define ERR(fmt, args...) ta_error("%s %s: " fmt "", DEBUG_TAG, __FUNCTION__, ## args)
#define DBG(fmt, args...) uart_printf_func("%s %s: " fmt "", DEBUG_TAG, __FUNCTION__, ## args)

#define WIN_DEFAULT_WIDTH       1080
#define WIN_DEFAULT_HEIGHT      1920

#define swap(x,y) \
    ({ typeof(x) t;\
     t = y; \
     y = x; \
     x = t; \
     })

#ifndef YES
#define YES       1
#endif
#ifndef NO
#define NO        0
#endif
#define NO_KEYVAL (-1)

/* colors */
#define BG_WHITE        0x00F0F0F0
#define BRAND_BG        0x0011EE22
#define BRAND_FG        0x001122EE
#define INDICATOR_BG    0x00EE1122
#define INDICATOR_FG    0x001122EE
#define ENTRIES_BG      0x00334455
#define ENTRIES_FG      0x00D0D0D0
#define BORDER_BLACK    0x00101010
#define BORDER_WHITE    0x00E0E0E0
#define LABELTEXT_RED   0x00A10000
#define LABELTEXT_GREEN 0x0000A100
#define LABELTEXT_BLUE  0x000000A1
#define EDITTEXT_RED    0x00E10000
#define EDITTEXT_GREEN  0x0000E100
#define EDITTEXT_BLUE   0x000000E1

/* ------------------------------------------------- */
struct text_measure_t {
	int32_t width;
	int32_t height;
	int32_t last_index;
};


#endif //__GLOBAL_H__
