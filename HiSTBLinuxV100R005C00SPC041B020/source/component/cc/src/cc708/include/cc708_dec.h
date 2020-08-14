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
#ifndef __CC708_DECODE_H__
#define __CC708_DECODE_H__

#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/


/*****************************************************************************
*                    Structure Definitions
*****************************************************************************/


 /*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/
HI_S32 CC708_DEC_Start(HI_U8 moduleID);

HI_S32 CC708_DEC_Stop(HI_U8 moduleID);

HI_S32 CC708_DEC_Reset(HI_U8 moduleID);

HI_S32 CC708_DEC_UserDataParse(HI_U8 *pu8UserData, HI_U32 u32DataLen, HI_BOOL bTopFieldFirst);

HI_S32 CC708_DEC_ProcessServicefifo(HI_VOID);


#ifdef __cplusplus
}
#endif

#endif

/*****************************************************************************
*                    End Of File
*****************************************************************************/