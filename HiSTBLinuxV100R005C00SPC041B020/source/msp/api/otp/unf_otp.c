/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_otp.c
  Version       : Initial Draft
  Author        : x57522
  Created       : 2010/12/22
  Last Modified :
  Description   : otp interface
  Function List :
  History       :
  1.Date        : 2010/12/22
    Author      : x57522
    Modification: Created file

  2.Date        : 2012/02/29
     Author     : c00209458
     Modification: Move the source code to OTP MPI interface
  3.Date        : 2012/08/29
     Author      : z00213260
     Modification: Add new interface.
******************************************************************************/
#include "hi_debug.h"
#include "hi_mpi_otp.h"

HI_S32 HI_UNF_OTP_Init(HI_VOID)
{
    return HI_MPI_OTP_Open();
}

HI_S32 HI_UNF_OTP_DeInit(HI_VOID)
{
    return HI_MPI_OTP_Close();
}

HI_S32 HI_UNF_OTP_SetCustomerKey(HI_U8 *pKey, HI_U32 u32KeyLen)
{
    return HI_MPI_OTP_SetCustomerKey(pKey, u32KeyLen);
}

HI_S32 HI_UNF_OTP_GetCustomerKey(HI_U8 *pu8Key, HI_U32 u32KeyLen)
{
    return HI_MPI_OTP_GetCustomerKey(pu8Key, u32KeyLen);    
}

HI_S32 HI_UNF_OTP_SetStbPrivData(HI_U32 u32Offset, HI_U8 u8Data)
{   
    return HI_MPI_OTP_SetStbPrivData(u32Offset, u8Data);
}

HI_S32 HI_UNF_OTP_GetStbPrivData(HI_U32 u32Offset, HI_U8 *pu8Data)
{
    return HI_MPI_OTP_GetStbPrivData(u32Offset, pu8Data);
}

HI_S32 HI_UNF_OTP_WriteHdcpRootKey(HI_U8 *pu8HdcpRootKey, HI_U32 u32Keylen)
{
    return HI_MPI_OTP_WriteHdcpRootKey(pu8HdcpRootKey, u32Keylen);
}

HI_S32 HI_UNF_OTP_ReadHdcpRootKey(HI_U8 *pu8HdcpRootKey, HI_U32 u32Keylen)
{
    return HI_MPI_OTP_ReadHdcpRootKey(pu8HdcpRootKey, u32Keylen);
}

HI_S32 HI_UNF_OTP_LockHdcpRootKey(HI_VOID)
{
    return HI_MPI_OTP_LockHdcpRootKey();
}

HI_S32 HI_UNF_OTP_GetHdcpRootKeyLockFlag(HI_BOOL *pbLockFlag)
{
    return HI_MPI_OTP_GetHdcpRootKeyLockFlag(pbLockFlag);
}

HI_S32 HI_UNF_OTP_WriteStbRootKey(HI_U8 *pu8StbRootKey, HI_U32 u32Keylen)
{
    return HI_MPI_OTP_WriteStbRootKey(pu8StbRootKey, u32Keylen);
}

HI_S32 HI_UNF_OTP_ReadStbRootKey(HI_U8 *pu8StbRootKey, HI_U32 u32Keylen)
{
    return HI_MPI_OTP_ReadStbRootKey(pu8StbRootKey, u32Keylen);
}

HI_S32 HI_UNF_OTP_LockStbRootKey(HI_VOID)
{
    return HI_MPI_OTP_LockStbRootKey();
}

HI_S32 HI_UNF_OTP_GetStbRootKeyLockFlag(HI_BOOL *pbLockFlag)
{
    return HI_MPI_OTP_GetStbRootKeyLockFlag(pbLockFlag);
}

HI_S32 HI_UNF_OTP_GetIDWordLockFlag(HI_BOOL *pbLockFlag)
{
    return HI_MPI_OTP_GetIDWordLockFlag(pbLockFlag);
}

HI_S32 HI_UNF_OTP_BurnToNormalChipset(HI_VOID)
{
    return HI_MPI_OTP_BurnToNormalChipset();    
}

HI_S32 HI_UNF_OTP_BurnToSecureChipset(HI_VOID)
{
    return HI_MPI_OTP_BurnToSecureChipset();    
}
