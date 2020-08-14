/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_drv_parse.h
Version            : Initial Draft
Author            :
Created            : 2015/02/02
Description        :
Function List     :


History           :
Date                Author                Modification
2015/02/02            y00181162              Created file
******************************************************************************/
#ifndef __JPEG_DRV_PARSE_H__
#define __JPEG_DRV_PARSE_H__


#include "hi_jpeg_config.h"

#ifdef CONFIG_JPEG_OMX_FUNCTION

/*********************************add include here******************************/
#ifndef HI_BUILD_IN_BOOT
    #include <linux/types.h>
    #include <linux/fs.h>
    #include <linux/interrupt.h>
    #include <linux/ioctl.h>
    #include <linux/delay.h>
    #include <linux/errno.h>
    #include <linux/mm.h>
    #include <linux/fcntl.h>
    #include <linux/slab.h>
    #include <asm/atomic.h>
    #include <asm/io.h>
#endif

#include "hi_type.h"
#include "hi_drv_jpeg.h"
#include "jpeg_drv_dec.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */



         /***************************** Macro Definition ******************************/
         /** \addtogroup      JPEG */
         /** @{ */    /** <!--[JPEG]*/

         #define JPEG_DRV_MCU_8ALIGN            8
         #define JPEG_DRV_MCU_16ALIGN             16

         /** Maximum number of data components */
         /** CNcomment:最大数据分量数 CNend */
         #define PIXEL_COMPONENT_NUM            3

         #define NUM_COMPS_IN_SCAN              4

         #define MAX_NUM_HUFF_TBLS              4

         #define MAX_NUM_QUANT_TBLS             4

         #define MAX_DCT_SIZE                   64
         /** @} */    /** <!-- ==== Macro Definition end ==== */

         /*************************** Enum Definition ****************************/

        /** \addtogroup   JPEG */
        /** @{ */    /** <!--[JPEG]*/

        typedef enum hiIMG_FMT_E
        {
            IMG_FMT_YUV400         = 0,   /**< yuv400     */
            IMG_FMT_BUTT0,              /**< NA         */
            IMG_FMT_BUTT1,              /**< NA         */
            IMG_FMT_YUV420,             /**< yuv420     */
            IMG_FMT_YUV422_21,          /**< yuv422_21  */
            IMG_FMT_YUV422_12,          /**< yuv422_12  */
            IMG_FMT_YUV444,             /**< yuv444     */
            IMG_FMT_YCCK,               /**< YCCK       */
            IMG_FMT_CMYK,               /**< CMYK       */
            IMG_FMT_BUTT
        }IMG_FMT_E;


        /** enum of mem type, mmz or mmu */
        /** CNcomment:判断内存类型，是mmu还是mmz CNend */
        typedef enum hiJPEG_MEMTYPE_E
        {
            JPEG_DRV_STREAM_MEM_MMU_TYPE            = 0X1,    /**< 0bit 码流内存为MMU类型              */
            JPEG_DRV_YOUTPUT_MEM_MMU_TYPE           = 0X2,    /**< 1bit Y分量输出内存为MMU类型         */
            JPEG_DRV_UVOUTPUT_MEM_MMU_TYPE          = 0X4,    /**< 2bit UV分量输出内存为MMU类型        */
            JPEG_DRV_XRGBSAMPLE0_READ_MEM_MMU_TYPE  = 0X8,    /**< RGB输出需要的上采样0buffer读类型    */
            JPEG_DRV_XRGBSAMPLE1_READ_MEM_MMU_TYPE  = 0X10,   /**< RGB输出需要的上采样1buffer读类型    */
            JPEG_DRV_XRGBSAMPLE0_WRITE_MEM_MMU_TYPE = 0X20,   /**< RGB输出需要的上采样0buffer写类型    */
            JPEG_DRV_XRGBSAMPLE1_WRITE_MEM_MMU_TYPE = 0X40,   /**< RGB输出需要的上采样1buffer写类型    */
            JPEG_DRV_XRGBOUTPUT_MEM_MMU_TYPE        = 0X80,   /**< 7bitRGB输出内存为MMU类型            */
            JPEG_DRV_MEMTYPE_BUTT
        }JPEG_MEMTYPE_E;

        /** @} */  /** <!-- ==== enum Definition end ==== */

        /*************************** Structure Definition ****************************/
        typedef struct tagJPEGHUFFTBL
        {
              HI_U8   bits[17];
              HI_U8   huffval[256];
              HI_BOOL bHasHuffTbl;
        } JPEG_HUFF_TBL;

        typedef struct tagJPEGQUANTTBL
        {
            HI_BOOL bHasQuantTbl;
            HI_U16  quantval[MAX_DCT_SIZE];
        } JPEG_QUANT_TBL;

        typedef struct tagCOMPONENT_INFO_S
        {
             HI_U8   u8HorSampleFac;
             HI_U8   u8VerSampleFac;
             HI_S32  s32ComponentId;
             HI_S32  s32ComponentIndex;
             HI_S32  s32QuantTblNo;
             HI_S32  s32DcTblNo;
             HI_S32  s32AcTblNo;
        }COMPONENT_INFO_S;

        typedef struct tagIMAG_INFO_S
        {

             HI_S32   s32InWidth;
             HI_S32   s32InHeight;
             HI_S32   s32NumComponent;
             HI_S32   s32ComInScan;
             HI_S32   s32Ss;
             HI_S32   s32Se;
             HI_S32   s32Ah;
             HI_S32   s32Al;
             HI_S32   s32LuDcLen[6];
             HI_S32   s32RestartInterval;
             HI_U32   InputDataSize[2];
             HI_U32   u32CurPos[2];
             HI_U64   u64LuPixValue;
             HI_U8    u8Fac[PIXEL_COMPONENT_NUM][2];
             HI_BOOL  bSofMark;
             HI_CHAR* InputDataVirBuf[2];
             HI_U32   u32StreamPhy;
             HI_CHAR* pStreamVir;
             COMPONENT_INFO_S stComponentInfo[PIXEL_COMPONENT_NUM];
             JPEG_HUFF_TBL    DcHuffTbl[MAX_NUM_HUFF_TBLS];
             JPEG_HUFF_TBL    AcHuffTbl[MAX_NUM_HUFF_TBLS];
             JPEG_QUANT_TBL   QuantTbl[MAX_NUM_QUANT_TBLS];
             IMG_FMT_E eImgFmt;
        }IMAG_INFO_S;

        /** \addtogroup   JPEG */
        /** @{ */    /** <!--[JPEG]*/


        /** @} */  /** <!-- ==== Structure Definition End ==== */

        /********************** Global Variable declaration **************************/

        /******************************* API declaration *****************************/

        HI_S32 DRV_JPEG_Parse(HI_DRV_JPEG_INMSG_S *pstInMsg, IMAG_INFO_S *pstImgInfo);

        HI_S32 DRV_JPEG_GetSofn(HI_DRV_JPEG_OUTMSG_S *pstOutMsg,IMAG_INFO_S *pstImgInfo);

        HI_VOID DRV_JPEG_GetImgInfo(HI_DRV_JPEG_INMSG_S *pstInMsg,IMAG_INFO_S *pstImgInfo);

        /****************************************************************************/



#ifdef __cplusplus

#if __cplusplus

    }
#endif
#endif /* __cplusplus */

#endif

#endif /* __JPEG_DRV_PARSE_H__*/
