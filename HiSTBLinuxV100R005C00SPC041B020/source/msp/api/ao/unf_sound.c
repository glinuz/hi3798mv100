#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "hi_error_mpi.h"
#include "hi_unf_sound.h"

#include "hi_mpi_ao.h"

#include "hi_drv_ao.h"
#include "hi_mpi_avplay.h"

#ifdef __DPT__
#include "mpi_aef.h"
#endif

#if defined (HI_SOUND_AI_SUPPORT)
#include "hi_mpi_ai.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/***************************** Macro Definition ******************************/
#define API_SND_CheckId(u32SndId) \
    do{ \
        if (HI_UNF_SND_BUTT <= u32SndId) \
        { \
            HI_ERR_AO("Sound ID(%#x) is Invalid!\n", u32SndId); \
            return HI_ERR_AO_INVALID_ID; \
        } \
    } while(0)

/******************************* API declaration *****************************/
HI_S32 HI_UNF_SND_Init(HI_VOID)
{
    return HI_MPI_AO_Init();
}

HI_S32 HI_UNF_SND_DeInit(HI_VOID)
{
    return HI_MPI_AO_DeInit();
}

HI_S32   HI_UNF_SND_SendTrackData(HI_HANDLE hTrack, const HI_UNF_AO_FRAMEINFO_S* pstAOFrame)
{
    HI_S32 s32Ret;

    if (HI_FALSE == HI_MPI_AO_Track_CheckIsLowLatency(hTrack)) //low latency track do not need start here
    {
        s32Ret = HI_MPI_AO_Track_Start(hTrack);
        if (s32Ret != HI_SUCCESS)
        {
            return s32Ret;
        }
    }

    s32Ret = HI_MPI_AO_Track_SendData(hTrack, pstAOFrame);
    if (HI_ERR_AO_OUT_BUF_FULL == s32Ret)
    {
        return s32Ret;
    }
    else if (s32Ret != HI_SUCCESS)
    {
        return HI_ERR_AO_INVALID_PARA;
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_GetTrackDelayMs(const HI_HANDLE hTrack, HI_U32* pDelayMs)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_GetDelayMs(hTrack, pDelayMs);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_AO("Get Track(%d) DelayMs failed(0x%x)\n", hTrack, s32Ret);
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_GetDefaultOpenAttr(HI_UNF_SND_E enSound, HI_UNF_SND_ATTR_S* pstAttr)
{
    HI_S32 s32Ret;

    API_SND_CheckId(enSound);

    s32Ret = HI_MPI_AO_SND_GetDefaultOpenAttr(enSound, pstAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("HI_UNF_SND_GetDefaultOpenAttr failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_Open(HI_UNF_SND_E enSound, const HI_UNF_SND_ATTR_S* pstAttr)
{
    HI_S32 s32Ret;

    API_SND_CheckId(enSound);

    s32Ret = HI_MPI_AO_SND_Open(enSound, pstAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("HI_MPI_AO_SND_Open failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_Close(HI_UNF_SND_E enSound)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_Close(enSound);
}

HI_S32  HI_UNF_SND_SetMute(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bMute)
{
    HI_S32 s32Ret;

    API_SND_CheckId(enSound);

    s32Ret = HI_MPI_AO_SND_SetMute(enSound, enOutPort, bMute);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("HI_MPI_AO_SND_SetMute failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_GetMute(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbMute)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_GetMute(enSound, enOutPort, pbMute);
}

HI_S32 HI_UNF_SND_SetVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, const HI_UNF_SND_GAIN_ATTR_S* pstGain)
{
    HI_S32 s32Ret;

    API_SND_CheckId(enSound);

    s32Ret = HI_MPI_AO_SND_SetVolume(enSound, enOutPort, pstGain);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("HI_MPI_AO_SND_SetVolume failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_GetVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S* pstGain)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_GetVolume(enSound, enOutPort, pstGain);
}

HI_S32   HI_UNF_SND_SetSpdifCategoryCode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                         HI_UNF_SND_SPDIF_CATEGORYCODE_E enSpdifCategoryCode)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_SetSpdifCategoryCode(enSound, enOutPort, enSpdifCategoryCode);
}

HI_S32   HI_UNF_SND_GetSpdifCategoryCode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                         HI_UNF_SND_SPDIF_CATEGORYCODE_E* penSpdifCategoryCode)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_GetSpdifCategoryCode(enSound, enOutPort, penSpdifCategoryCode);
}

HI_S32   HI_UNF_SND_SetSpdifSCMSMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                     HI_UNF_SND_SPDIF_SCMSMODE_E enSpdifSCMSMode)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_SetSpdifSCMSMode(enSound, enOutPort, enSpdifSCMSMode);
}

HI_S32   HI_UNF_SND_GetSpdifSCMSMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                     HI_UNF_SND_SPDIF_SCMSMODE_E* penSpdifSCMSMode)
{
    API_SND_CheckId(enSound);

    return HI_MPI_AO_SND_GetSpdifSCMSMode(enSound, enOutPort, penSpdifSCMSMode);
}

HI_S32 HI_UNF_SND_SetSampleRate(HI_UNF_SND_E enSound, HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    HI_S32 s32Ret;

    API_SND_CheckId(enSound);

    s32Ret = HI_MPI_AO_SND_SetSampleRate(enSound, enSampleRate);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("HI_MPI_AO_SND_SetSampleRate failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_GetSampleRate(HI_UNF_SND_E enSound, HI_UNF_SAMPLE_RATE_E* penSampleRate)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_GetSampleRate(enSound, penSampleRate);
}

HI_S32 HI_UNF_SND_SetSmartVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bSmartVolume)
{
    API_SND_CheckId(enSound);
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32 HI_UNF_SND_GetSmartVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbSmartVolume)
{
    API_SND_CheckId(enSound);
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32 HI_UNF_SND_SetTrackSmartVolume(HI_HANDLE hTrack, HI_BOOL bEnable)
{
#if defined (HI_SMARTVOLUME_SUPPORT)
    HI_S32 s32Ret;
    s32Ret = HI_MPI_AO_Track_SetSmartVolume(hTrack, bEnable);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_AO("Set Track(%d) Smart volume failed.\n", hTrack);
    }

    return s32Ret;
#else
    HI_ERR_AO("Do not Support, should enable 'SND Smart Volume Support' at make menuconfig.\n");
    return HI_ERR_AO_NOTSUPPORT;
#endif
}

HI_S32 HI_UNF_SND_GetTrackSmartVolume(HI_HANDLE hTrack, HI_BOOL* pbEnable)
{
#if defined (HI_SMARTVOLUME_SUPPORT)
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_GetSmartVolume(hTrack, pbEnable);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_AO("Get Track(%d) Smart volume failed.\n", hTrack);
    }

    return s32Ret;
#else
    HI_ERR_AO("Do not Support, should enable 'SND Smart Volume Support' at make menuconfig.\n");
    return HI_ERR_AO_NOTSUPPORT;
#endif
}

HI_S32 HI_UNF_SND_SetTrackMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_TRACK_MODE_E enMode)
{
    HI_S32 s32Ret;

    API_SND_CheckId(enSound);

    s32Ret = HI_MPI_AO_SND_SetTrackMode(enSound, enOutPort, enMode);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("set AO TrackMode to %d failed(0x%x)\n", enMode, s32Ret);
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_GetTrackMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_TRACK_MODE_E* penMode)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_GetTrackMode(enSound, enOutPort, penMode);
}

HI_S32  HI_UNF_SND_SetAllTrackMute(HI_UNF_SND_E enSound, HI_BOOL bMute)
{
    HI_S32 s32Ret;

    API_SND_CheckId(enSound);

    s32Ret = HI_MPI_AO_AllTrack_SetMute(enSound, bMute);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_AO("HI_MPI_AO_AllTrack_SetMute failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

HI_S32  HI_UNF_SND_GetAllTrackMute(HI_UNF_SND_E enSound, HI_BOOL* pbMute)
{
    HI_S32 s32Ret;

    API_SND_CheckId(enSound);

    s32Ret = HI_MPI_AO_AllTrack_GetMute(enSound, pbMute);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_AO("HI_MPI_AO_AllTrack_GetMute failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

HI_S32  HI_UNF_SND_SetAdacEnable(HI_UNF_SND_E enSound,  HI_BOOL bEnable)
{
    HI_S32 s32Ret;

    API_SND_CheckId(enSound);

    s32Ret = HI_MPI_AO_SND_SetAdacEnable(enSound, bEnable);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("HI_UNF_SND_SetAdacEnable failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_SetPrecisionVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, const HI_UNF_SND_PRECIGAIN_ATTR_S* pstPreciGain)
{
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32 HI_UNF_SND_GetPrecisionVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_PRECIGAIN_ATTR_S* pstPreciGain)
{
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32 HI_UNF_SND_SetBalance(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_S32 s32Balance)
{
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32 HI_UNF_SND_GetBalance(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_S32* ps32Balance)
{
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32   HI_UNF_SND_GetDefaultCastAttr(HI_UNF_SND_E enSound, HI_UNF_SND_CAST_ATTR_S* pstAttr)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_GetCastDefaultOpenAttr(pstAttr);
}

HI_S32 HI_UNF_SND_CreateCast(HI_UNF_SND_E enSound, HI_UNF_SND_CAST_ATTR_S* pstAttr, HI_HANDLE* phCast)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_CreateCast(enSound, pstAttr, phCast);
}

HI_S32 HI_UNF_SND_DestroyCast(HI_HANDLE hCast)
{
    return HI_MPI_AO_SND_DestroyCast(hCast);
}

HI_S32 HI_UNF_SND_SetCastEnable(HI_HANDLE hCast, HI_BOOL bEnable)
{
    return HI_MPI_AO_SND_SetCastEnable(hCast, bEnable);
}

HI_S32 HI_UNF_SND_GetCastEnable(HI_HANDLE hCast, HI_BOOL* pbEnable)
{
    return HI_MPI_AO_SND_GetCastEnable(hCast, pbEnable);
}

HI_S32 HI_UNF_SND_AcquireCastFrame(HI_HANDLE hCast, HI_UNF_AO_FRAMEINFO_S* pstCastFrame, HI_U32 u32TimeoutMs)
{
    HI_S32 s32Ret;
    HI_U32 u32SleepCnt;

    s32Ret = HI_MPI_AO_SND_AcquireCastFrame(hCast, pstCastFrame);
    if (HI_ERR_AO_CAST_TIMEOUT != s32Ret)
    {
        return s32Ret;
    }

    for (u32SleepCnt = 0; u32SleepCnt < u32TimeoutMs; u32SleepCnt++)
    {
        (HI_VOID)usleep(1000);

        s32Ret = HI_MPI_AO_SND_AcquireCastFrame(hCast, pstCastFrame);
        if (HI_ERR_AO_CAST_TIMEOUT != s32Ret)
        {
            return s32Ret;
        }
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_ReleaseCastFrame(HI_HANDLE hCast, HI_UNF_AO_FRAMEINFO_S* pstCastFrame)
{
    return HI_MPI_AO_SND_ReleaseCastFrame(hCast, pstCastFrame);
}

HI_S32   HI_UNF_SND_SetCastMute(HI_HANDLE hCast, HI_BOOL bMute)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Cast_SetMute(hCast, bMute);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_AO("Set Cast(%d) Mute failed.\n", hCast);
    }

    return s32Ret;
}

HI_S32   HI_UNF_SND_GetCastMute(HI_HANDLE hCast, HI_BOOL* pbMute)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Cast_GetMute(hCast, pbMute);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_AO("Get Cast(%d) Mute failed.\n", hCast);
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_SetCastAbsWeight(HI_HANDLE hCast, const HI_UNF_SND_ABSGAIN_ATTR_S* pstAbsWeightGain)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_SND_SetCastAbsWeight(hCast, pstAbsWeightGain);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_AO("Set Cast(%d) AbsWeight failed.\n", hCast);
    }

    return s32Ret;
}

HI_S32  HI_UNF_SND_GetCastAbsWeight(HI_HANDLE hCast,  HI_UNF_SND_ABSGAIN_ATTR_S* pstAbsWeightGain)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_SND_GetCastAbsWeight(hCast, pstAbsWeightGain);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_AO("Get CAST(%d) AbsWeight failed.\n", hCast);
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_Attach(HI_HANDLE hTrack, HI_HANDLE hSource)
{
    HI_S32 s32Ret;

    CHECK_AO_TRACK_ID(hTrack);

    if (HI_ID_AVPLAY == (hSource >> 16))
    {
        s32Ret = HI_MPI_AVPLAY_AttachSnd(hSource, hTrack);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_AO("call HI_MPI_AVPLAY_AttachSnd failed(0x%x)\n", s32Ret);
            return s32Ret;
        }

        s32Ret = HI_MPI_AO_Track_Start(hTrack);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_AO("call HI_MPI_AO_Track_Start failed(0x%x)\n", s32Ret);
            return s32Ret;
        }
    }

#if defined (HI_SOUND_AI_SUPPORT)
    else if (HI_ID_AI == (hSource >> 16))
    {
        s32Ret = HI_MPI_AI_Attach(hSource, hTrack);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_AO("call HI_MPI_AI_Attach failed(0x%x)\n", s32Ret);
            return s32Ret;
        }
    }
#endif
    else
    {
        HI_ERR_AO("Invalid hsoure(0x%x)\n", hSource);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_SND_Detach(HI_HANDLE hTrack, HI_HANDLE hSource)
{
    HI_S32 s32Ret;

    CHECK_AO_TRACK_ID(hTrack);

    if (HI_ID_AVPLAY == (hSource >> 16))
    {
        s32Ret = HI_MPI_AVPLAY_DetachSnd(hSource, hTrack);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("call HI_MPI_AVPLAY_DetachSnd failed(0x%x)\n", s32Ret);
            return s32Ret;
        }
    }

#if defined (HI_SOUND_AI_SUPPORT)
    else if (HI_ID_AI == (hSource >> 16))
    {
        s32Ret = HI_MPI_AI_Detach(hSource, hTrack);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("call HI_MPI_AI_DetachSnd failed(0x%x)\n", s32Ret);
            return s32Ret;
        }
    }
#endif
    else
    {
        HI_ERR_AO("Invalid hsoure(0x%x)\n", hSource);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_AO_Track_Stop(hTrack);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("call HI_MPI_AO_Track_Stop failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

HI_S32   HI_UNF_SND_SetTrackWeight(HI_HANDLE hTrack, const HI_UNF_SND_GAIN_ATTR_S* pstMixWeightGain)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_SetWeight(hTrack, pstMixWeightGain);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Set Track(%d) Weight failed.\n", hTrack);
    }

    return s32Ret;
}

HI_S32   HI_UNF_SND_GetTrackWeight(HI_HANDLE hTrack, HI_UNF_SND_GAIN_ATTR_S* pstMixWeightGain)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_GetWeight(hTrack, pstMixWeightGain);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Get Track(%d) Weight failed.\n", hTrack);
    }

    return s32Ret;
}

HI_S32  HI_UNF_SND_SetTrackAbsWeight(HI_HANDLE hTrack, const HI_UNF_SND_ABSGAIN_ATTR_S* pstAbsWeightGain)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_SetAbsWeight(hTrack, pstAbsWeightGain);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Set Track(%d) AbsWeight failed.\n", hTrack);
    }

    return s32Ret;
}

