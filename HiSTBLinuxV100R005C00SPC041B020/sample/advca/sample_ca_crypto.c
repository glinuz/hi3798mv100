/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_crypto.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hi_type.h"
#include "hi_common.h"
#include "hi_unf_advca.h"
#include "hi_unf_cipher.h"
#include "sample_ca_common.h"

/***************************** Macro Definition ******************************/
#define TEXT_LEN  16
/*************************** Structure Definition ****************************/
/********************** Global Variable declaration **************************/
HI_U8 g_PlainText[TEXT_LEN]      = {0x12,0x34,0x56,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  /* 436f6e6178204153 */
HI_U8 g_sessionkey1[16]          = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
HI_U8 g_contentkey[16]           = {0x0f,0x0e,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};
HI_U8 g_sessionkey2[16]          = {0x2f,0x0e,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};
HI_U8 g_sessionkey3[16]          = {0x3f,0x0e,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};
HI_U8 g_sessionkey4[16]          = {0x4f,0x0e,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};

/******************************* API declaration *****************************/

static HI_VOID Debug_Register(HI_VOID)
{
    usleep(100*1000);system("himd.l 0xF8A90000");usleep(100*1000);
}

/*
rootkey:00000000000000000000000000000000
public   key1 :000102030405060708090a0b0c0d0e0f
AES-ECB Decrypt:49 58 86 A2 8C 3E 7F ED 7F C8 FD 3A 1A 0C 10 92
content   key :0f0e0d0c0b0a09080706050403020100
AES-ECB Decrypt:BC FA 87 16 67 24 F9 85 3C B0 B5 3A 80 F2 7E 7D

plain    text :12345678000000000000000000000000
TDES-ECB Encrypt:
encrypt result:d613f1397e3857e241d1caf1fdad0f66
*/

static HI_U32 loadR2RLadderSessionKey(HI_HANDLE hCipher)
{
    HI_S32 Ret = 0, i = 0;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enStage = 0;
    HI_UNF_CIPHER_CTRL_S stCtrl;
    
    /* genarate private key */  
    Ret = HI_UNF_ADVCA_SetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_AES);
    Ret |= HI_UNF_ADVCA_GetR2RKeyLadderStage(&enStage);
    
    HI_DEBUG_ADVCA("================   key    ===================\n");
    if(enStage >= HI_UNF_ADVCA_KEYLADDER_LEV2)
    {
        Ret |= HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV1, g_sessionkey1);
        if (HI_SUCCESS != Ret)
        {
            return HI_FAILURE;
        }
        HI_DEBUG_ADVCA("public   key1 :");
        for ( i = 0 ; i < 16; i++ )
        {
            HI_DEBUG_ADVCA("%02x",g_sessionkey1[i]);
        }        
        HI_DEBUG_ADVCA("\n");
    }

    if(enStage >= HI_UNF_ADVCA_KEYLADDER_LEV3)
    {
        Ret |= HI_UNF_ADVCA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV2, g_sessionkey2);
        if (HI_SUCCESS != Ret)
        {
            return HI_FAILURE;
        }
        HI_DEBUG_ADVCA("public   key1 :");
        for ( i = 0 ; i < 16; i++ )
        {
            HI_DEBUG_ADVCA("%02x",g_sessionkey2[i]);
        }        
        HI_DEBUG_ADVCA("\n");
    }

    if(enStage > HI_UNF_ADVCA_KEYLADDER_LEV3)
    {
        return HI_FAILURE;
    }

    memset(&stCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
    memcpy(stCtrl.u32Key, g_contentkey, 16);
    stCtrl.bKeyByCA = HI_TRUE;
    stCtrl.enAlg = HI_UNF_CIPHER_ALG_3DES;
    stCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    stCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
    stCtrl.enKeyLen = HI_UNF_CIPHER_KEY_DES_2KEY;
    stCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
    Ret = HI_UNF_CIPHER_ConfigHandle(hCipher,&stCtrl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_ConfigHandle failed:%x\n",Ret);
        return HI_FAILURE;
    }  
    HI_DEBUG_ADVCA("content   key :");
    for ( i = 0 ; i < 16; i++ )
    {
        HI_DEBUG_ADVCA("%02x",g_contentkey[i]);
    }
    HI_DEBUG_ADVCA("\n");

    return HI_SUCCESS;
}

