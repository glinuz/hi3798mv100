/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : cylinder_menu.c
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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cylinder_menu.h"
#include "hi_egl.h"
#include "hi_texture.h"

/***************************** Macro Definition ******************************/

#define CYLINDER_SET_VERTEX(VT, x, y, z, s, t) \
    do { \
        VT.stCord.fX = x; \
        VT.stCord.fY = y; \
        VT.stCord.fZ = z; \
        VT.stTextureCord.fS = s; \
        VT.stTextureCord.fT = t; \
    } while (0)

#define CYLINDER_CHECK_ANGLE(fAngle) \
    do {\
        if ((fAngle) > 180)\
        {\
            (fAngle) -= 360; \
        } \
    } while (0)

/******************** to see which include file we want to use***************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

static HI_S32 gs_DispWidth;
static HI_S32 gs_DispHeight;

/******************************* API forward declarations *******************/

/******************************* API realization *****************************/

/**************************** realize hi_deal_with_data.h *************/

/******************************************************************************
func                     : CYLINDER_MENU_Create
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_SUCCESS
others 	                 :
******************************************************************************/
HMENU CYLINDER_MENU_Create(HTEXTURE hTexture, CYLINDER_RECT_S *pstRect)
{
    HMENU hMenu;

    EGLDisplay dpy = hi_egl_getdisp();
    EGLSurface window = hi_egl_getsurface();

    HI_FLOAT fXOffset, fX0, fY0, fX1, fY1, fS0, fT0, fS1, fT1;
    HI_FLOAT fbRatio;

    if ((HI_NULL == pstRect) || (HI_NULL == hTexture))
    {
        HI3D_TRACE("<%s> Line %d:null pointer!\n", __FUNCTION__, __LINE__);
        return HI_NULL;
    }

    hMenu = malloc(sizeof(CYLINDER_MENU_S));
    if (HI_NULL == hMenu)
    {
        HI3D_TRACE("<%s> Line %d:malloc menu failed!\n", __FUNCTION__, __LINE__);
        return HI_NULL;
    }

    memset(hMenu, 0, sizeof(CYLINDER_MENU_S));

    hMenu->hTexture = hTexture;
    hMenu->stRect = *pstRect;
    hMenu->pstVertex = malloc(sizeof(HI_VERTEX_S) * 4);
    if (HI_NULL == hMenu->pstVertex)
    {
        HI3D_TRACE("<%s> Line %d:malloc vertex failed!\n", __FUNCTION__, __LINE__);
        free(hMenu);
        return HI_NULL;
    }

    eglQuerySurface(dpy, window, EGL_WIDTH, &gs_DispWidth);
    eglQuerySurface(dpy, window, EGL_HEIGHT, &gs_DispHeight);

    fbRatio  = (HI_FLOAT)gs_DispWidth / gs_DispHeight;
    fXOffset = 1.0 / gs_DispHeight / 2.0;

    /** calculate vetex */
    fX0 = -fbRatio - fXOffset;
    fY0 = 1.0;
    fX1 = fX0 + (HI_FLOAT) pstRect->w / gs_DispHeight * 2 + fXOffset * 2;
    fY1 = fY0 - (HI_FLOAT) pstRect->h / gs_DispHeight * 2;

    /** calculate texture coordinate */
    fS0 = (HI_FLOAT) hTexture->stRect.x / pstRect->w;
    fT0 = (HI_FLOAT) hTexture->stRect.y / pstRect->h;
    fS1 = (HI_FLOAT) (hTexture->stRect.x + hTexture->stRect.w) / pstRect->w;
    fT1 = (HI_FLOAT) (hTexture->stRect.y + hTexture->stRect.h) / pstRect->h;

    memset(hMenu->pstVertex, 0, sizeof(HI_VERTEX_S) * 4);

    /** be care about direction */
    CYLINDER_SET_VERTEX(hMenu->pstVertex[0], fX0, fY0, 0, fS0, fT0);
    CYLINDER_SET_VERTEX(hMenu->pstVertex[1], fX0, fY1, 0, fS0, fT1);
    CYLINDER_SET_VERTEX(hMenu->pstVertex[2], fX1, fY1, 0, fS1, fT1);
    CYLINDER_SET_VERTEX(hMenu->pstVertex[3], fX1, fY0, 0, fS1, fT0);

    /*HI3D_TRACE("<%s> Line %d: p0 (%f, %f),  p1(%f, %f)!\n", __FUNCTION__, __LINE__, fX0, fY0, fX1, fY1); */
    /** set scale ratio */
    hMenu->stScale.stCord.fX = 1;
    hMenu->stScale.stCord.fY = 1;
    hMenu->stScale.stCord.fZ = 1;

    return hMenu;
}

