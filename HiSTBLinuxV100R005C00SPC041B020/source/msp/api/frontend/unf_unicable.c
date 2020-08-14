/******************************************************************************

  Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name      : unf_unicable.c
Version         : Initial Draft
Author          : Hisilicon multimedia Software group
Created         : 2014/06/20
Last Modified   :
Description     :
Function List   :
History         :
 ******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_mpi_mem.h"
#include "drv_tuner_ioctl.h"
#include "hi_drv_struct.h"


//#define UNICABLE_UB_OCCUPIED_CHECK

/** BEGIN: Added by Howie, 2011/1/5  修改原因: 增加全频点扫描UB功能 */

/** Step size for full band scan. */
#define SCR_D_SCAN_FREQ_STEP           (4)

#define SCR_D_SCAN_FREQ_STEP_MIN        (2)

#define SCR_USER_BAND_NUMBERS               (8)

#define SCR_D_MAX_USER_BAND             (8)

/** Start frequency for full band scan. */
#define SCR_D_SCAN_FREQ_START          (950)

/** End frequency for full band scan. */
#define SCR_D_SCAN_FREQ_END            (2150)

/** Scan points for full band scan. */
#define SCR_D_SCAN_POINTS              (((SCR_D_SCAN_FREQ_END)-(SCR_D_SCAN_FREQ_START))/(SCR_D_SCAN_FREQ_STEP))

#define MIN_AGC_CHANGED_BETWEEN_ON_AND_OFF  600//400

#define MIN_DISTANCE_BETWEEN_UB_MHZ         80

/** Max number of points maybe UBs after scan full band. */
#define SCR_D_MAX_POINTS_MAYBE_USER_BAND    (160)

#define HON_D_FREQ_OFFSET_NO            (20)      //unic MHz

#define FRAMING_BYTE (0)
#define ADDRESS_BYTE (1)
#define COMMAND_BYTE (2)
#define DATA_BYTE_0 (3)
#define DATA_BYTE_1 (4)
#define DATA_BYTE_2 (5)


#define FORMAT_DISEQC_CMD_VALUE(a, F, A, C, aD, L) \
    { \
        int i; \
        a[FRAMING_BYTE] = F; \
        a[ADDRESS_BYTE] = A; \
        a[COMMAND_BYTE] = C; \
        for (i = 0; i < L; i++){  a[DATA_BYTE_0 + i] = ((HI_U8*)aD)[i]; } \
    }

#define ABS(val)  (((val)<0)? (0 - (val)): (val))

typedef enum
{
    TONE_SEARCH_UB_NOT_ASSIGNED,
    TONE_SEARCH_UB_ASSIGNED,
    TONE_SEARCH_UB_NO_TONE,
    TONE_SEARCH_UB_ACCEPT_ERROR
}TONE_SEARCH_UB_ACCEPT_E;

typedef enum
{
    UB_NOT_ASSIGNED,
    UB_ASSIGNED,
    UB_CHECK_BUTT
}UB_STATUS_E;

typedef struct
{
    HI_S32          s32Freq;
    HI_S32          s32Agc;
} SCR_TONE_S;

typedef struct
{
    HI_U32                      u32UBIndex;
    HI_U32                      u32CurrCentralFreq;     //unit MHz
    HI_U32                      u32TotalUBNumber;
    HI_UNF_TUNER_SCR_UB_S       u32UBInfo[SCR_USER_BAND_NUMBERS];
}UNICABLE_INFO_S;

//static SCR_TONE_S s_sScanToneList[SCR_D_MAX_POINTS_MAYBE_USER_BAND];

static UNICABLE_INFO_S s_SysUnicable;

static HI_U32 tsResultAgc = 0xFFFFFFFF;
static HI_U32 tsAvgToneCount  = 0;
static HI_U32 tsAvgToneAgc    = 0;
static HI_U32 g_u32UnicableStopping = 0;

static int32_t ScrPresetValue[SCR_D_MAX_USER_BAND] =
{
    0x042,
    0x084,
    0x0c6,
    0x108,
    0x14a,
    0x18c,
    0x1ce,
    0x210
};

static int32_t ScrLastValue[SCR_D_MAX_USER_BAND] =
{
    0x042,
    0x084,
    0x0c6,
    0x108,
    0x14a,
    0x18c,
    0x1ce,
    0x210
};

extern HI_S32 UNICABLE_DISEQC_SendRecvMessage(HI_U32 u32TunerId,
                                           const HI_UNF_TUNER_DISEQC_SENDMSG_S * pstSendMsg,
                                           HI_UNF_TUNER_DISEQC_RECVMSG_S * pstRecvMsg);
extern HI_S32 TUNER_Get_Agc(HI_U32 u32TunerId,HI_S32 s32CenterFreq, HI_S32 *ps32Agc);
extern HI_UNF_TUNER_SWITCH_TONEBURST_E TUNER_DISEQC_GetToneBurstStatus(HI_U32 u32TunerId);

/**
uint8_t ucScrIndex: range[0-3]
*/
static HI_U32  Scr_GetCenterFreq(HI_U32 ucScrIndex)
{
    return (HI_U32)s_SysUnicable.u32UBInfo[s_SysUnicable.u32UBIndex].s32CenterFreq;

}

