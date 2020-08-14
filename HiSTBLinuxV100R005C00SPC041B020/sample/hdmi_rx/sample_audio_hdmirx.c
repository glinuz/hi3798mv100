/******************************************************************************

  Copyright (C), 2001-2050, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     :
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/01/27
  Description   :
  History       :
  1.Date        : 2014/01/27
    Author      : t00202585
    Modification: Created file

******************************************************************************/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <linux/fb.h>
#include <assert.h>
#include <pthread.h>
#include <sys/wait.h>

#include "hi_type.h"
//#include "hi_unf_tvd.h"
#include "hi_unf_vi.h"
#include "hi_unf_vo.h"
#include "hi_unf_disp.h"
//#include "hi_unf_panel.h"
#include "hi_adp_mpi.h"
#include "hi_unf_hdmirx.h"

#include "hi_unf_common.h"
#include "hi_unf_ai.h"
#include "hi_unf_sound.h"
#include "hi_adp_mpi.h"
//#include "hi_amp.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

#define SAMPLE_PRINT_FUNC(str)\
    do{\
        printf("test case <%s> %s\n",__FUNCTION__, str);\
    }while(0)


HI_U32 g_u32FileSaveLimitSize = 100;
FILE *g_pAIFile = HI_NULL;
static pthread_t g_VirThd;
static HI_BOOL g_StopThread = HI_FALSE;

	
HI_VOID VirTthread(HI_VOID *args)
{
	HI_S32 Ret;
	HI_UNF_AO_FRAMEINFO_S  stAiFrame;
	HI_UNF_AO_FRAMEINFO_S *pstAiFrame = &stAiFrame;
	HI_U32 FrameSize = 0;
	HI_U32 u32TotalCounter=0;
	HI_HANDLE hVirTra = *(HI_HANDLE *)args; 

    g_pAIFile = fopen("./vir.pcm", "wb");
    if (!g_pAIFile)
    {
        sample_printf("open file %s error!\n", "./vir.pcm");
        return;
    }

	while (!g_StopThread)
	{
		Ret = HI_UNF_SND_AcquireTrackFrame(hVirTra, pstAiFrame, 0);
		if(HI_SUCCESS != Ret)
		{
            usleep(5*1000);
            continue;
		}
		FrameSize = pstAiFrame->u32PcmSamplesPerFrame*pstAiFrame->u32Channels*pstAiFrame->s32BitPerSample /8;
		u32TotalCounter += FrameSize;
		if(u32TotalCounter <= g_u32FileSaveLimitSize*1024*1024)
		{
			//fwrite(pstAiFrame->ps32PcmBuffer, 1, FrameSize, g_pAIFile);
		}

		HI_UNF_SND_ReleaseTrackFrame(hVirTra, pstAiFrame);
	}
	
	if(g_pAIFile)
	{
		fclose(g_pAIFile);	
		g_pAIFile = HI_NULL;
	}

	return;
}



