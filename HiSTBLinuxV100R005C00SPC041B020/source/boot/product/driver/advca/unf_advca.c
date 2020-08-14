/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : unf_advca.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2014-09-16
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#include "advca_drv.h"
#include "hi_unf_cipher.h"
#include "drv_cipher.h"
#include "hi_unf_otp.h"

extern HI_S32 OTP_V200_getChipID(HI_U32 *pID);
extern HI_S32 HAL_ADVCA_V300_SetKeyladderAttr(CA_KEYLADDER_ATTR_S *pstKeyladderAttr);

HI_S32 HI_UNF_ADVCA_Init(HI_VOID)
{
    return CA_Init();
}

HI_S32 HI_UNF_ADVCA_DeInit(HI_VOID)
{
     return CA_DeInit();
}

HI_S32 HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key)
{
	return CA_SetR2RSessionKey(enStage, pu8Key);
}

HI_S32 HI_UNF_ADVCA_SetRSAKey(HI_U8 *pkey)
{
	HI_S32 ret = HI_SUCCESS;
	HI_U8 au8KeyOut[512] = {0};
	HI_U8 u8Tmp = 0;
	HI_U32 index = 0;
	HI_U8 *ptr = NULL;
	HI_U32 rsa_e_offset = (512 - 16);

	if(NULL == pkey)
	{
		HI_ERR_CA("Invalid param, null pointer.\n");
		return HI_ERR_CA_INVALID_PARA;
	}

	(HI_VOID)HI_UNF_OTP_Init();

	/* check if rsa key is locked or not */
	u8Tmp = HI_OTP_ReadByte(OTP_INTERNAL_DATALOCK_1);	//0x14[0]: rsa_key_lock
	if(1 == (u8Tmp & 1))
	{
		HI_ERR_CA("RSA Key has already been locked, can not be set any more.\n");
		return HI_FAILURE;
	}

	/* check if rsa key has been set or not */
	for(index = 0; index < 16; index++)
	{
		u8Tmp = HI_OTP_ReadByte(OTP_RSA_KEY_E_ADDR + index);
		if(0 != u8Tmp)
		{
			HI_ERR_CA("RSA_Key_E have been already set.\n");
			return HI_FAILURE;
		}
	}

	for(index = 0; index < 256; index++)
	{
		u8Tmp = HI_OTP_ReadByte(OTP_RSA_KEY_N_ADDR + index);
		if(0 != u8Tmp)
		{
			HI_ERR_CA("RSA_Key_N have been already set.\n");
			return HI_FAILURE;
		}
	}

	/* write rsa key */
	ptr = pkey + rsa_e_offset;
	for(index = 0; index < 16; index++)
	{
		ret = HI_OTP_WriteByte(OTP_RSA_KEY_E_ADDR + index, ptr[index]);
		if(HI_SUCCESS != ret)
		{
			HI_ERR_CA("Set RSA_Key_E failed ,ret:0x%x, index:0x%x.\n", ret, index);
			return HI_FAILURE;
		}
	}

	ptr = pkey;
	for(index = 0; index < 256; index++)
	{
		ret = HI_OTP_WriteByte(OTP_RSA_KEY_N_ADDR + index, ptr[index]);
		if(HI_SUCCESS != ret)
		{
			HI_ERR_CA("Set RSA_Key_N failed ,ret:0x%x, index:0x%x.\n", ret, index);
			return HI_FAILURE;
		}
	}

	/* Read rsa key and compare it with the original rsa key */
	memset(au8KeyOut, 0, sizeof(au8KeyOut));
	
	ptr = au8KeyOut + rsa_e_offset;
	for(index = 0; index < 16; index++)
	{
		ptr[index] = HI_OTP_ReadByte(OTP_RSA_KEY_E_ADDR + index);
	}

	ptr = au8KeyOut;
	for(index = 0; index < 256; index++)
	{
		ptr[index] = HI_OTP_ReadByte(OTP_RSA_KEY_N_ADDR + index);
	}

	ret = memcmp(pkey, au8KeyOut, sizeof(au8KeyOut));
	if(0 != ret)
	{
		HI_ERR_CA("Memcmp failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_GetRSAKey(HI_U8 *pkey)
{
	//HI_S32 ret = HI_SUCCESS;
	HI_U8 *ptr = NULL;
	HI_U32 index = 0;

	if(NULL == pkey)
	{
		HI_ERR_CA("Invalid param, null pointer.\n");
		return HI_ERR_CA_INVALID_PARA;
	}

	(HI_VOID)HI_UNF_OTP_Init();

	memset(pkey, 0, 512);

	ptr = pkey + 512 - 16;
	for(index = 0; index < 16; index++)
	{
		ptr[index] = HI_OTP_ReadByte(OTP_RSA_KEY_E_ADDR + index);
	}

	ptr = pkey;
	for(index = 0; index < 256; index++)
	{
		ptr[index] = HI_OTP_ReadByte(OTP_RSA_KEY_N_ADDR + index);
	}

	return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse, HI_UNF_ADVCA_OTP_ATTR_S *pstOtpFuseAttr)
{
	HI_S32 ret = HI_SUCCESS;

	if((NULL == pstOtpFuseAttr) || (enOtpFuse >= HI_UNF_ADVCA_OTP_BUTT))
	{
		HI_ERR_CA("Invalid params input, enOtpFuse:0x%x.\n", enOtpFuse);
		return HI_ERR_CA_INVALID_PARA;
	}

	(HI_VOID)HI_UNF_OTP_Init();

	switch(enOtpFuse)
	{
		case HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG:
		{
			ret = CA_SetRSAKeyLockFlag(pstOtpFuseAttr);
			break;
		}
		default:
		{
			HI_ERR_CA("Invalid params input, enOtpFuse:0x%x.\n", enOtpFuse);
			return HI_ERR_CA_INVALID_PARA;
		}
	}

	return ret;
}

HI_S32 HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse, HI_UNF_ADVCA_OTP_ATTR_S *pstOtpFuseAttr)
{
	HI_S32 ret = HI_SUCCESS;

	if((NULL == pstOtpFuseAttr) || (enOtpFuse >= HI_UNF_ADVCA_OTP_BUTT))
	{
		HI_ERR_CA("Invalid params input, enOtpFuse:0x%x.\n", enOtpFuse);
		return HI_ERR_CA_INVALID_PARA;
	}

	(HI_VOID)HI_UNF_OTP_Init();

	switch(enOtpFuse)
	{
		case HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG:
		{
			ret = CA_GetRSAKeyLockFlag(pstOtpFuseAttr);
			break;
		}
		default:
		{
			HI_ERR_CA("Invalid params input, enOtpFuse:0x%x.\n", enOtpFuse);
			return HI_ERR_CA_INVALID_PARA;
		}
	}

	return ret;
}

HI_S32 HI_UNF_ADVCA_EnableSecBoot(HI_UNF_ADVCA_FLASH_TYPE_E enFlashType)
{
	(HI_VOID)HI_UNF_OTP_Init();

	return CA_EnableSCSActive(enFlashType);
}

HI_S32 HI_UNF_ADVCA_GetSecBootStat(HI_BOOL *pbEnable,HI_UNF_ADVCA_FLASH_TYPE_E *penFlashType)
{
	HI_S32 ret = HI_SUCCESS;

	if((NULL == pbEnable) || (NULL == penFlashType))
	{
		HI_ERR_CA("Invalid params input!\n");
		return HI_ERR_CA_INVALID_PARA;
	}

	(HI_VOID)HI_UNF_OTP_Init();

	ret = CA_GetSCSActiveFlag(pbEnable);
	if(HI_SUCCESS != ret)
	{
		HI_ERR_CA("Get SCS flag failed, ret:0x%x, *pbEnable:0x%x.\n", ret, *pbEnable);
		return HI_FAILURE;
	}

	ret = CA_GetBootMode(penFlashType);
	if(HI_SUCCESS != ret)
	{
		HI_ERR_CA("Get boot mode failed, ret:0x%x, *penFlashType:0x%x.\n", ret, *penFlashType);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

static HI_S32 ADVCA_AuthCbcMac(HI_U8 *pu8RefCbcMac, HI_U32 u32AppLen)
{
    HI_S32 Ret;

    if (pu8RefCbcMac == NULL)
    {
        HI_ERR_CA("Invalid parameter!\n");
        return HI_FAILURE;
    }    
  
    if (u32AppLen % 16 != 0)
    {
        HI_ERR_CA("Invalid parameter, u32AppLen should be algined with 16 byte!\n");
        return HI_FAILURE;
    }

    Ret = DRV_Cipher_AuthCbcMac(pu8RefCbcMac, u32AppLen);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }
    return HI_SUCCESS;
}

static HI_S32 ADVCA_CalCbcMac(HI_U8 *pInputData, HI_U32 u32InputDataLen, HI_U8 *pOutputMAC, HI_BOOL bIsLastBlock)
{
    HI_S32 Ret = HI_SUCCESS;
    static HI_BOOL bIsFirstBlock = HI_TRUE;
    static HI_HANDLE hHashHandle;
    HI_UNF_CIPHER_HASH_ATTS_S stCipherHash;

     if(pInputData == NULL || pOutputMAC == NULL)
    {
        HI_ERR_CA("Invalid parameter!\n");
        return HI_FAILURE;
    }

    if(u32InputDataLen % 16 != 0)
    {
        HI_ERR_CA("Invalid length, should be aligned with 16 byte!\n");
        return HI_FAILURE;
    }

    if(bIsFirstBlock)
    {
        memset(&stCipherHash, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));
        stCipherHash.eShaType = HI_UNF_CIPHER_HASH_TYPE_IRDETO_CBCMAC;
        Ret = HI_UNF_CIPHER_HashInit(&stCipherHash, &hHashHandle);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_CA("Fail to call  HI_UNF_CIPHER_HashInit! Ret = 0x%x\n", Ret);
        }
        bIsFirstBlock = HI_FALSE;
    }

   Ret = HI_UNF_CIPHER_HashUpdate(hHashHandle, pInputData, u32InputDataLen);
   if(Ret != HI_SUCCESS)
   {
        HI_ERR_CA("HI_UNF_CIPHER_HashUpdate failed! Ret = 0x%x\n", Ret);
        return Ret;
   }

    if(bIsLastBlock)
    {
        bIsFirstBlock = HI_TRUE;

        Ret = HI_UNF_CIPHER_HashFinal(hHashHandle, pOutputMAC);
        if(Ret != HI_SUCCESS)
        {
            HI_ERR_CA("HI_UNF_CIPHER_HashFinal failed! Ret = 0x%x\n", Ret);
            return Ret;
        }        
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_CaVendorOperation(HI_UNF_ADVCA_VENDORID_E enCaVendor, HI_UNF_ADVCA_CA_VENDOR_OPT_S *pstCaVendorOpt)
{
    HI_S32 ret = HI_SUCCESS;
    HI_ADVCA_CA_VENDOR_OPT_E enCaVendorOpt = HI_ADVCA_CAVENDOR_ATTR_BUTT;

    if(enCaVendor >= HI_ADVCA_VENDORIDE_BUTT)
    {
        HI_ERR_CA("Invalid parameter, enCaVendor = %d\n", enCaVendor);
        return HI_ERR_CA_INVALID_PARA;
    }

    if(pstCaVendorOpt == NULL)
    {
        HI_ERR_CA("Invalid parameter, pstCaVendorOpt = NULL\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    enCaVendorOpt = pstCaVendorOpt->enCaVendorOpt;
    switch(enCaVendorOpt)
    {
	    case HI_ADVCA_CAVENDOR_ATTR_IRDETO_CBCMAC_CALC:
		{
	        ret = ADVCA_CalCbcMac(pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.pu8InputData,
	                                                        pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.u32InputDataLen,
	                                                        pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.u8OutputCbcMac,
	                                                        HI_TRUE);
	        break;
		}
	    case HI_ADVCA_CAVENDOR_ATTR_IRDETO_CBCMAC_AUTH:
    	{
	        ret = ADVCA_AuthCbcMac(pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.u8RefCbcMAC, 
	                                                        pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.u32AppLen);        
	        ret |= ADVCA_CalCbcMac(pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.pu8InputData,
	                                                        pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.u32InputDataLen,
	                                                        pstCaVendorOpt->unCaVendorOpt.stIrdetoCbcMac.u8OutputCbcMac,
	                                                        HI_TRUE);
	        break;
		}
	    default:
    	{
	        HI_ERR_CA("Invalid parameter, enCaVendorOpt = %d\n", enCaVendorOpt);
	        break;
	    }
    }

    return ret;
}

HI_S32 HI_UNF_ADVCA_GetVendorID(HI_U32 *pu32VendorID)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32VendorID)
    {
        return HI_ERR_CA_COMMON_INVALID_PARA;
    }
    
    *pu32VendorID = otp_read_reg(OTP_V200_CA_VENDOR_ID) & 0xff;

    return ret;
}

static HI_VOID ReverseChipId(HI_U32* pu32ChipID)
{
    HI_U32 U32ChipID = 0;
    HI_U32 U32ReverseChipID = 0;
    U32ChipID = *pu32ChipID;
    HI_U8* pu8IDBuf = (HI_U8*)&U32ChipID;
    U32ReverseChipID = (*pu8IDBuf<<24) + (*(pu8IDBuf + 1)<<16) + (*(pu8IDBuf + 2)<<8) + *(pu8IDBuf + 3);
    *pu32ChipID = U32ReverseChipID;
    return;
}

HI_S32 HI_UNF_ADVCA_GetChipId(HI_U32 *pu32ChipId)
{
    HI_U32 u32VendorId = 0;
	
    OTP_V200_getChipID(pu32ChipId);

    HI_UNF_ADVCA_GetVendorID(&u32VendorId);
	
    if ((0x03 != u32VendorId) && (0x01 != u32VendorId))
    {
        ReverseChipId(pu32ChipId);
    }
    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_GetStbSn(HI_U8 au8StbSn[4])
{
    unsigned int id;
	
    if (au8StbSn == NULL)
    {
        return HI_ERR_CA_COMMON_INVALID_PARA;
    }
	
    id = otp_read_reg(OTP_V200_STBSN);

    au8StbSn[0] = (id >> 24) & 0xff;
    au8StbSn[1] = (id >> 16) & 0xff;
    au8StbSn[2] = (id >> 8) & 0xff;
    au8StbSn[3] = id & 0xff;
	
    return HI_SUCCESS;
}

HI_S32 HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_TYPE_E enKeyLadderType, HI_UNF_ADVCA_KEYLADDER_ATTR_S *pstKeyladderAttr)
{
    HI_S32 ret = HI_SUCCESS;
    CA_KEYLADDER_ATTR_S stKeyladderAttr;

    if(enKeyLadderType >= HI_UNF_ADVCA_KEYLADDER_TYPE_BUTT)
    {
        HI_ERR_CA("Invalid parameter, enKeyLadderType = %d\n", enKeyLadderType);
        return HI_ERR_CA_INVALID_PARA;
    }

    if(pstKeyladderAttr == NULL)
    {
        HI_ERR_CA("Invalid parameter, pstKeyladderAttr = NULL\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    /**The LPK Protected Data is decrypted in the UNF layer**/
    if(enKeyLadderType == HI_UNF_ADVCA_KEYLADDER_LP &&
        pstKeyladderAttr->unKlAttr.stLpKlAttr.enLPKlAttr == HI_UNF_ADVCA_KEYLADDER_LP_ATTR_DECRYPT_PROTECTED_DATA)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    memset(&stKeyladderAttr, 0, sizeof(stKeyladderAttr));
    stKeyladderAttr.enKeyLadderType = enKeyLadderType;
    memcpy(&stKeyladderAttr.stKeyladderAttr, pstKeyladderAttr, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
    ret = HAL_ADVCA_V300_SetKeyladderAttr(&stKeyladderAttr);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Set keyladder attrf err:0x%x\n", ret);
        return ret;
    }

    return ret;
}

HI_S32 HI_UNF_ADVCA_SetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_E enType)
{
    HI_S32 ret;
    ret = CA_SetR2RAlg(enType);

    return ret;
}

HI_S32 HI_UNF_ADVCA_GetR2RKeyLadderStage(HI_UNF_ADVCA_KEYLADDER_LEV_E *penStage)
{
   HI_S32 ret;
   ret = CA_GetR2RLadder(penStage);
   return ret;
}

HI_S32 HI_UNF_ADVCA_EncryptSWPK(HI_U8 *pPlainSwpk,HI_U8 *pEncryptedSwpk)
{
   HI_S32 ret;
   
   if(pPlainSwpk == NULL || pEncryptedSwpk == NULL)
   {
       HI_ERR_CA("Invalid parameter!\n");
       return HI_FAILURE;
   }
   
   ret = CA_EncryptSwpk((HI_U32 *)pPlainSwpk, (HI_U32 *)pEncryptedSwpk);
   return ret;
}
