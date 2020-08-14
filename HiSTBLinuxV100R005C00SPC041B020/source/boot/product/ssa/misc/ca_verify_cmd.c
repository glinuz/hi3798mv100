/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : ca_verify_cmd.c
Version       : Initial
Author        : Hisilicon hisecurity team
Created       : 2015-12-24
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef HI_MINIBOOT_SUPPORT

#ifndef HI_ADVCA_FUNCTION_RELEASE
#include "hi_type.h"
#include "exports.h"
#include "hi_unf_cipher.h"
#include "hi_unf_otp.h"
#include "hi_unf_advca.h"
#include "ca_verify_api.h"
#include "ca_debug.h"
#include "ca_flash.h"
#include "hi_common.h"
#include "ca_adp.h"
#include "ca_cfg.h"

typedef union
{
    struct 
    {
        HI_U8 reserved1        : 5;//[0:4]
        HI_U8 bload_mode_sel   : 1; //[5]
        HI_U8 reserved2        : 2; //[6:7]
    }bits;
    HI_U8 u8;
}CA_V300_BLOAD_MODE_U;

#define BLOAD_MODE_SEL_ADDR     0x03
#define BOOT_BASIC          "boot"

#if defined(CHIP_TYPE_hi3798cv200) \
   || defined(CHIP_TYPE_hi3798mv200) \
   || defined(CHIP_TYPE_hi3798mv200_a)
#define NEW_BOOT_FORMAT
#elif defined (CHIP_TYPE_hi3716cv200) \
   || defined (CHIP_TYPE_hi3716mv400) \
   || defined (CHIP_TYPE_hi3716mv410) \
   || defined (CHIP_TYPE_hi3716mv420) \
   || defined (CHIP_TYPE_hi3718cv100) \
   || defined (CHIP_TYPE_hi3718mv100) \
   || defined (CHIP_TYPE_hi3719cv100) \
   || defined (CHIP_TYPE_hi3719mv100) \
   || defined (CHIP_TYPE_hi3798mv100) \
   || defined (CHIP_TYPE_hi3796mv100)
#define OLD_BOOT_FORMAT
#else
#error "Not support chipset!!!"
#endif

#define BLPK_LEN           0x10
#define BOOT_IMG_LEN       0x100000

#ifdef OLD_BOOT_FORMAT
#define BLPK_SPACE_OFFSET  0x1800
#define BLPK_PARAM_OFFSET  0x304
#define BLPK_SPACE_LEN     0x20
#else
#define AUXAREALEN_ADDR             (0x218)

#define EXTERN_RAS_PUB_KEY_LENGTH   (0x200)
#define PARMS_AREA_OFFSET           (0x400)
#define UNCKECKED_AREA_HISI_OFFSET  (0x3000 - 0X40)
#define AUXILIARY_CODE_AREA_OFFSET  (0x3000)
#define CHECK_AREA_LEN_ADDR         (PARMS_AREA_OFFSET + 4)
#define CHECK_AREA_ENC_FLAG_ADDR    (UNCKECKED_AREA_HISI_OFFSET + 12)
#define BLPK_SPACE_ADDR             (UNCKECKED_AREA_HISI_OFFSET + 16)
#define BOOT_AREA_LEN_ADDR          (PARMS_AREA_OFFSET + 4 + 4)
#endif

#define MAX_STRLEN(a, b) ((strlen(a) > strlen(b)) ? strlen(a) : strlen(b))


extern HI_U32 g_EnvFlashAddr;
extern HI_U32 g_EnvFlashPartionSize;
extern HI_U8 g_customer_rsa_public_key_N[256];
extern HI_U32  g_customer_rsa_public_key_E;

extern HI_FLASH_TYPE_E CA_get_env_flash_type(HI_VOID);
extern HI_S32 CA_Search_SignHeader(HI_CHAR *PartitionSignName, HI_CASignImageTail_S *pSignImageInfo, HI_U32 *pOffset);
extern HI_S32 CA_GetImageData(HI_U32 u32MemAddr, HI_CHAR *PartitionImageName, HI_CASignImageTail_S *pSignImageInfo, HI_CA_NEW_SignParam_S *pSignParam);
HI_S32 Debug_CASignImageTail_Value(HI_CASignImageTail_S *pCASignTail);
extern unsigned long DVB_CRC32(unsigned char *buf, int length);
extern HI_S32 HI_Android_Authenticate(HI_VOID);

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

