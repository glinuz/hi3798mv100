/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : cmd_authenticate.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 204-09-19
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef HI_MINIBOOT_SUPPORT
#ifndef HI_ADVCA_FUNCTION_RELEASE

#ifndef HI_MINIBOOT_SUPPORT
#include "exports.h"
#endif

#if defined (CHIP_TYPE_hi3716mv310)
#include "hi_boot_common.h"
#else
#include "hi_common.h"
#endif
#include "hi_unf_cipher.h"
#include "hi_unf_otp.h"
#include "hi_unf_advca.h"
#include "cx_adp.h"
#include "cx_flash.h"
#include "cx_debug.h"
#include "cx_cfg.h"


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

extern HI_U32 g_EnvFlashAddr;
extern HI_U32 g_EnvFlashPartionSize;


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

static HI_VOID compare_hex(char *title, HI_U8 *pu8Src, HI_U8 *pu8Dst, HI_U32 u32Length)
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

//test for AES CBC MAC

extern HI_S32 HI_CA_CalcMAC(HI_U8 *pu8Key, HI_U8 *pInputData, HI_U32 u32InputDataLen,
                                        HI_U8 *pOutputMAC, HI_BOOL bIsLastBlock);

HI_S32 cipher_calcmac_test(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{

    HI_U8 T[16];
    HI_U8 M[64] = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
        0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
        0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
        0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
        0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
        0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
        0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
        0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
    };
    HI_U8 key[16] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
	//Root key ?a0¨º¡À¦Ì?2a¨º??¨¢1?
    HI_U8 AEC_CBC_MAC_0[16] = {
        0x10, 0x80, 0xBB, 0x96, 0x6B, 0x0A, 0xEA, 0xA8, 
        0x2D, 0x4E, 0x07, 0x61, 0xB6, 0xDA, 0xAC, 0x57
    };

    HI_U8 AEC_CBC_MAC_16[16] = {
        0x52, 0x8F, 0xCE, 0x1F, 0xC0, 0xFE, 0x06, 0xF8,
        0x4A, 0xD9, 0xFE, 0xF5, 0x37, 0x42, 0x2B, 0x2D 
    };
    HI_U8 AEC_CBC_MAC_40[16] = {
        0x93, 0x4C, 0xBC, 0xE5, 0x97, 0x70, 0xA7, 0x21,
        0x64, 0x31, 0x02, 0xFD, 0xFB, 0x01, 0x36, 0x9B
    };
    HI_U8 AEC_CBC_MAC_64[16] = {
        0x21, 0x43, 0xEE, 0x76, 0x35, 0xBD, 0x01, 0x7D,
        0x61, 0xAE, 0xC9, 0xCD, 0xF9, 0x04, 0xC0, 0x50
    };
    
    HI_UNF_ADVCA_Init();

    HI_SIMPLEINFO_CA("\nExample 1: len = 0\n");
    HI_SIMPLEINFO_CA("M "); HI_SIMPLEINFO_CA("<empty string>\n");
    HI_CA_CalcMAC(key, M, 0, T, HI_TRUE);
    HI_SIMPLEINFO_CA("AES_CMAC "); printf_hex(T, 16); HI_SIMPLEINFO_CA("\n");
    compare_hex("M_0", AEC_CBC_MAC_0, T, 16);

    HI_SIMPLEINFO_CA("\nExample 2: len = 16\n");
    HI_SIMPLEINFO_CA("M "); printf_hex(M, 16);
    HI_CA_CalcMAC(key, M, 16, T, HI_TRUE);
    HI_SIMPLEINFO_CA("AES_CMAC "); printf_hex(T, 16); HI_SIMPLEINFO_CA("\n");
    compare_hex("M_16", AEC_CBC_MAC_16, T, 16);

    HI_SIMPLEINFO_CA("\nExample 3: len = 64\n");
    HI_CA_CalcMAC(key, M, 64, T, HI_TRUE);
    HI_SIMPLEINFO_CA("AES_CMAC "); printf_hex(T, 16); HI_SIMPLEINFO_CA("\n");
    compare_hex("M_64", AEC_CBC_MAC_64, T, 16);

    HI_SIMPLEINFO_CA("\nExample 4: len = 40\n");
    HI_CA_CalcMAC(key, M, 40, T, HI_TRUE);
    HI_SIMPLEINFO_CA("AES_CMAC "); printf_hex(T, 16); HI_SIMPLEINFO_CA("\n");
    compare_hex("M_40", AEC_CBC_MAC_40, T, 16);

    HI_U32 timeuse = 0;
    HI_U32 newtime, oldtime;
    HI_U32 i;

    oldtime = get_timer(0);

    HI_SIMPLEINFO_CA("start 300M data test......\r\n");

    for(i = 0; i < 299; i++)
    {
        HI_CA_CalcMAC(key, (HI_U8 *)IMG_VERIFY_ADDRESS, 0x100000, T, HI_FALSE);
    }
    HI_CA_CalcMAC(key, (HI_U8 *)IMG_VERIFY_ADDRESS, 0x100000, T, HI_TRUE);

    newtime = get_timer(0);
    timeuse= (newtime - oldtime)/get_tbclk();
    HI_SIMPLEINFO_CA("time use: %d s\r\n", timeuse);

    HI_SIMPLEINFO_CA("--------------------------------------------------\n");

    return 0;
}
U_BOOT_CMD(cipher_calcmac_test, 2, 1, cipher_calcmac_test, "", "");


