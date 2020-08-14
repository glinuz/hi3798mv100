/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : cmd_cipher.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
//#include <stdlib.h>
#include "hi_type.h"
#include "drv_cipher_intf.h"
#include "hi_unf_cipher.h"
#ifndef HI_MINIBOOT_SUPPORT
#include <common.h>
#else
#include "string.h"
#include "stdio.h"
#endif

#define CIPHER_CMD_TEST

#ifndef HI_ADVCA_FUNCTION_RELEASE

#if defined(CIPHER_CMD_TEST) && !defined(HI_MINIBOOT_SUPPORT)
static HI_S32 Cipher_printBuffer(const HI_CHAR *string, const HI_U8 *pu8Input, HI_U32 u32Length)
{
    HI_U32 i = 0;
    
    if ( NULL != string )
    {
        printf("%s\n", string);
    }

    for ( i = 0 ; i < u32Length; i++ )
    {
        if( (i % 16 == 0) && (i != 0)) printf("\n");
        printf("0x%02x ", pu8Input[i]);
    }
    printf("\n");

    return HI_SUCCESS;
}

HI_U8 u8HMACSha1Key[16] = {0x7e, 0xe0, 0x1e, 0x27, 0x94, 0x95, 0x29, 0x65, 0x14, 0xed, 0x4d, 0x39, 0x5a, 0x60, 0x6c, 0x62};
HI_U8 u8HMACSha2Key[16] = {0x16, 0x37, 0xa4, 0x54, 0x64, 0xda, 0xe9, 0x19, 0x03, 0x69, 0xe4, 0x1f, 0xea, 0xa9, 0xfa, 0x0a};


static unsigned char sha1_buf[3][57] = {
    {"abc"},
    {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"},
    {""}
};
static const int sha1_buflen[3] ={3, 56, 1024};
static const unsigned char sha1_sum[3][20] ={
    {0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E,
     0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D},
    {0x84, 0x98, 0x3E, 0x44, 0x1C, 0x3B, 0xD2, 0x6E, 0xBA, 0xAE,
     0x4A, 0xA1, 0xF9, 0x51, 0x29, 0xE5, 0xE5, 0x46, 0x70, 0xF1},
    {0x8e, 0xca, 0x55, 0x46, 0x31, 0xdf, 0x9e, 0xad, 0x14, 0x51,
     0x0e, 0x1a, 0x70, 0xae, 0x48, 0xc7, 0x0f, 0x9b, 0x93, 0x84}
};

static const unsigned char hmac_sha1_sum[3][20] ={
    {0x2c, 0x70, 0x65, 0xf0, 0xbb, 0xfa, 0x67, 0x39, 0x91, 0x6b,
     0x63, 0xe8, 0x33, 0xd3, 0x80, 0x91, 0xa2, 0x6a, 0x4c, 0x83},
    {0xed, 0xc8, 0x02, 0x3a, 0x75, 0x1a, 0xa5, 0x87, 0xe7, 0xff,
     0x04, 0x81, 0x93, 0x5d, 0x02, 0x1f, 0xf4, 0x06, 0x09, 0x4a},
    {0x9e, 0x7d, 0x9a, 0x09, 0x2b, 0x47, 0x81, 0x6b, 0x27, 0xef, 
     0xad, 0x19, 0xbd, 0x49, 0x00, 0xce, 0xff, 0x71, 0x9b, 0x99}
};

/*static unsigned char sha2_buf[3][57] = {
    {"abc"},
    {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"},
    {""}
};*/
static const int sha2_buflen[3] = {3, 56, 1024};
static const unsigned char sha2_sum[6][32] ={
    /** SHA-256 sample vectors*/
    {0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23,
     0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD},
    {0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
     0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1},
    {0x2e, 0xdc, 0x98, 0x68, 0x47, 0xe2, 0x09, 0xb4, 0x01, 0x6e, 0x14, 0x1a, 0x6d, 0xc8, 0x71, 0x6d, 
     0x32, 0x07, 0x35, 0x0f, 0x41, 0x69, 0x69, 0x38, 0x2d, 0x43, 0x15, 0x39, 0xbf, 0x29, 0x2e, 0x4a }
};

static const unsigned char hmac_sha2_sum[3][32] ={
    {0xd3, 0x8b, 0x88, 0x1e, 0x79, 0xe5, 0x1f, 0xb4, 0xd8, 0xee, 0x47, 0xe5, 0xe8, 0x91, 0x10, 0xf7, 
     0x3d, 0xa9, 0x7d, 0xd1, 0x9f, 0x6b, 0xd6, 0x14, 0x8a, 0x19, 0x4c, 0xf9, 0x90, 0x7a, 0x68, 0xb1},
    {0xc8, 0xd7, 0x55, 0xa5, 0xce, 0x90, 0xef, 0xe5, 0xb9, 0x56, 0x99, 0xf5, 0x32, 0x29, 0x10, 0x2d, 
     0x3c, 0xc6, 0x56, 0x4b, 0x61, 0xf4, 0x0d, 0xda, 0xf0, 0xdc, 0x66, 0xa8, 0x1e, 0x81, 0xab, 0xe0},
    {0xfc, 0xcb, 0x8f, 0x53, 0xa2, 0xee, 0x0c, 0xd1, 0x9b, 0x04, 0x23, 0xc5, 0x37, 0x64, 0x76, 0x7f, 
     0x02, 0x7c, 0x03, 0xe1, 0x37, 0xb6, 0x37, 0xb3, 0x85, 0x22, 0x5b, 0x1b, 0x12, 0x1d, 0xee, 0xe2}
};

HI_S32 SHA1(HI_VOID)
{
    HI_U8 u8Hash[20];
    HI_U32 i;
    HI_U8 u8Buf[1024];
    HI_U32 buflen;
    HI_HANDLE hHandle;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr ={0};

	HI_UNF_CIPHER_Init();

    memset(u8Hash, 0, 20);

    for(i = 0; i < 3; i++)
    {
        if(i == 2)
        {
            memset(u8Buf, 'a', buflen = 1024);
            stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_SHA1;
            stHashAttr.u32TotalDataLen = buflen;
            HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle);
            HI_UNF_CIPHER_HashUpdate(hHandle, u8Buf, 1024);
            HI_UNF_CIPHER_HashFinal(hHandle, u8Hash);
        }
        else
        {
            stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_SHA1;
            stHashAttr.u32TotalDataLen = sha1_buflen[i];
            HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle);
            HI_UNF_CIPHER_HashUpdate(hHandle, sha1_buf[i], sha1_buflen[i]);
            HI_UNF_CIPHER_HashFinal(hHandle, u8Hash);
        }

        Cipher_printBuffer("Sha1 result:", u8Hash, 20);

        if(memcmp(u8Hash, sha1_sum[i], 20) != 0)
        {
            HI_ERR_CIPHER("\033[0;31m" "SHA1 run failed, sample %d!\n" "\033[0m", i);
            Cipher_printBuffer("golden data:", sha1_sum[i], 20);
            return HI_FAILURE;
        }
    }

    HI_ERR_CIPHER("sample SHA1 run successfully!\n");
    HI_UNF_CIPHER_DeInit();

    return HI_SUCCESS;
}

