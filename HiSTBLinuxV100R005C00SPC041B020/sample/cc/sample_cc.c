#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "hi_type.h"
#include "hi_debug.h"

#include "hi_unf_disp.h"
#include "hi_unf_common.h"
#include "hi_unf_demux.h"
#include "hi_unf_ecs.h"
#include "hi_unf_avplay.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "hi_unf_sound.h"

#include "hi_unf_cc.h"
#include "sample_cc_out.h"
#include "sample_cc_data.h"
#include "sample_cc_xds.h"

#include "hi_adp.h"

#define HI_FATAL_CC(fmt...)      HI_FATAL_PRINT(HI_ID_CC, fmt)
#define HI_ERR_CC(fmt...)        HI_ERR_PRINT(HI_ID_CC, fmt)
#define HI_WARN_CC(fmt...)       HI_WARN_PRINT(HI_ID_CC, fmt)
#define HI_INFO_CC(fmt...)       HI_INFO_PRINT(HI_ID_CC, fmt)

#define TRACE_INFO printf

#define DMXID (0)

typedef struct tagCC_Tuner_Param_S
{
    HI_U32 u32TunerFreq;
    HI_U32 u32TunerSymbolRate;
    HI_U32 u32TunerQam;
}CC_Tuner_Param_S;

typedef enum tagCC_Data_SOURCE_type_E
{
    DATA_SOURCE_FILE = 0,
    DATA_SOURCE_TUNER
}CC_Data_SOURCE_type_E;

typedef struct tagCC_Data_Source_Param_S
{
    CC_Data_SOURCE_type_E type;
    HI_U8 au8File[256];
    CC_Tuner_Param_S stTunerParam;
}CC_Data_Source_Param_S;

typedef struct tagCmdLinePara
{
    HI_U32 u32TunerFreq;
    HI_U32 u32TunerSymbolRate;
    HI_U32 u32TunerQam;
    HI_U8* pszFileName;
}CMD_LINE_ST;


HI_BOOL g_bTaskQuit = HI_FALSE;
HI_BOOL g_bUsrThreadFlag = HI_FALSE;
HI_BOOL g_bStartFlag = HI_FALSE;

HI_HANDLE hAvplay = 0;
HI_HANDLE hWindow = 0;
HI_HANDLE hTsBuffer = 0;
HI_HANDLE hcc = 0;
HI_HANDLE hOutput = 0;
HI_UNF_CC_DATA_TYPE_E g_enCCDataType;

#define SYN_USING_LOCALTIME 0
static HI_S32 GetCurPts(HI_VOID* pUserData, HI_S64 *ps64CurrentPts)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_AVPLAY_STATUS_INFO_S stStatusInfo = {{0}};
    static HI_U32 u32PrePtsTime = 0;

    s32Ret = HI_UNF_AVPLAY_GetStatusInfo(hAvplay, &stStatusInfo);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to HI_UNF_AVPLAY_GetStatusInfo\n");
    }

#if SYN_USING_LOCALTIME
    *ps64CurrentPts = stStatusInfo.stSyncStatus.u32LocalTime;
#else
    *ps64CurrentPts = stStatusInfo.stSyncStatus.u32LastVidPts;
#endif

    if (*ps64CurrentPts < u32PrePtsTime)
    {
        TRACE_INFO("Get PTS:%llu, pre time is %u\n", *ps64CurrentPts, u32PrePtsTime);
    }

    u32PrePtsTime = *ps64CurrentPts;

    return HI_SUCCESS;
}

/* flag = 1 : switch text color
   flag = 0 : switch text bg color*/
static void SwitchColor(HI_U8 u8flag)
{
    HI_UNF_CC_ATTR_S stCCAttr = {0};
    HI_S8 inputCmd[32];
    HI_S32 type = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    HI_UNF_CC_COLOR_E ColorMap[] =
    {
        HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_COLOR_WHITE,HI_UNF_CC_COLOR_RED,
        HI_UNF_CC_COLOR_GREEN,HI_UNF_CC_COLOR_BLUE,HI_UNF_CC_COLOR_YELLOW,
        HI_UNF_CC_COLOR_MAGENTA,HI_UNF_CC_COLOR_CYAN,HI_UNF_CC_COLOR_DEFAULT
    };

    if(HI_SUCCESS == HI_UNF_CC_GetAttr(hcc,&stCCAttr))
    {
        TRACE_INFO("0 ------ black\n");
        TRACE_INFO("1 ------ white\n");
        TRACE_INFO("2 ------ red\n");
        TRACE_INFO("3 ------ green\n");
        TRACE_INFO("4 ------ blue\n");
        TRACE_INFO("5 ------ yellow\n");
        TRACE_INFO("6 ------ megenta\n");
        TRACE_INFO("7 ------ cyan\n");
        TRACE_INFO("8 ------ default\n");

        SAMPLE_GET_INPUTCMD(inputCmd);
        type = atoi((char*)inputCmd);

        if(type > 8)
        {
            TRACE_INFO("Input error!\n");
            return;
        }

        if(1 == u8flag)
        {
            if(HI_UNF_CC_DATA_TYPE_608 == stCCAttr.enCCDataType)
            {
                stCCAttr.unCCConfig.stCC608ConfigParam.u32CC608TextColor = ColorMap[type];
            }
            else if(HI_UNF_CC_DATA_TYPE_708 == stCCAttr.enCCDataType)
            {
                stCCAttr.unCCConfig.stCC708ConfigParam.u32CC708TextColor = ColorMap[type];
            }

        }
        else
        {
            if(HI_UNF_CC_DATA_TYPE_608 == stCCAttr.enCCDataType)
            {
                stCCAttr.unCCConfig.stCC608ConfigParam.u32CC608BgColor = ColorMap[type];
            }
            else if(HI_UNF_CC_DATA_TYPE_708 == stCCAttr.enCCDataType)
            {
                stCCAttr.unCCConfig.stCC708ConfigParam.u32CC708BgColor = ColorMap[type];
            }
        }

        s32Ret = HI_UNF_CC_SetAttr(hcc,&stCCAttr);
        if (HI_SUCCESS != s32Ret)
        {
            TRACE_INFO("failed to HI_UNF_CC_SetAttr\n");
        }
    }

}

