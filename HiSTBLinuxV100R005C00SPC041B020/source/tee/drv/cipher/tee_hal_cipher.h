/******************************************************************************
 *
 * Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
 * ******************************************************************************
 * File Name     : hal_cipher.h
 * Version       : Initial
 * Author        : Hisilicon hisecurity team
 * Created       : 2013-12-28
 * Last Modified :
 * Description   : 
 * Function List :
 * History       :
 * ******************************************************************************/

#ifndef __HAL_CIPHER_H__
#define __HAL_CIPHER_H__

#include "tee_drv_cipher_reg.h"
#include "tee_drv_cipher_define.h"
#include "tee_drv_cipher.h"
#include "hi_tee_cipher_ioctl.h"
#include "tee_internal_api.h"
#include "drv_tee_mem.h"

#ifdef __cplusplus
extern "C" {
#endif
/***************************** Macro Definition ******************************/

#define  IO_ADDRESS(x)         (x)

#define  CIPHER_CHAN_NUM                        (8)
#define  CIPHER_PKGx1_CHAN                      (0)
#define  CIPHER_PKGxN_CHAN_MIN                  (4)
#define  CIPHER_PKGxN_CHAN_MAX                  (7)
#define  CIPHER_HMAC_KEY_LEN                    (16)

#ifdef  WITH_HISI_SMMU
#define CIPHER_MMU_SUPPORT
#endif

#if defined (CHIP_TYPE_hi3798cv200)   \
    || defined (CHIP_TYPE_hi3798mv200_a) \
    || defined (CHIP_TYPE_hi3798mv200)
#define HW_RSA_SUPPORT
#define CIPHER_MHASH_SUPPORT
#endif

/*************************** Structure Definition ****************************/
/** */
typedef enum hiCIPHER_INT_TYPE_E
{
    CIPHER_INT_TYPE_IN_BUF  = 					0x1,
    CIPHER_INT_TYPE_OUT_BUF = 					0x2,
} CIPHER_INT_TYPE_E;

typedef enum hiCIPHER_BUF_TYPE_E
{
    CIPHER_BUF_TYPE_IN  = 						0x1,
    CIPHER_BUF_TYPE_OUT = 						0x2,
} CIPHER_BUF_TYPE_E;

typedef enum hiCIPHER_RSA_DATA_TYPE_E
{
    CIPHER_RSA_DATA_TYPE_CONTEXT,
    CIPHER_RSA_DATA_TYPE_MODULE,
    CIPHER_RSA_DATA_TYPE_KEY,
} CIPHER_RSA_DATA_TYPE_E;

typedef enum hiCIPHER_RSA_KEY_WIDTH_E
{ 
    CIPHER_RSA_KEY_WIDTH_1K = 0x00,
    CIPHER_RSA_KEY_WIDTH_2K = 0x01,
    CIPHER_RSA_KEY_WIDTH_4K = 0x02,
}CIPHER_RSA_KEY_WIDTH_E;

typedef struct hiHASH_INFO_S
{
    HI_BOOL bIsUsed;
    HI_TEE_CIPHER_HASH_TYPE_E enShaType;
    HI_U32 u32TotalDataLen;
    HI_BOOL bIsUseIinitVar;
    HI_U32  u32Hash[8];
    HI_U32  u32HashLen;
    HI_U8   u8LastBlock[HASH_BLOCK_SIZE];
    HI_U32  u8LastBlockSize;
    TEE_MMZ_BUFFER_S stMMZBuffer;
    TEE_UUID     stUserUUID;
}HASH_INFO_S;

/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/
HI_VOID *Cipher_Malloc(HI_U32 size);
HI_VOID Cipher_Free(HI_VOID *memblock);

HI_S32 HAL_Cipher_GetInBufCnt(HI_U32 chnId, HI_U32 *pNum);
HI_S32 HAL_Cipher_GetInBufEmpty(HI_U32 chnId, HI_U32 *pNum);
HI_S32 HAL_Cipher_GetInBufNum(HI_U32 chnId, HI_U32 *pNum);
HI_S32 HAL_Cipher_GetOutBufCnt(HI_U32 chnId, HI_U32 *pNum);
HI_S32 HAL_Cipher_GetOutBufFull(HI_U32 chnId, HI_U32 *pNum);
HI_S32 HAL_Cipher_GetOutBufNum(HI_U32 chnId, HI_U32 *pNum);
HI_S32 HAL_Cipher_SetInBufCnt(HI_U32 chnId, HI_U32 num);
HI_S32 HAL_Cipher_GetSrcLstRaddr(HI_U32 chnId, HI_U32 *addr);
HI_S32 HAL_Cipher_GetDestLstRaddr(HI_U32 chnId, HI_U32 *addr);
HI_S32 HAL_Cipher_SetInBufEmpty(HI_U32 chnId, HI_U32 num);
HI_S32 HAL_Cipher_SetInBufNum(HI_U32 chnId, HI_U32 num);
HI_S32 HAL_Cipher_SetOutBufCnt(HI_U32 chnId, HI_U32 num);
HI_S32 HAL_Cipher_SetOutBufFull(HI_U32 chnId, HI_U32 num);
HI_S32 HAL_Cipher_SetOutBufNum(HI_U32 chnId, HI_U32 num);
HI_S32 HAL_Cipher_SetMmuBypass(HI_U32 chnId);
HI_VOID HAL_Cipher_SetMmuAddr(HI_U32 u32ptaddr, HI_U32 u32err_rdaddr, HI_U32 u32err_wraddr);
HI_VOID HAL_Cipher_GetMmuIntState(HI_U32 *pState);
HI_VOID HAL_Cipher_ClrMmuIntState(HI_U32 intStatus);
HI_S32 HAL_Cipher_SetAGEThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value);
HI_S32 HAL_Cipher_SetIntThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value);
HI_VOID HAL_Cipher_DisableAllInt(HI_VOID);
HI_S32 HAL_Cipher_DisableInt(HI_U32 chnId, int intType);
HI_S32 HAL_Cipher_EnableInt(HI_U32 chnId, int intType);
HI_VOID HAL_Cipher_GetRawIntState(HI_U32 *pState);
HI_VOID HAL_Cipher_ClrIntState(HI_U32 intStatus);
HI_VOID HAL_Cipher_EnableAllSecChn(HI_VOID);
HI_VOID HAL_Cipher_GetIntState(HI_U32 *pState);
HI_VOID HAL_Cipher_GetIntEnState(HI_U32 *pState);

