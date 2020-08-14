/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_dei.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/11/20
  Description   :

******************************************************************************/

#ifndef __PQ_HAL_DEI_H__
#define __PQ_HAL_DEI_H__

#include "hi_type.h"
#include "pq_hal_comm.h"
#include "pq_mng_dei_alg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* AV500新加 */
typedef struct hiMc_DeiParam_S
{
    int MC_ONLY ;
    int MA_ONLY ;

    // ME Downscaling
    int MEDS_EN;  //[0,1]

    // ** ME of MCDI **
    int CORE_MAG_RG;
    int LMT_MAG_RG;

    int K_RGDIFYCORE;
    int G_RGDIFYCORE;

    int COEF_SADLPF;

    int CORE_RGDIFY;
    int LMT_RGDIFY ;

    int KMV_RGSAD;//4;
    int K_TPDIF_RGSAD;
    int G_TPDIF_RGSAD;
    int THMAG_RGMV ;
    int TH_SADDIF_RGMV;
    int TH_0MVSAD_RGMV;

    int CORE_MV_RGMVLS ;
    int K_MV_RGMVLS;
    int CORE_MAG_RGMVLS;
    int K_MAG_RGMVLS   ;

    int TH_MVADJ_RGMVLS;
    int EN_MVADJ_RGMVLS; // 1

    // mv chk
    int BLKMVSTARTC;
    int BLKMVENDC;

    // .. candidat mv
    int TH_ZEROBLKMV;

    int KSAD_RGMV;
    int ADDSAD_NEIBRGMV;

    // .. mv refinement
    int CORE_MVX_REFSAD;
    int K_MVX_REFSAD;
    int MIN_MVX_REFSAD;
    int MAX_MVX_REFSAD;

    // .. blk sad
    int MAX_HFCORE;
    int K_MAXHFCORE;
    int K_HFXCORE;
    int K_HFYCORE;

    int CORE_MAG_BLK;
    int LMT_MAG_BLK;

    int DIFLMT_SAD_BLK;

    // .. blk sad penalty

    // K_PNL
    int CORE_MV_KPNL;
    int K_MV_KPNL;

    int CORE_KSADDIF_KPNL;
    int K_KSADDIF_KPNL;
    int G_KSADDIF_KPNL;

    int CORE_RGMAG_KPNL;
    int K_RGMAG_KPNL ;
    int TH_RGSAD_KPNL;
    int K_RGSAD_KPNL ;
    int CORE_RGLS_KPNL;

    int K_W_SELRGMV;

    int TH_RATIO_KPNL;
    int K_RATIO_KPNL;
    int G_RATIO_KPNL;

    int K_PNL_SAD;
    int G_PNL_SAD;
    // K_PNL adjustment
    int X_RGMV_KPNL;
    int K_RGMV_KPNL;

    int X_DIFPNL_KPNL;
    int K_DIFPNL_KPNL;
    int G_DIFPNL_KPNL;

    // mv value penalty
    int CORE_MVDST_PNL;
    int K_MVPNL;
    int G_MVPNL;

    // mv spatial penalty
    int K_SPPNL;
    int G_SPPNL;

    int CORE_WMAG_SPPNL;
    int K_WMAG_SPPNL;
    int B_WMAG_SPPNL;

    int TH_WSAD_SPPNL;
    int K_WSAD_SPPNL;
    int B_WSAD_SPPNL;

    int CORE_WMAG_TPPNL;
    int K_WMAG_TPPNL;
    int B_WMAG_TPPNL;

    int TH_WSAD_TPPNL;
    int K_WSAD_TPPNL;
    int B_WSAD_TPPNL;

    // mv temporal penalty
    int K_TPPNL;
    int G_TPPNL;

    // mv rgmv penalty
    int CORE_SAD_RGPNL;
    int K_SAD_RGPNL;
    int G_SAD_RGPNL;

    int TH_MAG_RGPNL;
    int K_MAG_RGPNL;
    int G_MAG_RGPNL;

    int TH_LS_RGPNL;
    int K_LS_RGPNL;
    int G_LS_RGPNL;

    // mv mag penalty
    int CORE_BLKMAG_MAGPNL;
    int K_MAGPNL;
    int G_MAGPNL;

    int CORE_RGMAG_MAGPNL;
    int K_RGMAG_MAGPNL;
    int TH_RGSAD_MAGPNL;
    int K_RGSAD_MAGPNL;
    int CORE_RGLS_MAGPNL;
    int K_RGLS_MAGPNL;

    int K_RGMV_MAGPNL; // 4
    int G_RGMV_MAGPNL;

    // 0mv's penalty
    int CORE_RGMVDIF_0MVPNL;
    int K_RGMVDIF_0MVPNL;

    int CORE_RGMAG_0MVPNL;
    int K_RGMAG_0MVPNL;

    int CORE_RGLS_0MVPNL;
    int K_RGLS_0MVPNL;

    int K_RG_0MVPNL;
    int G_RG_0MVPNL;

    // .. mv selection

    int FORCE_MVEN;
    int FORCE_MVX;
    //int FORCE_MVY;

    // .. blk mv postprocessing
    int TH0_MVDIST_MVSPCHK;
    int TH1_MVDIST_MVSPCHK;
    int TH_MAG_MVSPCHK;
    int TH_WRG_MVSPCHK;
    int TH_SAD_MVSPCHK;

    int TH0_MVDIST_MVSPDLT;
    int TH1_MVDIST_MVSPDLT;
    int TH_MAG_MVSPDLT;
    int TH_WRG_MVSPDLT;
    int TH_SAD_MVSPDLT;

    int EN_MVDILATION;
    int THL_MAG_MVMEDFILT;
    int THH_MAG_MVMEDFILT;

    /*------------------ MC of MCDI -----------------*/

    // .. select two mvs
    int G_SAD_SELMCMV;
    int K_SAD_SELMCMV;
    int G_MAG_SELMCMV;
    int K_MAG_SELMCMV;
    int K_MAXMAG_SELMCMV;

    int K_CORE_SIMIMV;
    int GL_CORE_SIMIMV;
    int GH_CORE_SIMIMV;
    int K_SIMIMVW;
    int K_RGLSW;

    int K_CURMAG_MCW ;
    int K_CURW_MCBLD ;

    // .. RGMV mc weight
    int B_BLKSAD_MCW;

    int CORE_MVY_MCW;
    int K_MVY_MCW;

    int CORE_RGSADADJ_MCW;
    int K_RGSADADJ_MCW ;

    int K_CORE_VSADDIF;
    int GL_CORE_VSADDIF;
    int GH_CORE_VSADDIF;
    int K_VSADDIFW;

    int MODE_RGYSAD_MCW;

    int CORE_RGMAG_MCW;
    int X0_RGMAG_MCW;
    int K0_RGMAG_MCW;
    int G0_RGMAG_MCW;
    int K1_RGMAG_MCW;

    int CORE_RGSAD_MCW;
    int X0_RGSAD_MCW;
    int K0_RGSAD_MCW;
    int G0_RGSAD_MCW;
    int K1_RGSAD_MCW;

    // .. blk mc weight
    int K_MAG_BLKSADCORE_MCW; // 1
    int G_MAG_BLKSADCORE_MCW;

    int X0_MAG_BLK_MCW;
    int K0_MAG_BLK_MCW;
    int G0_MAG_BLK_MCW;
    int K1_MAG_BLK_MCW;
    int G1_MAG_BLK_MCW;

    int X0_SAD_BLK_MCW;
    int K0_SAD_BLK_MCW;
    int G0_SAD_BLK_MCW;
    int K1_SAD_BLK_MCW;
    int G1_SAD_BLK_MCW;

    int K_AVGBLKMCW;
    int K_CURBLKMCW;
    int G_CURBLKMCW;

    // .. tp_mv mc weight
    int X0_TPMVDIST_MCW;
    int K0_TPMVDIST_MCW;
    int G0_TPMVDIST_MCW;
    int K1_TPMVDIST_MCW;

    int K_MINMVDIST_MCW;
    int K_AVGMVDIST_MCW;

    int B_CORE_TPMVDIST_MCW;
    int K_CORE_TPMVDIST_MCW;

    // .. wnd mc weight
    int K_DIFHCORE_MCW ;
    int K_SUB_HCORE_MCW;
    int K_DIFVCORE_MCW ;
    int K_MAX_CORE_MCW;
    int K_MAX_DIF_MCW;
    int K1_MAX_MAG_MCW;
    int K0_MAX_MAG_MCW;

    int K_TBDIF_MCW;
    int K_TBMAG_MCW;

    int K_BLKSAD_WND_MCW;

    int K_MAG_WNDSADCORE_MCW;
    int G_MAG_WNDSADCORE_MCW;

    int X0_MAG_WND_MCW;
    int K0_MAG_WND_MCW;
    int G0_MAG_WND_MCW;
    int K1_MAG_WND_MCW;
    int G1_MAG_WND_MCW;

    int X0_SAD_WND_MCW;
    int K0_SAD_WND_MCW;
    int G0_SAD_WND_MCW;
    int K1_SAD_WND_MCW;
    int G1_SAD_WND_MCW;

    // .. blk mag/sad adjustment
    int K_MAGADD_DATA_MCW ;
    int K_SADCORE_DATA_MCW;

    int X0_K_BLKMAGCORE_MCW;
    int G0_K_BLKMAGCORE_MCW;
    int K0_K_BLKMAGCORE_MCW;
    int G1_K_BLKMAGCORE_MCW;
    int G_BLKMAGCORE_MCW;

    int X0_BLKSADCORE_MCW;
    int K0_BLKSADCORE_MCW;
    int G0_BLKSADCORE_MCW;
    int K1_BLKSADCORE_MCW;
    int G1_BLKSADCORE_MCW;

    // .. vert weight calculation
    int K_MAX_HVDIF_DW;
    int GAIN_LPF_DW;

    int CORE_BHVDIF_DW;
    int K_BHVDIF_DW;
    int B_BHVDIF_DW;
    int B_HVDIF_DW;
    int CORE_HVDIF_DW;

    int K_DIFV_DW;


    int X0_HVDIF_DW;
    int K0_HVDIF_DW;
    int G0_HVDIF_DW;
    int K1_HVDIF_DW;

    int CORE_MV_DW;
    int B_MV_DW ;
    int X0_MV_DW;
    int K0_MV_DW;
    int G0_MV_DW;
    int K1_MV_DW;

    int B_MT_DW ;
    int X0_MT_DW;
    int K0_MT_DW;
    int G0_MT_DW;
    int K1_MT_DW;

    int G0_MV_MT;
    int X0_MV_MT;
    int K1_MV_MT;

    // .. mc
    int GAIN_UP_MCLPFV;
    int GAIN_DN_MCLPFV;
    int GAIN_MCLPFH;

    int RS_PXLMAG_MCW;
    int X_PXLMAG_MCW;
    int K_PXLMAG_MCW;

    int Mc_LAI_BLDMode;
    int nK_MCW_Y;
    int nK_MCW_C;
    int nK_MCBLD_Y;
    int nK_MCBLD_C;

    int X0_MCW_ADJ;
    int K0_MCW_ADJ;
    int G0_MCW_ADJ;
    int K1_MCW_ADJ;


    int X0_MCBLD;
    int K0_MCBLD;
    int G0_MCBLD;
    int K1_MCBLD;

    int K_Y_VERTW;
    int K_C_VERTW; //add by z00222166
    //int K_CY_VERTW;

    int K_AVG_DELTA;
    int K_DELTA;
    int X0_MV_DELTA;
    int K1_MV_DELTA;

    int BDH_MCPOS;
    int BDV_MCPOS;

    int X0_MV_GMD_MC;
    int K1_MV_GMD_MC;

    int DEC_HFCORE_GMD_MC;
    int INC_HFCORE_GMD_MC;

    int K_HFCORE_GMD_MC;

    // .. gmd
    int MTSUM0_GMD;
    int MTSUM1_GMD;
    int MTSUM2_GMD;
    int MTSUM3_GMD;

    int MTFILTEN_GMD;

    int K_MAXMAG_GMD;
    int K_DIFH_GMD;
    int K_MAG_GMD;

    int MTTH0_GMD;
    int MTTH1_GMD;
    int MTTH2_GMD;
    int MTTH3_GMD;

    // .. progressive gmd
    int MTSUM0_GMD_P;
    int MTSUM1_GMD_P;
    int MTSUM2_GMD_P;
    int MTSUM3_GMD_P;

    int MCLPF_MODE;
    int Ass_VertLPFON;
    int Wnd_MaxOn;
    int McStartR;
    int McStartC;
    int McEndR;
    int McEndC;

    HI_BOOL bfld_motion_wnd_mode;
    HI_BOOL blong_motion_shf;

    //add by z214841 2015.7.8
    int K_SADCORE_RGMV;
    int TH_SAD_RGLS;
    int TH_MAG_RGLS;
    int K_SAD_RGLS;

    int TH_HBLKDIST_MVDLT;
    int TH_VBLKDIST_MVDLT;
    int TH_LS_MVDLT;
    int TH_RGMVX_MVDLT;
    int TH_BLKMVX_MVDLT;

    int THH_SAD_MVDLT;
    int THL_SAD_MVDLT;
    int G_MAG_MVDLT;
    int TH_MAG_MVDLT;
    int K_SADCORE_MVDLT;

    int RGTB_EN_MCW;

    int X0_SMRG_MCW;
    int K0_SMRG_MCW;

    int X_RGSAD_MCW;
    int K_RGSAD_MCW;

    int G_PXLMAG_MCW;

    int K_Y_MCW;
    int K_C_MCW;

    int X0_MV_MC;
    int K1_MV_MC;

    int C0_MC;
    int R0_MC;

    int C1_MC;
    int R1_MC;

    int MCMVRange;

    int SCENECHANGE_MC;
    int X_FRCOUNT_MC;
    int K_FRCOUNT_MC;

    int MC_LAI_BLDMODE;
    int MC_NUMT_BLDEN;

    //no register
    int DG_NUM;   //no register
    int DG_ROW;   //no register
    int DG_ROW_O; //no register

    int DG_BLKH; //no register
    int DG_BLKV; //no register
    int DG_RGH;  //no register
    int DG_RGV;  //no register

    int DUMP_EN;//1;//1;//0;//1;//0;//1;//1;//1; //1;//no register

    int MAVERT_ONLY;   //no register

    int PRJNUM;        // fixed   //no register
    int BIT_DIV;       // fixed   //no register
    int BIT_NORM_O;    // fixed   //no register
    int BIT_SUBMV;     // fixed   //no register
    int BIT_SADADD;    // fixed   //no register
    int BIT_BLKMAGADD; // fixed   //no register

    int CANDIMV_NUM;   // fixed   //no register
    int CANDIRGMV_NUM; // fixed   //no register

    int K_MCMVY; // fixed   //no register

    int FRAME_IN;  // [0,1]   //no register
    int FR_TBCONV; // for frame in, 0-top/bot-->top/bot,1-top/bot-->bot/top //[0,1]   //no register
    int ISTOPFST;  // 1-top first,0-bot first //[0,1]   //no register

    int RGB2YUV_EN; // not used   //no register
    int YUV2RGB_EN; // not used   //no register
    int CSC_MODE;   // not used   //no register
    int AUTOSET_EN; // not used   //no register

    int BEINYUV444;  // not used   //no register
    int BEOUTYUV444; // not used   //no register
    int YUV_FORMAT; //3;  // [0,2]//0-444,1-422,2-420   //no register

    int FORCE_YUV444;//1;  // not used   //no register
    int CS_UV; // not used   //no register
    int RS_UV; // not used   //no register

    int COEF0_DS; // [0,7]   //no register
    int COEF1_DS; // [0,15]  //no register
    int COEF2_DS; // [0,15]  //no register
    int COEF3_DS; // [0,15]  //no register
    int COEF4_DS; // [0,7]   //no register

    int BIT_BILIN; // fixed   //no register
    int MODE_MEP1; //1; // fixed  //no register

    int RGNUMH; //no register
    int RGNUMV; //no register

    int RGSIZEH; // fixed   //no register
    int RGSIZEV; //16;  // fixed   //no register
    int RGSTATH; // fixed  //no register
    int RGSTATV; //32; // fixed   //no register
    int DIV_RGHEIGHT; // fixed   //no register
    int DIV_RGWIDTH;  // fixed   //no register

    int RGOFFSETH; // [0,63]   //no register
    int RGOFFSETV; // [0,15]   //no register

    int RGBORDER_EN; // fixed   //no register
    int RGBORDERH0;  // fixed   //no register
    int RGBORDERV0;  // fixed   //no register
    int RGBORDERH1;  // fixed   //no register
    int RGBORDERV1;  // fixed   //no register

    int RGLSTOFFSETH; // [0,127]   //no register
    int RGLSTOFFSETV; // [0,31]   //no register

    int RGMVWNDH; // fixed   //no register
    int RGMVWNDV; // fixed   //no register

    int MVRANGEH; // fixed   //no register
    int MVRANGEV; // fixed   //no register

    int BLKMVRANGEH; // fixed   //no register
    int BLKMVRANGEV; // fixed   //no register

    int HBLKSIZE;
    int VBLKSIZE;

    int MODE_SELBLKMV;
    int BITSHIFT_GMD;

    int COL0_GMD;
    int ROW0_GMD;
    int COL1_GMD;
    int ROW1_GMD;

    int DIVCOL_GMD;
    int DIVROW_GMD;

    int TH_GMDSUM_MD;
    int K_GMDSUM_MD;//32;//32; //16;
    int G_GMDSUM_MD;

    /********************************新增的几个配置*************************************************/
    /***** for MV200 update, CV200 don't have these para     ****************************************************/
    //for horizontal/vertical mag calculation and horizontal edge detection
    int KHOREDGE;
    int KMAGH_1;
    int KMAGH_2;
    int KMAGV_1;
    int KMAGV_2;
    int GAIN_HOREDGE;
    int FRAME_MAG_EN;
    int MOTION_LIMT_1;
    int MOTION_LIMT_2;
    int GAIN_FRAMEMOTION;
    /***********************************************************************************************/

} HI_Mc_DeiParam_S;

