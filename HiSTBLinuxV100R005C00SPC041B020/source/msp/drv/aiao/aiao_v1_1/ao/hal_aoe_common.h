/******************************************************************************

Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hal_aoe_common.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/09/22
Last Modified :
Description   : hal_aoe
Function List :
History       :
* main\1    2013-03-11   zgjie     init.
******************************************************************************/
#ifndef __HI_HAL_AOE_COMMON_H__
#define __HI_HAL_AOE_COMMON_H__

#include "hi_type.h"
#include "hi_audsp_aoe.h"

/* AOE AIP Definition */
typedef enum
{
    AOE_AIP0  = 0x00,
    AOE_AIP1  = 0x01,
    AOE_AIP2  = 0x02,
    AOE_AIP3  = 0x03,
    AOE_AIP4  = 0x04,
    AOE_AIP5  = 0x05,
    AOE_AIP6  = 0x06,
    AOE_AIP7  = 0x07,

    AOE_AIP_BUTT = AOE_MAX_AIP_NUM,
} AOE_AIP_ID_E;

/* AOE AOP Definition */
typedef enum
{
    AOE_AOP0 = 0x00,
    AOE_AOP1 = 0x01,
    AOE_AOP2 = 0x02,
    AOE_AOP3 = 0x03,
    AOE_AOP4 = 0x04,
    AOE_AOP5 = 0x05,
    AOE_AOP6 = 0x06,
    AOE_AOP7 = 0x07,

    AOE_AOP_BUTT = AOE_MAX_AOP_NUM,
} AOE_AOP_ID_E;

/* AOE ENGINE Definition */
typedef enum
{
    AOE_ENGINE0 = 0x00,
    AOE_ENGINE1 = 0x01,
    AOE_ENGINE2 = 0x02,
    AOE_ENGINE3 = 0x03,
    AOE_ENGINE4 = 0x04,
    AOE_ENGINE5 = 0x05,

    AOE_ENGINE_BUTT = AOE_MAX_ENGINE_NUM,
} AOE_ENGINE_ID_E;

/**Defines the  status of an AIP.*/
typedef enum
{
    AOE_AIP_STATUS_STOP = 0, /**<Stop*//**<CNcomment: 停止 */
    AOE_AIP_STATUS_START, /**<Start*//**<CNcomment: 运行 */
    AOE_AIP_STATUS_PAUSE,

    AIP_STATUS_BUTT
} AOE_AIP_STATUS_E;

/**Defines the  status of an AOP.*/
typedef enum
{
    AOE_AOP_STATUS_STOP = 0, /**<Stop*//**<CNcomment: 停止 */
    AOE_AOP_STATUS_START, /**<Start*//**<CNcomment: 运行 */

    AOE_AOP_STATUS_BUTT
} AOE_AOP_STATUS_E;

/**Defines the  status of an Engine.*/
typedef enum
{
    AOE_ENGINE_STATUS_STOP = 0, /**<Stop*//**<CNcomment: 停止 */
    AOE_ENGINE_STATUS_START, /**<Start*//**<CNcomment: 运行 */

    AOE_ENGINE_STATUS_BUTT
} AOE_ENGINE_STATUS_E;

typedef struct
{
    HI_PHYS_ADDR_T  tBufPhyAddr;  // hw aoe
    HI_PHYS_ADDR_T  tBufPhyWptr;  // hw aoe
    HI_PHYS_ADDR_T  tBufPhyRptr;  // hw aoe
    HI_VIRT_ADDR_T  tBufVirAddr;  // sw aoe
    HI_VIRT_ADDR_T  tBufVirWptr;  // sw aoe
    HI_VIRT_ADDR_T  tBufVirRptr;  // sw aoe
    HI_U32  u32BufWptrRptrFlag;  /* 0: u32BufWptr & u32BufRptr located at AIP Reg, else: */
    HI_U32  u32BufSize;
} AOE_RBUF_ATTR_S;

