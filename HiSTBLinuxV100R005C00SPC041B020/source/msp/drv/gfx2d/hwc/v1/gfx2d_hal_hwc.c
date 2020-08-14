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

#include <asm/barrier.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#if HWC_VERSION == 0x300
#define HWC_REG_BASE_ADDR 0xFF400000
#else
#define HWC_REG_BASE_ADDR 0xf8c20000
#endif
#define HWC_ISRNUM 0x7c

#define GFX2D_CONFIG_ZME_LAYER 2

#define GFX2D_HAL_MAX_NODE 2

#define GFX2D_CONFIG_MAX_ZMERATION 256

/*获取滤波系数的符号位及低9bit,组成10bit数*/
#define HWC_GET_BITS_OF_FILTERPARA(para) ((((para) & 0x80000000) >> 22) | ((para) & 0x1ff))

#define GFX2D_HAL_HWC_HSTEP_FLOATLEN 20
#define GFX2D_HAL_HWC_NOSCALE_HSTEP (0x1 << GFX2D_HAL_HWC_HSTEP_FLOATLEN)
#define GFX2D_HAL_HWC_VSTEP_FLOATLEN 12
#define GFX2D_HAL_HWC_NOSCALE_VSTEP (0x1 << GFX2D_HAL_HWC_VSTEP_FLOATLEN)

typedef union tagHWC_BUSLIMITER_U
{
    struct
    {
        HI_U32 bit10WriteCnt : 10;
        HI_U32 bit10ReadCnt : 10;
        HI_U32 bit11Reserved : 11;
        HI_U32 bit1CkGtEn : 1;
    } stBits;
    HI_U32 u32All;
} HWC_BUSLIMITER_U;

typedef union tagHWC_AXIID_U
{
    struct
    {
#if HWC_VERSION == 0x400
        HI_U32 u32Reserved1 : 8;
#else
        HI_U32 bit4AxiRid0 : 4;
        HI_U32 bit4AxiRid1 : 4;
#endif
        HI_U32 bit4AxiWid0 : 4;
        HI_U32 bit1ReqSepEn4k : 1;
#if HWC_VERSION == 0x400
        HI_U32 u32Reserved2 : 4;
        HI_U32 bit4OutstdRid0 : 4;
#else
        HI_U32 bit4OutstdRid0 : 4;
        HI_U32 bit4OutstdRid1 : 4;
#endif
        HI_U32 bit4OutstdWid0 : 4;
#if HWC_VERSION == 0x400
        HI_U32 bit5Reserved3 : 5;
        HI_U32 bit2RbusMode : 2;
#else
        HI_U32 bit7Reserved3 : 7;
#endif
    } stBits;
    HI_U32 u32All;
} HWC_AXIID_U;

typedef union tagHWC_IDSEL_U
{
    struct
    {
        HI_U32 bit1VhdRidSel : 1;
        HI_U32 bit1G0RidSel : 1;
        HI_U32 bit1G1RidSel : 1;
        HI_U32 bit1G2RidSel : 1;
        HI_U32 bit1G3RidSel : 1;
        HI_U32 bit1G4RidSel : 1;
        HI_U32 bit1G5RidSel : 1;
        HI_U32 bit1CasRidSel : 1;
        HI_U32 bit24Reserved : 24;
    } stBits;
    HI_U32 u32All;
} HWC_IDSEL_U;

typedef struct tagHWC_REG_S
{
    HI_U32 u32Version;
    HI_U32 u32Reserved1;
    HI_U32 u32HwcRst;
    HI_U32 u32HwcCtl;
    HI_U32 u32HwcAqCtl;
    HI_U32 u32AqListHeadAddr;
    HI_U32 u32AqListHeadId;
    HWC_BUSLIMITER_U unBusLimiter;
    HI_U32 u32Monitor;
    HWC_AXIID_U unAxiId;
#if HWC_VERSION == 0x400
    HI_U32 u32Reserved2;
#else
    HWC_IDSEL_U unIdSel;
#endif
    HI_U32 u32Status;
    HI_U32 u32PendingNodeAddr;
    HI_U32 u32WorkingNodeAddr;
    HI_U32 u32CompNodeAddr;
    HI_U32 u32CompNodeId;
    HI_U32 u32MaskInt;
    HI_U32 u32Int;
#if HWC_VERSION >= 0x200
    HI_U32 u32Cnt;
    HI_U32 u32RCnt;
    HI_U32 u32WCnt;
    HI_U32 u32RLenCnt;
    HI_U32 u32WLenCnt;
    HI_U32 u32ErrInt;
    HI_U32 u32MaskErrInt;
    HI_U32 u32CmpSta;
#if HWC_VERSION == 0x400
    HI_U32 u32Reserved[234];
#else
    HI_U32 u32Reserved[186];
#endif
#else
    HI_U32 u32Reserved[194];
#endif
    HI_U32 u32Histogram[16];
} HWC_REG_S;

typedef union tagHWC_INTMASK_U
{
    struct
    {
        HI_U32 bit1ListCompMask : 1;
        HI_U32 bit1NodeCompMask : 1;
        HI_U32 bit2Reserved1 : 2;
        HI_U32 bit1SlaveWriteErrMask : 1;
        HI_U32 bit1DecWriteErrMask : 1;
        HI_U32 bit1SlaveReadErrMask : 1;
        HI_U32 bit1DecReadErrMask : 1;
        HI_U32 bit1DieErrMask : 1;
#if HWC_VERSION == 0x400
        HI_U32 bit1CfgErrMask : 1;
        HI_U32 bit22Reserved2 : 22;
#else
        HI_U32 bit23Reserved2 : 23;
#endif
    } stBits;
    HI_U32 u32All;
} HWC_INTMASK_U;

typedef union tagHWC_VHDCTL_U
{
    struct
    {
        HI_U32 bit5Fmt : 5;
#if HWC_VERSION == 0x400
        HI_U32 bit4Reserved1 : 4;
#else
        HI_U32 bit2ChromaReadMode : 2;
        HI_U32 bit2LumReadMode : 2;
#endif
        HI_U32 bit2ExpandMode : 2;
        HI_U32 bit1UVOrder : 1;
#if HWC_VERSION == 0x400
        HI_U32 bit1ChromaMmuBypass : 1;
        HI_U32 bit1LumMmuBypass : 1;
        HI_U32 bit11Reserved2 : 11;
#else
        HI_U32 bit13Reserved2 : 13;
#endif
        HI_U32 bit4ZmehDwsMode : 4;
        HI_U32 bit1Reserved3 : 1;
        HI_U32 bit1ZmehDwsm : 1;
        HI_U32 bit1IsEnable : 1;
    } stBits;
    HI_U32 u32All;
} HWC_VHDCTL_U;

typedef union tagHWC_POS_U
{
    struct
    {
        HI_U32 bit12X : 12;
        HI_U32 bit12Y : 12;
        HI_U32 bit8Reserved : 8;
    } stBits;
    HI_U32 u32All;
} HWC_POS_U;

typedef union tagHWC_RES_U
{
    struct
    {
        HI_U32 bit12Width : 12;
        HI_U32 bit12Height : 12;
        HI_U32 bit8Reserved : 8;
    } stBits;
    HI_U32 u32All;
} HWC_RES_U;

typedef union tagHWC_VHDRAWMODE_U
{
    struct
    {
        HI_U32 bit3Mode : 3;
        HI_U32 bit29Reserved : 29;
    } stBits;
    HI_U32 u32All;
} HWC_VHDRAWMODE_U;

typedef union tagHWC_VHDCBMPARA_U
{
    struct
    {
        HI_U32 bit8GlobalAlpha : 8;
        HI_U32 bit4Reserved1 : 4;
        HI_U32 bit1PixelAlphaEnable : 1;
        HI_U32 bit1bPreMul : 1;
        HI_U32 bit1bCovBlend : 1;
        HI_U32 bit1Reserved2 : 1;
        HI_U32 bit8Alpha0 : 8;
        HI_U32 bit8Alpha1 : 8;
    } stBits;
    HI_U32 u32All;
} HWC_VHDCBMPARA_U;


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
#if HWC_VERSION == 0x400
        HI_U32 bit1Reserved : 1;
#else
        HI_U32 bit1IsNonLinear : 1;
#endif
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

typedef union tagHWC_GCTRL_U
{
    struct
    {
        HI_U32 bit8Fmt : 8;
        HI_U32 bit2ExpandMode : 2;
#if HWC_VERSION == 0x400
        HI_U32 bit9Reserved1 : 9;
        HI_U32 bit4ZmehDwsMode : 4;
        HI_U32 bit1ZmehDwsm : 1;
        HI_U32 bit1MmuBypass : 1;
        HI_U32 bit5Reserved2 : 5;
#else
        HI_U32 bit16Reserved1 : 16;
        HI_U32 bit1ReadMode : 1;
        HI_U32 bit1ReadField : 1;
        HI_U32 bit2Reserved2 : 2;
#endif
        HI_U32 bit1IsDim : 1;
        HI_U32 bit1IsEnable : 1;
    } stBits;
    HI_U32 u32All;
} HWC_GCTRL_U;

typedef union tagHWC_GCBMPARA_U
{
    struct
    {
        HI_U32 bit8GlobalAlpha : 8;
        HI_U32 bit1Reserved1 : 1;
        HI_U32 bit3Reserved2 : 3;
        HI_U32 bit1PixelAlphaEnable : 1;
        HI_U32 bit1bPreMul : 1;
        HI_U32 bit1bCovBlend : 1;
        HI_U32 bit1Reserved3 : 1;
        HI_U32 bit8Alpha0 : 8;
        HI_U32 bit8Alpha1 : 8;
    } stBits;
    HI_U32 u32All;
} HWC_GCBMPARA_U;

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
        HI_U32 bit11Reserved : 11;
    } stBits;
    HI_U32 u32All;
} HWC_CBMMIX_U;

typedef union tagHWC_OUTCTL_U
{
    struct
    {
        HI_U32 bit8Fmt : 8;
        HI_U32 bit8AlphaThreshold : 8;
        HI_U32 bit1Dither: 1;
#if HWC_VERSION == 0x400
        HI_U32 bit6Reserved1 : 6;
        HI_U32 bit1ChormeMmuBypass : 1;
        HI_U32 bit1LumMmuBypass : 1;
        HI_U32 bit3Reserved2 : 3;
        HI_U32 bit1CmpEn : 1;
        HI_U32 bit2CasProt : 2;
#else
        HI_U32 bit9Reserved1 : 9;
        HI_U32 bit1WriteMode : 1;
        HI_U32 bit1WriteField : 1;
        HI_U32 bit3Reserved2 : 3;
#endif
        HI_U32 bit1Enable : 1;
    } stBits;
    HI_U32 u32All;
} HWC_OUTCTL_U;

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


