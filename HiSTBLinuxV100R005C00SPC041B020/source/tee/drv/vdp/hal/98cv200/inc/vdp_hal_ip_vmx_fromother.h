#ifndef __HAL_IP_VMX_FROMOTHER_H__
#define __HAL_IP_VMX_FROMOTHER_H__

#include "vdp_define.h"



HI_VOID  VDP_VID_SetParaUpd_VMX(HI_U32 u32Data, VDP_VID_PARA_E enMode);

HI_VOID  VDP_VP0_SetParaUpd_VMX(HI_U32 u32Data, VDP_VP_PARA_E  enMode);


HI_VOID  VDP_VID_GetLayerEnable_VMX(HI_U32 u32Data, HI_U32 *pu32Enable);


HI_VOID  VDP_VID_GetVideoReso_VMX(HI_U32 u32Data, HI_U32 *pu32Width, HI_U32 *pu32Height);

HI_VOID  VDP_VID_GetVideoPos_VMX(HI_U32 u32Data, HI_U32 *pu32X, HI_U32 *pu32Y);


HI_VOID VDP_DISP_GetHDREnable_VMX(HI_U32 *pu32Enable);

HI_VOID  VDP_VP_GetLayerReso_VMX(HI_U32 *pu32Width, HI_U32 *pu32Height);

HI_VOID  VDP_VP_GetDispMode(HI_U32 *u32DispMode);


HI_VOID  VDP_MixvGetTopLayer(HI_U32 *u32TopLayer);




#endif
