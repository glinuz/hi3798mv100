/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ipplay.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/26
  Description   :
  History       :
  1.Date        : 2010/01/26
    Author      : w58735
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
#include "hi_adp_hdmi.h"
#include "hi_adp_mpi.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

#define UDP_RECV_MEM_MAX (1 * 1024 * 1024)

static pthread_t    g_SocketThd;
static HI_CHAR     *g_pMultiAddr;
static HI_U16       g_UdpPort;

static HI_BOOL          g_StopSocketThread = HI_FALSE;
static HI_HANDLE        g_hIpTsBuf;
static PMT_COMPACT_TBL *g_pIpProgTbl = HI_NULL;

static HI_VOID SocketThread(HI_VOID *args)
{
    HI_S32              SocketFd;
    struct sockaddr_in  ServerAddr;
    in_addr_t           IpAddr;
    struct ip_mreq      Mreq;
    HI_U32              AddrLen;
    HI_UNF_STREAM_BUF_S StreamBuf;
    HI_U32              ReadLen;
    HI_U32              GetBufCount = 0;
    HI_U32              ReceiveCount = 0;
    HI_S32              Ret;
    HI_S32              optVal = UDP_RECV_MEM_MAX;
    socklen_t           optLen = sizeof(HI_S32);

    SocketFd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (SocketFd < 0)
    {
        sample_printf("create socket error [%d].\n", errno);
        return;
    }

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ServerAddr.sin_port = htons(g_UdpPort);

    if (bind(SocketFd, (struct sockaddr *)(&ServerAddr), sizeof(struct sockaddr_in)) < 0)
    {
        sample_printf("socket bind error [%d].\n", errno);
        close(SocketFd);
        return;
    }

    IpAddr = inet_addr(g_pMultiAddr);
    if (IpAddr)
    {
        Mreq.imr_multiaddr.s_addr = IpAddr;
        Mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(SocketFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &Mreq, sizeof(struct ip_mreq)))
        {
            sample_printf("Socket setsockopt ADD_MEMBERSHIP error [%d].\n", errno);
            close(SocketFd);
            return;
        }
    }

    Ret = setsockopt(SocketFd, SOL_SOCKET, SO_RCVBUF, (HI_CHAR*)&optVal, optLen);
    if (0 != Ret)
    {
        sample_printf("setsockopt error 0x%x", Ret);
    }

    AddrLen = sizeof(ServerAddr);

    while (!g_StopSocketThread)
    {
        Ret = HI_UNF_DMX_GetTSBuffer(g_hIpTsBuf, 50 * 188, &StreamBuf, 0);
        if (Ret != HI_SUCCESS)
        {
            if (GetBufCount++ >= 10)
            {
                sample_printf("########## TS come too fast! #########, Ret=0x%x\n", Ret);
                GetBufCount = 0;
            }

            usleep(10 * 1000);
            continue;
        }

        GetBufCount = 0;

        ReadLen = recvfrom(SocketFd, StreamBuf.pu8Data, StreamBuf.u32Size, 0,
                           (struct sockaddr *)&ServerAddr, &AddrLen);
        if (ReadLen <= 0)
        {
            if (ReceiveCount++ >= 50)
            {
                sample_printf("########## TS come too slow or net error! #########\n");
                ReceiveCount = 0;
            }
        }
        else
        {
            ReceiveCount = 0;

            Ret = HI_UNF_DMX_PutTSBuffer(g_hIpTsBuf, ReadLen);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("HI_UNF_DMX_PutTSBuffer failed 0x%x\n", Ret);
            }
        }
    }

    close(SocketFd);
    return;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32                      Ret;
    HI_HANDLE                   hWin;
    HI_U32                      DmxId = 4;
    HI_UNF_DMX_PORT_E           RamPort = HI_UNF_DMX_PORT_RAM_0;
    HI_HANDLE                   hAvplay;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_HANDLE                   hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S    TrackAttr;
    HI_CHAR                     InputCmd[32];
    HI_U32                      ProgNum;
    HI_UNF_ENC_FMT_E            Format = HI_UNF_ENC_FMT_1080i_50;

    if (argc < 3)
    {
        printf("Usage: sample_ipplay MultiAddr UdpPort [vo_format]\n"
               "       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50\n"
           "Example:./sample_ipplay 224.0.0.1 1234 1080i_50\n");
        return 0;
    }

    g_pMultiAddr = argv[1];
    g_UdpPort = atoi(argv[2]);

    if (argc >= 4)
    {
        Format = HIADP_Disp_StrToFmt(argv[3]);
    }

    HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = HIADP_Snd_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call SndInit failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Disp_Init(Format);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call DispInit failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,&hWin);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HIADP_VO_Init failed\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("HI_UNF_DMX_Init failed 0x%x\n", Ret);
        goto VO_DEINIT;
    }

    Ret = HI_UNF_DMX_AttachTSPort(DmxId, RamPort);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("HI_UNF_DMX_AttachTSPort failed 0x%x\n", Ret);
        goto VO_DEINIT;
    }

    Ret = HI_UNF_DMX_CreateTSBuffer(RamPort, 0x200000, &g_hIpTsBuf);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("HI_UNF_DMX_CreateTSBuffer failed 0x%x\n", Ret);
        goto DMX_DEINIT;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HIADP_AVPlay_RegADecLib failed.\n");
        goto TSBUF_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("HI_UNF_AVPLAY_Init failed 0x%x\n", Ret);
        goto TSBUF_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    AvplayAttr.u32DemuxId = DmxId;
    AvplayAttr.stStreamAttr.u32VidBufSize = 0x300000;

    Ret = HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("HI_UNF_AVPLAY_Create failed 0x%x\n", Ret);
        goto AVPLAY_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("HI_UNF_AVPLAY_ChnOpen failed 0x%x\n", Ret);
        goto AVPLAY_DESTROY;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("HI_UNF_VO_AttachWindow failed 0x%x\n", Ret);
        goto AVCHN_CLOSE;
    }

    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("HI_UNF_VO_SetWindowEnable failed 0x%x\n", Ret);
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &TrackAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("HI_UNF_SND_GetDefaultTrackAttr failed 0x%x\n", Ret);
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &TrackAttr, &hTrack);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("HI_UNF_SND_CreateTrack failed 0x%x\n", Ret);
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_Attach(hTrack, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("HI_UNF_SND_Attach failed 0x%x\n", Ret);
        goto TRACK_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;

    Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("HI_UNF_AVPLAY_SetAttr failed 0x%x\n", Ret);
        goto SND_DETATCH;
    }

    g_StopSocketThread = HI_FALSE;
    pthread_create(&g_SocketThd, HI_NULL, (HI_VOID *)SocketThread, HI_NULL);

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(DmxId, &g_pIpProgTbl);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HIADP_Search_GetAllPmt failed.\n");
        goto THD_STOP;
    }

    Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pIpProgTbl,0,HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call SwitchProg failed.\n");
        goto PSISI_FREE;
    }

    while(1)
    {
        printf("please input the q to quit!\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }

        ProgNum = atoi(InputCmd);
        Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pIpProgTbl,ProgNum,HI_TRUE);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call SwitchProgfailed!\n");
            break;
        }
    }

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;
    Ret = HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
    if (Ret != HI_SUCCESS )
    {
        sample_printf("HI_UNF_AVPLAY_Stop failed 0x%x\n", Ret);
    }

PSISI_FREE:
    HIADP_Search_FreeAllPmt(g_pIpProgTbl);
    HIADP_Search_DeInit();

THD_STOP:
    g_StopSocketThread = HI_TRUE;
    pthread_join(g_SocketThd, HI_NULL);

SND_DETATCH:
    HI_UNF_SND_Detach(hTrack, hAvplay);

TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrack);

WIN_DETACH:
    HI_UNF_VO_SetWindowEnable(hWin,HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, hAvplay);

AVCHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

AVPLAY_DESTROY:
    HI_UNF_AVPLAY_Destroy(hAvplay);

AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

TSBUF_DESTROY:
    HI_UNF_DMX_DestroyTSBuffer(g_hIpTsBuf);

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

    return 0;
}


