/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : coverflow_texture.h
Version		    : Initial Draft
Author		    :
Created		    : 2012/05/14
Description	    :
Function List 	:


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

#ifndef __COVERFLOW_TEXTURE_H__
#define __COVERFLOW_TEXTURE_H__

/*********************************add include here******************************/

#include <GLES/gl.h>
#include "hi_type.h"

/*****************************************************************************/

/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/

/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/******************************************************************************
func                     : COVERFLOW_TEXTURE_Create
description              :
param[in]                : pImage
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_SUCCESS
others 	                 :
******************************************************************************/
GLint  COVERFLOW_TEXTURE_Create(HI_CHAR* pImage);

/******************************************************************************
func                     : COVERFLOW_TEXTURE_Destroy
description              :
param[in]                : textures[]
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_SUCCESS
others 	                 :
******************************************************************************/
HI_S32 COVERFLOW_TEXTURE_Destroy(GLint textures[], HI_U32 imageNum);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __COVERFLOW_TEXTURE_H__*/
