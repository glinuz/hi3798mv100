/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_gfxzme.h
  Version       : Initial Draft
  Author        : pengjunwei 00203646
  Created       : 2015/10/15
  Description   :

******************************************************************************/

#ifndef __PQ_HAL_GFXZME_H__
#define __PQ_HAL_GFXZME_H__

#include "hi_type.h"
#include "pq_hal_comm.h"

#ifndef HI_BUILD_IN_BOOT
#include "hi_drv_mmz.h"
#else
#include "hi_common.h"
#endif



/***************************** Macro Definition ******************************/
/**
 ** ZME COEF
 ** 处理水平精度查询
 ** 98MV100/HIFONE-STB: 水平精度查询是按照20bit
 ** 其它: 水平精度查询是按照12bit
 ** 本来是按照12bit计算的，按照这个再进行偏移
 **/
#define PQ_PRECISION_GZMERATIONH_12BITOFFSET     8

#define PQ_G_HZME_PRECISION                      1048576
#define PQ_G_VZME_PRECISION                      4096


/*************************** Structure Definition ****************************/
typedef struct
{

    HI_U32 u32ZmeCoefAddrHL_1;
    HI_U32 u32ZmeCoefAddrHL_E1;
    HI_U32 u32ZmeCoefAddrHL_075;
    HI_U32 u32ZmeCoefAddrHL_05;
    HI_U32 u32ZmeCoefAddrHL_033;
    HI_U32 u32ZmeCoefAddrHL_025;
    HI_U32 u32ZmeCoefAddrHL_0;

    HI_U32 u32ZmeCoefAddrHC_1;
    HI_U32 u32ZmeCoefAddrHC_E1;
    HI_U32 u32ZmeCoefAddrHC_075;
    HI_U32 u32ZmeCoefAddrHC_05;
    HI_U32 u32ZmeCoefAddrHC_033;
    HI_U32 u32ZmeCoefAddrHC_025;
    HI_U32 u32ZmeCoefAddrHC_0;

    HI_U32 u32ZmeCoefAddrVL_1;
    HI_U32 u32ZmeCoefAddrVL_E1;
    HI_U32 u32ZmeCoefAddrVL_075;
    HI_U32 u32ZmeCoefAddrVL_05;
    HI_U32 u32ZmeCoefAddrVL_033;
    HI_U32 u32ZmeCoefAddrVL_025;
    HI_U32 u32ZmeCoefAddrVL_0;

    HI_U32 u32ZmeCoefAddrVC_1;
    HI_U32 u32ZmeCoefAddrVC_E1;
    HI_U32 u32ZmeCoefAddrVC_075;
    HI_U32 u32ZmeCoefAddrVC_05;
    HI_U32 u32ZmeCoefAddrVC_033;
    HI_U32 u32ZmeCoefAddrVC_025;
    HI_U32 u32ZmeCoefAddrVC_0;

    HI_U32 u32ZmeSDCoefAddrHL_1;
    HI_U32 u32ZmeSDCoefAddrHL_E1;
    HI_U32 u32ZmeSDCoefAddrHL_075;
    HI_U32 u32ZmeSDCoefAddrHL_05;
    HI_U32 u32ZmeSDCoefAddrHL_033;
    HI_U32 u32ZmeSDCoefAddrHL_025;
    HI_U32 u32ZmeSDCoefAddrHL_0;

    HI_U32 u32ZmeSDCoefAddrHC_1;
    HI_U32 u32ZmeSDCoefAddrHC_E1;
    HI_U32 u32ZmeSDCoefAddrHC_075;
    HI_U32 u32ZmeSDCoefAddrHC_05;
    HI_U32 u32ZmeSDCoefAddrHC_033;
    HI_U32 u32ZmeSDCoefAddrHC_025;
    HI_U32 u32ZmeSDCoefAddrHC_0;

    HI_U32 u32ZmeSDCoefAddrVL_1;
    HI_U32 u32ZmeSDCoefAddrVL_E1;
    HI_U32 u32ZmeSDCoefAddrVL_075;
    HI_U32 u32ZmeSDCoefAddrVL_05;
    HI_U32 u32ZmeSDCoefAddrVL_033;
    HI_U32 u32ZmeSDCoefAddrVL_025;
    HI_U32 u32ZmeSDCoefAddrVL_0;

    HI_U32 u32ZmeSDCoefAddrVC_1;
    HI_U32 u32ZmeSDCoefAddrVC_E1;
    HI_U32 u32ZmeSDCoefAddrVC_075;
    HI_U32 u32ZmeSDCoefAddrVC_05;
    HI_U32 u32ZmeSDCoefAddrVC_033;
    HI_U32 u32ZmeSDCoefAddrVC_025;
    HI_U32 u32ZmeSDCoefAddrVC_0;

    /**================ begin ==================**/
    /**
     **add from HiFoneB2 4K tiny zme by d00241380
     **/
    HI_U32 u32Zme2T16PCoefAddrVL_1;
    HI_U32 u32Zme2T16PCoefAddrVL_E1;
    HI_U32 u32Zme2T16PCoefAddrVL_075;
    HI_U32 u32Zme2T16PCoefAddrVL_05;
    HI_U32 u32Zme2T16PCoefAddrVL_033;
    HI_U32 u32Zme2T16PCoefAddrVL_025;
    HI_U32 u32Zme2T16PCoefAddrVL_0;

    HI_U32 u32Zme2T16PCoefAddrVC_1;
    HI_U32 u32Zme2T16PCoefAddrVC_E1;
    HI_U32 u32Zme2T16PCoefAddrVC_075;
    HI_U32 u32Zme2T16PCoefAddrVC_05;
    HI_U32 u32Zme2T16PCoefAddrVC_033;
    HI_U32 u32Zme2T16PCoefAddrVC_025;
    HI_U32 u32Zme2T16PCoefAddrVC_0;
    /**================ end ==================**/
} PQ_GZME_COEF_ADDR_S;


