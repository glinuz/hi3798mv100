/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : cx_verify_cmd.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2016-01-20
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef HI_MINIBOOT_SUPPORT
#ifndef HI_ADVCA_FUNCTION_RELEASE
#include "hi_type.h"
#ifdef HI_MINIBOOT_SUPPORT
#include "app.h"
#else
#include "exports.h"
#endif
#include "hi_common.h"
#include "hi_unf_cipher.h"
#include "hi_unf_otp.h"
#include "hi_unf_advca.h"
#include "cx_debug.h"
#include "cx_flash.h"
#include "cx_adp.h"
#include "cx_verify_api.h"



#define MAX_STRLEN(a, b) ((strlen(a) > strlen(b)) ? strlen(a) : strlen(b))


extern HI_U8 CX_CAIMGHEAD_MAGICNUMBER[32];
extern HI_U32 g_MaxBlockSize;

extern HI_S32 HI_SSA_Entry(HI_VOID);
extern HI_S32 HI_SSA_Init(HI_VOID);

#define LOADER_INFOBASIC    "loaderdb"


HI_S32 CX_CMD_CX_Authenticate(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;

    HI_SSA_Init();
    
    ret = HI_SSA_Entry();

    return ret;
}
U_BOOT_CMD(cx_ssa, 1, 1, CX_CMD_CX_Authenticate, "Conax CA security system authenticate", "eg: cx_ssa");

static HI_VOID printf_hex(HI_U8 *pu8Buf, HI_U32 u32Length)
{
    HI_U32 i;

    for(i = 0; i < u32Length; i++)
    {
        if(i % 16 == 0 && i != 0)
        {
            HI_SIMPLEINFO_CA("\n");
        }
        HI_SIMPLEINFO_CA("0x%02X, ", pu8Buf[i]);
    }
    HI_SIMPLEINFO_CA("\n");

    return;
}


HI_S32 CX_EncryptBurnImageByPartName(HI_U8* pImageDDRAddress, HI_CHAR* pPartionName)
{
    HI_S32 ret = 0;
    HI_U32 FlashWriteSize = 0 ;
    HI_CAImgHead_S ImgInfo;
    HI_CA_CryptoInfo_S stCryptoInfo;

    stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    stCryptoInfo.bIsUseKLD = HI_TRUE;
    stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;
        
    memset(&ImgInfo, 0, sizeof(HI_CAImgHead_S));

    memcpy(&ImgInfo, pImageDDRAddress, sizeof(HI_CAImgHead_S));
     if(memcmp(ImgInfo.u8MagicNumber,CX_CAIMGHEAD_MAGICNUMBER,sizeof(ImgInfo.u8MagicNumber)))
    {
        HI_ERR_CA("Img format error\n");

	    HI_INFO_CA("ImgInfo.u8MagicNumber:%s\n", ImgInfo.u8MagicNumber);
        printf_hex(ImgInfo.u8MagicNumber, sizeof(ImgInfo.u8MagicNumber));

		HI_INFO_CA("CX_CAIMGHEAD_MAGICNUMBER:%s\n", CX_CAIMGHEAD_MAGICNUMBER);
        printf_hex(CX_CAIMGHEAD_MAGICNUMBER, sizeof(CX_CAIMGHEAD_MAGICNUMBER));

        return -1;
    }

    CA_ASSERT(HI_CX_ADP_DataEncrypt(&stCryptoInfo,pImageDDRAddress, ImgInfo.u32TotalLen, pImageDDRAddress),ret);
    
    FlashWriteSize = (ImgInfo.u32TotalLen + g_MaxBlockSize - 1)& (~(g_MaxBlockSize -1) );
    memset(pImageDDRAddress + ImgInfo.u32TotalLen ,0xff,FlashWriteSize - ImgInfo.u32TotalLen);
    
    CA_ASSERT(CA_flash_write(pPartionName, 0ULL, FlashWriteSize,pImageDDRAddress),ret);
    HI_INFO_CA("Img has been encrypted by SBP img encrypt process and write back to flash\n");
    
    CA_ASSERT(HI_CX_ADP_DataDecrypt(&stCryptoInfo,pImageDDRAddress,ImgInfo.u32TotalLen,pImageDDRAddress),ret);

    return ret;
}



