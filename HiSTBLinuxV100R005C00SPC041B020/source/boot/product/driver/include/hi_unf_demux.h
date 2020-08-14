/******************************************************************************
Copyright (C), 2004-2008, HiSilicon Technologies Co., Ltd.
******************************************************************************
File Name     : hi_unf_demux.h
Version       : Initial
Author        : HiSilicon multimedia software group
Created       : 2008-06-10
Last Modified :
Description   : HiSilicon DEMUX API declaration
******************************************************************************/

#ifndef __HI_UNF_DEMUX_H__
#define __HI_UNF_DEMUX_H__

#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    HI_UNF_DMX_PORT_IF_0 = 0x0,
    HI_UNF_DMX_PORT_IF_1,
    HI_UNF_DMX_PORT_IF_2,
    HI_UNF_DMX_PORT_IF_3,
    HI_UNF_DMX_PORT_IF_4,
    HI_UNF_DMX_PORT_IF_5,
    HI_UNF_DMX_PORT_IF_6,
    HI_UNF_DMX_PORT_IF_7,

    HI_UNF_DMX_PORT_TSI_0 = 0x20,
    HI_UNF_DMX_PORT_TSI_1,
    HI_UNF_DMX_PORT_TSI_2,
    HI_UNF_DMX_PORT_TSI_3,
    HI_UNF_DMX_PORT_TSI_4,
    HI_UNF_DMX_PORT_TSI_5,
    HI_UNF_DMX_PORT_TSI_6,
    HI_UNF_DMX_PORT_TSI_7,

    HI_UNF_DMX_PORT_RAM_0 = 0x80,
    HI_UNF_DMX_PORT_RAM_1,
    HI_UNF_DMX_PORT_RAM_2,
    HI_UNF_DMX_PORT_RAM_3,
    HI_UNF_DMX_PORT_RAM_4,
    HI_UNF_DMX_PORT_RAM_5,
    HI_UNF_DMX_PORT_RAM_6,
    HI_UNF_DMX_PORT_RAM_7,

    HI_UNF_DMX_PORT_BUTT
} HI_UNF_DMX_PORT_E;

typedef enum
{
    HI_UNF_DMX_CHAN_TYPE_SEC = 0,
    HI_UNF_DMX_CHAN_TYPE_PES,
    HI_UNF_DMX_CHAN_TYPE_AUD,
    HI_UNF_DMX_CHAN_TYPE_VID,
    HI_UNF_DMX_CHAN_TYPE_POST,
    HI_UNF_DMX_CHAN_TYPE_ECM_EMM,

    HI_UNF_DMX_CHAN_TYPE_BUTT
} HI_UNF_DMX_CHAN_TYPE_E;

typedef enum
{
    HI_UNF_DMX_CHAN_CRC_MODE_FORBID                 = 0,
    HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD      = 1,
    HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_SEND         = 2,
    HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD  = 3,
    HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_SEND     = 4,

    HI_UNF_DMX_CHAN_CRC_MODE_BUTT
} HI_UNF_DMX_CHAN_CRC_MODE_E;

typedef enum
{
    HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY        = 0x1,
    HI_UNF_DMX_CHAN_OUTPUT_MODE_REC         = 0x2,
    HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY_REC    = 0x3,

    HI_UNF_DMX_CHAN_OUTPUT_MODE_BUTT
} HI_UNF_DMX_CHAN_OUTPUT_MODE_E;

typedef enum hiUNF_DMX_PORT_MODE_E
{
    HI_UNF_DMX_PORT_MODE_EXTERNAL,   
    HI_UNF_DMX_PORT_MODE_INTERNAL,   
    HI_UNF_DMX_PORT_MODE_RAM,        

    HI_UNF_DMX_PORT_MODE_BUTT
} HI_UNF_DMX_PORT_MODE_E;

typedef enum hiUNF_DMX_PORT_TYPE_E
{
    HI_UNF_DMX_PORT_TYPE_PARALLEL_BURST,         
    HI_UNF_DMX_PORT_TYPE_PARALLEL_VALID,          
    HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188,    
    HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_204,    
    HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188_204, 
    HI_UNF_DMX_PORT_TYPE_SERIAL,                 
    HI_UNF_DMX_PORT_TYPE_USER_DEFINED,           
    HI_UNF_DMX_PORT_TYPE_SERIAL2BIT,             
    HI_UNF_DMX_PORT_TYPE_SERIAL_NOSYNC,           
    HI_UNF_DMX_PORT_TYPE_SERIAL2BIT_NOSYNC,     
    HI_UNF_DMX_PORT_TYPE_AUTO,                    

    HI_UNF_DMX_PORT_TYPE_BUTT
} HI_UNF_DMX_PORT_TYPE_E;

typedef struct hiUNF_DMX_PORT_ATTR_S
{
    HI_UNF_DMX_PORT_MODE_E enPortMod;  
    HI_UNF_DMX_PORT_TYPE_E enPortType; 
    HI_U32 u32SyncLostTh;              
    HI_U32 u32SyncLockTh;              
    HI_U32 u32TunerInClk;             
    HI_U32 u32SerialBitSelector;       
    HI_U32 u32TunerErrMod;           
    HI_U32 u32UserDefLen1;          
    HI_U32 u32UserDefLen2;           

} HI_UNF_DMX_PORT_ATTR_S;

typedef struct
{
    HI_U32                          u32BufSize;
    HI_UNF_DMX_CHAN_TYPE_E          enChannelType;
    HI_UNF_DMX_CHAN_CRC_MODE_E      enCRCMode;
    HI_UNF_DMX_CHAN_OUTPUT_MODE_E   enOutputMode;
} HI_UNF_DMX_CHAN_ATTR_S;

#define DMX_FILTER_MAX_DEPTH            16

typedef struct
{
    HI_U32  u32FilterDepth;
    HI_U8   au8Match[DMX_FILTER_MAX_DEPTH];
    HI_U8   au8Mask[DMX_FILTER_MAX_DEPTH];
    HI_U8   au8Negate[DMX_FILTER_MAX_DEPTH];
} HI_UNF_DMX_FILTER_ATTR_S;

typedef enum
{
    HI_UNF_DMX_DATA_TYPE_WHOLE = 0,
    HI_UNF_DMX_DATA_TYPE_HEAD,
    HI_UNF_DMX_DATA_TYPE_BODY,
    HI_UNF_DMX_DATA_TYPE_TAIL,

    HI_UNF_DMX_DATA_TYPE_BUTT
} HI_UNF_DMX_DATA_TYPE_E;

typedef struct
{
    HI_U8                  *pu8Data;
    HI_U32                  u32Size;
    HI_UNF_DMX_DATA_TYPE_E  enDataType;
} HI_UNF_DMX_DATA_S;

HI_S32 HI_UNF_DMX_Init(HI_VOID);
HI_S32 HI_UNF_DMX_DeInit(HI_VOID);

HI_S32 HI_UNF_DMX_AttachTSPort(HI_U32 u32DmxId, HI_UNF_DMX_PORT_E enPortId);
HI_S32 HI_UNF_DMX_DetachTSPort(HI_U32 u32DmxId);
HI_S32 HI_UNF_DMX_GetTSPortAttr(HI_UNF_DMX_PORT_E enPortId, HI_UNF_DMX_PORT_ATTR_S *pstAttr);
HI_S32 HI_UNF_DMX_SetTSPortAttr(HI_UNF_DMX_PORT_E enPortId, const HI_UNF_DMX_PORT_ATTR_S *pstAttr);

HI_S32 HI_UNF_DMX_CreateChannel(HI_U32 u32DmxId, const HI_UNF_DMX_CHAN_ATTR_S *pstChAttr, HI_HANDLE *phChannel);
HI_S32 HI_UNF_DMX_DestroyChannel(HI_HANDLE hChannel);
HI_S32 HI_UNF_DMX_SetChannelPID(HI_HANDLE hChannel, HI_U32 u32Pid);
HI_S32 HI_UNF_DMX_OpenChannel(HI_HANDLE hChannel);
HI_S32 HI_UNF_DMX_CloseChannel(HI_HANDLE hChannel);

HI_S32 HI_UNF_DMX_CreateFilter(HI_U32 u32DmxId, const HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr, HI_HANDLE *phFilter);
HI_S32 HI_UNF_DMX_DestroyFilter(HI_HANDLE hFilter);
HI_S32 HI_UNF_DMX_AttachFilter(HI_HANDLE hFilter, HI_HANDLE hChannel);
HI_S32 HI_UNF_DMX_DetachFilter(HI_HANDLE hFilter, HI_HANDLE hChannel);

HI_S32 HI_UNF_DMX_AcquireBuf(
        HI_HANDLE           hChannel,
        HI_U32              u32AcquireNum,
        HI_U32             *pu32AcquiredNum,
        HI_UNF_DMX_DATA_S  *pstBuf,
        HI_U32              u32TimeOutMs
    );

HI_S32 HI_UNF_DMX_ReleaseBuf(HI_HANDLE hChannel, HI_U32 u32ReleaseNum, HI_UNF_DMX_DATA_S *pstBuf);

#ifdef __cplusplus
}
#endif

#endif  // __HI_UNF_DEMUX_H__

