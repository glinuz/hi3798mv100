/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_db.h
  Version       : Initial Draft
  Author        :
  Created       : 2015/10/09
  Description   : De-blocking

******************************************************************************/

#ifndef __PQ_HAL_DB_H__
#define __PQ_HAL_DB_H__


#include "hi_type.h"
#include "pq_hal_comm.h"
#include "pq_mng_db_alg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/* dbm demo mode */
typedef enum
{
    DBM_DEMO_ENABLE_L = 0, /* Enable Left = 0 */
    DBM_DEMO_ENABLE_R    ,
    DBM_DEMO_BUTT
} DBM_DEMO_MODE_E;


//定义DB模块中寄存器所使用到的参数值
typedef struct
{
    HI_U16 u16DbmOutMode;
    /* BEGIN: Added by yuanle, 2014/8/22 */
    HI_S32 YWidth;   //reg_in [0,4095]
    HI_S32 YStride;  //reg_in [0,4096]
    HI_S32 YHeight;  //reg_in [0,2160]
    HI_S32 CWidth;   //reg_in [0,4095]
    HI_S32 CStride;  //reg_in [0,4096]
    HI_S32 CHeight;  //reg_in [0,2160]
    HI_U16* pYInBuf;
    HI_U16* pUInBuf;
    HI_U16* pVInBuf;
    /* END:   Added by yuanle, 2014/8/22 */

    HI_U8 u8BitDepthPrc;//8,10

    HI_U8 DetSizeEn;   //[0,1]

    HI_U8 hyEn;      //[0,1]
    HI_U8 hchrEn;    //[0,1]
    HI_U8 vyEn;      //[0,1]
    HI_U8 vchrEn;    //[0,1]

    //这个表示当edge的强度小于这个值就可能这个位置是border
    HI_U16 Hythreshold_edge;   //8bit [0 63]
    HI_U16 Vythreshold_edge;   //8bit [0 63]
    HI_U16 Hchrthreshold_edge; //8bit [0 63]
    HI_U16 Vchrthreshold_edge; //8bit [0 63]

    //左右各三个像素点最大值必须小于这个值，则认为是平坦的，大于这个值则认为纹理比较丰富
    HI_U16 Hythreshold_texture;   //8bit [0 63]
    HI_U16 Vythreshold_texture;   //8bit [0 63]
    HI_U16 Hchrthreshold_texture; //8bit [0 63]
    HI_U16 Vchrthreshold_texture; //8bit [0 63]

    HI_U8 lutweight[16]; //[0,16]

    HI_U8 HyRationBorderText;   //[8,31]  /8
    HI_U8 VyRationBorderText;   //[8,31]  /8
    HI_U8 HchrRationBorderText; //[8,31]  /8
    HI_U8 VchrRationBorderText; //[8,31]  /8

    HI_U32 HyTestBlkNumber;   //[0,1023]
    HI_U32 HchrTestBlkNumber; //[0,1023]
    HI_U32 VyTestBlkNumber;   //[0,1023]
    HI_U32 VchrTestBlkNumber; //[0,1023]

    HI_U16 HyRadix8BinLut0;  //[0,2047]
    HI_U16 HyRadix8BinLut1;  //[0,2047]
    HI_U16 HyRadix8BinLut2;  //[0,2047]
    HI_U16 HyRadix8BinLut3;  //[0,2047]
    HI_U16 HyRadix8BinLut4;  //[0,2047]
    HI_U16 HyRadix8BinLut5;  //[0,2047]
    HI_U16 HyRadix8BinLut6;  //[0,2047]

    HI_U16 VyRadix8BinLut0;  //[0,2047]
    HI_U16 VyRadix8BinLut1;  //[0,2047]
    HI_U16 VyRadix8BinLut2;  //[0,2047]
    HI_U16 VyRadix8BinLut3;  //[0,2047]
    HI_U16 VyRadix8BinLut4;  //[0,2047]
    HI_U16 VyRadix8BinLut5;  //[0,2047]
    HI_U16 VyRadix8BinLut6;  //[0,2047]

    HI_U16 HchrRadix8BinLut0;  //[0,2047]
    HI_U16 HchrRadix8BinLut1;  //[0,2047]
    HI_U16 HchrRadix8BinLut2;  //[0,2047]
    HI_U16 HchrRadix8BinLut3;  //[0,2047]
    HI_U16 HchrRadix8BinLut4;  //[0,2047]
    HI_U16 HchrRadix8BinLut5;  //[0,2047]
    HI_U16 HchrRadix8BinLut6;  //[0,2047]

    HI_U16 VchrRadix8BinLut0;  //[0,2047]
    HI_U16 VchrRadix8BinLut1;  //[0,2047]
    HI_U16 VchrRadix8BinLut2;  //[0,2047]
    HI_U16 VchrRadix8BinLut3;  //[0,2047]
    HI_U16 VchrRadix8BinLut4;  //[0,2047]
    HI_U16 VchrRadix8BinLut5;  //[0,2047]
    HI_U16 VchrRadix8BinLut6;  //[0,2047]

    HI_U8 MinBlkSize;     //[0,16]
    HI_U8 HyMaxBlkSize;   //[8,64] change
    HI_U8 HchrMaxBlkSize; //[8,64]
    HI_U8 VyMaxBlkSize;   //[8,64]
    HI_U8 VchrMaxBlkSize; //[8,64]

    HI_U8 Thr_flat;       //[0,16]

    HI_U8 u8GlobalDbStrengthLum;
} DB_PARAM_S;

//DB强度值,对应VPSS_DB_WEIGHT寄存器
typedef struct
{
    HI_U8    u8HWeight;  //6bits [0,63]
    HI_U8    u8VWeight;  //6bits [0,63]
} DB_WEIGHT_S;

HI_S32 PQ_HAL_SetDBBlkNumberCfg(HI_U32 u32HandleNo, DB_PARAM_S* pstDBPara);
HI_S32 PQ_HAL_SetDBApiReg(HI_U32 u32HandleNo, DB_API_REG_S* pstApiPara);
HI_S32 PQ_HAL_GetDBDetEnReg(HI_U32 u32Data, HI_S32* pDet_size_en,
                            HI_S32* pDet_hy_en, HI_S32* pDet_hc_en, HI_S32* pDet_vy_en);
HI_S32 PQ_HAL_GetDbmEnReg(HI_U32 u32HandleNo, HI_BOOL* pbDbEn);
HI_S32 PQ_HAL_SetDBDetSizeEn(HI_U32 u32HandleNo, HI_BOOL bOnOff);
HI_S32 PQ_HAL_EnableDB(HI_U32 u32Data, HI_BOOL bOnOff);
HI_S32 PQ_HAL_EnableDBDemo(HI_U32 u32Data, HI_BOOL bOnOff);
HI_S32 PQ_HAL_SetDbmDemoMode(HI_U32 u32Data, DBM_DEMO_MODE_E enMode);
HI_S32 PQ_HAL_DBDemoPos(HI_U32 u32HandleNo, HI_U32 u32DBDemoPos);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif



