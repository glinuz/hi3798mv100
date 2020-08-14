/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_comm.h
  Version       : Initial Draft
  Author        : Hisilicon PQ software group
  Created       : 2013/06/04
  Description   : pq common define

******************************************************************************/

#ifndef __PQ_COMM_H__
#define __PQ_COMM_H__


#include "hi_type.h"
#include "hi_debug.h"
#include "hi_module.h"
#include "hi_drv_struct.h"
#include "drv_pq_define.h"
#include "pq_comm_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif/*cplusplus*/

#define VDP_PQ_NAME  "/dev/"UMAP_DEVNAME_PQ

typedef struct hiPQ_FLASH_INFO_S
{
    HI_CHAR             Name[32];
    HI_U32              u32Offset;
    HI_U32              u32Size;
} PQ_FLASH_INFO_S;
/*--------------------------------------------------------------------------------*
Function        :HI_S32  PQ_DRV_Init()
Description     : 初始化PQ设备 模块
Input           :NA
Output
Return          :HI_SUCCESS, 初始化成功
                   :HI_FAILURE, 初始化失败
Others          :NA
*--------------------------------------------------------------------------------*/

HI_S32                PQ_DRV_Init(HI_VOID);

/*--------------------------------------------------------------------------------*
Function        :HI_S32  PQ_DRV_Deinit()
Description     : 去初始化PQ设备 模块
Input           :NA
Output          :NA
Return          :HI_SUCCESS, 去初始化成功
                   :HI_FAILURE, 去初始化失败
Others          :NA
*--------------------------------------------------------------------------------*/
HI_S32                PQ_DRV_Deinit(HI_VOID);


/*--------------------------------------------------------------------------------*
Function        :PQ_DRV_WriteReg(HI_U32 u32RegAddr, HI_U32 u32Value)
Description     : 写物理寄存器
Input           :u32RegAddr, 需要写入的寄存器地址
                    u32Value,写入的值
                    *pstBitRange  需要读取的寄存器比特位
Output          :NA
Return          :HI_SUCCESS
                   :HI_FAILURE
Others          :NA
*--------------------------------------------------------------------------------*/
HI_S32 PQ_DRV_WriteReg(HI_U32 u32RegAddr, HI_U32 u32Value, PQ_DBG_BIT_RANGE_S* pstBitRange);


/*--------------------------------------------------------------------------------*
Function        :PQ_DRV_ReadReg(HI_U32 u32RegAddr, HI_U32 u32Value, PQ_DBG_BIT_RANGE_S* pstBitRange)
Description     : 读物理寄存器
Input           :u32RegAddr, 需要读取的寄存器地址
                    *pstBitRange  需要读取的寄存器比特位
Output          :pu32Value，读取的值
Return          :HI_SUCCESS
                   :HI_FAILURE
Others          :NA
*--------------------------------------------------------------------------------*/
HI_S32 PQ_DRV_ReadReg(HI_U32 u32RegAddr, HI_U32* pu32Value, PQ_DBG_BIT_RANGE_S* pstBitRange);



/*--------------------------------------------------------------------------------*
Function        :PQ_DRV_ReadGroupPara(HI_U32 u32RegAddr, HI_U32* pu32Value,HI_U32 u32Len)
Description     : 读物理寄存器
Input             :u32RegAddr, 需要读取的虚拟寄存器地址(可理解为参数组ID)
                    :u32Len ,需要读取的长度，用来校验
Output          :pu32Value，读取的值
Return          :HI_SUCCESS
                   :HI_FAILURE
Others          :NA
*--------------------------------------------------------------------------------*/
HI_S32 PQ_DRV_ReadFromPqDriver(HI_U32 u32RegAddr, HI_U32* pu32Data, HI_U32 u32Len);

/*--------------------------------------------------------------------------------*
Function        :PQ_DRV_WriteGroupPara(HI_U32 u32RegAddr, const HI_U32* pu32Value,HI_U32 u32Len)
Description     : 读物理寄存器
Input             :u32RegAddr, 需要写入的虚拟寄存器地址(可理解为参数组ID)
                    :u32Len ,需要写入的长度，用来校验
Output          :pu32Value，写入的值
Return          :HI_SUCCESS
                   :HI_FAILURE
Others          :NA
*--------------------------------------------------------------------------------*/
HI_S32 PQ_DRV_WriteToPqDriver(HI_U32 u32RegAddr, const HI_U32* pu32Data, HI_U32 u32Len);

HI_S32 PQ_DRV_WriteACM(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen);

HI_S32 PQ_DRV_ReadDCI(HI_U32 u32RegAddr, HI_U8* pu8Buffer, HI_U32 u32BufferLen);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif/*__cplusplus*/

#endif/*__MPI_PQ_COMM_H__*/