#define BLPK_SPACE_V300_OFFSET  0x1800
#define BLPK_SPACE_V200_OFFSET  0x800
#define BLPK_PARAM_OFFSET       0x304
#define BLPK_LEN                0x10
#define BOOT_IMG_LEN            0x100000
#define BLOAD_MODE_SEL_ADDR     0x03

#define BOOT_BASIC          "boot"

static HI_U8 g_BootImg[BOOT_IMG_LEN] = {0};

HI_S32 CX_EncryptBoot_Test(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_S32                  ret = 0;
    HI_U8                   *pBlpk;
    HI_U8                   *pBootImage;
    HI_U8                   EncryptBlpk[BLPK_LEN];
    HI_U8                   g_customer_blpk[16];
    HI_U8                   BlpkClear;
    HI_UNF_CIPHER_CTRL_S    CipherCtrl;
    HI_HANDLE               blpkHandle = 0;
    HI_U8                   u8CheckedAreaOffset[4] = {0};
    HI_U8                   u8CheckedAreaLen[4] = {0};
    HI_U32                  u32CheckedAreaOffset = 0;
    HI_U32                  u32CheckedAreaLen = 0;
    HI_U32                  i = 0;
    CA_V300_BLOAD_MODE_U    bloadMode;
    static HI_U32           u32MmzPhyAddr = 0;
    HI_BOOL                 bIsV300 = HI_FALSE;
    HI_U32                  u32BlpkSpaceOffset = 0;
    HI_UNF_CIPHER_ATTS_S stCipherAtts;

    u32BlpkSpaceOffset= BLPK_SPACE_V300_OFFSET;
    bIsV300 = HI_TRUE;

    /* read all valid data of boot partition  */
    CA_ASSERT(CA_flash_read(BOOT_BASIC,0ULL,BOOT_IMG_LEN,(HI_U8*)g_BootImg, HI_NULL),ret);

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

    HI_SIMPLEINFO_CA("u32CheckedAreaOffset = %x, u32CheckedAreaLen = %x\n", u32CheckedAreaOffset, u32CheckedAreaLen);

    if (0 != (u32CheckedAreaLen % 0x100))
    {
        HI_ERR_CA("checked area length invalid!\n");
        return HI_FAILURE;
    }

    /* read blpk and its clear flag */
    pBlpk = g_BootImg + u32BlpkSpaceOffset;
    pBootImage = g_BootImg + u32BlpkSpaceOffset + u32CheckedAreaOffset;
    BlpkClear = pBlpk[BLPK_LEN];
    
    HI_CX_GetBLPKInfor(g_customer_blpk);
    
    if (bIsV300)
    {
        if (0x48 == BlpkClear)
        {
            HI_ERR_CA("blpk is already encrypted\n");
            return HI_SUCCESS;
        }

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
            HI_ERR_CA("HI_UNF_CIPHER_Encrypt failed:%#x\n",ret);
            goto _CIPHER_DESTROY;
        }
        memcpy(pBootImage, (HI_U8 *)u32MmzPhyAddr, u32CheckedAreaLen);

        /* encrypt blpk */
        HI_UNF_ADVCA_Init();

        ret = HI_UNF_ADVCA_EncryptSWPK(g_customer_blpk,EncryptBlpk);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("1: Fail to encrypt SWPK:%#x\n",ret);
            goto _CIPHER_DESTROY;
        }

        HI_SIMPLEINFO_CA("encrypted key:");
        for (i = 0; i < BLPK_LEN; i++)
        {
            HI_SIMPLEINFO_CA("%02x ", EncryptBlpk[i]);
        }
        HI_SIMPLEINFO_CA("\n");

        /* write encrypted blpk and set flag*/
        memcpy(pBlpk,EncryptBlpk,BLPK_LEN);
        pBlpk[BLPK_LEN] = BlpkClear = 0X48;

        /* write all valid data of boot partition  */
        ret = CA_flash_write(BOOT_BASIC,0ULL,BOOT_IMG_LEN,(HI_U8*)g_BootImg);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("CA_flash_write failed:%#x\n",ret);
            goto _CIPHER_DESTROY;
        }

        HI_UNF_OTP_Init();

        /*Set bload mode sel flag*/
        bloadMode.u8 = 0;
        bloadMode.bits.bload_mode_sel = 1;
        ret = HI_OTP_WriteByte(BLOAD_MODE_SEL_ADDR, bloadMode.u8);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Set bload_mod_sel failed:%#x\n",ret);
            goto _OTP_DEINIT;
        }

        HI_UNF_OTP_DeInit();

        ret = HI_SUCCESS;