HI_S32 SHA256(HI_VOID)
{
    HI_U8 u8Hash[32];
    HI_U32 i;
    HI_U8 u8Buf[1024];
    HI_U32 buflen;
    HI_HANDLE hHandle;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr = {0};

	HI_UNF_CIPHER_Init();

    for(i = 0; i < 3; i++)
    {
        if(i == 2)
        {
            memset(u8Buf, 'a', buflen = 1024);

            stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_SHA256;
            stHashAttr.u32TotalDataLen = buflen;
            HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle);
            HI_UNF_CIPHER_HashUpdate(hHandle, u8Buf, 1024);
            HI_UNF_CIPHER_HashFinal(hHandle, u8Hash);
        }
        else
        {
            stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_SHA256;
            stHashAttr.u32TotalDataLen = sha1_buflen[i];
            HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle);
            HI_UNF_CIPHER_HashUpdate(hHandle, sha1_buf[i], sha1_buflen[i]);
            HI_UNF_CIPHER_HashFinal(hHandle, u8Hash);
        }

        Cipher_printBuffer("Sha256 result:", u8Hash, 32);

        if(memcmp(u8Hash, sha2_sum[i], 32) != 0)
        {
            HI_ERR_CIPHER("\033[0;31m" "SHA256 run failed, sample %d!\n" "\033[0m", i);
            Cipher_printBuffer("golden data:", sha2_sum[i], 32);
            return HI_FAILURE;
        }
    }

    HI_ERR_CIPHER("sample SHA256 run successfully!\n");
    HI_UNF_CIPHER_DeInit();
    return HI_SUCCESS;
}

HI_S32 HMAC_SHA1(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 u8Hash[20];
    HI_U32 i;
    HI_U8 u8Buf[1024];
    HI_U32 buflen;
    HI_HANDLE hHandle;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr = {0};

	HI_UNF_CIPHER_Init();

    memset(u8Hash, 0, 20);

    for(i = 0; i < 3; i++)
    {
        if(i == 2)
        {            
            memset(u8Buf, 'a', buflen = 1024);
            stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1;
            stHashAttr.u32TotalDataLen = buflen;
            memcpy(stHashAttr.u8HMACKey, u8HMACSha1Key, 16);
            ret = HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle);
            if( HI_SUCCESS != ret)
            {
                HI_ERR_CIPHER("HASH INIT FAILED!\n");
                return HI_FAILURE;
            }
            ret = HI_UNF_CIPHER_HashUpdate(hHandle, u8Buf, 1024);
            if( HI_SUCCESS != ret)
            {
                HI_ERR_CIPHER("HASH UPDATE FAILED!\n");
                ret = HI_FAILURE;
                goto EXIT;
            }
            
            ret = HI_UNF_CIPHER_HashFinal(hHandle, u8Hash);
            if( HI_SUCCESS != ret)
            {
                HI_ERR_CIPHER("HASH FINAL FAILED!\n");
                ret = HI_FAILURE;
                goto EXIT;
            }
        }
        else
        {
            stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1;
            stHashAttr.u32TotalDataLen = sha1_buflen[i];
            memcpy(stHashAttr.u8HMACKey, u8HMACSha1Key, 16);
            ret = HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle);
            if(HI_SUCCESS != ret)
            {
                HI_ERR_CIPHER("hash init failed!\n");
                ret = HI_FAILURE;
                goto EXIT;
            }
            
            ret = HI_UNF_CIPHER_HashUpdate(hHandle, sha1_buf[i], sha1_buflen[i]);
            if(HI_SUCCESS != ret)
            {
                HI_ERR_CIPHER("hash update failed!\n");
                ret = HI_FAILURE;
                goto EXIT;
            }
            
            ret = HI_UNF_CIPHER_HashFinal(hHandle, u8Hash);
            if(HI_SUCCESS != ret)
            {
                HI_ERR_CIPHER("hash final failed!\n");
                ret = HI_FAILURE;
                goto EXIT;
            }
        }

        Cipher_printBuffer("hmac-Sha1 result:", u8Hash, 20);

        if(memcmp(u8Hash, hmac_sha1_sum[i], 20) != 0)
        {
            HI_ERR_CIPHER("\033[0;31m" "HMAC SHA1 run failed, sample %d!\n" "\033[0m", i);
            Cipher_printBuffer("golden data:", hmac_sha1_sum[i], 20);
            return HI_FAILURE;
        }
    }

    HI_ERR_CIPHER("sample HMAC SHA1 run successfully!\n");

EXIT:
    HI_UNF_CIPHER_DeInit();

    return ret;
}


HI_S32 HMAC_SHA256(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;
    HI_U8 u8Hash[32];
    HI_U32 i;
    HI_U8 u8Buf[1024];
    HI_U32 buflen;
    HI_HANDLE hHandle;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr = {0};
    

	HI_UNF_CIPHER_Init();

    memset(u8Hash, 0, 32);

    for(i = 0; i < 3; i++)
    {
        if(i == 2)
        {            
            memset(u8Buf, 'a', buflen = 1024);
            stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256;
            stHashAttr.u32TotalDataLen = buflen;
            memcpy(stHashAttr.u8HMACKey, u8HMACSha2Key, 16);
            ret = HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle);
            if(HI_SUCCESS != ret)
            {
                HI_ERR_CIPHER("hash init failed!\n");
                goto EXIT;
            }
            
            ret = HI_UNF_CIPHER_HashUpdate(hHandle, u8Buf, 1024);
            if(HI_SUCCESS != ret)
            {
                HI_ERR_CIPHER("hash update failed!\n");
                goto EXIT;
            }
            
            ret = HI_UNF_CIPHER_HashFinal(hHandle, u8Hash);
            if(HI_SUCCESS != ret)
            {
                HI_ERR_CIPHER("hash final failed!\n");
                goto EXIT;
            }
        }
        else
        {
            stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256;
            stHashAttr.u32TotalDataLen = sha1_buflen[i];
            memcpy(stHashAttr.u8HMACKey, u8HMACSha2Key, 16);
            HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle);
            HI_UNF_CIPHER_HashUpdate(hHandle, sha1_buf[i], sha1_buflen[i]);
            HI_UNF_CIPHER_HashFinal(hHandle, u8Hash);
        }

        Cipher_printBuffer("hmac-Sha256 result:", u8Hash, 32);

        if(memcmp(u8Hash, hmac_sha2_sum[i], 32) != 0)
        {
            HI_ERR_CIPHER("\033[0;31m" "HMAC SHA256 run failed, sample %d!\n" "\033[0m", i);
            Cipher_printBuffer("golden data:", hmac_sha2_sum[i], 32);
            ret = HI_FAILURE;
            goto EXIT;
        }
    }

    HI_ERR_CIPHER("sample HMAC SHA256 run successfully!\n");

