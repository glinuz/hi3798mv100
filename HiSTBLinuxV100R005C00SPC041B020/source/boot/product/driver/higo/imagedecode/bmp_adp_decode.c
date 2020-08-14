/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : bmp_adp_decode.c
Version             : Initial Draft
Author              :
Created             : 2016/01/12
Description         : bmp解码
Function List       :
History             :
Date                       Author                   Modification
2016/01/12                 y00181162                Created file
******************************************************************************/

/*********************************add include here******************************/
#include "hi_gfx_io.h"
#include "bmp_adp_decode.h"

#ifdef HI_BUILD_IN_MINI_BOOT
#include "mmu.h"
#endif

/***************************** Macro Definition ******************************/

#define BMP_WORD_LEN                  sizeof(HI_U16)
#define BMP_DWORD_LEN                 sizeof(HI_U32)
#define BMP_FILEHEADER_LEN            0x0E


/*************************** Structure Definition ****************************/


typedef struct tag_HIGFX_BITMAPFILEHEADER
{
    HI_U16 bfType;
    HI_U32 bfSize;
    HI_U16 bfReserved1;
    HI_U16 bfReserved2;
    HI_U32 bfOffBits;
} HIGFX_BITMAPFILEHEADER;

typedef struct tagHIGFX_BITMAPINFOHEADER
{
    HI_U32 biSize;
    HI_S32 biWidth;
    HI_S32 biHeight;
    HI_U16 biPlanes;
    HI_U16 biBitCount;       /** 每个像素所需的位数, 1: 双色 4: 16色 8: 256色 24: 真彩色 32: 32位真彩 **/
    HI_U32 biCompression;    /** 位图压缩类型, 0: 不压缩 1: BI_RLE8压缩类型 2: BI_RLE4压缩类型        **/
    HI_U32 biSizeImage;      /** 位图的大小, 以字节为单位, 对于BI_RGB必须设置为0                      **/
    HI_S32 biXPelsPerMeter;
    HI_S32 biYPelsPerMeter;
    HI_U32 biClrUsed;
    HI_U32 biClrImportant;
}HIGFX_BITMAPINFOHEADER;

typedef struct BMP_DECODER
{
    HIGFX_IO_INSTANCE_S    stStream;
    HIGFX_BITMAPFILEHEADER FileHeader;
    HIGFX_BITMAPINFOHEADER InfoHeader;
}BMP_DECODER_S;

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

