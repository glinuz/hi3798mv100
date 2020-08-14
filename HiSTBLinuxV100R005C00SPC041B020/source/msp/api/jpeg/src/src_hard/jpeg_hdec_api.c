/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_api.c
Version          : Initial Draft
Author           : y00181162
Created          : 2016/05/09
Description      : hard decode CNend\n
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
#include "jpeg_hdec_error.h"
#include "jpeg_hdec_mem.h"
#include "jpeg_hdec_rwreg.h"
#include "jpeg_hdec_adp.h"
#include "jpeg_hdec_csc.h"
#include "hi_jpeg_reg.h"
#include "hi_gfx_sys.h"


#ifdef CONFIG_JPEG_PROC_ENABLE
#include "jpegint.h"
#endif

#if    defined(CONFIG_JPEG_TEST_SAVE_BMP_PIC)
    || defined(CONFIG_JPEG_TEST_SAVE_YUVSP_DATA)
#include "hi_jpeg_hdec_test.h"
#endif

#if defined(CONFIG_JPEG_ANDROID_DEBUG_ENABLE) && defined(CONFIG_JPEG_DEBUG_INFO)
#include <cutils/properties.h>
#define LOG_TAG "libjpeg"
#endif


/***************************** Macro Definition ********************************/
#if 0
#define MEMINFO_FILE                     "/proc/meminfo"
#else
#define MEMINFO_FILE                     ""
#endif

#define JPGD_HDEC_FLEXION_SIZE          0
#define  MEMLEAVESIZE_DEFAULT           0x20000000     /** 512M **/


/***************************** Structure Definition ****************************/

typedef struct tabJPEGLEAVEMEMINFO
{
  const HI_CHAR *pName;
  HI_U64 *pSize;
}JPEG_LEAVE_MEMINFO;


#ifndef CONFIG_JPEG_CSC_DISABLE
JPEG_DECOMPRESS_RES gs_stJpegDecompressRes = {-1, NULL, NULL};
#endif


/***************************** Global Variable declaration *********************/

/***************************** API forward declarations ************************/
extern HI_BOOL JPEG_HDEC_IfSupport          (j_decompress_ptr cinfo);
extern HI_BOOL JPEG_HDEC_GetTable           (j_decompress_ptr cinfo);
extern HI_BOOL JPEG_HDEC_AllocDecodeMemory  (j_decompress_ptr cinfo);
extern HI_VOID JPEG_HDEC_SetProcInfo        (const struct jpeg_decompress_struct *cinfo);
extern HI_VOID JPEG_HDEC_TEST_SaveYUVSP     (const struct jpeg_decompress_struct *cinfo);


static HI_S32  JPEG_HDEC_SendStream   (j_decompress_ptr cinfo);
static HI_BOOL JPEG_HDEC_OpenDev      (const struct jpeg_decompress_struct *cinfo);
static HI_VOID JPEG_HDEC_ReleaseRes   (const struct jpeg_common_struct *cinfo);
static inline HI_S32 JPEG_HDEC_NeedCpyDataToUserBuffer (j_decompress_ptr cinfo,JDIMENSION max_lines);


/******************************* API realization *****************************/


void __attribute__ ((constructor)) jpeg_lib_creat(void)
{
#if 0
    JPEG_TRACE("\n==========================================================\n");
    JPEG_TRACE("==== create process_id = %u,pthread_id = %u\n",(unsigned int)getpid(), (unsigned int)syscall(SYS_gettid));
    JPEG_TRACE("==========================================================\n");
#endif
}


void __attribute__ ((destructor)) jpeg_lib_destroy(void)
{
#ifndef CONFIG_JPEG_CSC_DISABLE
      if (gs_stJpegDecompressRes.s32CscDev >= 0){
         JPEG_HDEC_CSC_Close(gs_stJpegDecompressRes.s32CscDev);
      }
      gs_stJpegDecompressRes.s32CscDev = -1;
#endif

#if 0
     JPEG_TRACE("\n==========================================================\n");
     JPEG_TRACE("==== destory process_id = %u,pthread_id = %u\n",(unsigned int)getpid(), (unsigned int)syscall(SYS_gettid));
     JPEG_TRACE("==========================================================\n");
#endif
}