EXIT:
    HI_UNF_CIPHER_DeInit();

    return ret;
}

int test_hash(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    
    HI_S32 ret = HI_SUCCESS;
    
    if ( argc != 2)
    {
        HI_INFO_CIPHER("Usage: %s funcNumber\n\n", argv[0]);
        HI_INFO_CIPHER("#########hash sample##########\n"
                        "[0] SHA1\n"
                        "[1] SHA256\n"
                        "[2] HMAC-SHA1\n"
                        "[3] HMAC-SHA256\n");
        return HI_SUCCESS;
    }
    
	if (!strncmp(argv[1], "0", 1))
    {
        ret = SHA1();
    }
    else if (!strncmp(argv[1], "1", 1))
    {
        ret = SHA256();
    }
    else if (!strncmp(argv[1], "2", 1))
    {
        ret = HMAC_SHA1();
    }
    else if (!strncmp(argv[1], "3", 1))
    {
        ret = HMAC_SHA256();
    }
    else
    {      
        HI_INFO_CIPHER("funcNumber %s do not exist!\n",argv[1]);
    }

    if(HI_SUCCESS != ret)    
    {        
        HI_INFO_CIPHER("funcNumber %s run failure.\n",argv[1]);        
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

U_BOOT_CMD(
	hash_test,	3,	1,	test_hash,
	"hash_test [x]:[0] SHA1; [1] SHA256; [2] HMAC-SHA1; [3] HMAC-SHA256;",
	"Test the hash interface");

static HI_S32 Hash_SHA1_Calc(HI_U8 *pu8Buffer, HI_U32 u32Len)
{
	HI_S32 ret = 0;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;
    HI_U8 au8Hash[32] = {0};
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    HI_U32 index = 0;
    HI_U32 u32BlockSize = 1024 * 1024;
    HI_U32 round = 0;
    HI_U32 tail = 0;

	if(NULL == pu8Buffer || 0 == u32Len)
	{
		printf("Invalid parameters!\n");
		return HI_FAILURE;
	}

	memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));
    stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_SHA1;
    stHashAttr.u32TotalDataLen = u32Len;

    ret = HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle);
	if(HI_SUCCESS != ret)
	{
		printf("HI_UNF_CIPHER_HashInit failed, ret: 0x%x!\n", ret);
		return HI_FAILURE;
	}

	round = u32Len / u32BlockSize;
	for(index = 0; index < (round * u32BlockSize); index+=u32BlockSize)
	{
	    ret = HI_UNF_CIPHER_HashUpdate(hHandle, pu8Buffer + index, u32BlockSize);
		if(HI_SUCCESS != ret)
		{
			printf("HI_UNF_CIPHER_HashUpdate failed, ret: 0x%x!\n", ret);
			return HI_FAILURE;
		}
	}

	/* the last round */
	tail = u32Len % u32BlockSize;
	if(0 != tail)
	{
	    ret = HI_UNF_CIPHER_HashUpdate(hHandle, pu8Buffer + index, tail);
		if(HI_SUCCESS != ret)
		{
			printf("HI_UNF_CIPHER_HashUpdate failed, ret: 0x%x!\n", ret);
			return HI_FAILURE;
		}
	}

	ret = HI_UNF_CIPHER_HashFinal(hHandle, au8Hash);
	if(HI_SUCCESS != ret)
	{
		printf("HI_UNF_CIPHER_HashFinal failed, ret: 0x%x!\n", ret);
		return HI_FAILURE;
	}

	Cipher_printBuffer("SHA1 result:", au8Hash, 20);

	return HI_SUCCESS;
}

static HI_S32 Hash_SHA2_Calc(HI_U8 *pu8Buffer, HI_U32 u32Len)
{
	HI_S32 ret = 0;
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr ={0};
    HI_U8 au8Hash[32] = {0};
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    HI_U32 index = 0;
    HI_U32 u32BlockSize = 1024 * 1024;
    HI_U32 round = 0;
    HI_U32 tail = 0;

	if(NULL == pu8Buffer || 0 == u32Len)
	{
		printf("Invalid parameters!\n");
		return HI_FAILURE;
	}

	memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));
    stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_SHA256;
    stHashAttr.u32TotalDataLen = u32Len;

    ret = HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle);
	if(HI_SUCCESS != ret)
	{
		printf("HI_UNF_CIPHER_HashInit failed, ret: 0x%x!\n", ret);
		return HI_FAILURE;
	}

	round = u32Len / u32BlockSize;
	for(index = 0; index < (round * u32BlockSize); index+=u32BlockSize)
	{
	    ret = HI_UNF_CIPHER_HashUpdate(hHandle, pu8Buffer + index, u32BlockSize);
		if(HI_SUCCESS != ret)
		{
			printf("HI_UNF_CIPHER_HashUpdate failed, ret: 0x%x!\n", ret);
			return HI_FAILURE;
		}
	}

	/* the last round */
	tail = u32Len % u32BlockSize;
	if(0 != tail)
	{
	    ret = HI_UNF_CIPHER_HashUpdate(hHandle, pu8Buffer + index, tail);
		if(HI_SUCCESS != ret)
		{
			printf("HI_UNF_CIPHER_HashUpdate failed, ret: 0x%x!\n", ret);
			return HI_FAILURE;
		}
	}

	ret = HI_UNF_CIPHER_HashFinal(hHandle, au8Hash);
	if(HI_SUCCESS != ret)
	{
		printf("HI_UNF_CIPHER_HashFinal failed, ret: 0x%x!\n", ret);
		return HI_FAILURE;
	}

	Cipher_printBuffer("SHA2 result:", au8Hash, 32);

	return HI_SUCCESS;
}

static int Hash_Calc(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32Len = 0;
    HI_U32 u32Addr = 0;
    HI_U32 u32HashType = 0;
    //HI_UNF_CIPHER_HASH_ATTS_S stHashAttr ={0};

	if ( argc != 4)
    {
        HI_INFO_CIPHER("Usage: %s funcNumber addr len\n\n", argv[0]);
        HI_INFO_CIPHER("#########funcNumber##########\n"
                        "[0] SHA1\n"
                        "[1] SHA256\n");
        return HI_SUCCESS;
    }

	u32HashType = (HI_U32)simple_strtoul(argv[1], NULL, 0);
	u32Addr = (HI_U32)simple_strtoul(argv[2], NULL, 0);
	u32Len = (HI_U32)simple_strtoul(argv[3], NULL, 0);

	printf("u32HashType: 0x%x, Address: 0x%x, Length: 0x%x\n", u32HashType, u32Addr, u32Len);

	HI_UNF_CIPHER_Init();

	if (0 == u32HashType)
    {
        ret = Hash_SHA1_Calc((HI_U8*)u32Addr, u32Len);
    }
    else if (1 == u32HashType)
    {
        ret = Hash_SHA2_Calc((HI_U8*)u32Addr, u32Len);
    }
    else
    {
		printf("Failed, Hash type not supported: 0x%x!\n", u32HashType);
    }

    return ret;
}

