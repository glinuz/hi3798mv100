/******************************************************************************

          Copyright (C), 2001-2015, Hisilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : dcas_ssa.c
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2016-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2016-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#ifdef HI_MINIBOOT_SUPPORT
#include <cpuinfo.h>
#include "run.h"
#include "boot.h"
#else
#include "common.h"
#endif

#ifdef HI_BUILD_WITH_KEYLED
#include "hi_unf_keyled.h"
#endif

#include "hi_drv_pdm.h"
#include "hi_drv_pq.h"
#include "hi_boot_logo.h"
#include "hi_boot_adp.h"
#include "hi_go_common.h"
#include "hi_go_logo.h"

#include "dcas_debug.h"
#include "dcas_config.h"
#include "dcas_image.h"
#include "dcas_verify.h"
#include "loader_dbinfo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) \
    || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv410) \
    || defined(CHIP_TYPE_hi3716mv420)
#define SSA_HDMI_SUPPORT (1)
#endif

#ifdef CONFIG_USB_HOST_BOOTSTRAP
#define LOADER_APPLOADER_IMAGENAME      "apploader.bin"

extern void selfboot_init(void);
extern unsigned int get_selfboot_type(void);
extern int download_bootargs(void);
extern int download_image(const char* imagename);
#endif

#define SSA_LEN_COMMAND                  (1024)

#ifdef HI_BUILD_WITH_KEYLED

#define SSA_KEY_TRY_TIMES                (50)
#define SSA_KEY_TRY_INTERVAL             (10000) /** us */

#if defined(HI_BOOT_KEYLED_GPIOKEY) && defined(CHIP_TYPE_hi3716mv420)
#define SSA_KEY_VALUE_MENU               (0x40)
#define SSA_KEY_VALUE_OK                 (0x2)
#elif defined(HI_BOOT_KEYLED_GPIOKEY) && defined(CHIP_TYPE_hi3798cv200)
#define SSA_KEY_VALUE_OK                 (0x1)
#else
#define SSA_KEY_VALUE_MENU               (0x0)
#define SSA_KEY_VALUE_OK                 (0x1)
#endif

static HI_U32 s_auMandatoryUpgradeKey[] =
{
#if defined(HI_BOOT_KEYLED_GPIOKEY) && defined(CHIP_TYPE_hi3798cv200)
    SSA_KEY_VALUE_OK,
#else
    SSA_KEY_VALUE_MENU,
    SSA_KEY_VALUE_OK,
#endif
};

static HI_S32 ssaMandatoryUpgradeCheck(HI_VOID)
{
    HI_S32 i = 0;
    HI_U32 u32PressStatus = 0;
    HI_U32 u32KeyValue = 0;
    HI_S32 s32KeyIndex = 0;
    HI_S32 s32KeyCount = 0;

    HI_S32 s32Ret = HI_FAILURE;

    s32KeyCount = sizeof(s_auMandatoryUpgradeKey) / sizeof(HI_U32);

    for (s32KeyIndex = 0; s32KeyIndex < s32KeyCount; s32KeyIndex++)
    {
        for (i = 0; i < SSA_KEY_TRY_TIMES; i++)
        {
            s32Ret = HI_UNF_KEY_GetValue(&u32PressStatus, &u32KeyValue);
            if (HI_SUCCESS != s32Ret)
            {
                udelay(SSA_KEY_TRY_INTERVAL);
                continue;
            }

            if (0x1 != u32PressStatus)
            {
                udelay(SSA_KEY_TRY_INTERVAL);
                continue;
            }

            if (s_auMandatoryUpgradeKey[s32KeyIndex] == u32KeyValue)
            {
                break;
            }
        }

        if (SSA_KEY_TRY_TIMES == i)
        {
            return HI_FAILURE;
        }

        udelay(SSA_KEY_TRY_INTERVAL);
    }

    return HI_SUCCESS;
}

#endif

static HI_S32 ssaBootM(HI_U8* pu8MemBuffer)
{
#ifdef HI_MINIBOOT_SUPPORT
    kern_load((HI_CHAR*)(pu8MemBuffer));
#else
    HI_CHAR as8BootCmd[SSA_LEN_COMMAND] = {0};

    snprintf(as8BootCmd, sizeof(as8BootCmd), "bootm 0x%x", (HI_U32)pu8MemBuffer);
    run_command(as8BootCmd, 0);
#endif

    return HI_SUCCESS;
}