static void SwitchOpacity(void)
{
    HI_UNF_CC_ATTR_S stCCAttr = {0};
    HI_S8 inputCmd[32];
    HI_S32 type = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    HI_UNF_CC_FONTSTYLE_E OpacityMap[] =
    {
        HI_UNF_CC_OPACITY_TRANSPARENT,HI_UNF_CC_OPACITY_SOLID,HI_UNF_CC_OPACITY_DEFAULT
    };

    if(HI_SUCCESS == HI_UNF_CC_GetAttr(hcc,&stCCAttr))
    {
        TRACE_INFO("0 ------ transparent\n");
        TRACE_INFO("1 ------ solid\n");
        TRACE_INFO("2 ------ default\n");

        SAMPLE_GET_INPUTCMD(inputCmd);
        type = atoi((char*)inputCmd);

        if(type > 2)
        {
            TRACE_INFO("Input error!\n");
            return;
        }

        if(HI_UNF_CC_DATA_TYPE_608 == stCCAttr.enCCDataType)
        {
            stCCAttr.unCCConfig.stCC608ConfigParam.enCC608BgOpac = OpacityMap[type];
        }
        else if(HI_UNF_CC_DATA_TYPE_708 == stCCAttr.enCCDataType)
        {
            stCCAttr.unCCConfig.stCC708ConfigParam.enCC708BgOpac = OpacityMap[type];
        }

        s32Ret = HI_UNF_CC_SetAttr(hcc,&stCCAttr);
        if (HI_SUCCESS != s32Ret)
        {
            printf("failed to HI_UNF_CC_SetAttr\n");
        }
    }

}

static void SwitchCCChanel(void)
{
    HI_UNF_CC_ATTR_S stCCAttr = {0};
    HI_S8 inputCmd[32];
    HI_S32 type = 0;
    HI_S32 s32Ret = HI_SUCCESS;


    HI_UNF_CC_608_DATATYPE_E NtscChannelMap[] =
    {
        HI_UNF_CC_608_DATATYPE_CC1,HI_UNF_CC_608_DATATYPE_CC2,HI_UNF_CC_608_DATATYPE_CC3,
        HI_UNF_CC_608_DATATYPE_CC4,HI_UNF_CC_608_DATATYPE_TEXT1,HI_UNF_CC_608_DATATYPE_TEXT2,
        HI_UNF_CC_608_DATATYPE_TEXT3,HI_UNF_CC_608_DATATYPE_TEXT4
    };

    HI_UNF_CC_708_SERVICE_NUM_E DtvServiceMap[] =
    {
        HI_UNF_CC_708_SERVICE1,HI_UNF_CC_708_SERVICE2,HI_UNF_CC_708_SERVICE3,
        HI_UNF_CC_708_SERVICE4,HI_UNF_CC_708_SERVICE5,HI_UNF_CC_708_SERVICE6
    };

    if(HI_SUCCESS == HI_UNF_CC_GetAttr(hcc,&stCCAttr))
    {
        switch(stCCAttr.enCCDataType)
        {
            case HI_UNF_CC_DATA_TYPE_608:
                TRACE_INFO("1 ------ CC1\n");
                TRACE_INFO("2 ------ CC2\n");
                TRACE_INFO("3 ------ CC3\n");
                TRACE_INFO("4 ------ CC4\n");
                TRACE_INFO("5 ------ TEXT1\n");
                TRACE_INFO("6 ------ TEXT2\n");
                TRACE_INFO("7 ------ TEXT3\n");
                TRACE_INFO("8 ------ TEXT4\n");
                TRACE_INFO("input channel number :\n");

                SAMPLE_GET_INPUTCMD(inputCmd);
                type = atoi((char*)inputCmd);
                if(type > 8 || type <= 0)
                {
                    TRACE_INFO("Input error!\n");
                    return;
                }

                stCCAttr.unCCConfig.stCC608ConfigParam.enCC608DataType = NtscChannelMap[type - 1];
                break;
            case HI_UNF_CC_DATA_TYPE_708:
                TRACE_INFO("1 ------ CS1\n");
                TRACE_INFO("2 ------ CS2\n");
                TRACE_INFO("3 ------ CS3\n");
                TRACE_INFO("4 ------ CS4\n");
                TRACE_INFO("5 ------ CS5\n");
                TRACE_INFO("6 ------ CS6\n");
                TRACE_INFO("input service number :\n");

                SAMPLE_GET_INPUTCMD(inputCmd);
                type = atoi((char*)inputCmd);
                if(type > 6 || type <= 0)
                {
                    TRACE_INFO("Input error!\n");
                    return;
                }
                stCCAttr.unCCConfig.stCC708ConfigParam.enCC708ServiceNum = DtvServiceMap[type - 1];
                break;
            default:
                break;
        }

        s32Ret = HI_UNF_CC_SetAttr(hcc,&stCCAttr);
        if (HI_SUCCESS != s32Ret)
        {
            printf("failed to HI_UNF_CC_SetAttr\n");
        }
    }

}

