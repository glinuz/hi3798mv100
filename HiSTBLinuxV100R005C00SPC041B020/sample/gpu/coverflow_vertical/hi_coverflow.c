/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_coverflow.c
Version		    : Initial Draft
Author		    :
Created		    : 2012/05/14
Description	    :
Function List 	:


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

/*********************************add include here******************************/
#include <stdio.h>
#include "hi_coverflow.h"
#include "coverflow_texture.h"
#include "hi_egl.h"
#include "hi_texture.h"

/***************************** Macro Definition ******************************/

#ifndef max
 #define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define STOP_TIME_MAX 10

/******************** to see which include file we want to use***************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

static HI_S32 g_s32ImageNum = 0;

/******************************* API forward declarations *******************/

/******************************* API realization *****************************/

/**************************** realize hi_deal_with_data.h *************/

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

HI_S32 HI_COVERFLOW_InitGLES(HI_U32 Width, HI_U32 Height, HI_U32 Samples)
{
    HI_S32 s32Ret;

    s32Ret = hi_egl_setup(0, 0, Width, Height, Samples, 0);
    if (HI_SUCCESS != s32Ret)
    {
        HI3D_TRACE(" egl setup failure \n");
        return HI_FAILURE;
    }

    /* Enable depth testing. */
    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);

    /* Set clear colors. */
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    glClearDepthf(1.0f);

    glShadeModel(GL_SMOOTH);

    /* Setup projection. */
    glMatrixMode(GL_PROJECTION);
    glFrustumf(-1280.0f / 720.0f, 1280.0f / 720.0f, -1, 1, 1.0f, 3.0f);

    glMatrixMode(GL_MODELVIEW);

    /* Enable textures. */
    glEnable(GL_TEXTURE_2D);

    /* Load texture and modulate with constant color. */

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    return HI_SUCCESS;
}

