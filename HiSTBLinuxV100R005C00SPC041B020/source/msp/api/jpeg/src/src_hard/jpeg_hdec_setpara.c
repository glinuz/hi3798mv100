/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_setpara.c
Version            : Initial Draft
Author            : y00181162
Created            : 2014/06/20
Description        : set parameter that hard decode need
                  CNcomment: 配置硬件解码需要的参数信息 CNend\n
Function List     :


History           :
Date                Author                Modification
2014/06/20            y00181162            Created file
******************************************************************************/
/*********************************add include here******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hi_jpeg_config.h"
#include "jpeg_hdec_api.h"
#include "jpeg_hdec_mem.h"
#include "jpeg_hdec_rwreg.h"
#include "hi_jpeg_reg.h"
#include  "hi_gfx_sys.h"

#ifdef CONFIG_JPEG_FPGA_TEST_SET_DIFFERENT_OUTSTANDING_VALUE
#include "hi_jpeg_hdec_test.h"
#endif
/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/

/******************************* API forward declarations *******************/

/******************************* API realization *****************************/

/*****************************************************************************
 * func          : JPEG_HDEC_ImagFmt2HardFmt
 * description     : convert the input jpeg format to hard register need format
                   CNcomment:  将输入图片的像素格式转换成硬件需要的像素格式 CNend\n
 * param[in]     : cinfo       CNcomment: 解码对象     CNend\n
 * retval         : NA
 * others:         : NA
 *****************************************************************************/
static HI_VOID JPEG_HDEC_ImagFmt2HardFmt(const HI_JPEG_FMT_E enJpegFmt,HI_U32 *pu32Fmt)
{

        switch(enJpegFmt){
            case JPEG_FMT_YUV400:
                   *pu32Fmt = 0;
               break;
            case JPEG_FMT_YUV420:
                   *pu32Fmt = 3;
                   break;
            case JPEG_FMT_YUV422_21:
                   *pu32Fmt = 4;
                   break;
            case JPEG_FMT_YUV422_12:
                   *pu32Fmt = 5;
               break;
            case JPEG_FMT_YUV444:
                   *pu32Fmt = 6;
               break;
            default:
                   *pu32Fmt = 7;
                   break;
        }

}


/*****************************************************************************
* func            : JPEG_HDEC_SetDqt
* description    : set quant table
                  CNcomment: 设置量化表                   CNend\n
* param[in]     : cinfo         CNcomment: 解码对象    CNend\n
* others:        : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SetDqt(const struct jpeg_decompress_struct *cinfo)
{
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
     JPEG_HDEC_CpyData2Reg(pJpegHandle->pJpegRegVirAddr, (HI_VOID *)pJpegHandle->s32QuantTab, JPGD_REG_QUANT, sizeof(pJpegHandle->s32QuantTab));
}


/*****************************************************************************
* func            : JPEG_HDEC_SetDhtDc
* description    : set huffman table
                  CNcomment: 设置哈夫曼表直流分量      CNend\n
* param[in]     : cinfo         CNcomment: 解码对象    CNend\n
* others:        : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SetDhtDc(const struct jpeg_decompress_struct *cinfo)
{
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
     /**
      **DC TABLE
      **/
     #if defined(CONFIG_JPEG_MPG_DEC_ENABLE) && defined(CONFIG_JPEG_USE_CALC_DEFAULT_VALUE)
        HI_U32 u32TmpHdcTab[12] = {0x200200,0x201302,0x202303,0x306304,0x40e305,0x51e306,
                                   0x63e40e,0x77e51e,0x8fe63e,0x9fe77e,0xafe8fe,0xbfe9fe};
        if ((NULL == cinfo->dc_huff_tbl_ptrs[0]) || (NULL != cinfo->dc_huff_tbl_ptrs[2]))
        {/** 这里已经计算好了 **/
            JPEG_HDEC_CpyData2Reg(pJpegHandle->pJpegRegVirAddr,u32TmpHdcTab,JPGD_REG_HDCTABLE,48);
            return;
        }
     #endif
     JPEG_HDEC_CpyData2Reg(pJpegHandle->pJpegRegVirAddr,pJpegHandle->u32HuffDcTab,JPGD_REG_HDCTABLE,48);
}

