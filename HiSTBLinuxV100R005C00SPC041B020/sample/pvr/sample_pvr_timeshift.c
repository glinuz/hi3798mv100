/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_pvr_timeshift.c
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

#ifndef ANDROID

//#define DISPLAY_JPEG
#ifdef DISPLAY_JPEG
#include "hi_go_gdev.h"
#include "hi_go_bliter.h"
#include "hi_go_decoder.h"
#endif
#include "hi_go_comm.h"
#include "hi_go_gdev.h"
#include "hi_go_bliter.h"
#include "hi_go_text.h"
#endif

#ifdef ENABLE_IR_PRG
#include "hi_unf_ir.h"
#endif

#define MAX_TIMESHIFT_REC_FILE_SIZE       (2000*1024*1024LLU)


static pthread_t g_TimeThread;
HI_U32 g_PlayChn = 0xffffffff;
HI_U32 g_TunerFreq;
HI_U32 g_TunerSrate;
HI_U32 g_ThirdParam;
HI_BOOL g_bIsPlayStop = HI_FALSE;
static HI_UNF_ENC_FMT_E g_enDefaultFmt = HI_UNF_ENC_FMT_1080P_60;
PMT_COMPACT_TBL   *g_pProgTbl = HI_NULL;

HI_HANDLE hLayer_pvr = HI_INVALID_HANDLE;
HI_HANDLE hLayerSurface_pvr;
HI_HANDLE hFont_pvr = HI_INVALID_HANDLE;


#ifdef ENABLE_IR_PRG
pthread_t g_IrThread = 0;
HI_BOOL g_bIrTaskRunning = HI_FALSE;
HI_HANDLE g_hIrAvplay = 0;

HI_S32 IRProcess(HI_U32 u32RecChn);
#endif

HI_S32 DmxInitAndSearch(HI_U32 u32TunerID)
{
    HI_S32 Ret;

    Ret = HI_UNF_DMX_Init();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_DMX_Init failed.\n");
        return Ret;
    }
    Ret = HIADP_DMX_AttachTSPort(PVR_DMX_ID_LIVE, u32TunerID);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_DMX_AttachTSPort failed.\n");
        HI_UNF_DMX_DeInit();
        return Ret;
    }

    Ret = HIADP_DMX_AttachTSPort(PVR_DMX_ID_REC, u32TunerID);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_DMX_AttachTSPort for REC failed.\n");
        HI_UNF_DMX_DeInit();
        return Ret;
    }

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(PVR_DMX_ID_LIVE, &g_pProgTbl);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Search_GetAllPmt failed\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 DmxDeInit(HI_VOID)
{
    HI_S32 Ret;

    HIADP_Search_FreeAllPmt(g_pProgTbl);

    Ret = HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_LIVE);
    Ret |= HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_REC);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_DMX_DetachTSPort failed.\n");
        return Ret;
    }

    Ret = HI_UNF_DMX_DeInit();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_DMX_DeInit failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
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


    PVR_SwitchDmxSource(PVR_DMX_ID_LIVE, PVR_DMX_PORT_ID_DVB);
    PVR_StartLivePlay(hAvplay, pProgInfo);
    return HI_SUCCESS;
}

#ifdef DISPLAY_JPEG
static HI_S32 file_dec_doublevo(HI_CHAR *pszFileName, HI_HANDLE *pSurface)
{
    HI_S32 ret;
    HI_HANDLE hDecoder;
    HIGO_DEC_ATTR_S stSrcDesc;

    /** create decode*/
    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = pszFileName;
    ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
    if (HI_SUCCESS != ret)
    {
        PVR_SAMPLE_Printf("HI_GO_CreateDecoder failed!\n");
        return ret;
    }

    /** decode it to Surface */
    ret  = HI_GO_DecImgData(hDecoder, 0, NULL, pSurface);
    ret |= HI_GO_DestroyDecoder(hDecoder);

    return ret;
}

