#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "hi_type.h"
#include "hi_unf_disp.h"
#include "hi_unf_common.h"
#include "hi_unf_demux.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "hi_unf_so.h"
#include "hi_adp_mpi.h"
#include "hi_unf_sound.h"
#include "hi_adp.h"

#include "hi_unf_subt.h"
#include "sample_subtitle_out.h"
#include "sample_subtitle_data.h"

#define HI_FATAL_SUBT(fmt...)      HI_FATAL_PRINT(HI_ID_SUBT, fmt)
#define HI_ERR_SUBT(fmt...)        HI_ERR_PRINT(HI_ID_SUBT, fmt)
#define HI_WARN_SUBT(fmt...)       HI_WARN_PRINT(HI_ID_SUBT, fmt)
#define HI_INFO_SUBT(fmt...)       HI_INFO_PRINT(HI_ID_SUBT, fmt)
#define TRACE_INFO printf

#define DMXID (0)

#define HI_SUBT_MAX_PTS_OFFSET 0x100000 /* the max value of pts offset */

typedef struct tagSubt_Tuner_Param_S
{
    HI_U32 u32TunerFreq;
    HI_U32 u32TunerSymbolRate;
    HI_U32 u32TunerQam;
}Subt_Tuner_Param_S;

typedef enum tagSubt_Inject_Data_type_E
{
    DATA_SOURCE_FILE = 0,
    DATA_SOURCE_TUNER
}Subt_Data_SOURCE_type_E;

typedef struct tagSubt_Data_Source_Param_S
{
    Subt_Data_SOURCE_type_E type;
    HI_U8 au8File[256];
    Subt_Tuner_Param_S stTunerParam;
}Subt_Data_Source_Param_S;

typedef struct tagCmdLinePara
{
    HI_U32 u32TunerFreq;
    HI_U32 u32TunerSymbolRate;
    HI_U32 u32TunerQam;
    HI_U8* pszFileName;
}CMD_LINE_ST;


HI_BOOL g_bTaskQuit = HI_FALSE;
HI_HANDLE hUnfSubt = 0;
HI_HANDLE hUnfSO = 0;
HI_HANDLE hAvplay = 0;
HI_HANDLE hWindow = 0;
HI_HANDLE hSubtOut = 0;
HI_HANDLE hTsBuffer = 0;
HI_HANDLE hTrack = 0;
HI_UNF_SUBT_DATA_TYPE_E g_enSubtType = HI_UNF_SUBT_BUTT;


