/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_sentstream.c
Version            : Initial Draft
Author            : y00181162
Created            : 2014/06/20
Description        : dispose the inside stream
                  CNcomment: 码流的处理方式 CNend\n
Function List     :


History           :
Date                Author                Modification
2014/06/20            y00181162            Created file
******************************************************************************/

/*********************************add include here******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "jpeglib.h"
#include "jpegint.h"
#include "jdatasrc.h"

#include "jpeg_hdec_adp.h"
#include "jpeg_hdec_api.h"
#include "jpeg_hdec_rwreg.h"
#include "hi_jpeg_config.h"
#include "hi_drv_jpeg.h"
#include "hi_jpeg_reg.h"
#include "jpeg_hdec_mem.h"
#include "hi_gfx_sys.h"

#include "hi_jpeg_hdec_test.h"

#ifdef CONFIG_JPEG_SUSPEND
#include "jpeg_hdec_suspend.h"
#endif

#ifdef CONFIG_JPEG_TEST_HARD_DEC_CAPA
#include <sys/time.h>
#endif

#if defined(CONFIG_JPEG_ANDROID_DEBUG_ENABLE) && defined(CONFIG_JPEG_DEBUG_INFO)
#define LOG_TAG "libjpeg"
#endif

/***************************** Macro Definition ******************************/

#define JPG_RESUME_VALUE           0x01
#define JPG_EOF_VALUE              0x02
#define JPG_INTTYPE_DELAY_TIME    10000 /** 10s **/

#ifdef CONFIG_JPEG_TEST_HARD_DEC_CAPA
#define HARDDEC_TINIT()   struct timeval tv_start, tv_end; unsigned int time_cost,line_start
#define HARDDEC_TSTART()  gettimeofday(&tv_start, NULL);line_start = __LINE__
#define HARDDEC_TEND()     \
gettimeofday(&tv_end, NULL); \
time_cost = ((tv_end.tv_usec - tv_start.tv_usec) + (tv_end.tv_sec - tv_start.tv_sec)*1000000); \
JPEG_TRACE("=============================================================================\n"); \
JPEG_TRACE("FROM LINE: %d TO LINE: %d COST: %d us\n",line_start, __LINE__, time_cost);         \
JPEG_TRACE("=============================================================================\n")
#endif

/******************** to see which include file we want to use***************/



/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/

/******************************* API forward declarations *******************/
static inline HI_VOID JPEG_HDEC_SetDecStatus(j_decompress_ptr cinfo);
static inline HI_VOID JPEG_HDEC_CpyMmzBufToSysBuf(j_decompress_ptr cinfo);


/******************************* API release ********************************/

/*****************************************************************************
* func          : JPEG_HDEC_CheckOut
* description   : check whether should Memcpy to output buffer
                   CNcomment:  判断是否需要输出到输出buffer中    CNend\n
* param[in]     : *pJpegHandle   CNcomment: 解码器句柄          CNend\n
* retval        : HI_SUCCESS  CNcomment:  成功           CNend\n
* retval        : HI_FAILURE  CNcomment:  失败           CNend\n
* others:       : NA
*****************************************************************************/
static HI_S32 JPEG_HDEC_CheckOut(const JPEG_HDEC_HANDLE_S *pJpegHandle)
{
#ifdef CONFIG_JPEG_HARDDEC2ARGB
        if(   (HI_TRUE == pJpegHandle->stOutDesc.stOutSurface.bUserPhyMem)
            &&((HI_TRUE == pJpegHandle->bOutYCbCrSP) ||(HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB)))
#else
        if(  (HI_TRUE == pJpegHandle->stOutDesc.stOutSurface.bUserPhyMem)
           &&(HI_TRUE == pJpegHandle->bOutYCbCrSP))
#endif
        {
            return HI_FAILURE;
        }

#ifdef CONFIG_JPEG_HARDDEC2ARGB
        if((HI_TRUE != pJpegHandle->bOutYCbCrSP) && (HI_TRUE != pJpegHandle->bDecOutColorSpaecXRGB))
#else
        if(HI_TRUE != pJpegHandle->bOutYCbCrSP)
#endif
        {
            return HI_FAILURE;
        }

        return HI_SUCCESS;

}

/*****************************************************************************
* func            : JPEG_HDEC_GetIntStatus
* description    : get interrupt status
                  CNcomment:  获取中断状态             CNend\n
* param[in]        : *pJpegHandle   CNcomment: 解码器句柄 CNend\n
* retval        : HI_SUCCESS  CNcomment:  成功           CNend\n
* retval        : HI_FAILURE  CNcomment:  失败           CNend\n
* others:        : NA
*****************************************************************************/
static HI_S32 JPEG_HDEC_GetIntStatus(const JPEG_HDEC_HANDLE_S *pJpegHandle, JPG_INTTYPE_E *pIntType, HI_U32 u32TimeOut)
{

        HI_S32 s32RetVal;
        JPG_GETINTTYPE_S GetIntType;

        GetIntType.IntType = JPG_INTTYPE_NONE;
        GetIntType.TimeOut = u32TimeOut;

        s32RetVal = ioctl(pJpegHandle->s32JpegDev, CMD_JPG_GETINTSTATUS, &GetIntType);

        if (HI_SUCCESS != s32RetVal)
        {
            return s32RetVal;
        }
        *pIntType = GetIntType.IntType;

#ifdef CONFIG_JPEG_TEST_SUPPORT
        if(HI_TRUE == pJpegHandle->stTestInfo.bExitContinueDec && JPG_INTTYPE_CONTINUE == GetIntType.IntType){
            JPEG_TRACE("\n===============================================================\n");
            JPEG_TRACE("== test hard decode to soft decode after continue decode error\n");
            JPEG_TRACE("===============================================================\n");
            *pIntType = JPG_INTTYPE_ERROR;
            return HI_FAILURE;
        }
#endif

        return HI_SUCCESS;

}
/*****************************************************************************
* func            : JPEG_HDEC_SuspendDispose
* description    : dispose the suspend
                  CNcomment:  待机处理     CNend\n
* param[in]     : cinfo               CNcomment:  解码对象               CNend\n
* param[in]     : pbResumeOk           CNcomment:  待机已经唤醒         CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SuspendDispose(const struct jpeg_decompress_struct *cinfo,HI_BOOL *pbResumeOk)
{
#ifdef CONFIG_JPEG_SUSPEND
        HI_BOOL bSuspendSingal   = HI_FALSE;
        HI_BOOL bResumeSingal    =  HI_FALSE;
        JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

        JPEG_HDEC_GetSuspendSignal(pJpegHandle,&bSuspendSingal);
        JPEG_HDEC_GetResumeSignal(pJpegHandle,&bResumeSingal);
        if( (HI_TRUE == bSuspendSingal) ||  (HI_TRUE == bResumeSingal))
        {
            JPEG_HDEC_Resume(cinfo);
            *pbResumeOk = HI_TRUE;
        }
#else
        UNUSED(cinfo);
        UNUSED(pbResumeOk);
#endif
}


/*****************************************************************************
* func            : JPEG_HDEC_SendStreamFromPhyMem
* description    : get the stream from physics memory
                  CNcomment:  码流来源连续的物理内存的处理方式     CNend\n
* param[in]     : cinfo       CNcomment:  解码对象       CNend\n
* retval        : HI_SUCCESS  CNcomment:  成功           CNend\n
* retval        : HI_FAILURE  CNcomment:  失败           CNend\n
* others:        : NA
*****************************************************************************/