static HI_U32 loadMiscKeyLadderSessionKey(HI_HANDLE hCipher)
{
    HI_S32 Ret = 0, i = 0;
    HI_UNF_ADVCA_KEYLADDER_ATTR_S stKeyladderAttr;
    HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse;
    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
    HI_UNF_CIPHER_CTRL_S stCtrl;

    memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
    stKeyladderAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_ENABLE;
    stKeyladderAttr.unKlAttr.stMiscKlAttr.bEnable = HI_TRUE;
    Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKeyladderAttr);	
    
    
    memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
    stKeyladderAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_ALG;
    stKeyladderAttr.unKlAttr.stMiscKlAttr.enAlgType = HI_UNF_ADVCA_ALG_TYPE_TDES;
    Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKeyladderAttr);

    enOtpFuse = HI_UNF_ADVCA_OTP_MISC_KL_LEVEL_SEL;
    memset(&stOtpFuseAttr, 0, sizeof(stOtpFuseAttr));
    Ret = HI_UNF_ADVCA_GetOtpFuse(enOtpFuse, &stOtpFuseAttr);
    if(Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_ADVCA_GetOtpFuse get MISC Key-ladder level failed\n");
        goto DEACTIVEMISC;
    }
    printf("Misc key-level stage is:0x%x\n", stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel);
    if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV2)
    {
        printf("Level 1 is setting\n");
        memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
        stKeyladderAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_SESSION_KEY;
		stKeyladderAttr.unKlAttr.stMiscKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV1;
        memcpy(stKeyladderAttr.unKlAttr.stMiscKlAttr.u8SessionKey, g_sessionkey1, 16);
        Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKeyladderAttr);
        if (Ret != HI_SUCCESS)
        {
            goto DEACTIVEMISC;
        }
        Debug_Register();
    }
    if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV3)
    {
        printf("Level 2 is setting\n");
        memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
        stKeyladderAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_SESSION_KEY;
		stKeyladderAttr.unKlAttr.stMiscKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV2;
        memcpy(stKeyladderAttr.unKlAttr.stMiscKlAttr.u8SessionKey, g_sessionkey2, 16);
        Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKeyladderAttr);
        if (Ret != HI_SUCCESS)
        {
            goto DEACTIVEMISC;
        }
        Debug_Register();
    }
    if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV4)
    {
        printf("Level 3 is setting\n");
        memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
        stKeyladderAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_SESSION_KEY;
		stKeyladderAttr.unKlAttr.stMiscKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV3;
        memcpy(stKeyladderAttr.unKlAttr.stMiscKlAttr.u8SessionKey, g_sessionkey3, 16);
        Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKeyladderAttr);
        if (Ret != HI_SUCCESS)
        {
            goto DEACTIVEMISC;
        }

        Debug_Register();
    }
    if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV5)
    {
        printf("Level 4 is setting\n");
        memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
        stKeyladderAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_SESSION_KEY;
		stKeyladderAttr.unKlAttr.stMiscKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV4;
        memcpy(stKeyladderAttr.unKlAttr.stMiscKlAttr.u8SessionKey, g_sessionkey4, 16);
        Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKeyladderAttr);
        if (Ret != HI_SUCCESS)
        {
            goto DEACTIVEMISC;
        }

        Debug_Register();
    }

    memset(&stCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
    memcpy(stCtrl.u32Key, g_contentkey, 16);
    stCtrl.bKeyByCA = HI_TRUE;
    stCtrl.enAlg = HI_UNF_CIPHER_ALG_3DES;
    stCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    stCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
    stCtrl.enKeyLen = HI_UNF_CIPHER_KEY_DES_2KEY;
    stCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
    Ret = HI_UNF_CIPHER_ConfigHandle(hCipher,&stCtrl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_ConfigHandle failed:%x\n",Ret);
        goto DEACTIVEMISC;
    }  
    HI_DEBUG_ADVCA("content   key :");
    for ( i = 0 ; i < 16; i++ )
    {
        HI_DEBUG_ADVCA("%02x",g_contentkey[i]);
    }
    HI_DEBUG_ADVCA("\n");

DEACTIVEMISC:    
	//After use the MISC keyladder, deactive the MISC keyladder
    stKeyladderAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr = HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_ENABLE;
    stKeyladderAttr.unKlAttr.stMiscKlAttr.bEnable = HI_FALSE;
    HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_MISC, &stKeyladderAttr);
    
    return Ret;
}

