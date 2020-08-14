/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_blpk.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "hi_type.h"
#include "hi_unf_advca.h"
#include "hi_unf_ecs.h"
#include "hi_flash.h"
#include "sample_ca_common.h"

/***************************** Macro Definition ******************************/
#if defined(CHIP_TYPE_hi3798cv200) \
  || defined(CHIP_TYPE_hi3798mv200)  \
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
   || defined (CHIP_TYPE_hi3796mv100) \
   || defined (CHIP_TYPE_hi3716dv100)
#define OLD_BOOT_FORMAT
#else
#error "Not support chipset!!!"
#endif

#define BLPK_LEN           0x10
#define BOOT_IMG_LEN       0x100000
#define SIGNATURE_LEN      0x100

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

/*************************** Structure Definition ****************************/

static HI_U8 g_BootImg[BOOT_IMG_LEN] = {0};

static HI_U8 g_BLPK[BLPK_LEN] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};

/******************************* API declaration *****************************/
HI_S32 main(HI_S32 argc,HI_CHAR** argv)
{
    HI_HANDLE       hFlash;
    HI_U8           *pBlpk;
    HI_U8           *pBootImage;
    HI_U8           EncryptBlpk[BLPK_LEN];
    HI_S32          Ret;
    HI_UNF_CIPHER_CTRL_S CipherCtrl;
    HI_HANDLE       blpkHandle = 0;

    HI_U32          u32CheckedAreaLen = 0;
    HI_U32          Testcached = 0;
    HI_U32          cipherAddrPhy;
    HI_U8           *pCipherAddrVir;
    HI_U32          cipherOutAddrPhy;
    HI_U8           *pCipherOutAddrVir;
    HI_S32          blpkFd = -1;
    HI_S32          len = 0;
    HI_U32          i = 0;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;
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

    memset(&stCipherAttr, 0, sizeof(stCipherAttr));
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;

    Ret = HI_SYS_Init();
    if (HI_FAILURE == Ret)
    {
        HI_DEBUG_ADVCA("HI_SYS_Init Failed:%#x\n",Ret);
        return Ret;
    }

    /* open flash */
#ifdef ANDROID
    hFlash = HI_Flash_OpenByName("fastboot");
#else
    hFlash = HI_Flash_OpenByName("boot");
#endif
    if (0xffffffff == hFlash)
    {
        HI_DEBUG_ADVCA("HI_Flash_Open failed\n");
        Ret = HI_FAILURE;
        goto _SYS_DEINIT;
    }

    /* read all valid data of boot partition  */
    Ret = HI_Flash_Read(hFlash,(HI_U64)0, g_BootImg, BOOT_IMG_LEN, HI_FLASH_RW_FLAG_RAW);
    if (HI_FAILURE == Ret)
    {
        HI_DEBUG_ADVCA("HI_Flash_Read Failed:%#x\n",Ret);
        goto _FLASH_CLOSE;
    }
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

    HI_DEBUG_ADVCA("u32CheckedAreaOffset = 0x%x, u32CheckedAreaLen = 0x%x\n", u32CheckedAreaOffset, u32CheckedAreaLen);

    if (0 != (u32CheckedAreaLen % 0x100))
    {
        HI_DEBUG_ADVCA("checked area length invalid!\n");
        Ret = HI_FAILURE;
        goto _FLASH_CLOSE;
    }

    /* read blpk and its clear flag */
    pBlpk = g_BootImg + BLPK_SPACE_OFFSET;
    pBootImage = g_BootImg + BLPK_SPACE_OFFSET + u32CheckedAreaOffset;
    BlpkClear = pBlpk[BLPK_LEN];
    if (0x48 == BlpkClear)
    {
        HI_DEBUG_ADVCA("blpk is already encrypted\n");
        (HI_VOID)HI_Flash_Close(hFlash);
        (HI_VOID)HI_SYS_DeInit();
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

    HI_DEBUG_ADVCA("AuxAreaLen:0x%x,Checked_Area_Off:0x%x\n", AuxAreaLen, Checked_Area_Off);
    HI_DEBUG_ADVCA("u32CheckedAreaLen:0x%x,Boot_Area_Len:0x%x,Checked_area_enc_flag:0x%x\n", u32CheckedAreaLen,Boot_Area_Len,Checked_area_enc_flag);

#ifdef ANDROID
    blpkFd = open ("org_boot.bin", O_RDWR | O_CREAT, 0666);
#else
    blpkFd = open ("org_boot.bin", O_RDWR | O_CREAT);
#endif

    len = write(blpkFd, g_BootImg, AUXILIARY_CODE_AREA_OFFSET + AuxAreaLen + Boot_Area_Len);

    if(Checked_area_enc_flag == 0x2a13c812)
    {
        printf("BOOT image have been encrypted, Finish!\n");
        close(blpkFd);
        (HI_VOID)HI_Flash_Close(hFlash);
        (HI_VOID)HI_SYS_DeInit();
        return 0;
    }

    close(blpkFd);
    blpkFd = -1;
#endif
    HI_DEBUG_ADVCA("clear key:");
    for (i = 0; i < BLPK_LEN; i++)
    {
        HI_DEBUG_ADVCA("%02x ", g_BLPK[i]);
    }
    HI_DEBUG_ADVCA("\n");

    Ret = HI_UNF_CIPHER_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Init failed:%#x\n",Ret);
        goto _FLASH_CLOSE;
    }

    Ret = HI_UNF_CIPHER_CreateHandle(&blpkHandle, &stCipherAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_CreateHandle failed:%#x\n",Ret);
        goto _CIPHER_CLOSE;
    }

    CipherCtrl.bKeyByCA = HI_FALSE;
    CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
    CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
    CipherCtrl.stChangeFlags.bit1IV = 1;

    memcpy(CipherCtrl.u32Key, g_BLPK, BLPK_LEN);
    memset(CipherCtrl.u32IV, 0, 16);

    Ret = HI_UNF_CIPHER_ConfigHandle(blpkHandle, &CipherCtrl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_ConfigHandle failed:%#x\n",Ret);
        goto _CIPHER_DESTROY;
    }

    /*************************************************************/
    cipherAddrPhy = (HI_U32)((HI_ULONG)HI_MMZ_New(BOOT_IMG_LEN, 0, NULL, "cipherIn"));
    if (0 == cipherAddrPhy)
    {
        HI_DEBUG_ADVCA("get phyaddr for Input!\n");
        Ret = HI_FAILURE;
        goto _CIPHER_DESTROY;
    }

    pCipherAddrVir = HI_MMZ_Map(cipherAddrPhy, Testcached);

    /*************************************************************/
    cipherOutAddrPhy = (HI_U32)((HI_ULONG)HI_MMZ_New(BOOT_IMG_LEN, 0, NULL, "cipherOut"));
    if (0 == cipherOutAddrPhy)
    {
        HI_DEBUG_ADVCA("get phyaddr for Output!\n");
        Ret = HI_FAILURE;
        goto _CIPHER_DESTROY;
    }

    pCipherOutAddrVir = HI_MMZ_Map(cipherOutAddrPhy, Testcached);

    /*************************************************************/
    memcpy(pCipherAddrVir, pBootImage, u32CheckedAreaLen);
    memset(pCipherOutAddrVir, 0, BOOT_IMG_LEN);

    Ret = HI_UNF_CIPHER_Encrypt(blpkHandle, cipherAddrPhy, cipherOutAddrPhy, u32CheckedAreaLen);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Encrypt failed:%#x\n",Ret);
        goto _MMZ_DELETE;
    }

    /* encrypt blpk */
    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed:%#x\n",Ret);
        goto _MMZ_DELETE;
    }

    Ret = HI_UNF_ADVCA_EncryptSWPK(g_BLPK, EncryptBlpk);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_EncryptSWPK failed:%#x\n",Ret);
        goto _ADVCA_DEINIT;
    }

    HI_DEBUG_ADVCA("encrypted key:");
    for (i = 0; i < BLPK_LEN; i++)
    {
        HI_DEBUG_ADVCA("%02x ", EncryptBlpk[i]);
    }
    HI_DEBUG_ADVCA("\n");

    /* write encrypted blpk and set flag*/
    memcpy(pBlpk, EncryptBlpk, BLPK_LEN);

