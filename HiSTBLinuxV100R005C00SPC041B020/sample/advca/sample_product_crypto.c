/**
\file
\brief r2r hardware cryption
\copyright Shenzhen Hisilicon Co., Ltd.
\version draft
\author f00172091
\date 2013-1-22
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hi_type.h"
#include "hi_unf_common.h"
#include "hi_unf_advca.h"
#include "hi_unf_ecs.h"
#include "sample_ca_common.h"


/***************************** Macro Definition ******************************/
#define TEXT_LEN  16
#define MAX_DATA_SIZE   (512*1024)
/*************************** Structure Definition ****************************/
/********************** Global Variable declaration **************************/
#if 0
HI_U8 SWPK[16] = "8288300882883009";
HI_U8 G1_KEYLADDER_KEY1[16] = "auth1_keyladder1";
HI_U8 G1_KEYLADDER_KEY2[16] = "auth1_keyladder2";
HI_U8 G1_CIPHERKEY[16] = "Hisilicon_cipher";

HI_U8 G2_KEYLADDER_KEY1[16] = "auth2_keyladder1";
HI_U8 G2_KEYLADDER_KEY2[16] = "auth2_keyladder2";
HI_U8 G2_CIPHERKEY[16] = "Hisilicon_second";
#else
HI_U8 SWPK[16] = {0x67,0xb1,0xf8,0x28,0xdd,0x93,0x41,0xbc,0xca,0xed,0x73,0xd4,0x8d,0xa8,0xe9,0x1f};
static HI_U8 G1_CIPHERKEY[16] = {0x5b,0x45,0x14,0x37,0x6d,0xed,0x1d,0x08,0x64,0x97,0xbc,0xba,0xe6,0x7f,0x90,0x78};
static HI_U8 G2_CIPHERKEY[16] = {0xd6,0xbc,0x9e,0xe2,0x3a,0x61,0x50,0x10,0xb4,0x64,0x2e,0xaa,0xe8,0xf0,0x53,0xa6};
static HI_U8 G1_KEYLADDER_KEY1[16] = {0x1c,0x21,0x42,0xa2,0x9e,0x31,0x57,0x8d,0xff,0xa4,0xe1,0xdf,0x2e,0x36,0xa0,0x6e};
//static HI_U8 G1_KEYLADDER_KEY2[16] = {0x7c,0x76,0x1a,0x38,0xc1,0x46,0x4e,0x53,0x8c,0xa3,0xd7,0xae,0x56,0xee,0x27,0xb7};
static HI_U8 G2_KEYLADDER_KEY1[16] = {0x41,0x98,0x30,0x77,0x14,0xed,0x9d,0xc3,0x60,0x55,0x49,0xcf,0x5e,0x75,0x13,0xa2};
//static HI_U8 G2_KEYLADDER_KEY2[16] = {0x5c,0x98,0xbe,0x7c,0x52,0x24,0x35,0xb8,0x30,0xa5,0x10,0x54,0x33,0xc3,0x21,0xf8};
#endif

/******************************* API declaration *****************************/



