/******************************************************************************

  Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_common.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/15
  Description   : Common apis for hisilicon system.
  History       :
  1.Date        : 2010/01/25
    Author      : jianglei
    Modification: Created file

*******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_module.h"
#include "hi_osal.h"

#include "mpi_mmz.h"
#include "hi_mpi_mem.h"
#include "hi_drv_struct.h"
#include "mpi_module.h"
#include "mpi_log.h"
#include "drv_sys_ioctl.h"
#include "hi_mpi_stat.h"
#include "mpi_memdev.h"
#include "mpi_userproc.h"

//////////////////////////////////////////////////////////////////////////////////////
/// STATIC CONST Variable
//////////////////////////////////////////////////////////////////////////////////////
static const HI_U8 s_szMonth[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

#ifndef HI_ADVCA_FUNCTION_RELEASE
static const HI_CHAR s_szCommonVersion[] __attribute__((used)) = USER_VERSION_STRING;
#endif

static pthread_mutex_t   s_SysMutex = PTHREAD_MUTEX_INITIALIZER;

/// STATIC Variable
static HI_U32 s_u32SysInitTimes = 0;
static HI_S32 s_s32SysFd = -1;

/// DEFINATION MACRO
#define SYS_OPEN_FILE                                       \
do{                                                         \
    if (-1 == s_s32SysFd)                                   \
    {                                                       \
        s_s32SysFd = open("/dev/"UMAP_DEVNAME_SYS, O_RDWR); \
        if (s_s32SysFd < 0)                                 \
        {                                                   \
            HI_PRINT("open error\n");                       \
            HI_SYS_UNLOCK();                                \
            return HI_FAILURE;                              \
        }                                                   \
    }                                                       \
}while(0)

#define HI_SYS_LOCK()     (void)pthread_mutex_lock(&s_SysMutex);
#define HI_SYS_UNLOCK()   (void)pthread_mutex_unlock(&s_SysMutex);

#define IS_CHIP_TYPE(type) (type == SysVer.enChipTypeHardWare)
#define IS_CHIP(type, ver) ((type == SysVer.enChipTypeHardWare) && (ver == SysVer.enChipVersion))

HI_S32 HI_SYS_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    HI_SYS_LOCK();

    if (0 == s_u32SysInitTimes ++)
    {
        SYS_OPEN_FILE;

        s32Ret = HI_MPI_LogInit();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_SYS("HI_MPI_LogInit failure: %d\n", s32Ret);
            goto LogErrExit;
        }

        s32Ret = HI_MODULE_Init();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_SYS("HI_ModuleMGR_Init failure: %d\n", s32Ret);
            goto ModuleErrExit;
        }

        //MUST unlock, because HI_MPI_STAT_Init calling HI_SYS_GetVersion will cause deadlock
        HI_SYS_UNLOCK();

        s32Ret = HI_MPI_STAT_Init();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_SYS("HI_MPI_STAT_Init failure: %d\n", s32Ret);
            goto StatErrExit;
        }

        s32Ret = MPI_MEMDEV_Init();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_SYS("MPI_MEMDEV_Init failure: %d\n", s32Ret);
            goto MemdevErrExit;
        }

        s32Ret = MPI_UPROC_Init();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_SYS("MPI_UPROC_Init failure: %d\n", s32Ret);
            goto UprocErrExit;
        }

        HI_SYS_LOCK();

        HI_INFO_SYS("HI_SYS_Init init OK\n");
    }

    HI_SYS_UNLOCK();

    return (HI_S32)HI_SUCCESS;

UprocErrExit:
    (HI_VOID)MPI_MEMDEV_DeInit();

MemdevErrExit:
    (HI_VOID)HI_MPI_STAT_DeInit();

StatErrExit:
    /* Lock for access of s_s32SysInitTimes */
    HI_SYS_LOCK();
    (HI_VOID)HI_MODULE_DeInit();

