#ifndef __DRV_PQ_HDR_H__
#define __DRV_PQ_HDR_H__

#include "vdp_define.h"
#include "KdmTypeFxp.h"
HI_S32 VDP_DRV_SetDmCvmCoef(HI_S16 * ptmLutI, HI_S16 * ptmLutS, HI_S16 * psmLutI, HI_S16 * psmLutS);
HI_VOID VDP_DRV_SetDmDegammaCoef (DmKsFxp_t *pKs) ;
HI_VOID VDP_DRV_SetDmgammaCoef(DmKsFxp_t *pKs);
HI_VOID VDP_DRV_SetGdmDegammaCoef(HI_U32 *u32GdmDeGammaCoef) ;
HI_VOID VDP_DRV_SetGdmgammaCoef(HI_U32 *u32GdmGammaCoef) ;


#endif