#ifdef CONFIG_JPEG_STREAMBUF_4ALIGN
/**
 ** before 3716CV200 EC,the save buffer should 4bytes align
 ** CNcomment:3716CV200 EC之前的存储码流buffer起始地址需要4字节对齐 CNend\n
 **/
HI_S32 JPEG_HDEC_SendStreamFromPhyMem(j_decompress_ptr cinfo)
{

      JPG_INTTYPE_E eIntStatus        = JPG_INTTYPE_NONE;
      HI_S32 s32Cnt                    = 0;
      HI_U32 u32ConsumSize            = 0;
      HI_U64 u64LeaveSize             = 0;
      HI_U32 u32AlignSize             = 0;
      HI_U32 u32DecSize               = 0;
      HI_BOOL bStartDec                 = HI_TRUE;
      HI_U32  u32DataPhyAddr            = 0;
      HI_CHAR* pDataVirAddr             = NULL;
      HI_U32  u32SaveStreamPhyAddr      = 0;
      HI_CHAR* pSaveStreamVirAddr       = NULL;
      HI_U32   u32SaveStreamEndPhyAddr  = 0;

      HI_S32 s32Ret = HI_SUCCESS;

      HI_BOOL bReachEOF = HI_FALSE;
      HI_BOOL b4Align   = HI_FALSE;/**< whether 4bytes align *//**<CNcomment:是否4字节对齐*/

      JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

      /**
      ** calc the consume stream size
      ** CNcomment:进入硬件之前消耗的码流数等于每次读码流累加-4096中剩余的码流数 CNend\n
      **/
      u32ConsumSize = pJpegHandle->stHDecDataBuf.u32ConsumeDataSize - cinfo->src->bytes_in_buffer;
      u64LeaveSize  = pJpegHandle->stHDecDataBuf.u64DataSize - ((HI_U64)u32ConsumSize);

      u32DataPhyAddr          = pJpegHandle->stHDecDataBuf.u32PhyBuf   + u32ConsumSize;
      pDataVirAddr            = pJpegHandle->stHDecDataBuf.pDataVirBuf + u32ConsumSize;
      u32SaveStreamPhyAddr    = pJpegHandle->stHDecDataBuf.u32PhyBuf   + u32ConsumSize;
      pSaveStreamVirAddr      = pJpegHandle->stHDecDataBuf.pDataVirBuf + u32ConsumSize;
      u32SaveStreamEndPhyAddr = pJpegHandle->stHDecDataBuf.u32PhyBuf   + pJpegHandle->stHDecDataBuf.u64DataSize;
      if(0 == u32DataPhyAddr % JPGD_HDEC_MMZ_BUG_BUFFER_ALIGN)
      {
          b4Align = HI_TRUE;
      }
      else
      {
          /**
          **第一次解码JPGD_HDEC_MMZ_ALIGN_16BYTES + 4字节对齐要解的码流大小，因为要是码流太少解不了
          **/
          u32SaveStreamPhyAddr = (u32SaveStreamPhyAddr + 16 + JPGD_HDEC_MMZ_BUG_BUFFER_ALIGN - 1) & (~(JPGD_HDEC_MMZ_BUG_BUFFER_ALIGN - 1));
          pSaveStreamVirAddr   = (HI_CHAR*)(((unsigned long)pSaveStreamVirAddr + 16 + JPGD_HDEC_MMZ_BUG_BUFFER_ALIGN - 1) & (~(JPGD_HDEC_MMZ_BUG_BUFFER_ALIGN - 1)));
          u32AlignSize         = u32SaveStreamPhyAddr - u32DataPhyAddr;
      }

      u32DecSize = (u64LeaveSize <= u32AlignSize) ? ((HI_U32)u64LeaveSize) : (u32AlignSize);
      bReachEOF  = (u64LeaveSize <= u32AlignSize) ? (HI_TRUE) : (HI_FALSE);

      if(HI_FALSE == b4Align)
      {
          HI_GFX_Memcpy(pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf,pDataVirAddr,u32DecSize);
          /** 刷码流数据 **/
#ifdef CONFIG_GFX_MEM_ION
          s32Ret = HI_GFX_Flush(pJpegHandle->s32MMZDev,pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf, pJpegHandle->pSaveStreamMemHandle);
#else
          s32Ret = HI_GFX_Flush(pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf,HI_TRUE);
#endif
      }

#ifdef CONFIG_JPEG_TEST_HARD_DEC_CAPA
      HARDDEC_TINIT();
      HARDDEC_TSTART();
#endif

      do
      {

          if(HI_TRUE == bStartDec && HI_FALSE == b4Align)
          {
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_STADD, pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf);
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ENDADD,pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf + u32DecSize);
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_RESUME,(bReachEOF ? JPG_EOF_VALUE : 0x0));
#ifdef CONFIG_JPEG_4KDDR_DISABLE
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x5);
#else
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x1);
#endif
          }
          else if(HI_TRUE == bStartDec && HI_TRUE == b4Align)
          {
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_STADD, u32SaveStreamPhyAddr);
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ENDADD,u32SaveStreamEndPhyAddr);
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_RESUME, 0x2);
#ifdef CONFIG_JPEG_4KDDR_DISABLE
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x5);
#else
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x1);
#endif
          }
          else
          {
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_STADD, u32SaveStreamPhyAddr);
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ENDADD,u32SaveStreamEndPhyAddr);
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_RESUME,(JPG_EOF_VALUE|JPG_RESUME_VALUE));
          }

          bStartDec  = HI_FALSE;
          bReachEOF  = HI_TRUE;

          eIntStatus = JPG_INTTYPE_ERROR;
          s32Ret = JPEG_HDEC_GetIntStatus(pJpegHandle, &eIntStatus, JPG_INTTYPE_DELAY_TIME);
          if (HI_SUCCESS != s32Ret)
          {
             return HI_FAILURE;
          }

          if (JPG_INTTYPE_CONTINUE == eIntStatus)
          {
              JPEG_HDEC_SuspendDispose(cinfo,&bResumeOk);
              continue;
          }

          if ( (JPG_INTTYPE_ERROR == eIntStatus) || (JPG_INTTYPE_FINISH == eIntStatus))
          {
              break;
          }

          return HI_FAILURE;

      }while(JPG_INTTYPE_FINISH != eIntStatus);