ModuleErrExit:
    HI_MPI_LogDeInit();

LogErrExit:
    if (s_s32SysFd != -1)
    {
        close(s_s32SysFd);
        s_s32SysFd = -1;
    }

    s_u32SysInitTimes --;

    HI_SYS_UNLOCK();

    return (HI_S32)HI_FAILURE;
}

HI_S32 HI_SYS_DeInit(HI_VOID)
{
    HI_SYS_LOCK();

    if (!s_u32SysInitTimes)
    {
        goto out;
    }

    if (0 == --s_u32SysInitTimes)
    {
        (HI_VOID)MPI_UPROC_DeInit();

        (HI_VOID)MPI_MEMDEV_DeInit();

        (HI_VOID)HI_MPI_STAT_DeInit();

        (HI_VOID)HI_MODULE_DeInit();

        HI_MPI_LogDeInit();

        if (s_s32SysFd != -1)
        {
            close(s_s32SysFd);
            s_s32SysFd = -1;
        }
    }
out:
    HI_SYS_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_SYS_GetBuildTime(struct tm * pstTime)
{
    char szData[] = __DATE__;
    char szTime[] = __TIME__;
    char szTmp[5];
    int i;

    if (NULL == pstTime)
    {
        return HI_FAILURE;
    }

    /* month */
    memset(szTmp, 0, sizeof(szTmp));
    (HI_VOID)HI_OSAL_Strncpy(szTmp, szData, 3);

    for(i=0; i<12; i++)
    {
            if(!HI_OSAL_Strncmp((const char*)s_szMonth[i], szTmp, sizeof(s_szMonth[i])))
            {
                pstTime->tm_mon = i + 1;
                break;
            }
    }

    /* day */
    memset(szTmp, 0, sizeof(szTmp));
    (HI_VOID)HI_OSAL_Strncpy(szTmp, szData+4, 2);
    pstTime->tm_mday = atoi(szTmp);

    /* year */
    memset(szTmp, 0, sizeof(szTmp));
    (HI_VOID)HI_OSAL_Strncpy(szTmp, szData+7, 4);
    pstTime->tm_year = atoi(szTmp);

    /* hour */
    memset(szTmp, 0, sizeof(szTmp));
    (HI_VOID)HI_OSAL_Strncpy(szTmp, szTime, 2);
    pstTime->tm_hour = atoi(szTmp);

    /* minute */
    memset(szTmp, 0, sizeof(szTmp));
    (HI_VOID)HI_OSAL_Strncpy(szTmp, szTime+3, 2);
    pstTime->tm_min = atoi(szTmp);


    /* second */
    memset(szTmp, 0, sizeof(szTmp));
    (HI_VOID)HI_OSAL_Strncpy(szTmp, szTime+6, 2);
    pstTime->tm_sec = atoi(szTmp);

    return HI_SUCCESS;
}

HI_S32 HI_SYS_GetVersion(HI_SYS_VERSION_S *pstVersion)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (NULL == pstVersion)
    {
        return HI_FAILURE;
    }

    HI_SYS_LOCK();

    if (s_s32SysFd < 0)
    {
        HI_SYS_UNLOCK();
        return HI_FAILURE;
    }

    s32Ret = ioctl(s_s32SysFd, SYS_GET_SYS_VERSION, pstVersion);
    if (s32Ret != 0)
    {
        HI_SYS_UNLOCK();

        HI_ERR_SYS("ioctl SYS_GET_SYS_VERSION error!\n");
        return HI_FAILURE;
    }
#ifndef HI_ADVCA_FUNCTION_RELEASE
    (HI_VOID)HI_OSAL_Snprintf(pstVersion->aVersion, sizeof(pstVersion->aVersion), "%s", s_szCommonVersion);
#endif
    /* for detect soft and chip dismatch */
#if defined(CHIP_TYPE_hi3716cv200)
    pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3716C;
#elif defined(CHIP_TYPE_hi3716mv400) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3716M;
#elif defined(CHIP_TYPE_hi3718cv100)
    pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3718C;
#elif defined(CHIP_TYPE_hi3718mv100)
    pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3718M;
#elif defined(CHIP_TYPE_hi3719cv100)
    pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3719C;
#elif defined(CHIP_TYPE_hi3719mv100)
    pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3719M;
#elif defined(CHIP_TYPE_hi3796cv100)
    pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3796C;
#elif defined(CHIP_TYPE_hi3798cv100) || defined(CHIP_TYPE_hi3798cv200)
    pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3798C;
#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3798mv200)
    pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3798M;
#elif defined(CHIP_TYPE_hi3798mv200_a)
    pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3798M_A;
#elif defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3796mv200)
    pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3796M;
#else
    pstVersion->enChipTypeSoft = HI_CHIP_TYPE_BUTT;
#endif

    HI_SYS_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_SYS_GetChipPackageType(HI_CHIP_PACKAGE_TYPE_E *penPackageType)
{
    HI_S32 ret = HI_FAILURE;

    if (NULL == penPackageType)
    {
        return HI_FAILURE;
    }

    HI_SYS_LOCK();

    if (s_s32SysFd >= 0)
    {
        ret = ioctl(s_s32SysFd, SYS_GET_CHIPPACKAGETYPE, (HI_U32*)penPackageType);
    }

    HI_SYS_UNLOCK();

    return ret;
}

static HI_S32 GetChipCapSupportHelper(HI_CHIP_CAP_E enChipCap, HI_U32 *pu32Support)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (!pu32Support)
        return HI_FAILURE;

    HI_SYS_LOCK();

    if (s_s32SysFd < 0)
    {
        s32Ret = HI_FAILURE;
        goto out;
    }

    switch(enChipCap)
    {
        case HI_CHIP_CAP_DOLBY:
            s32Ret = ioctl(s_s32SysFd, SYS_GET_DOLBYSUPPORT, pu32Support);
            break;

        case HI_CHIP_CAP_DTS:
            s32Ret =  ioctl(s_s32SysFd, SYS_GET_DTSSUPPORT, pu32Support);
            break;

        case HI_CHIP_CAP_ADVCA:
            s32Ret =  ioctl(s_s32SysFd, SYS_GET_ADVCASUPPORT, pu32Support);
            break;

        case HI_CHIP_CAP_MACROVISION:
            s32Ret =  ioctl(s_s32SysFd, SYS_GET_MACROVISIONSUPPORT, pu32Support);
            break;

        case HI_CHIP_CAP_HDR10:
            s32Ret =  ioctl(s_s32SysFd, SYS_GET_HDR10SUPPORT, pu32Support);    
            break;

        case HI_CHIP_CAP_DOLBYVISION:
            s32Ret =  ioctl(s_s32SysFd, SYS_GET_DOLBYVISIONSUPPORT, pu32Support);
            break;

        default:
            HI_ERR_SYS("invalid chip attrs type!\n");
            s32Ret = HI_FAILURE;
            break;
    }

out:
    HI_SYS_UNLOCK();

    return s32Ret;
}

