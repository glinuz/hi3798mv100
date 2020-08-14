#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <string.h>

#include "hifb.h"
#include "hi_go.h"
#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_descrambler.h"
#include "hi_unf_demux.h"
#include "hi_unf_hdmi.h"
#include "hi_unf_ecs.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_pvr.h"
#include "hi_video_codec.h"


#if defined(CHIP_TYPE_hi3716mv310)
#define PVR_MAX_PLAY_CHN (1)
#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
#define PVR_MAX_PLAY_CHN (2)
#else
#define PVR_MAX_PLAY_CHN (3)
#endif
#define PVR_MAX_REC_CHN (4)

#if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
#define PVR_WIN_WIDTH (320)
#define PVR_WIN_HEIGHT (180)

#define PVR_ZOOM_HEIGHT_STEP (32)
#define PVR_ZOOM_WIDTH_STEP (60)
#define PVR_ZOOM_X_STEP (26)
#else
#define PVR_WIN_WIDTH (480)
#define PVR_WIN_HEIGHT (270)

#define PVR_ZOOM_HEIGHT_STEP (48)
#define PVR_ZOOM_WIDTH_STEP (90)
#define PVR_ZOOM_X_STEP (40)
#endif

#define DEMUX_ID_0 (0)
#define DEMUX_ID_1 (1)
#define DEMUX_ID_2 (2)
#define DEMUX_ID_3 (3)
#define DEMUX_ID_4 (4)
#define DEMUX_ID_5 (5)
#define DEMUX_ID_6 (6)

#define FRAMEBUFF_PATH "/dev/fb0"
#define HISILICON_LOGO_PATH "../res/jpg/performance.jpg"

#define BLUE   0xff0000ff
#define RED    0xffff0000
#define GREEN  0xff00ff00
#define WHITE  0xffffffff
#define YELLOW 0xffffff00
#define PVR_INFO_COLOR (0xff3b7bbf)//(0xff405278)
#define PVR_PROGRESS_COLOR YELLOW
#define PVR_ALERT_COLOR RED
#define PVR_DEMUX_BUFF_SIZE (0x100000)

#define PVR_IR_POWER  0x639cff00
#define PVR_IR_EPG    0x629dff00
#define PVR_IR_ZOOM   0x6897ff00
#define PVR_IR_LEFT   0x6699ff00
#define PVR_IR_RIGHT  0x3ec1ff00
#define PVR_IR_UP     0x35caff00
#define PVR_IR_DOWN   0x2dd2ff00
#define PVR_IR_NPVR   0x3dc2ff00

#define PVR_IR_NUM_EN 0x34cbff00 //-/--
#define PVR_IR_0      0x7887ff00
#define PVR_IR_1      0x6d92ff00
#define PVR_IR_2      0x6c93ff00
#define PVR_IR_3      0x33ccff00
#define PVR_IR_4      0x718eff00
#define PVR_IR_5      0x708fff00
#define PVR_IR_6      0x37c8ff00
#define PVR_IR_7      0x758aff00
#define PVR_IR_8      0x748bff00
#define PVR_IR_9      0x3bc4ff00
#define PVR_IR_OK     0x31ceff00
#define PVR_IR_PLAY_PAUSE 0x3cc3ff00
#define PVR_IR_VOD    0x659aff00
#define PVR_IR_BTV    0x649bff00
#define PVR_IR_PGUP   0x30cfff00
#define PVR_IR_PGDOWN 0x6798ff00
#define PVR_IR_REW    0x25daff00
#define PVR_IR_FF     0x29d6ff00
#define PVR_IR_BACK   0x6f90ff00
#define PVR_IR_STOP   0x2fd0ff00
#define PVR_IR_MUTE   0x22ddff00

#define PVR_IR_F1     0x7b84ff00
#define PVR_IR_F2     0x7689ff00


#define PVR_MAX_INFO_SIZE (256)

/*conax 测试码流 INTEGRATION_Chipset_SingleCrypto.ts 的全局key*/
/*
the service Conax2 shall be used.
The file is 10 seconds long.
for the first 7 seconds, ODD CW is used. the remainder uses EVEN CW.

ODD CW:
clear CW:
FE 17 2C 41 A2 E1 D4 57
CW encrypted with CWPK:
44 2C F6 2E 00 D0 77 96

Even CW:
clear CW:
D4 22 82 78 1D 28 72 B7
CW encrypted with CWPK:
D8 DC 8E 22 04 1A C4 9B

CWPK:
See separate file.


HI_U8       Odd_CW[8] = {0xFE ,0x17 ,0x2C ,0x41 ,0xA2 ,0xE1 ,0xD4 ,0x57};
HI_U8       Even_CW[8] = {0xD4 ,0x22 ,0x82 ,0x78 ,0x1D ,0x28 ,0x72 ,0xB7};

*/



/* suma 测试码流 suma_big.ts
static HI_U8 Odd_CW[8] = {0xff ,0xff ,0xff ,0xfd ,0xff ,0xff, 0xff ,0xfd};
static HI_U8 Even_CW[8] = {0xff ,0xff ,0xff ,0xfd ,0xff ,0xff, 0xff ,0xfd};
static HI_U8 g_u8EncrytedOddKey[8] = {0xf0,0x0e,0x15,0x28,0x8a,0xdd,0x94,0x81};
static HI_U8 g_u8EncrytedEvenKey[8] = {0xf0,0x0e,0x15,0x28,0x8a,0xdd,0x94,0x81};
*/
static HI_U8 Odd_CW[8] = {0xff ,0xff ,0xff ,0xfd ,0xff ,0xff, 0xff ,0xfd};
static HI_U8 Even_CW[8] = {0xff ,0xff ,0xff ,0xfd ,0xff ,0xff, 0xff ,0xfd};
/*HIGO的全局变量*/
HI_HANDLE hLayer_pvr = HI_INVALID_HANDLE;
HI_HANDLE hLayerSurface_pvr = HI_INVALID_HANDLE;
HI_HANDLE hFont_pvr = HI_INVALID_HANDLE;
HI_RECT rc_Str = {100, 200, 100, 30};

typedef struct pvrPLAY_CHN_ATTR_S
{
    HI_HANDLE hPlayChn;
    HI_HANDLE hWindow;
    HI_HANDLE hAvplay;
    HI_HANDLE hSoundTrack;
    HI_HANDLE hTsBufForPlayback;

    HI_BOOL bPlayChnInited;
    HI_BOOL bPlayBackPaused;
    HI_BOOL bVideoExist;
    HI_BOOL bMuteSeted;
    HI_BOOL bFullScreen;
    HI_U32 u32PvrPlayChn;
    HI_U32 u32DemuxId;
    HI_U32 u32WindowZorder;
    HI_U32 u32FFSpeedTimes;
    HI_U32 u32FBSpeedTimes;
    HI_U32 u32SFSpeedTimes;

    HI_RECT_S  stRect;
    HI_RECT stMuteTextRect;
    HI_UNF_WINDOW_ATTR_S stWinAttr;
}PVR_PLAY_CHN_ATTR;

typedef struct pvrTimeShift_CHN_ATTR_S
{
    HI_HANDLE hPlayChn;
    HI_HANDLE hWindow;
    HI_HANDLE hAvplay;
    HI_HANDLE hSoundTrack;
    HI_HANDLE hTsBufForPlayback;

    HI_BOOL bRecChnInited;
    HI_BOOL bPlayChnInited;
    HI_BOOL bLiveStoped;
    HI_BOOL bPlayBackPaused;
    HI_BOOL bVideoExist;
    HI_BOOL bMuteSeted;
    HI_BOOL bFullScreen;
    HI_U32 u32PvrPlayChn;
    HI_U32 u32PvrRecChn;
    HI_U32 u32PlayDemuxId;
    HI_U32 u32RecDemuxId;
    HI_U32 u32WindowZorder;
    HI_U32 u32ProgNum;
    HI_U32 u32DmxPort;
    HI_U32 u32FFSpeedTimes;
    HI_U32 u32FBSpeedTimes;
    HI_U32 u32SFSpeedTimes;

    HI_RECT_S  stRect;
    HI_RECT stMuteTextRect;
    HI_UNF_WINDOW_ATTR_S stWinAttr;
}PVR_TIMESHIFT_CHN_ATTR;



/*
    Play Channel 1:
             1X                 30fps                 00:02:01/01:00:00
             10.123 Mbps   12/1024 (MB)      567/7200
             |||||||||||||||||||||||||||||||||||||||||||   xxx.ts
*/
typedef struct pvrDEMO_PLAY_INFO_RECT_S
{
    HI_RECT stPlayChnName;         //显示"Play Channel 1"等
    HI_RECT stStreamInfoRect;      //显示音视频格式，宽高
    HI_RECT stPlaySpeedRect;       //显示"1X"或"2X"或"-2X"等
    HI_RECT stPlayFps;             //显示"xxx  /  xxx"
    HI_RECT stPlayTimeProgress;   //显示"xxx  /  xxx"

    HI_RECT stReadSpeedRect;      //显示"xxx.xxx   Mpbs"
    HI_RECT stFilePosRect;        //显示"xxx  /  xxx (MB)"
    HI_RECT stFrmPosRect;         //显示"xxx  /  xxx"

    HI_RECT stPlayProgressBarRect; //显示播放进度条
    HI_RECT stPlayFileNameRect;    //显示播放文件名

    HI_RECT stPlayDmxBuffBarRect; //显示当前使用demux的tsbuffer状态
    HI_RECT stPlayDmxNameRect;    //显示当前使用demux编号
}PVR_PLAY_INFO_RECT;

/*
        Rec Channel 1:
             10.123 Mbps   12 /1024 (MB)     Rewind          By Size
                    或                  或              或
             140 / 600 (s)       NO Rewind     By Time
                    或                  或              或
             120 (MB)               140 (s)        No Limit
             |||||||||||||||||||||||||||||||||||||||||||   xxx.ts
*/

typedef struct pvrDEMO_REC_INFO_RECT_S
{
    HI_RECT stRecChnName;         //显示"Rec Channel 1"等
    HI_RECT stValidInfoRect;      //显示录制码流的有效时间和大小
    HI_RECT stWriteSpeedRect;  //显示"xxx.xxx   Mpbs"
    HI_RECT stSizePosRect;     //显示"xxx  /  xxx (MB)"    按文件大小录制，后面显示预设大小
    HI_RECT stTimePosRect;     //显示"xxx  /  xxx (MB)"    按时间长度录制，后面实现预设时间
    HI_RECT stRecFileSizeRect;
    HI_RECT stRecFileTimeRect;
    HI_RECT stReWindType;     //显示"Rewind" 或"NO Rewind"
    HI_RECT stRecType;         //显示"By size" 或"By time"或"Both" 或"NoLimit"
    HI_RECT stEncryptRect;     //显示"Encrypt" 或"Uncrypt"


    HI_RECT stRecProgressBarRect; //显示播放进度条
    HI_RECT stRecFileNameRect;    //显示播放文件名

    HI_RECT stRecTsBufBarRect; //录制状态条
    HI_RECT stDmxFileNameRect;    //显示录制文件名
}PVR_REC_INFO_RECT;


typedef struct pvrDEMO_HIGO_ATTR_S
{
    HI_HANDLE hLayer;
    HI_HANDLE hLayerSurface;
    HI_HANDLE hFont;
}PVR_DEMO_HIGO_ATTR;

typedef enum pvrTEST_MODE
{
    PVR_TEST_MODE_TIMESHIFT = 0,
    PVR_TEST_MODE_PLAYBACK,
    PVR_TEST_MODE_RECORDING
}PVR_TEST_MODE;

typedef enum pvrUI_DEFINITION
{
    PVR_UI_DEFINITION_720P = 0,
    PVR_UI_DEFINITION_1080P
}PVR_UI_DEFINITION;

extern HI_VOID PVR_MarkerEventTime();
static HI_U32 g_PlayChn = 0;
static HI_U32 g_TimeshiftChn = 0;
static HI_CHAR *g_sRecPath = HI_NULL;
static PVR_PLAY_CHN_ATTR g_stPlayChnAttr[PVR_MAX_PLAY_CHN];
static PVR_TIMESHIFT_CHN_ATTR g_stTimeShiftChnAttr[PVR_MAX_PLAY_CHN];
static PVR_PLAY_INFO_RECT g_stPlayInfoRect[PVR_MAX_PLAY_CHN];
static PVR_REC_INFO_RECT g_stRecInfoRect[PVR_MAX_REC_CHN];

static HI_BOOL g_bIsMainRoutineStop = HI_FALSE;
static HI_BOOL g_bIsEthSocketRecvThreadStop = HI_FALSE;
static HI_HANDLE g_hEthSocketRecvTsBuff = 0;
static PMT_COMPACT_TBL *g_pstProgTbl;
static pthread_mutex_t  g_stDrawRectMutex;

static struct sockaddr_in g_stServerAddr;
static struct ip_mreq g_stIpMreq;
static HI_U32              EventTime = 0;
static HI_S8 g_s8VideoType[][32] = {"MPEG2", "MPEG4 DIVX4 DIVX5", "AVS", "H263", "H264",
                                      "REAL8", "REAL9", "VC-1", "VP6", "VP6F", "VP6A", "MJPEG",
                                      "SORENSON", "DIVX3", "RAW", "JPEG", "VP8", "MSMPEG4V1",
                                      "MSMPEG4V2", "MSVIDEO1", "WMV1", "WMV2", "RV10", "RV20",
                                      "SVQ1", "SVQ3", "H261", "VP3", "VP5", "CINEPAK", "INDEO2",
                                      "INDEO3", "INDEO4", "INDEO5", "MJPEGB", "MVC", "HEVC", "DV", "UNKONOW"};


static HI_S8 g_s8AudioType[][32] = {"PCM",
                                      "AAC",
                                      "MP2",
                                      "MP3",
                                      "",
                                      "",
                                      "WMA9STD",
                                      "DRA",
                                      "TRUEHD",
                                      "COOK",
                                      "",
                                      "",
                                      "",
                                      "",
                                      "",
                                      "",
                                      "DOLBY_PLUS",
                                      "DOLBY_TRUEHD"
                                      "UNKONOW"};

static HI_CHAR g_aHisiliconLogoPath[256] = {0};
static HI_HANDLE g_hNorcaDesc = HI_NULL;

HI_S32 PVR_AddALivePlay(HI_U32 u32ProgramNum);
HI_S32 PVR_ZoomInDispToFullScreen(HI_HANDLE hWindow, HI_RECT_S *pstOrgRect);
HI_S32 PVR_ZoomOutDispToWindow(HI_HANDLE hWindow, HI_RECT_S *pstOrgRect, HI_U32 u32WindowZorder);
HI_S32 PVR_AddARec(HI_U32 u32ChnNum,
                           const HI_CHAR *pcRecPath,
                           HI_U32 u32ProgramNum,
                           HI_BOOL bRewind,
                           HI_BOOL bCipher,
                           HI_U64 u64MaxRecSize,
                           HI_U64 u64MaxRecTimeMs,
                           HI_U32 *pu32RecChn,
                           PVR_TEST_MODE enTestMode);
HI_S32 PVR_StartLive(HI_U32 u32ProgramNum, HI_U32 u32DmxId, HI_HANDLE hAvplay, HI_BOOL *pbVedioExist, HI_U32 u32DmxPort);
HI_S32 PVR_StopLive(HI_HANDLE hAvplay);
HI_S32 PVR_StartPlay(const HI_CHAR *pszFileName,
                          HI_U32 u32ChnNum,
                          HI_U32 u32DmxId,
                          HI_HANDLE hAvplay,
                          HI_HANDLE *phPlaybackTsbuff,
                          HI_U32 *pu32PvrPlayChn,
                          HI_BOOL *pbVdeioExist);
HI_VOID PVR_StopPlay(HI_U32 playChn, HI_U32 u32DmxId, HI_HANDLE hDmxTsBuf);


HI_S32 PVR_Disp_Init(HI_UNF_ENC_FMT_E enFormat)
{
    HI_S32                      Ret;
    HI_UNF_DISP_BG_COLOR_S      BgColor;
    HI_UNF_DISP_INTF_S          DispIntf;
    HI_UNF_DISP_OFFSET_S        offset;

    Ret = HI_UNF_DISP_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DISP_Init failed, Ret=%#x.\n", Ret);
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
        sample_common_printf("call HI_UNF_DISP_AttachIntf failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_DeInit();
        return Ret;
    }
#endif

    DispIntf.enIntfType = HI_UNF_DISP_INTF_TYPE_HDMI;
    DispIntf.unIntf.enHdmi = HI_UNF_HDMI_ID_0;
    Ret = HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY1, &DispIntf, 1);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DISP_AttachIntf failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

#ifdef HI_DAC_CVBS
    DispIntf.enIntfType = HI_UNF_DISP_INTF_TYPE_CVBS;
    DispIntf.unIntf.stCVBS.u8Dac = HI_DAC_CVBS;
    Ret = HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY0, &DispIntf, 1);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DISP_AttachIntf failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_DeInit();
        return Ret;
    }
#endif

    Ret = HI_UNF_DISP_Attach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DISP_Attach failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_DeInit();
        return Ret;
    }
    /* set display1 format*/
    Ret = HI_UNF_DISP_SetFormat(HI_UNF_DISPLAY1, enFormat);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DISP_SetFormat failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }
    if ((HI_UNF_ENC_FMT_1080P_60 == enFormat)
        ||(HI_UNF_ENC_FMT_1080i_60 == enFormat)
        ||(HI_UNF_ENC_FMT_720P_60 == enFormat)
        ||(HI_UNF_ENC_FMT_480P_60 == enFormat)
        ||(HI_UNF_ENC_FMT_NTSC == enFormat)
        ||(HI_UNF_ENC_FMT_3840X2160_30 == enFormat)
        ||(HI_UNF_ENC_FMT_3840X2160_24 == enFormat))
    {
        Ret = HI_UNF_DISP_SetFormat(HI_UNF_DISPLAY0, HI_UNF_ENC_FMT_NTSC);
        if (HI_SUCCESS != Ret)
        {
            sample_common_printf("call HI_UNF_DISP_SetFormat failed, Ret=%#x.\n", Ret);
            HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
            HI_UNF_DISP_DeInit();
            return Ret;
        }
    }

    if ((HI_UNF_ENC_FMT_1080P_50 == enFormat)
        ||(HI_UNF_ENC_FMT_1080i_50 == enFormat)
        ||(HI_UNF_ENC_FMT_720P_50 == enFormat)
        ||(HI_UNF_ENC_FMT_576P_50 == enFormat)
        ||(HI_UNF_ENC_FMT_PAL == enFormat)
        ||(HI_UNF_ENC_FMT_3840X2160_25 == enFormat))
    {
        Ret = HI_UNF_DISP_SetFormat(HI_UNF_DISPLAY0, HI_UNF_ENC_FMT_PAL);
        if (HI_SUCCESS != Ret)
        {
            sample_common_printf("call HI_UNF_DISP_SetFormat failed, Ret=%#x.\n", Ret);
            HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
            HI_UNF_DISP_DeInit();
            return Ret;
        }
    }

#ifndef ANDROID
#if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    Ret = HI_UNF_DISP_SetVirtualScreen(HI_UNF_DISPLAY1, 1280, 720);
#else
    Ret = HI_UNF_DISP_SetVirtualScreen(HI_UNF_DISPLAY1, 1920, 1080);
