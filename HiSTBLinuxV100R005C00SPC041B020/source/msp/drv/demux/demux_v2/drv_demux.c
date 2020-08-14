/******************************************************************************

Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_demux.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2009-12-14
Last Modified :
Description   : MPI Layer function declaration for linux
Function List :
History       :
* main\1    2009-12-14   jianglei     init.
******************************************************************************/

#include <linux/kthread.h>

#include "hi_type.h"
#include "hi_module.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"
#include "hi_drv_sys.h"
#include "hi_drv_module.h"
#include "hi_drv_proc.h"
#include "hi_drv_file.h"
#include "hi_kernel_adapt.h"

#include "demux_debug.h"
#include "hi_mpi_demux.h"

#include "hi_drv_demux.h"
#include "drv_demux_ioctl.h"
#include "hal_demux.h"
#include "drv_demux_func.h"
#include "drv_demux.h"
#include "drv_demux_reg.h"
#include "drv_demux_sw.h"
#include "drv_demux_osal.h"
#include "drv_demux_ext.h"

#ifdef DMX_DESCRAMBLER_SUPPORT
#include "hi_drv_descrambler.h"
#include "drv_descrambler.h"
#endif

#include "drv_sync_ext.h"

/***************************** Macro Definition ******************************/
#define DEMUX_NAME                      "HI_DEMUX"

#define DMX_MIN_POOLBUFFER_SIZE         0x80000
#define DMX_MAX_POOLBUFFER_SIZE         0x400000
#define DMX_DEFAULT_POOLBUFFER_SIZE     CFG_HI_DEMUX_POOLBUF_SIZE

/**************************** global variables ****************************/

static DEMUX_EXPORT_FUNC_S s_DmxExportFuncs =
{
    .pfnDmxAcquireEs    = HI_DRV_DMX_AcquireEs,
    .pfnDmxReleaseEs    = HI_DRV_DMX_ReleaseEs,
    .pfnDmxSuspend      = DMX_OsiSuspend,
    .pfnDmxResume       = DMX_OsiResume,
};

uint DmxPoolBufSize  = DMX_DEFAULT_POOLBUFFER_SIZE;
uint DmxBlockSize    = DMX_FQ_COM_BLKSIZE;

module_param(DmxPoolBufSize, uint, S_IRUGO);
module_param(DmxBlockSize, uint, S_IRUGO);

extern SYNC_EXPORT_FUNC_S   *g_pSyncFunc;

/****************************** internal function *****************************/
#ifdef HI_DEMUX_PROC_SUPPORT
static HI_VOID DMX_OsrStopSaveEs(HI_VOID);
static HI_VOID DMX_OsrSaveIPTsStop(HI_VOID);
static HI_VOID DMX_OsrSaveALLTs_Stop(HI_VOID);
static HI_VOID DMX_OsrSaveDmxTs_Stop(HI_VOID);
#endif

static HI_U32 DMXParseBootargs(const HI_CHAR *buf, const HI_CHAR *str)
{
    HI_U32      ret = 0;
    HI_CHAR    *p;

    p = strstr(buf, str);
    if (p)
    {
        p += strlen(str);

        ret = simple_strtol(p, NULL, 0);
    }

    return ret;
}

static HI_VOID DMXGetExternalParameter(HI_U32 *BufSize)
{
    HI_U32 ret;

    ret = DMXParseBootargs(saved_command_line, "DmxPoolBufSize=");
    if (0 != ret)
    {
        if (ret < DMX_MIN_POOLBUFFER_SIZE)
        {
            *BufSize = DMX_MIN_POOLBUFFER_SIZE;
        }
        else if (ret > DMX_MAX_POOLBUFFER_SIZE)
        {
            *BufSize = DMX_MAX_POOLBUFFER_SIZE;
        }
        else
        {
            *BufSize = ret;
        }
    }
}

/**************************** external functions ******************************/

