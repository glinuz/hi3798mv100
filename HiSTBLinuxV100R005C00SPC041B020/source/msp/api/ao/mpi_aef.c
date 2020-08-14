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


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


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
#include <pthread.h>
#include <dlfcn.h>
#include <dirent.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_module.h"
#include "hi_mpi_mem.h"
#include "hi_error_mpi.h"

#include "hi_unf_audio.h"
#include "hi_mpi_ao.h"
#include "hi_audio_effect.h"
#include "mpi_aef.h"

#ifdef HI_SND_USER_AEF_SUPPORT
#include "hi_aef_sws.h"
#include "hi_aef_srs.h"
#include "hi_aef_customer.h"
#include "hi_aef_dbx.h"
#endif

/* User Effect number: AOE_AEF_MAX_CHAN_NUM, DSP Effect number: AOE_AEF_MAX_CHAN_NUM */
static HI_VOID* s_apAefChn[2 * AOE_AEF_MAX_CHAN_NUM] = {0};
#if defined(HI_SND_USER_AEF_SUPPORT)
static HI_VOID* s_apSndAefAttr[HI_UNF_SND_BUTT] = {0};
#endif
/* aef authorize management */
static HI_AEF_AUTHORIZE_S* g_hFirstAefAuth = NULL;

#if defined(HI_HAEFFECT_SWS_SUPPORT)
#if defined(HI_SND_USER_AEF_SUPPORT)
extern HI_AEF_COMPONENT_S ha_sws_effect_sw_entry;
#else
extern HI_AEF_COMPONENT_S ha_sws_effect_entry;
#endif
#endif

#if defined(HI_HAEFFECT_LP_SUPPORT)
#if defined(HI_SND_USER_AEF_SUPPORT)
extern HI_AEF_COMPONENT_S ha_linearph_effect_sw_entry;
#endif
#endif

#if defined(HI_HAEFFECT_SRS_SUPPORT)
#if !defined(HI_SND_USER_AEF_SUPPORT)
extern HI_AEF_COMPONENT_S ha_srs_effect_entry;
#endif
#endif

#if defined(HI_HAEFFECT_CUSTOMER_SUPPORT)
#if defined(HI_SND_USER_AEF_SUPPORT)
extern HI_AEF_COMPONENT_S ha_customer_effect_sw_entry;
#endif
#endif

#if defined(HI_HAEFFECT_DBX_SUPPORT)
#if !defined(HI_SND_USER_AEF_SUPPORT)
extern HI_AEF_COMPONENT_S ha_dbx_effect_entry;
#endif
#endif

static pthread_mutex_t     g_AefThreadMutex = PTHREAD_MUTEX_INITIALIZER;     /*mutex for data safety use*/

#ifdef HI_SND_USER_AEF_SUPPORT

#define AEF_SLEEP_TIME_MS  (5)



static HI_S32 AEFGetOutBufSpaceAddr(HI_UNF_SND_E enSound, HI_VOID** ppOutBuf)
{
    HI_U32 ReadIdx;
    HI_U32 WriteIdx;
    HI_U32 ReadWrap;
    HI_U32 WriteWrap;
    U_ENGINE_AEF_BUF_READIDX  AEF_OUTBUF_READIDX;
    U_ENGINE_AEF_BUF_WRITEIDX AEF_OUTBUF_WRITEIDX;
    SND_AEF_ATTR_S* pstSndAefAttr = (SND_AEF_ATTR_S*)s_apSndAefAttr[enSound];
    S_MIXER_REGS_TYPE* pEngRegAddr = pstSndAefAttr->pEngRegAddr;

    AEF_OUTBUF_READIDX.u32  = pEngRegAddr->ENGINE_AEF_OUTBUF_READIDX.u32;
    AEF_OUTBUF_WRITEIDX.u32 = pEngRegAddr->ENGINE_AEF_OUTBUF_WRITEIDX.u32;

    ReadIdx   = AEF_OUTBUF_READIDX.bits.periond_read_idx;
    ReadWrap  = AEF_OUTBUF_READIDX.bits.periond_read_wrap;
    WriteIdx  = AEF_OUTBUF_WRITEIDX.bits.periond_write_idx;
    WriteWrap = AEF_OUTBUF_WRITEIDX.bits.periond_write_wrap;

    if ((ReadIdx == WriteIdx) && (ReadWrap != WriteWrap))
    {
        return HI_ERR_AO_OUT_BUF_FULL;
    }

    *ppOutBuf = pstSndAefAttr->pOutBufAddr + pstSndAefAttr->stAefBufAttr.u32OutBufFrameSize * WriteIdx;

    return HI_SUCCESS;
}

static HI_S32 AEFGetInBufDataAddr(HI_UNF_SND_E enSound, HI_VOID** ppInBuf)
{
    HI_U32 ReadIdx;
    HI_U32 WriteIdx;
    HI_U32 ReadWrap;
    HI_U32 WriteWrap;
    U_ENGINE_AEF_BUF_READIDX  AEF_INBUF_READIDX;
    U_ENGINE_AEF_BUF_WRITEIDX AEF_INBUF_WRITEIDX;
    SND_AEF_ATTR_S* pstSndAefAttr = (SND_AEF_ATTR_S*)s_apSndAefAttr[enSound];
    S_MIXER_REGS_TYPE* pEngRegAddr = pstSndAefAttr->pEngRegAddr;

    AEF_INBUF_READIDX.u32  = pEngRegAddr->ENGINE_AEF_INBUF_READIDX.u32;
    AEF_INBUF_WRITEIDX.u32 = pEngRegAddr->ENGINE_AEF_INBUF_WRITEIDX.u32;

    ReadIdx   = AEF_INBUF_READIDX.bits.periond_read_idx;
    ReadWrap  = AEF_INBUF_READIDX.bits.periond_read_wrap;
    WriteIdx  = AEF_INBUF_WRITEIDX.bits.periond_write_idx;
    WriteWrap = AEF_INBUF_WRITEIDX.bits.periond_write_wrap;

    if ((ReadIdx == WriteIdx) && (ReadWrap == WriteWrap))
    {
        return HI_ERR_AO_INBUF_EMPTY;
    }

    *ppInBuf = pstSndAefAttr->pInBufAddr + pstSndAefAttr->stAefBufAttr.u32InBufFrameSize * ReadIdx;

    return HI_SUCCESS;
}

