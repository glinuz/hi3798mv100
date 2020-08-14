#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <sys/time.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_ecs.h"
#include "hi_unf_demux.h"
#include "HA.AUDIO.MP3.decode.h"
#include "hi_unf_ecs.h"
#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"

#ifdef  CONFIG_SUPPORT_CA_RELEASE
#define HI_TUNER_ERR(format, arg...)
#define HI_TUNER_INFO(format, arg...)
#define HI_TUNER_WARN(format, arg...)
#else
#define HI_TUNER_ERR(format, arg...)    printf( "[ERR]: %s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_TUNER_INFO(format, arg...)   printf( format , ## arg)
#define HI_TUNER_WARN(format, arg...)   printf( "[WARNING]: %s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#endif

char *asignalTable[] = {"CAB","SAT","DVB-T","DVB-T2","ISDB-T","ATSC-T","DTMB","J83B"};


HI_UNF_DEMOD_DEV_TYPE_E S_DEMOD_TYPE = HI_UNF_DEMOD_DEV_TYPE_3130I;
typedef HI_VOID (*Test_Func_Proc)(char* args);
typedef struct stTEST_FUNC_S
{
    HI_CHAR*         name;
    Test_Func_Proc   proc;
} TEST_FUNC_S;

typedef struct stCMD_HELP_S
{
    HI_CHAR         name[30];
    HI_CHAR         help_info[3000];
} CMD_HELP_S;

CMD_HELP_S g_cmdhelp[] =
{
        {"help", "help: show help menu, only input 'help' will print command list,\n"
                        "\t 'help cmd' will print the help infomation of the cmd.\n"},
        {"setchnl", "setchnl 666 6875000 64 (freqency/symbolrate/qam),\n"
                "\t freqency unit MHz for cable,\n"
                "\t symbolrate unit baud,\n"
                "\t qam 64:64qam  256:256qam.\n"},
        {"getsignalinfo", "getsignalinfo: get detailed infomation of current locked signal.[FOR satellite and terrestrial signal].\n"},
        {"play", "play 513 660: set video pid 513 and audio pid 660\n"},
        {"getmsc", "getmsc: get mosaic num ,arg type: int time,float berlimit\n"},
        {"getber", "getber: get ber\n"},
        {"settype", "settype 0: set tuner type. \n"
                       "\t 0  cd1616,  1  tdae3,  2  mt2081,  3  tdcc,   4  tmx7070x,\n"
                       "\t 5  tda18250, 6  tda18250b 7  mxl203, 8  r820c\n"},
        {"setfreq", "setfreq 403: set freqency(MHz for cable).\n"},
        {"setsymb", "setsymb 6875000: set symbrate, unit baud\n"},
        {"setqam", "setqam  64: set qam type , 64 means 64qam etc.\n"},
        {"getoffset", "getoffset : getfreq getsyb. \n"},
        {"start", "start  500: lock 500 times --->time_file\n"},
        {"setport", "setport 0:Config tuner port index 0.\n"
                      "\t Param1:0 tuner 0,1 tuner 1.\n"},
        {"exit", "exit    : exit the sample\n"}
};

#define MAX_CMD_BUFFER_LEN 256
#define UDP_STR_SPLIT ' '
#define TEST_ARGS_SPLIT " "
#define HI_RESULT_SUCCESS "SUCCESS"
#define HI_RESULT_FAIL "FAIL"
#define DEFAULT_PORT 1234
#define TUNER_NUM 5

static HI_S32 s_s32TunerPort = 0;
/*static HI_S32 s_s32TunerPort = 1;*/
/*static HI_U32 s_u32TunerFreq = 403000;*/
/*static HI_U32 s_u32ErrorNum;*/
/*static HI_FLOAT s_fMskBer = 0.0014;*/
/*static HI_U8 s_au8MskTmp[64];*/
static HI_U32 s_u32CurrentQamType;

/*save results, then send client*/
char s_acTestResult[MAX_CMD_BUFFER_LEN];

static HI_UNF_TUNER_CONNECT_PARA_S  s_stConnectPara[TUNER_NUM];

static HI_UNF_ENC_FMT_E   s_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;

HI_HANDLE phWin;
HI_HANDLE hAvplay;

HI_HANDLE hTrack;

PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;

HI_U32  ProgNum = 0;

FILE *fp = NULL;
static HI_S32 s_s32LoopNum = 0;

static HI_S32 s_s32FailTime =0;
static HI_S32 s_s32Out1Time = 0;
static HI_S32 s_s32OutFailTime = 0;
static HI_S32 s_s32Time = 0;

HI_S32 printime_init()
{

    FILE *time_file = NULL;


    time_file = fopen("time_file", "wt");
    if (NULL == time_file)
    {
        HI_TUNER_ERR("open time_file ,line = %d\n", __LINE__);
        return HI_FAILURE;
    }

    fclose(time_file);

    return HI_SUCCESS;
}
void printtime_file(int locknum, int locktime,int isfail)
{
    FILE *time_file = NULL;

#ifdef  CONFIG_SUPPORT_CA_RELEASE
    time_file = fopen("/tmp/time_file", "at");
#else
    time_file = fopen("time_file", "at");
#endif
    if (NULL == time_file)
    {
        HI_TUNER_INFO("open time_file,line = %d\n", __LINE__);
    }
    if(isfail== 1)
    {
        /*fprintf(time_file, "FAIL: locknum = %d,locktime = %d   isfail = %d\n", locknum, locktime,isfail);*/
        fprintf(time_file, "%d       isfail = %d\n", locktime,isfail);
        s_s32Time =s_s32Time+ locktime;
        s_s32FailTime++;
    }
    else if(isfail == 2)
    {
        s_s32Out1Time++;
        fprintf(time_file, "%d      isfail = %d\n",  locktime,isfail);
        s_s32Time =s_s32Time+ locktime;
    }
    else if(isfail == 3)
    {
        s_s32OutFailTime++;
        if(locktime>1000)
        {
            s_s32Out1Time++;
        }
        fprintf(time_file, "%d      isfail = %d\n",  locktime,isfail);
        s_s32Time =s_s32Time+ locktime;
    }
    else
    {
        /*fprintf(time_file, "OK: locknum = %d,locktime = %d,isfail = %d\n", locknum, locktime,isfail);*/
        fprintf(time_file, "%d   isfail = %d\n", locktime,isfail);
        s_s32Time =s_s32Time+ locktime;

    }

    if(locknum+1>= s_s32LoopNum)
    {

        /*printf("locknum =%d,s_s32LoopNum=%d\n",locknum,s_s32LoopNum);*/
        fprintf(time_file, "locknum =%d,s_s32LoopNum=%d\n",locknum+1,s_s32LoopNum);
        fprintf(time_file, "AVERVAGE TIME : %d\n", s_s32Time/(s_s32LoopNum));
        fprintf(time_file, "FAIL NUM: %d\n", s_s32FailTime);
        fprintf(time_file, " >1s NUM: %d\n", s_s32Out1Time);
        fprintf(time_file, " out fail  NUM: %d\n", s_s32OutFailTime);
    }
    fclose(time_file);

}

HI_U32  getcurtime()
{
    struct timeval tv;
    gettimeofday(&tv, HI_NULL );
    return (((HI_U32)tv.tv_sec)*1000 + ((HI_U32)tv.tv_usec)/1000);
}

HI_VOID set_pin_mux(HI_UNF_TUNER_ATTR_S stTunerAttr)
{
    HI_U8 *pu8MUXAddr = NULL;

    pu8MUXAddr = (HI_U8 *)HI_MEM_Map(0x10203000, 0x1000);
    if (NULL == pu8MUXAddr)
    {
        return;
    }

#if defined (BOARD_TYPE_hi3716mdmo3dvera)
    if (HI_UNF_TUNER_SIG_TYPE_CAB != stTunerAttr.enSigType)
    {
        pu8MUXAddr = (HI_U8 *)HI_MEM_Map(0x10203000, 0x1000);
        if (NULL == pu8MUXAddr)
        {
            return;
        }

        *(pu8MUXAddr + 0x14c) = 0;
    }
#endif

}

//#define BOARD_TYPE_hi3716mdmo3fvera

