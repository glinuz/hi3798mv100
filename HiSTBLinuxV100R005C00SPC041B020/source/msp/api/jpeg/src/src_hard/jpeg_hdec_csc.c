/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_csc.c
Version          : Initial Draft
Author           : y00181162
Created          : 2016/05/09
Description      :
Function List    :


History          :
Date                Author                Modification
2016/05/09          y00181162            Created file
******************************************************************************/

/***************************** add include here*********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_jpeg_config.h"
#include "jpeg_hdec_adp.h"
#include "hi_gfx_sys.h"

#ifndef CONFIG_JPEG_CSC_DISABLE
#include  "jpeg_hdec_csc.h"

#if   defined(CHIP_TYPE_hi3712)      \
   || defined(CHIP_TYPE_hi3716m)
#include  "hi_tde_ioctl.h"
#else
#include  "hi_drv_tde.h"
#endif


/***************************** Macro Definition ********************************/


/***************************** Structure Definition ****************************/

/***************************** Global Variable declaration *********************/
#ifndef CONFIG_JPEG_CSC_DISABLE
extern JPEG_DECOMPRESS_RES gs_stJpegDecompressRes;
#endif
/***************************** API forward declarations ************************/

static TDE2_COLOR_FMT_E JPEG_HDEC_ConvertJpegFmtToTdeFmt(j_decompress_ptr cinfo);


/***************************** API realization *********************************/


#ifdef CONFIG_JPEG_SOFTCSC_ENABLE

static HI_VOID ycc2rgb(HI_U8 y, HI_U8 cb, HI_U8 cr, HI_U8 *r, HI_U8 *g, HI_U8 *b)
{
    *r = y               + 1.40200 * cr;
    *g = y - 0.34414 * cb - 0.71414 * cr;
    *b = y + 1.77200 * cb;
}

static HI_VOID JPEG_HDEC_YUV400TORGB(j_decompress_ptr cinfo)
{
    HI_U8 r  = 0;
    HI_U8 g  = 0;
    HI_U8 b  = 0;
    HI_U8 y  = 0;
    HI_U8 cb = 0;
    HI_U8 cr = 0;
    HI_U32 u32Red   = 0;
    HI_U32 u32Green = 1;
    HI_U32 u32Blue  = 2;
    HI_S32 s32Row   = 0;
    HI_S32 s32Col   = 0;
    HI_U32 u32Count = 0;
    HI_S32 s32YRowStride   = 0;
    HI_S32 s32UVRowStride  = 0;
    HI_S32 s32DstRowStride = 0;
    HI_S32 s32YOffset      = 0;
    HI_S32 s32UVOffset     = 0;
    HI_S32 s32DstOffset    = 0;
    HI_CHAR* pYSrcBuf      = NULL;
    HI_CHAR* pUVSrcBuf     = NULL;
    HI_CHAR* pDtsSrcBuf    = NULL;
    JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

    pYSrcBuf    = pJpegHandle->stMiddleSurface.pMiddleVir[0];
    pUVSrcBuf   = pJpegHandle->stMiddleSurface.pMiddleVir[1];
    pDtsSrcBuf  = pJpegHandle->stMiddleSurface.pOutVir;

    s32YRowStride     = pJpegHandle->stJpegSofInfo.u32YStride;
    s32UVRowStride    = pJpegHandle->stJpegSofInfo.u32CbCrStride;
    s32DstRowStride      = pJpegHandle->stJpegSofInfo.u32DisplayStride;

    for(s32Row = 0; s32Row < cinfo->output_height; s32Row++){
        u32Count      = 0;
        s32YOffset    = s32YRowStride   * s32Row;
        s32UVOffset   = s32UVRowStride  * s32Row;
        s32DstOffset  = s32DstRowStride * s32Row;
        for(s32Col = 0; s32Col < cinfo->output_width; s32Col++){
            y  = pYSrcBuf[s32YOffset + s32Col];
            cb = 0x80;
            cr = 0x80;
            ycc2rgb(y,cb,cr,&r,&g,&b);
            switch(cinfo->out_color_space){
                case JCS_RGB:
                    pDtsSrcBuf[s32DstOffset + 3 * u32Count + u32Red]   = r;
                    pDtsSrcBuf[s32DstOffset + 3 * u32Count + u32Green] = g;
                    pDtsSrcBuf[s32DstOffset + 3 * u32Count + u32Blue]  = b;
                    u32Count++;
                    break;
                case JCS_BGR:
                    pDtsSrcBuf[s32DstOffset + 3 * u32Count + u32Blue]   = r;
                    pDtsSrcBuf[s32DstOffset + 3 * u32Count + u32Green]  = g;
                    pDtsSrcBuf[s32DstOffset + 3 * u32Count + u32Red]    = b;
                    u32Count++;
                    break;
                case JCS_ARGB_8888:
                    break;
                case JCS_ABGR_8888:
                    break;
                case JCS_ARGB_1555:
                    break;
                case JCS_ABGR_1555:
                    break;
                case JCS_RGB_565:
                    break;
                case JCS_BGR_565:
                    break;
                default:
                    break;
            }
        }
    }
}