HI_VOID HAL_Cipher_GetMmuIntState(HI_U32 *pState);
HI_VOID HAL_Cipher_ClrMmuIntState(HI_U32 intStatus);
HI_VOID HAL_Cipher_EnableMmu(HI_VOID);

HI_VOID HAL_Cipher_ClearHdcpCtrlReg(HI_VOID);
HI_S32 HAL_Cipher_Config(HI_U32 chnId, HI_BOOL bDecrypt, HI_BOOL bIVChange, HI_TEE_CIPHER_CTRL_S* pCtrl);

HI_S32 HAL_Cipher_SetBufAddr(HI_U32 chnId, CIPHER_BUF_TYPE_E bufType, HI_U32 addr);
HI_S32 HAL_Cipher_SetInIV(HI_U32 chnId, HI_TEE_CIPHER_CTRL_S* pCtrl);
HI_S32 HAL_Cipher_GetOutIV(HI_U32 chnId, HI_TEE_CIPHER_CTRL_S* pCtrl);
HI_S32 HAL_Cipher_SetKey(HI_U32 chnId, HI_TEE_CIPHER_CTRL_S* pCtrl);

HI_S32 HAL_Cipher_SetDataSinglePkg(HI_DRV_CIPHER_DATA_INFO_S * info);
HI_S32 HAL_Cipher_StartSinglePkg(HI_U32 chnId);
HI_S32 HAL_Cipher_ReadDataSinglePkg(HI_U32 *pData);

HI_S32 HAL_Cipher_WaitIdle(HI_VOID);
HI_BOOL HAL_Cipher_IsIdle(HI_U32 chn);
HI_VOID HAL_Cipher_Reset(HI_VOID);
HI_S32 HAL_Cipher_Stat(HI_U32 chnId, HI_U32 *pCipherStat);
HI_VOID HAL_Cipher_HDCPTxReadEnable(HI_VOID);
HI_S32 HAL_CIPHER_LoadSTBRootKey(HI_U32 u32ChID);

HI_S32 HAL_Cipher_CalcHashInit(HASH_INFO_S *pCipherHashData);
HI_S32 HAL_Cipher_CalcHashUpdate(HASH_INFO_S *pCipherHashData);
HI_S32 HAL_Cipher_CalcHashFinal(HASH_INFO_S *pCipherHashData);

HI_VOID HAL_Cipher_Init(void);
HI_VOID HAL_Cipher_DeInit(void);

HI_VOID HAL_RSA_Reset(HI_VOID);
HI_S32 HAL_RSA_WaitFree(HI_VOID);
HI_VOID HAL_RSA_Start(HI_VOID);
HI_VOID HAL_RSA_ClearRam(HI_VOID);
HI_VOID HAL_RSA_ConfigMode(CIPHER_RSA_KEY_WIDTH_E enKenWidth);
HI_VOID HAL_RSA_WriteData(CIPHER_RSA_DATA_TYPE_E enDataType, HI_U8 *pu8Data, HI_U32 u32DataLen, HI_U32 u32Len);
HI_VOID HAL_RSA_ReadData(HI_U8 *pu8Data, HI_U32 u32DataLen, HI_U32 u32Len);

#ifdef __cplusplus
}
#endif
#endif /* __HAL_CIPHER_H__ */


