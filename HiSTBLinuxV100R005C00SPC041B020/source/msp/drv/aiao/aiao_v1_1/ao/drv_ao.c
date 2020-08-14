/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_ao.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/17
  Description   :
  History       :
  1.Date        : 2013/04/17
    Author      : zgjie
    Modification: Created file

******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <asm/io.h>

/* Unf headers */
#include "hi_error_mpi.h"
#include "hi_drv_module.h"
#include "hi_drv_proc.h"

/* Drv headers */
#include "hi_drv_ao.h"
#include "hi_drv_ai.h"
#include "drv_ao_ioctl.h"
#include "drv_ao_ext.h"
#include "drv_adsp_ext.h"
#include "drv_ao_private.h"

#include "hi_audsp_aoe.h"
#include "hal_aoe.h"
#include "hal_cast.h"
#include "hal_aiao.h"

#include "drv_ao_op.h"
#include "drv_ao_track.h"
#include "drv_ao_aef.h"
#include "audio_util.h"
#if defined (HI_AUDIO_AI_SUPPORT)
#include "drv_ai_private.h"
#endif

#include "drv_hdmi_ext.h"
#include "drv_ao_cast.h"
#include "hi_drv_file.h"

#ifdef HI_ALSA_AO_SUPPORT
#include <linux/platform_device.h>

#ifdef CONFIG_PM
extern int snd_soc_suspend(struct device* dev); //kernel inteface
extern int snd_soc_resume(struct device* dev);
static  HI_BOOL bu32shallowSuspendActive = HI_FALSE;
#endif

extern struct platform_device* hisi_snd_device;

#define PM_LOW_SUSPEND_FLAG   0x5FFFFFFF
#endif

#ifdef HI_AIAO_TIMER_SUPPORT
#include "drv_timer_private.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

struct file  g_filp;

#if defined(HI_AIAO_VERIFICATION_SUPPORT)
#include "drv_aiao_ioctl_veri.h"
extern HI_VOID AIAO_VERI_Open(HI_VOID);
extern HI_VOID AIAO_VERI_Release(HI_VOID);
extern HI_S32  AIAO_VERI_ProcRead(struct seq_file* p, HI_VOID* v);
extern HI_S32 AIAO_VERI_ProcessCmd( struct inode* inode, struct file* file, HI_U32 cmd, HI_VOID* arg );
#endif

#if (1 == HI_PROC_SUPPORT)
static HI_VOID  AO_RegProc(HI_U32 u32Snd);
static HI_VOID  AO_UnRegProc(HI_U32 u32Snd);
static HI_S32   AO_DRV_ReadProc(struct seq_file* file, HI_VOID* v);
static HI_S32   AO_DRV_WriteProc(struct file* file, const char __user* buf, size_t count, loff_t* ppos);
#endif

HI_S32 AO_DRV_Resume(PM_BASEDEV_S* pdev);
HI_S32 AO_DRV_Suspend(PM_BASEDEV_S* pdev, pm_message_t state);
HI_S32 AO_Track_GetDefAttr(HI_UNF_AUDIOTRACK_ATTR_S* pstDefAttr);
HI_S32 AO_Track_AllocHandle(HI_UNF_SND_E enSound, HI_UNF_SND_TRACK_TYPE_E enTrackType, struct file* pstFile, HI_HANDLE* phHandle);
HI_VOID AO_Track_FreeHandle(HI_HANDLE hHandle);
HI_S32 AO_Track_PreCreate(HI_UNF_SND_E enSound, HI_UNF_AUDIOTRACK_ATTR_S* pstAttr,
                          HI_BOOL bAlsaTrack, AO_BUF_ATTR_S* pstBuf, HI_HANDLE hTrack);
HI_S32 AO_Track_Destory(HI_U32 u32TrackID);
HI_S32 AO_Track_Start(HI_U32 u32TrackID);
HI_S32 AO_Track_Stop(HI_U32 u32TrackID);
HI_S32 AO_Track_SendData(HI_U32 u32TrackID, AO_FRAMEINFO_S* pstAOFrame);
HI_S32 AO_Track_GetDelayMs(HI_U32 u32TrackID, HI_U32* pu32DelayMs);

HI_S32 AO_DRV_Kopen(struct file* file);
HI_S32 AO_DRV_Krelease(struct file* file);
HI_S32 AO_Snd_Kclose(HI_UNF_SND_E arg, struct file* file);
HI_S32 AO_Snd_Kclose(HI_UNF_SND_E arg, struct file* file);

DEFINE_SEMAPHORE(g_AoMutex);

static AO_GLOBAL_PARAM_S s_stAoDrv =
{
    .u32TrackNum         =  0,
    .u32SndNum           =  0,
    .bLowLatencyCreated  =  HI_FALSE,
    .atmOpenCnt          = ATOMIC_INIT(0),
    .bReady              = HI_FALSE,

#if (1 == HI_PROC_SUPPORT)
    .stProcParam         =
    {
        .pfnReadProc     = AO_DRV_ReadProc,
        .pfnWriteProc    = AO_DRV_WriteProc,
    },
#endif

    .pAdspFunc           = HI_NULL,
    .pstPDMFunc          = HI_NULL,
    .stExtFunc           =
    {
        .pfnAO_DrvResume = AO_DRV_Resume,
        .pfnAO_DrvSuspend = AO_DRV_Suspend,

        .pfnAO_TrackGetDefAttr = AO_Track_GetDefAttr,
        .pfnAO_TrackAllocHandle = AO_Track_AllocHandle,
        .pfnAO_TrackFreeHandle = AO_Track_FreeHandle,

        .pfnAO_TrackCreate = AO_Track_PreCreate,
        .pfnAO_TrackDestory = AO_Track_Destory,
        .pfnAO_TrackStart = AO_Track_Start,
        .pfnAO_TrackStop = AO_Track_Stop,
        .pfnAO_TrackSendData = AO_Track_SendData,
    }
};

/***************************** Original Static Definition *****************************/
#ifdef HI_SND_USER_AEF_SUPPORT
static HI_S32 AOAllocAefBuf(SND_CARD_STATE_S* pCard)
{
    HI_S32 Ret;
    HI_U32 u32FrmSize, u32McFrmSize;
    HI_U32 u32BufSize, u32PeriodSize, u32DmxBufSize, u32BakDmxBufSize, u32AssocBufSize, u32AefBufSize;
    HI_U32 u32Samples;
#ifdef DRV_SND_24BIT_OUTPUT
    u32FrmSize = AUTIL_CalcFrameSize(AO_TRACK_NORMAL_CHANNELNUM, AO_TRACK_BITDEPTH_HIGH);
    u32McFrmSize = AUTIL_CalcFrameSize(AO_TRACK_MC_CHANNELNUM, AO_TRACK_BITDEPTH_HIGH);
#else
    u32FrmSize = AUTIL_CalcFrameSize(AO_TRACK_NORMAL_CHANNELNUM, AO_TRACK_BITDEPTH_LOW);
    u32McFrmSize = AUTIL_CalcFrameSize(AO_TRACK_MC_CHANNELNUM, AO_TRACK_BITDEPTH_LOW);
#endif
    u32Samples      = AOE_ENGINE_PROC_SAMPLES * AOE_AEF_PERIODBUF_NUM;
    u32DmxBufSize   = AOE_ENGINE_PROC_SAMPLES * u32FrmSize;
    u32AssocBufSize = u32DmxBufSize;
    u32McFrmSize    = AOE_ENGINE_PROC_SAMPLES * u32McFrmSize;
    u32BakDmxBufSize = u32DmxBufSize;
    u32AefBufSize   = u32DmxBufSize;
    u32PeriodSize   = sizeof(AOE_AEF_FRAME_INFO_S);
    u32PeriodSize  += u32DmxBufSize;
#ifdef DRV_SND_AD_OUTPUT_SUPPORT
    u32PeriodSize  += u32AssocBufSize;
#endif

#ifdef DRV_SND_MULTICH_AEF_SUPPORT
    u32PeriodSize  += u32McFrmSize;
#endif
    u32PeriodSize  += u32BakDmxBufSize;

    u32BufSize      = u32PeriodSize * AOE_AEF_PERIODBUF_NUM;

#if 0//#ifndef HI_SND_AOE_HW_SUPPORT  //sw aoe  //AOEIMP noise
    Ret = HI_DRV_MMZ_Alloc("AO_AefInbuf", MMZ_OTHERS, u32BufSize, AIAO_BUFFER_ADDR_ALIGN,
                           &pCard->stAefInBufMmz);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HI_DRV_MMZ_Alloc AO_AefInbuf failed\n");
        return Ret;
    }
    Ret = HI_DRV_MMZ_MapCache(&pCard->stAefInBufMmz);
    if (HI_SUCCESS != Ret)
    {
        HI_DRV_MMZ_Release(&pCard->stAefInBufMmz);
        HI_ERR_AO("HI_DRV_MMZ_MapCache AO_AefInbuf failed\n");
        return Ret;
    }
#else
    Ret = HI_DRV_MMZ_AllocAndMap("AO_AefInbuf", MMZ_OTHERS, u32BufSize, 0, &pCard->stAefInBufMmz);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HI_DRV_MMZ_AllocAndMap AO_AefInbuf failed\n");
        return Ret;
    }
#endif

    u32PeriodSize = u32DmxBufSize;

#ifdef DRV_SND_AD_OUTPUT_SUPPORT
    u32PeriodSize += u32AssocBufSize;
#endif

    u32PeriodSize += u32AefBufSize;
    u32BufSize = u32PeriodSize * AOE_AEF_PERIODBUF_NUM;

#if 0//#ifndef HI_SND_AOE_HW_SUPPORT  //sw aoe  //AOEIMP noise
    Ret = HI_DRV_MMZ_Alloc("AO_AefOutbuf", MMZ_OTHERS, u32BufSize, AIAO_BUFFER_ADDR_ALIGN,
                           &pCard->stAefOutBufMmz);
    if (HI_SUCCESS != Ret)
    {
        HI_DRV_MMZ_Unmap(&pCard->stAefInBufMmz);
        HI_DRV_MMZ_Release(&pCard->stAefInBufMmz);
        HI_ERR_AO("HI_DRV_MMZ_Alloc AO_AefOutbuf failed\n");
        return Ret;
    }
    Ret = HI_DRV_MMZ_MapCache(&pCard->stAefOutBufMmz);
    if (HI_SUCCESS != Ret)
    {
        HI_DRV_MMZ_Release(&pCard->stAefOutBufMmz);
        HI_DRV_MMZ_Unmap(&pCard->stAefInBufMmz);
        HI_DRV_MMZ_Release(&pCard->stAefInBufMmz);
        HI_ERR_AO("HI_DRV_MMZ_MapCache AO_AefOutbuf failed\n");
        return Ret;
    }
#else
    Ret = HI_DRV_MMZ_AllocAndMap("AO_AefOutbuf", MMZ_OTHERS, u32BufSize, 0, &pCard->stAefOutBufMmz);
    if (HI_SUCCESS != Ret)
    {
        HI_DRV_MMZ_UnmapAndRelease(&pCard->stAefInBufMmz);
        HI_ERR_AO("HI_DRV_MMZ_AllocAndMap AO_AefOutbuf failed\n");
        return Ret;
    }
#endif

    return HI_SUCCESS;
}

static HI_VOID AOReleaseAefBuf(SND_CARD_STATE_S* pCard)
{
#ifndef HI_SND_AOE_HW_SUPPORT  //sw aoe
    if (pCard->stAefOutBufMmz.u32StartPhyAddr)
    {
        HI_DRV_MMZ_Unmap(&pCard->stAefOutBufMmz);
        HI_DRV_MMZ_Release(&pCard->stAefOutBufMmz);
        memset(&pCard->stAefOutBufMmz, 0, sizeof(MMZ_BUFFER_S));
    }

    if (pCard->stAefInBufMmz.u32StartPhyAddr)
    {
        HI_DRV_MMZ_Unmap(&pCard->stAefInBufMmz);
        HI_DRV_MMZ_Release(&pCard->stAefInBufMmz);
        memset(&pCard->stAefInBufMmz, 0, sizeof(MMZ_BUFFER_S));
    }
#else  //hw aoe
    if (pCard->stAefOutBufMmz.u32StartPhyAddr)
    {
        HI_DRV_MMZ_UnmapAndRelease(&pCard->stAefOutBufMmz);
        memset(&pCard->stAefOutBufMmz, 0, sizeof(MMZ_BUFFER_S));
    }
    if (pCard->stAefInBufMmz.u32StartPhyAddr)
    {
        HI_DRV_MMZ_UnmapAndRelease(&pCard->stAefInBufMmz);
        memset(&pCard->stAefInBufMmz, 0, sizeof(MMZ_BUFFER_S));
    }
#endif
}
#endif

static SND_CARD_STATE_S* SND_CARD_GetCard(HI_UNF_SND_E enSound)
{
    return s_stAoDrv.astSndEntity[enSound].pCard;
}

static HI_UNF_SND_E SND_CARD_GetSnd(SND_CARD_STATE_S* pCard)
{
    HI_U32 i;

    for (i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
    {
        if (s_stAoDrv.astSndEntity[i].pCard == pCard)
        {
            return i;
        }
    }

    return AO_MAX_TOTAL_SND_NUM;
}

static HI_VOID AO_Snd_FreeHandle(HI_UNF_SND_E enSound, struct file* filp)
{
    SND_CARD_STATE_S* pCard = s_stAoDrv.astSndEntity[enSound].pCard;
    HI_U32 u32FileId;
    DRV_AO_STATE_S* pAOState = HI_NULL;

    if (HI_NULL == pCard)
    {
        return;
    }

    if (0 == atomic_read(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
    {
#ifdef HI_SND_USER_AEF_SUPPORT
        AOReleaseAefBuf(pCard);
#endif

        AUTIL_AO_FREE(HI_ID_AO, pCard);
        s_stAoDrv.astSndEntity[enSound].pCard = HI_NULL;
        s_stAoDrv.u32SndNum--;
    }

    if (HI_NULL == filp)
    {
        return;
    }

    pAOState = (DRV_AO_STATE_S*)filp->private_data;
    if (HI_NULL == pAOState)
    {
        return;
    }

    u32FileId = pAOState->u32FileId[enSound];
    if (u32FileId < SND_MAX_OPEN_NUM)
    {
        s_stAoDrv.astSndEntity[enSound].pstFile[u32FileId] = HI_NULL;
    }

    pAOState->u32FileId[enSound] = AO_SND_FILE_NOUSE_FLAG;
}

static HI_S32 SNDGetFreeFileId(HI_UNF_SND_E enSound)
{
    HI_U32 i;

    for (i = 0; i < SND_MAX_OPEN_NUM; i++)
    {
        if (s_stAoDrv.astSndEntity[enSound].pstFile[i] == HI_NULL)
        {
            return i;
        }
    }

    return SND_MAX_OPEN_NUM;
}

static HI_S32 AO_Snd_AllocHandle(HI_UNF_SND_E enSound, struct file* pstFile)
{
#ifdef HI_SND_USER_AEF_SUPPORT
    HI_S32 s32Ret;
#endif
    SND_CARD_STATE_S* pCard;
    HI_U32 u32FreeId;

    if (enSound >= HI_UNF_SND_BUTT)
    {
        HI_ERR_AO("Bad param!\n");
        return HI_ERR_AO_INVALID_ID;
    }

    /* Check ready flag */
    if (s_stAoDrv.bReady != HI_TRUE)
    {
        HI_ERR_AO("Need open first!\n");
        return HI_ERR_AO_DEV_NOT_OPEN;
    }

    u32FreeId = SNDGetFreeFileId(enSound);
    if (u32FreeId >= SND_MAX_OPEN_NUM)
    {
        HI_ERR_AO("Get free file id failed!\n");
        return HI_FAILURE;
    }

    if (AO_SND_FILE_NOUSE_FLAG == ((DRV_AO_STATE_S*)(pstFile->private_data))->u32FileId[enSound])
    {
        ((DRV_AO_STATE_S*)(pstFile->private_data))->u32FileId[enSound] = u32FreeId;
        s_stAoDrv.astSndEntity[enSound].pstFile[u32FreeId] = pstFile;
    }

    /* Allocate new snd resource */
    if (0 == atomic_read(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
    {
        pCard = (SND_CARD_STATE_S*)AUTIL_AO_MALLOC(HI_ID_AO, sizeof(SND_CARD_STATE_S), GFP_KERNEL);
        if (HI_NULL == pCard)
        {
            HI_ERR_AO("Kmalloc card failed!\n");
            goto err1;
        }
        memset(pCard, 0, sizeof(SND_CARD_STATE_S));

#ifdef HI_SND_USER_AEF_SUPPORT
        s32Ret = AOAllocAefBuf(pCard);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("Alloc aef buffer failed!\n");
            goto err2;
        }
#endif

        s_stAoDrv.astSndEntity[enSound].pCard = pCard;
        s_stAoDrv.u32SndNum++;
    }

    return HI_SUCCESS;

#ifdef HI_SND_USER_AEF_SUPPORT
err2:
    AUTIL_AO_FREE(HI_ID_AO, pCard);
#endif
err1:
    s_stAoDrv.astSndEntity[enSound].pstFile[u32FreeId] = HI_NULL;
    return HI_FAILURE;
}

/******************************Snd process FUNC*************************************/
static HI_BOOL AOCheckOutPortIsAttached(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort)
{
    HI_U32 u32Snd;
    HI_U32 u32Port;
    SND_CARD_STATE_S* pCard;

    // check if port is attached by another sound
    for (u32Snd = 0; u32Snd < (HI_U32)HI_UNF_SND_BUTT; u32Snd++)
    {
        if (u32Snd == (HI_U32)enSound)
        {
            continue;
        }

        pCard = SND_CARD_GetCard((HI_UNF_SND_E)u32Snd);
        if (HI_NULL == pCard)
        {
            continue;
        }

        for (u32Port = 0; u32Port < pCard->stUserOpenParam.u32PortNum; u32Port++)
        {
            if (enOutPort == pCard->stUserOpenParam.stOutport[u32Port].enOutPort)
            {
                return HI_TRUE;
            }
        }
    }

    return HI_FALSE;
}

#if 0
static HI_VOID AOGetI2SDefOpenAttr(HI_UNF_I2S_ATTR_S* pstAttr)
{
    pstAttr->bMaster = HI_TRUE;
    pstAttr->enI2sMode = HI_UNF_I2S_STD_MODE;
    pstAttr->enMclkSel = HI_UNF_I2S_MCLK_256_FS;
    pstAttr->enBclkSel = HI_UNF_I2S_BCLK_4_DIV;
    pstAttr->enChannel = HI_UNF_I2S_CHNUM_2;
    pstAttr->enBitDepth = HI_UNF_I2S_BIT_DEPTH_16;
    pstAttr->bPcmSampleRiseEdge = HI_TRUE;
    pstAttr->enPcmDelayCycle = HI_UNF_I2S_PCM_1_DELAY;
}
#endif

static HI_S32 AOGetOpenAttrFromPDM(AO_SND_OpenDefault_Param_S* pstSndDefaultAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 i;
    HI_UNF_PDM_SOUND_PARAM_S stSoundPdmParam;

    if (!(s_stAoDrv.pstPDMFunc && s_stAoDrv.pstPDMFunc->pfnGetSoundParam))
    {
        HI_WARN_AO("Get PDMFunc Symbol failed, use default param!\n");
        return HI_FAILURE;
    }

    s32Ret = (s_stAoDrv.pstPDMFunc->pfnGetSoundParam)(pstSndDefaultAttr->enSound, &stSoundPdmParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_AO("Get PDM param failed, use default param!\n");
        return s32Ret;
    }

    if (HI_UNF_SND_OUTPUTPORT_MAX < stSoundPdmParam.u32PortNum || 0 == stSoundPdmParam.u32PortNum)
    {
        HI_ERR_AO("Get PDM param invalid u32PortNum = 0x%x\n", stSoundPdmParam.u32PortNum);
        return HI_ERR_AO_INVALID_PARA;
    }

    pstSndDefaultAttr->stAttr.u32PortNum = stSoundPdmParam.u32PortNum;
    for (i = 0; i < stSoundPdmParam.u32PortNum; i++)
    {
        pstSndDefaultAttr->stAttr.stOutport[i].enOutPort = stSoundPdmParam.stOutport[i].enOutPort;
        switch (pstSndDefaultAttr->stAttr.stOutport[i].enOutPort)
        {
            case HI_UNF_SND_OUTPUTPORT_DAC0:
                HI_INFO_AO(">> HI_UNF_SND_OUTPUTPORT_DAC0\n");
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.u32DacAttr = HI_NULL;
                break;

            case HI_UNF_SND_OUTPUTPORT_SPDIF0:
                HI_INFO_AO(">> HI_UNF_SND_OUTPUTPORT_SPDIF0\n");
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.u32SpdifAttr = HI_NULL;
                break;

            case HI_UNF_SND_OUTPUTPORT_HDMI0:
                HI_INFO_AO(">> HI_UNF_SND_OUTPUTPORT_HDMI0\n");
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.u32HDMIAttr = HI_NULL;
                break;

            case HI_UNF_SND_OUTPUTPORT_I2S0:    //default support
                HI_INFO_AO(">> HI_UNF_SND_OUTPUTPORT_I2S0\n");
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.bMaster = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.bMaster;
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enI2sMode = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.enI2sMode;
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enMclkSel = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.enMclkSel;
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enBclkSel = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.enBclkSel;
                /*Compatible with old base param,  force to 2ch output */
                //pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enChannel = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.enChannel;
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enChannel = HI_UNF_I2S_CHNUM_2;
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enBitDepth = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.enBitDepth;
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.bPcmSampleRiseEdge = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.bPcmSampleRiseEdge;
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enPcmDelayCycle = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.enPcmDelayCycle;
                HI_INFO_AO("bMaster:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.bMaster);
                HI_INFO_AO("enI2sMode:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enI2sMode);
                HI_INFO_AO("enMclkSel:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enMclkSel);
                HI_INFO_AO("enBclkSel:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enBclkSel);
                HI_INFO_AO("enChannel:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enChannel);
                HI_INFO_AO("enBitDepth:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enBitDepth);
                HI_INFO_AO("bPcmSampleRiseEdge:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.bPcmSampleRiseEdge);
                HI_INFO_AO("enPcmDelayCycle:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enPcmDelayCycle);
                break;

            case HI_UNF_SND_OUTPUTPORT_I2S1:    //default support
                HI_INFO_AO(">> HI_UNF_SND_OUTPUTPORT_I2S1\n");
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.bMaster = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.bMaster;
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enI2sMode = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.enI2sMode;
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enMclkSel = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.enMclkSel;
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enBclkSel = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.enBclkSel;
                /* I2S do not support 1&8 channel output, so force to 2ch output */
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enChannel = HI_UNF_I2S_CHNUM_2;
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enBitDepth = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.enBitDepth;
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.bPcmSampleRiseEdge = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.bPcmSampleRiseEdge;
                pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enPcmDelayCycle = stSoundPdmParam.stOutport[i].unAttr.stI2sAttr.stAttr.enPcmDelayCycle;
                HI_INFO_AO("bMaster:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.bMaster);
                HI_INFO_AO("enI2sMode:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enI2sMode);
                HI_INFO_AO("enMclkSel:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enMclkSel);
                HI_INFO_AO("enBclkSel:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enBclkSel);
                HI_INFO_AO("enChannel:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enChannel);
                HI_INFO_AO("enBitDepth:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enBitDepth);
                HI_INFO_AO("bPcmSampleRiseEdge:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.bPcmSampleRiseEdge);
                HI_INFO_AO("enPcmDelayCycle:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].unAttr.stI2sAttr.stAttr.enPcmDelayCycle);
                break;

            default:
                HI_WARN_AO("SND Not support OUTPUTPORT:0x%x\n", pstSndDefaultAttr->stAttr.stOutport[i].enOutPort);
                break;
        }
    }

    pstSndDefaultAttr->stAttr.enSampleRate = HI_UNF_SAMPLE_RATE_48K;
    return HI_SUCCESS;
}

/*
 * use bootargs to contrl audio output ports:
 * 1. example: "audio=noadac,nospdif",then adac apdif and hdmi will be closed
 * 2. if audio is not set in bootargs, then use default setting
 * 3. audio_options_string is defined in kernel
 */
extern char* audio_options_string;

static HI_S32 AOGetOpenAttrFromBootargs(AO_SND_OpenDefault_Param_S* pstAttr)
{
    if (HI_NULL == audio_options_string)
    {
        return HI_FAILURE;
    }

    pstAttr->stAttr.u32PortNum = 0;

    if (HI_NULL == strstr(audio_options_string, "noadac"))
    {
        pstAttr->stAttr.stOutport[pstAttr->stAttr.u32PortNum].enOutPort = HI_UNF_SND_OUTPUTPORT_DAC0;
        pstAttr->stAttr.stOutport[pstAttr->stAttr.u32PortNum].unAttr.u32DacAttr = HI_NULL;
        pstAttr->stAttr.u32PortNum++;
    }

    if (HI_NULL == strstr(audio_options_string, "nospdif"))
    {
        pstAttr->stAttr.stOutport[pstAttr->stAttr.u32PortNum].enOutPort = HI_UNF_SND_OUTPUTPORT_SPDIF0;
        pstAttr->stAttr.stOutport[pstAttr->stAttr.u32PortNum].unAttr.u32SpdifAttr = HI_NULL;
        pstAttr->stAttr.u32PortNum++;
    }

    if (HI_NULL == strstr(audio_options_string, "nohdmi"))
    {
        pstAttr->stAttr.stOutport[pstAttr->stAttr.u32PortNum].enOutPort = HI_UNF_SND_OUTPUTPORT_HDMI0;
        pstAttr->stAttr.stOutport[pstAttr->stAttr.u32PortNum].unAttr.u32HDMIAttr = HI_NULL;
        pstAttr->stAttr.u32PortNum++;
    }

    if (0 == pstAttr->stAttr.u32PortNum)
    {
        HI_ERR_AO("Invalid audio close port bootargs setting!\n");
        return HI_FAILURE;
    }

    pstAttr->stAttr.enSampleRate = HI_UNF_SAMPLE_RATE_48K;

    return HI_SUCCESS;
}

HI_S32 AOGetSndDefOpenAttr(AO_SND_OpenDefault_Param_S* pstSndDefaultAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = AOGetOpenAttrFromBootargs(pstSndDefaultAttr);
    if (HI_SUCCESS == s32Ret)
    {
        return HI_SUCCESS;
    }

    s32Ret = AOGetOpenAttrFromPDM(pstSndDefaultAttr);
    if (HI_SUCCESS == s32Ret)
    {
        return HI_SUCCESS;
    }

    pstSndDefaultAttr->stAttr.u32PortNum = 3;
    pstSndDefaultAttr->stAttr.stOutport[0].enOutPort = HI_UNF_SND_OUTPUTPORT_DAC0;
    pstSndDefaultAttr->stAttr.stOutport[0].unAttr.u32DacAttr = HI_NULL;
    pstSndDefaultAttr->stAttr.stOutport[1].enOutPort = HI_UNF_SND_OUTPUTPORT_SPDIF0;
    pstSndDefaultAttr->stAttr.stOutport[1].unAttr.u32SpdifAttr = HI_NULL;
    pstSndDefaultAttr->stAttr.stOutport[2].enOutPort = HI_UNF_SND_OUTPUTPORT_HDMI0;
    pstSndDefaultAttr->stAttr.stOutport[2].unAttr.u32HDMIAttr = HI_NULL;

    pstSndDefaultAttr->stAttr.enSampleRate = HI_UNF_SAMPLE_RATE_48K;

#if 0 //#if defined(HI_I2S0_SUPPORT)
    pstSndDefaultAttr->stAttr.stOutport[pstSndDefaultAttr->stAttr.u32PortNum].enOutPort = HI_UNF_SND_OUTPUTPORT_I2S0;
    AOGetI2SDefOpenAttr(&pstSndDefaultAttr->stAttr.stOutport[pstSndDefaultAttr->stAttr.u32PortNum].unAttr.stI2sAttr.stAttr);
    pstSndDefaultAttr->stAttr.u32PortNum++;
#endif

#if 0 //#if defined(HI_I2S1_SUPPORT)
    pstSndDefaultAttr->stAttr.stOutport[pstSndDefaultAttr->stAttr.u32PortNum].enOutPort = HI_UNF_SND_OUTPUTPORT_I2S1;
    AOGetI2SDefOpenAttr(&pstSndDefaultAttr->stAttr.stOutport[pstSndDefaultAttr->stAttr.u32PortNum].unAttr.stI2sAttr.stAttr);
    pstSndDefaultAttr->stAttr.stOutport[pstSndDefaultAttr->stAttr.u32PortNum].unAttr.stI2sAttr.stAttr.enBitDepth = HI_UNF_I2S_BIT_DEPTH_24;  //connet AMP, use 24bit avoiding little signal noise
    pstSndDefaultAttr->stAttr.u32PortNum++;
#endif

    return HI_SUCCESS;
}

static HI_BOOL AOCheckOutPortIsValid(HI_UNF_SND_OUTPUTPORT_E enOutPort)
{
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3719mv100_a) \
    || defined(CHIP_TYPE_hi3798cv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100)   \
    || defined(CHIP_TYPE_hi3716dv100)
    if ((HI_UNF_SND_OUTPUTPORT_DAC0 != enOutPort) &&
        (HI_UNF_SND_OUTPUTPORT_SPDIF0 != enOutPort) &&
        (HI_UNF_SND_OUTPUTPORT_HDMI0 != enOutPort) &&
        (HI_UNF_SND_OUTPUTPORT_I2S0 != enOutPort) &&
        (HI_UNF_SND_OUTPUTPORT_I2S1 != enOutPort))
    {
        HI_ERR_AO("just support I2S0, I2S1, DAC0, SPDIF0 and HDMI0 Port!\n");
        return HI_FALSE;
    }
#elif    defined(CHIP_TYPE_hi3719mv100)   \
      || defined(CHIP_TYPE_hi3718mv100)   \
      || defined(CHIP_TYPE_hi3798cv200_a) \
      || defined(CHIP_TYPE_hi3798cv200_b) \
      || defined(CHIP_TYPE_hi3798cv200)   \
      || defined(CHIP_TYPE_hi3798mv200)   \
      || defined(CHIP_TYPE_hi3798mv200_a) \
      || defined(CHIP_TYPE_hi3716mv420)   \
      || defined(CHIP_TYPE_hi3796mv200)
    if ((HI_UNF_SND_OUTPUTPORT_DAC0 != enOutPort) &&
        (HI_UNF_SND_OUTPUTPORT_SPDIF0 != enOutPort) &&
        (HI_UNF_SND_OUTPUTPORT_HDMI0 != enOutPort) &&
        (HI_UNF_SND_OUTPUTPORT_I2S0 != enOutPort))
    {
        HI_ERR_AO("just support I2S0, DAC0, SPDIF0 and HDMI0 Port!\n");
        return HI_FALSE;
    }
#elif defined(CHIP_TYPE_hi3716mv410)
    if ((HI_UNF_SND_OUTPUTPORT_DAC0 != enOutPort) &&
        (HI_UNF_SND_OUTPUTPORT_SPDIF0 != enOutPort) &&
        (HI_UNF_SND_OUTPUTPORT_HDMI0 != enOutPort))
    {
        HI_ERR_AO("just support DAC0, SPDIF0 and HDMI0 Port!\n");
        return HI_FALSE;
    }
#elif defined(CHIP_TYPE_hi3751v100)  || defined(CHIP_TYPE_hi3751v100b)
    if ((HI_UNF_SND_OUTPUTPORT_DAC0 != enOutPort) && (HI_UNF_SND_OUTPUTPORT_EXT_DAC1 != enOutPort)
        && (HI_UNF_SND_OUTPUTPORT_EXT_DAC2 != enOutPort) && (HI_UNF_SND_OUTPUTPORT_EXT_DAC3 != enOutPort)
        && (HI_UNF_SND_OUTPUTPORT_SPDIF0 != enOutPort) && (HI_UNF_SND_OUTPUTPORT_I2S0 != enOutPort)
        && (HI_UNF_SND_OUTPUTPORT_I2S1 != enOutPort))
    {
        HI_ERR_AO("just support I2S0, I2S1, DAC0, DAC1, DAC2, DAC3 and SPDIF0 Port!\n");
        return HI_FALSE;
    }
#elif defined(CHIP_TYPE_hi3751v600) || defined (CHIP_TYPE_hi3751lv500) || defined(CHIP_TYPE_hi3751v620)
    if ((HI_UNF_SND_OUTPUTPORT_DAC0 != enOutPort) && (HI_UNF_SND_OUTPUTPORT_EXT_DAC1 != enOutPort)
        && (HI_UNF_SND_OUTPUTPORT_EXT_DAC2 != enOutPort) && (HI_UNF_SND_OUTPUTPORT_SPDIF0 != enOutPort)
        && (HI_UNF_SND_OUTPUTPORT_I2S0 != enOutPort) && (HI_UNF_SND_OUTPUTPORT_I2S1 != enOutPort))
    {
        HI_ERR_AO("just support I2S0, I2S1, DAC0, DAC1, DAC2 and SPDIF0 Port!\n");
        return HI_FALSE;
    }
#elif defined(CHIP_TYPE_hi3751v500)
    if ((HI_UNF_SND_OUTPUTPORT_DAC0 != enOutPort) && (HI_UNF_SND_OUTPUTPORT_EXT_DAC1 != enOutPort)
        && (HI_UNF_SND_OUTPUTPORT_EXT_DAC2 != enOutPort) && (HI_UNF_SND_OUTPUTPORT_SPDIF0 != enOutPort)
        && (HI_UNF_SND_OUTPUTPORT_I2S1 != enOutPort))
    {
        HI_ERR_AO("just support I2S0, I2S1, DAC0, DAC1, DAC2 and SPDIF0 Port!\n");
        return HI_FALSE;
    }
#else
#error YOU MUST DEFINE  CHIP_TYPE!
#endif

    return HI_TRUE;
}

#ifdef __DPT__
static HI_VOID AOGetDefAvcAttr(HI_UNF_SND_AVC_ATTR_S* pstAvcAttr)
{
    HI_UNF_SND_AVC_ATTR_S stDfAvcAttr = {50, 100, -32, 5, -10};
    memcpy(pstAvcAttr, &stDfAvcAttr, sizeof(HI_UNF_SND_AVC_ATTR_S));
}

static HI_VOID AOGetDefGeqAttr(HI_UNF_SND_GEQ_ATTR_S* pstGeqAttr)
{
    HI_UNF_SND_GEQ_ATTR_S stDfGeqAttr =
    {
        5,
        {
            {120, 0},
            {500, 0},
            {1500, 0},
            {5000, 0},
            {10000, 0},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0},
            {0, 0}
        }
    };
    memcpy(pstGeqAttr, &stDfGeqAttr, sizeof(HI_UNF_SND_GEQ_ATTR_S));
}
#endif

#ifndef HI_SND_AOE_HW_SUPPORT
static HI_S32 AOAllocCacheAipBuf(SND_CARD_STATE_S* pCard)
{
    HI_S32 Ret = HI_SUCCESS;

    Ret = HI_DRV_MMZ_Alloc("AO_MAipPcm", MMZ_OTHERS, AO_TRACK_PCM_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                           &pCard->stTrackRbfMmz[SND_AIP_TYPE_PCM]);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("MMZ_AllocAndMap(AO_MAipPcm) failed\n");
        goto ALLOCATE_MAIPPCM_ERR;
    }

    Ret = HI_DRV_MMZ_MapCache(&pCard->stTrackRbfMmz[SND_AIP_TYPE_PCM]);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HI_DRV_MMZ_MapCache(AO_MAipPcm) failed\n");
        goto MAPCACHE_MAIPPCM_ERR;
    }

#ifdef DRV_SND_MULTICH_AEF_SUPPORT
    Ret = HI_DRV_MMZ_Alloc("AO_MAipMcPcm", MMZ_OTHERS, AO_TRACK_HBR_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                           &pCard->stTrackRbfMmz[SND_AIP_TYPE_MC_PCM]);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("MMZ_AllocAndMap(AO_MAipMcPcm) failed\n");
        goto ALLOCATE_MAIPMCPCM_ERR;
    }

    Ret = HI_DRV_MMZ_MapCache(&pCard->stTrackRbfMmz[SND_AIP_TYPE_MC_PCM]);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HI_DRV_MMZ_MapCache(AO_MAipMcPcm) failed\n");
        goto MAPCACHE_MAIPMCPCM_ERR;
    }
#endif

#ifdef DRV_SND_AD_OUTPUT_SUPPORT
    Ret = HI_DRV_MMZ_Alloc("AO_MAipAssocPcm", MMZ_OTHERS, AO_TRACK_PCM_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                           &pCard->stTrackRbfMmz[SND_AIP_TYPE_ASSOC_PCM]);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("MMZ_AllocAndMap(AO_MAipAssocPcm) failed\n");
        goto ALLOCATE_MAIPASSOCPCM_ERR;
    }

    Ret = HI_DRV_MMZ_MapCache(&pCard->stTrackRbfMmz[SND_AIP_TYPE_ASSOC_PCM]);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HI_DRV_MMZ_MapCache(AO_MAipAssocPcm) failed\n");
        goto MAPCACHE_MAIPASSOCPCM_ERR;
    }
#endif

    if (SND_MODE_NONE != pCard->enSpdifPassthrough)
    {
        Ret = HI_DRV_MMZ_Alloc("AO_MAipSpdRaw", MMZ_OTHERS, AO_TRACK_LBR_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                               &pCard->stTrackRbfMmz[SND_AIP_TYPE_SPDIF_RAW]);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("MMZ_AllocAndMap(AO_MAipSpdRaw) failed\n");
            goto ALLOCATE_MAIPSPDRAW_ERR;
        }

        Ret = HI_DRV_MMZ_MapCache(&pCard->stTrackRbfMmz[SND_AIP_TYPE_SPDIF_RAW]);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HI_DRV_MMZ_MapCache(AO_MAipSpdRaw) failed\n");
            goto MAPCACHE_MAIPSPDRAW_ERR;
        }
    }

    if (SND_MODE_NONE != pCard->enHdmiPassthrough)
    {
        Ret = HI_DRV_MMZ_Alloc("AO_MAipHdmiRaw", MMZ_OTHERS, AO_TRACK_HBR_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                               &pCard->stTrackRbfMmz[SND_AIP_TYPE_HDMI_RAW]);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HI_DRV_MMZ_Alloc(AO_MAipHdmiRaw) failed\n");
            goto ALLOCATE_MAIPHDMIRAW_ERR;
        }

        Ret = HI_DRV_MMZ_MapCache(&pCard->stTrackRbfMmz[SND_AIP_TYPE_HDMI_RAW]);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HI_DRV_MMZ_MapCache(AO_MAipHdmiRaw) failed\n");
            goto MAPCACHE_MAIPHDMIRAW_ERR;
        }
    }

    return HI_SUCCESS;

MAPCACHE_MAIPHDMIRAW_ERR:
    if (SND_MODE_NONE != pCard->enHdmiPassthrough)
    {
        HI_DRV_MMZ_Release(&pCard->stTrackRbfMmz[SND_AIP_TYPE_HDMI_RAW]);
    }
ALLOCATE_MAIPHDMIRAW_ERR:
    if (SND_MODE_NONE != pCard->enSpdifPassthrough)
    {
        HI_DRV_MMZ_Unmap(&pCard->stTrackRbfMmz[SND_AIP_TYPE_SPDIF_RAW]);
    }
MAPCACHE_MAIPSPDRAW_ERR:
    if (SND_MODE_NONE != pCard->enSpdifPassthrough)
    {
        HI_DRV_MMZ_Release(&pCard->stTrackRbfMmz[SND_AIP_TYPE_SPDIF_RAW]);
    }
ALLOCATE_MAIPSPDRAW_ERR:

#ifdef DRV_SND_AD_OUTPUT_SUPPORT
    HI_DRV_MMZ_Unmap(&pCard->stTrackRbfMmz[SND_AIP_TYPE_ASSOC_PCM]);
MAPCACHE_MAIPASSOCPCM_ERR:
    HI_DRV_MMZ_Release(&pCard->stTrackRbfMmz[SND_AIP_TYPE_ASSOC_PCM]);
ALLOCATE_MAIPASSOCPCM_ERR:
#endif

#ifdef DRV_SND_MULTICH_AEF_SUPPORT
    HI_DRV_MMZ_Unmap(&pCard->stTrackRbfMmz[SND_AIP_TYPE_MC_PCM]);
MAPCACHE_MAIPMCPCM_ERR:
    HI_DRV_MMZ_Release(&pCard->stTrackRbfMmz[SND_AIP_TYPE_MC_PCM]);
ALLOCATE_MAIPMCPCM_ERR:
#endif

    HI_DRV_MMZ_Unmap(&pCard->stTrackRbfMmz[SND_AIP_TYPE_PCM]);
MAPCACHE_MAIPPCM_ERR:
    HI_DRV_MMZ_Release(&pCard->stTrackRbfMmz[SND_AIP_TYPE_PCM]);
ALLOCATE_MAIPPCM_ERR:

    return HI_FAILURE;
}

static HI_VOID AOReleaseCacheAipBuf(SND_CARD_STATE_S* pCard)
{
    HI_DRV_MMZ_Unmap(&pCard->stTrackRbfMmz[SND_AIP_TYPE_PCM]);
    HI_DRV_MMZ_Release(&pCard->stTrackRbfMmz[SND_AIP_TYPE_PCM]);

#ifdef DRV_SND_AD_OUTPUT_SUPPORT
    HI_DRV_MMZ_Unmap(&pCard->stTrackRbfMmz[SND_AIP_TYPE_ASSOC_PCM]);
    HI_DRV_MMZ_Release(&pCard->stTrackRbfMmz[SND_AIP_TYPE_ASSOC_PCM]);
#endif

#ifdef DRV_SND_MULTICH_AEF_SUPPORT
    HI_DRV_MMZ_Unmap(&pCard->stTrackRbfMmz[SND_AIP_TYPE_MC_PCM]);
    HI_DRV_MMZ_Release(&pCard->stTrackRbfMmz[SND_AIP_TYPE_MC_PCM]);
#endif

    if (SND_MODE_NONE != pCard->enSpdifPassthrough)
    {
        HI_DRV_MMZ_Unmap(&pCard->stTrackRbfMmz[SND_AIP_TYPE_SPDIF_RAW]);
        HI_DRV_MMZ_Release(&pCard->stTrackRbfMmz[SND_AIP_TYPE_SPDIF_RAW]);
    }

    if (SND_MODE_NONE != pCard->enHdmiPassthrough)
    {
        HI_DRV_MMZ_Unmap(&pCard->stTrackRbfMmz[SND_AIP_TYPE_HDMI_RAW]);
        HI_DRV_MMZ_Release(&pCard->stTrackRbfMmz[SND_AIP_TYPE_HDMI_RAW]);
    }
}

#else

static HI_S32 AOAllocNoCacheAipBuf(SND_CARD_STATE_S* pCard)
{
    HI_S32 Ret = HI_SUCCESS;

    Ret = HI_DRV_MMZ_AllocAndMap("AO_MAipPcm", MMZ_OTHERS, AO_TRACK_PCM_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                                 &pCard->stTrackRbfMmz[SND_AIP_TYPE_PCM]);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("MMZ_AllocAndMap(AO_MAipPcm) failed\n");
        goto ALLOCATE_MAIPPCM_ERR;
    }

#ifdef DRV_SND_MULTICH_AEF_SUPPORT
    Ret = HI_DRV_MMZ_AllocAndMap("AO_MAipMcPcm", MMZ_OTHERS, AO_TRACK_HBR_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                                 &pCard->stTrackRbfMmz[SND_AIP_TYPE_MC_PCM]);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("MMZ_AllocAndMap(AO_MAipMcPcm) failed\n");
        goto ALLOCATE_MAIPMCPCM_ERR;
    }
#endif

#ifdef DRV_SND_AD_OUTPUT_SUPPORT
    Ret = HI_DRV_MMZ_AllocAndMap("AO_MAipAssocPcm", MMZ_OTHERS, AO_TRACK_PCM_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                                 &pCard->stTrackRbfMmz[SND_AIP_TYPE_ASSOC_PCM]);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("MMZ_AllocAndMap(AO_MAipAssocPcm) failed\n");
        goto ALLOCATE_MAIPASSOCPCM_ERR;
    }
#endif

    if (SND_MODE_NONE != pCard->enSpdifPassthrough)
    {
        Ret = HI_DRV_MMZ_AllocAndMap("AO_MAipSpdRaw", MMZ_OTHERS, AO_TRACK_LBR_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                                     &pCard->stTrackRbfMmz[SND_AIP_TYPE_SPDIF_RAW]);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("MMZ_AllocAndMap(AO_MAipSpdRaw) failed\n");
            goto ALLOCATE_MAIPSPDRAW_ERR;
        }
    }

    if (SND_MODE_NONE != pCard->enHdmiPassthrough)
    {
        Ret = HI_DRV_MMZ_AllocAndMap("AO_MAipHdmiRaw", MMZ_OTHERS, AO_TRACK_HBR_BUFSIZE_BYTE_MAX, AIAO_BUFFER_ADDR_ALIGN,
                                     &pCard->stTrackRbfMmz[SND_AIP_TYPE_HDMI_RAW]);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("MMZ_AllocAndMap(AO_MAipHdmiRaw) failed\n");
            goto ALLOCATE_MAIPHDMIRAW_ERR;
        }
    }

    return HI_SUCCESS;

ALLOCATE_MAIPHDMIRAW_ERR:
    if (SND_MODE_NONE != pCard->enSpdifPassthrough)
    {
        HI_DRV_MMZ_UnmapAndRelease(&pCard->stTrackRbfMmz[SND_AIP_TYPE_SPDIF_RAW]);
    }

ALLOCATE_MAIPSPDRAW_ERR:
#ifdef DRV_SND_AD_OUTPUT_SUPPORT
    HI_DRV_MMZ_UnmapAndRelease(&pCard->stTrackRbfMmz[SND_AIP_TYPE_ASSOC_PCM]);
ALLOCATE_MAIPASSOCPCM_ERR:
#endif

#ifdef DRV_SND_MULTICH_AEF_SUPPORT
    HI_DRV_MMZ_UnmapAndRelease(&pCard->stTrackRbfMmz[SND_AIP_TYPE_MC_PCM]);
ALLOCATE_MAIPMCPCM_ERR:
#endif

    HI_DRV_MMZ_UnmapAndRelease(&pCard->stTrackRbfMmz[SND_AIP_TYPE_PCM]);
ALLOCATE_MAIPPCM_ERR:

    return HI_FAILURE;
}

static HI_VOID AOReleaseNoCacheAipBuf(SND_CARD_STATE_S* pCard)
{
    HI_DRV_MMZ_UnmapAndRelease(&pCard->stTrackRbfMmz[SND_AIP_TYPE_PCM]);

#ifdef DRV_SND_AD_OUTPUT_SUPPORT
    HI_DRV_MMZ_UnmapAndRelease(&pCard->stTrackRbfMmz[SND_AIP_TYPE_ASSOC_PCM]);
#endif

#ifdef DRV_SND_MULTICH_AEF_SUPPORT
    HI_DRV_MMZ_UnmapAndRelease(&pCard->stTrackRbfMmz[SND_AIP_TYPE_MC_PCM]);
#endif

    if (SND_MODE_NONE != pCard->enSpdifPassthrough)
    {
        HI_DRV_MMZ_UnmapAndRelease(&pCard->stTrackRbfMmz[SND_AIP_TYPE_SPDIF_RAW]);
    }

    if (SND_MODE_NONE != pCard->enHdmiPassthrough)
    {
        HI_DRV_MMZ_UnmapAndRelease(&pCard->stTrackRbfMmz[SND_AIP_TYPE_HDMI_RAW]);
    }
}
#endif

static HI_VOID AOInitCardState(SND_CARD_STATE_S* pCard)
{
    memset(pCard, 0, sizeof(SND_CARD_STATE_S));

    pCard->enPcmOutput = SND_PCM_OUTPUT_VIR_SPDIFORHDMI;
    pCard->enHdmiPassthrough = SND_MODE_NONE;
    pCard->enSpdifPassthrough = SND_MODE_NONE;
    pCard->bHdmiDebug = HI_FALSE;
    pCard->bCastSimulateOp = HI_FALSE;

#if (1 == HI_PROC_SUPPORT)
    pCard->u32SaveCnt = 0;
    pCard->enSaveState = SND_DEBUG_CMD_CTRL_STOP;
    pCard->pstFileHandle = HI_NULL;
#endif

#ifdef __DPT__
    pCard->enSaveAefState   = SND_DEBUG_CMD_CTRL_STOP;
    pCard->u32SaveAefCnt    = 0;
    pCard->pstAefFileHandle = HI_NULL;

    pCard->bAllTrackMute = HI_FALSE;
    pCard->bAllCastMute  = HI_FALSE;

    AOGetDefAvcAttr(&pCard->stAvcAttr);
    AOGetDefGeqAttr(&pCard->stGeqAttr);
    pCard->bGeqEnable = HI_FALSE;
    pCard->bAvcEnable = HI_FALSE;
#endif
}

static HI_S32 AOCheckOpAttr(DRV_SND_OUTPORT_S* pstOutPort)
{
#ifdef __DPT__
    HI_U32 u32PortFlag = 0;

    if (pstOutPort->enOutPort >= HI_UNF_SND_OUTPUTPORT_EXT_DAC1)
    {
        u32PortFlag |= (HI_U32)0x10000 << (pstOutPort->enOutPort & 0xf);
    }
    else
    {
        u32PortFlag |= (HI_U32)1 << pstOutPort->enOutPort;
    }

#if defined (CHIP_TYPE_hi3751v600) || defined (CHIP_TYPE_hi3751lv500) || defined (CHIP_TYPE_hi3751lv620)
    if ((u32PortFlag & ((HI_U32)1 << HI_UNF_SND_OUTPUTPORT_DAC0)) &&
        (u32PortFlag & ((HI_U32)1 << HI_UNF_SND_OUTPUTPORT_I2S1)))
    {
        HI_ERR_AO("I2S1 and DAC0 can not coexist!\n");
        return HI_FAILURE;
    }
#endif

#if defined (CHIP_TYPE_hi3751v500)
    if ((u32PortFlag & ((HI_U32)0x10000 << (HI_UNF_SND_OUTPUTPORT_EXT_DAC2 & 0xf))) &&
        (u32PortFlag & ((HI_U32)1 << HI_UNF_SND_OUTPUTPORT_I2S1)))
    {
        HI_ERR_AO("I2S1 and DAC2 can not coexist!\n");
        return HI_FAILURE;
    }
#endif
#endif

    return HI_SUCCESS;
}

static HI_S32 AOCheckOutPortAttr(HI_UNF_SND_E enSound, DRV_SND_OUTPORT_S* pstOutPort)
{
    if (HI_FALSE == AOCheckOutPortIsValid(pstOutPort->enOutPort))
    {
        HI_ERR_AO("port(%d) is invalid.\n", pstOutPort->enOutPort);
        return HI_FAILURE;
    }

    if (HI_TRUE == AOCheckOutPortIsAttached(enSound, pstOutPort->enOutPort))
    {
        HI_ERR_AO("port(%d) is aready attatched.\n", pstOutPort->enOutPort);
        return HI_FAILURE;
    }

    return AOCheckOpAttr(pstOutPort);
}

static HI_S32 AOInitHdmiAttr(SND_CARD_STATE_S* pCard)
{
    HI_S32 s32Ret;
    HDMI_AUDIO_ATTR_S stHdmiAttr;

    s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_HDMI, (HI_VOID**)&pCard->pstHdmiFunc);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Get hdmi function failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    if (HI_NULL == pCard->pstHdmiFunc)
    {
        HI_ERR_AO("Hdmi func is null\n");
        return HI_FAILURE;
    }

    if (pCard->pstHdmiFunc->pfnHdmiGetAoAttr)
    {
        (pCard->pstHdmiFunc->pfnHdmiGetAoAttr)(HI_UNF_HDMI_ID_0, &stHdmiAttr);
    }

    if (pCard->pstHdmiFunc->pfnHdmiAudioChange)
    {
        stHdmiAttr.enSoundIntf  = HDMI_AUDIO_INTERFACE_I2S;
        stHdmiAttr.enSampleRate = HI_UNF_SAMPLE_RATE_48K;
        stHdmiAttr.u32Channels  = AO_TRACK_NORMAL_CHANNELNUM;
        stHdmiAttr.enAudioCode  = HI_UNF_EDID_AUDIO_FORMAT_CODE_PCM;
        (pCard->pstHdmiFunc->pfnHdmiAudioChange)(HI_UNF_HDMI_ID_0, &stHdmiAttr);
    }

    return HI_SUCCESS;
}

static HI_S32 AOInitOutputMode(SND_CARD_STATE_S* pCard,
                               HI_UNF_SND_OUTPUTPORT_E enOutPort)
{
    if (HI_UNF_SND_OUTPUTPORT_HDMI0 == enOutPort)
    {
        HI_S32 s32Ret = AOInitHdmiAttr(pCard);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("Init hdmi attr failed(0x%x)\n", s32Ret);
            return s32Ret;
        }

        pCard->enHdmiPassthrough = SND_MODE_PCM;
    }
    else if (HI_UNF_SND_OUTPUTPORT_SPDIF0 == enOutPort)
    {
        pCard->enSpdifPassthrough = SND_MODE_PCM;
    }
    else
    {
        pCard->enPcmOutput = SND_PCM_OUTPUT_CERTAIN;
    }

    return HI_SUCCESS;
}

static HI_S32 AOAllocAipBuf(SND_CARD_STATE_S* pCard)
{
#ifndef HI_SND_AOE_HW_SUPPORT
    return AOAllocCacheAipBuf(pCard);
#else
    return AOAllocNoCacheAipBuf(pCard);
#endif
}

static HI_VOID AOReleaseAipBuf(SND_CARD_STATE_S* pCard)
{
#ifndef HI_SND_AOE_HW_SUPPORT
    AOReleaseCacheAipBuf(pCard);
#else
    AOReleaseNoCacheAipBuf(pCard);
#endif
}

HI_S32 AO_SND_Open(HI_UNF_SND_E enSound, DRV_SND_ATTR_S* pstAttr,
                   AO_ALSA_I2S_Param_S* pstAoI2sParam, HI_BOOL bResume)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32PortId;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_NULL_PTR(pstAttr);
    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_PORTNUM(pstAttr->u32PortNum);
    CHECK_AO_SAMPLERATE(pstAttr->enSampleRate);

    AOInitCardState(pCard);

    for (u32PortId = 0; u32PortId < pstAttr->u32PortNum; u32PortId++)
    {
        s32Ret = AOCheckOutPortAttr(enSound, &pstAttr->stOutport[u32PortId]);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("Check outport attr failed(0x%x)\n", s32Ret);
            return s32Ret;
        }

        s32Ret = AOInitOutputMode(pCard, pstAttr->stOutport[u32PortId].enOutPort);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("Init output mode failed(0x%x)\n", s32Ret);
            return s32Ret;
        }
    }

    s32Ret = SND_CreateOp(pCard, pstAttr, pstAoI2sParam, bResume);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Create snd op failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    s32Ret = AOAllocAipBuf(pCard);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Alloc Aip Buf failed(0x%x)\n", s32Ret);
        goto ERR_ALLOC_AIP;
    }

