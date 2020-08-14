/******************************************************************************

Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hal_cast.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2013/03/09
Last Modified :
Description   : mirror
Function List :
History       :
* main\1    2012-03-09  zgjie     init.
******************************************************************************/
#ifndef __HI_HAL_CAST_H__
#define __HI_HAL_CAST_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define AIAO_CAST_BUFSIZE_MIN 2048
/***************************** Macro Definition ******************************/
typedef enum
{
    AIAO_CAST_0 = 0x00,
    AIAO_CAST_1 = 0x01,
    AIAO_CAST_2 = 0x02,
    AIAO_CAST_3 = 0x03,

    AIAO_CAST_BUTT = 0x4,
} AIAO_CAST_ID_E;

typedef enum
{
    AIAO_CAST_STATUS_STOP = 0,
    AIAO_CAST_STATUS_START,
    AIAO_CAST_STATUS_CAST_BUTT,
} AIAO_CAST_STATUS_E;

/* void internal mmz */
typedef struct
{
    HI_U32  u32BufSize;
    HI_PHYS_ADDR_T  tBufPhyAddr;
    HI_VIRT_ADDR_T  tBufVirAddr;
    HI_VIRT_ADDR_T  tWptrAddr;
    HI_VIRT_ADDR_T  tRptrAddr;
} AIAO_CAST_MEM_ATTR_S;

typedef struct
{
    HI_U32  todo;
} AIAO_CAST_RBUF_ATTR_S;

typedef struct
{
    AIAO_CAST_MEM_ATTR_S extDmaMem;
    HI_U32 u32BufBitPerSample; /**<I/O, bit per sampling*//**<CNcomment:OUT. Bit per sample */
    HI_U32 u32BufChannels; /**<I/O, number of channels*//**<CNcomment:OUT. 输出声道数  */
    HI_U32 u32BufSampleRate; /**<I/O, sampling rate*//**<CNcomment:OUT. 输出采样频率 */
    HI_U32 u32BufDataFormat;          /**<I/O, 0, linear pcm, 1, iec61937 */
    HI_U32 u32BufLatencyThdMs;   /* 40 ~ 1000 ms */

    HI_BOOL bAddMute;
} AIAO_CAST_ATTR_S;

/* global function */
HI_S32                  HAL_CAST_Init(HI_VOID);
HI_VOID                 HAL_CAST_DeInit(HI_VOID);

/* port function */
HI_S32                  HAL_CAST_Create(AIAO_CAST_ID_E* penCast, AIAO_CAST_ATTR_S* pstAttr);
HI_VOID                 HAL_CAST_Destroy(AIAO_CAST_ID_E enCast);
HI_S32                  HAL_CAST_SetAttr(AIAO_CAST_ID_E enCast, AIAO_CAST_ATTR_S* pstAttr);
HI_S32                  HAL_CAST_GetAttr(AIAO_CAST_ID_E enCast, AIAO_CAST_ATTR_S* pstAttr);
HI_S32                  HAL_CAST_Start(AIAO_CAST_ID_E enCast);
HI_S32                  HAL_CAST_Stop(AIAO_CAST_ID_E enCast);
/* port buffer function */
HI_U32                  HAL_CAST_ReadData(AIAO_CAST_ID_E enCAST, HI_U32* pu32DataOffset, HI_U32 u32DestSize);
HI_U32                  HAL_CAST_ReleaseData(AIAO_CAST_ID_E enCAST, HI_U32 u32DestSize);
HI_U32                  HAL_CAST_QueryBufData(AIAO_CAST_ID_E enCast);
HI_U32                  HAL_CAST_QueryBufFree(AIAO_CAST_ID_E enCast);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // __HI_HAL_CAST_H__