static HI_VOID AEFUpdateInBufReadIdx(HI_UNF_SND_E enSound)
{
    HI_U32 ReadIdx;
    HI_U32 ReadWrap;
    U_ENGINE_AEF_BUF_READIDX AEF_INBUF_READIDX;
    SND_AEF_ATTR_S* pstSndAefAttr = (SND_AEF_ATTR_S*)s_apSndAefAttr[enSound];
    S_MIXER_REGS_TYPE* pEngRegAddr = pstSndAefAttr->pEngRegAddr;

    AEF_INBUF_READIDX.u32 = pEngRegAddr->ENGINE_AEF_INBUF_READIDX.u32;

    ReadIdx  = AEF_INBUF_READIDX.bits.periond_read_idx;
    ReadWrap = AEF_INBUF_READIDX.bits.periond_read_wrap;

    ReadIdx++;
    if (ReadIdx >= pstSndAefAttr->stAefBufAttr.u32InBufFrameNum)
    {
        ReadIdx = 0;
        ReadWrap ^= 1;
    }

    AEF_INBUF_READIDX.bits.periond_read_idx   = ReadIdx;
    AEF_INBUF_READIDX.bits.periond_read_wrap  = ReadWrap;
    pEngRegAddr->ENGINE_AEF_INBUF_READIDX.u32 = AEF_INBUF_READIDX.u32;

    return;
}

static HI_VOID AEFUpdateOutBufWriteIdx(HI_UNF_SND_E enSound)
{
    HI_U32 WriteIdx;
    HI_U32 WriteWrap;
    U_ENGINE_AEF_BUF_WRITEIDX AEF_OUTBUF_WRITEIDX;
    SND_AEF_ATTR_S* pstSndAefAttr = (SND_AEF_ATTR_S*)s_apSndAefAttr[enSound];
    S_MIXER_REGS_TYPE* pEngRegAddr = pstSndAefAttr->pEngRegAddr;

    AEF_OUTBUF_WRITEIDX.u32 = pEngRegAddr->ENGINE_AEF_OUTBUF_WRITEIDX.u32;

    WriteIdx  = AEF_OUTBUF_WRITEIDX.bits.periond_write_idx;
    WriteWrap = AEF_OUTBUF_WRITEIDX.bits.periond_write_wrap;

    WriteIdx++;
    if (WriteIdx >= pstSndAefAttr->stAefBufAttr.u32OutBufFrameNum)
    {
        WriteIdx = 0;
        WriteWrap ^= 1;
    }

    AEF_OUTBUF_WRITEIDX.bits.periond_write_idx  = WriteIdx;
    AEF_OUTBUF_WRITEIDX.bits.periond_write_wrap = WriteWrap;
    pEngRegAddr->ENGINE_AEF_OUTBUF_WRITEIDX.u32 = AEF_OUTBUF_WRITEIDX.u32;

    return;
}

static HI_VOID AEFSendBypassData(HI_VOID* pInBuf, HI_VOID* pOutBuf)
{
    AOE_AEF_FRAME_INFO_S* pstAefFrm = {0};
    HI_VOID* pDmxInBuf, *pAssocInBuf, *pDmxOutBuf, *pAssocOutBuf;

    //Dmx without aef, bypass
    pstAefFrm = (AOE_AEF_FRAME_INFO_S*)pInBuf;   //frame info addr;
    pDmxInBuf = pInBuf + sizeof(AOE_AEF_FRAME_INFO_S) + pstAefFrm->u32MixDmxDataSize + pstAefFrm->u32MixAssocDataSize + pstAefFrm->u32MixMcDataSize;
    pDmxOutBuf = pOutBuf;
    memcpy(pDmxOutBuf, pDmxInBuf, pstAefFrm->u32BakMixDmxDataSize);

    //Assoc, bypass
    if (pstAefFrm->u32MixAssocDataSize)
    {
        pAssocInBuf = pDmxInBuf + pstAefFrm->u32MixDmxDataSize;
        pAssocOutBuf = pDmxOutBuf + pstAefFrm->u32MixDmxDataSize;
        memcpy(pAssocOutBuf, pAssocInBuf, pstAefFrm->u32MixAssocDataSize);
    }
    return;
}

static HI_VOID AEFFlush(HI_UNF_SND_E enSound, SND_AEF_ATTR_S* pstSndAefAttr, HI_VOID* pInBuf, HI_VOID* pOutBuf)
{
    HI_S32   s32Ret;
    HI_U32   u32AefId, u32AefMask, i;
    AEF_CHANNEL_S*        pstAefChn   = HI_NULL;
    HI_AEF_COMPONENT_S*   pstEntry    = HI_NULL;
    HI_BOOL  bMcProcess = HI_FALSE;
    HI_VOID* pDmxInBuf, *pMcInBuf;
    HI_VOID* pDmxOutBuf, *pAefOutBuf;
    HI_AEF_PROCFRAME_S    stProcFrame = {{0}};
    AOE_AEF_FRAME_INFO_S* pstAefFrm;

    pstAefFrm  = (AOE_AEF_FRAME_INFO_S*)pInBuf;   //frame info addr;
    pDmxOutBuf = pOutBuf;
    pDmxInBuf  = pInBuf + sizeof(AOE_AEF_FRAME_INFO_S);
    pAefOutBuf = pDmxOutBuf + pstAefFrm->u32MixDmxDataSize + pstAefFrm->u32MixAssocDataSize;     //aef proc out buffer
    bMcProcess = pstSndAefAttr->pEngRegAddr->ENGINE_ATTR.bits.aef_support_mc;
    u32AefMask = pstSndAefAttr->pEngRegAddr->ENGINE_ATT_AEF.bits.aef_att_ena;

    for (u32AefId = AOE_AEF_MAX_CHAN_NUM; u32AefId < AOE_AEF_MAX_CHAN_NUM * 2; u32AefId++)
    {
        if (!(u32AefMask & (1 << u32AefId)))
        {
            continue;
        }
        pstAefChn = (AEF_CHANNEL_S*)s_apAefChn[u32AefId];
        if (!pstAefChn)
        {
            continue;
        }
        if (enSound != pstAefChn->enSnd)
        {
            continue;
        }
        pstEntry   = (HI_AEF_COMPONENT_S*)pstAefChn->hEntry;

        if (pstAefFrm->u32MixMcDataSize && (HI_TRUE == bMcProcess)) //Mc
        {
            pMcInBuf = pInBuf + sizeof(AOE_AEF_FRAME_INFO_S) + pstAefFrm->u32MixDmxDataSize + pstAefFrm->u32MixAssocDataSize;
            stProcFrame.stInBufAttr.u32BufAddr = (HI_U32)pMcInBuf;
            stProcFrame.stOutBufAttr.u32BufAddr = (HI_U32)pAefOutBuf;
            stProcFrame.stInBufAttr.u32ChNum = 6;//todo
            bMcProcess = HI_FALSE;
        }
        else
        {
            stProcFrame.stInBufAttr.u32BufAddr = (HI_U32)pDmxInBuf;
            stProcFrame.stOutBufAttr.u32BufAddr = (HI_U32)pAefOutBuf;
            stProcFrame.stInBufAttr.u32ChNum = 2;//todo
        }
        stProcFrame.stInBufAttr.u32BitWidth     = pstAefFrm->u32BitPerSample;
        stProcFrame.stInBufAttr.u32SamplesPerFrame = pstAefFrm->u32SamplesPerFrame;

        for(i = 0; i < AEF_FLUSH_PROC_COUNT; i++)
        {
            s32Ret = pstEntry->AefProcFrame(pstAefChn->hHaEffect, &stProcFrame);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_AO(" Aef Proc Frame failed\n");
            }
        }
    }

    AEFUpdateInBufReadIdx(enSound);
    return;
}

