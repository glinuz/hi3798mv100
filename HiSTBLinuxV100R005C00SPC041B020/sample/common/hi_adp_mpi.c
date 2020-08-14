#include <string.h>
#include <sys/time.h>

#include "hi_unf_demux.h"
#include "hi_unf_avplay.h"
#include "hi_unf_common.h"
#include "hi_unf_vo.h"
#include "hi_unf_sound.h"
#include "hi_unf_ai.h"
#include "hi_common.h"
#include "hi_unf_disp.h"
#include "hi_unf_hdmi.h"
#include "hi_audio_codec.h"
#include "hi_error_mpi.h"
#include "hi_unf_mce.h"
#include "hi_unf_pdm.h"

#include "hi_adp_mpi.h"
#include "hi_adp.h"
#include "hi_adp_data.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"

#include "HA.AUDIO.G711.codec.h"
#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"
#include "HA.AUDIO.AMRWB.codec.h"
#include "HA.AUDIO.TRUEHDPASSTHROUGH.decode.h"
#include "HA.AUDIO.DOLBYTRUEHD.decode.h"
#include "HA.AUDIO.DTSHD.decode.h"
#if defined (DOLBYPLUS_HACODEC_SUPPORT)
 #include "HA.AUDIO.DOLBYPLUS.decode.h"
#endif
#include "HA.AUDIO.AC3PASSTHROUGH.decode.h"
#include "HA.AUDIO.DTSM6.decode.h"

#include "HA.AUDIO.DTSPASSTHROUGH.decode.h"
#include "HA.AUDIO.FFMPEG_DECODE.decode.h"
#include "HA.AUDIO.AAC.encode.h"

#ifdef ANDROID
#include <utils/Log.h>
#include "hi_adp_osd.h"
#endif

#define MPI_DEMUX_NUM 5
#define MPI_DEMUX_PLAY 0
#define MPI_DEMUX_REC_0 1
#define MPI_DEMUX_REC_1 2
#define MPI_DEMUX_TIMETHIFT 3
#define MPI_DEMUX_PLAYBACK 4

#define FIRST_TUNER   0
#define SECOND_TUNER  1
#define THIRD_TUNER   2
#define FOURTH_TUNER  3
/*
big-endian pcm output format, if extword is 1, choose normal pcm decoder,
                                            if extword is 2, choose wifidsp_lpcm decoder(Frame Header:0xA0,0x06)
                                            if others, fail to decode.
*/
#define NORMAL_PCM_EXTWORD    1
#define WIFIDSP_LPCM_EXTWORD  2


HI_U8 u8DecOpenBuf[1024];
HI_U8 u8EncOpenBuf[1024];

#if defined (DOLBYPLUS_HACODEC_SUPPORT)

DOLBYPLUS_STREAM_INFO_S g_stDDpStreamInfo;

/*dolby Dual Mono type control*/
HI_U32  g_u32DolbyAcmod = 0;
HI_BOOL g_bDrawChnBar = HI_TRUE;

#endif

#ifdef ANDROID
static HI_HANDLE               g_hSurface;

#define LOG_MAX_LEN 1024

void LogPrint(const char *format, ...)
{
    char LogStr[LOG_MAX_LEN];
    va_list args = {0};

    va_start(args, format);
    vsnprintf(LogStr, LOG_MAX_LEN, format, args);
    va_end(args);
    android_printLog(ANDROID_LOG_ERROR, "SAMPLE", "%s", LogStr);
}
#endif

/************************************DISPLAY Common Interface*******************************/
HI_S32 HIADP_Disp_StrToFmt(HI_CHAR *pszFmt)
{
    HI_UNF_ENC_FMT_E fmtReturn = HI_UNF_ENC_FMT_BUTT;

    if (NULL == pszFmt)
    {
        return HI_UNF_ENC_FMT_BUTT;
    }

    if (0 == strcasecmp(pszFmt, "1080P_60"))
    {
        fmtReturn = HI_UNF_ENC_FMT_1080P_60;
    }
    else if (0 == strcasecmp(pszFmt, "1080P_50"))
    {
        fmtReturn = HI_UNF_ENC_FMT_1080P_50;
    }
    else if (0 == strcasecmp(pszFmt, "1080P_30"))
    {
        fmtReturn = HI_UNF_ENC_FMT_1080P_30;
    }
    else if (0 == strcasecmp(pszFmt, "1080P_25"))
    {
        fmtReturn = HI_UNF_ENC_FMT_1080P_25;
    }
    else if (0 == strcasecmp(pszFmt, "1080P_24"))
    {
        fmtReturn = HI_UNF_ENC_FMT_1080P_24;
    }
    else if (0 == strcasecmp(pszFmt, "1080i_60"))
    {
        fmtReturn = HI_UNF_ENC_FMT_1080i_60;
    }
    else if (0 == strcasecmp(pszFmt, "1080i_50"))
    {
        fmtReturn = HI_UNF_ENC_FMT_1080i_50;
    }
    else if (0 == strcasecmp(pszFmt, "720P_60"))
    {
        fmtReturn = HI_UNF_ENC_FMT_720P_60;
    }
    else if (0 == strcasecmp(pszFmt, "720P_50"))
    {
        fmtReturn = HI_UNF_ENC_FMT_720P_50;
    }
    else if (0 == strcasecmp(pszFmt, "576P_50"))
    {
        fmtReturn = HI_UNF_ENC_FMT_576P_50;
    }
    else if (0 == strcasecmp(pszFmt, "480P_60"))
    {
        fmtReturn = HI_UNF_ENC_FMT_480P_60;
    }
    else if (0 == strcasecmp(pszFmt, "PAL"))
    {
        fmtReturn = HI_UNF_ENC_FMT_PAL;
    }
    else if (0 == strcasecmp(pszFmt, "NTSC"))
    {
        fmtReturn = HI_UNF_ENC_FMT_NTSC;
    }
    else if (0 == strcasecmp(pszFmt, "1080P_24_FP"))
    {
        fmtReturn = HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING;
    }
    else if (0 == strcasecmp(pszFmt, "720P_60_FP"))
    {
        fmtReturn = HI_UNF_ENC_FMT_720P_60_FRAME_PACKING;
    }
    else if (0 == strcasecmp(pszFmt, "720P_50_FP"))
    {
        fmtReturn = HI_UNF_ENC_FMT_720P_50_FRAME_PACKING;
    }
    else if (0 == strcasecmp(pszFmt, "2160P_24"))
    {
        fmtReturn = HI_UNF_ENC_FMT_3840X2160_24;
    }
    else if (0 == strcasecmp(pszFmt, "2160P_25"))
    {
        fmtReturn = HI_UNF_ENC_FMT_3840X2160_25;
    }
    else if (0 == strcasecmp(pszFmt, "2160P_30"))
    {
        fmtReturn = HI_UNF_ENC_FMT_3840X2160_30;
    }
    else if (0 == strcasecmp(pszFmt, "2160P_50"))
    {
        fmtReturn = HI_UNF_ENC_FMT_3840X2160_50;
    }
    else if (0 == strcasecmp(pszFmt, "2160P_60"))
    {
        fmtReturn = HI_UNF_ENC_FMT_3840X2160_60;
    }
    else
    {
        fmtReturn = HI_UNF_ENC_FMT_720P_50;
        sample_common_printf("\n!!! Can NOT match format, set format to is '720P_50'/%d.\n\n", HI_UNF_ENC_FMT_720P_50);
    }

    return fmtReturn;
}

