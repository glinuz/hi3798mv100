#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/vmalloc.h>
#include <linux/time.h>

#include "hi_drv_mce.h"
#include "hi_mpi_adec.h"

#include "hi_kernel_adapt.h"

#include "drv_i2c_ext.h"
#include "drv_gpio_ext.h"
#include "drv_gpioi2c_ext.h"
#include "drv_tuner_ext.h"
#include "drv_demux_ext.h"
#include "drv_base_ext_k.h"
#include "drv_vdec_ext.h"
#include "drv_win_ext.h"
#include "drv_disp_ext.h"
#include "drv_sync_ext.h"
#include "drv_vfmw_ext.h"
#include "drv_adec_ext.h"
#include "drv_ao_ext.h"
#include "hi_drv_file.h"
#include "hi_drv_sys.h"

#include "drv_mce_frc.h"
#include "drv_mce_avplay.h"

#define MCE_MAX_AVPLAY_NUM          1

#define DRV_AVPLAY_LOCK(pMutex)    \
    do{ \
        if(down_interruptible(pMutex)) \
        {       \
            HI_ERR_MCE("ERR: avplay lock error!\n");    \
        }   \
    }while(0)

#define DRV_AVPLAY_UNLOCK(pMutex)    \
    do{ \
        up(pMutex); \
    }while(0)


#define AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay)  \
    do{ \
        if (hAvplay >= MCE_MAX_AVPLAY_NUM) \
        { \
            HI_ERR_MCE("Avplay id:%d is invalid\n", hAvplay); \
            return HI_FAILURE; \
        } \
        if (HI_INVALID_HANDLE == g_hAvplay[hAvplay]) \
        { \
            HI_ERR_MCE("Avplay handle is invalid\n"); \
            return HI_FAILURE; \
        }\
        pAvplay = &g_stAvplay[hAvplay]; \
    }while(0);

#define FASTPLAY_ADEC_FRAME_NUM         (8)

/* video buffer dither waterline */
/* CNcomment: 视频缓冲管理抖动水线的百分比，0-99 */
#define FASTPLAY_ES_VID_FULL_PERCENT    85

/* audio buffer dither waterline */
/* CNcomment: 音频缓冲管理抖动水线的百分比，0-99 */
#define FASTPLAY_ES_AUD_FULL_PERCENT    98

static HI_U32 MCE_GetSysTime(HI_VOID);

static MCE_AVPLAY_S    g_stAvplay[MCE_MAX_AVPLAY_NUM];
static HI_HANDLE       g_hAvplay[MCE_MAX_AVPLAY_NUM];


HI_S32 DRV_AVPLAY_MallocVidChn(MCE_AVPLAY_S *pAvplay, const HI_VOID *pPara)
{
    HI_S32                      Ret = 0;
    HI_UNF_AVPLAY_OPEN_OPT_S    *pOpenPara = (HI_UNF_AVPLAY_OPEN_OPT_S*)pPara;
    HI_UNF_DMX_CHAN_ATTR_S      DmxChnAttr;
    DMX_MMZ_BUF_S               stMmzBuf;

    Ret = HI_DRV_VDEC_Open();
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_VDEC_Init failed.\n");
        goto ERR1;
    }

    Ret = HI_DRV_VDEC_AllocChan(&pAvplay->hVdec, pOpenPara);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_VDEC_AllocChan failed.\n");
        goto ERR2;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        memset(&DmxChnAttr, 0, sizeof(HI_UNF_DMX_CHAN_ATTR_S));
        DmxChnAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;
        DmxChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_VID;
        DmxChnAttr.u32BufSize = pAvplay->AvplayAttr.stStreamAttr.u32VidBufSize;

        Ret = HI_DRV_DMX_CreateChannel(pAvplay->AvplayAttr.u32DemuxId, &DmxChnAttr, &pAvplay->hDmxVid, &stMmzBuf, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("call HI_DRV_DMX_CreateChannel failed.\n");
            goto ERR3;
        }

        Ret = HI_DRV_VDEC_ChanBufferInit(pAvplay->hVdec, 0, pAvplay->hDmxVid);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("call HI_DRV_VDEC_ChanBufferInit failed.\n");
            goto ERR4;
        }
    }
    else if (HI_UNF_AVPLAY_STREAM_TYPE_ES == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = HI_DRV_VDEC_ChanBufferInit(pAvplay->hVdec, pAvplay->AvplayAttr.stStreamAttr.u32VidBufSize, HI_INVALID_HANDLE);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("call HI_DRV_VDEC_ChanBufferInit failed.\n");
            goto ERR3;
        }
    }

    return HI_SUCCESS;

ERR4:
    (HI_VOID)HI_DRV_DMX_DestroyChannel(pAvplay->hDmxVid);
ERR3:
    (HI_VOID)HI_DRV_VDEC_FreeChan(pAvplay->hVdec);
ERR2:
    (HI_VOID)HI_DRV_VDEC_Close();
ERR1:
    return Ret;
}

HI_S32 DRV_AVPLAY_FreeVidChn(MCE_AVPLAY_S *pAvplay)
{
    HI_S32  Ret;

    Ret = HI_DRV_VDEC_ChanBufferDeInit(pAvplay->hVdec);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_VDEC_ChanBufferDeInit failed.\n");
        return Ret;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        if(HI_INVALID_HANDLE != pAvplay->hDmxVid)
        {
            Ret = HI_DRV_DMX_DestroyChannel(pAvplay->hDmxVid);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("call HI_DRV_DMX_DestroyChannel failed.\n");
                return Ret;
            }

            pAvplay->hDmxVid = HI_INVALID_HANDLE;
        }
    }

    if(HI_INVALID_HANDLE != pAvplay->hVdec)
    {
        Ret = HI_DRV_VDEC_FreeChan(pAvplay->hVdec);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("call HI_DRV_VDEC_FreeChan failed.\n");
            return Ret;
        }

        pAvplay->hVdec = HI_INVALID_HANDLE;
    }

    HI_DRV_VDEC_Close();

    return HI_SUCCESS;
}

HI_S32 DRV_AVPLAY_MallocAudChn(MCE_AVPLAY_S *pAvplay)
{
    HI_S32                      Ret = 0;
    HI_UNF_DMX_CHAN_ATTR_S      DmxChnAttr;
    DMX_MMZ_BUF_S               stMmzBuf;
    ADEC_SLIM_ATTR_S            AdecAttr;
/*
    Ret = HI_DRV_ADEC_Init();
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_ADEC_Init failed.\n");
        goto ERR1;
    }
*/
    AdecAttr.enCodecType = ADEC_KEL_TPYE_MPEG;
    AdecAttr.u32InBufSize = pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize * 2 / 3;
    AdecAttr.u32OutBufNum = FASTPLAY_ADEC_FRAME_NUM;
    Ret = HI_DRV_ADEC_Open(&pAvplay->hAdec, &AdecAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_ADEC_Open failed.\n");
        goto ERR1;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        memset(&DmxChnAttr, 0, sizeof(HI_UNF_DMX_CHAN_ATTR_S));
        DmxChnAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;
        DmxChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_AUD;
        DmxChnAttr.u32BufSize = pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize;

        Ret = HI_DRV_DMX_CreateChannel(pAvplay->AvplayAttr.u32DemuxId, &DmxChnAttr, &pAvplay->hDmxAud, &stMmzBuf, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("call HI_DRV_DMX_CreateChannel failed.\n");
            goto ERR3;
        }
    }

    return HI_SUCCESS;

ERR3:
    (HI_VOID)HI_DRV_ADEC_Close(pAvplay->hAdec);
/*
ERR2:
    (HI_VOID)HI_DRV_ADEC_DeInit();
*/
ERR1:
    return Ret;
}

HI_S32 DRV_AVPLAY_FreeAudChn(MCE_AVPLAY_S *pAvplay)
{
    HI_S32  Ret;

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        if(HI_INVALID_HANDLE != pAvplay->hDmxAud)
        {
            Ret = HI_DRV_DMX_DestroyChannel(pAvplay->hDmxAud);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("call HI_DRV_DMX_DestroyChannel failed.\n");
                return Ret;
            }

            pAvplay->hDmxAud = HI_INVALID_HANDLE;
        }
    }

    if(HI_INVALID_HANDLE != pAvplay->hAdec)
    {
        Ret = HI_DRV_ADEC_Close(pAvplay->hAdec);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("call HI_DRV_ADEC_Close failed.\n");
            return Ret;
        }

        pAvplay->hAdec = HI_INVALID_HANDLE;
    }

/*
    Ret = HI_DRV_ADEC_DeInit();
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_ADEC_deInit failed.\n");
    }
*/
    return HI_SUCCESS;
}

HI_S32 DRV_AVPLAY_SetAdecAttr(MCE_AVPLAY_S *pAvplay, const ADEC_SLIM_ATTR_S *pAdecAttr)
{
    HI_S32              Ret;
    ADEC_SLIM_ATTR_S    AdecAttr;

    if (HI_INVALID_HANDLE == pAvplay->hAdec)
    {
        HI_ERR_MCE("aud chn is close, can not set adec attr.\n");
        return HI_FAILURE;
    }

    if (pAvplay->AudEnable)
    {
        HI_ERR_MCE("aud chn is running, can not set adec attr.\n");
        return HI_FAILURE;
    }

    AdecAttr.enCodecType = pAdecAttr->enCodecType;

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        AdecAttr.u32InBufSize = pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize * 2 / 3;
    }
    else
    {
        AdecAttr.u32InBufSize = pAvplay->AvplayAttr.stStreamAttr.u32AudBufSize;
    }
    AdecAttr.u32OutBufNum = FASTPLAY_ADEC_FRAME_NUM;

    Ret = HI_DRV_ADEC_SetAttr(pAvplay->hAdec, &AdecAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_ADEC_SetAttr failed.\n");
        return Ret;
    }

    pAvplay->AdecType = AdecAttr.enCodecType;

    return Ret;
}

HI_S32 DRV_AVPLAY_GetAdecAttr(const MCE_AVPLAY_S *pAvplay, ADEC_SLIM_ATTR_S *pAdecAttr)
{
    HI_S32       Ret;

    if (HI_INVALID_HANDLE == pAvplay->hAdec)
    {
        HI_ERR_MCE("aud chn is close, can not set adec attr.\n");
        return HI_FAILURE;
    }

    Ret = HI_DRV_ADEC_GetAttr(pAvplay->hAdec, pAdecAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_ADEC_GetAttr failed.\n");
    }

    return Ret;
}

HI_S32 DRV_AVPLAY_SetVdecAttr(MCE_AVPLAY_S *pAvplay, HI_UNF_VCODEC_ATTR_S *pVdecAttr)
{
    HI_UNF_VCODEC_ATTR_S  VdecAttr;
    HI_S32                Ret;

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_MCE("vid chn is close, can not set vdec attr.\n");
        return HI_FAILURE;
    }

    Ret = HI_DRV_VDEC_GetChanAttr(pAvplay->hVdec, &VdecAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_MPI_VDEC_GetChanAttr failed.\n");
        return Ret;
    }

    if (pAvplay->VidEnable)
    {
        if (VdecAttr.enType != pVdecAttr->enType)
        {
            HI_ERR_MCE("vid chn is running, can not set vdec type.\n");
            return HI_FAILURE;
        }
    }

    Ret = HI_DRV_VDEC_SetChanAttr(pAvplay->hVdec,pVdecAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_VDEC_SetChanAttr failed.\n");
    }

    memcpy(&pAvplay->VdecAttr, pVdecAttr, sizeof(HI_UNF_VCODEC_ATTR_S));

    return Ret;
}