static HI_VOID Aef_DebugSetting(HI_AEF_SETTING_S *pstAefSetting, HI_AEF_COMPONENT_S* pstEntry, HI_HANDLE hHaEffect)
{
    if ((pstAefSetting->bChanged) && (pstAefSetting->enAefType == pstEntry->enEffectID))
    {
        pstEntry->AefSetConfig(hHaEffect, HI_AEF_DEBUG_CONFIG_CMD, &(pstAefSetting->pAefSetting));
        pstAefSetting->bChanged = HI_FALSE;
    }

    if ((pstAefSetting->bDebug) && (pstAefSetting->enAefType == pstEntry->enEffectID))
    {
        pstEntry->AefGetConfig(hHaEffect, HI_AEF_DEBUG_CONFIG_CMD, &(pstAefSetting->pAefSetting));
        pstAefSetting->bDebug = HI_FALSE;
    }
}

HI_VOID* Aef_ProcessThread(HI_VOID* Arg)
{
    HI_S32   s32Ret;
    HI_U32   u32AefId;
    HI_U32   u32AefMask;
    HI_BOOL  bMcProcess = HI_FALSE;
    AOE_AEF_FRAME_INFO_S* pstAefFrm;
    HI_VOID* pInBuf, *pOutBuf;
    HI_VOID* pDmxInBuf, *pMcInBuf;
    HI_VOID* pDmxOutBuf, *pAefOutBuf;
    HI_AEF_COMPONENT_S*   pstEntry    = HI_NULL;
    AEF_CHANNEL_S*        pstAefChn   = HI_NULL;
    HI_AEF_PROCFRAME_S    stProcFrame = {{0}, {0}};
    HI_UNF_SND_E          enSound = (HI_UNF_SND_E)Arg;
    SND_AEF_ATTR_S*       pstSndAefAttr = (SND_AEF_ATTR_S*)s_apSndAefAttr[enSound];
    HI_U32                u32AefDir = 0;

    while (pstSndAefAttr->bAefThreadRun)
    {
        AEF_LOCK(&g_AefThreadMutex);

        if (!pstSndAefAttr->pEngRegAddr)
        {
            pstSndAefAttr->stAefBufAttr.stAefDebugAddr.u32SettingSize = HI_AEF_SETTINGSIZE;
            s32Ret = HI_MPI_AO_SND_GetAefBufAttr(enSound, &pstSndAefAttr->stAefBufAttr);
            if ((HI_SUCCESS != s32Ret) || (0 == pstSndAefAttr->stAefBufAttr.tPhyEngineAddr))
            {
                AEF_UNLOCK(&g_AefThreadMutex);
                usleep(AEF_SLEEP_TIME_MS * 1000);
                continue;
            }
            else
            {
                pstSndAefAttr->pInBufAddr = HI_MEM_Map(pstSndAefAttr->stAefBufAttr.tPhyInBufAddr, pstSndAefAttr->stAefBufAttr.u32InBufSize);
                if (HI_NULL == pstSndAefAttr->pInBufAddr)
                {
                    HI_ERR_AO(" HI_MEM_Map aef inbuf failed\n");
                    AEF_UNLOCK(&g_AefThreadMutex);
                    return HI_NULL;
                }

                pstSndAefAttr->pOutBufAddr = HI_MEM_Map(pstSndAefAttr->stAefBufAttr.tPhyOutBufAddr, pstSndAefAttr->stAefBufAttr.u32OutBufSize);
                if (HI_NULL == pstSndAefAttr->pOutBufAddr)
                {
                    HI_ERR_AO(" HI_MEM_Map aef outbuf failed\n");
                    HI_MEM_Unmap(pstSndAefAttr->pInBufAddr);
                    AEF_UNLOCK(&g_AefThreadMutex);
                    return HI_NULL;
                }

                pstSndAefAttr->pEngRegAddr = (S_MIXER_REGS_TYPE*)HI_MEM_Map(pstSndAefAttr->stAefBufAttr.tPhyEngineAddr, sizeof(S_MIXER_REGS_TYPE));
                if (HI_NULL == pstSndAefAttr->pEngRegAddr)
                {
                    HI_ERR_AO(" HI_MEM_Map engine reg addr failed\n");
                    HI_MEM_Unmap(pstSndAefAttr->pInBufAddr);
                    HI_MEM_Unmap(pstSndAefAttr->pOutBufAddr);
                    AEF_UNLOCK(&g_AefThreadMutex);
                    return HI_NULL;
                }

                pstSndAefAttr->pAefSettingAddr = HI_MEM_Map(pstSndAefAttr->stAefBufAttr.stAefDebugAddr.tPhySettingAddr, pstSndAefAttr->stAefBufAttr.stAefDebugAddr.u32SettingSize);
                if (HI_NULL == pstSndAefAttr->pAefSettingAddr)
                {
                    HI_ERR_AO(" HI_MEM_Map aef debug seeting failed\n");
                    HI_MEM_Unmap(pstSndAefAttr->pInBufAddr);
                    HI_MEM_Unmap(pstSndAefAttr->pOutBufAddr);
                    HI_MEM_Unmap(pstSndAefAttr->pEngRegAddr);
                    AEF_UNLOCK(&g_AefThreadMutex);
                    return HI_NULL;
                }
            }
        }

        s32Ret = AEFGetOutBufSpaceAddr(enSound, &pOutBuf);
        if (HI_SUCCESS != s32Ret)
        {
            AEF_UNLOCK(&g_AefThreadMutex);
            usleep(AEF_SLEEP_TIME_MS * 1000);
            continue;
        }

        s32Ret = AEFGetInBufDataAddr(enSound, &pInBuf);
        if (HI_SUCCESS != s32Ret)
        {
            AEF_UNLOCK(&g_AefThreadMutex);
            usleep(AEF_SLEEP_TIME_MS * 1000);
            continue;
        }

        pstAefFrm  = (AOE_AEF_FRAME_INFO_S*)pInBuf;   //frame info addr;

        if(pstAefFrm->bFlush)
        {
            AEFFlush(enSound, pstSndAefAttr, pInBuf, pOutBuf);
            AEF_UNLOCK(&g_AefThreadMutex);
            continue;
        }

        pDmxOutBuf = pOutBuf;
        pDmxInBuf  = pInBuf + sizeof(AOE_AEF_FRAME_INFO_S);
        pAefOutBuf = pDmxOutBuf + pstAefFrm->u32MixDmxDataSize + pstAefFrm->u32MixAssocDataSize;     //aef proc out buffer
        bMcProcess = pstSndAefAttr->pEngRegAddr->ENGINE_ATTR.bits.aef_support_mc;
        u32AefMask = pstSndAefAttr->pEngRegAddr->ENGINE_ATT_AEF.bits.aef_att_ena;

        AEFSendBypassData(pInBuf, pOutBuf);
        for (u32AefId = AOE_AEF_MAX_CHAN_NUM; u32AefId < AOE_AEF_MAX_CHAN_NUM * 2; u32AefId++)
        {
            if (!(u32AefMask & (1 << u32AefId)))
            {
                continue;
            }
            pstAefChn = (AEF_CHANNEL_S*)s_apAefChn[u32AefId];
            if (!pstAefChn)
            {
                continue;
            }
            if (enSound != pstAefChn->enSnd)
            {
                continue;
            }
            pstEntry   = (HI_AEF_COMPONENT_S*)pstAefChn->hEntry;

            Aef_DebugSetting((HI_AEF_SETTING_S *)pstSndAefAttr->pAefSettingAddr, pstEntry, pstAefChn->hHaEffect);

            u32AefDir++;

            //below code if for aef content data optimitizer, reduce cnt of memcpy
            if (pstAefFrm->u32MixMcDataSize && (HI_TRUE == bMcProcess)) //Mc
            {
                pMcInBuf = pInBuf + sizeof(AOE_AEF_FRAME_INFO_S) + pstAefFrm->u32MixDmxDataSize + pstAefFrm->u32MixAssocDataSize;
                stProcFrame.stInBufAttr.u32BufAddr = (HI_U32)pMcInBuf;
                stProcFrame.stOutBufAttr.u32BufAddr = (HI_U32)pAefOutBuf;
                stProcFrame.stInBufAttr.u32ChNum = 6;//todo
                bMcProcess = HI_FALSE;
            }
            else if (u32AefDir%2 == 0)  //when dir is even, input buf of aef: dmx_buf, output buf of aef: aefout_buf
            {
                stProcFrame.stInBufAttr.u32BufAddr = (HI_U32)pAefOutBuf;
                stProcFrame.stOutBufAttr.u32BufAddr = (HI_U32)pDmxInBuf;
                stProcFrame.stInBufAttr.u32ChNum = 2;//todo
            }
            else   //when dir is odd, input buf of aef: aefout_buf, output buf of aef: dmx_buf
            {
                stProcFrame.stInBufAttr.u32BufAddr = (HI_U32)pDmxInBuf;
                stProcFrame.stOutBufAttr.u32BufAddr = (HI_U32)pAefOutBuf;
                stProcFrame.stInBufAttr.u32ChNum = 2;//todo
            }
            stProcFrame.stInBufAttr.u32BitWidth     = pstAefFrm->u32BitPerSample;
            stProcFrame.stInBufAttr.u32SamplesPerFrame = pstAefFrm->u32SamplesPerFrame;

            s32Ret = pstEntry->AefProcFrame(pstAefChn->hHaEffect, &stProcFrame);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_AO(" Aef Proc Frame failed\n");
            }
        }

        if (u32AefDir%2 == 0)
        {
            memcpy(pAefOutBuf, pDmxInBuf, pstAefFrm->u32MixDmxDataSize);
        }

        stProcFrame.stInBufAttr.u32BufAddr = 0;
        stProcFrame.stOutBufAttr.u32BufAddr = 0;
        u32AefDir = 0;

        //proc finish, update inbuf read index and outbuf write index
        AEFUpdateInBufReadIdx(enSound);
        AEFUpdateOutBufWriteIdx(enSound);
        AEF_UNLOCK(&g_AefThreadMutex);
    }

    return HI_NULL;
}
#endif