static HI_S32 CX_CMD_EncryptBurnImage(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_U32 u32RamAddr = IMG_VERIFY_ADDRESS;

    if (argc != 3)
    {
        HI_SIMPLEINFO_CA( "CX_EncryptBurnImage  - burn DDR image by flash_patition_name, for example: CX_EncryptBurnImage  DDRAddress FlashPartition (eg: CX_EncryptBurnImage 0x1000000 kernel)\n\n");
        return 0;
    }

    u32RamAddr = simple_strtoul(argv[1], NULL, 16);
    HI_DEBUG_CA("burn DDR address 0x%x  to flash area %s\n",u32RamAddr,argv[2]);

    HI_CX_VerifyInit();
   
    CA_ASSERT(CX_EncryptBurnImageByPartName((HI_U8*)u32RamAddr,(HI_CHAR*)argv[2]),ret);
    HI_DEBUG_CA("EncryptBurnFlash success\n");

    return ret;
}
U_BOOT_CMD(CX_EncryptBurnImage,3,1,CX_CMD_EncryptBurnImage,\
"Encrypt signed image with R2R Key-ladder and burn DDR image by flash address", "CX_EncryptBurnImage  DDRAddress PartitionName (eg: CX_EncryptBurnImage 0x1000000 kernel)");


HI_S32 CX_EncryptBurnImageByPartAddr(HI_U8* pImageDDRAddress, HI_U32 PartionAddr)
{
    HI_S32 ret = 0;
    HI_U32 FlashWriteSize = 0 ;
    HI_CAImgHead_S ImgInfo;
    HI_CA_CryptoInfo_S stCryptoInfo;

    stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    stCryptoInfo.bIsUseKLD = HI_TRUE;
    stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;

    HI_CX_VerifyInit();
    
    memset(&ImgInfo, 0, sizeof(HI_CAImgHead_S));
    memcpy(&ImgInfo, pImageDDRAddress, sizeof(HI_CAImgHead_S));

    if(memcmp(ImgInfo.u8MagicNumber,CX_CAIMGHEAD_MAGICNUMBER,sizeof(ImgInfo.u8MagicNumber)))
    {
        HI_ERR_CA("Img format error\n");
        return -1;
    }

    CA_ASSERT(HI_CX_ADP_DataEncrypt(&stCryptoInfo, pImageDDRAddress,ImgInfo.u32TotalLen,pImageDDRAddress),ret);

    FlashWriteSize = (ImgInfo.u32TotalLen + g_MaxBlockSize - 1)& (~(g_MaxBlockSize -1) );
    memset(pImageDDRAddress + ImgInfo.u32TotalLen, 0xff, FlashWriteSize - ImgInfo.u32TotalLen);

    CA_ASSERT(CA_flash_write_addr(PartionAddr, FlashWriteSize, pImageDDRAddress),ret);
    HI_INFO_CA("Img has been encrypted by SBP img encrypt process and write back to flash address = 0x%x\n",PartionAddr);

    CA_ASSERT(HI_CX_ADP_DataDecrypt(&stCryptoInfo, pImageDDRAddress,ImgInfo.u32TotalLen,pImageDDRAddress),ret);

    return ret;
}

static HI_S32 CX_CMD_EncryptBurnImageByPartAddr(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_U32 u32RamAddr = IMG_VERIFY_ADDRESS;
    HI_U32 u32PartitonAddr = 0x10000;

    if (argc != 3)
    {
        HI_SIMPLEINFO_CA( "cx_encrypt_burn  - burn DDR image to  flash_patition_name, for example: CX_EncryptBurnSignedImge  DDRAddress FlashPartition (eg: CX_EncryptBurnSignedImge 0x1000000 kernel)\n\n");
        return 0;
    }

    u32RamAddr = simple_strtoul(argv[1], NULL, 16);
    HI_DEBUG_CA("burn DDR address 0x%x\n",u32RamAddr);

    u32PartitonAddr = simple_strtoul(argv[2], NULL, 16);
    HI_DEBUG_CA("flash partiton address 0x%x \n", u32PartitonAddr);

    HI_CX_VerifyInit();
   
    CA_ASSERT(CX_EncryptBurnImageByPartAddr((HI_U8*)u32RamAddr,u32PartitonAddr),ret);
    HI_DEBUG_CA("EncryptBurnFlash success\n");

    return ret;
}
U_BOOT_CMD(CX_EncryptBurnImageEx,3,1,CX_CMD_EncryptBurnImageByPartAddr,\
"Encrypt signed image with R2R Key-ladder and burn DDR image into flash addr", "CX_EncryptBurnImageEx  DDRAddress PartitionAddr (eg: CX_EncryptBurnImageEx 0x1000000 0x10000)");


