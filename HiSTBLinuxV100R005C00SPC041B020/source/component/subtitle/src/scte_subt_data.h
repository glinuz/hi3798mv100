#ifndef __SCTE_SUBT_DATA_H__
#define __SCTE_SUBT_DATA_H__

#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif

HI_S32 SCTE_SUBT_Data_Init(HI_VOID);

HI_S32 SCTE_SUBT_Data_DeInit(HI_VOID);

HI_S32 SCTE_SUBT_Data_Create(HI_VOID* hParse, HI_VOID** pphData);

HI_S32 SCTE_SUBT_Data_Destroy(HI_VOID* hData);

HI_S32 SCTE_SUBT_Data_Reset(HI_VOID* hData);

HI_S32 SCTE_SUBT_Data_Inject(HI_VOID* hData, const HI_U8 *pu8Data, HI_U32 u32DataSize);

#ifdef __cplusplus
}
#endif

#endif
