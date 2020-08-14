/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hal_advca_v300.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :

******************************************************************************/
#ifndef __HAL_ADVCA_V300_H__
#define __HAL_ADVCA_V300_H__

#include "drv_advca_external.h"

typedef struct
{
    HI_BOOL                      OpenFlag;  //Flag for Keyladder usage
    HI_UNF_ADVCA_ALG_TYPE_E      Alg;       //KeyLadder algorithm
    HI_UNF_ADVCA_SP_DSC_MODE_E   enDscMode;
    HI_UNF_ADVCA_KEYLADDER_LEV_E MaxLevel;  //Max level
    HI_U32                       Keyladder_Ready;     //Max level
    HI_U8                        SessionKey[16][32];
    HI_U8                        GG_Flag[4];
    HI_U32                       SessionKey_sec[16];
    HI_U8                        LastEvenKey[16];
    HI_U32                       LastEvenKey_secTime;
    HI_U8                        LastOddKey[16];
    HI_U32                       LastOddKey_secTime;
}CA_KEYLADDER_INFO_S;

extern CA_KEYLADDER_INFO_S g_CSA2Info;
extern CA_KEYLADDER_INFO_S g_CSA3Info;
extern CA_KEYLADDER_INFO_S g_R2RInfo;
extern CA_KEYLADDER_INFO_S g_SPInfo;
extern CA_KEYLADDER_INFO_S g_MiscInfo;
extern CA_KEYLADDER_INFO_S g_DCASInfo;
extern CA_KEYLADDER_INFO_S g_TAInfo;
extern CA_KEYLADDER_INFO_S g_GDRMInfo;

HI_VOID DRV_ADVCA_SystemSetClock(HI_VOID);
HI_VOID DRV_ADVCA_SystemSetReset(HI_VOID);

HI_U32 HAL_ADVCA_V300_Open(HI_VOID);

HI_S32 HAL_ADVCA_V300_StatHardCwSel(HI_U32 *pu32Lock);
HI_S32 HAL_ADVCA_V300_StatR2RHardKey(HI_U32 *pu32Lock);  
HI_S32 HAL_ADVCA_V300_StatTdesLock(HI_U32 *pu32Lock);
HI_S32 HAL_ADVCA_V300_SetCWLadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E enSel);
HI_S32 HAL_ADVCA_V300_SetCSA3LadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E enSsel);
HI_S32 HAL_ADVCA_V300_SetR2RLadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E enSel);
HI_S32 HAL_ADVCA_V300_SetSPLadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E enSel);
HI_S32 HAL_ADVCA_V300_SetCWAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E enType);
HI_S32 HAL_ADVCA_V300_GetCWAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E *penType);
HI_S32 HAL_ADVCA_V300_SetCSA3Algorithm(HI_UNF_ADVCA_ALG_TYPE_E enType);
HI_S32 HAL_ADVCA_V300_GetCSA3Algorithm(HI_UNF_ADVCA_ALG_TYPE_E *pentype);
HI_S32 HAL_ADVCA_V300_SetR2RAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E enType);
HI_S32 HAL_ADVCA_V300_GetR2RAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E *enType);
HI_S32 HAL_ADVCA_V300_SetSPAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E enType);
HI_S32 HAL_ADVCA_V300_GetSPAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E *penType);
HI_S32 HAL_ADVCA_V300_SetMiscAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E enType);
HI_S32 HAL_ADVCA_V300_GetMiscAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E *penType);
HI_S32 HAL_ADVCA_V300_DecryptLpk(HI_U32 *pEncryptLpk);
HI_S32 HAL_ADVCA_V300_DecryptLPKProtectData(HI_U32 *pEncryptData,HI_U32 *pClearData);
HI_S32 HAL_ADVCA_V300_EncryptSwpk(HI_U32 *pClearSwpk,HI_U32 *pEncryptSwpk);
HI_S32 HAL_ADVCA_V300_DCASOpen(HI_UNF_CIPHER_ALG_E enAlg);
HI_S32 HAL_ADVCA_V300_DCASClose(void);
HI_S32 HAL_ADVCA_V300_DecryptDCAS(HI_U32 enLevel,HI_U32 *pu32DataIn,HI_U32 AddrID,HI_BOOL bEvenOrOdd,HI_U32 *pDataOut, HI_UNF_ADVCA_CA_TARGET_E enKlTarget);
HI_S32 HAL_ADVCA_V300_SWPKKeyLadderOpen(void);
HI_S32 HAL_ADVCA_V300_SWPKKeyLadderClose(void);
HI_S32 HAL_ADVCA_V300_ResetOTP(void);
HI_S32 HAL_ADVCA_V300_SetSPDscMode(HI_UNF_ADVCA_SP_DSC_MODE_E eDscMode);
HI_S32 HAL_ADVCA_V300_GetSPDscMode(HI_UNF_ADVCA_SP_DSC_MODE_E *pu32Type);
HI_S32 HAL_ADVCA_V300_SetKeyladderAttr(CA_KEYLADDER_ATTR_S *pstKeyladderAttr);
HI_S32 HAL_ADVCA_V300_SetCavendorOpt(CA_VENDOR_OPT_S *pstCavendorOpt);
HI_S32 HAL_ADVCA_V300_SetTaAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E enType);
HI_S32 HAL_ADVCA_V300_DecTAKeyLadder(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel, HI_U32 *pu32InData, HI_U32 u32InDataLen);
HI_S32 HAL_ADVCA_V300_CryptHCA(HI_U32 AddrID);

#endif	/* __HAL_ADVCA_V300_H__ */