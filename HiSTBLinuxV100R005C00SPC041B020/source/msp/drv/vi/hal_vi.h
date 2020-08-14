#ifndef __HAL_VI_H__
#define __HAL_VI_H__

#include "hi_unf_video.h"

#include "drv_vi_reg_hi3716cv200.h"

#define VI_REG_BASE (0xF8CA0000)
#define VI_INT_NUM (89 + 32)

#define VI_PT_DIFF 0x100
#define VI_CH_DIFF 0x1000

#define VI_MASTER_NUM 1
#define VI_PT_NUM 1
#define VI_CH_NUM 1

typedef struct hiSIZE_S
{
    HI_U32 u32Width;
    HI_U32 u32Height;
} SIZE_S;

typedef struct hi_RECT_S
{
    HI_S32 s32X;
    HI_S32 s32Y;
    HI_U32 u32Width;
    HI_U32 u32Height;
} RECT_S;

typedef struct hiVI_DRV_TIMING_BLANK
{
    HI_U32 u32HsyncHfb;
    HI_U32 u32HsyncAct;
    HI_U32 u32HsyncHbb;
    HI_U32 u32VsyncOfb;
    HI_U32 u32VsyncOAct;
    HI_U32 u32VsyncObb;
    HI_U32 u32VsyncEfb;
    HI_U32 u32VsyncEAct;
    HI_U32 u32VsyncEbb;
} VI_DRV_TIMING_BLANK;

typedef struct hiVI_DRV_BT656_CFG
{
    HI_U32  u32Mode;    /* 0:10bit 1:20bit 2:30bit */
    HI_BOOL bVsyncInv;
    HI_BOOL bHsyncInv;
    HI_BOOL bFieldInv;
    HI_BOOL bEn;
} VI_DRV_BT656_CFG;

typedef struct hiVI_DRV_CHN_STORE_INFO
{
    HI_U32                u32Width;
    HI_U32                u32Height;
    HI_UNF_VIDEO_FORMAT_E enPixFormat;
    HI_U32                u32BitWidth;
    HI_U32                u32Stride;
    HI_BOOL               bProgressive;
} VI_DRV_CHN_STORE_INFO;

typedef struct hiVI_DRV_CHN_ZME_INFO
{
    SIZE_S                stSrcSize;
    SIZE_S                stDstSize;
    HI_UNF_VIDEO_FORMAT_E enSrcPixFormat;
    HI_UNF_VIDEO_FORMAT_E enDstPixFormat;
} VI_DRV_CHN_ZME_INFO;

typedef struct hiVI_DRV_PORT_MSK
{
    HI_U32 u32RMsk;
    HI_U32 u32GMsk;
    HI_U32 u32BMsk;

    HI_U32 u32RRev;
    HI_U32 u32GRev;
    HI_U32 u32BRev;

    HI_U32 u32ROff;
    HI_U32 u32GOff;
    HI_U32 u32BOff;
} VI_DRV_PORT_MSK;

typedef struct hiVI_DRV_CHN_LHFIR_SPH
{
    HI_U32 hlmsc_en;
    HI_U32 hlmid_en;
    HI_U32 hlfir_en;
    HI_U32 hlratio;
} VI_DRV_CHN_LHFIR_SPH;

typedef struct hiVI_DRV_CHN_CHFIR_SPH
{
    HI_U32 hchmsc_en;
    HI_U32 hchmid_en;
    HI_U32 hchfir_en;
    HI_U32 hchratio;
} VI_DRV_CHN_CHFIR_SPH;

typedef struct hiVI_DRV_PORT_UNIFY_DATA
{
    HI_U32 enable;
    HI_U32 uv_seq;
    HI_U32 yc_seq;
    HI_U32 comp_num;
} VI_DRV_PORT_UNIFY_DATA;

typedef struct hiVI_DRV_PORT_UNIFY_TIMING
{
    HI_U32 field_inv;
    HI_U32 field_sel;
    HI_U32 vsync_mode;
    HI_U32 vsync_inv;
    HI_U32 vsync_sel;
    HI_U32 hsync_mode;
    HI_U32 hsync_and;
    HI_U32 hsync_inv;
    HI_U32 hsync_sel;
    HI_U32 de_inv;
    HI_U32 de_sel;
} VI_DRV_PORT_UNIFY_TIMING;

typedef struct hiVI_DRV_PORT_GEN_DATA_COEF
{
    HI_U32 inc_frame;
    HI_U32 step_frame;
    HI_U32 inc_space;
    HI_U32 step_space;
} VI_DRV_PORT_GEN_DATA_COEF;

typedef struct hiVI_DRV_PORT_GEN_DATA
{
    HI_U32 enable;
    HI_U32 data_move[3];
    HI_U32 vsync_reset;
    HI_U32 hsync_reset;
    HI_U32 vsync_move;
    HI_U32 hsync_move;
    HI_U32 de_move;
} VI_DRV_PORT_GEN_DATA;

