/******************************************************************************

  Copyright (C), 2014-2015, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_pq_table.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2013/10/15
  Description   :

******************************************************************************/

#ifndef __PQ_MNG_PQ_TABLE_H__
#define __PQ_MNG_PQ_TABLE_H__

#include "hi_drv_pq.h"
#include "drv_pq_comm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct hiPQ_BIN_MODULE_POS_S
{
    HI_U32 u32StartPos;
    HI_U32 u32EndPos;
} HI_PQ_BIN_MODULE_POS_S;

HI_S32 PQ_TABLE_DeInitPQTable(HI_VOID);

HI_S32 PQ_TABLE_InitPQTable(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault);

HI_S32 PQ_TABLE_LoadPhyList(PQ_REG_TYPE_E enRegType, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, HI_U32 u32HandleNo);

HI_S32 PQ_TABLE_LoadMultiList(PQ_REG_TYPE_E enRegType, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode);

HI_S32 PQ_TABLE_InitPhyList(HI_U32 u32ID, HI_PQ_MODULE_E enModule, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode);

HI_U32 PQ_TABLE_GetSoftTable(HI_U32 u32Lut, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, HI_U32 u32DefaultValue);

HI_S32 PQ_TABLE_FindBinPos(HI_PQ_MODULE_E enModule, HI_U32* pu32StartPos, HI_U32* pu32EndPos);

HI_S32 PQ_TABLE_LoadPhyListOfAlg(HI_PQ_MODULE_E enModule, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode);

HI_S32 PQ_TABLE_InitSoftTable(HI_BOOL bInit);

HI_S32 PQ_TABLE_SetTableRegDefault(HI_VOID);

HI_U32 PQ_TABLE_GetPhyListNum(HI_VOID);

HI_U32 PQ_TABLE_GetSoftListNum(HI_VOID);

HI_S32 PQ_TABLE_SetSelVdpReg(HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U32 u32Value);

HI_S32 PQ_TABLE_SetVpssReg(HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U32 u32Value);

HI_S32 PQ_TABLE_SetVdpReg(HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U32 u32Value);

PQ_PHY_REG_S* PQ_TABLE_GetPhyReg(HI_VOID);

PQ_PHY_REG_S* PQ_TABLE_GetSoftReg(HI_VOID);







#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
