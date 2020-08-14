/***********************************************************************************
*             Copyright 2006 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: common_demux.h
* Description: ADP_DEMUX module external output 
*
* History:
* Version   Date             Author     DefectNum    Description
* main\1    2006-12-04   g47171     NULL         Create this file.
***********************************************************************************/

#ifndef  _COMMON_DEMUX_H
#define  _COMMON_DEMUX_H

#include "hi_type.h"
#include "hi_unf_demux.h"
#include <stdio.h>
#include <pthread.h>

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#define MAX_SECTION_LEN 4096
#define MAX_SECTION_NUM 256
#define INVALID_PORT_DMX_ID 0xFFFF
#define MAX_IP_THREAD_CNT 16
#define MAX_FILE_THREAD_CNT 16

typedef HI_S32 (*T_CommSectionCallback)(const HI_U8 *pu8Buffer, HI_S32 s32BufferLength, HI_U8 *pSectionStruct);

/********************************************************************
  mark:SECTIONSVR_DATA_FILTER_S
  type:data struct
  purpose:present SECTION data filter contents, application can use it for special filtrate request
  definition:
 **********************************************************************/
typedef struct  hiDMX_DATA_FILTER_S
{
    HI_U32 u32TSPID;                /* TSPID */
    HI_U32 u32BufSize;          /* hareware BUFFER request */

    HI_U8 u8SectionType;       /* section type, 0-section 1-PES */
    HI_U8 u8Crcflag;               /* channel CRC open flag, 0-not open; 1-open */

    HI_U8  u8Match[DMX_FILTER_MAX_DEPTH];
    HI_U8  u8Mask[DMX_FILTER_MAX_DEPTH];
    HI_U8  u8Negate[DMX_FILTER_MAX_DEPTH];
    HI_U16 u16FilterDepth;          /* filtrate depth, 0xff-data use all the user set, otherwise, use DVB algorithm(fixme)*/

    HI_U32 u32TimeOut;       /* timeout, in second. 0-permanent wait */

    T_CommSectionCallback funSectionFunCallback;   /* section end callback */
    HI_U8 *pSectionStruct;
} DMX_DATA_FILTER_S;

HI_S32 DMX_SectionStartDataFilter(HI_U32 u32DmxId, DMX_DATA_FILTER_S * pstDataFilter);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#endif /* _SECTIONSVR_PUB_H*/
