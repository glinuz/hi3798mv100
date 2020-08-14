#ifndef __WIN_HAL_ADP_H
#define __WIN_HAL_ADP_H
#include "drv_pq_ext.h"
HI_S32 Chip_Specific_LayerZmeFunc(HI_PQ_ZME_LAYER_E u32LayerId,
                            HI_PQ_ZME_PARA_IN_S *pstZmeI,
                            HI_U32     bFirlterEnable);
                            
HI_S32 Chip_Specific_WinHalSetAddr(HI_U32 u32LayerId, 
				WIN_HAL_PARA_S *pstPara,
				HI_S32 s32exl);

HI_S32 Chip_Specific_WinDoFurtherZmeProcess(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara);

HI_S32 Chip_Specific_WinHalSetRegionMute(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara);
HI_S32 Chip_Specific_SetDispMode(HI_U32 u32id, HI_DRV_DISP_STEREO_MODE_E eMode);

HI_S32 Chip_Specific_SetLayerCapability(VIDEO_LAYER_CAPABILITY_S *pstVideoLayerCap);
HI_U32 Chip_Specific_GetMixvMaxNumvber(VDP_CBM_MIX_E eM);

HI_VOID Chip_Specific_CBM_GetMixvPrio(VDP_CBM_MIX_E enMixer, HI_U32 u32prio, HI_U32 *pu32layer_id);
HI_VOID Chip_Specific_CBM_SetMixvPrio(VDP_CBM_MIX_E enMixer,HI_U32 *pMixArray,HI_U32 u32prio);

#endif