static HI_U32 AEFHandle2ID(HI_HANDLE hAef)
{
    return hAef & AO_AEF_CHNID_MASK;
}

static HI_AEF_AUTHORIZE_S* AEFFindHaEffectAuth(HI_U32 enEffectID)
{
    HI_AEF_AUTHORIZE_S* p;

    p = g_hFirstAefAuth;
    while (p)
    {
        if ((HA_GET_ID(p->enEffectID) == HA_GET_ID(enEffectID)))
        {

            return p;
        }

        p = p->pstNext;
    }

    HI_ERR_AO ("  AEFFindHaAffectAuth  effect(ID=0x%x) Fail \n", enEffectID);

    return NULL;
}

HI_S32 HI_MPI_AO_AEFFindHaEffectAuth(HI_U32 enEffectID, HI_AEF_AUTHORIZE_S** ppstAefAuth)
{
    if (NULL == ppstAefAuth)
    {
        HI_ERR_AO ("  AEFFindHaAffectAuth ppstAefauth invalid \n");
        return HI_FAILURE;
    }

    *ppstAefAuth = AEFFindHaEffectAuth(enEffectID);

    if (NULL == *ppstAefAuth)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 AEFCheckHaEffectAuth(const HI_AEF_AUTHORIZE_S* pEntry)
{
    CHECK_AO_NULL_PTR(pEntry->GetAuthKey);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AO_AEF_RegisterAuthLib(const HI_CHAR* pAefLibFileName)
{
    HI_VOID* pDllModule;
    HI_AEF_AUTHORIZE_S** p;
    HI_AEF_AUTHORIZE_S* pEntry;

    CHECK_AO_NULL_PTR(pAefLibFileName);

    /* load the audio effect authorize lib and check for an error.  If filename is not an
     * absolute path (i.e., it does not  begin with a "/"), then the
     * file is searched for in the following locations:
     *
     *     The LD_LIBRARY_PATH environment variable locations
     *     The library cache, /etc/ld.so.cache.
     *     /lib
     *     /usr/lib
     *
     * If there is an error, we can't go on, so set the error code and exit */
    pDllModule = dlopen(pAefLibFileName, RTLD_LAZY | RTLD_GLOBAL);
    if (pDllModule == NULL)
    {
        HI_WARN_AO ( "  ****** Audio effect authorize lib %s failed because dlopen fail %s\n\n", pAefLibFileName, dlerror());
        return HI_FAILURE;
    }

    /* Get a entry pointer to the "ha_audio_decode_entry" .  If
     * there is an error, we can't go on, so set the error code and exit */
    pEntry = (HI_AEF_AUTHORIZE_S*)dlsym(pDllModule, "ha_audio_effect_auth_entry");
    if (pEntry == NULL)
    {
        HI_ERR_AO ( "  %s Failed because dlsym fail %s\n\n", pAefLibFileName, dlerror());
        dlclose(pDllModule);
        return HI_FAILURE;
    }

    if (HI_SUCCESS != AEFCheckHaEffectAuth(pEntry))
    {
        HI_ERR_AO ( " Register %s Failed \n", pAefLibFileName);
        dlclose(pDllModule);
        return HI_FAILURE;
    }

    p = &g_hFirstAefAuth;
    while (*p != NULL)
    {
        if (HA_GET_ID((*p)->enEffectID) == HA_GET_ID(pEntry->enEffectID))
        {
            HI_WARN_AO ( " Fail:Effect(ID=0x%x) had been Registered \n\n",
                         pEntry->enEffectID);
            dlclose(pDllModule);
            return HI_SUCCESS;
        }

        p = &(*p)->pstNext;
    }

    HI_INFO_AO ( "##### %s Effect Auth  Success #####\n\n", (HI_CHAR*)(pEntry->szName));

    *p = pEntry;
    (*p)->pstNext = NULL;

    return HI_SUCCESS;
}

static HI_AEF_COMPONENT_S* AEFFindHaEffectComp(HI_UNF_SND_AEF_TYPE_E enEffectID)
{
#ifdef __arm__
    switch (enEffectID)
    {
#if defined(DOLBYDV258_SUPPORT)
        case HI_UNF_SND_AEF_TYPE_DOLBYDV258:
            return &ha_dolbydv258_entry;
#endif

#if defined(HI_HAEFFECT_SRS_SUPPORT)
        case HI_UNF_SND_AEF_TYPE_SRS3D:
#if !defined(HI_SND_USER_AEF_SUPPORT)
            return &ha_srs_effect_entry;
#endif
#endif

#if defined(HI_HAEFFECT_SWS_SUPPORT)
        case HI_UNF_SND_AEF_TYPE_SWS:
#if defined(HI_SND_USER_AEF_SUPPORT)
            return &ha_sws_effect_sw_entry;
#else
            return &ha_sws_effect_entry;
#endif
#endif

#if defined(HI_HAEFFECT_LP_SUPPORT)
        case HI_UNF_SND_AEF_TYPE_LP:
#if defined(HI_SND_USER_AEF_SUPPORT)
            return &ha_linearph_effect_sw_entry;
#else
            return HI_NULL;
#endif
#endif

#if defined(HI_HAEFFECT_CUSTOMER_SUPPORT)
        case HI_UNF_SND_AEF_TYPE_CUSTOMER:
#if defined(HI_SND_USER_AEF_SUPPORT)
            return &ha_customer_effect_sw_entry;
#else
            return HI_NULL;
#endif
#endif

#if defined(HI_HAEFFECT_DBX_SUPPORT)

        case HI_UNF_SND_AEF_TYPE_DBX:
            return &ha_dbx_effect_entry;
#endif

        default:
            return HI_NULL;
    }
#else
    return HI_NULL;
#endif
}


static HI_S32 AEFAllocChn(HI_U32* pu32AefId)
{
    HI_U32 u32AefId;
    HI_U32 u32AefMaxChanNum;  //The max   channel num of aef
    HI_U32 u32AefBegChanNum;  //The begin channel num of aef

#ifdef HI_SND_USER_AEF_SUPPORT
    //User aef : 4~7
    u32AefMaxChanNum = AOE_AEF_MAX_CHAN_NUM * 2;
    u32AefBegChanNum = AOE_AEF_MAX_CHAN_NUM;
#else
    //Dsp  aef : 0~3
    u32AefMaxChanNum = AOE_AEF_MAX_CHAN_NUM;
    u32AefBegChanNum = 0;
#endif

    for (u32AefId = u32AefBegChanNum; u32AefId < u32AefMaxChanNum; u32AefId++)
    {
        if (HI_NULL == s_apAefChn[u32AefId])
        {
            break;
        }
    }

    if (u32AefId == u32AefMaxChanNum)
    {
        HI_ERR_AO(" no aef resource\n");
        return HI_FAILURE;
    }

    *pu32AefId = u32AefId;
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AO_AEF_Create(HI_UNF_SND_E enSound, HI_UNF_SND_AEF_TYPE_E enAefType, HI_VOID* pstAdvAttr, HI_HANDLE* phAef)
{
#ifdef __arm__
    HI_S32    s32Ret;
    HI_U32    u32AfltId;
    HI_U32    u32AefId  = 0, u32AefMaxInCh = 0, u32AefFrmDelay = 0;
    HI_HANDLE hHaEffect = HI_INVALID_HANDLE;
    HI_U32    u32AefProcAddr = 0;
    HI_AEF_AUTHORIZE_S*  pstAuthEntry = HI_NULL;
    HI_AEF_COMPONENT_S*  pstEntry  = HI_NULL;
    AEF_CHANNEL_S*       pstAefChn = HI_NULL;
    AO_AEF_PROC_ITEM_S*  pstProcItem;
    AO_AEF_ATTR_S        stAefAttr;
#ifdef HI_SND_USER_AEF_SUPPORT
    SND_AEF_ATTR_S* pstSndAefAttr = (SND_AEF_ATTR_S*)s_apSndAefAttr[enSound];
#endif

    CHECK_AO_NULL_PTR(pstAdvAttr);
    CHECK_AO_NULL_PTR(phAef);
    AEF_LOCK(&g_AefThreadMutex);

#ifdef HI_SND_USER_AEF_SUPPORT
    if (HI_NULL == pstSndAefAttr)
    {
        pstSndAefAttr = (SND_AEF_ATTR_S*)HI_MALLOC(HI_ID_AO, sizeof(SND_AEF_ATTR_S));
        if (HI_NULL == pstSndAefAttr)
        {
            HI_ERR_AO(" Snd aef attr malloc failed\n");
            goto AEF_ERR_RETURE;
        }
        memset(pstSndAefAttr, 0, sizeof(SND_AEF_ATTR_S));
        s_apSndAefAttr[enSound] = (HI_VOID*)pstSndAefAttr;
    }
#endif

    s32Ret = AEFAllocChn(&u32AefId);
    if (HI_SUCCESS != s32Ret)
    {
        goto AEF_ERR_MALLOC1;
    }

    if ((enAefType == HI_UNF_SND_AEF_TYPE_SRS3D) || (enAefType == HI_UNF_SND_AEF_TYPE_DBX))
    {
        //find effect authorize lib
        pstAuthEntry = AEFFindHaEffectAuth((HI_U32)enAefType);
        if (HI_NULL == pstAuthEntry)
        {
            HI_ERR_AO("  AEFFindHaEffectAuth fail u32EffectID(0x%x) ! \n", (HI_U32)enAefType);
            goto AEF_ERR_MALLOC1;
        }
    }

    //find effect component
    pstEntry = AEFFindHaEffectComp(enAefType);
    if (HI_NULL == pstEntry)
    {
        HI_ERR_AO(" can't find haeffect component %d\n", enAefType);
        goto AEF_ERR_MALLOC1;
    }

    //alloc aef channel
    pstAefChn = (AEF_CHANNEL_S*)HI_MALLOC(HI_ID_AO, sizeof(AEF_CHANNEL_S));
    if (HI_NULL == pstAefChn)
    {
        HI_ERR_AO(" Aef malloc channel failed\n");
        goto AEF_ERR_MALLOC1;
    }
    memset(pstAefChn, 0, sizeof(AEF_CHANNEL_S));

    //create aef channe //TODO¡¡pstAdvAttr
    s32Ret = pstEntry->AefCreate(pstAuthEntry, pstAdvAttr, &hHaEffect);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" Aef create failed\n");
        goto AEF_ERR_MALLOC;
    }

#ifdef HI_SND_USER_AEF_SUPPORT
    u32AfltId = u32AefId;
#else
    s32Ret = pstEntry->AefGetConfig(hHaEffect, HI_AEF_GET_AFLTID_CMD, (HI_VOID*)(&u32AfltId));
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" AefGetConfig(HI_AEF_GET_AFLTID_CMD) failed\n");
        goto AEF_ERR_MALLOC;
    }
