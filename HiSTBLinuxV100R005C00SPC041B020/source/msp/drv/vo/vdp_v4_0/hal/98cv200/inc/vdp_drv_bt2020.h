#ifndef __DRV_BT2020_H__
#define __DRV_BT2020_H__

#include "drv_pq_ext.h"
#include "vdp_drv_hihdr.h"



HI_VOID VDP_DRV_SetVidBt2020Mode(HI_PQ_CSC_MODE_E enPqCscMode);
HI_VOID VDP_DRV_SetGfxBt2020Mode(HI_PQ_CSC_MODE_E enPqCscMode);
HI_S32 VDP_DRV_SetHisiDmCvmCoef(HI_S16 * ptmLutI, HI_S16 * ptmLutS, HI_S16 * psmLutI, HI_S16 * psmLutS);
HI_VOID VDP_DRV_SetHisigammaCoef(HI_U32 *u32L2GLutX,HI_U32 *u32L2GLutA,HI_U32 *u32L2GLutB);
HI_VOID VDP_DRV_SetHisiDegammaCoef (HI_U32 *u32Degamma,HI_U32 u32Length);
HI_VOID VDP_DRV_SetHisiGdm709_2020(HI_VOID);
HI_S32 VDP_DRV_SetHisiBt2020GfxBaseSetting(VDP_HISI_HDR_CFG_S  *stHdrCfg);
HI_S32 VDP_DRV_SetHisiGfxMode(VDP_HISI_HDR_CFG_S  *stHdrCfg, HI_PQ_HDR_CFG  *pstDmKs);
HI_VOID VDP_DRV_SetVidRgb2YuvMode(VDP_CSC_MODE_E enCscMode);
HI_VOID VDP_DRV_SetGfxRgbtYuvMode(VDP_CSC_MODE_E enVdpCscMode);
HI_VOID VDP_DRV_SetHisiHlgDegammaCoef (HI_U32 *u32Degamma,HI_U32 u32Length);
HI_VOID VDP_DRV_SetHisiHlgGammaCoef(HI_U32 *u32IngammaLutX,HI_U32 *u32IngammaLutA,HI_U32 *u32IngammaLutB,HI_U32 *u32OutgammaLutX,HI_U32 *u32OutgammaLutA,HI_U32 *u32OutgammaLutB	);

#endif
