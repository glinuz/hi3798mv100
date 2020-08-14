/******************************************************************************
 Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : mpi_descrambler.c
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 2013/04/16
 Description   :
******************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "hi_type.h"
#include "hi_debug.h"

#include "demux_debug.h"
#include "hi_mpi_descrambler.h"

#include "drv_descrambler_ioctl.h"

extern HI_S32 g_s32DmxFd;

#define MPIDscrCheckDeviceFd()          \
    do                                  \
    {                                   \
        if (-1 == g_s32DmxFd)           \
        {                               \
            return HI_ERR_DMX_NOT_INIT; \
        }                               \
    } while (0)

#define MPIDscrCheckPointer(p)          \
    do                                  \
    {                                   \
        if (HI_NULL == p)               \
        {                               \
            return HI_ERR_DMX_NULL_PTR; \
        }                               \
    } while (0)

HI_S32 HI_MPI_DMX_CreateDescrambler(HI_U32 u32DmxId, const HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstDesramblerAttr, HI_HANDLE *phKey)
{
    HI_S32          ret;
    DMX_NewKey_S    Param;

    MPIDscrCheckDeviceFd();
    MPIDscrCheckPointer(pstDesramblerAttr);
    MPIDscrCheckPointer(phKey);

    Param.DmxId = u32DmxId;
    memcpy(&Param.DesramblerAttr, pstDesramblerAttr, sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_KEYS_NEW, &Param);
    if (HI_SUCCESS == ret)
    {
        *phKey = Param.KeyHandle;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_DestroyDescrambler(HI_HANDLE hKey)
{
    MPIDscrCheckDeviceFd();

    return ioctl(g_s32DmxFd, CMD_DEMUX_KEYS_DEL, &hKey);
}

HI_S32 HI_MPI_DMX_GetDescramblerAttr(HI_HANDLE hKey, HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstAttr)
{
    HI_S32 ret = HI_FAILURE;
    DMX_DescramblerAttr_S stDescAttr;

    MPIDscrCheckDeviceFd();
    MPIDscrCheckPointer(pstAttr);

    stDescAttr.hKeyHandle = hKey;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_KEYS_GET_DESCRAMBLERATTR, &stDescAttr);
    if(HI_SUCCESS != ret)
    {
        goto out;
    }

    pstAttr->enCaType = stDescAttr.stDescramblerAttr.enCaType;
    pstAttr->enDescramblerType = stDescAttr.stDescramblerAttr.enDescramblerType;
    pstAttr->enEntropyReduction = stDescAttr.stDescramblerAttr.enEntropyReduction;
    
out:
    return ret;
}

HI_S32 HI_MPI_DMX_SetDescramblerAttr(HI_HANDLE hKey, HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstAttr)
{
    DMX_DescramblerAttr_S stDescAttr;

    MPIDscrCheckDeviceFd();
    MPIDscrCheckPointer(pstAttr);

    stDescAttr.hKeyHandle = hKey;
    
    stDescAttr.stDescramblerAttr.enCaType = pstAttr->enCaType;
    stDescAttr.stDescramblerAttr.enDescramblerType = pstAttr->enDescramblerType;
    stDescAttr.stDescramblerAttr.enEntropyReduction = pstAttr->enEntropyReduction;

    return ioctl(g_s32DmxFd, CMD_DEMUX_KEYS_SET_DESCRAMBLERATTR, &stDescAttr);
}
HI_S32 HI_MPI_DMX_SetDescramblerEvenKey(HI_HANDLE hKey, const HI_U8 *pu8EvenKey)
{
    DMX_KeySet_S Param;

    MPIDscrCheckDeviceFd();
    MPIDscrCheckPointer(pu8EvenKey);

    Param.KeyHandle = hKey;
    memcpy(Param.Key, pu8EvenKey, sizeof(Param.Key));

    return ioctl(g_s32DmxFd, CMD_DEMUX_KEYS_SET_EVEN, &Param);
}

HI_S32 HI_MPI_DMX_SetDescramblerOddKey(HI_HANDLE hKey, const HI_U8 *pu8OddKey)
{
    DMX_KeySet_S Param;

    MPIDscrCheckDeviceFd();
    MPIDscrCheckPointer(pu8OddKey);

    Param.KeyHandle = hKey;
    memcpy(Param.Key, pu8OddKey, sizeof(Param.Key));

    return ioctl(g_s32DmxFd, CMD_DEMUX_KEYS_SET_ODD, &Param);
}

HI_S32 HI_MPI_DMX_SetDescramblerEvenIVKey(HI_HANDLE hKey, const HI_U8 *pu8IVKey)
{
    DMX_KeySet_S Param;

    MPIDscrCheckDeviceFd();
    MPIDscrCheckPointer(pu8IVKey);

    Param.KeyHandle = hKey;
    memcpy(Param.Key, pu8IVKey, sizeof(Param.Key));

    return ioctl(g_s32DmxFd, CMD_DEMUX_KEYS_SET_IVEVEN, &Param);
}

HI_S32 HI_MPI_DMX_SetDescramblerOddIVKey(HI_HANDLE hKey, const HI_U8 *pu8IVKey)
{
    DMX_KeySet_S Param;

    MPIDscrCheckDeviceFd();
    MPIDscrCheckPointer(pu8IVKey);

    Param.KeyHandle = hKey;
    memcpy(Param.Key, pu8IVKey, sizeof(Param.Key));

    return ioctl(g_s32DmxFd, CMD_DEMUX_KEYS_SET_IVODD, &Param);
}

HI_S32 HI_MPI_DMX_AttachDescrambler(HI_HANDLE hKey, HI_HANDLE hChannel)
{
    DMX_KeyAttach_S Param;

    MPIDscrCheckDeviceFd();

    Param.KeyHandle = hKey;
    Param.ChanHandle= hChannel;

    return ioctl(g_s32DmxFd, CMD_DEMUX_KEYS_ATTACH, &Param);
}

HI_S32 HI_MPI_DMX_DetachDescrambler(HI_HANDLE hKey, HI_HANDLE hChannel)
{
    DMX_KeyAttach_S Param;

    MPIDscrCheckDeviceFd();

    Param.KeyHandle = hKey;
    Param.ChanHandle= hChannel;

    return ioctl(g_s32DmxFd, CMD_DEMUX_KEYS_DETACH, &Param);
}

HI_S32 HI_MPI_DMX_GetDescramblerKeyHandle(HI_HANDLE hChannel, HI_HANDLE *phKey)
{
    HI_S32          ret;
    DMX_KeyAttach_S Param;

    MPIDscrCheckDeviceFd();
    MPIDscrCheckPointer(phKey);

    Param.ChanHandle = hChannel;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_KEYS_GET_ID, &Param);
    if (HI_SUCCESS == ret)
    {
        *phKey = Param.KeyHandle;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_GetFreeDescramblerKeyCount(HI_U32 u32DmxId, HI_U32 *pu32FreeCount)
{
    HI_S32              ret;
    DMX_FreeKeyGet_S    Param;

    MPIDscrCheckDeviceFd();
    MPIDscrCheckPointer(pu32FreeCount);

    Param.DmxId = u32DmxId;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_KEYS_GET_FREE, &Param);
    if (HI_SUCCESS == ret)
    {
        *pu32FreeCount = Param.FreeCount;
    }

    return ret;
}

