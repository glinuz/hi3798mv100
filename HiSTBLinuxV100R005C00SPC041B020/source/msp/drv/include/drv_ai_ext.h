#ifndef __DRV_AI_EXT_H__
#define __DRV_AI_EXT_H__

#include "hi_type.h"
#include "hi_drv_ai.h"
#include "hi_drv_ao.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef HI_S32 (*FN_AI_DrvResume)(PM_BASEDEV_S*);
typedef HI_S32 (*FN_AI_DrvSuspend)(PM_BASEDEV_S*, pm_message_t);

typedef struct
{
    FN_AI_DrvResume  pfnAI_DrvResume;
    FN_AI_DrvSuspend pfnAI_DrvSuspend;
} AI_EXPORT_FUNC_S;

HI_S32  AI_DRV_ModInit(HI_VOID);
HI_VOID AI_DRV_ModExit(HI_VOID);

HI_S32 HI_DRV_AI_Init(HI_VOID);
HI_VOID HI_DRV_AI_DeInit(HI_VOID);
HI_S32 HI_DRV_AI_Open(struct file* file);
HI_S32 HI_DRV_AI_Release(struct file* file);
HI_S32 HI_DRV_AI_GetDefaultAttr(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S* pstAttr);
HI_S32 HI_DRV_AI_Create(AI_DRV_Create_Param_S* pstAiPara, struct file* file);
HI_S32 HI_DRV_AI_Destroy(HI_HANDLE hAi);
HI_S32 HI_DRV_AI_SetEnable(HI_HANDLE hAi, HI_BOOL bEnable);
HI_S32 HI_DRV_AI_GetEnable(HI_HANDLE hAi, HI_BOOL* pbEnable);
HI_S32 HI_DRV_AI_GetAttr(HI_HANDLE hAi, HI_UNF_AI_ATTR_S* pstAttr);
HI_S32 HI_DRV_AI_GetAttr(HI_HANDLE hAi, HI_UNF_AI_ATTR_S* pstAttr);
HI_S32 HI_DRV_AI_AcquireFrame(HI_HANDLE hAi, AO_FRAMEINFO_S* pstFrame);
HI_S32 HI_DRV_AI_ReleaseFrame(HI_HANDLE hAi);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif//__DRV_AI_EXT_H__
