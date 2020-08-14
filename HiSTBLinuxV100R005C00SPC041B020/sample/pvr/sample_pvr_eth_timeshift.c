/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_pvr_eth_timeshift.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/26
  Description   :
  History       :
  1.Date        : 2010/01/26
    Author      : q46153
    Modification: Created file

******************************************************************************/
#include <stdlib.h>

#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "hi_adp_pvr.h"

#include "hi_go_comm.h"
#include "hi_go_gdev.h"
#include "hi_go_bliter.h"
#include "hi_go_text.h"

static pthread_t   g_StatusThread;
static HI_CHAR *g_pMultiAddr;
static HI_U16   g_UdpPort;
static HI_UNF_ENC_FMT_E   g_enDefaultFmt = HI_UNF_ENC_FMT_1080P_60;
PMT_COMPACT_TBL   *g_pProgTbl = HI_NULL;
HI_HANDLE hLayer_pvr = HI_INVALID_HANDLE;
HI_HANDLE hLayerSurface_pvr;
HI_HANDLE hFont_pvr = HI_INVALID_HANDLE;
HI_BOOL g_bIsPlayStop = HI_FALSE;
HI_U32 g_RecChn;
HI_U32 g_PlayChn = 0xffffffff;
static HI_BOOL     g_StopSocketThread = HI_FALSE;
static HI_HANDLE   g_hIpTsBuf;
static pthread_t   g_SocketThd;



#define MAX_TIMESHIFT_REC_FILE_SIZE       (2000*1024*1024LLU)
#define MODULE_MEM_TEST_INFO  _IOWR(0, 8, int)


static HI_VOID SocketThread(HI_VOID *args)
{
    HI_S32              SocketFd;
    struct sockaddr_in  ServerAddr;
    in_addr_t           IpAddr;
    struct ip_mreq      Mreq;
    HI_U32              AddrLen;

    HI_UNF_STREAM_BUF_S     StreamBuf;
    HI_U32              ReadLen;
    HI_U32              GetBufCount=0;
    HI_U32              ReceiveCount=0;
    HI_S32              Ret;

    SocketFd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (SocketFd < 0)
    {
        PVR_SAMPLE_Printf("create socket error [%d].\n", errno);
        return;
    }

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ServerAddr.sin_port = htons(g_UdpPort);

    if (bind(SocketFd,(struct sockaddr *)(&ServerAddr),sizeof(struct sockaddr_in)) < 0)
    {
        PVR_SAMPLE_Printf("socket bind error [%d].\n", errno);
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
            PVR_SAMPLE_Printf("Socket setsockopt ADD_MEMBERSHIP error [%d].\n", errno);
            close(SocketFd);
            return;
        }
    }

    AddrLen = sizeof(ServerAddr);

    while (!g_StopSocketThread)
    {
        Ret = HI_UNF_DMX_GetTSBuffer(g_hIpTsBuf, 188*50, &StreamBuf, 0);
        if (Ret != HI_SUCCESS)
        {
            GetBufCount++;
            if(GetBufCount >= 10)
            {
                PVR_SAMPLE_Printf("########## TS come too fast! #########, Ret=%d\n", Ret);
                GetBufCount=0;
            }

            usleep(10000) ;
            continue;
        }
        GetBufCount=0;

        ReadLen = recvfrom(SocketFd, StreamBuf.pu8Data, StreamBuf.u32Size, 0,
                           (struct sockaddr *)&ServerAddr, &AddrLen);
        if (ReadLen <= 0)
        {
            ReceiveCount++;
            if (ReceiveCount >= 50)
            {
                PVR_SAMPLE_Printf("########## TS come too slow or net error! #########\n");
                ReceiveCount = 0;
            }
        }
        else
        {
            ReceiveCount = 0;
            Ret = HI_UNF_DMX_PutTSBuffer(g_hIpTsBuf, ReadLen);
            if (Ret != HI_SUCCESS )
            {
                PVR_SAMPLE_Printf("call HI_UNF_DMX_PutTSBuffer failed.\n");
            }
        }
    }

    close(SocketFd);
    return;
}

HI_S32 SwitchToShiftPlay(const HI_CHAR *pszFileName, HI_U32 *pu32PlayChn,
             HI_HANDLE hAvplay)
{

    PVR_SwitchDmxSource(PVR_DMX_ID_LIVE, PVR_DMX_PORT_ID_PLAYBACK);
    PVR_StartPlayBack(pszFileName, pu32PlayChn, hAvplay);

    return HI_SUCCESS;
}