static HI_VOID JPEG_HDEC_YUV420TORGB(j_decompress_ptr cinfo)
{


}


static HI_VOID JPEG_HDEC_YUV444TORGB(j_decompress_ptr cinfo)
{


}

static HI_VOID JPEG_HDEC_YUV42212TORGB(j_decompress_ptr cinfo)
{


}

static HI_VOID JPEG_HDEC_YUV42221TORGB(j_decompress_ptr cinfo)
{


}

HI_S32 JPEG_HDEC_SoftCSC(j_decompress_ptr cinfo)
{
    JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

    #ifdef  CONFIG_JPEG_HARDDEC2ARGB
        if(HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB){
            pJpegHandle->bCSCEnd = HI_TRUE;
            JPEG_HDEC_CalcOutputComponents(cinfo);
            return HI_SUCCESS;
        }
    #endif

    if((HI_TRUE == pJpegHandle->bOutYCbCrSP)||(HI_TRUE == pJpegHandle->bCSCEnd)){
            return HI_SUCCESS;
    }

    switch(pJpegHandle->enImageFmt){
        case JPEG_FMT_YUV400:
            JPEG_HDEC_YUV400TORGB(cinfo);
            break;
        case JPEG_FMT_YUV420:
            JPEG_HDEC_YUV420TORGB(cinfo);
            break;
        case JPEG_FMT_YUV444:
            JPEG_HDEC_YUV444TORGB(cinfo);
            break;
        case JPEG_FMT_YUV422_12:
            JPEG_HDEC_YUV42212TORGB(cinfo);
            break;
        case JPEG_FMT_YUV422_21:
            JPEG_HDEC_YUV42221TORGB(cinfo);
            break;
        default:
            return HI_FAILURE;
    }

    switch(cinfo->out_color_space){
        case JCS_RGB:
        case JCS_BGR:
            cinfo->output_components = 3;
            break;
        case JCS_ARGB_8888:
        case JCS_ABGR_8888:
            cinfo->output_components = 4;
            break;
        case JCS_ARGB_1555:
        case JCS_ABGR_1555:
        case JCS_RGB_565:
        case JCS_BGR_565:
            cinfo->output_components = 2;
            break;
        default:
            return HI_FAILURE;
    }

    pJpegHandle->bCSCEnd = HI_TRUE;

    return HI_SUCCESS;
}

#else