HI_S32 DRV_AVPLAY_GetVdecAttr(const MCE_AVPLAY_S *pAvplay, HI_UNF_VCODEC_ATTR_S *pVdecAttr)
{
    HI_S32                Ret;

    if (HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_MCE("vid chn is close, can not set vdec attr.\n");
        return HI_FAILURE;
    }

    Ret = HI_DRV_VDEC_GetChanAttr(pAvplay->hVdec, pVdecAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_VDEC_GetChanAttr failed.\n");
    }

    return Ret;
}

HI_S32 DRV_AVPLAY_SetPid(MCE_AVPLAY_S *pAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, const HI_U32 *pPid)
{
    HI_S32       Ret = HI_SUCCESS;

    if (pAvplay->AvplayAttr.stStreamAttr.enStreamType != HI_UNF_AVPLAY_STREAM_TYPE_TS)
    {
        HI_ERR_MCE("avplay is not ts mode.\n");
        return HI_FAILURE;
    }

    if (HI_UNF_AVPLAY_ATTR_ID_AUD_PID == enAttrID)
    {
        if (HI_INVALID_HANDLE == pAvplay->hAdec)
        {
            HI_ERR_MCE("aud chn is close, can not set aud pid.\n");
            return HI_FAILURE;
        }

        if (pAvplay->AudEnable)
        {
            HI_ERR_MCE("aud chn is running, can not set aud pid.\n");
            return HI_FAILURE;
        }

        Ret = HI_DRV_DMX_SetChannelPID(pAvplay->hDmxAud, *pPid);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("call HI_DRV_DMX_SetChannelPID failed.\n");
        }

        pAvplay->DmxAudPid = *pPid;
    }
    else if (HI_UNF_AVPLAY_ATTR_ID_VID_PID == enAttrID)
    {
        if (pAvplay->VidEnable)
        {
            HI_ERR_MCE("vid chn is running, can not set vid pid.\n");
            return HI_FAILURE;
        }

        if (HI_INVALID_HANDLE == pAvplay->hVdec)
        {
            HI_ERR_MCE("vid chn is close, can not set vid pid.\n");
            return HI_FAILURE;
        }

        Ret = HI_DRV_DMX_SetChannelPID(pAvplay->hDmxVid, *pPid);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("call HI_DRV_DMX_SetChannelPID failed.\n");
        }

        pAvplay->DmxVidPid = *pPid;
    }

    return Ret;
}

HI_S32 DRV_AVPLAY_GetPid(const MCE_AVPLAY_S *pAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_U32 *pPid)
{
    HI_S32       Ret = HI_SUCCESS;

    if (pAvplay->AvplayAttr.stStreamAttr.enStreamType != HI_UNF_AVPLAY_STREAM_TYPE_TS)
    {
        HI_ERR_MCE("avplay is not ts mode.\n");
        return HI_FAILURE;
    }

    if (HI_UNF_AVPLAY_ATTR_ID_AUD_PID == enAttrID)
    {
        if (HI_INVALID_HANDLE == pAvplay->hAdec)
        {
            HI_ERR_MCE("aud chn is close, can not get aud pid.\n");
            return HI_FAILURE;
        }

        Ret = HI_DRV_DMX_GetChannelPID(pAvplay->hDmxAud, pPid);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("call HI_DRV_DMX_GetChannelPID failed.\n");
        }
    }
    else if (HI_UNF_AVPLAY_ATTR_ID_VID_PID == enAttrID)
    {
        if (HI_INVALID_HANDLE == pAvplay->hVdec)
        {
            HI_ERR_MCE("vid chn is close, can not get vid pid.\n");
            return HI_FAILURE;
        }

        Ret = HI_DRV_DMX_GetChannelPID(pAvplay->hDmxVid, pPid);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("call HI_DRV_DMX_GetChannelPID failed.\n");
        }
    }

    return Ret;
}

HI_S32 DRV_AVPLAY_SetSyncAttr(MCE_AVPLAY_S *pAvplay, HI_UNF_SYNC_ATTR_S *pSyncAttr)
{
    return HI_DRV_SYNC_SetAttr(pAvplay->hSync, pSyncAttr);
}

HI_S32 DRV_AVPLAY_GetSyncAttr(MCE_AVPLAY_S *pAvplay, HI_UNF_SYNC_ATTR_S *pSyncAttr)
{
    return HI_DRV_SYNC_GetAttr(pAvplay->hSync, pSyncAttr);
}

HI_S32 DRV_AVPLAY_StartVidChn(const MCE_AVPLAY_S *pAvplay)
{
    HI_S32         Ret;

    Ret = HI_DRV_SYNC_Start(pAvplay->hSync, SYNC_CHAN_VID);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_SYNC_Start Vid failed.\n");
        goto ERR1;
    }

    Ret = HI_DRV_VDEC_ChanStart(pAvplay->hVdec);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_VDEC_ChanStart failed.\n");
        goto ERR2;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = HI_DRV_DMX_OpenChannel(pAvplay->hDmxVid);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("call HI_DRV_DMX_OpenChannel failed.\n");
            goto ERR3;
        }
    }

    return HI_SUCCESS;

ERR3:
    (HI_VOID)HI_DRV_VDEC_ChanStop(pAvplay->hDmxVid);
ERR2:
    (HI_VOID)HI_DRV_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_VID);
ERR1:
    return Ret;
}

HI_S32 DRV_AVPLAY_StopVidChn(const MCE_AVPLAY_S *pAvplay, HI_UNF_AVPLAY_STOP_MODE_E enMode)
{
    HI_S32         Ret;
    HI_DRV_WIN_SWITCH_E     SwitchType = HI_DRV_WIN_SWITCH_BUTT;
    HI_DRV_WIN_INFO_S       stWinInfo = {0};

    Ret = HI_DRV_VDEC_ChanStop(pAvplay->hVdec);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_VDEC_ChanStop failed.\n");
        return Ret;
    }

    Ret = HI_DRV_VDEC_ResetChan(pAvplay->hVdec);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_VDEC_ResetChan failed.\n");
        return Ret;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = HI_DRV_DMX_CloseChannel(pAvplay->hDmxVid);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("call HI_DRV_DMX_CloseChannel failed.\n");
            return Ret;
        }
    }

    if (HI_UNF_AVPLAY_STOP_MODE_STILL == enMode)
    {
        SwitchType = HI_DRV_WIN_SWITCH_LAST;
    }
    else
    {
        SwitchType = HI_DRV_WIN_SWITCH_BLACK;
    }

    if (HI_INVALID_HANDLE != pAvplay->hSharedOrgWin)
    {
        (HI_VOID)HI_DRV_WIN_GetInfo(pAvplay->hSharedOrgWin, &stWinInfo);

        (HI_VOID)HI_DRV_WIN_Reset(pAvplay->hSharedOrgWin, SwitchType);
    }
    else
    {
        stWinInfo.hPrim = HI_INVALID_HANDLE;
        stWinInfo.hSec = HI_INVALID_HANDLE;
    }

    if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
    {
        if (pAvplay->MasterFrmChn.hWindow != stWinInfo.hPrim)
        {
            (HI_VOID)HI_DRV_WIN_Reset(pAvplay->MasterFrmChn.hWindow, SwitchType);
        }
    }

    if (HI_INVALID_HANDLE != pAvplay->SlaveFrmChn.hWindow)
    {
        if (pAvplay->SlaveFrmChn.hWindow != stWinInfo.hSec)
        {
            (HI_VOID)HI_DRV_WIN_Reset(pAvplay->SlaveFrmChn.hWindow, SwitchType);
        }
    }

    Ret = HI_DRV_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_VID);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_SYNC_Stop Vid failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_AVPLAY_StartAudChn(const MCE_AVPLAY_S *pAvplay)
{
    HI_S32         Ret;

    Ret = HI_DRV_SYNC_Start(pAvplay->hSync, SYNC_CHAN_AUD);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_SYNC_Start Aud failed.\n");
        goto ERR1;
    }
/*
    Ret = HI_DRV_ADEC_Start(pAvplay->hAdec);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_ADEC_Start failed.\n");
        goto ERR2;
    }
*/

    Ret = HI_DRV_AO_Track_Start(pAvplay->hSnd);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_AO_Track_Start failed.\n");
        goto ERR2;
    }

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = HI_DRV_DMX_OpenChannel(pAvplay->hDmxAud);
        if(HI_SUCCESS != Ret)
        {
            HI_ERR_MCE("call HI_DRV_DMX_OpenChannel failed.\n");
            goto ERR2;
        }
    }

    return HI_SUCCESS;
/*
ERR3:
    (HI_VOID)HI_DRV_ADEC_Stop(pAvplay->hAdec);
*/
ERR2:
    (HI_VOID)HI_DRV_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_AUD);
ERR1:
    return Ret;
}

HI_S32 DRV_AVPLAY_StopAudChn(const MCE_AVPLAY_S *pAvplay)
{
    HI_S32         Ret;
/*
    Ret = HI_DRV_ADEC_Stop(pAvplay->hAdec);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_ADEC_Stop failed.\n");
        return Ret;
    }
*/

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        Ret = HI_DRV_DMX_CloseChannel(pAvplay->hDmxAud);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("call HI_DRV_DMX_CloseChannel failed.\n");
            return Ret;
        }
    }

    Ret = HI_DRV_AO_Track_Stop(pAvplay->hSnd);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_AO_Track_Stop failed.\n");
        return Ret;
    }

    Ret = HI_DRV_SYNC_Stop(pAvplay->hSync, SYNC_CHAN_AUD);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_MCE("call HI_DRV_SYNC_Stop Aud failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_AVPLAY_Reset(MCE_AVPLAY_S *pAvplay)
{
    return HI_SUCCESS;
}

HI_VOID DRV_AVPLAY_ProcAdecToAo(MCE_AVPLAY_S *pAvplay)
{
    HI_S32                  Ret;
    ADEC_EXTFRAMEINFO_S     AdecExtInfo;
    HI_U32                  AoBufTime = 0;
    ADEC_STATUSINFO_S       AdecStatusinfo;

    if (!pAvplay->AudEnable)
    {
        return;
    }

    if (!pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO])
    {
        Ret = HI_DRV_ADEC_ReceiveFrame(pAvplay->hAdec, &pAvplay->AvplayAudFrm, &AdecExtInfo);
        if (HI_SUCCESS == Ret)
        {
            pAvplay->AudInfo.SrcPts = AdecExtInfo.u32OrgPtsMs;
            pAvplay->AudInfo.Pts = pAvplay->AvplayAudFrm.u32PtsMs;
            pAvplay->AudInfo.FrameTime = AdecExtInfo.u32FrameDurationMs;

            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_TRUE;
        }
        else
        {
            (HI_VOID)HI_DRV_ADEC_Pull(pAvplay->hAdec);
        }
    }

    if (pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO])
    {
    #if 0
        static struct file    *fpDst = HI_NULL;
        if (HI_NULL == fpDst)
        {
            fpDst = HI_DRV_FILE_Open("/mnt/mce.pcm", 1);
            HI_ERR_MCE("%u %u %u\n", pAvplay->AvplayAudFrm.u32Channels, pAvplay->AvplayAudFrm.s32BitPerSample
            , pAvplay->AvplayAudFrm.u32SampleRate);
        }

        HI_DRV_FILE_Write(fpDst, pAvplay->AvplayAudFrm.ps32PcmBuffer, pAvplay->AvplayAudFrm.u32PcmSamplesPerFrame *
                     pAvplay->AvplayAudFrm.u32Channels* pAvplay->AvplayAudFrm.s32BitPerSample / 8);

        (HI_VOID)HI_DRV_ADEC_ReleaseFrame(pAvplay->hAdec, &pAvplay->AvplayAudFrm);
        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE;
    #endif
        (HI_VOID)HI_DRV_AO_Track_GetDelayMs(pAvplay->hSnd, &AoBufTime);

        (HI_VOID)HI_DRV_ADEC_GetStatusInfo(pAvplay->hAdec, &AdecStatusinfo);

        pAvplay->AudInfo.BufTime = AoBufTime;
        pAvplay->AudInfo.FrameNum = AdecStatusinfo.u32UsedBufNum;

        Ret = HI_DRV_SYNC_AudJudge(pAvplay->hSync, &pAvplay->AudInfo, &pAvplay->AudOpt);
        if (HI_SUCCESS == Ret)
        {
            if (SYNC_PROC_DISCARD == pAvplay->AudOpt.SyncProc)
            {
                pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE;
                pAvplay->AvplayProcContinue = HI_TRUE;
                (HI_VOID)HI_DRV_ADEC_ReleaseFrame(pAvplay->hAdec, &pAvplay->AvplayAudFrm);
                return;
            }
            else if (SYNC_PROC_REPEAT == pAvplay->AudOpt.SyncProc
                    || SYNC_AUD_SPEED_ADJUST_DOWN == pAvplay->AudOpt.SpeedAdjust)
            {
                (HI_VOID)HI_DRV_ADEC_Pull(pAvplay->hAdec);
                return;
            }
        }

        Ret = HI_DRV_AO_Track_SendData(pAvplay->hSnd, &pAvplay->AvplayAudFrm);
        if (HI_SUCCESS == Ret)
        {
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE;
            pAvplay->AvplayProcContinue = HI_TRUE;

            (HI_VOID)HI_DRV_ADEC_ReleaseFrame(pAvplay->hAdec, &pAvplay->AvplayAudFrm);
        }
        else
        {
            if (HI_ERR_AO_OUT_BUF_FULL != Ret
                && HI_ERR_AO_SENDMUTE != Ret) /* Error drop this frame */
            {
                HI_ERR_MCE("Send AudFrame to AO failed:%#x, drop a frame.\n", Ret);
                pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE;
                pAvplay->AvplayProcContinue = HI_TRUE;
                (HI_VOID)HI_DRV_ADEC_ReleaseFrame(pAvplay->hAdec, &pAvplay->AvplayAudFrm);
            }
        }
    }

    return;
}