/* ref iso-8859-1 subtitle language */
HI_U8 g_LangChTable[16][16] =
{
     /* 0    1    2    3    4    5    6    7     8    9    A    B    C    D    E    F */
/*0*/ {' ', ' ', ' ', '0', '@', 'P', '`', 'p',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*1*/ {' ', ' ', '!', '1', 'A', 'Q', 'a', 'q',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*2*/ {' ', ' ', '"', '2', 'B', 'R', 'b', 'r',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*3*/ {' ', ' ', '#', '3', 'C', 'S', 'c', 's',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*4*/ {' ', ' ', '$', '4', 'D', 'T', 'd', 't',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*5*/ {' ', ' ', '%', '5', 'E', 'U', 'e', 'u',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*6*/ {' ', ' ', '&', '6', 'F', 'V', 'f', 'v',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*7*/ {' ', ' ','\'', '7', 'G', 'W', 'g', 'w',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*8*/ {' ', ' ', '(', '8', 'H', 'X', 'h', 'x',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*9*/ {' ', ' ', ')', '9', 'I', 'Y', 'i', 'y',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*A*/ {' ', ' ', '*', ':', 'J', 'Z', 'j', 'z',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*B*/ {' ', ' ', '+', ';', 'K', '[', 'k', '{',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*C*/ {' ', ' ', ',', '<', 'L','\\', 'l', '|',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*D*/ {' ', ' ', '-', '=', 'M', ']', 'm', '}',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*E*/ {' ', ' ', '.', '>', 'N', '^', 'n', '~',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},

/*F*/ {' ', ' ', '/', '?', 'O', '_', 'o', ' ',  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
};

#define CHECK_NIBBLE(a) ( (a) > 0xf ? HI_TRUE: HI_FALSE )




/* Get language by subtitle code, per language composed of three subtitle codec */
static HI_S32 GetLangStr(HI_U32 u32LangCode, HI_U8 *pu8Buf, HI_U8 u8BufLen)
{
    HI_U8 u8FirstNibble = 0;
    HI_U8 u8SecondNibble = 0;

    if (pu8Buf == HI_NULL || u8BufLen < 3)
    {
        return HI_FAILURE;
    }

    u8FirstNibble = ((u32LangCode >> 16)&0xf0) >> 4;
    u8SecondNibble = (u32LangCode >> 16)&0x0f;

    if (u8FirstNibble > 0xf || u8SecondNibble > 0xf)
    {
        return HI_FAILURE;
    }

    pu8Buf[0] = g_LangChTable[u8SecondNibble][u8FirstNibble];

    u8FirstNibble = ((u32LangCode >> 8)&0xf0) >> 4;
    u8SecondNibble = (u32LangCode >> 8)&0x0f;

    if (u8FirstNibble > 0xf || u8SecondNibble > 0xf)
    {
        return HI_FAILURE;
    }

    pu8Buf[1] = g_LangChTable[u8SecondNibble][u8FirstNibble];

    u8FirstNibble = ((u32LangCode)&0xf0) >> 4;
    u8SecondNibble = (u32LangCode)&0x0f;

    if (u8FirstNibble > 0xf || u8SecondNibble > 0xf)
    {
        return HI_FAILURE;
    }

    pu8Buf[2] = g_LangChTable[u8SecondNibble][u8FirstNibble];

    return HI_SUCCESS;
}

#define SYN_USING_LOCALTIME 0
static HI_S32 GetCurPts(HI_VOID* pUserData, HI_S64 *ps64CurrentPts)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_AVPLAY_STATUS_INFO_S stStatusInfo;
    HI_HANDLE hAvplayer = (HI_HANDLE)(intptr_t)pUserData;
    static HI_U32 u32PrePtsTime = 0;


	memset(&stStatusInfo, 0x0, sizeof(stStatusInfo));

    s32Ret = HI_UNF_AVPLAY_GetStatusInfo(hAvplayer, &stStatusInfo);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_SUBT("failed to HI_UNF_AVPLAY_GetStatusInfo\n");
    }

#if SYN_USING_LOCALTIME
    *ps64CurrentPts = stStatusInfo.stSyncStatus.u32LocalTime;
#else
    *ps64CurrentPts = stStatusInfo.stSyncStatus.u32LastVidPts;
#endif


    if (*ps64CurrentPts < u32PrePtsTime)
    {
        TRACE_INFO("Get PTS:%llu, pre time is %u\n", *ps64CurrentPts, u32PrePtsTime);

        /* reset the output */
#if !SYN_USING_LOCALTIME
        if (hUnfSO)
        {
            (HI_VOID)HI_UNF_SO_ResetSubBuf(hUnfSO);
        }
#endif
    }

    u32PrePtsTime = *ps64CurrentPts;

    return HI_SUCCESS;
}


static HI_S32 FilterDataCallback(HI_U32 u32UserData, HI_U8 *pu8Data, HI_U32 u32DataLength)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32SubtPID = u32UserData;

    if (hUnfSubt)
    {
        HI_INFO_SUBT("u32SubtPID=0x%x, u32DataLength=%d\n", u32SubtPID, u32DataLength);
        if(u32SubtPID)
        {
            s32Ret = HI_UNF_SUBT_InjectData(hUnfSubt, u32SubtPID, pu8Data, u32DataLength);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_SUBT("failed to HI_UNF_SUBT_InjectData, u32SubtPID=0x%x\n",u32SubtPID);
            }
        }
    }

    return s32Ret;
}

static HI_S32 UnfSubtCallback(HI_VOID* pUserData, HI_UNF_SUBT_DATA_S *pstData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hSO = (HI_HANDLE)(intptr_t)pUserData;
    HI_UNF_SO_SUBTITLE_INFO_S stSubtitleOut;
    HI_UNF_AVPLAY_STATUS_INFO_S stStatusInfo;

    if (NULL != pstData)
    {
        /* PTS validity check */
        memset(&stStatusInfo, 0, sizeof(HI_UNF_AVPLAY_STATUS_INFO_S));
        s32Ret = HI_UNF_AVPLAY_GetStatusInfo(hAvplay, &stStatusInfo);
        if ((s32Ret == HI_SUCCESS) && ((pstData->u32PTS + pstData->u32Duration) < stStatusInfo.stSyncStatus.u32LastVidPts))
        {
            if ((pstData->u32PTS + HI_SUBT_MAX_PTS_OFFSET) < stStatusInfo.stSyncStatus.u32LastVidPts)
            {
                HI_INFO_SUBT("subtitle pts is wrong, try to update subtitle pts=%d -> %d, Duration=%d\n", pstData->u32PTS,
                       stStatusInfo.stSyncStatus.u32LastVidPts, pstData->u32Duration);
                pstData->u32PTS = stStatusInfo.stSyncStatus.u32LastVidPts;
            }
            else
            {
                HI_INFO_SUBT("discard a subtitle, pts=%d, Duration=%d, current pts = %d\n", pstData->u32PTS,
                       pstData->u32Duration, stStatusInfo.stSyncStatus.u32LastVidPts);
                return HI_SUCCESS;
            }
        }

        /* send subtitle data to so */
        memset(&stSubtitleOut, 0, sizeof(stSubtitleOut));

        stSubtitleOut.eType = (HI_UNF_SO_SUBTITLE_TYPE_E)pstData->enDataType;
        if (pstData->enPageState == HI_UNF_SUBT_PAGE_NORMAL_CASE)
        {
            stSubtitleOut.unSubtitleParam.stGfx.enMsgType = HI_UNF_SO_DISP_MSG_NORM;
        }
        else
        {
            stSubtitleOut.unSubtitleParam.stGfx.enMsgType = HI_UNF_SO_DISP_MSG_ERASE;
        }

        stSubtitleOut.unSubtitleParam.stGfx.x = pstData->u32x;
        stSubtitleOut.unSubtitleParam.stGfx.y = pstData->u32y;
        stSubtitleOut.unSubtitleParam.stGfx.w = pstData->u32w;
        stSubtitleOut.unSubtitleParam.stGfx.h = pstData->u32h;

        stSubtitleOut.unSubtitleParam.stGfx.s32BitWidth = pstData->u32BitWidth;
        if(pstData->pvPalette && pstData->u32PaletteItem)
        {
            memcpy(stSubtitleOut.unSubtitleParam.stGfx.stPalette, pstData->pvPalette, pstData->u32PaletteItem);
        }

        stSubtitleOut.unSubtitleParam.stGfx.s64Pts = pstData->u32PTS;
        stSubtitleOut.unSubtitleParam.stGfx.u32Duration = pstData->u32Duration;

        stSubtitleOut.unSubtitleParam.stGfx.u32Len = pstData->u32DataLen;
        stSubtitleOut.unSubtitleParam.stGfx.pu8PixData = pstData->pu8SubtData;
        stSubtitleOut.unSubtitleParam.stGfx.u32CanvasWidth = pstData->u32DisplayWidth;
        stSubtitleOut.unSubtitleParam.stGfx.u32CanvasHeight = pstData->u32DisplayHeight;

        if (hSO)
        {
            s32Ret = HI_UNF_SO_SendData(hSO, &stSubtitleOut, 1000);
            if (s32Ret != HI_SUCCESS)
            {
                HI_WARN_SUBT("failed to HI_UNF_SO_SendData\n");
            }
        }
    }

    return HI_SUCCESS;
}



static HI_U8 SelectProgram(PMT_COMPACT_TBL *pSubTitleProgTbl)
{
    HI_U8 u8ProgNo = 0; /* for prog No. */

    TRACE_INFO("program number is %d \n", pSubTitleProgTbl->prog_num);

    /* for multi-program, need to select someone, range from 0 to total number -1 */

    if (pSubTitleProgTbl->prog_num > 1)
    {
        HI_U8  au8ProgNo[32]; /* for program number */

        TRACE_INFO("\n Please input a number from 0 to %d for program: \n", pSubTitleProgTbl->prog_num-1);

		SAMPLE_GET_INPUTCMD(au8ProgNo);

        u8ProgNo = atoi((char*)au8ProgNo);
        if (u8ProgNo >= pSubTitleProgTbl->prog_num)
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

static HI_U8 SelectSubtitle(PMT_COMPACT_PROG *pstProginfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8  u8Index = 0;
    HI_U32 u32LangCode = 0;
    HI_U8  au8Buf[32] = {0};
    HI_U8  u8SubtNo = 0;         /* for subtitle No. */


    TRACE_INFO("DVB subtitle number is %d\n", pstProginfo->u16SubtitlingNum);

    /*for multi-subtitle,need to select someone */
    if (pstProginfo->u16SubtitlingNum > 1)
    {
        for(u8Index=0; u8Index<pstProginfo->u16SubtitlingNum; u8Index++)
        {
            u32LangCode = pstProginfo->SubtitingInfo[u8Index].DesInfo[0].u32LangCode;

            memset(au8Buf, 0, sizeof(au8Buf));
            s32Ret = GetLangStr(u32LangCode, au8Buf, sizeof(au8Buf));
            if (s32Ret != HI_SUCCESS)
            {
                HI_WARN_SUBT("failed to GetLangStr\n");
            }
            TRACE_INFO("%d is the %s language\n", u8Index, au8Buf);
        }

        TRACE_INFO("Please input a number from 0 to %d for subtitle:\n", pstProginfo->u16SubtitlingNum-1);

        SAMPLE_GET_INPUTCMD(au8Buf);

        u8SubtNo = atoi((char*)au8Buf);
        if (u8SubtNo >= pstProginfo->u16SubtitlingNum)
        {
            TRACE_INFO("\nsubtitle %d is not exist!! usd default subtitle 0\n", u8SubtNo);
            u8SubtNo = 0;
        }
    }
    else
    {
        TRACE_INFO("\nusd default subtitle 0\n");
        u8SubtNo = 0;
    }

    return u8SubtNo;
}

static HI_S32 StartAVPlay(PMT_COMPACT_TBL *pSubTitleProgTbl, HI_U8 u8ProgNo)
{
    HI_S32                      s32Ret = HI_SUCCESS;
    HI_UNF_AVPLAY_ATTR_S        stAvplayAttr;
    HI_UNF_AUDIOTRACK_ATTR_S    stTrackAttr;

    if (0 == hAvplay)
    {
        s32Ret = HIADP_AVPlay_RegADecLib();
        s32Ret |= HI_UNF_AVPLAY_Init();
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_SUBT("failed to HI_UNF_AVPLAY_Init\n");
            goto ERROR;
        }

        memset(&stAvplayAttr, 0 ,sizeof(stAvplayAttr));
        s32Ret = HI_UNF_AVPLAY_GetDefaultConfig(&stAvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
        stAvplayAttr.stStreamAttr.u32VidBufSize = 0x300000;
        s32Ret |= HI_UNF_AVPLAY_Create(&stAvplayAttr, &hAvplay);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_SUBT("failed to HI_UNF_AVPLAY_Create\n");
            goto ERROR;
        }

        s32Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
        s32Ret |= HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to HI_UNF_AVPLAY_ChnOpen\n");
            goto ERROR;
        }

        s32Ret = HIADP_Snd_Init();
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to HIADP_Snd_Init\n");
            goto ERROR;
        }

        s32Ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_50);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to HIADP_Disp_Init\n");
            goto ERROR;
        }

        s32Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
        s32Ret |= HIADP_VO_CreatWin(HI_NULL,&hWindow);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to HIADP_VO_CreatWin\n");
            goto ERROR;
        }

        s32Ret = HI_UNF_VO_AttachWindow(hWindow, hAvplay);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to HI_UNF_VO_AttachWindow\n");
            goto ERROR;
        }
        s32Ret = HI_UNF_VO_SetWindowEnable(hWindow, HI_TRUE);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to HI_UNF_VO_SetWindowEnable\n");
            goto ERROR;
        }

        s32Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_SUBT("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
            goto ERROR;
        }
        s32Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_SUBT("failed to HI_UNF_SND_CreateTrack\n");
            goto ERROR;
        }

        s32Ret = HI_UNF_SND_Attach(hTrack, hAvplay);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_SUBT("failed to HI_UNF_SND_Attach\n");
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
            HI_ERR_SUBT("failed to HI_UNF_AVPLAY_SetAttr\n");
            goto ERROR;
        }
    }

    s32Ret = HIADP_AVPlay_PlayProg(hAvplay,pSubTitleProgTbl,u8ProgNo,HI_TRUE);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_SUBT("failed to HIADP_AVPlay_PlayProg\n");
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

static HI_S32 CreateDVBSubt(PMT_COMPACT_PROG *pstProginfo)
{
    HI_U8 u8Index ;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_SUBT_PARAM_S stSubtParam;

    if(HI_NULL == pstProginfo)
    {
        HI_ERR_SUBT("Param is NULL!\n");
        return HI_FAILURE;
    }
    memset(&stSubtParam, 0, sizeof(stSubtParam));
    stSubtParam.enDataType = HI_UNF_SUBT_DVB;

    stSubtParam.pfnCallback = UnfSubtCallback;
    stSubtParam.pUserData = (HI_VOID*)(intptr_t)hUnfSO;
    stSubtParam.u8SubtItemNum = pstProginfo->u16SubtitlingNum;
    for (u8Index = 0; u8Index < stSubtParam.u8SubtItemNum; u8Index++)
    {
        stSubtParam.astItems[u8Index].u32SubtPID = pstProginfo->SubtitingInfo[u8Index].u16SubtitlingPID;
        stSubtParam.astItems[u8Index].u16PageID = pstProginfo->SubtitingInfo[u8Index].DesInfo[0].u16PageID;
        stSubtParam.astItems[u8Index].u16AncillaryID = pstProginfo->SubtitingInfo[u8Index].DesInfo[0].u16AncillaryPageID;
    }
    s32Ret = HI_UNF_SUBT_Init();
    s32Ret |= HI_UNF_SUBT_Create(&stSubtParam, &hUnfSubt);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to HI_UNF_SUBT_Create\n");
    }

    return s32Ret;

}

static HI_S32 CreateSCTESubt()
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_SUBT_PARAM_S stSubtParam;

    memset(&stSubtParam, 0, sizeof(stSubtParam));
    stSubtParam.enDataType = HI_UNF_SUBT_SCTE;

    stSubtParam.pfnCallback = UnfSubtCallback;
    stSubtParam.pUserData = (HI_VOID*)(intptr_t)hUnfSO;

    s32Ret = HI_UNF_SUBT_Init();
    s32Ret |= HI_UNF_SUBT_Create(&stSubtParam, &hUnfSubt);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to HI_UNF_SUBT_Create\n");
    }
    s32Ret = HI_UNF_SUBT_RegGetPtsCb(hUnfSubt, GetCurPts, (HI_VOID*)(intptr_t)hAvplay);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to HI_UNF_SCTESUBT_RegGetPtsCb\n");
    }
    return s32Ret;
}