/*****************************************************************************
* func            : JPEG_HDEC_SetDhtAc
* description    : set huffman table
                  CNcomment: 设置哈夫曼表交流分量      CNend\n
* param[in]     : cinfo         CNcomment: 解码对象    CNend\n
* others:        : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SetDhtAc(const struct jpeg_decompress_struct *cinfo)
{
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
     #if defined(CONFIG_JPEG_MPG_DEC_ENABLE) && defined(CONFIG_JPEG_USE_CALC_DEFAULT_VALUE)
        HI_U32 u32TmpHacMinTab[8]     = {0x0,0xa040a04,0x38183a1a,0xf678f878,0xf6f4f6f6,0xf4f6f4f6,0xe0f0e0f0,0x88c282c0};
        HI_U32 u32TmpHacBaseTab[8]      = {0x0,0xf9fef9fe,0xd1edcfec,0x1a951793,0x2520211c,0x302a2c26,0x48000000,0xa367a364};
        HI_U32 u32TmpHacSymblTab[256] =
              {
                0x1,      0x102,    0x203,      0x300,   0x1104,      0x411,   0x505,    0x2112,
                0x3121,   0x631,   0x1241,      0x4106,  0x5113,      0x751,   0x6161,      0x7107,
                0x1322,   0x2271,    0x3214,   0x8132,    0x881,      0x1491,  0x42a1,      0x9108,
                0xa123,   0xb142,    0xc1b1,   0x9c1,   0x2315,      0x3352,  0x52d1,      0xf0f0,
                0x1524,   0x6233,    0x7262,   0xd172,    0xa82,      0x1609,  0x240a,      0x3416,
                0xe117,   0x2518,    0xf119,   0x171a,    0x1825,   0x1926,    0x1a27,   0x2628,
                0x2729,   0x282a,    0x2934,   0x2a35,    0x3536,   0x3637,    0x3738,   0x3839,
                0x393a,   0x3a43,    0x4344,   0x4445,    0x4546,   0x4647,    0x4748,   0x4849,
                0x494a,   0x4a53,    0x5354,   0x5455,    0x5556,   0x5657,    0x5758,   0x5859,
                0x595a,   0x5a63,    0x6364,   0x6465,    0x6566,   0x6667,    0x6768,   0x6869,
                0x696a,   0x6a73,    0x7374,   0x7475,    0x7576,   0x7677,    0x7778,   0x7879,
                0x797a,   0x7a83,    0x8284,   0x8385,    0x8486,   0x8587,    0x8688,   0x8789,
                0x888a,   0x8992,    0x8a93,   0x9294,    0x9395,   0x9496,    0x9597,   0x9698,
                0x9799,   0x989a,    0x99a2,   0x9aa3,    0xa2a4,   0xa3a5,    0xa4a6,   0xa5a7,
                0xa6a8,   0xa7a9,    0xa8aa,   0xa9b2,    0xaab3,   0xb2b4,    0xb3b5,   0xb4b6,
                0xb5b7,   0xb6b8,    0xb7b9,   0xb8ba,    0xb9c2,   0xbac3,    0xc2c4,   0xc3c5,
                0xc4c6,   0xc5c7,    0xc6c8,   0xc7c9,    0xc8ca,   0xc9d2,    0xcad3,   0xd2d4,
                0xd3d5,   0xd4d6,    0xd5d7,   0xd6d8,    0xd7d9,   0xd8da,    0xd9e1,   0xdae2,
                0xe2e3,   0xe3e4,    0xe4e5,   0xe5e6,    0xe6e7,   0xe7e8,    0xe8e9,   0xe9ea,
                0xeaf1,   0xf2f2,    0xf3f3,   0xf4f4,    0xf5f5,   0xf6f6,    0xf7f7,   0xf8f8,
                0xf9f9,   0xfafa,    0x0,      0x0,      0x0,       0x0,     0x0,      0x0,
                0x0,   0x0,   0x0,     0x0,    0x0,   0x0,   0x0,     0x0,
                0x0,   0x0,   0x0,     0x0,    0x0,   0x0,   0x0,     0x0,
                0x0,   0x0,   0x0,     0x0,    0x0,   0x0,   0x0,     0x0,
                0x0,   0x0,   0x0,     0x0,    0x0,   0x0,   0x0,     0x0,
                0x0,   0x0,   0x0,     0x0,    0x0,   0x0,   0x0,     0x0,
                0x0,   0x0,   0x0,     0x0,    0x0,   0x0,   0x0,     0x0,
                0x0,   0x0,   0x0,     0x0,    0x0,   0x0,   0x0,     0x0,
                0x0,   0x0,   0x0,     0x0,    0x0,   0x0,   0x0,     0x0,
                0x0,   0x0,   0x0,     0x0,    0x0,   0x0,   0x0,     0x0,
                0x0,   0x0,   0x0,     0x0,    0x0,   0x0,   0x0,     0x0,
                0x0,   0x0,   0x0,     0x0,    0x0,   0x0,   0x0,     0x0
              };
        if ((NULL == cinfo->ac_huff_tbl_ptrs[0]) || (NULL != cinfo->ac_huff_tbl_ptrs[2]))
        {
            JPEG_HDEC_CpyData2Reg(pJpegHandle->pJpegRegVirAddr, u32TmpHacMinTab,   JPGD_REG_HACMINTABLE,  32);
            JPEG_HDEC_CpyData2Reg(pJpegHandle->pJpegRegVirAddr, u32TmpHacBaseTab,  JPGD_REG_HACBASETABLE, 32);
            JPEG_HDEC_CpyData2Reg(pJpegHandle->pJpegRegVirAddr, u32TmpHacSymblTab, JPGD_REG_HACSYMTABLE,  0x2c0);
            return;
        }
     #endif

     /** Write hac_min/base/symbol_table into  HW reisters */
     /** CNcomment: 将hac_min/base值配给硬件寄存器 */
     JPEG_HDEC_CpyData2Reg(pJpegHandle->pJpegRegVirAddr, pJpegHandle->u32HufAcMinTab,  JPGD_REG_HACMINTABLE,  32);
     JPEG_HDEC_CpyData2Reg(pJpegHandle->pJpegRegVirAddr, pJpegHandle->u32HufAcBaseTab, JPGD_REG_HACBASETABLE, 32);
     /** 0x2c0 = ac_max_sum_syms * 4 */
     /** CNcomment: 值0x2c0 = ac_max_sum_syms * 4计算得来 */
     JPEG_HDEC_CpyData2Reg(pJpegHandle->pJpegRegVirAddr, pJpegHandle->u32HufAcSymbolTab,JPGD_REG_HACSYMTABLE,  0x2c0);

}


/*****************************************************************************
* func            : JPEG_HDEC_SetDht
* description    : set huffman table
                  CNcomment: 设置哈夫曼表              CNend\n
* param[in]     : cinfo         CNcomment: 解码对象    CNend\n
* others:        : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SetDht(const struct jpeg_decompress_struct *cinfo)
{
    JPEG_HDEC_SetDhtDc(cinfo);
    JPEG_HDEC_SetDhtAc(cinfo);
}

/*****************************************************************************
* func            : JPEG_HDEC_SetSof
* description    : set the sof message
                  CNcomment: 设置sof基本信息           CNend\n
* param[in]     : cinfo         CNcomment: 解码对象    CNend\n
* retval        : HI_SUCCESS    CNcomment: 成功        CNend\n
* retval        : HI_FAILURE    CNcomment: 失败        CNend\n
* others:        : NA
*****************************************************************************/
static HI_S32 JPEG_HDEC_SetSof(const struct jpeg_decompress_struct *cinfo)
{


        HI_S32 s32DefaultScale    = 0;
        HI_U32 u32Stride        = 0;
        HI_U32 u32HardFmt      = 0;

#ifdef CONFIG_JPEG_FPGA_TEST_SET_DIFFERENT_OUTSTANDING_VALUE
        HI_S32 s32OutstandingValue = 0;
        HI_BOOL bOutStandingFlag   = HI_FALSE;
#endif
        JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);


        /**
         ** set the picture type
         ** CNcomment: 设置图片类型 CNend\n
         **/
        JPEG_HDEC_ImagFmt2HardFmt(pJpegHandle->enImageFmt,&u32HardFmt);
        if(u32HardFmt >= 7){
            return HI_FAILURE;
        }
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr, JPGD_REG_PICTYPE, (HI_S32)u32HardFmt);

        /**
         ** set the dec scale
         ** CNcomment: 设置解码缩放比例 CNend\n
         **/
        #ifdef CONFIG_JPEG_HARDDEC2ARGB
        if(HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB){
            /**
             ** do not write data to ddr and no scale
             ** CNcomment: 不往DDR写YUV数据并且不缩放并且不缩放，默认值为0x34 CNend\n
             **/
           /*Enable mem clock: | 0x8*/
           JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_SCALE, (0x30 | 0x8));
        }
        else
        #endif
        {
            /**
             ** set the scale register
             ** CNcomment: 设置要解码的缩放比例，看该寄存器的结构来配置
             **            先获取硬件默认的缩放比例 Scale.struBits.Outstanding = 7 CNend\n
             **/
            s32DefaultScale = (HI_S32)0x34;

            /** 时钟门控打开-带宽 **/
            JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_SCALE, \
                                  (HI_S32)(((HI_U32)s32DefaultScale & 0xfffffffc) | pJpegHandle->u32ScalRation | 0x8));
        }