HI_S32  HI_DRV_DMX_BasicInit(HI_VOID)
{
    HI_S32 ret;

    ret = HI_DRV_MODULE_Register(HI_ID_DEMUX, DEMUX_NAME, (HI_VOID*)&s_DmxExportFuncs);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_DEMUX("HI_DRV_MODULE_Register failed\n");

        return ret;
    }

    DMXGetExternalParameter(&DmxPoolBufSize);

    ret = DMX_OsiDeviceInit();
    if (HI_SUCCESS != ret)
    {
        HI_DRV_MODULE_UnRegister(HI_ID_DEMUX);
        HI_ERR_DEMUX("DMX_OsiDeviceInit failed 0x%x\n", ret);

        return ret;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : HI_DRV_DMX_Init
 Description  : DEMUX module initialize function
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
HI_S32 HI_DRV_DMX_Init(HI_VOID)
{
    HI_S32 ret;

    ret = HI_DRV_DMX_BasicInit();
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    ret = GetDmxCluster()->Ops->StartCluster();
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

out:
    return ret;
}

/*****************************************************************************
 Prototype    : HI_DRV_DMX_DeInit
 Description  : DEMUX module exit function
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
HI_VOID HI_DRV_DMX_DeInit(HI_VOID)
{
#ifdef HI_DEMUX_PROC_SUPPORT
    DMX_OsrStopSaveEs();
    DMX_OsrSaveIPTsStop();
    DMX_OsrSaveALLTs_Stop();
    DMX_OsrSaveDmxTs_Stop();
#endif

    DMX_OsiDeviceDeInit();

    HI_DRV_MODULE_UnRegister(HI_ID_DEMUX);
}

/*****************************************************************************
 Prototype    : HI_DRV_DMX_Open
 Description  : demux open, called when open device node
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
HI_S32 HI_DRV_DMX_Open(HI_VOID *file)
{
    HI_S32 ret = HI_FAILURE;

    ((struct file *)file)->private_data = (void *)(unsigned long)task_tgid_nr(current);

    ret = GetDmxCluster()->Ops->StartCluster();
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    ret = HI_DRV_MODULE_GetFunction(HI_ID_SYNC, (HI_VOID**)&g_pSyncFunc);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

out:
    return ret;
}

/*****************************************************************************
 Prototype    : HI_DRV_DMX_Close
 Description  : demux close function, called when close device node
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
HI_S32 HI_DRV_DMX_Close(HI_VOID *file)
{
    GetDmxCluster()->Ops->ReleaseCluster((struct file *)file);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_DMX_GetPoolBufAddr(DMX_MMZ_BUF_S *PoolBuf)
{
    CHECKPOINTER(PoolBuf);

    return DMX_OsiGetPoolBufAddr(PoolBuf);
}

HI_S32 HI_DRV_DMX_GetCapability(HI_UNF_DMX_CAPABILITY_S *Cap)
{
    CHECKPOINTER(Cap);

    return DMX_OsiGetCapability(Cap);
}

HI_S32 HI_DRV_DMX_SetPusi(HI_BOOL bCheckPusi)
{
    HI_BOOL bNoPusiEn = HI_TRUE;
    bNoPusiEn = (bCheckPusi == HI_TRUE)?HI_FALSE:HI_TRUE;

    DMX_OsiSetNoPusiEn(bNoPusiEn);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_DMX_SetTei(HI_UNF_DMX_TEI_SET_S *pstTei)
{
    CHECKPOINTER(pstTei);

    DMX_OsiSetTei(pstTei->u32DemuxID,pstTei->bTei);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_DMX_GetResumeCount(HI_U32 *pCount)
{
    CHECKPOINTER(pCount);

    return DMX_OsiGetResumeCount(pCount);
}

HI_S32 HI_DRV_DMX_TSOPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_TSO_PORT_ATTR_S *PortAttr)
{
    CHECKTSOPORTID(PortId);
    CHECKPOINTER(PortAttr);

    return DMX_OsiTSOPortGetAttr(PortId, PortAttr);
}

HI_S32 HI_DRV_DMX_TSOPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_TSO_PORT_ATTR_S *PortAttr)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;

    CHECKPOINTER(PortAttr);

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        if (DmxSet->TSOPortCnt > 0)
        {
            ret = DMX_OsiTSOPortSetAttr(PortId, PortAttr);
            if (HI_SUCCESS != ret)
            {
                goto out;
            }
        }
    }

out:
    return ret;
}

HI_S32 HI_DRV_DMX_TunerPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_PORT_ATTR_S *PortAttr)
{
    CHECKPOINTER(PortAttr);

    return DMX_OsiTunerPortGetAttr(PortId, PortAttr);
}

HI_S32 HI_DRV_DMX_TunerPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_PORT_ATTR_S *PortAttr)
{
    CHECKPOINTER(PortAttr);

    return DMX_OsiTunerPortSetAttr(PortId, PortAttr);
}

HI_S32 HI_DRV_DMX_RamPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_PORT_ATTR_S *PortAttr)
{
    CHECKPOINTER(PortAttr);

    return DMX_OsiRamPortGetAttr(PortId, PortAttr);
}

HI_S32 HI_DRV_DMX_RamPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_PORT_ATTR_S *PortAttr)
{
    CHECKPOINTER(PortAttr);

    return DMX_OsiRamPortSetAttr(PortId, PortAttr);
}

#if defined(DMX_TAGPORT_CNT) && (DMX_TAGPORT_CNT > 0)
HI_S32  HI_DRV_DMX_TagPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_TAG_ATTR_S *pstAttr)
{
    CHECKTAGPORTID(PortId);
    CHECKPOINTER(pstAttr);

    return DMX_OsiTagPortGetAttr(PortId, pstAttr);
}

HI_S32  HI_DRV_DMX_TagPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_TAG_ATTR_S *pstAttr)
{
    CHECKTAGPORTID(PortId);
    CHECKPOINTER(pstAttr);

    return DMX_OsiTagPortSetAttr(PortId, pstAttr);
}
#endif

HI_S32 HI_DRV_DMX_AttachTunerPort(HI_U32 DmxId, HI_U32 PortId)
{
    return DMX_OsiAttachPort(DmxId, DMX_PORT_MODE_TUNER, PortId);
}

HI_S32 HI_DRV_DMX_AttachRamPort(HI_U32 DmxId, HI_U32 PortId)
{
    return DMX_OsiAttachPort(DmxId, DMX_PORT_MODE_RAM, PortId);
}

HI_S32 HI_DRV_DMX_AttachTagPort(HI_U32 DmxId, HI_U32 PortId)
{
    return DMX_OsiAttachPort(DmxId, DMX_PORT_MODE_TAG, PortId);
}

HI_S32 HI_DRV_DMX_AttachRmxPort(HI_U32 DmxId, HI_U32 PortId)
{
    return DMX_OsiAttachPort(DmxId, DMX_PORT_MODE_RMX, PortId);
}

HI_S32 HI_DRV_DMX_AttachPort(HI_U32 DmxId, DMX_PORT_MODE_E PortMode, HI_U32 PortId)
{
    HI_S32 s32Ret = HI_FAILURE;

    switch(PortMode)
    {
        case DMX_PORT_MODE_TUNER:
        {
            s32Ret = HI_DRV_DMX_AttachTunerPort(DmxId, PortId);
            break;
        }

        case DMX_PORT_MODE_RAM:
        {
            s32Ret = HI_DRV_DMX_AttachRamPort(DmxId, PortId);
            break;
        }

        case DMX_PORT_MODE_TAG:
        {
            s32Ret = HI_DRV_DMX_AttachTagPort(DmxId, PortId);
            break;
        }

        case DMX_PORT_MODE_RMX:
        {
            s32Ret = HI_DRV_DMX_AttachRmxPort(DmxId, PortId);
            break;
        }

        default:
        {
            HI_ERR_DEMUX("Invalid Port Mode(%d).\n", PortMode);

            s32Ret = HI_ERR_DMX_INVALID_PARA;
            break;
        }
    }

    return s32Ret;
}

HI_S32 HI_DRV_DMX_DetachPort(HI_U32 DmxId)
{
    return DMX_OsiDetachPort(DmxId);
}

HI_S32 HI_DRV_DMX_GetPortId(HI_U32 DmxId, DMX_PORT_MODE_E *PortMode, HI_U32 *PortId)
{
    CHECKPOINTER(PortMode);
    CHECKPOINTER(PortId);

    return DMX_OsiGetPortId(DmxId, PortMode, PortId);
}

HI_S32 HI_DRV_DMX_TunerPortGetPacketNum(const HI_U32 PortId, HI_U32 *TsPackCnt, HI_U32 *ErrTsPackCnt)
{
    CHECKPOINTER(TsPackCnt);
    CHECKPOINTER(ErrTsPackCnt);

    return DMX_OsiTunerPortGetPacketNum(PortId, TsPackCnt, ErrTsPackCnt);
}

HI_S32 HI_DRV_DMX_RamPortGetPacketNum(const HI_U32 PortId, HI_U32 *TsPackCnt)
{
    CHECKPOINTER(TsPackCnt);

    return DMX_OsiRamPortGetPacketNum(PortId, TsPackCnt);
}

/*
    application ts buffer, initialized repeatly will return failure.
    repeatly initialize in the same thread, will return success in user mode.
    In this repeatly call it, imply called from different process.
    not supported used in more than one process for one port TS buffer
 */
HI_S32 HI_DRV_DMX_CreateTSBuffer(const HI_U32 PortId, const HI_UNF_DMX_TSBUF_ATTR_S *pstTsBufAttr, DMX_MMZ_BUF_S *TsBuf, const HI_VOID *file)
{
    CHECKPOINTER(pstTsBufAttr);
    CHECKPOINTER(TsBuf);

    return DMX_OsiTsBufferCreate(PortId, pstTsBufAttr, TsBuf);
}

HI_S32 HI_DRV_DMX_DestroyTSBuffer(const HI_U32 PortId)
{
    return DMX_OsiTsBufferDestroy(PortId);
}

HI_S32 HI_DRV_DMX_GetTSBuffer(const HI_U32 PortId, const HI_U32 ReqLen, DMX_DATA_BUF_S *Data, const HI_U32 TimeoutMs)
{
    CHECKPOINTER(Data);

    return DMX_OsiTsBufferGet(PortId, ReqLen, Data, TimeoutMs);
}

HI_S32 HI_DRV_DMX_PushTSBuffer(const HI_U32 PortId, DMX_DATA_BUF_S *pstData)
{
    CHECKPOINTER(pstData);

    return DMX_OsiTsBufferPush(PortId, pstData);
}

HI_S32 HI_DRV_DMX_ReleaseTSBuffer(const HI_U32 PortId, DMX_DATA_BUF_S *pstData)
{
    CHECKPOINTER(pstData);

    return DMX_OsiTsBufferRelease(PortId, pstData);
}

HI_S32 HI_DRV_DMX_PutTSBuffer(const HI_U32 PortId, const HI_U32 DataLen, const HI_U32 StartPos)
{
    return DMX_OsiTsBufferPut(PortId, DataLen, StartPos);
}

HI_S32 HI_DRV_DMX_ResetTSBuffer(const HI_U32 PortId)
{
    return DMX_OsiTsBufferReset(PortId);
}

HI_S32 HI_DRV_DMX_GetTSBufferStatus(const HI_U32 PortId, HI_UNF_DMX_TSBUF_STATUS_S *Status)
{
    CHECKPOINTER(Status);

    return DMX_OsiTsBufferGetStatus(PortId, Status);
}

HI_S32 HI_DRV_DMX_CreateChannel(
        HI_U32                  u32DmxId,
        HI_UNF_DMX_CHAN_ATTR_S *pstChAttr,
        HI_HANDLE              *phChannel,
        DMX_MMZ_BUF_S          *pstChBuf,
        HI_VOID                  *file
    )
{
    HI_S32  ret;
    HI_U32  ChanId;

    CHECKPOINTER(pstChAttr);
    CHECKPOINTER(phChannel);
    CHECKPOINTER(pstChBuf);

    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & pstChAttr->enOutputMode)
    {
        HI_ERR_DEMUX("Create the rec channel with HI_DRV_DMX_CreateChannel has deprecated by HI_DRV_DMX_AddRecPid.\n");

        return HI_ERR_DMX_INVALID_PARA;
    }

    ret = DMX_OsiCreateChannel(u32DmxId, pstChAttr, pstChBuf, &ChanId);
    if (HI_SUCCESS == ret)
    {
        *phChannel = DMX_CHANHANDLE(u32DmxId, ChanId);
    }

    return ret;
}

HI_S32 HI_DRV_DMX_CreateChannelWithPID(
        HI_U32                  u32DmxId,
        HI_U32                  u32Pid,
        HI_UNF_DMX_CHAN_ATTR_S *pstChAttr,
        HI_HANDLE              *phChannel,
        DMX_MMZ_BUF_S          *pstChBuf,
        HI_VOID                  *file
    )
{
    HI_S32  ret;
    HI_U32  ChanId;

    CHECKPOINTER(pstChAttr);
    CHECKPOINTER(phChannel);
    CHECKPOINTER(pstChBuf);

    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & pstChAttr->enOutputMode)
    {
        HI_ERR_DEMUX("Create the rec channel with HI_DRV_DMX_CreateChannelWithPID has deprecated by HI_DRV_DMX_AddRecPid.\n");

        return HI_ERR_DMX_INVALID_PARA;
    }

    ret = DMX_OsiGetChannelId(u32DmxId, u32Pid, &ChanId);
    if (HI_SUCCESS != ret)
    {
        ret = DMX_OsiCreateChannel(u32DmxId, pstChAttr, pstChBuf, &ChanId);
        if (HI_SUCCESS != ret)
        {
            goto out;
        }

        ret = DMX_OsiSetChannelPid(u32DmxId, ChanId, u32Pid);
        if (HI_SUCCESS != ret)
        {
            DMX_OsiDestroyPlyChannel(u32DmxId, ChanId);

            goto out;
        }
    }
    else
    {
        ret = DMX_OsiAddChannelFunc(u32DmxId, ChanId, u32Pid, pstChAttr, pstChBuf);
        if (HI_SUCCESS != ret)
        {
            goto out;
        }
    }

    *phChannel = DMX_CHANHANDLE(u32DmxId, ChanId);

out:
    return ret;
}

HI_S32 HI_DRV_DMX_DestroyChannel(HI_HANDLE hChannel)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiDestroyPlyChannel(DmxId, ChanId);
}

HI_S32 HI_DRV_DMX_GetChannelAttr(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_ATTR_S *pstChAttr)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiGetChannelAttr(DmxId, ChanId, pstChAttr);
}

HI_S32 HI_DRV_DMX_SetChannelAttr(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_ATTR_S *pstChAttr)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiSetChannelAttr(DmxId, ChanId, pstChAttr);
}

HI_S32 HI_DRV_DMX_SetChannelPID(HI_HANDLE hChannel, HI_U32 u32Pid)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiSetChannelPid(DmxId, ChanId, u32Pid);
}