HI_S32 HIADP_Disp_Init(HI_UNF_ENC_FMT_E enFormat)
{
    HI_S32                      Ret;
    HI_UNF_DISP_BG_COLOR_S      BgColor;
    HI_UNF_DISP_INTF_S          DispIntf;
    HI_UNF_DISP_OFFSET_S        offset;
    HI_UNF_ENC_FMT_E            SdFmt = HI_UNF_ENC_FMT_PAL;
#if defined(CHIP_TYPE_hi3798cv200)
    HI_UNF_EDID_BASE_INFO_S     stSinkAttr;
#endif

    Ret = HI_UNF_DISP_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("HI_UNF_DISP_Init failed 0x%x\n", Ret);
        return Ret;
    }

    DispIntf.enIntfType = HI_UNF_DISP_INTF_TYPE_HDMI;
    DispIntf.unIntf.enHdmi = HI_UNF_HDMI_ID_0;
    Ret = HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY1, &DispIntf, 1);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("HI_UNF_DISP_AttachIntf hdmi failed 0x%x\n", Ret);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

#if defined(HI_DAC_YPBPR_Y) && defined(HI_DAC_YPBPR_PB) && defined(HI_DAC_YPBPR_PR)
    DispIntf.enIntfType = HI_UNF_DISP_INTF_TYPE_YPBPR;
    DispIntf.unIntf.stYPbPr.u8DacY  = HI_DAC_YPBPR_Y;
    DispIntf.unIntf.stYPbPr.u8DacPb = HI_DAC_YPBPR_PB;
    DispIntf.unIntf.stYPbPr.u8DacPr = HI_DAC_YPBPR_PR;
    Ret = HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY1, &DispIntf, 1);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("HI_UNF_DISP_AttachIntf ypbpr failed 0x%x\n", Ret);
        HI_UNF_DISP_DeInit();
        return Ret;
    }
#endif

#ifdef HI_DAC_CVBS
    DispIntf.enIntfType = HI_UNF_DISP_INTF_TYPE_CVBS;
    DispIntf.unIntf.stCVBS.u8Dac = HI_DAC_CVBS;
    Ret = HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY0, &DispIntf, 1);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("HI_UNF_DISP_AttachIntf cvbs failed 0x%x\n", Ret);
        HI_UNF_DISP_DeInit();
        return Ret;
    }