HI_S32 CX_EncryptBurnAnyData(HI_U8* pImageDDRAddress, HI_U32 u32Len, HI_CHAR* pPartionName)
{
    HI_S32 ret = 0;
    HI_U32 FlashWriteSize = 0 ;
    HI_CA_CryptoInfo_S stCryptoInfo;

    stCryptoInfo.bIsUseKLD = HI_TRUE;
    stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;    

    u32Len = (u32Len + NAND_PAGE_SIZE - 1)& (~(NAND_PAGE_SIZE -1) );
    /*loaderdb should use G2 to encrypt*/
    if (0 == strncmp(pPartionName, (HI_CHAR *)LOADER_INFOBASIC, MAX_STRLEN(pPartionName, LOADER_INFOBASIC)))
    {
        HI_INFO_CA("Img has been encrypted using G2 key\n");
        stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_2;
    }
    else
    {
        HI_INFO_CA("Img has been encrypted using G1 key\n");
        stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    }

    CA_ASSERT(HI_CX_ADP_DataEncrypt(&stCryptoInfo,pImageDDRAddress,u32Len,pImageDDRAddress),ret);

    FlashWriteSize = (u32Len + g_MaxBlockSize - 1)& (~(g_MaxBlockSize -1) );
    memset(pImageDDRAddress + u32Len ,0xff,FlashWriteSize - u32Len);

    CA_ASSERT(CA_flash_write(pPartionName, 0ULL, FlashWriteSize,pImageDDRAddress),ret);
    HI_INFO_CA("Img has been encrypted by SBP img encrypt process and write back to flash\n");

    CA_ASSERT(HI_CX_ADP_DataDecrypt(&stCryptoInfo,pImageDDRAddress,u32Len,pImageDDRAddress),ret);

    return ret;
}

HI_S32 CX_CMD_EncryptBurnData(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_U32 addr = 0;
    HI_U32 len = 0;
    HI_U8* pImageDDRAddress = NULL;
    
    if (argc != 4)
    {
        HI_SIMPLEINFO_CA( "CX_EncryptBurnWithLen  - burn DDR image to flash_patition_name, for example: EncryptBurnFlashEx  DDRAddress DataLen PartitionName (eg: CX_EncryptBurnWithLen 0x1000000 0x80000 logo)\n");
        return 0;
    }
    addr = simple_strtoul(argv[1], NULL, 16);
    pImageDDRAddress = (HI_U8*)addr;
    len =   simple_strtoul(argv[2], NULL, 16);
    len =  (len + NAND_PAGE_SIZE - 1)& (~(NAND_PAGE_SIZE -1) );
    HI_DEBUG_CA("burn DDR address 0x%x  with len = 0x%x to flash area %s\n",addr,len ,argv[3]);

    HI_CX_VerifyInit();
    
    CA_ASSERT(CX_EncryptBurnAnyData(pImageDDRAddress,len,argv[3]),ret);
    HI_DEBUG_CA("EncryptBurnFlashEx success\n");
    return ret;
}
U_BOOT_CMD(CX_EncryptBurnData,4,1,CX_CMD_EncryptBurnData,\
"Encrypt data with R2R Key-ladder and burn data into flash", "CX_EncryptBurnWithLen DDRAddress DataLen PartitionName (eg: CX_EncryptBurnData 0x1000000 0x80000 logo)");


HI_S32 CX_DecryptFlashImage2DDR(HI_CHAR* pPartionName)
{
    HI_S32 ret = 0;
    HI_CAImgHead_S ImgInfo;
    HI_BOOL IsEncrypt = HI_TRUE;
    HI_CA_AuthInfo_S stAuthInfo;

    stAuthInfo.stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    stAuthInfo.stCryptoInfo.bIsUseKLD = HI_TRUE;
    stAuthInfo.stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    stAuthInfo.stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;
    stAuthInfo.eAuthAlg = AUTH_SHA2;     

    CA_CheckPointer(pPartionName);   

    memset(&ImgInfo, 0, sizeof(HI_CAImgHead_S));

    CA_ASSERT(HI_CX_GetFlashImgInfoByName(pPartionName, &stAuthInfo, &ImgInfo, &IsEncrypt),ret);

    CA_ASSERT(CA_flash_read(pPartionName,0ULL, ImgInfo.u32TotalLen, (HI_U8*)IMG_VERIFY_ADDRESS, HI_NULL),ret);
    if (HI_TRUE == IsEncrypt)
    {
        CA_ASSERT(HI_CX_ADP_DataDecrypt(&stAuthInfo.stCryptoInfo, (HI_U8 *)IMG_VERIFY_ADDRESS, ImgInfo.u32TotalLen, (HI_U8 *)IMG_VERIFY_ADDRESS),ret);
    }
    
    if(memcmp(ImgInfo.u8MagicNumber,CX_CAIMGHEAD_MAGICNUMBER,sizeof(ImgInfo.u8MagicNumber)))
    {
        HI_ERR_CA("Img format error\n");
        return -1;
    }
    
    return ret;
}