/***************************************************************************************
* func          : BMP_Read_DecData
* description   : CNcomment: 读取解码后的数据 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 BMP_Read_DecData(BMP_DECODER_S *pBmpDec,HI_HANDLE hSurface)
{
    HI_U32 u32Row        = 0;
    HI_U32 u32Col        = 0;
    HI_CHAR *pDstData    = NULL;
    HI_CHAR *pDstPos     = NULL;
    HI_CHAR *pSrcData    = NULL;
    HI_CHAR *pSrcPos     = NULL;
    HI_U32 u32CopyLen    = 0;

    HIGFX_IO_INSTANCE_S *pstStream = NULL;
    HI_GO_SURFACE_S *pstSurface   = (HI_GO_SURFACE_S*)hSurface;

    if (NULL == pBmpDec)
    {
       HI_GFX_Log();
       return HI_FAILURE;
    }

    pstStream = &(pBmpDec->stStream);
    if ( (NULL == pstSurface) || (NULL == pstStream))
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    if (0 == pstSurface->u32Stride[0] || 0 == pstSurface->u32Height[0])
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pDstData = (HI_CHAR*)(pstSurface->pVirAddr[0]);
    pSrcData = (HI_CHAR*)(pstStream->pAddr + pstStream->u32Position);
    if ( (NULL == pDstData) || (NULL == pSrcData))
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    if (pstStream->u32Length < pstStream->u32Position)
    {
        return HI_FAILURE;
    }

#ifdef HI_BUILD_IN_MINI_BOOT
    mmu_cache_enable();
#else
    dcache_enable(0);
#endif

#if 0
    HI_GFX_Print("\n=============================================\n");
    HI_GFX_Print("pstStream->u32Length    = %d\n",  pstStream->u32Length);
    HI_GFX_Print("pstStream->u32Position  = %d pBmpDec->InfoHeader.biBitCount = %d\n",  pstStream->u32Position,pBmpDec->InfoHeader.biBitCount);
    HI_GFX_Print("=============================================\n");
#endif

    for (u32Row = 0; u32Row < pstSurface->u32Height[0]; u32Row++)
    {
        if (pBmpDec->InfoHeader.biHeight > 0)
        {
             pDstPos = pDstData + ((pstSurface->u32Height[0] - 1) - u32Row) * pstSurface->u32Stride[0];
        }
        else
        {
             pDstPos = pDstData + u32Row * pstSurface->u32Stride[0];
        }
        pSrcPos = pSrcData + u32CopyLen;

        if (32 == pBmpDec->InfoHeader.biBitCount)
        {
            for (u32Col = 0; u32Col < pstSurface->u32Width[0]; u32Col++)
            {
                memcpy(pDstPos, pSrcPos, 4);
                pDstPos[3]  = 0xff;
                pDstPos    += 4;
                pSrcPos    += 4;
                u32CopyLen += 4;
            }
        }
        else
        {
            for (u32Col = 0; u32Col < pstSurface->u32Width[0]; u32Col++)
            {
                memcpy(pDstPos, pSrcPos, 3);
                pDstPos[3] = 0xff;
                pDstPos    += 4;
                pSrcPos    += 3;
                u32CopyLen += 3;
            }
        }
    }

#ifdef HI_BUILD_IN_MINI_BOOT
    mmu_cache_disable();
#else
    dcache_disable();
#endif

    return HI_SUCCESS;
}

/*****************************************************************************
* func       : BMP_ParseFileHeader
* description: 解析bmp文件
* retval     :
* others:     :
*****************************************************************************/
static HI_S32 BMP_ParseFileHeader(HIGFX_IO_INSTANCE_S* const pstStream, HIGFX_BITMAPFILEHEADER *pFileHeader)
{
    HI_S32 s32Ret  = HI_SUCCESS;
    HI_U8 Bytes[4] = {0};
    HI_U32 CopyLen = 0, Reserved;
    HI_BOOL EndFlag;

    /** FileType */
    s32Ret = HI_GFX_IORead(pstStream, Bytes, 2, &CopyLen, &EndFlag);
    if ((HI_SUCCESS != s32Ret) || (CopyLen < 2))
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pFileHeader->bfType = Bytes[0] | (Bytes[1] << 8);

    /** FileSize */
    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_DWORD_LEN, &CopyLen, &EndFlag);
    if ((HI_SUCCESS != s32Ret) || (CopyLen < BMP_DWORD_LEN))
    {
       HI_GFX_Log();
        return HI_FAILURE;
    }

    pFileHeader->bfSize = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);

    /** Reserver Validation */
    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_DWORD_LEN, &CopyLen, &EndFlag);
    if ((HI_SUCCESS != s32Ret) || (CopyLen < BMP_DWORD_LEN))
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    Reserved = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);
    if (0 != Reserved)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pFileHeader->bfReserved1 = pFileHeader->bfReserved2 = 0;

    /** OffBits */
    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_DWORD_LEN, &CopyLen, &EndFlag);
    if ((HI_SUCCESS != s32Ret) || (CopyLen < BMP_DWORD_LEN))
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pFileHeader->bfOffBits = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);

    return HI_SUCCESS;
}