typedef struct
{
    PQ_MMZ_BUF_S stMBuf; /** common drv **/
    PQ_GZME_COEF_ADDR_S stZmeCoefAddr;
} PQ_GZME_MEM_S;


typedef struct
{
    HI_BOOL bZmeEnH;    /*zme enable horizontal: 0-off; 1-on*/
    HI_BOOL bZmeEnV;    /*zme enable vertical: 0-off; 1-on*/

    HI_BOOL bZmeMdHLC;    /*zme mode of horizontal luma and chroma: 0-copy mode; 1-FIR filter mode*/
    HI_BOOL bZmeMdHA;     /*zme mode of horizontal alpha: 0-copy mode; 1-FIR filter mode*/
    HI_BOOL bZmeMdVLC;    /*zme mode of vertical luma and chroma: 0-copy mode; 1-FIR filter mode*/
    HI_BOOL bZmeMdVA;     /*zme mode of vertical alpha: 0-copy mode; 1-FIR filter mode*/

    HI_BOOL bZmeMedHL;   /*zme Median filter enable of horizontal luma: 0-off; 1-on*/
    HI_BOOL bZmeMedHC;
    HI_BOOL bZmeMedHA;
    HI_BOOL bZmeMedVL;   /*zme Median filter enable of vertical luma: 0-off; 1-on*/
    HI_BOOL bZmeMedVC;
    HI_BOOL bZmeMedVA;

    HI_S32 s32ZmeOffsetHL;    /* offset of horizontal luma*/
    HI_S32 s32ZmeOffsetHC;    /* offset of horizontal chroma*/
    HI_S32 s32ZmeOffsetVTop;  /* offset when output is interlaced and top field*/
    HI_S32 s32ZmeOffsetVBtm;  /* offset when output is interlaced and bottom field*/

    HI_U32 u32ZmeWIn;
    HI_U32 u32ZmeHIn;
    HI_U32 u32ZmeWOut;
    HI_U32 u32ZmeHOut;

    HI_U32 u32ZmeRatioHL;
    HI_U32 u32ZmeRatioVL;
    HI_U32 u32ZmeRatioHC;
    HI_U32 u32ZmeRatioVC;


    HI_BOOL bZmeOrder;  /*zme order of hzme and vzme: 0-hzme first; 1-vzme first*/
    HI_BOOL bZmeTapVC;  /*zme tap of vertical chroma: 0-4tap; 1-2tap*/
    /**
     **add by d00241380 for hifoneB02
     **/
    HI_BOOL bZmeTapV;      /* zme tap of vertical : 0-4tap; 1-2tap */

    HI_U8 u8ZmeYCFmtIn;   /*video format for zme input: 0-422; 1-420; 2-444*/
    HI_U8 u8ZmeYCFmtOut;  /*video format for zme output: 0-422; 1-420; 2-444*/

    HI_U32 u32ZmeCoefAddrHL;
    HI_U32 u32ZmeCoefAddrHC;
    HI_U32 u32ZmeCoefAddrVL;
    HI_U32 u32ZmeCoefAddrVC;
} PQ_GZME_RTL_PARA_S;

