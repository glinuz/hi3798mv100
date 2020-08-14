/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_db_alg.h
  Version       : Initial Draft
  Author        : pengjunwei
  Created       : 2016-03-02
  Description   : soft alg

******************************************************************************/

#include "hi_math.h"

#ifndef __PQ_MNG_DB_ALG_H__
#define __PQ_MNG_DB_ALG_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define DBM_MIN(x, y) (((x) > (y)) ? (y) : (x))
#define DBM_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define DBM_CLIP3(low,high,x) (DBM_MAX(DBM_MIN((high),(x)),(low)))

#define MAXLINESIZE         4096
#define MAX_PICTURE_WIDTH   4096
#define MAX_PICTURE_HEIGHT  2160
#define MIN_BLOCKSIZE       4
#define MAX_HOR_BLOCK_NUM  (MAX_PICTURE_WIDTH / MIN_BLOCKSIZE)
#define MAX_VER_BLOCK_NUM  (MAX_PICTURE_HEIGHT / MIN_BLOCKSIZE)
#define MAX_BORDIST         32

#define READ_STROW_DATA_NUM     2048
#define DET_BLK_STATIC_INFO     4096
#define DB_DETECTOR             4096

typedef struct DetblkInfo_s
{
    HI_S8  Detsize;
    HI_U32 CounterBorder;
} DetblkInfo_s;

typedef struct  DetStaticUsing_S
{
    /* 结合时域信息之后，检测的结果 */
    HI_S8  UsingSizeHy;
    HI_S8  UsingSizeHchr;
    HI_S8  UsingSizeVy;
    HI_U32 UsingHyCounterBorder;
    HI_U32 UsingVyCounterBorder;

    HI_U32 UsingHchrCounterBorder;
    HI_U8  UsingBorderIndex[MAXLINESIZE];
    HI_U8* ptrUsingHyBorderIndex;
    HI_U8* ptrUsingVyBorderIndex;
    HI_U8* ptrUsingHchrBorderIndex;

    HI_U8  NumberWindow; /* 时间窗口 */
    HI_U8  SaturatWindow;

    HI_U8  HyMINBlkNum;
    HI_U8  HchrMINBlkNum;
    HI_U8  VyMINBlkNum;

    HI_U32 frms_NumsHy;
    HI_U32 frms_NumsHchr;
    HI_U32 frms_NumsVy;
    HI_U8  keyfrm_num;
    HI_U8  HyBordCountThreshold;
    HI_U8  HchrBordCountThreshold;
    HI_U8  VyBordCountThreshold;

    DetblkInfo_s Hy[16];
    DetblkInfo_s Vy[16];
    DetblkInfo_s Hchr[16];

    HI_S8  BorderCnt[MAXLINESIZE];
    HI_S8* ptrHyCnt;
    HI_S8* ptrHchrCnt;
    HI_S8* ptrVyCnt;
    HI_U8  frmsCntTempHy;
    HI_U8  frmsCntTempHchr;
    HI_U8  frmsCntTempVy;
    HI_U8  frmsWindowTemp;
    HI_U8  frmsSatWindowTemp;

    HI_S32 FillBorderNumHy;
    HI_S32 FillBorderNumVy;
    HI_S32 FillBorderNumHchr;
} DetStaticUsing_S;

typedef struct
{
    /* basic info, might be modified */
    HI_S32 s32LumWidth;
    HI_S32 s32LumHeight;
    HI_S32 s32ChrWidth;
    HI_S32 s32ChrHeight;
    HI_S32 s32LumHBlkNum; /* [0,480] */
    HI_S32 s32LumVBlkNum; /* [0,180] */
    HI_S32 s32ChrHBlkNum; /* [0,480] */

    HI_S32 det_size_en;
    HI_S32 det_hy_en;
    HI_S32 det_hc_en;
    HI_S32 det_vy_en;

    /* from wbc stt infor */
    HI_S32 hy_counter;
    HI_S32 hy_size;
    HI_S32 vy_counter;
    HI_S32 vy_size;
    HI_S32 hc_counter;
    HI_S32 hc_size;
    HI_U32 db_border[128];
    HI_S32 db_hy_counter[2048];

    /* software only */
    HI_S32 s32DBLumHBlkDetNum;
    HI_S32 s32DBLumVBlkDetNum;
    HI_S32 s32DBLumHBlkSize; /* non-interface register [6,64] */
    HI_S32 s32DBLumVBlkSize; /* non-interface register [6,64] */

    HI_U8 DbDetBlkNumWindow;
    HI_U8 DbFrmCnt;
    HI_U8 DBTempDetBlkRatioH[4];
    HI_U8 DBTempDetBlkRatioV[4];

    HI_S32 u32W;
    HI_S32 u32H;

} DB_DET_INFO_S;

