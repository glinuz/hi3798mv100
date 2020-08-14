#ifndef     _DOLBY_COMMON_H
#define     _DOLBY_COMMON_H

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "hi_common.h"
#include "hi_type.h"
#include "hi_unf_common.h"
#include "hi_go_comm.h"
#include "hi_adp_mpi.h"



#define HI_DEBUG

#ifdef HI_DEBUG_
#	define Hi_Dolby_Debug(HI_VOID)  printf("**** %s , %d ****\n",__FILE__,__LINE__)
#else
#	define Hi_Dolby_Debug(HI_VOID)  
#endif

#define MAX_MEMBER_LIST 4
#define VOL_NUM      15
#define LAYER_WIDTH  1280
#define LAYER_HEIGHT 720


/**** color define ****/
#define GET_COLOR(r, g, b) (((0xff << 24) & 0xff000000) | (((r) << 16) & 0x00ff0000) | (((g) << 8) & 0x0000ff00) | (b & 0xff))

#define APP_WIND_BGCOLOR GET_COLOR(72, 160, 248) 
#define APP_WIND_LCOLOR GET_COLOR(192, 240, 240)
#define APP_BG_COLOR GET_COLOR(0, 0, 0) 
#define APP_WIND_BWIDTH 2
#define APP_TXT_FONTCOLOR GET_COLOR(0, 6, 0)
#define APP_TXT_BGCOLOR 0x80FAFA00 
#define APP_TXT_FONTCOLOR_F GET_COLOR(255, 0, 255)
#define APP_TXT_BGCOLOR_F  0x80FAFA00 
#define APP_WIN_TXT_BGCOLOR GET_COLOR(120, 120, 248)
#define APP_WIN_TXT_COLOR GET_COLOR(248, 248, 248)
#define APP_PROGREE_BGCOLOR GET_COLOR(192, 192, 192)
#define APP_WIN_TXT_RED GET_COLOR(0, 0, 248)
#define YELLOW_COLOR GET_COLOR(178, 178, 178)
#define WHITE_COLOR 0xFFFFFFFF
#define BACK_COLOR 0xFF000000
#define BLUE_COLOR 0xFF0000FA

#endif