HI_VOID JPEG_HDEC_CloseDev(const struct jpeg_common_struct *cinfo)
{
#ifdef CONFIG_JPEG_4KDDR_DISABLE_SHOWMSG
      HI_U32 u32AXI4KCNT   = 0;
      HI_U32 u32AXI16MCNT  = 0;
      HI_U32 u32ReadAXI4KCNT   = 0;
      HI_U32 u32WriteAXI4KCNT  = 0;
      HI_U32 u32ReadAXI16MCNT  = 0;
      HI_U32 u32WriteAXI16MCNT = 0;
#endif

      HI_S32 Ret = HI_SUCCESS;
      JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

      JPEG_HDEC_SetProcInfo((j_decompress_ptr)cinfo);

#ifdef CONFIG_JPEG_4KDDR_DISABLE_SHOWMSG
      if (NULL != pJpegHandle->pJpegRegVirAddr)
      {
          u32AXI4KCNT  = (HI_U32)JPEG_HDEC_ReadReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_AXI4KCNT);
          u32AXI16MCNT = (HI_U32)JPEG_HDEC_ReadReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_AXI16MCNT);
      }
      u32ReadAXI4KCNT   = u32AXI4KCNT  & 0xffff0000;
      u32WriteAXI4KCNT  = u32AXI4KCNT  & 0x0000ffff;
      u32ReadAXI16MCNT  = u32AXI16MCNT & 0xffff0000;
      u32WriteAXI16MCNT = u32AXI16MCNT & 0x0000ffff;
      if (0 != u32AXI4KCNT || 0 != u32AXI16MCNT || 0 != u32ReadAXI4KCNT || 0 != u32WriteAXI4KCNT || 0 != u32ReadAXI16MCNT || 0 != u32WriteAXI16MCNT)
      {
          JPEG_TRACE("\n=====================================================\n");
          JPEG_TRACE("跨4K次数  = %d\n", u32AXI4KCNT);
          JPEG_TRACE("跨16M次数 = %d\n", u32AXI16MCNT);
          JPEG_TRACE("读地址跨4K次数 = %d\n", u32ReadAXI4KCNT);
          JPEG_TRACE("写地址跨4K次数 = %d\n", u32WriteAXI4KCNT);
          JPEG_TRACE("读地址跨16M次数 = %d\n",u32ReadAXI16MCNT);
          JPEG_TRACE("写地址跨16M次数 = %d\n",u32WriteAXI16MCNT);
          JPEG_TRACE("=====================================================\n");
          sleep(2);
      }

      #ifdef CONFIG_JPEG_4KDDR_DISABLE
      if (0 != u32AXI4KCNT || 0 != u32AXI16MCNT)
      {
          JPEG_TRACE("\n=====================================================\n");
          JPEG_TRACE("跨4K开关关掉异常,逻辑有问题\n");
          JPEG_TRACE("按回车键结束\n");
          JPEG_TRACE("=====================================================\n");
          getchar();
      }
      #endif
#endif

      if (NULL != pJpegHandle->pJpegRegVirAddr)
      {
          Ret = munmap((void*)pJpegHandle->pJpegRegVirAddr, JPGD_REG_LENGTH);
          if (HI_SUCCESS != Ret)
          {
            JPEG_TRACE("warning: munmap register address failure\n");
          }
          pJpegHandle->pJpegRegVirAddr = NULL;
      }

      JPEG_HDEC_SetLowDelayBufferDate((j_decompress_ptr)cinfo);

      if (pJpegHandle->s32JpegDev < 0)
      {
          return;
      }

#ifdef CONFIG_JPEG_TEST_SUPPORT
      if (HI_TRUE == pJpegHandle->stTestInfo.bDiscardClose)
      {
          JPEG_TRACE("\n===============================================================\n");
          JPEG_TRACE("== test not call close jpeg device\n");
          JPEG_TRACE("===============================================================\n");
          return;
      }
#endif

      close(pJpegHandle->s32JpegDev);

      pJpegHandle->s32JpegDev = -1;

      return;
}



HI_S32 JPEG_HDEC_Init(j_common_ptr cinfo)
{
#ifdef CONFIG_JPEG_GETDECTIME
     HI_S32 s32Ret  = HI_SUCCESS;
#endif

     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = NULL;

     pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)calloc(1, sizeof(JPEG_HDEC_HANDLE_S));
     JPEG_ASSERT((NULL == pJpegHandle), JPEG_ERR_NOMEM);

