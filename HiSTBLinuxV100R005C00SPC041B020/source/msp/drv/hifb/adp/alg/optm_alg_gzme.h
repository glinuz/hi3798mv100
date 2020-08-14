/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : optm_alg_gzme.h
Version             : Initial Draft
Author              :
Created             : 2014/09/09
Description         : ALG这一层代码是由算法提供的
Function List       :
History             :
Date                       Author                   Modification
2014/09/09                 y00181162                Created file
******************************************************************************/

#ifndef __OPTM_ALG_GZME_H__
#define __OPTM_ALG_GZME_H__


/*********************************add include here******************************/
#include "hi_type.h"
#ifndef HI_BUILD_IN_BOOT
#include "hi_drv_mmz.h"
#else
#include "hi_common.h"
#endif

#include "hi_gfx_comm_k.h"
#include "drv_hifb_config.h"


/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C"
{
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

#define OPTM_ALG_G_HZME_PRECISION     CONFIG_HIFB_ALG_G_HZME_PRECISION
#define OPTM_ALG_G_VZME_PRECISION     CONFIG_HIFB_ALG_G_VZME_PRECISION
#define OPTM_VouBitValue(a)    (a)


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
} OPTM_ALG_GZME_COEF_ADDR_S;


typedef struct
{
    GFX_MMZ_BUFFER_S stMBuf; /** common drv **/
    OPTM_ALG_GZME_COEF_ADDR_S stZmeCoefAddr;
}OPTM_ALG_GZME_MEM_S;




typedef struct
{
    HI_U32 u32ZmeFrmWIn;    /*zme input frame width*/
    HI_U32 u32ZmeFrmHIn;    /*zme input frame height*/
    HI_U32 u32ZmeFrmWOut;   /*zme output frame width*/
    HI_U32 u32ZmeFrmHOut;   /*zme output frame height*/
    HI_U32 u32ZmeHdDeflicker; /*DeFlicker */
    HI_U32 u32ZmeSdDeflicker; /*DeFlicker*/

    HI_BOOL bZmeFrmFmtIn;    /*Frame format for zme input: 0-field; 1-frame*/
    HI_BOOL bZmeFrmFmtOut;   /*Frame format for zme Output: 0-field; 1-frame*/
}OPTM_ALG_GZME_DRV_PARA_S;



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
}OPTM_ALG_GZME_RTL_PARA_S;

typedef enum
{
    GZME_COEF_1   = 0,
    GZME_COEF_E1     ,
    GZME_COEF_075    ,
    GZME_COEF_05     ,
    GZME_COEF_033    ,
    GZME_COEF_025    ,
    GZME_COEF_0      ,
    GZME_COEF_RATIO_BUTT

} OPTM_GZME_COEF_RATIO_E;

typedef enum
{
    GZME_COEF_8T8P_LH   = 0,
    GZME_COEF_4T16P_LV     ,
    GZME_COEF_8T8P_CH      ,
    GZME_COEF_4T16P_CV     ,
    GZME_COEF_2T16P_LV     ,//add from HiFoneB2 4K tiny zme by d00241380
    GZME_COEF_2T16P_CV     ,//add from HiFoneB2 4K tiny zme by d00241380
    GZME_COEF_TYPE_BUTT
} OPTM_GZME_COEF_TYPE_E;

typedef enum
{
    GDTI_COEF_2T8P_LH  = 0,
    GDTI_COEF_2T16P_LV    ,
    GDTI_COEF_2T8P_CH     ,
    GDTI_COEF_2T16P_CV    ,
    GDTI_COEF_TYPE_BUTT
} OPTM_GDTI_COEF_TYPE_E;

/**================ begin ==================**/
/**
 **add from HiFoneB2 4K tiny zme by d00241380
 **/
typedef enum tagOPTM_GZME_HVORDER_E
{
    GZME_ORDER_HV = 0x0 , //Hor First
    GZME_ORDER_VH        ,  //Ver First

    GZME_ORDER_BUTT
} OPTM_GZME_ORDER_E;

typedef enum tagOPTM_GZME_VER_TAP_E
{
    GZME_VER_TAP_4 = 0x0 , //Ver 4Tap
    GZME_VER_TAP_2         ,  //Ver 2Tap

    GZME_VER_TAP_BUTT
} OPTM_GZME_VER_TAP_E;

/**================ end ==================**/

typedef struct
{
    HI_S32    bits_0    :    10    ;
    HI_S32    bits_1    :    10    ;
    HI_S32    bits_2    :    10    ;
    HI_S32    bits_32    :    2    ;
    HI_S32    bits_38    :    8    ;
    HI_S32    bits_4    :    10    ;
    HI_S32    bits_5    :    10    ;
    HI_S32    bits_64    :    4    ;
    HI_S32    bits_66    :    6    ;
    HI_S32    bits_7    :    10    ;
    HI_S32    bits_8    :    10    ;
    HI_S32    bits_96    :    6    ;
    HI_S32    bits_94    :    4    ;
    HI_S32    bits_10    :    10    ;
    HI_S32    bits_11    :    10    ;
    HI_S32    bits_12    :    8    ;
} OPTM_ZME_COEF_BIT_S;

typedef struct
{
    HI_U32          u32Size;
    OPTM_ZME_COEF_BIT_S  stBit[12];
} OPTM_ZME_COEF_BITARRAY_S;



/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/

HI_S32 OPTM_ALG_GZmeVdpComnInit(OPTM_ALG_GZME_MEM_S *pstGZmeCoefMem);
HI_VOID OPTM_ALG_GZmeVdpComnDeInit(OPTM_ALG_GZME_MEM_S *pstGZmeCoefMem);
HI_VOID OPTM_ALG_GZmeHDSet(OPTM_ALG_GZME_MEM_S *pstMem, OPTM_ALG_GZME_DRV_PARA_S *pstZmeDrvPara, OPTM_ALG_GZME_RTL_PARA_S *pstZmeRtlPara);
HI_VOID OPTM_ALG_GZmeSDSet(OPTM_ALG_GZME_MEM_S *pstMem, OPTM_ALG_GZME_DRV_PARA_S *pstZmeDrvPara, OPTM_ALG_GZME_RTL_PARA_S *pstZmeRtlPara);

#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __OPTM_ALG_GZME_H__ */