_OTP_DEINIT:
        HI_UNF_OTP_DeInit();

_CIPHER_DESTROY:
        HI_UNF_CIPHER_DestroyHandle(blpkHandle);

_CIPHER_CLOSE:
        HI_UNF_CIPHER_DeInit();
    }
    else
    {
        if (0x0 == BlpkClear)
        {
            HI_ERR_CA("blpk is already encrypted\n");
            return HI_SUCCESS;
        }

        HI_SIMPLEINFO_CA("clear key:");
        for (i = 0; i < BLPK_LEN; i++)
        {
            HI_SIMPLEINFO_CA("%02x ", pBlpk[i]);
        }
        HI_SIMPLEINFO_CA("\n");

        /* encrypt blpk */
        HI_UNF_ADVCA_Init();

        ret = HI_UNF_ADVCA_EncryptSWPK(pBlpk, EncryptBlpk);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("2: Fail to encrypt SWPK:%#x\n",ret);
            goto _END_PROCESS;
        }

        HI_SIMPLEINFO_CA("encrypted key:");
        for (i = 0; i < BLPK_LEN; i++)
        {
            HI_SIMPLEINFO_CA("%02x ", EncryptBlpk[i]);
        }
        HI_SIMPLEINFO_CA("\n");

        /* write encrypted blpk and set flag*/
        memcpy(pBlpk,EncryptBlpk,BLPK_LEN);
        pBlpk[BLPK_LEN] = BlpkClear = 0X0;

        /* write all valid data of boot partition  */
        ret = CA_flash_write(BOOT_BASIC,0ULL,BOOT_IMG_LEN,(HI_U8*)g_BootImg);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("CA_flash_write failed:%#x\n",ret);
            goto _END_PROCESS;
        }

        ret = HI_SUCCESS;
    }

_END_PROCESS:
    if (HI_SUCCESS == ret)
    {
        HI_ERR_CA("ca_encryptboot blpk SUCCESS!\n");
    }
    else
    {
        HI_ERR_CA("ca_encryptboot blpk FAIL!\n");
    }

    return ret;
}
U_BOOT_CMD(cx_encryptboot, 1, 1, CX_EncryptBoot_Test, "CX Encrypt Boot ", "eg: cx_encryptboot");


