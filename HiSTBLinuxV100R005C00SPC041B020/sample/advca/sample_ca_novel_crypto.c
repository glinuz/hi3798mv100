/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ca_crypto.c
  Version       : Initial Draft
  Author        : l00188263
  Created       : 2012/1/6
  Last Modified :
  Description   : CA_CRYPTO
  Function List :
              main
  History       :
  1.Date        : 2012/1/6
    Author      : l00188263
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "hi_type.h"
#include <string.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_adp.h"
#include "hi_unf_advca.h"

#define TEXT_LEN  16
HI_U8 g_PlainText[TEXT_LEN]     = "0123456789abcdef";  /* 436f6e6178204153 */
HI_S32 main(HI_S32 argc,HI_CHAR** argv)
{
    HI_S32 Ret;
    HI_S32 index;

    HI_MMZ_BUF_S PlainBuf,EncrytedBuf, DecryptBuf;
    
    strcpy(PlainBuf.bufname,"plain_text");
    PlainBuf.bufsize = 0x1000;
    PlainBuf.overflow_threshold = 100;
    PlainBuf.underflow_threshold = 0;
    Ret = HI_MMZ_Malloc(&PlainBuf);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_MMZ_Malloc failed:%x\n",Ret);
        return HI_FAILURE;
    }

    strcpy(EncrytedBuf.bufname,"encrypted_text");
    EncrytedBuf.bufsize = 0x1000;
    EncrytedBuf.overflow_threshold = 100;
    EncrytedBuf.underflow_threshold = 0;
    Ret = HI_MMZ_Malloc(&EncrytedBuf);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_MMZ_Malloc failed:%x\n",Ret);
        (HI_VOID)HI_MMZ_Free(&PlainBuf);
        return HI_FAILURE;
    }
    
    strcpy(DecryptBuf.bufname,"decrypted_text");
    DecryptBuf.bufsize = 0x1000;
    DecryptBuf.overflow_threshold = 100;
    DecryptBuf.underflow_threshold = 0;
    Ret = HI_MMZ_Malloc(&DecryptBuf);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_MMZ_Malloc failed:%x\n",Ret);
        (HI_VOID)HI_MMZ_Free(&PlainBuf);
        (HI_VOID)HI_MMZ_Free(&EncrytedBuf);
        return HI_FAILURE;
    }
    
    memcpy(PlainBuf.user_viraddr, g_PlainText, TEXT_LEN);
    memset(EncrytedBuf.user_viraddr, 0, EncrytedBuf.bufsize); 
    memset(DecryptBuf.user_viraddr, 0, DecryptBuf.bufsize); 

    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_ADVCA_Init failed\n");
        return HI_FAILURE;
    }  
    Ret = HI_UNF_ADVCA_NovelCryptologyConfig();
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_ADVCA_NovelCryptologyConfig failed\n");
        return HI_FAILURE;
    } 
    printf("contents of plain buf is \n");
    for(index = 0;index<TEXT_LEN;index++)
    {
        printf("0x%02x ", PlainBuf.user_viraddr[index]);
    }
    printf("\n");
    Ret = HI_UNF_ADVCA_NovelDataEncrypt((HI_U8 *)PlainBuf.phyaddr, TEXT_LEN, (HI_U8 *)EncrytedBuf.phyaddr);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_ADVCA_NovelDataEncrypt failed\n");
        return HI_FAILURE;
    } 

    printf("after encrypt the plain buf ,contents of encrypt buf is \n");
    for(index = 0;index<TEXT_LEN;index++)
    {
        printf("0x%02x ", EncrytedBuf.user_viraddr[index]);
    }
    printf("\n");
    Ret = HI_UNF_ADVCA_NovelDataDecrypt((HI_U8 *)EncrytedBuf.phyaddr, TEXT_LEN, (HI_U8 *)DecryptBuf.phyaddr);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_ADVCA_HI_UNF_ADVCA_NovelDataDecryptNovelDataEncrypt failed\n");
        return HI_FAILURE;
    }
    printf("after decrypt the encrypt buf ,contents of result buf is \n");
    for(index = 0;index<TEXT_LEN;index++)
    {
        printf("0x%02x ", DecryptBuf.user_viraddr[index]);
    }
    printf("\n");
    if(!memcmp(PlainBuf.user_viraddr,DecryptBuf.user_viraddr,TEXT_LEN))
    {
        printf("contents of plain buf and result buf is same ,test successful!\n");
    }
    else
    {
        printf("contents of plain buf and result buf is different ,test failed!\n");
    }
    HI_UNF_ADVCA_DeInit();   

    return Ret;
}




