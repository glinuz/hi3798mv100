/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_mem.c
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

#include  "hi_jpeg_config.h"
#include  "jpeg_hdec_mem.h"
#include  "jpeg_hdec_api.h"
#include  "jpeg_hdec_error.h"
#include  "hi_gfx_sys.h"

/***************************** Macro Definition ********************************/

/** the first class is jpeg */
/** CNcomment:第一级内存为jpeg分区 */
#ifdef CONFIG_GFX_MMU_SUPPORT
#define MMZ_TAG          "iommu"
#else
#define MMZ_TAG          "jpeg"
#endif
/** the second class is jpeg */
/** CNcomment:第二级内存为graphics分区 */
//#define MMZ_TAG_1        "graphics"
/** the last class is jpeg */
/** CNcomment:最后一级内存为整个MMZ分区 */
//#define MMZ_TAG_2        ""

/** the module name */
/** CNcomment:分配给jpeg模块的名字，这里可以通过mmz proc来查看分配给谁了 */
#define MMZ_MODULE       "JPEG_STREAM_OUT_BUF"


#if defined(CONFIG_JPEG_ANDROID_DEBUG_ENABLE) && defined(CONFIG_JPEG_DEBUG_INFO)
#define LOG_TAG    "libjpeg"
#endif

/***************************** Structure Definition ****************************/

/***************************** Global Variable declaration *********************/

/***************************** API forward declarations ************************/

#ifdef CONFIG_JPEG_USE_MEMINFO_TO_CHECK
static inline HI_S32 compare_meminfo(const HI_VOID *pSrc, const HI_VOID *pDst);
#endif


/***************************** API realization *********************************/


