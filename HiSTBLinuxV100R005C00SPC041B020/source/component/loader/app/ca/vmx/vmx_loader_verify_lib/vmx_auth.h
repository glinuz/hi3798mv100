/******************************************************************************
Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : vmx_auth.h
Version       : Initial Draft
Author        : Hisilicon hisecurity team
Created       : 2014/09/24
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __VMX_AUTH_H__
#define __VMX_AUTH_H__

int verifySignature( unsigned char* signature,
                     unsigned char* src,
                     unsigned char* tmp,
                     unsigned int len,
                     unsigned int maxLen,
                     unsigned char mode,
                     unsigned char* errorCode );

#endif /* __VMX_AUTH_H__ */