/******************************************************************************
func                     : HI_COVERFLOW_AdjustMatrix
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

static HI_VOID HI_COVERFLOW_AdjustMatrix(        HI_U32          u32Steps, \
                                                 HI_COVERFLOW_S *psCoverFlow)
{
    HI_FLOAT Axis_X, Axis_Y, Axis_Z, color;
    HI_S32 i;

    /** Set matrix for focused image. **/
    psCoverFlow->matrix[psCoverFlow->s32CurrentImageIndex][TARGET_MATRIX].x = 0.0f;
    psCoverFlow->matrix[psCoverFlow->s32CurrentImageIndex][TARGET_MATRIX].y = 0.0f;
    psCoverFlow->matrix[psCoverFlow->s32CurrentImageIndex][TARGET_MATRIX].z = -1.2f;
    psCoverFlow->matrix[psCoverFlow->s32CurrentImageIndex][TARGET_MATRIX].sizeX = 0.2f;
    psCoverFlow->matrix[psCoverFlow->s32CurrentImageIndex][TARGET_MATRIX].sizeY = 0.8f;

    psCoverFlow->matrix[psCoverFlow->s32CurrentImageIndex][TARGET_MATRIX].rotation = 0.0f;
    psCoverFlow->matrix[psCoverFlow->s32CurrentImageIndex][TARGET_MATRIX].color = 1.0f;

    /* Set matrix for left images. */

    Axis_X = -0.2f;
    Axis_Y = 0.2f;
    Axis_Z = -1.4f;
    color = 0.7f;
    for (i = psCoverFlow->s32CurrentImageIndex - 1; i >= 0; --i)
    {
        psCoverFlow->matrix[i][TARGET_MATRIX].x = Axis_X;
        psCoverFlow->matrix[i][TARGET_MATRIX].y = Axis_Y;

        psCoverFlow->matrix[i][TARGET_MATRIX].z = Axis_Z;
        psCoverFlow->matrix[i][TARGET_MATRIX].sizeX = 0.2f;
        psCoverFlow->matrix[i][TARGET_MATRIX].sizeY = 0.8f;

        psCoverFlow->matrix[i][TARGET_MATRIX].rotation = 0.0f;
        psCoverFlow->matrix[i][TARGET_MATRIX].color = color;

        /**left image translate -0.2 on x axis **/
        Axis_X -= 0.2f;
        Axis_Y += 0.2f;
        Axis_Z -= 0.2f;

        color = max(color - 0.1f, 0.5f);
    }

    /* Set matrix for right images. */
    Axis_Y = -0.1f;
    Axis_Z = -1.0f;

    color = 0.7f;
    for (i = psCoverFlow->s32CurrentImageIndex + 1; i < g_s32ImageNum; ++i)
    {
        psCoverFlow->matrix[i][TARGET_MATRIX].x = 0.0f;
        psCoverFlow->matrix[i][TARGET_MATRIX].y = Axis_Y;
        psCoverFlow->matrix[i][TARGET_MATRIX].z = Axis_Z;
        psCoverFlow->matrix[i][TARGET_MATRIX].sizeX = 0.2f;
        psCoverFlow->matrix[i][TARGET_MATRIX].sizeY = 0.8f;
        psCoverFlow->matrix[i][TARGET_MATRIX].rotation = 90.0f;
        psCoverFlow->matrix[i][TARGET_MATRIX].color = color;
        Axis_Y -= 0.1f;
        Axis_Z += 0.2f;

        color = max(color - 0.1f, 0.5f);
    }

    /* Set animation steps. */
    psCoverFlow->animation = max(u32Steps, 1);

    /* Compute animation steps. */
    for (i = 0; i < g_s32ImageNum; ++i)
    {
        psCoverFlow->matrix[i][STEP_MATRIX].x =
            (psCoverFlow->matrix[i][TARGET_MATRIX].x - psCoverFlow->matrix[i][CURRENT_MATRIX].x)
            / psCoverFlow->animation;

        psCoverFlow->matrix[i][STEP_MATRIX].y =
            (psCoverFlow->matrix[i][TARGET_MATRIX].y - psCoverFlow->matrix[i][CURRENT_MATRIX].y)
            / psCoverFlow->animation;

        psCoverFlow->matrix[i][STEP_MATRIX].z =
            (psCoverFlow->matrix[i][TARGET_MATRIX].z - psCoverFlow->matrix[i][CURRENT_MATRIX].z)
            / psCoverFlow->animation;

        psCoverFlow->matrix[i][STEP_MATRIX].sizeX =
            (psCoverFlow->matrix[i][TARGET_MATRIX].sizeX - psCoverFlow->matrix[i][CURRENT_MATRIX].sizeX)
            / psCoverFlow->animation;

        psCoverFlow->matrix[i][STEP_MATRIX].sizeY =
            (psCoverFlow->matrix[i][TARGET_MATRIX].sizeY - psCoverFlow->matrix[i][CURRENT_MATRIX].sizeY)
            / psCoverFlow->animation;

        psCoverFlow->matrix[i][STEP_MATRIX].rotation =
            (psCoverFlow->matrix[i][TARGET_MATRIX].rotation - psCoverFlow->matrix[i][CURRENT_MATRIX].rotation)
            / psCoverFlow->animation;

        psCoverFlow->matrix[i][STEP_MATRIX].color =
            (psCoverFlow->matrix[i][TARGET_MATRIX].color - psCoverFlow->matrix[i][CURRENT_MATRIX].color)
            / psCoverFlow->animation;
    }
}

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

