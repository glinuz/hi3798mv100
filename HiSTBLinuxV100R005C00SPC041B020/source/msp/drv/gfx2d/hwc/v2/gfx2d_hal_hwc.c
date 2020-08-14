/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: gfx2d_hal_hwc.c
* Description: Graphic 2D engine hwc code
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-10-23              NULL         Create this file.
*******************************************************************************/
#include "hi_gfx2d_type.h"
#include "hi_gfx_comm_k.h"
#include "gfx2d_hal.h"
#include "hi_reg_common.h"
#include "hi_gfx2d_errcode.h"
#include "gfx2d_hal_hwc_filterPara.h"
#include "gfx2d_hal_hwc_adp.h"
#include "gfx2d_mem.h"
#include "gfx2d_config.h"
#include "drv_mmz.h"
#include "hi_gfx_sys_k.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#define HWC_REG_BASE_ADDR 0xf8c10000

#define HWC_ISRNUM 0x7b

#define GFX2D_HAL_MAX_NODE 7

#define GFX2D_CONFIG_MAX_ZMERATION 256

/*获取滤波系数的符号位及低9bit,组成10bit数*/
#define HWC_GET_BITS_OF_FILTERPARA(para) ((((para) & 0x80000000) >> 22) | ((para) & 0x1ff))

#define GFX2D_HAL_HWC_HSTEP_FLOATLEN 20
#define GFX2D_HAL_HWC_NOSCALE_HSTEP (0x1 << GFX2D_HAL_HWC_HSTEP_FLOATLEN)
#define GFX2D_HAL_HWC_VSTEP_FLOATLEN 12
#define GFX2D_HAL_HWC_NOSCALE_VSTEP (0x1 << GFX2D_HAL_HWC_VSTEP_FLOATLEN)

#define GFX2D_CONFIG_NODE_SIZE ((sizeof(HWC_NODE_S) + 0xf) & 0xffffff0)

typedef union tagHWC_POS_U
{
    struct
    {
        HI_U32 bit16X : 16;
        HI_U32 bit16Y : 16;
    } stBits;
    HI_U32 u32All;
} HWC_POS_U;

typedef union tagHWC_RES_U
{
    struct
    {
        HI_U32 bit16Width : 16;
        HI_U32 bit16Height : 16;
    } stBits;
    HI_U32 u32All;
} HWC_RES_U;

typedef union tagHWC_VHDCSCIDC_U
{
    struct
    {
        HI_U32 bit9Idc0 : 9;
        HI_U32 bit9Idc1 : 9;
        HI_U32 bit9Idc2 : 9;
        HI_U32 bit1IsEnable : 1;
        HI_U32 bit1Reserved : 4;
    } stBits;
    HI_U32 u32All;
} HWC_VHDCSCIDC_U;

typedef union tagHWC_VHDCSCODC_U
{
    struct
    {
        HI_U32 bit9Odc0 : 9;
        HI_U32 bit9Odc1 : 9;
        HI_U32 bit9Odc2 : 9;
        HI_U32 bit5Reserved : 5;
    } stBits;
    HI_U32 u32All;
} HWC_VHDCSCODC_U;

typedef union tagHWC_VHDCSCPARAM_U
{
    struct
    {
        HI_U32 bit15Param0 : 15;
        HI_U32 bit1Reserved1 : 1;
        HI_U32 bit15Param1 : 15;
        HI_U32 bit1Reserved2 : 1;
    } stBits;
    HI_U32 u32All;
} HWC_VHDCSCPARAM_U;

typedef union tagHWC_VHDHSP_U
{
    struct
    {
        HI_U32 bit24Ratio : 24;
        HI_U32 bit1Order : 1;
        HI_U32 bit1ChrmoaMode : 1;
        HI_U32 bit1LumMode : 1;
        HI_U32 bit1AlphaMode : 1;
        HI_U32 bit1IsChromaMid : 1;
        HI_U32 bit1IsLumMid : 1;
        HI_U32 bit1IsChromaEnable : 1;
        HI_U32 bit1IsLumEnable : 1;
    } stBits;
    HI_U32 u32All;
} HWC_VHDHSP_U;

typedef union tagHWC_VHDHOFFSET_U
{
    struct
    {
        HI_U32 bit28Offset : 28;
        HI_U32 bit4Reserved : 4;
    } stBits;
    HI_U32 u32All;
} HWC_VHDHOFFSET_U;

typedef union tagHWC_VHDVSP_U
{
    struct
    {
        HI_U32 bit19Reserved : 19;
        HI_U32 bit2InFmt : 2;
        HI_U32 bit2OutFmt : 2;
        HI_U32 bit1ChromaMode : 1;
        HI_U32 bit1LumMode : 1;
        HI_U32 bit1Reserved1 : 1;
        HI_U32 bit1ChromaTap : 1;
        HI_U32 bit1Reserved2 : 1;
        HI_U32 bit1IsChromaMid : 1;
        HI_U32 bit1IsLumMid : 1;
        HI_U32 bit1IsChromaEnable : 1;
        HI_U32 bit1IsLumEnable : 1;
    } stBits;
    HI_U32 u32All;
} HWC_VHDVSP_U;

typedef union tagHWC_CBMMIX_U
{
    struct
    {
        HI_U32 bit3Prio0 : 3;
        HI_U32 bit3Prio1 : 3;
        HI_U32 bit3Prio2 : 3;
        HI_U32 bit3Prio3 : 3;
        HI_U32 bit3Prio4 : 3;
        HI_U32 bit3Prio5 : 3;
        HI_U32 bit3Prio6 : 3;
        HI_U32 bit4AluMode              : 4   ; /* [24..21]  */
        HI_U32 bit1CbmMode              : 1   ; /* [25]  */
        HI_U32 bit5Reserved            : 5   ; /* [30..26]  */
        HI_U32 bit1En                : 1   ; /* [31]  */
    } stBits;
    HI_U32 u32All;
} HWC_CBMMIX_U;

typedef union tagHWC_OUTCSCIDC_U
{
    struct
    {
        HI_U32 bit9Idc0 : 9;
        HI_U32 bit9Idc1 : 9;
        HI_U32 bit9Idc2 : 9;
        HI_U32 bit1IsEnable : 1;
        HI_U32 bit4Reserved : 4;
    } stBits;
    HI_U32 u32All;
} HWC_OUTCSCIDC_U;

typedef union tagHWC_OUTCSCODC_U
{
    struct
    {
        HI_U32 bit9Odc0 : 9;
        HI_U32 bit9Odc1 : 9;
        HI_U32 bit9Odc2 : 9;
        HI_U32 bit5Reserved : 5;
    } stBits;
    HI_U32 u32All;
} HWC_OUTCSCODC_U;

typedef union tagHWC_OUTCSCPARAM_U
{
    struct
    {
        HI_U32 bit15Param0 : 15;
        HI_U32 bit1Reserved1 : 1;
        HI_U32 bit15Param1 : 15;
        HI_U32 bit1Reserved2 : 1;
    } stBits;
    HI_U32 u32All;
} HWC_OUTCSCPARAM_U;

typedef union tagHWC_CMPCTRL_U
{
    struct
    {
        HI_U32 bit1CmpEnable : 1;
        HI_U32 bit1LossEnable : 1;
        HI_U32 bit1SgmtType : 1;
        HI_U32 bit29Reserved : 29;
    } stBits;
    HI_U32 u32All;
} HWC_CMPCTRL_U;

typedef union tagHWC_CROPPOS_U
{
    struct
    {
        HI_U32 bit13XPos : 13;
        HI_U32 bit3Reserved1 : 3;
        HI_U32 bit13YPos : 13;
        HI_U32 bit2Reserved2 : 2;
        HI_U32 bit1IsEnable : 1;
    } stBits;
    HI_U32 u32All;
} HWC_CROPPOS_U;

typedef union tagHWC_CROPRESO_U
{
    struct
    {
        HI_U32 bit13Width : 13;
        HI_U32 bit3Reserved1 : 3;
        HI_U32 bit13Height : 13;
        HI_U32 bit2Reserved2 : 3;
    } stBits;
    HI_U32 u32All;
} HWC_CROPRESO_U;

typedef union tagHWC_CMPGLBINFO_U
{
    struct
    {
        HI_U32 bit1IsLossLess : 1;
        HI_U32 bit1IsAlphaLossLess : 1;
        HI_U32 bit1Mode : 1;
        HI_U32 bit2OsdMode : 2;
        HI_U32 bit1IsPartition : 1;
        HI_U32 bit3PartitionNum : 3;
        HI_U32 bit23Reserved : 23;
    } stBits;
    HI_U32 u32All;
} HWC_CMPGLBINFO_U;

typedef union tagHWC_CMPRESO_U
{
    struct
    {
        HI_U32 bit13Width : 13;
        HI_U32 bit3Reserved1 : 3;
        HI_U32 bit13Height : 13;
        HI_U32 bit2Reserved2 : 3;
    } stBits;
    HI_U32 u32All;
} HWC_CMPRESO_U;

typedef union tagHWC_CMPCFG0_U
{
    struct
    {
        HI_U32 bit10BudgetBitsMb : 10;
        HI_U32 bit6Reserved1 : 6;
        HI_U32 bit10MinMbBits : 10;
        HI_U32 bit2Reserved2 : 6;
    } stBits;
    HI_U32 u32All;
} HWC_CMPCFG0_U;

typedef union tagHWC_CMPCFG1_U
{
    struct
    {
        HI_U32 bit10BudgetBitsMbCap : 10;
        HI_U32 bit6Reserved : 22;
    } stBits;
    HI_U32 u32All;
} HWC_CMPCFG1_U;

typedef union tagHWC_CMPCFG2_U
{
    struct
    {
        HI_U32 bit4MaxQp : 4;
        HI_U32 bit4Reserved1 : 4;
        HI_U32 bit4SadBitsNgain : 4;
        HI_U32 bit4Reserved2 : 4;
        HI_U32 bit3RcSmthNgain : 3;
        HI_U32 bit5Reserved3 : 5;
        HI_U32 bit4SpecialBitGain : 4;
        HI_U32 bit4Reserved4 : 4;
    } stBits;
    HI_U32 u32All;
} HWC_CMPCFG2_U;

typedef union tagHWC_CMPCFG3_U
{
    struct
    {
        HI_U32 bit7MaxSadThr : 7;
        HI_U32 bit9Reserved1 : 9;
        HI_U32 bit7MinSadThr : 7;
        HI_U32 bit9Reserved2 : 9;
    } stBits;
    HI_U32 u32All;
} HWC_CMPCFG3_U;

typedef union tagHWC_CMPCFG4_U
{
    struct
    {
        HI_U32 bit7SmthThr : 7;
        HI_U32 bit1Reserved1 : 1;
        HI_U32 bit7StillThr : 7;
        HI_U32 bit1Reserved2 : 1;
        HI_U32 bit10BigGradThr : 10;
        HI_U32 bit6Reserved3 : 6;
    } stBits;
    HI_U32 u32All;
} HWC_CMPCFG4_U;

typedef union tagHWC_CMPCFG5_U
{
    struct
    {
        HI_U32 bit6SmthPixNumThr : 6;
        HI_U32 bit2Reserved1 : 2;
        HI_U32 bit6StillPixNumThr : 6;
        HI_U32 bit2Reserved2 : 2;
        HI_U32 bit6NoisePixNumThr : 6;
        HI_U32 bit2Reserved3 : 2;
        HI_U32 bit6LargeSmthPixNumThr : 6;
        HI_U32 bit2Reserved4 : 2;
    } stBits;
    HI_U32 u32All;
} HWC_CMPCFG5_U;

typedef union tagHWC_CMPCFG6_U
{
    struct
    {
        HI_U32 bit7NoiseSad : 7;
        HI_U32 bit9Reserved1 : 9;
        HI_U32 bit9PixDiffThr : 9;
        HI_U32 bit7Reserved2 : 7;
    } stBits;
    HI_U32 u32All;
} HWC_CMPCFG6_U;

typedef union tagHWC_CMPCFG7_U
{
    struct
    {
        HI_U32 bit7AdjSadBitsThr : 7;
        HI_U32 bit1Reserved1 : 1;
        HI_U32 bit8MaxTrowBits : 8;
        HI_U32 bit16Reserved2 : 16;
    } stBits;
    HI_U32 u32All;
} HWC_CMPCFG7_U;

typedef union tagHWC_CMPCFG8_U
{
    struct
    {
        HI_U32 bit8QpInc1BitsThr : 8;
        HI_U32 bit8QpDec1BitsThr : 8;
        HI_U32 bit8QpInc2BitsThr : 8;
        HI_U32 bit8QpDec2BitsThr : 8;
    } stBits;
    HI_U32 u32All;
} HWC_CMPCFG8_U;

typedef union tagHWC_CMPCFG9_U
{
    struct
    {
        HI_U32 bit10ForceQpThr : 10;
        HI_U32 bit6Reserved1 : 6;
        HI_U32 bit10ForceQpThrCap : 10;
        HI_U32 bit6Reserved2 : 6;
    } stBits;
    HI_U32 u32All;
} HWC_CMPCFG9_U;

typedef struct tagHWC_REG_S
{
    HI_U32 u32AqListHeadAddr;
    HI_U32 u32HwcCtl;
    HI_U32 u32MaskInt;
    HI_U32 u32IntClr;
    HI_U32 u32Int;
    HI_U32 u32TimeCnt;
    HI_U32 u32MiscEllaneous;
} HWC_REG_S;

typedef union tagHWC_SRCCTL_U
{
    struct
    {
        HI_U32 bit6Fmt             : 6   ; /* [5..0]  */
        HI_U32 bit5ArgbOrder       : 5   ; /* [10..6]  */
        HI_U32 bit1UVOrder         : 1   ; /* [11]  */
        HI_U32 bit2ExpandMode      : 2   ; /* [13..12]  */
        HI_U32 bit4Reserved0       : 4   ; /* [17..14]  */
        HI_U32 bit1AlphaRange      : 1   ; /* [18]  */
        HI_U32 bit1VScanOrd        : 1   ; /* [19]  */
        HI_U32 bit1HScanOrd        : 1   ; /* [20]  */
        HI_U32 bit1422Pro          : 1   ; /* [21]  */
        HI_U32 bit7Reserved1       : 7   ; /* [28..22]  */
        HI_U32 bit1Dma             : 1   ; /* [29]  */
        HI_U32 bit1IsDim           : 1   ; /* [30]  */
        HI_U32 bit1IsEnable        : 1   ; /* [31]  */
    } stBits;
    HI_U32 u32All;
} HWC_SRCCTL_U;

typedef union tagHWC_ALPHA_U
{
    struct
    {
        HI_U32 bit8Alpha0           : 8   ; /* [7..0]  */
        HI_U32 bit8Alpha1           : 8   ; /* [15..8]  */
        HI_U32 bit16Reserved        : 16  ; /* [31..16]  */
    } stBits;
    HI_U32 u32All;
} HWC_ALPHA_U;

typedef struct tagHWC_LAYER_S
{
    HWC_SRCCTL_U unCtrl;                    /* 0x0 */
    HI_U32 u32Phyaddr;                      /* 0x4 */
    HI_U32 u32CbcrPhyaddr;                      /* 0x8 */
    HI_U32 u32Stride;                       /* 0xc */
    HI_U32 u32CbcrStride;                    /* 0x10 */
    HWC_RES_U unRes;                        /* 0x14 */
    HI_U32 u32DimValue;                     /* 0x18 */
    HWC_ALPHA_U unAlpha;                    /* 0x1c */
    HI_U32 u32PixOffset;                        /* 0x20 */
    HI_U32 u32Reserved[7];                  /* 0x24~0x3c */
} HWC_LAYER_S;

typedef union tagHWC_OUTCTL_U
{
    struct
    {
        HI_U32 bit6Fmt : 6;
        HI_U32 bit5ArgbOrder : 5;
        HI_U32 bit1CbcrOrder : 1;
        HI_U32 bit5Reserved0 : 5;
        HI_U32 bit1Dither  : 1;
        HI_U32 bit1AlphaRange : 1;
        HI_U32 bit1VScanOrder : 1;
        HI_U32 bit1HScanOrder : 1;
        HI_U32 bit2Reserved1 : 2;
        HI_U32 bit1CmpEn : 1;
        HI_U32 bit1CmpAddrChangeEn : 1;
        HI_U32 bit2Reserved2 : 2;
        HI_U32 bit1ChormeMmuBypass : 1;
        HI_U32 bit1LumMmuBypass : 1;
        HI_U32 bit1ARMmuBypass : 1;
        HI_U32 bit1GBMmuBypass : 1;
        HI_U32 bit1Enable : 1;
    } stBits;
    HI_U32 u32All;
} HWC_OUTCTL_U;

/* Define the union U_DES_ALPHA */
typedef union tagHWC_OUTALPHA_U
{
    struct
    {
        HI_U32 bit8AlphaThreshold   : 8   ; /* [7..0]  */
        HI_U32 bit1CropMode         : 1   ; /* [8]  */
        HI_U32 bit1CropEn           : 1   ; /* [9]  */
        HI_U32 bit22Reserved        : 22  ; /* [31..10]  */
    } stBits;
    HI_U32 u32All;
} HWC_OUTALPHA_U;

typedef union tagHWC_MMU_PROT_CTL_U
{
    struct
    {
        HI_U32 bit1Src1Ch0MmuBypass   : 1   ; /* [0]  */
        HI_U32 bit1Src1Ch1MmuBypass   : 1   ; /* [1]  */
        HI_U32 bit1Src2Ch0MmuBypass   : 1   ; /* [2]  */
        HI_U32 bit1Src2Ch1MmuBypass   : 1   ; /* [3]  */
        HI_U32 bit1Src3MmuBypass      : 1   ; /* [4]  */
        HI_U32 bit1Src4MmuBypass       : 1   ; /* [5]  */
        HI_U32 bit1Src5MmuBypass       : 1   ; /* [6]  */
        HI_U32 bit1Src6MmuBypass       : 1   ; /* [7]  */
        HI_U32 bit1Src7MmuBypass       : 1   ; /* [8]  */
        HI_U32 bit1ClutMmuBypass       : 1   ; /* [9]  */
        HI_U32 bit6Reserved0            : 6   ; /* [15..10]  */
        HI_U32 bit1Src1Ch0Prot         : 1   ; /* [16]  */
        HI_U32 bit1Src1Ch1Prot         : 1   ; /* [17]  */
        HI_U32 bit1Src2Ch0Prot         : 1   ; /* [18]  */
        HI_U32 bit1Src2Ch1Prot         : 1   ; /* [19]  */
        HI_U32 bit1Src3Prot             : 1   ; /* [20]  */
        HI_U32 bit1Src4Prot             : 1   ; /* [21]  */
        HI_U32 bit1Src5Prot             : 1   ; /* [22]  */
        HI_U32 bit1Src6Prot             : 1   ; /* [23]  */
        HI_U32 bit1Src7Prot             : 1   ; /* [24]  */
        HI_U32 bit7Reserved1            : 7   ; /* [31..25]  */
    } stBits;
    HI_U32 u32All;
} HWC_MMU_PROT_CTL_U;

typedef union tagHWC_HPZME_U
{
    struct
    {
        HI_U32 bit1ZmehDwsm             : 1   ; /* [0]  */
        HI_U32 bit3Reserved0            : 3   ; /* [3..1]  */
        HI_U32 bit4ZmehDwsMode          : 4   ; /* [7..4]  */
        HI_U32 bit24Reserved1           : 24  ; /* [31..8]  */
    } stBits;
    HI_U32 u32All;
} HWC_HPZME_U;

typedef union tagHWC_CSCMUX_U
{
    struct
    {
        HI_U32 bit1CscMode              : 1   ; /* [0]  */
        HI_U32 bit1OutPremultEn         : 1   ; /* [1]  */
        HI_U32 bit30Reserved            : 30  ; /* [31..2]  */
    } stBits;
    HI_U32 u32All;
} HWC_CSCMUX_U;

typedef union tagHWC_OUTDSWM_U
{
    struct
    {
        HI_U32 bit2HDswmMode       : 2   ; /* [1..0]  */
        HI_U32 bit2Reserved0            : 2   ; /* [3..2]  */
        HI_U32 bit1VDswmMode       : 1   ; /* [4]  */
        HI_U32 bit27Reserved1            : 27  ; /* [31..5]  */
    } stBits;
    HI_U32 u32All;
} HWC_OUTDSWM_U;

typedef union tagHWC_CBMPARA_U
{
    struct
    {
        HI_U32 bit1GlobalAlphaEnable           : 1   ; /* [0]  */
        HI_U32 bit1PixelAlphaEnable           : 1   ; /* [1]  */
        HI_U32 bit1bPreMul          : 1   ; /* [2]  */
        HI_U32 bit1MultiGEn      : 1   ; /* [3]  */
        HI_U32 bit4BlendMode          : 4   ; /* [7..4]  */
        HI_U32 bit8GlobalAlpha             : 8   ; /* [15..8]  */
        HI_U32 bit15Reserved            : 15  ; /* [30..16]  */
        HI_U32 bit1bCovBlend       : 1   ; /* [31]  */
    } stBits;
    HI_U32 u32All;
} HWC_CBMPARA_U;

typedef struct tagHWC_CBMINFO_S
{
    HWC_CBMPARA_U unCbmPara;
    HWC_POS_U unCbmStartPos;
} HWC_CBMINFO_S;

typedef union tagHWC_INTMASK_U
{
    struct
    {
        HI_U32 bit1NodeCompMask              : 1   ; /* [0]  */
        HI_U32 bit1TimeoutMask          : 1   ; /* [1]  */
        HI_U32 bit1BusErrMask          : 1   ; /* [2]  */
        HI_U32 bit1ListCompMask          : 1   ; /* [3]  */
        HI_U32 bit28Reserved            : 28  ; /* [31..4]  */
    } stBits;
    HI_U32 u32All;
} HWC_INTMASK_U;