HI_VOID DRV_AVPLAY_ProcOriVidFrameRate(MCE_AVPLAY_S *pAvplay)
{
#ifdef MCE_FRAME_RATE_ADJUST_SUPPORT
    HI_U32                i;
    HI_DRV_VIDEO_FRAME_S *pVidFrm = HI_NULL;

    for (i = 0; i < pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        if (   (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->MasterFrmChn.hPort)
            || (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->SlaveFrmChn.hPort) )
        {
            pVidFrm = &pAvplay->CurFrmPack.stFrame[i].stFrameVideo;
            pVidFrm->u32OriFrameRate = pVidFrm->u32FrameRate;
        }
    }
#endif

    return;
}

HI_VOID DRV_AVPLAY_ProcVidFrameRate(MCE_AVPLAY_S *pAvplay, HI_DRV_VIDEO_FRAME_S *pVideoFrame)
{
   /*
    * Private video playback:
    * 1.Set the input frame rate is equal to the output frame rate;
    * 2.The display module needn't do FRC;
    */
#ifdef MCE_FRAME_RATE_ADJUST_SUPPORT
    HI_S32                  Ret;
    HI_DRV_WIN_PLAY_INFO_S  WinInfo = {0};

    Ret = HI_DRV_WIN_GetPlayInfo(pAvplay->MasterFrmChn.hWindow, &WinInfo);
    if (HI_SUCCESS == Ret)
    {
        pVideoFrame->u32FrameRate = WinInfo.u32DispRate * 10;
    }
#endif

    return;
}


HI_VOID DRV_AVPLAY_ProcVidFrc(MCE_AVPLAY_S *pAvplay)
{
    HI_U32                              i;
    HI_DRV_WIN_PLAY_INFO_S              WinInfo;

    pAvplay->FrcNeedPlayCnt = 1;
    pAvplay->FrcCurPlayCnt = 0;
    pAvplay->FrcCtrlInfo.s32FrmState = 0;

    /* find the master chan */
    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->MasterFrmChn.hPort)
        {
            break;
        }
    }

    if (i == pAvplay->CurFrmPack.u32FrmNum)
    {
        return;
    }

    (HI_VOID)HI_DRV_WIN_GetPlayInfo(pAvplay->MasterFrmChn.hWindow, &WinInfo);

    pAvplay->FrcParamCfg.u32InRate = pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32FrameRate/100;
    pAvplay->FrcParamCfg.u32OutRate = WinInfo.u32DispRate/10;

    if (HI_TRUE == pAvplay->bFrcEnable)
    {
        /*do frc for every new frame*/
        (HI_VOID)MCE_FrcCalculate(&pAvplay->FrcCalAlg, &pAvplay->FrcParamCfg, &pAvplay->FrcCtrlInfo);

        pAvplay->FrcNeedPlayCnt = 1 + pAvplay->FrcCtrlInfo.s32FrmState;
    }

    return;
}

HI_VOID DRV_AVPLAY_ProcVidSync(MCE_AVPLAY_S *pAvplay)
{
    HI_U32                              i;
    HI_DRV_WIN_PLAY_INFO_S              WinInfo;
    HI_DRV_VIDEO_PRIVATE_S              *pstFrmPriv = HI_NULL;

    /* find the master chan */
    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->MasterFrmChn.hPort)
        {
            break;
        }
    }

    if (i == pAvplay->CurFrmPack.u32FrmNum)
    {
        return;
    }

    pAvplay->VidInfo.SrcPts = pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32SrcPts;
    pAvplay->VidInfo.Pts = pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32Pts;

    if (0 != pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32OriFrameRate)
    {
        pAvplay->VidInfo.FrameTime = 1000000 / pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32OriFrameRate;
    }
    else
    {
        pAvplay->VidInfo.FrameTime = 40;
    }

    pstFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)(pAvplay->CurFrmPack.stFrame[i].stFrameVideo.u32Priv);
    pstFrmPriv->u32PlayTime = 1;

    /* obtain original stream info, judge whether Progressive*/
    if (HI_DRV_FIELD_ALL == pstFrmPriv->eOriginField)
    {
        pAvplay->VidInfo.bProgressive = HI_TRUE;
    }
    else
    {
        pAvplay->VidInfo.bProgressive = HI_FALSE;
    }

    pAvplay->VidInfo.DispTime = pAvplay->FrcNeedPlayCnt;

    /* need to obtain real-time delaytime */
    (HI_VOID)HI_DRV_WIN_GetPlayInfo(pAvplay->MasterFrmChn.hWindow, &WinInfo);
    pAvplay->VidInfo.DelayTime = WinInfo.u32DelayTime;


    (HI_VOID)HI_DRV_SYNC_VidJudge(pAvplay->hSync, &pAvplay->VidInfo, &pAvplay->VidOpt);

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        pAvplay->CurFrmPack.stFrame[i].stFrameVideo.enTBAdjust = HI_DRV_VIDEO_TB_PLAY;
    }

    return;
}

HI_VOID DRV_AVPLAY_ProcVidPlay(MCE_AVPLAY_S *pAvplay)
{
    HI_S32                              Ret;
    HI_U32                              i;

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->MasterFrmChn.hPort)
        {
            break;
        }
    }

    if (i == pAvplay->CurFrmPack.u32FrmNum)
    {
        return;
    }

    DRV_AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);

    Ret = HI_DRV_WIN_QFrame(pAvplay->MasterFrmChn.hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
    if (HI_SUCCESS == Ret)
    {
        memcpy(&pAvplay->LstFrmPack, &pAvplay->CurFrmPack, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));
        pAvplay->AvplayProcContinue = HI_TRUE;
        pAvplay->FrcCurPlayCnt++;
    }
    else if (HI_ERR_VO_BUFQUE_FULL != Ret)
    {
        HI_ERR_MCE("Play: queue frame to master win failed, Ret=%#x!\n", Ret);

        if (0 == pAvplay->FrcCurPlayCnt)
        {
            (HI_VOID)HI_DRV_WIN_QULSFrame(pAvplay->MasterFrmChn.hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
        }

        pAvplay->AvplayProcContinue = HI_TRUE;
        pAvplay->FrcCurPlayCnt = pAvplay->FrcNeedPlayCnt;
    }
    else
    {
        /* master window is full, do not send to slave window */
        HI_INFO_MCE("Play: queue frame to master win, master win full!\n");
        return;
    }

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->SlaveFrmChn.hPort)
        {
            DRV_AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);

            Ret = HI_DRV_WIN_QFrame(pAvplay->SlaveFrmChn.hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
            if (HI_SUCCESS != Ret)
            {
                if ((0 == pAvplay->FrcCurPlayCnt) || (1 == pAvplay->FrcCurPlayCnt))
                {
                    (HI_VOID)HI_DRV_WIN_QULSFrame(pAvplay->SlaveFrmChn.hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
                }
            }
        }
    }

    return;
}


HI_VOID DRV_AVPLAY_ProcVidRepeat(MCE_AVPLAY_S *pAvplay)
{
    HI_S32                              Ret;
    HI_U32                              i;

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->MasterFrmChn.hPort)
        {
            break;
        }
    }

    if (i == pAvplay->CurFrmPack.u32FrmNum)
    {
        return;
    }

    if (0 == pAvplay->LstFrmPack.u32FrmNum)
    {
        return;
    }

    if (pAvplay->CurFrmPack.stFrame[i].hport != pAvplay->LstFrmPack.stFrame[i].hport)
    {
        return;
    }

    DRV_AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->LstFrmPack.stFrame[i].stFrameVideo);

    Ret = HI_DRV_WIN_QFrame(pAvplay->MasterFrmChn.hWindow, &pAvplay->LstFrmPack.stFrame[i].stFrameVideo);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_MCE("Repeat, queue last frame to master win failed, Ret=%#x!\n", Ret);
        return;
    }

    HI_INFO_MCE("Repeat: Queue frame to master win success!\n");

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {

        if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->SlaveFrmChn.hPort)
        {
            if (pAvplay->CurFrmPack.stFrame[i].hport != pAvplay->LstFrmPack.stFrame[i].hport)
            {
                continue;
            }

            DRV_AVPLAY_ProcVidFrameRate(pAvplay, &pAvplay->LstFrmPack.stFrame[i].stFrameVideo);

            Ret = HI_DRV_WIN_QFrame(pAvplay->SlaveFrmChn.hWindow, &pAvplay->LstFrmPack.stFrame[i].stFrameVideo);
            if (HI_SUCCESS != Ret)
            {
                HI_INFO_MCE("Sync repeat, queue last frame to slave win failed, Ret=%#x!\n", Ret);
            }
        }
    }

    return;

}

HI_VOID DRV_AVPLAY_ProcVidDiscard(MCE_AVPLAY_S *pAvplay)
{
    HI_S32                              Ret;
    HI_U32                              i;

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->MasterFrmChn.hPort)
        {
            break;
        }
    }

    if (i == pAvplay->CurFrmPack.u32FrmNum)
    {
        return;
    }

    Ret = HI_DRV_WIN_QULSFrame(pAvplay->MasterFrmChn.hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_MCE("Discard, queue useless frame to master win failed, Ret=%#x!\n", Ret);
        return;
    }

    HI_INFO_MCE("Discard, queue useless frame to master win success!\n");

    pAvplay->AvplayProcContinue = HI_TRUE;
    pAvplay->FrcCurPlayCnt = pAvplay->FrcNeedPlayCnt;

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
        if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->SlaveFrmChn.hPort)
        {
            (HI_VOID)HI_DRV_WIN_QULSFrame(pAvplay->SlaveFrmChn.hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
        }
    }

    return;
}

