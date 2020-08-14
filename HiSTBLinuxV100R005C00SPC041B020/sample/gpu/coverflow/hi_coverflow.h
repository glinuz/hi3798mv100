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

#ifndef __HI_COVERFLOW_H__
#define __HI_COVERFLOW_H__

/*********************************add include here******************************/

#include "hi_type.h"
#include <GLES/gl.h>
#include <sys/time.h>

/*****************************************************************************/

/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/***************************** Macro Definition ******************************/

#define  NEED_DEPEND_SDK_CONFIG

#define IMAGES_NUM_MAX 10

/*************************** Structure Definition ****************************/

typedef struct hiCOVERMATRIX
{
    GLfloat x, y, z;
    GLfloat sizeX, sizeY;
    GLfloat rotation;
    GLfloat color;
} HI_COVERMATRIX_S;

typedef enum hiMATRIXTYPE
{
    CURRENT_MATRIX,
    TARGET_MATRIX,
    STEP_MATRIX,
    MATRIX_TYPE_COUNT,
    MATRIX_TYPE_BUTT
} HI_MATRIXTYPE_E;

typedef enum hiCOVERSTATUS
{
    COVER_TURN_LEFT,
    COVER_TURN_RIGHT,
    COVER_TURN_AUTO,
    COVER_STOP,
    COVER_TURN_BUTT
} HI_COVERSTATUS_E;

typedef struct hiCOVERFOLOW
{
    HI_S32           animation;
    HI_S32           redraw;
    HI_FLOAT         angle;
    HI_S32           s32CurrentImageIndex;
    HI_S32           s32AutomaticMove;
    HI_BOOL          hasInvertedImage;
    HI_COVERSTATUS_E eCoverStatus;
    HI_S32           s32Steps;
    struct timeval tv_still;
    HI_COVERMATRIX_S matrix[IMAGES_NUM_MAX][MATRIX_TYPE_COUNT];
    GLint            textures[IMAGES_NUM_MAX];
    GLint            BackgroundTexID;
} HI_COVERFLOW_S;

/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/******************************************************************************
func                     : HI_COVERFLOW_InitGLES
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_S32	HI_COVERFLOW_InitGLES(HI_U32 Width, HI_U32 Height, HI_U32 Samples);

/******************************************************************************
func                     : HI_COVERFLOW_SetCoverflow
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_S32	HI_COVERFLOW_SetCoverflow(HI_COVERFLOW_S *psCoverFlow, \
                                  HI_BOOL          hasInvertedImage, \
                                  HI_S32           s32Steps, \
                                  HI_FLOAT         angle, \
                                  HI_U32           u32ImageNum, \
                                  HI_CHAR **       pImages, \
                                  HI_CHAR *        pBackground );

/******************************************************************************
func                     : HI_COVERFLOW_CreateCoverflow
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_VOID HI_COVERFLOW_CreateCoverflow();

/******************************************************************************
func                     : HI_COVERFLOW_Render
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_VOID HI_COVERFLOW_Render(HI_COVERFLOW_S *psCoverFlow);

/******************************************************************************
func                     : HI_COVERFLOW_DestroyCoverflow
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_VOID HI_COVERFLOW_DestroyCoverflow(HI_COVERFLOW_S *psCoverFlow);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __HI_COVERFLOW_H__*/
