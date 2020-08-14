/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_parse.c
Version          : Initial Draft
Author           : y00181162
Created          : 2016/05/09
Description      : parse jpeg information CNend\n
Function List    :


History          :
Date                Author                Modification
2016/05/09          y00181162            Created file
******************************************************************************/

/***************************** add include here*********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hi_jpeg_config.h"
#include "jpeg_hdec_adp.h"
#include "jpeg_hdec_api.h"
#include "jpeg_hdec_mem.h"

/***************************** Macro Definition ********************************/
#define JPEG_ALIGNED_SCALE(x, i)    ( ( (x) + (1 << (i)) - 1 ) >> (i) )
#define JPEG_ROUND_UP(a,b)          ( ((a) + (b) - (1L)) / (b) )

/***************************** Structure Definition ****************************/

/***************************** Global Variable declaration *********************/

/***************************** API forward declarations ************************/

static inline HI_BOOL JPEG_HDEC_CheckDecXRGBSupport    (const struct jpeg_decompress_struct *cinfo);
static inline HI_VOID JPEG_HDEC_SetOutColorSapceMarker (const struct jpeg_decompress_struct *cinfo);
static inline HI_VOID JPEG_HDEC_CalcScale              (const struct jpeg_decompress_struct *cinfo);
static inline HI_VOID JPEG_HDEC_GetOutSize(const HI_U32 u32Ration,const HI_U32 u32InWidth, const HI_U32 u32InHeight,HI_U32 *pu32OutWidth,HI_U32 *pu32OutHeight);

static HI_VOID JPEG_HDEC_CalcOutColorComponents(j_decompress_ptr cinfo);
static HI_VOID JPEG_HDEC_CalcOutStride(j_decompress_ptr cinfo,HI_U32 u32DecWidth,HI_U32 u32DisplayWidth);
static inline HI_VOID JPEG_HDEC_GetCropRect(const struct jpeg_decompress_struct *cinfo);
static inline HI_VOID JPEG_HDEC_GetCalcOutXRGBSize(const struct jpeg_decompress_struct *cinfo, HI_U32 *pYSize, HI_U32 *pCSize);
static inline HI_VOID JPEG_HDEC_GetCropSize(const struct jpeg_decompress_struct *cinfo);
static inline HI_VOID JPEG_HDEC_GetUVResolutionForYuv420sp(const struct jpeg_decompress_struct *cinfo, HI_U32 YHeightTmp, HI_U32 YStride, HI_U32 *pUVHeightTmp, HI_U32 *pUVStride);

/***************************** API realization *********************************/