/*************************************************************************
 *
 * The test below is for Irdeto MSR2.2 chipset AES CBC-MAC.
 *
 *************************************************************************/
HI_S32 CA_CbcMacTest(cmd_tbl_t *cmdtp, HI_S32 flag, HI_S32 argc, char *argv[])
{
    HI_U8 u8TestData[16];
    HI_U8 u8CbcMac[16];
    HI_S32 Ret = HI_SUCCESS;
    HI_UNF_ADVCA_CA_VENDOR_OPT_S stCaVendorOpt;

    HI_UNF_ADVCA_Init();
    CA_ASSERT(HI_UNF_CIPHER_Init(), Ret);

    HI_SIMPLEINFO_CA("Test case 1: Calculate the AES CBC MAC\n");

    memset(u8TestData, 0xA5, sizeof(u8TestData));
    stCaVendorOpt.enCaVendorOpt = HI_ADVCA_CAVENDOR_ATTR_IRDETO_CBCMAC_CALC;
    stCaVendorOpt.unCaVendorOpt.stIrdetoCbcMac.pu8InputData = u8TestData;
    stCaVendorOpt.unCaVendorOpt.stIrdetoCbcMac.u32InputDataLen = sizeof(u8TestData);
    Ret = HI_UNF_ADVCA_CaVendorOperation(HI_ADVCA_IRDETO, &stCaVendorOpt);
    if(Ret != HI_SUCCESS)
    {
        HI_ERR_CA("Calcualte the AES CBC MAC failed! Ret = 0x%08x\n", Ret);
        return HI_FAILURE;
    }
    memcpy(u8CbcMac, stCaVendorOpt.unCaVendorOpt.stIrdetoCbcMac.u8OutputCbcMac, 16);
    HI_SIMPLEINFO_CA("The AES CBC-MAC is:\n");
    printf_hex(u8CbcMac, 16);

    HI_SIMPLEINFO_CA("Test case 2: Authenticate the AES CBC MAC\n");

    //If the authentication failed, the chipset will reset automatically
    memset(u8TestData, 0xA5, sizeof(u8TestData));
    stCaVendorOpt.enCaVendorOpt = HI_ADVCA_CAVENDOR_ATTR_IRDETO_CBCMAC_AUTH;
    stCaVendorOpt.unCaVendorOpt.stIrdetoCbcMac.pu8InputData = u8TestData;
    stCaVendorOpt.unCaVendorOpt.stIrdetoCbcMac.u32InputDataLen = sizeof(u8TestData);
    stCaVendorOpt.unCaVendorOpt.stIrdetoCbcMac.u32AppLen = sizeof(u8TestData);

    //Use the last AES CBC-MAC for test
    memcpy(stCaVendorOpt.unCaVendorOpt.stIrdetoCbcMac.u8RefCbcMAC, u8CbcMac, 16);
    Ret = HI_UNF_ADVCA_CaVendorOperation(HI_ADVCA_IRDETO, &stCaVendorOpt);
    if(Ret != HI_SUCCESS)
    {
        HI_ERR_CA("Authenticate the AES CBC MAC failed! Ret = 0x%08x\n", Ret);
        return HI_FAILURE;
    }
    HI_SIMPLEINFO_CA("Authenticate the AES CBC MAC successfully\n");

    return HI_SUCCESS;
}
U_BOOT_CMD(ca_cbcmac_test, 2, 1, CA_CbcMacTest, "", "");

/*temporarily comment the function, to be done */
#if 0
HI_S32 OTP_GetChipIDEx(HI_U8 *pu8ChipId, HI_U32 len)
{
    HI_U32 index = 0;

    if(NULL == pu8ChipId || 8 != len)
    {
        HI_ERR_OTP("Invalid parameters when read chipid!\n");
        return HI_FAILURE;
    }

    for(index = 0; index < len; index++)
    {
        pu8ChipId[index] = HAL_OTP_V200_ReadByte(OTP_CHIP_ID_ADDR + index);
    }

    return HI_SUCCESS;
}