typedef struct tagHWC_NODE_S
{
    HWC_LAYER_S astLayer[7];                /*0x0 ~ 0x1bc*/
    HWC_OUTCTL_U unOutCtl;                  /* 0x1c0 */
    HI_U32 u32OutPhyaddr;                   /* 0x1c4 */
    HI_U32 u32OutChromPhyaddr;                   /* 0x1c8 */
    HI_U32 u32OutStride;                 /* 0x1cc */
    HI_U32 u32OutChromStride;                 /* 0x1d0 */
    HWC_RES_U unOutRes;                     /* 0x1d4 */
    HWC_OUTALPHA_U unOutAlpha;              /* 0x1d8 */
    HWC_POS_U unOutCropStart;                  /* 0x1dc */
    HWC_POS_U unOutCropEnd;                    /* 0x1e0 */
    HWC_MMU_PROT_CTL_U unMmuCtl;            /* 0x1e4 */
    HI_U32 u32OutPixOffset;                        /* 0x1e8 */
    HI_U32 u32ARAddr;                       /* 0x1ec */
    HI_U32 u32GBAddr;                       /* 0x1f0 */
    HI_U32 u32Reserved0[3];                 /* 0x1f4~0x1fc */

    HWC_VHDHSP_U unHsp;                     /* 0x200 */
    HWC_VHDHOFFSET_U unHLumOffset;          /* 0x204 */
    HWC_VHDHOFFSET_U unHChromaOffset;       /* 0x208 */
    HWC_VHDVSP_U unVsp;                     /* 0x20c */
    HI_U32 u32Vsr;                          /* 0x210 */
    HI_U32 u32VOffset;                      /* 0x214 */
    HWC_RES_U unZmeOutRes;                  /* 0x218 */
    HWC_RES_U unZmeInRes;                   /* 0x21c */
    HI_U32 u32Reserved1[2];                 /* 0x220~0x224 */

    HWC_HPZME_U  unHpZme;                   /* 0x228 */
    HWC_RES_U unHpZmeReso;                  /* 0x22c */

    HWC_OUTCSCIDC_U unVhdCscIdc;               /* 0x230 */
    HWC_OUTCSCODC_U unVhdCscOdc;               /* 0x234 */

    HWC_OUTCSCPARAM_U unVhdCscParam0;          /* 0x238 */
    HWC_OUTCSCPARAM_U unVhdCscParam1;          /* 0x23c */
    HWC_OUTCSCPARAM_U unVhdCscParam2;          /* 0x240 */
    HWC_OUTCSCPARAM_U unVhdCscParam3;          /* 0x244 */
    HWC_OUTCSCPARAM_U unVhdCscParam4;          /* 0x248 */


    HI_U32 u32Reserved2[13];                /* 0x24c~0x27c */
    HWC_VHDHSP_U unVhdHsp;                  /* 0x280 */
    HWC_VHDHOFFSET_U unVhdLumHOffset;       /* 0x284 */
    HWC_VHDHOFFSET_U unVhdChromaHOffset;    /* 0x288 */
    HWC_VHDVSP_U unVhdVsp;                  /* 0x28c */
    HI_U32 u32VhdVsr;                       /* 0x290 */
    HI_U32 u32VhdVOffset;                   /* 0x294 */
    HWC_RES_U unVhdZmeOutRes;               /* 0x298 */
    HWC_RES_U unVhdZmeInRes;                /* 0x29c */
    HI_U32 u32Reserved3[2];                 /* 0x2a0~0x2a4 */
    HWC_HPZME_U unVhdHpZme;                 /* 0x2a8 */
    HWC_RES_U unVhdHpZmeReso;               /* 0x2ac */

    HWC_CSCMUX_U unCscMux;                  /* 0x2b0 */
    HI_U32 Reserved4[3];                    /* 0x2b4~0x2bc */

    HWC_OUTCSCIDC_U unOutCscIdc;            /* 0x2c0 */
    HWC_OUTCSCODC_U unOutCscOdc;            /* 0x2c4 */
    HWC_OUTCSCPARAM_U unOutCscParam0;       /* 0x2c8 */
    HWC_OUTCSCPARAM_U unOutCscParam1;       /* 0x2cc */
    HWC_OUTCSCPARAM_U unOutCscParam2;       /* 0x2d0 */
    HWC_OUTCSCPARAM_U unOutCscParam3;       /* 0x2d4 */
    HWC_OUTCSCPARAM_U unOutCscParam4;       /* 0x2d8 */

    HWC_OUTDSWM_U unOutDswm;                /* 0x2dc */
    HI_U32 Reserved5[8];                    /* 0x2e0~0x2fc */

    HWC_CBMMIX_U unCbmMix;                  /* 0x300 */
    HI_U32 u32CbmBkgValue;                  /* 0x304 */

    HI_U32 Reserved6[6];                    /* 0x308~0x31c */

    HWC_CBMINFO_S astCmbInfo[7];            /* 0x320~0x354 */

    HI_U32 Reserved7[26];                   /* 0x358~0x3bc */

    HWC_CMPGLBINFO_U unCmpGlbInfo;          /* 0x3c0 */
    HWC_CMPRESO_U unCmpReso;                /* 0x3c4 */
    HWC_CMPCFG0_U unCmpCfg0;                /* 0x3c8 */
    HWC_CMPCFG1_U unCmpCfg1;                /* 0x3cc */
    HWC_CMPCFG2_U unCmpCfg2;                /* 0x3d0 */
    HWC_CMPCFG3_U unCmpCfg3;                /* 0x3d4 */
    HWC_CMPCFG4_U unCmpCfg4;                /* 0x3d8 */
    HWC_CMPCFG5_U unCmpCfg5;                /* 0x3dc */
    HWC_CMPCFG6_U unCmpCfg6;                /* 0x3e0 */
    HWC_CMPCFG7_U unCmpCfg7;                /* 0x3e4 */
    HWC_CMPCFG8_U unCmpCfg8;                /* 0x3e8 */
    HWC_CMPCFG9_U unCmpCfg9;                /* 0x3ec */
    HI_U32 u32CmpMaxRowLen;                 /* 0x3f0 */
    HI_U32 Reserved8[35];                   /* 0x3f4~0x47c */
    HI_U32 u32G0HCoefPhyaddr;               /* 0x480 */
    HI_U32 u32G0VCoefPhyaddr;               /* 0x484 */
    HI_U32 u32G0HChromaCoefPhyaddr;         /* 0x488 */
    HI_U32 u32G0VChromaCoefPhyaddr;         /* 0x48c */
    HI_U32 u32HCoefPhyaddr;                 /* 0x490 */
    HI_U32 u32VCoefPhyaddr;                 /* 0x494 */
    HI_U32 u32HChromaCoefPhyaddr;           /* 0x498 */
    HI_U32 u32VChromaCoefPhyaddr;           /* 0x49c */
    HI_U32 Reserved9[20];                   /* 0x4a0~0x4ec */
    HI_U32 u32AXIID;                        /* 0x4f0 */
    HI_U32 u32NodeId;                       /* 0x4f4 */
    HWC_INTMASK_U unIntMask;                /* 0x4f8 */
    HI_U32 u32NextNodePhyaddr;
    HI_VOID *pNextNode;
    HI_VOID *pFirstNode;
} HWC_NODE_S;

/*硬件层ID定义*/
typedef enum
{
    HWC_LAYER_VHD = 0x1,
    HWC_LAYER_G0,
    HWC_LAYER_G1,
    HWC_LAYER_G2,
    HWC_LAYER_G3,
    HWC_LAYER_G4,
    HWC_LAYER_G5,
    HWC_LAYER_BUTT
} HWC_LAYER_ID;

/*水平系数存储方式:每8个系数一组，3 + 3 + 2 占用12byte*/
#define HWC_HORIZON_LUM_PARA_SIZE_PERLEVEL (((HWC_FILTER_PHASE * 12) + 0xf) & 0xfffffff0)

/*垂直系数存储方式:每4个系数一组，占用8byte*/
#define HWC_VERTICAL_LUM_PARA_SIZE_PERLEVEL (((HWC_FILTER_PHASE * 8) + 0xf) & 0xfffffff0)

#define HWC_HORIZON_CHM_PARA_SIZE_PERLEVEL (((HWC_FILTER_PHASE * 12) + 0xf) & 0xfffffff0)
#define HWC_VERTICAL_CHM_PARA_SIZE_PERLEVEL (((HWC_FILTER_PHASE * 8) + 0xf) & 0xfffffff0)

#define HWC_HORIZON_PARA_SIZE_PERLEVEL (HWC_HORIZON_LUM_PARA_SIZE_PERLEVEL)
#define HWC_VERTICAL_PARA_SIZE_PERLEVEL (HWC_VERTICAL_LUM_PARA_SIZE_PERLEVEL)

#define HWC_HORIZON_PARA_SIZE_TOTAL (HWC_FILTER_LEVELS * HWC_HORIZON_PARA_SIZE_PERLEVEL)
#define HWC_VERTICAL_PARA_SIZE_TOTAL (HWC_FILTER_LEVELS * HWC_VERTICAL_PARA_SIZE_PERLEVEL)

/*水平滤波系数地址*/
static HI_U32 gs_u32HFilterParamAddr = 0;

/*垂直2阶滤波系数地址*/
static HI_U32 gs_u32VTap2FilterParamAddr = 0;

/*垂直4阶滤波系数地址*/
static HI_U32 gs_u32VTap4FilterParamAddr = 0;

static volatile HWC_REG_S *gs_pstHWCReg = HI_NULL;

static HI_S32 InitFilterParam(HI_VOID);/*加载滤波系数*/
static HI_VOID DeinitFilterParam(HI_VOID);
static inline HI_VOID InitCrgReg(HI_VOID);/*初始化crg寄存器*/
static inline HI_VOID DeinitCrgReg(HI_VOID);/*去初始化crg寄存器*/
static inline HI_S32 InitDevReg(HI_VOID);/*初始化设备寄存器*/
static inline HI_VOID DeinitDevReg(HI_VOID);/*去初始化设备寄存器*/
static HI_S32 CheckCompose(const HI_GFX2D_DEV_ID_E enDevId,
                           HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                           HI_GFX2D_SURFACE_S *pstDstSurface);/*compose参数检查*/
static inline HI_VOID InitNode(HWC_NODE_S *pstNode); /*初始化任务节点*/
static HI_VOID SetSrc(HWC_NODE_S *pstNode, HI_GFX2D_COMPOSE_S *pstCompose,
                      HI_U32 u32Zorder, HWC_LAYER_ID enLayerId);
static inline HI_VOID SetDst(HWC_NODE_S *pstNode, HI_GFX2D_SURFACE_S *pstDstSurface);
static inline HI_VOID SetInCsc(HWC_NODE_S *pstNode);/*设置输入CSC*/
static inline HI_VOID SetOutCsc(HWC_NODE_S *pstNode);/*设置输出CSC*/
static inline HI_VOID SetBgColor(HWC_NODE_S *pstNode, HI_U32 u32BgColor);
static HI_S32 SelectLayerId(HI_GFX2D_COMPOSE_LIST_S *pstComposeList, HWC_LAYER_ID *penLayerId);
static HI_VOID AdjustCompose(const HI_GFX2D_DEV_ID_E enDevId,
                             HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                             HI_GFX2D_SURFACE_S *pstDstSurface);
HI_S32 GFX2D_HAL_HWC_LinkNode(HI_VOID *pCurNode, HI_VOID *pNextNode);

static HWC_CAPABILITY_S gs_stCapability = {0};

const HI_U32 g_u32NodeSize = ((sizeof(HWC_NODE_S) + 0xf) & 0xffffff0);

HI_S32 GFX2D_HAL_HWC_Init(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32NodeNum;

    s32Ret = InitFilterParam();
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    u32NodeNum = GFX2D_CONFIG_GetNodeNum();

    s32Ret = GFX2D_MEM_Register(g_u32NodeSize, u32NodeNum);
    if (HI_SUCCESS != s32Ret)
    {
        DeinitFilterParam();
        return s32Ret;
    }

    HWC_ADP_GetCapability(&gs_stCapability);

    return HI_SUCCESS;
}

HI_S32 GFX2D_HAL_HWC_Deinit(HI_VOID)
{
    GFX2D_Mem_UnRegister(GFX2D_CONFIG_NODE_SIZE);

    DeinitFilterParam();

    return HI_SUCCESS;
}

HI_S32 GFX2D_HAL_HWC_Open(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = InitDevReg();
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 GFX2D_HAL_HWC_Close(HI_VOID)
{
    DeinitDevReg();

    return HI_SUCCESS;
}

#define GFX2D_HAL_HWC_MAX(a, b) (((a) > (b))?(a):(b))
#define GFX2D_HAL_HWC_MIN(a, b) (((a) < (b))?(a):(b))

/*data_in :positive or negetive  ;  bit:complement width */
HI_U32 GFX2D_HAL_HWC_TrueValueToComplement(HI_S32 data_in, HI_U32 bit)
{
    HI_U32 data_out ;
    HI_U32 data_in_tmp ;

    if (data_in >> (bit - 1))
    {
        data_in_tmp = 0 - data_in ;
        data_out = ((1 << (bit - 1)) | (((~data_in_tmp) & ((1 << (bit - 1)) - 1)) + 1)) & ((1 << bit) - 1) ;
    }
    else
    {
        data_out = data_in ;
    }

    return data_out ;
}

/*data_in :positive or negetive  ;  bit:complement width*/
HI_S32 GFX2D_HAL_HWC_ComplementToTrueValue(HI_U32 data_in, HI_U32 bit)
{
    HI_S32 data_out ;
    HI_U32 data_in_tmp ;

    if (data_in >> (bit - 1))
    {
        data_in_tmp = data_in & ((1 << (bit - 1)) - 1);
        data_out = (((~data_in_tmp) & ((1 << (bit - 1)) - 1)) + 1) * (-1) ;
    }
    else
    {
        data_out = data_in ;
    }

    return data_out ;
}

static HWC_NODE_S *GFX2D_HAL_HWC_DuplicateNodes(HWC_NODE_S *pstNode, HI_U32 u32NodeNum)
{
    HI_U32 i;
    HWC_NODE_S *pstCurNode = HI_NULL;
    HWC_NODE_S *pstNodeHead = HI_NULL;
    HWC_NODE_S *pstTmpNode = HI_NULL;

    for (i = 0; i < u32NodeNum; i++)
    {
        pstCurNode = (HWC_NODE_S *)GFX2D_MEM_Alloc(sizeof(HWC_NODE_S));
        if (pstCurNode == NULL)
        {
            HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "No mem!\n");
            goto ERR;
        }

        HI_GFX_Memcpy(pstCurNode, pstNode, sizeof(HWC_NODE_S));

        pstCurNode->pNextNode = pstNodeHead;
        if (pstNodeHead != NULL)
        {
            pstCurNode->u32NextNodePhyaddr = GFX2D_MEM_GetPhyaddr(pstNodeHead);
        }
        else
        {
            pstCurNode->u32NextNodePhyaddr = 0;
        }
        pstNodeHead = pstCurNode;
    }

    return pstNodeHead;

ERR:
    pstCurNode = pstNodeHead;
    while (pstCurNode)
    {
        pstTmpNode = pstCurNode->pNextNode;
        GFX2D_MEM_Free(pstCurNode);
        pstCurNode = pstTmpNode;
    }

    return HI_NULL;
}

#define GFX2D_ZME_TAP 8

typedef struct
{
    HI_U8 h_scan_ord;
    HI_U8 fmt;
    HI_U32 width;
    HI_U32 hoffset_pix;
    HI_U32 v_pro;

    HI_BOOL hlmsc_en;
    HI_BOOL hchmsc_en;
    HI_U32 hratio;
    HI_S32 hor_loffset;
    HI_S32 hor_coffset;

    HI_U32 zme_ow;
    HI_U32 zme_iw;

    HI_BOOL hpzme_en;
    HI_U8 hpzme_mode;
    HI_U32 hpzme_width;

    HI_U32 xfpos;
    HI_U32 xdpos;

    HI_U32 xst_pos_cord;
    HI_U32 xed_pos_cord;

    HI_U32 hor_loffset_cfg_int;
    HI_U32 hor_loffset_cfg_fraction;
    HI_U32 hor_loffset_pix_fraction;
    HI_U32 hor_loffset_fraction;
    HI_S32 xst_pos_cord_in_offset;
    HI_S32 xed_pos_cord_in_offset;
    HI_U32 xst_pos_cord_in_tap_rgb;
    HI_U32 xed_pos_cord_in_tap_rgb;
    HI_U32 node_cfg_zme_iw_rgb;
    HI_U32 node_cfg_hor_loffset_rgb;

    HI_U32 xst_pos_cord_in_tap_luma;
    HI_U32 xed_pos_cord_in_tap_luma;
    HI_U32 xst_pos_cord_in_chroma;
    HI_U32 xed_pos_cord_in_chroma;
    HI_U32 hor_coffset_cfg_int;
    HI_U32 hor_coffset_cfg_fraction;
    HI_U32 hor_coffset_pix_fraction;
    HI_U32 hor_coffset_fraction;
    HI_S32 xst_pos_cord_in_offset_chroma;
    HI_S32 xed_pos_cord_in_offset_chroma;
    HI_U32 xst_pos_cord_in_tap_chroma;
    HI_U32 xed_pos_cord_in_tap_chroma;
    HI_U32 xst_pos_cord_in_tap_sp;
    HI_U32 xed_pos_cord_in_tap_sp;
    HI_U32 node_cfg_zme_iw_sp;
    HI_U32 node_cfg_hor_loffset_sp;
    HI_U32 node_cfg_hor_coffset_sp;

    HI_U32 u32InWidth;
    HI_U32 u32ZmeInWidth;
    HI_U32 u32ZmeOutWidth;
    HI_U32 u32HpZmeWidth;

    HI_U32 xst_pos_cord_in_tap;
    HI_U32 xed_pos_cord_in_tap;

    HI_U32 xst_pos_cord_in_tap_hso;
    HI_U32 xed_pos_cord_in_tap_hso;

    HI_U32 hor_loffset_int_beyond_complent;
}GFX2D_HAL_BLOCKCONFIG_S;

static HI_VOID GFX2D_HAl_HWC_GetNodeConfig(HWC_NODE_S *pstNode, HI_U8 u8LayerId, GFX2D_HAL_BLOCKCONFIG_S *pstConfig)
{
    pstConfig->h_scan_ord = pstNode->astLayer[u8LayerId].unCtrl.stBits.bit1HScanOrd;
    pstConfig->fmt = pstNode->astLayer[u8LayerId].unCtrl.stBits.bit6Fmt;
    pstConfig->width = pstNode->astLayer[u8LayerId].unRes.stBits.bit16Width + 1;
    pstConfig->hoffset_pix = pstNode->astLayer[u8LayerId].u32PixOffset;
    pstConfig->v_pro = pstNode->astLayer[u8LayerId].unCtrl.stBits.bit1422Pro;

    if (u8LayerId == 0)
    {
        pstConfig->hlmsc_en = pstNode->unHsp.stBits.bit1IsLumEnable;
        pstConfig->hchmsc_en = pstNode->unHsp.stBits.bit1IsChromaEnable;
        pstConfig->hratio = pstNode->unHsp.stBits.bit24Ratio;
        pstConfig->hor_loffset = pstNode->unHLumOffset.stBits.bit28Offset;
        pstConfig->hor_coffset = pstNode->unHChromaOffset.stBits.bit28Offset;

        pstConfig->zme_ow = pstNode->unZmeOutRes.stBits.bit16Width + 1;
        pstConfig->zme_iw = pstNode->unZmeInRes.stBits.bit16Width + 1;

        pstConfig->hpzme_en = pstNode->unHpZme.stBits.bit1ZmehDwsm;
        pstConfig->hpzme_mode = pstNode->unHpZme.stBits.bit4ZmehDwsMode;
        pstConfig->hpzme_width = pstNode->unHpZmeReso.stBits.bit16Width + 1;
    }
    else if (u8LayerId == 1)
    {
        pstConfig->hlmsc_en = pstNode->unVhdHsp.stBits.bit1IsLumEnable;
        pstConfig->hchmsc_en = pstNode->unVhdHsp.stBits.bit1IsChromaEnable;
        pstConfig->hratio = pstNode->unVhdHsp.stBits.bit24Ratio;
        pstConfig->hor_loffset = pstNode->unVhdLumHOffset.stBits.bit28Offset;
        pstConfig->hor_coffset = pstNode->unVhdChromaHOffset.stBits.bit28Offset;

        pstConfig->zme_ow = pstNode->unVhdZmeOutRes.stBits.bit16Width + 1;
        pstConfig->zme_iw = pstNode->unVhdZmeInRes.stBits.bit16Width + 1;

        pstConfig->hpzme_en = pstNode->unVhdHpZme.stBits.bit1ZmehDwsm;
        pstConfig->hpzme_mode = pstNode->unVhdHpZme.stBits.bit4ZmehDwsMode;
        pstConfig->hpzme_width = pstNode->unVhdHpZmeReso.stBits.bit16Width + 1;
    }
    else
    {
        pstConfig->hlmsc_en = HI_FALSE;
        pstConfig->hchmsc_en = HI_FALSE;
        pstConfig->hratio = 0;
        pstConfig->hor_loffset = 0;
        pstConfig->hor_coffset = 0;

        pstConfig->zme_ow = pstNode->astLayer[u8LayerId].unRes.stBits.bit16Width + 1;
        pstConfig->zme_iw = pstNode->astLayer[u8LayerId].unRes.stBits.bit16Width + 1;

        pstConfig->hpzme_en = HI_FALSE;
        pstConfig->hpzme_mode = 0;
        pstConfig->hpzme_width = pstNode->astLayer[u8LayerId].unRes.stBits.bit16Width + 1;
    }


    pstConfig->xfpos = pstNode->astCmbInfo[u8LayerId].unCbmStartPos.stBits.bit16X;
    pstConfig->xdpos = (pstConfig->hlmsc_en | pstConfig->hchmsc_en) ? (pstConfig->xfpos + pstConfig->zme_ow - 1) : (pstConfig->hpzme_en ?
                      (pstConfig->xfpos + pstConfig->hpzme_width - 1) : (pstConfig->xfpos + pstConfig->width - 1));

    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "%d, %d\n",
                         pstConfig->xfpos,
                         pstConfig->xdpos);

    return;
}