U_BOOT_CMD(hash, 4, 1, Hash_Calc, "Calcluate hash", "for example: hash type address length  type:0:sha1, 1:sha2");

static unsigned char test_data[] = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
static unsigned char sha256_sum[32] =
{   
    0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39, 
    0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1,
};

static HI_U8 N[]  = 
{
    0x82, 0x78, 0xA0, 0xC5, 0x39, 0xE6, 0xF6, 0xA1, 0x5E, 0xD1, 0xC6, 0x8B, 0x9C, 0xF9, 0xC4, 0x3F, 
    0xEA, 0x19, 0x16, 0xB0, 0x96, 0x3A, 0xB0, 0x5A, 0x94, 0xED, 0x6A, 0xD3, 0x83, 0xE8, 0xA0, 0xFD, 
    0x01, 0x5E, 0x92, 0x2A, 0x7D, 0x0D, 0xF9, 0x72, 0x1E, 0x03, 0x8A, 0x68, 0x8B, 0x4D, 0x57, 0x55, 
    0xF5, 0x2F, 0x9A, 0xC9, 0x45, 0xCF, 0x9B, 0xB7, 0xF5, 0x11, 0x94, 0x7A, 0x16, 0x0B, 0xED, 0xD9, 
    0xA3, 0xF0, 0x63, 0x8A, 0xEC, 0xD3, 0x21, 0xAB, 0xCF, 0x74, 0xFC, 0x6B, 0xCE, 0x06, 0x4A, 0x51, 
    0xC9, 0x7C, 0x7C, 0xA3, 0xC4, 0x10, 0x63, 0x7B, 0x00, 0xEC, 0x2D, 0x02, 0x18, 0xD5, 0xF1, 0x8E, 
    0x19, 0x7F, 0xBE, 0xE2, 0x45, 0x5E, 0xD7, 0xA8, 0x95, 0x90, 0x88, 0xB0, 0x73, 0x35, 0x89, 0x66, 
    0x1C, 0x23, 0xB9, 0x6E, 0x88, 0xE0, 0x7A, 0x57, 0xB0, 0x55, 0x8B, 0x81, 0x9B, 0x9C, 0x34, 0x9F, 
    0x86, 0x0E, 0x15, 0x94, 0x2C, 0x6B, 0x12, 0xC3, 0xB9, 0x56, 0x60, 0x25, 0x59, 0x3E, 0x50, 0x7B, 
    0x62, 0x4A, 0xD0, 0xF0, 0xB6, 0xB1, 0x94, 0x83, 0x51, 0x66, 0x6F, 0x60, 0x4D, 0xEF, 0x8F, 0x94, 
    0xA6, 0xD1, 0xA2, 0x80, 0x06, 0x24, 0xF2, 0x6E, 0xD2, 0xC7, 0x01, 0x34, 0x8D, 0x2B, 0x6B, 0x03, 
    0xF7, 0x05, 0xA3, 0x99, 0xCC, 0xC5, 0x16, 0x75, 0x1A, 0x81, 0xC1, 0x67, 0xA0, 0x88, 0xE6, 0xE9, 
    0x00, 0xFA, 0x62, 0xAF, 0x2D, 0xA9, 0xFA, 0xC3, 0x30, 0x34, 0x98, 0x05, 0x4C, 0x1A, 0x81, 0x0C, 
    0x52, 0xCE, 0xBA, 0xD6, 0xEB, 0x9C, 0x1E, 0x76, 0x01, 0x41, 0x6C, 0x34, 0xFB, 0xC0, 0x83, 0xC5, 
    0x4E, 0xB3, 0xF2, 0x5B, 0x4F, 0x94, 0x08, 0x33, 0x87, 0x5E, 0xF8, 0x39, 0xEF, 0x7F, 0x72, 0x94, 
    0xFF, 0xD7, 0x51, 0xE8, 0xA2, 0x5E, 0x26, 0x25, 0x5F, 0xE9, 0xCC, 0x2A, 0x7D, 0xAC, 0x5B, 0x35
};
static HI_U8 E[]  = 
{
    0x01, 0x00, 0x01
};

static HI_U8 D[]  = 
{
    0x49, 0x7E, 0x93, 0xE9, 0xA5, 0x7D, 0x42, 0x0E, 0x92, 0xB0, 0x0E, 0x6C, 0x94, 0xC7, 0x69, 0x52, 
    0x2B, 0x97, 0x68, 0x5D, 0x9E, 0xB2, 0x7E, 0xA6, 0xF7, 0xDF, 0x69, 0x5E, 0xAE, 0x9E, 0x7B, 0x19, 
    0x2A, 0x0D, 0x50, 0xBE, 0xD8, 0x64, 0xE7, 0xCF, 0xED, 0xB2, 0x46, 0xE4, 0x2F, 0x1C, 0x29, 0x07, 
    0x45, 0xAF, 0x44, 0x3C, 0xFE, 0xB3, 0x3C, 0xDF, 0x7A, 0x10, 0x26, 0x18, 0x43, 0x95, 0x02, 0xAD, 
    0xA7, 0x98, 0x81, 0x2A, 0x3F, 0xCF, 0x8A, 0xD7, 0x12, 0x6C, 0xAE, 0xC8, 0x37, 0x6C, 0xF9, 0xAE, 
    0x6A, 0x96, 0x52, 0x4B, 0x99, 0xE5, 0x35, 0x74, 0x93, 0x87, 0x76, 0xAF, 0x08, 0xB8, 0x73, 0x72, 
    0x7D, 0x50, 0xA5, 0x81, 0x26, 0x5C, 0x8F, 0x94, 0xEA, 0x73, 0x59, 0x5C, 0x33, 0xF9, 0xC3, 0x65, 
    0x1E, 0x92, 0xCD, 0x20, 0xC3, 0xBF, 0xD7, 0x8A, 0xCF, 0xCC, 0xD0, 0x61, 0xF8, 0xFB, 0x1B, 0xF4, 
    0xB6, 0x0F, 0xD4, 0xCF, 0x3E, 0x55, 0x48, 0x4C, 0x99, 0x2D, 0x40, 0x44, 0x7C, 0xBA, 0x7B, 0x6F, 
    0xDB, 0x5D, 0x71, 0x91, 0x2D, 0x93, 0x80, 0x19, 0xE3, 0x26, 0x5D, 0x59, 0xBE, 0x46, 0x6D, 0x90, 
    0x4B, 0xDF, 0x72, 0xCE, 0x6C, 0x69, 0x72, 0x8F, 0x5B, 0xA4, 0x74, 0x50, 0x2A, 0x42, 0x95, 0xB2, 
    0x19, 0x04, 0x88, 0xD7, 0xDA, 0xBB, 0x17, 0x23, 0x69, 0xF4, 0x52, 0xEB, 0xC8, 0x55, 0xBE, 0xBC, 
    0x2E, 0xA9, 0xD0, 0x57, 0x7D, 0xC6, 0xC8, 0x8B, 0x86, 0x7B, 0x73, 0xCD, 0xE4, 0x32, 0x79, 0xC0, 
    0x75, 0x53, 0x53, 0xE7, 0x59, 0x38, 0x0A, 0x8C, 0xEC, 0x06, 0xA9, 0xFC, 0xA5, 0x15, 0x81, 0x61, 
    0x3E, 0x44, 0xCD, 0x05, 0xF8, 0x54, 0x04, 0x00, 0x79, 0xB2, 0x0D, 0x69, 0x2A, 0x47, 0x60, 0x1A, 
    0x2B, 0x79, 0x3D, 0x4B, 0x50, 0x8A, 0x31, 0x72, 0x48, 0xBB, 0x75, 0x78, 0xD6, 0x35, 0x90, 0xE1,
};