HI_S32 HI_DRV_DMX_GetChannelPID(HI_HANDLE hChannel, HI_U32 *pu32Pid)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);
    CHECKPOINTER(pu32Pid);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiGetChannelPid(DmxId, ChanId, pu32Pid);
}

HI_S32 HI_DRV_DMX_OpenChannel(HI_HANDLE hChannel)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiOpenPlyChannel(DmxId, ChanId);
}

HI_S32 HI_DRV_DMX_CloseChannel(HI_HANDLE hChannel)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiClosePlyChannel(DmxId, ChanId);
}

HI_S32 HI_DRV_DMX_GetChannelStatus(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_STATUS_S *pstStatus)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiGetChannelStatus(DmxId, ChanId, &pstStatus->enChanStatus);
}

HI_S32 HI_DRV_DMX_GetChannelHandle(HI_U32 DmxId, HI_U32 Pid, HI_HANDLE *ChanHandle)
{
    HI_S32  ret;
    HI_U32  ChanId;

    CHECKPOINTER(ChanHandle);

    ret = DMX_OsiGetChannelId(DmxId, Pid, &ChanId);
    if (HI_SUCCESS == ret)
    {
        *ChanHandle = DMX_CHANHANDLE(DmxId, ChanId);
    }

    return ret;
}

HI_S32 HI_DRV_DMX_GetFreeChannelCount (HI_U32 DmxId, HI_U32 *FreeCount)
{
    CHECKPOINTER(FreeCount);

    return DMX_OsiGetFreeChannelNum(DmxId, FreeCount);
}

HI_S32 HI_DRV_DMX_GetScrambledFlag(HI_HANDLE hChannel, HI_UNF_DMX_SCRAMBLED_FLAG_E *ScrambleFlag)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);
    CHECKPOINTER(ScrambleFlag);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiGetChannelScrambleFlag(DmxId, ChanId, ScrambleFlag);
}

HI_S32 HI_DRV_DMX_SetChannelEosFlag(HI_HANDLE hChannel)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiSetChannelEosFlag(DmxId, ChanId);
}

HI_S32 HI_DRV_DMX_GetChannelTsCount(HI_HANDLE hChannel, HI_U32 *pu32TsCnt)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);
    CHECKPOINTER(pu32TsCnt);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiGetChannelTsCnt(DmxId, ChanId, pu32TsCnt);
}

HI_S32 HI_DRV_DMX_SetChannelCCRepeat(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_CC_REPEAT_SET_S *pstChCCReaptSet)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiSetChannelCCRepeat(DmxId, ChanId, pstChCCReaptSet);
}