static HI_VOID GFX2D_HAl_HWC_CalcSpZme(HI_U8 u8LayerId, GFX2D_HAL_BLOCKCONFIG_S *pstConfig, HWC_NODE_S *pstSplitNode)
{
    pstConfig->xst_pos_cord_in_tap_luma = ((pstConfig->xst_pos_cord_in_tap_rgb % 2) == 1) ?  pstConfig->xst_pos_cord_in_tap_rgb - 1 :
                                             pstConfig->xst_pos_cord_in_tap_rgb;
    pstConfig->xed_pos_cord_in_tap_luma = ((pstConfig->xed_pos_cord_in_tap_rgb % 2) == 0) ?  pstConfig->xed_pos_cord_in_tap_rgb + 1 :
                                             pstConfig->xed_pos_cord_in_tap_rgb;

    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "%d, %d\n", pstConfig->xst_pos_cord_in_tap_luma, pstConfig->xed_pos_cord_in_tap_luma);

    pstConfig->xst_pos_cord_in_chroma = (pstConfig->fmt == 50) ? ((pstConfig->xst_pos_cord * pstConfig->hratio) >> 20) : ((pstConfig->xst_pos_cord * (pstConfig->hratio / 2)) >> 20);
    pstConfig->xed_pos_cord_in_chroma = (pstConfig->fmt == 50) ? ((pstConfig->xed_pos_cord * pstConfig->hratio) >> 20) : ((pstConfig->xed_pos_cord * (pstConfig->hratio / 2)) >> 20);

    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "%d, %d\n", pstConfig->xst_pos_cord_in_chroma, pstConfig->xed_pos_cord_in_chroma);

    pstConfig->hor_coffset_cfg_int = GFX2D_HAL_HWC_ComplementToTrueValue((pstConfig->hor_coffset >> 20), 8);

    pstConfig->hor_coffset_cfg_fraction = pstConfig->hor_coffset & 0xfffff;
    pstConfig->hor_coffset_pix_fraction = (pstConfig->fmt == 50) ? ((pstConfig->xst_pos_cord * pstConfig->hratio) & 0xfffff) : ((pstConfig->xst_pos_cord * (pstConfig->hratio / 2)) & 0xfffff);
    pstConfig->hor_coffset_fraction = (pstConfig->hor_coffset_cfg_fraction +  pstConfig->hor_coffset_pix_fraction) & 0xfffff;

    pstConfig->xst_pos_cord_in_offset_chroma = pstConfig->xst_pos_cord_in_chroma + pstConfig->hor_coffset_cfg_int + (((pstConfig->hor_coffset_cfg_fraction +
                                                  pstConfig->hor_coffset_pix_fraction) & 0xfff00000) != 0);
    pstConfig->xed_pos_cord_in_offset_chroma = pstConfig->xed_pos_cord_in_chroma + pstConfig->hor_coffset_cfg_int + (((pstConfig->hor_coffset_cfg_fraction +
                                                  pstConfig->hor_coffset_pix_fraction) & 0xfff00000) != 0);

    pstConfig->xst_pos_cord_in_tap_chroma = (pstConfig->xst_pos_cord_in_offset_chroma < 0) ? 0 : ((pstConfig->xst_pos_cord_in_offset_chroma >= (GFX2D_ZME_TAP / 2 - 1))  ?
                                               (pstConfig->xst_pos_cord_in_offset_chroma - (GFX2D_ZME_TAP / 2 - 1))  : 0);
    pstConfig->xed_pos_cord_in_tap_chroma = (pstConfig->xed_pos_cord_in_offset_chroma + GFX2D_ZME_TAP / 2) < 0 ? 0 : (((pstConfig->xed_pos_cord_in_offset_chroma + GFX2D_ZME_TAP / 2) >=
                                               ((pstConfig->fmt == 50) ?
                                               ((pstConfig->zme_iw - 1)) : ((pstConfig->zme_iw - 1) / 2))) ? ((pstConfig->fmt == 50) ? ((pstConfig->zme_iw - 1)) : ((pstConfig->zme_iw - 1) / 2))  :
                                               (pstConfig->xed_pos_cord_in_offset_chroma + GFX2D_ZME_TAP / 2)) ;

    pstConfig->xst_pos_cord_in_tap_chroma = (pstConfig->fmt == 50) ? pstConfig->xst_pos_cord_in_tap_chroma : pstConfig->xst_pos_cord_in_tap_chroma * 2;
    pstConfig->xed_pos_cord_in_tap_chroma = (pstConfig->fmt == 50) ? pstConfig->xed_pos_cord_in_tap_chroma : (pstConfig->xed_pos_cord_in_tap_chroma * 2 + 1);

    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "%d, %d\n", pstConfig->xst_pos_cord_in_tap_chroma, pstConfig->xed_pos_cord_in_tap_chroma);

    pstConfig->xst_pos_cord_in_tap_sp = GFX2D_HAL_HWC_MIN(pstConfig->xst_pos_cord_in_tap_luma, pstConfig->xst_pos_cord_in_tap_chroma);
    pstConfig->xed_pos_cord_in_tap_sp = GFX2D_HAL_HWC_MAX(pstConfig->xed_pos_cord_in_tap_luma, pstConfig->xed_pos_cord_in_tap_chroma);

    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "%d, %d\n", pstConfig->xst_pos_cord_in_tap_sp, pstConfig->xed_pos_cord_in_tap_sp);

    pstConfig->node_cfg_zme_iw_sp = pstConfig->xed_pos_cord_in_tap_sp - pstConfig->xst_pos_cord_in_tap_sp + 1;

    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "%d\n", pstConfig->node_cfg_zme_iw_sp);

    pstConfig->node_cfg_hor_loffset_sp = (GFX2D_HAL_HWC_TrueValueToComplement((pstConfig->xst_pos_cord_in_offset - pstConfig->xst_pos_cord_in_tap_sp), 8) << 20) +
                                             pstConfig->hor_loffset_fraction;
    pstConfig->node_cfg_hor_coffset_sp = (GFX2D_HAL_HWC_TrueValueToComplement((pstConfig->xst_pos_cord_in_offset_chroma - ((pstConfig->fmt == 50) ? pstConfig->xst_pos_cord_in_tap_sp :
                                             pstConfig->xst_pos_cord_in_tap_sp / 2)), 8) << 20) + pstConfig->hor_coffset_fraction;

    return;
}

static HI_VOID GFX2D_HAL_HWC_CalcZme(HI_U8 u8LayerId, GFX2D_HAL_BLOCKCONFIG_S *pstConfig, HWC_NODE_S *pstSplitNode)
{
    pstConfig->hor_loffset_cfg_int = GFX2D_HAL_HWC_ComplementToTrueValue((pstConfig->hor_loffset >> 20), 8);

    pstConfig->hor_loffset_cfg_fraction = pstConfig->hor_loffset & 0xfffff;
    pstConfig->hor_loffset_pix_fraction = (pstConfig->xst_pos_cord * pstConfig->hratio) & 0xfffff;
    pstConfig->hor_loffset_fraction = (pstConfig->hor_loffset_cfg_fraction +  pstConfig->hor_loffset_pix_fraction) & 0xfffff;

    pstConfig->xst_pos_cord_in_offset = ((pstConfig->xst_pos_cord * pstConfig->hratio)  >> 20) +  pstConfig->hor_loffset_cfg_int  +
                                        (((pstConfig->hor_loffset_cfg_fraction + pstConfig->hor_loffset_pix_fraction) & 0xfff00000) != 0);
    pstConfig->xed_pos_cord_in_offset = ((pstConfig->xed_pos_cord * pstConfig->hratio)  >> 20) +  pstConfig->hor_loffset_cfg_int  +
                                        (((pstConfig->hor_loffset_cfg_fraction + pstConfig->hor_loffset_pix_fraction) & 0xfff00000) != 0);

    pstConfig->xst_pos_cord_in_tap_rgb = (pstConfig->xst_pos_cord_in_offset < 0) ? 0 : ((pstConfig->xst_pos_cord_in_offset >= (GFX2D_ZME_TAP / 2 - 1))  ?
                                         (pstConfig->xst_pos_cord_in_offset - (GFX2D_ZME_TAP / 2 - 1))  : 0);
    pstConfig->xed_pos_cord_in_tap_rgb = (pstConfig->xed_pos_cord_in_offset + GFX2D_ZME_TAP / 2) < 0 ? 0 : (((pstConfig->xed_pos_cord_in_offset + GFX2D_ZME_TAP / 2) >=
                                          pstConfig->zme_iw - 1) ? pstConfig->zme_iw - 1  : (pstConfig->xed_pos_cord_in_offset + GFX2D_ZME_TAP / 2))  ;

    pstConfig->node_cfg_zme_iw_rgb = pstConfig->xed_pos_cord_in_tap_rgb - pstConfig->xst_pos_cord_in_tap_rgb + 1;

    pstConfig->node_cfg_hor_loffset_rgb = (GFX2D_HAL_HWC_TrueValueToComplement((pstConfig->xst_pos_cord_in_offset - pstConfig->xst_pos_cord_in_tap_rgb), 8) << 20) +
    pstConfig->hor_loffset_fraction;

    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "%d\n", pstConfig->node_cfg_zme_iw_rgb);

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  SP  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if (pstConfig->fmt >= 32)
    {
        GFX2D_HAl_HWC_CalcSpZme(u8LayerId, pstConfig, pstSplitNode);
    }

    if (u8LayerId == 0)
    {
        pstConfig->u32ZmeOutWidth = pstConfig->xed_pos_cord - pstConfig->xst_pos_cord + 1;
        pstConfig->u32ZmeInWidth = (((pstConfig->fmt >= 32) && (pstConfig->fmt != 48) && (!((pstConfig->fmt == 50) && !pstConfig->v_pro)) && (pstConfig->fmt != 52)) ? pstConfig->node_cfg_zme_iw_sp : ((pstConfig->hlmsc_en || pstConfig->hchmsc_en) ?
                                      pstConfig->node_cfg_zme_iw_rgb : pstConfig->u32ZmeOutWidth));
        pstSplitNode->unHLumOffset.stBits.bit28Offset = (((pstConfig->fmt >= 32) && (pstConfig->fmt != 48) && (!((pstConfig->fmt == 50) &&
                                                        !pstConfig->v_pro)) && (pstConfig->fmt != 52)) ? pstConfig->node_cfg_hor_loffset_sp : pstConfig->node_cfg_hor_loffset_rgb);
        pstSplitNode->unHChromaOffset.stBits.bit28Offset = (((pstConfig->fmt >= 32) && (pstConfig->fmt != 48) && (!((pstConfig->fmt == 50) &&
                                                           !pstConfig->v_pro)) && (pstConfig->fmt != 52)) ? pstConfig->node_cfg_hor_coffset_sp : pstConfig->node_cfg_hor_loffset_rgb);
        pstConfig->u32HpZmeWidth = pstConfig->u32ZmeInWidth;
    }
    else if (u8LayerId == 1)
    {
        pstConfig->u32ZmeOutWidth = pstConfig->xed_pos_cord - pstConfig->xst_pos_cord + 1;
        HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "%d, %d\n",
                             pstConfig->xed_pos_cord,
                             pstConfig->xst_pos_cord);
        pstConfig->u32ZmeInWidth = (((pstConfig->fmt >= 32) && (pstConfig->fmt != 48) && (!((pstConfig->fmt == 50) && !pstConfig->v_pro)) &&
                                      (pstConfig->fmt != 52)) ? pstConfig->node_cfg_zme_iw_sp : ((pstConfig->hlmsc_en || pstConfig->hchmsc_en) ? pstConfig->node_cfg_zme_iw_rgb :
                                      pstConfig->u32ZmeOutWidth));
        HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "%d, %d\n",
                             pstConfig->u32ZmeInWidth,
                             pstConfig->u32ZmeOutWidth);
        pstSplitNode->unVhdLumHOffset.stBits.bit28Offset = (((pstConfig->fmt >= 32) && (pstConfig->fmt != 48) && (!((pstConfig->fmt == 50) &&
                                                           !pstConfig->v_pro)) && (pstConfig->fmt != 52)) ? pstConfig->node_cfg_hor_loffset_sp : pstConfig->node_cfg_hor_loffset_rgb);
        pstSplitNode->unVhdChromaHOffset.stBits.bit28Offset = (((pstConfig->fmt >= 32) && (pstConfig->fmt != 48) && (!((pstConfig->fmt == 50) &&
                                                              !pstConfig->v_pro)) && (pstConfig->fmt != 52)) ? pstConfig->node_cfg_hor_coffset_sp : pstConfig->node_cfg_hor_loffset_rgb);
        pstConfig->u32HpZmeWidth = pstConfig->u32ZmeInWidth;
    }

    return;
}

static HI_VOID GFX2D_HAL_HWC_EvenWidth(HI_U8 u8LayerId, GFX2D_HAL_BLOCKCONFIG_S *pstConfig, HWC_NODE_S *pstSplitNode)
{
    if (pstConfig->h_scan_ord)
    {
        if (u8LayerId == 0)
        {
            pstConfig->u32ZmeInWidth += 1;
        }
        else if (u8LayerId == 1)
        {
            pstConfig->u32ZmeInWidth += 1;
        }
    }
    else
    {
        if (u8LayerId == 0)
        {
            pstConfig->u32ZmeInWidth += 1;
            pstSplitNode->unHLumOffset.stBits.bit28Offset += (pstConfig->hor_loffset_int_beyond_complent << 20);
            pstSplitNode->unHChromaOffset.stBits.bit28Offset += (pstConfig->hor_loffset_int_beyond_complent << 20) ;
        }
        else if (u8LayerId == 1)
        {
            pstConfig->u32ZmeInWidth += 1;
            pstSplitNode->unVhdLumHOffset.stBits.bit28Offset += (pstConfig->hor_loffset_int_beyond_complent << 20);
            pstSplitNode->unVhdChromaHOffset.stBits.bit28Offset += (pstConfig->hor_loffset_int_beyond_complent << 20) ;
        }
    }
    pstConfig->u32InWidth += 1;
    pstSplitNode->astLayer[u8LayerId].u32PixOffset -= 1 ;

    return;
}

static HI_BOOL CheckEvenWidth(GFX2D_HAL_BLOCKCONFIG_S *pstConfig)
{
    return ((pstConfig->hpzme_en == 0)
            && (pstConfig->u32InWidth % 2 == 1)
            && ((pstConfig->fmt == 48)
                || ((pstConfig->fmt == 50)
                    && !pstConfig->v_pro)
                || (pstConfig->fmt == 52))
            && (pstConfig->xed_pos_cord_in_tap_hso == pstConfig->u32InWidth - 1)
            && (pstConfig->u32InWidth % 2 == 0));
}

static HI_VOID GFX2D_HAL_HWC_CalcXStartPos(GFX2D_HAL_BLOCKCONFIG_S *pstConfig)
{
    if (pstConfig->hlmsc_en || pstConfig->hchmsc_en)
    {
        if ((pstConfig->fmt >= 32) && (pstConfig->fmt != 48) &&
            (!((pstConfig->fmt == 50) && !pstConfig->v_pro)) && (pstConfig->fmt != 52))
        {
            pstConfig->xst_pos_cord_in_tap = pstConfig->xst_pos_cord_in_tap_sp;
        }
        else
        {
            pstConfig->xst_pos_cord_in_tap = pstConfig->xst_pos_cord_in_tap_rgb;
        }
    }
    else
    {
        pstConfig->xst_pos_cord_in_tap = pstConfig->xst_pos_cord;
    }

    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "%d, %d, %d, %d\n" ,
                         pstConfig->xst_pos_cord_in_tap,
                         pstConfig->xst_pos_cord,
                         pstConfig->xst_pos_cord_in_tap_sp,
                         pstConfig->xst_pos_cord_in_tap_rgb);

    pstConfig->xst_pos_cord_in_tap = pstConfig->hpzme_en ? (pstConfig->xst_pos_cord_in_tap * (pstConfig->hpzme_mode + 1)) : pstConfig->xst_pos_cord_in_tap;

    return;
}

static HI_VOID GFX2D_HAL_HWC_CalcXEndPos(GFX2D_HAL_BLOCKCONFIG_S *pstConfig)
{
    if (pstConfig->hlmsc_en || pstConfig->hchmsc_en)
    {
        if ((pstConfig->fmt >= 32) && (pstConfig->fmt != 48) &&
            (!((pstConfig->fmt == 50) && !pstConfig->v_pro)) && (pstConfig->fmt != 52))
        {
            pstConfig->xed_pos_cord_in_tap = pstConfig->xed_pos_cord_in_tap_sp;
        }
        else
        {
            pstConfig->xed_pos_cord_in_tap = pstConfig->xed_pos_cord_in_tap_rgb;
        }
    }
    else
    {
        pstConfig->xed_pos_cord_in_tap = pstConfig->xed_pos_cord;
    }

    pstConfig->xed_pos_cord_in_tap = pstConfig->hpzme_en ? ((pstConfig->xed_pos_cord_in_tap + 1) * (pstConfig->hpzme_mode + 1) - 1) :
                                        pstConfig->xed_pos_cord_in_tap;

    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "%d, %d\n",
                         pstConfig->xst_pos_cord_in_tap,
                         pstConfig->hpzme_en);

    return;
}

static HI_VOID GFX2D_HAL_HWC_CalcResolution(HI_U8 u8LayerId, GFX2D_HAL_BLOCKCONFIG_S *pstConfig, HWC_NODE_S *pstSplitNode)
{
    GFX2D_HAL_HWC_CalcXStartPos(pstConfig);

    GFX2D_HAL_HWC_CalcXEndPos(pstConfig);

    if (pstConfig->h_scan_ord)
    {
        pstConfig->xst_pos_cord_in_tap_hso = pstConfig->width - 1 - pstConfig->xed_pos_cord_in_tap;
        pstConfig->xed_pos_cord_in_tap_hso = pstConfig->width - 1 - pstConfig->xst_pos_cord_in_tap;
    }
    else
    {
        pstConfig->xst_pos_cord_in_tap_hso = pstConfig->xst_pos_cord_in_tap;
        pstConfig->xed_pos_cord_in_tap_hso = pstConfig->xed_pos_cord_in_tap;
    }

    pstConfig->u32InWidth = (pstConfig->xed_pos_cord_in_tap_hso - pstConfig->xst_pos_cord_in_tap_hso + 1);
    pstSplitNode->astLayer[u8LayerId].u32PixOffset = pstConfig->xst_pos_cord_in_tap_hso;

    pstConfig->hor_loffset_int_beyond_complent = GFX2D_HAL_HWC_TrueValueToComplement(1, 8);

    if (CheckEvenWidth(pstConfig))
    {
        GFX2D_HAL_HWC_EvenWidth(u8LayerId, pstConfig, pstSplitNode);
    }

    if (pstConfig->hpzme_en)
    {
        if (u8LayerId == 0 || u8LayerId == 1)
        {
            pstConfig->u32InWidth = pstConfig->u32HpZmeWidth * (pstConfig->hpzme_mode + 1);
        }
        pstSplitNode->astLayer[u8LayerId].u32PixOffset = ((pstConfig->h_scan_ord ? pstConfig->xst_pos_cord_in_tap_hso : pstConfig->xst_pos_cord_in_tap)  * (pstConfig->hpzme_mode + 1)) +  pstConfig->hoffset_pix;
    }
    else
    {
        if (u8LayerId == 0 || u8LayerId == 1)
        {
            pstConfig->u32InWidth = pstConfig->u32HpZmeWidth;
        }

        pstSplitNode->astLayer[u8LayerId].u32PixOffset = (pstConfig->h_scan_ord ? pstConfig->xst_pos_cord_in_tap_hso : pstConfig->xst_pos_cord_in_tap) +  pstConfig->hoffset_pix;
    }

    if (u8LayerId == 0)
    {
        pstSplitNode->unZmeInRes.stBits.bit16Width = pstConfig->u32ZmeInWidth - 1;
        pstSplitNode->unZmeOutRes.stBits.bit16Width = pstConfig->u32ZmeOutWidth - 1;
        pstSplitNode->unHpZmeReso.stBits.bit16Width = pstConfig->u32HpZmeWidth - 1;
    }
    else if (u8LayerId == 1)
    {
        pstSplitNode->unVhdZmeInRes.stBits.bit16Width = pstConfig->u32ZmeInWidth - 1;
        pstSplitNode->unVhdZmeOutRes.stBits.bit16Width = pstConfig->u32ZmeOutWidth - 1;
        pstSplitNode->unVhdHpZmeReso.stBits.bit16Width = pstConfig->u32HpZmeWidth - 1;
    }
    pstSplitNode->astLayer[u8LayerId].unRes.stBits.bit16Width = pstConfig->u32InWidth - 1;

    return;
}

static HI_VOID GFX2D_HAL_HWC_ConfigSplitNodeSrc(HWC_NODE_S *pstNode, HWC_NODE_S *pstSplitNode, HI_U32 u32DstStartPos, HI_U32 u32DstEndPos)
{
    HI_U8 u8LayerId;

    GFX2D_HAL_BLOCKCONFIG_S stBlockConfig = {0};

    for (u8LayerId = 0; u8LayerId < sizeof(pstNode->astLayer) / sizeof(pstNode->astLayer[0]); u8LayerId++)
    {
        if (!pstNode->astLayer[u8LayerId].unCtrl.stBits.bit1IsEnable)
        {
            continue;
        }

        GFX2D_HAl_HWC_GetNodeConfig(pstNode, u8LayerId, &stBlockConfig);

        /* setp2 : judge src is or not in picture and if in calc src blk pos in big picture*/
        if ((stBlockConfig.xfpos >  u32DstEndPos) ||  (stBlockConfig.xdpos < u32DstStartPos))
        {
            pstSplitNode->astLayer[u8LayerId].unCtrl.stBits.bit1IsEnable = 0x0;
            continue;
        }

        /*setp3 : out pos relative to disp start*/
        stBlockConfig.xst_pos_cord = GFX2D_HAL_HWC_MAX(u32DstStartPos, stBlockConfig.xfpos) - stBlockConfig.xfpos;
        stBlockConfig.xed_pos_cord = GFX2D_HAL_HWC_MIN(u32DstEndPos, stBlockConfig.xdpos) - stBlockConfig.xfpos;
        pstSplitNode->astCmbInfo[u8LayerId].unCbmStartPos.stBits.bit16X = GFX2D_HAL_HWC_MAX(u32DstStartPos, stBlockConfig.xfpos) - u32DstStartPos;

        HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "%d, %d, %d, %d\n" ,
                             u32DstStartPos,
                             stBlockConfig.xfpos,
                             u32DstEndPos,
                             stBlockConfig.xdpos);

        /*setp4 : calc src zme parameter*/
        GFX2D_HAL_HWC_CalcZme(u8LayerId, &stBlockConfig, pstSplitNode);

        GFX2D_HAL_HWC_CalcResolution(u8LayerId, &stBlockConfig, pstSplitNode);
    }

    return;
}