#if (1 == HI_PROC_SUPPORT)
    AO_RegProc((HI_U32)enSound);
#endif

    return HI_SUCCESS;

ERR_ALLOC_AIP:
    SND_DestroyOp(pCard, HI_FALSE);

    return s32Ret;
}

HI_S32 AO_SND_Close(HI_UNF_SND_E enSound, HI_BOOL bSuspend)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

#if (1 == HI_PROC_SUPPORT)
    AO_UnRegProc((HI_U32)enSound);
#endif

    AOReleaseAipBuf(pCard);
    SND_DestroyOp(pCard, bSuspend);
#ifdef __DPT__
    AEF_DestroyDebugAddr(pCard);
#endif
    TRACK_DestroyEngine(pCard);

    return HI_SUCCESS;
}

HI_S32 AO_SND_SetMute(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bMute)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpMute(pCard, enOutPort, bMute);
}

HI_S32 AO_SND_GetMute(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbMute)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pbMute);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpMute(pCard, enOutPort, pbMute);
}

HI_S32 AO_SND_GetAlsaTrackDelayMs(HI_UNF_SND_E enSound, HI_U32* pu32DelayMs)
{
    HI_U32 u32TrackCount = 0;
    HI_U32 u32TrackId = -1;
    SND_TRACK_STATE_S* pstTrackState;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pu32DelayMs);
    CHECK_AO_NULL_PTR(pCard);

    for (u32TrackCount = 0; u32TrackCount < AO_MAX_TOTAL_TRACK_NUM; u32TrackCount++)
    {
        pstTrackState = (SND_TRACK_STATE_S*)pCard->pSndTrack[u32TrackCount];
        if (pstTrackState && pstTrackState->bAlsaTrack)
        {
            u32TrackId = pstTrackState->TrackId;
            break;
        }
    }

    if (AO_MAX_TOTAL_TRACK_NUM == u32TrackCount)
    {
        return HI_FAILURE;
    }

    return AO_Track_GetDelayMs(u32TrackId, pu32DelayMs);
}

static HI_S32 AO_SND_SetHdmiMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_HDMI_MODE_E enMode)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_HDMIMODE(enMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpHdmiMode(pCard, enOutPort, enMode);
}

static HI_S32 AO_SND_GetHdmiMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_HDMI_MODE_E* penMode)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(penMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpHdmiMode(pCard, enOutPort, penMode);
}

static HI_S32 AO_SND_SetSpdifMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_SPDIF_MODE_E enMode)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_SPDIFMODE(enMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpSpdifMode(pCard, enOutPort, enMode);
}

static HI_S32 AO_SND_GetSpdifMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_SPDIF_MODE_E* penMode)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(penMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpSpdifMode(pCard, enOutPort, penMode);
}

#ifdef __DPT__
static HI_S32 AO_SND_SetArcEnable(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bEnable)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpArcEnable(pCard, enOutPort, bEnable);
}

static HI_S32 AO_SND_GetArcEnable(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbEnable)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pbEnable);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpArcEnable(pCard, enOutPort, pbEnable);
}

static HI_S32 AO_SND_SetArcMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_ARC_AUDIO_MODE_E enMode)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_ARCMODE(enMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpArcMode(pCard, enOutPort, enMode);
}

static HI_S32 AO_SND_GetArcMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_ARC_AUDIO_MODE_E* penMode)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(penMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpArcMode(pCard, enOutPort, penMode);
}

static HI_S32 AO_SND_SetArcCap(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_ARC_AUDIO_CAP_S stCap)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpArcCap(pCard, enOutPort, stCap);
}

static HI_S32 AO_SND_GetArcCap(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_ARC_AUDIO_CAP_S* pstCap)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pstCap);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpArcCap(pCard, enOutPort, pstCap);
}
#endif

HI_S32 AO_SND_SetVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S* pstGain)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pstGain);

    if (HI_TRUE == pstGain->bLinearMode)
    {
        CHECK_AO_LINEARVOLUME(pstGain->s32Gain);
    }
    else
    {
        CHECK_AO_ABSLUTEVOLUME(pstGain->s32Gain);
    }
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpVolume(pCard, enOutPort, pstGain);
}

static HI_S32 AO_SND_GetVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S* pstGain)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pstGain);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    // TODO: Check volumn Attr

    return SND_GetOpVolume(pCard, enOutPort, pstGain);
}

static HI_S32 AO_SND_SetSpdifCategoryCode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
        HI_UNF_SND_SPDIF_CATEGORYCODE_E enCategoryCode)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_CATEGORYCODE(enCategoryCode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);
    return SND_SetOpSpdifCategoryCode(pCard, enOutPort, enCategoryCode);
}

static HI_S32 AO_SND_GetSpdifCategoryCode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
        HI_UNF_SND_SPDIF_CATEGORYCODE_E* penCategoryCode)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(penCategoryCode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);
    return SND_GetOpSpdifCategoryCode(pCard, enOutPort, penCategoryCode);
}

static HI_S32 AO_SND_SetSpdifSCMSMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                      HI_UNF_SND_SPDIF_SCMSMODE_E enSCMSMode)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_SPDIFSCMSMODE(enSCMSMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpSpdifSCMSMode(pCard, enOutPort, enSCMSMode);
}

static HI_S32 AO_SND_GetSpdifSCMSMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                      HI_UNF_SND_SPDIF_SCMSMODE_E* penSCMSMode)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(penSCMSMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpSpdifSCMSMode(pCard, enOutPort, penSCMSMode);
}

static HI_S32 AO_SND_SetSampleRate(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                   HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_SAMPLERATE(enSampleRate);
    CHECK_AO_NULL_PTR(pCard);

    return SND_SetOpSampleRate(pCard, enOutPort, enSampleRate);
}

static HI_S32 AO_SND_GetSampleRate(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                   HI_UNF_SAMPLE_RATE_E* penSampleRate)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(penSampleRate);
    CHECK_AO_NULL_PTR(pCard);

    return SND_GetOpSampleRate(pCard, enOutPort, penSampleRate);
}

#ifdef __DPT__
static HI_S32 AO_SND_GetAlsaPrescale(HI_UNF_SND_E enSound, HI_UNF_SND_PRECIGAIN_ATTR_S* pstPreciGain)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);

    memcpy(pstPreciGain, &(pCard->stTrackConfig.stTrackPrescale[HI_UNF_SND_SOURCE_ALSA]), sizeof(HI_UNF_SND_PRECIGAIN_ATTR_S));

    CHECK_AO_ABSLUTEPRECIVOLUME(pstPreciGain->s32IntegerGain, pstPreciGain->s32DecimalGain);

    return HI_SUCCESS;
}

static HI_S32 AO_SND_SetAlsaPrescale(HI_UNF_SND_E enSound, HI_UNF_SND_PRECIGAIN_ATTR_S* pstPreciGain)
{
    HI_U32 i;
    HI_U32 u32TrackID;
    HI_S32 s32Ret;
    SND_TRACK_STATE_S* pTrack;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);

    if (HI_NULL == pCard || HI_NULL == pstPreciGain)
    {
        HI_ERR_AO("Invaild parameter!\n");
        return HI_FAILURE;
    }

    CHECK_AO_ABSLUTEPRECIVOLUME(pstPreciGain->s32IntegerGain, pstPreciGain->s32DecimalGain);

    memcpy(&(pCard->stTrackConfig.stTrackPrescale[HI_UNF_SND_SOURCE_ALSA]), pstPreciGain, sizeof(HI_UNF_SND_PRECIGAIN_ATTR_S));

    for (i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
    {
        if (!(pCard->uSndTrackInitFlag & ((HI_U32)1L << i)))
        {
            continue;
        }

        pTrack = (SND_TRACK_STATE_S*)pCard->pSndTrack[i];

        if (HI_TRUE == pTrack->bAlsaTrack)
        {
            break;
        }
    }

    /* if alsa track is not exist, only save prescale, but not set into alsa reg */
    if (i >= AO_MAX_TOTAL_TRACK_NUM)
    {
        return HI_SUCCESS;
    }

    u32TrackID = i;

    s32Ret = TRACK_SetPrescale(pCard, u32TrackID, pstPreciGain);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("set alsa prescale failure!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
#endif

static HI_S32 AO_SND_SetTrackMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_TRACK_MODE_E enMode)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_TRACKMODE(enMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpTrackMode(pCard, enOutPort, enMode);
}

static HI_S32 AO_SND_GetTrackMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_TRACK_MODE_E* penMode)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(penMode);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpTrackMode(pCard, enOutPort, penMode);
}

static HI_S32 AO_SND_SetDelayCompensation(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32 u32DelayMs)
{
    HI_S32 s32Ret;
    SND_CARD_STATE_S* pCard;

    CHECK_AO_SNDCARD(enSound);
    pCard = SND_CARD_GetCard(enSound);
    CHECK_AO_NULL_PTR(pCard);

    if (u32DelayMs > AO_MAX_DELAYMS)
    {
        HI_ERR_AO("Invalid u32DelayMs(%d), Max(%d)\n", u32DelayMs, AO_MAX_DELAYMS);
        return HI_ERR_AO_INVALID_PARA;
    }

    s32Ret = SND_SetDelayCompensation(pCard, enOutPort, u32DelayMs);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("SND_SetDelayCompensation (enOutPort = 0x%x) failed(0x%x)\n", enOutPort, s32Ret);
    }

    return s32Ret;
}

static HI_S32 AO_SND_GetDelayCompensation(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32* pu32DelayMs)
{
    HI_S32 s32Ret;
    SND_CARD_STATE_S* pCard;

    CHECK_AO_SNDCARD(enSound);
    pCard = SND_CARD_GetCard(enSound);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_NULL_PTR(pu32DelayMs);

    s32Ret = SND_GetDelayCompensation(pCard, enOutPort, pu32DelayMs);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("SND_GetDelayCompensation (enOutPort = 0x%x) failed(0x%x)\n", enOutPort, s32Ret);
    }

    return s32Ret;
}

static HI_S32 AO_SND_SetAllTrackMute(HI_UNF_SND_E enSound, HI_BOOL bMute)
{
    HI_S32 s32Ret;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    CHECK_AO_NULL_PTR(pCard);

    s32Ret = TRACK_SetAllMute(pCard, bMute);
    if (HI_SUCCESS != s32Ret)
    {
        // TODO:
    }

    pCard->bAllTrackMute = bMute;
    HI_INFO_AO("bAllTrackMute status %d.\n", pCard->bAllTrackMute);

    return s32Ret;
}

static HI_S32 AO_SND_GetAllTrackMute(HI_UNF_SND_E enSound, HI_BOOL* pbMute)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_NULL_PTR(pbMute);
    CHECK_AO_NULL_PTR(pCard);

    *pbMute = pCard->bAllTrackMute;
    return HI_SUCCESS;
}

static HI_S32  AO_SND_SetSmartVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bSmartVolume)
{
    // TODO:
    //verify
    return HI_SUCCESS;
}

static HI_S32 AO_SND_GetSmartVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbSmartVolume)
{
    // TODO:
    //verify
    return HI_SUCCESS;
}

static HI_S32 AO_Snd_SetLowLatency(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E eOutPort, HI_U32 u32LatencyMs)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    CHECK_AO_NULL_PTR(pCard);

    return  SND_SetLowLatency(pCard, eOutPort, u32LatencyMs);
}

static HI_S32 AO_Snd_GetLowLatency(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E eOutPort, HI_U32* pu32LatencyMs)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    CHECK_AO_NULL_PTR(pCard);

    return  SND_GetLowLatency(pCard, eOutPort, pu32LatencyMs);
}

static HI_S32 AO_Snd_SetExtDelayMs(HI_UNF_SND_E enSound, HI_U32 u32DelayMs)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    if (HI_NULL == pCard)
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }

    pCard->u32ExtDelayMs = u32DelayMs;

    return HI_SUCCESS;
}

static HI_S32 AO_Snd_AllocDMA(struct file* pstFile, HI_UNF_SND_E enSound)
{
    /* DMA only support single user */
    if (0 != atomic_read(&s_stAoDrv.astSndDMAEntity[enSound].atmUseCnt))
    {
        return HI_ERR_AO_USED;
    }

    if (HI_TRUE == s_stAoDrv.bLowLatencyCreated)
    {
        HI_ERR_AO("cannot enable SND DMA mode when LowLatency track created!\n");
        return HI_ERR_AO_USED;
    }

    s_stAoDrv.astSndDMAEntity[enSound].pstFile = pstFile;
    atomic_inc(&s_stAoDrv.astSndDMAEntity[enSound].atmUseCnt);
    return HI_SUCCESS;
}

static HI_S32 AO_Snd_FreeDMA(struct file* pstFile, HI_UNF_SND_E enSound)
{
    if (pstFile != s_stAoDrv.astSndDMAEntity[enSound].pstFile)
    {
        HI_ERR_AO("Invalid file handle!\n");
        return HI_ERR_AO_INVALID_PARA;
    }
    s_stAoDrv.astSndDMAEntity[enSound].pstFile = HI_NULL;
    atomic_set(&s_stAoDrv.astSndDMAEntity[enSound].atmUseCnt, 0);
    return HI_SUCCESS;
}

static HI_S32 AO_Snd_DMADestory(struct file* pstFile, HI_UNF_SND_E enSound)
{
    HI_S32 s32Ret = HI_SUCCESS;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    if (HI_TRUE == pCard->bDMAMode)
    {
        s32Ret = SND_SetPortSampleRate(pCard, HI_UNF_SND_OUTPUTPORT_ALL, (HI_U32)(pCard->enUserSampleRate));
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("SND_SetPortSampleRate(%d) HI_UNF_SND_OUTPUTPORT_ALL failed!\n", (HI_U32)(pCard->enUserSampleRate));
            return s32Ret;
        }
        s32Ret = AO_Snd_FreeDMA(pstFile, enSound);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("AO_Snd_FreeDMA failed!\n");
            return s32Ret;
        }
        pCard->bDMAMode = HI_FALSE;
    }
    return s32Ret;
}