static HI_U8 RES[] =
{
    0x5c, 0xfd, 0x7c, 0xb3, 0x9f, 0xf1, 0x2b, 0xd0, 0x73, 0x23, 0x21, 0x4e, 0x25, 0x3d, 0x68, 0x5c,
    0x6c, 0x4b, 0x12, 0x77, 0x6b, 0x0e, 0x26, 0x80, 0x2a, 0xf4, 0xd2, 0x92, 0x66, 0x40, 0xe0, 0xb2,
    0x5f, 0xbe, 0x81, 0xa1, 0xda, 0xa4, 0xc5, 0x07, 0x96, 0x17, 0x4a, 0x12, 0x5f, 0xa4, 0x33, 0x43,
    0x3f, 0x94, 0x7e, 0xe7, 0xbb, 0xd1, 0x7b, 0xde, 0x03, 0xb8, 0xcc, 0xe5, 0x79, 0x5c, 0x3e, 0x5c,
    0x3c, 0xa1, 0x49, 0xbf, 0xda, 0xc4, 0xb4, 0x73, 0x2d, 0x49, 0x9f, 0x7a, 0xf9, 0x1d, 0x8a, 0xbc,
    0x39, 0x17, 0xbc, 0xd7, 0x45, 0xaf, 0xad, 0x38, 0x99, 0xc1, 0x1c, 0xf7, 0xe6, 0xf2, 0x7b, 0x16,
    0x26, 0xaa, 0xa9, 0x16, 0x96, 0xff, 0x02, 0x41, 0x97, 0x1e, 0x81, 0x43, 0xab, 0xff, 0xa8, 0xb7,
    0x19, 0xdf, 0x19, 0xac, 0x19, 0xdb, 0xbf, 0x15, 0x00, 0x12, 0xbe, 0x64, 0x2b, 0xd4, 0x50, 0x32,
    0x87, 0xdf, 0x52, 0xb1, 0x78, 0x65, 0x53, 0x7d, 0x10, 0x2d, 0xc1, 0x39, 0xfc, 0x9d, 0x05, 0x15,
    0x07, 0x84, 0xb2, 0xa1, 0x5b, 0x72, 0x82, 0x22, 0x73, 0x1f, 0x00, 0xbf, 0xf8, 0x71, 0x3a, 0xf3,
    0x5a, 0x02, 0x60, 0x53, 0x40, 0x44, 0x65, 0x0e, 0xb1, 0x3b, 0xe4, 0x9a, 0xe9, 0x8d, 0x10, 0x81,
    0xa4, 0x0b, 0xed, 0x02, 0xb7, 0x7f, 0x4a, 0x32, 0x90, 0xbb, 0xe7, 0xe6, 0xb8, 0x69, 0x0f, 0x95,
    0xea, 0x93, 0x45, 0x2c, 0x5f, 0x76, 0xfd, 0xb6, 0xcb, 0x1a, 0x7b, 0xe9, 0xc1, 0x37, 0xf7, 0x77,
    0xba, 0xb4, 0x1a, 0x26, 0xea, 0x68, 0x18, 0x35, 0x5d, 0x71, 0xe8, 0x3f, 0xdd, 0x97, 0x7d, 0x57,
    0xa6, 0x40, 0x45, 0xd8, 0x0d, 0xe4, 0xc7, 0xc0, 0x04, 0xdf, 0x20, 0x9e, 0x3a, 0x85, 0x85, 0x44,
    0x37, 0x45, 0x31, 0x96, 0x3b, 0xa8, 0xa7, 0xf6, 0xec, 0xff, 0xf1, 0xd1, 0xa4, 0x23, 0x7e, 0x8c, 
};

HI_S32 RSA_SIGN_VERIFY(HI_UNF_CIPHER_RSA_SIGN_SCHEME_E enScheme)
{
	HI_S32 ret = HI_SUCCESS;
    HI_U8  u8Sign[256];
    HI_U32 u32SignLen;
    HI_UNF_CIPHER_RSA_SIGN_S stRsaSign;
    HI_UNF_CIPHER_RSA_VERIFY_S stRsaVerify;
    HI_U32 time_start;

	ret = HI_UNF_CIPHER_Init();
	if ( HI_SUCCESS != ret )
	{
		return HI_FAILURE;
	}

    memset(&stRsaSign, 0, sizeof(HI_UNF_CIPHER_RSA_SIGN_S));
    stRsaSign.enScheme = enScheme;
    stRsaSign.stPriKey.pu8N = N;
    stRsaSign.stPriKey.pu8D = D;
    stRsaSign.stPriKey.u16NLen = sizeof(N);
    stRsaSign.stPriKey.u16DLen = sizeof(D);

    printf("RSA Sign Start...\n");
    time_start = get_timer(0);
    ret = HI_UNF_CIPHER_RsaSign(&stRsaSign, test_data, sizeof(test_data) - 1, sha256_sum, u8Sign, &u32SignLen);
    if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaSign failed\n");
		return HI_FAILURE;
	}
    time_start = get_timer(time_start);
    printf("RSA Sign Use Clock Cycle: %u, Clock Cycle for 1S: %lu\n", time_start, CONFIG_SYS_HZ);

    switch(enScheme)
    {
    case HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256:
        if(memcmp(u8Sign, RES, sizeof(RES)) != 0)
    	{
    	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaSign failed\n");
            Cipher_printBuffer("sign", u8Sign, u32SignLen);
            Cipher_printBuffer("golden", RES, sizeof(RES));
    		return HI_FAILURE;
    	}
        break;
    default:
        break;
    }

    Cipher_printBuffer("sign", u8Sign, u32SignLen);
    
    memset(&stRsaVerify, 0, sizeof(HI_UNF_CIPHER_RSA_VERIFY_S));
    stRsaVerify.enScheme = enScheme;
    stRsaVerify.stPubKey.pu8N = N;
    stRsaVerify.stPubKey.pu8E = E;
    stRsaVerify.stPubKey.u16NLen = sizeof(N);
    stRsaVerify.stPubKey.u16ELen = sizeof(E);

    printf("RSA Verify Start...\n");
    time_start = get_timer(0);
    ret = HI_UNF_CIPHER_RsaVerify(&stRsaVerify, test_data, sizeof(test_data) - 1, HI_NULL, u8Sign, u32SignLen);
    if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaVerify failed\n");
		return HI_FAILURE;
	}
    time_start = get_timer(time_start);
    printf("RSA Verify Use Clock Cycle: %u, Clock Cycle for 1S: %lu\n", time_start, CONFIG_SYS_HZ);

    printf("\033[0;32m""sample %s run successfully!\n""\033[0m",  __FUNCTION__);

    HI_UNF_CIPHER_DeInit();

    return HI_SUCCESS;
}