/******************************************************************************
func                     : CYLINDER_MENU_Destroy
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_SUCCESS
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Destroy(HMENU hMenu)
{
    if (HI_NULL == hMenu)
    {
        return HI_SUCCESS;
    }

    if (HI_NULL != hMenu->pstVertex)
    {
        free(hMenu->pstVertex);
    }

    if (HI_NULL != hMenu->pstReflectionVertex)
    {
        free(hMenu->pstReflectionVertex);
    }

    free(hMenu);

    return HI_SUCCESS;
}

/******************************************************************************
func                     : CYLINDER_MENU_Move
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_SUCCESS
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Move(HMENU hMenu, HI_U32 u32Frames, HI_3DCORD_S *pstPosition)
{
    if ((HI_NULL == pstPosition) || (HI_NULL == hMenu))
    {
        HI3D_TRACE("<%s> Line %d:null pointer!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    if (u32Frames)
    {
        /* Set position increment */
        hMenu->stPosition.stCordInc.fX = (pstPosition->fX - hMenu->stPosition.stCord.fX) / u32Frames;
        hMenu->stPosition.stCordInc.fY = (pstPosition->fY - hMenu->stPosition.stCord.fY) / u32Frames;
        hMenu->stPosition.stCordInc.fZ = (pstPosition->fZ - hMenu->stPosition.stCord.fZ) / u32Frames;
    }
    else
    {
        /* Set position immediately */
        hMenu->stPosition.stCord = *pstPosition;
    }

    hMenu->stPosition.u32Frames = u32Frames;

    return HI_SUCCESS;
}

/******************************************************************************
func                     : CYLINDER_MENU_Rotate
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_SUCCESS
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Rotate(HMENU hMenu, HI_U32 u32Frames, HI_3DCORD_S *pstRotate)
{
    if ((HI_NULL == pstRotate) || (HI_NULL == hMenu))
    {
        HI3D_TRACE("<%s> Line %d:null pointer!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    if (u32Frames)
    {
        /* Set rotation increment */
        hMenu->stRotate.stCordInc.fX = (pstRotate->fX - hMenu->stRotate.stCord.fX) / u32Frames;
        hMenu->stRotate.stCordInc.fY = (pstRotate->fY - hMenu->stRotate.stCord.fY) / u32Frames;
        hMenu->stRotate.stCordInc.fZ = (pstRotate->fZ - hMenu->stRotate.stCord.fZ) / u32Frames;
    }
    else
    {
        /* Set rotation immediately */
        hMenu->stRotate.stCord = *pstRotate;
    }

    hMenu->stRotate.u32Frames = u32Frames;

    return HI_SUCCESS;
}

