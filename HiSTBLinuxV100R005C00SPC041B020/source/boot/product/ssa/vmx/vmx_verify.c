/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : vmx_verify.c
Version       : Initial
Author        : Hisilicon hisecurity team
Created       : 2015-12-24
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "hi_type.h"

#ifdef HI_MINIBOOT_SUPPORT
#include "run.h"
#include "boot.h"
#include "app.h"
#ifdef reserve_mem_alloc
#undef resmemalign
#undef reserve_mem_alloc
#endif
#endif

#ifdef HI_UBOOT_SUPPORT
#include "exports.h"
#endif
#ifdef HI_MINIBOOT_SUPPORT
#include "crc32.h"
#endif
#include "vmx_debug.h"
#include "hi_drv_pdm.h"
#include "loader_dbinfo.h"
#include "vmx_flash.h"
#include "vmx_verify_def.h"
#include "vmx_verify_api.h"
#include "vmx_3rd_auth.h"

#define LOADER_INFOBASIC    "loaderdb"

#define VMX_SIG_OFFSET  (16)
#define VMX_SIG_LENGTH  (256)
#define VMX_IMG_OFFSET  (VMX_SIG_OFFSET + VMX_SIG_LENGTH)
#define VMX_HEAD_LENGTH (VMX_IMG_OFFSET)
#define VMX_VERIFY_BUFFER_ALLIGN      (0x100000)


//The address below is used for image verify
#if defined(HI_ANDROID_BOOT_SUPPORT)
#define VMX_VERIFY_TEMP_ADDR    (0x20000000)
#define ANDROID_SYSTEM_VERIFY_ADDR    (0x4800000)
#else
#define VMX_VERIFY_TEMP_ADDR    (0x8000000)
#endif


#if defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3719cv100)
#define KERNEL_VERIFY_ADDR_VMX  (0x4100000)
#define UPGRADE_VERIFY_ADDR_VMX  (0x4100000)
#else
#define KERNEL_VERIFY_ADDR_VMX  (0x1FFFFC0 - VMX_HEAD_LENGTH)
#define UPGRADE_VERIFY_ADDR_VMX  (0x2100000)
#endif

/******************extern variable or function statement**********************************/
#ifdef HI_ANDROID_BOOT_SUPPORT
extern const char *boot_select(void);
#endif




extern HI_S32 Logo_Main(HI_BOOL bNeedFastplay,HI_PDM_MEMINFO_S *pstMemInfo);

/************************Local function statement***************************/
/*To be done*/

/*****************************************************************************/
HI_VOID CA_Reset(HI_VOID)
{
#ifndef HI_MINIBOOT_SUPPORT
    HI_INFO_CA(" \n");
    do_reset (NULL, 0, 0, NULL);
    do_reset (NULL, 0, 0, NULL);
    do_reset (NULL, 0, 0, NULL);
    while(1) { };
#else
    reboot();
#endif
}


HI_S32 VMX_PrintBuffer(const HI_CHAR *string, HI_U8 *pu8Input, HI_U32 u32Length)
{
    HI_U32 i = 0;

    if ( NULL != string )
    {
        HI_PRINT("%s\n", string);
    }

    for ( i = 0 ; i < u32Length; i++ )
    {
        if( (i % 16 == 0) && (i != 0))
        {
            HI_PRINT("\n");
        }

        HI_PRINT("%02x ", pu8Input[i]);
    }
    HI_PRINT("\n");

    return HI_SUCCESS;
}