static int rsa_sign_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    return RSA_SIGN_VERIFY(HI_UNF_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256);
}

U_BOOT_CMD(rsa_sign_test, 1, 1, rsa_sign_test, "RSA PKCS1# V1_5 Sign Test", "for example: rsa_sign_test");

static HI_U8 NO_PADDING[] =
{
    0x31, 0x5d, 0xfa, 0x52, 0xa4, 0x93, 0x52, 0xf8, 0xf5, 0xed, 0x39, 0xf4, 0xf8, 0x23, 0x4b, 0x30, 
    0x11, 0xa2, 0x2c, 0x5b, 0xa9, 0x8c, 0xcf, 0xdf, 0x19, 0x66, 0xf5, 0xf5, 0x1a, 0x6d, 0xf6, 0x25, 
    0x89, 0xaf, 0x06, 0x13, 0xdc, 0xa4, 0xd4, 0x0b, 0x3c, 0x1c, 0x4f, 0xb9, 0xd3, 0xd0, 0x63, 0x29, 
    0x2a, 0x5d, 0xfe, 0xb6, 0x99, 0x20, 0x58, 0x36, 0x2b, 0x1d, 0x57, 0xf4, 0x71, 0x38, 0xa7, 0x8b, 
    0xad, 0x8c, 0xef, 0x1f, 0x2f, 0xea, 0x4c, 0x87, 0x2b, 0xd7, 0xb8, 0xc8, 0xb8, 0x09, 0xcb, 0xb9, 
    0x05, 0xab, 0x43, 0x41, 0xd9, 0x75, 0x36, 0x4d, 0xb6, 0x8a, 0xd3, 0x45, 0x96, 0xfd, 0x9c, 0xe8, 
    0x6e, 0xc8, 0x37, 0x5e, 0x4f, 0x63, 0xf4, 0x1c, 0x18, 0x2c, 0x38, 0x79, 0xe2, 0x5a, 0xe5, 0x1d, 
    0x48, 0xf6, 0xb2, 0x79, 0x57, 0x12, 0xab, 0xae, 0xc1, 0xb1, 0x9d, 0x11, 0x4f, 0xa1, 0x4d, 0x1b, 
    0x4c, 0x8c, 0x3a, 0x2d, 0x7b, 0x98, 0xb9, 0x89, 0x7b, 0x38, 0x84, 0x13, 0x8e, 0x3f, 0x3c, 0xe8, 
    0x59, 0x26, 0x90, 0x77, 0xe7, 0xca, 0x52, 0xbf, 0x3a, 0x5e, 0xe2, 0x58, 0x54, 0xd5, 0x9b, 0x2a, 
    0x0d, 0x33, 0x31, 0xf4, 0x4d, 0x68, 0x68, 0xf3, 0xe9, 0xb2, 0xbe, 0x28, 0xeb, 0xce, 0xdb, 0x36, 
    0x1e, 0xae, 0xb7, 0x37, 0xca, 0xaa, 0xf0, 0x9c, 0x6e, 0x27, 0x93, 0xc9, 0x61, 0x76, 0x99, 0x1a, 
    0x0a, 0x99, 0x57, 0xa8, 0xea, 0x71, 0x96, 0x63, 0xbc, 0x76, 0x11, 0x5c, 0x0c, 0xd4, 0x70, 0x0b, 
    0xd8, 0x1c, 0x4e, 0x95, 0x89, 0x5b, 0x09, 0x17, 0x08, 0x44, 0x70, 0xec, 0x60, 0x7c, 0xc9, 0x8a, 
    0xa0, 0xe8, 0x98, 0x64, 0xfa, 0xe7, 0x52, 0x73, 0xb0, 0x04, 0x9d, 0x78, 0xee, 0x09, 0xa1, 0xb9, 
    0x79, 0xd5, 0x52, 0x4f, 0xf2, 0x39, 0x1c, 0xf7, 0xb9, 0x73, 0xe0, 0x3d, 0x6b, 0x54, 0x64, 0x86
};