static HI_S32 ssaUpgradeCheck(HI_BOOL* pbNeedUpgrade)
{
    LDDB_LOADER_INFO_S stInfoParam;
    HI_S32 s32Ret = HI_FAILURE;

#ifdef CONFIG_USB_HOST_BOOTSTRAP
    if (SELF_BOOT_TYPE_USBHOST == get_selfboot_type())
    {
        selfboot_init();

        s32Ret = download_bootargs();
        if (HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(download_bootargs, s32Ret);
            return s32Ret;
        }

        memset(&stInfoParam, 0x00, sizeof(stInfoParam));
        s32Ret = LDDB_GetLoaderInfo(&stInfoParam);
        if (HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
            memset(&stInfoParam, 0x00, sizeof(stInfoParam));
        }

        stInfoParam.stTAGParamData.bNeedUpgrade = HI_TRUE;
        stInfoParam.stTAGParamData.bUsbBurn = HI_TRUE;
        s32Ret = LDDB_SetLoaderInfo(&stInfoParam);
        if (HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
            return s32Ret;
        }

#ifdef HI_APPLOADER_SUPPORT
        s32Ret = download_image(LOADER_APPLOADER_IMAGENAME);
        if (HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(download_image, s32Ret);
            return s32Ret;
        }
#endif
        *pbNeedUpgrade = HI_TRUE;
        return HI_SUCCESS;
    }
#endif

#ifdef HI_BUILD_WITH_KEYLED
    s32Ret = ssaMandatoryUpgradeCheck();
    if (HI_SUCCESS == s32Ret)
    {
        memset(&stInfoParam, 0x00, sizeof(stInfoParam));
        s32Ret = LDDB_GetLoaderInfo(&stInfoParam);
        if (HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
            memset(&stInfoParam, 0x00, sizeof(stInfoParam));
        }

        stInfoParam.stTAGParamData.bNeedUpgrade = HI_TRUE;
        stInfoParam.stTAGParamData.bForceUpgrade = HI_TRUE;
        s32Ret = LDDB_SetLoaderInfo(&stInfoParam);
        if (HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
            return s32Ret;
        }

        *pbNeedUpgrade = HI_TRUE;
        return HI_SUCCESS;
    }
#endif

    memset(&stInfoParam, 0x00, sizeof(stInfoParam));
    s32Ret = LDDB_GetLoaderInfo(&stInfoParam);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(LDDB_GetLoaderInfo, s32Ret);
        return s32Ret;
    }

    *pbNeedUpgrade = stInfoParam.stTAGParamData.bNeedUpgrade;

    return HI_SUCCESS;
}

HI_S32 ssaShowLogo(HI_VOID)
{
    HI_GO_LOGO_INFO stLogoInfo;
    HI_LOGO_PARAM_S stLogoParam;
    HI_DISP_PARAM_S stDispParam0;
    HI_DISP_PARAM_S stDispParam1;
    SSA_FLASH_OPENPARAM_S stDataParam;

    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_DRV_PDM_GetLogoParam(&stLogoParam);
    if(HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_DRV_PDM_GetLogoParam, s32Ret);
        return SSA_ERR_EXTERNAL;
    }

    if(1 != stLogoParam.u32CheckFlag)
    {
        SSA_DBG_FuncExit();
        return HI_SUCCESS;
    }

    memset(&stDispParam0, 0x00, sizeof(stDispParam0));
    memset(&stDispParam1, 0x00, sizeof(stDispParam1));

    s32Ret = HI_ADP_DispInit();
    if(HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_ADP_DispInit, s32Ret);
        return SSA_ERR_EXTERNAL;
    }

    memset(&stDispParam0, 0x00, sizeof(stDispParam0));
    s32Ret = HI_DRV_PDM_GetDispParam(HI_UNF_DISPLAY0, &stDispParam0);
    if(HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_DRV_PDM_GetDispParam, s32Ret);
        return SSA_ERR_EXTERNAL;
    }

    memset(&stDispParam1, 0x00, sizeof(stDispParam1));
    s32Ret = HI_DRV_PDM_GetDispParam(HI_UNF_DISPLAY1, &stDispParam1);
    if(HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_DRV_PDM_GetDispParam, s32Ret);
        return SSA_ERR_EXTERNAL;
    }