typedef struct
{
    /* normlized strength */
    HI_U16 EDGE_NORM[12];    /* unsigned 12 bit */

    /* edge strength adjustment for MC */
    HI_U16 INTER_DIFF_THD0;  /* 8bit */
    HI_U16 INTER_DIFF_THD1;  /* 8bit */
    HI_U16 INTER_DIFF_THD2;  /* 8bit */
    HI_S16 MC_STRENGTH_K0;   /* signed 8bit */
    HI_S8  MC_STRENGTH_K1;   /* signed 8bit */
    HI_S8  MC_STRENGTH_K2;   /* signed 8bit */
    HI_S8  MC_STRENGTH_K3;   /* signed 8bit */
    HI_U8  MC_STRENGTH_G0;   /* unsigned 8bit */
    HI_U8  MC_STRENGTH_G1;   /* unsigned 8bit */
    HI_U8  MC_STRENGTH_G2;   /* unsigned 8bit */
    HI_U8  MC_STRENGTH_G3;   /* unsigned 8bit */
    HI_U8  MC_STRENGTH_MAXG; /* unsigned 8bit */
    HI_U8  MC_STRENGTH_MING; /* unsigned 8bit */

    /* edge strength adjustment for MC */
    HI_U16 MC_EDGE_CORING; /* unsigned 10bit */
    HI_U16 MC_EDGE_SCALE;  /* unsigned 10bit */

} InterpInfo_S;