typedef enum
{
    PQ_GZME_MODE_HOR = 0,
    PQ_GZME_MODE_VER    ,

    PQ_GZME_MODE_HORL   ,
    PQ_GZME_MODE_HORC   ,
    PQ_GZME_MODE_VERL   ,
    PQ_GZME_MODE_VERC   ,

    PQ_GZME_MODE_ALPHA  ,
    PQ_GZME_MODE_ALPHAV ,
    PQ_GZME_MODE_VERT   ,
    PQ_GZME_MODE_VERB   ,

    PQ_GZME_MODE_ALL    ,
    PQ_GZME_MODE_NONL   ,

    PQ_GZME_MODE_BUTT
} PQ_GZME_MODE_E;

typedef enum hiPQ_GZME_GP_E
{
    PQ_GZME_GP_0 = 0x0,
    PQ_GZME_GP_1      ,

    PQ_GZME_GP_BUTT
} PQ_GZME_GP_E;


typedef enum hiPQ_GP_PARA_E
{
    PQ_GP_PARA_ZME_HOR = 0,
    PQ_GP_PARA_ZME_VER    ,

    PQ_GP_PARA_BUTT
} PQ_GP_PARA_E;

typedef enum hiPQ_GP_ORDER_E
{
    PQ_GP_ORDER_NULL     = 0x0,
    PQ_GP_ORDER_CSC      = 0x1,
    PQ_GP_ORDER_ZME      = 0x2,
    PQ_GP_ORDER_CSC_ZME  = 0x3,
    PQ_GP_ORDER_ZME_CSC  = 0x4,

    PQ_GP_ORDER_BUTT
} PQ_GP_ORDER_E;

typedef struct
{
    HI_S32  bits_0  :   10  ;
    HI_S32  bits_1  :   10  ;
    HI_S32  bits_2  :   10  ;
    HI_S32  bits_32 :   2   ;
    HI_S32  bits_38 :   8   ;
    HI_S32  bits_4  :   10  ;
    HI_S32  bits_5  :   10  ;
    HI_S32  bits_64 :   4   ;
    HI_S32  bits_66 :   6   ;
    HI_S32  bits_7  :   10  ;
    HI_S32  bits_8  :   10  ;
    HI_S32  bits_96 :   6   ;
    HI_S32  bits_94 :   4   ;
    HI_S32  bits_10 :   10  ;
    HI_S32  bits_11 :   10  ;
    HI_S32  bits_12 :   8   ;
} PQ_GZME_COEF_BIT_S;

typedef struct
{
    HI_U32          u32Size;
    PQ_GZME_COEF_BIT_S  stBit[12];
} PQ_GZME_COEF_BITARRAY_S;


HI_VOID PQ_HAL_SetGZmeEnable(HI_U32 u32Data, PQ_GZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID PQ_HAL_SetGZmePhase(HI_U32 u32Data, PQ_GZME_MODE_E enMode, HI_S32 s32Phase);
HI_VOID PQ_HAL_SetGZmeFirEnable(HI_U32 u32Data, PQ_GZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID PQ_HAL_SetGZmeMidEnable(HI_U32 u32Data, PQ_GZME_MODE_E enMode, HI_U32 u32bEnable);
HI_VOID PQ_HAL_SetGZmeHorRatio(HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID PQ_HAL_SetGZmeVerRatio(HI_U32 u32Data, HI_U32 u32Ratio);
HI_VOID PQ_HAL_SetGZmeHfirOrder(HI_U32 u32Data, HI_U32 u32HfirOrder);
HI_VOID PQ_HAL_SetGZmeCoefAddr(HI_U32 u32Data, HI_U32 u32Mode, HI_U32 u32Addr);
HI_VOID PQ_HAL_SetGZmeTabV(HI_U32 u32Data, HI_U32 u32ZmeTabV);
HI_VOID PQ_HAL_SetGZmeIpOrder(HI_U32 u32Data, HI_BOOL bSlvGp, PQ_GP_ORDER_E enIpOrder);



#endif