HI_S32  HI_UNF_SND_GetTrackAbsWeight(HI_HANDLE hTrack, HI_UNF_SND_ABSGAIN_ATTR_S* pstAbsWeightGain)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_GetAbsWeight(hTrack, pstAbsWeightGain);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Get Track(%d) AbsWeight failed.\n", hTrack);
    }

    return s32Ret;
}

HI_S32  HI_UNF_SND_SetTrackMute(HI_HANDLE hTrack, HI_BOOL bMute)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_SetMute(hTrack, bMute);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Set Track(%d) Mute failed.\n", hTrack);
    }

    return s32Ret;
}

HI_S32  HI_UNF_SND_GetTrackMute(HI_HANDLE hTrack, HI_BOOL* pbMute)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_GetMute(hTrack, pbMute);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Get Track(%d) Mute failed.\n", hTrack);
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_SetTrackChannelMode(HI_HANDLE hTrack, HI_UNF_TRACK_MODE_E enMode)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_SetChannelMode(hTrack, enMode);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Set Track(%d) SoftTrackMode failed.\n", hTrack);
    }

    return s32Ret;
}


HI_S32 HI_UNF_SND_GetTrackChannelMode(HI_HANDLE hTrack, HI_UNF_TRACK_MODE_E* penMode)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_GetChannelMode(hTrack, penMode);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Get Track(%d) SoftTrackMode failed.\n", hTrack);
    }

    return s32Ret;
}

