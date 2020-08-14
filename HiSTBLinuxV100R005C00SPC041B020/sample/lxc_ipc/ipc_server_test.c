/******************************************************************************

  Copyright (C), 2016-2026, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ipc_server_test.c
  Version       : Initial Draft
  Author        : Hisilicon security software group
  Created       : 2016/04/13
  Description   :
  History       :
  1.Date        : 2016/04/13
    Author      : security team
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
#include <string.h>
#include <pthread.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_unf_hdmi.h"
#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"
#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"


#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf(pszFormat...)
#else
#define sample_printf printf
#endif

HI_U32 g_TunerFreq;
HI_U32 g_TunerSrate;
HI_U32 g_ThirdParam;
static HI_UNF_ENC_FMT_E   g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;

PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;



static HI_S32 server_socket_select(int fd) 
{
    int rc = -1;
    fd_set rfds;
    struct timeval tv;
    int timeoutInterval = 200; /* 200msec */

    while (1) 
    {        
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        tv.tv_sec = 0; 
        tv.tv_usec = timeoutInterval * 1000; /* in micro-secs */

        rc = select(fd +1, &rfds, NULL, NULL, &tv);
        if (rc < 0) 
        {
            if (errno == EINTR) 
            {
                sample_printf("%s: select is interrupted, re-try\n", __FUNCTION__);
                continue;
            }
            sample_printf("%s: ERROR: select(): rc=%d, errno=%d", __FUNCTION__, rc, errno);
            return HI_FAILURE;
        }

        if (rc == 0 || !FD_ISSET(fd, &rfds)) 
        {
            continue;
        }
        
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}


#define TEST_IPC_PORT 6000
#define MAX_BUF 1024

static int IPC_ServerCreate(HI_S32 *pu32ServerFd)
{
    HI_S32 server_fd;
    struct sockaddr_in server_addr;
    int reuse = 1;

    sample_printf("%s: creat socket \n", __FUNCTION__);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( server_fd < 0) 
    {
        /* Socket Create Error */
        perror("Socket Open Err \n");
        return -EINVAL;
    }    

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0) 
    {
        sample_printf("REUSE socket error\n");
        close(server_fd);
        return HI_FAILURE;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;   
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(TEST_IPC_PORT);
    

    sample_printf("%s: bind local address to socket \n", __FUNCTION__);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))) 
    {
        sample_printf("Socket bind error");
        close(server_fd);
        return HI_FAILURE;
    }

    //chmod(localAddr.sun_path, 0666);

    sample_printf("%s: start listen \n", __FUNCTION__);
    if (listen(server_fd, 10)) 
    {
        sample_printf("Socket listen error");
        close(server_fd);
        return HI_FAILURE;
    }  

    *pu32ServerFd = server_fd;
    return HI_SUCCESS;

}



HI_S32 IPC_ServerNewFd(HI_S32 server_fd, HI_S32 *pu32NewFd)
{
    HI_S32 new_fd=0; 
    
    if (server_socket_select(server_fd))
    {
       sample_printf("Fail to Waiting for connection");
       return HI_FAILURE;
    }

    if ((new_fd = accept(server_fd, NULL, NULL)) == -1)
    {
       sample_printf("Socket accept error \n");
       return HI_FAILURE;
    }

    if (pu32NewFd != HI_NULL)
    {
       *pu32NewFd = new_fd;
    }

    return HI_SUCCESS;
}