#ifdef CONFIG_JPEG_TEST_HARD_DEC_CAPA
      HARDDEC_TEND();
      /** himd.l 0xf8c40018 **/
      /** 残差的值 himd.l 0xf8c40008 **/
#endif

      JPEG_HDEC_SetDecStatus(cinfo);

      s32Ret = JPEG_HDEC_CheckOut(pJpegHandle);
      if(HI_SUCCESS != s32Ret)
      {
          return HI_SUCCESS;
      }

      JPEG_HDEC_CpyMmzBufToSysBuf(cinfo);

      return HI_SUCCESS;
}

HI_S32 JPEG_HDEC_SendStreamFromReturnPhyMem(j_decompress_ptr cinfo)
{

      JPG_INTTYPE_E eIntStatus = JPG_INTTYPE_NONE;
      HI_S32 s32Cnt            = 0;
      HI_U32 u32ConsumSize     = 0;
      HI_U64 u64LeaveSize      = 0;
      HI_U32 u32AlignSize      = 0;
      HI_U32 u32DecSize        = 0;
      HI_BOOL bStartDec        = HI_TRUE;
      HI_BOOL bUseReturn       = HI_FALSE;
      HI_U32  u32DataPhyAddr   = 0;
      HI_U32  u32DataStartPhyAddr = 0;
      HI_CHAR* pDataVirAddr = NULL;
      HI_U32  u32SaveStreamPhyAddr = 0;
      HI_CHAR* pSaveStreamVirAddr  = NULL;
      HI_U32   u32SaveStreamEndPhyAddr = 0;

      HI_S32 s32Ret = HI_SUCCESS;

      HI_BOOL bReachEOF = HI_FALSE;
      HI_BOOL bNeedContinue = HI_FALSE;

      JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

      /**
      ** calc the consume stream size
      ** CNcomment:进入硬件之前消耗的码流数等于每次读码流累加-4096中剩余的码流数 CNend\n
      **/
      if (pJpegHandle->stHDecDataBuf.u32ConsumeDataSize <= pJpegHandle->stHDecDataBuf.u64DataSize)
      {/** 剩余码流拿去copy然后解码 **/
          u32ConsumSize   = pJpegHandle->stHDecDataBuf.u32ConsumeDataSize - cinfo->src->bytes_in_buffer;
          pDataVirAddr    = pJpegHandle->stHDecDataBuf.pDataVirBuf + u32ConsumSize;
          u32DataPhyAddr  = pJpegHandle->stHDecDataBuf.u32PhyBuf   + u32ConsumSize;
          u32DecSize      = pJpegHandle->stHDecDataBuf.u64DataSize - u32ConsumSize;
          if (u32DecSize > (HI_U32)(JPGD_HARD_BUFFER))
          {
              u32DataStartPhyAddr = (u32DataPhyAddr + JPGD_HDEC_MMZ_YUVSP_BUFFER_ALIGN + JPGD_HDEC_MMZ_YUVSP_BUFFER_ALIGN - 1) & (~(JPGD_HDEC_MMZ_YUVSP_BUFFER_ALIGN - 1));
              u32DecSize = u32DataStartPhyAddr - u32DataPhyAddr;
              bUseReturn = HI_TRUE;
          }
          HI_GFX_Memcpy(pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf,pDataVirAddr,u32DecSize);
          #ifdef CONFIG_GFX_MEM_ION
          s32Ret = HI_GFX_Flush(pJpegHandle->s32MMZDev,pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf, pJpegHandle->pSaveStreamMemHandle);
          #else
          s32Ret = HI_GFX_Flush(pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf,HI_TRUE);
          #endif

          u32SaveStreamPhyAddr = (HI_FALSE == bUseReturn) ? (pJpegHandle->stHDecDataBuf.u32StartBufPhy) : (u32DataStartPhyAddr);

          u32SaveStreamEndPhyAddr = u32SaveStreamPhyAddr + pJpegHandle->stHDecDataBuf.s32StreamReturnLen;
          bNeedContinue = HI_TRUE;
          bReachEOF   = HI_FALSE;
      }
      else
      {
          u32ConsumSize = pJpegHandle->stHDecDataBuf.u32ConsumeDataSize - pJpegHandle->stHDecDataBuf.u64DataSize - cinfo->src->bytes_in_buffer;
          u64LeaveSize  = pJpegHandle->stHDecDataBuf.s32StreamReturnLen - ((HI_U64)u32ConsumSize);

          u32DataPhyAddr          = pJpegHandle->stHDecDataBuf.u32StartBufPhy    + u32ConsumSize;
          pDataVirAddr            = pJpegHandle->stHDecDataBuf.pDataVirBufReturn + u32ConsumSize;
          u32SaveStreamPhyAddr    = pJpegHandle->stHDecDataBuf.u32StartBufPhy    + u32ConsumSize;
          pSaveStreamVirAddr      = pJpegHandle->stHDecDataBuf.pDataVirBufReturn + u32ConsumSize;
          u32SaveStreamEndPhyAddr = pJpegHandle->stHDecDataBuf.u32StartBufPhy    + pJpegHandle->stHDecDataBuf.s32StreamReturnLen;

          bNeedContinue = (0 == u32DataPhyAddr % JPGD_HDEC_MMZ_BUG_BUFFER_ALIGN) ? (HI_FALSE) : (HI_TRUE);

          if (0 != u32DataPhyAddr % JPGD_HDEC_MMZ_BUG_BUFFER_ALIGN)
          {
              u32SaveStreamPhyAddr = (u32SaveStreamPhyAddr + 16 + JPGD_HDEC_MMZ_BUG_BUFFER_ALIGN - 1) & (~(JPGD_HDEC_MMZ_BUG_BUFFER_ALIGN - 1));
              pSaveStreamVirAddr   = (HI_CHAR*)(((unsigned long)pSaveStreamVirAddr + 16 + JPGD_HDEC_MMZ_BUG_BUFFER_ALIGN - 1) & (~(JPGD_HDEC_MMZ_BUG_BUFFER_ALIGN - 1)));
              u32AlignSize         = u32SaveStreamPhyAddr - u32DataPhyAddr;
          }

          u32DecSize = (u64LeaveSize <= u32AlignSize) ? ((HI_U32)u64LeaveSize) : (u32AlignSize);
          bReachEOF  = (u64LeaveSize <= u32AlignSize) ? (HI_TRUE) : (HI_FALSE);

          if (HI_TRUE == bNeedContinue)
          {
              HI_GFX_Memcpy(pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf,pDataVirAddr,u32DecSize);
              #ifdef CONFIG_GFX_MEM_ION
              s32Ret = HI_GFX_Flush(pJpegHandle->s32MMZDev,pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf, pJpegHandle->pSaveStreamMemHandle);
              #else
              s32Ret = HI_GFX_Flush(pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf,HI_TRUE);
              #endif
          }
      }

      do
      {

          if(HI_TRUE == bStartDec && HI_TRUE == bNeedContinue)
          {
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_STADD, pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf);
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ENDADD,pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf + u32DecSize);
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_RESUME,(bReachEOF ? JPG_EOF_VALUE : 0x0));
#ifdef CONFIG_JPEG_4KDDR_DISABLE
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x5);
#else
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x1);
#endif

          }
          else if(HI_TRUE == bStartDec && HI_FALSE == bNeedContinue)
          {
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_STADD, u32SaveStreamPhyAddr);
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ENDADD,u32SaveStreamEndPhyAddr);
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_RESUME, 0x2);
#ifdef CONFIG_JPEG_4KDDR_DISABLE
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x5);
#else
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x1);
#endif

          }
          else
          {
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_STADD, u32SaveStreamPhyAddr);
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ENDADD,u32SaveStreamEndPhyAddr);
              JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_RESUME,(JPG_EOF_VALUE|JPG_RESUME_VALUE));
          }

          bStartDec  = HI_FALSE;
          bReachEOF  = HI_TRUE;

          eIntStatus = JPG_INTTYPE_ERROR;
          (HI_VOID)JPEG_HDEC_GetIntStatus(pJpegHandle, &eIntStatus, JPG_INTTYPE_DELAY_TIME);
          if (JPG_INTTYPE_CONTINUE == eIntStatus)
          {
              continue;
          }

          if ( (JPG_INTTYPE_ERROR == eIntStatus) || (JPG_INTTYPE_FINISH == eIntStatus))
          {
              break;
          }

          return HI_FAILURE;

      }while(JPG_INTTYPE_FINISH != eIntStatus);

      JPEG_HDEC_SetDecStatus(cinfo);

      s32Ret = JPEG_HDEC_CheckOut(pJpegHandle);
      if(HI_SUCCESS != s32Ret)
      {
          return HI_SUCCESS;
      }

      JPEG_HDEC_CpyMmzBufToSysBuf(cinfo);

      return HI_SUCCESS;
}