int main(int argc, char *argv[])
{
    //HI_CHAR s8Cmd[1];
    
    //printf("Switch To Hdmirx \n");
    //HI_HANDLE hWindow;
    HI_UNF_SIG_STATUS_E enStatus;
    HI_UNF_HDMIRX_TIMING_INFO_S stTimingInfo;
	
	HI_S32 Ret;
	HI_HANDLE hAi;
	HI_UNF_AI_ATTR_S stAitAttr;
	HI_HANDLE hTrackSlave;
	HI_HANDLE hTrackVir;
	HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;
	HI_UNF_AI_HDMI_ATTR_S stAudioInfo;

	HI_CHAR InputCmd[32];
    
    //HIADP_StartAtvMoniter();
    HI_UNF_HDMIRX_Init();
    HI_UNF_HDMIRX_Connect(1);

	sleep(2);
    while(1)
    {
        printf("Start get HDMI signal status \n");
        enStatus = HI_UNF_HDMIRX_GetSignalStatus(&enStatus);
        if(enStatus == HI_UNF_SIG_SUPPORT)
        {

			enStatus = HI_UNF_HDMIRX_GetTimingInfo(&stTimingInfo);
			if (HI_FAILURE != enStatus)
			{
				printf("u32Width;		 %d\n", stTimingInfo.u32Width		 );
				printf("u32Height;		 %d\n", stTimingInfo.u32Height		 );
				printf("u32FrameRate;	 %d\n", stTimingInfo.u32FrameRate	 );
				printf("enColorSpace;0:601,1:709,2:RGB	   %d\n", stTimingInfo.enColorSpace    );
				printf("enPixelFmt;0:RGB,1:420,2:422, 3:444 	 %d\n", stTimingInfo.enPixelFmt 	 );
				printf("enBitWidth;0:8bit,1:10bit,2:12bit	   %d\n", stTimingInfo.enBitWidth	   );
				printf("bInterlace;0:Procc;1:Inter		%d\n", stTimingInfo.bInterlace		);
				printf("enOverSampleMode;0:1X;1:2X,2:4X  %d\n", stTimingInfo.enOversample);
				printf("en3dFmt; 0:2D		 %d\n", stTimingInfo.en3dFmt);
				printf("bDviMode;		 %d\n", stTimingInfo.bHdmiMode		  );
				printf("u32Vblank;		 %d\n", stTimingInfo.u32Vblank		 );
				printf("bPcMode;	   %d\n", stTimingInfo.bPcMode		 );
				printf("u32TimingIndex; 	  %d\n", stTimingInfo.u32TimingIdx);
				//break;
			}
			sleep(1);
            enStatus = HI_UNF_HDMIRX_GetAudioInfo(&stAudioInfo);
			if (HI_FAILURE != enStatus)
			{
	            printf("enChannel:%d\n", stAudioInfo.enChannel);
	            printf("enBitDepth:%d\n", stAudioInfo.enBitDepth);
	            printf("enSampleRate:%d\n", stAudioInfo.enSampleRate);
	            printf("enHdmiAudioDataFormat:%d\n", stAudioInfo.enHdmiAudioDataFormat);
				sleep(1);
				{
		
					HI_SYS_Init();
					Ret = HI_UNF_AI_Init();
					if (HI_SUCCESS != Ret)
					{
						sample_printf("call AiInit failed.\n");
						break;
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
						break;
					}
					
					Ret = HI_UNF_AI_Create(HI_UNF_AI_HDMI0, &stAitAttr, &hAi);
                    printf("end HI_UNF_AI_Create \n");
					
					if(HI_SUCCESS != Ret)
					{
						printf("Ai Create Failed \n");
						break;
					}
					
					Ret = HIADP_Snd_Init();
					if (Ret != HI_SUCCESS)
					{
						sample_printf("call SndInit failed.\n");
						break;
					}
					
					Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_SLAVE, &stTrackAttr);
					if (Ret != HI_SUCCESS)
					{
						sample_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
						break;
					}
					Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrackSlave);
					if (Ret != HI_SUCCESS)
					{
						sample_printf("call HI_UNF_SND_CreateTrack failed.\n");
						break;
					}
					
					Ret = HI_UNF_SND_Attach(hTrackSlave, hAi);
					if (Ret != HI_SUCCESS)
					{
						sample_printf("call HI_UNF_SND_Attach failed.\n");
						break;
					}
					
					Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_VIRTUAL, &stTrackAttr);
					if (Ret != HI_SUCCESS)
					{
						sample_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
						break;
					}
					
					Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrackVir);
					if (Ret != HI_SUCCESS)
					{
						sample_printf("call HI_UNF_SND_CreateTrack failed.\n");
						break;
					}
					
					Ret = HI_UNF_SND_Attach(hTrackVir, hAi);
					if (Ret != HI_SUCCESS)
					{
						sample_printf("call HI_UNF_SND_Attach failed.\n");
						break;
					}
					
					Ret = HI_UNF_AI_SetEnable(hAi, HI_TRUE);
				    if (Ret != HI_SUCCESS)
				    {
				        sample_printf("call HI_UNF_SND_Attach failed.\n");
				        break;
				    }
					
				//	HI_AMP_Init();
				//	HI_AMP_SetMute(HI_FALSE);

					g_StopThread = HI_FALSE;
				    pthread_create(&g_VirThd, HI_NULL, (HI_VOID *)VirTthread, &hTrackVir);
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

				    g_StopThread = HI_TRUE;
				    pthread_join(g_VirThd, HI_NULL);

					{
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
				}
			}
			else
			{
				printf("Can not get audio info \n");
			}
			
            break;
        }
        else if(enStatus == HI_UNF_SIG_UNSTABLE)
        {
            printf("unstable \n");
        }
        else if(enStatus == HI_UNF_SIG_NO_SIGNAL)
        {
            printf("no signal \n");
            break;
        }
		else
	    {
	        printf("no support \n");
	    }
        usleep(1000*300);;
    }
     	
    HI_UNF_HDMIRX_DeInit();
    return 0;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