#ifdef CONFIG_JPEG_GETDECTIME
     s32Ret  = HI_GFX_GetTimeStamp(&pJpegHandle->u32CurTime,NULL);
     if (HI_SUCCESS != s32Ret)
     {
         free(pJpegHandle);
         pJpegHandle = NULL;
         return HI_FAILURE;
     }
#endif

     pJpegHandle->bFillInput      =  HI_TRUE;
     pJpegHandle->u32StrideAlign  = JPGD_HDEC_MMZ_CSCOUT_STRIDE_ALIGN;
     pJpegHandle->s32ClientData   =  CLIENT_DATA_MARK;
     pJpegHandle->s32JpegDev      = -1;
     pJpegHandle->u32Inflexion    =  JPGD_HDEC_FLEXION_SIZE;
     pJpegHandle->u32Alpha        =  0xFF;
#ifdef CONFIG_GFX_MEM_ION
     pJpegHandle->s32MMZDev       =  -1;
#endif
#ifdef CONFIG_JPEG_PROC_ENABLE
     pJpegHandle->eDecState       =  JPEG_DEC_STATE_BUTT;
#endif
     pJpegHandle->enImageFmt      =  JPEG_FMT_BUTT;

     cinfo->client_data = (void *)pJpegHandle;

     return HI_SUCCESS;
}



HI_VOID JPEG_HDEC_Abort(const struct jpeg_common_struct *cinfo)
{
#ifdef CONFIG_JPEG_GETDECTIME
      HI_U32 u32PreTime = 0;
#endif

      JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
      if (NULL == pJpegHandle){
          return;
      }

      if (HI_FALSE == pJpegHandle->bReleaseRes){
          JPEG_HDEC_ReleaseRes(cinfo);
      }

      HI_GFX_Memset(pJpegHandle,0,sizeof(JPEG_HDEC_HANDLE_S));

      pJpegHandle->bReleaseRes  =  HI_TRUE;
      pJpegHandle->bFillInput   =  HI_TRUE;
      pJpegHandle->u32StrideAlign = JPGD_HDEC_MMZ_CSCOUT_STRIDE_ALIGN;
      pJpegHandle->s32ClientData  = CLIENT_DATA_MARK;
      pJpegHandle->s32JpegDev     = -1;
      pJpegHandle->u32Inflexion   = JPGD_HDEC_FLEXION_SIZE;
      pJpegHandle->u32Alpha       = 0xFF;
#ifdef CONFIG_GFX_MEM_ION
      pJpegHandle->s32MMZDev      =  -1;
#endif
#ifdef CONFIG_JPEG_PROC_ENABLE
      pJpegHandle->eDecState      =  JPEG_DEC_STATE_BUTT;
#endif

      pJpegHandle->enImageFmt     =  JPEG_FMT_BUTT;

#ifdef CONFIG_JPEG_GETDECTIME
       u32PreTime = pJpegHandle->u32CurTime;
       HI_GFX_GetTimeStamp(&pJpegHandle->u32CurTime,NULL);
       pJpegHandle->u32DecTime = pJpegHandle->u32CurTime - u32PreTime;
#endif

      return;
}


HI_VOID JPEG_HDEC_Destroy(const struct jpeg_common_struct *cinfo)
{
    JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
    if (NULL == pJpegHandle)
    {
        return;
    }

    if (HI_FALSE == pJpegHandle->bReleaseRes)
    {
        JPEG_HDEC_ReleaseRes(cinfo);
    }

    free(pJpegHandle);
    pJpegHandle = NULL;

    return;
}


static HI_VOID JPEG_HDEC_ReleaseRes(const struct jpeg_common_struct *cinfo)
{
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     JPEG_HDEC_FreeStreamMem(pJpegHandle);

     JPEG_HDEC_FreeYUVMem(pJpegHandle);

#ifdef CONFIG_JPEG_HARDDEC2ARGB
     if (HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB){
         JPEG_HDEC_FreeMinMem(pJpegHandle);
     }
#endif

     JPEG_HDEC_FreeOutMem(pJpegHandle);

#ifdef CONFIG_GFX_MEM_ION
     if(pJpegHandle->s32MMZDev >= 0){
         gfx_mem_close(pJpegHandle->s32MMZDev);
         pJpegHandle->s32MMZDev = -1;
     }
#endif
     return;
}



