/******************************************************************************

  Copyright (C), 2001-2050, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     :
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/01/27
  Description   :
  History       :
  1.Date        : 2014/06/9
    Author      : l00214567
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/time.h>
#include <linux/videodev2.h>

#include "hi_unf_common.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_vi.h"
#include "hi_adp.h"
#include "hi_adp_mpi.h"
#include "hi_drv_hdmirx.h"


#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_unf_common.h"
#include "hi_unf_ai.h"
#include "hi_unf_sound.h"
#include "hi_adp_mpi.h"
#include "hi_adp_hdmi_rx.h"



#if 0
#define sample_printf
#else
#define sample_printf printf
#endif



#ifdef HI_HDMI_RX_INSIDE 

HI_S32 hdmi_audio(HI_VOID)
{
    HI_UNF_SIG_STATUS_E enStatus;
	
	HI_S32 Ret;
	HI_HANDLE hAi;
	HI_UNF_AI_ATTR_S stAitAttr;
	HI_HANDLE hTrackSlave;
	HI_HANDLE hTrackVir;
	HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;
	HI_UNF_AI_HDMI_ATTR_S stAudioInfo;

	HI_CHAR InputCmd[32];
    

    enStatus = HI_UNF_HDMIRX_GetAudioInfo(&stAudioInfo);
    if (HI_FAILURE != enStatus)
    {
        sample_printf("enChannel:%d\n", stAudioInfo.enChannel);
        sample_printf("enBitDepth:%d\n", stAudioInfo.enBitDepth);
        sample_printf("enSampleRate:%d\n", stAudioInfo.enSampleRate);
        sample_printf("enHdmiAudioDataFormat:%d\n", stAudioInfo.enHdmiAudioDataFormat);

    	Ret = HI_UNF_AI_Init();
    	if (HI_SUCCESS != Ret)
    	{
    		printf("call AiInit failed.\n");
            return HI_FAILURE;
    	}
    	Ret = HI_UNF_AI_GetDefaultAttr(HI_UNF_AI_HDMI0,&stAitAttr);
    	//	stAitAttr.u32PcmFrameMaxNum = 8;
    	stAitAttr.enSampleRate = stAudioInfo.enSampleRate;
		stAitAttr.unAttr.stHDMIAttr.enBitDepth = stAudioInfo.enBitDepth;
		stAitAttr.unAttr.stHDMIAttr.enChannel = stAudioInfo.enChannel;
        stAitAttr.unAttr.stHDMIAttr.enHdmiAudioDataFormat = stAudioInfo.enHdmiAudioDataFormat;
    	if(HI_SUCCESS != Ret)
    	{
    		printf("Get Default Ai Attr Failed \n");
            return HI_FAILURE;
    	}
    	
    	Ret = HI_UNF_AI_Create(HI_UNF_AI_HDMI0, &stAitAttr, &hAi);
        sample_printf("end HI_UNF_AI_Create \n");
    	
    	if(HI_SUCCESS != Ret)
    	{
    		printf("Ai Create Failed \n");
            return HI_FAILURE;
    	}
    	
    	Ret = HIADP_Snd_Init();
    	if (Ret != HI_SUCCESS)
    	{
    		printf("call SndInit failed.\n");
            return HI_FAILURE;
    	}
    	
    	Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_SLAVE, &stTrackAttr);
    	if (Ret != HI_SUCCESS)
    	{
    		printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
            return HI_FAILURE;
    	}
    	Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrackSlave);
    	if (Ret != HI_SUCCESS)
    	{
    		printf("call HI_UNF_SND_CreateTrack failed.\n");
            return HI_FAILURE;
    	}
    	
    	Ret = HI_UNF_SND_Attach(hTrackSlave, hAi);
    	if (Ret != HI_SUCCESS)
    	{
    		printf("call HI_UNF_SND_Attach failed.\n");
            return HI_FAILURE;
    	}
    	
    	Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_VIRTUAL, &stTrackAttr);
    	if (Ret != HI_SUCCESS)
    	{
    		printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
            return HI_FAILURE;
    	}
    	
    	Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrackVir);
    	if (Ret != HI_SUCCESS)
    	{
    		printf("call HI_UNF_SND_CreateTrack failed.\n");
            return HI_FAILURE;
    	}
    	
    	Ret = HI_UNF_SND_Attach(hTrackVir, hAi);
    	if (Ret != HI_SUCCESS)
    	{
    		printf("call HI_UNF_SND_Attach failed.\n");
            return HI_FAILURE;
    	}
				
    	Ret = HI_UNF_AI_SetEnable(hAi, HI_TRUE);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_SND_Attach failed.\n");
            return HI_FAILURE;
        }

        while (1)
        {
            printf("please input the q to quit!\n");
            SAMPLE_GET_INPUTCMD(InputCmd);

            if ('q' == InputCmd[0])
            {
                printf("prepare to quit!\n");
                break;
            }
            
        }


		HI_UNF_AI_SetEnable(hAi, HI_FALSE);
		HI_UNF_SND_Detach(hTrackSlave, hAi);
		HI_UNF_SND_Detach(hTrackVir, hAi);
		HI_UNF_SND_DestroyTrack(hTrackSlave);
		HI_UNF_SND_DestroyTrack(hTrackVir);
		HIADP_Snd_DeInit();
		HI_UNF_AI_Destroy(hAi);
		HI_UNF_AI_DeInit();
		HI_SYS_DeInit();
	}
			
    return 0;
}


#else

HI_S32 hdmi_audio(HI_VOID)
{
    HI_S32 Ret;
    HI_HANDLE hAi;
    HI_UNF_AI_ATTR_S stAitAttr;
    HI_HANDLE hTrackSlave;
    HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;
    HI_HANDLE hTrackVir;
	HI_CHAR InputCmd[32];

    Ret = HI_UNF_AI_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("call AiInit failed.\n");
        goto SYS_DEINIT;
    }
    Ret = HI_UNF_AI_GetDefaultAttr(HI_UNF_AI_I2S0,&stAitAttr);


    stAitAttr.unAttr.stI2sAttr.stAttr.bMaster = HI_FALSE;

    stAitAttr.unAttr.stI2sAttr.stAttr.enBitDepth = HI_UNF_I2S_BIT_DEPTH_16;	//default 16 Bit

    stAitAttr.u32PcmFrameMaxNum = 8;
    if(HI_SUCCESS != Ret)
    {
        printf("Get Default Ai Attr Failed \n");
        goto AI_DEINIT;
    }

    Ret = HI_UNF_AI_Create(HI_UNF_AI_I2S0, &stAitAttr, &hAi);
    if(HI_SUCCESS != Ret)
    {
        printf("Ai Create Failed \n");
        goto AI_DEINIT;
    }

    Ret = HIADP_Snd_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call SndInit failed.\n");
        goto AI_DESTROY;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_SLAVE, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto SND_DEINIT;
    }
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrackSlave);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto SND_DEINIT;
    }

    Ret = HI_UNF_SND_Attach(hTrackSlave, hAi);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_Attach failed.\n");
        goto TRACK_SLAVE_DESTROY;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_VIRTUAL, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto TRACK_SLAVE_DETACH;
    }

    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrackVir);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto TRACK_SLAVE_DESTROY;
    }

    Ret = HI_UNF_SND_Attach(hTrackVir, hAi);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_Attach failed.\n");
        goto TRACK_VIR_DESTROY;
    }

    Ret = HI_UNF_AI_SetEnable(hAi, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_Attach failed.\n");
        goto TRACK_VIR_DETACH;
    }


    while (1)
    {
        printf("please input the q to quit!\n");
        SAMPLE_GET_INPUTCMD(InputCmd);
    
        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }
        
    }

    HI_UNF_AI_SetEnable(hAi, HI_FALSE);

TRACK_VIR_DETACH:
    HI_UNF_SND_Detach(hTrackVir, hAi);
TRACK_VIR_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrackVir);
TRACK_SLAVE_DETACH:
    HI_UNF_SND_Detach(hTrackSlave, hAi);
TRACK_SLAVE_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrackSlave);
SND_DEINIT:
    HIADP_Snd_DeInit();
AI_DESTROY:
    HI_UNF_AI_Destroy(hAi);
AI_DEINIT:
    HI_UNF_AI_DeInit();
SYS_DEINIT:
    HI_SYS_DeInit();

    return HI_SUCCESS;
}

#endif


HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 Ret;
    HI_UNF_ENC_FMT_E enFormat;
    HI_HANDLE hWin, hVi;
    HI_UNF_VI_ATTR_S stViAttr;

    Ret = HI_UNF_VI_GetDefaultAttr(&stViAttr);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_VI_GetDefaultAttr failed\n");
        goto ERR4;
    }

    if (argc > 2)
    {
        printf("Usage: %s [vo_format]\n"
               "    [vo_format]: [720P_50] | 720P_60 | 1080P_30 | 1080i_50 | 1080i_60\n",
               argv[0]);
        printf("Example:%s 720P_50\n", argv[0]);
        return -1;
    }
    else if (argc == 2)
    {
        enFormat = HIADP_Disp_StrToFmt(argv[1]);
    }
    else
    {
        printf("Usage: %s [vo_format][Width][Height][X][Y]\n"
               "    [vo_format]: [720P_50] 2160P_30 | 2160P_24 | 720P_60 | 1080P_30 | 1080i_50 | 1080i_60\n",
               argv[0]);
        printf("Example:%s 720P_50\n", argv[0]);
        enFormat = HI_UNF_ENC_FMT_720P_50;
    }


    HI_SYS_Init();
    HIADP_MCE_Exit();

    Ret = HIADP_Disp_Init(enFormat);
    if (HI_SUCCESS != Ret)
    {
        printf("HIADP_Disp_Init failed\n");
        goto ERR_SYS_Deint;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    if (HI_SUCCESS != Ret)
    {
        printf("HIADP_VO_Init failed\n");
        goto ERR0;
    }

    Ret = HIADP_VO_CreatWin(HI_NULL, &hWin);
    if (HI_SUCCESS != Ret)
    {
        printf("HIADP_VO_CreatWin failed\n");
        goto ERR1;
    }
    
    Ret = HI_ADP_HDMIRX_Init_GetViAttr(&stViAttr);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_ADP_HDMIRX_Init failed\n");
        goto ERR2;
    }

    sleep(1);
    Ret = HI_UNF_VI_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_VI_Init failed\n");
        goto ERR3;
    }


    Ret = HI_UNF_VI_Create(HI_UNF_VI_PORT0, &stViAttr, &hVi);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_VI_Create failed\n");
        goto ERR4;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, hVi);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_VO_AttachWindow failed\n");
        goto ERR5;
    }

    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto ERR6;
    }
    HI_UNF_VO_SetQuickOutputEnable(hWin, HI_TRUE);
    Ret = HI_UNF_VI_Start(hVi);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_VI_Start failed.\n");
        goto ERR7;
    }

    printf("press any key to quit!\n");
     //getchar();

    hdmi_audio();

    HI_UNF_VI_Stop(hVi);
ERR7:
    HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
ERR6:
    HI_UNF_VO_DetachWindow(hWin, hVi);
ERR5:
    HI_UNF_VI_Destroy(hVi);
ERR4:
    HI_UNF_VI_DeInit();
ERR3:
    HI_ADP_HDMIRX_DeInit();
ERR2:
    HI_UNF_VO_DestroyWindow(hWin);
ERR1:
    HIADP_VO_DeInit();
ERR0:
    HIADP_Disp_DeInit();
ERR_SYS_Deint:
    HI_SYS_DeInit();

    return 0;
}