static HI_S32 AO_Snd_DMACreate(struct file* pstFile, HI_UNF_SND_E enSound, SND_PORT_KERNEL_ATTR_S* pstPortKAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    CHECK_AO_NULL_PTR(pCard);

    s32Ret = AO_Snd_AllocDMA(pstFile, enSound);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("SND DMA mode occupied!\n");
        return s32Ret;
    }
    pCard->bDMAMode = HI_TRUE;

    s32Ret = Track_ForceAllToPcm(pCard); // Warning: might sleep
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("TRACK_RouteAll failed!\n");
        (HI_VOID)AO_Snd_FreeDMA(pstFile, enSound);
        pCard->bDMAMode = HI_FALSE;
        return s32Ret;
    }

    s32Ret = SND_GetPortInfo(pCard, HI_UNF_SND_OUTPUTPORT_ALL, pstPortKAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("SND_GetPortInfo failed!\n");
        (HI_VOID)AO_Snd_FreeDMA(pstFile, enSound);
        pCard->bDMAMode = HI_FALSE;
    }

    return s32Ret;
}

static HI_S32 AO_Snd_DMAModeSetSampleRate(HI_UNF_SND_E enSound, HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    HI_S32 s32Ret = HI_SUCCESS;
    SND_CARD_STATE_S* pCard = HI_NULL;

    CHECK_AO_SNDCARD(enSound);
    pCard = SND_CARD_GetCard(enSound);
    CHECK_AO_NULL_PTR(pCard);

    if (HI_FALSE == pCard->bDMAMode)
    {
        return HI_ERR_AO_NOTSUPPORT;
    }

    s32Ret = SND_SetPortSampleRate(pCard, HI_UNF_SND_OUTPUTPORT_ALL, (HI_U32)(enSampleRate));
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("SND_SetPortSampleRate(%d) HI_UNF_SND_OUTPUTPORT_ALL failed!\n", (HI_U32)(enSampleRate));
    }

    return s32Ret;
}

#ifdef __DPT__
static HI_S32 AO_SND_SetPreciVol(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_PRECIGAIN_ATTR_S stPreciGain)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_ABSLUTEPRECIVOLUME(stPreciGain.s32IntegerGain, stPreciGain.s32DecimalGain);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpPreciVol(pCard, enOutPort, stPreciGain);
}

static HI_S32 AO_SND_GetPreciVol(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_PRECIGAIN_ATTR_S* pstPreciGain)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pstPreciGain);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpPreciVol(pCard, enOutPort, pstPreciGain);
}

static HI_S32 AO_SND_SetBalance(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_S32 s32Balance)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_BALANCE(s32Balance);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpBalance(pCard, enOutPort, s32Balance);
}

static HI_S32 AO_SND_GetBalance(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_S32* ps32Balance)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(ps32Balance);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpBalance(pCard, enOutPort, ps32Balance);
}

static HI_S32 AO_SND_SetDrcEnable(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort,  HI_BOOL bEnable)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpDrcEnable(pCard, enOutPort, bEnable);
}

static HI_S32 AO_SND_GetDrcEnable(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbEnable)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpDrcEnable(pCard, enOutPort, pbEnable);
}

static HI_S32 AO_SND_SetDrcAttr(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_DRC_ATTR_S* pstDrcAttr)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pstDrcAttr);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_DRCATTR(pstDrcAttr);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpDrcAttr(pCard, enOutPort, pstDrcAttr);
}

static HI_S32 AO_SND_GetDrcAttr(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_DRC_ATTR_S* pstDrcAttr)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pstDrcAttr);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpDrcAttr(pCard, enOutPort, pstDrcAttr);
}

static HI_S32 AOCheckPeqBandNum(HI_U32 u32BandNum)
{
    if ((u32BandNum < PEQ_BANDNUM_MIN) || (u32BandNum > PEQ_BANDNUM_MAX))
    {
        HI_ERR_AO("Invalid Peq BandNum! bandnum(%d)!\n", u32BandNum);
        return HI_ERR_AO_INVALID_PARA;
    }

    return HI_SUCCESS;
}

static HI_S32 AOCheckPeqBandAttr(HI_UNF_SND_PEQ_BAND_ATTR_S* pstEqParam)
{
    HI_U32 u32Qmin;
    HI_U32 u32Qmax;
    HI_U32 u32Freqmin;
    HI_U32 u32Freqmax;

    switch (pstEqParam->enFltType)
    {
        case HI_UNF_PEQ_FILTER_TYPE_HP:
            u32Qmin = PEQ_Q_HP_MIN;
            u32Qmax = PEQ_Q_HP_MAX;
            u32Freqmin = PEQ_FREQ_HP_MIN;
            u32Freqmax = PEQ_FREQ_HP_MAX;
            break;

        case HI_UNF_PEQ_FILTER_TYPE_LP:
            u32Qmin = PEQ_Q_LP_MIN;
            u32Qmax = PEQ_Q_LP_MAX;
            u32Freqmin = PEQ_FREQ_LP_MIN;
            u32Freqmax = PEQ_FREQ_LP_MAX;
            break;

        case HI_UNF_PEQ_FILTER_TYPE_LS:
            u32Qmin = PEQ_Q_LS_MIN;
            u32Qmax = PEQ_Q_LS_MAX;
            u32Freqmin = PEQ_FREQ_LS_MIN;
            u32Freqmax = PEQ_FREQ_LS_MAX;
            break;

        case HI_UNF_PEQ_FILTER_TYPE_HS:
            u32Qmin = PEQ_Q_HS_MIN;
            u32Qmax = PEQ_Q_HS_MAX;
            u32Freqmin = PEQ_FREQ_HS_MIN;
            u32Freqmax = PEQ_FREQ_HS_MAX;
            break;

        case HI_UNF_PEQ_FILTER_TYPE_PK:
            u32Qmin = PEQ_Q_PK_MIN;
            u32Qmax = PEQ_Q_PK_MAX;
            u32Freqmin = PEQ_FREQ_PK_MIN;
            u32Freqmax = PEQ_FREQ_PK_MAX;
            break;

        default:
            HI_ERR_AO("Invalid Filter Type %d\n", pstEqParam->enFltType);
            return HI_ERR_AO_INVALID_PARA;

    }

    AO_CHECK_PEQQ(pstEqParam->u32Q, u32Qmin, u32Qmax);
    AO_CHECK_PEQFREQ(pstEqParam->u32freq, u32Freqmin, u32Freqmax);
    AO_CHECK_PEQGAIN(pstEqParam->s32Gain, PEQ_GAIN_MIN, PEQ_GAIN_MAX);

    return HI_SUCCESS;
}

static HI_S32 AOCheckPeqAttr(HI_UNF_SND_PEQ_ATTR_S* pstPeqAttr)
{
    HI_S32 s32Ret;
    HI_U32 u32BandId;

    s32Ret = AOCheckPeqBandNum(pstPeqAttr->u32BandNum);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    for (u32BandId = 0; u32BandId < pstPeqAttr->u32BandNum; u32BandId++)
    {
        s32Ret = AOCheckPeqBandAttr(&pstPeqAttr->stEqParam[u32BandId]);
        if (HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 AO_SND_SetPeqAttr(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_PEQ_ATTR_S* pstPeqAttr)
{
    HI_S32 s32Ret;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pstPeqAttr);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    s32Ret = AOCheckPeqAttr(pstPeqAttr);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    return SND_SetOpPeqAttr(pCard, enOutPort, pstPeqAttr);
}

static HI_S32 AO_SND_GetPeqAttr(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_PEQ_ATTR_S* pstPeqAttr)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pstPeqAttr);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpPeqAttr(pCard, enOutPort, pstPeqAttr);
}

static HI_S32 AO_SND_SetPeqEnable(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bEnable)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpPeqEnable(pCard, enOutPort, bEnable);
}

static HI_S32 AO_SND_GetPeqEnable(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbEnable)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpPeqEnable(pCard, enOutPort, pbEnable);
}

static HI_S32 AO_Check_AvcAttr(HI_UNF_SND_AVC_ATTR_S* pstAvcAttr)
{
    if ((pstAvcAttr->u32AttackTime < 20) || (pstAvcAttr->u32ReleaseTime < 20)
        || (pstAvcAttr->u32AttackTime > 2000) || (pstAvcAttr->u32ReleaseTime > 2000)
        || (pstAvcAttr->s32ThresholdLevel < -39) || (pstAvcAttr->s32ThresholdLevel > -17)
        || (pstAvcAttr->s32Gain < 0) || (pstAvcAttr->s32Gain > 8)
        || (pstAvcAttr->s32LimiterLevel < -16) || (pstAvcAttr->s32LimiterLevel > -6))
    {
        return HI_ERR_AO_INVALID_PARA;
    }

    return HI_SUCCESS;
}

static HI_S32 AO_SND_SetAvcAttr(HI_UNF_SND_E enSound, HI_UNF_SND_AVC_ATTR_S* pstAvcAttr)
{
    HI_S32 Ret;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    SND_ENGINE_STATE_S* pstEnginestate = (SND_ENGINE_STATE_S*)pCard->pSndEngine[SND_ENGINE_TYPE_PCM];

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pstAvcAttr);
    CHECK_AO_NULL_PTR(pCard);

    Ret = AO_Check_AvcAttr(pstAvcAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("invalid avc attr!\n");
        return Ret;
    }

    if (!pstEnginestate)
    {
        memcpy(&pCard->stAvcAttr, pstAvcAttr, sizeof(HI_UNF_SND_AVC_ATTR_S));
        HI_INFO_AO("pcm engine do not exist!\n");
        return HI_SUCCESS;
    }

    Ret = HAL_AOE_ENGINE_SetAvcAttr(pstEnginestate->enEngine, pstAvcAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HAL_AOE_ENGINE_SetAvcAttr failed!\n");
        return Ret;
    }
    memcpy(&pCard->stAvcAttr, pstAvcAttr, sizeof(HI_UNF_SND_AVC_ATTR_S));

    return Ret;
}

static HI_S32 AO_SND_GetAvcAttr(HI_UNF_SND_E enSound, HI_UNF_SND_AVC_ATTR_S* pstAvcAttr)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pstAvcAttr);
    CHECK_AO_NULL_PTR(pCard);
    memcpy(pstAvcAttr, &pCard->stAvcAttr, sizeof(HI_UNF_SND_AVC_ATTR_S));

    return HI_SUCCESS;
}

static HI_S32 AO_SND_SetAvcEnable(HI_UNF_SND_E enSound, HI_BOOL bEnable)
{
    HI_S32 Ret;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    SND_ENGINE_STATE_S* pstEnginestate = (SND_ENGINE_STATE_S*)pCard->pSndEngine[SND_ENGINE_TYPE_PCM];

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pCard);

    if (!pstEnginestate)
    {
        pCard->bAvcEnable = bEnable;
        HI_INFO_AO("pcm engine do not exist!\n");
        return HI_SUCCESS;
    }

    Ret = HAL_AOE_ENGINE_SetAvcEnable(pstEnginestate->enEngine, bEnable);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HAL_AOE_ENGINE_SetAvcEnable failed!\n");
        return Ret;
    }

    pCard->bAvcEnable = bEnable;
    return Ret;
}

static HI_S32 AO_SND_GetAvcEnable(HI_UNF_SND_E enSound, HI_BOOL* pbEnable)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pCard);
    *pbEnable = pCard->bAvcEnable;

    return HI_SUCCESS;
}

static HI_S32 AO_Check_GeqAttr(HI_UNF_SND_GEQ_ATTR_S* pstGeqAttr)
{
    HI_U32 i;

    if ((pstGeqAttr->u32BandNum < 5) || (pstGeqAttr->u32BandNum > 10))
    {
        HI_ERR_AO("Invalid Geq BandNum! bandnum(%d)!\n", pstGeqAttr->u32BandNum);
        return HI_ERR_AO_INVALID_PARA;
    }

    for (i = 0; i < pstGeqAttr->u32BandNum; i++)
    {
        if ((pstGeqAttr->stEqParam[i].s32Gain < -15000) || (pstGeqAttr->stEqParam[i].s32Gain > 15000) || (pstGeqAttr->stEqParam[i].s32Gain % 125))
        {
            HI_ERR_AO("Invalid Geq Gain value!\n");
            return HI_ERR_AO_INVALID_PARA;
        }

        if ((pstGeqAttr->stEqParam[i].u32freq < 20) || (pstGeqAttr->stEqParam[i].u32freq > 20000))
        {
            HI_ERR_AO("Invalid Geq Gain value!\n");
            return HI_ERR_AO_INVALID_PARA;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 AO_SND_SetGeqAttr(HI_UNF_SND_E enSound, HI_UNF_SND_GEQ_ATTR_S* pstGeqAttr)
{
    HI_S32 Ret;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    SND_ENGINE_STATE_S* pstEnginestate = (SND_ENGINE_STATE_S*)pCard->pSndEngine[SND_ENGINE_TYPE_PCM];

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pstGeqAttr);
    CHECK_AO_NULL_PTR(pCard);

    Ret = AO_Check_GeqAttr(pstGeqAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("invalid geq attr!\n");
        return Ret;
    }

    if (!pstEnginestate)
    {
        memcpy(&pCard->stGeqAttr, pstGeqAttr, sizeof(HI_UNF_SND_GEQ_ATTR_S));
        HI_INFO_AO("pcm engine do not exist!\n");
        return HI_SUCCESS;
    }

    Ret = HAL_AOE_ENGINE_SetGeqAttr(pstEnginestate->enEngine, pstGeqAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HAL_AOE_ENGINE_SetGeqAttr failed!\n");
        return Ret;
    }

    memcpy(&pCard->stGeqAttr, pstGeqAttr, sizeof(HI_UNF_SND_GEQ_ATTR_S));
    return Ret;
}

static HI_S32 AO_SND_GetGeqAttr(HI_UNF_SND_E enSound, HI_UNF_SND_GEQ_ATTR_S* pstGeqAttr)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pstGeqAttr);
    CHECK_AO_NULL_PTR(pCard);

    memcpy(pstGeqAttr, &pCard->stGeqAttr, sizeof(HI_UNF_SND_GEQ_ATTR_S));
    return HI_SUCCESS;
}

static HI_S32 AO_SND_SetGeqEnable(HI_UNF_SND_E enSound, HI_BOOL bEnable)
{
    HI_S32 Ret;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    SND_ENGINE_STATE_S* pstEnginestate = (SND_ENGINE_STATE_S*)pCard->pSndEngine[SND_ENGINE_TYPE_PCM];

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pCard);

    if (!pstEnginestate)
    {
        pCard->bGeqEnable = bEnable;
        HI_INFO_AO("pcm engine do not exist!\n");
        return HI_SUCCESS;
    }

    Ret = HAL_AOE_ENGINE_SetGeqEnable(pstEnginestate->enEngine, bEnable);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HAL_AOE_ENGINE_SetGeqEnable failed!\n");
        return Ret;
    }

    pCard->bGeqEnable = bEnable;
    return Ret;
}

static HI_S32 AO_SND_GetGeqEnable(HI_UNF_SND_E enSound, HI_BOOL* pbEnable)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pCard);

    *pbEnable = pCard->bGeqEnable;
    return HI_SUCCESS;
}

static HI_S32 AO_SND_SetGeqGain(HI_UNF_SND_E enSound, HI_U32 u32Band, HI_S32 s32Gain)
{
    HI_S32 Ret;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    SND_ENGINE_STATE_S* pstEnginestate;

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pCard);

    pstEnginestate = (SND_ENGINE_STATE_S*)pCard->pSndEngine[SND_ENGINE_TYPE_PCM];

    if ((s32Gain < -15000) || (s32Gain > 15000) || (s32Gain % 125))
    {
        HI_ERR_AO("invalid geq gain!\n");
        return HI_ERR_AO_INVALID_PARA;
    }

    if (!pstEnginestate)
    {
        pCard->stGeqAttr.stEqParam[u32Band].s32Gain = s32Gain;
        HI_INFO_AO("pcm engine do not exist!\n");
        return HI_SUCCESS;
    }

    Ret = HAL_AOE_ENGINE_SetGeqGain(pstEnginestate->enEngine, u32Band, s32Gain);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HAL_AOE_ENGINE_SetGeqGain failed!\n");
        return Ret;
    }

    pCard->stGeqAttr.stEqParam[u32Band].s32Gain = s32Gain;
    return Ret;
}

static HI_S32 AO_SND_GetGeqGain(HI_UNF_SND_E enSound, HI_U32 u32Band, HI_S32* ps32Gain)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pCard);

    *ps32Gain = pCard->stGeqAttr.stEqParam[u32Band].s32Gain;
    return HI_SUCCESS;
}


static HI_S32 AO_WriteProc_CastSimulateOp(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    HI_CHAR* pcOnCmd  = "on";
    HI_CHAR* pcOffCmd = "off";

    pCard->enCastSimulatePort = AUTIL_PortName2Port(pcBuf);

    AO_STRING_SKIP_BLANK(pcBuf);

    if (strstr(pcBuf, pcOnCmd))
    {
        pCard->bCastSimulateOp = HI_TRUE;
    }
    else if (strstr(pcBuf, pcOffCmd))
    {
        pCard->bCastSimulateOp = HI_FALSE;
    }
    else
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 AO_SND_AttachAef(HI_UNF_SND_E enSound, HI_U32 u32AefId, AO_AEF_ATTR_S* pstAefAttr, HI_U32* pu32AefProcAddr)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pu32AefProcAddr);
    CHECK_AO_NULL_PTR(pCard);

    return AEF_AttachSnd(pCard, u32AefId, pstAefAttr, pu32AefProcAddr);
}

static HI_S32 AO_SND_DetachAef(HI_UNF_SND_E enSound, HI_U32 u32AefId, AO_AEF_ATTR_S* pstAefAttr)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pCard);

    return AEF_DetachSnd(pCard, u32AefId, pstAefAttr);
}

static HI_S32 AO_SND_SetAefBypass(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bBypass)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpAefBypass(pCard, enOutPort, bBypass);
}

static HI_S32 AO_SND_GetAefBypass(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbBypass)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pbBypass);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpAefBypass(pCard, enOutPort, pbBypass);
}

static HI_S32 AO_Snd_GetAefBufAttr(HI_UNF_SND_E enSound, AO_AEF_BUF_ATTR_S* pstAefBuf)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_NULL_PTR(pstAefBuf);
    CHECK_AO_NULL_PTR(pCard);

    return AEF_GetBufAttr(pCard, pstAefBuf);
}

static HI_S32 AO_Snd_GetDebugAttr(HI_UNF_SND_E enSound, AO_DEBUG_ATTR_S* pstDebugAttr)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_NULL_PTR(pstDebugAttr);
    CHECK_AO_NULL_PTR(pCard);

    switch (pstDebugAttr->enDebugType)
    {
        case AO_SND_DEBUG_TYPE_AEF:
            AEF_GetDebugAddr(pCard, &(pstDebugAttr->unDebugAttr.stAefDebugAttr));
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 AO_SND_SetADOutputEnable(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bEnable)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_SetOpADOutputEnable(pCard, enOutPort, bEnable);
}

static HI_S32 AO_SND_GetADOutputEnable(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbEnable)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_OUTPORT(enOutPort);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    return SND_GetOpADOutputEnable(pCard, enOutPort, pbEnable);
}

static HI_S32 AO_SND_TrackConfigInit(HI_UNF_SND_E enSound, HI_UNF_SND_TRACK_CONFIG_S* pstTrackConfig)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    HI_U32 u32SrcNum;

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pstTrackConfig);
    for (u32SrcNum = HI_UNF_SND_SOURCE_ALSA; u32SrcNum < HI_UNF_SND_SOURCE_BUTT; u32SrcNum++)
    {
        CHECK_AO_ABSLUTEPRECIVOLUME(pstTrackConfig->stTrackPrescale[u32SrcNum].s32IntegerGain,
                                    pstTrackConfig->stTrackPrescale[u32SrcNum].s32DecimalGain);
    }

    memcpy(&pCard->stTrackConfig, pstTrackConfig, sizeof(HI_UNF_SND_TRACK_CONFIG_S));
    return HI_SUCCESS;
}
#endif

/******************************Snd Track FUNC*************************************/
static SND_CARD_STATE_S* TRACK_CARD_GetCard(HI_U32 Id)
{
    HI_UNF_SND_E sndx;
    SND_CARD_STATE_S* pCard = HI_NULL;

    if (Id >= AO_MAX_TOTAL_TRACK_NUM)
    {
        return HI_NULL;
    }

    for (sndx = HI_UNF_SND_0; sndx < HI_UNF_SND_BUTT; sndx++)
    {
        pCard = SND_CARD_GetCard(sndx);
        if (pCard)
        {
            if (pCard->uSndTrackInitFlag & (1L << Id))
            {
                return pCard;
            }
        }
    }

    return HI_NULL;
}

static HI_S32 AO_Snd_GetXRunCount(HI_UNF_SND_E enSound, HI_U32* pu32Count)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_NULL_PTR(pCard);
    SND_GetXRunCount(pCard, pu32Count);

    return HI_SUCCESS;
}

#ifdef __DPT__
static HI_S32 AO_SND_TrackDuplicate(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32 u32TrackID, HI_BOOL bEnable)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_PORTEXIST(pCard->stUserOpenParam.u32PortNum);

    u32TrackID &= AO_TRACK_CHNID_MASK;
    if (pCard != TRACK_CARD_GetCard(u32TrackID))
    {
        HI_ERR_AO("this track is not in the sound!\n");
        return HI_FAILURE;
    }

    if ((HI_UNF_SND_OUTPUTPORT_DAC0 != enOutPort) && (HI_UNF_SND_OUTPUTPORT_EXT_DAC1 != enOutPort) &&
        (HI_UNF_SND_OUTPUTPORT_EXT_DAC2 != enOutPort) && (HI_UNF_SND_OUTPUTPORT_EXT_DAC3 != enOutPort))
    {
        HI_ERR_AO("enOutPort(%d)is invalid, only support DAC port!\n", AUTIL_Port2Name(enOutPort));
        return HI_FAILURE;
    }

    return TRACK_Duplicate2Aop(pCard, enOutPort, u32TrackID, bEnable);
}

static HI_S32 AO_SND_GetTrackInfo(HI_UNF_SND_E enSound, HI_UNF_SND_TRACK_INFO_S* pstTrackInfo)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_SNDCARD(enSound);
    CHECK_AO_NULL_PTR(pstTrackInfo);
    CHECK_AO_NULL_PTR(pCard);

    memcpy(pstTrackInfo, &pCard->stTrackInfo, sizeof(HI_UNF_SND_TRACK_INFO_S));

    return HI_SUCCESS;
}
#endif

static HI_S32 AO_Snd_SetAdacEnable(HI_UNF_SND_E enSound, HI_BOOL bEnable)
{
    HI_S32 Ret = HI_SUCCESS;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_NULL_PTR(pCard);
    Ret = SND_SetAdacEnable(pCard, bEnable);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_AO("Set Adac Enable Failed\n");
    }
    pCard->bAdacEnable = bEnable;

    return Ret;
}

static HI_S32 AO_Snd_GetAdacEnable(HI_UNF_SND_E enSound, HI_BOOL* pbEnable)
{
    HI_S32 Ret = HI_SUCCESS;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_NULL_PTR(pCard);

    *pbEnable = pCard->bAdacEnable;

    return Ret;
}

/* snd open kernel intf */
HI_S32 AO_Snd_Kopen(AO_SND_Open_Param_S_PTR arg, AO_ALSA_I2S_Param_S* pstAoI2sParam, struct file* file)
{
    HI_S32 s32Ret;
    HI_UNF_SND_E enSound = HI_UNF_SND_BUTT;
    AO_SND_Open_Param_S_PTR pstSndParam = ( AO_SND_Open_Param_S_PTR )arg;
    DRV_AO_STATE_S* pAOState = file->private_data;

    enSound = pstSndParam->enSound;
    CHECK_AO_SNDCARD( enSound );

    s32Ret = down_interruptible(&g_AoMutex);
    if (HI_SUCCESS == AO_Snd_AllocHandle(enSound, file))
    {
        if (0 == atomic_read(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
        {
            s32Ret = AO_SND_Open( enSound, &pstSndParam->stAttr, pstAoI2sParam, HI_FALSE);
            if (HI_SUCCESS != s32Ret)
            {
                AO_Snd_FreeHandle(enSound, file);
                up(&g_AoMutex);
                return HI_FAILURE;
            }
        }
    }

    atomic_inc(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
    atomic_inc(&pAOState->atmUserOpenCnt[enSound]);

    up(&g_AoMutex);

    return HI_SUCCESS;
}

/* snd close kernel intf */
HI_S32 AO_Snd_Kclose(HI_UNF_SND_E  arg, struct file* file)
{
    HI_S32 s32Ret;
    HI_UNF_SND_E enSound = HI_UNF_SND_BUTT;
    DRV_AO_STATE_S* pAOState = file->private_data;
    enSound = arg;
    CHECK_AO_SNDCARD_OPEN( enSound );

    s32Ret = down_interruptible(&g_AoMutex);
    if (atomic_dec_and_test(&pAOState->atmUserOpenCnt[enSound]))
    {
        if (atomic_dec_and_test(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
        {
            s32Ret = AO_SND_Close( enSound, HI_FALSE );
            if (HI_SUCCESS != s32Ret)
            {
                atomic_inc(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
                up(&g_AoMutex);
                return HI_FAILURE;
            }

            AO_Snd_FreeHandle(enSound, file);
        }
    }
    else
    {
        atomic_dec(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
    }

    up(&g_AoMutex);

    return HI_SUCCESS;
}

#ifdef __DPT__
static HI_VOID AO_SND_UpdateTrackInfo(HI_HANDLE hTrack, HI_BOOL bAdd)
{
    HI_U32 i, j;
    SND_CARD_STATE_S* pCard = TRACK_CARD_GetCard(hTrack & AO_TRACK_CHNID_MASK);

    if (HI_NULL == pCard)
    {
        HI_WARN_AO("Sound card is null!\n");
        return;
    }

    if (HI_TRUE == bAdd)
    {
        if (pCard->stTrackInfo.u32TrackNum >= HI_UNF_SND_TRACK_MAX)
        {
            HI_WARN_AO("Track number is up to max!\n");
            return;
        }
        pCard->stTrackInfo.hTrack[pCard->stTrackInfo.u32TrackNum++] = hTrack;
    }
    else
    {
        for (i = 0; i < pCard->stTrackInfo.u32TrackNum; i++)
        {
            if (hTrack == pCard->stTrackInfo.hTrack[i] || hTrack == (pCard->stTrackInfo.hTrack[i] & AO_TRACK_CHNID_MASK))
            {
                break;
            }
        }

        if (i == pCard->stTrackInfo.u32TrackNum)
        {
            return;
        }

        for (j = i; j < pCard->stTrackInfo.u32TrackNum - 1; j++)
        {
            pCard->stTrackInfo.hTrack[j] = pCard->stTrackInfo.hTrack[j + 1];
        }
        pCard->stTrackInfo.u32TrackNum--;
    }

    return;
}
#endif

HI_S32 AO_Track_AllocHandle(HI_UNF_SND_E enSound, HI_UNF_SND_TRACK_TYPE_E enTrackType, struct file* pstFile, HI_HANDLE* phHandle)
{
    HI_U32 i;

    CHECK_AO_NULL_PTR(phHandle);

    if (s_stAoDrv.bReady != HI_TRUE)
    {
        HI_ERR_AO("Need open first!\n");
        return HI_ERR_AO_DEV_NOT_OPEN;
    }

    if (s_stAoDrv.u32TrackNum >= AO_MAX_TOTAL_TRACK_NUM)
    {
        HI_ERR_AO("Too many track:%d!\n", s_stAoDrv.u32TrackNum);
        return HI_ERR_AO_NOTSUPPORT;
    }

    for (i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
    {
        if (0 == atomic_read(&s_stAoDrv.astTrackEntity[i].atmUseCnt))
        {
            break;
        }
    }

    if (i >= AO_MAX_TOTAL_TRACK_NUM)
    {
        HI_ERR_AO("Too many track!\n");
        return HI_ERR_AO_NOTSUPPORT;
    }

    if (enTrackType == HI_UNF_SND_TRACK_TYPE_LOWLATENCY)
    {
        if (s_stAoDrv.bLowLatencyCreated == HI_TRUE)
        {
            HI_ERR_AO("Too many LowLatency track!\n");
            return HI_ERR_AO_NOTSUPPORT;
        }

        if (0 != atomic_read(&s_stAoDrv.astSndDMAEntity[enSound].atmUseCnt))
        {
            HI_ERR_AO("cannot create LowLatency track in SND DMA mode!\n");
            return HI_ERR_AO_NOTSUPPORT;
        }
    }

    s_stAoDrv.astTrackEntity[i].pstFile = pstFile;
    s_stAoDrv.u32TrackNum++;
    atomic_inc(&s_stAoDrv.astTrackEntity[i].atmUseCnt);

    /*
      define of Track Handle :
      bit31                                                           bit0
        |<----   16bit --------->|<---   8bit    --->|<---  8bit   --->|
        |--------------------------------------------------------------|
        |      HI_MOD_ID_E       |  sub_mod defined  |     chnID       |
        |--------------------------------------------------------------|
      */

    if (enTrackType == HI_UNF_SND_TRACK_TYPE_LOWLATENCY)
    {
        *phHandle = (HI_ID_AO << 16) | (HI_ID_LOWLATENCY_TRACK << 8) | i;
        s_stAoDrv.bLowLatencyCreated = HI_TRUE;
    }
    else
    {
        *phHandle = (HI_ID_AO << 16) | (HI_ID_MASTER_SLAVE_TRACK << 8) | i;
    }

    return HI_SUCCESS;
}

HI_VOID AO_Track_FreeHandleById(HI_U32 u32TrackID)
{
    s_stAoDrv.bLowLatencyCreated = HI_FALSE;  //low latency track

    if (0 == atomic_read(&s_stAoDrv.astTrackEntity[u32TrackID].atmUseCnt))
    {
        return;
    }

    s_stAoDrv.astTrackEntity[u32TrackID].pstFile = HI_NULL;
    s_stAoDrv.u32TrackNum--;
    atomic_set(&s_stAoDrv.astTrackEntity[u32TrackID].atmUseCnt, 0);
}


HI_VOID AO_Track_FreeHandle(HI_HANDLE hHandle)
{
    if ((hHandle & 0xff00) == (HI_ID_LOWLATENCY_TRACK << 8)) //low latency track
    {
        s_stAoDrv.bLowLatencyCreated = HI_FALSE;
    }

    hHandle &= AO_TRACK_CHNID_MASK;
    if (0 == atomic_read(&s_stAoDrv.astTrackEntity[hHandle].atmUseCnt))
    {
        return;
    }
    s_stAoDrv.astTrackEntity[hHandle].pstFile = HI_NULL;
    s_stAoDrv.u32TrackNum--;
    atomic_set(&s_stAoDrv.astTrackEntity[hHandle].atmUseCnt, 0);
}

static HI_VOID AO_TRACK_SaveSuspendAttr(HI_HANDLE hHandle, AO_Track_Create_Param_S_PTR pstTrack)
{
    hHandle &= AO_TRACK_CHNID_MASK;
    s_stAoDrv.astTrackEntity[hHandle].stSuspendAttr.enSound = pstTrack->enSound;
    s_stAoDrv.astTrackEntity[hHandle].stSuspendAttr.bAlsaTrack = pstTrack->bAlsaTrack;
    memcpy(&s_stAoDrv.astTrackEntity[hHandle].stSuspendAttr.stBufAttr, &pstTrack->stBuf, sizeof(AO_BUF_ATTR_S));
}

HI_S32 AO_Track_GetDefAttr(HI_UNF_AUDIOTRACK_ATTR_S* pstDefAttr)
{
    return TRACK_GetDefAttr(pstDefAttr);
}

static HI_S32 AO_Track_GetAttr(HI_U32 u32TrackID, HI_UNF_AUDIOTRACK_ATTR_S* pstTrackAttr)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetAttr(pCard, u32TrackID, pstTrackAttr);
    }
    else
    {
        return HI_FAILURE;
    }
}

static HI_S32 AO_Track_SetAttr(HI_U32 u32TrackID, HI_UNF_AUDIOTRACK_ATTR_S* pstTrackAttr)
{
    HI_S32 s32Ret;
    SND_CARD_STATE_S* pCard;

    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {

        s32Ret = TRACK_SetAttr(pCard, u32TrackID, pstTrackAttr);
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("TRACK_SetAttr fail\n");
            return HI_FAILURE;
        }

        return s32Ret;
    }

    return HI_FAILURE;
}

HI_S32 AO_Track_Create(HI_UNF_SND_E enSound, HI_UNF_AUDIOTRACK_ATTR_S* pstAttr,
                       HI_BOOL bAlsaTrack, AO_BUF_ATTR_S* pstBuf, HI_HANDLE hTrack)
{
    HI_S32 s32Ret;
    SND_CARD_STATE_S* pCard;
    hTrack &= AO_TRACK_CHNID_MASK;
    pCard = SND_CARD_GetCard(enSound);

    if (pCard)
    {
        s32Ret = TRACK_CreateNew(pCard, pstAttr, bAlsaTrack, pstBuf, hTrack);
        if (HI_SUCCESS == s32Ret)
        {
            if (HI_FALSE == bAlsaTrack)  //Not alsa track
            {
#ifdef __DPT__
                SND_TRACK_STATE_S* pTrack;
                pTrack = (SND_TRACK_STATE_S*)pCard->pSndTrack[hTrack];
                pTrack->enSource = HI_UNF_SND_SOURCE_BUTT;

                //default track source prescale
                s32Ret = TRACK_SetPrescale(pCard, hTrack, &(pCard->stTrackConfig.stTrackPrescale[HI_UNF_SND_SOURCE_DTV]));
                if (HI_SUCCESS != s32Ret)
                {
                    HI_FATAL_AO("TRACK_SetPrescale fail\n");
                    return HI_FAILURE;
                }
#endif
                HI_INFO_AO("set track %d mute \n", hTrack);
                return TRACK_SetMute(pCard, hTrack, HI_FALSE);
            }
#ifdef __DPT__
            else
            {
                s32Ret = AO_SND_SetAlsaPrescale(enSound, &(pCard->stTrackConfig.stTrackPrescale[HI_UNF_SND_SOURCE_ALSA]));
                if (HI_SUCCESS != s32Ret)
                {
                    HI_FATAL_AO("AO_SND_SetAlsaPrescale fail\n");
                    return HI_FAILURE;
                }
            }
#endif
        }

        return s32Ret;
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

HI_S32 AO_Track_Destory(HI_U32 u32TrackID)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
#ifdef __DPT__
    AO_SND_UpdateTrackInfo(u32TrackID, HI_FALSE);
#endif
    pCard = TRACK_CARD_GetCard(u32TrackID);
    if (pCard)
    {
        return TRACK_Destroy(pCard, u32TrackID);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

HI_S32 AO_Track_Start(HI_U32 u32TrackID)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_Start(pCard, u32TrackID);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

HI_S32 AO_Track_Stop(HI_U32 u32TrackID)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_Stop(pCard, u32TrackID);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

HI_S32 AO_Track_Pause(HI_U32 u32TrackID)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_Pause(pCard, u32TrackID);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

HI_S32 AO_Track_Flush(HI_U32 u32TrackID)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_Flush(pCard, u32TrackID);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

HI_S32 AO_TrackSetAipFiFoBypass(HI_U32 u32TrackID, HI_BOOL bEnable)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);
    if (pCard)
    {
        return TRACK_SetAipFiFoBypass(pCard, u32TrackID, bEnable);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

HI_S32 AO_Track_SendData(HI_U32 u32TrackID, AO_FRAMEINFO_S* pstAOFrame)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        if (HI_TRUE == pCard->bDMAMode)
        {
            return HI_SUCCESS;
        }
        else
        {
            return TRACK_SendData(pCard, u32TrackID, pstAOFrame);
        }
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_Mmap(HI_U32 u32TrackID, AO_Track_Mmap_Param_S* pstParam)
{
    HI_S32 s32Ret = HI_SUCCESS;
    SND_CARD_STATE_S* pCard;

    u32TrackID &= AO_TRACK_CHNID_MASK;

    pCard = TRACK_CARD_GetCard(u32TrackID);
    if (HI_NULL == pCard)
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }

    s32Ret = TRACK_Mmap(pCard, u32TrackID, pstParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("call TRACK_MmapTrackAttr failed!\n");
    }

    return s32Ret;
}

static HI_S32 AO_Track_SetWeight(HI_U32 u32TrackID, HI_UNF_SND_GAIN_ATTR_S* pstTrackGain)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SetWeight(pCard, u32TrackID, pstTrackGain);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_GetWeight(HI_U32 u32TrackID, HI_UNF_SND_GAIN_ATTR_S* pstTrackGain)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetWeight(pCard, u32TrackID, pstTrackGain);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_SetAbsGain(HI_U32 u32TrackID, HI_UNF_SND_ABSGAIN_ATTR_S* pstTrackAbsGain)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SetAbsGain(pCard, u32TrackID, pstTrackAbsGain);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_GetAbsGain(HI_U32 u32TrackID, HI_UNF_SND_ABSGAIN_ATTR_S* pstTrackAbsGain)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetAbsGain(pCard, u32TrackID, pstTrackAbsGain);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

#ifdef __DPT__
static HI_S32 AO_Track_SetPrescale(HI_U32 u32TrackID, HI_UNF_SND_PRECIGAIN_ATTR_S* pstPreciGain)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SetPrescale(pCard, u32TrackID, pstPreciGain);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_GetPrescale(HI_U32 u32TrackID, HI_UNF_SND_PRECIGAIN_ATTR_S* pstPreciGain)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetPrescale(pCard, u32TrackID, pstPreciGain);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}
#endif

static HI_S32 AO_Track_SetMute(HI_U32 u32TrackID, HI_BOOL bMute)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SetMute(pCard, u32TrackID, bMute);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_GetMute(HI_U32 u32TrackID, HI_BOOL* pbMute)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetMute(pCard, u32TrackID, pbMute);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_SetChannelMode(HI_U32 u32TrackID, HI_UNF_TRACK_MODE_E enMode)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SetChannelMode(pCard, u32TrackID, &enMode);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_GetChannelMode(HI_U32 u32TrackID, HI_UNF_TRACK_MODE_E* penMode)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetChannelMode(pCard, u32TrackID, penMode);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_SePriority(HI_U32 u32TrackID, HI_BOOL bEnable)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SetPriority(pCard, u32TrackID, bEnable);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_GePriority(HI_U32 u32TrackID, HI_BOOL* pbEnable)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetPriority(pCard, u32TrackID, pbEnable);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_SetSpeedAdjust(HI_U32 u32TrackID, AO_SND_SPEEDADJUST_TYPE_E enType, HI_S32 s32Speed)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SetSpeedAdjust(pCard, u32TrackID, enType, s32Speed);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

HI_S32 AO_Track_GetDelayMs(HI_U32 u32TrackID, HI_U32* pu32DelayMs)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetDelayMs(pCard, u32TrackID, pu32DelayMs);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_GetExtDelayMs(HI_U32 u32TrackID, HI_U32* pu32DelayMs)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);
    if (HI_NULL == pCard)
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }

    *pu32DelayMs = pCard->u32ExtDelayMs;
    return HI_SUCCESS;
}

static HI_S32 AO_Track_IsBufEmpty(HI_U32 u32TrackID, HI_BOOL* pbBufEmpty)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_IsBufEmpty(pCard, u32TrackID, pbBufEmpty);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_SetEosFlag(HI_U32 u32TrackID, HI_BOOL bEosFlag)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SetEosFlag(pCard, u32TrackID, bEosFlag);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

HI_S32 AO_Track_AttachAi(HI_U32 u32TrackID, HI_HANDLE hAi)
{
#if defined (HI_AUDIO_AI_SUPPORT)
    HI_S32 Ret;
    SND_CARD_STATE_S* pCard;
    HI_HANDLE   hTrack;
    hTrack = u32TrackID & AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(hTrack);

    if (pCard)
    {
        Ret = TRACK_SetPcmAttr(pCard, hTrack, hAi);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("call TRACK_SetPcmAttr failed!\n");
            return Ret;
        }

        Ret = AI_AttachTrack(hAi, hTrack);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("call AI_SetAttachFlag failed!\n");
            return Ret;
        }

        Ret = TRACK_AttachAi(pCard, hTrack, hAi);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("call TRACK_AttachAi failed!\n");
            return Ret;
        }
        return Ret;
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }

#else
    return HI_FAILURE;
#endif
}

