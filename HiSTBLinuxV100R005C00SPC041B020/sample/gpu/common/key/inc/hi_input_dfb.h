/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_input_dfb.h
Version		    : Initial Draft
Author		    :
Created		    : 2011/11/23
Description	    : the input device
Function List 	: 使用DFB里的输入设备，包括鼠标，键盘，遥控器
                  当使用DFB显示机制时，用的输入等机制要和DFB一样，否则会冲突

History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

#ifndef __HI_INPUT_DFB_H__
#define __HI_INPUT_DFB_H__

#ifdef EGL_API_DIRECTFB

/*********************************add include here******************************/

 #include <EGL/egl.h>
 #include "hi_type.h"
 #include "directfb.h"

/*****************************************************************************/

 #ifdef __cplusplus
  #if __cplusplus

extern "C"
{
  #endif
 #endif/* __cplusplus */

/***************************** Macro Definition ******************************/

 #define HI_ADP_KEY_UP 103
 #define HI_ADP_KEY_DOWN 108
 #define HI_ADP_KEY_LEFT 105
 #define HI_ADP_KEY_RIGHT 106
 #define HI_ADP_KEY_ENTER 28
 #define HI_ADP_KEY_BACK 135
 #define HI_ADP_KEY_DEL 14

 #define HI_ADP_KEY_F1 59
 #define HI_ADP_KEY_F2 15
 #define HI_ADP_KEY_F3 61
 #define HI_ADP_KEY_F4 62

 #define HI_ADP_KEY_SET 143
 #define HI_ADP_KEY_SUB 145
 #define HI_ADP_KEY_SEEK 152
 #define HI_ADP_KEY_INFO 156
 #define HI_ADP_KEY_POWER 129

/*************************** Structure Definition ****************************/

typedef struct tagSCREEN_INFO
{
    HI_S32 s32offsetX;
    HI_S32 s32offsetY;
    HI_S32 s32ScreenWidth;
    HI_S32 s32ScreenHeight;
} SCREEN_INFO;

/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/******************************************************************************
 func                     : HI_Input_GetWindowHandle
 description              :
 param[in]                :
 retval                   :HI_SUCCESS
 retval                   :HI_FAILURE
 others 	                 :nothing
******************************************************************************/
HI_S32 HI_Input_GetWindowHandle(IDirectFB              *pdfb,
                                IDirectFBWindow        *window,
                                DFBWindowID             windowID,
                                SCREEN_INFO             screeninfo,
                                NativeWindowType        windowsurface);

/******************************************************************************
func                     : HI_DfbInput_Initialize
description              :
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_DfbInput_Initialize(HI_VOID);

/******************************************************************************
func                     : HI_DfbInput_Shutdown
description              :
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_DfbInput_Shutdown(HI_VOID);

/******************************************************************************
func                     : HI_DfbInput_GetEvents
description              : get the key of remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_DfbInput_GetEvents(HI_VOID);

/******************************************************************************

                   layer上的，参见df_input

******************************************************************************/

/******************************************************************************
func                     : HI_Mouse_GetKey
description              :
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_Mouse_GetKey(HI_VOID);

/****************************************************************************/

 #ifdef __cplusplus

  #if __cplusplus

}
  #endif

 #endif   /* __cplusplus */

#endif/** #ifdef EGL_API_DIRECTFB **/

#endif /* __HI_INPUT_DFB_H__*/
