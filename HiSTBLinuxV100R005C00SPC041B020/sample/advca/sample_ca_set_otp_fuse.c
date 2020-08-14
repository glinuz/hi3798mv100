/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_set_otp_fuse.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2014-04-29
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hi_type.h"
#include "hi_unf_advca.h"
#include "sample_ca_common.h"

#define MAX_CMDLINE_LEN     1280
#define MAX_ARGS_COUNT      10
#define MAX_ARGS_LEN            128

static HI_CHAR  g_otp_fuse_info[][512] = {

"Unknown OTP fuse id!\n",

"[HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION] - This fuse is used to enable the secure boot authentication. \
After enabled, the chipset will always authenticate the secure boot when startup.\n",

"[HI_UNF_ADVCA_OTP_BOOT_DECRYPTION_ACTIVATION] - This fuse is used to enable the boot decryption control. \
After enabled, the chipset will always decrypt the secure boot when startup.\n",
    
"[HI_UNF_ADVCA_OTP_SELF_BOOT_DEACTIVATION] - This fuse is used to disable the self boot. \
After this fuse is set, the self boot is disabled. And the boot is not able to be downloaded via UART or USB.\n",

"[HI_UNF_ADVCA_OTP_DDR_WAKEUP_DEACTIVATION] - This fuse is used to disabled the DDR wakeup. \
After this fuse is set, the chipset will reset when wakeup from standby. It is a cold reset.\n",

"[HI_UNF_ADVCA_OTP_CSA2_KL_LEVEL_SEL] - This fuse is used to set the CSA2 keyladder stage.\n",
  
"[HI_UNF_ADVCA_OTP_R2R_KL_LEVEL_SEL] - This fuse is used to set the R2R keyladder stage.\n",   

"[HI_UNF_ADVCA_OTP_SP_KL_LEVEL_SEL] - This fuse is used to set the SP keyladder stage.\n",

"[HI_UNF_ADVCA_OTP_CSA3_KL_LEVEL_SEL] - This fuse is used to set the CSA3 keyladder stage.\n",

"[HI_UNF_ADVCA_OTP_LP_DEACTIVATION] - This fuse is used to disabled the Link Protection Keyladder.\n",

"[HI_UNF_ADVCA_OTP_CSA2_CW_HARDONLY_ACTIVATION] - After this fuse is set, the host CPU can ONLY \
accept the encrypted CW for the CSA2 descambling.\n",
    
"[HI_UNF_ADVCA_OTP_SP_CW_HARDONLY_ACTIVATION] - After this fuse is set, the host CPU can ONLY \
accept the encrypted CW for the SP keyladder.\n", 

"[HI_UNF_ADVCA_OTP_CSA3_CW_HARDONLY_ACTIVATION] - After this fuse is set, the host CPU can ONLY \
accept the encrypted CW for the CSA3 keyladder.\n", 
    
"[HI_UNF_ADVCA_OTP_CSA2_KL_DEACTIVATION] - This fuse is used to disabled the CSA2 keyladder crypto engine.\n",
    
"[HI_UNF_ADVCA_OTP_SP_KL_DEACTIVATION] - This fuse is used to disabled the SP keyladder crypto engine.\n", 
    
"[HI_UNF_ADVCA_OTP_CSA3_KL_DEACTIVATION] - This fuse is used to disabled the CSA3 keyladder crypto engine.\n", 
    
"[HI_UNF_ADVCA_OTP_MISC_KL_DEACTIVATION] - This fuse is used to disabled the MISC keyladder crypto engine.\n", 
    
"[HI_UNF_ADVCA_OTP_GOOGLE_KL_DEACTIVATION] - This fuse is used to disabled the Google keyladder crypto engine.\n", 
    
"[HI_UNF_ADVCA_OTP_DCAS_KL_DEACTIVATION] - This fuse is used to disabled the DCAS keyladder crypto engine.\n",    

"[HI_UNF_ADVCA_OTP_DDR_SCRAMBLE_ACTIVATION] - This fuse is used to enable the DDR scramble function.\n",    

"[HI_UNF_ADVCA_OTP_GLOBAL_LOCK_ACTIVATION] - This fuse is used to lock the whole OTP area. \
After this fuse is set, the OTP can NOT be programmed any more.\n",    

"[HI_UNF_ADVCA_OTP_RUNTIME_CHECK_ACTIVATION]- This fuse is used to enabled the runtime check function.\n",    

"[HI_UNF_ADVCA_OTP_DDR_WAKEUP_CHECK_ACTIVATION] - After this fuse is set, when wakeup from standby, \
the chipset will first authenticate the DDR content.\n",
    
"[HI_UNF_ADVCA_OTP_VERSION_ID_CHECK_ACTIVATION] - After this fuse is set, the chipset will always check if the \
the version id in the boot is equalled to or higher than the version id in the OTP when startup.\n",

"[HI_UNF_ADVCA_OTP_BOOT_MSID_CHECK_ACTIVATION] - After this fuse is set, the chipset will always check if the \
MSID in the boot is equalled to the MSID in the OTP.\n",    

"[HI_UNF_ADVCA_OTP_JTAG_MODE] - This fuse is used to set the JTAG protection mode.\n",

"[HI_UNF_ADVCA_OTP_JTAG_READ_DEACTIVATION] - After this fuse is set, the OTP can NOT be read via JTAG.\n",    

"[HI_UNF_ADVCA_OTP_CSA2_ROOTKEY] - This fuse is used to set the CSA2 root key in the OTP\n",    

"[HI_UNF_ADVCA_OTP_R2R_ROOTKEY] - This fuse is used to set the R2R root key in the OTP\n",    

"[HI_UNF_ADVCA_OTP_SP_ROOTKEY] - This fuse is used to set the SP root key in the OTP\n",    

"[HI_UNF_ADVCA_OTP_CSA3_ROOTKEY] - This fuse is used to set the CSA3 root key in the OTP\n",    

"[HI_UNF_ADVCA_OTP_MISC_ROOTKEY] - This fuse is used to set the MISC root key in the OTP\n",    

"[HI_UNF_ADVCA_OTP_OEM_ROOTKEY] - This fuse is used to set the OEM root key in the OTP\n",

"[HI_UNF_ADVCA_OTP_ESCK_ROOTKEY] - This fuse is used to set the ESCK root key in the OTP\n",    

"[HI_UNF_ADVCA_OTP_JTAG_KEY] - This fuse is used to set the JTAG root key in the OTP\n",

"[HI_UNF_ADVCA_OTP_CHIP_ID] - This fuse is used to set the CHIP ID in the OTP\n",

"[HI_UNF_ADVCA_OTP_MARKET_SEGMENT_ID] - This fuse is used to set the Market Segment ID in the OTP.\n",    

"[HI_UNF_ADVCA_OTP_VERSION_ID] - This fuse is used to set the Version ID in the OTP.\n", 

"[HI_UNF_ADVCA_OTP_MISC_KL_LEVEL_SEL]\n",

"[HI_UNF_ADVCA_OTP_VMX_BL_FUSE]\n",

"[HI_UNF_ADVCA_OTP_IRDETO_ITCSA3_ACTIVATION]\n",

"[HI_UNF_ADVCA_OTP_BOOTINFO_DEACTIVATION]\n",

"[HI_UNF_ADVCA_OTP_ITCSA3_IMLB]\n",

#if defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)

"[HI_UNF_ADVCA_OTP_USB_DEACTIVATION] - This fuse is used to disable usb in the OTP.\n",

"[HI_UNF_ADVCA_OTP_SERIAL_DEACTIVATION] - This fuse is used to disable serial port in the OTP.\n",

"[HI_UNF_ADVCA_OTP_ETHERNET_DEACTIVATION] - This fuse is used to disable ethernet in the OTP.\n",    

"[HI_UNF_ADVCA_OTP_SM4_CRYPTO_ENGINE_DEACTIVATION] - This fuse is used to disable SM4 crypto engine in the OTP.\n",    

"[HI_UNF_ADVCA_OTP_TDES_CRYPTO_ENGINE_DEACTIVATION] - This fuse is used to disable TDES crypto engine in the OTP.\n", 
#endif
};

