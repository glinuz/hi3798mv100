/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : cylinder_cylinder.c
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
#include <math.h>
#include <sys/time.h>

#include <GLES/gl.h>

#include "hi_cylinder.h"
#include "cylinder_menu.h"
#include "cylinder_texture.h"
#include "hi_egl.h"
#include "hi_texture.h"

/***************************** Macro Definition ******************************/

#define CLINDER_CHECH_POINTER(ptr, rtn) \
    do {\
        if (HI_NULL == ptr)\
        {\
            return rtn; \
        } \
    } while (0)

#define FixedFromInt(n) (GLfixed) ((n) << 16)
#define ZERO FixedFromInt(0)
#define ONE FixedFromInt(1)

#define PI 3.14159265f
#define CYLINDER_RADIUS 350.0f
#define CYLINDER_XOFFSET 100

/******************** to see which include file we want to use***************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API forward declarations *******************/

/******************************* API realization *****************************/

/**************************** realize hi_cylinder.h *************/

static HI_VOID HI_CYLINDER_SetView(CYLINDER_S *pstCylinder, HI_FLOAT fEyeY);
static HI_S32  HI_CYLINDER_DrawBackground(HMENU hBackground, CYLINDER_S *pstCylinder);

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
HCYLINDER HI_CYLINDER_Create(CYLNDER_IMAGE_S *pMenus, HI_U32 u32MenuNum, CYLNDER_IMAGE_S *pBackGround, HI_U8 u8Alpha)
{
    HI_S32 i;
    CYLINDER_S *pstCylinder;
    HTEXTURE hTexture;
    HMENU hMenu;
    EGLDisplay dpy = hi_egl_getdisp();
    EGLSurface window = hi_egl_getsurface();

    if (HI_NULL == pMenus)
    {
        HI3D_TRACE("<%s> Line %d:pMenus is NULL!\n", __FUNCTION__, __LINE__);
        return HI_NULL;
    }

    if (0 == u32MenuNum)
    {
        HI3D_TRACE("<%s> Line %d:no menu!\n", __FUNCTION__, __LINE__);
        return HI_NULL;
    }

    pstCylinder = malloc(sizeof(CYLINDER_S));
    if (HI_NULL == pstCylinder)
    {
        HI3D_TRACE("<%s> Line %d:malloc cylinder object failed!\n", __FUNCTION__, __LINE__);
        return HI_NULL;
    }

    memset(pstCylinder, 0, sizeof(CYLINDER_S));
    pstCylinder->u32MenuNum = u32MenuNum;
    pstCylinder->phMenus = malloc(sizeof(HMENU) * u32MenuNum);
    if (HI_NULL == pstCylinder->phMenus)
    {
        HI3D_TRACE("<%s> Line %d:malloc phMenus failed!\n", __FUNCTION__, __LINE__);
        free(pstCylinder);

        return HI_NULL;
    }

    /** create background */
    if (HI_NULL != pBackGround)
    {
        hTexture = CYLINDER_TEXTURE_Create(pBackGround);
        if (HI_NULL == hTexture)
        {
            free(pstCylinder);
            return HI_NULL;
        }

        hMenu = CYLINDER_MENU_Create(hTexture, &pBackGround->stRect);
        if (HI_NULL == hMenu)
        {
            CYLINDER_TEXTURE_Destroy(hTexture);
            free(pstCylinder);
            return HI_NULL;
        }

        pstCylinder->hBackGround = hMenu;
    }

    /** create menus */
    for (i = 0; i < u32MenuNum; i++)
    {
        hTexture = CYLINDER_TEXTURE_Create(&pMenus[i]);
        if (HI_NULL == hTexture)
        {
            HI_CYLINDER_Destroy((HCYLINDER)pstCylinder);
            return HI_NULL;
        }

        hMenu = CYLINDER_MENU_Create(hTexture, &pMenus[i].stRect);
        if (HI_NULL == hTexture)
        {
            HI_CYLINDER_Destroy((HCYLINDER)pstCylinder);
            return HI_NULL;
        }

        pstCylinder->phMenus[i] = hMenu;
    }

    pstCylinder->fBGAlpha  = u8Alpha;
    pstCylinder->fEyeAngle = 0.75;

    /** Get Display width and height */
    eglQuerySurface(dpy, window, EGL_WIDTH, (EGLint*)&pstCylinder->u32DispWidth);
    eglQuerySurface(dpy, window, EGL_HEIGHT, (EGLint*)&pstCylinder->u32DispHeight);

    glViewport( 0, 0, pstCylinder->u32DispWidth, pstCylinder->u32DispHeight );

    /** set view  */
    HI_CYLINDER_SetView(pstCylinder, pstCylinder->fEyeAngle);

    pstCylinder->fAngle = 360.0f / pstCylinder->u32MenuNum;

    return (HCYLINDER)pstCylinder;
}

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
HI_VOID HI_CYLINDER_Destroy(HCYLINDER hCylinder)
{
    HI_S32 i;
    CYLINDER_S *pstCylinder = (CYLINDER_S *)hCylinder;

    if (HI_NULL == pstCylinder)
    {
        HI3D_TRACE("<%s> Line %d:hCylinder is NULL!\n", __FUNCTION__, __LINE__);
        return;
    }

    for (i = 0; i < pstCylinder->u32MenuNum; i++)
    {
        if (HI_NULL != pstCylinder->phMenus[i])
        {
            if (HI_NULL != pstCylinder->phMenus[i]->hTexture)
            {
                CYLINDER_TEXTURE_Destroy(pstCylinder->phMenus[i]->hTexture);
            }

            CYLINDER_MENU_Destroy(pstCylinder->phMenus[i]);
        }
    }

    if (HI_NULL != pstCylinder->hBackGround)
    {
#if 0
        if (HI_NULL != pstCylinder->hBackGround->hTexture)
        {
            CYLINDER_TEXTURE_Destroy(pstCylinder->hBackGround->hTexture);
        }

        CYLINDER_MENU_Destroy(pstCylinder->hBackGround);
#endif
    }

    if (HI_NULL != pstCylinder->phMenus)
    {
        free(pstCylinder->phMenus);
    }

    free(pstCylinder);

    return;
}

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
HI_S32 HI_CYLINDER_SetFrames(HCYLINDER hCylinder, HI_U32 u32Frames)
{
    CYLINDER_S *pstCylinder = (CYLINDER_S *)hCylinder;

    CLINDER_CHECH_POINTER(hCylinder, HI_FAILURE);

    pstCylinder->u32Frames = u32Frames;
    pstCylinder->fAngleInc = 360.0f / pstCylinder->u32MenuNum / u32Frames;

    return HI_SUCCESS;
}

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
HI_S32 HI_CYLINDER_SetReflection(HCYLINDER hCylinder, HI_FLOAT fOffset, HI_FLOAT fHeight, HI_FLOAT fAlpha)
{
    CYLINDER_S *pstCylinder = (CYLINDER_S *)hCylinder;

    CLINDER_CHECH_POINTER(hCylinder, HI_FAILURE);

    pstCylinder->bReflection    = HI_TRUE;
    pstCylinder->fReflectAlpha  = fAlpha;
    pstCylinder->fReflectHeight = fHeight;
    pstCylinder->fReflectOffset = fOffset;

    return HI_SUCCESS;
}

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
HI_S32 HI_CYLINDER_SetCurFocus(HCYLINDER hCylinder, HI_U32 u32Focus)
{
    CYLINDER_S *pstCylinder = (CYLINDER_S *)hCylinder;

    CLINDER_CHECH_POINTER(hCylinder, HI_FAILURE);

    if (u32Focus >= pstCylinder->u32MenuNum)
    {
        HI3D_TRACE("<%s> Line %d: focus index out of range!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    pstCylinder->u32FocusIndex = u32Focus;
    pstCylinder->u32StartIndex = u32Focus;
    return HI_SUCCESS;
}

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
HI_S32 HI_CYLINDER_GetCurFocus(HCYLINDER hCylinder, HI_U32 *pu32Focus)
{
    CYLINDER_S *pstCylinder = (CYLINDER_S *)hCylinder;

    CLINDER_CHECH_POINTER(hCylinder, HI_FAILURE);
    CLINDER_CHECH_POINTER(pu32Focus, HI_FAILURE);

    *pu32Focus = pstCylinder->u32FocusIndex;

    return HI_SUCCESS;
}

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
HI_S32 HI_CYLINDER_StarAction(HCYLINDER hCylinder, CYLINDER_ACTION_E enAction)
{
    CYLINDER_S *pstCylinder = (CYLINDER_S *)hCylinder;

    CLINDER_CHECH_POINTER(hCylinder, HI_FAILURE);

    if (enAction >= CYLINDER_ACTION_BUTT)
    {
        HI3D_TRACE("<%s> Line %d: enAction is invalid!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    pstCylinder->enAction = enAction;

    switch (enAction)
    {
    case CYLINDER_ACTION_TURNLEFT:
    {
        gettimeofday(&pstCylinder->tvstill, 0);
        break;
    }

    case CYLINDER_ACTION_TURNRIGHT:
    {
        gettimeofday(&pstCylinder->tvstill, 0);
        break;
    }

    default:
        break;
    }

    pstCylinder->bActionChange = HI_TRUE;

    return HI_SUCCESS;
}

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
HI_S32 HI_CYLINDER_AdjustViewAngle(HCYLINDER hCylinder, HI_FLOAT fAdjustAngle)
{
    CYLINDER_S *pstCylinder = (CYLINDER_S *)hCylinder;

    CLINDER_CHECH_POINTER(hCylinder, HI_FAILURE);

    pstCylinder->fEyeAngle += fAdjustAngle;

    HI_CYLINDER_SetView(pstCylinder, pstCylinder->fEyeAngle);

    return HI_SUCCESS;
}

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
HI_S32 HI_CYLINDER_GetCurAction(HCYLINDER hCylinder, CYLINDER_ACTION_E *penAction)
{
    CYLINDER_S *pstCylinder = (CYLINDER_S *)hCylinder;

    CLINDER_CHECH_POINTER(hCylinder, HI_FAILURE);
    CLINDER_CHECH_POINTER(penAction, HI_FAILURE);

    *penAction = pstCylinder->enAction;

    return HI_SUCCESS;
}

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
HI_S32 HI_CYLINDER_SetMostStillSeconds(HCYLINDER hCylinder, HI_U32 u32Seconds)
{
    CYLINDER_S *pstCylinder = (CYLINDER_S *)hCylinder;

    CLINDER_CHECH_POINTER(hCylinder, HI_FAILURE);

    pstCylinder->u32StillSeconds = u32Seconds;
    return HI_SUCCESS;
}

static HI_VOID __gluMakeIdentityf(GLfloat m[16])
{
    m[0 + 4 * 0] = 1;
    m[0 + 4 * 1] = 0;
    m[0 + 4 * 2] = 0;
    m[0 + 4 * 3] = 0;
    m[1 + 4 * 0] = 0;
    m[1 + 4 * 1] = 1;
    m[1 + 4 * 2] = 0;
    m[1 + 4 * 3] = 0;
    m[2 + 4 * 0] = 0;
    m[2 + 4 * 1] = 0;
    m[2 + 4 * 2] = 1;
    m[2 + 4 * 3] = 0;
    m[3 + 4 * 0] = 0;
    m[3 + 4 * 1] = 0;
    m[3 + 4 * 2] = 0;
    m[3 + 4 * 3] = 1;
}

static HI_VOID normalize(HI_FLOAT v[3])
{
    HI_FLOAT r;

    r = (HI_FLOAT)sqrt((HI_DOUBLE)v[0] * v[0] + (HI_DOUBLE)v[1] * v[1] + (HI_DOUBLE)v[2] * v[2] );
    if (r == 0.0)
    {
        return;
    }

    v[0] /= r;
    v[1] /= r;
    v[2] /= r;
}

static HI_VOID cross(HI_FLOAT v1[3], HI_FLOAT v2[3], HI_FLOAT result[3])
{
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[1] = v1[2] * v2[0] - v1[0] * v2[2];
    result[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

HI_VOID gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx,
                  GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy,
                  GLfloat upz)
{
    HI_FLOAT forward[3], side[3], up[3];
    GLfloat m[4][4];

    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;

    up[0] = upx;
    up[1] = upy;
    up[2] = upz;

    normalize(forward);

    /* Side = forward x up */
    cross(forward, up, side);
    normalize(side);

    /* Recompute up as: up = side x forward */
    cross(side, forward, up);

    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];

    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];

    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];

    glMultMatrixf(&m[0][0]);
    glTranslatef(-eyex, -eyey, -eyez);
}

/******************************************************************************
func                     : HI_CYLINDER_SetView
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
static HI_VOID HI_CYLINDER_SetView(CYLINDER_S *pstCylinder, HI_FLOAT fEyeY)
{
    HI_FLOAT Ratio = (HI_FLOAT)pstCylinder->u32DispWidth / pstCylinder->u32DispHeight;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustumf(-Ratio / 2, Ratio / 2, -0.5f, 0.5f, 1.0f, 10.0f);
    gluLookAt(0.0f, fEyeY, 0.0, 0.0f, 0.0f, -4.0f, 0.0f, 1.0f, 0.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Init various states */
    glEnable( GL_DITHER );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_SCISSOR_TEST );
    glEnableClientState( GL_VERTEX_ARRAY );
    glDepthFunc( GL_LEQUAL );
    glShadeModel( GL_SMOOTH );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_GENERATE_MIPMAP_HINT, GL_NICEST );
    glFrontFace( GL_CW );
    glScissor(0, 0, pstCylinder->u32DispWidth, pstCylinder->u32DispHeight);
}

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
HI_S32 HI_CYLINDER_Draw(HCYLINDER hCylinder)
{
    HI_S32 i;
    HI_FLOAT fAngle;
    HI_3DCORD_S stPosition, stRotate, stScale;
    CYLINDER_REFLECTION_S stReflection;

    CYLINDER_S *pstCylinder = (CYLINDER_S *)hCylinder;

    CLINDER_CHECH_POINTER(hCylinder, HI_FAILURE);
    glClearColorx(ZERO, ZERO, ZERO, ZERO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (HI_NULL != pstCylinder->hBackGround)
    {
        HI_CYLINDER_SetView(pstCylinder, 0);
        HI_CYLINDER_DrawBackground(pstCylinder->hBackGround, pstCylinder);
    }

    if (HI_NULL != pstCylinder->hBackGround)
    {
        HI_FLOAT eyeAngle = pstCylinder->fEyeAngle;
        HI_CYLINDER_SetView(pstCylinder, eyeAngle);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    stScale.fX = 0.5;
    stScale.fY = 0.5;
    stScale.fZ = 1.0;

    stRotate.fX = 0.0;
    stRotate.fZ = 0.0;

    stReflection.bBlending = HI_TRUE;   //enable blending effect
    stReflection.fAlpha  = pstCylinder->fReflectAlpha;
    stReflection.fHeight = pstCylinder->fReflectHeight;
    stReflection.fOffset = pstCylinder->fReflectOffset;

    /** set menu attribution and draw menu */
    for (i = 0; i < pstCylinder->u32MenuNum; i++)
    {
        fAngle = ((i + pstCylinder->u32MenuNum
                   - pstCylinder->u32StartIndex)
                  % pstCylinder->u32MenuNum) * pstCylinder->fAngle + pstCylinder->u32CurFrame * pstCylinder->fAngleInc;
        stRotate.fY = fAngle;

        stPosition.fX = pstCylinder->u32DispWidth / 2.0
                        + CYLINDER_RADIUS *		sin(fAngle * 2 * PI
                                                    / 360.0) - pstCylinder->phMenus[i]->stRect.w / 2.0 / 2.0;
        stPosition.fY = pstCylinder->phMenus[i]->stRect.h / 2.0;
        stPosition.fZ = 0.1f - (CYLINDER_RADIUS / pstCylinder->u32DispHeight) * cos(fAngle * 2 * PI / 360.0) * 2.0f;

        //stPosition.fZ = 0.1f - cos(fAngle * 2 * PI /360.0)  ;

        CYLINDER_MENU_Scale(pstCylinder->phMenus[i], 0, &stScale);
        CYLINDER_MENU_Reflect(pstCylinder->phMenus[i], 0, &stReflection);
        CYLINDER_MENU_Rotate(pstCylinder->phMenus[i], 0, &stRotate);
        CYLINDER_MENU_Move(pstCylinder->phMenus[i], 0, &stPosition);
        CYLINDER_MENU_Draw(pstCylinder->phMenus[i]);
    }

#if 0
    if (HI_NULL != pstCylinder->hBackGround)
    {
        HI_CYLINDER_SetView(pstCylinder, 0);
        HI_CYLINDER_DrawBackground(pstCylinder->hBackGround, pstCylinder);
    }
#endif

    switch (pstCylinder->enAction)
    {
    case CYLINDER_ACTION_AUTORUN:
    case CYLINDER_ACTION_TURNUP:
    case CYLINDER_ACTION_TURNDOWN:
    {
        pstCylinder->enAction = CYLINDER_ACTION_AUTORUN;
        pstCylinder->u32CurFrame++;
        break;
    }

    case CYLINDER_ACTION_TURNLEFT:
    {
        pstCylinder->u32CurFrame--;
        if (0 == pstCylinder->u32CurFrame % pstCylinder->u32Frames)
        {
            pstCylinder->u32FocusIndex = (pstCylinder->u32FocusIndex + 1) % (pstCylinder->u32MenuNum);
            pstCylinder->enAction = CYLINDER_ACTION_STOP;
        }

        break;
    }

    case CYLINDER_ACTION_TURNRIGHT:
    {
        pstCylinder->u32CurFrame++;
        if (0 == pstCylinder->u32CurFrame % pstCylinder->u32Frames)
        {
            pstCylinder->u32FocusIndex = (pstCylinder->u32FocusIndex - 1
                                          + pstCylinder->u32MenuNum) % (pstCylinder->u32MenuNum);
            pstCylinder->enAction = CYLINDER_ACTION_STOP;
        }

        break;
    }
    case CYLINDER_ACTION_STOP:
    {
        struct timeval tv;

        if (pstCylinder->u32StillSeconds)
        {
            gettimeofday(&tv, 0);
            if ((tv.tv_sec - pstCylinder->tvstill.tv_sec) > pstCylinder->u32StillSeconds)
            {
                pstCylinder->enAction = CYLINDER_ACTION_AUTORUN;
                pstCylinder->u32CurFrame = 0;
            }
        }
        else
        {
            pstCylinder->enAction = CYLINDER_ACTION_AUTORUN;
            pstCylinder->u32CurFrame = 0;
        }

        break;
    }

    default:
    {
        pstCylinder->enAction = CYLINDER_ACTION_STOP;
        break;
    }
    }

    return HI_SUCCESS;
}

/******************************************************************************
func                     : HI_CYLINDER_DrawBackground
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_S32 HI_CYLINDER_DrawBackground(HMENU hBackground, CYLINDER_S *pstCylinder)
{
    HI_FLOAT fPosX = pstCylinder->u32DispWidth / (HI_FLOAT)pstCylinder->u32DispHeight * 5;
    HI_FLOAT fPosY = 5.0f;

    glMatrixMode( GL_MODELVIEW );
    GLfloat pVertices[] = {
        -fPosX, fPosY,	0,
        -fPosX, -fPosY, 0,
        fPosX,	-fPosY, 0,
        fPosX,	fPosY, 0
    };

    GLfloat pTexCoord[] = {
        0, 1,
        0, 0,
        1, 0,
        1, 1
    };
    /* Setup blend */

    glEnable( GL_BLEND );
    //glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
    glDisableClientState(GL_COLOR_ARRAY);

    /* Enable texture mapping */
    if (hBackground->hTexture)
    {
        /* Set texture state */
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, hBackground->hTexture->u32TextureID);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    }

    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    /* Set translation, scale, and rotation */
    glMatrixMode( GL_MODELVIEW );

    //glLoadIdentity();
    glPushMatrix();
    glTranslatef(.0f, .0f, -10.0f);

    //gluLookAt(0.0f, 0.0f, 0.0, 0.0f, 0.0f, -4.0f, 0.0f, 1.0f, 0.0f);//
    glRotatef(180, 1, 0, 0);

    glVertexPointer(3, GL_FLOAT, 0, pVertices);
    glTexCoordPointer(2, GL_FLOAT, 0, pTexCoord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glPopMatrix();
    return HI_SUCCESS;
}
