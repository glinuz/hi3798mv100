/******************************************************************************
Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_rng.c
Version       : Initial Draft
Author        : Hisilicon
Created       : 2012/07/10
Last Modified :
Description   :	sample for hash
Function List :
History       :
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <assert.h> 

#include "hi_type.h"
#include "hi_unf_cipher.h"
#include "hi_common.h"
#include "hi_adp.h"

#define HI_ERR_CIPHER(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_INFO_CIPHER(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)

#if defined(CHIP_TYPE_hi3798cv200)
#define MAX_HASH_HANDLE 4
#define MAX_HMAC_HANDLE 3
#else
#define MAX_HASH_HANDLE 1
#define MAX_HMAC_HANDLE 1
#endif
#define LONG_DATA_SIZE 10000

static HI_S32 printBuffer(const HI_CHAR *string, const HI_U8 *pu8Input, HI_U32 u32Length)
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

static unsigned char sha1_buf[3][128] = {
    {"abc"},
    {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"},
    {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopqabcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"}
};
static const int sha1_buflen[4] ={3, 56, 112, 1000000};
static const unsigned char sha1_sum[5][20] =
{
    {0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E, 0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D},
    {0x84, 0x98, 0x3E, 0x44, 0x1C, 0x3B, 0xD2, 0x6E, 0xBA, 0xAE, 0x4A, 0xA1, 0xF9, 0x51, 0x29, 0xE5, 0xE5, 0x46, 0x70, 0xF1},
    {0xaf, 0xc5, 0x3a, 0x4e, 0xa2, 0x08, 0x56, 0xf9, 0x8e, 0x08, 0xdc, 0x6f, 0x3a, 0x5c, 0x98, 0x33, 0x13, 0x77, 0x68, 0xed},
    {0x34, 0xaa, 0x97, 0x3c, 0xd4, 0xc4, 0xda, 0xa4, 0xf6, 0x1e, 0xeb, 0x2b, 0xdb, 0xad, 0x27, 0x31, 0x65, 0x34, 0x01, 0x6f},
    {0x7d, 0xf9, 0x62, 0x1f, 0x17, 0xad, 0x18, 0xc5, 0x8a, 0x5a, 0xf7, 0x99, 0x1d, 0x12, 0x62, 0x20, 0x0f, 0xaf, 0xa9, 0x0f},
};

static const unsigned char sha2_sum[4][32] ={
    /** SHA-256 sample vectors*/
    {0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23, 0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD},
    {0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39, 0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1},
    {0x59, 0xf1, 0x09, 0xd9, 0x53, 0x3b, 0x2b, 0x70, 0xe7, 0xc3, 0xb8, 0x14, 0xa2, 0xbd, 0x21, 0x8f, 0x78, 0xea, 0x5d, 0x37, 0x14, 0x45, 0x5b, 0xc6, 0x79, 0x87, 0xcf, 0x0d, 0x66, 0x43, 0x99 ,0xcf},
    {0xcd, 0xc7, 0x6e, 0x5c, 0x99, 0x14, 0xfb, 0x92, 0x81, 0xa1, 0xc7, 0xe2, 0x84, 0xd7, 0x3e, 0x67, 0xf1, 0x80, 0x9a, 0x48, 0xa4, 0x97, 0x20, 0x0e, 0x04, 0x6d, 0x39, 0xcc, 0xc7, 0x11, 0x2c, 0xd0},
};

static HI_U8 sha1_hmac_test_key[3][26] =
{
    { "Hi ThereHi There" },//16 bytes
    { "There is the key" },
    { "This is your dog" },
};

static HI_U8 sha1_hmac_test_sum[3][20] =
{	
	{ 0x40, 0x99, 0x4A, 0xE6, 0xFF, 0x19, 0xDF, 0x88, 0x28, 0xBB,
	  0xEC, 0x7F, 0xA6, 0x68, 0x4C, 0x23, 0x2E, 0x78, 0x6B, 0xB3 },//one block
	{ 0xFC, 0x66, 0xA4, 0xCB, 0x9E, 0xF4, 0xE6, 0x88, 0x19, 0xF3,
	  0xE9, 0x40, 0xAE, 0xF0, 0x7A, 0x59, 0x5B, 0x72, 0x7A, 0x0F },//two blocks
	{ 0x73, 0xD3, 0x86, 0xE3, 0x0E, 0x42, 0x1A, 0x27, 0xC7, 0x53,
	  0xFA, 0x45, 0xA4, 0x78, 0x31, 0x34, 0xC0, 0x2A, 0x71, 0x2B } //10000 'a'
	
};

