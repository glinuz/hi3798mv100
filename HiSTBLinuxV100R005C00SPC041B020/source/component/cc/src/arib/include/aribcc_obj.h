#ifndef _ARIBCC_OBJ_H
#define _ARIBCC_OBJ_H

#include "hi_type.h"
#include "hi_unf_cc.h"
#include "aribcc_def.h"

#if defined __cplusplus || defined __cplusplus__
extern "C" {
#endif

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/
typedef struct tagARIBCC_OBJECT_S
{
    HI_CHAR    acISO639LanguageCode[4];
    HI_U32     u32PesBufSize;
} ARIBCC_OBJECT_S;

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

 /*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/
HI_S32 ARIBCC_OBJ_Create(HI_UNF_CC_ARIB_CONFIGPARAM_S *pstAribConfig);

ARIBCC_OBJECT_S *ARIBCC_OBJ_Get(void);

#if defined __cplusplus || defined __cplusplus__
}
#endif

#endif //#ifndef _HEADERNAME_H