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
#include <sys/stat.h>

#include "hi_unf_common.h"
#include "hi_unf_ecs.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_mpi.h"
#include "hi_adp_search.h"

#include "fv_threads.h"
#include "fv_face.h"


static FILE               *g_pTsFile = HI_NULL;
static pthread_t g_TsThd;
static pthread_t g_searchThd;
static pthread_mutex_t g_TsMutex;
static HI_BOOL g_tsplay_exit_flag = HI_FALSE;
static HI_HANDLE g_TsBuf;
static PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;
static HI_HANDLE g_hAvplay = HI_INVALID_HANDLE;
static HI_CHAR g_tsfile[256];
static unsigned int g_hWin   = HI_NULL;
static HI_U32 g_tsfile_exist = 0;

#define  PLAY_DMX_ID 0

void isTsFileExist(int exist)
{
    g_tsfile_exist = exist;
}

void getTsFileState(int *exist)
{
    *exist = g_tsfile_exist;
}

HI_S32  prepare_avplay()
{
    HI_S32 Ret;
    HI_UNF_AVPLAY_ATTR_S AvplayAttr;
    HI_UNF_SYNC_ATTR_S SyncAttr;
    HI_HANDLE                   hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S    stTrackAttr;

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Snd_Init failed.\n");
        goto SND_DEINIT;
    }

    Ret  = HI_UNF_DMX_Init();
    Ret |= HI_UNF_DMX_AttachTSPort(PLAY_DMX_ID, HI_UNF_DMX_PORT_RAM_0);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Demux_Init failed.\n");
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_0, 0x200000, &g_TsBuf);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_DMX_CreateTSBuffer failed.\n");
        goto TSBUF_FREE;
    }

    Ret  = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    AvplayAttr.u32DemuxId = PLAY_DMX_ID;
    AvplayAttr.stStreamAttr.u32VidBufSize = (9 * 1024 * 1024);
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &g_hAvplay);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto AUD_CHN_CLOSE;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto VID_CHN_CLOSE;
    }

    Ret = HI_UNF_VO_AttachWindow(g_hWin, g_hAvplay);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_VO_AttachWindow failed.\n");
        goto VID_CHN_CLOSE;
    }

    Ret = HI_UNF_VO_SetWindowEnable(g_hWin, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    }
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_Attach(hTrack, g_hAvplay);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_Attach failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(g_hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    Ret = HI_UNF_AVPLAY_SetAttr(g_hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    return HI_SUCCESS;

SND_DETACH:
    HI_UNF_SND_Detach(HI_UNF_SND_0, g_hAvplay);

WIN_DETACH:
    HI_UNF_VO_SetWindowEnable(g_hWin, HI_FALSE);
    HI_UNF_VO_DetachWindow(g_hWin, g_hAvplay);

AUD_CHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

VID_CHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID );

    HI_UNF_AVPLAY_Destroy(g_hAvplay);
AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

TSBUF_FREE:
    HI_UNF_DMX_DestroyTSBuffer(g_TsBuf);

DMX_DEINIT:
    HI_UNF_DMX_DetachTSPort(0);
    HI_UNF_DMX_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();
    g_hAvplay = HI_INVALID_HANDLE;
    return Ret;
}