HI_VOID compare_hex(char *title, HI_U8 *pu8Src, HI_U8 *pu8Dst, HI_U32 u32Length)
{
    HI_U32 i;

    for(i = 0; i < u32Length; i++)
    {
        if(pu8Src[i] != pu8Dst[i])
        {
           HI_SIMPLEINFO_CA("\033[0;31m" "%s failed!\n" "\033[0m", title);
           return;
        }
    }
    HI_SIMPLEINFO_CA("%s success!\n", title);
    return;
}


/* normal signing test */
HI_S32 CA_CMD_VerifyImageSignature(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_U32 u32MemAddr = IMG_VERIFY_ADDRESS;

    if(argc >= 4)
    {
        HI_U32 offset = 0;

        offset = (HI_U32)simple_strtoul((const char *)argv[3], NULL, 16);
        CA_ASSERT(HI_CA_Verify_Signature(u32MemAddr, (HI_CHAR*)argv[1], (HI_CHAR*)argv[2], offset),ret);
    }
    else if(argc >= 3)
    {
        CA_ASSERT(HI_CA_Verify_Signature(u32MemAddr, (HI_CHAR*)argv[1], (HI_CHAR*)argv[2], 0),ret);
    }
    else if(argc >= 2)
    {
        CA_ASSERT(HI_CA_Verify_Signature(u32MemAddr, (HI_CHAR*)argv[1], (HI_CHAR*)argv[1], 0),ret);
    }
    else
    {
        HI_ERR_CA("Error Input, for example: ca_common_verify_image_signature imgepartionname signpartionname signoffsetinpartion\n");
    }

    return ret;
}

U_BOOT_CMD(ca_verify,4,1,CA_CMD_VerifyImageSignature, \
"verify image signature with tail mode","for example: ca_verify imgepartionname signpartionname signoffsetinpartion");


HI_S32 CA_CMD_RandomVerifyImage(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_U32 u32MemAddr = IMG_VERIFY_ADDRESS;

    if(argc >= 4)
    {
        HI_U32 offset = 0;

        offset = (HI_U32)simple_strtoul((const char *)argv[3], NULL, 16);
        CA_ASSERT(HI_CA_Verify_Signature_Ex(u32MemAddr, (HI_CHAR*)argv[1], (HI_CHAR*)argv[2], offset),ret);
    }
    else if(argc >= 3)
    {
        CA_ASSERT(HI_CA_Verify_Signature_Ex(u32MemAddr, (HI_CHAR*)argv[1], (HI_CHAR*)argv[2], 0),ret);
    }
    else if(argc >= 2)
    {
        CA_ASSERT(HI_CA_Verify_Signature_Ex(u32MemAddr, (HI_CHAR*)argv[1], (HI_CHAR*)argv[1], 0),ret);
    }
    else
    {
        HI_ERR_CA("Error Input\n");
    }

    return ret;
}

U_BOOT_CMD(ca_verify_ran,4,1,CA_CMD_RandomVerifyImage, \
"randomly verify image signature with tail mode","for example: ca_verify_ran imgepartionname signpartionname signoffsetinpartion");


HI_S32 CA_Parse_SubCommand(HI_U8 *string, HI_U32 length, HI_U8 *ImageName, HI_U8 *SignName, HI_U32 *SignOffset)
{
    int ret = 0;
    HI_U8 *pImageoffset, *pSignimageoffset, *pSignloacationoffset;
    HI_U8 deliver[] = ",";

    pImageoffset = string;

    pSignimageoffset = (HI_U8 *)strstr((const char *)string, (const char *)deliver);
    if(pSignimageoffset == HI_NULL)
    {
        HI_ERR_CA("Can not find , in string:%s\n", string);
        return -1;
    }
    //Get ImageName
    memcpy(ImageName, pImageoffset, (pSignimageoffset - pImageoffset));

    pSignimageoffset ++; //jump ;
    pSignloacationoffset = (HI_U8 *)strstr((const char *)pSignimageoffset, (const char *)deliver);
    if(pSignloacationoffset == HI_NULL)
    {
        HI_ERR_CA("Can not find , in pSignimageoffset:%s\n", pSignimageoffset);
        return -1;
    }
    //Get SiagnName
    memcpy(SignName, pSignimageoffset, (pSignloacationoffset - pSignimageoffset));

    //Get Siagn loaction offset
    pSignloacationoffset ++; //jump ;
    *SignOffset = (HI_U32)simple_strtoul((const char *)pSignloacationoffset, NULL, 16);

    return ret;
}