/*****************************************************************************
* func          : JPEG_HDEC_Start
* description   : start jpeg hard decode
                  CNcomment: 开始硬件解码
* param[in]     : cinfo       CNcomment: 解码对象   CNend\n
* retval        : HI_TRUE     CNcomment: 成功       CNend\n
* retval        : HI_FALSE    CNcomment: 失败       CNend\n
* others:       : NA
*****************************************************************************/
HI_BOOL JPEG_HDEC_Start(j_decompress_ptr cinfo)
{

    HI_S32 Ret = HI_SUCCESS;

    JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

    if ( (NULL == pJpegHandle) || (CLIENT_DATA_MARK != pJpegHandle->s32ClientData))
    {
         ERREXIT(cinfo, JERR_CLIENT_DATA_ERR);
    }

    pJpegHandle->bReleaseRes =  HI_FALSE;

    JPEG_HDEC_GetImagInfo(cinfo);

    if (HI_TRUE != JPEG_HDEC_IfSupport(cinfo)){
        return HI_FALSE;
    }

    if (HI_TRUE != JPEG_HDEC_GetTable(cinfo)){
        return HI_FALSE;
    }

    if (HI_TRUE != JPEG_HDEC_AllocDecodeMemory(cinfo)){
        return HI_FALSE;
    }

    if (HI_TRUE != JPEG_HDEC_OpenDev(cinfo)){
        return HI_FALSE;
    }

    Ret = JPEG_HDEC_SendStream(cinfo);
    if (HI_SUCCESS != Ret){
        return HI_FALSE;
    }

    return HI_TRUE;
}



static HI_BOOL JPEG_HDEC_OpenDev(const struct jpeg_decompress_struct *cinfo)
{

     HI_S32 Ret = HI_SUCCESS;
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     if (NULL == pJpegHandle){
         return HI_FALSE;
     }

#ifdef CONFIG_JPEG_TEST_SUPPORT
     if (HI_TRUE == pJpegHandle->stTestInfo.bExitBeforeOpenDev){
         JPEG_TRACE("\n===============================================================\n");
         JPEG_TRACE("== test hard decode to soft decode before open device error\n");
         JPEG_TRACE("===============================================================\n");
         return HI_FAILURE;
     }
#endif

     if (pJpegHandle->s32JpegDev < 0){
          pJpegHandle->s32JpegDev = open(JPG_DEV, O_RDWR | O_SYNC);
     }
     if (pJpegHandle->s32JpegDev < 0){
         return HI_FALSE;
     }

     pJpegHandle->pJpegRegVirAddr = (volatile char*  )mmap(NULL,JPGD_REG_LENGTH,
                                                           PROT_READ | PROT_WRITE,MAP_SHARED,
                                                           pJpegHandle->s32JpegDev,(off_t)0);
     if (MAP_FAILED == pJpegHandle->pJpegRegVirAddr){
         goto ERROR_EXIT_CLOSE_DEV;
     }

#ifdef CONFIG_JPEG_TEST_SUPPORT
     if (HI_TRUE == pJpegHandle->stTestInfo.bExitAfterOpenDev){
         JPEG_TRACE("\n===============================================================\n");
         JPEG_TRACE("== test hard decode to soft decode after open device error\n");
         JPEG_TRACE("===============================================================\n");
         goto ERROR_EXIT_DESTORY_ALL;
     }
#endif

#ifndef CONFIG_JPEG_CSC_DISABLE
     if (gs_stJpegDecompressRes.s32CscDev < 0){
          gs_stJpegDecompressRes.s32CscDev = JPEG_HDEC_CSC_Open();
     }
     if (gs_stJpegDecompressRes.s32CscDev < 0){
         goto ERROR_EXIT_DESTORY_ALL;
     }
#endif

     return HI_TRUE;

ERROR_EXIT_DESTORY_ALL:
     if (NULL != pJpegHandle->pJpegRegVirAddr)
     {
         Ret = munmap((void*)pJpegHandle->pJpegRegVirAddr, JPGD_REG_LENGTH);
         if (HI_SUCCESS != Ret)
         {
            JPEG_TRACE("warning: munmap register address failure\n");
         }
         pJpegHandle->pJpegRegVirAddr = NULL;
     }

 ERROR_EXIT_CLOSE_DEV:
     if (pJpegHandle->s32JpegDev >= 0)
     {
         close(pJpegHandle->s32JpegDev);
         pJpegHandle->s32JpegDev = -1;
     }

     return HI_FALSE;
}