HI_S32 BMP_ParseInfoHeader(HIGFX_IO_INSTANCE_S* const pstStream, HIGFX_BITMAPINFOHEADER *pInfoHeader)
{
    HI_S32 s32Ret  = HI_SUCCESS;
    HI_U8 Bytes[4] = {0};
    HI_U32 CopyLen;
    HI_BOOL EndFlag;

    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_DWORD_LEN, &CopyLen, &EndFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pInfoHeader->biSize = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);
    if (pInfoHeader->biSize != 0x28)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    /** ImageWidth */
    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_DWORD_LEN, &CopyLen, &EndFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pInfoHeader->biWidth = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);

    /** ImageHeight*/
    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_DWORD_LEN, &CopyLen, &EndFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pInfoHeader->biHeight = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);

    /** Planes always is 1 */
    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_WORD_LEN, &CopyLen, &EndFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pInfoHeader->biPlanes = Bytes[0] | (Bytes[1] << 8);
    if (1 != pInfoHeader->biPlanes)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    /** BitCount */
    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_WORD_LEN, &CopyLen, &EndFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pInfoHeader->biBitCount = Bytes[0] | (Bytes[1] << 8);

    /** Compression 0-no compress 1- RLE 8  2-RLE 4 3-Bitfields */
    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_DWORD_LEN, &CopyLen, &EndFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pInfoHeader->biCompression = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);

    /** ImageSize */
    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_DWORD_LEN, &CopyLen, &EndFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pInfoHeader->biSizeImage = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);

    /** HResolution */
    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_DWORD_LEN, &CopyLen, &EndFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pInfoHeader->biXPelsPerMeter = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);

    /** VResolution */
    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_DWORD_LEN, &CopyLen, &EndFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pInfoHeader->biYPelsPerMeter = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);

    /** Colors */
    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_DWORD_LEN, &CopyLen, &EndFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pInfoHeader->biClrUsed = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);

    /** Colors Important */
    s32Ret = HI_GFX_IORead(pstStream, Bytes, BMP_DWORD_LEN, &CopyLen, &EndFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pInfoHeader->biClrImportant = Bytes[0] | (Bytes[1] << 8) | (Bytes[2] << 16) | (Bytes[3] << 24);

    return HI_SUCCESS;
}


/***************************************************************************************
* func          : BMP_ParseFile
* description   : CNcomment: 文件解析 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 BMP_ParseFile(BMP_DECODER_S *pDecoder)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = BMP_ParseFileHeader((HIGFX_IO_INSTANCE_S* const)(&(pDecoder->stStream)), &(pDecoder->FileHeader));
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    s32Ret = BMP_ParseInfoHeader((HIGFX_IO_INSTANCE_S* const)(&(pDecoder->stStream)), &(pDecoder->InfoHeader));
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/***************************************************************************************
* func          : BMP_GetPixelFmt
* description   : CNcomment: 获取像素格式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_VOID BMP_GetPixelFmt(const BMP_DECODER_S *pBmpDec, HIGO_PF_E *enPixelFormat)
{
    if ((NULL == pBmpDec) || (NULL == enPixelFormat))
    {
        return;
    }

    switch (pBmpDec->InfoHeader.biBitCount)
    {
        case 24:
        case 32:
            *enPixelFormat = HIGO_PF_8888;
            break;
        default:
            *enPixelFormat = HIGO_PF_BUTT;
    }
}

/***************************************************************************************
* func          : BMP_ADP_CreateDecode
* description   : CNcomment: 创建bmp解码器 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 BMP_ADP_CreateDecode(HI_U32 *pDecHandle,HI_CHAR* pSrcBuf,HI_U32 u32SrcLen)
{

    HI_S32 s32Ret  = HI_SUCCESS;
    BMP_DECODER_S *pBmpDecoder = NULL;

    pBmpDecoder = (BMP_DECODER_S*)HI_GFX_Malloc(sizeof(BMP_DECODER_S),"bmp-adp-handle");
    if (NULL == pBmpDecoder)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }
    memset(pBmpDecoder, 0, sizeof(BMP_DECODER_S));

    s32Ret = HI_GFX_IOCreate(&(pBmpDecoder->stStream),pSrcBuf,u32SrcLen);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        goto ERR1;
    }

    s32Ret = BMP_ParseFile(pBmpDecoder);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        goto ERR2;
    }

    if (0 != pBmpDecoder->InfoHeader.biCompression)
    {/** 只支持非压缩的的bmp文件 **/
        HI_GFX_Log();
        goto ERR2;
    }

    *pDecHandle = (unsigned long)pBmpDecoder;

    return HI_SUCCESS;

