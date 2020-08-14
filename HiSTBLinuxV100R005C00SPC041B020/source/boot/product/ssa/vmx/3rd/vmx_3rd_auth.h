/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : vmx_3rd_auth.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2015-12-10
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __VMX_3RD_AUTH_H__
#define __VMX_3RD_AUTH_H__

typedef struct
{
    unsigned int u32ImageLen;
    struct
    {
        unsigned char enc_flag     :1;
        unsigned char reserved     :7;
	}enc_ctrl;
	unsigned char au8Reserved[11];
}VMX_APPLICATION_HEADER_S;

void VMX3RD_SetEncFlag(unsigned char bEncFlag);

#endif /* __VMX_3RD_AUTH_H__ */