#else



static HI_VOID JPEG_HDEC_PrintLowDelayLineNum(struct jpeg_decompress_struct *cinfo)
{
#if 1
      UNUSED(cinfo);
#else
      HI_CHAR *pVirLineBuf = NULL;
      HI_U32 u32YLineNum   = 0;
      HI_U32 u32UVLineNum  = 0;
      JPEG_HDEC_HANDLE_S_PTR pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

      UNUSED(u32YLineNum);
      UNUSED(u32UVLineNum);

      if ( (NULL == pJpegHandle) || (NULL == pJpegHandle->pVirLineBuf)){
          return;
      }
      pVirLineBuf = pJpegHandle->pVirLineBuf;

      /** 前16个字节的前4个字节 **/
      u32YLineNum  = (HI_U32)(*pVirLineBuf)|(HI_U32)(*(pVirLineBuf+1))<<8|(HI_U32)(*(pVirLineBuf+2))<<16|(HI_U32)(*(pVirLineBuf+3))<<24;

      /** 后16个字节的前4个字节 **/
      u32UVLineNum = (HI_U32)(*(pVirLineBuf + 16))|(HI_U32)(*(pVirLineBuf + 17))<<8|(HI_U32)(*(pVirLineBuf + 18))<<16|(HI_U32)(*(pVirLineBuf + 19))<<24;

      JPEG_TRACE("\n==================================================\n");
      JPEG_TRACE("u32YHeight  = %d u32UVHeight  = %d\n",pJpegHandle->stJpegSofInfo.u32YMcuHeight,pJpegHandle->stJpegSofInfo.u32CMcuHeight);
      JPEG_TRACE("u32YLineNum = %d u32UVLineNum = %d\n",u32YLineNum,u32UVLineNum);
      if (cinfo->output_height > 720){
         JPEG_TRACE("LowDelayIntLine = 8\n");
      }else{
         JPEG_TRACE("LowDelayIntLine = 4\n");
      }
      JPEG_TRACE("==================================================\n");
#endif
      return;
}


HI_S32 JPEG_HDEC_SendStreamFromPhyMem(j_decompress_ptr cinfo)
{

      HI_S32 s32Ret = HI_SUCCESS;
      HI_U32 u32ConsumSize            = 0;
      HI_U32 u32SaveStreamPhyAddr     = 0;
      HI_U32 u32SaveStreamEndPhyAddr  = 0;
      HI_U32 LowDelayValue            = 0;
      JPG_INTTYPE_E eIntStatus        = JPG_INTTYPE_ERROR;

      JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);


#ifdef CONFIG_JPEG_LOW_DELAY_SUPPORT
      if (HI_TRUE == pJpegHandle->bLowDelayEn)
      {
          LowDelayValue = 0x8;
      }