HI_S32 dev_init()
{
    HI_S32 s32Ret = 0;
    HI_UNF_TUNER_ATTR_S   stTunerAttr;

    set_pin_mux(stTunerAttr);

    /*sys init*/
    HI_SYS_Init();

    HIADP_MCE_Exit();

    /*sound init*/
    s32Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HIADP_Snd_Init failed.\n");
        return s32Ret;
    }

    /*display init*/
    s32Ret = HIADP_Disp_Init(s_enDefaultFmt);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HIADP_Disp_DeInit failed.\n");
        return s32Ret;
    }

    /*vo init*/
    s32Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    s32Ret |= HIADP_VO_CreatWin(HI_NULL,&phWin);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        return s32Ret;
    }

    s32Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HIADP_Tuner_Init failed.\n");
        HIADP_VO_DeInit();
        return s32Ret;
    }

    GET_CONNECT_PARA(s_stConnectPara[0]);
    GET_CONNECT_PARA1(s_stConnectPara[1]);

    s32Ret = HI_UNF_DMX_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_DMX_Init failed.\n");
        HIADP_Tuner_DeInit();
        return s32Ret;
    }

    s32Ret = HIADP_DMX_AttachTSPort(0, 0);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HIADP_DMX_AttachTSPort failed 0x%x\n", s32Ret);
        HI_UNF_DMX_DeInit();
        HIADP_Tuner_DeInit();
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 dev_deinit()
{
    HI_S32 s32Ret = 0;

    s32Ret = HI_UNF_DMX_DetachTSPort(0);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_DMX_DetachTSPort failed.\n");
        return s32Ret;
    }

    s32Ret = HI_UNF_DMX_DeInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_DMX_AttachTSPort failed.\n");
        return s32Ret;
    }

    HIADP_Tuner_DeInit();

    s32Ret = HI_UNF_VO_DestroyWindow(phWin);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_VO_DestroyWindow failed.\n");
        return s32Ret;
    }

#if 0
    s32Ret = HI_UNF_VO_Close(HI_UNF_DISPLAY1);
     if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_VO_Close failed.\n");
        return s32Ret;
    }
#endif
    s32Ret = HI_UNF_VO_DeInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_VO_DeInit failed.\n");
        return s32Ret;
    }

    s32Ret = HI_UNF_DISP_Close(HI_UNF_DISPLAY0);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_DISP_Close failed.\n");
        return s32Ret;
    }

    s32Ret = HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_DISP_Close failed.\n");
        return s32Ret;
    }

    s32Ret = HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_DISP_Detach failed.\n");
        return s32Ret;
    }

    s32Ret = HI_UNF_DISP_DeInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_DISP_DeInit failed.\n");
        return s32Ret;
    }

    /*aaa hdmiDeInit();*/
    s32Ret = HI_UNF_SND_Close(HI_UNF_SND_0);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_SND_Close failed.\n");
        return s32Ret;
    }

    s32Ret = HI_UNF_SND_DeInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_SND_DeInit failed.\n");
        return s32Ret;
    }

    HI_SYS_DeInit();

    return HI_SUCCESS;
}

HI_S32 avplay_Init()
{
    HI_S32 s32Ret = 0;

    HI_UNF_AVPLAY_ATTR_S        stAvplayAttr;
    HI_UNF_SYNC_ATTR_S          stSyncAttr;
    HI_UNF_VCODEC_ATTR_S        stVdecAttr;
    HI_UNF_ACODEC_ATTR_S        stAdecAttr;
    HI_UNF_AUDIOTRACK_ATTR_S    stTrackAttr;

    /*register lib*/
    s32Ret = HIADP_AVPlay_RegADecLib();
    s32Ret |= HI_UNF_AVPLAY_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_WARN("call HI_UNF_AVPLAY_Init failed.\n");
        dev_deinit();
    }

    s32Ret = HI_UNF_AVPLAY_GetDefaultConfig(&stAvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_GetDefaultConfig failed.\n");
        HI_UNF_AVPLAY_DeInit();
        dev_deinit();
        return s32Ret;
    }
    stAvplayAttr.stStreamAttr.u32VidBufSize = 0x300000;
    s32Ret = HI_UNF_AVPLAY_Create(&stAvplayAttr, &hAvplay);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_Create failed.\n");
        HI_UNF_AVPLAY_DeInit();
        dev_deinit();
        return s32Ret;
    }

    s32Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        dev_deinit();
        return s32Ret;
    }

    s32Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        dev_deinit();
        return s32Ret;
    }

    s32Ret = HI_UNF_VO_AttachWindow(phWin, hAvplay);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_VO_AttachWindow failed.\n");
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        dev_deinit();
        return s32Ret;
    }

    s32Ret = HI_UNF_VO_SetWindowEnable(phWin, HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_VO_SetWindowEnable failed.\n");
        HI_UNF_VO_DetachWindow(phWin, hAvplay);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        dev_deinit();
        return s32Ret;
    }

    //stTrackAttr.enTrackType = HI_UNF_SND_TRACK_TYPE_MASTER;
    s32Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (s32Ret != HI_SUCCESS)
    {
        HI_TUNER_ERR("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        return s32Ret;
    }
    s32Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_SND_CreateTrack failed.\n");
        HI_UNF_VO_SetWindowEnable(phWin, HI_FALSE);
        HI_UNF_VO_DetachWindow(phWin, hAvplay);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        dev_deinit();
        return s32Ret;
    }

    s32Ret = HI_UNF_SND_Attach(hTrack, hAvplay);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_SND_Attach failed.\n");
        HI_UNF_SND_DestroyTrack(hTrack);
        HI_UNF_VO_SetWindowEnable(phWin, HI_FALSE);
        HI_UNF_VO_DetachWindow(phWin, hAvplay);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        dev_deinit();
        return s32Ret;
    }

    HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVdecAttr);
    stAdecAttr.enType = HA_AUDIO_ID_MP3;
    stAdecAttr.stDecodeParam.enDecMode = HD_DEC_MODE_RAWPCM;
    stAdecAttr.stDecodeParam.pCodecPrivateData = HI_NULL;
    stAdecAttr.stDecodeParam.u32CodecPrivateDataSize = 0;
    stAdecAttr.stDecodeParam.sPcmformat.u32DesiredOutChannels = 2;
    stAdecAttr.stDecodeParam.sPcmformat.bInterleaved  = HI_TRUE;
    stAdecAttr.stDecodeParam.sPcmformat.u32BitPerSample = 16;
    stAdecAttr.stDecodeParam.sPcmformat.u32DesiredSampleRate = 48000;
    s32Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_ADEC, &stAdecAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_SetAttr_ADEC failed.\n");
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        dev_deinit();
        return s32Ret;
    }

    HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVdecAttr);
    stVdecAttr.enMode = HI_UNF_VCODEC_MODE_NORMAL;
#if defined(ISDBT_IT9170)
    stVdecAttr.enType = HI_UNF_VCODEC_TYPE_H264;
#else
    stVdecAttr.enType = HI_UNF_VCODEC_TYPE_MPEG2;
#endif
    stVdecAttr.u32ErrCover = 80;
    stVdecAttr.u32Priority = HI_UNF_VCODEC_MAX_PRIORITY;
    stVdecAttr.bOrderOutput = HI_FALSE;
    s32Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVdecAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_SetAttr_VDEC failed.\n");
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        dev_deinit();
        return s32Ret;
    }

    s32Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_SetAttr failed.\n");
        HI_UNF_SND_Detach(hTrack, hAvplay);
        HI_UNF_SND_DestroyTrack(hTrack);
        HI_UNF_VO_SetWindowEnable(phWin, HI_FALSE);
        HI_UNF_VO_DetachWindow(phWin, hAvplay);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        dev_deinit();
        return s32Ret;
    }
    stSyncAttr.enSyncRef = HI_UNF_SYNC_REF_PCR;
    stSyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
    stSyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
    stSyncAttr.bQuickOutput = HI_FALSE;
    s32Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_SetAttr failed.\n");
        HI_UNF_SND_Detach(hTrack, hAvplay);
        HI_UNF_SND_DestroyTrack(hTrack);
        HI_UNF_VO_SetWindowEnable(phWin, HI_FALSE);
        HI_UNF_VO_DetachWindow(phWin, hAvplay);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        dev_deinit();
        return s32Ret;
    }

    /*aaa AudioLineOutMuteCntrDisable();*/

    s32Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_INFO("call HI_UNF_AVPLAY_Start_AUD failed.\n");
        /*return ret;*/
    }

    s32Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_Start_VID failed.\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 avplay_deinit()
{
    HI_S32 s32Ret = 0;
    HI_UNF_AVPLAY_STOP_OPT_S    stStop;

    stStop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    stStop.u32TimeoutMs = 0;

    s32Ret = HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &stStop);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_WARN("call HI_UNF_AVPLAY_Stop failed.\n");
    }

    s32Ret = HI_UNF_SND_Detach(hTrack, hAvplay);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_WARN("call HI_UNF_SND_Detach failed.\n");
    }

    s32Ret = HI_UNF_SND_DestroyTrack(hTrack);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_WARN("call HI_UNF_SND_DestroyTrack failed.\n");
    }

    s32Ret = HI_UNF_VO_SetWindowEnable(phWin,HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_WARN("call HI_UNF_VO_SetWindowEnable failed.\n");
    }

    s32Ret = HI_UNF_VO_DetachWindow(phWin, hAvplay);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_WARN("call HI_UNF_VO_DetachWindow failed.\n");
    }

    s32Ret= HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_WARN("call HI_UNF_AVPLAY_ChnClose failed.\n");
    }

    s32Ret = HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_WARN("call HI_UNF_AVPLAY_ChnClose failed.\n");
    }

    s32Ret = HI_UNF_AVPLAY_Destroy(hAvplay);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_WARN("call HI_UNF_AVPLAY_Destroy failed.\n");
    }

    s32Ret = HI_UNF_AVPLAY_DeInit();
   if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_WARN("call HI_UNF_AVPLAY_DeInit failed.\n");
    }

    return HI_SUCCESS;
}


