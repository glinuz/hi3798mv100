/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_ifmd_alg.h
  Version       : Initial Draft
  Author        : pengjunwei
  Created       : 2016-03-03
  Description   : soft alg

******************************************************************************/

#ifndef __PQ_MNG_IFMD_ALG_H__
#define __PQ_MNG_IFMD_ALG_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define PLAYBACK_FRMRATETHR_L       23800
#define PLAYBACK_FRMRATETHR_H       24200

typedef enum
{
    IP_DECTECT_INTERLACE_PRIMARY = 0,
    IP_DECTECT_PROGRESSIVE_PRIMARY,
    IP_DECTECT_BUTT
} PQ_IFMD_IP_DECTECT_E;

typedef enum
{
    ALG_DEI_MODE_5FLD = 0,
    ALG_DEI_MODE_4FLD,
    ALG_DEI_MODE_3FLD,
    ALG_DEI_MODE_BUTT
} ALG_DEI_MODE_E;


typedef enum
{
    PB_VIDEO = 0,
    PB_32PD,
    PB_22PD
} PB_STATE_E;

typedef struct
{
    HI_S32 IsProgressiveSeq;
    HI_S32 IsProgressiveFrm;
    HI_S32 RealFrmRate;
} ALG_VDEC_INFO_S;

typedef struct
{
    HI_BOOL Pd32Det;
    HI_BOOL WithOvlpTitle;
    HI_S8   PdState;
    HI_S8   RptFlag;
    HI_S8   PdSubState; /*10 sub state to set pd para. meaningless when pd32det is false*/
} PD32_INFO_S;


typedef struct
{
    HI_S32 match_tkr;
    HI_S32 nonmatch_tkr;
} FMD_TKR_S;

typedef struct
{
    HI_S32 match_weave   ;
    HI_S32 nonmatch_weave;
} FMD_WEAVE_S;



typedef struct
{
    HI_S32 Sads[5];
    HI_S32 MinSad  ;
    HI_S32 MinSad2nd;
    HI_S32 MaxSad;
} FMD_SAD_S;


typedef struct
{
    HI_S32 Count;
} PD32_PHASE_INFO_S;

typedef struct
{
    HI_S32 histFrmITDiff[5];   /* save five successive history FrmITDiff values for calculating mean value. */
    HI_S32 lstFrmITDiffMean;   /* mean value of last five history FrmITDiff values. */
    HI_S32 curFrmITDiffMean;   /* mean value of current five history FrmITDiff values. */
    HI_S32 meanFrmITDiffAcc;   /* Accumulation value of history FrmITDiff mean diffrence. */
    HI_S32 lstRepfMotion;      /* Repeate field motion of last time. */
    HI_S32 anomalCnt;          /* abnormal situation counter of progressive state. */

    HI_S32 lstPcc_Crss;        /* save the current PCC_CRSS for next use,add by b00182026 for 4 field IP detect at 2011-11-16 */
    HI_S32 lstnonmatch_weave;  /* save the current PCC_CRSS for next use,add by b00182026 for 4 field IP detect at 2011-11-16 */

    HI_S32 field_match_weave;
    HI_S32 field_nonmatch_weave;
    HI_S32 field_pcc_crss;
    HI_S32 field_match_tkr;
    HI_S32 field_nonmatch_tkr;

    HI_S32 frame_match_weave;
    HI_S32 frame_nonmatch_weave;
    HI_S32 frame_pcc_crss;
    HI_S32 frame_match_tkr;
    HI_S32 frame_nonmatch_tkr;
    HI_S8   phaseCnt[2];         /* 22pulldown phase counter */
    HI_BOOL pld22Lock;          /* lock state:0-unlock; 1-lock */
    HI_BOOL bMCDIMode;          /* deiMode Sel:0:MC; 1-MA */
    HI_S8     s8DeiModeCnt;       /* dei mode counter */
} PLD22_CNTXT_S;

typedef struct
{
    HI_S32 lst_repeat;
    HI_S32 cur_repeat;
    HI_S32 nxt_repeat;
} REPEAT_S;

typedef struct
{
    HI_S32 lst_drop;
    HI_S32 cur_drop;
    HI_S32 nxt_drop;
} DROP_S;

typedef struct
{
    HI_U8 g_lstreffld;    /* reference field for the frame befor the current frame */
    HI_U8 g_curreffld;    /* reference field for the current frame */
    HI_U8 g_nxtreffld;    /* reference field for the coming frame */
} REF_FLD_S;

typedef struct
{
    HI_U8 g_lstbtmode;
    HI_U8 g_curbtmode;
    HI_U8 g_nxtbtmode;
} BTMODE_S;

