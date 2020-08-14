/******************************************************************************

Copyright (C), 2012-2014, HiSilicon Technologies Co., Ltd.
******************************************************************************
File Name     :
Version       : Initial draft
Author        :
Created Date   :
Last Modified by:
Description   :
Function List :
Change History:
******************************************************************************/

#ifndef __ARIBCC_COMMON_H__
#define __ARIBCC_COMMON_H__


//#include <limits.h>
#include <sys/time.h>

#include "aribcc_def.h"

/*Size of message queue buffer*/
#define ARIBCC_MSGBUFF_SIZE           512

#define SCR_SPEED_MIN           0
#define SCR_SPEED_MAX           0xffff
#define RCS_INDEX_MIN           0
#define RCS_INDEX_MAX           127
#define ORN_INDEX_MIN           0
#define ORN_INDEX_MAX           127

typedef enum    tagARIBCC_MsgType_E
{
    MSGTYPE_CC_DEC_START = 1,
    MSGTYPE_CC_DECODE,
    MSGTYPE_CC_DEC_STOP,
    MSGTYPE_CC_DEC_END,
    MSGTYPE_CC_DEC_SETLANG,
    MSGTYPE_CC_DEC_MUTE,
    MSGTYPE_CC_DISP_START,
    MSGTYPE_CC_DISP_START_COMP,
    MSGTYPE_CC_DISP_PARSE,
    MSGTYPE_CC_DISP_PARSE_COMP,
    MSGTYPE_CC_DISP_STOP,
    MSGTYPE_CC_DISP_STOP_COMP,
    MSGTYPE_CC_DISP_END,
    MSGTYPE_CC_DISP_CLEAR,
    MSGTYPE_CC_DISP_TIMEOUT
} ARIBCC_MsgType_E;


//*********************************************************************************************************************


HI_S32 ARIBCC_Com_Init (void);

HI_S32 ARIBCC_Com_DeInit (void);

HI_S32 ARIBCC_Com_InitPESPtr (ARIBCC_CaptionType_E enCaptionType, HI_U32 u32Size);
HI_S32 ARIBCC_Com_ClearPESPtr (ARIBCC_CaptionType_E enCaptionType);
HI_S32 ARIBCC_Com_AllocPESPtr (ARIBCC_CaptionType_E enCaptionType, HI_U8 **ppu8CcPesPtr, HI_U32 u32CcPesSize);
HI_S32 ARIBCC_Com_FreePESPtr (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8CcPesPtr);
HI_S32 ARIBCC_Com_DeInitPESPtr (ARIBCC_CaptionType_E enCaptionType);
HI_S32 ARIBCC_Com_GetPESSize (ARIBCC_CaptionType_E enCaptionType, HI_U32 *pu32Size);



HI_S32 ARIBCC_Com_MessageInit(HI_VOID);
HI_S32 ARIBCC_Com_MessageDeInit(HI_VOID);
HI_S32 ARIBCC_Com_MessageSend(ARIBCC_CaptionType_E enCaptionType, HI_S32 s32Type, HI_U8 *pu8Data, HI_S32 s32Size );
HI_S32 ARIBCC_Com_MessageReceive(ARIBCC_CaptionType_E enCaptionType, HI_S32 *ps32Type, HI_U8 *pu8Data, HI_S32 *ps32Size );


HI_S32 ARIBCC_Com_ClearTimer (ARIBCC_CaptionType_E enCaptionType);
HI_S32 ARIBCC_Com_RegistTimer (ARIBCC_CaptionType_E enCaptionType, HI_S32 s32MsgType, HI_VOID *pvManagement,
                               HI_VOID *pvUnitGroup, struct timeval *pstTimeValue);
HI_S32 ARIBCC_Com_TimerCheck (ARIBCC_CaptionType_E enCaptionType);

HI_U32 ARIBCC_GetBits (HI_U8 *buf, HI_S32 byte_offset, HI_S32 startbit, HI_S32 bitlen);

#define GET_BIT(p,b,n)  ARIBCC_GetBits(p,0,b,n)

#endif
