/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_getOtpFuse.c
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
#include "hi_type.h"
#include "hi_unf_advca.h"
#include "sample_ca_common.h"


HI_S32 main(HI_S32 argc,HI_CHAR** argv)
{
    HI_S32 Ret;
    HI_UNF_ADVCA_OTP_ATTR_S stOtpAttr;
    HI_BOOL bEnable;
    HI_UNF_ADVCA_FLASH_TYPE_E enFlashType;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enKeyladderLev;
    HI_UNF_ADVCA_JTAG_MODE_E enJtagMode;

    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed\n");
        return HI_FAILURE;
    }

    /*To get the CHIP ID*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_CHIP_ID, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get chip id failed\n");
        return HI_FAILURE;
    }    
    HI_DEBUG_ADVCA("Chip id:0x%02x %02x %02x %02x\n", stOtpAttr.unOtpFuseAttr.stChipId.au8ChipId[0], 
                                                      stOtpAttr.unOtpFuseAttr.stChipId.au8ChipId[1],
                                                      stOtpAttr.unOtpFuseAttr.stChipId.au8ChipId[2],
                                                      stOtpAttr.unOtpFuseAttr.stChipId.au8ChipId[3]);

    /*To check if the secure boot check is enabled, and get the startup flash*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get secure boot activation status failed\n");
        return HI_FAILURE;
    } 
    bEnable = stOtpAttr.unOtpFuseAttr.stEnableSecureBoot.bEnable;
    enFlashType = stOtpAttr.unOtpFuseAttr.stEnableSecureBoot.enFlashType;
    HI_DEBUG_ADVCA("Secure boot: %s\n",  bEnable == HI_TRUE ? "Enabled" : "NOT Enabled");
    HI_DEBUG_ADVCA("Startup flash mode: %s\n",  enFlashType ==  HI_UNF_ADVCA_FLASH_TYPE_SPI ? "SPI" : 
        (enFlashType ==  HI_UNF_ADVCA_FLASH_TYPE_NAND ? "NAND" : (enFlashType == HI_UNF_ADVCA_FLASH_TYPE_EMMC ? "EMMC" : 
        (enFlashType == HI_UNF_ADVCA_FLASH_TYPE_SPI_NAND ? "SPI_NAND" : 
        (enFlashType == HI_UNF_ADVCA_FLASH_TYPE_SD ? "SD" : "Unknow")))));

    /*To check if the secure boot encryption is enabled, if it is enabled, when the chipset
    startup, the chipset will always decrypt the secure boot.
    */
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get boot encryption status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("Boot encryption: %s\n",  bEnable == HI_TRUE ? "Enabled" : "NOT Enabled");

    /*To check if the self boot is disabled, if the self boot is disabled, the chipset will NOT
    be able to download the secure boot via UART or USB*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SELF_BOOT_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get selfboot status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("Selfboot activation: %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");

    /*To check if the DDR wake up is disabled, if it is disabled, when wake up from standby,
    the system will reset, it is a cold reset*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_DDR_WAKEUP_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get DDR wakeup deactivation status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("DDR wakeup status: %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");

    /*To get the CSA2 keyladder stage*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_CSA2_KL_LEVEL_SEL, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get CSA2 keyladder stage failed\n");
        return HI_FAILURE;
    }
    enKeyladderLev = stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel;
    HI_DEBUG_ADVCA("CSA2 keyladder stage: %d level\n",  enKeyladderLev + 1);

    /*To get the R2R keyladder stage*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_R2R_KL_LEVEL_SEL, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get R2R keyladder stage failed\n");
        return HI_FAILURE;
    }
    enKeyladderLev = stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel;
    HI_DEBUG_ADVCA("R2R keyladder stage: %d level\n",  enKeyladderLev + 1);   

    /*To get the SP keyladder stage*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SP_KL_LEVEL_SEL, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get SP keyladder stage failed\n");
        return HI_FAILURE;
    }
    enKeyladderLev = stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel;
    HI_DEBUG_ADVCA("SP keyladder stage: %d level\n",  enKeyladderLev + 1);

    /*To get the CSA3 keyladder stage*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_CSA3_KL_LEVEL_SEL, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get CSA3 keyladder stage failed\n");
        return HI_FAILURE;
    }
    enKeyladderLev = stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel;
    HI_DEBUG_ADVCA("CSA3 keyladder stage: %d level\n",  enKeyladderLev + 1);

    /*To check if the Link Protection Keyladder is disabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_LP_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get LP status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("Link Protection status: %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");

    /*To check if the CSA2 keyladder is accepted encrypted CW only*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_CSA2_CW_HARDONLY_ACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get CSA2 CW Hard Only status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("CSA2 CW Hard Only: %s\n",  bEnable == HI_TRUE ? "Enabled" : "NOT Enabled");    

    /*To check if the SP keyladder is accepted encrypted CW only*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SP_CW_HARDONLY_ACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get SP CW Hard Only status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("SP CW Hard Only: %s\n",  bEnable == HI_TRUE ? "Enabled" : "NOT Enabled");       

    /*To check if the CSA3 keyladder is accepted encrypted CW only*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_CSA3_CW_HARDONLY_ACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get CSA3 CW Hard Only status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("CSA3 CW Hard Only: %s\n",  bEnable == HI_TRUE ? "Enabled" : "NOT Enabled");

    /*To check if the CSA2 keyladder crypto engine is disabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_CSA2_KL_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get CSA2 KL Deactivation status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("CSA2 Keyladder: %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");

    /*To check if the SP keyladder crypto engine is disabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SP_KL_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get SP KL Deactivation status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("SP Keyladder: %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");    

    /*To check if the CSA3 keyladder crypto engine is disabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_CSA3_KL_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get CSA3 KL Deactivation status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("CSA3 Keyladder: %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");  

    /*To check if the MISC keyladder crypto engine is disabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_MISC_KL_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get MISC KL Deactivation status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("MISC Keyladder: %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");  

    /*To check if the google widevine keyladder crypto engine is disabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_GOOGLE_KL_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get Google KL Deactivation status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("Google Keyladder: %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");  

    /*To check if the DCAS keyladder crypto engine is disabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_DCAS_KL_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get DCAS KL Deactivation status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("DCAS Keyladder: %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");  

    /*To check if the DDR scrambled function is enabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_DDR_SCRAMBLE_ACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get DDR Scrambled status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("DDR Scrambled: %s\n",  bEnable == HI_TRUE ? "Enabled" : "NOT Enabled");     

    /*To check if the whole OTP area is locked, if it is lock, the OTP can't be
    programmed anymore.*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_GLOBAL_LOCK_ACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get OTP Global lock status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("OTP Global Lock: %s\n",  bEnable == HI_TRUE ? "Enabled" : "NOT Enabled");

    /*To check if the runtime check function is disabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_RUNTIME_CHECK_ACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get runtime check status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("Runtime check: %s\n",  bEnable == HI_TRUE ? "Enabled" : "NOT Enabled");    

    /*To check if the DDR wakeup check function is enabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_DDR_WAKEUP_CHECK_ACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get DDR Wakeup check status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("DDR Wakeup Check: %s\n",  bEnable == HI_TRUE ? "Enabled" : "NOT Enabled");

    /*To check if the Version ID Check function is enabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_VERSION_ID_CHECK_ACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get Version ID check status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("Version ID Check: %s\n",  bEnable == HI_TRUE ? "Enabled" : "NOT Enabled");

    /*To check if the MSID Check function is enabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_BOOT_MSID_CHECK_ACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get MSID check status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("MSID Check: %s\n",  bEnable == HI_TRUE ? "Enabled" : "NOT Enabled");
    
    /*To get the JTAG protection mode*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_JTAG_MODE, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get JTAG mode failed\n");
        return HI_FAILURE;
    }
    enJtagMode = stOtpAttr.unOtpFuseAttr.stJtagPrtMode.enJtagMode;
     HI_DEBUG_ADVCA("JTAG Protect mode: %s\n",  enJtagMode ==  HI_UNF_ADVCA_JTAG_MODE_OPEN ? "Open" : 
        (enJtagMode ==  HI_UNF_ADVCA_JTAG_MODE_PROTECT ? "Protected" : "Closed"));
        
    /*To check if it is able to read the OTP area via JTAG*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_JTAG_READ_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get JTAG Read Deactivation status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("Read OTP via JTAG: %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");

    /*To get the MSID in the OTP*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_MARKET_SEGMENT_ID, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get MSID failed\n");
        return HI_FAILURE;
    }
    HI_DEBUG_ADVCA("MSID: 0x%02x %02x %02x %02x\n",  stOtpAttr.unOtpFuseAttr.stMarketId.u8MSID[0],
                                                stOtpAttr.unOtpFuseAttr.stMarketId.u8MSID[1],
                                                stOtpAttr.unOtpFuseAttr.stMarketId.u8MSID[2],
                                                stOtpAttr.unOtpFuseAttr.stMarketId.u8MSID[3]); 
    
    /*To get the Version ID in the OTP*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_VERSION_ID, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get Version ID failed\n");
        return HI_FAILURE;
    }
    HI_DEBUG_ADVCA("Version ID: 0x%02x %02x %02x %02x\n",  stOtpAttr.unOtpFuseAttr.stVersionId.u8VersionId[0],
                                                stOtpAttr.unOtpFuseAttr.stVersionId.u8VersionId[1],
                                                stOtpAttr.unOtpFuseAttr.stVersionId.u8VersionId[2],
                                                stOtpAttr.unOtpFuseAttr.stVersionId.u8VersionId[3]);

    /*To check if the irdeto tweaked CSA3 is enabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_IRDETO_ITCSA3_ACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get irdeto tweaked CSA3 status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("Irdeto tweaked CSA3: %s\n",  bEnable == HI_TRUE ? "Enabled" : "Disabled");

    /*To get the irdeto tweaked CSA3 IMLB in the OTP*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_ITCSA3_IMLB, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get irdeto tweaked CSA3 IMLB failed\n");
        return HI_FAILURE;
    }
    HI_DEBUG_ADVCA("IMLB: 0x%02x %02x\n",  stOtpAttr.unOtpFuseAttr.stItCsa3IMLB.au8ItCsa3IMLB[0],
                                                stOtpAttr.unOtpFuseAttr.stItCsa3IMLB.au8ItCsa3IMLB[1]);    
#if defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    /*To check if the USB is disabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_USB_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get USB Deactivation status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("usb status : %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");

    /*To check if the serial port is diabled */
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SERIAL_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get serial port Deactivation status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("Serial port: %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");
	
    /*To check if the ethernet is diabled */
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_ETHERNET_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get ethernet Deactivation status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("ethernet status : %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");   


    /*To check if SM4 crypto engine is diabled */
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SM4_CRYPTO_ENGINE_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get SM4 crypto engine status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("SM4 crypto engine : %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");   

    /*To check if TDES crypto engine is diabled */
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_TDES_CRYPTO_ENGINE_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get TDES crypto engine status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("TDES crypto engine : %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");
#endif
    /*To check if the bootrom debug information is disabled*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_BOOTINFO_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get bootrom debug information status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("bootrom debug information : %s\n",  bEnable == HI_TRUE ? "Disabled" : "Enabled");

    /*To check if the RSA_Root_Key lock flag is setted*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get RSA_Root_Key lock status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("RSA_Root_Key : %s\n",  bEnable == HI_TRUE ? "Locked" : "UnLock");

    /*To check if the STBSN lock flag is setted*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_STBSN_LOCK_FLAG, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get STBSN lock status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("STBSN : %s\n",  bEnable == HI_TRUE ? "Locked" : "UnLock");

    /*To check if the Market ID lock flag is setted*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_MSID_LOCK_FLAG, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get Market ID lock status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("Market ID : %s\n",  bEnable == HI_TRUE ? "Locked" : "UnLock");

    /*To check if the Version ID lock flag is setted*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_VERSIONID_LOCK_FLAG, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get Version ID lock status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("Version ID : %s\n",  bEnable == HI_TRUE ? "Locked" : "UnLock");

    /*To check if the OEM_Root_Key lock flag is setted*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_OEM_ROOTKEY_LOCK_FLAG, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get OEM_Root_Key lock status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("OEM_Root_Key : %s\n",  bEnable == HI_TRUE ? "Locked" : "UnLock");

    /*To check if the R2R_Root_Key lock flag is setted*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_R2R_ROOTKEY_LOCK_FLAG, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get R2R_Root_Key lock status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("R2R_Root_Key : %s\n",  bEnable == HI_TRUE ? "Locked" : "UnLock");

    /*To check if the JTAG Key lock flag is setted*/
    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_JTAG_KEY_LOCK_FLAG, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get JTAG key lock status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("JTAG key : %s\n",  bEnable == HI_TRUE ? "Locked" : "UnLock");

    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_BOOT_ENCRYPTION_DEACTIVATION, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get Boot Encryption status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("Bootloader key encryption: %s\n",  bEnable == HI_TRUE ? "Disabled" : "enabled");

    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_BOOT_ROOTKEY_LOCK_FLAG, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get BootRoot key lock status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("BootRoot key : %s\n",  bEnable == HI_TRUE ? "Locked" : "UnLock");

    Ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SEC_STORE_ROOTKEY_LOCK_FLAG, &stOtpAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Get SecStore key lock status failed\n");
        return HI_FAILURE;
    }
    bEnable = stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
    HI_DEBUG_ADVCA("SecStore key : %s\n",  bEnable == HI_TRUE ? "Locked" : "UnLock");    


    (HI_VOID)HI_UNF_ADVCA_DeInit();
    return Ret;
}
