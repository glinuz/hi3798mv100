/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : cylinder_common.h
Version		    : Initial Draft
Author		    :
Created		    : 2012/05/14
Description	    :
Function List 	:


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

#ifndef __HI_CYLINDER_COMMON_H__
#define __HI_CYLINDER_COMMON_H__

/*********************************add include here******************************/

#include "hi_cylinder.h"

/*****************************************************************************/

/*****************************************************************************/

#ifdef __cplusplus
 #if __cplusplus

extern "C"
{
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

#define   DEBUG_POS HI3D_TRACE("<%s> Line %d:\n", __FUNCTION__, __LINE__)

/*************************** Structure Definition ****************************/

typedef struct
{
    HI_FLOAT fX;
    HI_FLOAT fY;
    HI_FLOAT fZ;
} HI_3DCORD_S;

typedef struct
{
    HI_FLOAT fS;
    HI_FLOAT fT;
} HI_TEXCORD_S;

typedef struct
{
    HI_3DCORD_S  stCord;
    HI_TEXCORD_S stTextureCord;
    HI_3DCORD_S  stNormalCord;
    HI_FLOAT     fAlpha;
} HI_VERTEX_S;

typedef struct
{
    HI_U32      u32Frames;
    HI_3DCORD_S stCord;
    HI_3DCORD_S stCordInc;
} HI_CORDADJUST_S;

typedef struct
{
    GLuint          u32TextureID;
    HI_BOOL         bHasAlpha;
    HI_BOOL         bMipMap;
    CYLINDER_RECT_S stRect;
} CYLINDER_TEXTURE_S;

typedef CYLINDER_TEXTURE_S* HTEXTURE;

typedef struct
{
    HI_BOOL  bBlending;
    HI_FLOAT fAlpha;
    HI_FLOAT fOffset;
    HI_FLOAT fHeight;
} CYLINDER_REFLECTION_S;

typedef struct
{
    HI_VERTEX_S         * pstVertex;
    HI_VERTEX_S         * pstReflectionVertex;
    HI_U32                u32VertexCount;
    CYLINDER_RECT_S       stRect;
    HTEXTURE              hTexture;
    HI_CORDADJUST_S       stPosition;
    HI_CORDADJUST_S       stScale;
    HI_CORDADJUST_S       stRotate;
    CYLINDER_REFLECTION_S stReflection;
} CYLINDER_MENU_S;

typedef CYLINDER_MENU_S*    HMENU;

typedef struct
{
    HI_U32            u32DispWidth;
    HI_U32            u32DispHeight;
    HMENU             hBackGround;
    HI_FLOAT          fBGAlpha;
    HMENU           * phMenus;
    HI_U32            u32MenuNum;
    HI_U32            u32Frames;
    HI_U32            u32CurFrame;
    HI_U32            u32StillSeconds;
    HI_FLOAT          fReflectOffset;
    HI_FLOAT          fReflectHeight;
    HI_FLOAT          fReflectAlpha;
    CYLINDER_ACTION_E enAction;
    HI_FLOAT          fEyeAngle;
    HI_FLOAT          fAngle;
    HI_FLOAT          fAngleInc;
    HI_BOOL           bReflection;
    HI_U32            u32FocusIndex;
    HI_U32            u32StartIndex;
    HI_BOOL           bAngleChange;
    HI_BOOL           bActionChange;
    struct timeval tvstill;
} CYLINDER_S;

/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

#ifdef __cplusplus

 #if __cplusplus

}

 #endif

#endif    /* __cplusplus */

#endif /* __HI_CYLINDER_COMMON_H__*/