typedef struct DetResultInfo_S
{
    HI_U32 ptrHyStrRow[MAXLINESIZE];
    //HI_U32* ptrHyStrRow;
    HI_U32 HyCounterBorder;            /* [0,1023] */
    HI_U32 HchrCounterBorder;          /* [0,1023] */
    HI_U32 VyCounterBorder;            /* [0,1023] */
    HI_S8  detsize_hy;                 /* [-1,63] */
    HI_S8  detsize_hchr;               /* [-1,63] */
    HI_S8  detsize_vy;                 /* [-1,63] */
    HI_U8  BorderIndex[MAXLINESIZE];
    HI_U8* ptrHyBorderIndex;           /* [0,1] */
    HI_U8* ptrHchrBorderIndex;         /* [0,1] */
    HI_U8* ptrVyBorderIndex;           /* [0,1] */
} DetResultInfo_S;


typedef struct
{
    HI_U32 u32ImageAvgQP;
    HI_U32 u32Protocol;
    HI_U32 u32VdhInfoStride;
    HI_U32 db_strethEn;
    HI_U32 u32VDHInfoAddr;

} VDH_INFO_S;

typedef struct
{
    HI_U8 u8DBEn;
    HI_U8 u8DBLumHorEn;
    HI_U8 u8DBLumVerEn;
    HI_U8 u8DBGradSubRatio;
    HI_U8 u8DBCtrstThresh;
    HI_U8 u8DBDirStrGainLut;
    HI_U8 u8DBDirStrLut;
    HI_U8 u8DBLumHStrFadeLut1;
    HI_U8 u8DBLumHStrFadeLut2;
    HI_U8 u8DBLumHStrFadeLut3;
    HI_U8 u8DBLumHStrFadeLut4;
    HI_BOOL bBlkPosMode;
    HI_U8 u8DBDiffStrLut;
    HI_U8 u8DBSobelStrLut;
    HI_U8 u8DBLumVStrFadeLut;
    HI_U8 u8GlobalDbStrengthLum;
    HI_U8 u8DBLumHorDeltaLut;
    HI_U8 u8DBLumVerDeltaLut;
    HI_U8 u8DBLumHorTrue;
    HI_U8 u8DBLumVerTrue;
    HI_U8 u8DBLumTrue;

    HI_U8 DBTextStrGain;
    HI_U8 DBvdhinfostridew;
    HI_U8 DBvdhinfostrideh;

    HI_U32 TLXOffset;
    HI_U32 TLYOffset;
    HI_U32 BRXOffset;
    HI_U32 BRYOffset;

    HI_U32 u32DBYWidth;
    HI_U32 u32DBYHeight;
    HI_U32 u32DBBitDepth;
    HI_U32 DBprotocol;
    HI_U8  DB_DET_EN;
    HI_U8  DBDetectRatioH;
    HI_U8  DBDetectRatioV;
    HI_U8  DBLumHBorderStrTh;
    HI_U8  DBLumVBorderStrTh;
    HI_U8  pLumHorBordFlag;
    HI_U8  pLumVerBordFlag;

    HI_U16 u16DBDirSmoothMode;
    HI_U16 u16DBDiffMpaLut;
    HI_U16 u16LumHorVarCore;
    HI_U16 u16LumHorGain1;
    HI_U16 u16LumHorGain2;
    HI_U16 u16DBDiffWeightLut;
} DB_REG_S;