static void SwitchCaptionOut(HI_HANDLE hCC)
{
    if(HI_FALSE == g_bStartFlag)
    {
        HI_UNF_CC_Start(hCC);
        g_bStartFlag = HI_TRUE;
        TRACE_INFO("cc started\n");
    }
    else
    {
        HI_UNF_CC_Stop(hCC);
        g_bStartFlag = HI_FALSE;
        TRACE_INFO("cc stoped\n");
    }
}

static HI_S32 FilterDataCallback(HI_U32 u32UserData, HI_U8 *pu8Data, HI_U32 u32DataLength)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ARIBCCPID = u32UserData;

    if (hcc)
    {
        if(u32ARIBCCPID)
        {
            //printf("<<<<<<<<<<<< ARIBCCPID = %#x,DataLength = %d\n",u32ARIBCCPID,u32DataLength);
            s32Ret = HI_UNF_CC_InjectPESData(hcc, pu8Data, u32DataLength);
            if (s32Ret != HI_SUCCESS)
            {
                TRACE_INFO("failed to HI_UNF_SUBT_InjectData, u32ARIBCCPID=0x%x\n",u32ARIBCCPID);
            }
        }
    }
    return s32Ret;
}

static void *UsrDataInject(void* arg)
{
    HI_UNF_VIDEO_USERDATA_S stUsrData;
    HI_UNF_VIDEO_USERDATA_TYPE_E enType;
    HI_UNF_CC_USERDATA_S stUserData;

    HI_S32 s32Ret = HI_SUCCESS;
    static  HI_U32 time = 0;

    while(g_bTaskQuit == HI_FALSE)
    {
        while(g_bUsrThreadFlag)
        {
            usleep(100*1000);
        }
        if(hAvplay)
        {
            s32Ret = HI_UNF_AVPLAY_AcqUserData(hAvplay, &stUsrData, &enType);
            if (HI_SUCCESS == s32Ret)
            {
                time = 0;
                if(HI_UNF_VIDEO_USERDATA_DVB1_CC == enType)
                {
                    if(hcc)
                    {
                        stUserData.pu8userdata = stUsrData.pu8Buffer;
                        stUserData.u32dataLen = stUsrData.u32Length;
                        s32Ret = HI_UNF_CC_InjectUserData(hcc, &stUserData);
                        if (s32Ret != HI_SUCCESS)
                        {
                            HI_ERR_CC("failed to HI_UNF_CC_InjectUserData, u32Length=0x%x\n",stUsrData.u32Length);
                        }
                    }
                }

                s32Ret = HI_UNF_AVPLAY_RlsUserData(hAvplay, &stUsrData);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_CC("HI_UNF_AVPLAY_RlsUserData return %#x\n", s32Ret);
                }
            }
            else
            {
                time ++;
                if(time > 5000)
                {
                    g_bUsrThreadFlag = HI_TRUE;
                    HI_ERR_CC("HI_UNF_AVPLAY_AcqUserData failed\n");
                    time = 0;
                }
                usleep(50*1000);
            }
        }
    }
    return (void *)0;
}
static HI_U8 SelectProgram(PMT_COMPACT_TBL *pProgTbl)
{
    HI_U8 u8ProgNo = 0; /* for prog No. */

    TRACE_INFO("program number is %d\n", pProgTbl->prog_num);

    /* for multi-program, need to select someone, range from 0 to total number -1 */
    if (pProgTbl->prog_num > 1)
    {
        HI_U8  au8ProgNo[32]; /* for program number */

        TRACE_INFO("Please input a number from 0 to %d for program:\n", pProgTbl->prog_num-1);
        SAMPLE_GET_INPUTCMD(au8ProgNo);

        u8ProgNo = atoi((char*)au8ProgNo);
        if (u8ProgNo >= pProgTbl->prog_num)
        {
            TRACE_INFO("\nprogram %d is not exist!! usd default program 0\n", u8ProgNo);
            u8ProgNo = 0;
        }
    }
    else
    {
        TRACE_INFO("\nusd default program 0\n");
        u8ProgNo = 0;
    }

    return u8ProgNo;
}
HI_HANDLE hTrack = 0;
static HI_S32 StartAVPlay(PMT_COMPACT_TBL *pProgTbl, HI_U8 u8ProgNo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_AVPLAY_ATTR_S stAvplayAttr;
    HI_UNF_AUDIOTRACK_ATTR_S    stTrackAttr;
    //HI_HANDLE hTrack = 0;

    if (0 == hAvplay)
    {
        s32Ret = HIADP_AVPlay_RegADecLib();
        s32Ret |= HI_UNF_AVPLAY_Init();
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_CC("failed to HI_UNF_AVPLAY_Init\n");
            goto ERROR;
        }

        memset(&stAvplayAttr, 0 ,sizeof(stAvplayAttr));
        s32Ret = HI_UNF_AVPLAY_GetDefaultConfig(&stAvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
        s32Ret |= HI_UNF_AVPLAY_Create(&stAvplayAttr, &hAvplay);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_CC("failed to HI_UNF_AVPLAY_Create\n");
            goto ERROR;
        }

        s32Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
        s32Ret |= HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_CC("failed to HI_UNF_AVPLAY_ChnOpen\n");
            goto ERROR;
        }

        s32Ret = HIADP_Snd_Init();
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_CC("failed to HIADP_Snd_Init\n");
            goto ERROR;
        }

        s32Ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_60);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_CC("failed to HIADP_Disp_Init\n");
            goto ERROR;
        }

        s32Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
        s32Ret |= HIADP_VO_CreatWin(HI_NULL,&hWindow);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_CC("failed to HIADP_VO_CreatWin\n");
            goto ERROR;
        }

        s32Ret = HI_UNF_VO_AttachWindow(hWindow, hAvplay);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_CC("failed to HI_UNF_VO_AttachWindow\n");
            goto ERROR;
        }

        s32Ret = HI_UNF_VO_SetWindowEnable(hWindow, HI_TRUE);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_CC("failed to HI_UNF_VO_SetWindowEnable\n");
            goto ERROR;
        }

        s32Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_CC("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
            goto ERROR;
        }
        s32Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_CC("failed to HI_UNF_SND_CreateTrack\n");
            goto ERROR;
        }

        s32Ret = HI_UNF_SND_Attach(hTrack, hAvplay);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_CC("failed to HI_UNF_SND_Attach\n");
            goto ERROR;
        }

        HI_UNF_SYNC_ATTR_S SyncAttr;
        s32Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
        SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
        SyncAttr.stSyncStartRegion.s32VidPlusTime = 60;
        SyncAttr.stSyncStartRegion.s32VidNegativeTime = -20;
        SyncAttr.bQuickOutput = HI_FALSE;
        s32Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_CC("failed to HI_UNF_AVPLAY_SetAttr\n");
            goto ERROR;
        }
    }

    s32Ret = HIADP_AVPlay_PlayProg(hAvplay,pProgTbl,u8ProgNo,HI_TRUE);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to HIADP_AVPlay_PlayProg\n");
        goto ERROR;
    }

    return HI_SUCCESS;