#if SSA_HDMI_SUPPORT
    s32Ret = HI_UNF_HDMI_Init();
    if(HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_UNF_HDMI_Init, s32Ret);
        return SSA_ERR_EXTERNAL;
    }

    if ((HI_UNF_DISP_INTF_TYPE_HDMI == stDispParam1.stIntf[HI_UNF_DISP_INTF_TYPE_HDMI].enIntfType)
        &&(HI_UNF_HDMI_ID_0 == stDispParam1.stIntf[HI_UNF_DISP_INTF_TYPE_HDMI].unIntf.enHdmi))
    {
        s32Ret = HI_UNF_HDMI_Open(HI_UNF_HDMI_ID_0, stDispParam1.enFormat);
        if(HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(HI_UNF_HDMI_Open, s32Ret);
            HI_UNF_HDMI_DeInit();
            return SSA_ERR_EXTERNAL;
        }
    }
#endif

    memset(&stDataParam, 0x00, sizeof(stDataParam));
    stDataParam.pcPartitionName = SSA_PARTNAME_LOGO;
    stDataParam.enFlashType     = HI_FLASH_TYPE_BUTT;

    s32Ret = SSA_IMG_Read(&stDataParam,
                          HI_NULL_PTR,
                          SSA_MEM_ADDR_VERIFY1,
                          SSA_MEM_SIZE_VERIFY_DATA,
                          SSA_MEM_ADDR_VERIFY1_SIGN,
                          SSA_MEM_SIZE_VERIFY_SIGN);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_IMG_Read, s32Ret);
        return SSA_ERR_EXTERNAL;
    }

    memset(&stLogoInfo, 0x00, sizeof(stLogoInfo));
    stLogoInfo.DispToSD     = (HI_UNF_DISPLAY1 != stDispParam0.enSrcDisp) ? HI_TRUE : HI_FALSE;
    stLogoInfo.Support4KLogo = HI_FALSE;
    stLogoInfo.LogoDataLen  = stLogoParam.u32LogoLen;
    stLogoInfo.LogoDataBuf  = (HI_S32)SSA_MEM_ADDR_VERIFY1 + MCE_DEF_LOGOPARAM_SIZE;

    stLogoInfo.DispColorFmt[CONFIG_GFX_DISP_LOGO_SD] = stDispParam0.enPixelFormat;
    stLogoInfo.DispVirtScreenWidth[CONFIG_GFX_DISP_LOGO_SD]  = stDispParam0.u32VirtScreenWidth;
    stLogoInfo.DispVirtScreenHeight[CONFIG_GFX_DISP_LOGO_SD] = stDispParam0.u32VirtScreenHeight;

    stLogoInfo.DispColorFmt[CONFIG_GFX_DISP_LOGO_HD] = stDispParam1.enPixelFormat;
    stLogoInfo.DispVirtScreenWidth[CONFIG_GFX_DISP_LOGO_HD]  = stDispParam1.u32VirtScreenWidth;
    stLogoInfo.DispVirtScreenHeight[CONFIG_GFX_DISP_LOGO_HD] = stDispParam1.u32VirtScreenHeight;

    s32Ret = HI_GO_DisplayLogo(&stLogoInfo);
    if(HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_GO_DisplayLogo, s32Ret);

#if SSA_HDMI_SUPPORT
        HI_UNF_HDMI_Close(HI_UNF_HDMI_ID_0);
        HI_UNF_HDMI_DeInit();
#endif
        return SSA_ERR_EXTERNAL;
    }

    HI_DRV_PDM_SetTagData();
    return HI_SUCCESS;
}