HI_S32 HI_COVERFLOW_SetCoverflow(HI_COVERFLOW_S *psCoverFlow, \
                                 HI_BOOL         hasInvertedImage, \
                                 HI_S32          s32Steps, \
                                 HI_FLOAT        angle, \
                                 HI_U32          u32ImageNum, \
                                 HI_CHAR **      pImages, \
                                 HI_CHAR *       pBackground )
{
    HI_U32 i;

    if ((u32ImageNum <= 0) || (HI_NULL == pImages))
    {
        HI3D_TRACE("没有要显示的图片\n");
        return HI_FAILURE;
    }

    psCoverFlow->hasInvertedImage = hasInvertedImage;

    psCoverFlow->s32Steps = (s32Steps < 0 ? -s32Steps : s32Steps);

    psCoverFlow->angle = (angle < 0.0f ? -angle : angle);

    psCoverFlow->eCoverStatus = COVER_STOP;
    g_s32ImageNum = (u32ImageNum > IMAGES_NUM_MAX ? IMAGES_NUM_MAX : u32ImageNum);

    psCoverFlow->s32CurrentImageIndex = g_s32ImageNum - 1;

    psCoverFlow->s32AutomaticMove = 1;
    psCoverFlow->redraw = 1;
    psCoverFlow->animation = 0;

    for (i = 0; i < g_s32ImageNum; i++)
    {
        psCoverFlow->textures[i] = COVERFLOW_TEXTURE_Create(pImages[i]);
        if (-1 == psCoverFlow->textures[i])
        {
            return HI_FAILURE;
        }
    }

    HI_COVERFLOW_AdjustMatrix(1, psCoverFlow);

    /** Use the target matrix as the current matrix. **/
    for (i = 0; i < g_s32ImageNum; ++i)
    {
        psCoverFlow->matrix[i][CURRENT_MATRIX] = \
            psCoverFlow->matrix[i][TARGET_MATRIX];
    }

    if (pBackground != HI_NULL)
    {
        psCoverFlow->BackgroundTexID = COVERFLOW_TEXTURE_Create(pBackground);

        if (-1 == psCoverFlow->BackgroundTexID)
        {
            HI3D_TRACE("<%s>Line %d: Create Background failed!\n", __FUNCTION__, __LINE__);
        }
    }

    return HI_SUCCESS;
}

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

HI_VOID HI_COVERFLOW_CreateCoverflow()
{
    /* Vertex buffer. */
    static const GLfloat VertexBuffer[] =
    {
        /* Image. */
        -0.5f,	0.5f,            0.0f,                0.0f,
        0.5f,	0.5f, 320.0f / 512.0f,                0.0f,
        -0.5f, -0.5f,            0.0f,     468.0f / 512.0f,
        0.5f,  -0.5f, 320.0f / 512.0f,     468.0f / 512.0f,

        /* Reflection. */
        -0.5f, -0.5f,            0.0f,     468.0f / 512.0f,
        0.5f,  -0.5f, 320.0f / 512.0f,     468.0f / 512.0f,
        -0.5f, -1.0f,            0.0f, 468.0f / 512.0f / 2,
        0.5f,  -1.0f, 320.0f / 512.0f, 468.0f / 512.0f / 2,

        /* Vertex color. */
        0.8f,	0.8f,            0.8f,                1.0f,
        0.8f,	0.8f,            0.8f,                1.0f,
        0.8f,	0.8f,            0.8f,                1.0f,
        0.8f,	0.8f,            0.8f,                1.0f,

        0.6f,	0.6f,            0.6f,                1.0f,
        0.6f,	0.6f,            0.6f,                1.0f,
        0.2f,	0.2f,            0.2f,                0.3f,
        0.2f,	0.2f,            0.2f,                0.3f,
    };

    /* VBO */

    glBindBuffer(GL_ARRAY_BUFFER, 1);

    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBuffer), VertexBuffer, GL_STATIC_DRAW);

    static const GLfloat BackGroundBuffer[] =
    {
        -1280.0f / 720.0f * 3.0f,  3.0f, 0.0f, 1.0f,
        -1280.0f / 720.0f * 3.0f, -3.0f, 0.0f, 0.0f,
        1280.0f / 720.0f * 3.0f,  -3.0f, 1.0f, 0.0f,
        1280.0f / 720.0f * 3.0f,   3.0f, 1.0f, 1.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, 2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BackGroundBuffer), BackGroundBuffer, GL_STATIC_DRAW);
}