typedef enum
{
    VI_PT_HEIGHTERR_INT = 0x00000004,
    VI_PT_WIDTHERR_INT   = 0x00000002,
    VI_PT_FRAMEPULSE_INT = 0x00000001,
} VI_DRV_PT_INT_t;

typedef enum
{
    VI_BUSERRY_INT   = 0x00000040,
    VI_BUSERRC_INT   = 0x00000020,
    VI_UPDATECFG_INT = 0x00000010,
    VI_DATATHROW_INT = 0x00000008,
    VI_BUFOVF_INT = 0x00000004,
    VI_CC_INT = 0x00000002,
    VI_FRAMEPULSE_INT = 0x00000001,
} VI_DRV_CH_INT_t;

HI_U32 ViRegRead(HI_U32 a);
HI_VOID ViRegWrite(HI_U32 a, HI_U32 b);
HI_VOID VI_DRV_SetMasterOtd(HI_U32 u32MasterId, HI_U32 u32Otd);
HI_U32 VI_DRV_GetVicapInt(HI_VOID);

HI_VOID VI_DRV_SetWorkMode(HI_BOOL bLowDelay, HI_BOOL bInnerClk);
HI_VOID VI_DRV_SetVicapIntMask(HI_U32 int_pt0_en, HI_U32 int_ch0_en);