HI_S32 AO_Track_DetachAi(HI_U32 u32TrackID, HI_HANDLE hAi)
{
#if defined (HI_AUDIO_AI_SUPPORT)
    HI_S32 Ret;
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        Ret = TRACK_DetachAi(pCard, u32TrackID);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("call TRACK_DetachAi failed!\n");
            return Ret;
        }

        Ret = AI_DetachTrack(hAi, u32TrackID);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("call AI_SetAttachFlag failed!\n");
            return Ret;
        }
        return Ret;
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }

#else
    return HI_FAILURE;
#endif
}

#ifdef __DPT__
HI_S32 AO_Track_SetNRAttr(HI_U32 u32TrackID, HI_UNF_AI_NR_ATTR_S* pstNRParam)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_SetNRAttr(pCard, u32TrackID, pstNRParam);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}
#endif

static HI_S32 AO_Track_MasterSlaveExchange(SND_CARD_STATE_S* pCard, HI_U32 u32TrackID)
{
    HI_U32 u32MTrackID;
    HI_UNF_AUDIOTRACK_ATTR_S* pstTrackAttr;
    HI_BOOL bAlsaTrack;
    AO_BUF_ATTR_S* pstBuf;
    HI_UNF_SND_E enSound;

    SND_TRACK_SETTINGS_S stTrackSettings;
    HI_S32 s32Ret;

    enSound = SND_CARD_GetSnd(pCard);
    if (AO_MAX_TOTAL_SND_NUM == enSound)
    {
        return HI_FAILURE;
    }

    u32MTrackID = TRACK_GetMasterId(pCard);
    if (AO_MAX_TOTAL_TRACK_NUM != u32MTrackID)  //judge if master track exist
    {
        //Master -> slave
        TRACK_GetSetting(pCard, u32MTrackID, &stTrackSettings); //save track setting
        //Master Track is NOT STOP,Not support Exchange
        if (SND_TRACK_STATUS_STOP != stTrackSettings.enCurnStatus)
        {
            HI_FATAL_AO("Exist Master Track(%d) is Not Stop!\n", u32MTrackID);
            return HI_FAILURE;
        }

        /* Destory track */
        s32Ret = AO_Track_Destory(u32MTrackID);
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("AO_Track_Destory fail\n");
            return HI_FAILURE;
        }

        pstTrackAttr = &stTrackSettings.stTrackAttr;
        //bAlsaTrack = stTrackSettings.bAlsaTrack;
        bAlsaTrack = HI_FALSE;        //ALSA TRACK NEVER EXCHANGE
        pstBuf = &stTrackSettings.stBufAttr;
        pstTrackAttr->enTrackType = HI_UNF_SND_TRACK_TYPE_SLAVE;

        /* Recreate slave track  */
        s32Ret = AO_Track_Create(enSound, pstTrackAttr, bAlsaTrack, pstBuf, u32MTrackID);
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("AO_Track_Create fail\n");
            return HI_FAILURE;
        }
        TRACK_RestoreSetting(pCard, u32MTrackID, &stTrackSettings); //restore track setting
    }

    if (u32MTrackID == u32TrackID)    //if input track id is master ,just return here
    {
        return HI_SUCCESS;
    }
    else
    {
        //slave -> Master
        TRACK_GetSetting(pCard, u32TrackID, &stTrackSettings);
        /* Destory track */
        s32Ret = AO_Track_Destory(u32TrackID);
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("AO_Track_Destory fail\n");
            return HI_FAILURE;
        }

        pstTrackAttr = &stTrackSettings.stTrackAttr;
        //bAlsaTrack = stTrackSettings.bAlsaTrack;
        bAlsaTrack = HI_FALSE;        //ALSA TRACK NEVER EXCHANGE
        pstBuf = &stTrackSettings.stBufAttr;
        pstTrackAttr->enTrackType = HI_UNF_SND_TRACK_TYPE_MASTER;

        /* Recreate Master track  */
        s32Ret = AO_Track_Create(enSound, pstTrackAttr, bAlsaTrack, pstBuf, u32TrackID);
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("AO_Track_Create fail\n");
            return HI_FAILURE;
        }

        TRACK_RestoreSetting(pCard, u32TrackID, &stTrackSettings);
        return HI_SUCCESS;
    }
}

HI_S32 AO_Track_PreCreate(HI_UNF_SND_E enSound, HI_UNF_AUDIOTRACK_ATTR_S* pstAttr,
                          HI_BOOL bAlsaTrack, AO_BUF_ATTR_S* pstBuf, HI_HANDLE hTrack)
{
    HI_U32 u32TrackID;
    HI_S32 s32Ret;

    SND_CARD_STATE_S* pCard;
    hTrack &= AO_TRACK_CHNID_MASK;
    pCard = SND_CARD_GetCard(enSound);

    if (pCard)
    {
        s32Ret = TRACK_CheckAttr(pstAttr);
        if (HI_SUCCESS != s32Ret)
        {
            return HI_FAILURE;
        }

        if (HI_UNF_SND_TRACK_TYPE_MASTER == pstAttr->enTrackType)
        {
            u32TrackID = TRACK_GetMasterId(pCard);
            if (AO_MAX_TOTAL_TRACK_NUM != u32TrackID)  //judge if master track exist
            {
                s32Ret = AO_Track_MasterSlaveExchange(pCard, u32TrackID);    //force master to slave
                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_AO("Failed to Force Master track(%d) To Slave!\n", u32TrackID);
                    return HI_FAILURE;
                }
            }
        }

        return AO_Track_Create(enSound, pstAttr, bAlsaTrack, pstBuf, hTrack);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Track_PreSetAttr(HI_U32 u32TrackID, HI_UNF_AUDIOTRACK_ATTR_S* pstTrackAttr)
{
    HI_S32 s32Ret;
    SND_CARD_STATE_S* pCard;
    SND_TRACK_ATTR_SETTING_E enAttrSetting;
    HI_UNF_AUDIOTRACK_ATTR_S stTrackTmpAttr = {0};
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        s32Ret = TRACK_DetectAttr(pCard, u32TrackID, pstTrackAttr, &enAttrSetting);
        if (HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }

        switch (enAttrSetting)
        {
            case SND_TRACK_ATTR_RETAIN:
                s32Ret = HI_SUCCESS;
                break;
            case SND_TRACK_ATTR_MODIFY:
                s32Ret = AO_Track_SetAttr(u32TrackID, pstTrackAttr);
                break;
            case SND_TRACK_ATTR_MASTER2SLAVE:
            case SND_TRACK_ATTR_SLAVE2MASTER:
                AO_Track_GetAttr(u32TrackID, &stTrackTmpAttr);          //save track attr
                s32Ret = AO_Track_SetAttr(u32TrackID, pstTrackAttr);
                s32Ret = AO_Track_MasterSlaveExchange(pCard, u32TrackID);
                if (HI_SUCCESS != s32Ret)
                {
                    AO_Track_SetAttr(u32TrackID, &stTrackTmpAttr);      //exchange failed ,restore track attr
                }
                break;

            default:
                s32Ret = HI_SUCCESS;        //TODO
                break;
        }

        return s32Ret;
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

HI_S32 AO_Track_UpdateBufWptr(HI_U32 u32TrackID, HI_U32* pu32WritePos)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_UpdateWptrPos(pCard, u32TrackID, pu32WritePos);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

HI_S32 AO_Track_UpdateBufRptr(HI_U32 u32TrackID, HI_U32* pu32ReadPos)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);
    if (pCard)
    {
        return TRACK_UpdateRptrPos(pCard, u32TrackID, pu32ReadPos);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

HI_S32 AO_Track_GetAipReadPos(HI_U32 u32TrackID, HI_U32* pu32ReadPos)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_GetReadPos(pCard, u32TrackID, pu32ReadPos);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

HI_S32 AO_Track_FlushBuf(HI_U32 u32TrackID)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    if (pCard)
    {
        return TRACK_FlushBuf(pCard, u32TrackID);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

#ifdef __DPT__
HI_S32 AO_Track_SetSource(HI_U32 u32TrackID, HI_UNF_SND_SOURCE_E enSource)
{
    SND_CARD_STATE_S* pCard;
    u32TrackID &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(u32TrackID);

    //except alsa track
    if ((HI_UNF_SND_SOURCE_ATV > enSource) || (HI_UNF_SND_SOURCE_BUTT <= enSource))
    {
        HI_ERR_AO("Invalid Source!\n");
        return HI_FAILURE;
    }

    if (pCard)
    {
        return TRACK_SetSource(pCard, u32TrackID, enSource);
    }
    else
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}
#endif

#ifdef HI_ALSA_AO_SUPPORT

/* track create kernel intf */
HI_S32 AO_Track_Kcreate(AO_Track_Create_Param_S_PTR  arg, struct file* file)
{
    HI_S32 s32Ret;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    AO_Track_Create_Param_S_PTR pstTrack = (AO_Track_Create_Param_S_PTR)arg;

    s32Ret = down_interruptible(&g_AoMutex);

    if (HI_SUCCESS == AO_Track_AllocHandle(pstTrack->enSound, pstTrack->stAttr.enTrackType, file, &hHandle))
    {
        s32Ret = AO_Track_Create(pstTrack->enSound, &pstTrack->stAttr, pstTrack->bAlsaTrack,
                                 &pstTrack->stBuf, hHandle);
        if (HI_SUCCESS != s32Ret)
        {
            AO_Track_FreeHandle(hHandle);
            up(&g_AoMutex);
            return HI_FAILURE;
        }

        AO_TRACK_SaveSuspendAttr(hHandle, pstTrack);
        pstTrack->hTrack = hHandle;
    }
    up(&g_AoMutex);

    return HI_SUCCESS;
}

/* track destroy kernel intf */
HI_S32 AO_Track_Kdestory(HI_HANDLE*  arg)
{
    HI_S32 s32Ret;
    HI_HANDLE hTrack = *(HI_HANDLE*)arg;

    s32Ret = down_interruptible(&g_AoMutex);

    CHECK_AO_TRACK_OPEN(hTrack);
    s32Ret = AO_Track_Destory( hTrack );
    if (HI_SUCCESS != s32Ret)
    {
        up(&g_AoMutex);
        return HI_FAILURE;
    }
    AO_Track_FreeHandle(hTrack);

    up(&g_AoMutex);
    return HI_SUCCESS;
}

/* track start kernel intf */
HI_S32 AO_Track_Kstart(HI_HANDLE*  arg)
{
    HI_S32 s32Ret;
    HI_HANDLE hTrack = *(HI_HANDLE*)arg;

    //s32Ret = down_interruptible(&g_AoMutex);  //alsa atomic area

    CHECK_AO_TRACK_OPEN(hTrack);
    s32Ret = AO_Track_Start(hTrack);

    //up(&g_AoMutex);
    return s32Ret;
}

/* track stop kernel intf */
HI_S32 AO_Track_Kstop(HI_HANDLE*  arg)
{
    HI_S32 s32Ret;
    HI_HANDLE hTrack = *(HI_HANDLE*)arg;

    //s32Ret = down_interruptible(&g_AoMutex);

    CHECK_AO_TRACK_OPEN(hTrack);
    s32Ret = AO_Track_Stop(hTrack);

    //up(&g_AoMutex);
    return s32Ret;
}
/* track stop kernel intf */
HI_S32 AO_Track_Kflush(HI_HANDLE*  arg)
{
    HI_S32 s32Ret;
    HI_HANDLE hTrack = *(HI_HANDLE*)arg;

    //s32Ret = down_interruptible(&g_AoMutex);

    CHECK_AO_TRACK_OPEN(hTrack);
    s32Ret = AO_Track_Flush(hTrack);

    //up(&g_AoMutex);
    return s32Ret;
}
#endif

/******************************Snd Cast FUNC*************************************/
#ifdef __DPT__
static HI_S32 AO_SND_SetAllCastMute(HI_UNF_SND_E enSound, HI_BOOL bMute)
{
    HI_S32 s32Ret;
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    CHECK_AO_NULL_PTR(pCard);

    s32Ret = CAST_SetAllMute(pCard, bMute);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("Set All Cast Mute Failed, Err 0x%x", s32Ret);
        return s32Ret;
    }

    pCard->bAllCastMute = bMute;
    HI_INFO_AO("Set All Cast Mute Staues %d\n", pCard->bAllCastMute);

    return s32Ret;
}

static HI_S32 AO_SND_GetAllCastMute(HI_UNF_SND_E enSound, HI_BOOL* pbMute)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_NULL_PTR(pbMute);
    CHECK_AO_NULL_PTR(pCard);

    *pbMute = pCard->bAllCastMute;
    return HI_SUCCESS;
}
#endif

static HI_S32 AO_Cast_AllocHandle(HI_HANDLE* phHandle, struct file* pstFile, HI_UNF_SND_CAST_ATTR_S* pstUserCastAttr)
{
    HI_U32 i;
    HI_S32 Ret;
    HI_U32 uFrameSize, uBufSize;
    MMZ_BUFFER_S stRbfMmz;

    if (HI_NULL == phHandle)
    {
        HI_ERR_AO("Bad param!\n");
        return HI_FAILURE;
    }

    /* Check ready flag */
    if (s_stAoDrv.bReady != HI_TRUE)
    {
        HI_ERR_AO("Need open first!\n");
        return HI_FAILURE;
    }

    /* Check channel number */
    if (s_stAoDrv.u32CastNum >= AO_MAX_CAST_NUM)
    {
        HI_ERR_AO("Too many Cast:%d!\n", s_stAoDrv.u32CastNum);
        goto err0;
    }

    /* Allocate new channel */
    for (i = 0; i < AO_MAX_CAST_NUM; i++)
    {
        if (0 == atomic_read(&s_stAoDrv.astCastEntity[i].atmUseCnt))
        {
            s_stAoDrv.astCastEntity[i].pstFile = HI_NULL;
            break;
        }
    }

    if (i >= AO_MAX_CAST_NUM)
    {
        HI_ERR_AO("Too many Cast chans!\n");
        goto err0;
    }

    /* Allocate cast mmz resource */
    uFrameSize = AUTIL_CalcFrameSize(AO_TRACK_NORMAL_CHANNELNUM, AO_TRACK_BITDEPTH_LOW); /* force 2ch 16bit */
    uBufSize = pstUserCastAttr->u32PcmFrameMaxNum * pstUserCastAttr->u32PcmSamplesPerFrame * uFrameSize;
    if (uBufSize > AO_CAST_MMZSIZE_MAX)
    {
        HI_ERR_AO("Invalid Cast FrameMaxNum(%d), PcmSamplesPerFrame(%d)!\n", pstUserCastAttr->u32PcmFrameMaxNum,
                  pstUserCastAttr->u32PcmSamplesPerFrame);
        goto err0;
    }

    Ret = HI_DRV_MMZ_AllocAndMap("AO_Cast", MMZ_OTHERS, AO_CAST_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AIAO("MMZ_AllocAndMap failed\n");
        goto err0;
    }

    s_stAoDrv.astCastEntity[i].stRbfMmz   = stRbfMmz;
    s_stAoDrv.astCastEntity[i].u32ReqSize = uBufSize;

    /* Allocate resource */
    s_stAoDrv.astCastEntity[i].pstFile = pstFile;
    s_stAoDrv.u32CastNum++;
    atomic_inc(&s_stAoDrv.astCastEntity[ i].atmUseCnt);
    *phHandle = (HI_ID_AO << 16) | (HI_ID_CAST << 8) | i;
    return HI_SUCCESS;

err0:
    return HI_FAILURE;
}

static HI_VOID AO_Cast_FreeHandle(HI_HANDLE hHandle)
{
    hHandle &= AO_CAST_CHNID_MASK;

    /* Freee cast mmz resource */
    HI_DRV_MMZ_UnmapAndRelease(&s_stAoDrv.astCastEntity[hHandle].stRbfMmz);

    s_stAoDrv.astCastEntity[hHandle].pstFile = HI_NULL;
    s_stAoDrv.u32CastNum--;
    atomic_set(&s_stAoDrv.astCastEntity[hHandle].atmUseCnt, 0);
}

#define CHECK_AO_CAST_OPEN(Cast) \
    do                                                         \
    {                                                          \
        CHECK_AO_CAST(Cast);                             \
        if (0 == atomic_read(&s_stAoDrv.astCastEntity[Cast & AO_CAST_CHNID_MASK].atmUseCnt))   \
        {                                                       \
            HI_WARN_AO(" Invalid Cast id 0x%x\n", Cast);        \
            return HI_ERR_AO_INVALID_PARA;                       \
        }                                                       \
    } while (0)


static SND_CARD_STATE_S* CAST_CARD_GetCard(HI_U32 Id)
{
    HI_UNF_SND_E sndx;
    SND_CARD_STATE_S* pCard = HI_NULL;

    if (Id >= AO_MAX_CAST_NUM)
    {
        HI_WARN_AO(" Invalid Cast id 0x%x\n", Id);
        return HI_NULL;
    }

    for (sndx = HI_UNF_SND_0; sndx < HI_UNF_SND_BUTT; sndx++)
    {
        pCard = SND_CARD_GetCard(sndx);
        if (pCard)
        {
            if (pCard->uSndCastInitFlag & (1L << Id))
            {
                return pCard;
            }
        }
    }

    return HI_NULL;
}

static HI_S32 AO_Cast_SetMute(HI_U32 u32CastID, HI_BOOL bMute)
{
    SND_CARD_STATE_S* pCard;

    u32CastID &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(u32CastID);
    if (pCard)
    {
        return CAST_SetMute(pCard, u32CastID, bMute);
    }
    else
    {
        HI_FATAL_AO("Ao Sound Not Open!\n");
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Cast_GetMute(HI_U32 u32CastID, HI_BOOL* pbMute)
{
    SND_CARD_STATE_S* pCard;

    u32CastID &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(u32CastID);

    if (pCard)
    {
        return CAST_GetMute(pCard, u32CastID, pbMute);
    }
    else
    {
        HI_FATAL_AO("Ao Sound Not Open!\n");
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}


static HI_S32 AO_Cast_SetAbsGain(HI_U32 u32CastID, HI_UNF_SND_ABSGAIN_ATTR_S stCastAbsGain)
{
    SND_CARD_STATE_S* pCard;
    u32CastID &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(u32CastID);

    if (pCard)
    {
        return CAST_SetAbsGain(pCard, u32CastID, &stCastAbsGain);
    }
    else
    {
        HI_FATAL_AO("Ao Sound Not Open!\n");
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

static HI_S32 AO_Cast_GetAbsGain(HI_U32 u32CastID, HI_UNF_SND_ABSGAIN_ATTR_S* pstCastAbsGain)
{

    SND_CARD_STATE_S* pCard;
    u32CastID &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(u32CastID);

    if (pCard)
    {
        return CAST_GetAbsGain(pCard, u32CastID, pstCastAbsGain);
    }
    else
    {
        HI_FATAL_AO("Ao Sound Not Open!\n");
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}

#ifdef __DPT__
static HI_S32 AO_Cast_SetAefBypass(HI_U32 u32CastID, HI_BOOL bBypass)
{
    SND_CARD_STATE_S* pCard;
    u32CastID &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(u32CastID);

    if (pCard)
    {
        return Cast_SetAefBypass(pCard, u32CastID, bBypass);
    }
    else
    {
        HI_FATAL_AO("Ao Sound Not Open!\n");
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }
}
#endif

static HI_S32 AO_Cast_GetDefAttr(HI_UNF_SND_CAST_ATTR_S* pstDefAttr)
{
    return CAST_GetDefAttr(pstDefAttr);
}

static HI_VOID AO_Cast_SaveSuspendAttr(HI_UNF_SND_E enSound, HI_HANDLE hHandle, HI_UNF_SND_CAST_ATTR_S* pstCastAttr)
{
    hHandle &= AO_TRACK_CHNID_MASK;
    s_stAoDrv.astCastEntity[hHandle].stSuspendAttr.enSound = enSound;
    s_stAoDrv.astCastEntity[hHandle].stSuspendAttr.stCastAttr = *pstCastAttr;
}

HI_S32 AO_Cast_Create(HI_UNF_SND_E enSound, HI_UNF_SND_CAST_ATTR_S* pstCastAttr, MMZ_BUFFER_S* pstMMz, HI_HANDLE hCast)
{
    HI_S32 s32Ret;
    SND_CARD_STATE_S* pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = SND_CARD_GetCard(enSound);
    CHECK_AO_NULL_PTR(pCard);

    s32Ret = CAST_CreateNew(pCard, pstCastAttr, pstMMz, hCast);
    if (HI_SUCCESS == s32Ret)
    {
        HI_INFO_AO("set cast %d mute or unmute \n", hCast);
        return CAST_SetMute(pCard, hCast, HI_FALSE);
    }

    return s32Ret;
}

HI_S32 AO_Cast_Destory(HI_HANDLE hCast)
{
    SND_CARD_STATE_S* pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    return CAST_DestroyCast(pCard, hCast);
}

HI_S32 AO_Cast_SetInfo(HI_HANDLE hCast, HI_VIRT_ADDR_T tUserVirtAddr)
{
    SND_CARD_STATE_S* pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    return CAST_SetInfo(pCard, hCast, tUserVirtAddr);
}

HI_S32 AO_Cast_GetInfo(HI_HANDLE hCast, AO_Cast_Info_Param_S* pstInfo)
{
    SND_CARD_STATE_S* pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    return CAST_GetInfo(pCard, hCast, pstInfo);
}

HI_S32 AO_Cast_SetEnable(HI_HANDLE hCast, HI_BOOL bEnable)
{
    SND_CARD_STATE_S* pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    return CAST_SetEnable(pCard, hCast, bEnable);
}

HI_S32 AO_Cast_GetEnable(HI_HANDLE hCast, HI_BOOL* pbEnable)
{
    SND_CARD_STATE_S* pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    return CAST_GetEnable(pCard, hCast, pbEnable);
}

static HI_S32 AO_Cast_ReadData(HI_HANDLE hCast, AO_Cast_Data_Param_S* pstCastData)
{
    SND_CARD_STATE_S* pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    return CAST_ReadData(pCard, hCast, pstCastData);
}

static HI_S32 AO_Cast_ReleseData(HI_HANDLE hCast, AO_Cast_Data_Param_S* pstCastData)
{
    SND_CARD_STATE_S* pCard;
    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    return CAST_ReleaseData(pCard, hCast, pstCastData);
}

/********************************Driver inteface FUNC****************************************/

#if (1 == HI_PROC_SUPPORT)
/*********************************** Code ************************************/
//zgjiere; proc兼容需求是否满足，起码风格类似于V1R1
static HI_BOOL bSaveThreadRunFlag = HI_FALSE;
#ifdef __DPT__
static HI_BOOL bSaveAefThreadRunFlag = HI_FALSE;
#endif
static HI_BOOL bSuspend2SaveThreadFlag = HI_FALSE;    //HI_TRUE meas suspend start, thread should exit
static volatile HI_BOOL bSaveThread2SuspendFlag = HI_FALSE;    //HI_TRUE means  suspend wait until hi_false
#ifdef __DPT__
static HI_BOOL bSuspend2SaveAefThreadFlag = HI_FALSE;    //HI_TRUE meas suspend start, thread should exit
static volatile HI_BOOL bSaveAefThread2SuspendFlag = HI_FALSE;    //HI_TRUE means  suspend wait until hi_false
#endif

#ifdef __DPT__
static HI_S32 AODebugShowHelp(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    HI_DRV_PROC_EchoHelper("-----------------------------------------------------------------------------SOUND HELP-----------------------------------------------------------------------------\n");
    HI_DRV_PROC_EchoHelper("echo command1         para1             command2      para2            path                explanation\n\n");
    HI_DRV_PROC_EchoHelper("echo save_track       [track_id]                      [start/stop]  >  /proc/msp/sound%d    save pcm data from track(track_id:0,1,2...)\n", enSound);
    HI_DRV_PROC_EchoHelper("echo save_sound       [start/stop]                                  >  /proc/msp/sound%d    save pcm data from sound\n", enSound);
    HI_DRV_PROC_EchoHelper("echo save_sound aef   [start/stop]                                  >  /proc/msp/sound%d    save pcm data with audio effect proccess from sound\n", enSound);
    HI_DRV_PROC_EchoHelper("echo                  TRACK[track_id]   weight        [0~100]       >  /proc/msp/sound%d    set track L/R weight(track_id:0,1,2...)\n", enSound);
    HI_DRV_PROC_EchoHelper("echo                  TRACK[track_id]   prescale      [-81~18]      >  /proc/msp/sound%d    set track mute(track_id:0,1,2...;1:mute on;0:mute off)\n", enSound);
    HI_DRV_PROC_EchoHelper("echo                  TRACK[track_id]   mute          [0/1]         >  /proc/msp/sound%d    set track prescale(track_id:0,1,2...;step:0.125dB)\n", enSound);
    HI_DRV_PROC_EchoHelper("echo                  aef[aef_id]                     [0/1]         >  /proc/msp/sound%d    set aef attach|detach(aef_id:0...7;1:attach;0:detach)\n", enSound);
    HI_DRV_PROC_EchoHelper("echo                  [outport]         aefbypass     [0/1]         >  /proc/msp/sound%d    set outport aefbypass(outport:DAC0/DAC1/SPDIF0/I2S0/...;0:off;1:on)\n", enSound);
    HI_DRV_PROC_EchoHelper("echo                  [outport]         mute          [0/1]         >  /proc/msp/sound%d    set outport mute(outport:DAC0/DAC1/SPDIF0/I2S0/...;0:off;1:on)\n", enSound);
    HI_DRV_PROC_EchoHelper("echo                  [outport]         precivolume   [-81~18]      >  /proc/msp/sound%d    set outport precivolume(outport:DAC0/DAC1/SPDIF0/I2S0/...;step:0.125dB)\n", enSound);
    HI_DRV_PROC_EchoHelper("echo                  [outport]         balance       [-50~50]      >  /proc/msp/sound%d    set outport balance(outport:DAC0/DAC1/SPDIF0/I2S0/...;step:1dB)\n", enSound);
    HI_DRV_PROC_EchoHelper("echo                  [outport]         delaycomps    [0~500]       >  /proc/msp/sound%d    set outport delay compensation(outport:DAC0/DAC1/SPDIF0/I2S0/...;step:1ms)\n", enSound);
    HI_DRV_PROC_EchoHelper("echo                  [outport]         drc           [0/1]         >  /proc/msp/sound%d    outport drc switch(outport:DAC0/DAC1/SPDIF0/I2S0/...;1:on;0:off)\n", enSound);
    HI_DRV_PROC_EchoHelper("echo                  [outport]         peq           [0/1]         >  /proc/msp/sound%d    outport peq switch(outport:DAC0/DAC1/SPDIF0/I2S0/...;1:on;0:off)\n", enSound);
    HI_DRV_PROC_EchoHelper("echo                                    geq           [0/1]         >  /proc/msp/sound%d    outport geq switch(outport:DAC0/DAC1/SPDIF0/I2S0/...;1:on;0:off)\n", enSound);
    HI_DRV_PROC_EchoHelper("echo                                    avc           [0/1]         >  /proc/msp/sound%d    outport avc switch(1:on;0:off)\n", enSound);
    HI_DRV_PROC_EchoHelper("---------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    return HI_SUCCESS;
}

static HI_S32 AODebugShowPivHelp(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    HI_DRV_PROC_EchoHelper("\necho TRACK0 weight -40dB > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo TRACK0 prescale -10.500dB > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo TRACK0 mute on|off > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("\necho DAC0 aefbypass on|off > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo DAC0 drc on|off > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo DAC0 drc attacktime|releasetime 1000ms|1000ms > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo DAC0 drc thrhd|limit -10dB|-20dB > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo DAC0 mute on|off > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo DAC0 volume -40dB > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo DAC0 precivolume -40dB > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo DAC0 balance -40 > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo DAC0 peq on|off > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo DAC0 peq band0 type|gain|Q|freq HP|-10dB|0.7|1000Hz > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("\necho aef0 attach|detach > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("\necho geq on|off > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo geq band0 gain -10dB > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("\necho avc on|off > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo avc attacktime|releasetime 1000ms|1000ms > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo avc thrhd|limit -30dB|-10dB > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("echo avc gain 5dB > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("\necho castsimulateop I2S1 on|off > /proc/msp/sound%d\n\n", enSound);

    return HI_SUCCESS;
}

#else

static HI_S32 AODebugShowHelp(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    HI_DRV_PROC_EchoHelper("\nfunction: save pcm data from track\n");
    HI_DRV_PROC_EchoHelper("commad:   echo save_track track_id start|stop > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("example:  echo save_track 0 start > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("function: \nfunction: save pcm data from sound\n");
    HI_DRV_PROC_EchoHelper("commad:   echo save_sound start|stop > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("example:  echo save_sound start > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("function: \nfunction: save hdmi or spdif output mode\n");
    HI_DRV_PROC_EchoHelper("commad:   echo hdmi=pcm|hdmi=auto|hdmi=raw|hdmi=hbr2lbr|spdif=pcm|spdif=raw > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("example:  echo hdmi=hbr2lbr > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("example:  echo spdif=raw > /proc/msp/sound%d\n\n", enSound);
    HI_DRV_PROC_EchoHelper("function: \nfunction: echo DAC|HDMI|SPDIF|I2S0 mute on|off > /proc/msp/sound%d\n", enSound);
    HI_DRV_PROC_EchoHelper("example:  echo DAC0 mute on|off > /proc/msp/sound%d\n", enSound);

    return HI_SUCCESS;
}
#endif

static HI_S32 SndProcSaveThread(void* Arg)
{
    HI_S32 s32Ret;
    SND_PCM_SAVE_ATTR_S* pstThreadArg = (SND_PCM_SAVE_ATTR_S*)Arg;
    SND_PCM_SAVE_ATTR_S stThreadArg;

    //use cast
    HI_UNF_SND_CAST_ATTR_S stCastAttr;
    AO_Cast_Create_Param_S  stCastParam;
    AO_Cast_Enable_Param_S stEnableAttr;
    AO_Cast_Info_Param_S stCastInfo;
    AO_Cast_Data_Param_S stCastData;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    stThreadArg.enSound = pstThreadArg->enSound;
    stThreadArg.pstFileHandle = pstThreadArg->pstFileHandle;
    stThreadArg.pstDevfileHandle = pstThreadArg->pstDevfileHandle;

    CHECK_AO_SNDCARD_OPEN(stThreadArg.enSound);
    CHECK_AO_NULL_PTR(stThreadArg.pstFileHandle);

    s32Ret = down_interruptible(&g_AoMutex);
    bSaveThread2SuspendFlag = HI_TRUE;

    s32Ret = AO_Cast_GetDefAttr(&stCastAttr);
    if (HI_SUCCESS != s32Ret)
    {
        up(&g_AoMutex);
        goto Close_File;
    }

    stCastParam.enSound = stThreadArg.enSound;
    memcpy(&stCastParam.stCastAttr, &stCastAttr, sizeof(HI_UNF_SND_CAST_ATTR_S));

    if (HI_SUCCESS == AO_Cast_AllocHandle(&hHandle, stThreadArg.pstFileHandle, &stCastParam.stCastAttr))
    {
        s32Ret = AO_Cast_Create(stCastParam.enSound, &stCastParam.stCastAttr, &s_stAoDrv.astCastEntity[hHandle & AO_CAST_CHNID_MASK].stRbfMmz,
                                hHandle);
        if (HI_SUCCESS != s32Ret)
        {
            AO_Cast_FreeHandle(hHandle);
            up(&g_AoMutex);
            goto Close_File;
        }
        //AO_Cast_SaveSuspendAttr(stCastParam.enSound, hHandle, &stCastParam.stCastAttr);   //NO resume
        stCastParam.u32ReqSize = s_stAoDrv.astCastEntity[hHandle & AO_CAST_CHNID_MASK].u32ReqSize;
        stCastParam.hCast = hHandle;
    }
    else
    {
        up(&g_AoMutex);
        goto Close_File;
    }

#ifdef __DPT__
    (HI_VOID)AO_Cast_SetAefBypass(stCastParam.hCast, pstThreadArg->bAefBypass);
#endif

    stCastInfo.hCast = stCastParam.hCast;
    s32Ret = AO_Cast_GetInfo(stCastInfo.hCast, &stCastInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" AO_Cast_GetInfo Failed\n");
        up(&g_AoMutex);
        goto Destory_Cast;
    }
    //HI_ERR_AO(" stCastInfo u32KernelVirtAddr=0x%x \n", stCastInfo.u32KernelVirtAddr);

    stEnableAttr.hCast = stCastParam.hCast;
    stEnableAttr.bCastEnable = HI_TRUE;
    s32Ret = AO_Cast_SetEnable(stEnableAttr.hCast, stEnableAttr.bCastEnable);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" AO_Cast_SetEnable Enable Failed\n");
        up(&g_AoMutex);
        //return HI_FAILURE;
        goto Destory_Cast;
    }
    up(&g_AoMutex);

    stCastData.hCast = stCastParam.hCast;
    while (HI_TRUE == bSaveThreadRunFlag) // NO !kthread_should_stop() to avoid dead lock
    {
        HI_U32 u32PcmSize;

        s32Ret = down_interruptible(&g_AoMutex);
        if (bSuspend2SaveThreadFlag == HI_TRUE)
        {
            up(&g_AoMutex);
            goto Destory_Cast;
        }

        s32Ret = AO_Cast_ReadData(stCastData.hCast, &stCastData);
        up(&g_AoMutex);
        if (HI_SUCCESS == s32Ret)
        {
            if (stCastData.stAOFrame.u32PcmSamplesPerFrame == 0)
            {
                msleep(5);
                continue;
            }
            else
            {
                //HI_ERR_AO(" Once Length : %d\n", u32PcmSize);
                //HI_ERR_AO(" Once Offset : %d\n", stCastData.u32DataOffset);
                u32PcmSize = stCastData.stAOFrame.u32PcmSamplesPerFrame * stCastData.stAOFrame.u32Channels * stCastData.stAOFrame.s32BitPerSample / 8;
                if (stThreadArg.pstFileHandle)
                {
                    HI_DRV_FILE_Write(stThreadArg.pstFileHandle, (HI_S8*)(stCastInfo.tKernelVirtAddr + stCastData.u32DataOffset) , u32PcmSize);
                }
                else
                {
                    HI_ERR_AO("stThreadArg.fileHandle is NULL!\n");
                    goto Destory_Cast;
                }
                s32Ret = down_interruptible(&g_AoMutex);
                if (bSuspend2SaveThreadFlag == HI_TRUE)
                {
                    up(&g_AoMutex);
                    goto Destory_Cast;
                }

                s32Ret = AO_Cast_ReleseData(stCastData.hCast, &stCastData);
                up(&g_AoMutex);
                if (HI_SUCCESS != s32Ret)
                {
                    goto Close_File;
                }
            }
        }
        else
        {
            goto Close_File;
        }
    }

Destory_Cast:
    CHECK_AO_CAST_OPEN(stCastParam.hCast);
    s32Ret = down_interruptible(&g_AoMutex);
    s32Ret = AO_Cast_Destory(stCastParam.hCast);
    AO_Cast_FreeHandle(stCastParam.hCast);
    up(&g_AoMutex);
Close_File:
    if (stThreadArg.pstFileHandle)
    {
        HI_DRV_FILE_Close(stThreadArg.pstFileHandle);
    }

    s32Ret = AO_Snd_Kclose(stThreadArg.enSound, stThreadArg.pstDevfileHandle);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("AO_Snd_Kclose %d failed \n", (HI_U32)stThreadArg.enSound);
    }

    s32Ret = AO_DRV_Krelease(stThreadArg.pstDevfileHandle);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("AO_DRV_Krelease\n");
    }

    bSaveThread2SuspendFlag = HI_FALSE;
    return HI_SUCCESS;
}

#ifdef __DPT__
static HI_S32 SndProcSaveAefThread(void* Arg)
{
    HI_S32 s32Ret;
    SND_PCM_SAVE_ATTR_S* pstThreadArg = (SND_PCM_SAVE_ATTR_S*)Arg;
    SND_PCM_SAVE_ATTR_S stThreadArg;

    //use cast
    HI_UNF_SND_CAST_ATTR_S stCastAttr;
    AO_Cast_Create_Param_S stCastParam;
    AO_Cast_Enable_Param_S stEnableAttr;
    AO_Cast_Info_Param_S   stCastInfo;
    AO_Cast_Data_Param_S   stCastData;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    stThreadArg.enSound = pstThreadArg->enSound;
    stThreadArg.pstFileHandle = pstThreadArg->pstFileHandle;
    stThreadArg.pstDevfileHandle = pstThreadArg->pstDevfileHandle;

    CHECK_AO_SNDCARD_OPEN(stThreadArg.enSound);
    CHECK_AO_NULL_PTR(stThreadArg.pstFileHandle);

    s32Ret = down_interruptible(&g_AoMutex);
    bSaveAefThread2SuspendFlag = HI_TRUE;

    s32Ret = AO_Cast_GetDefAttr(&stCastAttr);
    if (HI_SUCCESS != s32Ret)
    {
        up(&g_AoMutex);
        goto Close_File;
    }

    stCastParam.enSound = stThreadArg.enSound;
    memcpy(&stCastParam.stCastAttr, &stCastAttr, sizeof(HI_UNF_SND_CAST_ATTR_S));

    if (HI_SUCCESS == AO_Cast_AllocHandle(&hHandle, stThreadArg.pstFileHandle, &stCastParam.stCastAttr))
    {
        s32Ret = AO_Cast_Create(stCastParam.enSound, &stCastParam.stCastAttr, &s_stAoDrv.astCastEntity[hHandle & AO_CAST_CHNID_MASK].stRbfMmz,
                                hHandle);
        if (HI_SUCCESS != s32Ret)
        {
            AO_Cast_FreeHandle(hHandle);
            up(&g_AoMutex);
            goto Close_File;
        }
        //AO_Cast_SaveSuspendAttr(stCastParam.enSound, hHandle, &stCastParam.stCastAttr);   //NO resume
        stCastParam.u32ReqSize = s_stAoDrv.astCastEntity[hHandle & AO_CAST_CHNID_MASK].u32ReqSize;
        stCastParam.hCast = hHandle;
    }
    else
    {
        up(&g_AoMutex);
        goto Close_File;
    }

    (HI_VOID)AO_Cast_SetAefBypass(stCastParam.hCast, pstThreadArg->bAefBypass);//应为false

    stCastInfo.hCast = stCastParam.hCast;
    s32Ret = AO_Cast_GetInfo(stCastInfo.hCast, &stCastInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" AO_Cast_GetInfo Failed\n");
        up(&g_AoMutex);
        goto Destory_Cast;
    }

    stEnableAttr.hCast = stCastParam.hCast;
    stEnableAttr.bCastEnable = HI_TRUE;
    s32Ret = AO_Cast_SetEnable(stEnableAttr.hCast, stEnableAttr.bCastEnable);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" AO_Cast_SetEnable Enable Failed\n");
        up(&g_AoMutex);
        goto Destory_Cast;
    }
    up(&g_AoMutex);

    stCastData.hCast = stCastParam.hCast;
    while (HI_TRUE == bSaveAefThreadRunFlag) // NO !kthread_should_stop() to avoid dead lock
    {
        HI_U32 u32PcmSize;

        s32Ret = down_interruptible(&g_AoMutex);
        if (bSuspend2SaveAefThreadFlag == HI_TRUE)
        {
            up(&g_AoMutex);
            goto Destory_Cast;
        }

        s32Ret = AO_Cast_ReadData(stCastData.hCast, &stCastData);
        up(&g_AoMutex);
        if (HI_SUCCESS == s32Ret)
        {
            if (stCastData.stAOFrame.u32PcmSamplesPerFrame == 0)
            {
                msleep(5);
                continue;
            }
            else
            {
                u32PcmSize = stCastData.stAOFrame.u32PcmSamplesPerFrame * stCastData.stAOFrame.u32Channels * stCastData.stAOFrame.s32BitPerSample / 8;
                if (stThreadArg.pstFileHandle)
                {
                    HI_DRV_FILE_Write(stThreadArg.pstFileHandle, (HI_S8*)(stCastInfo.tKernelVirtAddr + stCastData.u32DataOffset) , u32PcmSize);
                }
                else
                {
                    HI_ERR_AO("stThreadArg.fileHandle is NULL!\n");
                    goto Destory_Cast;
                }
                s32Ret = down_interruptible(&g_AoMutex);
                if (bSuspend2SaveAefThreadFlag == HI_TRUE)
                {
                    up(&g_AoMutex);
                    goto Destory_Cast;
                }

                s32Ret = AO_Cast_ReleseData(stCastData.hCast, &stCastData);
                up(&g_AoMutex);
                if (HI_SUCCESS != s32Ret)
                {
                    goto Close_File;
                }
            }
        }
        else
        {
            goto Close_File;
        }
    }

Destory_Cast:
    CHECK_AO_CAST_OPEN(stCastParam.hCast);
    s32Ret = down_interruptible(&g_AoMutex);
    s32Ret = AO_Cast_Destory(stCastParam.hCast);
    AO_Cast_FreeHandle(stCastParam.hCast);
    up(&g_AoMutex);
Close_File:
    if (stThreadArg.pstFileHandle)
    {
        HI_DRV_FILE_Close(stThreadArg.pstFileHandle);
    }

    s32Ret = AO_Snd_Kclose(stThreadArg.enSound, stThreadArg.pstDevfileHandle);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("AO_Snd_Kclose %d failed \n", (HI_U32)stThreadArg.enSound);
    }

    s32Ret = AO_DRV_Krelease(stThreadArg.pstDevfileHandle);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("AO_DRV_Krelease\n");
    }

    bSaveAefThread2SuspendFlag = HI_FALSE;
    return HI_SUCCESS;
}


HI_S32 SND_AefWriteProc(SND_CARD_STATE_S* pCard, HI_BOOL bBypass, SND_DEBUG_CMD_CTRL_E enCmd)
{
    HI_CHAR szPath[AO_SOUND_PATH_NAME_MAXLEN + AO_SOUND_FILE_NAME_MAXLEN] = {0};
    HI_UNF_SND_E  enSound;
    static struct  task_struct* g_pstSndSaveAefThread = NULL;
    static SND_PCM_SAVE_ATTR_S  stThreadArg;
    static AO_SND_Open_Param_S  stSndOpenParam;
    static struct file   g_file;    //just a dev handle no use
    struct tm now;
    HI_S32 s32Ret;

    enSound = SND_CARD_GetSnd(pCard);

    if (SND_DEBUG_CMD_CTRL_START == enCmd && pCard->enSaveAefState == SND_DEBUG_CMD_CTRL_STOP)
    {
        if (HI_SUCCESS != HI_DRV_FILE_GetStorePath(szPath, AO_SOUND_PATH_NAME_MAXLEN))
        {
            HI_ERR_AO("get store path failed\n");
            return HI_FAILURE;
        }

        time_to_tm(get_seconds(), 0, &now);

        snprintf(szPath, sizeof(szPath), "%s/sound%d_aef_%02u_%02u_%02u.pcm", szPath, (HI_U32)enSound, now.tm_hour, now.tm_min, now.tm_sec);

        pCard->pstAefFileHandle = HI_DRV_FILE_Open(szPath, 1);
        if (!pCard->pstAefFileHandle)
        {
            HI_ERR_AO("open %s error\n", szPath);
            return HI_FAILURE;
        }

        stThreadArg.enSound = enSound;
        stThreadArg.bAefBypass = bBypass;
        stThreadArg.pstFileHandle = pCard->pstAefFileHandle;
        stThreadArg.pstDevfileHandle = &g_file;

        stSndOpenParam.enSound = enSound;
        up(&g_AoMutex);
        s32Ret = AO_DRV_Kopen(&g_file);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("AO_DRV_Kopen failed\n");
        }

        s32Ret = AO_Snd_Kopen(&stSndOpenParam, HI_NULL, &g_file);    //never first open
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("AO_Snd_Kopen failed\n");
        }
        s32Ret = down_interruptible(&g_AoMutex);

        bSaveAefThreadRunFlag = HI_TRUE;
        g_pstSndSaveAefThread = kthread_create(SndProcSaveAefThread, &stThreadArg, "AoSndProcSaveAef");        //Name To Do
        if (IS_ERR(g_pstSndSaveAefThread))
        {
            HI_ERR_AO("creat sound proc aef write thread failed\n");
            return HI_FAILURE;
        }

        wake_up_process(g_pstSndSaveAefThread);
        HI_DRV_PROC_EchoHelper("Started saving aef sound data to %s\n", szPath);

        pCard->u32SaveAefCnt++;
    }
    else if (SND_DEBUG_CMD_CTRL_START == enCmd && pCard->enSaveAefState == SND_DEBUG_CMD_CTRL_START)
    {
        HI_DRV_PROC_EchoHelper("Started saving aef sound data already.\n");
    }

    if (SND_DEBUG_CMD_CTRL_STOP == enCmd && pCard->enSaveAefState == SND_DEBUG_CMD_CTRL_START)
    {
        bSaveAefThreadRunFlag = HI_FALSE;
        //kthread_stop(g_pstSndSaveThread);
        g_pstSndSaveAefThread = HI_NULL;
        HI_DRV_PROC_EchoHelper("Finished saving aef sound data.\n");

        //Warnning : HI_DRV_FILE_Close called in Thread, To avoid hold mutex lock long time
    }
    else if (SND_DEBUG_CMD_CTRL_STOP == enCmd && pCard->enSaveAefState == SND_DEBUG_CMD_CTRL_STOP)
    {
        HI_DRV_PROC_EchoHelper("No aef sound data saved, please start saving.\n");
        AODebugShowHelp(enSound, pCard, HI_NULL);
    }

    if (pCard)
    {
        pCard->enSaveAefState = enCmd;
    }

    return HI_SUCCESS;
}
#endif

HI_S32 SND_WriteProc(SND_CARD_STATE_S* pCard, HI_BOOL bBypass, SND_DEBUG_CMD_CTRL_E enCmd)
{
    HI_CHAR szPath[AO_SOUND_PATH_NAME_MAXLEN + AO_SOUND_FILE_NAME_MAXLEN] = {0};
    HI_UNF_SND_E enSound;
    static struct  task_struct* g_pstSndSaveThread = NULL; //name todo
    static SND_PCM_SAVE_ATTR_S stThreadArg;
    static AO_SND_Open_Param_S stSndOpenParam;
    static struct file g_file; //just a dev handle no use
    struct tm now;
    HI_S32 s32Ret;
    enSound = SND_CARD_GetSnd(pCard);

    if (SND_DEBUG_CMD_CTRL_START == enCmd && pCard->enSaveState == SND_DEBUG_CMD_CTRL_STOP)
    {
        if (HI_SUCCESS != HI_DRV_FILE_GetStorePath(szPath, AO_SOUND_PATH_NAME_MAXLEN))
        {
            HI_ERR_AO("get store path failed\n");
            return HI_FAILURE;
        }

        time_to_tm(get_seconds(), 0, &now);

        snprintf( szPath, sizeof(szPath), "%s/sound%d_%02u_%02u_%02u.pcm", szPath, (HI_U32)enSound, now.tm_hour, now.tm_min, now.tm_sec);

        pCard->pstFileHandle = HI_DRV_FILE_Open(szPath, 1);
        if (!pCard->pstFileHandle)
        {
            HI_ERR_AO("open %s error\n", szPath);
            return HI_FAILURE;
        }

        stThreadArg.enSound = enSound;
        stThreadArg.bAefBypass = bBypass;
        stThreadArg.pstFileHandle = pCard->pstFileHandle;
        stThreadArg.pstDevfileHandle = &g_file;

        stSndOpenParam.enSound = enSound;
        up(&g_AoMutex);
        s32Ret = AO_DRV_Kopen(&g_file);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("AO_DRV_Kopen failed\n");
        }

        s32Ret = AO_Snd_Kopen(&stSndOpenParam, HI_NULL, &g_file);    //never first open
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("AO_Snd_Kopen failed\n");
        }
        s32Ret = down_interruptible(&g_AoMutex);

        bSaveThreadRunFlag = HI_TRUE;
        g_pstSndSaveThread = kthread_create(SndProcSaveThread, &stThreadArg, "AoSndProcSave");        //Name To Do
        if (IS_ERR(g_pstSndSaveThread))
        {
            HI_ERR_AO("creat sound proc write thread failed\n");
            return HI_FAILURE;
        }
        wake_up_process(g_pstSndSaveThread);
        HI_DRV_PROC_EchoHelper("Started saving sound data to %s\n", szPath);

        pCard->u32SaveCnt++;
    }
    else if (SND_DEBUG_CMD_CTRL_START == enCmd && pCard->enSaveState == SND_DEBUG_CMD_CTRL_START)
    {
        HI_DRV_PROC_EchoHelper("Started saving sound data already.\n");
    }

    if (SND_DEBUG_CMD_CTRL_STOP == enCmd && pCard->enSaveState == SND_DEBUG_CMD_CTRL_START)
    {
        bSaveThreadRunFlag = HI_FALSE;
        //kthread_stop(g_pstSndSaveThread);
        g_pstSndSaveThread = HI_NULL;
        HI_DRV_PROC_EchoHelper("Finished saving sound data.\n");

        //Warnning : HI_DRV_FILE_Close called in Thread, To avoid hold mutex lock long time
    }
    else if (SND_DEBUG_CMD_CTRL_STOP == enCmd && pCard->enSaveState == SND_DEBUG_CMD_CTRL_STOP)
    {
        HI_DRV_PROC_EchoHelper("No sound data saved, please start saving.\n");
        AODebugShowHelp(enSound, pCard, HI_NULL);
    }

    if (pCard)
    {
        pCard->enSaveState = enCmd;
    }

    return HI_SUCCESS;
}