static HI_U32 loadSPKeyLadderSessionKey(HI_HANDLE hCipher)
{
    HI_S32 Ret = 0, i = 0;
    HI_UNF_ADVCA_KEYLADDER_ATTR_S stKeyladderAttr;
    HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse;
    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
    HI_UNF_CIPHER_CTRL_S stCtrl;

    memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
    stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ENABLE;
    stKeyladderAttr.unKlAttr.stSPKlAttr.bEnable = HI_TRUE;
    Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);	
    
    memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
    stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ALG;
    stKeyladderAttr.unKlAttr.stSPKlAttr.enAlgType = HI_UNF_ADVCA_ALG_TYPE_TDES;
    Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);

    enOtpFuse = HI_UNF_ADVCA_OTP_SP_KL_LEVEL_SEL;
    memset(&stOtpFuseAttr, 0, sizeof(stOtpFuseAttr));
    Ret = HI_UNF_ADVCA_GetOtpFuse(enOtpFuse, &stOtpFuseAttr);
    if(Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_ADVCA_GetOtpFuse get SP Key-ladder level failed\n");
        goto DEACTIVESP;
    }
    printf("SP key-level stage is:0x%x\n", stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel);
    if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV2)
    {
        printf("Level 1 is setting\n");
        memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV1;
        memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_sessionkey1, 16);
        Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
        Debug_Register();
    }
    if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV3)
    {
        printf("Level 2 is setting\n");
        memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV2;
        memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_sessionkey2, 16);
        Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
        Debug_Register();
    }
    if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV4)
    {
        printf("Level 3 is setting\n");
        memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV3;
        memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_sessionkey3, 16);
        Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
        Debug_Register();
    }
    if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV5)
    {
        printf("Level 4 is setting\n");
        memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV4;
        memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_sessionkey4, 16);
        Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
        Debug_Register();
    }

    memset(&stCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
    memcpy(stCtrl.u32Key, g_contentkey, 16);
    stCtrl.bKeyByCA = HI_TRUE;
    stCtrl.enAlg = HI_UNF_CIPHER_ALG_3DES;
    stCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
    stCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
    stCtrl.enKeyLen = HI_UNF_CIPHER_KEY_DES_2KEY;
    stCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
    Ret = HI_UNF_CIPHER_ConfigHandle(hCipher,&stCtrl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_ConfigHandle failed:%x\n",Ret);
        goto DEACTIVESP;
    }  
    HI_DEBUG_ADVCA("content   key :");
    for ( i = 0 ; i < 16; i++ )
    {
        HI_DEBUG_ADVCA("%02x",g_contentkey[i]);
    }
    HI_DEBUG_ADVCA("\n");

DEACTIVESP:   
	//After use the SP keyladder, deactive the SP keyladder
    stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ENABLE;
    stKeyladderAttr.unKlAttr.stSPKlAttr.bEnable = HI_FALSE;
    Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
    
    return 0;
}