#endif

      if (HI_TRUE == pJpegHandle->stHDecDataBuf.bNeedStreamReturn){
          if (pJpegHandle->stHDecDataBuf.u32ConsumeDataSize <= pJpegHandle->stHDecDataBuf.u64DataSize){
              u32ConsumSize = pJpegHandle->stHDecDataBuf.u32ConsumeDataSize - cinfo->src->bytes_in_buffer;
              u32SaveStreamPhyAddr     =   pJpegHandle->stHDecDataBuf.u32PhyBuf      + u32ConsumSize;
              u32SaveStreamEndPhyAddr  =   pJpegHandle->stHDecDataBuf.u32StartBufPhy + pJpegHandle->stHDecDataBuf.s32StreamReturnLen;
          }else{
              u32ConsumSize = pJpegHandle->stHDecDataBuf.u32ConsumeDataSize - pJpegHandle->stHDecDataBuf.u64DataSize - cinfo->src->bytes_in_buffer;
              u32SaveStreamPhyAddr     =   pJpegHandle->stHDecDataBuf.u32StartBufPhy + u32ConsumSize;
              u32SaveStreamEndPhyAddr  =   pJpegHandle->stHDecDataBuf.u32StartBufPhy + pJpegHandle->stHDecDataBuf.s32StreamReturnLen;
          }
      }else{
          u32ConsumSize = pJpegHandle->stHDecDataBuf.u32ConsumeDataSize - cinfo->src->bytes_in_buffer;
          u32SaveStreamPhyAddr     =   pJpegHandle->stHDecDataBuf.u32PhyBuf + u32ConsumSize;
          u32SaveStreamEndPhyAddr  =   pJpegHandle->stHDecDataBuf.u32PhyBuf + pJpegHandle->stHDecDataBuf.u64DataSize;
      }

#ifdef CONFIG_JPEG_TEST_HARD_DEC_CAPA
     HARDDEC_TINIT();
     HARDDEC_TSTART();
#endif

      JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_STADD, u32SaveStreamPhyAddr & 0xffffffff);
      JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ENDADD,u32SaveStreamEndPhyAddr & 0xffffffff);

      JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_RESUME, 0x2);
#ifdef CONFIG_JPEG_4KDDR_DISABLE
      JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, (HI_S32)(0x5 | LowDelayValue));
#else
      JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, (HI_S32)(0x1 | LowDelayValue));
#endif

      s32Ret = JPEG_HDEC_GetIntStatus(pJpegHandle, &eIntStatus, JPG_INTTYPE_DELAY_TIME);
      if (HI_SUCCESS != s32Ret)
      {
          JPEG_HDEC_SetLowDelayBufferDate(cinfo);
          return HI_FAILURE;
      }

      if (JPG_INTTYPE_CONTINUE == eIntStatus)
      {
          JPEG_HDEC_SetLowDelayBufferDate(cinfo);
          return HI_FAILURE;
      }

#ifdef CONFIG_JPEG_TEST_HARD_DEC_CAPA
      HARDDEC_TEND();
#endif

      JPEG_HDEC_PrintLowDelayLineNum(cinfo);
      JPEG_HDEC_SetDecStatus(cinfo);

      s32Ret = JPEG_HDEC_CheckOut(pJpegHandle);
      if (HI_SUCCESS != s32Ret)
      {
          return HI_SUCCESS;
      }

      JPEG_HDEC_CpyMmzBufToSysBuf(cinfo);

      return HI_SUCCESS;
}
#endif



/*****************************************************************************
* func            : JPEG_HDEC_SendStreamFromVirMem
* description    : get the stream from virtual memory
                  CNcomment:  码流来源虚拟内存的处理方式   CNend\n
* param[in]     : cinfo       CNcomment:  解码对象     CNend\n
* retval        : HI_SUCCESS  CNcomment:  成功         CNend\n
* retval        : HI_FAILURE  CNcomment:  失败         CNend\n
* others:        : NA
*****************************************************************************/
HI_S32 JPEG_HDEC_SendStreamFromVirMem(j_decompress_ptr cinfo)
{


      JPG_INTTYPE_E eIntStatus = JPG_INTTYPE_NONE;
      HI_BOOL bReachEOF         = HI_FALSE;
      HI_BOOL bStartDec         = HI_FALSE;

      HI_BOOL bResumeOk       =  HI_FALSE;
#ifdef CONFIG_JPEG_SUSPEND
      HI_U32 u32ResumeSize    = 0;
#endif
      HI_U32 u32ReadDataSize  = 0;
      /**
       ** the continue stream size should big than 16bytes
       ** CNcomment:每一段续码流必须大于16个字节，保守 CNend\n
       **/
      HI_U32 u32NeedDecCnt               = 0;
      HI_U32 u32ConsumSize              = 0;
      HI_U64 u64LeaveSize               = 0;
      HI_U32 u32TotalSize               = 0;
      HI_CHAR* pDataVirAddr             = NULL;
      HI_U32 u32SaveStreamStartPhyAddr  = 0;
      HI_U32 u32SaveStreamEndPhyAddr    = 0;

      HI_S32 s32Ret = HI_SUCCESS;

      JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

      /**
      ** tell the hard the stream is end.
      ** CNcomment:这个是必须的，要告诉硬件码流已经读完了，否则硬件会一直解
      **           硬件本省无法判断读码流结束 CNend\n
      **/
      bReachEOF = HI_FALSE;

      u32ReadDataSize = JPGD_STREAM_BUFFER;
      u32ConsumSize   = pJpegHandle->stHDecDataBuf.u32ConsumeDataSize - cinfo->src->bytes_in_buffer;
      u64LeaveSize    = pJpegHandle->stHDecDataBuf.u64DataSize - ((HI_U64)u32ConsumSize);
      pDataVirAddr    = pJpegHandle->stHDecDataBuf.pDataVirBuf + u32ConsumSize;
      u32SaveStreamStartPhyAddr = pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf;

      do {
          /**
          ** the consume stream size
          ** CNcomment:消耗的码流数 CNend\n
          **/
          JPEG_HDEC_SuspendDispose(cinfo,&bResumeOk);

#ifdef CONFIG_JPEG_SUSPEND
          if(HI_TRUE == bResumeOk)
          {
            /**
             ** the consume stream size
             ** CNcomment:消耗的码流数 CNend\n
             **/
             u32ResumeSize = u32NeedDecCnt - pJpegHandle->u32ResByteConsum;
             HI_GFX_Memcpy(pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf,    \
                       pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf + pJpegHandle->u32ResByteConsum,\
                       u32ResumeSize);

             u32NeedDecCnt = u32ResumeSize;
             if(u32ResumeSize < 16 && HI_FALSE == bReachEOF)
             {
                /**
                    ** the consume stream size
                 ** CNcomment:不是最后一帧码流并且硬件需要的码流不够 CNend\n
                 **/
                 HI_GFX_Memcpy(pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf + u32ResumeSize, \
                        (pDataVirAddr + u32TotalSize),                                \
                        (u32ReadDataSize - u32ResumeSize));

                u32TotalSize += (u32ReadDataSize - u32ResumeSize);
                u32NeedDecCnt = u32ReadDataSize;
             }

          }
          else
#endif
          {
              u32NeedDecCnt  = u32ReadDataSize < ((HI_U32)u64LeaveSize - u32TotalSize) ? u32ReadDataSize : ((HI_U32)u64LeaveSize - u32TotalSize);
              HI_GFX_Memcpy(pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf, (pDataVirAddr + u32TotalSize),u32NeedDecCnt);
              u32TotalSize  += u32NeedDecCnt;
          }


          if(u32TotalSize >= u64LeaveSize)
          {
               bReachEOF = HI_TRUE;
          }
          if( (0xFF == (HI_UCHAR)pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf[0]) \
           && (0xD9 == (HI_UCHAR)pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf[1]))
          {
             /**
              ** strengthen the condition check
              ** CNcomment:加强条件判断 CNend\n
              **/
              bReachEOF = HI_TRUE;
          }

          /** 刷码流数据 **/
#ifdef CONFIG_GFX_MEM_ION
          (HI_VOID)HI_GFX_Flush(pJpegHandle->s32MMZDev,pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf, pJpegHandle->pSaveStreamMemHandle);
#else
          (HI_VOID)HI_GFX_Flush(pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf, HI_TRUE);
#endif

          u32SaveStreamEndPhyAddr   = u32SaveStreamStartPhyAddr + u32NeedDecCnt;

          JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_STADD, u32SaveStreamStartPhyAddr);
          JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ENDADD,u32SaveStreamEndPhyAddr);