HI_S32 CA_CMD_VerifyByEnvCmd(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_CHAR  *str = NULL;        /* used to set string properties */
    int strlength = 0, offset = 0;
    HI_CHAR head[] = "sign:";
    HI_U8 *pParamoffset, *pImageoffset;

    HI_U8  ImageName[50] = {0};
    HI_U8  SignName[50] = {0};
    HI_U32 SignOffset = 0;
    HI_U32 u32MemAddr = IMG_VERIFY_ADDRESS;


    str = getenv("signature_check");
    if (str == NULL)
    {
        HI_ERR_CA("can not find boot argures:signature_check\n");
        return HI_FAILURE;
    }
    strlength = strlen(str);
    if(strlength > 256)
    {
        HI_ERR_CA("signature_check is too long > 256,error\n");
        return -1;
    }

    //parse as: setenv signature_check 'sign:kernel,kernel,0x280000 sign:fs,fs,0x600000 sign:subfs,subfs,0x400000'
    offset = 0;
    while(offset <= strlength)
    {
        pParamoffset = (HI_U8 *)strstr((const char *)(str + offset), (const char *)head);
        if(pParamoffset == HI_NULL)
        {
            break;
        }

        //To get the Image,Image_Signature,SignatureOffset
        pImageoffset = pParamoffset + strlen(head);

        offset = pImageoffset - (HI_U8 *)str;//change offset
        ret = CA_Parse_SubCommand(pImageoffset, strlen((HI_CHAR *)pImageoffset), ImageName, SignName, &SignOffset);
        if (ret != 0)
        {
            HI_ERR_CA("Error to parse data\n");
            return -1;
        }
        HI_INFO_CA("verify image:%s, sign:%s, offset:0x%x\n", ImageName, SignName, SignOffset);
        ret = HI_CA_Verify_Signature(u32MemAddr, (HI_CHAR *)ImageName, (HI_CHAR *)SignName, SignOffset);
        if (ret != 0)
        {
            HI_ERR_CA("Verify image %s failed!\n", ImageName);
            return -1;
        }
    }
    HI_INFO_CA("Finish\n");

    return 0;
}

U_BOOT_CMD(ca_verify_by_env,2,1,CA_CMD_VerifyByEnvCmd, \
"verify image from command line,please pre-set signature_check 'Image,Image_Signature,SignatureOffset'","for example: ca_verify_by_env");