ERR2:
    HI_GFX_IODestroy(&(pBmpDecoder->stStream));

ERR1:
    if (NULL != pBmpDecoder)
    {
        HI_GFX_Free((HI_CHAR*)pBmpDecoder);
        pBmpDecoder = NULL;
    }
    *pDecHandle = 0;

    return HI_FAILURE;
}

/***************************************************************************************
* func          : BMP_ADP_StartDecode
* description   : CNcomment:开始解码CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 BMP_ADP_StartDecode(HI_U32 DecHandle, HI_HANDLE hSurface)
{
    HI_S32 s32Ret = HI_SUCCESS;

    BMP_DECODER_S *pBmpDec  = (BMP_DECODER_S*)(unsigned long)DecHandle;
    if (NULL == pBmpDec)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    s32Ret = HI_GFX_IOSeek((HIGFX_IO_INSTANCE_S* const)(&(pBmpDec->stStream)),pBmpDec->FileHeader.bfOffBits);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    s32Ret = BMP_Read_DecData(pBmpDec, hSurface);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}

/***************************************************************************************
* func          : BMP_ADP_GetImgInfo
* description   : CNcomment:获取图片信息CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 BMP_ADP_GetImgInfo(HI_U32 DecHandle, HI_HANDLE hSurface)
{

    BMP_DECODER_S *pBmpDec       = (BMP_DECODER_S*)(unsigned long)DecHandle;
    HI_GO_SURFACE_S *pstSurface  = (HI_GO_SURFACE_S*)hSurface;

    if (NULL == pstSurface || NULL == pBmpDec)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pstSurface->u32Width[0]  = (HI_U32)pBmpDec->InfoHeader.biWidth;
    pstSurface->u32Height[0] = (HI_U32)(pBmpDec->InfoHeader.biHeight);

    if (pBmpDec->InfoHeader.biHeight < 0)
    {
       pstSurface->u32Height[0] = -pBmpDec->InfoHeader.biHeight;
    }
    else
    {
       pstSurface->u32Height[0] = pBmpDec->InfoHeader.biHeight;
    }

    BMP_GetPixelFmt(pBmpDec, &(pstSurface->enPixelFormat));

    if (HIGO_PF_BUTT == pstSurface->enPixelFormat)
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    pstSurface->u32Stride[0] = ((pstSurface->u32Width[0] * 32 + 31) / 32) * 4;
    pstSurface->u32Size[0]   = pstSurface->u32Stride[0] * pstSurface->u32Height[0];

    if ((0 == pstSurface->u32Width[0]) || (0 == pstSurface->u32Height[0]) || (0 == pstSurface->u32Stride[0]))
    {
        HI_GFX_Log();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/***************************************************************************************
* func          : BMP_ADP_DestroyDecoder
* description   : CNcomment:销毁bmp解码器 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 BMP_ADP_DestroyDecode(HI_U32 DecHandle)
{
    BMP_DECODER_S *pBmpDecoder = (BMP_DECODER_S*)(unsigned long)DecHandle;

    if (NULL == pBmpDecoder)
    {
        return HI_SUCCESS;
    }

    HI_GFX_IODestroy(&(pBmpDecoder->stStream));

    HI_GFX_Free((HI_CHAR*)pBmpDecoder);
    pBmpDecoder = NULL;

    return HI_SUCCESS;
}