#ifdef CONFIG_JPEG_SUSPEND
          if(HI_FALSE == bStartDec || HI_TRUE == bResumeOk)
#else
          if(HI_FALSE == bStartDec)
#endif
          {
               /**
                ** strengthen the condition check
                ** CNcomment:是否为当前帧的最后一段续码。0：不是；1：是 CNend\n
                **/
                JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_RESUME,(bReachEOF ? JPG_EOF_VALUE : 0x0));
                #ifdef CONFIG_JPEG_4KDDR_DISABLE
                JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x5);
                #else
                JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x1);
                #endif
                bStartDec  = HI_TRUE;
                bResumeOk  = HI_FALSE;

          }
          else
          {
                JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_RESUME,(bReachEOF ? (JPG_EOF_VALUE|JPG_RESUME_VALUE) : JPG_RESUME_VALUE));
          }

          eIntStatus = JPG_INTTYPE_ERROR;
          (HI_VOID)JPEG_HDEC_GetIntStatus(pJpegHandle, &eIntStatus, JPG_INTTYPE_DELAY_TIME);

          if (JPG_INTTYPE_CONTINUE == eIntStatus)
          {
              JPEG_HDEC_SuspendDispose(cinfo,&bResumeOk);
              continue;
          }

          if ( (JPG_INTTYPE_ERROR == eIntStatus) || (JPG_INTTYPE_FINISH == eIntStatus))
          {
              break;
          }

          return HI_FAILURE;

      } while (JPG_INTTYPE_FINISH != eIntStatus);

      JPEG_HDEC_SetDecStatus(cinfo);

      s32Ret = JPEG_HDEC_CheckOut(pJpegHandle);
      if(HI_SUCCESS != s32Ret)
      {
          return HI_SUCCESS;
      }

      JPEG_HDEC_CpyMmzBufToSysBuf(cinfo);

      return HI_SUCCESS;
}


/*****************************************************************************
* func            : JPEG_HDEC_SendStreamFromFile
* description    : CNcomment:  码流来源文件
* param[in]     : cinfo       CNcomment:  解码对象
* param[in]     : NA
* retval        : HI_SUCCESS  CNcomment:  成功
* retval        : HI_FAILURE  CNcomment:  失败
* others:        : NA
*****************************************************************************/
static inline HI_VOID JPEG_HDEC_SaveScen(j_decompress_ptr cinfo)
{
#ifdef CONFIG_JPEG_FPGA_TEST_SAVE_SCEN_ENABLE
     static HI_BOOL bStartFirst = HI_FALSE;
     if (HI_TRUE == pJpegHandle->bSaveScen){
         HI_JPEG_OpenScenFile(cinfo);
         HI_JPEG_OutScenData(cinfo,u32StreamStartPhyAddr,u32StreamEndPhyAddr,pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf,(HI_U64)u32NeedDecCnt,bStartFirst);
         bStartFirst = HI_TRUE;
     }
#endif
     return;
}