static HI_S32 UpdateDVBSubt(PMT_COMPACT_PROG *pstProginfo)
{
    HI_U8 u8Index = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_SUBT_PARAM_S stSubtParam;

    if(HI_NULL == pstProginfo)
    {
        HI_ERR_SUBT("Param is NULL!\n");
        return HI_FAILURE;
    }

    memset(&stSubtParam, 0, sizeof(stSubtParam));
    stSubtParam.pfnCallback = UnfSubtCallback;
    stSubtParam.pUserData = (HI_VOID*)(intptr_t)hUnfSO;
    stSubtParam.enDataType = HI_UNF_SUBT_DVB;
    stSubtParam.u8SubtItemNum = pstProginfo->u16SubtitlingNum;
    for (u8Index = 0; u8Index < stSubtParam.u8SubtItemNum; u8Index++)
    {
        stSubtParam.astItems[u8Index].u32SubtPID = pstProginfo->SubtitingInfo[u8Index].u16SubtitlingPID;
        stSubtParam.astItems[u8Index].u16PageID = pstProginfo->SubtitingInfo[u8Index].DesInfo[0].u16PageID;
        stSubtParam.astItems[u8Index].u16AncillaryID = pstProginfo->SubtitingInfo[u8Index].DesInfo[0].u16AncillaryPageID;
        TRACE_INFO("update subtitle[0x%x,0x%x,0x%x]\n",stSubtParam.astItems[u8Index].u32SubtPID,
                        stSubtParam.astItems[u8Index].u16PageID,stSubtParam.astItems[u8Index].u16AncillaryID);
    }

    s32Ret = HI_UNF_SUBT_Reset(hUnfSubt);
    s32Ret |= HI_UNF_SUBT_Update(hUnfSubt, &stSubtParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to HI_UNF_SUBT_Update\n");
    }
    return s32Ret;
}