static int hi3136_SearchTone(HI_U32 u32TunerId,HI_U32 CenterFreq,HI_U32 *pu32ToneTrue, HI_U32 *peakAgc)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32PeakAgc;
    //HI_U32 u32ToneTrue = 0;

    //HI_INFO_TUNER("\n center_freq is %d. \n", CenterFreq);

    if ((CenterFreq < SCR_D_SCAN_FREQ_START) || (CenterFreq > SCR_D_SCAN_FREQ_END))
    {
       HI_ERR_TUNER("start_freq_hz out of range\n");
       return HI_FAILURE;
    }

    s32Ret = TUNER_Get_Agc(u32TunerId,CenterFreq,(HI_S32*)&u32PeakAgc);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("TUNER_Get_Agc fail.\n");
        return HI_FAILURE;
    }

    tsResultAgc = u32PeakAgc;
    *peakAgc = u32PeakAgc;

    /*if(u32ToneTrue)
    {
        if (tsAvgToneCount == 0)
        {
            tsAvgToneAgc = tsResultAgc;
            tsAvgToneCount++;
        }
        else
        {
            tsAvgToneAgc = ((HI_U64)(tsAvgToneAgc)*tsAvgToneCount + tsResultAgc)/(tsAvgToneCount+1);
            tsAvgToneCount++;
            //HI_INFO_TUNER("Get Average agc value %d\n", tsAvgToneAgc);
        }

        *pu32ToneTrue = 1;
    }
    else
    {
        *pu32ToneTrue = 0;
    }*/
    *pu32ToneTrue = 0;
    /*usleep(2000);*/
    return HI_SUCCESS;
}

/* ----------------------------------------------------------------------------
Name: scr_tone_enable()
Description:

Parameters:
Return Value:
---------------------------------------------------------------------------- */
static HI_S32 Scr_EnableTone(HI_U32 u32TunerId)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U8  u8ChanByte[2] = {0x0};

    stSendMsg.enLevel = HI_UNF_TUNER_DISEQC_LEVEL_1_X;
    stSendMsg.enToneBurst = TUNER_DISEQC_GetToneBurstStatus(u32TunerId);
    stSendMsg.u8Length = 5;
    stSendMsg.u8RepeatTimes = 0;

    FORMAT_DISEQC_CMD_VALUE(stSendMsg.au8Msg, 0xE0, 0x10, 0x5B, u8ChanByte, 2);
    s32Ret = UNICABLE_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send scr msg fail.\n");
        return s32Ret;
    }

    return s32Ret;
}

/* ----------------------------------------------------------------------------
Name: Scr_OffTones()
Description:
Parameters:
Return Value:
---------------------------------------------------------------------------- */
static HI_S32 Scr_OffTones(HI_U32 u32TunerId, HI_U32 SCRBPF)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U8  u8ChanByte[2] = {0};

    //unsigned char command_array[] = {0xE0, 0x00, 0x5A, 0x00, 0x00};

    if (SCRBPF>=8)
    {
         return HI_FAILURE;
    }
    //HI_INFO_TUNER("enter Scr_OffTones, u32TunerId is 0x%x, UB is %d\n", u32TunerId, SCRBPF+1);

    stSendMsg.enLevel = HI_UNF_TUNER_DISEQC_LEVEL_1_X;
    stSendMsg.enToneBurst = TUNER_DISEQC_GetToneBurstStatus(u32TunerId);
    stSendMsg.u8Length = 5;
    stSendMsg.u8RepeatTimes = 0;

    u8ChanByte[0] |=(SCRBPF<<5);

    FORMAT_DISEQC_CMD_VALUE(stSendMsg.au8Msg, 0xE0, 0x10, 0x5A, u8ChanByte, 2);
    s32Ret = UNICABLE_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send scr msg fail.\n");
        return s32Ret;
    }

    /* 修改原因: 等待MDU设备将信号关闭 */
    usleep(2000);

    return HI_SUCCESS;
}

/* ----------------------------------------------------------------------------
Name: scr_scrdrv_SetFrequency()
Description:
Parameters:
Return Value:
---------------------------------------------------------------------------- */
HI_S32 Scr_SetFrequency(HI_U32 u32TunerId,  HI_U32 Frequency, HI_U32 LNBIndex, HI_U32 SCRBPF)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 tuning_word, temp;
    HI_U8  u8ChanByte[2] = {0};

    if (SCRBPF>=12)
    {
         return HI_FAILURE;
    }

    HI_INFO_TUNER("enter SetFrequency Scr set Freq %d, LNBIndex %d, SCRBPF %d\n", Frequency, LNBIndex, SCRBPF);

    stSendMsg.enLevel = HI_UNF_TUNER_DISEQC_LEVEL_1_X;
    stSendMsg.enToneBurst = TUNER_DISEQC_GetToneBurstStatus(u32TunerId);
    stSendMsg.u8Length = 5;
    stSendMsg.u8RepeatTimes = 0;

    SCRBPF = SCRBPF%4;

    tuning_word = (Frequency/4) - 350; /* Formula according to data sheet */

    u8ChanByte[1] = tuning_word & 0xFF;
    temp = tuning_word & 0x300;
    temp = temp>>8;
    u8ChanByte[0] |= temp;

    temp = SCRBPF<<5;
    u8ChanByte[0] |= temp;

    temp = LNBIndex<<2;
    u8ChanByte[0] |= temp;

    FORMAT_DISEQC_CMD_VALUE(stSendMsg.au8Msg, 0xE0, 0x10, 0x5A, u8ChanByte, 2);
    s32Ret = UNICABLE_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send scr msg fail.\n");
        return s32Ret;
    }

    return(s32Ret);
}

#if 0
static HI_S32 Scr_UBxDeAllocation(HI_U32 u32TunerId, HI_U8 SCRBPF)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_S32 s32Ret = HI_SUCCESS;
    //HI_U8 command_array [5] = {0xE0, 0x00, 0x5B, 0x05, 0x00};
    HI_U8  u8ChanByte[2] = {0x5,0x0};

    if (SCRBPF>=SCR_USER_BAND_NUMBERS)
    {
         return HI_FAILURE;
    }

    HI_INFO_TUNER("enter Scr_UBxDeAllocation,UB is %d\n",SCRBPF);

    stSendMsg.enLevel = HI_UNF_TUNER_DISEQC_LEVEL_1_X;
    stSendMsg.enToneBurst = TUNER_DISEQC_GetToneBurstStatus(u32TunerId);
    stSendMsg.u8Length = 5;
    stSendMsg.u8RepeatTimes = 0;

    u8ChanByte[0] |= (SCRBPF<<5);

    FORMAT_DISEQC_CMD_VALUE(stSendMsg.au8Msg, 0xE0, 0x10, 0x5B, u8ChanByte, 2);
    s32Ret = UNICABLE_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send scr msg fail.\n");
        return s32Ret;
    }


    return s32Ret;
}

