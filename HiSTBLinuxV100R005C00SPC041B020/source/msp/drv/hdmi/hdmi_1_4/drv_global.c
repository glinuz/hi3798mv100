
#include <linux/kernel.h>
//include memset
#include <linux/interrupt.h>

#include "drv_global.h"
#include "drv_disp_ext.h"

#include "hi_unf_audio.h"
#include "hi_unf_hdmi.h"
#include "hi_drv_hdmi.h"
#include "hi_drv_module.h"

#include "si_hdmitx.h"
#include "si_edid.h"

#include "test_edid.h"

//#include "si_timer.h"

static HDMI_COMM_ATTR_S g_stHdmiCommParam;

static HDMI_CHN_ATTR_S  g_stHdmiChnParam[HI_UNF_HDMI_ID_BUTT];

static HI_UNF_EDID_BASE_INFO_S g_stEdidInfo[HI_UNF_HDMI_ID_BUTT];

static HDMI_PRIVATE_EDID_S g_stPriEdidInfo[HI_UNF_HDMI_ID_BUTT];

static HI_BOOL            bForceOutput = HI_FALSE;
static HI_BOOL            bForcePowerON = HI_FALSE;
static HI_BOOL            s_bSoftResume = HI_FALSE;

HI_VOID DRV_HDMI_SetSoftResume(HI_BOOL bSoftResume)
{
	s_bSoftResume = bSoftResume;
}

HI_BOOL DRV_HDMI_GetSoftResume(HI_VOID)
{
	return s_bSoftResume;
}
extern HI_DRV_DISP_FMT_E DispFormatAdjust(HI_DRV_DISP_FMT_E enEncFmt);


static HDMI_Test_EDID_S *EDID_List[] =
{
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST1) , EDID_BLOCK_TEST1},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST2) , EDID_BLOCK_TEST2},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST3) , EDID_BLOCK_TEST3},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST4) , EDID_BLOCK_TEST4},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST5) , EDID_BLOCK_TEST5},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST6) , EDID_BLOCK_TEST6},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST7) , EDID_BLOCK_TEST7},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST8) , EDID_BLOCK_TEST8},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST9) , EDID_BLOCK_TEST9},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST10), EDID_BLOCK_TEST10},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST11), EDID_BLOCK_TEST11},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST12), EDID_BLOCK_TEST12},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST13), EDID_BLOCK_TEST13},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST14), EDID_BLOCK_TEST14},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST15), EDID_BLOCK_TEST15},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST16), EDID_BLOCK_TEST16},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST17), EDID_BLOCK_TEST17},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST18), EDID_BLOCK_TEST18},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST19), EDID_BLOCK_TEST19},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST20), EDID_BLOCK_TEST20},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST21), EDID_BLOCK_TEST21},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST22), EDID_BLOCK_TEST22},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST23), EDID_BLOCK_TEST23},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST24), EDID_BLOCK_TEST24},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST25), EDID_BLOCK_TEST25},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST26), EDID_BLOCK_TEST26},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST27), EDID_BLOCK_TEST27},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST28), EDID_BLOCK_TEST28},
    &(HDMI_Test_EDID_S){sizeof(EDID_BLOCK_TEST29), EDID_BLOCK_TEST29},
    NULL
};


//extern DISP_EXPORT_FUNC_S *disp_func_ops;

HI_DRV_HDMI_AUDIO_CAPABILITY_S g_stHdmiOldAudio;

HI_U32 g_u32DDCDelayCount = 0x1e;

//HI_U8 g_u8ExtEdid[EDID_SIZE] = {0};
//HI_U32 g_u32ExtEdidLengh = 0;
HI_BOOL g_bExtEdid[HI_UNF_HDMI_ID_BUTT] = {HI_FALSE};
HDMI_EDID_S g_ExtEdid[HI_UNF_HDMI_ID_BUTT];

HI_DRV_HDMI_AUDIO_CAPABILITY_S *DRV_Get_OldAudioCap()
{
    return &g_stHdmiOldAudio;
}


HDMI_COMM_ATTR_S *DRV_Get_CommAttr()
{
    COM_INFO("Get_CommAttr \n");
    return &g_stHdmiCommParam;
}


void DRV_PrintCommAttr()
{
    HI_PRINT("g_stHdmiCommParam \n"
        "bOpenGreenChannel :%d \n"
        "bOpenedInBoot :%d \n"
        "kThreadTimerStop :%d \n"
        "enVidInMode :%d \n",
        g_stHdmiCommParam.bOpenMce2App,
        g_stHdmiCommParam.bOpenedInBoot,
        g_stHdmiCommParam.kThreadTimerStop,
        g_stHdmiCommParam.enVidInMode);
}

HI_S32 DRV_Get_IsMce2App(HI_VOID)
{
    return g_stHdmiCommParam.bOpenMce2App;
}

void DRV_Set_Mce2App(HI_BOOL bSmooth)
{
    COM_INFO("Set_GreenChannel bGreen : %d \n",bSmooth);
    g_stHdmiCommParam.bOpenMce2App = bSmooth;
}

HI_S32 DRV_Get_IsOpenedInBoot(HI_VOID)
{
    return g_stHdmiCommParam.bOpenedInBoot;
}

void DRV_Set_OpenedInBoot(HI_BOOL bOpened)
{
    COM_INFO("Set_OpenedInBoot bOpend : %d \n",bOpened);
    g_stHdmiCommParam.bOpenedInBoot = bOpened;
}

HI_S32 DRV_Get_IsThreadStoped(HI_VOID)
{
    return g_stHdmiCommParam.kThreadTimerStop;
}

void DRV_Set_ThreadStop(HI_BOOL bStop)
{
    COM_INFO("Set_ThreadStatus bStop : %d \n",bStop);
    g_stHdmiCommParam.kThreadTimerStop = bStop;
}

HI_UNF_HDMI_VIDEO_MODE_E DRV_Get_VIDMode(HI_VOID)
{
    return g_stHdmiCommParam.enVidInMode;
}

void DRV_Set_VIDMode(HI_UNF_HDMI_VIDEO_MODE_E enVInMode)
{
    g_stHdmiCommParam.enVidInMode = enVInMode;
}



HDMI_CHN_ATTR_S *DRV_Get_ChnAttr()
{
    return g_stHdmiChnParam;
}

HDMI_ATTR_S *DRV_Get_HDMIAttr(HI_UNF_HDMI_ID_E enHdmi)
{
    return &g_stHdmiChnParam[enHdmi].stHDMIAttr;
}

HI_U32 DRV_HDMI_SetDefaultAttr(HI_VOID)
{
    HI_DRV_DISP_FMT_E   enEncFmt = HI_DRV_DISP_FMT_1080i_50;
    HDMI_VIDEO_ATTR_S   *pstVidAttr = DRV_Get_VideoAttr(HI_UNF_HDMI_ID_0);
    HDMI_AUDIO_ATTR_S   *pstAudAttr = DRV_Get_AudioAttr(HI_UNF_HDMI_ID_0);
    HDMI_APP_ATTR_S     *pstAppAttr = DRV_Get_AppAttr(HI_UNF_HDMI_ID_0);
    DISP_EXPORT_FUNC_S  *disp_func_ops = HI_NULL;
    HI_S32              ret = HI_SUCCESS;

    memset(pstVidAttr,0,sizeof(HDMI_VIDEO_ATTR_S));
    memset(pstAudAttr,0,sizeof(HDMI_AUDIO_ATTR_S));
    memset(pstAppAttr,0,sizeof(HDMI_APP_ATTR_S));

    ret = HI_DRV_MODULE_GetFunction(HI_ID_DISP, (HI_VOID**)&disp_func_ops);
    if((NULL == disp_func_ops) || (ret != HI_SUCCESS))
    {
        COM_FATAL("can't get disp funcs!\n");
        //return HI_FAILURE;
    }

    // GetFormat need add 3d mode
    if(disp_func_ops && disp_func_ops->pfnDispGetFormat)
    {
        disp_func_ops->pfnDispGetFormat(HI_DRV_DISPLAY_1, &enEncFmt);
        enEncFmt = DispFormatAdjust(enEncFmt);
    }

    // if don't get disp fmt,then use default fmt
    pstVidAttr->enVideoFmt = enEncFmt;
    if ((enEncFmt < HI_DRV_DISP_FMT_861D_640X480_60) || DRV_Get_Is4KFmt(enEncFmt))
    {
        pstAppAttr->enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
    }
    else
    {
        pstAppAttr->enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
    }

    pstVidAttr->b3DEnable = HI_FALSE;
    pstVidAttr->u83DParam = HI_UNF_EDID_3D_BUTT;

    pstAudAttr->enBitDepth = HI_UNF_BIT_DEPTH_16;
    pstAudAttr->enSoundIntf = HDMI_AUDIO_INTERFACE_I2S;
    pstAudAttr->enSampleRate = HI_UNF_SAMPLE_RATE_48K;
    pstAudAttr->u32Channels = 2;
    pstAudAttr->bIsMultiChannel = HI_FALSE;

    return HI_SUCCESS;
}

HDMI_APP_ATTR_S   *DRV_Get_AppAttr(HI_UNF_HDMI_ID_E enHdmi)
{
    return &g_stHdmiChnParam[enHdmi].stHDMIAttr.stAppAttr;
}

HDMI_VIDEO_ATTR_S *DRV_Get_VideoAttr(HI_UNF_HDMI_ID_E enHdmi)
{
    return &g_stHdmiChnParam[enHdmi].stHDMIAttr.stVideoAttr;
}

HDMI_AUDIO_ATTR_S *DRV_Get_AudioAttr(HI_UNF_HDMI_ID_E enHdmi)
{
    return &g_stHdmiChnParam[enHdmi].stHDMIAttr.stAudioAttr;
}

HI_UNF_EDID_BASE_INFO_S *DRV_Get_SinkCap(HI_UNF_HDMI_ID_E enHdmi)
{
    return &g_stEdidInfo[enHdmi];
}