HI_S32 OTP_GetChipID_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_U8 au8ChipId[8] = {0};

    HI_UNF_OTP_Init();
    OTP_GetChipIDEx(au8ChipId, 8);

    HI_SIMPLEINFO_CA("Chipid:\n");
    printf_hex(au8ChipId, 8);

    HI_SIMPLEINFO_CA("otp_getchipid success\n");

    return HI_SUCCESS;
}
U_BOOT_CMD(otp_getchipid,2,1,OTP_GetChipID_test,"otp_getchipid ","");
#endif

HI_S32 OTP_GetStbSN_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 i = 0;
    HI_U8 u8SN[4];

    memset(u8SN,0,sizeof(u8SN));

	ret = HI_UNF_ADVCA_Init();
    ret |= HI_UNF_ADVCA_GetStbSn(u8SN);
	if(HI_SUCCESS != ret)
	{
		HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetStbSn failed\n");
	    return HI_FAILURE;
	}

    HI_SIMPLEINFO_CA("STBSN = ");
    for(i = 0;i<4;i++)
    {
        HI_SIMPLEINFO_CA("0x%x ",u8SN[i]);
    }
    HI_SIMPLEINFO_CA("\n");
    HI_SIMPLEINFO_CA("otp_getstbsn success\n");
    return HI_SUCCESS;
}
U_BOOT_CMD(otp_getstbsn,2,1,OTP_GetStbSN_test,"otp_getstbsn ","");

HI_S32 OTP_GetMarketID_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_U8 i = 0;
    HI_U8 u8SN[4];

    HI_UNF_OTP_Init();

    memset(u8SN,0,sizeof(u8SN));
    OTP_GetMarketID(u8SN);

    HI_SIMPLEINFO_CA("MarketID = \n");
    for(i = 0;i<4;i++)
    {
        HI_SIMPLEINFO_CA("0x%x ",u8SN[i]);
    }
    HI_SIMPLEINFO_CA("\n");
    HI_SIMPLEINFO_CA("otp_getmsid success\n");
    return HI_SUCCESS;
}
U_BOOT_CMD(otp_getmsid,2,1,OTP_GetMarketID_test,"otp_getmsid ","for example: getMarketID");


HI_S32 OTP_GetSecureBootEn_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bSecBootEn = HI_FALSE;
	HI_UNF_ADVCA_FLASH_TYPE_E enFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;

	s32Ret = HI_UNF_ADVCA_Init();
    s32Ret |= HI_UNF_ADVCA_GetSecBootStat(&bSecBootEn, &enFlashType);
    if(HI_SUCCESS != s32Ret)
    {
        HI_SIMPLEINFO_CA("Get Secure boot en failed!\n");
        return HI_FAILURE;
    }

    HI_SIMPLEINFO_CA("SecBootEn = %d\n", bSecBootEn);
    HI_SIMPLEINFO_CA("otp_getsecurebooten success\n");

    return HI_SUCCESS;
}
U_BOOT_CMD(otp_getsecurebooten,2,1,OTP_GetSecureBootEn_test,"otp_getsecurebooten ","");