extern HI_BOOL g_bRecvVideoFrame;
static HI_U32 g_u32FrameCount = 0;

HI_VOID DRV_AVPLAY_ProcVdecToVo(MCE_AVPLAY_S *pAvplay)
{
    HI_S32  Ret;

    if (!pAvplay->VidEnable)
    {
        return;
    }

    if (!pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
    {
        Ret = HI_DRV_VDEC_Chan_RecvVpssFrmBuf(pAvplay->hVdec, &pAvplay->CurFrmPack);
        if (HI_SUCCESS != Ret)
        {
             return;
        }

        g_u32FrameCount++;
        if (g_u32FrameCount == 2)
        {
            g_bRecvVideoFrame = HI_TRUE;
        }

        HI_INFO_MCE("=====Receive a new frame, sys=%u, id=%u, pts=%u=====\n",
            MCE_GetSysTime(), pAvplay->CurFrmPack.stFrame[0].stFrameVideo.u32FrameIndex,
            pAvplay->CurFrmPack.stFrame[0].stFrameVideo.u32Pts);

        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_TRUE;

        DRV_AVPLAY_ProcOriVidFrameRate(pAvplay);

        DRV_AVPLAY_ProcVidFrc(pAvplay);
    }

    if (pAvplay->bStepMode)
    {
        if (pAvplay->bStepPlay)
        {
            DRV_AVPLAY_ProcVidPlay(pAvplay);
            pAvplay->bStepPlay = HI_FALSE;
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
        }

        return;
    }

    if (0 == pAvplay->FrcCurPlayCnt)
    {
        DRV_AVPLAY_ProcVidSync(pAvplay);
    }

    HI_INFO_MCE("sys:%u, frm:%d, need:%u, cur:%u, sync:%u, delay:%u\n",
        MCE_GetSysTime(),pAvplay->CurFrmPack.stFrame[0].stFrameVideo.u32FrameIndex, pAvplay->FrcNeedPlayCnt,
        pAvplay->FrcCurPlayCnt, pAvplay->VidOpt.SyncProc, pAvplay->VidInfo.DelayTime);

    if ((pAvplay->FrcCurPlayCnt < pAvplay->FrcNeedPlayCnt)
        || (0 == pAvplay->FrcNeedPlayCnt)
        )
    {
        if (SYNC_PROC_PLAY == pAvplay->VidOpt.SyncProc)
        {
            DRV_AVPLAY_ProcVidPlay(pAvplay);
        }
        else if (SYNC_PROC_REPEAT == pAvplay->VidOpt.SyncProc)
        {
            DRV_AVPLAY_ProcVidRepeat(pAvplay);
        }
        else if (SYNC_PROC_DISCARD == pAvplay->VidOpt.SyncProc)
        {
            DRV_AVPLAY_ProcVidDiscard(pAvplay);
        }
    }

    if (pAvplay->FrcCurPlayCnt >= pAvplay->FrcNeedPlayCnt)
    {
        pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
    }

    return;
}

HI_VOID DRV_AVPLAY_ProcDmxToAdec(MCE_AVPLAY_S *pAvplay)
{
    HI_UNF_STREAM_BUF_S             AdecEsBuf;
    HI_S32                          Ret;

    if (!pAvplay->AudEnable)
    {
        return;
    }

    if (!pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC])
    {
        Ret = HI_DRV_DMX_AcquireEs(pAvplay->hDmxAud, &(pAvplay->AvplayDmxEsBuf));
        if (HI_SUCCESS == Ret)
        {
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC] = HI_TRUE;
        }
    }

    if (pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC])
    {
        AdecEsBuf.pu8Data = pAvplay->AvplayDmxEsBuf.pu8BufVirAddr;
        AdecEsBuf.u32Size = pAvplay->AvplayDmxEsBuf.u32BufLen;

        Ret = HI_DRV_ADEC_SendStream(pAvplay->hAdec, &AdecEsBuf, pAvplay->AvplayDmxEsBuf.u32PtsMs);
        if (HI_SUCCESS == Ret)
        {
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC] = HI_FALSE;
            pAvplay->AvplayProcContinue = HI_TRUE;
            (HI_VOID)HI_DRV_DMX_ReleaseEs(pAvplay->hDmxAud, &pAvplay->AvplayDmxEsBuf);
        }
        else
        {
            if (HI_ERR_ADEC_IN_BUF_FULL != Ret)
            {
                HI_ERR_MCE("Send AudEs buf to ADEC fail:%#x, drop a pkg.\n", Ret);
                pAvplay->AvplayProcDataFlag[AVPLAY_PROC_DMX_ADEC] = HI_FALSE;
                pAvplay->AvplayProcContinue = HI_TRUE;
                (HI_VOID)HI_DRV_DMX_ReleaseEs(pAvplay->hDmxAud, &pAvplay->AvplayDmxEsBuf);
            }
        }
    }

    return;
}

HI_VOID DRV_AVPLAY_ProcCheckBuf(MCE_AVPLAY_S *pAvplay)
{
    HI_S32                      Ret;
    ADEC_STATUSINFO_S           AudAdecBuf;
    VDEC_STATUSINFO_S           VidVdecBuf;
    HI_MPI_DMX_BUF_STATUS_S     AudDmxBuf;
    HI_MPI_DMX_BUF_STATUS_S     VidDmxBuf;
    HI_U32                      AudBufPercent = 0;
    HI_U32                      VidBufPercent = 0;
    HI_BOOL                     ResetProc = HI_FALSE;
    DMX_PORT_MODE_E             PortMode;
    HI_U32                      PortID;

    if(pAvplay->AudEnable)
    {
        Ret = HI_DRV_ADEC_GetStatusInfo(pAvplay->hAdec, &AudAdecBuf);

        if(HI_SUCCESS == Ret)
        {
            if(HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
            {
                Ret = HI_DRV_DMX_GetPESBufferStatus(pAvplay->hDmxAud, &AudDmxBuf);
                if (HI_SUCCESS == Ret)
                {
                    AudBufPercent = (AudDmxBuf.u32UsedSize + AudAdecBuf.u32BufferUsed) * 100 / (AudDmxBuf.u32BufSize + AudAdecBuf.u32BufferSize);
                }
            }
            else
            {
                AudBufPercent = AudAdecBuf.u32BufferUsed * 100 / AudAdecBuf.u32BufferSize;
            }
        }
    }

    if(pAvplay->VidEnable)
    {
        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            Ret = HI_DRV_DMX_GetPESBufferStatus(pAvplay->hDmxVid, &VidDmxBuf);
            if(HI_SUCCESS == Ret)
            {
                VidBufPercent = VidDmxBuf.u32UsedSize * 100 / VidDmxBuf.u32BufSize;
            }
        }
        else
        {
            Ret = HI_DRV_VDEC_GetChanStatusInfo(pAvplay->hVdec, &VidVdecBuf);
            {
                VidBufPercent = VidVdecBuf.u32BufferUsed * 100 / VidVdecBuf.u32BufferSize;
            }
        }
    }


    if(HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        (HI_VOID)HI_DRV_DMX_GetPortId(pAvplay->AvplayAttr.u32DemuxId, &PortMode, &PortID);
    }

    if (DMX_PORT_MODE_RAM == PortMode)
    {
        if (AudBufPercent >= FASTPLAY_ES_AUD_FULL_PERCENT)
        {
            ResetProc = HI_TRUE;
        }

        if(VidBufPercent >= FASTPLAY_ES_VID_FULL_PERCENT)
        {
            ResetProc = HI_TRUE;
        }
    }

    if (ResetProc)
    {
        (HI_VOID)DRV_AVPLAY_Reset(pAvplay);

        AudBufPercent = 0;
        VidBufPercent = 0;
    }

    return;
}


HI_S32 DRV_AVPLAY_ProcThread(HI_VOID *Arg)
{
    MCE_AVPLAY_S        *pAvplay;
    pAvplay = (MCE_AVPLAY_S *)Arg;

    while (pAvplay->AvplayThreadRun)
    {
        DRV_AVPLAY_LOCK(&(pAvplay->AvplayThreadMutex));

        pAvplay->AvplayProcContinue = HI_FALSE;

        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            DRV_AVPLAY_ProcDmxToAdec(pAvplay);
        }

        DRV_AVPLAY_ProcAdecToAo(pAvplay);

        DRV_AVPLAY_ProcVdecToVo(pAvplay);

        DRV_AVPLAY_ProcCheckBuf(pAvplay);

        DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayThreadMutex));

        if (pAvplay->AvplayProcContinue)
        {
            continue;
        }

        (HI_VOID)msleep(10);
    }

    return HI_SUCCESS;
}