#ifdef CONFIG_JPEG_FPGA_TEST_SET_DIFFERENT_OUTSTANDING_VALUE
        HI_JPEG_GetOutstandingValue(&s32OutstandingValue,&bOutStandingFlag);
        if(HI_TRUE == bOutStandingFlag){
            s32DefaultScale = JPEG_HDEC_ReadReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_SCALE);
            JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,   \
                                 JPGD_REG_SCALE,               \
                                (HI_S32)(((s32DefaultScale) & s32OutstandingValue) | 0x8));
            #if 0
            /** himd.l 0xf8c40000 看是否已经写入 **/
            JPEG_TRACE("please enter getchar to next\n");
            getchar();
            #endif
            if( (0x10 != (s32DefaultScale & s32OutstandingValue & 0xf0)) && (0x20 != (s32DefaultScale & s32OutstandingValue & 0xf0))){
                JPEG_TRACE("=====================================================================\n");
                JPEG_TRACE("set the outstanding value failure,please enter getchar to exit\n");
                JPEG_TRACE("s32DefaultScale = 0x%x\n",s32DefaultScale);
                JPEG_TRACE("s32DefaultScale & s32OutstandingValue = 0x%x\n",s32DefaultScale & s32OutstandingValue);
                JPEG_TRACE("=====================================================================\n");
                getchar();
            }
        }
#endif

        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr, JPGD_REG_PICSIZE, (HI_S32)pJpegHandle->stJpegSofInfo.u32InWandH);

        /**
         ** set lu and ch stride
         ** CNcomment: 配置亮度和色度的行间距值 CNend\n
         **/
        u32Stride = (pJpegHandle->stJpegSofInfo.u32CbCrStride << 16 ) | pJpegHandle->stJpegSofInfo.u32YStride;

        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr, JPGD_REG_STRIDE,(HI_S32)u32Stride);

        return HI_SUCCESS;


}
/*****************************************************************************
* func            : JPEG_HDEC_SetSampleFactor
* description    : set the sample factor---hifone has revise this bug
                  CNcomment: 设置采样因子信息          CNend\n
* param[in]     : cinfo         CNcomment: 解码对象    CNend\n
* retval        : HI_SUCCESS    CNcomment: 成功        CNend\n
* retval        : HI_FAILURE    CNcomment: 失败        CNend\n
* others:        : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SetSampleFactor(const struct jpeg_decompress_struct *cinfo)
{
    JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
    HI_U8 u8YFac = 0,u8UFac = 0,u8VFac = 0;
    u8YFac = (HI_U8)(((pJpegHandle->u8Fac[0][0] << 4) | pJpegHandle->u8Fac[0][1]) & 0xff);
    u8UFac = (HI_U8)(((pJpegHandle->u8Fac[1][0] << 4) | pJpegHandle->u8Fac[1][1]) & 0xff);
    u8VFac = (HI_U8)(((pJpegHandle->u8Fac[2][0] << 4) | pJpegHandle->u8Fac[2][1]) & 0xff);
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr, JPGD_REG_SAMPLINGFACTOR,(HI_S32)((u8YFac << 16) |(u8UFac << 8) | u8VFac));
}

/*****************************************************************************
* func            : JPEG_HDEC_SetDri
* description    : set dri value
                  CNcomment: 设置dri得值          CNend\n
* param[in]     : cinfo         CNcomment: 解码对象    CNend\n
* others:        : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SetDri(const struct jpeg_decompress_struct *cinfo)
{
    HI_S32 s32DriValue = 0;
    JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
    s32DriValue = (HI_S32)cinfo->restart_interval;
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr, JPGD_REG_DRI,s32DriValue);
}


/*****************************************************************************
* func            : JPEG_HDEC_SetMemtype
* description    : set mem type
                  CNcomment: 设置内存类型           CNend\n
* param[in]     : cinfo      CNcomment: 解码对象    CNend\n
* others:        : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SetMemtype(const struct jpeg_decompress_struct *cinfo)
{
    /**=========================================================================
     ** [0] :  1 MMZ   0 MMU   读码流内存类型
     ** [1] :  1 MMZ   0 MMU   Y分量输出内存类型
     ** [2] :  1 MMZ   0 MMU   UV分量输出内存类型
     ** ...
     **======================================================================**/
    JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);
    HI_U32 u32Mask =   JPEG_STREAM_MEM_MMU_TYPE            \
                     | JPEG_YOUTPUT_MEM_MMU_TYPE           \
                     | JPEG_UVOUTPUT_MEM_MMU_TYPE          \
                     | JPEG_XRGBSAMPLE0_READ_MEM_MMU_TYPE  \
                     | JPEG_XRGBSAMPLE1_READ_MEM_MMU_TYPE  \
                     | JPEG_XRGBSAMPLE0_WRITE_MEM_MMU_TYPE \
                     | JPEG_XRGBSAMPLE1_WRITE_MEM_MMU_TYPE \
                     | JPEG_XRGBOUTPUT_MEM_MMU_TYPE;