HI_S32 CA_GetSecBootStat_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_S32 ret = HI_SUCCESS;
    HI_BOOL bEnable = HI_FALSE;
    HI_UNF_ADVCA_FLASH_TYPE_E enFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;

    ret = HI_UNF_ADVCA_Init();
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_Init failed!\n");
        return HI_FAILURE;
    }

    ret = HI_UNF_ADVCA_GetSecBootStat(&bEnable, &enFlashType);
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetSecBootStat failed!\n");
        return HI_FAILURE;
    }

    if(HI_TRUE == bEnable)
    {
        HI_SIMPLEINFO_CA("Secure boot is enabled\n");
    }
    else
    {
        HI_SIMPLEINFO_CA("Secure boot is not enabled\n");
        return HI_SUCCESS;
    }

    switch(enFlashType)
    {
        case HI_UNF_ADVCA_FLASH_TYPE_SPI:
        {
            HI_SIMPLEINFO_CA("Secure boot from SPI flash(enFlashType:%d).\n", enFlashType);
            break;
        }
        case HI_UNF_ADVCA_FLASH_TYPE_NAND:
        {
            HI_SIMPLEINFO_CA("Secure boot from NAND flash(enFlashType:%d).\n", enFlashType);
            break;
        }
        case HI_UNF_ADVCA_FLASH_TYPE_NOR:
        {
            HI_SIMPLEINFO_CA("Secure boot from NOR flash(enFlashType:%d).\n", enFlashType);
            break;
        }
        case HI_UNF_ADVCA_FLASH_TYPE_EMMC:
        {
            HI_SIMPLEINFO_CA("Secure boot from EMMC flash(enFlashType:%d).\n", enFlashType);
            break;
        }
        case HI_UNF_ADVCA_FLASH_TYPE_SPI_NAND:
        {
            HI_SIMPLEINFO_CA("Secure boot from SPI_NAND flash(enFlashType:%d).\n", enFlashType);
            break;
        }
        case HI_UNF_ADVCA_FLASH_TYPE_SD:
        {
            HI_SIMPLEINFO_CA("Secure boot from SD flash(enFlashType:%d).\n", enFlashType);
            break;
        }
        default:
        {
            HI_SIMPLEINFO_CA("Invalid enFlashType:%d.\n", enFlashType);
            break;
        }
    }

    return HI_SUCCESS;
}
U_BOOT_CMD(ca_getsecbootstatus,2,1,CA_GetSecBootStat_test,"ca_getsecbootstatus ","");

HI_S32 CA_EnableSecBoot_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_ADVCA_FLASH_TYPE_E enFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;

    if(2 != argc)
    {
        HI_SIMPLEINFO_CA("Invalid argument, argc:%d!\n", argc);
        return HI_FAILURE;
    }

    ret = HI_UNF_ADVCA_Init();
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_Init failed!\n");
        return HI_FAILURE;
    }

    if(0 == strncmp(argv[1], "spi", strlen("spi")))
    {
        HI_SIMPLEINFO_CA("Flash type: spi(%s)\n", argv[1]);
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_SPI;
    }
    else if(0 == strncmp(argv[1], "nand", strlen("nand")))
    {
        HI_SIMPLEINFO_CA("Flash type: nand(%s)\n", argv[1]);
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_NAND;
    }
    else if(0 == strncmp(argv[1], "nor", strlen("nor")))
    {
        HI_SIMPLEINFO_CA("Flash type: nor(%s)\n", argv[1]);
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_NOR;
    }
    else if(0 == strncmp(argv[1], "emmc", strlen("emmc")))
    {
        HI_SIMPLEINFO_CA("Flash type: emmc(%s)\n", argv[1]);
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_EMMC;
    }
    else if(0 == strncmp(argv[1], "sd", strlen("sd")))
    {
        HI_SIMPLEINFO_CA("Flash type: sd(%s)\n", argv[1]);
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_SD;
    }
    else if(0 == strncmp(argv[1], "spi_nand", strlen("spi_nand")))
    {
        HI_SIMPLEINFO_CA("Flash type: spi_nand(%s)\n", argv[1]);
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_SPI_NAND;
    }
    else
    {
        HI_SIMPLEINFO_CA("Invalid flash type: %s\n", argv[1]);
        return HI_FAILURE;
    }

    ret = HI_UNF_ADVCA_EnableSecBoot(enFlashType);
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_EnableSecBoot failed!\n");
        return HI_FAILURE;
    }

    HI_SIMPLEINFO_CA("Enable secure boot success!\n");

    return HI_SUCCESS;
}
U_BOOT_CMD(ca_enablesecboot,2,1,CA_EnableSecBoot_test,"ca_enablesecboot flash_type(spi|nand|sd|emmc)","");