#endif
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DISP_SetVirtualScreen failed, Ret=%#x.\n", Ret);
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
        sample_common_printf("call HI_UNF_DISP_SetBgColor failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    /*set display0 screen offset*/
    Ret = HI_UNF_DISP_SetScreenOffset(HI_UNF_DISPLAY0, &offset);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DISP_SetBgColor failed, Ret=%#x.\n", Ret);
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
        sample_common_printf("call HI_UNF_DISP_SetBgColor failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HI_UNF_DISP_Open(HI_UNF_DISPLAY1);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DISP_Open DISPLAY1 failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HI_UNF_DISP_Open(HI_UNF_DISPLAY0);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DISP_Open DISPLAY0 failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HIADP_HDMI_Init(HI_UNF_HDMI_ID_0);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("call HIADP_HDMI_Init failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Close(HI_UNF_DISPLAY0);
        HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

#ifdef ANDROID
    HIADP_SURFACE_ATTR_S    stSurAttr;
    HI_UNF_PDM_DISP_PARAM_S stDispParam;

    HIADP_OSD_Init();

    Ret = HI_UNF_PDM_GetBaseParam(HI_UNF_PDM_BASEPARAM_DISP0, &stDispParam);
    if (HI_SUCCESS != Ret)
    {
        stSurAttr.u32Width = 1920;
        stSurAttr.u32Height = 1080;
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

HI_VOID PVR_InitInfoRect(PVR_UI_DEFINITION enPvrUiDefinition)
{
    HI_U32 i = 0;
    HI_U32 u32InitX = 0;
    HI_U32 u32InitY = 0;
    HI_U32 u32YDistence = 0;
    HI_U32 u32ChnDistenc = 0;

    HI_U32 u32RectHeight = 0;
    HI_U32 u32WidthCnt = 0;
    HI_U32 u32DefinitionSelect = 0;
    HI_U32 aPlayRectWidth[2][12] = {
                                    {153, 400, 67, 120, 200,
                                     120, 120, 133,
                                     267, 267,
                                     267, 267},
                                    {230, 600, 100, 180, 300,
                                     180, 180, 200,
                                     400, 400,
                                     400, 400}
                                   };
    HI_U32 aPlayRectDeltaX[2][12] = {
                                    {0, 153, 33, 153, 300,
                                     33, 153, 300,
                                     33, 300,
                                     33, 300},
                                    {0, 230, 50, 230, 450,
                                     50, 230, 450,
                                     50, 450,
                                     50, 450}
                                    };

    HI_U32 aRecRectWidth[2][11] = {
                                    {133, 400, 100, 100, 100, 87, 100,
                                     333, 200,
                                     333, 200},
                                    {200, 600, 150, 150, 150, 130, 150,
                                     500, 300,
                                     500, 300}
                                  };
    HI_U32 aRecRectDeltaX[2][11] = {
                                    {0, 133, 33, 133, 267, 367, 453,
                                     33, 367,
                                     33, 367},
                                    {0, 200, 50, 200, 400, 550, 680,
                                     50, 550,
                                     50, 550}
                                    };
    HI_U32 aRecYDistenc[2] = {23, 35};

    HI_RECT stRectTmp;

    switch(enPvrUiDefinition)
    {
        case PVR_UI_DEFINITION_720P:
            u32InitX = 30;
            u32InitY = 30;
            u32YDistence = 27;
            u32ChnDistenc = 140;



            u32RectHeight = 27;
            u32DefinitionSelect = 0;
            break;
        case PVR_UI_DEFINITION_1080P:
            u32InitX = 100;
            u32InitY = 100;
            u32YDistence = 40;
            u32ChnDistenc = 210;

            u32RectHeight = 40;
            u32DefinitionSelect = 1;
            break;
        default:
            sample_common_printf("Unknown UI definition %d.\n", enPvrUiDefinition);
            return;
    };



    /*播放坐标初始化*/
    for(i = 0; i < PVR_MAX_PLAY_CHN; i++)
    {
        u32YDistence = u32RectHeight;

        u32WidthCnt = 0;

        stRectTmp.x = u32InitX + aPlayRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY + i*u32ChnDistenc;
        stRectTmp.w = aPlayRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stPlayInfoRect[i].stPlayChnName, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aPlayRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY + i*u32ChnDistenc;
        stRectTmp.w = aPlayRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stPlayInfoRect[i].stStreamInfoRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aPlayRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY + i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aPlayRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stPlayInfoRect[i].stPlaySpeedRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aPlayRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY + i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aPlayRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stPlayInfoRect[i].stPlayFps, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aPlayRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY + i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aPlayRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stPlayInfoRect[i].stPlayTimeProgress, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        u32YDistence += u32RectHeight;


        stRectTmp.x = u32InitX + aPlayRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY + i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aPlayRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stPlayInfoRect[i].stReadSpeedRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aPlayRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY + i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aPlayRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stPlayInfoRect[i].stFilePosRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aPlayRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY + i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aPlayRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stPlayInfoRect[i].stFrmPosRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        u32YDistence += u32RectHeight;


        stRectTmp.x = u32InitX + aPlayRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY + i*u32ChnDistenc+u32YDistence+10;
        stRectTmp.w = aPlayRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight/2;
        memcpy(&g_stPlayInfoRect[i].stPlayProgressBarRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aPlayRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY + i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aPlayRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stPlayInfoRect[i].stPlayFileNameRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        u32YDistence += u32RectHeight;


        stRectTmp.x = u32InitX + aPlayRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY + i*u32ChnDistenc+u32YDistence+10;
        stRectTmp.w = aPlayRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight/2;
        memcpy(&g_stPlayInfoRect[i].stPlayDmxBuffBarRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aPlayRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY + i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aPlayRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stPlayInfoRect[i].stPlayDmxNameRect, &stRectTmp, sizeof(HI_RECT));

    }


    u32WidthCnt = 0;

    switch(enPvrUiDefinition)
    {
        case PVR_UI_DEFINITION_720P:
            u32InitX = 640;
            u32InitY = 30;
            u32YDistence = 23;
            u32ChnDistenc = 107;



            u32RectHeight = 27;
            u32DefinitionSelect = 0;
            break;
        case PVR_UI_DEFINITION_1080P:
            u32InitX = 960;
            u32InitY = 100;
            u32YDistence = 35;
            u32ChnDistenc = 160;

            u32RectHeight = 40;
            u32DefinitionSelect = 1;
            break;
        default:
            sample_common_printf("Unknown UI definition %d.\n", enPvrUiDefinition);
            return;
    };

    for(i = 0; i < PVR_MAX_REC_CHN; i++)
    {
        u32YDistence = aRecYDistenc[u32DefinitionSelect];
        u32WidthCnt = 0;

        stRectTmp.x = u32InitX + aRecRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY+i*u32ChnDistenc;
        stRectTmp.w = aRecRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stRecInfoRect[i].stRecChnName, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aRecRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY+i*u32ChnDistenc;
        stRectTmp.w = aRecRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stRecInfoRect[i].stValidInfoRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aRecRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY+i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aRecRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stRecInfoRect[i].stWriteSpeedRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aRecRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY+i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aRecRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stRecInfoRect[i].stSizePosRect, &stRectTmp, sizeof(HI_RECT));
        memcpy(&g_stRecInfoRect[i].stTimePosRect, &stRectTmp, sizeof(HI_RECT));
        memcpy(&g_stRecInfoRect[i].stRecFileSizeRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aRecRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY+i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aRecRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stRecInfoRect[i].stReWindType, &stRectTmp, sizeof(HI_RECT));
        memcpy(&g_stRecInfoRect[i].stRecFileTimeRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;


        stRectTmp.x = u32InitX + aRecRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY+i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aRecRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stRecInfoRect[i].stRecType, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;


        stRectTmp.x = u32InitX + aRecRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY+i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aRecRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stRecInfoRect[i].stEncryptRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        u32YDistence += aRecYDistenc[u32DefinitionSelect];


        stRectTmp.x = u32InitX + aRecRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY+i*u32ChnDistenc+u32YDistence+10;
        stRectTmp.w = aRecRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight/2;
        memcpy(&g_stRecInfoRect[i].stRecProgressBarRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aRecRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY+i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aRecRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stRecInfoRect[i].stRecFileNameRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        u32YDistence += aRecYDistenc[u32DefinitionSelect];


        stRectTmp.x = u32InitX + aRecRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY+i*u32ChnDistenc+u32YDistence+10;
        stRectTmp.w = aRecRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight/2;
        memcpy(&g_stRecInfoRect[i].stRecTsBufBarRect, &stRectTmp, sizeof(HI_RECT));
        u32WidthCnt++;

        stRectTmp.x = u32InitX + aRecRectDeltaX[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.y = u32InitY+i*u32ChnDistenc+u32YDistence;
        stRectTmp.w = aRecRectWidth[u32DefinitionSelect][u32WidthCnt];
        stRectTmp.h = u32RectHeight;
        memcpy(&g_stRecInfoRect[i].stDmxFileNameRect, &stRectTmp, sizeof(HI_RECT));
    }
}

HI_S32 PVR_DrawString(HI_CHAR *szText, HI_RECT *pstRect, HI_COLOR u32Color)
{
#ifndef ANDROID
    HI_S32 Ret;
    if ((HI_NULL == pstRect)
        || (0 == pstRect->w)
        || (0 == pstRect->h))
    {
        return 0;
    }
    HI_GO_FillRect(hLayerSurface_pvr, pstRect, 0x00000000, HIGO_COMPOPT_NONE);
    //HI_GO_DrawRect(hLayerSurface_pvr, pstRect, 0xffffffff);

    HI_GO_SetTextColor(hFont_pvr, u32Color);

    Ret = HI_GO_TextOutEx(hFont_pvr, hLayerSurface_pvr, szText, pstRect, HIGO_LAYOUT_LEFT);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_GO_TextOut faild!\n");
    }

    /*Ret = HI_GO_RefreshLayer(hLayer_pvr, NULL);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_GO_RefreshLayer faild!\n");
    }*/

#else
    sample_common_printf("STATUS:%s\n", szText);
#endif
    return 0;
}

HI_S32 PVR_DrawGraphic(const HI_CHAR *pszFileName, HI_RECT_S *pstRect)
{
    HI_S32 ret;
    HI_HANDLE hDecoder;
    HI_HANDLE hDecSurface;
    HIGO_DEC_ATTR_S stSrcDesc;
    HIGO_DEC_IMGINFO_S stImgInfo;
    HIGO_DEC_IMGATTR_S stAttr;
    HIGO_BLTOPT_S stBltOpt = {0};
    HI_RECT stRectTmp;

    if ((HI_NULL == pszFileName)
        || (HI_NULL == pstRect)
        || (0 == pstRect->s32Width)
        || (0 == pstRect->s32Height))
    {
        return HI_SUCCESS;
    }
    /** create decode*/
    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = pszFileName;
    ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
    if (HI_SUCCESS != ret)
    {
        sample_common_printf("HI_GO_CreateDecoder fail! ret=%#x\n", ret);
        return ret;
    }

    ret = HI_GO_DecImgInfo(hDecoder, 0, &stImgInfo);
    if (HI_SUCCESS != ret)
    {
        HI_GO_DestroyDecoder(hDecoder);

        sample_common_printf("HI_GO_DecImgInfo fail! ret=%#x\n", ret);
        return ret;
    }

    /** decode it to Surface */
    stAttr.Width = pstRect->s32Width;
    stAttr.Height = pstRect->s32Height;
    stAttr.Format = HIGO_PF_8888;

    ret = HI_GO_DecImgData(hDecoder, 0, &stAttr, &hDecSurface);
    if (HI_SUCCESS != ret)
    {
        HI_GO_DestroyDecoder(hDecoder);

        sample_common_printf("HI_GO_DecImgData fail! ret=%#x\n", ret);
        return ret;
    }

    ret = HI_GO_DestroyDecoder(hDecoder);
    if (HI_SUCCESS != ret)
    {
        sample_common_printf("HI_GO_DestroyDecoder fail! ret=%#x\n", ret);
        return ret;
    }

    /** Blit it to graphic layer Surface */
    stBltOpt.EnableScale = HI_TRUE;

    stRectTmp.x = pstRect->s32X;
    stRectTmp.y = pstRect->s32Y;
    stRectTmp.w = pstRect->s32Width;
    stRectTmp.h = pstRect->s32Height;

    ret = HI_GO_Blit(hDecSurface, NULL, hLayerSurface_pvr, &stRectTmp, &stBltOpt);
    if (HI_SUCCESS == ret)
    {
        /** fresh display*/
        HI_GO_RefreshLayer(hLayer_pvr, NULL);
    }
    else
    {
        sample_common_printf("HI_GO_Blit fail! ret=%#x\n", ret);
        return ret;
    }

    /**  free Surface */
    ret = HI_GO_FreeSurface(hDecSurface);
    if (HI_SUCCESS != ret)
    {
        sample_common_printf("HI_GO_FreeSurface fail! ret=%#x\n", ret);
        return ret;
    }

    return HI_SUCCESS;
}


HI_S32 PVR_DrawSelectRect(HI_RECT_S *pstRect, HI_BOOL bVisible, HI_BOOL bVedioExist)
{
    HI_RECT stWindowRect;

    stWindowRect.w = pstRect->s32Width;
    stWindowRect.h = pstRect->s32Height+20;
    stWindowRect.x = pstRect->s32X;
    stWindowRect.y = pstRect->s32Y-10;

    HI_GO_FillRect(hLayerSurface_pvr, &stWindowRect, 0, HIGO_COMPOPT_NONE);
    if (HI_TRUE == bVisible)
    {
        HI_GO_DrawRect(hLayerSurface_pvr, &stWindowRect, PVR_INFO_COLOR);
    }
    else
    {
        HI_GO_DrawRect(hLayerSurface_pvr, &stWindowRect, 0);
    }

    if (HI_FALSE == bVedioExist)
    {
        PVR_DrawGraphic(g_aHisiliconLogoPath, pstRect);
    }

    HI_GO_RefreshLayer(hLayer_pvr, NULL);

    return HI_SUCCESS;
}

HI_S32 PVR_DrawMuteText(HI_RECT *pstRect, HI_BOOL bMuted)
{
    HI_CHAR cStringTmp[PVR_MAX_INFO_SIZE] = {0};

    if((HI_TRUE != bMuted) && (HI_FALSE != bMuted))
    {
        return HI_SUCCESS;
    }

    memset(cStringTmp, 0, sizeof(cStringTmp));
    if (HI_FALSE == bMuted)
    {
        snprintf(cStringTmp, PVR_MAX_INFO_SIZE, " ");
    }
    else
    {
        snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "mute");
    }

    PVR_DrawString(cStringTmp, pstRect, PVR_ALERT_COLOR);

    HI_GO_RefreshLayer(hLayer_pvr, NULL);

    return HI_SUCCESS;
}

HI_S32 PVR_DrawProgressBar(HI_RECT *pstBarRect, HI_RECT *pstRealBarRect, HI_COLOR u32Color)
{
    HI_GO_FillRect(hLayerSurface_pvr, pstBarRect, 0x00000000, HIGO_COMPOPT_NONE);
    HI_GO_FillRect(hLayerSurface_pvr, pstRealBarRect, u32Color, HIGO_COMPOPT_NONE);
    HI_GO_DrawRect(hLayerSurface_pvr, pstBarRect, PVR_INFO_COLOR);

    //HI_GO_RefreshLayer(hLayer_pvr, NULL);

    return HI_SUCCESS;
}

HI_S32 PVR_DrawPlayChnInitRect(HI_U32 u32ChnNum)
{
    HI_CHAR cStringTmp[PVR_MAX_INFO_SIZE] = {0};

    (void)pthread_mutex_lock(&g_stDrawRectMutex);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Play Channel");
    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[u32ChnNum].stPlayChnName, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "              ");
    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[u32ChnNum].stStreamInfoRect, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%dX", 0);
    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[u32ChnNum].stPlaySpeedRect, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%dfps",0);
    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[u32ChnNum].stPlayFps, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%02d:%02d:%02d/%02d:%02d:%02d", 0,0,0,0,0,0);
    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[u32ChnNum].stPlayTimeProgress, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%d Mbps", 0,0);
    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[u32ChnNum].stReadSpeedRect, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d/%d MB", 0,0);
    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[u32ChnNum].stFilePosRect, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d/%d", 0,0);
    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[u32ChnNum].stFrmPosRect, PVR_INFO_COLOR);

    HI_GO_FillRect(hLayerSurface_pvr, &g_stPlayInfoRect[u32ChnNum].stPlayProgressBarRect, 0x00000000, HIGO_COMPOPT_NONE);
    HI_GO_DrawRect(hLayerSurface_pvr, &g_stPlayInfoRect[u32ChnNum].stPlayProgressBarRect, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", "NULL");
    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[u32ChnNum].stPlayFileNameRect, PVR_INFO_COLOR);


    HI_GO_FillRect(hLayerSurface_pvr, &g_stPlayInfoRect[u32ChnNum].stPlayDmxBuffBarRect, 0x00000000, HIGO_COMPOPT_NONE);
    HI_GO_DrawRect(hLayerSurface_pvr, &g_stPlayInfoRect[u32ChnNum].stPlayDmxBuffBarRect, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", "NULL");
    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[u32ChnNum].stPlayDmxNameRect, PVR_INFO_COLOR);

    HI_GO_RefreshLayer(hLayer_pvr, NULL);

    (void)pthread_mutex_unlock(&g_stDrawRectMutex);
    return 0;
}

HI_S32 PVR_DrawRecChnInitRect(HI_U32 u32ChnNum)
{
    HI_CHAR cStringTmp[PVR_MAX_INFO_SIZE] = {0};

    /*Rec Channel 1:
                10.123 Mbps   12 /1024 (MB)     Rewind          By Size
                        或              或               或
                140 / 600 (s)      NO Rewind     By Time
                                            或
                120 (MB)            140 (s)          No Limit
                |||||||||||||||||||||||||||||||||||||||||||   xxx.ts*/
    (void)pthread_mutex_lock(&g_stDrawRectMutex);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Rec Channel");
    PVR_DrawString(cStringTmp, &g_stRecInfoRect[u32ChnNum].stRecChnName, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%d Mbps", 0,0);
    PVR_DrawString(cStringTmp, &g_stRecInfoRect[u32ChnNum].stWriteSpeedRect, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d MB", 0);
    PVR_DrawString(cStringTmp, &g_stRecInfoRect[u32ChnNum].stRecFileSizeRect, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d s", 0);
    PVR_DrawString(cStringTmp, &g_stRecInfoRect[u32ChnNum].stRecFileTimeRect, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", "NULL");
    PVR_DrawString(cStringTmp, &g_stRecInfoRect[u32ChnNum].stRecType, PVR_INFO_COLOR);


    HI_GO_FillRect(hLayerSurface_pvr, &g_stRecInfoRect[u32ChnNum].stRecProgressBarRect, 0x00000000, HIGO_COMPOPT_NONE);
    HI_GO_DrawRect(hLayerSurface_pvr, &g_stRecInfoRect[u32ChnNum].stRecProgressBarRect, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", "NULL");
    PVR_DrawString(cStringTmp, &g_stRecInfoRect[u32ChnNum].stRecFileNameRect, PVR_INFO_COLOR);

    HI_GO_FillRect(hLayerSurface_pvr, &g_stRecInfoRect[u32ChnNum].stRecTsBufBarRect, 0x00000000, HIGO_COMPOPT_NONE);
    HI_GO_DrawRect(hLayerSurface_pvr, &g_stRecInfoRect[u32ChnNum].stRecTsBufBarRect, PVR_INFO_COLOR);

    memset(cStringTmp, 0, sizeof(cStringTmp));
    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", "NULL");
    PVR_DrawString(cStringTmp, &g_stRecInfoRect[u32ChnNum].stDmxFileNameRect, PVR_INFO_COLOR);

    HI_GO_RefreshLayer(hLayer_pvr, NULL);

    (void)pthread_mutex_unlock(&g_stDrawRectMutex);
    return 0;
}

HI_S32 PVR_DrawInitInfoRect(HI_VOID)
{
    HI_U32 i = 0;

    for(i = 0; i < PVR_MAX_PLAY_CHN; i++)
    {
        PVR_DrawPlayChnInitRect(i);
    }

    for(i = 0; i < PVR_MAX_REC_CHN; i++)
    {
        PVR_DrawRecChnInitRect(i);
    }

    return 0;
}

HI_S32 PVR_InitOsd(HI_CHAR *pu8CurrentPath)
{
#ifndef ANDROID
    HI_S32   s32Ret;
    HIGO_LAYER_INFO_S stLayerInfo = {0};
    HIGO_TEXT_INFO_S stTextInfo;

#if defined(HI_ADVCA_FUNCTION_RELEASE)||\
    defined(HI_ADVCA_FUNCTION_DEBUG) ||\
    defined(HI_ADVCA_FUNCTION_FINAL)
#else
    system("rmmod hi_fb");
    system("insmod /kmod/hi_fb.ko  video=\"hi_fb:vram0_size:16200,vram4_size:2430\"");
#endif

    s32Ret = HI_GO_Init();
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("failed to init higo! ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }

    HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);
    stLayerInfo.PixelFormat  = HIGO_PF_1555;
    stLayerInfo.ScreenWidth  = 1280;
    stLayerInfo.ScreenHeight = 720;
#if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    stLayerInfo.CanvasWidth = 1280;
    stLayerInfo.CanvasHeight = 720;
    stLayerInfo.DisplayWidth  = 1280;
    stLayerInfo.DisplayHeight = 720;
#else
    stLayerInfo.CanvasWidth = 1920;
    stLayerInfo.CanvasHeight = 1080;
    stLayerInfo.DisplayWidth  = 1920;
    stLayerInfo.DisplayHeight = 1080;
#endif
    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_OVER;

    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer_pvr);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("failed to create the layer HD0, ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret =  HI_GO_GetLayerSurface(hLayer_pvr, &hLayerSurface_pvr);
    if  (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("failed to get layer surface! s32Ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    HI_GO_FillRect(hLayerSurface_pvr, NULL, 0x00000000, HIGO_COMPOPT_NONE);

    stTextInfo.pMbcFontFile = NULL;
    pu8CurrentPath = strncat(pu8CurrentPath, "../res/font/DroidSansFallbackLegacy.ttf", sizeof("../res/font/DroidSansFallbackLegacy.ttf"));
    stTextInfo.pSbcFontFile = pu8CurrentPath;
#if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    stTextInfo.u32Size = 17;
#else
    stTextInfo.u32Size = 25;
#endif

    s32Ret = HI_GO_CreateTextEx(&stTextInfo,  &hFont_pvr);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("failed to create the font:%s !\n", pu8CurrentPath);
        return HI_FAILURE;
    }
    HI_GO_RefreshLayer(hLayer_pvr, NULL);

    /*初始化屏幕坐标*/
#if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    PVR_InitInfoRect(PVR_UI_DEFINITION_720P);
#else
    PVR_InitInfoRect(PVR_UI_DEFINITION_1080P);
#endif

    PVR_DrawInitInfoRect();
#endif
    return HI_SUCCESS;
}

HI_S32 PVR_InitIR(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_UNF_IR_Init();
    if (s32Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_IR_Init failed.\n");
        return s32Ret;
    }

s32Ret = HI_UNF_IR_EnableKeyUp(HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        (HI_VOID)HI_UNF_IR_DeInit();
        return s32Ret;
    }

    s32Ret = HI_UNF_IR_SetRepKeyTimeoutAttr(300);   //???  108< parament   <65536
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        (HI_VOID)HI_UNF_IR_DeInit();
        return s32Ret;
    }

    s32Ret = HI_UNF_IR_EnableRepKey(HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        (HI_VOID)HI_UNF_IR_DeInit();
        return s32Ret;
    }

    s32Ret = HI_UNF_IR_Enable(HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        (HI_VOID)HI_UNF_IR_DeInit();
        return s32Ret;
    }

    return s32Ret;
}


HI_VOID *PVR_EthSocketRecvThread(HI_VOID *args)
{
    HI_S32              SocketFd;
    HI_U32              AddrLen;

    HI_UNF_STREAM_BUF_S     StreamBuf;
    HI_U32              ReadLen;
    HI_U32              GetBufCount=0;
    HI_U32              ReceiveCount=0;
    HI_S32              Ret;

    SocketFd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (SocketFd < 0)
    {
        sample_common_printf("create socket error [%d].\n", errno);
        return HI_NULL;
    }

    if (bind(SocketFd,(struct sockaddr *)(&g_stServerAddr),sizeof(struct sockaddr_in)) < 0)
    {
        sample_common_printf("socket bind error [%d].\n", errno);
        close(SocketFd);
        return HI_NULL;
    }

    if (setsockopt(SocketFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &g_stIpMreq, sizeof(struct ip_mreq)))
    {
        sample_common_printf("Socket setsockopt ADD_MEMBERSHIP error [%d].\n", errno);
        close(SocketFd);
        return HI_NULL;
    }

    AddrLen = sizeof(g_stServerAddr);

    while (HI_FALSE == g_bIsEthSocketRecvThreadStop)
    {
        Ret = HI_UNF_DMX_GetTSBuffer(g_hEthSocketRecvTsBuff, 0xbc000, &StreamBuf, 0);
        if (Ret != HI_SUCCESS)
        {
            GetBufCount++;
            if(GetBufCount >= 10)
            {
                sample_common_printf("########## TS come too fast! #########, Ret=%d\n", Ret);
                GetBufCount=0;
            }

            usleep(1000);
            continue;
        }
        GetBufCount=0;

        ReadLen = recvfrom(SocketFd, StreamBuf.pu8Data, StreamBuf.u32Size, 0,
                           (struct sockaddr *)&g_stServerAddr, &AddrLen);
        if (ReadLen <= 0)
        {
            ReceiveCount++;
            if (ReceiveCount >= 50)
            {
                sample_common_printf("########## TS come too slow or net error! #########\n");
                ReceiveCount = 0;
            }
        }
        else
        {
            ReceiveCount = 0;
            Ret = HI_UNF_DMX_PutTSBuffer(g_hEthSocketRecvTsBuff, ReadLen);
            if (Ret != HI_SUCCESS )
            {
                sample_common_printf("call HI_UNF_DMX_PutTSBuffer failed.\n");
            }
        }
    }

    close(SocketFd);
    return HI_NULL;
}

HI_VOID *PVR_UpdatePlayStatusThread(HI_VOID *args)
{
    HI_S32 s32Ret;
    HI_U32 i = 0;
    HI_U32 u32SleepTime = 250000;
    static HI_U32 u32LastSystemTimeMs[PVR_MAX_PLAY_CHN] = {0};
    HI_U32 u32CurSystemTimeMs = 0;
    HI_U32 u32DeltaTime = 0;
    HI_U32 u32PlayedFrmCnt[PVR_MAX_PLAY_CHN] = {0};
    HI_U64 u64PlayedSizeCnt[PVR_MAX_PLAY_CHN]  = {0};
    HI_CHAR cStringTmp[PVR_MAX_INFO_SIZE] = {0};
    HI_RECT stRectTmp[PVR_MAX_PLAY_CHN];
    HI_UNF_PVR_PLAY_STATUS_S stPvrPlayStatus;
    HI_UNF_PVR_FILE_ATTR_S stPvrPlayFileAttr;
    HI_UNF_PVR_PLAY_ATTR_S stPvrPlayAttr;
    HI_UNF_AVPLAY_STATUS_INFO_S stAvplayStatus;
    HI_UNF_AVPLAY_STREAM_INFO_S stAvplayStrmInfo;
    HI_UNF_ACODEC_ATTR_S stAcodecAttr;
    HI_UNF_VCODEC_ATTR_S stVcodecAttr;
    HI_UNF_DMX_TSBUF_STATUS_S stDmxTsBuffStatus;


    while (HI_FALSE == g_bIsMainRoutineStop)
    {
        usleep(u32SleepTime);

        (void)pthread_mutex_lock(&g_stDrawRectMutex);

        for(i = 0; i < PVR_MAX_PLAY_CHN; i++)
        {
            if (HI_TRUE != g_stPlayChnAttr[i].bPlayChnInited)
            {
                continue;
            }
            s32Ret = HI_UNF_PVR_PlayGetChn(g_stPlayChnAttr[i].u32PvrPlayChn, &stPvrPlayAttr);
            s32Ret = HI_UNF_DMX_GetTSBufferStatus(g_stPlayChnAttr[i].hTsBufForPlayback, &stDmxTsBuffStatus);
            s32Ret |= HI_UNF_AVPLAY_GetStatusInfo(g_stPlayChnAttr[i].hAvplay, &stAvplayStatus);
            s32Ret |= HI_UNF_PVR_GetFileAttrByFileName(stPvrPlayAttr.szFileName, &stPvrPlayFileAttr);
            s32Ret |= HI_UNF_PVR_PlayGetStatus(g_stPlayChnAttr[i].u32PvrPlayChn, &stPvrPlayStatus);
            s32Ret |= HI_UNF_AVPLAY_GetStreamInfo(g_stPlayChnAttr[i].hAvplay, &stAvplayStrmInfo);
            s32Ret |= HI_UNF_AVPLAY_GetAttr(g_stPlayChnAttr[i].hAvplay, HI_UNF_AVPLAY_ATTR_ID_ADEC, &stAcodecAttr);
            s32Ret |= HI_UNF_AVPLAY_GetAttr(g_stPlayChnAttr[i].hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVcodecAttr);
            if(HI_SUCCESS != s32Ret)
            {
                continue;
            }

            HI_SYS_GetTimeStampMs(&u32CurSystemTimeMs);
            u32DeltaTime = u32CurSystemTimeMs - u32LastSystemTimeMs[i];
            u32LastSystemTimeMs[i] = u32CurSystemTimeMs;

            /*Update play chan NO.*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Play Channel %d:", i);
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stPlayChnName, PVR_INFO_COLOR);

            /*Update Stream info.*/
            memset(cStringTmp, 0, sizeof(cStringTmp));

            if(HI_TRUE == g_stPlayChnAttr[i].bVideoExist)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s\t\t%dx%d\t\t%s\t\t%s",
                          g_s8VideoType[stVcodecAttr.enType],
                          stAvplayStrmInfo.stVidStreamInfo.u32Width,
                          stAvplayStrmInfo.stVidStreamInfo.u32Height,
                          g_s8AudioType[0xff & HA_GET_ID(stAcodecAttr.enType)],
                          (HI_TRUE == stPvrPlayAttr.stDecryptCfg.bDoCipher) ? "Encrypt" : "Uncrypt");
            }
            else
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s\t\t%dx%d\t\t%s\t\t%s",
                          "UNKONOW",
                          0,
                          0,
                          g_s8AudioType[0xff & HA_GET_ID(stAcodecAttr.enType)],
                          (HI_TRUE == stPvrPlayAttr.stDecryptCfg.bDoCipher) ? "Encrypt" : "Uncrypt");
            }

            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stStreamInfoRect, PVR_ALERT_COLOR);

            /*Update play speed*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            if (HI_UNF_PVR_PLAY_STATE_PAUSE == stPvrPlayStatus.enState)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Pause");
            }
            else if (HI_UNF_PVR_PLAY_STATE_STEPF == stPvrPlayStatus.enState)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Step F");
            }
            else if (HI_UNF_PVR_PLAY_STATE_STEPB == stPvrPlayStatus.enState)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Step B");
            }
            else if (0 > stPvrPlayStatus.enSpeed)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "-%dX", (stPvrPlayStatus.enSpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL));
            }
            else if (HI_UNF_PVR_PLAY_SPEED_NORMAL > stPvrPlayStatus.enSpeed)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "1/%dX", (HI_UNF_PVR_PLAY_SPEED_NORMAL / stPvrPlayStatus.enSpeed));
            }
            else
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%dX", (stPvrPlayStatus.enSpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL));
            }

            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stPlaySpeedRect, PVR_INFO_COLOR);

            /*Update play fps*/
            if (stAvplayStatus.u32VidFrameCount < u32PlayedFrmCnt[i])
            {
                u32PlayedFrmCnt[i] = stAvplayStatus.u32VidFrameCount;
            }
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%01dfps",
                      (1000*(stAvplayStatus.u32VidFrameCount - u32PlayedFrmCnt[i])/u32DeltaTime),
                      (1000*(stAvplayStatus.u32VidFrameCount - u32PlayedFrmCnt[i])%u32DeltaTime)/100);
            u32PlayedFrmCnt[i] = stAvplayStatus.u32VidFrameCount;
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stPlayFps, PVR_INFO_COLOR);

            /*Update play time progress*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%02d:%02d:%02d/%02d:%02d:%02d",
                      stPvrPlayStatus.u32CurPlayTimeInMs/3600000,
                      (stPvrPlayStatus.u32CurPlayTimeInMs%3600000)/60000,
                      ((stPvrPlayStatus.u32CurPlayTimeInMs%3600000)%60000)/1000,
                      stPvrPlayFileAttr.u32EndTimeInMs/3600000,
                      (stPvrPlayFileAttr.u32EndTimeInMs%3600000)/60000,
                      ((stPvrPlayFileAttr.u32EndTimeInMs%3600000)%60000)/1000);
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stPlayTimeProgress, PVR_INFO_COLOR);

            if(0 < (HI_S32)stPvrPlayStatus.enSpeed)
            {
                /*Update play read file speed*/
                /*avoid a very large value be calculating.
                         some stream's decoder order and display order is not the same,
                         so, the offset of a frame which displayed earlier may less than displayed later */
                if(stPvrPlayStatus.u64CurPlayPos >= u64PlayedSizeCnt[i])
                {
                    HI_U32 u32ReadSpeedInt = 0,u32ReadSpeedDec = 0;
                    memset(cStringTmp, 0, sizeof(cStringTmp));

                    u32ReadSpeedInt = (HI_U32)(1000*(8*(stPvrPlayStatus.u64CurPlayPos - u64PlayedSizeCnt[i]))/0x100000)/u32DeltaTime;

                    if (0 == u32ReadSpeedInt)
                    {
                        u32ReadSpeedInt = (HI_U32)(1000*(8*(stPvrPlayStatus.u64CurPlayPos - u64PlayedSizeCnt[i]))/0x400)/u32DeltaTime;
                        u32ReadSpeedDec = (HI_U32)(1000*(8*(stPvrPlayStatus.u64CurPlayPos - u64PlayedSizeCnt[i]))/0x400)%u32DeltaTime;
                        snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%01d Kbps", u32ReadSpeedInt, u32ReadSpeedDec/100);
                    }
                    else
                    {
                        u32ReadSpeedDec = (HI_U32)((1000*(8*(stPvrPlayStatus.u64CurPlayPos - u64PlayedSizeCnt[i]))/0x100000)%u32DeltaTime)/0x400;
                        snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%01d Mbps", u32ReadSpeedInt, u32ReadSpeedDec/100);
                    }

                    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stReadSpeedRect, PVR_INFO_COLOR);
                }
            }
            else
            {
                /*Update play read file speed*/
                /*avoid a very large value be calculating.
                         some stream's decoder order and display order is not the same,
                         so, the offset of a frame which displayed earlier may less than displayed later */
                if(stPvrPlayStatus.u64CurPlayPos <= u64PlayedSizeCnt[i])
                {
                    HI_U32 u32ReadSpeedInt = 0,u32ReadSpeedDec = 0;
                    memset(cStringTmp, 0, sizeof(cStringTmp));

                    u32ReadSpeedInt = (HI_U32)(1000*(8*(u64PlayedSizeCnt[i] - stPvrPlayStatus.u64CurPlayPos))/0x100000)/u32DeltaTime;

                    if (0 == u32ReadSpeedInt)
                    {
                        u32ReadSpeedInt = (HI_U32)(1000*(8*(u64PlayedSizeCnt[i] - stPvrPlayStatus.u64CurPlayPos))/0x400)/u32DeltaTime;
                        u32ReadSpeedDec = (HI_U32)(1000*(8*(u64PlayedSizeCnt[i] - stPvrPlayStatus.u64CurPlayPos))/0x400)%u32DeltaTime;
                        snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%01d Kbps", u32ReadSpeedInt, u32ReadSpeedDec/100);
                    }
                    else
                    {
                        u32ReadSpeedDec = (HI_U32)((1000*(8*(u64PlayedSizeCnt[i] - stPvrPlayStatus.u64CurPlayPos))/0x100000)%u32DeltaTime)/0x400;
                        snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%01d Mbps", u32ReadSpeedInt, u32ReadSpeedDec/100);
                    }

                    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stReadSpeedRect, PVR_INFO_COLOR);
                }
            }
            u64PlayedSizeCnt[i] = stPvrPlayStatus.u64CurPlayPos;

            /*Update play file progress*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            if (0 == stPvrPlayFileAttr.u64ValidSizeInByte/0x100000)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%lld.%01lld/%lld.%01lld KB",
                          stPvrPlayStatus.u64CurPlayPos/0x400,
                          (stPvrPlayStatus.u64CurPlayPos%0x400)/100,
                          stPvrPlayFileAttr.u64ValidSizeInByte/0x400,
                          (stPvrPlayFileAttr.u64ValidSizeInByte%0x400)/100);
            }
            else
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%lld.%01lld/%lld.%01lld MB",
                          stPvrPlayStatus.u64CurPlayPos/0x100000,
                          (stPvrPlayStatus.u64CurPlayPos%0x100000)/0x400/100,
                          stPvrPlayFileAttr.u64ValidSizeInByte/0x100000,
                          (stPvrPlayFileAttr.u64ValidSizeInByte%0x100000)/0x400/100);
            }
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stFilePosRect, PVR_INFO_COLOR);

            /*Update play frames*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d/%d", stPvrPlayStatus.u32CurPlayFrame, stPvrPlayFileAttr.u32FrameNum);
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stFrmPosRect, PVR_INFO_COLOR);

            /*Update play progress bar*/
            if (stPvrPlayStatus.u32CurPlayTimeInMs > stPvrPlayFileAttr.u32StartTimeInMs)
            {
                memcpy(&stRectTmp[i], &g_stPlayInfoRect[i].stPlayProgressBarRect, sizeof(HI_RECT));
                stRectTmp[i].w = g_stPlayInfoRect[i].stPlayProgressBarRect.w * (stPvrPlayStatus.u32CurPlayTimeInMs - stPvrPlayFileAttr.u32StartTimeInMs) / (stPvrPlayFileAttr.u32EndTimeInMs - stPvrPlayFileAttr.u32StartTimeInMs);
                PVR_DrawProgressBar(&g_stPlayInfoRect[i].stPlayProgressBarRect, &stRectTmp[i], PVR_PROGRESS_COLOR);
            }

            /*Update play File Name*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", stPvrPlayAttr.szFileName);
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stPlayFileNameRect, PVR_INFO_COLOR);

            /*Update Dmx TS buffer bar*/
            memcpy(&stRectTmp[i], &g_stPlayInfoRect[i].stPlayDmxBuffBarRect, sizeof(HI_RECT));
            stRectTmp[i].w = g_stPlayInfoRect[i].stPlayDmxBuffBarRect.w * stDmxTsBuffStatus.u32UsedSize/ stDmxTsBuffStatus.u32BufSize;
            PVR_DrawProgressBar(&g_stPlayInfoRect[i].stPlayDmxBuffBarRect, &stRectTmp[i], PVR_PROGRESS_COLOR);

            /*Update Dmx Ts buffer size*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Demux%d %d/%dKB",
                      g_stPlayChnAttr[i].u32DemuxId,
                      stDmxTsBuffStatus.u32UsedSize/0x400,
                      stDmxTsBuffStatus.u32BufSize/0x400);
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stPlayDmxNameRect, PVR_INFO_COLOR);
        }

        HI_GO_RefreshLayer(hLayer_pvr, HI_NULL);
        (void)pthread_mutex_unlock(&g_stDrawRectMutex);
    }

    return HI_NULL;
}

HI_VOID *PVR_UpdateTimeShiftStatusThread(HI_VOID *args)
{
    HI_S32 s32Ret;
    HI_U32 i = 0;
    HI_U32 u32SleepTime = 250000;

    static HI_U32 u32RecLastSystemTimeMs[PVR_MAX_REC_CHN] = {0};
    HI_U32 u32RecCurSystemTimeMs = 0;
    HI_U32 u32RecDeltaTime = 0;

    static HI_U32 u32PlayLastSystemTimeMs[PVR_MAX_PLAY_CHN] = {0};
    HI_U32 u32PlayCurSystemTimeMs = 0;
    HI_U32 u32PlayDeltaTime = 0;

    HI_U64 u64LastRecPos[PVR_MAX_REC_CHN] = {0};
    HI_U32 u32PlayedFrmCnt[PVR_MAX_PLAY_CHN] = {0};
    HI_U64 u64PlayedSizeCnt[PVR_MAX_PLAY_CHN]  = {0};
    HI_CHAR cStringTmp[PVR_MAX_INFO_SIZE] = {0};
    HI_RECT stRectTmp[PVR_MAX_PLAY_CHN];
    HI_UNF_PVR_REC_ATTR_S stPvrRecAttr;
    HI_UNF_PVR_REC_STATUS_S stPvrRecStatus;
    HI_UNF_PVR_FILE_ATTR_S stPvrFileAttr;

    HI_UNF_PVR_PLAY_STATUS_S stPvrPlayStatus;
    HI_UNF_PVR_FILE_ATTR_S stPvrPlayFileAttr;
    HI_UNF_PVR_PLAY_ATTR_S stPvrPlayAttr;
    HI_UNF_AVPLAY_STATUS_INFO_S stAvplayStatus;
    HI_UNF_DMX_TSBUF_STATUS_S stDmxTsBuffStatus;

    HI_UNF_AVPLAY_STREAM_INFO_S stAvplayStrmInfo;
    HI_UNF_ACODEC_ATTR_S stAcodecAttr;
    HI_UNF_VCODEC_ATTR_S stVcodecAttr;


    while (HI_FALSE == g_bIsMainRoutineStop)
    {
        usleep(u32SleepTime);

        (void)pthread_mutex_lock(&g_stDrawRectMutex);

        for(i = 0; i < PVR_MAX_REC_CHN; i++)
        {
            if (HI_TRUE != g_stTimeShiftChnAttr[i].bRecChnInited)
            {
                continue;
            }

            s32Ret = HI_UNF_PVR_RecGetChn(g_stTimeShiftChnAttr[i].u32PvrRecChn, &stPvrRecAttr);
            s32Ret |= HI_UNF_PVR_RecGetStatus(g_stTimeShiftChnAttr[i].u32PvrRecChn, &stPvrRecStatus);
            s32Ret |= HI_UNF_PVR_GetFileAttrByFileName(stPvrRecAttr.szFileName, &stPvrFileAttr);

            if (HI_SUCCESS != s32Ret)
            {
                continue;
            }

            HI_SYS_GetTimeStampMs(&u32RecCurSystemTimeMs);
            u32RecDeltaTime = u32RecCurSystemTimeMs - u32RecLastSystemTimeMs[i];
            u32RecLastSystemTimeMs[i] = u32RecCurSystemTimeMs;

            /*Update Rec chan NO.*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Rec Channel %d:", i);
            PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stRecChnName, PVR_INFO_COLOR);

            /*Update valid time and size and bitrate*/
            {
                HI_U32 u32ValidTime = 0;
                HI_U64 u64ValidSize = 0;

                u32ValidTime = stPvrRecStatus.u32EndTimeInMs - stPvrRecStatus.u32StartTimeInMs;
                u64ValidSize = stPvrFileAttr.u64ValidSizeInByte;

                memset(cStringTmp, 0, sizeof(cStringTmp));
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "VALID  %d.%01ds  %lld.%01lldMB  %lld.%01lldMbps",
                        u32ValidTime/1000,
                        (u32ValidTime%1000)/100,
                        u64ValidSize/0x100000,
                        (u64ValidSize%0x100000)/0x400/100,
                        (1000 * 8 * u64ValidSize / 0x100000) / ((0 == u32ValidTime)?1:u32ValidTime),
                        (1000 * (((8 * u64ValidSize) % 0x100000) /0x400) / ((0 == u32ValidTime)?1:u32ValidTime))/100);
                PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stValidInfoRect, PVR_ALERT_COLOR);
            }

            /*Update Rec write speed.*/
            if (stPvrRecStatus.u64CurWritePos > u64LastRecPos[i])
            {
                memset(cStringTmp, 0, sizeof(cStringTmp));
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%01d Mbps",
                          (HI_U32)(1000*(8*(stPvrRecStatus.u64CurWritePos - u64LastRecPos[i]))/0x100000)/u32RecDeltaTime,
                          (HI_U32)((1000*(8*(stPvrRecStatus.u64CurWritePos - u64LastRecPos[i]))/0x100000)%u32RecDeltaTime)/0x400/100);
                PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stWriteSpeedRect, PVR_INFO_COLOR);
            }
            u64LastRecPos[i] = stPvrRecStatus.u64CurWritePos;

            if((0 == stPvrRecAttr.u64MaxFileSize) && (0 == stPvrRecAttr.u64MaxTimeInMs))
            {
                memset(cStringTmp, 0, sizeof(cStringTmp));
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%lld.%01lldMB",
                          stPvrRecStatus.u64CurWritePos/0x100000,
                          (stPvrRecStatus.u64CurWritePos%0x100000)/0x400/100);
                PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stRecFileSizeRect, PVR_INFO_COLOR);

                memset(cStringTmp, 0, sizeof(cStringTmp));
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%01ds",
                          stPvrRecStatus.u32CurTimeInMs/1000,
                          (stPvrRecStatus.u32CurTimeInMs%1000)/100);
                PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stRecFileTimeRect, PVR_INFO_COLOR);

                memset(cStringTmp, 0, sizeof(cStringTmp));
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", "No Limit");
                PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stRecType, PVR_INFO_COLOR);
            }
            else
            {
                /*Update Recording by Time  info.*/
                if((0 == stPvrRecAttr.u64MaxFileSize) && (0 != stPvrRecAttr.u64MaxTimeInMs))
                {
                    memset(cStringTmp, 0, sizeof(cStringTmp));
                    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%01d/%lld.%01llds",
                              stPvrRecStatus.u32CurTimeInMs/1000,
                              (stPvrRecStatus.u32CurTimeInMs%1000)/100,
                              stPvrRecAttr.u64MaxTimeInMs/1000,
                              (stPvrRecAttr.u64MaxTimeInMs%1000)/100);
                    PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stTimePosRect, PVR_INFO_COLOR);

                    memset(cStringTmp, 0, sizeof(cStringTmp));
                    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", (HI_TRUE == stPvrRecAttr.bRewind) ? "Rewind" : "Not Rewind");
                    PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stReWindType, PVR_INFO_COLOR);

                    memset(cStringTmp, 0, sizeof(cStringTmp));
                    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", "By Time");
                    PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stRecType, PVR_INFO_COLOR);

                    /*Update Rec progress bar*/
                    memcpy(&stRectTmp[i], &g_stRecInfoRect[i].stRecProgressBarRect, sizeof(HI_RECT));
                    if (HI_TRUE == stPvrRecAttr.bRewind)
                    {
                        stRectTmp[i].w = g_stRecInfoRect[i].stRecProgressBarRect.w * (stPvrRecStatus.u32CurTimeInMs % stPvrRecAttr.u64MaxTimeInMs) / stPvrRecAttr.u64MaxTimeInMs;
                    }
                    else
                    {
                        stRectTmp[i].w = g_stRecInfoRect[i].stRecProgressBarRect.w * stPvrRecStatus.u32CurTimeInMs / stPvrRecAttr.u64MaxTimeInMs;
                    }

                    PVR_DrawProgressBar(&g_stRecInfoRect[i].stRecProgressBarRect, &stRectTmp[i], PVR_PROGRESS_COLOR);
                }

                /*Update Recording by Size info.*/
                if((0 != stPvrRecAttr.u64MaxFileSize) && (0 == stPvrRecAttr.u64MaxTimeInMs))
                {
                    memset(cStringTmp, 0, sizeof(cStringTmp));
                    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%lld.%01lld/%lld.%01lldMB",
                              stPvrRecStatus.u64CurWritePos/0x100000,
                              (stPvrRecStatus.u64CurWritePos%0x100000)/0x400/100,
                              stPvrRecAttr.u64MaxFileSize/0x100000,
                              (stPvrRecAttr.u64MaxFileSize%0x100000)/0x400/100);
                    PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stSizePosRect, PVR_INFO_COLOR);

                    memset(cStringTmp, 0, sizeof(cStringTmp));
                    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", (HI_TRUE == stPvrRecAttr.bRewind) ? "Rewind" : "Not Rewind");
                    PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stReWindType, PVR_INFO_COLOR);

                    memset(cStringTmp, 0, sizeof(cStringTmp));
                    snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", "By Size");
                    PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stRecType, PVR_INFO_COLOR);

                    /*Update Rec progress bar*/
                    memcpy(&stRectTmp[i], &g_stRecInfoRect[i].stRecProgressBarRect, sizeof(HI_RECT));
                    if (HI_TRUE == stPvrRecAttr.bRewind)
                    {
                        stRectTmp[i].w = g_stRecInfoRect[i].stRecProgressBarRect.w * (stPvrRecStatus.u64CurWritePos % stPvrRecAttr.u64MaxFileSize) / stPvrRecAttr.u64MaxFileSize;
                    }
                    else
                    {
                        stRectTmp[i].w = g_stRecInfoRect[i].stRecProgressBarRect.w * stPvrRecStatus.u64CurWritePos / stPvrRecAttr.u64MaxFileSize;
                    }

                    PVR_DrawProgressBar(&g_stRecInfoRect[i].stRecProgressBarRect, &stRectTmp[i], PVR_PROGRESS_COLOR);
                }
            }

            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", (HI_TRUE == stPvrRecAttr.stEncryptCfg.bDoCipher) ? "Encrypt" : "Uncrypt");
            PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stEncryptRect, PVR_ALERT_COLOR);

            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", stPvrRecAttr.szFileName);
            PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stRecFileNameRect, PVR_INFO_COLOR);


            /*Update Ts buf bar*/
            if (0 != stPvrRecStatus.stRecBufStatus.u32BufSize)
            {
                memcpy(&stRectTmp[i], &g_stRecInfoRect[i].stRecTsBufBarRect, sizeof(HI_RECT));
                stRectTmp[i].w = g_stRecInfoRect[i].stRecTsBufBarRect.w * stPvrRecStatus.stRecBufStatus.u32UsedSize / stPvrRecStatus.stRecBufStatus.u32BufSize;
                if (90 < ((stPvrRecStatus.stRecBufStatus.u32UsedSize * 100) / stPvrRecStatus.stRecBufStatus.u32BufSize))
                {
                    PVR_DrawProgressBar(&g_stRecInfoRect[i].stRecTsBufBarRect, &stRectTmp[i], PVR_ALERT_COLOR);
                }
                else
                {
                    PVR_DrawProgressBar(&g_stRecInfoRect[i].stRecTsBufBarRect, &stRectTmp[i], PVR_PROGRESS_COLOR);
                }
            }

            /*Update DMX name and size*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Demux%d %d/%dKB",
                      stPvrRecAttr.u32DemuxID,
                      stPvrRecStatus.stRecBufStatus.u32UsedSize/0x400,
                      stPvrRecStatus.stRecBufStatus.u32BufSize/0x400);
            PVR_DrawString(cStringTmp, &g_stRecInfoRect[i].stDmxFileNameRect, PVR_INFO_COLOR);
        }

        for(i = 0; i < PVR_MAX_PLAY_CHN; i++)
        {
            if (HI_TRUE != g_stTimeShiftChnAttr[i].bPlayChnInited)
            {
                continue;
            }

            s32Ret = HI_UNF_PVR_PlayGetChn(g_stTimeShiftChnAttr[i].u32PvrPlayChn, &stPvrPlayAttr);
            s32Ret = HI_UNF_DMX_GetTSBufferStatus(g_stTimeShiftChnAttr[i].hTsBufForPlayback, &stDmxTsBuffStatus);
            s32Ret |= HI_UNF_AVPLAY_GetStatusInfo(g_stTimeShiftChnAttr[i].hAvplay, &stAvplayStatus);
            s32Ret |= HI_UNF_PVR_GetFileAttrByFileName(stPvrPlayAttr.szFileName, &stPvrPlayFileAttr);
            s32Ret |= HI_UNF_PVR_PlayGetStatus(g_stTimeShiftChnAttr[i].u32PvrPlayChn, &stPvrPlayStatus);
            s32Ret |= HI_UNF_AVPLAY_GetStreamInfo(g_stTimeShiftChnAttr[i].hAvplay, &stAvplayStrmInfo);
            s32Ret |= HI_UNF_AVPLAY_GetAttr(g_stTimeShiftChnAttr[i].hAvplay, HI_UNF_AVPLAY_ATTR_ID_ADEC, &stAcodecAttr);
            s32Ret |= HI_UNF_AVPLAY_GetAttr(g_stTimeShiftChnAttr[i].hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVcodecAttr);
            if(HI_SUCCESS != s32Ret)
            {
                continue;
            }

            HI_SYS_GetTimeStampMs(&u32PlayCurSystemTimeMs);
            u32PlayDeltaTime = u32PlayCurSystemTimeMs - u32PlayLastSystemTimeMs[i];
            u32PlayLastSystemTimeMs[i] = u32PlayCurSystemTimeMs;

            /*Update play chan NO.*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Play Channel %d:", i);
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stPlayChnName, PVR_INFO_COLOR);

            /*Update Stream info.*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            if(HI_TRUE == g_stTimeShiftChnAttr[i].bVideoExist)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s\t\t%dx%d\t\t%s\t\t%s",
                          g_s8VideoType[stVcodecAttr.enType],
                          stAvplayStrmInfo.stVidStreamInfo.u32Width,
                          stAvplayStrmInfo.stVidStreamInfo.u32Height,
                          g_s8AudioType[0xff & HA_GET_ID(stAcodecAttr.enType)],
                          (HI_TRUE == stPvrPlayAttr.stDecryptCfg.bDoCipher) ? "Encrypt" : "Uncrypt");
            }
            else
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s\t\t%dx%d\t\t%s\t\t%s",
                          "UNKONOW",
                          0,
                          0,
                          g_s8AudioType[0xff & HA_GET_ID(stAcodecAttr.enType)],
                          (HI_TRUE == stPvrPlayAttr.stDecryptCfg.bDoCipher) ? "Encrypt" : "Uncrypt");
            }
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stStreamInfoRect, PVR_ALERT_COLOR);

            /*Update play speed*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            if (HI_UNF_PVR_PLAY_STATE_PAUSE == stPvrPlayStatus.enState)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Pause");
            }
            else if (HI_UNF_PVR_PLAY_STATE_STEPF == stPvrPlayStatus.enState)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Step F");
            }
            else if (HI_UNF_PVR_PLAY_STATE_STEPB == stPvrPlayStatus.enState)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Step B");
            }
            else if (0 > stPvrPlayStatus.enSpeed)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "-%dX", (stPvrPlayStatus.enSpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL));
            }
            else if (HI_UNF_PVR_PLAY_SPEED_NORMAL > stPvrPlayStatus.enSpeed)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "1/%dX", (HI_UNF_PVR_PLAY_SPEED_NORMAL / stPvrPlayStatus.enSpeed));
            }
            else
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%dX", (stPvrPlayStatus.enSpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL));
            }
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stPlaySpeedRect, PVR_INFO_COLOR);

            /*Update play fps*/
            if (stAvplayStatus.u32VidFrameCount < u32PlayedFrmCnt[i])
            {
                u32PlayedFrmCnt[i] = stAvplayStatus.u32VidFrameCount;
            }
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%01dfps",
                      (1000*(stAvplayStatus.u32VidFrameCount - u32PlayedFrmCnt[i])/u32PlayDeltaTime),
                      (1000*(stAvplayStatus.u32VidFrameCount - u32PlayedFrmCnt[i])%u32PlayDeltaTime)/100);
            u32PlayedFrmCnt[i] = stAvplayStatus.u32VidFrameCount;
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stPlayFps, PVR_INFO_COLOR);

            /*Update play time progress*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%02d:%02d:%02d/%02d:%02d:%02d",
                      stPvrPlayStatus.u32CurPlayTimeInMs/3600000,
                      (stPvrPlayStatus.u32CurPlayTimeInMs%3600000)/60000,
                      ((stPvrPlayStatus.u32CurPlayTimeInMs%3600000)%60000)/1000,
                      stPvrPlayFileAttr.u32EndTimeInMs / 3600000,
                      (stPvrPlayFileAttr.u32EndTimeInMs % 3600000)/60000,
                      ((stPvrPlayFileAttr.u32EndTimeInMs % 3600000)%60000)/1000);
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stPlayTimeProgress, PVR_INFO_COLOR);

            if(0 < (HI_S32)stPvrPlayStatus.enSpeed)
            {
                /*Update play read file speed*/
                /*avoid a very large value be calculating.
                   some stream's decoder order and display order is not the same,
                   so, the offset of a frame which displayed earlier may less than displayed later */
                if(stPvrPlayStatus.u64CurPlayPos >= u64PlayedSizeCnt[i])
                {
                    HI_U32 u32ReadSpeedInt = 0,u32ReadSpeedDec = 0;
                    memset(cStringTmp, 0, sizeof(cStringTmp));

                    u32ReadSpeedInt = (HI_U32)(1000*(8*(stPvrPlayStatus.u64CurPlayPos - u64PlayedSizeCnt[i]))/0x100000)/u32PlayDeltaTime;

                    if (0 == u32ReadSpeedInt)
                    {
                        u32ReadSpeedInt = (HI_U32)(1000*(8*(stPvrPlayStatus.u64CurPlayPos - u64PlayedSizeCnt[i]))/0x400)/u32PlayDeltaTime;
                        u32ReadSpeedDec = (HI_U32)(1000*(8*(stPvrPlayStatus.u64CurPlayPos - u64PlayedSizeCnt[i]))/0x400)%u32PlayDeltaTime;
                        snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%01d Kbps", u32ReadSpeedInt, u32ReadSpeedDec/100);
                    }
                    else
                    {
                        u32ReadSpeedDec = (HI_U32)((1000*(8*(stPvrPlayStatus.u64CurPlayPos - u64PlayedSizeCnt[i]))/0x100000)%u32PlayDeltaTime)/0x400;
                        snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%01d Mbps", u32ReadSpeedInt, u32ReadSpeedDec/100);
                    }

                    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stReadSpeedRect, PVR_INFO_COLOR);
                }
            }
            else
            {
                /*Update play read file speed*/
                /*avoid a very large value be calculating.
                   some stream's decoder order and display order is not the same,
                   so, the offset of a frame which displayed earlier may less than displayed later */
                if(stPvrPlayStatus.u64CurPlayPos <= u64PlayedSizeCnt[i])
                {
                    HI_U32 u32ReadSpeedInt = 0,u32ReadSpeedDec = 0;
                    memset(cStringTmp, 0, sizeof(cStringTmp));

                    u32ReadSpeedInt = (HI_U32)(1000*(8*(u64PlayedSizeCnt[i] - stPvrPlayStatus.u64CurPlayPos))/0x100000)/u32PlayDeltaTime;

                    if (0 == u32ReadSpeedInt)
                    {
                        u32ReadSpeedInt = (HI_U32)(1000*(8*(u64PlayedSizeCnt[i] - stPvrPlayStatus.u64CurPlayPos))/0x400)/u32PlayDeltaTime;
                        u32ReadSpeedDec = (HI_U32)(1000*(8*(u64PlayedSizeCnt[i] - stPvrPlayStatus.u64CurPlayPos))/0x400)%u32PlayDeltaTime;
                        snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%01d Kbps", u32ReadSpeedInt, u32ReadSpeedDec/100);
                    }
                    else
                    {
                        u32ReadSpeedDec = (HI_U32)((1000*(8*(u64PlayedSizeCnt[i] - stPvrPlayStatus.u64CurPlayPos))/0x100000)%u32PlayDeltaTime)/0x400;
                        snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d.%01d Mbps", u32ReadSpeedInt, u32ReadSpeedDec/100);
                    }

                    PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stReadSpeedRect, PVR_INFO_COLOR);
                }
            }
            u64PlayedSizeCnt[i] = stPvrPlayStatus.u64CurPlayPos;

            /*Update play file progress*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            if (0 == stPvrPlayFileAttr.u64ValidSizeInByte/0x100000)
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%lld.%01lld/%lld.%01lld KB",
                          stPvrPlayStatus.u64CurPlayPos/0x400,
                          (stPvrPlayStatus.u64CurPlayPos%0x400)/100,
                          stPvrPlayFileAttr.u64ValidSizeInByte/0x400,
                          (stPvrPlayFileAttr.u64ValidSizeInByte%0x400)/100);
            }
            else
            {
                snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%lld.%01lld/%lld.%01lld MB",
                          stPvrPlayStatus.u64CurPlayPos/0x100000,
                          (stPvrPlayStatus.u64CurPlayPos%0x100000)/0x400/100,
                          stPvrPlayFileAttr.u64ValidSizeInByte/0x100000,
                          (stPvrPlayFileAttr.u64ValidSizeInByte%0x100000)/0x400/100);
            }
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stFilePosRect, PVR_INFO_COLOR);

            /*Update play frames*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%d/%d", stPvrPlayStatus.u32CurPlayFrame, stPvrPlayFileAttr.u32FrameNum);
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stFrmPosRect, PVR_INFO_COLOR);

            /*Update play progress bar*/
            if (stPvrPlayStatus.u32CurPlayTimeInMs > stPvrPlayFileAttr.u32StartTimeInMs)
            {
                memcpy(&stRectTmp[i], &g_stPlayInfoRect[i].stPlayProgressBarRect, sizeof(HI_RECT));
                stRectTmp[i].w = g_stPlayInfoRect[i].stPlayProgressBarRect.w * (stPvrPlayStatus.u32CurPlayTimeInMs - stPvrPlayFileAttr.u32StartTimeInMs) / (stPvrPlayFileAttr.u32EndTimeInMs - stPvrPlayFileAttr.u32StartTimeInMs);
                PVR_DrawProgressBar(&g_stPlayInfoRect[i].stPlayProgressBarRect, &stRectTmp[i], PVR_PROGRESS_COLOR);
            }

            /*Update play File Name*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "%s", stPvrPlayAttr.szFileName);
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stPlayFileNameRect, PVR_INFO_COLOR);


            /*Update Dmx TS buffer bar*/
            memcpy(&stRectTmp[i], &g_stPlayInfoRect[i].stPlayDmxBuffBarRect, sizeof(HI_RECT));
            stRectTmp[i].w = g_stPlayInfoRect[i].stPlayDmxBuffBarRect.w * stDmxTsBuffStatus.u32UsedSize/ stDmxTsBuffStatus.u32BufSize;
            PVR_DrawProgressBar(&g_stPlayInfoRect[i].stPlayDmxBuffBarRect, &stRectTmp[i], PVR_PROGRESS_COLOR);

            /*Update Dmx Ts buffer size*/
            memset(cStringTmp, 0, sizeof(cStringTmp));
            snprintf(cStringTmp, PVR_MAX_INFO_SIZE, "Demux%d %d/%dKB",
                      g_stTimeShiftChnAttr[i].u32PlayDemuxId,
                      stDmxTsBuffStatus.u32UsedSize/0x400,
                      stDmxTsBuffStatus.u32BufSize/0x400);
            PVR_DrawString(cStringTmp, &g_stPlayInfoRect[i].stPlayDmxNameRect, PVR_INFO_COLOR);
        }

        HI_GO_RefreshLayer(hLayer_pvr, NULL);

        (void)pthread_mutex_unlock(&g_stDrawRectMutex);
    }

    return HI_NULL;
}