HI_S32 PKCS_PUB_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_E enScheme, HI_U8 *pu8Expect)
{
	HI_S32 ret = HI_SUCCESS;
    HI_U8  u8Enc[256];
    HI_U8  u8Dec[256];
    HI_U32 u32OutLen;
    HI_UNF_CIPHER_RSA_PUB_ENC_S stRsaEnc;
    HI_UNF_CIPHER_RSA_PRI_ENC_S stRsaDec;

	ret = HI_UNF_CIPHER_Init();
	if ( HI_SUCCESS != ret )
	{
		return HI_FAILURE;
	}

    memset(&stRsaEnc, 0, sizeof(HI_UNF_CIPHER_RSA_PUB_ENC_S));
    memset(&stRsaDec, 0, sizeof(HI_UNF_CIPHER_RSA_PRI_ENC_S));

    stRsaEnc.enScheme = enScheme;
    stRsaEnc.stPubKey.pu8N = N;
    stRsaEnc.stPubKey.pu8E = E;
    stRsaEnc.stPubKey.u16NLen = sizeof(N);
    stRsaEnc.stPubKey.u16ELen = sizeof(E);
    if(enScheme == HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING)
    {
        ret = HI_UNF_CIPHER_RsaPublicEncrypt(&stRsaEnc, NO_PADDING, 256, u8Enc, &u32OutLen);
    }
    else
    {
        ret = HI_UNF_CIPHER_RsaPublicEncrypt(&stRsaEnc, test_data, sizeof(test_data) - 1, u8Enc, &u32OutLen);
    }

    if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaPublicEnc failed\n");
		return HI_FAILURE;
	}

    if(pu8Expect != HI_NULL)
    {
        if(memcmp(u8Enc, pu8Expect, u32OutLen) != 0)
    	{
    	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaPublicEnc failed\n");
            Cipher_printBuffer("enc", u8Enc, u32OutLen);
            Cipher_printBuffer("expect", pu8Expect, 256);
    		return HI_FAILURE;
    	}
    }
   
    stRsaDec.enScheme = enScheme;
    stRsaDec.stPriKey.pu8N = N;
    stRsaDec.stPriKey.pu8D = D;
    stRsaDec.stPriKey.u16NLen = sizeof(N);
    stRsaDec.stPriKey.u16DLen = sizeof(D);

    ret = HI_UNF_CIPHER_RsaPrivateDecrypt(&stRsaDec, u8Enc, u32OutLen, u8Dec, &u32OutLen);
    if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaVerify failed\n");
		return HI_FAILURE;
	}

    if(enScheme == HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING)
    {
        if(memcmp(u8Dec, NO_PADDING, 256) != 0)
        {
            HI_ERR_CIPHER("HI_UNF_CIPHER_RsaPrivateDec failed\n");
            Cipher_printBuffer("enc", u8Enc, u32OutLen);
            Cipher_printBuffer("expect", NO_PADDING, 256);
            return HI_FAILURE;
        }
    }
    else
    {
        if((sizeof(test_data) - 1) != u32OutLen)
        {
            HI_ERR_CIPHER("HI_UNF_CIPHER_RsaPrivateDec len error\n");
            printf("dec: 0x%x, expect: 0x%x\n", u32OutLen, sizeof(test_data) - 1);
            return HI_FAILURE;
        }
        if(memcmp(u8Dec, test_data, sizeof(test_data) - 1) != 0)
        {
            HI_ERR_CIPHER("HI_UNF_CIPHER_RsaPrivateDec failed\n");
            Cipher_printBuffer("enc", u8Enc, u32OutLen);
            Cipher_printBuffer("expect", test_data, sizeof(test_data) - 1);
            return HI_FAILURE;
        }
    }

    printf("\033[0;32m""sample %s run successfully!\n""\033[0m",  __FUNCTION__);
    
    HI_UNF_CIPHER_DeInit();

    return HI_SUCCESS;
}

HI_S32 PKCS_PRI_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_E enScheme)
{
	HI_S32 ret = HI_SUCCESS;
    HI_U8  u8Sign[256];
    HI_U32 u32SignLen;
    HI_U8  u8Hash[32];
    HI_U32 u32HLen;
    HI_UNF_CIPHER_RSA_PRI_ENC_S stRsaEnc;
    HI_UNF_CIPHER_RSA_PUB_ENC_S stRsaDec;

	ret = HI_UNF_CIPHER_Init();
	if ( HI_SUCCESS != ret )
	{
		return HI_FAILURE;
	}

    memset(&stRsaEnc, 0, sizeof(HI_UNF_CIPHER_RSA_SIGN_S));
    memset(&stRsaDec, 0, sizeof(HI_UNF_CIPHER_RSA_VERIFY_S));
    stRsaEnc.enScheme = enScheme;
    stRsaDec.enScheme = enScheme;

    stRsaEnc.stPriKey.pu8N = N;
    stRsaEnc.stPriKey.pu8D = D;
    stRsaEnc.stPriKey.u16NLen = sizeof(N);
    stRsaEnc.stPriKey.u16DLen = sizeof(D);
    stRsaDec.stPubKey.pu8N = N;
    stRsaDec.stPubKey.pu8E = E;
    stRsaDec.stPubKey.u16NLen = sizeof(N);
    stRsaDec.stPubKey.u16ELen = sizeof(E);
    
    ret = HI_UNF_CIPHER_RsaPrivateEncrypt(&stRsaEnc, sha256_sum, 32, u8Sign, &u32SignLen);
    if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaSign failed\n");
		return HI_FAILURE;
	}

    switch(enScheme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        break;
    default:
        break;
    }

 //   printBuffer("sign", u8Sign, u32SignLen);

    ret = HI_UNF_CIPHER_RsaPublicDecrypt(&stRsaDec, u8Sign, u32SignLen, u8Hash, &u32HLen);
    if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaVerify failed\n");
		return HI_FAILURE;
	}

    switch(enScheme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        if(memcmp(u8Hash, sha256_sum, u32HLen) != 0)
    	{
    	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaPublicDec failed\n");
            Cipher_printBuffer("sign", u8Sign, u32SignLen);
            Cipher_printBuffer("sha256_sum", sha256_sum, sizeof(sha256_sum));
    		return HI_FAILURE;
    	}
        break;
    default:
        break;
    }

    printf("\033[0;32m""sample %s run successfully!\n""\033[0m",  __FUNCTION__);
    
    HI_UNF_CIPHER_DeInit();

    return HI_SUCCESS;
}

static int rsa_enc_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    PKCS_PUB_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING, HI_NULL);
    PKCS_PUB_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2, HI_NULL);
    PKCS_PUB_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1, HI_NULL);
    PKCS_PUB_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256, HI_NULL);
    PKCS_PUB_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5, HI_NULL);
    PKCS_PRI_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1);
    
    return HI_SUCCESS;
}

U_BOOT_CMD(rsa_enc_test, 1, 1, rsa_enc_test, "RSA PKCS1# V1_5 encrypt/decrypt Test", "for example: rsa_enc_test");

static HI_S32 Setconfiginfo(HI_HANDLE chnHandle, HI_BOOL bKeyByCA, HI_UNF_CIPHER_ALG_E alg, HI_UNF_CIPHER_WORK_MODE_E mode, HI_UNF_CIPHER_KEY_LENGTH_E keyLen,
                                                const HI_U8 u8KeyBuf[16], const HI_U8 u8IVBuf[16])
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_CIPHER_CTRL_S CipherCtrl;
    
    memset(&CipherCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
    CipherCtrl.enAlg = alg;
    CipherCtrl.enWorkMode = mode;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    CipherCtrl.enKeyLen = keyLen;
    CipherCtrl.bKeyByCA = bKeyByCA;
    if(CipherCtrl.enWorkMode != HI_UNF_CIPHER_WORK_MODE_ECB)
    {
        CipherCtrl.stChangeFlags.bit1IV = 1;  //must set for CBC , CFB mode
        memcpy(CipherCtrl.u32IV, u8IVBuf, 16);
    }

    memcpy(CipherCtrl.u32Key, u8KeyBuf, 16);
           
    s32Ret = HI_UNF_CIPHER_ConfigHandle(chnHandle, &CipherCtrl);
    if(HI_SUCCESS != s32Ret)
	{
		return HI_FAILURE;	
	}

    return HI_SUCCESS;
}

