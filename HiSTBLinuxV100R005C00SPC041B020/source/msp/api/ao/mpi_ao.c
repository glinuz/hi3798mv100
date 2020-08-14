/*****************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_aenc.c
* Description: Describe main functionality and purpose of this file.
*
* History:
* Version   Date         Author     DefectNum    Description
* 0.01
*
*****************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>

#include "hi_debug.h"
#include "hi_module.h"
#include "hi_mpi_mem.h"
#include "hi_drv_struct.h"
#include "hi_error_mpi.h"

#include "hi_mpi_ao.h"
#include "hi_drv_ao.h"
#include "drv_ao_ioctl.h"
#include "hi_mpi_ai.h"

#include "mpi_vir.h"
#include "mpi_lowlatency.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 g_s32AOFd = -1;
#define DEVNAME_AO "/dev/" UMAP_DEVNAME_AO
static HI_U32 g_s32AoInitCnt = 0;

#if defined (HI_SMARTVOLUME_SUPPORT)
#include "smartvol.h"

/* smart sound information */
typedef struct
{
    HSmartVol          hSmartVol;      /* Smart volume Handle */
    HI_S32             s32LastStatus;
    HI_BOOL            bTrackOpen;
    HI_BOOL            bSwitchProgram;
    HI_BOOL            bEnable;
    SamrtVol_In_Para_S stInParam;
} AO_CHN_SMARTVOL_S;

static AO_CHN_SMARTVOL_S g_stSmartVol[AO_MAX_TOTAL_TRACK_NUM] =
{
    {
        HI_NULL, HI_SUCCESS, HI_FALSE, HI_FALSE, HI_FALSE,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    }
};

static HI_VOID    SmartVolumeProcess(HI_UNF_AO_FRAMEINFO_S* pstAOFrame, AO_CHN_SMARTVOL_S* stSmartVol)
{
    if (NULL == stSmartVol)
    {
        HI_ERR_AO("Null pointer stSmartVol!\n");
        return;
    }

    if (pstAOFrame->u32PcmSamplesPerFrame == 0)
    {
        HI_ERR_AO("pcm samples per frame is zero!\n");
        return;
    }

    if (pstAOFrame->u32Channels > 2)
    {
        HI_ERR_AO("Smartvolume don't support multichannel(%d)\n", pstAOFrame->u32Channels);
        return;
    }

    if (HI_FALSE == pstAOFrame->bInterleaved)
    {
        HI_ERR_AO("Smartvolume don't support none bInterleaved pcm format\n");
        return;
    }

    SamrtVol_In_Para_P pstInParam = &(stSmartVol->stInParam);

    if (HI_NULL == stSmartVol->hSmartVol)
    {
        HSmartVol tmp;
        (HI_VOID)HI_SmartGetDefaultConfig(pstInParam);
        pstInParam->framelen = (HI_S32)pstAOFrame->u32PcmSamplesPerFrame;
        pstInParam->samprate = (HI_S32)pstAOFrame->u32SampleRate;
        pstInParam->nchans   = (HI_S32)pstAOFrame->u32Channels;
        pstInParam->bitdepth = (pstAOFrame->s32BitPerSample == 16) ? 16 : 32;
        pstInParam->Interleaved = 1;
        tmp = HI_SmartVolOpen(pstInParam);

        if (HI_NULL != tmp)
        {
            stSmartVol->hSmartVol = tmp;
            (HI_VOID)HI_SmartVolEnable(tmp);
        }
    }

    if (HI_NULL != stSmartVol->hSmartVol)
    {
        HI_S32 bReset = 0;

        /* check if need to reset samrtvol state */
        //bReset |= (pstAOFrame->u32PcmSamplesPerFrame != pstInParam->framelen);
        //bReset |= (((HI_S32)(pstAOFrame->u32SampleRate)) != pstInParam->samprate);
        //bReset |= (pstAOFrame->u32Channels != pstInParam->nchans);
        //bReset |= (pstAOFrame->bInterleaved != pstInParam->Interleaved);
        if ((pstAOFrame->u32PcmSamplesPerFrame != (HI_U32)pstInParam->framelen) || (pstAOFrame->u32SampleRate != (HI_U32)pstInParam->samprate) || (pstAOFrame->u32Channels != (HI_U32)pstInParam->nchans) || ((HI_S32)pstAOFrame->bInterleaved != pstInParam->Interleaved))
        {
            bReset = 1;
        }

        if ((0 != bReset) || (stSmartVol->bSwitchProgram))
        {
            pstInParam->framelen = (HI_S32)pstAOFrame->u32PcmSamplesPerFrame;
            pstInParam->samprate = (HI_S32)(pstAOFrame->u32SampleRate);
            pstInParam->nchans   = (HI_S32)pstAOFrame->u32Channels;
            pstInParam->bitdepth = (pstAOFrame->s32BitPerSample == 16) ? 16 : 32;
            pstInParam->Interleaved = 1;
            (HI_VOID)HI_SmartVolClear(stSmartVol->hSmartVol, pstInParam);
            HI_INFO_AO("audio swtich, smartvol trigger\n");
        }

        if (HI_TRUE == stSmartVol->bSwitchProgram)
        {
            stSmartVol->bSwitchProgram = HI_FALSE;
        }

        /* SmartVolume Process */
        (HI_VOID)HI_SmartVolPro(stSmartVol->hSmartVol, (HI_S32*)pstAOFrame->ps32PcmBuffer,
                                NULL, 0, (HI_S32)pstAOFrame->u32PcmSamplesPerFrame);
    }
}

