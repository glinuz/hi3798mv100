#ifndef __DRV_PDM_EXT_H__
#define __DRV_PDM_EXT_H__

#include "hi_type.h"
#include "hi_drv_pdm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef HI_S32 (*FN_PDM_GetDispParam)(HI_UNF_DISP_E enDisp, HI_DISP_PARAM_S *pstDispParam);
typedef HI_S32 (*FN_PDM_GetMceParam)(HI_MCE_PARAM_S *pMceParam);
typedef HI_S32 (*FN_PDM_GetMceData)(HI_U32 u32Size, HI_U8 **pAddr);
typedef HI_S32 (*FN_PDM_ReleaseReserveMem)(const HI_CHAR *BufName);
typedef HI_S32 (*FN_PDM_GetData)(const HI_CHAR *BufName, HI_U8 **pDataAddr, HI_U32 *pu32DataLen);
typedef HI_S32 (*FN_PDM_GetSoundParam)(HI_UNF_SND_E enSound, HI_UNF_PDM_SOUND_PARAM_S * pstSoundParam);

typedef struct tagPDM_EXPORT_FUNC_S
{
    FN_PDM_GetDispParam             pfnPDM_GetDispParam;
    FN_PDM_GetMceParam              pfnPDM_GetMceParam;
    FN_PDM_GetMceData               pfnPDM_GetMceData;
    FN_PDM_ReleaseReserveMem        pfnPDM_ReleaseReserveMem;
	FN_PDM_GetData                  pfnPDM_GetData; 
	FN_PDM_GetSoundParam			pfnGetSoundParam;
}PDM_EXPORT_FUNC_S;

HI_S32 PDM_DRV_ModInit(HI_VOID);
HI_VOID PDM_DRV_ModExit(HI_VOID);


#ifdef __cplusplus
}
#endif

#endif