HI_S32 HI_SYS_GetChipCapability(HI_CHIP_CAP_E enChipCap, HI_BOOL *pbSupport)
{
    HI_S32  s32Ret;
    HI_U32 u32Support = 0;

    if (!pbSupport)
    {
        HI_ERR_SYS("null ptr!\n");
        return HI_FAILURE;
    }

    s32Ret = GetChipCapSupportHelper(enChipCap, &u32Support);

    if (s32Ret == HI_SUCCESS)
    {
        *pbSupport = (HI_BOOL)u32Support;
    }

    return s32Ret;
}

HI_S32 HI_SYS_GetChipAttr(HI_SYS_CHIP_ATTR_S *pstChipAttr)
{
    HI_S32              s32Ret = HI_FAILURE;
    HI_U32              u32Support = 0;

    if (!pstChipAttr)
    {
        HI_ERR_SYS("null ptr!\n");
        return HI_FAILURE;
    }

    s32Ret = GetChipCapSupportHelper(HI_CHIP_CAP_DOLBY, &u32Support);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SYS("Get ChipAttr DOLBY error!\n");
        return HI_FAILURE;
    }
    pstChipAttr->bDolbySupport = (HI_BOOL)u32Support;

    s32Ret = GetChipCapSupportHelper(HI_CHIP_CAP_DTS, &u32Support);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SYS("Get ChipAttr DTS error!\n");
        return HI_FAILURE;
    }
    pstChipAttr->bDTSSupport = (HI_BOOL)u32Support;

    s32Ret = GetChipCapSupportHelper(HI_CHIP_CAP_ADVCA, &u32Support);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SYS("Get ChipAttr ADVCA error!\n");
        return HI_FAILURE;
    }
    pstChipAttr->bADVCASupport = (HI_BOOL)u32Support;

    s32Ret = GetChipCapSupportHelper(HI_CHIP_CAP_MACROVISION, &u32Support);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SYS("Get ChipAttr MACROVISION error!\n");
        return HI_FAILURE;
    }
    pstChipAttr->bMacrovisionSupport = (HI_BOOL)u32Support;

    s32Ret = GetChipCapSupportHelper(HI_CHIP_CAP_HDR10, &u32Support);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SYS("Get ChipAttr HDR10 error!\n");
        return HI_FAILURE;
    }
    pstChipAttr->bHDR10 = (HI_BOOL)u32Support;

    s32Ret = GetChipCapSupportHelper(HI_CHIP_CAP_DOLBYVISION, &u32Support);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SYS("Get ChipAttr DOLBYVISION error!\n");
        return HI_FAILURE;
    }
    pstChipAttr->bDolbyvisionSupport = (HI_BOOL)u32Support;

    s32Ret = ioctl(s_s32SysFd, SYS_GET_64DIEID, &(pstChipAttr->u64ChipID));
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SYS("Get ChipAttr 64DIEID error!\n");
        return HI_FAILURE;
    }

    s32Ret = ioctl(s_s32SysFd, SYS_GET_32DIEID, &(pstChipAttr->u32ChipID));
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SYS("Get ChipAttr 32DIEID error!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}

HI_S32 HI_SYS_CRC32(HI_U8 *pu8Src, HI_U32 u32SrcLen, HI_U32 *pu32Dst)
{
    HI_S32          Ret = HI_SUCCESS;
    unsigned long   poly_nomial = 0x04c11db7;
    unsigned long   CrcTable[256];          /*Calculate CRC32*/
    unsigned long   crc = 0xffffffff;

    if (HI_NULL == pu8Src || HI_NULL == pu32Dst || 0 == u32SrcLen)
    {
        HI_ERR_SYS("Invalid parameter!\n");
        return HI_FAILURE;
    }

    /*Init _CrcTable[256] Flag, Only init once time*/
    int   i, j;
    unsigned long crc_accum;

    for (i = 0; i < 256;  i++)
    {
       crc_accum =  ( i << 24 );

       for (j = 0; j < 8; j++)
       {
           if( crc_accum & 0x80000000L )
               crc_accum = ( crc_accum << 1 ) ^ poly_nomial;
           else
               crc_accum = ( crc_accum << 1 );
       }

       CrcTable[i] = crc_accum;
    }

    while (u32SrcLen--)
    {
        crc = (crc << 8) ^ CrcTable[((crc >> 24) ^ *pu8Src++) & 0xff];
    }

    *pu32Dst = crc;

    return Ret;
}

HI_S32 HI_SYS_GetMemConfig(HI_SYS_MEM_CONFIG_S *pstConfig)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (NULL == pstConfig)
    {
        HI_ERR_SYS("Get DDRConf ptr!\n");

        return HI_FAILURE;
    }

    HI_SYS_LOCK();

    if (s_s32SysFd < 0)
    {
        HI_SYS_UNLOCK();
        return HI_FAILURE;
    }

    s32Ret = ioctl(s_s32SysFd, SYS_GET_DDRCONFIG, pstConfig);

    HI_SYS_UNLOCK();

    return s32Ret;
}