typedef struct
{
    HI_U32 MOTION_HIST[32];             /* output of logic, 32bit unsigned */
    HI_U32 SMALL_MOTION_NUM;            /* output of logic, 32bit unsigned */
    HI_U32 SMALL_MOTION_SUM;            /* output of logic, 32bit unsigned */
    HI_U32 LARGE_MOTION_NUM;            /* output of logic, 32bit unsigned */
    HI_U32 LARGE_MOTION_SUM;            /* output of logic, 32bit unsigned */
    HI_U32 MIDDL_MOTION_NUM;            /* output of logic, 32bit unsigned */
    HI_U32 MIDDL_MOTION_SUM;            /* output of logic, 32bit unsigned */

    HI_U32 MOTION_THD0;
    HI_U32 MOTION_THD1;
    HI_U32 MOTION_THD2;
    HI_U32 MOTION_THD3;
} MotionHist_S;

typedef struct
{
    HI_U16 ADJUST_GAIN; /* 8-bit unsigned, (2.6) foramt, in practice it will be ">> 6" */ /* 对应寄存器 motion_adjust_gain, 直接把结果设置到寄存器即可 */
    HI_U16 rec_mode_fld_motion_step_0;
    HI_U16 rec_mode_fld_motion_step_1;
} MotionDetect_S;

