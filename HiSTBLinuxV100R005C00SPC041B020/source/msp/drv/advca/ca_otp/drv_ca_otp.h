/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_ca_otp.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/

#ifndef __DRV_CA_OTP_H__
#define __DRV_CA_OTP_H__

#include "drv_ca_otp_common.h"
#include "drv_advca_external.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

HI_S32 CA_OTP_GetBloadDecEn(HI_U32 *pu32Enable);
HI_S32 CA_OTP_SetTsOutDisableLock(HI_VOID);
HI_S32 CA_OTP_GetChipId(HI_U32 *pu32ChipID);
HI_S32 CA_OTP_GetBlMSIDCheckEnLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_SetMktIdP(HI_VOID);
HI_S32 CA_OTP_GetRSAKeyN(HI_U8 *pu8Key);
HI_S32 CA_OTP_SetCSA3LadderLevel(CA_OTP_KEY_LEVEL_E level);
HI_S32 CA_OTP_SetLpcMasterEn(HI_U32 u32LpcMasterDisable);
HI_S32 CA_OTP_SetBlMSIDCheckEn(HI_U32 u32Disable);
HI_S32 CA_OTP_SetRSAKeyE(HI_U8 *pu8Key);
HI_S32 CA_OTP_LockHardAESSel(HI_VOID);
HI_S32 CA_OTP_GetSPERootKey(HI_U8 *pu8Key);
HI_S32 CA_OTP_GetDVBLadderLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetDesDisableLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetBootSelCtrlLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_DisableSelfBoot(HI_U32 u32SelfBoot);
HI_S32 CA_OTP_GetDDRScrambleEnLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetR2RRootKey(HI_U8 *pu8Key);
HI_S32 CA_OTP_LockHardSPESel(HI_VOID);
HI_S32 CA_OTP_GetUniqueVersionId(HI_U8 *pu8VersionID);
HI_S32 CA_OTP_GetRigthCtrlEn(HI_U32 *pu32Enable);
HI_S32 CA_OTP_SetTsOutDisable(HI_U32 u32Disable);
HI_S32 CA_OTP_SetBloadModeSelLock(HI_VOID);
HI_S32 CA_OTP_SetTdesLock(HI_U32 u32TdesLock);
HI_S32 CA_OTP_SetUniqueMarketId(HI_U8 *pu8Id);
HI_S32 CA_OTP_GetTSCWDeactiveLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetSecureChipId(CA_OTP_VENDOR_TYPE_E *pSecureChipId);
HI_S32 CA_OTP_SetSPERootKey(HI_U8 *pu8Key);
HI_S32 CA_OTP_SetCSA3HardOnlyEnLock(HI_VOID);
HI_S32 CA_OTP_SetUniqueVersionId(HI_U8 *pu8Id);
HI_S32 CA_OTP_GetLinkProtectDisable(HI_U32 *pu32Disable);
HI_S32 CA_OTP_GetCSA2LadderLevel(CA_OTP_KEY_LEVEL_E *pLevel);
HI_S32 CA_OTP_SetSecureChipFlag(HI_U32 u32Flag);
HI_S32 CA_OTP_GetStbSnP(HI_U32 *pu32StbSnP);
HI_S32 CA_OTP_SetR2RLadderLevel(CA_OTP_KEY_LEVEL_E level);
HI_S32 CA_OTP_SetDesDisable(HI_U32 u32Disable);
HI_S32 CA_OTP_GetSelfBoot(HI_U32 *pu32SelfBoot);
HI_S32 CA_OTP_SetSCSActive(HI_VOID);
HI_S32 CA_OTP_SetRuntimeCheckEnLock(HI_VOID);
HI_S32 CA_OTP_GetR2RLadderLevel(CA_OTP_KEY_LEVEL_E *pLevel);
HI_S32 CA_OTP_GetWakeupDDRCheckEn(HI_U32 *pu32Disable);
HI_S32 CA_OTP_GetTSSPDeactive(HI_U32 *pu32Deactive);
HI_S32 CA_OTP_GetRuntimeCheckEnLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetSecureChipFlag(HI_U32 *pu32SecureChipFlag);
HI_S32 CA_OTP_GetVersionIDCheckEnLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_SetVersionIdCheckEnLock(HI_VOID);
HI_S32 CA_OTP_SetVersionIdCheckEn(HI_U32 u32Disable);
HI_S32 CA_OTP_SetBootSelCtrl(HI_U32 u32BootSel);
HI_S32 CA_OTP_SetOtpGlobalLockEn(HI_U32 u32Enable);
HI_S32 CA_OTP_SetLpcMasterEnLock(HI_VOID);
HI_S32 CA_OTP_GetRootKeyLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_SetSecureChipId(CA_OTP_VENDOR_TYPE_E secureChipId);
HI_S32 CA_OTP_LockBootDecEn(HI_VOID);
HI_S32 CA_OTP_GetTSCSA3HardonlyEn(HI_U32 *pu32Enable);
HI_S32 CA_OTP_GetRSAKeyE(HI_U8 *pu8Key);
HI_S32 CA_OTP_SetWakeupDDRCheckEnLock(HI_VOID);
HI_S32 CA_OTP_GetTSSPHardonlyEn(HI_U32 *pu32Enable);
HI_S32 CA_OTP_SetBootDecMode(HI_U32 u32Mode);
HI_S32 CA_OTP_GetCSA3RootKey(HI_U8 *pu8Key);
HI_S32 CA_OTP_GetRSAKeyLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetRuntimeCheckEn(HI_U32 *pu32Disable);
HI_S32 CA_OTP_GetJtagReadDisable(HI_U32 *pu32Disable);
HI_S32 CA_OTP_GetLinkProtectDisableLock(HI_U32 *pu32Disable);
HI_S32 CA_OTP_GetJtagWriteDisable(HI_U32 *pu32Disable);
HI_S32 CA_OTP_SetWakeupDDRCheckEn(HI_U32 u32Disable);
HI_S32 CA_OTP_SetTSCSA3HardonlyEn(HI_U32 u32Enable);
HI_S32 CA_OTP_GetOtpGlobalLockEnLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetTSCSA3Deactive(HI_U32 *pu32Deactive);
HI_S32 CA_OTP_GetChipIdLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_SetDDRScrambleEnLock(HI_VOID);
HI_S32 CA_OTP_SetSelfbootLock(HI_VOID);
HI_S32 CA_OTP_GetBootMode(CA_OTP_FLASH_TYPE_E *pType);
HI_S32 CA_OTP_GetR2RLadderLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_SetDesDisableLock(HI_VOID);
HI_S32 CA_OTP_GetDcasKLDisableLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetOtpGlobalLockEn(HI_U32 *pu32Enable);
HI_S32 CA_OTP_SetStbSnP(HI_U32 u32StbSnP);
HI_S32 CA_OTP_GetLpcMasterEn(HI_U32 *pu32LpcMasterDisable);
HI_S32 CA_OTP_SetLowPowerDisable(HI_U32 u32Flag);
HI_S32 CA_OTP_GetSCSActive(HI_U32 *pu32ActiveFlag);
HI_S32 CA_OTP_GetLpcEnLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_SetDcasKLDisableLock(HI_VOID);
HI_S32 CA_OTP_GetVersionIdCheckLock(HI_U32 *pu32VersionIdCheckP);
HI_S32 CA_OTP_GetLpcEn(HI_U32 *pu32LpcDisable);
HI_S32 CA_OTP_GetHDCPKey(HI_U8 *pu8HDCPData);
HI_S32 CA_OTP_SetDDRScrambleEn(HI_U32 u32Mode);
HI_S32 CA_OTP_GetTSCsa2HardonlyEn(HI_U32 *pu32Enable);
HI_S32 CA_OTP_DisableLinkProtect(HI_VOID);
HI_S32 CA_OTP_GetDesDisable(HI_U32 *pu32Disable);
HI_S32 CA_OTP_GetJtagKey(HI_U8 *pu8Key);
HI_S32 CA_OTP_SetRigthCtrlEn(HI_VOID);
HI_S32 CA_OTP_SetTSCSA3DeactiveLock(HI_VOID);
HI_S32 CA_OTP_GetTsOutDisableLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetTSCWDeactive(HI_U32 *pu32Deactive);
HI_S32 CA_OTP_GetBloadModeSel(HI_U32 *pu32Mode);
HI_S32 CA_OTP_SetCSA3RootKey(HI_U8 *pu8Key);
HI_S32 CA_OTP_GetDDRScrambleEn(HI_U32 *pu32Mode);
HI_S32 CA_OTP_SetLpcEnLock(HI_VOID);
HI_S32 CA_OTP_GetCSA3LadderLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetBootSelCtrl(HI_U32 *pu32BootSel);
HI_S32 CA_OTP_LockR2RHardKey(HI_VOID);
HI_S32 CA_OTP_GetBloadModeSelLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_LockRSAKey(HI_VOID);
HI_S32 CA_OTP_GetCSA2RootKey(HI_U8 *pu8Key);
HI_S32 CA_OTP_GetBootSelLock0(HI_U32 *pu32BootSelLock);
HI_S32 CA_OTP_SetBootSelLock1(HI_VOID);
HI_S32 CA_OTP_GetUniqueChipId(HI_U8 *pu8ChipID);
HI_S32 CA_OTP_SetCSA3LadderLock(HI_VOID);
HI_S32 CA_OTP_SetLowPowerDisableLock(HI_VOID);
HI_S32 CA_OTP_SetBootSelCtrlLock(HI_VOID);
HI_S32 CA_OTP_GetMCK1DESHardonlyEnLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_SetTSCWDeactive(HI_U32 u32Deactive);
HI_S32 CA_OTP_SetChipIdLock(HI_VOID);
HI_S32 CA_OTP_SetTSSPDeactive(HI_U32 u32Deactive);
HI_S32 CA_OTP_GetLpcMasterEnLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetTSCSA3DeactiveLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetTSCsa2HardonlyEnLock(HI_U32 *pu32Enable);
HI_S32 CA_OTP_GetMktIdP(HI_U32 *pu32mktIdP);
HI_S32 CA_OTP_GetTSSPHardonlyEnLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_LockHardTDESSel(HI_VOID);
HI_S32 CA_OTP_GetDcasKLDisable(HI_U32 *pu32Disable);
HI_S32 CA_OTP_DisableJtagWrite(HI_VOID);
HI_S32 CA_OTP_GetTSCSA3HardonlyEnLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_SetDebugDisable(HI_VOID);
HI_S32 CA_OTP_GetMCK1DESHardonlyEn(HI_U32 *pu32Enable);
HI_S32 CA_OTP_GetBloadDecEnLock(HI_U32 *pu32BloadDecEncLock);
HI_VOID CA_OTP_DoApbWriteByte(HI_U32 addr,HI_U32 tdata);
HI_S32 CA_OTP_SetApbWriteProtect(HI_VOID);
HI_S32 CA_OTP_SetBootSelLock0(HI_VOID);
HI_S32 CA_OTP_SetSPLadderLevel(CA_OTP_KEY_LEVEL_E level);
HI_S32 CA_OTP_SetRootKeyLock(HI_VOID);
HI_S32 CA_OTP_SetBloadDecEn(HI_U32 u32Enable);
HI_S32 CA_OTP_SetSecureChipFlagLock(HI_VOID);
HI_S32 CA_OTP_SetJtagProtectMode(CA_OTP_JTAG_MODE_E JtagMode);
HI_S32 CA_OTP_GetJtagProtectMode(CA_OTP_JTAG_MODE_E *pJtagMode);
HI_S32 CA_OTP_SetSPLadderLock(HI_VOID);
HI_S32 CA_OTP_GetSecureChipFlagLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetSPLadderLevel(CA_OTP_KEY_LEVEL_E *pLevel);
HI_S32 CA_OTP_LockHardCwSel(HI_VOID);
HI_S32 CA_OTP_SetCSA2LadderLevel(CA_OTP_KEY_LEVEL_E level);
HI_S32 CA_OTP_SetBootMode(CA_OTP_FLASH_TYPE_E type);
HI_S32 CA_OTP_SetTSSPDeactiveLock(HI_VOID);
HI_S32 CA_OTP_GetVersionIDCheckEn(HI_U32 *pu32VersionIDCheckEn);
HI_S32 CA_OTP_SetDVBLadderLock(HI_VOID);
HI_S32 CA_OTP_SetTSCWDeactiveLock(HI_VOID);
HI_S32 CA_OTP_GetApbWriteProtect(unsigned int *penable);
HI_U32 CA_OTP_DoApbParaRead(HI_U32 addr);
HI_S32 CA_OTP_GetTSSPDeactiveLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_SetBlMSIDCheckEnLock(HI_VOID);
HI_S32 CA_OTP_GetUniqueMarketId(HI_U8 *pu8MarketID);
HI_S32 CA_OTP_SetMCTDESHardonlyEn(HI_U32 u32Enable);
HI_S32 CA_OTP_SetDesHardDisableLock(HI_VOID);
HI_S32 CA_OTP_GetLowPowerDisableLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetWakeupDDRCheckEnLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_SetOtpGlobalLockEnLock(HI_VOID);
HI_S32 CA_OTP_GetMCAESHardonlyEnLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_SetJtagKey(HI_U8 *pu8Key);
HI_S32 CA_OTP_SetTSSPEHardonlyEn(HI_U32 u32Enable);
HI_S32 CA_OTP_SetChipId(HI_U32 u32Id);
HI_S32 CA_OTP_SetSecureChipP(HI_VOID);
HI_S32 CA_OTP_GetCSA3LadderLevel(CA_OTP_KEY_LEVEL_E *pLevel);
HI_S32 CA_OTP_SetR2RLadderLock(HI_VOID);
HI_S32 CA_OTP_SetR2RRootKey(HI_U8 *pu8Key);
HI_S32 CA_OTP_GetSelfbootLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_SetVersionIdLock(HI_VOID);
HI_S32 CA_OTP_Get_UniqueSTBSN(HI_U8 *pu8SN);
HI_S32 CA_OTP_SetLpcEn(HI_U32 u32LpcDisable);
HI_S32 CA_OTP_SetJtagReadDisable(HI_VOID);
HI_S32 CA_OTP_GetMCAESHardonlyEn(HI_U32 *pu32Enable);
HI_S32 CA_OTP_GetDebugDisable(HI_U32 *pu32Disable);
HI_S32 CA_OTP_SetDcasKLDisable(HI_U32 u32Disable);
HI_S32 CA_OTP_GetTsOutDisable(HI_U32 *pu32Disable);
HI_S32 CA_OTP_SetUniqueSTBSN(HI_U8 *pu8SN);
HI_S32 CA_OTP_GetLowPowerDisable(HI_U32 *pu32Flag);
HI_S32 CA_OTP_GetSPLadderLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_GetBlMSIDCheckEn(HI_U32 *pu32BlMSIDCheckEn);
HI_S32 CA_OTP_GetBootSelLock1(HI_U32 *pu32BootSelLock);
HI_S32 CA_OTP_SetMCAESHardonlyEn(HI_U32 u32Enable);
HI_S32 CA_OTP_GetSecureChipP(HI_U32 *pu32SecureChipP);
HI_S32 CA_OTP_SetRuntimeCheckEn(HI_U32 u32Disable);
HI_S32 CA_OTP_SetRSAKeyN(HI_U8 *pu8Key);
HI_S32 CA_OTP_SetCSA2RootKey(HI_U8 *pu8Key);
HI_S32 CA_OTP_SetMarketId(HI_U32 u32Id);
HI_S32 CA_OTP_GetMarketId(HI_U32 *pu32MarketID);
HI_S32 CA_OTP_SetSTBSN(HI_U32 u32SN);
HI_S32 CA_OTP_GetSTBSN(HI_U32 *pu32SN);
HI_S32 CA_OTP_LockHardNVSel(HI_VOID);
HI_S32 CA_OTP_DisableNV(HI_VOID);
HI_S32 CA_OTP_DisableRuntimeCheck(HI_VOID);
HI_S32 CA_OTP_SetCSA2Deactive(HI_VOID);
HI_S32 CA_OTP_GetCSA2Deactive(HI_U32 *pu32CSA2Deactive);
HI_S32 CA_OTP_SetR2RDeactive(HI_VOID);
HI_S32 CA_OTP_GetR2RDeactive(HI_U32 *pu32R2RDeactive);
HI_S32 CA_OTP_SetSPDeactive(HI_VOID);
HI_S32 CA_OTP_GetSPDeactive(HI_U32 *pu32SPDeactive);
HI_S32 CA_OTP_SetTSCSA3Deactive(HI_U32 u32Deactive);
HI_S32 CA_OTP_SetDesHardDisable(HI_U32 u32Disable);
HI_S32 CA_OTP_GetDesHardDisable(HI_U32 *pu32Disable);
HI_S32 CA_OTP_GetDesHardDisableLock(HI_U32 *pu32Lock);
HI_S32 CA_OTP_SetCSA3Deactive(HI_VOID);
HI_S32 CA_OTP_GetCSA3Deactive(HI_U32 *pu32Deactive);
HI_S32 CA_OTP_SetLinkProtectDisable(HI_U32 u32Disable);
HI_S32 CA_OTP_SetTSKLNVDisable(HI_U32 u32Disable);
HI_S32 CA_OTP_SetTSKLOthersDisable(HI_U32 u32Disable);
HI_S32 CA_OTP_SetBloadEncDisable(HI_U32 u32Disable);
HI_S32 CA_OTP_SetNfRNGDisable(HI_U32 u32Disable);
HI_S32 CA_OTP_SetTSCsa2HardonlyEn(HI_U32 u32Enable);
HI_S32 CA_OTP_SetTSDESHardonlyEn(HI_U32 u32Enable);
HI_S32 CA_OTP_SetTSNVHardonlyEn(HI_U32 u32Enable);
HI_S32 CA_OTP_SetTSOthersHardonlyEn(HI_U32 u32Enable);
HI_S32 CA_OTP_SetSelftestDisable(HI_VOID);
HI_S32 CA_OTP_GetTdesLock(HI_U32 *pu32TdesLock);
HI_S32 CA_OTP_SetCsa3En(HI_VOID);
HI_S32 CA_OTP_GetCsa3En(HI_U32 *pu32CSA3Disable);
HI_VOID CA_OTP_GetSRBit(int pos, int * pvalue);
HI_S32  CA_OTP_SetSRBit(int pos);
HI_VOID CA_OTP_DoApbWriteBit(HI_U32 addr,HI_U32 bit_pos ,HI_U32  bit_value);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __DRV_CA_OTP_H__ */

