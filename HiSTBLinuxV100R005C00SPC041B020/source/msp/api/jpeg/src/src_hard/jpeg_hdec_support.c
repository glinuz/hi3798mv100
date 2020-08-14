/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_support.c
Version          : Initial Draft
Author           : y00181162
Created          : 2016/05/09
Description      : check whether hard decode support CNend\n
Function List    :


History          :
Date                Author                Modification
2016/05/09          y00181162            Created file
******************************************************************************/

/***************************** add include here*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "jpeglib.h"
#include "jerror.h"
#include "hi_jpeg_config.h"
#include "jpeg_hdec_api.h"
#include  "hi_gfx_sys.h"


#if defined(CONFIG_JPEG_ANDROID_DEBUG_ENABLE) && defined(CONFIG_JPEG_DEBUG_INFO)
#include <cutils/properties.h>
#endif

/***************************** Macro Definition ********************************/


/***************************** Structure Definition ****************************/

/***************************** Global Variable declaration *********************/

/***************************** API forward declarations ************************/
static inline HI_BOOL JPEG_HDEC_ForceToSoftDec      (const struct jpeg_decompress_struct *cinfo);
#ifdef CONFIG_JPEG_ADD_GOOGLEFUNCTION
static inline HI_BOOL JPEG_HDEC_SupportCts(HI_VOID);
#endif
static inline HI_BOOL JPEG_HDEC_HardDecInflexion    (const struct jpeg_decompress_struct *cinfo);
static inline HI_BOOL JPEG_HDEC_OutYUV420SPSupport  (const struct jpeg_decompress_struct *cinfo);
#ifndef CONFIG_JPEG_DRI_SUPPORT
static inline HI_BOOL JPEG_HDEC_DriSupport(const struct jpeg_decompress_struct *cinfo);
#endif
static inline HI_BOOL JPEG_HDEC_SampleFactorSupport (const struct jpeg_decompress_struct *cinfo);
static inline HI_BOOL JPEG_HDEC_InputImgSizeSupport (const struct jpeg_decompress_struct *cinfo);
static inline HI_BOOL JPEG_HDEC_OutColorSpaceSupport(const struct jpeg_decompress_struct *cinfo);
static inline HI_VOID JPEG_HDEC_ExistQuantTable     (const struct jpeg_decompress_struct *cinfo);
static inline HI_VOID JPEG_HDEC_ExistHuffTable      (const struct jpeg_decompress_struct *cinfo);
static inline HI_BOOL JPEG_HDEC_CropRectSupport     (const struct jpeg_decompress_struct *cinfo);

/***************************** API realization *********************************/


