/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_cipher.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/

#ifndef __DRV_CIPHER_H__
#define __DRV_CIPHER_H__

/* add include here */
#include "hal_cipher.h"
#include "drv_cipher_ioctl.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/
#define CIPHER_DEFAULT_INT_NUM    1

HI_S32 DRV_CIPHER_ReadReg(HI_U32 *addr, HI_U32 *pVal);
HI_S32 DRV_CIPHER_WriteReg(HI_U32 *addr, HI_U32 Val);
HI_S32 DRV_CIPHER_Init(HI_VOID);
HI_VOID DRV_CIPHER_DeInit(HI_VOID);
HI_S32 DRV_CIPHER_Open(struct inode * inode, struct file * file);
HI_S32 DRV_CIPHER_Release(struct inode * inode, struct file * file);
HI_S32 DRV_CIPHER_OpenChn(HI_U32 softChnId);
HI_S32 DRV_CIPHER_CloseChn(HI_U32 softChnId);
HI_S32 DRV_CIPHER_CreatTask(HI_U32 softChnId, HI_DRV_CIPHER_TASK_S *pTask, HI_U32 *pKey, HI_U32 *pIV);
HI_S32 DRV_CIPHER_HdcpParamConfig(HI_DRV_CIPHER_HDCP_KEY_MODE_E enHdcpEnMode, HI_DRV_CIPHER_HDCP_ROOT_KEY_TYPE_E enRootKeyType, HI_DRV_HDCPKEY_TYPE_E enHDCPKeyType);
HI_S32 DRV_CIPHER_ClearHdcpConfig(HI_VOID);
HI_S32 DRV_CIPHER_GetHandleConfig(CIPHER_Config_CTRL *pstCipherConfig);
HI_S32 DRV_CIPHER_CreatMultiPkgTask(HI_U32 softChnId, HI_DRV_CIPHER_DATA_INFO_S *pBuf2Process, HI_U32 pkgNum, HI_BOOL isMultiIV, HI_U32 callBackArg);
HI_VOID DRV_CIPHER_SetHdcpModeEn(HI_DRV_CIPHER_HDCP_KEY_MODE_E enMode);
HI_S32 DRV_CIPHER_GetHdcpModeEn(HI_DRV_CIPHER_HDCP_KEY_MODE_E *penMode);
HI_VOID DRV_CIPHER_SetHdcpkeyRxRead(HI_DRV_CIPHER_HDCP_KEY_RX_READ_E enRxReadMode);
HI_VOID DRV_CIPHER_SetHdcpKeyTxRead(HI_DRV_CIPHER_HDCP_KEY_RAM_MODE_E enMode);
HI_S32 DRV_CIPHER_SetHdcpkeyRxSelect(HI_DRV_CIPHER_HDCP_KEY_RX_SELECT_E enRxSelect);
HI_S32 DRV_CIPHER_SetHdcpKeySelectMode(HI_DRV_CIPHER_HDCP_ROOT_KEY_TYPE_E enHdcpKeySelectMode);
HI_S32 DRV_CIPHER_GetHdcpKeySelectMode(HI_DRV_CIPHER_HDCP_ROOT_KEY_TYPE_E *penHdcpKeySelectMode);
HI_S32 DRV_CIPHER_EncryptHDCPKey(CIPHER_HDCP_KEY_TRANSFER_S *pstHdcpKeyTransfer);
HI_S32 DRV_CIPHER_CbcMacAuth(CIPHER_CBCMAC_DATA_S *pstParam);
HI_S32 DRV_CIPHER_ConfigChn(HI_U32 softChnId,  HI_UNF_CIPHER_CTRL_S *pConfig);
HI_S32 DRV_CIPHER_DestroyHandle(HI_HANDLE hCipherchn);
HI_S32 DRV_CIPHER_LoadHdcpKey(HI_DRV_CIPHER_FLASH_ENCRYPT_HDCPKEY_S *pstFlashHdcpKey);
HI_S32 DRV_CIPHER_GetRandomNumber(CIPHER_RNG_S *pstRNG);

HI_S32 DRV_CIPHER_CalcRsaCrc(HI_U16 *pu16Crc);
HI_S32 DRV_CIPHER_CalcRsa_ex(CIPHER_RSA_DATA_S *pCipherRsaData);



#ifdef __cplusplus
}
#endif
#endif /* __DRV_CIPHER_H__ */