HI_S32 HI_SYS_SetConf(const HI_SYS_CONF_S *pstSysConf)
{
    HI_S32 s32Ret = HI_FAILURE;

    if ( NULL == pstSysConf )
    {
        HI_ERR_SYS("Set pstSysConf ptr!\n");

        return HI_FAILURE;
    }

    HI_SYS_LOCK();

    if (s_s32SysFd < 0)
    {
        HI_SYS_UNLOCK();
        return HI_FAILURE;
    }

    s32Ret = ioctl(s_s32SysFd, SYS_SET_CONFIG_CTRL, pstSysConf);

    HI_SYS_UNLOCK();

    return s32Ret;
}

HI_S32 HI_SYS_GetConf(HI_SYS_CONF_S *pstSysConf)
{
    HI_S32 s32Ret = HI_FAILURE;

    if ( NULL == pstSysConf )
    {
        HI_ERR_SYS("Get pstSysConf ptr!\n");

        return HI_FAILURE;
    }

    HI_SYS_LOCK();

    if (s_s32SysFd < 0)
    {
        HI_SYS_UNLOCK();
        return HI_FAILURE;
    }

    s32Ret = ioctl(s_s32SysFd, SYS_GET_CONFIG_CTRL, pstSysConf);

    HI_SYS_UNLOCK();

    return s32Ret;
}

