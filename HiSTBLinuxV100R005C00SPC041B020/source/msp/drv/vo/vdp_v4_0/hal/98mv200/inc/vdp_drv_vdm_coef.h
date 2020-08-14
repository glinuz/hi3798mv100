// *****************************************************************************
//
// Copyright(c) 2015,Hisilicon Technologies Co., Ltd.
// All Rights Reserved
//
// Project name : VDP
// File Name    : coef.cpp
// Author       : z00294247
// Date         : 2015/7/17
//
// Module Name  : COEF.CPP
// -----------------------------------------------------------------------------
// Abstract     :
//code_template.v*
//
// *****************************************************************************
// Called By    : TOP
// Modification History
// -----------------------------------------------------------------------------
// $Log :     $
//
//
// --=========================================================================--

#ifndef __VDM_COEF_H__
#define __VDM_COEF_H__


#include "hi_type.h"
//#include "c_union_define.h"
//#include "vdp_sti.h"
//#include <stdio.h>
//#include "vdp_env_def.h"
#include "VdpHiHDR_cfg.h"
#include "vdp_drv_hihdr.h"


//HI_VOID vSetVdmCoef_Degmm(VDP_RM_COEF_MODE_E enCoefMode);
//HI_VOID vSetVdmCoef_Tmap(VDP_RM_COEF_MODE_E enCoefMode);
//HI_VOID vSetVdmCoef_Gmm(VDP_RM_COEF_MODE_E enCoefMode);

#ifndef  __DISP_PLATFORM_BOOT__
HI_VOID Vdp_Drv_SetVdmCoef_gmm(HIHDR_SCENE_MODE_E  enHiHdrMode,HI_PQ_HDR_CFG  *HiHdrCfg);
HI_VOID Vdp_Drv_SetVdmCoef_Degmm(HIHDR_SCENE_MODE_E  enHiHdrMode,HI_PQ_HDR_CFG  *HiHdrCfg);
HI_VOID Vdp_Drv_SetVdmCoef_Tmapping(HIHDR_SCENE_MODE_E  enHiHdrMode,HI_PQ_HDR_CFG  *HiHdrCfg);
HI_VOID VDP_DRV_SetBt2020GammaCoef(HIHDR_SCENE_MODE_E  enHiHdrMode,HI_PQ_HDR_CFG   *HiHdrCfg);
HI_VOID VDP_DRV_SetBt2020DegmmaCoef(HIHDR_SCENE_MODE_E  enHiHdrMode,HI_PQ_HDR_CFG  *HiHdrCfg);

#endif

#endif