#endif

    Ret = HI_UNF_DISP_Attach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("HI_UNF_DISP_Attach failed 0x%x\n", Ret);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HI_UNF_DISP_SetFormat(HI_UNF_DISPLAY1, enFormat);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("HI_UNF_DISP_SetFormat DISPLAY1 0x%x failed 0x%x\n", enFormat, Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    switch (enFormat)
    {
        case HI_UNF_ENC_FMT_4096X2160_60 :
        case HI_UNF_ENC_FMT_4096X2160_30 :
        case HI_UNF_ENC_FMT_4096X2160_24 :
        case HI_UNF_ENC_FMT_3840X2160_60 :
        case HI_UNF_ENC_FMT_3840X2160_30 :
        case HI_UNF_ENC_FMT_3840X2160_24 :
        case HI_UNF_ENC_FMT_1080P_60 :
        case HI_UNF_ENC_FMT_1080P_30 :
        case HI_UNF_ENC_FMT_1080i_60 :
        case HI_UNF_ENC_FMT_720P_60 :
        case HI_UNF_ENC_FMT_480P_60 :
        case HI_UNF_ENC_FMT_NTSC :
            SdFmt = HI_UNF_ENC_FMT_NTSC;
            break;

        case HI_UNF_ENC_FMT_4096X2160_50 :
        case HI_UNF_ENC_FMT_4096X2160_25 :
        case HI_UNF_ENC_FMT_3840X2160_50 :
        case HI_UNF_ENC_FMT_3840X2160_25 :
        case HI_UNF_ENC_FMT_1080P_50 :
        case HI_UNF_ENC_FMT_1080P_25 :
        case HI_UNF_ENC_FMT_1080i_50 :
        case HI_UNF_ENC_FMT_720P_50 :
        case HI_UNF_ENC_FMT_576P_50 :
        case HI_UNF_ENC_FMT_PAL :
            SdFmt = HI_UNF_ENC_FMT_PAL;
            break;

        default:
            break;
    }

    Ret = HI_UNF_DISP_SetFormat(HI_UNF_DISPLAY0, SdFmt);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("HI_UNF_DISP_SetFormat DISPLAY0 0x%x failed 0x%x\n", SdFmt, Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

#ifndef ANDROID
    Ret = HI_UNF_DISP_SetVirtualScreen(HI_UNF_DISPLAY1, 1280, 720);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("HI_UNF_DISP_SetVirtualScreen failed 0x%x\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    offset.u32Left      = 0;
    offset.u32Top       = 0;
    offset.u32Right     = 0;
    offset.u32Bottom    = 0;
    /*set display1 screen offset*/
    Ret = HI_UNF_DISP_SetScreenOffset(HI_UNF_DISPLAY1, &offset);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("HI_UNF_DISP_SetScreenOffset DISPLAY1 failed 0x%x\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    /*set display0 screen offset*/
    Ret = HI_UNF_DISP_SetScreenOffset(HI_UNF_DISPLAY0, &offset);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("HI_UNF_DISP_SetScreenOffset DISPLAY0 failed 0x%x\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }
#endif

    BgColor.u8Red   = 0;
    BgColor.u8Green = 0;
    BgColor.u8Blue  = 0;

    Ret = HI_UNF_DISP_SetBgColor(HI_UNF_DISPLAY1, &BgColor);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("HI_UNF_DISP_SetBgColor failed 0x%x\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HI_UNF_DISP_Open(HI_UNF_DISPLAY1);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("HI_UNF_DISP_Open DISPLAY1 failed 0x%x\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HI_UNF_DISP_Open(HI_UNF_DISPLAY0);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("HI_UNF_DISP_Open DISPLAY0 failed 0x%x\n", Ret);
        HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HIADP_HDMI_Init(HI_UNF_HDMI_ID_0);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("HIADP_HDMI_Init failed 0x%x\n", Ret);
        HI_UNF_DISP_Close(HI_UNF_DISPLAY0);
        HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }
 #if defined(CHIP_TYPE_hi3798cv200)    
    Ret = HI_UNF_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_0, &stSinkAttr);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_UNF_HDMI_GetSinkCapability failed:%#x.\n",Ret);
    }
    else
    {
        //sample_common_printf("----->bDolbySupport: %d, bHdrSupport: %d.\n",stSinkAttr.bDolbySupport,stSinkAttr.bHdrSupport);
        if((HI_TRUE  == stSinkAttr.bDolbySupport) && (HI_TRUE  == stSinkAttr.bHdrSupport))
        {
            /*目前尚无电视同时支持两种模式,保持SDR不变*/
        }
        else if(HI_TRUE  == stSinkAttr.bDolbySupport)
        {
            Ret = HI_UNF_DISP_SetHDRType(HI_UNF_DISPLAY1, HI_UNF_DISP_HDR_TYPE_DOLBY); 
            if (HI_SUCCESS != Ret)
            {
                sample_common_printf("call HI_UNF_DISP_SetHDRType: failed:%#x.\n",Ret);
                return Ret;
            }
        }
        else if(HI_TRUE  == stSinkAttr.bHdrSupport)
        {
            Ret = HI_UNF_DISP_SetHDRType(HI_UNF_DISPLAY1, HI_UNF_DISP_HDR_TYPE_HDR10); 
            if (HI_SUCCESS != Ret)
            {
                sample_common_printf("call HI_UNF_DISP_SetHDRType failed:%#x.\n",Ret);
                return Ret;
            }
        }
        else
        {
            /*SDR电视，保持SDR输出*/
        }
    }
#endif

#ifdef ANDROID
    HIADP_SURFACE_ATTR_S    stSurAttr;
    HI_UNF_PDM_DISP_PARAM_S stDispParam;

    HIADP_OSD_Init();

    Ret = HI_UNF_PDM_GetBaseParam(HI_UNF_PDM_BASEPARAM_DISP0, &stDispParam);
    if (HI_SUCCESS != Ret)
    {
        stSurAttr.u32Width = 1280;
        stSurAttr.u32Height = 720;
    }
    else
    {
        stSurAttr.u32Width = stDispParam.u32VirtScreenWidth;
        stSurAttr.u32Height = stDispParam.u32VirtScreenHeight;
    }

    stSurAttr.enPixelFormat = HIADP_PF_8888;
    Ret = HIADP_OSD_CreateSurface(&stSurAttr, &g_hSurface);
    if (HI_SUCCESS != Ret)
    {
        HI_UNF_DISP_Close(HI_UNF_DISPLAY0);
        HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        HIADP_OSD_DeInit();
        return Ret;
    }
#endif

    return HI_SUCCESS;
}


HI_S32 HIADP_Disp_DeInit(HI_VOID)
{
    HI_S32                      Ret;

#ifdef ANDROID
    Ret = HIADP_OSD_DestroySurface(g_hSurface);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    HIADP_OSD_DeInit();
#endif

    Ret = HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DISP_Close failed, Ret=%#x.\n", Ret);
        return Ret;
    }

    Ret = HI_UNF_DISP_Close(HI_UNF_DISPLAY0);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DISP_Close failed, Ret=%#x.\n", Ret);
        return Ret;
    }

    Ret = HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DISP_Detach failed, Ret=%#x.\n", Ret);
        return Ret;
    }

    Ret = HI_UNF_DISP_DeInit();
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DISP_DeInit failed, Ret=%#x.\n", Ret);
        return Ret;
    }

    HIADP_HDMI_DeInit(HI_UNF_HDMI_ID_0);

    return HI_SUCCESS;
}

/****************************VO Common Interface********************************************/
HI_S32 HIADP_VO_Init(HI_UNF_VO_DEV_MODE_E enDevMode)
{
    HI_S32             Ret;


    Ret = HI_UNF_VO_Init(enDevMode);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_VO_Init failed.\n");
        return Ret;
    }

#if 0
    Ret = HI_UNF_VO_Open(HI_UNF_DISPLAY1);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_VO_Open failed.\n");
        HI_UNF_VO_DeInit();
        return Ret;
    }
#endif
    return HI_SUCCESS;
}