HI_S32  release_avplay()
{
    HI_UNF_SND_Detach(HI_UNF_SND_0, g_hAvplay);
    HI_UNF_VO_SetWindowEnable(g_hWin, HI_FALSE);
    HI_UNF_VO_DetachWindow(g_hWin, g_hAvplay);
    HI_UNF_AVPLAY_ChnClose(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    HI_UNF_AVPLAY_Destroy(g_hAvplay);
    g_hAvplay = HI_NULL;
    HI_UNF_AVPLAY_DeInit();
    HI_UNF_DMX_DetachTSPort(0);
    HI_UNF_DMX_DeInit();
    HIADP_Snd_DeInit();
    return HI_SUCCESS;
}

HI_VOID release_psi_thread()
{
    if (g_pProgTbl)
    {
        HIADP_Search_FreeAllPmt(g_pProgTbl);
        g_pProgTbl = HI_NULL;
        HIADP_Search_DeInit();
    }

    pthread_join(g_TsThd, HI_NULL);
    pthread_join(g_searchThd, HI_NULL);
    pthread_mutex_destroy(&g_TsMutex);
}

int fv_tsplay_getVolume(unsigned int *u32Volume)
{
    HI_S32 Ret = 0;
    HI_UNF_SND_GAIN_ATTR_S stGain;

    Ret = HI_UNF_SND_GetVolume(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_HDMI0, &stGain);
    if (Ret != HI_SUCCESS)
    {
    	printf("HI_UNF_SND_GetVolume failed.0x%x\n", Ret);
        *u32Volume = 0xff;
        return Ret;
    }

    *u32Volume = stGain.s32Gain;
    return Ret;
}

int fv_tsplay_setVolume(unsigned int u32Volume)
{
    HI_S32 Ret = 0;
    HI_UNF_SND_GAIN_ATTR_S stGain;

    stGain.bLinearMode = HI_TRUE;
    stGain.s32Gain        = u32Volume % 101;
    Ret = HI_UNF_SND_SetVolume(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_HDMI0, &stGain);
    if (Ret != HI_SUCCESS)
    {
        printf("HI_UNF_AVPLAY_SetAttr Return failed:0x%x\n", Ret);
        return Ret;
    }
	
    return Ret;
}

HI_VOID search_and_play_Thread(HI_VOID *args)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 ProgNum;
    HI_UNF_AVPLAY_STOP_OPT_S Stop;
    HI_U32 vol;


    while(0 == g_tsfile_exist)
    {
        // printf("tsfile not exist!\n");
        sleep(1);
    }

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;

    while (1)
    {

        if(0 != g_tsfile_exist)
        {
            HIADP_Search_Init();
            Ret = HIADP_Search_GetAllPmt(PLAY_DMX_ID, &g_pProgTbl);
            if (Ret != HI_SUCCESS)
            {
                printf("call HIADP_Search_GetAllPmt failed.\n");
                HIADP_Search_FreeAllPmt(g_pProgTbl);
                g_pProgTbl = HI_NULL;
                HIADP_Search_DeInit();
                continue;
            }

            ProgNum = 0;

            pthread_mutex_lock(&g_TsMutex);
            rewind(g_pTsFile);
            HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
            pthread_mutex_unlock(&g_TsMutex);

            Ret = HIADP_AVPlay_PlayProg(g_hAvplay, g_pProgTbl, ProgNum, HI_TRUE);
            if (Ret != HI_SUCCESS)
            {
                printf("call HIADP_AVPlay_PlayProg failed.\n");
                HI_UNF_AVPLAY_Stop(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
                HIADP_Search_FreeAllPmt(g_pProgTbl);
                g_pProgTbl = HI_NULL;
                HIADP_Search_DeInit();
                continue;
            }
            fv_tsplay_getVolume(&vol);
            if(vol >= 0 && vol <= 100)
            {
	      fv_draw_vol(vol);
	    }
            break;
        }
    }

    return;
}

HI_VOID TsTthread(HI_VOID *args)
{
    HI_UNF_STREAM_BUF_S StreamBuf;
    HI_S32 Readlen;
    HI_S32 Ret = HI_SUCCESS;
    HI_UNF_AVPLAY_STOP_OPT_S Stop;
    HI_U32 vol;

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs  = 0;

    while (1)
    {
        if(0 != g_tsfile_exist)
        {
            g_pTsFile = fopen(g_tsfile, "rb");
            if (!g_pTsFile)
            {
                printf("open file %s error!\n", g_tsfile);
                fclose(g_pTsFile);
                g_pTsFile = HI_NULL;
                continue;
            }
            else
            {
                break;
            }
        }

        sleep(1);
    }

    while (!g_tsplay_exit_flag)
    {
        while (!g_tsplay_exit_flag)
        {
            pthread_mutex_lock(&g_TsMutex);
            Ret = HI_UNF_DMX_GetTSBuffer(g_TsBuf, 188 * 50, &StreamBuf, 1000);
            if (Ret != HI_SUCCESS)
            {
                pthread_mutex_unlock(&g_TsMutex);
                continue;
            }

            if(g_tsfile_exist == 0)
            {
                printf("READ FILE ERROR !!!!!\n");
                pthread_mutex_unlock(&g_TsMutex);
                if (g_pTsFile)
                {
                    fclose(g_pTsFile);
		    g_tsfile_exist = -1;
                    g_pTsFile = HI_NULL;
                }

                HI_UNF_AVPLAY_Stop(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
	        fv_draw_vol(0);
                break;
            }

            Readlen = fread(StreamBuf.pu8Data, sizeof(HI_S8), 188 * 50, g_pTsFile);
            if (Readlen == 0)
            {
                //printf("read ts file end and rewind!\n");
                rewind(g_pTsFile);
                pthread_mutex_unlock(&g_TsMutex);
                continue;
            }
            else if (Readlen < 0)
            {
                perror("Fail to read!\n");
                return;
            }

            Ret = HI_UNF_DMX_PutTSBuffer(g_TsBuf, Readlen);
            if (Ret != HI_SUCCESS)
            {
                printf("call HI_UNF_DMX_PutTSBuffer failed.\n");
            }

            pthread_mutex_unlock(&g_TsMutex);
        }

        while ((!g_tsplay_exit_flag) && ((0 == g_tsfile_exist) || -1 == g_tsfile_exist))
        {
            sleep(1);
        }

        if (g_tsplay_exit_flag)
        {
            break;
        }
        else
        {
            while (1)
            {
                    g_pTsFile = fopen(g_tsfile, "rb");
                    if (!g_pTsFile)
                    {
                        printf("open file %s error!\n", g_tsfile);
                        fclose(g_pTsFile);
                        g_pTsFile = HI_NULL;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                
                sleep(1);
            }
            do{
            
            	Ret = HI_UNF_AVPLAY_Start(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        
	     }while(Ret != HI_SUCCESS);
            fv_tsplay_getVolume(&vol);
            if(vol >= 0 && vol <= 100)
            {
	      fv_draw_vol(vol);
	    }	     
        }
    }

    Ret = HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_DMX_ResetTSBuffer failed.\n");
    }

    return;
}

int fv_tsplay_enter(char tsfile[])
{
    HI_S32 Ret = 0;
    if (strlen(tsfile) == 0)
    {
        printf("Please Load corret tsfile\n");
        return -1;
    }
    
    strcpy(g_tsfile, tsfile);

    HI_SYS_Init();

    Ret = fv_get_win_handle(&g_hWin);
    if (Ret != HI_SUCCESS)
    {
        printf("fv_get_win_handle failed\n");
        HI_SYS_DeInit();
        return -1;
    }

    Ret = prepare_avplay();
    if (Ret != HI_SUCCESS)
    {
        printf("Can't prepare avplay\n");
        HI_SYS_DeInit();
        return -1;
    }

    pthread_mutex_init(&g_TsMutex, NULL);
    g_tsplay_exit_flag = HI_FALSE;
    pthread_create(&g_TsThd, HI_NULL, (HI_VOID *)TsTthread, HI_NULL);
    pthread_create(&g_searchThd, HI_NULL, (HI_VOID *)search_and_play_Thread, HI_NULL);

    return 0;
}

int fv_tsplay_exit()
{
    HI_UNF_AVPLAY_STOP_OPT_S Stop;

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs  = 0;
    g_tsplay_exit_flag = HI_TRUE;
    HI_UNF_AVPLAY_Stop(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
    fv_draw_vol(0);
    release_psi_thread();
    release_avplay();
    fv_destroy_win_handle();
    g_tsplay_exit_flag = HI_FALSE;
    if (g_pTsFile)
    {
        fclose(g_pTsFile);
        g_pTsFile = HI_NULL;
    }

    return 0;
}