/*0--switch to dvb,1--switch to scte,default--none*/
static HI_S32 SwitchDecoder(HI_U32 type, PMT_COMPACT_PROG *pstProginfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    switch(type)
    {
        case 0:     /**DVB*/
            TRACE_INFO("DVB subtitle\n");
            if(g_enSubtType == HI_UNF_SUBT_DVB)
            {
                s32Ret = UpdateDVBSubt(pstProginfo);
            }
            else if(g_enSubtType == HI_UNF_SUBT_SCTE)
            {
                s32Ret = HI_UNF_SUBT_Destroy(hUnfSubt);
                s32Ret |= CreateDVBSubt(pstProginfo);
            }
            else
            {
                s32Ret |= CreateDVBSubt(pstProginfo);
            }
            g_enSubtType = HI_UNF_SUBT_DVB;
            break;
        case 1:         /**SCTE*/
            TRACE_INFO("SCTE subtitle\n");
            if(g_enSubtType == HI_UNF_SUBT_DVB)
            {
                s32Ret = HI_UNF_SUBT_Destroy(hUnfSubt);
                s32Ret |= CreateSCTESubt();
            }
            else if(g_enSubtType == HI_UNF_SUBT_SCTE)
            {
                s32Ret = HI_UNF_SUBT_Reset(hUnfSubt);
            }
            else
            {
                s32Ret |= CreateSCTESubt();
            }
            g_enSubtType = HI_UNF_SUBT_SCTE;
            break;
        default:
            TRACE_INFO("Input error!No subtitle!\n");
            g_enSubtType = HI_UNF_SUBT_BUTT;
            if(hUnfSubt)
            {
                s32Ret = HI_UNF_SUBT_Destroy(hUnfSubt);
                hUnfSubt = 0;
            }
            break;
    }
    return s32Ret;
}
static HI_S32 CreateDecoderByProgram(PMT_COMPACT_PROG *pstProginfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    if(pstProginfo->SubtType == (HI_UNF_SUBT_DVB|HI_UNF_SUBT_SCTE))
    {
        HI_U32 type = 0;
        HI_U8 au8Buf[32];
        TRACE_INFO("The program  has two kinds of subtitles:\n");
        TRACE_INFO("\t0 ---  choose DVB subt\n");
        TRACE_INFO("\t1 ---  choose SCTE subt\n");
        TRACE_INFO("Input choice:");

        SAMPLE_GET_INPUTCMD(au8Buf);
        type = atoi((char*)au8Buf);

        s32Ret = SwitchDecoder(type, pstProginfo);

    }
    else if(pstProginfo->SubtType == HI_UNF_SUBT_DVB)
    {
        s32Ret = SwitchDecoder(0, pstProginfo);
    }
    else if(pstProginfo->SubtType == HI_UNF_SUBT_SCTE)
    {
        s32Ret = SwitchDecoder(1, pstProginfo);
    }
    else
    {
        s32Ret = SwitchDecoder(2, pstProginfo);
    }
    return s32Ret;
}
static HI_S32 StartSubtitle(PMT_COMPACT_TBL *pSubTitleProgTbl, HI_U8 u8ProgNo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    static HI_U8 u8CurProgNo = 0;

    PMT_COMPACT_PROG *pstProginfo = &pSubTitleProgTbl->proginfo[u8ProgNo];

    /* Create SO instance, so that the subtitle data can be processed with this instance */
    if (0 == hSubtOut)
    {
        s32Ret = Subt_Output_Init(&hSubtOut);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to Subt_Output_Init\n");
            goto ERROR;
        }
    }

    if (0 == hUnfSO)
    {
        s32Ret = HI_UNF_SO_Init();
        s32Ret |= HI_UNF_SO_Create(&hUnfSO);

        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_SUBT("failed to SUBT_DataRecv_BindParsing\n");

            return HI_FAILURE;
        }
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to HI_UNF_SO_Create\n");
            goto ERROR;
        }
        s32Ret = HI_UNF_SO_RegGetPtsCb(hUnfSO, GetCurPts, (HI_VOID*)(intptr_t)hAvplay);
        s32Ret |= HI_UNF_SO_RegOnDrawCb(hUnfSO, Subt_Output_OnDraw, Subt_Output_OnClear, (HI_VOID*)(intptr_t)hSubtOut);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to HI_UNF_SO_RegOnDrawCb\n");
            goto ERROR;
        }
    }

    /* Create subtitle module instance */
    if (0 == hUnfSubt)
    {
        s32Ret = CreateDecoderByProgram(pstProginfo);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to CreateSubtDecoder\n");
            goto ERROR;
        }
        u8CurProgNo = u8ProgNo;
    }

    if (hUnfSubt)
    {
        HI_U8  u8SubtNo = 0;
        HI_UNF_SUBT_ITEM_S stSubtItem;

        if (hUnfSO)
        {
            HI_UNF_SO_CLEAR_PARAM_S stClearParam;

            memset(&stClearParam, 0, sizeof(HI_UNF_SO_CLEAR_PARAM_S));
            (HI_VOID)Subt_Output_OnClear(0, (HI_VOID *)&stClearParam);
            (HI_VOID)HI_UNF_SO_ResetSubBuf(hUnfSO);
        }

        if (u8CurProgNo != u8ProgNo)
        {
            s32Ret = CreateDecoderByProgram(pstProginfo);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_SUBT("failed to CreateDecoderByProgram\n");
                goto ERROR;
            }
            u8CurProgNo = u8ProgNo;
        }


        if(HI_UNF_SUBT_DVB == g_enSubtType)
        {
            u8SubtNo = SelectSubtitle(pstProginfo);

            memset(&stSubtItem, 0, sizeof(stSubtItem));
            stSubtItem.u32SubtPID = pstProginfo->SubtitingInfo[u8SubtNo].u16SubtitlingPID;
            stSubtItem.u16PageID = pstProginfo->SubtitingInfo[u8SubtNo].DesInfo[0].u16PageID;
            stSubtItem.u16AncillaryID = pstProginfo->SubtitingInfo[u8SubtNo].DesInfo[0].u16AncillaryPageID;
            TRACE_INFO("select subtitle[%d,%d,%d]\n",stSubtItem.u32SubtPID,stSubtItem.u16PageID,stSubtItem.u16AncillaryID);
            s32Ret = HI_UNF_SUBT_SwitchContent(hUnfSubt, &stSubtItem);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_SUBT("failed to HI_UNF_SUBT_SwitchContent\n");
            }
        }

    }

    return HI_SUCCESS;