/******************************************************************************
func                     : CYLINDER_MENU_Rotate
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_SUCCESS
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Scale(HMENU hMenu, HI_U32 u32Frames, HI_3DCORD_S *pstScale)
{
    if ((HI_NULL == pstScale) || (HI_NULL == hMenu))
    {
        HI3D_TRACE("<%s> Line %d:null pointer!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    if (u32Frames)
    {
        /* Set rotation increment */
        hMenu->stScale.stCordInc.fX = (pstScale->fX - hMenu->stScale.stCord.fX) / u32Frames;
        hMenu->stScale.stCordInc.fY = (pstScale->fY - hMenu->stScale.stCord.fY) / u32Frames;
        hMenu->stScale.stCordInc.fZ = (pstScale->fZ - hMenu->stScale.stCord.fZ) / u32Frames;
    }
    else
    {
        /* Set rotation immediately */
        hMenu->stScale.stCord = *pstScale;
    }

    hMenu->stScale.u32Frames = u32Frames;

    return HI_SUCCESS;
}

/******************************************************************************
func                     : CYLINDER_MENU_Reflect
description              :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_SUCCESS
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Reflect(HMENU hMenu, HI_U32 u32Frames, CYLINDER_REFLECTION_S *pstReflection)
{
    if ((HI_NULL == pstReflection) || (HI_NULL == hMenu))
    {
        HI3D_TRACE("<%s> Line %d:null pointer!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    /* Set reflection immediately */
    hMenu->stReflection = *pstReflection;

    return HI_SUCCESS;
}

