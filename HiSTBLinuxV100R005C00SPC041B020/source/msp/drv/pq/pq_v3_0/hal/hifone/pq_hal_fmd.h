/****************************************************************************
*
* Copyright (C) 2014-2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_fmd.h
  Version       : Initial Draft
  Author        :  p00203646
  Created       : 2014/2/18
  Description   :

******************************************************************************/

#ifndef __PQ_HAL_FMD_H__
#define __PQ_HAL_FMD_H__

#include "hi_type.h"
#include "pq_hal_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


typedef struct
{
    HI_S32 lasiCnt14;
    HI_S32 lasiCnt32;
    HI_S32 lasiCnt34;
} REG_LASI_STAT_S;

typedef struct
{
    HI_S32 PCC_FFWD;
    HI_S32 PCC_FWD;
    HI_S32 PCC_BWD;
    HI_S32 PCC_CRSS;
    HI_S32 pixel_weave;
    HI_S32 PCC_FWD_TKR;
    HI_S32 PCC_BWD_TKR;
    HI_S32 PCCBLK_FWD[9];
    HI_S32 PCCBLK_BWD[9];
} REG_PCC_STAT_S;

typedef struct
{
    HI_S32 HISTOGRAM_BIN_1;
    HI_S32 HISTOGRAM_BIN_2;
    HI_S32 HISTOGRAM_BIN_3;
    HI_S32 HISTOGRAM_BIN_4;
} REG_HISTOGRAM_STAT_S;

typedef struct
{
    HI_S32 match_UM;
    HI_S32 nonmatch_UM;
    HI_S32 match_UM2;
    HI_S32 nonmatch_UM2;
} REG_UM_STAT_S;

typedef struct
{
    HI_S32 StillBlkCnt;
    HI_S32 BlkSad[16];
} REG_STILLBLK_STAT_S;

typedef struct
{
    HI_S32 iCHD;
} REG_SCENCHG_STAT_S;

typedef struct
{
    HI_S32               frmITDiff;
    REG_UM_STAT_S        frmUm;
    REG_PCC_STAT_S       frmPcc;
    REG_HISTOGRAM_STAT_S frmHstBin;
    REG_LASI_STAT_S      lasiStat;
    REG_STILLBLK_STAT_S  StillBlkInfo;
    REG_SCENCHG_STAT_S   SceneChangeInfo;
    REG_PCC_STAT_S       frmPcc2;
    REG_LASI_STAT_S      lasiStat2;
} ALG_FMD_RTL_STATPARA_S;

typedef struct
{
    HI_U32 u32MotionHist[32];         /* output of logic, 32bit unsigned */
    HI_U32 u32SmallMtnNum;            /* output of logic, 32bit unsigned */
    HI_U32 u32SmallMtnSum;            /* output of logic, 32bit unsigned */
    HI_U32 u32LargeMtnNum;            /* output of logic, 32bit unsigned */
    HI_U32 u32LargeMtnSum;            /* output of logic, 32bit unsigned */
    HI_U32 u32MidMtnNum0;
    HI_U32 u32MidMtnSum0;
    HI_U32 u32MidMtnNum1;
    HI_U32 u32MidMtnSum1;
    HI_U32 u32MidMtnNum2;
    HI_U32 u32MidMtnSum2;

} ALG_MAD_RTL_STATPARA_S;

typedef struct
{
    ALG_FMD_RTL_STATPARA_S stFmdRtlStatPara; /* 需要从逻辑读的统计信息 */
    ALG_MAD_RTL_STATPARA_S stMadRtlStatPara; /* 需要从逻辑读的统计信息 */
} IFMD_TotalHardPara;

typedef struct
{
    HI_S32 IsProgressiveSeq;
    HI_S32 IsProgressiveFrm;
    HI_S32 RealFrmRate;

} VDEC_TRAN_INFO_S;

/*IFMD软算法所需驱动传入参数，VPSS->PQ*/
typedef struct hi_PQ_REG_IFMD_CALC_S
{
    HI_U32  u32HandleNo;
    HI_U32  u32WidthY;
    HI_U32  u32HeightY;
    HI_S32  s32FieldOrder;                 /* 顶底场序 顶场优先底场优先 */
    HI_S32  s32FieldMode;                  /* 顶底场标志 */
    HI_U32  u32FrameRate;
    HI_BOOL bPreInfo;                      /* DEI逻辑处理timeout，仍旧处理上一场 */
    VDEC_TRAN_INFO_S  stVdecTranInfo;
    S_VPSSWB_REGS_TYPE* pstIfmdHardOutReg; /* 读取ifmd的状态寄存器 */

} PQ_IFMD_CALC_S;

typedef struct
{
    HI_S32 die_reset;
    HI_S32 die_stop;
    HI_S32 dir_mch;
    HI_S32 die_out_sel;
    HI_S32 jud_en;
    //HI_S32  ChromaVertFltEn;
    HI_S32 s32FieldOrder;      /* 顶底场序 */
    HI_S32 s32SceneChanged;    /* 场景切换信息 */
    HI_S32 s32FilmType;        /* 电影模式 */
    HI_U32 u32KeyValue;        /* 输出关键帧信息*/
    HI_U32 u32EdgeSmoothEn;

} REG_PLAYBACK_CONFIG_S;


HI_S32 PQ_HAL_GetIFmdStatInfo(PQ_IFMD_CALC_S* pstIfmdCalcInfo, IFMD_TotalHardPara* pIfmdTotalHardPara);
HI_S32 PQ_HAL_SetIFmdInfo(HI_U32 u32HandleNo, REG_PLAYBACK_CONFIG_S* NewCfg);
HI_S32 PQ_HAL_SetIFmdEn(HI_U32 u32HandleNo, HI_U32 u32IFmdEn);
HI_S32 PQ_HAL_SetEdgeSmoothEn(HI_U32 u32HandleNo, HI_BOOL bEdgeSmoothEn);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