HI_S32 DRV_AVPLAY_RelSpecialFrame(MCE_AVPLAY_S *pAvplay, HI_HANDLE hWin)
{
    HI_U32                              i;

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
         if (hWin == pAvplay->MasterFrmChn.hWindow)
         {
             (HI_VOID)HI_DRV_WIN_QULSFrame(pAvplay->MasterFrmChn.hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
         }
         else if (hWin == pAvplay->SlaveFrmChn.hWindow)
         {
             (HI_VOID)HI_DRV_WIN_QULSFrame(pAvplay->SlaveFrmChn.hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
         }
    }

    return HI_SUCCESS;
}

HI_S32 DRV_AVPLAY_RelAllChnFrame(MCE_AVPLAY_S *pAvplay)
{
    HI_U32                              i;

    for (i=0; i<pAvplay->CurFrmPack.u32FrmNum; i++)
    {
         if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->MasterFrmChn.hPort)
         {
             (HI_VOID)HI_DRV_WIN_QULSFrame(pAvplay->MasterFrmChn.hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
         }
         else if (pAvplay->CurFrmPack.stFrame[i].hport == pAvplay->SlaveFrmChn.hPort)
         {
             (HI_VOID)HI_DRV_WIN_QULSFrame(pAvplay->SlaveFrmChn.hWindow, &pAvplay->CurFrmPack.stFrame[i].stFrameVideo);
         }
    }

    memset(&pAvplay->LstFrmPack, 0, sizeof(HI_DRV_VIDEO_FRAME_PACKAGE_S));

    return HI_SUCCESS;
}

HI_S32 DRV_AVPLAY_SetPortAttr(MCE_AVPLAY_S *pAvplay, HI_HANDLE hPort, HI_BOOL bMainPort)
{
    HI_S32                      Ret = HI_SUCCESS;

    if (bMainPort)
    {
        /*default all port is ordinary*/
        Ret = HI_DRV_VDEC_SetPortType(pAvplay->hVdec, hPort, VDEC_PORT_TYPE_MASTER);
    }
    else
    {
        Ret = HI_DRV_VDEC_SetPortType(pAvplay->hVdec, hPort, VDEC_PORT_TYPE_SLAVE);
    }

    if (HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VDEC_SetPortType.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_DRV_VDEC_EnablePort(pAvplay->hVdec, hPort);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VDEC_EnablePort.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    return HI_SUCCESS;
}


HI_S32 DRV_AVPLAY_CreatePort(MCE_AVPLAY_S *pAvplay, HI_HANDLE hWin, VDEC_PORT_ABILITY_E enAbility, HI_HANDLE *phPort)
{
    HI_S32                      Ret = HI_SUCCESS;
    VDEC_PORT_PARAM_S           stPortPara;
    HI_DRV_WIN_SRC_HANDLE_S       stSrcInfo;

    Ret = HI_DRV_VDEC_CreatePort(pAvplay->hVdec, phPort, enAbility);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VDEC_CreatePort.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_DRV_VDEC_GetPortParam(pAvplay->hVdec, *phPort, &stPortPara);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VDEC_GetPortParam.\n");
        (HI_VOID)HI_DRV_VDEC_DestroyPort(pAvplay->hVdec, *phPort);
        *phPort = HI_INVALID_HANDLE;
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    stSrcInfo.hSrc          = *phPort;
    stSrcInfo.enSrcMode = HI_ID_VDEC;
    stSrcInfo.hSecondSrc    = HI_INVALID_HANDLE;

    Ret = HI_DRV_WIN_SetSource(hWin, &stSrcInfo);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_WIN_SetSource 0x%x\n", Ret);
        (HI_VOID)HI_DRV_VDEC_DestroyPort(pAvplay->hVdec, *phPort);
        *phPort = HI_INVALID_HANDLE;
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_AVPLAY_DestroyPort(MCE_AVPLAY_S *pAvplay, HI_HANDLE hWin, HI_HANDLE hPort)
{
    HI_S32                      Ret = HI_SUCCESS;
    HI_DRV_WIN_SRC_HANDLE_S       stSrcInfo;

    stSrcInfo.hSrc          = HI_INVALID_HANDLE;
    stSrcInfo.hSecondSrc    = HI_INVALID_HANDLE;
    stSrcInfo.enSrcMode = HI_ID_BUTT;

    Ret = HI_DRV_WIN_SetSource(hWin, &stSrcInfo);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_WIN_SetSource 0x%x\n", Ret);
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    Ret = HI_DRV_VDEC_DestroyPort(pAvplay->hVdec, hPort);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_DRV_VDEC_DestroyPort.\n");
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    return Ret;
}

static HI_U32 MCE_GetSysTime(HI_VOID)
{
    HI_U32      Ticks=10;
    return Ticks * 10;
}

HI_S32 HI_DRV_AVPLAY_Init(HI_VOID)
{
    HI_S32 i;

    for(i = 0; i < MCE_MAX_AVPLAY_NUM; i++)
    {
        memset(&g_stAvplay[i], 0x00, sizeof(MCE_AVPLAY_S));
        g_hAvplay[i] = HI_INVALID_HANDLE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_AVPLAY_DeInit(HI_VOID)
{
    HI_S32 i;

    for(i = 0; i < MCE_MAX_AVPLAY_NUM; i++)
    {
        memset(&g_stAvplay[i], 0x00, sizeof(MCE_AVPLAY_S));
        g_hAvplay[i] = HI_INVALID_HANDLE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_AVPLAY_GetDefaultConfig(HI_UNF_AVPLAY_ATTR_S *pstAvAttr, HI_UNF_AVPLAY_STREAM_TYPE_E enCfg)
{
    HI_CHIP_TYPE_E     enChipType    = HI_CHIP_TYPE_HI3716C;
    HI_CHIP_VERSION_E  enChipVersion = HI_CHIP_VERSION_V200;

    if (!pstAvAttr)
    {
        HI_ERR_MCE("para pstAvAttr is null.\n");
        return HI_FAILURE ;
    }

    if ((HI_UNF_AVPLAY_STREAM_TYPE_TS != enCfg) && (HI_UNF_AVPLAY_STREAM_TYPE_ES != enCfg))
    {
        HI_ERR_MCE("para enCfg is invalid.\n");
        return HI_FAILURE;
    }

    pstAvAttr->u32DemuxId = 0;
    pstAvAttr->stStreamAttr.enStreamType = enCfg;

    HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);

    if (   ((HI_CHIP_TYPE_HI3798M == enChipType) && (HI_CHIP_VERSION_V100 == enChipVersion))
        || ((HI_CHIP_TYPE_HI3796M == enChipType) && (HI_CHIP_VERSION_V100 == enChipVersion))
        || ((HI_CHIP_TYPE_HI3798C == enChipType) && (HI_CHIP_VERSION_V200 == enChipVersion)) )
    {
        pstAvAttr->stStreamAttr.u32VidBufSize = 16 * 1024 * 1024;
        pstAvAttr->stStreamAttr.u32AudBufSize = 768 * 1024;
    }
    else
    {
        pstAvAttr->stStreamAttr.u32VidBufSize = 6 * 1024 * 1024;
        pstAvAttr->stStreamAttr.u32AudBufSize = 384 * 1024;
    }

    return HI_SUCCESS;
}
HI_S32 HI_DRV_AVPLAY_Create(const HI_UNF_AVPLAY_ATTR_S *pstAvAttr, HI_HANDLE *phAvplay)
{
    HI_S32              Ret;
    HI_S32              i;
    HI_UNF_SYNC_ATTR_S  SyncAttr;
    HI_CHAR             ac8Name[32];

    for(i = 0; i < MCE_MAX_AVPLAY_NUM; i++)
    {
        if(HI_INVALID_HANDLE == g_hAvplay[i])
        {
            break;
        }
    }

    if(MCE_MAX_AVPLAY_NUM == i)
    {
        return HI_FAILURE;
    }

    memcpy(&(g_stAvplay[i].AvplayAttr), pstAvAttr, sizeof(HI_UNF_AVPLAY_ATTR_S));

    g_stAvplay[i].hAvplay = HI_INVALID_HANDLE;

    g_stAvplay[i].hVdec = HI_INVALID_HANDLE;
    g_stAvplay[i].VdecAttr.enType = HI_UNF_VCODEC_TYPE_BUTT;
    g_stAvplay[i].VdecAttr.enMode = HI_UNF_VCODEC_MODE_NORMAL;
    g_stAvplay[i].VdecAttr.u32ErrCover = 0;
    g_stAvplay[i].VdecAttr.u32Priority = 0;
    g_stAvplay[i].hDmxVid   = HI_INVALID_HANDLE;
    g_stAvplay[i].DmxVidPid = 0x1fff;

    g_stAvplay[i].hAdec = HI_INVALID_HANDLE;
    g_stAvplay[i].AdecType = 0xffffffff;
    g_stAvplay[i].hDmxAud   = HI_INVALID_HANDLE;
    g_stAvplay[i].DmxAudPid = 0x1fff;

    g_stAvplay[i].SlaveFrmChn.hWindow = HI_INVALID_HANDLE;
    g_stAvplay[i].SlaveFrmChn.hPort = HI_INVALID_HANDLE;
    g_stAvplay[i].MasterFrmChn.hWindow = HI_INVALID_HANDLE;
    g_stAvplay[i].MasterFrmChn.hPort = HI_INVALID_HANDLE;
    g_stAvplay[i].hSharedOrgWin = HI_INVALID_HANDLE;

    g_stAvplay[i].hSnd = HI_INVALID_HANDLE;

    g_stAvplay[i].VidEnable = HI_FALSE;
    g_stAvplay[i].AudEnable = HI_FALSE;

    g_stAvplay[i].bStepMode = HI_FALSE;
    g_stAvplay[i].bStepPlay = HI_FALSE;
    g_stAvplay[i].bSetEosFlag = HI_FALSE;

    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
    SyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
    SyncAttr.stSyncStartRegion.bSmoothPlay = HI_TRUE;
    SyncAttr.stSyncNovelRegion.s32VidPlusTime = 3000;
    SyncAttr.stSyncNovelRegion.s32VidNegativeTime = -3000;
    SyncAttr.stSyncNovelRegion.bSmoothPlay = HI_FALSE;
    SyncAttr.s32VidPtsAdjust = 0;
    SyncAttr.s32AudPtsAdjust = 0;
    SyncAttr.u32PreSyncTimeoutMs = 0;
    SyncAttr.bQuickOutput = HI_TRUE;
    Ret = HI_DRV_SYNC_Create(&SyncAttr, &g_stAvplay[i].hSync);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("create sync err!\n");
        return HI_FAILURE;
    }

    MCE_FrcCreate(&g_stAvplay[i].FrcCalAlg, &g_stAvplay[i].FrcParamCfg, &g_stAvplay[i].FrcCtrlInfo);

    g_stAvplay[i].bFrcEnable = HI_TRUE;

    HI_INIT_MUTEX(&g_stAvplay[i].AvplayMutex);
    HI_INIT_MUTEX(&g_stAvplay[i].AvplayThreadMutex);

    g_stAvplay[i].AvplayThreadRun = HI_TRUE;

    snprintf(ac8Name, sizeof(ac8Name), "HI_MCE_AvPlay%d", i);
    g_stAvplay[i].pAvplayThreadInst = kthread_create(DRV_AVPLAY_ProcThread, &(g_stAvplay[i]), ac8Name);
    if (IS_ERR(g_stAvplay[i].pAvplayThreadInst))
    {
        HI_ERR_MCE("create mce avplay thread err!\n");
        HI_DRV_SYNC_Destroy(g_stAvplay[i].hSync);
        return HI_FAILURE;
    }

    wake_up_process(g_stAvplay[i].pAvplayThreadInst);

    g_hAvplay[i] = i;
    *phAvplay = g_hAvplay[i];

    return HI_SUCCESS;

}

HI_S32 HI_DRV_AVPLAY_Destroy(HI_HANDLE hAvplay)
{
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    pAvplay->AvplayThreadRun = HI_FALSE;

    if(HI_NULL != pAvplay->pAvplayThreadInst)
    {
        pAvplay->AvplayThreadRun = HI_FALSE;

        kthread_stop(pAvplay->pAvplayThreadInst);

        pAvplay->pAvplayThreadInst = HI_NULL;
    }

    HI_DRV_SYNC_Destroy(pAvplay->hSync);

    MCE_FrcDestroy(&pAvplay->FrcCalAlg);
    pAvplay->bFrcEnable = HI_FALSE;

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));

    memset(pAvplay, 0x00, sizeof(MCE_AVPLAY_S));
    g_hAvplay[hAvplay] = HI_INVALID_HANDLE;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_AVPLAY_ChnOpen(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_VOID *pPara)
{
    HI_S32          Ret;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    if(enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_VID))
    {
        if(HI_INVALID_HANDLE == pAvplay->hVdec)
        {
            Ret = DRV_AVPLAY_MallocVidChn(pAvplay, pPara);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("Avplay malloc vid chn failed.\n");
                goto ERR1;
            }
        }
    }

    if(enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_AUD))
    {
        if(HI_INVALID_HANDLE == pAvplay->hAdec)
        {
            Ret = DRV_AVPLAY_MallocAudChn(pAvplay);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("Avplay malloc vid chn failed.\n");
                goto ERR2;
            }
        }
    }

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
    return Ret;

ERR2:
    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_VID))
    {
        (HI_VOID)DRV_AVPLAY_FreeVidChn(pAvplay);
    }

ERR1:
    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
    return Ret;

}

HI_S32 HI_DRV_AVPLAY_ChnClose(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn)
{
    HI_S32          Ret;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_VID))
    {
        if (pAvplay->VidEnable)
        {
            HI_ERR_MCE("vid chn is enable, can not colsed.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_FAILURE;
        }

        if ((pAvplay->MasterFrmChn.hWindow != HI_INVALID_HANDLE)
            || (pAvplay->SlaveFrmChn.hWindow != HI_INVALID_HANDLE))
        {
            HI_ERR_MCE("window is attach to vdec, can not colsed.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_FAILURE;
        }

        if (pAvplay->hVdec != HI_INVALID_HANDLE)
        {
            Ret = DRV_AVPLAY_FreeVidChn(pAvplay);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("Avplay free vid chn failed.\n");
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return Ret;
            }
        }
    }

    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_AUD))
    {
        if (pAvplay->AudEnable)
        {
            HI_ERR_MCE("aud chn is enable, can not colsed.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_FAILURE;
        }

        if (pAvplay->hSnd!= HI_INVALID_HANDLE)
        {
            HI_ERR_MCE("snd is attach to adec, can not colsed.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_FAILURE;
        }

        if (pAvplay->hAdec != HI_INVALID_HANDLE)
        {
            Ret = DRV_AVPLAY_FreeAudChn(pAvplay);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("Avplay free aud chn failed.\n");
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return Ret;
            }
        }
    }

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
    return HI_SUCCESS;
}

