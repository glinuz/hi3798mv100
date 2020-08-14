#ifndef __FUNC_VID_H__
#define __FUNC_VID_H__

#include "vdp_define.h"
#include "vdp_hal_vid.h"

typedef enum
{
	VDP_VID_CFG_TYP  = 0,
	VDP_VID_CFG_RAND    ,
	VDP_VID_CFG_BUTT
}VDP_VID_CFG_MODE_E;

HI_S32 VDP_FUNC_InitVidInfo(VDP_LAYER_VID_E u32LayerId, VDP_LAYER_VID_INFO_S *pstInfo, HI_U32 w, HI_U32 h);
HI_S32 VDP_FUNC_GetVidInfo(VDP_LAYER_VID_E u32LayerId, VDP_LAYER_VID_INFO_S *pstInfo, VDP_VID_CFG_MODE_E enCfgMode);

#endif