static HI_S32 JPEG_HDEC_SendStream(j_decompress_ptr cinfo)
{

     HI_S32 s32RetVal    =  HI_FAILURE;

#ifdef CONFIG_JPEG_OUTPUT_LUPIXSUM
     HI_U32 u32RegistLuaPixSum0 = 0;
     HI_U64 u64RegistLuaPixSum1 = 0;
#endif

     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

#ifdef CONFIG_JPEG_TEST_SUPPORT
     if (HI_TRUE == pJpegHandle->stTestInfo.bExitBeforeGetDev)
     {
         JPEG_TRACE("\n===============================================================\n");
         JPEG_TRACE("== test hard decode to soft decode before get device error\n");
         JPEG_TRACE("===============================================================\n");
         return HI_FAILURE;
     }
#endif

     s32RetVal = ioctl(pJpegHandle->s32JpegDev, CMD_JPG_GETDEVICE);
     if (HI_SUCCESS != s32RetVal)
     {
         return HI_FAILURE;
     }

#ifdef CONFIG_JPEG_TEST_SUPPORT
     if (HI_TRUE == pJpegHandle->stTestInfo.bExitAfterGetDev)
     {
         JPEG_TRACE("\n===============================================================\n");
         JPEG_TRACE("== test hard decode to soft decode after get device error\n");
         JPEG_TRACE("===============================================================\n");
         return HI_FAILURE;
     }
#endif

     s32RetVal = JPEG_HDEC_SetPara(cinfo);
     if (HI_SUCCESS != s32RetVal)
     {
         return HI_FAILURE;
     }

     pJpegHandle->bInHardDec = HI_TRUE;

     if (HI_FALSE == pJpegHandle->stHDecDataBuf.bUseInsideData)
     {
         s32RetVal = JPEG_HDEC_SendStreamFromCallBack(cinfo);
     }
     else
     {
     #ifdef CONFIG_JPEG_STREAMBUF_4ALIGN
         if (HI_TRUE == pJpegHandle->stHDecDataBuf.bNeedStreamReturn)
         {
             JPEG_HDEC_SendStreamFromReturnPhyMem(cinfo);
         }
         else if (HI_TRUE == pJpegHandle->stHDecDataBuf.bUserPhyMem)
     #else
         if (HI_TRUE == pJpegHandle->stHDecDataBuf.bUserPhyMem)
     #endif
         {
             s32RetVal = JPEG_HDEC_SendStreamFromPhyMem(cinfo);
         }
         else if (HI_TRUE == pJpegHandle->stHDecDataBuf.bUseFileData)
         {
             s32RetVal = JPEG_HDEC_SendStreamFromFile(cinfo);
         }
         else
         {
             s32RetVal = JPEG_HDEC_SendStreamFromVirMem(cinfo);
         }
     }

     if (HI_FAILURE == s32RetVal)
     {
          return HI_FAILURE;
     }


#ifdef CONFIG_JPEG_OUTPUT_LUPIXSUM
     if (HI_TRUE == pJpegHandle->bLuPixSum)
     {
         u32RegistLuaPixSum0 = (HI_U32)JPEG_HDEC_ReadReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_LPIXSUM0);
         u64RegistLuaPixSum1 = (HI_U64)(JPEG_HDEC_ReadReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_LPIXSUM1) & 0xf);
         pJpegHandle->u64LuPixValue = (HI_U64)((u64RegistLuaPixSum1<<32) | u32RegistLuaPixSum0);
     }
#endif

     pJpegHandle->bHdecEnd  = HI_TRUE;

     JPEG_HDEC_TEST_SaveYUVSP(cinfo);

     return HI_SUCCESS;
}