/*lock freq function*/
HI_S32 hi_tuner_connect()
{
    HI_S32 s32Ret = HI_FAILURE;
    //HI_UNF_TUNER_SIGNALINFO_S stInfo;
    HI_UNF_TUNER_STATUS_S stTunerStatus;
    HI_U32 u32Loop = 0;
    HI_U32 u32LoopTimes = 100;
    HI_U32 u32Freq = 0;
    HI_U32 u32SymbolRate = 0;
    HI_CHAR InputCmd[32];

    s32Ret = HI_UNF_TUNER_Connect(s_s32TunerPort, &s_stConnectPara[s_s32TunerPort], 0);
    u32Freq = s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32Freq;
    u32SymbolRate = s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32SymbolRate;

    if (HI_SUCCESS == s32Ret)
    {
        for (u32Loop = 0; u32Loop < u32LoopTimes; u32Loop++)
        {
            s32Ret = HI_UNF_TUNER_GetStatus(s_s32TunerPort, &stTunerStatus);
            if (HI_UNF_TUNER_SIGNAL_LOCKED == stTunerStatus.enLockStatus)
            {
                    switch (s_stConnectPara[s_s32TunerPort].enSigType)
                    {
                        case HI_UNF_TUNER_SIG_TYPE_CAB:
                        case HI_UNF_TUNER_SIG_TYPE_J83B:
                            printf("Tuner   Lock freq %d symb %d  qam%d Success!\n",
                               u32Freq, u32SymbolRate, s_u32CurrentQamType);

                            /*automatically play the first program after locked successfully*/
                            //printf("SUCCESS end\n");
                            break;
                        case HI_UNF_TUNER_SIG_TYPE_SAT:
                            printf("Tuner   Lock freq %d symb %d Success!\n",
                               u32Freq, u32SymbolRate);
                            //printf("SUCCESS end\n");
                            break;
                        default:
                            break;
                    }
                
                    HIADP_Search_Init();
                    s32Ret = HIADP_Search_GetAllPmt(0,&g_pProgTbl);
                    if (HI_SUCCESS != s32Ret)
                    {
                        HI_TUNER_ERR("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
                        return s32Ret;
                    }
                    ProgNum = 0;
                    s32Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum,HI_TRUE);
                    if (s32Ret != HI_SUCCESS)
                    {
                        printf("call SwitchProg failed.\n");
                        return HI_FAILURE;
                    }


                    while(1)
                    {
                        printf("please input the q to quit!\n");
                        fgets(InputCmd, 30, stdin);

                        if ('q' == InputCmd[0])
                        {
                            printf("prepare to quit!\n");
                            break;
                        }

                        ProgNum = atoi(InputCmd);

                        if(ProgNum == 0)
                            ProgNum = 1;

                        s32Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum-1,HI_TRUE);
                        if (s32Ret != HI_SUCCESS)
                        {
                            printf("call SwitchProgfailed!\n");
                            break;
                        }

                    }


                    strcpy(s_acTestResult, HI_RESULT_SUCCESS);
                    return HI_SUCCESS;
        }
            else
            {
                usleep(10000);
            }
        }
    }
    else
    {
        switch (s_stConnectPara[s_s32TunerPort].enSigType)
        {
            case HI_UNF_TUNER_SIG_TYPE_CAB:
            case HI_UNF_TUNER_SIG_TYPE_J83B:
                printf("Tuner Lock freq %d symb %d  qam%d Fail!, s32Ret = 0x%x\n",
                       u32Freq, u32SymbolRate, s_u32CurrentQamType, s32Ret);
                break;

            case HI_UNF_TUNER_SIG_TYPE_SAT:
                printf("Tuner Lock freq %d symb %d  Fail!, s32Ret = 0x%x\n",
                       u32Freq, u32SymbolRate, s32Ret);
                break;

            default:
                break;
        }
    }

    if (u32Loop == u32LoopTimes)
    {
        switch (s_stConnectPara[s_s32TunerPort].enSigType)
        {
            case HI_UNF_TUNER_SIG_TYPE_CAB:
            case HI_UNF_TUNER_SIG_TYPE_J83B:
                printf("Tuner Lock freq %d symb %d  qam%d Fail!\n",
                       u32Freq, u32SymbolRate, s_u32CurrentQamType);
                break;

            case HI_UNF_TUNER_SIG_TYPE_SAT:
                printf("Tuner Lock freq %d symb %d  Fail!\n",
                       u32Freq, u32SymbolRate);
                break;

            default:
                break;
        }
    }

    printf("FAIL end\n");
    strcpy(s_acTestResult,HI_RESULT_FAIL);
    return HI_FAILURE;
}

HI_VOID hi_tuner_start(char * locktime)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_UNF_TUNER_STATUS_S stTunerStatus;
    HI_UNF_TUNER_CONNECT_PARA_S  stTmpConnectPara;
    HI_U32 u32Loop = 0;
    HI_U32 u32StatTime = 0;
    HI_U32 u32EndTime = 0;
    HI_U32 u32TempTime = 0;
    HI_U32 u32IndexLockTime;

    if (HI_NULL_PTR == locktime)
    {
        HI_TUNER_ERR("please input loctime count\n");
        return;
    }
    HI_U32 u32TotalLockTime = atoi(locktime);

    memcpy(&stTmpConnectPara,&s_stConnectPara[s_s32TunerPort],sizeof(HI_UNF_TUNER_CONNECT_PARA_S));

    s_s32LoopNum = u32TotalLockTime;

    s_s32FailTime =0;
    s_s32Out1Time = 0;
    s_s32OutFailTime = 0;
    s_s32Time = 0;

    s32Ret = printime_init();
    if (HI_FAILURE == s32Ret)
    {
        HI_TUNER_ERR("printime_init failure\n");
        return;
    }

    for(u32IndexLockTime = 0; u32IndexLockTime < u32TotalLockTime; u32IndexLockTime++)
    {
           u32StatTime = getcurtime();
           s32Ret = HI_UNF_TUNER_Connect(s_s32TunerPort, &s_stConnectPara[s_s32TunerPort], 0);
            if (HI_SUCCESS == s32Ret)
            {
                for (u32Loop = 0; u32Loop < 300; u32Loop++)
                {
                    s32Ret = HI_UNF_TUNER_GetStatus(s_s32TunerPort, &stTunerStatus);
                    if (HI_UNF_TUNER_SIGNAL_LOCKED == stTunerStatus.enLockStatus)
                    {
                        u32EndTime = getcurtime();
                        u32TempTime = u32EndTime - u32StatTime;
                    if(u32TempTime > 1000)
                    {
                        HI_TUNER_INFO("===111===IndexLockTime=%d,locktime=%d\n",u32IndexLockTime,u32TempTime);
                        printtime_file(u32IndexLockTime, u32TempTime,2);
                    }
                    else
                    {
                        HI_TUNER_INFO("===000===IndexLockTime=%d,locktime=%d\n",u32IndexLockTime,u32TempTime);
                        printtime_file(u32IndexLockTime, u32TempTime,0);
                    }

                        HI_TUNER_INFO("SUCCESS end\n");
                        strcpy(s_acTestResult,HI_RESULT_SUCCESS);
                        break;
                    }
                    else
                    {
                        usleep(10000);
                    }
                }
            }
            else
            {
            HI_TUNER_WARN("Tuner Lock freq %d symb %d  qam%d Fail!, s32Ret = 0x%x\n",
                   s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32Freq,
                   s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32SymbolRate, s_u32CurrentQamType, s32Ret);
            }

            if (u32Loop == 300)
            {
            HI_TUNER_WARN("Tuner Lock freq %d symb %d  qam%d time out , s32Ret = 0x%x\n",
                   s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32Freq,
                 s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32SymbolRate, s_u32CurrentQamType, s32Ret);

                 u32EndTime = getcurtime();
                 u32TempTime = u32EndTime - u32StatTime;
                 printtime_file(u32IndexLockTime, u32TempTime, 3);
            }

            if((u32IndexLockTime%2) == 0)
            {

                stTmpConnectPara.unConnectPara.stCab.u32Freq =  s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32Freq+8*1000;
                stTmpConnectPara.unConnectPara.stCab.enModType =  s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.enModType;
                stTmpConnectPara.unConnectPara.stCab.u32SymbolRate =  s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32SymbolRate;

                HI_UNF_TUNER_Connect(s_s32TunerPort, &stTmpConnectPara, 500);

                usleep(200000);
            }
    }
}

/* set frequency and call hi_tuner_connect */
HI_VOID hi_tuner_setfreq(char * freq)
{
    if (freq == HI_NULL_PTR)
    {
        return;
    }
    s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32Freq = atoi(freq) * 1000;
    hi_tuner_connect();
}