HI_S32 HIADP_VO_CreatWin(HI_RECT_S *pstWinRect,HI_HANDLE *phWin)
{
    HI_S32 Ret;
    HI_UNF_WINDOW_ATTR_S   WinAttr;
    memset(&WinAttr, 0, sizeof(HI_UNF_WINDOW_ATTR_S));
    WinAttr.enDisp = HI_UNF_DISPLAY1;
    WinAttr.bVirtual = HI_FALSE;
    WinAttr.stWinAspectAttr.enAspectCvrs = HI_UNF_VO_ASPECT_CVRS_IGNORE;
    WinAttr.stWinAspectAttr.bUserDefAspectRatio = HI_FALSE;
    WinAttr.stWinAspectAttr.u32UserAspectWidth  = 0;
    WinAttr.stWinAspectAttr.u32UserAspectHeight = 0;
    WinAttr.bUseCropRect = HI_FALSE;
    WinAttr.stInputRect.s32X = 0;
    WinAttr.stInputRect.s32Y = 0;
    WinAttr.stInputRect.s32Width = 0;
    WinAttr.stInputRect.s32Height = 0;

    if (HI_NULL == pstWinRect)
    {
        memset(&WinAttr.stOutputRect, 0x0, sizeof(HI_RECT_S));
    }
    else
    {
        memcpy(&WinAttr.stOutputRect,pstWinRect,sizeof(HI_RECT_S));
    }

    Ret = HI_UNF_VO_CreateWindow(&WinAttr, phWin);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_VO_CreateWindow failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HIADP_VO_CreatWinExt(HI_RECT_S *pstWinRect,HI_HANDLE *phWin,HI_BOOL bVirtScreen)
{
    HI_S32 Ret;
    HI_UNF_WINDOW_ATTR_S   WinAttr;
    memset(&WinAttr, 0, sizeof(HI_UNF_WINDOW_ATTR_S));
    WinAttr.enDisp = HI_UNF_DISPLAY1;
    WinAttr.bVirtual = HI_FALSE;
    WinAttr.stWinAspectAttr.enAspectCvrs = HI_UNF_VO_ASPECT_CVRS_IGNORE;
    WinAttr.stWinAspectAttr.bUserDefAspectRatio = HI_FALSE;
    WinAttr.stWinAspectAttr.u32UserAspectWidth  = 0;
    WinAttr.stWinAspectAttr.u32UserAspectHeight = 0;
    WinAttr.bUseCropRect = HI_FALSE;
    WinAttr.stInputRect.s32X = 0;
    WinAttr.stInputRect.s32Y = 0;
    WinAttr.stInputRect.s32Width = 0;
    WinAttr.stInputRect.s32Height = 0;

    if (HI_NULL == pstWinRect)
    {
        memset(&WinAttr.stOutputRect, 0x0, sizeof(HI_RECT_S));
    }
    else
    {
        memcpy(&WinAttr.stOutputRect,pstWinRect,sizeof(HI_RECT_S));
    }

    Ret =  HI_UNF_VO_CreateWindowExt(&WinAttr, phWin, bVirtScreen);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_VO_CreateWindowExt failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HIADP_VO_DeInit()
{
    HI_S32         Ret;

#if 0
    Ret = HI_UNF_VO_Close(HI_UNF_DISPLAY1);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_VO_Close failed.\n");
        return Ret;
    }
#endif
    Ret = HI_UNF_VO_DeInit();
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_VO_DeInit failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

/*****************************************SOUND Common Interface************************************/
HI_S32 HIADP_Snd_Init(HI_VOID)
{
    HI_S32                  Ret;
    HI_UNF_SND_ATTR_S       stAttr;

    Ret = HI_UNF_SND_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_SND_Init failed.\n");
        return Ret;
    }
    Ret = HI_UNF_SND_GetDefaultOpenAttr(HI_UNF_SND_0, &stAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_SND_GetDefaultOpenAttr failed.\n");
        return Ret;
    }

    Ret = HI_UNF_SND_Open(HI_UNF_SND_0, &stAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_SND_Open failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HIADP_Snd_DeInit(HI_VOID)
{
    HI_S32                  Ret;

    Ret = HI_UNF_SND_Close(HI_UNF_SND_0);
    if (Ret != HI_SUCCESS )
    {
        sample_common_printf("call HI_UNF_SND_Close failed.\n");
        return Ret;
    }

    Ret = HI_UNF_SND_DeInit();
    if (Ret != HI_SUCCESS )
    {
        sample_common_printf("call HI_UNF_SND_DeInit failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

#ifdef HI_AUDIO_AI_SUPPORT
/*****************************************AI Common Interface************************************/
HI_S32 HIADP_AI_Init(HI_UNF_AI_E enAISrc, HI_HANDLE *pAIHandle, HI_HANDLE *pTrackSlave, HI_HANDLE *pATrackVir)
{
    HI_S32                  Ret;
    HI_UNF_AI_ATTR_S        stAitAttr = {0};
    HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;

    Ret = HI_UNF_AI_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_UNF_AI_Init failed.\n");
    }

    Ret = HI_UNF_AI_GetDefaultAttr(enAISrc,&stAitAttr);
    stAitAttr.u32PcmFrameMaxNum = 8;
    if(HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_UNF_AI_GetDefaultAttr Failed \n");
        return Ret;
    }

    Ret = HI_UNF_AI_Create(enAISrc, &stAitAttr, pAIHandle);
    if(HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_UNF_AI_Create Failed \n");
        return Ret;
    }


    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_SLAVE, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        return Ret;
    }
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr, pTrackSlave);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_SND_CreateTrack failed.\n");
        return Ret;
    }

    Ret = HI_UNF_SND_Attach(*pTrackSlave, *pAIHandle);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_SND_Attach failed.\n");
        return Ret;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_VIRTUAL, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        return Ret;
    }

    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,pATrackVir);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_SND_CreateTrack failed.\n");
        return Ret;
    }

    Ret = HI_UNF_SND_Attach(*pATrackVir, *pAIHandle);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_SND_Attach failed.\n");
        return Ret;
    }

    Ret = HI_UNF_AI_SetEnable(*pAIHandle, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_AI_SetEnable failed.\n");
        return Ret;
    }


    return HI_SUCCESS;
}