typedef struct tagHWC_GLAYER_S
{
    HWC_GCTRL_U unCtrl;
    HI_U32 u32Phyaddr;
    HI_U32 u32Stride;
    HI_U32 u32SfPos;
    HI_U32 u32DimValue;
    HWC_GCBMPARA_U unBmpPara;
    HWC_RES_U unRes;
    HWC_POS_U unOutStartPos;
    HWC_POS_U unOutEndPos;
#if HWC_VERSION == 0x400
    HWC_VHDHSP_U unG0Hsp;
    HWC_VHDHOFFSET_U unLumHOffset;
    HWC_VHDHOFFSET_U unChromaHOffset;
    HWC_VHDVSP_U unVsp;
    HI_U32 u32Vsr;
    HI_U32 u32VOffset;
    HWC_RES_U unZmeOutRes;
    HWC_RES_U unZmeInRes;
    HWC_CROPPOS_U unCropPos;
    HWC_CROPRESO_U unCropInReso;
    HWC_CROPRESO_U unCropOutReso;
#else
    HI_U32 u32Reserved1;   /*0x1a4*/
    HI_U32 u32Reserved2;   /*0x1a8*/
    HI_U32 u32Reserved3;   /*0x1ac*/
#endif
} HWC_GLAYER_S;


/*node structure*/
typedef struct tagHWC_NODE_S
{
    HWC_INTMASK_U unIntMask;
    HI_U32 u32HCoefPhyaddr;
    HI_U32 u32VCoefPhyaddr;
#if HWC_VERSION >= 0x200
    HI_U32 u32HChromaCoefPhyaddr;
    HI_U32 u32VChromaCoefPhyaddr;
    HI_U32 u32CfgErrIntMask;
#if HWC_VERSION == 0x400
    HI_U32 u32G0HCoefPhyaddr;
    HI_U32 u32G0VCoefPhyaddr;
    HI_U32 u32G0HChromaCoefPhyaddr;
    HI_U32 u32G0VChromaCoefPhyaddr;
    HI_U32 au32Reserved1[6];
#else
    HI_U32 au32Reserved1[10];   /*0xd8-0xfc*/
#endif
#else
    HI_U32 au32Reserved1[13];   /*0xcc-0xfc*/
#endif
    HWC_VHDCTL_U unVhdCtl;
    HI_U32 u32VhdYPhyaddr;
    HI_U32 u32VhdCbPhyaddr;
    HI_U32 u32VhdCrPhyaddr;
    HI_U32 u32VhdYSride;
    HI_U32 u32VhdCrStride;
    HWC_POS_U unCropStartPos;
    HWC_POS_U unCropEndPos;
    HWC_RES_U unVhdRes;
#if HWC_VERSION == 0x400
    HI_U32 u32Reserved;
#else
    HWC_VHDRAWMODE_U unVhdDrawMode;
#endif
    HWC_RES_U unVhdInRes;
    HWC_VHDCBMPARA_U unVhdCbmPara;
    HWC_POS_U unOutStartPos;
    HWC_POS_U unOutEndPos;
    HWC_VHDCSCIDC_U unVhdCscIdc;
    HWC_VHDCSCODC_U unVhdCscOdc;
    HWC_VHDCSCPARAM_U unVhdCscParam0;
    HWC_VHDCSCPARAM_U unVhdCscParam1;
    HWC_VHDCSCPARAM_U unVhdCscParam2;
    HWC_VHDCSCPARAM_U unVhdCscParam3;
    HWC_VHDCSCPARAM_U unVhdCscParam4;
    HWC_VHDHSP_U unVhdHsp;
    HWC_VHDHOFFSET_U unVhdLumHOffset;
    HWC_VHDHOFFSET_U unVhdChromaHOffset;
    HWC_VHDVSP_U unVhdVsp;
    HI_U32 u32VhdVsr;
    HI_U32 u32VhdVOffset;
    HWC_RES_U unVhdZmeOutRes;
    HWC_RES_U unVhdZmeInRes;
#if HWC_VERSION == 0x400
    HWC_CROPPOS_U unVhdCropPos;
    HWC_CROPRESO_U unVhdCropInReso;
    HWC_CROPRESO_U unVhdCropOutReso;
#else
    HI_U32 au32Reserved2[3];    /*0x174-0x17c*/
#endif
    HWC_GLAYER_S astGLayer[6];    /*0x180-0x29c*/
    HI_U32 u32CbmBkgValue;
    HWC_CBMMIX_U unCbmMix;
    HI_U32 au32Reserved3[2];    /*0x2a8-0x2ac*/
    HWC_OUTCTL_U unOutCtl;
    HI_U32 u32OutPhyaddr;
    HI_U32 u32OutChromPhyaddr;
    HI_U32 u32OutStride;
    HWC_RES_U unOutRes;
    HI_U32 au32Reserved4[7];    /*0x2c4-0x2dc*/
    HWC_OUTCSCIDC_U unOutCscIdc;
    HWC_OUTCSCODC_U unOutCscOdc;
    HWC_OUTCSCPARAM_U unOutCscParam0;
    HWC_OUTCSCPARAM_U unOutCscParam1;
    HWC_OUTCSCPARAM_U unOutCscParam2;
    HWC_OUTCSCPARAM_U unOutCscParam3;
    HWC_OUTCSCPARAM_U unOutCscParam4;
#if HWC_VERSION == 0x400
    HI_U32 u32Reserved5;
    HWC_CMPGLBINFO_U unCmpGlbInfo;
    HWC_CMPRESO_U unCmpReso;
    HWC_CMPCFG0_U unCmpCfg0;
    HWC_CMPCFG1_U unCmpCfg1;
    HWC_CMPCFG2_U unCmpCfg2;
    HWC_CMPCFG3_U unCmpCfg3;
    HWC_CMPCFG4_U unCmpCfg4;
    HWC_CMPCFG5_U unCmpCfg5;
    HWC_CMPCFG6_U unCmpCfg6;
    HWC_CMPCFG7_U unCmpCfg7;
    HWC_CMPCFG8_U unCmpCfg8;
    HWC_CMPCFG9_U unCmpCfg9;
#else
    HI_U32 au32Reserved5[9];    /*0x2fc-0x31c*/
    HWC_CMPCTRL_U unCmpCtrl;    /*0x320*/
    HI_U32 u32CmpDdr;
    HI_U32 u32CmpDataOffsetL;
    HI_U32 u32CmpDataOffsetC;
#endif
    HI_U32 u32NextNodePhyaddr;
    HI_U32 u32NextNodeId;
    HI_VOID *pNextNode;
    HI_U32 u32NodeId;
    HI_U32 *pu32Histogram;
    unsigned long u32Offset;
} HWC_NODE_S;

/*每128bit一组,每10bit存放一个系数.每128bit存放12个系数*/
typedef struct tagHWC_FILTERPARA_S
{
    struct
    {
        HI_U32 bit10Para0 : 10;
        HI_U32 bit10Para1 : 10;
        HI_U32 bit10Para2 : 10;
        HI_U32 bit2Para3 : 2;
    } stDW1;
    struct
    {
        HI_U32 bit8Para3 : 8;
        HI_U32 bit10Para4 : 10;
        HI_U32 bit10Para5 : 10;
        HI_U32 bit4Para6 : 4;
    } stDW2;
    struct
    {
        HI_U32 bit6Para6 : 6;
        HI_U32 bit10Para7 : 10;
        HI_U32 bit10Para8 : 10;
        HI_U32 bit6Para9 : 6;
    } stDW3;
    struct
    {
        HI_U32 bit4Para9 : 4;
        HI_U32 bit10Para10 : 10;
        HI_U32 bit10Para11 : 10;
        HI_U32 bit8Reserved : 8;
    } stDW4;
} HWC_FILTERPARA_S;

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

/*滤波系数在内存中的存储方式:硬件提供了两个滤波系数地址寄存器:
水平滤波系数地址,垂直滤波系数地址.
水平亮度滤波系数和水平色度滤波系数存放在一起,亮度在前,色度在后.同样,垂直亮度
滤波系数和垂直色度滤波系数放在一起,亮度在前,色度在后*/
/*每个缩放比对应的滤波系数需要的存储空间大小,单位为字节*/
/*存储方式:每12个系数一组,每组16个字节*/
#define HWC_HORIZON_LUM_PARA_SIZE_PERLEVEL (((HWC_FILTER_HORIZON_LUM_TAP * HWC_FILTER_PHASE + 11) / 12)  * 16)
#define HWC_VERTICAL_LUM_PARA_SIZE_PERLEVEL (((HWC_FILTER_VERTICAL_LUM_TAP * HWC_FILTER_PHASE + 11) / 12)  * 16)
#define HWC_HORIZON_CHM_PARA_SIZE_PERLEVEL (((HWC_FILTER_HORIZON_CHM_TAP * HWC_FILTER_PHASE + 11) / 12)  * 16)
#define HWC_VERTICAL_CHM_PARA_SIZE_PERLEVEL (((HWC_FILTER_VERTICAL_CHM_TAP * HWC_FILTER_PHASE + 11) / 12)  * 16)

#define HWC_HORIZON_PARA_SIZE_PERLEVEL ((HWC_HORIZON_LUM_PARA_SIZE_PERLEVEL) + (HWC_HORIZON_CHM_PARA_SIZE_PERLEVEL))
#define HWC_VERTICAL_PARA_SIZE_PERLEVEL ((HWC_VERTICAL_LUM_PARA_SIZE_PERLEVEL) +(HWC_VERTICAL_CHM_PARA_SIZE_PERLEVEL))

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
static inline HI_VOID InitNode(HWC_NODE_S* pstNode); /*初始化任务节点*/
static HI_VOID SetSrc(HWC_NODE_S* pstNode, HI_GFX2D_COMPOSE_S *pstCompose,
                      HI_U32 u32Zorder, HWC_LAYER_ID enLayerId);
static inline HI_VOID SetDst(HWC_NODE_S* pstNode, HI_GFX2D_SURFACE_S *pstDstSurface);
static inline HI_VOID SetInCsc(HWC_NODE_S *pstNode);/*设置输入CSC*/
static inline HI_VOID SetOutCsc(HWC_NODE_S* pstNode);/*设置输出CSC*/
static inline HI_VOID SetBgColor(HWC_NODE_S* pstNode, HI_U32 u32BgColor);
static HI_S32 SelectLayerId(HI_GFX2D_COMPOSE_LIST_S *pstComposeList, HWC_LAYER_ID *penLayerId);
static HI_VOID AdjustCompose(const HI_GFX2D_DEV_ID_E enDevId,
                             HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                             HI_GFX2D_SURFACE_S *pstDstSurface);

