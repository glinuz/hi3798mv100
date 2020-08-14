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
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

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

///Vector for DCAS



#ifdef __HI3716MV310__  //Hi3716MV300 Hi3716MV310
#define STB_SN2_ADDR	0x10180118
#define STB_SN3_ADDR	0x1018011C
#else  //Hi3716cv200
#define STB_SN2_ADDR	0xf8ab0118
#define STB_SN3_ADDR	0xf8ab011c
#endif

//Test Stream:
//INTEGRATION_Chipset_SingleCrypto.ts
//ESCK for the test vector in this sample
//static HI_U8 g_u8ESCK[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static HI_U8 g_u8Nonce[16]  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
static HI_U8 g_u8OutputBuf[16];

//VendorSysId is a short integer, 2byte£¬should be extended to 16byte with 0x00.
//For example, the VendorSysId in this sample is 0x0a.
static unsigned char g_u8Vendor_ID[16] = {0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};

//K3:0xcc,0x35,0x29,0x96,0x8d,0x54,0x08,0xd3,0x36,0x00,0xc1,0xa1,0x84,0x3d,0x72,0xa3
static unsigned char g_K3[16] = {0xcc,0x35,0x29,0x96,0x8d,0x54,0x08,0xd3,0x36,0x00,0xc1,0xa1,0x84,0x3d,0x72,0xa3};

static HI_U8 g_u8ClearOddKey[8] = {0xFE,0x17,0x2C,0x41,0xA2,0xE1,0xD4,0x57};
static HI_U8 g_u8ClearEvenKey[8] = {0xD4,0x22,0x82,0x78,0x1D,0x28,0x72,0xB7};

//TDES
static HI_U8 g_u8TdesEK2[16] = {0x9a,0x3a,0x13,0xa4,0x08,0x40,0x9e,0x62,0xd3,0x50,0x0d,0x06,0x35,0x36,0x9a,0x09};
static HI_U8 g_u8TdesEK1[16] = {0xdd,0x20,0x99,0xa6,0xc9,0x83,0x6a,0xf7,0xe9,0xe3,0xdd,0xdf,0xf8,0x33,0x26,0x0c};
//nonce should be 1d,3f,18,b8,6a,f3,a0,94,30,e2,55,7f,f0,4b,b2,f4
static HI_U8 g_u8TdesEncOddECW[16] = {0xA8,0x28,0xFC,0x87,0x7B,0x56,0x01,0x81};
static HI_U8 g_u8TdesEncEvenECW[16] = {0xCD,0x6F,0x5F,0x98,0x53,0x32,0x2B,0xB2};

//AES
static HI_U8 g_u8AesEK2[16] = {0x28,0x5b,0x79,0xe4,0x2b,0xb3,0x5d,0x6f,0x92,0x03,0x61,0x98,0xbf,0xad,0x1c,0x3a};
static HI_U8 g_u8AesEK1[16] = {0x75,0xc4,0x41,0x3c,0xbc,0xfc,0x1e,0x64,0xa5,0xe7,0x6b,0x05,0x30,0xc6,0xc4,0xe2};
//nonce should be 40,fb,24,ca,a6,70,b5,f4,81,20,dd,ec,20,64,01,0e
static HI_U8 g_u8AesEncOddECW[16] = {0x26,0x6A,0xC8,0xB8,0x8C,0x3B,0x19,0x85,0x7B,0x6E,0xCD,0x28,0xAC,0x7B,0x77,0x5F};
static HI_U8 g_u8AesEncEvenECW[16] = {0xEA,0x3D,0xA6,0x2C,0x00,0x26,0x43,0xC3,0xAE,0x58,0x52,0x6B,0xA8,0x34,0x0C,0xB1};

//DCAS R2R crypt
#define TEXT_LEN  16
static HI_U8 g_u8Encontentkey[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
static HI_U8 g_PlainText[TEXT_LEN]= {0x12,0x34,0x56,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  /* 436f6e6178204153 */

//TDES-ECB Encrypt: 26fc762bf21704eeefe5ed0079022b48
//AES-ECB Encrypt: f270bd4242d41827bd149dd956768391

#ifndef DEFAULT_DVB_PORT
#define DEFAULT_DVB_PORT 0
#endif

#define INVALID_HANDLE      0xffffffff

PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;
static HI_HANDLE g_hAvplay;
static HI_HANDLE g_hNorcaDesc,g_hAdvcaDesc;

#define MAX_CMDLINE_LEN  1280
#define MAX_ARGS_COUNT   10
#define MAX_ARGS_LEN     128
static HI_CHAR g_CmdLine[MAX_CMDLINE_LEN];
static HI_S32  g_Argc;
static HI_CHAR g_Argv[MAX_ARGS_COUNT][MAX_ARGS_LEN];
static HI_BOOL g_bRun = HI_TRUE;

HI_S32 GetChipId(HI_U32 *pu32ChipId0, HI_U32 *pu32ChipId1);
HI_S32 GetNonce(HI_U8 nonce[], HI_S32 s32KldAlg, HI_U8 dnonce[]);
HI_S32 R2R_Test(HI_S32 s32KldAlg);
HI_S32 simulator_keyladder(HI_S32 s32KldAlg);

HI_S32 parse_cmdline(HI_CHAR *pCmdLine,HI_S32 *pArgc,HI_CHAR Argv[MAX_ARGS_COUNT][MAX_ARGS_LEN])
{
    HI_CHAR *ptr = pCmdLine;
    int i;

    while ((*ptr == ' ') && (*ptr++ != '\0'))
    {
        ;
    }

    for (i = (int)strlen(ptr); i > 0; i--)
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
           "nonce type(0: TDES, 1: AES)\n"
           "tunerlock freq symbrate\n"
           "loadcw type(0: clear CW, 1: TDES, 2: AES)\n"
           "playbynum prognum\n"
           "playbypid vpid apid\n"
           "stopplay\n"
           "r2r_test  type(0: TDES, 1: AES)\n"
           "simulator_keyladder(0: TDES, 1: AES)\n"
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
    else if(1 == CwType) //TDES
    {
		//Set the algorithm to TDES
		HI_UNF_ADVCA_DCASOpen(HI_UNF_CIPHER_ALG_3DES);
				
		//Set Vendor ID
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_VENDORSYSID, g_u8Vendor_ID, g_u8OutputBuf);
		
		//Set EK2
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_EK2, g_u8TdesEK2, g_u8OutputBuf);
				
		//Set EK1
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_EK1, g_u8TdesEK1, g_u8OutputBuf);
        
        //Set the encrypted control word
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hCaDesc,g_u8TdesEncOddECW);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hCaDesc,g_u8TdesEncEvenECW);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerEvenKey failed\n");
            return HI_FAILURE;
        }
        HI_UNF_ADVCA_DCASClose();
    }
    else if(2 == CwType) //AES
    {
		//Set the algorithm to AES		
		HI_UNF_ADVCA_DCASOpen(HI_UNF_CIPHER_ALG_AES);
		
		//Set Vendor ID
		HI_UNF_ADVCA_SetDCASSessionKey(8, g_u8Vendor_ID, g_u8OutputBuf);
		
		//Set EK2
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_EK2, g_u8AesEK2, g_u8OutputBuf);
				
		//Set EK1
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_EK1, g_u8AesEK1, g_u8OutputBuf);
        
        //Set the encrypted control word
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hCaDesc,g_u8AesEncOddECW);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hCaDesc,g_u8AesEncEvenECW);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerEvenKey failed\n");
            return HI_FAILURE;
        }        
        HI_UNF_ADVCA_DCASClose();
    }

    return HI_SUCCESS;
}

HI_S32 run_cmdline(HI_S32 argc,HI_CHAR argv[MAX_ARGS_COUNT][MAX_ARGS_LEN])
{
    HI_S32 Ret;
    HI_S32 s32KldAlg;
    HI_U8 u8Dnonce[16];
    HI_U32 i;
    
    if (!strncmp(argv[0], "help", 4))
    {
        show_usage();
    }
    else if (!strncmp(argv[0], "getchipid", 9))
    {
        HI_U32 u32ChipId0, u32ChipId1;
        HI_U32 u32MSBID, u32LSBID;
        Ret = GetChipId(&u32ChipId0, &u32ChipId1);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("GetChipId failed:%x\n",Ret);
            return HI_FAILURE;
        }
        Ret = HI_UNF_ADVCA_GetDCASChipId(&u32MSBID, &u32LSBID);
        printf("CHIPID:0x%08x%08x\n", u32ChipId0, u32ChipId1);
        printf("CHIPID:0x%08x%08x\n", u32MSBID, u32LSBID);
    }
    else if (!strncmp(argv[0], "nonce", 9))
    {
        if (argc < 2)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        s32KldAlg = (HI_U32)strtol(argv[1],NULL,0);    	
        Ret = GetNonce(g_u8Nonce, s32KldAlg, u8Dnonce);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("Get nonce failed:%x\n",Ret);
            return HI_FAILURE;
        }
        
        printf("Dnonce:\r\n");
		for(i = 0; i < 16; i++)
		{
			printf("%02x,", u8Dnonce[i]);
		}
		printf("\r\n");
    }    
    else if (!strncmp(argv[0], "tunerlock", 9))
    {
        HI_U32 Freq,Srate,Qam;
        if(argc < 3)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        Freq  = (HI_U32)strtol(argv[1],NULL,0);
        Srate = (HI_U32)strtol(argv[2],NULL,0);
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
    else if (!strncmp(argv[0], "loadcw", 6))
    {
        HI_U32 CwType;
        if (argc < 2)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        CwType = (HI_U32)strtol(argv[1],NULL,0);
        Ret = load_cw(g_hAvplay,CwType);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("load cw failed\n");
        }
    }
    else if (!strncmp(argv[0], "playbypid", 9))
    {
        HI_U32 VidPid,AudPid;
        if (argc < 3)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        VidPid = (HI_U32)strtol(argv[1],NULL,0);
        AudPid = (HI_U32)strtol(argv[2],NULL,0);
        
        Ret = play_bypid(g_hAvplay,VidPid,AudPid);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("play prog failed:%x\n",Ret);
            return HI_FAILURE;
        }
    }
    else if (!strncmp(argv[0], "playbynum", 9))
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
        ProgNum = (HI_U32)strtol(argv[1],NULL,0);
        Ret = HIADP_AVPlay_PlayProg(g_hAvplay,g_pProgTbl,ProgNum - 1,HI_TRUE);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("play prog failed:%x\n",Ret);
            return HI_FAILURE;
        }
    }
    else if (!strncmp(argv[0],"stopplay", 8))
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
    else if (!strcmp(argv[0],"r2r_test"))
    {
        if (argc < 2)
        {
            HI_DEBUG_ADVCA("invalid arguments\n");
            return HI_FAILURE;
        }
        s32KldAlg = (HI_U32)strtol(argv[1],NULL,0);    	
        Ret = R2R_Test(s32KldAlg);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("Get nonce failed:%x\n",Ret);
            return HI_FAILURE;
        }
    }
    else if(!strcmp(argv[0],"simulator_keyladder"))
    {
        s32KldAlg = (HI_U32)strtol(argv[1],NULL,0); 
        Ret = simulator_keyladder(s32KldAlg);        
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

HI_S32 GetChipId(HI_U32 *pu32ChipId0, HI_U32 *pu32ChipId1)
{
    HI_S32 Ret;
    HI_U32 u32StbSN0, u32StbSN1;
            
    if(pu32ChipId0 == NULL || pu32ChipId1 == NULL)
    {
    	printf("Invalid parameter!\r\n");	
    	return HI_FAILURE;
    }
    
    Ret = HI_SYS_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_SYS_Init failed\n");
        return HI_FAILURE;
    }

    Ret = HI_SYS_ReadRegister(STB_SN2_ADDR, &u32StbSN0);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("get stb sn failed\n");
        return HI_FAILURE;
    }
    
    Ret = HI_SYS_ReadRegister(STB_SN3_ADDR, &u32StbSN1);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("get stb sn failed\n");
        return HI_FAILURE;
    }
    
	*pu32ChipId0 = u32StbSN0;
	*pu32ChipId1 = u32StbSN1;
       
    (HI_VOID)HI_SYS_DeInit();
    return HI_SUCCESS;
}