HI_S32 HI_DRV_AVPLAY_SetAttr(HI_HANDLE hAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara)
{
    HI_S32          Ret = HI_SUCCESS;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));
    switch (enAttrID)
    {
        case HI_UNF_AVPLAY_ATTR_ID_STREAM_MODE:
            break;

        case HI_UNF_AVPLAY_ATTR_ID_ADEC:
            Ret = DRV_AVPLAY_SetAdecAttr(pAvplay, (ADEC_SLIM_ATTR_S *)pPara);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("set adec attr failed.\n");
            }
            break;

        case HI_UNF_AVPLAY_ATTR_ID_VDEC:
            Ret = DRV_AVPLAY_SetVdecAttr(pAvplay, (HI_UNF_VCODEC_ATTR_S *)pPara);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("set vdec attr failed.\n");
            }
            break;

        case HI_UNF_AVPLAY_ATTR_ID_AUD_PID:
            Ret = DRV_AVPLAY_SetPid(pAvplay, enAttrID, (HI_U32 *)pPara);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("set aud pid failed.\n");
            }
            break;

        case HI_UNF_AVPLAY_ATTR_ID_VID_PID:
            Ret = DRV_AVPLAY_SetPid(pAvplay, enAttrID, (HI_U32 *)pPara);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("set vid pid failed.\n");
            }
            break;

        case HI_UNF_AVPLAY_ATTR_ID_PCR_PID:
            break;

        case HI_UNF_AVPLAY_ATTR_ID_SYNC:
            Ret = DRV_AVPLAY_SetSyncAttr(pAvplay, (HI_UNF_SYNC_ATTR_S *)pPara);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("set sync attr failed.\n");
            }
            break;

        case HI_UNF_AVPLAY_ATTR_ID_OVERFLOW:
            break;

       case HI_UNF_AVPLAY_ATTR_ID_MULTIAUD:
            break;

        default:
            break;
    }

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
    return Ret;
}

HI_S32 HI_DRV_AVPLAY_GetAttr(HI_HANDLE hAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara)
{
    HI_S32          Ret = HI_SUCCESS;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));
    switch (enAttrID)
    {
        case HI_UNF_AVPLAY_ATTR_ID_STREAM_MODE:
            break;

        case HI_UNF_AVPLAY_ATTR_ID_ADEC:
            Ret = DRV_AVPLAY_GetAdecAttr(pAvplay, pPara);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("get adec attr failed.\n");
            }
            break;

        case HI_UNF_AVPLAY_ATTR_ID_VDEC:
            Ret = DRV_AVPLAY_GetVdecAttr(pAvplay, pPara);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("get vdec attr failed.\n");
            }
            break;

        case HI_UNF_AVPLAY_ATTR_ID_AUD_PID:
            Ret = DRV_AVPLAY_GetPid(pAvplay, enAttrID, pPara);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("get aud pid failed.\n");
            }
            break;

        case HI_UNF_AVPLAY_ATTR_ID_VID_PID:
            Ret = DRV_AVPLAY_GetPid(pAvplay, enAttrID, pPara);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("get vid pid failed.\n");
            }
            break;

        case HI_UNF_AVPLAY_ATTR_ID_PCR_PID:
             break;

        case HI_UNF_AVPLAY_ATTR_ID_SYNC:
            Ret = DRV_AVPLAY_GetSyncAttr(pAvplay, pPara);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("get sync attr failed.\n");
            }
            break;

        case HI_UNF_AVPLAY_ATTR_ID_OVERFLOW:
            break;

        default:
            break;
    }

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
    return Ret;
}

HI_S32 HI_DRV_AVPLAY_Start(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn)
{
    HI_S32          Ret = HI_SUCCESS;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));
    DRV_AVPLAY_LOCK(&(pAvplay->AvplayThreadMutex));

    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_VID))
    {
        if (!pAvplay->VidEnable)
        {
            if (HI_INVALID_HANDLE == pAvplay->hVdec)
            {
                HI_ERR_MCE("vid chn is close, can not start.\n");
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayThreadMutex));
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return HI_FAILURE;
            }

            if ((pAvplay->MasterFrmChn.hWindow == HI_INVALID_HANDLE)
                && (pAvplay->SlaveFrmChn.hWindow == HI_INVALID_HANDLE))
            {
                HI_ERR_MCE("window is invalid, can not start.\n");
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayThreadMutex));
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return HI_FAILURE;
            }

            Ret = DRV_AVPLAY_StartVidChn(pAvplay);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("start vid chn failed.\n");
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayThreadMutex));
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return Ret;
            }

            Ret = HI_DRV_SYNC_Play(pAvplay->hSync);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("call HI_MPI_SYNC_Play Vid failed.\n");
            }

            pAvplay->VidEnable = HI_TRUE;
        }
    }

    if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_AUD))
    {
        if (!pAvplay->AudEnable)
        {
            if (HI_INVALID_HANDLE == pAvplay->hAdec)
            {
                HI_ERR_MCE("aud chn is close, can not start.\n");
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayThreadMutex));
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return HI_FAILURE;
            }

            if (HI_INVALID_HANDLE == pAvplay->hSnd)
            {
                HI_ERR_MCE("snd is not attached, can not start.\n");
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayThreadMutex));
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return HI_FAILURE;
            }

            Ret = DRV_AVPLAY_StartAudChn(pAvplay);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("start aud chn failed.\n");
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayThreadMutex));
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return Ret;
            }

            Ret = HI_DRV_SYNC_Play(pAvplay->hSync);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_MCE("call HI_MPI_SYNC_Play Aud failed.\n");
            }

            pAvplay->AudEnable = HI_TRUE;
        }
    }

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayThreadMutex));
    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
    return HI_SUCCESS;
}

HI_S32 HI_DRV_AVPLAY_Stop(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_UNF_AVPLAY_STOP_OPT_S *pStop)
{
    HI_UNF_AVPLAY_STOP_OPT_S   StopOpt;

    HI_S32          Ret = HI_SUCCESS;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    if (pStop)
    {
        StopOpt.u32TimeoutMs = pStop->u32TimeoutMs;
        StopOpt.enMode = pStop->enMode;
    }
    else
    {
        StopOpt.u32TimeoutMs = 0;
        StopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    }

    /*The relevant channel is already stopped*/
    if ( ((HI_UNF_AVPLAY_MEDIA_CHAN_AUD == enChn) && (!pAvplay->AudEnable) )
       || ((HI_UNF_AVPLAY_MEDIA_CHAN_VID == enChn) && (!pAvplay->VidEnable))
       || ((((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_AUD | (HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_VID) == enChn)
           && (!pAvplay->AudEnable) && (!pAvplay->VidEnable)))
    {
        HI_INFO_MCE("The chn is already stoped\n");
        return HI_SUCCESS;
    }

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));
    DRV_AVPLAY_LOCK(&(pAvplay->AvplayThreadMutex));

    if (0 == StopOpt.u32TimeoutMs)
    {
        if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_VID))
        {
            if (pAvplay->VidEnable)
            {
                Ret = DRV_AVPLAY_StopVidChn(pAvplay, StopOpt.enMode);
                if (Ret != HI_SUCCESS)
                {
                    HI_ERR_MCE("stop vid chn failed.\n");
                    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayThreadMutex));
                    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                    return Ret;
                }

                if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
                {
                    /* resume the frc and window ratio */
                    pAvplay->bFrcEnable = HI_TRUE;
                    pAvplay->FrcParamCfg.u32PlayRate = AVPLAY_ALG_FRC_BASE_PLAY_RATIO;
                }

                pAvplay->VidEnable = HI_FALSE;

                /* may be only stop vidchannel,avoid there is frame at avplay, when stop avplay, we drop this frame*/
                if (HI_TRUE == pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
                {
                    /*Release vpss frame*/
                    (HI_VOID)DRV_AVPLAY_RelAllChnFrame(pAvplay);
                    pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
                }
            }
        }

        if (enChn & ((HI_U32)HI_UNF_AVPLAY_MEDIA_CHAN_AUD))
        {
            if (pAvplay->AudEnable)
            {
                Ret = DRV_AVPLAY_StopAudChn(pAvplay);
                if (Ret != HI_SUCCESS)
                {
                    HI_ERR_MCE("stop aud chn failed.\n");
                    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayThreadMutex));
                    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                    return Ret;
                }

                pAvplay->AudEnable = HI_FALSE;
                pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO] = HI_FALSE; /* avoid there is frame at avplay, when stop avplay, we drop this frame  */
            }
        }
    }

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayThreadMutex));
    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
    return HI_SUCCESS;
}

HI_S32 HI_DRV_AVPLAY_Step(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_STEP_OPT_S *pstStepOpt)
{
    HI_S32                  Ret = HI_SUCCESS;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));
    DRV_AVPLAY_LOCK(&(pAvplay->AvplayThreadMutex));

    pAvplay->bStepMode = HI_TRUE;
    pAvplay->bStepPlay = HI_TRUE;

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayThreadMutex));
    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));

    return Ret;
}

HI_S32 HI_DRV_AVPLAY_SetProgressive(HI_HANDLE hAvplay, HI_BOOL bProgressive)
{
    HI_S32                  Ret;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    Ret = HI_DRV_VDEC_SetProgressive(pAvplay->hVdec, bProgressive);

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));

    return Ret;
}


