/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : schedule.c
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2015-07-29  l00163273  N/A               Create this file.
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
#include "run.h"
#include "boot.h"
#include "app.h"
#else
#include "common.h"
#include "exports.h"
#endif

#ifdef HI_BUILD_WITH_KEYLED
#include "hi_unf_keyled.h"
#endif

#include "loader_debug.h"
#include "loader_verify.h"
#include "schedule.h"
#include "loader_dbinfo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LOADER_PARTNAME_LOADER          "loader"
#define LOADER_PARTNAME_LOADERBK        "loaderbak"
#define LOADER_PARTSIZE_LOADER          (0x02000000)

#ifdef CONFIG_USB_HOST_BOOTSTRAP
#define LOADER_APPLOADER_IMAGENAME      "apploader.bin"

extern void selfboot_init(void);
extern unsigned int get_selfboot_type(void);
extern int download_bootargs(void);
extern int download_image(const char* imagename);
#endif

#define LOADER_LEN_COMMAND                  (1024)

#ifdef HI_BUILD_WITH_KEYLED

#define LOADER_KEY_TRY_TIMES                (50)
#define LOADER_KEY_TRY_INTERVAL             (10000) /** us */

#if defined(HI_BOOT_KEYLED_GPIOKEY) && defined(CHIP_TYPE_hi3716mv420)
#define LOADER_KEY_VALUE_MENU               (0x40)
#define LOADER_KEY_VALUE_OK                 (0x2)
#elif defined(HI_BOOT_KEYLED_GPIOKEY) && defined(CHIP_TYPE_hi3798cv200)
#define LOADER_KEY_VALUE_OK                 (0x1)
#else
#define LOADER_KEY_VALUE_MENU               (0x0)
#define LOADER_KEY_VALUE_OK                 (0x1)
#endif

static HI_U32 s_auMandatoryUpgradeKey[] =
{
#if defined(HI_BOOT_KEYLED_GPIOKEY) && defined(CHIP_TYPE_hi3798cv200)
    LOADER_KEY_VALUE_OK,
#else
    LOADER_KEY_VALUE_MENU,
    LOADER_KEY_VALUE_OK,
#endif
};

static HI_S32 loaderMandatoryUpgradeCheck(HI_VOID)
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
        for (i = 0; i < LOADER_KEY_TRY_TIMES; i++)
        {
            s32Ret = HI_UNF_KEY_GetValue(&u32PressStatus, &u32KeyValue);
            if (HI_SUCCESS != s32Ret)
            {
                udelay(LOADER_KEY_TRY_INTERVAL);
                continue;
            }

            if (0x2 != u32PressStatus)
            {
                udelay(LOADER_KEY_TRY_INTERVAL);
                continue;
            }

            if (s_auMandatoryUpgradeKey[s32KeyIndex] == u32KeyValue)
            {
                break;
            }
        }

        if (LOADER_KEY_TRY_TIMES == i)
        {
            return HI_FAILURE;
        }

        udelay(LOADER_KEY_TRY_INTERVAL);
    }

    return HI_SUCCESS;
}

#endif

#ifdef HI_APPLOADER_SUPPORT

HI_S32 loaderBootM(HI_U8* pu8MemBuffer)
{
#ifdef HI_MINIBOOT_SUPPORT
    kern_load((HI_CHAR*)(pu8MemBuffer));
#else
    HI_CHAR as8BootCmd[LOADER_LEN_COMMAND] = {0};

    snprintf(as8BootCmd, sizeof(as8BootCmd), "bootm 0x%x", (HI_U32)pu8MemBuffer);
    run_command(as8BootCmd, 0);
#endif

    return HI_SUCCESS;
}

static HI_S32 loaderUpgradeCheck(HI_BOOL* pbNeedUpgrade)
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
            LOADER_PrintFuncErr(download_bootargs, s32Ret);
            return s32Ret;
        }

        memset(&stInfoParam, 0x00, sizeof(stInfoParam));
        s32Ret = LDDB_GetLoaderInfo(&stInfoParam);
        if (HI_SUCCESS != s32Ret)
        {
            LOADER_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
            memset(&stInfoParam, 0x00, sizeof(stInfoParam));
        }

        stInfoParam.stTAGParamData.bNeedUpgrade = HI_TRUE;
        stInfoParam.stTAGParamData.bUsbBurn = HI_TRUE;
        s32Ret = LDDB_SetLoaderInfo(&stInfoParam);
        if (HI_SUCCESS != s32Ret)
        {
            LOADER_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
            return s32Ret;
        }

#ifdef HI_APPLOADER_SUPPORT
        s32Ret = download_image(LOADER_APPLOADER_IMAGENAME);
        if (HI_SUCCESS != s32Ret)
        {
            LOADER_PrintFuncErr(download_image, s32Ret);
            return s32Ret;
        }
#endif
        *pbNeedUpgrade = HI_TRUE;
        return HI_SUCCESS;
    }
#endif