static HI_S32 AOReadSndProc(struct seq_file* p, HI_UNF_SND_E enSnd)
{
    HI_U32 i;
    DRV_SND_ATTR_S* pstSndAttr;
    SND_CARD_STATE_S* pCard;
#ifdef __DPT__
    HI_S32 s32DecimalGain;
    HI_UNF_EDID_AUDIO_FORMAT_CODE_E enFmt;
#endif

    pCard = SND_CARD_GetCard(enSnd);
    if (HI_NULL == pCard)
    {
        PROC_PRINT( p, "\n------------------------------------  SOUND[%d] Not Open ----------------------------------\n", (HI_U32)enSnd );
        return HI_SUCCESS;
    }

    PROC_PRINT( p, "\n-------------------------------------------  SOUND[%d]  Status  ----------------------------------------------------\n", (HI_U32)enSnd );
    pstSndAttr = &pCard->stUserOpenParam;

    PROC_PRINT( p,
                "SampleRate   :%d\n",
                pstSndAttr->enSampleRate );

    if (SND_MODE_NONE != pCard->enSpdifPassthrough)
    {
        PROC_PRINT( p,
                    "SPDIF Status :UserSetMode(%s) DataFormat(%s)\n",
                    AUTIL_SpdifMode2Name(pCard->enUserSpdifMode),
                    AUTIL_Format2Name(pCard->u32SpdifDataFormat));
    }

#ifdef __DPT__
    if (HI_TRUE == pCard->bUserArcEnable)
    {
        PROC_PRINT( p,
                    "ARC   Status :UserSetMode(%s) DataFormat(%s)",
                    AUTIL_ArcMode2Name(pCard->enUserArcMode),
                    AUTIL_Format2Name(pCard->u32SpdifDataFormat));
        if (HI_UNF_SND_ARC_AUDIO_MODE_AUTO == pCard->enUserArcMode)
        {
            PROC_PRINT( p, " SupportFormat(");
            for (enFmt = HI_UNF_EDID_AUDIO_FORMAT_CODE_RESERVED; enFmt < HI_UNF_EDID_AUDIO_FORMAT_CODE_BUTT; enFmt++)
            {
                if (HI_TRUE == pCard->stUserArcCap.bAudioFmtSupported[enFmt])
                {
                    PROC_PRINT( p, "[%s]", AUTIL_EdidFormat2Name(enFmt));
                }
            }
            PROC_PRINT( p, ")\n");
        }
        else
        {
            PROC_PRINT( p, "\n");
        }
    }
#endif

    if (SND_MODE_NONE != pCard->enHdmiPassthrough)
    {
        PROC_PRINT( p,
                    "HDMI Status  :UserSetMode(%s) DataFormat(%s)\n",
                    AUTIL_HdmiMode2Name(pCard->enUserHdmiMode),
                    AUTIL_Format2Name(pCard->u32HdmiDataFormat));
    }

    if (HI_TRUE == pCard->bDMAMode)
    {
        PROC_PRINT( p, "\nSND DMA Mode :On\n");
    }

#ifdef __DPT__
    PROC_PRINT( p, "\nAll Mute     :Track(%s)", ((HI_FALSE == pCard->bAllTrackMute) ? "off" : "on"));

    PROC_PRINT( p, ", Cast(%s)\n", ((HI_FALSE == pCard->bAllCastMute) ? "off" : "on"));


    if (HI_TRUE == pCard->bAvcEnable)
    {
        PROC_PRINT( p, "AVC Attr     :");
        PROC_PRINT( p, "AttackTime (%dms), ReleaseTime (%dms), Thrhd (%ddB), gain (%ddB), Limit (%ddB)",
                    pCard->stAvcAttr.u32AttackTime,
                    pCard->stAvcAttr.u32ReleaseTime,
                    pCard->stAvcAttr.s32ThresholdLevel,
                    pCard->stAvcAttr.s32Gain,
                    pCard->stAvcAttr.s32LimiterLevel);
        PROC_PRINT( p, "\n");
    }

    if (HI_TRUE == pCard->bGeqEnable)
    {
        PROC_PRINT( p, "GEQ Attr     :");
        for (i = 0; i < pCard->stGeqAttr.u32BandNum; i++)
        {
            s32DecimalGain = pCard->stGeqAttr.stEqParam[i].s32Gain % 1000;
            PROC_PRINT( p,
                        "Band%d(%dHz,%d.%.3ddB) ",
                        i,
                        pCard->stGeqAttr.stEqParam[i].u32freq,
                        pCard->stGeqAttr.stEqParam[i].s32Gain / 1000,
                        (s32DecimalGain < 0) ? -s32DecimalGain : s32DecimalGain);

            if (0 == ((i + 1) % 5) && (pCard->stGeqAttr.u32BandNum != (i + 1))) //a line 5 Band
            {
                PROC_PRINT( p, "\n              ");
            }
        }
        PROC_PRINT( p, "\n");
    }
#endif

    PROC_PRINT( p, "\n---------------------------------------------  OutPort Status  ---------------------------------------------\n" );
    for (i = 0; i < pstSndAttr->u32PortNum; i++)
    {
        SND_ReadOpProc( p, pCard, pstSndAttr->stOutport[i].enOutPort );
    }

    if (pCard->uSndCastInitFlag)
    {
        PROC_PRINT( p, "------------------------------------------------ Cast Status  ----------------------------------------------\n" );
        CAST_ReadProc(p, pCard);
    }

    if (pCard->uSndTrackInitFlag)
    {
        PROC_PRINT( p, "------------------------------------------------ Track Status  ----------------------------------------------\n" );
        TRACK_ReadProc( p, pCard );
    }

#ifdef __DPT__
    if (pCard->u32AttAef)
    {
        PROC_PRINT( p, "--------------------------------------------- Audio Effect Status  ------------------------------------------\n" );
        AEF_ReadProc( p, pCard );
    }
#endif
    PROC_PRINT( p, "--------------------------------------------------------------------------------------------------------------------\n");

    return HI_SUCCESS;
}

HI_S32 AO_DRV_ReadProc(struct seq_file* file, HI_VOID* v)
{
    HI_U32 u32Snd;
    DRV_PROC_ITEM_S* pstProcItem;

    pstProcItem = file->private;

    u32Snd = (pstProcItem->entry_name[5] - '0');
    if (u32Snd >= AO_MAX_TOTAL_SND_NUM)
    {
        PROC_PRINT(file, "Invalid Sound ID:%d.\n", u32Snd);
        return HI_FAILURE;
    }

    AOReadSndProc(file, (HI_UNF_SND_E)u32Snd);

    return HI_SUCCESS;
}

static HI_S32 AO_Proc_SetHdmiDebug(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    pCard->bHdmiDebug = !(pCard->bHdmiDebug);
    return HI_SUCCESS;
}

static HI_S32 AO_Proc_SetHdmiModeLpcm(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    return AO_SND_SetHdmiMode(enSound, HI_UNF_SND_OUTPUTPORT_HDMI0, HI_UNF_SND_HDMI_MODE_LPCM);
}

static HI_S32 AO_Proc_SetHdmiModeRaw(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    return AO_SND_SetHdmiMode(enSound, HI_UNF_SND_OUTPUTPORT_HDMI0, HI_UNF_SND_HDMI_MODE_RAW);
}

static HI_S32 AO_Proc_SetHdmiModeHbr2Lbr(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    return AO_SND_SetHdmiMode(enSound, HI_UNF_SND_OUTPUTPORT_HDMI0, HI_UNF_SND_HDMI_MODE_HBR2LBR);
}

static HI_S32 AO_Proc_SetHdmiModeAuto(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    return AO_SND_SetHdmiMode(enSound, HI_UNF_SND_OUTPUTPORT_HDMI0, HI_UNF_SND_HDMI_MODE_AUTO);
}

static HI_S32 AO_Proc_SetSpdifModeLpcm(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    return AO_SND_SetSpdifMode(enSound, HI_UNF_SND_OUTPUTPORT_SPDIF0, HI_UNF_SND_SPDIF_MODE_LPCM);
}

static HI_S32 AO_Proc_SetSpdifModeRaw(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    return AO_SND_SetSpdifMode(enSound, HI_UNF_SND_OUTPUTPORT_SPDIF0, HI_UNF_SND_SPDIF_MODE_RAW);
}

#ifdef __DPT__
static HI_BOOL AO_PROC_JUDGE_OUTPORT(HI_CHAR* pcBuf)
{
    if ((pcBuf == strstr(pcBuf, "DAC0")) || (pcBuf == strstr(pcBuf, "DAC1")) || (pcBuf == strstr(pcBuf, "DAC2")) || (pcBuf == strstr(pcBuf, "DAC3"))
        || (pcBuf == strstr(pcBuf, "I2S0")) || (pcBuf == strstr(pcBuf, "I2S1")) || (pcBuf == strstr(pcBuf, "SPDIF0"))
        || (pcBuf == strstr(pcBuf, "HDMI0")) || (pcBuf == strstr(pcBuf, "ARC0")) || (pcBuf == strstr(pcBuf, "ALLPORT")))
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}
#endif

static HI_S32 AO_WriteProc_SaveTrack(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    HI_U32 u32TrackId = AO_MAX_TOTAL_TRACK_NUM;
    SND_DEBUG_CMD_CTRL_E enCtrlCmd;
    HI_CHAR* pcStartCmd = "start";
    HI_CHAR* pcStopCmd = "stop";

    if (pcBuf[0] < '0' || pcBuf[0] > '9')//do not have param
    {
        return HI_FAILURE;
    }

    u32TrackId = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
    if (u32TrackId >= AO_MAX_TOTAL_TRACK_NUM)
    {
        return HI_FAILURE;
    }

    AO_STRING_SKIP_NON_BLANK(pcBuf);
    AO_STRING_SKIP_BLANK(pcBuf);

    if (pcBuf == strstr(pcBuf, pcStartCmd))
    {
        enCtrlCmd = SND_DEBUG_CMD_CTRL_START;
    }
    else if (pcBuf == strstr(pcBuf, pcStopCmd))
    {
        enCtrlCmd = SND_DEBUG_CMD_CTRL_STOP;
    }
    else
    {
        return HI_FAILURE;
    }

    if (HI_SUCCESS != TRACK_WriteProc_SaveData(pCard, u32TrackId, enCtrlCmd))
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#ifdef __DPT__
static HI_S32 AO_WriteProc_AVC(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    HI_CHAR* pcOnCmd = "on";
    HI_CHAR* pcOffCmd = "off";
    HI_CHAR *pcOneCmd = "1";
    HI_CHAR *pcZeroCmd = "0";
    HI_CHAR* pcAttackTime = "attacktime";
    HI_CHAR* pcReleaseTime = "releasetime";
    HI_CHAR* pcThrhd = "thrhd";
    HI_CHAR* pcLimit = "limit";
    HI_CHAR* pgain = "gain";
    HI_BOOL  bAvcEnable = HI_FALSE;
    HI_S32   s32Ret;
    HI_UNF_SND_AVC_ATTR_S stAvcAttr = {0};

    if ((pcBuf == strstr(pcBuf, pcOnCmd)) || (pcBuf == strstr(pcBuf, pcOneCmd)))
    {
        s32Ret = AO_SND_GetAvcEnable(enSound, &bAvcEnable);
        if (HI_SUCCESS != s32Ret)
        {
            HI_DRV_PROC_EchoHelper("Get AVC enable failed!\n");
        }

        s32Ret = AO_SND_SetAvcEnable(enSound, HI_TRUE);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("Set AVC enable failed!\n");
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Set AVC success: %s -> 1(ON)\n", bAvcEnable ? "1(ON)" : "0(OFF)");
        }

        return s32Ret;
    }
    else if ((pcBuf == strstr(pcBuf, pcOffCmd)) || (pcBuf == strstr(pcBuf, pcZeroCmd)))
    {
        s32Ret = AO_SND_GetAvcEnable(enSound, &bAvcEnable);
        if (HI_SUCCESS != s32Ret)
        {
            HI_DRV_PROC_EchoHelper("Get AVC enable failed!\n");
        }

        s32Ret = AO_SND_SetAvcEnable(enSound, HI_FALSE);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("Set AVC enable failed!\n");
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Set AVC success: %s -> 0(OFF)\n", bAvcEnable ? "1(ON)" : "0(OFF)");
        }

        return s32Ret;
    }
    else
    {
        AO_SND_GetAvcAttr(enSound, &stAvcAttr);
        if (pcBuf == strstr(pcBuf, pcAttackTime))
        {
            pcBuf += strlen(pcAttackTime);
            AO_STRING_SKIP_BLANK(pcBuf);
            stAvcAttr.u32AttackTime = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
        }
        else if (pcBuf == strstr(pcBuf, pcReleaseTime))
        {
            pcBuf += strlen(pcReleaseTime);
            AO_STRING_SKIP_BLANK(pcBuf);
            stAvcAttr.u32ReleaseTime = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
        }
        else if (pcBuf == strstr(pcBuf, pcThrhd))
        {
            pcBuf += strlen(pcThrhd);
            AO_STRING_SKIP_BLANK(pcBuf);
            stAvcAttr.s32ThresholdLevel = simple_strtol(pcBuf, &pcBuf, 10);
        }
        else if (pcBuf == strstr(pcBuf, pcLimit))
        {
            pcBuf += strlen(pcLimit);
            AO_STRING_SKIP_BLANK(pcBuf);
            stAvcAttr.s32LimiterLevel = simple_strtol(pcBuf, &pcBuf, 10);
        }
        else if (pcBuf == strstr(pcBuf, pgain))
        {
            pcBuf += strlen(pgain);
            AO_STRING_SKIP_BLANK(pcBuf);
            stAvcAttr.s32Gain = simple_strtoul(pcBuf, &pcBuf, 10);
        }
        else
        {
            return HI_FAILURE;
        }

        AO_Check_AvcAttr(&stAvcAttr);
        return AO_SND_SetAvcAttr(enSound, &stAvcAttr);
    }
}

