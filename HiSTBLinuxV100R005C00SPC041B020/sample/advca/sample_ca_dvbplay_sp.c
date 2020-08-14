/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_dvbplay.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_unf_hdmi.h"
#include "hi_unf_descrambler.h"
#include "hi_unf_advca.h"
#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"
#include "hi_adp.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "sample_ca_common.h"

HI_U32 g_TunerFreq;
HI_U32 g_TunerSrate;
HI_U32 g_TunerQam;

#ifndef DEFAULT_DVB_PORT
#define DEFAULT_DVB_PORT 0
#endif

#define INVALID_HANDLE      0xffffffff

PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;
static HI_HANDLE g_hAvplay;
static HI_HANDLE g_hNorca_CSA2_Desc,g_hAdvca_CSA2_Desc;
static HI_HANDLE g_hNorca_CSA3_Desc,g_hAdvca_CSA3_Desc;
static HI_HANDLE g_hNorca_AESIPTV_Desc,g_hAdvca_AESIPTV_Desc;
static HI_HANDLE g_hNorca_TDESCBC_Desc,g_hAdvca_TDESCBC_Desc;

/* intergration and sample stream */
static HI_U8 g_u8ClearOddKey[8] = {0xFE,0x17,0x2C,0x41,0xA2,0xE1,0xD4,0x57};
static HI_U8 g_u8ClearEvenKey[8] = {0xD4,0x22,0x82,0x78,0x1D,0x28,0x72,0xB7};
static HI_U8 g_u8EncrytedOddKey[8] = {0x44,0x2C,0xF6,0x2E,0x00,0xD0,0x77,0x96};
static HI_U8 g_u8EncrytedEvenKey[8] = {0xD8,0xDC,0x8E,0x22,0x04,0x1A,0xC4,0x9B};
#if 0
static HI_U8 g_u8ClearCwpk[16] = {0x01,0x23,0x45,0x67,
                                  0x89,0xab,0xcd,0xef,
                                  0x01,0x01,0x01,0x01,
                                  0x01,0x01,0x01,0x01};  /*0123456789abcdef0101010101010101*/
#endif
#if 0
/*intergration stream*/
/*fa57ac29c80fe95839d29a7b88d12f93*/
static HI_U8 g_u8EncrptedCwpk[16] = {0x16,0xc5,0x5b,0x89,
                                     0x92,0x0a,0xfc,0xd5,
                                     0xc6,0xfc,0x6d,0xb8,
                                     0x6d,0xc1,0xf1,0x78};
#else
/*sample stream*/
/*16c55b89920afcd5c6fc6db86dc1f178*/
//static HI_U8 g_u8EncrptedCwpk[16] = {0x80, 0xD6, 0xA3, 0x89, 0x22, 0xF9, 0x41, 0x3B, 0x26, 0xB5, 0xB1, 0x0C, 0x9D, 0xC3, 0xBF, 0x09};

//static HI_U8 sp_key[16]  = {0x00, 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
//TEST 1 rootkey:00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
//static HI_U8 g_u8EncrptedCwpk_1[16] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //TEST 1