HI_S32 hi_bootargs_append(HI_CHAR* appendStr)
{
    HI_CHAR *pTmp = HI_NULL;
    HI_CHAR Bootarg[1024] = {0};

    pTmp = getenv("bootargs");
    if(pTmp)
    {
        snprintf(Bootarg, 1024, "%s %s" ,pTmp, appendStr);
        setenv("bootargs", Bootarg);
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

#ifdef HI_ADVCA_VMX_3RD_SIGN
static int g_vmx_reboot_flag = 0;
#endif
static HI_S32 VMX_Auth_Partition_Image(HI_CHAR *partition_name, HI_U8 *ptr, HI_U8 *ptr_tmp)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 image_len = 0;
    HI_U8 errorCode = 0;
    HI_U8 *pu8Tmp = NULL;
#ifdef HI_ADVCA_VMX_3RD_SIGN
    VMX_APPLICATION_HEADER_S stAppHeader;
#endif

    if(NULL == partition_name || NULL == ptr || NULL == ptr_tmp)
    {
        HI_ERR_CA("failed, NULL pointer!\n");
        return HI_FAILURE;
    }

    HI_INFO_CA("VMX Verify no-boot-image name:%s\n", partition_name);

#ifdef HI_ADVCA_VMX_3RD_SIGN
    memset(&stAppHeader, 0, sizeof(VMX_APPLICATION_HEADER_S));
    stAppHeader.u32ImageLen = *((HI_U32 *)ptr);
    stAppHeader.enc_ctrl.enc_flag = ptr[4]&0x1;

    (void)VMX3RD_SetEncFlag(stAppHeader.enc_ctrl.enc_flag);
#endif

    image_len = *((HI_U32 *)ptr);
    VMX_PrintBuffer("image:", ptr, 32);
    HI_INFO_CA("%s image_len: 0x%08x\n", partition_name, image_len);

    if(0 != (image_len & 0xF))
    {
        HI_ERR_CA("failed, Invalid image_len:0x%x!\n", image_len);
        return HI_FAILURE;
    }

    pu8Tmp = ptr_tmp;

    /*
        errorCode is a pointer to a value that tells the reason for not having a valid image
            1 means no valid signature found - reboot
            2 means store src part to flash and reboot after it
        return value
            0 indicates do not start the application
            1 indicates the application can be started

        There are three situations: ret=1; ret=0 and errorCode=1;  ret=0 and errorCode=2;
    */

    HI_INFO_CA("verify parameter ptr:0x%p, image len:%x\n", ptr, image_len);
    ret = verifySignature((ptr + VMX_SIG_OFFSET),
                        (ptr + VMX_IMG_OFFSET),
                        pu8Tmp + VMX_IMG_OFFSET,
                        image_len,
                        image_len,
                        0,
                        &errorCode);


    if(1 == ret)
    {
        HI_ERR_CA("verify success! ret:0x%x, Continue ...\n", ret);
        return HI_SUCCESS;
    }
    else if((1 == errorCode) && (0 == ret))         /* Invalid signature, maybe */
    {
        HI_ERR_CA("do not start the application, reset! errorCode: 0x%x, ret: 0x%x, Resetting ...\n", errorCode, ret);
        CA_Reset();
    }
    else if((2 == errorCode) && (0 == ret))         /* Src is re-encrypted inside BL library, burn to flash */
    {
        HI_ERR_CA("verify success! burn src to flash, errorCode:0x%x\n", errorCode);

        HI_U32 PartionSize = 0;
        HI_U32 BlockSize = 0;
        HI_U32 PageSize = 0;
        HI_U32 OobSize = 0;
        HI_U32 write_length = 0;

        ret = CA_flash_getinfo(partition_name, &PartionSize, &BlockSize, &PageSize, &OobSize);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CA("CA_flash_getinfo failed! ret:0x%x\n", ret);
            return HI_FAILURE;
        }


        write_length = ((VMX_HEAD_LENGTH + image_len) + BlockSize - 1) & (~(BlockSize -1));

        VMX_PrintBuffer("before write(32 bytes):", ptr, 32);

        HI_INFO_CA("write info: partition_name:%s, BlockSize:0x%x, PageSize:0x%x, OobSize:0x%x, write_length:0x%x\n", partition_name, BlockSize, PageSize, OobSize, write_length);
#ifdef HI_ADVCA_VMX_3RD_SIGN
        memcpy(ptr_tmp, ptr, VMX_IMG_OFFSET);
        ret = CA_flash_write(partition_name, (HI_U64)0, write_length, ptr_tmp);
#else
        ret= CA_flash_write(partition_name, (HI_U64)0, write_length, ptr);
#endif
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CA("Burn image to flash failed! ret:0x%x, Resetting ...\n", ret);
            CA_Reset();
        }
        else
        {
            HI_INFO_CA("\nWrite src to flash success, ret:0x%x! Resetting ...\n", ret);
#ifdef HI_ADVCA_VMX_3RD_SIGN
            g_vmx_reboot_flag = 1;
#else
            CA_Reset();
#endif
        }

    }
    else
    {
        //not supported;
    }

    return HI_SUCCESS;
}