static HI_S32 AO_WriteProc_GEQ(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    HI_CHAR* pcOnCmd = "on";
    HI_CHAR* pcOffCmd = "off";
    HI_CHAR *pcOneCmd = "1";
    HI_CHAR *pcZeroCmd = "0";
    HI_CHAR* pcBand = "band";
    HI_CHAR* pcGain = "gain";
    HI_U32   u32BandId;
    HI_U32   u32Val1, u32Val2;
    HI_S32   s32Ret;
    HI_BOOL  bNagetive = HI_FALSE;
    HI_BOOL  bGeqEnable = HI_FALSE;
    HI_UNF_SND_GEQ_ATTR_S stGeqAttr = {0};

    if ((pcBuf == strstr(pcBuf,pcOnCmd)) || (pcBuf == strstr(pcBuf,pcOneCmd)))
    {
        s32Ret = AO_SND_GetGeqEnable(enSound, &bGeqEnable);
        if (HI_SUCCESS != s32Ret)
        {
            HI_DRV_PROC_EchoHelper("Get GEQ enable failed!\n");
        }

        s32Ret = AO_SND_SetGeqEnable(enSound, HI_TRUE);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("Set GEQ enable failed!\n");
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Set GEQ success: %s -> 1(ON)\n", bGeqEnable ? "1(ON)" : "0(OFF)");
        }

        return s32Ret;
    }
    else if ((pcBuf == strstr(pcBuf,pcOffCmd)) || (pcBuf == strstr(pcBuf,pcZeroCmd)))
    {
        s32Ret = AO_SND_GetGeqEnable(enSound, &bGeqEnable);
        if (HI_SUCCESS != s32Ret)
        {
            HI_DRV_PROC_EchoHelper("Get GEQ enable failed!\n");
        }

        s32Ret = AO_SND_SetGeqEnable(enSound, HI_FALSE);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("Set GEQ enable failed!\n");
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Set GEQ success: %s -> 0(OFF)\n", bGeqEnable ? "1(ON)" : "0(OFF)");
        }

        return s32Ret;
    }
    else if (pcBuf == strstr(pcBuf, pcBand))
    {
        AO_SND_GetGeqAttr(enSound, &stGeqAttr);

        pcBuf += strlen(pcBand);
        u32BandId = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
        if (u32BandId >= stGeqAttr.u32BandNum)
        {
            return HI_FAILURE;
        }
        AO_STRING_SKIP_BLANK(pcBuf);
        if (pcBuf == strstr(pcBuf, pcGain))
        {
            pcBuf += strlen(pcGain);
            AO_STRING_SKIP_BLANK(pcBuf);
            if (pcBuf[0] == '-')
            {
                bNagetive = HI_TRUE;
                pcBuf++;
            }
            u32Val1 = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
            if (pcBuf[0] == '.')
            {
                pcBuf++;
                u32Val2 = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
            }
            else
            {
                u32Val2 = 0;
            }
            stGeqAttr.stEqParam[u32BandId].s32Gain = (HI_TRUE == bNagetive) ? (-(u32Val1 * 1000 + u32Val2)) : (u32Val1 * 1000 + u32Val2);
            return AO_SND_SetGeqAttr(enSound, &stGeqAttr);
        }
        else
        {
            return HI_FAILURE; //TODO
        }
    }
    else
    {
        return HI_FAILURE;   //TODO
    }
}
#endif