//TEST 2: RootKey: 0x03, 0x00, 0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//(03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00)
//83 90 8C EA 6F BB 57 38 BE 18 A9 3B 5D BB 64 92
static HI_U8 g_u8EncrptedCwpk_1[16] = {0x83,0x90,0x8C,0xEA,0x6F,0xBB,0x57,0x38,0xBE,0x18,0xA9,0x3B,0x5D,0xBB,0x64,0x92}; //TEST 2
//sesSionkey1://70 F1 A2 D4 D6 C5 D0 F1 85 34 23 CC 08 D4 03 AC
static HI_U8 g_u8EncrptedCwpk_2[16] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//sessionkey2://6B 17 03 9D B7 00 CD 7F EE 41 9C D1 4E D8 E4 F7
static HI_U8 g_u8EncrptedCwpk_3[16] = {0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//sessionkey3://51 3E CD C4 D0 2F 51 1D AB 89 D5 13 47 E2 0F C9
static HI_U8 g_u8EncrptedCwpk_4[16] = {0x19, 0x3E, 0xFE, 0x7A, 0xE5, 0xAA, 0x11, 0x15, 0x83, 0xE2, 0xF5, 0xE2, 0xA4, 0xB2, 0x9B, 0xD7};
//sessionkey4://01 23 45 67 89 ab cd ef 01 01 01 01 01 01 01 01
//static HI_U8 g_u8ClearOddKey[8] = {0xFE,0x17,0x2C,0x41,0xA2,0xE1,0xD4,0x57};
//static HI_U8 g_u8ClearEvenKey[8] = {0xD4,0x22,0x82,0x78,0x1D,0x28,0x72,0xB7};
//static HI_U8 g_u8EncrytedOddKey[8] = {0x44,0x2C,0xF6,0x2E,0x00,0xD0,0x77,0x96};
//static HI_U8 g_u8EncrytedEvenKey[8] = {0xD8,0xDC,0x8E,0x22,0x04,0x1A,0xC4,0x9B};

#endif

//NoPCD3_DSC_Emi0x0001_01.02.ts
//Clear Odd key:d9 ec 13 d8 fa fd d9 d0 38 08 8f cf a0 df 69 e8
static HI_U8 g_u8_CSA3_ClearOddKey[16] = {0xd9,0xec,0x13,0xd8,0xfa,0xfd,0xd9,0xd0,0x38,0x08,0x8f,0xcf,0xa0,0xdf,0x69,0xe8};
//Clear even key:69 d7 ef 2f a8 84 52 7e f1 70 31 92 db 72 59 a6
static HI_U8 g_u8_CSA3_ClearEvenKey[16] = {0x69,0xd7,0xef,0x2f,0xa8,0x84,0x52,0x7e,0xf1,0x70,0x31,0x92,0xdb,0x72,0x59,0xa6};

//CSA3 Encryted OddKey: 68 DF 44 3D 24 02 0C 58 2E 9C 66 76 33 1F 33 07
static HI_U8 g_u8_CSA3_EncrytedOddKey[16] = {0x68,0xDF,0x44,0x3D,0x24,0x02,0x0C,0x58,0x2E,0x9C,0x66,0x76,0x33,0x1F,0x33,0x07};
//CSA3 EncrytedEvenKey: 8C 3A BD 1E F4 29 BF 35 83 BB 6A 7B 09 F7 86 F1
static HI_U8 g_u8_CSA3_EncrytedEvenKey[16] = {0x8C,0x3A,0xBD,0x1E,0xF4,0x29,0xBF,0x35,0x83,0xBB,0x6A,0x7B,0x09,0xF7,0x86,0xF1};

//NoPCDA_DSC_Emi0x0020_01.01.ts
//Clear Odd key: 98 03 52 90 2f 11 51 af bb d2 27 61 b7 bc e0 ac
static HI_U8 g_u8_AESIPTV_ClearOddKey[16] = {0x98,0x03,0x52,0x90,0x2f,0x11,0x51,0xaf,0xbb,0xd2,0x27,0x61,0xb7,0xbc,0xe0,0xac};
//Clear Even key: be 2b 15 fe ff 0c 61 6c 19 9f 8d b2 46 be 8a a0
static HI_U8 g_u8_AESIPTV_ClearEvenKey[16] = {0xbe,0x2b,0x15,0xfe,0xff,0x0c,0x61,0x6c,0x19,0x9f,0x8d,0xb2,0x46,0xbe,0x8a,0xa0};
//AES-IPTV Encryted OddKey: 08 93 19 59 D5 6D 12 7E 39 D2 50 A3 67 53 22 42
static HI_U8 g_u8_AESIPTV_EncrytedOddKey[16] = {0x08,0x93,0x19,0x59,0xD5,0x6D,0x12,0x7E,0x39,0xD2,0x50,0xA3,0x67,0x53,0x22,0x42};
//AES-IPTV EncrytedEvenKey: 3D ED C7 5F E4 6E CF 64 5B 67 5F 74 4C BC F8 E8
static HI_U8 g_u8_AESIPTV_EncrytedEvenKey[16] = {0x3D,0xED,0xC7,0x5F,0xE4,0x6E,0xCF,0x64,0x5B,0x67,0x5F,0x74,0x4C,0xBC,0xF8,0xE8};

//Clear Odd key:89 30 25 09 F2 11 15 FA BB 2D 72 16 7B CB 0E CA
static HI_U8 g_u8_TDESCBC_ClearOddKey[16] = {0x89,0x30,0x25,0x09,0xF2,0x11,0x15,0xFA,0xBB,0x2D,0x72,0x16,0x7B,0xCB,0x0E,0xCA};
//Clear Even key:EB B2 51 EF FF C0 16 C6 91 F9 D8 2B 4B 68 8A 0A
static HI_U8 g_u8_TDESCBC_ClearEvenKey[16] = {0xEB,0xB2,0x51,0xEF,0xFF,0xC0,0x16,0xC6,0x91,0xF9,0xD8,0x2B,0x4B,0x68,0x8A,0x0A};
//Encrypt Odd key:DE 5F 6B 9D 51 18 23 1C 51 61 E5 E7 5F C1 68 97
static HI_U8 g_u8_TDESCBC_EncrytedOddKey[16] = {0xDE,0x5F,0x6B,0x9D,0x51,0x18,0x23,0x1C,0x51,0x61,0xE5,0xE7,0x5F,0xC1,0x68,0x97};
//Encrypt Even key:62 63 7F A3 71 48 F2 20 C5 E3 3B 71 7E F3 3F 14
static HI_U8 g_u8_TDESCBC_EncrytedEvenKey[16] = {0x62,0x63,0x7F,0xA3,0x71,0x48,0xF2,0x20,0xC5,0xE3,0x3B,0x71,0x7E,0xF3,0x3F,0x14};


#define MAX_CMDLINE_LEN  1280
#define MAX_ARGS_COUNT   10
#define MAX_ARGS_LEN     128
static HI_CHAR g_CmdLine[MAX_CMDLINE_LEN];
static HI_S32  g_Argc;
static HI_CHAR g_Argv[MAX_ARGS_COUNT][MAX_ARGS_LEN];
static HI_BOOL g_bRun = HI_TRUE;

static HI_VOID Debug_Register(HI_VOID)
{
    usleep(100*1000);system("himd.l 0xF8A90000");usleep(100*1000);
}

HI_S32 parse_cmdline(HI_CHAR *pCmdLine,HI_S32 *pArgc,HI_CHAR Argv[MAX_ARGS_COUNT][MAX_ARGS_LEN])
{
    HI_CHAR *ptr = pCmdLine;
    int i;

    while ((*ptr == ' ') && (*ptr++ != '\0'))
    {
        ;
    }

    for (i = strlen(ptr); i > 0; i--)
    {
        if ((*(ptr + i - 1) == 0x0a) || (*(ptr + i - 1) == ' '))
        {
            *(ptr + i - 1) = '\0';
        }
        else
        {
            break;
        }
    }

    for (i = 0; i < MAX_ARGS_COUNT; i++)
    {
        int j = 0;
        while ((*ptr == ' ') && (*(++ptr) != '\0'))
        {
            ;
        }

        while ((*ptr != ' ') && (*ptr != '\0') && (j < MAX_ARGS_LEN))
        {
            Argv[i][j++] = *ptr++;
        }

        Argv[i][j] = '\0';
        if ('\0' == *ptr)
        {
            i++;
            break;
        }
    }
    *pArgc = i;

    return HI_SUCCESS;
}

HI_VOID show_usage(HI_VOID)
{
    printf("======command list=======\n"
           "help:show this command list\n"
           "getchipid\n"
           "tunerlock freq symbrate\n"
           "loadcsa2cw type\n"
           "loadcsa3cw type\n"
           "loadaesidsacw type\n"
           "loadtdescbccw type\n"
           "playbynum prognum\n"
           "playbypid vpid apid\n"
           "stopplay\n"
           "quit\n"
           "=========================\n");
}

HI_S32 play_bypid(HI_HANDLE hAvplay,HI_U32 VidPid,HI_U32 AudPid)
{
    HI_S32 Ret;
    HI_UNF_AVPLAY_STOP_OPT_S StopOpt;
    StopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    StopOpt.u32TimeoutMs = 0;
    Ret = HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &StopOpt);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Stop failed.\n");
        return HI_FAILURE;
    }

    if (VidPid != INVALID_TSPID)
    {
        Ret = HIADP_AVPlay_SetVdecAttr(hAvplay,HI_UNF_VCODEC_TYPE_MPEG2,HI_UNF_VCODEC_MODE_NORMAL);
        Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VID_PID,&VidPid);
        if (Ret != HI_SUCCESS)
        {
            HI_DEBUG_ADVCA("call HIADP_AVPlay_SetVdecAttr failed.\n");
            return Ret;
        }

        Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Start failed.\n");
            return Ret;
        }
    }

    if (AudPid != INVALID_TSPID)
    {
        Ret = HIADP_AVPlay_SetAdecAttr(hAvplay,HA_AUDIO_ID_MP3,HD_DEC_MODE_RAWPCM,1);
        Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_AUD_PID,&AudPid);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("HIADP_AVPlay_SetAdecAttr failed:%#x\n",Ret);
            return Ret;
        }

        Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Start failed.\n");
            return Ret;
        }
    }
    return HI_SUCCESS;
}