static HI_S32 Scr_UBxPresent(HI_U32 u32TunerId, HI_U32 SCRBPF)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_S32 s32Ret = HI_SUCCESS;
    /* E0(Framing)  10/11(address)   5B(command)  data1(Sub_func = 0x06)   data2 =00 */
    HI_U8  u8ChanByte[2] = {0x6,0x0};

    if (SCRBPF>=SCR_USER_BAND_NUMBERS)
    {
         return HI_FAILURE;
    }
    HI_INFO_TUNER("enter Scr_UBxPresent,UB is %d\n",SCRBPF);

    stSendMsg.enLevel = HI_UNF_TUNER_DISEQC_LEVEL_1_X;
    stSendMsg.enToneBurst = TUNER_DISEQC_GetToneBurstStatus(u32TunerId);
    stSendMsg.u8Length = 5;
    stSendMsg.u8RepeatTimes = 0;

    u8ChanByte[0] |= (SCRBPF<<5);

    FORMAT_DISEQC_CMD_VALUE(stSendMsg.au8Msg, 0xE0, 0x10, 0x5B, u8ChanByte, 2);
    s32Ret = UNICABLE_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send scr msg fail.\n");
        return s32Ret;
    }

    return s32Ret;
}
#endif
HI_U32 Scr_RandValue(HI_U32 ucUbNo, HI_U32 ulIsFirstTime)
{
    HI_U32 ulValue = 0;
    HI_U32 ulPvalue = 0;
    HI_U8 ucX1 = 0, ucX3 = 0, ucX10 = 0;
    /** BEGIN: Modified by x00221621, 2014/1/2 问题单号:JIRA DS360T-68  */
    if (ucUbNo >= SCR_USER_BAND_NUMBERS)
    {
        return 0;
    }
    /** END:   Modified by x00221621, 2014/1/2 问题单号:JIRA DS360T-68  */
    if (ulIsFirstTime)
    {
        ulValue = ScrPresetValue[ucUbNo];
    }
    else
    {
        ulValue = ScrLastValue[ucUbNo];
    }
    ucX10 = ulValue & 0x001;
    ucX3  = (ulValue & 0x080) >> 7;
    ucX1  = (ucX3 ^ ucX10) & 0x01;
    ulPvalue = (ulValue >> 1) | (ucX1 << 9);
    ScrLastValue[ucUbNo] = ulPvalue;

    /** Howie add 200ms to avoid a verry small value. */
    return (ulPvalue + 200);
}

HI_U32 Scr_RandTime(HI_U32 ulMaxDur)
{
    HI_S32 fd;
    HI_U32 Rand     = 0;
    HI_U32 RandTime = 0;

    fd = open("/dev/urandom", O_RDONLY | O_NONBLOCK);
    if (fd >= 0)
    {
        HI_S32 ret;

        ret = read(fd, &Rand, sizeof(HI_U32)); /* generate a random integer from /dev/random */
        if (sizeof(HI_U32) == ret)
        {
            RandTime = Rand % ulMaxDur;

            if (RandTime < 200)
            {
                RandTime += 200;
            }
        }

        close(fd);
    }

    return RandTime;
}

TONE_SEARCH_UB_ACCEPT_E Scr_CheckToneOnFreq(HI_U32 u32TunerId, HI_U32 ulCenterFreq)
{
    HI_U32    power_yes      = 1;
    HI_U32       u32ToneTrueYes  = 0;
    HI_U32    power_no       = 1;
    HI_U32       u32ToneTrueNo   = 0;
    HI_U32    center_freq    = ulCenterFreq;
    HI_U32    ulTryTimes     = 0;
    TONE_SEARCH_UB_ACCEPT_E enAccept       = TONE_SEARCH_UB_NO_TONE;

    HI_INFO_TUNER("(%s,%d): Start tone signal scan. Freq = %d!\n", __FUNCTION__, __LINE__, ulCenterFreq);

    if ((center_freq < SCR_D_SCAN_FREQ_START)
        || (center_freq > SCR_D_SCAN_FREQ_END))
    {
        HI_ERR_TUNER("(%s,%d): center_freq out of range!\n",__FUNCTION__, __LINE__);
        return TONE_SEARCH_UB_NO_TONE;
    }

    do
    {
        ulTryTimes++;
        /** 获取位置YES的TONE信号状态 */
        (void)hi3136_SearchTone(u32TunerId, center_freq, &u32ToneTrueYes, &power_yes);

        /** 获取位置NO的TONE信号状态 */
        (void)hi3136_SearchTone(u32TunerId, center_freq+HON_D_FREQ_OFFSET_NO, &u32ToneTrueNo, &power_no);

        HI_INFO_TUNER("[%d]Search tone yes %d   %d", center_freq, center_freq,  power_yes);
        HI_INFO_TUNER("[%d]Search tone no  %d   %d", center_freq, center_freq + HON_D_FREQ_OFFSET_NO,   power_no);

        if (u32ToneTrueYes)
        {
            HI_INFO_TUNER("(%s,%d): [YES] tone detected. tone_freq = %d!\n", __FUNCTION__, __LINE__, center_freq);
            enAccept = TONE_SEARCH_UB_NOT_ASSIGNED;
            break;
        }
        else if (u32ToneTrueNo)
        {
            HI_INFO_TUNER("(%s,%d): [NO] tone detected. tone_freq = %d!\n", __FUNCTION__, __LINE__, center_freq + HON_D_FREQ_OFFSET_NO);
            enAccept = TONE_SEARCH_UB_ASSIGNED;
            break;
        }
        else
        {
            HI_ERR_TUNER("[%d](%s,%d): There is no valid tone detected. \n", ulTryTimes, __FUNCTION__, __LINE__);
        }

    }while(ulTryTimes < 3);
    HI_INFO_TUNER("(%s,%d): Stop tone signal scan. center_freq = %d!\n", __FUNCTION__, __LINE__, center_freq);
    return enAccept;
}

