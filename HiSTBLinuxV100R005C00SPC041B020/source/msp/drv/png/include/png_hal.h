/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name    : png_hal.h
Version        : Initial Draft
Author        : z00141204
Created        : 2010/10/11
Description    : PNG hal interface
Function List     : PngHalInit
          PngHalDeinit
          PngHalReset

History           :
Date                Author                Modification
2010/10/11        z00141204        Created file
******************************************************************************/

#ifndef __PNG_HAL_H__
#define __PNG_HAL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#include "hi_png_type.h"
#include "hi_png_config.h"


/*Base address of PNG register*/
/*CNcomment:PNG寄存器基地址*/
#if defined(CHIP_TYPE_hi3796cv100_a) || defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100_a) || defined(CHIP_TYPE_hi3798cv100)
#define PNG_REG_BASEADDR   0xFF310000
#else
#define PNG_REG_BASEADDR   0xf8c70000
#endif

/*Size of PNG register*/
/*CNcomment:PNG寄存器大小*/
#define PNG_REG_SIZE        0x120


/*Address of PNG reset regioster*/
/*CNcomment:PNG复位寄存器基地址 */
#if defined(CHIP_TYPE_hi3796cv100_a) || defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100_a) || defined(CHIP_TYPE_hi3798cv100)
#define PNG_REG_RESET       0xf8a22084
#else
#define PNG_REG_RESET       0xf8a22084
#endif
/*PNG interrupt num*/
/*CNcomment:PNG中断号*/
#if defined(CHIP_TYPE_hi3796cv100_a) || defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100_a) || defined(CHIP_TYPE_hi3798cv100)
#define PNG_INTNUM          (96 + 32)
#else
#define PNG_INTNUM          (96 + 32)
#endif

/*Interrupt mask*/
/*CNcomment:中断掩码*/
#define PNG_INT_FINISH_MASK  0x1    /*Finish interrupt mask*//*CNcomment:完成中断*/
#define PNG_INT_RESUME_MASK  0x2    /*Resume stream interrupt mask*//*CNcomment:续码流中断*/
#define PNG_INT_ERR_MASK     0x4    /*Decode err interrupt mask*//*CNcomment:错误中断*/

typedef union tag_PNG_SIZE_U
{
    struct
    {
        HI_U32 u32Height:   13;
        HI_U32 u32Reserved1 :   3;
        HI_U32 u32Width :   13;
        HI_U32 u32Reserved2 :   3;
    }stBits;
    HI_U32 u32All;
}PNG_SIZE_U;

typedef union tag_PNG_TYPE_U
{
    struct
    {
        HI_U32 u32BitDepth  :   5;
        HI_U32 u32Reserved1 :   3;
        HI_U32 u32ColorFmt :   3;
        HI_U32 u32Reserved2 :   5;
        HI_U32 u32InterlaceType :   1;
        HI_U32 u32FunSel    :   1;
        HI_U32 u32StreamType    :   1;
        HI_U32 u32Reserved3  :   13;
    }stBits;
    HI_U32 u32All;
}PNG_TYPE_U;