HI_BOOL DRV_Get_IsNeedForceUpdate(HI_UNF_HDMI_ID_E enHdmi)
{
    COM_INFO("Get g_stHdmiChnParam[%d].ForceUpdateFlag %d\n",enHdmi,g_stHdmiChnParam[enHdmi].ForceUpdateFlag);
    return g_stHdmiChnParam[enHdmi].ForceUpdateFlag;
}


void DRV_Set_ForceUpdateFlag(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bupdate)
{
    COM_INFO("Set g_stHdmiChnParam[%d].ForceUpdateFlag %d\n",enHdmi,g_stHdmiChnParam[enHdmi].ForceUpdateFlag);
    g_stHdmiChnParam[enHdmi].ForceUpdateFlag = bupdate;
}

HI_BOOL DRV_Get_IsNeedPartUpdate(HI_UNF_HDMI_ID_E enHdmi)
{
    return g_stHdmiChnParam[enHdmi].partUpdateFlag;
}

void DRV_Set_PartUpdateFlag(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bupdate)
{
    g_stHdmiChnParam[enHdmi].partUpdateFlag = bupdate;
}



HDMI_PROC_EVENT_S *DRV_Get_EventList(HI_UNF_HDMI_ID_E enHdmi)
{
    //eventlist is not associateed to channel,so force return  HI_UNF_HDMI_ID_0
    return g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList;
}

HI_UNF_HDMI_CEC_STATUS_S *DRV_Get_CecStatus(HI_UNF_HDMI_ID_E enHdmi)
{
    return &g_stHdmiChnParam[enHdmi].stCECStatus;
}

HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *DRV_Get_AviInfoFrm(HI_UNF_HDMI_ID_E enHdmi)
{
    return &g_stHdmiChnParam[enHdmi].stAVIInfoFrame;
}

HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *DRV_Get_AudInfoFrm(HI_UNF_HDMI_ID_E enHdmi)
{
    return &g_stHdmiChnParam[enHdmi].stAUDInfoFrame;
}

HI_BOOL DRV_Get_IsChnOpened(HI_UNF_HDMI_ID_E enHdmi)
{
    return g_stHdmiChnParam[enHdmi].bOpen;
}

void DRV_Set_ChnOpen(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bChnOpen)
{
    g_stHdmiChnParam[enHdmi].bOpen = bChnOpen;
}

HI_BOOL DRV_Get_IsChnStart(HI_UNF_HDMI_ID_E enHdmi)
{
    return g_stHdmiChnParam[enHdmi].bStart;
}

void DRV_Set_ChnStart(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bChnStart)
{
    g_stHdmiChnParam[enHdmi].bStart = bChnStart;
}

HI_BOOL DRV_Get_IsCECEnable(HI_UNF_HDMI_ID_E enHdmi)
{
    return g_stHdmiChnParam[enHdmi].bCECEnable;
}

void DRV_Set_CECEnable(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bCecEnable)
{
    g_stHdmiChnParam[enHdmi].bCECEnable = bCecEnable;
}

HI_BOOL DRV_Get_IsCECStart(HI_UNF_HDMI_ID_E enHdmi)
{
    return g_stHdmiChnParam[enHdmi].bCECStart;
}

void DRV_Set_CECStart(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bCecStart)
{
    g_stHdmiChnParam[enHdmi].bCECStart = bCecStart;
}

HI_BOOL DRV_Get_IsValidSinkCap(HI_UNF_HDMI_ID_E enHdmi)
{
    return g_stHdmiChnParam[enHdmi].bValidSinkCap;
}

void DRV_Set_SinkCapValid(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bSinkValid)
{
    g_stHdmiChnParam[enHdmi].bValidSinkCap = bSinkValid;
}

void DRV_Set_DefaultOutputMode(HI_UNF_HDMI_ID_E enHdmi,HI_UNF_HDMI_DEFAULT_ACTION_E enDefaultMode)
{
    g_stHdmiChnParam[enHdmi].enDefaultMode = enDefaultMode;
}


HI_UNF_HDMI_DEFAULT_ACTION_E DRV_Get_DefaultOutputMode(HI_UNF_HDMI_ID_E enHdmi)
{
    return g_stHdmiChnParam[enHdmi].enDefaultMode;
}

void DRV_Set_DDCSpeed(HI_U32 delayCount)
{
    g_u32DDCDelayCount = delayCount;
}

HI_U32 DRV_Get_DDCSpeed(void)
{
    return g_u32DDCDelayCount;
}


HDMI_EDID_S *DRV_Get_UserEdid(HI_UNF_HDMI_ID_E enHdmi)
{
    return &g_ExtEdid[enHdmi];
}

void DRV_Set_UserEdid(HI_UNF_HDMI_ID_E enHdmi,HDMI_EDID_S *pEDID)
{
    //only memset one ExtEdid,NOt All
    memset(&g_ExtEdid[enHdmi],0,sizeof(HDMI_EDID_S));

    memcpy(&g_ExtEdid[enHdmi],pEDID,sizeof(HDMI_EDID_S));
}

HI_BOOL DRV_Get_IsUserEdid(HI_UNF_HDMI_ID_E enHdmi)
{
    return g_bExtEdid[enHdmi];
}

void DRV_Set_UserEdidMode(HI_UNF_HDMI_ID_E enHdmi,HI_BOOL bUserEdid)
{
    g_bExtEdid[enHdmi] = bUserEdid;
}


HI_U32 DRV_Get_DebugEdidNum(void)
{
    HI_U32 index;

    //calc edid list number
    for (index = 0; EDID_List[index] != NULL; index++)
    {
        continue;
    }

    COM_INFO("%d EDID in list \n",index);
    return index;
}

//param start frome 1,not 0
HDMI_Test_EDID_S *DRV_Get_DebugEdid(HI_U32 u32Num)
{
    return EDID_List[u32Num-1];
}


VSDB_MODE_E DRV_Get_VSDBMode(HI_UNF_HDMI_ID_E enHdmi)
{
    return g_stHdmiChnParam[enHdmi].enVSDBMode;
}


void DRV_Set_VSDBMode(HI_UNF_HDMI_ID_E enHdmi,VSDB_MODE_E enVSDBMode)
{
    g_stHdmiChnParam[enHdmi].enVSDBMode = enVSDBMode;
}

//HDMI_VIDEO_ATTR_S   *pstVidAttr = DRV_Get_VideoAttr(enHdmi);
HI_BOOL DRV_Get_Is4KFmt(HI_DRV_DISP_FMT_E enFmt)
{
    if ((enFmt == HI_DRV_DISP_FMT_3840X2160_24)
     || (enFmt == HI_DRV_DISP_FMT_3840X2160_25)
     || (enFmt == HI_DRV_DISP_FMT_3840X2160_30)
     || (enFmt == HI_DRV_DISP_FMT_4096X2160_24)
     || (enFmt == HI_DRV_DISP_FMT_3840X2160_23_976)
     || (enFmt == HI_DRV_DISP_FMT_3840X2160_29_97))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

// 640*480 is CEA861 Fmt,so not In this func
HI_BOOL DRV_Get_IsLCDFmt(HI_DRV_DISP_FMT_E enFmt)
{
    if((enFmt == HI_DRV_DISP_FMT_VESA_800X600_60)
     || (enFmt == HI_DRV_DISP_FMT_VESA_1024X768_60)
     || (enFmt == HI_DRV_DISP_FMT_VESA_1280X720_60)

     || (enFmt == HI_DRV_DISP_FMT_VESA_1280X800_60)
     || (enFmt == HI_DRV_DISP_FMT_VESA_1280X1024_60)
     || (enFmt == HI_DRV_DISP_FMT_VESA_1360X768_60)

     || (enFmt == HI_DRV_DISP_FMT_VESA_1366X768_60)
     || (enFmt == HI_DRV_DISP_FMT_VESA_1400X1050_60)
     || (enFmt == HI_DRV_DISP_FMT_VESA_1440X900_60)

     || (enFmt == HI_DRV_DISP_FMT_VESA_1440X900_60_RB)
     || (enFmt == HI_DRV_DISP_FMT_VESA_1600X900_60_RB)
     || (enFmt == HI_DRV_DISP_FMT_VESA_1600X1200_60)

     || (enFmt == HI_DRV_DISP_FMT_VESA_1680X1050_60)
     || (enFmt == HI_DRV_DISP_FMT_VESA_1680X1050_60_RB)
     || (enFmt == HI_DRV_DISP_FMT_VESA_1920X1080_60)

     || (enFmt == HI_DRV_DISP_FMT_VESA_1920X1200_60)
     || (enFmt == HI_DRV_DISP_FMT_VESA_1920X1440_60)
     || (enFmt == HI_DRV_DISP_FMT_VESA_1680X1050_60_RB)

     || (enFmt == HI_DRV_DISP_FMT_VESA_2048X1152_60)
     || (enFmt == HI_DRV_DISP_FMT_VESA_2560X1440_60_RB)
     || (enFmt == HI_DRV_DISP_FMT_VESA_2560X1600_60_RB)

     || (enFmt == HI_DRV_DISP_FMT_CUSTOM))
    {
        return HI_TRUE;
    }
    return HI_FALSE;
}

HI_BOOL DRV_Get_IsPixelRepeatFmt(HI_DRV_DISP_FMT_E enFmt)
{
    if ((enFmt == HI_DRV_DISP_FMT_PAL)
       || (enFmt == HI_DRV_DISP_FMT_PAL_B)
       || (enFmt == HI_DRV_DISP_FMT_PAL_B1)
       || (enFmt == HI_DRV_DISP_FMT_PAL_D)
       || (enFmt == HI_DRV_DISP_FMT_PAL_D1)
       || (enFmt == HI_DRV_DISP_FMT_PAL_G)
       || (enFmt == HI_DRV_DISP_FMT_PAL_H)
       || (enFmt == HI_DRV_DISP_FMT_PAL_K)
       || (enFmt == HI_DRV_DISP_FMT_PAL_I)
       || (enFmt == HI_DRV_DISP_FMT_PAL_M)
       || (enFmt == HI_DRV_DISP_FMT_PAL_N)
       || (enFmt == HI_DRV_DISP_FMT_PAL_Nc)
       || (enFmt == HI_DRV_DISP_FMT_PAL_60)
       || (enFmt == HI_DRV_DISP_FMT_1440x576i_50)
       || (enFmt == HI_DRV_DISP_FMT_SECAM_SIN)
       || (enFmt == HI_DRV_DISP_FMT_SECAM_COS)
       || (enFmt == HI_DRV_DISP_FMT_SECAM_L)
       || (enFmt == HI_DRV_DISP_FMT_SECAM_B)
       || (enFmt == HI_DRV_DISP_FMT_SECAM_G)
       || (enFmt == HI_DRV_DISP_FMT_SECAM_D)
       || (enFmt == HI_DRV_DISP_FMT_SECAM_K)
       || (enFmt == HI_DRV_DISP_FMT_SECAM_H)
       || (enFmt == HI_DRV_DISP_FMT_NTSC)
       || (enFmt == HI_DRV_DISP_FMT_NTSC_J)
       || (enFmt == HI_DRV_DISP_FMT_1440x480i_60)
       || (enFmt == HI_DRV_DISP_FMT_NTSC_443))
    {
        return HI_TRUE;
    }
    return HI_FALSE;
}

HDMI_PRIVATE_EDID_S *DRV_Get_PriSinkCap(HI_UNF_HDMI_ID_E enHdmi)
{
    return &g_stPriEdidInfo[enHdmi];
}

HI_BOOL DRV_Get_IsForceOutput(void)
{
    //return g_stHdmiChnParam[enHdmi].bForceOutput;
    return bForceOutput;
}

void DRV_Set_ForceOutputMode(HI_BOOL bForce)
{
    //g_stHdmiChnParam[enHdmi].bForceOutput = bForce;
    bForceOutput = bForce;
}


HI_BOOL DRV_Get_ForcePowerState(void)
{
    //return g_stHdmiChnParam[enHdmi].bForceOutput;
    return bForcePowerON;
}

void DRV_Set_ForcePowerState(HI_BOOL bForce)
{
    //g_stHdmiChnParam[enHdmi].bForceOutput = bForce;
    bForcePowerON = bForce;
}

extern HI_U32 u32PixelClk[];

HI_U32 DRV_Calculate_TMDSCLK(HI_UNF_ENC_FMT_E fmt, HI_UNF_HDMI_DEEP_COLOR_E enDeepColorMode)
{
    HI_U32 u32PClk;

    if(fmt >= HI_UNF_ENC_FMT_BUTT)
    {
        COM_ERR("DRV_Calculate_TMDSCLK : wrong Format\n");
        return HI_FAILURE;
    }
    
    u32PClk = u32PixelClk[fmt];
    COM_INFO("DRV_Calculate_TMDSCLK : Pixel Clock : %u, fmt : %d, enDeepColorMode : %d\n", u32PClk, fmt, enDeepColorMode);
    
    switch(enDeepColorMode)
    {
        case HI_UNF_HDMI_DEEP_COLOR_OFF     : 
        case HI_UNF_HDMI_DEEP_COLOR_24BIT   : 
            u32PClk = u32PClk * 1;             //Ratio 1
            break;
        case HI_UNF_HDMI_DEEP_COLOR_30BIT   : 
            u32PClk = (u32PClk * 4) / 5 ;      //Ratio 1.25
            break;
        case HI_UNF_HDMI_DEEP_COLOR_36BIT   : 
            u32PClk =  (u32PClk * 3) / 2 ;     //Ratio 1.5
            break;  

        default:
            u32PClk = u32PClk * 1;
            break;
    }
    
    return u32PClk;       
}

HI_UNF_ENC_FMT_E DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_E SrcFmt)
{
    HI_UNF_ENC_FMT_E UnfFmt = HI_UNF_ENC_FMT_BUTT;
    
    switch(SrcFmt)
    {
        case HI_DRV_DISP_FMT_1080P_60 :
            UnfFmt = HI_UNF_ENC_FMT_1080P_60;
            break;
        case HI_DRV_DISP_FMT_1080P_50 :
            UnfFmt = HI_UNF_ENC_FMT_1080P_50;
            break;
        case HI_DRV_DISP_FMT_1080P_30 :
            UnfFmt = HI_UNF_ENC_FMT_1080P_30;
            break;
        case HI_DRV_DISP_FMT_1080P_25 :
            UnfFmt = HI_UNF_ENC_FMT_1080P_25;
            break;
        case HI_DRV_DISP_FMT_1080P_24 :
        case HI_DRV_DISP_FMT_1080P_24_FP:
            UnfFmt = HI_UNF_ENC_FMT_1080P_24;
            break;
        case HI_DRV_DISP_FMT_1080i_60 :
            UnfFmt = HI_UNF_ENC_FMT_1080i_60;
            break;
        case HI_DRV_DISP_FMT_1080i_50 :
            UnfFmt = HI_UNF_ENC_FMT_1080i_50;
            break;
        case HI_DRV_DISP_FMT_720P_60 :
        case HI_DRV_DISP_FMT_720P_60_FP:
            UnfFmt = HI_UNF_ENC_FMT_720P_60;
            break;
        case HI_DRV_DISP_FMT_720P_50 :
        case HI_DRV_DISP_FMT_720P_50_FP:
            UnfFmt = HI_UNF_ENC_FMT_720P_50;
            break;
        case HI_DRV_DISP_FMT_576P_50 :
            UnfFmt = HI_UNF_ENC_FMT_576P_50;
            break;
        case HI_DRV_DISP_FMT_480P_60 :
            UnfFmt = HI_UNF_ENC_FMT_480P_60;
            break;
        case HI_DRV_DISP_FMT_PAL:
        case HI_DRV_DISP_FMT_PAL_B:
        case HI_DRV_DISP_FMT_PAL_B1:
        case HI_DRV_DISP_FMT_PAL_D:
        case HI_DRV_DISP_FMT_PAL_D1:
        case HI_DRV_DISP_FMT_PAL_G:
        case HI_DRV_DISP_FMT_PAL_H:
        case HI_DRV_DISP_FMT_PAL_K:
        case HI_DRV_DISP_FMT_PAL_I:
        case HI_DRV_DISP_FMT_PAL_M:
        case HI_DRV_DISP_FMT_PAL_N:
        case HI_DRV_DISP_FMT_PAL_Nc:
        case HI_DRV_DISP_FMT_PAL_60:
        case HI_DRV_DISP_FMT_1440x576i_50:

        case HI_DRV_DISP_FMT_SECAM_SIN:
        case HI_DRV_DISP_FMT_SECAM_COS:
        case HI_DRV_DISP_FMT_SECAM_L:
        case HI_DRV_DISP_FMT_SECAM_B:
        case HI_DRV_DISP_FMT_SECAM_G:
        case HI_DRV_DISP_FMT_SECAM_D:
        case HI_DRV_DISP_FMT_SECAM_K:
        case HI_DRV_DISP_FMT_SECAM_H:
            UnfFmt = HI_UNF_ENC_FMT_PAL;
            break;
        case HI_DRV_DISP_FMT_NTSC :
        case HI_DRV_DISP_FMT_NTSC_J :
        case HI_DRV_DISP_FMT_NTSC_443 :
        case HI_DRV_DISP_FMT_1440x480i_60:
            UnfFmt = HI_UNF_ENC_FMT_NTSC;
            break;
        case HI_DRV_DISP_FMT_861D_640X480_60 :
            UnfFmt = HI_UNF_ENC_FMT_861D_640X480_60;
            break;
        case HI_DRV_DISP_FMT_VESA_800X600_60 :
            UnfFmt = HI_UNF_ENC_FMT_VESA_800X600_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1024X768_60 :
            UnfFmt = HI_UNF_ENC_FMT_VESA_1024X768_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1280X720_60 :
            UnfFmt = HI_UNF_ENC_FMT_VESA_1280X720_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1280X800_60 :
            UnfFmt = HI_UNF_ENC_FMT_VESA_1280X800_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1280X1024_60 :
            UnfFmt = HI_UNF_ENC_FMT_VESA_1280X1024_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1360X768_60 :
            UnfFmt = HI_UNF_ENC_FMT_VESA_1360X768_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1366X768_60 :
            UnfFmt = HI_UNF_ENC_FMT_VESA_1366X768_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1400X1050_60:
            UnfFmt = HI_UNF_ENC_FMT_VESA_1400X1050_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1440X900_60:
            UnfFmt = HI_UNF_ENC_FMT_VESA_1440X900_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1440X900_60_RB:
            UnfFmt = HI_UNF_ENC_FMT_VESA_1440X900_60_RB;
            break;
        case HI_DRV_DISP_FMT_VESA_1600X900_60_RB:
            UnfFmt = HI_UNF_ENC_FMT_VESA_1600X900_60_RB;
            break;
        case HI_DRV_DISP_FMT_VESA_1600X1200_60:
            UnfFmt = HI_UNF_ENC_FMT_VESA_1600X1200_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1680X1050_60:
            UnfFmt = HI_UNF_ENC_FMT_VESA_1680X1050_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1920X1080_60:
            UnfFmt = HI_UNF_ENC_FMT_VESA_1920X1080_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1920X1200_60:
            UnfFmt = HI_UNF_ENC_FMT_VESA_1920X1200_60;
            break;
        case HI_DRV_DISP_FMT_VESA_2048X1152_60:
            UnfFmt = HI_UNF_ENC_FMT_VESA_2048X1152_60;
            break;
        case HI_DRV_DISP_FMT_VESA_2560X1440_60_RB:
            UnfFmt = HI_UNF_ENC_FMT_VESA_2560X1440_60_RB;
            break;
        case HI_DRV_DISP_FMT_VESA_2560X1600_60_RB:
            UnfFmt = HI_UNF_ENC_FMT_VESA_2560X1600_60_RB;
            break;
        case HI_DRV_DISP_FMT_3840X2160_24:
            UnfFmt = HI_UNF_ENC_FMT_3840X2160_24;
            break;
        case HI_DRV_DISP_FMT_3840X2160_25:
            UnfFmt = HI_UNF_ENC_FMT_3840X2160_25;
            break;
        case HI_DRV_DISP_FMT_3840X2160_30:
            UnfFmt = HI_UNF_ENC_FMT_3840X2160_30;
            break;
        case HI_DRV_DISP_FMT_4096X2160_24:
            UnfFmt = HI_UNF_ENC_FMT_4096X2160_24;
            break;
        case HI_DRV_DISP_FMT_3840X2160_23_976 :
            UnfFmt = HI_UNF_ENC_FMT_3840X2160_23_976;
            break;            
        case HI_DRV_DISP_FMT_3840X2160_29_97:
            UnfFmt = HI_UNF_ENC_FMT_3840X2160_29_97;
            break;
        case HI_DRV_DISP_FMT_720P_59_94:
            UnfFmt = HI_UNF_ENC_FMT_720P_59_94;
            break;
        case HI_DRV_DISP_FMT_1080P_59_94:
            UnfFmt = HI_UNF_ENC_FMT_1080P_59_94;
            break;
        case HI_DRV_DISP_FMT_1080P_29_97:
            UnfFmt = HI_UNF_ENC_FMT_1080P_29_97;
            break;
        case HI_DRV_DISP_FMT_1080P_23_976:
            UnfFmt = HI_UNF_ENC_FMT_1080P_23_976;
            break;
        case HI_DRV_DISP_FMT_1080i_59_94:
            UnfFmt = HI_UNF_ENC_FMT_1080i_59_94;
            break;
        //ADD
        case HI_DRV_DISP_FMT_CUSTOM:
        case HI_DRV_DISP_FMT_BUTT:
        default:
            UnfFmt = HI_UNF_ENC_FMT_BUTT;
            break;
    }
    return UnfFmt;
}

