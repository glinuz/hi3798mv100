/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_advca_v300.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_advca_internal.h"
#include "drv_advca_external.h"
#include "asm/io.h"
static HI_U32 *g_pDrvSema = NULL;

static HI_S32 DRV_ADVCA_V300_Ioctl_Child(CA_CMD_SUPPER_ID_S *pstCmdParam)
{
    HI_S32 ret = HI_SUCCESS;

    if (NULL == pstCmdParam)
    {
        HI_ERR_CA("Null pointer input!\n");
        return HI_FAILURE;
    }

    switch (pstCmdParam->enCmdChildID)
    {
        case CMD_CHILD_ID_GET_VENDOR_ID:
        {
            CA_OTP_VENDOR_TYPE_E *penVendorID = (CA_OTP_VENDOR_TYPE_E *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetVendorId(penVendorID);
            break;
        }
        case CMD_CHILD_ID_SET_KL_DPA_CLK_SEL_EN:
        {
            HI_BOOL bValue = *(HI_BOOL *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetKlDPAClkSelEn(bValue);
            break;
        }
        case CMD_CHILD_ID_GET_KL_DPA_CLK_SEL_EN:
        {
            HI_BOOL *pbValue = (HI_BOOL *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetKlDPAClkSelEn(pbValue);
            break;
        }
        case CMD_CHILD_ID_SET_KL_DPA_FILTER_CLK_EN:
        {
            HI_BOOL bValue = *(HI_BOOL *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetKlDPAFilterClkEn(bValue);
            break;
        }
        case CMD_CHILD_ID_GET_KL_DPA_FILTER_CLK_EN:
        {
            HI_BOOL *pbValue = (HI_BOOL *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetKlDPAFilterClkEn(pbValue);
            break;
        }
        case CMD_CHILD_ID_SET_MC_DPA_CLK_SEL_EN:
        {
            HI_BOOL bValue = *(HI_BOOL *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetMcDPAClkSelEn(bValue);
            break;
        }
        case CMD_CHILD_ID_GET_MC_DPA_CLK_SEL_EN:
        {
            HI_BOOL *pbValue = (HI_BOOL *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetMcDPAClkSelEn(pbValue);
            break;
        }
        case CMD_CHILD_ID_SET_PVR_DPA_FILTER_CLK_EN:
        {
            HI_BOOL bValue = *(HI_BOOL *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetPvrDPAFilterClkEn(bValue);
            break;
        }
        case CMD_CHILD_ID_GET_PVR_DPA_FILTER_CLK_EN:
        {
            HI_BOOL *pbValue = (HI_BOOL *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetPvrDPAFilterClkEn(pbValue);
            break;
        }
        case CMD_CHILD_ID_SET_MISC_LV_SEL:
        {
            HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel = *(HI_UNF_ADVCA_KEYLADDER_LEV_E *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetMiscKlLevel(enLevel);
            break;
        }
        case CMD_CHILD_ID_GET_MISC_LV_SEL:
        {
            HI_UNF_ADVCA_KEYLADDER_LEV_E *penValue = (HI_UNF_ADVCA_KEYLADDER_LEV_E *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetMiscKlLevel(penValue);
            break;
        }
        case CMD_CHILD_ID_SET_OEM_ROOT_KEY:
        {
            HI_U8 *pu8OEMRootKey = (HI_U8 *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetOEMRootKey(pu8OEMRootKey);
            break;
        }
        case CMD_CHILD_ID_SET_SOC_OTP_TZ_EN:
        {
            HI_BOOL bValue = *(HI_BOOL *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetTZEnable(bValue);
            break;
        }
        case CMD_CHILD_ID_GET_SOC_OTP_TZ_EN:
        {
            HI_BOOL *pbEn = (HI_BOOL *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetTZEnStatus(pbEn);
            break;
        }
        case CMD_CHILD_ID_SET_OTP_TZ:
        {
            CA_OTP_TZ_DATA_S stOTPSetParam = *(CA_OTP_TZ_DATA_S *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetOTPTzData(&stOTPSetParam);
            break;
        }
        case CMD_CHILD_ID_GET_OTP_TZ:
        {
            CA_OTP_TZ_DATA_S *pstOTPSetParam = (CA_OTP_TZ_DATA_S *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetOTPTzData(pstOTPSetParam);
            break;
        }
        case CMD_CHILD_ID_LOCK_OTP_TZ:
        {
            HI_UNF_ADVCA_LOCK_TZ_OTP_PARAM_S stOTPLockParam = *(HI_UNF_ADVCA_LOCK_TZ_OTP_PARAM_S *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_LockOTPTzData(&stOTPLockParam);
            break;
        }
        case CMD_CHILD_ID_CA_GDRM_CRYPT:
        {
            HI_U32 u32GDRMFlag = 0;
            CA_GDRM_CRYPTPM_S *pstParam = (CA_GDRM_CRYPTPM_S *)pstCmdParam->pu8ParamBuf;

            ret = HAL_ADVCA_V300_CryptGDRM(pstParam->stCACryptParams.ladder, pstParam->stCACryptParams.pDin, pstParam->u32KeyAddr, 1, pstParam->stCACryptParams.enKlTarget);
            if (HI_UNF_ADVCA_KEYLADDER_LEV3 == pstParam->stCACryptParams.ladder)
            {
                ret |= HAL_ADVCA_V300_GetGDRMFlag(&u32GDRMFlag);
                *(pstParam->pu32GDRMFlag) = u32GDRMFlag;
            }
            break;
        }
        case CMD_CHILD_ID_GET_CHECKSUM_FLAG:
        {
            HI_U32 *pu32ChecksumFlag = (HI_U32 *)pstCmdParam->pu8ParamBuf;
            ret = HAL_ADVCA_V300_GetSecretKeyChecksumFlag(pu32ChecksumFlag);
            break;
        }
        case CMD_CHILD_ID_GET_CASTATE:
        {
            HI_U32 *pu32Statte = (HI_U32 *)pstCmdParam->pu8ParamBuf;
            HAL_ADVCA_V300_GetCAStates(pu32Statte);
            break;
        }
        case CMD_CHILD_ID_MISC_SETALG:
        {
            HI_UNF_ADVCA_ALG_TYPE_E enMISCAlgType = *(HI_UNF_ADVCA_ALG_TYPE_E *)pstCmdParam->pu8ParamBuf;
            ret = HAL_ADVCA_V300_SetMiscAlgorithm(enMISCAlgType);
            break;
        }
        case CMD_CHILD_ID_MISC_GETALG:
        {
            HI_UNF_ADVCA_ALG_TYPE_E *penMISCAlgType = (HI_UNF_ADVCA_ALG_TYPE_E *)pstCmdParam->pu8ParamBuf;
            ret = HAL_ADVCA_V300_GetMiscAlgorithm(penMISCAlgType);
            break;
        }
        case CMD_CHILD_ID_SET_MISC_ROOT_KEY:
        {
            HI_U8 *pu8MiscRootKey = (HI_U8 *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetMISCRootKey(pu8MiscRootKey);
            break;
        }
        case CMD_CHILD_ID_SET_ESCK:
        {
            HI_U8 *pu8ESCK = (HI_U8 *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetESCK(pu8ESCK);
            break;
        }
        case CMD_CHILD_ID_GET_ESCK_LOCK:
        {
            HI_BOOL *pbLock = (HI_BOOL *)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetESCKLock(pbLock);
            break;
        }
        case CMD_CHILD_ID_LOCK_RSA_KEY:
        {
            ret = DRV_CA_OTP_V200_LockRSAKey();
            break;
        }
        case CMD_CHILD_ID_SET_R2R_ROOT_KEY_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetR2rRootKeyLock();
            break;
        }
        case CMD_CHILD_ID_GET_R2R_ROOT_KEY_LOCK:
        {
            HI_U32 *pR2rRootKeyLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetR2rRootKeyLock(pR2rRootKeyLock);
            break;
        }
        case CMD_CHILD_ID_SET_SP_ROOT_KEY_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetSpRootKeyLock();
            break;
        }
        case CMD_CHILD_ID_GET_SP_ROOT_KEY_LOCK:
        {
            HI_U32 *pSpRootKeyLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetSpRootKeyLock(pSpRootKeyLock);
            break;
        }
        case CMD_CHILD_ID_SET_DVB_CSA3_ROOT_KEY_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetCsa3RootKeyLock();
            break;
        }
        case CMD_CHILD_ID_GET_DVB_CSA3_ROOT_KEY_LOCK:
        {
            HI_U32 *pCsa3RootKeyLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetCsa3RootKeyLock(pCsa3RootKeyLock);
            break;
        }
        case CMD_CHILD_ID_SET_JTAG_KEY_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetJtagKeyLock();
            break;
        }
        case CMD_CHILD_ID_GET_JTAG_KEY_LOCK:
        {
            HI_U32 *pJtagKeyLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetJtagKeyLock(pJtagKeyLock);
            break;
        }
        case CMD_CHILD_ID_SET_ID_WORD:
        {
            HI_U32 *pIdWord = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetIdWord(*pIdWord);
            break;
        }
        case CMD_CHILD_ID_GET_ID_WORD:
        {
            HI_U32 *pIdWord = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetIdWord(pIdWord);
            break;
        }
        case CMD_CHILD_ID_SET_ID_WORD_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetIdWordLock();
            break;
        }
        case CMD_CHILD_ID_GET_ID_WORD_LOCK:
        {
            HI_U32 *pIdWordLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetIdWordLock(pIdWordLock);
            break;
        }
        case CMD_CHILD_ID_SET_CSA2_ROOTKEY_CRC:
        {
            HI_U32 *pCrc = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetCsa2RootKeyCrc(*pCrc);
            break;
        }
        case CMD_CHILD_ID_GET_CSA2_ROOTKEY_CRC:
        {
            HI_U32 *pCrc = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetCsa2RootKeyCrc(pCrc);
            break;
        }
        case CMD_CHILD_ID_SET_CSA2_ROOTKEY_CRC_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetCsa2RootKeyCrcLock();
            break;
        }
        case CMD_CHILD_ID_GET_CSA2_ROOTKEY_CRC_LOCK:
        {
            HI_U32 *pLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetCsa2RootKeyCrcLock(pLock);
            break;
        }
        case CMD_CHILD_ID_SET_R2R_ROOTKEY_CRC:
        {
            HI_U32 *pCrc = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetR2rRootKeyCrc(*pCrc);
            break;
        }
        case CMD_CHILD_ID_GET_R2R_ROOTKEY_CRC:
        {
            HI_U32 *pCrc = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetR2rRootKeyCrc(pCrc);
            break;
        }
        case CMD_CHILD_ID_SET_R2R_ROOTKEY_CRC_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetR2rRootKeyCrcLock();
            break;
        }
        case CMD_CHILD_ID_GET_R2R_ROOTKEY_CRC_LOCK:
        {
            HI_U32 *pLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetR2rRootKeyCrcLock(pLock);
            break;
        }
        case CMD_CHILD_ID_SET_SP_ROOTKEY_CRC:
        {
            HI_U32 *pCrc = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetSpRootKeyCrc(*pCrc);
            break;
        }
        case CMD_CHILD_ID_GET_SP_ROOTKEY_CRC:
        {
            HI_U32 *pCrc = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetSpRootKeyCrc(pCrc);
            break;
        }
        case CMD_CHILD_ID_SET_SP_ROOTKEY_CRC_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetSpRootKeyCrcLock();
            break;
        }
        case CMD_CHILD_ID_GET_SP_ROOTKEY_CRC_LOCK:
        {
            HI_U32 *pLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetSpRootKeyCrcLock(pLock);
            break;
        }
        case CMD_CHILD_ID_SET_CSA3_ROOTKEY_CRC:
        {
            HI_U32 *pCrc = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetCsa3RootKeyCrc(*pCrc);
            break;
        }
        case CMD_CHILD_ID_GET_CSA3_ROOTKEY_CRC:
        {
            HI_U32 *pCrc = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetCsa3RootKeyCrc(pCrc);
            break;
        }
        case CMD_CHILD_ID_SET_CSA3_ROOTKEY_CRC_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetCsa3RootKeyCrcLock();
            break;
        }
        case CMD_CHILD_ID_GET_CSA3_ROOTKEY_CRC_LOCK:
        {
            HI_U32 *pLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetCsa3RootKeyCrcLock(pLock);
            break;
        }
        case CMD_CHILD_ID_SET_JTAG_KEY_CRC:
        {
            HI_U32 *pCrc = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetJtagKeyCrc(*pCrc);
            break;
        }
        case CMD_CHILD_ID_GET_JTAG_KEY_CRC:
        {
            HI_U32 *pCrc = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetJtagKeyCrc(pCrc);
            break;
        }
        case CMD_CHILD_ID_SET_JTAG_KEY_CRC_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetJtagKeyCrcLock();
            break;
        }
        case CMD_CHILD_ID_GET_JTAG_KEY_CRC_LOCK:
        {
            HI_U32 *pLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetJtagKeyCrcLock(pLock);
            break;
        }
        case CMD_CHILD_ID_SET_SCS_EN_BAK:
        {
            HI_U32 *pBak = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetScsEnBak(*pBak);
            break;
        }
        case CMD_CHILD_ID_GET_SCS_EN_BAK:
        {
            HI_U32 *pBak = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetScsEnBak(pBak);
            break;
        }
        case CMD_CHILD_ID_SET_SCS_EN_BAK_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetScsEnBakLock();
            break;
        }
        case CMD_CHILD_ID_GET_SCS_EN_BAK_LOCK:
        {
            HI_U32 *pLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetScsEnBakLock(pLock);
            break;
        }
        case CMD_CHILD_ID_SET_JTAG_MODE_BAK:
        {
            HI_U32 *pBak = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetJtagModeBak(*pBak);
            break;
        }
        case CMD_CHILD_ID_GET_JTAG_MODE_BAK:
        {
            HI_U32 *pBak = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetJtagModeBak(pBak);
            break;
        }
        case CMD_CHILD_ID_SET_JTAG_MODE_BAK_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetJtagModeBakLock();
            break;
        }
        case CMD_CHILD_ID_GET_JTAG_MODE_BAK_LOCK:
        {
            HI_U32 *pLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetJtagModeBakLock(pLock);
            break;
        }
        case CMD_CHILD_ID_SET_RIGHT_CTRL_EN_BAK:
        {
            HI_U32 *pBak = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetRightCtrlEnBak(*pBak);
            break;
        }
        case CMD_CHILD_ID_GET_RIGHT_CTRL_EN_BAK:
        {
            HI_U32 *pBak = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetRightCtrlEnBak(pBak);
            break;
        }
        case CMD_CHILD_ID_SET_RIGHT_CTRL_EN_BAK_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetRightCtrlEnBakLock();
            break;
        }
        case CMD_CHILD_ID_GET_RIGHT_CTRL_EN_BAK_LOCK:
        {
            HI_U32 *pLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetRightCtrlEnBakLock(pLock);
            break;
        }
        case CMD_CHILD_ID_SET_MISC_KL_DISABLE:
        {
            HI_U32 *pDisable = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetMiscKlDisable(*pDisable);
            break;
        }
        case CMD_CHILD_ID_GET_MISC_KL_DISABLE:
        {
            HI_U32 *pDisable = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetMiscKlDisable(pDisable);
            break;
        }
        case CMD_CHILD_ID_SET_MISC_KL_DISABLE_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetMiscKlDisableLock();
            break;
        }
        case CMD_CHILD_ID_GET_MISC_KL_DISABLE_LOCK:
        {
            HI_U32 *pLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetMiscKlDisableLock(pLock);
            break;
        }
        case CMD_CHILD_ID_SET_GG_KL_DISABLE:
        {
            HI_U32 *pDisable = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetGgKlDisable(*pDisable);
            break;
        }
        case CMD_CHILD_ID_GET_GG_KL_DISABLE:
        {
            HI_U32 *pDisable = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetGgKlDisable(pDisable);
            break;
        }
        case CMD_CHILD_ID_SET_GG_KL_DISABLE_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetGgKlDisableLock();
            break;
        }
        case CMD_CHILD_ID_GET_GG_KL_DISABLE_LOCK:
        {
            HI_U32 *pLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetGgKlDisableLock(pLock);
            break;
        }
        case CMD_CHILD_ID_SET_BOOT_INFO_DISABLE:
        {
            ret = DRV_CA_OTP_V200_SetBootInfoDisable();
            break;
        }
        case CMD_CHILD_ID_GET_BOOT_INFO_DISABLE:
        {
            HI_U32 *pDisable = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetBootInfoDisable(pDisable);
            break;
        }
        case CMD_CHILD_ID_SET_CHECKSUM_READ_DISABLE:
        {
            ret = DRV_CA_OTP_V200_SetChecksumReadDisable();
            break;
        }
        case CMD_CHILD_ID_GET_CHECKSUM_READ_DISABLE:
        {
            HI_U32 *pDisable = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetChecksumReadDisable(pDisable);
            break;
        }
        case CMD_CHILD_ID_SET_SELF_BOOT_DISABLE_BAK:
        {
            HI_U32 *pBak = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetSelfBootDisableBak(*pBak);
            break;
        }
        case CMD_CHILD_ID_GET_SELF_BOOT_DISABLE_BAK:
        {
            HI_U32 *pBak = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetSelfBootDisableBak(pBak);
            break;
        }
        case CMD_CHILD_ID_SET_TZ_AREA_LOCK:
        {
            HI_U32 *pLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetTzAreaLock(*pLock);
            break;
        }
        case CMD_CHILD_ID_GET_TZ_AREA_LOCK:
        {
            HI_U32 *pLock = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetTzAreaLock(pLock);
            break;
        }
        case CMD_CHILD_ID_SET_BOOTMODE_0:
        {
            HI_UNF_ADVCA_FLASH_TYPE_E BootMode = *(HI_UNF_ADVCA_FLASH_TYPE_E*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetBootMode_0(BootMode);
            break;
        }
        case CMD_CHILD_ID_GET_BOOTMODE_0:
        {
            HI_UNF_ADVCA_FLASH_TYPE_E *pBootMode = (HI_UNF_ADVCA_FLASH_TYPE_E*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetBootMode_0((CA_OTP_FLASH_TYPE_E *)pBootMode);
            break;
        }
        case CMD_CHILD_ID_SET_BOOTMODE_1:
        {
            HI_U32 BootMode = *(HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_SetBootMode_1(BootMode);
            break;
        }
        case CMD_CHILD_ID_GET_BOOTMODE_1:
        {
            HI_U32 *pBootMode = (HI_U32*)pstCmdParam->pu8ParamBuf;
            ret = DRV_CA_OTP_V200_GetBootMode_1((CA_OTP_FLASH_TYPE_E *)pBootMode);
            break;
        }
        case CMD_CHILD_ID_LOCK_MISC_KL_DISABLE:
        {
            HI_U32 u32MiscKlDisable = 1;

            ret = DRV_CA_OTP_V200_SetMiscKLDisable(u32MiscKlDisable);
            break;
        }
        case CMD_CHILD_ID_LOCK_GG_KL_DISABLE:
        {
            HI_U32 u32GgKlDisable = 1;

            ret = DRV_CA_OTP_V200_SetGgKLDisable(u32GgKlDisable);
            break;
        }
        case CMD_CHILD_ID_LOCK_TSKL_CSA3_DISABLE:
        {
            HI_U32 u32TsklCsa3Disable = 1;

            ret = DRV_CA_OTP_V200_SetTsklCsa3Disable(u32TsklCsa3Disable);
            break;
        }
        case CMD_CHILD_ID_OTP_READ_BYTE:
        {
            HI_U32 Addr = *(HI_U32 *)(pstCmdParam->pu8ParamBuf);

            pstCmdParam->pu8ParamBuf[4] = DRV_CA_OTP_V200_ReadByte(Addr);

            ret = HI_SUCCESS;
            break;
        }
        default:
        {
            HI_ERR_CA("Invalid child cmd id!\n");
            return HI_FAILURE;
        }
    }

    return ret;
}

HI_S32 DRV_ADVCA_V300_Ioctl(HI_U32 cmd, HI_VOID* arg)
{
    HI_S32 ret;
    HI_U32 u32VendorId = 0;
    static HI_U8 keybuf[512] = {0};
    HI_U8 IDbuf[4] = {0};

    ret = ca_down_interruptible(g_pDrvSema);
    if (HI_FAILURE == ret)
    {
        HI_ERR_CA("%s:  down_interruptible err ! \n", __FUNCTION__);
        return HI_FAILURE;
    }

    ret = DRV_CA_OTP_V200_GetVendorId((CA_OTP_VENDOR_TYPE_E *)&u32VendorId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("%s:  get vendor type err ! \n", __FUNCTION__);
        ca_up(g_pDrvSema);
        return ret;
    }

    switch (cmd)
    {
        case CMD_CA_GET_CHIPID:
        {
            HI_U32 *pu32ChipId = (HI_U32*)arg;

            if (HI_NULL == pu32ChipId)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            if ((CA_OTP_VENDOR_CONAX == u32VendorId) || (CA_OTP_VENDOR_NAGRA == u32VendorId))
            {
                ret = DRV_CA_OTP_V200_GetChipId(pu32ChipId);
            }
            else
            {
                memset(IDbuf, 0x0, 4);
                ret = DRV_CA_OTP_V200_GetUniqueChipId(IDbuf);
                *pu32ChipId = (IDbuf[0]<<24) + (IDbuf[1]<<16) + (IDbuf[2]<<8) + IDbuf[3];
            }
            break;
        }
        case CMD_CA_GET_UNIQUE_CHIPID:
        {
            ret = HI_ERR_CA_NOT_SUPPORT;
            break;
        }
        case CMD_CA_SET_MARKETID:
        {
            if (CA_OTP_VENDOR_NAGRA == u32VendorId)
            {
                HI_U32 u32MaketId;

                if (HI_NULL == arg)
                {
                    HI_ERR_CA("invalid arg\n");
                    ca_up(g_pDrvSema);
                    return HI_FAILURE;
                }
                u32MaketId = *(HI_U32*)arg;
                ret = DRV_CA_OTP_V200_Set_MarketId(u32MaketId);
            }
            else
            {
                CA_KEY_S *pKey = (CA_KEY_S*)arg;

                if (HI_NULL == pKey)
                {
                    HI_ERR_CA("invalid arg\n");
                    ca_up(g_pDrvSema);
                    return HI_FAILURE;
                }
                memset(IDbuf, 0x0, 4);
                IDbuf[0] = pKey->KeyBuf[3];
                IDbuf[1] = pKey->KeyBuf[2];
                IDbuf[2] = pKey->KeyBuf[1];
                IDbuf[3] = pKey->KeyBuf[0];
                ret = DRV_CA_OTP_V200_Set_UniqueMarketId(IDbuf);
            }
            break;
        }
        case CMD_CA_GET_MARKETID:
        {
            if (CA_OTP_VENDOR_NAGRA == u32VendorId)
            {
                HI_U32 *pu32MarketId = (HI_U32*)arg;

                if (HI_NULL == pu32MarketId)
                {
                    HI_ERR_CA("invalid arg\n");
                    ca_up(g_pDrvSema);
                    return HI_FAILURE;
                }
                ret = DRV_CA_OTP_V200_MarketId(pu32MarketId);
            }
            else
            {
                CA_KEY_S *pKey = (CA_KEY_S*)arg;

                if (HI_NULL == pKey)
                {
                    HI_ERR_CA("invalid arg\n");
                    ca_up(g_pDrvSema);
                    return HI_FAILURE;
                }
                memset(IDbuf, 0x0, 4);
                ret = DRV_CA_OTP_V200_Get_UniqueMarketId(IDbuf);
                pKey->KeyBuf[0] = IDbuf[3];
                pKey->KeyBuf[1] = IDbuf[2];
                pKey->KeyBuf[2] = IDbuf[1];
                pKey->KeyBuf[3] = IDbuf[0];
            }
            break;
        }
        case CMD_CA_SET_STBSN:
        {
            if (CA_OTP_VENDOR_NAGRA == u32VendorId)
            {
                HI_U32 u32StbSN;

                if (HI_NULL == arg)
                {
                    HI_ERR_CA("invalid arg\n");
                    ca_up(g_pDrvSema);
                    return HI_FAILURE;
                }
                u32StbSN = *(HI_U32*)arg;
                ret = DRV_CA_OTP_V200_Set_STBSN(u32StbSN);
            }
            else
            {
                CA_KEY_S *pKey = (CA_KEY_S*)arg;

                if (HI_NULL == pKey)
                {
                    HI_ERR_CA("invalid arg\n");
                    ca_up(g_pDrvSema);
                    return HI_FAILURE;
                }
                memset(IDbuf, 0x0, 4);
                IDbuf[0] = pKey->KeyBuf[3];
                IDbuf[1] = pKey->KeyBuf[2];
                IDbuf[2] = pKey->KeyBuf[1];
                IDbuf[3] = pKey->KeyBuf[0];
                ret = DRV_CA_OTP_V200_Set_UniqueSTBSN(IDbuf);
            }
            break;
        }
        case CMD_CA_GET_STBSN:
        {
            if (CA_OTP_VENDOR_NAGRA == u32VendorId)
            {
                HI_U32 *pu32StbSN = (HI_U32*)arg;

                if (HI_NULL == pu32StbSN)
                {
                    HI_ERR_CA("invalid arg\n");
                    ca_up(g_pDrvSema);
                    return HI_FAILURE;
                }
                ret = DRV_CA_OTP_V200_GetSTBSN(pu32StbSN);
            }
            else
            {
                CA_KEY_S *pKey = (CA_KEY_S*)arg;

                if (HI_NULL == pKey)
                {
                    HI_ERR_CA("invalid arg\n");
                    ca_up(g_pDrvSema);
                    return HI_FAILURE;
                }
                memset(IDbuf, 0x0, 4);
                ret = DRV_CA_OTP_V200_Get_UniqueSTBSN(IDbuf);
                pKey->KeyBuf[0] = IDbuf[3];
                pKey->KeyBuf[1] = IDbuf[2];
                pKey->KeyBuf[2] = IDbuf[1];
                pKey->KeyBuf[3] = IDbuf[0];
            }
            break;
        }
        case CMD_CA_SET_VERSIONID:
        {
            CA_KEY_S *pKey = (CA_KEY_S*)arg;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            memset(IDbuf, 0x0, 4);
            IDbuf[0] = pKey->KeyBuf[3];
            IDbuf[1] = pKey->KeyBuf[2];
            IDbuf[2] = pKey->KeyBuf[1];
            IDbuf[3] = pKey->KeyBuf[0];
            ret = DRV_CA_OTP_V200_Set_UniqueVersionId(IDbuf);
            break;
        }
        case CMD_CA_GET_VERSIONID:
        {
            CA_KEY_S *pKey = (CA_KEY_S*)arg;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            memset(IDbuf, 0x0, 4);
            ret = DRV_CA_OTP_V200_Get_UniqueVersionId(IDbuf);
            pKey->KeyBuf[0] = IDbuf[3];
            pKey->KeyBuf[1] = IDbuf[2];
            pKey->KeyBuf[2] = IDbuf[1];
            pKey->KeyBuf[3] = IDbuf[0];
            break;
        }
        case CMD_CA_LOCKHARDCWSEL:
        {
            ret = DRV_CA_OTP_V200_LockHardCwSel();
            break;
        }
        case CMD_CA_LOCKBOOTDECEN:
        {
            ret = DRV_CA_OTP_V200_SetBloadDecEn(1);
            break;
        }
        case CMD_CA_LOCKR2RHARDKEY:
        {
            ret = DRV_CA_OTP_V200_LockR2RHardKey();
            break;
        }
        case CMD_CA_STATHARDCWSEL :
        {
            HI_U32 *pbLock = (HI_U32*)arg;

            if (HI_NULL == pbLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_StatHardCwSel(pbLock);
            break;
        }
        case CMD_CA_STATR2RHARDKEY :
        {
            HI_U32 *pbLock = (HI_U32*)arg;

            if (HI_NULL == pbLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_StatR2RHardKey(pbLock);
            break;
        }
        case CMD_CA_STATTDESLOCK :
        {
            HI_U32 *pbLock = (HI_U32*)arg;

            if (HI_NULL == pbLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTdesLock(pbLock);
            break;
        }
        case CMD_CA_DISABLELPT:
        {
            ret = DRV_CA_OTP_V200_DisableLinkProtect();
            break;
        }
        case CMD_CA_DISABLESELFBOOT:
        {
            ret = DRV_CA_OTP_V200_DisableSelfBoot(1);
            break;
        }
        case CMD_CA_SET_SELFBOOT:
        {
            HI_U32 *pu32DisableSelfBoot = (HI_U32 *)arg;

            if (HI_NULL == pu32DisableSelfBoot)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_DisableSelfBoot(*pu32DisableSelfBoot);
            break;
        }
        case CMD_CA_SET_BOOTMODE:
        {
            HI_UNF_ADVCA_FLASH_TYPE_E BootMode;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            BootMode = *(HI_UNF_ADVCA_FLASH_TYPE_E*)arg;
            ret = DRV_CA_OTP_V200_SetBootMode((CA_OTP_FLASH_TYPE_E)BootMode);
            break;
        }
        case CMD_CA_GET_BOOTMODE:
        {
            HI_UNF_ADVCA_FLASH_TYPE_E *pBootMode = (HI_UNF_ADVCA_FLASH_TYPE_E *)arg;

            if (HI_NULL == pBootMode)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetBootMode(pBootMode);
            break;
        }
        case CMD_CA_SET_SCSACTIVE:
        {
            ret = DRV_CA_OTP_V200_SetSCSActive();
            break;
        }
        case CMD_CA_GET_SCSACTIVE:
        {
            HI_U32 *pu32SecBoot = (HI_U32*)arg;

            if (HI_NULL == pu32SecBoot)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetSCSActive(pu32SecBoot);
            break;
        }
        case CMD_CA_SET_JTAGPRTMODE:
        {
            HI_UNF_ADVCA_JTAG_MODE_E JtagMode;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            JtagMode = *(HI_UNF_ADVCA_JTAG_MODE_E*)arg;
            ret = DRV_CA_OTP_V200_SetJtagProtectMode(JtagMode);
            break;
        }
        case CMD_CA_GET_JTAGPRTMODE:
        {
            HI_UNF_ADVCA_JTAG_MODE_E *pJtagMode = (HI_UNF_ADVCA_JTAG_MODE_E*)arg;

            if (HI_NULL == pJtagMode)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetJtagProtectMode((CA_OTP_JTAG_MODE_E *)pJtagMode);
            break;
        }
        case CMD_CA_DVB_SETLADDER:
        {
            HI_UNF_ADVCA_KEYLADDER_LEV_E DvbLadder;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            DvbLadder = *(HI_UNF_ADVCA_KEYLADDER_LEV_E*)arg;
            ret = HAL_ADVCA_V300_SetCWLadderLevel(DvbLadder);
            break;
        }
        case CMD_CA_DVB_GETLADDER:
        {
            HI_UNF_ADVCA_KEYLADDER_LEV_E *pDvbLadder = (HI_UNF_ADVCA_KEYLADDER_LEV_E*)arg;

            if (HI_NULL == pDvbLadder)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_GetCWLadderLevel(pDvbLadder);
            break;
        }
        case CMD_CA_R2R_SETLADDER:
        {
            HI_UNF_ADVCA_KEYLADDER_LEV_E R2RLadder;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            R2RLadder = *(HI_UNF_ADVCA_KEYLADDER_LEV_E*)arg;
            ret = HAL_ADVCA_V300_SetR2RLadderLevel(R2RLadder);
            break;
        }
        case CMD_CA_R2R_GETLADDER:
        {
            HI_UNF_ADVCA_KEYLADDER_LEV_E *pR2RLadder = (HI_UNF_ADVCA_KEYLADDER_LEV_E*)arg;

            if (HI_NULL == pR2RLadder)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_GetR2RLadderLevel(pR2RLadder);
            break;
        }
        case CMD_CA_SP_SETLADDER:
        {
            HI_UNF_ADVCA_KEYLADDER_LEV_E SPLadder;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            SPLadder = *(HI_UNF_ADVCA_KEYLADDER_LEV_E*)arg;
            ret = HAL_ADVCA_V300_SetSPLadderLevel(SPLadder);
            break;
        }
        case CMD_CA_SP_GETLADDER:
        {
            HI_UNF_ADVCA_KEYLADDER_LEV_E *pSPLadder = (HI_UNF_ADVCA_KEYLADDER_LEV_E*)arg;

            if (HI_NULL == pSPLadder)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_GetSPLadderLevel(pSPLadder);
            break;
        }
        // TODO: Need set? Now fix on TDEA
        case CMD_CA_DVB_SETALG:
        {
            HI_UNF_ADVCA_ALG_TYPE_E CwAlgType;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            CwAlgType = *(HI_U32*)arg;
            ret = HAL_ADVCA_V300_SetCWAlgorithm(CwAlgType);
            break;
        }
        case CMD_CA_DVB_CSA3_SETALG:
        {
            HI_UNF_ADVCA_ALG_TYPE_E Csa3AlgType;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            Csa3AlgType = *(HI_U32*)arg;
            ret = HAL_ADVCA_V300_SetCSA3Algorithm(Csa3AlgType);
            break;
        }
        case CMD_CA_R2R_SETALG:
        {
            HI_UNF_ADVCA_ALG_TYPE_E R2RType;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            R2RType = *(HI_U32*)arg;
            ret = HAL_ADVCA_V300_SetR2RAlgorithm(R2RType);
            break;
        }
        case CMD_CA_SP_SETALG:
        {
            HI_UNF_ADVCA_ALG_TYPE_E eSPType;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            eSPType = *(HI_U32*)arg;
            ret = HAL_ADVCA_V300_SetSPAlgorithm(eSPType);
            break;
        }
        case CMD_CA_SP_SET_DSC_MODE:
        {
            HI_UNF_ADVCA_SP_DSC_MODE_E eSPDscMode;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            eSPDscMode = *(HI_UNF_ADVCA_SP_DSC_MODE_E*)arg;
            ret = HAL_ADVCA_V300_SetSPDscMode(eSPDscMode);
            break;
        }
        case CMD_CA_DVB_GETALG:
        {
            HI_U32 *pu32CwAlgType = (HI_U32*)arg;

            if (HI_NULL == pu32CwAlgType)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_GetCWAlgorithm(pu32CwAlgType);
            break;
        }
        case CMD_CA_DVB_CSA3_GETALG:
        {
            HI_U32 *pu32Csa3AlgType = (HI_U32*)arg;

            if (HI_NULL == pu32Csa3AlgType)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_GetCSA3Algorithm(pu32Csa3AlgType);
            break;
        }
        case CMD_CA_R2R_GETALG:
        {
            HI_U32 *pu32R2RAlgType = (HI_U32*)arg;

            if (HI_NULL == pu32R2RAlgType)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_GetR2RAlgorithm(pu32R2RAlgType);
            break;
        }
        case CMD_CA_SP_GETALG:
        {
            HI_UNF_ADVCA_ALG_TYPE_E *penSPAlgType = (HI_UNF_ADVCA_ALG_TYPE_E*)arg;

            if (HI_NULL == penSPAlgType)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_GetSPAlgorithm(penSPAlgType);
            break;
        }
        case CMD_CA_SP_GET_DSC_MODE:
        {
            HI_UNF_ADVCA_SP_DSC_MODE_E *penSPDscMode = (HI_UNF_ADVCA_SP_DSC_MODE_E*)arg;

            if (HI_NULL == penSPDscMode)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_GetSPDscMode(penSPDscMode);
            break;
        }
        case CMD_CA_DVB_CRYPT:
        {
            CA_CRYPTPM_S *pCryptParam = (CA_CRYPTPM_S*)arg;

            //It is to set DVB Chipset Pairing level 1 Input
            if (HI_NULL == pCryptParam)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_DecryptCw(pCryptParam->ladder,pCryptParam->pDin,0,0);
            break;
        }
        case CMD_CA_R2R_CRYPT:
        {
            CA_CRYPTPM_S *pCryptParam = (CA_CRYPTPM_S*)arg;

            //It is to set Cipher level 1 Input
            if (HI_NULL == pCryptParam)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_CryptR2R(pCryptParam->ladder,pCryptParam->pDin,0,1);
            break;
        }
        case CMD_CA_SP_CRYPT:
        {
            CA_CRYPTPM_S *pCryptParam = (CA_CRYPTPM_S*)arg;

            //It is to set Cipher level 1 Input
            if (HI_NULL == pCryptParam)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_DecryptSP(pCryptParam->ladder,pCryptParam->pDin,0,0,0);
            break;
        }
        case CMD_CA_CSA3_CRYPT:
        {
            CA_CRYPTPM_S *pCryptParam = (CA_CRYPTPM_S*)arg;

            //It is to set CSA3 Chipset Pairing level 1 Input
            if (HI_NULL == pCryptParam)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_DecryptCsa3(pCryptParam->ladder,pCryptParam->pDin,0,0);
            break;
        }
        case CMD_CA_LPK_LOAD:
        {
            CA_LOADLPK_S *pLoadLpk = (CA_LOADLPK_S*)arg;

            /* Decrypt LPK */
            if (HI_NULL == pLoadLpk)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_DecryptLpk(pLoadLpk->EncryptLpk);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_CA("ca_DecryptLpk error\n");
            }
            break;
        }
        case CMD_CA_LPK_DECRYPT :
        {
            CA_DECRYPTLPTDATA_S *pDecryptLptData = (CA_DECRYPTLPTDATA_S*)arg;

            if (HI_NULL == pDecryptLptData)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            /* Decrypt data*/
            ret = HAL_ADVCA_V300_DecryptLPKProtectData(pDecryptLptData->EncryptData,pDecryptLptData->ClearData);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_CA("ca_DecryptLpk error\n");
            }
            break;
        }
        case CMD_CA_SWPK_CRYPT :
        {
            CA_ENCRYPTSWPK_S *pEncryptSwpk = (CA_ENCRYPTSWPK_S*)arg;

            if (HI_NULL == pEncryptSwpk)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
#if defined (CHIP_TYPE_hi3716mv410) || defined (CHIP_TYPE_hi3716mv420) || defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
            //use stb rootkey,not set the bload_dec_mode
#else
            /*Set the bload_dec_mode to R2R Rootkey*/
            ret = DRV_CA_OTP_V200_SetBootDecMode(1);
#endif
            ret |= HAL_ADVCA_V300_EncryptSwpk(pEncryptSwpk->ClearSwpk,pEncryptSwpk->EncryptSwpk);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_CA("ca_EncryptSwpk error\n");
            }
            break;
        }

        case CMD_CA_SET_RSA_KEY_N:
        {
            CA_KEY_S *pKey = (CA_KEY_S*)arg;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_SetRSAKeyN(pKey->KeyBuf);
            break;
        }
        case CMD_CA_SET_RSA_KEY_E:
        {
            CA_KEY_S *pKey = (CA_KEY_S*)arg;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_SetRSAKeyE(pKey->KeyBuf);
            break;
        }
        case CMD_CA_SET_RSA_KEY_LOCK:
        {
            ret = DRV_CA_OTP_V200_LockRSAKey();
            break;
        }
        case CMD_CA_SET_DVB_ROOT_KEY:
        {
            CA_KEY_S *pKey = (CA_KEY_S*)arg;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_SetCSA2RootKey(pKey->KeyBuf);
            break;
        }
        case CMD_CA_SET_DVB_CSA3_ROOT_KEY:
        {
            CA_KEY_S *pKey = (CA_KEY_S*)arg;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_SetCSA3RootKey(pKey->KeyBuf);
            break;
        }
        case CMD_CA_SET_R2R_ROOT_KEY:
        {
            CA_KEY_S *pKey = (CA_KEY_S*)arg;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_SetR2RRootKey(pKey->KeyBuf);
            break;
        }
        case CMD_CA_SET_SP_ROOT_KEY:
        {
            CA_KEY_S *pKey = (CA_KEY_S*)arg;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_SetSPRootKey(pKey->KeyBuf);
            break;
        }
        case CMD_CA_SET_JTAG_KEY:
        {
            CA_KEY_S *pKey = (CA_KEY_S*)arg;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_SetJtagKey(pKey->KeyBuf);
            break;
        }
        case CMD_CA_SET_SECRET_KEY_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetRootKeyLock();
            break;
        }
        case CMD_CA_SET_CHIPID:
        {
            HI_U32 u32ChipID;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32ChipID = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetChipId(u32ChipID);
            break;
        }
        case CMD_CA_SET_CHIPID_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetChipIdLock();
            break;
        }
        case CMD_CA_SET_STBSN_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetStbSnLock_0();
            break;
        }
        case CMD_CA_SET_MKTID_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetMktIdLock();
            break;
        }
        case CMD_CA_SET_VENDOR_ID:
        {
            CA_OTP_VENDOR_TYPE_E vendorType;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            vendorType = *(CA_OTP_VENDOR_TYPE_E*)arg;
            ret = DRV_CA_OTP_V200_SetVendorId(vendorType);
            break;
        }
        case CMD_CA_SET_VENDOR_ID_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetVendorIdLock();
            break;
        }
        case CMD_CA_SET_VERSION_ID_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetVersionIdLock();
            break;
        }
        case CMD_CA_SET_BOOT_SEL_LOCK_0:
        {
            ret = DRV_CA_OTP_V200_SetBootSelLock_0();
            break;
        }
        case CMD_CA_SET_BOOT_SEL_LOCK_1:
        {
            ret = DRV_CA_OTP_V200_SetBootSelLock_1();
            break;
        }
        case CMD_CA_SET_BLOAD_DEC_EN:
        {
            HI_U32 u32BloadEncDisable;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32BloadEncDisable = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetBloadDecEn(u32BloadEncDisable);
            break;
        }
        case CMD_CA_SET_BLOAD_DEC_EN_LOCK:
        {
            ret = DRV_CA_OTP_V200_LockBootDecEn();
            break;
        }
        case CMD_CA_SET_SELFBOOT_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetSelfbootLock();
            break;
        }
        case CMD_CA_SET_LOWPOWER_DISABLE:
        {
            HI_U32 u32LowPowerDisable;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32LowPowerDisable = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetLowPowerDisable(u32LowPowerDisable);
            break;
        }
        case CMD_CA_SET_LOWPOWER_DISABLE_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetLowPowerDisableLock();
            break;
        }
        case CMD_CA_DVB_SETLADDERLOCK:
        {
            ret = DRV_CA_OTP_V200_SetDVBLadderLock();
            break;
        }
        case CMD_CA_CSA3_SETLADDER:
        {
            CA_OTP_KEY_LEVEL_E otpKeyLevel;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            otpKeyLevel = *(CA_OTP_KEY_LEVEL_E*)arg;
            ret = DRV_CA_OTP_V200_SetCSA3LadderLevel(otpKeyLevel);
            break;
        }
        case CMD_CA_CSA3_SETLADDERLOCK:
        {
            ret = DRV_CA_OTP_V200_SetCSA3LadderLock();
            break;
        }
        case CMD_CA_R2R_SETLADDERLOCK:
        {
            ret = DRV_CA_OTP_V200_SetR2RLadderLock();
            break;
        }
        case CMD_CA_SP_SETLADDERLOCK:
        {
            ret = DRV_CA_OTP_V200_SetSPLadderLock();
            break;
        }
        case CMD_CA_SET_LINK_PRT_DISABLE_LOCK:
        {
            ret = DRV_CA_OTP_V200_DisableLinkProtect();
            break;
        }
        case CMD_CA_SET_CSA2_HARDONLY_EN_LOCK:
        {
            ret = DRV_CA_OTP_V200_LockHardCwSel();
            break;
        }
        case CMD_CA_SET_CSA3_HARDONLY_EN:
        {
            HI_U32 u32CSA3HardOnlyEn;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32CSA3HardOnlyEn = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetTSCSA3HardonlyEn(u32CSA3HardOnlyEn);
            break;
        }
        case CMD_CA_SET_CSA3_HARDONLY_EN_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetCSA3HardOnlyEnLock();
            break;
        }
        case CMD_CA_SET_SP_HARDONLY_EN:
        {
            HI_U32 u32SPHardOnlyEn;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32SPHardOnlyEn = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetTSSPEHardonlyEn(u32SPHardOnlyEn);
            break;
        }
        case CMD_CA_SET_SP_HARDONLY_EN_LOCK:
        {
            ret = DRV_CA_OTP_V200_LockHardSPESel();
            break;
        }
        case CMD_CA_SET_TSKL_CSA2_DISABLE:
        {
            HI_U32 u32TsklCSA2Disable;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32TsklCSA2Disable = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetTSCWDeactive(u32TsklCSA2Disable);
            break;
        }
        case CMD_CA_SET_TSKL_CSA2_DISABLE_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetTSCWDeactiveLock();
            break;
        }
        case CMD_CA_SET_TSKL_CSA3_DISABLE:
        {
            HI_U32 u32TsklCSA3Disable;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32TsklCSA3Disable = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetTSCSA3Deactive(u32TsklCSA3Disable);
            break;
        }
        case CMD_CA_SET_TSKL_CSA3_DISABLE_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetTSCSA3DeactiveLock();
            break;
        }
         case CMD_CA_SET_TSKL_SP_DISABLE:
        {
            HI_U32 u32TsklSPDisable;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32TsklSPDisable = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetTSSPDeactive(u32TsklSPDisable);
            break;
        }
        case CMD_CA_SET_TSKL_SP_DISABLE_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetTSSPDeactiveLock();
            break;
        }
        case CMD_CA_SET_TSKL_DES_DISABLE:
        {
            HI_U32 u32TsklDesDisable;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32TsklDesDisable = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetDesDisable(u32TsklDesDisable);
            break;
        }
        case CMD_CA_SET_TSKL_DES_DISABLE_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetDesDisableLock();
            break;
        }
        case CMD_CA_SET_MCKL_DES_HARD_DISABLE:
        {
            HI_U32 u32McklDesHardDisable;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32McklDesHardDisable = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetDesHardDisable(u32McklDesHardDisable);
            break;
        }
        case CMD_CA_SET_MCKL_DES_HARD_DISABLE_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetDesHardDisableLock();
            break;
        }
        case CMD_CA_SET_BLOAD_MODE_SEL:
        {
            HI_U32 u32BootDecMode;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32BootDecMode = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetBootDecMode(u32BootDecMode);
            break;
        }
        case CMD_CA_SET_BLOAD_MODE_SEL_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetBloadModeSelLock();
            break;
        }
        case CMD_CA_SET_DDR_SCRAMBLE_EN:
        {
            HI_U32 u32DdrScrambleEn;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32DdrScrambleEn = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetDDRScrambleEn(u32DdrScrambleEn);
            break;
        }
        case CMD_CA_SET_DDR_SCRAMBLE_EN_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetDDRScrambleEnLock();
            break;
        }
        case CMD_CA_SET_OTP_GLOBAL_LOCK_EN:
        {
            HI_U32 u32OtpGlobalLockEn;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32OtpGlobalLockEn = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetOtpGlobalLockEn(u32OtpGlobalLockEn);
            break;
        }
        case CMD_CA_SET_OTP_GLOBAL_LOCK_EN_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetOtpGlobalLockEnLock();
            break;
        }
        case CMD_CA_SET_SECURE_CHIP_FLAG:
        {
            HI_U32 u32SecureChipFlag;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32SecureChipFlag = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetSecureChipFlag(u32SecureChipFlag);
            break;
        }
        case CMD_CA_SET_SECURE_CHIP_FLAG_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetSecureChipFlagLock();
            break;
        }
        case CMD_CA_SET_DCAS_KL_DISABLE:
        {
            HI_U32 u32DcasKlDisable;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32DcasKlDisable = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetDcasKLDisable(u32DcasKlDisable);
            break;
        }
        case CMD_CA_SET_DCAS_KL_DISABLE_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetDcasKLDisableLock();
            break;
        }
        case CMD_CA_SET_TS_OUT_DISABLE:
        {
            HI_U32 u32TsOutDisable;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32TsOutDisable = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetTsOutDisable(u32TsOutDisable);
            break;
        }
        case CMD_CA_SET_TS_OUT_DISABLE_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetTsOutDisableLock();
            break;
        }
        case CMD_CA_SET_LPC_DISABLE:
        {
            HI_U32 u32LpcDisable;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32LpcDisable = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetLpcEn(u32LpcDisable);
            break;
        }
        case CMD_CA_SET_LPC_DISABLE_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetLpcEnLock();
            break;
        }
        case CMD_CA_SET_LPC_MASTER_DISABLE:
        {
            HI_U32 u32LpcMasterDisable;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32LpcMasterDisable = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetLpcMasterEn(u32LpcMasterDisable);
            break;
        }
        case CMD_CA_SET_LPC_MASTER_DISABLE_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetLpcMasterEnLock();
            break;
        }
        case CMD_CA_SET_BOOTSEL_CTRL_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetBootSelCtrlLock();
            break;
        }
        case CMD_CA_SET_RUNTIME_CHECK_EN:
        {
            HI_U32 u32RuntimeCheckEn;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32RuntimeCheckEn = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetRuntimeCheckEn(u32RuntimeCheckEn);
            break;
        }
        case CMD_CA_SET_RUNTIME_CHECK_EN_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetRuntimeCheckEnLock();
            break;
        }
        case CMD_CA_SET_WAKEUP_DDR_CHECK_EN:
        {
            HI_U32 u32WakeupDdrCheckEn;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32WakeupDdrCheckEn = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetWakeupDDRCheckEn(u32WakeupDdrCheckEn);
            break;
        }
        case CMD_CA_SET_WAKEUP_DDR_CHECK_EN_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetWakeupDDRCheckEnLock();
            break;
        }
        case CMD_CA_SET_VERSION_CHECK_EN:
        {
            HI_U32 u32VersionIdCheckEn;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32VersionIdCheckEn = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetVersionIdCheckEn(u32VersionIdCheckEn);
            break;
        }
        case CMD_CA_SET_VERSION_CHECK_EN_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetVersionIdCheckEnLock();
            break;
        }
        case CMD_CA_SET_BL_MSID_CHECK_EN:
        {
            HI_U32 u32MsidCheckEn;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32MsidCheckEn = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetBlMSIDCheckEn(u32MsidCheckEn);
            break;
        }
        case CMD_CA_SET_BL_MSID_CHECK_EN_LOCK:
        {
            ret = DRV_CA_OTP_V200_SetBlMSIDCheckEnLock();
            break;
        }
        case CMD_CA_SET_JTAG_READ_DISABLE:
        {
            ret = DRV_CA_OTP_V200_SetJtagReadDisable();
            break;
        }
        case CMD_CA_SET_JTAG_WRITE_DISABLE:
        {
            ret = DRV_CA_OTP_V200_DisableJtagWrite();
            break;
        }
        case CMD_CA_SET_DEBUG_DISABLE:
        {
            ret = DRV_CA_OTP_V200_SetDebugDisable();
            break;
        }
        case CMD_CA_SET_RIGHT_CTRL_EN:
        {
            ret = DRV_CA_OTP_V200_SetRigthCtrlEn();
            break;
        }
        case CMD_CA_GET_RSA_KEY_N:
        {
            CA_KEY_S *pKey = (CA_KEY_S*)arg;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            memset(keybuf, 0x0, 256);
            ret = DRV_CA_OTP_V200_GetRSAKeyN(keybuf);
            memcpy(pKey->KeyBuf, keybuf, 256);
            break;
        }
        case CMD_CA_GET_RSA_KEY_E:
        {
            CA_KEY_S *pKey = (CA_KEY_S*)arg;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            memset(keybuf, 0x0, 16);
            ret = DRV_CA_OTP_V200_GetRSAKeyE(keybuf);
            memcpy(pKey->KeyBuf, keybuf, 16);
            break;
        }
        case CMD_CA_GET_RSA_KEY_LOCK:
        {
            HI_U32 *pbLock = (HI_U32*)arg;

            if (HI_NULL == pbLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetRSAKeyLock(pbLock);
            break;
        }
        case CMD_CA_GET_SECRET_KEY_LOCK:
        {
            HI_U32 *pbLock = (HI_U32*)arg;

            if (HI_NULL == pbLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetRootKeyLock(pbLock);

            break;
        }
        case CMD_CA_GET_CHIPID_LOCK:
        {
            HI_U32 *pbLock = (HI_U32*)arg;

            if (HI_NULL == pbLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetChipIdLock(pbLock);

            break;
        }
        case CMD_CA_GET_STBSN_LOCK:
        {
            HI_U32 *pu32StbSnP = (HI_U32*)arg;

            if (HI_NULL == pu32StbSnP)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetStbSnLock_0(pu32StbSnP);
            break;
        }
        case CMD_CA_GET_MKTID_LOCK:
        {
            HI_U32 *pu32MKTKDLock = (HI_U32*)arg;

            if (HI_NULL == pu32MKTKDLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetMktIdLock(pu32MKTKDLock);
            break;
        }
        case CMD_CA_GET_VENDOR_ID:
        {
            CA_OTP_VENDOR_TYPE_E *penVendorId = (CA_OTP_VENDOR_TYPE_E *)arg;

            if (HI_NULL == penVendorId)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetVendorId(penVendorId);
            break;
        }
        case CMD_CA_GET_VENDOR_ID_LOCK:
        {
            HI_U32 *pu32VendorIdLock = (HI_U32*)arg;

            if (HI_NULL == pu32VendorIdLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetVendorIdLock(pu32VendorIdLock);
            break;
        }
        case CMD_CA_GET_VERSION_ID_LOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetVersionIdCheckLock(pu32Lock);
            break;
        }
        case CMD_CA_GET_BOOT_SEL_LOCK_0:
        {
            HI_U32 *pu32BootSelLock = (HI_U32*)arg;

            if (HI_NULL == pu32BootSelLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetBootSelLock_0(pu32BootSelLock);
            break;
        }
        case CMD_CA_GET_BOOT_SEL_LOCK_1:
        {
            HI_U32 *pu32BootSelLock = (HI_U32*)arg;

            if (HI_NULL == pu32BootSelLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetBootSelLock_1(pu32BootSelLock);
            break;
        }
        case CMD_CA_GET_BLOAD_DEC_EN:
        {
            HI_U32 *pu32BloadDecEn = (HI_U32*)arg;

            if (HI_NULL == pu32BloadDecEn)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetBloadDecEn(pu32BloadDecEn);
            break;
        }
        case CMD_CA_GET_BLOAD_DEC_EN_LOCK:
        {
            HI_U32 *pu32BloadDecEnLock = (HI_U32*)arg;

            if (HI_NULL == pu32BloadDecEnLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetBloadDecEnLock(pu32BloadDecEnLock);
            break;
        }

        case CMD_CA_GET_SELFBOOT:
        {
            HI_U32 *pu32SelfBoot = (HI_U32*)arg;

            if (HI_NULL == pu32SelfBoot)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetSelfBoot(pu32SelfBoot);
            break;
        }
        case CMD_CA_GET_SELFBOOT_LOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetSelfbootLock(pu32Lock);
            break;
        }
        case CMD_CA_GET_LOWPOWER_DISABLE:
        {
            HI_U32 *pu32LowPowerDisable = (HI_U32*)arg;

            if (HI_NULL == pu32LowPowerDisable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetLowPowerDisable(pu32LowPowerDisable);
            break;
        }
        case CMD_CA_GET_LOWPOWER_DISABLE_LOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetLowPowerDisableLock(pu32Lock);
            break;
        }
        case CMD_CA_DVB_GETLADDERLOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetDVBLadderLock(pu32Lock);
            break;
        }
        case CMD_CA_CSA3_GETLADDER:
        {
            HI_UNF_ADVCA_KEYLADDER_LEV_E *pCSA3Ladder = (HI_UNF_ADVCA_KEYLADDER_LEV_E*)arg;

            if (HI_NULL == pCSA3Ladder)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_GetCSA3LadderLevel(pCSA3Ladder);
            break;
        }
        case CMD_CA_CSA3_GETLADDERLOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetCSA3LadderLock(pu32Lock);
            break;
        }
        case CMD_CA_R2R_GETLADDERLOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetR2RLadderLock(pu32Lock);
            break;
        }
        case CMD_CA_SP_GETLADDERLOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetSPLadderLock(pu32Lock);
            break;
        }
        case CMD_CA_GET_LINK_PRT_DISABLE_LOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetLinkProtectDisableLock(pu32Lock);
            break;
        }
        case CMD_CA_GET_CSA2_HARDONLY_EN:
        {
            HI_U32 *pu32Csa2HardonlyEn = (HI_U32*)arg;

            if (HI_NULL == pu32Csa2HardonlyEn)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTSCsa2HardonlyEn(pu32Csa2HardonlyEn);
            break;
        }
        case CMD_CA_GET_CSA2_HARDONLY_EN_LOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTSCsa2HardonlyEnLock(pu32Lock);
            break;
        }
        case CMD_CA_GET_CSA3_HARDONLY_EN:
        {
            HI_U32 *pu32Csa3HardonlyEn = (HI_U32*)arg;

            if (HI_NULL == pu32Csa3HardonlyEn)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTSCSA3HardonlyEn(pu32Csa3HardonlyEn);
            break;
        }
        case CMD_CA_GET_CSA3_HARDONLY_EN_LOCK:
        {
            HI_U32 *pu32Csa3HardonlyEnLock = (HI_U32*)arg;

            if (HI_NULL == pu32Csa3HardonlyEnLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTSCSA3HardonlyEnLock(pu32Csa3HardonlyEnLock);
            break;
        }
        case CMD_CA_GET_SP_HARDONLY_EN:
        {
            HI_U32 *pu32SpHardonlyEn = (HI_U32*)arg;

            if (HI_NULL == pu32SpHardonlyEn)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTSSPHardonlyEn(pu32SpHardonlyEn);
            break;
        }
        case CMD_CA_GET_SP_HARDONLY_EN_LOCK:
        {
            HI_U32 *pu32SPHardonlyEnLock = (HI_U32*)arg;

            if (HI_NULL == pu32SPHardonlyEnLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTSSPHardonlyEnLock(pu32SPHardonlyEnLock);
            break;
        }
        case CMD_CA_GET_TSKL_CSA2_DISABLE:
        {
            HI_U32 *pCsa2Disable = (HI_U32*)arg;

            if (HI_NULL == pCsa2Disable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTSCWDeactive(pCsa2Disable);
            break;
        }
        case CMD_CA_GET_TSKL_CSA2_DISABLE_LOCK:
        {
            HI_U32 *pCsa2DisableLock = (HI_U32*)arg;

            if (HI_NULL == pCsa2DisableLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTSCWDeactiveLock(pCsa2DisableLock);
            break;
        }
        case CMD_CA_GET_TSKL_CSA3_DISABLE:
        {
            HI_U32 *pCsa3Disable = (HI_U32*)arg;

            if (HI_NULL == pCsa3Disable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTSCSA3Deactive(pCsa3Disable);
            break;
        }
        case CMD_CA_GET_TSKL_CSA3_DISABLE_LOCK:
        {
            HI_U32 *pCsa3DisableLock = (HI_U32*)arg;

            if (HI_NULL == pCsa3DisableLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTSCSA3DeactiveLock(pCsa3DisableLock);
            break;
        }
        case CMD_CA_GET_TSKL_SP_DISABLE:
        {
            HI_U32 *pu32SPDisable = (HI_U32*)arg;

            if (HI_NULL == pu32SPDisable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTSSPDeactive(pu32SPDisable);
            break;
        }
        case CMD_CA_GET_TSKL_SP_DISABLE_LOCK:
        {
            HI_U32 *pu32SPDisableLock = (HI_U32*)arg;

            if (HI_NULL == pu32SPDisableLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTSSPDeactiveLock(pu32SPDisableLock);
            break;
        }
        case CMD_CA_GET_TSKL_DES_DISABLE:
        {
            HI_U32 *pTsklDesDisable = (HI_U32*)arg;

            if (HI_NULL == pTsklDesDisable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetDesDisable(pTsklDesDisable);
            break;
        }
        case CMD_CA_GET_TSKL_DES_DISABLE_LOCK:
        {
            HI_U32 *pTsklDesDisableLock = (HI_U32*)arg;

            if (HI_NULL == pTsklDesDisableLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetDesDisableLock(pTsklDesDisableLock);
            break;
        }
        case CMD_CA_GET_MCKL_DES_HARD_DISABLE:
        {
            HI_U32 *pMcklDesHardOnlyEn = (HI_U32*)arg;

            if (HI_NULL == pMcklDesHardOnlyEn)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetDesHardDisable(pMcklDesHardOnlyEn);
            break;
        }
        case CMD_CA_GET_MCKL_DES_HARD_DISABLE_LOCK:
        {
            HI_U32 *pMcklDesHardOnlyEnLock = (HI_U32*)arg;

            if (HI_NULL == pMcklDesHardOnlyEnLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetDesHardDisableLock(pMcklDesHardOnlyEnLock);
            break;
        }
        case CMD_CA_GET_BLOAD_MODE_SEL:
        {
            HI_U32 *pu32BloadModeSel = (HI_U32*)arg;

            if (HI_NULL == pu32BloadModeSel)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetBloadModeSel(pu32BloadModeSel);
            break;
        }
        case CMD_CA_GET_BLOAD_MODE_SEL_LOCK:
        {
            HI_U32 *pu32BloadModeSelLock = (HI_U32*)arg;

            if (HI_NULL == pu32BloadModeSelLock)
            {
                HI_ERR_CA("invalid arg\n");
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetBloadModeSelLock(pu32BloadModeSelLock);
            break;
        }
        case CMD_CA_GET_DDR_SCRAMBLE_EN:
        {
            HI_U32 *pDdrScrambleEn = (HI_U32*)arg;

            if (HI_NULL == pDdrScrambleEn)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetDDRScrambleEn(pDdrScrambleEn);
            break;
        }
        case CMD_CA_GET_DDR_SCRAMBLE_EN_LOCK:
        {
            HI_U32 *pDdrScrambleEnLock = (HI_U32*)arg;

            if (HI_NULL == pDdrScrambleEnLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetDDRScrambleEnLock(pDdrScrambleEnLock);
            break;
        }
        case CMD_CA_GET_OTP_GLOBAL_LOCK_EN:
        {
            HI_U32 *pOtpGlobalLockEn = (HI_U32*)arg;

            if (HI_NULL == pOtpGlobalLockEn)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetOtpGlobalLockEn(pOtpGlobalLockEn);
            break;
        }
        case CMD_CA_GET_OTP_GLOBAL_LOCK_EN_LOCK:
        {
            HI_U32 *pOtpGlobalLockEnLock = (HI_U32*)arg;

            if (HI_NULL == pOtpGlobalLockEnLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetOtpGlobalLockEnLock(pOtpGlobalLockEnLock);
            break;
        }
        case CMD_CA_GET_SECURE_CHIP_FLAG:
        {
            HI_U32 *pu32SecureChipFlag = (HI_U32*)arg;

            if (HI_NULL == pu32SecureChipFlag)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetSecureChipFlag(pu32SecureChipFlag);
            break;
        }
        case CMD_CA_GET_SECURE_CHIP_FLAG_LOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetSecureChipFlagLock(pu32Lock);
            break;
        }
        case CMD_CA_GET_DCAS_KL_DISABLE:
        {
            HI_U32 *pDCASKlDisable = (HI_U32*)arg;

            if (HI_NULL == pDCASKlDisable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetDcasKLDisable(pDCASKlDisable);
            break;
        }
        case CMD_CA_GET_DCAS_KL_DISABLE_LOCK:
        {
            HI_U32 *pDCASKlDisableLock = (HI_U32*)arg;

            if (HI_NULL == pDCASKlDisableLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetDcasKLDisableLock(pDCASKlDisableLock);
            break;
        }
        case CMD_CA_GET_TS_OUT_DISABLE:
        {
            HI_U32 *pu32Disable = (HI_U32*)arg;

            if (HI_NULL == pu32Disable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTsOutDisable(pu32Disable);
            break;
        }
        case CMD_CA_GET_TS_OUT_DISABLE_LOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock )
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetTsOutDisableLock(pu32Lock);
            break;
        }
        case CMD_CA_GET_LPC_DISABLE:
        {
            HI_U32 *pu32LpcEn = (HI_U32*)arg;

            if (HI_NULL == pu32LpcEn)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetLpcEn(pu32LpcEn);
            break;
        }
        case CMD_CA_GET_LPC_DISABLE_LOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetLpcEnLock(pu32Lock);
            break;
        }
        case CMD_CA_GET_LPC_MASTER_DISABLE:
        {
            HI_U32 *pu32LpcMasterEn = (HI_U32*)arg;

            if (HI_NULL == pu32LpcMasterEn)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetLpcMasterEn(pu32LpcMasterEn);
            break;
        }
        case CMD_CA_GET_LPC_MASTER_DISABLE_LOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetLpcMasterEnLock(pu32Lock);
            break;
        }
        case CMD_CA_GET_BOOTSEL_CTRL:
        {
            HI_U32 *pu32BootSel = (HI_U32*)arg;

            if (HI_NULL == pu32BootSel)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetBootSelCtrl(pu32BootSel);
            break;
        }
        case CMD_CA_GET_BOOTSEL_CTRL_LOCK:
        {
            HI_U32 *pu32Lock = (HI_U32*)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetBootSelCtrlLock(pu32Lock);
            break;
        }
        case CMD_CA_GET_RUNTIME_CHECK_EN:
        {
            HI_U32 *pu32RuntimeCheckEn = (HI_U32 *)arg;

            if (HI_NULL == pu32RuntimeCheckEn)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetRuntimeCheckEn(pu32RuntimeCheckEn);
            break;
        }
        case CMD_CA_GET_RUNTIME_CHECK_EN_LOCK:
        {
            HI_U32 *pu32RuntimeCheckEnLock = (HI_U32 *)arg;

            if (HI_NULL == pu32RuntimeCheckEnLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetRuntimeCheckEnLock(pu32RuntimeCheckEnLock);
            break;
        }
        case CMD_CA_GET_WAKEUP_DDR_CHECK_EN:
        {
            HI_U32 *pu32WakeupDdrCheckEn = (HI_U32 *)arg;

            if (HI_NULL == pu32WakeupDdrCheckEn)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetWakeupDDRCheckEn(pu32WakeupDdrCheckEn);
            break;
        }
        case CMD_CA_GET_WAKEUP_DDR_CHECK_EN_LOCK:
        {
            HI_U32 *pu32WakeipDdrCheckEnLock = (HI_U32 *)arg;

            if (HI_NULL == pu32WakeipDdrCheckEnLock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetWakeupDDRCheckEnLock(pu32WakeipDdrCheckEnLock);
            break;
        }
        case CMD_CA_GET_VERSION_CHECK_EN:
        {
            HI_U32 *pu32VersionIDCheckEn = (HI_U32 *)arg;

            if (HI_NULL == pu32VersionIDCheckEn)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetVersionIDCheckEn(pu32VersionIDCheckEn);
            break;
        }
        case CMD_CA_GET_VERSION_CHECK_EN_LOCK:
        {
            HI_U32 *pu32Lock = (HI_U32 *)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetVersionIDCheckEnLock(pu32Lock);
            break;
        }
        case CMD_CA_GET_BL_MSID_CHECK_EN:
        {
            HI_U32 *pu32BlMSIDCheckEn = (HI_U32 *)arg;

            if (HI_NULL == pu32BlMSIDCheckEn)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetBlMSIDCheckEn(pu32BlMSIDCheckEn);
            break;
        }
        case CMD_CA_GET_BL_MSID_CHECK_EN_LOCK:
        {
            HI_U32 *pu32Lock = (HI_U32 *)arg;

            if (HI_NULL == pu32Lock)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetBlMSIDCheckEnLock(pu32Lock);
            break;
        }
        case CMD_CA_GET_JTAG_READ_DISABLE:
        {
            HI_U32 *pu32Disable = (HI_U32*)arg;

            if (HI_NULL == pu32Disable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetJtagReadDisable(pu32Disable);
            break;
        }
        case CMD_CA_GET_JTAG_WRITE_DISABLE:
        {
            HI_U32 *pu32Disable = (HI_U32*)arg;

            if (HI_NULL == pu32Disable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetJtagWriteDisable(pu32Disable);
            break;
        }
        case CMD_CA_GET_DEBUG_DISABLE:
        {
            HI_U32 *pu32Disable = (HI_U32*)arg;

            if (HI_NULL == pu32Disable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetDebugDisable(pu32Disable);
            break;
        }
        case CMD_CA_GET_RIGHT_CTRL_EN:
        {
            HI_U32 *pu32RightCtrl = (HI_U32*)arg;

            if (HI_NULL == pu32RightCtrl)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetRigthCtrlEn(pu32RightCtrl);
            break;
        }
        case CMD_CA_GET_LINK_PRT_DISABLE:
        {
            HI_U32 *pu32LinkPrtDisable = (HI_U32*)arg;

            if (HI_NULL == pu32LinkPrtDisable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetLinkProtectDisable(pu32LinkPrtDisable);
            break;
        }
        case CMD_CA_SET_PROTECT:
        {
            ret = HI_ERR_CA_NOT_SUPPORT;
            break;
        }
        case CMD_CA_GET_PROTECT:
        {
            ret = HI_ERR_CA_NOT_SUPPORT;
            break;
        }
        case CMD_CA_SET_BOOTSEL_CTRL:
        {
            HI_U32 u32BootSel ;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            u32BootSel = *(HI_U32*)arg;
            ret = DRV_CA_OTP_V200_SetBootSelCtrl(u32BootSel);
            break;
        }
        case CMD_CA_CHECK_MARKET_ID_SET:
        {
            HI_U32 u32MarketIdSet;
            HI_BOOL *pbIsMarketIdSet = (HI_BOOL*)arg;

            if (HI_NULL == pbIsMarketIdSet)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetMktIdLock(&u32MarketIdSet);
            if (ret)
            {
                ret = HI_FAILURE;
            }
            else
            {
                ret = HI_SUCCESS;
            }
            *pbIsMarketIdSet = (HI_BOOL)u32MarketIdSet;
            break;
        }
        case CMD_CA_EXT1_SETRSAKEY:
        {
            /*The RSA key should be converted to N and E*/
            CA_KEY_S *pKey = (CA_KEY_S*)arg;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            /*The first 256 Bytes are the N value of RSA key*/
            memset(keybuf, 0x0, 512);
            memcpy(keybuf, pKey->KeyBuf, 256);
            ret = DRV_CA_OTP_V200_SetRSAKeyN(keybuf);
            if(HI_SUCCESS != ret)
            {
                HI_ERR_CA("Set rsa key N failed!\n");
                break;
            }
            /*The last 16 Bytes are the E value of RSA key*/
            memset(keybuf, 0x0, 32);
            memcpy(keybuf, (pKey->KeyBuf + 496), 16);
            ret |= DRV_CA_OTP_V200_SetRSAKeyE(keybuf);
            if(HI_SUCCESS != ret)
            {
                HI_ERR_CA("Set rsa key E failed!\n");
            }
#if defined (CHIP_TYPE_hi3716mv410) || defined (CHIP_TYPE_hi3716mv420) || defined(CHIP_TYPE_hi3798cv200)  || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
            /*Set rsa key crc*/
            ret = DRV_CA_OTP_V200_SetRSACRC(pKey->KeyBuf);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_CA("Set rsa key crc failed!\n");
            }
#endif
            break;
        }
        case CMD_CA_EXT1_GETRSAKEY:
        {
            /*The RSA key should be converted to N and E*/
            CA_KEY_S *pKey = (CA_KEY_S*)arg;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            /*The first 256 Bytes are the N value of RSA key*/
            memset(pKey->KeyBuf, 0x0, 512);
            ret = DRV_CA_OTP_V200_GetRSAKeyN(keybuf);
            memcpy(pKey->KeyBuf, keybuf, 256);

            /*The last 16 Bytes are the E value of RSA key*/
            memset(keybuf, 0x0, 512);
            ret |= DRV_CA_OTP_V200_GetRSAKeyE(keybuf);//Only read 16 bytes data
            memcpy((pKey->KeyBuf + 496), keybuf, 16);
            break;
        }
        case CMD_CA_DCAS_OPEN:
        {
            HI_UNF_CIPHER_ALG_E enAlg ;

            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            enAlg = *(HI_UNF_CIPHER_ALG_E*)arg;
            ret = HAL_ADVCA_V300_DCASOpen(enAlg);
            break;
        }
        case CMD_CA_DCAS_CLOSE:
        {
            ret = HAL_ADVCA_V300_DCASClose();
            break;
        }
        case CMD_CA_DCAS_PARAM_ID_SET:
        {
            CA_DCAS_PARAM_S *pDCASParam = (CA_DCAS_PARAM_S *)arg;

            if (HI_NULL == pDCASParam)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_DecryptDCAS(pDCASParam->level, pDCASParam->pDin, 0, 0, pDCASParam->pDout, 0);
            break;
        }
        case CMD_CA_SWPK_KEY_LADDER_OPEN:
        {
            ret = HAL_ADVCA_V300_SWPKKeyLadderOpen();
            break;
        }
        case CMD_CA_SWPK_KEY_LADDER_CLOSE:
        {
            ret = HAL_ADVCA_V300_SWPKKeyLadderClose();
            break;
        }
        case CMD_CA_LOCKSECRETKEY:
        {
            ret = DRV_CA_OTP_V200_SetRootKeyLock();
            ret |= DRV_CA_OTP_V200_SetChipIdLock();
            ret |= DRV_CA_OTP_V200_LockRSAKey();
            break;
        }
        case CMD_CA_GET_CHIP_VERSION:
        {
            HI_CHAR *pChipVersion = (HI_CHAR*)arg;
            HI_U32 u32Version = 0;
            HI_U8 *pu8VirAddr = HI_NULL;

            if (HI_NULL == pChipVersion)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            pu8VirAddr= (HI_U8*)ioremap_nocache(CA_V300_CA_VERSION,0x10);
            if(HI_NULL == pu8VirAddr)
            {
                HI_ERR_OTP("ioremap_nocache map error\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            CA_readReg(pu8VirAddr, &u32Version);
            iounmap(pu8VirAddr);
            snprintf(pChipVersion, sizeof(HI_U32), "%x", (u32Version & 0xff));
            break;
        }
        case CMD_CA_SET_CSA3_EN:
        {
            ret = DRV_CA_OTP_V200_SetCsa3En();
            break;
        }
        case CMD_CA_GET_CSA3_EN:
        {
            HI_U32 *pu32Csa3En = (HI_U32*)arg;

            if (HI_NULL == pu32Csa3En)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetCsa3En(pu32Csa3En);
            break;
        }
        case CMD_CA_SET_LINK_PRT_DISABLE:
        {
            HI_U32 *pu32Disable = (HI_U32*)arg;

            if (HI_NULL == pu32Disable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_SetLinkProtectDisable(*pu32Disable);
            break;
        }
        case CMD_CA_SET_BLOAD_ENC_EN:
        {
            HI_U32 *pu32Disable = (HI_U32*)arg;

            if (HI_NULL == pu32Disable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_SetBloadEncDisable(*pu32Disable);
            break;
        }
        case CMD_CA_SET_CSA2_HARDONLY_EN:
        {
            HI_U32 *pu32Enable = (HI_U32*)arg;

            if (HI_NULL == pu32Enable)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_SetTSCsa2HardonlyEn(*pu32Enable);
            break;
        }
        case CMD_CA_EXT0_SETCWDEACTIVE:
        {
            ret = DRV_CA_OTP_V200_SetCSA2Deactive();
            break;
        }
        case CMD_CA_EXT0_GETCWDEACTIVE:
        {
            HI_U32 *pCsa2Deactive = (HI_U32*)arg;

            if (HI_NULL == pCsa2Deactive)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetCSA2Deactive(pCsa2Deactive);
            break;
        }
        case CMD_CA_EXT0_SETR2RDEACTIVE:
        {
            ret = DRV_CA_OTP_V200_SetR2RDeactive();
            break;
        }
        case CMD_CA_EXT0_GETR2RDEACTIVE:
        {
            HI_U32 *pR2RDeactive = (HI_U32*)arg;

            if (HI_NULL == pR2RDeactive)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetR2RDeactive(pR2RDeactive);
            break;
        }
        case CMD_CA_EXT0_SETCSA3DEACTIVE:
        {
            ret = DRV_CA_OTP_V200_SetCSA3Deactive();
            break;
        }
        case CMD_CA_EXT0_GETCSA3DEACTIVE:
        {
            HI_U32 *pR2RDeactive = (HI_U32*)arg;

            if (HI_NULL == pR2RDeactive)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetCSA3Deactive(pR2RDeactive);
            break;
        }
        case CMD_CA_EXT0_SETSPDEACTIVE:
        {
            ret = DRV_CA_OTP_V200_SetSPDeactive();
            break;
        }
        case CMD_CA_EXT0_GETSPDEACTIVE:
        {
            HI_U32 *pSPDeactive = (HI_U32*)arg;

            if (HI_NULL == pSPDeactive)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetSPDeactive(pSPDeactive);
            break;
        }
        case CMD_CA_OTP_RESET:
        {
            ret = HAL_ADVCA_V300_ResetOTP();
            break;
        }
        case CMD_CA_GET_REVISION:
        {
            CA_KEY_S *pKey = (CA_KEY_S*)arg;
            HI_CHAR *str = (CA_OTP_VENDOR_NAGRA == u32VendorId) ? NAGRA_REVISION : ADVCA_REVISION;

            if (HI_NULL == pKey)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            strncpy(pKey->KeyBuf, str, sizeof(str) < sizeof(pKey->KeyBuf) ? sizeof(str) : sizeof(pKey->KeyBuf) - 1);

            break;
        }
        case CMD_CA_SUPPER_ID:
        {
            CA_CMD_SUPPER_ID_S *pstCmdParam = (CA_CMD_SUPPER_ID_S *)arg;

            if (HI_NULL == pstCmdParam)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_ADVCA_V300_Ioctl_Child(pstCmdParam);
            break;
        }
        case CMD_CA_EXT1_OTPREAD:
        {
            OTP_ENTRY_S *pOtpEntry = (OTP_ENTRY_S *)arg;

            if (HI_NULL == pOtpEntry)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            pOtpEntry->Value = DRV_CA_OTP_V200_Read(pOtpEntry->Addr);
            break;
        }
        case CMD_CA_EXT1_OTPWRITEBYTE:
        {
            OTP_ENTRY_S *pOtpEntry = (OTP_ENTRY_S *)arg;

            if (HI_NULL == pOtpEntry)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_WriteByte(pOtpEntry->Addr,pOtpEntry->Value);
            break;
        }
        case CMD_CA_SET_OTP_FUSE:
        {
            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_SetOtpFuse((CA_OTP_ATTR_S*)arg);
            break;
        }
        case CMD_CA_GET_OTP_FUSE:
        {
            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = DRV_CA_OTP_V200_GetOtpFuse((CA_OTP_ATTR_S*)arg);
            break;
        }
        case CMD_CA_SET_KEYLADDER_ATTR:
        {
            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_SetKeyladderAttr((CA_KEYLADDER_ATTR_S*)arg);
            break;
        }
        case CMD_CA_SET_CAVENDOR_OPT:
        {
            if (HI_NULL == arg)
            {
                HI_ERR_CA("invalid arg\n");
                ca_up(g_pDrvSema);
                return HI_FAILURE;
            }
            ret = HAL_ADVCA_V300_SetCavendorOpt((CA_VENDOR_OPT_S*)arg);
            break;
        }

        default:
        {
            break;
        }
    }

    ca_up(g_pDrvSema);

    if (ret == HI_SUCCESS)
    {
        HI_INFO_CA("cmd = 0x%08x  ok !\n", cmd);
    }

    return ret;
}

int DRV_ADVCA_V300_Open(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Value = 0;

    s32Ret = ca_down_interruptible(g_pDrvSema);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CA("%s:  down_interruptible err ! \n", __FUNCTION__);
        return HI_FAILURE;
    }

    s32Ret = ca_atomic_inc_return(&u32Value);
    if (s32Ret != HI_SUCCESS)
    {
        ca_up(g_pDrvSema);
        return HI_FAILURE;
    }

    ca_up(g_pDrvSema);

    return 0;
}

int DRV_ADVCA_V300_Release(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Value = 0;

    s32Ret = ca_down_interruptible(g_pDrvSema);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CA("%s:  down_interruptible err ! \n", __FUNCTION__);
        return HI_FAILURE;
    }

    s32Ret = ca_atomic_dec_return(&u32Value);
    if (s32Ret != HI_SUCCESS)
    {
        ca_up(g_pDrvSema);
        return HI_FAILURE;
    }

    if (0 == u32Value)
    {
        HI_INFO_CA("%s:  ok ! \n", __FUNCTION__);
    }

    ca_up(g_pDrvSema);

    return 0;
}

HI_S32  DRV_ADVCA_V300_ModeInit_0(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;

    DRV_ADVCA_SystemSetClock();
    DRV_ADVCA_SystemSetReset();
    DRV_ADVCA_RegisterMap();

    g_pDrvSema = ca_create_mutex();
    if (HI_NULL == g_pDrvSema)
    {
        HI_ERR_CA("%s:  fail to create ca mutex! \n", __FUNCTION__);
        return HI_FAILURE;
    }

    ca_initMutex(g_pDrvSema);

    ret = HAL_ADVCA_V300_Open();
    if (ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

#if   (defined(CHIP_TYPE_hi3798cv200) && !defined(HI_TEE_SUPPORT)) \
   || (defined(CHIP_TYPE_hi3798mv200) && !defined(HI_TEE_SUPPORT)) \
   || (defined(CHIP_TYPE_hi3798mv200_a) && !defined(HI_TEE_SUPPORT))
    //Set keyladder to secure keyladder
    DRV_ADVCA_WriteReg(CA_V300_KL_SEC_EN, CA_V300_KL_EN_MASK);
#endif

    return ret;
}

HI_S32  DRV_ADVCA_V300_ModeExit_0(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;

    DRV_ADVCA_RegisterUnMap();

    return ret;
}

/*--------------------------------------------END-------------------------------------*/