HI_S32 GFX2D_HAL_HWC_SplitNode(HWC_NODE_S *pstNode)
{
    //HI_U8 u8LayerId;

    //HI_BOOL en;
    //HI_U8 h_scan_ord;
    //HI_U32 fmt;
    //HI_U32 width;
    //HI_U32 hoffset_pix;
    //HI_U32 v_pro;

    //HI_BOOL hlmsc_en;
    //HI_BOOL hchmsc_en;
    //HI_U32 hratio;
    //HI_S32 hor_loffset;
    //HI_S32 hor_coffset;

    //HI_U32 zme_ow;
    //HI_U32 zme_iw;

    //HI_BOOL hpzme_en;
    //HI_U8 hpzme_mode;
    //HI_U32 hpzme_width;

    //HI_U32 xfpos;
    //HI_U32 xdpos;

    HI_BOOL des_en = pstNode->unOutCtl.stBits.bit1Enable;
    HI_U8 des_h_scan_ord = pstNode->unOutCtl.stBits.bit1HScanOrder;
    HI_BOOL des_crop_en = pstNode->unOutAlpha.stBits.bit1CropEn;

    HI_U32 des_crop_mode = pstNode->unOutAlpha.stBits.bit1CropMode;
    HI_U32 des_width = pstNode->unOutRes.stBits.bit16Width + 1;
    HI_U32 des_crop_start_x = pstNode->unOutCropStart.stBits.bit16X;
    HI_U32 des_crop_end_x = pstNode->unOutCropEnd.stBits.bit16X;
    HI_U32 des_hoffset_pix = pstNode->u32OutPixOffset;

    HI_U32 node_num;
    HI_U32 i;

    //HI_S32 s32Ret = HI_SUCCESS;

    HWC_NODE_S *pstNodeHead = NULL;
    HWC_NODE_S *pstCurNode = pstNode;
//    HWC_NODE_S *pstTmpNode = NULL;

    HI_U32 des_xst_pos_blk;
    HI_U32 des_xed_pos_blk;
    //HI_U32 xst_pos_cord;
    //HI_U32 xed_pos_cord;
    //HI_U32 hor_loffset_cfg_int;
    //HI_U32 hor_loffset_cfg_fraction;
    //HI_U32 hor_loffset_pix_fraction;
    //HI_U32 hor_loffset_fraction;
    //HI_S32 xst_pos_cord_in_offset;
    //HI_S32 xed_pos_cord_in_offset;
    //HI_U32 xst_pos_cord_in_tap_rgb;
    //HI_U32 xed_pos_cord_in_tap_rgb;
    //HI_U32 xst_pos_cord_in_tap_sp = 0;
    //HI_U32 xed_pos_cord_in_tap_sp = 0;
    //HI_U32 xst_pos_cord_in_tap_luma;
    //HI_U32 xed_pos_cord_in_tap_luma;
    //HI_U32 xst_pos_cord_in_chroma;
    //HI_U32 xed_pos_cord_in_chroma;
    //HI_U32 hor_coffset_cfg_int;
    //HI_U32 hor_coffset_cfg_fraction;
    //HI_U32 hor_coffset_pix_fraction;
    //HI_U32 hor_coffset_fraction;
    //HI_S32 xst_pos_cord_in_offset_chroma;
    //HI_S32 xed_pos_cord_in_offset_chroma;
    //HI_U32 xst_pos_cord_in_tap_chroma;
    //HI_U32 xed_pos_cord_in_tap_chroma;
    //HI_U32 node_cfg_hor_loffset_rgb;
    //HI_U32 node_cfg_hor_loffset_sp = 0;
    //HI_U32 node_cfg_zme_iw_rgb;
    //HI_U32 node_cfg_zme_iw_sp = 0;
    //HI_U32 node_cfg_hor_coffset_sp = 0;
    //HI_U32 xst_pos_cord_in_tap_hso;
    //HI_U32 xed_pos_cord_in_tap_hso;
    //HI_U32 xst_pos_cord_in_tap;
    //HI_U32 xed_pos_cord_in_tap;
    //HI_U32 hor_loffset_int_beyond_complent;
    //HI_U32 tap = 8;

    //HI_U32 u32InWidth;
    //HI_U32 u32ZmeInWidth = 0;
    //HI_U32 u32ZmeOutWidth = 0;
    //HI_U32 u32HpZmeWidth = 0;

#if 1
    if (!pstNode->unHsp.stBits.bit1IsLumEnable
        && !pstNode->unHsp.stBits.bit1IsChromaEnable
        && !pstNode->unVhdHsp.stBits.bit1IsLumEnable
        && !pstNode->unVhdHsp.stBits.bit1IsChromaEnable
        && !pstNode->unOutCtl.stBits.bit1CmpEn)
    {
        return HI_SUCCESS;
    }
#endif

    node_num = des_width / 256 + ((des_width % 256) != 0);
    if (node_num == 0)
    {
        return HI_SUCCESS;
    }

    pstNodeHead = GFX2D_HAL_HWC_DuplicateNodes(pstNode, node_num);
    if (HI_NULL == pstNodeHead)
    {
        return HI_ERR_GFX2D_NO_MEM;
    }

    pstCurNode = pstNodeHead;

    for (i = 0; i < node_num; i++)
    {
        /*setp1 : calc blk pos*/
        des_xst_pos_blk = i * 256;
        des_xed_pos_blk = (((i + 1) * 256) <= des_width) ? (((i + 1) * 256) - 1) : (des_width - 1);

        GFX2D_HAL_HWC_ConfigSplitNodeSrc(pstNode, pstCurNode, des_xst_pos_blk, des_xed_pos_blk);

        //...................................des........................................................................................
        pstCurNode->unOutCtl.stBits.bit1Enable = des_crop_en ? (((des_crop_mode == 0) && ((des_crop_start_x > des_xed_pos_blk) || (des_crop_end_x < des_xst_pos_blk))) ? 0 : des_en) :  des_en;
        pstCurNode->unOutRes.stBits.bit16Width = des_xed_pos_blk - des_xst_pos_blk + 1 - 1;
        pstCurNode->u32OutPixOffset = (des_h_scan_ord ? (des_width - 1 - des_xed_pos_blk ) : des_xst_pos_blk) + des_hoffset_pix;
        pstCurNode->unOutAlpha.stBits.bit1CropEn = (!((des_crop_start_x >  des_xed_pos_blk) ||  (des_crop_end_x < des_xst_pos_blk))) && des_crop_en;
        pstCurNode->unOutCropStart.stBits.bit16X = pstCurNode->unOutAlpha.stBits.bit1CropEn ? (GFX2D_HAL_HWC_MAX(des_xst_pos_blk, des_crop_start_x) - des_xst_pos_blk) :
                0;
        pstCurNode->unOutCropEnd.stBits.bit16X = pstCurNode->unOutAlpha.stBits.bit1CropEn ? (GFX2D_HAL_HWC_MIN(des_xed_pos_blk, des_crop_end_x) - des_xst_pos_blk)
                : 0;

        pstCurNode = pstCurNode->pNextNode;
    }

    HI_GFX_Memcpy(pstNode, pstNodeHead, sizeof(HWC_NODE_S));

    GFX2D_MEM_Free(pstNodeHead);

    return HI_SUCCESS;

#if 0
ERR:
    pstCurNode = pstNodeHead;
    while (pstCurNode)
    {
        pstTmpNode = pstCurNode->pNextNode;
        GFX2D_MEM_Free(pstCurNode);
        pstCurNode = pstTmpNode;
    }

    return s32Ret;
#endif
}

static HI_U32 GFX2D_HAL_HWC_ConfigComposeList(HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                                                 HI_GFX2D_COMPOSE_LIST_S *pstNewComposeList,
                                                 HI_GFX2D_SURFACE_S *pstDstSurface)
{
    HI_U8 u8NodeCnt = 0;
    HI_U8 u8LayerCnt = 0;
    HI_U8 u8ZmeLayerCnt = 0;
    HI_U32 i = 0;
    HI_GFX2D_COMPOSE_S *pstCompose;
    HI_S32 s32Ret;
    HI_U8 u8StartLayer = 0;

    for (i = 0; i < pstComposeList->u32ComposeCnt; i++)
    {
        if ((u8NodeCnt + 1) > GFX2D_HAL_MAX_NODE)
        {
            s32Ret = HI_ERR_GFX2D_UNSUPPORT;
            goto ERR;
        }

        if (pstComposeList->pstCompose[i].stOpt.stResize.bResizeEnable
            && (pstComposeList->pstCompose[i].stSurface.enType ==
                HI_GFX2D_SURFACE_TYPE_MEM))
        {
            u8ZmeLayerCnt++;
        }

        if (u8ZmeLayerCnt <= gs_stCapability.u8ZmeLayerNum)
        {
            u8LayerCnt++;
        }

        if ((u8ZmeLayerCnt <= gs_stCapability.u8ZmeLayerNum)
            && (i != (pstComposeList->u32ComposeCnt - 1)))
        {
            continue;
        }

        pstCompose = HI_GFX_KZALLOC(HIGFX_GFX2D_ID, sizeof(HI_GFX2D_COMPOSE_S) * u8LayerCnt, GFP_KERNEL);
        if (NULL == pstCompose)
        {
            s32Ret = HI_ERR_GFX2D_NO_MEM;
            goto ERR;
        }

        if (u8NodeCnt == 0)
        {
            HI_GFX_Memcpy(pstCompose, pstComposeList->pstCompose, sizeof(HI_GFX2D_COMPOSE_S) * u8LayerCnt);
        }
        else
        {
            HI_GFX_Memcpy(&pstCompose[0].stSurface, pstDstSurface, sizeof(HI_GFX2D_SURFACE_S));
            pstCompose[0].stInRect.u32Width = pstDstSurface->u32Width;
            pstCompose[0].stInRect.u32Height = pstDstSurface->u32Height;
            pstCompose[0].stOutRect.u32Width = pstDstSurface->u32Width;
            pstCompose[0].stOutRect.u32Height = pstDstSurface->u32Height;

            HI_GFX_Memcpy(&pstCompose[1],
                   &(pstComposeList->pstCompose[u8StartLayer]), sizeof(HI_GFX2D_COMPOSE_S) * (u8LayerCnt - 1));
        }

        pstNewComposeList[u8NodeCnt].pstCompose = pstCompose;
        pstNewComposeList[u8NodeCnt].u32BgColor = pstComposeList->u32BgColor;
        pstNewComposeList[u8NodeCnt].u32ComposeCnt = u8LayerCnt;

        if ((u8ZmeLayerCnt > gs_stCapability.u8ZmeLayerNum) && (i > 0))
        {
            i--;
        }

        u8StartLayer = i + 1;

        u8LayerCnt = 1;

        u8ZmeLayerCnt = 0;

        u8NodeCnt++;
    }

    return u8NodeCnt;

ERR:
    for (i = 0; i < u8NodeCnt; i++)
    {
        if (pstNewComposeList[i].pstCompose != NULL)
        {
            HI_GFX_KFREE(HIGFX_GFX2D_ID, pstNewComposeList[i].pstCompose);
            pstNewComposeList[i].pstCompose = NULL;
        }
    }

    return 0;
}

static HI_S32 GFX2D_HAL_HWC_CreateNode(const HI_GFX2D_DEV_ID_E enDevId,
                                       HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                                       HI_GFX2D_SURFACE_S *pstDstSurface,
                                       HI_U8 u8NodeCnt,
                                       HWC_NODE_S **ppstNode)
{
    HI_U8 i, j;
    HI_S32 s32Ret;
    HWC_LAYER_ID aenLayerId[HWC_LAYER_G5] = {0};
    HWC_NODE_S *pstNode[GFX2D_HAL_MAX_NODE] = {0};

    for (i = 0; i < u8NodeCnt; i++)
    {
        s32Ret = CheckCompose(enDevId, &pstComposeList[i], pstDstSurface);
        if (HI_SUCCESS != s32Ret)
        {
            goto ERR;
        }

        AdjustCompose(enDevId, &pstComposeList[i], pstDstSurface);

        /*选择对应的层ID*/
        s32Ret = SelectLayerId(&pstComposeList[i], aenLayerId);
        if (s32Ret < 0)
        {
            goto ERR;
        }

        pstNode[i] = (HWC_NODE_S *)GFX2D_MEM_Alloc(sizeof(HWC_NODE_S));
        if (NULL == pstNode[i])
        {
            s32Ret = HI_ERR_GFX2D_NO_MEM;
            goto ERR;
        }

        InitNode(pstNode[i]);

        pstNode[i]->pFirstNode = pstNode[0];

        for (j = 0; j < pstComposeList[i].u32ComposeCnt; j++)
        {
            SetSrc(pstNode[i], &(pstComposeList[i].pstCompose[j]), j, aenLayerId[j]);
        }

        SetDst(pstNode[i], pstDstSurface);

        SetBgColor(pstNode[i], pstComposeList[i].u32BgColor);

#ifdef GFX2D_HISTOGRAM_SUPPORT
        if ((pstDstSurface->enFmt == HI_GFX2D_FMT_SEMIPLANAR420UV)
            && (pstDstSurface->u32HistogramPhyaddr != 0))
        {
            unsigned long VBaddr;
            unsigned long Outoffset;
            pstNode[i]->pu32Histogram = remap_mmb_2(pstDstSurface->u32HistogramPhyaddr, &VBaddr, &Outoffset);
            pstNode[i]->u32Offset = Outoffset;
        }
#endif

        s32Ret = GFX2D_HAL_HWC_SplitNode(pstNode[i]);
        if (s32Ret < 0)
        {
            goto ERR;
        }

        if (i > 0)
        {
#if 0
            pstNode[i - 1]->u32NextNodePhyaddr = GFX2D_MEM_GetPhyaddr(pstNode[i]);

            if (pstNode[i - 1]->u32NextNodePhyaddr == 0)
            {
                HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "%p\n", pstNode[i]);
            }
            pstNode[i - 1]->pNextNode = pstNode[i];
            pstNode[i - 1]->u32NextNodeId = pstNode[i]->u32NodeId;
#else
            s32Ret = GFX2D_HAL_HWC_LinkNode(pstNode[i - 1], pstNode[i]);
            if (s32Ret < 0)
            {
                goto ERR;
            }
#endif
        }
    }

    *ppstNode = pstNode[0];

    return HI_SUCCESS;

ERR:
    for (i = 0; i < u8NodeCnt; i++)
    {
        if (pstNode[i] != NULL)
        {
            GFX2D_MEM_Free(pstNode[i]);
        }
    }

    *ppstNode = HI_NULL;

    return s32Ret;
}

static HI_S32 GFX2D_HAL_HWC_CheckCompose(const HI_GFX2D_DEV_ID_E enDevId,
                             HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                             HI_GFX2D_SURFACE_S *pstDstSurface,
                             HI_VOID **ppNode, GFX2D_HAL_DEV_TYPE_E *penNodeType)
{
    if ((HI_NULL == pstComposeList) || (HI_NULL == pstComposeList->pstCompose)
        || (HI_NULL == pstDstSurface)
        || (HI_NULL == ppNode) || (HI_NULL == penNodeType))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Null ptr\n");
        return HI_ERR_GFX2D_NULL_PTR;
    }

    if ((0 == pstComposeList->u32ComposeCnt) || (pstComposeList->u32ComposeCnt > gs_stCapability.u8MaxLayerNum))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid compose cnt:%d\n", pstComposeList->u32ComposeCnt);
        return HI_ERR_GFX2D_INVALID_COMPOSECNT;
    }

    return HI_SUCCESS;
}

/*HWC设置compose操作*/
HI_S32 GFX2D_HAL_HWC_Compose(const HI_GFX2D_DEV_ID_E enDevId,
                             HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                             HI_GFX2D_SURFACE_S *pstDstSurface,
                             HI_VOID **ppNode, GFX2D_HAL_DEV_TYPE_E *penNodeType)
{
    HWC_NODE_S *pstNode = NULL;
    HI_U32 i = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8NodeCnt = 0;
    HI_GFX2D_COMPOSE_LIST_S stComposeList[GFX2D_HAL_MAX_NODE];

    s32Ret = GFX2D_HAL_HWC_CheckCompose(enDevId,
                             pstComposeList,
                             pstDstSurface,
                             ppNode, penNodeType);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    HI_GFX_Memset(stComposeList, 0, sizeof(HI_GFX2D_COMPOSE_LIST_S) * GFX2D_HAL_MAX_NODE);

    u8NodeCnt = GFX2D_HAL_HWC_ConfigComposeList(pstComposeList, stComposeList, pstDstSurface);
    if (0 == u8NodeCnt)
    {
        return HI_ERR_GFX2D_UNSUPPORT;
    }

    s32Ret = GFX2D_HAL_HWC_CreateNode(enDevId,
                                       stComposeList,
                                       pstDstSurface,
                                       u8NodeCnt,
                                       &pstNode);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR;
    }

    *ppNode = pstNode;
    *penNodeType = GFX2D_HAL_DEV_TYPE_HWC;

ERR:
    for (i = 0; i < u8NodeCnt; i++)
    {
        if (stComposeList[i].pstCompose != NULL)
        {
            HI_GFX_KFREE(HIGFX_GFX2D_ID, stComposeList[i].pstCompose);
        }
    }

    return s32Ret;
}

static inline HI_VOID InitSmmuCrgReg(HI_VOID)
{
#ifdef CONFIG_GFX_MMU_SUPPORT
    volatile U_PERI_CRG204 unTempValue;
    HI_U8 u8Cnt;

    unTempValue.u32 = g_pstRegCrg->PERI_CRG204.u32;

    /*enable clock*/
    unTempValue.bits.tde_smmu_cken = 0x1;

    g_pstRegCrg->PERI_CRG204.u32 = unTempValue.u32;

    for (u8Cnt = 0; u8Cnt < 10; u8Cnt++)
    {
        if (g_pstRegCrg->PERI_CRG204.u32 == unTempValue.u32)
        {
            break;
        }
    }

    /*cancel reset*/
    unTempValue.bits.tde_smmu_srst_req = 0x0;

    g_pstRegCrg->PERI_CRG204.u32 = unTempValue.u32;

    for (u8Cnt = 0; u8Cnt < 10; u8Cnt++)
    {
        if (g_pstRegCrg->PERI_CRG204.u32 == unTempValue.u32)
        {
            break;
        }
    }

    mb();
#endif
    return;
}

static inline HI_VOID DeinitSmmuCrgReg(HI_VOID)
{
#ifdef CONFIG_GFX_MMU_SUPPORT
    U_PERI_CRG204 unTempValue;

    unTempValue.u32 = g_pstRegCrg->PERI_CRG204.u32;

    /*disable clock*/
    unTempValue.bits.tde_smmu_cken = 0x0;

    /*reset*/
    unTempValue.bits.tde_smmu_srst_req = 0x1;

#endif
    return;
}

static inline HI_VOID InitCrgReg(HI_VOID)
{

#ifdef CONFIG_GFX_MMU_SUPPORT
    HI_U32 u32BaseAddr = 0;
#endif

    volatile U_PERI_CRG37 unTempValue;
    HI_U8 u8Cnt;

    InitSmmuCrgReg();

    unTempValue.u32 = g_pstRegCrg->PERI_CRG37.u32;

    /*enable clock*/
    unTempValue.bits.tde_cken = 0x1;

    g_pstRegCrg->PERI_CRG37.u32 = unTempValue.u32;

    for (u8Cnt = 0; u8Cnt < 10; u8Cnt++)
    {
        if (g_pstRegCrg->PERI_CRG37.u32 == unTempValue.u32)
        {
            break;
        }
    }

    /*cancel reset*/
    unTempValue.bits.tde_srst_req = 0x0;

    g_pstRegCrg->PERI_CRG37.u32 = unTempValue.u32;

    for (u8Cnt = 0; u8Cnt < 10; u8Cnt++)
    {
        if (g_pstRegCrg->PERI_CRG37.u32 == unTempValue.u32)
        {
            break;
        }
    }

    mb();

    if (gs_pstHWCReg != NULL)
    {
        gs_pstHWCReg->u32MiscEllaneous = 0x300647f;
    }

#ifdef CONFIG_GFX_MMU_SUPPORT
    u32BaseAddr = GFX2D_HAL_GetBaseAddr(HI_GFX2D_DEV_ID_0, GFX2D_HAL_DEV_TYPE_HWC);
    HI_GFX_MapSmmuReg(u32BaseAddr + 0xf000);
    HI_GFX_InitSmmu(u32BaseAddr + 0xf000);
#endif

    return;
}

static inline HI_VOID DeinitCrgReg(HI_VOID)
{
    U_PERI_CRG37 unTempValue;

    DeinitSmmuCrgReg();

    unTempValue.u32 = g_pstRegCrg->PERI_CRG37.u32;

    /*reset*/
    unTempValue.bits.tde_srst_req = 0x1;

    //g_pstRegCrg->PERI_CRG37.u32 = unTempValue.u32;

    /*disable clock*/
    unTempValue.bits.tde_cken = 0x0;

    //g_pstRegCrg->PERI_CRG37.u32 = unTempValue.u32;

    mb();

    return;
}

static inline HI_S32 InitDevReg(HI_VOID)
{
    /*map register*/
    if (HI_NULL == gs_pstHWCReg)
    {
        gs_pstHWCReg = HI_GFX_REG_MAP((HWC_REG_BASE_ADDR + 0x4fc), sizeof(HWC_REG_S));
        if (HI_NULL == gs_pstHWCReg)
        {
            HI_GFX_COMM_LOG_FATAL(HIGFX_GFX2D_ID, "Map reg failed!\n");
            return HI_ERR_GFX2D_SYS;
        }
    }

    return HI_SUCCESS;
}

static inline HI_VOID DeinitDevReg(HI_VOID)
{
    /*unmap register*/
    if (HI_NULL != gs_pstHWCReg)
    {
        HI_GFX_REG_UNMAP(gs_pstHWCReg);
        gs_pstHWCReg = HI_NULL;
    }

    return;
}

static HI_S32 InitFilterParam(HI_VOID)
{
    HI_VOID *pVir;
    HI_U32 *pu32HorizonPara;
    HI_U32 *pu32VerticalPara;
    HI_U32 u32FiterMemSize;
    HI_BOOL bMmu = HI_FALSE;

    u32FiterMemSize = sizeof(gs_as32HorizonCoef) + sizeof(gs_as32VerticalCoef);
    gs_u32HFilterParamAddr = HI_GFX_AllocMem("GFX2D_FilterParam", NULL, u32FiterMemSize, &bMmu);
    if (0 == gs_u32HFilterParamAddr)
    {
        return HI_ERR_GFX2D_NO_MEM;
    }

    gs_u32VTap4FilterParamAddr = gs_u32HFilterParamAddr + sizeof(gs_as32HorizonCoef);

    pVir = HI_GFX_Map(gs_u32HFilterParamAddr, 0, bMmu);
    if (NULL == pVir)
    {
        HI_GFX_FreeMem(gs_u32HFilterParamAddr, bMmu);
        gs_u32HFilterParamAddr = 0;
        return HI_ERR_GFX2D_NO_MEM;
    }

    pu32HorizonPara = gs_as32HorizonCoef;
    pu32VerticalPara = gs_as32VerticalCoef;

    HI_GFX_Memcpy(pVir, gs_as32HorizonCoef, sizeof(gs_as32HorizonCoef));

    HI_GFX_Memcpy(pVir + sizeof(gs_as32HorizonCoef), gs_as32VerticalCoef, sizeof(gs_as32VerticalCoef));

    HI_GFX_Unmap(pVir, bMmu);

    return HI_SUCCESS;
}

static HI_VOID DeinitFilterParam(HI_VOID)
{
    HI_GFX_FreeMem(gs_u32HFilterParamAddr, HI_FALSE);
    gs_u32HFilterParamAddr = 0;
    gs_u32VTap2FilterParamAddr = 0;
    gs_u32VTap4FilterParamAddr = 0;

    return;
}

static inline HI_VOID InitNode(HWC_NODE_S *pstNode)
{
    HI_GFX_Memset(pstNode, 0, sizeof(HWC_NODE_S));

    /*Eanble list complete intterupt and err interrupts*/
    pstNode->unIntMask.stBits.bit1ListCompMask = 0x1;

    pstNode->unIntMask.stBits.bit1TimeoutMask = 0x1;
    pstNode->unIntMask.stBits.bit1BusErrMask = 0x1;

    pstNode->unCbmMix.stBits.bit1En = 0x1;

    return;
}

