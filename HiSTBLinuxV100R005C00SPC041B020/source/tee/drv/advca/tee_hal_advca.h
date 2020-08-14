/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : tee_hal_advca.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#ifndef __TEE_HAL_ADVCA_H__
#define __TEE_HAL_ADVCA_H__

#include "hi_type.h"
#include "hi_tee_cipher.h"
#include "hi_tee_advca.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

typedef struct
{
    HI_BOOL                      OpenFlag;  //Flag for Keyladder usage
    HI_TEE_ADVCA_ALG_TYPE_E      Alg;       //KeyLadder algorithm
    HI_TEE_ADVCA_SP_DSC_MODE_E   enDscMode;
    HI_TEE_ADVCA_KEYLADDER_LEV_E MaxLevel;  //Max level
    HI_U32                       Keyladder_Ready;     //Max level
    HI_U8                        SessionKey[16][32];
}TEE_ADVCA_KEYLADDER_INFO_S;

extern HI_TEE_ADVCA_ALG_TYPE_E g_R2RAlgType;
extern HI_TEE_ADVCA_KEYLADDER_LEV_E g_R2RLadderSel;

extern TEE_ADVCA_KEYLADDER_INFO_S g_SPInfo;
extern TEE_ADVCA_KEYLADDER_INFO_S g_MiscInfo;
extern TEE_ADVCA_KEYLADDER_INFO_S g_DcasInfo;

HI_S32 HAL_ADVCA_Open();

HI_S32 HAL_ADVCA_Close();

HI_S32 HAL_ADVCA_Suspend(HI_VOID);

HI_S32 HAL_ADVCA_Resume(HI_VOID);

HI_S32 HAL_ADVCA_GetR2RLadderLevel(OTP_KEY_LEVEL_E *pLevel);

HI_S32 HAL_ADVCA_CryptR2R(HI_TEE_ADVCA_KEYLADDER_LEV_E enLevel,HI_U32 *pu32DataIn,HI_U32 AddrID,HI_BOOL bIsDeCrypt);

HI_S32 HAL_ADVCA_DecryptSP(HI_TEE_ADVCA_KEYLADDER_LEV_E enLevel, HI_U32 *pu32DataIn, HI_U32 AddrID, HI_BOOL bEvenOrOdd, DRV_ADVCA_CA_TARGET_E enCwTarget);

HI_S32 HAL_ADVCA_DecryptMisc(HI_TEE_ADVCA_KEYLADDER_LEV_E enLevel, HI_U32 *pu32DataIn, HI_U32 AddrID, HI_BOOL bEvenOrOdd, DRV_ADVCA_CA_TARGET_E enCwTarget);

HI_S32 HAL_ADVCA_GetSPLadderLevel(HI_TEE_ADVCA_KEYLADDER_LEV_E *pLevel);

HI_S32 HAL_ADVCA_GetMISCLadderLevel(HI_TEE_ADVCA_KEYLADDER_LEV_E *pLevel);

HI_S32 HAL_ADVCA_SetSPWaterMark(HI_U32 *pU32WaterMark);

HI_S32 HAL_ADVCA_SetMiscWaterMark(HI_U32 *pU32WaterMark);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif   /* _TEE_HAL_ADVCA_H__ */