HI_S32 CA_VerifyEncryptBurnImage(HI_CHAR *PartitionImagenName, HI_CHAR *PartitionSignName, HI_U32 offset)
{
    HI_S32 ret = 0;
    HI_U32 EncryptLength = 0;
    HI_CASignImageTail_S SignImageInfo;
    HI_CA_NEW_SignParam_S SignParam;
    HI_U32 PartionSize;
    HI_U32 PageSize;
    HI_U32 BlockSize;
    HI_U32 OobSize;
    HI_U32 u32MemAddr = IMG_VERIFY_ADDRESS;
    HI_CA_CryptoInfo_S stCryptoInfo;

    //Verify Partition
    CA_CheckPointer(PartitionImagenName);
    CA_CheckPointer(PartitionSignName);

    CA_ASSERT(CA_Search_SignHeader(PartitionSignName, &SignImageInfo, &offset),ret);
    Debug_CASignImageTail_Value(&SignImageInfo);
    //Get image data
    CA_ASSERT(CA_GetImageData(u32MemAddr, PartitionImagenName, &SignImageInfo, &SignParam),ret);
    //Verify image signature
    CA_ASSERT(HI_CA_ADP_VerifyImage((HI_U32)SignParam.pImageData, SignParam.ImageDataLength, SignParam.pCASignTail->u8Signature, AUTH_SHA2),ret);
    HI_INFO_CA("Check Authenitication is ok\n");

    if(SignImageInfo.u32IsNeedEncrypt == 0)
    {
        HI_ERR_CA("Do not need to encrypt image\n");
        return 0;
    }
    else if(SignImageInfo.u32IsEncrypted == 1)
    {
        HI_ERR_CA("Image pariton is already encrypted\n");
        return 0;
    }

    if(strncmp(PartitionImagenName, PartitionSignName, MAX_STRLEN(PartitionImagenName, PartitionSignName)) == 0)
    {
        HI_ERR_CA("Error Image Partition and SignImagePartion are the same(%s)\n", PartitionSignName);
        return -1;
    }
    if(SignImageInfo.u32IsYaffsImage != 0)
    {
        HI_ERR_CA("Error Yaffs Image can not be encrypted\n");
        return -1;
    }

    //User R2R to encrypt Image Data Partition
    CA_ASSERT(CA_flash_getinfo(PartitionImagenName, &PartionSize, &BlockSize, &PageSize, &OobSize),ret);
    HI_INFO_CA("NAND BlockSize:0x%x, PageSize:0x%x, OobSize:0x%x\n", BlockSize, PageSize, OobSize);
    EncryptLength = (SignParam.ImageDataLength + BlockSize - 1) & (~(BlockSize -1));
    
    stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    stCryptoInfo.bIsUseKLD  = HI_TRUE;
    stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;    
    CA_ASSERT(HI_CA_ADP_DataEncrypt(&stCryptoInfo,(HI_U8*)SignParam.pImageData, EncryptLength, (HI_U8*)SignParam.pImageData),ret);

    //write data back to flash
    CA_ASSERT(CA_flash_write(PartitionImagenName, 0x00, EncryptLength, (HI_U8*)SignParam.pImageData),ret);
    //Set SignHead Encrypt flag to true
    SignImageInfo.u32IsEncrypted = 1;
    SignImageInfo.CRC32 = DVB_CRC32((HI_U8*)(&SignImageInfo), (sizeof(HI_CASignImageTail_S) - 4));
    HI_INFO_CA("new crc32:%x\n", SignImageInfo.CRC32);
    //write data back to flash
    CA_ASSERT(CA_flash_write(PartitionSignName, 0x00, BlockSize, (HI_U8*)(&SignImageInfo)),ret);

    return ret;
}


HI_S32 CA_CMD_VerifyEncryptBurnCommand(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;

    if(argc >= 3)
    {
        CA_ASSERT(CA_VerifyEncryptBurnImage((HI_CHAR*)argv[1], (HI_CHAR*)argv[2], 0),ret);
    }
    else
    {
        HI_ERR_CA("Error Input\n");
    }

    return ret;
}

U_BOOT_CMD(ca_VerifyEncryptBurn,3,1,CA_CMD_VerifyEncryptBurnCommand, \
"Verify encrypt and burn image to partition","for example: ca_VerifyEncryptBurn PartName PartSigName");


HI_S32 CA_CMD_VerifyBootParaDebug(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_U32 u32MemAddr;

    HI_SIMPLEINFO_CA("g_EnvFlashAddr:0x%x, g_EnvFlashPartionSize:0x%x\n", g_EnvFlashAddr, g_EnvFlashPartionSize);
    u32MemAddr = IMG_VERIFY_ADDRESS;
    
    ret = HI_CA_Verify_BootPara(u32MemAddr, g_EnvFlashAddr, g_EnvFlashPartionSize);
    if(HI_FAILURE != ret)
    {
        HI_DEBUG_CA("verify success\n");
    }
    else
    {
        HI_DEBUG_CA("verify failed\n");
    }

    return ret;
}
U_BOOT_CMD(ca_verify_bootargs_by_env,2,1,CA_CMD_VerifyBootParaDebug, \
"verify bootargs from env ","for example: ca_verify_bootargs_by_env ");

static HI_U8 g_BootImg[BOOT_IMG_LEN] = {0};
static HI_U8 g_customer_blpk[BLPK_LEN] = {0};