typedef struct
{
    //VPSS_DB_CTRL
    HI_U16 db_lum_en;
    HI_U16 db_lum_ver_en;          /* DB滤波垂直亮度使能 */
    HI_U16 db_lum_hor_en;          /* DB滤波水平亮度使能 */
    HI_U16 db_cb_en;
    HI_U16 db_cr_en;
    HI_U16 db_chr_ver_en;          /* DB滤波垂直色度使能 */
    HI_U16 db_chr_hor_en;          /* DB滤波水平色度使能 */
    HI_U16 db_chr_hor_mid_grey_en;
    HI_U16 det_size_en;
    HI_U16 det_hy_en;
    HI_U16 det_hc_en;
    HI_U16 det_vy_en;

    //VPSS_DBD_TSTBLKNUM
    HI_U16 hy_tst_blk_num;
    HI_U16 hc_tst_blk_num;
    HI_U16 vy_tst_blk_num;

    //VPSS_DBD_BLKSIZE
    HI_U16 min_blk_size;
    HI_U16 hy_max_blk_size;
    HI_U16 hc_max_blk_size;
    HI_U16 vy_max_blk_size;

    //VPSS_DB_BORD_FLAG
    HI_U32 vpss_db_bord_flag[128];

    //VPSS_DB_DIR
    HI_U16 grad_sub_ratio;

    //VPSS_DB_BLK
    HI_U16 lum_h_blk_size;
    HI_U16 lum_v_blk_size;
    HI_U16 chr_h_blk_size;
    HI_U16 chr_v_blk_size;

    //VPSS_DB_RATIO
    HI_U16 lum_hor_scale_ratio;
    HI_U16 lum_ver_scale_ratio;
    HI_U16 chr_hor_scale_ratio;
    HI_U16 lum_hor_filter_sel;
    HI_U16 chr_hor_filter_sel;
    HI_U16 lum_hor_txt_win_size;

    //VPSS_DB_LHHF
    HI_U16 lum_hor_hf_diff_core;
    HI_U16 lum_hor_hf_diff_gain1;
    HI_U16 lum_hor_hf_diff_gain2;
    HI_U16 lum_hor_hf_var_core;
    HI_U16 lum_hor_bord_adj_gain;
    HI_U16 lum_hor_hf_var_gain1;
    HI_U16 lum_hor_hf_var_gain2;

    //VPSS_DB_LVHF
    HI_U16 lum_ver_hf_var_core;
    HI_U16 lum_ver_bord_adj_gain;
    HI_U16 lum_ver_hf_var_gain1;
    HI_U16 lum_ver_hf_var_gain2;
    HI_U16 lum_hor_adj_gain;
    HI_U16 lum_ver_adj_gain;

    //VPSS_DB_CTRS
    HI_U16 max_lum_hor_db_dist;
    HI_U16 max_chr_hor_db_dist;

    VDH_INFO_S stTransVDHInf;
    DB_REG_S   stDbRegs;
} DB_API_REG_S;

/* DB、DM运算需要VPSS传递进来的信息 */
typedef struct
{
    HI_U32  u32HandleNo;

    HI_U32  u32BitDepth;
    HI_U32  u32Width;
    HI_U32  u32Height;
    HI_U32  u32LeftOffset;
    HI_U32  u32TopOffset;
    HI_U32  u32RightOffset;
    HI_U32  u32BottomOffset;

    HI_BOOL bDbBlkPosModeEn;        /* VDH 信息使能 */
    HI_BOOL bDbFltDetModeEn;        /* 检测开关 */
    HI_BOOL bDBLumHorEn;            /* 水平滤波开关 */
    HI_BOOL bDBLumVerEn;            /* 垂直滤波开关 */

    HI_U32  u32SingleModeFlag;      /* 单场模式标识; 1:单场;0:非单场 */
    HI_U32  u32DBDeiEnFlag;         /* DEI 使能标识 */

    HI_U32  u32Protocol;            /* 解码协议 */
    HI_U32  u32ImageAvgQP;          /* 图像平均 QP */
    HI_U32  u32VdhInfoAddr;         /* dbinfo 地址 */
    HI_U32  u32VdhInfoStride;       /* 信息 stride, 算法约定为(图像宽度+7)/8 */
    HI_U32  u32ImageStruct;         /* 帧/场编码标识 */
    HI_U32  u32FlagLine;            /* 当前场极性标识 */

    HI_VOID* pstVPSSWbcReg;
} DB_CALC_INFO_S;

HI_S32 PQ_MNG_ALG_DBCfgDetector(DB_API_REG_S* pInput, DB_DET_INFO_S* p_Inf, const DetStaticUsing_S* pDetUsing_in);
HI_S32 PQ_MNG_ALG_GetDbDetInfo(DB_DET_INFO_S* p_InfOut, DetStaticUsing_S* p_DetUsing_info, DB_DET_INFO_S* p_InfIn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif



