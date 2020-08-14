/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : cylinder_texture.c
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

#include "hi_egl.h"
#include "hi_texture.h"
#include "cylinder_texture.h"
#include "hi_go_surface.h"
#include "hi_go_decoder.h"
#include <GLES/glext.h>

/***************************** Macro Definition ******************************/

/******************** to see which include file we want to use***************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API forward declarations *******************/

/******************************* API realization *****************************/

/**************************** realize hi_deal_with_data.h *************/

HI_S32 CYLINDER_TEXTURE_DecodeImage(HI_CHAR *pFilename, CYLINDER_RECT_S *pRect, HI_HANDLE *pSurface);
HI_S32 CYLINDER_TEXTURE_MergeJPEGImage(HI_HANDLE pColorImage, HI_HANDLE pAlphaImage, CYLINDER_RECT_S *pRect,
                                       HI_HANDLE *pFinalImage);
HI_S32 CYLINDER_TEXTURE_GetSurfaceData(HI_HANDLE surface, HIGO_PIXELDATA_S *pPixData);

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
HTEXTURE CYLINDER_TEXTURE_Create(const CYLNDER_IMAGE_S *pstImage)
{
    //GLenum eFormat;
    GLenum eType = GL_UNSIGNED_BYTE;
    HI_VOID *pImageData = HI_NULL;
    GLuint uiTextureID = -1;
    HTEXTURE hTexture = HI_NULL;
    HI_HANDLE hSurface;
    HI_S32 Ret;
    HIGO_PIXELDATA_S stPixData;

    hTexture = (HTEXTURE)malloc(sizeof(CYLINDER_TEXTURE_S));
    memset(hTexture, 0, sizeof(CYLINDER_TEXTURE_S));
    if (HI_NULL == hTexture)
    {
        HI3D_TRACE("<%s>%d:Malloc for Texture failed!\n", __FUNCTION__, __LINE__);
        return HI_NULL;
    }

    if (HI_NULL != pstImage->pszColorAlphaFile)
    {
       // eFormat = GL_RGBA;
        Ret = CYLINDER_TEXTURE_DecodeImage(pstImage->pszColorAlphaFile, &hTexture->stRect, &hSurface);
        if (HI_SUCCESS != Ret)
        {
            HI3D_TRACE("<%s>%d:Decode Image failed!\n", __FUNCTION__, __LINE__);
            free(hTexture);
            return HI_NULL;
        }

        hTexture->bHasAlpha = HI_TRUE;
    }
    else if (HI_NULL != pstImage->pszColorFile)
    {
        HI_HANDLE hColorSurface;

        //eFormat = GL_RGB;
        hTexture->bHasAlpha = HI_FALSE;

        Ret = CYLINDER_TEXTURE_DecodeImage(pstImage->pszColorFile, &hTexture->stRect, &hColorSurface);
        if (HI_SUCCESS != Ret)
        {
            HI3D_TRACE("<%s>%d:Decode Image failed!\n", __FUNCTION__, __LINE__);
            free(hTexture);
            return HI_NULL;
        }

        hSurface = hColorSurface;

        if (HI_NULL != pstImage->pszAlphaFile)
        {
            HI_HANDLE hAlphaSurface;
           // eFormat = GL_RGBA;

            hTexture->bHasAlpha = HI_TRUE;
            Ret = CYLINDER_TEXTURE_DecodeImage(pstImage->pszAlphaFile, &hTexture->stRect, &hAlphaSurface);
            if (HI_SUCCESS != Ret)
            {
                HI3D_TRACE("<%s>%d:Decode Image failed!\n", __FUNCTION__, __LINE__);
                free(hTexture);
                HI_GO_FreeSurface(hColorSurface);
                return HI_NULL;
            }

            Ret = CYLINDER_TEXTURE_MergeJPEGImage(hColorSurface, hAlphaSurface, &hTexture->stRect, &hSurface);
            HI_GO_FreeSurface(hColorSurface);
            HI_GO_FreeSurface(hAlphaSurface);
            if (HI_SUCCESS != Ret)
            {
                HI3D_TRACE("<%s>%d:Merge Image failed!\n", __FUNCTION__, __LINE__);
                free(hTexture);
                HI_GO_FreeSurface(hSurface);
            }
        }
    }
    else
    {
        HI3D_TRACE("<%s>%d:Image File Invalid!\n", __FUNCTION__, __LINE__);
        free(hTexture);
        return HI_NULL;
    }

    Ret = CYLINDER_TEXTURE_GetSurfaceData(hSurface, &stPixData);
    if (HI_SUCCESS != Ret)
    {
        HI3D_TRACE("<%s>%d: GetSurfaceData failed!\n", __FUNCTION__, __LINE__);
        HI_GO_FreeSurface(hSurface);
        free(hTexture);
        return HI_NULL;
    }

    pImageData = stPixData.pData;

    glGenTextures(1, &uiTextureID);
    if (GL_NO_ERROR != glGetError())
    {
        HI3D_TRACE("<%s>%d: glGenTexture failed!\n", __FUNCTION__, __LINE__);
        HI_GO_FreeSurface(hSurface);
        free(hTexture);
        return HI_NULL;
    }

    glBindTexture( GL_TEXTURE_2D, uiTextureID);
    if (GL_NO_ERROR != glGetError())
    {
        HI3D_TRACE("<%s>%d: glBindTexture failed!\n", __FUNCTION__, __LINE__);
        HI_GO_FreeSurface(hSurface);
        free(hTexture);
        return HI_NULL;
    }

    hTexture->u32TextureID = uiTextureID;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (pstImage->bMipmap)
    {
        hTexture->bMipMap = HI_TRUE;
        glTexParameterf( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else
    {
        hTexture->bMipMap = HI_FALSE;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA_EXT, hTexture->stRect.w, hTexture->stRect.h,
                 0, GL_BGRA_EXT, eType, (GLvoid *)pImageData);
    if (GL_NO_ERROR != glGetError())
    {
        HI3D_TRACE("<%s>%d: glTexImage2D failed!\n", __FUNCTION__, __LINE__);
        HI_GO_FreeSurface(hSurface);
        free(hTexture);
        return HI_NULL;
    }

    HI_GO_FreeSurface(hSurface);
    return hTexture;
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

HI_S32 CYLINDER_TEXTURE_Destroy(HTEXTURE hTexture)
{
    if (HI_NULL == hTexture)
    {
        HI3D_TRACE("<%s>%d: Null Pointer!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    GLuint uiTextureID[1];
    uiTextureID[0] = hTexture->u32TextureID;
    glDeleteTextures(1, uiTextureID);

    free(hTexture);
    return HI_SUCCESS;
}

/******************************************************************************
func                     : CYLINDER_TEXTURE_DecodeImage
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32 CYLINDER_TEXTURE_DecodeImage(HI_CHAR *pFilename, CYLINDER_RECT_S *pRec, HI_HANDLE *pSurface)
{
    HI_S32 Ret;
    HI_HANDLE hDecoder;

    HIGO_DEC_ATTR_S stSrcDesc;
    HIGO_DEC_IMGATTR_S sImgAttr;
    HIGO_DEC_IMGINFO_S sImageInfo;

    memset(&stSrcDesc, 0, sizeof(stSrcDesc));
    memset(&sImgAttr, 0, sizeof(sImgAttr));
    memset(&sImageInfo, 0, sizeof(sImageInfo));
    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = pFilename;

    if (HI_SUCCESS != (Ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder)))
    {
        HI3D_TRACE("<%s>%d: HI_GO_CreateDecoder Failed!\n", __FUNCTION__, __LINE__);
        return Ret;
    }

    if (HI_SUCCESS != (Ret = HI_GO_DecImgInfo(hDecoder, 0, &sImageInfo)))
    {
        HI3D_TRACE("<%s>%d: HI_GO_CreateDecoder Failed!\n", __FUNCTION__, __LINE__);
        HI_GO_DestroyDecoder(hDecoder);
        return Ret;
    }

    sImgAttr.Width  = sImageInfo.Width;
    sImgAttr.Height = sImageInfo.Height;
    sImgAttr.Format = HIGO_PF_8888;
    pRec->x = 0;
    pRec->y = 0;
    pRec->w = sImageInfo.Width;
    pRec->h = sImageInfo.Height;

    if (HI_SUCCESS != (Ret = HI_GO_DecImgData(hDecoder, 0, &sImgAttr, pSurface)))
    {
        HI3D_TRACE("<%s>%d: HI_GO_DecImgData Failed!\n", __FUNCTION__, __LINE__);
        HI_GO_DestroyDecoder(hDecoder);
        return Ret;
    }

    HI_GO_DestroyDecoder(hDecoder);
    return HI_SUCCESS;
}

/******************************************************************************
func                     : CYLINDER_TEXTURE_MergeJPEGImage
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32 CYLINDER_TEXTURE_MergeJPEGImage(HI_HANDLE colorSurface, HI_HANDLE alphaSurface, CYLINDER_RECT_S *pRect,
                                       HI_HANDLE *pFinalSurface)
{
    HI_S32 Ret;
    HIGO_PIXELDATA_S stColorData;
    HIGO_PIXELDATA_S stAlphaData;
    HIGO_PIXELDATA_S stFinalData;
    HI_U32 *pFinalData, *pColorData, *pAlphaData;
    HI_U32 i, j;

    if (HI_SUCCESS != (Ret = HI_GO_CreateSurface(pRect->w, pRect->h, HIGO_PF_8888, pFinalSurface)))
    {
        HI3D_TRACE("<%s>%d: HI_GO_CreateSurface Failed!\n", __FUNCTION__, __LINE__);
        return Ret;
    }

    if (HI_SUCCESS != (Ret = CYLINDER_TEXTURE_GetSurfaceData(*pFinalSurface, &stFinalData)))
    {
        HI3D_TRACE("<%s>%d: CYLINDER_TEXTURE_GetSurfaceData Failed!\n", __FUNCTION__, __LINE__);
        goto fail;
    }

    pFinalData = (HI_U32 *)stFinalData.pData;
    if (HI_SUCCESS != (Ret = CYLINDER_TEXTURE_GetSurfaceData(colorSurface, &stColorData)))
    {
        HI3D_TRACE("<%s>%d: CYLINDER_TEXTURE_GetSurfaceData Failed!\n", __FUNCTION__, __LINE__);
        goto fail;
    }

    if (HI_SUCCESS != (Ret = CYLINDER_TEXTURE_GetSurfaceData(alphaSurface, &stAlphaData)))
    {
        HI3D_TRACE("<%s>%d: CYLINDER_TEXTURE_GetSurfaceData Failed!\n", __FUNCTION__, __LINE__);
        goto fail;
    }

    for (i = 0; i < pRect->h; i++)
    {
        pColorData = (HI_U32 *)((HI_U8 *)stColorData.pData + stColorData.Pitch * i);
        pAlphaData = (HI_U32 *)((HI_U8 *)stAlphaData.pData + stAlphaData.Pitch * i);
        pFinalData = (HI_U32 *)((HI_U8 *)stFinalData.pData + stFinalData.Pitch * i);
        for (j = 0; j < pRect->w; j++)
        {
            *pFinalData++ = ((*pAlphaData++ & 0xff) << 24) | (*pColorData++ & 0x00ffffff);
        }
    }

    return HI_SUCCESS;
fail:
    HI_GO_FreeSurface(*pFinalSurface);
    return Ret;
}

/******************************************************************************
func                     : CYLINDER_TEXTURE_GetSurfaceData
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32 CYLINDER_TEXTURE_GetSurfaceData(HI_HANDLE surface, HIGO_PIXELDATA_S *pPixData)
{
    HI_PIXELDATA pData;
    HI_S32 Ret;

    if (HI_SUCCESS != (Ret = HI_GO_LockSurface(surface, pData, HI_FALSE)))
    {
        HI3D_TRACE("<%s>%d: HI_GO_LockSurface Failed, Ret= 0x%x\n", __FUNCTION__, __LINE__, Ret);
        return Ret;
    }

    if (HI_SUCCESS != (Ret = HI_GO_UnlockSurface(surface)))
    {
        HI3D_TRACE("<%s>%d: HI_GO_UnlockSurface Failed!\n", __FUNCTION__, __LINE__);
        return Ret;
    }

    *pPixData = pData[0];

    return HI_SUCCESS;
}