#endif

    //Get max supported input channel of current aef
    s32Ret = pstEntry->AefGetConfig(hHaEffect, HI_AEF_GET_CH_CMD, (HI_VOID*)(&u32AefMaxInCh));
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" AefGetConfig(HI_AEF_GET_CH_CMD) failed\n");
        goto AEF_ERR_MALLOC;
    }

    //Get the frame delay of current aef
    s32Ret = pstEntry->AefGetConfig(hHaEffect, HI_AEF_GET_FRAMEDELAY_CMD, (HI_VOID*)(&u32AefFrmDelay));
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" AefGetConfig(HI_AEF_GET_FRAMEDELAY_CMD) failed\n");
        goto AEF_ERR_MALLOC;
    }

    memset(&stAefAttr, 0, sizeof(AO_AEF_ATTR_S));
    stAefAttr.u32FrameDelay = u32AefFrmDelay;
    stAefAttr.bMcSupported  = (u32AefMaxInCh > 2) ? HI_TRUE : HI_FALSE;

    //attach aef channel sound
    s32Ret = HI_MPI_AO_SND_AttachAef(enSound, u32AfltId, &stAefAttr, &u32AefProcAddr);
    if (HI_SUCCESS != s32Ret || 0 == u32AefProcAddr)
    {
        HI_ERR_AO(" Aef[%d] attach sound[%d] failed\n", u32AfltId, enSound);
        goto AEF_ERR_CREATE;
    }

    pstProcItem = (AO_AEF_PROC_ITEM_S*)HI_MEM_Map(u32AefProcAddr, sizeof(AO_AEF_PROC_ITEM_S));
    if (HI_NULL == pstProcItem)
    {
        HI_ERR_AO(" HI_MEM_Map aef proc item failed\n");
        goto AEF_ERR_ATTACH;
    }

    //save proc info
    pstProcItem->enAefType = enAefType;