HI_DRV_DISP_FMT_E DRV_HDMI_Unf2DispFmt(HI_UNF_ENC_FMT_E SrcFmt)
{
    HI_DRV_DISP_FMT_E dstFmt = HI_DRV_DISP_FMT_BUTT;
    switch(SrcFmt)
    {
        case HI_UNF_ENC_FMT_1080P_60 :
            dstFmt = HI_DRV_DISP_FMT_1080P_60;
            break;
        case HI_UNF_ENC_FMT_1080P_50 :
            dstFmt = HI_DRV_DISP_FMT_1080P_50;
            break;
        case HI_UNF_ENC_FMT_1080P_30 :
            dstFmt = HI_DRV_DISP_FMT_1080P_30;
            break;
        case HI_UNF_ENC_FMT_1080P_25 :
            dstFmt = HI_DRV_DISP_FMT_1080P_25;
            break;
        case HI_UNF_ENC_FMT_1080P_24 :
            dstFmt = HI_DRV_DISP_FMT_1080P_24;
            break;
        case HI_UNF_ENC_FMT_1080i_60 :
            dstFmt = HI_DRV_DISP_FMT_1080i_60;
            break;
        case HI_UNF_ENC_FMT_1080i_50 :
            dstFmt = HI_DRV_DISP_FMT_1080i_50;
            break;
        case HI_UNF_ENC_FMT_720P_60 :
        case HI_UNF_ENC_FMT_720P_60_FRAME_PACKING:
            dstFmt = HI_DRV_DISP_FMT_720P_60;
            break;
        case HI_UNF_ENC_FMT_720P_50 :
        case HI_UNF_ENC_FMT_720P_50_FRAME_PACKING:
            dstFmt = HI_DRV_DISP_FMT_720P_50;
            break;
        case HI_UNF_ENC_FMT_576P_50 :
            dstFmt = HI_DRV_DISP_FMT_576P_50;
            break;
        case HI_UNF_ENC_FMT_480P_60 :
            dstFmt = HI_DRV_DISP_FMT_480P_60;
            break;
        case HI_UNF_ENC_FMT_PAL:
        case HI_UNF_ENC_FMT_PAL_N:
        case HI_UNF_ENC_FMT_PAL_Nc:

        case HI_UNF_ENC_FMT_SECAM_SIN:
        case HI_UNF_ENC_FMT_SECAM_COS:
            dstFmt = HI_DRV_DISP_FMT_PAL;
            break;
        case HI_UNF_ENC_FMT_NTSC :
        case HI_UNF_ENC_FMT_NTSC_J :            
        case HI_UNF_ENC_FMT_NTSC_PAL_M:
            dstFmt = HI_DRV_DISP_FMT_NTSC;
            break;
        case HI_UNF_ENC_FMT_861D_640X480_60 :
            dstFmt = HI_DRV_DISP_FMT_861D_640X480_60;
            break;
        case HI_UNF_ENC_FMT_VESA_800X600_60 :
            dstFmt = HI_DRV_DISP_FMT_VESA_800X600_60;
            break;
        case HI_UNF_ENC_FMT_VESA_1024X768_60 :
            dstFmt = HI_DRV_DISP_FMT_VESA_1024X768_60;
            break;
        case HI_UNF_ENC_FMT_VESA_1280X720_60 :
            dstFmt = HI_DRV_DISP_FMT_VESA_1280X720_60;
            break;
        case HI_UNF_ENC_FMT_VESA_1280X800_60 :
            dstFmt = HI_DRV_DISP_FMT_VESA_1280X800_60;
            break;
        case HI_UNF_ENC_FMT_VESA_1280X1024_60 :
            dstFmt = HI_DRV_DISP_FMT_VESA_1280X1024_60;
            break;
        case HI_UNF_ENC_FMT_VESA_1360X768_60 :
            dstFmt = HI_DRV_DISP_FMT_VESA_1360X768_60;
            break;
        case HI_UNF_ENC_FMT_VESA_1366X768_60 :
            dstFmt = HI_DRV_DISP_FMT_VESA_1366X768_60;
            break;
        case HI_UNF_ENC_FMT_VESA_1400X1050_60:
            dstFmt = HI_DRV_DISP_FMT_VESA_1400X1050_60;
            break;
        case HI_UNF_ENC_FMT_VESA_1440X900_60:
            dstFmt = HI_DRV_DISP_FMT_VESA_1440X900_60;
            break;
        case HI_UNF_ENC_FMT_VESA_1440X900_60_RB:
            dstFmt = HI_DRV_DISP_FMT_VESA_1440X900_60_RB;
            break;
        case HI_UNF_ENC_FMT_VESA_1600X900_60_RB:
            dstFmt = HI_DRV_DISP_FMT_VESA_1600X900_60_RB;
            break;
        case HI_UNF_ENC_FMT_VESA_1600X1200_60:
            dstFmt = HI_DRV_DISP_FMT_VESA_1600X1200_60;
            break;
        case HI_UNF_ENC_FMT_VESA_1680X1050_60:
            dstFmt = HI_DRV_DISP_FMT_VESA_1680X1050_60;
            break;
        case HI_UNF_ENC_FMT_VESA_1920X1080_60:
            dstFmt = HI_DRV_DISP_FMT_VESA_1920X1080_60;
            break;
        case HI_UNF_ENC_FMT_VESA_1920X1200_60:
            dstFmt = HI_DRV_DISP_FMT_VESA_1920X1200_60;
            break;
        case HI_UNF_ENC_FMT_VESA_2048X1152_60:
            dstFmt = HI_DRV_DISP_FMT_VESA_2048X1152_60;
            break;
        case HI_UNF_ENC_FMT_VESA_2560X1440_60_RB:
            dstFmt = HI_DRV_DISP_FMT_VESA_2560X1440_60_RB;
            break;
        case HI_UNF_ENC_FMT_VESA_2560X1600_60_RB:
            dstFmt = HI_DRV_DISP_FMT_VESA_2560X1600_60_RB;
            break;
        case HI_UNF_ENC_FMT_3840X2160_24:
            dstFmt = HI_DRV_DISP_FMT_3840X2160_24;
            break;
        case HI_UNF_ENC_FMT_3840X2160_25:
            dstFmt = HI_DRV_DISP_FMT_3840X2160_25;
            break;
        case HI_UNF_ENC_FMT_3840X2160_30:
            dstFmt = HI_DRV_DISP_FMT_3840X2160_30;
            break;
        case HI_UNF_ENC_FMT_4096X2160_24:
            dstFmt = HI_DRV_DISP_FMT_4096X2160_24;
            break;

        case HI_UNF_ENC_FMT_3840X2160_23_976:
            dstFmt = HI_DRV_DISP_FMT_3840X2160_23_976;
            break;            
        case HI_UNF_ENC_FMT_3840X2160_29_97:
            dstFmt = HI_DRV_DISP_FMT_3840X2160_29_97;
            break;
        case HI_UNF_ENC_FMT_720P_59_94:
            dstFmt = HI_DRV_DISP_FMT_720P_59_94;
            break;
        case HI_UNF_ENC_FMT_1080P_59_94:
            dstFmt = HI_DRV_DISP_FMT_1080P_59_94;
            break;
        case HI_UNF_ENC_FMT_1080P_29_97:
            dstFmt = HI_DRV_DISP_FMT_1080P_29_97;
            break;
        case HI_UNF_ENC_FMT_1080P_23_976:
            dstFmt = HI_DRV_DISP_FMT_1080P_23_976;
            break;
        case HI_UNF_ENC_FMT_1080i_59_94:
            dstFmt = HI_DRV_DISP_FMT_1080i_59_94;
            break;
        default:
            dstFmt = HI_DRV_DISP_FMT_BUTT;
            break;
    }
    
    return dstFmt;
}

