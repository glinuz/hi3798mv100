/******************************************************************************
 Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : hi_mpi_descrambler.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 2013/04/16
 Description   :
******************************************************************************/

#ifndef __HI_MPI_DESCRAMBLER_H__
#define __HI_MPI_DESCRAMBLER_H__

#include "hi_type.h"

#include "hi_unf_descrambler.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define DMX_KEYHANDLE_MAGIC      (0x5)
#define DMX_KEYID(KeyHandle)    ((KeyHandle) & 0x000000ff)
#define DMX_KEYHANDLE(DmxId, KeyId)    ((HI_ID_DEMUX << 24) | (DMX_KEYHANDLE_MAGIC << 16) | ((DmxId << 8) & 0x0000ff00) | (KeyId & 0x000000ff))
#define DMX_CHECK_KEYHANDLE(KeyHandle)                                  \
    do                                                                  \
    {                                                                   \
        if (((KeyHandle >> 24) & 0x000000ff) != HI_ID_DEMUX || ((KeyHandle >> 16) & 0x000000ff) != DMX_KEYHANDLE_MAGIC) \
        {                                                               \
            HI_ERR_DEMUX("Invalid Key Handle 0x%x\n", KeyHandle);          \
            return HI_ERR_DMX_INVALID_PARA;                             \
        }                                                               \
    } while (0)

HI_S32 HI_MPI_DMX_CreateDescrambler(HI_U32 u32DmxId, const HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstDesramblerAttr, HI_HANDLE *phKey);
HI_S32 HI_MPI_DMX_DestroyDescrambler(HI_HANDLE hKey);
HI_S32 HI_MPI_DMX_SetDescramblerEvenKey(HI_HANDLE hKey, const HI_U8 *pu8EvenKey);
HI_S32 HI_MPI_DMX_SetDescramblerOddKey(HI_HANDLE hKey, const HI_U8 *pu8OddKey);
HI_S32 HI_MPI_DMX_SetDescramblerEvenIVKey(HI_HANDLE hKey, const HI_U8 *pu8IVKey);
HI_S32 HI_MPI_DMX_SetDescramblerOddIVKey(HI_HANDLE hKey, const HI_U8 *pu8IVKey);
HI_S32 HI_MPI_DMX_AttachDescrambler(HI_HANDLE hKey, HI_HANDLE hChannel);
HI_S32 HI_MPI_DMX_DetachDescrambler(HI_HANDLE hKey, HI_HANDLE hChannel);
HI_S32 HI_MPI_DMX_GetDescramblerKeyHandle(HI_HANDLE hChannel, HI_HANDLE *phKey);
HI_S32 HI_MPI_DMX_GetFreeDescramblerKeyCount(HI_U32 u32DmxId, HI_U32 *pu32FreeCount);
HI_S32 HI_MPI_DMX_GetDescramblerAttr(HI_HANDLE hKey, HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstAttr);
HI_S32 HI_MPI_DMX_SetDescramblerAttr(HI_HANDLE hKey, HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstAttr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __HI_MPI_DESCRAMBLER_H__ */