HI_S32 HI_SYS_WriteRegister(HI_U32 u32RegAddr, HI_U32 u32Value)
{
    return MPI_MEMDEV_WriteRegister(u32RegAddr, u32Value);
}

HI_S32 HI_SYS_ReadRegister(HI_U32 u32RegAddr, HI_U32 *pu32Value)
{
    return MPI_MEMDEV_ReadRegister(u32RegAddr, pu32Value);
}

HI_S32 HI_SYS_MapRegister(HI_U32 u32RegAddr, HI_U32 u32Length, HI_VOID *pVirAddr)
{
    return MPI_MEMDEV_MapRegister(u32RegAddr, u32Length, pVirAddr);
}

HI_S32 HI_SYS_UnmapRegister(HI_VOID * pVirAddr)
{
    return MPI_MEMDEV_UnmapRegister(pVirAddr);
}

HI_S32 HI_SYS_GetTimeStampMs(HI_U32 *pu32TimeMs)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_NULL == pu32TimeMs)
    {
        HI_ERR_SYS("null pointer error\n");
        return HI_FAILURE;
    }

    HI_SYS_LOCK();

    if (s_s32SysFd < 0)
    {
        HI_SYS_UNLOCK();
        return HI_FAILURE;
    }

    s32Ret = ioctl(s_s32SysFd, SYS_GET_TIMESTAMPMS, pu32TimeMs);

    HI_SYS_UNLOCK();

    return s32Ret;
}

HI_S32 HI_SYS_SetLogLevel(HI_MOD_ID_E enModId,  HI_LOG_LEVEL_E enLogLevel)
{
    return HI_MPI_LogLevelSet((HI_U32)enModId, enLogLevel);
}

HI_S32 HI_SYS_SetLogPath(const HI_CHAR* pszLogPath)
{
    return HI_MPI_LogPathSet(pszLogPath);
}

HI_S32 HI_SYS_SetStorePath(const HI_CHAR* pszPath)
{
    return HI_MPI_StorePathSet(pszPath);
}