typedef union tag_PNG_TRANSFORM_U
{
    struct
    {
        /*Adjut pixel ordir when bpp(bit per pixel) = 1,2,4*/
        /*CNcomment:调整1，2，4bit的象素顺序*/
        HI_U32 u32PixelOrderEn  :   1;

        /*Transform 1,2,4 bit to 8 bit if image type is gray*/
        /*CNcomment:将1，2，4bit的灰度图转换成8bit */
        HI_U32 u32PackByteEn    :   1;

        /*Transform gray image to true color*/
        /*CNcomment:将灰度图转换位真彩色图 */
        HI_U32 u32Gray2BGREn    :   1;

        /*Add alpha channel*/
        /*CNcomment:增加alpha通道 */
        HI_U32 u32AddAlphaEn    :   1;

        /*Strip alpha channel*/
        /*CNcomment:去除alpha通道*/
        HI_U32 u32StripAlphaEn  :   1;

        /*Transform BGR to RGB*/
        /*CNcomment:BGR转换为RGB */
        HI_U32 u32BGR2RGBEn     :   1;

        /*Swap alpha channel and data channel*/
        /*CNcomment:交换alpha和图象数据的位置*/
        HI_U32 u32SwapAlphaEn   :   1;

        /*Reserved*/
        /*CNcomment:保留*/
        HI_U32 u32Reserved1     :   1;

        /*Trasnform image form 16 bit depth to 8 bit depth*/
        /*CNcomment:16位转换位8位*/
        HI_U32 u32Strip16En     :   1;

        /*Transform image from 8 bit depth to 4 bit depth*/
        /*CNcomment:8位转换位4位*/
        HI_U32 u32Strip4En      :   1;

        /*Reserved*/
        /*CNcomment:保留*/
        HI_U32 u32Reserved2     :   6;

        /*The mode used when trasfomr 1,2,4 bit to 8 bit*/
        /*CNcomment:1,2,4bit转换为8bit时的低位填充方式*/
        HI_U32 u32PackBypeMode  :   1;

        /*The mode used when strip 16 bit to 8 bit*/
        /*CNcomment:16位转8位处理模式*/
        HI_U32 u32Streip16Mode  :   1;

        /*Enable transparent value*/
        /*CNcomment:是否使能透明色信息*/
        HI_U32 u32TransInfoEn   :   1;

        /*Dst color format when trasform to 4 bit*/
        /*CNcomment:转换为4bit时的象素格式*/
        HI_U32 u32Strip4Fmt     :   3;

        HI_U32 u32PreMultiAlphaEn     :   1;
        HI_U32 u32PreMultiRoundMode     :   2;
        /*Reserved*/
        /*CNcomment:保留*/
        HI_U32 u32Reserved3     :   7;
    }stBits;
    HI_U32 u32All;
}PNG_TRANSFORM_U;

typedef union tag_PNG_TRANS_COLOR0_U
{
    struct
    {
        HI_U16 u32TransColorR;
        HI_U16 u32TransColorG;
    }stBits;
    HI_U32 u32All;
}PNG_TRANS_COLOR0_U;

typedef union tag_PNG_TRANS_COLOR1_U
{
    struct
    {
        HI_U32 u32TransColorB    :    16;
        HI_U32 u32AlphaFiller    :    16;
    }stBits;
    HI_U32 u32All;
}PNG_TRANS_COLOR1_U;

typedef union tag_PNG_INT_U
{
    struct
    {
        HI_U32 u32DecFinish :   1;
        HI_U32 u32DecResume :   1;
        HI_U32 u32DecError  :   1;
        HI_U32 u32Reserved  :   13;

        /*Write out of bound when transforming image*/
        /*CNcomment:格式转换时buf写越界*/
        HI_U32 u32FmtBufErr  :   1;

        /*Filter module write out of bound*/
        /*CNcomment:filter模块写越界*/
        HI_U32 u32FltBufErr :   1;

        /*RDC module write out of bound*/
        /*CNcomment:rdc模块写buf越界*/
        HI_U32 u32RdcBufErr :   1;

        /*Filter type invalid*/
        /*CNcomment:滤波类型错误 */
        HI_U32 u32FltTypeErr    :   1;

        /*Copy counte is not equal to 0 when ending*/
        /*CNcomment:cpy数结束时不为0 */
        HI_U32 u32DecCpyErr :   1;

        /*Unkonwn Huffman code*/
        /*CNcomment:未知Huffman码*/
        HI_U32 u32HuffDecErr    :   1;

        /*Length of distance table is larger than 31*/
        /*CNcomment:distance表长大于31*/
        HI_U32 u32ZlibHdisErr   :   1;

        /*Length of literal table is larger than 29*/
        /*CNcomment:literal表长大于29 */
        HI_U32 u32ZlibHlitErr   :   1;

        /*Length of no inflate data invalid*/
        /*CNcomment:非压缩数据len校验错误*/
        HI_U32 u32ZlibLenErr    :   1;

        /*Block type err*/
        /*CNcomment:块类型错误*/
        HI_U32 u32ZlibBtypeErr  :   1;

        /*CMF check err*/
        /*CNcomment:CMF校验错*/
        HI_U32 u32ZlibFcheckErr :   1;

        /*Dictionary ID err*/
        /*CNcomment:字典标识错误*/
        HI_U32 u32ZlibFdictErr  :   1;

        /*Size of window is larger than 32K byte*/
        /*CNcomment:窗口大小大于32k*/
        HI_U32 u32ZlibCinfoErr  :   1;

        /*Inflate method of Zlib stream err*/
        /*CNcomment:Zlib流压缩方法错误*/
        HI_U32 u32ZlibCmErr     :   1;

        /*Adler check err*/
        /*CNcomment:Adler校验错误*/
        HI_U32 u32AdlerErr      :   1;

        /*CRC check err*/
        /*CNcomment:CRC错误*/
        HI_U32 u32CrcErr        :   1;
    }stBits;
    HI_U32 u32All;
}PNG_INT_U;

