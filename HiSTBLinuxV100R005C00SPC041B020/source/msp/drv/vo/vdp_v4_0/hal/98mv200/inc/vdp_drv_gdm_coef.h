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

#ifndef __GDM_COEF_H__
#define __GDM_COEF_H__


#include "hi_type.h"
//#include "c_union_define.h"
//#include "vdp_sti.h"
//#include <stdio.h>
//#include "vdp_env_def.h"
#include "VdpHiHDR_cfg.h"
#include "vdp_drv_hihdr.h"
#include "vdp_drv_ip_gdm.h"

HI_VOID Vdp_Drv_SetGdmCoef_Gmma(GDM_SCENE_MODE_E   enHiHdrGfxMode);
HI_VOID Vdp_Drv_SetGdmCoef_Tmmp(GDM_SCENE_MODE_E   enHiHdrGfxMode);

#endif

