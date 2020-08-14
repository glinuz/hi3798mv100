/******************************************************************************
  *
  * Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
  *
  * This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
  *  and may not be copied, reproduced, modified, disclosed to others, published or used, in
  * whole or in part, without the express prior written permission of Hisilicon.
  *
******************************************************************************
  File Name     : pq_hal_dci.h
  Version       : Initial Draft
  Author        :
  Created       : 2013/04/20
  Description   :

******************************************************************************/

#ifndef __PQ_HAL_DCI_H__
#define __PQ_HAL_DCI_H__

#include "hi_type.h"
#include "pq_hal_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define DCI_GAIN_ADDR      0xf8cc612c
#define DCI_GAIN0_LSB      26
#define DCI_GAIN0_MSB      31
#define DCI_GAIN1_LSB      20
#define DCI_GAIN1_MSB      25
#define DCI_GAIN2_LSB      14
#define DCI_GAIN2_MSB      19


/*DCI LUT表结构*/
typedef struct hiDCI_LUT_S
{
    HI_S16 s16HistCoef0[32];  /* 直方图加权系数0 */
    HI_S16 s16HistCoef1[32];  /* 直方图加权系数1 */
    HI_S16 s16HistCoef2[32];  /* 直方图加权系数2 */
    HI_S16 s16YDivCoef[64];   /* 除法表 */
    HI_S16 s16WgtCoef0[33];   /* DCI weight LUT0 */
    HI_S16 s16WgtCoef1[33];   /* DCI weight LUT1 */
    HI_S16 s16WgtCoef2[33];   /* DCI weight LUT2 */
    HI_U32 s32DciCfgReg[28];
} DCI_LUT_S;

/*DCI曲线配置表*/
typedef struct hiDCI_WGT_S
{
    HI_S16 s16WgtCoef0[33];
    HI_S16 s16WgtCoef1[33];
    HI_S16 s16WgtCoef2[33];
    HI_U16 u16Gain0;
    HI_U16 u16Gain1;
    HI_U16 u16Gain2;
} DCI_WGT_S;

/*DCI Black Stretch*/
typedef struct hiDCI_BS_LUT_S
{
    HI_S16 s16BSDelta[320];    /*二次曲线与斜率为1的曲线的对应点差值*/
    HI_S16 s16VertOffset[160]; /*黑扩展曲线垂直偏移*/
} DCI_BS_LUT_S;


/*DCI直方图结构*/
typedef struct hiDCI_HISTGRAM_S
{
    HI_S32 s32HistGram[32];
} DCI_HISTGRAM_S;

/*DCI窗口配置*/
typedef struct hiDCI_WINDOW_S
{
    HI_U16 u16Hstart;
    HI_U16 u16Hend;
    HI_U16 u16Vstart;
    HI_U16 u16Vend;
} DCI_WINDOW_S;

/* DCI Demo Mode */
typedef enum hiDCI_DEMO_MODE_E
{
    DCI_DEMO_ENABLE_R = 0,
    DCI_DEMO_ENABLE_L    ,

    DCI_DEMO_ENABLE_BUTT
} DCI_DEMO_MODE_E;


/* 初始化DCI模块*/
HI_S32  PQ_HAL_DCIInit(HI_VOID);
/* 去初始化DCI模块 */
HI_S32  PQ_HAL_DCIDeinit(HI_VOID);
/* 开关卖场模式 */
HI_VOID PQ_HAL_EnableDCIDemo(PQ_HAL_LAYER_VID_E u32ChId, HI_BOOL bOnOff);
HI_VOID PQ_HAL_SetDCIDemoMode(PQ_HAL_LAYER_VID_E u32ChId, DCI_DEMO_MODE_E enMode);
/* 开关DCI模块 */
HI_VOID PQ_HAL_EnableDCI(PQ_HAL_LAYER_VID_E u32ChId, HI_BOOL bOnOff);
/*开关DCI BS模块*/
HI_VOID PQ_HAL_EnableDciBS(PQ_HAL_LAYER_VID_E u32ChId, HI_BOOL bOnOff);

/* 获取DCI强度 */
HI_VOID PQ_HAL_GetDCIlevel(PQ_HAL_LAYER_VID_E u32ChId, HI_U16 *pu16Gain0, HI_U16 *pu16Gain1, HI_U16 *pu16Gain2);
/* 设置DCI强度 */
HI_VOID PQ_HAL_SetDCIlevel(PQ_HAL_LAYER_VID_E u32ChId, HI_U16 u16Gain0, HI_U16 u16Gain1, HI_U16 u16Gain2);
/* 设置DCI统计窗口 */
HI_VOID PQ_HAL_SetDCIWindow(PQ_HAL_LAYER_VID_E u32ChId, HI_U16 u16HStar, HI_U16 u16HEnd, HI_U16 u16VStar, HI_U16 u16VEnd);
/* 获取DCI作用窗口 */
HI_VOID PQ_HAL_GetDCIWindow(HI_U16 *u16HStar, HI_U16 *u16HEnd, HI_U16 *u16VStar, HI_U16 *u16VEnd);
/* 设置DCI配置曲线 */
HI_S32  PQ_HAL_SetDCIWgtLut(DCI_WGT_S *pstDciCoef);
/* 获取DCI配置曲线 */
HI_S32  PQ_HAL_GetDCIWgtLut(DCI_WGT_S *pstDciCoef);
/* 设置DCI的LUT */
HI_S32  PQ_HAL_SetDciLut(DCI_LUT_S *pstDciLut);
/* 获取DCI直方图 */
HI_S32  PQ_HAL_GetDCIHistgram(DCI_HISTGRAM_S *pstDciHist);

HI_S32  PQ_HAL_SetDciBSLut(DCI_BS_LUT_S *pstDciBSLut);

/* 设置DCI BS配置曲线 */
HI_S32  PQ_HAL_SetDciBSCoef(DCI_BS_LUT_S *pstDciCoef);
/* 获取DCI BS 配置曲线 */
HI_S32  PQ_HAL_GetDciBSCoef(DCI_BS_LUT_S *pstDciCoef);

HI_VOID PQ_HAL_UpdateDCICoef(PQ_HAL_LAYER_VID_E u32ChId);

HI_S32 PQ_HAL_SetDCIDemoPos(PQ_HAL_LAYER_VID_E u32ChId, HI_U32 u32Pos);

HI_S32 PQ_HAL_GetDCIDemoPos(PQ_HAL_LAYER_VID_E u32ChId, HI_U32 *u32Pos);

HI_VOID PQ_HAL_UpdataDCIScd(PQ_HAL_LAYER_VID_E u32ChId, HI_BOOL EnSceneChange);

HI_S32 PQ_HAL_SetDciAlphaBlend(PQ_HAL_LAYER_VID_E u32VId, HI_U32 u32AlphaBlend);

HI_S32 PQ_HAL_SetDciClipLimit(PQ_HAL_LAYER_VID_E u32VId, HI_U32 u32ClipLimitR, HI_U32 u32ClipLimitL);

HI_S32 PQ_HAL_SetDciManAlgBlendEn(PQ_HAL_LAYER_VID_E u32VId, HI_BOOL bEn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