HI_S32 HI_DRV_DMX_CreateFilter(HI_U32 DmxId, HI_UNF_DMX_FILTER_ATTR_S *FilterAttr, HI_HANDLE *Filter, HI_VOID *file)
{
    HI_S32  ret;
    HI_U32  FilterId;

    CHECKPOINTER(FilterAttr);
    CHECKPOINTER(Filter);

    ret = DMX_OsiNewFilter(DmxId, &FilterId);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    ret = DMX_OsiSetFilterAttr(DmxId, FilterId, FilterAttr);
    if (HI_SUCCESS != ret)
    {
        DMX_OsiDeleteFilter(DmxId, FilterId);

        return ret;
    }

    *Filter = DMX_FLTHANDLE(DmxId, FilterId);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_DMX_DestroyFilter(HI_HANDLE Filter)
{
    HI_U32  FltId, DmxId;

    DMX_CHECK_FLTHANDLE(Filter);

    DmxId = DMX_DMXID(Filter);
    FltId = DMX_FLTID(Filter);

    return DMX_OsiDeleteFilter(DmxId, FltId);
}

HI_S32 HI_DRV_DMX_DestroyAllFilter(HI_HANDLE Channel)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32  DmxId;
    Dmx_Set_S *DmxSet = HI_NULL;
    HI_U32 FilterId;

    DMX_CHECK_CHANHANDLE(Channel);

    DmxId = DMX_DMXID(Channel);

    DmxSet = GetDmxSetByDmxid(DmxId);
    if (!DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    mutex_lock(&DmxSet->LockAllFlt);

    for_each_set_bit(FilterId, DmxSet->FilterBitmap, DmxSet->DmxFilterCnt)
    {
        HI_U32 ChanId;

        ret = DMX_OsiGetFilterChannel(DmxId, FilterId, &ChanId);
        if (HI_SUCCESS == ret)
        {
            if (ChanId == DMX_CHANID(Channel))
            {
                ret = DMX_OsiDeleteFilter(DmxId, FilterId);
                if (HI_SUCCESS != ret)
                {
                    HI_ERR_DEMUX("DMX_OsiDeleteFilter failed:0x%x\n", ret);
                }
            }
        }
    }

    mutex_unlock(&DmxSet->LockAllFlt);

    ret = HI_SUCCESS;

out:
    return ret;
}

HI_S32 HI_DRV_DMX_SetFilterAttr(HI_HANDLE Filter, HI_UNF_DMX_FILTER_ATTR_S *FilterAttr)
{
    HI_U32  FltId, DmxId;

    DMX_CHECK_FLTHANDLE(Filter);
    CHECKPOINTER(FilterAttr);

    DmxId = DMX_DMXID(Filter);
    FltId = DMX_FLTID(Filter);

    return DMX_OsiSetFilterAttr(DmxId, FltId, FilterAttr);
}

HI_S32 HI_DRV_DMX_GetFilterAttr(HI_HANDLE Filter, HI_UNF_DMX_FILTER_ATTR_S *FilterAttr)
{
    HI_U32  FltId, DmxId;

    DMX_CHECK_FLTHANDLE(Filter);
    CHECKPOINTER(FilterAttr);

    DmxId = DMX_DMXID(Filter);
    FltId = DMX_FLTID(Filter);

    return DMX_OsiGetFilterAttr(DmxId, FltId, FilterAttr);
}

HI_S32 HI_DRV_DMX_AttachFilter(HI_HANDLE Filter, HI_HANDLE Channel)
{
    HI_U32  FltId, DmxId, ChanId;

    DMX_CHECK_FLTHANDLE(Filter);
    DMX_CHECK_CHANHANDLE(Channel);

    FltId = DMX_FLTID(Filter);
    DmxId = DMX_DMXID(Filter);
    ChanId = DMX_CHANID(Channel);

    return DMX_OsiAttachFilter(DmxId, FltId, ChanId);
}

HI_S32 HI_DRV_DMX_DetachFilter(HI_HANDLE Filter, HI_HANDLE Channel)
{
    HI_U32  FltId, DmxId, ChanId;

    DMX_CHECK_FLTHANDLE(Filter);
    DMX_CHECK_CHANHANDLE(Channel);

    FltId = DMX_FLTID(Filter);
    DmxId = DMX_DMXID(Filter);
    ChanId = DMX_CHANID(Channel);

    return DMX_OsiDetachFilter(DmxId, FltId, ChanId);
}

HI_S32 HI_DRV_DMX_GetFilterChannelHandle(HI_HANDLE Filter, HI_HANDLE *Channel)
{
    HI_S32 ret;
    HI_U32  FltId, DmxId, ChanId;

    DMX_CHECK_FLTHANDLE(Filter);
    CHECKPOINTER(Channel);

    DmxId = DMX_DMXID(Filter);
    FltId = DMX_FLTID(Filter);

    ret = DMX_OsiGetFilterChannel(DmxId, FltId, &ChanId);
    if (HI_SUCCESS == ret)
    {
        *Channel = DMX_CHANHANDLE(DmxId, ChanId);
    }

    return ret;
}

HI_S32 HI_DRV_DMX_GetFreeFilterCount(HI_U32 DmxId, HI_U32 *FreeCount)
{
    CHECKPOINTER(FreeCount);

    return DMX_OsiGetFreeFilterNum(DmxId, FreeCount);
}

HI_S32 HI_DRV_DMX_GetDataHandle(HI_U32 *pu32ValidChannel, HI_U32 *pu32ValidNum, HI_U32 u32TimeOutMs)
{
    CHECKPOINTER(pu32ValidChannel);
    CHECKPOINTER(pu32ValidNum);

    if (0 == *pu32ValidNum)
    {
        HI_ERR_DEMUX("u32WatchNum == 0!\n");
        return HI_ERR_DMX_INVALID_PARA;
    }

    return DMX_OsiGetDataFlag(pu32ValidChannel, pu32ValidNum, u32TimeOutMs);
}

HI_S32 HI_DRV_DMX_SelectDataHandle(HI_U32 *pu32WatchChannel, HI_U32 u32WatchNum, HI_U32 *pu32ValidChannel, HI_U32 *pu32ValidNum, HI_U32 u32TimeOutMs)
{
    CHECKPOINTER(pu32WatchChannel);
    CHECKPOINTER(pu32ValidChannel);
    CHECKPOINTER(pu32ValidNum);

    if (0 == u32WatchNum)
    {
        HI_ERR_DEMUX("u32WatchNum == 0!\n");
        return HI_ERR_DMX_INVALID_PARA;
    }

    return DMX_OsiSelectDataFlag(pu32WatchChannel, u32WatchNum, pu32ValidChannel, pu32ValidNum, u32TimeOutMs);
}

/*pstBuf is the address of user mode, need to copy out buf info
   pu8Buf in pstBuf need to return physical address to user mode
 */
HI_S32 HI_DRV_DMX_AcquireBuf(HI_HANDLE hChannel, HI_U32 u32AcquireNum,
                          HI_U32 *pu32AcquiredNum, DMX_UserMsg_S *pstBuf,
                          HI_U32 u32TimeOutMs)
{
    HI_S32  ret;
    HI_U32  DmxId, ChanId;
    HI_U32  i = 0;

    DMX_CHECK_CHANHANDLE(hChannel);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    *pu32AcquiredNum = 0;

    ret = DMX_OsiReadDataRequset(DmxId, ChanId, u32AcquireNum, pu32AcquiredNum, pstBuf, u32TimeOutMs);
    if (HI_SUCCESS != ret)
    {
        HI_WARN_DEMUX("DMX_OsiReadDataRequset failed:%x.\n",ret);
        return ret;
    }

    for (i = 0; i < *pu32AcquiredNum; i++)
    {
        if (DMX_OsiGetChType(DmxId, ChanId) != HI_UNF_DMX_CHAN_TYPE_PES)
        {
            pstBuf[i].enDataType = HI_UNF_DMX_DATA_TYPE_WHOLE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_DMX_ReleaseBuf(HI_HANDLE hChannel, HI_U32 u32ReleaseNum, DMX_UserMsg_S *pstBuf)
{
    HI_S32 ret;
    HI_U32  DmxId, ChanId;

    DMX_CHECK_CHANHANDLE(hChannel);
    CHECKPOINTER(pstBuf);

    if (0 == u32ReleaseNum)
    {
        HI_ERR_DEMUX("u32ReleaseNum == 0!\n");
        return HI_ERR_DMX_INVALID_PARA;
    }

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    ret = DMX_OsiReleaseReadData(DmxId, ChanId, u32ReleaseNum, pstBuf);
    if (HI_SUCCESS != ret)
    {
        HI_WARN_DEMUX("DMX_OsiReleaseReadData failed: ChId=%d, ret=0x%x\n", ChanId, ret);
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_DMX_PeekBuf(HI_HANDLE hChannel, HI_U32 u32PeekLen,DMX_UserMsg_S *pstBuf)
{
    HI_S32  ret;
    HI_U32  DmxId, ChanId;
    DMX_UserMsg_S stReqBufTmp[16];

    DMX_CHECK_CHANHANDLE(hChannel);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    memset(stReqBufTmp,0x0,sizeof(stReqBufTmp));

    ret = DMX_OsiPeekDataRequest(DmxId, ChanId, u32PeekLen, stReqBufTmp);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    memcpy(pstBuf,stReqBufTmp,sizeof(DMX_UserMsg_S));
    if (pstBuf->u32MsgLen > u32PeekLen)
    {
        pstBuf->u32MsgLen = u32PeekLen;
    }

    return HI_SUCCESS;

}

HI_S32 HI_DRV_DMX_CreatePcrChannel(const HI_U32 DmxId, HI_U32 *PcrHandle, const HI_VOID *file)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 PcrId ;

    CHECKPOINTER(PcrHandle);

    ret = DMX_OsiPcrChannelCreate(DmxId, &PcrId);
    if (HI_SUCCESS == ret)
    {
        *PcrHandle = DMX_PCRHANDLE(DmxId, PcrId);
    }

    return ret;
}

HI_S32 HI_DRV_DMX_DestroyPcrChannel(const HI_U32 PcrHandle)
{
    HI_U32  PcrId, DmxId;

    DMX_CHECK_PCRHANDLE(PcrHandle);

    DmxId = DMX_DMXID(PcrHandle);
    PcrId = DMX_PCRID(PcrHandle);

    return DMX_OsiPcrChannelDestroy(DmxId, PcrId);
}

HI_S32 HI_DRV_DMX_PcrPidSet(const HI_U32 PcrHandle, const HI_U32 PcrPid)
{
    HI_U32  PcrId, DmxId;

    DMX_CHECK_PCRHANDLE(PcrHandle);

    DmxId = DMX_DMXID(PcrHandle);
    PcrId = DMX_PCRID(PcrHandle);

    return DMX_OsiPcrChannelSetPid(DmxId, PcrId, PcrPid);
}

HI_S32 HI_DRV_DMX_PcrPidGet(const HI_U32 PcrHandle, HI_U32 *PcrPid)
{
    HI_U32  PcrId, DmxId;

    DMX_CHECK_PCRHANDLE(PcrHandle);
    CHECKPOINTER(PcrPid);

    DmxId = DMX_DMXID(PcrHandle);
    PcrId = DMX_PCRID(PcrHandle);

    return DMX_OsiPcrChannelGetPid(DmxId, PcrId, PcrPid);
}

HI_S32 HI_DRV_DMX_PcrScrGet(const HI_U32 PcrHandle, HI_U64 *PcrValue, HI_U64 *ScrValue)
{
    HI_U32  PcrId, DmxId;

    DMX_CHECK_PCRHANDLE(PcrHandle);
    CHECKPOINTER(PcrValue);
    CHECKPOINTER(ScrValue);

    DmxId = DMX_DMXID(PcrHandle);
    PcrId = DMX_PCRID(PcrHandle);

    return DMX_OsiPcrChannelGetClock(DmxId, PcrId, PcrValue, ScrValue);
}

HI_S32 HI_DRV_DMX_PcrSyncAttach(const HI_U32 PcrHandle, const HI_U32 SyncHandle)
{
    HI_U32  PcrId, DmxId;

    DMX_CHECK_PCRHANDLE(PcrHandle);

    DmxId = DMX_DMXID(PcrHandle);
    PcrId = DMX_PCRID(PcrHandle);

    return DMX_OsiPcrChannelAttachSync(DmxId, PcrId, SyncHandle);
}

HI_S32 HI_DRV_DMX_PcrSyncDetach(const HI_U32 PcrHandle)
{
    HI_U32  PcrId, DmxId;

    DMX_CHECK_PCRHANDLE(PcrHandle);

    DmxId = DMX_DMXID(PcrHandle);
    PcrId = DMX_PCRID(PcrHandle);

    return DMX_OsiPcrChannelDetachSync(DmxId, PcrId);
}

HI_S32 HI_DRV_DMX_GetPESBufferStatus(HI_HANDLE ChanHandle, HI_MPI_DMX_BUF_STATUS_S *BufStatus)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(ChanHandle);
    CHECKPOINTER(BufStatus);

    DmxId = DMX_DMXID(ChanHandle);
    ChanId = DMX_CHANID(ChanHandle);

    return DMX_OsiGetChanBufStatus(DmxId, ChanId, BufStatus);
}

HI_S32 HI_DRV_DMX_AcquireEs(HI_HANDLE hChannel, DMX_Stream_S *pEsBuf)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);
    CHECKPOINTER(pEsBuf);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiReadEsRequest(DmxId, ChanId, pEsBuf);
}

HI_S32 HI_DRV_DMX_ReleaseEs(HI_HANDLE hChannel, DMX_Stream_S *pEsBuf)
{
    HI_U32  ChanId, DmxId;

    DMX_CHECK_CHANHANDLE(hChannel);
    CHECKPOINTER(pEsBuf);

    DmxId = DMX_DMXID(hChannel);
    ChanId = DMX_CHANID(hChannel);

    return DMX_OsiReleaseReadEs(DmxId, ChanId, pEsBuf);
}

HI_S32 HI_DRV_DMX_CreateRecChn(
        HI_UNF_DMX_REC_ATTR_S  *RecAttr,
        HI_HANDLE              *RecHandle,
        DMX_MMZ_BUF_S    *RecBuf,
        HI_VOID                *file
    )
{
    HI_S32  ret;
    HI_U32  RecId;
    DMX_REC_TIMESTAMP_MODE_E enRecTimeStamp = DMX_REC_TIMESTAMP_NONE;

    CHECKPOINTER(RecAttr);
    CHECKPOINTER(RecHandle);
    CHECKPOINTER(RecBuf);

    ret = DMX_DRV_REC_CreateChannel(RecAttr, enRecTimeStamp,&RecId, RecBuf);
    if (HI_SUCCESS == ret)
    {
        *RecHandle = DMX_RECHANDLE(RecAttr->u32DmxId, RecId);
    }

    return ret;
}

HI_S32 HI_DRV_DMX_DestroyRecChn(HI_HANDLE RecHandle)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    /*
     * we must del all rec pid channel before DMX_DRV_REC_DestroyChannel release its oq and fq.
     * otherwise rec pid channel maybe still use the oq and fq which owned by other new created instance.
     */
    ret = DMX_DRV_REC_DelAllRecPid(DmxId, RecId);
    if (HI_SUCCESS == ret)
    {
        ret = DMX_DRV_REC_DestroyChannel(DmxId, RecId);
    }

    return ret;
}

HI_S32 HI_DRV_DMX_AddRecPid(HI_HANDLE RecHandle, HI_U32 Pid, HI_HANDLE *ChanHandle, HI_VOID *file)
{
    HI_S32 ret;
    HI_U32  RecId, DmxId, ChanId;

    DMX_CHECK_RECHANDLE(RecHandle);
    CHECKPOINTER(ChanHandle);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    ret = DMX_DRV_REC_AddRecPid(DmxId, RecId, Pid, &ChanId);
    if (HI_SUCCESS == ret)
    {
        *ChanHandle = DMX_CHANHANDLE2(DmxId, ChanId, HI_UNF_DMX_CHAN_OUTPUT_MODE_REC);
    }

    return ret;
}

HI_S32 HI_DRV_DMX_DelRecPid(HI_HANDLE RecHandle, HI_HANDLE ChanHandle)
{
    HI_U32  RecId, DmxId, ChanId;

    DMX_CHECK_RECHANDLE(RecHandle);
    DMX_CHECK_CHANHANDLE(ChanHandle);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);
    ChanId = DMX_CHANID(ChanHandle);

    return DMX_DRV_REC_DelRecPid(DmxId, RecId, ChanId);
}

HI_S32 HI_DRV_DMX_DelAllRecPid(HI_HANDLE RecHandle)
{
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    return DMX_DRV_REC_DelAllRecPid(DmxId, RecId);
}

HI_S32 HI_DRV_DMX_GetRecTsCnt(HI_HANDLE RecHandle,HI_U32* TSCnt)
{
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    return DMX_DRV_REC_GetTsCnt(DmxId, RecId, TSCnt);
}


HI_S32 HI_DRV_DMX_AddExcludeRecPid(HI_HANDLE RecHandle, HI_U32 Pid)
{
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    return DMX_DRV_REC_AddExcludeRecPid(DmxId, RecId, Pid);
}

HI_S32 HI_DRV_DMX_DelExcludeRecPid(HI_HANDLE RecHandle, HI_U32 Pid)
{
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    return DMX_DRV_REC_DelExcludeRecPid(DmxId, RecId, Pid);
}

HI_S32 HI_DRV_DMX_DelAllExcludeRecPid(HI_HANDLE RecHandle)
{
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    return DMX_DRV_REC_DelAllExcludeRecPid(DmxId, RecId);
}

HI_S32 HI_DRV_DMX_StartRecChn(HI_HANDLE RecHandle)
{
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    return DMX_DRV_REC_StartRecChn(DmxId, RecId);
}

HI_S32 HI_DRV_DMX_StopRecChn(HI_HANDLE RecHandle)
{
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    return DMX_DRV_REC_StopRecChn(DmxId, RecId);
}

HI_S32 HI_DRV_DMX_AcquireRecData(HI_HANDLE RecHandle, HI_MPI_DMX_REC_DATA_S *RecData, HI_U32 Timeout)
{
    HI_S32  ret;
    HI_U32  PhyAddr;
    HI_U32  Len;
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);
    CHECKPOINTER(RecData);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    ret = DMX_DRV_REC_AcquireRecData(DmxId, RecId, &PhyAddr, HI_NULL, &Len, Timeout);
    if (HI_SUCCESS == ret)
    {
        RecData->u32DataPhyAddr = PhyAddr;
        RecData->u32Len         = Len;
    }

    return ret;
}

HI_S32 HI_DRV_DMX_ReleaseRecData(HI_HANDLE RecHandle, const HI_MPI_DMX_REC_DATA_S *RecData)
{
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);
    CHECKPOINTER(RecData);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    return DMX_DRV_REC_ReleaseRecData(DmxId, RecId, RecData->u32DataPhyAddr, RecData->u32Len);
}

HI_S32 HI_DRV_DMX_AcquireRecIndex(HI_HANDLE RecHandle, HI_MPI_DMX_REC_INDEX_S *RecIndex, HI_U32 Timeout)
{
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);
    CHECKPOINTER(RecIndex);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    return DMX_DRV_REC_AcquireRecIndex(DmxId, RecId, RecIndex, Timeout);
}

HI_S32 HI_DRV_DMX_AcquireRecDataIndex(HI_HANDLE RecHandle, HI_MPI_DMX_REC_DATA_INDEX_S *RecDataIndex)
{
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);
    CHECKPOINTER(RecDataIndex);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    return DMX_DRV_REC_AcquireRecDataIndex(DmxId, RecId, RecDataIndex);
}

HI_S32 HI_DRV_DMX_ReleaseRecDataIndex(HI_HANDLE RecHandle, HI_MPI_DMX_REC_DATA_INDEX_S *RecDataIndex)
{
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);
    CHECKPOINTER(RecDataIndex);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    return DMX_DRV_REC_ReleaseRecDataIndex(DmxId, RecId, RecDataIndex);
}

HI_S32 HI_DRV_DMX_GetRecBufferStatus(HI_HANDLE RecHandle, HI_UNF_DMX_RECBUF_STATUS_S *BufStatus)
{
    HI_U32  RecId, DmxId;

    DMX_CHECK_RECHANDLE(RecHandle);
    CHECKPOINTER(BufStatus);

    RecId = DMX_RECID(RecHandle);
    DmxId = DMX_DMXID(RecHandle);

    return DMX_DRV_REC_GetRecBufferStatus(DmxId, RecId, BufStatus);
}

HI_S32 HI_DRV_RMX_Create(HI_MPI_RMX_ATTR_S *Attr, HI_HANDLE *Handle)
{
    HI_S32  ret;
    HI_U32  RmxId, ChanId;

    CHECKPOINTER(Attr);
    CHECKPOINTER(Handle);

    ret = RMX_OsiCreate(Attr, &ChanId);
    if (HI_SUCCESS == ret)
    {
        RmxId = Attr->enOutPortId - HI_MPI_DMX_PORT_RMX_0;

        *Handle = RMX_HANDLE(RmxId, ChanId);
    }

    return ret;
}

HI_S32 HI_DRV_RMX_Destroy(HI_HANDLE Handle)
{
    HI_U32  RmxId, ChanId;

    RMX_CHECK_HANDLE(Handle);

    RmxId = RMX_RMXID(Handle);
    ChanId = RMX_ID(Handle);

    return RMX_OsiDestroy(RmxId, ChanId);
}

HI_S32 HI_DRV_RMX_GetAttr(HI_HANDLE Handle, HI_MPI_RMX_ATTR_S *Attr)
{
    HI_U32  RmxId, ChanId;

    RMX_CHECK_HANDLE(Handle);

    RmxId = RMX_RMXID(Handle);
    ChanId = RMX_ID(Handle);

    return RMX_OsiGetAttr(RmxId, ChanId, Attr);
}

HI_S32 HI_DRV_RMX_SetAttr(HI_HANDLE Handle, HI_MPI_RMX_ATTR_S *Attr)
{
    HI_U32  RmxId, ChanId;

    RMX_CHECK_HANDLE(Handle);

    RmxId = RMX_RMXID(Handle);
    ChanId = RMX_ID(Handle);

    return RMX_OsiSetAttr(RmxId, ChanId, Attr);
}

HI_S32 HI_DRV_RMX_Start(HI_HANDLE Handle)
{
    HI_U32  RmxId, ChanId;

    RMX_CHECK_HANDLE(Handle);

    RmxId = RMX_RMXID(Handle);
    ChanId = RMX_ID(Handle);

    return RMX_OsiStart(RmxId, ChanId);
}

HI_S32 HI_DRV_RMX_Stop(HI_HANDLE Handle)
{
     HI_U32  RmxId, ChanId;

    RMX_CHECK_HANDLE(Handle);

    RmxId = RMX_RMXID(Handle);
    ChanId = RMX_ID(Handle);

    return RMX_OsiStop(RmxId, ChanId);
}

HI_S32 HI_DRV_RMX_AddPump(HI_HANDLE RmxHandle, HI_MPI_RMX_PUMP_ATTR_S *Attr, HI_HANDLE *PumpHandle)
{
    HI_S32  ret;
    HI_U32  RmxId, ChanId, PumpId;

    RMX_CHECK_HANDLE(RmxHandle);
    CHECKPOINTER(Attr);
    CHECKPOINTER(PumpHandle);

    RmxId = RMX_RMXID(RmxHandle);
    ChanId = RMX_ID(RmxHandle);

    ret = RMX_OsiAddPump(RmxId, ChanId, Attr, &PumpId);
    if (HI_SUCCESS == ret)
    {
        *PumpHandle = RMX_PUMPHANDLE(RmxId, ChanId, PumpId);
    }

    return ret;
}

HI_S32 HI_DRV_RMX_DelPump(HI_HANDLE PumpHandle)
{
    HI_U32  RmxId, ChanId, PumpId;

    RMX_CHECK_PUMPHANDLE(PumpHandle);

    RmxId = RMX_RMXID(PumpHandle);
    ChanId = RMX_ID(PumpHandle);
    PumpId = RMX_PUMPID(PumpHandle);

    return RMX_OsiDelPump(RmxId, ChanId, PumpId);
}

HI_S32 HI_DRV_RMX_GetPumpDefaultAttr(HI_MPI_RMX_PUMP_ATTR_S *Attr)
{
    CHECKPOINTER(Attr);

    return RMX_OsiGetPumpDefaultAttr(Attr);
}

HI_S32 HI_DRV_RMX_GetPumpAttr(HI_HANDLE PumpHandle, HI_MPI_RMX_PUMP_ATTR_S *Attr)
{
    HI_U32  RmxId, ChanId, PumpId;

    RMX_CHECK_PUMPHANDLE(PumpHandle);
    CHECKPOINTER(Attr);

    RmxId = RMX_RMXID(PumpHandle);
    ChanId = RMX_ID(PumpHandle);
    PumpId = RMX_PUMPID(PumpHandle);

    return RMX_OsiGetPumpAttr(RmxId, ChanId, PumpId, Attr);
}

HI_S32 HI_DRV_RMX_SetPumpAttr(HI_HANDLE PumpHandle, HI_MPI_RMX_PUMP_ATTR_S *Attr)
{
    HI_U32  RmxId, ChanId, PumpId;

    RMX_CHECK_PUMPHANDLE(PumpHandle);
    CHECKPOINTER(Attr);

    RmxId = RMX_RMXID(PumpHandle);
    ChanId = RMX_ID(PumpHandle);
    PumpId = RMX_PUMPID(PumpHandle);

    return RMX_OsiSetPumpAttr(RmxId, ChanId, PumpId, Attr);
}

#ifdef HI_DEMUX_PROC_SUPPORT
#define DMX_FILE_NAME_LEN   (256)
static HI_U32 SaveEsFlag = 0;

/*
 * upper caller has hold channel instance.
 */
HI_VOID DMX_OsrSaveEs(DMX_ChanInfo_S *ChanInfo, DMX_Stream_S *EsData)
{
    struct tm now;

    if (0 == SaveEsFlag)
    {
        return;
    }

    if (HI_UNF_DMX_SECURE_MODE_TEE == ChanInfo->ChanSecure)
    {
        if (HI_UNF_DMX_CHAN_TYPE_VID == ChanInfo->ChanType)
        {
            HI_ERR_DEMUX("not support saving vid es data from secure buffer.\n");
            return;
        }
    }

    if (EsData->u32BufLen)
    {
        if (HI_UNF_DMX_CHAN_TYPE_VID == ChanInfo->ChanType || HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType)
        {
            if (HI_NULL == ChanInfo->SaveEsHandle)
            {
                HI_CHAR str[DMX_FILE_NAME_LEN]  = {0};
                HI_CHAR path[DMX_FILE_NAME_LEN] = {0};

                if (HI_SUCCESS != HI_DRV_FILE_GetStorePath(path, DMX_FILE_NAME_LEN))
                {
                    HI_ERR_DEMUX("get path failed\n");

                    return;
                }

                time_to_tm(get_seconds(), 0, &now);

                if (HI_UNF_DMX_CHAN_TYPE_VID == ChanInfo->ChanType)
                {
                    snprintf(str, sizeof(str),"%s/dmx_vid_%u-%02u_%02u_%02u.es", path, ChanInfo->ChanId, now.tm_hour, now.tm_min, now.tm_sec);
                }
                else
                {
                    snprintf(str,sizeof(str), "%s/dmx_aud_%u-%02u_%02u_%02u.es", path, ChanInfo->ChanId, now.tm_hour, now.tm_min, now.tm_sec);
                }

                ChanInfo->SaveEsHandle = HI_DRV_FILE_Open(str, 1);
                if (!ChanInfo->SaveEsHandle)
                {
                    HI_ERR_DEMUX("open %s error\n", str);

                    return;
                }
            }

            HI_DRV_FILE_Write(ChanInfo->SaveEsHandle, EsData->pu8BufVirAddr, EsData->u32BufLen);
        }
    }
}

static HI_S32 DMX_OsrStartSaveEs(HI_VOID)
{
    HI_CHAR path[DMX_FILE_NAME_LEN] = {0};

    if (SaveEsFlag)
    {
        return HI_SUCCESS;
    }

    if (HI_SUCCESS != HI_DRV_FILE_GetStorePath(path, DMX_FILE_NAME_LEN))
    {
        HI_ERR_DEMUX("get path failed\n");

        return HI_FAILURE;
    }

    SaveEsFlag = 1;

    return HI_FAILURE;
}

static HI_VOID DMX_OsrStopSaveEs(HI_VOID)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;
    HI_U32 ChanId;

    SaveEsFlag = 0;

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        mutex_lock(&DmxSet->LockAllChn);

        for_each_set_bit(ChanId, DmxSet->ChnBitmap, DmxSet->DmxChanCnt)
        {
            if (HI_SUCCESS == DmxGetChnInstance(DmxSet, ChanId))
            {
                if (DmxSet->DmxChanInfo[ChanId].SaveEsHandle)
                {
                    HI_DRV_FILE_Close(DmxSet->DmxChanInfo[ChanId].SaveEsHandle);
                    DmxSet->DmxChanInfo[ChanId].SaveEsHandle = HI_NULL;
                }

                DmxPutChnInstance(DmxSet, ChanId);
            }
        }

        mutex_unlock(&DmxSet->LockAllChn);
    }
}

HI_DECLARE_MUTEX(DmxRamPortTsMutex);

static struct file *DmxRamPortTsHandle  = HI_NULL;
static HI_U32       DmxRamPortID         = 0;

HI_VOID DMX_OsrSaveIPTs(DMX_RamPort_Info_S *PortInfo, HI_U8 *buf, HI_U32 len,HI_U32 u32PortID)
{
    if (0 == down_interruptible(&DmxRamPortTsMutex))
    {
        if (DmxRamPortTsHandle && (DmxRamPortID - HI_UNF_DMX_PORT_RAM_0) == u32PortID)
        {
            if (HI_UNF_DMX_SECURE_MODE_TEE == PortInfo->SecureMode)
            {
                HI_ERR_DEMUX("not support saving ts data from secure buffer.\n");
            }
            else
            {
                HI_DRV_FILE_Write(DmxRamPortTsHandle, buf, len);
            }
        }

        up(&DmxRamPortTsMutex);
    }
}

static HI_S32 DMX_OsrSaveIPTsStart(HI_U32 u32PortID)
{
    char str[DMX_FILE_NAME_LEN];
    struct tm now;

    if ((u32PortID < HI_UNF_DMX_PORT_RAM_0) || (u32PortID  > (HI_UNF_DMX_PORT_RAM_0 + GetDmxCluster()->Ops->GetRamPortNum())))
    {
        HI_ERR_DEMUX("invalid port id:%d\n",u32PortID);
        return HI_FAILURE;
    }

    if (HI_SUCCESS != HI_DRV_FILE_GetStorePath(str, DMX_FILE_NAME_LEN))
    {
        HI_ERR_DEMUX("get path failed\n");

        return HI_FAILURE;
    }
    DmxRamPortID = u32PortID;

    if (0 == down_interruptible(&DmxRamPortTsMutex))
    {
        if (DmxRamPortTsHandle == HI_NULL)
        {
            time_to_tm(get_seconds(), 0, &now);
            snprintf(str, sizeof(str),"%s/dmx_ram_%u-%02u_%02u_%02u.ts", str, u32PortID, now.tm_hour, now.tm_min, now.tm_sec);

            DmxRamPortTsHandle = HI_DRV_FILE_Open(str, 1);
            if (!DmxRamPortTsHandle)
            {
                HI_ERR_DEMUX("open %s error\n", str);
                up(&DmxRamPortTsMutex);
                return HI_FAILURE;
            }
        }

        up(&DmxRamPortTsMutex);
    }

    return HI_SUCCESS;
}

static HI_VOID DMX_OsrSaveIPTsStop(HI_VOID)
{
    if (0 == down_interruptible(&DmxRamPortTsMutex))
    {
        if (DmxRamPortTsHandle)
        {
            HI_DRV_FILE_Close(DmxRamPortTsHandle);
            DmxRamPortTsHandle = HI_NULL;
            DmxRamPortID = 0;
        }

        up(&DmxRamPortTsMutex);
    }
}

/* candidate demux id used for allts record. */
static HI_U32 DmxIdGrp[2] = {4, 11};
static HI_U32 DmxAllTsDmxId = DMX_INVALID_DEMUX_ID;
static HI_U32 DmxTsDmxId = DMX_INVALID_DEMUX_ID;

static struct file         *DmxAllTsHandle  = HI_NULL;
static struct task_struct  *DmxAllTsThread  = HI_NULL;
static HI_U32               DmxAllTsRecId   = DMX_INVALID_REC_ID;

static HI_S32 HI_DMX_SaveAllTS_Routine(HI_VOID *arg)
{
    while (1)
    {
        HI_U32  RecId   = DmxAllTsRecId;
        HI_U32  PhyAddr;
        HI_U8   *KerAddr;
        HI_U32  Len;

        if (kthread_should_stop())
        {
            break;
        }

        if (DMX_INVALID_REC_ID == RecId)
        {
            continue;
        }

        if (HI_SUCCESS == DMX_DRV_REC_AcquireRecData(DmxAllTsDmxId, RecId, &PhyAddr, &KerAddr, &Len, 500))
        {
            if (DmxAllTsHandle)
            {
                HI_DRV_FILE_Write(DmxAllTsHandle, KerAddr, Len);
            }

            DMX_DRV_REC_ReleaseRecData(DmxAllTsDmxId, RecId, PhyAddr, Len);

        }

        yield();
    }

    return 0;
}

