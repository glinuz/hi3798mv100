/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : vmx_ssa.c
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

#include "hi_ssa.h"
#include "vmx_debug.h"
#include "vmx_config.h"
#include "vmx_image.h"
#include "loader_dbinfo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

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

static HI_S32 ssaBootargsAppend(HI_U32 u32RootfsPhyAddr, HI_U32 u32RootfsSize)
{
    HI_CHAR* pcBootargs = HI_NULL_PTR;
    HI_CHAR acBootarg[SSA_LEN_COMMAND] = {0};

    SSA_DBG_FuncEnter();

    pcBootargs = getenv("bootargs");
    if (HI_NULL_PTR == pcBootargs)
    {
        return HI_FAILURE;
    }

#if defined(HI_ANDROID_BOOT_SUPPORT)
    snprintf(acBootarg, sizeof(acBootarg), "%s initmrd=1,0x%x,0x%x", pcBootargs, u32RootfsPhyAddr, u32RootfsSize);
#else
    snprintf(acBootarg, sizeof(acBootarg), "%s initrd=0x%x,0x%x root=/dev/ram ramdisk_size=40960 rootfstype=squashfs", pcBootargs, u32RootfsPhyAddr, u32RootfsSize);
#endif

    setenv("bootargs", acBootarg);

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 ssaImageRestruct(HI_CHAR* pcPartitionName)
{
    HI_U32 u32RetSize = 0;
    HI_U32 u32DataSize = 0;
    HI_U32 u32PartSize = 0;
    HI_U8* pu8PartBuff = HI_NULL_PTR;
    HI_U8* pu8DataAddr = HI_NULL_PTR;
    LDDB_HANDLE hFlashHandle = HI_NULL_PTR;

    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = LDDB_FlashOpen(pcPartitionName, &hFlashHandle, &u32PartSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(LDDB_FlashOpen, s32Ret);
        return s32Ret;
    }

    s32Ret = LDDB_FlashRead(hFlashHandle, SSA_VERIFY_ADDR_SHARE, u32PartSize, &u32RetSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(LDDB_FlashRead, s32Ret);
        SSA_CHECK(LDDB_FlashClose(hFlashHandle));
        return s32Ret;
    }

    s32Ret = LDDB_FlashDataVerify(LDDB_HARDEN_TYPE_CIPHER, SSA_VERIFY_ADDR_SHARE, u32PartSize, &pu8DataAddr, &u32DataSize);
    if (HI_SUCCESS == s32Ret)
    {
        SSA_CHECK(LDDB_FlashClose(hFlashHandle));
        SSA_DBG_FuncExit();
        return HI_SUCCESS;
    }

    s32Ret = LDDB_FlashDataVerify(LDDB_HARDEN_TYPE_COMMON, SSA_VERIFY_ADDR_SHARE, u32PartSize, &pu8DataAddr, &u32DataSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(LDDB_FlashDataVerify, s32Ret);
        SSA_CHECK(LDDB_FlashClose(hFlashHandle));
        return s32Ret;
    }

    if (SSA_VERIFY_SIZE_RESERVE < u32PartSize)
    {
        SSA_PrintErrCode(SSA_ERR_OVERFLOW);
        return SSA_ERR_OVERFLOW;
    }

    s32Ret = LDDB_FlashDataSign(LDDB_HARDEN_TYPE_CIPHER, pu8DataAddr, u32DataSize, SSA_VERIFY_ADDR_RESERVE, u32PartSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(LDDB_FlashDataSign, s32Ret);
        SSA_CHECK(LDDB_FlashClose(hFlashHandle));
        return s32Ret;
    }

    s32Ret = LDDB_FlashWrite(hFlashHandle, SSA_VERIFY_ADDR_RESERVE, u32PartSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(LDDB_FlashWrite, s32Ret);

        free(pu8PartBuff);
        SSA_CHECK(LDDB_FlashClose(hFlashHandle));

        return s32Ret;
    }

    free(pu8PartBuff);
    SSA_CHECK(LDDB_FlashClose(hFlashHandle));

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 ssaLoadLoaderImage(HI_VOID)
{
    HI_U32 u32OffsetM = 0;
    HI_U32 u32OffsetB = 0;
    HI_U32 u32DataSizeM = 0;
    HI_U32 u32DataSizeB = 0;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    s32Ret = VMX_GetImageDataByName(SSA_PARTNAME_LOADER,
                                    SSA_VERIFY_ADDR_LOADER,
                                    SSA_VERIFY_SIZE_LOADER,
                                    &u32OffsetM,
                                    &u32DataSizeM);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(VMX_GetImageDataByName, s32Ret);
        u32DataSizeM = 0;
    }

    s32Ret = VMX_GetImageDataByName(SSA_PARTNAME_LOADERBK,
                                    SSA_VERIFY_ADDR_LOADERBK,
                                    SSA_VERIFY_SIZE_LOADER,
                                    &u32OffsetB,
                                    &u32DataSizeB);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(VMX_GetImageDataByName, s32Ret);
        u32DataSizeB = 0;
    }

    if ((0 == u32DataSizeM)
        && (0 == u32DataSizeB))
    {
        SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
        return SSA_ERR_ILLEGAL_IMAGE;
    }

    if ((0 == u32DataSizeM)
        && (0 < u32DataSizeB))
    {
        SSA_CHECK(VMX_CpyImageData(SSA_PARTNAME_LOADER, SSA_PARTNAME_LOADERBK, SSA_VERIFY_ADDR_LOADER, SSA_VERIFY_SIZE_LOADER));
        SSA_CHECK(ssaBootM(SSA_VERIFY_ADDR_LOADERBK + u32OffsetB));

        SSA_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if ((0 < u32DataSizeM)
        && (0 == u32DataSizeB))
    {
        SSA_CHECK(VMX_CpyImageData(SSA_PARTNAME_LOADERBK, SSA_PARTNAME_LOADER, SSA_VERIFY_ADDR_LOADERBK, SSA_VERIFY_SIZE_LOADER));
        SSA_CHECK(ssaBootM(SSA_VERIFY_ADDR_LOADER + u32OffsetM));

        SSA_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if ((u32OffsetB != u32OffsetM)
        || (u32DataSizeB != u32DataSizeM)
        || (HI_TRUE != VMX_CmpImageData(SSA_VERIFY_ADDR_LOADER, SSA_VERIFY_ADDR_LOADERBK, u32OffsetM, u32DataSizeM)))
    {
        SSA_CHECK(VMX_CpyImageData(SSA_PARTNAME_LOADERBK, SSA_PARTNAME_LOADER, SSA_VERIFY_ADDR_LOADERBK, SSA_VERIFY_SIZE_LOADER));
    }

    SSA_CHECK(ssaBootM(SSA_VERIFY_ADDR_LOADER + u32OffsetM));

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 ssaLoadKernelImage(HI_VOID)
{
    HI_U32 u32KernelOffset = 0;
    HI_U32 u32RootfsOffset = 0;
    HI_U32 u32KernelDataSize = 0;
    HI_U32 u32RootfsDataSize = 0;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    s32Ret = VMX_GetImageDataByName(SSA_PARTNAME_KERNEL,
                                    SSA_VERIFY_ADDR_KERNEL,
                                    SSA_VERIFY_SIZE_KERNEL,
                                    &u32KernelOffset,
                                    &u32KernelDataSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(VMX_GetImageDataByName, s32Ret);
        return s32Ret;
    }

    s32Ret = VMX_GetImageDataByName(SSA_PARTNAME_ROOTFS,
                                    SSA_VERIFY_ADDR_ROOTFS,
                                    SSA_VERIFY_SIZE_ROOTFS,
                                    &u32RootfsOffset,
                                    &u32RootfsDataSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(VMX_GetImageDataByName, s32Ret);
        return s32Ret;
    }

    s32Ret = ssaBootargsAppend((HI_U32)(SSA_VERIFY_ADDR_ROOTFS + u32RootfsOffset), u32RootfsDataSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(ssaBootargsAppend, s32Ret);
        return s32Ret;
    }

    s32Ret = ssaBootM(SSA_VERIFY_ADDR_KERNEL + u32KernelOffset);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(ssaBootM, s32Ret);
        return s32Ret;
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HI_SSA_Entry(HI_VOID)
{
    HI_BOOL bNeedUpgrade = HI_FALSE;
    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    SSA_CHECK(ssaImageRestruct(SSA_PARTNAME_SWINFO));
    SSA_CHECK(ssaUpgradeCheck(&bNeedUpgrade));

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
int load_direct_env(void* env, unsigned int offset, unsigned int size)
{
    HI_U32 u32Offset = 0;
    HI_U32 u32DataSize = 0;
    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    s32Ret = VMX_GetImageDataByAddr(offset, SSA_PARTSIZE_BOOTARGS, SSA_VERIFY_ADDR_SHARE, SSA_VERIFY_SIZE_SHARE, &u32Offset, &u32DataSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(VMX_GetImageDataByAddr, s32Ret);
        return s32Ret;
    }

    if (u32DataSize > size)
    {
        SSA_PrintErrCode(SSA_ERR_OVERFLOW);
        return SSA_ERR_OVERFLOW;
    }

    memset(env, 0x00, size);
    memcpy(env, (HI_U8*)(SSA_VERIFY_ADDR_SHARE + u32Offset), u32DataSize);

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

