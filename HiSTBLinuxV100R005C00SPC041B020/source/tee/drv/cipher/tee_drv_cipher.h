/******************************************************************************
 *
 * Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
 * ******************************************************************************
 * File Name     : drv_cipher.h
 * Version       : Initial
 * Author        : Hisilicon hisecurity team
 * Created       : 2013-12-28
 * Last Modified :
 * Description   : 
 * Function List :
 * History       :
 * ******************************************************************************/

#ifndef __TEE_DRV_CIPHER_H__
#define __TEE_DRV_CIPHER_H__

#include "tee_drv_cipher_intf.h"
#include "tee_hal_cipher.h"
#include "tee_drv_cipher_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/
#define CIPHER_DEFAULT_INT_NUM    1
#define CIPHER_SOFT_CHAN_NUM      CIPHER_CHAN_NUM
#define CIPHER_INVALID_CHN        (0xffffffff)

/*************************** Structure Definition ****************************/

/** */
typedef HI_VOID (*funcCipherCallback)(HI_U32);

HI_S32 DRV_Cipher_OpenChn(HI_U32 softChnId);
HI_S32 DRV_Cipher_CloseChn(HI_U32 softChnId);
HI_S32 DRV_Cipher_ConfigChn(HI_U32 softChnId, HI_TEE_CIPHER_CTRL_S *pConfig, funcCipherCallback fnCallBack);
HI_S32 DRV_Cipher_CreatTask(HI_U32 softChnId, HI_DRV_CIPHER_TASK_S *pTask, HI_U32 *pKey, HI_U32 *pIV);
HI_S32 DRV_CipherDataDoneMultiPkg(HI_U32 chnId);

HI_S32 DRV_Cipher_CreatMultiPkgTask(HI_U32 softChnId, HI_DRV_CIPHER_DATA_INFO_S *pBuf2Process, HI_U32 pkgNum, HI_BOOL isMultiIV, HI_U32 callBackArg);
HI_S32 DRV_Cipher_Stat(HI_U32 chnId, HI_U32 *pCipherStat);

HI_S32 DRV_Cipher_Init(HI_VOID);
HI_VOID DRV_Cipher_DeInit(HI_VOID);
HI_S32 DRV_Cipher_waitStat(HI_U32 chnId);
HI_BOOL DRV_Cipher_IsBusy(HI_U32 chnId);

HI_S32 Cipher_CalcHashInit(HASH_INFO_S *pCipherHashData);
HI_S32 Cipher_CalcHashUpdate(HASH_INFO_S *pCipherHashData);
HI_S32 Cipher_CalcHashFinal(HASH_INFO_S *pCipherHashData);
HI_S32 DRV_Cipher_CalcHash(HASH_INFO_S *pCipherHashData);

HI_S32 DRV_Cipher_GetHandleConfig(HI_U32 u32SoftChanId, HI_TEE_CIPHER_CTRL_S *pCipherCtrl);
HI_TEE_CIPHER_CTRL_S * DRV_Cipher_GetHandleConfig_ptr(HI_U32 u32SoftChanId);

HI_S32 DRV_Cipher_GetIVOut(HI_U32 u32SoftChanId, HI_U8 *pu8IVOut);
HI_VOID DRV_Cipher_HDCPTxReadEnable(HI_VOID);

HI_VOID DRV_CIPHER_Suspend(HI_VOID);
HI_S32 DRV_CIPHER_Resume(HI_VOID);

#ifdef __cplusplus
}
#endif
#endif /* __DRV_CIPHER_H__ */