ERROR:
    if (hUnfSubt)
    {
        (HI_VOID)HI_UNF_SUBT_Destroy(hUnfSubt);
        (HI_VOID)HI_UNF_SUBT_DeInit();
        hUnfSubt = 0;
    }

    if (hUnfSO)
    {
        (HI_VOID)HI_UNF_SO_Destroy(hUnfSO);
        (HI_VOID)HI_UNF_SO_DeInit();
        hUnfSO = 0;
    }

    if (hSubtOut)
    {
        (HI_VOID)Subt_Output_DeInit(hSubtOut);
        hSubtOut = 0;
    }

    return HI_FAILURE;
}

static HI_S32 StartDataFilter(PMT_COMPACT_TBL *pSubTitleProgTbl, HI_U8 u8ProgNo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8  u8Index = 0;
    HI_HANDLE hSubtData = 0;
    PMT_COMPACT_PROG *pstProginfo = &pSubTitleProgTbl->proginfo[u8ProgNo];
    SUBT_DATA_INSTALL_PARAM_S stInstallParam;

    s32Ret = SUBT_Data_DeInit();
    s32Ret |= SUBT_Data_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to SUBT_Data_Init\n");

        return HI_FAILURE;
    }

    stInstallParam.u32DmxID = 0;
    stInstallParam.pfnCallback = FilterDataCallback;

    if(HI_UNF_SUBT_DVB == g_enSubtType)/*DVB*/
    {
        for (u8Index = 0; u8Index < pstProginfo->u16SubtitlingNum; u8Index++)
        {
            stInstallParam.u16SubtPID = pstProginfo->SubtitingInfo[u8Index].u16SubtitlingPID;
            stInstallParam.u32UserData = (HI_U32)pstProginfo->SubtitingInfo[u8Index].u16SubtitlingPID;
            stInstallParam.enDataType = HI_UNF_SUBT_DVB;
            s32Ret |= SUBT_Data_Install(&stInstallParam, &hSubtData);
        }
    }
    else if(HI_UNF_SUBT_SCTE == g_enSubtType)/*SCTE*/
    {
        stInstallParam.u16SubtPID = pstProginfo->stSCTESubtInfo.u16SCTESubtPID;
        stInstallParam.u32UserData = (HI_U32)pstProginfo->stSCTESubtInfo.u16SCTESubtPID;
        stInstallParam.enDataType = HI_UNF_SUBT_SCTE;
        s32Ret |= SUBT_Data_Install(&stInstallParam, &hSubtData);
    }
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to SUBT_Data_Init\n");

        (HI_VOID)SUBT_Data_DeInit();

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 StartDmx(Subt_Data_Source_Param_S *pstParam)
{
    HI_S32  s32Ret = HI_SUCCESS;

    if (DATA_SOURCE_TUNER == pstParam->type)
    {
    #ifdef HI_FRONTEND_SUPPORT
        s32Ret = HIADP_Tuner_Init();
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to HIADP_Tuner_Init\n");

            return HI_FAILURE;
        }

        s32Ret = HIADP_Tuner_Connect(TUNER_USE,pstParam->stTunerParam.u32TunerFreq,
                            pstParam->stTunerParam.u32TunerSymbolRate, pstParam->stTunerParam.u32TunerQam);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to HIADP_Tuner_Connect\n");

            return HI_FAILURE;
        }

        s32Ret = HI_UNF_DMX_Init();
        s32Ret |= HI_UNF_DMX_AttachTSPort(DMXID,DEFAULT_DVB_PORT);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to HI_UNF_DMX_AttachTSPort\n");
            (HI_VOID)HI_UNF_DMX_DeInit();

            return HI_FAILURE;
        }
    #else
        HI_ERR_SUBT("Tuner is not support\n");
    #endif
    }
    else if (DATA_SOURCE_FILE == pstParam->type)
    {
        s32Ret = HI_UNF_DMX_Init();
        s32Ret |= HI_UNF_DMX_AttachTSPort(DMXID, HI_UNF_DMX_PORT_RAM_0);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to HI_UNF_DMX_AttachTSPort\n");
            (HI_VOID)HI_UNF_DMX_DeInit();

            return HI_FAILURE;
        }

        s32Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_0, 0x200000, &hTsBuffer);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_SUBT("failed to HI_UNF_DMX_CreateTSBuffer\n");
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