HI_S32 AO_WriteProc_SaveSound(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    SND_DEBUG_CMD_CTRL_E enCtrlCmd;
    HI_BOOL bBypass;
    HI_CHAR* pcStartCmd = "start";
    HI_CHAR* pcStopCmd = "stop";
    HI_CHAR* pcAefCmd = "aef";

    if (pcBuf == strstr(pcBuf, pcAefCmd))
    {
        bBypass = HI_FALSE;
        pcBuf += strlen(pcAefCmd);
        AO_STRING_SKIP_BLANK(pcBuf);
    }
    else
    {
        bBypass = HI_TRUE;
    }

    if (pcBuf == strstr(pcBuf, pcStartCmd))
    {
        enCtrlCmd = SND_DEBUG_CMD_CTRL_START;
    }
    else if (pcBuf == strstr(pcBuf, pcStopCmd))
    {
        enCtrlCmd = SND_DEBUG_CMD_CTRL_STOP;
    }
    else
    {
        return HI_FAILURE;
    }

#ifdef __DPT__
    if (HI_FALSE == bBypass)
    {
        if (HI_SUCCESS != SND_AefWriteProc(pCard, HI_FALSE, enCtrlCmd))
        {
            return HI_FAILURE;
        }
    }
    else
#endif
    {
        if (HI_SUCCESS != SND_WriteProc(pCard, HI_TRUE, enCtrlCmd))
        {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 AO_DRV_WriteProc(struct file* file, const char __user* buf, size_t count, loff_t* ppos)
{
    HI_S32 s32Ret;
    HI_U32 u32Snd;
    HI_U32 u32CmdId;
    SND_CARD_STATE_S* pCard;
    HI_CHAR szBuf[256] = {0};
    HI_CHAR* pcBuf = szBuf;

    struct seq_file* p = file->private_data;
    DRV_PROC_ITEM_S* pstProcItem = p->private;

    SND_WRITE_PROC_S stWritePoc[] =
    {
        {"help",            AODebugShowHelp},
#ifdef __DPT__
        {"*help",           AODebugShowPivHelp},
#endif
        {"save_sound",      AO_WriteProc_SaveSound},
        {"save_track",      AO_WriteProc_SaveTrack},
        {"TRACK",           TRACK_WriteProc},
        {"hdmi=debug",      AO_Proc_SetHdmiDebug},
        {"hdmi=pcm",        AO_Proc_SetHdmiModeLpcm},
        {"hdmi=raw",        AO_Proc_SetHdmiModeRaw},
        {"hdmi=hbr2lbr",    AO_Proc_SetHdmiModeHbr2Lbr},
        {"hdmi=auto",       AO_Proc_SetHdmiModeAuto},
        {"spdif=pcm",       AO_Proc_SetSpdifModeLpcm},
        {"spdif=raw",       AO_Proc_SetSpdifModeRaw},
#ifdef __DPT__
        {"aef",             AEF_WriteProc},
        {"geq",             AO_WriteProc_GEQ},
        {"avc",             AO_WriteProc_AVC},
        {"castsimulateop",  AO_WriteProc_CastSimulateOp}
#endif
    };

    s32Ret = down_interruptible(&g_AoMutex);

    if (copy_from_user(szBuf, buf, count < sizeof(szBuf) - 1 ? count : sizeof(szBuf) - 1))
    {
        HI_ERR_AO("copy from user failed\n");
        up(&g_AoMutex);
        return HI_FAILURE;
    }

    // sizeof("sound") is 5
    u32Snd = (pstProcItem->entry_name[5] - '0');
    if (u32Snd >= AO_MAX_TOTAL_SND_NUM)
    {
        HI_ERR_AO("Invalid Sound ID:%d\n", u32Snd);
        return HI_FAILURE;
    }

    pCard = SND_CARD_GetCard((HI_UNF_SND_E)u32Snd);
    if (HI_NULL == pCard)
    {
        HI_ERR_AO("Sound %d is not open\n", u32Snd);
        goto SAVE_CMD_FAULT;
    }

    AO_STRING_SKIP_BLANK(pcBuf);

#ifdef __DPT__
    if (HI_TRUE == AO_PROC_JUDGE_OUTPORT(pcBuf))
    {
        if (HI_SUCCESS != SND_WriteOpProc(pCard, pcBuf))
        {
            goto SAVE_CMD_FAULT;
        }
    }
#endif

    for (u32CmdId = 0; u32CmdId < sizeof(stWritePoc) / sizeof(stWritePoc[0]); u32CmdId++)
    {
        if (pcBuf == strstr(pcBuf, stWritePoc[u32CmdId].pCmd))
        {
            pcBuf += strlen(stWritePoc[u32CmdId].pCmd);
            AO_STRING_SKIP_BLANK(pcBuf);
            if (HI_SUCCESS != stWritePoc[u32CmdId].func(u32Snd, pCard, pcBuf))
            {
                goto SAVE_CMD_FAULT;
            }
            break;
        }
    }

    up(&g_AoMutex);
    return count;

SAVE_CMD_FAULT:
    HI_ERR_AO("proc cmd is fault\n");
    AODebugShowHelp(u32Snd, pCard, pcBuf);
    up(&g_AoMutex);
    return HI_FAILURE;
}

static HI_VOID AO_RegProc(HI_U32 u32Snd)
{
    HI_CHAR aszBuf[16];
    DRV_PROC_ITEM_S* pProcItem;

    /* Create proc */
    snprintf(aszBuf, sizeof(aszBuf), "sound%d", u32Snd);
    pProcItem = HI_DRV_PROC_AddModule(aszBuf, HI_NULL, HI_NULL);
    if (!pProcItem)
    {
        HI_INFO_AO("Create sound%d proc entry fail!\n", u32Snd);
        return;
    }

    /* Set functions */
    pProcItem->read  = s_stAoDrv.stProcParam.pfnReadProc;
    pProcItem->write = s_stAoDrv.stProcParam.pfnWriteProc;

    HI_INFO_AO("Create sound%d proc entry OK!\n", u32Snd);
    return;
}

static HI_VOID AO_UnRegProc(HI_U32 u32Snd)
{
    HI_CHAR aszBuf[16];
    snprintf(aszBuf, sizeof(aszBuf), "sound%d", u32Snd);

    HI_DRV_PROC_RemoveModule(aszBuf);
}
#endif

#if defined(HI_AIAO_VERIFICATION_SUPPORT)
DRV_PROC_EX_S stAIAOCbbOpt =
{
    .fnRead = AIAO_VERI_ProcRead,
};
#endif

static HI_S32 AO_OpenDev(HI_VOID)
{
    HI_U32 i;

    HI_S32 s32Ret;

    /* Init global track parameter */
    for (i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
    {
        atomic_set(&s_stAoDrv.astTrackEntity[i].atmUseCnt, 0);
    }

    s_stAoDrv.u32SndNum = 0;
    /* Init global snd parameter */
    for (i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
    {
        atomic_set(&s_stAoDrv.astSndEntity[i].atmUseTotalCnt, 0);
    }

    s_stAoDrv.pAdspFunc = HI_NULL;
    s_stAoDrv.pstPDMFunc = HI_NULL;

    /* Get pdm functions */
    s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_PDM, (HI_VOID**)&s_stAoDrv.pstPDMFunc);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_AO("Get pdm function err:%#x!\n", s32Ret);
        goto err;
    }

    /* Get adsp functions */
    s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_ADSP, (HI_VOID**)&s_stAoDrv.pAdspFunc);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_AO("Get adsp function err:%#x!\n", s32Ret);
        goto err;
    }

    /* HAL_AOE_Init , Init aoe hardare */
    if (s_stAoDrv.pAdspFunc && s_stAoDrv.pAdspFunc->pfnADSP_LoadFirmware)
    {
        s32Ret = (s_stAoDrv.pAdspFunc->pfnADSP_LoadFirmware)(ADSP_CODE_AOE);
        if (HI_SUCCESS != s32Ret)
        {
            goto err;
        }
        if (s_stAoDrv.pAdspFunc && s_stAoDrv.pAdspFunc->pfnADSP_GetAoeFwmInfo)
        {
            ADSP_FIRMWARE_AOE_INFO_S stAoeInfo;
            s32Ret = (s_stAoDrv.pAdspFunc->pfnADSP_GetAoeFwmInfo)(ADSP_CODE_AOE, &stAoeInfo);
            if (HI_SUCCESS != s32Ret)
            {
                s32Ret = (s_stAoDrv.pAdspFunc->pfnADSP_UnLoadFirmware)(ADSP_CODE_AOE);
                goto err;
            }
            HAL_AOE_Init(stAoeInfo.bAoeSwFlag);
        }
    }

    /* HAL_AIAO_Init, Init aiao hardare */
    HAL_AIAO_Init();

    /* HAL_CAST_Init , Init cast hardare */
    HAL_CAST_Init();

#if defined(HI_AIAO_VERIFICATION_SUPPORT)
    {
        DRV_PROC_ITEM_S* item;
        AIAO_VERI_Open();
        item = HI_DRV_PROC_AddModule(AIAO_VERI_PROC_NAME, &stAIAOCbbOpt, NULL);
        if (!item)
        {
            HI_WARN_AIAO("add proc aiao_port failed\n");
        }
    }
#endif

    /* Set ready flag */
    s_stAoDrv.bReady = HI_TRUE;

    HI_INFO_AO("AO_OpenDev OK.\n");
    return HI_SUCCESS;
err:
    return HI_FAILURE;
}

static HI_S32 AO_CloseDev(HI_VOID)
{
    HI_U32 i, j;
    HI_S32 s32Ret;

    DRV_AO_STATE_S* pAOState = HI_NULL;

    /* Reentrant */
    if (s_stAoDrv.bReady == HI_FALSE)
    {
        return HI_SUCCESS;
    }

    /* Set ready flag */
    s_stAoDrv.bReady = HI_FALSE;

    /* Free all Cast */
    for (i = 0; i < AO_MAX_CAST_NUM; i++)
    {
        if (atomic_read(&s_stAoDrv.astCastEntity[i].atmUseCnt))
        {
            (HI_VOID)AO_Cast_Destory( i );
            AO_Cast_FreeHandle(i);
        }
    }

    /* Free all track */
    for (i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
    {
        if (atomic_read(&s_stAoDrv.astTrackEntity[i].atmUseCnt))
        {
#ifdef __DPT__
            AO_SND_UpdateTrackInfo(i, HI_FALSE);
#endif
            (HI_VOID)AO_Track_Destory(i);
            AO_Track_FreeHandleById(i);
        }
    }

    for (i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
    {
        if (s_stAoDrv.astSndDMAEntity[i].pstFile != HI_NULL)
        {
            AO_Snd_DMADestory(s_stAoDrv.astSndDMAEntity[i].pstFile, i);
        }
    }

    /* Free all snd */
    for (i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
    {
        if (s_stAoDrv.astSndEntity[i].pCard)
        {
            for (j = 0; j < SND_MAX_OPEN_NUM; j++)
            {
                if (s_stAoDrv.astSndEntity[i].pstFile[j] != 0)
                {
                    HI_U32 u32UserOpenCnt = 0;
                    pAOState = (DRV_AO_STATE_S*)((s_stAoDrv.astSndEntity[i].pstFile[j]))->private_data;

                    u32UserOpenCnt = atomic_read(&pAOState->atmUserOpenCnt[i]);
                    if (atomic_sub_and_test(u32UserOpenCnt, &s_stAoDrv.astSndEntity[i].atmUseTotalCnt))
                    {
                        (HI_VOID)AO_SND_Close( i, HI_FALSE );
                    }
                    AO_Snd_FreeHandle(i, s_stAoDrv.astSndEntity[i].pstFile[j]);
                }
            }
        }
    }

    /* HAL_AOE_DeInit */
    if (s_stAoDrv.pAdspFunc && s_stAoDrv.pAdspFunc->pfnADSP_UnLoadFirmware)
    {
        HAL_AOE_DeInit( );
        s32Ret = (s_stAoDrv.pAdspFunc->pfnADSP_UnLoadFirmware)(ADSP_CODE_AOE);
    }

    /* HAL_CAST_DeInit  */
    HAL_CAST_DeInit();

    /* HAL_AIAO_DeInit */
    HAL_AIAO_DeInit();

#if defined(HI_AIAO_VERIFICATION_SUPPORT)
    HI_DRV_PROC_RemoveModule(AIAO_VERI_PROC_NAME);
    AIAO_VERI_Release();
#endif

    return HI_SUCCESS;
}

static HI_S32 AO_ProcessCmd(struct inode* inode, struct file* file, HI_U32 cmd, HI_VOID* arg)
{
    HI_S32 Ret = HI_SUCCESS;

    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    HI_UNF_SND_E enSound = HI_UNF_SND_BUTT;

    CHECK_AO_NULL_PTR(arg);

#if defined(HI_AIAO_VERIFICATION_SUPPORT)
    if ((cmd & 0xff) >= CMD_AIAO_VERI_IOCTL)
    {
        return AIAO_VERI_ProcessCmd(inode, file, cmd, arg);
    }
#endif

    switch (cmd)
    {
        //Snd CMD TYPE(call hal_aiao)
        case CMD_AO_GETSNDDEFOPENATTR:
        {
            Ret = AOGetSndDefOpenAttr((AO_SND_OpenDefault_Param_S_PTR)arg);
            break;
        }

        case CMD_AO_SND_OPEN:
        {
            DRV_AO_STATE_S* pAOState = file->private_data;
            AO_SND_Open_Param_S_PTR pstSndParam = (AO_SND_Open_Param_S_PTR)arg;
            enSound = pstSndParam->enSound;
            CHECK_AO_SNDCARD(enSound);

            Ret = AO_Snd_AllocHandle(enSound, file);
            if (HI_SUCCESS == Ret)
            {
                if (0 == atomic_read(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
                {
                    Ret = AO_SND_Open( enSound, &pstSndParam->stAttr, NULL, HI_FALSE);
                    if (HI_SUCCESS != Ret)
                    {
                        AO_Snd_FreeHandle(enSound, file);
                        break;
                    }
                }
            }

            atomic_inc(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
            atomic_inc(&pAOState->atmUserOpenCnt[enSound]);

            break;
        }

        case CMD_AO_SND_CLOSE:
        {
            DRV_AO_STATE_S* pAOState = file->private_data;
            enSound = *(HI_UNF_SND_E*)arg;
            CHECK_AO_SNDCARD_OPEN(enSound);

            if (atomic_dec_and_test(&pAOState->atmUserOpenCnt[enSound]))
            {
                if (atomic_dec_and_test(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
                {
                    Ret = AO_SND_Close(enSound, HI_FALSE);
                    if (HI_SUCCESS != Ret)
                    {
                        atomic_inc(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
                        atomic_inc(&pAOState->atmUserOpenCnt[enSound]);
                        break;
                    }

                    AO_Snd_FreeHandle(enSound, file);
                }
            }
            else
            {
                atomic_dec(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
            }
            Ret = HI_SUCCESS;
            break;
        }

        case CMD_AO_SND_SETMUTE:
        {
            AO_SND_Mute_Param_S_PTR pstMute = (AO_SND_Mute_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstMute->enSound );
            Ret = AO_SND_SetMute(pstMute->enSound, pstMute->enOutPort, pstMute->bMute);
            break;
        }

        case CMD_AO_SND_GETMUTE:
        {
            AO_SND_Mute_Param_S_PTR pstMute = (AO_SND_Mute_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstMute->enSound );
            Ret = AO_SND_GetMute(pstMute->enSound, pstMute->enOutPort, &pstMute->bMute);
            break;
        }

        case CMD_AO_SND_SETHDMIMODE:
        {
            AO_SND_HdmiMode_Param_S_PTR pstMode = (AO_SND_HdmiMode_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstMode->enSound );
            Ret = AO_SND_SetHdmiMode(pstMode->enSound, pstMode->enOutPort, pstMode->enMode);
            break;
        }
        case CMD_AO_SND_GETHDMIMODE:
        {
            AO_SND_HdmiMode_Param_S_PTR pstMode = (AO_SND_HdmiMode_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstMode->enSound );
            Ret = AO_SND_GetHdmiMode(pstMode->enSound, pstMode->enOutPort, &pstMode->enMode);
            break;
        }

        case CMD_AO_SND_SETSPDIFMODE:
        {
            AO_SND_SpdifMode_Param_S_PTR pstMode = (AO_SND_SpdifMode_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstMode->enSound );
            Ret = AO_SND_SetSpdifMode(pstMode->enSound, pstMode->enOutPort, pstMode->enMode);
            break;
        }
        case CMD_AO_SND_GETSPDIFMODE:
        {
            AO_SND_SpdifMode_Param_S_PTR pstMode = (AO_SND_SpdifMode_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstMode->enSound );
            Ret = AO_SND_GetSpdifMode(pstMode->enSound, pstMode->enOutPort, &pstMode->enMode);
            break;
        }

        case CMD_AO_SND_SETVOLUME:
        {
            AO_SND_Volume_Param_S_PTR pstVolume = (AO_SND_Volume_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstVolume->enSound );
            Ret = AO_SND_SetVolume(pstVolume->enSound, pstVolume->enOutPort, &pstVolume->stGain);
            break;
        }

        case CMD_AO_SND_GETVOLUME:
        {
            AO_SND_Volume_Param_S_PTR pstVolume = (AO_SND_Volume_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstVolume->enSound );
            Ret = AO_SND_GetVolume(pstVolume->enSound, pstVolume->enOutPort, &pstVolume->stGain);
            break;
        }

        case CMD_AO_SND_SETSPDIFSCMSMODE:
        {
            AO_SND_SpdifSCMSMode_Param_S_PTR pstSCMSMode = (AO_SND_SpdifSCMSMode_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstSCMSMode->enSound );
            Ret = AO_SND_SetSpdifSCMSMode(pstSCMSMode->enSound, pstSCMSMode->enOutPort, pstSCMSMode->enSCMSMode);
            break;
        }

        case CMD_AO_SND_GETSPDIFSCMSMODE:
        {
            AO_SND_SpdifSCMSMode_Param_S_PTR pstSCMSMode = (AO_SND_SpdifSCMSMode_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstSCMSMode->enSound );
            Ret = AO_SND_GetSpdifSCMSMode(pstSCMSMode->enSound, pstSCMSMode->enOutPort, &pstSCMSMode->enSCMSMode);
            break;
        }
#ifdef __DPT__
        case CMD_AO_SND_SETALSAPRESCALE:
        {
            AO_SND_Set_Alsa_Prescale_S_PTR pstAlsaPrescale = (AO_SND_Set_Alsa_Prescale_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstAlsaPrescale->enSound );
            Ret = AO_SND_SetAlsaPrescale(pstAlsaPrescale->enSound, &(pstAlsaPrescale->stPreciGain));
            break;
        }
#endif
        case CMD_AO_SND_SETSPDIFCATEGORYCODE:
        {
            AO_SND_SpdifCategoryCode_Param_S_PTR pstCategoryCode = (AO_SND_SpdifCategoryCode_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstCategoryCode->enSound );
            Ret = AO_SND_SetSpdifCategoryCode(pstCategoryCode->enSound, pstCategoryCode->enOutPort, pstCategoryCode->enCategoryCode);
            break;
        }

        case CMD_AO_SND_GETSPDIFCATEGORYCODE:
        {
            AO_SND_SpdifCategoryCode_Param_S_PTR pstCategoryCode = (AO_SND_SpdifCategoryCode_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstCategoryCode->enSound );
            Ret = AO_SND_GetSpdifCategoryCode(pstCategoryCode->enSound, pstCategoryCode->enOutPort, &pstCategoryCode->enCategoryCode);
            break;
        }

        case CMD_AO_SND_SETSAMPLERATE:
        {
            AO_SND_SampleRate_Param_S_PTR pstSampleRate = (AO_SND_SampleRate_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstSampleRate->enSound );
            Ret = AO_SND_SetSampleRate(pstSampleRate->enSound, pstSampleRate->enOutPort, pstSampleRate->enSampleRate);
            break;
        }

        case CMD_AO_SND_GETSAMPLERATE:
        {
            AO_SND_SampleRate_Param_S_PTR pstSampleRate = (AO_SND_SampleRate_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstSampleRate->enSound );
            Ret = AO_SND_GetSampleRate(pstSampleRate->enSound, pstSampleRate->enOutPort, &pstSampleRate->enSampleRate);
            break;
        }

        case CMD_AO_SND_SETTRACKMODE:
        {
            AO_SND_TrackMode_Param_S_PTR pstMode = (AO_SND_TrackMode_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstMode->enSound );
            Ret = AO_SND_SetTrackMode(pstMode->enSound, pstMode->enOutPort, pstMode->enMode);
            break;
        }

        case CMD_AO_SND_GETTRACKMODE:
        {
            AO_SND_TrackMode_Param_S_PTR pstMode = (AO_SND_TrackMode_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstMode->enSound );
            Ret = AO_SND_GetTrackMode(pstMode->enSound, pstMode->enOutPort, &pstMode->enMode);
            break;
        }

        case CMD_AO_SND_SETALLTRACKMUTE:
        {
            AO_SND_AllTrackMute_Param_S_PTR pstAllMute = (AO_SND_AllTrackMute_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstAllMute->enSound );
            Ret = AO_SND_SetAllTrackMute(pstAllMute->enSound, pstAllMute->bMute);
            break;
        }

        case CMD_AO_SND_GETALLTRACKMUTE:
        {
            AO_SND_AllTrackMute_Param_S_PTR pstAllMute = (AO_SND_AllTrackMute_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstAllMute->enSound );
            Ret = AO_SND_GetAllTrackMute(pstAllMute->enSound, &pstAllMute->bMute);
            break;
        }

        case CMD_AO_SND_SETSMARTVOLUME:
        {
            AO_SND_SmartVolume_Param_S_PTR pstSmartVol = (AO_SND_SmartVolume_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstSmartVol->enSound );
            Ret = AO_SND_SetSmartVolume(pstSmartVol->enSound, pstSmartVol->enOutPort, pstSmartVol->bSmartVolume);
            break;
        }

        case CMD_AO_SND_GETSMARTVOLUME:
        {
            AO_SND_SmartVolume_Param_S_PTR pstSmartVol = (AO_SND_SmartVolume_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstSmartVol->enSound );
            Ret = AO_SND_GetSmartVolume(pstSmartVol->enSound, pstSmartVol->enOutPort, &pstSmartVol->bSmartVolume);
            break;
        }

        case CMD_AO_SND_SETLOWLATENCY:
        {
            AO_SND_Set_LowLatency_Param_S_PTR pstLowLatencyParam = (AO_SND_Set_LowLatency_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstLowLatencyParam->enSound);
            Ret = AO_Snd_SetLowLatency(pstLowLatencyParam->enSound, pstLowLatencyParam->eOutPort, pstLowLatencyParam->u32LatencyMs);
            break;
        }

        case CMD_AO_SND_GETLOWLATENCY:
        {
            AO_SND_Set_LowLatency_Param_S_PTR pstLowLatencyParam = (AO_SND_Set_LowLatency_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstLowLatencyParam->enSound);
            Ret = AO_Snd_GetLowLatency(pstLowLatencyParam->enSound, pstLowLatencyParam->eOutPort, &pstLowLatencyParam->u32LatencyMs);
            break;
        }

        case CMD_AO_SND_SETSOUNDDELAY:
        {
            AO_SND_OpDelay_Param_S_PTR pstOpDelay = (AO_SND_OpDelay_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstOpDelay->enSound );
            Ret = AO_SND_SetDelayCompensation(pstOpDelay->enSound, pstOpDelay->enOutPort, pstOpDelay->u32DelayMs);
            break;
        }

        case CMD_AO_SND_GETSOUNDDELAY:
        {
            AO_SND_OpDelay_Param_S_PTR pstOpDelay = (AO_SND_OpDelay_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstOpDelay->enSound );
            Ret = AO_SND_GetDelayCompensation(pstOpDelay->enSound, pstOpDelay->enOutPort, &pstOpDelay->u32DelayMs);
            break;
        }

#ifdef __DPT__
        case CMD_AO_SND_SETPRECIVOL:
        {
            AO_SND_PreciVolume_Param_S_PTR pstPreciVol = (AO_SND_PreciVolume_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstPreciVol->enSound );
            Ret = AO_SND_SetPreciVol(pstPreciVol->enSound, pstPreciVol->enOutPort, pstPreciVol->stPreciGain);
            break;
        }

        case CMD_AO_SND_GETPRECIVOL:
        {
            AO_SND_PreciVolume_Param_S_PTR pstPreciVol = (AO_SND_PreciVolume_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstPreciVol->enSound );
            Ret = AO_SND_GetPreciVol(pstPreciVol->enSound, pstPreciVol->enOutPort, &pstPreciVol->stPreciGain);
            break;
        }

        case CMD_AO_SND_SETBALANCE:
        {
            AO_SND_Balance_Param_S_PTR pstBalance = (AO_SND_Balance_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstBalance->enSound );
            Ret = AO_SND_SetBalance(pstBalance->enSound, pstBalance->enOutPort, pstBalance->s32Balance);
            break;
        }

        case CMD_AO_SND_GETBALANCE:
        {
            AO_SND_Balance_Param_S_PTR pstBalance = (AO_SND_Balance_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstBalance->enSound );
            Ret = AO_SND_GetBalance(pstBalance->enSound, pstBalance->enOutPort, &pstBalance->s32Balance);
            break;
        }

        case CMD_AO_SND_SETARCENABLE:
        {
            AO_SND_ARC_ENABLE_Param_S_PTR pstArcEnable = (AO_SND_ARC_ENABLE_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstArcEnable->enSound );
            Ret = AO_SND_SetArcEnable(pstArcEnable->enSound, pstArcEnable->enOutPort, pstArcEnable->bEnable);
            break;
        }

        case CMD_AO_SND_GETARCENABLE:
        {
            AO_SND_ARC_ENABLE_Param_S_PTR pstArcEnable = (AO_SND_ARC_ENABLE_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstArcEnable->enSound );
            Ret = AO_SND_GetArcEnable(pstArcEnable->enSound, pstArcEnable->enOutPort, &pstArcEnable->bEnable);
            break;
        }

        case CMD_AO_SND_SETARCMODE:
        {
            AO_SND_ARC_MODE_Param_S_PTR pstArcMode = (AO_SND_ARC_MODE_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstArcMode->enSound );
            Ret = AO_SND_SetArcMode(pstArcMode->enSound, pstArcMode->enOutPort, pstArcMode->enMode);
            break;
        }

        case CMD_AO_SND_GETARCMODE:
        {
            AO_SND_ARC_MODE_Param_S_PTR pstArcMode = (AO_SND_ARC_MODE_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstArcMode->enSound );
            Ret = AO_SND_GetArcMode(pstArcMode->enSound, pstArcMode->enOutPort, &pstArcMode->enMode);
            break;
        }

        case CMD_AO_SND_SETARCCAP:
        {
            AO_SND_ARC_CAP_Param_S_PTR pstArcCap = (AO_SND_ARC_CAP_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstArcCap->enSound );
            Ret = AO_SND_SetArcCap(pstArcCap->enSound, pstArcCap->enOutPort, pstArcCap->stCap);
            break;
        }

        case CMD_AO_SND_GETARCCAP:
        {
            AO_SND_ARC_CAP_Param_S_PTR pstArcCap = (AO_SND_ARC_CAP_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstArcCap->enSound );
            Ret = AO_SND_GetArcCap(pstArcCap->enSound, pstArcCap->enOutPort, &pstArcCap->stCap);
            break;
        }

        case CMD_AO_SND_SETAVCATTR:
        {
            AO_SND_Avc_Param_S_PTR pstAvcAttr = (AO_SND_Avc_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstAvcAttr->enSound );
            Ret = AO_SND_SetAvcAttr(pstAvcAttr->enSound, &pstAvcAttr->stAvcAttr);
            break;
        }

        case CMD_AO_SND_GETAVCATTR:
        {
            AO_SND_Avc_Param_S_PTR pstAvcAttr = (AO_SND_Avc_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstAvcAttr->enSound );
            Ret = AO_SND_GetAvcAttr(pstAvcAttr->enSound, &pstAvcAttr->stAvcAttr);
            break;
        }

        case CMD_AO_SND_SETAVCENABLE:
        {
            AO_SND_Avc_Enable_S_PTR pstAvcEanble = (AO_SND_Avc_Enable_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstAvcEanble->enSound );
            Ret = AO_SND_SetAvcEnable(pstAvcEanble->enSound, pstAvcEanble->bAvcEnable);
            break;
        }

        case CMD_AO_SND_GETAVCENABLE:
        {
            AO_SND_Avc_Enable_S_PTR pstAvcEanble = (AO_SND_Avc_Enable_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstAvcEanble->enSound );
            Ret = AO_SND_GetAvcEnable(pstAvcEanble->enSound, &pstAvcEanble->bAvcEnable);
            break;
        }

        case CMD_AO_SND_SETGEQATTR:
        {
            AO_SND_Geq_Param_S_PTR pstGeqAttr = (AO_SND_Geq_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstGeqAttr->enSound );
            Ret = AO_SND_SetGeqAttr(pstGeqAttr->enSound, &pstGeqAttr->stEqAttr);
            break;
        }

        case CMD_AO_SND_GETGEQATTR:
        {
            AO_SND_Geq_Param_S_PTR pstGeqAttr = (AO_SND_Geq_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstGeqAttr->enSound );
            Ret = AO_SND_GetGeqAttr(pstGeqAttr->enSound, &pstGeqAttr->stEqAttr);
            break;
        }

        case CMD_AO_SND_SETGEQENABLE:
        {
            AO_SND_Eq_Enable_S_PTR pstGeqEanble = (AO_SND_Eq_Enable_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstGeqEanble->enSound );
            Ret = AO_SND_SetGeqEnable(pstGeqEanble->enSound, pstGeqEanble->bEqEnable);
            break;
        }

        case CMD_AO_SND_GETGEQENABLE:
        {
            AO_SND_Eq_Enable_S_PTR pstGeqEanble = (AO_SND_Eq_Enable_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstGeqEanble->enSound );
            Ret = AO_SND_GetGeqEnable(pstGeqEanble->enSound, &pstGeqEanble->bEqEnable);
            break;
        }

        case CMD_AO_SND_SETGEQGAIN:
        {
            AO_SND_Geq_Gain_S_PTR pstGeqGain = (AO_SND_Geq_Gain_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstGeqGain->enSound );
            Ret = AO_SND_SetGeqGain(pstGeqGain->enSound, pstGeqGain->u32Band, pstGeqGain->s32Gain);
            break;
        }

        case CMD_AO_SND_GETGEQGAIN:
        {
            AO_SND_Geq_Gain_S_PTR pstGeqGain = (AO_SND_Geq_Gain_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstGeqGain->enSound );
            Ret = AO_SND_GetGeqGain(pstGeqGain->enSound, pstGeqGain->u32Band, &pstGeqGain->s32Gain);
            break;
        }

        case CMD_AO_SND_SETDRCENABLE:
        {
            AO_SND_Drc_Enable_S_PTR pstDrcEanble = (AO_SND_Drc_Enable_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstDrcEanble->enSound );
            Ret = AO_SND_SetDrcEnable(pstDrcEanble->enSound, pstDrcEanble->enOutPort, pstDrcEanble->bDrcEnable);
            break;
        }

        case CMD_AO_SND_GETDRCENABLE:
        {
            AO_SND_Drc_Enable_S_PTR pstDrcEanble = (AO_SND_Drc_Enable_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstDrcEanble->enSound );
            Ret = AO_SND_GetDrcEnable(pstDrcEanble->enSound, pstDrcEanble->enOutPort, &pstDrcEanble->bDrcEnable);
            break;
        }

        case CMD_AO_SND_SETDRCATTR:
        {
            AO_SND_Drc_Param_S_PTR pstDrcAttr = (AO_SND_Drc_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstDrcAttr->enSound );
            Ret = AO_SND_SetDrcAttr(pstDrcAttr->enSound, pstDrcAttr->enOutPort, &pstDrcAttr->stDrcAttr);
            break;
        }

        case CMD_AO_SND_GETDRCATTR:
        {
            AO_SND_Drc_Param_S_PTR pstDrcAttr = (AO_SND_Drc_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstDrcAttr->enSound );
            Ret = AO_SND_GetDrcAttr(pstDrcAttr->enSound, pstDrcAttr->enOutPort, &pstDrcAttr->stDrcAttr);
            break;
        }

        case CMD_AO_SND_SETPEQATTR:
        {
            AO_SND_Peq_Param_S_PTR pstPeqAttr = (AO_SND_Peq_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstPeqAttr->enSound );
            Ret = AO_SND_SetPeqAttr(pstPeqAttr->enSound, pstPeqAttr->enOutPort, &pstPeqAttr->stEqAttr);
            break;
        }

        case CMD_AO_SND_GETPEQATTR:
        {
            AO_SND_Peq_Param_S_PTR pstPeqAttr = (AO_SND_Peq_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstPeqAttr->enSound );

            Ret = AO_SND_GetPeqAttr(pstPeqAttr->enSound, pstPeqAttr->enOutPort, &pstPeqAttr->stEqAttr);
            break;
        }

        case CMD_AO_SND_SETPEQENABLE:
        {
            AO_SND_Eq_Enable_S_PTR pstPeqEanble = (AO_SND_Eq_Enable_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstPeqEanble->enSound );
            Ret = AO_SND_SetPeqEnable(pstPeqEanble->enSound, pstPeqEanble->enOutPort, pstPeqEanble->bEqEnable);
            break;
        }

        case CMD_AO_SND_GETPEQENABLE:
        {
            AO_SND_Eq_Enable_S_PTR pstPeqEanble = (AO_SND_Eq_Enable_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstPeqEanble->enSound );
            Ret = AO_SND_GetPeqEnable(pstPeqEanble->enSound, pstPeqEanble->enOutPort, &pstPeqEanble->bEqEnable);
            break;
        }

        case CMD_AO_SND_ATTACHAEF:
        {
            AO_SND_AttAef_Param_S_PTR pstSndAttAef = (AO_SND_AttAef_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstSndAttAef->enSound);
            Ret = AO_SND_AttachAef(pstSndAttAef->enSound, pstSndAttAef->u32AefId, &pstSndAttAef->stAefAttr, &pstSndAttAef->u32AefProcAddr);
            break;
        }

        case CMD_AO_SND_DETACHAEF:
        {
            AO_SND_AttAef_Param_S_PTR pstSndAttAef = (AO_SND_AttAef_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstSndAttAef->enSound);
            Ret = AO_SND_DetachAef(pstSndAttAef->enSound, pstSndAttAef->u32AefId, &pstSndAttAef->stAefAttr);
            break;
        }

        case CMD_AO_SND_SETAEFBYPASS:
        {
            AO_SND_AefBypass_Param_S_PTR pstAefBypass = (AO_SND_AefBypass_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstAefBypass->enSound);
            Ret = AO_SND_SetAefBypass(pstAefBypass->enSound, pstAefBypass->enOutPort, pstAefBypass->bBypass);
            break;
        }

        case CMD_AO_SND_GETAEFBYPASS:
        {
            AO_SND_AefBypass_Param_S_PTR pstAefBypass = (AO_SND_AefBypass_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstAefBypass->enSound);
            Ret = AO_SND_GetAefBypass(pstAefBypass->enSound, pstAefBypass->enOutPort, &pstAefBypass->bBypass);
            break;
        }

        case CMD_AO_SND_GETTRACKINFO:
        {
            AO_SND_TrackInfo_Param_S_PTR pstTrackInfo = (AO_SND_TrackInfo_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstTrackInfo->enSound);
            Ret = AO_SND_GetTrackInfo(pstTrackInfo->enSound, &pstTrackInfo->stTrackInfo);
            break;
        }
#endif

        case CMD_AO_SND_GETXRUNCOUNT:
        {
            AO_SND_Get_Xrun_Param_S_PTR pstXrunStatus = (AO_SND_Get_Xrun_Param_S_PTR)arg;

            CHECK_AO_SNDCARD_OPEN(pstXrunStatus->enSound);
            Ret = AO_Snd_GetXRunCount(pstXrunStatus->enSound, &pstXrunStatus->u32Count);

            break;
        }

        case CMD_AO_SND_SETADACENABLE:
        {
            AO_SND_Adac_Enable_Param_S_PTR pstAdacEnable = (AO_SND_Adac_Enable_Param_S_PTR)arg;

            CHECK_AO_SNDCARD_OPEN(pstAdacEnable->enSound);
            Ret = AO_Snd_SetAdacEnable(pstAdacEnable->enSound, pstAdacEnable->bEnable);

            break;
        }

        case CMD_AO_SND_GETADACENABLE:
        {
            AO_SND_Adac_Enable_Param_S_PTR pstAdacEnable = (AO_SND_Adac_Enable_Param_S_PTR)arg;

            CHECK_AO_SNDCARD_OPEN(pstAdacEnable->enSound);
            Ret = AO_Snd_GetAdacEnable(pstAdacEnable->enSound, &pstAdacEnable->bEnable);

            break;
        }

#ifdef __DPT__
        case CMD_AO_SND_GETAEFBUFATTR:
        {
            AO_SND_Aef_Buf_Param_S_PTR pstAefBuf = (AO_SND_Aef_Buf_Param_S_PTR)arg;

            CHECK_AO_SNDCARD_OPEN(pstAefBuf->enSound);
            Ret = AO_Snd_GetAefBufAttr(pstAefBuf->enSound, &pstAefBuf->stAefBuf);

            break;
        }

        case CMD_AO_SND_GETDEBUGPARAM:
        {
            AO_SND_DEBUG_Param_S_PTR pstDebug = (AO_SND_DEBUG_Param_S_PTR)arg;

            CHECK_AO_SNDCARD_OPEN(pstDebug->enSound);
            Ret = AO_Snd_GetDebugAttr(pstDebug->enSound, &pstDebug->stDebugAttr);
            break;
        }

        case CMD_AO_SND_SETADOUTPUTENABLE:
        {
            AO_SND_ADOutput_Enable_S_PTR pstADOutputEanble = (AO_SND_ADOutput_Enable_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstADOutputEanble->enSound );
            Ret = AO_SND_SetADOutputEnable(pstADOutputEanble->enSound, pstADOutputEanble->enOutPort, pstADOutputEanble->bADOutputEnable);
            break;
        }

        case CMD_AO_SND_GETADOUTPUTENABLE:
        {
            AO_SND_ADOutput_Enable_S_PTR pstADOutputEanble = (AO_SND_ADOutput_Enable_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstADOutputEanble->enSound );
            Ret = AO_SND_GetADOutputEnable(pstADOutputEanble->enSound, pstADOutputEanble->enOutPort, &pstADOutputEanble->bADOutputEnable);
            break;
        }

        case CMD_AO_SND_DUPLICATETRACK:
        {
            AO_SND_TrackDuplicate_Param_S_PTR pstDuplicateTrack = (AO_SND_TrackDuplicate_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN( pstDuplicateTrack->enSound );
            Ret = AO_SND_TrackDuplicate(pstDuplicateTrack->enSound, pstDuplicateTrack->enOutPort,
                                        pstDuplicateTrack->hTrack, pstDuplicateTrack->bEnable);
            break;
        }

        case CMD_AO_SND_SETALLCASTMUTE:
        {
            AO_SND_AllCastMute_Param_S_PTR pstAllCastMute = (AO_SND_AllCastMute_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstAllCastMute->enSound);
            Ret = AO_SND_SetAllCastMute(pstAllCastMute->enSound, pstAllCastMute->bMute);
            break;
        }
        case CMD_AO_SND_GETALLCASTMUTE:
        {
            AO_SND_AllCastMute_Param_S_PTR pstAllCastMute = (AO_SND_AllCastMute_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstAllCastMute->enSound);
            Ret = AO_SND_GetAllCastMute(pstAllCastMute->enSound, &pstAllCastMute->bMute);
            break;
        }
#endif

        case CMD_AO_CAST_GETDEFATTR:
        {
            HI_UNF_SND_CAST_ATTR_S* pstDefAttr = (HI_UNF_SND_CAST_ATTR_S*)arg;
            Ret = AO_Cast_GetDefAttr(pstDefAttr);
            break;
        }

        case CMD_AO_CAST_CREATE:
        {
            AO_Cast_Create_Param_S_PTR  pstCastAttr = (AO_Cast_Create_Param_S_PTR)arg;

            //HI_ERR_AO("CMD_AO_CAST_CREATE\n");
            CHECK_AO_SNDCARD_OPEN( pstCastAttr->enSound);

            if (HI_SUCCESS == AO_Cast_AllocHandle(&hHandle, file, &pstCastAttr->stCastAttr))
            {
                Ret = AO_Cast_Create(pstCastAttr->enSound, &pstCastAttr->stCastAttr, &s_stAoDrv.astCastEntity[hHandle
                                     & AO_CAST_CHNID_MASK].stRbfMmz,
                                     hHandle);
                if (HI_SUCCESS != Ret)
                {
                    AO_Cast_FreeHandle(hHandle);
                    break;
                }

                AO_Cast_SaveSuspendAttr(pstCastAttr->enSound, hHandle, &pstCastAttr->stCastAttr);
                pstCastAttr->u32ReqSize = s_stAoDrv.astCastEntity[hHandle & AO_CAST_CHNID_MASK].u32ReqSize;
                pstCastAttr->hCast = hHandle;
            }
            break;
        }

        case CMD_AO_CAST_DESTROY:
        {
            HI_HANDLE  hCast = *(HI_HANDLE*)arg;
            //HI_ERR_AO("CMD_AO_CAST_DESTORY\n");

            CHECK_AO_CAST_OPEN(hCast);
            Ret = AO_Cast_Destory(hCast);
            if (HI_SUCCESS != Ret)
            {
                break;
            }
            AO_Cast_FreeHandle(hCast);
            break;
        }

        case CMD_AO_CAST_SETINFO:
        {
            AO_Cast_Info_Param_S_PTR pstInfo = (AO_Cast_Info_Param_S_PTR)arg;
            CHECK_AO_CAST_OPEN(pstInfo->hCast);

            //HI_ERR_AO("CMD_AO_CAST_SETINFO  CastID=0x%x, pu8UserVirtAddr=0x%p\n",pstInfo->hCast, pstInfo->pu8UserVirtAddr);
            Ret = AO_Cast_SetInfo(pstInfo->hCast, pstInfo->tUserVirtAddr);
            break;
        }

        case CMD_AO_CAST_GETINFO:
        {
            AO_Cast_Info_Param_S_PTR pstInfo = (AO_Cast_Info_Param_S_PTR)arg;
            //HI_ERR_AO("pstInfo->hCast=0x%x \n", pstInfo->hCast);
            CHECK_AO_CAST_OPEN(pstInfo->hCast);
            Ret = AO_Cast_GetInfo(pstInfo->hCast, pstInfo);
            //HI_ERR_AO("CMD_AO_CAST_GETINFO  CastID=0x%x, pu8UserVirtAddr=0x%p Ret=0x%x\n",pstInfo->hCast, pstInfo->pu8UserVirtAddr, Ret);
            break;
        }

        case CMD_AO_CAST_SETENABLE:
        {
            AO_Cast_Enable_Param_S_PTR pstEnable = (AO_Cast_Enable_Param_S_PTR)arg;
            CHECK_AO_CAST_OPEN(pstEnable->hCast);
            //HI_ERR_AO("CMD_AO_CAST_SETENABLE  CastID=0x%x, bCastEnable=0x%x\n",pstEnable->hCast, pstEnable->bCastEnable);
            Ret = AO_Cast_SetEnable(pstEnable->hCast, pstEnable->bCastEnable);
            break;
        }

        case CMD_AO_CAST_GETENABLE:
        {
            AO_Cast_Enable_Param_S_PTR pstEnable = (AO_Cast_Enable_Param_S_PTR)arg;
            CHECK_AO_CAST_OPEN(pstEnable->hCast);
            //HI_ERR_AO("CMD_AO_CAST_GETENABLE CastID=0x%x, bCastEnable=0x%x\n",pstEnable->hCast, pstEnable->bCastEnable);
            Ret = AO_Cast_GetEnable(pstEnable->hCast, &pstEnable->bCastEnable);
            break;
        }

        case CMD_AO_CAST_ACQUIREFRAME:
        {
            AO_Cast_Data_Param_S_PTR pstCastData = (AO_Cast_Data_Param_S_PTR)arg;
            CHECK_AO_CAST_OPEN(pstCastData->hCast);

            Ret = AO_Cast_ReadData(pstCastData->hCast, pstCastData);
            break;
        }

        case CMD_AO_CAST_RELEASEFRAME:
        {
            AO_Cast_Data_Param_S_PTR pstCastData = (AO_Cast_Data_Param_S_PTR)arg;
            CHECK_AO_CAST_OPEN(pstCastData->hCast);
            //HI_ERR_AO("CMD_AO_CAST_RELEASEFRAME  CastID=0x%x\n",pstCastData->hCast);
            Ret = AO_Cast_ReleseData(pstCastData->hCast, pstCastData);

            break;
        }

        case CMD_AO_CAST_SETABSGAIN:
        {
            AO_Cast_AbsGain_Param_S_PTR pstAbsGain = (AO_Cast_AbsGain_Param_S_PTR)arg;
            CHECK_AO_CAST_OPEN(pstAbsGain->hCast);
            Ret = AO_Cast_SetAbsGain(pstAbsGain->hCast, pstAbsGain->stCastAbsGain);
            break;
        }

        case CMD_AO_CAST_GETABSGAIN:
        {
            AO_Cast_AbsGain_Param_S_PTR pstAbsGain = (AO_Cast_AbsGain_Param_S_PTR)arg;
            CHECK_AO_CAST_OPEN(pstAbsGain->hCast);
            Ret = AO_Cast_GetAbsGain(pstAbsGain->hCast, &pstAbsGain->stCastAbsGain);
            break;
        }

        case CMD_AO_CAST_SETMUTE:
        {
            AO_Cast_Mute_Param_S_PTR pstMute = (AO_Cast_Mute_Param_S_PTR)arg;
            CHECK_AO_CAST_OPEN(pstMute->hCast);
            Ret = AO_Cast_SetMute(pstMute->hCast, pstMute->bMute);
            break;
        }

        case CMD_AO_CAST_GETMUTE:
        {
            AO_Cast_Mute_Param_S_PTR pstMute = (AO_Cast_Mute_Param_S_PTR)arg;
            CHECK_AO_CAST_OPEN(pstMute->hCast);
            Ret = AO_Cast_GetMute(pstMute->hCast, &pstMute->bMute);
            break;
        }

        case CMD_AO_SND_ATTACHTRACK:
            break;
        case CMD_AO_SND_DETACHTRACK:
            break;
#ifdef __DPT__
        case CMD_AO_SND_TRACKCONFIGINIT:
        {
            AO_SND_TrackConfig_Param_S_PTR pstTrackConfigParam = (AO_SND_TrackConfig_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstTrackConfigParam->enSound);
            Ret = AO_SND_TrackConfigInit(pstTrackConfigParam->enSound, &pstTrackConfigParam->stTrackConfig);
            break;
        }
#endif
        //Track CMD TYPE (call hal_aoe)
        case CMD_AO_TRACK_GETDEFATTR:
        {
            HI_UNF_AUDIOTRACK_ATTR_S* pstDefAttr = (HI_UNF_AUDIOTRACK_ATTR_S*)arg;
            Ret = AO_Track_GetDefAttr(pstDefAttr);
            break;
        }

        case CMD_AO_TRACK_SETATTR:
        {
            AO_Track_Attr_Param_S_PTR pstTrackAttr = (AO_Track_Attr_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstTrackAttr->hTrack);
            Ret = AO_Track_PreSetAttr(pstTrackAttr->hTrack, &pstTrackAttr->stAttr);
            break;
        }

        case CMD_AO_TRACK_GETATTR:
        {
            AO_Track_Attr_Param_S_PTR pstTrackAttr = (AO_Track_Attr_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstTrackAttr->hTrack);
            Ret = AO_Track_GetAttr(pstTrackAttr->hTrack, &pstTrackAttr->stAttr);
            break;
        }

        case CMD_AO_TRACK_MMAP:
        {
            AO_Track_Mmap_Param_S_PTR pstParam = (AO_Track_Mmap_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstParam->hTrack);
            Ret = AO_Track_Mmap(pstParam->hTrack, pstParam);
            break;
        }

        case CMD_AO_TRACK_CREATE:
        {
            AO_Track_Create_Param_S_PTR pstTrack = (AO_Track_Create_Param_S_PTR)arg;

            CHECK_AO_SNDCARD_OPEN(pstTrack->enSound);
            Ret = AO_Track_AllocHandle(pstTrack->enSound, pstTrack->stAttr.enTrackType, file, &hHandle);
            if (HI_SUCCESS == Ret)
            {
                Ret = AO_Track_PreCreate(pstTrack->enSound, &pstTrack->stAttr, pstTrack->bAlsaTrack,
                                         &pstTrack->stBuf, hHandle);
                if (HI_SUCCESS != Ret)
                {
                    AO_Track_FreeHandle(hHandle);
                    break;
                }
#ifdef __DPT__
                AO_SND_UpdateTrackInfo(hHandle, HI_TRUE);
#endif
                AO_TRACK_SaveSuspendAttr(hHandle, pstTrack);
                pstTrack->hTrack = hHandle;
            }

            break;
        }

        case CMD_AO_TRACK_DESTROY:
        {
            HI_HANDLE hTrack = *(HI_HANDLE*)arg;
            CHECK_AO_TRACK_OPEN(hTrack);
#ifdef __DPT__
            AO_SND_UpdateTrackInfo(hTrack, HI_FALSE);
#endif
            Ret = AO_Track_Destory(hTrack);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("AO_Track_Destory failed Ret %x\n", Ret);
                break;
            }

            AO_Track_FreeHandle(hTrack);

            break;
        }

        case CMD_AO_TRACK_START:
        {
            HI_HANDLE hTrack = *(HI_HANDLE*)arg;
            CHECK_AO_TRACK_OPEN(hTrack);
            Ret = AO_Track_Start(hTrack);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AO("AO_Track_Start failed %x\n", Ret);
            }
            break;
        }

        case CMD_AO_TRACK_STOP:
        {
            HI_HANDLE hTrack = *(HI_HANDLE*)arg;
            CHECK_AO_TRACK_OPEN(hTrack);
            Ret = AO_Track_Stop(hTrack);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_AO("AO_Track_Stop failed %x\n", Ret);
            }
            break;
        }

        case CMD_AO_TRACK_PAUSE:
        {
            HI_HANDLE hTrack = *(HI_HANDLE*)arg;
            CHECK_AO_TRACK_OPEN(hTrack);
            Ret = AO_Track_Pause(hTrack);
            break;
        }

        case CMD_AO_TRACK_FLUSH:
        {
            HI_HANDLE hTrack = *(HI_HANDLE*)arg;
            CHECK_AO_TRACK_OPEN(hTrack);
            Ret = AO_Track_Flush(hTrack);
            break;
        }

        case CMD_AO_TRACK_SENDDATA:
        {
            AO_Track_SendData_Param_S_PTR pstData = (AO_Track_SendData_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstData->hTrack);
            Ret = AO_Track_SendData(pstData->hTrack, &pstData->stAOFrame);
            if (HI_FAILURE == Ret)
            {
                HI_ERR_AO("AO_Track_SendData failed %x\n", Ret);
            }

            break;
        }

        case CMD_AO_TRACK_SETWEITHT:
        {
            AO_Track_Weight_Param_S_PTR pstWeight = (AO_Track_Weight_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstWeight->hTrack);
            Ret = AO_Track_SetWeight(pstWeight->hTrack, &pstWeight->stTrackGain);
            break;
        }

        case CMD_AO_TRACK_GETWEITHT:
        {
            AO_Track_Weight_Param_S_PTR pstWeight = (AO_Track_Weight_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstWeight->hTrack);
            Ret = AO_Track_GetWeight(pstWeight->hTrack, &pstWeight->stTrackGain);
            break;
        }

        case CMD_AO_TRACK_SETABSGAIN:
        {
            AO_Track_AbsGain_Param_S_PTR pstAbsGain = (AO_Track_AbsGain_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstAbsGain->hTrack);
            Ret = AO_Track_SetAbsGain(pstAbsGain->hTrack, &pstAbsGain->stTrackAbsGain);
            break;
        }

        case CMD_AO_TRACK_GETABSGAIN:
        {
            AO_Track_AbsGain_Param_S_PTR pstAbsGain = (AO_Track_AbsGain_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstAbsGain->hTrack);
            Ret = AO_Track_GetAbsGain(pstAbsGain->hTrack, &pstAbsGain->stTrackAbsGain);
            break;
        }

#ifdef __DPT__
        case CMD_AO_TRACK_SETPRESCALE:
        {
            AO_Track_Prescale_Param_S_PTR pstPrescale = (AO_Track_Prescale_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstPrescale->hTrack);
            Ret = AO_Track_SetPrescale(pstPrescale->hTrack, &pstPrescale->stPreciGain);
            break;
        }

        case CMD_AO_TRACK_GETPRESCALE:
        {
            AO_Track_Prescale_Param_S_PTR pstPrescale = (AO_Track_Prescale_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstPrescale->hTrack);
            Ret = AO_Track_GetPrescale(pstPrescale->hTrack, &pstPrescale->stPreciGain);
            break;
        }
#endif

        case CMD_AO_TRACK_SETMUTE:
        {
            AO_Track_Mute_Param_S_PTR pstMute = (AO_Track_Mute_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstMute->hTrack);
            Ret = AO_Track_SetMute(pstMute->hTrack, pstMute->bMute);
            break;
        }

        case CMD_AO_TRACK_GETMUTE:
        {
            AO_Track_Mute_Param_S_PTR pstMute = (AO_Track_Mute_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstMute->hTrack);
            Ret = AO_Track_GetMute(pstMute->hTrack, &pstMute->bMute);
            break;
        }

        case CMD_AO_TRACK_SETCHANNELMODE:
        {
            AO_Track_ChannelMode_Param_S_PTR pstChannelMode = (AO_Track_ChannelMode_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstChannelMode->hTrack);
            Ret = AO_Track_SetChannelMode(pstChannelMode->hTrack, pstChannelMode->enMode);
            break;
        }

        case CMD_AO_TRACK_GETCHANNELMODE:
        {
            AO_Track_ChannelMode_Param_S_PTR pstChannelMode = (AO_Track_ChannelMode_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstChannelMode->hTrack);
            Ret = AO_Track_GetChannelMode(pstChannelMode->hTrack, &pstChannelMode->enMode);
            break;
        }

        case CMD_AO_TRACK_SETFIFOBYPASS:
        {
            AO_Track_FifoBypass_Param_S_PTR pstFifoBypass = (AO_Track_FifoBypass_Param_S_PTR)arg;
            HI_HANDLE hTrack = pstFifoBypass->hTrack;
            CHECK_AO_TRACK_OPEN(hTrack);
            CHECK_AO_TRACK_OPEN(pstFifoBypass->hTrack);
            Ret = AO_TrackSetAipFiFoBypass(hTrack, pstFifoBypass->bEnable);
            break;
        }

        case CMD_AO_TRACK_SETPRIORITY:
        {
            AO_Track_Priority_Param_S_PTR pstTrackPriority = (AO_Track_Priority_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstTrackPriority->hTrack);
            Ret = AO_Track_SePriority(pstTrackPriority->hTrack, pstTrackPriority->bEnable);
            break;
        }

        case CMD_AO_TRACK_GETPRIORITY:
        {
            AO_Track_Priority_Param_S_PTR pstTrackPriority = (AO_Track_Priority_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstTrackPriority->hTrack);
            Ret = AO_Track_GePriority(pstTrackPriority->hTrack, &pstTrackPriority->bEnable);
            break;
        }

        case CMD_AO_TRACK_SETSPEEDADJUST:
        {
            AO_Track_SpeedAdjust_Param_S_PTR pstSpeed = (AO_Track_SpeedAdjust_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstSpeed->hTrack);
            Ret = AO_Track_SetSpeedAdjust(pstSpeed->hTrack, pstSpeed->enType, pstSpeed->s32Speed);
            break;
        }

        case CMD_AO_TRACK_GETDELAYMS:
        {
            HI_S32 s32Ret;
            HI_U32 u32TrackDelayMs = 0;
            HI_U32 u32ExtDelayMs = 0;
            AO_Track_DelayMs_Param_S_PTR pstDelayMs = (AO_Track_DelayMs_Param_S_PTR)arg;

            CHECK_AO_TRACK_OPEN(pstDelayMs->hTrack);

            s32Ret = AO_Track_GetDelayMs(pstDelayMs->hTrack, &u32TrackDelayMs);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_AO("AO_Track_GetDelayMs failed(0x%x)\n", s32Ret);
                break;
            }

            s32Ret = AO_Track_GetExtDelayMs(pstDelayMs->hTrack, &u32ExtDelayMs);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_AO("AO_Track_GetExtDelayMs failed(0x%x)\n", s32Ret);
                break;
            }
            pstDelayMs->u32DelayMs = u32TrackDelayMs + u32ExtDelayMs;

            break;
        }

        case CMD_AO_TRACK_ISBUFEMPTY:
        {
            AO_Track_BufEmpty_Param_S_PTR pstBufEmpty = (AO_Track_BufEmpty_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstBufEmpty->hTrack);
            Ret = AO_Track_IsBufEmpty(pstBufEmpty->hTrack, &pstBufEmpty->bEmpty);
            break;
        }

        case CMD_AO_TRACK_SETEOSFLAG:
        {
            AO_Track_EosFlag_Param_S_PTR pstEosFlag = (AO_Track_EosFlag_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstEosFlag->hTrack);
            Ret = AO_Track_SetEosFlag(pstEosFlag->hTrack, pstEosFlag->bEosFlag);
            break;
        }

        case CMD_AO_TRACK_ATTACHAI:
        {
            AO_Track_AttAi_Param_S_PTR pstTrackAttAi = (AO_Track_AttAi_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstTrackAttAi->hTrack);
            Ret = AO_Track_AttachAi(pstTrackAttAi->hTrack, pstTrackAttAi->hAi);
            break;
        }

        case CMD_AO_TRACK_DETACHAI:
        {
            AO_Track_AttAi_Param_S_PTR pstTrackDetAi = (AO_Track_AttAi_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstTrackDetAi->hTrack);
            Ret = AO_Track_DetachAi(pstTrackDetAi->hTrack, pstTrackDetAi->hAi);
            break;
        }
#ifdef __DPT__
        case CMD_AO_TRACK_SETSOURCE:
        {
            AO_Track_Source_Param_S_PTR pstTrackSource = (AO_Track_Source_Param_S_PTR)arg;
            CHECK_AO_TRACK_OPEN(pstTrackSource->hTrack);
            Ret = AO_Track_SetSource(pstTrackSource->hTrack, pstTrackSource->enSource);
            break;
        }
#endif
        case CMD_AO_SND_SETEXTDELAYMS:
        {
            AO_SND_Set_ExtDelayMs_Param_S_PTR pstExtDelayMsParam = (AO_SND_Set_ExtDelayMs_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstExtDelayMsParam->enSound);
            Ret = AO_Snd_SetExtDelayMs(pstExtDelayMsParam->enSound, pstExtDelayMsParam->u32DelayMs);
            break;
        }

        case CMD_AO_SND_DMACREATE:
        {
            AO_SND_DMACreate_Param_S_PTR pstParam = (AO_SND_DMACreate_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstParam->enSound);
            Ret = AO_Snd_DMACreate(file, pstParam->enSound, pstParam->stPortKAttr);
            break;
        }

        case CMD_AO_SND_DMADESTORY:
        {
            AO_SND_DMACreate_Param_S_PTR pstParam = (AO_SND_DMACreate_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstParam->enSound);
            Ret = AO_Snd_DMADestory(file, pstParam->enSound);
            break;
        }

        case CMD_AO_SND_DMASETSAMPLERATE:
        {
            AO_SND_DMASetSampleRate_Param_S_PTR pstParam = (AO_SND_DMASetSampleRate_Param_S_PTR)arg;
            CHECK_AO_SNDCARD_OPEN(pstParam->enSound);
            Ret = AO_Snd_DMAModeSetSampleRate(pstParam->enSound, pstParam->enSampleRate);
            break;
        }

        default:
        {
            Ret = HI_FAILURE;
            HI_WARN_AO("unknown cmd: 0x%x\n", cmd);
            break;
        }
    }

    return Ret;
}

long AO_DRV_Ioctl(struct file* file, HI_U32 cmd, unsigned long arg)
{
    long s32Ret = HI_SUCCESS;

    s32Ret = down_interruptible(&g_AoMutex);

    //cmd process
    s32Ret = (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, AO_ProcessCmd);

    up(&g_AoMutex);

    return s32Ret;
}

HI_S32 AO_DRV_Open(struct inode* inode, struct file* filp)
{
    HI_S32 s32Ret;
    HI_U32 cnt;
    DRV_AO_STATE_S* pAOState = HI_NULL;

    if (!filp)
    {
        HI_FATAL_AO("file handle is null.\n");
        return HI_FAILURE;
    }

    s32Ret = down_interruptible(&g_AoMutex);

    pAOState = AUTIL_AO_MALLOC(HI_ID_AO, sizeof(DRV_AO_STATE_S), GFP_KERNEL);
    if (!pAOState)
    {
        HI_FATAL_AO("malloc pAOState failed.\n");
        up(&g_AoMutex);
        return HI_FAILURE;
    }
    for (cnt = 0; cnt < AO_MAX_TOTAL_SND_NUM; cnt++)
    {
        atomic_set(&(pAOState->atmUserOpenCnt[cnt]), 0);
        pAOState->u32FileId[cnt] = AO_SND_FILE_NOUSE_FLAG;
    }

    if (atomic_inc_return(&s_stAoDrv.atmOpenCnt) == 1)
    {
        /* Init device */
        if (HI_SUCCESS != AO_OpenDev())
        {
            HI_FATAL_AO("AO_OpenDev err!\n" );
            goto err;
        }
    }

    filp->private_data = pAOState;

    up(&g_AoMutex);
    return HI_SUCCESS;
err:
    AUTIL_AO_FREE(HI_ID_AO, pAOState);
    atomic_dec(&s_stAoDrv.atmOpenCnt);
    up(&g_AoMutex);
    return HI_FAILURE;
}

static HI_S32 AOSNDFreeCast(struct file* filp)
{
    HI_S32 s32Ret;
    HI_U32 u32CastID;

    for (u32CastID = 0; u32CastID < AO_MAX_CAST_NUM; u32CastID++)
    {
        if (s_stAoDrv.astCastEntity[u32CastID].pstFile != filp)
        {
            continue;
        }

        if (!atomic_read(&s_stAoDrv.astCastEntity[u32CastID].atmUseCnt))
        {
            continue;
        }

        s32Ret = AO_Cast_Destory(u32CastID);
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("AO_Cast_Destory failed(0x%x), u32CastID = %d!\n", s32Ret, u32CastID);
            return s32Ret;
        }

        AO_Cast_FreeHandle(u32CastID);
    }

    return HI_SUCCESS;
}

static HI_S32 AOSNDFreeTrack(struct file* filp)
{
    HI_S32 s32Ret;
    HI_U32 u32TrackID;

    for (u32TrackID = 0; u32TrackID < AO_MAX_TOTAL_TRACK_NUM; u32TrackID++)
    {
        if (s_stAoDrv.astTrackEntity[u32TrackID].pstFile != filp)
        {
            continue;
        }

        if (0 == atomic_read(&s_stAoDrv.astTrackEntity[u32TrackID].atmUseCnt))
        {
            continue;
        }

        s32Ret = AO_Track_Destory(u32TrackID);
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("AO_Track_Destory failed(0x%x), u32TrackID = %d!\n", s32Ret, u32TrackID);
            return s32Ret;
        }

        AO_Track_FreeHandleById(u32TrackID);
    }

    return HI_SUCCESS;
}

static HI_S32 AOSndClose(DRV_AO_STATE_S* pstAoState, HI_U32 u32SndID)
{
    HI_S32 s32Ret;
    HI_U32 u32UserOpenCnt = 0;

    u32UserOpenCnt = atomic_read(&pstAoState->atmUserOpenCnt[u32SndID]);
    if (atomic_sub_and_test(u32UserOpenCnt, &s_stAoDrv.astSndEntity[u32SndID].atmUseTotalCnt))
    {
        s32Ret = AO_SND_Close(u32SndID, HI_FALSE);
        if (HI_SUCCESS != s32Ret)
        {
            atomic_inc(&s_stAoDrv.astSndEntity[u32SndID].atmUseTotalCnt);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 AOSNDFree(struct file* filp)
{
    HI_S32 s32Ret;
    HI_U32 u32SndID;
    HI_U32 u32SndOpenCnt;
    DRV_AO_STATE_S* pstAoState = filp->private_data;

    for (u32SndID = 0; u32SndID < AO_MAX_TOTAL_SND_NUM; u32SndID++)
    {
        if ((!s_stAoDrv.astSndEntity[u32SndID].pCard) || (!pstAoState))
        {
            continue;
        }

        for (u32SndOpenCnt = 0; u32SndOpenCnt < SND_MAX_OPEN_NUM; u32SndOpenCnt++)
        {
            struct file* pstFileCurSndOpen = s_stAoDrv.astSndEntity[u32SndID].pstFile[u32SndOpenCnt];
            if (pstFileCurSndOpen != filp)
            {
                continue;
            }

            s32Ret = AOSndClose(pstAoState, u32SndID);
            if (HI_SUCCESS != s32Ret)
            {
                HI_FATAL_AO("AOSndClose Failed(0x%x), u32SndID = %d!\n", s32Ret, u32SndID);
                return s32Ret;
            }

            AO_Snd_FreeHandle(u32SndID, pstFileCurSndOpen);
        }

    }

    return HI_SUCCESS;
}

HI_S32 AO_DRV_Release(struct inode* inode, struct file* filp)
{
    HI_S32 s32Ret = HI_SUCCESS;
    DRV_AO_STATE_S* pstAOState = filp->private_data;

    s32Ret = down_interruptible(&g_AoMutex);

    /* Not the last close, only close the cast, track & snd which matches filp */
    if (0 != atomic_dec_return(&s_stAoDrv.atmOpenCnt))
    {
#ifdef HI_SND_CAST_SUPPORT
        s32Ret = AOSNDFreeCast(filp);
        if (HI_SUCCESS != s32Ret)
        {
            goto ERR_AO_DRV_RELEASE;
        }
#endif

        s32Ret = AOSNDFreeTrack(filp);
        if (HI_SUCCESS != s32Ret)
        {
            goto ERR_AO_DRV_RELEASE;
        }

        /* Free all snd */
        s32Ret = AOSNDFree(filp);
        if (HI_SUCCESS != s32Ret)
        {
            goto ERR_AO_DRV_RELEASE;
        }
    }
    else
    {
        AO_CloseDev();
    }

    if (HI_NULL != pstAOState)
    {
        AUTIL_AO_FREE(HI_ID_AO, pstAOState);
    }
    up(&g_AoMutex);
    return HI_SUCCESS;

ERR_AO_DRV_RELEASE:
    atomic_inc(&s_stAoDrv.atmOpenCnt);
    up(&g_AoMutex);
    return HI_FAILURE;
}

/* drv open  kernel intf */
HI_S32 AO_DRV_Kopen(struct file*  file)
{
    return AO_DRV_Open(HI_NULL, file);
}

/*drv close kernel intf */
HI_S32 AO_DRV_Krelease(struct file*  file)
{
    return AO_DRV_Release(HI_NULL, file);
}

#if defined (HI_ALSA_I2S_ONLY_SUPPORT)|| defined (HI_ALSA_HDMI_ONLY_SUPPORT)
HI_S32 AO_DRV_Krelease(struct file*  file);
HI_S32 AOSetProcStatistics(AIAO_IsrFunc* pFunc)//only for alsa use
{
    HAL_AIAO_P_SetTxI2SDfAttr(AIAO_PORT_TX0, pFunc); //pIsrFunc is the same for all ports
    return HI_SUCCESS;
}

HI_S32 AOGetProcStatistics(AIAO_IsrFunc** pFunc)//only for alsa use
{
    AIAO_PORT_USER_CFG_S pAttr;
    HAL_AIAO_P_GetTxI2SDfAttr(AIAO_PORT_TX0, &pAttr); //pIsrFunc is the same for all ports
    *pFunc = pAttr.pIsrFunc;
    return HI_SUCCESS;
}

HI_S32 AOGetEnport(HI_UNF_SND_E enSound, AIAO_PORT_ID_E* enPort, SND_OUTPUT_TYPE_E enOutType)
{
    SND_OP_STATE_S* state;
    AIAO_PORT_ID_E enAOPort;
    SND_CARD_STATE_S* pCard;

    pCard = SND_CARD_GetCard(enSound);
    if (pCard != HI_NULL)
    {
        if (enOutType == SND_OUTPUT_TYPE_HDMI)
        {
            state = (SND_OP_STATE_S*)SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_HDMI);
        }
        else
        {
            state = (SND_OP_STATE_S*)SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_I2S);
        }
    }
    else
    {
        goto _GET_ERR;
    }

    if (state != HI_NULL)
    {
        enAOPort = state->enPortID[state->ActiveId];
    }
    else
    {
        goto _GET_ERR;
    }

    *enPort = enAOPort;
    return HI_SUCCESS;

_GET_ERR:
    HI_FATAL_AO("Get Enpot Error\n");
    return HI_FAILURE;
}

HI_S32 AOGetHandel(HI_UNF_SND_E enSound, HI_VOID* *ppSndOp, SND_OUTPUT_TYPE_E enOutType)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
    if (pCard != HI_NULL)
    {
        if (enOutType == SND_OUTPUT_TYPE_HDMI) //HI_ALSA_HDMI_ONLY_SUPPORT
        {
            *ppSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_HDMI);
        }
        else
        {
            *ppSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_I2S);
        }
    }
    else
    {
        goto _GET_ERR;
    }

    if (*ppSndOp == HI_NULL)
    {
        goto _GET_ERR;
    }

    return HI_SUCCESS;

_GET_ERR:
    HI_FATAL_AO("Get AOGetHandel Error\n");
    return HI_FAILURE;
}

HI_S32 Alsa_AO_OpenDev(struct file*  file, HI_VOID* pAlsaAttr, HI_VOID* p)
{
    if (HI_SUCCESS != AO_DRV_Kopen(file))
    {
        HI_FATAL_AO("AO_DRV_Kopen err!\n" );
        goto err;
    }

    if (HI_SUCCESS != AO_Snd_Kopen((AO_SND_Open_Param_S*)p, (AO_ALSA_I2S_Param_S*)pAlsaAttr, file))
    {
        AO_DRV_Krelease(file);
        HI_FATAL_AO("AO_Snd_Kopen err\n");
        goto err;
    }
    return HI_SUCCESS;

err:
    return HI_FAILURE;
}

HI_S32 Alsa_AO_CloseDev(struct file*  file, HI_UNF_SND_E snd_idx)
{
    if (HI_SUCCESS != AO_Snd_Kclose(snd_idx, file))
    {
        HI_FATAL_AO("AO_Snd_Kclose err!\n" );
        goto err;
    }

    if (HI_SUCCESS != AO_DRV_Krelease(file))
    {
        HI_FATAL_AO("AO_DRV_Krelease err!\n" );
        goto err;
    }

    return HI_SUCCESS;

err:
    return HI_FAILURE;
}
#endif

#if (1 == HI_PROC_SUPPORT)
HI_S32 AO_DRV_RegisterProc(HI_VOID)
{
    HI_U32 i;

    /* Create proc when use, if MCE open snd , reg proc here*/
    for (i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
    {
        if (s_stAoDrv.astSndEntity[i].pCard)
        {
            AO_RegProc(i);
        }
    }

    return HI_SUCCESS;
}

HI_VOID AO_DRV_UnregisterProc(HI_VOID)
{
}
#endif

#if defined (HI_SND_DRV_SUSPEND_SUPPORT)
static HI_S32 AO_TRACK_GetSettings(HI_HANDLE hTrack, SND_TRACK_SETTINGS_S* pstSndSettings)
{
    SND_CARD_STATE_S* pCard;

    hTrack &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(hTrack);
    if (pCard)
    {
        return TRACK_GetSetting(pCard, hTrack, pstSndSettings);
    }
    else
    {
        HI_ERR_AO("Track(%d) don't attach card!\n", hTrack);
        return HI_FAILURE;
    }
}

static HI_S32 AO_TRACK_RestoreSettings(HI_HANDLE hTrack, SND_TRACK_SETTINGS_S* pstSndSettings)
{
    SND_CARD_STATE_S* pCard;

    hTrack &= AO_TRACK_CHNID_MASK;
    pCard = TRACK_CARD_GetCard(hTrack);
    if (pCard)
    {
        return TRACK_RestoreSetting(pCard, hTrack, pstSndSettings);
    }
    else
    {
        HI_ERR_AO("Track(%d) don't attach card!\n", hTrack);
        return HI_FAILURE;
    }
}

static HI_S32 AO_SND_GetSettings(HI_UNF_SND_E enSound, SND_CARD_SETTINGS_S* pstSndSettings)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_NULL_PTR(pstSndSettings);

    pstSndSettings->bAllTrackMute = pCard->bAllTrackMute;
#ifdef __DPT__
    pstSndSettings->bAllCastMute = pCard->bAllCastMute;

    memcpy(&pstSndSettings->stTrackConfig, &pCard->stTrackConfig, sizeof(HI_UNF_SND_TRACK_CONFIG_S));
#endif
    if (HI_SUCCESS != SND_GetOpSetting(pCard, pstSndSettings))
    {
        return HI_FAILURE;
    }

#ifdef __DPT__
    if (HI_SUCCESS != AEF_GetSetting(pCard, pstSndSettings))
    {
        return HI_FAILURE;
    }
#endif

    return HI_SUCCESS;
}

static HI_S32 AO_SND_RestoreSettings(HI_UNF_SND_E enSound, SND_CARD_SETTINGS_S* pstSndSettings)
{
    SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);

    CHECK_AO_NULL_PTR(pCard);
    CHECK_AO_NULL_PTR(pstSndSettings);
    pCard->bAllTrackMute = pstSndSettings->bAllTrackMute; //All track mute restore in Track restore.
#ifdef __DPT__
    pCard->bAllCastMute = pstSndSettings->bAllCastMute; //All track mute restore in Track restore.
    memcpy(&pCard->stTrackConfig, &pstSndSettings->stTrackConfig, sizeof(HI_UNF_SND_TRACK_CONFIG_S));
#endif
    if (HI_SUCCESS != SND_RestoreOpSetting(pCard, pstSndSettings))
    {
        return HI_FAILURE;
    }

#ifdef __DPT__
    if (HI_SUCCESS != AEF_RestoreSetting(pCard, pstSndSettings))
    {
        return HI_FAILURE;
    }
#endif

    return HI_SUCCESS;
}
#endif

HI_S32 HI_DRV_AO_Init(HI_VOID)
{
    return AO_DRV_Init();
}

HI_VOID HI_DRV_AO_DeInit(HI_VOID)
{
    AO_DRV_Exit();
}

HI_S32 HI_DRV_AO_SND_Init(struct file* pfile)
{
    struct file* pstfilp;

    if (HI_NULL == pfile)
    {
        pstfilp = &g_filp;
    }
    else
    {
        pstfilp = pfile;
    }

    HI_INFO_AO("HI_DRV_AO_SND_Init file = %p\n", pfile);

    return AO_DRV_Open(HI_NULL, pstfilp);
}

HI_S32 HI_DRV_AO_SND_DeInit(struct file* pfile)
{
    struct file* pstfilp;

    if (HI_NULL == pfile)
    {
        pstfilp = &g_filp;
    }
    else
    {
        pstfilp = pfile;
    }
    HI_INFO_AO("HI_DRV_AO_SND_DeInit file = %p\n", pfile);

    return AO_DRV_Release(HI_NULL, pstfilp);
}

HI_S32 HI_DRV_AO_SND_GetDefaultOpenAttr(HI_UNF_SND_E enSound, DRV_SND_ATTR_S* pstAttr)
{
    HI_S32 Ret;
    AO_SND_OpenDefault_Param_S stSndDefaultAttr;
    CHECK_AO_SNDCARD(enSound);

    Ret = down_interruptible(&g_AoMutex);

    stSndDefaultAttr.enSound = enSound;
    Ret = AOGetSndDefOpenAttr(&stSndDefaultAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("AOGetSndDefOpenAttr Failed!\n");
        up(&g_AoMutex);
        return Ret;
    }
    memcpy(pstAttr, &stSndDefaultAttr.stAttr, sizeof(DRV_SND_ATTR_S));
    up(&g_AoMutex);

    return Ret;
}

HI_S32 HI_DRV_AO_SND_Open(HI_UNF_SND_E enSound, DRV_SND_ATTR_S* pstAttr, struct file*  pfile)
{
    HI_S32 Ret;
    DRV_AO_STATE_S* pAOState;
    struct file*  pstfile;

    Ret = down_interruptible(&g_AoMutex);

    if (HI_NULL == pfile)
    {
        pAOState = g_filp.private_data;
        pstfile = &g_filp;
    }
    else
    {
        pAOState = pfile->private_data;
        pstfile = pfile;
    }

    CHECK_AO_SNDCARD(enSound);

    HI_INFO_AO("HI_DRV_AO_SND_Open file = %p\n", pfile);

    Ret = AO_Snd_AllocHandle(enSound, pstfile);
    if (HI_SUCCESS == Ret)
    {
        if (0 == atomic_read(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
        {
            Ret = AO_SND_Open(enSound, pstAttr, NULL, HI_FALSE);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("AO_SND_Open Failed(0x%x)\n", Ret);
                AO_Snd_FreeHandle(enSound, pstfile);
                up(&g_AoMutex);
                return Ret;
            }
        }
    }

    atomic_inc(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
    atomic_inc(&pAOState->atmUserOpenCnt[enSound]);
    up(&g_AoMutex);

    return Ret;
}

HI_S32 HI_DRV_AO_SND_Close(HI_UNF_SND_E enSound, struct file* pfile)
{
    HI_S32 Ret;
    DRV_AO_STATE_S* pAOState;
    struct file* pstfilp;

    Ret = down_interruptible(&g_AoMutex);

    if (HI_NULL == pfile)
    {
        pAOState = g_filp.private_data;
        pstfilp = &g_filp;
    }
    else
    {
        pAOState = pfile->private_data;
        pstfilp = pfile;
    }

    CHECK_AO_SNDCARD_OPEN(enSound);

    HI_INFO_AO("HI_DRV_AO_SND_Close file = %p\n", pfile);

    if (atomic_dec_and_test(&pAOState->atmUserOpenCnt[enSound]))
    {
        if (atomic_dec_and_test(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt))
        {
            Ret = AO_SND_Close( enSound, HI_FALSE );
            if (HI_SUCCESS != Ret)
            {
                atomic_inc(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
                atomic_inc(&pAOState->atmUserOpenCnt[enSound]);
                HI_ERR_AO("AO_SND_Close failed(0x%x)\n", Ret);
                up(&g_AoMutex);
                return Ret;
            }

            AO_Snd_FreeHandle(enSound, pstfilp);
        }
    }
    else
    {
        atomic_dec(&s_stAoDrv.astSndEntity[enSound].atmUseTotalCnt);
    }
    up(&g_AoMutex);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_AO_Snd_GetXRunCount(HI_UNF_SND_E enSound, HI_U32* pu32Count)
{
    CHECK_AO_SNDCARD_OPEN(enSound);
    return AO_Snd_GetXRunCount(enSound, pu32Count);
}

HI_S32 HI_DRV_AO_SND_SetVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S* pstGain)
{
    HI_S32 Ret = HI_SUCCESS;
    CHECK_AO_SNDCARD_OPEN( enSound );

    Ret = down_interruptible(&g_AoMutex);
    Ret = AO_SND_SetVolume(enSound, enOutPort, pstGain);
    up(&g_AoMutex);
    return Ret;
}

HI_S32 HI_DRV_AO_SND_GetVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S* pstGain)
{
    HI_S32 Ret = HI_SUCCESS;
    CHECK_AO_SNDCARD_OPEN( enSound );
    Ret = down_interruptible(&g_AoMutex);
    Ret = AO_SND_GetVolume(enSound, enOutPort, pstGain);
    up(&g_AoMutex);
    return Ret;
}

HI_S32 HI_DRV_AO_Track_GetDefaultOpenAttr(HI_UNF_SND_TRACK_TYPE_E enTrackType, HI_UNF_AUDIOTRACK_ATTR_S* pstAttr)
{
    HI_S32 Ret = HI_SUCCESS;

    Ret = down_interruptible(&g_AoMutex);

    pstAttr->enTrackType = enTrackType;
    Ret = AO_Track_GetDefAttr(pstAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("AO_Track_GetDefAttr failed(0x%x)\n", Ret);
        up(&g_AoMutex);
        return Ret;
    }
    up(&g_AoMutex);

    return Ret;
}

HI_S32 HI_DRV_AO_Track_Create(HI_UNF_SND_E enSound, HI_UNF_AUDIOTRACK_ATTR_S* pstAttr, HI_BOOL bAlsaTrack, struct file* pfile, HI_HANDLE* phTrack)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    struct file* pstfilp;
#ifdef __DPT__
    HI_UNF_SND_PRECIGAIN_ATTR_S stPreciGain = { -6 , 0};
#endif
    Ret = down_interruptible(&g_AoMutex);

    if (HI_NULL == pfile)
    {
        pstfilp = &g_filp;
    }
    else
    {
        pstfilp = pfile;
    }
    HI_INFO_AO("HI_DRV_AO_Track_Create bAlsaTrack = %d, file = %p\n", bAlsaTrack, pfile);

    Ret = AO_Track_AllocHandle(enSound, pstAttr->enTrackType, pstfilp, &hHandle);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("AO_Track_AllocHandle Failed \n", Ret);
        up(&g_AoMutex);
        return Ret;
    }

    Ret = AO_Track_PreCreate(enSound, pstAttr, bAlsaTrack, NULL, hHandle);
    if (HI_SUCCESS != Ret)
    {
        AO_Track_FreeHandle(hHandle);
        HI_ERR_AO("AO_Track_FreeHandle Failed(0x%x)\n", Ret);
        up(&g_AoMutex);
        return Ret;
    }
#ifdef __DPT__
    /* Get Alsa Prescale, if fail, then use default value */
    Ret = AO_SND_GetAlsaPrescale(enSound, &stPreciGain);
    if (HI_SUCCESS != Ret)
    {
        stPreciGain.s32IntegerGain = -6;
        stPreciGain.s32DecimalGain = 0;
    }

    /* Set Prescale for ALSA track */
    AO_Track_SetPrescale(hHandle, &stPreciGain);
#endif
    *phTrack = hHandle;
    up(&g_AoMutex);

    return Ret;
}

HI_S32 HI_DRV_AO_Track_Destroy(HI_HANDLE hSndTrack)
{
    HI_S32 Ret = HI_SUCCESS;
    CHECK_AO_TRACK_OPEN(hSndTrack);
    Ret = down_interruptible(&g_AoMutex);

    Ret = AO_Track_Destory(hSndTrack);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("AO_Track_Destory Failed(0x%x)\n", Ret);
        up(&g_AoMutex);
        return Ret;
    }

    AO_Track_FreeHandle(hSndTrack);
    up(&g_AoMutex);

    return Ret;
}

HI_S32 HI_DRV_AO_Track_Flush(HI_HANDLE hSndTrack)
{
    CHECK_AO_TRACK_OPEN(hSndTrack);
    return AO_Track_Flush(hSndTrack);
}

HI_S32 HI_DRV_AO_Track_Start(HI_HANDLE hSndTrack)
{
    CHECK_AO_TRACK_OPEN(hSndTrack);
    return AO_Track_Start(hSndTrack);
}

HI_S32 HI_DRV_AO_Track_Stop(HI_HANDLE hSndTrack)
{
    CHECK_AO_TRACK_OPEN(hSndTrack);
    return AO_Track_Stop(hSndTrack);
}

HI_S32 HI_DRV_AO_Track_GetDelayMs(HI_HANDLE hSndTrack, HI_U32* pDelayMs)
{
    CHECK_AO_TRACK_OPEN(hSndTrack);
    return AO_Track_GetDelayMs(hSndTrack, pDelayMs);
}

HI_S32 HI_DRV_AO_Track_SendData(HI_HANDLE hSndTrack, AO_FRAMEINFO_S* pstAOFrame)
{
    CHECK_AO_TRACK_OPEN(hSndTrack);
    return AO_Track_SendData(hSndTrack, pstAOFrame);
}

HI_S32 HI_DRV_AO_Track_AtomicSendData(HI_HANDLE hTrack, AO_FRAMEINFO_S* pstAOFrame)
{
    HI_U32 u32TrackID;
    SND_CARD_STATE_S* pCard;

    CHECK_AO_TRACK_OPEN(hTrack);

    u32TrackID = hTrack & AO_TRACK_CHNID_MASK;

    pCard = TRACK_CARD_GetCard(u32TrackID);
    if (HI_NULL == pCard)
    {
        return HI_ERR_AO_SOUND_NOT_OPEN;
    }

    if (HI_TRUE == pCard->bDMAMode)
    {
        return HI_SUCCESS;
    }

    return TRACK_AtomicSendData(pCard, u32TrackID, pstAOFrame);
}

HI_S32 HI_DRV_AO_Track_AttachAi(HI_HANDLE hSndTrack, HI_HANDLE hAi)
{
    CHECK_AO_TRACK_OPEN(hSndTrack);
    return AO_Track_AttachAi(hSndTrack, hAi);
}

HI_S32 HI_DRV_AO_Track_DetachAi(HI_HANDLE hSndTrack, HI_HANDLE hAi)
{
    CHECK_AO_TRACK_OPEN(hSndTrack);
    return AO_Track_DetachAi(hSndTrack, hAi);
}

static HI_S32 AO_CAST_GetSettings(HI_HANDLE hCast, SND_CAST_SETTINGS_S* pstCastSettings)
{
    SND_CARD_STATE_S* pCard;

    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    CAST_GetSettings(pCard, hCast, pstCastSettings);
    return HI_SUCCESS;
}

static HI_S32 AO_CAST_RestoreSettings(HI_HANDLE hCast, SND_CAST_SETTINGS_S* pstCastSettings)
{
    SND_CARD_STATE_S* pCard;

    hCast &= AO_CAST_CHNID_MASK;
    pCard = CAST_CARD_GetCard(hCast);
    CHECK_AO_NULL_PTR(pCard);

    CAST_RestoreSettings(pCard, hCast, pstCastSettings);
    return HI_SUCCESS;
}

HI_S32 AO_DRV_Suspend(PM_BASEDEV_S* pdev,
                      pm_message_t state)
{
#if defined (HI_SND_DRV_SUSPEND_SUPPORT)
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;

#if defined(HI_ALSA_AO_SUPPORT) && defined(CONFIG_PM)
    HI_INFO_AO("AO shallow suspend state.event = 0x%x\n", state.event);
    if (PM_LOW_SUSPEND_FLAG == state.event && HI_NULL != hisi_snd_device)
    {
        bu32shallowSuspendActive = HI_TRUE;
        s32Ret = snd_soc_suspend(&hisi_snd_device->dev);    //shallow suspent here suspend alsa driver
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("AO ALSA shallow suspend fail 0x%x\n", s32Ret);
        }
        HI_PRINT("AO ALSA shallow suspend OK.\n");
    }
#endif

#ifdef HI_AIAO_TIMER_SUPPORT
    s32Ret = HI_DRV_Timer_Suspend();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("AO HI_DRV_Timer_Suspend fail 0x%x\n", s32Ret);
    }
    HI_PRINT("AO Timer suspend OK.\n");