static HWC_CAPABILITY_S gs_stCapability = {0};

HI_S32 GFX2D_HAL_HWC_Init(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32NodeNum;
    HI_U32 u32NodeSize;

    s32Ret = InitFilterParam();
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    u32NodeNum = GFX2D_CONFIG_GetNodeNum();
    u32NodeSize = ((sizeof(HWC_NODE_S) + 0xf) & 0xffffff0);

    s32Ret = GFX2D_MEM_Register(u32NodeSize, u32NodeNum);
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
    HI_U32 u32NodeSize;

    u32NodeSize = ((sizeof(HWC_NODE_S) + 0xf) & 0xffffff0);
    GFX2D_Mem_UnRegister(u32NodeSize);

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

static HI_VOID *GFX2D_HAL_HWC_CreateNode(const HI_GFX2D_DEV_ID_E enDevId,
                                       HI_GFX2D_COMPOSE_LIST_S *pstComposeList,
                                       HI_GFX2D_SURFACE_S *pstDstSurface,
                                       HI_U8 u8NodeCnt)
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

        if (i > 0)
        {
            pstNode[i - 1]->u32NextNodePhyaddr = GFX2D_MEM_GetPhyaddr(pstNode[i]);

            if (pstNode[i - 1]->u32NextNodePhyaddr == 0)
            {
                HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "%p\n", pstNode[i]);
            }
            pstNode[i - 1]->pNextNode = pstNode[i];
            pstNode[i - 1]->u32NextNodeId = pstNode[i]->u32NodeId;
        }
    }

    return pstNode[0];

