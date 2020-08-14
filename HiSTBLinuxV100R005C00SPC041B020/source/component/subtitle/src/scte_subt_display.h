#ifndef __SCTE_SUBT_DISPLAY_H__
#define __SCTE_SUBT_DISPLAY_H__

#include "hi_type.h"
#include "scte_subt_parse.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagSCTE_SUBT_DISPALY_PARAM_S
{
    SCTE_SUBT_DISP_E enDISP;
    HI_U32           u32x;
    HI_U32           u32y;
    HI_U32           u32w;
    HI_U32           u32h;
    HI_U32           u32BitWidth;
    HI_U32           u32PTS;
    HI_U32           u32Duration;

    HI_U32           u32DataLen;
    HI_U8*           pu8SubtData;
    HI_U32           u32DisplayWidth;
    HI_U32           u32DisplayHeight;
} SCTE_SUBT_DISPALY_PARAM_S;

typedef HI_S32 (*SCTE_SUBT_DISPLAY_CALLBACK_FN)(HI_U32 u32UserData, HI_VOID *pstDisplayDate);

HI_S32 SCTE_SUBT_Display_Init(HI_VOID);

HI_S32 SCTE_SUBT_Display_DeInit(HI_VOID);

HI_S32 SCTE_SUBT_Display_Create(SCTE_SUBT_DISPLAY_CALLBACK_FN pfnCallback, HI_U32 u32UserData, HI_VOID** pphDisplay);

HI_S32 SCTE_SUBT_Display_Destroy(HI_VOID* hDisplay);

HI_S32 SCTE_SUBT_Display_DisplaySubt(HI_VOID* hDisplay, SCTE_SUBT_OUTPUT_S *pstOutData);

#ifdef __cplusplus
}
#endif
#endif