/* set symbol rate and call hi_tuner_connect */
HI_VOID hi_tuner_setsymb(char * symb)
{
    if (symb == HI_NULL_PTR)
    {
        return;
    }

    if (s_stConnectPara[s_s32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
    {
        s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32SymbolRate = atoi(symb);
    }
    else if (s_stConnectPara[s_s32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {
        s_stConnectPara[s_s32TunerPort].unConnectPara.stSat.u32SymbolRate = atoi(symb);
    }
    else
    {
        printf("Error signal type!\n");
    }

    hi_tuner_connect();
}
/* set  type of modulation and call hi_tuner_connect */
HI_VOID hi_tuner_setqam(char * qam)
{
    HI_S32 s32GetQam;

    if (qam == HI_NULL_PTR)
    {
        return;
    }

    s32GetQam = atoi(qam);
    s_u32CurrentQamType = s32GetQam;
    switch (s32GetQam)
    {
        case 64:
        {
            if(HI_UNF_DEMOD_DEV_TYPE_J83B == S_DEMOD_TYPE)
            {
                s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32SymbolRate = 5057000;
            }
            else
            {
                s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32SymbolRate = 6875000;
            }
            s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64;
            break;
        }
        case 256:
        {
            if(HI_UNF_DEMOD_DEV_TYPE_J83B == S_DEMOD_TYPE)
            {
                s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32SymbolRate = 5361000;
            }
            else
            {
                s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32SymbolRate = 6875000;
            }
            s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_256;
            break;
        }
        case 16:
            s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_16;
            break;
        case 32:
            s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_32;
            break;
        case 128:
            s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_128;
            break;
        default:
            s_u32CurrentQamType = 64;
            s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64;
            break;
    }

    hi_tuner_connect();
}

#if 1
HI_VOID hi_tuner_getmsc(char *arg)
{
    HI_S32 s32Ret;

    HI_S32 i;
    HI_S32 s32Time;
   HI_UNF_AVPLAY_STATUS_INFO_S stStatusInfoStart;
   HI_UNF_AVPLAY_STATUS_INFO_S stStatusInfoEnd;

    /*HI_U32 u32ErrNumStart = 0;
    HI_U32 u32TotalNumStart = 0;
    HI_U32 u32ErrNumEnd = 0;
    HI_U32 u32TotalNumEnd = 0;*/
    HI_U32 u32ErrNum;
    HI_S32 s32TotalErrNum = 0;
    if(arg == 0)
    {
        s32Time = 1;
    }
    else
    {
        sscanf(arg,"%d ",&s32Time);
    }

    /*msc_appear_times = 0;*/
    for(i = 0; i < s32Time; i++)
    {

        s32Ret = HI_UNF_AVPLAY_GetStatusInfo( hAvplay, &stStatusInfoStart);
    //s32Ret =  HI_MPI_VDEC_GetChanStatusInfo(0x260000, &stVdecStatInfoStart );

        sleep(1);

     s32Ret |= HI_UNF_AVPLAY_GetStatusInfo( hAvplay, &stStatusInfoEnd);
      //  s32Ret |= HI_MPI_VDEC_GetChanStatusInfo(0x260000, &stVdecStatInfoEnd );

        /*u32ErrNum = u32ErrNumEnd - u32ErrNumStart;*/
        //u32ErrNum = stVdecStatInfoEnd.u32TotalErrFrmNum - stVdecStatInfoStart.u32TotalErrFrmNum;
        u32ErrNum = stStatusInfoEnd.u32VidErrorFrameCount - stStatusInfoStart.u32VidErrorFrameCount;
        if(HI_SUCCESS == s32Ret)
        {
            if( u32ErrNum > 0 )
            {
                HI_TUNER_INFO("---SUCCESS%d end\n", u32ErrNum );
                s32TotalErrNum = s32TotalErrNum + u32ErrNum;
                sprintf(s_acTestResult,HI_RESULT_SUCCESS"%d", u32ErrNum );
                return;
            }
            /*else if( u32TotalNumStart == u32TotalNumEnd )*/
           // else if( stVdecStatInfoStart.u32TotalDecFrmNum == stVdecStatInfoEnd.u32TotalDecFrmNum )
            else if( stStatusInfoStart.u32VidFrameCount == stStatusInfoEnd.u32VidFrameCount )
            {
                HI_TUNER_ERR("SUCCESS%d end\n",25);
                sprintf(s_acTestResult,HI_RESULT_SUCCESS"%d",25);
                return;
            }
        }
        else
        {
            HI_TUNER_ERR("HI_VID_GetErrorFrameNum Faild %x!\n",s32Ret);
            HI_TUNER_ERR("FAIL end\n");
            strcpy(s_acTestResult,HI_RESULT_FAIL);
            return;
        }
    }

    HI_TUNER_INFO("SUCCESS%d end\n",0);
    sprintf(s_acTestResult,HI_RESULT_SUCCESS"%d",0);

    return ;
}
#endif
#if 0
/* not be used temporarily */
HI_VOID hi_tuner_getmsc_ber(char * arg)
{
    HI_S32 s32Ret;
    HI_U32 au32Ber[3];
    HI_U32 au32MskTmp[3];
    HI_DOUBLE dRealBer;
    struct timeval stBtv;
    struct timeval stEtv;
    HI_S32 s32UseTime;
    HI_S32 s32SetTime = 0;
    HI_S32 s32MskNum = 0;
    HI_FLOAT fMskBer = 0.0;
    HI_FLOAT fAvgBer = 0.0;
    HI_S32 i = 0;

    if(arg == 0)
    {
        s32SetTime = 2;
    }
    else
    {
        sscanf(arg,"%d %f",&s32SetTime,&fMskBer);
    }

    if(fMskBer < 0.0000001)
    {
        fMskBer = s_fMskBer;
    }

    gettimeofday(&stBtv,0);
    while(1)
    {
        s32Ret = HI_UNF_TUNER_GetBER(s_s32TunerPort, au32Ber);
        if(HI_SUCCESS == s32Ret)
        {
            sprintf((char *)au32MskTmp, "%d.%de-%d", au32Ber[0], au32Ber[1], au32Ber[2]);
            dRealBer = strtod((char *)au32MskTmp, NULL);
#if 0
            u32ber = (ber[0]<<16)|(ber[1]<<8)|ber[2];
            realber = u32ber /8388608.0;
#endif
            /*printf("ber :%f\n", realber);*/
            i++;
            fAvgBer = (fAvgBer * (i - 1) + dRealBer) / i;

            /*printf("HI_TUNER_GetBER ber:%10.6e    avg:%10.6e\n",realber,avgber);*/
            gettimeofday(&stEtv,0);
            s32UseTime = (stEtv.tv_sec - stBtv.tv_sec) * 1000 + (stEtv.tv_usec - stBtv.tv_usec) / 1000;
            if(fAvgBer > fMskBer)
            {
                s32MskNum = (int)(fAvgBer / fMskBer);
                if(s32SetTime > 10)
                {
                    HI_TUNER_INFO("SUCCESS%d end\n",s32MskNum);
                    break;
                }
            }

            if(s32UseTime >= s32SetTime * 1000)
            {
                HI_TUNER_INFO("SUCCESS%d end\n",s32MskNum);
                break;
            }
        }
        else
        {
            gettimeofday(&stEtv,0);
            s32UseTime = (stEtv.tv_sec - stBtv.tv_sec) * 1000 + (stEtv.tv_usec - stBtv.tv_usec) /1000;
            if(s32UseTime >= 2000)
            {
                HI_TUNER_WARN("NOTLOCK end\n");
                break;
            }

            usleep(250000);
        }

        /*usleep(100000);*/
    }

    return;
}
#endif

HI_VOID hi_tuner_settype(char * type)
{
    HI_UNF_TUNER_ATTR_S stTunerAttr;
    HI_S32 s32TunerType;
    HI_S32 s32Ret = HI_SUCCESS;

    if (type == HI_NULL_PTR)
    {
        return;
    }

    s32Ret = HI_UNF_TUNER_GetAttr(s_s32TunerPort,&stTunerAttr);
    if( HI_SUCCESS != s32Ret) return;

    s32TunerType = atoi(type);

    switch (s32TunerType)
    {
        case 0:
        stTunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_CD1616;
            break;

        case 1:
        stTunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_ALPS_TDAE;
            break;

        case 2:
        stTunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_MT2081;
            break;

        case 3:
        stTunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_TDCC;
            break;

        case 4:
        stTunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_TMX7070X;
            break;

        case 5:
        stTunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_TDA18250;
            break;

        case 6:
        stTunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_TDA18250B;
            break;
        case 7:
        stTunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_MXL203;
            break;
        case 8:
        stTunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_R820C;
            break;

        default:
        stTunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_CD1616;
            break;
    }

    (HI_VOID)HI_UNF_TUNER_SetAttr(s_s32TunerPort, &stTunerAttr);
    hi_tuner_connect();
}

HI_VOID hi_tuner_getsignalinfo(char * para)
{
    HI_S32 s32Ret = 0;
    HI_UNF_TUNER_SIGNALINFO_S stInfo;
    HI_U32 u32BER;
    HI_U32 u32SNR;
    HI_U32 u32SignalStrength;
    HI_U32 u32SignalQuality;

    s32Ret = HI_UNF_TUNER_GetBER(s_s32TunerPort, &u32BER);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("HI_UNF_TUNER_GetBER failed\n");
    }
    HI_TUNER_INFO("BER:\t\t\t%d\n", u32BER);
    s32Ret = HI_UNF_TUNER_GetSNR(s_s32TunerPort, &u32SNR);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("HI_UNF_TUNER_GetSNR failed\n");
    }
    HI_TUNER_INFO("SNR:\t\t\t%d\n", u32SNR);

    s32Ret = HI_UNF_TUNER_GetSignalStrength(s_s32TunerPort, &u32SignalStrength);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("HI_UNF_TUNER_GetSignalStrength failed\n");
    }
    HI_TUNER_INFO("Signal Strength:\t%ddBuv\n", u32SignalStrength);

    s32Ret = HI_UNF_TUNER_GetSignalQuality(s_s32TunerPort, &u32SignalQuality);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("HI_UNF_TUNER_GetSignalQuality failed\n");
    }
    HI_TUNER_INFO("Signal Quality:\t\t%d%%\n", u32SignalQuality);

    if ((HI_UNF_TUNER_SIG_TYPE_CAB <= s_stConnectPara[s_s32TunerPort].enSigType) ||
        (HI_UNF_TUNER_SIG_TYPE_J83B >= s_stConnectPara[s_s32TunerPort].enSigType))
    {
        s32Ret = HI_UNF_TUNER_GetSignalInfo(s_s32TunerPort, &stInfo);
        if (s32Ret != HI_SUCCESS)
        {
            HI_TUNER_ERR("call HI_UNF_TUNER_GetSignalInfo failed.\n");
            return;
        }
        
        switch (stInfo.enSigType)
        {
            case HI_UNF_TUNER_SIG_TYPE_CAB:
            case HI_UNF_TUNER_SIG_TYPE_J83B:
                HI_TUNER_INFO("Signal type:\t\tCable\n");
                break;
            case HI_UNF_TUNER_SIG_TYPE_DVB_T:
            case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
            case HI_UNF_TUNER_SIG_TYPE_ISDB_T:
            case HI_UNF_TUNER_SIG_TYPE_ATSC_T:
            case HI_UNF_TUNER_SIG_TYPE_DTMB:
                HI_TUNER_INFO("Signal type:\t\tTerrestrial\n");
                break;
            case HI_UNF_TUNER_SIG_TYPE_SAT:
                HI_TUNER_INFO("Signal type:\t\tSatellite\n");
                break;
            case HI_UNF_TUNER_SIG_TYPE_BUTT:
            default:
                HI_TUNER_INFO("Signal type:\t\tUnknown\n");
                break;
        }

        if (HI_UNF_TUNER_SIG_TYPE_SAT == stInfo.enSigType)
        {
            switch (stInfo.unSignalInfo.stSat.enModType)
            {
            case HI_UNF_MOD_TYPE_QAM_16:
            case HI_UNF_MOD_TYPE_QAM_32:
            case HI_UNF_MOD_TYPE_QAM_64:
            case HI_UNF_MOD_TYPE_QAM_128:
            case HI_UNF_MOD_TYPE_QAM_256:
            case HI_UNF_MOD_TYPE_QAM_512:
                HI_TUNER_INFO("Modulation type: \tQAM\n");
                break;
            case HI_UNF_MOD_TYPE_BPSK:
                HI_TUNER_INFO("Modulation type: \tBPSK\n");
                break;
            case HI_UNF_MOD_TYPE_QPSK:
                HI_TUNER_INFO("Modulation type: \tQPSK\n");
                break;
            case HI_UNF_MOD_TYPE_8PSK:
                HI_TUNER_INFO("Modulation type: \t8PSK\n");
                break;
            case HI_UNF_MOD_TYPE_16APSK:
                HI_TUNER_INFO("Modulation type: \t16APSK\n");
                break;
            case HI_UNF_MOD_TYPE_32APSK:
                HI_TUNER_INFO("Modulation type: \t32APSK\n");
                break;
            case HI_UNF_MOD_TYPE_DEFAULT:
            case HI_UNF_MOD_TYPE_AUTO:
            case HI_UNF_MOD_TYPE_BUTT:
            default:
                HI_TUNER_INFO("Modulation type: \tUnknown\n");
                break;
            }

            switch (stInfo.unSignalInfo.stSat.enSATType)
            {
            case HI_UNF_TUNER_FE_DVBS:
                HI_TUNER_INFO("FEC type:\t\tDVBS\n");
                break;
            case HI_UNF_TUNER_FE_DVBS2:
                HI_TUNER_INFO("FEC type:\t\tDVBS2\n");
                break;
            case HI_UNF_TUNER_FE_DIRECTV:
                HI_TUNER_INFO("FEC type:\t\tDIRECTV\n");
                break;
            case HI_UNF_TUNER_FE_BUTT:
            default:
                HI_TUNER_INFO("FEC type:\t\tUnknown\n");
                break;
            }

            switch (stInfo.unSignalInfo.stSat.enFECRate)
            {
            case HI_UNF_TUNER_FE_FEC_1_2:
                HI_TUNER_INFO("FEC rate:\t\t1/2\n");
                break;
            case HI_UNF_TUNER_FE_FEC_2_3:
                HI_TUNER_INFO("FEC rate:\t\t2/3\n");
                break;
            case HI_UNF_TUNER_FE_FEC_3_4:
                HI_TUNER_INFO("FEC rate:\t\t3/4\n");
                break;
            case HI_UNF_TUNER_FE_FEC_4_5:
                HI_TUNER_INFO("FEC rate:\t\t4/5\n");
                break;
            case HI_UNF_TUNER_FE_FEC_5_6:
                HI_TUNER_INFO("FEC rate:\t\t5/6\n");
                break;
            case HI_UNF_TUNER_FE_FEC_6_7:
                HI_TUNER_INFO("FEC rate:\t\t6/7\n");
                break;
            case HI_UNF_TUNER_FE_FEC_7_8:
                HI_TUNER_INFO("FEC rate:\t\t7/8\n");
                break;
            case HI_UNF_TUNER_FE_FEC_8_9:
                HI_TUNER_INFO("FEC rate:\t\t8/9\n");
                break;
            case HI_UNF_TUNER_FE_FEC_9_10:
                HI_TUNER_INFO("FEC rate:\t\t9/10\n");
                break;
            case HI_UNF_TUNER_FE_FEC_1_4:
                HI_TUNER_INFO("FEC rate:\t\t1/4\n");
                break;
            case HI_UNF_TUNER_FE_FEC_2_5:
                HI_TUNER_INFO("FEC rate:\t\t2/5\n");
                break;
            case HI_UNF_TUNER_FE_FEC_3_5:
                HI_TUNER_INFO("FEC rate:\t\t3/5\n");
                break;
            case HI_UNF_TUNER_FE_FECRATE_BUTT:
            case HI_UNF_TUNER_FE_FEC_AUTO:
            default:
                HI_TUNER_INFO("FEC rate:\t\tUnknown\n");
                break;
            }

        	switch(stInfo.unSignalInfo.stSat.enCodeModulation)
        	{
        		case HI_UNF_TUNER_CODE_MODULATION_VCM_ACM:
        			HI_TUNER_INFO("Code and Modulation:\tVCM/ACM.\n");
        			break;
        		case HI_UNF_TUNER_CODE_MODULATION_CCM:
        		default:
        			HI_TUNER_INFO("Code and Modulation:\tCCM.\n");
        			break;
        	}
        }
        else if (HI_UNF_TUNER_SIG_TYPE_DVB_T <= stInfo.enSigType
                && HI_UNF_TUNER_SIG_TYPE_DTMB >= stInfo.enSigType)
        {
            switch (stInfo.unSignalInfo.stTer.enFECRate)
            {
                case HI_UNF_TUNER_FE_FEC_1_2:
                    printf("FEC rate:\t\t1/2\n");
                    break;
                case HI_UNF_TUNER_FE_FEC_2_3:
                    printf("FEC rate:\t\t2/3\n");
                    break;
                case HI_UNF_TUNER_FE_FEC_3_4:
                    printf("FEC rate:\t\t3/4\n");
                    break;
                case HI_UNF_TUNER_FE_FEC_4_5:
                    printf("FEC rate:\t\t4/5\n");
                    break;
                case HI_UNF_TUNER_FE_FEC_5_6:
                    printf("FEC rate:\t\t5/6\n");
                    break;
                case HI_UNF_TUNER_FE_FEC_6_7:
                    printf("FEC rate:\t\t6/7\n");
                    break;
                case HI_UNF_TUNER_FE_FEC_7_8:
                    printf("FEC rate:\t\t7/8\n");
                    break;
                case HI_UNF_TUNER_FE_FEC_8_9:
                    printf("FEC rate:\t\t8/9\n");
                    break;
                case HI_UNF_TUNER_FE_FEC_9_10:
                    printf("FEC rate:\t\t9/10\n");
                    break;
                case HI_UNF_TUNER_FE_FEC_1_4:
                    printf("FEC rate:\t\t1/4\n");
                    break;
                case HI_UNF_TUNER_FE_FEC_1_3:
                    printf("FEC rate:\t\t1/3\n");
                    break;
                case HI_UNF_TUNER_FE_FEC_2_5:
                    printf("FEC rate:\t\t2/5\n");
                    break;
                case HI_UNF_TUNER_FE_FEC_3_5:
                    printf("FEC rate:\t\t3/5\n");
                    break;
                case HI_UNF_TUNER_FE_FECRATE_BUTT:
                case HI_UNF_TUNER_FE_FEC_AUTO:
                default:
                    printf("FEC rate:\t\tUnknown\n");
                    break;
            }

            switch (stInfo.unSignalInfo.stTer.enGuardIntv)
            {
                case HI_UNF_TUNER_FE_GUARD_INTV_1_128:
                    printf("GI:\t\t\t1/128\n");
                    break;
                case HI_UNF_TUNER_FE_GUARD_INTV_1_32:
                    printf("GI:\t\t\t1/32\n");
                    break;
                case HI_UNF_TUNER_FE_GUARD_INTV_1_16:
                    printf("GI:\t\t\t1/16\n");
                    break;
                case HI_UNF_TUNER_FE_GUARD_INTV_1_8:
                    printf("GI:\t\t\t1/8\n");
                    break;
                case HI_UNF_TUNER_FE_GUARD_INTV_1_4:
                    printf("GI:\t\t\t1/4\n");
                    break;
                default:
                    printf("GI:\t\tUnknown\n");
                    break;
            }

            switch (stInfo.unSignalInfo.stTer.enModType)
            {
                case HI_UNF_MOD_TYPE_QPSK:
                    printf("ModType:\t\tQPSK\n");
                    break;
                case HI_UNF_MOD_TYPE_QAM_16:
                    printf("ModType:\t\tQAM_16\n");
                    break;
                case HI_UNF_MOD_TYPE_QAM_64:
                    printf("ModType:\t\tQAM_64\n");
                    break;
                case HI_UNF_MOD_TYPE_QAM_128:
                    printf("ModType:\t\tQAM_128\n");
                    break;
                case HI_UNF_MOD_TYPE_QAM_256:
                    printf("ModType:\t\tQAM_256\n");
                    break;
                default:
                    printf("ModType:\t\tUnknown\n");
                    break;
            }

            switch (stInfo.unSignalInfo.stTer.enFFTMode)
            {
                case HI_UNF_TUNER_FE_FFT_2K:
                    printf("FFTMode:\t\t2K\n");
                    break;
                case HI_UNF_TUNER_FE_FFT_4K:
                    printf("FFTMode:\t\t4K\n");
                    break;
                case HI_UNF_TUNER_FE_FFT_8K:
                    printf("FFTMode:\t\t8K\n");
                    break;
                case HI_UNF_TUNER_FE_FFT_16K:
                    printf("FFTMode:\t\t16K\n");
                    break;
                case HI_UNF_TUNER_FE_FFT_32K:
                    printf("FFTMode:\t\t32K\n");
                    break;
                case HI_UNF_TUNER_FE_FFT_64K:
                    printf("FFTMode:\t\t64K\n");
                    break;
                default:
                    printf("FFTMode:\t\tUnknown\n");
                    break;
            }
        }
    }
}