/*****************************************************************************
* func            : JPEG_HDEC_GetImagInfo
* description    : get jpeg picture information
                  CNcomment:  获取图片信息 CNend\n
* param[in]     : cinfo       CNcomment: 解码对象    CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID JPEG_HDEC_GetImagInfo(j_decompress_ptr cinfo)
{
      HI_U32 InputMcuWidth   = 0;
      HI_U32 InputMcuHeight  = 0;
      HI_U32 DecWidth        = 0;
      HI_U32 DecHeight       = 0;
      HI_U32 YWidthTmp       = 0;
      HI_U32 YHeightTmp      = 0;
      HI_U32 CHeightTmp      = 0;
      HI_U32 u32YStride      = 0;
      HI_U32 u32UVStride     = 0;
      HI_U32 u32YWidth       = 0;
      HI_U32 u32YHeight      = 0;
      HI_U32 u32UVWidth      = 0;
      HI_U32 u32UVHeight     = 0;
      HI_U32 u32StrideNeedWidth  = 0;
      #if defined(CONFIG_JPEG_OUTPUT_CROP) && defined(CONFIG_JPEG_HARDDEC2ARGB)
      HI_U32 u32DecSize      = 0;
      #endif
      HI_U32 u32YSize        = 0;
      HI_U32 u32CSize        = 0;

      JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

      JPEG_HDEC_GetCropSize(cinfo);

      if (HI_TRUE == pJpegHandle->stJpegSofInfo.bCalcSize)
      {
          return;
      }

      JPEG_HDEC_CalcScale(cinfo);

      JPEG_HDEC_GetOutSize(pJpegHandle->u32ScalRation,cinfo->image_width,cinfo->image_height,
                           &(cinfo->output_width),&(cinfo->output_height));

      JPEG_HDEC_SetOutColorSapceMarker(cinfo);

      HI_GFX_GetStride(cinfo->output_width,&u32YStride,JPGD_HDEC_MMZ_YUVSP_STRIDE_ALIGN);

      u32YWidth  = (cinfo->output_width <= 1)  ? (2) : (cinfo->output_width);
      u32YHeight = (cinfo->output_height <= 1) ? (2) : (cinfo->output_height);

      if (1 == pJpegHandle->u8Fac[0][0])
      {
           InputMcuWidth  = (cinfo->image_width  + JPEG_MCU_8ALIGN - 1) >> 3;
           YWidthTmp      = (cinfo->output_width  + JPEG_MCU_8ALIGN - 1) & (~(JPEG_MCU_8ALIGN - 1));
           DecWidth       = (cinfo->image_width   + JPEG_MCU_8ALIGN - 1) & (~(JPEG_MCU_8ALIGN - 1));
      }
      else
      {
           InputMcuWidth  = (cinfo->image_width  + JPEG_MCU_16ALIGN - 1) >> 4;
           YWidthTmp      = (cinfo->output_width  + JPEG_MCU_16ALIGN - 1) & (~(JPEG_MCU_16ALIGN - 1));
           DecWidth       = (cinfo->image_width   + JPEG_MCU_16ALIGN - 1) & (~(JPEG_MCU_16ALIGN - 1));
      }

      if (1 == pJpegHandle->u8Fac[0][1])
      {
           InputMcuHeight = (cinfo->image_height + JPEG_MCU_8ALIGN - 1) >> 3;
           YHeightTmp     = (cinfo->output_height + JPEG_MCU_8ALIGN - 1) & (~(JPEG_MCU_8ALIGN - 1));
           DecHeight      = (cinfo->image_height  + JPEG_MCU_8ALIGN - 1) & (~(JPEG_MCU_8ALIGN - 1));
      }
      else
      {
           InputMcuHeight = (cinfo->image_height + JPEG_MCU_16ALIGN - 1) >> 4;
           YHeightTmp     = (cinfo->output_height + JPEG_MCU_16ALIGN - 1) & (~(JPEG_MCU_16ALIGN - 1));
           DecHeight      = (cinfo->image_height  + JPEG_MCU_16ALIGN - 1) & (~(JPEG_MCU_16ALIGN - 1));
      }

      switch (pJpegHandle->enImageFmt){
          case JPEG_FMT_YUV420:
              CHeightTmp  = YHeightTmp >> 1;
              u32UVStride = u32YStride;
              u32YWidth   = (u32YWidth  >> 1) << 1;
              u32YHeight  = (u32YHeight >> 1) << 1;
              u32UVWidth  = u32YWidth  >> 1;
              u32UVHeight = u32YHeight >> 1;
              break;
          case JPEG_FMT_YUV422_21:
              CHeightTmp  = YHeightTmp;
              u32UVStride = u32YStride;
              JPEG_HDEC_GetUVResolutionForYuv420sp(cinfo,YHeightTmp,u32YStride,&CHeightTmp,&u32UVStride);
              u32YWidth    = (u32YWidth  >> 1) << 1;
              u32UVWidth   = u32YWidth >> 1;
              u32UVHeight  = u32YHeight;
              break;
          case JPEG_FMT_YUV422_12:
              CHeightTmp  = YHeightTmp>>1;
              u32UVStride = u32YStride<<1;
              JPEG_HDEC_GetUVResolutionForYuv420sp(cinfo,YHeightTmp,u32YStride,&CHeightTmp,&u32UVStride);
              u32YHeight   = (u32YHeight >> 1) << 1;
              u32UVWidth   = u32YWidth;
              u32UVHeight  = u32YHeight >> 1;
              break;
          case JPEG_FMT_YUV444:
              CHeightTmp  = YHeightTmp;
              u32UVStride = u32YStride << 1;
              JPEG_HDEC_GetUVResolutionForYuv420sp(cinfo,YHeightTmp,u32YStride,&CHeightTmp,&u32UVStride);
              u32UVWidth   = u32YWidth;
              u32UVHeight  = u32YHeight;
              break;
          default:
              break;
      }

#ifdef CONFIG_JPEG_OUTPUT_YUV420SP
      if(HI_TRUE == pJpegHandle->bOutYUV420SP && JPEG_FMT_YUV400 != pJpegHandle->enImageFmt){
          u32YWidth   = (0 == u32YWidth  % 2)? u32YWidth  : (u32YWidth  - 1);
          u32YHeight  = (0 == u32YHeight % 2)? u32YHeight : (u32YHeight - 1);
          u32UVWidth  = u32YWidth >> 1;
          u32UVHeight = u32YHeight >> 1;
      }
#endif

      JPEG_HDEC_GetCropRect(cinfo);

      #ifdef CONFIG_JPEG_HARDDEC2ARGB
          u32StrideNeedWidth = (HI_U32)((pJpegHandle->stOutDesc.stCropRect.w + JPEG_MCU_8ALIGN - 1) & (~(JPEG_MCU_8ALIGN - 1)));
      #else
          u32StrideNeedWidth = (HI_U32)(pJpegHandle->stOutDesc.stCropRect.w);
      #endif

      JPEG_HDEC_CalcOutColorComponents(cinfo);
      JPEG_HDEC_CalcOutputComponents(cinfo);

      JPEG_HDEC_CalcOutStride(cinfo,YWidthTmp,u32StrideNeedWidth);
      pJpegHandle->stJpegSofInfo.u32DecStride = (HI_TRUE == pJpegHandle->bOutYCbCrSP) ? (u32YStride) : (pJpegHandle->stJpegSofInfo.u32DecStride);

      u32YSize = YHeightTmp * u32YStride;
      u32CSize = CHeightTmp * u32UVStride;
      JPEG_HDEC_GetCalcOutXRGBSize(cinfo, &u32YSize, &u32CSize);

      pJpegHandle->stJpegSofInfo.bCalcSize       =  HI_TRUE;
      pJpegHandle->stJpegSofInfo.u32YWidth       =  u32YWidth;
      pJpegHandle->stJpegSofInfo.u32YHeight      =  u32YHeight;
      pJpegHandle->stJpegSofInfo.u32YMcuHeight   =  YHeightTmp;
      pJpegHandle->stJpegSofInfo.u32YSize        =  u32YSize;
      pJpegHandle->stJpegSofInfo.u32CWidth       =  u32UVWidth;
      pJpegHandle->stJpegSofInfo.u32CHeight      =  u32UVHeight;
      pJpegHandle->stJpegSofInfo.u32CMcuHeight   =  CHeightTmp;
      pJpegHandle->stJpegSofInfo.u32CSize        =  u32CSize;
      pJpegHandle->stJpegSofInfo.u32YStride      =  u32YStride;
      pJpegHandle->stJpegSofInfo.u32CbCrStride   =  u32UVStride;
      pJpegHandle->stJpegSofInfo.u32DisplayW     =  cinfo->output_width;
      pJpegHandle->stJpegSofInfo.u32DisplayH     =  cinfo->output_height;
      pJpegHandle->stJpegSofInfo.u32DecW         =  DecWidth  >> pJpegHandle->u32ScalRation;
      pJpegHandle->stJpegSofInfo.u32DecH         =  DecHeight >> pJpegHandle->u32ScalRation;
      pJpegHandle->stJpegSofInfo.u32InWandH      =  (InputMcuWidth | (InputMcuHeight << 16));
#ifdef CONFIG_JPEG_HARDDEC2ARGB
      pJpegHandle->stJpegSofInfo.u32McuWidth     =  InputMcuWidth;
      pJpegHandle->stJpegSofInfo.u32MINSize      =  InputMcuWidth * 128;
      pJpegHandle->stJpegSofInfo.u32MIN1Size     =  pJpegHandle->stJpegSofInfo.u32MINSize;
      pJpegHandle->stJpegSofInfo.u32RGBSizeReg   =  2 * pJpegHandle->stJpegSofInfo.u32MINSize;
#endif

      return;
}


static inline HI_VOID JPEG_HDEC_GetCropSize(const struct jpeg_decompress_struct *cinfo)
{
#if defined(CONFIG_JPEG_OUTPUT_CROP) && defined(CONFIG_JPEG_HARDDEC2ARGB)
    JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
    if ( (HI_TRUE != pJpegHandle->stOutDesc.bCrop) || (HI_TRUE != pJpegHandle->bDecOutColorSpaecXRGB))
    {
         return;
    }

    pJpegHandle->stJpegSofInfo.u32YSize = (HI_U32)((pJpegHandle->stJpegSofInfo.u32DisplayStride) * (pJpegHandle->stOutDesc.stCropRect.h))
    pJpegHandle->stJpegSofInfo.u32CSize = 0;
#else
    UNUSED(cinfo);
#endif
    return;
}

static inline HI_VOID JPEG_HDEC_GetUVResolutionForYuv420sp(const struct jpeg_decompress_struct *cinfo, HI_U32 YHeightTmp, HI_U32 YStride, HI_U32 *pUVHeightTmp, HI_U32 *pUVStride)
{
#ifdef CONFIG_JPEG_OUTPUT_YUV420SP
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     if (HI_TRUE != pJpegHandle->bOutYUV420SP)
     {
         return;
     }

     if (JPEG_FMT_YUV422_21 == pJpegHandle->enImageFmt)
     {
         *pUVHeightTmp  = YHeightTmp >> 1;
         return;
     }

     if (JPEG_FMT_YUV422_12 == pJpegHandle->enImageFmt)
     {
         *pUVStride = (YStride << 1) >> 1;
         return;
     }

     if (JPEG_FMT_YUV444 == pJpegHandle->enImageFmt)
     {
         *pUVHeightTmp  = YHeightTmp  >> 1;
         *pUVStride = (YStride << 1) >> 1;
         return;
     }
#else
    UNUSED(cinfo);
    UNUSED(YHeightTmp);
    UNUSED(YStride);
    UNUSED(pUVHeightTmp);
    UNUSED(pUVStride);
#endif
    return;
}

static inline HI_VOID JPEG_HDEC_GetCropRect(const struct jpeg_decompress_struct *cinfo)
{
    JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
    if (HI_FALSE != pJpegHandle->stOutDesc.bCrop)
    {
         return;
    }
    pJpegHandle->stOutDesc.stCropRect.x = 0;
    pJpegHandle->stOutDesc.stCropRect.y = 0;
    pJpegHandle->stOutDesc.stCropRect.w = (HI_S32)cinfo->output_width;
    pJpegHandle->stOutDesc.stCropRect.h = (HI_S32)cinfo->output_height;
    return;
}

static inline HI_VOID JPEG_HDEC_GetCalcOutXRGBSize(const struct jpeg_decompress_struct *cinfo, HI_U32 *pYSize, HI_U32 *pCSize)
{
#ifdef CONFIG_JPEG_HARDDEC2ARGB
   JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
    if (HI_TRUE != pJpegHandle->bDecOutColorSpaecXRGB)
    {
        return;
    }

    *pYSize = pJpegHandle->stJpegSofInfo.u32DisplayStride * pJpegHandle->stOutDesc.stCropRect.h;
    *pCSize = 0;
#else
    UNUSED(cinfo);
    UNUSED(pYSize);
    UNUSED(pCSize);
#endif
    return;
}

static inline HI_VOID JPEG_HDEC_CalcScale(const struct jpeg_decompress_struct *cinfo)
{
      HI_U32 InputScale = 0;
      HI_U32 NewScale   = 0;
      HI_U32 OutWidth   = 0;
      HI_U32 OutHeight  = 0;

      JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

#ifdef CONFIG_GFX_LIB_TURBO
      if (cinfo->scale_num * DCTSIZE <= cinfo->scale_denom){
          InputScale = JPEG_SCALEDOWN_8;
      }else if (cinfo->scale_num * DCTSIZE <= cinfo->scale_denom * 2){
          InputScale = JPEG_SCALEDOWN_4;
      }else if (cinfo->scale_num * DCTSIZE <= cinfo->scale_denom * 4){
          InputScale = JPEG_SCALEDOWN_2;
      }else if (cinfo->scale_num * DCTSIZE <= cinfo->scale_denom * 8){
          InputScale = JPEG_SCALEDOWN_1;
      }else{
          InputScale = JPEG_SCALEDOWN_BUTT;
          pJpegHandle->u32ScalRation = JPEG_SCALEDOWN_BUTT;
          return;
      }
#else
      if (cinfo->scale_num * 8 <= cinfo->scale_denom){
          InputScale = JPEG_SCALEDOWN_8;
      }else if (cinfo->scale_num * 4 <= cinfo->scale_denom){
          InputScale = JPEG_SCALEDOWN_4;
      }else if (cinfo->scale_num * 2 <= cinfo->scale_denom){
          InputScale = JPEG_SCALEDOWN_2;
      }else if (cinfo->scale_num == cinfo->scale_denom){
           InputScale = JPEG_SCALEDOWN_1;
      }else{
           InputScale = JPEG_SCALEDOWN_BUTT;
      }
#endif

      JPEG_HDEC_GetOutSize(InputScale,cinfo->image_width,cinfo->image_height,&OutWidth,&OutHeight);

      for (NewScale = JPEG_SCALEDOWN_1; NewScale <= JPEG_SCALEDOWN_8; NewScale++){
          if (JPEG_ALIGNED_SCALE(cinfo->image_width, NewScale) == OutWidth){
              break;
          }
      }

      if (JPEG_ALIGNED_SCALE(cinfo->image_width, (NewScale + 1)) != OutWidth){
          pJpegHandle->u32ScalRation = (NewScale < InputScale) ? (NewScale) : (InputScale);
          return;
      }

      for (NewScale = JPEG_SCALEDOWN_1; NewScale <= JPEG_SCALEDOWN_8; NewScale++){
          if (JPEG_ALIGNED_SCALE(cinfo->image_height, NewScale) == OutHeight){
              break;
          }
      }

      pJpegHandle->u32ScalRation = (NewScale < InputScale) ? (NewScale) : (InputScale);

      return;
}


static inline HI_VOID JPEG_HDEC_GetOutSize(const HI_U32 u32Ration,const HI_U32 u32InWidth, const HI_U32 u32InHeight,
                                           HI_U32 *pu32OutWidth,HI_U32 *pu32OutHeight)
{
      switch(u32Ration){
            case 0:
                *pu32OutWidth  = u32InWidth;
                *pu32OutHeight = u32InHeight;
                break;
            case 1:
                *pu32OutWidth  = (JDIMENSION)JPEG_ROUND_UP((long) u32InWidth,  2L);
                *pu32OutHeight = (JDIMENSION)JPEG_ROUND_UP((long) u32InHeight, 2L);
                break;
            case 2:
                *pu32OutWidth  = (JDIMENSION)JPEG_ROUND_UP((long) u32InWidth,  4L);
                *pu32OutHeight = (JDIMENSION)JPEG_ROUND_UP((long) u32InHeight, 4L);
                break;
            case 3:
                *pu32OutWidth  = (JDIMENSION)JPEG_ROUND_UP((long) u32InWidth,  8L);
                *pu32OutHeight = (JDIMENSION)JPEG_ROUND_UP((long) u32InHeight, 8L);
                break;
            default:
                break;

      }
      return;
}


static inline HI_VOID JPEG_HDEC_SetOutColorSapceMarker(const struct jpeg_decompress_struct *cinfo)
{
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     if (  (JCS_YUV400_SP    == cinfo->out_color_space)
         ||(JCS_YUV420_SP    == cinfo->out_color_space)
         ||(JCS_YUV422_SP_12 == cinfo->out_color_space)
         ||(JCS_YUV422_SP_21 == cinfo->out_color_space)
         ||(JCS_YUV444_SP    == cinfo->out_color_space)){
          pJpegHandle->bOutYCbCrSP = HI_TRUE;
      }else{
          pJpegHandle->bOutYCbCrSP = HI_FALSE;
      }

      if (0 == pJpegHandle->u32ScalRation){
          pJpegHandle->bDecOutColorSpaecXRGB = JPEG_HDEC_CheckDecXRGBSupport(cinfo);
      }else{
          pJpegHandle->bDecOutColorSpaecXRGB = HI_FALSE;
      }
      return;
}


static inline HI_BOOL JPEG_HDEC_CheckDecXRGBSupport(const struct jpeg_decompress_struct *cinfo)
{
 #ifdef CONFIG_JPEG_HARDDEC2ARGB
    #ifdef CONFIG_GFX_LIB_TURBO
        if(  (JCS_RGB565   == cinfo->out_color_space) || (JCS_RGB == cinfo->out_color_space)
           ||(JCS_EXT_RGBA == cinfo->out_color_space) || (JCS_EXT_BGRA == cinfo->out_color_space)
           ||(JCS_EXT_RGB  == cinfo->out_color_space) || (JCS_EXT_BGR == cinfo->out_color_space)){
            return HI_TRUE;
        }
   #else
        if(  (JCS_ARGB_8888 == cinfo->out_color_space)
           ||(JCS_ABGR_8888 == cinfo->out_color_space)
           ||(JCS_ARGB_1555 == cinfo->out_color_space)
           ||(JCS_ABGR_1555 == cinfo->out_color_space)
           ||(JCS_RGB_565   == cinfo->out_color_space)
           ||(JCS_BGR_565   == cinfo->out_color_space)
           ||(JCS_RGB       == cinfo->out_color_space)
           ||(JCS_BGR       == cinfo->out_color_space)
           ||(JCS_RGBA_8888 == cinfo->out_color_space)){
            return HI_TRUE;
        }
   #endif
 #endif
        return HI_FALSE;
}


#ifdef CONFIG_GFX_LIB_TURBO

static HI_VOID JPEG_HDEC_CalcOutColorComponents(j_decompress_ptr cinfo)
{
    switch(cinfo->out_color_space){
        case JCS_CMYK:
        case JCS_EXT_RGBA:
        case JCS_EXT_BGRA:
             cinfo->out_color_components = 4;
             break;
        case JCS_EXT_RGB:
        case JCS_EXT_BGR:
        case JCS_RGB:
        case JCS_YCbCr:
        case JCS_RGB565:
             cinfo->out_color_components = 3;
             break;
        default:
             cinfo->out_color_components = 1;
             break;
    }
    return;
}

HI_VOID JPEG_HDEC_CalcOutputComponents(j_decompress_ptr cinfo)
{
    switch(cinfo->out_color_space){
        case JCS_CMYK:
        case JCS_EXT_RGBA:
        case JCS_EXT_BGRA:
             cinfo->output_components = 4;
             break;
        case JCS_EXT_RGB:
        case JCS_EXT_BGR:
        case JCS_RGB:
        case JCS_YCbCr:
             cinfo->output_components = 3;
             break;
        case JCS_RGB565:
             cinfo->output_components = 2;
             break;
        default:
             cinfo->output_components = 1;
             break;
    }
    return;
}

#else

static HI_VOID JPEG_HDEC_CalcOutColorComponents(j_decompress_ptr cinfo)
{
    switch(cinfo->out_color_space){
        case JCS_RGB:
        case JCS_BGR:
        case JCS_RGB_565:
        case JCS_BGR_565:
        case JCS_CrCbY:
        case JCS_YCbCr:
             cinfo->out_color_components = 3;
             break;
        case JCS_CMYK:
        case JCS_ARGB_8888:
        case JCS_ABGR_8888:
        #ifdef CONFIG_JPEG_ADD_GOOGLEFUNCTION
        case JCS_RGBA_8888:
        #endif
             cinfo->out_color_components = 4;
             break;
        case JCS_ARGB_1555:
        case JCS_ABGR_1555:
             cinfo->out_color_components = 2;
             break;
        default:
             cinfo->out_color_components = 1;
             break;
    }
    return;
}

HI_VOID JPEG_HDEC_CalcOutputComponents(j_decompress_ptr cinfo)
{
    switch(cinfo->out_color_space){
        case JCS_RGB:
        case JCS_BGR:
        case JCS_CrCbY:
        case JCS_YCbCr:
             cinfo->output_components = 3;
             break;
        case JCS_CMYK:
        case JCS_ARGB_8888:
        case JCS_ABGR_8888:
        #ifdef CONFIG_JPEG_ADD_GOOGLEFUNCTION
        case JCS_RGBA_8888:
        #endif
             cinfo->output_components = 4;
             break;
        case JCS_ARGB_1555:
        case JCS_ABGR_1555:
        case JCS_RGB_565:
        case JCS_BGR_565:
             cinfo->output_components = 2;
             break;
        default:
             cinfo->output_components = 1;
             break;
    }
    return;
}


#endif


/*****************************************************************************
* func            : JPEG_HDEC_CalcOutStride
* description    : calculate out stride
                  CNcomment:  计算输出行间距 CNend\n
* param[in]     : cinfo       CNcomment: 解码对象    CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_CalcOutStride(j_decompress_ptr cinfo,HI_U32 u32DecWidth,HI_U32 u32DisplayWidth)
{
    JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
    HI_GFX_GetStride(u32DisplayWidth * cinfo->output_components,&(pJpegHandle->stJpegSofInfo.u32DisplayStride),pJpegHandle->u32StrideAlign);
    pJpegHandle->stJpegSofInfo.u32DecStride = u32DecWidth * cinfo->output_components;
    return;
}