typedef struct hiDEI_Variable_S
{
    InterpInfo_S   stINTERPINFO;   /* mc */
    MotionHist_S   stMOTIONHIST;
    MotionDetect_S stMDINFO;       /* new motion detection para info */
    DeiSwAlgCfg_S  stDeiSwAlgCfg;
} HI_DEI_Variable_S;

typedef struct  hiDEI_PARAMETER
{
    /* for base config */
    HI_BOOL  bDeiOutSelLum;
    HI_BOOL  bDeiOutSelChm;
    HI_U8    u8DirMchC;
    HI_U8    u8DirMchL;
    HI_U8    u8DeiLmmode;
    HI_U8    u8DeiChmmode;
    HI_BOOL  bDieRst;
    HI_BOOL  bStinfoStop;

    /* For Low Angle Interpolation */
    HI_U8    au8DirRatio[15];
    HI_U32   u32DirRatioVer;
    HI_U32   u32Bc1MaxDz;
    HI_U32   u32Bc1AutoDzGain;
    HI_U32   u32Bc1Gain;
    HI_U32   u32Bc2MaxDz;
    HI_U32   u32Bc2AutoDzGain;
    HI_U32   u32Bc2Gain;
    HI_U32   u32ScaleRange;
    HI_S32   s32MinVerStrength;
    HI_U8    au8IntpScaleRatio[15];
    HI_S32   s32StrengthThd;
    HI_U32   u32DirThd;
    HI_U32   u32BcGain;
    HI_BOOL  bEdgeSmoothEn;
    HI_U32   u32EdgeSmoothRatio;
    HI_U8    u8EdgeMode;
    HI_U8    u8HorEdgeEn;
    HI_BOOL  bLumaMfMax; /* 0-亮度mf为统计值；1-亮度mf固定为最大值 */
    HI_BOOL  bChmaMfMax; /* 0-色度mf为统计值；1-色度mf固定为最大值 */
    /** For Motion Detection **/

    /* only for 2-field dei */
    //HI_S32 s32MedThd;

    /* for calc frame motion: min or max of CE_SAD and DF_SAD */
    HI_U32  u32SCDUsingMax;

    /* for frame motion smooth */
    HI_BOOL bFrameMotionSmoothEn;
    HI_U16  au16FrameMotionSmoothThd[6];
    HI_U16  au16FrameMotionSmoothRatio[5];
    HI_S16  as16FrameMotionSmoothSlope[4];
    HI_U16  u16StartFrameMotionSmoothRatio;
    HI_U16  u16MaxFrameMotionSmoothRatio;
    HI_U16  u16MinFrameMotionSmoothRatio;

    /* for calc the jitter motion (horizontal texture) */
    HI_U16  u16JitterGain;   /* 4-bit unsigned, (1.3) foramt, in practice it will be ">> 3" */
    HI_U16  u16JitterCoring; /* 8-bit unsigned, when 10-bit processing depth, it will be multiplied by 4 in code */

    /* for calc field/feather motion ratio */
    HI_U16  u16FldMotionThdLow;    /* 8-bit unsigned */
    HI_U16  u16FldMotionThdHigh;
    HI_S8   s8FldMotionCurveSlope; /* 6-bit signed */
    HI_U8   u8FldMotionGain;       /* 4-bit unsigned */
    HI_U16  u16FldMotionCoring;    /* 8-bit unsigned */

    /* for calc motion IIR weight */
    HI_BOOL bMotionIIREn;               /* 1-bit unsigned */
    HI_U16  au16MotionDiffThd[6];       /* 8-bit unsigned */
    HI_S16  as16MotionIIRCurveSlope[4]; /* 6-bit signed */
    HI_U16  au16MotionIIRRatio[5];      /* 7-bit unsigned */

    HI_U16  u16MinMotionIIrRatio; /* 7-bit unsigned */
    HI_U16  u16MaxMotionIIrRatio; /* 7-bit unsigned */

    /* for history motion information and recursive mode */
    HI_BOOL bPreInfoEn;  /* 1-bit unsigned */
    HI_BOOL bPpreInfoEn; /* 1-bit unsigned */
    //HI_U32 u32FourPxlShare; /* 1-bit unsigned */

    /* for recursive mode */
    HI_BOOL bRecModeEn; /* 1-bit unsigned */
    HI_U8   u8RecModeWriteMode;
    HI_U8   au8RecModeFldMotionStep[2]; /* 3-bit unsigned */
    HI_U8   au8RecModeFrmMotionStep[2]; /* 2-bit unsigned */
    HI_U8   u8FrmFldBlendMode;          /* 0-clamp;1-blending */
    HI_U16  au16FrmFldBlendThd[6];
    HI_S16  as16FrmFldBlendSlope[4];
    HI_U16  au16FrmFldBlendRatio[5];
    //HI_U16  u16StartFrmFldBlendRatio;
    HI_U16  u16MinFrmFldBlendRatio;
    HI_U16  u16MaxFrmFldBlendRatio;

    /* for history motion */
    HI_BOOL bHisMotionEn;
    HI_BOOL bHisMotionUsingMode;  /* 1-bit unsigned:0-max;1-blending */
    HI_BOOL bHisMotionWriteMode;  /* 1-bit unsigned:0-max;1-blending */

    /* for  motion info write */
    HI_BOOL bMotionInfoWriteMode; /* 1-bit unsigned:0-max;1-blending */

    /* for morpological filter */
    HI_BOOL bMorFltEn;
    HI_U8   u8MorFltSize; /* 2-bit unsigned:0-1x9;1-1x7;2-1x5;3-1x3 */
    HI_U8   u8MorFltThd;

    /* for spatial-temporal blending */
    HI_BOOL bMedBlendEn;

    /* for chroma motion adjust */
    HI_U16 u16ChromMaOffset;  /* 8-bit unsigned:it will be multiplied by 4 when 10-bit processing bitdepth */

    /* for motion adjustment based global motion */
    HI_U16 u16AdjustCoring;
    HI_U16 u16AdjustGain;
    HI_U16 u16AdjustGainChroma;

    /* for comb check */
    HI_BOOL bCombChkEn;
    HI_U16  u16CombChkUpperLimit;
    HI_U16  u16CombChkLowerLimit;
    HI_U16  u16CombChkMinVThd;
    HI_U16  u16CombChkMinHThd;
    HI_U16  u16CombChkEdgeThd;
    HI_U16  u16CombChkMdThd;

    /* for   globalmotion statistic */
    HI_U32  u32SmallmotionThd; /* input of logic,10bit unsigned */
    HI_U32  u32LargemotionThd; /* input of logic,10bit unsigned */
    HI_BOOL  bIgblEn;

    HI_Mc_DeiParam_S  mcdeiparams;
    HI_DEI_Variable_S mcdeibaseParams;

    /* DeiSwAlgCfg_S   swAlgCfg; */
    HI_U32 ma_gbm_thd0;
    HI_U32 ma_gbm_thd1;
    HI_U32 ma_gbm_thd2;
    HI_U32 ma_gbm_thd3;

    HI_U32 numt_lpf_en;
    HI_U32 numt_gain;
    HI_U32 numt_coring;
} DEI_PARAMETER_S;