#ifdef HI_SND_USER_AEF_SUPPORT
    pstProcItem->bEnable  = HI_TRUE;
#else
    pstProcItem->bEnable  = HI_FALSE;
#endif

    pstProcItem->u32AefId  = u32AfltId;
    strncpy(pstProcItem->szName, pstEntry->szName, sizeof(pstProcItem->szName));
    if (enAefType == HI_UNF_SND_AEF_TYPE_SRS3D)
    {
        strncpy(pstProcItem->szDescription, pstAuthEntry->pszCustomerDescription, sizeof(pstProcItem->szDescription));
    }
    else
    {
        snprintf(pstProcItem->szDescription, sizeof(pstProcItem->szDescription), "None");
    }
    pstProcItem->szName[sizeof(pstProcItem->szName) - 1] = '\0';  //TQE

    //save info into aef channel
    memcpy(&pstAefChn->stAefAttr, &stAefAttr, sizeof(AO_AEF_ATTR_S));
    pstAefChn->hHaEffect   = hHaEffect;
    pstAefChn->hEntry      = (HI_HANDLE)pstEntry;
    pstAefChn->enSnd       = enSound;
    pstAefChn->pstProcItem = pstProcItem;
    s_apAefChn[u32AefId]   = (HI_VOID*)pstAefChn;

#ifdef HI_SND_USER_AEF_SUPPORT
    if (0 == pstSndAefAttr->u32AefMask)
    {
        pstSndAefAttr->bAefThreadRun = HI_TRUE;
        s32Ret = pthread_create(&pstSndAefAttr->AefThdInst, HI_NULL, (HI_VOID*)Aef_ProcessThread, (HI_VOID*)enSound);
        if (s32Ret != HI_SUCCESS)
        {
            goto AEF_ERR_UMMAP_PROC;
        }
    }

    pstSndAefAttr->u32AefMask |= 1 << u32AefId;
#endif

    /*
     define of Aef Handle :
     bit31                                                           bit0
       |<----   16bit --------->|<---   8bit    --->|<---  8bit   --->|
       |--------------------------------------------------------------|
       |      HI_MOD_ID_E       |  sub_mod defined  |     chnID       |
       |--------------------------------------------------------------|
    */
    *phAef = (HI_ID_AO << 16) | (HI_ID_AEF << 8) | u32AefId;

    AEF_UNLOCK(&g_AefThreadMutex);
    return HI_SUCCESS;