HI_VOID *PVR_IRReceiveThread(HI_VOID *args)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TestMode = 0;
    HI_U32 u32ChanNum = 0;
    HI_U64 u64KeyId = 0;
    HI_UNF_KEY_STATUS_E PressStatus;

    u32TestMode = *(HI_U32 *)args;

    while (HI_FALSE == g_bIsMainRoutineStop)
    {
        /* get ir press codevalue & press status*/
        s32Ret = HI_UNF_IR_GetValue(&PressStatus, &u64KeyId, 0);
        if ((HI_SUCCESS != s32Ret) || (PressStatus == HI_UNF_KEY_STATUS_UP))
        {
            continue;
        }

        if( PVR_IR_POWER == u64KeyId)
        {
            sample_common_printf("\nExit main routine!!\n");
            g_bIsMainRoutineStop = HI_TRUE;
            break;
        }

        if(PVR_TEST_MODE_TIMESHIFT == u32TestMode)
        {
            if((0 == g_TimeshiftChn) && (PVR_IR_EPG != u64KeyId) && (PVR_IR_NUM_EN != u64KeyId))
            {
                usleep(10000);
                continue;
            }
        }

        if(PVR_TEST_MODE_PLAYBACK == u32TestMode)
        {
            if((0 == g_PlayChn) && (PVR_IR_EPG != u64KeyId))
            {
                usleep(10000);
                continue;
            }
        }

        switch(u32TestMode)
        {
            case PVR_TEST_MODE_TIMESHIFT:
                {
                    switch(u64KeyId)
                    {
                        case PVR_IR_EPG://help, EPG button
                            {
                                printf("keys:\n"
                                       "1.\"EPG\"\n"
                                       "\tpress \"EPG\" button for help\n"
                                       "2.\"Power\"\n"
                                       "\t\"Power\" button to exit programe\n"
                                       "3.\"-/--\" -> NUM -> \"OK\" \n"
                                       "\tadd a program, press \"-/--\" button, then press a number button,then \"OK\" button\n"
                                       "4.\"LEFT\" or \"RIGHT\"\n"
                                       "\tpress \"LEFT\" or \"RIGHT\" button to select the left or right window\n"
                                       "5.\"Search\"\n"
                                       "\tpress \"Search\" button to zoomin or zoomout a selected window\n"
                                       "6.\"nPVR\"\n"
                                       "\tpress \"nPVR\" button to start a recording of a selected window\n"
                                       "7.\"Play/Pause\"\n"
                                       "\tpress \"Play/Pause\" button to pause or resume a play window.\n"
                                       "\twhen the selected window is live playing, press this button twice will start a playback from the current time.\n"
                                       "8.\"VOD\"\n"
                                       "\tswitch live playing to playback\n"
                                       "9.\"BTV\"\n"
                                       "\tswitch playback to live playing\n"
                                       "10.\"PgUp\" or \"PgDown\"\n"
                                       "\tseek backward or forward 5 seconds\n"
                                       "11.\"UP\" or \"DOWN\"\n"
                                       "\tseek to start or end of file\n"
                                       "12.\"REW\" or \"FF\"\n"
                                       "\tfast backward or fast forward play, press once to switch a speed\n"
                                       "13.\"Back\"\n"
                                       "\tslow forward play, press once to switch a speed\n"
                                       "14.\"Stop\"\n"
                                       "\tpress \"Stop\" button in other mode will enter the step mode, press once to display a frame\n"
                                       "15.\"mute\"\n"
                                       "\tpress \"Mute\" button to set or cancel audio mute\n"
                                       "16.\"F1\" or \"F2\"\n"
                                       "\tresume or pause a recording channel\n");
                                break;
                            }
                        case PVR_IR_NUM_EN://enable program number input, -/--button
                            {
                                HI_U32 u32PressNum = 1;
                                HI_U32 u32ProgNum = 0;

                                u64KeyId = 0;
                                printf("\rPress num:");
                                fflush(stdout);
                                do
                                {
                                    s32Ret = HI_UNF_IR_GetValue(&PressStatus, &u64KeyId, 0);
                                    if ((HI_SUCCESS != s32Ret) || (PressStatus == HI_UNF_KEY_STATUS_UP))
                                    {
                                        continue;
                                    }

                                    if ((PVR_IR_0 != u64KeyId) && (PVR_IR_1 != u64KeyId) && (PVR_IR_2 != u64KeyId) &&
                                       (PVR_IR_3 != u64KeyId) && (PVR_IR_4 != u64KeyId) && (PVR_IR_5 != u64KeyId) &&
                                       (PVR_IR_6 != u64KeyId) && (PVR_IR_7 != u64KeyId) && (PVR_IR_8 != u64KeyId) &&
                                       (PVR_IR_9 != u64KeyId) && (PVR_IR_OK != u64KeyId))
                                    {
                                        printf("please press button 0 ~ 9\n");
                                        continue;
                                    }

                                    switch (u64KeyId)
                                    {
                                        case PVR_IR_0:
                                            u32ProgNum *= u32PressNum;
                                            break;
                                        case PVR_IR_1:
                                            u32ProgNum = u32ProgNum*u32PressNum + 1;
                                            break;
                                        case PVR_IR_2:
                                            u32ProgNum = u32ProgNum*u32PressNum + 2;
                                            break;
                                        case PVR_IR_3:
                                            u32ProgNum = u32ProgNum*u32PressNum + 3;
                                            break;
                                        case PVR_IR_4:
                                            u32ProgNum = u32ProgNum*u32PressNum + 4;
                                            break;
                                        case PVR_IR_5:
                                            u32ProgNum = u32ProgNum*u32PressNum + 5;
                                            break;
                                        case PVR_IR_6:
                                            u32ProgNum = u32ProgNum*u32PressNum + 6;
                                            break;
                                        case PVR_IR_7:
                                            u32ProgNum = u32ProgNum*u32PressNum + 7;
                                            break;
                                        case PVR_IR_8:
                                            u32ProgNum = u32ProgNum*u32PressNum + 8;
                                            break;
                                        case PVR_IR_9:
                                            u32ProgNum = u32ProgNum*u32PressNum + 9;
                                            break;
                                        default:
                                            break;
                                    };

                                    if ((0 <= u32ProgNum) && (9 >= u32ProgNum))
                                    {
                                        u32PressNum = 10;
                                    }

                                    printf("\rPress num:%d",u32ProgNum);
                                    fflush(stdout);
                                }while(((PVR_IR_OK != u64KeyId) || (HI_SUCCESS != s32Ret)) &&
                                                   (HI_FALSE == g_bIsMainRoutineStop));
                                printf("\n");

                                if(u32ProgNum > g_pstProgTbl->prog_num)
                                {
                                    printf("program number %d is out range, the program number is 1 ~ %d.\n",
                                            u32ProgNum, g_pstProgTbl->prog_num);
                                    break;
                                }

                                PVR_AddALivePlay(u32ProgNum);
                                break;
                            }
                        case PVR_IR_MUTE://mute a play channel, Mute button
                            {
                                if (HI_FALSE == g_stTimeShiftChnAttr[u32ChanNum].bMuteSeted)
                                {
                                    s32Ret = HI_UNF_SND_SetTrackMute(g_stTimeShiftChnAttr[u32ChanNum].hSoundTrack, HI_TRUE);
                                    if (HI_SUCCESS != s32Ret)
                                    {
                                        sample_common_printf("Set mute fail! track=%#x ret=%#x\n", g_stTimeShiftChnAttr[u32ChanNum].hSoundTrack, s32Ret);
                                        break;
                                    }

                                    g_stTimeShiftChnAttr[u32ChanNum].bMuteSeted = HI_TRUE;
                                }
                                else
                                {
                                    s32Ret = HI_UNF_SND_SetTrackMute(g_stTimeShiftChnAttr[u32ChanNum].hSoundTrack, HI_FALSE);
                                    if (HI_SUCCESS != s32Ret)
                                    {
                                        sample_common_printf("Stop avplay fail! ret=%#x\n", s32Ret);
                                        break;
                                    }

                                    g_stTimeShiftChnAttr[u32ChanNum].bMuteSeted = HI_FALSE;
                                }

                                (void)pthread_mutex_lock(&g_stDrawRectMutex);
                                if (HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bFullScreen)
                                {
                                    HI_RECT stMuteRect;

                                    #if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
                                    stMuteRect.x = 50;
                                    stMuteRect.y = 10;
                                    stMuteRect.w = 40;
                                    stMuteRect.h = 25;
                                    #else
                                    stMuteRect.x = 50;
                                    stMuteRect.y = 50;
                                    stMuteRect.w = 60;
                                    stMuteRect.h = 40;
                                    #endif

                                    PVR_DrawMuteText(&stMuteRect, g_stTimeShiftChnAttr[u32ChanNum].bMuteSeted);
                                }
                                else
                                {
                                    PVR_DrawMuteText(&g_stTimeShiftChnAttr[u32ChanNum].stMuteTextRect,
                                                       g_stTimeShiftChnAttr[u32ChanNum].bMuteSeted);
                                }
                                (void)pthread_mutex_unlock(&g_stDrawRectMutex);
                                break;
                            }
                        case PVR_IR_LEFT://select left window, left button
                            {
                                HI_U32 i = 0;
                                HI_UNF_WINDOW_ATTR_S stWinAttr;

                                for(i = 0; i < PVR_MAX_PLAY_CHN; i++)
                                {
                                    if (0 != g_stTimeShiftChnAttr[u32ChanNum].hWindow)
                                    {
                                        s32Ret = HI_UNF_VO_GetWindowAttr(g_stTimeShiftChnAttr[u32ChanNum].hWindow,
                                                                            &stWinAttr);
                                        if(stWinAttr.stOutputRect.s32Width != g_stTimeShiftChnAttr[u32ChanNum].stRect.s32Width)
                                        {
                                            break;
                                        }
                                    }
                                }

                                if (PVR_MAX_PLAY_CHN != i)
                                {
                                    break;
                                }

                                //u32ChanNum = (0 == u32ChanNum) ? 0 : (--u32ChanNum);

                                if (0 != u32ChanNum)
                                {
                                    u32ChanNum--;
                                }

                                if (0 != g_stTimeShiftChnAttr[u32ChanNum].hWindow)
                                {
                                    (void)pthread_mutex_lock(&g_stDrawRectMutex);

                                    if (0 != g_stTimeShiftChnAttr[u32ChanNum+1].hWindow)
                                    {
                                        PVR_DrawSelectRect(&g_stTimeShiftChnAttr[u32ChanNum+1].stRect,
                                                             HI_FALSE,
                                                             g_stTimeShiftChnAttr[u32ChanNum+1].bVideoExist);
                                    }

                                    PVR_DrawSelectRect(&g_stTimeShiftChnAttr[u32ChanNum].stRect,
                                                         HI_TRUE,
                                                         g_stTimeShiftChnAttr[u32ChanNum].bVideoExist);
                                    (void)pthread_mutex_unlock(&g_stDrawRectMutex);
                                }
                                else
                                {
                                    u32ChanNum++;
                                }

                                break;
                            }
                        case PVR_IR_RIGHT://select right window, right button
                            {
                                HI_U32 i = 0;
                                HI_UNF_WINDOW_ATTR_S stWinAttr;

                                for(i = 0; i < PVR_MAX_PLAY_CHN; i++)
                                {
                                    if (0 != g_stTimeShiftChnAttr[u32ChanNum].hWindow)
                                    {
                                        s32Ret = HI_UNF_VO_GetWindowAttr(g_stTimeShiftChnAttr[u32ChanNum].hWindow,
                                                                            &stWinAttr);
                                        if(stWinAttr.stOutputRect.s32Width != g_stTimeShiftChnAttr[u32ChanNum].stRect.s32Width)
                                        {
                                            break;
                                        }
                                    }
                                }

                                if (PVR_MAX_PLAY_CHN != i)
                                {
                                    break;
                                }

                                //u32ChanNum = ((PVR_MAX_PLAY_CHN-1) == u32ChanNum) ? (PVR_MAX_PLAY_CHN-1) : (++u32ChanNum);

                                if((PVR_MAX_PLAY_CHN-1) != u32ChanNum)
                                {
                                    u32ChanNum++;
                                }


                                if (0 != g_stTimeShiftChnAttr[u32ChanNum].hWindow)
                                {
                                    (void)pthread_mutex_lock(&g_stDrawRectMutex);

                                    if (0 != g_stTimeShiftChnAttr[u32ChanNum-1].hWindow)
                                    {
                                        PVR_DrawSelectRect(&g_stTimeShiftChnAttr[u32ChanNum-1].stRect,
                                                             HI_FALSE,
                                                             g_stTimeShiftChnAttr[u32ChanNum-1].bVideoExist);
                                    }

                                    PVR_DrawSelectRect(&g_stTimeShiftChnAttr[u32ChanNum].stRect,
                                                         HI_TRUE,
                                                         g_stTimeShiftChnAttr[u32ChanNum].bVideoExist);

                                    (void)pthread_mutex_unlock(&g_stDrawRectMutex);
                                }
                                else
                                {
                                    u32ChanNum--;
                                }
                                break;
                            }
                        case PVR_IR_UP://seek to start, up button
                            {
                                HI_UNF_PVR_PLAY_POSITION_S stPlayPos;

                                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                s32Ret = HI_UNF_PVR_PlayResumeChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                                }

                                stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                                stPlayPos.s64Offset = 0;
                                stPlayPos.s32Whence = SEEK_SET;
                                s32Ret = HI_UNF_PVR_PlaySeek(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayPos);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayStep failed.\n");
                                }

                                break;
                            }
                        case PVR_IR_DOWN://seek to end, down button
                            {
                                HI_UNF_PVR_PLAY_POSITION_S stPlayPos;

                                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                s32Ret = HI_UNF_PVR_PlayResumeChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                                }

                                stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                                stPlayPos.s64Offset = 0;
                                stPlayPos.s32Whence = SEEK_END;
                                s32Ret = HI_UNF_PVR_PlaySeek(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayPos);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayStep failed.\n");
                                }

                                break;
                            }
                        case PVR_IR_ZOOM://zoom in and zoom out a window,  zoom button
                            {
                                HI_UNF_WINDOW_ATTR_S stWinAttr;

                                s32Ret = HI_UNF_VO_GetWindowAttr(g_stTimeShiftChnAttr[u32ChanNum].hWindow,
                                                                    &stWinAttr);
                                if(HI_SUCCESS != s32Ret)
                                {
                                    sample_common_printf("Get window attr fail! window handle=%d chnnum=%d\n",g_stTimeShiftChnAttr[u32ChanNum].hWindow, u32ChanNum);
                                    break;
                                }

                                if (stWinAttr.stOutputRect.s32Width == g_stTimeShiftChnAttr[u32ChanNum].stRect.s32Width)
                                {
                                    if (HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bVideoExist)
                                    {
                                        PVR_ZoomInDispToFullScreen(g_stTimeShiftChnAttr[u32ChanNum].hWindow,
                                                                      &g_stTimeShiftChnAttr[u32ChanNum].stRect);

                                        g_stTimeShiftChnAttr[u32ChanNum].bFullScreen = HI_TRUE;
                                    }
                                }
                                else
                                {
                                    if (HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bVideoExist)
                                    {
                                        PVR_ZoomOutDispToWindow(g_stTimeShiftChnAttr[u32ChanNum].hWindow,
                                                                  &g_stTimeShiftChnAttr[u32ChanNum].stRect,
                                                                  g_stTimeShiftChnAttr[u32ChanNum].u32WindowZorder);

                                        g_stTimeShiftChnAttr[u32ChanNum].bFullScreen = HI_FALSE;
                                    }
                                }
                                break;
                            }
                        case PVR_IR_NPVR://start recording with default configuration, nPVR button
                            {
                                HI_U64 u64MaxRecTimeMs = 0;
                                HI_U64 u64MaxRecSize = 0;

                                if(HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bRecChnInited)
                                {
                                    sample_common_printf("Chan %d is already recording!!\n", u32ChanNum);
                                    break;
                                }

                                u64MaxRecTimeMs = 1000 * 3600;
                                u64MaxRecSize = 0;

                                //rewind recording by 60min
                                if(HI_SUCCESS == PVR_AddARec(g_stTimeShiftChnAttr[u32ChanNum].hPlayChn,
                                                             g_sRecPath,
                                                             g_stTimeShiftChnAttr[u32ChanNum].u32ProgNum,
                                                             HI_TRUE,
                                                             HI_FALSE,
                                                             u64MaxRecSize,
                                                             u64MaxRecTimeMs,
                                                             &g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn,
                                                             PVR_TEST_MODE_TIMESHIFT))
                                {
                                    g_stTimeShiftChnAttr[u32ChanNum].bRecChnInited = HI_TRUE;
                                }
                                break;
                            }
                        case PVR_IR_PLAY_PAUSE:
                            {
                                HI_UNF_PVR_REC_ATTR_S stPvrRecAttr;

                                /*recording and live playing*/
                                if ((HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bRecChnInited) &&
                                    (HI_FALSE == g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited))
                                {
                                    if (HI_FALSE == g_stTimeShiftChnAttr[u32ChanNum].bLiveStoped)
                                    {
                                        s32Ret = PVR_StopLive(g_stTimeShiftChnAttr[u32ChanNum].hAvplay);
                                        if (s32Ret != HI_SUCCESS)
                                        {
                                            sample_common_printf("Stop live failed. ret=%#x\n", s32Ret);
                                        }

                                        s32Ret = HI_UNF_PVR_PlayPauseChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn);
                                        if (s32Ret != HI_SUCCESS)
                                        {
                                            sample_common_printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
                                        }

                                        g_stTimeShiftChnAttr[u32ChanNum].bLiveStoped = HI_TRUE;
                                    }
                                    else
                                    {
                                        g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                        g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                        g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                        s32Ret = HI_UNF_PVR_RecGetChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn, &stPvrRecAttr);

                                        s32Ret = HI_UNF_DMX_DetachTSPort(g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId);

                                        s32Ret = PVR_StartPlay(stPvrRecAttr.szFileName,
                                                                 g_stTimeShiftChnAttr[u32ChanNum].hPlayChn,
                                                                 g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId,
                                                                 g_stTimeShiftChnAttr[u32ChanNum].hAvplay,
                                                                 &g_stTimeShiftChnAttr[u32ChanNum].hTsBufForPlayback,
                                                                 &g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn,
                                                                 &g_stTimeShiftChnAttr[u32ChanNum].bVideoExist);
                                        g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited = HI_TRUE;
                                        if(0 != s32Ret)
                                        {
                                            sample_common_printf("Start pvr play fail! playchn=%d filename=%s\n", g_stTimeShiftChnAttr[u32ChanNum].hPlayChn, stPvrRecAttr.szFileName);
                                            break;
                                        }
                                    }
                                }
                                else /*play back*/
                                {
                                    if (HI_FALSE == g_stTimeShiftChnAttr[u32ChanNum].bPlayBackPaused)
                                    {
                                        s32Ret = HI_UNF_PVR_PlayPauseChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn);
                                        if (s32Ret != HI_SUCCESS)
                                        {
                                            sample_common_printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
                                        }

                                        g_stTimeShiftChnAttr[u32ChanNum].bPlayBackPaused = HI_TRUE;
                                    }
                                    else
                                    {
                                        g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                        g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                        g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;
                                        s32Ret = HI_UNF_PVR_PlayResumeChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn);
                                        if (s32Ret != HI_SUCCESS)
                                        {
                                            sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                                        }

                                        g_stTimeShiftChnAttr[u32ChanNum].bPlayBackPaused = HI_FALSE;
                                    }
                                }
                                break;
                            }
                        case PVR_IR_VOD://switch live to playback, VOD button
                            {
                                HI_UNF_PVR_REC_ATTR_S stPvrRecAttr;
                                HI_UNF_AVPLAY_STOP_OPT_S stopOpt;

                                if (HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited)
                                {
                                    sample_common_printf("Chan %d is already playing!!\n", u32ChanNum);
                                    break;
                                }

                                if (HI_FALSE == g_stTimeShiftChnAttr[u32ChanNum].bRecChnInited)
                                {
                                    sample_common_printf("Chan %d dosn't recording yet!!\n", u32ChanNum);
                                    break;
                                }

                                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                stopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
                                stopOpt.u32TimeoutMs = 0;

                                s32Ret = HI_UNF_AVPLAY_Stop(g_stTimeShiftChnAttr[u32ChanNum].hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stopOpt);

                                s32Ret = HI_UNF_PVR_RecGetChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn, &stPvrRecAttr);

                                s32Ret = HI_UNF_DMX_DetachTSPort(g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId);

                                s32Ret = PVR_StartPlay(stPvrRecAttr.szFileName,
                                                         g_stTimeShiftChnAttr[u32ChanNum].hPlayChn,
                                                         g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId,
                                                         g_stTimeShiftChnAttr[u32ChanNum].hAvplay,
                                                         &g_stTimeShiftChnAttr[u32ChanNum].hTsBufForPlayback,
                                                         &g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn,
                                                         &g_stTimeShiftChnAttr[u32ChanNum].bVideoExist);
                                g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited = HI_TRUE;
                                if(0 != s32Ret)
                                {
                                    sample_common_printf("Start pvr play fail! playchn=%d filename=%s\n", g_stTimeShiftChnAttr[u32ChanNum].hPlayChn, stPvrRecAttr.szFileName);
                                }

                                g_stTimeShiftChnAttr[u32ChanNum].bLiveStoped = HI_TRUE;
                                break;
                            }
                        case PVR_IR_BTV://switch playback to live,  BTV button
                            {
                                if (HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bLiveStoped)
                                {
                                    g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited = HI_FALSE;

                                    PVR_StopPlay(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn,
                                                  g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId,
                                                  g_stTimeShiftChnAttr[u32ChanNum].hTsBufForPlayback);

                                    s32Ret = HI_UNF_DMX_AttachTSPort(g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId, g_stTimeShiftChnAttr[u32ChanNum].u32DmxPort);

                                    s32Ret = PVR_StartLive(g_stTimeShiftChnAttr[u32ChanNum].u32ProgNum,
                                                            g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId,
                                                            g_stTimeShiftChnAttr[u32ChanNum].hAvplay,
                                                            &g_stTimeShiftChnAttr[u32ChanNum].bVideoExist,
                                                            g_stTimeShiftChnAttr[u32ChanNum].u32DmxPort);

                                    PVR_DrawPlayChnInitRect(g_stTimeShiftChnAttr[u32ChanNum].hPlayChn);

                                    g_stTimeShiftChnAttr[u32ChanNum].bLiveStoped = HI_FALSE;
                                }
                                break;
                            }
                        case PVR_IR_PGUP://seek backward 5s, PgUp button
                            {
                                HI_UNF_PVR_PLAY_POSITION_S stPlayPos;

                                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                s32Ret = HI_UNF_PVR_PlayResumeChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                                }

                                stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                                stPlayPos.s64Offset = -(HI_S64)5000;
                                stPlayPos.s32Whence = SEEK_CUR;
                                s32Ret = HI_UNF_PVR_PlaySeek(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayPos);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayStep failed.\n");
                                }

                                break;
                            }
                        case PVR_IR_PGDOWN://seek forward 5s, PgDown
                            {
                                HI_UNF_PVR_PLAY_POSITION_S stPlayPos;

                                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                s32Ret = HI_UNF_PVR_PlayResumeChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                                }

                                stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                                stPlayPos.s64Offset = (HI_S64)5000;
                                stPlayPos.s32Whence = SEEK_CUR;
                                s32Ret = HI_UNF_PVR_PlaySeek(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayPos);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayStep failed.\n");
                                }
                                break;
                            }
                        case PVR_IR_REW://fast backward play, REW button
                            {
                                HI_UNF_PVR_PLAY_MODE_S stPlayTrickMode;
                                HI_UNF_PVR_REC_ATTR_S stPvrRecAttr;

                                if((HI_FALSE == g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited) &&
                                    (HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bRecChnInited))
                                {
                                    s32Ret = PVR_StopLive(g_stTimeShiftChnAttr[u32ChanNum].hAvplay);
                                    if (s32Ret != HI_SUCCESS)
                                    {
                                        sample_common_printf("Stop live failed. ret=%#x\n", s32Ret);
                                    }

                                    s32Ret = HI_UNF_PVR_PlayPauseChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn);
                                    if (s32Ret != HI_SUCCESS)
                                    {
                                        sample_common_printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
                                    }


                                    s32Ret = HI_UNF_PVR_RecGetChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn, &stPvrRecAttr);

                                    s32Ret = HI_UNF_DMX_DetachTSPort(g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId);

                                    s32Ret = PVR_StartPlay(stPvrRecAttr.szFileName,
                                                             g_stTimeShiftChnAttr[u32ChanNum].hPlayChn,
                                                             g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId,
                                                             g_stTimeShiftChnAttr[u32ChanNum].hAvplay,
                                                             &g_stTimeShiftChnAttr[u32ChanNum].hTsBufForPlayback,
                                                             &g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn,
                                                             &g_stTimeShiftChnAttr[u32ChanNum].bVideoExist);

                                    g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited = HI_TRUE;
                                    g_stTimeShiftChnAttr[u32ChanNum].bLiveStoped = HI_TRUE;

                                    g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                    g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;
                                    g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                }

                                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes %= 6;
                                stPlayTrickMode.enSpeed = -(0x1 << (g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes + 1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;

                                s32Ret = HI_UNF_PVR_PlayTPlay(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayTrickMode);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                                }

                                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes++;
                                g_stTimeShiftChnAttr[u32ChanNum].bPlayBackPaused = HI_TRUE;

                                break;
                            }
                        case PVR_IR_FF://fast forward play, FF button
                            {
                                HI_UNF_PVR_PLAY_MODE_S stPlayTrickMode;

                                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes %= 6;
                                stPlayTrickMode.enSpeed = (0x1 << (g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes + 1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;

                                s32Ret = HI_UNF_PVR_PlayTPlay(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayTrickMode);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                                }

                                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes++;
                                g_stTimeShiftChnAttr[u32ChanNum].bPlayBackPaused = HI_TRUE;

                                break;
                            }
                        case PVR_IR_BACK://slow forward play, Back button
                            {
                                HI_UNF_PVR_PLAY_MODE_S stPlayTrickMode;

                                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;

                                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes %= 6;
                                stPlayTrickMode.enSpeed = HI_UNF_PVR_PLAY_SPEED_NORMAL / (0x1 << (g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes + 1));

                                s32Ret = HI_UNF_PVR_PlayTPlay(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayTrickMode);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                                }

                                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes++;
                                g_stTimeShiftChnAttr[u32ChanNum].bPlayBackPaused = HI_TRUE;

                                break;
                            }
                        case PVR_IR_STOP://step play, Stop button
                            {
                                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                s32Ret = HI_UNF_PVR_PlayStep(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn, 1);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayStep failed.\n");
                                }

                                g_stTimeShiftChnAttr[u32ChanNum].bPlayBackPaused = HI_TRUE;

                                break;
                            }
                        case PVR_IR_F1://resume recording, F1 button
                            {
                                s32Ret = HI_UNF_PVR_RecResumeChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_RecResumeChn failed.\n");
                                }

                                break;
                            }
                        case PVR_IR_F2://pause recording, F2 button
                            {
                                s32Ret = HI_UNF_PVR_RecPauseChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_RecPauseChn failed.\n");
                                }

                                break;
                            }
                        default:
                            break;

                    };
                    break;
                }
            case PVR_TEST_MODE_PLAYBACK:
                {
                    switch(u64KeyId)
                    {
                        case PVR_IR_EPG://help, EPG button
                            {
                                printf("keys:\n"
                                       "1.\"EPG\"\n"
                                       "\tpress \"EPG\" button for help\n"
                                       "2.\"Power\"\n"
                                       "\t\"Power\" button to exit programe\n"
                                       "3.\"LEFT\" or \"RIGHT\"\n"
                                       "\tpress \"LEFT\" or \"RIGHT\" button to select the left or right window\n"
                                       "4.\"Search\"\n"
                                       "\tpress \"Search\" button to zoomin or zoomout a selected window\n"
                                       "5.\"Play/Pause\"\n"
                                       "\tpress \"Play/Pause\" button to pause or resume a play window.\n"
                                       "\twhen the selected window is live playing, press this button twice will start a playback from the current time.\n"
                                       "6.\"PgUp\" or \"PgDown\"\n"
                                       "\tseek backward or forward 5 seconds\n"
                                       "7.\"UP\" or \"DOWN\"\n"
                                       "\tseek to start or end of file\n"
                                       "8.\"REW\" or \"FF\"\n"
                                       "\tfast backward or fast forward play, press once to switch a speed\n"
                                       "9.\"Back\"\n"
                                       "\tslow forward play, press once to switch a speed\n"
                                       "10.\"Stop\"\n"
                                       "\tpress \"Stop\" button in other mode will enter the step mode, press once to display a frame\n"
                                       "11.\"mute\"\n"
                                       "\tpress \"Mute\" button to set or cancel audio mute\n");
                                break;
                            }
                        case PVR_IR_LEFT://select left window, left button
                            {
                                HI_U32 i = 0;
                                HI_UNF_WINDOW_ATTR_S stWinAttr;

                                for(i = 0; i < PVR_MAX_PLAY_CHN; i++)
                                {
                                    if (0 != g_stPlayChnAttr[u32ChanNum].hWindow)
                                    {
                                        s32Ret = HI_UNF_VO_GetWindowAttr(g_stPlayChnAttr[u32ChanNum].hWindow,
                                                                            &stWinAttr);
                                        if(stWinAttr.stOutputRect.s32Width != g_stPlayChnAttr[u32ChanNum].stRect.s32Width)
                                        {
                                            break;
                                        }
                                    }
                                }

                                if (PVR_MAX_PLAY_CHN != i)
                                {
                                    break;
                                }

                                //u32ChanNum = (0 == u32ChanNum) ? 0 : (--u32ChanNum);
                                if (0 != u32ChanNum)
                                {
                                    u32ChanNum--;
                                }

                                if (0 != g_stPlayChnAttr[u32ChanNum].hWindow)
                                {
                                    (void)pthread_mutex_lock(&g_stDrawRectMutex);

                                    if (0 != g_stPlayChnAttr[u32ChanNum+1].hWindow)
                                    {
                                        PVR_DrawSelectRect(&g_stPlayChnAttr[u32ChanNum+1].stRect,
                                                             HI_FALSE,
                                                             g_stPlayChnAttr[u32ChanNum+1].bVideoExist);
                                    }

                                    PVR_DrawSelectRect(&g_stPlayChnAttr[u32ChanNum].stRect,
                                                         HI_TRUE,
                                                         g_stPlayChnAttr[u32ChanNum].bVideoExist);

                                    (void)pthread_mutex_unlock(&g_stDrawRectMutex);
                                }
                                else
                                {
                                    u32ChanNum++;
                                }

                                break;
                            }
                        case PVR_IR_RIGHT://select right window, right button
                            {
                                HI_U32 i = 0;
                                HI_UNF_WINDOW_ATTR_S stWinAttr;

                                for(i = 0; i < PVR_MAX_PLAY_CHN; i++)
                                {
                                    if (0 != g_stPlayChnAttr[u32ChanNum].hWindow)
                                    {
                                        s32Ret = HI_UNF_VO_GetWindowAttr(g_stPlayChnAttr[u32ChanNum].hWindow,
                                                                            &stWinAttr);
                                        if(stWinAttr.stOutputRect.s32Width != g_stPlayChnAttr[u32ChanNum].stRect.s32Width)
                                        {
                                            break;
                                        }
                                    }
                                }

                                if (PVR_MAX_PLAY_CHN != i)
                                {
                                    break;
                                }

                                //u32ChanNum = ((PVR_MAX_PLAY_CHN-1) == u32ChanNum) ? (PVR_MAX_PLAY_CHN-1) : (++u32ChanNum);

                                if ((PVR_MAX_PLAY_CHN-1) != u32ChanNum)
                                {
                                    u32ChanNum++;
                                }

                                if (0 != g_stPlayChnAttr[u32ChanNum].hWindow)
                                {
                                    (void)pthread_mutex_lock(&g_stDrawRectMutex);

                                    if (0 != g_stPlayChnAttr[u32ChanNum-1].hWindow)
                                    {
                                        PVR_DrawSelectRect(&g_stPlayChnAttr[u32ChanNum-1].stRect,
                                                             HI_FALSE,
                                                             g_stPlayChnAttr[u32ChanNum-1].bVideoExist);
                                    }

                                    PVR_DrawSelectRect(&g_stPlayChnAttr[u32ChanNum].stRect,
                                                         HI_TRUE,
                                                         g_stPlayChnAttr[u32ChanNum].bVideoExist);

                                    (void)pthread_mutex_unlock(&g_stDrawRectMutex);
                                }
                                else
                                {
                                    u32ChanNum--;
                                }
                                break;
                            }
                        case PVR_IR_UP://seek to start, up button
                            {
                                HI_UNF_PVR_PLAY_POSITION_S stPlayPos;

                                g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                s32Ret = HI_UNF_PVR_PlayResumeChn(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                                }

                                stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                                stPlayPos.s64Offset = 0;
                                stPlayPos.s32Whence = SEEK_SET;
                                s32Ret = HI_UNF_PVR_PlaySeek(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayPos);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayStep failed.\n");
                                }

                                break;
                            }
                        case PVR_IR_DOWN://seek to end, down button
                            {
                                HI_UNF_PVR_PLAY_POSITION_S stPlayPos;

                                g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                s32Ret = HI_UNF_PVR_PlayResumeChn(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                                }

                                stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                                stPlayPos.s64Offset = 0;
                                stPlayPos.s32Whence = SEEK_END;
                                s32Ret = HI_UNF_PVR_PlaySeek(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayPos);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayStep failed.\n");
                                }

                                break;
                            }
                        case PVR_IR_ZOOM://zoom in and zoom out a window,  zoom button
                            {
                                HI_UNF_WINDOW_ATTR_S stWinAttr;

                                s32Ret = HI_UNF_VO_GetWindowAttr(g_stPlayChnAttr[u32ChanNum].hWindow,
                                                                    &stWinAttr);
                                if(HI_SUCCESS != s32Ret)
                                {
                                    sample_common_printf("Get window attr fail! window handle=%d chnnum=%d\n",g_stPlayChnAttr[u32ChanNum].hWindow, u32ChanNum);
                                    break;
                                }

                                if (stWinAttr.stOutputRect.s32Width == g_stPlayChnAttr[u32ChanNum].stRect.s32Width)
                                {
                                    if (HI_TRUE == g_stPlayChnAttr[u32ChanNum].bVideoExist)
                                    {
                                        PVR_ZoomInDispToFullScreen(g_stPlayChnAttr[u32ChanNum].hWindow,
                                                                      &g_stPlayChnAttr[u32ChanNum].stRect);

                                        g_stPlayChnAttr[u32ChanNum].bFullScreen = HI_TRUE;
                                    }
                                }
                                else
                                {
                                    if (HI_TRUE == g_stPlayChnAttr[u32ChanNum].bVideoExist)
                                    {
                                        PVR_ZoomOutDispToWindow(g_stPlayChnAttr[u32ChanNum].hWindow,
                                                                  &g_stPlayChnAttr[u32ChanNum].stRect,
                                                                  g_stPlayChnAttr[u32ChanNum].u32WindowZorder);

                                        g_stPlayChnAttr[u32ChanNum].bFullScreen = HI_FALSE;
                                    }
                                }
                                break;
                            }
                        case PVR_IR_PLAY_PAUSE:
                            {
                                if (HI_FALSE == g_stPlayChnAttr[u32ChanNum].bPlayBackPaused)
                                {
                                    s32Ret = HI_UNF_PVR_PlayPauseChn(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn);
                                    if (s32Ret != HI_SUCCESS)
                                    {
                                        sample_common_printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
                                    }

                                    g_stPlayChnAttr[u32ChanNum].bPlayBackPaused = HI_TRUE;
                                }
                                else
                                {
                                    g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                    g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                    g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes = 0;
                                    s32Ret = HI_UNF_PVR_PlayResumeChn(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn);
                                    if (s32Ret != HI_SUCCESS)
                                    {
                                        sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                                    }

                                    g_stPlayChnAttr[u32ChanNum].bPlayBackPaused = HI_FALSE;
                                }
                                break;
                            }
                        case PVR_IR_PGUP://seek backward 5s, PgUp button
                            {
                                HI_UNF_PVR_PLAY_POSITION_S stPlayPos;

                                g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                s32Ret = HI_UNF_PVR_PlayResumeChn(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                                }

                                stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                                stPlayPos.s64Offset = -(HI_S64)5000;
                                stPlayPos.s32Whence = SEEK_CUR;
                                s32Ret = HI_UNF_PVR_PlaySeek(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayPos);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayStep failed.\n");
                                }

                                break;
                            }
                        case PVR_IR_PGDOWN://seek forward 5s, PgDown
                            {
                                HI_UNF_PVR_PLAY_POSITION_S stPlayPos;

                                g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                s32Ret = HI_UNF_PVR_PlayResumeChn(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                                }

                                stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                                stPlayPos.s64Offset = (HI_S64)5000;
                                stPlayPos.s32Whence = SEEK_CUR;
                                s32Ret = HI_UNF_PVR_PlaySeek(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayPos);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayStep failed.\n");
                                }
                                break;
                            }
                        case PVR_IR_REW://fast backward play, REW button
                            {
                                HI_UNF_PVR_PLAY_MODE_S stPlayTrickMode;

                                g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes %= 6;
                                stPlayTrickMode.enSpeed = -(0x1 << (g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes + 1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;

                                s32Ret = HI_UNF_PVR_PlayTPlay(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayTrickMode);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                                }

                                g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes++;
                                g_stPlayChnAttr[u32ChanNum].bPlayBackPaused = HI_TRUE;

                                break;
                            }
                        case PVR_IR_FF://fast forward play, FF button
                            {
                                HI_UNF_PVR_PLAY_MODE_S stPlayTrickMode;

                                g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes %= 6;
                                stPlayTrickMode.enSpeed = (0x1 << (g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes + 1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;

                                s32Ret = HI_UNF_PVR_PlayTPlay(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayTrickMode);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                                }

                                g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes++;
                                g_stPlayChnAttr[u32ChanNum].bPlayBackPaused = HI_TRUE;

                                break;
                            }
                        case PVR_IR_BACK://slow forward play, Back button
                            {
                                HI_UNF_PVR_PLAY_MODE_S stPlayTrickMode;

                                g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;

                                g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes %= 6;
                                stPlayTrickMode.enSpeed = HI_UNF_PVR_PLAY_SPEED_NORMAL / (0x1 << (g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes + 1));

                                s32Ret = HI_UNF_PVR_PlayTPlay(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayTrickMode);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                                }

                                g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes++;
                                g_stPlayChnAttr[u32ChanNum].bPlayBackPaused = HI_TRUE;

                                break;
                            }
                        case PVR_IR_STOP://step play, Stop button
                            {
                                g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                                g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                                g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                                s32Ret = HI_UNF_PVR_PlayStep(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, 1);
                                if (s32Ret != HI_SUCCESS)
                                {
                                    sample_common_printf("call HI_UNF_PVR_PlayStep failed.\n");
                                }

                                g_stPlayChnAttr[u32ChanNum].bPlayBackPaused = HI_TRUE;

                                break;
                            }
                        case PVR_IR_MUTE://mute a play channel, Mute button
                            {
                                if (HI_FALSE == g_stPlayChnAttr[u32ChanNum].bMuteSeted)
                                {
                                    s32Ret = HI_UNF_SND_SetTrackMute(g_stPlayChnAttr[u32ChanNum].hSoundTrack, HI_TRUE);
                                    if (HI_SUCCESS != s32Ret)
                                    {
                                        sample_common_printf("Set mute fail! track=%#x ret=%#x\n", g_stPlayChnAttr[u32ChanNum].hSoundTrack, s32Ret);
                                        break;
                                    }

                                    g_stPlayChnAttr[u32ChanNum].bMuteSeted = HI_TRUE;
                                }
                                else
                                {
                                    s32Ret = HI_UNF_SND_SetTrackMute(g_stPlayChnAttr[u32ChanNum].hSoundTrack, HI_FALSE);
                                    if (HI_SUCCESS != s32Ret)
                                    {
                                        sample_common_printf("Enable audio fail! track=%#x ret=%#x\n", g_stPlayChnAttr[u32ChanNum].hSoundTrack, s32Ret);
                                        break;
                                    }

                                    g_stPlayChnAttr[u32ChanNum].bMuteSeted = HI_FALSE;
                                }

                                if (HI_TRUE == g_stPlayChnAttr[u32ChanNum].bFullScreen)
                                {
                                    HI_RECT stMuteRect;

                                    #if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
                                    stMuteRect.x = 50;
                                    stMuteRect.y = 10;
                                    stMuteRect.w = 40;
                                    stMuteRect.h = 25;
                                    #else
                                    stMuteRect.x = 50;
                                    stMuteRect.y = 50;
                                    stMuteRect.w = 60;
                                    stMuteRect.h = 40;
                                    #endif

                                    PVR_DrawMuteText(&stMuteRect, g_stPlayChnAttr[u32ChanNum].bMuteSeted);
                                }
                                else
                                {
                                    PVR_DrawMuteText(&g_stPlayChnAttr[u32ChanNum].stMuteTextRect,
                                                       g_stPlayChnAttr[u32ChanNum].bMuteSeted);
                                }
                                break;
                            }
                        default:
                            break;

                    };
                }
                break;
            case PVR_TEST_MODE_RECORDING:
                break;
            default:
                break;
        };

        usleep(50000);
    }

    return HI_NULL;
}