HI_S32 CA_EncryptBoot_Test(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32                  ret = 0;
    HI_U8                   *pBlpk;
    HI_U8                   *pBootImage;
    HI_U8                   EncryptBlpk[BLPK_LEN];
    HI_UNF_CIPHER_CTRL_S    CipherCtrl;
    HI_HANDLE               blpkHandle = 0;
    HI_U32                  u32CheckedAreaLen = 0;
    HI_U32                  i = 0;
    static HI_U32           u32MmzPhyAddr = 0;
    HI_UNF_CIPHER_ATTS_S stCipherAtts;

#ifdef OLD_BOOT_FORMAT
    HI_U32          u32CheckedAreaOffset = 0;
	HI_U8           BlpkClear;
    HI_U8           u8CheckedAreaOffset[4] = {0};
    HI_U8           u8CheckedAreaLen[4] = {0};
#else
    HI_U32 AuxAreaLen = 0;
    HI_U32 Checked_Area_Off = 0;
    HI_U32 Boot_Area_Len = 0;
    HI_U32 Checked_area_enc_flag = 0;  
#endif  

    CA_ASSERT(HI_CA_GetBLPKInfor(g_customer_blpk), ret);
    /* read all valid data of boot partition  */
    CA_ASSERT(CA_flash_read(BOOT_BASIC, 0ULL, BOOT_IMG_LEN, (HI_U8*)g_BootImg, HI_NULL), ret);

#ifdef OLD_BOOT_FORMAT
    /*read the offset and length of the param area*/
    memcpy(u8CheckedAreaOffset, g_BootImg + BLPK_PARAM_OFFSET, 4);
    memcpy(u8CheckedAreaLen, g_BootImg + BLPK_PARAM_OFFSET + 8, 4);

    u32CheckedAreaOffset = u8CheckedAreaOffset[0];
    u32CheckedAreaOffset += u8CheckedAreaOffset[1] << 8;
    u32CheckedAreaOffset += u8CheckedAreaOffset[2] << 16;
    u32CheckedAreaOffset += u8CheckedAreaOffset[3] << 24;

    u32CheckedAreaLen = u8CheckedAreaLen[0];
    u32CheckedAreaLen += u8CheckedAreaLen[1] << 8;
    u32CheckedAreaLen += u8CheckedAreaLen[2] << 16;
    u32CheckedAreaLen += u8CheckedAreaLen[3] << 24;

    HI_INFO_CA("u32CheckedAreaOffset = 0x%x, u32CheckedAreaLen = 0x%x\n", u32CheckedAreaOffset, u32CheckedAreaLen);

    if (0 != (u32CheckedAreaLen % 0x100))
    {
        HI_ERR_CA("checked area length invalid!\n");
        return HI_FAILURE;
    }

   /* read blpk and its clear flag */
    pBlpk = g_BootImg + BLPK_SPACE_OFFSET; 
    pBootImage = g_BootImg + BLPK_SPACE_OFFSET + u32CheckedAreaOffset;
    BlpkClear = pBlpk[BLPK_LEN];
    if (0x48 == BlpkClear)
    {
        HI_ERR_CA("BOOT image have been encrypted, Finish!\n");
        return HI_SUCCESS;
    }
#else
    AuxAreaLen = *((HI_U32 *) (g_BootImg + AUXAREALEN_ADDR));                   
    
    Checked_Area_Off  = *((HI_U32 *) (g_BootImg + PARMS_AREA_OFFSET));
    u32CheckedAreaLen = *((HI_U32 *) (g_BootImg + CHECK_AREA_LEN_ADDR));
    Boot_Area_Len     = *((HI_U32 *) (g_BootImg + BOOT_AREA_LEN_ADDR));
    Checked_area_enc_flag = *((HI_U32 *) (g_BootImg + CHECK_AREA_ENC_FLAG_ADDR));
    pBlpk = g_BootImg + BLPK_SPACE_ADDR;    
    pBootImage = g_BootImg + AUXILIARY_CODE_AREA_OFFSET + AuxAreaLen + Checked_Area_Off;
    
    HI_INFO_CA("AuxAreaLen:0x%x,Checked_Area_Off:0x%x\n", AuxAreaLen, Checked_Area_Off);
    HI_INFO_CA("u32CheckedAreaLen:0x%x,Boot_Area_Len:0x%x,Checked_area_enc_flag:0x%x\n", u32CheckedAreaLen,Boot_Area_Len,Checked_area_enc_flag);

    if(Checked_area_enc_flag == 0x2a13c812)
    {
        HI_ERR_CA("BOOT image have been encrypted, Finish!\n");
        return HI_SUCCESS;
    }    
#endif
    HI_SIMPLEINFO_CA("clear key:");
    for (i = 0; i < BLPK_LEN; i++)
    {
        HI_SIMPLEINFO_CA("%02x ", g_customer_blpk[i]);
    }
    HI_SIMPLEINFO_CA("\n");

    ret = HI_UNF_CIPHER_Init();
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("HI_UNF_CIPHER_Init failed:%#x\n",ret);
        return HI_FAILURE;
    }

    memset(&stCipherAtts, 0, sizeof(HI_UNF_CIPHER_ATTS_S));
    stCipherAtts.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    ret = HI_UNF_CIPHER_CreateHandle(&blpkHandle, &stCipherAtts);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("HI_UNF_CIPHER_CreateHandle failed:%#x\n",ret);
        goto _CIPHER_CLOSE;
    }

    CipherCtrl.bKeyByCA = HI_FALSE;
    CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
    CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;

    memcpy(CipherCtrl.u32Key, g_customer_blpk, BLPK_LEN);
    memset(CipherCtrl.u32IV, 0, 16);

    ret = HI_UNF_CIPHER_ConfigHandle(blpkHandle, &CipherCtrl);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("HI_UNF_CIPHER_ConfigHandle failed:%#x\n",ret);
        goto _CIPHER_DESTROY;
    }

    /*************************************************************/
    ret = HI_MEM_Alloc(&u32MmzPhyAddr, MAX_DATA_LEN);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("malloc input memory failed!\n");
        goto _CIPHER_DESTROY;
    }
    memset((void *)u32MmzPhyAddr, 0, BOOT_IMG_LEN);

    /*************************************************************/
    memcpy((void *)u32MmzPhyAddr, pBootImage, u32CheckedAreaLen);

    ret = HI_UNF_CIPHER_Encrypt(blpkHandle, u32MmzPhyAddr, u32MmzPhyAddr, u32CheckedAreaLen);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("HI_UNF_CIPHER_Encrypt failed:%#x\n", ret);
        goto _CIPHER_DESTROY;
    }
    memcpy(pBootImage, (HI_U8 *)u32MmzPhyAddr, u32CheckedAreaLen);

    /* encrypt blpk */
    ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("HI_UNF_ADVCA_Init failed:%#x\n", ret);
        goto _CIPHER_DESTROY;
    }

    ret = HI_UNF_ADVCA_EncryptSWPK(g_customer_blpk,EncryptBlpk);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to encrypt SWPK:%#x\n", ret);
        goto _ADVCA_DEINIT;
    }

    HI_SIMPLEINFO_CA("encrypted key:");
    for (i = 0; i < BLPK_LEN; i++)
    {
        HI_SIMPLEINFO_CA("%02x ", EncryptBlpk[i]);
    }
    HI_SIMPLEINFO_CA("\n");

    /* write encrypted blpk and set flag*/
    memcpy(pBlpk,EncryptBlpk,BLPK_LEN);