HI_S32 JPEG_HDEC_HardCSC(j_decompress_ptr cinfo)
{
#ifndef CONFIG_JPEG_CSC_DISABLE
      TDE2_MB_S        SrcSurface;
      TDE2_SURFACE_S    DstSurface;
      TDE2_RECT_S  SrcRect,DstRect;

      TDE2_MBOPT_S  stMbOpt;
      TDE_HANDLE s32Handle;
      HI_S32 s32Ret           =  HI_SUCCESS;

      TDE2_MB_COLOR_FMT_E enMbFmt[6] = {
          TDE2_MB_COLOR_FMT_JPG_YCbCr400MBP,
          TDE2_MB_COLOR_FMT_JPG_YCbCr420MBP,
          TDE2_MB_COLOR_FMT_JPG_YCbCr422MBHP,
          TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP,
          TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP,
          TDE2_MB_COLOR_FMT_BUTT,
      };

      JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

#ifdef  CONFIG_JPEG_HARDDEC2ARGB
      if (HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB)
      {
          pJpegHandle->bCSCEnd = HI_TRUE;
          JPEG_HDEC_CalcOutputComponents(cinfo);
          return HI_SUCCESS;
      }
#endif

      if ( (HI_TRUE == pJpegHandle->bOutYCbCrSP)||(HI_TRUE == pJpegHandle->bCSCEnd))
      {
          return HI_SUCCESS;
      }

      SrcSurface.u32YPhyAddr    = pJpegHandle->stMiddleSurface.u32MiddlePhy[0];
      SrcSurface.u32CbCrPhyAddr = pJpegHandle->stMiddleSurface.u32MiddlePhy[1];
      SrcSurface.enMbFmt        = enMbFmt[pJpegHandle->enImageFmt];
      SrcSurface.u32YWidth      = pJpegHandle->stJpegSofInfo.u32DisplayW;
      SrcSurface.u32YHeight     = pJpegHandle->stJpegSofInfo.u32DisplayH;
      SrcSurface.u32YStride     = pJpegHandle->stJpegSofInfo.u32YStride;
      SrcSurface.u32CbCrStride  = pJpegHandle->stJpegSofInfo.u32CbCrStride;

#if 0
      JPEG_TRACE("============================================================================\n");
      /** crop debug **/
      JPEG_TRACE("SrcSurface.u32YWidth      = %d\n",SrcSurface.u32YWidth);
      JPEG_TRACE("SrcSurface.u32YHeight     = %d\n",SrcSurface.u32YHeight);
      JPEG_TRACE("SrcSurface.u32YStride     = %d\n",SrcSurface.u32YStride);
      JPEG_TRACE("SrcSurface.u32CbCrStride  = %d\n",SrcSurface.u32CbCrStride);
      JPEG_TRACE("============================================================================\n");
#endif

      JPEG_HDEC_CalcOutputComponents(cinfo);
      DstSurface.enColorFmt = JPEG_HDEC_ConvertJpegFmtToTdeFmt(cinfo);

      DstSurface.u32PhyAddr = pJpegHandle->stMiddleSurface.u32OutPhy;

      if (HI_TRUE == pJpegHandle->stOutDesc.stOutSurface.bUserPhyMem)
      {
          DstSurface.u32Stride = pJpegHandle->stOutDesc.stOutSurface.u32OutStride[0];
      }
      else
      {
          DstSurface.u32Stride = pJpegHandle->stJpegSofInfo.u32DisplayStride;
      }
      DstSurface.u32Width       = (HI_U32)pJpegHandle->stOutDesc.stCropRect.w;
      DstSurface.u32Height      = (HI_U32)pJpegHandle->stOutDesc.stCropRect.h;
      DstSurface.pu8ClutPhyAddr = NULL;
      DstSurface.bYCbCrClut     = HI_FALSE;
      DstSurface.bAlphaMax255   = HI_TRUE;
      DstSurface.bAlphaExt1555  = HI_TRUE;
      DstSurface.u8Alpha0       = 0;
      DstSurface.u8Alpha1       = 255;
      DstSurface.u32CbCrPhyAddr = 0;
      DstSurface.u32CbCrStride  = 0;

      SrcRect.s32Xpos   = pJpegHandle->stOutDesc.stCropRect.x;
      SrcRect.s32Ypos   = pJpegHandle->stOutDesc.stCropRect.y;
      SrcRect.u32Width  = (HI_U32)pJpegHandle->stOutDesc.stCropRect.w;
      SrcRect.u32Height = (HI_U32)pJpegHandle->stOutDesc.stCropRect.h;
      DstRect.s32Xpos   = 0;
      DstRect.s32Ypos   = 0;
      DstRect.u32Width  = (HI_U32)pJpegHandle->stOutDesc.stCropRect.w;
      DstRect.u32Height = (HI_U32)pJpegHandle->stOutDesc.stCropRect.h;

#if 0
      JPEG_TRACE("============================================================================\n");
      /** crop debug **/
      JPEG_TRACE("DstSurface.u32Width  = %d\n",DstSurface.u32Width);
      JPEG_TRACE("DstSurface.u32Height = %d\n",DstSurface.u32Height);
      JPEG_TRACE("DstSurface.u32Stride = %d\n",DstSurface.u32Stride);
      JPEG_TRACE("DstRect.s32Xpos      = %d\n",DstRect.s32Xpos);
      JPEG_TRACE("DstRect.s32Ypos      = %d\n",DstRect.s32Ypos);
      JPEG_TRACE("DstRect.u32Width     = %d\n",DstRect.u32Width);
      JPEG_TRACE("DstRect.u32Height    = %d\n",DstRect.u32Height);
      JPEG_TRACE("============================================================================\n");
#endif

      /**
      **这个操作性能会变差，但是消告警
      **/
      HI_GFX_Memset(&stMbOpt,0,sizeof(TDE2_MBOPT_S));
      stMbOpt.enResize   = TDE2_MBRESIZE_QUALITY_LOW;
      //stMbOpt.bDeflicker = HI_TRUE;

      if ((s32Handle = JPEG_HDEC_CSC_BeginJob(gs_stJpegDecompressRes.s32CscDev)) != HI_ERR_TDE_INVALID_HANDLE)
      {
          s32Ret = JPEG_HDEC_CSC_MbBlit(s32Handle, &SrcSurface, &SrcRect, &DstSurface, &DstRect, &stMbOpt,gs_stJpegDecompressRes.s32CscDev);
          if (HI_SUCCESS != s32Ret)
          {
              JPEG_TRACE("==== JPEG_HDEC_CSC_MbBlit Failure,s32Ret = 0x%x!\n",s32Ret);
              return HI_FAILURE;
          }
          /**
          ** if HI_TRUE,is no sync. and HI_FALSE you should call tde wait for done to
          ** waite the tde work finish.
          ** CNcomment:HI_TRUE 阻塞，要是非阻塞要调用waitfordone等待TDE操作完成 CNend\n
          **/
          s32Ret = JPEG_HDEC_CSC_EndJob(s32Handle, HI_FALSE, HI_TRUE, 10000,gs_stJpegDecompressRes.s32CscDev);
          if (HI_SUCCESS != s32Ret)
          {
              JPEG_TRACE("==== JPEG_HDEC_CSC_EndJob Failure,s32Ret = 0x%x!\n",s32Ret);
              return HI_FAILURE;
          }
      }

      pJpegHandle->bCSCEnd = HI_TRUE;

#ifdef CONFIG_JPEG_TEST_SAVE_BMP_PIC
      HI_JPEG_SaveBmp(DstSurface.u32PhyAddr, \
                      DstRect.u32Width,      \
                      DstRect.u32Height,     \
                      DstSurface.u32Stride,  \
                      cinfo);
#endif


#else
      #ifdef  CONFIG_JPEG_HARDDEC2ARGB
      JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
      if (HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB)
      {
          pJpegHandle->bCSCEnd = HI_TRUE;
      }
      #endif
#endif
      return HI_SUCCESS;


}


