#ifndef __FUNC_HDR_H__
#define __FUNC_HDR_H__

#include "vdp_drv_hdr.h"
#include "vdp_drv_func.h"

HI_S32 VDP_DRV_SetHdrEdaCfg       (VDP_HDR_CFG_S *stHdrCfg);
HI_S32 VDP_FUNC_GetVdmCfg         (VDP_HDR_DM_CFG_E DmMode, DmKsFxp_t * pKs);

#endif
