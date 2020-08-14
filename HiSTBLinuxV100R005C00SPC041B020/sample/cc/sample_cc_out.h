#ifndef __SAMPLE_CC_OUT_H__
#define __SAMPLE_CC_OUT_H__

#include "hi_unf_cc.h"
#ifdef __cplusplus
extern "C"{
#endif

HI_S32 CC_Output_Init(HI_HANDLE* phOut);

HI_S32 CC_Output_DeInit(HI_HANDLE hOut);

HI_S32 CC_Output_OnDraw(HI_VOID* pUserData, HI_UNF_CC_DISPLAY_PARAM_S *pstDisplayParam);

HI_S32 CC_Output_GetTextSize(HI_VOID* pUserdata, HI_U16 *u8Str,HI_S32 StrNum, HI_S32 *width, HI_S32 *heigth);

HI_S32 CC_Output_Blit(HI_VOID* pUserPrivatData, HI_UNF_CC_RECT_S *SrcRect, HI_UNF_CC_RECT_S *DestRect);

HI_S32 CC_Output_VBIOutput(HI_VOID* pUserData, HI_UNF_CC_VBI_DADA_S *pstVBIDataField1, HI_UNF_CC_VBI_DADA_S *pstVBIDataField2);

#ifdef __cplusplus
}
#endif

#endif