static HI_U8 sha2_hmac_test_sum[3][32] =
{

	{ 0x92, 0x89, 0xA8, 0xC5, 0x56, 0x35, 0xAE, 0x82,
	  0x9A, 0x63, 0x52, 0x55, 0xCF, 0x61, 0x42, 0x13,
	  0x58, 0x24, 0xD3, 0xDB, 0xAB, 0x2D, 0x91, 0x1A,
	  0xBC, 0x84, 0x22, 0x9C, 0x65, 0xCD, 0x3D, 0x4A }, //one block
	{ 0xEF, 0x03, 0xC8, 0xB5, 0x2F, 0xFC, 0x02, 0x3B,
	  0x7A, 0x99, 0x69, 0xF7, 0x0E, 0xF6, 0x03, 0x22,
	  0x90, 0x2B, 0x47, 0x98, 0x1F, 0x9B, 0x8F, 0xAF,
	  0xFE, 0x43, 0xE5, 0x61, 0x33, 0x8A, 0xE3, 0x44 }, //two blocks
	{ 0x4B, 0x3B, 0xF5, 0xA2, 0x57, 0xB6, 0xF9, 0x10,
	  0x35, 0x9E, 0xF4, 0x8D, 0x8A, 0x3B, 0xE1, 0x6E,
      0x0A, 0x09, 0x3A, 0x96, 0x53, 0x51, 0x72, 0xBD, 
	  0x6F, 0x82, 0x0D, 0xBC, 0xFB, 0x1D, 0x9B, 0x00 }  //10000 'a'
};

static HI_U8 au8Buf[LONG_DATA_SIZE];

HI_S32 SHA1()
{
	HI_S32 ret = HI_SUCCESS;
    HI_U8 u8Hash[20];
    HI_U32 i = 0,j = 0;
    HI_HANDLE hHandle[MAX_HASH_HANDLE];
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;

	ret = HI_UNF_CIPHER_Init();
	if ( HI_SUCCESS != ret )
	{
		return HI_FAILURE;
	}

    memset(u8Hash, 0, 20);

    for(i = 0; i < MAX_HASH_HANDLE; i++)
    {
        memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));
        stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_SHA1;

        ret = HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle[i]);
		if ( HI_SUCCESS != ret )
		{
			HI_UNF_CIPHER_DeInit();
			return HI_FAILURE;
		}
     }
     for(i = 0; i < MAX_HASH_HANDLE; i++)
     {
        if(i == 3)
        {
            memset(au8Buf, 'a', LONG_DATA_SIZE);
            for(j=0; j<1000000/LONG_DATA_SIZE; j++)
            {
                ret = HI_UNF_CIPHER_HashUpdate(hHandle[i], au8Buf, LONG_DATA_SIZE);
        		if ( HI_SUCCESS != ret )
        		{   
        			HI_UNF_CIPHER_DeInit();
        			return HI_FAILURE;
        		}
            }  
        }
        else
        {
            ret = HI_UNF_CIPHER_HashUpdate(hHandle[i], sha1_buf[i], sha1_buflen[i]);
    		if ( HI_SUCCESS != ret )
    		{
    			HI_UNF_CIPHER_DeInit();
    			return HI_FAILURE;
    		}
        }
     }

     for(i = 0; i < MAX_HASH_HANDLE; i++)
     {
        ret = HI_UNF_CIPHER_HashFinal(hHandle[i], u8Hash);
		if ( HI_SUCCESS != ret )
		{
			HI_UNF_CIPHER_DeInit();
			return HI_FAILURE;
		}
        if(memcmp(u8Hash, sha1_sum[i], 20) != 0)
        {
            HI_ERR_CIPHER("\033[0;31m" "SHA1 run failed, sample %d!\n" "\033[0m", i);
            printBuffer("Sha1 result:", u8Hash, 20);
            printBuffer("golden data:", sha1_sum[i], 20);
            HI_UNF_CIPHER_DeInit();
            return HI_FAILURE;
        }
        printf("SHA1 run success, sample %d!\n", i);
     }
    
    HI_ERR_CIPHER("sample SHA1 run successfully!\n");
    HI_UNF_CIPHER_DeInit();

    return HI_SUCCESS;
}