HI_S32   HI_MPI_AO_Track_SetSmartVolume(HI_HANDLE hTrack, HI_BOOL bEnable)
{
    HI_S32 s32Ret = HI_FAILURE;
    AO_CHN_SMARTVOL_S* pstSmartVolume;

    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    pstSmartVolume = &(g_stSmartVol[hTrack & AO_TRACK_CHNID_MASK]);
    if (HI_TRUE == pstSmartVolume->bTrackOpen)
    {
        pstSmartVolume->bEnable = bEnable;
        s32Ret = HI_SUCCESS;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_GetSmartVolume(HI_HANDLE hTrack, HI_BOOL* pbEnable)
{
    HI_S32 s32Ret = HI_FAILURE;
    AO_CHN_SMARTVOL_S* pstSmartVolume;

    CHECK_AO_NULL_PTR(pbEnable);
    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    pstSmartVolume = &(g_stSmartVol[hTrack & AO_TRACK_CHNID_MASK]);
    if (HI_TRUE == pstSmartVolume->bTrackOpen)
    {
        *pbEnable = pstSmartVolume->bEnable;
        s32Ret = HI_SUCCESS;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_TriggerSmartVolume(HI_HANDLE hTrack)
{
    HI_S32 s32Ret = HI_FAILURE;
    AO_CHN_SMARTVOL_S* pstSmartVolume;

    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    pstSmartVolume = &(g_stSmartVol[hTrack & AO_TRACK_CHNID_MASK]);
    if (HI_TRUE == pstSmartVolume->bTrackOpen)
    {
        pstSmartVolume->bSwitchProgram = HI_TRUE;
        pstSmartVolume->s32LastStatus  = HI_SUCCESS;
        s32Ret = HI_SUCCESS;
    }

    return s32Ret;
}
#endif

HI_S32 HI_MPI_AO_Init(HI_VOID)
{
    if (!g_s32AoInitCnt)
    {
        if (g_s32AOFd < 0)
        {
            g_s32AOFd = open(DEVNAME_AO, O_RDWR, 0);
            if (g_s32AOFd < 0)
            {
                HI_FATAL_AO("OpenAODevice err\n");
                g_s32AOFd = -1;
                return HI_ERR_AO_CREATE_FAIL;
            }
        }

        VIR_InitRS();
    }

    g_s32AoInitCnt++;

    return HI_SUCCESS;
}

HI_S32   HI_MPI_AO_DeInit(HI_VOID)
{
    if (!g_s32AoInitCnt)
    {
        return HI_SUCCESS;
    }

    g_s32AoInitCnt--;

    if (!g_s32AoInitCnt)
    {
        if (g_s32AOFd > 0)
        {
            close(g_s32AOFd);
            g_s32AOFd = -1;
        }

        VIR_DeInitRS();
    }

    return HI_SUCCESS;
}

static HI_VOID AO_SND_ATTR_UNF2DRV(const HI_UNF_SND_ATTR_S* pstUnfAttr, DRV_SND_ATTR_S* pstDrvAttr)
{
    HI_U32 i = 0;
    pstDrvAttr->u32PortNum              = pstUnfAttr->u32PortNum;
    pstDrvAttr->enSampleRate            = pstUnfAttr->enSampleRate;
    pstDrvAttr->u32MasterOutputBufSize  = pstUnfAttr->u32MasterOutputBufSize;
    pstDrvAttr->u32SlaveOutputBufSize   = pstUnfAttr->u32SlaveOutputBufSize;

    for (i = 0; i < HI_UNF_SND_OUTPUTPORT_MAX; i++)
    {
        pstDrvAttr->stOutport[i].enOutPort = pstUnfAttr->stOutport[i].enOutPort;
        memcpy(&pstDrvAttr->stOutport[i].unAttr,
               &pstUnfAttr->stOutport[i].unAttr,
               sizeof(HI_UNF_SND_I2S_ATTR_S));
    }
}

static HI_VOID AO_SND_ATTR_DRV2UNF(const DRV_SND_ATTR_S* pstDrvAttr, HI_UNF_SND_ATTR_S* pstUnfAttr)
{
    HI_U32 i = 0;
    pstUnfAttr->u32PortNum              = pstDrvAttr->u32PortNum;
    pstUnfAttr->enSampleRate            = pstDrvAttr->enSampleRate;
    pstUnfAttr->u32MasterOutputBufSize  = pstDrvAttr->u32MasterOutputBufSize;
    pstUnfAttr->u32SlaveOutputBufSize   = pstDrvAttr->u32SlaveOutputBufSize;

    for (i = 0; i < HI_UNF_SND_OUTPUTPORT_MAX; i++)
    {
        pstUnfAttr->stOutport[i].enOutPort = pstDrvAttr->stOutport[i].enOutPort;
        memcpy(&pstUnfAttr->stOutport[i].unAttr,
               &pstDrvAttr->stOutport[i].unAttr,
               sizeof(HI_UNF_SND_I2S_ATTR_S));
    }
}

HI_S32   HI_MPI_AO_SND_GetDefaultOpenAttr(HI_UNF_SND_E enSound, HI_UNF_SND_ATTR_S* pstAttr)
{
    HI_S32 s32Ret;
    AO_SND_OpenDefault_Param_S stDefaultParam;

    CHECK_AO_NULL_PTR(pstAttr);

    stDefaultParam.enSound = enSound;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_GETSNDDEFOPENATTR, &stDefaultParam);
    if (HI_SUCCESS == s32Ret)
    {
        AO_SND_ATTR_DRV2UNF(&stDefaultParam.stAttr, pstAttr);
    }
    else
    {
        memset(pstAttr, 0, sizeof(HI_UNF_SND_ATTR_S));
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_Open(HI_UNF_SND_E enSound, const HI_UNF_SND_ATTR_S* pstAttr)
{
    AO_SND_Open_Param_S stSnd;

    CHECK_AO_NULL_PTR(pstAttr);

    stSnd.enSound = enSound;
    AO_SND_ATTR_UNF2DRV(pstAttr, &stSnd.stAttr);

    return ioctl(g_s32AOFd, CMD_AO_SND_OPEN, &stSnd);
}

HI_S32   HI_MPI_AO_SND_Close(HI_UNF_SND_E enSound)
{
    return ioctl(g_s32AOFd, CMD_AO_SND_CLOSE, &enSound);
}

HI_S32   HI_MPI_AO_SND_SetMute(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bMute)
{
    AO_SND_Mute_Param_S stMute;

    stMute.enSound = enSound;
    stMute.enOutPort = enOutPort;
    stMute.bMute = bMute;

    return ioctl(g_s32AOFd, CMD_AO_SND_SETMUTE, &stMute);
}

HI_S32   HI_MPI_AO_SND_GetMute(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbMute)
{
    HI_S32 s32Ret;
    AO_SND_Mute_Param_S stMute;

    CHECK_AO_NULL_PTR(pbMute);

    stMute.enSound = enSound;
    stMute.enOutPort = enOutPort;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETMUTE, &stMute);
    if (HI_SUCCESS == s32Ret)
    {
        *pbMute = stMute.bMute;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_SetHdmiMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                   HI_UNF_SND_HDMI_MODE_E enHdmiMode)
{
    AO_SND_HdmiMode_Param_S stHdmiMode;

    stHdmiMode.enSound = enSound;
    stHdmiMode.enOutPort = enOutPort;
    stHdmiMode.enMode = enHdmiMode;

    return ioctl(g_s32AOFd, CMD_AO_SND_SETHDMIMODE, &stHdmiMode);
}

HI_S32   HI_MPI_AO_SND_GetHdmiMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                   HI_UNF_SND_HDMI_MODE_E* penHdmiMode)
{
    HI_S32 s32Ret;
    AO_SND_HdmiMode_Param_S stHdmiMode;

    CHECK_AO_NULL_PTR(penHdmiMode);

    stHdmiMode.enSound = enSound;
    stHdmiMode.enOutPort = enOutPort;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETHDMIMODE, &stHdmiMode);
    if (HI_SUCCESS == s32Ret)
    {
        *penHdmiMode = stHdmiMode.enMode;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_SetSpdifMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                    HI_UNF_SND_SPDIF_MODE_E enSpdifMode)
{
    AO_SND_SpdifMode_Param_S stSpdifMode;

    stSpdifMode.enSound = enSound;
    stSpdifMode.enOutPort = enOutPort;
    stSpdifMode.enMode = enSpdifMode;

    return ioctl(g_s32AOFd, CMD_AO_SND_SETSPDIFMODE, &stSpdifMode);
}

HI_S32   HI_MPI_AO_SND_GetSpdifMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                    HI_UNF_SND_SPDIF_MODE_E* penSpdifMode)
{
    HI_S32 s32Ret;
    AO_SND_SpdifMode_Param_S stSpdifMode;

    CHECK_AO_NULL_PTR(penSpdifMode);

    stSpdifMode.enSound = enSound;
    stSpdifMode.enOutPort = enOutPort;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETSPDIFMODE, &stSpdifMode);
    if (HI_SUCCESS == s32Ret)
    {
        *penSpdifMode = stSpdifMode.enMode;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_SetVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                 const HI_UNF_SND_GAIN_ATTR_S* pstGain)
{
    AO_SND_Volume_Param_S stVolume;

    CHECK_AO_NULL_PTR(pstGain);

    stVolume.enSound = enSound;
    stVolume.enOutPort = enOutPort;
    memcpy(&stVolume.stGain, pstGain, sizeof(HI_UNF_SND_GAIN_ATTR_S));

    return ioctl(g_s32AOFd, CMD_AO_SND_SETVOLUME, &stVolume);
}

HI_S32   HI_MPI_AO_SND_GetVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                 HI_UNF_SND_GAIN_ATTR_S* pstGain)
{
    HI_S32 s32Ret;
    AO_SND_Volume_Param_S stVolume;

    CHECK_AO_NULL_PTR(pstGain);

    stVolume.enSound = enSound;
    stVolume.enOutPort = enOutPort;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETVOLUME, &stVolume);
    if (HI_SUCCESS == s32Ret)
    {
        memcpy(pstGain, &stVolume.stGain, sizeof(HI_UNF_SND_GAIN_ATTR_S));
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_SetSpdifCategoryCode(HI_UNF_SND_E enSound,
                                            HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                            HI_UNF_SND_SPDIF_CATEGORYCODE_E enSpdifCategoryCode)
{
    AO_SND_SpdifCategoryCode_Param_S stSpdifCategoryCode;
    stSpdifCategoryCode.enSound = enSound;
    stSpdifCategoryCode.enOutPort  = enOutPort;
    stSpdifCategoryCode.enCategoryCode = enSpdifCategoryCode;

    return ioctl(g_s32AOFd, CMD_AO_SND_SETSPDIFCATEGORYCODE, &stSpdifCategoryCode);
}

HI_S32   HI_MPI_AO_SND_GetSpdifCategoryCode(HI_UNF_SND_E enSound,
                                            HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                            HI_UNF_SND_SPDIF_CATEGORYCODE_E* penSpdifCategoryCode)
{
    HI_S32 s32Ret;
    AO_SND_SpdifCategoryCode_Param_S stSpdifCategoryCode;

    CHECK_AO_NULL_PTR(penSpdifCategoryCode);

    stSpdifCategoryCode.enSound = enSound;
    stSpdifCategoryCode.enOutPort = enOutPort;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETSPDIFCATEGORYCODE, &stSpdifCategoryCode);
    if (HI_SUCCESS == s32Ret)
    {
        *penSpdifCategoryCode = stSpdifCategoryCode.enCategoryCode;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_SetSpdifSCMSMode(HI_UNF_SND_E enSound,
                                        HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                        HI_UNF_SND_SPDIF_SCMSMODE_E enSpdifSCMSMode)
{
    AO_SND_SpdifSCMSMode_Param_S stSpdifSCMSMode;

    stSpdifSCMSMode.enSound = enSound;
    stSpdifSCMSMode.enOutPort  = enOutPort;
    stSpdifSCMSMode.enSCMSMode = enSpdifSCMSMode;

    return ioctl(g_s32AOFd, CMD_AO_SND_SETSPDIFSCMSMODE, &stSpdifSCMSMode);
}

HI_S32   HI_MPI_AO_SND_GetSpdifSCMSMode(HI_UNF_SND_E enSound,
                                        HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                        HI_UNF_SND_SPDIF_SCMSMODE_E* penSpdifSCMSMode)
{
    HI_S32 s32Ret;
    AO_SND_SpdifSCMSMode_Param_S stSpdifSCMSMode;

    CHECK_AO_NULL_PTR(penSpdifSCMSMode);
    stSpdifSCMSMode.enSound = enSound;
    stSpdifSCMSMode.enOutPort = enOutPort;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETSPDIFSCMSMODE, &stSpdifSCMSMode);
    if (HI_SUCCESS == s32Ret)
    {
        *penSpdifSCMSMode = stSpdifSCMSMode.enSCMSMode;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_SetSampleRate(HI_UNF_SND_E enSound, HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    AO_SND_SampleRate_Param_S stSampleRate;

    stSampleRate.enSound = enSound;
    stSampleRate.enOutPort = HI_UNF_SND_OUTPUTPORT_ALL;
    stSampleRate.enSampleRate = enSampleRate;

    return ioctl(g_s32AOFd, CMD_AO_SND_SETSAMPLERATE, &stSampleRate);
}

HI_S32   HI_MPI_AO_SND_GetSampleRate(HI_UNF_SND_E enSound, HI_UNF_SAMPLE_RATE_E* penSampleRate)
{
    HI_S32 s32Ret;
    AO_SND_SampleRate_Param_S stSampleRate;

    CHECK_AO_NULL_PTR(penSampleRate);

    stSampleRate.enSound = enSound;
    stSampleRate.enOutPort = HI_UNF_SND_OUTPUTPORT_ALL;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETSAMPLERATE, &stSampleRate);
    if (HI_SUCCESS == s32Ret)
    {
        *penSampleRate = stSampleRate.enSampleRate;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_SetTrackMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_TRACK_MODE_E enMode)
{
    AO_SND_TrackMode_Param_S stTrackMode;

    stTrackMode.enSound = enSound;
    stTrackMode.enOutPort = enOutPort;
    stTrackMode.enMode = enMode;

    return ioctl(g_s32AOFd, CMD_AO_SND_SETTRACKMODE, &stTrackMode);
}

HI_S32   HI_MPI_AO_SND_GetTrackMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                    HI_UNF_TRACK_MODE_E* penMode)
{
    HI_S32 s32Ret;
    AO_SND_TrackMode_Param_S stTrackMode;

    CHECK_AO_NULL_PTR(penMode);

    stTrackMode.enSound = enSound;
    stTrackMode.enOutPort = enOutPort;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETTRACKMODE, &stTrackMode);
    if (HI_SUCCESS == s32Ret)
    {
        *penMode = stTrackMode.enMode;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_GetXrunCount(HI_UNF_SND_E enSound, HI_U32* pu32Count)
{
    HI_S32 s32Ret;
    AO_SND_Get_Xrun_Param_S stXunParam;

    CHECK_AO_NULL_PTR(pu32Count);
    stXunParam.enSound = enSound;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETXRUNCOUNT, &stXunParam);
    if (HI_SUCCESS == s32Ret)
    {
        *pu32Count = stXunParam.u32Count;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_AllTrack_SetMute(HI_UNF_SND_E enSound, HI_BOOL bMute)
{
    AO_SND_AllTrackMute_Param_S stAllTrackMute;

    stAllTrackMute.enSound = enSound;
    stAllTrackMute.bMute  = bMute;

    return ioctl(g_s32AOFd, CMD_AO_SND_SETALLTRACKMUTE, &stAllTrackMute);
}

HI_S32   HI_MPI_AO_AllTrack_GetMute(HI_UNF_SND_E enSound, HI_BOOL* pbMute)
{
    HI_S32 s32Ret;
    AO_SND_AllTrackMute_Param_S stAllTrackMute;

    CHECK_AO_NULL_PTR(pbMute);

    stAllTrackMute.enSound = enSound;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETALLTRACKMUTE, &stAllTrackMute);
    if (HI_SUCCESS == s32Ret)
    {
        *pbMute = stAllTrackMute.bMute;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_SetLowLatency(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E eOutPort, HI_U32 u32LatecnyMs)
{
    AO_SND_Set_LowLatency_Param_S stLowLatencyParam;

    stLowLatencyParam.enSound = enSound;
    stLowLatencyParam.eOutPort = eOutPort;
    stLowLatencyParam.u32LatencyMs = u32LatecnyMs;

    return ioctl(g_s32AOFd, CMD_AO_SND_SETLOWLATENCY, &stLowLatencyParam);
}

HI_S32  HI_MPI_AO_SND_SetAdacEnable(HI_UNF_SND_E enSound, HI_BOOL bEnable)
{
    AO_SND_Adac_Enable_Param_S stAdacEnable;

    stAdacEnable.enSound = enSound;
    stAdacEnable.bEnable = bEnable;

    return ioctl(g_s32AOFd, CMD_AO_SND_SETADACENABLE, &stAdacEnable);
}

HI_S32  HI_MPI_AO_SND_GetAdacEnable(HI_UNF_SND_E enSound, HI_BOOL* pbEnable)
{
    HI_S32 s32Ret;
    AO_SND_Adac_Enable_Param_S stAdacEnable;

    CHECK_AO_NULL_PTR(pbEnable);

    stAdacEnable.enSound = enSound;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETADACENABLE, &stAdacEnable);
    if (HI_SUCCESS == s32Ret)
    {
        *pbEnable = stAdacEnable.bEnable;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_GetLowLatency(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E eOutPort, HI_U32* pu32LatecnyMs)
{
    HI_S32 s32Ret = HI_FAILURE;
    AO_SND_Set_LowLatency_Param_S stParam;

    CHECK_AO_NULL_PTR(pu32LatecnyMs);

    stParam.enSound = enSound;
    stParam.eOutPort = eOutPort;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETLOWLATENCY, &stParam);
    if (HI_SUCCESS == s32Ret)
    {
        *pu32LatecnyMs = stParam.u32LatencyMs;
    }

    return s32Ret;
}

// create SND Directional Memory Access(DMA)
HI_S32   HI_MPI_AO_SND_DMACreate(HI_VOID)
{
    return SND_DMA_Create();
}

// destory SND Directional Memory Access(DMA)
HI_S32   HI_MPI_AO_SND_DMADestory(HI_VOID)
{
    return SND_DMA_Destory();
}

HI_S32   HI_MPI_AO_SND_DMASendData(const HI_UNF_AO_FRAMEINFO_S* pstAOFrame, const HI_U32 u32LatencyMs)
{
    CHECK_AO_NULL_PTR(pstAOFrame);
    return SND_DMA_SendData(pstAOFrame, u32LatencyMs);
}

HI_S32   HI_MPI_AO_SND_DMAGetDelayMs(HI_U32* pu32DelayMs)
{
    CHECK_AO_NULL_PTR(pu32DelayMs);
    return SND_DMA_GetDelayMs(pu32DelayMs);
}

/******************************* MPI Track for UNF_SND*****************************/
HI_S32   HI_MPI_AO_Track_GetDefaultOpenAttr(HI_UNF_SND_TRACK_TYPE_E enTrackType, HI_UNF_AUDIOTRACK_ATTR_S* pstAttr)
{
    CHECK_AO_NULL_PTR(pstAttr);
    pstAttr->enTrackType = enTrackType;

    return ioctl(g_s32AOFd, CMD_AO_TRACK_GETDEFATTR, pstAttr);
}

HI_S32   HI_MPI_AO_Track_GetAttr(HI_HANDLE hTrack, HI_UNF_AUDIOTRACK_ATTR_S* pstAttr)
{
    HI_S32 s32Ret;
    AO_Track_Attr_Param_S stTrackAttr;

    CHECK_AO_NULL_PTR(pstAttr);
    CHECK_AO_TRACK_ID(hTrack);

    if ((hTrack & AO_TRACK_CHNID_MASK) >= AO_MAX_REAL_TRACK_NUM)
    {
        return VIR_GetAttr(hTrack, pstAttr);
    }

    stTrackAttr.hTrack = hTrack;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_TRACK_GETATTR, &stTrackAttr);
    if (HI_SUCCESS == s32Ret)
    {
        memcpy(pstAttr, &stTrackAttr.stAttr, sizeof(HI_UNF_AUDIOTRACK_ATTR_S));
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_SetAttr(HI_HANDLE hTrack, const HI_UNF_AUDIOTRACK_ATTR_S* pstAttr)
{
    HI_S32 s32Ret;

    CHECK_AO_NULL_PTR(pstAttr);
    CHECK_AO_TRACK_ID(hTrack);

    if ((hTrack & AO_TRACK_CHNID_MASK) >= AO_MAX_REAL_TRACK_NUM)
    {
        return HI_FAILURE;
    }

    AO_Track_Attr_Param_S stTrackAttr;
    stTrackAttr.hTrack = hTrack;
    memcpy(&stTrackAttr.stAttr, pstAttr, sizeof(HI_UNF_AUDIOTRACK_ATTR_S));

    s32Ret =  ioctl(g_s32AOFd, CMD_AO_TRACK_SETATTR, &stTrackAttr);

    return s32Ret;
}

HI_BOOL   HI_MPI_AO_Track_CheckIsLowLatency(HI_HANDLE hTrack)
{
    return LOWLATENCY_CheckIsLowLatencyTrack(hTrack);
}

HI_S32   HI_MPI_AO_Track_Create(HI_UNF_SND_E enSound, const HI_UNF_AUDIOTRACK_ATTR_S* pstAttr, HI_HANDLE* phTrack)
{
    HI_S32 s32Ret;
    HI_HANDLE hTrack;

    CHECK_AO_NULL_PTR(pstAttr);
    CHECK_AO_NULL_PTR(phTrack);

    if (HI_UNF_SND_TRACK_TYPE_VIRTUAL == pstAttr->enTrackType)
    {
        s32Ret = VIR_CreateTrack(pstAttr, &hTrack);
        if (HI_SUCCESS == s32Ret)
        {
            *phTrack = hTrack;
        }
    }
    else
    {
        AO_Track_Create_Param_S stTrackParam;
        stTrackParam.enSound = enSound;
        memcpy(&stTrackParam.stAttr, pstAttr, sizeof(HI_UNF_AUDIOTRACK_ATTR_S));
        //stTrackParam.stBuf  //verify
        stTrackParam.bAlsaTrack = HI_FALSE;

        s32Ret = ioctl(g_s32AOFd, CMD_AO_TRACK_CREATE, &stTrackParam);
        hTrack = stTrackParam.hTrack;

        if (HI_SUCCESS == s32Ret)
        {
#if defined (HI_SMARTVOLUME_SUPPORT)
            AO_CHN_SMARTVOL_S* pstSmartVolume;
            pstSmartVolume = &(g_stSmartVol[hTrack & AO_TRACK_CHNID_MASK]);
            pstSmartVolume->bTrackOpen = HI_TRUE;
#endif
            *phTrack = hTrack;
        }
    }

    if (pstAttr->enTrackType == HI_UNF_SND_TRACK_TYPE_LOWLATENCY)
    {
        s32Ret = LOWLATENCY_Enable(hTrack);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("LOWLATENCY_Enable failed(%x).\n", s32Ret);
        }

        s32Ret = HI_MPI_AO_Track_Start(hTrack);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("HI_MPI_AO_Track_Start failed(%x).\n", s32Ret);
        }
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_Destroy(HI_HANDLE hTrack)
{
    HI_S32 s32Ret;

    CHECK_AO_TRACK_ID(hTrack);

    if ((hTrack & AO_TRACK_CHNID_MASK) >= AO_MAX_REAL_TRACK_NUM)
    {
        return VIR_DestroyTrack(hTrack);
    }

    if (HI_TRUE == LOWLATENCY_CheckIsLowLatencyTrack(hTrack))
    {
        s32Ret = LOWLATENCY_Disable(hTrack);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("LOWLATENCY_Disable failed(0x%x).", s32Ret);
            return s32Ret;
        }
    }

    s32Ret = ioctl(g_s32AOFd, CMD_AO_TRACK_DESTROY, &hTrack);

#if defined (HI_SMARTVOLUME_SUPPORT)
    if (HI_SUCCESS == s32Ret)
    {
        AO_CHN_SMARTVOL_S* pstSmartVolume;
        pstSmartVolume = &(g_stSmartVol[hTrack & AO_TRACK_CHNID_MASK]);
        pstSmartVolume->bTrackOpen = HI_FALSE;

        if (HI_NULL != pstSmartVolume->hSmartVol)
        {
            HI_SmartVolClose(pstSmartVolume->hSmartVol);
            pstSmartVolume->hSmartVol = HI_NULL;
        }
    }
#endif

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_Start(HI_HANDLE hTrack)
{
    HI_S32 s32Ret;
    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);
    s32Ret = ioctl(g_s32AOFd, CMD_AO_TRACK_START, &hTrack);

#if defined (HI_SMARTVOLUME_SUPPORT)
    if (HI_SUCCESS == s32Ret)
    {
        HI_MPI_AO_Track_TriggerSmartVolume(hTrack);
    }
#endif

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_Stop(HI_HANDLE hTrack)
{
    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);
    return ioctl(g_s32AOFd, CMD_AO_TRACK_STOP, &hTrack);
}

HI_S32   HI_MPI_AO_Track_Pause(HI_HANDLE hTrack)
{
    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);
    return ioctl(g_s32AOFd, CMD_AO_TRACK_PAUSE, &hTrack);
}

HI_S32   HI_MPI_AO_Track_Flush(HI_HANDLE hTrack)
{
    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);
    return ioctl(g_s32AOFd, CMD_AO_TRACK_FLUSH, &hTrack);
}

HI_S32   HI_MPI_AO_Track_SetFifoBypass(HI_HANDLE hTrack, HI_BOOL bEnable)
{
    AO_Track_FifoBypass_Param_S stParam;

    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    stParam.hTrack = hTrack;
    stParam.bEnable = bEnable;
    return ioctl(g_s32AOFd, CMD_AO_TRACK_SETFIFOBYPASS, &stParam);
}

HI_S32   HI_MPI_AO_Track_SetPriority(HI_HANDLE hTrack, HI_BOOL bEnable)
{
    AO_Track_Priority_Param_S stParam;

    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    stParam.hTrack = hTrack;
    stParam.bEnable = bEnable;
    return ioctl(g_s32AOFd, CMD_AO_TRACK_SETPRIORITY, &stParam);
}

HI_S32   HI_MPI_AO_Track_GetPriority(HI_HANDLE hTrack, HI_BOOL* pbEnable)
{
    HI_S32 s32Ret;
    AO_Track_Priority_Param_S stTrackPriority;

    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);
    CHECK_AO_NULL_PTR(pbEnable);

    stTrackPriority.hTrack = hTrack;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_TRACK_GETPRIORITY, &stTrackPriority);
    if (HI_SUCCESS == s32Ret)
    {
        *pbEnable = stTrackPriority.bEnable;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_SendData(HI_HANDLE hTrack, const HI_UNF_AO_FRAMEINFO_S* pstAOFrame)
{
    HI_S32 s32Ret;

    CHECK_AO_NULL_PTR(pstAOFrame);
    CHECK_AO_TRACK_ID(hTrack);

    if ((hTrack & AO_TRACK_CHNID_MASK) >= AO_MAX_REAL_TRACK_NUM)
    {
        return VIR_SendData(hTrack, pstAOFrame);
    }

#if defined (HI_SMARTVOLUME_SUPPORT)
    AO_CHN_SMARTVOL_S* pstSmartVolume;
    pstSmartVolume = &(g_stSmartVol[hTrack & AO_TRACK_CHNID_MASK]);

    if ((HI_TRUE == pstSmartVolume->bEnable) &&
        (HI_SUCCESS == pstSmartVolume->s32LastStatus))
    {
        SmartVolumeProcess(pstAOFrame, pstSmartVolume);
    }
#endif

    if (HI_TRUE == LOWLATENCY_CheckIsLowLatencyTrack(hTrack))
    {
        return LOWLATENCY_SendData(hTrack, pstAOFrame);
    }

    AO_Track_SendData_Param_S stTrackAoFrame;
    stTrackAoFrame.hTrack = hTrack;
    AUDIO_UNFFRAME2DRVFRAME(pstAOFrame, &stTrackAoFrame.stAOFrame);

    s32Ret = ioctl(g_s32AOFd, CMD_AO_TRACK_SENDDATA, &stTrackAoFrame);

#if defined (HI_SMARTVOLUME_SUPPORT)
    if (HI_ERR_AO_OUT_BUF_FULL == s32Ret || HI_ERR_AO_PAUSE_STATE == s32Ret)
    {
        pstSmartVolume->s32LastStatus = HI_FAILURE;
    }
    else
    {
        pstSmartVolume->s32LastStatus = HI_SUCCESS;
    }
#endif

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_SetWeight(HI_HANDLE hTrack, const HI_UNF_SND_GAIN_ATTR_S* pstTrackGain)
{
    AO_Track_Weight_Param_S stWeight;

    CHECK_AO_NULL_PTR(pstTrackGain);
    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    stWeight.hTrack = hTrack;
    memcpy(&stWeight.stTrackGain, pstTrackGain, sizeof(HI_UNF_SND_GAIN_ATTR_S));

    return ioctl(g_s32AOFd, CMD_AO_TRACK_SETWEITHT, &stWeight);
}

HI_S32   HI_MPI_AO_Track_GetWeight(HI_HANDLE hTrack, HI_UNF_SND_GAIN_ATTR_S* pstTrackGain)
{
    HI_S32 s32Ret;
    AO_Track_Weight_Param_S stWeight;

    CHECK_AO_NULL_PTR(pstTrackGain);
    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    stWeight.hTrack = hTrack;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_TRACK_GETWEITHT, &stWeight);
    if (HI_SUCCESS == s32Ret)
    {
        memcpy(pstTrackGain, &stWeight.stTrackGain, sizeof(HI_UNF_SND_GAIN_ATTR_S));
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_SetAbsWeight(HI_HANDLE hTrack, const HI_UNF_SND_ABSGAIN_ATTR_S* pstAbsWeightGain)
{
    AO_Track_AbsGain_Param_S stAbsGain;

    CHECK_AO_NULL_PTR(pstAbsWeightGain);
    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    stAbsGain.hTrack = hTrack;
    memcpy(&stAbsGain.stTrackAbsGain, pstAbsWeightGain, sizeof(HI_UNF_SND_ABSGAIN_ATTR_S));

    return ioctl(g_s32AOFd, CMD_AO_TRACK_SETABSGAIN, &stAbsGain);
}

HI_S32   HI_MPI_AO_Track_GetAbsWeight(HI_HANDLE hTrack, HI_UNF_SND_ABSGAIN_ATTR_S* pstAbsWeightGain)
{
    HI_S32 s32Ret;
    AO_Track_AbsGain_Param_S stAbsGain;

    CHECK_AO_NULL_PTR(pstAbsWeightGain);
    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    stAbsGain.hTrack = hTrack;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_TRACK_GETABSGAIN, &stAbsGain);
    if (HI_SUCCESS == s32Ret)
    {
        memcpy(pstAbsWeightGain, &stAbsGain.stTrackAbsGain, sizeof(HI_UNF_SND_ABSGAIN_ATTR_S));
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_SetMute(HI_HANDLE hTrack, HI_BOOL bMute)
{
    AO_Track_Mute_Param_S stMute;

    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    stMute.hTrack = hTrack;
    stMute.bMute  = bMute;

    return ioctl(g_s32AOFd, CMD_AO_TRACK_SETMUTE, &stMute);
}

HI_S32   HI_MPI_AO_Track_GetMute(HI_HANDLE hTrack, HI_BOOL* pbMute)
{
    HI_S32 s32Ret;
    AO_Track_Mute_Param_S stMute;

    CHECK_AO_NULL_PTR(pbMute);
    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    stMute.hTrack = hTrack;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_TRACK_GETMUTE, &stMute);
    if (HI_SUCCESS == s32Ret)
    {
        *pbMute = stMute.bMute;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_SetChannelMode(HI_HANDLE hTrack, HI_UNF_TRACK_MODE_E enMode)
{
    AO_Track_ChannelMode_Param_S stTrackMode;

    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    stTrackMode.hTrack = hTrack;
    stTrackMode.enMode = enMode;

    return ioctl(g_s32AOFd, CMD_AO_TRACK_SETCHANNELMODE, &stTrackMode);
}

HI_S32   HI_MPI_AO_Track_GetChannelMode(HI_HANDLE hTrack, HI_UNF_TRACK_MODE_E* penMode)
{
    HI_S32 s32Ret;
    AO_Track_ChannelMode_Param_S stTrackMode;

    CHECK_AO_NULL_PTR(penMode);
    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    stTrackMode.hTrack = hTrack;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_TRACK_GETCHANNELMODE, &stTrackMode);
    if (HI_SUCCESS == s32Ret)
    {
        *penMode = stTrackMode.enMode;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_AcquireFrame(HI_HANDLE hTrack, HI_UNF_AO_FRAMEINFO_S* pstAOFrame, HI_U32 u32TimeoutMs)
{
    HI_U32 u32SleepCnt;
    HI_S32 s32Ret = HI_FAILURE;

    CHECK_AO_NULL_PTR(pstAOFrame);
    CHECK_AO_TRACK_ID(hTrack);
    CHECK_REAL_Track(hTrack);

    s32Ret = VIR_AcquireFrame(hTrack, pstAOFrame);
    if (HI_ERR_AO_VIRTUALBUF_EMPTY == s32Ret)
    {
        for (u32SleepCnt = 0; u32SleepCnt < u32TimeoutMs; u32SleepCnt++)
        {
            (HI_VOID)usleep(1 * 1000);
            s32Ret = VIR_AcquireFrame(hTrack, pstAOFrame);
            if (HI_ERR_AO_VIRTUALBUF_EMPTY != s32Ret)
            {
                break;
            }
        }
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_ReleaseFrame(HI_HANDLE hTrack, HI_UNF_AO_FRAMEINFO_S* pstAOFrame)
{
    CHECK_AO_NULL_PTR(pstAOFrame);
    CHECK_AO_TRACK_ID(hTrack);
    CHECK_REAL_Track(hTrack);

    return VIR_ReleaseFrame(hTrack, pstAOFrame);
}

HI_S32   HI_MPI_AO_Track_SetEosFlag(HI_HANDLE hTrack, HI_BOOL bEosFlag)
{
    AO_Track_EosFlag_Param_S stEosFlag;

    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    stEosFlag.hTrack = hTrack;
    stEosFlag.bEosFlag = bEosFlag;

    return ioctl(g_s32AOFd, CMD_AO_TRACK_SETEOSFLAG, &stEosFlag);
}

HI_S32   HI_MPI_AO_Track_SetSpeedAdjust(HI_HANDLE hTrack, HI_S32 s32Speed, HI_MPI_SND_SPEEDADJUST_TYPE_E enType)
{
    AO_Track_SpeedAdjust_Param_S stSpeedAdjust;

    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);

    stSpeedAdjust.hTrack = hTrack;
    stSpeedAdjust.enType = (AO_SND_SPEEDADJUST_TYPE_E)enType;
    stSpeedAdjust.s32Speed = s32Speed;

    return ioctl(g_s32AOFd, CMD_AO_TRACK_SETSPEEDADJUST, &stSpeedAdjust);
}

HI_S32   HI_MPI_AO_Track_GetDelayMs(const HI_HANDLE hTrack, HI_U32* pDelayMs)
{
    HI_S32 s32Ret;
    AO_Track_DelayMs_Param_S stDelayMs;

    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);
    CHECK_AO_NULL_PTR(pDelayMs);

    stDelayMs.hTrack = hTrack;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_TRACK_GETDELAYMS, &stDelayMs);
    if (HI_SUCCESS == s32Ret)
    {
        *pDelayMs = stDelayMs.u32DelayMs;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_GetBufDelayMs(const HI_HANDLE hTrack, HI_U32* pDelayMs)
{
    HI_S32 s32Ret;

    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);
    CHECK_AO_NULL_PTR(pDelayMs);

    if (HI_TRUE == LOWLATENCY_CheckIsLowLatencyTrack(hTrack))
    {
        s32Ret = LOWLATENCY_GetAIPDelayMs(hTrack, pDelayMs);
    }
    else
    {
        HI_ERR_AO("HI_MPI_AO_Track_GetBufDelayMs only support lowlatency track\n");
        s32Ret = HI_ERR_AO_NOTSUPPORT;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_Track_IsBufEmpty(const HI_HANDLE hTrack, HI_BOOL* pbEmpty)
{
    HI_S32 s32Ret;
    AO_Track_BufEmpty_Param_S stEmpty;

    CHECK_AO_TRACK_ID(hTrack);
    CHECK_VIRTUAL_Track(hTrack);
    CHECK_AO_NULL_PTR(pbEmpty);

    stEmpty.hTrack = hTrack;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_TRACK_ISBUFEMPTY, &stEmpty);
    if (HI_SUCCESS == s32Ret)
    {
        *pbEmpty = stEmpty.bEmpty;
    }

    return s32Ret;
}

HI_S32 HI_MPI_AO_Track_AttachAi(const HI_HANDLE hAi, const HI_HANDLE hTrack)
{
    AO_Track_AttAi_Param_S stTrackAttAi;

    CHECK_AO_TRACK_ID(hTrack);

    stTrackAttAi.hTrack = hTrack;
    stTrackAttAi.hAi = hAi;

    return ioctl(g_s32AOFd, CMD_AO_TRACK_ATTACHAI, &stTrackAttAi);
}

HI_S32 HI_MPI_AO_Track_DetachAi(const HI_HANDLE hAi, const HI_HANDLE hTrack)
{
    AO_Track_AttAi_Param_S stTrackAttAi;

    CHECK_AO_TRACK_ID(hTrack);

    stTrackAttAi.hTrack = hTrack;
    stTrackAttAi.hAi = hAi;

    return ioctl(g_s32AOFd, CMD_AO_TRACK_DETACHAI, &stTrackAttAi);
}

HI_S32   HI_MPI_AO_SND_GetCastDefaultOpenAttr(HI_UNF_SND_CAST_ATTR_S* pstAttr)
{
    CHECK_AO_NULL_PTR(pstAttr);

    return ioctl(g_s32AOFd, CMD_AO_CAST_GETDEFATTR, pstAttr);
}

HI_S32   HI_MPI_AO_SND_CreateCast(HI_UNF_SND_E enSound, HI_UNF_SND_CAST_ATTR_S* pstCastAttr, HI_HANDLE* phCast)
{
    HI_S32 s32Ret;
    AO_Cast_Create_Param_S stCastParam;
    AO_Cast_Info_Param_S stCastInfo;
    HI_VOID* pUserVirtAddr = HI_NULL;

    CHECK_AO_NULL_PTR(phCast);
    CHECK_AO_NULL_PTR(pstCastAttr);

    stCastParam.enSound = enSound;
    memcpy(&stCastParam.stCastAttr, pstCastAttr, sizeof(HI_UNF_SND_CAST_ATTR_S));

    s32Ret = ioctl(g_s32AOFd, CMD_AO_CAST_CREATE, &stCastParam);
    if (HI_SUCCESS == s32Ret)
    {
        *phCast = stCastParam.hCast;
    }
    else
    {
        HI_ERR_AO("ioctl CMD_AO_CAST_CREATE failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    stCastInfo.hCast = stCastParam.hCast;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_CAST_GETINFO, &stCastInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("ioctl CMD_AO_CAST_GETINFO failed(0x%x)\n", s32Ret);
        goto ERR_CREAT;
    }

    if (!stCastInfo.tPhyAddr)
    {
        HI_ERR_AO("ERROE Phy addr =0x%x \n", stCastInfo.tPhyAddr);
        goto ERR_CREAT;
    }

    pUserVirtAddr = HI_MEM_Map(stCastInfo.tPhyAddr, stCastParam.u32ReqSize);
    if (HI_NULL == pUserVirtAddr)
    {
        HI_ERR_AO("u32PhyAddr(0x%x) HI_MEM_Map Failed \n", stCastInfo.tPhyAddr);
        goto ERR_CREAT;
    }

    stCastInfo.tUserVirtAddr = (HI_U8*)pUserVirtAddr - (HI_U8*)HI_NULL;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_CAST_SETINFO, &stCastInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("ioctl CMD_AO_CAST_SETINFO failed(0x%x)\n", s32Ret);
        goto ERR_MMAP;
    }

    return s32Ret;

ERR_MMAP:
    (HI_VOID)HI_MEM_Unmap(pUserVirtAddr);
ERR_CREAT:
    (HI_VOID)ioctl(g_s32AOFd, CMD_AO_CAST_DESTROY, &stCastInfo.hCast);

    return HI_FAILURE;
}

HI_S32   HI_MPI_AO_SND_DestroyCast(HI_HANDLE hCast)
{
    HI_S32 s32Ret;
    AO_Cast_Info_Param_S stCastInfo;

    stCastInfo.hCast = hCast;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_CAST_GETINFO, &stCastInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("ioctl CMD_AO_CAST_GETINFO failed(0x%x)\n", s32Ret);
    }
    else
    {
        (HI_VOID)HI_MEM_Unmap((HI_VOID*)((HI_U8*)HI_NULL + stCastInfo.tUserVirtAddr));
    }

    return ioctl(g_s32AOFd, CMD_AO_CAST_DESTROY, &hCast);
}

HI_S32   HI_MPI_AO_SND_SetCastEnable(HI_HANDLE hCast, HI_BOOL bEnable)
{
    HI_S32 s32Ret;
    AO_Cast_Enable_Param_S stEnableAttr;

    stEnableAttr.hCast = hCast;
    stEnableAttr.bCastEnable = bEnable;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_CAST_SETENABLE, &stEnableAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("ioctl CMD_AO_CAST_SETENABLE failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_GetCastEnable(HI_HANDLE hCast, HI_BOOL* pbEnable)
{
    HI_S32 s32Ret;
    AO_Cast_Enable_Param_S stEnableAttr;

    CHECK_AO_NULL_PTR(pbEnable);

    stEnableAttr.hCast = hCast;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_CAST_GETENABLE, &stEnableAttr);
    if (HI_SUCCESS == s32Ret)
    {
        *pbEnable = stEnableAttr.bCastEnable;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_Cast_SetMute(HI_HANDLE hCast, HI_BOOL bMute)
{
    AO_Cast_Mute_Param_S stMute;
    stMute.hCast = hCast;
    stMute.bMute  = bMute;
    return ioctl(g_s32AOFd, CMD_AO_CAST_SETMUTE, &stMute);
}

HI_S32   HI_MPI_AO_Cast_GetMute(HI_HANDLE hCast, HI_BOOL* pbMute)
{
    HI_S32 s32Ret;
    AO_Cast_Mute_Param_S stMute;
    CHECK_AO_NULL_PTR(pbMute);

    stMute.hCast = hCast;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_CAST_GETMUTE, &stMute);
    if (HI_SUCCESS == s32Ret)
    {
        *pbMute = stMute.bMute;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_SetCastAbsWeight(HI_HANDLE hCast, const HI_UNF_SND_ABSGAIN_ATTR_S* pstAbsWeightGain)
{
    CHECK_AO_NULL_PTR(pstAbsWeightGain);

    AO_Cast_AbsGain_Param_S stAbsGain;
    stAbsGain.hCast = hCast;
    memcpy(&stAbsGain.stCastAbsGain, pstAbsWeightGain, sizeof(HI_UNF_SND_ABSGAIN_ATTR_S));

    return ioctl(g_s32AOFd, CMD_AO_CAST_SETABSGAIN, &stAbsGain);
}

HI_S32   HI_MPI_AO_SND_GetCastAbsWeight(HI_HANDLE hCast, HI_UNF_SND_ABSGAIN_ATTR_S* pstAbsWeightGain)
{
    HI_S32 s32Ret;
    AO_Cast_AbsGain_Param_S stAbsGain;
    CHECK_AO_NULL_PTR(pstAbsWeightGain);

    stAbsGain.hCast = hCast;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_CAST_GETABSGAIN, &stAbsGain);
    if (HI_SUCCESS == s32Ret)
    {
        memcpy(pstAbsWeightGain, &stAbsGain.stCastAbsGain, sizeof(HI_UNF_SND_ABSGAIN_ATTR_S));
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_AcquireCastFrame(HI_HANDLE hCast, HI_UNF_AO_FRAMEINFO_S* pstCastFrame)
{
    HI_S32 s32Ret;
    AO_Cast_Info_Param_S stCastInfo;
    AO_Cast_Data_Param_S stCastData;

    CHECK_AO_NULL_PTR(pstCastFrame);

    stCastInfo.hCast = hCast;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_CAST_GETINFO, &stCastInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("ioctl CMD_AO_CAST_GETINFO failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    stCastData.hCast = hCast;
    stCastData.u32FrameBytes = stCastInfo.u32FrameBytes;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_CAST_ACQUIREFRAME, &stCastData);
    if (HI_SUCCESS != s32Ret)
    {
        pstCastFrame->u32PcmSamplesPerFrame = 0;
        return s32Ret;
    }

    AUDIO_DRVFRAME2UNFFRAME(&stCastData.stAOFrame, pstCastFrame);
    pstCastFrame->ps32PcmBuffer = (HI_S32*)((HI_U8*)HI_NULL + stCastInfo.tUserVirtAddr + stCastData.u32DataOffset);

    if (0 == pstCastFrame->u32PcmSamplesPerFrame)
    {
        return HI_ERR_AO_CAST_TIMEOUT;
    }

    return HI_SUCCESS;
}

HI_S32   HI_MPI_AO_SND_ReleaseCastFrame(HI_HANDLE hCast, HI_UNF_AO_FRAMEINFO_S* pstCastFrame)
{
    HI_S32 s32Ret;
    AO_Cast_Info_Param_S stCastInfo;
    AO_Cast_Data_Param_S stCastData;

    CHECK_AO_NULL_PTR(pstCastFrame);

    if (0 == pstCastFrame->u32PcmSamplesPerFrame)
    {
        return  HI_SUCCESS;
    }

    stCastInfo.hCast = hCast;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_CAST_GETINFO, &stCastInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("ioctl CMD_AO_CAST_GETINFO failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    if (pstCastFrame->u32PcmSamplesPerFrame != stCastInfo.u32FrameSamples ||
        pstCastFrame->u32Channels != stCastInfo.u32Channels               ||
        pstCastFrame->s32BitPerSample != stCastInfo.s32BitPerSample)
    {
        HI_ERR_AO("Release Cast Frame Error: u32PcmSamplesPerFrame(0x%x) u32Channels(0x%x) u32SampleRate(0x%x)\n" ,
            pstCastFrame->u32PcmSamplesPerFrame, pstCastFrame->u32Channels, pstCastFrame->u32SampleRate);
        return HI_FAILURE;
    }

    stCastData.hCast = hCast;
    stCastData.u32FrameBytes = stCastInfo.u32FrameBytes;
    AUDIO_UNFFRAME2DRVFRAME(pstCastFrame, &stCastData.stAOFrame);

    s32Ret = ioctl(g_s32AOFd, CMD_AO_CAST_RELEASEFRAME, &stCastData);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("ioctl CMD_AO_CAST_RELEASEFRAME failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_AttachAef(HI_UNF_SND_E enSound, HI_U32 u32AefId, AO_AEF_ATTR_S* pstAefAttr, HI_U32* pu32AefProcAddr)
{
    HI_S32 s32Ret;
    AO_SND_AttAef_Param_S stSndAttAef;

    CHECK_AO_NULL_PTR(pu32AefProcAddr);
    CHECK_AO_NULL_PTR(pstAefAttr);

    stSndAttAef.enSound  = enSound;
    stSndAttAef.u32AefId = u32AefId;
    memcpy(&stSndAttAef.stAefAttr, pstAefAttr, sizeof(AO_AEF_ATTR_S));

    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_ATTACHAEF, &stSndAttAef);
    if (HI_SUCCESS == s32Ret)
    {
        *pu32AefProcAddr = stSndAttAef.u32AefProcAddr;
    }

    return s32Ret;
}

HI_S32   HI_MPI_AO_SND_DetachAef(HI_UNF_SND_E enSound, HI_U32 u32AefId, AO_AEF_ATTR_S* pstAefAttr)
{
    AO_SND_AttAef_Param_S stSndAttAef;

    CHECK_AO_NULL_PTR(pstAefAttr);

    stSndAttAef.enSound  = enSound;
    stSndAttAef.u32AefId = u32AefId;
    memcpy(&stSndAttAef.stAefAttr, pstAefAttr, sizeof(AO_AEF_ATTR_S));

    return ioctl(g_s32AOFd, CMD_AO_SND_DETACHAEF, &stSndAttAef);
}

HI_S32  HI_MPI_AO_SND_SetAefBypass(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bBypass)
{
    AO_SND_AefBypass_Param_S stAefBypass;

    stAefBypass.enSound = enSound;
    stAefBypass.enOutPort = enOutPort;
    stAefBypass.bBypass = bBypass;

    return ioctl(g_s32AOFd, CMD_AO_SND_SETAEFBYPASS, &stAefBypass);
}

HI_S32  HI_MPI_AO_SND_GetAefBypass(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbBypass)
{
    HI_S32 s32Ret;
    AO_SND_AefBypass_Param_S stAefBypass;

    CHECK_AO_NULL_PTR(pbBypass);

    stAefBypass.enSound = enSound;
    stAefBypass.enOutPort = enOutPort;

    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETAEFBYPASS, &stAefBypass);
    if (HI_SUCCESS == s32Ret)
    {
        *pbBypass = stAefBypass.bBypass;
    }

    return s32Ret;
}

#if defined (HI_SOUND_SPDIF_COMPENSATION_SUPPORT)
HI_S32   HI_MPI_AO_SND_SetDelayCompensationMs(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32 u32DelayMs)
{
    AO_SND_OpDelay_Param_S stOpDelay;

    stOpDelay.enSound = enSound;
    stOpDelay.enOutPort = enOutPort;
    stOpDelay.u32DelayMs = u32DelayMs;
    return ioctl(g_s32AOFd, CMD_AO_SND_SETSOUNDDELAY, &stOpDelay);
}

HI_S32   HI_MPI_AO_SND_GetDelayCompensationMs(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32* pu32DelayMs)
{
    HI_S32 s32Ret;
    AO_SND_OpDelay_Param_S stOpDelay;

    CHECK_AO_NULL_PTR(pu32DelayMs);

    stOpDelay.enSound = enSound;
    stOpDelay.enOutPort = enOutPort;
    s32Ret = ioctl(g_s32AOFd, CMD_AO_SND_GETSOUNDDELAY, &stOpDelay);
    if(HI_SUCCESS == s32Ret)
    {
        *pu32DelayMs = stOpDelay.u32DelayMs;
    }
    return s32Ret;
}
#endif

HI_S32   HI_MPI_AO_SND_SetExtDelayMs(HI_UNF_SND_E enSound, HI_U32 u32Delay)
{
    AO_SND_Set_ExtDelayMs_Param_S stExtDelayMsParam;

    stExtDelayMsParam.enSound = enSound;
    stExtDelayMsParam.u32DelayMs = u32Delay;

    return ioctl(g_s32AOFd, CMD_AO_SND_SETEXTDELAYMS, &stExtDelayMsParam);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
