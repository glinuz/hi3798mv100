#ifndef __PQ_MNG_IFMD_H__
#define __PQ_MNG_IFMD_H__

#include "hi_type.h"

#include "pq_hal_comm.h"
#include "pq_hal_fmd.h"
#include "drv_pq_table.h"
#include "pq_mng_ifmd_alg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



#if 0
HI_S32 PQ_MNG_IFMD_SofeParaInitByHandle(HI_U32 u32HandleNo);
HI_S32 PQ_MNG_InitIfmd(PQ_PARAM_S* pstPqParam);
HI_S32 PQ_MNG_DeInitIfmd(HI_VOID);
HI_S32 PQ_MNG_IfmdDect(PQ_IFMD_CALC_S* pstIfmdCalc, REG_PLAYBACK_CONFIG_S* pstIfmdResult);
HI_S32 PQ_MNG_GetIfmdDectInfo(REG_PLAYBACK_CONFIG_S* pstIfmdInfo);
HI_S32 PQ_MNG_SetDieOutMode(PQ_DIE_OUT_MODE_E enMode);
HI_S32 PQ_MNG_SetFodMode(PQ_FOD_MODE_E enMode);
HI_S32 PQ_MNG_GetDeiMdLum(HI_VOID);
#endif
HI_S32 PQ_MNG_RegisterFMD(PQ_REG_TYPE_E enType);

HI_S32 PQ_MNG_UnRegisterFMD(HI_VOID);



#ifdef __cplusplus
#if __cplusplus
}
#endif

#endif

#endif