static int AES(HI_UNF_CIPHER_WORK_MODE_E mode,
           const unsigned char u8Key[16], const unsigned char u8IV[16],
           const unsigned char u8Src[16], const unsigned char u8Dst[16])
{
	int s32Ret = 0;
    HI_HANDLE hTestchnid;
    unsigned char u8InBuf[16];
    unsigned char u8OutBuf[16];
    HI_UNF_CIPHER_ATTS_S stCipherAttr = {0};

    s32Ret = HI_UNF_CIPHER_Init();
    if(HI_SUCCESS != s32Ret)
	{
		return HI_FAILURE;	
	}
    
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    s32Ret = HI_UNF_CIPHER_CreateHandle(&hTestchnid, &stCipherAttr);
    if(0 != s32Ret)
	{
	    HI_ERR_CIPHER("Error: CreateHandle failed!\n");
		return HI_FAILURE;
	}

	/* For encrypt */
    s32Ret = Setconfiginfo(hTestchnid,
                            HI_FALSE,
                            HI_UNF_CIPHER_ALG_AES,
                            mode,
                            HI_UNF_CIPHER_KEY_AES_128BIT,
                            u8Key,
                            u8IV);
	if(0 != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memcpy(u8InBuf, u8Src, 16);
    s32Ret = HI_UNF_CIPHER_Encrypt(hTestchnid, (HI_U32)u8InBuf, (HI_U32)u8OutBuf, 16);
    if(0 != s32Ret)
	{
		HI_ERR_CIPHER("Cipher encrypt failed. hTestchnid=0x%x\n", hTestchnid);
		s32Ret = -1;
		goto __CIPHER_EXIT__;
	}
    Cipher_printBuffer("ENCRYPT", u8OutBuf, 16);
    if(memcmp(u8OutBuf, u8Dst, 16))
	{
		HI_ERR_CIPHER("Cipher encrypt failed. hTestchnid=0x%x\n", hTestchnid);
		s32Ret = -1;
		goto __CIPHER_EXIT__;
	}
    
	/* For decrypt */
	s32Ret = Setconfiginfo(hTestchnid,
                                    HI_FALSE,
                                    HI_UNF_CIPHER_ALG_AES,
                                    mode,
                                    HI_UNF_CIPHER_KEY_AES_128BIT,
                                    u8Key,
                                    u8IV);
	if(0 != s32Ret)
	{
		HI_ERR_CIPHER("Set config info failed.\n");
		goto __CIPHER_EXIT__;
	}

    memcpy(u8InBuf, u8Dst, 16);
    s32Ret = HI_UNF_CIPHER_Decrypt(hTestchnid, (HI_U32)u8InBuf, (HI_U32)u8OutBuf, 16);
    if(0 != s32Ret)
	{
		HI_ERR_CIPHER("Cipher decrypt failed.\n");
		s32Ret = -1;
		goto __CIPHER_EXIT__;
	}

    Cipher_printBuffer("DECRYPT", u8OutBuf, 16);
    if(memcmp(u8OutBuf, u8Src, 16))
	{
		HI_ERR_CIPHER("Cipher decrypt failed. hTestchnid=0x%x\n", hTestchnid);
		s32Ret = -1;
		goto __CIPHER_EXIT__;
	}

    printf("\033[0;32m""sample %s run successfully!\n""\033[0m",  __FUNCTION__);
    
__CIPHER_EXIT__:

    HI_UNF_CIPHER_DestroyHandle(hTestchnid);
    HI_UNF_CIPHER_DeInit();

    return s32Ret;
}

/* encrypt data using special chn*/
static int AES_CBC128(int argc, char* argv[])
{
    unsigned char u8Key[16] = {0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char u8IV[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,};
    unsigned char u8Src[16] = {0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char u8Dst[16] = {0x1e, 0x07, 0x63, 0x70, 0x20, 0x29, 0x01, 0xdf,0x6e, 0xc8, 0x4f, 0x5e, 0x42, 0x49, 0xb7, 0x2b};

    printf("\nAES-128-CBC TEST\n");

    return AES(HI_UNF_CIPHER_WORK_MODE_CBC, u8Key, u8IV, u8Src, u8Dst);
}

/* encrypt data using special chn*/
static int AES_CTR128(int argc, char* argv[])
{
    unsigned char u8Key[16] = {"\x7E\x24\x06\x78\x17\xFA\xE0\xD7\x43\xD6\xCE\x1F\x32\x53\x91\x63"};
    unsigned char u8IV[16]  = {"\x00\x6C\xB6\xDB\xC0\x54\x3B\x59\xDA\x48\xD9\x0B\x00\x00\x00\x01"};
    unsigned char u8Src[32] = {"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"};
    unsigned char u8Dst[32] = {"\x51\x04\xA1\x06\x16\x8A\x72\xD9\x79\x0D\x41\xEE\x8E\xDA\xD3\x88\xEB\x2E\x1E\xFC\x46\xDA\x57\xC8\xFC\xE6\x30\xDF\x91\x41\xBE\x28"};

    printf("\nAES-128-CTR TEST\n");

	return AES(HI_UNF_CIPHER_WORK_MODE_CTR, u8Key, u8IV, u8Src, u8Dst);
}

/* encrypt data using special chn*/
static int AES_CFB128(int argc, char* argv[])
{
    unsigned char u8Key[16] = {"\x2B\x7E\x15\x16\x28\xAE\xD2\xA6\xAB\xF7\x15\x88\x09\xCF\x4F\x3C"};
    unsigned char u8IV[16]  = {"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"};
    unsigned char u8Src[32] = {"\x6B\xC1\xBE\xE2\x2E\x40\x9F\x96\xE9\x3D\x7E\x11\x73\x93\x17\x2A\xAE\x2D\x8A\x57\x1E\x03\xAC\x9C\x9E\xB7\x6F\xAC\x45\xAF\x8E\x51"};
    unsigned char u8Dst[32] = {"\x3B\x3F\xD9\x2E\xB7\x2D\xAD\x20\x33\x34\x49\xF8\xE8\x3C\xFB\x4A\xC8\xA6\x45\x37\xA0\xB3\xA9\x3F\xCD\xE3\xCD\xAD\x9F\x1C\xE5\x8B"};

    printf("\nAES-128-CFB TEST\n");

	return AES(HI_UNF_CIPHER_WORK_MODE_CFB, u8Key, u8IV, u8Src, u8Dst);
}

static int cipher_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    AES_CBC128(argc, argv);
    AES_CTR128(argc, argv);
    AES_CFB128(argc, argv);
    
    return HI_SUCCESS;
}

U_BOOT_CMD(cipher_test, 1, 1, cipher_test, "CIPHER Encrypt And Decrypt Test", "for example: cipher_test");

static int rng_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_U32 i;
    HI_U32 u32Random;

    for(i=0; i<16; i++)
    {
        HI_UNF_CIPHER_GetRandomNumber(&u32Random);
        printf("Random: 0x%x\n", u32Random);
    }
    
    return HI_SUCCESS;
}

U_BOOT_CMD(rng_test, 1, 1, rng_test, "RNG Test", "for example: rng_test");

#endif /* #ifdef CIPHER_CMD_TEST */

#endif /* #ifndef HI_ADVCA_FUNCTION_RELEASE */