#ifdef CONFIG_GFX_MMU_SUPPORT
    u32Mask &= ~JPEG_STREAM_MEM_MMU_TYPE;
    u32Mask &= ~JPEG_YOUTPUT_MEM_MMU_TYPE;
    u32Mask &= ~JPEG_UVOUTPUT_MEM_MMU_TYPE;
    u32Mask &= ~JPEG_XRGBSAMPLE0_READ_MEM_MMU_TYPE;
    u32Mask &= ~JPEG_XRGBSAMPLE1_READ_MEM_MMU_TYPE;
    u32Mask &= ~JPEG_XRGBSAMPLE0_WRITE_MEM_MMU_TYPE;
    u32Mask &= ~JPEG_XRGBSAMPLE1_WRITE_MEM_MMU_TYPE;
    u32Mask &= ~JPEG_XRGBOUTPUT_MEM_MMU_TYPE;
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_MMU_BYPASS,u32Mask);
    //JPEG_TRACE("===%s %s %d u32Mask = 0x%x\n",__FILE__,__FUNCTION__,__LINE__,u32Mask);
#else
    /** 非mmu，都bypass **/
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_MMU_BYPASS,u32Mask);
#endif
    return;
}


/*****************************************************************************
* func            : JPEG_HDEC_SetStreamBuf
* description    : set stream buffer message
                  CNcomment: 设置码流buffer寄存器        CNend\n
* param[in]     : cinfo         CNcomment: 解码对象    CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
HI_VOID JPEG_HDEC_SetStreamBuf(const struct jpeg_decompress_struct *cinfo)
{

        HI_U32 u32Align       = 64;  /**< the stream buffer should 64 bytes align *//**<CNcomment:码流buffer需要64字节对齐 */
        HI_U32 u32Offset      = 100; /**< you can set >=0,insure include the save stream buffer *//**<CNcomment:大于等于0的值，保证能够包含存储码流buffer的区域 */
        HI_U64 u64StreamSize  = 0;
        HI_U32 u32StartStreamPhy  = 0;
        HI_U32 u32EndStreamPhy    = 0;
        JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

        /**
        ** if use user buffer, this buffer can not sure is 64 bytes align
        ** CNcomment: 如果是使用用户的连续码流buffer，并不一定是64字节对齐
        **            硬件buffer只有有两个值即可，真正使用的存储码流的buffer，
        **            必须在这两个值之间 CNend\n
        **/
        if(HI_TRUE == pJpegHandle->stHDecDataBuf.bUserPhyMem && HI_FALSE == pJpegHandle->stHDecDataBuf.bNeedStreamReturn){
            u64StreamSize = pJpegHandle->stHDecDataBuf.u64DataSize + u32Offset;
            /**
            ** this can insure the start buffer is before the stream address
            ** after align.
            ** CNcomment: 这个能确保对齐之后在码流地址之前 CNend\n
            **/
            u32StartStreamPhy = pJpegHandle->stHDecDataBuf.u32PhyBuf - u32Align;
            u32StartStreamPhy = (u32StartStreamPhy + u32Align - 1) & (~(u32Align - 1));
            u32StartStreamPhy = u32StartStreamPhy - u32Offset;
            u32EndStreamPhy   = (pJpegHandle->stHDecDataBuf.u32PhyBuf + u64StreamSize);
        }else if(HI_TRUE == pJpegHandle->stHDecDataBuf.bNeedStreamReturn){/** 码流回绕 **/
            u32StartStreamPhy = pJpegHandle->stHDecDataBuf.u32StartBufPhy;
            u32EndStreamPhy   = u32StartStreamPhy + pJpegHandle->stHDecDataBuf.s32BufLen;
        }else{
            u64StreamSize   = JPGD_HARD_BUFFER;
            u32StartStreamPhy = pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf;
            u32EndStreamPhy   = pJpegHandle->stHDecDataBuf.u32SaveStreamPhyBuf + u64StreamSize;
        }


        /**
         ** the end address [0:d] invalid. just ok
         ** CNcomment: 结束地址回读的时候[0:5]无效，为零，所以会看到和实际不一致，正常的 CNend\n
         **/
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_STADDR, u32StartStreamPhy);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ENDADDR,u32EndStreamPhy);

}


/*****************************************************************************
* func            : JPEG_HDEC_SetMidderBuf
* description    : set middle buffer register
                  CNcomment: 设置JPEG硬件解码输出寄存器   CNend\n
* param[in]     : cinfo         CNcomment: 解码对象       CNend\n
* retval        : NA
* others:        : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SetMidderBuf(const struct jpeg_decompress_struct * cinfo)
{

        JPEG_HDEC_HANDLE_S_PTR      pJpegHandle = NULL;
        pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);


        /**
         ** set y address,if jpeg hard decode out to argb,is also argb address
         ** CNcomment: 设置亮度输出地址，要是解码输出ARGB，则也是ARGB输出地址 CNend\n
         **/
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,     \
                             JPGD_REG_YSTADDR,                  \
                             pJpegHandle->stMiddleSurface.u32MiddlePhy[0]);

#ifdef CONFIG_JPEG_HARDDEC2ARGB
        if(HI_FALSE == pJpegHandle->bDecOutColorSpaecXRGB)
#endif
        {
            JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,       \
                                  JPGD_REG_UVSTADDR,                \
                                  pJpegHandle->stMiddleSurface.u32MiddlePhy[1]);

        }

}


#ifdef CONFIG_JPEG_HARDDEC2ARGB

/*****************************************************************************
* func           : JPEG_HDEC_SET_REGISTER_FliterEnable
* description  : set register for fliter enable
                 CNcomment: 配置滤波是否使能寄存器         CNend\n
* param[in]       : cinfo           CNcomment: 解码对象           CNend\n
* retval        : NA
* others:       : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SET_REGISTER_FliterEnable(const struct jpeg_decompress_struct *cinfo)
{
     HI_U32 HorFliter   = 0x0;
     HI_U32 VerFliter   = 0x0;
     HI_U32 OutPut      = 0x0;
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     if (HI_TRUE == pJpegHandle->stDecCoef.bEnHorMedian)
     {
        HorFliter = 0x8; /** 1000 **/
     }

     if (HI_TRUE == pJpegHandle->stDecCoef.bEnVerMedian)
     {
        VerFliter = 0x10; /** 10000 **/
     }

     OutPut = (HI_U32)JPEG_HDEC_ReadReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_OUTTYPE);
     OutPut = OutPut | HorFliter | VerFliter;
     JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_OUTTYPE,(HI_S32)OutPut);

     return;
}

