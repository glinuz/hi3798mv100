/******************************************************************************
Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : nocs_bsd.c
Version       : Initial Draft
Author        : Hisilicon hisecurity group
Created       : 2013/12/26
Last Modified :
Description   : Implementation of BSD API
Function List :
History       :
******************************************************************************/
#include "ca_defs.h"
#include "nocs_bsd.h"
#include "nocs_bsd_impl.h"
//#include "drv_cipher_intf.h"
//#include "advca_drv.h"
//#include "ca_reg.h"
#include "nocs_cipher.h"
//#include "otp.h"
#include "hi_common.h"
#include "hi_unf_advca.h"
#include "hi_unf_cipher.h"

#ifndef HI_MINIBOOT_SUPPORT
#include "common.h"
#endif

#define SOFTWARE_VERSION "1.0.1.5\0"
#define CHIPSET_MANUFACTURER "HSI\0"

#define BSD_READ_REG(base, offset) \
    (*(volatile HI_U32   *)((HI_U32)(base) + (offset)))

#define BSD_GET_CHNID(handle)    (((handle)) & 0xff)

#ifdef HI_ADVCA_FUNCTION_RELEASE
#define HI_ERR_BSD(fmt...) //HI_TRACE(HI_LOG_LEVEL_ERROR, HI_DEBUG_ID_ECS, fmt)
#else
#define HI_ERR_BSD(fmt...) printf(fmt)
#endif

/*judge BSD is initialized or not*/
#define CHECK_BSD_INIT()\
do{\
    if (0 == g_bsdInitialize)\
    {\
        HI_ERR_BSD("BSD Not Initialize!\n");\
        return BSD_ERROR;\
    }\
}while(0)

#define BSD_CHECK_NULL_PTR(ptr)\
do{\
    if (NULL == ptr )\
    {\
        HI_ERR_BSD("Null Pointer!\n");\
        return BSD_ERROR_INVALID_PARAMETERS;\
    }\
}while(0)

/** CA vendor type*/
typedef enum hiUNF_ADVCA_VENDOR_TYPE_E
{
    HI_UNF_ADVCA_VENDOR_NONE  = 0x0,
    HI_UNF_ADVCA_VENDOR_NAGRA,
    HI_UNF_ADVCA_VENDOR_BUTT
}HI_UNF_ADVCA_VENDOR_TYPE_E;

static TUnsignedInt32 g_bsdInitialize = 0;

const TUnsignedInt8 g_bsd_xR2RCipheredProtectingKeys[16] =
        {0xe4, 0x21, 0x13, 0xa7, 0x99, 0xca, 0xf4, 0xc7,
          0x05, 0xff, 0x14, 0x32, 0x23, 0x32, 0x91, 0xcb};

const TUnsignedInt8 g_bsd_pxCipheredContentKey[16] = 
        {0xe4, 0x21, 0x13, 0xa7, 0x99, 0xca, 0xf4, 0xc7,
          0x05, 0xff, 0x14, 0x32, 0x23, 0x32, 0x91, 0xcb};

TBsdStatus bsd_Pad(const TUnsignedInt8 *xInputData, TUnsignedInt8* xOutData, TUnsignedInt32 length)
{
    TUnsignedInt8 zeroData[16] = {0};
    TUnsignedInt32 zeroLength = 16 -length;
    
    BSD_CHECK_NULL_PTR(xInputData);
    BSD_CHECK_NULL_PTR(xOutData);
    
    if (length > 16)
    {
        return BSD_ERROR;
    }

    memcpy(xOutData, zeroData, zeroLength);
    memcpy(xOutData + zeroLength, xInputData, length);

    return BSD_NO_ERROR;
}

TBsdStatus bsdInitialize(TBsdInitParameters*  pxInitParameters)
{
    HI_S32 Ret = HI_SUCCESS;
    
    if (g_bsdInitialize > 0)
    {
        HI_ERR_BSD("BSD Already Initialized!\n");
        return BSD_ERROR_DRIVER_ALREADY_INITIALIZED;
    }

    CA_Init();
    
    Ret = BSD_CIPHER_Init();
    if (HI_SUCCESS != Ret)
    {
       HI_ERR_BSD("BSD Initialize ERROR! Ret = 0x%x\n", Ret);
       return BSD_ERROR;
    }

    g_bsdInitialize = 1;

    return BSD_NO_ERROR;
}

TBsdStatus bsdTerminate(TBsdTerminateParameters*  pxTerminateParameters)
{
    CHECK_BSD_INIT();

    BSD_CIPHER_Exit();

    g_bsdInitialize = 0;
    
    return BSD_NO_ERROR;
}

