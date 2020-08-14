/******************************************************************************
 Copyright (C), 2001-2012, Hisilicon Tech. Co., Ltd.
******************************************************************************
 File Name     : mpi_otp.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       :
 Last Modified :
 Description   :
 Function List :
 History       :
******************************************************************************/
#ifndef __HI_MPI_OTP_H__
#define __HI_MPI_OTP_H__

#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define HI_MPI_OTP_Open(HI_VOID) HI_MPI_OTP_Init(HI_VOID)
#define HI_MPI_OTP_Close(HI_VOID) HI_MPI_OTP_DeInit(HI_VOID)

HI_S32 HI_MPI_OTP_Init(HI_VOID);
HI_S32 HI_MPI_OTP_DeInit(HI_VOID);
HI_S32 HI_MPI_OTP_SetCustomerKey(HI_U8 *pu8Key, HI_U32 u32KeyLen);
HI_S32 HI_MPI_OTP_GetCustomerKey(HI_U8 *pu8Key, HI_U32 u32KeyLen);
HI_S32 HI_MPI_OTP_IsDDPLUSSupport(HI_BOOL* pbDDPLUSFlag);
HI_S32 HI_MPI_OTP_IsDTSSupport(HI_BOOL* pbDTSFlag);
HI_S32 HI_MPI_OTP_SetStbPrivData(HI_U32 u32Offset, HI_U8 u8Data);
HI_S32 HI_MPI_OTP_GetStbPrivData(HI_U32 u32Offset, HI_U8 *pu8Data);
HI_S32 HI_MPI_OTP_WriteHdcpRootKey(HI_U8 *pu8Key, HI_U32 u32KeyLen);
HI_S32 HI_MPI_OTP_ReadHdcpRootKey(HI_U8 *pu8Key, HI_U32 u32KeyLen);
HI_S32 HI_MPI_OTP_LockHdcpRootKey(HI_VOID);
HI_S32 HI_MPI_OTP_GetHdcpRootKeyLockFlag(HI_BOOL *pbKeyLockFlag);
HI_S32 HI_MPI_OTP_WriteStbRootKey(HI_U8 *pu8Key, HI_U32 u32KeyLen);
HI_S32 HI_MPI_OTP_ReadStbRootKey(HI_U8 *pu8Key, HI_U32 u32KeyLen);
HI_S32 HI_MPI_OTP_LockStbRootKey(HI_VOID);
HI_S32 HI_MPI_OTP_GetStbRootKeyLockFlag(HI_BOOL *pbKeyLockFlag);
HI_S32 HI_MPI_OTP_BurnToNormalChipset(HI_VOID);
HI_S32 HI_MPI_OTP_GetIDWordLockFlag(HI_BOOL *pbLockFlag);
HI_S32 HI_MPI_OTP_LockIDWord(HI_VOID);
HI_S32 HI_MPI_OTP_BurnToSecureChipset(HI_VOID);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_MPI_OTP_H__ */

