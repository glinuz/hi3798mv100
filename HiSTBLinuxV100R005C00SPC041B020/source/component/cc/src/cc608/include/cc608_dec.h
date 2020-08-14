


#ifndef _CC608_DEC_H_
#define _CC608_DEC_H_

#include "hi_type.h"

#if defined __cplusplus || defined __cplusplus__
extern "C" {
#endif

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

 /*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/

void CC608_DEC_PAC(HI_U8 moduleID,HI_U8 b1,HI_U8 b2,HI_U8 u8Flag);
void CC608_DEC_Color(HI_U8 moduleID, HI_U8 b1, HI_U8 b2);
void CC608_DEC_MidRow(HI_U8 moduleID,HI_U8 b1, HI_U8 b2);
void CC608_DEC_Command(HI_U8 moduleID, HI_U8 u8IsCaption, HI_U8 b1, HI_U8 b2);
HI_U16 CC608_DEC_ExtCharSet(HI_U8 moduleID, HI_U16 u16LastChar,HI_U8 b1, HI_U8 b2);
HI_S32 CC608_DEC_Decode(HI_U8 module_id,HI_U16 *pu16CCData,  HI_U8 u8FieldNum);
HI_S32 CC608_DEC_CheckXDS(HI_U8 moduleID, HI_U16 *pu16CCData,  HI_U8 u8FieldNum);
HI_S32 CC608_DEC_Convert2Unicode(HI_U8 *pu8CCChars, HI_U32 u32Len, HI_U16 *pu16UniChars);

#if defined __cplusplus || defined __cplusplus__
}
#endif

#endif //#ifndef _CC608DEC_H

/*****************************************************************************
*                    End Of File
*****************************************************************************/