ERR:
    for (i = 0; i < u8NodeCnt; i++)
    {
        if (pstNode[i] != NULL)
        {
            GFX2D_MEM_Free(pstNode[i]);
        }
    }

    return HI_NULL;
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

    pstNode = GFX2D_HAL_HWC_CreateNode(enDevId,
                                       stComposeList,
                                       pstDstSurface,
                                       u8NodeCnt);
    if (NULL == pstNode)
    {
        s32Ret = HI_ERR_GFX2D_UNSUPPORT;
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

static inline HI_VOID InitCrgReg(HI_VOID)
{
    volatile U_PERI_CRG61 unTempValue;
    HI_U8 u8Cnt;

    unTempValue.u32 = g_pstRegCrg->PERI_CRG61.u32;

    /*enable clock*/
    unTempValue.bits.hwc_cken = 0x1;

    /*select frequency:432M*/
    unTempValue.bits.hwc_clk_sel = 0x2;

    g_pstRegCrg->PERI_CRG61.u32 = unTempValue.u32;

    for (u8Cnt = 0; u8Cnt < 10; u8Cnt++)
    {
        if (g_pstRegCrg->PERI_CRG61.u32 == unTempValue.u32)
        {
            break;
        }
    }

    /*cancel reset*/
    unTempValue.bits.hwc_srst_req = 0x0;

    g_pstRegCrg->PERI_CRG61.u32 = unTempValue.u32;

    for (u8Cnt = 0; u8Cnt < 10; u8Cnt++)
    {
        if (g_pstRegCrg->PERI_CRG61.u32 == unTempValue.u32)
        {
            break;
        }
    }

    mb();

    if (gs_pstHWCReg != NULL)
    {
        gs_pstHWCReg->u32HwcRst = 0x1;
        gs_pstHWCReg->unBusLimiter.stBits.bit1CkGtEn = 0x1;
        gs_pstHWCReg->u32HwcAqCtl = 0x80000000;
        gs_pstHWCReg->unAxiId.stBits.bit1ReqSepEn4k = 0x1;
#if HWC_VERSION == 0x400
        //gs_pstHWCReg->unAxiId.stBits.bit2RbusMode = 0x1;
#endif
    }

    return;
}

static inline HI_VOID DeinitCrgReg(HI_VOID)
{
    U_PERI_CRG61 unTempValue;

    unTempValue.u32 = g_pstRegCrg->PERI_CRG61.u32;

    /*disable clock*/
    unTempValue.bits.hwc_cken = 0x0;

    /*reset*/
    unTempValue.bits.hwc_srst_req = 0x1;

    unTempValue.u32 = g_pstRegCrg->PERI_CRG61.u32;

    return;
}

static inline HI_S32 InitDevReg(HI_VOID)
{
    /*map register*/
    if (HI_NULL == gs_pstHWCReg)
    {
        gs_pstHWCReg = HI_GFX_REG_MAP(HWC_REG_BASE_ADDR, sizeof(HWC_REG_S));
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

static HI_VOID SetFilterPara(HWC_FILTERPARA_S *pstPara, HI_S32 s32Value, HI_U32 u32Index)
{
    switch (u32Index % 12)
    {
        case 0:
            pstPara->stDW1.bit10Para0 = HWC_GET_BITS_OF_FILTERPARA(s32Value);
            break;
        case 1:
            pstPara->stDW1.bit10Para1 = HWC_GET_BITS_OF_FILTERPARA(s32Value);
            break;
        case 2:
            pstPara->stDW1.bit10Para2 = HWC_GET_BITS_OF_FILTERPARA(s32Value);
            break;
        case 3:
            pstPara->stDW1.bit2Para3 = HWC_GET_BITS_OF_FILTERPARA(s32Value) & 0x3;
            pstPara->stDW2.bit8Para3 = HWC_GET_BITS_OF_FILTERPARA(s32Value) >> 2;
            break;
        case 4:
            pstPara->stDW2.bit10Para4 = HWC_GET_BITS_OF_FILTERPARA(s32Value);
            break;
        case 5:
            pstPara->stDW2.bit10Para5 = HWC_GET_BITS_OF_FILTERPARA(s32Value);
            break;
        case 6:
            pstPara->stDW2.bit4Para6 = HWC_GET_BITS_OF_FILTERPARA(s32Value) & 0xf;
            pstPara->stDW3.bit6Para6 = HWC_GET_BITS_OF_FILTERPARA(s32Value) >> 4;
            break;
        case 7:
            pstPara->stDW3.bit10Para7 = HWC_GET_BITS_OF_FILTERPARA(s32Value);
            break;
        case 8:
            pstPara->stDW3.bit10Para8 = HWC_GET_BITS_OF_FILTERPARA(s32Value);
            break;
        case 9:
            pstPara->stDW3.bit6Para9 = HWC_GET_BITS_OF_FILTERPARA(s32Value) & 0x3f;
            pstPara->stDW4.bit4Para9 = HWC_GET_BITS_OF_FILTERPARA(s32Value) >> 6;
            break;
        case 10:
            pstPara->stDW4.bit10Para10 = HWC_GET_BITS_OF_FILTERPARA(s32Value);
            break;
        case 11:
            pstPara->stDW4.bit10Para11 = HWC_GET_BITS_OF_FILTERPARA(s32Value);
            break;
    }

    return;
}

static HI_VOID LoadFilterParam(HWC_FILTERPARA_S *pstPara,
                               HI_S32 *ps32LumPara, HI_U32 u32LumTap,
                               HI_S32 *ps32ChmPara, HI_U32 u32ChmTap)
{
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U32 k = 0;
    HI_U32 u32Count = 0;

    for (i = 0; i < HWC_FILTER_LEVELS; i++)
    {
        u32Count = 0;
        for (j = 0; j < HWC_FILTER_PHASE; j++)
        {
            for (k = 0; k < u32LumTap; k++)
            {
                SetFilterPara(pstPara, *ps32LumPara, u32Count);
                ps32LumPara++;
                u32Count++;
                if (0 == (u32Count % 12))
                {
                    pstPara++;
                }
            }
        }
        pstPara++;
        u32Count = 0;
        for (j = 0; j < HWC_FILTER_PHASE; j++)
        {
            for (k = 0; k < u32ChmTap; k++)
            {
                SetFilterPara(pstPara, *ps32ChmPara, u32Count);
                ps32ChmPara++;
                u32Count++;
                if (0 == (u32Count % 12))
                {
                    pstPara++;
                }
            }
        }
        pstPara++;
    }

    return;
}

static HI_S32 InitFilterParam(HI_VOID)
{
    HI_VOID *pVir;
    HI_S32 *ps32LumPara;
    HI_S32 *ps32ChmPara;
    HI_U32 u32FiterMemSize;
    HI_BOOL bMmu = HI_FALSE;

    u32FiterMemSize = HWC_HORIZON_PARA_SIZE_TOTAL + HWC_VERTICAL_PARA_SIZE_TOTAL * 2;
    gs_u32HFilterParamAddr = HI_GFX_AllocMem("GFX2D_FilterParam", NULL, u32FiterMemSize, &bMmu);
    if (0 == gs_u32HFilterParamAddr)
    {
        return HI_ERR_GFX2D_NO_MEM;
    }

    pVir = HI_GFX_Map(gs_u32HFilterParamAddr, 0, bMmu);
    if (NULL == pVir)
    {
        HI_GFX_FreeMem(gs_u32HFilterParamAddr, bMmu);
        gs_u32HFilterParamAddr = 0;
        return HI_ERR_GFX2D_NO_MEM;
    }

    ps32LumPara = gs_as32LHorizonCoef;
    ps32ChmPara = gs_as32CHorizonCoef;
    LoadFilterParam((HWC_FILTERPARA_S *)pVir,
                    ps32LumPara, HWC_FILTER_HORIZON_LUM_TAP,
                    ps32ChmPara, HWC_FILTER_HORIZON_CHM_TAP);

    /*加载垂直2阶滤波系数*/
    gs_u32VTap2FilterParamAddr = gs_u32HFilterParamAddr + HWC_HORIZON_PARA_SIZE_TOTAL;

    ps32LumPara = gs_as32LVerticalCoef;
    ps32ChmPara = gs_as32CVerticalCoefOfTap2;
    LoadFilterParam((HWC_FILTERPARA_S *)(pVir + HWC_HORIZON_PARA_SIZE_TOTAL),
                    ps32LumPara, HWC_FILTER_VERTICAL_LUM_TAP,
                    ps32ChmPara, HWC_FILTER_VERTICAL_CHM_TAP);

    /*加载垂直4阶滤波系数*/
    gs_u32VTap4FilterParamAddr = gs_u32VTap2FilterParamAddr + HWC_VERTICAL_PARA_SIZE_TOTAL;

    ps32LumPara = gs_as32LVerticalCoef;
    ps32ChmPara = gs_as32CVerticalCoefOfTap4;
    LoadFilterParam((HWC_FILTERPARA_S *)(pVir + HWC_HORIZON_PARA_SIZE_TOTAL + HWC_VERTICAL_PARA_SIZE_TOTAL),
                    ps32LumPara, HWC_FILTER_VERTICAL_LUM_TAP,
                    ps32ChmPara, HWC_FILTER_VERTICAL_CHM_TAP);

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

static inline HI_VOID InitNode(HWC_NODE_S* pstNode)
{
    HI_GFX_Memset(pstNode, 0, sizeof(HWC_NODE_S));

    /*Eanble list complete intterupt and err interrupts*/
    pstNode->unIntMask.stBits.bit1ListCompMask = 0x1;
    pstNode->unIntMask.stBits.bit1SlaveWriteErrMask = 0x1;
    pstNode->unIntMask.stBits.bit1DecWriteErrMask = 0x1;
    pstNode->unIntMask.stBits.bit1SlaveReadErrMask = 0x1;
    pstNode->unIntMask.stBits.bit1DecReadErrMask = 0x1;
    pstNode->unIntMask.stBits.bit1DieErrMask = 0x1;

#if HWC_VERSION == 0x400
    pstNode->unIntMask.stBits.bit1CfgErrMask = 0x1;
#endif

#if HWC_VERSION >= 0x200
    pstNode->u32CfgErrIntMask = 0xffff;
#endif

    return;
}

/*Map table of graphic layer fmt and HI_GFX2D_FMT*/
static HI_U32 gs_au32GLayerFmt[HI_GFX2D_FMT_PLANAR444 + 1] =
{
    0, 0, 0, 0, 0x5, 0, 0x8, 0x9, 0x2, 0x3, 0, 0, 0, 0, 0x4, 0, 0, 0, 0, 0, 0, 0, 0, 0x1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*Map table of video layer fmt and HI_GFX2D_FMT*/
static HI_U32 gs_au32VHDLayerFmt[HI_GFX2D_FMT_PLANAR444 + 1] =
{
    0, 0, 0, 0, 0x12, 0, 0xf, 0x10, 0xd, 0xe, 0, 0, 0, 0, 0x11, 0, 0, 0, 0, 0, 0, 0, 0xb, 0xc, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0, 0x1, 0x1, 0x2, 0x2, 0x3, 0x3, 0, 0, 0x4, 0x7, 0x6, 0x5,
    0x8, 0x9, 0xa
};

/*Map table of out layer fmt and HI_GFX2D_FMT*/
static HI_U32 gs_au32DstLayerFmt[HI_GFX2D_FMT_PLANAR444 + 1] =
{
    0, 0, 0, 0, 0, 0, 0x3, 0x4, 0, 0, 0, 0, 0, 0, 0x1, 0, 0, 0, 0, 0, 0, 0, 0, 0x2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x5, 0x5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
#if HWC_VERSION >= 0x200
    pstNode->unVhdCscParam0.stBits.bit15Param0 = 297 << 2;
    pstNode->unVhdCscParam0.stBits.bit15Param1 = 0 << 2;
    pstNode->unVhdCscParam1.stBits.bit15Param0 = 408 << 2;
    pstNode->unVhdCscParam1.stBits.bit15Param1 = 297 << 2;
    pstNode->unVhdCscParam2.stBits.bit15Param0 = -100 << 2;
    pstNode->unVhdCscParam2.stBits.bit15Param1 = -208 << 2;
    pstNode->unVhdCscParam3.stBits.bit15Param0 = 297 << 2;
    pstNode->unVhdCscParam3.stBits.bit15Param1 = 516 << 2;
    pstNode->unVhdCscParam4.stBits.bit15Param0 = 0 << 2;
#else
    pstNode->unVhdCscParam0.stBits.bit15Param0 = 297;
    pstNode->unVhdCscParam0.stBits.bit15Param1 = 0;
    pstNode->unVhdCscParam1.stBits.bit15Param0 = 408;
    pstNode->unVhdCscParam1.stBits.bit15Param1 = 297;
    pstNode->unVhdCscParam2.stBits.bit15Param0 = -100;
    pstNode->unVhdCscParam2.stBits.bit15Param1 = -208;
    pstNode->unVhdCscParam3.stBits.bit15Param0 = 297;
    pstNode->unVhdCscParam3.stBits.bit15Param1 = 516;
    pstNode->unVhdCscParam4.stBits.bit15Param0 = 0;
#endif

    return;
}

static inline HI_VOID SetOutCsc(HWC_NODE_S* pstNode)
{
    pstNode->unOutCscIdc.stBits.bit1IsEnable = 0x1;
    pstNode->unOutCscIdc.stBits.bit9Idc0 = 0;
    pstNode->unOutCscIdc.stBits.bit9Idc1 = 0;
    pstNode->unOutCscIdc.stBits.bit9Idc2 = 0;

    pstNode->unOutCscOdc.stBits.bit9Odc0 = 128;
    pstNode->unOutCscOdc.stBits.bit9Odc1 = 128;
    pstNode->unOutCscOdc.stBits.bit9Odc2 = 16;

#if HWC_VERSION >= 0x200
    pstNode->unOutCscParam0.stBits.bit15Param0 = 66 << 2;
    pstNode->unOutCscParam0.stBits.bit15Param1 = 129 << 2;
    pstNode->unOutCscParam1.stBits.bit15Param0 = 25 << 2;
    pstNode->unOutCscParam1.stBits.bit15Param1 = -38 << 2;
    pstNode->unOutCscParam2.stBits.bit15Param0 = -74 << 2;
    pstNode->unOutCscParam2.stBits.bit15Param1 = 112 << 2;
    pstNode->unOutCscParam3.stBits.bit15Param0 = 112 << 2;
    pstNode->unOutCscParam3.stBits.bit15Param1 = -94 << 2;
    pstNode->unOutCscParam4.stBits.bit15Param0 = -18 << 2;
#else
    pstNode->unOutCscParam0.stBits.bit15Param0 = 66;
    pstNode->unOutCscParam0.stBits.bit15Param1 = 129;
    pstNode->unOutCscParam1.stBits.bit15Param0 = 25;
    pstNode->unOutCscParam1.stBits.bit15Param1 = -38;
    pstNode->unOutCscParam2.stBits.bit15Param0 = -74;
    pstNode->unOutCscParam2.stBits.bit15Param1 = 112;
    pstNode->unOutCscParam3.stBits.bit15Param0 = 112;
    pstNode->unOutCscParam3.stBits.bit15Param1 = -94;
    pstNode->unOutCscParam4.stBits.bit15Param0 = -18;
#endif

    return;
}


static inline HI_VOID SetDst(HWC_NODE_S* pstNode, HI_GFX2D_SURFACE_S *pstDstSurface)
{
    pstNode->unOutCtl.stBits.bit1Enable = 0x1;
    pstNode->unOutCtl.stBits.bit8Fmt = gs_au32DstLayerFmt[pstDstSurface->enFmt];
    pstNode->unOutCtl.stBits.bit8AlphaThreshold = 0x80;
    pstNode->unOutCtl.stBits.bit1Dither = 0x0;
#if HWC_VERSION == 0x400
#ifdef CONFIG_GFX_MMU_SUPPORT
    pstNode->unOutCtl.stBits.bit1LumMmuBypass = 0x0;
    pstNode->unOutCtl.stBits.bit1ChormeMmuBypass = 0x0;
#else
    pstNode->unOutCtl.stBits.bit1LumMmuBypass = 0x1;
    pstNode->unOutCtl.stBits.bit1ChormeMmuBypass = 0x1;
#endif
    pstNode->unOutCtl.stBits.bit2CasProt = 0x2;
#else
    pstNode->unOutCtl.stBits.bit1WriteMode = 0x0;
#endif
    pstNode->u32OutPhyaddr = pstDstSurface->u32Phyaddr[0];
    pstNode->u32OutStride = pstDstSurface->u32Stride[0];
    pstNode->unOutRes.stBits.bit12Width = pstDstSurface->u32Width - 1;
    pstNode->unOutRes.stBits.bit12Height = pstDstSurface->u32Height - 1;

    if (0x5 == pstNode->unOutCtl.stBits.bit8Fmt)
    {
        SetOutCsc(pstNode);
        pstNode->u32OutChromPhyaddr = pstDstSurface->u32Phyaddr[1];;
        pstNode->u32OutStride |= (pstDstSurface->u32Stride[1] << 16);
    }

    if (pstDstSurface->stCompressInfo.enCompressType != HI_GFX2D_COMPRESS_NONE)
    {
#if HWC_VERSION == 0x400
        pstNode->unOutCtl.stBits.bit1CmpEn = 0x1;
        pstNode->unCmpGlbInfo.stBits.bit1IsLossLess = 0x1;
        pstNode->unCmpGlbInfo.stBits.bit1IsAlphaLossLess = 0x1;
        pstNode->unCmpGlbInfo.stBits.bit3PartitionNum = 0x1;
        //pstNode->unCmpGlbInfo.stBits.bit1Mode = 0x1;
        pstNode->unCmpReso.stBits.bit13Width = pstDstSurface->u32Width - 1;
        pstNode->unCmpReso.stBits.bit13Height = pstDstSurface->u32Height - 1;
#else
        pstNode->unCmpCtrl.stBits.bit1CmpEnable = HI_TRUE;
        pstNode->unCmpCtrl.stBits.bit1LossEnable = 0x0;
        pstNode->unCmpCtrl.stBits.bit1SgmtType = 0x1;
        pstNode->u32CmpDdr = 0x0;
        pstNode->u32CmpDataOffsetL = (((pstDstSurface->u32Width * 2 + 255) / 256 + 15) & 0xfffffff0) * pstDstSurface->u32Height;
        pstNode->u32CmpDataOffsetC = pstNode->u32CmpDataOffsetL;
        pstNode->u32OutChromPhyaddr = pstDstSurface->u32Phyaddr[0]
                                      + ((pstNode->u32CmpDataOffsetL + pstDstSurface->u32Stride[0] * pstDstSurface->u32Height + 15) & 0xfffffff0);
        pstNode->u32OutStride |= (pstDstSurface->u32Stride[0] << 16);

#endif
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
        return 1;
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
#if HWC_VERSION >= 0x200
    pstNode->u32HChromaCoefPhyaddr = pstNode->u32HCoefPhyaddr + HWC_HORIZON_LUM_PARA_SIZE_PERLEVEL;
#endif

    if (0x0 == u32ChromaTap)
    {
        pstNode->u32VCoefPhyaddr = gs_u32VTap4FilterParamAddr
                                   + GetFilterParaLevel(GFX2D_HAL_HWC_VSTEP_FLOATLEN, u32VStep) * HWC_VERTICAL_PARA_SIZE_PERLEVEL;
#if HWC_VERSION >= 0x200
        pstNode->u32VChromaCoefPhyaddr = pstNode->u32VCoefPhyaddr + HWC_VERTICAL_LUM_PARA_SIZE_PERLEVEL;
#endif
    }
    else
    {
        pstNode->u32VCoefPhyaddr = gs_u32VTap2FilterParamAddr
                                   + GetFilterParaLevel(GFX2D_HAL_HWC_VSTEP_FLOATLEN, u32VStep) * HWC_VERTICAL_PARA_SIZE_PERLEVEL;
#if HWC_VERSION >= 0x200
        pstNode->u32VChromaCoefPhyaddr = pstNode->u32VCoefPhyaddr + HWC_VERTICAL_LUM_PARA_SIZE_PERLEVEL;
#endif
    }

    return;
}

static inline HI_VOID SetGfxLayerZmeCoefAddr(HWC_NODE_S *pstNode, HI_U32 u32HStep, HI_U32 u32VStep, HI_U32 u32ChromaTap)
{
    pstNode->u32G0HCoefPhyaddr =  gs_u32HFilterParamAddr
                                  + GetFilterParaLevel(GFX2D_HAL_HWC_HSTEP_FLOATLEN, u32HStep) * HWC_HORIZON_PARA_SIZE_PERLEVEL;
    pstNode->u32G0HChromaCoefPhyaddr = pstNode->u32G0HCoefPhyaddr + HWC_HORIZON_LUM_PARA_SIZE_PERLEVEL;

    if (0x0 == u32ChromaTap)
    {
        pstNode->u32G0VCoefPhyaddr = gs_u32VTap4FilterParamAddr
                                     + GetFilterParaLevel(GFX2D_HAL_HWC_VSTEP_FLOATLEN, u32VStep) * HWC_VERTICAL_PARA_SIZE_PERLEVEL;
        pstNode->u32G0VChromaCoefPhyaddr = pstNode->u32G0VCoefPhyaddr + HWC_VERTICAL_LUM_PARA_SIZE_PERLEVEL;
    }
    else
    {
        pstNode->u32G0VCoefPhyaddr = gs_u32VTap2FilterParamAddr
                                     + GetFilterParaLevel(GFX2D_HAL_HWC_VSTEP_FLOATLEN, u32VStep) * HWC_VERTICAL_PARA_SIZE_PERLEVEL;
        pstNode->u32G0VChromaCoefPhyaddr = pstNode->u32G0VCoefPhyaddr + HWC_VERTICAL_LUM_PARA_SIZE_PERLEVEL;
    }

    return;
}

typedef struct
{
    HWC_VHDCTL_U unVhdCtl;
#if HWC_VERSION == 0x400
    HWC_GCTRL_U unCtrl;
#endif
    HWC_RES_U unRes;
    HWC_VHDHSP_U unVhdHsp;
    HWC_VHDHOFFSET_U unVhdLumHOffset;
    HWC_VHDHOFFSET_U unVhdChromaHOffset;
    HWC_VHDVSP_U unVhdVsp;
    HWC_RES_U unVhdZmeOutRes;
    HWC_RES_U unVhdZmeInRes;
    HI_U32 u32Vsr;
    HI_BOOL bFilter;
#if HWC_VERSION == 0x400
    HWC_CROPPOS_U unVhdCropPos;
    HWC_CROPRESO_U unVhdCropInReso;
    HWC_CROPRESO_U unVhdCropOutReso;
#endif
}GFX2D_HAL_ZME_S;

static HI_VOID GetZmeInfo(HWC_NODE_S *pstNode, HWC_LAYER_ID enLayerId, GFX2D_HAL_ZME_S *pstZme)
{
    if (enLayerId == HWC_LAYER_VHD)
    {
        pstZme->unVhdCtl.u32All = pstNode->unVhdCtl.u32All;
        pstZme->unRes.u32All = pstNode->unVhdInRes.u32All;
        pstZme->unVhdHsp.u32All = pstNode->unVhdHsp.u32All;
        pstZme->unVhdLumHOffset.u32All = pstNode->unVhdLumHOffset.u32All;
        pstZme->unVhdChromaHOffset.u32All = pstNode->unVhdChromaHOffset.u32All;
        pstZme->unVhdVsp.u32All = pstNode->unVhdVsp.u32All;
        pstZme->unVhdZmeOutRes.u32All = pstNode->unVhdZmeOutRes.u32All;
        pstZme->unVhdZmeInRes.u32All = pstNode->unVhdZmeInRes.u32All;
#if HWC_VERSION == 0x400
        pstZme->unVhdCropPos.u32All = pstNode->unVhdCropPos.u32All;
        pstZme->unVhdCropInReso.u32All = pstNode->unVhdCropInReso.u32All;
        pstZme->unVhdCropOutReso.u32All = pstNode->unVhdCropOutReso.u32All;
#endif
    }
#if HWC_VERSION == 0x400
    else if (enLayerId >= HWC_LAYER_G0)
    {
        HWC_GLAYER_S *pstLayer = &(pstNode->astGLayer[enLayerId - HWC_LAYER_G0]);

        pstZme->unCtrl.u32All = pstLayer->unCtrl.u32All;
        pstZme->unRes.u32All = pstLayer->unRes.u32All;
        pstZme->unVhdHsp.u32All = pstLayer->unG0Hsp.u32All;
        pstZme->unVhdLumHOffset.u32All = pstLayer->unLumHOffset.u32All;
        pstZme->unVhdChromaHOffset.u32All = pstLayer->unChromaHOffset.u32All;
        pstZme->unVhdVsp.u32All = pstLayer->unVsp.u32All;
        pstZme->unVhdZmeOutRes.u32All = pstLayer->unZmeOutRes.u32All;
        pstZme->unVhdZmeInRes.u32All = pstLayer->unZmeInRes.u32All;
        pstZme->unVhdCropPos.u32All = pstLayer->unCropPos.u32All;
        pstZme->unVhdCropInReso.u32All = pstLayer->unCropInReso.u32All;
        pstZme->unVhdCropOutReso.u32All = pstLayer->unCropOutReso.u32All;
    }
#endif
    return;
}

static HI_VOID SetZmeInfo(HWC_NODE_S *pstNode, HWC_LAYER_ID enLayerId, GFX2D_HAL_ZME_S *pstZme)
{
    if (enLayerId == HWC_LAYER_VHD)
    {
        pstNode->unVhdCtl.u32All = pstZme->unVhdCtl.u32All;
        pstNode->unVhdRes.u32All = pstZme->unRes.u32All;
        pstNode->unVhdHsp.u32All = pstZme->unVhdHsp.u32All;
        pstNode->unVhdLumHOffset.u32All = pstZme->unVhdLumHOffset.u32All;
        pstNode->unVhdChromaHOffset.u32All = pstZme->unVhdChromaHOffset.u32All;
        pstNode->unVhdVsp.u32All = pstZme->unVhdVsp.u32All;
        pstNode->u32VhdVsr = pstZme->u32Vsr;
        pstNode->u32VhdVOffset = 0x0;
        pstNode->unVhdZmeOutRes.u32All = pstZme->unVhdZmeOutRes.u32All;
        pstNode->unVhdZmeInRes.u32All = pstZme->unVhdZmeInRes.u32All;
#if HWC_VERSION == 0x400
        pstNode->unVhdCropPos.u32All = pstZme->unVhdCropPos.u32All;
        pstNode->unVhdCropInReso.u32All = pstZme->unVhdCropInReso.u32All;
        pstNode->unVhdCropOutReso.u32All = pstZme->unVhdCropOutReso.u32All;
#endif
        if (pstZme->bFilter)
        {
            SetVhdLayerZmeCoefAddr(pstNode, pstZme->unVhdHsp.stBits.bit24Ratio, pstZme->u32Vsr, pstZme->unVhdVsp.stBits.bit1ChromaTap);
        }
    }
#if HWC_VERSION == 0x400
    else if (enLayerId >= HWC_LAYER_G0)
    {
        HWC_GLAYER_S *pstLayer = &(pstNode->astGLayer[enLayerId - HWC_LAYER_G0]);

        pstLayer->unCtrl.u32All = pstZme->unCtrl.u32All;
        pstLayer->unRes.u32All = pstZme->unRes.u32All;
        pstLayer->unG0Hsp.u32All = pstZme->unVhdHsp.u32All;
        pstLayer->unLumHOffset.u32All = pstZme->unVhdLumHOffset.u32All;
        pstLayer->unChromaHOffset.u32All = pstZme->unVhdChromaHOffset.u32All;
        pstLayer->unVsp.u32All = pstZme->unVhdVsp.u32All;
        pstLayer->u32Vsr = pstZme->u32Vsr;
        pstLayer->u32VOffset = 0x0;
        pstLayer->unZmeOutRes.u32All = pstZme->unVhdZmeOutRes.u32All;
        pstLayer->unZmeInRes.u32All = pstZme->unVhdZmeInRes.u32All;
        pstLayer->unCropPos.u32All = pstZme->unVhdCropPos.u32All;
        pstLayer->unCropInReso.u32All = pstZme->unVhdCropInReso.u32All;
        pstLayer->unCropOutReso.u32All = pstZme->unVhdCropOutReso.u32All;

        if (pstZme->bFilter)
        {
            SetGfxLayerZmeCoefAddr(pstNode, pstZme->unVhdHsp.stBits.bit24Ratio, pstZme->u32Vsr, pstZme->unVhdVsp.stBits.bit1ChromaTap);
        }
    }
#endif
}

static HI_BOOL CheckExceedZmeBuf(HI_GFX2D_FMT_E enFmt, HI_U32 u32InWidth, HI_U32 u32OutWidth)
{
    return (((enFmt <= HI_GFX2D_FMT_BGRA8888) && (u32InWidth > 1920) && (u32OutWidth > 1920))
             || ((enFmt >= HI_GFX2D_FMT_YUV888) && (u32InWidth > 1920) && (u32OutWidth > 960)));
}

static inline HI_VOID SetZme(HWC_NODE_S* pstNode, HI_GFX2D_COMPOSE_S *pstCompose, HWC_LAYER_ID enLayerId)
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
        if (enLayerId == HWC_LAYER_VHD)
        {
            stZme.unVhdCtl.stBits.bit1ZmehDwsm = HI_TRUE;
            stZme.unVhdCtl.stBits.bit4ZmehDwsMode = HdwsmFactor - 1;
        }
#if HWC_VERSION == 0x400
        else
        {
            stZme.unCtrl.stBits.bit1ZmehDwsm = HI_TRUE;
            stZme.unCtrl.stBits.bit4ZmehDwsMode = HdwsmFactor - 1;
        }
#endif
    }
    else if (HI_TRUE == CheckExceedZmeBuf(pstSurface->enFmt, u32InWidth, pstCompose->stOutRect.u32Width))
    {
        u32InWidth = ((u32InWidth / 2) & ~(gs_stCapability.u8ZmeAlign - 1));
        u32HStep = (u32InWidth << GFX2D_HAL_HWC_HSTEP_FLOATLEN)
                   / pstCompose->stOutRect.u32Width;
        HdwsmFactor = 2;
        if (enLayerId == HWC_LAYER_VHD)
        {
            stZme.unVhdCtl.stBits.bit1ZmehDwsm = HI_TRUE;
            stZme.unVhdCtl.stBits.bit4ZmehDwsMode = 1;
        }
#if HWC_VERSION == 0x400
        else
        {
            stZme.unCtrl.stBits.bit1ZmehDwsm = HI_TRUE;
            stZme.unCtrl.stBits.bit4ZmehDwsMode = 1;
        }
#endif
    }

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

    stZme.unVhdZmeInRes.stBits.bit12Width = u32InWidth - 1;
    stZme.unVhdZmeInRes.stBits.bit12Height = u32InHeight - 1;
    stZme.unVhdZmeOutRes.stBits.bit12Width = pstCompose->stOutRect.u32Width - 1;
    stZme.unVhdZmeOutRes.stBits.bit12Height = pstCompose->stOutRect.u32Height - 1;
    stZme.unRes.stBits.bit12Width = u32InWidth * HdwsmFactor - 1;

    stZme.u32Vsr = u32VStep;
    stZme.bFilter = bFilter;

#if HWC_VERSION == 0x400
    if (pstCompose->stOpt.stClip.enClipMode == HI_GFX2D_CLIP_INSIDE)
    {
        stZme.unVhdCropPos.stBits.bit13XPos = pstCompose->stOpt.stClip.stClipRect.s32XPos;
        stZme.unVhdCropPos.stBits.bit13YPos = pstCompose->stOpt.stClip.stClipRect.s32YPos;
        stZme.unVhdCropInReso.stBits.bit13Width = pstCompose->stOutRect.u32Width - 1;
        stZme.unVhdCropInReso.stBits.bit13Height = pstCompose->stOutRect.u32Height - 1;
        stZme.unVhdCropOutReso.stBits.bit13Width = pstCompose->stOpt.stClip.stClipRect.u32Width - 1;
        stZme.unVhdCropOutReso.stBits.bit13Height = pstCompose->stOpt.stClip.stClipRect.u32Height - 1;
    }
    else
    {
        stZme.unVhdCropPos.stBits.bit13XPos = 0;
        stZme.unVhdCropPos.stBits.bit13YPos = 0;
        stZme.unVhdCropInReso.stBits.bit13Width = pstCompose->stOutRect.u32Width - 1;
        stZme.unVhdCropInReso.stBits.bit13Height = pstCompose->stOutRect.u32Height - 1;
        stZme.unVhdCropOutReso.stBits.bit13Width = pstCompose->stOutRect.u32Width - 1;
        stZme.unVhdCropOutReso.stBits.bit13Height = pstCompose->stOutRect.u32Height - 1;
    }
#endif

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
            /*图形层不支持缩放，则选择视频层*/
            if (!gs_stCapability.bGZmeSupport)
            {
                penLayerId[i] = HWC_LAYER_VHD;
            }
            else
            {
                penLayerId[i] = SelectZmeLayer(au8Ref);
                if (HWC_LAYER_BUTT == penLayerId[i])
                {
                    goto ERR;
                }
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
        else if (HI_TRUE == CheckSelectResultInvalid(penLayerId[i], pstCompose))
        {
            goto ERR;
        }
        else if ((penLayerId[i] == HWC_LAYER_VHD)
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
                                HWC_NODE_S* pstNode,
                                HI_GFX2D_COMPOSE_S *pstCompose)
{
    HI_U32 u32Phyaddr = 0;
    HI_U32 u32Stride = 0;
    HI_U32 u32SfPos = 0;
    HI_U32 u32DimValue = 0;
    HWC_GCTRL_U unCtrl;
    HWC_GCBMPARA_U unBmpPara;
    HWC_RES_U unRes;
    HWC_POS_U unOutStartPos;
    HWC_POS_U unOutEndPos;
    HI_U8 u8Bpp;
    HWC_GLAYER_S *pstGLayer;
    HI_GFX2D_SURFACE_S *pstSurface = &(pstCompose->stSurface);

    if (enLayerId < HWC_LAYER_G0)
    {
        return;
    }

    pstGLayer = &(pstNode->astGLayer[enLayerId - HWC_LAYER_G0]);

    unCtrl.u32All = pstGLayer->unCtrl.u32All;
    unBmpPara.u32All = pstGLayer->unBmpPara.u32All;
    unRes.u32All = pstGLayer->unRes.u32All;
    unOutStartPos.u32All = pstGLayer->unOutStartPos.u32All;
    unOutEndPos.u32All = pstGLayer->unOutEndPos.u32All;

    unCtrl.stBits.bit1IsEnable = 0x1;
    if (HI_GFX2D_SURFACE_TYPE_COLOR == pstSurface->enType)
    {
        unCtrl.stBits.bit1IsDim = 0x1;
        unCtrl.stBits.bit8Fmt = gs_au32GLayerFmt[pstSurface->enFmt];
        u32DimValue = pstSurface->u32Color;
        unBmpPara.stBits.bit1bPreMul = pstSurface->bPremulti;
        if (pstSurface->stAlphaExt.bExtAlpha)
        {
            unBmpPara.stBits.bit8Alpha0 = pstSurface->stAlphaExt.u8Alpha0;
            unBmpPara.stBits.bit8Alpha1 = pstSurface->stAlphaExt.u8Alpha1;
        }
        else
        {
            unBmpPara.stBits.bit8Alpha0 = 0x0;
            unBmpPara.stBits.bit8Alpha1 = 0xff;
        }

    }
    else
    {
        unCtrl.stBits.bit1IsDim = 0x0;
        unCtrl.stBits.bit8Fmt = gs_au32GLayerFmt[pstSurface->enFmt];
        u8Bpp = GetBpp(pstSurface->enFmt, HI_FALSE);
        u32Phyaddr = pstSurface->u32Phyaddr[0]
                     + pstCompose->stInRect.s32XPos * u8Bpp
                     + pstCompose->stInRect.s32YPos * pstSurface->u32Stride[0];
        u32Stride = pstSurface->u32Stride[0];
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
        else if (u8Bpp != 0)
        {
            u32SfPos = (u32Phyaddr % 16) / u8Bpp;
        }
        unBmpPara.stBits.bit1bPreMul = pstSurface->bPremulti;
        if (pstSurface->stAlphaExt.bExtAlpha)
        {
            unBmpPara.stBits.bit8Alpha0 = pstSurface->stAlphaExt.u8Alpha0;
            unBmpPara.stBits.bit8Alpha1 = pstSurface->stAlphaExt.u8Alpha1;
        }
        else
        {
            unBmpPara.stBits.bit8Alpha0 = 0x0;
            unBmpPara.stBits.bit8Alpha1 = 0xff;
        }
    }

    unCtrl.stBits.bit2ExpandMode = 0x0 ;

#if HWC_VERSION == 0x400
#ifdef CONFIG_GFX_MMU_SUPPORT
    unCtrl.stBits.bit1MmuBypass = 0x0;
#else
    unCtrl.stBits.bit1MmuBypass = 0x1;
#endif
#else
    unCtrl.stBits.bit1ReadMode = 0x0;
#endif

    if (pstCompose->stOpt.stBlend.bGlobalAlphaEnable)
    {
        unBmpPara.stBits.bit8GlobalAlpha = pstCompose->stOpt.stBlend.u8GlobalAlpha;
    }
    else
    {
        unBmpPara.stBits.bit8GlobalAlpha = 0xff;
    }
    unBmpPara.stBits.bit1PixelAlphaEnable = pstCompose->stOpt.stBlend.bPixelAlphaEnable;
    unBmpPara.stBits.bit1bCovBlend = pstCompose->stOpt.stBlend.bCovBlend;

    if (HI_GFX2D_SURFACE_TYPE_COLOR == pstSurface->enType)
    {
        unRes.stBits.bit12Width = pstCompose->stOutRect.u32Width - 1;
        unRes.stBits.bit12Height = pstCompose->stOutRect.u32Height - 1;
    }
    else
    {
        unRes.stBits.bit12Width = pstCompose->stInRect.u32Width - 1;
        unRes.stBits.bit12Height = pstCompose->stInRect.u32Height - 1;
    }
    unOutStartPos.stBits.bit12X = pstCompose->stOutRect.s32XPos;
    unOutStartPos.stBits.bit12Y = pstCompose->stOutRect.s32YPos;
    unOutEndPos.stBits.bit12X = pstCompose->stOutRect.s32XPos
                                + pstCompose->stOutRect.u32Width - 1;
    unOutEndPos.stBits.bit12Y = pstCompose->stOutRect.s32YPos
                                + pstCompose->stOutRect.u32Height - 1;

    pstGLayer->unCtrl.u32All = unCtrl.u32All;
    pstGLayer->u32Phyaddr = u32Phyaddr;
    pstGLayer->u32Stride = u32Stride;
    pstGLayer->u32SfPos = u32SfPos;
    pstGLayer->u32DimValue = u32DimValue;
    pstGLayer->unBmpPara.u32All = unBmpPara.u32All;
    pstGLayer->unRes.u32All = unRes.u32All;
    pstGLayer->unOutStartPos.u32All = unOutStartPos.u32All;
    pstGLayer->unOutEndPos.u32All = unOutEndPos.u32All;

    if (HI_GFX2D_SURFACE_TYPE_COLOR != pstSurface->enType)
    {
        SetZme(pstNode, pstCompose, enLayerId);
    }

    return;
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

    return;
}


static inline HI_VOID SetVHDLayer(HWC_LAYER_ID enLayerId,
                                  HWC_NODE_S* pstNode,
                                  HI_GFX2D_COMPOSE_S *pstCompose)
{
    HI_U8 u8Bpp;
    HI_U8 u8HorizonSampleFactor = 1;
    HI_U8 u8VerticalSampleFactor = 1;

    HI_GFX2D_SURFACE_S *pstSurface = &(pstCompose->stSurface);

    pstNode->unVhdCtl.stBits.bit1IsEnable = HI_TRUE;
    pstNode->unVhdCtl.stBits.bit2ExpandMode = 0x0;
    if ((HI_GFX2D_FMT_SEMIPLANAR422VU_H == pstSurface->enFmt)
        || (HI_GFX2D_FMT_SEMIPLANAR422VU_V == pstSurface->enFmt)
        || (HI_GFX2D_FMT_SEMIPLANAR420VU == pstSurface->enFmt))
    {
        pstNode->unVhdCtl.stBits.bit1UVOrder = 0x1;
    }

#if HWC_VERSION <= 0x300
    pstNode->unVhdCtl.stBits.bit2LumReadMode = 0x1;
    pstNode->unVhdCtl.stBits.bit2ChromaReadMode = 0x1;
#endif
    pstNode->unVhdCtl.stBits.bit5Fmt = gs_au32VHDLayerFmt[pstSurface->enFmt];

#if HWC_VERSION == 0x400
#ifdef CONFIG_GFX_MMU_SUPPORT
    pstNode->unVhdCtl.stBits.bit1LumMmuBypass = 0x0;
    pstNode->unVhdCtl.stBits.bit1ChromaMmuBypass = 0x0;
#else
    pstNode->unVhdCtl.stBits.bit1LumMmuBypass = 0x1;
    pstNode->unVhdCtl.stBits.bit1ChromaMmuBypass = 0x1;
#endif
#endif

    u8Bpp = GetBpp(pstSurface->enFmt, HI_TRUE);
    pstNode->u32VhdYPhyaddr = pstSurface->u32Phyaddr[0]
                              + pstCompose->stInRect.s32YPos * pstSurface->u32Stride[0]
                              + pstCompose->stInRect.s32XPos * u8Bpp;

    u8Bpp = GetBpp(pstSurface->enFmt, HI_FALSE);

    GetSampleFactor(pstSurface->enFmt, &u8HorizonSampleFactor, &u8VerticalSampleFactor);

    if ((pstSurface->enFmt >= HI_GFX2D_FMT_SEMIPLANAR420UV)
        && (HI_GFX2D_FMT_PLANAR400 != pstSurface->enFmt))
    {
        pstNode->u32VhdCbPhyaddr = pstSurface->u32Phyaddr[1]
                                   + pstCompose->stInRect.s32YPos / u8VerticalSampleFactor * pstSurface->u32Stride[1]
                                   + pstCompose->stInRect.s32XPos / u8HorizonSampleFactor * u8Bpp;
    }

    if ((pstSurface->enFmt >= HI_GFX2D_FMT_PLANAR420)
        && (HI_GFX2D_FMT_PLANAR400 != pstSurface->enFmt))
    {
        pstNode->u32VhdCrPhyaddr = pstSurface->u32Phyaddr[2]
                                   + pstCompose->stInRect.s32YPos / u8VerticalSampleFactor * pstSurface->u32Stride[2]
                                   + pstCompose->stInRect.s32XPos / u8HorizonSampleFactor * u8Bpp;
    }

    pstNode->u32VhdYSride = pstSurface->u32Stride[0]
                            | (pstSurface->u32Stride[1] << 16);
    pstNode->u32VhdCrStride = pstSurface->u32Stride[2];
    pstNode->unCropStartPos.stBits.bit12X = 0;
    pstNode->unCropStartPos.stBits.bit12Y = 0;
    pstNode->unCropEndPos.stBits.bit12X = pstCompose->stInRect.u32Width - 1;
    pstNode->unCropEndPos.stBits.bit12Y = pstCompose->stInRect.u32Height - 1;
    pstNode->unVhdRes.stBits.bit12Width = pstCompose->stInRect.u32Width - 1;
    pstNode->unVhdRes.stBits.bit12Height = pstCompose->stInRect.u32Height - 1;
    pstNode->unVhdInRes.stBits.bit12Width = pstCompose->stInRect.u32Width - 1;
    pstNode->unVhdInRes.stBits.bit12Height = pstCompose->stInRect.u32Height - 1;
    if (pstCompose->stOpt.stBlend.bGlobalAlphaEnable)
    {
        pstNode->unVhdCbmPara.stBits.bit8GlobalAlpha = pstCompose->stOpt.stBlend.u8GlobalAlpha;
    }
    else
    {
        pstNode->unVhdCbmPara.stBits.bit8GlobalAlpha = 0xff;
    }
    pstNode->unVhdCbmPara.stBits.bit1bCovBlend = pstCompose->stOpt.stBlend.bCovBlend;
    pstNode->unVhdCbmPara.stBits.bit1PixelAlphaEnable = pstCompose->stOpt.stBlend.bPixelAlphaEnable;
    pstNode->unVhdCbmPara.stBits.bit1bPreMul = pstSurface->bPremulti;
    if (pstSurface->stAlphaExt.bExtAlpha)
    {
        pstNode->unVhdCbmPara.stBits.bit8Alpha0 = pstSurface->stAlphaExt.u8Alpha0;
        pstNode->unVhdCbmPara.stBits.bit8Alpha1 = pstSurface->stAlphaExt.u8Alpha1;
    }
    else
    {
        pstNode->unVhdCbmPara.stBits.bit8Alpha0 = 0x0;
        pstNode->unVhdCbmPara.stBits.bit8Alpha1 = 0xff;
    }
    pstNode->unOutStartPos.stBits.bit12X = pstCompose->stOutRect.s32XPos;
    pstNode->unOutStartPos.stBits.bit12Y = pstCompose->stOutRect.s32YPos;
    pstNode->unOutEndPos.stBits.bit12X = pstCompose->stOutRect.s32XPos
                                         + pstCompose->stOutRect.u32Width - 1;
    pstNode->unOutEndPos.stBits.bit12Y = pstCompose->stOutRect.s32YPos
                                         + pstCompose->stOutRect.u32Height - 1;
    if (pstSurface->enFmt >= HI_GFX2D_FMT_YUYV422)
    {
        SetInCsc(pstNode);
    }
    SetZme(pstNode, pstCompose, enLayerId);

    return;
}

static HI_VOID SetSrc(HWC_NODE_S* pstNode, HI_GFX2D_COMPOSE_S *pstCompose,
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
            pstNode->unCbmMix.stBits.bit3Prio0 = enLayerId;
            break;
        }
        case 1:
        {
            pstNode->unCbmMix.stBits.bit3Prio1 = enLayerId;
            break;
        }
        case 2:
        {
            pstNode->unCbmMix.stBits.bit3Prio2 = enLayerId;
            break;
        }
        case 3:
        {
            pstNode->unCbmMix.stBits.bit3Prio3 = enLayerId;
            break;
        }
        case 4:
        {
            pstNode->unCbmMix.stBits.bit3Prio4 = enLayerId;
            break;
        }
        case 5:
        {
            pstNode->unCbmMix.stBits.bit3Prio5 = enLayerId;
            break;
        }
        case 6:
        {
            pstNode->unCbmMix.stBits.bit3Prio6 = enLayerId;
            break;
        }
    }
    return;
}

static inline HI_VOID SetBgColor(HWC_NODE_S* pstNode, HI_U32 u32BgColor)
{
    pstNode->u32CbmBkgValue = u32BgColor;

    return;
}

HI_VOID PrintNodeInfo(HWC_NODE_S* pstNode)
{
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_INTMASK\t\t0x%08x\n" ,
                         pstNode->unIntMask.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDHCOEFADDR\t0x%08x\n" ,
                         pstNode->u32HCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDVCOEFADDR\t0x%08x\n" ,
                         pstNode->u32VCoefPhyaddr);
#if HWC_VERSION >= 0x200
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDHCHROMACOEFADDR\t0x%08x\n" ,
                         pstNode->u32HChromaCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDVCHROMACOEFADDR\t0x%08x\n" ,
                         pstNode->u32VChromaCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CFGERRINTMASK\t0x%08x\n" ,
                         pstNode->u32CfgErrIntMask);
#if HWC_VERSION == 0x400
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "G0HCOEFADDR\t0x%08x\n" ,
                         pstNode->u32G0HCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "G0VCOEFADDR\t0x%08x\n" ,
                         pstNode->u32G0VCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "G0HCHROMACOEFADDR\t0x%08x\n" ,
                         pstNode->u32G0HChromaCoefPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "G0VCHROMACOEFADDR\t0x%08x\n" ,
                         pstNode->u32G0VChromaCoefPhyaddr);
#endif
#endif
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCTRL\t\t0x%08x\n" ,
                         pstNode->unVhdCtl.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDADDR\t\t0x%08x\n" ,
                         pstNode->u32VhdYPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCADDR\t0x%08x\n" ,
                         pstNode->u32VhdCbPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCRADDR\t0x%08x\n" ,
                         pstNode->u32VhdCrPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDSTRIDE\t0x%08x\n" ,
                         pstNode->u32VhdYSride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSTRIDE\t0x%08x\n" ,
                         pstNode->u32VhdCrStride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCFPOS\t0x%08x\n" ,
                         pstNode->unCropStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCLPOS\t0x%08x\n" ,
                         pstNode->unCropEndPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDSRCRESO\t0x%08x\n" ,
                         pstNode->unVhdRes.u32All);
#if HWC_VERSION <= 0x300
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDDRAWMODE\t0x%08x\n" ,
                         pstNode->unVhdDrawMode.u32All);
#endif
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDIRESO\t0x%08x\n" ,
                         pstNode->unVhdInRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCBMPARA\t0x%08x\n" ,
                         pstNode->unVhdCbmPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDDFPOS\t0x%08x\n" ,
                         pstNode->unOutStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDDLPOS\t0x%08x\n" ,
                         pstNode->unOutEndPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCIDC\t0x%08x\n" ,
                         pstNode->unVhdCscIdc.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCODC\t0x%08x\n" ,
                         pstNode->unVhdCscOdc.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCP0\t0x%08x\n" ,
                         pstNode->unVhdCscParam0.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCP1\t0x%08x\n" ,
                         pstNode->unVhdCscParam1.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCP2\t0x%08x\n" ,
                         pstNode->unVhdCscParam2.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCP3\t0x%08x\n" ,
                         pstNode->unVhdCscParam3.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCSCP4\t0x%08x\n" ,
                         pstNode->unVhdCscParam4.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDHSP\t\t0x%08x\n" ,
                         pstNode->unVhdHsp.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDHLOFFSET\t0x%08x\n" ,
                         pstNode->unVhdLumHOffset.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDHCOFFSET\t0x%08x\n" ,
                         pstNode->unVhdChromaHOffset.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDVSP\t\t0x%08x\n" ,
                         pstNode->unVhdVsp.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDVSR\t\t0x%08x\n" ,
                         pstNode->u32VhdVsr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDVOFFSET\t0x%08x\n" ,
                         pstNode->u32VhdVOffset);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDZMEORESO\t0x%08x\n" ,
                         pstNode->unVhdZmeOutRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDZMEIRESO\t0x%08x\n" ,
                         pstNode->unVhdZmeInRes.u32All);
#if HWC_VERSION == 0x400
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCROPPOS\t0x%08x\n" ,
                         pstNode->unVhdCropPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCROPINRESO\t0x%08x\n" ,
                         pstNode->unVhdCropInReso.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VHDCROPOUTRESO\t0x%08x\n" ,
                         pstNode->unVhdCropOutReso.u32All);
#endif
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0CTRL\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].unCtrl.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0ADDR\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].u32Phyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0STRIDE\t0x%08x\n" ,
                         pstNode->astGLayer[0].u32Stride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0SFPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].u32SfPos);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0DIMPARA\t0x%08x\n" ,
                         pstNode->astGLayer[0].u32DimValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0CBMPARA\t0x%08x\n" ,
                         pstNode->astGLayer[0].unBmpPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0IRESO\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].unRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0DFPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].unOutStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0DLPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].unOutEndPos.u32All);
#if HWC_VERSION == 0x400
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G0HSP\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].unG0Hsp.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_LHOFFSET\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].unLumHOffset.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CHOFFSET\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].unChromaHOffset.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VSP\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].unVsp.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VSR\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].u32Vsr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_VOFFSET\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].u32VOffset);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_ZMEOUTRES\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].unZmeOutRes);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_ZMEINRES\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].unZmeInRes);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CROPPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].unCropPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CROPINRESO\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].unCropInReso.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CROPOUTRESO\t\t0x%08x\n" ,
                         pstNode->astGLayer[0].unCropOutReso.u32All);
#endif
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1CTRL\t\t0x%08x\n" ,
                         pstNode->astGLayer[1].unCtrl.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1ADDR\t\t0x%08x\n" ,
                         pstNode->astGLayer[1].u32Phyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1STRIDE\t0x%08x\n" ,
                         pstNode->astGLayer[1].u32Stride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1SFPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[1].u32SfPos);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1DIMPARA\t0x%08x\n" ,
                         pstNode->astGLayer[1].u32DimValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1CBMPARA\t0x%08x\n" ,
                         pstNode->astGLayer[1].unBmpPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1IRESO\t\t0x%08x\n" ,
                         pstNode->astGLayer[1].unRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1DFPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[1].unOutStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G1DLPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[1].unOutEndPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2CTRL\t\t0x%08x\n" ,
                         pstNode->astGLayer[2].unCtrl.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2ADDR\t\t0x%08x\n" ,
                         pstNode->astGLayer[2].u32Phyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2STRIDE\t0x%08x\n" ,
                         pstNode->astGLayer[2].u32Stride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2SFPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[2].u32SfPos);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2DIMPARA\t0x%08x\n" ,
                         pstNode->astGLayer[2].u32DimValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2CBMPARA\t0x%08x\n" ,
                         pstNode->astGLayer[2].unBmpPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2IRESO\t\t0x%08x\n" ,
                         pstNode->astGLayer[2].unRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2DFPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[2].unOutStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G2DLPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[2].unOutEndPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3CTRL\t\t0x%08x\n" ,
                         pstNode->astGLayer[3].unCtrl.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3ADDR\t\t0x%08x\n" ,
                         pstNode->astGLayer[3].u32Phyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3STRIDE\t0x%08x\n" ,
                         pstNode->astGLayer[3].u32Stride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3SFPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[3].u32SfPos);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3DIMPARA\t0x%08x\n" ,
                         pstNode->astGLayer[3].u32DimValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3CBMPARA\t0x%08x\n" ,
                         pstNode->astGLayer[3].unBmpPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3IRESO\t\t0x%08x\n" ,
                         pstNode->astGLayer[3].unRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3DFPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[3].unOutStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G3DLPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[3].unOutEndPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4CTRL\t\t0x%08x\n" ,
                         pstNode->astGLayer[4].unCtrl.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4ADDR\t\t0x%08x\n" ,
                         pstNode->astGLayer[4].u32Phyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4STRIDE\t0x%08x\n" ,
                         pstNode->astGLayer[4].u32Stride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4SFPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[4].u32SfPos);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4DIMPARA\t0x%08x\n" ,
                         pstNode->astGLayer[4].u32DimValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4CBMPARA\t0x%08x\n" ,
                         pstNode->astGLayer[4].unBmpPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4IRESO\t\t0x%08x\n" ,
                         pstNode->astGLayer[4].unRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4DFPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[4].unOutStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G4DLPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[4].unOutEndPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5CTRL\t\t0x%08x\n" ,
                         pstNode->astGLayer[5].unCtrl.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5ADDR\t\t0x%08x\n" ,
                         pstNode->astGLayer[5].u32Phyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5STRIDE\t0x%08x\n" ,
                         pstNode->astGLayer[5].u32Stride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5SFPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[5].u32SfPos);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5DIMPARA\t0x%08x\n" ,
                         pstNode->astGLayer[5].u32DimValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5CBMPARA\t0x%08x\n" ,
                         pstNode->astGLayer[5].unBmpPara.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5IRESO\t\t0x%08x\n" ,
                         pstNode->astGLayer[5].unRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5DFPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[5].unOutStartPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_G5DLPOS\t\t0x%08x\n" ,
                         pstNode->astGLayer[5].unOutEndPos.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CBMBKGV\t\t0x%08x\n" ,
                         pstNode->u32CbmBkgValue);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CBMMIX\t\t0x%08x\n" ,
                         pstNode->unCbmMix.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCTRL\t\t0x%08x\n" ,
                         pstNode->unOutCtl.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTADDR\t\t0x%08x\n" ,
                         pstNode->u32OutPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTADDR_C\t\t0x%08x\n" ,
                         pstNode->u32OutChromPhyaddr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTSTRIDE\t0x%08x\n" ,
                         pstNode->u32OutStride);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTRESO\t\t0x%08x\n" ,
                         pstNode->unOutRes.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCIDC\t0x%08x\n" ,
                         pstNode->unOutCscIdc.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCODC\t0x%08x\n" ,
                         pstNode->unOutCscOdc.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCPARAM0\t0x%08x\n" ,
                         pstNode->unOutCscParam0.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCPARAM1\t0x%08x\n" ,
                         pstNode->unOutCscParam1.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCPARAM2\t0x%08x\n" ,
                         pstNode->unOutCscParam2.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCPARAM3\t0x%08x\n" ,
                         pstNode->unOutCscParam3.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_OUTCSCPARAM4\t0x%08x\n" ,
                         pstNode->unOutCscParam4.u32All);