#ifdef CONFIG_GFX_LIB_TURBO

static TDE2_COLOR_FMT_E JPEG_HDEC_ConvertJpegFmtToTdeFmt(j_decompress_ptr cinfo)
{
    switch(cinfo->out_color_space){
        case JCS_EXT_RGBA:
            return TDE2_COLOR_FMT_ABGR8888;
        case JCS_EXT_BGRA:
            return TDE2_COLOR_FMT_ARGB8888;
        case JCS_RGB:
        case JCS_EXT_RGB:
            return TDE2_COLOR_FMT_BGR888;
        case JCS_EXT_BGR:
            return TDE2_COLOR_FMT_RGB888;
        case JCS_RGB565:
              return TDE2_COLOR_FMT_RGB565;
        default:
            return TDE2_COLOR_FMT_BUTT;
    }
}

#else

static TDE2_COLOR_FMT_E JPEG_HDEC_ConvertJpegFmtToTdeFmt(j_decompress_ptr cinfo)
{
    switch(cinfo->out_color_space){
        case JCS_RGB:
            return TDE2_COLOR_FMT_BGR888;
        case JCS_BGR:
            return TDE2_COLOR_FMT_RGB888;
        case JCS_ARGB_8888:
    #ifdef CONFIG_JPEG_ADD_GOOGLEFUNCTION
        case JCS_RGBA_8888:
    #endif
            return TDE2_COLOR_FMT_ABGR8888;
        case JCS_ABGR_8888:
            return TDE2_COLOR_FMT_ARGB8888;
        case JCS_ARGB_1555:
            return TDE2_COLOR_FMT_ABGR1555;
        case JCS_ABGR_1555:
            return TDE2_COLOR_FMT_ARGB1555;
        case JCS_RGB_565:
        #ifdef CONFIG_JPEG_LITTLE_TRANSFORM_BIG_ENDIAN
              return TDE2_COLOR_FMT_RGB565;
        #else
              return TDE2_COLOR_FMT_BGR565;
        #endif
        case JCS_BGR_565:
            return TDE2_COLOR_FMT_RGB565;
        case JCS_CrCbY:
            return TDE2_COLOR_FMT_YCbCr888;
        default:
            return TDE2_COLOR_FMT_BUTT;
    }
}