/*****************************************************************************
 Prototype    : HI_COVERFLOW_DrawBackground
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
 ******************************************************************************/

static HI_VOID HI_COVERFLOW_DrawBackground(HI_COVERFLOW_S *psCoverFlow)
{
    glBindTexture(GL_TEXTURE_2D, psCoverFlow->BackgroundTexID);

    glBindBuffer(GL_ARRAY_BUFFER, 2);

    glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 4 * sizeof(GLfloat), (GLvoid *) 0);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 4 * sizeof(GLfloat), (GLvoid *)(2 * sizeof(GLfloat)));

    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -3.0f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/******************************************************************************
func                     : HI_COVERFLOW_DrawImage
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

static HI_VOID HI_COVERFLOW_DrawImage(HI_COVERFLOW_S *   psCoverFlow, \
                                      HI_U32             Image, \
                                      GLint              Texture, \
                                      HI_COVERMATRIX_S * Matrix)
{
    HI_S32 s32VertexNum = 4;

    /* Enable 2D tetxure. */
    glBindTexture(GL_TEXTURE_2D, Texture);
    glBindBuffer(GL_ARRAY_BUFFER, 1);

    glDisable(GL_CULL_FACE);

    /* Set vertex array. */
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 4 * sizeof(GLfloat), (GLvoid *) 0);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, 4 * sizeof(GLfloat), (GLvoid*) (8 * 4 * sizeof(GLfloat)));

    /* Set texture array. */
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 4 * sizeof(GLfloat), (GLvoid *) (2 * sizeof(GLfloat)));

    /* Translate position. */
    glLoadIdentity();
    glTranslatef(Matrix->x, Matrix->y, Matrix->z);
    glTranslatef(0.0f, -0.5f, 0.0f);

    /** move the current image to the x axis in order to rotate by the x axis **/
    glRotatef(Matrix->rotation, 1, 0, 0);
    glTranslatef(0.0f, 0.5f, 0.0f);

    if ((Image == psCoverFlow->s32CurrentImageIndex) \
        && (psCoverFlow->animation <= 0) \
        && psCoverFlow->hasInvertedImage)
    {
        s32VertexNum = 8;
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, s32VertexNum);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

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