/*****************************************************************************
* func          : JPEG_HDEC_IfSupport
* description   : check whether the hard decode support
                  CNcomment: 判断是否支持硬件解码   CNend\n
* param[in]     : cinfo       CNcomment: 解码对象   CNend\n
* retval        : HI_TRUE     CNcomment: support    CNend\n
* retval        : HI_FALSE    CNcomment: unsupport  CNend\n
* others:       : NA
*****************************************************************************/
HI_BOOL JPEG_HDEC_IfSupport(j_decompress_ptr cinfo)
{
#ifndef CONFIG_JPEG_DRI_SUPPORT
    HI_BOOL DriSupport     = HI_FALSE;
#endif
#ifdef CONFIG_JPEG_ADD_GOOGLEFUNCTION
    HI_BOOL SupportCts     = HI_FALSE;
#endif
    HI_BOOL ForceToSoftDec = HI_FALSE;
    HI_BOOL OutYUV420SPSupport  = HI_FALSE;
    HI_BOOL InputImgSizeSupport = HI_FALSE;
    HI_BOOL OutColorSpaceSupport = HI_FALSE;
    HI_BOOL HardDecInflexion = HI_FALSE;
    HI_BOOL SampleFactorSupport = HI_FALSE;
    HI_BOOL CropRectSupport = HI_FALSE;

    ForceToSoftDec = JPEG_HDEC_ForceToSoftDec(cinfo);
    if (HI_TRUE == ForceToSoftDec){
        return HI_FALSE;
    }

    HardDecInflexion = JPEG_HDEC_HardDecInflexion(cinfo);
    if (HI_TRUE == HardDecInflexion){
        return HI_FALSE;
    }

#ifdef CONFIG_JPEG_ADD_GOOGLEFUNCTION
    SupportCts = JPEG_HDEC_SupportCts();
    if (HI_TRUE == SupportCts)
    {
        return HI_FALSE;
    }
#endif

    OutYUV420SPSupport = JPEG_HDEC_OutYUV420SPSupport(cinfo);
    if (HI_TRUE != OutYUV420SPSupport){
        return HI_FALSE;
    }

#ifndef CONFIG_JPEG_DRI_SUPPORT
    DriSupport = JPEG_HDEC_DriSupport(cinfo);
    if (HI_TRUE != DriSupport)
    {
        return HI_FALSE;
    }
#endif

    SampleFactorSupport = JPEG_HDEC_SampleFactorSupport(cinfo);
    if (HI_TRUE != SampleFactorSupport){
        return HI_FALSE;
    }

    InputImgSizeSupport = JPEG_HDEC_InputImgSizeSupport(cinfo);
    if (HI_TRUE != InputImgSizeSupport){
        return HI_FALSE;
    }

    OutColorSpaceSupport = JPEG_HDEC_OutColorSpaceSupport(cinfo);
    if (HI_TRUE != OutColorSpaceSupport){
        return HI_FALSE;
    }

    JPEG_HDEC_ExistQuantTable(cinfo);

    JPEG_HDEC_ExistHuffTable(cinfo);

    CropRectSupport = JPEG_HDEC_CropRectSupport(cinfo);
    if (HI_TRUE != CropRectSupport){
        return HI_FALSE;
    }

    return HI_TRUE;
}


static inline HI_BOOL JPEG_HDEC_ForceToSoftDec(const struct jpeg_decompress_struct *cinfo)
{
#ifdef CONFIG_JPEG_DEBUG_INFO

   #ifdef CONFIG_JPEG_ANDROID_DEBUG_ENABLE
       HI_CHAR JpegDecMod[256] = {0};
       /** setprop JPEGDECMOD soft **/
       property_get("JPEGDECMOD",JpegDecMod,"hw");
       if (0 != strncmp("hw", JpegDecMod, strlen("hw")>strlen(JpegDecMod)?strlen("hw"):strlen(JpegDecMod))){
          JPEG_TRACE("=== force to soft decode !\n");
          return HI_TRUE;
       }
   #else
       HI_CHAR *pJpegDecMod       = NULL;
       /** export JPEGDECMOD=soft **/
       pJpegDecMod = getenv( "JPEGDECMOD" );
       if (pJpegDecMod && 0 == strncmp("soft", pJpegDecMod, strlen("soft")>strlen(pJpegDecMod)?strlen("soft"):strlen(pJpegDecMod))){
          JPEG_TRACE("=== force to soft decode !\n");
          return HI_TRUE;
       }
   #endif

#endif
    if( (FALSE != cinfo->progressive_mode) || (FALSE != cinfo->arith_code) ||(8 != cinfo->data_precision)){
        return HI_TRUE;
    }

    return HI_FALSE;
}


static inline HI_BOOL JPEG_HDEC_HardDecInflexion(const struct jpeg_decompress_struct *cinfo)
{
    HI_U32 ImgSize = cinfo->image_width * cinfo->image_height;
    JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
    if(ImgSize <= pJpegHandle->u32Inflexion){
         return HI_TRUE;
    }
    return HI_FALSE;
}


