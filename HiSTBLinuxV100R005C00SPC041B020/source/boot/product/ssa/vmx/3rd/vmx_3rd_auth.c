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
#ifdef LOADER
#include <stdio.h>
#include <string.h>
#endif

#include "hi_debug.h"

#ifdef HI_MINIBOOT_SUPPORT
#include "app.h"
#endif

#ifdef HI_UBOOT_SUPPORT
#include "exports.h"
#endif

#include "vmx_3rd_adp.h"
#include "vmx_3rd_auth.h"

#define VMX_PRINT(fmt...)   HI_INFO_PRINT(HI_ID_CA, fmt)

//#define __TIME_TEST__  only for debugging
#if defined(__TIME_TEST__) && defined(LOADER)
#include <sys/time.h>
#endif

static int g_enc_flag = 0;

static int VMX_PrintBuffer(const char *string, unsigned char *pu8Input, unsigned int u32Length)
{

    unsigned int i = 0;

    if ( 0 != string )
    {
        HI_PRINT("%s\n", string);
    }

    for ( i = 0 ; i < u32Length; i++ )
    {
        if( (i % 16 == 0) && (i != 0)) HI_PRINT("\n");
        HI_PRINT("%02x ", pu8Input[i]);
    }
    HI_PRINT("\n");

    return 0;
}

#if defined(__TIME_TEST__) && defined(LOADER)
static void printDiffTimeUs(struct timeval oldtime)
{
    unsigned int timeuse_us = 0;
    unsigned int timeuse_old_us = 0;
    struct timeval newtime;

    timeuse_old_us = 1000000*oldtime.tv_sec + oldtime.tv_usec;

    gettimeofday(&newtime, 0); //
    timeuse_us = 1000000*newtime.tv_sec + newtime.tv_usec;

    VMX_PRINT(" ************************Time use :%d us***************\n",timeuse_us - timeuse_old_us);

    return;
}
#endif

void VMX3RD_SetEncFlag(unsigned char bEncFlag)
{
	g_enc_flag = bEncFlag;
}

int VMX3RD_GetEncFlag(unsigned char *pbEncFlag)
{
	if(0 == (unsigned int)pbEncFlag)
	{
		return -1;
	}

	*pbEncFlag = g_enc_flag;
	return 0;
}

