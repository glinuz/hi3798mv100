#include "hi_type.h"
#include "hi_flash.h"
#include "hi_unf_advca.h"
#include "hi_unf_ir.h"

#ifdef HI_MINIBOOT_SUPPORT
#include "app.h"
#else
#include "exports.h"
#endif

#define HI_SIMPLEINFO_CA(format, arg...)   printf(format, ## arg)

#ifdef HI_MINIBOOT_SUPPORT
extern int run_cmd(char *cmdline, int length);
#endif

extern void load_recovery(void);
extern int set_default_ethaddr(void);
extern HI_S32 HI_Android_Authenticate(HI_VOID);
#ifdef HI_ADVCA_USB_BURN_KEY
extern int read_ca_key_file(char *filename, void *buffer);
extern HI_FLASH_TYPE_E CA_get_env_flash_type(HI_VOID);
#endif

int Android_Main(void)
{
    load_recovery();
    set_default_ethaddr();
#ifdef HI_ADVCA_SUPPORT
    if (-1 == HI_Android_Authenticate())
    {
	#ifdef HI_MINIBOOT_SUPPORT
        run_cmd("reset", 0);
	#else
        run_command("reset", 0);
	#endif
    }
#endif
    return 0;
}

#ifdef HI_ADVCA_USB_BURN_KEY
int Android_Security_WriteRSAKey(void)
{
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_ADVCA_OTP_ATTR_S     stOtpFuseAttr;
    HI_UNF_ADVCA_FLASH_TYPE_E   enFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;
    HI_FLASH_TYPE_E             EnvFlashType;
    HI_U8 CRCKey[516] = {0};
    HI_U8 RSAKey[512] = {0};
    HI_U8 oldRSAKey[512] = {0};
    HI_U32 i = 0;
    HI_BOOL bEnable = HI_FALSE;
    HI_BOOL rsaKeySetFlag = HI_FALSE;

    HI_SIMPLEINFO_CA("Security begin read RSA CRC Key in USB disk\n");
    ret = read_ca_key_file("root_rsa_pub_crc.bin", CRCKey);
    if (ret != sizeof(CRCKey))
    {
        HI_SIMPLEINFO_CA("Security no RSA CRC Key in USB disk\n");
        return HI_FAILURE;
    }
    HI_SIMPLEINFO_CA("Security read RSA CRC Key in USB disk success\n");

    ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_Init failed 0x%x\n", ret);
        return HI_FAILURE;
    }

    ret = HI_UNF_ADVCA_GetSecBootStat(&bEnable, &enFlashType);
    if (HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetSecBootStat failed 0x%x\n", ret);
        return HI_FAILURE;
    }

    if (HI_TRUE == bEnable)
    {
        HI_SIMPLEINFO_CA("Secure boot is enabled\n");
        return HI_FAILURE;
    }

    HI_SIMPLEINFO_CA("Secure boot is not enabled\n");

    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));

    stOtpFuseAttr.unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked = HI_FALSE;

    ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG, &stOtpFuseAttr);
    if (HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetOtpFuse failed 0x%x\n", ret);
        return HI_FAILURE;
    }

    if (HI_TRUE == stOtpFuseAttr.unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked)
    {
        HI_SIMPLEINFO_CA("Failed, rsa key has already been locked before!\n");
        //return HI_FAILURE;
    }
    else
    {
        memcpy(RSAKey, CRCKey, sizeof(RSAKey));

        ret = HI_UNF_ADVCA_GetRSAKey(oldRSAKey);
        if (HI_SUCCESS != ret)
        {
            HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetRSAKey failed 0x%x\n", ret);
            return HI_FAILURE;
        }

        for (i = 0; i < sizeof(oldRSAKey); i++)
        {
            if (oldRSAKey[i] != 0x0)
            {
                HI_SIMPLEINFO_CA("Security RSA Key has already been set!\n");
                rsaKeySetFlag = HI_TRUE;
                break;
            }
        }

        if (rsaKeySetFlag)
        {
            ret = memcmp(RSAKey, oldRSAKey, sizeof(RSAKey));
            if(0 != ret)
            {
                HI_SIMPLEINFO_CA("RSAKey and oldRSAKey are not the same and not lock the key! ret:0x%x\n", ret);
                return HI_FAILURE;
            }
        }
        else
        {
            ret = HI_UNF_ADVCA_SetRSAKey(RSAKey);
            if (HI_SUCCESS != ret)
            {
                HI_SIMPLEINFO_CA("HI_UNF_ADVCA_SetRSAKey failed 0x%x\n", ret);
                return HI_FAILURE;
            }
        }

        memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));

        stOtpFuseAttr.unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked = HI_TRUE;

        ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG, &stOtpFuseAttr);
        if (HI_SUCCESS != ret)
        {
            HI_SIMPLEINFO_CA("HI_UNF_ADVCA_GetOtpFuse failed 0x%x\n", ret);
            return HI_FAILURE;
        }

        HI_SIMPLEINFO_CA("Security Lock RSA Key Success\n");
    }
    EnvFlashType = CA_get_env_flash_type();
    if (EnvFlashType == HI_FLASH_TYPE_SPI_0)
    {
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_SPI;
    }
    else if (EnvFlashType == HI_FLASH_TYPE_NAND_0)
    {
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_NAND;
    }
    else if (EnvFlashType == HI_FLASH_TYPE_EMMC_0)
    {
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_EMMC;
    }
    else if (EnvFlashType == HI_FLASH_TYPE_SD)
    {
        enFlashType = HI_UNF_ADVCA_FLASH_TYPE_SD;
    }
    else
    {
        HI_SIMPLEINFO_CA("Security Invalid Flash Type: %d\n", enFlashType);
        return HI_FAILURE;
    }

    ret = HI_UNF_ADVCA_EnableSecBoot(enFlashType);
    if (HI_SUCCESS != ret)
    {
        HI_SIMPLEINFO_CA("HI_UNF_ADVCA_EnableSecBoot failed!\n");
        return HI_FAILURE;
    }

    HI_SIMPLEINFO_CA("Security Enable Secure Boot Success\n");

    return 0;
}
#endif

#ifdef HI_BOOT_IR_SUPPORT
int Android_Qb_CheckRemote(void)
{
    HI_S32 ret;
    HI_U32 ret_val = 0;
    enum KEY_STATUS PressStatus = KEY_STAT_BUTT;
    HI_U64 KeyId = 0;

    printf("Android_Qb_CheckRemote begin\n");

    ret = HI_UNF_IR_Init();
    if (HI_SUCCESS != ret)
    {
        printf("HI_UNF_IR_Init failed\n");
        return 0;
    }

    ret = HI_UNF_IR_Enable(HI_TRUE);
    if (HI_SUCCESS != ret)
    {
        printf("HI_UNF_IR_Enable failed\n");
        return 0;
    }

    ret = HI_UNF_IR_GetValue(&PressStatus, &KeyId, 400);
    if (HI_SUCCESS == ret)
    {
        if (0x6f90ff00 == KeyId)
        {
            printf("Android_Qb_CheckRemote Success!");
            ret_val = 1;
        }
    }

    ret = HI_UNF_IR_DeInit();
    if (HI_SUCCESS != ret)
    {
        printf("HI_UNF_IR_DeInit failed\n");
    }

    return ret_val;
}
#endif