HI_S32 loadcsa2cw(HI_HANDLE hAvplay,HI_U32 CwType)
{
    HI_S32 Ret;
    HI_HANDLE hDmxVidChn,hDmxAudChn;
    HI_HANDLE hDescrambler,hCaDesc;

    /* attach the same descrambler to audio and video channel*/
    hCaDesc = (0 == CwType)?(g_hNorca_CSA2_Desc):(g_hAdvca_CSA2_Desc);
    if (INVALID_HANDLE == hCaDesc)
    {
        HI_DEBUG_ADVCA("Unsupported CwType\n");
        return HI_FAILURE;
    }

    Ret = HI_UNF_AVPLAY_GetDmxVidChnHandle(hAvplay,&hDmxVidChn);
    Ret |= HI_UNF_AVPLAY_GetDmxAudChnHandle(hAvplay,&hDmxAudChn);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_AVPLAY_GetDmxChnHandle failed:%x\n",Ret);
        return Ret;
    }

    Ret = HI_UNF_DMX_GetDescramblerKeyHandle(hDmxVidChn,&hDescrambler);
    if (HI_SUCCESS != Ret)
    {
        if(HI_ERR_DMX_NOATTACH_KEY == Ret)
        {
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxVidChn);
        }
        else
        {
            HI_DEBUG_ADVCA("HI_UNF_DMX_GetDescramblerKeyHandle failed:%x\n",Ret);
            return Ret;
        }
    }
    else
    {
        if ((hDescrambler != hCaDesc))
        {
            (HI_VOID)HI_UNF_DMX_DetachDescrambler(hDescrambler,hDmxVidChn);
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxVidChn);
        }
    }

    Ret = HI_UNF_DMX_GetDescramblerKeyHandle(hDmxAudChn,&hDescrambler);
    if (HI_SUCCESS != Ret)
    {
        if(HI_ERR_DMX_NOATTACH_KEY == Ret)
        {
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxAudChn);
        }
        else
        {
            HI_DEBUG_ADVCA("HI_UNF_DMX_GetDescramblerKeyHandle failed:%x\n",Ret);
            return Ret;
        }
    }
    else
    {
        if ((hDescrambler != hCaDesc))
        {
            (HI_VOID)HI_UNF_DMX_DetachDescrambler(hDescrambler,hDmxAudChn);
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxAudChn);
        }
    }

    /* load cw*/
    if (0 == CwType)
    {
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hCaDesc,g_u8ClearOddKey);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hCaDesc,g_u8ClearEvenKey);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerKey failed\n");
            return HI_FAILURE;
        }
    }
    else
    {
        HI_UNF_ADVCA_KEYLADDER_ATTR_S stKeyladderAttr;
        HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse;
        HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;

        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ENABLE;
        stKeyladderAttr.unKlAttr.stSPKlAttr.bEnable = HI_TRUE;
        Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);

        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ALG;
        stKeyladderAttr.unKlAttr.stSPKlAttr.enAlgType = HI_UNF_ADVCA_ALG_TYPE_TDES;
        Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);

        enOtpFuse = HI_UNF_ADVCA_OTP_SP_KL_LEVEL_SEL;
        memset(&stOtpFuseAttr, 0, sizeof(stOtpFuseAttr));
        Ret = HI_UNF_ADVCA_GetOtpFuse(enOtpFuse, &stOtpFuseAttr);
        if(Ret != HI_SUCCESS)
        {
            HI_DEBUG_ADVCA("call HI_UNF_ADVCA_GetOtpFuse get SP Key-ladder level failed\n");
            return HI_FAILURE;
        }
        printf("SP key-level stage is:0x%x\n", stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel);
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV2)
        {
            printf("Level 1 is setting\n");
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV1;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_1, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV3)
        {
            printf("Level 2 is setting\n");
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV2;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_2, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV4)
        {
            printf("Level 3 is setting\n");
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV3;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_3, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV5)
        {
            printf("Level 4 is setting\n");
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV4;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_4, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }

        memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_DSC_MODE;
        stKeyladderAttr.unKlAttr.stSPKlAttr.enDscMode = HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_CSA2;
        Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
        Debug_Register();

        Ret |= HI_UNF_DMX_SetDescramblerOddKey(hCaDesc,g_u8EncrytedOddKey);
        Debug_Register();
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hCaDesc,g_u8EncrytedEvenKey);
        Debug_Register();
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerEvenKey failed\n");
            return HI_FAILURE;
        }

		//After use the SP keyladder, deactive the SP keyladder
        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ENABLE;
        stKeyladderAttr.unKlAttr.stSPKlAttr.bEnable = HI_FALSE;
        Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
    }

    return HI_SUCCESS;
}