typedef union tag_PNG_INT_MASK_U
{
    struct
    {
        HI_U32 u32DecFinishMask :   1;
        HI_U32 u32DecResumeMask  :   1;
        HI_U32 u32Reserved      :   14;
        HI_U32 u32FmtBufMask    :   1;
        HI_U32 u32FltButMask    :   1;
        HI_U32 u32RdcBufMask    :   1;
        HI_U32 u32FltTypeMask   :   1;
        HI_U32 u32DecCpyMask    :   1;
        HI_U32 u32HuffDecMask   :   1;
        HI_U32 u32ZlibHdistMask :   1;
        HI_U32 u32ZlibHlitMask  :   1;
        HI_U32 u32ZlibLenMask   :   1;
        HI_U32 u32ZlibBtypeMask :   1;
        HI_U32 u32ZlibFcheckMask    :   1;
        HI_U32 u32ZlibFdictMask :   1;
        HI_U32 u32ZlibCinfoMask :   1;
        HI_U32 u32ZlibCmMask    :   1;
        HI_U32 u32AdlerMask     :   1;
        HI_U32 u32CrcMask       :   1;
    }stBits;
    HI_U32 u32All;
}PNG_INT_MASK_U;

typedef union tag_PNG_ERR_MODE_U
{
    struct
    {
        HI_U32 u32Reserved  :   16; /*Reserved*//*CNcomment:保留*/
        HI_U32 u32FmtBufStop    :   1;
        HI_U32 u32FltBufStop    :   1;
        HI_U32 u32RdcBufStop    :   1;
        HI_U32 u32FltTypeStop   :   1;
        HI_U32 u32DecCpyStop    :   1;
        HI_U32 u32HuffDecStop   :   1;
        HI_U32 u32ZlibHdistStop :   1;
        HI_U32 u32ZlibHlitStop  :   1;
        HI_U32 u32ZlibLenStop   :   1;
        HI_U32 u32ZlibBtypeStop :   1;
        HI_U32 u32ZlibFcheckStop    :   1;
        HI_U32 u32ZlibFdictStop :   1;
        HI_U32 u32ZlibCinfoStop :   1;
        HI_U32 u32ZlibCmStop    :   1;
        HI_U32 u32AdlerStop     :   1;
        HI_U32 u32CrcStop       :   1;
    }stBits;
    HI_U32 u32All;
}PNG_ERR_MODE_U;

typedef union tag_PNG_ERR_STATICS_U
{
    struct
    {
        HI_U32 u32TotalIdatNums :   8;
        HI_U32 u32ErrIdatNums   :   8;
        HI_U32 u32Reserved      :   16;
    }stBits;
    HI_U32 u32All;
}PNG_ERR_STATICS_U;

#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
typedef union tag_PNG_MMU_BYPASS_U
{
    struct
    {
        HI_U32 u32StreamMmuBypass    :    1;
        HI_U32 u32RdcRdMmuBypass    :    1;
        HI_U32 u32FilterRdMmuBypass    :    1;
        HI_U32 u32RdcWtMmuBypass    :    1;
        HI_U32 u32FilterWtMmuBypass    :    1;
        HI_U32 u32OutMmuBypass        :    1;
        HI_U32 u32Reserved            :    26;
    }stBits;
    HI_U32 u32All;
}PNG_MMU_BYPASS_U;
#endif

