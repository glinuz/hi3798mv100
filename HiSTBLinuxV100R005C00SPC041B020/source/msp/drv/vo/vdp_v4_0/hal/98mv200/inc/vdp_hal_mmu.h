#ifndef __HAL_MMU_H__
#define __HAL_MMU_H__
#include "vdp_define.h"
#if EDA_TEST
#include <stdio.h>
#include <stdlib.h>
#include <vdp_assert.h>
#endif
#include "vdp_define.h"
#if VDP_C_COMPILE
#endif


//-------------------------------------------------------------------
//VID_BEGIN
//-------------------------------------------------------------------
HI_VOID VDP_VID_SetAllRegionSmmuBypassDisable (HI_U32 u32Data,HI_U32 u32Bypass);
HI_VOID VDP_VID_Set16RegionSmmuLumBypass   (HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32Bypass);
HI_VOID VDP_VID_Set16RegionSmmuChromBypass (HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32Bypass);

HI_VOID VDP_GFX_SetSmmu3dRBypass           (HI_U32 u32Data, HI_U32 u32Bypass);
HI_VOID VDP_GFX_SetSmmu2dBypass            (HI_U32 u32Data, HI_U32 u32Bypass);
HI_VOID VDP_GFX_SetSmmuGBHeadBypass        (HI_U32 u32Data, HI_U32 u32Bypass);
HI_VOID VDP_GFX_SetSmmuARHeadBypass        (HI_U32 u32Data, HI_U32 u32Bypass);

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

HI_VOID VDP_SMMU_SetIntsTlbinvalidRdMsk    (HI_U32 u32Msk) ;
HI_VOID VDP_SMMU_SetIntsTlbinvalidWrMsk    (HI_U32 u32Msk) ;
HI_VOID VDP_SMMU_SetIntsPtwTransMsk        (HI_U32 u32Msk);
HI_VOID VDP_SMMU_SetIntsTlbmissMsk         (HI_U32 u32Msk);

HI_VOID VDP_SMMU_SetIntsTlbinvalidRdClr(HI_U32 u32Clear);
HI_VOID VDP_SMMU_SetIntsTlbinvalidWrClr(HI_U32 u32Clear);
HI_VOID VDP_SMMU_SetIntsPtwTransClr        (HI_U32 u32Clear);
HI_VOID VDP_SMMU_SetIntsTlbmissClr         (HI_U32 u32Clear);

HI_VOID VDP_SMMU_SetIntnsTlbinvalidRdMsk(HI_U32 u32Msk);
HI_VOID VDP_SMMU_SetIntnsTlbinvalidWrMsk(HI_U32 u32Msk);
HI_VOID VDP_SMMU_SetIntnsPtwTransMsk       (HI_U32 u32Msk);
HI_VOID VDP_SMMU_SetIntnsTlbmissMsk        (HI_U32 u32Msk);

HI_VOID VDP_SMMU_SetIntnsTlbinvalidRdClr(HI_U32 u32Clear);
HI_VOID VDP_SMMU_SetIntnsTlbinvalidWrClr(HI_U32 u32Clear);
HI_VOID VDP_SMMU_SetIntnsPtwTransClr       (HI_U32 u32Clear);
HI_VOID VDP_SMMU_SetIntnsTlbmissClr        (HI_U32 u32Clear);

HI_VOID VDP_SMMU_SetScbTtbr                (HI_U32 u32scb_ttbr);
HI_VOID VDP_SMMU_SetCbTtbr                 (HI_U32 u32cb_ttbr);

HI_VOID VDP_SMMU_SetErrRdAddr              (HI_U32 u32Addr);
HI_VOID VDP_SMMU_SetErrNsRdAddr(HI_U32 u32Addr);
HI_VOID VDP_SMMU_SetErrWrAddr              (HI_U32 u32Addr);
HI_VOID VDP_SMMU_SetErrNsWrAddr(HI_U32 u32Addr);

HI_U32 VDP_SMMU_GetIntsTlbinvalidRdSta(HI_VOID);
HI_U32 VDP_SMMU_GetIntsTlbinvalidWrSta(HI_VOID);
HI_U32 VDP_SMMU_GetIntsPtwTransSta(HI_VOID);
HI_U32 VDP_SMMU_GetIntsTlbmissSta(HI_VOID);

HI_U32 VDP_SMMU_GetIntnsTlbinvalidRdSta(HI_VOID);
HI_U32 VDP_SMMU_GetIntnsTlbinvalidWrSta(HI_VOID);
HI_U32 VDP_SMMU_GetIntnsPtwTransSta(HI_VOID);
HI_U32 VDP_SMMU_GetIntnsTlbmissSta(HI_VOID);


HI_U32 VDP_SMMU_GetRdErrAddr(HI_VOID);
HI_U32 VDP_SMMU_GetWrErrAddr(HI_VOID);
HI_U32 VDP_SMMU_GetSecureRdErrAddr(HI_VOID);
HI_U32 VDP_SMMU_GetSecureWrErrAddr(HI_VOID);



#endif