TBsdStatus bsdGetApiVersion(TUnsignedInt32*  pxBsdApiVersion)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdGetSoftwareVersion(TBsd20CharsString  xSoftwareVersion)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdGetNuid(TBsd4BytesVector xNuid)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 u32Nuid = 0;

    CHECK_BSD_INIT();
    BSD_CHECK_NULL_PTR(xNuid);

	Ret = HI_UNF_ADVCA_Init();
    Ret |= HI_UNF_ADVCA_GetChipId(&u32Nuid);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_BSD("BSD Get NUID ERROR! Ret = 0x%x\n", Ret);
        return Ret;
    }

    xNuid[0] = (TUnsignedInt8)(u32Nuid);
    xNuid[1] = (TUnsignedInt8)(u32Nuid >> 8);
    xNuid[2] = (TUnsignedInt8)(u32Nuid >> 16);
    xNuid[3] = (TUnsignedInt8)(u32Nuid >> 24);
    
    return BSD_NO_ERROR;
}

TBsdStatus bsdGetNuid64(TBsd8BytesVector xNuid64)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdSetStbCaSn(const TBsd4BytesVector xStbCaSn)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdGetStbCaSn(TBsd4BytesVector xStbCaSn)
{
    HI_S32 Ret = HI_SUCCESS;

    CHECK_BSD_INIT();
    BSD_CHECK_NULL_PTR(xStbCaSn);

	Ret = HI_UNF_ADVCA_Init();
    Ret |= HI_UNF_ADVCA_GetStbSn(xStbCaSn);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_BSD("BSD Get NUID ERROR! Ret = 0x%x\n", Ret);
        return Ret;
    }

    return BSD_NO_ERROR;
}


TBsdStatus bsdSetMarketSegmentId(const TBsd4BytesVector xMarketSegmentId)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdGetMarketSegmentId(TBsd4BytesVector xMarketSegmentId)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdGetNUIDCheckNumber(TBsd4BytesVector xNUIDCheckNumber)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdGetDataIntegrityCheckNumber(const TUnsignedInt8 *pxData, TUnsignedInt32 xDataSize, TBsd4BytesVector xDataIntegrityCheckNumber)
{
    TBsdStatus bsdStatus = BSD_NO_ERROR;
    TUnsignedInt8 paddedDataIntegrity[16] = {0};
    TUnsignedInt8 encryptedData[16] = {0};
    HI_U32 resultData = 0;

    CHECK_BSD_INIT();
    BSD_CHECK_NULL_PTR(pxData);
    BSD_CHECK_NULL_PTR(xDataIntegrityCheckNumber);

    if ((xDataSize > 16) || (xDataSize < 1))
    {
        HI_ERR_BSD("BSD Get Data Integrity Check Number Parameters Invalid!\n");
        return BSD_ERROR_INVALID_PARAMETERS;
    }

    bsdStatus = bsd_Pad(pxData, paddedDataIntegrity, xDataSize);
    if (BSD_NO_ERROR != bsdStatus)
    {
        HI_ERR_BSD("BSD Get Data Integrity Check Number pad Data Integrity ERROR! bsdStatus = 0x%x\n", bsdStatus);
        return BSD_ERROR;
    }
    
    bsdStatus = bsdEncryptDataWithSecretContentKey(BSD_R2R_ALGORITHM_TDES,
                                    paddedDataIntegrity,
                                    encryptedData,
                                    16);
    if (BSD_NO_ERROR != bsdStatus)
    {
        HI_ERR_BSD("BSD Get Data Integrity Check Number Encrypt ERROR! bsdStatus = 0x%x\n", bsdStatus);
        return BSD_ERROR;
    }
    
    resultData = crc32(0L, encryptedData, sizeof(encryptedData));

    xDataIntegrityCheckNumber[0] = resultData >> 24;
    xDataIntegrityCheckNumber[1] = resultData >> 16;
    xDataIntegrityCheckNumber[2] = resultData >> 8;
    xDataIntegrityCheckNumber[3] = resultData;
    
    return BSD_NO_ERROR;
}

