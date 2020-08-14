#ifndef __SAMPLE_SUBTITLE_OUT_H__
#define __SAMPLE_SUBTITLE_OUT_H__

#ifdef __cplusplus
extern "C"{
#endif


HI_S32 Subt_Output_OnClear(HI_VOID * pUserData, HI_VOID *pArg);
HI_S32 Subt_Output_OnDraw(HI_VOID * pUserData, const HI_UNF_SO_SUBTITLE_INFO_S *pstInfo, HI_VOID *pArg);

HI_S32 Subt_Output_Init(HI_HANDLE* phOut);

HI_S32 Subt_Output_DeInit(HI_HANDLE hOut);

#ifdef __cplusplus
}
#endif

#endif