HI_S32 HIADP_AI_DeInit(HI_HANDLE hAI, HI_HANDLE hAISlave, HI_HANDLE hAIVir)
{
    HI_S32                  Ret;

    Ret = HI_UNF_AI_SetEnable(hAI, HI_FALSE);
    if (Ret != HI_SUCCESS )
    {
        sample_common_printf("call HI_UNF_AI_SetEnable failed.\n");
        return Ret;
    }

    HI_UNF_SND_Detach(hAIVir, hAI);
    HI_UNF_SND_DestroyTrack(hAIVir);
    HI_UNF_SND_Detach(hAISlave, hAI);
    HI_UNF_SND_DestroyTrack(hAISlave);

    HI_UNF_AI_Destroy(hAI);
    if (Ret != HI_SUCCESS )
    {
        sample_common_printf("call HI_UNF_AI_Destroy failed.\n");
        return Ret;
    }

    HI_UNF_AI_DeInit();
    if (Ret != HI_SUCCESS )
    {
        sample_common_printf("call HI_UNF_AI_DeInit failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}
#endif
#if 0  //v1r3
HI_S32 HIADP_Snd_RegAefAuthLib()
{
    HI_S32 Ret = HI_SUCCESS;

    Ret = HI_UNF_SND_RegisterAefAuthLib("libHA.AUDIO.SRS.effect.auth.so");

    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("\n\n!!! some audio effect authorize lib NOT found. you may NOT able to realize some audio effect process.\n\n");
    }

    return HI_SUCCESS;
}
#endif

HI_S32 HIADP_AVPlay_RegADecLib()
{
    HI_S32 Ret = HI_SUCCESS;

    Ret = HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.AMRWB.codec.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.MP3.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.MP2.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.AAC.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.DOLBYTRUEHD.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.DRA.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.TRUEHDPASSTHROUGH.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.AMRNB.codec.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.WMA.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.COOK.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.G711.codec.so");
#ifdef DOLBYPLUS_HACODEC_SUPPORT
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.DOLBYPLUS.decode.so");
#endif
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.DTSHD.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.DTSM6.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.DTSPASSTHROUGH.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.AC3PASSTHROUGH.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.PCM.decode.so");

    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("\n\n!!! some audio codec NOT found. you may NOT able to decode some audio type.\n\n");
    }

    return HI_SUCCESS;
}

HI_S32 HIADP_AVPlay_Init()
{
    HI_S32 Ret;
    Ret = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    return Ret;
}

HI_S32 HIADP_AVPlay_Create(HI_HANDLE *avplay,
                                 HI_U32 u32DemuxId,
                                 HI_UNF_AVPLAY_STREAM_TYPE_E streamtype,
                                 HI_UNF_VCODEC_CAP_LEVEL_E vdeccap,
                                 HI_U32 channelflag)
{
    HI_UNF_AVPLAY_ATTR_S attr;
    HI_HANDLE avhandle;
    HI_UNF_AVPLAY_OPEN_OPT_S maxCapbility;

    if(avplay == HI_NULL)
        return HI_FAILURE;

    if ((u32DemuxId != MPI_DEMUX_PLAY) && (u32DemuxId != MPI_DEMUX_PLAYBACK))
    {
        sample_common_printf("%d is not a play demux , please select play demux \n", u32DemuxId);
        return HI_FAILURE;
    }

    if(streamtype >= HI_UNF_AVPLAY_STREAM_TYPE_BUTT)
        return HI_FAILURE;

    if(vdeccap >= HI_UNF_VCODEC_CAP_LEVEL_BUTT)
        return HI_FAILURE;

    HIAPI_RUN_RETURN(HI_UNF_AVPLAY_GetDefaultConfig(&attr, streamtype));

    attr.u32DemuxId = u32DemuxId;
    attr.stStreamAttr.u32VidBufSize = 0x300000;
    HIAPI_RUN_RETURN(HI_UNF_AVPLAY_Create(&attr, &avhandle));
    maxCapbility.enDecType = HI_UNF_VCODEC_DEC_TYPE_NORMAL;
    maxCapbility.enCapLevel = vdeccap;
    maxCapbility.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_H264;

    if(channelflag&HI_UNF_AVPLAY_MEDIA_CHAN_AUD)
        HIAPI_RUN_RETURN(HI_UNF_AVPLAY_ChnOpen(avhandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL));

    if(channelflag&HI_UNF_AVPLAY_MEDIA_CHAN_VID)
        HIAPI_RUN_RETURN(HI_UNF_AVPLAY_ChnOpen(avhandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &maxCapbility));

    *avplay = avhandle;

    sample_common_printf("demux %d create avplay 0x%x  \n", u32DemuxId, avhandle);

    return HI_SUCCESS;
}

HI_S32 HIADP_AVPlay_SetVdecAttr(HI_HANDLE hAvplay,HI_UNF_VCODEC_TYPE_E enType,HI_UNF_VCODEC_MODE_E enMode)
{
    HI_S32 Ret;
    HI_UNF_VCODEC_ATTR_S        VdecAttr;

    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VdecAttr);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("HI_UNF_AVPLAY_GetAttr failed:%#x\n",Ret);
        return Ret;
    }

    VdecAttr.enType = enType;
    VdecAttr.enMode = enMode;
    VdecAttr.u32ErrCover = 100;
    VdecAttr.u32Priority = 3;

    Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VdecAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        return Ret;
    }

    return Ret;
}

#if defined (DOLBYPLUS_HACODEC_SUPPORT)

static HI_VOID DDPlusCallBack(DOLBYPLUS_EVENT_E Event, HI_VOID *pUserData)
{
    DOLBYPLUS_STREAM_INFO_S *pstInfo = (DOLBYPLUS_STREAM_INFO_S *)pUserData;
#if 0
    sample_common_printf( "DDPlusCallBack show info:\n \
                s16StreamType          = %d\n \
                s16Acmod               = %d\n \
                s32BitRate             = %d\n \
                s32SampleRateRate      = %d\n \
                Event                  = %d\n",
                pstInfo->s16StreamType, pstInfo->s16Acmod, pstInfo->s32BitRate, pstInfo->s32SampleRateRate,Event);
#endif
    g_u32DolbyAcmod = pstInfo->s16Acmod;

    if (HA_DOLBYPLUS_EVENT_SOURCE_CHANGE == Event)
    {
        g_bDrawChnBar = HI_TRUE;
        //printf("DDPlusCallBack enent !\n");
    }
    return;
}