TBsdStatus bsdChipsetInformation(TBsdInfoChipset* pxInfoChipset)
{
#if 0 /*temporarily comment, to be debug*/
    HI_S32 Ret = HI_SUCCESS;
    HI_CHIP_TYPE_E chipType;
    HI_U32 u32ChipVersion;
    
    CHECK_BSD_INIT();
    BSD_CHECK_NULL_PTR(pxInfoChipset);

    memcpy(pxInfoChipset->BSD_CM, CHIPSET_MANUFACTURER, 4);

    HI_DRV_SYS_GetChipVersion(&chipType, HI_NULL);

    switch (chipType)
    {
        case HI_CHIP_TYPE_HI3716C:
        {
            snprintf((char *)pxInfoChipset->BSD_CT, sizeof(pxInfoChipset->BSD_CT), "Hi3716CV2");
            break;
        }
        case HI_CHIP_TYPE_HI3719C:
        {
            snprintf((char *)pxInfoChipset->BSD_CT, sizeof(pxInfoChipset->BSD_CT), "Hi3719CV1");
            break;
        }
        case HI_CHIP_TYPE_HI3719M:
        {
            snprintf((char *)pxInfoChipset->BSD_CT, sizeof(pxInfoChipset->BSD_CT), "Hi3719MV1");
            break;
        }
        case HI_CHIP_TYPE_HI3716M:
        {
            snprintf((char *)pxInfoChipset->BSD_CT, sizeof(pxInfoChipset->BSD_CT), "Hi3716MV4");
            break;
        }
        default:
            snprintf((char *)pxInfoChipset->BSD_CT, sizeof(pxInfoChipset->BSD_CT), "UNKNOWN");
    }
    
    u32ChipVersion = otp_read_reg(CA_V300_CA_VERSION);
    snprintf((char *)pxInfoChipset->BSD_CUT, sizeof(pxInfoChipset->BSD_CUT), "%x", u32ChipVersion & 0xff);
    
    snprintf((char *)pxInfoChipset->BSD_EXT, sizeof(pxInfoChipset->BSD_EXT), "N/A");
#endif
    return BSD_NO_ERROR;
}

TBsdStatus bsdPrint(const TChar*  pxString)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

void bsdChipReset()
{
    return;
}

TBsdStatus bsdEncryptFlashProtKey(const TUnsignedInt8* pxInput, TUnsignedInt8* pxOutput, size_t xSize)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdOpenRam2RamEncryptSession(TBsdCipherSession*  pxSession)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdOpenRam2RamDecryptSession(TBsdCipherSession*  pxSession)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdCloseSession(TBsdCipherSession  xSession)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdSetClearTextKey(
        TBsdCipherSession   xSession,
        TUnsignedInt16      xEmi,
        TSize               xClearTextKeySize,
  const TUnsignedInt8*     pxClearTextKey)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdSet2LevelProtectedKey(
        TBsdCipherSession  xSession,
        TUnsignedInt16     xEmi,
        TSize              xCipheredContentKeySize,
  const TUnsignedInt8*    pxCipheredContentKey,
        TSize              xCipheredProtectingKeySize,
  const TUnsignedInt8*    pxL1CipheredProtectingKey,
  const TUnsignedInt8*    pxL2CipheredProtectingKey)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdUseFlashProtKey(TBsdCipherSession xSession, TUnsignedInt16 xEmi)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdUseCertKey(TBsdCipherSession xSession, TUnsignedInt16 xEmi)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdSessionEncrypt(
        TBsdCipherSession   xSession,
  const TUnsignedInt8*     pxInput,
        TUnsignedInt8*     pxOutput,
        TSize              xMessageSize,
  const TUnsignedInt8*     pxInitVector,
        TSize              xInitVectorSize)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdSessionDecrypt(
        TBsdCipherSession   xSession,
  const TUnsignedInt8*     pxInput,
        TUnsignedInt8*     pxOutput,
        TSize               xMessageSize,
  const TUnsignedInt8*     pxInitVector,
        TSize               xInitVectorSize)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdRsaPublicEncrypt(
  const TUnsignedInt8*      pxInput,
        TSize                xInputSize,
        TUnsignedInt8*      pxOutput,
        TUnsignedInt32       xE,
  const TUnsignedInt8*      pxN,
        TSize                xKeySize,
        TBsdRsaPadding       xPadding)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdRsaPublicDecrypt(
  const TUnsignedInt8*      pxInput,
        TUnsignedInt8*      pxOutput,
        TSize*              pxOutputSize,
        TUnsignedInt32       xE,
  const TUnsignedInt8*      pxN,
        TSize                xKeySize,
        TBsdRsaPadding       xPadding)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdSha256Init(TBsdHashContext*  pxContext)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdSha256Update(
        TBsdHashContext  xContext,
  const TUnsignedInt8*  pxMessageChunk,
        TSize            xChunkSize)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdSha256Final(TBsdHashContext xContext, TUnsignedInt8* pxMessageDigest)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdHmacSha256Init(
  const TUnsignedInt8*     pxKey,
        TSize               xKeySize,
        TBsdHashContext*   pxContext)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdHmacSha256Update(
        TBsdHashContext  xContext,
  const TUnsignedInt8*  pxMessageChunk,
        TSize            xChunkSize)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdHmacSha256Final(TBsdHashContext xContext, TUnsignedInt8* pxMessageDigest)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}