#ifdef HI_ADVCA_VERIFY_ENABLE
static HI_S32 VMX_Auth_Bootargs(HI_U8 *ptr, HI_U8 *ptr_tmp, HI_U32 offset, HI_U32 size)
{
    HI_U32 image_len = 0;
    HI_U8 errorCode = 0;
    HI_S32 ret;
#ifdef HI_ADVCA_VMX_3RD_SIGN
    VMX_APPLICATION_HEADER_S stAppHeader;
#endif

#ifdef HI_ADVCA_VMX_3RD_SIGN
    memset(&stAppHeader, 0, sizeof(VMX_APPLICATION_HEADER_S));
    stAppHeader.u32ImageLen = *((HI_U32 *)ptr);
    stAppHeader.enc_ctrl.enc_flag = ptr[4]&0x1;

    HI_INFO_CA("bootargs enc flag:%d\n", stAppHeader.enc_ctrl.enc_flag);
    (void)VMX3RD_SetEncFlag(stAppHeader.enc_ctrl.enc_flag);
#endif

    image_len = *((HI_U32 *)ptr);

    if(0 != (image_len & 0xF))
    {
        HI_ERR_CA("failed, Invalid image_len:0x%x!\n", image_len);
        return HI_FAILURE;
    }
    HI_INFO_CA("verify bootargs parameter ptr:0x%p, image len:%x\n", ptr, image_len);
    ret = verifySignature((ptr + VMX_SIG_OFFSET),
                        (ptr + VMX_IMG_OFFSET),
                        ptr_tmp + VMX_IMG_OFFSET,
                        image_len,
                        image_len,
                        0,
                        &errorCode);
    if (1 == ret)
    {
        return HI_SUCCESS;
    }
    else if ((1 == errorCode) && (0 == ret))
    {
        HI_ERR_CA("verify bootargs errorCode: 0x%x, ret: 0x%x!!!!!!!!!\n", errorCode, ret);
        return HI_FAILURE;
    }
    else if ((2 == errorCode) && (0 == ret))
    {

        HI_INFO_CA("verify success! burn src to flash, errorCode:0x%x\n", errorCode);

        HI_HANDLE hFlashHandle;
        HI_Flash_InterInfo_S stFlashInfo;
        HI_U32 write_length = 0;

        hFlashHandle = CA_flash_open_addr((HI_U64)offset, (HI_U64)(size + VMX_HEAD_LENGTH));
        HI_INFO_CA("CA_flash_open_addr: Offset:0x%x, size:0x%x, hFlashHandle:%x\n", offset, (image_len + VMX_HEAD_LENGTH), hFlashHandle);
        if ((0 == hFlashHandle) || (HI_INVALID_HANDLE == hFlashHandle))
        {
            HI_ERR_CA("HI_Flash_Open error\n");
            return HI_FAILURE;
        }

        ret = HI_Flash_GetInfo(hFlashHandle, &stFlashInfo);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("HI_Flash_GetInfo error\n");
            return HI_FAILURE;
        }

        if(stFlashInfo.FlashType == HI_FLASH_TYPE_EMMC_0)
        {
            write_length =  ((VMX_HEAD_LENGTH + image_len) + stFlashInfo.BlockSize - 1) & (~(stFlashInfo.BlockSize - 1));
        }
        else
        {
            write_length =  ((VMX_HEAD_LENGTH + image_len) + stFlashInfo.PageSize - 1) & (~(stFlashInfo.PageSize - 1));
        }

        HI_INFO_CA("Try to Write back the bootargs into Flash\n");
        HI_INFO_CA("write info: BlockSize:0x%x, PageSize:0x%x, image_len:0x%x, write_length:0x%x, g_EnvFlashAddr:0x%x\n",
                    stFlashInfo.BlockSize, stFlashInfo.PageSize, image_len, write_length, offset);

#ifdef HI_ADVCA_VMX_3RD_SIGN
        memcpy(ptr_tmp, ptr, VMX_IMG_OFFSET);
        ret = CA_flash_write_addr(offset, (HI_U64)write_length, ptr_tmp);
#else
        ret = CA_flash_write_addr(offset, (HI_U64)write_length, ptr);
#endif
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Burn image to flash failed! ret:0x%x, Resetting ...\n", ret);
            CA_Reset();
            return HI_FAILURE;
        }
        else
        {
            HI_INFO_CA("\nWrite src to flash success, ret:0x%x! Resetting ...\n", ret);
            ret = HI_Flash_Close(hFlashHandle);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_CA("HI_Flash_Close error \n");
                return HI_FAILURE;
            }