/*structure of pulldown software result */
typedef struct
{
    HI_BOOL Is2ndFld;             /*表示计算统计信息时，参考场是否为第二场*/
    HI_BOOL NxtIs2ndFld;          /*表示驱动下一次配置给逻辑的参考场是否为第二场*/
    HI_BOOL BtMode;               /*表示计算统计信息时，配置给逻辑的场序*/
    HI_BOOL RefFld;               /*表示计算统计信息时，配置给逻辑的参考场,算法人员描述的参考场也是逻辑人员的当前场*/
    HI_S32  s32PbDelay;           /*delayed fields*/
    HI_S32  SadCnt;
    HI_S32  SadBuf[16];
    HI_S32 *pSadRd;
    HI_S32 *pSadWt;

    HI_S8   MainState;
    HI_S8   PrevMainState;
    HI_S8   init_symbol;
    HI_S8   SceneChange[2];
    HI_S8   Last32Phase;
    HI_S32  SADDiffAcc;
    HI_S32  FieldOrder;

    HI_S32  ScSadBuf[6];
    HI_U16  sadReg[16][3];
    HI_BOOL Pld22LockState[3];   /* lock state of last 3 fields */
    PLD22_CNTXT_S Pld22Ctx;

    PD32_INFO_S PdInfo;
    PD32_INFO_S Pld32InfoBuf[5];
    PD32_PHASE_INFO_S Phases32[5];

    REPEAT_S stRepeatHist;
    DROP_S   stDropHist;
} ALG_FMD_CTX_S;

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


/* still blocks */
typedef struct
{
    HI_S32 STILLBLK_THR;    /* 8bit [0:255] */
} STILLBLK_THD_S;


/* submission information struct for hardware (exterior interface) */
typedef struct
{
    HI_BOOL DirMch;
    HI_BOOL DieOutSelLum;
    HI_BOOL DieOutSelChr;
    HI_BOOL EdgeSmoothEn;   /* 仅在2:2电影模式且边缘平滑使能情况下为1*/

    HI_BOOL SceneChange;    /* 场景切换信息 */
    HI_S32  s32FieldOrder;  /* 顶地场序 */
    HI_S32  s32FilmType;    /* 电影模式 */
} ALG_FMD_RTL_OUTPARA_S;


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
    HI_BOOL bDeiEnLum;
    HI_BOOL bDeiEnChr;
    HI_S32  s32DeiMdLum;   /* 0-5 field; 1-4 filed; 2-3 field; 3-reserved */
    HI_S32  s32DeiMdChr;
    HI_BOOL bDeiRst;       /* 0-reset invalid; 1-reset valid,don't read history motion infomation; */
    HI_S32  FodEnable;     /* field order detect enable on-off: 1-enable; 2-disable,forced to top first; 3-disable,forced to bottom first; 0-disable,default; */
    HI_BOOL Pld22Enable;   /* Interleaved/Progressive detect enable on-off: 1-enable; 0-disable */
    HI_BOOL Pld32Enable;   /* pulldown detect enable on-off: 1-enable; 0-disable */
    HI_BOOL EdgeSmoothEn;  /* edge smooth enable on-off: 1-disable; 0-enable */
    HI_S32  s32Pld22Md;

    HI_S32  s32FrmHeight;  /* height of source */
    HI_S32  s32FrmWidth;   /* width  of source */
    //HI_S32 s32PbDelay;   /* delayed fields */
    HI_BOOL bOfflineMode;  /*system mode: 1-offline mode; 0-online mode*/

    /* the following may be different for different frames */
    HI_S32  s32Repeat;     /* if s32Repeat>0, it indicates this frame is repeated. */
    HI_S32  s32Drop;       /* if s32Repeat>0, it indicates this frame is  dropped. */
    HI_BOOL BtMode;        /* 为读统计信息时的那一场的场序， 0 topFirst */
    HI_BOOL RefFld;        /* ref field  为读统计信息时的那一场，顶场 配0 当前场为底场 配1 */
    HI_BOOL bPreInfo;      /* DEI逻辑处理timeout，仍旧处理上一场 */

    ALG_VDEC_INFO_S stVdecInfo;
    ALG_FMD_RTL_STATPARA_S stFmdRtlStatPara; /* 需要从逻辑读的统计信息 */
    ALG_MAD_RTL_STATPARA_S stMadRtlStatPara; /* 需要从逻辑读的统计信息 */
} ALG_DEI_DRV_PARA_S;

typedef struct
{
    HI_U32          u32InitFlag;
    REF_FLD_S       stRefFld;
    BTMODE_S        stBtMode;
    ALG_FMD_CTX_S   stFmdCtx;
    STILLBLK_THD_S  StillBlkCtrl;
    ALG_FMD_RTL_OUTPARA_S stRtlOutParaBak;/* 当DEI逻辑Timeout时，使用上一次的计算结果 */
} ALG_FMD_SOFTINFO_S;


HI_S32 PQ_MNG_ALG_FmdSet(ALG_FMD_SOFTINFO_S *pstFmdSoftInfo, ALG_DEI_DRV_PARA_S *pstDeiDrvPara, ALG_FMD_RTL_OUTPARA_S *pstFmdRtlOutPara);



#ifdef __cplusplus
#if __cplusplus
}
#endif

#endif

#endif

