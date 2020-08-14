/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : cylinder_texture.h
Version		    : Initial Draft
Author		    :
Created		    : 2012/05/14
Description	    :
Function List 	:


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

#ifndef __CYLINDER_TEXTURE_H__
#define __CYLINDER_TEXTURE_H__

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
func                     : CYLINDER_TEXTURE_Create
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HTEXTURE CYLINDER_TEXTURE_Create(const CYLNDER_IMAGE_S *pstImage);

/******************************************************************************
func                     : CYLINDER_TEXTURE_Destroy
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32   CYLINDER_TEXTURE_Destroy(HTEXTURE hTexture);

#ifdef __cplusplus

 #if __cplusplus
}

 #endif

#endif    /* __cplusplus */

#endif /* __CYLINDER_TEXTURE_H__*/