#ifdef HI_ADVCA_VMX_3RD_SIGN
            HI_INFO_CA("set reboot flag to 1, return success\n");
            g_vmx_reboot_flag = 1;
            return HI_SUCCESS;
#else
            CA_Reset();
#endif
        }
    }
    else
    {
        HI_ERR_CA("VMX Not support return!!!!!!\n");
    }

    return HI_FAILURE;
}

#ifdef CONFIG_ENV_BACKUP
    static HI_U32 s_u32LoadEnvLoop = 2;
#else
    static HI_U32 s_u32LoadEnvLoop = 1;
#endif

int load_direct_env(void *env, unsigned int offset, unsigned int size)
{
    HI_S32 ret = 0;

    HI_INFO_CA("Enter VMX Advcanced load_direct_env offset:%x, size:%x\n", offset, size);
    if (s_u32LoadEnvLoop > 0)
    {
        s_u32LoadEnvLoop--;
    }
    else
    {
        return HI_FAILURE;
    }

    ret = CA_flash_read_addr(offset, size + VMX_HEAD_LENGTH, (HI_U8*)IMG_VERIFY_ADDRESS, HI_NULL);
    VMX_PrintBuffer("bootargs:", (HI_U8 *)IMG_VERIFY_ADDRESS, 512);
    if (HI_SUCCESS == ret)
    {
        ret = VMX_Auth_Bootargs((HI_U8 *)IMG_VERIFY_ADDRESS, (HI_U8 *) VMX_VERIFY_TEMP_ADDR, offset, size);
    }
    else
    {
        HI_INFO_CA("CA_flash_read_addr bootargs failed\n");
    }

    if ((HI_SUCCESS != ret) && (0 == s_u32LoadEnvLoop))
    {
        HI_ERR_CA("Authenticate bootargs failed\n");
        CA_Reset();
    }

    if (HI_SUCCESS == ret)
    {
        HI_INFO_CA("Authenticate bootargs success\n");

        memset(env, 0, size);
        memcpy(env, (HI_U8 *)(IMG_VERIFY_ADDRESS + VMX_IMG_OFFSET), size);
        VMX_PrintBuffer("after decrypt bootargs, env(start):", env, 256);
        VMX_PrintBuffer("after decrypt bootargs, env(end):", env+size-256, 256);
    }

    return ret;
}
#else
int load_direct_env(void *env, unsigned int offset, unsigned int size)
{
    return -1; /*return Failure and fastboot will continue to read env by normal way*/
}
#endif