/******************************************************************************
func                     : CYLINDER_MENU_Draw
description              :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Draw(HMENU hMenu)
{
    if (HI_NULL == hMenu)
    {
        HI3D_TRACE("<%s> Line %d:null pointer!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    /* Calculate positions to be used before and after scaling and rotation */
    HI_FLOAT fHalfW_IM = ((HI_FLOAT) hMenu->hTexture->stRect.w / 2);
    HI_FLOAT fHalfH_IM = ((HI_FLOAT) hMenu->hTexture->stRect.h / 2);
    HI_FLOAT fHalfW_FB = (HI_FLOAT) gs_DispWidth / 2;
    HI_FLOAT fHalfH_FB = (HI_FLOAT) gs_DispHeight / 2;

    HI_FLOAT fPosX0 = (fHalfW_FB - fHalfW_IM) / gs_DispHeight * 2;
    HI_FLOAT fPosY0 = -(fHalfH_FB - fHalfH_IM) / gs_DispHeight * 2;
    HI_FLOAT fPosX1 = (hMenu->stPosition.stCord.fX
                       - (fHalfW_FB - fHalfW_IM * hMenu->stScale.stCord.fX)) / gs_DispHeight * 2;
    HI_FLOAT fPosY1 = -(hMenu->stPosition.stCord.fY
                        - (fHalfH_FB - fHalfH_IM * hMenu->stScale.stCord.fY)) / gs_DispHeight * 2;

    hi_egl_makecurrent();

    /* Setup blend */
    if ((HI_NULL != hMenu->hTexture) && (hMenu->hTexture->bHasAlpha))
    {
        /* Enable alpha blending */
        glEnable( GL_BLEND );
        glDepthMask( GL_TRUE );
    }
    else
    {
        /* Disable alpha blending */
        glDisable( GL_BLEND );
        glDepthMask( GL_TRUE );
    }

    glMatrixMode( GL_TEXTURE );
    glPushMatrix();

    /* Enable texture mapping */
    if (hMenu->hTexture)
    {
        /* Set texture state */
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, hMenu->hTexture->u32TextureID);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                         hMenu->hTexture->bMipMap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    }
    else
    {
        /* Disable texture mapping */
        glBindTexture( GL_TEXTURE_2D, 0 );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        glDisable( GL_TEXTURE_2D );
    }

    glDisable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );

    /* Set translation, scale, and rotation */
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glTranslatef( fPosX1, fPosY1, -hMenu->stPosition.stCord.fZ - 2 );

    glPushMatrix();
    glRotatef( hMenu->stRotate.stCord.fX, 1, 0, 0 );
    glRotatef( hMenu->stRotate.stCord.fY, 0, 1, 0 );
    glRotatef( hMenu->stRotate.stCord.fZ, 0, 0, 1 );

    glScalef( hMenu->stScale.stCord.fX, hMenu->stScale.stCord.fY, hMenu->stScale.stCord.fZ );
    glTranslatef( fPosX0, fPosY0, 0 );

    /*HI3D_TRACE("<%s> Line %d:(%f, %f, %f), %f\n", __FUNCTION__, __LINE__, hMenu->stPosition.stCord.fX, hMenu->stPosition.stCord.fY, hMenu->stPosition.stCord.fZ, hMenu->stRotate.stCord.fY);*/
    glShadeModel( GL_SMOOTH);

    /* Draw 3D Rectangle */
    glDisable( GL_LIGHTING );
    glDisable( GL_CULL_FACE );
    glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );

    /* Draw Object */
    glVertexPointer( 3, GL_FLOAT, sizeof (HI_VERTEX_S), hMenu->pstVertex);
    glTexCoordPointer( 2, GL_FLOAT, sizeof (HI_VERTEX_S), (HI_FLOAT *) hMenu->pstVertex + 3 );
    glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );

    glMatrixMode( GL_TEXTURE );
    glPopMatrix();

    glMatrixMode( GL_MODELVIEW );

    /* Reflection */
    if (hMenu->stReflection.fAlpha != 0.0)
    {
        HI_FLOAT afTex[8];
        HI_FLOAT afColor[16];
        HI_FLOAT fA = 1.0;
        HI_FLOAT fR = 1.0;
        HI_FLOAT fG = 1.0;
        HI_FLOAT fB = 1.0;

        /* Transform reflected Object */
        HI_FLOAT fHeight  = (HI_FLOAT)hMenu->hTexture->stRect.h;
        HI_FLOAT fFactorY = hMenu->stReflection.fHeight / fHeight;
        HI_FLOAT fOffsetY = (fHeight - hMenu->stReflection.fHeight) / 2;
        HI_FLOAT fPosY = (fHeight + hMenu->stReflection.fOffset - fOffsetY) / gs_DispHeight * 2;
        fPosY0 = -(fHalfH_FB - fHalfH_IM) / gs_DispHeight * 2;

        glPopMatrix();
        glPushMatrix();
        glRotatef( -hMenu->stRotate.stCord.fX, 1, 0, 0 );
        glRotatef( hMenu->stRotate.stCord.fY, 0, 1, 0 );
        glRotatef( hMenu->stRotate.stCord.fZ, 0, 0, 1 );
        glScalef( hMenu->stScale.stCord.fX, hMenu->stScale.stCord.fY, hMenu->stScale.stCord.fZ );

        glTranslatef( 0, -fPosY, 0 );
        glScalef( 1, -fFactorY, 1 );
        glTranslatef( fPosX0, fPosY0, 0 );

        glDepthMask( GL_TRUE );
        glVertexPointer( 3, GL_FLOAT, sizeof (HI_VERTEX_S), hMenu->pstVertex);
        if (hMenu->hTexture)
        {
            HI_FLOAT fX0 = (HI_FLOAT) hMenu->hTexture->stRect.x;
            HI_FLOAT fY0 = (HI_FLOAT) hMenu->hTexture->stRect.y
                           + ((HI_FLOAT) hMenu->hTexture->stRect.h - hMenu->stReflection.fHeight);
            HI_FLOAT fX1 = (HI_FLOAT) (hMenu->hTexture->stRect.x + hMenu->hTexture->stRect.w);
            HI_FLOAT fY1 = (HI_FLOAT) (hMenu->hTexture->stRect.y + hMenu->hTexture->stRect.h);
            HI_FLOAT fS0 = fX0 / hMenu->hTexture->stRect.w;
            HI_FLOAT fT0 = fY0 / hMenu->hTexture->stRect.h;
            HI_FLOAT fS1 = fX1 / hMenu->hTexture->stRect.w;
            HI_FLOAT fT1 = fY1 / hMenu->hTexture->stRect.h;

            afTex[0] = fS0;
            afTex[1] = fT0;
            afTex[2] = fS0;
            afTex[3] = fT1;
            afTex[4] = fS1;
            afTex[5] = fT1;
            afTex[6] = fS1;
            afTex[7] = fT0;
        }

        memset( afColor, 0, sizeof (float) * 16 );

        if (hMenu->stReflection.bBlending)
        {
            glEnable( GL_BLEND );
            afColor[4]  = fR;
            afColor[5]  = fG;
            afColor[6]  = fB;
            afColor[7]  = hMenu->stReflection.fAlpha;
            afColor[8]  = fR;
            afColor[9]  = fG;
            afColor[10] = fB;
            afColor[11] = hMenu->stReflection.fAlpha;
        }
        else
        {
            glDisable( GL_BLEND );
            afColor[4]  = fR * hMenu->stReflection.fAlpha;
            afColor[5]  = fG * hMenu->stReflection.fAlpha;
            afColor[6]  = fB * hMenu->stReflection.fAlpha;
            afColor[7]  = hMenu->stReflection.fAlpha;
            afColor[8]  = fR * hMenu->stReflection.fAlpha;
            afColor[9]  = fG * hMenu->stReflection.fAlpha;
            afColor[10] = fB * hMenu->stReflection.fAlpha;
            afColor[11] = hMenu->stReflection.fAlpha;
        }

        glColor4f( fR, fG, fB, fA);

        glEnableClientState( GL_COLOR_ARRAY );
        glTexCoordPointer( 2, GL_FLOAT, 0, afTex);
        glColorPointer( 4, GL_FLOAT, 0, afColor );
        glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
    }

    glPopMatrix();
    glPopMatrix();

    return HI_SUCCESS;
}

