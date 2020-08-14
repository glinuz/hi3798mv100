/******************************************************************************

  Copyright (C), 2001-2013, Hisilicon Tech. Co., Ltd.

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <string.h>
#include <net/if.h>

#include "hi_unf_common.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_vi.h"
#include "hi_unf_venc.h"
#include "hi_unf_vp.h"
#include "hi_unf_hdmi.h"
#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_mpi.h"
#include "hi_adp_hdmi_rx.h"

#define DFLT_SCOPE_ID  "eth0"  /* Default scope identifier.  */
#define DFLT_RTP_PORT  16384  /* Default RTP port.  */ 

static pthread_t g_threadSampleStatics;
static HI_U32 g_stopGet = 1;

typedef enum tagRTP_PAYLOAD_TYPE_E
{
    RTP_PT_ULAW = 0,           /* mu-law */
    RTP_PT_GSM  = 3,           /* GSM */
    RTP_PT_G723 = 4,           /* G.723 */
    RTP_PT_ALAW = 8,           /* a-law */
    RTP_PT_G722 = 9,           /* G.722 */
    RTP_PT_S16BE_STEREO = 10,  /* linear 16, 44.1khz, 2 channel */
    RTP_PT_S16BE_MONO = 11,    /* linear 16, 44.1khz, 1 channel */
    RTP_PT_MPEGAUDIO = 14,     /* mpeg audio */
    RTP_PT_JPEG = 26,          /* jpeg */
    RTP_PT_H261 = 31,          /* h.261 */
    RTP_PT_MPEGVIDEO = 32,     /* mpeg video */
    RTP_PT_MPEG2TS = 33,       /* mpeg2 TS stream */
    RTP_PT_H263  = 34,         /* old H263 encapsulation */
    RTP_PT_H264  = 96,         /* hisilicon define as h.264 */
    RTP_PT_G726  = 97,         /* hisilicon define as G.726 */
    RTP_PT_ADPCM = 98,         /* hisilicon define as ADPCM */
    RTP_PT_AAC  = 99,          /* hisilicon define as AAC */
    RTP_PT_DATA = 100,         /* hisilicon define as md alarm data */
    RTP_PT_ARM     = 101,      /* hisilicon define as AMR */
    RTP_PT_MJPEG   = 102,      /* hisilicon define as MJPEG */
    RTP_PT_H265  = 103,
    RTP_PT_INVALID = 127
} RTP_PAYLOAD_TYPE_E;
static HI_S32 Checkipaddr(HI_BOOL bIpv4, HI_CHAR *ipaddr)
{
    struct sockaddr_in  stIpv4Attr;
    struct sockaddr_in6 stIpv6Attr;

    if (bIpv4)
    {
        return inet_pton(AF_INET, ipaddr, (void *)&stIpv4Attr.sin_addr); 
    }
    else
    {
        return inet_pton(AF_INET6, ipaddr, (void *)&stIpv6Attr.sin6_addr); 
    }
}