HI_S32 BlitPic(HI_HANDLE* phLayerHD, HI_CHAR *pu8CurrentPath)
{
    HI_S32 s32Ret = 0;

    HI_U32 StartX_HD, StartY_HD;
    HI_HANDLE hDecSurface, hLayer_HD, hLayer_Surface_HD;
    HI_CHAR aszFileName[256] = {0};
    HIGO_LAYER_INFO_S stLayerInfo_HD;
    HIGO_BLTOPT_S stBltOpt   = {0};
    HIGO_LAYER_E eLayerID_HD = HIGO_LAYER_HD_0;
    HI_RECT stRect;

    s32Ret  = HI_GO_Init();
    s32Ret |= HI_GO_GetLayerDefaultParam(eLayerID_HD, &stLayerInfo_HD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GO_Deinit();

        PVR_SAMPLE_Printf("HI_GO_GetLayerDefaultParam failed: line:%d\n", __LINE__);

        return HI_FAILURE;
    }

    //stLayerInfo_HD.ScreenWidth  = 720;
    //stLayerInfo_HD.ScreenHeight = 576;
    stLayerInfo_HD.PixelFormat = HIGO_PF_8888;
    stLayerInfo_HD.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;

    /**create the graphic layer and get the handler */
    s32Ret = HI_GO_CreateLayer(&stLayerInfo_HD, &hLayer_HD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GO_Deinit();

        PVR_SAMPLE_Printf("HI_GO_CreateLayer failed: line:%d\n", __LINE__);

        return HI_FAILURE;
    }

    *phLayerHD = hLayer_HD;

    StartX_HD = 0;
    StartY_HD = 0;
    HI_GO_SetLayerPos(hLayer_HD, StartX_HD, StartY_HD);
    s32Ret = HI_GO_GetLayerSurface(hLayer_HD, &hLayer_Surface_HD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_GO_DestroyLayer(hLayer_HD);

        HI_GO_Deinit();

        PVR_SAMPLE_Printf("HI_GO_GetLayerSurface failed: line:%d\n", __LINE__);

        return HI_FAILURE;
    }
    HI_GO_FillRect(hLayer_Surface_HD, NULL, 0xff000000, 0);
    HI_GO_RefreshLayer(hLayer_HD, NULL);
    HI_GO_SetLayerAlpha(hLayer_HD, 200);

    stBltOpt.EnableScale = HI_TRUE;

    snprintf(aszFileName, sizeof(aszFileName), "%s/%s", pu8CurrentPath, "../res/jpg/image1.jpg");

    stRect.x = stRect.y = 0;
    stRect.w = stLayerInfo_HD.DisplayWidth;
    stRect.h = stLayerInfo_HD.DisplayHeight;

    /**decoding*/
    s32Ret = file_dec_doublevo(aszFileName, &hDecSurface);
    if (s32Ret == -1)
    {
        PVR_SAMPLE_Printf("file_dec_doublevo failed, again...\n");
        s32Ret = file_dec_doublevo(aszFileName, &hDecSurface);
    }
    if (HI_SUCCESS == s32Ret)
    {
        /** Blit it to graphic layer Surface */
        s32Ret = HI_GO_Blit(hDecSurface, NULL, hLayer_Surface_HD, &stRect, &stBltOpt);
        PVR_SAMPLE_Printf("HI_GO_Blit result is %d:(0x%08x)\n",s32Ret,s32Ret);
        HI_GO_RefreshLayer(hLayer_HD, NULL);
        HI_GO_FreeSurface(hDecSurface);
    }

    return HI_SUCCESS;
}
#endif


