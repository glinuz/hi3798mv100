/******************************************************************************
  Copyright (C), 2001-2016, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hdr_transfer.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2016/06/15
Last Modified :
Description   : interfaces to adapt hal's structures.
Function List :
History       :
******************************************************************************/
#ifndef __HDR_TRANSFER_H__
#define __HDR_TRANSFER_H__

#include "drv_disp_hdr.h"
#include "vdp_drv_hdr.h"
#include "vdp_drv_hihdr.h"


#ifdef __cplusplus
#if __cplusplus
 extern "C"
{
#endif
#endif

#ifndef __DISP_PLATFORM_BOOT__
HI_VOID HAL_TransferToDolbyHalHdrCfg(HI_DRV_DISP_DOLBY_HDR_CFG_S *pstDrvPara,VDP_HDR_CFG_S *pstHalHdrCfg);

HI_VOID HAL_TransferToHiHalHdrCfg(HI_DRV_DISP_HISI_HDR_CFG_S *pstDrvPara,VDP_HISI_HDR_CFG_S *pstHalHdrCfg);

#endif

/**********************put functions of hal here temply***************************/
HI_VOID VDP_DRV_HiSiHdrClean(HI_BOOL bclear);
HI_VOID VDP_DRV_HiSiHdrMute(HI_BOOL bMuteEn);

HI_VOID VDP_DRV_MaskAndBackupGfx(HI_U32 u32GfxLayerId,HI_U32 *pu32Alpha,HI_U32 *pu32MuteState);
HI_VOID VDP_DRV_UnmaskGfx(HI_U32 u32GfxLayerId,HI_U32 u32Alpha,HI_U32 u32MuteState);
HI_VOID VDP_DRV_SetGfxMixvBypassState(HI_U32 u32GfxLayerId,HI_BOOL bEnable);
HI_VOID VDP_DRV_GetGfxState(HI_BOOL *pbNeedProcessGfx);




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

