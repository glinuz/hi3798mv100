#ifndef  TELETEXT_MEMORY_H__
#define  TELETEXT_MEMORY_H__

#include "teletext_def.h"
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

HI_S32    TTX_Mem_Init(HI_U8 *pu8MemAddr, HI_U32 u32MemSize);

HI_S32    TTX_Mem_DeInit(HI_VOID);

HI_VOID * TTX_Mem_Malloc(HI_U32 u32MemSize);

HI_VOID   TTX_Mem_Free(const HI_VOID *pvMemStart);

HI_VOID   TTX_Mem_Memset(HI_VOID *pvMemStart, HI_S32 s32InitNum, HI_U32 u32MemSize);

HI_S32    TTX_Mem_GetSpareSize(HI_U32 * u32SpareSize);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif
