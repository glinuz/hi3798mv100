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
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_unf_common.h"
#include "hi_unf_ecs.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_descrambler.h"
#include "hi_unf_advca.h"
#include "hi_unf_demux.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_mpi.h"
#include "sample_ca_common.h"

FILE               *g_pTsFile = HI_NULL;
static pthread_t   g_TsThd;
static pthread_mutex_t g_TsMutex;
static HI_BOOL     g_bStopTsThread = HI_FALSE;
HI_HANDLE          g_TsBuf;
PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;
HI_S32             g_CwType = 0;
#define  PLAY_DMX_ID  0

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
//K3 should be:0xcc,0x35,0x29,0x96,0x8d,0x54,0x08,0xd3,0x36,0x00,0xc1,0xa1,0x84,0x3d,0x72,0xa3

static HI_U8 g_u8ClearOddKey[8] = {0xFE,0x17,0x2C,0x41,0xA2,0xE1,0xD4,0x57};
static HI_U8 g_u8ClearEvenKey[8] = {0xD4,0x22,0x82,0x78,0x1D,0x28,0x72,0xB7};

//TDES
static HI_U8 g_u8TdesEK2[16] = {0x9a,0x3a,0x13,0xa4,0x08,0x40,0x9e,0x62,0xd3,0x50,0x0d,0x06,0x35,0x36,0x9a,0x09};
static HI_U8 g_u8TdesEK1[16] = {0xdd,0x20,0x99,0xa6,0xc9,0x83,0x6a,0xf7,0xe9,0xe3,0xdd,0xdf,0xf8,0x33,0x26,0x0c};
//nonce should be 1d,3f,18,b8,6a,f3,a0,94,30,e2,55,7f,f0,4b,b2,f4
static HI_U8 g_u8TdesEncOddECW[8] = {0xA8,0x28,0xFC,0x87,0x7B,0x56,0x01,0x81};
static HI_U8 g_u8TdesEncEvenECW[8] = {0xCD,0x6F,0x5F,0x98,0x53,0x32,0x2B,0xB2};

//AES
static HI_U8 g_u8AesEK2[16] = {0x28,0x5b,0x79,0xe4,0x2b,0xb3,0x5d,0x6f,0x92,0x03,0x61,0x98,0xbf,0xad,0x1c,0x3a};
static HI_U8 g_u8AesEK1[16] = {0x75,0xc4,0x41,0x3c,0xbc,0xfc,0x1e,0x64,0xa5,0xe7,0x6b,0x05,0x30,0xc6,0xc4,0xe2};
//nonce should be 40,fb,24,ca,a6,70,b5,f4,81,20,dd,ec,20,64,01,0e
static HI_U8 g_u8AesEncOddECW[16] = {0x26,0x6A,0xC8,0xB8,0x8C,0x3B,0x19,0x85,0x7B,0x6E,0xCD,0x28,0xAC,0x7B,0x77,0x5F};
static HI_U8 g_u8AesEncEvenECW[16] = {0xEA,0x3D,0xA6,0x2C,0x00,0x26,0x43,0xC3,0xAE,0x58,0x52,0x6B,0xA8,0x34,0x0C,0xB1};

extern HI_S32 GetNonce(HI_U8 nonce[], HI_S32 s32KldAlg, HI_U8 dnonce[]);
extern HI_S32 GetChipId(HI_U32 *pu32ChipId0, HI_U32 *pu32ChipId1);

HI_VOID *TsTthread(HI_VOID *args)
{
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_U32            Readlen;
    HI_S32            Ret;

    while (!g_bStopTsThread)
    {
        pthread_mutex_lock(&g_TsMutex);
        Ret = HI_UNF_DMX_GetTSBuffer(g_TsBuf, 188*50, &StreamBuf, 1000);
        if (Ret != HI_SUCCESS )
        {
            pthread_mutex_unlock(&g_TsMutex);
            continue;
        }

        Readlen = fread(StreamBuf.pu8Data, sizeof(HI_S8), 188*50, g_pTsFile);
        if(Readlen <= 0)
        {
            HI_DEBUG_ADVCA("read ts file end and rewind!\n");
            rewind(g_pTsFile);
            pthread_mutex_unlock(&g_TsMutex);
            continue;
        }

        Ret = HI_UNF_DMX_PutTSBuffer(g_TsBuf, Readlen);
        if (Ret != HI_SUCCESS )
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_PutTSBuffer failed.\n");
        }
        pthread_mutex_unlock(&g_TsMutex);
        usleep(5*1000);
    }

    Ret = HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
    if (Ret != HI_SUCCESS )
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_ResetTSBuffer failed.\n");
    }

    return HI_NULL;
}

HI_S32 AVPLAY_SetCW(HI_HANDLE hKey)
{
    HI_S32 Ret;

    if (0 == g_CwType)
    {
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hKey,g_u8ClearOddKey);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hKey,g_u8ClearEvenKey);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerKey failed\n");
            return Ret;
        }
    }
    else if(1 == g_CwType) //TDES
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
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hKey,g_u8TdesEncOddECW);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hKey,g_u8TdesEncEvenECW);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerEvenKey failed\n");
            return HI_FAILURE;
        }
		
        HI_UNF_ADVCA_DCASClose();

    }
    else if(2 == g_CwType) //AES
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
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hKey,g_u8AesEncOddECW);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hKey,g_u8AesEncEvenECW);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerEvenKey failed\n");
            return HI_FAILURE;
        }        
        HI_UNF_ADVCA_DCASClose();	
    }
    else
    {
        printf("Error g_CwType:%d\n", g_CwType);
    }

    
    return HI_SUCCESS;
}

HI_S32 AVPLAY_AttachDescramble(HI_HANDLE hAvplay,HI_HANDLE *phKey)
{
    HI_S32 Ret;
    HI_UNF_DMX_DESCRAMBLER_ATTR_S stDesramblerAttr;
    HI_HANDLE hKey;
    HI_HANDLE hDmxVidChn,hDmxAudChn;

	memset(&stDesramblerAttr,0,sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));
    if (0 == g_CwType)
    {
        stDesramblerAttr.enCaType = HI_UNF_DMX_CA_NORMAL;
    }
    else
    {
        stDesramblerAttr.enCaType = HI_UNF_DMX_CA_ADVANCE;
    }
    stDesramblerAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_CSA2;
	stDesramblerAttr.enEntropyReduction = HI_UNF_DMX_CA_ENTROPY_REDUCTION_CLOSE;
    Ret = HI_UNF_DMX_CreateDescramblerExt(0,&stDesramblerAttr,&hKey);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_CreateDescramblerExt failed %#x\n",Ret);
        return HI_FAILURE;
    }

    Ret = HI_UNF_AVPLAY_GetDmxVidChnHandle(hAvplay,&hDmxVidChn);
    Ret |= HI_UNF_AVPLAY_GetDmxAudChnHandle(hAvplay,&hDmxAudChn);
    Ret = HI_UNF_DMX_AttachDescrambler(hKey,hDmxVidChn);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_AttachDescrambler hDmxVidChn failed\n");        
        (HI_VOID)HI_UNF_DMX_DestroyDescrambler(hKey);
        return HI_FAILURE;
    }

    Ret = HI_UNF_DMX_AttachDescrambler(hKey,hDmxAudChn);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_AttachDescrambler hDmxAudChn failed\n");                
        (HI_VOID)HI_UNF_DMX_DetachDescrambler(hKey,hDmxVidChn);
        (HI_VOID)HI_UNF_DMX_DestroyDescrambler(hKey);
        return HI_FAILURE;
    }
    *phKey = hKey;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_DetachDescramble(HI_HANDLE hAvplay,HI_HANDLE hKey)
{
    HI_S32 Ret;
    HI_HANDLE hDmxVidChn,hDmxAudChn;

    Ret = HI_UNF_AVPLAY_GetDmxVidChnHandle(hAvplay,&hDmxVidChn);
    Ret |= HI_UNF_AVPLAY_GetDmxAudChnHandle(hAvplay,&hDmxAudChn);
    Ret |= HI_UNF_DMX_DetachDescrambler(hKey,hDmxVidChn);
    Ret |= HI_UNF_DMX_DetachDescrambler(hKey,hDmxAudChn);
    Ret |= HI_UNF_DMX_DestroyDescrambler(hKey);
    
    return Ret;
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


HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32                  Ret;
    HI_HANDLE               hWin;
    HI_HANDLE               hAvplay;
    HI_HANDLE               hKey;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    HI_CHAR                 InputCmd[32];
    HI_UNF_ENC_FMT_E   enFormat = HI_UNF_ENC_FMT_1080i_50;
    HI_U32             ProgNum;
    HI_U32                      i;

    HI_HANDLE                   hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S    stTrackAttr;
#ifdef __HI3716MV310__	
        HI_UNF_MIXER_ATTR_S stMixerAttr;
#endif
    HI_U32 u32ChipId0, u32ChipId1;

    HI_U32 u32MSBID, u32LSBID;
	HI_U8 u8Dnonce[16]={0};
    
    if(argc < 3)
    {
        printf("Usage: sample_ca_dcas_tsplay file CwType\n"
               "CwType 0-ClearCw 1-TDES 2-AES\n");
        return 0;
    }

    g_CwType = (HI_U32)strtol(argv[2],NULL,0);
    enFormat = HI_UNF_ENC_FMT_1080i_50;
    
    g_pTsFile = fopen(argv[1], "rb");
    if (!g_pTsFile)
	{
        HI_DEBUG_ADVCA("open file %s error!\n", argv[1]);
		return -1;
	}

    (HI_VOID)HI_SYS_Init();
#ifndef __HI3716MV310__
    HIADP_MCE_Exit();
#endif
    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_ADVCA_Init failed\n");
        goto SYS_DEINIT;
    }

    Ret = GetChipId(&u32ChipId0, &u32ChipId1);
    Ret |= HI_UNF_ADVCA_GetDCASChipId(&u32MSBID, &u32LSBID);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("GetChipId failed:%x\n",Ret);
        return HI_FAILURE;
    }
    printf("CHIPID:0x%08x%08x\n", u32ChipId0, u32ChipId1);	
    printf("CHIPID:0x%08x%08x\n", u32MSBID, u32LSBID);

    Ret = GetNonce(g_u8Nonce, HI_UNF_CIPHER_ALG_AES, u8Dnonce);
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
		
    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call SndInit failed.\n");
        goto CA_DEINIT;
    }

    Ret = HIADP_Disp_Init(enFormat);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Disp_Init failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,&hWin);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call VoInit failed.\n");
        (HI_VOID)HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
#ifdef __HI3716MV310__
    Ret |= HI_UNF_DMX_AttachTSPort(PLAY_DMX_ID, HI_UNF_DMX_PORT_3_RAM);
#else	
    Ret = HI_UNF_DMX_AttachTSPort(PLAY_DMX_ID,HI_UNF_DMX_PORT_RAM_0);
#endif
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call VoInit failed.\n");
        goto VO_DEINIT;
    }
#ifdef __HI3716MV310__
    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_3_RAM, 0x200000, &g_TsBuf);
#else
    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_0, 0x200000, &g_TsBuf);
#endif
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_CreateTSBuffer failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call RegADecLib failed.\n");
        goto TSBUF_FREE;
    }

    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Init failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto AVPLAY_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto VCHN_CLOSE;
    }

    Ret = AVPLAY_AttachDescramble(hAvplay,&hKey);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call AVPLAY_AttachDescramble failed.\n");
        goto ACHN_CLOSE;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
    Ret |= HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto WIN_DETACH;
    }

#ifndef __HI3716MV310__
    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    }
    //Ret = HI_UNF_SND_Mixer_Open(HI_UNF_SND_0, &hTrack, &stMixerAttr);
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }
#endif
#ifdef 	__HI3716MV310__
    Ret = HI_UNF_SND_Attach(HI_UNF_SND_0, hAvplay, HI_UNF_SND_MIX_TYPE_MASTER, 100);