/******************************************************************************
func                     : CYLINDER_MENU_Update
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_SUCCESS
others 	                 :
******************************************************************************/
HI_S32 CYLINDER_MENU_Update(HMENU hMenu)
{
    if (HI_NULL == hMenu)
    {
        HI3D_TRACE("<%s> Line %d:null pointer!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    /* Update position */
    if (0 != hMenu->stPosition.u32Frames)
    {
        hMenu->stPosition.stCord.fX += hMenu->stPosition.stCordInc.fX;
        hMenu->stPosition.stCord.fX += hMenu->stPosition.stCordInc.fX;
        hMenu->stPosition.stCord.fX += hMenu->stPosition.stCordInc.fX;

        if (hMenu->stPosition.u32Frames > 0)
        {
            hMenu->stPosition.u32Frames--;
        }
    }

    /* Update scale */
    if (0 != hMenu->stScale.u32Frames)
    {
        hMenu->stScale.stCord.fX += hMenu->stScale.stCordInc.fX;
        hMenu->stScale.stCord.fX += hMenu->stScale.stCordInc.fX;
        hMenu->stScale.stCord.fX += hMenu->stScale.stCordInc.fX;

        if (hMenu->stScale.u32Frames > 0)
        {
            hMenu->stScale.u32Frames--;
        }
    }

    /* Update angle */
    if (0 != hMenu->stRotate.u32Frames)
    {
        hMenu->stRotate.stCord.fX += hMenu->stRotate.stCordInc.fX;
        hMenu->stRotate.stCord.fX += hMenu->stRotate.stCordInc.fX;
        hMenu->stRotate.stCord.fX += hMenu->stRotate.stCordInc.fX;

        if (hMenu->stRotate.u32Frames > 0)
        {
            hMenu->stRotate.u32Frames--;
        }

        CYLINDER_CHECK_ANGLE(hMenu->stRotate.stCord.fX);
        CYLINDER_CHECK_ANGLE(hMenu->stRotate.stCord.fY);
        CYLINDER_CHECK_ANGLE(hMenu->stRotate.stCord.fZ);

        if (hMenu->stRotate.u32Frames > 0)
        {
            hMenu->stRotate.u32Frames--;
        }
    }

    return HI_SUCCESS;
}