HI_VOID* Vp_ScheduleNetStatics(HI_VOID* args)
{
    HI_S32 s32Ret;
    HI_UNF_VP_NET_STA_S stNetStatics;
    HI_HANDLE *hVp = (HI_HANDLE *)args;

    while (!g_stopGet)
    {
        usleep(5000000);
        if (g_stopGet)
        {
            break;
        }

        s32Ret = HI_UNF_VP_GetNetStatics(*hVp, &stNetStatics);
        if (HI_SUCCESS == s32Ret)
        {
            printf("==================Get Vp Net Statics:===================\n");
            printf("recvbytes:        %u\n", stNetStatics.u32RecvBytes);
            printf("recvpacket:         %u\n", stNetStatics.u32RecvPacketNum);
            printf("sendbytes:        %u\n", stNetStatics.u32SendBytes);
            printf("sendpacket:         %u\n", stNetStatics.u32SendPacketNum);

            printf("LostRate(Recent 5s):        %u%%\n", stNetStatics.stRecvInfo.u32LossFraction);
            printf("Total Lost Packets:         %u\n", stNetStatics.stRecvInfo.u32TotalLostPacket);
            printf("Jitter:                     %u(ms)\n", stNetStatics.stRecvInfo.u32Jitter);
            printf("Delay(total sides):         %u(ms)\n", stNetStatics.stRecvInfo.u32Delay);

            printf("LostRate(Recent 5s):        %u%%\n", stNetStatics.stSendInfo.u32LossFraction);
            printf("Total Lost Packets:         %u\n", stNetStatics.stSendInfo.u32TotalLostPacket);
            printf("Jitter:                     %u(ms)\n", stNetStatics.stSendInfo.u32Jitter);
            printf("Delay(total sides):         %u(ms)\n", stNetStatics.stSendInfo.u32Delay);
            printf("=========================End============================\n");
        }
    }

    return NULL;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_HANDLE hVP;
    HI_HANDLE hVI;
    HI_HANDLE hLocalWin;
    HI_HANDLE hRemoteWin;
    HI_RECT_S stRect = {0};
    HI_UNF_VI_ATTR_S stViAttr;
    HI_UNF_VP_ATTR_S stVPAttr;
    HI_UNF_VP_SOURCE_ATTR_S stSrcAttr = {0};
    HI_U32  u32af_inet;
    HI_UNF_ENC_FMT_E voFormat = HI_UNF_ENC_FMT_720P_50;
    HI_UNF_VCODEC_TYPE_E vdecType = HI_UNF_VCODEC_TYPE_H264;
    HI_CHAR *localIpAddr;
    HI_CHAR *remoteIpAddr;

    if (argc >= 5)
    {
        voFormat = HIADP_Disp_StrToFmt(argv[4]);
    }
    else if (argc == 4)
    {
        voFormat = HI_UNF_ENC_FMT_720P_50;
    }
    else
    {
        printf("Usage: sample_vp_vi af_inet vtype localIP remoteIP [vo_format]\n"
               "       af_inet: 1-ipv4|0-ipv6\n"
               "       vtype: h264/h265\n"
               "       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|1080i_50|720P_60|[720P_50]\n"
               "Example:./sample_vp_vi 1 h265 192.168.1.2 192.168.1.3 720p_50\n"
               "Example:./sample_vp_vi 0 h265 fe80::228:1ff:fe19:777 fe80::228:2ff:fe19:777 720p_50\n");
        return 0;
    }

    u32af_inet = strtol(argv[1], NULL, 0);
    if ((0 != u32af_inet) && (1 != u32af_inet))
    {
        printf("wrong af inet!\n");
        return HI_FAILURE;
    }
    if (!strcasecmp("h264", argv[2]))
    {
        vdecType = HI_UNF_VCODEC_TYPE_H264;
    }
    else if (!strcasecmp("h265", argv[2]))
    {
        vdecType = HI_UNF_VCODEC_TYPE_HEVC;
    }
    localIpAddr  = argv[3];
    remoteIpAddr = argv[4];
    if ((1 != Checkipaddr(u32af_inet, localIpAddr))
        || (1 != Checkipaddr(u32af_inet, remoteIpAddr))
        )

    {
        printf("wrong ip format!\n");
        return HI_FAILURE;
    }

    if (0 == memcmp(localIpAddr, remoteIpAddr, strlen(localIpAddr)))
    {
        system("ifconfig lo up");
    }

    HI_SYS_Init();

    HIADP_MCE_Exit();

    s32Ret = HIADP_Disp_Init(voFormat);
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HIADP_Disp_DeInit failed.\n");
        goto SYS_DEINIT;
    }

    s32Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HIADP_VO_Init failed.\n");
        goto DISP_DEINIT;
    }
    HIAPI_RUN(HI_UNF_VI_Init(), s32Ret);
    memset(&stViAttr, 0, sizeof(HI_UNF_VI_ATTR_S));
    HIAPI_RUN(HI_UNF_VI_GetDefaultAttr(&stViAttr), s32Ret);
    
    s32Ret = HI_ADP_HDMIRX_Init_GetViAttr(&stViAttr);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_ADP_HDMIRX_Init failed\n");
        goto VI_DEINIT;
    }

    HIAPI_RUN(HI_UNF_AVPLAY_Init(), s32Ret);
    HIAPI_RUN(HI_UNF_VENC_Init(), s32Ret);
    HIAPI_RUN(HI_UNF_VP_Init(), s32Ret);

    stRect.s32X = 0;
    stRect.s32Y = 100;
    stRect.s32Width  = 640;
    stRect.s32Height = 480;
    s32Ret = HIADP_VO_CreatWin(&stRect, &hLocalWin);
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HIADP_VO_CreatWin failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    stRect.s32X = 640;
    stRect.s32Y = 100;
    stRect.s32Width  = 640;
    stRect.s32Height = 480;
    s32Ret |= HIADP_VO_CreatWin(&stRect, &hRemoteWin);
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }



    HIAPI_RUN(HI_UNF_VI_Create(HI_UNF_VI_PORT0, &stViAttr, &hVI), s32Ret);
    HIAPI_RUN(HI_UNF_VI_Start(hVI), s32Ret);

    memset(&stVPAttr, 0, sizeof(HI_UNF_VP_ATTR_S));
    HIAPI_RUN(HI_UNF_VP_GetDefaultAttr(&stVPAttr), s32Ret);
    if (vdecType == HI_UNF_VCODEC_TYPE_H264)
    {
        stVPAttr.stRtpAttr.u32PayLoadType = RTP_PT_H264;
    }
    else if (vdecType == HI_UNF_VCODEC_TYPE_HEVC)
    {
        stVPAttr.stRtpAttr.u32PayLoadType = RTP_PT_H265;
    }
    stVPAttr.stVencAttr.enVencType = vdecType;
    stVPAttr.stVencAttr.u32Width  = stViAttr.stInputRect.s32Width / 4 * 4;
    stVPAttr.stVencAttr.u32Height = stViAttr.stInputRect.s32Height / 4 * 4;
    stVPAttr.stVencAttr.u32InputFrmRate  = 30;
    stVPAttr.stVencAttr.u32TargetFrmRate = 30;
    stVPAttr.stVencAttr.u32Gop = 300;
    if (stVPAttr.stVencAttr.u32Width > 1280)
    {
        stVPAttr.stVencAttr.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
        stVPAttr.stVencAttr.u32StrmBufSize   = 1920 * 1080 * 2;
        stVPAttr.stVencAttr.u32TargetBitRate = 5 * 1024 * 1024;
    }
    else if (stVPAttr.stVencAttr.u32Width > 720)
    {
        stVPAttr.stVencAttr.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_720P;
        stVPAttr.stVencAttr.u32StrmBufSize   = 1280 * 720 * 2;
        stVPAttr.stVencAttr.u32TargetBitRate = 3 * 1024 * 1024;
    }
    else if (stVPAttr.stVencAttr.u32Width > 352)
    {
        stVPAttr.stVencAttr.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_D1;
        stVPAttr.stVencAttr.u32StrmBufSize   = 720 * 576 * 2;
        stVPAttr.stVencAttr.u32TargetBitRate = 3 / 2 * 1024 * 1024;
    }
    else
    {
        stVPAttr.stVencAttr.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_CIF;
        stVPAttr.stVencAttr.u32StrmBufSize   = 352 * 288 * 2;
        stVPAttr.stVencAttr.u32TargetBitRate = 800 * 1024;
    }

    stVPAttr.enDecType = vdecType;   
    stVPAttr.bIsIpv4 = u32af_inet;
    if (stVPAttr.bIsIpv4)
    {
        if (0 == memcmp(localIpAddr, remoteIpAddr, strlen(localIpAddr)))
        {
            localIpAddr = "127.0.0.1";
            remoteIpAddr = "127.0.0.1";
        }
        stVPAttr.stLocalNetAttr.stIpv4Attr.sin_family = AF_INET;
        stVPAttr.stLocalNetAttr.stIpv4Attr.sin_addr.s_addr = inet_addr(localIpAddr);
        stVPAttr.stLocalNetAttr.stIpv4Attr.sin_port = htons(DFLT_RTP_PORT);
        stVPAttr.stRemoteNetAttr.stIpv4Attr.sin_family = AF_INET;
        stVPAttr.stRemoteNetAttr.stIpv4Attr.sin_addr.s_addr = inet_addr(remoteIpAddr);
        stVPAttr.stRemoteNetAttr.stIpv4Attr.sin_port = htons(DFLT_RTP_PORT);
    }
    else
    {
        if (0 == memcmp(localIpAddr, remoteIpAddr, strlen(localIpAddr)))
        {
            localIpAddr = "::1";
            remoteIpAddr = "::1";
        }
        else
        {
            stVPAttr.stLocalNetAttr.stIpv6Attr.sin6_scope_id = if_nametoindex(DFLT_SCOPE_ID);
        }

        stVPAttr.stLocalNetAttr.stIpv6Attr.sin6_family = AF_INET6;
        inet_pton(AF_INET6, localIpAddr, (void *)&stVPAttr.stLocalNetAttr.stIpv6Attr.sin6_addr);
        stVPAttr.stLocalNetAttr.stIpv6Attr.sin6_port = htons(DFLT_RTP_PORT);
        stVPAttr.stRemoteNetAttr.stIpv6Attr.sin6_family = AF_INET6;
        inet_pton(AF_INET6, remoteIpAddr, (void *)&stVPAttr.stRemoteNetAttr.stIpv6Attr.sin6_addr); 
        stVPAttr.stRemoteNetAttr.stIpv6Attr.sin6_port = htons(DFLT_RTP_PORT);
    }

    HIAPI_RUN(HI_UNF_VP_Create(&stVPAttr, &hVP), s32Ret);

    stSrcAttr.enSrcMode = HI_UNF_VP_SRC_MODE_VI;
    stSrcAttr.hSrc = hVI;
    stSrcAttr.u32VIBufNum = 6;
    HIAPI_RUN(HI_UNF_VP_AttachSource(hVP, &stSrcAttr), s32Ret);

    HIAPI_RUN(HI_UNF_VP_AttachLocalWin(hVP, hLocalWin), s32Ret);
    HIAPI_RUN(HI_UNF_VP_AttachRemoteWin(hVP, hRemoteWin), s32Ret);

    HIAPI_RUN(HI_UNF_VP_Start(hVP), s32Ret);

    g_stopGet = 0;
    pthread_create(&g_threadSampleStatics, HI_NULL, Vp_ScheduleNetStatics, (void*)(&hVP));

    printf("\npress any key to quit!\n");
    getchar();
    g_stopGet = 1;
    pthread_join(g_threadSampleStatics, NULL);

    HIAPI_RUN(HI_UNF_VP_Stop(hVP), s32Ret);
    HIAPI_RUN(HI_UNF_VP_DetachRemoteWin(hVP), s32Ret);
    HIAPI_RUN(HI_UNF_VP_DetachLocalWin(hVP), s32Ret);
    HIAPI_RUN(HI_UNF_VP_DetachSource(hVP), s32Ret);

    HIAPI_RUN(HI_UNF_VO_DestroyWindow(hLocalWin), s32Ret);
    HIAPI_RUN(HI_UNF_VO_DestroyWindow(hRemoteWin), s32Ret);

    HIAPI_RUN(HI_UNF_VP_Destroy(hVP), s32Ret);

    HIAPI_RUN(HI_UNF_VP_DeInit(), s32Ret);
    HIAPI_RUN(HI_UNF_VENC_DeInit(), s32Ret);
    HIAPI_RUN(HI_UNF_VI_Destroy(hVI), s32Ret);
    
    HIAPI_RUN(HI_UNF_AVPLAY_DeInit(), s32Ret);
    
    HI_ADP_HDMIRX_DeInit();
VI_DEINIT:
    HIAPI_RUN(HI_UNF_VI_DeInit(), s32Ret);
    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();

    return s32Ret;
}
