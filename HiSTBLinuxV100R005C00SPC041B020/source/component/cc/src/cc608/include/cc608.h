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


#ifndef _CC608_H_
#define _CC608_H_

#include "ccdisp_api.h"
#include "hi_type.h"
#include "hi_cc608_def.h"

#if defined __cplusplus || defined __cplusplus__
extern "C" {
#endif

/*******************************************************************************
* CC module Main screen structure
*******************************************************************************/
typedef struct tag_CC608_ELEMENT_S
{
    HI_U8 *     pu8Buffer;
    HI_U16      u16BufferSize;
    HI_U32      u32DataSize;
    HI_U16      u16Flags;
}CC608_ELEMENT_S;

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

 /*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/
HI_S32 CC608_Init(HI_VOID);
HI_S32 CC608_DeInit(HI_VOID);
HI_S32 CC608_Create(HI_U8 moduleid);

HI_S32 CC608_Destroy(HI_U8 moduleid);
HI_S32 CC608_Config(HI_U8 moduleid,HI_UNF_CC_608_CONFIGPARAM_S *pstCC608Config);
HI_S32 CC608_GetConfig(HI_U8 moduleid,HI_UNF_CC_608_CONFIGPARAM_S *pstCC608ConfigParam);
HI_U8  CC608_IsStart(HI_U8 moduleid);
HI_S32 CC608_Start(HI_U8 moduleid);
HI_S32 CC608_Stop(HI_U8 moduleid);
HI_S32 CC608_Reset(HI_U8 moduleid);

HI_S32 CC608_VBIParse(HI_U8 moduleid, HI_U8 *pu8CCData, HI_U8 u8CCCount);
HI_S32 CC608_Decode(HI_U8 module_id,HI_U16 *pu16CCData,  HI_U8 u8FieldNum);
HI_S32 CC608_ConvertCC608Char2Unicode(HI_U8 *pu8CCChars, HI_U32 u32Len, HI_U16 *pu16UniChars);
void  CC608_XDSReset(HI_U8 moduleid);

#if defined __cplusplus || defined __cplusplus__
}
#endif

#endif

/*****************************************************************************
*                    End Of File
*****************************************************************************/