static HI_S32 SwitchAudio(PMT_COMPACT_TBL *pSubTitleProgTbl, HI_U8 u8ProgNo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S8  inputCmd[32];
    HI_U32 u32Index = 0;
    PMT_COMPACT_PROG *pstProginfo = &pSubTitleProgTbl->proginfo[u8ProgNo];
    HI_U32 u32AudioNum = pstProginfo->AElementNum;

    if (u32AudioNum <= 1)
    {
        TRACE_INFO("This program %d has only one audio\n", u8ProgNo);

        return HI_SUCCESS;
    }

    TRACE_INFO("Please input the audio index range(0-%d):",u32AudioNum-1);
    SAMPLE_GET_INPUTCMD(inputCmd);
    u32Index = atoi((const char *)inputCmd);
    if (u32Index >= u32AudioNum)
    {
        TRACE_INFO("Your input %d is invalid!\n",u32Index);

        return HI_FAILURE;
    }

    HI_U32 AudPid = pstProginfo->Audioinfo[u32Index].u16AudioPid;
    HI_U32 u32AudType = pstProginfo->Audioinfo[u32Index].u32AudioEncType;
    if (hAvplay)
    {
        s32Ret = HIADP_AVPlay_SwitchAud(hAvplay, AudPid, u32AudType);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_SUBT("failed to HIADP_AVPlay_SwitchAud\n");

            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}


static HI_S32 InjectTsTask(HI_VOID *args)
{
    HI_S32  s32Ret = HI_SUCCESS;
    HI_U32  Readlen = 0;
    HI_UNF_STREAM_BUF_S   StreamBuf;
    FILE *pTsFile = NULL;
    HI_UNF_DMX_TSBUF_STATUS_S stTsBufStatus = {0,0};

    Subt_Data_Source_Param_S *pstParam = (Subt_Data_Source_Param_S *)(args);

    if (DATA_SOURCE_FILE != pstParam->type)
    {
        HI_ERR_SUBT("data source not support!\n");
        return HI_FAILURE;
    }


    pTsFile = fopen((char*)pstParam->au8File, "rb");

    if(pTsFile == NULL)
    {
        HI_ERR_SUBT("\nfile %s open error!!\n", pstParam->au8File);

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
            
            TRACE_INFO("Read ts file end and rewind and Reset TS BUFFER and AVPLAYER...............!\n");

            /* wait for avplay end */
            do
            {
                sleep(1);
                HI_UNF_DMX_GetTSBufferStatus(hTsBuffer, &stTsBufStatus);
            } while(stTsBufStatus.u32UsedSize > 0x100);

            rewind(pTsFile);
            continue;
        }

        s32Ret = HI_UNF_DMX_PutTSBuffer(hTsBuffer, Readlen);
        if (s32Ret != HI_SUCCESS )
        {
            HI_WARN_SUBT("failed to HI_UNF_DMX_PutTSBuffer\n");
        }
    }

    if (pTsFile)
    {
        fclose(pTsFile);
        pTsFile = NULL;
    }

    return HI_SUCCESS;
}