static HI_S32 DMX_OsrSaveALLTs_Start(HI_U32 PortId)
{
    HI_S32                  ret;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    DMX_PORT_MODE_E         PortMode;
    HI_UNF_DMX_REC_ATTR_S   RecAttr = {0};
    DMX_MMZ_BUF_S    RecBuf;
    HI_CHAR                 FileName[DMX_FILE_NAME_LEN] = {0};
    DMX_REC_TIMESTAMP_MODE_E enRecTimeStamp = DMX_REC_TIMESTAMP_NONE;
    struct tm now;
    HI_U32 index;

    ret = HI_DRV_FILE_GetStorePath(FileName, DMX_FILE_NAME_LEN);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("get path failed\n");

        return ret;
    }

    if (HI_NULL != DmxAllTsHandle)
    {
        HI_ERR_DEMUX("already started\n");

        return HI_FAILURE;
    }

    time_to_tm(get_seconds(), 0, &now);
    snprintf(FileName, sizeof(FileName),"%s/dmx_allts_%u-%02u_%02u_%02u.ts", FileName, PortId, now.tm_hour, now.tm_min, now.tm_sec);

    DmxAllTsHandle = HI_DRV_FILE_Open(FileName, 1);
    if (!DmxAllTsHandle)
    {
        HI_ERR_DEMUX("open %s error\n", FileName);

        goto exit;
    }

    if (PortId < HI_UNF_DMX_PORT_IF_0 + DmxCluster->Ops->GetIFPortNum())
    {
        PortMode = DMX_PORT_MODE_TUNER;
        PortId = PortId - HI_UNF_DMX_PORT_IF_0;
    }
    else if(PortId >= HI_UNF_DMX_PORT_TSI_0 && PortId < HI_UNF_DMX_PORT_TSI_0 + DmxCluster->Ops->GetTSIPortNum())
    {
        PortMode = DMX_PORT_MODE_TUNER;
        PortId = PortId - HI_UNF_DMX_PORT_TSI_0 + DmxCluster->Ops->GetIFPortNum();
    }
    else if (PortId >= HI_UNF_DMX_PORT_RAM_0 && PortId < HI_UNF_DMX_PORT_RAM_0 + DmxCluster->Ops->GetRamPortNum())
    {
        PortMode = DMX_PORT_MODE_RAM;
        PortId = PortId - HI_UNF_DMX_PORT_RAM_0;
    }
    else if (PortId >= HI_MPI_DMX_PORT_RMX_0 && PortId < HI_MPI_DMX_PORT_RMX_0 + DmxCluster->Ops->GetRmxNum())
    {
        PortMode = DMX_PORT_MODE_RMX;
        PortId = PortId - HI_MPI_DMX_PORT_RMX_0;
    }
    else
    {
        PortMode = DMX_PORT_MODE_BUTT;
    }

    for (index = 0; index < sizeof(DmxIdGrp) /sizeof(HI_U32); index ++)
    {
        DmxAllTsDmxId = DmxIdGrp[index];

        ret = DMX_OsiAttachPort(DmxAllTsDmxId, PortMode, PortId);
        if (HI_SUCCESS == ret)
        {
            break;
        }
    }

    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("AttachPort failed 0x%x\n", ret);

        goto exit;
    }

    RecAttr.u32DmxId        = DmxAllTsDmxId;
    RecAttr.u32RecBufSize   = 0x400000;
    RecAttr.enRecType       = HI_UNF_DMX_REC_TYPE_ALL_PID;
    RecAttr.enSecureMode = HI_UNF_DMX_SECURE_MODE_NONE;

    ret = DMX_DRV_REC_CreateChannel(&RecAttr, enRecTimeStamp,&DmxAllTsRecId, &RecBuf);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("open rec failed 0x%x\n", ret);

        goto exit;
    }

    ret = DMX_DRV_REC_StartRecChn(DmxAllTsDmxId, DmxAllTsRecId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("start rec failed 0x%x\n", ret);

        goto exit;
    }

    DmxAllTsThread = kthread_create(HI_DMX_SaveAllTS_Routine, HI_NULL, "SaveAllTs");
    if (IS_ERR(DmxAllTsThread))
    {
        HI_ERR_DEMUX("create kthread failed\n");

        goto exit;
    }

    wake_up_process(DmxAllTsThread);

    return HI_SUCCESS;

exit :
    if (DMX_INVALID_REC_ID != DmxAllTsRecId)
    {
        DMX_DRV_REC_StopRecChn(DmxAllTsDmxId, DmxAllTsRecId);

        DMX_DRV_REC_DestroyChannel(DmxAllTsDmxId, DmxAllTsRecId);

        DmxAllTsRecId = DMX_INVALID_REC_ID;
    }

    DMX_OsiDetachPort(DmxAllTsDmxId);

    if (DmxAllTsHandle)
    {
        HI_DRV_FILE_Close(DmxAllTsHandle);
        DmxAllTsHandle = HI_NULL;
    }

    return HI_FAILURE;
}

static HI_VOID DMX_OsrSaveALLTs_Stop(HI_VOID)
{
    if (DmxAllTsHandle)
    {
        kthread_stop(DmxAllTsThread);
        DmxAllTsThread = HI_NULL;

        DMX_DRV_REC_StopRecChn(DmxAllTsDmxId, DmxAllTsRecId);

        DMX_DRV_REC_DestroyChannel(DmxAllTsDmxId, DmxAllTsRecId);

        DmxAllTsRecId = DMX_INVALID_REC_ID;

        DMX_OsiDetachPort(DmxAllTsDmxId);

        HI_DRV_FILE_Close(DmxAllTsHandle);
        DmxAllTsHandle = HI_NULL;

    }
}

static struct task_struct  *DmxRecTsThread  = HI_NULL;
static struct file         *DmxRecTsHandle  = HI_NULL;
static HI_U32               DmxTsRecId      = DMX_INVALID_REC_ID;

static HI_S32 HI_DMX_SaveDmxTS_Routine(HI_VOID *arg)
{
    while (1)
    {
        HI_U32  RecId   = DmxTsRecId;
        HI_U32  PhyAddr;
        HI_U8   *KerAddr;
        HI_U32  Len;

        if (kthread_should_stop())
        {
            break;
        }

        if (DMX_INVALID_REC_ID == RecId)
        {
            continue;
        }

        if (HI_SUCCESS == DMX_DRV_REC_AcquireRecData(DmxTsDmxId, RecId, &PhyAddr, &KerAddr, &Len, 500))
        {
            if (DmxRecTsHandle)
            {
                HI_DRV_FILE_Write(DmxRecTsHandle, KerAddr, Len);
            }

            DMX_DRV_REC_ReleaseRecData(DmxTsDmxId, RecId, PhyAddr, Len);
        }

        yield();
    }

    return 0;
}

static HI_S32 DMX_OsrSaveDmxTs_Start(HI_U32 DmxId)
{
    HI_S32                  ret;
    HI_UNF_DMX_REC_ATTR_S   RecAttr = {0};
    DMX_MMZ_BUF_S    RecBuf;
    HI_CHAR                 FileName[DMX_FILE_NAME_LEN] = {0};
    DMX_REC_TIMESTAMP_MODE_E enRecTimeStamp = DMX_REC_TIMESTAMP_NONE;
    struct tm now;
    HI_U32 index;

    ret = HI_DRV_FILE_GetStorePath(FileName, DMX_FILE_NAME_LEN);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("get path failed\n");

        return HI_FAILURE;
    }

    if (HI_NULL != DmxRecTsHandle)
    {
        HI_ERR_DEMUX("already started\n");

        return HI_FAILURE;
    }

    time_to_tm(get_seconds(), 0, &now);
    snprintf(FileName,sizeof(FileName), "%s/dmx_rects_%u-%02u_%02u_%02u.ts", FileName, DmxId, now.tm_hour, now.tm_min, now.tm_sec);

    DmxRecTsHandle = HI_DRV_FILE_Open(FileName, 1);
    if (!DmxRecTsHandle)
    {
        HI_ERR_DEMUX("open %s error\n", FileName);

        goto exit;
    }

    for (index = 0; index < sizeof(DmxIdGrp) /sizeof(HI_U32); index ++)
    {
        Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
        DMX_DmxInfo_S  *DmxInfo = DmxSet->Ops->GetDmxInfo(DmxSet, DmxId);

        DmxTsDmxId = DmxIdGrp[index];

        ret = DMX_OsiAttachPort(DmxTsDmxId, DmxInfo->PortMode, DmxInfo->PortId);
        if (HI_SUCCESS == ret)
        {
            break;
        }
    }

    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("AttachPort failed 0x%x\n", ret);

        goto exit;
    }

    RecAttr.u32DmxId        = DmxTsDmxId;
    RecAttr.bDescramed     = HI_TRUE;
    RecAttr.enIndexType     = HI_UNF_DMX_REC_INDEX_TYPE_NONE;
    RecAttr.u32IndexSrcPid  = DMX_INVALID_PID;
    RecAttr.enVCodecType    = HI_UNF_VCODEC_TYPE_MPEG2;
    RecAttr.enRecType       = HI_UNF_DMX_REC_TYPE_SELECT_PID;
    RecAttr.u32RecBufSize   = 0x400000;
    RecAttr.enSecureMode = HI_UNF_DMX_SECURE_MODE_NONE;

    ret = DMX_DRV_REC_CreateChannel(&RecAttr, enRecTimeStamp,&DmxTsRecId, &RecBuf);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("open rec failed 0x%x\n", ret);

        goto exit;
    }

    ret = DMX_OsiSaveDmxTs_Start(DmxTsRecId, DmxId, DmxTsDmxId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("ts start failed\n");

        return HI_FAILURE;
    }

    ret = DMX_DRV_REC_StartRecChn(DmxTsDmxId, DmxTsRecId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("start rec failed 0x%x\n", ret);

        goto exit;
    }

    DmxRecTsThread = kthread_create(HI_DMX_SaveDmxTS_Routine, HI_NULL, "SaveDmxTs");
    if (IS_ERR(DmxRecTsThread))
    {
        HI_ERR_DEMUX("create kthread failed\n");

        goto exit;
    }

    wake_up_process(DmxRecTsThread);

    return HI_SUCCESS;

exit :

    if (DMX_INVALID_REC_ID != DmxTsRecId)
    {
        DMX_DRV_REC_StopRecChn(DmxTsDmxId, DmxTsRecId);

        DMX_DRV_REC_DelAllRecPid(DmxTsDmxId, DmxTsRecId);

        DMX_DRV_REC_DestroyChannel(DmxTsDmxId, DmxTsRecId);

        DmxTsRecId = DMX_INVALID_REC_ID;
    }

    DMX_OsiDetachPort(DmxTsDmxId);

    if (DmxRecTsHandle)
    {
        HI_DRV_FILE_Close(DmxRecTsHandle);
        DmxRecTsHandle = HI_NULL;
    }

    return HI_FAILURE;
}