#ifdef HI_SND_USER_AEF_SUPPORT
AEF_ERR_UMMAP_PROC:
    if (pstProcItem)
        HI_MEM_Unmap(pstProcItem);
#endif

AEF_ERR_ATTACH:
    (HI_VOID)HI_MPI_AO_SND_DetachAef(enSound, u32AfltId, &stAefAttr);

AEF_ERR_CREATE:
    (HI_VOID)pstEntry->AefDestroy(hHaEffect);

AEF_ERR_MALLOC:
    HI_FREE(HI_ID_AO, pstAefChn);

AEF_ERR_MALLOC1:

#ifdef HI_SND_USER_AEF_SUPPORT

    if (0 == pstSndAefAttr->u32AefMask)
        HI_FREE(HI_ID_AO, pstSndAefAttr);

AEF_ERR_RETURE:
#endif

    AEF_UNLOCK(&g_AefThreadMutex);
    return HI_FAILURE;
#else
    return HI_SUCCESS;
#endif
}

HI_S32 HI_MPI_AO_AEF_Destroy(HI_HANDLE hAef)
{
#ifdef __arm__
    HI_S32 s32Ret;
    HI_U32 u32AefId;
    HI_AEF_COMPONENT_S* pstEntry  = HI_NULL;
    AEF_CHANNEL_S*      pstAefChn = HI_NULL;
    HI_U32 u32AfltId;
#ifdef HI_SND_USER_AEF_SUPPORT
    SND_AEF_ATTR_S* pstSndAefAttr = HI_NULL;
#endif

    CHECK_AO_AEF_HANDLE(hAef);
    AEF_LOCK(&g_AefThreadMutex);

    u32AefId  = AEFHandle2ID(hAef);
    if (u32AefId >= AOE_AEF_MAX_CHAN_NUM * 2)
    {
        HI_ERR_AO("invalid aef(0x%x) handle!\n", hAef);
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_ERR_AO_INVALID_PARA;
    }

    pstAefChn = (AEF_CHANNEL_S*)s_apAefChn[u32AefId];
    if (HI_NULL == pstAefChn)
    {
        HI_ERR_AO(" HI_MPI_AO_AEF_Destroy Failed: pstAefChn is null\n");
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_FAILURE;
    }

#ifdef HI_SND_USER_AEF_SUPPORT
    pstSndAefAttr = (SND_AEF_ATTR_S*)s_apSndAefAttr[pstAefChn->enSnd];
    pstSndAefAttr->u32AefMask &= ~(1 << u32AefId);

    if (0 == pstSndAefAttr->u32AefMask)
    {
        pstSndAefAttr->bAefThreadRun = HI_FALSE;
        usleep(2 * AEF_SLEEP_TIME_MS * 1000);
        (HI_VOID)pthread_join(pstSndAefAttr->AefThdInst, HI_NULL);

        if (pstSndAefAttr->pEngRegAddr)
        {
            HI_MEM_Unmap(pstSndAefAttr->pEngRegAddr);
            pstSndAefAttr->pEngRegAddr = HI_NULL;
        }
        if (pstSndAefAttr->pOutBufAddr)
        {
            HI_MEM_Unmap(pstSndAefAttr->pOutBufAddr);
            pstSndAefAttr->pOutBufAddr = HI_NULL;
        }

        if (pstSndAefAttr->pInBufAddr)
        {
            HI_MEM_Unmap(pstSndAefAttr->pInBufAddr);
            pstSndAefAttr->pInBufAddr = HI_NULL;
        }

        HI_FREE(HI_ID_AO, (HI_VOID*)pstSndAefAttr);
        s_apSndAefAttr[pstAefChn->enSnd] = HI_NULL;
    }
#endif

    if (pstAefChn->pstProcItem)
    {
        HI_MEM_Unmap((HI_VOID*)pstAefChn->pstProcItem);
        pstAefChn->pstProcItem = HI_NULL;
    }

    pstEntry = (HI_AEF_COMPONENT_S*)pstAefChn->hEntry;
#ifdef HI_SND_USER_AEF_SUPPORT
    u32AfltId = u32AefId;
#else
    s32Ret = pstEntry->AefGetConfig(pstAefChn->hHaEffect, HI_AEF_GET_AFLTID_CMD, (HI_VOID*)(&u32AfltId));
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" AefGetConfig failed\n");
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_FAILURE;
    }
#endif

    s32Ret = HI_MPI_AO_SND_DetachAef(pstAefChn->enSnd, u32AfltId, &pstAefChn->stAefAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" Aef[%d] detach sound[0x%x] failed\n", pstAefChn->enSnd, u32AfltId);
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_FAILURE;
    }

    s32Ret = pstEntry->AefDestroy(pstAefChn->hHaEffect);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" Aef destory failed\n");
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_FAILURE;
    }

    HI_FREE(HI_ID_AO, pstAefChn);
    s_apAefChn[u32AefId] = HI_NULL;

    AEF_UNLOCK(&g_AefThreadMutex);
#endif
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AO_AEF_SetEnable(HI_HANDLE hAef, HI_BOOL bEnable)
{
#ifdef __arm__
#ifndef HI_SND_USER_AEF_SUPPORT
    HI_S32 s32Ret;
    HI_U32 u32AefId;
    HI_AEF_COMPONENT_S* pstEntry  = HI_NULL;
    AEF_CHANNEL_S* pstAefChn      = HI_NULL;

    CHECK_AO_AEF_HANDLE(hAef);
    AEF_LOCK(&g_AefThreadMutex);

    u32AefId  = AEFHandle2ID(hAef);
    if (u32AefId >= AOE_AEF_MAX_CHAN_NUM * 2)
    {
        HI_ERR_AO("invalid aef(0x%x) handle!\n", hAef);
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_ERR_AO_INVALID_PARA;
    }

    pstAefChn = (AEF_CHANNEL_S*)s_apAefChn[u32AefId];
    pstEntry  = (HI_AEF_COMPONENT_S*)pstAefChn->hEntry;
    s32Ret    = pstEntry->AefSetEnable(pstAefChn->hHaEffect, bEnable);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" Aef setEnable failed\n");
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_FAILURE;
    }

    HI_WARN_AO(" HI_MPI_AO_AEF_SetEnable can not be effect!\n");

    pstAefChn->bEnable = bEnable;
    pstAefChn->pstProcItem->bEnable = bEnable;

    AEF_UNLOCK(&g_AefThreadMutex);