#endif

#endif


/*****************************************************************************
* func            : JPEG_HDEC_CSC_Open
* description    : Open the csc device
                  CNcomment: CSC转换设备打开 CNend\n
* retval        : HI_SUCCESS    CNcomment: 成功       CNend\n
* retval        : HI_FAILURE    CNcomment: 失败       CNend\n
* others:        : NA
*****************************************************************************/
HI_S32 JPEG_HDEC_CSC_Open()
{

    HI_S32 s32CscDev = -1;
    s32CscDev = open(CSC_DEV, O_RDWR, 0);
    if (s32CscDev < 0){
        return HI_ERR_TDE_DEV_OPEN_FAILED;
    }
    return s32CscDev;
}

/*****************************************************************************
* func            : JPEG_HDEC_CSC_Close
* description    : close the csc device
                  CNcomment: CSC转换设备关闭 CNend\n
* param[in]        : s32CscDev     CNcomment: CSC 设备   CNend\n
* retval        : HI_SUCCESS    CNcomment: 成功       CNend\n
* retval        : HI_FAILURE    CNcomment: 失败       CNend\n
* others:        : NA
*****************************************************************************/
HI_VOID JPEG_HDEC_CSC_Close(HI_S32 s32CscDev)
{
    close(s32CscDev);
}

/*****************************************************************************
* func            : JPEG_HDEC_CSC_BeginJob
* description    : create csc task
                  CNcomment: 创建CSC任务 CNend\n
* param[in]        : s32CscDev     CNcomment: CSC 设备   CNend\n
* retval        : HI_SUCCESS    CNcomment: 成功       CNend\n
* retval        : HI_FAILURE    CNcomment: 失败       CNend\n
* others:        : NA
*****************************************************************************/
HI_S32 JPEG_HDEC_CSC_BeginJob(HI_S32 s32CscDev)
{
    TDE_HANDLE s32Handle;
    if(s32CscDev < 0){
        return HI_FAILURE;
    }
    if (ioctl(s32CscDev, TDE_BEGIN_JOB, &s32Handle) < 0){
        return HI_ERR_TDE_INVALID_HANDLE;
    }
    return s32Handle;
}