HI_S32 PVR_CreateAvplay(HI_U32 u32ChnNum, HI_HANDLE hWindow, HI_HANDLE *phSndTrack, HI_HANDLE *phAvplay, HI_U32 u32DmxId)
{
    HI_S32                  Ret;
    HI_HANDLE               hAvplay;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;

    Ret = HIADP_AVPlay_RegADecLib();
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HIADP_AVPlay_RegADecLib failed.\n");
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_AVPLAY_GetDefaultConfig failed.\n");
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    AvplayAttr.u32DemuxId = u32DmxId;

    AvplayAttr.stStreamAttr.u32VidBufSize = 2*1024*1024;
    AvplayAttr.stStreamAttr.u32AudBufSize = 192*1024;

    Ret = HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_AVPLAY_Create failed.\n");
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_AVPLAY_GetAttr failed.\n");
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    SyncAttr.stSyncStartRegion.s32VidPlusTime = 60;
    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -20;
    SyncAttr.u32PreSyncTimeoutMs = 1000;
    SyncAttr.bQuickOutput = HI_FALSE;

    Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        return Ret;
    }

    Ret = HI_UNF_VO_AttachWindow(hWindow, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_VO_AttachWindow failed.\n");
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_VO_SetWindowEnable(hWindow, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        HI_UNF_VO_DetachWindow(hWindow, hAvplay);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    /*Master track*/
    if (0 == u32ChnNum)
    {
        Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    }
    else
    {
        stTrackAttr.enTrackType = HI_UNF_SND_TRACK_TYPE_SLAVE;
        Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_SLAVE, &stTrackAttr);
    }

    if (Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        return Ret;
    }
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, phSndTrack);
    if (Ret != HI_SUCCESS)
    {
        HI_UNF_VO_SetWindowEnable(hWindow, HI_FALSE);
        HI_UNF_VO_DetachWindow(hWindow, hAvplay);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_SND_Attach(*phSndTrack, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_UNF_SND_DestroyTrack(*phSndTrack);
        sample_common_printf("call HI_SND_Attach failed.\n");
        HI_UNF_VO_SetWindowEnable(hWindow, HI_FALSE);
        HI_UNF_VO_DetachWindow(hWindow, hAvplay);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    *phAvplay = hAvplay;
    return HI_SUCCESS;
}

HI_S32 PVR_DestroyAvplay(HI_HANDLE hAvplay, HI_HANDLE hWin, HI_HANDLE hSoundTrack)
{
    HI_S32                      Ret;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;

    Ret = HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
    if (Ret != HI_SUCCESS )
    {
        sample_common_printf("call HI_UNF_AVPLAY_Stop failed.\n");
    }

    HI_UNF_VO_SetWindowEnable(hWin,HI_FALSE);

    Ret = HI_UNF_SND_Detach(hSoundTrack, hAvplay);
    if (Ret != HI_SUCCESS )
    {
        sample_common_printf("call HI_UNF_SND_Detach failed.\n");
    }

    Ret = HI_UNF_SND_DestroyTrack(hSoundTrack);
    if (Ret != HI_SUCCESS )
    {
        sample_common_printf("call HI_UNF_SND_DestroyTrack failed.\n");
    }

    Ret = HI_UNF_VO_DetachWindow(hWin, hAvplay);
    if (Ret != HI_SUCCESS )
    {
        sample_common_printf("call HI_UNF_VO_DetachWindow failed.\n");
    }

    Ret = HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if (Ret != HI_SUCCESS )
    {
        sample_common_printf("call HI_UNF_AVPLAY_ChnClose failed.\n");
    }

    Ret = HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    if (Ret != HI_SUCCESS )
    {
        sample_common_printf("call HI_UNF_AVPLAY_ChnClose failed.\n");
    }

    Ret = HI_UNF_AVPLAY_Destroy(hAvplay);
    if (Ret != HI_SUCCESS )
    {
        sample_common_printf("call HI_UNF_AVPLAY_Destroy failed.\n");
    }

    return HI_SUCCESS;
}


HI_S32 PVR_StartPlay(const HI_CHAR *pszFileName,
                          HI_U32 u32ChnNum,
                          HI_U32 u32DmxId,
                          HI_HANDLE hAvplay,
                          HI_HANDLE *phPlaybackTsbuff,
                          HI_U32 *pu32PvrPlayChn,
                          HI_BOOL *pbVedioExist)
{
    HI_S32 ret;
    HI_U32 playChn;
    PVR_PROG_INFO_S        stFileInfo;
    HI_UNF_PVR_PLAY_ATTR_S stAttr;
    PMT_COMPACT_TBL        *pPmttal;
    HI_U32 pid = 0;
    HI_UNF_AVPLAY_MEDIA_CHAN_E enMediaType = 0;
    SAMPLE_CheckNullPTR(pszFileName);
    HI_UNF_AVPLAY_FRMRATE_PARAM_S stFrmRateAttr = {0};
    HI_UNF_SYNC_ATTR_S stSyncAttr = {0};
    HI_UNF_AVPLAY_TPLAY_OPT_S stTplayOpts = {0};
    HI_CODEC_VIDEO_CMD_S  stVdecCmdPara = {0};
    PMT_COMPACT_PROG stProgInfoTmp;

    SAMPLE_RUN(PVR_GetPorgInfo(&stFileInfo, pszFileName), ret);
    if (HI_SUCCESS != ret)
    {
        PRINT_SMP("Can NOT get prog INFO, can't play.\n");
        PVR_SearchFile(u32DmxId,
                         (HI_UNF_DMX_PORT_RAM_0 + u32ChnNum),
                         pszFileName,
                         &pPmttal);
        PVR_SetAvplayPidAndCodecType(hAvplay, &(pPmttal->proginfo[0]));

        memcpy(stAttr.szFileName, pszFileName, strlen(pszFileName) + 1);
        stAttr.u32FileNameLen = strlen(pszFileName);
        stAttr.enStreamType = HI_UNF_PVR_STREAM_TYPE_TS;
        stAttr.bIsClearStream = HI_TRUE;
    }
    else
    {
        stProgInfoTmp.ProgID = stFileInfo.ProgID;
        stProgInfoTmp.PmtPid = stFileInfo.PmtPid;
        stProgInfoTmp.PcrPid = stFileInfo.PcrPid;
        stProgInfoTmp.VideoType = stFileInfo.VideoType;
        stProgInfoTmp.VElementNum = stFileInfo.VElementNum;
        stProgInfoTmp.VElementPid = stFileInfo.VElementPid;
        stProgInfoTmp.AudioType = stFileInfo.AudioType;
        stProgInfoTmp.AElementNum = stFileInfo.AElementNum;
        stProgInfoTmp.AElementPid = stFileInfo.AElementPid;
        memcpy(stProgInfoTmp.Audioinfo, stFileInfo.Audioinfo, PROG_MAX_AUDIO*sizeof(PMT_AUDIO));
        stProgInfoTmp.SubtType = stFileInfo.SubtType;
        stProgInfoTmp.u16SubtitlingNum = stFileInfo.u16SubtitlingNum;
        memcpy(stProgInfoTmp.SubtitingInfo, stFileInfo.SubtitingInfo, SUBTITLING_MAX*sizeof(PMT_SUBTITLE));
        memcpy(&(stProgInfoTmp.stSCTESubtInfo), &(stFileInfo.stSCTESubtInfo), sizeof(PMT_SCTE_SUBTITLE_S));
        stProgInfoTmp.u16ClosedCaptionNum = stFileInfo.u16ClosedCaptionNum;
        memcpy(stProgInfoTmp.stClosedCaption, stFileInfo.stClosedCaption, CAPTION_SERVICE_MAX*sizeof(PMT_CLOSED_CAPTION_S));
        stProgInfoTmp.u16ARIBCCPid = stFileInfo.u16ARIBCCPid;
        stProgInfoTmp.u16TtxNum = stFileInfo.u16TtxNum;
        memcpy(stProgInfoTmp.stTtxInfo, stFileInfo.stTtxInfo, TTX_MAX*sizeof(PMT_TTX_S));

        PVR_SetAvplayPidAndCodecType(hAvplay, &(stProgInfoTmp));

        memcpy(stAttr.szFileName, pszFileName, strlen(pszFileName) + 1);
        stAttr.u32FileNameLen = strlen(pszFileName);
        stAttr.enStreamType = stFileInfo.stRecAttr.enStreamType;
        stAttr.bIsClearStream = stFileInfo.stRecAttr.bIsClearStream;
    }

    if ((1 == stFileInfo.stRecAttr.stEncryptCfg.bDoCipher) && (ret == HI_SUCCESS))
    {
        memcpy(&(stAttr.stDecryptCfg), &(stFileInfo.stRecAttr.stEncryptCfg), sizeof(HI_UNF_PVR_CIPHER_S));

        sample_common_printf("{%s_%d]cipher info:\n", __FILE__, __LINE__);
        sample_common_printf("[%s_%d]  enType:%d\n", __FILE__, __LINE__, stAttr.stDecryptCfg.enType);
    }
    else
    {
        sample_common_printf("[%s_%d]cipher info: not encrypt\n", __FILE__, __LINE__);
        stAttr.stDecryptCfg.bDoCipher = HI_FALSE;
    }

    ret = HI_UNF_DMX_AttachTSPort(u32DmxId, (HI_UNF_DMX_PORT_RAM_0 + u32ChnNum));
    if (ret != HI_SUCCESS)
    {
        sample_common_printf("[%s_%d]call HI_UNF_DMX_AttachTSPort failed.\n", __FILE__, __LINE__);
        HI_UNF_DMX_DeInit();
        return ret;
    }

    SAMPLE_RUN(HI_UNF_DMX_CreateTSBuffer((HI_UNF_DMX_PORT_RAM_0 + u32ChnNum), 
                                              PVR_DEMUX_BUFF_SIZE, 
                                              phPlaybackTsbuff), 
                                              ret);
    if (ret != HI_SUCCESS)
    {
        HI_UNF_DMX_DetachTSPort(u32DmxId);
        HI_UNF_DMX_DeInit();
        return ret;
    }

    /*create new play channel*//*CNcomment:申请新的播放通道*/
    SAMPLE_RUN(HI_UNF_PVR_PlayCreateChn(&playChn, &stAttr, hAvplay, *phPlaybackTsbuff), ret);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &pid);
    if ((HI_SUCCESS != ret) || (0x1fff == pid))
    {
        sample_common_printf("[%s_%d]has no audio stream!\n", __FILE__, __LINE__);
    }
    else
    {
        enMediaType |= HI_UNF_AVPLAY_MEDIA_CHAN_AUD;
    }

    ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VID_PID, &pid);
    if ((HI_SUCCESS != ret) || (0x1fff == pid))
    {
        sample_common_printf("[%s_%d]has no video stream!\n", __FILE__, __LINE__);
        *pbVedioExist = HI_FALSE;
    }
    else
    {
        enMediaType |= HI_UNF_AVPLAY_MEDIA_CHAN_VID;
        *pbVedioExist = HI_TRUE;
    }

    /*enable avplay A/V sync*//*CNcomment:使能avplay音视频同步*/
    if (HI_SUCCESS != HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr))
    {
        sample_common_printf("[%s_%d]get avplay sync attr fail!\n", __FILE__, __LINE__);
        return HI_FAILURE;
    }
    stSyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    stSyncAttr.stSyncStartRegion.s32VidPlusTime = 60;
    stSyncAttr.stSyncStartRegion.s32VidNegativeTime = -20;
    stSyncAttr.u32PreSyncTimeoutMs = 1000;
    stSyncAttr.bQuickOutput = HI_FALSE;

    if (HI_SUCCESS != HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr))
    {
        sample_common_printf("[%s_%d]set avplay sync attr fail!\n", __FILE__, __LINE__);
        return HI_FAILURE;
    }

    SAMPLE_RUN(HI_UNF_PVR_PlayStartChn(playChn), ret);
    if (HI_SUCCESS != ret)
    {
        HI_UNF_PVR_PlayDestroyChn(playChn);
        return ret;
    }

    /*enable vo frame rate detect*//*CNcomment:使能VO自动帧率检测*/
    stFrmRateAttr.enFrmRateType = HI_UNF_AVPLAY_FRMRATE_TYPE_PTS;
    stFrmRateAttr.stSetFrmRate.u32fpsInteger = 0;
    stFrmRateAttr.stSetFrmRate.u32fpsDecimal = 0;
    if (HI_SUCCESS != HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_FRMRATE_PARAM, &stFrmRateAttr))
    {
        sample_common_printf("[%s_%d]set frame to VO fail.\n", __FILE__, __LINE__);
        return HI_FAILURE;
    }

    /*set avplay trick mode to normal*//*CNcomment:设置avplay特技模式为正常*/
    if (enMediaType & HI_UNF_AVPLAY_MEDIA_CHAN_VID)
    {
        stTplayOpts.enTplayDirect = HI_UNF_AVPLAY_TPLAY_DIRECT_FORWARD;
        stTplayOpts.u32SpeedInteger = 1;
        stTplayOpts.u32SpeedDecimal = 0;
        stVdecCmdPara.u32CmdID = HI_UNF_AVPLAY_SET_TPLAY_PARA_CMD;
        stVdecCmdPara.pPara = &stTplayOpts;
        ret = HI_UNF_AVPLAY_Invoke(hAvplay, HI_UNF_AVPLAY_INVOKE_VCODEC, (void *)&stVdecCmdPara);
        if (HI_SUCCESS != ret)
        {
            sample_common_printf("[%s_%d]Resume Avplay trick mode to normal fail.\n", __FILE__, __LINE__);
            return HI_FAILURE;
        }
    }

    *pu32PvrPlayChn = playChn;
    return HI_SUCCESS;
}

