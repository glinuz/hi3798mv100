/******************************************************************************
Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : nocs1x_cipher.c
Version       : Initial Draft
Author        : Hisilicon hisecurity group
Created       : 2013/12/26
Last Modified :
Description   : Implementation of CIPHER functions for BSD API
Function List :
History       :
******************************************************************************/
#include "nocs_cipher.h"
//#include "drv_cipher_intf.h"
#include "hi_unf_cipher.h"
#ifndef HI_MINIBOOT_SUPPORT
#include "linux/mtd/compat.h"
#endif

HI_S32 BSD_CIPHER_Init(HI_VOID)
{
    //return Cipher_Init();
    return HI_UNF_CIPHER_Init();
}

HI_VOID BSD_CIPHER_Exit(HI_VOID)
{
    //Cipher_Exit();
    return HI_UNF_CIPHER_DeInit();
}

HI_S32 BSD_CIPHER_CreateHandle(HI_HANDLE* phCipher)
{
    HI_UNF_CIPHER_ATTS_S stCipherAtts;
    
    memset(&stCipherAtts, 0, sizeof(HI_UNF_CIPHER_ATTS_S));
    stCipherAtts.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    //return Cipher_CreateHandle(phCipher, &stCipherAtts);
    return HI_UNF_CIPHER_CreateHandle(phCipher, &stCipherAtts);
}

HI_S32 BSD_CIPHER_DestroyHandle(HI_HANDLE hCipher)
{        
    //return Cipher_DestroyHandle(hCipher);
    return HI_UNF_CIPHER_DestroyHandle(hCipher);
}

HI_S32 BSD_CIPHER_ConfigHandle(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl)
{               
    //return Cipher_ConfigHandle(hCipher,pstCtrl);
    return HI_UNF_CIPHER_ConfigHandle(hCipher,pstCtrl);
}

HI_S32 BSD_CIPHER_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{   
    //return Cipher_Encrypt(hCipher,u32SrcPhyAddr,u32DestPhyAddr,u32ByteLength);
    return HI_UNF_CIPHER_Encrypt(hCipher,u32SrcPhyAddr,u32DestPhyAddr,u32ByteLength);
}

HI_S32 BSD_CIPHER_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{ 
    //return Cipher_Decrypt(hCipher,u32SrcPhyAddr,u32DestPhyAddr,u32ByteLength);
    return HI_UNF_CIPHER_Decrypt(hCipher,u32SrcPhyAddr,u32DestPhyAddr,u32ByteLength);
}

