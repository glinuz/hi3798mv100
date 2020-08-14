#ifndef __DRV_PDM_H__
#define __DRV_PDM_H__

#include <linux/semaphore.h>
#include "hi_type.h"
#include "drv_pdm_ext.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PDM_MAX_BUF_NUM     16

typedef struct tagPDM_BUF_INFO_S
{
	HI_CHAR             as8BufName[16];
	HI_BOOL 			bRelease;
	HI_U32				u32Lenth;
	HI_U8				*pPhyAddr;
	HI_U8				*pVirAddr;
}PDM_BUF_INFO_S;

typedef struct tagPDM_GLOBAL_S
{
    HI_DISP_PARAM_S     stDispParam;
    HI_MCE_PARAM_S      stMceParam;
    PDM_BUF_INFO_S      stBufInfo[PDM_MAX_BUF_NUM];
    HI_U32              u32BufNum;
    struct semaphore    PdmMutex;
}PDM_GLOBAL_S;

HI_S32 DRV_PDM_GetDispParam(HI_UNF_DISP_E enDisp, HI_DISP_PARAM_S *pstDispParam);
HI_S32 DRV_PDM_GetMceParam(HI_MCE_PARAM_S *pMceParam);
HI_S32 DRV_PDM_GetMceData(HI_U32 u32Size, HI_U8 **pAddr);
HI_S32 DRV_PDM_ReleaseReserveMem(const HI_CHAR *BufName);
HI_S32 DRV_PDM_GetData(const HI_CHAR *BufName, HI_U8 **pDataAddr, HI_U32 *pu32DataLen);
HI_S32 DRV_PDM_GetSoundParam(HI_UNF_SND_E enSound, HI_UNF_PDM_SOUND_PARAM_S *pstSoundParam);

#ifdef __cplusplus
}
#endif

#endif