HI_S32 JPEG_HDEC_SendStreamFromFile(j_decompress_ptr cinfo)
{
     JPG_INTTYPE_E eIntStatus = JPG_INTTYPE_NONE;
     HI_BOOL bReachEOF       = HI_FALSE;
     HI_BOOL bStartDec       = HI_FALSE;
     HI_BOOL bResumeOk       =  HI_FALSE;
     HI_U32 u32ReadDataSize  = 0;
     HI_U32 u32ResumeSize    = 0;

     HI_U32 u32NeedDecCnt           = 0;
     HI_U32 u32StreamStartPhyAddr   = 0;
     HI_CHAR* pStreamStartVirAddr   = NULL;
     HI_U32 u32StreamEndPhyAddr     = 0;

     HI_S32 s32Ret = HI_SUCCESS;

     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     bReachEOF = HI_FALSE;

     pStreamStartVirAddr = pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf;
     u32ReadDataSize     = pJpegHandle->stHDecDataBuf.u32ReadDataSize;

     do {

         JPEG_HDEC_SuspendDispose(cinfo,&bResumeOk);

#ifdef CONFIG_JPEG_SUSPEND
         if (HI_TRUE == bResumeOk)
         {
             u32ResumeSize = u32NeedDecCnt - pJpegHandle->u32ResByteConsum;
             HI_GFX_Memcpy(pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf,    \
                    pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf + pJpegHandle->u32ResByteConsum,u32ResumeSize);
         }
#endif
         if (0 != cinfo->src->bytes_in_buffer)
         {
             u32NeedDecCnt = cinfo->src->bytes_in_buffer;
             HI_GFX_Memcpy(pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf, (char*)cinfo->src->next_input_byte,u32NeedDecCnt);

             bReachEOF = (  (0xFF == (*(cinfo->src->next_input_byte + u32NeedDecCnt - 2)))
                         && (0xD9 == (*(cinfo->src->next_input_byte + u32NeedDecCnt - 1)))) ? (HI_TRUE) : (bReachEOF);

             if (HI_FALSE == bReachEOF)
             {
                 pJpegHandle->stHDecDataBuf.bReadToDataBuf  = HI_TRUE;
                 pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf = pStreamStartVirAddr + u32NeedDecCnt;

                 pJpegHandle->stHDecDataBuf.u32ReadDataSize = \
                 pJpegHandle->stHDecDataBuf.u32ReadDataSize - u32NeedDecCnt;

                 (HI_VOID)(*cinfo->src->fill_input_buffer)(cinfo);

                 u32NeedDecCnt = u32NeedDecCnt + cinfo->src->bytes_in_buffer;
                 pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf     = pStreamStartVirAddr;
                 pJpegHandle->stHDecDataBuf.u32ReadDataSize = u32ReadDataSize;
             }

         }

         if (0 == cinfo->src->bytes_in_buffer)
         {
             pJpegHandle->stHDecDataBuf.bReadToDataBuf  = HI_TRUE;
             pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf = pStreamStartVirAddr + u32ResumeSize;
             pJpegHandle->stHDecDataBuf.u32ReadDataSize = pJpegHandle->stHDecDataBuf.u32ReadDataSize - u32ResumeSize;

             (HI_VOID)(*cinfo->src->fill_input_buffer)(cinfo);

             u32NeedDecCnt = (HI_TRUE == bResumeOk) ? (u32ResumeSize + cinfo->src->bytes_in_buffer) :
                                                      (cinfo->src->bytes_in_buffer);
             pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf  = pStreamStartVirAddr;
             pJpegHandle->stHDecDataBuf.u32ReadDataSize    = u32ReadDataSize;

         }

         bReachEOF = (HI_TRUE == bStartDec && u32NeedDecCnt < pJpegHandle->stHDecDataBuf.u32ReadDataSize) ? (HI_TRUE) : (bReachEOF);
         bReachEOF = (  (0xFF == (HI_UCHAR)pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf[0]) \
                      &&(0xD9 == (HI_UCHAR)pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf[1]) \
                      &&(2 == cinfo->src->bytes_in_buffer)) ? (HI_TRUE) : (bReachEOF);

         u32ResumeSize = 0;
         cinfo->src->bytes_in_buffer = 0;

#ifdef CONFIG_GFX_MEM_ION
         (HI_VOID)HI_GFX_Flush(pJpegHandle->s32MMZDev,pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf, pJpegHandle->pSaveStreamMemHandle);
#else
         (HI_VOID)HI_GFX_Flush(pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf, HI_TRUE);
#endif


         u32StreamStartPhyAddr = pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf;
         u32StreamEndPhyAddr   = u32StreamStartPhyAddr + u32NeedDecCnt;

         JPEG_HDEC_SaveScen(cinfo);

         JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_STADD, u32StreamStartPhyAddr);
         JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ENDADD,u32StreamEndPhyAddr);


#ifdef CONFIG_JPEG_SUSPEND
         if(HI_FALSE == bStartDec || HI_TRUE == bResumeOk)
#else
         if(HI_FALSE == bStartDec)
#endif
         {
             JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_RESUME,(bReachEOF ? JPG_EOF_VALUE : 0x0));
#ifdef CONFIG_JPEG_4KDDR_DISABLE
             JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x5);
#else
             JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x1);
#endif
             bStartDec  = HI_TRUE;
             bResumeOk  = HI_FALSE;
         }
         else
         {
             JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_RESUME,(bReachEOF ? (JPG_EOF_VALUE|JPG_RESUME_VALUE) : JPG_RESUME_VALUE));
         }


         eIntStatus = JPG_INTTYPE_ERROR;

         (HI_VOID)JPEG_HDEC_GetIntStatus(pJpegHandle, &eIntStatus, JPG_INTTYPE_DELAY_TIME);

         if (JPG_INTTYPE_CONTINUE == eIntStatus)
         {
             JPEG_HDEC_SuspendDispose(cinfo,&bResumeOk);
             continue;
         }

         if ( (JPG_INTTYPE_ERROR == eIntStatus) || (JPG_INTTYPE_FINISH == eIntStatus))
         {
             break;
         }

         goto FAIL;
     } while (JPG_INTTYPE_FINISH != eIntStatus);

     JPEG_HDEC_SetDecStatus(cinfo);

     s32Ret = JPEG_HDEC_CheckOut(pJpegHandle);
     if (HI_SUCCESS != s32Ret)
     {
         return HI_SUCCESS;
     }

     JPEG_HDEC_CpyMmzBufToSysBuf(cinfo);

     return HI_SUCCESS;

   FAIL:
     cinfo->src->bytes_in_buffer  = 0;
     pJpegHandle->stHDecDataBuf.bReadToDataBuf  = HI_FALSE;

     return HI_FAILURE;
}


static inline HI_BOOL JPEG_HDEC_CheckCallBackFuncStreamIsNull(j_decompress_ptr cinfo, HI_U32 *pReadStreamPos)
{
    HI_BOOL bReachEOF = HI_TRUE;

    if (cinfo->src->bytes_in_buffer)
    {
        return HI_FALSE;
    }

    (HI_VOID)cinfo->src->fill_input_buffer(cinfo);
    if (cinfo->src->bytes_in_buffer)
    {
        bReachEOF = HI_FALSE;
    }

    *pReadStreamPos = 0;

    return bReachEOF;
}