HI_S32 SwitchToLivePlay(HI_U32 u32PlayChn, HI_HANDLE hAvplay, const PMT_COMPACT_PROG *pProgInfo)
{
    PVR_StopPlayBack(u32PlayChn);


    PVR_SwitchDmxSource(PVR_DMX_ID_LIVE, HI_UNF_DMX_PORT_RAM_0);
    PVR_StartLivePlay(hAvplay, pProgInfo);
    return HI_SUCCESS;
}

HI_S32 OsdInit(HI_CHAR *pu8CurrentPath)
{
#ifndef ANDROID
    HI_S32   s32Ret;
    HIGO_LAYER_INFO_S stLayerInfo = {0};
    HIGO_TEXT_INFO_S stTextInfo;

    s32Ret = HI_GO_Init();
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to init higo! ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }

    HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);
    stLayerInfo.PixelFormat  = HIGO_PF_1555;
    stLayerInfo.ScreenWidth  = 1920;
    stLayerInfo.ScreenHeight = 1080;
    stLayerInfo.CanvasWidth = 1920;
    stLayerInfo.CanvasHeight = 1080;
    stLayerInfo.DisplayWidth  = 1280;
    stLayerInfo.DisplayHeight = 720;

    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer_pvr);
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to create the layer sd 0, ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret =  HI_GO_GetLayerSurface(hLayer_pvr, &hLayerSurface_pvr);
    if  (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to get layer surface! s32Ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    HI_GO_FillRect(hLayerSurface_pvr, NULL, 0x00000000, HIGO_COMPOPT_NONE);

    stTextInfo.pMbcFontFile = NULL;
    pu8CurrentPath = strncat(pu8CurrentPath, "../res/font/DroidSansFallbackLegacy.ttf", sizeof("../res/font/DroidSansFallbackLegacy.ttf"));
    stTextInfo.pSbcFontFile = pu8CurrentPath;
    stTextInfo.u32Size = 20;

    s32Ret = HI_GO_CreateTextEx(&stTextInfo,  &hFont_pvr);
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to create the font:.%s!\n",pu8CurrentPath);
        return HI_FAILURE;
    }
    HI_GO_RefreshLayer(hLayer_pvr, NULL);
#endif

    return HI_SUCCESS;
}


HI_S32 DrawString(HI_U32 u32TimeType, HI_CHAR *szText)
{
#ifndef ANDROID
    HI_S32   s32Ret;
    HI_RECT rc_Str[4] = {{100, 200, 150, 30},
                         {100, 230, 180, 30},
                         {100, 260, 180, 30},
                         {100, 290, 180, 30}};

    if (3 < u32TimeType)
    {
        PVR_SAMPLE_Printf("Out of time type!\n");
        return HI_FAILURE;
    }

    HI_GO_FillRect(hLayerSurface_pvr, &rc_Str[u32TimeType], 0x80000000, HIGO_COMPOPT_NONE);
    HI_GO_DrawRect(hLayerSurface_pvr, &rc_Str[u32TimeType], 0xffffffff);

    HI_GO_SetTextColor(hFont_pvr, 0xff00ff00);

    s32Ret = HI_GO_TextOutEx(hFont_pvr, hLayerSurface_pvr, szText, &rc_Str[u32TimeType], HIGO_LAYOUT_RIGHT);
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to text out char!\n");
        return HI_FAILURE;
    }

    HI_GO_RefreshLayer(hLayer_pvr, NULL);
#else
    PVR_SAMPLE_Printf("%s\n", szText);
#endif
    return HI_SUCCESS;
}