ERROR:

    if (hAvplay)
    {
        (HI_VOID)HI_UNF_AVPLAY_Destroy(hAvplay);

        (HI_VOID)HI_UNF_AVPLAY_DeInit();

        hAvplay = 0;
    }

    if (hWindow)
    {
        (HI_VOID)HI_UNF_VO_DestroyWindow(hWindow);
        (HI_VOID)HIADP_VO_DeInit();
        hWindow = 0;
    }

    return HI_FAILURE;

}

static HI_S32 StartCC(PMT_COMPACT_TBL *pProgTbl, HI_U8 u8ProgNo)
{
    HI_UNF_CC_ATTR_S  stCCAttr;
    HI_UNF_CC_PARAM_S stCCParam;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bHaveCC708 = HI_FALSE;
    HI_U8 i = 0;
    HI_U8 au8CCNo[32];

    memset(&stCCAttr, 0, sizeof(HI_UNF_CC_ATTR_S));
    memset(&stCCParam, 0, sizeof(HI_UNF_CC_PARAM_S));

    CC608_XDS_Init();

    if(0 == hOutput)
    {
        if (HI_FAILURE == CC_Output_Init(&hOutput))
        {
            HI_ERR_CC("CC_Output_Init failed!\n");
            return HI_FAILURE;
        }
    }

    for (i = 0; i < pProgTbl->proginfo[u8ProgNo].u16ClosedCaptionNum; i++)
    {
        if (pProgTbl->proginfo[u8ProgNo].stClosedCaption[i].u8IsDigitalCC
            && pProgTbl->proginfo[u8ProgNo].stClosedCaption[i].u8ServiceNumber)
        {
            bHaveCC708 = HI_TRUE;
            break;
        }
    }

    TRACE_INFO("\n\t 0 - CC 608\n");
    if (bHaveCC708)
    {
        TRACE_INFO("\t 1 - CC 708\n");
    }
    if (pProgTbl->proginfo[u8ProgNo].u16ARIBCCPid)
    {
        TRACE_INFO("\t 2 - ARIB CC\n");
    }

    TRACE_INFO("Please select a closed caption :\n");
    SAMPLE_GET_INPUTCMD(au8CCNo);
    if (1 == atoi((char*)au8CCNo))
    {
        TRACE_INFO("\nUsed CC 708\n");
        stCCAttr.enCCDataType = HI_UNF_CC_DATA_TYPE_708;
        HI_UNF_CC_GetDefaultAttr(&stCCAttr);
        stCCAttr.unCCConfig.stCC708ConfigParam.enCC708DispFormat = HI_UNF_CC_DF_1280X720;
    }
    else if (2 == atoi((char*)au8CCNo))
    {
        TRACE_INFO("\nUsed ARIB CC\n");
        stCCAttr.enCCDataType = HI_UNF_CC_DATA_TYPE_ARIB;
        HI_UNF_CC_GetDefaultAttr(&stCCAttr);
    }
    else
    {
        TRACE_INFO("\nUsed CC 608\n");
        stCCAttr.enCCDataType = HI_UNF_CC_DATA_TYPE_608;
        HI_UNF_CC_GetDefaultAttr(&stCCAttr);
        stCCAttr.unCCConfig.stCC608ConfigParam.enCC608DispFormat = HI_UNF_CC_DF_1280X720;
    }

    stCCParam.pfnCCDisplay = CC_Output_OnDraw;
    stCCParam.stCCAttr = stCCAttr;
    stCCParam.pfnCCGetTextSize = CC_Output_GetTextSize;
    stCCParam.pfnBlit = CC_Output_Blit;
    stCCParam.pfnVBIOutput = CC_Output_VBIOutput;
    stCCParam.pfnCCGetPts = GetCurPts;
    stCCParam.pfnXDSOutput = CC608_XDS_Decode;

    HI_UNF_CC_Init();
    s32Ret = HI_UNF_CC_Create(&stCCParam,&hcc);
    if(HI_FAILURE == s32Ret)
    {
        HI_UNF_CC_DeInit();
        return s32Ret;
    }
    s32Ret |= HI_UNF_CC_Start(hcc);
    g_bStartFlag = HI_TRUE;

    g_enCCDataType = stCCAttr.enCCDataType;
    return s32Ret;

}