HI_S32 HI_PROC_AddDir(const HI_CHAR * pszName)
{
    return MPI_UPROC_AddDir(pszName);
}

HI_S32 HI_PROC_RemoveDir(const HI_CHAR *pszName)
{
    return MPI_UPROC_RemoveDir(pszName);
}

HI_S32 HI_PROC_AddEntry(HI_U32 u32ModuleID, const HI_PROC_ENTRY_S* pstEntry)
{
    return MPI_UPROC_AddEntry(u32ModuleID, pstEntry);
}

HI_S32 HI_PROC_RemoveEntry(HI_U32 u32ModuleID, const HI_PROC_ENTRY_S* pstEntry)
{
    return MPI_UPROC_RemoveEntry(u32ModuleID, pstEntry);
}

HI_S32 HI_PROC_Printf(HI_PROC_SHOW_BUFFER_S *pstBuf, const HI_CHAR *pFmt, ...)
{
    HI_U32 u32Len = 0;
    va_list args = {0};

    if ((HI_NULL == pstBuf) || (HI_NULL == pstBuf->pu8Buf) || (HI_NULL == pFmt))
    {
        return HI_FAILURE;
    }

    /* log buffer overflow */
    if (pstBuf->u32Offset >= pstBuf->u32Size)
    {
        HI_ERR_SYS("userproc log buffer(size:%d) overflow.\n", pstBuf->u32Size);
        return HI_FAILURE;
    }

    va_start(args, pFmt);
    u32Len = (HI_U32)HI_OSAL_Vsnprintf((HI_CHAR*)pstBuf->pu8Buf + pstBuf->u32Offset,
                            pstBuf->u32Size - pstBuf->u32Offset, pFmt, args);
    va_end(args);

    pstBuf->u32Offset += u32Len;

    return HI_SUCCESS;
}

HI_S32 HI_MMZ_Malloc(HI_MMZ_BUF_S *pstBuf)
{
#ifdef HI_SMMU_SUPPORT
    HI_U32  SmmuAddr;
    HI_U8  *UsrAddr;

    if (!pstBuf)
    {
        return HI_FAILURE;
    }

    SmmuAddr = HI_MPI_SMMU_New(pstBuf->bufname, pstBuf->bufsize);
    if (HI_INVALID_MEM_ADDR == SmmuAddr)
    {
        return HI_FAILURE;
    }

    UsrAddr = (HI_U8*)HI_MPI_SMMU_Map(SmmuAddr, HI_FALSE);
    if (!UsrAddr)
    {
        HI_ERR_SYS("HI_MPI_SMMU_Map failed\n");

        HI_MPI_SMMU_Delete(SmmuAddr);

        return HI_FAILURE;
    }

    pstBuf->phyaddr = SmmuAddr;
    pstBuf->kernel_viraddr = HI_NULL;
    pstBuf->user_viraddr = UsrAddr;
    pstBuf->overflow_threshold = 100;
    pstBuf->underflow_threshold = 0;

    return HI_SUCCESS;
#else
    return HI_MPI_MMZ_Malloc(pstBuf);
#endif
}

HI_S32 HI_MMZ_Free(HI_MMZ_BUF_S *pstBuf)
{
#ifdef HI_SMMU_SUPPORT
    HI_S32  ret;
    HI_U32  SmmuAddr;

    if (!pstBuf)
    {
        return HI_FAILURE;
    }

    SmmuAddr = pstBuf->phyaddr;

    ret = HI_MPI_SMMU_Unmap(SmmuAddr);
    if (0 != ret)
    {
        HI_ERR_SYS("HI_MPI_SMMU_Unmap failed\n");
        return HI_FAILURE;
    }

    HI_MPI_SMMU_Delete(SmmuAddr);

    return HI_SUCCESS;
#else
    return HI_MPI_MMZ_Free(pstBuf);
#endif
}