HI_S32   HI_UNF_SND_SetHdmiMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                HI_UNF_SND_HDMI_MODE_E enHdmiMode)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_SetHdmiMode(enSound, enOutPort, enHdmiMode);
}

HI_S32   HI_UNF_SND_GetHdmiMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_HDMI_MODE_E* penHdmiMode)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_GetHdmiMode(enSound, enOutPort, penHdmiMode);
}

HI_S32   HI_UNF_SND_SetSpdifMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                 HI_UNF_SND_SPDIF_MODE_E enSpdifMode)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_SetSpdifMode(enSound, enOutPort, enSpdifMode);
}

HI_S32   HI_UNF_SND_GetSpdifMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                 HI_UNF_SND_SPDIF_MODE_E* penSpdifMode)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_GetSpdifMode(enSound, enOutPort, penSpdifMode);
}


HI_S32   HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_E enTrackType, HI_UNF_AUDIOTRACK_ATTR_S* pstAttr)
{
    return HI_MPI_AO_Track_GetDefaultOpenAttr(enTrackType, pstAttr);
}

HI_S32   HI_UNF_SND_CreateTrack(HI_UNF_SND_E enSound, const HI_UNF_AUDIOTRACK_ATTR_S* pTrackAttr, HI_HANDLE* phTrack)
{
    HI_S32 s32Ret;
    API_SND_CheckId(enSound);

    s32Ret = HI_MPI_AO_Track_Create(enSound, pTrackAttr, phTrack);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Create Track failed, ERR:%#x\n", s32Ret);
    }

    return s32Ret;
}

