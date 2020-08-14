/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : download.c
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

#ifdef HI_LOADER_BOOTLOADER
#include <uboot.h>
#else
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/types.h>
#include "hi_flash.h"
#endif

#include "hi_type.h"
#include "com_osal.h"
#include "com_debug.h"
#include "com_config.h"
#include "download_fs.h"
#include "loader_dbinfo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define DOWNLOAD_FS_DEVFILENAME             "/dev/sda"
#define DOWNLOAD_FS_PARTITION_MAXNUM        (32)
#define DOWNLOAD_FS_TRYTIMES                (20)
#define DOWNLOAD_FS_DEVICECHECK_INTERVAL    (250)
#define DOWNLOAD_FS_MOUNT_FLAGS             (MS_NOSUID | MS_NODEV | MS_NOEXEC)
#define DOWNLOAD_FS_UMOUNT2_FLAGS           (MNT_FORCE | MNT_DETACH)
#define DOWNLOAD_FS_CMDLINE                 "/proc/cmdline"         /**< Path of cmdline file  */
#define DOWNLOAD_FS_COMMAND_LEN             (1024)                  /**< Command line size according to arch/arm/include/asm/setup.h COMMAND_LINE_SIZE */

static HI_BOOL      s_bInited = HI_FALSE;
static HI_HANDLE    s_hFileHandle = 0;

#ifdef HI_LOADER_BOOTLOADER

static HI_U64   s_u64ReadOffset = 0;
static HI_CHAR  s_acFileName[HI_LOADER_FILENAME_LEN] = {0};

