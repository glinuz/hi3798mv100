/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_texture.h
Version		    : Initial Draft
Author		    :
Created		    : 2012/05/14
Description	    :
Function List 	:


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

#ifndef __HI_TEXTURE_H__
#define __HI_TEXTURE_H__

/*********************************add include here******************************/

#include "hi_type.h"

/*****************************************************************************/

/*****************************************************************************/

#ifdef __cplusplus
 #if __cplusplus

extern "C"
{
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

#define HI_TGA 0
#define HI_PKM 1

/*************************** Structure Definition ****************************/

/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/******************************************************************************
func                     : hiLoad2DTexture
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_S32 hiLoad2DTexture(const HI_CHAR *pFileName, HI_S32 s32DataType);

#ifdef __cplusplus

 #if __cplusplus
}

 #endif

#endif    /* __cplusplus */

#endif /* __HI_TEXTURE_H__*/