/*****************************************************************************
* func           : JPEG_HDEC_SET_REGISTER_XRGBFmt
* description  : set xrgb fmt register
                 CNcomment: 配置XRGB寄存器          CNend\n
* param[in]       : cinfo           CNcomment: 解码对象    CNend\n
* retval        : NA
* others:       : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SET_REGISTER_XRGBFmt(const struct jpeg_decompress_struct *cinfo)
{
    HI_U32 OutType = 0x0;
    HI_U32 OutPut  = 0x0;
    JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

    OutPut = (HI_U32)JPEG_HDEC_ReadReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_OUTTYPE);

    #ifdef CONFIG_GFX_LIB_TURBO
        if (JCS_EXT_RGBA == cinfo->out_color_space)
        {
            OutType  =  0x100;
        }
        else if (JCS_EXT_BGRA == cinfo->out_color_space)
        {
            OutType  =  0x000;
        }
        else if (JCS_RGB565 == cinfo->out_color_space)
        {
            OutType  =  0x400;
        }
        else if ((JCS_RGB == cinfo->out_color_space) || (JCS_EXT_RGB == cinfo->out_color_space))
        {
            OutType  =  0x700;
        }
        else
        {
            OutType  =  0x600;
        }
    #else

        if ((JCS_ARGB_8888 == cinfo->out_color_space) || (JCS_RGBA_8888 == cinfo->out_color_space))
        {
            OutType  =  0x100;
        }
        else if (JCS_ABGR_8888 == cinfo->out_color_space)
        {
            OutType  =  0x000;
        }
        else if (JCS_ARGB_1555 == cinfo->out_color_space)
        {
            OutType  =  0x300;
        }
        else if (JCS_ABGR_1555 == cinfo->out_color_space)
        {
            OutType  =  0x200;
        }
        else if (JCS_RGB_565 == cinfo->out_color_space)
        {
            OutType  =  0x400;
        }
        else if (JCS_BGR_565 == cinfo->out_color_space)
        {
            OutType  =  0x500;
        }
        else if (JCS_RGB == cinfo->out_color_space)
        {
            OutType  =  0x700;
        }
        else
        {
            OutType  =  0x600;
        }
     #endif

        OutPut = OutPut | OutType;

        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_OUTTYPE,(HI_S32)OutPut);

        return;

}

/*****************************************************************************
* func           : JPEG_HDEC_SetDecARGBPara
* description  : set the argb parameter when jpeg hard decode output argb
                 CNcomment: 当解码输出ARGB时候需要设置的一些参数     CNend\n
* param[in]       : cinfo           CNcomment: 解码对象                     CNend\n
* retval        : NA
* others:       : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SetDecARGBPara(const struct jpeg_decompress_struct *cinfo)
{


        HI_S32 s32CropX          = 0;
        HI_S32 s32CropY          = 0;
        HI_S32 s32CropStargPos  = 0;
        HI_S32 s32CropEndX       = 0;
        HI_S32 s32CropEndY       = 0;
        HI_S32 s32CropEndPos     = 0;

        JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);


        /**
        ** calc crop or no crop position and size
        ** CNcomment: 计算裁剪或非裁剪的位置和大小,原图裁剪和真裁剪 CNend\n
        **/
        s32CropX    = pJpegHandle->stOutDesc.stCropRect.x;
        s32CropY    = pJpegHandle->stOutDesc.stCropRect.y;
        s32CropEndX = (s32CropX + pJpegHandle->stOutDesc.stCropRect.w - 1);
        s32CropEndY = (s32CropY + pJpegHandle->stOutDesc.stCropRect.h - 1);

        s32CropStargPos = (HI_S32)((HI_U32)(s32CropX) | ((HI_U32)s32CropY << 16));
        s32CropEndPos   = (HI_S32)(((HI_U32)s32CropEndX) | ((HI_U32)s32CropEndY << 16));

        if(HI_TRUE == pJpegHandle->stOutDesc.stOutSurface.bUserPhyMem){
            JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ARGBOUTSTRIDE,pJpegHandle->stOutDesc.stOutSurface.u32OutStride[0]);
        }else{
            JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ARGBOUTSTRIDE,pJpegHandle->stJpegSofInfo.u32DisplayStride);
        }
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_OUTSTARTPOS,s32CropStargPos);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_OUTENDPOS,s32CropEndPos);


        /**
        ** set alpha value,default is zero
        ** CNcomment: 设置alpha值，寄存器默认值为0 CNend\n
        **/
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_ALPHA,(HI_S32)pJpegHandle->u32Alpha);

        /**
        ** set the min buffer
        ** CNcomment: 设置ARGB的行buffer地址 CNend\n
        **/
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_MINADDR,  pJpegHandle->u32MinPhyBuf);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_MINADDR1, pJpegHandle->u32MinPhyBuf  + pJpegHandle->stJpegSofInfo.u32MINSize);


        JPEG_HDEC_SET_REGISTER_FliterEnable(cinfo);
        JPEG_HDEC_SET_REGISTER_XRGBFmt(cinfo);
        return;

}