typedef struct
{
    AOE_RBUF_ATTR_S stRbfAttr;
    HI_U32 u32BufBitPerSample; /**<I/O, bit per sampling*//**<CNcomment:OUT. Bit per sample */
    HI_U32 u32BufChannels; /**<I/O, number of channels*//**<CNcomment:OUT. 输出声道数  */
    HI_U32 u32BufSampleRate; /**<I/O, sampling rate*//**<CNcomment:OUT. 输出采样频率 */
    HI_U32 u32BufDataFormat;          /**<I/O, 0, linear pcm, 1, iec61937 */
    HI_U32 u32BufLatencyThdMs;   /* 40 ~ 1000 ms */
    HI_U32  u32FadeinMs;
    HI_U32  u32FadeoutMs;
    HI_BOOL bFadeEnable;
    AOE_AIP_TYPE_PCM_E eAipType;
    HI_BOOL bMixPriority;  /* TRUE: high priority */

} AOE_AIP_INBUF_ATTR_S;

typedef struct
{
    HI_U32 u32FifoBitPerSample; /**<I/O, bit per sampling*/ /**<CNcomment:OUT. Bit per sample */
    HI_U32 u32FifoChannels; /**<I/O, number of channels*/ /**<CNcomment:OUT. 输出声道数  */
    HI_U32 u32FifoSampleRate; /**<I/O, sampling rate*/ /**<CNcomment:OUT. 输出采样频率 */
    HI_U32 u32FifoDataFormat;               /**<I/O, 0, linear pcm, 1, iec61937 */
    HI_U32 u32FiFoLatencyThdMs; /* 10 ~ 40 ms */
} AOE_AIP_OUTFIFO_ATTR_S;

typedef struct
{
    AOE_AIP_INBUF_ATTR_S   stBufInAttr;
    AOE_AIP_OUTFIFO_ATTR_S stFifoOutAttr;
    HI_BOOL bFifoBypass;
} AOE_AIP_CHN_ATTR_S;

typedef struct
{
    AOE_RBUF_ATTR_S stRbfAttr;
    HI_U32  u32BufBitPerSample; /**<I/O, bit per sampling*//**<CNcomment:OUT. Bit per sample */
    HI_U32  u32BufChannels; /**<I/O, number of channels*//**<CNcomment:OUT. 输出声道数  */
    HI_U32  u32BufSampleRate; /**<I/O, sampling rate*//**<CNcomment:OUT. 输出采样频率 */
    HI_U32  u32BufDataFormat;          /**<I/O, 0, linear pcm, 1, iec61937 */
    HI_U32  u32BufLatencyThdMs;        /* 10 ~ 40 ms */
} AOE_AOP_OUTBUF_ATTR_S;


typedef struct
{
    AOE_AOP_OUTBUF_ATTR_S stRbfOutAttr;
    HI_BOOL bRbfHwPriority; /* TRUE: high priority */

    HI_BOOL bIsCast;  /* HI_TRUE:aop is cast ; HI_FALSE:aop is normal aop */
    HI_BOOL bAddMute; /* Just for cast aop */
} AOE_AOP_CHN_ATTR_S;

#ifdef __DPT__
typedef struct
{
    HI_PHYS_ADDR_T  tBufPhyAddr;
    HI_VIRT_ADDR_T  tBufVirAddr;
    HI_U32      u32BufSize;
    HI_U32      u32PeriodSize;        //data size per frame
    HI_U32      u32PeriodNum;          /* 2/4/8 */
} AOE_ENGINE_AEFBUF_ATTR_S;
#endif

typedef struct
{
    HI_U32 u32BitPerSample; /**<I/O, bit per sampling*/ /**<CNcomment:OUT. Bit per sample */
    HI_U32 u32Channels; /**<I/O, number of channels*/ /**<CNcomment:OUT. 输出声道数  */
    HI_U32 u32SampleRate; /**<I/O, sampling rate*/ /**<CNcomment:OUT. 输出采样频率 */
    HI_U32 u32DataFormat;               /**<I/O, 0, linear pcm, 1, iec61937 */
#ifdef HI_SND_USER_AEF_SUPPORT
    AOE_ENGINE_AEFBUF_ATTR_S stAefInBufAttr;
    AOE_ENGINE_AEFBUF_ATTR_S stAefOutBufAttr;
#endif
} AOE_ENGINE_CHN_ATTR_S;

#endif  // __HI_HAL_AOE_COMMON_H__