HI_VOID PVR_StopPlay(HI_U32 playChn, HI_U32 u32DmxId, HI_HANDLE hDmxTsBuf)
{
    HI_UNF_AVPLAY_STOP_OPT_S stopOpt;

    stopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    stopOpt.u32TimeoutMs = 0;
    HI_UNF_PVR_PlayStopChn(playChn, &stopOpt);
    HI_UNF_PVR_PlayDestroyChn(playChn);

    HI_UNF_DMX_DestroyTSBuffer(hDmxTsBuf);
    HI_UNF_DMX_DetachTSPort(u32DmxId);
}

HI_S32 PVR_StartLive(HI_U32 u32ProgramNum, HI_U32 u32DmxId, HI_HANDLE hAvplay, HI_BOOL *pbVideoExist, HI_U32 u32DmxPort)
{
    HI_U32 s32Ret = HI_SUCCESS;
    HI_U32 pid = 0;
    HI_UNF_AVPLAY_MEDIA_CHAN_E enMediaType = 0;
    HI_UNF_AVPLAY_FRMRATE_PARAM_S stFrmRateAttr = {0};
    HI_UNF_SYNC_ATTR_S stSyncAttr = {0};
    HI_CODEC_VIDEO_CMD_S  stVdecCmdPara = {0};
    HI_UNF_AVPLAY_TPLAY_OPT_S stTplayOpts = {0};
    PMT_COMPACT_PROG *pstProg = HI_NULL;

    //pstTimeShiftChnAttr->u32PlayDemuxId = DEMUX_ID_0 + pstTimeShiftChnAttr->hPlayChn;

    if((DEMUX_ID_0 != u32DmxId) || (HI_UNF_DMX_PORT_RAM_4 == u32DmxPort))
    {
        s32Ret = HI_UNF_DMX_AttachTSPort(u32DmxId, u32DmxPort);
        if (s32Ret != HI_SUCCESS)
        {
            sample_common_printf("call HI_UNF_DMX_AttachTSPort failed. ret=%#x dmxid=%d port=%d\n", s32Ret, u32DmxId, u32DmxPort);
            return -1;
        }
    }

    pstProg = g_pstProgTbl->proginfo + ((u32ProgramNum-1) % g_pstProgTbl->prog_num);

    PVR_SetAvplayPidAndCodecType(hAvplay, pstProg);

    s32Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &pid);
    if ((HI_SUCCESS != s32Ret) || (0x1fff == pid))
    {
        sample_common_printf("has no audio stream!\n");
    }
    else
    {
        enMediaType |= HI_UNF_AVPLAY_MEDIA_CHAN_AUD;
    }

    s32Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VID_PID, &pid);
    if ((HI_SUCCESS != s32Ret) || (0x1fff == pid))
    {
        sample_common_printf("has no video stream!\n");
        *pbVideoExist = HI_FALSE;
    }
    else
    {
        enMediaType |= HI_UNF_AVPLAY_MEDIA_CHAN_VID;
        *pbVideoExist = HI_TRUE;
    }

    if ((enMediaType & HI_UNF_AVPLAY_MEDIA_CHAN_AUD) && (enMediaType & HI_UNF_AVPLAY_MEDIA_CHAN_VID))
    {
        /*enable vo frame rate detect*//*CNcomment:使能VO自动帧率检测*/
        stFrmRateAttr.enFrmRateType = HI_UNF_AVPLAY_FRMRATE_TYPE_PTS;
        stFrmRateAttr.stSetFrmRate.u32fpsInteger = 0;
        stFrmRateAttr.stSetFrmRate.u32fpsDecimal = 0;
        if (HI_SUCCESS != HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_FRMRATE_PARAM, &stFrmRateAttr))
        {
            sample_common_printf("set frame to VO fail.\n");
            return -1;
        }

        /*enable avplay A/V sync*//*CNcomment:使能avplay音视频同步*/
        if (HI_SUCCESS != HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr))
        {
            sample_common_printf("get avplay sync attr fail!\n");
            return -1;
        }

        stSyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
        stSyncAttr.stSyncStartRegion.s32VidPlusTime = 60;
        stSyncAttr.stSyncStartRegion.s32VidNegativeTime = -20;
        stSyncAttr.u32PreSyncTimeoutMs = 1000;
        stSyncAttr.bQuickOutput = HI_FALSE;

        if (HI_SUCCESS != HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr))
        {
            sample_common_printf("set avplay sync attr fail!\n");
            return -1;
        }
    }

    /*start to play audio and video*//*CNcomment:开始音视频播放*/
    SAMPLE_RUN(HI_UNF_AVPLAY_Start(hAvplay, enMediaType, NULL), s32Ret);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    /*set avplay trick mode to normal*//*CNcomment:设置avplay特技模式为正常*/
    if (enMediaType & HI_UNF_AVPLAY_MEDIA_CHAN_VID)
    {
        stTplayOpts.enTplayDirect = HI_UNF_AVPLAY_TPLAY_DIRECT_FORWARD;
        stTplayOpts.u32SpeedInteger = 1;
        stTplayOpts.u32SpeedDecimal = 0;
        stVdecCmdPara.u32CmdID = HI_UNF_AVPLAY_SET_TPLAY_PARA_CMD;
        stVdecCmdPara.pPara = &stTplayOpts;
        s32Ret = HI_UNF_AVPLAY_Invoke(hAvplay, HI_UNF_AVPLAY_INVOKE_VCODEC, (void *)&stVdecCmdPara);
        if (HI_SUCCESS != s32Ret)
        {
            sample_common_printf("Resume Avplay trick mode to normal fail.\n");
            return -1;
        }
    }

    return 0;
}