HI_S32 main(HI_S32 argc,HI_CHAR **argv)
{
    HI_S32 Ret,i, index;
    HI_HANDLE hCipher;
    HI_MMZ_BUF_S PlainBuf,EncrytedBuf;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;
    //HI_UNF_ADVCA_KEYLADDER_LEV_E enStage = 0;

    strcpy(PlainBuf.bufname,"plain_text");
    PlainBuf.bufsize = 0x1000;
    PlainBuf.overflow_threshold = 100;
    PlainBuf.underflow_threshold = 0;
    Ret = HI_MMZ_Malloc(&PlainBuf);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_MMZ_Malloc failed:%x\n",Ret);
        return HI_FAILURE;
    }

    strcpy(EncrytedBuf.bufname,"encrypted_text");
    EncrytedBuf.bufsize = 0x1000;
    EncrytedBuf.overflow_threshold = 100;
    EncrytedBuf.underflow_threshold = 0;
    Ret = HI_MMZ_Malloc(&EncrytedBuf);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_MMZ_Malloc failed:%x\n",Ret);
        (HI_VOID)HI_MMZ_Free(&PlainBuf);
        return HI_FAILURE;
    }

    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed:%x\n",Ret);
        goto FREE;
    }

    Ret = HI_UNF_CIPHER_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Init failed:%x\n",Ret);
        goto CA_DEINIT;
    }

    memset(&stCipherAttr, 0, sizeof(stCipherAttr));
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    Ret = HI_UNF_CIPHER_CreateHandle(&hCipher, &stCipherAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_CreateHandle failed:%x\n",Ret);
        goto CLOSE;
    }
    for(index = 0; index < 3; index ++)
    {
        if(index == 0)
        {
            HI_DEBUG_ADVCA("===============R2R_RootKey KeyLadder==========\n");
            Ret = loadR2RLadderSessionKey(hCipher);
        }
        else if(index == 1)
        {
            HI_DEBUG_ADVCA("===============MISC_RootKey KeyLadder==========\n");
            Ret = loadMiscKeyLadderSessionKey(hCipher);            
        }
        else if(index == 2)
        {
            HI_DEBUG_ADVCA("===============SP_RootKey KeyLadder==========\n");
            Ret = loadSPKeyLadderSessionKey(hCipher);
        }

        if (HI_SUCCESS != Ret)
        {
            continue;
        }
      
        /* encryption demo */  
        printf("===============encryption demo===============\n");
        memcpy(PlainBuf.user_viraddr, g_PlainText, TEXT_LEN);
        memset(EncrytedBuf.user_viraddr, 0, EncrytedBuf.bufsize);   
        Ret = HI_UNF_CIPHER_Encrypt(hCipher,PlainBuf.phyaddr, EncrytedBuf.phyaddr, TEXT_LEN);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("HI_UNF_CIPHER_Encrypt failed:%x\n",Ret);
            goto DESTROY;
        }

        printf("plain    text :");
        for ( i = 0 ; i < TEXT_LEN; i++ )
        {
            printf("%02x",PlainBuf.user_viraddr[i]);
        }
        printf("\n");
        printf("encrypt result:");
        for ( i = 0 ; i < TEXT_LEN; i++ )
        {
            printf("%02x",EncrytedBuf.user_viraddr[i]);
        }
        printf("\n");

        /* deccryption demo */  
        printf("===============decryption demo===============\n");
        memset(PlainBuf.user_viraddr, 0, PlainBuf.bufsize);
        Ret = HI_UNF_CIPHER_Decrypt(hCipher, EncrytedBuf.phyaddr, PlainBuf.phyaddr,TEXT_LEN);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("HI_UNF_CIPHER_Encrypt failed:%x\n",Ret);
            goto DESTROY;
        }

        printf("encrypted text:");
        for ( i = 0 ; i < TEXT_LEN; i++ )
        {
            printf("%02x",EncrytedBuf.user_viraddr[i]);
        }
        printf("\n");

        printf("decrypt result:");
        for ( i = 0 ; i < TEXT_LEN; i++ )
        {
            printf("%02x",PlainBuf.user_viraddr[i]);
        }
        usleep(100*1000);
        printf("\n");
    }

DESTROY:
    (HI_VOID)HI_UNF_CIPHER_DestroyHandle(hCipher);
CLOSE:
    (HI_VOID)HI_UNF_CIPHER_DeInit();
CA_DEINIT:
    (HI_VOID)HI_UNF_ADVCA_DeInit();
FREE:
    (HI_VOID)HI_MMZ_Free(&PlainBuf);
    (HI_VOID)HI_MMZ_Free(&EncrytedBuf);    
    return Ret;
}