/* If your diseqc device need config polarization and 22K, you need registe the callback */
HI_VOID hi_tuner_diseqc_set(HI_U32 u32TunerId, HI_UNF_TUNER_FE_POLARIZATION_E enPolar,
                                                            HI_UNF_TUNER_FE_LNB_22K_E enLNB22K)
{
}

/* Standby test */
HI_VOID hi_tuner_standby(char * pPara)
{
    HI_U32 u32Para;
    HI_S32 s32Ret = HI_SUCCESS;

    if (pPara == HI_NULL_PTR)
    {
        return;
    }

    sscanf(pPara, "%d", (HI_S32*)&u32Para);
    HI_TUNER_INFO("hi_tuner_standby %d\n", u32Para);

    if (0 == u32Para)
    {
        s32Ret = HI_UNF_TUNER_WakeUp(s_s32TunerPort);
        if (HI_SUCCESS != s32Ret)
        {
            HI_TUNER_ERR("Tuner wake up failed.\n");
        }
    }
    else
    {
        s32Ret = HI_UNF_TUNER_Standby(s_s32TunerPort);
        if (HI_SUCCESS != s32Ret)
        {
            HI_TUNER_ERR("Tuner standby failed.\n");
        }
    }
}

/*play program*/
HI_VOID hi_tuner_play(char * avpid)
{
    HI_S32 s32Ret;
    HI_U32 u32Apid = 0;
    HI_U32 u32Vpid = 0;
    HI_UNF_AVPLAY_STOP_OPT_S    stStop;

    stStop.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    stStop.u32TimeoutMs = 0;

    if (avpid == HI_NULL_PTR)
    {
        return;
    }

    s32Ret = sscanf(avpid, "%x" TEST_ARGS_SPLIT "%x", &u32Vpid, &u32Apid);

    s32Ret= HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &stStop);
    if (s32Ret != HI_SUCCESS )
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_Stop failed.\n");
        return ;
    }

    s32Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &u32Apid);
    if (s32Ret != HI_SUCCESS)
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_SetAttr failed.\n");
        return ;
    }

    s32Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VID_PID, &u32Vpid);
    if (s32Ret != HI_SUCCESS)
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_SetAttr failed.\n");
        return ;
    }

    HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL);
    if (s32Ret != HI_SUCCESS)
    {
        HI_TUNER_WARN("call HI_UNF_AVPLAY_Start_AUD failed.\n");
        /*return ;*/
    }

    HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, NULL);
    if (s32Ret != HI_SUCCESS)
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_Start_VID failed.\n");
        return ;
    }

    HI_TUNER_INFO("Play u32Vpid %d u32Apid %d\n", u32Vpid, u32Apid);

    HI_TUNER_INFO("SUCCESS end\n");
    strcpy(s_acTestResult,HI_RESULT_SUCCESS);
}
HI_VOID hi_tuner_getoffset()
{
    HI_U32 u32Symb;
    HI_U32 u32Freq;
    /*HI_U32 au32BER[3] = {0};
    HI_U32 u32SNR = 0;
    HI_U32 u32SignalStrength = 0;*/
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = HI_UNF_TUNER_GetRealFreqSymb(s_s32TunerPort, &u32Freq, &u32Symb );
    if ( HI_SUCCESS != s32Ret )
    {
        HI_TUNER_ERR("HI_UNF_TUNER_GetOffset failed\n");
        return;
    }
    HI_TUNER_INFO("freq = %d, actul_symb = %d\n",u32Freq, u32Symb);

    /*s32Ret = HI_UNF_TUNER_GetBER(s_s32TunerPort , au32BER);
    if ( HI_SUCCESS != s32Ret )
    {
        printf("HI_UNF_TUNER_GetBER failed\n");
        return;
    }
    printf("BER :%d.%de-%d\n", au32BER[0], au32BER[1], au32BER[2]);

    s32Ret = HI_UNF_TUNER_GetSNR(s_s32TunerPort, &u32SNR);
    if ( HI_SUCCESS != s32Ret )
    {
        printf("HI_UNF_TUNER_GetSNR failed\n");
        return;
    }
    printf("SNR :%d\n", u32SNR);

    s32Ret = HI_UNF_TUNER_GetSignalStrength(s_s32TunerPort, &u32SignalStrength);
    if ( HI_SUCCESS != s32Ret )
    {
        printf("HI_UNF_TUNER_GetSignalStrength failed\n");
        return;
    }
    printf("SignalStrength :%d\n", u32SignalStrength);*/

    return;
}