HI_S32 PVR_StopLive(HI_HANDLE hAvplay)
{
    HI_UNF_AVPLAY_STOP_OPT_S stStopOption;

    stStopOption.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    stStopOption.u32TimeoutMs = 0;

    sample_common_printf("stop live play ...\n");

    /*stop playing audio and video*//*CNcomment:停止音视频设备*/
    return HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &stStopOption);
}


HI_S32 PVR_StartRec(const HI_CHAR *path,
                           PMT_COMPACT_PROG *pstProgInfo,
                           HI_U32 u32DemuxID,
                           HI_BOOL bRewind,
                           HI_BOOL bDoCipher,
                           HI_U64 u64MaxSize,
                           HI_U64 u64MaxTimeMs,
                           HI_U32 *pu32RecChn)
{
    HI_U32 u32RecChn;
    HI_S32 ret = HI_SUCCESS;
    HI_U32                u32VidPid;
    HI_U32                u32AudPid = 0;
    HI_UNF_PVR_REC_ATTR_S stAttr;
    PVR_PROG_INFO_S       stFileInfo;
    HI_CHAR               szFileName[PVR_MAX_FILENAME_LEN];

    stAttr.u32DemuxID    = u32DemuxID;

    if (pstProgInfo->VElementNum > 0)
    {
        stAttr.u32IndexPid   = pstProgInfo->VElementPid;
        stAttr.enIndexType   = HI_UNF_PVR_REC_INDEX_TYPE_VIDEO;
        stAttr.enIndexVidType = pstProgInfo->VideoType;
    }
    else
    {
        stAttr.u32IndexPid   = pstProgInfo->AElementPid;
        stAttr.enIndexType   = HI_UNF_PVR_REC_INDEX_TYPE_AUDIO;
        stAttr.enIndexVidType = HI_UNF_VCODEC_TYPE_BUTT;
    }

    sprintf(szFileName, "rec_v%d_a%d.ts",
                        pstProgInfo->VElementPid,
                        pstProgInfo->AElementPid);

    sprintf(stAttr.szFileName, "%s/", path);

    strcat(stAttr.szFileName, szFileName);

    stAttr.u32FileNameLen = strlen(stAttr.szFileName);
    stAttr.u32ScdBufSize = PVR_STUB_SC_BUF_SZIE;
    stAttr.u32DavBufSize = PVR_STUB_TSDATA_SIZE;
    stAttr.enStreamType  = HI_UNF_PVR_STREAM_TYPE_TS;
    stAttr.bRewind = bRewind;
    stAttr.u64MaxFileSize= u64MaxSize;//source;
    stAttr.u64MaxTimeInMs= u64MaxTimeMs;
    stAttr.bIsClearStream = HI_TRUE;
    stAttr.u32UsrDataInfoSize = sizeof(PVR_PROG_INFO_S) + 100;/*the one in index file is a multipleit of 40 bytes*//*CNcomment:索引文件里是40个字节对齐的*/

    stAttr.stEncryptCfg.bDoCipher = bDoCipher;
    stAttr.stEncryptCfg.enType = HI_UNF_CIPHER_ALG_AES;
    stAttr.stEncryptCfg.u32KeyLen = 16;          /*strlen(PVR_CIPHER_KEY)*/
    SAMPLE_RUN(HI_UNF_PVR_RecCreateChn(&u32RecChn, &stAttr), ret);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    SAMPLE_RUN(HI_UNF_PVR_RecAddPID(u32RecChn, 0), ret);
    SAMPLE_RUN(HI_UNF_PVR_RecAddPID(u32RecChn, pstProgInfo->PmtPid), ret);
    if (pstProgInfo->AElementNum > 0)
    {
        u32AudPid = pstProgInfo->AElementPid;
        SAMPLE_RUN(HI_UNF_PVR_RecAddPID(u32RecChn, u32AudPid), ret);
    }

    if (pstProgInfo->VElementNum > 0)
    {
        u32VidPid = pstProgInfo->VElementPid;
        SAMPLE_RUN(HI_UNF_PVR_RecAddPID(u32RecChn, u32VidPid), ret);
    }

    SAMPLE_RUN(HI_UNF_PVR_RecStartChn(u32RecChn), ret);
    if (HI_SUCCESS != ret)
    {
        HI_UNF_PVR_RecDestroyChn(u32RecChn);
        return ret;
    }

    stFileInfo.ProgID = pstProgInfo->ProgID;
    stFileInfo.PmtPid = pstProgInfo->PmtPid;
    stFileInfo.PcrPid = pstProgInfo->PcrPid;
    stFileInfo.VideoType = pstProgInfo->VideoType;
    stFileInfo.VElementNum = pstProgInfo->VElementNum;
    stFileInfo.VElementPid = pstProgInfo->VElementPid;
    stFileInfo.AudioType = pstProgInfo->AudioType;
    stFileInfo.AElementNum = pstProgInfo->AElementNum;
    stFileInfo.AElementPid = pstProgInfo->AElementPid;
    memcpy(stFileInfo.Audioinfo, pstProgInfo->Audioinfo, PROG_MAX_AUDIO*sizeof(PMT_AUDIO));
    stFileInfo.SubtType = pstProgInfo->SubtType;
    stFileInfo.u16SubtitlingNum = pstProgInfo->u16SubtitlingNum;
    memcpy(stFileInfo.SubtitingInfo, pstProgInfo->SubtitingInfo, SUBTITLING_MAX*sizeof(PMT_SUBTITLE));
    memcpy(&(stFileInfo.stSCTESubtInfo), &(pstProgInfo->stSCTESubtInfo), sizeof(PMT_SCTE_SUBTITLE_S));
    stFileInfo.u16ClosedCaptionNum = pstProgInfo->u16ClosedCaptionNum;
    memcpy(stFileInfo.stClosedCaption, pstProgInfo->stClosedCaption, CAPTION_SERVICE_MAX*sizeof(PMT_CLOSED_CAPTION_S));
    stFileInfo.u16ARIBCCPid = pstProgInfo->u16ARIBCCPid;
    stFileInfo.u16TtxNum = pstProgInfo->u16TtxNum;
    memcpy(stFileInfo.stTtxInfo, pstProgInfo->stTtxInfo, TTX_MAX*sizeof(PMT_TTX_S));

    memcpy(&(stFileInfo.stRecAttr), &stAttr, sizeof(stFileInfo.stRecAttr));

    SAMPLE_RUN(PVR_SavePorgInfo(&stFileInfo, stAttr.szFileName), ret);
    if (HI_SUCCESS != ret)
    {
        HI_UNF_PVR_RecStopChn(u32RecChn);
        HI_UNF_PVR_RecDestroyChn(u32RecChn);
        return ret;
    }

    *pu32RecChn = u32RecChn;

    return HI_SUCCESS;
}

HI_S32 PVR_StopRec(HI_U32 u32RecChn, HI_U32 u32DmxId)
{
    HI_S32 s32Ret = 0;
    HI_UNF_PVR_REC_ATTR_S stRecAttr;

    s32Ret = HI_UNF_PVR_RecGetChn(u32RecChn, &stRecAttr);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("Get rec chn stAttr fail! ret=%#x\n", s32Ret);
    }

    s32Ret = HI_UNF_PVR_RecStopChn(u32RecChn);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("Stop chan %d recording fail! ret=%#x\n", u32RecChn, s32Ret);
    }

    (HI_VOID)HI_UNF_PVR_RecDelAllPID(u32RecChn);

    s32Ret = HI_UNF_PVR_RecDestroyChn(u32RecChn);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("Destroy chn %d fail! ret=%#x\n", u32RecChn, s32Ret);
    }

    return s32Ret;
}


HI_S32 PVR_AddNewWindow(HI_U32 u32ChnNum, HI_RECT_S *pstRect, HI_HANDLE *phWindow, HI_U32 *pu32WinZorder)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32WinZorder = 0;

    pstRect->s32Width = PVR_WIN_WIDTH;
    pstRect->s32Height = PVR_WIN_HEIGHT;
    pstRect->s32X = u32ChnNum * PVR_WIN_WIDTH + 64;
    pstRect->s32Y = 3*PVR_WIN_HEIGHT - 32;

    s32Ret = HIADP_VO_CreatWin(pstRect, phWindow);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("call HIADP_VO_CreatWin failed. ret=%#x\n", s32Ret);
        return -1;
    }

    s32Ret = HI_UNF_VO_GetWindowZorder(*phWindow, &u32WinZorder);
    if(HI_SUCCESS != s32Ret)
    {
        sample_common_printf("Get window Z order fail! window handle=%d ret=%d\n",*phWindow, s32Ret);
        return -1;
    }

    *pu32WinZorder = u32WinZorder;

    if (0 == u32ChnNum)
    {
        (void)pthread_mutex_lock(&g_stDrawRectMutex);
        PVR_DrawSelectRect(pstRect, HI_TRUE, HI_TRUE);
        (void)pthread_mutex_unlock(&g_stDrawRectMutex);
    }

    return 0;
}

HI_S32 PVR_AddAPlayBack(const HI_CHAR *pcFileName)
{
    HI_S32 s32Ret = 0;
    PVR_PLAY_CHN_ATTR *pstPlayChnAttr;

    if(g_PlayChn > (PVR_MAX_PLAY_CHN -1))
    {
        sample_common_printf("Only support %d play channels\n", PVR_MAX_PLAY_CHN);
        return -1;
    }

    pstPlayChnAttr = &g_stPlayChnAttr[g_PlayChn];

    memset(pstPlayChnAttr, 0, sizeof(PVR_PLAY_CHN_ATTR));
    pstPlayChnAttr->bMuteSeted = HI_FALSE;
    pstPlayChnAttr->hWindow = -1;
    pstPlayChnAttr->hPlayChn = g_PlayChn;
    pstPlayChnAttr->u32DemuxId = DEMUX_ID_0 + pstPlayChnAttr->hPlayChn;

    s32Ret = PVR_AddNewWindow(pstPlayChnAttr->hPlayChn, &pstPlayChnAttr->stRect, &pstPlayChnAttr->hWindow, &pstPlayChnAttr->u32WindowZorder);
    if(0 != s32Ret)
    {
        sample_common_printf("Add a play window fail!\n");
        return -1;
    }

    pstPlayChnAttr->stMuteTextRect.x = pstPlayChnAttr->stRect.s32X;
    pstPlayChnAttr->stMuteTextRect.y = pstPlayChnAttr->stRect.s32Y - 50;
    pstPlayChnAttr->stMuteTextRect.w = 60;
    pstPlayChnAttr->stMuteTextRect.h = 40;

    s32Ret = PVR_CreateAvplay(pstPlayChnAttr->hPlayChn,
                              pstPlayChnAttr->hWindow,
                              &pstPlayChnAttr->hSoundTrack,
                              &pstPlayChnAttr->hAvplay,
                              pstPlayChnAttr->u32DemuxId);
    if(0 != s32Ret)
    {
        sample_common_printf("Init avplay fail! playchn=%d\n", pstPlayChnAttr->hPlayChn);
        return -1;
    }

    s32Ret = PVR_StartPlay(pcFileName,
                             pstPlayChnAttr->hPlayChn,
                             pstPlayChnAttr->u32DemuxId,
                             pstPlayChnAttr->hAvplay,
                             &pstPlayChnAttr->hTsBufForPlayback,
                             &pstPlayChnAttr->u32PvrPlayChn,
                             &pstPlayChnAttr->bVideoExist);
    if(0 != s32Ret)
    {
        sample_common_printf("Start pvr play fail! playchn=%d filename=%s\n", pstPlayChnAttr->hPlayChn, pcFileName);
        return -1;
    }

    /*No vedio, display hisilicon logo*/
    if(HI_FALSE == pstPlayChnAttr->bVideoExist)
    {
        (void)pthread_mutex_lock(&g_stDrawRectMutex);
        PVR_DrawGraphic(g_aHisiliconLogoPath, &pstPlayChnAttr->stRect);
        (void)pthread_mutex_unlock(&g_stDrawRectMutex);
    }

    pstPlayChnAttr->bPlayChnInited = HI_TRUE;

    g_PlayChn++;

    return 0;
}


HI_S32 PVR_LoadCW(HI_HANDLE hAvplay)
{
    HI_S32 Ret;
    HI_HANDLE hDmxVidChn,hDmxAudChn;
    HI_HANDLE hDescrambler,hCaDesc;
    HI_UNF_DMX_DESCRAMBLER_ATTR_S stNorcaDescAttr;
    if ( g_hNorcaDesc == HI_NULL )
    {
        memset(&stNorcaDescAttr,0,sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));
        stNorcaDescAttr.enCaType = HI_UNF_DMX_CA_NORMAL;
        stNorcaDescAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_CSA2;
        Ret = HI_UNF_DMX_CreateDescramblerExt(0,&stNorcaDescAttr,&g_hNorcaDesc);
        if (HI_SUCCESS != Ret)
        {
            g_hNorcaDesc = HI_NULL;
        }
    }
    hCaDesc = g_hNorcaDesc;
    if (HI_NULL == hCaDesc)
    {
        sample_common_printf("Unsupported CwType\n");
        return HI_FAILURE;
    }
    Ret = HI_UNF_AVPLAY_GetDmxVidChnHandle(hAvplay,&hDmxVidChn);
    Ret |= HI_UNF_AVPLAY_GetDmxAudChnHandle(hAvplay,&hDmxAudChn);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("HI_UNF_AVPLAY_GetDmxChnHandle failed:%x\n",Ret);
        return Ret;
    }
    Ret = HI_UNF_DMX_GetDescramblerKeyHandle(hDmxVidChn,&hDescrambler);
    if (HI_SUCCESS != Ret)
    {
        if(HI_ERR_DMX_NOATTACH_KEY == Ret)
        {
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxVidChn);
        }
        else
        {
            sample_common_printf("HI_UNF_DMX_GetDescramblerKeyHandle failed:%x\n",Ret);
            return Ret;
        }
    }
    else
    {
        if ((hDescrambler != hCaDesc))
        {
            (HI_VOID)HI_UNF_DMX_DetachDescrambler(hDescrambler,hDmxVidChn);
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxVidChn);
        }
    }
    Ret = HI_UNF_DMX_GetDescramblerKeyHandle(hDmxAudChn,&hDescrambler);
    if (HI_SUCCESS != Ret)
    {
        if(HI_ERR_DMX_NOATTACH_KEY == Ret)
        {
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxAudChn);
        }
        else
        {
            sample_common_printf("HI_UNF_DMX_GetDescramblerKeyHandle failed:%x\n",Ret);
            return Ret;
        }
    }
    else
    {
        if ((hDescrambler != hCaDesc))
        {
            (HI_VOID)HI_UNF_DMX_DetachDescrambler(hDescrambler,hDmxAudChn);
            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hCaDesc,hDmxAudChn);
        }
    }
    Ret = HI_UNF_DMX_SetDescramblerOddKey(hCaDesc,Odd_CW);
    Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hCaDesc,Even_CW);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("call HI_UNF_DMX_SetDescramblerKey failed\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}
HI_S32 PVR_AddALivePlay(HI_U32 u32ProgramNum)
{
    HI_S32 s32Ret = 0;
    PVR_TIMESHIFT_CHN_ATTR *pstTimeshiftChnAttr;

    if(g_TimeshiftChn > (PVR_MAX_PLAY_CHN -1))
    {
        sample_common_printf("Only support %d timeshift channels\n", PVR_MAX_PLAY_CHN);
        return -1;
    }

    pstTimeshiftChnAttr = &g_stTimeShiftChnAttr[g_TimeshiftChn];

    memset(pstTimeshiftChnAttr, 0, sizeof(PVR_PLAY_CHN_ATTR));
    pstTimeshiftChnAttr->hWindow = -1;
    pstTimeshiftChnAttr->hPlayChn = g_TimeshiftChn;
    pstTimeshiftChnAttr->u32ProgNum = u32ProgramNum;
    pstTimeshiftChnAttr->u32PlayDemuxId = DEMUX_ID_0 + pstTimeshiftChnAttr->hPlayChn;
    if(0 == g_hEthSocketRecvTsBuff)
    {
        pstTimeshiftChnAttr->u32DmxPort = PVR_DMX_PORT_ID_DVB;
    }
    else
    {
        pstTimeshiftChnAttr->u32DmxPort = HI_UNF_DMX_PORT_RAM_4;
    }

    s32Ret = PVR_AddNewWindow(pstTimeshiftChnAttr->hPlayChn, &pstTimeshiftChnAttr->stRect, &pstTimeshiftChnAttr->hWindow, &pstTimeshiftChnAttr->u32WindowZorder);
    if(0 != s32Ret)
    {
        sample_common_printf("Add a timeshift window fail!\n");
        return -1;
    }

    pstTimeshiftChnAttr->stMuteTextRect.x = pstTimeshiftChnAttr->stRect.s32X;
    pstTimeshiftChnAttr->stMuteTextRect.y = pstTimeshiftChnAttr->stRect.s32Y - 50;
    pstTimeshiftChnAttr->stMuteTextRect.w = 60;
    pstTimeshiftChnAttr->stMuteTextRect.h = 40;

    s32Ret = PVR_CreateAvplay(pstTimeshiftChnAttr->hPlayChn,
                              pstTimeshiftChnAttr->hWindow,
                              &pstTimeshiftChnAttr->hSoundTrack,
                              &pstTimeshiftChnAttr->hAvplay,
                              pstTimeshiftChnAttr->u32PlayDemuxId);
    if(0 != s32Ret)
    {
        sample_common_printf("Init avplay fail! playchn=%d\n", pstTimeshiftChnAttr->hPlayChn);
        return -1;
    }

    s32Ret = PVR_StartLive(u32ProgramNum,
                            pstTimeshiftChnAttr->u32PlayDemuxId,
                            pstTimeshiftChnAttr->hAvplay,
                            &pstTimeshiftChnAttr->bVideoExist,
                            pstTimeshiftChnAttr->u32DmxPort);
    if(0 != s32Ret)
    {
        sample_common_printf("start live play fail! playchn=%d\n", pstTimeshiftChnAttr->hPlayChn);
        return -1;
    }

    /*No vedio, display hisilicon logo*/
    if(HI_FALSE == pstTimeshiftChnAttr->bVideoExist)
    {
        (void)pthread_mutex_lock(&g_stDrawRectMutex);
        PVR_DrawGraphic(g_aHisiliconLogoPath, &pstTimeshiftChnAttr->stRect);
        (void)pthread_mutex_unlock(&g_stDrawRectMutex);
    }

    g_TimeshiftChn++;

    return 0;
}

HI_S32 PVR_AddARec(HI_U32 u32ChnNum,
                            const HI_CHAR *pcRecPath,
                            HI_U32 u32ProgramNum,
                            HI_BOOL bRewind,
                            HI_BOOL bCipher,
                            HI_U64 u64MaxRecSize,
                            HI_U64 u64MaxRecTimeMs,
                            HI_U32 *pu32RecChn,
                            PVR_TEST_MODE enTestMode)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32RecDemuxId = 0;
    PMT_COMPACT_PROG *pstProg = HI_NULL;

    u32RecDemuxId = DEMUX_ID_3 + u32ChnNum;

    if (PVR_TEST_MODE_TIMESHIFT == enTestMode)
    {
        g_stTimeShiftChnAttr[u32ChnNum].u32RecDemuxId = u32RecDemuxId;
    }

    pstProg = g_pstProgTbl->proginfo + ((u32ProgramNum-1) % g_pstProgTbl->prog_num);

    if(0 == g_hEthSocketRecvTsBuff)
    {
        s32Ret = HI_UNF_DMX_AttachTSPort(u32RecDemuxId, PVR_DMX_PORT_ID_DVB);
    }
    else
    {
        s32Ret = HI_UNF_DMX_AttachTSPort(u32RecDemuxId, HI_UNF_DMX_PORT_RAM_4);
    }

    if (s32Ret != HI_SUCCESS)
    {
        sample_common_printf("\t[%s_%d]call HI_UNF_DMX_AttachTSPort for REC failed.\n", __FILE__, __LINE__);
        HI_UNF_DMX_DeInit();
        return -1;
    }

    PVR_StartRec(pcRecPath, pstProg, u32RecDemuxId, bRewind, bCipher, u64MaxRecSize, u64MaxRecTimeMs, pu32RecChn);

    return 0;
}

HI_S32 PVR_ProcZoominScreen(HI_HANDLE hWindow)
{
    HI_S32 i = 0;
    HI_RECT stMuteRect;

#if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    stMuteRect.x = 50;
    stMuteRect.y = 10;
    stMuteRect.w = 40;
    stMuteRect.h = 25;
#else
    stMuteRect.x = 50;
    stMuteRect.y = 50;
    stMuteRect.w = 60;
    stMuteRect.h = 40;
#endif

    for(i = 0; i < PVR_MAX_PLAY_CHN; i++)
    {
        (void)pthread_mutex_lock(&g_stDrawRectMutex);

        if (-1 != g_stTimeShiftChnAttr[i].hWindow)
        {
            PVR_DrawSelectRect(&g_stTimeShiftChnAttr[i].stRect,
                                 HI_FALSE,
                                 HI_TRUE);

            PVR_DrawMuteText(&g_stTimeShiftChnAttr[i].stMuteTextRect, HI_FALSE);

            if(g_stTimeShiftChnAttr[i].hWindow == hWindow)
            {
                PVR_DrawMuteText(&stMuteRect, g_stTimeShiftChnAttr[i].bMuteSeted);
            }
        }

        if (-1 != g_stPlayChnAttr[i].hWindow)
        {
            PVR_DrawSelectRect(&g_stPlayChnAttr[i].stRect,
                                 HI_FALSE,
                                 HI_TRUE);

            PVR_DrawMuteText(&g_stPlayChnAttr[i].stMuteTextRect, HI_FALSE);

            if(g_stPlayChnAttr[i].hWindow == hWindow)
            {
                PVR_DrawMuteText(&stMuteRect, g_stPlayChnAttr[i].bMuteSeted);
            }
        }

        (void)pthread_mutex_unlock(&g_stDrawRectMutex);
    }

    return HI_SUCCESS;
}

HI_S32 PVR_ProcZoomoutScreen(HI_HANDLE hWindow)
{
    HI_S32 i = 0;
    HI_RECT stMuteRect;
#if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    stMuteRect.x = 50;
    stMuteRect.y = 10;
    stMuteRect.w = 40;
    stMuteRect.h = 25;
#else
    stMuteRect.x = 50;
    stMuteRect.y = 50;
    stMuteRect.w = 60;
    stMuteRect.h = 40;
#endif

    PVR_DrawMuteText(&stMuteRect, HI_FALSE);

    for(i = 0; i < PVR_MAX_PLAY_CHN; i++)
    {
        (void)pthread_mutex_lock(&g_stDrawRectMutex);

        if (-1 != g_stTimeShiftChnAttr[i].hWindow)
        {
            PVR_DrawSelectRect(&g_stTimeShiftChnAttr[i].stRect,
                                 HI_FALSE,
                                 g_stTimeShiftChnAttr[i].bVideoExist);

            PVR_DrawMuteText(&g_stTimeShiftChnAttr[i].stMuteTextRect, g_stTimeShiftChnAttr[i].bMuteSeted);
        }

        if (-1 != g_stPlayChnAttr[i].hWindow)
        {
            PVR_DrawSelectRect(&g_stPlayChnAttr[i].stRect,
                                 HI_FALSE,
                                 g_stPlayChnAttr[i].bVideoExist);

            PVR_DrawMuteText(&g_stPlayChnAttr[i].stMuteTextRect, g_stPlayChnAttr[i].bMuteSeted);
        }

        (void)pthread_mutex_unlock(&g_stDrawRectMutex);
    }

    return HI_SUCCESS;
}


HI_S32 PVR_ZoomInDispToFullScreen(HI_HANDLE hWindow, HI_RECT_S *pstOrgRect)
{
    HI_S32 i = 0;
    HI_S32 s32Ret = 0;
    HI_UNF_WINDOW_ATTR_S stWinAttr;

    PVR_ProcZoominScreen(hWindow);

    s32Ret = HI_UNF_VO_SetWindowZorder(hWindow, HI_LAYER_ZORDER_MOVETOP);
    if(HI_SUCCESS != s32Ret)
    {
        sample_common_printf("Get window Z order fail! window handle=%d i=%d\n",hWindow, i);
        return -1;
    }

    for (i = 0; i < 16; i++)
    {
        s32Ret = HI_UNF_VO_GetWindowAttr(hWindow, &stWinAttr);
        if(HI_SUCCESS != s32Ret)
        {
            sample_common_printf("Get window attr fail! window handle=%d i=%d\n",hWindow, i);
            return -1;
        }

        stWinAttr.stOutputRect.s32X -= pstOrgRect->s32X/16;
        stWinAttr.stOutputRect.s32Y -= PVR_ZOOM_HEIGHT_STEP;
        stWinAttr.stOutputRect.s32Width += PVR_ZOOM_WIDTH_STEP;
        stWinAttr.stOutputRect.s32Height += PVR_ZOOM_HEIGHT_STEP;

        s32Ret = HI_UNF_VO_SetWindowAttr(hWindow, &stWinAttr);
        if(HI_SUCCESS != s32Ret)
        {
            sample_common_printf("Set window attr fail! window handle=%d i=%d ret=%#x\n",hWindow, i, s32Ret);
            return -1;
        }

    }

    stWinAttr.stOutputRect.s32X = 0;
    stWinAttr.stOutputRect.s32Y = 0;
#if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    stWinAttr.stOutputRect.s32Width = 1280;
    stWinAttr.stOutputRect.s32Height = 720;
#else
    stWinAttr.stOutputRect.s32Width = 1920;
    stWinAttr.stOutputRect.s32Height = 1080;
#endif
    s32Ret = HI_UNF_VO_SetWindowAttr(hWindow, &stWinAttr);
    if(HI_SUCCESS != s32Ret)
    {
        sample_common_printf("Set window attr fail! window handle=%d i=%d\n",hWindow, i);
        return -1;
    }

    s32Ret = HI_UNF_VO_SetWindowZorder(hWindow, HI_LAYER_ZORDER_MOVETOP);
    if(HI_SUCCESS != s32Ret)
    {
        sample_common_printf("Get window Z order fail! window handle=%d i=%d\n",hWindow, i);
        return -1;
    }

    return 0;
}