HI_S32 IPC_ServerSendTotalProNum(HI_S32 new_fd, HI_U32 prog_num)
{ 
    HI_S32 len;    
    char   snd_buffer[MAX_BUF + 1];
    HI_U32 *pu32Buf;

    sample_printf("Total program number is %d \n", prog_num);
    memset(snd_buffer, 0x0, sizeof(snd_buffer));
    pu32Buf = (HI_U32 *)snd_buffer;
    *pu32Buf = prog_num;   
    
    len = write(new_fd, snd_buffer, strlen(snd_buffer));
    if (len < 0)
    {
        sample_printf ("%s:Fail to send first confirm data\n",__FUNCTION__);
       return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 IPC_ServerGetProgramNum(HI_S32 new_fd, HI_U32 *pu32PlayNum)
{
    HI_S32 len;    
    char   rev_buffer[MAX_BUF + 1];
    char   snd_buffer[MAX_BUF + 1];
    HI_U32 u32PlayNumber = 1;
    HI_U32 *pu32Buf;
          
   memset(rev_buffer, 0x0, sizeof(rev_buffer));
   len = read(new_fd, rev_buffer, MAX_BUF);
   if (len < 0 )
   {
      sample_printf ("%s:Fail to read data\n",__FUNCTION__);
      return HI_FAILURE;
   }
   pu32Buf = (HI_U32 *)rev_buffer;
   u32PlayNumber = *pu32Buf;
   if (pu32PlayNum != HI_NULL)
   {
     if (u32PlayNumber > 0 && u32PlayNumber < 1024)
	     *pu32PlayNum = u32PlayNumber;
   }

   sample_printf("Server received command to play program %d......\n", u32PlayNumber);
   /***************************************************************************/

   memset(snd_buffer, 0x0, sizeof(snd_buffer));
   snprintf(snd_buffer, sizeof(snd_buffer), "Sever Play program %d OK!!!!!\n", u32PlayNumber); 
   len = write(new_fd, snd_buffer, strlen(snd_buffer));
   if (len < 0)
   {
       sample_printf ("%s:Fail to send confirm data to client\n",__FUNCTION__);
       return HI_FAILURE;
   }       
   sample_printf("****************Server****************\n");  
    
    return HI_SUCCESS;
}


static void usage(HI_CHAR *sample_name)
{
     printf("Usage: %s [freq] [srate] [qamtype or polarization] [vo_format] [tuner]\n"
                "       qamtype or polarization: \n"
                "           For cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64] \n"
                "           For satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0] \n"
                "       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50  default HI_UNF_ENC_FMT_1080i_50\n"
                "       Tuner: value can be 0, 1, 2, 3\n",
                sample_name);

        printf("Example: %s 618 6875 64 1080i_50 0\n", sample_name);
        printf("  Board HI3796CDMO1B support 3 tuners: \n");
        printf("    Satellite : %s 3840 27500 0 1080i_50 0\n", sample_name);
        printf("    Cable     : %s 618 6875 64 1080i_50 1\n", sample_name);
        printf("    Terestrial: %s 474 8000 64 1080i_50 2\n", sample_name);
    return;
}


HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32                  Ret;
    HI_HANDLE               hWin;

    HI_HANDLE               hAvplay;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    HI_U32                  ProgNum;

    HI_HANDLE hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;
    HI_U32                      Tuner;

    HI_S32 server_fd = -1;
    HI_S32 new_fd = -1;
    
    if (6 == argc)
    {
        g_TunerFreq  = strtol(argv[1],NULL,0);
        g_TunerSrate = strtol(argv[2],NULL,0);
        g_ThirdParam = strtol(argv[3],NULL,0);
        g_enDefaultFmt = HIADP_Disp_StrToFmt(argv[4]);
        Tuner = strtol(argv[5],NULL,0);
    }
    else if (5 == argc)
    {
        g_TunerFreq  = strtol(argv[1],NULL,0);
        g_TunerSrate = strtol(argv[2],NULL,0);
        g_ThirdParam = strtol(argv[3],NULL,0);
        g_enDefaultFmt = HIADP_Disp_StrToFmt(argv[4]);
        Tuner = 0;
    }
    else if (4 == argc)
    {
        g_TunerFreq  = strtol(argv[1],NULL,0);
        g_TunerSrate = strtol(argv[2],NULL,0);
        g_ThirdParam = strtol(argv[3],NULL,0);
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
        Tuner = 0;
    }
    else if(3 == argc)
    {
        g_TunerFreq  = strtol(argv[1],NULL,0);
        g_TunerSrate = strtol(argv[2],NULL,0);
        g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
        Tuner = 0;
    }
    else if(2 == argc)
    {
        g_TunerFreq  = strtol(argv[1],NULL,0);
        g_TunerSrate = (g_TunerFreq>1000) ? 27500 : 6875;
        g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
        Tuner = 0;
    }
    else
    {
        usage(argv[0]);
        return HI_FAILURE;
    }
    
    HI_SYS_Init();
    
    IPC_ServerCreate(&server_fd);

    HIADP_MCE_Exit();

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Snd_Init failed.\n");
        goto SYS_DEINIT;
    }
    //HI_SYS_GetPlayTime(& u32Playtime);
    //sample_printf("u32Playtime = %d\n",u32Playtime);
    Ret = HIADP_Disp_Init(g_enDefaultFmt);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Disp_DeInit failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,&hWin);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
    Ret |= HIADP_DMX_AttachTSPort(0, Tuner);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HIADP_Demux_Init failed.\n");
        goto VO_DEINIT;
    }

    Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Demux_Init failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HIADP_Tuner_Connect(Tuner, g_TunerFreq, g_TunerSrate, g_ThirdParam);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Tuner_Connect failed.\n");
        goto TUNER_DEINIT;
    }

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(0,&g_pProgTbl);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Search_GetAllPmt failed\n");
        goto PSISI_FREE;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto PSISI_FREE;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    AvplayAttr.u32DemuxId = 0;
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    Ret |= HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto CHN_CLOSE;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_VO_AttachWindow failed.\n");
        goto CHN_CLOSE;
    }
    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    }
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrack);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_Attach(hTrack, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_Attach failed.\n");
        goto TRACK_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    SyncAttr.stSyncStartRegion.s32VidPlusTime = 60;
    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -20;
    SyncAttr.bQuickOutput = HI_FALSE;
    Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    ProgNum = 1;
    Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum,HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call SwitchProg failed.\n");
        goto AVPLAY_STOP;
    }

    Ret = IPC_ServerNewFd(server_fd, &new_fd);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("Fail to new Fd.\n");
        goto AVPLAY_STOP;
    }
    
    Ret = IPC_ServerSendTotalProNum(new_fd, g_pProgTbl->prog_num);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("Fail to get program numbers.\n");
        goto AVPLAY_STOP;
    }

    while(1)
    {
        if (server_socket_select(new_fd))
        {
           usleep(2*1000);
           continue;
        }         

        if(HI_SUCCESS != IPC_ServerGetProgramNum(new_fd, &ProgNum))
        {
           continue;
        }

    	if(ProgNum == 0)
    	    ProgNum = 1;

		Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum-1,HI_TRUE);
		if (Ret != HI_SUCCESS)
		{
			sample_printf("call SwitchProgfailed!\n");
			break;
		}
    }

AVPLAY_STOP:
    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;
    HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);

SND_DETACH:
    HI_UNF_SND_Detach(hTrack, hAvplay);

TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrack);

WIN_DETACH:
    HI_UNF_VO_SetWindowEnable(hWin,HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, hAvplay);

CHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

    HI_UNF_AVPLAY_Destroy(hAvplay);

AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

PSISI_FREE:
    HIADP_Search_FreeAllPmt(g_pProgTbl);
    HIADP_Search_DeInit();

TUNER_DEINIT:
    HIADP_Tuner_DeInit();

DMX_DEINIT:
    HI_UNF_DMX_DetachTSPort(0);
    HI_UNF_DMX_DeInit();

VO_DEINIT:
    HI_UNF_VO_DestroyWindow(hWin);
    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();

    if (server_fd != -1)
    {
       close(server_fd);
    }

    if (new_fd != -1)
    {
       close(new_fd);
    }

    return Ret;
}