HI_S32 parse_cmdline(HI_CHAR *pCmdLine, HI_S32 *pArgc, HI_CHAR **Argv)
{
    HI_CHAR *ptr = pCmdLine;
    HI_U32 i;

    while ((*ptr == ' ') && (*ptr++ != '\0'))
    {
        ;
    }

    for (i = strlen(ptr); i > 0; i--)
    {
        if ((*(ptr + i - 1) == 0x0a) || (*(ptr + i - 1) == ' '))
        {
            *(ptr + i - 1) = '\0';
        }
        else
        {
            break;
        }
    }

    for (i = 0; i < MAX_ARGS_COUNT; i++)
    {
        HI_U32 j = 0;
        while ((*ptr == ' ') && (*(++ptr) != '\0'))
        {
            ;
        }

        while ((*ptr != ' ') && (*ptr != '\0') && (j < MAX_ARGS_LEN))
        {
            Argv[i][j++] = *ptr++;
        }

        Argv[i][j] = '\0';
        if ('\0' == *ptr)
        {
            i++;
            break;
        }
    }
    *pArgc = i;

    return HI_SUCCESS;
}

HI_VOID show_usage(HI_VOID)
{
    HI_DEBUG_ADVCA("Usage:\n"
            "./sample_ca_set_otp_fuse [otp fuse id]\n"
            "./sample_ca_set_otp_fuse help [otp fuse id]\n");
    HI_DEBUG_ADVCA("\n[WARNING]: The OTP fuse can only be set once, please be careful to set the OTP fuse."
            "If you are not clear about the OTP fuse, please contact HiSilicon.\n");

    HI_DEBUG_ADVCA("\notp fuse id:\n\n");
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION\n", HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_BOOT_DECRYPTION_ACTIVATION\n", HI_UNF_ADVCA_OTP_BOOT_DECRYPTION_ACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_SELF_BOOT_DEACTIVATION\n", HI_UNF_ADVCA_OTP_SELF_BOOT_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_DDR_WAKEUP_DEACTIVATION\n", HI_UNF_ADVCA_OTP_DDR_WAKEUP_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_CSA2_KL_LEVEL_SEL\n", HI_UNF_ADVCA_OTP_CSA2_KL_LEVEL_SEL);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_R2R_KL_LEVEL_SEL\n", HI_UNF_ADVCA_OTP_R2R_KL_LEVEL_SEL);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_SP_KL_LEVEL_SEL\n", HI_UNF_ADVCA_OTP_SP_KL_LEVEL_SEL);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_CSA3_KL_LEVEL_SEL\n", HI_UNF_ADVCA_OTP_CSA3_KL_LEVEL_SEL);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_LP_DEACTIVATION\n", HI_UNF_ADVCA_OTP_LP_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_CSA2_CW_HARDONLY_ACTIVATION\n", HI_UNF_ADVCA_OTP_CSA2_CW_HARDONLY_ACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_SP_CW_HARDONLY_ACTIVATION\n", HI_UNF_ADVCA_OTP_SP_CW_HARDONLY_ACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_CSA3_CW_HARDONLY_ACTIVATION\n", HI_UNF_ADVCA_OTP_CSA3_CW_HARDONLY_ACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_CSA2_KL_DEACTIVATION\n", HI_UNF_ADVCA_OTP_CSA2_KL_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_SP_KL_DEACTIVATION\n", HI_UNF_ADVCA_OTP_SP_KL_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_CSA3_KL_DEACTIVATION\n", HI_UNF_ADVCA_OTP_CSA3_KL_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_MISC_KL_DEACTIVATION\n", HI_UNF_ADVCA_OTP_MISC_KL_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_GOOGLE_KL_DEACTIVATION\n", HI_UNF_ADVCA_OTP_GOOGLE_KL_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_DCAS_KL_DEACTIVATION\n", HI_UNF_ADVCA_OTP_DCAS_KL_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_DDR_SCRAMBLE_ACTIVATION\n", HI_UNF_ADVCA_OTP_DDR_SCRAMBLE_ACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_GLOBAL_LOCK_ACTIVATION\n", HI_UNF_ADVCA_OTP_GLOBAL_LOCK_ACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_RUNTIME_CHECK_ACTIVATION\n", HI_UNF_ADVCA_OTP_RUNTIME_CHECK_ACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_DDR_WAKEUP_CHECK_ACTIVATION\n", HI_UNF_ADVCA_OTP_DDR_WAKEUP_CHECK_ACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_VERSION_ID_CHECK_ACTIVATION\n", HI_UNF_ADVCA_OTP_VERSION_ID_CHECK_ACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_BOOT_MSID_CHECK_ACTIVATION\n", HI_UNF_ADVCA_OTP_BOOT_MSID_CHECK_ACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_JTAG_MODE\n", HI_UNF_ADVCA_OTP_JTAG_MODE);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_JTAG_READ_DEACTIVATION\n", HI_UNF_ADVCA_OTP_JTAG_READ_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_CSA2_ROOTKEY\n", HI_UNF_ADVCA_OTP_CSA2_ROOTKEY);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_R2R_ROOTKEY\n", HI_UNF_ADVCA_OTP_R2R_ROOTKEY);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_SP_ROOTKEY\n", HI_UNF_ADVCA_OTP_SP_ROOTKEY);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_CSA3_ROOTKEY\n", HI_UNF_ADVCA_OTP_CSA3_ROOTKEY);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_MISC_ROOTKEY\n", HI_UNF_ADVCA_OTP_MISC_ROOTKEY);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_OEM_ROOTKEY\n", HI_UNF_ADVCA_OTP_OEM_ROOTKEY);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_ESCK_ROOTKEY\n", HI_UNF_ADVCA_OTP_ESCK_ROOTKEY);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_JTAG_KEY\n", HI_UNF_ADVCA_OTP_JTAG_KEY);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_CHIP_ID\n", HI_UNF_ADVCA_OTP_CHIP_ID);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_MARKET_SEGMENT_ID\n", HI_UNF_ADVCA_OTP_MARKET_SEGMENT_ID);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_VERSION_ID\n", HI_UNF_ADVCA_OTP_VERSION_ID);
#if defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_USB_DEACTIVATION\n", HI_UNF_ADVCA_OTP_USB_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_SERIAL_DEACTIVATION\n", HI_UNF_ADVCA_OTP_SERIAL_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_ETHERNET_DEACTIVATION\n", HI_UNF_ADVCA_OTP_ETHERNET_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_SM4_CRYPTO_ENGINE_DEACTIVATION\n", HI_UNF_ADVCA_OTP_SM4_CRYPTO_ENGINE_DEACTIVATION);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_TDES_CRYPTO_ENGINE_DEACTIVATION\n", HI_UNF_ADVCA_OTP_TDES_CRYPTO_ENGINE_DEACTIVATION);
#endif
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_BOOT_ENCRYPTION_DEACTIVATION\n", HI_UNF_ADVCA_OTP_BOOT_ENCRYPTION_DEACTIVATION);
    
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_BOOT_ROOTKEY\n", HI_UNF_ADVCA_OTP_BOOT_ROOTKEY);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_SEC_STORE_ROOTKEY\n", HI_UNF_ADVCA_OTP_SEC_STORE_ROOTKEY);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_BOOT_ROOTKEY_LOCK_FLAG\n", HI_UNF_ADVCA_OTP_BOOT_ROOTKEY_LOCK_FLAG);
    HI_DEBUG_ADVCA("%d: HI_UNF_ADVCA_OTP_SEC_STORE_ROOTKEY_LOCK_FLAG\n", HI_UNF_ADVCA_OTP_SEC_STORE_ROOTKEY_LOCK_FLAG);                
}

