/**
\file
\brief cipher osi
\copyright Shenzhen Hisilicon Co., Ltd.
\version draft
\author QuYaxin 46153
\date 2009-11-8
*/

#ifndef __DRV_CIPHER_EXT_H__
#define __DRV_CIPHER_EXT_H__

/* add include here */
#include "hi_error_mpi.h"
#include "hi_unf_cipher.h"
#include "hi_drv_cipher.h"
#include "drv_cipher_ioctl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef HI_VOID (*funcCipherCallback)(HI_U32);

typedef HI_S32 (*FN_CIPHER_CreateHandle)(CIPHER_HANDLE_S * pstCIHandle, HI_VOID *file);
typedef HI_S32 (*FN_CIPHER_ConfigChn)(HI_U32 softChnId,  HI_UNF_CIPHER_CTRL_S *pConfig);
typedef HI_S32 (*FN_CIPHER_DestroyHandle)(HI_HANDLE hCipherchn);
typedef HI_S32 (*FN_CIPHER_Encrypt)(CIPHER_DATA_S * pstCIData);
typedef HI_S32 (*FN_CIPHER_Decrypt)(CIPHER_DATA_S * pstCIData);
typedef HI_S32 (*FN_CIPHER_EncryptMulti)(CIPHER_PKG_S * pstPkg);
typedef HI_S32 (*FN_CIPHER_DecryptMulti)(CIPHER_PKG_S * pstPkg);
typedef HI_S32 (*FN_CIPHER_GetRandomNumber)(CIPHER_RNG_S *pstRNG);
typedef HI_S32 (*FN_CIPHER_GetHandleConfig)(CIPHER_Config_CTRL * pstCipherConfig);
typedef HI_S32 (*FN_CIPHER_LoadHdcpKey)(HI_DRV_CIPHER_FLASH_ENCRYPT_HDCPKEY_S *pstFlashHdcpKey);
typedef HI_S32 (*FN_CIPHER_SoftReset)(HI_VOID);
typedef HI_S32 (*FN_CIPHER_CalcHashInit)(CIPHER_HASH_DATA_S *pCipherHashData);
typedef HI_S32 (*FN_CIPHER_CalcHashUpdate)(CIPHER_HASH_DATA_S *pCipherHashData);
typedef HI_S32 (*FN_CIPHER_CalcHashFinal)(CIPHER_HASH_DATA_S *pCipherHashData);
typedef HI_S32 (*FN_CIPHER_Resume)(HI_VOID);
typedef HI_VOID (*FN_CIPHER_Suspend)(HI_VOID);
typedef HI_S32 (*FN_CIPHER_EncryptHDCPKey)(CIPHER_HDCP_KEY_TRANSFER_S *pstHdcpKeyTransfer);
typedef HI_VOID (*FN_CIPHER_SetHdmiReadClk)(HI_DRV_HDCPKEY_TYPE_E enHDCPKeyType);

typedef struct
{
	FN_CIPHER_CreateHandle 		pfnCipherCreateHandle;
	FN_CIPHER_ConfigChn 		pfnCipherConfigChn;
	FN_CIPHER_DestroyHandle 	pfnCipherDestroyHandle;
	FN_CIPHER_Encrypt 			pfnCipherEncrypt;
	FN_CIPHER_Decrypt 			pfnCipherDecrypt;
	FN_CIPHER_EncryptMulti 		pfnCipherEncryptMulti;
	FN_CIPHER_DecryptMulti 		pfnCipherDecryptMulti;
	FN_CIPHER_GetRandomNumber 	pfnCipherGetRandomNumber;
	FN_CIPHER_GetHandleConfig 	pfnCipherGetHandleConfig;
	FN_CIPHER_LoadHdcpKey		pfnCipherLoadHdcpKey;
	FN_CIPHER_SoftReset 		pfnCipherSoftReset;
    FN_CIPHER_CalcHashInit 		pfnCipherCalcHashInit;
    FN_CIPHER_CalcHashUpdate 	pfnCipherCalcHashUpdate;
    FN_CIPHER_CalcHashFinal 	pfnCipherCalcHashFinal;
	FN_CIPHER_Resume			pfnCipherResume;
	FN_CIPHER_Suspend			pfnCipherSuspend;
    FN_CIPHER_EncryptHDCPKey    pfnCipherEncryptHDCPKey;
    FN_CIPHER_SetHdmiReadClk    pfnCipherSetHdmiReadClk;
}CIPHER_EXPORT_FUNC_S;

HI_S32	CIPHER_DRV_ModInit(HI_VOID);
HI_VOID	CIPHER_DRV_ModExit(HI_VOID);

#ifdef __cplusplus
}
#endif
#endif /* __DRV_CIPHER_EXT_H__ */

