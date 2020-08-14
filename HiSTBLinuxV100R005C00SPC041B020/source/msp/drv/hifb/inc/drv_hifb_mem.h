/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_mem.h
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef __DRV_HIFB_MEM_H__
#define __DRV_HIFB_MEM_H__


/*********************************add include here******************************/

#include "hifb.h"
#include "hi_debug.h"

#include "hi_gfx_comm_k.h"
#include "hi_gfx_sys_k.h"
#include "drv_hifb_config.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */


/***************************** Macro Definition ******************************/

/* define debug level */
#define HIFB_FATAL(fmt...)              HI_GFX_COMM_LOG_FATAL(HIGFX_FB_ID,fmt)
#define HIFB_ERROR(fmt...)              HI_GFX_COMM_LOG_ERROR(HIGFX_FB_ID,fmt)
#define HIFB_WARNING(fmt...)            HI_GFX_COMM_LOG_WARNING(HIGFX_FB_ID,fmt)
#define HIFB_INFO(fmt...)               HI_GFX_COMM_LOG_INFO(HIGFX_FB_ID,fmt)

#define HIFB_FILE_PATH_MAX_LEN         256
#define HIFB_FILE_NAME_MAX_LEN         32


/* unit rect */
#define HIFB_UNITE_RECT(stDstRect, stSrcRect) do\
{\
    HIFB_RECT stRect;\
    stRect.x = (stDstRect.x < stSrcRect.x)? stDstRect.x : stSrcRect.x;\
    stRect.y = (stDstRect.y < stSrcRect.y)? stDstRect.y : stSrcRect.y;\
    stRect.w = ((stDstRect.x + stDstRect.w) > (stSrcRect.x + stSrcRect.w))? \
        (stDstRect.x + stDstRect.w - stRect.x) : (stSrcRect.x + stSrcRect.w - stRect.x);\
    stRect.h = ((stDstRect.y + stDstRect.h) > (stSrcRect.y + stSrcRect.h))? \
        (stDstRect.y + stDstRect.h - stRect.y) : (stSrcRect.y + stSrcRect.h - stRect.y);\
    HI_GFX_Memcpy(&stDstRect, &stRect, sizeof(HIFB_RECT));\
}while(0)


#define HIFB_MIN(m, n) (m) > (n) ? (n) : (m)


#define CONFIG_HIFB_DECOMPRESS_HEADER_STRIDE          16
#define CONFIG_HIFB_DECOMPRESS_HEADER_SIZE_ALIGN      256
#define CONFIG_HIFB_DECOMPRESS_DATA_STRIDE_ALIGN      256


/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
HI_VOID *hifb_buf_map(HI_U32 u32PhyAddr);

HI_S32 hifb_buf_ummap(HI_VOID *pViraddr);

HI_VOID hifb_buf_freemem(HI_U32 u32Phyaddr, HI_BOOL bSmmu);
HI_U32 hifb_buf_allocmem(HI_CHAR *pName, HI_CHAR* pZoneName, HI_U32 u32LayerSize);
HI_U32 hifb_buf_map2smmu(HI_U32 u32PhyBuf);
HI_S32 hifb_buf_unmapsmmu(HI_U32 u32SmmuBuf);

HI_BOOL HIFB_IsIntersectRect(const HIFB_RECT* pRect1, const HIFB_RECT* pRect2);

HI_BOOL hifb_iscontain(HIFB_RECT *pstParentRect, HIFB_RECT *pstChildRect);

HI_BOOL hifb_isoverlay(HIFB_RECT *pstSrcRect, HIFB_RECT *pstDstRect);

HI_U32 DRV_HIFB_GetBppByFmt(HIFB_COLOR_FMT_E enDataFmt);

HI_S32 hifb_bitfieldcmp(struct fb_bitfield x, struct fb_bitfield y);


#ifdef CFG_HI_FB_DECOMPRESS_SUPPORT
    #ifdef CONFIG_HIFB_CALCSTRIDE_WITH_ONEPOINTTHREE
       /** decompress stride equal to (NoCmpStride * 1.3)**/
       #define CONIFG_HIFB_GetMaxStride              DRV_HIFB_MEM_GetMaxStrideWithLineCompress
    #else
       #define CONIFG_HIFB_GetMaxStride              DRV_HIFB_MEM_GetMaxStrideWithBlockCompress
    #endif
#else
       #define CONIFG_HIFB_GetMaxStride              DRV_HIFB_MEM_GetMaxStrideWithNoCompress
#endif

/******************************* API release ************************************/
#ifndef HI_BUILD_IN_BOOT

#ifdef CFG_HI_FB_DECOMPRESS_SUPPORT