#endif
HI_S32 HIADP_AVPlay_SetAdecAttr(HI_HANDLE hAvplay, HI_U32 enADecType, HI_HA_DECODEMODE_E enMode, HI_S32 isCoreOnly)
{
    HI_UNF_ACODEC_ATTR_S AdecAttr;
    WAV_FORMAT_S stWavFormat;

    HIAPI_RUN_RETURN(HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_ADEC, &AdecAttr));
    AdecAttr.enType = enADecType;

    if (HA_AUDIO_ID_PCM == AdecAttr.enType)
    {
        HI_BOOL isBigEndian;

        /* if big-endian pcm */
        isBigEndian = HI_FALSE;
        if(HI_TRUE == isBigEndian)
        {
            stWavFormat.cbSize = 4;
            stWavFormat.cbExtWord[0] = NORMAL_PCM_EXTWORD; //choose normal pcm decoder
            //stWavFormat.cbExtWord[0] = WIFIDSP_LPCM_EXTWORD; //choose wifi_dsp_lpcm decoder
        }

        if(stWavFormat.cbExtWord[0] == NORMAL_PCM_EXTWORD || HI_FALSE == isBigEndian)
        {
        /*
            if choose normal pcm decoder, set attribute
            if choose wifi_dsp_lpcm decoder, need not to set attribute by follows, ignore it
        */
        /* set pcm wav format here base on pcm file */
            stWavFormat.nChannels = 1;
            stWavFormat.nSamplesPerSec = 48000;
            stWavFormat.wBitsPerSample = 16;
        }
        HA_PCM_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam),&stWavFormat);
        sample_common_printf("please make sure the attributes of PCM stream is tme same as defined in function of \"HIADP_AVPlay_SetAdecAttr\"? \n");
        sample_common_printf("(nChannels = 1, wBitsPerSample = 16, nSamplesPerSec = 48000, isBigEndian = HI_FALSE) \n");
    }
    else if (HA_AUDIO_ID_MP2 == AdecAttr.enType)
    {
         HA_MP2_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam));
    }
    else if (HA_AUDIO_ID_AAC == AdecAttr.enType)
    {
         HA_AAC_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam));
    }
    else if (HA_AUDIO_ID_MP3 == AdecAttr.enType)
    {
         HA_MP3_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam));
    }
    else if (HA_AUDIO_ID_AMRNB == AdecAttr.enType)
    {
        AMRNB_DECODE_OPENCONFIG_S *pstConfig = (AMRNB_DECODE_OPENCONFIG_S *)u8DecOpenBuf;
        HA_AMRNB_GetDecDefalutOpenParam(&(AdecAttr.stDecodeParam), pstConfig);
        pstConfig->enFormat = AMRNB_MIME;
    }
    else if (HA_AUDIO_ID_AMRWB == AdecAttr.enType)
    {
        AMRWB_DECODE_OPENCONFIG_S *pstConfig = (AMRWB_DECODE_OPENCONFIG_S *)u8DecOpenBuf;
        HA_AMRWB_GetDecDefalutOpenParam(&(AdecAttr.stDecodeParam), pstConfig);
        pstConfig->enFormat = AMRWB_FORMAT_MIME;
    }
    else if (HA_AUDIO_ID_G711 == AdecAttr.enType)
    {
        G711_DECODE_OPENCONFIG_S *pstConfig = (G711_DECODE_OPENCONFIG_S *)u8DecOpenBuf;
        pstConfig->isAlaw = 1;
        pstConfig->u32SamplePerFrame = G711_FRAME_LEN;
        HA_G711_GetDecDefalutOpenParam(&(AdecAttr.stDecodeParam), pstConfig);
    }
    else if (HA_AUDIO_ID_AC3PASSTHROUGH == AdecAttr.enType)
    {
        HA_AC3PASSTHROUGH_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam));
        AdecAttr.stDecodeParam.enDecMode = HD_DEC_MODE_THRU;
    }
    else if(HA_AUDIO_ID_DTSPASSTHROUGH == AdecAttr.enType)
    {
                HA_DTSPASSTHROUGH_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam));
             AdecAttr.stDecodeParam.enDecMode = HD_DEC_MODE_THRU;
    }
    else if (HA_AUDIO_ID_TRUEHD == AdecAttr.enType)
    {
        HA_TRUEHD_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam));
        if (HD_DEC_MODE_THRU != enMode)
        {
            sample_common_printf(" MLP decoder enMode(%d) error (mlp only support hbr Pass-through only).\n", enMode);
            return -1;
        }

        AdecAttr.stDecodeParam.enDecMode = HD_DEC_MODE_THRU;        /* truehd just support pass-through */
        sample_common_printf(" TrueHD decoder(HBR Pass-through only).\n");
    }
    else if (HA_AUDIO_ID_DOLBY_TRUEHD == AdecAttr.enType)
    {
          TRUEHD_DECODE_OPENCONFIG_S *pstConfig = (TRUEHD_DECODE_OPENCONFIG_S *)u8DecOpenBuf;
        HA_DOLBY_TRUEHD_DecGetDefalutOpenConfig(pstConfig);
        HA_DOLBY_TRUEHD_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam), pstConfig);
    }
    else if (HA_AUDIO_ID_DTSHD == AdecAttr.enType)
    {
        DTSHD_DECODE_OPENCONFIG_S *pstConfig = (DTSHD_DECODE_OPENCONFIG_S *)u8DecOpenBuf;
        HA_DTSHD_DecGetDefalutOpenConfig(pstConfig);
        HA_DTSHD_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam), pstConfig);
        AdecAttr.stDecodeParam.enDecMode = HD_DEC_MODE_SIMUL;
    }
    else if (HA_AUDIO_ID_DTSM6 == AdecAttr.enType)
    {
        DTSM6_DECODE_OPENCONFIG_S *pstConfig = (DTSM6_DECODE_OPENCONFIG_S *)u8DecOpenBuf;
        HA_DTSM6_DecGetDefalutOpenConfig(pstConfig);
        HA_DTSM6_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam), pstConfig);
    }
