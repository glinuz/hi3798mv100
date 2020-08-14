#ifndef __HAL_MMU_H__
#define __HAL_MMU_H__
#include "vdp_define.h"
#if EDA_TEST
#include <stdio.h>
#include <stdlib.h>
#endif

#if VDP_CBB_FPGA
//#include <vdp_assert.h>
#endif

//-------------------------------------------------------------------
//VID_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VID_SetAllRegionSmmuBypassDisable (HI_U32 u32Data,HI_U32 u32Bypass);
HI_VOID VDP_VID_Set16RegionSmmuLumBypass   (HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32Bypass);
HI_VOID VDP_VID_Set16RegionSmmuChromBypass (HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32Bypass);

HI_VOID VDP_GFX_SetSmmu3dRBypass           (HI_U32 u32Data, HI_U32 u32Bypass);
HI_VOID VDP_GFX_SetSmmu2dBypass            (HI_U32 u32Data, HI_U32 u32Bypass);

HI_VOID VDP_WBC_SetSmmuCBypass             (HI_U32 u32Data, HI_U32 u32Bypass);
HI_VOID VDP_WBC_SetSmmuLBypass             (HI_U32 u32Data, HI_U32 u32Bypass);

HI_VOID VDP_WBC_SetSmmuBypass              (HI_U32 u32Data, HI_U32 u32Bypass);

//-------------------------------------------------------------------
//SMMU Begin
//-------------------------------------------------------------------
HI_VOID VDP_SMMU_SetPtwPf                  (HI_U32 u32ptw_pf);
HI_VOID VDP_SMMU_SetIntEn                  (HI_U32 u32Enable);
HI_VOID VDP_SMMU_SetGlbBypass              (HI_U32 u32GlbBypass);
HI_VOID VDP_SMMU_SetAutoClkGtEn            (HI_U32 u32Enable);

HI_VOID VDP_SMMU_SetIntsTlbinvalidMsk      (HI_U32 u32Msk);
HI_VOID VDP_SMMU_SetIntsPtwTransMsk        (HI_U32 u32Msk);
HI_VOID VDP_SMMU_SetIntsTlbmissMsk         (HI_U32 u32Msk);

HI_VOID VDP_SMMU_SetIntsTlbinvalidClr      (HI_U32 u32Clear);
HI_VOID VDP_SMMU_SetIntsPtwTransClr        (HI_U32 u32Clear);
HI_VOID VDP_SMMU_SetIntsTlbmissClr         (HI_U32 u32Clear);

HI_VOID VDP_SMMU_SetIntnsTlbinvalidMsk     (HI_U32 u32Msk);
HI_VOID VDP_SMMU_SetIntnsPtwTransMsk       (HI_U32 u32Msk);
HI_VOID VDP_SMMU_SetIntnsTlbmissMsk        (HI_U32 u32Msk);

HI_VOID VDP_SMMU_SetIntnsTlbinvalidClr     (HI_U32 u32Clear);
HI_VOID VDP_SMMU_SetIntnsPtwTransClr       (HI_U32 u32Clear);
HI_VOID VDP_SMMU_SetIntnsTlbmissClr        (HI_U32 u32Clear);

HI_VOID VDP_SMMU_SetScbTtbr                (HI_U32 u32scb_ttbr);
HI_VOID VDP_SMMU_SetCbTtbr                 (HI_U32 u32cb_ttbr);

HI_VOID VDP_SMMU_SetErrRdAddr              (HI_U32 u32Addr);
HI_VOID VDP_SMMU_SetErrWrAddr              (HI_U32 u32Addr);


HI_BOOL VDP_SMMU_GetIntsTlbinvalidSta(HI_VOID);
HI_BOOL VDP_SMMU_GetIntsPtwTransSta(HI_VOID);
HI_BOOL VDP_SMMU_GetIntsTlbmissSta(HI_VOID);

HI_BOOL VDP_SMMU_GetIntnsTlbinvalidSta(HI_VOID);
HI_BOOL VDP_SMMU_GetIntnsPtwTransSta(HI_VOID);
HI_BOOL VDP_SMMU_GetIntnsTlbmissSta(HI_VOID);


HI_U32 VDP_SMMU_GetRdErrAddr(HI_VOID);
HI_U32 VDP_SMMU_GetWrErrAddr(HI_VOID);
HI_U32 VDP_SMMU_GetSecureRdErrAddr(HI_VOID);
HI_U32 VDP_SMMU_GetSecureWrErrAddr(HI_VOID);


#endif
