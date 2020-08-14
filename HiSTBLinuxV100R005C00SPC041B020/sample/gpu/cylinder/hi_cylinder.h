/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_cylinder.h
Version		    : Initial Draft
Author		    :
Created		    : 2012/05/14
Description	    :
Function List 	:


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

#ifndef __HI_CYLINDER_H__
#define __HI_CYLINDER_H__

/*********************************add include here******************************/

#include "hi_type.h"
#include <GLES/gl.h>

/*****************************************************************************/

/*****************************************************************************/

#ifdef __cplusplus
 #if __cplusplus

extern "C"
{
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/
typedef HI_VOID* HCYLINDER;

/*************************** Structure Definition ****************************/

typedef enum
{
    CYLINDER_ACTION_AUTORUN,
    CYLINDER_ACTION_TURNLEFT,
    CYLINDER_ACTION_TURNRIGHT,
    CYLINDER_ACTION_TURNUP,
    CYLINDER_ACTION_TURNDOWN,
    CYLINDER_ACTION_STOP,
    CYLINDER_ACTION_BUTT
} CYLINDER_ACTION_E;

typedef struct
{
    HI_S32 x;
    HI_S32 y;
    HI_S32 w;
    HI_S32 h;
} CYLINDER_RECT_S;

typedef struct
{
    HI_CHAR     *   pszColorAlphaFile;
    HI_CHAR     *   pszColorFile;
    HI_CHAR     *   pszAlphaFile;
    HI_BOOL         bMipmap;
    CYLINDER_RECT_S stRect;
} CYLNDER_IMAGE_S;

/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/******************************************************************************
func                     : HI_CYLINDER_Create
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HCYLINDER HI_CYLINDER_Create(CYLNDER_IMAGE_S *pMenus, HI_U32 u32MenuNum, CYLNDER_IMAGE_S *pBackGround, HI_U8 u8Alpha);

/******************************************************************************
func                     : HI_CYLINDER_Destroy
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_VOID   HI_CYLINDER_Destroy(HCYLINDER hCylinder);

/******************************************************************************
func                     : HI_CYLINDER_SetFrames
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32    HI_CYLINDER_SetFrames(HCYLINDER hCylinder, HI_U32 u32Frames);

/******************************************************************************
func                     : HI_CYLINDER_SetReflection
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32    HI_CYLINDER_SetReflection(HCYLINDER hCylinder, HI_FLOAT fOffset, HI_FLOAT fHeight, HI_FLOAT fAlpha);

/******************************************************************************
func                     : HI_CYLINDER_SetCurFocus
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32    HI_CYLINDER_SetCurFocus(HCYLINDER hCylinder, HI_U32 u32Focus);

/******************************************************************************
func                     : HI_CYLINDER_GetCurFocus
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32    HI_CYLINDER_GetCurFocus(HCYLINDER hCylinder, HI_U32 *pu32Focus);

/******************************************************************************
func                     : HI_CYLINDER_StarAction
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32    HI_CYLINDER_StarAction(HCYLINDER hCylinder, CYLINDER_ACTION_E enAction);

/******************************************************************************
func                     : HI_CYLINDER_AdjustViewAngle
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32    HI_CYLINDER_AdjustViewAngle(HCYLINDER hCylinder, HI_FLOAT fAdjustAngle);

/******************************************************************************
func                     : HI_CYLINDER_GetCurAction
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32    HI_CYLINDER_GetCurAction(HCYLINDER hCylinder, CYLINDER_ACTION_E *penAction);

/******************************************************************************
func                     : HI_CYLINDER_SetMostStillSeconds
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32    HI_CYLINDER_SetMostStillSeconds(HCYLINDER hCylinder, HI_U32 u32Seconds);

/******************************************************************************
func                     : HI_CYLINDER_Draw
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32    HI_CYLINDER_Draw(HCYLINDER hCylinder);

#ifdef __cplusplus

 #if __cplusplus

}

 #endif

#endif    /* __cplusplus */

#endif /* __HI_CYLINDER_H__*/