HI_VOID HI_COVERFLOW_Render(HI_COVERFLOW_S *psCoverFlow)
{
    int image;
    int left, right;

    static struct timeval tv;

    /**
    ** See if we have still some animation left
    **/
    if (psCoverFlow->animation > 0)
    {
        /** Next animation step. **/
        psCoverFlow->animation -= 1;
        psCoverFlow->redraw = 1;
    }
    /** See if auto-run is enabled. **/
    else if ((psCoverFlow->eCoverStatus != COVER_STOP) \
             && !((0 == psCoverFlow->s32CurrentImageIndex) \
                  && (COVER_TURN_LEFT == psCoverFlow->eCoverStatus)) \
             && !(((g_s32ImageNum - 1) == psCoverFlow->s32CurrentImageIndex) \
                  && (COVER_TURN_RIGHT == psCoverFlow->eCoverStatus))
    )
    {
        if (COVER_TURN_LEFT == psCoverFlow->eCoverStatus)
        {
            psCoverFlow->s32CurrentImageIndex -= 1;
        }
        else if (COVER_TURN_RIGHT == psCoverFlow->eCoverStatus)
        {
            psCoverFlow->s32CurrentImageIndex += 1;
        }
        else
        {
            psCoverFlow->s32CurrentImageIndex += psCoverFlow->s32AutomaticMove;
        }

        /** Make sure imagea are in-bound. **/
        if (psCoverFlow->s32CurrentImageIndex < 0)
        {
            psCoverFlow->s32CurrentImageIndex = 1;

            psCoverFlow->s32AutomaticMove =
                (COVER_TURN_LEFT == psCoverFlow->eCoverStatus ? -1 : 1);
        }
        else if (psCoverFlow->s32CurrentImageIndex >= g_s32ImageNum)
        {
            psCoverFlow->s32CurrentImageIndex = g_s32ImageNum - 2;
            psCoverFlow->s32AutomaticMove =
                (COVER_TURN_LEFT == psCoverFlow->eCoverStatus ? 1 : -1);
        }

        HI_COVERFLOW_AdjustMatrix(psCoverFlow->s32Steps, psCoverFlow);
        psCoverFlow->redraw = 1;
    }

    if ((COVER_TURN_LEFT == psCoverFlow->eCoverStatus) \
        || (COVER_TURN_RIGHT == psCoverFlow->eCoverStatus))
    {
        psCoverFlow->eCoverStatus = COVER_STOP;
        gettimeofday(&psCoverFlow->tv_still, HI_NULL);
    }
    else if (COVER_STOP == psCoverFlow->eCoverStatus)
    {
        gettimeofday(&tv, HI_NULL);
        if ((tv.tv_sec - psCoverFlow->tv_still.tv_sec) > STOP_TIME_MAX)
        {
            psCoverFlow->eCoverStatus = COVER_TURN_AUTO;
        }
    }

    /** Only execute of we have something to render. **/
    if (!psCoverFlow->redraw)
    {
        return;
    }

    /** Clear color and depth buffers. **/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /** Rendering Background **/
    HI_COVERFLOW_DrawBackground(psCoverFlow);

    image = psCoverFlow->s32CurrentImageIndex;
    left  = image > 0;
    right = (image + 1) < g_s32ImageNum;

    /** Process all images. **/
    while (left || right)
    {
        if (psCoverFlow->animation > 1)
        {
            /** Use "Step" to move to new location. **/
            psCoverFlow->matrix[image][CURRENT_MATRIX].x +=
                psCoverFlow->matrix[image][STEP_MATRIX].x;

            psCoverFlow->matrix[image][CURRENT_MATRIX].y +=
                psCoverFlow->matrix[image][STEP_MATRIX].y;

            psCoverFlow->matrix[image][CURRENT_MATRIX].z +=
                psCoverFlow->matrix[image][STEP_MATRIX].z;

            psCoverFlow->matrix[image][CURRENT_MATRIX].sizeX +=
                psCoverFlow->matrix[image][STEP_MATRIX].sizeX;

            psCoverFlow->matrix[image][CURRENT_MATRIX].sizeY +=
                psCoverFlow->matrix[image][STEP_MATRIX].sizeY;

            psCoverFlow->matrix[image][CURRENT_MATRIX].rotation +=
                psCoverFlow->matrix[image][STEP_MATRIX].rotation;

            psCoverFlow->matrix[image][CURRENT_MATRIX].color +=
                psCoverFlow->matrix[image][STEP_MATRIX].color;
        }
        else
        {
            /** Arrived at final location. **/
            psCoverFlow->matrix[image][CURRENT_MATRIX] = \
                psCoverFlow->matrix[image][TARGET_MATRIX];
        }

        /** Draw the image. **/
        HI_COVERFLOW_DrawImage(psCoverFlow, image, \
                               psCoverFlow->textures[image], \
                               &psCoverFlow->matrix[image][CURRENT_MATRIX]);

        if (left)
        {
            image -= 1;
            left = image > 0;
        }
        else
        {
            if (image < psCoverFlow->s32CurrentImageIndex)
            {
                image = psCoverFlow->s32CurrentImageIndex;
            }

            image += 1;
            right = image < g_s32ImageNum;
        }
    }

    /** Rendering done. **/
    psCoverFlow->redraw = 0;

    hi_egl_swap();
}

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

HI_VOID HI_COVERFLOW_DestroyCoverflow(        HI_COVERFLOW_S *psCoverFlow)
{
    COVERFLOW_TEXTURE_Destroy(psCoverFlow->textures, g_s32ImageNum);
    hi_egl_destroy();
}
