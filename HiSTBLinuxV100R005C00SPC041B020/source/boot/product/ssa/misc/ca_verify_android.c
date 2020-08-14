/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : ca_verify_android.c
Version       : Initial
Author        : Hisilicon hisecurity team
Created       : 2016-01-15
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "hi_type.h"
#ifdef HI_MINIBOOT_SUPPORT
#include "command.h"
#include "app.h"
#else
#include "exports.h"
#endif
#include "ca_debug.h"
#include "ca_flash.h"
#include "ca_adp.h"
#include "ca_verify_api.h"


#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3798cv200) \
     || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
static HI_U8 CAIMGHEAD_MAGICNUMBER[32]  =  "Hisilicon_ADVCA_ImgHead_MagicNum";
HI_U32 HI_CA_GetAuthMode(HI_CHAR* pPartionName)
{
    HI_S32 ret = 0;
    HI_U8 CAImageArea[CA_IMAGE_HEADER_AREA_LEN] = {0};
    HI_Flash_InterInfo_S flashInfo;
    HI_CAImgHead_S stImgInfo;

    CA_CheckPointer(pPartionName);
    CA_ASSERT(CA_flash_read(pPartionName, 0ULL, CA_IMAGE_HEADER_AREA_LEN,(HI_U8*)CAImageArea, &flashInfo),ret);
    memcpy(&stImgInfo,CAImageArea,sizeof(HI_CAImgHead_S));

    if(!memcmp(stImgInfo.u8MagicNumber,CAIMGHEAD_MAGICNUMBER,sizeof(stImgInfo.u8MagicNumber)))
    {
        HI_INFO_CA("Advca Signature!\n");
        return ADVCA_MODE;
    }
    else
    {
        HI_INFO_CA("Common Signature!\n");
        return COMMON_MODE;
    }
}


HI_S32 HI_Android_Auth_Recovery(HI_VOID)
{
    HI_S32 ret = 0;
    HI_BOOL CAFlag = HI_FALSE;
    HI_UNF_ADVCA_FLASH_TYPE_E enCAFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;
    HI_U32 recoveryAuthMode = COMMON_MODE;

    CA_ASSERT(HI_UNF_ADVCA_Init(), ret);
    CA_ASSERT(HI_UNF_ADVCA_GetSecBootStat(&CAFlag, &enCAFlashType), ret);
    if (CAFlag == HI_TRUE)
    {
        ret = run_command("ca_get_extern_rsa_key fastboot", 0);
        if (ret == -1)
        {
            HI_ERR_CA("Get extern rsa key failed!\n");
            return -1;
        }

        ret = run_command("ca_verify_bootpara bootargs", 0);
        if (ret == -1)
        {
            HI_ERR_CA("Fail to verify bootargs!\n");
            return -1;
        }

        recoveryAuthMode = HI_CA_GetAuthMode("recovery");

        if (recoveryAuthMode == ADVCA_MODE)
        {
            ret = run_command("advca_verify recovery", 0);
            if (ret == -1)
            {
                HI_ERR_CA("Fail to  verify recovery !\n");
                return -1;
            }
        }
        else
        {
            HI_ERR_CA("AuthMode Not support!\n");
            return -1;
        }
    }
    else
    {
        HI_SIMPLEINFO_CA("enter write OTP mode!\n");
    }

    return 0;
}


