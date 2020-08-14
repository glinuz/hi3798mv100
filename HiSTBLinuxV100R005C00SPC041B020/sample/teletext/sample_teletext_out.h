#ifndef  __SAMPLE_TELETEXT_OUT_H__
#define __SAMPLE_TELETEXT_OUT_H__

#include  "hi_unf_ttx.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ANDROID
HI_S32 FontInit(HI_VOID);

HI_VOID FontDeInit(HI_VOID);

HI_S32 OsdInit(HI_VOID);

HI_VOID OsdDeInit(HI_VOID);
#else
HI_S32	Higo_Teletext_Init(HI_VOID);

HI_VOID Higo_Teletext_DeInit(HI_VOID);
#endif

HI_S32	SetAVplayHandle(HI_HANDLE hAVPlayer);

HI_S32	TTX_SampleCallBack(HI_HANDLE hTTX, HI_UNF_TTX_CB_E enCB, HI_VOID *pvCBParam);

#ifdef __cplusplus
}
#endif
#endif