HI_VOID show_help(HI_U32 u32otpfuse)
{
    if(u32otpfuse == 0)
    {
        HI_DEBUG_ADVCA("help:\n"
                "./sample_ca_set_otp_fuse [otp fuse id]\n\n");
    }
    else
    {
            if(u32otpfuse >= HI_UNF_ADVCA_OTP_FUSE_BUTT)
            {
                u32otpfuse = 0;
            }
            HI_DEBUG_ADVCA("%d: %s\n", u32otpfuse, g_otp_fuse_info[u32otpfuse]);
    }
}

HI_S32 run_cmdline(HI_S32 argc, HI_CHAR** argv)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 u32otpfuse;
    HI_CHAR cUserInput[512];
    HI_UNF_ADVCA_OTP_ATTR_S stOtpAttr;
    HI_UNF_ADVCA_FLASH_TYPE_E enFlash;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enKeyladderLevel;
    HI_UNF_ADVCA_JTAG_MODE_E enJtagMode;
    HI_U32 i;
    HI_CHAR cTmpBuf[16];
    HI_U32 u32MSID;
    HI_U32 u32VersionId;
    HI_U32 u32Flag;

    if (argc == 2 && strcmp(argv[1], "help") == 0)
    {
        for(i = 1; i < HI_UNF_ADVCA_OTP_FUSE_BUTT; i++)
        {
            show_help(i);
        }
        return HI_SUCCESS;
    } 
    else if (argc == 3 && strcmp(argv[1], "help") == 0)
    {
        u32otpfuse = strtol(argv[2], NULL, 0);
        show_help(u32otpfuse);
        return HI_SUCCESS;
    }
    else
    {
        u32otpfuse = strtol(argv[1], NULL, 0);

        if(u32otpfuse == 0 || u32otpfuse >= HI_UNF_ADVCA_OTP_FUSE_BUTT)
        {
            HI_DEBUG_ADVCA("Unknown OTP fuse ID!\n");
            return HI_FAILURE;
        }
        
        HI_DEBUG_ADVCA("%s\n", g_otp_fuse_info[u32otpfuse]);
        HI_DEBUG_ADVCA("Are you sure to set this fuse? (y/n)\n");
        fgets(cUserInput, 512, stdin);
        if(cUserInput[0] == 'y' || cUserInput[0] == 'Y')
        {
            //do nothing
        }
        else if(cUserInput[0] == 'n' || cUserInput[0] == 'N')
        {
            return HI_SUCCESS;
        }
        else
        {
            HI_DEBUG_ADVCA("Unknown input!\n");
            return HI_FAILURE;
        }
        
        switch(u32otpfuse)
        {
        case HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION: 

            HI_DEBUG_ADVCA("Please input the startup flash: [0] - SPI, [1] - NAND, [2] - SD, [3] - EMMC\n");
            fgets(cUserInput, 512, stdin);
            enFlash = (HI_UNF_ADVCA_FLASH_TYPE_E)strtol(cUserInput, NULL, 0);
            
            stOtpAttr.unOtpFuseAttr.stEnableSecureBoot.enFlashType = enFlash;
            Ret = HI_UNF_ADVCA_SetOtpFuse((HI_UNF_ADVCA_OTP_FUSE_E)u32otpfuse, &stOtpAttr);
            break;
            
        case HI_UNF_ADVCA_OTP_BOOT_DECRYPTION_ACTIVATION: 
        case HI_UNF_ADVCA_OTP_SELF_BOOT_DEACTIVATION: 
        case HI_UNF_ADVCA_OTP_DDR_WAKEUP_DEACTIVATION: 
        case HI_UNF_ADVCA_OTP_LP_DEACTIVATION: 
        case HI_UNF_ADVCA_OTP_CSA2_CW_HARDONLY_ACTIVATION: 
        case HI_UNF_ADVCA_OTP_SP_CW_HARDONLY_ACTIVATION: 
        case HI_UNF_ADVCA_OTP_CSA3_CW_HARDONLY_ACTIVATION: 
        case HI_UNF_ADVCA_OTP_CSA2_KL_DEACTIVATION: 
        case HI_UNF_ADVCA_OTP_SP_KL_DEACTIVATION:
        case HI_UNF_ADVCA_OTP_CSA3_KL_DEACTIVATION: 
        case HI_UNF_ADVCA_OTP_MISC_KL_DEACTIVATION: 
        case HI_UNF_ADVCA_OTP_GOOGLE_KL_DEACTIVATION: 
        case HI_UNF_ADVCA_OTP_DCAS_KL_DEACTIVATION:
        case HI_UNF_ADVCA_OTP_DDR_SCRAMBLE_ACTIVATION: 
        case HI_UNF_ADVCA_OTP_RUNTIME_CHECK_ACTIVATION: 
        case HI_UNF_ADVCA_OTP_DDR_WAKEUP_CHECK_ACTIVATION: 
        case HI_UNF_ADVCA_OTP_VERSION_ID_CHECK_ACTIVATION: 
        case HI_UNF_ADVCA_OTP_BOOT_MSID_CHECK_ACTIVATION: 
        case HI_UNF_ADVCA_OTP_JTAG_READ_DEACTIVATION:
#if defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)            
        case HI_UNF_ADVCA_OTP_SERIAL_DEACTIVATION:
        case HI_UNF_ADVCA_OTP_ETHERNET_DEACTIVATION:
        case HI_UNF_ADVCA_OTP_SM4_CRYPTO_ENGINE_DEACTIVATION:
        case HI_UNF_ADVCA_OTP_TDES_CRYPTO_ENGINE_DEACTIVATION:
#endif      
            Ret = HI_UNF_ADVCA_SetOtpFuse((HI_UNF_ADVCA_OTP_FUSE_E)u32otpfuse, &stOtpAttr);
            break;
            
        case HI_UNF_ADVCA_OTP_CSA2_KL_LEVEL_SEL: 
        case HI_UNF_ADVCA_OTP_R2R_KL_LEVEL_SEL: 
        case HI_UNF_ADVCA_OTP_SP_KL_LEVEL_SEL: 
        case HI_UNF_ADVCA_OTP_CSA3_KL_LEVEL_SEL: 

            HI_DEBUG_ADVCA("Please input the keyladder stage: [0] - 2 level, [1] - 3 level\n");
            fgets(cUserInput, 512, stdin);
            enKeyladderLevel = (HI_UNF_ADVCA_KEYLADDER_LEV_E)strtol(cUserInput, NULL, 0) + 1;
            
            stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel = enKeyladderLevel;
            Ret = HI_UNF_ADVCA_SetOtpFuse((HI_UNF_ADVCA_OTP_FUSE_E)u32otpfuse, &stOtpAttr);
            break;

        case HI_UNF_ADVCA_OTP_JTAG_MODE: 
            
            HI_DEBUG_ADVCA("Please input the JTAG protection mode: [0] - Open, [1] - Protected, [2] - Closed\n");
            fgets(cUserInput, 512, stdin);
            enJtagMode = (HI_UNF_ADVCA_JTAG_MODE_E)strtol(cUserInput, NULL, 0);
            
            stOtpAttr.unOtpFuseAttr.stJtagPrtMode.enJtagMode = enJtagMode;
            Ret = HI_UNF_ADVCA_SetOtpFuse((HI_UNF_ADVCA_OTP_FUSE_E)u32otpfuse, &stOtpAttr);
            break;
            
        case HI_UNF_ADVCA_OTP_CSA2_ROOTKEY: 
        case HI_UNF_ADVCA_OTP_R2R_ROOTKEY: 
        case HI_UNF_ADVCA_OTP_SP_ROOTKEY: 
        case HI_UNF_ADVCA_OTP_CSA3_ROOTKEY: 
        case HI_UNF_ADVCA_OTP_MISC_ROOTKEY: 
        case HI_UNF_ADVCA_OTP_OEM_ROOTKEY: 
        case HI_UNF_ADVCA_OTP_ESCK_ROOTKEY: 
        case HI_UNF_ADVCA_OTP_BOOT_ROOTKEY:
        case HI_UNF_ADVCA_OTP_SEC_STORE_ROOTKEY:

            HI_DEBUG_ADVCA("Please input the root key, the length of the root key is 16Byte.\n"
                "Input the root key as the following format (32 characters):\n"
                "00112233445566778899AABBCCDDEEFF\n");
            
            fgets(cUserInput, 512, stdin);
            for(i = 0; i < 16; i++)
            {
                memset(cTmpBuf, 0, sizeof(cTmpBuf));
                memcpy(cTmpBuf, cUserInput + i * 2, 2);
                stOtpAttr.unOtpFuseAttr.stKeyladderRootKey.u8RootKey[i] = strtol(cTmpBuf, NULL, 16);
            }           

            Ret = HI_UNF_ADVCA_SetOtpFuse((HI_UNF_ADVCA_OTP_FUSE_E)u32otpfuse, &stOtpAttr);
            break;
            
        case HI_UNF_ADVCA_OTP_JTAG_KEY: 

            HI_DEBUG_ADVCA("Please input the JTAG key, the length of the root key is 8Byte.\n"
                "Input the root key as the following format (16 characters):\n"
                "1122334455667788\n");
            
            fgets(cUserInput, 512, stdin);
            for(i = 0; i < 8; i++)
            {
                memset(cTmpBuf, 0, sizeof(cTmpBuf));
                memcpy(cTmpBuf, cUserInput + i * 2, 2);
                stOtpAttr.unOtpFuseAttr.stKeyladderRootKey.u8RootKey[i] = strtol(cTmpBuf, NULL, 16);
            }

            Ret = HI_UNF_ADVCA_SetOtpFuse((HI_UNF_ADVCA_OTP_FUSE_E)u32otpfuse, &stOtpAttr);
            break;

        case HI_UNF_ADVCA_OTP_CHIP_ID:
            
            HI_DEBUG_ADVCA("Please input the CHIP ID, it is a 32 bit integer, for example, 12345678\n");
            HI_U32 u32ChipId = 0;
            fgets(cUserInput, 512, stdin);
            u32ChipId = strtol(cUserInput, NULL, 16);
            stOtpAttr.unOtpFuseAttr.stChipId.au8ChipId[0] = (HI_U8)(u32ChipId >> 24);
            stOtpAttr.unOtpFuseAttr.stChipId.au8ChipId[1] = (HI_U8)(u32ChipId >> 16);
            stOtpAttr.unOtpFuseAttr.stChipId.au8ChipId[2] = (HI_U8)(u32ChipId >> 8);
            stOtpAttr.unOtpFuseAttr.stChipId.au8ChipId[3] = (HI_U8)u32ChipId;

            Ret = HI_UNF_ADVCA_SetOtpFuse((HI_UNF_ADVCA_OTP_FUSE_E)u32otpfuse, &stOtpAttr);
            break;

        case HI_UNF_ADVCA_OTP_MARKET_SEGMENT_ID: 
            
            HI_DEBUG_ADVCA("Please input the MSID, it is a 32 bit integer, for example, 12345678\n");
            fgets(cUserInput, 512, stdin);
            u32MSID = strtol(cUserInput, NULL, 16);
            
            stOtpAttr.unOtpFuseAttr.stMarketId.u8MSID[0] = (HI_U8)(u32MSID >> 24);
            stOtpAttr.unOtpFuseAttr.stMarketId.u8MSID[1] = (HI_U8)(u32MSID >> 16);
            stOtpAttr.unOtpFuseAttr.stMarketId.u8MSID[2] = (HI_U8)(u32MSID >> 8);
            stOtpAttr.unOtpFuseAttr.stMarketId.u8MSID[3] = (HI_U8)u32MSID;

            Ret = HI_UNF_ADVCA_SetOtpFuse((HI_UNF_ADVCA_OTP_FUSE_E)u32otpfuse, &stOtpAttr);
            break;
            
        case HI_UNF_ADVCA_OTP_VERSION_ID:

            HI_DEBUG_ADVCA("Please input the Version ID, it is a 32 bit integer, for example, 12345678\n");
            fgets(cUserInput, 512, stdin);
            u32VersionId = strtol(cUserInput, NULL, 16);
            
            stOtpAttr.unOtpFuseAttr.stVersionId.u8VersionId[0] = (HI_U8)(u32VersionId >> 24);
            stOtpAttr.unOtpFuseAttr.stVersionId.u8VersionId[1] = (HI_U8)(u32VersionId >> 16);
            stOtpAttr.unOtpFuseAttr.stVersionId.u8VersionId[2] = (HI_U8)(u32VersionId >> 8);
            stOtpAttr.unOtpFuseAttr.stVersionId.u8VersionId[3] = (HI_U8)u32VersionId;

            Ret = HI_UNF_ADVCA_SetOtpFuse((HI_UNF_ADVCA_OTP_FUSE_E)u32otpfuse, &stOtpAttr);
            break;
        case HI_UNF_ADVCA_OTP_GLOBAL_LOCK_ACTIVATION:
            HI_DEBUG_ADVCA("Please input 1 to set GLOBAL_LOCK or 0 to not set GLOBAL_LOCK\n");
            fgets(cUserInput, 512, stdin);
            u32Flag = strtol(cUserInput, NULL, 16);
            stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = ((u32Flag == 0) ? 0 : 1);
            Ret = HI_UNF_ADVCA_SetOtpFuse((HI_UNF_ADVCA_OTP_FUSE_E)u32otpfuse, &stOtpAttr);
            break;
        case HI_UNF_ADVCA_OTP_BOOT_ENCRYPTION_DEACTIVATION:
            HI_DEBUG_ADVCA("Please input 1 to set bload_enc_disable or 0 to not set bload_enc_disable\n");
            fgets(cUserInput, 512, stdin);
            u32Flag = strtol(cUserInput, NULL, 16);
            stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = ((u32Flag == 0) ? 0 : 1);
            Ret = HI_UNF_ADVCA_SetOtpFuse((HI_UNF_ADVCA_OTP_FUSE_E)u32otpfuse, &stOtpAttr);
            break;
#if defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)            
        case HI_UNF_ADVCA_OTP_USB_DEACTIVATION:
            HI_DEBUG_ADVCA("Please input 1 to set usb disable or 0 to not set usb disable\n");
            fgets(cUserInput, 512, stdin);
            u32Flag = strtol(cUserInput, NULL, 16);
            stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = ((u32Flag == 0) ? 0 : 1);
            Ret = HI_UNF_ADVCA_SetOtpFuse((HI_UNF_ADVCA_OTP_FUSE_E)u32otpfuse, &stOtpAttr);            
            break;
#endif            
        default:
            HI_DEBUG_ADVCA("not support num!!!\n");
            Ret = HI_FAILURE;
            break;
        }
    }

    if(Ret == HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("Set the OTP fuse successfully!\n");
    }
    else
    {
        HI_DEBUG_ADVCA("Set the OTP fuse failed!\n");
    }

    return Ret;    
}


HI_S32 main(HI_S32 argc, HI_CHAR** argv)
{
    HI_S32 Ret;

    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed\n");
        return HI_FAILURE;
    }

    if(argc <= 1)
    {
        show_usage();
        Ret = HI_FAILURE;
        goto DEINIT; 
    }

    (HI_VOID)run_cmdline(argc, argv);

DEINIT:
    (HI_VOID)HI_UNF_ADVCA_DeInit();
    return Ret;
}