int verifySignature( unsigned char* signature,
					unsigned char* src,
					unsigned char* tmp,
					unsigned int len,
					unsigned int maxLen,
					unsigned char mode,
					unsigned char *errorCode )
{
	int ret = 0;
	unsigned char enc_flag = 0;
#if defined(__TIME_TEST__) && defined(LOADER)
	struct timeval oldtime;
#endif

	if( (0 == signature) || (0 == src) || (0 == tmp) || (0 == errorCode) || (0 == len) || (0xFFFFFFFF == len) || (0xFFFFFFFF == maxLen) )
	{
		VMX_PRINT("Invalid parameters!\n");
		*errorCode = 1;
		return 0;
	}

	if(maxLen < len)
	{
		VMX_PRINT("Invalid parameter, len:0x%x > maxLen:0x%x!\n", len, maxLen);
		*errorCode = 1;
		return 0;
	}

	*errorCode = 0;

	ret = VMX3RD_GetEncFlag(&enc_flag);
	if(0 != ret)
	{
		VMX_PRINT("Get VMX image Enc flag failed!\n");
		*errorCode = 1;
		return 0;
	}
	VMX_PRINT("enc_flag :0X%X\n", enc_flag);

#ifndef LOADER
	/* decrypt using MISC keyladder */
	if(1 == enc_flag)
	{
#if defined(__TIME_TEST__) && defined(LOADER)
		VMX_PRINT("Time use, before VMX3RD_ADP_R2RByMiscKeyladder\n");
		gettimeofday(&oldtime, 0);
#endif
		ret = VMX3RD_ADP_R2RByMiscKeyladder(1, src, len, tmp);
		if(0 != ret)
		{
			VMX_PRINT("Decrypt src using MISC keyladder failed!\n");
			*errorCode = 1;
			return 0;
		}
#if defined(__TIME_TEST__) && defined(LOADER)
		VMX_PRINT("Time use, after VMX3RD_ADP_R2RByMiscKeyladder\n");
		printDiffTimeUs(oldtime);
#endif
		VMX_PRINT("VMX3RD_ADP_R2RByMiscKeyladder(1) success\n");
	}

	/* verify signature */
	ret = VMX3RD_ADP_VerifyRSASignature(signature, 256, tmp, len);
	if(0 == ret)
	{
		VMX_PRINT("Verify RSA signature success!\n");
#if defined(__TIME_TEST__) && defined(LOADER)
		VMX_PRINT("Time use, before memcpy\n");
		gettimeofday(&oldtime, 0);
#endif
		memcpy(src, tmp, len);
#if defined(__TIME_TEST__) && defined(LOADER)
		VMX_PRINT("Time use, after memcpy\n");
		printDiffTimeUs(oldtime);
#endif
		return 1;
	}
#if defined(__TIME_TEST__) && defined(LOADER)
	VMX_PRINT("Time use, after VMX3RD_ADP_VerifyRSASignature\n");
	printDiffTimeUs(oldtime);
#endif
	VMX_PRINT("[WARNING]VMX3RD_ADP_VerifyRSASignature failed, enc_flag:0x%x!\n", enc_flag);

	if(0 == enc_flag)
	{
		*errorCode = 1;
		return 0;
	}
#endif

	/* decrypt using SWPK by */
	VMX_PrintBuffer("before decrypt by swpk, src(start):", src, 256);
	VMX_PrintBuffer("before decrypt by swpk, src(end):", src+len-256, 256);

#if defined(__TIME_TEST__) && defined(LOADER)
	VMX_PRINT("Time use, before VMX3RD_ADP_DecryptBySWPK\n");
	gettimeofday(&oldtime, 0);
#endif	
	ret = VMX3RD_ADP_DecryptBySWPK(src, len, src);
	if(0 != ret)
	{
		VMX_PRINT("Decrypt src using SWPK failed!\n");
		*errorCode = 1;
		return 0;
	}
#if defined(__TIME_TEST__) && defined(LOADER)
	VMX_PRINT("Time use, after VMX3RD_ADP_DecryptBySWPK\n");
	printDiffTimeUs(oldtime);
#endif
	VMX_PRINT("VMX3RD_ADP_DecryptBySWPK success, len:0X%X!\n", len);

	/* verify signature */
	VMX_PrintBuffer("before verify rsa, signature:", signature, 256);
	VMX_PrintBuffer("before verify rsa, src(start):", src, 256);
	VMX_PrintBuffer("before verify rsa, src(end):", src+len-256, 256);

#if defined(__TIME_TEST__) && defined(LOADER)
	VMX_PRINT("Time use, before VMX3RD_ADP_VerifyRSASignature\n");
	gettimeofday(&oldtime, 0);
#endif
	ret = VMX3RD_ADP_VerifyRSASignature(signature, 256, src, len);
	if(0 != ret)
	{
		VMX_PRINT("Verify RSA signature failed!\n");
		*errorCode = 1;
		return 0;
	}
#if defined(__TIME_TEST__) && defined(LOADER)
	VMX_PRINT("Time use, after VMX3RD_ADP_VerifyRSASignature\n");
	printDiffTimeUs(oldtime);
#endif

	VMX_PrintBuffer("after verify rsa, signature:", signature, 256);
	VMX_PrintBuffer("after verify rsa, src(start):", src, 256);
	VMX_PrintBuffer("after verify rsa, src(end):", src+len-256, 256);

	/* decrypt using MISC keyladder */
#if defined(__TIME_TEST__) && defined(LOADER)
	VMX_PRINT("Time use, before VMX3RD_ADP_R2RByMiscKeyladder(1)\n");
	gettimeofday(&oldtime, 0);
#endif

#ifdef LOADER
	ret = VMX3RD_ADP_R2RByMiscKeyladder(0, src, len, src);	//for loader or recovery, encrypt to the original src addr for the re-encryption
#else
	ret = VMX3RD_ADP_R2RByMiscKeyladder(0, src, len, tmp);  //for boot, encrypt to the tmp addr, to support the first startup of the bootargs partition, as the bootargs needs to be both started and re-encrypted.
#endif

#if defined(__TIME_TEST__) && defined(LOADER)
	VMX_PRINT("Time use, after VMX3RD_ADP_R2RByMiscKeyladder(1)\n");
	printDiffTimeUs(oldtime);
#endif

	if(0 != ret)
	{
		VMX_PRINT("Decrypt src using MISC keyladder failed!\n");
		*errorCode = 1;
		return 0;
	}

	*errorCode = 2;
	return 0;
}