HI_S32 SHA256()
{
	HI_S32 ret = HI_SUCCESS;
    HI_U8 u8Hash[32];
    HI_U32 i = 0, j = 0;
    HI_HANDLE hHandle[MAX_HASH_HANDLE];
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;

	ret = HI_UNF_CIPHER_Init();
	if ( HI_SUCCESS != ret )
	{
		return HI_FAILURE;
	}

    for(i = 0; i < MAX_HASH_HANDLE; i++)
    {
        memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));
        stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_SHA256;

        ret = HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle[i]);
		if ( HI_SUCCESS != ret )
		{
			HI_UNF_CIPHER_DeInit();
			return HI_FAILURE;
		}
    }
    for(i = 0; i < MAX_HASH_HANDLE; i++)
    {
        if(i == 3)
        {
            memset(au8Buf, 'a', LONG_DATA_SIZE);
            for(j=0; j<1000000/LONG_DATA_SIZE; j++)
            {
                ret = HI_UNF_CIPHER_HashUpdate(hHandle[i], au8Buf, LONG_DATA_SIZE);
        		if ( HI_SUCCESS != ret )
        		{   
        			HI_UNF_CIPHER_DeInit();
        			return HI_FAILURE;
        		}
            }  
        }
        else
        {
            ret = HI_UNF_CIPHER_HashUpdate(hHandle[i], sha1_buf[i], sha1_buflen[i]);
    		if ( HI_SUCCESS != ret )
    		{
    			HI_UNF_CIPHER_DeInit();
    			return HI_FAILURE;
    		}
        }
    }
    for(i = 0; i < MAX_HASH_HANDLE; i++)
    {
        ret = HI_UNF_CIPHER_HashFinal(hHandle[i], u8Hash);
		if ( HI_SUCCESS != ret )
		{
			HI_UNF_CIPHER_DeInit();
			return HI_FAILURE;
		}

        if(memcmp(u8Hash, sha2_sum[i], 32) != 0)
        {
            HI_ERR_CIPHER("\033[0;31m" "SHA256 run failed, sample %d!\n" "\033[0m", i);
            printBuffer("Sha256 result:", u8Hash, 32);
            printBuffer("golden data:", sha2_sum[i], 20);
			HI_UNF_CIPHER_DeInit();
            return HI_FAILURE;
        }
        printf("SHA256 run success, sample %d!\n", i);
    }

    HI_ERR_CIPHER("sample SHA256 run successfully!\n");
    HI_UNF_CIPHER_DeInit();
    return HI_SUCCESS;
}

HI_S32 HMAC_SHA1()
{
    HI_S32 ret = HI_SUCCESS;
    HI_U8 u8Hash[20];
    HI_U32 i = 0;
    HI_HANDLE hHandle[MAX_HMAC_HANDLE];
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;

	ret = HI_UNF_CIPHER_Init();
	if ( HI_SUCCESS != ret )
	{
		return HI_FAILURE;
	}

    memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));
	stHashAttr.pu8HMACKey = malloc(16);
	stHashAttr.u32HMACKeyLen = 16;
	if ( NULL == stHashAttr.pu8HMACKey )
	{
	    HI_UNF_CIPHER_DeInit();
		return HI_FAILURE;
	}

    memset(u8Hash, 0, 20);

    for(i = 0; i < MAX_HMAC_HANDLE; i++)
    {
        stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1;
        memcpy(stHashAttr.pu8HMACKey, sha1_hmac_test_key[i], 16);
        
        ret = HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle[i]);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CIPHER("hash init failed!\n");
            ret = HI_FAILURE;
            goto __EXIT__;
        }
    }
    for(i = 0; i < MAX_HMAC_HANDLE; i++)
    {
        if (i == 2)
        {
            memset(au8Buf, 'a', 10000);
            ret = HI_UNF_CIPHER_HashUpdate(hHandle[i], au8Buf, 10000);
        }
        else
        {
            ret = HI_UNF_CIPHER_HashUpdate(hHandle[i], sha1_buf[i], sha1_buflen[i]);
        }
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CIPHER("hash update failed!\n");
            ret = HI_FAILURE;
            goto __EXIT__;
        }
    }
    for(i = 0; i < MAX_HMAC_HANDLE; i++)
    {
        ret = HI_UNF_CIPHER_HashFinal(hHandle[i], u8Hash);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CIPHER("hash final failed!\n");
            ret = HI_FAILURE;
            goto __EXIT__;
        }
        printBuffer("hmac-Sha1 result:", u8Hash, 20);
        if(memcmp(u8Hash, sha1_hmac_test_sum[i], 20) != 0)
        {
            HI_ERR_CIPHER("\033[0;31m" "HMAC_SHA1 run failed, sample %d!\n" "\033[0m", i);
            printBuffer("HMAC1 result:", u8Hash, 20);
            printBuffer("golden data:", sha1_hmac_test_sum[i], 20);
            HI_UNF_CIPHER_DeInit();
            return HI_FAILURE;
        }
    }

    HI_ERR_CIPHER("sample HMAC SHA1 run successfully!\n");