#ifdef CONFIG_JPEG_ADD_GOOGLEFUNCTION
static inline HI_BOOL JPEG_HDEC_SupportCts(HI_VOID)
{
    HI_CHAR Name[25];
    HI_CHAR ProcName[25];
    HI_CHAR ProcessStatusPath[256] ;
    HI_S32 s32Pid = 0;
    FILE* pProcessStatus = NULL;

    HI_GFX_Memset(Name, 0, 25);
    HI_GFX_Memset(ProcName, 0, 25);
    HI_GFX_Memset(ProcessStatusPath, 0, 256);

    s32Pid = getpid();
    snprintf(ProcessStatusPath,256,"%s%d%s","/proc/",s32Pid,"/status");

    pProcessStatus = fopen(ProcessStatusPath, "r");
    if (NULL != pProcessStatus)
    {
       fscanf(pProcessStatus,"%s %s",Name,ProcName);
       fclose(pProcessStatus);
    }

    if (  (0 == strncmp("ndroid.cts.stub", ProcName, strlen("ndroid.cts.stub") > strlen(ProcName) ? strlen("ndroid.cts.stub") : strlen(ProcName)))
        ||(0 == strncmp("id.cts.graphics", ProcName, strlen("id.cts.graphics") > strlen(ProcName) ? strlen("id.cts.graphics") : strlen(ProcName))))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}
#endif

static inline HI_BOOL JPEG_HDEC_OutYUV420SPSupport(const struct jpeg_decompress_struct *cinfo)
{
#ifdef CONFIG_JPEG_OUTPUT_YUV420SP
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
     if ( (HI_TRUE == pJpegHandle->bOutYUV420SP) && (CONFIG_JPEG_SCALE_MULTIPLE_8 == pJpegHandle->u32ScalRation)){
         return HI_FALSE;
     }
#endif
     if (pJpegHandle->u32ScalRation > CONFIG_JPEG_SCALE_MULTIPLE_8){
         return HI_FALSE;
     }
     return HI_TRUE;
}

#ifndef CONFIG_JPEG_DRI_SUPPORT
static inline HI_BOOL JPEG_HDEC_DriSupport(const struct jpeg_decompress_struct *cinfo)
{
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
     if (  (0 != pJpegHandle->s32LuDcLen[0]) || (0 != pJpegHandle->s32LuDcLen[1])
         ||(0 != pJpegHandle->s32LuDcLen[2]) || (0 != pJpegHandle->s32LuDcLen[3])
         ||(0 != pJpegHandle->s32LuDcLen[4]) || (0 != pJpegHandle->s32LuDcLen[5]))
     {
         return HI_FALSE;
     }

     return HI_TRUE;
}
#endif

static inline HI_BOOL JPEG_HDEC_SampleFactorSupport(const struct jpeg_decompress_struct *cinfo)
{
#ifdef CONFIG_JPEG_SET_SAMPLEFACTOR
    HI_S32  ComponentCnt = 0;
    HI_BOOL bY22 = HI_FALSE;
    HI_BOOL bU12 = HI_FALSE;
    HI_BOOL bV12 = HI_FALSE;
    jpeg_component_info *compptr = NULL;

    JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

    for (ComponentCnt = 0; ComponentCnt < cinfo->comps_in_scan; ComponentCnt++){
         compptr = cinfo->cur_comp_info[ComponentCnt];
         if( (0 == ComponentCnt) && (2 == compptr->h_samp_factor) && (2 == compptr->v_samp_factor))
             bY22 = HI_TRUE;
         if( (1 == ComponentCnt) && (1 == compptr->h_samp_factor) && (2 == compptr->v_samp_factor))
             bU12 = HI_TRUE;
         if( (2 == ComponentCnt) && (1 == compptr->h_samp_factor) && (2 == compptr->v_samp_factor))
             bV12 = HI_TRUE;
    }

    if ( (HI_TRUE == bY22) && (HI_TRUE == bU12) && (HI_TRUE == bV12)){
         return HI_FALSE;
    }

    if ( (JPEG_FMT_YUV444 == pJpegHandle->enImageFmt) && (HI_TRUE == bU12) && (HI_TRUE == bV12)){
        return HI_FALSE;
    }
#endif
    return HI_TRUE;
}