static HI_S32 VMX_ReadUpgrdFlag(HI_BOOL *pbNeedUpgrd)
{
#if defined(HI_ANDROID_BOOT_SUPPORT)
    if (strncmp(boot_select(),"recovery",sizeof("recovery") - 1) == 0)
    {
        *pbNeedUpgrd = HI_TRUE;
    }
    else
    {
        *pbNeedUpgrd = HI_FALSE;
    }

#elif defined(HI_LOADER_SUPPORT)
    LDDB_LOADER_INFO_S stInfoParam;
    HI_S32 s32Ret = HI_SUCCESS;

	memset(&stInfoParam, 0x00, sizeof(stInfoParam));

    s32Ret = LDDB_GetLoaderInfo(&stInfoParam);
	if (HI_SUCCESS != s32Ret)
	{
		HI_ERR_CA("Failed to get upgrade parameter info.");
		return s32Ret;
	}

    *pbNeedUpgrd = stInfoParam.stTAGParamData.bNeedUpgrade;

#endif

    return HI_SUCCESS;
}

#ifdef HI_ANDROID_BOOT_SUPPORT
extern int set_default_ethaddr(void);
#endif

HI_S32 HI_SSA_Init(HI_VOID)
{
    HI_LOG_SetLevel(HI_ID_BUTT, 4);

#ifdef HI_ANDROID_BOOT_SUPPORT
    set_default_ethaddr();
#endif

    return HI_SUCCESS;
}