HI_S32 main(HI_S32 argc,HI_CHAR **argv)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_HANDLE hCipher;
    HI_UNF_CIPHER_CTRL_S stCtrl;
    HI_MMZ_BUF_S mmzbuf;
    HI_UNF_CIPHER_ATTS_S stCipherAttr = {0};
    
    FILE * fp_in = HI_NULL;
    FILE * fp_out = HI_NULL;
    char *pFileNameSrc = HI_NULL;
    char *pFileNameDst = HI_NULL;
    HI_U32 readlen;
    HI_U32  crypto_flag = 0;
    HI_U32  key_flag = 1;
    HI_U8 *pCipherKey=HI_NULL;
    HI_U8 *pProtectionKey1=HI_NULL;
    //HI_U8 *pProtectionKey2=HI_NULL;
        
    if (5 == argc)
    {
        crypto_flag  = strtol(argv[1],NULL,0);
        key_flag = strtol(argv[2],NULL,0);
        pFileNameSrc   = argv[3];
        pFileNameDst   = argv[4];
    }
    else
    {
        printf("Usage: product_crypto crypto_flag  key_flag input_file output_file\n"
               "       crypto_flag: 0--encrypt, 1--decrypt\n"
               "       key_flag: 0--SWPK 1--key1, 2--key2\n");
        printf("Example:./product_crypto 0 1 bootargs.sig bootargs.img\n");
        return HI_FAILURE;
    }

    HI_DEBUG_ADVCA("\n************ Crypt Image Start *************\n");
    HI_DEBUG_ADVCA("%s %s %s %s %s\n",argv[0],argv[1],argv[2],argv[3], argv[4]);

    if(key_flag > 2)
    {
    	HI_DEBUG_ADVCA("invalid key_flag!\n");
    	return(HI_FAILURE);
    }
        
    fp_out = fopen(pFileNameDst,"wb");
    if(!fp_out)
    {
    	HI_DEBUG_ADVCA("open file %s error!\n",pFileNameDst);
    	return(HI_FAILURE);
    }

    fp_in = fopen(pFileNameSrc,"rb");
    if(!fp_in)
    {
    	HI_DEBUG_ADVCA("open file %s error!\n",pFileNameSrc);
    	fclose(fp_out);
       return(HI_FAILURE);
    }

    strncpy(mmzbuf.bufname,"encrypted_text", MAX_BUFFER_NAME_SIZE);
    mmzbuf.bufsize = MAX_DATA_SIZE;
    mmzbuf.overflow_threshold = 100;
    mmzbuf.underflow_threshold = 0;
    Ret = HI_MMZ_Malloc(&mmzbuf);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_MMZ_Malloc failed:%x\n",Ret);
        goto EXIT_CLOSE_FILE;
    }

    if(key_flag == 0)
    {
        pCipherKey = SWPK;
    }
   else  if(key_flag == 1)
    {
        pCipherKey = G1_CIPHERKEY;
        pProtectionKey1 = G1_KEYLADDER_KEY1;
        //pProtectionKey2 = G1_KEYLADDER_KEY2;
    }
    else if(key_flag == 2)
    {
        pCipherKey = G2_CIPHERKEY;
        pProtectionKey1 = G2_KEYLADDER_KEY1;
        //pProtectionKey2 = G2_KEYLADDER_KEY2;
    }
    
    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed:%x\n",Ret);
        goto EXIT_FREE_BUF;
    }

    Ret = HI_UNF_CIPHER_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Open failed:%x\n",Ret);
        goto EXIT_CA_DEINIT;
    }

	stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    Ret = HI_UNF_CIPHER_CreateHandle(&hCipher, &stCipherAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_CreateHandle failed:%x\n",Ret);
        goto EXIT_CLOSE_CIPHER;
    }

    /* Set R2R keyladder key */  
    if((key_flag == 1)||(key_flag == 2))
    {
        Ret = HI_UNF_ADVCA_SetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_AES);
        Ret |= HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV1, pProtectionKey1);
        if (HI_SUCCESS != Ret)
        {
            goto EXIT_DESTROY;
        }
    }
    
    memcpy(stCtrl.u32Key, pCipherKey,16);
    if(key_flag == 0)
    {
        /*used to encrypt upgrade image*/
        stCtrl.bKeyByCA = HI_FALSE;
        stCtrl.enAlg = HI_UNF_CIPHER_ALG_3DES;
        stCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
        stCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
        stCtrl.enKeyLen = HI_UNF_CIPHER_KEY_DES_2KEY;        
    }
    else    
    {
        stCtrl.bKeyByCA = HI_TRUE;
        stCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
        stCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
        stCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
        stCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
        stCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
    }
    Ret = HI_UNF_CIPHER_ConfigHandle(hCipher,&stCtrl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_ConfigHandle failed:%x\n",Ret);
        goto EXIT_DESTROY;
    }

    
    while(1)
    {
        readlen = fread(mmzbuf.user_viraddr,1, MAX_DATA_SIZE, fp_in);
        if(readlen<=0)
        {
//            HI_DEBUG_ADVCA("Read to file end.\n");
            goto EXIT_FREE_BUF;
        }
        else if((0 != (readlen%16)) && (0==feof(fp_in)))
        {
            HI_DEBUG_ADVCA("Not EOF and the readlen 0x%x is not align to 16bytes!\n",readlen);
            goto EXIT_FREE_BUF;
        }
        else
        {
            if(0 != (readlen%16))
            {
                HI_DEBUG_ADVCA("Read to file end, readlen %x is not align to 16bytes!\n",readlen);
                readlen = (readlen/16)*16+16;
            }
        
            if(0 == crypto_flag)
            {
                Ret = HI_UNF_CIPHER_Encrypt(hCipher,mmzbuf.phyaddr,mmzbuf.phyaddr,readlen);
            }
            else
            {
                Ret = HI_UNF_CIPHER_Decrypt(hCipher,mmzbuf.phyaddr,mmzbuf.phyaddr,readlen);
            }
                
            if (HI_SUCCESS != Ret)
            {
                HI_DEBUG_ADVCA("HI_UNF_CIPHER_Encrypt failed:%x\n",Ret);
                goto EXIT_FREE_BUF;
            }
            
            fwrite(mmzbuf.user_viraddr,1,readlen,fp_out);
        }
    }    

EXIT_DESTROY:
    (HI_VOID)HI_UNF_CIPHER_DestroyHandle(hCipher);
EXIT_CLOSE_CIPHER:
    (HI_VOID)HI_UNF_CIPHER_DeInit();
EXIT_CA_DEINIT:
    (HI_VOID)HI_UNF_ADVCA_DeInit();

EXIT_FREE_BUF:
    (HI_VOID)HI_MMZ_Free(&mmzbuf);    

EXIT_CLOSE_FILE:
    if(HI_NULL != fp_in)
    {
        fclose(fp_in);
    }
    if(HI_NULL != fp_out)
    {
        fclose(fp_out);
    }

    HI_DEBUG_ADVCA("************ Crypt Image End, Ret=0x%x *************\n\n", Ret);

    return Ret;
}