TONE_SEARCH_UB_ACCEPT_E Scr_CheckToneOnUB(HI_U32 u32TunerId,HI_U32 SCRBPF)
{
    HI_U32    center_freq    = 0;
    HI_U32     ucIndex        = 0;

    HI_INFO_TUNER("(%s,%d): Start tone signal scan. UB = %d!\n", __FUNCTION__, __LINE__, SCRBPF+1);

    ucIndex = SCRBPF;

    center_freq = Scr_GetCenterFreq(ucIndex);

    return Scr_CheckToneOnFreq(u32TunerId, center_freq);
}

HI_U32 Scr_GetToneAgc(HI_U32 u32TunerId, HI_U32 u32CenterFreq)
{
    HI_U32    u32RatioYes      = 1;
    HI_U32    u32ToneTrue  = 0;
    HI_U32    u32IfAgcYes      = 0xffffffff;

    HI_INFO_TUNER("(%s,%d): Start tone signal scan. ulCenterFreq = %d!\n", __FUNCTION__, __LINE__, u32CenterFreq);

    if ((u32CenterFreq < SCR_D_SCAN_FREQ_START)
        || (u32CenterFreq > SCR_D_SCAN_FREQ_END))
    {
        HI_ERR_TUNER("(%s,%d): center_freq out of range!\n",__FUNCTION__, __LINE__);
        return u32RatioYes;
    }

    /** 获取位置YES的TONE信号状态 */
    hi3136_SearchTone(u32TunerId, u32CenterFreq, &u32ToneTrue, &u32IfAgcYes);

    if (u32ToneTrue)
    {
        HI_INFO_TUNER("(%s,%d): [YES] tone detected. tone_freq = %d!\n", __FUNCTION__, __LINE__, u32CenterFreq);
    }
    else
    {
        HI_INFO_TUNER("(%s,%d): There is no valid tone detected. \n", __FUNCTION__, __LINE__);
    }

    //HI_INFO_TUNER("(%s,%d): Stop tone signal scan. center_freq = %d!\n", __FUNCTION__, __LINE__, u32CenterFreq);
    return u32IfAgcYes;
}

HI_S32 Scr_CheckOffValid(HI_U32 u32TunerId, HI_U32 u32Index, HI_U32 u32CentralFreq)
{
    HI_U32    u32TaskDelayMs   = 0;
    HI_S32    s32Agc           = 0;
    HI_U32     i               = 0;
    HI_U8     u32FreqIndex     = 0;
    HI_S32    s32Ret            = HI_FAILURE;
    HI_U32    u32Fcenter = 0;

    if (SCR_USER_BAND_NUMBERS <= u32Index)
    {
        HI_ERR_TUNER("cIndex out of range!\n");
        return HI_FAILURE;
    }

    HI_INFO_TUNER("Check off valid index[%d]  centralfreq[%d]", u32Index, u32CentralFreq);

    u32FreqIndex = u32Index;
    u32Fcenter = u32CentralFreq;

    if (0 == u32Fcenter)
    {
        u32Fcenter = Scr_GetCenterFreq(u32FreqIndex);
    }

    for (i=0; i<3; i++)
    {
        /*ulTaskDelayMs = Scr_RandValue(ucIndex,ulIsFirstTime);
        usleep(ulTaskDelayMs);*/
        u32TaskDelayMs = Scr_RandTime(1000);
        usleep(u32TaskDelayMs);

        Scr_OffTones(u32TunerId, u32Index);

        Scr_GetToneAgc(u32TunerId, u32Fcenter);

        s32Agc = (HI_S32)tsResultAgc;

        if ( s32Agc > tsAvgToneAgc + 300)
        {
            HI_INFO_TUNER("UB:%d off valid.\n",u32Index+1);
            s32Ret = HI_SUCCESS;
            break;
        }

    }

    return s32Ret;
}

#if 0
/** 修改原因: 解决MDU自动安装冲突问题 */
static TONE_SEARCH_UB_ACCEPT_E Scr_MDUAccept(HI_U32 u32TunerId, HI_U32 SCRBPF, HI_U16 SCRCenterFrequency)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U8  u8ChanByte[2] = {0x4,0x0};
    /** HON_Status Error1       = NEXUS_TIMEOUT; */

    //HI_U8 command_array [5] = {0xE0, 0x00, 0x5B, 0x04, 0x00};

    TONE_SEARCH_UB_ACCEPT_E enAccept    = TONE_SEARCH_UB_NO_TONE;

    HI_INFO_TUNER("%s entered SCRBPF %d, Freq %d\n", __FUNCTION__, SCRBPF+1, SCRCenterFrequency);

    if (SCRBPF>= SCR_USER_BAND_NUMBERS || SCRCenterFrequency<=950 || SCRCenterFrequency>=2150)
    {
        HI_ERR_TUNER("Accept error\n");
        return TONE_SEARCH_UB_ACCEPT_ERROR;
    }

    stSendMsg.enLevel = HI_UNF_TUNER_DISEQC_LEVEL_1_X;
    stSendMsg.enToneBurst = TUNER_DISEQC_GetToneBurstStatus(u32TunerId);
    stSendMsg.u8Length = 5;
    stSendMsg.u8RepeatTimes = 0;

    /* E0(Framing)  10/11(address)   5B(command)  data1(Sub_func = 0x04)   data2 =00 */
    u8ChanByte[0] |= (SCRBPF<<5);

    HI_INFO_TUNER("Accept the ub %d !", SCRBPF);

    FORMAT_DISEQC_CMD_VALUE(stSendMsg.au8Msg, 0xE0, 0x0, 0x5B, u8ChanByte, 2);
    s32Ret = UNICABLE_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send scr msg fail.\n");
        return s32Ret;
    }

    usleep(100);

    /** 修改原因: 只要状态不是空闲，都需要放弃该UB */
    enAccept = Scr_CheckToneOnFreq(u32TunerId, SCRCenterFrequency);

    return enAccept;
}