#ifdef HI_BUILD_WITH_KEYLED
    s32Ret = loaderMandatoryUpgradeCheck();
    if (HI_SUCCESS == s32Ret)
    {
        memset(&stInfoParam, 0x00, sizeof(stInfoParam));
        s32Ret = LDDB_GetLoaderInfo(&stInfoParam);
        if (HI_SUCCESS != s32Ret)
        {
            LOADER_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
            memset(&stInfoParam, 0x00, sizeof(stInfoParam));
        }

        stInfoParam.stTAGParamData.bNeedUpgrade = HI_TRUE;
        stInfoParam.stTAGParamData.bForceUpgrade = HI_TRUE;
        s32Ret = LDDB_SetLoaderInfo(&stInfoParam);
        if (HI_SUCCESS != s32Ret)
        {
            LOADER_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
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
        LOADER_PrintFuncErr(LDDB_GetLoaderInfo, s32Ret);
        return s32Ret;
    }

    *pbNeedUpgrade = stInfoParam.stTAGParamData.bNeedUpgrade;

    return HI_SUCCESS;
}

static HI_S32 loaderLoadLoaderImage(HI_VOID)
{
    HI_U32 u32OffsetM = 0;
    HI_U32 u32OffsetB = 0;
    HI_U32 u32DataSizeM = 0;
    HI_U32 u32DataSizeB = 0;
    HI_U32 u32MemAddrM = 0;
    HI_U32 u32MemAddrB = 0;
    HI_U8* pu8BufferM = HI_NULL_PTR;
    HI_U8* pu8BufferB = HI_NULL_PTR;

    HI_S32 s32Ret = HI_FAILURE;

    LOADER_DBG_FuncEnter();

    s32Ret = HI_MEM_Alloc(&u32MemAddrM, LOADER_PARTSIZE_LOADER);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(HI_MEM_Alloc, LOADER_ERR_MEM);
        return LOADER_ERR_MEM;
    }

    s32Ret = HI_MEM_Alloc(&u32MemAddrB, LOADER_PARTSIZE_LOADER);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(HI_MEM_Alloc, LOADER_ERR_MEM);
        return LOADER_ERR_MEM;
    }

    pu8BufferM = (HI_U8*)u32MemAddrM;
    pu8BufferB = (HI_U8*)u32MemAddrB;

    s32Ret = LOADER_GetImageDataByName(LOADER_PARTNAME_LOADER, pu8BufferM, LOADER_PARTSIZE_LOADER, &u32OffsetM, &u32DataSizeM);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(LOADER_GetImageDataByName, s32Ret);
        u32DataSizeM = 0;
    }

    s32Ret = LOADER_GetImageDataByName(LOADER_PARTNAME_LOADERBK, pu8BufferB, LOADER_PARTSIZE_LOADER, &u32OffsetB, &u32DataSizeB);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(LOADER_GetImageDataByName, s32Ret);
        u32DataSizeB = 0;
    }

    if ((0 == u32DataSizeM)
        && (0 == u32DataSizeB))
    {
        LOADER_PrintErrCode(LOADER_ERR_ILLEGAL_IMAGE);
        return LOADER_ERR_ILLEGAL_IMAGE;
    }

    if ((0 == u32DataSizeM)
        && (0 < u32DataSizeB))
    {
        LOADER_CHECK(LOADER_CpyImageData(LOADER_PARTNAME_LOADER, LOADER_PARTNAME_LOADERBK, pu8BufferM, LOADER_PARTSIZE_LOADER));
        LOADER_CHECK(loaderBootM(pu8BufferB));

        LOADER_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if ((0 < u32DataSizeM)
        && (0 == u32DataSizeB))
    {
        LOADER_CHECK(LOADER_CpyImageData(LOADER_PARTNAME_LOADERBK, LOADER_PARTNAME_LOADER, pu8BufferB, LOADER_PARTSIZE_LOADER));
        LOADER_CHECK(loaderBootM(pu8BufferM));

        LOADER_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if ((u32OffsetB != u32OffsetM)
        || (u32DataSizeB != u32DataSizeM)
        || (HI_TRUE != LOADER_CmpImageData(pu8BufferM, pu8BufferB, u32OffsetM, u32DataSizeM)))
    {
        LOADER_CHECK(LOADER_CpyImageData(LOADER_PARTNAME_LOADERBK, LOADER_PARTNAME_LOADER, pu8BufferB, LOADER_PARTSIZE_LOADER));
    }

    LOADER_CHECK(loaderBootM(pu8BufferM));

    LOADER_DBG_FuncExit();
    return HI_SUCCESS;
}
#endif

HI_S32 Loader_Main(HI_VOID)
{
    HI_BOOL bNeedUpgrade = HI_FALSE;
    HI_S32 s32Ret = HI_FAILURE;

    LOADER_DBG_FuncEnter();

    s32Ret = loaderUpgradeCheck(&bNeedUpgrade);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(loaderUpgradeCheck, s32Ret);
        return s32Ret;
    }

    if (HI_TRUE == bNeedUpgrade)
    {
#ifdef HI_APPLOADER_SUPPORT
        s32Ret = loaderLoadLoaderImage();
        if (HI_SUCCESS != s32Ret)
        {
            LOADER_PrintFuncErr(loaderLoadLoaderImage, s32Ret);
            return s32Ret;
        }
#else
        s32Ret = LOADER_App();
        if (HI_SUCCESS != s32Ret)
        {
            LOADER_PrintFuncErr(LOADER_App, s32Ret);
            return s32Ret;
        }
#endif
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