/*Define PNG register structure*/
/*CNcomment:png寄存器结构体 */
typedef struct tag_PNG_HAL_REGISTER_S
{
    volatile HI_U32 u32DecStart;
    volatile HI_U32 u32ResumeStart;
    volatile HI_U32 u32Stride;  /*4 byte aligned*//*CNcomment:4字节对齐*/
    volatile PNG_SIZE_U uSize;
    volatile PNG_TYPE_U uType;
    volatile PNG_TRANSFORM_U uTransform;
    volatile HI_U32 u32Reserved1;
    volatile HI_U32 u32Reserved2;
    volatile HI_U32 u32BitBufStAddr;
    volatile HI_U32 u32BitBufEndAddr;
    volatile HI_U32 u32BitStreamStAddr;
    volatile HI_U32 u32BitStreamEndAddr;
    volatile HI_U32 u32Pass0StAddr;
    volatile HI_U32 u32Pass1StAddr;
    volatile HI_U32 u32Pass2StAddr;
    volatile HI_U32 u32Pass3StAddr;
    volatile HI_U32 u32Pass4StAddr;
    volatile HI_U32 u32Pass5StAddr;
    volatile HI_U32 u32Pass6StAddr;
    volatile HI_U32 u32FinalStAddr;
    volatile HI_U32 u32FinalEndAddr;
    volatile HI_U32 u32Reserved3;
    volatile HI_U32 u32Reserved4;
    volatile HI_U32 u32Pass0Stride;
    volatile HI_U32 u32Pass1Stride;
    volatile HI_U32 u32Pass2Stride;
    volatile HI_U32 u32Pass3Stride;
    volatile HI_U32 u32Pass4Stride;
    volatile HI_U32 u32Pass5Stride;
    volatile HI_U32 u32Pass6Stride;
    volatile PNG_TRANS_COLOR0_U uTransColor0;
    volatile PNG_TRANS_COLOR0_U uTransColor1;
    volatile HI_U32 u32RdcStAddr;
    volatile HI_U32 u32Reserved5;
    volatile HI_U32 u32FltStAddr;
    volatile HI_U32 u32FltEndAddr;

    /*Suggest value is 0x20441*/
    /*CNcomment:0x90 建议配置成0x20441*/
    volatile HI_U32 u32AXIConfig;

    /*Suggest value is 0x80008*/
    /*CNcomment:0x94 建议配置成0x80008*/
    volatile HI_U32 u32TimeOut;

#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798hv100)
    volatile PNG_MMU_BYPASS_U uMmuBypass;    /* 0x98 */
#else
    volatile HI_U32 u32Reserved6;       /* 0x98 */
#endif
    volatile HI_U32 u32Reserved7;       /* 0x9c*/
    volatile HI_U32 u32Reserved8;   /* 0xa0 */
    volatile HI_U32 u32Reserved9;   /* 0xa4 */
    volatile HI_U32 u32Reserved10;   /* 0xa8 */
    volatile HI_U32 u32Reserved11;   /* 0xac*/
    volatile HI_U32 u32Reserved12;   /* 0xb0 */
    volatile HI_U32 u32Reserved13;   /* 0xb4 */
    volatile HI_U32 u32Reserved14;   /* 0xb8 */
    volatile HI_U32 u32Reserved15;   /* 0xbc*/
    volatile HI_U32 u32Reserved16;   /* 0xc0 */
    volatile HI_U32 u32Reserved17;   /* 0xc4 */
    volatile HI_U32 u32Reserved18;   /* 0xc8 */
    volatile HI_U32 u32Reserved19;   /* 0xcc*/
    volatile HI_U32 u32Reserved20;   /* 0xd0 */
    volatile HI_U32 u32Reserved21;   /* 0xd4 */
    volatile HI_U32 u32Reserved22;   /* 0xd8 */
    volatile HI_U32 u32Reserved23;   /* 0xdc*/
    volatile HI_U32 u32Reserved24;   /* 0xe0 */
    volatile HI_U32 u32Reserved25;   /* 0xe4 */
    volatile HI_U32 u32Reserved26;   /* 0xe8 */
    volatile HI_U32 u32Reserved27;   /* 0xec*/
    volatile HI_U32 u32Reserved28;   /* 0xf0 */
    volatile HI_U32 u32Reserved29;   /* 0xf4 */
    volatile HI_U32 u32Reserved30;   /* 0xf8 */
    volatile HI_U32 u32Reserved31;   /* 0xfc*/
    volatile PNG_INT_U uInt;        /* 0x100 */
    volatile PNG_INT_MASK_U uIntMask;
    volatile PNG_ERR_MODE_U uErrMode;
    volatile PNG_ERR_STATICS_U uErrStatics;
    volatile HI_U32 u32RstBusy;
}PNG_HAL_REGISTER_S;

/********************************************************************************************
* func:    Initialize Hal layerHal     CNcomment:层初始化
* in:    none
* out:    none
* ret:    HI_SUCCESS    Open dev success  CNcomment:成功打开设备
* ret    HI_ERR_PNG_SYS    System err, such as map register failed
                                 CNcomment:系统错误,比如寄存器映射失败
* others:
*********************************************************************************************/
HI_S32 PngHalInit(HI_VOID);

