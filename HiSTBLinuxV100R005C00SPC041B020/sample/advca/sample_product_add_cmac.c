/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

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

HI_U8 SWPK[16] = {0x67,0xb1,0xf8,0x28,0xdd,0x93,0x41,0xbc,0xca,0xed,0x73,0xd4,0x8d,0xa8,0xe9,0x1f};

static HI_U8 u8InputKey[16]= {0x23,0x34,0x34,0x52,0x34,0x55,0x45,0x84,0x54,0x54,0x84,0x53,0x34,0x47,0x34,0x47};

/******************************* API declaration *****************************/

#define CMAC_VALUE_LEN 16

/*The data strcutue must be 16Bytes align*/
typedef struct hi_CaCMACDataHead_S
{   
    HI_U8  au8MagicNumber[32];
    HI_U32 u32Version;
    HI_U32 u32CMACDataLen;                       //Data length
    HI_U32 u32Reserve[6];                        //Reserve for forture  
    HI_U8  u8CMACValue[CMAC_VALUE_LEN]; 
    HI_U8  u8HeaderCMACValue[CMAC_VALUE_LEN];
} HI_CaCMACDataHead_S;


HI_S32 main(HI_S32 argc,HI_CHAR **argv)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_S32 i;
    
    FILE * fp_in = HI_NULL;
    FILE * fp_out = HI_NULL;
    char *pFileNameSrc = HI_NULL;
    char *pFileNameDst = HI_NULL;
    HI_U8  *pu8DataBuf = HI_NULL;  
    HI_U32  u32DataTotalLen = 0, u32HeaderLen, u32ReadLen, U32TmpLen;  
    
    HI_CaCMACDataHead_S *pstCaCMACDataHeader;
    HI_U8 u8OutCmacValue[CMAC_VALUE_LEN];
        
    if (3 == argc)
    {
        pFileNameSrc   = argv[1];
        pFileNameDst   = argv[2];
    }
    else
    {
        HI_DEBUG_ADVCA("Usage: product_add_cmac input_file output_file\n");
        HI_DEBUG_ADVCA("Example:./product_add_cmac logo.img logo_cmac.img\n");
        return HI_FAILURE;
    }

    HI_DEBUG_ADVCA("\n************ Start to generate CMAC image*************\n");
    HI_DEBUG_ADVCA("%s %s %s\n",argv[0],argv[1],argv[2]);

    if (HI_SUCCESS != HI_UNF_ADVCA_Init())
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed:%x\n",Ret);
        return HI_FAILURE;
    }

    if (HI_SUCCESS != HI_UNF_CIPHER_Init())
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Open failed:%x\n",Ret);
        goto ERROR_EXIT_1;
    }
        
    fp_out = fopen(pFileNameDst,"wb");
    if(!fp_out)
    {
    	HI_DEBUG_ADVCA("open file %s error!\n",pFileNameDst);
    	goto ERROR_EXIT_2;
    }

    fp_in = fopen(pFileNameSrc,"rb");
    if(!fp_in)
    {
    	HI_DEBUG_ADVCA("open file %s error!\n",pFileNameSrc);  
        goto ERROR_EXIT_2;
    }

    fseek(fp_in, 0L, SEEK_END);   
    u32ReadLen = ftell(fp_in);
    rewind(fp_in);
    //fseek(fp_in, 0L, SEEK_SET);

    u32HeaderLen = sizeof(HI_CaCMACDataHead_S);
    u32DataTotalLen = u32ReadLen + u32HeaderLen;
    if (0 != (u32DataTotalLen%16))
    {
       HI_DEBUG_ADVCA("The data length is not align to 16 byte, adjust it!\n");
       u32DataTotalLen = u32DataTotalLen + 16 - (u32DataTotalLen%16);// align to 16 bytes
    }

    pu8DataBuf = malloc(u32DataTotalLen);
    memset(pu8DataBuf, 0xFF, u32DataTotalLen);
    
    U32TmpLen = fread(&pu8DataBuf[u32HeaderLen], 1, u32ReadLen, fp_in);
    if (U32TmpLen != u32ReadLen)
    {
        HI_DEBUG_ADVCA("The length read from input is not correct!\n");
        goto ERROR_EXIT_2;
    }
    
    U32TmpLen = u32DataTotalLen - u32HeaderLen; 
    Ret = HI_UNF_ADVCA_CalculteAES_CMAC(&pu8DataBuf[u32HeaderLen], U32TmpLen, u8InputKey, u8OutCmacValue);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Fail to calculate data CMAC. Ret: %d\n",Ret);
        goto ERROR_EXIT_2;
    }

    HI_DEBUG_ADVCA("Data CMAC value is :");
    for (i=0; i< CMAC_VALUE_LEN; i++)
    {
       HI_DEBUG_ADVCA(" 0x%x", u8OutCmacValue[i]);
    }
    HI_DEBUG_ADVCA("\n");

    pstCaCMACDataHeader = (HI_CaCMACDataHead_S *)pu8DataBuf;
    pstCaCMACDataHeader->u32CMACDataLen = U32TmpLen;
    memcpy(pstCaCMACDataHeader->u8CMACValue, u8OutCmacValue, CMAC_VALUE_LEN);
    /***********************************************************************************
                    start to calculate head data length
    ***********************************************************************************/    
    U32TmpLen = u32HeaderLen - CMAC_VALUE_LEN; 
    Ret = HI_UNF_ADVCA_CalculteAES_CMAC(&pu8DataBuf[0], U32TmpLen, u8InputKey, u8OutCmacValue);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Fail to calculate header CMAC. Ret: %d\n",Ret);
        goto ERROR_EXIT_2;
    }

    HI_DEBUG_ADVCA("header CMAC value is :");
    for (i=0; i< CMAC_VALUE_LEN; i++)
    {
       HI_DEBUG_ADVCA(" 0x%x", u8OutCmacValue[i]);
    }
    HI_DEBUG_ADVCA("\n");

    memcpy(pstCaCMACDataHeader->u8HeaderCMACValue, u8OutCmacValue, CMAC_VALUE_LEN);

    fwrite(pu8DataBuf, 1, u32DataTotalLen, fp_out);

ERROR_EXIT_2:
    if(HI_NULL != fp_in)
    {
        fclose(fp_in);
    }
    
    if(HI_NULL != fp_out)
    {
        fclose(fp_out);
    }

    if (pu8DataBuf != HI_NULL)
    {
       free(pu8DataBuf);
    }
    
ERROR_EXIT_1:
    (HI_VOID)HI_UNF_CIPHER_DeInit();
    (HI_VOID)HI_UNF_ADVCA_DeInit(); 
    
    return Ret;
}


