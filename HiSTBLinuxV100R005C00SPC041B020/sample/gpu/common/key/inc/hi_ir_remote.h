/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_ir_remote.h
Version		    : Initial Draft
Author		    :
Created		    : 2011/11/23
Description	    : the remote device
Function List 	: HI_RemoteDev_Initialize
                : HI_RemoteDev_Shutdown
                : HI_RemoteDev_GetInput


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

#ifndef __HI_IR_REMOTE_H__
#define __HI_IR_REMOTE_H__

#ifndef EGL_API_DIRECTFB

/** 当没有使用DFB显示机制的时候才使用这个遥控器适配层，要是使用DFB显示机制，
** 要使用DFB的适配层。
**/

/*********************************add include here******************************/

 #include "hi_unf_ecs.h"
 #include "hi_type.h"

/*****************************************************************************/

 #ifdef __cplusplus
  #if __cplusplus

extern "C"
{
  #endif
 #endif/* __cplusplus */

/***************************** Macro Definition ******************************/

 #define HI_USE_KEYS_FOR_STB_THEME    /**
                                          Enable keys for HiSilicon STB theme.
                                          Check theme source to confirm which
                                          keycode should be used
                                       **/

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

/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/******************************************************************************
func                     : HI_RemoteDev_Initialize
description              : initialize the remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_RemoteDev_Initialize(HI_VOID);

/******************************************************************************
func                     : HI_RemoteDev_Shutdown
description              : close the remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_RemoteDev_Shutdown(HI_VOID);

/******************************************************************************
func                     : HI_RemoteDev_GetInput
description              : get the input of remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_RemoteDev_GetInput(HI_VOID);

/****************************************************************************/

 #ifdef __cplusplus

  #if __cplusplus
}

  #endif

 #endif   /* __cplusplus */

#endif /** #ifndef EGL_API_DIRECTFB **/

#endif /* __HI_IR_REMOTE_H__*/