static HI_VOID printbuffer(HI_CHAR *str, HI_U8 *pu8Buf, HI_U32 u32Length)
{
    HI_U32 i = 0;

    if(NULL != str)
    {
        HI_SIMPLEINFO_CA("%s\n", str);
    }

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

HI_S32 CA_SetRSAKey_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
    /* This rsa key is only for test */
    HI_U8 au8Key[512] =
    {
        0xa3, 0x1f, 0x12, 0xf8, 0xa6, 0xbf, 0xb5, 0x19, 0xad, 0xa3, 0xef, 0x18, 0x68, 0x37, 0x2b, 0xd1,
        0xa5, 0x7d, 0x1c, 0x2f, 0x83, 0x88, 0x56, 0xe5, 0xfb, 0xe6, 0xa0, 0xac, 0xa4, 0xd7, 0xb9, 0xd8,
        0xb7, 0xca, 0x37, 0x5a, 0xda, 0xd7, 0x6d, 0xb7, 0x76, 0x59, 0x96, 0x44, 0x9f, 0x79, 0xab, 0xd8,
        0x18, 0xe2, 0x00, 0xb7, 0x67, 0xa0, 0x0f, 0x59, 0x9b, 0x9f, 0x8b, 0xb0, 0x5d, 0x6f, 0x9f, 0xa6,
        0x74, 0xe0, 0xfd, 0x14, 0x4d, 0xae, 0x23, 0x9b, 0x5d, 0x18, 0x7f, 0x37, 0x82, 0x03, 0x44, 0xac,
        0xca, 0x6d, 0x88, 0xc3, 0x2c, 0x5b, 0x47, 0x8c, 0xb5, 0x07, 0x6b, 0x9c, 0xd7, 0x6b, 0x64, 0x1e,
        0x82, 0x6e, 0xb8, 0x7e, 0xd0, 0xc2, 0xf4, 0x26, 0x17, 0xe1, 0xdc, 0x2f, 0x81, 0x3b, 0xa6, 0x9e,
        0x1f, 0xbe, 0xc6, 0x89, 0xc4, 0x1d, 0xa3, 0xe1, 0x2f, 0x88, 0x5f, 0x53, 0xd8, 0xae, 0xe6, 0xef,
        0xb2, 0x5c, 0x6a, 0xba, 0xba, 0xfd, 0x29, 0xfe, 0x99, 0x1a, 0x89, 0x73, 0xf7, 0xc0, 0x6e, 0xb7,
        0x13, 0x58, 0xc8, 0x9a, 0x29, 0x17, 0xba, 0xf2, 0x6c, 0xd7, 0x16, 0xc4, 0x9e, 0x3b, 0xe8, 0x90,
        0x72, 0x40, 0xa6, 0x4f, 0xee, 0x45, 0x9d, 0xe9, 0xae, 0xa0, 0x41, 0xfc, 0xc6, 0x01, 0x9b, 0x26,
        0x38, 0xf6, 0x5d, 0x11, 0xa1, 0x18, 0x54, 0x2d, 0x15, 0x60, 0x7a, 0xac, 0xc7, 0x7a, 0x18, 0x1d,
        0xeb, 0xcf, 0x15, 0x4c, 0xdd, 0x88, 0xa4, 0x41, 0xbe, 0x6b, 0x1a, 0x5a, 0x4f, 0xf7, 0x8e, 0xbe,
        0x5b, 0xc9, 0x6a, 0x2f, 0x6a, 0x74, 0xb0, 0xfc, 0x8b, 0xe8, 0xae, 0x68, 0xd7, 0x86, 0xad, 0x60,
        0x5b, 0xf0, 0x8a, 0x2c, 0xe1, 0xb2, 0xf9, 0x3f, 0x35, 0x5d, 0xa3, 0xd6, 0x5e, 0xfd, 0x12, 0xb6,
        0x21, 0xca, 0x72, 0xa9, 0xba, 0xf2, 0x97, 0xbc, 0x2d, 0x79, 0xba, 0xd6, 0x48, 0x53, 0x97, 0x5d,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
    };

    ret = HI_UNF_ADVCA_Init();
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_Init failed!\n");
        return HI_FAILURE;
    }

    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
    stOtpFuseAttr.unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked = HI_FALSE;
    ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG, &stOtpFuseAttr);
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetSecBootStat failed!\n");
        return HI_FAILURE;
    }

    if(HI_TRUE == stOtpFuseAttr.unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked)
    {
        HI_SIMPLEINFO_CA("Failed, rsa key has already been locked before!\n");
        return HI_FAILURE;
    }

    ret = HI_UNF_ADVCA_SetRSAKey(au8Key);
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetSecBootStat failed!\n");
        return HI_FAILURE;
    }

    memset(au8Key, 0, sizeof(au8Key));
    ret = HI_UNF_ADVCA_GetRSAKey(au8Key);
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetSecBootStat failed!\n");
        return HI_FAILURE;
    }

    printbuffer("set rsa key success:", au8Key, sizeof(au8Key));

    return HI_SUCCESS;
}
U_BOOT_CMD(ca_setotprsakey,2,1,CA_SetRSAKey_test,"ca_setotprsakey ","");

HI_S32 CA_GetRSAKey_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
    HI_U8 au8Key[512] = {0};

    ret = HI_UNF_ADVCA_Init();
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_Init failed!\n");
        return HI_FAILURE;
    }

    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
    stOtpFuseAttr.unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked = HI_FALSE;
    ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG, &stOtpFuseAttr);
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetSecBootStat failed!\n");
        return HI_FAILURE;
    }

    if(HI_TRUE == stOtpFuseAttr.unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked)
    {
        HI_SIMPLEINFO_CA("Failed, rsa key can not be read out, it has already been locked before!\n");
        return HI_FAILURE;
    }

    memset(au8Key, 0, sizeof(au8Key));
    ret = HI_UNF_ADVCA_GetRSAKey(au8Key);
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetRSAKey failed! ret:0x%x\n", ret);
        return HI_FAILURE;
    }

    printbuffer("rsa key:", au8Key, sizeof(au8Key));

    return HI_SUCCESS;
}
U_BOOT_CMD(ca_getotprsakey,2,1,CA_GetRSAKey_test,"ca_getotprsakey ","");

HI_S32 CA_GetRSAKeyLockFlag_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;

    ret = HI_UNF_ADVCA_Init();
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_Init failed!\n");
        return HI_FAILURE;
    }

    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
    stOtpFuseAttr.unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked = HI_FALSE;
    ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG, &stOtpFuseAttr);
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetOtpFuse failed!\n");
        return HI_FAILURE;
    }

    HI_SIMPLEINFO_CA("get rsa key lock flag: %d\n", stOtpFuseAttr.unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked);

    return HI_SUCCESS;
}
U_BOOT_CMD(ca_getrsakeylockflag,2,1,CA_GetRSAKeyLockFlag_test,"ca_getrsakeylockflag ","");

HI_S32 CA_LockRSAKey_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
    //HI_U8 au8Key[512] = {0};

    ret = HI_UNF_ADVCA_Init();
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_Init failed!\n");
        return HI_FAILURE;
    }

    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
    stOtpFuseAttr.unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked = HI_FALSE;
    ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG, &stOtpFuseAttr);
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetOtpFuse failed!\n");
        return HI_FAILURE;
    }

    if(HI_TRUE == stOtpFuseAttr.unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked)
    {
        HI_SIMPLEINFO_CA("Failed, rsa key can not be locked anymore, it has already been locked before!\n");
        return HI_FAILURE;
    }

    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
    stOtpFuseAttr.unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked = HI_TRUE;
    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG, &stOtpFuseAttr);
    if(HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetOtpFuse failed!\n");
        return HI_FAILURE;
    }

    HI_SIMPLEINFO_CA("lock rsa key success!\n");

    return HI_SUCCESS;
}
U_BOOT_CMD(ca_lockrsakey,2,1,CA_LockRSAKey_test,"ca_lockrsakey ","");

#endif /* #ifndef HI_ADVCA_FUNCTION_RELEASE */
#endif