HI_VOID hi_tuner_change_ac_to_b(char *type)
{
    //HI_S32 s32Type = 0;
    char mode[10] = "";
    HI_S32 s32Ret = HI_FAILURE;
    HI_UNF_TUNER_ATTR_S stTunerAttr;

    if(type == HI_NULL_PTR)
    {
        printf("pointer null\n");
        return;
    }
    sscanf(type, "%s ", mode);
    printf("sizeof(mode)=%zu, strlen(mode)=%zu, mode=%s\n", sizeof(mode), strlen(mode), mode);

    s32Ret = HI_UNF_TUNER_GetAttr(s_s32TunerPort, &stTunerAttr);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_UNF_TUNER_GetAttr fail\n");
        return;
    }
    //printf("type = %s \n", type);
    //printf("sizeof(type) = %d, strlen(type) = %d, %d\n", sizeof(type), strlen(type), strcmp(type, "j83b"));
    //if(0 == strcmp(type, "j83b"))
    //if(0 == strncmp(type, "j83b", (strlen(type) - 1)))
    if(0 == strcmp(mode, "j83b"))
    {
        printf("j83b\n");
        stTunerAttr.enDemodDevType = HI_UNF_DEMOD_DEV_TYPE_J83B;
        S_DEMOD_TYPE = HI_UNF_DEMOD_DEV_TYPE_J83B;
    }
    else
    {
        printf("j83ac\n");
        stTunerAttr.enDemodDevType = HI_UNF_DEMOD_DEV_TYPE_3130I;
        S_DEMOD_TYPE = HI_UNF_DEMOD_DEV_TYPE_3130I;
    }
    s32Ret = HI_UNF_TUNER_SetAttr(s_s32TunerPort, &stTunerAttr);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_UNF_TUNER_SetAttr fail\n");
        return;
    }

    return;
}