/*Map table of graphic layer fmt and HI_GFX2D_FMT*/
static HI_U32 gs_au32GLayerFmt[HI_GFX2D_FMT_PLANAR444 + 1] =
{
    0, 0, 0, 0, 0x8, 0, 0x5, 0x50005, 0x1, 0x50001, 0, 0, 0, 0, 0x3, 0, 0, 0, 0, 0, 0, 0, 0, 0x50000, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*Map table of video layer fmt and HI_GFX2D_FMT*/
static HI_U32 gs_au32VHDLayerFmt[HI_GFX2D_FMT_PLANAR444 + 1] =
{
    0, 0, 0, 0, 0x8, 0, 0x5, 0x50005, 0x1, 0x50001, 0, 0, 0, 0, 0x3, 0, 0, 0, 0, 0, 0, 0, 0, 0x50000, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0x23, 0x22, 0x21, 0x24, 0x20, 0x25, 0, 0x33, 0x33, 0x31, 0x31, 0x32, 0x32, 0, 0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0
};

/*Map table of out layer fmt and HI_GFX2D_FMT*/
static HI_U32 gs_au32DstLayerFmt[HI_GFX2D_FMT_PLANAR444 + 1] =
{
    0, 0, 0, 0, 0, 0, 0x5, 0x50005, 0, 0, 0, 0, 0, 0, 0x3, 0, 0, 0, 0, 0, 0, 0, 0, 0x50000, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x33, 0x33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static inline HI_VOID SetInCsc(HWC_NODE_S *pstNode)
{
    pstNode->unVhdCscIdc.stBits.bit1IsEnable = 0x1;
    pstNode->unVhdCscIdc.stBits.bit9Idc0 = -128;
    pstNode->unVhdCscIdc.stBits.bit9Idc1 = -128;
    pstNode->unVhdCscIdc.stBits.bit9Idc2 = -16;
    pstNode->unVhdCscOdc.stBits.bit9Odc0 = 0;
    pstNode->unVhdCscOdc.stBits.bit9Odc1 = 0;
    pstNode->unVhdCscOdc.stBits.bit9Odc2 = 0;

    pstNode->unVhdCscParam0.stBits.bit15Param0 = 297 << 2;
    pstNode->unVhdCscParam0.stBits.bit15Param1 = 0 << 2;
    pstNode->unVhdCscParam1.stBits.bit15Param0 = 408 << 2;
    pstNode->unVhdCscParam1.stBits.bit15Param1 = 297 << 2;
    pstNode->unVhdCscParam2.stBits.bit15Param0 = -100 << 2;
    pstNode->unVhdCscParam2.stBits.bit15Param1 = -208 << 2;
    pstNode->unVhdCscParam3.stBits.bit15Param0 = 297 << 2;
    pstNode->unVhdCscParam3.stBits.bit15Param1 = 516 << 2;
    pstNode->unVhdCscParam4.stBits.bit15Param0 = 0 << 2;

    pstNode->unCscMux.stBits.bit1CscMode = 0x1;

    return;
}

static inline HI_VOID SetOutCsc(HWC_NODE_S *pstNode)
{
    pstNode->unOutCscIdc.stBits.bit1IsEnable = 0x1;
    pstNode->unOutCscIdc.stBits.bit9Idc0 = 0;
    pstNode->unOutCscIdc.stBits.bit9Idc1 = 0;
    pstNode->unOutCscIdc.stBits.bit9Idc2 = 0;

    pstNode->unOutCscOdc.stBits.bit9Odc0 = 128;
    pstNode->unOutCscOdc.stBits.bit9Odc1 = 128;
    pstNode->unOutCscOdc.stBits.bit9Odc2 = 16;

    pstNode->unOutCscParam0.stBits.bit15Param0 = 66 << 2;
    pstNode->unOutCscParam0.stBits.bit15Param1 = 129 << 2;
    pstNode->unOutCscParam1.stBits.bit15Param0 = 25 << 2;
    pstNode->unOutCscParam1.stBits.bit15Param1 = -38 << 2;
    pstNode->unOutCscParam2.stBits.bit15Param0 = -74 << 2;
    pstNode->unOutCscParam2.stBits.bit15Param1 = 112 << 2;
    pstNode->unOutCscParam3.stBits.bit15Param0 = 112 << 2;
    pstNode->unOutCscParam3.stBits.bit15Param1 = -94 << 2;
    pstNode->unOutCscParam4.stBits.bit15Param0 = -18 << 2;

    return;
}


static inline HI_VOID SetDst(HWC_NODE_S *pstNode, HI_GFX2D_SURFACE_S *pstDstSurface)
{
    pstNode->unOutCtl.stBits.bit1Enable = 0x1;

    pstNode->unOutCtl.stBits.bit6Fmt = gs_au32DstLayerFmt[pstDstSurface->enFmt] & 0xffff;
    pstNode->unOutCtl.stBits.bit5ArgbOrder = (gs_au32DstLayerFmt[pstDstSurface->enFmt] >> 16);
    pstNode->unOutCtl.stBits.bit1Dither = 0x0;

    pstNode->unOutAlpha.stBits.bit8AlphaThreshold = 0x80;

#ifdef CONFIG_GFX_MMU_SUPPORT
    pstNode->unOutCtl.stBits.bit1LumMmuBypass = 0x0;
    pstNode->unOutCtl.stBits.bit1ChormeMmuBypass = 0x0;
    pstNode->unOutCtl.stBits.bit1ARMmuBypass = 0x0;
    pstNode->unOutCtl.stBits.bit1GBMmuBypass = 0x0;
#else
    pstNode->unOutCtl.stBits.bit1LumMmuBypass = 0x1;
    pstNode->unOutCtl.stBits.bit1ChormeMmuBypass = 0x1;
    pstNode->unOutCtl.stBits.bit1ARMmuBypass = 0x1;
    pstNode->unOutCtl.stBits.bit1GBMmuBypass = 0x1;
#endif

    pstNode->u32OutPhyaddr = pstDstSurface->u32Phyaddr[0];
    pstNode->u32OutStride = pstDstSurface->u32Stride[0];

    pstNode->unOutRes.stBits.bit16Width = pstDstSurface->u32Width - 1;
    pstNode->unOutRes.stBits.bit16Height = pstDstSurface->u32Height - 1;

    if (pstDstSurface->enFmt >= HI_GFX2D_FMT_YUV888)
    {
        SetOutCsc(pstNode);
        pstNode->u32OutChromPhyaddr = pstDstSurface->u32Phyaddr[1];
        pstNode->u32OutChromStride = pstDstSurface->u32Stride[1];

        pstNode->unOutDswm.stBits.bit2HDswmMode = 0x1;
        pstNode->unOutDswm.stBits.bit1VDswmMode = 0x1;
    }

    if (pstDstSurface->stCompressInfo.enCompressType != HI_GFX2D_COMPRESS_NONE)
    {
        pstNode->unOutCtl.stBits.bit1CmpEn = 0x1;
        pstNode->unOutCtl.stBits.bit1CmpAddrChangeEn = 0x1;
#if 0
        pstNode->unCmpGlbInfo.stBits.bit1IsLossLess = 0x1;
        pstNode->unCmpGlbInfo.stBits.bit1IsAlphaLossLess = 0x1;
        pstNode->unCmpGlbInfo.stBits.bit3PartitionNum = 0x1;
        //pstNode->unCmpGlbInfo.stBits.bit1Mode = 0x1;
        pstNode->unCmpReso.stBits.bit13Width = pstDstSurface->u32Width - 1;
        pstNode->unCmpReso.stBits.bit13Height = pstDstSurface->u32Height - 1;
#endif
        /*todo*/
        pstNode->u32ARAddr = pstDstSurface->u32Phyaddr[0];
        pstNode->u32OutPhyaddr = pstDstSurface->u32Phyaddr[0] + ((16 * pstDstSurface->u32Height + 0xff) & 0xffffff00);
        pstNode->u32GBAddr = pstNode->u32OutPhyaddr + pstDstSurface->u32Stride[0] * pstDstSurface->u32Height;
        pstNode->u32OutChromPhyaddr = pstNode->u32GBAddr + ((16 * pstDstSurface->u32Height + 0xff) & 0xffffff00);
        pstNode->u32OutChromStride = pstDstSurface->u32Stride[0];
    }

    return;
}

static inline HI_U8 GetBpp(HI_GFX2D_FMT_E enFmt, HI_BOOL bYComponent)
{
    if (enFmt <= HI_GFX2D_FMT_BGR565)
    {
        return 2;
    }
    else if (enFmt <= HI_GFX2D_FMT_BGR888)
    {
        return 3;
    }
    else if (enFmt <= HI_GFX2D_FMT_KBGR888)
    {
        return 4;
    }
    else if (enFmt <= HI_GFX2D_FMT_BGRA1555)
    {
        return 2;
    }
    else if (enFmt <= HI_GFX2D_FMT_BGRA8565)
    {
        return 3;
    }
    else if (enFmt <= HI_GFX2D_FMT_BGRA8888)
    {
        return 4;
    }
    else if (enFmt <= HI_GFX2D_FMT_ACLUT88)
    {
        return 0;
    }
    else if (enFmt <= HI_GFX2D_FMT_YUV888)
    {
        return 3;
    }
    else if (enFmt <= HI_GFX2D_FMT_AYUV8888)
    {
        return 4;
    }
    else if (enFmt <= HI_GFX2D_FMT_VUYY422)
    {
        return 2;
    }
    else if (enFmt <= HI_GFX2D_FMT_SEMIPLANAR444VU)
    {
        if (bYComponent)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        return 0;
    }
}

static inline HI_U32 GetFilterParaLevel(HI_U32 u32FloatBitLen, HI_U32 u32Step)
{
    HI_U32 u32Level;

    /* 根据步长找到表的索引 */
    if ((0x1 << u32FloatBitLen) > u32Step)
    {
        u32Level = 0;
    }
    else if ((0x1 << u32FloatBitLen) == u32Step)
    {
        u32Level = 1;
    }
    else if (((0x4 << u32FloatBitLen) / 0x3) >= u32Step)
    {
        u32Level = 2;
    }
    else if ((0x2 << u32FloatBitLen) >= u32Step)
    {
        u32Level = 3;
    }
    else if ((0x3 << u32FloatBitLen) >= u32Step)
    {
        u32Level = 4;
    }
    else if ((0x4 << u32FloatBitLen) >= u32Step)
    {
        u32Level = 5;
    }
    else
    {
        u32Level = 6;
    }

    return u32Level;
}

static inline HI_VOID SetZmeOrder(HI_GFX2D_FMT_E enFmt, HI_U32 u32OutWidth, HWC_VHDHSP_U *punHsp)
{
#if 0
    if (enFmt <= HI_GFX2D_FMT_BGRA8888)
    {
        if (u32OutWidth > 1920)
        {
            punHsp->stBits.bit1Order = 0x1;
        }
        else
        {
            punHsp->stBits.bit1Order = 0x0;
        }
    }
    else
    {
        if (u32OutWidth > 960)
        {
            punHsp->stBits.bit1Order = 0x1;
        }
        else
        {
            punHsp->stBits.bit1Order = 0x0;
        }
    }
#else
    punHsp->stBits.bit1Order = 0x0;
#endif

    return;
}

static inline HI_U32 GetZmeInputFmt(HI_GFX2D_FMT_E enFmt)
{
    if ((HI_GFX2D_FMT_SEMIPLANAR422UV_H == enFmt)
    || (HI_GFX2D_FMT_SEMIPLANAR422VU_H == enFmt)
    || ((HI_GFX2D_FMT_YUYV422 <= enFmt)
        && (enFmt <= HI_GFX2D_FMT_VUYY422))
    || (HI_GFX2D_FMT_PLANAR400 == enFmt)
    || (HI_GFX2D_FMT_PLANAR411 == enFmt)
    || (HI_GFX2D_FMT_PLANAR422H == enFmt)
    || (HI_GFX2D_FMT_PLANAR444 == enFmt))
    {
        return 0x0;
    }
    else if ((HI_GFX2D_FMT_SEMIPLANAR420UV == enFmt)
         || (HI_GFX2D_FMT_SEMIPLANAR420VU == enFmt)
         || (HI_GFX2D_FMT_PLANAR420 == enFmt)
         || (HI_GFX2D_FMT_SEMIPLANAR422UV_V == enFmt)
         || (HI_GFX2D_FMT_SEMIPLANAR422VU_V == enFmt)
         || (HI_GFX2D_FMT_PLANAR410 == enFmt)
         || (HI_GFX2D_FMT_PLANAR420 == enFmt)
         || (HI_GFX2D_FMT_PLANAR422V == enFmt))
    {
        return 0x1;
    }
    else
    {
        return 0x3;
    }
}

static inline HI_VOID SetVhdLayerZmeCoefAddr(HWC_NODE_S *pstNode, HI_U32 u32HStep, HI_U32 u32VStep, HI_U32 u32ChromaTap)
{
    pstNode->u32HCoefPhyaddr =  gs_u32HFilterParamAddr
                            + GetFilterParaLevel(GFX2D_HAL_HWC_HSTEP_FLOATLEN, u32HStep) * HWC_HORIZON_PARA_SIZE_PERLEVEL;
    pstNode->u32HChromaCoefPhyaddr = pstNode->u32HCoefPhyaddr;

    if (0x0 == u32ChromaTap)
    {
        pstNode->u32VCoefPhyaddr = gs_u32VTap4FilterParamAddr
                                   + GetFilterParaLevel(GFX2D_HAL_HWC_VSTEP_FLOATLEN, u32VStep) * HWC_VERTICAL_PARA_SIZE_PERLEVEL;
        pstNode->u32VChromaCoefPhyaddr = pstNode->u32VCoefPhyaddr;
    }
    else
    {
        pstNode->u32VCoefPhyaddr = gs_u32VTap2FilterParamAddr
                                   + GetFilterParaLevel(GFX2D_HAL_HWC_VSTEP_FLOATLEN, u32VStep) * HWC_VERTICAL_PARA_SIZE_PERLEVEL;
        pstNode->u32VChromaCoefPhyaddr = pstNode->u32VCoefPhyaddr;
    }

    return;
}

static inline HI_VOID SetGfxLayerZmeCoefAddr(HWC_NODE_S *pstNode, HI_U32 u32HStep, HI_U32 u32VStep, HI_U32 u32ChromaTap)
{
    pstNode->u32G0HCoefPhyaddr =  gs_u32HFilterParamAddr
                                  + GetFilterParaLevel(GFX2D_HAL_HWC_HSTEP_FLOATLEN, u32HStep) * HWC_HORIZON_PARA_SIZE_PERLEVEL;
    pstNode->u32G0HChromaCoefPhyaddr = pstNode->u32G0HCoefPhyaddr;

    if (0x0 == u32ChromaTap)
    {
        pstNode->u32G0VCoefPhyaddr = gs_u32VTap4FilterParamAddr
                                     + GetFilterParaLevel(GFX2D_HAL_HWC_VSTEP_FLOATLEN, u32VStep) * HWC_VERTICAL_PARA_SIZE_PERLEVEL;
        pstNode->u32G0VChromaCoefPhyaddr = pstNode->u32G0VCoefPhyaddr;
    }
    else
    {
        pstNode->u32G0VCoefPhyaddr = gs_u32VTap2FilterParamAddr
                                     + GetFilterParaLevel(GFX2D_HAL_HWC_VSTEP_FLOATLEN, u32VStep) * HWC_VERTICAL_PARA_SIZE_PERLEVEL;
        pstNode->u32G0VChromaCoefPhyaddr = pstNode->u32G0VCoefPhyaddr;
    }

    return;
}

typedef struct
{
    HWC_SRCCTL_U unCtrl;
    HWC_RES_U unRes;
    HWC_VHDHSP_U unVhdHsp;
    HWC_VHDHOFFSET_U unVhdLumHOffset;
    HWC_VHDHOFFSET_U unVhdChromaHOffset;
    HWC_VHDVSP_U unVhdVsp;
    HWC_RES_U unVhdZmeOutRes;
    HWC_RES_U unVhdZmeInRes;
    HWC_HPZME_U unHpZme;
    HI_U32 u32Vsr;
    HI_BOOL bFilter;
}GFX2D_HAL_ZME_S;

static HI_VOID GetZmeInfo(HWC_NODE_S *pstNode, HWC_LAYER_ID enLayerId, GFX2D_HAL_ZME_S *pstZme)
{
    if (enLayerId == HWC_LAYER_VHD)
    {
        pstZme->unCtrl.u32All = pstNode->astLayer[1].unCtrl.u32All;
        pstZme->unRes.u32All = pstNode->astLayer[1].unRes.u32All;
        pstZme->unVhdHsp.u32All = pstNode->unVhdHsp.u32All;
        pstZme->unVhdLumHOffset.u32All = pstNode->unVhdLumHOffset.u32All;
        pstZme->unVhdChromaHOffset.u32All = pstNode->unVhdChromaHOffset.u32All;
        pstZme->unVhdVsp.u32All = pstNode->unVhdVsp.u32All;
        pstZme->unVhdZmeOutRes.u32All = pstNode->unVhdZmeOutRes.u32All;
        pstZme->unVhdZmeInRes.u32All = pstNode->unVhdZmeInRes.u32All;
        pstZme->unHpZme.u32All = pstNode->unVhdHpZme.u32All;
    }
    else if (enLayerId >= HWC_LAYER_G0)
    {
        HWC_LAYER_S *pstLayer = &(pstNode->astLayer[(enLayerId == HWC_LAYER_G0) ? 0 : (enLayerId - 1)]);

        pstZme->unCtrl.u32All = pstLayer->unCtrl.u32All;
        pstZme->unRes.u32All = pstLayer->unRes.u32All;
        pstZme->unVhdHsp.u32All = pstNode->unHsp.u32All;
        pstZme->unVhdLumHOffset.u32All = pstNode->unHLumOffset.u32All;
        pstZme->unVhdChromaHOffset.u32All = pstNode->unHChromaOffset.u32All;
        pstZme->unVhdVsp.u32All = pstNode->unVsp.u32All;
        pstZme->unVhdZmeOutRes.u32All = pstNode->unZmeOutRes.u32All;
        pstZme->unVhdZmeInRes.u32All = pstNode->unZmeInRes.u32All;
        pstZme->unHpZme.u32All = pstNode->unHpZme.u32All;
    }
    return;
}

static HI_VOID SetZmeInfo(HWC_NODE_S *pstNode, HWC_LAYER_ID enLayerId, GFX2D_HAL_ZME_S *pstZme)
{
    if (enLayerId == HWC_LAYER_VHD)
    {
        pstNode->astLayer[1].unCtrl.u32All = pstZme->unCtrl.u32All;
        pstNode->astLayer[1].unRes.u32All = pstZme->unRes.u32All;
        pstNode->unVhdHsp.u32All = pstZme->unVhdHsp.u32All;
        pstNode->unVhdLumHOffset.u32All = pstZme->unVhdLumHOffset.u32All;
        pstNode->unVhdChromaHOffset.u32All = pstZme->unVhdChromaHOffset.u32All;
        pstNode->unVhdVsp.u32All = pstZme->unVhdVsp.u32All;
        pstNode->u32VhdVsr = pstZme->u32Vsr;
        pstNode->u32VhdVOffset = 0x0;
        pstNode->unVhdZmeOutRes.u32All = pstZme->unVhdZmeOutRes.u32All;
        pstNode->unVhdZmeInRes.u32All = pstZme->unVhdZmeInRes.u32All;
        pstNode->unVhdHpZme.u32All = pstZme->unHpZme.u32All;

        if (pstZme->bFilter)
        {
            SetVhdLayerZmeCoefAddr(pstNode, pstZme->unVhdHsp.stBits.bit24Ratio, pstZme->u32Vsr, pstZme->unVhdVsp.stBits.bit1ChromaTap);
        }
    }
    else if (enLayerId >= HWC_LAYER_G0)
    {
        HWC_LAYER_S *pstLayer = &(pstNode->astLayer[(enLayerId == HWC_LAYER_G0) ? 0 : (enLayerId - 1)]);
        pstLayer->unCtrl.u32All = pstZme->unCtrl.u32All;
        pstLayer->unRes.u32All = pstZme->unRes.u32All;
        pstNode->unHsp.u32All = pstZme->unVhdHsp.u32All;
        pstNode->unHLumOffset.u32All = pstZme->unVhdLumHOffset.u32All;
        pstNode->unHChromaOffset.u32All = pstZme->unVhdChromaHOffset.u32All;
        pstNode->unVsp.u32All = pstZme->unVhdVsp.u32All;
        pstNode->u32Vsr = pstZme->u32Vsr;
        pstNode->u32VOffset = 0x0;
        pstNode->unZmeOutRes.u32All = pstZme->unVhdZmeOutRes.u32All;
        pstNode->unZmeInRes.u32All = pstZme->unVhdZmeInRes.u32All;
        pstNode->unHpZme.u32All = pstZme->unHpZme.u32All;

        if (pstZme->bFilter)
        {
            SetGfxLayerZmeCoefAddr(pstNode, pstZme->unVhdHsp.stBits.bit24Ratio, pstZme->u32Vsr, pstZme->unVhdVsp.stBits.bit1ChromaTap);
        }
    }

}

static inline HI_VOID SetZme(HWC_NODE_S *pstNode, HI_GFX2D_COMPOSE_S *pstCompose, HWC_LAYER_ID enLayerId)
{
    HI_U32 u32HStep;
    HI_U32 u32VStep;
    HI_U32 u32InWidth, u32InHeight;
    HI_GFX2D_SURFACE_S *pstSurface = &(pstCompose->stSurface);
    HI_BOOL bFilter = HI_TRUE;
    HI_U32 HdwsmFactor = 0x1;
    GFX2D_HAL_ZME_S stZme;

    HI_GFX_Memset(&stZme,0x0,sizeof(GFX2D_HAL_ZME_S));
    /*缩放规则:
    1.视频层格式(plannar,semi-plannar,package 422)缩放开关必须打开.且
    不支持(输入分辨率>1920 && 输出分辨率>960)
    2.图形层格式缩放开关可根据需要打开或者关闭.不支持
    (输入分辨率>1920 && 输出分辨率> 1920)*/
    if ((pstSurface->enFmt <= HI_GFX2D_FMT_BGRA8888)
        && (!pstCompose->stOpt.stResize.bResizeEnable))
    {
        return;
    }

    GetZmeInfo(pstNode, enLayerId, &stZme);

    if (pstCompose->stOpt.stResize.bResizeEnable && (HI_GFX2D_FILTER_NONE == pstCompose->stOpt.stResize.stFilter.enFilterMode))
    {
        bFilter = HI_FALSE;
    }

    u32InWidth = pstCompose->stInRect.u32Width;
    u32InHeight = pstCompose->stInRect.u32Height;

    u32HStep = (u32InWidth << GFX2D_HAL_HWC_HSTEP_FLOATLEN)
               / pstCompose->stOutRect.u32Width;
    u32VStep = (u32InHeight << GFX2D_HAL_HWC_VSTEP_FLOATLEN)
               / pstCompose->stOutRect.u32Height;

    /*大于等于16倍缩放时，选择缩放前进行n抽1,n由缩放倍数确定;进行n抽1时，会影响图像
    质量:由于硬件约束，右侧可能会丢点*/
    /*视屏格式时，(输入分辨率>1920 && 输出分辨率>960)，先进行水平抽点*/
    /*图形格式时，(输入分辨率>1920 && 输出分辨率>1920)，先进行水平抽点*/
    if (u32HStep >= (16 << GFX2D_HAL_HWC_HSTEP_FLOATLEN))
    {
        HdwsmFactor = ((u32HStep >> GFX2D_HAL_HWC_HSTEP_FLOATLEN) / 16 + 1);
        u32InWidth = ((u32InWidth / HdwsmFactor) & ~(gs_stCapability.u8ZmeAlign - 1));
        u32HStep = (u32InWidth << GFX2D_HAL_HWC_HSTEP_FLOATLEN)
                   / pstCompose->stOutRect.u32Width;
        stZme.unHpZme.stBits.bit1ZmehDwsm = HI_TRUE;
        stZme.unHpZme.stBits.bit4ZmehDwsMode = HdwsmFactor - 1;
    }
#if 0
    else if (((pstSurface->enFmt <= HI_GFX2D_FMT_BGRA8888) && (u32InWidth > 1920) && (pstCompose->stOutRect.u32Width > 1920))
             || ((pstSurface->enFmt >= HI_GFX2D_FMT_YUV888) && (u32InWidth > 1920) && (pstCompose->stOutRect.u32Width > 960)))
    {
        u32InWidth = ((u32InWidth / 2) & ~(gs_stCapability.u8ZmeAlign - 1));
        u32HStep = (u32InWidth << GFX2D_HAL_HWC_HSTEP_FLOATLEN)
                   / pstCompose->stOutRect.u32Width;
        HdwsmFactor = 2;
        unHpZme.stBits.bit1ZmehDwsm = HI_TRUE;
        unHpZme.stBits.bit4ZmehDwsMode = 1;
    }
#endif
    stZme.unVhdHsp.stBits.bit1IsLumEnable = HI_TRUE;
    stZme.unVhdHsp.stBits.bit1IsChromaEnable = HI_TRUE;
#if HWC_VERSION <= 0x300
    stZme.unVhdHsp.stBits.bit1IsNonLinear = 0x0;
#endif
    stZme.unVhdHsp.stBits.bit1LumMode = bFilter;
    stZme.unVhdHsp.stBits.bit1ChrmoaMode = bFilter;
    stZme.unVhdHsp.stBits.bit24Ratio = u32HStep;
    stZme.unVhdHsp.stBits.bit1IsLumMid = (u32HStep < GFX2D_HAL_HWC_NOSCALE_HSTEP) ? 0x1 : 0x0;
    stZme.unVhdHsp.stBits.bit1IsChromaMid = (u32HStep < GFX2D_HAL_HWC_NOSCALE_HSTEP) ? 0x1 : 0x0;

    SetZmeOrder(pstSurface->enFmt, pstCompose->stOutRect.u32Width, &stZme.unVhdHsp);

    stZme.unVhdLumHOffset.stBits.bit28Offset = 0x0;
    stZme.unVhdChromaHOffset.stBits.bit28Offset = 0x0;

    stZme.unVhdVsp.stBits.bit1IsLumEnable = 0x1;
    stZme.unVhdVsp.stBits.bit1IsChromaEnable = 0x1;
    stZme.unVhdVsp.stBits.bit1IsLumMid = (u32VStep < GFX2D_HAL_HWC_NOSCALE_VSTEP) ? 0x1 : 0x0;
    stZme.unVhdVsp.stBits.bit1IsChromaMid = (u32VStep < GFX2D_HAL_HWC_NOSCALE_VSTEP) ? 0x1 : 0x0;
    stZme.unVhdVsp.stBits.bit1LumMode = bFilter;
    stZme.unVhdVsp.stBits.bit1ChromaMode = bFilter;
    stZme.unVhdVsp.stBits.bit2OutFmt = 0x2;

    stZme.unVhdVsp.stBits.bit2InFmt = GetZmeInputFmt(pstSurface->enFmt);

    stZme.unVhdZmeInRes.stBits.bit16Width = u32InWidth - 1;
    stZme.unVhdZmeInRes.stBits.bit16Height = u32InHeight - 1;
    stZme.unVhdZmeOutRes.stBits.bit16Width = pstCompose->stOutRect.u32Width - 1;
    stZme.unVhdZmeOutRes.stBits.bit16Height = pstCompose->stOutRect.u32Height - 1;
    stZme.unRes.stBits.bit16Width = u32InWidth * HdwsmFactor - 1;

    stZme.u32Vsr = u32VStep;
    stZme.bFilter = bFilter;

    SetZmeInfo(pstNode, enLayerId, &stZme);

    return;
}

static HI_VOID TraceCfg(HI_GFX2D_COMPOSE_LIST_S *pstComposeList)
{
    HI_U32 i;
    HI_GFX2D_COMPOSE_S *pstCompose;

    for (i = 0; i < pstComposeList->u32ComposeCnt; i++)
    {
        pstCompose = &pstComposeList->pstCompose[i];
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Layer:%d\n", i);
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Type:%d, bResize:%d", pstCompose->stSurface.enType, pstCompose->stOpt.stResize.bResizeEnable);
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Input w:%d, h:%d\n", pstCompose->stInRect.u32Width, pstCompose->stInRect.u32Height);
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Output w:%d, h:%d\n", pstCompose->stOutRect.u32Width, pstCompose->stOutRect.u32Height);
    }

    return;
}

static HI_BOOL CheckVhdFormat(HI_GFX2D_SURFACE_S *pstSurface)
{
    return ((HI_GFX2D_SURFACE_TYPE_MEM == pstSurface->enType)
            && (HI_GFX2D_FMT_YUV888 <= pstSurface->enFmt));
}

static HI_BOOL CheckResizeEnable(HI_GFX2D_SURFACE_S *pstSurface, HI_GFX2D_COMPOSE_OPT_S *pstOpt)
{
    return ((HI_GFX2D_SURFACE_TYPE_MEM == pstSurface->enType)
             && (pstOpt->stResize.bResizeEnable));
}

static HWC_LAYER_ID SelectZmeLayer(HI_U8 *pu8LayerRefRecord)
{
    /*图形层不支持缩放，则选择视频层*/
    if (!gs_stCapability.bGZmeSupport)
    {
        return HWC_LAYER_VHD;
    }
    else
    {
        if (pu8LayerRefRecord[HWC_LAYER_G0] == 0)
        {
            return HWC_LAYER_G0;
        }
        else if (pu8LayerRefRecord[HWC_LAYER_VHD] == 0)
        {
            return HWC_LAYER_VHD;
        }
        else
        {
            return HWC_LAYER_BUTT;
        }
    }
}

static HWC_LAYER_ID SelectLayer(HI_U8 *pu8LayerRefRecord)
{
    HI_U8 i;

    for (i = HWC_LAYER_G1; i <= HWC_LAYER_G5; i++)
    {
        if (pu8LayerRefRecord[i] == 0)
        {
            return i;
        }
    }

    if (i > HWC_LAYER_G5)
    {
        if (pu8LayerRefRecord[HWC_LAYER_G0] == 0)
        {
            return HWC_LAYER_G0;
        }
        else if (pu8LayerRefRecord[HWC_LAYER_VHD] == 0)
        {
            return HWC_LAYER_VHD;
        }
        else
        {
            return HWC_LAYER_BUTT;
        }
    }

    return HWC_LAYER_BUTT;
}


static HI_BOOL CheckSelectResultInvalid(HWC_LAYER_ID enLayerId, HI_GFX2D_COMPOSE_S *pstCompose)
{
    return ((enLayerId == HWC_LAYER_VHD)
                 && ((pstCompose->stInRect.u32Width < gs_stCapability.u32MinVWidth)
                     || (pstCompose->stInRect.u32Height < gs_stCapability.u32MinVHeight))
                 && (HI_GFX2D_SURFACE_TYPE_MEM == pstCompose->stSurface.enType)
                 && (HI_GFX2D_FMT_YUV888 <= pstCompose->stSurface.enFmt));
}


/*根据应用配置，选择合适的层,选择原则:
若像素格式为YUV格式，那么采用视频层VHD；
若需要缩放，优先采用视频层，然后G0层
否则，依次选择未使用的图形层:G1,G2,G3,G4,G5,G0*/
static HI_S32 SelectLayerId(HI_GFX2D_COMPOSE_LIST_S *pstComposeList, HWC_LAYER_ID *penLayerId)
{
    HI_S32 i, k = 0;
    HI_GFX2D_COMPOSE_S *pstCompose;
    HI_U8 au8Ref[HWC_LAYER_G5 + 1] = {0};

    for (i = 0; i < pstComposeList->u32ComposeCnt; i++)
    {
        pstCompose = &(pstComposeList->pstCompose[i]);

        /*YUV格式，必须走视频层*/
        if (CheckVhdFormat(&pstCompose->stSurface))
        {
            penLayerId[i] = HWC_LAYER_VHD;
        }
        /*缩放，优先选择视频层，然后图形层*/
        else if (CheckResizeEnable(&pstCompose->stSurface, &pstCompose->stOpt))
        {
            penLayerId[i] = SelectZmeLayer(au8Ref);
            if (HWC_LAYER_BUTT == penLayerId[i])
            {
                goto ERR;
            }
        }
        else
        {
            penLayerId[i] = SelectLayer(au8Ref);
            if (HWC_LAYER_BUTT == penLayerId[i])
            {
                goto ERR;
            }
        }

        if (au8Ref[penLayerId[i]] > 0)
        {
            goto ERR;
        }

        if (HI_TRUE == CheckSelectResultInvalid(penLayerId[i], pstCompose))
        {
            goto ERR;
        }

        if ((penLayerId[i] == HWC_LAYER_VHD)
             && (pstCompose->stSurface.enType == HI_GFX2D_SURFACE_TYPE_COLOR))
        {
            for (k = i - 1; k >= 0; k--)
            {
                if ((pstComposeList->pstCompose[k].stInRect.u32Width >= gs_stCapability.u32MinVWidth)
                    && (pstComposeList->pstCompose[k].stInRect.u32Height >= gs_stCapability.u32MinVHeight)
                    && (pstComposeList->pstCompose[k].stSurface.enType == HI_GFX2D_SURFACE_TYPE_MEM))
                {
                    penLayerId[i] = penLayerId[k];
                    penLayerId[k] = HWC_LAYER_VHD;
                    au8Ref[HWC_LAYER_VHD]++;
                    break;
                }
            }

            if (k < 0)
            {
                goto ERR;
            }
        }
        else
        {
            au8Ref[penLayerId[i]]++;
        }
    }

    return HI_SUCCESS;

ERR:
    HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "unsupport!n");
    TraceCfg(pstComposeList);
    return HI_ERR_GFX2D_UNSUPPORT;
}

static inline HI_VOID SetGLayer(HWC_LAYER_ID enLayerId,
                                HWC_NODE_S *pstNode,
                                HI_GFX2D_COMPOSE_S *pstCompose)
{
    HI_U32 u32Phyaddr = 0;
    HI_U32 u32Stride = 0;
    //HI_U32 u32SfPos = 0;
    HI_U32 u32DimValue = 0;
    HI_U8 u8Bpp;
    HI_GFX2D_SURFACE_S *pstSurface = &(pstCompose->stSurface);
    HWC_SRCCTL_U unCtrl;
    HWC_RES_U unRes;
    HWC_ALPHA_U unAlpha;
    HWC_POS_U unOutStartPos;
    HWC_CBMPARA_U unCbmPara;
    HWC_MMU_PROT_CTL_U unMmuCtl;
    HWC_LAYER_S *pstGLayer;
    HI_U32 u32Index;

    u32Index = (enLayerId > HWC_LAYER_G0) ? (enLayerId - 1) : 0;
    pstGLayer = &(pstNode->astLayer[u32Index]);

    unCtrl.u32All = pstGLayer->unCtrl.u32All;
    unRes.u32All = pstGLayer->unRes.u32All;
    unAlpha.u32All = pstGLayer->unAlpha.u32All;
    unOutStartPos.u32All = pstNode->astCmbInfo[u32Index].unCbmStartPos.u32All;
    unCbmPara.u32All = pstNode->astCmbInfo[u32Index].unCbmPara.u32All;
    unMmuCtl.u32All = pstNode->unMmuCtl.u32All;

    unCtrl.stBits.bit1IsEnable = 0x1;
    if (HI_GFX2D_SURFACE_TYPE_COLOR == pstSurface->enType)
    {
        unCtrl.stBits.bit1IsDim = 0x1;
        //unCtrl.stBits.bit6Fmt = gs_au32GLayerFmt[pstSurface->enFmt] & 0xffff;
        //unCtrl.stBits.bit5ArgbOrder = gs_au32GLayerFmt[pstSurface->enFmt] >> 16;
        u32DimValue = pstSurface->u32Color;
    }
    else
    {
        unCtrl.stBits.bit1IsDim = 0x0;
        unCtrl.stBits.bit6Fmt = gs_au32GLayerFmt[pstSurface->enFmt] & 0xffff;
        unCtrl.stBits.bit5ArgbOrder = gs_au32GLayerFmt[pstSurface->enFmt] >> 16;

        u8Bpp = GetBpp(pstSurface->enFmt, HI_FALSE);
        u32Phyaddr = pstSurface->u32Phyaddr[0]
                     + pstCompose->stInRect.s32XPos * u8Bpp
                     + pstCompose->stInRect.s32YPos * pstSurface->u32Stride[0];
        u32Stride = pstSurface->u32Stride[0];
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        if (3 == u8Bpp)
        {
            if (((u32Phyaddr % 16) % 3) == 0)
            {
                u32SfPos = (u32Phyaddr % 16) / 3;
            }
            else if (((u32Phyaddr % 16) % 3) == 1)
            {
                u32SfPos = (11 + (u32Phyaddr % 16) / 3);
            }
            else
            {
                u32SfPos = (6 + (u32Phyaddr % 16) / 3);
            }
        }
        else
        {
            u32SfPos = (u32Phyaddr % 16) / u8Bpp;
        }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    }

    unCbmPara.stBits.bit1bPreMul = pstSurface->bPremulti;
    if (pstSurface->stAlphaExt.bExtAlpha)
    {
        unAlpha.stBits.bit8Alpha0 = pstSurface->stAlphaExt.u8Alpha0;
        unAlpha.stBits.bit8Alpha1 = pstSurface->stAlphaExt.u8Alpha1;
    }
    else
    {
        unAlpha.stBits.bit8Alpha0 = 0x0;
        unAlpha.stBits.bit8Alpha1 = 0xff;
    }

    unCtrl.stBits.bit2ExpandMode = 0x0 ;

#ifdef CONFIG_GFX_MMU_SUPPORT
    unMmuCtl.u32All = 0x1fff0000;
#else
    unMmuCtl.u32All = 0x1fff03ff;
#endif

    //unCbmPara.stBits.bit1GlobalAlphaEnable = pstCompose->stOpt.stBlend.bGlobalAlphaEnable;
    unCbmPara.stBits.bit8GlobalAlpha = 0xff;
    if (pstCompose->stOpt.stBlend.bGlobalAlphaEnable)
    {
        unCbmPara.stBits.bit8GlobalAlpha = pstCompose->stOpt.stBlend.u8GlobalAlpha;
    }

    unCbmPara.stBits.bit1PixelAlphaEnable = pstCompose->stOpt.stBlend.bPixelAlphaEnable;
    if (!unCbmPara.stBits.bit1PixelAlphaEnable && unCtrl.stBits.bit6Fmt == 0x0)
    {
        unCtrl.stBits.bit6Fmt = 0x1;
    }

    unCbmPara.stBits.bit1bCovBlend = pstCompose->stOpt.stBlend.bCovBlend;
    if (unCbmPara.stBits.bit1bCovBlend)
    {
        unCbmPara.stBits.bit1PixelAlphaEnable = 0x1;
    }

    if (HI_GFX2D_SURFACE_TYPE_COLOR == pstSurface->enType)
    {
        unRes.stBits.bit16Width = pstCompose->stOutRect.u32Width - 1;
        unRes.stBits.bit16Height = pstCompose->stOutRect.u32Height - 1;
    }
    else
    {
        unRes.stBits.bit16Width = pstCompose->stInRect.u32Width - 1;
        unRes.stBits.bit16Height = pstCompose->stInRect.u32Height - 1;
    }
    unOutStartPos.stBits.bit16X = pstCompose->stOutRect.s32XPos;
    unOutStartPos.stBits.bit16Y = pstCompose->stOutRect.s32YPos;

    pstGLayer->unCtrl.u32All = unCtrl.u32All;
    pstGLayer->u32Phyaddr = u32Phyaddr;

    if (enLayerId == HWC_LAYER_G0)
    {
        pstGLayer->u32Stride = u32Stride;
    }
    else
    {
        pstGLayer->u32CbcrStride = u32Stride;
    }

    //pstGLayer->u32SfPos = u32SfPos;
    pstGLayer->u32DimValue = u32DimValue;
    pstGLayer->unRes.u32All = unRes.u32All;

    pstNode->astCmbInfo[u32Index].unCbmPara.u32All = unCbmPara.u32All;
    pstNode->astCmbInfo[u32Index].unCbmStartPos.u32All = unOutStartPos.u32All;

    //pstGLayer->unBmpPara.u32All = unCbmPara.u32All;
    //pstGLayer->unOutStartPos.u32All = unOutStartPos.u32All;

    pstGLayer->unAlpha.u32All = unAlpha.u32All;
    pstNode->unMmuCtl.u32All = unMmuCtl.u32All;

    if (HI_GFX2D_SURFACE_TYPE_COLOR != pstSurface->enType)
    {
        SetZme(pstNode, pstCompose, enLayerId);
    }

    return;
}

static inline HI_BOOL CheckReverseUVOrder(HI_GFX2D_FMT_E enFmt)
{
    return ((HI_GFX2D_FMT_SEMIPLANAR422VU_H == enFmt)
        || (HI_GFX2D_FMT_SEMIPLANAR422VU_V == enFmt)
        || (HI_GFX2D_FMT_SEMIPLANAR420VU == enFmt));
}

static inline HI_VOID GetSampleFactor(HI_GFX2D_FMT_E enFmt, HI_U8 *pu8HorizonSampleFactor,
                                      HI_U8 *pu8VerticalSampleFactor)
{
    if ((HI_GFX2D_FMT_PLANAR420 == enFmt)
        || (HI_GFX2D_FMT_SEMIPLANAR420UV == enFmt)
        || (HI_GFX2D_FMT_SEMIPLANAR420VU == enFmt))
    {
        *pu8HorizonSampleFactor = 2;
        *pu8VerticalSampleFactor = 2;
    }
    else if (HI_GFX2D_FMT_PLANAR444 == enFmt)
    {
        *pu8HorizonSampleFactor = 1;
        *pu8VerticalSampleFactor = 1;
    }
    else if (HI_GFX2D_FMT_PLANAR410 == enFmt)
    {
        *pu8HorizonSampleFactor = 4;
        *pu8VerticalSampleFactor = 4;
    }
    else if (HI_GFX2D_FMT_PLANAR411 == enFmt)
    {
        *pu8HorizonSampleFactor = 4;
        *pu8VerticalSampleFactor = 1;
    }
    else if ((HI_GFX2D_FMT_PLANAR422H == enFmt)
             || (HI_GFX2D_FMT_SEMIPLANAR422UV_H == enFmt)
             || (HI_GFX2D_FMT_SEMIPLANAR422VU_H == enFmt))
    {
        *pu8HorizonSampleFactor = 2;
        *pu8VerticalSampleFactor = 1;
    }
    else if ((HI_GFX2D_FMT_PLANAR422V == enFmt)
             || (HI_GFX2D_FMT_SEMIPLANAR422UV_V == enFmt)
             || (HI_GFX2D_FMT_SEMIPLANAR422VU_V == enFmt))
    {
        *pu8HorizonSampleFactor = 1;
        *pu8VerticalSampleFactor = 2;
    }
    else
    {
        *pu8HorizonSampleFactor = 1;
        *pu8VerticalSampleFactor = 1;
    }

    return;
}

static inline HI_VOID SetVHDLayer(HWC_LAYER_ID enLayerId,
                                  HWC_NODE_S *pstNode,
                                  HI_GFX2D_COMPOSE_S *pstCompose)
{
    HI_U8 u8Bpp;
    HI_U8 u8HorizonSampleFactor = 1;
    HI_U8 u8VerticalSampleFactor = 1;

    HI_GFX2D_SURFACE_S *pstSurface = &(pstCompose->stSurface);

    pstNode->astLayer[1].unCtrl.stBits.bit1IsEnable = HI_TRUE;
    pstNode->astLayer[1].unCtrl.stBits.bit2ExpandMode = 0x0;
    pstNode->astLayer[1].unCtrl.stBits.bit1422Pro = 0x1;

    if (CheckReverseUVOrder(pstSurface->enFmt))
    {
        pstNode->astLayer[1].unCtrl.stBits.bit1UVOrder = 0x1;
    }

    pstNode->astLayer[1].unCtrl.stBits.bit6Fmt = gs_au32VHDLayerFmt[pstSurface->enFmt];
    pstNode->astLayer[1].unCtrl.stBits.bit5ArgbOrder = gs_au32VHDLayerFmt[pstSurface->enFmt] >> 16;

#ifdef CONFIG_GFX_MMU_SUPPORT
    pstNode->unMmuCtl.u32All = 0x1fff0000;
#else
    pstNode->unMmuCtl.u32All = 0x1fff03ff;
#endif

    u8Bpp = GetBpp(pstSurface->enFmt, HI_TRUE);

    GetSampleFactor(pstSurface->enFmt, &u8HorizonSampleFactor, &u8VerticalSampleFactor);

    pstNode->astLayer[1].u32Phyaddr = pstSurface->u32Phyaddr[0]
                                      + pstCompose->stInRect.s32YPos * pstSurface->u32Stride[0]
                                      + pstCompose->stInRect.s32XPos * u8Bpp;

    u8Bpp = GetBpp(pstSurface->enFmt, HI_FALSE);



    if ((pstSurface->enFmt >= HI_GFX2D_FMT_SEMIPLANAR420UV)
        && (HI_GFX2D_FMT_PLANAR400 != pstSurface->enFmt))
    {
        pstNode->astLayer[1].u32CbcrPhyaddr = pstSurface->u32Phyaddr[1]
                                              + pstCompose->stInRect.s32YPos / u8VerticalSampleFactor * pstSurface->u32Stride[1]
                                              + pstCompose->stInRect.s32XPos / u8HorizonSampleFactor * u8Bpp;
    }

    pstNode->astLayer[1].u32Stride = pstSurface->u32Stride[0];
    pstNode->astLayer[1].u32CbcrStride = pstSurface->u32Stride[1];

#if 0
    /*Attention:limit*/
    if (pstSurface->enFmt < HI_GFX2D_FMT_SEMIPLANAR400)
    {
        pstNode->u32VhdCbPhyaddr = pstNode->u32VhdYPhyaddr;
        pstNode->u32VhdCrPhyaddr = pstNode->u32VhdYPhyaddr;
        pstNode->u32VhdYSride pstSurface->u32Stride[0]
        | (pstSurface->u32Stride[0] << 16);
        pstNode->u32VhdCrStride = pstSurface->u32Stride[0];
    }
#endif

    pstNode->astLayer[1].unRes.stBits.bit16Width = pstCompose->stInRect.u32Width - 1;
    pstNode->astLayer[1].unRes.stBits.bit16Height = pstCompose->stInRect.u32Height - 1;

    //pstNode->astCmbInfo[1].unCbmPara.stBits.bit1GlobalAlphaEnable = pstCompose->stOpt.stBlend.bGlobalAlphaEnable;
    pstNode->astCmbInfo[1].unCbmPara.stBits.bit8GlobalAlpha = 0xff;
    if (pstCompose->stOpt.stBlend.bGlobalAlphaEnable)
    {
        pstNode->astCmbInfo[1].unCbmPara.stBits.bit8GlobalAlpha = pstCompose->stOpt.stBlend.u8GlobalAlpha;
    }

    pstNode->astCmbInfo[1].unCbmPara.stBits.bit1bCovBlend = pstCompose->stOpt.stBlend.bCovBlend;
    pstNode->astCmbInfo[1].unCbmPara.stBits.bit1PixelAlphaEnable = pstCompose->stOpt.stBlend.bPixelAlphaEnable;
    pstNode->astCmbInfo[1].unCbmPara.stBits.bit1bPreMul = pstSurface->bPremulti;
    if (pstSurface->stAlphaExt.bExtAlpha)
    {
        pstNode->astLayer[1].unAlpha.stBits.bit8Alpha0 = pstSurface->stAlphaExt.u8Alpha0;
        pstNode->astLayer[1].unAlpha.stBits.bit8Alpha1 = pstSurface->stAlphaExt.u8Alpha1;
    }
    else
    {
        pstNode->astLayer[1].unAlpha.stBits.bit8Alpha0 = 0x0;
        pstNode->astLayer[1].unAlpha.stBits.bit8Alpha1 = 0xff;
    }

    pstNode->astCmbInfo[1].unCbmStartPos.stBits.bit16X = pstCompose->stOutRect.s32XPos;
    pstNode->astCmbInfo[1].unCbmStartPos.stBits.bit16Y = pstCompose->stOutRect.s32YPos;

    if (pstSurface->enFmt >= HI_GFX2D_FMT_YUYV422)
    {
        SetInCsc(pstNode);
    }
    SetZme(pstNode, pstCompose, enLayerId);

    return;
}

/*HWC_LAYER_ID到硬件层的转换表*/
HI_U32 au32LayerToHalTable[] = {0x0, 0x2, 0x1, 0x3, 0x4, 0x5, 0x6, 0x7};

static HI_VOID SetSrc(HWC_NODE_S *pstNode, HI_GFX2D_COMPOSE_S *pstCompose,
                      HI_U32 u32Zorder, HWC_LAYER_ID enLayerId)
{
    if (enLayerId >= HWC_LAYER_G0)
    {
        SetGLayer(enLayerId, pstNode, pstCompose);
    }
    else
    {
        SetVHDLayer(enLayerId, pstNode, pstCompose);
    }

    switch (u32Zorder)
    {
        case 0:
        {
            pstNode->unCbmMix.stBits.bit3Prio0 = au32LayerToHalTable[enLayerId];
            break;
        }
        case 1:
        {
            pstNode->unCbmMix.stBits.bit3Prio1 = au32LayerToHalTable[enLayerId];
            break;
        }
        case 2:
        {
            pstNode->unCbmMix.stBits.bit3Prio2 = au32LayerToHalTable[enLayerId];
            break;
        }
        case 3:
        {
            pstNode->unCbmMix.stBits.bit3Prio3 = au32LayerToHalTable[enLayerId];
            break;
        }
        case 4:
        {
            pstNode->unCbmMix.stBits.bit3Prio4 = au32LayerToHalTable[enLayerId];
            break;
        }
        case 5:
        {
            pstNode->unCbmMix.stBits.bit3Prio5 = au32LayerToHalTable[enLayerId];
            break;
        }
        case 6:
        {
            pstNode->unCbmMix.stBits.bit3Prio6 = au32LayerToHalTable[enLayerId];
            break;
        }
    }
    return;
}

static inline HI_VOID SetBgColor(HWC_NODE_S *pstNode, HI_U32 u32BgColor)
{
    pstNode->u32CbmBkgValue = u32BgColor;

    return;
}

HI_VOID PrintNodeInfo(HWC_NODE_S *pstNode)
{
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0CTRL\t\t0x%08x\n" ,
                         pstNode->astLayer[0].unCtrl);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0ADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[0].u32Phyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0STRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[0].u32Stride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0CADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[0].u32CbcrPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0CSTRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[0].u32CbcrStride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0IRESO\t\t0x%08x\n" ,
                         pstNode->astLayer[0].unRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0DIMPARA\t\t0x%08x\n" ,
                         pstNode->astLayer[0].u32DimValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0ALPHA\t\t0x%08x\n" ,
                         pstNode->astLayer[0].unAlpha.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0PIXOFFSET\t0x%08x\n" ,
                         pstNode->astLayer[0].u32PixOffset);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCTRL\t\t0x%08x\n" ,
                         pstNode->astLayer[1].unCtrl);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[1].u32Phyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDSTRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[1].u32Stride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[1].u32CbcrPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSTRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[1].u32CbcrStride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDIRESO\t\t0x%08x\n" ,
                         pstNode->astLayer[1].unRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDDIMPARA\t\t0x%08x\n" ,
                         pstNode->astLayer[1].u32DimValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDALPHA\t\t0x%08x\n" ,
                         pstNode->astLayer[1].unAlpha.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDPIXOFFSET\t0x%08x\n" ,
                         pstNode->astLayer[1].u32PixOffset);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1CTRL\t\t0x%08x\n" ,
                         pstNode->astLayer[2].unCtrl);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1ADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[2].u32Phyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1STRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[2].u32Stride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0CADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[2].u32CbcrPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1CSTRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[2].u32CbcrStride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1IRESO\t\t0x%08x\n" ,
                         pstNode->astLayer[2].unRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1DIMPARA\t\t0x%08x\n" ,
                         pstNode->astLayer[2].u32DimValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1ALPHA\t\t0x%08x\n" ,
                         pstNode->astLayer[2].unAlpha.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1PIXOFFSET\t0x%08x\n" ,
                         pstNode->astLayer[2].u32PixOffset);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2CTRL\t\t0x%08x\n" ,
                         pstNode->astLayer[3].unCtrl);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2ADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[3].u32Phyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2STRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[3].u32Stride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2CADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[3].u32CbcrPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2CSTRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[3].u32CbcrStride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2IRESO\t\t0x%08x\n" ,
                         pstNode->astLayer[3].unRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2DIMPARA\t\t0x%08x\n" ,
                         pstNode->astLayer[3].u32DimValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2ALPHA\t\t0x%08x\n" ,
                         pstNode->astLayer[3].unAlpha.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2PIXOFFSET\t0x%08x\n" ,
                         pstNode->astLayer[3].u32PixOffset);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3CTRL\t\t0x%08x\n" ,
                         pstNode->astLayer[4].unCtrl);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3ADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[4].u32Phyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3STRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[4].u32Stride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3CADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[4].u32CbcrPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3CSTRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[4].u32CbcrStride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3IRESO\t\t0x%08x\n" ,
                         pstNode->astLayer[4].unRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3DIMPARA\t\t0x%08x\n" ,
                         pstNode->astLayer[4].u32DimValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3ALPHA\t\t0x%08x\n" ,
                         pstNode->astLayer[4].unAlpha.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3PIXOFFSET\t0x%08x\n" ,
                         pstNode->astLayer[4].u32PixOffset);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4CTRL\t\t0x%08x\n" ,
                         pstNode->astLayer[5].unCtrl);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4ADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[5].u32Phyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4STRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[5].u32Stride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4CADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[5].u32CbcrPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4CSTRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[5].u32CbcrStride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4IRESO\t\t0x%08x\n" ,
                         pstNode->astLayer[5].unRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4DIMPARA\t\t0x%08x\n" ,
                         pstNode->astLayer[5].u32DimValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4ALPHA\t\t0x%08x\n" ,
                         pstNode->astLayer[5].unAlpha.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4PIXOFFSET\t0x%08x\n" ,
                         pstNode->astLayer[5].u32PixOffset);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5CTRL\t\t0x%08x\n" ,
                         pstNode->astLayer[6].unCtrl);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5ADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[6].u32Phyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5STRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[6].u32Stride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5CADDR\t\t0x%08x\n" ,
                         pstNode->astLayer[6].u32CbcrPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5CSTRIDE\t\t0x%08x\n" ,
                         pstNode->astLayer[6].u32CbcrStride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5IRESO\t\t0x%08x\n" ,
                         pstNode->astLayer[6].unRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5DIMPARA\t\t0x%08x\n" ,
                         pstNode->astLayer[6].u32DimValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5ALPHA\t\t0x%08x\n" ,
                         pstNode->astLayer[6].unAlpha.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5PIXOFFSET\t0x%08x\n" ,
                         pstNode->astLayer[6].u32PixOffset);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCTRL\t\t0x%08x\n" ,
                         pstNode->unOutCtl.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTADDR\t\t0x%08x\n" ,
                         pstNode->u32OutPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTADDR_C\t\t0x%08x\n" ,
                         pstNode->u32OutChromPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTSTRIDE\t\t0x%08x\n" ,
                         pstNode->u32OutStride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTSTRIDE_C\t0x%08x\n" ,
                         pstNode->u32OutChromStride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTRESO\t\t0x%08x\n" ,
                         pstNode->unOutRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTALPHA\t\t0x%08x\n" ,
                         pstNode->unOutAlpha.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCROPSTART\t0x%08x\n" ,
                         pstNode->unOutCropStart.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCROPEND\t\t0x%08x\n" ,
                         pstNode->unOutCropEnd.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_MMUCTRL\t\t0x%08x\n" ,
                         pstNode->unMmuCtl.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTPIXOFFSET\t0x%08x\n" ,
                         pstNode->u32OutPixOffset);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTARADDR\t\t0x%08x\n" ,
                         pstNode->u32ARAddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTGBADDR\t\t0x%08x\n" ,
                         pstNode->u32GBAddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0HSP\t\t0x%08x\n" ,
                         pstNode->unHsp.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0LHOFFSET\t\t0x%08x\n" ,
                         pstNode->unHLumOffset.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0CHOFFSET\t\t0x%08x\n" ,
                         pstNode->unHChromaOffset.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0VSP\t\t0x%08x\n" ,
                         pstNode->unVsp.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0VSR\t\t0x%08x\n" ,
                         pstNode->u32Vsr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0VOFFSET\t\t0x%08x\n" ,
                         pstNode->u32VOffset);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0ZMEOUTRES\t0x%08x\n" ,
                         pstNode->unZmeOutRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0ZMEINRES\t\t0x%08x\n" ,
                         pstNode->unZmeInRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0HPZME\t\t0x%08x\n" ,
                         pstNode->unHpZme.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0HPZMERES\t\t0x%08x\n" ,
                         pstNode->unHpZmeReso.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCIDC\t\t0x%08x\n" ,
                         pstNode->unVhdCscIdc.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCODC\t\t0x%08x\n" ,
                         pstNode->unVhdCscOdc.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCP0\t\t0x%08x\n" ,
                         pstNode->unVhdCscParam0.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCP1\t\t0x%08x\n" ,
                         pstNode->unVhdCscParam1.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCP2\t\t0x%08x\n" ,
                         pstNode->unVhdCscParam2.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCP3\t\t0x%08x\n" ,
                         pstNode->unVhdCscParam3.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCP4\t\t0x%08x\n" ,
                         pstNode->unVhdCscParam4.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDHSP\t\t0x%08x\n" ,
                         pstNode->unVhdHsp.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDLHOFFSET\t0x%08x\n" ,
                         pstNode->unVhdLumHOffset.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCHOFFSET\t0x%08x\n" ,
                         pstNode->unVhdChromaHOffset.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDVSP\t\t0x%08x\n" ,
                         pstNode->unVhdVsp.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDVSR\t\t0x%08x\n" ,
                         pstNode->u32VhdVsr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDVOFFSET\t\t0x%08x\n" ,
                         pstNode->u32VhdVOffset);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDZMEOUTRES\t0x%08x\n" ,
                         pstNode->unVhdZmeOutRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDZMEINRES\t0x%08x\n" ,
                         pstNode->unVhdZmeInRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDHPZME\t\t0x%08x\n" ,
                         pstNode->unVhdHpZme.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDHPZMERES\t0x%08x\n" ,
                         pstNode->unVhdHpZmeReso.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CSCMUX\t\t0x%08x\n" ,
                         pstNode->unCscMux.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCIDC\t\t0x%08x\n" ,
                         pstNode->unOutCscIdc.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCODC\t\t0x%08x\n" ,
                         pstNode->unOutCscOdc.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCP0\t\t0x%08x\n" ,
                         pstNode->unOutCscParam0.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCP1\t\t0x%08x\n" ,
                         pstNode->unOutCscParam1.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCP2\t\t0x%08x\n" ,
                         pstNode->unOutCscParam2.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCP3\t\t0x%08x\n" ,
                         pstNode->unOutCscParam3.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCP4\t\t0x%08x\n" ,
                         pstNode->unOutCscParam4.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTDSWM\t\t0x%08x\n" ,
                         pstNode->unOutDswm.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CBMMIX\t\t0x%08x\n" ,
                         pstNode->unCbmMix.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CBMBKGV\t\t0x%08x\n" ,
                         pstNode->u32CbmBkgValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0CBMPARA\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[0].unCbmPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0OUTPOS\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[0].unCbmStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCBMPARA\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[1].unCbmPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDOUTPOS\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[1].unCbmStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1CBMPARA\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[2].unCbmPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1OUTPOS\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[2].unCbmStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2CBMPARA\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[3].unCbmPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2OUTPOS\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[3].unCbmStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3CBMPARA\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[4].unCbmPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3OUTPOS\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[4].unCbmStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4CBMPARA\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[5].unCbmPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4OUTPOS\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[5].unCbmStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5CBMPARA\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[6].unCbmPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5OUTPOS\t\t0x%08x\n" ,
                         pstNode->astCmbInfo[6].unCbmStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPGLBINFO\t\t0x%08x\n" ,
                         pstNode->unCmpGlbInfo.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPRESO\t\t0x%08x\n" ,
                         pstNode->unCmpReso.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG0\t\t0x%08x\n" ,
                         pstNode->unCmpCfg0.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG1\t\t0x%08x\n" ,
                         pstNode->unCmpCfg1.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG2\t\t0x%08x\n" ,
                         pstNode->unCmpCfg2.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG3\t\t0x%08x\n" ,
                         pstNode->unCmpCfg3.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG4\t\t0x%08x\n" ,
                         pstNode->unCmpCfg4.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG5\t\t0x%08x\n" ,
                         pstNode->unCmpCfg5.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG6\t\t0x%08x\n" ,
                         pstNode->unCmpCfg6.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG7\t\t0x%08x\n" ,
                         pstNode->unCmpCfg7.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG8\t\t0x%08x\n" ,
                         pstNode->unCmpCfg8.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG9\t\t0x%08x\n" ,
                         pstNode->unCmpCfg9.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPMAXLOWREN\t0x%08x\n" ,
                         pstNode->u32CmpMaxRowLen);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "G0HCOEFADDR\t\t0x%08x\n" ,
                         pstNode->u32G0HCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "G0VCOEFADDR\t\t0x%08x\n" ,
                         pstNode->u32G0VCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "G0HCHROMACOEFADDR\t0x%08x\n" ,
                         pstNode->u32G0HChromaCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "G0VCHROMACOEFADDR\t0x%08x\n" ,
                         pstNode->u32G0VChromaCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDHCOEFADDR\t0x%08x\n" ,
                         pstNode->u32HCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDVCOEFADDR\t0x%08x\n" ,
                         pstNode->u32VCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDHCHROMACOEFADDR\t0x%08x\n" ,
                         pstNode->u32HChromaCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDVCHROMACOEFADDR\t0x%08x\n" ,
                         pstNode->u32VChromaCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_AXIID\t\t0x%08x\n" ,
                         pstNode->u32AXIID);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_NODEID\t\t0x%08x\n" ,
                         pstNode->u32NodeId);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_INTMASK\t\t0x%08x\n" ,
                         pstNode->unIntMask.u32All);

    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "\n");

    return;
}