#endif
#endif

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AO_AEF_GetEnable(HI_HANDLE hAef, HI_BOOL* pbEnable)
{
#ifndef HI_SND_USER_AEF_SUPPORT
    HI_U32 u32AefId;
    AEF_CHANNEL_S* pstAefChn     = HI_NULL;

    CHECK_AO_NULL_PTR(pbEnable);
    CHECK_AO_AEF_HANDLE(hAef);
    AEF_LOCK(&g_AefThreadMutex);

    u32AefId = AEFHandle2ID(hAef);
    if (u32AefId >= AOE_AEF_MAX_CHAN_NUM * 2)
    {
        HI_ERR_AO("invalid aef(0x%x) handle!\n", hAef);
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_ERR_AO_INVALID_PARA;
    }

    pstAefChn = (AEF_CHANNEL_S*)s_apAefChn[u32AefId];

    *pbEnable = pstAefChn->bEnable;

    AEF_UNLOCK(&g_AefThreadMutex);
#endif
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AO_AEF_SetParams(HI_HANDLE hAef, HI_U32 u32ParamType, const HI_VOID* pstParms)
{
#ifdef __arm__
    HI_S32 s32Ret;
    HI_U32 u32AefId;
    HI_AEF_COMPONENT_S* pstEntry = HI_NULL;
    AEF_CHANNEL_S* pstAefChn     = HI_NULL;

    CHECK_AO_NULL_PTR(pstParms);
    CHECK_AO_AEF_HANDLE(hAef);
    AEF_LOCK(&g_AefThreadMutex);

    u32AefId = AEFHandle2ID(hAef);
    if (u32AefId >= AOE_AEF_MAX_CHAN_NUM * 2)
    {
        HI_ERR_AO("invalid aef(0x%x) handle!\n", hAef);
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_ERR_AO_INVALID_PARA;
    }

    pstAefChn = (AEF_CHANNEL_S*)s_apAefChn[u32AefId];
    pstEntry = (HI_AEF_COMPONENT_S*)pstAefChn->hEntry;

    if (HI_TRUE == pstAefChn->bEnable)
    {
        HI_ERR_AO(" should stop aef before Set Aef Parameter\n");
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_FAILURE;
    }

    s32Ret = pstEntry->AefSetParameter(pstAefChn->hHaEffect, u32ParamType, pstParms);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" Aef SetParameter failed\n");
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_FAILURE;
    }

    AEF_UNLOCK(&g_AefThreadMutex);
#endif

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AO_AEF_GetParams(HI_HANDLE hAef, HI_U32 u32ParamType, HI_VOID* pstParms)
{
#ifdef __arm__
    HI_S32 s32Ret;
    HI_U32 u32AefId;
    HI_AEF_COMPONENT_S* pstEntry = HI_NULL;
    AEF_CHANNEL_S* pstAefChn     = HI_NULL;

    CHECK_AO_NULL_PTR(pstParms);
    CHECK_AO_AEF_HANDLE(hAef);
    AEF_LOCK(&g_AefThreadMutex);

    u32AefId = AEFHandle2ID(hAef);
    if (u32AefId >= AOE_AEF_MAX_CHAN_NUM * 2)
    {
        HI_ERR_AO("invalid aef(0x%x) handle!\n", hAef);
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_ERR_AO_INVALID_PARA;
    }

    pstAefChn = (AEF_CHANNEL_S*)s_apAefChn[u32AefId];
    pstEntry = (HI_AEF_COMPONENT_S*)pstAefChn->hEntry;

    s32Ret = pstEntry->AefGetParameter(pstAefChn->hHaEffect, u32ParamType, pstParms);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" Aef GetParameter failed\n");
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_FAILURE;
    }

    AEF_UNLOCK(&g_AefThreadMutex);
#endif

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AO_AEF_SetConfig(HI_HANDLE hAef, HI_U32 u32CfgType, const HI_VOID* pstConfig)
{
#ifdef __arm__
    HI_S32 s32Ret;
    HI_U32 u32AefId;
    HI_AEF_COMPONENT_S* pstEntry = HI_NULL;
    AEF_CHANNEL_S* pstAefChn     = HI_NULL;

    CHECK_AO_NULL_PTR(pstConfig);
    CHECK_AO_AEF_HANDLE(hAef);
    AEF_LOCK(&g_AefThreadMutex);

    u32AefId = AEFHandle2ID(hAef);
    if (u32AefId >= AOE_AEF_MAX_CHAN_NUM * 2)
    {
        HI_ERR_AO("invalid aef(0x%x) handle!\n", hAef);
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_ERR_AO_INVALID_PARA;
    }

    pstAefChn = (AEF_CHANNEL_S*)s_apAefChn[u32AefId];
    pstEntry = (HI_AEF_COMPONENT_S*)pstAefChn->hEntry;

    s32Ret = pstEntry->AefSetConfig(pstAefChn->hHaEffect, u32CfgType, pstConfig);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" Aef SetConfig failed\n");
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_FAILURE;
    }

    AEF_UNLOCK(&g_AefThreadMutex);
#endif

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AO_AEF_GetConfig(HI_HANDLE hAef, HI_U32 u32CfgType, HI_VOID* pstConfig)
{
#ifdef __arm__
    HI_S32 s32Ret;
    HI_U32 u32AefId;
    HI_AEF_COMPONENT_S* pstEntry = HI_NULL;
    AEF_CHANNEL_S* pstAefChn     = HI_NULL;

    CHECK_AO_NULL_PTR(pstConfig);
    CHECK_AO_AEF_HANDLE(hAef);
    AEF_LOCK(&g_AefThreadMutex);

    u32AefId = AEFHandle2ID(hAef);
    if (u32AefId >= AOE_AEF_MAX_CHAN_NUM * 2)
    {
        HI_ERR_AO("invalid aef(0x%x) handle!\n", hAef);
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_ERR_AO_INVALID_PARA;
    }

    pstAefChn = (AEF_CHANNEL_S*)s_apAefChn[u32AefId];
    pstEntry = (HI_AEF_COMPONENT_S*)pstAefChn->hEntry;

    s32Ret = pstEntry->AefGetConfig(pstAefChn->hHaEffect, u32CfgType, pstConfig);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO(" Aef GetConfig failed\n");
        AEF_UNLOCK(&g_AefThreadMutex);
        return HI_FAILURE;
    }

    AEF_UNLOCK(&g_AefThreadMutex);
#endif

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