HI_S32 loadcsa3cw(HI_HANDLE hAvplay,HI_U32 CwType)
{
    HI_S32 Ret;
    HI_HANDLE hDmxVidChn,hDmxAudChn;
    HI_HANDLE hDescrambler,hCaDesc;

    /* attach the same descrambler to audio and video channel*/
    hCaDesc = (0 == CwType)?(g_hNorca_CSA3_Desc):(g_hAdvca_CSA3_Desc);
    if (INVALID_HANDLE == hCaDesc)
    {
        HI_DEBUG_ADVCA("Unsupported CwType\n");
        return HI_FAILURE;
    }

    Ret = HI_UNF_AVPLAY_GetDmxVidChnHandle(hAvplay,&hDmxVidChn);
    Ret |= HI_UNF_AVPLAY_GetDmxAudChnHandle(hAvplay,&hDmxAudChn);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_AVPLAY_GetDmxChnHandle failed:%x\n",Ret);
        return Ret;
    }

    Ret = HI_UNF_DMX_GetDescramblerKeyHandle(hDmxVidChn,&hDescrambler);
    if (HI_SUCCESS != Ret)
    {
        if(HI_ERR_DMX_NOATTACH_KEY == Ret)
        {
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxVidChn);
        }
        else
        {
            HI_DEBUG_ADVCA("HI_UNF_DMX_GetDescramblerKeyHandle failed:%x\n",Ret);
            return Ret;
        }
    }
    else
    {
        if ((hDescrambler != hCaDesc))
        {
            (HI_VOID)HI_UNF_DMX_DetachDescrambler(hDescrambler,hDmxVidChn);
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxVidChn);
        }
    }

    Ret = HI_UNF_DMX_GetDescramblerKeyHandle(hDmxAudChn,&hDescrambler);
    if (HI_SUCCESS != Ret)
    {
        if(HI_ERR_DMX_NOATTACH_KEY == Ret)
        {
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxAudChn);
        }
        else
        {
            HI_DEBUG_ADVCA("HI_UNF_DMX_GetDescramblerKeyHandle failed:%x\n",Ret);
            return Ret;
        }
    }
    else
    {
        if ((hDescrambler != hCaDesc))
        {
            (HI_VOID)HI_UNF_DMX_DetachDescrambler(hDescrambler,hDmxAudChn);
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxAudChn);
        }
    }

    /* load cw*/
    if (0 == CwType)
    {
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hCaDesc,g_u8_CSA3_ClearOddKey);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hCaDesc,g_u8_CSA3_ClearEvenKey);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerKey failed\n");
            return HI_FAILURE;
        }
    }
    else
    {
        HI_UNF_ADVCA_KEYLADDER_ATTR_S stKeyladderAttr;
        HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse;
        HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;

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
            return HI_FAILURE;
        }
        printf("SP key-level stage is:0x%x\n", stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel);
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV2)
        {
            printf("Level 1 is setting\n");
            memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV1;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_1, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV3)
        {
            printf("Level 2 is setting\n");
            memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV2;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_2, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV4)
        {
            printf("Level 3 is setting\n");
            memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV3;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_3, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV5)
        {
            printf("Level 4 is setting\n");
            memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV4;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_4, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }

        memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_DSC_MODE;
        stKeyladderAttr.unKlAttr.stSPKlAttr.enDscMode = HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_CSA3;
        Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
        Debug_Register();

        Ret |= HI_UNF_DMX_SetDescramblerOddKey(hCaDesc,g_u8_CSA3_EncrytedOddKey);
        Debug_Register();
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hCaDesc,g_u8_CSA3_EncrytedEvenKey);
        Debug_Register();
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerEvenKey failed\n");
            return HI_FAILURE;
        }

		//After use the SP keyladder, deactive the SP keyladder
        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ENABLE;
        stKeyladderAttr.unKlAttr.stSPKlAttr.bEnable = HI_FALSE;
        Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
    }

    return HI_SUCCESS;
}