#ifdef CONFIG_HIFB_CALCSTRIDE_WITH_ONEPOINTTHREE
/***************************************************************************************
* func          : DRV_HIFB_MEM_GetMaxStrideWithLineCompress
* description   : CNcomment: get max stride between noCmpStride and CmpStride CNend\n
*                 CompressStide:  TmpStride: Align(width * PixDepth * 1.3, 256)
* param[in]     : u32Width
* param[in]     : u32PixDepth
* param[in]     : pCmpStride
* param[in]     : u32Align
* retval        : NA
* others:       : NA
***************************************************************************************/
static inline HI_U32 DRV_HIFB_MEM_GetMaxStrideWithLineCompress(HI_U32 u32Width, HI_U32 u32PixDepth, HI_U32 *pCmpStride, HI_U32 u32Align)
{
     HI_U32 NoCmpStride = (u32Width * u32PixDepth + u32Align - 1) & (~(u32Align - 1));

     if (NULL == pCmpStride)
     {
        return NoCmpStride;
     }

     *pCmpStride = (4 != u32PixDepth) ? (0) : (((HI_U32)(((u32Width * u32PixDepth) * 13 + 9)/10) + (u32Align - 1)) & (~(u32Align - 1)));

     return (NoCmpStride > *pCmpStride) ? (NoCmpStride) : (*pCmpStride);
}
#else
/***************************************************************************************
* func          : DRV_HIFB_MEM_GetMaxStrideWithBlockCompress
* description   : CNcomment: get max stride between noCmpStride and CmpStride CNend\n
*                 CompressStide:  TmpStride: Align(width,256)
*                                 if (odd == TmpStride % 256)
*                                    return TmpStride
*                                 else
*                                    return TmpStride + 256;
* param[in]     : u32Width
* param[in]     : u32PixDepth
* param[in]     : pCmpStride
* param[in]     : u32Align
* retval        : NA
* others:       : NA
***************************************************************************************/
static inline HI_U32 DRV_HIFB_MEM_GetMaxStrideWithBlockCompress(HI_U32 u32Width, HI_U32 u32PixDepth, HI_U32 *pCmpStride, HI_U32 u32Align)
{
     HI_U32 NoCmpStride = (u32Width * u32PixDepth + u32Align - 1) & (~(u32Align - 1));
     HI_U32 DispBufferStride = 0;

     if (NULL == pCmpStride)
     {
        return NoCmpStride;
     }

     if (4 != u32PixDepth)
     {
         *pCmpStride = 0;
     }
     else
     {
         *pCmpStride = ((u32Width * u32PixDepth / 2) + CONFIG_HIFB_DECOMPRESS_DATA_STRIDE_ALIGN - 1) & (~(CONFIG_HIFB_DECOMPRESS_DATA_STRIDE_ALIGN - 1));
         *pCmpStride = (*pCmpStride % (CONFIG_HIFB_DECOMPRESS_DATA_STRIDE_ALIGN * 2)) ? (*pCmpStride) : (*pCmpStride + CONFIG_HIFB_DECOMPRESS_DATA_STRIDE_ALIGN);
         DispBufferStride = ((CONFIG_HIFB_DECOMPRESS_HEADER_SIZE_ALIGN * 2 + *pCmpStride * 2)  + u32Align - 1) & (~(u32Align - 1));
     }

     return (NoCmpStride > DispBufferStride) ? (NoCmpStride) : (DispBufferStride);
}
#endif

#else
static inline HI_U32 DRV_HIFB_MEM_GetMaxStrideWithNoCompress(HI_U32 u32Width, HI_U32 u32PixDepth, HI_U32 *pCmpStride, HI_U32 u32Align)
{
     *pCmpStride = (u32Width * u32PixDepth + u32Align - 1) & (~(u32Align - 1));
     return *pCmpStride;
}
#endif


/***************************************************************************************
* func          : DRV_HIFB_MEM_GetHeadStride
* description   : CNcomment: calc head stride CNend\n
                  AR head size:  (16 * height + 256 - 1) & (~(256 - 1));
* param[in]     : u32Stride
* param[in]     : u32Height
* retval        : NA
* others:       : NA
***************************************************************************************/
static inline HI_VOID DRV_HIFB_MEM_GetCmpHeadInfo(HI_U32 Height, HI_U32 *pHeadSize, HI_U32 *pHeadStride)
{
    if (NULL != pHeadStride)
    {
       *pHeadStride = CONFIG_HIFB_DECOMPRESS_HEADER_STRIDE;
    }

    if (NULL != pHeadSize)
    {
       *pHeadSize = (CONFIG_HIFB_DECOMPRESS_HEADER_STRIDE * Height + CONFIG_HIFB_DECOMPRESS_HEADER_SIZE_ALIGN - 1) & (~(CONFIG_HIFB_DECOMPRESS_HEADER_SIZE_ALIGN - 1));
    }
    return;
}


/***************************************************************************************
* func          : HI_HIFB_GetMemSize
* description   : CNcomment: calc memory size CNend\n
                  AR head size:  (16 * height + 256 - 1) & (~(256 - 1));
                  AR data size:  CmpStride * height;
                  GB head size:  (16 * height + 256 - 1) & (~(256 - 1));
                  GB data size:  CmpStride * height;
* param[in]     : u32Stride
* param[in]     : u32Height
* retval        : NA
* others:       : NA
***************************************************************************************/
static inline HI_U32 HI_HIFB_GetMemSize(HI_U32 u32Stride,HI_U32 u32Height)
{
    HI_U32 TotalSize = u32Stride *  u32Height;
    return ((TotalSize + PAGE_SIZE - 1) & (~(PAGE_SIZE - 1)));
}
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
