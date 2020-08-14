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

HI_VOID HAL_TransferToDolbyHalHdrCfg(HI_DRV_DISP_DOLBY_HDR_CFG_S *pstDrvPara,VDP_HDR_CFG_S *pstHalHdrCfg);

HI_VOID HAL_TransferToHiHalHdrCfg(HI_DRV_DISP_HISI_HDR_CFG_S *pstDrvPara,VDP_HISI_HDR_CFG_S *pstHalHdrCfg);
    
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