HI_S32 GFX2D_HAL_HWC_SubNode(HI_GFX2D_DEV_ID_E enDevId, HI_VOID *pNode)
{
    HWC_NODE_S *pstNode = (HWC_NODE_S *)pNode;

    while (pstNode != NULL)
    {
        PrintNodeInfo(pstNode);
        pstNode = pstNode->pNextNode;
    }

    pstNode = (HWC_NODE_S *)pNode;

    /*call initreg here instead in Init() to reduce power cost*/
    InitCrgReg();

    /*写入首节点地址,ID*/
    gs_pstHWCReg->u32AqListHeadAddr = GFX2D_MEM_GetPhyaddr(pstNode);
    if (gs_pstHWCReg->u32AqListHeadAddr == 0)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "%p\n", pstNode);
    }

    mb();

    /*启动*/
    gs_pstHWCReg->u32HwcCtl = 0x1;

    return HI_SUCCESS;
}

HI_S32 GFX2D_HAL_HWC_LinkNode(HI_VOID *pCurNode, HI_VOID *pNextNode)
{
    HWC_NODE_S *pstCurNode = (HWC_NODE_S *)pCurNode;
    HWC_NODE_S *pstNextNode = (HWC_NODE_S *)pNextNode;

    /*If curtask request historam info,we can not link curtask and nexttask,
    for nexttask may overlay the info!*/
#ifdef GFX2D_HISTOGRAM_SUPPORT
    if (pstCurNode->pu32Histogram != 0)
    {
        return HI_FAILURE;
    }
#endif

    while (pstCurNode->pNextNode != NULL)
    {
        pstCurNode = pstCurNode->pNextNode;
    }

    pstCurNode->u32NextNodePhyaddr = GFX2D_MEM_GetPhyaddr(pstNextNode);
    if (pstCurNode->u32NextNodePhyaddr == 0)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "%p\n", pstNextNode);
    }
    pstCurNode->pNextNode = pstNextNode;

    return HI_SUCCESS;
}