/*typedef struct  hiAGC_TEST_S
{
    HI_U32      u32Port;
    HI_U32      u32Agc1;
    HI_U32      u32Agc2;
    HI_BOOL     bLockFlag;
    HI_BOOL     bAgcLockFlag;
    HI_U8       u8BagcCtrl12;
    HI_U32      u32Count;
}AGC_TEST_S;

extern HI_S32 HI_UNF_TUNER_TEST_SINGLE_AGC(HI_U32   u32tunerId , AGC_TEST_S  *pstAgcTest);

HI_VOID hi_tuner_single_agc()
{
    HI_S32 s32Ret = HI_FAILURE;
    AGC_TEST_S stAgcTest = { 0 };


    s32Ret = HI_UNF_TUNER_TEST_SINGLE_AGC(s_s32TunerPort, &stAgcTest);
    if ( HI_SUCCESS != s32Ret )
    {
        printf("hi_tuner_single_agc failed\n");
        return;
    }

    printf("lock = %d, agc_lock = %d, agc2 = %d,      BAGC_CTRL_12 = %d, end\n",
        stAgcTest.bLockFlag, stAgcTest.bAgcLockFlag , stAgcTest.u32Agc2, stAgcTest.u8BagcCtrl12 );

    return;
}*/

/*help function*/
HI_VOID hi_showhelp(char * pCmd)
{
    HI_U32 u32Loop = 0;
    HI_U32 u32CmdNum = sizeof(g_cmdhelp) / sizeof(CMD_HELP_S);

    if (pCmd == HI_NULL_PTR)
    {
        HI_TUNER_INFO("command list:\n");
        for (u32Loop = 0; u32Loop < u32CmdNum; u32Loop++)
        {
            HI_TUNER_INFO("%s:%s\n", g_cmdhelp[u32Loop].name, g_cmdhelp[u32Loop].help_info);
        }
        return;
    }

    for (u32Loop = 0; u32Loop < u32CmdNum; u32Loop++)
    {
        if (0 == strncmp(pCmd, g_cmdhelp[u32Loop].name, strlen(g_cmdhelp[u32Loop].name)))
        {
            HI_TUNER_INFO("%s", g_cmdhelp[u32Loop].help_info);
        }
    }
}