HI_S32 OsdInit(HI_CHAR *pu8CurrentPath)
{
#ifndef ANDROID
    HI_S32 s32Ret;
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
        PVR_SAMPLE_Printf("failed to create the font:%s(ret = 0x%08x) !\n", pu8CurrentPath, s32Ret);
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
    HI_RECT rc_Str[6] = {{100, 200, 150, 30},
                         {100, 230, 180, 30},
                         {100, 260, 180, 30},
                         {100, 290, 180, 30},
                         {100, 320, 180, 30},
                         {100, 350, 180, 30}};

    if (5 < u32TimeType)
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
    HI_UNF_PVR_REC_ATTR_S stRecAttr;
    HI_UNF_PVR_REC_STATUS_S RecstStatus;
    HI_UNF_PVR_PLAY_STATUS_S PlaystStatus;
    HI_UNF_PVR_FILE_ATTR_S FileStatus;
    HI_CHAR Rectimestr[20];
    HI_CHAR Playtimestr[20];
    HI_CHAR Filetimestr[20];
    HI_CHAR StatString[32];
    
    while (HI_FALSE == g_bIsPlayStop)
    {
        sleep(1);
        (void)HI_UNF_PVR_RecGetChn(u32RecChn, &stRecAttr);
#ifdef ANDROID
        PVR_SAMPLE_Printf("----------------------------------\n");
#endif
        Ret = HI_UNF_PVR_RecGetStatus(u32RecChn, &RecstStatus);
        if (HI_SUCCESS == Ret)
        {
            snprintf(Rectimestr, sizeof(Rectimestr),"Rec time:%ds", RecstStatus.u32CurTimeInMs/1000);
            DrawString(1, Rectimestr);
            snprintf(Rectimestr, sizeof(Rectimestr),"Rec len:%ds", (RecstStatus.u32EndTimeInMs-RecstStatus.u32StartTimeInMs)/1000);
            DrawString(2, Rectimestr);
        }

        Ret = HI_UNF_PVR_GetFileAttrByFileName(stRecAttr.szFileName, &FileStatus);
        if (HI_SUCCESS == Ret)
        {
            snprintf(Rectimestr, sizeof(Rectimestr),"Rec size:%lld.%lldM",
                     (FileStatus.u64ValidSizeInByte)/0x100000,
                     ((FileStatus.u64ValidSizeInByte)%0x100000)/0x400);
            DrawString(3, Rectimestr);
        }

        if (0xffffffff != g_PlayChn)
        {
            if (HI_SUCCESS == Ret)
            {
                snprintf(Filetimestr, sizeof(Filetimestr),"File end time:%ds", FileStatus.u32EndTimeInMs/1000);
                DrawString(4, Filetimestr);
            }

            Ret = HI_UNF_PVR_PlayGetStatus(g_PlayChn, &PlaystStatus);
            if (HI_SUCCESS == Ret)
            {
                snprintf(Playtimestr, sizeof(Playtimestr),"Play time:%ds", PlaystStatus.u32CurPlayTimeInMs/1000);
                DrawString(5, Playtimestr);
                switch(PlaystStatus.enState)
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
                        snprintf(StatString, sizeof(StatString),"FF %dX", PlaystStatus.enSpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL);
                        break;
                    case HI_UNF_PVR_PLAY_STATE_FB:
                        snprintf(StatString, sizeof(StatString),"FB %dX", PlaystStatus.enSpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL);
                        break;
                    case HI_UNF_PVR_PLAY_STATE_SF:
                        snprintf(StatString, sizeof(StatString),"SF 1/%dX", HI_UNF_PVR_PLAY_SPEED_NORMAL/PlaystStatus.enSpeed);
                        break;
                    case HI_UNF_PVR_PLAY_STATE_SB:
                        snprintf(StatString, sizeof(StatString),"SB 1/%dX", HI_UNF_PVR_PLAY_SPEED_NORMAL/PlaystStatus.enSpeed);
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
                        snprintf(StatString, sizeof(StatString),"InValid(%d)", PlaystStatus.enState);
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

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_U32                  i;
    HI_U32                  u32Tuner;
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
#ifdef DISPLAY_JPEG
    HI_HANDLE               hLayer_HD;
    HI_U32                  u32Jpeg = 0;
#endif
    HI_BOOL                 bSupportTee = HI_FALSE;

    if (9 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = strtol(argv[3],NULL,0);
        g_ThirdParam = strtol(argv[4],NULL,0);
        g_enDefaultFmt = HIADP_Disp_StrToFmt(argv[5]);
        u32Tuner = strtol(argv[6],NULL,0);	
#ifdef DISPLAY_JPEG
        u32Jpeg = strtol(argv[7],NULL,0);
#endif
        bSupportTee = strtol(argv[8],NULL,0);
    }
    else if (8 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = strtol(argv[3],NULL,0);
        g_ThirdParam = strtol(argv[4],NULL,0);
        g_enDefaultFmt = HIADP_Disp_StrToFmt(argv[5]);
        u32Tuner = strtol(argv[6],NULL,0);	
#ifdef DISPLAY_JPEG
        u32Jpeg = strtol(argv[7],NULL,0);
#endif
    }
    else if (7 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = strtol(argv[3],NULL,0);
        g_ThirdParam = strtol(argv[4],NULL,0);
        g_enDefaultFmt = HIADP_Disp_StrToFmt(argv[5]);
        u32Tuner = strtol(argv[6],NULL,0);
    }
    else if (6 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = strtol(argv[3],NULL,0);
        g_ThirdParam = strtol(argv[4],NULL,0);
        g_enDefaultFmt = HIADP_Disp_StrToFmt(argv[5]);
        u32Tuner = 0;
    }
    else if (5 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = strtol(argv[3],NULL,0);
        g_ThirdParam = strtol(argv[4],NULL,0);
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
        u32Tuner = 0;
    }
    else if(4 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = strtol(argv[3],NULL,0);
        g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
        u32Tuner = 0;
    }
    else if(3 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = (g_TunerFreq>1000) ? 27500 : 6875;
        g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
        u32Tuner = 0;
    }
    else
    {
        printf("Usage: %s file_path freq [srate] [qamtype or polarization] [vo_format] [tuner] [JPEG][bSupportTee]\n"
                "       qamtype or polarization: \n"
                "           For cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64] \n"
                "           For satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0] \n"
                "       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50\n"
                "       u32Tuner: value can be 0, 1, 2, 3\n"
                "       bSupportTee: 1 -- support; 0 -- not support\n", 
                argv[0]);
        printf("Example: %s ./ 618 6875 64 1080i_50 0 0 0\n", argv[0]);
        printf("    Satellite : %s ./ 3840 27500 0 1080i_50 0 0 0\n", argv[0]);
        printf("    Cable     : %s ./ 618 6875 64 1080i_50 1 0 0\n", argv[0]);
        printf("    Terestrial: %s ./ 474 8000 64 1080i_50 2 0 0\n", argv[0]);
        return HI_FAILURE;
    }
    
    HI_SYS_Init();
    if (HI_FALSE != bSupportTee)
    {
        PVR_SetTvpFlag(HI_TRUE);
        PVR_SetCallbackFlag(HI_FALSE, HI_TRUE);
    }
    /*if you want to register callback to PVR for false == bSupportTee, you can call PVR_SetCallbackFlag*/
    HIADP_MCE_Exit();
    Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Demux_Init failed.\n");
        return Ret;
    }

    PVR_SAMPLE_Printf("HIADP_Tuner_Connect, frequency:%d, Symbol:%d, Qam:%d\n", g_TunerFreq,g_TunerSrate,g_ThirdParam);

    Ret = HIADP_Tuner_Connect(u32Tuner, g_TunerFreq, g_TunerSrate, g_ThirdParam);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Tuner_Connect failed. Ret = 0x%x\n", Ret);

        return Ret;
    }

    Ret = DmxInitAndSearch(u32Tuner);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call VoInit failed.\n");
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
    Ret |= HIADP_VO_CreatWin(HI_NULL, &hWin);
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


#ifdef DISPLAY_JPEG
    memset(aCurrentPath, 0, sizeof(aCurrentPath));
    memcpy(aCurrentPath, argv[0], (HI_U32)(strstr(argv[0], "sample_pvr_timeshift") - argv[0]));
    if (u32Jpeg)
    {
        BlitPic(&hLayer_HD, aCurrentPath);
    }
#endif

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

    PVR_SAMPLE_Printf("please input the numbers of program to record, the first program num will timeshift:\n");

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

#ifdef ENABLE_IR_PRG
    g_hIrAvplay = hAvplay;    
    IRProcess(aRecChn[0]);
#endif

    pthread_create(&g_TimeThread, HI_NULL, StatuThread, (HI_VOID *)&aRecChn[0]);

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
    memcpy(aCurrentPath, argv[0], (HI_U32)(strstr(argv[0], "sample_pvr_timeshift") - argv[0]));
    OsdInit(aCurrentPath);
    DrawString(0, "Live");

    while(1)
    {
        static HI_U32 u32RTimes = 0;
        static HI_U32 u32FTimes = 0;
        static HI_U32 u32STimes = 0;

        PVR_SAMPLE_Printf("please input the q to quit!\n");
#ifndef ANDROID
        if (!SAMPLE_GET_INPUTCMD(InputCmd))
        {
            sleep(1);
            continue;
        }
#else
        SAMPLE_GET_INPUTCMD(InputCmd);
#endif
        if ('q' == InputCmd[0])
        {
            g_bIsPlayStop = HI_TRUE;
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
                HI_UNF_PVR_RecGetChn(aRecChn[0],&stRecAttr);
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
            printf("    l: switch to live play\n");
            printf("    p: pause/play\n");
            printf("    f: fast forward\n");
            printf("    r: fast backward\n");
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
#ifdef DISPLAY_JPEG
    if (u32Jpeg)
    {
        HI_GO_DestroyLayer(hLayer_HD);

        HI_GO_Deinit();
    }
#endif

    pthread_join(g_TimeThread, HI_NULL);

    for (i = 0; i < u32ProgCount; i++)
    {
        PVR_RecStop(aRecChn[i]);
    }
    if (0xffffffff != g_PlayChn)
    {
        PVR_StopPlayBack(g_PlayChn);
    }

#ifdef ENABLE_IR_PRG
    g_bIrTaskRunning = HI_FALSE;
    pthread_join(g_IrThread, HI_NULL);

    HI_UNF_IR_DeInit();
#endif

    PVR_AvplayDeInit(hAvplay, hWin, hSoundTrack);

    HI_UNF_VO_DestroyWindow(hWin);
    HIADP_VO_DeInit();
    HIADP_Disp_DeInit();
    HIADP_Snd_DeInit();
    HI_SYS_DeInit();

    return 0;
}


#ifdef ENABLE_IR_PRG
const HI_CHAR IR_Stuts_Str[3][10] =
{
    "DOWN",
    "HOLD",
    "UP",
};

typedef enum tagKEY{
    KEY_FF = 0x29d6ff00,
    KEY_RW = 0x25daff00,
    KEY_STOP = 0x2fd0ff00,
    KEY_PLAY = 0x3cc3ff00,
    KEY_F1 = 0x7b84ff00,
    KEY_F2 = 0x8689ff00,
    KEY_F3 = 0x26d9ff00,
    KEY_F4 = 0x6996ff00,
    KEY_SEEK = 0x7d82ff00
}KEY_PAD;

HI_VOID *IR_ReceiveTask(HI_VOID *args)
{
    HI_U32 u32RecChn = *(HI_U32 *)args;
    HI_U32 u32Rate = 0;
    HI_U32 u32PauseFlag = 0xffffffff;
    HI_U32 u32SeekFlag = 0xffffffff;
    HI_S32 Ret = 0;
    HI_U64 u64KeyId = 0;
    HI_UNF_KEY_STATUS_E enPressStatus = HI_UNF_KEY_STATUS_DOWN;
    HI_UNF_IR_PROTOCOL_E enProtocol = HI_UNF_IR_NEC;

    while (g_bIrTaskRunning)
    {
        /*get ir press codevalue & press status*/
        Ret = HI_UNF_IR_GetValue(&enPressStatus, &u64KeyId, 200);
        if (HI_SUCCESS == Ret)
        {
            Ret = HI_UNF_IR_GetProtocol(&enProtocol);
            if (enPressStatus == 0)
            {
                switch(u64KeyId)
                {
                    case KEY_PLAY:
                    {
                        u32Rate = 0;

                        if(u32PauseFlag == 0xffffffff)
                        {
                            PVR_SAMPLE_Printf("PVR pause now.\n");
                            Ret = HI_UNF_PVR_PlayPauseChn(g_PlayChn);
                            if (Ret != HI_SUCCESS)
                            {
                                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
                            }
                            u32PauseFlag = 0;
                        }
                        else
                        {
                            PVR_SAMPLE_Printf("PVR resume now.\n");
                            Ret = HI_UNF_PVR_PlayResumeChn(g_PlayChn);
                            if (Ret != HI_SUCCESS)
                            {
                                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                            }

                            u32PauseFlag = 0xffffffff;
                        }
                    }
                    break;
                    case KEY_FF:
                    case KEY_RW:
                    {
                        HI_UNF_PVR_PLAY_MODE_S stTrickMode;

                        u32Rate = u32Rate%5;
                        stTrickMode.enSpeed = (0x1 << (u32Rate+1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;

                        if(u64KeyId == KEY_RW)
                        {
                            stTrickMode.enSpeed *= -1;
                        }

                        PVR_SAMPLE_Printf("trick mod:%d\n", stTrickMode.enSpeed);
                        Ret = HI_UNF_PVR_PlayTPlay(g_PlayChn, &stTrickMode);
                        if (Ret != HI_SUCCESS)
                        {
                            PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                        }

                        u32Rate++;
                        u32PauseFlag = 0;
                    }
                    break;
                    case KEY_STOP:
                        PVR_SAMPLE_Printf("====To stop player channel is %d\n", g_PlayChn);
                        PVR_StopPlayBack(g_PlayChn);
                        u32PauseFlag = 0;
                    break;
                    case KEY_F1:
                    case KEY_F2:
                    case KEY_F3:
                    case KEY_F4:
                    {
                        HI_UNF_PVR_REC_ATTR_S stRecAttr;

                        u32PauseFlag = 0xffffffff;

                        if(g_PlayChn != 0xffffffff)
                        {
                            PVR_StopPlayBack(g_PlayChn);
                        }

                        HI_UNF_PVR_RecGetChn(u32RecChn,&stRecAttr);
                        PVR_StartPlayBack(stRecAttr.szFileName, &g_PlayChn, g_hIrAvplay);
                        PVR_SAMPLE_Printf("====Created Player channel is %d\n", g_PlayChn);
                    }
                    break;
                    case KEY_SEEK:
                    {
                        HI_UNF_PVR_PLAY_POSITION_S stPos;
                        u32Rate = 0;

                        u32PauseFlag = 0xffffffff;

                        stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                        stPos.s64Offset = 0;
                        if(u32SeekFlag == 0xffffffff)
                        {
                            stPos.s32Whence = SEEK_SET;
                            u32SeekFlag = 0;
                            PVR_SAMPLE_Printf("\033[5;31mseek to start\n\033[0m");
                        }
                        else
                        {
                            PVR_SAMPLE_Printf("\033[5;31mseek to end\n\033[0m");
                            stPos.s32Whence = SEEK_END;
                            u32SeekFlag = 0xffffffff;
                        }

                        Ret = HI_UNF_PVR_PlaySeek(g_PlayChn, &stPos);
                        if (Ret != HI_SUCCESS)
                        {
                            PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayStep failed.\n");
                        }
                    }
                    break;
                    default:
                        PVR_SAMPLE_Printf(" IR   KeyId : 0x%llx   enPressStatus :%d[%s]\n", u64KeyId, enPressStatus, IR_Stuts_Str[enPressStatus]);
                    break;
                }
            }
        }
        usleep(10*1000);
    }
    return 0;
}

HI_S32  IRProcess(HI_U32 u32RecChn)
{
    HI_S32 Ret;

    /*open ir device*/
    Ret = HI_UNF_IR_Init();
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf(" ErrorCode=0x%x\n", Ret);
        return Ret;
    }

    Ret = HI_UNF_IR_EnableKeyUp(HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf(" ErrorCode=0x%x\n", Ret);
        HI_UNF_IR_DeInit();
        return Ret;
    }

    Ret = HI_UNF_IR_SetRepKeyTimeoutAttr(108);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf(" ErrorCode=0x%x\n", Ret);
        HI_UNF_IR_DeInit();
        return Ret;
    }

    Ret = HI_UNF_IR_EnableRepKey(HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf(" ErrorCode=0x%x\n", Ret);
        HI_UNF_IR_DeInit();
        return Ret;
    }

    Ret = HI_UNF_IR_Enable(HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf(" ErrorCode=0x%x\n", Ret);
        HI_UNF_IR_DeInit();
        return Ret;
    }

    g_bIrTaskRunning = HI_TRUE;

    /*create a thread for ir receive*/
    Ret = pthread_create(&g_IrThread, NULL, IR_ReceiveTask, (HI_VOID *)&u32RecChn);
    if (0 != Ret)
    {
        PVR_SAMPLE_Printf(" ErrorCode=0x%x\n", Ret);
        perror("pthread_create");
    }

    return HI_SUCCESS;
}
#endif

