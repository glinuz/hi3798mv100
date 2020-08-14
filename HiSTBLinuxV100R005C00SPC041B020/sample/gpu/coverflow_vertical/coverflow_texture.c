/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : coverflow_texture.c
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
#include <stdlib.h>
#include <string.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

#include "coverflow_texture.h"
#include "hi_go_surface.h"
#include "hi_go_decoder.h"
#include "hi_type.h"
#include "hi_3d_config.h"

/***************************** Macro Definition ******************************/

/******************** to see which include file we want to use***************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

static HI_U32 g_Width;
static HI_U32 g_Height;

/******************************* API forward declarations *******************/

/******************************* API realization *****************************/

/**************************** realize hi_deal_with_data.h *************/

/******************************************************************************
func                     : CYLINDER_TEXTURE_GetSurfaceData
description              :
param[in]                : HI_HANDLE surface
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_SUCCESS
others 	                 :
******************************************************************************/
static HI_S32 COVERFLOW_TEXTURE_GetSurfaceData(       HI_HANDLE         surface, \
                                                      HIGO_PIXELDATA_S *pPixData)
{
    HI_PIXELDATA pData;
    HI_S32 s32Ret;

    s32Ret = HI_GO_LockSurface(surface, pData, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        HI3D_TRACE("lock surface failure \n");
        return s32Ret;
    }

    s32Ret = HI_GO_UnlockSurface(surface);
    if (HI_SUCCESS != s32Ret)
    {
        HI3D_TRACE("HI_GO_UnlockSurface Failed!\n");
        return s32Ret;
    }

    *pPixData = pData[0];

    return HI_SUCCESS;
}

/******************************************************************************
func                     : CYLINDER_TEXTURE_DecodeImage
description              :
param[in]                : HI_CHAR *pFilename
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_SUCCESS
others 	                 :
******************************************************************************/
static HI_S32 COVERFLOW_TEXTURE_DecodeImage(            HI_CHAR *  pFilename, \
                                                        HI_HANDLE *pSurface)
{
    HI_S32 s32Ret;
    HI_HANDLE hDecoder;

    HIGO_DEC_ATTR_S stSrcDesc;
    HIGO_DEC_IMGATTR_S sImgAttr;
    HIGO_DEC_IMGINFO_S sImageInfo;

    memset(&stSrcDesc, 0, sizeof(stSrcDesc));
    memset(&sImgAttr, 0, sizeof(sImgAttr));
    memset(&sImageInfo, 0, sizeof(sImageInfo));

    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = pFilename;

    s32Ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
    if (HI_SUCCESS != s32Ret)
    {
        HI3D_TRACE("%s << %d : HI_GO_CreateDecoder Failed!\n", __FUNCTION__, __LINE__);
        return s32Ret;
    }

    s32Ret = HI_GO_DecImgInfo(hDecoder, 0, &sImageInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI3D_TRACE("higo get dec image information failure \n");
        HI_GO_DestroyDecoder(hDecoder);
        return s32Ret;
    }

    sImgAttr.Width  = sImageInfo.Width;
    sImgAttr.Height = sImageInfo.Height;

    g_Width  = sImageInfo.Width;
    g_Height = sImageInfo.Height;

    sImgAttr.Format = HIGO_PF_8888;

    s32Ret = HI_GO_DecImgData(hDecoder, 0, &sImgAttr, pSurface);
    if (HI_SUCCESS != s32Ret)
    {
        HI3D_TRACE("higo decode failure \n");
        HI_GO_DestroyDecoder(hDecoder);
        return s32Ret;
    }

    HI_GO_DestroyDecoder(hDecoder);

    return HI_SUCCESS;
}

/******************************************************************************
func                     : CYLINDER_TEXTURE_Create
description              :
param[in]                : CYLNDER_IMAGE_S *pstImage
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_SUCCESS
others 	                 :
******************************************************************************/
GLint  COVERFLOW_TEXTURE_Create(HI_CHAR* pSrcImage)
{
    GLuint uiTextureID = -1;
    HI_HANDLE hSurface;
    HI_S32 s32Ret;
    HIGO_PIXELDATA_S stPixData;
    HI_VOID *pImageData = HI_NULL;

    if (HI_NULL != pSrcImage)
    {
        s32Ret = COVERFLOW_TEXTURE_DecodeImage(pSrcImage, &hSurface);
        if (HI_SUCCESS != s32Ret)
        {
            HI3D_TRACE("<%s>%d:Decode Image failed!\n", __FUNCTION__, __LINE__);
            return HI_FAILURE;
        }
    }
    else
    {
        HI3D_TRACE("<%s>%d:Image File Invalid!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    s32Ret = COVERFLOW_TEXTURE_GetSurfaceData(hSurface, &stPixData);
    if (HI_SUCCESS != s32Ret)
    {
        HI3D_TRACE("<%s>%d: GetSurfaceData failed!\n", __FUNCTION__, __LINE__);
        HI_GO_FreeSurface(hSurface);
        return HI_FAILURE;
    }

    pImageData = stPixData.pData;

    glGenTextures(1, &uiTextureID);
    if (GL_NO_ERROR != glGetError())
    {
        HI3D_TRACE("<%s>%d: glGenTexture failed!\n", __FUNCTION__, __LINE__);
        HI_GO_FreeSurface(hSurface);
        return HI_FAILURE;
    }

    glBindTexture( GL_TEXTURE_2D, uiTextureID);
    if (GL_NO_ERROR != glGetError())
    {
        HI3D_TRACE("<%s>%d: glBindTexture failed!\n", __FUNCTION__, __LINE__);
        HI_GO_FreeSurface(hSurface);
        return HI_FAILURE;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, \
                 0, \
                 GL_BGRA_EXT, \
                 g_Width, \
                 g_Height, \
                 0, \
                 GL_BGRA_EXT, \
                 GL_UNSIGNED_BYTE, \
                 (GLvoid *)pImageData);
    if (GL_NO_ERROR != glGetError())
    {
        HI3D_TRACE("<%s>%d: glTexImage2D failed!\n", __FUNCTION__, __LINE__);
        HI_GO_FreeSurface(hSurface);
        return HI_FAILURE;
    }

    HI_GO_FreeSurface(hSurface);

    return uiTextureID;
}

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
HI_S32 COVERFLOW_TEXTURE_Destroy(GLint textures[], HI_U32 imageNum)
{
    if ((HI_NULL == textures) || (imageNum <= 0))
    {
        HI3D_TRACE("<%s>%d: Null Pointer!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    glDeleteTextures(imageNum, (const GLuint*)textures);

    return HI_SUCCESS;
}