HI_S32   HI_UNF_SND_DestroyTrack(HI_HANDLE hTrack)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_Destroy(hTrack);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Destroy Track failed, ERR:%#x\n", s32Ret);
    }

    return s32Ret;
}

HI_S32   HI_UNF_SND_SetTrackAttr(HI_HANDLE hTrack, const HI_UNF_AUDIOTRACK_ATTR_S* pstTrackAttr)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_SetAttr(hTrack, pstTrackAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Set Track Attr failed, ERR:%#x\n", s32Ret);
    }

    return s32Ret;
}

HI_S32   HI_UNF_SND_GetTrackAttr(HI_HANDLE hTrack, HI_UNF_AUDIOTRACK_ATTR_S* pstTrackAttr)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_GetAttr(hTrack, pstTrackAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Get Track Attr failed, ERR:%#x\n", s32Ret);
    }

    return s32Ret;
}

HI_S32  HI_UNF_SND_AcquireTrackFrame(HI_HANDLE hTrack, HI_UNF_AO_FRAMEINFO_S* pstAOFrame, HI_U32 u32TimeoutMs)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_AcquireFrame(hTrack, pstAOFrame, u32TimeoutMs);
    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_AO("HI_MPI_AO_Track_AcquireFrame failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

HI_S32  HI_UNF_SND_ReleaseTrackFrame(HI_HANDLE hTrack, HI_UNF_AO_FRAMEINFO_S* pstAOFrame)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_AO_Track_ReleaseFrame(hTrack, pstAOFrame);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("HI_MPI_AO_Track_ReleaseFrame failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

HI_S32 HI_UNF_SND_SetAefBypass(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bBypass)
{
    return HI_ERR_AO_NOTSUPPORT;
}
HI_S32 HI_UNF_SND_GetAefBypass(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbBypass)
{
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32 HI_UNF_SND_RegisterAefAuthLib(const HI_CHAR* pAefLibFileName)
{
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32 HI_UNF_SND_CreateAef(HI_UNF_SND_E enSound, HI_UNF_SND_AEF_TYPE_E enAefType, HI_VOID* pstAdvAttr, HI_HANDLE* phAef)
{
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32 HI_UNF_SND_DestroyAef(HI_HANDLE hAef)
{
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32 HI_UNF_SND_SetAefEnable(HI_HANDLE hAef, HI_BOOL bEnable)
{
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32 HI_UNF_SND_SetAefParams(HI_HANDLE hAef, HI_U32 u32ParamType, const HI_VOID* pstParms)
{
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32 HI_UNF_SND_GetAefParams(HI_HANDLE hAef, HI_U32 u32ParamType, HI_VOID* pstParms)
{
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32 HI_UNF_SND_SetAefConfig(HI_HANDLE hAef, HI_U32 u32CfgType, const HI_VOID* pstConfig)
{
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32 HI_UNF_SND_GetAefConfig(HI_HANDLE hAef, HI_U32 u32CfgType, HI_VOID* pstConfig)
{
    return HI_ERR_AO_NOTSUPPORT;
}

HI_S32   HI_UNF_SND_SetLowLatency(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E eOutPort, HI_U32 u32LatecnyMs)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_SetLowLatency(enSound, eOutPort, u32LatecnyMs);
}

HI_S32 HI_UNF_SND_GetLowLatency(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E eOutPort, HI_U32* p32LatecnyMs)
{
    API_SND_CheckId(enSound);
    return HI_MPI_AO_SND_GetLowLatency(enSound, eOutPort, p32LatecnyMs);
}

HI_S32 HI_UNF_SND_SetDelayCompensationMs(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32 u32DelayMs)
{
#if defined (HI_SOUND_SPDIF_COMPENSATION_SUPPORT)
    HI_S32 s32Ret;

    API_SND_CheckId(enSound);

    if (enOutPort != HI_UNF_SND_OUTPUTPORT_SPDIF0)
    {
        HI_ERR_AO("Invalid out port, only support HI_UNF_SND_OUTPUTPORT_SPDIF0!\n");
        return HI_ERR_AO_NOTSUPPORT;
    }

    s32Ret = HI_MPI_AO_SND_SetDelayCompensationMs(enSound, enOutPort, u32DelayMs);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("HI_MPI_AO_SND_SetDelayCompensationMs failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
#else
    HI_ERR_AO("Do not Support, should enable 'SND SPDIF Compensation' at make menuconfig.\n");
    return HI_ERR_AO_NOTSUPPORT;
#endif
}

HI_S32 HI_UNF_SND_GetDelayCompensationMs(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32* pu32DelayMs)
{
#if defined (HI_SOUND_SPDIF_COMPENSATION_SUPPORT)
    HI_S32 s32Ret;

    API_SND_CheckId(enSound);

    if (enOutPort != HI_UNF_SND_OUTPUTPORT_SPDIF0)
    {
        HI_ERR_AO("Invalid out port, only support HI_UNF_SND_OUTPUTPORT_SPDIF0!\n");
        return HI_ERR_AO_NOTSUPPORT;
    }

    s32Ret = HI_MPI_AO_SND_GetDelayCompensationMs(enSound, enOutPort, pu32DelayMs);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("HI_MPI_AO_SND_GetDelayCompensationMs failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
#else
    HI_ERR_AO("Do not Support, should enable 'SND SPDIF Compensation' at make menuconfig.\n");
    return HI_ERR_AO_NOTSUPPORT;
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