#ifdef OLD_BOOT_FORMAT
    pBlpk[BLPK_LEN] = BlpkClear = 0x48;
#else
    g_BootImg[CHECK_AREA_ENC_FLAG_ADDR]     = 0X12;
    g_BootImg[CHECK_AREA_ENC_FLAG_ADDR + 1] = 0XC8;
    g_BootImg[CHECK_AREA_ENC_FLAG_ADDR + 2] = 0X13;
    g_BootImg[CHECK_AREA_ENC_FLAG_ADDR + 3] = 0X2A;
#endif
    memcpy(pBootImage, pCipherOutAddrVir, u32CheckedAreaLen);

    Ret = HI_Flash_Write(hFlash, 0, g_BootImg, BOOT_IMG_LEN, HI_FLASH_RW_FLAG_ERASE_FIRST);
    if (HI_FAILURE == Ret)
    {
        HI_DEBUG_ADVCA("HI_Flash_Write Failed:%#x\n",Ret);
        goto _ADVCA_DEINIT;
    }

    Ret = HI_SUCCESS;
#ifdef ANDROID
    blpkFd = open ("enc_boot.bin", O_RDWR | O_CREAT, 0666);
#else
    blpkFd = open ("enc_boot.bin", O_RDWR | O_CREAT);
#endif
    if (blpkFd < 0)
    {
        HI_DEBUG_ADVCA("open blpk file ERROR!\n");
        goto _ADVCA_DEINIT;
    }
#ifdef OLD_BOOT_FORMAT
    len = write(blpkFd, g_BootImg, BLPK_SPACE_OFFSET + BLPK_SPACE_LEN + u32CheckedAreaLen + SIGNATURE_LEN);
#else
    len = write(blpkFd, g_BootImg, AUXILIARY_CODE_AREA_OFFSET + AuxAreaLen + Boot_Area_Len);
#endif
    if (0 == len)
    {
        HI_DEBUG_ADVCA("write to blpk.bin file FAIL!\n");
    }

    close(blpkFd);

_ADVCA_DEINIT:
    (HI_VOID)HI_UNF_ADVCA_DeInit();

_MMZ_DELETE:
    (HI_VOID)HI_MMZ_Unmap(cipherOutAddrPhy);
    (HI_VOID)HI_MMZ_Delete(cipherOutAddrPhy);

    (HI_VOID)HI_MMZ_Unmap(cipherAddrPhy);
    (HI_VOID)HI_MMZ_Delete(cipherAddrPhy);

_CIPHER_DESTROY:
    (HI_VOID)HI_UNF_CIPHER_DestroyHandle(blpkHandle);

_CIPHER_CLOSE:
    (HI_VOID)HI_UNF_CIPHER_DeInit();

_FLASH_CLOSE:
    (HI_VOID)HI_Flash_Close(hFlash);

_SYS_DEINIT:
    (HI_VOID)HI_SYS_DeInit();

    if (HI_SUCCESS == Ret)
    {
        HI_DEBUG_ADVCA("encrypt blpk SUCCESS!\n");
    }
    else
    {
        HI_DEBUG_ADVCA("encrypt blpk FAIL!\n");
    }

    return Ret;
}

