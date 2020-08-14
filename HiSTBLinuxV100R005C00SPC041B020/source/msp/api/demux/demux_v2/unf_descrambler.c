/******************************************************************************
 Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : unf_descrambler.c
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 2013/04/16
 Description   :
******************************************************************************/

#include <stdio.h>

#include "hi_type.h"
#include "hi_debug.h"

#include "demux_debug.h"
#include "hi_mpi_descrambler.h"
#include "hi_unf_descrambler.h"

HI_S32 HI_UNF_DMX_CreateDescrambler(HI_U32 u32DmxId, HI_HANDLE *phKey)
{
    HI_UNF_DMX_DESCRAMBLER_ATTR_S stAttr;

    stAttr.enCaType = HI_UNF_DMX_CA_NORMAL;
    stAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_CSA2;
    stAttr.enEntropyReduction = HI_UNF_DMX_CA_ENTROPY_REDUCTION_OPEN;

    return HI_MPI_DMX_CreateDescrambler(u32DmxId, &stAttr, phKey);
}

HI_S32 HI_UNF_DMX_CreateDescramblerExt(HI_U32 u32DmxId, const HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstDesramblerAttr, HI_HANDLE *phKey)
{
    return HI_MPI_DMX_CreateDescrambler(u32DmxId, pstDesramblerAttr, phKey);
}

HI_S32 HI_UNF_DMX_DestroyDescrambler(HI_HANDLE hKey)
{
    return HI_MPI_DMX_DestroyDescrambler(hKey);
}

HI_S32 HI_UNF_DMX_GetDescramblerAttr(HI_HANDLE hKey, HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstAttr)
{
	return HI_MPI_DMX_GetDescramblerAttr(hKey, pstAttr);
}

HI_S32 HI_UNF_DMX_SetDescramblerAttr(HI_HANDLE hKey, HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstAttr)
{
	return HI_MPI_DMX_SetDescramblerAttr(hKey, pstAttr);
}

HI_S32 HI_UNF_DMX_SetDescramblerEvenKey(HI_HANDLE hKey, const HI_U8 *pu8EvenKey)
{
    return HI_MPI_DMX_SetDescramblerEvenKey(hKey, pu8EvenKey);
}

HI_S32 HI_UNF_DMX_SetDescramblerOddKey(HI_HANDLE hKey, const HI_U8 *pu8OddKey)
{
    return HI_MPI_DMX_SetDescramblerOddKey(hKey, pu8OddKey);
}

HI_S32 HI_UNF_DMX_SetDescramblerEvenIVKey(HI_HANDLE hKey, const HI_U8 *pu8IVKey)
{
    return HI_MPI_DMX_SetDescramblerEvenIVKey(hKey, pu8IVKey);
}

HI_S32 HI_UNF_DMX_SetDescramblerOddIVKey(HI_HANDLE hKey, const HI_U8 *pu8IVKey)
{
    return HI_MPI_DMX_SetDescramblerOddIVKey(hKey, pu8IVKey);
}

HI_S32 HI_UNF_DMX_AttachDescrambler(HI_HANDLE hKey, HI_HANDLE hChannel)
{
    return HI_MPI_DMX_AttachDescrambler(hKey, hChannel);
}

HI_S32 HI_UNF_DMX_DetachDescrambler(HI_HANDLE hKey, HI_HANDLE hChannel)
{
    return HI_MPI_DMX_DetachDescrambler(hKey, hChannel);
}

HI_S32 HI_UNF_DMX_GetDescramblerKeyHandle(HI_HANDLE hChannel, HI_HANDLE *phKey)
{
    return HI_MPI_DMX_GetDescramblerKeyHandle(hChannel, phKey);
}

HI_S32 HI_UNF_DMX_GetFreeDescramblerKeyCount(HI_U32 u32DmxId, HI_U32 *pu32FreeCount)
{
    return HI_MPI_DMX_GetFreeDescramblerKeyCount(u32DmxId, pu32FreeCount);
}