HI_VOID GFX2D_HAL_HWC_SetNodeID(HI_VOID *pNode, HI_U32 u32NodeId)
{
    HWC_NODE_S *pstNode = (HWC_NODE_S *)pNode;

    pstNode->u32NodeId = u32NodeId;

    return;
}


HI_VOID GFX2D_HAL_HWC_FreeNode(HI_VOID *pNode)
{
    HWC_NODE_S *pstCurNode = (HWC_NODE_S *)pNode;
    HWC_NODE_S *pstNextNode = NULL;

    while ((pstCurNode != NULL) && (pstCurNode->pFirstNode == pNode))
    {
        pstNextNode = (HWC_NODE_S *)pstCurNode->pNextNode;
        GFX2D_MEM_Free(pstCurNode);
        pstCurNode = pstNextNode;
    }

    return;
}

HI_VOID GFX2D_HAL_HWC_NodeIsr(HI_VOID *pNode)
{
#ifdef GFX2D_HISTOGRAM_SUPPORT
    HWC_NODE_S *pstNode = (HWC_NODE_S *)pNode;
    HI_U32 i;

    if (pstNode->pu32Histogram != 0)
    {
        for (i = 0; i < 16; i++)
        {
            pstNode->pu32Histogram[i] = gs_pstHWCReg->u32Histogram[i];
        }
    }

    unmap_mmb_2(pstNode->pu32Histogram, pstNode->u32Offset);
#endif
    return;
}

HI_VOID GFX2D_HAL_HWC_AllNodeIsr(HI_VOID)
{
    DeinitCrgReg();
    return;
}


GFX2D_HAL_NODE_OPS_S g_stHwcNodeOps =
{
    .pfnSubNode = GFX2D_HAL_HWC_SubNode,
    .pfnLinkNode = GFX2D_HAL_HWC_LinkNode,
    .pfnSetNodeID = GFX2D_HAL_HWC_SetNodeID,
    .pfnFreeNode = GFX2D_HAL_HWC_FreeNode,
    .pfnNodeIsr = GFX2D_HAL_HWC_NodeIsr,
    .pfnAllNodeIsr = GFX2D_HAL_HWC_AllNodeIsr,
};

HI_VOID GFX2D_HAL_HWC_GetNodeOps(GFX2D_HAL_NODE_OPS_S **ppstNodeOps)
{
    *ppstNodeOps = &g_stHwcNodeOps;

    return;
}

HI_U32 GFX2D_HAL_HWC_GetIntStatus(HI_VOID)
{
    HI_U32 u32IntStatus = gs_pstHWCReg->u32MaskInt;
    HI_U32 u32Status = 0;

    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "\n");

    /*clear interrupt*/
    gs_pstHWCReg->u32IntClr = (u32IntStatus & 0xfffffffe);

    if (u32IntStatus & 0x4)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "\n");
    }

    if (u32IntStatus & 0x2)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "\n");
    }

    if (u32IntStatus & 0x8)
    {
        u32Status |= 0x1;
    }

    return u32Status;
}

HI_U32 GFX2D_HAL_HWC_GetIsrNum(HI_VOID)
{
    return HWC_ISRNUM;
}

HI_U32 GFX2D_HAL_HWC_GetBaseAddr(HI_VOID)
{
    return HWC_REG_BASE_ADDR;
}

#define IS_BIT_SET(bit,value) (0 == ((0x1 << bit) & value)?HI_FALSE:HI_TRUE)

static HI_S32 CheckSurfaceType(HI_GFX2D_SURFACE_S *pstSurface, HI_BOOL bInputSurface)
{
    if (HI_GFX2D_SURFACE_TYPE_COLOR < pstSurface->enType)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid surface type:%x\n", pstSurface->enType);
        return HI_ERR_GFX2D_INVALID_SURFACE_TYPE;
    }

    if (!bInputSurface && (HI_GFX2D_SURFACE_TYPE_MEM != pstSurface->enType))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid output surface type:%x\n", pstSurface->enType);
        return HI_ERR_GFX2D_INVALID_SURFACE_TYPE;
    }

    return HI_SUCCESS;
}

static HI_S32 CheckSurfaceFmt(HI_GFX2D_SURFACE_S *pstSurface, HI_BOOL bInputSurface)
{
    HI_GFX2D_FMT_E enFmt;

    enFmt = pstSurface->enFmt;

    if (enFmt > HI_GFX2D_FMT_PLANAR444)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid fmt:%d\n", enFmt);
        return HI_ERR_GFX2D_INVALID_SURFACE_FMT;
    }

    if (bInputSurface)
    {
        if (!IS_BIT_SET((enFmt % 32), gs_stCapability.au32InputFmtBitmap[enFmt / 32]))
        {
            HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid input fmt:%d\n", enFmt);
            return HI_ERR_GFX2D_INVALID_SURFACE_FMT;
        }
    }
    else
    {
        if (!IS_BIT_SET((enFmt % 32), gs_stCapability.au32OutputFmtBitmap[enFmt / 32]))
        {
            HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid output fmt:%d\n", enFmt);
            return HI_ERR_GFX2D_INVALID_SURFACE_FMT;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 CheckSurfaceAddr(HI_GFX2D_SURFACE_S *pstSurface, HI_BOOL bInputSurface)
{
    HI_GFX2D_FMT_E enFmt;

    enFmt = pstSurface->enFmt;

    if (0 == pstSurface->u32Phyaddr[0])
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid addr:0x%x\n", pstSurface->u32Phyaddr[0]);
        return HI_ERR_GFX2D_INVALID_SURFACE_ADDR;
    }

    if ((enFmt <= HI_GFX2D_FMT_SEMIPLANAR400) || (enFmt == HI_GFX2D_FMT_PLANAR400))
    {
        return HI_SUCCESS;
    }

    if (0 == pstSurface->u32Phyaddr[1])
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid addr:0x%x\n", pstSurface->u32Phyaddr[1]);
        return HI_ERR_GFX2D_INVALID_SURFACE_ADDR;
    }

    if (enFmt <= HI_GFX2D_FMT_PLANAR400)
    {
        return HI_SUCCESS;
    }

    if (0 == pstSurface->u32Phyaddr[2])
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid addr:0x%x\n", pstSurface->u32Phyaddr[2]);
        return HI_ERR_GFX2D_INVALID_SURFACE_ADDR;
    }

    return HI_SUCCESS;
}