HI_S32 Scr_GetAverageAGC(HI_U8 *pcAGC, HI_S32 iLength)
{
    HI_S32     iLoop       = 0;
    HI_S32     iIndex      = 0;
    HI_U8      cAgcTemp    = 0;
    HI_S32     iAgcSum     = 0;
    HI_S32     iAgcAverage = 0;

    if (0 >= iLength)
    {
        return 0;
    }
    else if (1 == iLength)
    {
        return *pcAGC;
    }

    /** Sort the AGC value at first. */
    for (iLoop = 0; iLoop < iLength-1; iLoop++)
    {
        for (iIndex = iLoop+1; iIndex < iLength; iIndex++)
        {
            if (*(pcAGC+iLoop) < *(pcAGC+iIndex))
            {
                cAgcTemp        = *(pcAGC+iLoop);
                *(pcAGC+iLoop)  = *(pcAGC+iIndex);
                *(pcAGC+iIndex) = cAgcTemp;
            }
        }
    }

    /** Calculate the average value of AGC. */
    if (2 == iLength)
    {
        iAgcAverage = (*pcAGC + *(pcAGC+1))/2;
    }
    else
    {
        for (iIndex = 1; iIndex < iLength-1 ; iIndex++)
        {
            iAgcSum += *(pcAGC+iIndex);
        }

        iAgcAverage = iAgcSum / (iLength-2);
    }

    return iAgcAverage;
}

/*HI_S32 Scr_DeleteSameTone(HI_U8 *pu8NbTones, HI_U32 *uiToneList, HI_U32 *uiPowerList)
{
    HI_U32 LastTone=0;
    HI_U32 ToneSum = 0;
    HI_U32 PowerSum = 0;
    HI_U32 u32Count = 1;
    HI_U32 UBList[SCR_D_MAX_POINTS_MAYBE_USER_BAND];
    HI_U32 PowerList[SCR_D_MAX_POINTS_MAYBE_USER_BAND];
    HI_U8 i,j=0;
    HI_U8 TonesNum=0;

    TonesNum = *pu8NbTones;
    LastTone = *uiToneList;
    ToneSum += LastTone;
    PowerSum += *uiPowerList;
    for(i = 1;i < TonesNum;i++)
    {
        if(ABS( *(uiToneList+i) - LastTone) <= 10)
        {
            ToneSum += *(uiToneList+i);
            PowerSum += *(uiPowerList+i);
            u32Count++;
            LastTone = *(uiToneList+i);
        }
        else
        {
            UBList[j] = ToneSum / u32Count;
            PowerList[j] = PowerSum / u32Count;
            ToneSum = 0;
            PowerSum = 0;
            u32Count = 1;
            LastTone = *(uiToneList+i);
            ToneSum += LastTone;
            PowerSum += *(uiPowerList+i);
            j++;
            if(j >= SCR_D_MAX_POINTS_MAYBE_USER_BAND)
            {
                break;
            }
        }
    }

    UBList[j] = ToneSum / u32Count;
    PowerList[j] = PowerSum / u32Count;
    j++;

    for(i = 0;i<j;i++)
    {
        *(uiToneList+i) = UBList[i];
        *(uiPowerList+i) = PowerList[i];
    }

   *pu8NbTones = j;

   return 0;
}*/
#endif

