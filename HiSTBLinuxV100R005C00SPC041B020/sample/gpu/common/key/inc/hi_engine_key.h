/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_engine_key.c
Version		    : Initial Draft
Author		    :
Created		    : 2011/11/23
Description	    : the input device
Function List 	:
                :
                :


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

#ifndef __HI_ENGINE_KEY__
#define __HI_ENGINE_KEY__

/*********************************add include here******************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "hi_type.h"

/*****************************************************************************/

#ifdef __cplusplus
 #if __cplusplus

extern "C"
{
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/

/** virtual key */
typedef enum hiENGINE_KEYCODE_E
{
    ENGINE_KEY_1 = 2,
    ENGINE_KEY_2,
    ENGINE_KEY_3,
    ENGINE_KEY_4,
    ENGINE_KEY_5,
    ENGINE_KEY_6,
    ENGINE_KEY_7,
    ENGINE_KEY_8,
    ENGINE_KEY_9,
    ENGINE_KEY_0,
    ENGINE_KEY_DEL  = 14,
    ENGINE_KEY_IME  = 38,
    ENGINE_KEY_FAV  = 189,
    ENGINE_KEY_MORE = 190,
    ENGINE_KEY_PREV = 191,
    ENGINE_KEY_REC   = 192,
    ENGINE_KEY_NEXT  = 193,
    ENGINE_KEY_SLOW  = 194,
    ENGINE_KEY_RADIO = 195,
    ENGINE_KEY_LIVE  = 196,

    ENGINE_KEY_SEARCH = 180,

    ENGINE_KEY_UP    = 103,
    ENGINE_KEY_DOWN  = 108,
    ENGINE_KEY_LEFT  = 105,
    ENGINE_KEY_RIGHT = 106,
    ENGINE_KEY_ENTER = 28,
    ENGINE_KEY_EXIT  = 29,
    ENGINE_KEY_MENU  = 30,

    ENGINE_KEY_PLAY = 146,
    ENGINE_KEY_STOP = 147,
    ENGINE_KEY_REW = 150,
    ENGINE_KEY_FF = 151,

    ENGINE_KEY_EPG    = 134,
    ENGINE_KEY_PGUP   = 104,
    ENGINE_KEY_PGDOWN = 109,
    ENGINE_KEY_BACK   = 135,

    ENGINE_KEY_NVOD = 183,
    ENGINE_KEY_BTV,
    ENGINE_KEY_VOD,
    ENGINE_KEY_NPVR,
    ENGINE_KEY_HELP,
    ENGINE_KEY_STOCK,

    ENGINE_KEY_GUIDE = 197,
    ENGINE_KEY_MAIL,
    ENGINE_KEY_NEWS,
    ENGINE_KEY_IPANEL,

    ENGINE_KEY_P1 = 210,
    ENGINE_KEY_P2,

    ENGINE_KEY_F1 = 59,
    ENGINE_KEY_F2 = 15,
    ENGINE_KEY_F3 = 61,
    ENGINE_KEY_F4 = 62,

    ENGINE_KEY_SET     = 143,
    ENGINE_KEY_SUB     = 145,
    ENGINE_KEY_SEEK    = 152,
    ENGINE_KEY_INFO    = 156,
    ENGINE_KEY_POWER   = 129,
    ENGINE_KEY_AUDIO   = 155,
    ENGINE_KEY_MUTE    = 130,
    ENGINE_KEY_CHNDOWN = 138,
    ENGINE_KEY_CHNUP   = 137,
    ENGINE_KEY_VOLDOWN = 148,
    ENGINE_KEY_VOLUP   = 149,

    ENGINE_KEY_BUTT
} ENGINE_KEYCODE_E;

typedef enum hiENGINE_KEYSTATUS_E
{
    ENGINE_KEYSTATUS_DOWN = 0,          /** KEY DOWN */
    ENGINE_KEYSTATUS_HOLE,          /** KEY HOLD */
    ENGINE_KEYSTATUS_UP,            /** KEY UP */
    ENGINE_KEYSTATUS_BUTT
} ENGINE_KEYSTATUS_E;

typedef struct hiENGINE_KEY_S
{
    ENGINE_KEYCODE_E   keyCode;
    ENGINE_KEYSTATUS_E keyStatus;
} ENGINE_KEY_S;

/** Menu */
typedef enum _HS_MENU_E
{
    HS_MENU_MAIN = 0,
    HS_MENU_VIDEO,
    HS_MENU_PHOTO,
    HI_MENU_BUTT
} HS_MENU_E;

/** button on mainmenu */
typedef enum _MAIN_BUTTON_E
{
    MAIN_BUTTON_VIDEO,
    MAIN_BUTTON_3DVIDEO,
    MAIN_BUTTON_MUSIC,
    MAIN_BUTTON_PHOTO,
    MAIN_BUTTON_3DPHOTO,
    MAIN_BUTTON_MULTIPIC,
    MAIN_BUTTON_VIDEOPHONE,
    MAIN_BUTTON_GAME,
    MAIN_BUTTON_BUTT
} MAIN_BUTTON_E;

typedef struct hiENGINE_MENUINFO_S
{
    HS_MENU_E     eMenu;           /* 当前菜单 */
    MAIN_BUTTON_E eButton;         /* 主菜单当前的焦点位置 */
    char*         pszFileName;    /* 子菜单中当前选中文件的文件名 */
} ENGINE_MENUINFO_S;

/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/******************************************************************************
func                     : HI_ENGINE_KEY_Init
description              : initialize the remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_ENGINE_KEY_Init();

/******************************************************************************
func                     : HI_ENGINE_KEY_DeInit
description              : close the remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_ENGINE_KEY_DeInit(HI_VOID);

/******************************************************************************
func                     : HI_ENGINE_KEY_Get
description              : get the input of remote device
param[in]                :
retval                   :HI_SUCCESS
retval                   :HI_FAILURE
others 	                 :nothing
******************************************************************************/
HI_S32 HI_ENGINE_KEY_Get(ENGINE_KEY_S *pKey);

/****************************************************************************/

#ifdef __cplusplus

 #if __cplusplus

}

 #endif

#endif     /* __cplusplus */

#endif /* __HI_ENGINE_KEY_H__ */