HI_VOID * StatuThread(HI_VOID *args)
{
    HI_U32 u32RecChn = *(HI_U32 *)args;
    HI_S32 Ret;
    HI_UNF_PVR_REC_STATUS_S stRecstStatus;
    HI_UNF_PVR_PLAY_STATUS_S stPlaystStatus;
    HI_UNF_PVR_FILE_ATTR_S stPlayFileStatus;
    HI_CHAR Rectimestr[20];
    HI_CHAR Playtimestr[20];
    HI_CHAR Filetimestr[20];
    HI_CHAR StatString[32];
    
    while (HI_FALSE == g_bIsPlayStop)
    {
        sleep(1);
#ifdef ANDROID
        PVR_SAMPLE_Printf("----------------------------------\n");
#endif
        Ret = HI_UNF_PVR_RecGetStatus(u32RecChn, &stRecstStatus);
        if (HI_SUCCESS == Ret)
        {
            snprintf(Rectimestr, sizeof(Rectimestr),"Rec time:%d", stRecstStatus.u32CurTimeInMs/1000);
            DrawString(1, Rectimestr);
        }

        if (0xffffffff != g_PlayChn)
        {
            Ret = HI_UNF_PVR_PlayGetFileAttr(g_PlayChn, &stPlayFileStatus);
            if (HI_SUCCESS == Ret)
            {
                snprintf(Filetimestr, sizeof(Filetimestr),"File end time:%d", stPlayFileStatus.u32EndTimeInMs/1000);
                DrawString(2, Filetimestr);
            }

            Ret = HI_UNF_PVR_PlayGetStatus(g_PlayChn, &stPlaystStatus);
            if (HI_SUCCESS == Ret)
            {
                snprintf(Playtimestr, sizeof(Playtimestr),"Play time:%d", stPlaystStatus.u32CurPlayTimeInMs/1000);
                DrawString(3, Playtimestr);
                switch(stPlaystStatus.enState)
                {
                    memset(StatString, 0x00, 32*sizeof(HI_CHAR));
                    case HI_UNF_PVR_PLAY_STATE_INIT:
                        snprintf(StatString, sizeof(StatString),"Init");
                        break;
                    case HI_UNF_PVR_PLAY_STATE_PLAY:
                        snprintf(StatString, sizeof(StatString),"Play");
                        break;
                    case HI_UNF_PVR_PLAY_STATE_PAUSE:
                        snprintf(StatString, sizeof(StatString),"Pause");
                        break;
                    case HI_UNF_PVR_PLAY_STATE_FF:
                        snprintf(StatString, sizeof(StatString),"FF %dX", stPlaystStatus.enSpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL);
                        break;
                    case HI_UNF_PVR_PLAY_STATE_FB:
                        snprintf(StatString, sizeof(StatString),"FB %dX", stPlaystStatus.enSpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL);
                        break;
                    case HI_UNF_PVR_PLAY_STATE_SF:
                        snprintf(StatString, sizeof(StatString),"SF 1/%dX", HI_UNF_PVR_PLAY_SPEED_NORMAL/stPlaystStatus.enSpeed);
                        break;
                    case HI_UNF_PVR_PLAY_STATE_SB:
                        snprintf(StatString, sizeof(StatString),"SB 1/%dX", HI_UNF_PVR_PLAY_SPEED_NORMAL/stPlaystStatus.enSpeed);
                        break;
                    case HI_UNF_PVR_PLAY_STATE_STEPF:
                        snprintf(StatString, sizeof(StatString),"Step F");
                        break;
                    case HI_UNF_PVR_PLAY_STATE_STEPB:
                        snprintf(StatString, sizeof(StatString),"Step B");
                        break;
                    case HI_UNF_PVR_PLAY_STATE_STOP:
                        snprintf(StatString, sizeof(StatString),"Stop");
                        break;
                    default:
                        snprintf(StatString, sizeof(StatString),"InValid(%d)", stPlaystStatus.enState);
                        break;
                }
                DrawString(0, StatString);
            }
        }
#ifdef ANDROID
        PVR_SAMPLE_Printf("----------------------------------\n\n");
#endif
    }

    return NULL;
}

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_U32                  i;
    HI_U32                  u32ProgCount = 0;
    HI_U32                  aProgNum[8] = {0};        //one demux can support  creating 8 record channel
    HI_U32                  aRecChn[8];
    HI_S32                  Ret;
    HI_CHAR                 InputCmd[32];
    HI_BOOL                 bLive = HI_TRUE;
    HI_BOOL                 bPause = HI_FALSE;
    HI_HANDLE               hAvplay;
    HI_HANDLE               hSoundTrack;
    HI_HANDLE               hWin;
    HI_UNF_PVR_REC_ATTR_S   stRecAttr;
    PMT_COMPACT_PROG        *pstCurrentProgInfo;
    HI_CHAR                   aCurrentPath[256] = {0};

    HI_ERR_PRINT(HI_ID_PVR, "%s... enter and argc is %d\n", __func__, argc);

    if(5 == argc)
    {
        g_pMultiAddr  = argv[2];
        g_UdpPort = strtol(argv[3],NULL,0);
        g_enDefaultFmt = HIADP_Disp_StrToFmt(argv[4]);
    }
    else if(4 == argc)
    {
        g_pMultiAddr  = argv[2];
        g_UdpPort = strtol(argv[3],NULL,0);
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
    }
    else
    {
        printf("Usage: %s file_path multiaddr portnum [vo_format]\n"
                "       multicast address: \n"
                "           UDP port \n"
                "       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50\n", argv[0]);
        return HI_FAILURE;
    }

    HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = HI_UNF_DMX_Init();
    Ret |= HI_UNF_DMX_AttachTSPort(PVR_DMX_ID_LIVE,PVR_DMX_PORT_ID_IP);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call VoInit failed.\n");
        return HI_FAILURE;
    }

    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_0, 0x200000, &g_hIpTsBuf);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_DMX_CreateTSBuffer failed.\n");
        return HI_FAILURE;
    }

    Ret = HI_UNF_DMX_AttachTSPort(PVR_DMX_ID_REC, PVR_DMX_PORT_ID_IP);

    g_StopSocketThread = HI_FALSE;
    pthread_create(&g_SocketThd, HI_NULL, (HI_VOID *)SocketThread, HI_NULL);

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(PVR_DMX_ID_LIVE,&g_pProgTbl);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Search_GetAllPmt failed\n");
        return Ret;
    }

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Snd_Init failed.\n");
        return Ret;
    }

    Ret = HIADP_Disp_Init(g_enDefaultFmt);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Disp_DeInit failed.\n");
        return Ret;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,&hWin);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        return Ret;
    }

    Ret = PVR_AvplayInit(hWin, &hAvplay, &hSoundTrack);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call VoInit failed.\n");
        HIADP_VO_DeInit();
        return Ret;
    }

    PVR_SAMPLE_Printf("================================\n");

    Ret = HI_UNF_PVR_RecInit();
    Ret |= HI_UNF_PVR_PlayInit();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_PVR_RecInit failed.\n");
        return Ret;
    }

    Ret = PVR_RegisterCallBacks();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call PVR_RegisterCallBacks failed.\n");
        return Ret;
    }

    PVR_SAMPLE_Printf("please input the number of program to record, the first program num can timeshift:\n");

    do
    {
        sleep(1);
        SAMPLE_GET_INPUTCMD(InputCmd);
        HI_U32 u32Strlen = strlen(InputCmd);
        HI_CHAR *pTmp = InputCmd;
        for (i = 0; i < u32Strlen; i++)
        {
            if (*pTmp >= '0' && *pTmp <= '7')
            {
                aProgNum[u32ProgCount] = atoi(pTmp);
                u32ProgCount++;
            }
            else if (*pTmp == 'q')
            {
                return 0;
            }
            pTmp++;
        }

        if (u32ProgCount == 0)
        {
            PVR_SAMPLE_Printf("input invalid! pleasd input again\n");
            continue;
        }
    }while(0);

    for (i = 0; i < u32ProgCount; i++)
    {
        pstCurrentProgInfo = g_pProgTbl->proginfo + (aProgNum[i]%g_pProgTbl->prog_num);
        PVR_RecStart(argv[1], pstCurrentProgInfo, PVR_DMX_ID_REC, 1,0, MAX_TIMESHIFT_REC_FILE_SIZE, &aRecChn[i]);
    }

    pthread_create(&g_StatusThread, HI_NULL, StatuThread, (HI_VOID *)&aRecChn[0]);

    bLive = HI_TRUE;
    bPause = HI_FALSE;
    pstCurrentProgInfo = g_pProgTbl->proginfo + aProgNum[0]%g_pProgTbl->prog_num;
    Ret = PVR_StartLivePlay(hAvplay, pstCurrentProgInfo);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call SwitchProg failed.\n");
        return Ret;
    }

    memset(aCurrentPath, 0, sizeof(aCurrentPath));
    memcpy(aCurrentPath, argv[0], (HI_U32)(strstr(argv[0], "sample_pvr_eth_timeshift") - argv[0]));
    OsdInit(aCurrentPath);
    DrawString(0, "Live");

    while(1)
    {
        static HI_U32 u32RTimes = 0;
        static HI_U32 u32FTimes = 0;
        static HI_U32 u32STimes = 0;

        PVR_SAMPLE_Printf("please input the q to quit!\n");

        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            g_bIsPlayStop = HI_TRUE;
            g_StopSocketThread = HI_TRUE;
            PVR_SAMPLE_Printf("prepare to exit!\n");
            break;
        }
        else if ('l' == InputCmd[0])
        {
            u32RTimes = 0;
            u32FTimes = 0;
            u32STimes = 0;
            if (!bLive)
            {
                DrawString(0, "Live play");
                SwitchToLivePlay(g_PlayChn, hAvplay, pstCurrentProgInfo);
                bLive = HI_TRUE;
                g_PlayChn = 0xffffffff;
                DrawString(0, "Live");
            }
            else
            {
                PVR_SAMPLE_Printf("already live play...\n");
            }
        }
        else if ('p' == InputCmd[0])
        {
            u32RTimes = 0;
            u32FTimes = 0;
            u32STimes = 0;
            if (bLive)
            {
                PVR_SAMPLE_Printf("now live play, switch timeshift and pause\n");
                HI_UNF_PVR_RecGetChn(aRecChn[0],&stRecAttr);
                PVR_SAMPLE_Printf("switch to timeshift:%s\n", stRecAttr.szFileName);
                PVR_StopLivePlay(hAvplay);
                SwitchToShiftPlay(stRecAttr.szFileName, &g_PlayChn, hAvplay);
                bLive = HI_FALSE;
                DrawString(0, "TimeShift");
                Ret = HI_UNF_PVR_PlayPauseChn(g_PlayChn);
                if (Ret != HI_SUCCESS)
                {
                    PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
                    return Ret;
                }
                bPause = HI_TRUE;
            }
            else
            {
                if (bPause)
                {
                    PVR_SAMPLE_Printf("PVR resume now.\n");
                    Ret = HI_UNF_PVR_PlayResumeChn(g_PlayChn);
                    if (Ret != HI_SUCCESS)
                    {
                        PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayPauseChn failed. ret = 0x%08x\n", Ret);
                        return Ret;
                    }
                    bPause = HI_FALSE;
                }
                else
                {
                    PVR_SAMPLE_Printf("PVR pause now.\n");
                    Ret = HI_UNF_PVR_PlayPauseChn(g_PlayChn);
                    if (Ret != HI_SUCCESS)
                    {
                        PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
                        return Ret;
                    }
                    bPause = HI_TRUE;
                }
            }
        }
        else if ('f' == InputCmd[0])
        {
            u32RTimes = 0;
            u32STimes = 0;
            if (bLive)  
            {
                PVR_SAMPLE_Printf("now live play, switch timeshift and ff\n");
                HI_UNF_PVR_RecGetChn(aRecChn[0],&stRecAttr);
                PVR_SAMPLE_Printf("switch to timeshift:%s\n", stRecAttr.szFileName);
                PVR_StopLivePlay(hAvplay);
                SwitchToShiftPlay(stRecAttr.szFileName, &g_PlayChn, hAvplay);
                bLive = HI_FALSE;
                bPause = HI_FALSE;
                DrawString(0, "TimeShift");
            }
            HI_UNF_PVR_PLAY_MODE_S stTrickMode;
            u32FTimes = u32FTimes%6;
            stTrickMode.enSpeed = (0x1 << (u32FTimes+1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;
            PVR_SAMPLE_Printf("trick mod:%d\n", stTrickMode.enSpeed);
            Ret = HI_UNF_PVR_PlayTPlay(g_PlayChn, &stTrickMode);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                return Ret;
            }
            bPause = HI_FALSE;
            u32FTimes++;
        }
        else if ('r' == InputCmd[0])
        {
            u32FTimes = 0;
            u32STimes = 0;
            if (bLive) 
            {
                PVR_SAMPLE_Printf("now live play, switch timeshift and fb\n");
                HI_UNF_PVR_RecGetChn(aRecChn[0],&stRecAttr);
                PVR_SAMPLE_Printf("switch to timeshift:%s\n", stRecAttr.szFileName);
                PVR_StopLivePlay(hAvplay);
                SwitchToShiftPlay(stRecAttr.szFileName, &g_PlayChn, hAvplay);
                bLive = HI_FALSE;
                bPause = HI_FALSE;
                DrawString(0, "TimeShift");
            }
            HI_UNF_PVR_PLAY_MODE_S stTrickMode;
            u32RTimes = u32RTimes%6;
            stTrickMode.enSpeed = -(0x1 << (u32RTimes+1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;
            PVR_SAMPLE_Printf("trick mod:%d\n", stTrickMode.enSpeed);
            Ret = HI_UNF_PVR_PlayTPlay(g_PlayChn, &stTrickMode);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                return Ret;
            }
            bPause = HI_FALSE;
            u32RTimes++;
        }
        else if ('n' == InputCmd[0])
        {
            u32RTimes = 0;
            u32FTimes = 0;
            u32STimes = 0;
            
            if (bLive)
            {
                HI_UNF_PVR_RecGetChn(g_RecChn,&stRecAttr);
                PVR_SAMPLE_Printf("switch to timeshift:%s\n", stRecAttr.szFileName);
                PVR_StopLivePlay(hAvplay);
                SwitchToShiftPlay(stRecAttr.szFileName, &g_PlayChn, hAvplay);
                PVR_SAMPLE_Printf("PlayChn ============= %d\n", g_PlayChn);
                bLive = HI_FALSE;
                DrawString(0, "TimeShift");
                bPause = HI_FALSE;
            }
            else
            {
                PVR_SAMPLE_Printf("resume to normal play now\n");
                Ret = HI_UNF_PVR_PlayResumeChn(g_PlayChn);
                if (Ret != HI_SUCCESS)
                {
                    PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                    return Ret;
                }
                bPause = HI_FALSE;
            }
        }
        else if ('s' == InputCmd[0])
        {
            u32RTimes = 0;
            u32FTimes = 0;
            if (bLive) 
            {
                PVR_SAMPLE_Printf("now live play, switch timeshift and slow play\n");
                HI_UNF_PVR_RecGetChn(aRecChn[0],&stRecAttr);
                PVR_SAMPLE_Printf("switch to timeshift:%s\n", stRecAttr.szFileName);
                PVR_StopLivePlay(hAvplay);
                SwitchToShiftPlay(stRecAttr.szFileName, &g_PlayChn, hAvplay);
                bLive = HI_FALSE;
                bPause = HI_FALSE;
                DrawString(0, "TimeShift");
            }
            HI_UNF_PVR_PLAY_MODE_S stTrickMode;
            u32STimes = u32STimes%6;
            stTrickMode.enSpeed = HI_UNF_PVR_PLAY_SPEED_NORMAL/(0x1 << (u32STimes+1));
            PVR_SAMPLE_Printf("trick mod:%d\n", stTrickMode.enSpeed);
            Ret = HI_UNF_PVR_PlayTPlay(g_PlayChn, &stTrickMode);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                return Ret;
            }
            bPause = HI_FALSE;
            u32STimes++;
        }
        else if ('k' == InputCmd[0])
        {
            HI_UNF_PVR_PLAY_POSITION_S stPos;
            HI_UNF_PVR_FILE_ATTR_S stFAttr;
            u32RTimes = 0;
            u32FTimes = 0;
            u32STimes = 0;
            if (bLive)  
            {
                PVR_SAMPLE_Printf("now live play, not support seek\n");

            }
            else
            {
                Ret = HI_UNF_PVR_PlayResumeChn(g_PlayChn);
                if (Ret != HI_SUCCESS)
                {
                    PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                    continue;
                }
                Ret = HI_UNF_PVR_PlayGetFileAttr(g_PlayChn,&stFAttr);
                if (Ret != HI_SUCCESS)
                {
                    PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayGetFileAttr failed.\n");
                }

                stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                stPos.s64Offset = stFAttr.u32StartTimeInMs;
                stPos.s32Whence = SEEK_SET;
                PVR_SAMPLE_Printf("seek to start\n");

                Ret = HI_UNF_PVR_PlaySeek(g_PlayChn, &stPos);
                if (Ret != HI_SUCCESS)
                {
                    PVR_SAMPLE_Printf("call HI_UNF_PVR_PlaySeek failed.\n");
                }
            }
        }
        else if ('e' == InputCmd[0])
        {
            HI_UNF_PVR_PLAY_POSITION_S stPos;
            u32RTimes = 0;
            u32FTimes = 0;
            u32STimes = 0;
            if (bLive)  
            {
                PVR_SAMPLE_Printf("now live play, not support seek\n");

            }
            else
            {
                Ret = HI_UNF_PVR_PlayResumeChn(g_PlayChn);
                if (Ret != HI_SUCCESS)
                {
                    PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                    continue;
                }
                stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                stPos.s64Offset = 0;
                stPos.s32Whence = SEEK_END;
                PVR_SAMPLE_Printf("seek end\n");

                Ret = HI_UNF_PVR_PlaySeek(g_PlayChn, &stPos);
                if (Ret != HI_SUCCESS)
                {
                    PVR_SAMPLE_Printf("call HI_UNF_PVR_PlaySeek failed.\n");
                }
            }
        }
        else if ('a' == InputCmd[0])
        {
            HI_UNF_PVR_PLAY_POSITION_S stPos;
            u32RTimes = 0;
            u32FTimes = 0;
            u32STimes = 0;
            if (bLive)  
            {
                PVR_SAMPLE_Printf("now live play, not support seek\n");

            }
            else
            {
                Ret = HI_UNF_PVR_PlayResumeChn(g_PlayChn);
                if (Ret != HI_SUCCESS)
                {
                    PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                    continue;
                }
                stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                stPos.s64Offset = -5000;
                stPos.s32Whence = SEEK_CUR;
                PVR_SAMPLE_Printf("seek reward 5 Second\n");

                Ret = HI_UNF_PVR_PlaySeek(g_PlayChn, &stPos);
                if (Ret != HI_SUCCESS)
                {
                    PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayStep failed.\n");
                    continue;
                }
            }
        }
        else if ('d' == InputCmd[0])
        {
            HI_UNF_PVR_PLAY_POSITION_S stPos;
            u32RTimes = 0;
            u32FTimes = 0;
            u32STimes = 0;

            if (bLive)  
            {
                PVR_SAMPLE_Printf("now live play, not support seek\n");

            }
            else
            {
                Ret = HI_UNF_PVR_PlayResumeChn(g_PlayChn);
                if (Ret != HI_SUCCESS)
                {
                    PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                    continue;
                }
                stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                stPos.s64Offset = 5000;
                stPos.s32Whence = SEEK_CUR;
                PVR_SAMPLE_Printf("seek forward 5 Second\n");

                Ret = HI_UNF_PVR_PlaySeek(g_PlayChn, &stPos);
                if (Ret != HI_SUCCESS)
                {
                    PVR_SAMPLE_Printf("call HI_UNF_PVR_PlaySeek failed.\n");
                }
            }
        }
        else if ('x' == InputCmd[0])
        {
            u32RTimes = 0;
            u32FTimes = 0;
            u32STimes = 0;
            if (!bLive)
            {
                PVR_StopPlayBack(g_PlayChn);

                Ret = PVR_StartPlayBack(stRecAttr.szFileName, &g_PlayChn, hAvplay);
                if (Ret != HI_SUCCESS)
                {
                    PVR_SAMPLE_Printf("call PVR_StartPlayBack failed.\n");
                }
            }
            else
            {
                PVR_SAMPLE_Printf("Now in live mode, can't recreate play channel!\n");
            }
        }
        else
        {
            printf("commond:\n");
            printf("    l: live play\n");
            printf("    p: pause/play\n");
            printf("    f: fast foreword\n");
            printf("    r: fast reword\n");
            printf("    n: normal play\n");
            printf("    s: slow play\n");
            printf("    k: seek to start\n");
            printf("    e: seek to end\n");
            printf("    a: seek backward 5s\n");
            printf("    d: seek forward 5s\n");
            printf("    x: destroy play channel and create again\n");
            printf("    h: help\n");
            printf("    q: quit\n");
            continue;
        }
    }

    pthread_join(g_SocketThd, HI_NULL);
    pthread_join(g_StatusThread, HI_NULL);

    for (i = 0; i < u32ProgCount; i++)
    {
        PVR_RecStop(aRecChn[i]);
    }

    if (0xffffffff != g_PlayChn)
    {
        PVR_StopPlayBack(g_PlayChn);
    }
    PVR_AvplayDeInit(hAvplay, hWin, hSoundTrack);

    HI_UNF_VO_DestroyWindow(hWin);
    HIADP_VO_DeInit();
    HIADP_Disp_DeInit();
    HIADP_Snd_DeInit();
    HI_SYS_DeInit();

    return 0;
}


