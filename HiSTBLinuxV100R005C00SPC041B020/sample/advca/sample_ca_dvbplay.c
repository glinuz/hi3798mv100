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
static HI_HANDLE g_hNorcaDesc,g_hAdvcaDesc;
static HI_HANDLE g_hNorca_CSA3_Desc,g_hAdvca_CSA3_Desc;

extern HI_S32 HI_UNF_ADVCA_SetCSA3SessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key);

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
static HI_U8 g_u8EncrptedCwpk[16] = {0xfa,0x57,0xac,0x29,
                                     0xc8,0x0f,0xe9,0x58,
                                     0x39,0xd2,0x9a,0x7b,
                                     0x88,0xd1,0x2f,0x93};
#endif
#if 0
/*sample stream*/
/*16c55b89920afcd5c6fc6db86dc1f178*/
static HI_U8 g_u8EncrptedCwpk[16] = {0x16,0xc5,0x5b,0x89,
                                     0x92,0x0a,0xfc,0xd5,
                                     0xc6,0xfc,0x6d,0xb8,
                                     0x6d,0xc1,0xf1,0x78};
#endif

#if 1
//rootkey: 0xff, 0x00, 0x00, ....
static HI_U8 g_u8EncrptedCwpk[16] =
   {0x3E,0xEC,0x20,0xC8,0xB2,0x13,0x27,0xAB,
    0xF8,0xD7,0xEE,0xFB,0xF8,0x42,0x68,0x0C};
#endif

//csa3_root_key:04 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//g_u8ClearCwpk 01 23 45 67 89 ab cd ef 01 01 01 01 01 01 01 01
//g_u8_CSA3_EncrptedCwpk: 82 18 65 d2 11 2b b7 0e b3 e7 34 05 4a 08 25 ea
static HI_U8 g_u8_CSA3_EncrptedCwpk[16] =
{0x82,0x18,0x65,0xd2,0x11,0x2b,0xb7,0x0e,0xb3,0xe7,0x34,0x05,0x4a,0x08,0x25,0xea};
//Clear Odd key:d9 ec 13 d8 fa fd d9 d0 38 08 8f cf a0 df 69 e8
static HI_U8 g_u8_CSA3_ClearOddKey[16] = {0xd9,0xec,0x13,0xd8,0xfa,0xfd,0xd9,0xd0,0x38,0x08,0x8f,0xcf,0xa0,0xdf,0x69,0xe8};
//Clear even key:69 d7 ef 2f a8 84 52 7e f1 70 31 92 db 72 59 a6
static HI_U8 g_u8_CSA3_ClearEvenKey[16] = {0x69,0xd7,0xef,0x2f,0xa8,0x84,0x52,0x7e,0xf1,0x70,0x31,0x92,0xdb,0x72,0x59,0xa6};
//Encrypted odd key: 68 DF 44 3D 24 02 0C 58 2E 9C 66 76 33 1F 33 07
static HI_U8 g_u8_CSA3_EncrytedOddKey[16] = {0x68,0xDF,0x44,0x3D,0x24,0x02,0x0C,0x58,0x2E,0x9C,0x66,0x76,0x33,0x1F,0x33,0x07};
//Encrypted even key: 8C 3A BD 1E F4 29 BF 35 83 BB 6A 7B 09 F7 86 F1
static HI_U8 g_u8_CSA3_EncrytedEvenKey[16] = {0x8C,0x3A,0xBD,0x1E,0xF4,0x29,0xBF,0x35,0x83,0xBB,0x6A,0x7B,0x09,0xF7,0x86,0xF1};


#define MAX_CMDLINE_LEN  1280
#define MAX_ARGS_COUNT   10
#define MAX_ARGS_LEN     128
static HI_CHAR g_CmdLine[MAX_CMDLINE_LEN];
static HI_S32  g_Argc;
static HI_CHAR g_Argv[MAX_ARGS_COUNT][MAX_ARGS_LEN];
static HI_BOOL g_bRun = HI_TRUE;

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
           "loadcw type\n"
           "loadcsa3 type\n"
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