static HI_S32 fsFileCheckInUsb(const HI_CHAR* pcFileName)
{
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    s32Ret = usb_set_negotiation_timeout(5);
    if (0 != s32Ret)
    {
        COM_PrintFuncErr(usb_set_negotiation_timeout, s32Ret);
        return HI_FAILURE;
    }

    s32Ret = usb_detect();
    if (0 != s32Ret)
    {
        COM_PrintFuncErr(usb_detect, s32Ret);
        return HI_FAILURE;
    }

    s32Ret = usb_file_check(pcFileName);
    if (0 != s32Ret)
    {
        COM_PrintFuncErr(usb_file_check, s32Ret);
        return HI_FAILURE;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 fsFileCheckInPartiton(HI_CHAR* pcPartitionName, HI_CHAR* pcFileName, HI_U32* puFileSize)
{
    COM_PrintErrCode(COM_ERR_UNSUPPORTED);
    return COM_ERR_UNSUPPORTED;
}

static HI_S32 fsFileOpen(HI_HANDLE* phHandle, HI_CHAR* pcFilename)
{
    COM_DBG_FuncEnter();

    COM_StrNCpy(s_acFileName, pcFilename, HI_LOADER_FILENAME_LEN - 1);

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 fsFileClose(HI_HANDLE hHandle)
{
    COM_DBG_FuncEnter();

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_U64 fsFileRead(HI_HANDLE hHandle, HI_VOID* pBuffer, HI_U64 u64MaxSize)
{
    HI_U64 s64RetSize = 0;

    s64RetSize = usb_file_read(s_acFileName, pBuffer, s_u64ReadOffset, u64MaxSize);
    if (0 >= s64RetSize)
    {
        COM_PrintFuncErr(usb_file_read, COM_ERR_FILE);
        return COM_ERR_FILE;
    }

    s_u64ReadOffset += (HI_U64)s64RetSize;

    return HI_SUCCESS;
}

static HI_S32 fsFileGetSize(HI_HANDLE hHandle, HI_U64* pu64FileSize)
{
    HI_S64 s64FileSize = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    s64FileSize = usb_get_file_size(s_acFileName);
    if (0 >= s64FileSize)
    {
        COM_PrintFuncErr(usb_get_file_size, COM_ERR_FILE);
        return COM_ERR_FILE;
    }

    *pu64FileSize = (HI_U64)s64FileSize;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_VOID fsPartitionReset(HI_CHAR* pcPartitionName)
{
    COM_PrintErrCode(COM_ERR_UNSUPPORTED);
    return COM_ERR_UNSUPPORTED;
}

#else

#define FS_ISBLK(pcBLKName, stStatBuf)    ((0 == lstat(pcBLKName, &stStatBuf)) && S_ISBLK(stStatBuf.st_mode))
#define FS_ISREG(pcREGName, stStatBuf)    ((0 == lstat(pcREGName, &stStatBuf)) && S_ISREG(stStatBuf.st_mode))

static HI_S32 fsMount(HI_CHAR* pcSource, const HI_CHAR* pcTarget)
{
    COM_DBG_FuncEnter();

    if (mount(pcSource, pcTarget, "vfat", DOWNLOAD_FS_MOUNT_FLAGS, HI_NULL_PTR) >= 0)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if (mount(pcSource, pcTarget, "ext4", DOWNLOAD_FS_MOUNT_FLAGS, HI_NULL_PTR) >= 0)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if (mount(pcSource, pcTarget, "ext3", DOWNLOAD_FS_MOUNT_FLAGS, HI_NULL_PTR) >= 0)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if (mount(pcSource, pcTarget, "ext2", DOWNLOAD_FS_MOUNT_FLAGS, HI_NULL_PTR) >= 0)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if (mount(pcSource, pcTarget, "tntfs", DOWNLOAD_FS_MOUNT_FLAGS, HI_NULL_PTR) >= 0)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    COM_PrintErrCode(COM_ERR_DEVICE);
    return COM_ERR_DEVICE;
}

static HI_S32 fsUMount(const HI_CHAR* pcTarget)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    s32Ret = umount2(pcTarget, DOWNLOAD_FS_UMOUNT2_FLAGS);
    if (0 > s32Ret)
    {
        COM_PrintFuncErr(umount2, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 fsGetBootargs(HI_CHAR* pcBuffer, HI_U32 u32BufferSize)
{
    FILE* pFile = HI_NULL_PTR;
    HI_CHAR* pcRet = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    pFile = fopen(DOWNLOAD_FS_CMDLINE, "r");
    if (HI_NULL_PTR == pFile)
    {
        COM_PrintFuncErr(fopen, COM_ERR_FILE);
        return COM_ERR_FILE;
    }

    pcRet = fgets(pcBuffer, (HI_S32)u32BufferSize, pFile);
    if (HI_NULL_PTR == pcRet)
    {
        COM_PrintFuncErr(fgets, COM_ERR_FILE);
        fclose(pFile);
        return COM_ERR_FILE;
    }

    fclose(pFile);

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 fsGetDeviceIndex(const HI_CHAR* pcPartitionDes, const HI_CHAR* pcPartitionName, HI_U32* pu32Index)
{
    HI_U32   u32Index = 0;
    HI_CHAR* pcStartP = HI_NULL_PTR;
    HI_CHAR* pcPartitionPos = HI_NULL_PTR;
    HI_CHAR  acPartitionFlag[HI_LOADER_PARTITION_NAME_LEN] = {0};

    snprintf(acPartitionFlag, sizeof(acPartitionFlag), "(%s)", pcPartitionName);
    pcPartitionPos = COM_StrStr(pcPartitionDes, acPartitionFlag);
    if (HI_NULL_PTR == pcPartitionPos)
    {
        COM_PrintErrCode(COM_ERR_NOEXIST);
        return COM_ERR_NOEXIST;
    }

    pcStartP = COM_StrStr(pcPartitionDes, ",");

    while ((HI_NULL_PTR != pcStartP)
           && (0 < pcPartitionPos - pcStartP))
    {
        u32Index ++;
        pcStartP ++;
        pcStartP = COM_StrStr(pcStartP, ",");
    }

    *pu32Index = u32Index;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 fsGetDeviceName(const HI_CHAR* pcPartitionName, HI_CHAR* pcDeviceName, HI_U32 u32Length)
{
    HI_U32   u32Index = 0;
    HI_CHAR  acBootArgs[DOWNLOAD_FS_COMMAND_LEN] = {0};
    HI_CHAR* pcPartDes = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = fsGetBootargs(acBootArgs, DOWNLOAD_FS_COMMAND_LEN);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(fsGetBootargs, s32Ret);
        return COM_ERR_NOEXIST;
    }

    pcPartDes = COM_StrStr(acBootArgs, "mmcblk0:");
    if (HI_NULL_PTR != pcPartDes)
    {
        s32Ret = fsGetDeviceIndex(pcPartDes, pcPartitionName, &u32Index);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(fsGetDeviceIndex, s32Ret);
            return COM_ERR_NOEXIST;
        }

        snprintf(pcDeviceName, u32Length, "/dev/mmcblk0p%d", u32Index + 1);
        return HI_SUCCESS;
    }

    pcPartDes = COM_StrStr(acBootArgs, "hinand:");
    if (HI_NULL_PTR != pcPartDes)
    {
        s32Ret = fsGetDeviceIndex(pcPartDes, pcPartitionName, &u32Index);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(fsGetDeviceIndex, s32Ret);
            return COM_ERR_NOEXIST;
        }

        snprintf(pcDeviceName, u32Length, "/dev/mtdblock%d", u32Index);
        return HI_SUCCESS;
    }

    pcPartDes = COM_StrStr(acBootArgs, "hi_sfc:");
    if (HI_NULL_PTR != pcPartDes)
    {
        s32Ret = fsGetDeviceIndex(pcPartDes, pcPartitionName, &u32Index);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(fsGetDeviceIndex, s32Ret);
            return COM_ERR_NOEXIST;
        }

        snprintf(pcDeviceName, u32Length, "/dev/mtdblock%d", u32Index);
        return HI_SUCCESS;
    }

    COM_PrintErrCode(COM_ERR_UNSUPPORTED);
    return COM_ERR_UNSUPPORTED;
}

static HI_S32 fsFileCheckInUsb(const HI_CHAR* pcFileName)
{
    HI_S32 i = 0;
    HI_BOOL bPartitioned = HI_FALSE;
    HI_CHAR acDeviceName[HI_LOADER_FILENAME_LEN] = {0};
    struct stat stStatBuf;

    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    for (i = 0; i < DOWNLOAD_FS_TRYTIMES; i++)
    {
        COM_MemSet(&stStatBuf, 0x00, sizeof(stStatBuf));

        if (!FS_ISBLK(DOWNLOAD_FS_DEVFILENAME, stStatBuf))
        {
            COM_Delayms(DOWNLOAD_FS_DEVICECHECK_INTERVAL);
            continue;
        }

        break;
    }

    if (DOWNLOAD_FS_TRYTIMES <= i)
    {
        COM_PrintErrCode(COM_ERR_NOEXIST);
        return COM_ERR_NOEXIST;
    }

    for (i = 1; i <= DOWNLOAD_FS_PARTITION_MAXNUM; i++)
    {
        COM_MemSet(acDeviceName, 0x00, sizeof(acDeviceName));

        snprintf(acDeviceName, sizeof(acDeviceName), DOWNLOAD_FS_DEVFILENAME"%d", i);
        s32Ret = fsMount(acDeviceName, COM_CFG_D_UPGRADE_FILEPATH);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(fsMount, s32Ret);
            break;
        }

        bPartitioned = HI_TRUE;

        COM_MemSet(&stStatBuf, 0x00, sizeof(stStatBuf));
        if (!FS_ISREG(pcFileName, stStatBuf))
        {
            fsUMount(COM_CFG_D_UPGRADE_FILEPATH);
            continue;
        }

        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if (HI_TRUE == bPartitioned)
    {
        COM_PrintErrCode(COM_ERR_NOEXIST);
        return COM_ERR_NOEXIST;
    }

    s32Ret = fsMount(DOWNLOAD_FS_DEVFILENAME, COM_CFG_D_UPGRADE_FILEPATH);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(fsMount, s32Ret);
        return COM_ERR_NOEXIST;
    }

    COM_MemSet(&stStatBuf, 0x00, sizeof(stStatBuf));

    if (!FS_ISREG(pcFileName, stStatBuf))
    {
        fsUMount(COM_CFG_D_UPGRADE_FILEPATH);
        return COM_ERR_NOEXIST;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 fsFileCheckInPartiton(HI_CHAR* pcPartitionName, HI_CHAR* pcFileName)
{
    HI_S32 i = 0;
    struct stat stStatBuf;
    HI_CHAR acDeviceName[HI_LOADER_FILENAME_LEN] = {0};
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    s32Ret = fsGetDeviceName(pcPartitionName, acDeviceName, HI_LOADER_FILENAME_LEN);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(fsGetDeviceName, s32Ret);
        return s32Ret;
    }

    COM_PrintStr(pcPartitionName);
    COM_PrintStr(acDeviceName);

    for (i = 0; i < DOWNLOAD_FS_TRYTIMES; i++)
    {
        COM_MemSet(&stStatBuf, 0x00, sizeof(stStatBuf));

        if (!FS_ISBLK(acDeviceName, stStatBuf))
        {
            COM_Delayms(DOWNLOAD_FS_DEVICECHECK_INTERVAL);
            continue;
        }

        break;
    }

    if (DOWNLOAD_FS_TRYTIMES <= i)
    {
        COM_PrintErrCode(COM_ERR_NOEXIST);
        return COM_ERR_NOEXIST;
    }

    s32Ret = fsMount(acDeviceName, COM_CFG_D_UPGRADE_FILEPATH);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(fsMount, s32Ret);
        return s32Ret;
    }

    COM_MemSet(&stStatBuf, 0x00, sizeof(stStatBuf));

    if (!FS_ISREG(pcFileName, stStatBuf))
    {
        COM_PrintErrCode(COM_ERR_NOEXIST);
        return COM_ERR_NOEXIST;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 fsFileOpen(HI_HANDLE* phHandle, HI_CHAR* pcFilename)
{
    FILE* pFile = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    pFile = fopen(pcFilename, "r");
    if (HI_NULL_PTR == pFile)
    {
        COM_PrintFuncErr(pFile, COM_ERR_FILE);
        return COM_ERR_FILE;
    }

    *phHandle = (HI_HANDLE)pFile;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 fsFileClose(HI_HANDLE hHandle)
{
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    s32Ret = fclose((FILE*)hHandle);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(fclose, COM_ERR_FILE);
        return COM_ERR_FILE;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_U64 fsFileRead(HI_HANDLE hHandle, HI_VOID* pBuffer, HI_U64 u64MaxSize)
{
    return (HI_U64)fread(pBuffer, 1, (HI_SIZE_T)u64MaxSize, (FILE*)hHandle);
}

static HI_S32 fsFileGetSize(HI_HANDLE hHandle, HI_U64* pu64FileSize)
{
    struct stat stStatBuf;
    HI_S32 s32FileNo = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    s32FileNo = fileno((FILE*)hHandle);

    COM_MemSet(&stStatBuf, 0x00, sizeof(stStatBuf));
    s32Ret = fstat(s32FileNo, &stStatBuf);
    if (0 != s32Ret)
    {
        COM_PrintErrCode(COM_ERR_FILE);
        return COM_ERR_FILE;
    }

    *pu64FileSize = (HI_U64)stStatBuf.st_size;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 fsPartitionReset(HI_CHAR* pcPartitionName)
{
    HI_U32   u32Index = 0;
    HI_CHAR  acBootArgs[DOWNLOAD_FS_COMMAND_LEN] = {0};
    HI_CHAR  acCommand[DOWNLOAD_FS_COMMAND_LEN] = {0};
    HI_CHAR* pcPartDes = HI_NULL_PTR;
    pid_t child_pid = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = fsGetBootargs(acBootArgs, DOWNLOAD_FS_COMMAND_LEN);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(fsGetBootargs, s32Ret);
        return COM_ERR_NOEXIST;
    }

    pcPartDes = COM_StrStr(acBootArgs, "mmcblk0:");
    if (HI_NULL_PTR != pcPartDes)
    {
        s32Ret = fsGetDeviceIndex(pcPartDes, pcPartitionName, &u32Index);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(fsGetDeviceIndex, s32Ret);
            return COM_ERR_NOEXIST;
        }

        snprintf(acCommand, sizeof(acCommand), "mkfs.ext4 /dev/mmcblk0p%d -F", u32Index + 1);
        child_pid = system(acCommand);
        if (!((-1 != child_pid) && WIFEXITED(child_pid) && (0 == WEXITSTATUS(child_pid))))
        {
            COM_PrintErrCode(COM_ERR_DEVICE);
            return COM_ERR_DEVICE;
        }

        return HI_SUCCESS;
    }

    COM_PrintErrCode(COM_ERR_UNSUPPORTED);
    return COM_ERR_UNSUPPORTED;
}

#endif

static HI_S32 fsRestructureFileName(HI_S8* ps8FileName, HI_S8* ps8DesFileName, HI_U32 u32DesLenth)
{
    HI_U32 u32Len = 0;

    COM_DBG_FuncEnter();

    u32Len = COM_StrLen(ps8FileName);
    if ((0 == u32Len)
        || (u32DesLenth <= u32Len))
    {
        COM_PrintErrCode(COM_ERR_PARAM);
        return COM_ERR_PARAM;
    }

    if ('/' == ps8FileName[0])
    {
        snprintf(ps8DesFileName, u32DesLenth, "%s%s", COM_CFG_D_UPGRADE_FILEPATH, ps8FileName);
    }
    else
    {
        snprintf(ps8DesFileName, u32DesLenth, "%s/%s", COM_CFG_D_UPGRADE_FILEPATH, ps8FileName);
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_FS_Init(HI_LOADER_PARAM_S* pstLoaderParam)
{
    HI_CHAR acFileName[HI_LOADER_FILENAME_LEN] = {0};
    HI_S32  s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstLoaderParam);

    if (HI_TRUE == s_bInited)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if (HI_LOADER_UPGRADE_TYPE_FS == pstLoaderParam->enUpgradeType)
    {
        s32Ret = fsRestructureFileName(pstLoaderParam->unParam.stFSParam.as8FileName, acFileName, HI_LOADER_FILENAME_LEN);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(fsRestructureFileName, s32Ret);
            return s32Ret;
        }

        s32Ret = fsFileCheckInPartiton(pstLoaderParam->unParam.stFSParam.as8PartitonName, acFileName);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(fsFileCheckInPartiton, s32Ret);
            return s32Ret;
        }
    }
    else if (HI_LOADER_UPGRADE_TYPE_USB == pstLoaderParam->enUpgradeType)
    {
        s32Ret = fsRestructureFileName(pstLoaderParam->unParam.stUSBParam.as8FileName, acFileName, HI_LOADER_FILENAME_LEN);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(fsRestructureFileName, s32Ret);
            return s32Ret;
        }

        s32Ret = fsFileCheckInUsb(acFileName);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(fsFileCheckInUsb, s32Ret);

            COM_Delayms(5000);

            s32Ret = fsFileCheckInUsb(acFileName);
            if (HI_SUCCESS != s32Ret)
            {
                COM_PrintFuncErr(fsFileCheckInUsb, s32Ret);
                return s32Ret;
            }
        }
    }
    else
    {
        COM_PrintErrCode(COM_ERR_UNKNOWNED);
        return COM_ERR_UNKNOWNED;
    }

    s32Ret = fsFileOpen(&s_hFileHandle, acFileName);
    if (HI_NULL_PTR == s_hFileHandle)
    {
        COM_PrintFuncErr(fsFileOpen, s32Ret);
        return s32Ret;
    }

    s_bInited = HI_TRUE;

    COM_DBG_FuncExit();
    return s32Ret;
}

HI_S32 DOWNLOAD_FS_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    if (HI_TRUE != s_bInited)
    {
        COM_PrintErrCode(COM_ERR_UNINITED);
        return COM_ERR_UNINITED;
    }

    COM_CHECK(fsFileClose(s_hFileHandle));
    s_hFileHandle = HI_NULL_PTR;

#ifdef HI_LOADER_APPLOADER
    COM_CHECK(fsUMount(COM_CFG_D_UPGRADE_FILEPATH));
#endif

    s_bInited = HI_FALSE;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_FS_Getdata(HI_VOID* pbuffer, HI_U32 u32BufferSize, HI_U32* pu32DataLen)
{
    HI_U64 s64RetSize = 0;

    COM_CHECK_PARAM(HI_NULL_PTR == pbuffer);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32DataLen);
    COM_CHECK_PARAM(0 == u32BufferSize);

    if (HI_TRUE != s_bInited)
    {
        COM_PrintErrCode(COM_ERR_UNINITED);
        return COM_ERR_UNINITED;
    }

    s64RetSize = fsFileRead(s_hFileHandle, pbuffer, (HI_U64)u32BufferSize);
    if (0 == s64RetSize)
    {
        COM_PrintFuncErr(fsFileRead, COM_ERR_FILE);
        return COM_ERR_FILE;
    }

    *pu32DataLen = (HI_U32)s64RetSize;

    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_FS_GetFileSize(HI_U32* pu32FileSize)
{
    HI_U64  u64FileSize = 0;
    HI_S32  s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu32FileSize);

    if (HI_TRUE != s_bInited)
    {
        COM_PrintErrCode(COM_ERR_UNINITED);
        return COM_ERR_UNINITED;
    }

    s32Ret = fsFileGetSize(s_hFileHandle, &u64FileSize);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(fsFileGetSize, COM_ERR_FILE);
        return COM_ERR_FILE;
    }

    *pu32FileSize = (HI_U32)u64FileSize;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_FS_PartitionReset(HI_VOID)
{
    HI_S32  s32Ret = HI_SUCCESS;

    s32Ret = fsPartitionReset("cache");
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(fsPartitionReset, COM_ERR_FILE);
        return COM_ERR_FILE;
    }

    s32Ret = fsPartitionReset("userdata");
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(fsPartitionReset, COM_ERR_FILE);
        return COM_ERR_FILE;
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