HI_S32 HI_DRV_AVPLAY_GetBuf(HI_HANDLE  hAvplay,
                            HI_UNF_AVPLAY_BUFID_E enBufId,
                            HI_U32                u32ReqLen,
                            HI_UNF_STREAM_BUF_S  *pstData,
                            HI_U32                u32TimeOutMs)
{
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    HI_S32               Ret;

    if (enBufId >= HI_UNF_AVPLAY_BUF_ID_BUTT)
    {
        HI_ERR_MCE("para enBufId is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    if (!pstData)
    {
        HI_ERR_MCE("para pstData is null.\n");
        return HI_ERR_AVPLAY_NULL_PTR;
    }

    if (u32TimeOutMs != 0)
    {
        HI_ERR_MCE("enBufId=%d NOT support block mode, please set 'u32TimeOutMs' to 0.\n", enBufId);
        return HI_ERR_AVPLAY_NOT_SUPPORT;
    }

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        HI_ERR_MCE("avplay is ts stream mode.\n");
        DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (HI_UNF_AVPLAY_BUF_ID_ES_VID == enBufId)
    {
        if (!pAvplay->VidEnable)
        {
            HI_WARN_MCE("vid chn is stopped.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        pAvplay->AvplayVidEsBuf.u32BufSize = pstData->u32Size;
        Ret = HI_DRV_VDEC_GetEsBuf(pAvplay->hVdec, &pAvplay->AvplayVidEsBuf);
        if (Ret != HI_SUCCESS)
        {
            if (Ret != HI_ERR_VDEC_BUFFER_FULL)
            {
                HI_WARN_MCE("call HI_MPI_VDEC_ChanGetBuffer failed, Ret=0x%x.\n", Ret);
            }

            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return Ret;
        }

        pstData->pu8Data = pAvplay->AvplayVidEsBuf.pu8Addr;
        pstData->u32Size = pAvplay->AvplayVidEsBuf.u32BufSize;
    }

#if 0

    if (HI_UNF_AVPLAY_BUF_ID_ES_AUD == enBufId)
    {
        if (!pAvplay->AudEnable)
        {
            HI_WARN_MCE("aud chn is stopped.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_ERR_AVPLAY_INVALID_OPT;
        }
        Ret = HI_DRV_ADEC_GetEsBuff(pAvplay->hAdec, u32ReqLen, &pAvplay->AvplayAudEsBuf);
        if (Ret != HI_SUCCESS)
        {
            if ((Ret != HI_ERR_ADEC_IN_BUF_FULL) && (Ret != HI_ERR_ADEC_IN_PTSBUF_FULL) )
            {
                HI_ERR_MCE("call HI_MPI_ADEC_GetBuffer failed, Ret=0x%x.\n", Ret);
            }

            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return Ret;
        }

        pstData->pu8Data = pAvplay->AvplayAudEsBuf.pu8Data;
        pstData->u32Size = pAvplay->AvplayAudEsBuf.u32Size;
    }
#endif

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
    return HI_SUCCESS;
}


HI_S32 HI_DRV_AVPLAY_PutBuf(HI_HANDLE hAvplay, HI_UNF_AVPLAY_BUFID_E enBufId,
                                       HI_U32 u32ValidDataLen, HI_U32 u32Pts, HI_UNF_AVPLAY_PUTBUFEX_OPT_S *pstExOpt)
{
    MCE_AVPLAY_S    *pAvplay = HI_NULL;
    HI_S32               Ret;

    if (enBufId >= HI_UNF_AVPLAY_BUF_ID_BUTT)
    {
        HI_ERR_MCE("para enBufId is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
    {
        HI_ERR_MCE("avplay is ts stream mode.\n");
        DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    if (HI_UNF_AVPLAY_BUF_ID_ES_VID == enBufId)
    {
        if (!pAvplay->VidEnable)
        {
            HI_ERR_MCE("vid chn is stopped.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        pAvplay->AvplayVidEsBuf.u32BufSize = u32ValidDataLen;
        pAvplay->AvplayVidEsBuf.u64Pts = u32Pts;
        pAvplay->AvplayVidEsBuf.bEndOfFrame = pstExOpt->bEndOfFrm;

        if (pstExOpt->bContinue)
        {
            pAvplay->AvplayVidEsBuf.bDiscontinuous = HI_FALSE;
        }
        else
        {
            pAvplay->AvplayVidEsBuf.bDiscontinuous = HI_TRUE;
        }

        Ret = HI_DRV_VDEC_PutEsBuf(pAvplay->hVdec, &pAvplay->AvplayVidEsBuf);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("HI_DRV_VDEC_PutEsBuf failed 0x%x\n", Ret);
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return Ret;
        }
    }

#if 0

    if (HI_UNF_AVPLAY_BUF_ID_ES_AUD == enBufId)
    {
        if (!pAvplay->AudEnable)
        {
            HI_ERR_MCE("aud chn is stopped.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_ERR_AVPLAY_INVALID_OPT;
        }
        pAvplay->AvplayAudEsBuf.u32Size = u32ValidDataLen;
        Ret = HI_DRV_ADEC_PutEsBuf(pAvplay->hAdec, &pAvplay->AvplayAudEsBuf, u32Pts);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_MCE("HI_DRV_ADEC_PutEsBuf failed 0x%x\n", Ret);
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return Ret;
        }
    }
#endif

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
    return HI_SUCCESS;
}

HI_S32 HI_DRV_AVPLAY_GetSyncVdecHandle(HI_HANDLE hAvplay, HI_HANDLE *phVdec, HI_HANDLE *phSync)
{
    HI_S32          Ret = HI_SUCCESS;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    if(HI_INVALID_HANDLE == pAvplay->hVdec)
    {
        HI_ERR_MCE("ERR: vdec handle is invalid\n");
        return HI_FAILURE;
    }

    if(HI_INVALID_HANDLE == pAvplay->hSync)
    {
        HI_ERR_MCE("ERR: sync handle is invalid\n");
        return HI_FAILURE;
    }

    *phVdec = pAvplay->hVdec;
    *phSync = pAvplay->hSync;

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));

    return Ret;
}

HI_S32 HI_DRV_AVPLAY_GetSndHandle(HI_HANDLE hAvplay, HI_HANDLE *phSnd)
{
    HI_S32          Ret = HI_SUCCESS;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    if(HI_INVALID_HANDLE == pAvplay->hSnd)
    {
        HI_ERR_MCE("ERR: Snd handle is invalid\n");
        return HI_FAILURE;
    }

    *phSnd = pAvplay->hSnd;

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));

    return Ret;
}

HI_S32 HI_DRV_AVPLAY_AttachWindow(HI_HANDLE hAvplay, HI_HANDLE hWindow)
{
    HI_S32          Ret = HI_SUCCESS;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;
    HI_DRV_WIN_INFO_S stWinInfo;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_MCE("para hWindow is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    Ret = HI_DRV_WIN_GetInfo(hWindow, &stWinInfo);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_MPI_WIN_GetPrivnfo.\n");
        DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    /* homologous window*/
    if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == stWinInfo.eType)
    {
        if (pAvplay->MasterFrmChn.hWindow == stWinInfo.hPrim)
        {
            HI_ERR_MCE("this window is already attached.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_SUCCESS;
        }

        /* if attach homologous window, homologous window must be master window*/
        if (HI_INVALID_HANDLE != pAvplay->MasterFrmChn.hWindow)
        {
            HI_ERR_MCE("avplay can only attach one master handle.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        if (HI_INVALID_HANDLE != pAvplay->SlaveFrmChn.hWindow)
        {
            HI_ERR_MCE("avplay has attached slave window.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_ERR_AVPLAY_INVALID_OPT;
        }
        Ret = DRV_AVPLAY_CreatePort(pAvplay, stWinInfo.hPrim, VDEC_PORT_HD, &(pAvplay->MasterFrmChn.hPort));
        if(HI_SUCCESS != Ret)
        {
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return Ret;
        }

        Ret = DRV_AVPLAY_SetPortAttr(pAvplay,pAvplay->MasterFrmChn.hPort, HI_TRUE);
        if(HI_SUCCESS != Ret)
        {
            (HI_VOID)DRV_AVPLAY_DestroyPort(pAvplay, stWinInfo.hPrim, pAvplay->MasterFrmChn.hPort);
            pAvplay->MasterFrmChn.hPort = HI_INVALID_HANDLE;

            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return Ret;
        }

#ifdef MCE_ACTIVE_MAIN_AND_SLAVE_WIN_SUPPORT
        Ret = DRV_AVPLAY_CreatePort(pAvplay, stWinInfo.hSec, VDEC_PORT_SD, &(pAvplay->SlaveFrmChn.hPort));
        if(HI_SUCCESS != Ret)
        {
            (HI_VOID)DRV_AVPLAY_DestroyPort(pAvplay, stWinInfo.hPrim, pAvplay->MasterFrmChn.hPort);
            pAvplay->MasterFrmChn.hPort = HI_INVALID_HANDLE;

            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return Ret;
        }

        Ret = DRV_AVPLAY_SetPortAttr(pAvplay,pAvplay->SlaveFrmChn.hPort,HI_FALSE);
        if(HI_SUCCESS != Ret)
        {
            (HI_VOID)DRV_AVPLAY_DestroyPort(pAvplay, stWinInfo.hPrim, pAvplay->MasterFrmChn.hPort);
            pAvplay->MasterFrmChn.hPort = HI_INVALID_HANDLE;
            (HI_VOID)DRV_AVPLAY_DestroyPort(pAvplay, stWinInfo.hSec, pAvplay->SlaveFrmChn.hPort);
            pAvplay->SlaveFrmChn.hPort = HI_INVALID_HANDLE;

            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return Ret;
        }

        /* no master window, may be exist virtual window , and virtual window is full*/
        /* avoid block master window, we drop this frame, then to obtain new frame*/
        if (HI_TRUE == pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
        {
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
        }

        pAvplay->SlaveFrmChn.hWindow = stWinInfo.hSec;
#endif
        pAvplay->MasterFrmChn.hWindow = stWinInfo.hPrim;
        pAvplay->hSharedOrgWin = hWindow;
    }
    /*  analogous master window*/
    else if (HI_DRV_WIN_ACTIVE_SINGLE == stWinInfo.eType)
    {
        if (hWindow == pAvplay->MasterFrmChn.hWindow)
        {
            HI_ERR_MCE("this window is alreay attached!\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_SUCCESS;
        }
        else if(hWindow == pAvplay->SlaveFrmChn.hWindow)
        {
            HI_ERR_MCE("this window is alreay attached!\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_SUCCESS;
        }

        if (HI_INVALID_HANDLE == pAvplay->MasterFrmChn.hWindow)
        {
            Ret = DRV_AVPLAY_CreatePort(pAvplay, hWindow, VDEC_PORT_HD, &(pAvplay->MasterFrmChn.hPort));
            if (HI_SUCCESS != Ret)
            {
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return Ret;
            }

            Ret = DRV_AVPLAY_SetPortAttr(pAvplay,pAvplay->MasterFrmChn.hPort, HI_TRUE);
            if(HI_SUCCESS != Ret)
            {
                (HI_VOID)DRV_AVPLAY_DestroyPort(pAvplay, hWindow, pAvplay->MasterFrmChn.hPort);
                pAvplay->MasterFrmChn.hPort = HI_INVALID_HANDLE;

                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return Ret;
            }

            /* no master window, may be exist virtual window , and virtual window is full*/
            /* avoid block master window, we drop this frame, then to obtain new frame*/
            if (HI_TRUE == pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
            {
                pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
            }

            pAvplay->MasterFrmChn.hWindow = hWindow;
        }
        else if (HI_INVALID_HANDLE == pAvplay->SlaveFrmChn.hWindow)
        {
            Ret = DRV_AVPLAY_CreatePort(pAvplay, hWindow, VDEC_PORT_SD, &(pAvplay->SlaveFrmChn.hPort));
            if(HI_SUCCESS != Ret)
            {
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return Ret;
            }

            Ret = DRV_AVPLAY_SetPortAttr(pAvplay,pAvplay->SlaveFrmChn.hPort,HI_FALSE);
            if(HI_SUCCESS != Ret)
            {
                (HI_VOID)DRV_AVPLAY_DestroyPort(pAvplay, hWindow, pAvplay->SlaveFrmChn.hPort);
                pAvplay->SlaveFrmChn.hPort = HI_INVALID_HANDLE;

                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return Ret;
            }

            pAvplay->SlaveFrmChn.hWindow = hWindow;
        }
        else
        {
            HI_ERR_MCE("mce can only attach one master handle and one slave handle.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_ERR_AVPLAY_INVALID_OPT;
        }
    }

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));

    return Ret;
}

HI_S32 HI_DRV_AVPLAY_DetachWindow(HI_HANDLE hAvplay, HI_HANDLE hWindow)
{
    HI_S32          Ret = HI_SUCCESS;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;
    HI_DRV_WIN_INFO_S WinInfo;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    if (HI_INVALID_HANDLE == hWindow)
    {
        HI_ERR_MCE("para hWindow is invalid.\n");
        return HI_ERR_AVPLAY_INVALID_PARA;
    }

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    Ret = HI_DRV_WIN_GetInfo(hWindow, &WinInfo);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_MCE("ERR: HI_MPI_VO_GetWindowInfo.\n");
        DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
        return HI_ERR_AVPLAY_INVALID_OPT;
    }

    /* homologous window*/ /* 同源窗口 */
    if (HI_DRV_WIN_ACTIVE_MAIN_AND_SLAVE == WinInfo.eType)
    {
#ifdef MCE_ACTIVE_MAIN_AND_SLAVE_WIN_SUPPORT
        if (pAvplay->MasterFrmChn.hWindow != WinInfo.hPrim)
        {
            HI_ERR_MCE("ERR: this is not a attached window.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_ERR_AVPLAY_INVALID_OPT;
        }
        else if(pAvplay->SlaveFrmChn.hWindow != WinInfo.hSec)
        {
            HI_ERR_MCE("ERR: this is not a attached window.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_ERR_AVPLAY_INVALID_OPT;
        }


        /* had recevied frmpack and frm is hold in AVPLAY or WIN(frc) */
        /*vir chn don't care*/
        if (HI_TRUE == pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
        {
            (HI_VOID)DRV_AVPLAY_RelAllChnFrame(pAvplay);
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
        }

        Ret = DRV_AVPLAY_DestroyPort(pAvplay, pAvplay->MasterFrmChn.hWindow, pAvplay->MasterFrmChn.hPort);
        Ret |= DRV_AVPLAY_DestroyPort(pAvplay, pAvplay->SlaveFrmChn.hWindow, pAvplay->SlaveFrmChn.hPort);
        if (HI_SUCCESS != Ret)
        {
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        pAvplay->MasterFrmChn.hWindow = HI_INVALID_HANDLE;
        pAvplay->MasterFrmChn.hPort = HI_INVALID_HANDLE;

        pAvplay->SlaveFrmChn.hWindow = HI_INVALID_HANDLE;
        pAvplay->SlaveFrmChn.hPort = HI_INVALID_HANDLE;

        pAvplay->hSharedOrgWin = HI_INVALID_HANDLE;
#else
        if (pAvplay->MasterFrmChn.hWindow != WinInfo.hPrim)
        {
            HI_ERR_MCE("ERR: this is not a attached window.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        /* had recevied frmpack and frm is hold in AVPLAY or WIN(frc) */
        /*vir chn don't care*/
        if (HI_TRUE == pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
        {
            (HI_VOID)DRV_AVPLAY_RelAllChnFrame(pAvplay);
            pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
        }

        Ret = DRV_AVPLAY_DestroyPort(pAvplay, pAvplay->MasterFrmChn.hWindow, pAvplay->MasterFrmChn.hPort);
        if (HI_SUCCESS != Ret)
        {
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        pAvplay->MasterFrmChn.hWindow = HI_INVALID_HANDLE;
        pAvplay->MasterFrmChn.hPort = HI_INVALID_HANDLE;
        pAvplay->hSharedOrgWin = HI_INVALID_HANDLE;
#endif
    }
    /*  analogous master window*/ /* 非同源 主窗口及从窗口 */
    else if (HI_DRV_WIN_ACTIVE_SINGLE == WinInfo.eType)
    {
        if (pAvplay->MasterFrmChn.hWindow == hWindow)
        {
            /* had recevied frmpack and frm is hold in AVPLAY or WIN(frc) */
            /*vir chn don't care*/
            if (HI_TRUE == pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO])
            {
                (HI_VOID)DRV_AVPLAY_RelAllChnFrame(pAvplay);
                pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO] = HI_FALSE;
            }

            Ret = DRV_AVPLAY_DestroyPort(pAvplay, pAvplay->MasterFrmChn.hWindow, pAvplay->MasterFrmChn.hPort);
            if (HI_SUCCESS != Ret)
            {
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return HI_ERR_AVPLAY_INVALID_OPT;
            }

            pAvplay->MasterFrmChn.hWindow = HI_INVALID_HANDLE;
            pAvplay->MasterFrmChn.hPort = HI_INVALID_HANDLE;

            //change slave to master
            if (HI_INVALID_HANDLE != pAvplay->SlaveFrmChn.hWindow)
            {
                pAvplay->MasterFrmChn.hWindow = pAvplay->SlaveFrmChn.hWindow;
                pAvplay->MasterFrmChn.hPort = pAvplay->SlaveFrmChn.hPort;

                Ret = DRV_AVPLAY_SetPortAttr(pAvplay,pAvplay->MasterFrmChn.hPort, HI_TRUE);
                if(HI_SUCCESS != Ret)
                {
                    HI_ERR_MCE("ERR: set main port failed.\n");
                }

                pAvplay->SlaveFrmChn.hWindow = HI_INVALID_HANDLE;
                pAvplay->SlaveFrmChn.hPort = HI_INVALID_HANDLE;

            }
        }
        else if (pAvplay->SlaveFrmChn.hWindow == hWindow)
        {

            /*had received frmpack and frm is hold in AVPLAY, but has master window, don't need to receive new frmpack*/
            if ((HI_TRUE == pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO]))
            {
                (HI_VOID)DRV_AVPLAY_RelSpecialFrame(pAvplay, hWindow);
            }

            /*FATAL: after AVPLAY_DettachWinRelFrame, but AVPLAY_DestroyPort Failed*/
            Ret = DRV_AVPLAY_DestroyPort(pAvplay, hWindow, pAvplay->SlaveFrmChn.hPort);
            if (HI_SUCCESS != Ret)
            {
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                return HI_ERR_AVPLAY_INVALID_OPT;
            }

            pAvplay->SlaveFrmChn.hWindow = HI_INVALID_HANDLE;
            pAvplay->SlaveFrmChn.hPort = HI_INVALID_HANDLE;
        }
        else
        {
            HI_ERR_MCE("ERR: this is not a attached window.\n");
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            return HI_ERR_AVPLAY_INVALID_OPT;
        }
    }

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));

    return Ret;
}

HI_S32 HI_DRV_AVPLAY_AttachSnd(HI_HANDLE hAvplay, HI_HANDLE hSnd)
{
    HI_S32          Ret = HI_SUCCESS;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    pAvplay->hSnd = hSnd;

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));

    return Ret;
}

HI_S32 HI_DRV_AVPLAY_DetachSnd(HI_HANDLE hAvplay, HI_HANDLE hSnd)
{
    HI_S32          Ret = HI_SUCCESS;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    pAvplay->hSnd = HI_INVALID_HANDLE;

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));

    return Ret;
}

HI_S32 HI_DRV_AVPLAY_IsBufEmpty(HI_HANDLE hAvplay, HI_BOOL *pbIsEmpty)
{
    HI_S32              Ret = HI_SUCCESS;
    MCE_AVPLAY_S       *pAvplay = HI_NULL;
    HI_BOOL             AudioEmpty = HI_FALSE;
    HI_BOOL             VideoEmpty = HI_FALSE;

    if (!pbIsEmpty)
    {
        HI_ERR_MCE("Para pbIsEmpty is invalid\n");
        return HI_FAILURE;
    }

    *pbIsEmpty = HI_FALSE;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    if (pAvplay->AudEnable)
    {
        ADEC_STATUSINFO_S   AdecBuf;
        HI_U32              SndDelay = 0;
        HI_U32              AudPercent = 100;

        Ret = HI_DRV_ADEC_GetStatusInfo(pAvplay->hAdec, &AdecBuf);
        if (HI_SUCCESS == Ret)
        {
            AudPercent = AdecBuf.u32BufferUsed * 100 / AdecBuf.u32BufferSize;
        }

        Ret = HI_DRV_AO_Track_GetDelayMs(pAvplay->hSnd, &SndDelay);
        if (HI_SUCCESS == Ret)
        {
            if ((SndDelay < 20) && (AudPercent < 1))
            {
                AudioEmpty = HI_TRUE;
            }
        }
    }

    if (pAvplay->VidEnable)
    {
        HI_MPI_DMX_BUF_STATUS_S     VidDmxBuf;
        VDEC_STATUSINFO_S           VdecBuf;
        HI_U32                      VidPercent = 100;
        HI_U32                      u32DelayTime;

        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            Ret = HI_DRV_DMX_GetPESBufferStatus(pAvplay->hDmxVid, &VidDmxBuf);
            if (HI_SUCCESS == Ret)
            {
                VidPercent = VidDmxBuf.u32UsedSize * 100 / VidDmxBuf.u32BufSize;
            }
        }
        else
        {
            Ret = HI_DRV_VDEC_GetChanStatusInfo(pAvplay->hVdec, &VdecBuf);
            if (HI_SUCCESS == Ret)
            {
                VidPercent = VdecBuf.u32BufferUsed * 100 / VdecBuf.u32BufferSize;
            }
        }

        Ret = HI_DRV_WIN_GetDelayMs(pAvplay->MasterFrmChn.hWindow, &u32DelayTime);
        if (HI_SUCCESS == Ret)
        {
            if ((u32DelayTime < 50) && (VidPercent < 1))
            {
                VideoEmpty = HI_TRUE;
            }
        }

   }

    if (pAvplay->AudEnable && pAvplay->VidEnable)
    {
        if (AudioEmpty && VideoEmpty)
        {
            *pbIsEmpty = HI_TRUE;
        }
        else
        {
            *pbIsEmpty = HI_FALSE;
        }
    }
    else if (pAvplay->AudEnable)
    {
        *pbIsEmpty = AudioEmpty;
    }
    else if (pAvplay->VidEnable)
    {
        *pbIsEmpty = VideoEmpty;
    }
    else
    {
        *pbIsEmpty = HI_FALSE;
    }

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));

    return HI_SUCCESS;
}

HI_S32 HI_DRV_AVPLAY_ProcStatus(HI_HANDLE hAvplay, HI_UNF_AVPLAY_BUFID_E enBufId, HI_BOOL *pbStatus)
{
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    if (enBufId >= HI_UNF_AVPLAY_BUF_ID_BUTT)
    {
        return HI_FAILURE;
    }

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayThreadMutex));

    if (enBufId == HI_UNF_AVPLAY_BUF_ID_ES_VID)
    {
        *pbStatus = pAvplay->AvplayProcDataFlag[AVPLAY_PROC_VDEC_VO];
    }
    else
    {
        *pbStatus = pAvplay->AvplayProcDataFlag[AVPLAY_PROC_ADEC_AO];
    }

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayThreadMutex));
    return HI_SUCCESS;
}

HI_S32 HI_DRV_AVPLAY_FlushStream(HI_HANDLE hAvplay)
{
    HI_S32                  Ret;
    MCE_AVPLAY_S    *pAvplay = HI_NULL;

    AVPLAY_GET_BY_HANDLE(hAvplay, pAvplay);

    DRV_AVPLAY_LOCK(&(pAvplay->AvplayMutex));

    if (pAvplay->bSetEosFlag)
    {
        DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
        HI_INFO_MCE("Eos Flag has been set!\n");
        return HI_SUCCESS;
    }

    if (pAvplay->VidEnable)
    {
        Ret = HI_DRV_VDEC_SetEosFlag(pAvplay->hVdec);
        if (HI_SUCCESS != Ret)
        {
            DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
            HI_ERR_MCE("ERR: HI_DRV_VDEC_SetEosFlag, Ret=%#x!\n", Ret);
            return HI_ERR_AVPLAY_INVALID_OPT;
        }

        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            Ret = HI_DRV_DMX_SetChannelEosFlag(pAvplay->hDmxVid);
            if (HI_SUCCESS != Ret)
            {
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                HI_ERR_MCE("ERR: HI_DRV_DMX_SetChannelEosFlag, Ret=%#x!\n", Ret);
                return HI_ERR_AVPLAY_INVALID_OPT;
            }
        }
    }

    if (pAvplay->AudEnable)
    {
        if (HI_UNF_AVPLAY_STREAM_TYPE_TS == pAvplay->AvplayAttr.stStreamAttr.enStreamType)
        {
            Ret = HI_DRV_DMX_SetChannelEosFlag(pAvplay->hDmxAud);
            if (HI_SUCCESS != Ret)
            {
                DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
                HI_ERR_MCE("ERR: HI_DRV_DMX_SetChannelEosFlag, Ret=%#x!\n", Ret);
                return HI_ERR_AVPLAY_INVALID_OPT;
            }
        }
    }

    pAvplay->bSetEosFlag = HI_TRUE;

    DRV_AVPLAY_UNLOCK(&(pAvplay->AvplayMutex));
    return Ret;
}