static HI_S32 CheckSurfaceStride(HI_GFX2D_SURFACE_S *pstSurface, HI_BOOL bInputSurface)
{
    HI_GFX2D_FMT_E enFmt;

    enFmt = pstSurface->enFmt;

    if ((pstSurface->u32Stride[0] < gs_stCapability.u32MinStride)
        || (pstSurface->u32Stride[0] > gs_stCapability.u32MaxStride)
        || (pstSurface->u32Stride[0] % gs_stCapability.u8StrideAlign))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid stride:%d\n", pstSurface->u32Stride[0]);
        return HI_ERR_GFX2D_INVALID_SURFACE_STRIDE;
    }

    if ((enFmt <= HI_GFX2D_FMT_SEMIPLANAR400) || (enFmt == HI_GFX2D_FMT_PLANAR400))
    {
        return HI_SUCCESS;
    }

    if ((pstSurface->u32Stride[1] < gs_stCapability.u32MinStride)
        || (pstSurface->u32Stride[1] > gs_stCapability.u32MaxStride)
        || (pstSurface->u32Stride[1] % gs_stCapability.u8StrideAlign))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid stride:%d\n", pstSurface->u32Stride[1]);
        return HI_ERR_GFX2D_INVALID_SURFACE_STRIDE;
    }

    if (enFmt <= HI_GFX2D_FMT_PLANAR400)
    {
        return HI_SUCCESS;
    }

    if ((pstSurface->u32Stride[2] < gs_stCapability.u32MinStride)
        || (pstSurface->u32Stride[2] > gs_stCapability.u32MaxStride)
        || (pstSurface->u32Stride[2] % gs_stCapability.u8StrideAlign))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid stride:%d\n", pstSurface->u32Stride[2]);
        return HI_ERR_GFX2D_INVALID_SURFACE_STRIDE;
    }

    return HI_SUCCESS;
}

static HI_S32 CheckSurfaceSolution(HI_GFX2D_SURFACE_S *pstSurface, HI_BOOL bInputSurface)
{
    if (pstSurface->u32Width < gs_stCapability.u32MinWidth)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid width:%d\n", pstSurface->u32Width);
        return HI_ERR_GFX2D_INVALID_SURFACE_RESO;
    }

    if (pstSurface->u32Height < gs_stCapability.u32MinHeight)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid height:%d\n", pstSurface->u32Height);
        return HI_ERR_GFX2D_INVALID_SURFACE_RESO;
    }

    return HI_SUCCESS;
}

static HI_S32 CheckSurfaceCompress(HI_GFX2D_SURFACE_S *pstSurface, HI_BOOL bInputSurface)
{
    if (pstSurface->stCompressInfo.enCompressType > HI_GFX2D_COMPRESS_NOLOSS)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid cmptype:%d\n", pstSurface->stCompressInfo.enCompressType);
        return HI_ERR_GFX2D_INVALID_SURFACE_CMPTYPE;
    }

    if (bInputSurface)
    {
        if (HI_GFX2D_COMPRESS_NONE != pstSurface->stCompressInfo.enCompressType)
        {
            HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid cmptype:%d\n", pstSurface->stCompressInfo.enCompressType);
            return HI_ERR_GFX2D_INVALID_SURFACE_CMPTYPE;
        }
    }
    else
    {
        if (HI_GFX2D_COMPRESS_NOLOSS == pstSurface->stCompressInfo.enCompressType)
        {
            /*Only ARGB8888 is supported in cmp surface.*/
            if (!IS_BIT_SET((pstSurface->enFmt % 32), gs_stCapability.au32CmpFmtBitmap[pstSurface->enFmt / 32]))
            {
                HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid cmp fmt:%d\n", pstSurface->enFmt);
                return HI_ERR_GFX2D_UNSUPPORT;
            }

            /*Width of cmp surface should be even.*/
            if (pstSurface->u32Width % gs_stCapability.u32CmpWidthAlign)
            {
                HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid cmp width:%d\n", pstSurface->u32Width);
                return HI_ERR_GFX2D_UNSUPPORT;
            }
        }
    }

    return HI_SUCCESS;
}

static HI_S32 CheckSurface(HI_GFX2D_SURFACE_S *pstSurface, HI_BOOL bInputSurface)
{
    HI_S32 s32Ret;

    s32Ret = CheckSurfaceType(pstSurface, bInputSurface);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    if (HI_GFX2D_SURFACE_TYPE_COLOR == pstSurface->enType)
    {
        return HI_SUCCESS;
    }

    s32Ret = CheckSurfaceFmt(pstSurface, bInputSurface);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    s32Ret = CheckSurfaceAddr(pstSurface, bInputSurface);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    s32Ret = CheckSurfaceStride(pstSurface, bInputSurface);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }


    s32Ret = CheckSurfaceCompress(pstSurface, bInputSurface);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    s32Ret = CheckSurfaceSolution(pstSurface, bInputSurface);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }


    return HI_SUCCESS;
}

static HI_S32 CheckRect(HI_GFX2D_RECT_S *pstRegionRect, HI_GFX2D_RECT_S *pstRect)
{
    if ((pstRegionRect->s32XPos < 0) || (pstRegionRect->s32XPos >= pstRect->u32Width)
        || (0 == pstRegionRect->u32Width)
        || ((pstRegionRect->s32XPos + pstRegionRect->u32Width) > pstRect->u32Width)
        || (pstRegionRect->s32YPos < 0) || (pstRegionRect->s32YPos >= pstRect->u32Height)
        || (0 == pstRegionRect->u32Height)
        || ((pstRegionRect->s32YPos + pstRegionRect->u32Height) > pstRect->u32Height))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid rect[%d, %d, %d, %d]\n",
                              pstRegionRect->s32XPos, pstRegionRect->s32YPos,
                              pstRegionRect->u32Width, pstRegionRect->u32Height);
        return HI_ERR_GFX2D_INVALID_RECT;
    }

    if ((pstRegionRect->u32Width < gs_stCapability.u32MinWidth) || (pstRegionRect->u32Width > gs_stCapability.u32MaxWidth))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid width:%d\n", pstRegionRect->u32Width);
        return HI_ERR_GFX2D_INVALID_SURFACE_RESO;
    }

    if ((pstRegionRect->u32Height < gs_stCapability.u32MinHeight) || (pstRegionRect->u32Height > gs_stCapability.u32MaxHeight))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid height:%d\n", pstRegionRect->u32Height);
        return HI_ERR_GFX2D_INVALID_SURFACE_RESO;
    }


    return HI_SUCCESS;
}

static HI_S32 CheckClip(HI_GFX2D_CLIP_S *pstClip, HI_GFX2D_RECT_S *pstRect)
{
    if (pstClip->enClipMode >= HI_GFX2D_CLIP_OUTSIDE)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid clip mode:%d\n", pstClip->enClipMode);
        return HI_ERR_GFX2D_INVALID_CLIP_MODE;
    }
    else if (HI_GFX2D_CLIP_INSIDE == pstClip->enClipMode)
    {
        HI_S32 s32LXPos = pstRect->s32XPos;
        HI_S32 s32RXPos = pstRect->s32XPos + pstRect->u32Width - 1;
        HI_S32 s32TYPos = pstRect->s32YPos;
        HI_S32 s32BYPos = pstRect->s32YPos + pstRect->u32Height - 1;

        HI_S32 s32ClipLXPos = pstClip->stClipRect.s32XPos;
        HI_S32 s32ClipRXPos = pstClip->stClipRect.s32XPos + pstClip->stClipRect.u32Width - 1;
        HI_S32 s32ClipTYPos = pstClip->stClipRect.s32YPos;
        HI_S32 s32ClipBYPos = pstClip->stClipRect.s32YPos + pstClip->stClipRect.u32Height - 1;

        /*no intersection between clip rect and surface rect*/
        if ((s32ClipLXPos > s32RXPos) || (s32ClipRXPos < s32LXPos)
            || (s32ClipTYPos > s32BYPos) || (s32ClipBYPos < s32TYPos))
        {
            HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid clip rect:[%d, %d, %d, %d]\n",
                                  pstClip->stClipRect.s32XPos, pstClip->stClipRect.s32YPos,
                                  pstClip->stClipRect.u32Width, pstClip->stClipRect.u32Height);
            return HI_ERR_GFX2D_INVALID_CLIP_RECT;
        }
    }

    return HI_SUCCESS;

}

static HI_S32 CheckResize(HI_GFX2D_COMPOSE_S *pstCompose)
{
    HI_GFX2D_RECT_S *pstInRect = &(pstCompose->stInRect);
    HI_GFX2D_RECT_S *pstOutRect = &(pstCompose->stOutRect);
    HI_GFX2D_RESIZE_S *pstResize = &(pstCompose->stOpt.stResize);

    if (!pstResize->bResizeEnable)
    {
        if ((pstInRect->u32Width != pstOutRect->u32Width)
            || (pstInRect->u32Height > pstOutRect->u32Height))
        {
            HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Src rect not same with dst\n");
            return HI_ERR_GFX2D_INVALID_RECT;
        }
        return HI_SUCCESS;
    }

    if (pstResize->stFilter.enFilterMode > HI_GFX2D_FILTER_NONE)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid filter mode:%d\n", pstResize->stFilter.enFilterMode);
        return HI_ERR_GFX2D_INVALID_RESIZE_FILTERMODE;
    }

    /*Resize ratio exceed the limit of hardware,return errcode:
    Horizontal deflate ratio should less than 256.
    Vertical deflate ratio should less tha 16.*/
    if ((pstOutRect->u32Width * GFX2D_CONFIG_MAX_ZMERATION) <= pstInRect->u32Width)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid horizonal resize:%d->%d\n", pstInRect->u32Width, pstOutRect->u32Width);
        return HI_ERR_GFX2D_INVALID_RESIZE_RATIO;
    }

    if ((pstOutRect->u32Height * GFX2D_CONFIG_MAX_ZMERATION) <= pstInRect->u32Height)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Invalid vertical resize:%d->%d\n", pstInRect->u32Height, pstOutRect->u32Height);
        return HI_ERR_GFX2D_INVALID_RESIZE_RATIO;
    }
    else if ((pstOutRect->u32Height * gs_stCapability.u32MaxVerticalZmeRatio) <= pstInRect->u32Height)
    {
        HI_U32 u32Factor = (pstInRect->u32Height / (pstOutRect->u32Height * gs_stCapability.u32MaxVerticalZmeRatio)) + 1;

        if (pstCompose->stSurface.u32Stride[0] * u32Factor > gs_stCapability.u32MaxStride)
        {
            HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "unsupport!n");
            return HI_ERR_GFX2D_UNSUPPORT;
        }
        pstCompose->stSurface.u32Stride[0] *= u32Factor;

        if (pstCompose->stSurface.enFmt >= HI_GFX2D_FMT_SEMIPLANAR420UV)
        {
            if (pstCompose->stSurface.u32Stride[1] * u32Factor > gs_stCapability.u32MaxStride)
            {
                HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "unsupport!n");
                return HI_ERR_GFX2D_UNSUPPORT;
            }
            pstCompose->stSurface.u32Stride[1] *= u32Factor;
        }

        if (pstCompose->stSurface.enFmt >= HI_GFX2D_FMT_PLANAR420)
        {
            if (pstCompose->stSurface.u32Stride[2] * u32Factor > gs_stCapability.u32MaxStride)
            {
                HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "unsupport!n");
                return HI_ERR_GFX2D_UNSUPPORT;
            }
            pstCompose->stSurface.u32Stride[2] *= u32Factor;
        }

        pstCompose->stInRect.s32YPos /= u32Factor;
        pstCompose->stInRect.u32Height /= u32Factor;
    }

    return HI_SUCCESS;
}

static HI_S32 CheckRop(HI_GFX2D_ROP_S *pstRop)
{
    if (pstRop->bEnable)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "unsupport!n");
        return HI_ERR_GFX2D_UNSUPPORT;
    }

    return HI_SUCCESS;
}

static HI_S32 CheckColorkey(HI_GFX2D_COLORKEY_S *pstColorkey)
{
    if (pstColorkey->bEnable)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "unsupport!n");
        return HI_ERR_GFX2D_UNSUPPORT;
    }

    return HI_SUCCESS;
}

static HI_S32 CheckComposeOption(HI_GFX2D_COMPOSE_OPT_S *pstOpt, HI_GFX2D_RECT_S *pstOutRect)
{
    HI_S32 s32Ret;

    s32Ret = CheckRop(&pstOpt->stRop);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    s32Ret = CheckColorkey(&pstOpt->stKey);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    s32Ret = CheckClip(&pstOpt->stClip, pstOutRect);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 CheckComposeSrc(HI_GFX2D_COMPOSE_S *pstCompose,
                           HI_GFX2D_SURFACE_S *pstDstSurface)
{
    HI_GFX2D_SURFACE_S *pstSurface;
    HI_GFX2D_RECT_S *pstInRect;
    HI_GFX2D_RECT_S *pstOutRect;
    HI_GFX2D_COMPOSE_OPT_S *pstOpt;

    HI_GFX2D_RECT_S stSrcSurfaceRect;
    HI_GFX2D_RECT_S stDstSurfaceRect;

    HI_S32 s32Ret;

    pstSurface = &(pstCompose->stSurface);
    pstInRect = &(pstCompose->stInRect);
    pstOutRect = &(pstCompose->stOutRect);
    pstOpt = &(pstCompose->stOpt);

    stSrcSurfaceRect.s32XPos = 0;
    stSrcSurfaceRect.s32YPos = 0;
    stSrcSurfaceRect.u32Width = pstSurface->u32Width;
    stSrcSurfaceRect.u32Height = pstSurface->u32Height;

    stDstSurfaceRect.s32XPos = 0;
    stDstSurfaceRect.s32YPos = 0;
    stDstSurfaceRect.u32Width = pstDstSurface->u32Width;
    stDstSurfaceRect.u32Height = pstDstSurface->u32Height;

    s32Ret = CheckSurface(pstSurface, HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    s32Ret = CheckRect(pstOutRect, &stDstSurfaceRect);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    s32Ret = CheckComposeOption(pstOpt, pstOutRect);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (HI_GFX2D_SURFACE_TYPE_COLOR == pstSurface->enType)
    {
        return HI_SUCCESS;
    }

    s32Ret = CheckRect(pstInRect, &stSrcSurfaceRect);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    s32Ret = CheckResize(pstCompose);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    /*缩放时做clip操作，需要硬件支持clip*/
    if (pstOpt->stResize.bResizeEnable
        && (pstOpt->stClip.enClipMode != HI_GFX2D_CLIP_NONE)
        && gs_stCapability.bClipSupport)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "unsupport!n");
        return HI_ERR_GFX2D_UNSUPPORT;
    }

    if (pstSurface->enFmt >= HI_GFX2D_FMT_YUV888)
    {
        if ((pstInRect->u32Width < gs_stCapability.u32MinVWidth)
            || (pstInRect->u32Height < gs_stCapability.u32MinVHeight))
        {
            HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "unsupport!n");
            return HI_ERR_GFX2D_UNSUPPORT;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 CheckCompose(const HI_GFX2D_DEV_ID_E enDevId,
                           HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                           HI_GFX2D_SURFACE_S *pstDstSurface)
{
    HI_U32 i;
    HI_S32 s32Ret;
    HI_U32 u32VhdLayerNum = 0;
    HI_U32 u32GLayerNum = 0;
    HI_U32 u32ZmeLayerNum = 0;
    HI_GFX2D_COMPOSE_OPT_S *pstOpt;

    s32Ret = CheckSurface(pstDstSurface, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    for (i = 0; i < pstComposeList->u32ComposeCnt; i++)
    {
        s32Ret = CheckComposeSrc(&(pstComposeList->pstCompose[i]), pstDstSurface);
        if (HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }

        if (HI_GFX2D_SURFACE_TYPE_COLOR == pstComposeList->pstCompose[i].stSurface.enType)
        {
            u32GLayerNum++;
        }
        else
        {
            pstOpt = &(pstComposeList->pstCompose[i].stOpt);

            if (pstOpt->stResize.bResizeEnable)
            {
                u32ZmeLayerNum++;
            }

            if (pstComposeList->pstCompose[i].stSurface.enFmt >= HI_GFX2D_FMT_YUV888)
            {
                u32VhdLayerNum++;
            }
            else if ((pstComposeList->pstCompose[i].stInRect.u32Width < gs_stCapability.u32MinVWidth)
                     || (pstComposeList->pstCompose[i].stInRect.u32Height < gs_stCapability.u32MinVHeight))
            {
                u32GLayerNum++;
            }
        }
    }

    /*视频层超过硬件个数*/
    if ((u32VhdLayerNum > gs_stCapability.u8MaxVLayerNum)
        || (u32GLayerNum > gs_stCapability.u8MaxGLayerNum)
        || (u32ZmeLayerNum > gs_stCapability.u8ZmeLayerNum))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "unsupport!\n");
        return HI_ERR_GFX2D_UNSUPPORT;
    }

    return HI_SUCCESS;
}

static HI_VOID AdjustClipRect(HI_GFX2D_RECT_S *pstInRect, HI_GFX2D_RECT_S *pstOutRect,
                              HI_GFX2D_RECT_S *pstClipRect)
{
    HI_S32 s32LXPos = pstOutRect->s32XPos;
    HI_S32 s32RXPos = pstOutRect->s32XPos + pstInRect->u32Width - 1;
    HI_S32 s32TYPos = pstOutRect->s32YPos;
    HI_S32 s32BYPos = pstOutRect->s32YPos + pstInRect->u32Height - 1;

    HI_S32 s32ClipLXPos = pstClipRect->s32XPos;
    HI_S32 s32ClipRXPos = pstClipRect->s32XPos + pstClipRect->u32Width - 1;
    HI_S32 s32ClipTYPos = pstClipRect->s32YPos;
    HI_S32 s32ClipBYPos = pstClipRect->s32YPos + pstClipRect->u32Height - 1;

    /*calculate intersection*/
    if (s32ClipLXPos < s32LXPos)
    {
        s32ClipLXPos = s32LXPos;
    }

    if (s32ClipRXPos > s32RXPos)
    {
        s32ClipRXPos = s32RXPos;
    }

    if (s32ClipTYPos < s32TYPos)
    {
        s32ClipTYPos = s32TYPos;
    }

    if (s32ClipBYPos > s32BYPos)
    {
        s32ClipBYPos = s32BYPos;
    }

    pstInRect->s32XPos += (s32ClipLXPos - pstOutRect->s32XPos);
    pstInRect->s32YPos += (s32ClipTYPos - pstOutRect->s32YPos);

    pstOutRect->s32XPos = s32ClipLXPos;
    pstOutRect->s32YPos = s32ClipTYPos;

    pstOutRect->u32Width = s32ClipRXPos - s32ClipLXPos + 1;
    pstOutRect->u32Height = s32ClipBYPos - s32ClipTYPos + 1;

    pstInRect->u32Width = pstOutRect->u32Width;
    pstInRect->u32Height = pstOutRect->u32Height;

#if 0
    printk("=====clip rect:[%d, %d, %d, %d], [%d, %d, %d, %d]\n",
           pstInRect->s32XPos, pstInRect->s32YPos, pstInRect->u32Width, pstInRect->u32Height,
           pstOutRect->s32XPos, pstOutRect->s32YPos, pstOutRect->u32Width, pstOutRect->u32Height);
#endif

    return;
}


static HI_VOID AlignRect(const HI_GFX2D_FMT_E enFmt, HI_GFX2D_RECT_S *pRect)
{
    switch (enFmt)
    {
        case HI_GFX2D_FMT_SEMIPLANAR420UV:
        case HI_GFX2D_FMT_SEMIPLANAR420VU:
        case HI_GFX2D_FMT_PLANAR420:
        {
            pRect->u32Width &= 0xfffffffe;
            pRect->u32Height &= 0xfffffffc;
            pRect->s32YPos &= 0xfffffffe;
            break;
        }
        case HI_GFX2D_FMT_SEMIPLANAR422UV_H:
        case HI_GFX2D_FMT_SEMIPLANAR422VU_H:
        case HI_GFX2D_FMT_PLANAR400:
        case HI_GFX2D_FMT_PLANAR422H:
        case HI_GFX2D_FMT_PLANAR444:
        case HI_GFX2D_FMT_YUYV422:
        case HI_GFX2D_FMT_YVYU422:
        case HI_GFX2D_FMT_UYVY422:
        case HI_GFX2D_FMT_YYUV422:
        case HI_GFX2D_FMT_VYUY422:
        case HI_GFX2D_FMT_VUYY422:
        {
            pRect->u32Width &= 0xfffffffe;
            pRect->u32Height &= 0xfffffffe;
            break;
        }
        case HI_GFX2D_FMT_SEMIPLANAR422UV_V:
        case HI_GFX2D_FMT_SEMIPLANAR422VU_V:
        case HI_GFX2D_FMT_PLANAR410:
        case HI_GFX2D_FMT_PLANAR422V:
        {
            pRect->u32Width &= 0xfffffffc;
            pRect->u32Height &= 0xfffffffc;
            break;
        }
        case HI_GFX2D_FMT_PLANAR411:
        default:
        {
            pRect->u32Width &= 0xfffffffc;
            pRect->u32Height &= 0xfffffffe;
            break;
        }
    }

    return;
}

static HI_VOID AdjustCompose(const HI_GFX2D_DEV_ID_E enDevId,
                             HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                             HI_GFX2D_SURFACE_S *pstDstSurface)
{
    HI_U32 i;
    HI_GFX2D_COMPOSE_S *pstCompose;

    for (i = 0; i < pstComposeList->u32ComposeCnt; i++)
    {
        pstCompose = &pstComposeList->pstCompose[i];

        if (HI_GFX2D_CLIP_INSIDE == pstCompose->stOpt.stClip.enClipMode
            && !pstCompose->stOpt.stResize.bResizeEnable)
        {
            AdjustClipRect(&(pstCompose->stInRect), &(pstCompose->stOutRect),
                           &(pstCompose->stOpt.stClip.stClipRect));
        }

        if (HI_GFX2D_SURFACE_TYPE_COLOR == pstCompose->stSurface.enType)
        {
            continue;
        }

        if (pstCompose->stSurface.enFmt >= HI_GFX2D_FMT_YUV888)
        {
            pstCompose->stInRect.s32XPos &= 0xfffffffe;

            AlignRect(pstCompose->stSurface.enFmt,&(pstCompose->stInRect));

            if (pstCompose->stOpt.stResize.bResizeEnable)
            {
                pstCompose->stOutRect.u32Width &= 0xfffffffe;
                pstCompose->stOutRect.u32Height &= 0xfffffffe;
            }
            else
            {
                pstCompose->stOutRect.u32Width = pstCompose->stInRect.u32Width;
                pstCompose->stOutRect.u32Height = pstCompose->stInRect.u32Height;
            }
            break;
        }
        else
        {
            if (pstCompose->stOpt.stResize.bResizeEnable)
            {
                pstCompose->stInRect.u32Width &= ~(gs_stCapability.u8ZmeAlign - 1);
                pstCompose->stInRect.u32Height &= ~(gs_stCapability.u8ZmeAlign - 1);
                pstCompose->stOutRect.u32Width &= ~(gs_stCapability.u8ZmeAlign - 1);
                pstCompose->stOutRect.u32Height &= ~(gs_stCapability.u8ZmeAlign - 1);
            }
        }
    }
}

#ifndef GFX2D_PROC_UNSUPPORT
/*HWC proc信息*/
HI_VOID GFX2D_HAL_HWC_ReadProc(struct seq_file *p, HI_VOID *v)
{
    SEQ_Printf(p, "---------------------GFX2D Hal Info--------------------\n");
    if (gs_pstHWCReg)
    {
        //SEQ_Printf(p, "Status\t:%s\n", (gs_pstHWCReg->u32Status & 0x1) ? "busy" : "idle");
    }
    else
    {
        SEQ_Printf(p, "Status\t\t:idle\n");
    }
    return;
}

HI_S32 GFX2D_HAL_HWC_WriteProc(struct seq_file *p, HI_VOID *v)
{
    return HI_SUCCESS;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/