#ifdef OLD_BOOT_FORMAT
    pBlpk[BLPK_LEN] = 0x48;
#else
    g_BootImg[CHECK_AREA_ENC_FLAG_ADDR]     = 0X12;
    g_BootImg[CHECK_AREA_ENC_FLAG_ADDR + 1] = 0XC8;
    g_BootImg[CHECK_AREA_ENC_FLAG_ADDR + 2] = 0X13;
    g_BootImg[CHECK_AREA_ENC_FLAG_ADDR + 3] = 0X2A;
#endif

    /* write all valid data of boot partition  */
    ret = CA_flash_write(BOOT_BASIC, 0ULL, BOOT_IMG_LEN, (HI_U8*)g_BootImg);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("CA_flash_write failed:%#x\n", ret);
        goto _ADVCA_DEINIT;
    }

    ret = HI_SUCCESS;

_ADVCA_DEINIT:
    HI_UNF_ADVCA_DeInit();
_CIPHER_DESTROY:
    HI_UNF_CIPHER_DestroyHandle(blpkHandle);
_CIPHER_CLOSE:
    HI_UNF_CIPHER_DeInit();

    if (HI_SUCCESS == ret)
    {
        HI_INFO_CA("ca_encryptboot blpk SUCCESS!\n");
    }
    else
    {
        HI_INFO_CA("ca_encryptboot blpk FAIL!\n");
    }
    return ret;
}
U_BOOT_CMD(ca_encryptboot, 1, 1, CA_EncryptBoot_Test, "CA Encrypt Boot ", "eg: ca_encryptboot");