static HI_S32 ssaVerifyImageByName(HI_CHAR* pcDataPartName, HI_CHAR* pcSignPartName, SSA_VERIFYMODE_E enVerifyMode)
{
    SSA_FLASH_OPENPARAM_S stSignParam;
    SSA_FLASH_OPENPARAM_S stDataParam;
    SSA_FLASH_OPENPARAM_S*  pstSignParam = HI_NULL_PTR;

    HI_S32 s32Ret = HI_FAILURE;

    memset(&stDataParam, 0x00, sizeof(stDataParam));
    stDataParam.pcPartitionName = pcDataPartName;
    stDataParam.enFlashType     = HI_FLASH_TYPE_BUTT;

    if (HI_NULL_PTR != pcSignPartName)
    {
        memset(&stSignParam, 0x00, sizeof(stSignParam));
        stSignParam.pcPartitionName = pcSignPartName;
        stSignParam.enFlashType     = HI_FLASH_TYPE_BUTT;

        pstSignParam = &stSignParam;
    }

    s32Ret = SSA_IMG_Verify(&stDataParam,
                            pstSignParam,
                            SSA_MEM_ADDR_VERIFY1,
                            SSA_MEM_SIZE_VERIFY_DATA,
                            SSA_MEM_ADDR_VERIFY1_SIGN,
                            SSA_MEM_SIZE_VERIFY_SIGN,
                            enVerifyMode);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_IMG_Verify, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 ssaLoadLoaderImage(HI_VOID)
{
    HI_U32 bValidM = HI_TRUE;
    HI_U32 bValidB = HI_TRUE;

    SSA_FLASH_OPENPARAM_S stDataParamM;
    SSA_FLASH_OPENPARAM_S stDataParamB;

    HI_U32 u32DataBufferSize = SSA_MEM_SIZE_BOOTM;
    HI_U32 u32SignBufferSize = SSA_MEM_SIZE_VERIFY;

    HI_U8* pu8DataBufferM = SSA_MEM_ADDR_BOOTM;
    HI_U8* pu8DataBufferB = SSA_MEM_ADDR_VERIFY1;

    HI_U8* pu8SignBufferM = SSA_MEM_ADDR_BOOTM_SIGN;
    HI_U8* pu8SignBufferB = SSA_MEM_ADDR_VERIFY1_SIGN;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    memset(&stDataParamM, 0x00, sizeof(stDataParamM));
    stDataParamM.pcPartitionName = SSA_PARTNAME_LOADER;
    stDataParamM.enFlashType     = HI_FLASH_TYPE_BUTT;

    s32Ret = SSA_IMG_Read(&stDataParamM, HI_NULL_PTR, pu8DataBufferM, u32DataBufferSize, pu8SignBufferM, u32SignBufferSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_IMG_Read, s32Ret);
        bValidM = HI_FALSE;
    }

    memset(&stDataParamB, 0x00, sizeof(stDataParamB));
    stDataParamB.pcPartitionName = SSA_PARTNAME_LOADERBK;
    stDataParamB.enFlashType     = HI_FLASH_TYPE_BUTT;

    s32Ret = SSA_IMG_Read(&stDataParamB, HI_NULL_PTR, pu8DataBufferB, u32DataBufferSize, pu8SignBufferB, u32SignBufferSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_IMG_Read, s32Ret);
        bValidB = HI_FALSE;
    }

    if ((HI_TRUE != bValidM)
        && (HI_TRUE != bValidB))
    {
        SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
        return SSA_ERR_ILLEGAL_IMAGE;
    }

    if ((HI_TRUE != bValidM)
        && (HI_TRUE == bValidB))
    {
        SSA_CHECK(SSA_IMG_Clone(&stDataParamM, &stDataParamB, pu8DataBufferM, u32DataBufferSize));
        SSA_CHECK(ssaBootM(pu8DataBufferB));

        SSA_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if ((HI_TRUE == bValidM)
        && (HI_TRUE != bValidB))
    {
        SSA_CHECK(SSA_IMG_Clone(&stDataParamB, &stDataParamM, pu8DataBufferM, u32DataBufferSize));
        SSA_CHECK(ssaBootM(pu8DataBufferM));

        SSA_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if (HI_TRUE != SSA_IMG_Compare(pu8SignBufferM, pu8SignBufferB))
    {
        SSA_CHECK(SSA_IMG_Clone(&stDataParamB, &stDataParamM, pu8DataBufferM, u32DataBufferSize));
    }

    SSA_CHECK(ssaBootM(pu8DataBufferM));

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 ssaLoadKernelImage(HI_VOID)
{
    SSA_FLASH_OPENPARAM_S stDataParam;
    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    s32Ret = ssaVerifyImageByName(SSA_PARTNAME_ROOTFS, HI_NULL_PTR, SSA_VERIFYMODE_SAMPLING);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(ssaVerifyImageByName, s32Ret);
        return s32Ret;
    }

    memset(&stDataParam, 0x00, sizeof(stDataParam));
    stDataParam.pcPartitionName = SSA_PARTNAME_KERNEL;
    stDataParam.enFlashType     = HI_FLASH_TYPE_BUTT;
    s32Ret = SSA_IMG_Read(&stDataParam,
                          HI_NULL_PTR,
                          SSA_MEM_ADDR_BOOTM,
                          SSA_MEM_SIZE_BOOTM_DATA,
                          SSA_MEM_ADDR_BOOTM_SIGN,
                          SSA_MEM_SIZE_BOOTM_SIGN);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_IMG_Read, s32Ret);
        return s32Ret;
    }

    s32Ret = ssaBootM(SSA_MEM_ADDR_BOOTM);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(ssaBootM, s32Ret);
        return s32Ret;
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HI_SSA_Init(HI_VOID)
{
    SSA_DBG_FuncEnter();

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HI_SSA_DeInit(HI_VOID)
{
    SSA_DBG_FuncEnter();

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HI_SSA_Entry(HI_VOID)
{
    HI_BOOL bNeedUpgrade = HI_FALSE;
    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    SSA_CHECK(ssaShowLogo());

    s32Ret = ssaUpgradeCheck(&bNeedUpgrade);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(ssaUpgradeCheck, s32Ret);
        bNeedUpgrade = HI_FALSE;
    }

    if (HI_TRUE == bNeedUpgrade)
    {
        s32Ret = ssaLoadLoaderImage();
        if (HI_SUCCESS != s32Ret)
        {
            s32Ret = ssaLoadKernelImage();
            if (HI_SUCCESS != s32Ret)
            {
                SSA_PrintFuncErr(ssaLoadKernelImage, s32Ret);
                return s32Ret;
            }
        }
    }
    else
    {
        s32Ret = ssaLoadKernelImage();
        if (HI_SUCCESS != s32Ret)
        {
            s32Ret = ssaLoadLoaderImage();
            if (HI_SUCCESS != s32Ret)
            {
                SSA_PrintFuncErr(ssaLoadLoaderImage, s32Ret);
                return s32Ret;
            }
        }
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef HI_ADVCA_VERIFY_ENABLE

static HI_FLASH_TYPE_E ssaGetFlashType(HI_VOID)
{
    HI_S32 enBootMediaType = BOOT_MEDIA_UNKNOWN;

    enBootMediaType = get_bootmedia(HI_NULL_PTR, HI_NULL_PTR);

    switch (enBootMediaType)
    {
        case BOOT_MEDIA_NAND:
        {
            return HI_FLASH_TYPE_NAND_0;
        }
        case BOOT_MEDIA_SPIFLASH:
        {
            return HI_FLASH_TYPE_SPI_0;
        }
        case BOOT_MEDIA_EMMC:
        {
            return HI_FLASH_TYPE_EMMC_0;
        }
        case BOOT_MEDIA_SD:
        {
            return HI_FLASH_TYPE_SD;
        }
        default:
        {
            return HI_FLASH_TYPE_BUTT;
        }
    }
}

HI_S32 load_direct_env(HI_VOID* pBuffer, HI_U32 u32Address, HI_U32 u32Length)
{
    SSA_FLASH_OPENPARAM_S stDataParam;
    HI_FLASH_TYPE_E enFlashType = HI_FLASH_TYPE_BUTT;
    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    enFlashType = ssaGetFlashType();
    if (HI_FLASH_TYPE_BUTT == enFlashType)
    {
        SSA_PrintFuncErr(ssaGetFlashType, SSA_ERR_FLASH);
        return SSA_ERR_FLASH;
    }

    memset(&stDataParam, 0x00, sizeof(stDataParam));
    stDataParam.pcPartitionName = HI_NULL_PTR;
    stDataParam.enFlashType     = enFlashType;
    stDataParam.u64Address      = 0ULL;
    stDataParam.u32Length       = SSA_PARTSIZE_BOOT;

    s32Ret = SSA_ConfigExternKey(&stDataParam, SSA_MEM_ADDR_VERIFY1, SSA_MEM_SIZE_VERIFY);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_ConfigExternKey, s32Ret);
        return s32Ret;
    }

    stDataParam.u64Address  = u32Address;
    stDataParam.u32Length   = SSA_PARTSIZE_BOOTARGS;
    s32Ret = SSA_IMG_Read(&stDataParam, HI_NULL_PTR, pBuffer, u32Length, SSA_MEM_ADDR_VERIFY1_SIGN, SSA_MEM_SIZE_VERIFY_SIGN);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_IMG_Read, s32Ret);
        return s32Ret;
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}
#else
int load_direct_env(void* env, unsigned int offset, unsigned int size)
{
    SSA_DBG_FuncEnter();

    SSA_DBG_FuncExit();
    return HI_FAILURE;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