HI_S32 loadaesidsacw(HI_HANDLE hAvplay,HI_U32 CwType)
{
    HI_S32 Ret;
    HI_HANDLE hDmxVidChn,hDmxAudChn;
    HI_HANDLE hDescrambler,hCaDesc;

    /* attach the same descrambler to audio and video channel*/
    hCaDesc = (0 == CwType)?(g_hNorca_AESIPTV_Desc):(g_hAdvca_AESIPTV_Desc);
    if (INVALID_HANDLE == hCaDesc)
    {
        HI_DEBUG_ADVCA("Unsupported CwType\n");
        return HI_FAILURE;
    }

    Ret = HI_UNF_AVPLAY_GetDmxVidChnHandle(hAvplay,&hDmxVidChn);
    Ret |= HI_UNF_AVPLAY_GetDmxAudChnHandle(hAvplay,&hDmxAudChn);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_AVPLAY_GetDmxChnHandle failed:%x\n",Ret);
        return Ret;
    }

    Ret = HI_UNF_DMX_GetDescramblerKeyHandle(hDmxVidChn,&hDescrambler);
    if (HI_SUCCESS != Ret)
    {
        if(HI_ERR_DMX_NOATTACH_KEY == Ret)
        {
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxVidChn);
        }
        else
        {
            HI_DEBUG_ADVCA("HI_UNF_DMX_GetDescramblerKeyHandle failed:%x\n",Ret);
            return Ret;
        }
    }
    else
    {
        if ((hDescrambler != hCaDesc))
        {
            (HI_VOID)HI_UNF_DMX_DetachDescrambler(hDescrambler,hDmxVidChn);
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxVidChn);
        }
    }

    Ret = HI_UNF_DMX_GetDescramblerKeyHandle(hDmxAudChn,&hDescrambler);
    if (HI_SUCCESS != Ret)
    {
        if(HI_ERR_DMX_NOATTACH_KEY == Ret)
        {
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxAudChn);
        }
        else
        {
            HI_DEBUG_ADVCA("HI_UNF_DMX_GetDescramblerKeyHandle failed:%x\n",Ret);
            return Ret;
        }
    }
    else
    {
        if ((hDescrambler != hCaDesc))
        {
            (HI_VOID)HI_UNF_DMX_DetachDescrambler(hDescrambler,hDmxAudChn);
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxAudChn);
        }
    }

    /* load cw*/
    if (0 == CwType)
    {
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hCaDesc,g_u8_AESIPTV_ClearOddKey);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hCaDesc,g_u8_AESIPTV_ClearEvenKey);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerKey failed\n");
            return HI_FAILURE;
        }
    }
    else
    {
        HI_UNF_ADVCA_KEYLADDER_ATTR_S stKeyladderAttr;
        HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse;
        HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;

        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ENABLE;
        stKeyladderAttr.unKlAttr.stSPKlAttr.bEnable = HI_TRUE;
        Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);

        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ALG;
        stKeyladderAttr.unKlAttr.stSPKlAttr.enAlgType = HI_UNF_ADVCA_ALG_TYPE_TDES;
        Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);

        enOtpFuse = HI_UNF_ADVCA_OTP_SP_KL_LEVEL_SEL;
        memset(&stOtpFuseAttr, 0, sizeof(stOtpFuseAttr));
        Ret = HI_UNF_ADVCA_GetOtpFuse(enOtpFuse, &stOtpFuseAttr);
        if(Ret != HI_SUCCESS)
        {
            HI_DEBUG_ADVCA("call HI_UNF_ADVCA_GetOtpFuse get SP Key-ladder level failed\n");
            return HI_FAILURE;
        }
        printf("SP key-level stage is:0x%x\n", stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel);
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV2)
        {
            printf("Level 1 is setting\n");
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV1;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_1, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV3)
        {
            printf("Level 2 is setting\n");
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV2;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_2, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV4)
        {
            printf("Level 3 is setting\n");
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV3;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_3, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV5)
        {
            printf("Level 4 is setting\n");
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV4;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_4, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }

        memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_DSC_MODE;
        stKeyladderAttr.unKlAttr.stSPKlAttr.enDscMode = HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_AES_CBC_IDSA;
        Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
        Debug_Register();

        Ret |= HI_UNF_DMX_SetDescramblerOddKey(hCaDesc,g_u8_AESIPTV_EncrytedOddKey);
        Debug_Register();
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hCaDesc,g_u8_AESIPTV_EncrytedEvenKey);
        Debug_Register();
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerEvenKey failed\n");
            return HI_FAILURE;
        }

		//After use the SP keyladder, deactive the SP keyladder
        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ENABLE;
        stKeyladderAttr.unKlAttr.stSPKlAttr.bEnable = HI_FALSE;
        Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
    }

    return HI_SUCCESS;
}