HI_VOID *HI_MMZ_New(HI_U32 u32Size, HI_U32 u32Align, HI_CHAR *ps8MMZName, HI_CHAR *ps8BufName)
{
#ifdef HI_SMMU_SUPPORT
    HI_U32  SmmuAddr;

    SmmuAddr = HI_MPI_SMMU_New(ps8BufName, u32Size);
    if (HI_INVALID_MEM_ADDR == SmmuAddr)
    {
        return HI_NULL;
    }

    UNUSED(u32Align);
    UNUSED(ps8MMZName);

    return (HI_VOID *)(unsigned long)SmmuAddr;
#else
    HI_U32  MmzAddr;

    MmzAddr = HI_MPI_MMZ_New(u32Size, u32Align, ps8MMZName, ps8BufName);
    if (HI_INVALID_MEM_ADDR == MmzAddr)
    {
        return HI_NULL;
    }

    return (HI_VOID*)MmzAddr;
#endif
}

HI_S32 HI_MMZ_Delete(HI_U32 u32PhysAddr)
{
#ifdef HI_SMMU_SUPPORT
    return HI_MPI_SMMU_Delete(u32PhysAddr);
#else
    return HI_MPI_MMZ_Delete(u32PhysAddr);
#endif
}

HI_S32 HI_MMZ_GetPhyaddr(HI_VOID * pVir, HI_U32 *pu32Phyaddr, HI_U32 *pu32Size)
{
#ifdef HI_SMMU_SUPPORT
    return HI_MPI_SMMU_GetSmmuAddrByMapAddr(pVir, pu32Phyaddr, pu32Size);
#else
    return HI_MPI_MMZ_GetPhyAddr(pVir, pu32Phyaddr, pu32Size);
#endif
}

HI_VOID *HI_MMZ_Map(HI_U32 u32PhysAddr, HI_U32 u32Cached)
{
    if ((0 != u32Cached) && (1 != u32Cached))
    {
        HI_ERR_SYS("invalid param\n");
        return HI_NULL;
    }

#ifdef HI_SMMU_SUPPORT
    return HI_MPI_SMMU_Map(u32PhysAddr, (1 == u32Cached) ? HI_TRUE : HI_FALSE);
#else
    return HI_MPI_MMZ_Map(u32PhysAddr, (1 == u32Cached) ? HI_TRUE : HI_FALSE);
#endif
}

HI_S32 HI_MMZ_Unmap(HI_U32 u32PhysAddr)
{
#ifdef HI_SMMU_SUPPORT
    return HI_MPI_SMMU_Unmap(u32PhysAddr);
#else
    return HI_MPI_MMZ_Unmap(u32PhysAddr);
#endif
}

HI_S32 HI_MMZ_Flush(HI_U32 u32PhysAddr)
{
#ifdef HI_SMMU_SUPPORT
    return HI_MPI_SMMU_Flush(u32PhysAddr);
#else
    return HI_MPI_MMZ_Flush(u32PhysAddr);
#endif
}

HI_VOID *HI_MEM_Map(HI_U32 u32PhyAddr, HI_U32 u32Size)
{
    return HI_MMAP(u32PhyAddr, u32Size);
}

HI_S32 HI_MEM_Unmap(HI_VOID *pAddrMapped)
{
    return HI_MUNMAP(pAddrMapped);
}

HI_VOID* HI_MEM_Malloc(HI_U32 u32ModuleID, HI_U32 u32Size)
{
    return HI_MALLOC(u32ModuleID, u32Size);
}

HI_VOID HI_MEM_Free(HI_U32 u32ModuleID, HI_VOID* pMemAddr)
{
    HI_FREE(u32ModuleID, pMemAddr);
}

HI_VOID* HI_MEM_Calloc(HI_U32 u32ModuleID, HI_U32 u32MemBlock, HI_U32 u32Size)
{
    return HI_CALLOC(u32ModuleID, u32MemBlock, u32Size);
}