__EXIT__:
	free(stHashAttr.pu8HMACKey);
	stHashAttr.pu8HMACKey = NULL;
    HI_UNF_CIPHER_DeInit();

    return ret;
}

HI_S32 HMAC_SHA256()
{
    HI_S32 ret = HI_FAILURE;
    HI_U8 u8Hash[32];
    HI_U32 i = 0;
    HI_HANDLE hHandle[MAX_HMAC_HANDLE];
    HI_UNF_CIPHER_HASH_ATTS_S stHashAttr;

	ret = HI_UNF_CIPHER_Init();
	if ( HI_SUCCESS != ret )
	{
		return HI_FAILURE;
	}

    memset(&stHashAttr, 0, sizeof(HI_UNF_CIPHER_HASH_ATTS_S));
	stHashAttr.pu8HMACKey = malloc(16);
	stHashAttr.u32HMACKeyLen = 16;
	if ( NULL == stHashAttr.pu8HMACKey )
	{
	    HI_UNF_CIPHER_DeInit();
		return HI_FAILURE;
	}
	
    memset(u8Hash, 0, 32);

    for(i = 0; i < MAX_HMAC_HANDLE; i++)
    {
        stHashAttr.eShaType = HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256;
        memcpy(stHashAttr.pu8HMACKey, sha1_hmac_test_key[i], 16);

        ret = HI_UNF_CIPHER_HashInit(&stHashAttr, &hHandle[i]);
		if ( HI_SUCCESS != ret )
		{
			goto __EXIT__;
		}
    }
    for(i = 0; i < MAX_HMAC_HANDLE; i++)
    {
        if (i == 2)
        {
            memset(au8Buf, 'a', 10000);
            ret = HI_UNF_CIPHER_HashUpdate(hHandle[i], au8Buf, 10000);
        }
        else
        {
            ret = HI_UNF_CIPHER_HashUpdate(hHandle[i], sha1_buf[i], sha1_buflen[i]);
        }
		if ( HI_SUCCESS != ret )
		{
			goto __EXIT__;
		}
    }
    for(i = 0; i < MAX_HMAC_HANDLE; i++)
    {		
        ret = HI_UNF_CIPHER_HashFinal(hHandle[i], u8Hash);
		if ( HI_SUCCESS != ret )
		{
			goto __EXIT__;
		}

        printBuffer("hmac-Sha256 result:", u8Hash, 32);
        if(memcmp(u8Hash, sha2_hmac_test_sum[i], 32) != 0)
        {
            HI_ERR_CIPHER("\033[0;31m" "HMAC_SHA256 run failed, sample %d!\n" "\033[0m", i);
            printBuffer("HMAC256 result:", u8Hash, 32);
            printBuffer("golden data:", sha2_hmac_test_sum[i], 20);
            HI_UNF_CIPHER_DeInit();
            return HI_FAILURE;
        }
    }

    HI_ERR_CIPHER("sample HMAC SHA256 run successfully!\n");

__EXIT__:
    free(stHashAttr.pu8HMACKey);
	stHashAttr.pu8HMACKey = NULL;
    HI_UNF_CIPHER_DeInit();

    return HI_SUCCESS;
}

int main(int argc, char* argv[])
{
    
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 funcNumber = 0;

    if ( 2 != argc )
    {
        HI_INFO_CIPHER("Usage: %s funcNumber\n\n", argv[0]);
        HI_INFO_CIPHER("#########cipher sample##########\n"
                        "[0] SHA1\n"
                        "[1] SHA256\n"
                        "[2] HMAC-SHA1\n"
                        "[3] HMAC-SHA256\n");

        return HI_SUCCESS;
    }
    funcNumber = strtol(argv[1],NULL,0);
    
	if ( 0 == funcNumber )
    {
        s32Ret = SHA1();
    }
    else if ( 1 == funcNumber )
    {
        s32Ret = SHA256();
    }
    else if ( 2 == funcNumber )
    {
        s32Ret = HMAC_SHA1();
    }
    else if ( 3 == funcNumber )
    {
        s32Ret = HMAC_SHA256();
    }
    else    
    {      
        HI_INFO_CIPHER("funcNumber %d do not exist!\n",funcNumber);
    }

    if(HI_SUCCESS != s32Ret)
    {        
        HI_INFO_CIPHER("funcNumber %d run failure.\n",funcNumber);        
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