#if defined (DOLBYPLUS_HACODEC_SUPPORT)
    else if (HA_AUDIO_ID_DOLBY_PLUS == AdecAttr.enType)
    {
        DOLBYPLUS_DECODE_OPENCONFIG_S *pstConfig = (DOLBYPLUS_DECODE_OPENCONFIG_S *)u8DecOpenBuf;
        HA_DOLBYPLUS_DecGetDefalutOpenConfig(pstConfig);
        pstConfig->pfnEvtCbFunc[HA_DOLBYPLUS_EVENT_SOURCE_CHANGE] = DDPlusCallBack;
        pstConfig->pAppData[HA_DOLBYPLUS_EVENT_SOURCE_CHANGE] = &g_stDDpStreamInfo;
        /* Dolby DVB Broadcast default settings */
        pstConfig->enDrcMode = DOLBYPLUS_DRC_RF;
        pstConfig->enDmxMode = DOLBYPLUS_DMX_SRND;
        HA_DOLBYPLUS_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam), pstConfig);
        AdecAttr.stDecodeParam.enDecMode = HD_DEC_MODE_SIMUL;
    }
#endif
    else
    {
//       HA_DRA_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam));
         HA_DRA_DecGetOpenParam_MultichPcm(&(AdecAttr.stDecodeParam));
    }

    HIAPI_RUN_RETURN(HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_ADEC, &AdecAttr));

    return HI_SUCCESS;
}

HI_S32 HIADP_AVPlay_PlayProg(HI_HANDLE hAvplay,PMT_COMPACT_TBL *pProgTbl,HI_U32 ProgNum,HI_BOOL bAudPlay)
{
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    HI_U32                  VidPid;
    HI_U32                  AudPid;
    HI_U32                  PcrPid;
    HI_UNF_VCODEC_TYPE_E    enVidType;
    HI_U32                  u32AudType;
    HI_S32                  Ret;

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;
    Ret = HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_UNF_AVPLAY_Stop failed.\n");
        return Ret;
    }

    ProgNum = ProgNum % pProgTbl->prog_num;
    if (pProgTbl->proginfo[ProgNum].VElementNum > 0 )
    {
        VidPid = pProgTbl->proginfo[ProgNum].VElementPid;
        enVidType = pProgTbl->proginfo[ProgNum].VideoType;
    }
    else
    {
        VidPid = INVALID_TSPID;
        enVidType = HI_UNF_VCODEC_TYPE_BUTT;
    }

    if (pProgTbl->proginfo[ProgNum].AElementNum > 0)
    {
        AudPid  = pProgTbl->proginfo[ProgNum].AElementPid;
        u32AudType = pProgTbl->proginfo[ProgNum].AudioType;
    }
    else
    {
        AudPid = INVALID_TSPID;
        u32AudType = 0xffffffff;
    }

    PcrPid = pProgTbl->proginfo[ProgNum].PcrPid;
    if (INVALID_TSPID != PcrPid)
    {
        HI_UNF_SYNC_ATTR_S  SyncAttr;

        Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
        if (HI_SUCCESS != Ret)
        {
            sample_common_printf("HI_UNF_AVPLAY_GetAttr Sync failed 0x%x\n", Ret);
            return Ret;
        }

        if (HI_UNF_SYNC_REF_PCR == SyncAttr.enSyncRef)
        {
            Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_PCR_PID, &PcrPid);
            if (HI_SUCCESS != Ret)
            {
                sample_common_printf("HI_UNF_AVPLAY_SetAttr Sync failed 0x%x\n", Ret);
                return Ret;
            }
        }
    }

    if (VidPid != INVALID_TSPID)
    {
        Ret = HIADP_AVPlay_SetVdecAttr(hAvplay,enVidType,HI_UNF_VCODEC_MODE_NORMAL);
        Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VID_PID,&VidPid);
        if (Ret != HI_SUCCESS)
        {
            sample_common_printf("call HIADP_AVPlay_SetVdecAttr failed.\n");
            return Ret;
        }

        Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            sample_common_printf("call HI_UNF_AVPLAY_Start failed.\n");
            return Ret;
        }
    }

    if (HI_TRUE == bAudPlay && AudPid != INVALID_TSPID)
    {
        //u32AudType = HA_AUDIO_ID_DTSHD;
        //printf("u32AudType = %#x\n",u32AudType);
        Ret  = HIADP_AVPlay_SetAdecAttr(hAvplay, u32AudType, HD_DEC_MODE_RAWPCM, 1);

        Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &AudPid);
        if (HI_SUCCESS != Ret)
        {
            sample_common_printf("HIADP_AVPlay_SetAdecAttr failed:%#x\n",Ret);
            return Ret;
        }

        Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            sample_common_printf("call HI_UNF_AVPLAY_Start to start audio failed.\n");
            //return Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HIADP_AVPlay_PlayAud(HI_HANDLE hAvplay,PMT_COMPACT_TBL *pProgTbl,HI_U32 ProgNum)
{
    HI_U32                  AudPid;
    HI_U32                  u32AudType;
    HI_S32                  Ret;

    Ret = HI_UNF_AVPLAY_Stop(hAvplay,HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_UNF_AVPLAY_Stop failed.\n");
        return Ret;
    }

    ProgNum = ProgNum % pProgTbl->prog_num;
    if (pProgTbl->proginfo[ProgNum].AElementNum > 0)
    {
        AudPid  = pProgTbl->proginfo[ProgNum].AElementPid;
        u32AudType = pProgTbl->proginfo[ProgNum].AudioType;
    }
    else
    {
        AudPid = INVALID_TSPID;
        u32AudType = 0xffffffff;
    }

    if (AudPid != INVALID_TSPID)
    {
        Ret  = HIADP_AVPlay_SetAdecAttr(hAvplay, u32AudType, HD_DEC_MODE_RAWPCM, 1);
        Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &AudPid);
        if (HI_SUCCESS != Ret)
        {
            sample_common_printf("HIADP_AVPlay_SetAdecAttr failed:%#x\n",Ret);
            return Ret;
        }

        Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            sample_common_printf("call HI_UNF_AVPLAY_Start failed.\n");
            return Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HIADP_AVPlay_SwitchAud(HI_HANDLE hAvplay,HI_U32 AudPid, HI_U32 u32AudType)
{
    HI_S32 Ret = HI_SUCCESS;

    if (AudPid == INVALID_TSPID)
    {
        sample_common_printf("%s, audio pid is invalid!\n", __func__);
        return HI_FAILURE;
    }

    Ret = HI_UNF_AVPLAY_Stop(hAvplay,HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_UNF_AVPLAY_Stop failed.\n");
        return Ret;
    }


    Ret  = HIADP_AVPlay_SetAdecAttr(hAvplay, u32AudType, HD_DEC_MODE_RAWPCM, 1);
    Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &AudPid);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("HIADP_AVPlay_SetAdecAttr failed:%#x\n",Ret);
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_AVPLAY_Start failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}