HI_S32 PVR_ZoomOutDispToWindow(HI_HANDLE hWindow, HI_RECT_S *pstOrgRect, HI_U32 u32WindowZorder)
{
    HI_S32 i = 0;
    HI_S32 s32Ret = 0;
    HI_UNF_WINDOW_ATTR_S stWinAttr;

    PVR_ProcZoomoutScreen(hWindow);

    s32Ret = HI_UNF_VO_SetWindowZorder(hWindow, u32WindowZorder);
    if(HI_SUCCESS != s32Ret)
    {
        sample_common_printf("Set window Z order fail! window handle=%d ret=%#x\n",hWindow, s32Ret);
        return -1;
    }

    for (i = 0; i < 16; i++)
    {
        s32Ret = HI_UNF_VO_GetWindowAttr(hWindow, &stWinAttr);
        if(HI_SUCCESS != s32Ret)
        {
            sample_common_printf("Get window attr fail! window handle=%d i=%d\n",hWindow, i);
            return -1;
        }

        if((stWinAttr.stOutputRect.s32X == pstOrgRect->s32X) &&
            (stWinAttr.stOutputRect.s32Y == pstOrgRect->s32Y))
        {
            sample_common_printf("Window %d is already in window mode!\n",hWindow);
            return 0;
        }

        stWinAttr.stOutputRect.s32X += pstOrgRect->s32X/16;
        stWinAttr.stOutputRect.s32Y += PVR_ZOOM_HEIGHT_STEP;
        stWinAttr.stOutputRect.s32Width -= PVR_ZOOM_WIDTH_STEP;
        stWinAttr.stOutputRect.s32Height -= PVR_ZOOM_HEIGHT_STEP;
        s32Ret = HI_UNF_VO_SetWindowAttr(hWindow, &stWinAttr);
        if(HI_SUCCESS != s32Ret)
        {
            sample_common_printf("Set window attr fail! window handle=%d i=%d\n",hWindow, i);
            return -1;
        }

    }

    stWinAttr.stOutputRect.s32X = pstOrgRect->s32X;
    stWinAttr.stOutputRect.s32Y = pstOrgRect->s32Y;
    stWinAttr.stOutputRect.s32Width = pstOrgRect->s32Width;
    stWinAttr.stOutputRect.s32Height = pstOrgRect->s32Height;

    s32Ret = HI_UNF_VO_SetWindowAttr(hWindow, &stWinAttr);
    if(HI_SUCCESS != s32Ret)
    {
        sample_common_printf("Set window attr fail! window handle=%d ret=%#x\n",hWindow, s32Ret);
        return -1;
    }

    s32Ret = HI_UNF_VO_SetWindowZorder(hWindow, u32WindowZorder);
    if(HI_SUCCESS != s32Ret)
    {
        sample_common_printf("Set window Z order fail! window handle=%d ret=%#x\n",hWindow, s32Ret);
        return -1;
    }

    (void)pthread_mutex_lock(&g_stDrawRectMutex);
    PVR_DrawSelectRect(pstOrgRect, HI_TRUE, HI_TRUE);
    (void)pthread_mutex_unlock(&g_stDrawRectMutex);

    return 0;
}

HI_VOID PVR_ProccessStreamEndEvent(HI_U32 u32ChnID, HI_UNF_PVR_EVENT_E EventType, HI_S32 s32EventValue, HI_VOID *args)
{
    HI_U32 i = 0;
    HI_U32 u32ChanNum = 0;
    HI_U32 LastEventTime = 0;
    HI_U32 TimeCost = 0;

    for(i = 0; i < sizeof(g_stPlayChnAttr)/sizeof(PVR_PLAY_CHN_ATTR); i++)
    {
        if (g_stPlayChnAttr[i].u32PvrPlayChn == u32ChnID)
        {
            u32ChanNum = i;
            break;
        }
    }

    if (i >= sizeof(g_stPlayChnAttr)/sizeof(PVR_PLAY_CHN_ATTR))
    {
        return;
    }

    if ( HI_UNF_PVR_EVENT_PLAY_EOF == EventType  )
    {
        LastEventTime = EventTime;
        HI_SYS_GetTimeStampMs(&EventTime);
        TimeCost = EventTime - LastEventTime;
        EventTime = 0;
        sample_common_printf("========== play to the end ======\n"
                "========== Channel:%d\n",u32ChanNum);

        if ( LastEventTime != 0 )
        {
            sample_common_printf("======Time Cost:%dms=======\n",TimeCost);
        }

    }
    else if ( HI_UNF_PVR_EVENT_PLAY_SOF == EventType  )
    {
        LastEventTime = EventTime;
        HI_SYS_GetTimeStampMs(&EventTime);
        TimeCost = EventTime - LastEventTime;
        EventTime = 0;
        sample_common_printf("========== play to the start ======\n"
                "========== Channel:%d\n",u32ChanNum);
        if ( LastEventTime != 0 )
        {
            sample_common_printf("======Time Cost:%dms=======\n",TimeCost);
        }
    }

    return;
}

HI_VOID PVR_ProccessReachEvent(HI_U32 u32ChnID, HI_UNF_PVR_EVENT_E EventType, HI_S32 s32EventValue, HI_VOID *args)
{
    HI_U32  i = 0;
    HI_U32  u32ChanNum = 0;
    HI_S32  Ret = 0;

    for(i = 0; i < sizeof(g_stTimeShiftChnAttr)/sizeof(PVR_TIMESHIFT_CHN_ATTR); i++)
    {
        if (HI_UNF_PVR_EVENT_PLAY_REACH_REC == EventType)
        {
            if (g_stTimeShiftChnAttr[i].u32PvrPlayChn == u32ChnID)
            {
                u32ChanNum = i;
                break;
            }
        }
        else
        {
            if (g_stTimeShiftChnAttr[i].u32PvrRecChn == u32ChnID)
            {
                u32ChanNum = i;
                break;
            }
        }
    }

    if (i >= sizeof(g_stTimeShiftChnAttr)/sizeof(PVR_TIMESHIFT_CHN_ATTR))
    {
        return;
    }

    g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited = HI_FALSE;
    g_stTimeShiftChnAttr[u32ChanNum].bLiveStoped = HI_FALSE;

    PVR_StopPlay(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn,
                  g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId,
                  g_stTimeShiftChnAttr[u32ChanNum].hTsBufForPlayback);

    Ret = HI_UNF_DMX_AttachTSPort(g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId, g_stTimeShiftChnAttr[u32ChanNum].u32DmxPort);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("[%s] HI_UNF_DMX_AttachTSPort failed 0x%x\n",
            __FUNCTION__, Ret);
    }

    Ret = PVR_StartLive(g_stTimeShiftChnAttr[u32ChanNum].u32ProgNum,
                            g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId,
                            g_stTimeShiftChnAttr[u32ChanNum].hAvplay,
                            &g_stTimeShiftChnAttr[u32ChanNum].bVideoExist,
                            g_stTimeShiftChnAttr[u32ChanNum].u32DmxPort);
    if (HI_SUCCESS != Ret)
    {
        sample_common_printf("[%s] PVR_StartLive failed\n", __FUNCTION__);
    }

    PVR_DrawPlayChnInitRect(g_stTimeShiftChnAttr[u32ChanNum].hPlayChn);

    if (HI_UNF_PVR_EVENT_PLAY_REACH_REC == EventType)
    {
        sample_common_printf("========== play reach to record ======\n"
                "========== Channel:%d\n"
                "========== resume to live play  ======\n",u32ChanNum);
    }
    else
    {
        sample_common_printf("========== record reach to play ======\n"
                "========== Channel:%d\n"
                "========== resume to live play  ======\n",u32ChanNum);
    }
}

HI_VOID PVR_ShowUsage(HI_VOID)
{
    printf("Usage: sample_pvr_demo <MODE> [ARGS]\n\n"
            "Available MODE: 0 Timeshift, support up to 3 timeshift\n"
            "                  ARGS: path freq srate qamtype or polarization \n"
            "                        or\n"
            "                        path multicast_address udp_port\n\n"
            "                1 Playback, support up to 3 playback\n"
            "                  ARGS: [file1] [file2] [file3]\n\n"
            /*"                2 Recording, support up to 4 recording\n"
            "                  ARGS: path freq [srate] [qamtype or polarization]\n\n"*/
            "                qamtype or polarization: \n"
            "                      For cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64] \n"
            "                      For satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0] \n"
            "For example:\n"
            "            sample_pvr_demo 0 /mnt 610 6875 64\n"
            "            sample_pvr_demo 0 /mnt 239.1.1.1 8800\n"
            "            sample_pvr_demo 1 rec_v512_a1380.ts rec_v513_a1381.ts rec_v513_a1382.ts\n"
            /*"            sample_pvr_demo 2 /mnt 610 6875 64\n"*/);
}

HI_VOID PVR_PlayBackCmdProcess(const HI_CHAR *pcInputCmd)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32ChanNum = 0;
    HI_U32 u32PlaySeekTime = 0;
    HI_CHAR pcPlayFileName[128] = {0};
    HI_UNF_PVR_PLAY_MODE_S stPlayTrickMode;
    HI_UNF_PVR_PLAY_POSITION_S stPlayPos;

    if('h' == pcInputCmd[0])
    {
        printf("command:\n"
               "1.add filename\n"
               "\tadd a play file, support up to 3 play windows\n"
               "2.CHAN CMD\n"
               "\tCHAN: 0 1 2\n"
               "\tCMD: zoomin zoomout pause resmue step ff fb sf seek mute\n"
               "\t\tzoomin: zoom in play window\n"
               "\t\tzoomount: zoom out play window to full screen\n"
               "\t\tpause: pause current play\n"
               "\t\tresume: resume current play\n"
               "\t\tstep: play frame by frame\n"
               "\t\tff: fast forward play, switch a speed one time\n"
               "\t\tfb: fast backward play, switch a speed one time\n"
               "\t\tsf: slow forward play, switch a speed one time\n"
               "\t\tseek: seek to some where by time(s), need another time parameter, like \"0 seek 5\" or \"0 seek -5\" or \"0 seek +5\"\n"
               "\t\tmute: mute current play channel\n"
               "3.Remote controller\n"
               "\tpress \"EPG\" button for help\n");
        return;
    }
    else
    {
        if(!strncmp(&pcInputCmd[0], "add", strlen("add")))
        {
            strncpy(pcPlayFileName, &pcInputCmd[4], strlen(&pcInputCmd[4])-1);

            PVR_AddAPlayBack(pcPlayFileName);

            memset(pcPlayFileName, 0 ,sizeof(pcPlayFileName));

            return;
        }

        u32ChanNum = strtol(&pcInputCmd[0],NULL,0);
        if(!strncmp(&pcInputCmd[2], "zoomin", strlen("zoomin")))
        {
            if (HI_TRUE == g_stPlayChnAttr[u32ChanNum].bVideoExist)
            {
                PVR_ZoomInDispToFullScreen(g_stPlayChnAttr[u32ChanNum].hWindow, &g_stPlayChnAttr[u32ChanNum].stRect);
                g_stPlayChnAttr[u32ChanNum].bFullScreen = HI_TRUE;
            }

            return;
        }

        if(!strncmp(&pcInputCmd[2], "zoomout", strlen("zoomout")))
        {
            if (HI_TRUE == g_stPlayChnAttr[u32ChanNum].bVideoExist)
            {
                PVR_ZoomOutDispToWindow(g_stPlayChnAttr[u32ChanNum].hWindow, &g_stPlayChnAttr[u32ChanNum].stRect, g_stPlayChnAttr[u32ChanNum].u32WindowZorder);
                g_stPlayChnAttr[u32ChanNum].bFullScreen = HI_FALSE;
            }

            return;
        }

        if(!strncmp(&pcInputCmd[2], "pause", strlen("pause")))
        {
            s32Ret = HI_UNF_PVR_PlayPauseChn(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn);
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
            }

            return;
        }

        if(!strncmp(&pcInputCmd[2], "resume", strlen("resume")))
        {
            g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
            g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
            g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

            s32Ret = HI_UNF_PVR_PlayResumeChn(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn);
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
            }

            return;
        }

        if(!strncmp(&pcInputCmd[2], "step", strlen("step")))
        {
            g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
            g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
            g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

            s32Ret = HI_UNF_PVR_PlayStep(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, 1);
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call HI_UNF_PVR_PlayStep failed.\n");
            }

            return;
        }

        if(!strncmp(&pcInputCmd[2], "ff", strlen("ff")))
        {
            g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
            g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

            g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes %= 6;
            stPlayTrickMode.enSpeed = (0x1 << (g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes + 1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;

            s32Ret = HI_UNF_PVR_PlayTPlay(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayTrickMode);
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call HI_UNF_PVR_PlayTPlay failed.\n");
            }
            g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes++;

            return;
        }

        if(!strncmp(&pcInputCmd[2], "fb", strlen("fb")))
        {
            g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
            g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

            g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes %= 6;
            stPlayTrickMode.enSpeed = -(0x1 << (g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes + 1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;

            s32Ret = HI_UNF_PVR_PlayTPlay(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayTrickMode);
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call HI_UNF_PVR_PlayTPlay failed.\n");
            }
            g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes++;

            return;
        }

        if(!strncmp(&pcInputCmd[2], "sf", strlen("sf")))
        {
            g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
            g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;

            g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes %= 6;
            stPlayTrickMode.enSpeed = HI_UNF_PVR_PLAY_SPEED_NORMAL / (0x1 << (g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes + 1));

            s32Ret = HI_UNF_PVR_PlayTPlay(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayTrickMode);
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call HI_UNF_PVR_PlayTPlay failed.\n");
            }
            g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes++;

            return;
        }

        if(!strncmp(&pcInputCmd[2], "seek", strlen("seek")))
        {
            g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
            g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
            g_stPlayChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

            if('+' == pcInputCmd[7])
            {
                u32PlaySeekTime = strtol(&pcInputCmd[8],NULL,0);
                stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                stPlayPos.s64Offset = (HI_S64)u32PlaySeekTime*1000;
                stPlayPos.s32Whence = SEEK_CUR;

                sample_common_printf("seek forward %ds\n", u32PlaySeekTime);
            }
            else if('-' == pcInputCmd[7])
            {
                u32PlaySeekTime = strtol(&pcInputCmd[8],NULL,0);

                stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                stPlayPos.s64Offset = -(HI_S64)u32PlaySeekTime*1000;
                stPlayPos.s32Whence = SEEK_CUR;

                sample_common_printf("seek backward %ds\n", u32PlaySeekTime);
            }
            else
            {
                u32PlaySeekTime = strtol(&pcInputCmd[7],NULL,0);

                stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                stPlayPos.s64Offset = u32PlaySeekTime*1000;
                stPlayPos.s32Whence = SEEK_SET;

                sample_common_printf("seek to %ds\n", u32PlaySeekTime);
            }

            s32Ret = HI_UNF_PVR_PlayResumeChn(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn);
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
            }

            s32Ret = HI_UNF_PVR_PlaySeek(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayPos);
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call HI_UNF_PVR_PlaySeek failed.\n");
            }

            return;
        }
        if(!strncmp(&pcInputCmd[2], "timecost", strlen("timecost")))
        {
            HI_S32 SpeedRate = 1;
            g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
            g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
            s32Ret = HI_UNF_PVR_PlayResumeChn(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn);
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
            }
            SpeedRate = strtol(&pcInputCmd[13],NULL,0);
            if(!strncmp(&pcInputCmd[11], "fb", strlen("fb")))
            {
                g_stPlayChnAttr[u32ChanNum].u32FBSpeedTimes = SpeedRate;
                SpeedRate = -1*SpeedRate;

                stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                stPlayPos.s64Offset = 0;
                stPlayPos.s32Whence = SEEK_END;
                sample_common_printf("seek end\n");
                s32Ret = HI_UNF_PVR_PlaySeek(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayPos);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("call HI_UNF_PVR_PlaySeek failed.\n");
                }

            }
            else if(!strncmp(&pcInputCmd[11], "ff", strlen("fb")))
            {
                g_stPlayChnAttr[u32ChanNum].u32FFSpeedTimes = SpeedRate;
                stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                stPlayPos.s64Offset = 0;
                stPlayPos.s32Whence = SEEK_SET;
                sample_common_printf("seek start\n");
                s32Ret = HI_UNF_PVR_PlaySeek(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayPos);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("call HI_UNF_PVR_PlaySeek failed.\n");
                }

            }
            /*
            Otherwise ,PVRPlayResetToCurFrame called by HI_UNF_PVR_PlayTPlay will re seek to the Cur frame.*/
            sleep(1);

            stPlayTrickMode.enSpeed = SpeedRate*HI_UNF_PVR_PLAY_SPEED_NORMAL;
            sample_common_printf("Tplay Speed:%d\n",stPlayTrickMode.enSpeed);
            s32Ret = HI_UNF_PVR_PlayTPlay(g_stPlayChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayTrickMode);
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call HI_UNF_PVR_PlayTPlay failed.\n");
            }
            HI_SYS_GetTimeStampMs(&EventTime);
            return;
        }

        if(!strncmp(&pcInputCmd[2], "mute", strlen("mute")))
        {
            if (HI_FALSE == g_stPlayChnAttr[u32ChanNum].bMuteSeted)
            {
                s32Ret = HI_UNF_SND_SetTrackMute(g_stPlayChnAttr[u32ChanNum].hSoundTrack, HI_TRUE);
                if (HI_SUCCESS != s32Ret)
                {
                    sample_common_printf("Set mute fail! track=%#x ret=%#x\n", g_stPlayChnAttr[u32ChanNum].hSoundTrack, s32Ret);
                    return;
                }

                g_stPlayChnAttr[u32ChanNum].bMuteSeted = HI_TRUE;
            }
            else
            {
                s32Ret = HI_UNF_SND_SetTrackMute(g_stPlayChnAttr[u32ChanNum].hSoundTrack, HI_FALSE);
                if (HI_SUCCESS != s32Ret)
                {
                    sample_common_printf("Enable audio fail! track=%#x ret=%#x\n", g_stPlayChnAttr[u32ChanNum].hSoundTrack, s32Ret);
                    return;
                }

                g_stPlayChnAttr[u32ChanNum].bMuteSeted = HI_FALSE;
            }

            if (HI_TRUE == g_stPlayChnAttr[u32ChanNum].bFullScreen)
            {
                HI_RECT stMuteRect;

                #if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
                stMuteRect.x = 50;
                stMuteRect.y = 10;
                stMuteRect.w = 40;
                stMuteRect.h = 25;
                #else
                stMuteRect.x = 50;
                stMuteRect.y = 50;
                stMuteRect.w = 60;
                stMuteRect.h = 40;
                #endif

                PVR_DrawMuteText(&stMuteRect, g_stPlayChnAttr[u32ChanNum].bMuteSeted);
            }
            else
            {
                PVR_DrawMuteText(&g_stPlayChnAttr[u32ChanNum].stMuteTextRect,
                                   g_stPlayChnAttr[u32ChanNum].bMuteSeted);
            }

            return;
        }
    }

    return;
}

