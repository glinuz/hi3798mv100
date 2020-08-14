/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : com_osal.c
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
#include <hi_common.h>
#else
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <fcntl.h>
#include <ctype.h>
#endif
#include "com_osal.h"
#include "com_debug.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/

/*************************** Structure Definition *****************************/

/*************************** API Declaration **********************************/

HI_S32  s_s32AvailableSize = 0; /** maximum available memory 0: not available*/

HI_U32 COM_StrLen(const HI_CHAR* pcStr)
{
    if (HI_NULL_PTR == pcStr)
    {
        COM_PrintErrCode(COM_ERR_PARAM);
        return 0;
    }

    return strlen(pcStr);
}

HI_S32 COM_StrNCmp(const HI_CHAR* pcStr1, const HI_CHAR* pcStr2, HI_U32 u32Count)
{
    return strncmp(pcStr1, pcStr2, u32Count);
}

HI_U64 COM_Strtoul(const HI_CHAR* nptr, HI_CHAR** endptr, int base)
{
#ifdef HI_LOADER_BOOTLOADER
    return simple_strtoul(nptr, endptr, base);
#else
    return strtoul(nptr, endptr, base);
#endif
}

HI_S32 COM_StrToS32(const HI_CHAR* pcStr)
{
    return atoi(pcStr);
}

HI_U32 COM_StrToU32(const HI_CHAR* pcStr)
{
    return (HI_U32)COM_Strtoul(pcStr, NULL, 0);
}

HI_S64 COM_StrToS64(const HI_CHAR* pcStr)
{
    return atoll(pcStr);
}

HI_U64 COM_StrToU64(const HI_CHAR* pcStr)
{
    return COM_Strtoul(pcStr, NULL, 0);
}

HI_CHAR* COM_StrStr(const HI_CHAR* pcStr1, const HI_CHAR* pcStr2)
{
    if ((HI_NULL_PTR == pcStr1)
        || (HI_NULL_PTR == pcStr2))
    {
        COM_PrintErrCode(COM_ERR_PARAM);
        return HI_NULL_PTR;
    }

    return (HI_CHAR*)strstr(pcStr1, pcStr2);
}

HI_CHAR* COM_StrNCpy(HI_CHAR* pcDestination, const HI_CHAR* pcSource , HI_U32 u32Count)
{
    if ((HI_NULL_PTR == pcDestination)
        || (HI_NULL_PTR == pcSource)
        || (0 == u32Count))
    {
        COM_PrintErrCode(COM_ERR_PARAM);
        return pcDestination;
    }

    return (HI_CHAR*)strncpy(pcDestination, pcSource, u32Count);
}

HI_VOID* COM_MemCpy(HI_VOID* pDest, const HI_VOID* pSrc, HI_U32 u32Count)
{
    if ((HI_NULL_PTR == pDest)
        || (HI_NULL_PTR == pSrc)
        || (0 == u32Count))
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return pDest;
    }

    return memcpy(pDest, pSrc, (size_t)u32Count);
}

HI_VOID* COM_MemSet(HI_VOID* pToSet, HI_S32 s32SetChar, HI_U32 u32Count)
{
    if ((HI_NULL_PTR == pToSet)
        || (0 == u32Count))
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return pToSet;
    }

    return memset(pToSet, s32SetChar, (size_t)u32Count);
}

HI_VOID* COM_MemMalloc(HI_U32 u32Size)
{
    if (0 == u32Size)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return HI_NULL_PTR;
    }

    return malloc((size_t)u32Size);
}

HI_VOID COM_MemFree(HI_VOID* pBuf)
{
    if (HI_NULL_PTR == pBuf)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return;
    }

    free(pBuf);
    return;
}

HI_U32 COM_MemSetAvailableSize(HI_U32 u32AvailableSize)
{
    s_s32AvailableSize = u32AvailableSize;

    return HI_SUCCESS;
}

#ifdef HI_LOADER_BOOTLOADER
HI_VOID* COM_MemGetUsable(HI_U32 u32ApplySize, HI_U32* pu32Size)
{
    HI_U32 u32Addr = 0;

    if (0 == u32ApplySize)
    {
        COM_PrintErrCode(COM_ERR_PARAM);
        return HI_NULL_PTR;
    }

    if (HI_NULL_PTR == pu32Size)
    {
        COM_PrintErrCode(COM_ERR_PARAM);
        return HI_NULL_PTR;
    }

    if ((0 < s_s32AvailableSize)
        && (u32ApplySize > s_s32AvailableSize))
    {
        u32ApplySize = s_s32AvailableSize;
    }

    while (u32ApplySize)
    {
        HI_MEM_Alloc(&u32Addr, u32ApplySize);

        if (0 == u32Addr)
        {
            u32ApplySize = u32ApplySize / 2;
        }
        else
        {
            break;
        }
    }

    *pu32Size = u32ApplySize;

    return (HI_U8*)u32Addr;
}

HI_VOID COM_MemFreeUsable(HI_VOID* buf)
{
    return;
}

HI_VOID COM_Delayms(HI_U32 u32TimeMS)
{
    udelay(u32TimeMS * 1000);
    return;
}

#else

HI_VOID* COM_MemGetUsable(HI_U32 u32ApplySize, HI_U32* pu32Size)
{
    HI_U32 u32AvailableMem = 0;
    HI_VOID* pMemAddr = HI_NULL_PTR;
    struct sysinfo stSysInfo;

    if (0 == u32ApplySize)
    {
        COM_PrintErrCode(COM_ERR_PARAM);
        return HI_NULL_PTR;
    }

    if (HI_NULL_PTR == pu32Size)
    {
        COM_PrintErrCode(COM_ERR_PARAM);
        return HI_NULL_PTR;
    }

    COM_MemSet(&stSysInfo, 0x00, sizeof(stSysInfo));
    if (-1 == sysinfo(&stSysInfo))
    {
        COM_PrintFuncErr(sysinfo, COM_ERR_DEVICE);
        return HI_NULL_PTR;
    }

    u32AvailableMem = (HI_U32)(stSysInfo.freeram * 0.8f); /** Reserve 80% for other service */
    u32ApplySize = (u32AvailableMem < u32ApplySize) ? u32AvailableMem : u32ApplySize;

    pMemAddr = COM_MemMalloc(u32ApplySize);
    if (HI_NULL_PTR == pMemAddr)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return HI_NULL_PTR;
    }

    *pu32Size = u32ApplySize;

    return pMemAddr;
}

HI_VOID COM_MemFreeUsable(HI_VOID* ptr)
{
    COM_MemFree(ptr);
}

HI_VOID COM_Delayms(HI_U32 u32TimeMS)
{
    usleep(u32TimeMS * 1000);
}

#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