#endif

#if (1 == HI_PROC_SUPPORT)
    bSuspend2SaveThreadFlag = HI_TRUE; //just for echo save sound pcm
    while (HI_TRUE == bSaveThread2SuspendFlag) //wait until echo proc for save sound pcm thread exit !
    {
        msleep(2);
    }
#endif

    s32Ret = down_interruptible(&g_AoMutex);
    if (HI_TRUE == s_stAoDrv.bReady)
    {
        /* Destory all cast */
        for (i = 0; i < AO_MAX_CAST_NUM; i++)
        {
            if (atomic_read(&s_stAoDrv.astCastEntity[i].atmUseCnt))
            {
                /* Store cast settings */
                AO_CAST_GetSettings(i, &s_stAoDrv.astCastEntity[i].stSuspendAttr);

                /* Destory cast */
                s32Ret = AO_Cast_Destory(i);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_FATAL_AO("AO_Cast_Destory fail\n");
                    up(&g_AoMutex);
                    return HI_FAILURE;
                }
            }
        }

        /* Destory all track */
        for (i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
        {
            if (atomic_read(&s_stAoDrv.astTrackEntity[i].atmUseCnt))
            {
                /* Store track settings */
                AO_TRACK_GetSettings(i, &s_stAoDrv.astTrackEntity[i].stSuspendAttr);

                /* Destory track */
                s32Ret = AO_Track_Destory(i);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_FATAL_AO("AO_Track_Destory fail\n");
                    up(&g_AoMutex);
                    return HI_FAILURE;
                }
            }
        }

        /* Destory all snd */
        for (i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
        {
            if (s_stAoDrv.astSndEntity[i].pCard)
            {
                /* Store snd settings */
                AO_SND_GetSettings(i, &s_stAoDrv.astSndEntity[i].stSuspendAttr);

                /* Destory snd */
                s32Ret = AO_SND_Close(i, HI_TRUE);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_FATAL_AO("AO_SND_Close fail\n");
                    up(&g_AoMutex);
                    return HI_FAILURE;
                }
            }
        }

        s32Ret = HAL_AIAO_Suspend();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("AIAO Suspend fail\n");
            up(&g_AoMutex);
            return HI_FAILURE;
        }

        if (s_stAoDrv.pAdspFunc && s_stAoDrv.pAdspFunc->pfnADSP_UnLoadFirmware)
        {
            s32Ret = (s_stAoDrv.pAdspFunc->pfnADSP_UnLoadFirmware)(ADSP_CODE_AOE);
        }
    }

    up(&g_AoMutex);
#endif
    HI_PRINT("AO suspend OK\n");
    return HI_SUCCESS;
}

HI_S32 AO_DRV_Resume(PM_BASEDEV_S* pdev)
{
#if defined (HI_SND_DRV_SUSPEND_SUPPORT)
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
#ifdef HI_AUDIO_AI_SUPPORT
    HI_BOOL bAiEnable;
#endif

#if (1 == HI_PROC_SUPPORT)
    bSuspend2SaveThreadFlag = HI_FALSE; //just for echo save sound pcm
#endif

    s32Ret = down_interruptible(&g_AoMutex);

    if (HI_TRUE == s_stAoDrv.bReady)
    {
        /* HAL_AOE_Init , Init aoe hardare */
        if (s_stAoDrv.pAdspFunc && s_stAoDrv.pAdspFunc->pfnADSP_LoadFirmware)
        {
            s32Ret = (s_stAoDrv.pAdspFunc->pfnADSP_LoadFirmware)(ADSP_CODE_AOE);
            if (HI_SUCCESS != s32Ret)
            {
                HI_FATAL_AO("load aoe fail\n");
                up(&g_AoMutex);
                return HI_FAILURE;
            }
        }

        s32Ret = HAL_AIAO_Resume();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("AIAO Resume fail\n");
            up(&g_AoMutex);
            return HI_FAILURE;
        }

        /* Restore all snd */
        for (i = 0; i < AO_MAX_TOTAL_SND_NUM; i++)
        {
            if (s_stAoDrv.astSndEntity[i].pCard)
            {
                /* Recreate snd */
                s32Ret = AO_SND_Open(i, &s_stAoDrv.astSndEntity[i].stSuspendAttr.stUserOpenParam, &s_stAoDrv.astSndEntity[i].stSuspendAttr.stUserOpenParamI2s, HI_TRUE); //#ifdef HI_ALSA_I2S_ONLY_SUPPORT  Check DO???
                if (HI_SUCCESS != s32Ret)
                {
                    HI_FATAL_AO("AO_SND_Open fail\n");
                    up(&g_AoMutex);
                    return HI_FAILURE;
                }

                /* Restore snd settings*/
                AO_SND_RestoreSettings(i, &s_stAoDrv.astSndEntity[i].stSuspendAttr);
            }
        }

        for (i = 0; i < AO_MAX_TOTAL_TRACK_NUM; i++)
        {
            if (atomic_read(&s_stAoDrv.astTrackEntity[i].atmUseCnt))
            {
                HI_UNF_SND_E enSound = s_stAoDrv.astTrackEntity[i].stSuspendAttr.enSound;
#ifdef __DPT__
                SND_CARD_STATE_S* pCard = SND_CARD_GetCard(enSound);
#endif
                HI_UNF_AUDIOTRACK_ATTR_S* pstAttr = &s_stAoDrv.astTrackEntity[i].stSuspendAttr.stTrackAttr;
                HI_BOOL bAlsaTrack = s_stAoDrv.astTrackEntity[i].stSuspendAttr.bAlsaTrack;
                AO_BUF_ATTR_S* pstBuf = &s_stAoDrv.astTrackEntity[i].stSuspendAttr.stBufAttr;

                /* Recreate track  */
                s32Ret = AO_Track_Create(enSound, pstAttr, bAlsaTrack, pstBuf, i);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_FATAL_AO("AO_Track_Create(%d) fail\n", i);
                    up(&g_AoMutex);
                    return HI_FAILURE;
                }
#ifdef HI_AUDIO_AI_SUPPORT
                if (s_stAoDrv.astTrackEntity[i].stSuspendAttr.bAttAi)
                {
                    s32Ret = AI_GetEnable(s_stAoDrv.astTrackEntity[i].stSuspendAttr.hAi, &bAiEnable);
                    if (HI_SUCCESS != s32Ret)
                    {
                        HI_FATAL_AO("AI GetEnable fail\n");
                        up(&g_AoMutex);
                        return HI_FAILURE;
                    }
                    if (HI_TRUE == bAiEnable)
                    {
                        s32Ret = AI_SetEnable(s_stAoDrv.astTrackEntity[i].stSuspendAttr.hAi, HI_FALSE, HI_TRUE);
                        if (HI_SUCCESS != s32Ret)
                        {
                            HI_FATAL_AO("AI SetEnable failed!!\n");
                            up(&g_AoMutex);
                            return HI_FAILURE;
                        }
                    }

                    s32Ret = AO_Track_AttachAi(i, s_stAoDrv.astTrackEntity[i].stSuspendAttr.hAi);
                    if (HI_SUCCESS != s32Ret)
                    {
                        HI_FATAL_AO("AO_Track_AttachAi(%d) fail\n", i);
                        up(&g_AoMutex);
                        return HI_FAILURE;
                    }

                    if (HI_TRUE == bAiEnable)
                    {
                        s32Ret = AI_SetEnable(s_stAoDrv.astTrackEntity[i].stSuspendAttr.hAi, HI_TRUE, HI_TRUE);
                        if (HI_SUCCESS != s32Ret)
                        {
                            HI_FATAL_AO("AI SetEnable failed!!\n");
                            up(&g_AoMutex);
                            return HI_FAILURE;
                        }
                    }
#ifdef __DPT__
                    if (HI_TRUE == s_stAoDrv.astTrackEntity[i].stSuspendAttr.bDuplicate)
                    {
                        s32Ret = TRACK_Duplicate2Aop(pCard, s_stAoDrv.astTrackEntity[i].stSuspendAttr.enDuplicatePort,
                                                     i, HI_TRUE);
                    }
#endif
                    s32Ret = AO_Track_Start(i);
                    if (HI_SUCCESS != s32Ret)
                    {
                        HI_FATAL_AO("AO_Track_Start(%d) fail\n", i);
                        up(&g_AoMutex);
                        return HI_FAILURE;
                    }
                }
#endif
                /* Restore track settings*/
                AO_TRACK_RestoreSettings(i, &s_stAoDrv.astTrackEntity[i].stSuspendAttr);
            }
        }

        /* Restore all cast */
        for (i = 0; i < AO_MAX_CAST_NUM; i++)
        {
            if (atomic_read(&s_stAoDrv.astCastEntity[i].atmUseCnt))
            {
                HI_UNF_SND_E enSound = s_stAoDrv.astCastEntity[i].stSuspendAttr.enSound;
                HI_UNF_SND_CAST_ATTR_S* pstAttr = &s_stAoDrv.astCastEntity[i].stSuspendAttr.stCastAttr;

                /* Recreate cast  */
                s32Ret = AO_Cast_Create(enSound, pstAttr, &s_stAoDrv.astCastEntity[i].stRbfMmz, i);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_FATAL_AO("AO_Cast_Create(%d) fail\n", i);
                    up(&g_AoMutex);
                    return HI_FAILURE;
                }

                /* Restore cast settings*/
                AO_CAST_RestoreSettings(i, &s_stAoDrv.astCastEntity[i].stSuspendAttr);
            }
        }
    }

    up(&g_AoMutex);
#endif

#ifdef HI_AIAO_TIMER_SUPPORT
    s32Ret = HI_DRV_Timer_Resume();
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_AO("HI_DRV_Timer_Resume fail 0x%x\n", s32Ret);
    }
#endif

    HI_PRINT("AO resume OK\n");
#if defined(HI_ALSA_AO_SUPPORT) && defined(CONFIG_PM)
    if (HI_NULL != hisi_snd_device && HI_TRUE == bu32shallowSuspendActive)
    {
        HI_INFO_AO("AO ALSA shallow resume.\n");

        bu32shallowSuspendActive = HI_FALSE;
        s32Ret = snd_soc_resume(&hisi_snd_device->dev);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("AO ALSA shallow resume fail.\n");
        }

        HI_PRINT("AO ALSA shallow resume OK.\n");
    }
#endif

    return HI_SUCCESS;
}

HI_S32 AO_DRV_Init(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = down_interruptible(&g_AoMutex);
    s32Ret = HI_DRV_MODULE_Register(HI_ID_AO, AO_NAME,
                                    (HI_VOID*)&s_stAoDrv.stExtFunc);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_AO("Reg module fail:%#x!\n", s32Ret);
        up(&g_AoMutex);
        return s32Ret;
    }

#ifdef ENA_AO_IRQ_PROC
    /* register ade ISR */
    if (0 != request_irq(AO_IRQ_NUM, AO_IntVdmProc,
                         IRQF_DISABLED, "aiao",
                         HI_NULL))
    {
        HI_FATAL_AO("FATAL: request_irq for VDI VDM err!\n");
        up(&g_AoMutex);
        return HI_FAILURE;
    }
#endif

    //for karaoke print
    s32Ret = HI_DRV_MODULE_Register(HI_ID_KARAOKE, "HI_KARAOKE", (HI_VOID*)HI_NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_AO("HI_DRV_MODULE_Register HI_KARAOKE failed 0x%x\n", s32Ret);
        up(&g_AoMutex);
        return s32Ret;
    }

    up(&g_AoMutex);
    return HI_SUCCESS;
}

HI_VOID AO_DRV_Exit(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = down_interruptible(&g_AoMutex);
#ifdef ENA_AO_IRQ_PROC
    free_irq(AO_IRQ_NUM, HI_NULL);
#endif
    HI_DRV_MODULE_UnRegister(HI_ID_KARAOKE);
    HI_DRV_MODULE_UnRegister(HI_ID_AO);

    up(&g_AoMutex);
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