/* dei demo mode */
typedef enum
{
    DEI_DEMO_ENABLE_L = 0,
    DEI_DEMO_ENABLE_R
} DEI_DEMO_MODE_E;

/* dei demo mode */
typedef enum
{
    MC_DIDISABLE    = 0,
    MC_MAONLYOUTPUT    ,
    MC_DIOUTPUT
} MC_DEMO_MODE_E;

HI_S32  PQ_HAL_SetDeiAdjustGain(HI_U32 u32HandleNo, HI_U32 u32AdjustGain);
HI_BOOL PQ_HAL_GetInterlaceMotionEn(HI_U32 u32HandleNo);
HI_S32  PQ_HAL_SetInterlaceMotionEn(HI_U32 u32HandleNo, HI_BOOL bOnOff);
HI_S32  PQ_HAL_SetSoftReg(HI_U32 u32HandleNo, HI_Mc_DeiParam_S *pstMcParm, HI_DEI_Variable_S *pstDeiVar);
HI_S32  PQ_HAL_UpdateBlkRect(HI_U32 u32HandleNo, HI_U32 u32Width, HI_U32 u32Height);
HI_S32  PQ_HAL_SetMotionCompensateEn(HI_U32 u32HandleNo, HI_BOOL bMcEnable);
HI_S32  PQ_HAL_EnableDeiDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff);
HI_S32  PQ_HAL_GetMaDeiEn(HI_U32 u32HandleNo, HI_BOOL *pbOnOff);
HI_S32  PQ_HAL_GetMcDeiEn(HI_U32 u32HandleNo, HI_BOOL *pbOnOff);
HI_S32  PQ_HAL_SetDeiDemoMode(HI_U32 u32HandleNo, DEI_DEMO_MODE_E enMode);
HI_S32  PQ_HAL_SetDeiDemoModeCoor(HI_U32 u32HandleNo, HI_U32 u32XPos);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