HI_S32 load_csa3(HI_HANDLE hAvplay,HI_U32 CwType)
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
        Ret = HI_UNF_ADVCA_SetDVBAlg(HI_UNF_ADVCA_ALG_TYPE_TDES);
        /* first level */
        Ret |= HI_UNF_ADVCA_SetCSA3SessionKey(HI_UNF_ADVCA_KEYLADDER_LEV1,g_u8_CSA3_EncrptedCwpk);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_ADVCA_SetDVBSessionKey failed\n");
            return HI_FAILURE;
        }
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hCaDesc,g_u8_CSA3_EncrytedOddKey);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hCaDesc,g_u8_CSA3_EncrytedEvenKey);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerEvenKey failed\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 load_cw(HI_HANDLE hAvplay,HI_U32 CwType)
{
    HI_S32 Ret;
    HI_HANDLE hDmxVidChn,hDmxAudChn;
    HI_HANDLE hDescrambler,hCaDesc;

    /* attach the same descrambler to audio and video channel*/
    hCaDesc = (0 == CwType)?(g_hNorcaDesc):(g_hAdvcaDesc);
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
        Ret = HI_UNF_ADVCA_SetDVBAlg(HI_UNF_ADVCA_ALG_TYPE_TDES);
        /* first level */
        Ret |= HI_UNF_ADVCA_SetDVBSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV1,g_u8EncrptedCwpk);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_ADVCA_SetDVBSessionKey failed\n");
            return HI_FAILURE;
        }
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hCaDesc,g_u8EncrytedOddKey);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hCaDesc,g_u8EncrytedEvenKey);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerEvenKey failed\n");
            return HI_FAILURE;
        }
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
    else if (!strcmp(argv[0],"loadcw"))
    {
        HI_U32 CwType;
        if (argc < 2)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        CwType = strtol(argv[1],NULL,0);
        Ret = load_cw(g_hAvplay,CwType);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("load cw failed\n");
        }
    }
    else if (!strcmp(argv[0],"loadcsa3"))
    {
        HI_U32 CwType;
        if (argc < 2)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        CwType = strtol(argv[1],NULL,0);
        Ret = load_csa3(g_hAvplay,CwType);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("load cw failed\n");
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
    //Ret |= HI_UNF_DMX_AttachTSPort(0, DEFAULT_DVB_PORT);
    Ret |= HIADP_DMX_AttachTSPort(0,TUNER_USE);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Demux_Init failed.\n");
        goto VO_DEINIT;
    }

    memset(&stNorcaDescAttr,0,sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));
    stNorcaDescAttr.enCaType = HI_UNF_DMX_CA_NORMAL;
    stNorcaDescAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_CSA2;
    Ret = HI_UNF_DMX_CreateDescramblerExt(0,&stNorcaDescAttr,&g_hNorcaDesc);
    if (HI_SUCCESS != Ret)
    {
        g_hNorcaDesc = INVALID_HANDLE;
    }

    memset(&stAdvcaDescAttr,0,sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));
    stAdvcaDescAttr.enCaType = HI_UNF_DMX_CA_ADVANCE;
    stAdvcaDescAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_CSA2;
    Ret = HI_UNF_DMX_CreateDescramblerExt(0,&stAdvcaDescAttr,&g_hAdvcaDesc);
    if (HI_SUCCESS != Ret)
    {
        g_hAdvcaDesc = INVALID_HANDLE;
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

#ifdef __HI3716MV310__
    Ret = HI_UNF_SND_Attach(HI_UNF_SND_0, g_hAvplay, HI_UNF_SND_MIX_TYPE_MASTER, 100);
#else
    Ret = HI_UNF_SND_Attach(hTrack, g_hAvplay);
#endif
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
#if 0
    ProgNum = 0;
    Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum,HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call SwitchProg failed.\n");
        goto AVPLAY_STOP;
    }

    while(1)
    {
        HI_CHAR  InputCmd[32];
        printf("please input the q to quit!\n");
        fgets(InputCmd, 30, stdin);

        if ('q' == InputCmd[0])
        {
            HI_DEBUG_ADVCA("prepare to quit!\n");
            break;
        }

    	ProgNum = atoi(InputCmd);

    	if(ProgNum == 0)
    	    ProgNum = 1;

		Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum-1,HI_TRUE);
		if (Ret != HI_SUCCESS)
		{
			HI_DEBUG_ADVCA("call SwitchProgfailed!\n");
			break;
		}
    }
#endif
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
    (HI_VOID)HI_UNF_DMX_DestroyDescrambler(g_hNorcaDesc);
    (HI_VOID)HI_UNF_DMX_DestroyDescrambler(g_hAdvcaDesc);

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