HI_S32 loadtdescbccw(HI_HANDLE hAvplay,HI_U32 CwType)
{
    HI_S32 Ret;
    HI_HANDLE hDmxVidChn,hDmxAudChn;
    HI_HANDLE hDescrambler,hCaDesc;

    /* attach the same descrambler to audio and video channel*/
    hCaDesc = (0 == CwType)?(g_hNorca_TDESCBC_Desc):(g_hAdvca_TDESCBC_Desc);
    if (INVALID_HANDLE == hCaDesc)
    {
        HI_DEBUG_ADVCA("Unsupported CwType\n");
        return HI_FAILURE;
    }

    Ret = HI_UNF_AVPLAY_GetDmxVidChnHandle(hAvplay,&hDmxVidChn);
    Ret |= HI_UNF_AVPLAY_GetDmxAudChnHandle(hAvplay,&hDmxAudChn);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_AVPLAY_GetDmxChnHandle failed:%x\n",Ret);
        return Ret;
    }

    Ret = HI_UNF_DMX_GetDescramblerKeyHandle(hDmxVidChn,&hDescrambler);
    if (HI_SUCCESS != Ret)
    {
        if(HI_ERR_DMX_NOATTACH_KEY == Ret)
        {
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxVidChn);
        }
        else
        {
            HI_DEBUG_ADVCA("HI_UNF_DMX_GetDescramblerKeyHandle failed:%x\n",Ret);
            return Ret;
        }
    }
    else
    {
        if ((hDescrambler != hCaDesc))
        {
            (HI_VOID)HI_UNF_DMX_DetachDescrambler(hDescrambler,hDmxVidChn);
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxVidChn);
        }
    }

    Ret = HI_UNF_DMX_GetDescramblerKeyHandle(hDmxAudChn,&hDescrambler);
    if (HI_SUCCESS != Ret)
    {
        if(HI_ERR_DMX_NOATTACH_KEY == Ret)
        {
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxAudChn);
        }
        else
        {
            HI_DEBUG_ADVCA("HI_UNF_DMX_GetDescramblerKeyHandle failed:%x\n",Ret);
            return Ret;
        }
    }
    else
    {
        if ((hDescrambler != hCaDesc))
        {
            (HI_VOID)HI_UNF_DMX_DetachDescrambler(hDescrambler,hDmxAudChn);
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxAudChn);
        }
    }

    /* load cw*/
    if (0 == CwType)
    {
        HI_U8 u8IVKey[16] = {0};
        Ret = HI_UNF_DMX_SetDescramblerEvenIVKey(hCaDesc,u8IVKey);
        Ret |= HI_UNF_DMX_SetDescramblerOddIVKey(hCaDesc,u8IVKey);
        Ret |= HI_UNF_DMX_SetDescramblerOddKey(hCaDesc,g_u8_TDESCBC_ClearOddKey);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hCaDesc,g_u8_TDESCBC_ClearEvenKey);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerKey failed\n");
            return HI_FAILURE;
        }
    }
    else
    {
        HI_UNF_ADVCA_KEYLADDER_ATTR_S stKeyladderAttr;
        HI_UNF_ADVCA_OTP_FUSE_E enOtpFuse;
        HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;

        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ENABLE;
        stKeyladderAttr.unKlAttr.stSPKlAttr.bEnable = HI_TRUE;
        Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);

        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ALG;
        stKeyladderAttr.unKlAttr.stSPKlAttr.enAlgType = HI_UNF_ADVCA_ALG_TYPE_TDES;
        Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);

        enOtpFuse = HI_UNF_ADVCA_OTP_SP_KL_LEVEL_SEL;
        memset(&stOtpFuseAttr, 0, sizeof(stOtpFuseAttr));
        Ret = HI_UNF_ADVCA_GetOtpFuse(enOtpFuse, &stOtpFuseAttr);
        if(Ret != HI_SUCCESS)
        {
            HI_DEBUG_ADVCA("call HI_UNF_ADVCA_GetOtpFuse get SP Key-ladder level failed\n");
            return HI_FAILURE;
        }
        printf("SP key-level stage is:0x%x\n", stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel);
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV2)
        {
            printf("Level 1 is setting\n");
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV1;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_1, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV3)
        {
            printf("Level 2 is setting\n");
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV2;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_2, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV4)
        {
            printf("Level 3 is setting\n");
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV3;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_3, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }
        if(stOtpFuseAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel >= HI_UNF_ADVCA_KEYLADDER_LEV5)
        {
            printf("Level 4 is setting\n");
            stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY;
    		stKeyladderAttr.unKlAttr.stSPKlAttr.enStage = HI_UNF_ADVCA_KEYLADDER_LEV4;
            memcpy(stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, g_u8EncrptedCwpk_4, 16);
            Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
            Debug_Register();
        }

        memset(&stKeyladderAttr, 0, sizeof(HI_UNF_ADVCA_KEYLADDER_ATTR_S));
        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_DSC_MODE;
        stKeyladderAttr.unKlAttr.stSPKlAttr.enDscMode = HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_TDES_CBC;
        Ret |= HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
        Debug_Register();

        Ret |= HI_UNF_DMX_SetDescramblerOddKey(hCaDesc,g_u8_TDESCBC_EncrytedOddKey);
        Debug_Register();
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hCaDesc,g_u8_TDESCBC_EncrytedEvenKey);
        Debug_Register();
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerEvenKey failed\n");
            return HI_FAILURE;
        }

		//After use the SP keyladder, deactive the SP keyladder
        stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ENABLE;
        stKeyladderAttr.unKlAttr.stSPKlAttr.bEnable = HI_FALSE;
        Ret = HI_UNF_ADVCA_SetKeyLadderAttr(HI_UNF_ADVCA_KEYLADDER_SP, &stKeyladderAttr);
    }

    return HI_SUCCESS;
}
           HI_S32 run_cmdline(HI_S32 argc,HI_CHAR argv[MAX_ARGS_COUNT][MAX_ARGS_LEN])
{
    HI_S32 Ret;
    if (!strcmp(argv[0],"help"))
    {
        show_usage();
    }
    else if (!strcmp(argv[0],"getchipid"))
    {
        HI_U32 ChipId;
        Ret = HI_UNF_ADVCA_GetChipId(&ChipId);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("HI_UNF_ADVCA_GetChipId failed:%x\n",Ret);
            return HI_FAILURE;
        }
        printf("CHIPID:0x%x\n",ChipId);
    }
    else if (!strcmp(argv[0],"tunerlock"))
    {
        HI_U32 Freq,Srate,Qam;
        if(argc < 3)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        Freq  = strtol(argv[1],NULL,0);
        Srate = strtol(argv[2],NULL,0);
        Qam   = 64;
        Ret = HIADP_Tuner_Connect(TUNER_USE, Freq,Srate,Qam);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("tuner lock failed\n");
            return HI_FAILURE;
        }
        else
        {
            HI_DEBUG_ADVCA("tuner lock success\n");
        }

        Ret = HIADP_Search_GetAllPmt(0,&g_pProgTbl);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HIADP_Search_GetAllPmt failed\n");
            return HI_FAILURE;
        }
    }
    else if (!strcmp(argv[0],"loadcsa2cw"))
    {
        HI_U32 CwType;
        if (argc < 2)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        CwType = strtol(argv[1],NULL,0);
        Ret = loadcsa2cw(g_hAvplay,CwType);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("load csa2 cw failed\n");
        }
    }
    else if (!strcmp(argv[0],"loadcsa3cw"))
    {
        HI_U32 CwType;
        if (argc < 2)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        CwType = strtol(argv[1],NULL,0);
        Ret = loadcsa3cw(g_hAvplay,CwType);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("load csa3 cw failed\n");
        }
    }
    else if (!strcmp(argv[0],"loadaesidsacw"))
    {
        HI_U32 CwType;
        if (argc < 2)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        CwType = strtol(argv[1],NULL,0);
        Ret = loadaesidsacw(g_hAvplay,CwType);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("load aes-cbc IDSA cw failed\n");
        }
    }
    else if (!strcmp(argv[0],"loadtdescbccw"))
    {
        HI_U32 CwType;
        if (argc < 2)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        CwType = strtol(argv[1],NULL,0);
        Ret = loadtdescbccw(g_hAvplay,CwType);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("load tdes-cbc cw failed\n");
        }
    }
    else if (!strcmp(argv[0],"playbypid"))
    {
        HI_U32 VidPid,AudPid;
        if (argc < 3)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        VidPid = strtol(argv[1],NULL,0);
        AudPid = strtol(argv[2],NULL,0);

        Ret = play_bypid(g_hAvplay,VidPid,AudPid);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("play prog failed:%x\n",Ret);
            return HI_FAILURE;
        }
    }
    else if (!strcmp(argv[0],"playbynum"))
    {
        HI_U32 ProgNum;
        if (HI_NULL == g_pProgTbl || 0 == g_pProgTbl->prog_num)
        {
            printf("run tunerlock first\n");
            return HI_FAILURE;
        }

        if (argc < 2)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        ProgNum = strtol(argv[1],NULL,0);
        Ret = HIADP_AVPlay_PlayProg(g_hAvplay,g_pProgTbl,ProgNum - 1,HI_TRUE);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("play prog failed:%x\n",Ret);
            return HI_FAILURE;
        }
    }
    else if (!strcmp(argv[0],"stopplay"))
    {
        HI_UNF_AVPLAY_STOP_OPT_S StopOpt;
        StopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
        StopOpt.u32TimeoutMs = 0;
        Ret = HI_UNF_AVPLAY_Stop(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &StopOpt);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Stop failed.\n");
            return Ret;
        }
    }
    else if (!strcmp(argv[0],"quit"))
    {
        g_bRun = HI_FALSE;
    }
    else
    {
        HI_DEBUG_ADVCA("invalid command\n");
        show_usage();
    }
    return HI_SUCCESS;
}

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32                  Ret;
    HI_HANDLE               hWin;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    HI_UNF_DMX_DESCRAMBLER_ATTR_S stNorcaDescAttr,stAdvcaDescAttr;

    HI_HANDLE hTrack;       //++
    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;       //++

    (HI_VOID)HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_ADVCA_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Snd_Init failed.\n");
        goto CA_DEINIT;
    }

    Ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_1080i_50);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Disp_DeInit failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,&hWin);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_VO_Init failed.\n");
        (HI_VOID)HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