static HI_S32 StartDataFilter(PMT_COMPACT_TBL *pSubTitleProgTbl, HI_U8 u8ProgNo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    PMT_COMPACT_PROG *pstProginfo = &pSubTitleProgTbl->proginfo[u8ProgNo];
    CC_DATA_INSTALL_PARAM_S stInstallParam;

    s32Ret = CC_Data_DeInit();
    s32Ret |= CC_Data_Init();
    if (HI_SUCCESS != s32Ret)
    {
        TRACE_INFO("failed to CC_Data_Init\n");

        return HI_FAILURE;
    }

    stInstallParam.u32DmxID = 0;
    stInstallParam.pfnCallback = FilterDataCallback;

    stInstallParam.u16CCPID = pstProginfo->u16ARIBCCPid;
    stInstallParam.u32UserData = (HI_U32)pstProginfo->u16ARIBCCPid;
    s32Ret |= CC_Data_Install(&stInstallParam);
    if (HI_SUCCESS != s32Ret)
    {
        TRACE_INFO("failed to CC_Data_Install\n");

        (HI_VOID)CC_Data_DeInit();

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 StartDmx(CC_Data_Source_Param_S *pstParam)
{
    HI_S32  s32Ret = HI_SUCCESS;

    if (DATA_SOURCE_TUNER == pstParam->type)
    {
    #ifdef HI_FRONTEND_SUPPORT
        s32Ret = HIADP_Tuner_Init();
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_CC("failed to HIADP_Tuner_Init\n");

            return HI_FAILURE;
        }

        s32Ret = HIADP_Tuner_Connect(TUNER_USE,pstParam->stTunerParam.u32TunerFreq,
                  pstParam->stTunerParam.u32TunerSymbolRate, pstParam->stTunerParam.u32TunerQam);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_CC("failed to HIADP_Tuner_Connect\n");

            return HI_FAILURE;
        }

        s32Ret = HI_UNF_DMX_Init();
        s32Ret |= HI_UNF_DMX_AttachTSPort(DMXID,DEFAULT_DVB_PORT);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_CC( "failed to HI_UNF_DMX_AttachTSPort\n");
            (HI_VOID)HI_UNF_DMX_DeInit();

            return HI_FAILURE;
        }
    #else
        HI_ERR_CC("Tuner is not support\n");
    #endif
    }

    //else if (DATA_SOURCE_FILE == pstParam->type)
	if (DATA_SOURCE_FILE == pstParam->type)
    {
        s32Ret = HI_UNF_DMX_Init();
        s32Ret |= HI_UNF_DMX_AttachTSPort(DMXID, HI_UNF_DMX_PORT_RAM_0);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_CC("failed to HI_UNF_DMX_AttachTSPort\n");
            (HI_VOID)HI_UNF_DMX_DeInit();

            return HI_FAILURE;
        }

        s32Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_0, 0x200000, &hTsBuffer);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_CC("failed to HI_UNF_DMX_CreateTSBuffer\n");
            (HI_VOID)HI_UNF_DMX_DetachTSPort(DMXID);
            (HI_VOID)HI_UNF_DMX_DeInit();
            return HI_FAILURE;
        }
    }
    else
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