HI_S32 JPEG_HDEC_OutUserBuf(j_decompress_ptr cinfo,JDIMENSION max_lines, HI_CHAR *scanlines)
{
     HI_U32 u32Cnt           = 0;
     HI_U32 u32SrcStride     = 0;
     HI_U32 u32DstStride     = 0;
     HI_S32 s32BufSrcLength  = 0;
     HI_CHAR *pDstBuf         = NULL;
     HI_CHAR *pSrcBuf         = NULL;
     HI_S32 s32Ret            = 0;

     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     if ((max_lines+(cinfo->output_scanline)) > (cinfo->output_height)){
         max_lines = (cinfo->output_height) - (cinfo->output_scanline);
     }

     s32Ret = JPEG_HDEC_NeedCpyDataToUserBuffer(cinfo,max_lines);
     if (HI_FAILURE != s32Ret){
         return s32Ret;
     }

     u32SrcStride  = pJpegHandle->stJpegSofInfo.u32DisplayStride;
     u32DstStride  = pJpegHandle->stOutDesc.stOutSurface.u32OutStride[0];

#ifdef  CONFIG_JPEG_HARDDEC2ARGB
     if (HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB){
        pSrcBuf = pJpegHandle->stMiddleSurface.pMiddleVir[0] + ((HI_S32)cinfo->output_scanline - pJpegHandle->stOutDesc.stCropRect.y) * (HI_S32)(u32SrcStride);
     }else
#endif
     {
         pSrcBuf   = pJpegHandle->stMiddleSurface.pOutVir + ((HI_S32)cinfo->output_scanline - pJpegHandle->stOutDesc.stCropRect.y) * (HI_S32)(u32SrcStride);
     }

     if(NULL != scanlines){
         pDstBuf = scanlines;
     }else{
         pDstBuf = pJpegHandle->stOutDesc.stOutSurface.pOutVir[0] + ((HI_S32)cinfo->output_scanline - pJpegHandle->stOutDesc.stCropRect.y) * (HI_S32)(u32DstStride);
     }

     s32BufSrcLength = (cinfo->output_components) * (pJpegHandle->stOutDesc.stCropRect.w);
     for (u32Cnt = 0; u32Cnt < max_lines; u32Cnt++){
         HI_GFX_Memcpy(pDstBuf,pSrcBuf,(size_t)s32BufSrcLength);
         (cinfo->output_scanline)++;
     }

     return (HI_S32)max_lines;
}


static inline HI_S32 JPEG_HDEC_NeedCpyDataToUserBuffer(j_decompress_ptr cinfo,JDIMENSION max_lines)
{
    HI_U32 u32Cnt = 0;
    JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

    if( (HI_FALSE == pJpegHandle->bCSCEnd) || (HI_TRUE == pJpegHandle->stOutDesc.stOutSurface.bUserPhyMem)){
        for(u32Cnt = 0; u32Cnt < max_lines; u32Cnt++){
            (cinfo->output_scanline)++;
        }
        return (HI_S32)max_lines;
    }

    if(   (HI_TRUE == pJpegHandle->stOutDesc.bCrop)
        &&(  ((HI_S32)(cinfo->output_scanline) < pJpegHandle->stOutDesc.stCropRect.y)
           ||((HI_S32)(cinfo->output_scanline+1) > (pJpegHandle->stOutDesc.stCropRect.h + pJpegHandle->stOutDesc.stCropRect.y)))){
        for(u32Cnt = 0; u32Cnt < max_lines; u32Cnt++){
            (cinfo->output_scanline)++;
        }
        return (HI_S32)max_lines;
    }

    return HI_FAILURE;
}

HI_VOID JPEG_HDEC_SetLowDelayBufferDate(j_decompress_ptr cinfo)
{
#ifndef CONFIG_JPEG_LOW_DELAY_SUPPORT
     UNUSED(cinfo);
#else
     HI_U32* pu32VirLineBuf = NULL;
     JPEG_HDEC_HANDLE_S_PTR pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     if (NULL == pJpegHandle)
     {
        return;
     }

     if (HI_TRUE != pJpegHandle->bLowDelayEn)
     {
        return;
     }

     pu32VirLineBuf = (HI_U32*)pJpegHandle->pVirLineBuf;
     if (NULL == pu32VirLineBuf)
     {
        return;
     }

     *pu32VirLineBuf       = pJpegHandle->stJpegSofInfo.u32YMcuHeight;
     *(pu32VirLineBuf + 4) = pJpegHandle->stJpegSofInfo.u32CMcuHeight;
#endif
     return;
}