TBsdStatus bsdGenerateRandomBytes(TSize xNumOfBytes, TUnsignedInt8* pxRandomBytes)
{
    return BSD_ERROR_NOT_IMPLEMENTED;
}


TBsdStatus bsdEncryptDataWithSecretContentKey(
        TBsdR2RAlgorithm                xAlgorithm,
        const TUnsignedInt8*              pxInputData,
        TUnsignedInt8*              pxOutputData,
        TUnsignedInt32               xDataSize)
{
#if 0 /*temporarily comment, to be debug*/
    HI_S32 Ret = HI_SUCCESS;
    HI_UNF_CIPHER_CTRL_S CipherCtrl;
    HI_U32 u32MmzPhyAddr = 0;
    HI_HANDLE cipherHandle;

    HI_UNF_ADVCA_ALG_TYPE_E r2rAlg = HI_UNF_ADVCA_ALG_TYPE_TDES;
    CA_CRYPTPM_S cryptPm;
    CHECK_BSD_INIT();
    
    if (xAlgorithm >= LAST_BSD_R2R_ALGORITHM)
    {
        HI_ERR_BSD("BSD Encrypt Data With Secret Content Key Operation Not Supported!\n");
        return BSD_ERROR_NOT_SUPPORTED;
    }

    /*the size of the input data block must be a multiple of 8 bytes if it must be encrypted with the TDES algorithm or
     a multiple of 16 bytes if it must be encrypted with the AES algorithm*/
    if (!(((BSD_R2R_ALGORITHM_AES == xAlgorithm) && (0 == xDataSize % 16)) ||
        ((BSD_R2R_ALGORITHM_TDES == xAlgorithm) && (0 == xDataSize % 8))))
    {
        HI_ERR_BSD("BSD Encrypt Data With Secret Content Key Parameters Invalid!\n");
        return BSD_ERROR_INVALID_PARAMETERS;
    }

    BSD_CHECK_NULL_PTR(pxInputData);
    BSD_CHECK_NULL_PTR(pxOutputData);

    /* genarate private key */
    cryptPm.ladder = HI_UNF_ADVCA_KEYLADDER_LEV1;
    memcpy(cryptPm.pDin, g_bsd_xR2RCipheredProtectingKeys, 16);
    
    CA_SetR2RAlg(r2rAlg);
    Ret = CA_CryptR2R(cryptPm.ladder,cryptPm.pDin,0,1);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_BSD("BSD Encrypt Data With Secret Content Key Genarate Private Key ERROR! Ret = 0x%x\n", Ret);
        return BSD_ERROR;
    }

    CipherCtrl.bKeyByCA = HI_TRUE;
    CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
    switch (xAlgorithm)
    {
        case BSD_R2R_ALGORITHM_AES:
            CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
            CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
            CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
            break;
        case BSD_R2R_ALGORITHM_TDES:
            CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_3DES;
            CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_64BIT;
            CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_DES_2KEY;
            break;
        default:
            CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_BUTT;
    }

    CipherCtrl.stChangeFlags.bit1IV = 0;
    
    memset(CipherCtrl.u32Key, 0, sizeof(CipherCtrl.u32Key));
    memset(CipherCtrl.u32IV, 0, sizeof(CipherCtrl.u32IV));

    memcpy(CipherCtrl.u32Key, g_bsd_pxCipheredContentKey, 16);

    Ret = BSD_CIPHER_CreateHandle(&cipherHandle);
    Ret |= BSD_CIPHER_ConfigHandle(cipherHandle, &CipherCtrl);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_BSD("BSD CIPHER Config Handle ERROR! Ret = 0x%x\n", Ret);
        return BSD_ERROR;
    }

    Ret = HI_MEM_Alloc(&u32MmzPhyAddr, xDataSize);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_BSD("BSD Get Memory for Input ERROR!\n");
        goto DESTROY_HANDLE;
    }

    memset((void*)u32MmzPhyAddr, 0, xDataSize);
    memcpy((void*)u32MmzPhyAddr, pxInputData, xDataSize);
    Ret = BSD_CIPHER_Encrypt(cipherHandle, u32MmzPhyAddr, u32MmzPhyAddr, xDataSize);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_BSD("BSD CIPHER Encrypt ERROR! Ret = 0x%x\n", Ret);
        goto DESTROY_HANDLE;
    }

    memcpy(pxOutputData, (void*)u32MmzPhyAddr, xDataSize);

