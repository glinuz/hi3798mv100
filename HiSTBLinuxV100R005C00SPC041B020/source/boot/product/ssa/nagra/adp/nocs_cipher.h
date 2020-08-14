/******************************************************************************
Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : nocs_cipher.h
Version       : Initial Draft
Author        : Hisilicon hisecurity group
Created       : 2013/12/26
Last Modified :
Description   : Header file of nocs_cipher.c
Function List :
History       :
******************************************************************************/

#ifndef CIPHER_H
#define CIPHER_H

#include "hi_type.h"
//#include "drv_cipher_define.h"
#include "hi_unf_cipher.h"

HI_S32 BSD_CIPHER_Init(HI_VOID);
HI_VOID BSD_CIPHER_Exit(HI_VOID);
HI_S32 BSD_CIPHER_CreateHandle(HI_HANDLE* phCipher);
HI_S32 BSD_CIPHER_DestroyHandle(HI_HANDLE hCipher);
HI_S32 BSD_CIPHER_ConfigHandle(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl);
HI_S32 BSD_CIPHER_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength);
HI_S32 BSD_CIPHER_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength);

#endif