HI_S32 HIADP_MCE_Exit(HI_VOID)
{
#ifndef ANDROID
    HI_S32                  Ret;
    HI_UNF_MCE_STOPPARM_S   stStop;

    Ret = HI_UNF_MCE_Init(HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_UNF_MCE_Init failed, Ret=%#x!\n", Ret);
        return Ret;
    }

    Ret = HI_UNF_MCE_ClearLogo();
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_UNF_MCE_ClearLogo failed, Ret=%#x!\n", Ret);
        return Ret;
    }

    stStop.enStopMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    stStop.enCtrlMode = HI_UNF_MCE_PLAYCTRL_BY_TIME;
    stStop.u32PlayTimeMs = 0;
    Ret = HI_UNF_MCE_Stop(&stStop);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_UNF_MCE_Stop failed, Ret=%#x!\n", Ret);
        return Ret;
    }

    Ret = HI_UNF_MCE_Exit(HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_UNF_MCE_Exit failed, Ret=%#x!\n", Ret);
        return Ret;
    }

    HI_UNF_MCE_DeInit();
#endif
    return HI_SUCCESS;
}
HI_S32 HIADP_DMX_AttachTSPort(HI_U32 Dmxid, HI_U32 TunerID)
{
    HI_S32                      Ret;
    HI_UNF_DMX_PORT_E           DmxAttachPort;
    HI_UNF_DMX_PORT_ATTR_S      DmxAttachPortAttr;

    if (FIRST_TUNER == TunerID)
    {
        DmxAttachPort = HI_DEMUX_PORT;
    }
    else if (SECOND_TUNER == TunerID)
    {
        DmxAttachPort = HI_DEMUX1_PORT;
    }
    else if (THIRD_TUNER == TunerID)
    {
        DmxAttachPort = HI_DEMUX2_PORT;
    }
    else if (FOURTH_TUNER == TunerID)
    {
        DmxAttachPort = HI_DEMUX3_PORT;
    }
    else
    {
        printf("No this tuner device\n");
        return HI_FAILURE;
    }
    
    Ret = HI_UNF_DMX_GetTSPortAttr(DmxAttachPort, &DmxAttachPortAttr);
    switch (DmxAttachPort)
    {
    case HI_UNF_DMX_PORT_IF_0:
    default:
        DmxAttachPortAttr.enPortType = HI_IF0_TYPE;
        DmxAttachPortAttr.u32SerialBitSelector = HI_IF0_BIT_SELECTOR;
        break;
    case HI_UNF_DMX_PORT_TSI_0:
        DmxAttachPortAttr.enPortType = HI_TSI0_TYPE;
        DmxAttachPortAttr.u32SerialBitSelector = HI_TSI0_BIT_SELECTOR;
        DmxAttachPortAttr.u32TunerInClk = HI_TSI0_CLK_PHASE_REVERSE;
        break;
    case HI_UNF_DMX_PORT_TSI_1:
        DmxAttachPortAttr.enPortType = HI_TSI1_TYPE;
        DmxAttachPortAttr.u32SerialBitSelector = HI_TSI1_BIT_SELECTOR;
        DmxAttachPortAttr.u32TunerInClk = HI_TSI1_CLK_PHASE_REVERSE;
        break;
    case HI_UNF_DMX_PORT_TSI_2:
        DmxAttachPortAttr.enPortType = HI_TSI2_TYPE;
        DmxAttachPortAttr.u32SerialBitSelector = HI_TSI2_BIT_SELECTOR;
        DmxAttachPortAttr.u32TunerInClk = HI_TSI2_CLK_PHASE_REVERSE;
        break;
    case HI_UNF_DMX_PORT_TSI_3:
        DmxAttachPortAttr.enPortType = HI_TSI3_TYPE;
        DmxAttachPortAttr.u32SerialBitSelector = HI_TSI3_BIT_SELECTOR;
        DmxAttachPortAttr.u32TunerInClk = HI_TSI3_CLK_PHASE_REVERSE;
        break;
    case HI_UNF_DMX_PORT_TSI_4:
        DmxAttachPortAttr.enPortType = HI_TSI4_TYPE;
        DmxAttachPortAttr.u32SerialBitSelector = HI_TSI4_BIT_SELECTOR;
        DmxAttachPortAttr.u32TunerInClk = HI_TSI4_CLK_PHASE_REVERSE;
        break;
    case HI_UNF_DMX_PORT_TSI_5:
        DmxAttachPortAttr.enPortType = HI_TSI5_TYPE;
        DmxAttachPortAttr.u32SerialBitSelector = HI_TSI5_BIT_SELECTOR;
        DmxAttachPortAttr.u32TunerInClk = HI_TSI5_CLK_PHASE_REVERSE;
        break;
    }
    
    Ret |= HI_UNF_DMX_SetTSPortAttr(DmxAttachPort, &DmxAttachPortAttr);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_DMX_SetTSPortAttr failed.\n");
        return HI_FAILURE;
    }
    
    Ret = HI_UNF_DMX_AttachTSPort(Dmxid, DmxAttachPort);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_DMX_AttachTSPort.\n");
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}