HI_S32 CA_GetExternRsaKey(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_S32 index = 0;
    HI_U8 buffer[512] = {0};
    HI_U8 command[512] = {0};
    HI_CHIP_TYPE_E enChipType;
    HI_CHIP_VERSION_E u32ChipVersion = 0;
    HI_FLASH_TYPE_E EnvFlashType;

    if (argc != 2)
    {
       HI_ERR_CA("Error Input\n");
       HI_ERR_CA("usage: ca_get_extern_rsa_key  fastboot_partition_name\n");
       return HI_FAILURE;
    }

    HI_DRV_SYS_GetChipVersion( &enChipType, &u32ChipVersion );

    EnvFlashType = CA_get_env_flash_type();
    if (EnvFlashType == HI_FLASH_TYPE_SPI_0 || EnvFlashType == HI_FLASH_TYPE_NAND_0)
    {
        CA_ASSERT(CA_flash_read(argv[1], 0ULL, 512, buffer, HI_NULL), ret);
    }
    else if (EnvFlashType == HI_FLASH_TYPE_EMMC_0 || EnvFlashType == HI_FLASH_TYPE_SD)
    {
       memset(command, 0, 256);
#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
       if (((HI_CHIP_TYPE_HI3719M == enChipType) && (HI_CHIP_VERSION_V100 == u32ChipVersion)) || \
          ((HI_CHIP_TYPE_HI3798C == enChipType) && (HI_CHIP_VERSION_V200 == u32ChipVersion)) || \
          ((HI_CHIP_TYPE_HI3798M == enChipType) && (HI_CHIP_VERSION_V200 == u32ChipVersion)) || \
          ((HI_CHIP_TYPE_HI3798M_A == enChipType) && (HI_CHIP_VERSION_V200 == u32ChipVersion)))
       {
           snprintf((char *)command, sizeof(command), "mmc read 0 0x%x 0x0 0x1 ", buffer);
       }
       else
#endif
       {
           snprintf((char *)command, sizeof(command), "mmc read 0 0x%x 0x1 0x1 ", buffer);
       }
       run_command((const char *)command, 0);
    }
    else
    {
        HI_ERR_CA("\nget extern rsa key fail!\n");
        return HI_FAILURE;
    }

    for (; index < 256; index++)
    {
       g_customer_rsa_public_key_N[index] = buffer[index];
    }
    g_customer_rsa_public_key_E = (buffer[508] << 24) + (buffer[509] << 16) + (buffer[510] << 8) + buffer[511];

//z00213260, for test
    HI_SIMPLEINFO_CA("RSA Module:\n");
    printf_hex(g_customer_rsa_public_key_N, sizeof(g_customer_rsa_public_key_N));

    HI_SIMPLEINFO_CA("RSA Exponent:\n");
    printf_hex((HI_U8 *)&g_customer_rsa_public_key_E, sizeof(g_customer_rsa_public_key_E));

    HI_SIMPLEINFO_CA("\nget extern rsa key success!\n");

    return ret;
}
U_BOOT_CMD(ca_get_extern_rsa_key,2,1,CA_GetExternRsaKey, "get external rsa key","for example: ca_get_extern_rsa_key fastboot");