HI_S32 HI_SSA_Entry(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32KernelSize = 0;
    HI_U32 u32FsSize = 0;
    HI_U8 *ptr = NULL;
    HI_U8 *ptr_trace = NULL;
    HI_U8 *ptr_fs_trace = NULL;
    HI_Flash_InterInfo_S flashInfo;

    HI_U32 KernelPartionSize = 0;
    HI_U32 UpgradePartionSize = 0;
    HI_U32 BlockSize = 0;
    HI_U32 PageSize = 0;
    HI_U32 OobSize = 0;
    HI_U32 u32FSReadSize = 0;
    HI_U32 u32KernelReadSize = 0;
    HI_CHAR *kernel_name = "kernel";
#if defined(HI_ANDROID_BOOT_SUPPORT)
    HI_CHAR *upgrade_name = "recovery";
#else
    HI_CHAR *upgrade_name = "loader";
#endif

#if defined(HI_ANDROID_BOOT_SUPPORT)
    HI_CHAR *rootfs_name = "shatable";
#else
    HI_CHAR *rootfs_name = "rootfs";
#endif
    HI_CHAR acRootfsParam[128];
    HI_BOOL bNeedUpgrd = HI_FALSE;
#ifndef HI_MINIBOOT_SUPPORT
    HI_CHAR as8BootCmd[64];
#endif

    HI_U8 *pu8Tmp_system = NULL;
    HI_INFO_CA("start ---------->\n");

    Logo_Main(HI_TRUE,HI_NULL);
    HI_DRV_PDM_SetTagData();

    ret = VMX_ReadUpgrdFlag(&bNeedUpgrd);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("read upgrade flag for loader failed !\n");
        //return HI_FAILURE;
    }
    HI_INFO_CA("bNeedUpgrd = %d\n", bNeedUpgrd);

    if (HI_TRUE == bNeedUpgrd)
    {
        ptr = (HI_U8 *)UPGRADE_VERIFY_ADDR_VMX;
        ptr_trace =  (HI_U8 *)VMX_VERIFY_TEMP_ADDR;

        HI_INFO_CA("Need to upgrade, enter to verify %s image\n", upgrade_name);
        /* prepare the loader image data to ddr */
        ret = CA_flash_getinfo(upgrade_name, &UpgradePartionSize, &BlockSize, &PageSize, &OobSize);
        HI_INFO_CA("%s: PartionSize:0x%x, BlockSize:0x%x, PageSize:0x%x\n", upgrade_name, UpgradePartionSize, BlockSize, PageSize);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CA("failed, ret:%d!\n", ret);
            return HI_FAILURE;
        }

        ret = CA_flash_read(upgrade_name, (unsigned long long)0, UpgradePartionSize, ptr, &flashInfo);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CA("failed, ret:%d!\n", ret);
            return HI_FAILURE;
        }
        VMX_PrintBuffer("upgrade:", ptr, 32);
        VMX_PrintBuffer("image(first 32 bytes):", ptr + VMX_HEAD_LENGTH, 32);
        HI_INFO_CA("%s partition size is: 0x%x\n", upgrade_name, UpgradePartionSize);
        /* verify loader now */
        ret = VMX_Auth_Partition_Image(upgrade_name, ptr, ptr_trace);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CA("Authenticate %s failed\n", upgrade_name);
            CA_Reset();
        }
        HI_INFO_CA("Authenticate %s success.\n\n\n", upgrade_name);
    }
    else
    {
        ptr = (HI_U8 *)KERNEL_VERIFY_ADDR_VMX;
        ptr_trace = (HI_U8 *)VMX_VERIFY_TEMP_ADDR;

        /* get kernel partition info */
        ret = CA_flash_getinfo(kernel_name, &KernelPartionSize, &BlockSize, &PageSize, &OobSize);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CA("failed, ret:%d!\n", ret);
            return HI_FAILURE;
        }
        HI_INFO_CA("%s: PartionSize:0x%x, BlockSize:0x%x, PageSize:0x%x\n", kernel_name, KernelPartionSize, BlockSize, PageSize);
        //read kernel signed header
        if(0 != PageSize)
        {
            ret = CA_flash_read(kernel_name, (unsigned long long)0, PageSize, ptr, &flashInfo);
        }
        else
        {
            ret = CA_flash_read(kernel_name, (unsigned long long)0, BlockSize, ptr, &flashInfo);
        }

        if(HI_SUCCESS != ret)
        {
            HI_ERR_CA("failed, ret:%d!\n", ret);
            return HI_FAILURE;
        }
        VMX_PrintBuffer("kernel header:", ptr, 32);
        u32KernelSize = *(HI_U32 *)ptr;

        if(0 != PageSize)
        {
            u32KernelReadSize = PageSize * ((u32KernelSize + PageSize -1)/PageSize)+PageSize;
        }
        else
        {
            u32KernelReadSize = BlockSize * ((u32KernelSize + BlockSize -1)/BlockSize)+BlockSize;
        }
        HI_INFO_CA("%s real size is: 0x%x, read size is: 0x%x\n", kernel_name, u32KernelSize, u32KernelReadSize);

        //read kernel image
        ret = CA_flash_read(kernel_name, (unsigned long long)0, u32KernelReadSize, ptr, &flashInfo);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CA("failed, ret:%d!\n", ret);
            return HI_FAILURE;
        }
        VMX_PrintBuffer("kernel header:", ptr, 32);
        VMX_PrintBuffer("kernel image(first 32 bytes):", ptr + VMX_HEAD_LENGTH, 32);
        ret = VMX_Auth_Partition_Image(kernel_name, ptr, ptr_trace);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CA("Authenticate %s failed\n", kernel_name);
            CA_Reset();
        }
        HI_INFO_CA("Authenticate %s success.\n\n\n", kernel_name);

      /* prepare fs image data to ddr */
#if defined(HI_ANDROID_BOOT_SUPPORT)
        pu8Tmp_system = (HI_U8 *)(ANDROID_SYSTEM_VERIFY_ADDR);
#else
        pu8Tmp_system = (HI_U8 *) ((KERNEL_VERIFY_ADDR_VMX + u32KernelReadSize + VMX_VERIFY_BUFFER_ALLIGN - 1) & ~(VMX_VERIFY_BUFFER_ALLIGN -1));