DESTROY_HANDLE:
    (HI_VOID)BSD_CIPHER_DestroyHandle(cipherHandle);
#endif
    return BSD_NO_ERROR;
}

TBsdStatus bsdDecryptDataWithSecretContentKey(
        TBsdR2RAlgorithm                xAlgorithm,
        const TUnsignedInt8*              pxInputData,
        TUnsignedInt8*              pxOutputData,
        TUnsignedInt32               xDataSize)
{
    HI_S32 Ret = HI_SUCCESS;
#if 0 /*temporarily comment, to be debug*/
    HI_UNF_CIPHER_CTRL_S CipherCtrl;
    HI_U32 u32MmzPhyAddr = 0;
    HI_HANDLE cipherHandle;
    
    HI_UNF_ADVCA_ALG_TYPE_E r2rAlg = HI_UNF_ADVCA_ALG_TYPE_TDES;
    CA_CRYPTPM_S cryptPm;

    CHECK_BSD_INIT();

    if (xAlgorithm >= LAST_BSD_R2R_ALGORITHM)
    {
        HI_ERR_BSD("BSD Decrypt Data With Secret Content Key Operation Not Supported!\n");
        return BSD_ERROR_NOT_SUPPORTED;
    }

    /*the size of the input data block must be a multiple of 8 bytes if it must be encrypted with the TDES algorithm or
     a multiple of 16 bytes if it must be encrypted with the AES algorithm*/
    if (!(((BSD_R2R_ALGORITHM_AES == xAlgorithm) && (0 == xDataSize % 16)) ||
        ((BSD_R2R_ALGORITHM_TDES == xAlgorithm) && (0 == xDataSize % 8))))
    {
        HI_ERR_BSD("BSD Decrypt Data With Secret Content Key Parameters Invalid!\n");
        return BSD_ERROR_INVALID_PARAMETERS;
    }

    BSD_CHECK_NULL_PTR(pxInputData);
    BSD_CHECK_NULL_PTR(pxOutputData);

    /* genarate private key */
    cryptPm.ladder = HI_UNF_ADVCA_KEYLADDER_LEV1;
    memcpy(cryptPm.pDin, g_bsd_xR2RCipheredProtectingKeys, 16);
    
    CA_SetR2RAlg(r2rAlg);
    Ret = CA_CryptR2R(cryptPm.ladder,cryptPm.pDin,0,1);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_BSD("BSD Decrypt Data With Secret Content Key Genarate Private Key ERROR! Ret = 0x%x\n", Ret);
        return BSD_ERROR;
    }

    CipherCtrl.bKeyByCA = HI_TRUE;
    CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
    switch (xAlgorithm)
    {
        case BSD_R2R_ALGORITHM_AES:
            CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
            CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
            CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
            break;
        case BSD_R2R_ALGORITHM_TDES:
            CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_3DES;
            CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_64BIT;
            CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_DES_2KEY;
            break;
        default:
            CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_BUTT;
    }

    memset(CipherCtrl.u32Key, 0, sizeof(CipherCtrl.u32Key));
    memset(CipherCtrl.u32IV, 0, sizeof(CipherCtrl.u32IV));

    memcpy(CipherCtrl.u32Key, g_bsd_pxCipheredContentKey, 16);

    Ret = BSD_CIPHER_CreateHandle(&cipherHandle);
    Ret |= BSD_CIPHER_ConfigHandle(cipherHandle, &CipherCtrl);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_BSD("BSD CIPHER Config Handle ERROR! Ret = 0x%x\n", Ret);
        return BSD_ERROR;
    }

    Ret = HI_MEM_Alloc(&u32MmzPhyAddr, xDataSize);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_BSD("BSD Get Memory for Input ERROR!\n");
        goto DESTROY_HANDLE;
    }

    memset((void*)u32MmzPhyAddr, 0, xDataSize);
    memcpy((void*)u32MmzPhyAddr, pxInputData, xDataSize);
    Ret = BSD_CIPHER_Decrypt(cipherHandle, u32MmzPhyAddr, u32MmzPhyAddr, xDataSize);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_BSD("BSD CIPHER Decrypt ERROR! Ret = 0x%x\n", Ret);
        goto DESTROY_HANDLE;
    }

    memcpy(pxOutputData, (void*)u32MmzPhyAddr, xDataSize);

DESTROY_HANDLE:
    BSD_CIPHER_DestroyHandle(cipherHandle);
#endif
    return BSD_NO_ERROR;
}

