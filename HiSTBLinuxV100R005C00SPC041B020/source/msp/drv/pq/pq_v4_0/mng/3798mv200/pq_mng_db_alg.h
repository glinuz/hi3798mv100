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

#define READ_STROW_DATA_NUM     1920
#define DET_BLK_STATIC_INFO     2048
#define DB_DETECTOR             2048


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
    //configure by upper application
    HI_S32 BitDepth;
    HI_U8  u8DbEn;
    HI_U8  u8DBLumVerEn;           //reg_in [0,1]
    HI_U8  u8DBLumHorEn;           //reg_in [0,1]
    HI_U8  u8DBChrVerEn;           //reg_in [0,1]
    HI_U8  u8DBChrHorEn;           //reg_in [0,1]
    HI_U8  u8DBChrHorMidGreyEn;    //reg_in [0,1]  for db hor chroma lite version only

    HI_U32 u32DBYWidth;  //reg_in [0,4095]
    HI_U32 u32DBYHeight; //reg_in [0,2160]
    HI_S32 YStride;      //reg_in [0,4096]
    HI_S32 CWidth;       //reg_in [0,4095]
    HI_S32 CStride;      //reg_in [0,4096]
    HI_S32 CHeight;      //reg_in [0,2160]

    //position
    HI_U16  u16LumStartX;          //non-interface register
    HI_U16  u16LumEndX;            //non-interface register
    HI_U16  u16LumStartY;          //non-interface register
    HI_U16  u16LumEndY;            //non-interface register
    HI_U16  u16ChrStartX;          //non-interface register
    HI_U16  u16ChrEndX;            //non-interface register
    HI_U16  u16ChrStartY;          //non-interface register
    HI_U16  u16ChrEndY;            //non-interface register
    HI_U16* pYInBuf;
    HI_U16* pYOutBuf;
    HI_U16* pUInBuf;
    HI_U16* pUOutBuf;
    HI_U16* pVInBuf;
    HI_U16* pVOutBuf;

    //direction det
    HI_U8 u8GradSubRatio;          //reg_in [0,31]
    HI_U8 u8CtrstThresh;           //reg_in [0,31] !!! need shifted for depth
    HI_U8 u8DirStrGainLut[8];
    HI_U8 u8DirStrLut[16];

    //register get from detector
    HI_U8* pLumHorBordFlag;        //reg_in ,buffer of 1 bit border flag, here only pointer
    HI_U8* pLumVerBordFlag;        //reg_in ,buffer of 1 bit border flag, here only pointer
    HI_U8* pChrHorBordFlag;        //reg_in ,buffer of 1 bit border flag, here only pointer
    HI_U8* pChrVerBordFlag;        //reg_in ,buffer of 1 bit border flag, here only pointer

    HI_U16 u16DbLumHorBlkArray[MAX_HOR_BLOCK_NUM];   //non-interface register, derived from 1bit flag
    HI_U16 u16DbLumVerBlkArray[MAX_VER_BLOCK_NUM];   //non-interface register, derived from 1bit flag
    HI_U16 u16DbChrHorBlkArray[MAX_HOR_BLOCK_NUM];   //non-interface register, derived from 1bit flag
    HI_U16 u16DbChrVerBlkArray[MAX_VER_BLOCK_NUM];   //non-interface register, derived from 1bit flag

    //calc from upper know paras
    HI_U8 u8LumHorScaleRatio;      //reg_in [0,3]
    HI_U8 u8LumVerScaleRatio;      //reg_in [0,3]
    HI_U8 u8ChrHorScaleRatio;      //reg_in [0,3]
    HI_U8 u8ChrVerScaleRatio;      //reg_in [0,3]
    HI_U8 u8LumHorFilterSel;       //reg_in [0,3]
    HI_U8 u8ChrHorFilterSel;       //reg_in [0,3]

    //luminance para
    HI_U8 u8DBLumTrue;             //non-interface register
    HI_U8 u8DBLumVerTrue;          //non-interface register
    HI_U8 u8DBLumHorTrue;          //non-interface register

    HI_S32 s32LumHBlkNum;          //reg_in [0,480]
    HI_S32 s32LumVBlkNum;          //reg_in [0,180]

    //chrominace para
    HI_U8 u8DBCbTrue;              //non-interface register
    HI_U8 u8DBCrTrue;              //non-interface register
    HI_U8 u8DBChrVerTrue;          //non-interface register
    HI_U8 u8DBChrHorTrue;          //non-interface register

    HI_S32 s32ChrHBlkNum;          //reg_in [0,480]
    HI_S32 s32ChrVBlkNum;          //reg_in [0,480]

    HI_U8 u8GlobalDbStrengthLum;   //reg_in [0,15]
    HI_U8 u8GlobalDbStrengthChr;   //reg_in [0,15]
    HI_U8 u8LumHorTxtWinSize;      //reg_in {4,8,12} only 3 possible value
    HI_U8 u8LumVerTxtWinSize;      //non-interface register fix to 3,

    HI_U8* pLumHorDeltaLut;        //reg_in 1 row x 16 col lut, 4 bit each
    HI_U8* pLumVerDeltaLut;        //reg_in 1 row x 16 col lut, 4 bit each
    HI_U8* pChrHorDeltaLut;        //reg_in 1 row x 16 col lut, 4 bit each
    HI_U8* pChrVerDeltaLut;        //reg_in 1 row x 16 col lut, 4 bit each
    HI_U16 u16LumHorHFDiffCore;    //reg_in [0,127]
    HI_U16 u16LumHorHFDiffGain1;   //reg_in [0,15]
    HI_U16 u16LumHorHFDiffGain2;   //reg_in [0,15]
    HI_U16 u16LumHorHFVarCore;     //reg_in [0,127]
    HI_U16 u16LumHorHFVarGain1;    //reg_in [0,15]
    HI_U16 u16LumHorHFVarGain2;    //reg_in [0,15]
    HI_U16 u16LumHorBordAdjGain;   //reg_in [0,7]
    HI_U16 u16ChrHorHFDiffCore;    //reg_in [0,127]
    HI_U16 u16ChrHorHFDiffGain1;   //reg_in [0,15]
    HI_U16 u16ChrHorHFDiffGain2;   //reg_in [0,15]
    HI_U16 u16ChrHorBordAdjGain;   //reg_in [0,7]

    HI_U16 u16LumVerHFVarCore;    //reg_in [0,127]
    HI_U16 u16LumVerHFVarGain1;   //reg_in [0,15]
    HI_U16 u16LumVerHFVarGain2;   //reg_in [0,15]
    HI_U16 u16LumVerBordAdjGain;  //reg_in [0,7]
    HI_U16 u16ChrVerBordAdjGain;  //reg_in [0,7]

    HI_U16 u16CtrstAdjCore;       //reg_in [0,31]
    HI_U16 u16CtrstAdjGain1;      //reg_in [0,15]
    HI_U16 u16CtrstAdjGain2;      //reg_in [0,15]
    HI_U16 u16DirSmoothMode;      //reg_in [0,2] 0, no smooth; 1, LPF mode; 2, Median mode;
    //pixel val adj
    HI_U16 u16LumHorAdjGain;      //reg_in [0,31]
    HI_U16 u16LumVerAdjGain;      //reg_in [0,31]
    HI_U16 u16ChrHorAdjGain;      //reg_in [0,31]
    HI_U16 u16ChrVerAdjGain;      //reg_in [0,31]
    // filter register
    HI_U8    u8MaxLumHorDbDist;   //reg_in [0,12]
    HI_U8    u8MaxChrHorDbDist;   //reg_in [0,12]
    HI_U8*   pLumHStrFadeLut;     //reg_in 1 row x 12 col lut, 3 bit each
    HI_U8*   pLumVStrFadeLut;     //reg_in 1 row x 12 col lut, 3 bit each
    HI_U8*   pChrHStrFadeLut;     //reg_in 1 row x 12 col lut, 3 bit each
    HI_U8*   pChrVStrFadeLut;     //reg_in 1 row x 12 col lut, 3 bit each
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