HI_VOID VI_DRV_SetApbTimeout(HI_BOOL bEn, HI_U32 timeout);
HI_VOID VI_DRV_SetPtEn(HI_U32 PtId, HI_BOOL bEn);
HI_U32  VI_DRV_GetPtStatus(HI_U32 PtId);
HI_U32  VI_DRV_GetPtBT656Status(HI_U32 PtId);
HI_VOID VI_DRV_SetPtSize(HI_U32 PtId, HI_U32 Width, HI_U32 Height);
HI_VOID VI_DRV_SetPtIntMask(HI_U32 PtId, HI_U32 fstart_en, HI_U32 width_err_en, HI_U32 height_err_en);
HI_VOID VI_DRV_ClrPtIntStatus(HI_U32 PtId, HI_U32 u32ClrInt);
HI_U32  VI_DRV_GetPtIntStatus(HI_U32 PtId);
HI_VOID VI_DRV_SetPtTimingMode(HI_U32 PtId, HI_U32 u32Mode);
HI_VOID VI_DRV_SetPtMask(HI_U32 PtId, VI_DRV_PORT_MSK * pstMsk);
HI_VOID VI_DRV_SetPtGenTiming(HI_U32 PtId, HI_BOOL bEn, HI_BOOL bVsyncMode, HI_BOOL bHsyncMode);
HI_VOID VI_DRV_SetPtGenData(HI_U32 PtId, VI_DRV_PORT_GEN_DATA * pstgendat);
HI_VOID VI_DRV_SetPtGenDataCoef(HI_U32 PtId, VI_DRV_PORT_GEN_DATA_COEF * pstgendatcoef);
HI_VOID VI_DRV_SetPtGenDataInit(HI_U32 PtId, HI_U32 data[3]);
HI_VOID VI_DRV_SetPtYUV444(HI_U32 PtId, HI_BOOL bEn);
HI_VOID VI_DRV_SetPtFstartDly(HI_U32 PtId, HI_U32 fstartdly);
HI_VOID VI_DRV_SetPtIntf(HI_U32 PtId, VI_DRV_TIMING_BLANK * pstIntfCfg);
HI_VOID VI_DRV_SetPtTimingBT656(HI_U32 PtId, VI_DRV_BT656_CFG * pst656Cfg);
HI_VOID VI_DRV_SetPtUnifyTiming(HI_U32 PtId, VI_DRV_PORT_UNIFY_TIMING * pstunitiming);
HI_VOID VI_DRV_SetPtUnifyData(HI_U32 PtId, VI_DRV_PORT_UNIFY_DATA * pstunidate);
HI_VOID VI_DRV_SetChEn(HI_U32 ChId, HI_BOOL bEn);
HI_VOID VI_DRV_SetChRegNewer(HI_U32 ChId);
HI_VOID VI_DRV_SetChIntMask(HI_U32 ChId, HI_U32 u32IntMask);
HI_VOID VI_DRV_ClrChIntStatus(HI_U32 ChId, HI_U32 u32ClrInt);
HI_U32  VI_DRV_GetChIntStatus(HI_U32 ChId);
HI_VOID VI_DRV_SetChAdapter(HI_U32 ChId, HI_BOOL bVsyncMode, HI_BOOL bHsyncMode);
HI_VOID VI_DRV_SetChCropEn(HI_U32 ChId, HI_BOOL n0_en, HI_BOOL n1_en);
HI_VOID VI_DRV_SetChCrop(HI_U32 ChId, HI_BOOL cropzoom, RECT_S *pstCapRect);
HI_VOID VI_DRV_SetChCropWin(HI_U32 ChId, HI_U32 width, HI_U32 height);
HI_VOID VI_DRV_SetChSkip(HI_U32 ChId, HI_U32 u32YSkip, HI_U32 u32CSkip);
HI_VOID VI_DRV_SetChSkipYWin(HI_U32 ChId, HI_U32 width, HI_U32 height);
HI_VOID VI_DRV_SetChSkipCWin(HI_U32 ChId, HI_U32 width, HI_U32 height);
HI_VOID VI_DRV_SetChCoefUpda(HI_U32 ChId, HI_U32 lhcoef_update, HI_U32 chcoef_update);
HI_VOID VI_DRV_SetChCoefRsel(HI_U32 ChId, HI_U32 lhcoef_read_sel, HI_U32 chcoef_read_sel);
HI_VOID VI_DRV_SetChHblankWidth(HI_U32 ChId, HI_U32 hblank_width);
HI_VOID VI_DRV_SetChLhFirSph(HI_U32 ChId, VI_DRV_CHN_LHFIR_SPH * pstlhfir);
HI_VOID VI_DRV_SetChChFirSph(HI_U32 ChId, VI_DRV_CHN_CHFIR_SPH * pstchfir);
HI_VOID VI_DRV_SetChLhFirOffset(HI_U32 ChId, HI_S32 hluma_offset);
HI_VOID VI_DRV_SetChChFirOffset(HI_U32 ChId, HI_S32 hchroma_offset);
HI_VOID VI_DRV_SetChLFirInSize(HI_U32 ChId, HI_U32 width);
HI_VOID VI_DRV_SetChCFirInSize(HI_U32 ChId, HI_U32 width);
HI_VOID VI_DRV_SetChLFirOutSize(HI_U32 ChId, HI_U32 width);
HI_VOID VI_DRV_SetChCFirOutSize(HI_U32 ChId, HI_U32 width);
HI_VOID VI_DRV_SetChLhFirCoef(HI_U32 ChId, HI_U32 phase_offset, const HI_S32 hlCoef[12]);
HI_VOID VI_DRV_SetChChFirCoef(HI_U32 ChId, HI_U32 phase_offset, const HI_S32 hchCoef[12]);
HI_VOID VI_DRV_SetChVcdsEn(HI_U32 ChId, HI_BOOL bEn);
HI_VOID VI_DRV_SetChVcdsCoef(HI_U32 ChId, HI_U32 Coef[2]);
HI_VOID VI_DRV_SetChDither(HI_U32 ChId, HI_U32 u32Mode);
HI_VOID VI_DRV_SetChDitherCoef(HI_U32 ChId, HI_U32 Coef[8]);
HI_VOID VI_DRV_SetChDitherSize(HI_U32 ChId, SIZE_S * pstYSrcSize, SIZE_S * pstCSrcSize);
HI_VOID VI_DRV_SetChMirror(HI_U32 ChId, HI_BOOL bEn);
HI_VOID VI_DRV_SetChFlip(HI_U32 ChId, HI_BOOL bEn);
HI_VOID VI_DRV_SetChDes(HI_U32 ChId, VI_DRV_CHN_STORE_INFO * pstStoreCfg);
HI_VOID VI_DRV_SetChDesBufLen(HI_U32 ChId, HI_U32 u32YBufLen, HI_U32 u32CBufLen);
HI_VOID VI_DRV_SetChDesDis(HI_U32 ChId, HI_U32 u32YDis, HI_U32 u32CDis);
HI_VOID VI_DRV_SetChDesAddr(HI_U32 ChId, HI_U32 u32YAddr, HI_U32 u32CAddr);
HI_U32  VI_DRV_GetChDesYAddr(HI_U32 ChId);
HI_U32  VI_DRV_GetChDesCAddr(HI_U32 ChId);
HI_BOOL VI_DRV_IsInCapTop(HI_U32 PtId);

HI_VOID VI_DRV_SetTiming_BT656(HI_U32 PtId);
HI_VOID VI_DRV_SetTiming_BT1120(HI_U32 PtId);
HI_VOID VI_DRV_SetTiming_DigitalCam(HI_U32 PtId);
HI_VOID VI_DRV_SetTiming_HDMI_RX(HI_U32 PtId);
HI_VOID VI_DRV_SetVcapInit(HI_U32 ChId, VI_DRV_CHN_STORE_INFO *pstStoreCfg);
HI_VOID VI_DRV_SetChZme(HI_U32 ChId, VI_DRV_CHN_ZME_INFO * pstZmeInfo);
HI_VOID VI_DRV_EnableColourBar(HI_VOID);

#endif // __HAL_VI_H__