static inline HI_BOOL JPEG_HDEC_InputImgSizeSupport(const struct jpeg_decompress_struct *cinfo)
{
     if ( (cinfo->image_width  < 1) || (cinfo->image_width  > 8096)
        ||(cinfo->image_height < 1) || (cinfo->image_height > 8096)){
         return HI_FALSE;
     }

     return HI_TRUE;
}


static inline HI_BOOL JPEG_HDEC_OutColorSpaceSupport(const struct jpeg_decompress_struct *cinfo)
{
#ifdef CONFIG_GFX_LIB_TURBO
     if (  (JCS_RGB565       != cinfo->out_color_space) && (JCS_RGB          != cinfo->out_color_space)
         &&(JCS_EXT_RGBA     != cinfo->out_color_space) && (JCS_EXT_BGRA     != cinfo->out_color_space)
         &&(JCS_EXT_RGB      != cinfo->out_color_space) && (JCS_EXT_BGR      != cinfo->out_color_space)
         &&(JCS_YUV400_SP    != cinfo->out_color_space) && (JCS_YUV420_SP    != cinfo->out_color_space)
         &&(JCS_YUV422_SP_12 != cinfo->out_color_space) && (JCS_YUV422_SP_21 != cinfo->out_color_space)
         &&(JCS_YUV444_SP    != cinfo->out_color_space)){
         return HI_FALSE;
     }
#else
     if (  (JCS_CMYK  == cinfo->out_color_space) || (JCS_YCCK  == cinfo->out_color_space)
         ||(JCS_YCbCr == cinfo->out_color_space) || (JCS_GRAYSCALE == cinfo->out_color_space)){
        return HI_FALSE;
     }
#endif
     return HI_TRUE;
}


static inline HI_VOID JPEG_HDEC_ExistQuantTable(const struct jpeg_decompress_struct *cinfo)
{
#ifndef CONFIG_JPEG_MPG_DEC_ENABLE
     if (NULL == cinfo->quant_tbl_ptrs[0]){
         ERREXIT(cinfo, JERR_NO_QUANT_TABLE);
     }
#else
     UNUSED(cinfo);
#endif
     return;
}


static inline HI_VOID JPEG_HDEC_ExistHuffTable(const struct jpeg_decompress_struct *cinfo)
{
#ifndef CONFIG_JPEG_MPG_DEC_ENABLE
     if (  (NULL == cinfo->dc_huff_tbl_ptrs[0]) || (NULL != cinfo->dc_huff_tbl_ptrs[2])
         ||(NULL == cinfo->ac_huff_tbl_ptrs[0]) || (NULL != cinfo->ac_huff_tbl_ptrs[2]) ){
         ERREXIT(cinfo, JERR_BAD_HUFF_TABLE);
     }
#else
     UNUSED(cinfo);
#endif
     return;
}

static inline HI_BOOL JPEG_HDEC_CropRectSupport(const struct jpeg_decompress_struct *cinfo)
{
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     if (HI_TRUE != pJpegHandle->stOutDesc.bCrop){
         return HI_TRUE;
     }

     if(  (pJpegHandle->stOutDesc.stCropRect.w <= 0) || (pJpegHandle->stOutDesc.stCropRect.h <= 0)
        ||(pJpegHandle->stOutDesc.stCropRect.x < 0)  ||  (pJpegHandle->stOutDesc.stCropRect.y < 0)
        ||((HI_U32)(pJpegHandle->stOutDesc.stCropRect.x + pJpegHandle->stOutDesc.stCropRect.w) > cinfo->output_width)
        ||((HI_U32)(pJpegHandle->stOutDesc.stCropRect.y + pJpegHandle->stOutDesc.stCropRect.h) > cinfo->output_height)){
          return HI_FALSE;
     }

     return HI_TRUE;
}