HI_VOID PVR_TimeShiftCmdProcess(const HI_CHAR *pcInputCmd, const HI_CHAR *pcRecPath)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32ChanNum = 0;
    HI_U32 u32ProgramNum = 0;
    HI_U64 u64MaxRecTimeMs = 0;
    HI_U64 u64MaxRecSize = 0;
    HI_BOOL bRewindFlag = HI_FALSE;
    HI_BOOL bCipherFlg = HI_FALSE;
    HI_UNF_PVR_REC_ATTR_S stPvrRecAttr;

    HI_U32 u32PlaySeekTime = 0;
    HI_UNF_PVR_PLAY_MODE_S stPlayTrickMode;
    HI_UNF_PVR_PLAY_POSITION_S stPlayPos;

    if('h' == pcInputCmd[0])
    {
        printf("command:\n"
               "1.add program\n"
               "\tadd a program, support up to 3 play windows\n"
               "2.CHAN CMD\n"
               "\tCHAN: 0 1 2\n"
               "\tCMD: zoomin zoomout rec play pause resmue step ff fb sf seek live mute loadcw\n"
               "\t\tzoomin: zoom in play window\n"
               "\t\tzoomount: zoom out play window to full screen\n"
               "\t\trec: record this program.\n"
               "\t\t     parameters: enc rewind time 120, means encrypt stream, rewind recording, rewind by time 120s\n"
               "\t\t                 unc rewind size 100, means unencrypt stream, rewind recording, rewind by size 100m\n"
               "\t\t                 enc norewind size 100, means encrypt stream, not rewind recording\n"
               "\t\t                 unc norewind, means unencrypt stream, not rewind, no limit recording\n"
               "\t\tplay: playback the recorded file\n"
               "\t\tThe following cmd only avalible in playback mode:\n"
               "\t\t     pause: pause current play, if live playing, it will stop current play and mark the pause position\n"
               "\t\t     resume: resume current play\n"
               "\t\t     setp: play frame by frame\n"
               "\t\t     ff: fast forward play, switch a speed one time\n"
               "\t\t     fb: fast backward play, switch a speed one time\n"
               "\t\t     sf: slow forward play, switch a speed one time\n"
               "\t\t     seek: seek to some where by time(s), need another time parameter, like \"0 seek 5\" or \"0 seek -5\" or \"0 seek +5\"\n"
               "\t\t     live: switch playback to live play\n"
               "\t\tmute: mute current play channel\n"
               "\t\tloadcw: load the cw to the descrambler to get clear stream  \n"
               "\t\t     parameters: loadcw default, means using default cw, only for suma_big.ts in this code\n"
               "\t\t                 loadcw keyvalue(odd even), means user should type the add and even key value tho load\n"
               "\t\t                 for example:\n"
               "\t\t                 loadcw ff ff ff fd ff ff ff fd ff ff ff fd ff ff ff fd\n"
               "3.Remote controller\n"
               "\tpress \"EPG\" button for help\n");
    }
    else
    {
        if(!strncmp(&pcInputCmd[0], "add", strlen("add")))
        {
            u32ProgramNum = strtol(&pcInputCmd[4],NULL,0);

            PVR_AddALivePlay(u32ProgramNum);

            return;
        }

        u32ChanNum = strtol(&pcInputCmd[0],NULL,0);

        if(u32ChanNum >= g_TimeshiftChn)
        {
            sample_common_printf("Channel number %d is invaild.\n", u32ChanNum);
            return;
        }

        if(!strncmp(&pcInputCmd[2], "loadcw", strlen("loadcw")))
        {
            /*
            0 loadcw 11 22 33 44 55 66 77 88 11 22 33 44 55 66 77 88
            以上命令为56个字符
            0 loadcw default*/

            HI_CHAR TmpKey[3] = {0};
            if(!strncmp(&pcInputCmd[9], "default", strlen("default")))
            {
                ;
            }
            else
            {
                HI_U8 i = 0;
                HI_U8 j = 0;
                printf("OddKey:\n");
                for ( i = 9 ; i < 33 ; i += 3 )
                {
                    strncpy(TmpKey,&pcInputCmd[i],2);
                    Odd_CW[j] = strtol(TmpKey,NULL,16);
                    printf("0x%x ",Odd_CW[j]);
                    j++;
                }
                j = 0;
                printf("\nEvenKey:\n");
                for ( i = 33 ; i < 56 ; i += 3 )
                {
                    strncpy(TmpKey,&pcInputCmd[i],2);
                    Even_CW[j] = strtol(TmpKey,NULL,16);
                    printf("0x%x ",Even_CW[j]);
                    j++;
                }

            }

            PVR_LoadCW(g_stTimeShiftChnAttr[u32ChanNum].hAvplay);
            return;
        }

        if(!strncmp(&pcInputCmd[2], "zoomin", strlen("zoomin")))
        {
            if (HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bVideoExist)
            {
                PVR_ZoomInDispToFullScreen(g_stTimeShiftChnAttr[u32ChanNum].hWindow, &g_stTimeShiftChnAttr[u32ChanNum].stRect);
                g_stTimeShiftChnAttr[u32ChanNum].bFullScreen = HI_TRUE;
            }

            return;
        }

        if(!strncmp(&pcInputCmd[2], "zoomout", strlen("zoomout")))
        {
            if (HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bVideoExist)
            {
                PVR_ZoomOutDispToWindow(g_stTimeShiftChnAttr[u32ChanNum].hWindow, &g_stTimeShiftChnAttr[u32ChanNum].stRect, g_stTimeShiftChnAttr[u32ChanNum].u32WindowZorder);
                g_stTimeShiftChnAttr[u32ChanNum].bFullScreen = HI_FALSE;
            }

            return;
        }

        if(!strncmp(&pcInputCmd[2], "rec", strlen("rec")))
        {
            /* 0 rec rewind time 120"
                 0 rec rewind size 100"
                 0 rec norewind size 100*/

            if(HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bRecChnInited)
            {
                sample_common_printf("Chan %d is already recording!!\n", u32ChanNum);
                return;
            }

            if(!strncmp(&pcInputCmd[6], "enc", strlen("enc")))
            {
                bCipherFlg = HI_TRUE;
            }
            else if(!strncmp(&pcInputCmd[6], "unc", strlen("unc")))
            {
                bCipherFlg = HI_FALSE;
            }
            else
            {
                sample_common_printf("Invalid parameter, press \'h\' for help.\n");
                return;
            }

            if(!strncmp(&pcInputCmd[10], "rewind", strlen("rewind")))
            {
                bRewindFlag = HI_TRUE;

                if(!strncmp(&pcInputCmd[17], "time", strlen("time")))
                {
                    u64MaxRecTimeMs = 1000 * (HI_U64)strtol(&pcInputCmd[22],NULL,0);
                    u64MaxRecSize = 0;
                }
                else if(!strncmp(&pcInputCmd[17], "size", strlen("size")))
                {
                    u64MaxRecSize = 0x100000 * (HI_U64)strtol(&pcInputCmd[22],NULL,0);
                    u64MaxRecTimeMs = 0;
                }
                else
                {
                    sample_common_printf("Invalid parameter, press \'h\' for help.\n");
                    return;
                }
            }
            else if(!strncmp(&pcInputCmd[10], "norewind", strlen("norewind")))
            {
                bRewindFlag = HI_FALSE;

                if(!strncmp(&pcInputCmd[19], "time", strlen("time")))
                {
                    u64MaxRecTimeMs = 1000 * (HI_U64)strtol(&pcInputCmd[24],NULL,0);
                    u64MaxRecSize = 0;
                }
                else if(!strncmp(&pcInputCmd[19], "size", strlen("size")))
                {
                    u64MaxRecSize = 0x100000 * (HI_U64)strtol(&pcInputCmd[24],NULL,0);
                    u64MaxRecTimeMs = 0;
                }
                else
                {
                    u64MaxRecTimeMs = 0;
                    u64MaxRecSize = 0;
                }
            }
            else
            {
                sample_common_printf("Invalid parameter, press \'h\' for help.\n");
                return;
            }

            if(HI_SUCCESS == PVR_AddARec(g_stTimeShiftChnAttr[u32ChanNum].hPlayChn,
                                         g_sRecPath,
                                         g_stTimeShiftChnAttr[u32ChanNum].u32ProgNum,
                                         bRewindFlag,
                                         bCipherFlg,
                                         u64MaxRecSize,
                                         u64MaxRecTimeMs,
                                         &g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn,
                                         PVR_TEST_MODE_TIMESHIFT))
            {
                g_stTimeShiftChnAttr[u32ChanNum].bRecChnInited = HI_TRUE;

                //=============if rec stream is scrambled ,attach descramble to demux rec chan=============//
                HI_HANDLE hAvplay ;
                HI_HANDLE hDmxVidChn,hDmxAudChn;
                HI_HANDLE hDmxVidRecChn,hDmxAudRecChn;
                HI_HANDLE hDescrambler;
                HI_U32 Vpid;
                HI_U32 Apid;

                hAvplay = g_stTimeShiftChnAttr[u32ChanNum].hAvplay;
                s32Ret = HI_UNF_AVPLAY_GetDmxVidChnHandle(hAvplay,&hDmxVidChn);
                s32Ret |= HI_UNF_AVPLAY_GetDmxAudChnHandle(hAvplay,&hDmxAudChn);
                if (HI_SUCCESS == s32Ret)
                {
                     s32Ret = HI_UNF_DMX_GetDescramblerKeyHandle(hDmxVidChn,&hDescrambler);
                     if ( HI_SUCCESS == s32Ret )
                     {
                        (HI_VOID)HI_UNF_DMX_GetChannelPID(hDmxVidChn, &Vpid);
                        (HI_VOID)HI_UNF_DMX_GetChannelPID(hDmxAudChn, &Apid);

                        s32Ret =  HI_UNF_DMX_GetChannelHandle(g_stTimeShiftChnAttr[u32ChanNum].u32RecDemuxId , Vpid, &hDmxVidRecChn);
                        if ( HI_SUCCESS == s32Ret )
                        {
                            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hDescrambler,hDmxVidRecChn);
                        }

                        s32Ret = HI_UNF_DMX_GetChannelHandle(g_stTimeShiftChnAttr[u32ChanNum].u32RecDemuxId , Apid, &hDmxAudRecChn);
                        if (HI_SUCCESS == s32Ret)
                        {
                            (HI_VOID)HI_UNF_DMX_AttachDescrambler(hDescrambler,hDmxAudRecChn);
                        }
                     }
                }
                //===============================================================================================//
            }
        }

        if(!strncmp(&pcInputCmd[2], "mute", strlen("mute")))
        {
            if (HI_FALSE == g_stTimeShiftChnAttr[u32ChanNum].bMuteSeted)
            {
                s32Ret = HI_UNF_SND_SetTrackMute(g_stTimeShiftChnAttr[u32ChanNum].hSoundTrack, HI_TRUE);
                if (HI_SUCCESS != s32Ret)
                {
                    sample_common_printf("Set mute fail! track=%#x ret=%#x\n", g_stTimeShiftChnAttr[u32ChanNum].hSoundTrack, s32Ret);
                    return;
                }

                g_stTimeShiftChnAttr[u32ChanNum].bMuteSeted = HI_TRUE;
            }
            else
            {
                s32Ret = HI_UNF_SND_SetTrackMute(g_stTimeShiftChnAttr[u32ChanNum].hSoundTrack, HI_FALSE);
                if (HI_SUCCESS != s32Ret)
                {
                    sample_common_printf("Enable audio fail! track=%#x ret=%#x\n", g_stTimeShiftChnAttr[u32ChanNum].hSoundTrack, s32Ret);
                    return;
                }

                g_stTimeShiftChnAttr[u32ChanNum].bMuteSeted = HI_FALSE;
            }

            if (HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bFullScreen)
            {
                HI_RECT stMuteRect;

                #if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
                stMuteRect.x = 50;
                stMuteRect.y = 10;
                stMuteRect.w = 40;
                stMuteRect.h = 25;
                #else
                stMuteRect.x = 50;
                stMuteRect.y = 50;
                stMuteRect.w = 60;
                stMuteRect.h = 40;
                #endif

                PVR_DrawMuteText(&stMuteRect, g_stTimeShiftChnAttr[u32ChanNum].bMuteSeted);
            }
            else
            {
                PVR_DrawMuteText(&g_stTimeShiftChnAttr[u32ChanNum].stMuteTextRect,
                                   g_stTimeShiftChnAttr[u32ChanNum].bMuteSeted);
            }

            return;
        }

        if ((HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bRecChnInited) &&
            (HI_FALSE == g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited))
        {
            if(!strncmp(&pcInputCmd[2], "pause", strlen("pause")))
            {
                s32Ret = PVR_StopLive(g_stTimeShiftChnAttr[u32ChanNum].hAvplay);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("Stop live failed. ret=%#x\n", s32Ret);
                }

                s32Ret = HI_UNF_PVR_PlayPauseChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
                }
            }

            if(!strncmp(&pcInputCmd[2], "resume", strlen("resume")))
            {
                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                s32Ret = HI_UNF_PVR_RecGetChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn, &stPvrRecAttr);

                s32Ret = HI_UNF_DMX_DetachTSPort(g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId);

                s32Ret = PVR_StartPlay(stPvrRecAttr.szFileName,
                                         g_stTimeShiftChnAttr[u32ChanNum].hPlayChn,
                                         g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId,
                                         g_stTimeShiftChnAttr[u32ChanNum].hAvplay,
                                         &g_stTimeShiftChnAttr[u32ChanNum].hTsBufForPlayback,
                                         &g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn,
                                         &g_stTimeShiftChnAttr[u32ChanNum].bVideoExist);
                g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited = HI_TRUE;
                g_stTimeShiftChnAttr[u32ChanNum].bLiveStoped = HI_TRUE;
                if(0 != s32Ret)
                {
                    sample_common_printf("[%s_%d]Start pvr play fail! playchn=%d filename=%s\n", 
                        __FILE__, __LINE__, g_stTimeShiftChnAttr[u32ChanNum].hPlayChn, stPvrRecAttr.szFileName);
                    return;
                }
            }

            if(!strncmp(&pcInputCmd[2], "fb", strlen("fb")))
            {
                s32Ret = PVR_StopLive(g_stTimeShiftChnAttr[u32ChanNum].hAvplay);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("Stop live failed. ret=%#x\n", s32Ret);
                }

                s32Ret = HI_UNF_PVR_PlayPauseChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
                }

                s32Ret = HI_UNF_PVR_RecGetChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn, &stPvrRecAttr);

                s32Ret = HI_UNF_DMX_DetachTSPort(g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId);

                s32Ret = PVR_StartPlay(stPvrRecAttr.szFileName,
                                         g_stTimeShiftChnAttr[u32ChanNum].hPlayChn,
                                         g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId,
                                         g_stTimeShiftChnAttr[u32ChanNum].hAvplay,
                                         &g_stTimeShiftChnAttr[u32ChanNum].hTsBufForPlayback,
                                         &g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn,
                                         &g_stTimeShiftChnAttr[u32ChanNum].bVideoExist);

                g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited = HI_TRUE;
                g_stTimeShiftChnAttr[u32ChanNum].bLiveStoped = HI_TRUE;

                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;
            }
        }

        if(!strncmp(&pcInputCmd[2], "play", strlen("play")))
        {
            HI_UNF_AVPLAY_STOP_OPT_S stopOpt;

            if(HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited)
            {
                sample_common_printf("[%s_%d]Chan %d is already playing!!\n", __FILE__, __LINE__, u32ChanNum);
                return;
            }

            stopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
            stopOpt.u32TimeoutMs = 0;
            printf("%s_%d](u32ChanNum = %d)!\n", __FILE__, __LINE__, u32ChanNum);
            s32Ret = HI_UNF_AVPLAY_Stop(g_stTimeShiftChnAttr[u32ChanNum].hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stopOpt);
            if (HI_SUCCESS != s32Ret)
            {
                printf("[%s_%d](s32Ret = 0x%08x)!\n", __FILE__, __LINE__, s32Ret);
            }
            s32Ret = HI_UNF_PVR_RecGetChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrRecChn, &stPvrRecAttr);
            if (HI_SUCCESS != s32Ret)
            {
                printf("[%s_%d](s32Ret = 0x%08x)!\n", __FILE__, __LINE__, s32Ret);
            }
            s32Ret = HI_UNF_DMX_DetachTSPort(g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId);
            if (HI_SUCCESS != s32Ret)
            {
                printf("[%s_%d](s32Ret = 0x%08x)!\n", __FILE__, __LINE__, s32Ret);
            }
            s32Ret = PVR_StartPlay(stPvrRecAttr.szFileName, 
                                     g_stTimeShiftChnAttr[u32ChanNum].hPlayChn, 
                                     g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId, 
                                     g_stTimeShiftChnAttr[u32ChanNum].hAvplay, 
                                     &g_stTimeShiftChnAttr[u32ChanNum].hTsBufForPlayback, 
                                     &g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn,
                                     &g_stTimeShiftChnAttr[u32ChanNum].bVideoExist);
            g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited = HI_TRUE;
            g_stTimeShiftChnAttr[u32ChanNum].bLiveStoped = HI_TRUE;
            if(0 != s32Ret)
            {
                sample_common_printf("[%s_%d]Start pvr play fail! playchn=%d filename=%s\n", __FILE__, __LINE__, g_stTimeShiftChnAttr[u32ChanNum].hPlayChn, stPvrRecAttr.szFileName);
                return;
            }
        }

        if(HI_TRUE == g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited)
        {
            if(!strncmp(&pcInputCmd[2], "pause", strlen("pause")))
            {
                s32Ret = HI_UNF_PVR_PlayPauseChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
                }

                return;
            }

            if(!strncmp(&pcInputCmd[2], "resume", strlen("resume")))
            {
                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;
                s32Ret = HI_UNF_PVR_PlayResumeChn(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                }

                return;
            }

            if(!strncmp(&pcInputCmd[2], "step", strlen("step")))
            {
                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                s32Ret = HI_UNF_PVR_PlayStep(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn, 1);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("call HI_UNF_PVR_PlayStep failed.\n");
                }

                return;
            }

            if(!strncmp(&pcInputCmd[2], "ff", strlen("ff")))
            {
                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes %= 6;
                stPlayTrickMode.enSpeed = (0x1 << (g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes + 1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;

                s32Ret = HI_UNF_PVR_PlayTPlay(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayTrickMode);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                }
                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes++;

                return;
            }

            if(!strncmp(&pcInputCmd[2], "fb", strlen("fb")))
            {
                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes %= 6;
                stPlayTrickMode.enSpeed = -(0x1 << (g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes + 1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;

                s32Ret = HI_UNF_PVR_PlayTPlay(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayTrickMode);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                }
                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes++;

                return;
            }

            if(!strncmp(&pcInputCmd[2], "sf", strlen("sf")))
            {
                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;

                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes %= 6;
                stPlayTrickMode.enSpeed = HI_UNF_PVR_PLAY_SPEED_NORMAL / (0x1 << (g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes + 1));

                s32Ret = HI_UNF_PVR_PlayTPlay(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayTrickMode);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                }
                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes++;

                return;
            }

            if(!strncmp(&pcInputCmd[2], "seek", strlen("seek")))
            {
                g_stTimeShiftChnAttr[u32ChanNum].u32FFSpeedTimes = 0;
                g_stTimeShiftChnAttr[u32ChanNum].u32FBSpeedTimes = 0;
                g_stTimeShiftChnAttr[u32ChanNum].u32SFSpeedTimes = 0;

                if('+' == pcInputCmd[7])
                {
                    u32PlaySeekTime = strtol(&pcInputCmd[8],NULL,0);
                    stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                    stPlayPos.s64Offset = (HI_S64)u32PlaySeekTime*1000;
                    stPlayPos.s32Whence = SEEK_CUR;

                    sample_common_printf("seek forward %ds\n", u32PlaySeekTime);
                }
                else if('-' == pcInputCmd[7])
                {
                    u32PlaySeekTime = strtol(&pcInputCmd[8],NULL,0);

                    stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                    stPlayPos.s64Offset = -(HI_S64)u32PlaySeekTime*1000;
                    stPlayPos.s32Whence = SEEK_CUR;

                    sample_common_printf("seek backward %ds\n", u32PlaySeekTime);
                }
                else
                {
                    u32PlaySeekTime = strtol(&pcInputCmd[7],NULL,0);

                    stPlayPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                    stPlayPos.s64Offset = u32PlaySeekTime*1000;
                    stPlayPos.s32Whence = SEEK_SET;

                    sample_common_printf("seek to %ds\n", u32PlaySeekTime);
                }

                s32Ret = HI_UNF_PVR_PlaySeek(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn, &stPlayPos);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("call HI_UNF_PVR_PlaySeek failed.\n");
                }

                return;
            }

            if(!strncmp(&pcInputCmd[2], "live", strlen("live")))
            {
                g_stTimeShiftChnAttr[u32ChanNum].bPlayChnInited = HI_FALSE;
                g_stTimeShiftChnAttr[u32ChanNum].bLiveStoped = HI_FALSE;

                PVR_StopPlay(g_stTimeShiftChnAttr[u32ChanNum].u32PvrPlayChn,
                              g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId,
                              g_stTimeShiftChnAttr[u32ChanNum].hTsBufForPlayback);

                s32Ret = HI_UNF_DMX_AttachTSPort(g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId, g_stTimeShiftChnAttr[u32ChanNum].u32DmxPort);

                s32Ret = PVR_StartLive(g_stTimeShiftChnAttr[u32ChanNum].u32ProgNum,
                                        g_stTimeShiftChnAttr[u32ChanNum].u32PlayDemuxId,
                                        g_stTimeShiftChnAttr[u32ChanNum].hAvplay,
                                        &g_stTimeShiftChnAttr[u32ChanNum].bVideoExist,
                                        g_stTimeShiftChnAttr[u32ChanNum].u32DmxPort);

                PVR_DrawPlayChnInitRect(g_stTimeShiftChnAttr[u32ChanNum].hPlayChn);
            }
        }
    }
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 i = 0;
    HI_S32 s32Ret = 0;
    HI_U32 u32TestMode = -1;
    HI_CHAR pcInputCmd[64];
    HI_U32 u32TunerFreq = 0;
    HI_U32 u32SymbolRate = 0;
    HI_U32 u32QamType_Ploarization = 0;
    HI_CHAR aCurrentPath[256] = {0};
    pthread_t   stUpdatePlayStatusThread;
    pthread_t   stUpdateTimeShiftStatusThread;
    pthread_t   stIrReceiveThread;
    pthread_t   stEthSocketReceiveThread;

    if(2 > argc)
    {
        PVR_ShowUsage();
        return -1;
    }
    else
    {
        u32TestMode = strtol(argv[1],NULL,0);

        switch(u32TestMode)
        {
            case PVR_TEST_MODE_TIMESHIFT:
                if((6 != argc) && (5 != argc))
                {
                    PVR_ShowUsage();
                    return -1;
                }
                g_sRecPath = argv[2];

                if(6 == argc)
                {
                    u32TunerFreq = strtol(argv[3],NULL,0);
                    u32SymbolRate = strtol(argv[4],NULL,0);
                    u32QamType_Ploarization = strtol(argv[5],NULL,0);
                }
                else
                {
                    g_stServerAddr.sin_family = AF_INET;
                    g_stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
                    g_stServerAddr.sin_port = htons(strtol(argv[4],NULL,0));

                    g_stIpMreq.imr_multiaddr.s_addr = inet_addr(argv[3]);
                    g_stIpMreq.imr_interface.s_addr = htonl(INADDR_ANY);
                }

                break;
            case PVR_TEST_MODE_PLAYBACK:
                if(6 < argc)
                {
                    PVR_ShowUsage();
                    return -1;
                }

                break;
            case PVR_TEST_MODE_RECORDING:
                sample_common_printf("Not support yet!\n");
                return -1;
                if(6 != argc)
                {
                    PVR_ShowUsage();
                    return -1;
                }
                g_sRecPath = argv[2];
                u32TunerFreq = strtol(argv[3],NULL,0);
                u32SymbolRate = strtol(argv[4],NULL,0);
                u32QamType_Ploarization = strtol(argv[5],NULL,0);

                break;
            default:
                PVR_ShowUsage();
                return -1;
                break;
        };
    }

    memset(g_stPlayChnAttr, 0, sizeof(g_stPlayChnAttr));
    g_stPlayChnAttr->hWindow = -1;
    memset(g_stTimeShiftChnAttr, 0, sizeof(g_stTimeShiftChnAttr));
    g_stTimeShiftChnAttr->hWindow = -1;

    HI_SYS_Init();

    HIADP_MCE_Exit();

    s32Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("call HIADP_Snd_Init failed.\n");
        return -1;
    }

    s32Ret = PVR_Disp_Init(HI_UNF_ENC_FMT_1080P_60);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("call HIADP_Disp_Init failed.\n");
        return -1;
    }

    s32Ret = HI_UNF_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    if (s32Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_VO_Init failed.\n");
        return -1;
    }

    s32Ret = HI_UNF_DMX_Init();
    if (s32Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_DMX_Init failed.\n");
        return s32Ret;
    }

    s32Ret = HI_UNF_AVPLAY_Init();
    if (s32Ret != HI_SUCCESS)
    {
        sample_common_printf("call HI_UNF_AVPLAY_Init failed.\n");
        return s32Ret;
    }

    pthread_mutex_init(&g_stDrawRectMutex, NULL);

    memset(aCurrentPath, 0, sizeof(aCurrentPath));
    memset(g_aHisiliconLogoPath, 0, sizeof(g_aHisiliconLogoPath));

    memcpy(aCurrentPath, argv[0], (HI_U32)(strstr(argv[0], "sample_pvr_demo") - argv[0]));

    snprintf(g_aHisiliconLogoPath, sizeof(g_aHisiliconLogoPath), "%s/%s", aCurrentPath, HISILICON_LOGO_PATH);

    s32Ret = PVR_InitOsd(aCurrentPath);
    if (s32Ret != HI_SUCCESS)
    {
        sample_common_printf("call PVR_InitOsd failed.\n");
        return s32Ret;
    }

    s32Ret = PVR_InitIR();
    if (s32Ret != HI_SUCCESS)
    {
        sample_common_printf("call PVR_InitIR failed.\n");
        return s32Ret;
    }

    s32Ret = pthread_create(&stIrReceiveThread, NULL, PVR_IRReceiveThread, &u32TestMode);
    if (0 != s32Ret)
    {
        sample_common_printf("Create IR receive task fail! ret=%#x\n",s32Ret);
        return s32Ret;
    }

    switch(u32TestMode)
    {
        case PVR_TEST_MODE_TIMESHIFT:

            s32Ret = HI_UNF_PVR_RecInit();
            if (HI_SUCCESS != s32Ret)
            {
                sample_common_printf("call HI_UNF_PVR_RecInit failed.\n");
                return -1;
            }

            s32Ret = HI_UNF_PVR_PlayInit();
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call HI_UNF_PVR_PlayInit failed.\n");
                return s32Ret;
            }

            s32Ret = PVR_RegisterCallBacks();
            s32Ret |= HI_UNF_PVR_UnRegisterEvent(HI_UNF_PVR_EVENT_PLAY_REACH_REC);
            s32Ret |= HI_UNF_PVR_UnRegisterEvent(HI_UNF_PVR_EVENT_REC_REACH_PLAY);
            s32Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_PLAY_REACH_REC, PVR_ProccessReachEvent, NULL);
            s32Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_REACH_PLAY, PVR_ProccessReachEvent, NULL);
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call PVR_RegisterCallBacks failed.\n");
                return s32Ret;
            }

            HIADP_Search_Init();

            if(5 == argc)
            {
                s32Ret = HI_UNF_DMX_AttachTSPort(DEMUX_ID_6, HI_UNF_DMX_PORT_RAM_4);
                if (HI_SUCCESS != s32Ret)
                {
                    sample_common_printf("call HI_UNF_DMX_AttachTSPort failed.\n");
                    return HI_FAILURE;
                }

                s32Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_4, 0x200000, &g_hEthSocketRecvTsBuff);
                if (HI_SUCCESS != s32Ret)
                {
                    sample_common_printf("call HI_UNF_DMX_CreateTSBuffer failed. ret=%#x\n", s32Ret);
                    return HI_FAILURE;
                }

                g_bIsEthSocketRecvThreadStop = HI_FALSE;
                pthread_create(&stEthSocketReceiveThread, HI_NULL, (HI_VOID *)PVR_EthSocketRecvThread, HI_NULL);

                s32Ret = HI_UNF_DMX_AttachTSPort(DEMUX_ID_6, HI_UNF_DMX_PORT_RAM_4);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("call HI_UNF_DMX_AttachTSPort failed. ret=%#x dmxid=%d port=%d\n", s32Ret, DEMUX_ID_6, HI_UNF_DMX_PORT_RAM_4);
                    return -1;
                }

                s32Ret = HIADP_Search_GetAllPmt(DEMUX_ID_6, &g_pstProgTbl);
                if (HI_SUCCESS != s32Ret)
                {
                    sample_common_printf("call HIADP_Search_GetAllPmt failed.ret=%#x\n",s32Ret);
                    return -1;
                }
            }
            else
            {
                s32Ret = HIADP_Tuner_Init();
                if (HI_SUCCESS != s32Ret)
                {
                    sample_common_printf("call HIADP_Tuner_Init failed.\n");
                    return -1;
                }
                printf("HIADP_Tuner_Connect, frequency:%d, Symbol:%d, Qam:%d\n", u32TunerFreq, u32SymbolRate, u32QamType_Ploarization);

                s32Ret = HIADP_Tuner_Connect(TUNER_USE, u32TunerFreq, u32SymbolRate, u32QamType_Ploarization);
                if (HI_SUCCESS != s32Ret)
                {
                    sample_common_printf("call HIADP_Tuner_Connect failed. Ret = 0x%x\n", s32Ret);
                    return -1;
                }
                
                s32Ret = HI_UNF_DMX_AttachTSPort(DEMUX_ID_0, PVR_DMX_PORT_ID_DVB);
                if (s32Ret != HI_SUCCESS)
                {
                    sample_common_printf("call HI_UNF_DMX_AttachTSPort failed. ret=%#x dmxid=%d port=%d\n", s32Ret, DEMUX_ID_0, PVR_DMX_PORT_ID_DVB);
                    return -1;
                }

                s32Ret = HIADP_Search_GetAllPmt(DEMUX_ID_0, &g_pstProgTbl);
                if (HI_SUCCESS != s32Ret)
                {
                    sample_common_printf("call HIADP_Search_GetAllPmt failed.ret=%#x\n",s32Ret);
                    return -1;
                }
            }


            pthread_create(&stUpdateTimeShiftStatusThread, HI_NULL, PVR_UpdateTimeShiftStatusThread, HI_NULL);

            break;
        case PVR_TEST_MODE_PLAYBACK:
            s32Ret = HI_UNF_PVR_PlayInit();
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call HI_UNF_PVR_PlayInit failed.\n");
                return s32Ret;
            }

            s32Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_PLAY_ERROR, PVR_CallBack, NULL);
            s32Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_PLAY_REACH_REC, PVR_CallBack, NULL);
            s32Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_DISKFULL, PVR_CallBack, NULL);
            s32Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_OVER_FIX, PVR_CallBack, NULL);
            s32Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_DISK_SLOW, PVR_CallBack, NULL);
            s32Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_REACH_PLAY, PVR_CallBack, NULL);
            s32Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_ERROR, PVR_CallBack, NULL);
            s32Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_PLAY_EOF, PVR_ProccessStreamEndEvent, NULL);
            s32Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_PLAY_SOF, PVR_ProccessStreamEndEvent, NULL);
            if (s32Ret != HI_SUCCESS)
            {
                sample_common_printf("call PVR_RegisterCallBacks failed.\n");
                return s32Ret;
            }

            for(i = 0; i < (argc - 2); i++)
            {
                PVR_AddAPlayBack(argv[2+i]);
            }

            pthread_create(&stUpdatePlayStatusThread, HI_NULL, PVR_UpdatePlayStatusThread, HI_NULL);
            break;
        case PVR_TEST_MODE_RECORDING:
            s32Ret = HIADP_Tuner_Init();
            if (HI_SUCCESS != s32Ret)
            {
                sample_common_printf("call HIADP_Tuner_Init failed.\n");
                return -1;
            }
            break;
        default:
            break;
    };


    while(HI_FALSE == g_bIsMainRoutineStop)
    {
        printf("Press q to exit, h for help...\n");

        SAMPLE_GET_INPUTCMD(pcInputCmd);

        if('q' == pcInputCmd[0])
        {
            sample_common_printf("Exit main routine!!\n");
            g_bIsMainRoutineStop = HI_TRUE;
            g_bIsEthSocketRecvThreadStop = HI_TRUE;
        }

        switch(u32TestMode)
        {
            case PVR_TEST_MODE_TIMESHIFT:
                PVR_TimeShiftCmdProcess(pcInputCmd, g_sRecPath);
                break;
            case PVR_TEST_MODE_PLAYBACK:
                PVR_PlayBackCmdProcess(pcInputCmd);
                break;
            case PVR_TEST_MODE_RECORDING:
                break;
            default:
                break;
        };

        memset(pcInputCmd, 0 ,sizeof(pcInputCmd));
    }

    pthread_mutex_destroy(&g_stDrawRectMutex);

    switch(u32TestMode)
    {
        case PVR_TEST_MODE_TIMESHIFT:
            pthread_join(stUpdateTimeShiftStatusThread, HI_NULL);

            for(i = 0; i< PVR_MAX_PLAY_CHN; i++)
            {
                if (HI_TRUE == g_stTimeShiftChnAttr[i].bRecChnInited)
                {
                    (HI_VOID)PVR_StopRec(g_stTimeShiftChnAttr[i].u32PvrRecChn,
                                          g_stTimeShiftChnAttr[i].u32RecDemuxId);
                }

                if (HI_TRUE == g_stTimeShiftChnAttr[i].bPlayChnInited)
                {
                    PVR_StopPlay(g_stTimeShiftChnAttr[i].u32PvrPlayChn,
                                  g_stTimeShiftChnAttr[i].u32PlayDemuxId,
                                  g_stTimeShiftChnAttr[i].hTsBufForPlayback);
                }

                if (0 != g_stTimeShiftChnAttr[i].hAvplay)
                {
                    (HI_VOID)PVR_DestroyAvplay(g_stTimeShiftChnAttr[i].hAvplay,
                                                g_stTimeShiftChnAttr[i].hWindow,
                                                g_stTimeShiftChnAttr[i].hSoundTrack);
                }
            }

            if (5 == argc)
            {
                pthread_join(stEthSocketReceiveThread, HI_NULL);
            }
            else
            {
                (HI_VOID)HIADP_Tuner_DeInit();
            }

            (HI_VOID)HI_UNF_PVR_RecDeInit();
            (HI_VOID)HI_UNF_PVR_PlayDeInit();
            break;
        case PVR_TEST_MODE_PLAYBACK:
            pthread_join(stUpdatePlayStatusThread, HI_NULL);

            for(i = 0; i< PVR_MAX_PLAY_CHN; i++)
            {
                if (g_stPlayChnAttr[i].bPlayChnInited)
                {
                    PVR_StopPlay(g_stPlayChnAttr[i].u32PvrPlayChn,
                                  g_stPlayChnAttr[i].u32DemuxId,
                                  g_stPlayChnAttr[i].hTsBufForPlayback);
                }

                if (0 != g_stPlayChnAttr[i].hAvplay)
                {
                    (HI_VOID)PVR_DestroyAvplay(g_stPlayChnAttr[i].hAvplay,
                                                g_stPlayChnAttr[i].hWindow,
                                                g_stPlayChnAttr[i].hSoundTrack);
                }
            }
            (HI_VOID)HI_UNF_PVR_PlayDeInit();
            break;
        case PVR_TEST_MODE_RECORDING:
            break;
        default:
            break;
    };
    pthread_join(stIrReceiveThread, HI_NULL);
    (HI_VOID)HI_UNF_IR_DeInit();
    (HI_VOID)HI_GO_Deinit();
    (HI_VOID)HI_UNF_AVPLAY_DeInit();
    (HI_VOID)HI_UNF_DMX_DeInit();
    (HI_VOID)HI_UNF_VO_DeInit();
    (HI_VOID)HIADP_Disp_DeInit();
    (HI_VOID)HIADP_Snd_DeInit();
    (HI_VOID)HI_SYS_DeInit();

    return 0;
}
