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
#include "hi_common.h"
#include "hi_unf_advca.h"
#include "hi_unf_ecs.h"
#include "sample_ca_common.h"

/***************************** Macro Definition ******************************/
#define MAX_DATA_SIZE   (1024*1024 - 16)
#define SWPK_LEN           0x10

#if defined(CHIP_TYPE_hi3798cv200)
#define NEW_BOOT_FORMAT
#else
#define OLD_BOOT_FORMAT
#endif

#ifdef OLD_BOOT_FORMAT
#define PARAM_OFFSET       0x304
#define SWPK_SPACE_OFFSET  0x1800
#else
#define AUXAREALEN_ADDR             (0x218)
#define PARMS_ARAR_OFFSET           (0x400)
#define UNCKECKED_AREA_HISI_OFFSET  (0x3000 - 0X40)
#define AUXILIARY_CODE_AREA_OFFSET  (0x3000)
#define SWPK_SPACE_OFFSET           (UNCKECKED_AREA_HISI_OFFSET + SWPK_LEN)
#endif

/*************************** Structure Definition ****************************/

typedef struct {
    HI_U32   check_area_off[2];
    HI_U32   check_area_len[2];
    HI_U32   boot_area_len;
    HI_U32   reserved;
}BOOT_AREA_INFO;

/********************** Global Variable declaration **************************/
HI_U8 Clear_SWPK[SWPK_LEN]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};

HI_U8 Enc_SWPK[SWPK_LEN] = {0};

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
    HI_U32 readlen, check_area_offset, check_area_length;

    BOOT_AREA_INFO *pstBootAreaInfo __attribute__((unused)) = HI_NULL;
    HI_U32 AuxAreaLen __attribute__((unused)) = 0;
    HI_U32 CheckedAreaOff __attribute__((unused)) = 0;

    if (3 == argc)
    {
        pFileNameSrc   = argv[1];
        pFileNameDst   = argv[2];
    }
    else
    {
        printf("Usage: product_encrypt_boot signed_clear_boot  encrypted_boot\n");
        return HI_FAILURE;
    }

    HI_DEBUG_ADVCA("\n************ Encrypt Boot Start *************\n");
    
    fp_out = fopen(pFileNameDst,"wb");
    if(!fp_out)
    {
        HI_DEBUG_ADVCA("open file %s error!\n",pFileNameDst);
        Ret = HI_FAILURE;
        goto EXIT_CLOSE_FILE;
    }

    fp_in = fopen(pFileNameSrc,"rb");
    if(!fp_in)
    {
        HI_DEBUG_ADVCA("open file %s error!\n",pFileNameSrc);
        Ret = HI_FAILURE;
        goto EXIT_CLOSE_FILE;
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

    readlen = fread(mmzbuf.user_viraddr,1, MAX_DATA_SIZE, fp_in);
    if(readlen <= SWPK_SPACE_OFFSET)
    {
        HI_DEBUG_ADVCA("Invalid file size %d.\n", readlen);
        goto EXIT_FREE_BUF;
    }

#ifdef OLD_BOOT_FORMAT
    pstBootAreaInfo = (BOOT_AREA_INFO *)(mmzbuf.user_viraddr + PARAM_OFFSET);
    check_area_offset = pstBootAreaInfo->check_area_off[0] + SWPK_SPACE_OFFSET;
    check_area_length = pstBootAreaInfo->check_area_len[0];

    if((check_area_offset <= SWPK_SPACE_OFFSET)||(check_area_offset >= MAX_DATA_SIZE))
    {
        HI_DEBUG_ADVCA("Invalid check_area_offset  0x%x.\n", check_area_offset);
        goto EXIT_FREE_BUF;
    }

    if((check_area_length <= 0)||(check_area_length >= (MAX_DATA_SIZE -SWPK_SPACE_OFFSET))||(0!=(check_area_length%16)))
    {
        HI_DEBUG_ADVCA("Invalid check_area_length  0x%x.\n", check_area_length);
        goto EXIT_FREE_BUF;
    }
#else
    AuxAreaLen = *((HI_U32 *) (mmzbuf.user_viraddr + AUXAREALEN_ADDR));
    CheckedAreaOff = *((HI_U32 *) (mmzbuf.user_viraddr + PARMS_ARAR_OFFSET));
    check_area_length = *((HI_U32 *) (mmzbuf.user_viraddr + PARMS_ARAR_OFFSET + 4));
    check_area_offset = AUXILIARY_CODE_AREA_OFFSET + AuxAreaLen + CheckedAreaOff;   
#endif

    HI_DEBUG_ADVCA("boot_size [0x%x], AuxAreaLen [0x%x], check_area_offset [0x%x], check_area_length [0x%x].\n", readlen, AuxAreaLen, check_area_offset, check_area_length);

    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed:%x\n",Ret);
        goto EXIT_FREE_BUF;
    }
    
    Ret = HI_UNF_ADVCA_EncryptSWPK(Clear_SWPK, Enc_SWPK);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_EncryptSWPK failed:%x\n",Ret);
        goto EXIT_CA_DEINIT;
    }

    #if 1
    {
        HI_U32 i;
        
        printf("\nClear SWPK : \n");
        for(i=0; i<16; i++)
        {
            printf("%02x ", Clear_SWPK[i]);
        }
        printf("\nEncrypted SWPK : \n");
        for(i=0; i<16; i++)
        {
            printf("%02x ", Enc_SWPK[i]);
        }
        printf("\n");
    }
    #endif
#ifdef OLD_BOOT_FORMAT
    memcpy((HI_U8 *)(mmzbuf.user_viraddr + SWPK_SPACE_OFFSET), Enc_SWPK, SWPK_LEN);
    memset((HI_U8 *)(mmzbuf.user_viraddr + SWPK_SPACE_OFFSET + SWPK_LEN), 0x48, 1);
#endif

#ifdef NEW_BOOT_FORMAT
    memcpy((HI_U8 *)(mmzbuf.user_viraddr + SWPK_SPACE_OFFSET), Enc_SWPK, SWPK_LEN);
    memset((HI_U8 *)(mmzbuf.user_viraddr + UNCKECKED_AREA_HISI_OFFSET + 12), 0x12, 1);
    memset((HI_U8 *)(mmzbuf.user_viraddr + UNCKECKED_AREA_HISI_OFFSET + 13), 0xC8, 1);
    memset((HI_U8 *)(mmzbuf.user_viraddr + UNCKECKED_AREA_HISI_OFFSET + 14), 0x13, 1);
    memset((HI_U8 *)(mmzbuf.user_viraddr + UNCKECKED_AREA_HISI_OFFSET + 15), 0x2A, 1);
#endif
    
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

    memset(&stCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
    memcpy(stCtrl.u32Key, Clear_SWPK,SWPK_LEN);
    stCtrl.bKeyByCA = HI_FALSE;
    stCtrl.stChangeFlags.bit1IV = 1;
    stCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
    stCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    stCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
    stCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
    
    Ret = HI_UNF_CIPHER_ConfigHandle(hCipher,&stCtrl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_ConfigHandle failed:%x\n",Ret);
        goto EXIT_DESTROY;
    }

    Ret = HI_UNF_CIPHER_Encrypt(hCipher, mmzbuf.phyaddr+check_area_offset, mmzbuf.phyaddr+check_area_offset, check_area_length);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Encrypt failed:%x\n",Ret);
        goto EXIT_FREE_BUF;
    }

    fwrite(mmzbuf.user_viraddr,1,readlen,fp_out);

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

    HI_DEBUG_ADVCA("************ Encrypt Boot End, Ret=%d *************\n\n", Ret);

    return Ret;
}