//    Ret |= HI_UNF_DMX_AttachTSPort(0,0);
    //Ret |= HI_UNF_DMX_AttachTSPort(0, DEFAULT_DVB_PORT);
    Ret |= HIADP_DMX_AttachTSPort(0,TUNER_USE);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Demux_Init failed.\n");
        goto VO_DEINIT;
    }
    //CSA2.0
    memset(&stNorcaDescAttr,0,sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));
    stNorcaDescAttr.enCaType = HI_UNF_DMX_CA_NORMAL;
    stNorcaDescAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_CSA2;
    Ret = HI_UNF_DMX_CreateDescramblerExt(0,&stNorcaDescAttr,&g_hNorca_CSA2_Desc);
    if (HI_SUCCESS != Ret)
    {
        g_hNorca_CSA2_Desc = INVALID_HANDLE;
    }
    //CSA2.0
    memset(&stAdvcaDescAttr,0,sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));
    stAdvcaDescAttr.enCaType = HI_UNF_DMX_CA_ADVANCE;
    stAdvcaDescAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_CSA2;
    Ret = HI_UNF_DMX_CreateDescramblerExt(0,&stAdvcaDescAttr,&g_hAdvca_CSA2_Desc);
    if (HI_SUCCESS != Ret)
    {
        g_hAdvca_CSA2_Desc = INVALID_HANDLE;
    }
    //CSA3.0
    memset(&stNorcaDescAttr,0,sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));
    stNorcaDescAttr.enCaType = HI_UNF_DMX_CA_NORMAL;
    stNorcaDescAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_CSA3;
    Ret = HI_UNF_DMX_CreateDescramblerExt(0,&stNorcaDescAttr,&g_hNorca_CSA3_Desc);
    if (HI_SUCCESS != Ret)
    {
        g_hNorca_CSA3_Desc = INVALID_HANDLE;
    }
    //CSA3.0
    memset(&stAdvcaDescAttr,0,sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));
    stAdvcaDescAttr.enCaType = HI_UNF_DMX_CA_ADVANCE;
    stAdvcaDescAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_CSA3;
    Ret = HI_UNF_DMX_CreateDescramblerExt(0,&stAdvcaDescAttr,&g_hAdvca_CSA3_Desc);
    if (HI_SUCCESS != Ret)
    {
        g_hAdvca_CSA3_Desc = INVALID_HANDLE;
    }
    //AES-IPTV
    memset(&stNorcaDescAttr,0,sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));
    stNorcaDescAttr.enCaType = HI_UNF_DMX_CA_NORMAL;
    stNorcaDescAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_AES_IPTV;
    Ret = HI_UNF_DMX_CreateDescramblerExt(0,&stNorcaDescAttr,&g_hNorca_AESIPTV_Desc);
    if (HI_SUCCESS != Ret)
    {
        g_hNorca_AESIPTV_Desc = INVALID_HANDLE;
    }
    //AES-IPTV
    memset(&stAdvcaDescAttr,0,sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));
    stAdvcaDescAttr.enCaType = HI_UNF_DMX_CA_ADVANCE;
    stAdvcaDescAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_AES_IPTV;
    Ret = HI_UNF_DMX_CreateDescramblerExt(0,&stAdvcaDescAttr,&g_hAdvca_AESIPTV_Desc);
    if (HI_SUCCESS != Ret)
    {
        g_hAdvca_AESIPTV_Desc = INVALID_HANDLE;
    }
    //TDES_CBC
    memset(&stNorcaDescAttr,0,sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));
    stNorcaDescAttr.enCaType = HI_UNF_DMX_CA_NORMAL;
    stNorcaDescAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_TDES_CBC;
    Ret = HI_UNF_DMX_CreateDescramblerExt(0,&stNorcaDescAttr,&g_hNorca_TDESCBC_Desc);
    if (HI_SUCCESS != Ret)
    {
        g_hNorca_TDESCBC_Desc = INVALID_HANDLE;
    }
    //TDES_CBC
    memset(&stAdvcaDescAttr,0,sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));
    stAdvcaDescAttr.enCaType = HI_UNF_DMX_CA_ADVANCE;
    stAdvcaDescAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_TDES_CBC;
    Ret = HI_UNF_DMX_CreateDescramblerExt(0,&stAdvcaDescAttr,&g_hAdvca_TDESCBC_Desc);
    if (HI_SUCCESS != Ret)
    {
        g_hAdvca_TDESCBC_Desc = INVALID_HANDLE;
    }

    Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Demux_Init failed.\n");
        goto DES_DESTROY;
    }