HI_S32 GetNonce(HI_U8 nonce[], HI_S32 s32KldAlg, HI_U8 dnonce[])
{
	if(s32KldAlg == 0)
	{
		//Set the algorithm to TDES
		HI_UNF_ADVCA_DCASOpen(HI_UNF_CIPHER_ALG_3DES);
			
		//Set Vendor ID
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_VENDORSYSID, g_u8Vendor_ID, g_u8OutputBuf);
	
		//Set EK2
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_EK2, g_u8TdesEK2, g_u8OutputBuf);
			
		//set nonce			
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_NONCE, g_u8Nonce, dnonce);
		
		HI_UNF_ADVCA_DCASClose();
	}
	else
	{
		//Set the algorithm to AES
		HI_UNF_ADVCA_DCASOpen(HI_UNF_CIPHER_ALG_AES);
			
		//Set Vendor ID
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_VENDORSYSID, g_u8Vendor_ID, g_u8OutputBuf);
	
		//Set EK2
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_EK2, g_u8AesEK2, g_u8OutputBuf);
			
		//set nonce			
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_NONCE, g_u8Nonce, dnonce);	
		
		HI_UNF_ADVCA_DCASClose();	
	}
	
	return HI_SUCCESS;	
}

HI_S32 R2R_Test(HI_S32 s32KldAlg)
{
    HI_S32 Ret = 0, i = 0;
    HI_HANDLE hCipher;
    HI_UNF_CIPHER_CTRL_S stCtrl;
    HI_MMZ_BUF_S PlainBuf,EncrytedBuf;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;

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

    memset(&stCipherAttr, 0, sizeof(stCipherAttr));
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    Ret = HI_UNF_CIPHER_CreateHandle(&hCipher, &stCipherAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_CreateHandle failed:%x\n",Ret);
        return HI_FAILURE;
    }
    
	if(s32KldAlg == 0)
	{
		//Set the algorithm to TDES
		HI_UNF_ADVCA_DCASOpen(HI_UNF_CIPHER_ALG_3DES);
			
		//Set Vendor ID
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_VENDORSYSID, g_u8Vendor_ID, g_u8OutputBuf);
	
		//Set EK2
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_EK2, g_u8TdesEK2, g_u8OutputBuf);
				
		//Set EK1
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_EK1, g_u8TdesEK1, g_u8OutputBuf);
        
		//set E-KEY
        memset(&stCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
        memcpy(stCtrl.u32Key, g_u8Encontentkey, 16);
        stCtrl.bKeyByCA = HI_TRUE;
        stCtrl.enAlg = HI_UNF_CIPHER_ALG_3DES;
        stCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_64BIT;
        stCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
        stCtrl.enKeyLen = HI_UNF_CIPHER_KEY_DES_2KEY;
        stCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
        Ret = HI_UNF_CIPHER_ConfigHandle(hCipher,&stCtrl);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("HI_UNF_CIPHER_ConfigHandle failed:%x\n",Ret);
            return HI_FAILURE;
        }  
		HI_UNF_ADVCA_DCASClose();
	}
	else
	{
		//Set the algorithm to AES
		HI_UNF_ADVCA_DCASOpen(HI_UNF_CIPHER_ALG_AES);
			
		//Set Vendor ID
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_VENDORSYSID, g_u8Vendor_ID, g_u8OutputBuf);
	
		//Set EK2
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_EK2, g_u8AesEK2, g_u8OutputBuf);
			
		//Set EK1
		HI_UNF_ADVCA_SetDCASSessionKey(HI_UNF_ADVCA_DCAS_KEYLADDER_EK1, g_u8AesEK1, g_u8OutputBuf);
        
		//set E-KEY
        memset(&stCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
        memcpy(stCtrl.u32Key, g_u8Encontentkey, 16);
        stCtrl.bKeyByCA = HI_TRUE;
        stCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
        stCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
        stCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
        stCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
        stCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
        Ret = HI_UNF_CIPHER_ConfigHandle(hCipher,&stCtrl);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("HI_UNF_CIPHER_ConfigHandle failed:%x\n",Ret);
            return HI_FAILURE;
        }  
		HI_UNF_ADVCA_DCASClose();	
	}
      
    /* encryption demo */  
    printf("===============encryption demo===============\n");
    memcpy(PlainBuf.user_viraddr, g_PlainText, TEXT_LEN);
    memset(EncrytedBuf.user_viraddr, 0, EncrytedBuf.bufsize);   
    Ret = HI_UNF_CIPHER_Encrypt(hCipher,PlainBuf.phyaddr, EncrytedBuf.phyaddr, TEXT_LEN);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Encrypt failed:%x\n",Ret);
        return HI_FAILURE;
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
        return HI_FAILURE;
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
    printf("\n");
    Ret = HI_UNF_CIPHER_DestroyHandle(hCipher);
	
	return HI_SUCCESS;	
}