HI_S32 JPEG_HDEC_SendStreamFromCallBack(j_decompress_ptr cinfo)
{
     JPG_INTTYPE_E eIntStatus = JPG_INTTYPE_NONE;
     HI_BOOL bReachEOF        = HI_FALSE;
     HI_BOOL bStartDec        = HI_FALSE;
     HI_S32  s32ByteInBuffer  = 0;
     HI_BOOL bResumeOk        =  HI_FALSE;
     HI_U32 u32ReadDataSize   = 0;

     HI_U32 u32ReadSize            = 0;
     HI_U32 u32ReadPos             = 0;
     HI_U32 u32NeedDecCnt          = 0;
     HI_U32 u32StreamStartPhyAddr  = 0;
     HI_CHAR* pStreamStartVirAddr  = NULL;
     HI_U32 u32StreamEndPhyAddr    = 0;

     HI_S32 s32Ret = HI_SUCCESS;


     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);


     pJpegHandle->bFirstContinueStream = HI_TRUE;
     pStreamStartVirAddr = pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf;
     u32ReadDataSize     = pJpegHandle->stHDecDataBuf.u32ReadDataSize;

     s32ByteInBuffer = cinfo->src->bytes_in_buffer;
     do
     {
         JPEG_HDEC_SuspendDispose(cinfo,&bResumeOk);

#ifdef CONFIG_JPEG_SUSPEND
         if (HI_TRUE == bResumeOk){
             u32NeedDecCnt = u32ReadDataSize - pJpegHandle->u32ResByteConsum;
             HI_GFX_Memcpy(pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf,    \
             pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf + pJpegHandle->u32ResByteConsum,u32NeedDecCnt);
         }
#endif
         do
         {/** read stream data till as 1M bytes **/

             if (0 == cinfo->src->bytes_in_buffer)
             {
                 (HI_VOID)(*cinfo->src->fill_input_buffer)(cinfo);
                 if (0 == cinfo->src->bytes_in_buffer)
                 {
                     break;
                 }
                 u32ReadPos = 0;
             }

             u32ReadSize = (cinfo->src->bytes_in_buffer <= (u32ReadDataSize - u32NeedDecCnt)) ?
                           (cinfo->src->bytes_in_buffer) : (u32ReadDataSize - u32NeedDecCnt);

             HI_GFX_Memcpy(pStreamStartVirAddr + u32NeedDecCnt, (char*)cinfo->src->next_input_byte+u32ReadPos, u32ReadSize);

             u32ReadPos    += u32ReadSize;
             u32NeedDecCnt += u32ReadSize;
             cinfo->src->bytes_in_buffer -= u32ReadSize;

         } while (u32NeedDecCnt < u32ReadDataSize);

         pJpegHandle->s32ContinueSize = u32NeedDecCnt;

         bReachEOF  = JPEG_HDEC_CheckCallBackFuncStreamIsNull(cinfo, &u32ReadPos);

#ifdef CONFIG_GFX_MEM_ION
         (HI_VOID)HI_GFX_Flush(pJpegHandle->s32MMZDev,pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf, pJpegHandle->pSaveStreamMemHandle);
#else
         (HI_VOID)HI_GFX_Flush(pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf, HI_TRUE);
#endif
         u32StreamStartPhyAddr = pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf;
         u32StreamEndPhyAddr   = u32StreamStartPhyAddr + u32NeedDecCnt;

         JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_STADD, u32StreamStartPhyAddr);
         JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ENDADD,u32StreamEndPhyAddr);

         u32NeedDecCnt = 0;


#ifdef CONFIG_JPEG_SUSPEND
         if(HI_FALSE == bStartDec || HI_TRUE == bResumeOk)
#else
         if(HI_FALSE == bStartDec)
#endif
         {
             JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_RESUME,(bReachEOF ? JPG_EOF_VALUE : 0x0));
    #ifdef CONFIG_JPEG_4KDDR_DISABLE
             JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x5);
    #else
             JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_START, 0x1);
    #endif
             bStartDec  = HI_TRUE;
             bResumeOk  = HI_FALSE;
         }
         else
         {
             pJpegHandle->bFirstContinueStream = HI_FALSE;
             JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_RESUME,(bReachEOF ? (JPG_EOF_VALUE|JPG_RESUME_VALUE) : JPG_RESUME_VALUE));
         }


         eIntStatus = JPG_INTTYPE_ERROR;
         s32Ret = JPEG_HDEC_GetIntStatus(pJpegHandle, &eIntStatus, JPG_INTTYPE_DELAY_TIME);
         if (HI_SUCCESS != s32Ret)
         {
             goto FAIL;
         }

         if (JPG_INTTYPE_CONTINUE == eIntStatus)
         {
             JPEG_HDEC_SuspendDispose(cinfo,&bResumeOk);
             continue;
         }

         if ( (JPG_INTTYPE_ERROR == eIntStatus) || (JPG_INTTYPE_FINISH == eIntStatus)){
             break;
         }

         goto FAIL;
     } while (JPG_INTTYPE_FINISH != eIntStatus);


     JPEG_HDEC_SetDecStatus(cinfo);

     s32Ret = JPEG_HDEC_CheckOut(pJpegHandle);
     if (HI_SUCCESS != s32Ret){
         return HI_SUCCESS;
     }

     JPEG_HDEC_CpyMmzBufToSysBuf(cinfo);

     return HI_SUCCESS;

FAIL:
     cinfo->src->bytes_in_buffer = s32ByteInBuffer;
     pJpegHandle->stHDecDataBuf.bReadToDataBuf  = HI_FALSE;

     return HI_FAILURE;
}


static inline HI_VOID JPEG_HDEC_SetDecStatus(j_decompress_ptr cinfo)
{

     HI_S32 ComponentCnt = 0;
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     cinfo->output_scanline        = 0;
     cinfo->global_state           = DSTATE_STOPPING;
     cinfo->inputctl->eoi_reached  = HI_TRUE;
     cinfo->rec_outbuf_height      = 1;
     cinfo->MCUs_per_row           = (1 == pJpegHandle->u8Fac[0][0])?((cinfo->image_width   + JPEG_MCU_8ALIGN - 1)>>3) : ((cinfo->image_width  + JPEG_MCU_16ALIGN - 1)>>4);
     cinfo->MCU_rows_in_scan       = (1 == pJpegHandle->u8Fac[0][1])?((cinfo->image_height  + JPEG_MCU_8ALIGN - 1)>>3) : ((cinfo->image_height + JPEG_MCU_16ALIGN - 1)>>4);
     cinfo->blocks_in_MCU          =  cinfo->num_components;

     for (ComponentCnt = 0; ComponentCnt < cinfo->num_components; ComponentCnt++){
         cinfo->MCU_membership[ComponentCnt] = ComponentCnt;
     }

     return;
}


static inline HI_VOID JPEG_HDEC_CpyMmzBufToSysBuf(j_decompress_ptr cinfo)
{
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

#ifdef  CONFIG_JPEG_HARDDEC2ARGB
     if ( (0 != pJpegHandle->stJpegSofInfo.u32YSize) && (HI_TRUE != pJpegHandle->bDecOutColorSpaecXRGB))
     {
#else
     if (0 != pJpegHandle->stJpegSofInfo.u32YSize)
     {
#endif
         HI_GFX_Memcpy(pJpegHandle->stOutDesc.stOutSurface.pOutVir[0],pJpegHandle->stMiddleSurface.pMiddleVir[0],pJpegHandle->stJpegSofInfo.u32YSize);
     }

     if (0 != pJpegHandle->stJpegSofInfo.u32CSize)
     {
         HI_GFX_Memcpy(pJpegHandle->stOutDesc.stOutSurface.pOutVir[1],pJpegHandle->stMiddleSurface.pMiddleVir[1],pJpegHandle->stJpegSofInfo.u32CSize);
     }

     return;
}