#if 0
    Ret = HIADP_Tuner_Connect(TUNER_USE,g_TunerFreq,g_TunerSrate,g_TunerQam);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Tuner_Connect failed.\n");
        goto TUNER_DEINIT;
    }
#endif
    HIADP_Search_Init();
#if 0
    Ret = HIADP_Search_GetAllPmt(0,&g_pProgTbl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Search_GetAllPmt failed\n");
        goto PSISI_FREE;
    }
#endif
    Ret = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Init failed.\n");
        goto PSISI_FREE;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &g_hAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    Ret |= HI_UNF_AVPLAY_ChnOpen(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto CHN_CLOSE;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, g_hAvplay);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_VO_AttachWindow failed.\n");
        goto CHN_CLOSE;
    }
    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    }
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrack);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }

//    Ret = HI_UNF_SND_Attach(HI_UNF_SND_0, g_hAvplay, HI_UNF_SND_MIX_TYPE_MASTER, 100);
    Ret = HI_UNF_SND_Attach(hTrack, g_hAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_SND_Attach failed.\n");
        goto TRACK_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(g_hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
    Ret = HI_UNF_AVPLAY_SetAttr(g_hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    show_usage();
    while(g_bRun)
    {
        HI_CHAR *pCmdLine,chRet;
        HI_DEBUG_ADVCA("\n>");
        pCmdLine = fgets(g_CmdLine,MAX_CMDLINE_LEN,stdin);
        if (HI_NULL == pCmdLine)
        {
            show_usage();
            continue;
        }

        chRet = parse_cmdline(pCmdLine,&g_Argc,g_Argv);
        if (HI_SUCCESS != chRet || 0 == g_Argc)
        {
            show_usage();
            continue;
        }

        (HI_VOID)run_cmdline(g_Argc,g_Argv);
    }

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;
    (HI_VOID)HI_UNF_AVPLAY_Stop(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);

SND_DETACH:
    (HI_VOID)HI_UNF_SND_Detach(hTrack, g_hAvplay);

TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrack);

WIN_DETACH:
    (HI_VOID)HI_UNF_VO_SetWindowEnable(hWin,HI_FALSE);
    (HI_VOID)HI_UNF_VO_DetachWindow(hWin, g_hAvplay);
CHN_CLOSE:
    (HI_VOID)HI_UNF_AVPLAY_ChnClose(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    (HI_VOID)HI_UNF_AVPLAY_Destroy(g_hAvplay);

AVPLAY_DEINIT:
    (HI_VOID)HI_UNF_AVPLAY_DeInit();

PSISI_FREE:
    (HI_VOID)HIADP_Search_FreeAllPmt(g_pProgTbl);
    HIADP_Search_DeInit();
    HIADP_Tuner_DeInit();

DES_DESTROY:
    (HI_VOID)HI_UNF_DMX_DestroyDescrambler(g_hNorca_CSA2_Desc);
    (HI_VOID)HI_UNF_DMX_DestroyDescrambler(g_hAdvca_CSA2_Desc);
    (HI_VOID)HI_UNF_DMX_DestroyDescrambler(g_hNorca_CSA3_Desc);
    (HI_VOID)HI_UNF_DMX_DestroyDescrambler(g_hAdvca_CSA3_Desc);
    (HI_VOID)HI_UNF_DMX_DestroyDescrambler(g_hNorca_AESIPTV_Desc);
    (HI_VOID)HI_UNF_DMX_DestroyDescrambler(g_hAdvca_AESIPTV_Desc);
    (HI_VOID)HI_UNF_DMX_DestroyDescrambler(g_hNorca_TDESCBC_Desc);
    (HI_VOID)HI_UNF_DMX_DestroyDescrambler(g_hAdvca_TDESCBC_Desc);

    (HI_VOID)HI_UNF_DMX_DetachTSPort(0);
    (HI_VOID)HI_UNF_DMX_DeInit();

VO_DEINIT:
    (HI_VOID)HI_UNF_VO_DestroyWindow(hWin);
    (HI_VOID)HIADP_VO_DeInit();

DISP_DEINIT:
    (HI_VOID)HIADP_Disp_DeInit();

SND_DEINIT:
    (HI_VOID)HIADP_Snd_DeInit();
CA_DEINIT:
    (HI_VOID)HI_UNF_ADVCA_DeInit();
SYS_DEINIT:
    (HI_VOID)HI_SYS_DeInit();

    return Ret;
}