static HI_VOID DMX_OsrSaveDmxTs_Stop(HI_VOID)
{
    if (DmxRecTsHandle)
    {
        kthread_stop(DmxRecTsThread);
        DmxRecTsThread = HI_NULL;

        DMX_DRV_REC_StopRecChn(DmxTsDmxId, DmxTsRecId);

        DMX_DRV_REC_DelAllRecPid(DmxTsDmxId, DmxTsRecId);

        DMX_DRV_REC_DestroyChannel(DmxTsDmxId, DmxTsRecId);

        DmxTsRecId = DMX_INVALID_REC_ID;

        DMX_OsiDetachPort(DmxTsDmxId);

        HI_DRV_FILE_Close(DmxRecTsHandle);
        DmxRecTsHandle = HI_NULL;
    }

}

/* control function entry by proc file system */
HI_S32 DMX_OsrDebugCtrl(HI_U32 cmd,DMX_DEBUG_CMD_CTRl cmdctrl,HI_U32 param)
{
    HI_S32 ret;

    GetDmxCluster()->Ops->StartCluster();

    switch (cmd)
    {
        case DMX_DEBUG_CMD_SAVE_ES:
        {
            if (DMX_DEBUG_CMD_START == cmdctrl)
            {

                ret = DMX_OsrStartSaveEs();
                if (HI_SUCCESS == ret)
                {
                    HI_PRINT("begin save es\n");
                }
            }
            else
            {
                HI_PRINT("stop save es\n");
                DMX_OsrStopSaveEs();
            }

            break;
        }

        case DMX_DEBUG_CMD_SAVE_ALLTS:
        {
            if (DMX_DEBUG_CMD_STOP == cmdctrl)
            {
                HI_PRINT("stop allts save\n");
                DMX_OsrSaveALLTs_Stop();
            }
            else
            {
                ret = DMX_OsrSaveALLTs_Start(param);
                if (HI_SUCCESS == ret)
                {
                    HI_PRINT("begin save allts of port:%d\n",param);
                }
            }

            break;
        }

        case DMX_DEBUG_CMD_SAVE_IPTS:
        {
            if (DMX_DEBUG_CMD_START == cmdctrl)
            {
                ret = DMX_OsrSaveIPTsStart(param);
                if (HI_SUCCESS == ret)
                {
                    HI_PRINT("begin save ram port:%d ts\n",param);
                }
            }
            else
            {
                HI_PRINT("stop save ip port ts\n");
                DMX_OsrSaveIPTsStop();
            }

            break;
        }

        case DMX_DEBUG_CMD_SAVE_DMXTS:
        {
            if (DMX_DEBUG_CMD_STOP == cmdctrl)
            {
                HI_PRINT("stop save dmx ts\n");
                DMX_OsrSaveDmxTs_Stop();
            }
            else
            {
                ret = DMX_OsrSaveDmxTs_Start(param);
                if (HI_SUCCESS == ret)
                {
                    HI_PRINT("begine save dmx:%d ts\n",param);
                }
            }

            break;
        }

        default:
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#endif

/*opentv5 export these symbol*/
EXPORT_SYMBOL(HI_DRV_DMX_Init);
EXPORT_SYMBOL(HI_DRV_DMX_DeInit);
EXPORT_SYMBOL(DMX_OsiSuspend);
EXPORT_SYMBOL(DMX_OsiResume);
EXPORT_SYMBOL(HI_DRV_DMX_Open);
EXPORT_SYMBOL(HI_DRV_DMX_Close);
EXPORT_SYMBOL(HI_DRV_DMX_GetPoolBufAddr);
EXPORT_SYMBOL(HI_DRV_DMX_GetCapability);
EXPORT_SYMBOL(HI_DRV_DMX_TunerPortGetAttr);
EXPORT_SYMBOL(HI_DRV_DMX_TunerPortSetAttr);
EXPORT_SYMBOL(HI_DRV_DMX_RamPortGetAttr);
EXPORT_SYMBOL(HI_DRV_DMX_RamPortSetAttr);
EXPORT_SYMBOL(HI_DRV_DMX_AttachTunerPort);
EXPORT_SYMBOL(HI_DRV_DMX_AttachRamPort);
EXPORT_SYMBOL(HI_DRV_DMX_DetachPort);
EXPORT_SYMBOL(HI_DRV_DMX_GetPortId);
EXPORT_SYMBOL(HI_DRV_DMX_TunerPortGetPacketNum);
EXPORT_SYMBOL(HI_DRV_DMX_RamPortGetPacketNum);
EXPORT_SYMBOL(HI_DRV_DMX_CreateTSBuffer);
EXPORT_SYMBOL(HI_DRV_DMX_DestroyTSBuffer);
EXPORT_SYMBOL(HI_DRV_DMX_GetTSBuffer);
EXPORT_SYMBOL(HI_DRV_DMX_PutTSBuffer);
EXPORT_SYMBOL(HI_DRV_DMX_ResetTSBuffer);
EXPORT_SYMBOL(HI_DRV_DMX_GetTSBufferStatus);
EXPORT_SYMBOL(HI_DRV_DMX_CreateChannel);
EXPORT_SYMBOL(HI_DRV_DMX_DestroyChannel);
EXPORT_SYMBOL(HI_DRV_DMX_GetChannelAttr);
EXPORT_SYMBOL(HI_DRV_DMX_SetChannelAttr);
EXPORT_SYMBOL(HI_DRV_DMX_SetChannelPID);
EXPORT_SYMBOL(HI_DRV_DMX_GetChannelPID);
EXPORT_SYMBOL(HI_DRV_DMX_OpenChannel);
EXPORT_SYMBOL(HI_DRV_DMX_CloseChannel);
EXPORT_SYMBOL(HI_DRV_DMX_GetChannelStatus);
EXPORT_SYMBOL(HI_DRV_DMX_GetChannelHandle);
EXPORT_SYMBOL(HI_DRV_DMX_GetFreeChannelCount);
EXPORT_SYMBOL(HI_DRV_DMX_GetScrambledFlag);
EXPORT_SYMBOL(HI_DRV_DMX_GetChannelTsCount);
EXPORT_SYMBOL(HI_DRV_DMX_SetChannelEosFlag);
EXPORT_SYMBOL(HI_DRV_DMX_CreateFilter);
EXPORT_SYMBOL(HI_DRV_DMX_DestroyFilter);
EXPORT_SYMBOL(HI_DRV_DMX_DestroyAllFilter);
EXPORT_SYMBOL(HI_DRV_DMX_SetFilterAttr);
EXPORT_SYMBOL(HI_DRV_DMX_GetFilterAttr);
EXPORT_SYMBOL(HI_DRV_DMX_AttachFilter);
EXPORT_SYMBOL(HI_DRV_DMX_DetachFilter);
EXPORT_SYMBOL(HI_DRV_DMX_GetFilterChannelHandle);
EXPORT_SYMBOL(HI_DRV_DMX_GetFreeFilterCount);
EXPORT_SYMBOL(HI_DRV_DMX_GetDataHandle);
EXPORT_SYMBOL(HI_DRV_DMX_SelectDataHandle);
EXPORT_SYMBOL(HI_DRV_DMX_AcquireBuf);
EXPORT_SYMBOL(HI_DRV_DMX_ReleaseBuf);
EXPORT_SYMBOL(HI_DRV_DMX_PeekBuf);
EXPORT_SYMBOL(HI_DRV_DMX_CreatePcrChannel);
EXPORT_SYMBOL(HI_DRV_DMX_DestroyPcrChannel);
EXPORT_SYMBOL(HI_DRV_DMX_PcrPidSet);
EXPORT_SYMBOL(HI_DRV_DMX_PcrPidGet);
EXPORT_SYMBOL(HI_DRV_DMX_PcrScrGet);
EXPORT_SYMBOL(HI_DRV_DMX_PcrSyncAttach);
EXPORT_SYMBOL(HI_DRV_DMX_PcrSyncDetach);
EXPORT_SYMBOL(HI_DRV_DMX_AcquireEs);
EXPORT_SYMBOL(HI_DRV_DMX_ReleaseEs);
EXPORT_SYMBOL(HI_DRV_DMX_GetPESBufferStatus);
EXPORT_SYMBOL(HI_DRV_DMX_CreateRecChn);
EXPORT_SYMBOL(HI_DRV_DMX_DestroyRecChn);
EXPORT_SYMBOL(HI_DRV_DMX_AddRecPid);
EXPORT_SYMBOL(HI_DRV_DMX_DelRecPid);
EXPORT_SYMBOL(HI_DRV_DMX_DelAllRecPid);
EXPORT_SYMBOL(HI_DRV_DMX_AddExcludeRecPid);
EXPORT_SYMBOL(HI_DRV_DMX_DelExcludeRecPid);
EXPORT_SYMBOL(HI_DRV_DMX_DelAllExcludeRecPid);
EXPORT_SYMBOL(HI_DRV_DMX_StartRecChn);
EXPORT_SYMBOL(HI_DRV_DMX_StopRecChn);
EXPORT_SYMBOL(HI_DRV_DMX_AcquireRecData);
EXPORT_SYMBOL(HI_DRV_DMX_ReleaseRecData);
EXPORT_SYMBOL(HI_DRV_DMX_AcquireRecIndex);
EXPORT_SYMBOL(HI_DRV_DMX_GetRecBufferStatus);
EXPORT_SYMBOL(HI_DRV_DMX_GetRecTsCnt);