HI_S32 Scr_SortUbByAgc(SCR_TONE_S *psUBList, HI_S32  s32Count)
{
    HI_S32              s32Index  = 0;
    SCR_TONE_S          sUbTemp;
    SCR_TONE_S          sUb1;
    SCR_TONE_S          sUb2;
    HI_S32              s32Loop   = 0;

    memset((void*)&sUbTemp, 0x00, sizeof(sUbTemp));
    memset((void*)&sUb1, 0x00, sizeof(sUb1));
    memset((void*)&sUb2, 0x00, sizeof(sUb2));

    if (1 >= s32Count)
    {
        return HI_FAILURE;
    }

    /** Sort the AGC value at first. */
    for (s32Loop = 0; s32Loop < s32Count-1; s32Loop++)
    {
        for (s32Index = s32Loop+1; s32Index < s32Count; s32Index++)
        {
            sUb1 = *(psUBList+s32Loop);
            sUb2 = *(psUBList+s32Index);
            if (sUb1.s32Agc > sUb2.s32Agc)
            {
                sUbTemp = *(psUBList+s32Loop);
                *(psUBList+s32Loop)   = *(psUBList+s32Index);
                *(psUBList+s32Index)  = sUbTemp;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 Scr_SortUbByFreq(SCR_TONE_S *psUBList, HI_S32 s32Count)
{
    HI_S32              s32Loop   = 0;
    HI_S32              s32Index  = 0;
    SCR_TONE_S          sUbTemp;
    SCR_TONE_S          sUb1;
    SCR_TONE_S          sUb2;

    memset((void*)&sUbTemp, 0x00, sizeof(sUbTemp));
    memset((void*)&sUb1, 0x00, sizeof(sUb1));
    memset((void*)&sUb2, 0x00, sizeof(sUb2));

    if (1 >= s32Count)
    {
        return HI_FAILURE;
    }

    /** Sort the freq value at first. */
    for (s32Loop = 0; s32Loop < s32Count-1; s32Loop++)
    {
        for (s32Index = s32Loop+1; s32Index < s32Count; s32Index++)
        {
            sUb1 = *(psUBList+s32Loop);
            sUb2 = *(psUBList+s32Index);
            if (sUb1.s32Freq > sUb2.s32Freq)
            {
                sUbTemp = *(psUBList+s32Loop);
                *(psUBList+s32Loop)    = *(psUBList+s32Index);
                *(psUBList+s32Index)   = sUbTemp;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 Scr_BlindScanTone(HI_VOID *pstScanPara)
{
    HI_S32          s32Index  = 0,s32Index2 = 0;
    HI_S32          s32MinAgcIndex = 0;
    HI_S32          s32Ret  = HI_FAILURE;
    HI_S32          s32Count=0,s32ToneFreucncy = 0;;
	HI_S32			s32Threshold = 0;
	HI_S32			s32AgcValue = 0;
	HI_S32			s32ToneFreq[SCR_USER_BAND_NUMBERS] = {0};
	HI_U32          u32UBCount=0,u32RealUBNumber = 0,i,u32UBCount_sub_1 = 0;
	HI_U32			u32TunerPort = 0;
	SCR_TONE_S		sSCRArray[1200];
	SCR_TONE_S		sMinAgc[SCR_USER_BAND_NUMBERS],sBoundaryAgc,sMaxAgc[SCR_USER_BAND_NUMBERS];
	HI_UNF_TUNER_UNICABLE_SCAN_STATUS_E stUnicScanStatus;
	HI_UNF_TUNER_UNICABLE_SCAN_PARA_S stLocalScanPara;
	HI_UNF_TUNER_UNICABLE_SCAN_USER_BAND_NOTIFY_S stNotify;
	HI_DOUBLE dThresholdStart, dThresholdStop, dToneFreq, dPercent;
	HI_U16 u16Progress = 0,u16PreProgress = 0;
	//timeval stStart, stStop;

	//gettimeofday(&stStart,NULL);
    stLocalScanPara = *(HI_UNF_TUNER_UNICABLE_SCAN_PARA_S *)pstScanPara;
    HI_FREE(HI_ID_TUNER, pstScanPara);
    
    
	Scr_EnableTone(u32TunerPort);
	usleep(100);

	memset(sSCRArray, 0x77, sizeof(sSCRArray));
	memset(sMaxAgc,0,sizeof(SCR_TONE_S)*SCR_USER_BAND_NUMBERS);
	
	for(s32Index = SCR_D_SCAN_FREQ_START; s32Index < SCR_D_SCAN_FREQ_END ; s32Index++)
	{
		sSCRArray[s32Count].s32Freq = s32Index;
		s32Ret = TUNER_Get_Agc(u32TunerPort,s32Index, &(sSCRArray[s32Count].s32Agc));
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("TUNER_Get_Agc fail.\n");
            return HI_FAILURE;
        }
		s32Count++;
		
		if(g_u32UnicableStopping)
		{
			stUnicScanStatus = HI_UNF_TUNER_UNICABLE_SCAN_STATUS_QUIT;
			stNotify.penStatus = &stUnicScanStatus;
			stLocalScanPara.pfnEVTNotify(u32TunerPort,HI_UNF_TUNER_UNICABLE_SCAN_EVT_STATUS, &stNotify);
			return HI_SUCCESS;
		}

        if(s32Count%5 == 0)
        {
            dPercent = (HI_DOUBLE)(s32Count)/1200*98;
            u16Progress = (HI_U16)dPercent;
            if(u16Progress > u16PreProgress)
            {
                u16PreProgress = u16Progress;
                stNotify.pu16ProgressPercent = &u16Progress;
                stLocalScanPara.pfnEVTNotify(u32TunerPort,HI_UNF_TUNER_UNICABLE_SCAN_EVT_PROGRESS, &stNotify);
            }
        }
    }

    Scr_SortUbByAgc(sSCRArray, s32Count);

    sMinAgc[0] = sSCRArray[0];
    u32UBCount = 1;
    for(s32Index = 1; s32Index < s32Count; s32Index++)
    {
        for(s32Index2 = 0; s32Index2 < SCR_USER_BAND_NUMBERS && s32Index2 < u32UBCount; s32Index2++)
        {
            if(ABS(sSCRArray[s32Index].s32Freq - sMinAgc[s32Index2].s32Freq) > MIN_DISTANCE_BETWEEN_UB_MHZ)
                continue;
            else
                break;
        }

        if(s32Index2 < SCR_USER_BAND_NUMBERS && s32Index2 >= u32UBCount)
        {
            sMinAgc[u32UBCount] = sSCRArray[s32Index];
            u32UBCount++;
            if(u32UBCount >= SCR_USER_BAND_NUMBERS)
            {
                break;
            }
        }
    }

    Scr_SortUbByFreq(sSCRArray,s32Count);

    Scr_SortUbByAgc(sMinAgc,SCR_USER_BAND_NUMBERS);

	//tuner lowpass filter bandwidth = 30MHz
	sBoundaryAgc = sSCRArray[sMinAgc[3].s32Freq - 15 - SCR_D_SCAN_FREQ_START];

	//agc threshold equal one half of max agc subing min agc
	s32Threshold = sMinAgc[3].s32Agc + ABS(sBoundaryAgc.s32Agc - sMinAgc[3].s32Agc ) / 4;

    for(u32UBCount = 0;u32UBCount < SCR_USER_BAND_NUMBERS;u32UBCount++)
    {
        if(sMinAgc[u32UBCount].s32Agc <= s32Threshold)
        {
            s32ToneFreq[u32UBCount] = sMinAgc[u32UBCount].s32Freq;
            u32RealUBNumber++;
        }
        else
            break;
    }

    Scr_SortUbByFreq(sMinAgc, u32RealUBNumber);

	//begin check tone signal is true or not
	for(u32UBCount = 0; u32UBCount < SCR_USER_BAND_NUMBERS;u32UBCount++)
	{
		Scr_OffTones(u32TunerPort,u32UBCount);
		usleep(5*1000);
	}
	s32Count = 0;
	for(u32UBCount = 0; u32UBCount < u32RealUBNumber;u32UBCount++)
	{
		if(u32UBCount_sub_1)
		{
			u32UBCount = 0;
			u32UBCount_sub_1 = 0;
		}
		for(i = 0;i < 10;i++)
		{
			s32Ret = TUNER_Get_Agc(u32TunerPort, sMinAgc[u32UBCount].s32Freq, &s32AgcValue);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("TUNER_Get_Agc fail.\n");
                return HI_FAILURE;
            }
            
			if(s32AgcValue != 0x77777777)   //agc not lock,try get agc again
			{
				break;
			}
		}
		//HI_INFO_TUNER("After scr off,tone freq %d agc value %d\n",sMinAgc[u32UBCount].s32Freq,s32AgcValue);
		if(ABS(s32AgcValue - sMinAgc[u32UBCount].s32Agc) >= MIN_AGC_CHANGED_BETWEEN_ON_AND_OFF)
		{
			sMaxAgc[s32Count].s32Agc = s32AgcValue;
			sMaxAgc[s32Count].s32Freq = sMinAgc[u32UBCount].s32Freq;
			s32Count++;
		}
		else
		{
			for(i = u32UBCount;i < u32RealUBNumber;i++)
			{
				if(i < (SCR_USER_BAND_NUMBERS-1))
				{
					sMinAgc[i] = sMinAgc[i+1];
				}
			}
			u32RealUBNumber--;
			if(u32UBCount > 0)
			{
				u32UBCount = u32UBCount - 1;
			}
			else
			{
				u32UBCount_sub_1 = 1;
			}
		}
	}
	//end check
	//gettimeofday(&stStop,NULL);
	u16Progress = 99;
	stNotify.pu16ProgressPercent = &u16Progress;
	stLocalScanPara.pfnEVTNotify(u32TunerPort,HI_UNF_TUNER_UNICABLE_SCAN_EVT_PROGRESS, &stNotify);
	
	u32RealUBNumber = (HI_U32)s32Count;
	if(s32Count == 0)
	{
		HI_INFO_TUNER("There is no tone,please check the unicable!\n");
		s_SysUnicable.u32TotalUBNumber = 0;
		u16Progress = 100;
		stNotify.pu16ProgressPercent = &u16Progress;
		stLocalScanPara.pfnEVTNotify(u32TunerPort,HI_UNF_TUNER_UNICABLE_SCAN_EVT_PROGRESS, &stNotify);
		
		stUnicScanStatus = HI_UNF_TUNER_UNICABLE_SCAN_STATUS_FAIL;
		stNotify.penStatus = &stUnicScanStatus;
		stLocalScanPara.pfnEVTNotify(u32TunerPort,HI_UNF_TUNER_UNICABLE_SCAN_EVT_STATUS, &stNotify);
		return HI_FAILURE;
	}

	Scr_EnableTone(u32TunerPort);
	usleep(100);
	/**maybe cable disconnect,check again**/
	s32Count = 0;
	for(u32UBCount = 0; u32UBCount < u32RealUBNumber;u32UBCount++)
	{
		if(u32UBCount_sub_1)
		{
			u32UBCount = 0;
			u32UBCount_sub_1 = 0;
		}
		for(i = 0;i < 10;i++)
		{
			s32Ret = TUNER_Get_Agc(u32TunerPort, sMinAgc[u32UBCount].s32Freq, &s32AgcValue);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("TUNER_Get_Agc fail.\n");
                return HI_FAILURE;
            }
            
			if(s32AgcValue != 0x77777777)   //agc not lock,try get agc again
			{
				break;
			}
		}
		//HI_INFO_TUNER("After scr on,tone freq %d agc value %d\n",sMinAgc[u32UBCount].s32Freq,s32AgcValue);
		if(ABS(s32AgcValue - sMaxAgc[u32UBCount].s32Agc) >= MIN_AGC_CHANGED_BETWEEN_ON_AND_OFF)
		{
			s32Count++;
		}
		else
		{
			for(i = u32UBCount;i < u32RealUBNumber;i++)
			{
				if(i < (SCR_USER_BAND_NUMBERS-1))
				{
					sMinAgc[i] = sMinAgc[i+1];
				}
			}
			u32RealUBNumber--;
			if(u32UBCount > 0)
			{
				u32UBCount = u32UBCount - 1;
			}
			else
			{
				u32UBCount_sub_1 = 1;
			}
		}
	}
	u32RealUBNumber = (HI_U32)s32Count;
	if(s32Count == 0)
	{
		HI_INFO_TUNER("There is no tone,please check the unicable!\n");
		s_SysUnicable.u32TotalUBNumber = 0;
		u16Progress = 100;
		stNotify.pu16ProgressPercent = &u16Progress;
		stLocalScanPara.pfnEVTNotify(u32TunerPort,HI_UNF_TUNER_UNICABLE_SCAN_EVT_PROGRESS, &stNotify);
		
		stUnicScanStatus = HI_UNF_TUNER_UNICABLE_SCAN_STATUS_FAIL;
		stNotify.penStatus = &stUnicScanStatus;
		stLocalScanPara.pfnEVTNotify(u32TunerPort,HI_UNF_TUNER_UNICABLE_SCAN_EVT_STATUS, &stNotify);
		return HI_FAILURE;
	}
	
	for(u32UBCount = 0; u32UBCount < u32RealUBNumber;u32UBCount++)
	{	
		s32MinAgcIndex = sMinAgc[u32UBCount].s32Freq - SCR_D_SCAN_FREQ_START;
		//dThresholdStart = 0;
		//dThresholdStop = 0;
		//dToneFreq = 0;

        for(s32Index = s32MinAgcIndex ; s32Index > s32MinAgcIndex - 30; s32Index--)
        {
            if(sSCRArray[s32Index].s32Agc < s32Threshold)
            {
                continue;
            }

            //连续两个点都在阈值线以下
            if(sSCRArray[s32Index - 1].s32Agc >= s32Threshold)
            {
                break;
            }
        }

        dThresholdStart = (HI_DOUBLE)sSCRArray[s32Index].s32Freq + ((HI_DOUBLE)sSCRArray[s32Index + 1].s32Freq - (HI_DOUBLE)sSCRArray[s32Index].s32Freq)/2.0;

        for(s32Index = s32MinAgcIndex; s32Index < s32MinAgcIndex + 30; s32Index++)
        {
            if(sSCRArray[s32Index].s32Agc < s32Threshold)
            {
                continue;
            }

            //连续两个点都在阈值线以下
            if(sSCRArray[s32Index + 1].s32Agc >= s32Threshold)
            {
                break;
            }
        }

        dThresholdStop = (HI_DOUBLE)sSCRArray[s32Index-1].s32Freq + ((HI_DOUBLE)sSCRArray[s32Index].s32Freq - (HI_DOUBLE)sSCRArray[s32Index-1].s32Freq)/2.0;
        dToneFreq = dThresholdStart + (dThresholdStop - dThresholdStart)/2.0;
        s32ToneFreucncy = (HI_S32)dToneFreq;
        //HI_INFO_TUNER("<<<<<<<<<====dToneFreq:%f======s32ToneFreucncy:%d\n",dToneFreq,s32ToneFreucncy);

        if(s32ToneFreucncy % 2)
        {
            for(s32Index2 =1;s32Index2 < 30;s32Index2++)
            {
                if(sSCRArray[s32MinAgcIndex + s32Index2].s32Agc < sSCRArray[s32MinAgcIndex - s32Index2].s32Agc)
                {
                    if(sSCRArray[s32MinAgcIndex + s32Index2 + 1].s32Agc <= sSCRArray[s32MinAgcIndex - s32Index2 - 1].s32Agc)
                    {
                        s32ToneFreucncy += 1;
                        break;
                    }
                }
                else if(sSCRArray[s32MinAgcIndex + s32Index2].s32Agc > sSCRArray[s32MinAgcIndex - s32Index2].s32Agc)
                {
                    if(sSCRArray[s32MinAgcIndex + s32Index2 + 1].s32Agc >= sSCRArray[s32MinAgcIndex - s32Index2 - 1].s32Agc)
                    {
                        s32ToneFreucncy -= 1;
                        break;
                    }
                }
            }
        }

        s32ToneFreq[u32UBCount] = s32ToneFreucncy;
    }

    for(u32UBCount = 0; u32UBCount < u32RealUBNumber;u32UBCount++)
    {
        HI_INFO_TUNER("++++++++u32ToneFreq:%d\n",s32ToneFreq[u32UBCount]);
    }

    s_SysUnicable.u32TotalUBNumber = u32RealUBNumber;
    for(u32UBCount = 0;u32UBCount < u32RealUBNumber;u32UBCount++)
    {
        s_SysUnicable.u32UBInfo[u32UBCount].u32SCRNo = u32UBCount;
        s_SysUnicable.u32UBInfo[u32UBCount].s32CenterFreq = s32ToneFreq[u32UBCount];
    }

    s_SysUnicable.u32UBIndex = 0;
    u16Progress = 100;
    stNotify.pu16ProgressPercent = &u16Progress;
    stLocalScanPara.pfnEVTNotify(u32TunerPort,HI_UNF_TUNER_UNICABLE_SCAN_EVT_PROGRESS, &stNotify);

    stUnicScanStatus = HI_UNF_TUNER_UNICABLE_SCAN_STATUS_FINISH;
    stNotify.penStatus = &stUnicScanStatus;
    stLocalScanPara.pfnEVTNotify(u32TunerPort,HI_UNF_TUNER_UNICABLE_SCAN_EVT_STATUS, &stNotify);

    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_UNICABLE_ScanUserBands(HI_U32 u32TunerId, HI_UNF_TUNER_UNICABLE_SCAN_PARA_S stScanPara)
{
    HI_UNF_TUNER_UNICABLE_SCAN_PARA_S *pstLocalScanPara = HI_NULL;
    HI_S32 s32Result;
    pthread_t thd_ScanUB;

    tsAvgToneCount = 0;
    tsAvgToneAgc = 0;

    /*在空闲UB上产生tone信号*/
    Scr_EnableTone(u32TunerId);

    usleep(2000);

    g_u32UnicableStopping = 0;
    
    pstLocalScanPara = HI_MALLOC(HI_ID_TUNER, sizeof(HI_UNF_TUNER_UNICABLE_SCAN_PARA_S));
    if (pstLocalScanPara == HI_NULL)
    {
        HI_ERR_TUNER("No memory.\n");
        return HI_ERR_TUNER_FAILED_ALLOC_MEM;
    }
    memcpy(pstLocalScanPara, &stScanPara, sizeof(HI_UNF_TUNER_UNICABLE_SCAN_PARA_S));
    
    /*全频段扫描疑似tone信号，并保存*/
    s32Result = pthread_create(&thd_ScanUB, 0, (HI_VOID *)Scr_BlindScanTone, pstLocalScanPara);
    if (HI_SUCCESS != s32Result)
    {
        HI_ERR_TUNER("Create pthread fail.\n");
        return HI_FAILURE;
    }
    pthread_detach(thd_ScanUB);

    return HI_SUCCESS;

}

HI_S32 HI_UNF_TUNER_UNICABLE_ExitScanUserBands(HI_U32 u32TunerId)
{
    g_u32UnicableStopping = 1;
    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_UNICABLE_GetUserBandsInfo(HI_U32 u32TunerId, HI_UNF_TUNER_SCR_UB_S **ppUBInfo, HI_U32 *pu32Num)
{
    if(HI_NULL == ppUBInfo)
    {
        HI_ERR_TUNER("data is null ptr.\n");
        return HI_FAILURE;
    }

    *pu32Num = s_SysUnicable.u32TotalUBNumber;

    memcpy(ppUBInfo,s_SysUnicable.u32UBInfo,(s_SysUnicable.u32TotalUBNumber)*sizeof(HI_UNF_TUNER_SCR_UB_S));

    return HI_SUCCESS;
}



/* ----------------------------- End of file (mdu.c) ------------------ */
