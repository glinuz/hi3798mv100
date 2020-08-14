#ifndef __VDP_DRV_HIHDR_H__
#define __VDP_DRV_HIHDR_H__
#include "VdpHiHDR_cfg.h"
#include "vdp_drv_ip_vdm.h"



#ifndef  __DISP_PLATFORM_BOOT__
//HI_VOID VDP_FUNC_SetVdmMode(HI_U32 enLayer, VDP_VID_VDM_MODE_E VdmMode, VDP_VDM_CFG_S * pstCfg);
HI_S32 VDP_DRV_SetHisiHdrMode(VDP_HISI_HDR_CFG_S *pstHdrCfg,HI_PQ_HDR_CFG  *pstPqHdrCfg);
HI_S32 VDP_DRV_GetHiHdrCfg(VDP_HISI_HDR_CFG_S *pstHdrCfg,DmCfg_t  *pstDmCfg);
HI_S32 VDP_DRV_SetHiHdrCfg(DmCfg_t  *pstDmCfg);

HI_VOID VDP_DRV_HiSiHdrClean(HI_BOOL bclear);
HI_VOID VDP_DRV_HiSiHdrMute(HI_BOOL bMuteEn);

#endif

#endif