/*****************************************************************************
* func            : JPEG_HDEC_CSC_MbBlit
* description    : run csc task
                  CNcomment: 执行任务 CNend\n
* param[in]        : s32CscDev     CNcomment: CSC 设备   CNend\n
* retval        : HI_SUCCESS    CNcomment: 成功       CNend\n
* retval        : HI_FAILURE    CNcomment: 失败       CNend\n
* others:        : NA
*****************************************************************************/
HI_S32 JPEG_HDEC_CSC_MbBlit(TDE_HANDLE s32Handle, TDE2_MB_S* pstMB, TDE2_RECT_S  *pstMbRect, TDE2_SURFACE_S* pstDst,TDE2_RECT_S  *pstDstRect,TDE2_MBOPT_S* pstMbOpt,HI_S32 s32CscDev)
{


    TDE_MBBITBLT_CMD_S stMbBlit;

    if(s32CscDev < 0){
        return HI_FAILURE;
    }
    if ((NULL == pstMB) || (NULL == pstDst) || (NULL == pstMbOpt)){
        return HI_ERR_TDE_NULL_PTR;
    }
    HI_GFX_Memset(&stMbBlit,0,sizeof(TDE_MBBITBLT_CMD_S));
    stMbBlit.s32Handle = s32Handle;
    HI_GFX_Memcpy(&stMbBlit.stMB, pstMB, sizeof(TDE2_MB_S));
    HI_GFX_Memcpy(&stMbBlit.stMbRect, pstMbRect, sizeof(TDE2_RECT_S));
    HI_GFX_Memcpy(&stMbBlit.stDstRect, pstDstRect, sizeof(TDE2_RECT_S));
    HI_GFX_Memcpy(&stMbBlit.stMbOpt, pstMbOpt, sizeof(TDE2_MBOPT_S));
    /** Support 64bit,don't use HI_GFX_Memcpy **/
    #if 0
    HI_GFX_Memcpy(&stMbBlit.stDst, pstDst, sizeof(TDE2_SURFACE_S));
    #else
    stMbBlit.stDst.bAlphaExt1555  = pstDst->bAlphaExt1555;
    stMbBlit.stDst.bAlphaMax255   = pstDst->bAlphaMax255;
    stMbBlit.stDst.bYCbCrClut     = pstDst->bYCbCrClut;
    stMbBlit.stDst.enColorFmt     = pstDst->enColorFmt;
    stMbBlit.stDst.u32ClutPhyAddr = 0;
    stMbBlit.stDst.u32CbCrPhyAddr = pstDst->u32CbCrPhyAddr;
    stMbBlit.stDst.u32CbCrStride  = pstDst->u32CbCrStride;
    stMbBlit.stDst.u32Height      = pstDst->u32Height;
    stMbBlit.stDst.u32PhyAddr     = pstDst->u32PhyAddr;
    stMbBlit.stDst.u32Stride      = pstDst->u32Stride;
    stMbBlit.stDst.u32Width       = pstDst->u32Width;
    stMbBlit.stDst.u8Alpha0       = pstDst->u8Alpha0;
    stMbBlit.stDst.u8Alpha1       = pstDst->u8Alpha1;
    #endif

    return ioctl(s32CscDev, TDE_MB_BITBLT, &stMbBlit);

}


/*****************************************************************************
* func            : JPEG_HDEC_CSC_EndJob
* description    : submit csc task
                  CNcomment: 提交任务 CNend\n
* param[in]        : s32CscDev     CNcomment: CSC 设备   CNend\n
* retval        : HI_SUCCESS    CNcomment: 成功       CNend\n
* retval        : HI_FAILURE    CNcomment: 失败       CNend\n
* others:        : NA
*****************************************************************************/
HI_S32 JPEG_HDEC_CSC_EndJob(TDE_HANDLE s32Handle, HI_BOOL bSync, HI_BOOL bBlock, HI_U32 u32TimeOut,HI_S32 s32CscDev)
{

    TDE_ENDJOB_CMD_S stEndJob;

    if(s32CscDev < 0){
        return HI_FAILURE;
    }
    /* Disable sync function */
    bSync = HI_FALSE;

    stEndJob.s32Handle  = s32Handle;
    stEndJob.bSync        = bSync;  /** 没用 **/
    stEndJob.bBlock     = bBlock; /** 同步 **/
    stEndJob.u32TimeOut = u32TimeOut;

    return ioctl(s32CscDev, TDE_END_JOB, &stEndJob);

}
#endif