HI_S32 simulator_keyladder(HI_S32 s32KldAlg)
{
    HI_U32 index;
    HI_U8 TempKey[16] = {0};
    HI_UNF_CIPHER_ALG_E enAlg;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;
    
    printf("This is just for simalutor of DCAS key-ladder, Alg is:");
    if(s32KldAlg == 0)
    {
        enAlg = HI_UNF_CIPHER_ALG_3DES;
        printf("TDES\n");
    }
    else
    {
        enAlg = HI_UNF_CIPHER_ALG_AES;
        printf("AES\n");
    }
    
    printf("suppose K3 is:\n");
    for(index = 0; index < 16; index ++)
    {
        printf("%02X,", g_K3[index]);
    }
    printf("\nSimualator K3-->CW [TDES]\n");
    
    HI_S32 Ret = 0, i = 0;
    HI_HANDLE hCipher;
    HI_UNF_CIPHER_CTRL_S stCtrl;
    HI_MMZ_BUF_S PlainBuf,EncrytedBuf;

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

    strcpy(EncrytedBuf.bufname,"cipher_text");
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

    memset(&stCipherAttr, 0, sizeof(stCipherAttr));
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    Ret = HI_UNF_CIPHER_CreateHandle(&hCipher, &stCipherAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_CreateHandle failed:%x\n",Ret);
        return HI_FAILURE;
    }
    
    memset(&stCtrl, 0, sizeof(HI_UNF_CIPHER_CTRL_S));
    if(enAlg == HI_UNF_CIPHER_ALG_3DES)
    {
        stCtrl.bKeyByCA = HI_FALSE;
        stCtrl.enAlg = enAlg;
        stCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_64BIT;
        stCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
        stCtrl.enKeyLen = HI_UNF_CIPHER_KEY_DES_2KEY;
        stCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
    }
    else
    {
        stCtrl.bKeyByCA = HI_FALSE;
        stCtrl.enAlg = enAlg;
        stCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_64BIT;
        stCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
        stCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
        stCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_R2R;
    }
    
    //Decrypt K2.
    memcpy(stCtrl.u32Key, g_K3, 16);
    Ret = HI_UNF_CIPHER_ConfigHandle(hCipher,&stCtrl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_ConfigHandle failed:%x\n",Ret);
        return HI_FAILURE;
    } 
    if(enAlg == HI_UNF_CIPHER_ALG_3DES)
    {    
        memcpy(EncrytedBuf.user_viraddr, g_u8TdesEK2, TEXT_LEN);  
    }
    else
    {
        memcpy(EncrytedBuf.user_viraddr, g_u8AesEK2, TEXT_LEN);  
    }
    memset(PlainBuf.user_viraddr, 0, TEXT_LEN);
    Ret = HI_UNF_CIPHER_Decrypt(hCipher, EncrytedBuf.phyaddr, PlainBuf.phyaddr,TEXT_LEN);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Encrypt failed:%x\n",Ret);
        return HI_FAILURE;
    } 
    printf("Even cipher  text :");
    for ( i = 0 ; i < TEXT_LEN; i++ )
    {
        printf("%02x",EncrytedBuf.user_viraddr[i]);
    }
    printf("\n");
    printf("k2 plain     text :");
    for ( i = 0 ; i < TEXT_LEN; i++ )
    {
        printf("%02x",PlainBuf.user_viraddr[i]);
    }
    printf("\n");
    memcpy(TempKey, PlainBuf.user_viraddr, TEXT_LEN);
    
    //Decrypt nonce
    memcpy(stCtrl.u32Key, PlainBuf.user_viraddr, 16);
    Ret = HI_UNF_CIPHER_ConfigHandle(hCipher,&stCtrl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_ConfigHandle failed:%x\n",Ret);
        return HI_FAILURE;
    } 
    memcpy(EncrytedBuf.user_viraddr, PlainBuf.user_viraddr, TEXT_LEN);  
    memset(PlainBuf.user_viraddr, 0, TEXT_LEN); 
    Ret = HI_UNF_CIPHER_Decrypt(hCipher, EncrytedBuf.phyaddr, PlainBuf.phyaddr,TEXT_LEN);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Encrypt failed:%x\n",Ret);
        return HI_FAILURE;
    } 
    
    memcpy(stCtrl.u32Key, PlainBuf.user_viraddr, 16);
    Ret = HI_UNF_CIPHER_ConfigHandle(hCipher,&stCtrl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_ConfigHandle failed:%x\n",Ret);
        return HI_FAILURE;
    } 
    memcpy(EncrytedBuf.user_viraddr, g_u8Nonce, TEXT_LEN);  
    memset(PlainBuf.user_viraddr, 0, TEXT_LEN); 
    Ret = HI_UNF_CIPHER_Decrypt(hCipher, EncrytedBuf.phyaddr, PlainBuf.phyaddr,TEXT_LEN);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Encrypt failed:%x\n",Ret);
        return HI_FAILURE;
    } 
    printf("nonce        text :");
    for ( i = 0 ; i < TEXT_LEN; i++ )
    {
        printf("%02x",EncrytedBuf.user_viraddr[i]);
    }
    printf("\n");
    printf("Dnonce       text :");
    for ( i = 0 ; i < TEXT_LEN; i++ )
    {
        printf("%02x",PlainBuf.user_viraddr[i]);
    }
    printf("\n");
    
    //Decrypt K1.
    memcpy(stCtrl.u32Key, TempKey, 16);
    Ret = HI_UNF_CIPHER_ConfigHandle(hCipher,&stCtrl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_ConfigHandle failed:%x\n",Ret);
        return HI_FAILURE;
    } 
    if(enAlg == HI_UNF_CIPHER_ALG_3DES)
    {    
        memcpy(EncrytedBuf.user_viraddr, g_u8TdesEK1, TEXT_LEN);  
    }
    else
    {
        memcpy(EncrytedBuf.user_viraddr, g_u8AesEK1, TEXT_LEN);  
    } 
    memset(PlainBuf.user_viraddr, 0, TEXT_LEN); 
    Ret = HI_UNF_CIPHER_Decrypt(hCipher, EncrytedBuf.phyaddr, PlainBuf.phyaddr,TEXT_LEN);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Encrypt failed:%x\n",Ret);
        return HI_FAILURE;
    } 
    printf("K1 cipher    text :");
    for ( i = 0 ; i < TEXT_LEN; i++ )
    {
        printf("%02x",EncrytedBuf.user_viraddr[i]);
    }
    printf("\n");
    printf("k1 plain     text :");
    for ( i = 0 ; i < TEXT_LEN; i++ )
    {
        printf("%02x",PlainBuf.user_viraddr[i]);
    }
    printf("\n");
    //Decrypt Odd CW.
    memcpy(stCtrl.u32Key, PlainBuf.user_viraddr, 16);
    Ret = HI_UNF_CIPHER_ConfigHandle(hCipher,&stCtrl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_ConfigHandle failed:%x\n",Ret);
        return HI_FAILURE;
    } 
    if(enAlg == HI_UNF_CIPHER_ALG_3DES)
    {    
        memcpy(EncrytedBuf.user_viraddr, g_u8TdesEncOddECW, TEXT_LEN);  
    }
    else
    {
        memcpy(EncrytedBuf.user_viraddr, g_u8AesEncOddECW, TEXT_LEN);  
    } 
    memset(PlainBuf.user_viraddr, 0, TEXT_LEN);   
    Ret = HI_UNF_CIPHER_Decrypt(hCipher, EncrytedBuf.phyaddr, PlainBuf.phyaddr,TEXT_LEN);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Encrypt failed:%x\n",Ret);
        return HI_FAILURE;
    } 
    printf("Odd cipher   text :");
    for ( i = 0 ; i < TEXT_LEN; i++ )
    {
        printf("%02x",EncrytedBuf.user_viraddr[i]);
    }
    printf("\n");
    printf("Odd plain    text :");
    for ( i = 0 ; i < TEXT_LEN; i++ )
    {
        printf("%02x",PlainBuf.user_viraddr[i]);
    }
    printf("\n");
    if(enAlg == HI_UNF_CIPHER_ALG_3DES)
    {    
        memcpy(EncrytedBuf.user_viraddr, g_u8TdesEncEvenECW, TEXT_LEN);  
    }
    else
    {
        memcpy(EncrytedBuf.user_viraddr, g_u8AesEncEvenECW, TEXT_LEN);  
    } 
    memset(PlainBuf.user_viraddr, 0, TEXT_LEN);  
    Ret = HI_UNF_CIPHER_Decrypt(hCipher, EncrytedBuf.phyaddr, PlainBuf.phyaddr,TEXT_LEN);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Encrypt failed:%x\n",Ret);
        return HI_FAILURE;
    } 
    printf("Even cipher  text :");
    for ( i = 0 ; i < TEXT_LEN; i++ )
    {
        printf("%02x",EncrytedBuf.user_viraddr[i]);
    }
    printf("\n");
    printf("Even plain   text :");
    for ( i = 0 ; i < TEXT_LEN; i++ )
    {
        printf("%02x",PlainBuf.user_viraddr[i]);
    }
    printf("\n");
    
    Ret = HI_UNF_CIPHER_DestroyHandle(hCipher);
    
    return 0;
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
#ifdef __HI3716MV310__	
    HI_UNF_MIXER_ATTR_S stMixerAttr;
#endif
    
    (HI_VOID)HI_SYS_Init();
#ifndef __HI3716MV310__
        HIADP_MCE_Exit();
#endif

    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_ADVCA_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HI_UNF_CIPHER_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_CIPHER_Init failed:%x\n",Ret);
        goto CA_DEINIT;
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
    Ret |= HI_UNF_DMX_AttachTSPort(0, DEFAULT_DVB_PORT);
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

    Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Demux_Init failed.\n");
        goto DES_DESTROY;
    }

    HIADP_Search_Init();

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

#ifndef __HI3716MV310__
    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    }
    //Ret = HI_UNF_SND_Mixer_Open(HI_UNF_SND_0, &hTrack, &stMixerAttr);
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrack);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }
#endif

#ifdef 	__HI3716MV310__
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
#ifdef __HI3716MV310__	
	HI_UNF_SND_Mixer_Close(hTrack);
#else
	HI_UNF_SND_DestroyTrack(hTrack);
#endif	
    
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
    (HI_VOID)HI_UNF_CIPHER_DeInit();
SYS_DEINIT:
    (HI_VOID)HI_SYS_DeInit();

    return Ret;
}