HI_S32 HI_Android_Authenticate(HI_VOID)
{
    HI_S32 ret = 0;
    HI_BOOL CAFlag = HI_FALSE;
    HI_UNF_ADVCA_FLASH_TYPE_E enCAFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;
    HI_U32 kernelAuthMode = COMMON_MODE;
    
    CA_ASSERT(HI_UNF_ADVCA_Init(), ret);
    CA_ASSERT(HI_UNF_ADVCA_GetSecBootStat(&CAFlag, &enCAFlashType), ret);
    if (CAFlag == HI_TRUE)
    {
        ret = run_command("ca_get_extern_rsa_key fastboot", 0);
        if (ret == -1)
        {
            HI_ERR_CA("Get extern rsa key failed!\n");
            return -1;
        }

        ret = run_command("ca_verify_bootpara bootargs", 0);
        if (ret == -1)
        {
            HI_ERR_CA("Verify bootargs failed!\n");
            return -1;
        }

        kernelAuthMode = HI_CA_GetAuthMode("kernel");

        if ( kernelAuthMode == ADVCA_MODE)
        {
            ret = run_command("advca_verify kernel", 0);
            if (ret == -1)
            {
                HI_ERR_CA("Fail to verify kernel!\n");
                return -1;
            }
        }        
        else
        {
            HI_ERR_CA("AuthMode Not support!\n");
            return -1;
        }
#ifdef HI_BOOT_CHECK_TRUSTEDCORE
        ret = run_command("advca_verify trustedcore", 0);
        if (ret == -1)
        {
            HI_ERR_CA("advca verify trustedcore failed!\n");
            return -1;
        }
#endif
    }
    else
    {
        HI_SIMPLEINFO_CA("enter write OTP mode!\n");
    }

    return 0;
}
#else
HI_S32 HI_Android_Authenticate(HI_VOID)
{
    HI_S32 ret = 0;
    HI_BOOL CAFlag = HI_FALSE;
    HI_U32 u32MemAddr = IMG_VERIFY_ADDRESS;
    HI_FLASH_TYPE_E EnvFlashType;
	HI_UNF_ADVCA_FLASH_TYPE_E enCAFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;	//not used!!

    EnvFlashType = CA_get_env_flash_type();

	CA_ASSERT(HI_UNF_ADVCA_Init(), ret);
	CA_ASSERT(HI_UNF_ADVCA_GetSecBootStat(&CAFlag, &enCAFlashType), ret);	/* parameter enCAFlashType is not used in this case */

    if (CAFlag == HI_TRUE)
    {
        ret = run_command("ca_get_extern_rsa_key fastboot", 0);
        if (ret == -1)
        {
            HI_ERR_CA("Fail to get extern rsa key!\n");
            return -1;
        }

        ret = run_command("ca_verify_bootpara bootargs", 0);
        if (ret == -1)
        {
            HI_ERR_CA("Fail to verify bootargs!\n");
            return -1;
        }

        ret = run_command("ca_verify recovery", 0);
        if (ret == -1)
        {
            HI_ERR_CA("Fail to verify recovery!\n");
            return -1;
        }

        ret = run_command("ca_verify kernel", 0);
        if (ret == -1)
        {
            HI_ERR_CA("Fail to verify kernel!\n");
            return -1;
        }

        if (EnvFlashType == HI_FLASH_TYPE_EMMC_0)
        {
            ret = run_command("ca_verify systemsign systemsign 0x3E8000", 0);
        }
        else
        {
            HI_U8 CAImageInfo[CA_IMAGE_HEADER_AREA_LEN];
            HI_CASignImageTail_S stSignImage;
            memset(CAImageInfo, 0x0, sizeof(CAImageInfo));
            memset(&stSignImage, 0x0, sizeof(stSignImage));
            
            CA_ASSERT(CA_flash_read("systemsign", 0UL, CA_IMAGE_HEADER_AREA_LEN,(HI_U8 *)CAImageInfo, HI_NULL),ret);
            memcpy(&stSignImage, CAImageInfo, sizeof(HI_CASignImageTail_S));
            u32MemAddr = IMG_VERIFY_ADDRESS;
            CA_ASSERT(HI_CA_Verify_Signature(u32MemAddr, "systemsign", "systemsign", stSignImage.u32TotalsectionID * NAND_PAGE_SIZE), ret);
        }

        if (ret == -1)
        {
            HI_ERR_CA("Fail to verify systemsign!\n");
            return -1;
        }

        ret = run_command("ca_verify_ran system systemsign 0x0", 0);
        if (ret == -1)
        {
            HI_ERR_CA("Fail to verify system!\n");
            return -1;
        }
    }
    else
    {
        HI_SIMPLEINFO_CA("enter write OTP mode!\n");
    }

    return 0;
}
#endif