#endif
        HI_INFO_CA("FS image read in addr: 0x%x\n", (HI_U32)pu8Tmp_system);

        if(0 != PageSize)
        {
            ret = CA_flash_read(rootfs_name, 0, PageSize, pu8Tmp_system, &flashInfo);
        }
        else
        {
            ret = CA_flash_read(rootfs_name, 0, BlockSize, pu8Tmp_system, &flashInfo);
        }
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CA("failed, ret:%d!\n", ret);
            return HI_FAILURE;
        }
        u32FsSize = *(HI_U32 *)pu8Tmp_system;
        if(0 != PageSize)
        {
            u32FSReadSize = PageSize * ((VMX_SIG_OFFSET + VMX_SIG_LENGTH + u32FsSize + PageSize -1)/PageSize);
        }
        else
        {
            u32FSReadSize = BlockSize * ((VMX_SIG_OFFSET + VMX_SIG_LENGTH + u32FsSize + BlockSize -1)/BlockSize);
        }
        HI_INFO_CA("%s partition size is: 0x%x, u32FSReadSize:0x%x\n", rootfs_name, u32FsSize, u32FSReadSize);

        ptr_fs_trace = pu8Tmp_system;


        HI_INFO_CA("%s partition size is: 0x%x, u32FSReadSize:0x%x\n", rootfs_name, u32FsSize, u32FSReadSize);
        ret = CA_flash_read(rootfs_name, (unsigned long long)0, u32FSReadSize, ptr_fs_trace, &flashInfo);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CA("failed, ret:%d!\n", ret);
            return HI_FAILURE;
        }
        VMX_PrintBuffer("rootfs:", ptr_trace, 32);
        ptr_trace = (HI_U8 *)VMX_VERIFY_TEMP_ADDR;

        ret = VMX_Auth_Partition_Image(rootfs_name, pu8Tmp_system, ptr_trace);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CA("Authenticate %s failed\n", rootfs_name);
            CA_Reset();
        }
        HI_INFO_CA("Authenticate %s success.\n\n\n", rootfs_name);

    //set system file start addr to bootargs
    memset(acRootfsParam, 0x0, sizeof (acRootfsParam));

#if defined(HI_ANDROID_BOOT_SUPPORT)
        snprintf(acRootfsParam, sizeof (acRootfsParam), "initmrd=1,0x%x,0x%x", ANDROID_SYSTEM_VERIFY_ADDR + VMX_HEAD_LENGTH, u32FSReadSize);
#else
        snprintf(acRootfsParam, sizeof (acRootfsParam), "initrd=0x%x,0x%x root=/dev/ram ramdisk_size=40960 rootfstype=squashfs", pu8Tmp_system + VMX_HEAD_LENGTH, u32FSReadSize);
#endif

        HI_INFO_CA("ram system param: %s\n", acRootfsParam);
        ret =  hi_bootargs_append(acRootfsParam);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("set ram system param error!\n");
            CA_Reset();
        }
    }

#ifdef HI_ADVCA_VMX_3RD_SIGN
    if(1 == g_vmx_reboot_flag)
    {
        CA_Reset();
    }
#endif

    /* start the kernel or loader or recovery*/
    ptr += VMX_HEAD_LENGTH;
    if(HI_TRUE == bNeedUpgrd)
    {
        HI_INFO_CA("enter %s:0x%08x\n", upgrade_name,  (HI_U32)ptr);
    }
    else
    {
        HI_INFO_CA("enter kernel:0x%08x\n", (HI_U32)ptr);
    }

#ifdef HI_MINIBOOT_SUPPORT
    HI_INFO_CA("Miniboot:Now Miniboot will jump to kernel. the kern_load:0x%x\n", ptr);
    kern_load((char *)ptr);
#else
    memset(as8BootCmd, 0, sizeof(as8BootCmd));
    snprintf(as8BootCmd, sizeof(as8BootCmd), "bootm 0x%x", ptr);
    HI_INFO_CA("Fastboot:Now Fastboot will jump to kernel. %s\n", as8BootCmd);
    run_command(as8BootCmd, 0);
#endif

    return HI_SUCCESS;
}