/*****************************************************************************
* func          : JPEG_HDEC_AllocDecodeMemory
* description   : alloc hard decode need memory
                  CNcomment: 分配解码需要的内存     CNend\n
* param[in]     : cinfo       CNcomment: 解码对象   CNend\n
* retval        : HI_TRUE     CNcomment: support    CNend\n
* retval        : HI_FALSE    CNcomment: unsupport  CNend\n
* others:       : NA
*****************************************************************************/
HI_BOOL JPEG_HDEC_AllocDecodeMemory(j_decompress_ptr cinfo)
{
     HI_S32 Ret = HI_SUCCESS;
#ifdef CONFIG_JPEG_STREAMBUF_4ALIGN
     HI_U32 u32ConsumSize  = 0;
     HI_U32 u32DataPhyAddr = 0;
#endif
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

#ifdef CONFIG_GFX_MEM_ION
     pJpegHandle->s32MMZDev = gfx_mem_open();
     if (pJpegHandle->s32MMZDev < 0){
         return HI_FALSE;
     }
#endif

#ifdef CONFIG_JPEG_STREAMBUF_4ALIGN
     u32ConsumSize  = pJpegHandle->stHDecDataBuf.u32ConsumeDataSize - cinfo->src->bytes_in_buffer;
     u32DataPhyAddr = pJpegHandle->stHDecDataBuf.u32PhyBuf + u32ConsumSize;
     if (   (HI_FALSE == pJpegHandle->stHDecDataBuf.bUserPhyMem)
          ||(0 != (u32DataPhyAddr % JPGD_HDEC_MMZ_BUG_BUFFER_ALIGN))){
#else
     if (HI_FALSE == pJpegHandle->stHDecDataBuf.bUserPhyMem){
#endif
         Ret = JPEG_HDEC_GetStreamMem(pJpegHandle,JPGD_HARD_BUFFER);
     }
     if (HI_SUCCESS != Ret){
         goto ERROR_EXIT;
     }

     Ret = JPEG_HDEC_GetYUVMem(pJpegHandle);
     if (HI_SUCCESS != Ret){
         goto ERROR_EXIT;
     }

#ifdef CONFIG_JPEG_HARDDEC2ARGB
     if (HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB){
         Ret = JPEG_HDEC_GetMinMem(pJpegHandle);
     }
     if (HI_SUCCESS != Ret){
         goto ERROR_EXIT;
     }
#endif

     Ret = JPEG_HDEC_GetOutMem(cinfo);
     if (HI_SUCCESS != Ret){
         goto ERROR_EXIT;
     }

     return HI_TRUE;

ERROR_EXIT:

#ifdef CONFIG_GFX_MEM_ION
    if (pJpegHandle->s32MMZDev >= 0){
        gfx_mem_close(pJpegHandle->s32MMZDev);
        pJpegHandle->s32MMZDev = -1;
    }
#endif

     return HI_FALSE;
}


/*****************************************************************************
* func            : JPEG_HDEC_GetStreamMem
* description    : alloc the stream buffer mem
                  CNcomment: 分配码流buffer内存 CNend\n
* param[in]     : u32MemSize   CNcomment: 要分配的内存大小    CNend\n
* param[out]    : pOutPhyAddr  CNcomment: 分配得到的物理地址  CNend\n
* param[out]    : pOutVirAddr  CNcomment: 分配得到的虚拟地址  CNend\n
* retval        : HI_SUCCESS   CNcomment: 成功  CNend\n
* retval        : HI_FAILURE   CNcomment: 失败   CNend\n
* others:        : NA
*****************************************************************************/
HI_S32    JPEG_HDEC_GetStreamMem(JPEG_HDEC_HANDLE_S_PTR     pJpegHandle,const HI_U32 u32MemSize)
{


        HI_U32 u32PhyBuf = 0;
        HI_CHAR *pVirBuf = NULL;
        HI_U32 u32StreamSize = JPGD_STREAM_BUFFER;
        HI_BOOL bMemMMUType  = HI_TRUE;

        JPEG_ASSERT((0 == u32MemSize),HI_FAILURE);

      /**
            ** use the third class manage to alloc mem,the stream buffer should 64bytes align
            ** CNcomment: 使用三级分配管理来分配内存 CNend\n
           **/
#ifdef CONFIG_GFX_MEM_ION
        u32PhyBuf = HI_GFX_AllocMem(pJpegHandle->s32MMZDev,u32MemSize, JPGD_HDEC_MMZ_STREAM_BUFFER_ALIGN, (HI_CHAR*)MMZ_TAG, (HI_CHAR*)MMZ_MODULE, &(pJpegHandle->pSaveStreamMemHandle),&bMemMMUType);
#else
        u32PhyBuf = HI_GFX_AllocMem(u32MemSize, JPGD_HDEC_MMZ_STREAM_BUFFER_ALIGN, (HI_CHAR*)MMZ_TAG, (HI_CHAR*)MMZ_MODULE, &bMemMMUType);
#endif
        if(0 == u32PhyBuf){
            return HI_FAILURE;
        }
#ifdef CONFIG_GFX_MEM_ION
        pVirBuf = (HI_CHAR*)HI_GFX_MapCached(pJpegHandle->s32MMZDev,u32PhyBuf, pJpegHandle->pSaveStreamMemHandle);
#else
        pVirBuf = (HI_CHAR*)HI_GFX_MapCached(u32PhyBuf, bMemMMUType);
#endif
        if(NULL == pVirBuf){
            return HI_FAILURE;
        }

        pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf    = u32PhyBuf;
        pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf      = pVirBuf;
        /**
        ** use the virtual memory, every time should read data size
        ** CNcomment: 虚拟内存码流的时候每次需要读取的码流大小 CNend\n
        **/
        pJpegHandle->stHDecDataBuf.u32ReadDataSize          = u32StreamSize;

#ifdef CONFIG_GFX_MMU_SUPPORT
        if(HI_TRUE != bMemMMUType){
            return HI_FAILURE;
        }
#endif
        return HI_SUCCESS;

}



/*****************************************************************************
* func            : JPEG_HDEC_FreeStreamMem
* description    : free the stream buffer mem
                  CNcomment: 释放码流buffer内存 CNend\n
* param[in]     : pInPhyAddr    CNcomment: 要释放的码流buffer物理地址 CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID JPEG_HDEC_FreeStreamMem(JPEG_HDEC_HANDLE_S_PTR pJpegHandle)
{

      HI_S32 s32Ret       = HI_SUCCESS;

      if(NULL == pJpegHandle){
           JPEG_TRACE("%s :%s : %d (the pJpegHandle is NULL)\n",__FILE__,__FUNCTION__,__LINE__);
           return;
      }

      if( 0 == pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf){
           return;
      }

#ifdef CONFIG_GFX_MEM_ION
      s32Ret = HI_GFX_Unmap(pJpegHandle->s32MMZDev,pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf, pJpegHandle->pSaveStreamMemHandle);
      s32Ret|= HI_GFX_FreeMem(pJpegHandle->s32MMZDev,pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf, pJpegHandle->pSaveStreamMemHandle,HI_TRUE);
#else
      s32Ret = HI_GFX_Unmap(pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf, HI_TRUE);
      s32Ret |= HI_GFX_FreeMem(pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf, HI_TRUE);
#endif
      if(HI_SUCCESS != s32Ret){
          JPEG_TRACE("HI_GFX_Unmap or  HI_GFX_FreeMem FAILURE\n");
          return;
      }
      pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf    = 0;
      pJpegHandle->stHDecDataBuf.pSaveStreamVirBuf        = NULL;

}


/*****************************************************************************
* func            : JPEG_HDEC_GetYUVMem
* description    : get the hard decode output mem
                  CNcomment: 获取硬件解码输出的内存 CNend\n
* param[in]        : pJpegHandle   CNcomment: 解码器句柄 CNend\n
* retval        : HI_SUCCESS    CNcomment: 成功 CNend\n
* retval        : HI_FAILURE    CNcomment: 失败 CNend\n
* others:        : NA
*****************************************************************************/
HI_S32 JPEG_HDEC_GetYUVMem(JPEG_HDEC_HANDLE_S_PTR    pJpegHandle)
{


        HI_U32 u32MemSize   = 0;
        HI_U32 u32YUVPhy    = 0;
        HI_CHAR *pYUVVir    = NULL;
        HI_U32 u32Align     = 0;
        HI_S32 s32Ret       = HI_SUCCESS;
        HI_BOOL bMemMMUType = HI_TRUE;

        /**
         ** check whether to alloc jpeg hard decode middle mem
         ** CNcomment: 判断是否分配硬件解码的中间buffer CNend\n
         **/
#ifdef  CONFIG_JPEG_HARDDEC2ARGB
        if(   (HI_TRUE == pJpegHandle->bOutYCbCrSP || HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB)
            &&(HI_TRUE == pJpegHandle->stOutDesc.stOutSurface.bUserPhyMem)){
#else
        if(   (HI_TRUE == pJpegHandle->bOutYCbCrSP)
            &&(HI_TRUE == pJpegHandle->stOutDesc.stOutSurface.bUserPhyMem)){
#endif
         /**
          ** use user mem
          ** CNcomment: 使用用户内存 CNend\n
          **/
              pJpegHandle->stMiddleSurface.u32MiddlePhy[0] = pJpegHandle->stOutDesc.stOutSurface.u32OutPhy[0];
              pJpegHandle->stMiddleSurface.u32MiddlePhy[1] = pJpegHandle->stOutDesc.stOutSurface.u32OutPhy[1];
              pJpegHandle->stMiddleSurface.u32MiddlePhy[2] = pJpegHandle->stOutDesc.stOutSurface.u32OutPhy[2];
              pJpegHandle->stMiddleSurface.pMiddleVir[0] = pJpegHandle->stOutDesc.stOutSurface.pOutVir[0];
              pJpegHandle->stMiddleSurface.pMiddleVir[1] = pJpegHandle->stOutDesc.stOutSurface.pOutVir[1];
              pJpegHandle->stMiddleSurface.pMiddleVir[2] = pJpegHandle->stOutDesc.stOutSurface.pOutVir[2];
              return HI_SUCCESS;
        }


#ifdef CONFIG_JPEG_HARDDEC2ARGB
        if(HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB){
          /**
           ** 4bytes align just ok
           ** CNcomment: 4字节对齐就可以了 CNend\n
           **/
           u32Align   = JPGD_HDEC_MMZ_ARGB_BUFFER_ALIGN;
           u32MemSize = pJpegHandle->stJpegSofInfo.u32YSize;
        }else{
#else
        {
#endif
           u32Align   = JPGD_HDEC_MMZ_YUVSP_BUFFER_ALIGN;
           u32MemSize = pJpegHandle->stJpegSofInfo.u32YSize + pJpegHandle->stJpegSofInfo.u32CSize;
        }
        /**
         ** use the third class manage to alloc mem,the stream buffer should 128bytes align
         ** CNcomment: 使用三级分配管理来分配内存,buffer要128字节对齐 CNend\n
         **/
    #ifdef CONFIG_GFX_MEM_ION
        u32YUVPhy = HI_GFX_AllocMem(pJpegHandle->s32MMZDev,u32MemSize,u32Align,(HI_CHAR*)MMZ_TAG,(HI_CHAR*)MMZ_MODULE, &(pJpegHandle->pMiddleMemHandle),&bMemMMUType);
    #else
        u32YUVPhy = HI_GFX_AllocMem(u32MemSize,u32Align,(HI_CHAR*)MMZ_TAG,(HI_CHAR*)MMZ_MODULE, &bMemMMUType);
    #endif
        if(0 == u32YUVPhy){
             return HI_FAILURE;
        }

#if !defined(CONFIG_JPEG_TEST_SAVE_YUVSP_DATA) && !defined(CONFIG_JPEG_SOFTCSC_ENABLE)
/** if need save yuvsp data,should virtual address **/
        #ifdef  CONFIG_JPEG_HARDDEC2ARGB
        if( (HI_TRUE == pJpegHandle->bOutYCbCrSP) || (HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB)){
        #else
        if(HI_TRUE == pJpegHandle->bOutYCbCrSP){
        #endif
#else
        {
#endif

        #ifdef CONFIG_GFX_MEM_ION
            pYUVVir = (HI_CHAR *)HI_GFX_MapCached(pJpegHandle->s32MMZDev,u32YUVPhy, pJpegHandle->pMiddleMemHandle);
        #else
            pYUVVir = (HI_CHAR *)HI_GFX_MapCached(u32YUVPhy, bMemMMUType);
        #endif
            if (NULL == pYUVVir){
                return HI_FAILURE;
            }

        #ifdef CONFIG_GFX_MEM_ION
            s32Ret = HI_GFX_Flush(pJpegHandle->s32MMZDev,u32YUVPhy, pJpegHandle->pMiddleMemHandle);
        #else
            s32Ret = HI_GFX_Flush(u32YUVPhy, bMemMMUType);
        #endif
            if(HI_SUCCESS != s32Ret){
                return HI_FAILURE;
            }
            pJpegHandle->stMiddleSurface.pMiddleVir[0] = pYUVVir;
            pJpegHandle->stMiddleSurface.pMiddleVir[1] = pYUVVir + pJpegHandle->stJpegSofInfo.u32YSize;
        }

        pJpegHandle->stMiddleSurface.u32MiddlePhy[0] = u32YUVPhy;
        pJpegHandle->stMiddleSurface.u32MiddlePhy[1] = u32YUVPhy + pJpegHandle->stJpegSofInfo.u32YSize;


#ifdef CONFIG_GFX_MMU_SUPPORT
        if(HI_TRUE != bMemMMUType){
            return HI_FAILURE;
        }
#endif

        return HI_SUCCESS;


}

/*****************************************************************************
* func            : JPEG_HDEC_FreeYUVMem
* description    : free the hard decode output mem
                  CNcomment: 释放硬件解码输出的地址  CNend\n
* param[in]        : pJpegHandle   CNcomment: 解码器句柄 CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID JPEG_HDEC_FreeYUVMem(JPEG_HDEC_HANDLE_S_PTR pJpegHandle)
{

        HI_S32 s32Ret = HI_SUCCESS;


        if(NULL == pJpegHandle){
            JPEG_TRACE("%s :%s : %d (the pJpegHandle is NULL)\n",__FILE__,__FUNCTION__,__LINE__);
            return;
        }

#ifdef  CONFIG_JPEG_HARDDEC2ARGB
        if(   (HI_TRUE == pJpegHandle->bOutYCbCrSP || (HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB))
            &&(HI_TRUE == pJpegHandle->stOutDesc.stOutSurface.bUserPhyMem)){
#else
        if(   (HI_TRUE == pJpegHandle->bOutYCbCrSP)
            &&(HI_TRUE == pJpegHandle->stOutDesc.stOutSurface.bUserPhyMem)){

#endif
       /**
        ** use user mem
        ** CNcomment: 使用用户内存 CNend\n
        **/
            return;
        }

        if(0 == pJpegHandle->stMiddleSurface.u32MiddlePhy[0]){
            return;
        }

#if !defined(CONFIG_JPEG_TEST_SAVE_YUVSP_DATA) && !defined(CONFIG_JPEG_SOFTCSC_ENABLE)
    #ifdef  CONFIG_JPEG_HARDDEC2ARGB
        if( (HI_TRUE == pJpegHandle->bOutYCbCrSP) || (HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB)){
    #else
        if(HI_TRUE == pJpegHandle->bOutYCbCrSP){
    #endif
#else
    {
#endif
        #ifdef CONFIG_GFX_MEM_ION
            s32Ret = HI_GFX_Unmap(pJpegHandle->s32MMZDev,pJpegHandle->stMiddleSurface.u32MiddlePhy[0], pJpegHandle->pMiddleMemHandle);
        #else
            s32Ret = HI_GFX_Unmap(pJpegHandle->stMiddleSurface.u32MiddlePhy[0], HI_TRUE);
        #endif
            if(HI_SUCCESS != s32Ret){
                JPEG_TRACE("HI_GFX_Unmap FAILURE\n");
                return;
            }
    }

#ifdef CONFIG_GFX_MEM_ION
        s32Ret = HI_GFX_FreeMem(pJpegHandle->s32MMZDev,pJpegHandle->stMiddleSurface.u32MiddlePhy[0], pJpegHandle->pMiddleMemHandle,HI_TRUE);
#else
        s32Ret = HI_GFX_FreeMem(pJpegHandle->stMiddleSurface.u32MiddlePhy[0], HI_TRUE);
#endif
        if(HI_SUCCESS != s32Ret){
            JPEG_TRACE("HI_GFX_FreeMem FAILURE\n");
            return;
        }
        pJpegHandle->stMiddleSurface.u32MiddlePhy[0]  = 0;
        pJpegHandle->stMiddleSurface.u32MiddlePhy[1]  = 0;
        pJpegHandle->stMiddleSurface.pMiddleVir[0]  = NULL;
        pJpegHandle->stMiddleSurface.pMiddleVir[1]  = NULL;

}


#ifdef CONFIG_JPEG_HARDDEC2ARGB
/*****************************************************************************
* func            : JPEG_HDEC_GetMinMem
* description    : get dec output argb min memory
                  CNcomment: 获取硬件解码输出为ARGB的行buffer CNend\n
* param[in]        : pJpegHandle   CNcomment: 解码器句柄 CNend\n
* retval        : HI_SUCCESS    CNcomment: 成功 CNend\n
* retval        : HI_FAILURE    CNcomment: 失败 CNend\n
* others:        : NA
*****************************************************************************/
HI_S32 JPEG_HDEC_GetMinMem(JPEG_HDEC_HANDLE_S_PTR pJpegHandle)
{

        HI_U32 u32MemSize   = 0;
        HI_U32 u32MinPhy    = 0;
        HI_BOOL bMemMMUType = HI_TRUE;

           u32MemSize = pJpegHandle->stJpegSofInfo.u32RGBSizeReg;

        /**
          ** use the third class manage to alloc mem,the min buffer should 128bytes align
         ** CNcomment: 使用三级分配管理来分配内存,argb行buffer要128字节对齐 CNend\n
         **/
#ifdef CONFIG_GFX_MEM_ION
        u32MinPhy = HI_GFX_AllocMem(pJpegHandle->s32MMZDev,u32MemSize,JPGD_HDEC_MMZ_YUVSP_BUFFER_ALIGN,(HI_CHAR*)MMZ_TAG,(HI_CHAR*)MMZ_MODULE, &(pJpegHandle->pMinMemHandle),&bMemMMUType);
#else
        u32MinPhy = HI_GFX_AllocMem(u32MemSize,JPGD_HDEC_MMZ_YUVSP_BUFFER_ALIGN,(HI_CHAR*)MMZ_TAG,(HI_CHAR*)MMZ_MODULE, &bMemMMUType);
#endif
        if(0 == u32MinPhy){
             return HI_FAILURE;
        }

        pJpegHandle->u32MinPhyBuf = u32MinPhy;

#ifdef CONFIG_GFX_MMU_SUPPORT
        if(HI_TRUE != bMemMMUType){
            return HI_FAILURE;
        }
#endif

        return HI_SUCCESS;


}

/*****************************************************************************
* func            : JPEG_HDEC_FreeMinMem
* description    : free dec output argb min memory
                  CNcomment: 释放硬件解码输出为ARGB的行buffer  CNend\n
* param[in]        : pJpegHandle   CNcomment: 解码器句柄 CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID JPEG_HDEC_FreeMinMem(JPEG_HDEC_HANDLE_S_PTR pJpegHandle)
{

        HI_S32 s32Ret = HI_SUCCESS;

        if(NULL == pJpegHandle){
             JPEG_TRACE("%s :%s : %d (the pJpegHandle is NULL)\n",__FILE__,__FUNCTION__,__LINE__);
             return;
        }

        if(0 == pJpegHandle->u32MinPhyBuf){
           return;
        }

#ifdef CONFIG_GFX_MEM_ION
        s32Ret = HI_GFX_FreeMem(pJpegHandle->s32MMZDev,pJpegHandle->u32MinPhyBuf, pJpegHandle->pMinMemHandle,HI_TRUE);
#else
        s32Ret = HI_GFX_FreeMem(pJpegHandle->u32MinPhyBuf,HI_TRUE);
#endif
        if(HI_SUCCESS != s32Ret){
           JPEG_TRACE("HI_GFX_FreeMem FAILURE\n");
           return;
        }

        pJpegHandle->u32MinPhyBuf  = 0;

}
#endif


/*****************************************************************************
* func            : JPEG_HDEC_GetOutMem
* description    : get the output buffer
                  CNcomment: 分配最终输出的内存      CNend\n
* param[in]        : cinfo         CNcomment: 解码对象  CNend\n
* retval        : HI_SUCCESS    CNcomment: 成功          CNend\n
* retval        : HI_FAILURE    CNcomment: 失败          CNend\n
* others:        : NA
*****************************************************************************/
HI_S32 JPEG_HDEC_GetOutMem(const struct jpeg_decompress_struct *cinfo)
{

        HI_U32 u32OutStride = 0;
        HI_U32 u32MemSize   = 0;
        HI_U32 u32OutPhy    = 0;
        HI_CHAR* pOutVir    = NULL;
        HI_S32 s32Ret       = HI_SUCCESS;
        HI_BOOL bMemMMUType = HI_TRUE;

        JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);


#ifdef CONFIG_JPEG_HARDDEC2ARGB
        if(HI_TRUE == pJpegHandle->bOutYCbCrSP || HI_TRUE  == pJpegHandle->bDecOutColorSpaecXRGB){
#else
        if(HI_TRUE == pJpegHandle->bOutYCbCrSP){
#endif
        /**
        ** shoule not csc,so not alloc output mem
        ** CNcomment: 不需要颜色空间转换，所以就不需要分配输出buffer CNend\n
        **/
            return HI_SUCCESS;
        }

        if(HI_TRUE == pJpegHandle->stOutDesc.stOutSurface.bUserPhyMem){
        /**
        ** use user mem
        ** CNcomment: 使用用户内存 CNend\n
        **/
            pJpegHandle->stMiddleSurface.u32OutPhy = pJpegHandle->stOutDesc.stOutSurface.u32OutPhy[0];
            pJpegHandle->stMiddleSurface.pOutVir = pJpegHandle->stOutDesc.stOutSurface.pOutVir[0];
            return HI_SUCCESS;
        }
        u32OutStride = pJpegHandle->stJpegSofInfo.u32DisplayStride;
        u32MemSize   = u32OutStride * ((HI_U32)pJpegHandle->stOutDesc.stCropRect.h);

        /**
        ** align depend the pixle
        ** CNcomment: 按照像素对齐 CNend\n
        **/
        /** 3字节对齐是有问题的，因为map不上来**/
#ifdef CONFIG_GFX_MEM_ION
        u32OutPhy = HI_GFX_AllocMem(pJpegHandle->s32MMZDev,u32MemSize, JPGD_HDEC_MMZ_CSCOUT_BUFFER_ALIGN, (HI_CHAR*)MMZ_TAG, (HI_CHAR*)MMZ_MODULE, &(pJpegHandle->pOutMemHandle),&bMemMMUType);
#else
        u32OutPhy = HI_GFX_AllocMem(u32MemSize, JPGD_HDEC_MMZ_CSCOUT_BUFFER_ALIGN, (HI_CHAR*)MMZ_TAG, (HI_CHAR*)MMZ_MODULE, &bMemMMUType);
#endif
        if(0 == u32OutPhy){
            return HI_FAILURE;
        }

#ifdef CONFIG_GFX_MEM_ION
        pOutVir = (HI_CHAR*)HI_GFX_MapCached(pJpegHandle->s32MMZDev,u32OutPhy, pJpegHandle->pOutMemHandle);
#else
        pOutVir = (HI_CHAR*)HI_GFX_MapCached(u32OutPhy, bMemMMUType);
#endif
        if (NULL == pOutVir){
            return HI_FAILURE;
        }

#ifdef CONFIG_GFX_MEM_ION
        s32Ret = HI_GFX_Flush(pJpegHandle->s32MMZDev,u32OutPhy, pJpegHandle->pOutMemHandle);
#else
        s32Ret = HI_GFX_Flush(u32OutPhy, bMemMMUType);
#endif
        if(HI_SUCCESS != s32Ret){
            return HI_FAILURE;
        }
        /**
        ** 要是用户没有设置输出图像大小则就使用默认的输出，也就是只能1/2/4/8四种缩放
        **/
        pJpegHandle->stMiddleSurface.u32OutPhy =  u32OutPhy;
        pJpegHandle->stMiddleSurface.pOutVir   =  pOutVir;

#ifdef CONFIG_GFX_MMU_SUPPORT
        if(HI_TRUE != bMemMMUType){
            return HI_FAILURE;
        }
#endif

        return HI_SUCCESS;


}

/*****************************************************************************
* func            : JPEG_HDEC_FreeOutMem
* description    : free the output buf
                  CNcomment: 释放最终输出的内存        CNend\n
* param[in]        : pJpegHandle   CNcomment: 解码器句柄  CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID JPEG_HDEC_FreeOutMem(JPEG_HDEC_HANDLE_S_PTR     pJpegHandle)
{

        HI_S32 s32Ret       = HI_SUCCESS;

        if(NULL == pJpegHandle){
            JPEG_TRACE("%s :%s : %d (the pJpegHandle is NULL)\n",__FILE__,__FUNCTION__,__LINE__);
            return;
        }
#ifdef CONFIG_JPEG_HARDDEC2ARGB
        if(HI_TRUE == pJpegHandle->bOutYCbCrSP || HI_TRUE  == pJpegHandle->bDecOutColorSpaecXRGB){
#else
        if(HI_TRUE == pJpegHandle->bOutYCbCrSP){
#endif
            return;
        }

        if(HI_TRUE == pJpegHandle->stOutDesc.stOutSurface.bUserPhyMem){
            return;
        }


        if(0 == pJpegHandle->stMiddleSurface.u32OutPhy){
            return;
        }

#ifdef CONFIG_GFX_MEM_ION
        s32Ret = HI_GFX_Unmap(pJpegHandle->s32MMZDev,pJpegHandle->stMiddleSurface.u32OutPhy, pJpegHandle->pOutMemHandle);
        s32Ret |= HI_GFX_FreeMem(pJpegHandle->s32MMZDev,pJpegHandle->stMiddleSurface.u32OutPhy, pJpegHandle->pOutMemHandle,HI_TRUE);
#else
        s32Ret = HI_GFX_Unmap(pJpegHandle->stMiddleSurface.u32OutPhy, HI_TRUE);
        s32Ret |= HI_GFX_FreeMem(pJpegHandle->stMiddleSurface.u32OutPhy, HI_TRUE);
#endif
        if(HI_SUCCESS != s32Ret){
            JPEG_TRACE("HI_GFX_Unmap or HI_GFX_FreeMem FAILURE\n");
            return;
        }
        pJpegHandle->stMiddleSurface.u32OutPhy = 0;
        pJpegHandle->stMiddleSurface.pOutVir   = NULL;

}

#ifdef CONFIG_GFX_JPGE_ENC_ENABLE
/*****************************************************************************
 * func          : JPGE_HENC_GetEncMem
 * description     : get encode need memory
                   CNcomment: 分配编码需要的内存        CNend\n
 * param[ou]     : pu32OutPhy   CNcomment: 返回物理地址      CNend\n
 * param[ou]     : pOutVir      CNcomment: 返回虚拟地址      CNend\n
 * retval         : NA
 * others:         : NA
 *****************************************************************************/
 HI_S32 JPGE_HENC_GetEncMem(JPGE_HENC_HANDLE_S_PTR pJpgeHandle)
{

        HI_S32 s32Ret = HI_SUCCESS;
        HI_S32 s32MemSize = 0;
        HI_U32 u32Phy = 0;
        HI_CHAR* pVir = NULL;
        HI_S32 s32Cnt = 0;
        HI_BOOL bMemMMUType = HI_FALSE;

        if( (HI_TRUE == pJpgeHandle->stStreamInfo.bUserPhyMem) && (HI_TRUE == pJpgeHandle->bCanUserPhyMem)){
            s32MemSize = pJpgeHandle->EncIn.OutBufSize;
        }else{
            s32MemSize =  pJpgeHandle->stStreamInfo.u32InSize[0] \
                        + pJpgeHandle->stStreamInfo.u32InSize[1] \
                        + pJpgeHandle->stStreamInfo.u32InSize[2] \
                        + pJpgeHandle->EncIn.OutBufSize;
        }

#ifdef CONFIG_GFX_MEM_ION
        u32Phy = HI_GFX_AllocMem(pJpgeHandle->s32MMZDev,s32MemSize, JPGD_HDEC_MMZ_STREAM_BUFFER_ALIGN, (HI_CHAR*)JPGE_TAG, (HI_CHAR*)JPGE_MODULE, &(pJpgeHandle->pMemHandle),&bMemMMUType);
#else
        u32Phy = HI_GFX_AllocMem(s32MemSize, JPGD_HDEC_MMZ_STREAM_BUFFER_ALIGN, (HI_CHAR*)JPGE_TAG, (HI_CHAR*)JPGE_MODULE, &bMemMMUType);
#endif
        if(0 == u32Phy){
            JPEG_TRACE("=====%s %d failure\n",__FUNCTION__,__LINE__);
            return HI_FAILURE;
        }
#ifdef CONFIG_GFX_MEM_ION
        pVir = (HI_CHAR*)HI_GFX_MapCached(pJpgeHandle->s32MMZDev,u32Phy, pJpgeHandle->pMemHandle);
#else
        pVir = (HI_CHAR*)HI_GFX_Map(u32Phy,bMemMMUType);
#endif
        if(NULL == pVir){
            #ifdef CONFIG_GFX_MEM_ION
                HI_GFX_FreeMem(pJpgeHandle->s32MMZDev,u32Phy, pJpgeHandle->pMemHandle,bMemMMUType);
            #else
                HI_GFX_FreeMem(u32Phy,bMemMMUType);
            #endif
            u32Phy = 0;
            pVir   = NULL;
            return HI_FAILURE;
        }
        HI_GFX_Memset(pVir,0,s32MemSize);

#ifdef CONFIG_GFX_MEM_ION
        s32Ret = HI_GFX_Flush(pJpgeHandle->s32MMZDev,u32Phy, pJpgeHandle->pMemHandle);
#else
        s32Ret = HI_GFX_Flush(u32Phy,bMemMMUType);
#endif
        if(HI_SUCCESS != s32Ret){
            #ifdef CONFIG_GFX_MEM_ION
                HI_GFX_Unmap(pJpgeHandle->s32MMZDev,(u32Phy, pJpgeHandle->pMemHandle);
                HI_GFX_FreeMem(pJpgeHandle->s32MMZDev,u32Phy, pJpgeHandle->pMemHandle);
            #else
                HI_GFX_Unmap(u32Phy,bMemMMUType);
                HI_GFX_FreeMem(u32Phy,bMemMMUType);
            #endif
            JPEG_TRACE("=====%s %d failure\n",__FUNCTION__,__LINE__);
            return HI_FAILURE;
        }

        if( (HI_TRUE == pJpgeHandle->stStreamInfo.bUserPhyMem) && (HI_TRUE == pJpgeHandle->bCanUserPhyMem)){
            for(s32Cnt = 0; s32Cnt < MAX_IN_PIXEL_COMPONENT_NUM; s32Cnt++){
                pJpgeHandle->stMiddleBuf.u32InPhy[s32Cnt] = pJpgeHandle->stStreamInfo.u32InPhy[s32Cnt];
                pJpgeHandle->stMiddleBuf.pInVir[s32Cnt]   = pJpgeHandle->stStreamInfo.pInVir[s32Cnt];
            }
            pJpgeHandle->stMiddleBuf.u32InPhy[3] = u32Phy;
            pJpgeHandle->stMiddleBuf.pInVir[3]   = pVir;
            return HI_SUCCESS;
        }

        pJpgeHandle->stMiddleBuf.u32InPhy[0] = u32Phy;
        pJpgeHandle->stMiddleBuf.pInVir[0]   = pVir;
        pJpgeHandle->stMiddleBuf.u32InPhy[1] = u32Phy + pJpgeHandle->stStreamInfo.u32InSize[0];
        pJpgeHandle->stMiddleBuf.pInVir[1]   = pVir + pJpgeHandle->stStreamInfo.u32InSize[0];
        pJpgeHandle->stMiddleBuf.u32InPhy[2] = pJpgeHandle->stMiddleBuf.u32InPhy[1] + pJpgeHandle->stStreamInfo.u32InSize[1];
        pJpgeHandle->stMiddleBuf.pInVir[2]   = pJpgeHandle->stMiddleBuf.pInVir[1] + pJpgeHandle->stStreamInfo.u32InSize[1];
        pJpgeHandle->stMiddleBuf.u32InPhy[3] = pJpgeHandle->stMiddleBuf.u32InPhy[2] + pJpgeHandle->stStreamInfo.u32InSize[2];
        pJpgeHandle->stMiddleBuf.pInVir[3]   = pJpgeHandle->stMiddleBuf.pInVir[2] + pJpgeHandle->stStreamInfo.u32InSize[2];

#ifdef CONFIG_GFX_MMU_SUPPORT
        if(HI_TRUE != bMemMMUType){
            return HI_FAILURE;
        }
#endif

        return HI_SUCCESS;

}


/*****************************************************************************
 * func          : JPGE_HENC_FreeEncMem
 * description     : free encode memory
                   CNcomment: 释放编码内存                   CNend\n
 * param[in]     : u32PhyAddr   CNcomment: 要释放的物理地址  CNend\n
 * retval         : NA
 * others:         : NA
 *****************************************************************************/
 HI_S32 JPGE_HENC_FreeEncMem(JPGE_HENC_HANDLE_S_PTR pJpgeHandle)
{

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Phy = 0;
    HI_S32 s32Cnt = 0;

    if( (HI_TRUE == pJpgeHandle->stStreamInfo.bUserPhyMem) && (HI_TRUE == pJpgeHandle->bCanUserPhyMem)){
        u32Phy = pJpgeHandle->stMiddleBuf.u32InPhy[3];
    }else{
        u32Phy = pJpgeHandle->stMiddleBuf.u32InPhy[0];
    }
    if(0 == u32Phy){
        return HI_SUCCESS;
    }

#ifdef CONFIG_GFX_MEM_ION
    s32Ret = HI_GFX_Unmap(pJpgeHandle->s32MMZDev,u32Phy, pJpgeHandle->pMemHandle);
    s32Ret = HI_GFX_FreeMem(pJpgeHandle->s32MMZDev,u32Phy, pJpgeHandle->pMemHandle,HI_TRUE);
#else
    s32Ret = HI_GFX_Unmap(u32Phy,HI_TRUE);
    s32Ret = HI_GFX_FreeMem(u32Phy,HI_TRUE);
#endif
    if(HI_SUCCESS != s32Ret){
       JPEG_TRACE("=====%s %d failure\n",__FUNCTION__,__LINE__);
       return HI_FAILURE;
    }
    for(s32Cnt = 0; s32Cnt < MAX_IN_PIXEL_COMPONENT_NUM + 1; s32Cnt++){
        pJpgeHandle->stMiddleBuf.u32InPhy[s32Cnt] = 0;
    }

    return HI_SUCCESS;

}
#endif



HI_S32 JPEG_HDEC_CheckStreamMemType(const struct jpeg_decompress_struct *cinfo,HI_UCHAR* pVirBuf)
{
     HI_U32 u32Size     = 0;
     HI_S32 s32Ret      = HI_SUCCESS;
     HI_U32 u32PhyAddr  = 0;
#ifndef CONFIG_GFX_MEM_ION
     HI_BOOL bMmu       = HI_FALSE;
#endif
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

#ifdef CONFIG_GFX_MEM_ION
     s32Ret = HI_GFX_GetPhyaddr(pJpegHandle->s32MMZDev,(HI_VOID*)pVirBuf, &u32PhyAddr, &u32Size, NULL);
     pJpegHandle->stHDecDataBuf.bUserPhyMem = HI_FALSE;
     return s32Ret;
#else
     s32Ret = HI_GFX_GetPhyaddr((HI_VOID*)pVirBuf, &u32PhyAddr,&u32Size, &bMmu);
     if(HI_SUCCESS == s32Ret){
          pJpegHandle->stHDecDataBuf.bUserPhyMem = HI_TRUE;
          pJpegHandle->stHDecDataBuf.u32PhyBuf   = u32PhyAddr;
          return HI_SUCCESS;
     }else{
         return HI_FAILURE;
     }
#endif
}



HI_S32 JPEG_HDEC_GetMemFromMemInfo(HI_U64 *pu64MemSize)
{

#ifndef CONFIG_JPEG_USE_MEMINFO_TO_CHECK
      UNUSED(pu64MemSize);
#else
      HI_U64 MEMINFO_Buffers  = 0L;
      HI_U64 MEMINFO_Cached   = 0L;
      HI_U64 MEMINFO_MemFree  = 0L;
      HI_U64 MEMINFO_MemTotal = 0L;
      HI_CHAR NameBuf[256]     = {0};

      HI_CHAR MemInfoBuf[2048] = {0};
      HI_S32  s32MemInfoFd        = -1;
      HI_S32  s32ReadMemInfoSize = 0;
      HI_S32  s32LeekOffset       = -1;
      JPEG_LEAVE_MEMINFO Findme  ={NameBuf, NULL};
      JPEG_LEAVE_MEMINFO *pFound = NULL;
      HI_CHAR *pHead = NULL;
      HI_CHAR *pTail = NULL;

      /**
       **这个赋值要注意顺序，因为是二分查找
       **/
      JPEG_LEAVE_MEMINFO MemInfo[] =
                            {
                                  {"Buffers",       &MEMINFO_Buffers},
                                  {"Cached",       &MEMINFO_Cached},
                                  {"MemFree",       &MEMINFO_MemFree},
                                  {"MemTotal",       &MEMINFO_MemTotal}
                            };

      const HI_U32 u32MemInfoCount = sizeof(MemInfo) / sizeof(JPEG_LEAVE_MEMINFO);

      *pu64MemSize = MEMLEAVESIZE_DEFAULT;
      /**=========================================================================
                               read meminfo start
       ==========================================================================**/

      s32MemInfoFd = open(MEMINFO_FILE,O_RDONLY);
      if(s32MemInfoFd < 0){
            JPEG_TRACE("open %s failure\n",MEMINFO_FILE);
              return HI_FAILURE;
      }

      s32LeekOffset = lseek(s32MemInfoFd, 0L, SEEK_SET);
      if(s32LeekOffset < 0){
            close(s32MemInfoFd);
            JPEG_TRACE("lseek %s failure\n",MEMINFO_FILE);
              return HI_FAILURE;
      }

      s32ReadMemInfoSize = read(s32MemInfoFd, MemInfoBuf, (sizeof(MemInfoBuf) - 1));
      if (s32ReadMemInfoSize <= 0 || s32ReadMemInfoSize > (HI_S32)(sizeof(MemInfoBuf) - 1)){
            close(s32MemInfoFd);
            JPEG_TRACE("read %s failure\n",MEMINFO_FILE);
              return HI_FAILURE;
      }else{
              MemInfoBuf[s32ReadMemInfoSize] = '\0';
      }

      close(s32MemInfoFd);

      /**=========================================================================
              read meminfo end,the proc message in MemInfoBuf
       ==========================================================================**/
      pHead = MemInfoBuf;
      for(;;){
            pTail = strchr(pHead, ':');
            if(!pTail) {
                break;
            }
            *pTail = '\0';

            if(strlen(pHead) >= sizeof(NameBuf)){
               pHead = pTail+1;
               goto NextFind;
            }

            strncpy(NameBuf, pHead, sizeof(NameBuf));
            NameBuf[sizeof(NameBuf) - 1] = '\0';

            pFound = (JPEG_LEAVE_MEMINFO *)bsearch(&Findme,                     \
                                                   MemInfo,                     \
                                                   u32MemInfoCount,             \
                                                   sizeof(JPEG_LEAVE_MEMINFO),  \
                                                   compare_meminfo);
            pHead = pTail + 1;
            if(!pFound){
                goto NextFind;
            }
            /**
             **转成10进制
             **/
            *(pFound->pSize) = strtoul(pHead,&pTail,10);

            NextFind:
                pTail = strchr(pHead, '\n');
                if(!pTail){
                    break;
                }
                   pHead = pTail + 1;

     }

     *pu64MemSize = MEMINFO_MemFree;

#if 0
     sys_tem("cat /proc/meminfo"); /*if want use these code , modify the sys_xx as sysxx ,this modify is for avoid unsafe function checking*/
     JPEG_TRACE("\n===================================================\n");
     JPEG_TRACE("MEMINFO_MemTotal = %lld\n",MEMINFO_MemTotal);
     JPEG_TRACE("MEMINFO_MemFree  = %lld\n",*pu64MemSize);
     JPEG_TRACE("MEMINFO_Buffers  = %lld\n",MEMINFO_Buffers);
     JPEG_TRACE("MEMINFO_Cached   = %lld\n",MEMINFO_Cached);
     JPEG_TRACE("===================================================\n");
#endif

#endif
     return HI_SUCCESS;

}

#ifdef CONFIG_JPEG_USE_MEMINFO_TO_CHECK
static inline HI_S32 compare_meminfo(const HI_VOID *pSrc, const HI_VOID *pDst)
{
      HI_U32 u32SrcSize  = (HI_U32)strlen(((const JPEG_LEAVE_MEMINFO*)pSrc)->pName);
      HI_U32 u32DstSize  = (HI_U32)strlen(((const JPEG_LEAVE_MEMINFO*)pDst)->pName);
      return strncmp(((const JPEG_LEAVE_MEMINFO*)pSrc)->pName, ((const JPEG_LEAVE_MEMINFO*)pDst)->pName,u32SrcSize > u32DstSize ? u32SrcSize : u32DstSize);
}
#endif