static HI_S32 InjectTsTask(HI_VOID *args)
{
    HI_S32  s32Ret = HI_SUCCESS;
    HI_U32  Readlen = 0;
    HI_UNF_STREAM_BUF_S   StreamBuf;
    FILE *pTsFile = NULL;

    CC_Data_Source_Param_S *pstParam = (CC_Data_Source_Param_S *)(args);

    if (DATA_SOURCE_FILE != pstParam->type)
    {
        HI_ERR_CC("data source not support!\n");
        return HI_FAILURE;
    }


    pTsFile = fopen((char*)pstParam->au8File, "rb");

    if(pTsFile == NULL)
    {
        HI_ERR_CC( "\nfile %s open error!!\n", pstParam->au8File);

        return HI_FAILURE;
    }

    /* loop in inject data */
    while(g_bTaskQuit == HI_FALSE)
    {
        s32Ret = HI_UNF_DMX_GetTSBuffer(hTsBuffer, 188*200, &StreamBuf, 1000);
        if (s32Ret != HI_SUCCESS )
        {
            continue;
        }

        Readlen = fread(StreamBuf.pu8Data, sizeof(HI_S8), StreamBuf.u32Size, pTsFile);
        if(Readlen <= 0)
        {
            if (hAvplay)
            {
                (HI_VOID)HI_UNF_AVPLAY_Reset(hAvplay, NULL);
            }

            (HI_VOID)HI_UNF_DMX_ResetTSBuffer(hTsBuffer);

            TRACE_INFO("Read ts file end and rewind and Reset TS BUFFER and AVPLAYER...............!\n");
            rewind(pTsFile);
            g_bUsrThreadFlag = HI_FALSE;
            continue;
        }

        s32Ret = HI_UNF_DMX_PutTSBuffer(hTsBuffer, Readlen);
        if (s32Ret != HI_SUCCESS )
        {
           HI_ERR_CC( "failed to HI_UNF_DMX_PutTSBuffer\n");
        }
    }

    if (pTsFile)
    {
        fclose(pTsFile);
        pTsFile = NULL;
    }

    return HI_SUCCESS;
}

static void PrintAribCCCaptionInfo(void)
{
    HI_U8 i;
    HI_UNF_CC_ARIB_INFO_S *pstCCAribInfo= HI_NULL;

    pstCCAribInfo = (HI_UNF_CC_ARIB_INFO_S *)malloc(sizeof(HI_UNF_CC_ARIB_INFO_S));

    if(HI_SUCCESS==HI_UNF_CC_GetARIBCCInfo(hcc,pstCCAribInfo))
    {

        TRACE_INFO("===================PrintAribCCCaptionInfo===================\n");

        TRACE_INFO("enCCAribTMD:%d\n",pstCCAribInfo->enCCAribTMD);
        TRACE_INFO("u32NumLanguage:%d\n",pstCCAribInfo->u32NumLanguage);
        for(i=0 ; i<pstCCAribInfo->u32NumLanguage ; i++)
        {
            TRACE_INFO("u8LanguageTag:%d,",pstCCAribInfo->stCCAribInfonode[i].u8LanguageTag);
            TRACE_INFO("enCCAribDMF:0x%x,",pstCCAribInfo->stCCAribInfonode[i].enCCAribDMF);
            TRACE_INFO("acISO639LanguageCode:%s,",pstCCAribInfo->stCCAribInfonode[i].acISO639LanguageCode);
            TRACE_INFO("enCCAribDF:%d,",pstCCAribInfo->stCCAribInfonode[i].enCCAribDF);
            TRACE_INFO("enCCAribTCS:%d,",pstCCAribInfo->stCCAribInfonode[i].enCCAribTCS);
            TRACE_INFO("enCCAribRollup:%d",pstCCAribInfo->stCCAribInfonode[i].enCCAribRollup);
        }
        TRACE_INFO("\n=====================================================\n");
    }
    else
    {
        TRACE_INFO("HI_UNF_CC_GetARIBCCCaptionInfo fail!\n");
    }

     free(pstCCAribInfo);
     pstCCAribInfo = HI_NULL;

}