/* set channel freqency/symbolrate/polar and call hi_tuner_connect */
HI_VOID hi_tuner_setchannel(char * channel)
{
    HI_U32  u32Freq,u32Symb,u32Arg = 0;
    HI_S32 s32Return;
    HI_UNF_AVPLAY_STOP_OPT_S    stStop;
    PMT_COMPACT_TBL   *ProgTbl = HI_NULL;

    stStop.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    stStop.u32TimeoutMs = 0;

    if (channel == HI_NULL_PTR)
    {
        return;
    }

    sscanf(channel, "%d" TEST_ARGS_SPLIT "%d" TEST_ARGS_SPLIT "%d",
               (HI_U32*)&u32Freq, (HI_U32*)&u32Symb, (HI_U32*)&u32Arg);


    if ((s_stConnectPara[s_s32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
        || (s_stConnectPara[s_s32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
    {
        s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32Freq = u32Freq * 1000;
        s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.u32SymbolRate = u32Symb;

        s_u32CurrentQamType = u32Arg;

        switch (u32Arg)
        {
            case 16:
            s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_16;
            break;
            case 32:
            s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_32;
            break;
            case 128:
            s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_128;
            break;
            case 256:
            s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_256;
            break;
            default:
            s_u32CurrentQamType = 64;
            s_stConnectPara[s_s32TunerPort].unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64;
        }
    }
    else if (s_stConnectPara[s_s32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {
        s_stConnectPara[s_s32TunerPort].unConnectPara.stSat.u32Freq = u32Freq * 1000;
        s_stConnectPara[s_s32TunerPort].unConnectPara.stSat.u32SymbolRate = u32Symb;
        s_stConnectPara[s_s32TunerPort].unConnectPara.stSat.enPolar = u32Arg;
    }
    
    s32Return = hi_tuner_connect();

    if(s32Return != HI_SUCCESS)
    {
        return;
    }

    s32Return= HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &stStop);
    if (s32Return != HI_SUCCESS )
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_Stop failed.\n");
        return ;
    }

    HIADP_Search_Init();
    s32Return = HIADP_Search_GetAllPmt(0,&ProgTbl);
    if (HI_SUCCESS != s32Return)
    {
        HI_TUNER_ERR("call HIADP_Search_GetAllPmt failed\n");
        printf("FAIL end\n");
    }
    else
    {
        printf("SUCCESS end\n");
    }

    s32Return = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL);
    if (s32Return != HI_SUCCESS)
    {
        HI_TUNER_WARN("call HI_UNF_AVPLAY_Start_AUD failed.\n");
        /*return ;*/
    }

    s32Return = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, NULL);
    if (s32Return != HI_SUCCESS)
    {
        HI_TUNER_ERR("call HI_UNF_AVPLAY_Start_VID failed.\n");
        return ;
    }
}

HI_VOID hi_tuner_selectCurrentPort(char * pPort)
{
    HI_S32 s32Ret;

    if (pPort == HI_NULL_PTR)
    {
        return;
    }

    if (HI_TUNER_NUMBER < 2)
    {
        HI_TUNER_ERR("HI_TUNER_NUMBER:%d,so this cmd is invalid.\n", HI_TUNER_NUMBER);
        return;
    }
    
    s_s32TunerPort = atoi(pPort);
    if(s_s32TunerPort > 1)
    {
        s_s32TunerPort %= 2;
    }

    s32Ret = HIADP_DMX_AttachTSPort(0, s_s32TunerPort);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("call HIADP_DMX_AttachTSPort failed 0x%x\n", s32Ret);
        HI_UNF_DMX_DeInit();
        return;
    }
    
    HI_TUNER_INFO("s_s32TunerPort:%d,switch to demuxPort:%d\n",
                             s_s32TunerPort,
                             (s_s32TunerPort==0)?HI_DEMUX_PORT:HI_DEMUX1_PORT);
    
}

static struct timeval s_stStart, s_stStop;
static HI_S32 s_s32TPNum = 0;
static HI_UNF_TUNER_SAT_TPINFO_S s_astTP[400];

HI_VOID hi_tuner_blindscan_notify(HI_U32 u32TunerId, HI_UNF_TUNER_BLINDSCAN_EVT_E enEVT, HI_UNF_TUNER_BLINDSCAN_NOTIFY_U* punNotify)
{
    HI_S32 i = 0;
    HI_U32 u32TimeUse;

    switch (enEVT)
    {
    case HI_UNF_TUNER_BLINDSCAN_EVT_STATUS:
        if (HI_UNF_TUNER_BLINDSCAN_STATUS_FAIL == *(punNotify->penStatus))
        {
            HI_TUNER_ERR("Scan fail.\n");
        }
        else if ((HI_UNF_TUNER_BLINDSCAN_STATUS_FINISH == *(punNotify->penStatus))
                    || (HI_UNF_TUNER_BLINDSCAN_STATUS_QUIT == *(punNotify->penStatus)))
        {

            gettimeofday(&s_stStop, NULL );
            u32TimeUse = 1000000 * (s_stStop.tv_sec - s_stStart.tv_sec ) + s_stStop.tv_usec - s_stStart.tv_usec;
            u32TimeUse /= 1000000;
            HI_TUNER_INFO("100");
            putchar('%');
            HI_TUNER_INFO(" done!\n");
            HI_TUNER_INFO("Scan over, find %d TP, use %ds.\n", s_s32TPNum, u32TimeUse);
            for (i=0; i<s_s32TPNum; i++)
            {
                HI_TUNER_INFO("%03d %d %d %d %d\n", i, s_astTP[i].u32Freq, s_astTP[i].u32SymbolRate, s_astTP[i].enPolar, \
                    s_astTP[i].cbs_reliablity/*, s_astTP[i].agc_h8*/);
            }
        }
        break;
    case HI_UNF_TUNER_BLINDSCAN_EVT_PROGRESS:
        HI_TUNER_INFO("%d",*(punNotify->pu16ProgressPercent));
        putchar('%');
        HI_TUNER_INFO(" done!\n");
        break;
    case HI_UNF_TUNER_BLINDSCAN_EVT_NEWRESULT:
        if(s_s32TPNum < sizeof(s_astTP) / sizeof(HI_UNF_TUNER_SAT_TPINFO_S))
        {
            s_astTP[s_s32TPNum] = *(punNotify->pstResult);
            s_s32TPNum++;
        }
        else
        {
            HI_TUNER_WARN("Too many channels!\n");
        }
        break;
    default:
        break;
    }
}


/* Blind scan */
HI_VOID hi_tuner_blindscan(char * pPara)
{
    HI_S32 s32Ret = 0;
    HI_S32 s32BlindScanType;
    HI_U32 u32StartFreq;
    HI_U32 u32StopFreq;
    HI_UNF_TUNER_FE_POLARIZATION_E enPolar;
    HI_UNF_TUNER_FE_LNB_22K_E enLNB22K;
    HI_UNF_TUNER_BLINDSCAN_PARA_S stBlindScanPara;

    if (pPara == HI_NULL_PTR)
    {
        return;
    }

    sscanf(pPara, "%d", (HI_S32*)&s32BlindScanType);
    if (1 == s32BlindScanType)
    {
        sscanf(pPara, "%d" TEST_ARGS_SPLIT "%d" TEST_ARGS_SPLIT "%d" TEST_ARGS_SPLIT "%d" TEST_ARGS_SPLIT "%d",
               (HI_S32*)&s32BlindScanType, (HI_S32*)&enPolar, (HI_S32*)&enLNB22K, &u32StartFreq, &u32StopFreq);
        HI_TUNER_INFO("hi_tuner_blindscan Type:%d, Polar %d, 22K %d,  %dkHz - %dkHz\n",
               s32BlindScanType, enPolar, enLNB22K, u32StartFreq, u32StopFreq);
    }

    s_s32TPNum = 0;
    gettimeofday(&s_stStart, NULL );
    
    if (s_stConnectPara[s_s32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {
        if (0 == s32BlindScanType)
        {
            /* Auto */
            stBlindScanPara.enMode = HI_UNF_TUNER_BLINDSCAN_MODE_AUTO;
            /* If your diseqc device need config polarization and 22K, you need registe the callback */
            stBlindScanPara.unScanPara.stSat.pfnDISEQCSet = hi_tuner_diseqc_set;
            stBlindScanPara.unScanPara.stSat.pfnEVTNotify = hi_tuner_blindscan_notify;
        }
        else
        {
            stBlindScanPara.enMode = HI_UNF_TUNER_BLINDSCAN_MODE_MANUAL;
            stBlindScanPara.unScanPara.stSat.enPolar  = enPolar;
            stBlindScanPara.unScanPara.stSat.enLNB22K = enLNB22K;
            stBlindScanPara.unScanPara.stSat.u32StartFreq = u32StartFreq;
            stBlindScanPara.unScanPara.stSat.u32StopFreq = u32StopFreq;
            stBlindScanPara.unScanPara.stSat.pfnDISEQCSet = HI_NULL;
            stBlindScanPara.unScanPara.stSat.pfnEVTNotify = hi_tuner_blindscan_notify;
        }
    }

    s32Ret = HI_UNF_TUNER_BlindScanStart(s_s32TunerPort, &stBlindScanPara);
    if (s32Ret != HI_SUCCESS)
    {
        HI_TUNER_ERR("call HI_UNF_TUNER_BlindScanStart failed.\n");
        return;
    }
}


/* set of received command */
TEST_FUNC_S g_testfunc[] =
{
    {"setport", hi_tuner_selectCurrentPort},              /*ø…”√”⁄≈‰÷√∂Àø⁄∫≈*/
    {"setchnl", hi_tuner_setchannel},                 /*À¯∆µ√¸¡Ó*/
    {"setfreq",    hi_tuner_setfreq },
    {"setsymb",   hi_tuner_setsymb},
    {"setqam",    hi_tuner_setqam },
    {"getsignalinfo", hi_tuner_getsignalinfo  },
    {"play",      hi_tuner_play },
    {"getmsc",    hi_tuner_getmsc},
    //{"getber",        hi_tuner_getmsc_ber     },
    {"settype",   hi_tuner_settype},
    {"getoffset", hi_tuner_getoffset},
    {"start",     hi_tuner_start},
    {"setmode", hi_tuner_change_ac_to_b},
    {"standby",       hi_tuner_standby        },

    {"help",      hi_showhelp}
};

/*search the handle function corresponding with the command in the character string */
Test_Func_Proc getFunbyName(char *name)
{
    HI_U32 u32Loop;
    HI_U32 u32FunNum = sizeof(g_testfunc) / sizeof(TEST_FUNC_S);

    if (HI_NULL_PTR == name)
    {
        return HI_NULL_PTR;
    }

    for (u32Loop = 0; u32Loop < u32FunNum; u32Loop++)
    {
        if (0 == strncmp(name, g_testfunc[u32Loop].name, strlen(g_testfunc[u32Loop].name)))
        {
            return g_testfunc[u32Loop].proc;
            HI_TUNER_WARN("here line:%d\n", __LINE__);
        }
    }

    return HI_NULL_PTR;
}

/*deals with the universal command */
HI_S32 procfun(char *funargs)
{
    char *argstr;
    Test_Func_Proc func;

    argstr = strchr(funargs, UDP_STR_SPLIT);
    if (HI_NULL_PTR != argstr)
    {
        *argstr = 0;
        argstr += 1;
    }

    func = getFunbyName(funargs);
    if (HI_NULL_PTR != func)
    {
        func(argstr);
        return HI_SUCCESS;
    }
    else
    {
        strcpy(s_acTestResult,HI_RESULT_FAIL" Can't find the function\n");
        HI_TUNER_ERR("Can't find the function  %s %s\n\n", funargs, argstr);
        return HI_FAILURE;
    }
}

int TestTCPOpen()
{
    HI_S32 s32SockFd = -1;
    struct sockaddr_in  stAddr;
    s32SockFd = socket(AF_INET,SOCK_STREAM,0);
    if(s32SockFd < 0)
    {
        HI_TUNER_ERR("Socket error...\n");
        return -1;
    }

    memset(&stAddr,0,sizeof(stAddr));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = INADDR_ANY;
    stAddr.sin_port = htons(DEFAULT_PORT);
    if(bind(s32SockFd,(struct sockaddr *)&stAddr,sizeof(struct sockaddr))<0)
    {
        close(s32SockFd);
        HI_TUNER_ERR("Bind error...\n");
        return -1;
    }

    listen(s32SockFd, 5);

    return s32SockFd;
}

void * tcprcv(void *arg)
{
    struct sockaddr_in stCliAddr;
    HI_S32 s32NewFd;
    HI_S32 s32Size;
    HI_S32 s32RcvLength;
    HI_CHAR acRecvBuf[MAX_CMD_BUFFER_LEN];
    HI_S32 s32Socketd = TestTCPOpen();
    HI_S32 s32Ret;
    if(s32Socketd < 0)
    {
        return (void *)0;
    }
    s32Size = sizeof(stCliAddr);
    if ( (s32NewFd = accept(s32Socketd, (struct sockaddr *) &stCliAddr, (socklen_t *)&s32Size)) < 0)
    {
        HI_TUNER_ERR("accept err\n");
        close(s32Socketd);
        return (void *)1;
    }
    HI_TUNER_INFO("accept socket %d\n",s32NewFd);

    while( 1)
    {
        s32RcvLength = read(s32NewFd, acRecvBuf, sizeof(acRecvBuf));
        if(s32RcvLength > 0)
        {
            acRecvBuf[s32RcvLength] = 0;
            HI_TUNER_INFO("receive cmd: %s\n",acRecvBuf);
            s32Ret = procfun(acRecvBuf);

            s_acTestResult[strlen(s_acTestResult)]= '\n';

            s32Ret = send(s32NewFd,  s_acTestResult, strlen(s_acTestResult), 0);
            if(s32Ret < 0)
            {
                HI_TUNER_WARN("send err:%s\n",strerror(errno));
            }
            HI_TUNER_INFO("%s result:%s\n",acRecvBuf,s_acTestResult);

            memset(s_acTestResult,0, sizeof(s_acTestResult));
            /*fflush(newfd);*/
        }
    }
}

HI_S32 main(HI_S32 argc, char *argv[])
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_CHAR acRecvBuf[MAX_CMD_BUFFER_LEN];
    HI_S32 s32Threadd;

    printf("\nCurrent signal:[tuner0:%s,tuner1:%s].\n\n", 
            asignalTable[(HI_U32)log2(HI_TUNER_SIGNAL_TYPE)], asignalTable[(HI_U32)log2(HI_TUNER1_SIGNAL_TYPE)]);

    /* init device */
    s32Ret = dev_init();
    if(HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    /*there are in Ret = HIADP_Snd_Init()*/
    /*AudioLineOutMuteCntrDisable();
    AudioSPDIFOutSharedEnable();*/

    /* init avplay */
    s32Ret = avplay_Init();
    if(HI_SUCCESS != s32Ret)
    {
        HI_TUNER_ERR("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    pthread_create((pthread_t  *)&s32Threadd, 0, tcprcv, 0);

    /*print help*/
    hi_showhelp(HI_NULL);

    /* recieve command */
    while (1)
    {
        SAMPLE_GET_INPUTCMD(acRecvBuf);
        if (strlen(acRecvBuf) < 3)
        {
            usleep(10000);
            continue;
        }

        if (strncmp(acRecvBuf, "exit", 4) == 0)
        {
            break;
        }

        procfun(acRecvBuf);
    }

    /* deinit device */
    avplay_deinit();
    dev_deinit();

    return HI_SUCCESS;
}

