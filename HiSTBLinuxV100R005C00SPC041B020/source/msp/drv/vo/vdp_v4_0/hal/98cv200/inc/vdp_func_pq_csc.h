#ifndef __FUNC_PQ_CSC_H__
#define __FUNC_PQ_CSC_H__
#include "hi_type.h"
#include "vdp_define.h"
#include "drv_pq_ext.h"

HI_S32 VDP_FUNC_GetCSCCoef(HI_PQ_CSC_CRTL_S* pstCscCtrl, VDP_CSC_MODE_E enCscMode, HI_PQ_CSC_COEF_S* pstCscCoef, HI_PQ_CSC_DCCOEF_S* pstCscDcCoef);
HI_S32 VDP_FUNC_PQ_GetWcgGmmCoef(HI_U32 u32Layer, HI_U16 *pu16GmmCoef);
HI_S32 VDP_FUNC_PQ_GetWcgDegmmCoef(HI_U32 u32Layer, HI_U16 *pu16DegmmCoef);
HI_S32 VDP_FUNC_SetWcgGmmCoef(HI_U32 u32Layer, HI_U16 *pu16GmmCoef);
HI_S32 VDP_FUNC_SetWcgDeGmmCoef(HI_U32 u32Layer, HI_U16 *pu16DegmmCoef);


#endif