static HI_S32 CmdTask(PMT_COMPACT_TBL *pProgTbl, HI_U8 u8ProgNo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8ProgramNo = u8ProgNo;
    HI_S8 inputCmd[32];

    while (1)
    {
        SAMPLE_GET_INPUTCMD(inputCmd);

        if ('q' == inputCmd[0])
        {
            TRACE_INFO("now exit!\n");
            g_bTaskQuit = HI_TRUE;
            break;
        }
        else if('p' == inputCmd[0])
        {
            u8ProgramNo = SelectProgram(pProgTbl);
            g_bUsrThreadFlag = HI_FALSE;
            s32Ret = StartAVPlay(pProgTbl, u8ProgramNo);
            if (s32Ret != HI_SUCCESS )
            {
                HI_ERR_CC("failed to StartAVPlay\n");
            }

            s32Ret = HI_UNF_CC_Stop(hcc);
            s32Ret |= HI_UNF_CC_Reset(hcc);
            if (s32Ret != HI_SUCCESS )
            {
                HI_ERR_CC("failed to HI_UNF_CC_Reset\n");
            }

            if(HI_UNF_CC_DATA_TYPE_ARIB == g_enCCDataType)
            {
                s32Ret = StartDataFilter(pProgTbl, u8ProgramNo);
                if (HI_SUCCESS != s32Ret)
                {
                   TRACE_INFO("failed to StartDataFilter\n");
                }
            }

            s32Ret = HI_UNF_CC_Start(hcc);
            if (s32Ret != HI_SUCCESS )
            {
                HI_ERR_CC("failed to HI_UNF_CC_Start\n");
            }
            continue;
        }
        else if('w' == inputCmd[0])
        {
            SwitchCaptionOut(hcc);
            continue;
        }
        else
        {
            TRACE_INFO("please input the 'q' to quit.\n");
            TRACE_INFO("please input the 'p' to choose program.\n");
            TRACE_INFO("please input the 'w' to start/stop caption.\n");
        }

        switch(g_enCCDataType)
        {
            case HI_UNF_CC_DATA_TYPE_608:
            case HI_UNF_CC_DATA_TYPE_708:

                TRACE_INFO("please input the 'a' to select text color.\n");
                TRACE_INFO("please input the 's' to select text bg color.\n");
                TRACE_INFO("please input the 'd' to select text bg opacity.\n");
                TRACE_INFO("please input the 'f' to select caption channel.\n");
                if('a' == inputCmd[0])
                {
                    SwitchColor(1);
                }
                else if('s' == inputCmd[0])
                {
                    SwitchColor(0);

                }
                else if ('d' == inputCmd[0])
                {
                    SwitchOpacity();
                }
                else if('f' == inputCmd[0])
                {
                    SwitchCCChanel();
                }
                break;
            case HI_UNF_CC_DATA_TYPE_ARIB:
                TRACE_INFO("please input the 'i' to print arib cc caption information.\n");
                if('i' == inputCmd[0])
                {
                    PrintAribCCCaptionInfo();
                }
            default:
                break;
        }
    }

    return s32Ret;
}

void show_help()
{
    TRACE_INFO("\nUsage:\n");
    TRACE_INFO("sample_cc [options] [file ..] [qam ..]\n");
    TRACE_INFO("    -f: path of the cc stream file\n");
}

static HI_S32 parseCmdParam(int nArgc, char** pArgv, CMD_LINE_ST *pstCmd)
{

    if (nArgc <= 2)
    {
        show_help();
        return HI_FAILURE;
    }

    switch (*pArgv[1])
    {
        case '-':
        {
            pArgv[1]++;
            switch(*pArgv[1])
            {
                case 'f':
                {
                    if (nArgc >= 3)
                    {
                        pstCmd->pszFileName = (HI_U8*)pArgv[2];
                    }
                }
                break;
                case 't':
                {
                    pstCmd->pszFileName = NULL;

                    if (nArgc >= 5)
                    {
                        pstCmd->u32TunerFreq = strtol(pArgv[2],NULL,0);
                        pstCmd->u32TunerSymbolRate = strtol(pArgv[3],NULL,0);
                        pstCmd->u32TunerQam = strtol(pArgv[4],NULL,0);
                    }
                    else if (nArgc >= 4)
                    {
                        pstCmd->u32TunerFreq = strtol(pArgv[2],NULL,0);
                        pstCmd->u32TunerSymbolRate = strtol(pArgv[3],NULL,0);
                        pstCmd->u32TunerQam = 64;
                    }
                    else
                    {
                        pstCmd->u32TunerFreq = strtol(pArgv[2],NULL,0);
                        pstCmd->u32TunerSymbolRate = 6875;
                        pstCmd->u32TunerQam = 64;
                    }
                }
                break;
                default:
                {
                    pstCmd->u32TunerFreq = 610;
                    pstCmd->u32TunerSymbolRate = 6875;
                    pstCmd->u32TunerQam = 64;
                    pstCmd->pszFileName = NULL;
                }
                break;
            }
        }
        break;
        default:
        {
            show_help();
            return HI_FAILURE;
        }

    }


    return HI_SUCCESS;
}



HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    HI_S32 s32Ret = HI_SUCCESS;
    pthread_t usrRecvThread = 0;
    pthread_t stInjectTSThread;

    CMD_LINE_ST stCmdLine = {0};

    s32Ret = parseCmdParam(argc, argv, &stCmdLine);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    (HI_VOID)HI_SYS_Init();

    CC_Data_Source_Param_S stParam;
    memset(&stParam, 0, sizeof(stParam));

    if (stCmdLine.pszFileName == NULL)
    {
        stParam.type = DATA_SOURCE_TUNER;
        stParam.stTunerParam.u32TunerFreq = stCmdLine.u32TunerFreq;
        stParam.stTunerParam.u32TunerSymbolRate = stCmdLine.u32TunerSymbolRate;
        stParam.stTunerParam.u32TunerQam = stCmdLine.u32TunerQam;
    }
    else
    {
        stParam.type = DATA_SOURCE_FILE;
        strncpy((char *)&stParam.au8File[0], (const char *)stCmdLine.pszFileName, sizeof(stParam.au8File));
    }

    s32Ret = StartDmx(&stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CC( "failed to StartDmx\n");
        goto SYS_DEINIT;
    }

    pthread_create(&stInjectTSThread, NULL, (void * (*)(void *))InjectTsTask, &stParam);

    /* Search table, including CC  informations */
    PMT_COMPACT_TBL   *pProgTbl = HI_NULL;

    HIADP_Search_Init();

    while (HI_SUCCESS != HIADP_Search_GetAllPmt(0, &pProgTbl))
    {
        HI_ERR_CC("failed to HIADP_Search_GetAllPmt\n");
        usleep(100000);
    }
    HI_U8 u8ProgNo = SelectProgram(pProgTbl);

    s32Ret = StartAVPlay(pProgTbl, u8ProgNo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CC("failed to StartAVPlay\n");
        goto EXIT;
    }

    s32Ret = StartCC(pProgTbl, u8ProgNo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CC("failed to StartCC\n");
        goto EXIT;
    }

    if(HI_UNF_CC_DATA_TYPE_ARIB == g_enCCDataType)
    {
        s32Ret = StartDataFilter(pProgTbl, u8ProgNo);
        if (HI_SUCCESS != s32Ret)
        {
           TRACE_INFO("failed to StartDataFilter\n");
           goto EXIT;
        }
    }
    else
    {
        pthread_create(&usrRecvThread,NULL,UsrDataInject,NULL);
    }

    (HI_VOID)CmdTask(pProgTbl, u8ProgNo);

    pthread_join(stInjectTSThread, NULL);
    if(usrRecvThread)
    {
        pthread_join(usrRecvThread, NULL);
    }

EXIT:

    (HI_VOID)CC_Data_DeInit();

#ifdef HI_FRONTEND_SUPPORT
    if (stCmdLine.pszFileName == NULL)
    {

        HIADP_Tuner_DeInit();

    }
#endif

    if(hcc)
    {
        (HI_VOID)HI_UNF_CC_Stop(hcc);
        (HI_VOID)HI_UNF_CC_Destroy(hcc);
        (HI_VOID)HI_UNF_CC_DeInit();
        g_bStartFlag = HI_FALSE;
        hcc = 0;
    }

    if (hAvplay)
    {
        HI_UNF_AVPLAY_STOP_OPT_S stStopOpt;
        stStopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
        stStopOpt.u32TimeoutMs = 0;
        (HI_VOID)HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &stStopOpt);
        (HI_VOID)HI_UNF_SND_Detach(hTrack, hAvplay);
        (HI_VOID)HI_UNF_VO_SetWindowEnable(hWindow,HI_FALSE);
        (HI_VOID)HI_UNF_VO_DetachWindow(hWindow, hAvplay);
        (HI_VOID)HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        (HI_VOID)HI_UNF_AVPLAY_Destroy(hAvplay);
        (HI_VOID)HI_UNF_AVPLAY_DeInit();
        hAvplay = 0;
    }

    (HI_VOID)HIADP_Search_FreeAllPmt(pProgTbl);
    (HI_VOID)HIADP_Search_DeInit();

    if (hWindow)
    {
        (HI_VOID)HI_UNF_VO_DestroyWindow(hWindow);
        (HI_VOID)HIADP_VO_DeInit();
        hWindow = 0;
    }
    if(hOutput)
    {
        CC_Output_DeInit(hOutput);
    }

    (HI_VOID)HIADP_Disp_DeInit();
    (HI_VOID)HIADP_Snd_DeInit();

SYS_DEINIT:
    (HI_VOID)HI_SYS_DeInit();

    return HI_SUCCESS;
}