/*****************************************************************************
* func           : JPEG_HDEC_SetHorSampleCoef
* description  : set the hor sample coef
                 CNcomment:  设置水平采样滤波系数    CNend\n
* param[in]        : *pJpegHandle   CNcomment: 解码器句柄 CNend\n
* retval       : NA
* others:       : NA
*****************************************************************************/
static HI_VOID  JPEG_HDEC_SetHorSampleCoef(const JPEG_HDEC_HANDLE_S *pJpegHandle)
{

        HI_S32 s324C = 0;
        HI_S32 s3250 = 0;
        HI_S32 s3254 = 0;
        HI_S32 s3258 = 0;
        //HI_S32 s325C = 0;
        //HI_S32 s3260 = 0;
        //HI_S32 s3264 = 0;
        //HI_S32 s3268 = 0;

        HI_S32 s326C = 0;
        HI_S32 s3270 = 0;
        HI_S32 s3274 = 0;
        HI_S32 s3278 = 0;
        //HI_S32 s327C = 0;
        //HI_S32 s3280 = 0;
        //HI_S32 s3284 = 0;
        //HI_S32 s3288 = 0;

        s324C = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[0][0] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[0][1] & 0x3fff) << 16));
        s3250 = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[0][2] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[0][3] & 0x3fff) << 16));
        s3254 = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[0][4] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[0][5] & 0x3fff) << 16));
        s3258 = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[0][6] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[0][7] & 0x3fff) << 16));
        //s325C = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[1][0] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[1][1] & 0x3fff) << 16));
        //s3260 = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[1][2] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[1][3] & 0x3fff) << 16));
        //s3264 = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[1][4] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[1][5] & 0x3fff) << 16));
        //s3268 = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[1][6] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[1][7] & 0x3fff) << 16));

        s326C = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[2][0] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[2][1] & 0x3fff) << 16));
        s3270 = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[2][2] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[2][3] & 0x3fff) << 16));
        s3274 = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[2][4] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[2][5] & 0x3fff) << 16));
        s3278 = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[2][6] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[2][7] & 0x3fff) << 16));
        //s327C = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[3][0] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[3][1] & 0x3fff) << 16));
        //s3280 = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[3][2] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[3][3] & 0x3fff) << 16));
        //s3284 = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[3][4] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[3][5] & 0x3fff) << 16));
        //s3288 = (HI_S32)((pJpegHandle->stDecCoef.s16HorCoef[3][6] & 0x3fff) | ((pJpegHandle->stDecCoef.s16HorCoef[3][7] & 0x3fff) << 16));


        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_HORCOEF00_01,s324C);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_HORCOEF02_03,s3250);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_HORCOEF04_05,s3254);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_HORCOEF06_07,s3258);

        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_HORCOEF20_21,s326C);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_HORCOEF22_23,s3270);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_HORCOEF24_25,s3274);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_HORCOEF26_27,s3278);


}


/*****************************************************************************
* func           : JPEG_HDEC_SetVerSampleCoef
* description  : set the ver sample coef
                 CNcomment:  设置垂直采样滤波系数    CNend\n
* param[in]        : *pJpegHandle   CNcomment: 解码器句柄 CNend\n
* retval       : NA
* others:       : NA
*****************************************************************************/
static HI_VOID  JPEG_HDEC_SetVerSampleCoef(const JPEG_HDEC_HANDLE_S *pJpegHandle)
{

        HI_S32 s328C = 0;
        HI_S32 s3290 = 0;
        HI_S32 s3294 = 0;
        HI_S32 s3298 = 0;
        HI_S32 s329C = 0;
        HI_S32 s32A0 = 0;
        HI_S32 s32A4 = 0;
        HI_S32 s32A8 = 0;

        s328C =  (pJpegHandle->stDecCoef.s16VerCoef[0][0] & 0x3fff) | ((pJpegHandle->stDecCoef.s16VerCoef[0][1] & 0x3fff) << 16);
        s3290 =  (pJpegHandle->stDecCoef.s16VerCoef[0][2] & 0x3fff) | ((pJpegHandle->stDecCoef.s16VerCoef[0][3] & 0x3fff) << 16);
        s3294 =  (pJpegHandle->stDecCoef.s16VerCoef[1][0] & 0x3fff) | ((pJpegHandle->stDecCoef.s16VerCoef[1][1] & 0x3fff) << 16);
        s3298 =  (pJpegHandle->stDecCoef.s16VerCoef[1][2] & 0x3fff) | ((pJpegHandle->stDecCoef.s16VerCoef[1][3] & 0x3fff) << 16);
        s329C =  (pJpegHandle->stDecCoef.s16VerCoef[2][0] & 0x3fff) | ((pJpegHandle->stDecCoef.s16VerCoef[2][1] & 0x3fff) << 16);
        s32A0 =  (pJpegHandle->stDecCoef.s16VerCoef[2][2] & 0x3fff) | ((pJpegHandle->stDecCoef.s16VerCoef[2][3] & 0x3fff) << 16);
        s32A4 =  (pJpegHandle->stDecCoef.s16VerCoef[3][0] & 0x3fff) | ((pJpegHandle->stDecCoef.s16VerCoef[3][1] & 0x3fff) << 16);
        s32A8 =  (pJpegHandle->stDecCoef.s16VerCoef[3][2] & 0x3fff) | ((pJpegHandle->stDecCoef.s16VerCoef[3][3] & 0x3fff) << 16);

        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_VERCOEF00_01,s328C);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_VERCOEF02_03,s3290);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_VERCOEF10_11,s3294);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_VERCOEF12_13,s3298);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_VERCOEF20_21,s329C);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_VERCOEF22_23,s32A0);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_VERCOEF30_31,s32A4);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_VERCOEF32_33,s32A8);


}


/*****************************************************************************
* func           : JPEG_HDEC_SetCSCCoef
* description  : set the CSC coef
                 CNcomment:  设置CSC转换系数         CNend\n
* param[in]        : *pJpegHandle   CNcomment: 解码器句柄 CNend\n
* retval       : NA
* others:       : NA
*****************************************************************************/
static HI_VOID  JPEG_HDEC_SetCSCCoef(const JPEG_HDEC_HANDLE_S *pJpegHandle)
{


        /**
         ** the dc coef is changeless, and the ac coef is change
         ** CNcomment: 直流系数DC保持固定不变，交流系数AC保可变 CNend\n
         **/

        HI_S32 s32B4 = 0;
        HI_S32 s32B8 = 0;
        HI_S32 s32BC = 0;
        HI_S32 s32C0 = 0;
        HI_S32 s32C4 = 0;

        s32B4 = (pJpegHandle->stDecCoef.s16CSCCoef[0][0] & 0x7fff) | ((pJpegHandle->stDecCoef.s16CSCCoef[0][1] & 0x7fff )<<16);
        s32B8 = (pJpegHandle->stDecCoef.s16CSCCoef[0][2] & 0x7fff) | ((pJpegHandle->stDecCoef.s16CSCCoef[1][0] & 0x7fff )<<16);
        s32BC = (pJpegHandle->stDecCoef.s16CSCCoef[1][1] & 0x7fff) | ((pJpegHandle->stDecCoef.s16CSCCoef[1][2] & 0x7fff )<<16);
        s32C0 = (pJpegHandle->stDecCoef.s16CSCCoef[2][0] & 0x7fff) | ((pJpegHandle->stDecCoef.s16CSCCoef[2][1] & 0x7fff )<<16);
        s32C4 = pJpegHandle->stDecCoef.s16CSCCoef[2][2]  & 0x7fff;

        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_CSC00_01,s32B4);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_CSC02_10,s32B8);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_CSC11_12,s32BC);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_CSC20_21,s32C0);
        JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_CSC22,   s32C4);


}