/********************************************************************************************
* func:    Delete initialize Hal layer     CNcomment:Hal层去初始化
* in:    none
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalDeinit(HI_VOID);

/********************************************************************************************
* func:    Reset register      CNcomment:寄存器复位
* in:    none
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalReset(HI_VOID);

/********************************************************************************************
* func:    Set base info of image      CNcomment:设置图片基本信息
* in:    stImgInfo Structrue of image info   CNcomment:图片信息结构体
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetImgInfo(HI_PNG_INFO_S stImgInfo);

/********************************************************************************************
* func:    Set data trasnformation     CNcomment:设置数据转换
* in:    stTransform Structure for data trasnformation   CNcomment:数据转换结构体
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetTransform(HI_PNG_TRANSFORM_S stTransform);


/********************************************************************************************
* func:    Set start physical address of RDC window    CNcomment:设置游程窗口起始物理地址
* in:    u32Phyaddr Physical address     CNcomment:物理地址
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetRdcAddr(HI_U32 u32Phyaddr);

/********************************************************************************************
* func:    Get start physical address of RDC window    CNcomment:获取游程窗口起始物理地址
* in:    none
* out:    none
* ret:    Physical address        CNcomment:物理地址
* others:
*********************************************************************************************/
HI_U32 PngHalGetRdcAddr(HI_VOID);

/********************************************************************************************
* func:    Set start address and size of filter buf     CNcomment:设置滤波buf起始、结束物理地址
* in:    u32Phyaddr Physical address     CNcomment:物理地址
* in:    u32Size Size of buf     CNcomment:buf大小
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetFltAddr(HI_U32 u32Phyaddr, HI_U32 u32Size);

/********************************************************************************************
* func:    Set target physical address and line length     CNcomment:设置目标buf
* in:    u32Phyaddr Physical address     CNcomment:物理地址
* in:    u32Stride Line length of buf    CNcomment:目标buf行宽
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetTgt(HI_U32 u32Phyaddr, HI_U32 u32Stride);

/********************************************************************************************
* func:    Set start physical address and size of stream buf   CNcomment:设置码流buf地址
* in:    u32Phyaddr Start physical address   CNcomment:物理地址
* in:    u32Size Size of buf     CNcomment:buf大小
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetStreamBuf(HI_U32 u32Phyaddr, HI_U32 u32Size);

/********************************************************************************************
* func:    Set start physical address and size of stream data      CNcomment:设置码流地址
* in:    u32Phyaddr Start physical address of stream     CNcomment:物理地址
* in:    u32Size Size of stream      CNcomment:码流大小
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetStreamAddr(HI_U32 u32Phyaddr, HI_U32 u32Size);

/********************************************************************************************
* func:    Start decoding      CNcomment:启动解码
* in:    none
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalStartDecode(HI_VOID);

/********************************************************************************************
* func:    Reusme decoding     CNcomment:续码流恢复解码
* in:    none
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalResumeDecode(HI_VOID);

/********************************************************************************************
* func:    Get interrupt value     CNcomment:读取中断状态并清中断
* in:    none
* out:    Interrupt value         CNcomment:中断状态值
* ret:    none
* others:
*********************************************************************************************/
HI_U32 PngHalGetIntStatus(HI_VOID);

/********************************************************************************************
* func:    Set port AXI and timeout        CNcomment:设置端口AXI和超时
* in:    none
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetAxiAndTimeout(HI_VOID);

/********************************************************************************************
* func:    Set interrupt mask value        CNcomment:设置中断掩码
* in:    u32Value Interrupt mask value       CNcomment:掩码值
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetIntmask(HI_U32 u32Value);

/********************************************************************************************
* func:    Set mode of handling err        CNcomment:设置错误处理模式
* in:    u32Value Mode of handling err       CNcomment:错误处理模式
* out:    none
* ret:    none
* others:
*********************************************************************************************/
HI_VOID PngHalSetErrmode(HI_U32 u32Value);

HI_VOID PngHalSetClock(HI_BOOL bEnable);

HI_S32 PngHalOpen(HI_VOID);

HI_U32 PngHalGetIrqNum(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cpulsplus */

#endif  /* __PNG_HAL_H__ */