HI_S32 CA_CMD_VerifyBootParaByPartName(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_U32 u32MemAddr = IMG_VERIFY_ADDRESS;
    HI_FLASH_TYPE_E EnvFlashType = CA_get_env_flash_type();

    if(argc == 2)
    {
        CA_ASSERT(CA_flash_GetSize(argv[1], &g_EnvFlashPartionSize), ret);
        CA_ASSERT(CA_flash_GetFlashAddressFromParitonName(EnvFlashType, argv[1], &g_EnvFlashAddr), ret);

        HI_SIMPLEINFO_CA("g_EnvFlashAddr:0x%x, g_EnvFlashPartionSize:0x%x\n", g_EnvFlashAddr, g_EnvFlashPartionSize);
        CA_ASSERT(HI_CA_Verify_BootPara(u32MemAddr, g_EnvFlashAddr, g_EnvFlashPartionSize), ret);
        HI_DEBUG_CA("verify success\n");
    }
    else
    {
        HI_ERR_CA("Error Input\n");
        HI_ERR_CA("usage: common_verify_bootargs  bootargs_partition_name\n");
        ret = HI_FAILURE;
    }

    return ret;
}
U_BOOT_CMD(ca_verify_bootpara,2,1,CA_CMD_VerifyBootParaByPartName, \
"verify bootpara from flash partition","for example: verify_bootargs  bootargs_partition_name");


HI_S32 CA_CMD_AdvcaAuthByPartName(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_U32 u32RamAddr = IMG_VERIFY_ADDRESS;
    HI_CA_AuthInfo_S stAuthInfo;

    stAuthInfo.stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    stAuthInfo.stCryptoInfo.bIsUseKLD  = HI_TRUE;
    stAuthInfo.stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    stAuthInfo.stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;    
    stAuthInfo.eAuthAlg = AUTH_SHA2;  

    if (argc != 2)
    {
        HI_SIMPLEINFO_CA( "advca_verify  - advca_verify flash_patition_name, for example: advca_verify bootpara\n");
        return 0;
    }

    HI_DEBUG_CA("advca_verify: %s\n",argv[1]);
    CA_ASSERT(HI_CA_AdvcaVerifyByPartName((HI_U8*)argv[1], u32RamAddr, &stAuthInfo, NULL), ret);
    HI_DEBUG_CA("advca_verify success\n");
    return ret;
}
U_BOOT_CMD(advca_verify, 2, 1, CA_CMD_AdvcaAuthByPartName, \
"advca_verify flash_patition_name", "eg: advca_verify kernel");


HI_S32 CA_CMD_AdvcaAuthByPartAddr(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_U32 u32Addr = 0;
    HI_U32 u32RamAddr = IMG_VERIFY_ADDRESS;
    HI_CA_AuthInfo_S stAuthInfo;

    stAuthInfo.stCryptoInfo.enKeyGroup = HI_CA_KEY_GROUP_1;
    stAuthInfo.stCryptoInfo.bIsUseKLD = HI_TRUE;
    stAuthInfo.stCryptoInfo.enKLDAlg = HI_UNF_ADVCA_ALG_TYPE_AES;
    stAuthInfo.stCryptoInfo.enCipherAlg = HI_UNF_CIPHER_ALG_AES;    
    stAuthInfo.eAuthAlg = AUTH_SHA2;  

    if (argc != 2)
    {
        HI_SIMPLEINFO_CA( "advca_verify_ex  - advca_verify_ex flash_patition_name, for example: advca_verify_ex 0x100000\n");
        return 0;
    }

    u32Addr = simple_strtoul(argv[1], NULL, 16);
    HI_DEBUG_CA("%s: Flash addr is 0x%X\n",__FUNCTION__, u32Addr);
    
    CA_ASSERT(HI_CA_AdvcaVerifyByPartAddr(u32Addr, u32RamAddr, &stAuthInfo, NULL), ret);
    HI_DEBUG_CA("advca_verify_ex success\n");
    return ret;
}
U_BOOT_CMD(advca_verify_ex, 2, 1, CA_CMD_AdvcaAuthByPartAddr, \
"advca_verify_ex flash_patition_name", "eg: advca_verify_ex 0x100000");


HI_S32 CA_CMD_Android_Authenticate(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32 ret = 0;
    
    ret = HI_Android_Authenticate();

    return ret;
}

U_BOOT_CMD(ca_verify_android, 2, 1, CA_CMD_Android_Authenticate, \
"verify android system: bootargs, recovory, kernel, system...", "for example: ca_verify_android");

#endif /* #ifndef HI_ADVCA_FUNCTION_RELEASE */
#endif