/*****************************************************************************
* func           : JPEG_HDEC_SetCropAndARGBInfo
* description  : set about dec out argb register
                 CNcomment: 设置解码输出为ARGB相关信息     CNend\n
* param[in]       : cinfo           CNcomment: 解码对象         CNend\n
* retval        : NA
* others:       : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SetCropAndARGBInfo(const struct jpeg_decompress_struct *cinfo)
{

        JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

        JPEG_HDEC_SetDecARGBPara(cinfo);

        if(HI_TRUE == pJpegHandle->stDecCoef.bSetHorSampleCoef){
            JPEG_HDEC_SetHorSampleCoef(pJpegHandle);
        }
        if(HI_TRUE == pJpegHandle->stDecCoef.bSetVerSampleCoef){
            JPEG_HDEC_SetVerSampleCoef(pJpegHandle);
        }
        if(HI_TRUE == pJpegHandle->stDecCoef.bSetCSCCoef){
            JPEG_HDEC_SetCSCCoef(pJpegHandle);
        }
}
#endif


/*****************************************************************************
* func           : JPEG_HDEC_SetCropAndARGBInfo
* description  : set all decode output yuv420sp information
                 CNcomment: 设置解码统一输出yuvsp420sp寄存器     CNend\n
* param[in]       : cinfo           CNcomment: 解码对象                 CNend\n
* retval        : NA
* others:       : NA
*****************************************************************************/
#ifdef CONFIG_JPEG_OUTPUT_YUV420SP
static HI_VOID JPEG_HDEC_SetYUV420SPInfo(const struct jpeg_decompress_struct *cinfo)
{

     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_OUTTYPE,0x4);

}
#endif


/*****************************************************************************
* func           : JPEG_HDEC_SetCropAndARGBInfo
* description  : set count lu pixle sum value
                 CNcomment: 设置统计亮度值寄存器      CNend\n
* param[in]       : cinfo           CNcomment: 解码对象     CNend\n
* retval        : NA
* others:       : NA
*****************************************************************************/

#ifdef CONFIG_JPEG_OUTPUT_LUPIXSUM
static HI_VOID JPEG_HDEC_SetLuPixSumInfo(const struct jpeg_decompress_struct *cinfo)
{

    JPEG_HDEC_HANDLE_S_PTR pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_LPIXSUM1,(HI_S32)0x80000000);

}
#endif



#ifdef CONFIG_JPEG_TEST_CHIP_PRESS
/*****************************************************************************
* func           : JPEG_HDEC_SetPress
* description  : set press register
                 CNcomment: 正常使用的不配置反压，测试使用
                             FPGA验证的时候使用,只要性能下降了就说明方压生效 CNend\n
* param[in]    : cinfo           CNcomment: 解码对象     CNend\n
* retval       : NA
* others:       : NA
*****************************************************************************/
static HI_VOID JPEG_HDEC_SetPress(const struct jpeg_decompress_struct *cinfo)
{

    JPEG_HDEC_HANDLE_S_PTR pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

    //JPEG_TRACE("\n==================================================================================================\n");
#if 1
     HI_U32 u32CurUsecTime = 0;
     HI_U32 u32CursecTime  = 0;
     HI_S32 a,b;
     HI_S32 tmp = 0;
     HI_S32 D32_00 = 0;
     HI_S32 D32_01 = 0;
     HI_S32 D32_02 = 0;
     HI_S32 D32_03 = 0;
     HI_S32 D32_04 = 0;
     HI_S32 D32_05 = 0;
     HI_S32 s32ByPass = 0;
     HI_U32 Axi_Press_bypass_flag = 0x80000000;

    /**第32为1为bypass , 0 为反压 **/
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_PRESS_BYPASS,(HI_S32)0x80000000);


    HI_GFX_GetTimeStamp(&u32CursecTime,&u32CurUsecTime);
    tmp = (rand() + u32CurUsecTime) % 4;
    if(tmp > 0)    {
        /** 带反压 **/
         Axi_Press_bypass_flag = 0;
    }

    HI_GFX_GetTimeStamp(&u32CursecTime,&u32CurUsecTime);
    tmp = 2000 + (rand() + u32CurUsecTime) % 30 ;
    D32_00 = 0x10000 | (tmp&0xffff) | Axi_Press_bypass_flag;

    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_PRESS_BYPASS,D32_00);
    //JPEG_TRACE("AXI_PRESS_BYPASS = %d \n",D32_00);

     s32ByPass = D32_00 & 0x80000000;
     //JPEG_TRACE("press by pass is  0x80000000 \n");
     //JPEG_TRACE("s32ByPass = %x \n",s32ByPass);
     if(0x80000000 != s32ByPass)
     {
        //JPEG_TRACE("====== 反压测试打开 \n");
     }

    /** 扣脉冲 **/
    a = rand()%31 + 1;
    b = rand()%31 + 1;
    if (a > b){
        b = a;
    }
    D32_01 = 0;
    D32_01 = (a<<8)|b;
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,0xffC,D32_01);
    //JPEG_TRACE("0xffC = %d \n",D32_01);


    HI_GFX_GetTimeStamp(&u32CursecTime,&u32CurUsecTime);
    tmp = 800 + (rand() + u32CurUsecTime) % 31 ;
    D32_02 = 0x10000 | (tmp & 0xffff);
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_PRESS_FF04, D32_02);
    //JPEG_TRACE("0xff04 = %d \n",D32_02);

    HI_GFX_GetTimeStamp(&u32CursecTime,&u32CurUsecTime);
    tmp = 800 + (rand() + u32CurUsecTime) % 32 ;
    D32_03 = 0x10000 | (tmp & 0xffff);
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_PRESS_FF08, D32_03);
    //JPEG_TRACE("0xff08 = %d \n",D32_03);

    HI_GFX_GetTimeStamp(&u32CursecTime,&u32CurUsecTime);
    tmp = 2000 + (rand() + u32CurUsecTime) % 23 ;
    D32_04 = 0x10000 | (tmp&0xffff);
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_PRESS_FF0C, D32_04);
    //JPEG_TRACE("0xff0c = %d \n",D32_04);

    HI_GFX_GetTimeStamp(&u32CursecTime,&u32CurUsecTime);
    tmp = 2000 + (rand() + u32CurUsecTime) % 44 ;
    D32_05 = 0x10000 | (tmp & 0xffff);
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_PRESS_FF10, D32_05);
    //JPEG_TRACE("0xff10 = %d \n",D32_05);