HI_S32 CX_CMD_DecryptFlashImage2DDRTest(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;

    if (argc != 2)
    {
        HI_SIMPLEINFO_CA( "cx_decrypt_partition  - decrypt flash_partition_name to DDR, for example: DecryptFlashImage kernel\n");
        return 0;
    }
    
    HI_CX_VerifyInit();  
     
    HI_DEBUG_CA("%s: %s\n", __FUNCTION__, argv[1]);
    CA_ASSERT(CX_DecryptFlashImage2DDR((HI_CHAR*)argv[1]),ret);
    HI_DEBUG_CA("%s: success\n", __FUNCTION__);

    return ret;
}
U_BOOT_CMD(cx_decrypt_partition,2,1,CX_CMD_DecryptFlashImage2DDRTest,"decrypt image in flash patition to DDR", "cx_decrypt_partition PartitionName(e.g:cx_decrypt_partition kernel)");



HI_S32 CX_CMD_FlashAuthenticateDebug(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_U32 u32RamAddr = IMG_VERIFY_ADDRESS;
    HI_CA_AuthInfo_S stAuthInfo;

    stAuthInfo.stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    stAuthInfo.stCryptoInfo.bIsUseKLD = HI_TRUE;
    stAuthInfo.stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;    
    stAuthInfo.stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;    
    stAuthInfo.eAuthAlg = AUTH_SHA2;    

    if (argc != 2)
    {
        HI_SIMPLEINFO_CA( "cx_verify  - verify flash_patition_name, for example: cx_verify kernel\n");
        return 0;
    }

    HI_DEBUG_CA("%s: %s\n",__FUNCTION__, argv[1]);
    HI_CX_VerifyInit();
    CA_ASSERT(HI_CX_VerifyFlashPartitionByName((HI_CHAR*)argv[1], u32RamAddr, &stAuthInfo, HI_NULL), ret);
    HI_DEBUG_CA("cx_verify success\n");
    return ret;
}
U_BOOT_CMD(cx_verify, 2, 1, CX_CMD_FlashAuthenticateDebug, \
"verify_test PatitionName", "eg: cx_verify kernel");



HI_S32 CX_CMD_FlashAuthenticateAddrDebug(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_U32 addr = 0;
    HI_U32 u32RamAddr = IMG_VERIFY_ADDRESS;
    HI_CA_AuthInfo_S stAuthInfo;

    stAuthInfo.stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    stAuthInfo.stCryptoInfo.bIsUseKLD = HI_TRUE;
    stAuthInfo.stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    stAuthInfo.stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;    
    stAuthInfo.eAuthAlg = AUTH_SHA2;    

    if (argc != 2)
    {
        HI_SIMPLEINFO_CA("verify_test_ex,for example: verify_test_ex 0x100000\n");
        return 0;
    }

    addr = simple_strtoul(argv[1], NULL, 16);
    HI_DEBUG_CA("%s: 0x%X\n",__FUNCTION__,addr);

    HI_CX_VerifyInit();    
    CA_ASSERT(HI_CX_VerifyFlashPartitionByAddr(addr, u32RamAddr, &stAuthInfo, HI_NULL),ret);
    HI_DEBUG_CA("cx_verify_ex success\n");

    return ret;
}
U_BOOT_CMD(verify_test_ex,2,1,CX_CMD_FlashAuthenticateAddrDebug,"cx_verify_ex flash_patition_Addr", "eg: cx_verify_ex 0x100000");


extern HI_S32 CX_AuthenticateBootPara(HI_VOID);

HI_S32 CX_CMD_AuthenticateBootParaDebug(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;

    if (argc != 1)
    {
        HI_SIMPLEINFO_CA("cx_verify_bootargs- verify bootargs, for example: cx_verify_bootargs\n");
        return 0;
    }

    HI_CX_VerifyInit();
    
    if (HI_SUCCESS == CX_AuthenticateBootPara())
    {
       ret = HI_SUCCESS;
       HI_DEBUG_CA("succeed in verify bootargs\n");
    }
    else
    {
       ret = HI_FAILURE;
       HI_DEBUG_CA("fail to verify bootargs\n");
    }
    
    return ret;
}
U_BOOT_CMD(cx_verify_bootargs,1,1,CX_CMD_AuthenticateBootParaDebug,"conax verify bootargs", "eg: cx_verify_bootargs");

#endif /* #ifndef HI_ADVCA_FUNCTION_RELEASE */
#endif
