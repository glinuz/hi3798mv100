/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : cylinder_menu.h
Version		    : Initial Draft
Author		    :
Created		    : 2012/05/14
Description	    :
Function List 	:


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

#ifndef __HI_CYLINER_MENU_H__
#define __HI_CYLINER_MENU_H__

/*********************************add include here******************************/

#include "cylinder_common.h"

/*****************************************************************************/

/*****************************************************************************/

#ifdef __cplusplus
 #if __cplusplus

extern "C"
{
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/

/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/******************************************************************************
func                     : CYLINDER_MENU_Create
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HMENU  CYLINDER_MENU_Create(HTEXTURE hTexture, CYLINDER_RECT_S *pstRect);

/******************************************************************************
func                     : CYLINDER_MENU_Destroy
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Destroy(HMENU hMenu);

/******************************************************************************
func                     : CYLINDER_MENU_Move
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Move(HMENU hMenu, HI_U32 u32Frames, HI_3DCORD_S *pstPosition);

/******************************************************************************
func                     : CYLINDER_MENU_Rotate
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Rotate(HMENU hMenu, HI_U32 u32Frames, HI_3DCORD_S *pstRotate);

/******************************************************************************
func                     : CYLINDER_MENU_Scale
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Scale(HMENU hMenu, HI_U32 u32Frames, HI_3DCORD_S *pstScale);

/******************************************************************************
func                     : CYLINDER_MENU_Reflect
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Reflect(HMENU hMenu, HI_U32 u32Frames, CYLINDER_REFLECTION_S *pstReflection);

/******************************************************************************
func                     : CYLINDER_MENU_Draw
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Draw(HMENU hMenu);

/******************************************************************************
func                     : CYLINDER_MENU_Update
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Update(HMENU hMenu);

#ifdef __cplusplus

 #if __cplusplus
}

 #endif

#endif    /* __cplusplus */

#endif /* __HI_CYLINER_MENU_H__*/