#else
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_PRESS_BYPASS, 2000);
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_PRESS_FF04, 800);
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_PRESS_FF08, 800);
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_PRESS_FF0C, 2000);
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_PRESS_FF10, 2000);
#endif

    /** 开始反压 **/
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_PRESS_FF20, 0);
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_PRESS_FF20, 1);

    //JPEG_TRACE("==================================================================================================\n");

}
#endif


/*****************************************************************************
* func           : JPEG_HDEC_SetLowDelayInf
* description  : set low delay information
                 CNcomment: 配置低延迟信息 CNend\n
* param[in]    : cinfo     CNcomment: 解码对象     CNend\n
* retval       : NA
* others:       : NA
*****************************************************************************/
#ifdef CONFIG_JPEG_LOW_DELAY_SUPPORT
static HI_S32 JPEG_HDEC_SetLowDelayInf(const struct jpeg_decompress_struct *cinfo)
{
    HI_S32 s32McuLineNum = 0;
    HI_S32 s32Ret        = HI_SUCCESS;
    HI_JPEG_OUTPUTINFO_S stOutPutInfo;
    JPEG_HDEC_HANDLE_S_PTR pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

    if (HI_TRUE != pJpegHandle->bLowDelayEn)
    {
        return HI_SUCCESS;
    }

    if (cinfo->output_height > 720)
    {
        s32McuLineNum = 8;
    }
    else
    {
        s32McuLineNum = 4;
    }

    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_LINE_ADDR,(HI_S32)pJpegHandle->u32PhyLineBuf);
    JPEG_HDEC_WriteReg(pJpegHandle->pJpegRegVirAddr,JPGD_REG_LINE_NUM,s32McuLineNum);

    HI_GFX_Memset(&stOutPutInfo, 0x0, sizeof(HI_JPEG_OUTPUTINFO_S));
    stOutPutInfo.pVirLineBuf     = pJpegHandle->pVirLineBuf;
    stOutPutInfo.u32OutHeight[0] = pJpegHandle->stJpegSofInfo.u32YMcuHeight;
    stOutPutInfo.u32OutHeight[1] = pJpegHandle->stJpegSofInfo.u32CMcuHeight;
    s32Ret = ioctl(pJpegHandle->s32JpegDev, CMD_JPG_LOWDELAY_LINEINFO, &stOutPutInfo);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
#endif

/*****************************************************************************
* func            : JPEG_HDEC_SetPara
* description    : set the parameter that hard decode need
                  CNcomment: 配置硬件解码需要的参数信息        CNend\n
* param[in]     : cinfo         CNcomment: 解码对象    CNend\n
* retval        : HI_SUCCESS    CNcomment: 成功        CNend\n
* retval        : HI_FAILURE    CNcomment: 失败        CNend\n
* others:        : NA
*****************************************************************************/
HI_S32 JPEG_HDEC_SetPara(const struct jpeg_decompress_struct *cinfo)
{


        JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

        HI_S32 s32RetVal = HI_SUCCESS;

        /**
         ** set dqt table register
         ** CNcomment: 配置量化表，21us CNend\n
         **/
        JPEG_HDEC_SetDqt(cinfo);

        /**
         ** set dht table register
         ** CNcomment: 配置哈夫曼表，100us ~ 120us CNend\n
         **/
        JPEG_HDEC_SetDht(cinfo);

        /**
         ** set sof message
         ** CNcomment: 设置SOF信息 CNend\n
         **/
        s32RetVal = JPEG_HDEC_SetSof(cinfo);
        if(HI_SUCCESS != s32RetVal)
        {
           return HI_FAILURE;
        }

        /**
         ** set sample factor message
         ** CNcomment: 设置采样因子信息 CNend\n
         **/
        JPEG_HDEC_SetSampleFactor(cinfo);

        /**
         ** set dri value
         ** CNcomment: 设置dri得值 CNend\n
         **/
        JPEG_HDEC_SetDri(cinfo);

        /**
         ** set mem type
         ** CNcomment: 设置内存类型 CNend\n
         **/
        JPEG_HDEC_SetMemtype(cinfo);

        /**
         ** set stream buffer message
         ** CNcomment: 设置码流buffer寄存器 CNend\n
         **/
        JPEG_HDEC_SetStreamBuf(cinfo);

        /**
         ** set middle buffer register
         ** CNcomment: 设置JPEG硬件解码输出寄存器 CNend\n
         **/
        JPEG_HDEC_SetMidderBuf(cinfo);


#ifdef CONFIG_JPEG_HARDDEC2ARGB
        /**
         ** set about dec out argb register
         ** CNcomment: 设置解码输出为ARGB相关信息 CNend\n
         **/
        if(HI_TRUE == pJpegHandle->bDecOutColorSpaecXRGB)
        {
           JPEG_HDEC_SetCropAndARGBInfo(cinfo);
        }
#endif

#ifdef CONFIG_JPEG_OUTPUT_YUV420SP
        /**
         ** set all decode output yuv420sp information
         ** CNcomment: 设置解码统一输出yuvsp420sp寄存器 CNend\n
         **/
        if(HI_TRUE == pJpegHandle->bOutYUV420SP)
        {
           JPEG_HDEC_SetYUV420SPInfo(cinfo);
        }
#endif

#ifdef CONFIG_JPEG_OUTPUT_LUPIXSUM
        /**
         ** set count lu pixle sum value
         ** CNcomment: 设置统计亮度值寄存器 CNend\n
         **/
        if(HI_TRUE == pJpegHandle->bLuPixSum)
        {
           JPEG_HDEC_SetLuPixSumInfo(cinfo);
        }
#endif

#ifdef CONFIG_JPEG_TEST_CHIP_PRESS
        JPEG_HDEC_SetPress(cinfo);
#endif

#ifdef CONFIG_JPEG_LOW_DELAY_SUPPORT
        s32RetVal = JPEG_HDEC_SetLowDelayInf(cinfo);
        if(HI_SUCCESS != s32RetVal)
        {
           return HI_FAILURE;
        }
#endif

        return HI_SUCCESS;

}