#else
    Ret = HI_UNF_SND_Attach(hTrack, hAvplay);
#endif
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_SND_Attach failed.\n");
        goto TRACK_DESTROY;
    }
    
    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
    Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    (HI_VOID)pthread_mutex_init(&g_TsMutex,NULL);
    g_bStopTsThread = HI_FALSE;
    (HI_VOID)pthread_create(&g_TsThd, HI_NULL, TsTthread, HI_NULL);

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(0, &g_pProgTbl);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HIADP_Search_GetAllPmt failed.\n");
        goto PSISI_FREE;
    }

#if 0
    HI_DEBUG_ADVCA("select channel number to start play\n");
    memset(InputCmd, 0, 30);
    fgets(InputCmd, 30, stdin);
    ProgNum = atoi(InputCmd);
#endif
    ProgNum = 0;

    pthread_mutex_lock(&g_TsMutex);
    rewind(g_pTsFile);
    (HI_VOID)HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
    pthread_mutex_unlock(&g_TsMutex);
    Ret = AVPLAY_SetCW(hKey);
    Ret |= HIADP_AVPlay_PlayProg(hAvplay, g_pProgTbl, ProgNum, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call SwitchProg failed.\n");
        goto AVPLAY_STOP;
    }

    while(1)
    {
        printf("please input 'q' to quit!\n");
        memset(InputCmd, 0, 30);
        (HI_VOID)fgets(InputCmd, 30, stdin);
        if ('q' == InputCmd[0])
        {
            HI_DEBUG_ADVCA("prepare to quit!\n");
            break;
        }

        ProgNum = atoi(InputCmd);
        if (ProgNum == 0)
            ProgNum = 1;
        printf("switch to program:%d\n", ProgNum);

        pthread_mutex_lock(&g_TsMutex);
        rewind(g_pTsFile);
        (HI_VOID)HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
        pthread_mutex_unlock(&g_TsMutex);
        Ret = AVPLAY_SetCW(hKey);
		Ret |= HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum-1,HI_TRUE);
		if (Ret != HI_SUCCESS)
		{
			HI_DEBUG_ADVCA("call SwitchProgfailed!\n");
			break;
		}
    }

AVPLAY_STOP:
    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;
    (HI_VOID)HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);

PSISI_FREE:
    HIADP_Search_DeInit();

    g_bStopTsThread = HI_TRUE;
    (HI_VOID)pthread_join(g_TsThd, HI_NULL);  
    (HI_VOID)pthread_mutex_destroy(&g_TsMutex);

SND_DETACH:
    (HI_VOID)HI_UNF_SND_Detach(hTrack, hAvplay);

TRACK_DESTROY:
#ifdef __HI3716MV310__	
        HI_UNF_SND_Mixer_Close(hTrack);
#else
        HI_UNF_SND_DestroyTrack(hTrack);
#endif	
    
WIN_DETACH:
    (HI_VOID)HI_UNF_VO_SetWindowEnable(hWin,HI_FALSE);
    (HI_VOID)HI_UNF_VO_DetachWindow(hWin, hAvplay);

    (HI_VOID)AVPLAY_DetachDescramble(hAvplay,hKey);

ACHN_CLOSE:
    (HI_VOID)HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

VCHN_CLOSE:
    (HI_VOID)HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

AVPLAY_DESTROY:
    (HI_VOID)HI_UNF_AVPLAY_Destroy(hAvplay);
    
AVPLAY_DEINIT:
    (HI_VOID)HI_UNF_AVPLAY_DeInit();

TSBUF_FREE:
    (HI_VOID)HI_UNF_DMX_DestroyTSBuffer(g_TsBuf);
    
DMX_DEINIT:
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
    (HI_VOID)fclose(g_pTsFile);
    g_pTsFile = HI_NULL;

    return Ret;
}


