


#ifndef _SAMPLE_CC_XDS_H_
#define _SAMPLE_CC_XDS_H_

#include "hi_type.h"

#if defined __cplusplus || defined __cplusplus__
extern "C" {
#endif



/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

 /*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/

HI_VOID CC608_XDS_Init();

HI_S32 CC608_XDS_Decode(HI_U8 u8XDSClass, HI_U8 u8XDSType, HI_U8 *pu8Data, HI_U8 u8DataLen);


#if defined __cplusplus || defined __cplusplus__
}
#endif

#endif