static HI_S32 CmdTask(PMT_COMPACT_TBL *pSubTitleProgTbl, HI_U8 u8ProgNo)
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
        else if ('h' == inputCmd[0])
        {
            TRACE_INFO("please input the 'q' to quit.\n");
            TRACE_INFO("please input the 'p' to choose program.\n");
            TRACE_INFO("please input the 'm' to switch audio\n");
            TRACE_INFO("please input the 's' to choose subtitle.\n");
        }
        else if ('p' == inputCmd[0])
        {
            u8ProgramNo = SelectProgram(pSubTitleProgTbl);

            s32Ret = StartAVPlay(pSubTitleProgTbl, u8ProgramNo);
            if (s32Ret != HI_SUCCESS )
            {
                HI_WARN_SUBT("failed to StartAVPlay\n");
            }
            s32Ret = StartSubtitle(pSubTitleProgTbl, u8ProgramNo);
            if (s32Ret != HI_SUCCESS )
            {
                HI_WARN_SUBT("failed to StartSubtitle\n");
            }
            s32Ret = StartDataFilter(pSubTitleProgTbl, u8ProgramNo);
            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_SUBT("failed to StartDataFilter\n");
            }
        }
        else if ('s' == inputCmd[0])
        {
            if (hUnfSO)
            {
                (HI_VOID)HI_UNF_SO_ResetSubBuf(hUnfSO);
            }

            s32Ret = CreateDecoderByProgram(&pSubTitleProgTbl->proginfo[u8ProgramNo]);
            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_SUBT("failed to CreateDecoderByProgram\n");
            }

            s32Ret = StartDataFilter(pSubTitleProgTbl, u8ProgramNo);
            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_SUBT("failed to StartDataFilter\n");
            }

            s32Ret = StartSubtitle(pSubTitleProgTbl, u8ProgramNo);
            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_SUBT("failed to StartSubtitle\n");
            }
        }
        else if ('m' == inputCmd[0])
        {
            s32Ret = SwitchAudio(pSubTitleProgTbl, u8ProgramNo);
            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_SUBT("failed to SwitchAudio\n");
            }
        }
        else
        {
            TRACE_INFO("please input the 'q' to quit.\n");
            TRACE_INFO("please input the 'p' to choose program.\n");
            TRACE_INFO("please input the 'm' to switch audio\n");
            TRACE_INFO("please input the 's' to choose subtitle.\n");
        }
    }

    return HI_SUCCESS;
}