#if HWC_VERSION == 0x400
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPGLBINFO\t0x%08x\n" ,
                         pstNode->unCmpGlbInfo.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPRESO\t0x%08x\n" ,
                         pstNode->unCmpReso.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG0\t0x%08x\n" ,
                         pstNode->unCmpCfg0.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG1\t0x%08x\n" ,
                         pstNode->unCmpCfg1.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG2\t0x%08x\n" ,
                         pstNode->unCmpCfg2.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG3\t0x%08x\n" ,
                         pstNode->unCmpCfg3.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG4\t0x%08x\n" ,
                         pstNode->unCmpCfg4.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG5\t0x%08x\n" ,
                         pstNode->unCmpCfg5.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG6\t0x%08x\n" ,
                         pstNode->unCmpCfg6.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG7\t0x%08x\n" ,
                         pstNode->unCmpCfg7.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG8\t0x%08x\n" ,
                         pstNode->unCmpCfg8.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCFG9\t0x%08x\n" ,
                         pstNode->unCmpCfg9.u32All);
    //HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID,"HWC_CMPMAXROWLEN\t0x%08x\n" ,
    //               pstNode->u32CmpMaxRowLen);
#else
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPCTRL\t0x%08x\n" ,
                         pstNode->unCmpCtrl.u32All);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPDDR\t0x%08x\n" ,
                         pstNode->u32CmpDdr);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPOFFSETL\t0x%08x\n" ,
                         pstNode->u32CmpDataOffsetL);
    HI_GFX_COMM_LOG_INFO(HIGFX_GFX2D_ID, "HWC_CMPOFFSETC\t0x%08x\n" ,
                         pstNode->u32CmpDataOffsetC);
#endif
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
    gs_pstHWCReg->u32AqListHeadId = pstNode->u32NodeId;

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
    //pstCurNode->pNextNode = pstNextNode;
    pstCurNode->u32NextNodeId = pstNextNode->u32NextNodeId;

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

    while (pstCurNode != NULL)
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
#if HWC_VERSION >= 0x200
    HI_U32 u32ErrIntStatus = gs_pstHWCReg->u32MaskErrInt;
#endif
    HI_U32 u32IntStatus = gs_pstHWCReg->u32MaskInt;

#if HWC_VERSION >= 0x200
    gs_pstHWCReg->u32MaskErrInt = u32ErrIntStatus;

    if (u32ErrIntStatus & 0xffff)
    {
        HI_GFX_COMM_LOG_WARNING(HIGFX_GFX2D_ID, "Cfg err:%x\n", u32ErrIntStatus);
    }
#endif

    /*clear interrupt*/
    gs_pstHWCReg->u32MaskInt = u32IntStatus;

    if (u32IntStatus & 0x100)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Fatal err!\n");
    }

    if (u32IntStatus & 0x80)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Dec read err!\n");
    }

    if (u32IntStatus & 0x40)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Slv read err!\n");
    }

    if (u32IntStatus & 0x20)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Dec write err!\n");
    }

    if (u32IntStatus & 0x10)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Slv write err!\n");
    }

    return u32IntStatus;
}

HI_U32 GFX2D_HAL_HWC_GetIsrNum(HI_VOID)
{
    return HWC_ISRNUM;
}

HI_U32 GFX2D_HAL_HWC_GetBaseAddr(HI_VOID)
{
    return HWC_REG_BASE_ADDR;
}

#ifndef GFX2D_CHECK_UNSUPPORT
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
            HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "unsupport!\n");
            return HI_ERR_GFX2D_UNSUPPORT;
        }
        pstCompose->stSurface.u32Stride[0] *= u32Factor;

        if (pstCompose->stSurface.enFmt >= HI_GFX2D_FMT_SEMIPLANAR420UV)
        {
            if (pstCompose->stSurface.u32Stride[1] * u32Factor > gs_stCapability.u32MaxStride)
            {
                HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "unsupport!\n");
                return HI_ERR_GFX2D_UNSUPPORT;
            }
            pstCompose->stSurface.u32Stride[1] *= u32Factor;
        }

        if (pstCompose->stSurface.enFmt >= HI_GFX2D_FMT_PLANAR420)
        {
            if (pstCompose->stSurface.u32Stride[2] * u32Factor > gs_stCapability.u32MaxStride)
            {
                HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "unsupport!\n");
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
#endif

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
        SEQ_Printf(p, "Status\t:%s\n", (gs_pstHWCReg->u32Status & 0x1) ? "busy" : "idle");
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