void show_help()
{
    TRACE_INFO("\nUsage:\n");
    TRACE_INFO("sample_subtitle [options] [file ..] [qam ..]\n");
    TRACE_INFO("    -f: path of the subtitle stream file\n");
    TRACE_INFO("    -t: frequency [symbol rate] [qam]\n");
    TRACE_INFO("example:\n");
    TRACE_INFO("    ./sample_subtitle -f Winter_Hill_Ch-66_1a_sub.ts\n");
    TRACE_INFO("    ./sample_subtitle -t 610 6875 64\n");
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
    HI_U8  u8SearchPmtTime = 10;
    pthread_t stInjectTSThread;


    CMD_LINE_ST stCmdLine = {0};

    s32Ret = parseCmdParam(argc, argv, &stCmdLine);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    (HI_VOID)HI_SYS_Init();
    (HI_VOID)HI_SYS_SetLogLevel(HI_ID_SUBT, HI_LOG_LEVEL_WARNING);
    HIADP_MCE_Exit();

    Subt_Data_Source_Param_S stParam;
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
        HI_ERR_SUBT("failed to StartDmx\n");
        goto SYS_DEINIT;
    }
    pthread_create(&stInjectTSThread, NULL, (void * (*)(void *))InjectTsTask, &stParam);


    /* Search table, including subtitle informations */

    PMT_COMPACT_TBL   *pSubTitleProgTbl = HI_NULL;

    HIADP_Search_Init();

    while (HI_SUCCESS != HIADP_Search_GetAllPmt(0, &pSubTitleProgTbl))
    {
        HI_WARN_SUBT("failed to HIADP_Search_GetAllPmt\n");
        usleep(100000);
    }

    HI_U8 u8ProgNo = SelectProgram(pSubTitleProgTbl);

    s32Ret = StartAVPlay(pSubTitleProgTbl, u8ProgNo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to StartAVPlay\n");
        goto EXIT;
    }

    while (u8SearchPmtTime &&
        0 == pSubTitleProgTbl->proginfo[u8ProgNo].u16SubtitlingNum &&
        0 == pSubTitleProgTbl->proginfo[u8ProgNo].stSCTESubtInfo.u16SCTESubtPID)
    {
        while (HI_SUCCESS != HIADP_Search_GetAllPmt(0, &pSubTitleProgTbl))
        {
            usleep(100000);
        }
        usleep(1000000);
        u8SearchPmtTime--;
    }

    s32Ret = StartSubtitle(pSubTitleProgTbl, u8ProgNo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to StartSubtitle\n");
        goto EXIT;
    }

    s32Ret = StartDataFilter(pSubTitleProgTbl, u8ProgNo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to StartDataFilter\n");
        goto EXIT;
    }

    (HI_VOID)CmdTask(pSubTitleProgTbl, u8ProgNo);

    pthread_join(stInjectTSThread, NULL);

EXIT:
    if (hUnfSubt)
    {
        (HI_VOID)HI_UNF_SUBT_Destroy(hUnfSubt);
        (HI_VOID)HI_UNF_SUBT_DeInit();
        hUnfSubt = 0;
    }
    if (hUnfSO)
    {
        (HI_VOID)HI_UNF_SO_Destroy(hUnfSO);
        (HI_VOID)HI_UNF_SO_DeInit();
        hUnfSO = 0;
    }
    if (hSubtOut)
    {
        (HI_VOID)Subt_Output_DeInit(hSubtOut);
        hSubtOut = 0;
    }

#ifdef HI_FRONTEND_SUPPORT
    if (stCmdLine.pszFileName == NULL)
    {
        HIADP_Tuner_DeInit();
    }
#endif

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

    if (hTrack)
    {
        (HI_VOID)HI_UNF_SND_DestroyTrack(hTrack);
    }

    (HI_VOID)HIADP_Search_FreeAllPmt(pSubTitleProgTbl);
    (HI_VOID)HIADP_Search_DeInit();

    if (hWindow)
    {
        (HI_VOID)HI_UNF_VO_DestroyWindow(hWindow);
        (HI_VOID)HIADP_VO_DeInit();
        hWindow = 0;
    }

    (HI_VOID)SUBT_Data_DeInit();

    (HI_VOID)HIADP_Disp_DeInit();

    (HI_VOID)HIADP_Snd_DeInit();

SYS_DEINIT:
    (HI_VOID)HI_SYS_DeInit();

    return HI_SUCCESS;
}


