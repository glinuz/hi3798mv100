/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  drv_demux_func.c
* Description:  Define interfaces of demux hardware abstract layer.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20090927    y00106256      NULL      Create this file.
***********************************************************************************/

#include <linux/kernel.h>
#include <asm/div64.h>

#include "hi_type.h"
#include "hi_module.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"
#include "hi_drv_sys.h"
#include "hi_kernel_adapt.h"

#include "demux_debug.h"
#include "hal_demux.h"
#include "drv_demux_reg.h"
#include "drv_demux_func.h"
#include "drv_demux_scd.h"
#include "drv_demux_index.h"
#include "drv_demux_sw.h"
#include "drv_demux_osal.h"

#ifdef DMX_DESCRAMBLER_SUPPORT
#include "drv_descrambler.h"
#include "drv_descrambler_func.h"
#endif

#include "drv_sync_ext.h"

#define DMX_PES_CHANNEL_MIN_SIZE            0x40000

#define DMX_RAM_PORT_AUTO_REGION            60

#define DMX_RAM_PORT_AUTO_STEP_1            0
#define DMX_RAM_PORT_AUTO_STEP_2            1
#define DMX_RAM_PORT_AUTO_STEP_4            2
#define DMX_RAM_PORT_AUTO_STEP_8            3

#define DMX_FQ_VID_BLOCK_SIZE               (16 * 1024)
#define DMX_FQ_AUD_BLOCK_SIZE               (1 * 256)  /* some aud bandwidth is small, big block will delay data too much. */

#define DMX_REC_MIN_BUF_SIZE                (256 * 1024)

#define DMX_REC_VID_SCD_BUF_SIZE            (56 * 1024)
#define DMX_REC_AUD_SCD_BUF_SIZE            (28 * 1024)

#define DMX_REC_TS_PACKETS_UNIT             (4 * 188)


#define DMX_REC_VID_TS_PACKETS_PER_BLOCK    (64 * DMX_REC_TS_PACKETS_UNIT)
#define DMX_REC_AUD_TS_PACKETS_PER_BLOCK    (32 * DMX_REC_TS_PACKETS_UNIT)

#define DMX_REC_TS_WITH_TIMESTAMP_UNIT      (4 * 192)
#define DMX_REC_VID_TS_WITH_TIMESTAMP_BLOCK_SIZE    (64 * DMX_REC_TS_WITH_TIMESTAMP_UNIT)
#define DMX_REC_AUD_TS_WITH_TIMESTAMP_BLOCK_SIZE    (32 * DMX_REC_TS_WITH_TIMESTAMP_UNIT)

#define DMX_REC_VID_SCD_NUM_PER_BLOCK       (8 * 28)
/*
fqdepth == DMX_REC_AUD_SCD_BUF_SIZE/DMX_REC_AUD_SCD_PER_BLOCK  >= 1023 , so ,the min DMX_REC_AUD_SCD_PER_BLOCK should be 28
*/
#define DMX_REC_AUD_SCD_NUM_PER_BLOCK       (4 * 28)

#define DMX_FQ_DESC_SIZE                    0x8
#define DMX_OQ_DESC_SIZE                    0x10

#define MIN_MMZ_BUF_SIZE                    4096

#define DMX_TS_BUFFER_EMPTY                 2048

static atomic_t g_DrvInitFlag = ATOMIC_INIT(0);
Dmx_Cluster_S *g_DmxClusterCtl = HI_NULL;

SYNC_EXPORT_FUNC_S *g_pSyncFunc    = HI_NULL;

static DMX_ERRMSG_S psErrMsg[DMX_MAX_ERRLIST_NUM];

/* select helper */
static DEFINE_MUTEX(g_SelectLock);
static HI_U32 g_SelectCondition = 0;
static DEFINE_SPINLOCK(g_WaitQueueLock);
static wait_queue_head_t *g_SelectWaitQueue = HI_NULL;
static struct workqueue_struct *g_SelectWorkQueue = HI_NULL;

struct DMX_Eop_Int_Mark{
    HI_BOOL ValidFlag;
    Dmx_Set_S *DmxSet;
    HI_U32 OQId;
    HI_U32 ChanId;
    struct work_struct work;
};

static struct DMX_Eop_Int_Mark g_EnEopIntMark[DMX_CLUSTER_TOTAL_SET_CNT * DMX_CHANNEL_CNT] = {{0}};

HI_VOID DMX_OsrSaveEs(DMX_ChanInfo_S *ChanInfo, DMX_Stream_S *EsData);
static HI_VOID DMXOsiOQGetReadWrite(Dmx_Set_S *DmxSet, HI_U32 OQId, HI_U32 *BlockWrite, HI_U32 *BlockRead);

static struct task_struct  *g_DmxMonitorThread  = HI_NULL;
HI_S32 CheckChnTimeoutProc(HI_VOID *ptr);

static HI_U32 GetQueueLenth(const HI_U32 Read, const HI_U32 Write, const HI_U32 Size)
{
    HI_U32 ret;

    if (Read > Write)
    {
        ret = Size + Write - Read;
    }
    else
    {
        ret = Write - Read;
    }

    return ret;
}

static HI_VOID DMXConfigIPPortRate(Dmx_Set_S *DmxSet, HI_U32 PortId)
{
    DMX_RamPort_Info_S *PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);
    HI_U8 i = 0;
    HI_U32 TSOClk = 150;
    HI_U32 DmxClk;
    HI_UNF_DMX_TSO_PORT_ATTR_S PortAttr;

    DmxClk = DmxHalGetDmxClk(DmxSet);

    for ( i = 0 ; i < DmxSet->TSOPortCnt; i++ )
    {
        DMX_OsiTSOPortGetAttr(i, &PortAttr);
        if ( (PortAttr.enTSSource  == PortId + HI_UNF_DMX_PORT_RAM_0))
        {
            switch ( PortAttr.enClk )
            {
                case HI_UNF_DMX_TSO_CLK_100M :
                    TSOClk = 100;
                    break;
                case HI_UNF_DMX_TSO_CLK_150M :
                    TSOClk = 150;
                    break;
                case HI_UNF_DMX_TSO_CLK_1200M :
                    TSOClk = 1200;
                    break;
                case HI_UNF_DMX_TSO_CLK_1500M :
                    TSOClk = 1500;
                    break;
                case HI_UNF_DMX_TSO_CLK_BUTT :
                    HI_ERR_DEMUX("TSO clk invalid\n");
                    break;
            }

            TSOClk = TSOClk/PortAttr.u32ClkDiv;

            if ( !PortAttr.bSerial )
            {
                TSOClk = TSOClk*8;
            }
            /*
            IPClk = DmxClk*8/(IPRate+1) < TSOClk; so IPRate >  DmxClk*8/TSOClk -1 ;
            */
            PortInfo->IpRate =  DmxClk*8/TSOClk ;

            /* experiment tune value for decrease ram port dispatch rate */
            PortInfo->IpRate += 3;
        }
    }

    DmxHalIPPortRateSet(DmxSet, PortId, PortInfo->IpRate);
}

static HI_VOID __DMXCheckFQBPStatus(Dmx_Set_S *DmxSet, HI_U32 RamPortId, HI_U32 FQId)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    HI_U32 FQ_WPtr, FQ_RPtr, ValidDes;

    BUG_ON(RamPortId >= DmxCluster->Ops->GetRamPortNum());

    if (DmxHalGetIPBPStatus(DmxSet, RamPortId))
    {
        DmxHalGetFQReadWritePtr(DmxSet, FQId, &FQ_RPtr, &FQ_WPtr);

        if (FQ_WPtr >= FQ_RPtr)
        {
            ValidDes = FQ_WPtr - FQ_RPtr;
        }
        else
        {
            ValidDes = (DmxSet->DmxFqInfo[FQId].FQDepth) - FQ_RPtr + FQ_WPtr;
        }

        if (ValidDes >= (DmxSet->DmxFqInfo[FQId].FQDepth / 10))
        {
            DmxHalClrIPFqBPStatus(DmxSet, RamPortId, FQId);
        }
    }
}

static HI_VOID DMXCheckFQBPStatus(Dmx_Set_S *DmxSet, DMX_DmxInfo_S *DmxInfo, HI_U32 FQId)
{
    switch(DmxInfo->PortMode)
    {
        case DMX_PORT_MODE_RAM:
        {
            __DMXCheckFQBPStatus(DmxSet, DmxInfo->PortId, FQId);
            break;
        }
        case DMX_PORT_MODE_RMX:
        {
            RmxChkFqBpIp(DmxSet, DmxInfo->PortId, FQId);
            break;
        }
        default:
            break;
    }
}

static HI_S32 DmxFlushChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_FLUSH_TYPE_E FlushType)
{
    HI_S32  ret = HI_SUCCESS;
    HI_U32  i;

    DmxHalFlushChannel(DmxSet, ChanId, FlushType);

    for (i = 0; i < DMX_MAX_FLUSH_WAIT; i++)
    {
        if (DmxHalIsFlushChannelDone(DmxSet))
        {
            break;
        }
    }

    if (DMX_MAX_FLUSH_WAIT == i)
    {
        HI_ERR_DEMUX("flush channel %u failed\n", ChanId);

        ret = HI_FAILURE;
    }

    return ret;
}

static HI_S32 DmxFlushOq(Dmx_Set_S *DmxSet, HI_U32 OqId, DMX_OQ_CLEAR_TYPE_E ClearType)
{
    HI_S32  ret = HI_SUCCESS;
    HI_U32  i;

    DmxHalClearOq(DmxSet, OqId, ClearType);

    for (i = 0; i < DMX_MAX_FLUSH_WAIT; i++)
    {
        if (DmxHalIsClearOqDone(DmxSet))
        {
            break;
        }
    }

    if (DMX_MAX_FLUSH_WAIT == i)
    {
        HI_ERR_DEMUX("clear oq %u failed\n", OqId);

        ret = HI_FAILURE;
    }

    return ret;
}

//enable oq recive
//close channel dmx ->  disable oq -> flush dmx channel
//-> clear oq reg infor -> enable oq -> set channel dmx
static HI_S32 DMXOsiEnableOQRecive(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 OqId)
{
    HI_U32  RawDmxId;
    HI_U32  Word;

    DmxHalGetChannelPlayDmxid(DmxSet, ChanId, &RawDmxId);
    DmxHalSetChannelPlayDmxid(DmxSet, ChanId, DMX_INVALID_DEMUX_ID);
    DmxHalDisableOQRecive(DmxSet, OqId);

    DmxFlushChannel(DmxSet, ChanId, DMX_FLUSH_TYPE_REC_PLAY);

    DmxHalSetOQWORDx(DmxSet, OqId, DMX_OQ_EOPWR_OFFSET, 0 );
    DmxHalGetOQWORDx(DmxSet, OqId, DMX_OQ_RSV_OFFSET, &Word);
    Word &= 0xffffff00;
    DmxHalSetOQWORDx(DmxSet, OqId, DMX_OQ_RSV_OFFSET, Word);
    DmxHalGetOQWORDx(DmxSet, OqId, DMX_OQ_CTRL_OFFSET, &Word);
    Word &= 0x80;
    DmxHalSetOQWORDx(DmxSet, OqId, DMX_OQ_CTRL_OFFSET, Word);

    DmxHalEnableOQRecive(DmxSet, OqId);
    DmxHalSetChannelPlayDmxid(DmxSet, ChanId, RawDmxId);

    return 0;
}

static HI_S32 ResetOQ(Dmx_Set_S *DmxSet, DMX_DmxInfo_S *DmxInfo, DMX_OQ_Info_S *pstOQInfo)
{
    HI_U32 i, u32PvrCtrl;
    HI_U32 u32WriteBlk, u32OqRead, u32OqWrite;
    HI_U32 u32BufPhyAddr, u32BlkSize, u32BlkNum;
    HI_S32 s32OQEnableStatus;
    FQ_DescInfo_S *FQ_Desc;
    OQ_DescInfo_S *OQ_Desc;
    DMX_FQ_Info_S  *FqInfo;
    HI_SIZE_T u32LockFlag;

    s32OQEnableStatus = DmxHalGetOQEnableStatus(DmxSet, pstOQInfo->u32OQId);
    DmxHalDisableOQRecive(DmxSet, pstOQInfo->u32OQId);
    DMXOsiOQGetReadWrite(DmxSet, pstOQInfo->u32OQId, &u32OqWrite, &u32OqRead);

    if (!DmxSet->DmxFqInfo[pstOQInfo->u32FQId].FqDescSetBase)
    {
        HI_ERR_DEMUX("fq not valid \n");

        DMXOsiEnableOQRecive(DmxSet, pstOQInfo->u32AttachId, pstOQInfo->u32OQId);
        return HI_FAILURE;
    }

    FqInfo = &DmxSet->DmxFqInfo[pstOQInfo->u32FQId];

    spin_lock_irqsave(&FqInfo->LockFq, u32LockFlag);

    DmxHalGetFQReadWritePtr(DmxSet, pstOQInfo->u32FQId, HI_NULL, &u32WriteBlk);

    u32BlkNum = GetQueueLenth(u32OqRead, u32OqWrite, pstOQInfo->OQDepth);
    for (i = 0; i < u32BlkNum; i++)
    {
        OQ_Desc = pstOQInfo->OqDescSetBase + u32OqRead;
        u32BufPhyAddr = OQ_Desc->start_addr;
        u32BlkSize = OQ_Desc->cactrl_buflen & 0xffff;

        FQ_Desc = DmxSet->DmxFqInfo[pstOQInfo->u32FQId].FqDescSetBase + u32WriteBlk;
        FQ_Desc->start_addr = u32BufPhyAddr;
        FQ_Desc->buflen = u32BlkSize;

        if (unlikely(!(FQ_Desc->start_addr >= FqInfo->BufPhyAddr && FQ_Desc->start_addr < FqInfo->BufPhyAddr + FqInfo->BufSize)))
        {
            HI_FATAL_DEMUX("invalid desc[0x%x, %d] of fq[0x%x, %d].\n", FQ_Desc->start_addr, FQ_Desc->buflen, FqInfo->BufPhyAddr, FqInfo->BufSize);
            BUG();
        }

        DMXINC(u32WriteBlk, DmxSet->DmxFqInfo[pstOQInfo->u32FQId].FQDepth);
        DMXINC(u32OqRead, pstOQInfo->OQDepth);
    }

    DmxHalGetOQWORDx(DmxSet, pstOQInfo->u32OQId, DMX_OQ_CTRL_OFFSET, &u32PvrCtrl);
    if (u32PvrCtrl & DMX_MASK_BIT_7)
    {
        DmxHalGetOQWORDx(DmxSet, pstOQInfo->u32OQId, DMX_OQ_SADDR_OFFSET, &u32BufPhyAddr);
        DmxHalGetOQWORDx(DmxSet, pstOQInfo->u32OQId, DMX_OQ_SZUS_OFFSET, &u32BlkSize);
        u32BlkSize  >>= 16;
        FQ_Desc = DmxSet->DmxFqInfo[pstOQInfo->u32FQId].FqDescSetBase + u32WriteBlk;
        FQ_Desc->start_addr = u32BufPhyAddr;
        FQ_Desc->buflen = u32BlkSize;

        if (unlikely(!(FQ_Desc->start_addr >= FqInfo->BufPhyAddr && FQ_Desc->start_addr < FqInfo->BufPhyAddr + FqInfo->BufSize)))
        {
            HI_FATAL_DEMUX("invalid desc[0x%x, %d] of fq[0x%x, %d].\n", FQ_Desc->start_addr, FQ_Desc->buflen, FqInfo->BufPhyAddr, FqInfo->BufSize);
            BUG();
        }

        DMXINC(u32WriteBlk, DmxSet->DmxFqInfo[pstOQInfo->u32FQId].FQDepth);
    }

    DmxHalSetFQWritePtr(DmxSet, pstOQInfo->u32FQId, u32WriteBlk);

    DMXCheckFQBPStatus(DmxSet, DmxInfo, pstOQInfo->u32FQId); //CLR BP STATUS

    spin_unlock_irqrestore(&FqInfo->LockFq, u32LockFlag);

    pstOQInfo->u32ProcsBlk = 0;
    pstOQInfo->u32ProcsOffset = 0;
    pstOQInfo->u32ReleaseBlk = 0;
    pstOQInfo->u32ReleaseOffset = 0;

    DmxHalSetOQWORDx(DmxSet, pstOQInfo->u32OQId, DMX_OQ_RSV_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, pstOQInfo->u32OQId, DMX_OQ_CTRL_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, pstOQInfo->u32OQId, DMX_OQ_EOPWR_OFFSET, 0 );
    DmxHalSetOQWORDx(DmxSet, pstOQInfo->u32OQId, DMX_OQ_SZUS_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, pstOQInfo->u32OQId, DMX_OQ_SADDR_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, pstOQInfo->u32OQId, DMX_OQ_RDWR_OFFSET, 0);

    //reset oq ts counter
    DmxHalResetOqCounter(DmxSet, pstOQInfo->u32OQId);
    if (s32OQEnableStatus)
    {
        DMXOsiEnableOQRecive(DmxSet, pstOQInfo->u32AttachId,pstOQInfo->u32OQId);
    }

    return HI_SUCCESS;
}

static HI_VOID TsBufferConfig(Dmx_Set_S *DmxSet, const HI_U32 PortId, const HI_BOOL Enable, const HI_U32 DescPhyAddr, const HI_U32 DescDepth)
{
    DmxHalIPPortDescSet(DmxSet, PortId, DescPhyAddr, DescDepth);

    DMXConfigIPPortRate(DmxSet, PortId);

    DmxHalIPPortSetOutInt(DmxSet, PortId, Enable);

    DmxHalEnableRamMmu(DmxSet, PortId);

    DmxHalIPPortStartStream(DmxSet, PortId, Enable);
}

static HI_VOID TsBufferDeConfig(Dmx_Set_S *DmxSet, const HI_U32 PortId, const HI_BOOL Enable, const HI_U32 DescPhyAddr, const HI_U32 DescDepth)
{
    BUG_ON(HI_FALSE != Enable);

    DmxHalIPPortStartStream(DmxSet, PortId, Enable);

    DmxHalDisableRamMmu(DmxSet, PortId);

    DmxHalIPPortSetOutInt(DmxSet, PortId, Enable);
}

#ifdef DMX_TEE_SUPPORT
static HI_S32 DmxInitTeec(Dmx_Cluster_S *DmxCluster)
{
    HI_S32 ret = HI_FAILURE;
    TEEC_UUID DmxTaskUUID =
    {
        0x10101010, \
        0x1010, \
        0x1010, \
        {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}
    };
    TEEC_Operation Operation;
    HI_CHAR GeneralSessionName[] = "tee_dmx_general_session";
    HI_CHAR LockSessionName[]    = "tee_dmx_lock_session";
    HI_CHAR UnlockSessionName[]  = "tee_dmx_unlock_session";
    HI_U32 RootId = 0;

    mutex_lock(&DmxCluster->teec.lock);

     if (unlikely(HI_TRUE == DmxCluster->teec.connected))
     {
        ret = HI_SUCCESS;
        goto out0;
     }

    ret = TEEK_InitializeContext("dmx_teec", &DmxCluster->teec.context);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("Initialise dmx teec context failed(0x%x)", ret);
        goto out1;
    }

    memset(&Operation, 0x0, sizeof(TEEC_Operation));
    Operation.started = 1;
    Operation.cancel_flag = 0;
    Operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);
    Operation.params[2].tmpref.buffer = (void *)(&RootId);
    Operation.params[2].tmpref.size = sizeof(RootId);
    Operation.params[3].tmpref.buffer = (void *)(GeneralSessionName);
    Operation.params[3].tmpref.size = strlen(GeneralSessionName) + 1;

    ret = TEEK_OpenSession(&DmxCluster->teec.context, &DmxCluster->teec.session, &DmxTaskUUID, TEEC_LOGIN_IDENTIFY, NULL, &Operation, NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("open dmx teec session failed(0x%x)", ret);
        goto out2;
    }

    memset(&Operation, 0x0, sizeof(TEEC_Operation));
    Operation.started = 1;
    Operation.cancel_flag = 0;
    Operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);
    Operation.params[2].tmpref.buffer = (void *)(&RootId);
    Operation.params[2].tmpref.size = sizeof(RootId);
    Operation.params[3].tmpref.buffer = (void *)(LockSessionName);
    Operation.params[3].tmpref.size = strlen(LockSessionName) + 1;

    ret = TEEK_OpenSession(&DmxCluster->teec.context, &DmxCluster->teec.session_lock, &DmxTaskUUID, TEEC_LOGIN_IDENTIFY, NULL, &Operation, NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("open dmx teec session_lock failed(0x%x)", ret);
        goto out3;
    }

    memset(&Operation, 0x0, sizeof(TEEC_Operation));
    Operation.started = 1;
    Operation.cancel_flag = 0;
    Operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);
    Operation.params[2].tmpref.buffer = (void *)(&RootId);
    Operation.params[2].tmpref.size = sizeof(RootId);
    Operation.params[3].tmpref.buffer = (void *)(UnlockSessionName);
    Operation.params[3].tmpref.size = strlen(UnlockSessionName) + 1;

    ret = TEEK_OpenSession(&DmxCluster->teec.context, &DmxCluster->teec.session_unlock, &DmxTaskUUID, TEEC_LOGIN_IDENTIFY, NULL, &Operation, NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("open dmx teec session_unlock failed(0x%x)", ret);
        goto out4;
    }

    DmxCluster->teec.connected = HI_TRUE;

    mutex_unlock(&DmxCluster->teec.lock);

    return HI_SUCCESS;

out4:
    TEEK_CloseSession(&DmxCluster->teec.session_lock);
out3:
    TEEK_CloseSession(&DmxCluster->teec.session);
out2:
    TEEK_FinalizeContext(&DmxCluster->teec.context);
out1:
    DmxCluster->teec.connected = HI_FALSE;
out0:
    mutex_unlock(&DmxCluster->teec.lock);
    return ret;
}

static HI_VOID DmxDeinitTeec(Dmx_Cluster_S *DmxCluster)
{
    mutex_lock(&DmxCluster->teec.lock);

    if (HI_TRUE == DmxCluster->teec.connected)
    {
        DmxCluster->teec.connected = HI_FALSE;

        TEEK_CloseSession(&DmxCluster->teec.session_unlock);

        TEEK_CloseSession(&DmxCluster->teec.session_lock);

        TEEK_CloseSession(&DmxCluster->teec.session);

        TEEK_FinalizeContext(&DmxCluster->teec.context);
    }

    mutex_unlock(&DmxCluster->teec.lock);
}

static HI_S32 DmxCluster_SendCmdToTA(HI_U32 CmdId, TEEC_Operation *Operation, HI_U32 *RetOrigin)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();

    if (unlikely(HI_FALSE == DmxCluster->teec.connected))
    {
        ret = DmxInitTeec(DmxCluster);
        if (HI_SUCCESS != ret)
        {
            goto out;
        }
    }

    switch(CmdId)
    {
        case TEEC_CMD_LOCK_CHANNEL:
            ret = TEEK_InvokeCommand(&DmxCluster->teec.session_lock, CmdId, Operation, RetOrigin);
            break;
        case TEEC_CMD_UNLOCK_CHANNEL:
            ret = TEEK_InvokeCommand(&DmxCluster->teec.session_unlock, CmdId, Operation, RetOrigin);
            break;
        default:
            ret = TEEK_InvokeCommand(&DmxCluster->teec.session, CmdId, Operation, RetOrigin);
            break;
    }

out:
    return ret;
}

static HI_S32 __DmxTeecRegisterChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    operation.params[0].value.a = DmxId;
    operation.params[0].value.b = ChanId;

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_REGISTER_CHANNEL, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_REGISTER_CHANNEL to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

out:
    return ret;
}

static HI_S32 __DmxTeecUnregisterChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    operation.params[0].value.a = DmxId;
    operation.params[0].value.b = ChanId;

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_UNREGISTER_CHANNEL, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_UNREGISTER_CHANNEL to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

out:
    return ret;
}

#ifdef DMX_SECURE_CHANNEL_SUPPORT
static HI_S32 DmxTeecRegisterChannel(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        return __DmxTeecRegisterChannel(DmxId, ChanId);
    }
    else
    {
        return HI_SUCCESS;
    }
}

static HI_S32 DmxTeecUnregisterChannel(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        return __DmxTeecUnregisterChannel(DmxId, ChanId);
    }
    else
    {
        return HI_SUCCESS;
    }
}

static HI_S32 __DmxTeecRegisterOq(HI_U32 DmxId, HI_U32 OqId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    operation.params[0].value.a = DmxId;
    operation.params[0].value.b = OqId;

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_REGISTER_OQ, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_REGISTER_OQ to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

out:
    return ret;
}

static HI_S32 DmxTeecRegisterOq(HI_U32 DmxId, HI_U32 OqId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        return __DmxTeecRegisterOq(DmxId, OqId);
    }
    else
    {
        return HI_SUCCESS;
    }
}

static HI_S32 __DmxTeecUnregisterOq(HI_U32 DmxId, HI_U32 OqId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    operation.params[0].value.a = DmxId;
    operation.params[0].value.b = OqId;

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_UNREGISTER_OQ, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_UNREGISTER_OQ to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

out:
    return ret;
}

static HI_S32 DmxTeecUnregisterOq(HI_U32 DmxId, HI_U32 OqId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        return __DmxTeecUnregisterOq(DmxId, OqId);
    }
    else
    {
        return HI_SUCCESS;
    }
}

static HI_S32 DmxTeecLockChannel(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_SUCCESS;
    }
}

static HI_VOID DmxTeecUnlockChannel(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        return;
    }
}

#else
static HI_S32 DmxTeecRegisterChannel(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("this chip not support secure extension.\n");

        return HI_ERR_DMX_NOT_SUPPORT;
    }
    else
    {
        return __DmxTeecRegisterChannel(DmxId, ChanId);
    }
}

static HI_S32 DmxTeecUnregisterChannel(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("this chip not support secure extension.\n");

        return HI_ERR_DMX_NOT_SUPPORT;
    }
    else
    {
        return __DmxTeecUnregisterChannel(DmxId, ChanId);
    }
}

static HI_S32 DmxTeecRegisterOq(HI_U32 DmxId, HI_U32 OqId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("this chip not support secure extension.\n");

        return HI_ERR_DMX_NOT_SUPPORT;
    }
    else
    {
        return HI_SUCCESS;
    }
}

static HI_S32 DmxTeecUnregisterOq(HI_U32 DmxId, HI_U32 OqId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("this chip not support secure extension.\n");

        return HI_ERR_DMX_NOT_SUPPORT;
    }
    else
    {
        return HI_SUCCESS;
    }
}

static HI_S32 DmxTeecLockChannel(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    BUG_ON(HI_UNF_DMX_SECURE_MODE_TEE == Secure);

    return HI_SUCCESS;
}

static HI_VOID DmxTeecUnlockChannel(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    BUG_ON(HI_UNF_DMX_SECURE_MODE_TEE == Secure);

    return;
}

#endif

#ifdef DMX_SECURE_RAM_PORT_SUPPORT
static HI_S32 __DmxTeecRegisterRamPort(HI_U32 RamPortId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    operation.params[0].value.a = RamPortId;

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_REGISTER_RAM_PORT, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_REGISTER_RAM_PORT to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

out:
    return ret;
}

static HI_S32 DmxTeecRegisterRamPort(HI_U32 RamPortId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        return __DmxTeecRegisterRamPort(RamPortId);
    }
    else
    {
        return HI_SUCCESS;
    }
}

static HI_S32 __DmxTeecUnregisterRamPort(HI_U32 RamPortId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    operation.params[0].value.a = RamPortId;

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_UNREGISTER_RAM_PORT, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_UNREGISTER_RAM_PORT to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

out:
    return ret;
}

static HI_S32 DmxTeecUnregisterRamPort(HI_U32 RamPortId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        return __DmxTeecUnregisterRamPort(RamPortId);
    }
    else
    {
        return HI_SUCCESS;
    }
}
#else
static HI_S32 DmxTeecRegisterRamPort(HI_U32 RamPortId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("this chip not support secure extension.\n");

        return HI_ERR_DMX_NOT_SUPPORT;
    }
    else
    {
        return HI_SUCCESS;
    }
}

static HI_S32 DmxTeecUnregisterRamPort(HI_U32 RamPortId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("this chip not support secure extension.\n");

        return HI_ERR_DMX_NOT_SUPPORT;
    }
    else
    {
        return HI_SUCCESS;
    }
}
#endif

#else
static inline HI_S32 DmxInitTeec(Dmx_Cluster_S *DmxCluster){return HI_FAILURE;};
static inline HI_VOID DmxDeinitTeec(Dmx_Cluster_S *DmxCluster) {}

static inline HI_S32 DmxCluster_SendCmdToTA(HI_U32 CmdId, TEEC_Operation *Operation, HI_U32 *RetOrigin)
{
    DmxInitTeec(GetDmxCluster());

    return HI_FAILURE;
}

static inline HI_S32 DmxTeecRegisterChannel(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("DMX_TEE_SUPPORT not configured.\n");

        return HI_ERR_DMX_NOT_SUPPORT;
    }
    else
    {
        return HI_SUCCESS;
    }
}

static inline HI_S32 DmxTeecUnregisterChannel(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("DMX_TEE_SUPPORT not configured.\n");

        return HI_ERR_DMX_NOT_SUPPORT;
    }
    else
    {
        return HI_SUCCESS;
    }
}

static inline HI_S32 DmxTeecRegisterOq(HI_U32 DmxId, HI_U32 OqId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("DMX_TEE_SUPPORT not configured.\n");

        return HI_ERR_DMX_NOT_SUPPORT;
    }
    else
    {
        return HI_SUCCESS;
    }
}

static inline HI_S32 DmxTeecUnregisterOq(HI_U32 DmxId, HI_U32 OqId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("DMX_TEE_SUPPORT not configured.\n");

        return HI_ERR_DMX_NOT_SUPPORT;
    }
    else
    {
        return HI_SUCCESS;
    }
}

static inline HI_S32 DmxTeecRegisterRamPort(HI_U32 RamPortId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("DMX_TEE_SUPPORT not configured.\n");

        return HI_ERR_DMX_NOT_SUPPORT;
    }
    else
    {
        return HI_SUCCESS;
    }
}

static inline HI_S32 DmxTeecUnregisterRamPort(HI_U32 RamPortId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("DMX_TEE_SUPPORT not configured.\n");

        return HI_ERR_DMX_NOT_SUPPORT;
    }
    else
    {
        return HI_SUCCESS;
    }
}

static inline HI_S32 DmxTeecLockChannel(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("DMX_TEE_SUPPORT not configured.\n");

        return HI_ERR_DMX_NOT_SUPPORT;
    }
    else
    {
        return HI_SUCCESS;
    }
}

static inline HI_VOID DmxTeecUnlockChannel(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_SECURE_MODE_E Secure)
{
    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == Secure))
    {
        HI_ERR_DEMUX("DMX_TEE_SUPPORT not configured.\n");
    }
}

#endif

/*
 * fq acquire and release helper functions.
 */
static HI_S32 DmxFqAcquire(Dmx_Set_S *DmxSet, HI_U32 *FqId)
{
    HI_S32 ret = HI_ERR_DMX_NOFREE_CHAN;

    do
    {
        HI_U32 index = find_first_zero_bit(DmxSet->FqBitmap, DmxSet->DmxFqCnt);

        if (unlikely(0 == index)) /* reserved 0  as common fq */
        {
            set_bit(0, DmxSet->FqBitmap);
        }
        else if (index < DmxSet->DmxFqCnt)
        {
            DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[index];

            spin_lock(&FqInfo->LockFq);

            /* double check */
            if (index == find_first_zero_bit(DmxSet->FqBitmap, DmxSet->DmxFqCnt))
            {
                set_bit(index, DmxSet->FqBitmap);
                *FqId = index;

                ret = HI_SUCCESS;
            }

            spin_unlock(&FqInfo->LockFq);
        }
        else
        {
            HI_ERR_DEMUX("There is no available FQ now!\n");
            break;
        }
    }while(HI_SUCCESS != ret);

    return ret ;
}

static HI_VOID DmxFqRelease(Dmx_Set_S *DmxSet, HI_U32 FqId)
{
    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[FqId];

    BUG_ON(FqId >= DmxSet->DmxFqCnt);

    spin_lock(&FqInfo->LockFq);

    clear_bit(FqId, DmxSet->FqBitmap);

    spin_unlock(&FqInfo->LockFq);
}

static HI_VOID DmxFqStart(Dmx_Set_S *DmxSet, HI_U32 FqId)
{
    DMX_FQ_Info_S  *FqInfo  = &DmxSet->DmxFqInfo[FqId];
    FQ_DescInfo_S  *FqDesc  = FqInfo->FqDescSetBase;
    HI_U32          PhyAddr = FqInfo->BufPhyAddr;
    HI_U32          i;

    for (i = 0; i < FqInfo->FQDepth - 1; i++)
    {
        FqDesc->start_addr  = PhyAddr;
        FqDesc->buflen      = FqInfo->BlockSize;

        PhyAddr += FqInfo->BlockSize;

        ++FqDesc;
    }

    FqDesc->start_addr  = 0;
    FqDesc->buflen      = 0;

    DmxHalSetFQWORDx(DmxSet, FqId, DMX_FQ_CTRL_OFFSET, DMX_FQ_ALOVF_CNT << 24);
    DmxHalSetFQWORDx(DmxSet, FqId, DMX_FQ_RDVD_OFFSET, 0);
    DmxHalSetFQWORDx(DmxSet, FqId, DMX_FQ_SZWR_OFFSET, (FqInfo->FQDepth << 16) | ((FqInfo->FQDepth - 1) & 0xffff));
    DmxHalSetFQWORDx(DmxSet, FqId, DMX_FQ_START_OFFSET, FqInfo->FqDescSetPhy);

    FqInfo->FqOverflowCount = 0;
    DmxHalFQClearOverflowInt(DmxSet, FqId);
    DmxHalFQSetOverflowInt(DmxSet, FqId, DMX_ENABLE);

    DmxHalFQEnableRecive(DmxSet, FqId, DMX_ENABLE);
}

static HI_VOID DmxFqStop(Dmx_Set_S *DmxSet, HI_U32 FqId)
{
    DmxHalFQEnableRecive(DmxSet, FqId, DMX_DISABLE);

    DmxHalFQSetOverflowInt(DmxSet, FqId, DMX_DISABLE);
    DmxHalFQClearOverflowInt(DmxSet, FqId);

    DmxHalSetFQWORDx(DmxSet, FqId, DMX_FQ_CTRL_OFFSET, 0);
    DmxHalSetFQWORDx(DmxSet, FqId, DMX_FQ_RDVD_OFFSET, 0);
    DmxHalSetFQWORDx(DmxSet, FqId, DMX_FQ_SZWR_OFFSET, 0);
    DmxHalSetFQWORDx(DmxSet, FqId, DMX_FQ_START_OFFSET, 0);
}

static HI_VOID DmxFqCheckOverflowInt(Dmx_Set_S *DmxSet, HI_U32 FqId)
{
    if (!DmxHalFQIsEnableOverflowInt(DmxSet, FqId))
    {
        DmxHalFQSetOverflowInt(DmxSet, FqId, DMX_ENABLE);
    }
}

/*
 * oq acquire and release helper functions.
 */
static HI_S32 DmxOqAcquire(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_UNF_DMX_SECURE_MODE_E Secure, HI_U32 *OqId)
{
    HI_S32 ret = HI_ERR_DMX_NOFREE_CHAN;

    BUG_ON(DmxSet != GetDmxSetByDmxid(DmxId));

    do
    {
        HI_U32 index = find_first_zero_bit(DmxSet->OqBitmap, DmxSet->DmxOqCnt);

        if (index < DmxSet->DmxOqCnt)
        {
            DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[index];

            mutex_lock(&OqInfo->LockOq);

            /* double check */
            if (index == find_first_zero_bit(DmxSet->OqBitmap, DmxSet->DmxOqCnt))
            {
                ret = DmxTeecRegisterOq(DmxId, index, Secure);
                if (HI_SUCCESS != ret)
                {
                    mutex_unlock(&OqInfo->LockOq);
                    break;
                }

                BUG_ON(index != OqInfo->u32OQId);
                OqInfo->enOQBufMode    = DMX_OQ_MODE_UNUSED;
                OqInfo->SecureMode     = Secure;
                OqInfo->u32AttachId    = DMX_INVALID_CHAN_ID;
                OqInfo->OqDescSetBase  = HI_NULL;
                OqInfo->OqDescSetPhy   = 0;
                OqInfo->OQDepth        = 0;
                OqInfo->u32FQId        = DMX_INVALID_FQ_ID;
                OqInfo->u32ProcsBlk    = 0;
                OqInfo->u32ProcsOffset = 0;
                OqInfo->u32ReleaseBlk  = 0;
                OqInfo->u32ReleaseOffset = 0;

                set_bit(index, DmxSet->OqBitmap);
                *OqId = index;

                ret = HI_SUCCESS;
            }

            mutex_unlock(&OqInfo->LockOq);
        }
        else
        {
            HI_ERR_DEMUX("There is no available OQ now!\n");
            break;
        }
    }while(HI_SUCCESS != ret);

    return ret;
}

static HI_VOID DmxOqRelease(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 OqId)
{
    DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[OqId];

    BUG_ON(DmxSet != GetDmxSetByDmxid(DmxId));
    BUG_ON(OqId >= DmxSet->DmxOqCnt);

    mutex_lock(&OqInfo->LockOq);

    DmxTeecUnregisterOq(DmxId, OqId, OqInfo->SecureMode);

    clear_bit(OqId, DmxSet->OqBitmap);

    mutex_unlock(&OqInfo->LockOq);
}

static HI_VOID DmxOqStart(Dmx_Set_S *DmxSet, HI_U32 OqId)
{
    DMX_OQ_Info_S  *OqInfo  = &DmxSet->DmxOqInfo[OqId];

    memset((HI_VOID*)OqInfo->OqDescSetBase, 0, OqInfo->OQDepth * DMX_OQ_DESC_SIZE);

    OqInfo->u32ProcsBlk         = 0;
    OqInfo->u32ProcsOffset      = 0;
    OqInfo->u32ReleaseBlk       = 0;
    OqInfo->u32ReleaseOffset    = 0;

    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_RSV_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_CTRL_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_EOPWR_OFFSET, 0 );
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_SZUS_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_SADDR_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_RDWR_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_CFG_OFFSET, DMX_OQ_OUTINT_CNT << 26 |
                     OqInfo->OQDepth << 16 | (DMX_OQ_ALOVF_CNT & 0xff) << 8 | OqInfo->u32FQId);
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_START_OFFSET, OqInfo->OqDescSetPhy);

    DmxHalResetOqCounter(DmxSet, OqInfo->u32OQId);

    DmxHalEnableOQMmu(DmxSet, OqId);

    DmxHalEnableOQRecive(DmxSet, OqInfo->u32OQId);
}

static HI_VOID DmxOqStop(Dmx_Set_S *DmxSet, HI_U32 OqId)
{
    DMX_OQ_Info_S  *OqInfo  = &DmxSet->DmxOqInfo[OqId];

    DmxHalOQEnableOutputInt(DmxSet, OqInfo->u32OQId, HI_FALSE);

    DmxHalDisableOQRecive(DmxSet, OqInfo->u32OQId);

    DmxHalDisableOQMmu(DmxSet, OqInfo->u32OQId);

    DmxHalResetOqCounter(DmxSet, OqInfo->u32OQId);

    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_RSV_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_CTRL_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_EOPWR_OFFSET, 0 );
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_SZUS_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_SADDR_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_RDWR_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_CFG_OFFSET, 0);
    DmxHalSetOQWORDx(DmxSet, OqInfo->u32OQId, DMX_OQ_START_OFFSET, 0);
}

 HI_S32 DmxOQRelease(Dmx_Set_S *DmxSet, HI_U32 FqId, HI_U32 OqId, HI_U32 PhyAddr, HI_U32 len)
{
    DMX_FQ_Info_S  *FqInfo  = &DmxSet->DmxFqInfo[FqId];
    DMX_OQ_Info_S  *OqInfo  = &DmxSet->DmxOqInfo[OqId];
    OQ_DescInfo_S  *OqDesc;
    FQ_DescInfo_S  *FqDesc;
    HI_U32          OqRead;
    HI_U32          FqWrite;
    HI_SIZE_T       u32LockFlag;

    DMXOsiOQGetReadWrite(DmxSet, OqInfo->u32OQId, HI_NULL, &OqRead);

    OqDesc = OqInfo->OqDescSetBase + OqRead;

    if (!(OqDesc->start_addr >= FqInfo->BufPhyAddr && OqDesc->start_addr < FqInfo->BufPhyAddr + FqInfo->BufSize))
    {
        HI_ERR_DEMUX("Invalid OQ(%d) descriptor[0x%x, 0x%x].\n", OqInfo->u32OQId, OqDesc->start_addr, OqDesc->cactrl_buflen & 0xffff);

        return HI_ERR_DMX_INVALID_PARA;
    }

    if ((DMX_OQ_MODE_REC == OqInfo->enOQBufMode) || (DMX_OQ_MODE_SCD == OqInfo->enOQBufMode))
    {
        HI_U32 DataLen = OqDesc->pvrctrl_datalen & 0xffff;

        if ((PhyAddr != OqDesc->start_addr) || (DataLen != len))
        {
            HI_ERR_DEMUX("buf %d release wrong data block\n", OqInfo->u32OQId);

            return HI_ERR_DMX_INVALID_PARA;
        }
        /*Delete the following code, because:
        Customers demands that rec data can be acquire several times then release them one by one.
        so we changed the DMX_DRV_REC_AcquireRecData and use the OqInfo->u32ProcsBlk to judge wether there
        have data in Rec buf. and ,every time after acquired a block of rec data (47K),
        the OqInfo->u32ProcsBlk++ , so .at here ,it can not  be added again*/
        DMXINC(OqInfo->u32ProcsBlk, OqInfo->OQDepth);
        OqInfo->u32ProcsOffset = 0;
    }

    DMXINC(OqRead, OqInfo->OQDepth);

    DmxHalSetOQReadPtr(DmxSet, OqInfo->u32OQId, OqRead);
    spin_lock_irqsave(&FqInfo->LockFq, u32LockFlag);

    DmxHalGetFQReadWritePtr(DmxSet, FqId, HI_NULL, &FqWrite);

    FqDesc = FqInfo->FqDescSetBase + FqWrite;

    FqDesc->start_addr  = OqDesc->start_addr;
    FqDesc->buflen      = OqDesc->cactrl_buflen & 0xffff;

    DMXINC(FqWrite, FqInfo->FQDepth);

    DmxHalSetFQWritePtr(DmxSet, FqId, FqWrite);

    spin_unlock_irqrestore(&FqInfo->LockFq, u32LockFlag);

    DmxFqCheckOverflowInt(DmxSet, FqId);

    return HI_SUCCESS;
}

HI_S32 DmxOQReleaseByBlockCnt(Dmx_Set_S *DmxSet, HI_U32 FqId, HI_U32 OqId, HI_U32 BlockCnt)
{
    DMX_FQ_Info_S  *FqInfo  = &DmxSet->DmxFqInfo[FqId];
    DMX_OQ_Info_S  *OqInfo  = &DmxSet->DmxOqInfo[OqId];
    OQ_DescInfo_S  *OqDesc;
    FQ_DescInfo_S  *FqDesc;
    HI_U32          OqRead;
    HI_U32          FqWrite;
    HI_SIZE_T       u32LockFlag;
    HI_U32 i = 0;

    DMXOsiOQGetReadWrite(DmxSet, OqInfo->u32OQId, HI_NULL, &OqRead);

    DmxHalGetFQReadWritePtr(DmxSet, FqId, HI_NULL, &FqWrite);

    for ( i = 0 ; i < BlockCnt ; i++ )
    {
        OqDesc = OqInfo->OqDescSetBase + OqRead;
        FqDesc = FqInfo->FqDescSetBase + FqWrite;

        FqDesc->start_addr  = OqDesc->start_addr;
        FqDesc->buflen      = OqDesc->cactrl_buflen & 0xffff;

        if (unlikely(!(FqDesc->start_addr >= FqInfo->BufPhyAddr && FqDesc->start_addr < FqInfo->BufPhyAddr + FqInfo->BufSize)))
        {
            HI_FATAL_DEMUX("invalid desc[0x%x, %d] of fq[0x%x, %d].\n", FqDesc->start_addr, FqDesc->buflen, FqInfo->BufPhyAddr, FqInfo->BufSize);
            BUG();
        }

        DMXINC(OqRead, OqInfo->OQDepth);
        DMXINC(FqWrite, FqInfo->FQDepth);
    }

    DmxHalSetOQReadPtr(DmxSet, OqInfo->u32OQId, OqRead);
    spin_lock_irqsave(&FqInfo->LockFq, u32LockFlag);
    DmxHalSetFQWritePtr(DmxSet, FqId, FqWrite);
    spin_unlock_irqrestore(&FqInfo->LockFq, u32LockFlag);

    DmxFqCheckOverflowInt(DmxSet, FqId);

    return HI_SUCCESS;
}

/*
 * channel acquire and release helper functions.
 */
static HI_S32 DmxCreateChannel(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr);
static HI_S32 DmxDestroyChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId);

static HI_S32 DmxChnAcquire(HI_U32 DmxId, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr, HI_U32 *ChanId, DMX_MMZ_BUF_S *ChanBuf)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    BUG_ON(!mutex_is_locked(&DmxSet->LockAllChn));

    *ChanId = find_first_zero_bit(DmxSet->ChnBitmap, DmxSet->DmxChanCnt);
    if (!(*ChanId < DmxSet->DmxChanCnt))
    {
        HI_ERR_DEMUX("There is no available channel now!\n");
        ret = HI_ERR_DMX_NOFREE_CHAN;
        goto out0;
    }

    ChanInfo = &DmxSet->DmxChanInfo[*ChanId];

    mutex_lock(&ChanInfo->LockChn);

    ret = DmxCreateChannel(DmxSet, DmxId, *ChanId, ChanAttr);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    if (ChanBuf)
    {
        DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
        DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];

        ChanBuf->VirAddr      = FqInfo->BufVirAddr;
        ChanBuf->PhyAddr      = FqInfo->BufPhyAddr;
        ChanBuf->Size         = FqInfo->BufSize;
        ChanBuf->Flag         = FqInfo->BufFlag;
    }

    ret = DmxTeecRegisterChannel(DmxId, *ChanId, ChanInfo->ChanSecure);
    if (HI_SUCCESS != ret)
    {
        goto out2;
    }

    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == ChanInfo->ChanSecure))
    {
        if (ChanBuf)
        {
            DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
            DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];

            if (HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType)
            {
                ChanBuf->VirAddr      = FqInfo->ShadowBufVirAddr;
                ChanBuf->PhyAddr      = FqInfo->ShadowBufPhyAddr;
                ChanBuf->Size         = FqInfo->ShadowBufSize;
                ChanBuf->Flag         = FqInfo->ShadowBufFlag;
            }
        }
    }

    set_bit(*ChanId, DmxSet->ChnBitmap);

    mutex_unlock(&ChanInfo->LockChn);

    return HI_SUCCESS;

out2:
     DmxDestroyChannel(DmxSet, *ChanId);
out1:
     mutex_unlock(&ChanInfo->LockChn);
out0:
    return ret;
}

static HI_S32 DmxChnRelease(HI_U32 DmxId, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    BUG_ON(ChanId >= DmxSet->DmxChanCnt);
    BUG_ON(!mutex_is_locked(&ChanInfo->LockChn));

    DmxTeecUnregisterChannel(DmxId, ChanId, ChanInfo->ChanSecure);

    ret = DmxDestroyChannel(DmxSet, ChanId);

    clear_bit(ChanId, DmxSet->ChnBitmap);

    return ret;
}

/*
 * exclusively get and put channel helper functions.
 */
HI_S32 DmxGetChnInstance(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    unsigned long mask, *p;
    DMX_ChanInfo_S  *ChanInfo = HI_NULL;

    if (unlikely(!DmxSet))
    {
        HI_ERR_DEMUX("Invalid DemuxSet.\n");
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    if (unlikely(ChanId >= DmxSet->DmxChanCnt))
    {
        HI_ERR_DEMUX("Invalid channel id :0x%x\n", ChanId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if (unlikely(0 != mutex_lock_interruptible(&ChanInfo->LockChn)))
    {
        HI_WARN_DEMUX("Channel mutex interruptibled.\n");

        ret = HI_ERR_DMX_BUSY_CHAN;
        goto out0;
    }

    mask = BIT_MASK(ChanId);
    p = ((unsigned long*)DmxSet->ChnBitmap) + BIT_WORD(ChanId);
    if (unlikely(!(*p & mask)))
    {
        HI_DBG_DEMUX("This channel(%d) instance has not alloced.\n", ChanId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    ret = DmxTeecLockChannel(ChanInfo->DmxId, ChanId, ChanInfo->ChanSecure);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    return HI_SUCCESS;

out1:
    mutex_unlock(&ChanInfo->LockChn);
out0:
    return ret;
}

HI_VOID DmxPutChnInstance(Dmx_Set_S *DmxSet, HI_U32 ChnId)
{
    DMX_ChanInfo_S  *ChanInfo = &DmxSet->DmxChanInfo[ChnId];

    BUG_ON(ChnId >= DmxSet->DmxChanCnt);
    BUG_ON(!mutex_is_locked(&ChanInfo->LockChn));

    DmxTeecUnlockChannel(ChanInfo->DmxId, ChnId, ChanInfo->ChanSecure);

    mutex_unlock(&ChanInfo->LockChn);
}

/*
 * diff with DmxPutChnInstance, DmxPutChnInstanceEx used for protecting DmxChnRelease which will set
 * ChanInfo->DmxId invalid.
 */
HI_VOID DmxPutChnInstanceEx(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 ChnId)
{
    DMX_ChanInfo_S  *ChanInfo = &DmxSet->DmxChanInfo[ChnId];

    BUG_ON(ChnId >= DmxSet->DmxChanCnt);
    BUG_ON(!mutex_is_locked(&ChanInfo->LockChn));

    DmxTeecUnlockChannel(DmxId, ChnId, ChanInfo->ChanSecure);

    mutex_unlock(&ChanInfo->LockChn);
}

static HI_VOID DmxSetChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY == (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode))
    {
        DmxHalSetChannelPlayBufId(DmxSet, ChanId, ChanInfo->ChanOqId);
    }

    if ((HI_UNF_DMX_CHAN_TYPE_SEC == ChanInfo->ChanType) || (HI_UNF_DMX_CHAN_TYPE_ECM_EMM == ChanInfo->ChanType))
    {
        DmxHalSetChannelDataType(DmxSet, ChanId, DMX_CHAN_DATA_TYPE_SEC);
    }
    else
    {
        DmxHalSetChannelDataType(DmxSet, ChanId, DMX_CHAN_DATA_TYPE_PES);
    }

    DmxHalSetChannelTsPostMode(DmxSet, ChanId, DMX_DISABLE);
    DmxHalSetChannelPusiCtrl(DmxSet, ChanId, DMX_DISABLE);

    if (HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType)
    {
        DmxHalSetChannelAttr(DmxSet, ChanId, DMX_CH_AUDIO);
    }
    else  if (HI_UNF_DMX_CHAN_TYPE_VID == ChanInfo->ChanType)
    {
        DmxHalSetChannelAttr(DmxSet, ChanId, DMX_CH_VIDEO);
    }
    else if (HI_UNF_DMX_CHAN_TYPE_PES == ChanInfo->ChanType)
    {
        DmxHalSetChannelAttr(DmxSet, ChanId, DMX_CH_PES);
    }
    else
    {
        DmxHalSetChannelAttr(DmxSet, ChanId, DMX_CH_GENERAL);
        if (HI_UNF_DMX_CHAN_TYPE_POST == ChanInfo->ChanType)
        {
            DmxHalSetChannelTsPostMode(DmxSet, ChanId, DMX_ENABLE);
            DmxHalSetChannelTsPostThresh(DmxSet, ChanId, DMX_DEFAULT_POST_TH);
            DmxHalSetChannelPusiCtrl(DmxSet, ChanId, DMX_ENABLE);    // receive the ts packet do not have pusi
        }
    }

    DmxHalSetChannelCRCMode(DmxSet, ChanId, ChanInfo->ChanCrcMode);
    DmxHalSetChannelAFMode(DmxSet, ChanId, DMX_AF_DISCARD);
    DmxHalSetChannelCCDiscon(DmxSet, ChanId, DMX_DISABLE);
    DmxHalSetChannelCCRepeatCtrl(DmxSet, ChanId, DMX_ENABLE);
    DmxHalSetChannelPid(DmxSet, ChanId, ChanInfo->ChanPid);
}

static HI_S32 DMXCheckBuffer(HI_U8 *BufKerAddr,
                               HI_U32 u32BufLen,
                               HI_U32 u32Offset)
{
    HI_U32 u32SecLen, u32BufLenTmp;
    HI_U8 *Buf;

    u32BufLenTmp = u32BufLen - u32Offset;
    while (u32BufLenTmp > 3)
    {
        u32SecLen  = 3;
        Buf = BufKerAddr + u32Offset;
        if (unlikely(u32BufLen < u32Offset))
        {
            HI_ERR_DEMUX("invalide offset,u32BufLen:%d,u32Offset:%d!\n",u32BufLen,u32Offset);
            return -1;
        }

        u32SecLen += (((*(HI_U8 *)(Buf + 1)) & 0xF) << 8);
        u32SecLen += *(HI_U8 *)(Buf + 2);
        if (unlikely((u32SecLen > u32BufLenTmp) || (u32SecLen > DMX_MAX_SEC_LEN)))
        {
            HI_ERR_DEMUX("invalid u32SecLen, u32SecLen:%d,u32BufLenTmp:%d!\n",u32SecLen,u32BufLenTmp);
            return -1;
        }

        u32Offset    += u32SecLen;
        u32BufLenTmp -= u32SecLen;
    }

    return 0;
}

HI_U32 GetSectionLength(HI_U8 *BufKerAddr, HI_U32 u32BufLen, HI_U32 u32Offset)
{
    HI_U32 u32PacketLen = 0;
    HI_U8 *BufAddr, *EndAddr;

    if (unlikely(0 == BufKerAddr || 0 == u32BufLen))
    {
        HI_ERR_DEMUX("invalid buffer start addr(0x%x) or len(%d).\n", BufKerAddr, u32BufLen);
        return 0;
    }

    if (unlikely(u32Offset >= u32BufLen))
    {
        HI_ERR_DEMUX("u32Offset is larger than buflen,%x!\n", u32Offset);
        return 0;
    }

    if (DMXCheckBuffer(BufKerAddr, u32BufLen, u32Offset) != 0)
    {
        return 0;
    }

    if (u32BufLen >= 3)
    {
        u32PacketLen = 3;
        BufAddr = BufKerAddr + u32Offset;
        EndAddr = BufKerAddr + u32BufLen;
        if (unlikely(BufAddr >= EndAddr))
        {
            HI_ERR_DEMUX("u32Offset is larger than buflen!\n");
            return 0;
        }

        u32PacketLen += (((*(HI_U8 *)(BufAddr + 1)) & 0xF) << 8);
        if (unlikely(BufAddr >= EndAddr))
        {
            HI_ERR_DEMUX("u32Offset is larger than buflen!\n");
            return 0;
        }

        u32PacketLen += *(HI_U8 *)(BufAddr + 2);
        if ((u32BufLen - u32Offset) < u32PacketLen)
        {
            u32PacketLen = 0;
        }
    }

    return u32PacketLen;
}

static HI_S32 RamPortClean(HI_U32 PortId)
{
    HI_U32 i;
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);

    for (i = 0; i < DmxSet->DmxCnt; i++)
    {
        if (DMX_PORT_MODE_TUNER == DmxSet->DmxInfo[i].PortMode)
        {
            continue;
        }

        if (PortId == DmxSet->DmxInfo[i].PortId)
        {
            /* Select the dmx port to invalid, to flush remain data*/
            __DmxHalSetDemuxPortId(DmxSet, i, DMX_PORT_MODE_BUTT, DMX_INVALID_PORT_ID);
        }
    }

    DmxHalSetFlushIPPort(DmxSet, PortId);
    DmxHalClrIPBPStatus(DmxSet, PortId);

    udelay(10);

    for (i = 0; i < DmxSet->DmxCnt; i++)
    {
        if (DMX_PORT_MODE_TUNER ==  DmxSet->DmxInfo[i].PortMode)
        {
            continue;
        }

        if (PortId == DmxSet->DmxInfo[i].PortId)
        {
            /* Select the dmx port to set back*/
            __DmxHalSetDemuxPortId(DmxSet, i, DMX_PORT_MODE_RAM, PortId);
        }
    }

#ifdef DMX_TS_BUF_VERSION_2
    /* reset buff blk map */
    if (PortInfo->pstBufBlkMap)  // map maybe have deleted
    {
        HI_U32 index;

        atomic_set(&PortInfo->FreeBufBlkNR, PortInfo->TotalBufBlkNR);
        memset(PortInfo->pstBufBlkMap, 0, sizeof(*PortInfo->pstBufBlkMap) * PortInfo->TotalBufBlkNR);

        for (index = 0; index < PortInfo->TotalBufBlkNR; index ++)
        {
            PortInfo->pstBufBlkMap[index].HeadBlkIdx = INVALID_HEAD_BLK_IDX;
        }
    }

    atomic_set(&PortInfo->LastDescReadIdx, 0);
    atomic_set(&PortInfo->LastDescWriteIdx, 0);
#else
    PortInfo->Read  = 0;
    PortInfo->Write = 0;
#endif

    PortInfo->DescWrite     = 0;
    PortInfo->DescCurrAddr  = (HI_U32*)PortInfo->DescKerAddr;

    PortInfo->GetCount      = 0;
    PortInfo->GetValidCount = 0;
    PortInfo->PutCount      = 0;

    return HI_SUCCESS;
}

#ifdef DMX_TS_BUF_VERSION_2
/*
 * ts buffer get/push/release helper functions.
 */
 static inline HI_U32 CalcBlkCnt(DMX_RamPort_Info_S *PortInfo, DMX_DATA_BUF_S *pstData)
{
    HI_U32 BuffPhyAddr = pstData->BufPhyAddr;
    HI_U32 Len = pstData->BufLen;
    HI_U32 BlkCnt = 0;

    /* calc blk nums */
    do
    {
        HI_U32 len = DMX_MAX_BLOCK_SIZE - (BuffPhyAddr - PortInfo->PhyAddr) % DMX_MAX_BLOCK_SIZE;
        len = len <= Len ? len : Len;

        if (0 == len)
        {
            break;
        }

        BuffPhyAddr += len;
        Len -= len;
        BlkCnt ++;
    }while(1);

    return BlkCnt;
}

static HI_S32 IterBufBlkMap(DMX_RamPort_Info_S *PortInfo, HI_U32 BlkNR, HI_U32 * pBufBlkPhyAddr, HI_U32 *pStartSearchIdx)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 index;

    for (index = *pStartSearchIdx; index < PortInfo->TotalBufBlkNR; index ++)
    {
        if (FREE_BLK_FLAG == PortInfo->pstBufBlkMap[index].BlkFlag)
        {
            HI_BOOL bFound = HI_TRUE;
            HI_U32 i;

            /* Check the adjacent blocks all free */
            for (i = index + 1 ; i < index + BlkNR; i++)
            {
                if (!(i < PortInfo->TotalBufBlkNR) || FREE_BLK_FLAG != PortInfo->pstBufBlkMap[i].BlkFlag)
                {
                    bFound = HI_FALSE;
                    break;
                }
            }

            if (HI_TRUE == bFound)
            {
                if (pBufBlkPhyAddr)
                {
                    *pBufBlkPhyAddr = PortInfo->PhyAddr + index * DMX_MAX_BLOCK_SIZE;
                }

                *pStartSearchIdx = index + BlkNR;
                ret = HI_SUCCESS;
                break;
            }
        }
    }

    return ret;
}

static HI_S32 SearchFreeBufBlk(DMX_RamPort_Info_S *PortInfo, HI_U32 BlkNR, HI_U32 * pBufBlkPhyAddr)
{
    HI_S32 ret = HI_FAILURE;
    static HI_U32 SearchIdx = 0;

    /* fastpath */
    if (HI_SUCCESS == IterBufBlkMap(PortInfo, BlkNR, pBufBlkPhyAddr, &SearchIdx))
    {
        ret = HI_SUCCESS;
        goto out;
    }

    /* slowpath: reset search from start idx */
    SearchIdx = 0;
    if (HI_SUCCESS == IterBufBlkMap(PortInfo, BlkNR, pBufBlkPhyAddr, &SearchIdx))
    {
        ret =  HI_SUCCESS;
        goto out;
    }

out:
    return ret;
}

HI_S32 AllocFreeBuf(DMX_RamPort_Info_S *PortInfo, HI_U32 ReqBufSize, HI_U32 *pBufBlkPhyAddr)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 BlkFrameIdx;
    HI_U32 BufBlkNR = (ReqBufSize  + DMX_MAX_BLOCK_SIZE - 1) / DMX_MAX_BLOCK_SIZE;
    HI_U32 index;
    HI_SIZE_T LockFlag;

    spin_lock_irqsave(&(PortInfo->LockRamPort), LockFlag);

    if (!PortInfo || !pBufBlkPhyAddr)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    /* fast path */
    if (atomic_read(&PortInfo->FreeBufBlkNR) < BufBlkNR)
    {
        ret = HI_ERR_DMX_NOAVAILABLE_BUF;
        goto out;
    }

    /* slow path */
    ret = SearchFreeBufBlk(PortInfo, BufBlkNR, pBufBlkPhyAddr);
    if (HI_SUCCESS != ret)
    {
        HI_DBG_DEMUX("No found free buff.\n");
        ret = HI_ERR_DMX_NOAVAILABLE_BUF;
        goto out;
    }

    /* set used tag */
    BlkFrameIdx = (*pBufBlkPhyAddr - PortInfo->PhyAddr) / DMX_MAX_BLOCK_SIZE;
    for (index = BlkFrameIdx;index < BlkFrameIdx + BufBlkNR; index ++)
    {
        PortInfo->pstBufBlkMap[index].BlkFlag = ALLOC_BLK_FLAG;
        PortInfo->pstBufBlkMap[index].BlkRef = 1;
        PortInfo->pstBufBlkMap[index].HeadBlkIdx = BlkFrameIdx;
        PortInfo->pstBufBlkMap[index].ReqLen = ReqBufSize;

        atomic_dec(&PortInfo->FreeBufBlkNR);
    }

    ret = HI_SUCCESS;

out:
    spin_unlock_irqrestore(&(PortInfo->LockRamPort), LockFlag);

    return ret;
}

HI_S32 FreeBuffBlk(DMX_RamPort_Info_S *PortInfo, DMX_DATA_BUF_S *pstData)
{
    HI_U32 BuffStartBlkIdx, TotalBlkNum;
    HI_U32 index;
    HI_SIZE_T LockFlag;

    spin_lock_irqsave(&(PortInfo->LockRamPort), LockFlag);

    BuffStartBlkIdx = (pstData->BufPhyAddr - PortInfo->PhyAddr) / DMX_MAX_BLOCK_SIZE;
    TotalBlkNum = CalcBlkCnt(PortInfo, pstData);

    BUG_ON(BuffStartBlkIdx != PortInfo->pstBufBlkMap[BuffStartBlkIdx].HeadBlkIdx);

    for (index = BuffStartBlkIdx; index < BuffStartBlkIdx + TotalBlkNum; index ++)
    {
        switch(PortInfo->pstBufBlkMap[index].BlkFlag)
        {
            case ALLOC_BLK_FLAG:
            {
                PortInfo->pstBufBlkMap[index].BlkRef --;

                BUG_ON(0 != PortInfo->pstBufBlkMap[index].BlkRef);

                PortInfo->pstBufBlkMap[index].BlkFlag = FREE_BLK_FLAG;
                PortInfo->pstBufBlkMap[index].HeadBlkIdx = INVALID_HEAD_BLK_IDX;
                PortInfo->pstBufBlkMap[index].ReqLen = 0;

                atomic_inc(&PortInfo->FreeBufBlkNR);
                break;
            }
            case ADD_INT_FLAG:
            {
                if (1 == PortInfo->pstBufBlkMap[index].BlkRef) /* blk can be safe release now */
                {
                    PortInfo->pstBufBlkMap[index].BlkFlag = FREE_BLK_FLAG;
                    PortInfo->pstBufBlkMap[index].HeadBlkIdx = INVALID_HEAD_BLK_IDX;
                    PortInfo->pstBufBlkMap[index].ReqLen = 0;
                    PortInfo->pstBufBlkMap[index].BlkRef = 0;

                    atomic_inc(&PortInfo->FreeBufBlkNR);
                }
                else if (PortInfo->pstBufBlkMap[index].BlkRef > 1) /* blk will be release after int processed */
                {
                    PortInfo->pstBufBlkMap[index].BlkFlag = ADD_RELCAIM_INT_FLAG;
                    PortInfo->pstBufBlkMap[index].HeadBlkIdx = INVALID_HEAD_BLK_IDX;
                    PortInfo->pstBufBlkMap[index].ReqLen = 0;
                    PortInfo->pstBufBlkMap[index].BlkRef --;
                }
                else
                {
                    BUG();
                }

                break;
            }
            default:
                BUG();
        }

        BUG_ON(atomic_read(&PortInfo->FreeBufBlkNR) > PortInfo->TotalBufBlkNR);
    }

    spin_unlock_irqrestore(&(PortInfo->LockRamPort), LockFlag);

    return HI_SUCCESS;
}

static HI_S32 PingIPBuf(Dmx_Set_S *DmxSet, HI_U32 RawPortId, HI_U32 ReqBufSize)
{
    HI_S32 ret;
    DMX_RamPort_Info_S *PortInfo = &DmxSet->RamPortInfo[RawPortId];
    HI_U32 FreeDesc, AddDesc, DescReadIdx, DescWriteIdx;
    HI_U32 BufBlkNR = (ReqBufSize  + DMX_MAX_BLOCK_SIZE - 1) / DMX_MAX_BLOCK_SIZE;

    /* 1: check free desc */
    DescReadIdx = atomic_read(&PortInfo->LastDescReadIdx);
    DescWriteIdx = atomic_read(&PortInfo->LastDescWriteIdx);
    FreeDesc = PortInfo->DescDepth - GetQueueLenth(DescReadIdx, DescWriteIdx, PortInfo->DescDepth);
    AddDesc  = ReqBufSize / DMX_MAX_BLOCK_SIZE + 2; /* +2 : self and null desc */
    if (AddDesc >= FreeDesc)
    {
        HI_DBG_DEMUX("no free desc add:%x, free:%x, r:%x, w:%x, u32DataSize:%x!\n", AddDesc, FreeDesc, DescReadIdx,
                      DescWriteIdx, ReqBufSize);
        ret = HI_FAILURE;
        goto out;
    }

    /* fast path */
    if (atomic_read(&PortInfo->FreeBufBlkNR) < BufBlkNR)
    {
        ret = HI_FAILURE;
        goto out;
    }

    /* slow path */
    if (HI_SUCCESS != SearchFreeBufBlk(PortInfo, BufBlkNR, NULL))
    {
        HI_DBG_DEMUX("buffer not enough, req size:0x%x!\n", ReqBufSize);
        ret = HI_FAILURE;
        goto out;
    }

    ret = HI_SUCCESS;

out:
    return ret;
}

static HI_S32 CheckIPBuf(Dmx_Set_S *DmxSet, HI_U32 PortId, HI_U32 ReqBufSize, HI_U32 *pBufPhyAddr)
{
    DMX_RamPort_Info_S *PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);
    HI_U32 FreeDesc, DescReadIdx, DescWriteIdx, AddDesc;

    /* 1: check free desc */
    DescReadIdx = atomic_read(&PortInfo->LastDescReadIdx);
    DescWriteIdx = atomic_read(&PortInfo->LastDescWriteIdx);
    FreeDesc = PortInfo->DescDepth - GetQueueLenth(DescReadIdx, DescWriteIdx, PortInfo->DescDepth);
    AddDesc  = ReqBufSize / DMX_MAX_BLOCK_SIZE + 2; /* +2 : self and null desc */
    if (AddDesc >= FreeDesc)
    {
        HI_DBG_DEMUX("no free desc add:%x, free:%x, r:%x, w:%x, ReqBufSize:%x!\n", AddDesc, FreeDesc, DescReadIdx,
                      DescWriteIdx, ReqBufSize);
        return HI_FAILURE;
    }

    /* 2: check free buff */
    if (HI_SUCCESS != AllocFreeBuf(PortInfo, ReqBufSize, pBufPhyAddr))
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 VerifyPushBuffBlk(DMX_RamPort_Info_S *PortInfo, DMX_DATA_BUF_S *pstData)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 StartBlkIdx, TotalBlkNum, HeadBlkIdx;
    HI_U32 index;

    StartBlkIdx = (pstData->BufPhyAddr - PortInfo->PhyAddr) / DMX_MAX_BLOCK_SIZE;
    TotalBlkNum = CalcBlkCnt(PortInfo, pstData);

    if (!PortInfo ||
        pstData->BufPhyAddr < PortInfo->PhyAddr ||
        pstData->BufLen > PortInfo->BufSize ||
        0 == pstData->BufLen ||
        StartBlkIdx + TotalBlkNum > PortInfo->TotalBufBlkNR)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    /* all blk have same headblkidx which means they belong to one big buffer */
    HeadBlkIdx = PortInfo->pstBufBlkMap[StartBlkIdx].HeadBlkIdx;

    for (index = StartBlkIdx; index < StartBlkIdx + TotalBlkNum; index ++)
    {
        /* has not allocated or head blk idx is wrong */
        if (FREE_BLK_FLAG == PortInfo->pstBufBlkMap[index].BlkFlag ||
                HeadBlkIdx != PortInfo->pstBufBlkMap[index].HeadBlkIdx)
        {
            ret = HI_ERR_DMX_INVALID_PARA;
            goto out;
        }

        /* the blk tag must be ALLOC_BLK_TAG or ADD_INT_TAG */
        WARN(ALLOC_BLK_FLAG != PortInfo->pstBufBlkMap[index].BlkFlag && ADD_INT_FLAG != PortInfo->pstBufBlkMap[index].BlkFlag,
                    "blk(%d) tag(0x%x) is wrong.", index, PortInfo->pstBufBlkMap[index].BlkFlag);
    }

    /* user can only push data within user defined range */
    if (pstData->BufLen > PortInfo->pstBufBlkMap[StartBlkIdx].ReqLen)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    ret = HI_SUCCESS;

out:

    return ret;
}

HI_S32 VerifyBuffBlk(DMX_RamPort_Info_S *PortInfo, DMX_DATA_BUF_S *pstData)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 StartBlkIdx, TotalBlkNum;
    HI_U32 index;

    StartBlkIdx = (pstData->BufPhyAddr - PortInfo->PhyAddr) / DMX_MAX_BLOCK_SIZE;
    TotalBlkNum = CalcBlkCnt(PortInfo, pstData);

    if (!PortInfo ||
        pstData->BufPhyAddr < PortInfo->PhyAddr ||
        pstData->BufLen > PortInfo->BufSize ||
        0 == pstData->BufLen ||
        StartBlkIdx + TotalBlkNum > PortInfo->TotalBufBlkNR)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    for (index = StartBlkIdx; index < StartBlkIdx + TotalBlkNum; index ++)
    {
        /* has not allocated or head blk idx is wrong */
        if (FREE_BLK_FLAG == PortInfo->pstBufBlkMap[index].BlkFlag ||
                StartBlkIdx != PortInfo->pstBufBlkMap[index].HeadBlkIdx)
        {
            ret = HI_ERR_DMX_INVALID_PARA;
            goto out;
        }
    }

    /* user can only release within user defined range */
    if (pstData->BufLen > PortInfo->pstBufBlkMap[StartBlkIdx].ReqLen)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    /* check next blk status */
    if (index < PortInfo->TotalBufBlkNR &&
            FREE_BLK_FLAG != PortInfo->pstBufBlkMap[index].BlkFlag &&
            StartBlkIdx == PortInfo->pstBufBlkMap[index].HeadBlkIdx)
    {

        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    ret = HI_SUCCESS;

out:

    return ret;
}

static HI_VOID  DMXOsiReleaseIpDes(Dmx_Set_S *DmxSet, HI_U32 RawPortId)
{
    DMX_RamPort_Info_S *PortInfo = &DmxSet->RamPortInfo[RawPortId];
    HI_U32 DescReadIdx,StartPhyAddr,DescBufSize;
    HI_U32 CurProcDescReadIdx, RelDescReadIdx, RelDescCount = 0;
    HI_U8 *DescAddr;
    HI_U32 index;

    CurProcDescReadIdx = atomic_read(&PortInfo->LastDescReadIdx);
    DescReadIdx = DmxHalIPPortDescGetRead(DmxSet, RawPortId);

    BUG_ON(DescReadIdx >= PortInfo->DescDepth || CurProcDescReadIdx >= PortInfo->DescDepth);

    DescReadIdx = (0 == DescReadIdx) ? PortInfo->DescDepth - 1 : DescReadIdx - 1;

    /* [CurProcDescReadIdx, DescReadIdx) */
    if (CurProcDescReadIdx != DescReadIdx)
    {
        if (CurProcDescReadIdx < DescReadIdx)
        {
            RelDescCount = DescReadIdx - CurProcDescReadIdx ;
        }
        else
        {
            RelDescCount = DescReadIdx + PortInfo->DescDepth - CurProcDescReadIdx ;
        }

        BUG_ON(RelDescCount > PortInfo->TotalBufBlkNR * 2); /* include self and null desc */

        for (index = CurProcDescReadIdx; index < CurProcDescReadIdx + RelDescCount; index ++)
        {
            RelDescReadIdx = (index >= PortInfo->DescDepth) ? index - PortInfo->DescDepth : index;
            DescAddr    = PortInfo->DescKerAddr + (RelDescReadIdx << 4);
            StartPhyAddr   = *(HI_U32*)DescAddr;
            DescBufSize = *(HI_U32*)(DescAddr + 4) & 0xffff;

            if (0 != StartPhyAddr &&
                (StartPhyAddr < PortInfo->PhyAddr ||
                 StartPhyAddr > PortInfo->PhyAddr + PortInfo->BufSize))
            {
                HI_ERR_DEMUX("IP DESC BUFFER ERROR,CHECK IT :%x,%x + %x!\n",StartPhyAddr, PortInfo->PhyAddr, PortInfo->BufSize);
            }
            else
            {
                /* discard null desc */
                if (DescBufSize > 0 )
                {
                    HI_U32 StartBlkIdx = (StartPhyAddr - PortInfo->PhyAddr) / DMX_MAX_BLOCK_SIZE;

                    switch(PortInfo->pstBufBlkMap[StartBlkIdx].BlkFlag)
                    {
                        case ADD_INT_FLAG:
                        {
                            PortInfo->pstBufBlkMap[StartBlkIdx].BlkRef --;
                            break;
                        }
                        case ADD_RELCAIM_INT_FLAG:
                        {
                            if (0 == --PortInfo->pstBufBlkMap[StartBlkIdx].BlkRef)
                            {
                                PortInfo->pstBufBlkMap[StartBlkIdx].BlkFlag = FREE_BLK_FLAG;
                                PortInfo->pstBufBlkMap[StartBlkIdx].HeadBlkIdx = INVALID_HEAD_BLK_IDX;
                                PortInfo->pstBufBlkMap[StartBlkIdx].ReqLen = 0;

                                atomic_inc(&PortInfo->FreeBufBlkNR);
                            }
                            break;
                        }
                        default:
                        {
                            WARN(1, "blk map idx(%d) tag is wrong(0x%x).\n", StartBlkIdx, PortInfo->pstBufBlkMap[StartBlkIdx].BlkFlag);

                            BUG();
                            break;
                        }
                    }

                    BUG_ON(atomic_read(&PortInfo->FreeBufBlkNR) > PortInfo->TotalBufBlkNR);
                }
            }
        }

        atomic_set(&PortInfo->LastDescReadIdx, DescReadIdx );
    }
}
#else
/*
1. check the avaliable space in ts buffer head part (Head) and tail part (Tail)
2. if Tail is enough for ReqLen, return Tail sapce, otherwise ,retuen Head space.
3. if both not enough ,return
PortInfo->ReqAddr   = 0;
PortInfo->ReqLen    = 0;
*/
static HI_VOID GetIPBufLen(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_U32 ReqLen)
{
    DMX_RamPort_Info_S *PortInfo = DmxSet->Ops->GetRamPortInfoByRawId(DmxSet, RawPortId);
    HI_U32              Head;/*the avaliable space in ts buffer head part*/
    HI_U32              Tail;/*the avaliable space in ts buffer tail part*/

    PortInfo->ReqAddr   = 0;
    PortInfo->ReqLen    = 0;

    if (PortInfo->Read < PortInfo->Write)
    {
        Head = PortInfo->Read;
        Tail = PortInfo->BufSize - PortInfo->Write;

        Head = (Head <= DMX_TS_BUFFER_GAP) ? 0 : (Head - DMX_TS_BUFFER_GAP);
        Tail = (Tail <= DMX_TS_BUFFER_GAP) ? 0 : (Tail - DMX_TS_BUFFER_GAP);

        if (Tail >= ReqLen)
        {
            PortInfo->ReqAddr = PortInfo->PhyAddr + PortInfo->Write;
        }
        else if (Head >= ReqLen)
        {
            PortInfo->ReqAddr = PortInfo->PhyAddr;
        }
    }
    else if (PortInfo->Read > PortInfo->Write)
    {
        Head = PortInfo->Read - PortInfo->Write;
        if (Head > DMX_TS_BUFFER_GAP)
        {
            Head -= DMX_TS_BUFFER_GAP;

            if (Head >= ReqLen)
            {
                PortInfo->ReqAddr = PortInfo->PhyAddr + PortInfo->Write;
            }
        }
    }
    else
    {
        if (0 == PortInfo->Read)
        {
            PortInfo->ReqAddr = PortInfo->PhyAddr;
        }
    }

    if (PortInfo->ReqAddr)
    {
        PortInfo->ReqLen = ReqLen;
    }
}

/**
1. check wether TS buffer have enough space for user to get (u32DataSize)
2. check wether Desc queue have enough free Desc to description the (u32DataSize) TS buffer ,
each Desc can descript 64K block of buffer.
 */
static HI_BOOL CheckIPBuf(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_U32 ReqLen)
{
    DMX_RamPort_Info_S *PortInfo = DmxSet->Ops->GetRamPortInfoByRawId(DmxSet, RawPortId);
    HI_U32              DescRead;
    HI_U32              AddDesc;
    HI_U32              FreeDesc;
    HI_SIZE_T           LockFlag;

    /*the number of Desc should add into the Desc queue */
    AddDesc  = ReqLen / DMX_MAX_BLOCK_SIZE + 2;

    DescRead = DmxHalIPPortDescGetRead(DmxSet, RawPortId);

    FreeDesc = PortInfo->DescDepth - GetQueueLenth(DescRead, PortInfo->DescWrite, PortInfo->DescDepth);
    if (AddDesc >= FreeDesc)
    {
        HI_WARN_DEMUX("no free desc. add=0x%x, free=0x%x, r=0x%x, w=0x%x, Size=0x%x\n",
            AddDesc, FreeDesc, DescRead, PortInfo->DescWrite, ReqLen);

        return HI_TRUE;
    }

    spin_lock_irqsave(&PortInfo->LockRamPort, LockFlag);
/*
call GetIPBufLen
1. check the avaliable space in ts buffer head part (Head) and tail part (Tail)
2. if Tail is enough for ReqLen, return Tail sapce, otherwise ,retuen Head space.
3. if both not enough ,return
PortInfo->ReqAddr   = 0;
PortInfo->ReqLen    = 0;
*/
    GetIPBufLen(DmxSet, RawPortId, ReqLen);
    spin_unlock_irqrestore(&PortInfo->LockRamPort, LockFlag);

    if (0 == PortInfo->ReqAddr)
    {
        HI_WARN_DEMUX("buffer not enough, r=0x%x, w=0x%x, size=0x%x\n",
            PortInfo->Read, PortInfo->Write, PortInfo->BufSize);

        return HI_TRUE;
    }

    return HI_FALSE;
}
#endif

static HI_S32 DmxResetChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_FLUSH_TYPE_E eFlushType)
{
    DMX_DmxInfo_S *DmxInfo;
    DMX_ChanInfo_S *ChanInfo;
    DMX_OQ_Info_S  *OqInfo;
    HI_S32 s32OQEnableStatus;

    ChanInfo    = &DmxSet->DmxChanInfo[ChanId];
    DmxInfo     = DmxSet->Ops->GetDmxInfo(DmxSet, ChanInfo->DmxId);
    OqInfo      = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];

    if (DMX_FLUSH_TYPE_REC & eFlushType)
    {
        DmxFlushChannel(DmxSet, ChanId, DMX_FLUSH_TYPE_REC);

        /*if only close rec channel and flush rec*/
        if (DMX_FLUSH_TYPE_REC == eFlushType)
        {
            return HI_SUCCESS;
        }
    }

    s32OQEnableStatus = DmxHalGetOQEnableStatus(DmxSet, ChanInfo->ChanOqId);
    DmxHalDisableOQRecive(DmxSet, ChanInfo->ChanOqId);
    DmxFlushChannel(DmxSet, ChanId, eFlushType);
    DmxFlushOq(DmxSet, ChanInfo->ChanOqId, DMX_OQ_CLEAR_TYPE_PLAY);

    if(s32OQEnableStatus)
    {
        DmxHalEnableOQRecive(DmxSet, ChanInfo->ChanOqId);
    }

    ResetOQ(DmxSet, DmxInfo, OqInfo);

    DMXCheckFQBPStatus(DmxSet, DmxInfo, OqInfo->u32FQId);

    //DMXCheckOQEnableStatus(pChanInfo->stChBuf.u32OQId, pChanInfo->stChBuf.OQDepth);
    return HI_SUCCESS;
}

HI_S32 DMX_OsiResetChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_FLUSH_TYPE_E eFlushType)
{
    HI_S32 ret;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = DmxResetChannel(DmxSet, ChanId, eFlushType);

    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

/**
 \brief
 \attention

 \param[in] u32Read
 \param[in] u32Write
 \param[in] IsAddrValid

 \retval none
 \return HI_SUCCESS
 \return HI_FAILURE


 \see
 \li ::
 */
static HI_S32 IsAddrValid( HI_U32 u32Read, HI_U32 u32Write, HI_U32 u32Addr)
{
    if (u32Read == u32Write)
    {
        HI_WARN_DEMUX("Read == Write,No Data, R=0x%x, W=0x%x, J=0x%x!\n", u32Read, u32Write, u32Addr);
        return HI_FAILURE;
    }

    if (u32Read < u32Write)
    {
        if ((u32Read < u32Addr) && (u32Write >= u32Addr))
        {
            return HI_SUCCESS;
        }
        else
        {
            HI_ERR_DEMUX("Addr is out of range, R=0x%x, W=0x%x, J=0x%x!\n", u32Read, u32Write, u32Addr);
            return HI_FAILURE;
        }
    }
    else
    {
        if ((u32Read < u32Addr) || (u32Write >= u32Addr))
        {
            return HI_SUCCESS;
        }
        else
        {
            HI_ERR_DEMUX("Addr is out of range, R=0x%x, W=0x%x, J=0x%x!\n", u32Read, u32Write, u32Addr);
            return HI_FAILURE;
        }
    }
}

static HI_U32 DMXOsiGetCurEopaddr(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo);
static HI_S32 DMXOsiFindPes(Dmx_Set_S *DmxSet, DMX_ChanInfo_S * pChanInfo,
                            DMX_UserMsg_S* psMsgList,
                            HI_U32         u32AcqNum,
                            HI_U32 *       pu32AcqedNum);
static HI_U32 GetPesChnFlag(Dmx_Set_S *DmxSet, DMX_ChanInfo_S * pChanInfo)
{
#if 0
    HI_U32 u32CurrentBlk,u32BufPhyAddr,u32PvrCtrl;
    HI_U8 *pu8BufVirAddr;
    HI_U32 u32WriteBlk,u32ReadBlk;
    OQ_DescInfo_S* oq_desc;
    DMX_DEV_OSI_S *pDmxDevOsi = g_pDmxDevOsi;
    HI_U32 u32OQlen,u32ValidLen = 0;
    HI_U32 u32PesLen = DMX_PES_MAX_SIZE;
    HI_S32 s32PesStartPos = -1;
    HI_U32 i;
    HI_U32 u32DropTimes = 0;

    DMXOsiOQGetReadWrite(pChanInfo->stChBuf.u32OQId, &u32WriteBlk, &u32ReadBlk);

    u32OQlen = GetQueueLenth(u32ReadBlk,u32WriteBlk,pChanInfo->stChBuf.OQDepth);
    for ( i = 0 ; i <  u32OQlen; i++ )
    {
        u32CurrentBlk = pChanInfo->stChBuf.u32OQVirAddr + u32ReadBlk * DMX_OQ_DESC_SIZE;
        oq_desc = (OQ_DescInfo_S*)u32CurrentBlk;
        u32BufPhyAddr = oq_desc->start_addr;
        u32PvrCtrl = oq_desc->pvrctrl_datalen;
        u32ValidLen++;
        if (u32PvrCtrl & DMX_MASK_BIT_18)//sop
        {
            if (s32PesStartPos > 0)
            {
                return 1;//reach 2nd sop
            }
            else if (u32ValidLen > 1)
            {
                return 1;//last pes end
            }
            s32PesStartPos = i;
            u32ValidLen = 1;
            pu8BufVirAddr = (HI_U8 *)(u32BufPhyAddr
            - pDmxDevOsi->DmxFqInfo[pChanInfo->stChBuf.u32FQId].u32BufPhyAddr
            + pDmxDevOsi->DmxFqInfo[pChanInfo->stChBuf.u32FQId].u32BufVirAddr);
            u32PesLen = ((HI_U32)pu8BufVirAddr[4] << 8) | pu8BufVirAddr[5];
            if (u32PesLen)
            {
                u32PesLen += 6;
            }
            else
            {
                u32PesLen = DMX_PES_MAX_SIZE;
            }
        }
        else if (u32PvrCtrl & DMX_MASK_BIT_19)//drop
        {
            if (s32PesStartPos > 0)//reach 2nd sop
            {
                s32PesStartPos= -1;
            }
            u32PesLen = DMX_PES_MAX_SIZE;
            u32ValidLen = 0;
            u32DropTimes++;
            if (u32DropTimes > 10)
            {
                return 1;
            }
        }
        if (u32ValidLen * DMX_FQ_COM_BLKSIZE >= u32PesLen)
        {
            return 1;
        }
        DMXINC(u32ReadBlk, pChanInfo->stChBuf.OQDepth);
    }

    return 0;
#else
    DMX_ChanInfo_S stTmpChanInfo;
    HI_U32 u32AcqedNum;
    HI_U32 u32CurDropCnt;
    DMX_UserMsg_S stMsgList[16];
    HI_S32 s32Ret;
    u32CurDropCnt = pChanInfo->u32DropCnt;
    memcpy(&stTmpChanInfo,pChanInfo,sizeof(DMX_ChanInfo_S));

    s32Ret = DMXOsiFindPes(DmxSet, &stTmpChanInfo,stMsgList,16, &u32AcqedNum);
    if (u32CurDropCnt != pChanInfo->u32DropCnt)//drop occurs
    {
        pChanInfo->u32PesBlkCnt   = 0;
        pChanInfo->u32PesLength   = 0;
        pChanInfo->u32ProcsOffset = 0;
    }

    if (HI_SUCCESS == s32Ret)
    {
        return 1;
    }
    return 0;
#endif
}

static HI_S32 __DMX_OsiGetChnDataFlag(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 OQId)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32OqId, u32OqRead, u32OqWrite, u32CurProcsBlk;
    HI_U32 u32LastEopAddr;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];
    DMX_OQ_Info_S  *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    HI_U32 u32DataLen;

    u32OqId = ChanInfo->ChanOqId;

    if (unlikely(u32OqId != OQId))
    {
        ret = HI_FAILURE;
        HI_ERR_DEMUX("The channel(%d) OQ Id changed from (%d) to (%d).\n", ChanId, OQId, u32OqId);
        goto out;
    }

    if ((HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType) || (HI_UNF_DMX_CHAN_TYPE_VID == ChanInfo->ChanType))
    {
        ret =  HI_FAILURE;   // do not receive vid and aud channel
        goto out;
    }

    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY != (ChanInfo->ChanOutMode & HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY))
    {
        ret =  HI_FAILURE;   // do not receive rec channel
        goto out;
    }

    u32CurProcsBlk = OqInfo->u32ProcsBlk;
    DMXOsiOQGetReadWrite(DmxSet, u32OqId, &u32OqWrite, &u32OqRead);

    u32LastEopAddr = DMXOsiGetCurEopaddr(DmxSet, ChanInfo);

    if (u32OqRead != u32OqWrite)
    {
        if (HI_UNF_DMX_CHAN_TYPE_PES == ChanInfo->ChanType) //pes channel just care about read and write ptr
        {
            if (GetPesChnFlag(DmxSet, ChanInfo))
            {
                ret =  HI_SUCCESS;
                goto out;
            }
            else
            {
                ret =  HI_FAILURE;
                goto out;
            }
        }

        if (u32OqWrite != u32CurProcsBlk)
        {
            DMXINC(u32CurProcsBlk, OqInfo->OQDepth);
            if (u32OqWrite == u32CurProcsBlk)
            {
                OQ_DescInfo_S* oq_dsc = OqInfo->OqDescSetBase + OqInfo->u32ProcsBlk;

                u32DataLen = oq_dsc->pvrctrl_datalen & 0xffff;

                if (OqInfo->u32ProcsOffset < u32DataLen)
                {
                    ret =  HI_SUCCESS;
                    goto out;
                }
                else if (u32LastEopAddr)
                {
                    ret =  HI_SUCCESS;
                    goto out;
                }
            }
            else
            {
                ret =  HI_SUCCESS;
                goto out;
            }
        }
    }

    if (HI_UNF_DMX_CHAN_TYPE_PES != ChanInfo->ChanType)
    {
        if ((u32LastEopAddr != OqInfo->u32ProcsOffset) && (u32LastEopAddr != 0))
        {
            ret =  HI_SUCCESS;
            goto out;
        }
    }

out:
    return ret;
}

HI_S32 DMX_OsiGetChnDataFlag(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    ret = __DMX_OsiGetChnDataFlag(DmxSet, ChanId, ChanInfo->ChanOqId);

    DmxPutChnInstance(DmxSet, ChanId);
out:
    return ret;
}

static HI_VOID DMXOsiOQGetReadWrite(Dmx_Set_S *DmxSet, HI_U32 OQId, HI_U32 *BlockWrite, HI_U32 *BlockRead)
{
    HI_U32 Value;

    DmxHalGetOQWORDx(DmxSet, OQId, DMX_OQ_RDWR_OFFSET, &Value);

    if (BlockWrite)
    {
        *BlockWrite = Value & DMX_OQ_DEPTH;
    }

    if (BlockRead)
    {
        *BlockRead = (Value >> 16) & DMX_OQ_DEPTH;
    }
}

static HI_S32 DMXOsiOQGetBbsAddrSize(Dmx_Set_S *DmxSet, HI_U32 OqId, HI_U32 OqWrite, HI_U32 *BbsAddr, HI_U32 *Size, HI_U32 *PvrCtrl)
{
    HI_S32  ret= HI_FAILURE;
    HI_U32  Value;
    HI_U32  CurrWrite;
    HI_U32  PhyAddr;
    HI_U32  len;

    *BbsAddr    = 0;
    *Size       = 0;
    *PvrCtrl    = 0;

    DmxHalGetOQWORDx(DmxSet, OqId, DMX_OQ_CTRL_OFFSET, &Value);

    Value &= 0xff;

    DmxHalGetOQWORDx(DmxSet, OqId, DMX_OQ_SADDR_OFFSET, &PhyAddr);

    DmxHalGetOQWORDx(DmxSet, OqId, DMX_OQ_EOPWR_OFFSET, &len);

    len &= 0xffff;

    DmxHalGetOQWORDx(DmxSet, OqId, DMX_OQ_RDWR_OFFSET, &CurrWrite);

    CurrWrite &= DMX_OQ_DEPTH;

    if ((OqWrite == CurrWrite) && (Value & DMX_MASK_BIT_7) && PhyAddr && len)
    {
        *BbsAddr    = PhyAddr;
        *Size       = len;
        *PvrCtrl    = Value;

        ret = HI_SUCCESS;
    }

    return ret;
}

static HI_VOID DMXOsiRelaseOQ(Dmx_Set_S *DmxSet, DMX_OQ_Info_S *pstChPlayBuf, HI_U32 u32ReadBlk, HI_U32 u32Num)
{
    HI_U32  FqId;
    HI_U32  fqwrite, oqread;
    OQ_DescInfo_S* oq_desc;
    FQ_DescInfo_S* fq_desc;
    DMX_FQ_Info_S  *FqInfo;
    HI_SIZE_T u32LockFlag;

    FqId = pstChPlayBuf->u32FQId;

    FqInfo = &DmxSet->DmxFqInfo[FqId];

    spin_lock_irqsave(&(FqInfo->LockFq), u32LockFlag);

    DmxHalGetFQReadWritePtr(DmxSet, FqId, HI_NULL, &fqwrite);

    oqread = u32ReadBlk;
    while (u32Num)
    {
        oq_desc = pstChPlayBuf->OqDescSetBase + oqread;
        fq_desc = DmxSet->DmxFqInfo[FqId].FqDescSetBase + fqwrite;
        fq_desc->start_addr = oq_desc->start_addr;
        fq_desc->buflen = oq_desc->cactrl_buflen & 0xffff;
        DMXINC(oqread, pstChPlayBuf->OQDepth);
        DMXINC(fqwrite, DmxSet->DmxFqInfo[FqId].FQDepth);
        u32Num--;
    }

    DmxHalSetOQReadPtr(DmxSet, pstChPlayBuf->u32OQId, oqread);
    pstChPlayBuf->u32ReleaseBlk = oqread;
    DmxHalSetFQWritePtr(DmxSet, FqId, fqwrite);

    spin_unlock_irqrestore(&(FqInfo->LockFq), u32LockFlag);
}

static HI_S32 DMXOsiFindPes(Dmx_Set_S *DmxSet, DMX_ChanInfo_S * pChanInfo,
                            DMX_UserMsg_S* psMsgList,
                            HI_U32         u32AcqNum,/*u32AcqNum = DMX_PES_MAX_SIZE*/
                            HI_U32 *       pu32AcqedNum)
{
    HI_U32 u32BufPhyAddr, u32PvrCtrl, u32DataLen;
    HI_U8 *pu8BufVirAddr;
    HI_U32 u32WriteBlk, u32ReadBlk, u32CurReadBlk;
    HI_U32 u32CurBlkNum, u32ProcsBlk;
    HI_U32 u32PesHead = 0;
    HI_U32 u32Offset, u32PesLen;
    HI_U32 u32DropTimes = 0;
    OQ_DescInfo_S* oq_desc;
    DMX_OQ_Info_S  *OqInfo = &DmxSet->DmxOqInfo[pChanInfo->ChanOqId];

    //always get new write
    DMXOsiOQGetReadWrite(DmxSet, pChanInfo->ChanOqId, &u32WriteBlk, &u32ReadBlk);
    /*Count of PES block alreadly be acquired by user,after finishing acquire a whole PES packet,it will be set 0*/
    u32ProcsBlk = pChanInfo->u32PesBlkCnt;

    /* only for pes, The offset of Current PES packet alreadly been acquired,
    each time after DMXOsiFindPes return ,it will recorded the added datalen .
    it will be set 0 after finishing acquire a whole PES packet*/
    u32Offset = pChanInfo->u32ProcsOffset;

    /*Current PES packet's len,The value been set when each time a new PES header come and parser it get the length.
    The value been set zeor when initialize or after inishing acquire a whole PES packet ""*/
    u32PesLen = pChanInfo->u32PesLength;
    u32CurReadBlk = u32ReadBlk;
    u32CurBlkNum  = 0;
    *pu32AcqedNum = 0;

    while (u32ReadBlk != u32WriteBlk)
    {
        oq_desc = OqInfo->OqDescSetBase + u32ReadBlk;
        u32BufPhyAddr = oq_desc->start_addr;
        u32PvrCtrl = oq_desc->pvrctrl_datalen;
        u32DataLen = oq_desc->pvrctrl_datalen & 0xffff;

        /*if current OQ descriptor have the flag of SOP (PVR CTRL bit2),it means one of the following events happens:
        event 1. a new PES packet start at this block and  we know last PES packet have alreadly finished (by (u32ProcsBlk = pChanInfo->u32PesBlkCnt) == 0)
        event 2. we do not know the last PES packet acquiring finished ((u32ProcsBlk = pChanInfo->u32PesBlkCnt) != 0) ,however a new PES packet start at this block ,
                 so ,last PES packet must finished.
                 (for PES packet with length fild = 0,it means the PES length uncertain ,we only can use 'SOP flag appearing' judge the last PES finished) */
        if (u32PvrCtrl & DMX_MASK_BIT_18)
        {
            /*event 1 happen*/
            if (u32ProcsBlk == 0)
            {
                pu8BufVirAddr = (HI_U8 *)(u32BufPhyAddr
                                          - DmxSet->DmxFqInfo[OqInfo->u32FQId].BufPhyAddr
                                          + DmxSet->DmxFqInfo[OqInfo->u32FQId].BufVirAddr);

                //according to protocol
                u32PesLen = ((HI_U32)pu8BufVirAddr[4] << 8) | pu8BufVirAddr[5];
                if (u32PesLen)
                {
                    u32PesLen += 6;
                }

                u32PesHead = 1;
                pChanInfo->u32PesLength = u32PesLen;
            }
            /*event 2 happen*/
            else
            {
                if (u32CurBlkNum)
                {
                    if (pChanInfo->u32PesBlkCnt)
                    {
                        /*pay attentaion: we use: 'psMsgList[u32CurBlkNum - 1]' instead of  'psMsgList[u32CurBlkNum]'
                        because we now change the last BB's data type . the last BB is the end of the last PES packet*/
                        psMsgList[u32CurBlkNum - 1].enDataType = HI_UNF_DMX_DATA_TYPE_TAIL;
                    }
                    else
                    {
                        psMsgList[u32CurBlkNum - 1].enDataType = HI_UNF_DMX_DATA_TYPE_WHOLE;
                    }

                    pChanInfo->u32PesLength   = 0;
                    pChanInfo->u32ProcsOffset = 0;
                    pChanInfo->u32PesBlkCnt = 0;
                    *pu32AcqedNum = u32CurBlkNum;
                    return HI_SUCCESS;
                }
                /*
                situation such as: 64K PES with len fild = 0,
                --------------------1PES-----------------------|------------------2PES-------------------------
                |sop(8k)| 8k  |  8k  | 8k |8k  |  8k  | 8k |8k | sop(8k)| 8k  |  8k  | 8k |8k  |  8k  | 8k |8k |
                when the second time meet BB with SOP ,now : u32ProcsBlk == 8, u32CurBlkNum == 0, will enter this else
                pay attentaion: in this situation ,the last time DMXOsiFindPes return ,may be the psMsgList[7].enDataType is BODY,
                acturally it should be Tail. but we have no idea how to change it before user use it.
                so ,just let it be ,you should know this may be error.
                */
                else
                {
                    u32ProcsBlk = 0;
                    pChanInfo->u32PesBlkCnt = 0;
                    pChanInfo->u32PesLength   = 0;
                    pChanInfo->u32ProcsOffset = 0;
                    continue;
                }
            }
        }

        /*if current OQ descriptor have the flag of DROP (PVR CTRL bit3),
        it means a PES packet have not finish but may be (FQ/OQ)overflow happened,
        in order to make sure the PES user got are entire, the before OQ descriptor -> buffer block should be backward,
        OQ read pointer should be backward*/
        if (u32PvrCtrl & DMX_MASK_BIT_19)
        {
            DMXOsiRelaseOQ(DmxSet, OqInfo, u32CurReadBlk, u32CurBlkNum + 1);/*release "u32CurBlkNum + 1" OQ desc*/
            DMXOsiOQGetReadWrite(DmxSet, pChanInfo->ChanOqId, &u32WriteBlk, &u32ReadBlk);
            u32CurReadBlk = u32ReadBlk;
            u32CurBlkNum = 0;
            u32ProcsBlk = 0;
            pChanInfo->u32PesBlkCnt   = 0;
            pChanInfo->u32PesLength   = 0;
            pChanInfo->u32ProcsOffset = 0;
            u32Offset = 0;
            u32DropTimes++;
            pChanInfo->u32DropCnt ++ ;
            if (u32DropTimes > 10)
            {
                return HI_FAILURE;
            }
            else
            {
                continue;
            }
        }

        /*
        1.get current psMsgList's addr and len;
        2.get current psMsgList's data type;
        3.added u32ReadBlk;
        4.store the u32ReadBlk into gloable sturucture OqInfo
        :&pDmxDevOsi->DmxOqInfo[pChanInfo->ChanOqId]
        */

        u32Offset += u32DataLen;
        psMsgList[u32CurBlkNum].u32BufStartAddr = u32BufPhyAddr;
        psMsgList[u32CurBlkNum].u32MsgLen = u32DataLen;
        if (u32PesHead)
        {
            psMsgList[u32CurBlkNum].enDataType = HI_UNF_DMX_DATA_TYPE_HEAD;
            u32PesHead = 0;
        }
        else
        {
            psMsgList[u32CurBlkNum].enDataType = HI_UNF_DMX_DATA_TYPE_BODY;
        }
        DMXINC(u32ReadBlk, OqInfo->OQDepth);
        OqInfo->u32ProcsBlk = u32ReadBlk;

        if ((u32Offset == u32PesLen) && u32PesLen)
        {
            if (u32ProcsBlk == 0)
            {
                psMsgList[u32CurBlkNum].enDataType = HI_UNF_DMX_DATA_TYPE_WHOLE;
            }
            else
            {
                psMsgList[u32CurBlkNum].enDataType = HI_UNF_DMX_DATA_TYPE_TAIL;
            }

            pChanInfo->u32PesLength   = 0;
            pChanInfo->u32ProcsOffset = 0;/*set to 0 at here, means a PES packet acquiring finished (next BB with SOP appears ,meas event1 happen,see line 1489)*/
            pChanInfo->u32PesBlkCnt = 0;
            *pu32AcqedNum = u32CurBlkNum + 1;
            return HI_SUCCESS;
        }

        u32ProcsBlk++;
        u32CurBlkNum++;

        /* if reach 64k and still not find next sop */
        if ((u32CurBlkNum == DMX_PES_MAX_SIZE) || (u32CurBlkNum == u32AcqNum))/*acturally ,u32AcqNum = DMX_PES_MAX_SIZE ,so this condition is unneed*/
        {
            break;
        }
    }
    /*1 . acturally ,u32AcqNum = DMX_PES_MAX_SIZE ,so this condition (u32CurBlkNum == u32AcqNum) is unneed
      2.  it seems can put this if into line 1609*/
    if ((DMX_PES_MAX_SIZE == u32CurBlkNum) || (u32CurBlkNum == u32AcqNum))
    {
        *pu32AcqedNum = u32CurBlkNum;
        pChanInfo->u32PesBlkCnt   += u32CurBlkNum;
        pChanInfo->u32ProcsOffset += u32Offset;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

static HI_S32 DMXOsiAddErrMSG(HI_U32 u32BufStartAddr)
{
    HI_U32 i = 0;

    for (i = 0; i < DMX_MAX_ERRLIST_NUM; i++)
    {
        if ((psErrMsg[i].u32UsedFlag == 1) && (psErrMsg[i].psMsg.u32BufStartAddr == u32BufStartAddr))
        {
            return 0;
        }
    }

    for (i = 0; i < DMX_MAX_ERRLIST_NUM; i++)
    {
        if (psErrMsg[i].u32UsedFlag == 0)
        {
            psErrMsg[i].psMsg.u32BufStartAddr = u32BufStartAddr;
            psErrMsg[i].u32UsedFlag = 1;
            return 0;
        }
    }

    return -1;
}

static HI_S32 DMXOsiRelErrMSG(HI_U32 u32BufStartAddr)
{
    HI_U32 i = 0;

    for (i = 0; i < DMX_MAX_ERRLIST_NUM; i++)
    {
        if ((psErrMsg[i].u32UsedFlag == 1) && (psErrMsg[i].psMsg.u32BufStartAddr == u32BufStartAddr))
        {
            psErrMsg[i].u32UsedFlag = 0;
            return 0;
        }
    }

    return -1;
}

static HI_S32 DMXOsiCheckErrMSG(HI_U32 u32BufStartAddr)
{
    HI_U32 i = 0;

    for (i = 0; i < DMX_MAX_ERRLIST_NUM; i++)
    {
        if ((psErrMsg[i].u32UsedFlag == 1) && (psErrMsg[i].psMsg.u32BufStartAddr == u32BufStartAddr))
        {
            return 1;
        }
    }

    return 0;
}

static HI_S32 DMXOsiParseSection(HI_U32 *pu32Parsed, HI_U32 u32AcqNum, HI_U32 *pu32Offset,
                                 HI_U32 u32BufPhyAddr, HI_U32 u32BufLen, DMX_ChanInfo_S *pChanInfo,
                                 DMX_UserMsg_S* psMsgList,DMX_OQ_Info_S  *OqInfo, Dmx_Set_S *DmxSet)
{
    HI_U32 i;
    HI_U32 u32Seclen;
    HI_U8  *BufKerAddr;

    BufKerAddr = u32BufPhyAddr
                    - DmxSet->DmxFqInfo[OqInfo->u32FQId].BufPhyAddr
                    + DmxSet->DmxFqInfo[OqInfo->u32FQId].BufVirAddr;

    if (unlikely(!(u32BufPhyAddr >= DmxSet->DmxFqInfo[OqInfo->u32FQId].BufPhyAddr &&
                                u32BufPhyAddr < DmxSet->DmxFqInfo[OqInfo->u32FQId].BufPhyAddr + DmxSet->DmxFqInfo[OqInfo->u32FQId].BufSize)))
    {
        HI_FATAL_DEMUX("invalid desc[0x%x, %d] of fq[0x%x, %d].\n", u32BufPhyAddr, u32BufLen,
                                                DmxSet->DmxFqInfo[OqInfo->u32FQId].BufPhyAddr, DmxSet->DmxFqInfo[OqInfo->u32FQId].BufSize);
        BUG();
    }


    if (*pu32Offset >= u32BufLen)
    {
        return HI_FAILURE;
    }

    for (i = *pu32Parsed; i < u32AcqNum;)
    {
        if (HI_UNF_DMX_CHAN_TYPE_POST == pChanInfo->ChanType)
        {
            u32Seclen = DMX_TS_PACKET_LEN;
            if (*(HI_U8 *)(BufKerAddr + *pu32Offset) != 0x47)
            {
                DMXOsiAddErrMSG(u32BufPhyAddr);
                pChanInfo->u32DropCnt ++ ;
                *pu32Offset += u32Seclen;
                if (*pu32Offset >= u32BufLen)
                {
                    break;
                }
                continue;
            }
        }
        else
        {
            u32Seclen = GetSectionLength(BufKerAddr, u32BufLen, *pu32Offset);
        }

        if ((0 == u32Seclen) || (u32Seclen > DMX_MAX_SEC_LEN))
        {
            DMXOsiAddErrMSG(u32BufPhyAddr);
            pChanInfo->u32DropCnt ++ ;

            HI_ERR_DEMUX("section error. ChanId=%u, Pid=0x%x, seclen:%x, phyaddr:%x, BufLen:%x, offset:%x\n",
                pChanInfo->ChanId, pChanInfo->ChanPid, u32Seclen, u32BufPhyAddr, u32BufLen, *pu32Offset);

            *pu32Offset = u32BufLen; //jump the error section

            break;
        }
        else if (u32Seclen < 7)
        {
            HI_U32 section_syntax_indicator = (*(HI_U8 *)(BufKerAddr + *pu32Offset + 1)) & 0x80;

            if (   (HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD == pChanInfo->ChanCrcMode)
                || ((HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD == pChanInfo->ChanCrcMode) &&  section_syntax_indicator) )
            {
                if (!*pu32Offset)   //add err addr, only when at the begin of the buffer
                {
                    DMXOsiAddErrMSG(u32BufPhyAddr);
                }

                pChanInfo->u32DropCnt ++ ;
                *pu32Offset += u32Seclen;
                if (*pu32Offset >= u32BufLen)
                {
                    break;
                }
                continue;
            }
        }

#ifndef DMX_FILTER_DEPTH_SUPPORT
        if (u32Seclen < DMX_FILTER_MAX_DEPTH + SECTION_LENGTH_FIELD_SIZE)
        {
            HI_U32              FilterId;
            DMX_FilterInfo_S   *FilterInfo  = DmxSet->DmxFilterInfo;
            HI_U32              ChanId      = pChanInfo->ChanId;
            HI_U8              *sect        = (HI_U8*)(BufKerAddr + *pu32Offset);
            HI_BOOL             pass        = HI_FALSE;

            for (FilterId = 0; FilterId < DMX_FILTER_CNT; FilterId++)
            {
                HI_U32  len     = 0;
                HI_U32  Depth   = FilterInfo[FilterId].Depth;
                HI_U8  *match   = FilterInfo[FilterId].Match;
                HI_U8  *mask    = FilterInfo[FilterId].Mask;
                HI_U8  *negate  = FilterInfo[FilterId].Negate;
                HI_U32  j;

                if (ChanId != FilterInfo[FilterId].ChanId)
                {
                    continue;
                }

                if (0 == Depth)
                {
                    pass = HI_TRUE;

                    break;
                }

                if (u32Seclen < Depth + SECTION_LENGTH_FIELD_SIZE)
                {
                    continue;
                }

                for (j = 0; j < Depth; j++)
                {
                    if (len == u32Seclen)
                    {
                        break;
                    }

                    if (negate[j])
                    {
                        if ((match[j] & ~mask[j]) == (sect[len] & ~mask[j]))
                        {
                            break;
                        }
                    }
                    else
                    {
                        if ((match[j] & ~mask[j]) != (sect[len] & ~mask[j]))
                        {
                            break;
                        }
                    }

                    if (0 == j)
                    {
                        len = 3;
                    }
                    else
                    {
                        ++len;
                    }
                }

                if (j == Depth)
                {
                    pass = HI_TRUE;

                    break;
                }
            }

            if (!pass)
            {
                DMXOsiAddErrMSG(u32BufPhyAddr);
                pChanInfo->u32DropCnt ++ ;
                *pu32Offset += u32Seclen;
                if (*pu32Offset >= u32BufLen)
                {
                    break;
                }
                continue;
            }
        }
#endif

        psMsgList[i].u32BufStartAddr = u32BufPhyAddr + *pu32Offset;
        psMsgList[i].u32MsgLen = u32Seclen;
        *pu32Offset += u32Seclen;
        DMXOsiRelErrMSG(u32BufPhyAddr);
        if (*pu32Offset >= u32BufLen)
        {
            i++;
            break;
        }
        i++;
    }

    if (i == *pu32Parsed)
    {
        return HI_FAILURE;
    }

    *pu32Parsed = i;
    return HI_SUCCESS;
}

static HI_VOID DmxEopWakeUpWork(struct work_struct *work)
{
    struct DMX_Eop_Int_Mark *Mark = container_of(work, struct DMX_Eop_Int_Mark, work);
    HI_SIZE_T flags;

    if (g_SelectWaitQueue && HI_TRUE == Mark->ValidFlag &&
            HI_SUCCESS == DMX_OsiGetChnDataFlag(Mark->DmxSet, Mark->ChanId))
    {
        spin_lock_irqsave(&g_WaitQueueLock, flags);

        /* double checking */
        if (g_SelectWaitQueue)
        {
            g_SelectCondition = 1;
            wake_up_interruptible(g_SelectWaitQueue);
        }

        spin_unlock_irqrestore(&g_WaitQueueLock, flags);
    }

}

#ifdef DMX_MMU_SUPPORT
#ifdef DMX_MMU_VERSION_1
static HI_VOID DmxMmuIsr(Dmx_Set_S *DmxSet)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    HI_U32 IntStatus;

    DmxHalDisMmuInt(&DmxCluster->mmu);

    IntStatus = DmxHalGetMmuIntStatus(&DmxCluster->mmu);
    if (IntStatus)
    {
        SMMU_INT_STAT SmmuIntStatus;

        SmmuIntStatus.u32 = IntStatus;

        if (SmmuIntStatus.bits.tlbmiss)
        {
            HI_FATAL_DEMUX("smmu unhandled tlb miss error.\n");
        }

        if (SmmuIntStatus.bits.ptwtrans)
        {
            HI_FATAL_DEMUX("smmu unhandled ptw trans error.\n");
        }

        if (SmmuIntStatus.bits.tlbinvalid)
        {
                HI_U32 ReadErrAddr = 0, WriteErrAddr = 0;

                DmxHalGetMmuErrAddr(&DmxCluster->mmu, &ReadErrAddr, &WriteErrAddr);

                HI_FATAL_DEMUX("SMMU_WRITE_ERR MODULE : DMX(smmu tlb invalid: readAddr:0x%x/writeAddr:0x%x error).\n",
                                   ReadErrAddr, WriteErrAddr);
        }

        DmxHalClrMmuIntStatus(&DmxCluster->mmu);
    }

    DmxHalEnMmuInt(&DmxCluster->mmu);
}
#elif defined(DMX_MMU_VERSION_2)
static HI_VOID DmxCheckExceptionCause(Dmx_Set_S *DmxSet)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    HI_U32 DmxId;

    for(DmxId = 0; DmxId < DmxSet->DmxCnt; DmxId ++)
    {
        DMX_DmxInfo_S *DmxInfo = &DmxSet->DmxInfo[DmxId];
        DMX_PORT_MODE_E PortMode = DmxInfo->PortMode;
        HI_U32 PortId = DmxInfo->PortId;

        if (DMX_PORT_MODE_RAM == PortMode && PortId < DmxCluster->Ops->GetRamPortNum())
        {
            DMX_RamPort_Info_S *PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);

            if (PortInfo && PortInfo->PhyAddr && HI_UNF_DMX_SECURE_MODE_TEE == PortInfo->SecureMode)
            {
                HI_U32 ChanId;

                for(ChanId = 0; ChanId < DmxSet->DmxChanCnt; ChanId ++ )
                {
                    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

                    if (ChanInfo->DmxId == DmxId && HI_UNF_DMX_SECURE_MODE_NONE == ChanInfo->ChanSecure)
                    {
                        HI_ERR_DEMUX("demux can't dispatch data from secure ramport(%d) to nonsecure pid channel(%d).\n",
                                    PortId + HI_UNF_DMX_PORT_RAM_0, ChanId);
                    }
                }
            }
        }
    }
}

static HI_VOID DmxMmuIsr(Dmx_Set_S *DmxSet)
{
    HI_U32 IntStatus;

    DmxHalDisMmuInt(&DmxSet->mmu);

    IntStatus = DmxHalGetMmuIntStatus(&DmxSet->mmu);
    if (IntStatus)
    {
        MMU_STA_INT MmuIntStatus;

        MmuIntStatus.u32 = IntStatus;

        if (MmuIntStatus.bits.sta_mmu_tab_err)
        {
            MMU_ERR_INFO MmuErrInfo;
            HI_U32 MmuErrVirAddr, MmuErrPhyAddr;

            DmxHalGetMmuErrInfo(&DmxSet->mmu, &MmuErrInfo.u32);
            DmxHalGetMmuErrAddr(&DmxSet->mmu, &MmuErrVirAddr, &MmuErrPhyAddr);

            HI_FATAL_DEMUX("mmu unhandled page fault error(va:0x%x, pa:0x%x, vld:%d, sec:%d, rw:%d).\n", MmuErrVirAddr, MmuErrPhyAddr,
                                MmuErrInfo.bits.dbg_mmu_err_mmu_vld, MmuErrInfo.bits.dbg_mmu_err_mmu_sec, MmuErrInfo.bits.dbg_mmu_err_mmu_wr);

            if (MmuErrInfo.bits.dbg_mmu_err_mmu_wr)
            {
                HI_FATAL_DEMUX("SMMU_WRITE_ERR MODULE : DMX(VirAddr:0x%x, PhyAddr:0x%x, Rw:%d).\n",
                                   MmuErrVirAddr, MmuErrPhyAddr, MmuErrInfo.bits.dbg_mmu_err_mmu_wr);
            }
        }

        if (MmuIntStatus.bits.sta_bus_rep_err)
        {
            HI_FATAL_DEMUX("mmu unhandled bus fault error.\n");
        }

        DmxHalClrMmuIntStatus(&DmxSet->mmu);

        DmxCheckExceptionCause(DmxSet);
    }

    DmxHalEnMmuInt(&DmxSet->mmu);
}
#endif
#else
static HI_VOID DmxMmuIsr(Dmx_Set_S *DmxSet){}
#endif

static HI_S32 DMXOsiIsr(int irq, void* priv)
{
    Dmx_Set_S *DmxSet = (Dmx_Set_S*)priv;
    DMX_ChanInfo_S *ChanInfo;
    DMX_OQ_Info_S  *OqInfo;
    HI_U32          DmxId;
    HI_U32          OqId;
    HI_U32          ChanId = 0;
    HI_U32           RawPortId;
    HI_U32          IntStatus;
    HI_U32          i;
    HI_U32          j;
    unsigned long flags;

    DmxHalDisableAllPVRInt(DmxSet);

    if (DmxHalGetTotalTeiIntStatus(DmxSet))
    {
        DmxHalGetTeiIntInfo(DmxSet, &DmxId, &ChanId);
        DmxHalClrTeiIntStatus(DmxSet);

        ChanInfo = &DmxSet->DmxChanInfo[ChanId];
        ChanInfo->u32TEIErr ++;
    }

    if (DmxHalGetTotalPcrIntStatus(DmxSet))
    {
        IntStatus = DmxHalGetPcrIntStatus(DmxSet);

        for (i = 0; i < DmxSet->DmxPcrCnt; i++)
        {
            DMX_PCR_Info_S *PcrInfo;

            if (0 == (IntStatus & (1 << i)))
            {
                continue;
            }

            PcrInfo = &DmxSet->DmxPcrInfo[i];
            if (PcrInfo->DmxId < GetDmxCluster()->Ops->GetDmxNum())
            {
                DmxHalGetPcrValue(DmxSet, i, &PcrInfo->PcrValue);
                DmxHalGetScrValue(DmxSet, i, &PcrInfo->ScrValue);

                if ((DMX_INVALID_SYNC_HANDLE != PcrInfo->SyncHandle) && g_pSyncFunc && g_pSyncFunc->pfnSYNC_PcrProc)
                {
                    HI_U32 Pcr1Khz = 0;

                    /* dmx pcr is sample with the freq of 90khz, but the sync module need the freq of 1khz
                     * so divide 90 here
                     */
                    Pcr1Khz = (HI_U32)((PcrInfo->PcrValue >> 1) & 0xffffffff);  // divide 2
                    Pcr1Khz /= 45;

                    (g_pSyncFunc->pfnSYNC_PcrProc)(PcrInfo->SyncHandle, Pcr1Khz);
                }
            }

            DmxHalClrPcrIntStatus(DmxSet, i);
        }
    }

    if (DmxHalGetTotalDiscIntStatus(DmxSet))
    {
        for (i = 0; i < DMX_CHANNEL_REGION_NUM; i++)
        {
            IntStatus = DmxHalGetDiscIntStatus(DmxSet, i);
            if (IntStatus)
            {
                for (j = 0; j < DMX_CHANNEL_NUM_PER_REGION; j++)
                {
                    if (IntStatus & (1 << j))
                    {
                        ChanId = i * DMX_CHANNEL_NUM_PER_REGION + j;

                        DmxHalClearDiscIntStatus(DmxSet, ChanId);

                        ChanInfo = &DmxSet->DmxChanInfo[ChanId];
                        ChanInfo->u32CCDiscErr ++;
                    }
                }
            }
        }
    }

    if (DmxHalGetTotalCrcIntStatus(DmxSet))
    {
        for (i = 0; i < DMX_CHANNEL_REGION_NUM; i++)
        {
            IntStatus = DmxHalGetCrcIntStatus(DmxSet, i);
            if (IntStatus)
            {
                for (j = 0; j < DMX_CHANNEL_NUM_PER_REGION; j++)
                {
                    if (IntStatus & (1 << j))
                    {
                        ChanId = i * DMX_CHANNEL_NUM_PER_REGION + j;

                        DmxHalClearCrcIntStatus(DmxSet, ChanId);

                        ChanInfo = &DmxSet->DmxChanInfo[ChanId];
                        ChanInfo->u32CRCErr ++;
                    }
                }
            }
        }
    }

    if (DmxHalGetTotalPenLenIntStatus(DmxSet))
    {
        for (i = 0; i < DMX_CHANNEL_REGION_NUM; i++)
        {
            IntStatus = DmxHalGetPesLenIntStatus(DmxSet, i);
            if (IntStatus)
            {
                for (j = 0; j < DMX_CHANNEL_NUM_PER_REGION; j++)
                {
                    if (IntStatus & (1 << j))
                    {
                        ChanId = i * DMX_CHANNEL_NUM_PER_REGION + j;

                        DmxHalClearPesLenIntStatus(DmxSet, ChanId);

                        ChanInfo = &DmxSet->DmxChanInfo[ChanId];
                        ChanInfo->u32PesLenErr ++;
                    }
                }
            }
        }
    }

    IntStatus = DmxHalFQGetAllOverflowIntStatus(DmxSet);
    if (IntStatus)
    {
        HI_U32 FqId;

        for (i = 0; i < DMX_FQ_REGION_NUM; i++)
        {
            FqId = i * DMX_FQ_NUM_PER_REGION;

            if (IntStatus & (1 << i))
            {
                HI_U32 FqIntStatus;
                HI_U32 FqIntType;

                FqIntStatus = DmxHalFQGetOverflowIntStatus(DmxSet, i);
                FqIntType = DmxHalFQGetOverflowIntType(DmxSet, i);

                for (j = 0; (j < DMX_FQ_NUM_PER_REGION) && FqIntStatus && (FqId < DMX_FQ_CNT); j++, FqId++)
                {
                    if (0 == (FqIntStatus & (1 << j)))
                    {
                        continue;
                    }

                    FqIntStatus &= ~(1 << j);

                    /*  1:overflow, 0:almost overflow. only record overflow int, usually we not care about "almost overflow". */
                    if ( FqIntType & (1 << j) )
                    {
                        DmxSet->DmxFqInfo[FqId].FqOverflowCount++;
                    }

                    DmxHalFQSetOverflowInt(DmxSet, FqId, DMX_DISABLE);

                    DmxHalFQClearOverflowInt(DmxSet, FqId);
                }
            }
        }
    }

    IntStatus = DmxHalOQGetAllOverflowIntStatus(DmxSet);
    if (IntStatus)
    {
        for (i = 0; i < DMX_OQ_REGION_NUM; i++)
        {
            if (IntStatus & (1 << i))
            {
                for (j = 0; j < DMX_OQ_NUM_PER_REGION; j++)
                {
                    OqId = i * DMX_OQ_NUM_PER_REGION + j;

                    if (DmxHalOQGetOverflowIntStatus(DmxSet, OqId))
                    {
                        DmxHalOQClearOverflowInt(DmxSet, OqId);
                    }
                }
            }
        }
    }

    IntStatus = DmxHalOQGetAllEopIntStatus(DmxSet);
    if (IntStatus)
    {
        for (i = 0; i < DMX_OQ_REGION_NUM; i++)
        {
            if (0 == (IntStatus & (1 << i)))
            {
                continue;
            }

            for (j = 0; j < DMX_OQ_NUM_PER_REGION; j++)
            {
                OqId = i * DMX_OQ_NUM_PER_REGION + j;

                if (!DmxHalGetOQEopIntStatus(DmxSet, OqId))
                {
                    continue;
                }

                DmxHalClearOQEopIntStatus(DmxSet, OqId);

                OqInfo = &DmxSet->DmxOqInfo[OqId];

                OqInfo->OqWaitCondition = 1;
                wake_up_interruptible(&OqInfo->OqWaitQueue);

                spin_lock_irqsave(&g_WaitQueueLock, flags);

                /* special for select */
                if (g_SelectWaitQueue)
                {
                    HI_U32 index = 0;

                    for (;index < sizeof(g_EnEopIntMark) /sizeof(g_EnEopIntMark[0]); index ++)
                    {
                        if (HI_TRUE == g_EnEopIntMark[index].ValidFlag && DmxSet == g_EnEopIntMark[index].DmxSet &&
                                    OqId == g_EnEopIntMark[index].OQId)
                        {
                            queue_work(g_SelectWorkQueue, &g_EnEopIntMark[index].work);
                        }
                    }
                }

                spin_unlock_irqrestore(&g_WaitQueueLock, flags);
            }
        }
    }

    IntStatus = DmxHalOQGetAllOutputIntStatus(DmxSet);

    for (i = 0; (i < DMX_OQ_REGION_NUM) && IntStatus; i++)
    {
        HI_U32 OqOutInt;

        if (0 == (IntStatus & (1 << i)))
        {
            continue;
        }

        IntStatus &= ~(1 << i);

        OqOutInt = DmxHalOQGetOutputIntStatus(DmxSet, i);

        for (j = 0; (j < DMX_OQ_NUM_PER_REGION) && OqOutInt; j++)
        {
            DMX_OQ_Info_S *OqInfo;

            if (0 == (OqOutInt & (1 << j)))
            {
                continue;
            }

            OqOutInt &= ~(1 << j);

            OqId = i * DMX_OQ_NUM_PER_REGION + j;

            DmxHalOQEnableOutputInt(DmxSet, OqId, HI_FALSE);

            OqInfo = &DmxSet->DmxOqInfo[OqId];
            if (OqInfo)
            {
                OqInfo->OqWakeUp = HI_TRUE;

                wake_up_interruptible(&OqInfo->OqWaitQueue);
            }
        }
    }

    /* this port id is raw id, so it can access port info directly */
    for (RawPortId = 0; RawPortId < DmxSet->RamPortCnt; RawPortId++)
    {
        if (DmxHalIPPortGetOutIntStatus(DmxSet, RawPortId))
        {
#ifdef DMX_TS_BUF_VERSION_2
            DMX_RamPort_Info_S *PortInfo = HI_NULL;

            DmxHalIPPortClearOutIntStatus(DmxSet, RawPortId);

            PortInfo = DmxSet->Ops->GetRamPortInfoByRawId(DmxSet, RawPortId);

            DMXOsiReleaseIpDes(DmxSet, RawPortId);

            if (PortInfo->WaitLen)
            {
                if (HI_SUCCESS == PingIPBuf(DmxSet, RawPortId, PortInfo->WaitLen))
                {
                    PortInfo->WakeUp    = HI_TRUE;
                    PortInfo->WaitLen   = 0;

                    wake_up_interruptible(&PortInfo->WaitQueue);
                }
            }
 #else
            DMX_RamPort_Info_S *PortInfo = HI_NULL;
            HI_SIZE_T           LockFlag;

            DmxHalIPPortClearOutIntStatus(DmxSet, RawPortId);

            PortInfo = DmxSet->Ops->GetRamPortInfoByRawId(DmxSet, RawPortId);

            spin_lock_irqsave(&PortInfo->LockRamPort, LockFlag);
            if (0 != PortInfo->Write)
            {
                HI_U32  DescRead;
                HI_U32 *ReadAddr;
                /*DmxHalIPPortDescGetRead return the Desc logic is reading */
                DescRead = DmxHalIPPortDescGetRead(DmxSet, RawPortId);
                if (0 == DescRead)
                {
                    DescRead = PortInfo->DescDepth - 1;
                }
                else
                {
                    --DescRead;/*it means the Desc which logic alreadly finished read it*/
                }

                ReadAddr = (HI_U32*)(PortInfo->DescKerAddr + (DescRead << 4));
                /*PortInfo->Read is the ts buffer read offset*/
                PortInfo->Read = *ReadAddr - PortInfo->PhyAddr;
                if (PortInfo->Read == PortInfo->Write)
                {
                    PortInfo->Read  = 0;
                    PortInfo->Write = 0;
                }
            }
            spin_unlock_irqrestore(&PortInfo->LockRamPort, LockFlag);

            /*
            in DMX_OsiTsBufferGet,
            if have not enough space ,the PortInfo->WaitLen will be give value ,and wait the interupt of
            DmxHalIPPortGetOutIntStatus (new Desc be readed by logic,so there may be new space avaliable),
            every time in DmxHalIPPortGetOutIntStatus ,will call GetIpFreeDescAndBufLen again , if return HI_FALSE,
            will wake up
            */
            if (PortInfo->WaitLen)
            {
                if (!CheckIPBuf(DmxSet, RawPortId, PortInfo->WaitLen))
                {
                    PortInfo->WakeUp    = HI_TRUE;
                    PortInfo->WaitLen   = 0;

                    wake_up_interruptible(&PortInfo->WaitQueue);
                }
            }
#endif
        }
    }

    /* bus interal buffer overflow  intterrupt status of play channel */
    if (DmxHalGetD2BFOvflIntStatus(DmxSet))
    {
        DmxHalClsD2BFOvflIntStatus(DmxSet);
        HI_PRINT("bus interal buffer overflow in play channel!\n");
    }

    /* bus interal buffer overflow  intterrupt status of record channel*/
    if (DmxHalGetD2BROvflIntStatus(DmxSet))
    {
        DmxHalClsD2BROvflIntStatus(DmxSet);
        HI_PRINT("bus interal buffer overflow in record channel!\n");
    }

    DmxMmuIsr(DmxSet);

    DmxHalEnableAllPVRInt(DmxSet);

    return IRQ_HANDLED;
}

static inline HI_S32 DmxChannelWaitTimeOut(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo, HI_U32 u32TimeOutMs)
{
    HI_S32 ret;
    DMX_OQ_Info_S  *OqInfo  = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];

    OqInfo->OqWaitCondition = 0;
    DmxHalEnableOQEopInt(DmxSet, ChanInfo->ChanOqId);
    ret = wait_event_interruptible_timeout(OqInfo->OqWaitQueue,
                                           OqInfo->OqWaitCondition,
                                           msecs_to_jiffies(u32TimeOutMs));
    OqInfo->OqWaitCondition = 0;
    DmxHalDisableOQEopInt(DmxSet, ChanInfo->ChanOqId);
    return ret;
}

/**
 \brief

 \attention

 \param[in] pChanInfo
 \param[in] u32AcqNum
 \param[out] pu32AcqedNum
 \param[out] psMsgList
 \param[in] u32TimeOutMs

 \retval none
 \return none

 \see
 \li ::
 */
static HI_S32 DMXOsiReadPes(Dmx_Set_S *DmxSet, DMX_ChanInfo_S * pChanInfo,
                            HI_U32         u32AcqNum, /*u32AcqNum = DMX_PES_MAX_SIZE*/
                            HI_U32 *       pu32AcqedNum,
                            DMX_UserMsg_S* psMsgList,
                            HI_U32         u32TimeOutMs)
{
    HI_S32 ret;

    ret = DMXOsiFindPes(DmxSet, pChanInfo, psMsgList, u32AcqNum, pu32AcqedNum);
    if (ret == HI_SUCCESS)
    {
        pChanInfo->u32HitAcq++;
        return ret;
    }

    *pu32AcqedNum = 0;
    return HI_ERR_DMX_NOAVAILABLE_DATA;
}

static HI_S32 DMXOsiGetSecNum(Dmx_Set_S *DmxSet, DMX_ChanInfo_S * pChanInfo,
                              HI_U32*        pu32BlkNum,
                              HI_U32         u32AcqNum,
                              DMX_UserMsg_S* psMsgList,
                              DMX_OQ_Info_S* OqInfo)
{
    HI_U32 u32BufPhyAddr, u32DataLen;
    HI_U32 u32Offset;
    HI_U32 u32WriteBlk, u32ReadBlk, u32ProcsBlk;
    HI_S32 s32Ret;

    /*get current OQ Desc write pointer in register*/
    DMXOsiOQGetReadWrite(DmxSet, pChanInfo->ChanOqId, &u32WriteBlk, HI_NULL);

    u32Offset   = OqInfo->u32ProcsOffset;
    /*we not directly using the read pointer in register but use the backup in software because the read pointer in register
    will only updated after the DMX_OsiReleaseReadData be called in order to avoid these buffer be writen by hardware*/
    u32ReadBlk  = OqInfo->u32ProcsBlk;/*the current OQ Desc now be processing (software is reading the data of the correlative buffer block)*/
    u32ProcsBlk = u32ReadBlk;
    *pu32BlkNum = 0;
    /*process block one by one ,until the u32ReadBlk == u32WriteBlk*/
    while (u32ReadBlk != u32WriteBlk)
    {
        OQ_DescInfo_S  *oq_dsc = OqInfo->OqDescSetBase + u32ReadBlk;

        u32BufPhyAddr = oq_dsc->start_addr;
        u32DataLen = oq_dsc->pvrctrl_datalen & 0xffff;

        /*if finished read the data of the current read  OQ Desc -> Buffer block  */
        if (u32Offset >= u32DataLen)
        {
            DMXINC(u32ReadBlk, OqInfo->OQDepth);
            DMXINC(u32ProcsBlk, OqInfo->OQDepth);
            OqInfo->u32ProcsBlk = u32ProcsBlk;
            OqInfo->u32ProcsOffset = 0;
            u32Offset = 0;
            continue;
        }

        s32Ret = DMXOsiParseSection(pu32BlkNum, u32AcqNum, &u32Offset, u32BufPhyAddr, u32DataLen, pChanInfo, psMsgList, OqInfo, DmxSet);
        if (s32Ret != HI_SUCCESS)
        {
            continue;
        }
        else
        {
            if (u32Offset >= u32DataLen)
            {
                DMXINC(u32ProcsBlk, OqInfo->OQDepth);
                u32Offset = 0;
            }

            if (*pu32BlkNum >= u32AcqNum)
            {
                break;
            }
        }

        DMXINC(u32ReadBlk, OqInfo->OQDepth);
    }

    if (*pu32BlkNum > 0)
    {
        OqInfo->u32ProcsBlk = u32ProcsBlk;
        OqInfo->u32ProcsOffset = u32Offset;
    }

    return HI_SUCCESS;
}

static HI_U32 DMXOsiGetCurEopaddr(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo)
{
    HI_U32 EopAddr = 0;
    HI_U32 PvrCtrl;

    DmxHalGetOQWORDx(DmxSet, ChanInfo->ChanOqId, DMX_OQ_CTRL_OFFSET, &PvrCtrl);

    if (PvrCtrl & DMX_MASK_BIT_7)
    {
        HI_U32 Value;

        DmxHalGetOQWORDx(DmxSet, ChanInfo->ChanOqId, DMX_OQ_EOPWR_OFFSET, &Value);

        if (HI_UNF_DMX_CHAN_TYPE_POST != ChanInfo->ChanType)
        {
            EopAddr = Value >> DMX_SHIFT_16BIT;
        }
        else
        {
            EopAddr = Value & 0xffff;
        }
    }

    return EopAddr;
}

static HI_S32 DMXOsiGetCurSAddr(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *pChanInfo, HI_U32* pu32BufPhyAddr, HI_U32* pu32EopAddr,DMX_OQ_Info_S  *OqInfo)
{
    HI_U32 u32ReadBlk, u32WriteBlk;
    HI_U32 u32OQId;

    u32OQId = pChanInfo->ChanOqId;
    u32ReadBlk   = OqInfo->u32ProcsBlk;
    *pu32EopAddr = DMXOsiGetCurEopaddr(DmxSet, pChanInfo);
    DmxHalGetOQWORDx(DmxSet, u32OQId, DMX_OQ_SADDR_OFFSET, pu32BufPhyAddr);
    DMXOsiOQGetReadWrite(DmxSet, u32OQId, &u32WriteBlk, HI_NULL);

    if (u32WriteBlk == u32ReadBlk)
    {
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

static HI_S32 DmxOsiReadToMsgBuffer(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *pChanInfo, HI_U32 u32AcqNum,
                                    HI_U32 *pu32AcqedNum, DMX_UserMsg_S* psMsgList,DMX_OQ_Info_S* OqInfo)
{
    HI_U32 u32BlkNum = 0;
    HI_S32 s32Ret;

    s32Ret = DMXOsiGetSecNum(DmxSet, pChanInfo, &u32BlkNum, u32AcqNum, psMsgList,OqInfo);
    if ((HI_SUCCESS == s32Ret) && (u32BlkNum > 0))
    {
        *pu32AcqedNum = u32BlkNum;
        return HI_SUCCESS;
    }

    *pu32AcqedNum = 0;

    return HI_ERR_DMX_NOAVAILABLE_DATA;
}

static HI_S32 DMXOsiReadSec(Dmx_Set_S *DmxSet,
                            DMX_ChanInfo_S * pChanInfo,
                            HI_U32         u32AcqNum,
                            HI_U32 *       pu32AcqedNum,
                            DMX_UserMsg_S* psMsgList,
                            DMX_OQ_Info_S* OqInfo,
                            HI_U32         u32TimeOutMs)
{
    HI_S32 ret;
    HI_U32 u32BufPhyAddr;
    HI_U32* pu32Offset;
    HI_U32 u32EopAddr;
    HI_U32 u32BlkNum = 0;

    u32EopAddr = DMXOsiGetCurEopaddr(DmxSet, pChanInfo);

    /* "DmxOsiReadToMsgBuffer" is used to read at least one whole buffer block,
    if there is no a whole block data, the pu32AcqedNum will == 0 */
    ret = DmxOsiReadToMsgBuffer(DmxSet, pChanInfo, u32AcqNum, pu32AcqedNum, psMsgList,OqInfo);
    if ((ret == HI_SUCCESS) && (*pu32AcqedNum > 0))
    {
        pChanInfo->u32HitAcq++;
        return HI_SUCCESS;
    }

    /* if there is no whole buffer block of data ,judge whether the current block (read==write)
    have some data or not ,if not ,wait untile time out */
    pu32Offset = &OqInfo->u32ProcsOffset;
    if (*pu32Offset == u32EopAddr)
    {
        return HI_ERR_DMX_NOAVAILABLE_DATA;
    }

    /* get current block's u32BufPhyAddr ,the following function DMXOsiParseSection will use it */
    if (DMXOsiGetCurSAddr(DmxSet, pChanInfo, &u32BufPhyAddr, &u32EopAddr,OqInfo) != HI_SUCCESS)
    {
        /*most possiblily, the DMXOsiGetCurBufblockSAddr return failure because of the "u32WriteBlk != u32ReadBlk" ,
        so we call DmxOsiReadToMsgBuffer to read whole buffer again*/
        ret = DmxOsiReadToMsgBuffer(DmxSet, pChanInfo, u32AcqNum, pu32AcqedNum, psMsgList,OqInfo);
        if (ret == HI_SUCCESS)
        {
            pChanInfo->u32HitAcq++;

            return HI_SUCCESS;
        }

        //read eop again,if not success return
        if (DMXOsiGetCurSAddr(DmxSet, pChanInfo, &u32BufPhyAddr, &u32EopAddr,OqInfo) != HI_SUCCESS)
        {
            HI_WARN_DEMUX("OQ %d no new eop with eop interrupt,rec addr:%x ,eop addr :%x!\n",
                          pChanInfo->ChanOqId, *pu32Offset, u32EopAddr);

            return HI_ERR_DMX_NOAVAILABLE_DATA;
        }
    }
    else /* read == write */
    {
        DMXOsiParseSection(&u32BlkNum, u32AcqNum, pu32Offset, u32BufPhyAddr, u32EopAddr, pChanInfo, psMsgList, OqInfo, DmxSet);
    }

    if (0 == u32BlkNum)
    {
        return HI_ERR_DMX_NOAVAILABLE_DATA;
    }

    pChanInfo->u32HitAcq++;

    *pu32AcqedNum = u32BlkNum;

    return HI_SUCCESS;
}

HI_U32 FilterGetValidDepth(const HI_UNF_DMX_FILTER_ATTR_S *FilterAttr)
{
    HI_U32 Depth = FilterAttr->u32FilterDepth;

    while (Depth)
    {
        if (0xff == FilterAttr->au8Mask[Depth - 1])
        {
            --Depth;
        }
        else
        {
            break;
        }
    }

    return Depth;
}

/**
\brief get channel buffer status
\attention
none
\param[in] u32ChanId

\retval none
\return 0  channel can't (or no need to)  reset
        1  channel buffer is < 20% of pool buffer,and it's buffer is > 1
        2  channel buffer is > 20% of pool buffer
\see
\li ::
*/
static HI_S32 GetChnDataStatus(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    DMX_ChanInfo_S *ChanInfo = HI_NULL;
    DMX_OQ_Info_S  *OqInfo = HI_NULL;

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    /* only care section/pes/post channel. */
    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode)
    {
        HI_U32 u32OqRead, u32OqWrite;
        HI_U32 FQDepth, u32ValDes;

        OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];

        if (DMX_FQ_COMMOM == OqInfo->u32FQId)
        {
            DMXOsiOQGetReadWrite(DmxSet, ChanInfo->ChanOqId, &u32OqWrite, &u32OqRead);

            if (OqInfo->u32ProcsBlk != u32OqRead)//there's buffer not released
            {
                return 0;
            }

            FQDepth = DmxSet->DmxFqInfo[OqInfo->u32FQId].FQDepth;
            u32ValDes = GetQueueLenth(u32OqRead, u32OqWrite, OqInfo->OQDepth);
            if(u32ValDes > (FQDepth*DMX_CHN_RESET_PERSENT/100))// > 20% of pool buffer
            {
                return 2;
            }

            if (u32ValDes > 1)//have buffer more than 1 block
            {
                return 1;
            }

            HI_DRV_SYS_GetTimeStampMs(&ChanInfo->u32AcqTime);   //if channel buffer is < 1,flush the acquire time
        }
    }

    return 0;
}

/**
\brief get channel acquire time status
\attention
none
\param[in] u32ChanId

\retval none
\return 0: do not care
        1: last acquire time - now > 2s
        2: last acquire time - now > 5s

\see
\li ::
*/
static HI_S32 GetChnAcqTimeStatus(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    DMX_ChanInfo_S *ChanInfo    = &DmxSet->DmxChanInfo[ChanId];
    DMX_OQ_Info_S  *OqInfo      = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    HI_U32 u32AcqTime,u32TimeNow;

    if (DMX_FQ_COMMOM != OqInfo->u32FQId)
    {
        return 0; //not usded channel,and not common channel,do not process
    }

    HI_DRV_SYS_GetTimeStampMs(&u32TimeNow);

    u32TimeNow += 10;   //add 10ms, void situation 1999ms < 2000

    u32AcqTime = ChanInfo->u32AcqTime;
    if ((u32TimeNow - u32AcqTime) > DMX_RESETCHN_TIME2)// > 5s
    {
        return 2;
    }

    if ((u32TimeNow - u32AcqTime) > DMX_RESETCHN_TIME1)// > 2s
    {
        return 1;
    }
    return 0;
}
/**
\brief get fq status
\attention
none
\param[in] u32FQId

\retval none
\return 0:do not care
        1: fq used des > 80% of fq buffer size

\see
\li ::
*/
static HI_S32 GetFQStatus(Dmx_Set_S *DmxSet, HI_U32 u32FQId)
{
    HI_U32 FQ_WPtr, FQ_RPtr, ValidDes, UsedDes;

    DmxHalGetFQReadWritePtr(DmxSet, u32FQId, &FQ_RPtr, &FQ_WPtr);

    if (FQ_WPtr >= FQ_RPtr)
    {
        ValidDes = FQ_WPtr - FQ_RPtr;
    }
    else
    {
        ValidDes = (DmxSet->DmxFqInfo[u32FQId].FQDepth) - FQ_RPtr + FQ_WPtr;
    }
    UsedDes = DmxSet->DmxFqInfo[u32FQId].FQDepth - ValidDes;

    if (UsedDes >= (DmxSet->DmxFqInfo[u32FQId].FQDepth)* DMX_CHECK_START_PERSENT/ 100)
    {
       return 1;
    }

    return 0;
}

/*
 * common fq(0) shared by all section channel, if one channel take most of fq space and no release them.
 * other channel maybe lock. so we have one monitor check it and manual reset channel.
 */
static HI_VOID CommFQMonitor(Dmx_Set_S *DmxSet)
{
    HI_S32 ret;

    if (GetFQStatus(DmxSet, DMX_FQ_COMMOM))//dmx pool buffer used > 80%
    {
        HI_U32 ChanId;

        mutex_lock(&DmxSet->LockAllChn);

        for_each_set_bit(ChanId, DmxSet->ChnBitmap, DmxSet->DmxChanCnt)
        {
            if (HI_SUCCESS == DmxGetChnInstance(DmxSet, ChanId))
            {
                DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

                ret = GetChnDataStatus(DmxSet, ChanId);
                if (ret == 2)//channel buffer > 20% of dmx pool buffer
                {
                    if (GetChnAcqTimeStatus(DmxSet, ChanId) >= 1)
                    {
                        DmxResetChannel(DmxSet, ChanId, DMX_FLUSH_TYPE_REC_PLAY);//reset channel
                        HI_DRV_SYS_GetTimeStampMs(&ChanInfo->u32AcqTime);

                        DmxSet->DmxFqInfo[DMX_FQ_COMMOM].FqOverflowCount++;
                    }
                }
                else if (ret == 1)//channel buffer < 20% of dmx pool buffer,> 1 blk
                {
                    if (GetChnAcqTimeStatus(DmxSet, ChanId) == 2)// > 5s not receive data
                    {
                        DmxResetChannel(DmxSet, ChanId, DMX_FLUSH_TYPE_REC_PLAY);//reset channel
                        HI_DRV_SYS_GetTimeStampMs(&ChanInfo->u32AcqTime);

                        DmxSet->DmxFqInfo[DMX_FQ_COMMOM].FqOverflowCount++;
                    }
                }

                DmxPutChnInstance(DmxSet, ChanId);
            }
        }

        mutex_unlock(&DmxSet->LockAllChn);
    }
}

#ifdef DMX_SCD_VERSION_2
/*
 * check whether shared rec pid ts packages had dropped.
 */
static HI_VOID SharedPidMonitor(Dmx_Set_S *DmxSet)
{
    HI_U32 AllRecDropFlag, RecId;

    AllRecDropFlag = DmxHalGetDavDropTsFlag(DmxSet);
    for_each_set_bit(RecId, (long unsigned int *)&AllRecDropFlag, sizeof(AllRecDropFlag))
    {
        HI_ERR_DEMUX("Rec instance(%d) had dropped shared ts packages.\n", RecId);

         /* clear drop flags */
        DmxHalClrDavDropTsFlag(DmxSet, RecId);
    }
}
#else
static inline HI_VOID SharedPidMonitor(Dmx_Set_S *DmxSet) {}
#endif

static HI_VOID DmxSwitchOverflowMonitor(Dmx_Set_S *DmxSet)
{
    HI_U32 DropFlag;
    HI_U32 idx;

    DropFlag = DmxHalGetSwitchDropTsFlag(DmxSet);

    for (idx = 0; idx < DmxSet->DmxCnt; idx ++)
    {
        DMX_DmxInfo_S *DmxInfo = &DmxSet->DmxInfo[idx];

        if (DMX_INVALID_DEMUX_ID != DmxInfo->DmxId && DropFlag & (1 << idx))
        {
            HI_ERR_DEMUX("Dmx(%d) had dropped ts packages.\n", DmxInfo->DmxId);
        }

        DmxHalClrSwitchDropTsFlag(DmxSet, DmxInfo->DmxId);
    }
}

static HI_VOID __CheckChnTimeoutProc(Dmx_Set_S *DmxSet)
{
    DmxSwitchOverflowMonitor(DmxSet);

    CommFQMonitor(DmxSet);

    SharedPidMonitor(DmxSet);

    RemuxOverflowMonitor(DmxSet);
}

HI_S32 CheckChnTimeoutProc(HI_VOID *ptr)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;

    while (!kthread_should_stop())
    {
        TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
        {
            __CheckChnTimeoutProc(DmxSet);
        }

        msleep(DMX_CHECKCHN_TIMEOUT);
    }

    return 0;
}

static HI_S32 DmxReset(Dmx_Set_S * DmxSet)
{
    HI_S32 ret;

    ret = DmxHalGetInitStatus(DmxSet);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_DEMUX("status error\n");

        return ret;
    }

    DmxHalEnableMmu(&DmxSet->mmu);

    DmxHalIPPortAutoClearBP(DmxSet);

#ifdef DMX_FILTER_DEPTH_SUPPORT
    DmxHalFilterEnableDepth(DmxSet);
#endif

#ifdef DMX_SECTION_PARSE_NOPUSI_SUPPORT
    DmxHalFilterSetSecStuffCtrl(DmxSet, HI_TRUE);
#endif

    DmxHalEnableRamClkAutoCtl(DmxSet);

    return HI_SUCCESS;
}

extern uint DmxPoolBufSize;
extern uint DmxBlockSize;

#ifdef DMX_MMU_SUPPORT
static HI_S32 DmxAllocPoolBuf(Dmx_Set_S *DmxSet)
{
    HI_S32 ret = HI_FAILURE;
    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[DMX_FQ_COMMOM];
    HI_U32 BufSize, FqDepth, FqBufSize;
    SMMU_BUFFER_S MmuBuf = {0};
    MMZ_BUFFER_S MmzBuf = {0};

    BufSize = (DmxPoolBufSize + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;

    ret = HI_DRV_SMMU_AllocAndMap("DMX_PoolBuf", BufSize , 0, &MmuBuf);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("mmu memory allocate failed, BufSize=0x%x\n", BufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out0;
    }

    FqInfo->BufVirAddr   = MmuBuf.pu8StartVirAddr;
    FqInfo->BufPhyAddr   = MmuBuf.u32StartSmmuAddr;
    FqInfo->BufSize      = BufSize;
    FqInfo->BufFlag      = DMX_MMU_BUF;
    FqInfo->BlockSize    = DmxBlockSize;

    FqDepth     = BufSize / FqInfo->BlockSize + 1;
    FqBufSize   = FqDepth * DMX_FQ_DESC_SIZE;

    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap("DMX_FqDscBuf", MMZ_OTHERS, FqBufSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("memory allocate failed\n");
        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out1;
    }

    FqInfo->FqDescSetPhy    = MmzBuf.u32StartPhyAddr;
    FqInfo->FqDescSetBase    = (FQ_DescInfo_S*)MmzBuf.pu8StartVirAddr;
    FqInfo->FQDepth      = FqDepth;

    return HI_SUCCESS;

 out1:
    MmuBuf.pu8StartVirAddr = FqInfo->BufVirAddr;
    MmuBuf.u32StartSmmuAddr = FqInfo->BufPhyAddr;

    HI_DRV_SMMU_UnmapAndRelease(&MmuBuf);
 out0:
    return ret;
}

static HI_VOID DmxFreePoolBuf(Dmx_Set_S *DmxSet)
{
    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[DMX_FQ_COMMOM];
    SMMU_BUFFER_S MmuBuf = {0};
    MMZ_BUFFER_S MmzBuf = {0};

    MmzBuf.pu8StartVirAddr  = (HI_U8*)FqInfo->FqDescSetBase;
    MmzBuf.u32StartPhyAddr  = FqInfo->FqDescSetPhy;

    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);

    MmuBuf.pu8StartVirAddr  = FqInfo->BufVirAddr;
    MmuBuf.u32StartSmmuAddr  = FqInfo->BufPhyAddr;

    HI_DRV_SMMU_UnmapAndRelease(&MmuBuf);
}
#else
static HI_S32 DmxAllocPoolBuf(Dmx_Set_S *DmxSet)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 BufSize, FqDepth, FqBufSize;
    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[DMX_FQ_COMMOM];
    MMZ_BUFFER_S MmzBuf = {0};

    BufSize     = (DmxPoolBufSize + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;
    FqDepth     = BufSize / DmxBlockSize + 1;
    FqBufSize   = FqDepth * DMX_FQ_DESC_SIZE;

    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap("DMX_PoolBuf", MMZ_OTHERS, BufSize + FqBufSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("memory allocate failed\n");
        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out;
    }

    FqInfo->BufPhyAddr   = MmzBuf.u32StartPhyAddr;
    FqInfo->BufVirAddr   = MmzBuf.pu8StartVirAddr;
    FqInfo->BufSize      = BufSize;
    FqInfo->BufFlag      = DMX_MMZ_BUF;
    FqInfo->BlockSize    = DmxBlockSize;
    FqInfo->FQDepth      = FqDepth;
    FqInfo->FqDescSetPhy     = FqInfo->BufPhyAddr + BufSize;
    FqInfo->FqDescSetBase    = (FQ_DescInfo_S*)(FqInfo->BufVirAddr + BufSize);

    ret = HI_SUCCESS;

 out:
    return ret;
}

static HI_VOID DmxFreePoolBuf(Dmx_Set_S *DmxSet)
{
    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[DMX_FQ_COMMOM];
    MMZ_BUFFER_S MmzBuf = {0};

    MmzBuf.pu8StartVirAddr  = FqInfo->BufVirAddr;
    MmzBuf.u32StartPhyAddr  = FqInfo->BufPhyAddr;

    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);
}
#endif

static HI_S32 DMX_OsiInitDmxSet(Dmx_Set_S * DmxSet)
{
    HI_S32 ret = HI_FAILURE;
    HI_UNF_DMX_PORT_ATTR_S  PortAttr;
    HI_UNF_DMX_TSO_PORT_ATTR_S  TSOPortAttr;

    HI_U32                  i;

    ret = DmxReset(DmxSet);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = DmxAllocPoolBuf(DmxSet);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    DmxFqStart(DmxSet, DMX_FQ_COMMOM);

    /* initialise tuner port */
    for (i = 0; i < DmxSet->TunerPortCnt; i++)
    {
        PortAttr.enPortMod              = (i < GetDmxCluster()->Ops->GetIFPortNum())?HI_UNF_DMX_PORT_MODE_INTERNAL:HI_UNF_DMX_PORT_MODE_EXTERNAL;
        PortAttr.enPortType             = HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188;
        PortAttr.u32SyncLostTh          = 1;
        PortAttr.u32SyncLockTh          = 5;
        PortAttr.u32TunerInClk          = 0;
        PortAttr.u32SerialBitSelector   = 0;
        PortAttr.u32TunerErrMod         = 0;
        PortAttr.u32UserDefLen1         = 0;
        PortAttr.u32UserDefLen2         = 0;

        __DMX_OsiTunerPortSetAttr(DmxSet, i, &PortAttr);

        DmxHalDvbPortSetTsCountCtrl(DmxSet, i, TS_COUNT_CRTL_START);
        DmxHalDvbPortSetErrTsCountCtrl(DmxSet, i, TS_COUNT_CRTL_START);
    }

    /* initialise ram port */
    for (i = 0; i < DmxSet->RamPortCnt; i++)
    {
        DMX_RamPort_Info_S *PortInfo = &DmxSet->RamPortInfo[i];

        init_waitqueue_head(&PortInfo->WaitQueue);
        PortInfo->IpRate = DMX_DEFAULT_IP_SPEED;

        PortAttr.enPortMod              = HI_UNF_DMX_PORT_MODE_RAM;
#ifdef DMX_RAM_PORT_AUTO_SCAN_SUPPORT
        PortAttr.enPortType             = HI_UNF_DMX_PORT_TYPE_AUTO;
#else
        PortAttr.enPortType             = HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188_204;
#endif
        PortAttr.u32SyncLostTh          = 0;
        PortAttr.u32SyncLockTh          = 7;
        PortAttr.u32TunerInClk          = 0;
        PortAttr.u32SerialBitSelector   = 0;
        PortAttr.u32TunerErrMod         = 0;
        PortAttr.u32UserDefLen1         = 0;
        PortAttr.u32UserDefLen2         = 0;

        __DMX_OsiRamPortSetAttr(DmxSet, i, &PortAttr);

        DmxHalIPPortEnableInt(DmxSet, i);
        DmxHalIPPortSetIntCnt(DmxSet, i, DMX_DEFAULT_INT_CNT);

        DmxHalIPPortSetTsCountCtrl(DmxSet, i, HI_TRUE);
    }

    /* initialise TSO port */
    for ( i = 0 ; i < DmxSet->TSOPortCnt; i++ )
    {
        TSOPortAttr.bEnable         = HI_TRUE;
        TSOPortAttr.bClkReverse     = HI_TRUE;
        TSOPortAttr.enTSSource      = HI_UNF_DMX_PORT_TSI_0;
        TSOPortAttr.enClkMode       = HI_UNF_DMX_TSO_CLK_MODE_NORMAL;
        TSOPortAttr.enValidMode     = HI_UNF_DMX_TSO_VALID_ACTIVE_OUTPUT;
        TSOPortAttr.bBitSync        = HI_TRUE;
        TSOPortAttr.bSerial         = HI_TRUE;
        TSOPortAttr.enBitSelector   = HI_UNF_DMX_TSO_SERIAL_BIT_0;
        TSOPortAttr.bLSB            = HI_FALSE;

        DmxHalGetTSOClkCfg(i, &TSOPortAttr.bClkReverse, (HI_U32*)&TSOPortAttr.enClk, &TSOPortAttr.u32ClkDiv);
        DMX_OsiTSOPortSetAttr(i, &TSOPortAttr);
    }

    /* initialise tagdeal port */
#if defined(DMX_TAGPORT_CNT) && (DMX_TAGPORT_CNT > 0)
    {
        DMX_TagPort_Info_S *TagPortInfo = DmxSet->TagPortInfo;

        TagPortInfo->enSyncMod = HI_UNF_DMX_TAG_HEAD_SYNC;
        TagPortInfo->u32TagLen = DMX_DEFAULT_TAG_LENGTH; /* bytes */
        TagPortInfo->enTSSource = HI_UNF_DMX_PORT_BUTT;

        for (i = 0; i < DmxSet->TagPortCnt ; i++)
        {
            TagPortInfo->TagPortAttrs[i].enSyncMod = TagPortInfo->enSyncMod;
            TagPortInfo->TagPortAttrs[i].u32TagLen = TagPortInfo->u32TagLen;
            TagPortInfo->TagPortAttrs[i].enTSSource = TagPortInfo->enTSSource;

            memset(&(TagPortInfo->TagPortAttrs[i].au8Tag[0]), 0, sizeof(TagPortInfo->TagPortAttrs[i].au8Tag));
        }

        /* disable TS port tag deal mode default */
        __DmxHalSetTagDealCtl(HI_FALSE, TagPortInfo->enTSSource, TagPortInfo->enSyncMod, TagPortInfo->u32TagLen);
    }
#endif

    /* initialise Dmx */
    for (i = 0; i < DmxSet->DmxCnt; i++)
    {
        DMX_DmxInfo_S  *DmxInfo = &DmxSet->DmxInfo[i];

        DmxInfo->DmxId     = DMX_INVALID_DEMUX_ID;
        DmxInfo->PortId     = DMX_INVALID_PORT_ID;
        DmxInfo->PortMode   = DMX_PORT_MODE_BUTT;
        atomic_set(&DmxInfo->RecLimit.RecInstCounter, 0);

        DmxHalSetSpsRefRecCh(DmxSet, i, 0xff);
    }

    /* initialise Dmx Rec */
    for (i = 0; i < DmxSet->DmxRecCnt; i ++)
    {
        DMX_RecInfo_S *RecInfo = &DmxSet->DmxRecInfo[i];

        RecInfo->DmxId = DMX_INVALID_DEMUX_ID;
        RecInfo->RecId = DMX_INVALID_REC_ID;
        INIT_LIST_HEAD(&RecInfo->head);
        RecInfo->RemIdxCnt = 0;

        mutex_init(&RecInfo->LockRec);
    }

    /* initialise Dmx channel */
    for (i = 0; i < DmxSet->DmxChanCnt; i++)
    {
        DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[i];

        ChanInfo->DmxId             = DMX_INVALID_DEMUX_ID;
        ChanInfo->ChanPid           = DMX_INVALID_PID;
        ChanInfo->ChanId            = DMX_INVALID_CHAN_ID;
        ChanInfo->KeyId             = DMX_INVALID_KEY_ID;

        mutex_init(&ChanInfo->LockChn);
    }

    /* initialise Dmx Filter */
    for (i = 0; i < DmxSet->DmxFilterCnt; i++)
    {
        DMX_FilterInfo_S *FilterInfo = &DmxSet->DmxFilterInfo[i];

        FilterInfo->ChanId      = DMX_INVALID_CHAN_ID;
        FilterInfo->FilterId    = DMX_INVALID_FILTER_ID;
        FilterInfo->Depth       = 0;

        mutex_init(&FilterInfo->LockFlt);
    }

#ifdef DMX_DESCRAMBLER_SUPPORT
    /* initialise Dmx Key */
    for (i = 0; i < DmxSet->DmxKeyCnt; i++)
    {
        DMX_KeyInfo_S *KeyInfo = &DmxSet->DmxKeyInfo[i];

        DescramblerReset(DmxSet, i, KeyInfo);

        mutex_init(&KeyInfo->LockKey);
    }
#endif

    /* initialise Dmx pcr channel */
    for (i = 0; i < DmxSet->DmxPcrCnt; i++)
    {
        DmxSet->DmxPcrInfo[i].DmxId = DMX_INVALID_DEMUX_ID;

        mutex_init(&DmxSet->DmxPcrInfo[i].LockPcr);
    }

    /* initialise Dmx OQ */
    for (i = 0; i < DmxSet->DmxOqCnt; i++)
    {
        DMX_OQ_Info_S  *OqInfo = &DmxSet->DmxOqInfo[i];

        OqInfo->u32OQId     = i;
        OqInfo->OqWakeUp    = HI_FALSE;

        atomic_set(&OqInfo->EopIntActiveCount, 0);
        init_waitqueue_head(&OqInfo->OqWaitQueue);
        OqInfo->OqWaitCondition = 0;
        mutex_init(&OqInfo->LockOq);
    }

    /* initialise Dmx FQ */
    for (i = 0; i < DmxSet->DmxFqCnt; i++)
    {
        DMX_FQ_Info_S  *FqInfo = &DmxSet->DmxFqInfo[i];

        spin_lock_init(&FqInfo->LockFq);
    }

    /* initialise Dmx SCD */
    for (i = 0; i < DmxSet->DmxScdCnt; i++)
    {
        DMX_SCD_Info_S *ScdInfo = &DmxSet->DmxScdInfo[i];

        spin_lock_init(&ScdInfo->LockScd);
    }

    /* initialise remux channel */
    for (i = 0; i < DmxSet->RmxChanCnt; i++)
    {
        RMX_Chan_Info_S *RmxChanInfo = &DmxSet->RmxChanInfo[i];

        mutex_init(&RmxChanInfo->LockRmx);
    }

    /* initialise remux pump */
    for (i = 0; i < DmxSet->RmxPumpCnt; i++)
    {
        RMX_Pump_Info_S *RmxPumpInfo = &DmxSet->RmxPumpInfo[i];

        RmxPumpInfo->Type = HI_MPI_RMX_PUMP_TYPE_BUTT;

        mutex_init(&RmxPumpInfo->LockPump);
    }

    // others global settings
    DmxHalEnablePidCopy(DmxSet);

    DmxHalEnableAllDavInt(DmxSet);
    DmxHalEnableAllChEopInt(DmxSet);
    DmxHalEnableAllChEnqueInt(DmxSet);
    DmxHalFQEnableAllOverflowInt(DmxSet);
    DmxHalEnableD2BFOvflInt(DmxSet);
    DmxHalEnableD2BROvflInt(DmxSet);

    DmxHalSetFlushMaxWaitTime(DmxSet, 0x2400);
    DmxHalSetDataFakeMod(DmxSet, DMX_ENABLE);

#ifdef DMX_DESCRAMBLER_SUPPORT
    DescInitHardFlag(DmxSet);
#endif

    memset(&psErrMsg, 0, sizeof(DMX_ERRMSG_S) * DMX_MAX_ERRLIST_NUM);

    if (0 != request_irq(DmxSet->Irq, (irq_handler_t) DMXOsiIsr, IRQF_DISABLED, "hi_dmx_irq", (HI_VOID*)DmxSet))
    {
        HI_ERR_DEMUX("request_irq(%d) error.\n", DmxSet->Irq);
        ret = HI_FAILURE;
        goto out1;
    }

    if (HI_SUCCESS != HI_DRV_SYS_SetIrqAffinity(HI_ID_DEMUX, DmxSet->Irq))
    {
        HI_WARN_DEMUX("set dmx irq affinity failed.\n");
    }

    DmxHalEnableAllPVRInt(DmxSet);

    return HI_SUCCESS;

out1:
    DmxFqStop(DmxSet, DMX_FQ_COMMOM);

    DmxFreePoolBuf(DmxSet);
out0:
    return ret;
}

static HI_S32 DMX_OsiDeInitDmxSet(Dmx_Set_S *DmxSet)
{
    MMZ_BUFFER_S MmzBuf = {0};
    HI_U32 i;

    DmxHalDisableAllPVRInt(DmxSet);

    free_irq(DmxSet->Irq, (HI_VOID*)DmxSet);

    for (i = 0; i < DmxSet->RamPortCnt; i++)
    {
        if (0 != DmxSet->RamPortInfo[i].DescPhyAddr)
        {
            MmzBuf.u32StartPhyAddr = DmxSet->RamPortInfo[i].DescPhyAddr;
            MmzBuf.pu8StartVirAddr = DmxSet->RamPortInfo[i].DescKerAddr;

            HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);
        }

        DmxHalIPPortDisableInt(DmxSet, i);
        DmxHalIPPortSetTsCountCtrl(DmxSet, i, HI_FALSE);
    }

    for (i = 0; i < DmxSet->TunerPortCnt; i++)
    {
        DmxHalDvbPortSetTsCountCtrl(DmxSet, i, TS_COUNT_CRTL_STOP);
        DmxHalDvbPortSetTsCountCtrl(DmxSet, i, TS_COUNT_CRTL_RESET);

        DmxHalDvbPortSetErrTsCountCtrl(DmxSet, i, TS_COUNT_CRTL_STOP);
        DmxHalDvbPortSetErrTsCountCtrl(DmxSet, i, TS_COUNT_CRTL_RESET);
    }

    DmxFqStop(DmxSet, DMX_FQ_COMMOM);

    DmxFreePoolBuf(DmxSet);

    return HI_SUCCESS;
}

HI_S32 DMX_OsiRelDmxSetPort(struct file * file)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet;
    HI_U32 PortId;

    /* release ts buffer(ramport) */
    for(PortId = 0 ; PortId < DmxCluster->Ops->GetRamPortNum(); PortId ++)
    {
        DMX_RamPort_Info_S *PortInfo;

        DmxSet = GetDmxSetByRamid(PortId);
        PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);

        if (PortInfo->Owner == (pid_t)(unsigned long)file->private_data)
        {
            DMX_OsiTsBufferDestroy(PortId);
        }
    }

    return HI_SUCCESS;
}

HI_S32 DMX_OsiRelDmxSetComm(Dmx_Set_S * DmxSet, struct file * file)
{
    HI_S32 ret= HI_SUCCESS;  /* we don't care any failure, but print error or warning. */
    HI_U32 RecId, FltId, ChanId, PcrId, RmxChanId;

    /* release filter */
    mutex_lock(&DmxSet->LockAllFlt);
    for_each_set_bit(FltId, DmxSet->FilterBitmap, DmxSet->DmxFilterCnt)
    {
        DMX_FilterInfo_S *FltInfo = &DmxSet->DmxFilterInfo[FltId];

        if (FltInfo->Owner == (pid_t)(unsigned long)file->private_data)
        {
            DMX_OsiDeleteFilter(FltInfo->DmxId, FltId);
        }
    }
    mutex_unlock(&DmxSet->LockAllFlt);

#ifdef DMX_DESCRAMBLER_SUPPORT
    DmxDestroyAllDescrambler(DmxSet, file);
#endif

    /* release rec channel */
    mutex_lock(&DmxSet->LockAllRec);
    for_each_set_bit(RecId, DmxSet->RecBitmap, DmxSet->DmxRecCnt)
    {
        DMX_RecInfo_S *RecInfo = &DmxSet->DmxRecInfo[RecId];

        if (RecInfo->Owner ==  (pid_t)(unsigned long)file->private_data)
        {
            DMX_DRV_REC_StopRecChn(RecInfo->DmxId, RecId);

            DMX_DRV_REC_DelAllRecPid(RecInfo->DmxId, RecId);

            DMX_DRV_REC_DestroyChannel(RecInfo->DmxId, RecId);
        }
    }
    mutex_unlock(&DmxSet->LockAllRec);

    /* release play channel */
    mutex_lock(&DmxSet->LockAllChn);
    for_each_set_bit(ChanId, DmxSet->ChnBitmap, DmxSet->DmxChanCnt)
    {
        DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

        if (ChanInfo->Owner ==  (pid_t)(unsigned long)file->private_data)
        {
            DMX_OsiDestroyChannel(ChanInfo->DmxId, ChanId);
        }
    }
    mutex_unlock(&DmxSet->LockAllChn);

    /* release pcr channel */
    mutex_lock(&DmxSet->LockAllPcr);
    for_each_set_bit(PcrId, DmxSet->PcrBitmap, DmxSet->DmxPcrCnt)
    {
        DMX_PCR_Info_S *PcrInfo = &DmxSet->DmxPcrInfo[PcrId];

        if (PcrInfo->Owner ==  (pid_t)(unsigned long)file->private_data)
        {
            DMX_OsiPcrChannelDestroy(PcrInfo->DmxId, PcrId);
        }
    }
    mutex_unlock(&DmxSet->LockAllPcr);

    /* release remux */
    mutex_lock(&DmxSet->LockAllRmx);
    for_each_set_bit(RmxChanId, DmxSet->RmxChanBitmap, DmxSet->RmxChanCnt)
    {
        RMX_Chan_Info_S *RmxChanInfo = &DmxSet->RmxChanInfo[RmxChanId];

        if (RmxChanInfo->Owner ==  (pid_t)(unsigned long)file->private_data)
        {
            RMX_OsiDestroy(RmxChanInfo->RmxId, RmxChanId);
        }
    }
    mutex_unlock(&DmxSet->LockAllRmx);

    return ret;
}

/*
 * simple dynamic tune dmx clock rule(range: [0 ~ 31]):
 * 1. no demux attached to any port, reduce = 31;
 * 2. 1 demux attached to any port, reduce = 10;
 * 3. more than 1 demux attached to any port, reduce = 0;
 * 4. ram port need external bandwidth, so we increase using count.
 * 5. rmx port no need reduce.
 * see actual dmx clk from offset 0x2320.
 */
static HI_VOID DMXDynamicTuneDmxClk(HI_VOID)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;
    HI_U32 ActiveCnt = 0;

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        HI_U32 index;

        for (index = 0; index < DmxSet->DmxCnt; index++)
        {
            DMX_DmxInfo_S *DmxInfo = &DmxSet->DmxInfo[index];

            if (DMX_INVALID_PORT_ID != DmxInfo->PortId)
            {
                BUG_ON(DmxInfo->DmxId >= DmxCluster->Ops->GetDmxNum());

                ActiveCnt ++;

                if (DMX_PORT_MODE_RAM == DmxInfo->PortMode)
                {
                    ActiveCnt ++ ;
                }

                if (DMX_PORT_MODE_RMX == DmxInfo->PortMode)
                {
                    ActiveCnt += 2 ;
                }
            }
        }
    }

    switch(ActiveCnt)
    {
        case 0:
            DmxHalDynamicTuneDmxClk(31);
            break;
        case 1:
            DmxHalDynamicTuneDmxClk(10);
            break;
        default:
            DmxHalDynamicTuneDmxClk(0);
            break;
    }
}

/*
 * NO error handling for DMX_OsiInitDmxSet, trigger BUG if it execute failed.
 */
static HI_VOID DmxStartCluster(Dmx_Cluster_S *DmxCluster)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;

    DmxHalConfigHardware();

    DmxHalEnableGblMmu(&DmxCluster->mmu);

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        ret = DMX_OsiInitDmxSet(DmxSet);
        if (HI_SUCCESS != ret)
        {
            HI_FATAL_DEMUX("start demux set(%d) failed.\n", DmxSet->SetId);
            BUG();
        }
    }

    DMXDynamicTuneDmxClk();

    g_DmxMonitorThread = kthread_create(CheckChnTimeoutProc, HI_NULL, "dmx_monitor");
    BUG_ON(IS_ERR(g_DmxMonitorThread));

    wake_up_process(g_DmxMonitorThread);

    g_SelectWorkQueue = create_singlethread_workqueue("dmx_wq");
    BUG_ON(!g_SelectWorkQueue);
}

/*
 * NO error handling for DMX_OsiDeInitDmxSet, trigger BUG if it execute failed.
 */
static HI_VOID DmxStopCluster(Dmx_Cluster_S *DmxCluster)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;

    DmxDeinitTeec(DmxCluster);

    if (g_SelectWorkQueue)
    {
        destroy_workqueue(g_SelectWorkQueue);
        g_SelectWorkQueue= HI_NULL;
    }

    if (g_DmxMonitorThread)
    {
        kthread_stop(g_DmxMonitorThread);
        g_DmxMonitorThread = HI_NULL;
    }

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        ret = DMX_OsiDeInitDmxSet(DmxSet);
        if (HI_SUCCESS != ret)
        {
            HI_FATAL_DEMUX("stop demux(%d) failed.\n");
            BUG();
        }
    }
    /* Deconfig the clock */
    DmxHalDeConfigHardware();
}

static DMX_Cluster_State_E DmxCluster_GetClusterState(HI_VOID)
{
    unsigned long flags;
    DMX_Cluster_State_E State;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();

    spin_lock_irqsave(&DmxCluster->lock, flags);

    State = DmxCluster->state;

    spin_unlock_irqrestore(&DmxCluster->lock, flags);

    return State;
}

static HI_S32 DmxCluster_StartCluster(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;
    unsigned long flags;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();

    spin_lock_irqsave(&DmxCluster->lock, flags);

    if (DMX_CLUSTER_ACTIVED == DmxCluster->state)
    {
        ret = HI_SUCCESS;
        spin_unlock_irqrestore(&DmxCluster->lock, flags);
        goto out;
    }

    DmxCluster->state = DMX_CLUSTER_ACTIVED;

    spin_unlock_irqrestore(&DmxCluster->lock, flags);

    DmxStartCluster(DmxCluster);

    ret = HI_SUCCESS;

out:
    return ret;
}

static HI_S32 DmxCluster_StopCluster(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;
    unsigned long flags;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();

    spin_lock_irqsave(&DmxCluster->lock, flags);

    if (DMX_CLUSTER_INACTIVED == DmxCluster->state)
    {
        ret = HI_SUCCESS;
        spin_unlock_irqrestore(&DmxCluster->lock, flags);
        goto out;
    }

    DmxCluster->state = DMX_CLUSTER_INACTIVED;

    spin_unlock_irqrestore(&DmxCluster->lock, flags);

    DmxStopCluster(DmxCluster);

    ret = HI_SUCCESS;

out:
    return ret;
}

static HI_S32 DmxCluster_ReleaseCluster(struct file * file)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet;

    DMX_OsiRelDmxSetPort(file);

    list_for_each_entry(DmxSet, &DmxCluster->head, list)
    {
        DMX_OsiRelDmxSetComm(DmxSet, file);
    }

    return HI_SUCCESS;
}

/*
 * Dmx Cluster factory functions.
 */
static HI_S32 DmxCluster_GetDmxSet(const HI_U32 DmxSetId, Dmx_Set_S **DmxSet)
{
    HI_S32 ret = HI_FAILURE;
    unsigned long flags;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *tmp;

    BUG_ON(!DmxSet);

    spin_lock_irqsave(&DmxCluster->lock, flags);

    list_for_each_entry(tmp, &DmxCluster->head, list)
    {
        if (tmp->SetId == DmxSetId)
        {
            *DmxSet = tmp;
            ret = HI_SUCCESS;
            break;
        }
    }

    spin_unlock_irqrestore(&DmxCluster->lock, flags);

    return ret;
}

static HI_S32 DmxCluster_AddDmxSet(Dmx_Set_S *DmxSet)
{
    unsigned long flags;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();

    BUG_ON(!DmxSet);

    spin_lock_irqsave(&DmxCluster->lock, flags);
    list_add_tail(&DmxSet->list, &DmxCluster->head);
    spin_unlock_irqrestore(&DmxCluster->lock, flags);

    return HI_SUCCESS;
}

static HI_S32 DmxCluster_DelDmxSet(const HI_U32 DmxSetId)
{
    HI_S32 ret = HI_FAILURE;
    unsigned long flags;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *tmp1, *tmp2;

    spin_lock_irqsave(&DmxCluster->lock, flags);

    list_for_each_entry_safe(tmp1, tmp2, &DmxCluster->head, list)
    {
        if (tmp1->SetId == DmxSetId)
        {
            list_del(&tmp1->list);

            iounmap(tmp1->IoBase);

            ret = HI_SUCCESS;
            break;
        }
    }

    spin_unlock_irqrestore(&DmxCluster->lock, flags);

    return ret;
}

static HI_U32 DmxCluster_GetIFPortNum(HI_VOID)
{
    return DMX_IFPORT_CNT;
}

static HI_U32 DmxCluster_GetTSIPortNum(HI_VOID)
{
    return DMX_TSIPORT_CNT;
}

static HI_U32 DmxCluster_GetTagPortNum(HI_VOID)
{
    return DMX_TAGPORT_CNT;
}

static HI_U32 DmxCluster_GetTsoPortNum(HI_VOID)
{
    HI_U32 total = 0;
    unsigned long flags;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet;

    spin_lock_irqsave(&DmxCluster->lock, flags);

    list_for_each_entry(DmxSet, &DmxCluster->head, list)
    {
        total += DmxSet->TSOPortCnt;
    }

    spin_unlock_irqrestore(&DmxCluster->lock, flags);

    return total;
}

static HI_U32 DmxCluster_GetRamPortNum(HI_VOID)
{
    HI_U32 total = 0;
    unsigned long flags;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet;

    spin_lock_irqsave(&DmxCluster->lock, flags);

    list_for_each_entry(DmxSet, &DmxCluster->head, list)
    {
        total += DmxSet->RamPortCnt;
    }

    spin_unlock_irqrestore(&DmxCluster->lock, flags);

    return total;
}

static HI_U32 DmxCluster_GetDmxNum(HI_VOID)
{
    HI_U32 total = 0;
    unsigned long flags;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet;

    spin_lock_irqsave(&DmxCluster->lock, flags);

    list_for_each_entry(DmxSet, &DmxCluster->head, list)
    {
        total += DmxSet->DmxCnt;
    }

    spin_unlock_irqrestore(&DmxCluster->lock, flags);

    return total;
}

static HI_U32 DmxCluster_GetRecNum(HI_VOID)
{
    HI_U32 total = 0;
    unsigned long flags;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet;

    spin_lock_irqsave(&DmxCluster->lock, flags);

    list_for_each_entry(DmxSet, &DmxCluster->head, list)
    {
        total += DmxSet->DmxRecCnt;
    }

    spin_unlock_irqrestore(&DmxCluster->lock, flags);

    return total;
}

static HI_U32 DmxCluster_GetChnNum(HI_VOID)
{
    HI_U32 total = 0;
    unsigned long flags;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet;

    spin_lock_irqsave(&DmxCluster->lock, flags);

    list_for_each_entry(DmxSet, &DmxCluster->head, list)
    {
        total += DmxSet->DmxChanCnt;
    }

    spin_unlock_irqrestore(&DmxCluster->lock, flags);

    return total;
}

/* should deprecate */
static HI_U32 DmxCluster_GetAVChnNum(HI_VOID)
{
    return DMX_AV_CHANNEL_CNT;
}

static HI_U32 DmxCluster_GetFilterNum(HI_VOID)
{
    HI_U32 total = 0;
    unsigned long flags;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet;

    spin_lock_irqsave(&DmxCluster->lock, flags);

    list_for_each_entry(DmxSet, &DmxCluster->head, list)
    {
        total += DmxSet->DmxFilterCnt;
    }

    spin_unlock_irqrestore(&DmxCluster->lock, flags);

    return total;
}

/* FIXME: */
static HI_U32 DmxCluster_GetKeyNum(HI_VOID)
{
    return DMX_KEY_CNT;
}

static HI_U32 DmxCluster_GetRmxNum(HI_VOID)
{
    HI_U32 total = 0;
    unsigned long flags;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet;

    spin_lock_irqsave(&DmxCluster->lock, flags);

    list_for_each_entry(DmxSet, &DmxCluster->head, list)
    {
        total += DmxSet->RmxChanCnt;
    }

    spin_unlock_irqrestore(&DmxCluster->lock, flags);

    return total;
}


static struct Dmx_Cluster_Operations DmxClusterOps =
{
    .GetClusterState = DmxCluster_GetClusterState,
    .StartCluster = DmxCluster_StartCluster,
    .StopCluster = DmxCluster_StopCluster,
    .ReleaseCluster = DmxCluster_ReleaseCluster,

    .GetDmxSet = DmxCluster_GetDmxSet,
    .AddDmxSet = DmxCluster_AddDmxSet,
    .DelDmxSet = DmxCluster_DelDmxSet,

    .SendCmdToTA = DmxCluster_SendCmdToTA,

    .GetIFPortNum = DmxCluster_GetIFPortNum,
    .GetTSIPortNum = DmxCluster_GetTSIPortNum,
    .GetTagPortNum = DmxCluster_GetTagPortNum,
    .GetTsoPortNum = DmxCluster_GetTsoPortNum,
    .GetRamPortNum = DmxCluster_GetRamPortNum,
    .GetDmxNum = DmxCluster_GetDmxNum,
    .GetRecNum = DmxCluster_GetRecNum,
    .GetChnNum = DmxCluster_GetChnNum,
    .GetAVChnNum = DmxCluster_GetAVChnNum,
    .GetFilterNum = DmxCluster_GetFilterNum,
    .GetKeyNum = DmxCluster_GetKeyNum,
    .GetRmxNum = DmxCluster_GetRmxNum,
};

/*
 * demux set share all tuner port .
 */
static DMX_TunerPort_Info_S* DmxSet_GetTunerPortInfo(const Dmx_Set_S *DmxSet, const HI_U32 TunerPortId)
{
    if(TunerPortId < DmxSet->TunerPortCnt)
    {
        return &DmxSet->TunerPortInfo[TunerPortId];
    }

    return HI_NULL;
}

static DMX_TunerPort_Info_S* DmxSet_GetTunerPortInfoByRawId(const Dmx_Set_S *DmxSet, const HI_U32 TunerRawPortId)
{
    if(TunerRawPortId < DmxSet->TunerPortCnt)
    {
        return &DmxSet->TunerPortInfo[TunerRawPortId];
    }

    return HI_NULL;
}

static HI_U32 DmxSet_GetTunerRawId(const Dmx_Set_S *DmxSet, const HI_U32 TunerPortId)
{
    if (TunerPortId < DmxSet->TunerPortCnt)
    {
        return TunerPortId;
    }

    return DMX_INVALID_PORT_ID;
}

/*
 * each demux set has private ram port.
 */
static DMX_RamPort_Info_S* DmxSet_GetRamPortInfo(const Dmx_Set_S *DmxSet, const HI_U32 RamPortId)
{
    if (DmxSet->RamPortCnt)
    {
        return &DmxSet->RamPortInfo[RamPortId % DmxSet->RamPortCnt];
    }

    return HI_NULL;
}

static DMX_RamPort_Info_S* DmxSet_GetRamPortInfoByRawId(const Dmx_Set_S *DmxSet, const HI_U32 RamRawPortId)
{
    if (RamRawPortId < DmxSet->RamPortCnt)
    {
        return &DmxSet->RamPortInfo[RamRawPortId];
    }

    return HI_NULL;
}

static HI_U32 DmxSet_GetRamRawId(const Dmx_Set_S *DmxSet, const HI_U32 RamPortId)
{
    if (DmxSet->RamPortCnt)
    {
        return RamPortId % DmxSet->RamPortCnt;
    }

    return DMX_INVALID_PORT_ID;
}

static DMX_DmxInfo_S* DmxSet_GetDmxInfo(const Dmx_Set_S *DmxSet, const HI_U32 DmxId)
{
    if (DmxSet->DmxCnt)
    {
        return &DmxSet->DmxInfo[DmxId % DmxSet->DmxCnt];
    }

    return HI_NULL;
}

static HI_U32 DmxSet_GetDmxRawId(const Dmx_Set_S *DmxSet, const HI_U32 DmxId)
{
    if (DmxSet->DmxCnt)
    {
        return DmxId % DmxSet->DmxCnt;
    }

    return DMX_INVALID_DEMUX_ID;
}

static HI_U32 DmxSet_GetRmxRawId(const Dmx_Set_S *DmxSet, const HI_U32 RmxId)
{
    if (DmxSet->RmxChanCnt)
    {
        return RmxId % DmxSet->RmxChanCnt;
    }

    return DMX_INVALID_REMUX_ID;
}

#ifdef DMX_MMU_SUPPORT
#ifdef DMX_MMU_VERSION_1
#define INIT_GBL_MMU_DEV(Mmu) ({ \
    Mmu->IoBase = ioremap_nocache(DMX_MMU_BASE, 0x10000); \
    BUG_ON(!Mmu->IoBase); \
    Mmu->CbTtbr = 0;\
})

#define UNIT_GBL_MMU_DEV(Mmu) ({ \
    BUG_ON(!Mmu->IoBase); \
    iounmap(Mmu->IoBase); \
})
#elif defined(DMX_MMU_VERSION_2)
#define INIT_GBL_MMU_DEV(Mmu) ({ \
    Mmu->IoBase = HI_NULL; \
})

#define UNIT_GBL_MMU_DEV(Mmu) ({ \
})
#endif
#else
#define INIT_GBL_MMU_DEV(Mmu) ({ \
    Mmu->IoBase = HI_NULL; \
})

#define UNIT_GBL_MMU_DEV(Mmu) ({ \
})
#endif

static struct Dmx_Set_Operations DmxSetOps =
{
    .GetTunerPortInfo = DmxSet_GetTunerPortInfo,
    .GetTunerPortInfoByRawId = DmxSet_GetTunerPortInfoByRawId,
    .GetTunerRawId = DmxSet_GetTunerRawId,
    .GetRamPortInfo = DmxSet_GetRamPortInfo,
    .GetRamPortInfoByRawId = DmxSet_GetRamPortInfoByRawId,
    .GetRamRawId = DmxSet_GetRamRawId,
    .GetDmxInfo = DmxSet_GetDmxInfo,
    .GetDmxRawId = DmxSet_GetDmxRawId,
    .GetRmxRawId = DmxSet_GetRmxRawId,
};

#define ADD_DMX_SET(ID, DmxSetInst) ({ \
    HI_S32 ret = HI_FAILURE; \
    BUG_ON(!DmxSetInst); \
    DmxSetInst->DmxSet.TunerPortInfo = DmxSetInst->TunerPortInfo;\
    DmxSetInst->DmxSet.TunerPortCnt = sizeof(DmxSetInst->TunerPortInfo) / sizeof(DMX_TunerPort_Info_S);\
    DmxSetInst->DmxSet.RamPortInfo = DmxSetInst->RamPortInfo;\
    DmxSetInst->DmxSet.RamPortCnt = sizeof(DmxSetInst->RamPortInfo) / sizeof(DMX_RamPort_Info_S);\
    DmxSetInst->DmxSet.TagPortInfo = &DmxSetInst->TagPortInfo;\
    DmxSetInst->DmxSet.TagPortCnt = sizeof(DmxSetInst->TagPortInfo.TagPortAttrs) / sizeof(HI_UNF_DMX_TAG_ATTR_S); \
    DmxSetInst->DmxSet.TSOPortInfo = DmxSetInst->TSOPortInfo;\
    DmxSetInst->DmxSet.TSOPortCnt = sizeof(DmxSetInst->TSOPortInfo) / sizeof(HI_UNF_DMX_TSO_PORT_ATTR_S);\
    DmxSetInst->DmxSet.DmxInfo = DmxSetInst->DmxInfo;\
    DmxSetInst->DmxSet.DmxCnt = sizeof(DmxSetInst->DmxInfo) / sizeof(DMX_DmxInfo_S);\
    DmxSetInst->DmxSet.DmxRecInfo = DmxSetInst->DmxRecInfo;\
    DmxSetInst->DmxSet.DmxRecCnt = sizeof(DmxSetInst->DmxRecInfo) / sizeof(DMX_RecInfo_S);\
    mutex_init(&DmxSetInst->DmxSet.LockAllRec);\
    mutex_init(&DmxSetInst->DmxSet.LockRecPid);\
    bitmap_zero(DmxSetInst->DmxSet.RecBitmap, DmxSetInst->DmxSet.DmxRecCnt); \
    DmxSetInst->DmxSet.DmxChanInfo = DmxSetInst->DmxChanInfo;\
    DmxSetInst->DmxSet.DmxChanCnt = sizeof(DmxSetInst->DmxChanInfo) / sizeof(DMX_ChanInfo_S);\
    mutex_init(&DmxSetInst->DmxSet.LockAllChn);\
    bitmap_zero(DmxSetInst->DmxSet.ChnBitmap, DmxSetInst->DmxSet.DmxChanCnt); \
    DmxSetInst->DmxSet.DmxFilterInfo = DmxSetInst->DmxFilterInfo;\
    DmxSetInst->DmxSet.DmxFilterCnt = sizeof(DmxSetInst->DmxFilterInfo) / sizeof(DMX_FilterInfo_S);\
    mutex_init(&DmxSetInst->DmxSet.LockAllFlt);\
    bitmap_zero(DmxSetInst->DmxSet.FilterBitmap, DmxSetInst->DmxSet.DmxFilterCnt); \
    DmxSetInst->DmxSet.DmxKeyInfo = DmxSetInst->DmxKeyInfo;\
    DmxSetInst->DmxSet.DmxKeyCnt = sizeof(DmxSetInst->DmxKeyInfo) / sizeof(DMX_KeyInfo_S);\
    mutex_init(&DmxSetInst->DmxSet.LockAllKey);\
    bitmap_zero(DmxSetInst->DmxSet.KeyBitmap, DmxSetInst->DmxSet.DmxKeyCnt); \
    DmxSetInst->DmxSet.DmxPcrInfo = DmxSetInst->DmxPcrInfo;\
    DmxSetInst->DmxSet.DmxPcrCnt = sizeof(DmxSetInst->DmxPcrInfo) / sizeof(DMX_PCR_Info_S);\
    mutex_init(&DmxSetInst->DmxSet.LockAllPcr);\
    bitmap_zero(DmxSetInst->DmxSet.PcrBitmap, DmxSetInst->DmxSet.DmxPcrCnt); \
    DmxSetInst->DmxSet.DmxFqInfo = DmxSetInst->DmxFqInfo;\
    DmxSetInst->DmxSet.DmxFqCnt = sizeof(DmxSetInst->DmxFqInfo) / sizeof(DMX_FQ_Info_S);\
    bitmap_zero(DmxSetInst->DmxSet.FqBitmap, DmxSetInst->DmxSet.DmxFqCnt); \
    DmxSetInst->DmxSet.DmxOqInfo = DmxSetInst->DmxOqInfo;\
    DmxSetInst->DmxSet.DmxOqCnt = sizeof(DmxSetInst->DmxOqInfo) / sizeof(DMX_OQ_Info_S);\
    bitmap_zero(DmxSetInst->DmxSet.OqBitmap, DmxSetInst->DmxSet.DmxOqCnt); \
    DmxSetInst->DmxSet.DmxScdInfo = DmxSetInst->DmxScdInfo;\
    DmxSetInst->DmxSet.DmxScdCnt = sizeof(DmxSetInst->DmxScdInfo) / sizeof(DMX_SCD_Info_S);\
    bitmap_zero(DmxSetInst->DmxSet.ScdBitmap, DmxSetInst->DmxSet.DmxScdCnt); \
    DmxSetInst->DmxSet.RmxChanInfo = DmxSetInst->RmxChanInfo;\
    DmxSetInst->DmxSet.RmxChanCnt = sizeof(DmxSetInst->RmxChanInfo) /sizeof(RMX_Chan_Info_S);\
    bitmap_zero(DmxSetInst->DmxSet.RmxChanBitmap, DmxSetInst->DmxSet.RmxChanCnt);\
    mutex_init(&DmxSetInst->DmxSet.LockAllRmx);\
    DmxSetInst->DmxSet.RmxPumpInfo = DmxSetInst->RmxPumpInfo;\
    DmxSetInst->DmxSet.RmxPumpCnt = sizeof(DmxSetInst->RmxPumpInfo) /sizeof(RMX_Pump_Info_S);\
    bitmap_zero(DmxSetInst->DmxSet.RmxPumpBitmap, DmxSetInst->DmxSet.RmxPumpCnt);\
    mutex_init(&DmxSetInst->DmxSet.LockAllPump);\
    DmxSetInst->DmxSet.SetId = ID; \
    DmxSetInst->DmxSet.IoBase = ioremap_nocache(ID##_BASE, 0x10000);\
    BUG_ON(!DmxSetInst->DmxSet.IoBase);\
    DmxSetInst->DmxSet.mmu.IoBase = DmxSetInst->DmxSet.IoBase;\
    DmxSetInst->DmxSet.mmu.CbTtbr = 0;\
    DmxSetInst->DmxSet.Irq = ID##_INT_NUM; \
    INIT_LIST_HEAD(&DmxSetInst->DmxSet.list);\
    DmxSetInst->DmxSet.Ops = &DmxSetOps; \
    ret = g_DmxClusterCtl->Ops->AddDmxSet(&DmxSetInst->DmxSet);\
    ret;\
})

/***********************************************************************************
* Function      : DMX_OsiInit
* Description   : Initialize demux module
* Input         :
* Output        :
* Return        : HI_SUCCESS:     success
*                 HI_FAILURE:     system error or allocated dma buffer size beyonds limit
* Others:
***********************************************************************************/
HI_S32 DMX_OsiInit(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_1_Cfg_S *DmxSet1 = HI_NULL;
#ifdef HI_DMX_SET_2
    Dmx_Set_2_Cfg_S *DmxSet2 = HI_NULL;
#endif

    /* avoid re-initialise drv */
    if(1 != atomic_inc_return(&g_DrvInitFlag))
    {
        HI_ERR_DEMUX("Demux drv has initliased.\n");
        ret = HI_FAILURE;
        goto out0;
    }

    g_DmxClusterCtl = HI_KZALLOC(HI_ID_DEMUX, sizeof(Dmx_Cluster_S), GFP_KERNEL);
    if (!g_DmxClusterCtl)
    {
        HI_ERR_DEMUX("allocate g_DmxClusterCtl failed\n");
        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out0;
    }

    /* initialise dmx grp global ctl */
    g_DmxClusterCtl->state = DMX_CLUSTER_INACTIVED;
    spin_lock_init(&g_DmxClusterCtl->lock);
    INIT_GBL_MMU_DEV((&g_DmxClusterCtl->mmu));
    mutex_init(&g_DmxClusterCtl->teec.lock);
    INIT_LIST_HEAD(&g_DmxClusterCtl->head);
    g_DmxClusterCtl->Ops = &DmxClusterOps;
    g_DmxClusterCtl->resumeCnt = 0;

    /* prepare dmxs */
    DmxSet1= HI_KMALLOC(HI_ID_DEMUX, sizeof(Dmx_Set_1_Cfg_S), GFP_KERNEL);
    if (!DmxSet1)
    {
        HI_ERR_DEMUX("allocate DmxSet1 failed\n");
        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out1;
    }

    memset(DmxSet1, 0 ,sizeof(Dmx_Set_1_Cfg_S));

    ret = ADD_DMX_SET(DMX_SET_1, DmxSet1);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("add DmxSet1 failed\n");
        goto out2;
    }

#ifdef HI_DMX_SET_2
    DmxSet2= HI_KMALLOC(HI_ID_DEMUX, sizeof(Dmx_Set_2_Cfg_S), GFP_KERNEL);
    if (!DmxSet2)
    {
        HI_ERR_DEMUX("allocate DmxSet2 failed\n");
        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out2;
    }

    memset(DmxSet2, 0 ,sizeof(Dmx_Set_2_Cfg_S));

    ret = ADD_DMX_SET(HI_DMX_SET_2, DmxSet2);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("add DmxSet2 failed\n");
        goto out3;
    }
#endif

    /* delay StartCluster() until open this device, refer to HI_DRV_DMX_Open. */

    return HI_SUCCESS;

#ifdef HI_DMX_SET_2
out3:
    HI_KFREE(HI_ID_DEMUX, DmxSet2);
    DmxSet2 = HI_NULL;
#endif
out2:
    HI_KFREE(HI_ID_DEMUX, DmxSet1);
    DmxSet1 = HI_NULL;
out1:
    UNIT_GBL_MMU_DEV((&g_DmxClusterCtl->mmu));

    HI_KFREE(HI_ID_DEMUX, g_DmxClusterCtl);
    g_DmxClusterCtl = HI_NULL;
out0:
    return ret;
}

/***********************************************************************************
* Function      : DMX_OsiDeInit
* Description   : destroy demux module
* Input         :
* Output        :
* Return        : HI_SUCCESS:     success
*                 HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiDeInit(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;

    /* check drv status firstly */
    if (0 == atomic_read(&g_DrvInitFlag))
    {
        HI_ERR_DEMUX("Demux drv has not initliased.\n");
        goto out;
    }

    ret = g_DmxClusterCtl->Ops->StopCluster();
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("stop All Demux failed\n");
    }

    /* del all demux resource */
    ret = g_DmxClusterCtl->Ops->GetDmxSet(DMX_SET_1, &DmxSet);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("get dmx failed\n");
        goto out;
    }

    ret = g_DmxClusterCtl->Ops->DelDmxSet(DMX_SET_1);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("del dmx failed\n");
        goto out;
    }

    HI_KFREE(HI_ID_DEMUX, DmxSet);

#ifdef HI_DMX_SET_2
    ret = g_DmxClusterCtl->Ops->GetDmxSet(HI_DMX_SET_2, &DmxSet);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("get dmx failed\n");
        goto out;
    }

    ret = g_DmxClusterCtl->Ops->DelDmxSet(HI_DMX_SET_2);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("del dmx failed\n");
        goto out;
    }

    HI_KFREE(HI_ID_DEMUX, DmxSet);
#endif

    /* release demux cluster */
    UNIT_GBL_MMU_DEV((&g_DmxClusterCtl->mmu));

    HI_KFREE(HI_ID_DEMUX, g_DmxClusterCtl);
    g_DmxClusterCtl = HI_NULL;

    atomic_set(&g_DrvInitFlag, 0);

    ret = HI_SUCCESS;

out:
    return ret;
}

HI_S32 DMX_OsiGetPoolBufAddr(DMX_MMZ_BUF_S *PoolBuf)
{
    Dmx_Set_S *DmxSet = GetDefDmxSet();
    DMX_FQ_Info_S  *FqInfo = &DmxSet->DmxFqInfo[DMX_FQ_COMMOM];

    PoolBuf->PhyAddr = FqInfo->BufPhyAddr;
    PoolBuf->Size = FqInfo->BufSize;
    PoolBuf->Flag = FqInfo->BufFlag;

    return HI_SUCCESS;
}

HI_S32 DMX_OsiGetCapability(HI_UNF_DMX_CAPABILITY_S *Cap)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();

    Cap->u32IFPortNum         = DmxCluster->Ops->GetIFPortNum();
    Cap->u32TSIPortNum       = DmxCluster->Ops->GetTSIPortNum();
    Cap->u32TSOPortNum      = DmxCluster->Ops->GetTsoPortNum();
    Cap->u32RamPortNum      = DmxCluster->Ops->GetRamPortNum();
    Cap->u32DmxNum           = DmxCluster->Ops->GetDmxNum();
    Cap->u32ChannelNum      = DmxCluster->Ops->GetChnNum();
    Cap->u32AVChannelNum   = DmxCluster->Ops->GetAVChnNum();
    Cap->u32FilterNum          = DmxCluster->Ops->GetFilterNum();
    Cap->u32KeyNum            = DmxCluster->Ops->GetKeyNum();
    Cap->u32RecChnNum       = DmxCluster->Ops->GetRecNum();
    Cap->u32TagPortNum       = DmxCluster->Ops->GetTagPortNum();
    // Cap->u32RmxNum             = DmxCluster->Ops->GetRmxNum();

    return HI_SUCCESS;
}

HI_VOID DMX_OsiSetNoPusiEn(HI_BOOL bNoPusiEn)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        DmxHalFilterSetSecStuffCtrl(DmxSet, bNoPusiEn);
    }
}

HI_VOID DMX_OsiSetTei(HI_U32 u32DmxId,HI_BOOL bTei)
{
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(u32DmxId);

    if (DmxSet)
    {
        DmxHalSetTei(DmxSet, u32DmxId,bTei);
    }
}

HI_S32  DMX_OsiGetResumeCount(HI_U32 *pCount)
{
    *pCount = GetDmxCluster()->resumeCnt;

    return HI_SUCCESS;
}

static HI_S32 __DMX_OsiAttachPort(const HI_U32 DmxId, const DMX_PORT_MODE_E PortMode, const HI_U32 PortId)
{
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_DmxInfo_S *DmxInfo = DmxSet->Ops->GetDmxInfo(DmxSet, DmxId);
    HI_U32 ChanId;

    DmxInfo->DmxId     = DmxId;
    DmxInfo->PortId     = PortId;  /* logical id */
    DmxInfo->PortMode   = PortMode;

    /*
    * data will be into demux immediately after DmxHalSetDemuxPortId succeed.
    * tune clk in advance.
    */
    DMXDynamicTuneDmxClk();

    DmxHalSetDemuxPortId(DmxSet, DmxId, PortMode, PortId);

    mutex_lock(&DmxSet->LockAllChn);

    for_each_set_bit(ChanId, DmxSet->ChnBitmap, DmxSet->DmxChanCnt)
    {
        if (HI_SUCCESS == DmxGetChnInstance(DmxSet, ChanId))
        {
            DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

            if ((ChanInfo->DmxId == DmxId) &&
                    (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode) &&
                        (HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType ||HI_UNF_DMX_CHAN_TYPE_VID == ChanInfo->ChanType))
            {
                DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];

                switch(PortMode)
                {
                    case DMX_PORT_MODE_RAM:
                        DmxHalAttachIPBPFQ(DmxSet, PortId, OqInfo->u32FQId);
                        break;
                    case DMX_PORT_MODE_RMX:
                        RmxEnFqBPIp(DmxSet, PortId, OqInfo->u32FQId);
                        break;
                    default:
                        break;
                }
            }

            DmxPutChnInstance(DmxSet, ChanId);
        }
    }

    mutex_unlock(&DmxSet->LockAllChn);

    return HI_SUCCESS ;
}

HI_S32 DMX_OsiAttachPort(const HI_U32 DmxId, const DMX_PORT_MODE_E PortMode, const HI_U32 PortId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;

    DmxSet = GetDmxSetByDmxid(DmxId);
    if (!DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    /* test whether the port can be bind to the correspond dmx  */
    switch(PortMode)
    {
        case DMX_PORT_MODE_TUNER: /* tuner port can be bind to any demux */
        {
            Dmx_Cluster_S *DmxCluster = GetDmxCluster();

            if (PortId >= DmxCluster->Ops->GetIFPortNum() + DmxCluster->Ops->GetTSIPortNum())
            {
                HI_ERR_DEMUX("Tuner Port(%d) can't bound to dmx(%d).\n", PortId, DmxId);
                ret = HI_ERR_DMX_INVALID_PARA;
                goto out;
            }

            break;
        }

        case DMX_PORT_MODE_RAM: /* each dmx set has private ram port, which can't mix bind. */
        {
            Dmx_Set_S *TmpDmxSet = GetDmxSetByRamid(PortId);

            if (TmpDmxSet->SetId != DmxSet->SetId)
            {
                HI_ERR_DEMUX("Ram Port(%d) can't bound to dmx(%d).\n", PortId, DmxId);
                ret = HI_ERR_DMX_INVALID_PARA;
                goto out;
            }

            break;
        }

        case DMX_PORT_MODE_TAG:
        {
            /* FIXME: second demux set maybe has no tag port */
            break;
        }

        case DMX_PORT_MODE_RMX:
        {
            Dmx_Cluster_S *DmxCluster = GetDmxCluster();

            if (PortId >= DmxCluster->Ops->GetRmxNum())
            {
                HI_ERR_DEMUX("invalid remux id(%d).\n", PortId);
                ret = HI_ERR_DMX_INVALID_PARA;
                goto out;
            }
            break;
        }

        default:
        {
            WARN(DMX_INVALID_PORT_ID != PortId, "PortMod(%d) is invalid, but PortId(%d) is valid.\n", PortMode, PortId);
            ret = HI_ERR_DMX_INVALID_PARA;
            goto out;
        }
    }

    ret = __DMX_OsiAttachPort(DmxId, PortMode, PortId);

out:
    return ret ;
}

HI_S32 DMX_OsiDetachPort(const HI_U32 DmxId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_DmxInfo_S *DmxInfo = HI_NULL;
    HI_U32 ChanId;

    DmxSet = GetDmxSetByDmxid(DmxId);
    if (!DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    DmxInfo = DmxSet->Ops->GetDmxInfo(DmxSet, DmxId);

    mutex_lock(&DmxSet->LockAllChn);

    for_each_set_bit(ChanId, DmxSet->ChnBitmap, DmxSet->DmxChanCnt)
    {
        if (HI_SUCCESS == DmxGetChnInstance(DmxSet, ChanId))
        {
            DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

            if ((ChanInfo->DmxId == DmxId) &&
                    (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode) &&
                        (HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType ||HI_UNF_DMX_CHAN_TYPE_VID == ChanInfo->ChanType))
            {
                DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];

                switch(DmxInfo->PortMode)
                {
                    case DMX_PORT_MODE_RAM:
                        DmxHalDetachIPBPFQ(DmxSet, DmxInfo->PortId, OqInfo->u32FQId);
                        DmxHalClrIPFqBPStatus(DmxSet, DmxInfo->PortId, OqInfo->u32FQId);
                        break;
                    case DMX_PORT_MODE_RMX:
                        RmxDisFqBPIp(DmxSet, DmxInfo->PortId, OqInfo->u32FQId);
                        break;
                    default:
                        break;
                }
            }

            DmxPutChnInstance(DmxSet, ChanId);
        }
    }

    mutex_unlock(&DmxSet->LockAllChn);

    DmxInfo->DmxId     = DMX_INVALID_DEMUX_ID;
    DmxInfo->PortId     = DMX_INVALID_PORT_ID;
    DmxInfo->PortMode   = DMX_PORT_MODE_BUTT;

    DmxHalSetDemuxPortId(DmxSet, DmxId, DMX_PORT_MODE_BUTT, DMX_INVALID_PORT_ID);

    DMXDynamicTuneDmxClk();

    ret = HI_SUCCESS;

out:
    return ret;
}

HI_S32 DMX_OsiGetPortId(const HI_U32 DmxId, DMX_PORT_MODE_E *PortMode, HI_U32 *PortId)
{
    HI_S32 ret = HI_ERR_DMX_NOATTACH_PORT;
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_DmxInfo_S *DmxInfo = HI_NULL;

    DmxSet = GetDmxSetByDmxid(DmxId);
    if (!DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    DmxInfo = DmxSet->Ops->GetDmxInfo(DmxSet, DmxId);

    if (DMX_INVALID_PORT_ID != DmxInfo->PortId)
    {
        *PortMode   = DmxInfo->PortMode;
        *PortId     = DmxInfo->PortId;

        ret = HI_SUCCESS;
    }
    else
    {
        HI_WARN_DEMUX("the demux not attach with any port, DmxId=%d.\n", DmxId);
    }

out:
    return ret;
}

HI_S32 DMX_OsiTSOPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_TSO_PORT_ATTR_S *PortAttr)
{
    Dmx_Set_S *DmxSet = GetDefDmxSet();
    HI_UNF_DMX_TSO_PORT_ATTR_S *PortInfo = &DmxSet->TSOPortInfo[PortId];

    PortAttr->bEnable         = PortInfo->bEnable;
    PortAttr->bClkReverse     = PortInfo->bClkReverse;
    PortAttr->enTSSource      = PortInfo->enTSSource;
    PortAttr->enClkMode       = PortInfo->enClkMode;
    PortAttr->enValidMode     = PortInfo->enValidMode;
    PortAttr->bBitSync        = PortInfo->bBitSync;
    PortAttr->bSerial         = PortInfo->bSerial;
    PortAttr->enBitSelector   = PortInfo->enBitSelector;
    PortAttr->bLSB            = PortInfo->bLSB;
    PortAttr->enClk           = PortInfo->enClk;
    PortAttr->u32ClkDiv       = PortInfo->u32ClkDiv;

    return HI_SUCCESS;
}

HI_S32 DMX_OsiTSOPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_TSO_PORT_ATTR_S *PortAttr)
{
    Dmx_Set_S *DmxSet = GetDefDmxSet();
    HI_UNF_DMX_TSO_PORT_ATTR_S *PortInfo = &DmxSet->TSOPortInfo[PortId];

    if (PortId >= DmxSet->TSOPortCnt)
    {
        HI_ERR_DEMUX("PortId(0x%x) invalid\n", PortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    if ((PortAttr->enClkMode >=  HI_UNF_DMX_TSO_CLK_MODE_BUTT) ||
        (PortAttr->enValidMode >=  HI_UNF_DMX_TSO_VALID_ACTIVE_BUTT ) ||
        (PortAttr->enClk >=  HI_UNF_DMX_TSO_CLK_BUTT ) )
    {
        HI_ERR_DEMUX("enClkMode or enValidMode or enClk is invalid\n");
        return HI_ERR_DMX_INVALID_PARA;
    }

    if ( (PortAttr->u32ClkDiv < 2) || (PortAttr->u32ClkDiv > 32) || (PortAttr->u32ClkDiv%2 != 0 ) )
    {
        HI_ERR_DEMUX("u32ClkDiv = %d is invalid\n",PortAttr->u32ClkDiv );
        return HI_ERR_DMX_INVALID_PARA;
    }

    if ( (PortAttr->enClkMode == HI_UNF_DMX_TSO_CLK_MODE_NORMAL) && (PortAttr->enValidMode == HI_UNF_DMX_TSO_VALID_ACTIVE_HIGH) )
    {
            HI_ERR_DEMUX("invalid tso port attr , can not config  (enClkMode == HI_UNF_DMX_TSO_CLK_MODE_NORMAL) \
                while (enValidMode == HI_UNF_DMX_TSO_VALID_ACTIVE_HIGH)\n");
            return HI_ERR_DMX_INVALID_PARA;
    }

    if ( PortAttr->enTSSource < HI_UNF_DMX_PORT_TSI_0 )
    {
        Dmx_Cluster_S *DmxCluster = GetDmxCluster();

        if ( 0 == DmxCluster->Ops->GetIFPortNum())
        {
            HI_ERR_DEMUX("this chipset have no IF port ,so can not choose enTSSource as IF port\n");
            return HI_ERR_DMX_INVALID_PARA;
        }
        else if ( PortAttr->enTSSource - HI_UNF_DMX_PORT_IF_0 >= DmxCluster->Ops->GetIFPortNum() )
        {
            HI_ERR_DEMUX("if enTSSource is HI_UNF_DMX_PORT_IF_x , must less than 0x%x\n", DmxCluster->Ops->GetIFPortNum());
            return HI_ERR_DMX_INVALID_PARA;
        }
    }
    else if ( (PortAttr->enTSSource >= HI_UNF_DMX_PORT_TSI_0) && (PortAttr->enTSSource <= HI_UNF_DMX_PORT_TSI_7)  )
    {
        if ( PortAttr->enTSSource - HI_UNF_DMX_PORT_TSI_0 >= DMX_TSIPORT_CNT )
        {
            HI_ERR_DEMUX("if enTSSource is HI_UNF_DMX_PORT_TS_x , must less than 0x%x\n",HI_UNF_DMX_PORT_TSI_0 + DMX_TSIPORT_CNT);
            return HI_ERR_DMX_INVALID_PARA;
        }
    }
    else if ( (PortAttr->enTSSource >= HI_UNF_DMX_PORT_RAM_0) && (PortAttr->enTSSource <= HI_UNF_DMX_PORT_RAM_7) )
    {
        if ( PortAttr->enTSSource - HI_UNF_DMX_PORT_RAM_0 >= DMX_RAMPORT_CNT )
        {
            HI_ERR_DEMUX("if enTSSource is HI_UNF_DMX_PORT_RAM_x , must less than 0x%x\n",HI_UNF_DMX_PORT_RAM_0 + DMX_RAMPORT_CNT);
            return HI_ERR_DMX_INVALID_PARA;
        }
    }
    else
    {
        HI_ERR_DEMUX("enTSSource is invalid\n");
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo->bEnable         = PortAttr->bEnable;
    PortInfo->bClkReverse     = PortAttr->bClkReverse;
    PortInfo->enTSSource      = PortAttr->enTSSource;
    PortInfo->enClkMode       = PortAttr->enClkMode;
    PortInfo->enValidMode     = PortAttr->enValidMode;
    PortInfo->bBitSync        = PortAttr->bBitSync;
    PortInfo->bSerial         = PortAttr->bSerial;
    PortInfo->enBitSelector   = PortAttr->enBitSelector;
    PortInfo->bLSB            = PortAttr->bLSB;
    PortInfo->enClk           = PortAttr->enClk;
    PortInfo->u32ClkDiv       = PortAttr->u32ClkDiv;

    DmxHalTSOPortSetAttr(DmxSet, PortId, PortInfo);
    DmxHalCfgTSOClk(PortId,PortInfo->bClkReverse,(HI_U32)PortInfo->enClk,PortInfo->u32ClkDiv);

    if ( PortInfo->enTSSource >=  HI_UNF_DMX_PORT_RAM_0)
    {
        DMXConfigIPPortRate(DmxSet, PortInfo->enTSSource - HI_UNF_DMX_PORT_RAM_0);
    }

    return HI_SUCCESS;
}

HI_S32 __DMX_OsiTunerPortGetAttr(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, HI_UNF_DMX_PORT_ATTR_S *PortAttr)
{
    DMX_TunerPort_Info_S *PortInfo = HI_NULL;

    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetTunerPortInfoByRawId(DmxSet, RawPortId);
    if (!PortInfo)
    {
        HI_ERR_DEMUX("Invalid Tuner Raw Port Id(%d).\n", RawPortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortAttr->enPortMod             = (RawPortId < GetDmxCluster()->Ops->GetIFPortNum())?HI_UNF_DMX_PORT_MODE_INTERNAL:HI_UNF_DMX_PORT_MODE_EXTERNAL;
    PortAttr->enPortType            = PortInfo->PortType;
    PortAttr->u32SyncLostTh         = PortInfo->SyncLostTh;
    PortAttr->u32SyncLockTh         = PortInfo->SyncLockTh;
    PortAttr->u32TunerInClk         = PortInfo->TunerInClk;
    PortAttr->u32SerialBitSelector  = PortInfo->BitSelector;
    PortAttr->u32TunerErrMod        = 0;
    PortAttr->u32UserDefLen1        = 0;
    PortAttr->u32UserDefLen2        = 0;

    /* get share clock port configure, becase RawPortId=PortIndex - HI_UNF_DMX_PORT_TSI_0+GetDmxCluster()->Ops->GetIFPortNum()
           so here we need substract the IFPortNum */
    if (HI_SUCCESS != DmxHalDvbPortGetShareClk(RawPortId - GetDmxCluster()->Ops->GetIFPortNum(), &PortAttr->enSerialPortShareClk))
    {
        HI_ERR_DEMUX("Get share clock port error.\n");
    }

    return HI_SUCCESS;
}

HI_S32 DMX_OsiTunerPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_PORT_ATTR_S *PortAttr)
{
    Dmx_Set_S *DmxSet = HI_NULL;
    HI_U32 RawPortId;

    DmxSet = GetDefDmxSet();
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    RawPortId = DmxSet->Ops->GetTunerRawId(DmxSet, PortId);
    if (DMX_INVALID_PORT_ID == RawPortId)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    return __DMX_OsiTunerPortGetAttr(DmxSet, RawPortId, PortAttr);
}

HI_S32 __DMX_OsiTunerPortSetAttr(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_UNF_DMX_PORT_ATTR_S *PortAttr)
{
    DMX_TunerPort_Info_S *PortInfo = HI_NULL;

    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetTunerPortInfoByRawId(DmxSet, RawPortId);
    if (!PortInfo)
    {
        HI_ERR_DEMUX("Invalid Tuner Raw Port Id(%d).\n", RawPortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    switch (PortAttr->enPortType)
    {
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_BURST :
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_VALID :
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188 :
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_204 :
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188_204 :
        case HI_UNF_DMX_PORT_TYPE_SERIAL :
            break;

        case HI_UNF_DMX_PORT_TYPE_SERIAL2BIT :
        case HI_UNF_DMX_PORT_TYPE_SERIAL_NOSYNC :
        case HI_UNF_DMX_PORT_TYPE_SERIAL2BIT_NOSYNC :
#ifdef DMX_TUNER_PORT_SERIAL_2BIT_AND_SERIAL_NOSYNC_SUPPORT
            break;
#else
            return HI_ERR_DMX_NOT_SUPPORT;
#endif

        case HI_UNF_DMX_PORT_TYPE_USER_DEFINED :
        case HI_UNF_DMX_PORT_TYPE_AUTO :
            return HI_ERR_DMX_NOT_SUPPORT;

        default :
            HI_ERR_DEMUX("Port %u set invalid port type %d\n", RawPortId, PortAttr->enPortType);

            return HI_ERR_DMX_INVALID_PARA;
    }

    if (PortAttr->u32TunerInClk > 1)
    {
        HI_ERR_DEMUX("Port %u set invalid tunner in clock %d\n", RawPortId, PortAttr->u32TunerInClk);

        return HI_ERR_DMX_INVALID_PARA;
    }

    if (PortAttr->u32SerialBitSelector > 1)
    {
        HI_ERR_DEMUX("Port %u set invalid Serial Bit Selector %d\n", RawPortId, PortAttr->u32SerialBitSelector);

        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo->PortType      = PortAttr->enPortType;
    PortInfo->SyncLockTh    = PortAttr->u32SyncLockTh > DMX_MAX_LOCK_TH ? DMX_MAX_LOCK_TH : PortAttr->u32SyncLockTh;
    PortInfo->SyncLostTh    = PortAttr->u32SyncLostTh > DMX_MAX_LOST_TH ? DMX_MAX_LOST_TH : PortAttr->u32SyncLostTh;
    PortInfo->BitSelector   = PortAttr->u32SerialBitSelector;

    /* set share clock port configure, becase RawPortId=PortIndex - HI_UNF_DMX_PORT_TSI_0+GetDmxCluster()->Ops->GetIFPortNum()
           so here we need substract the IFPortNum */
    DmxHalDvbPortSetShareClk(RawPortId - GetDmxCluster()->Ops->GetIFPortNum(), PortAttr->enSerialPortShareClk);

    DmxHalDvbPortSetAttr(DmxSet, RawPortId, PortInfo->PortType, PortInfo->SyncLockTh,
            PortInfo->SyncLostTh, PortInfo->TunerInClk, PortInfo->BitSelector);

    if (RawPortId >= GetDmxCluster()->Ops->GetIFPortNum())
    {
        PortInfo->TunerInClk = PortAttr->u32TunerInClk;

        DmxHalDvbPortSetClkInPol(RawPortId, PortInfo->TunerInClk);
    }

    return HI_SUCCESS;
}

HI_S32 DMX_OsiTunerPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_PORT_ATTR_S *PortAttr)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        HI_U32 RawPortId = DmxSet->Ops->GetTunerRawId(DmxSet, PortId);

        if (DMX_INVALID_PORT_ID == RawPortId)
        {
            ret = HI_ERR_DMX_INVALID_PARA;
            continue;
        }

        ret = __DMX_OsiTunerPortSetAttr(DmxSet, RawPortId, PortAttr);
        if (HI_SUCCESS != ret)
        {
            break;
        }
    }

    return ret;
}

HI_S32 __DMX_OsiRamPortGetAttr(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, HI_UNF_DMX_PORT_ATTR_S *PortAttr)
{
    DMX_RamPort_Info_S *PortInfo = HI_NULL;

    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfoByRawId(DmxSet, RawPortId);
    if (!PortInfo)
    {
        HI_ERR_DEMUX("Invalid Ram Raw Port Id(%d).\n", RawPortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortAttr->enPortMod             = HI_UNF_DMX_PORT_MODE_RAM;
    PortAttr->enPortType            = PortInfo->PortType;
    PortAttr->u32SyncLostTh         = PortInfo->SyncLostTh;
    PortAttr->u32SyncLockTh         = PortInfo->SyncLockTh;
    PortAttr->u32TunerInClk         = 0;
    PortAttr->u32SerialBitSelector  = 0;
    PortAttr->u32TunerErrMod        = 0;
    PortAttr->u32UserDefLen1        = PortInfo->MinLen;
    PortAttr->u32UserDefLen2        = PortInfo->MaxLen;
    PortAttr->enSerialPortShareClk  = HI_UNF_DMX_PORT_TYPE_BUTT;

    return HI_SUCCESS;
}

HI_S32 DMX_OsiRamPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_PORT_ATTR_S *PortAttr)
{
    Dmx_Set_S *DmxSet = HI_NULL;
    HI_U32 RawPortId ;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    RawPortId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);
    if (DMX_INVALID_PORT_ID == RawPortId)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    return __DMX_OsiRamPortGetAttr(DmxSet, RawPortId, PortAttr);
}

HI_S32 __DMX_OsiRamPortSetAttr(Dmx_Set_S *DmxSet, const HI_U32 RawPortId, const HI_UNF_DMX_PORT_ATTR_S *PortAttr)
{
    DMX_RamPort_Info_S *PortInfo = HI_NULL;

    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfoByRawId(DmxSet, RawPortId);
    if (!PortInfo)
    {
        HI_ERR_DEMUX("Invalid Ram Raw Port Id(%d).\n", RawPortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    switch (PortAttr->enPortType)
    {
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188 :
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_204 :
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188_204 :
            PortInfo->MinLen = 0;
            PortInfo->MaxLen = 0;

            break;

        case HI_UNF_DMX_PORT_TYPE_AUTO :
#ifdef DMX_RAM_PORT_AUTO_SCAN_SUPPORT
            PortInfo->MinLen = 0;
            PortInfo->MaxLen = 0;

            break;
#else
            HI_ERR_DEMUX("do not support auto scan!\n");
            return HI_ERR_DMX_NOT_SUPPORT;
#endif

        case HI_UNF_DMX_PORT_TYPE_USER_DEFINED :
#ifdef DMX_RAM_PORT_SET_LENGTH_SUPPORT
            if (   (PortAttr->u32UserDefLen1 < DMX_RAM_PORT_MIN_LEN)
                || (PortAttr->u32UserDefLen1 > DMX_RAM_PORT_MAX_LEN)
                || (PortAttr->u32UserDefLen2 < DMX_RAM_PORT_MIN_LEN)
                || (PortAttr->u32UserDefLen2 > DMX_RAM_PORT_MAX_LEN) )
            {
                HI_ERR_DEMUX("set len error. len1=%u, len2=%u\n", PortAttr->u32UserDefLen1, PortAttr->u32UserDefLen2);

                return HI_ERR_DMX_INVALID_PARA;
            }

            PortInfo->MinLen = PortAttr->u32UserDefLen1;
            PortInfo->MaxLen = PortAttr->u32UserDefLen2;

            break;
#else
            return HI_ERR_DMX_NOT_SUPPORT;
#endif

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_BURST :
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_VALID :
        case HI_UNF_DMX_PORT_TYPE_SERIAL :
        case HI_UNF_DMX_PORT_TYPE_SERIAL2BIT :
        case HI_UNF_DMX_PORT_TYPE_SERIAL_NOSYNC :
        case HI_UNF_DMX_PORT_TYPE_SERIAL2BIT_NOSYNC :
            return HI_ERR_DMX_NOT_SUPPORT;

        default :
            HI_ERR_DEMUX("Invalid type %u\n", PortAttr->enPortType);

            return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo->PortType      = PortAttr->enPortType;
    PortInfo->SyncLockTh    = PortAttr->u32SyncLockTh > DMX_MAX_LOCK_TH ? DMX_MAX_LOCK_TH : PortAttr->u32SyncLockTh;
    PortInfo->SyncLostTh    = PortAttr->u32SyncLostTh > DMX_MAX_LOST_TH ? DMX_MAX_LOST_TH : PortAttr->u32SyncLostTh;

    DmxHalIPPortSetAttr(DmxSet, RawPortId, PortInfo->PortType, PortInfo->SyncLockTh, PortInfo->SyncLostTh);

#ifdef DMX_RAM_PORT_SET_LENGTH_SUPPORT
    if (HI_UNF_DMX_PORT_TYPE_USER_DEFINED == PortAttr->enPortType)
    {
        DmxHalIPPortSetSyncLen(DmxSet, RawPortId, PortInfo->MinLen, PortInfo->MaxLen);
    }
    else
    {
        DmxHalIPPortSetSyncLen(DmxSet, RawPortId, DMX_TS_PACKET_LEN, DMX_TS_PACKET_LEN_204);
    }
#endif

#ifdef DMX_RAM_PORT_AUTO_SCAN_SUPPORT
    if (HI_UNF_DMX_PORT_TYPE_AUTO == PortAttr->enPortType)
    {
        DmxHalIPPortSetAutoScanRegion(DmxSet, RawPortId, DMX_RAM_PORT_AUTO_REGION, DMX_RAM_PORT_AUTO_STEP_4);
    }
#endif

    return HI_SUCCESS;
}

HI_S32 DMX_OsiRamPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_PORT_ATTR_S *PortAttr)
{
    Dmx_Set_S *DmxSet = HI_NULL;
    HI_U32 RawPortId = DMX_INVALID_PORT_ID;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    RawPortId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);
    if (DMX_INVALID_PORT_ID == RawPortId)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    return __DMX_OsiRamPortSetAttr(DmxSet, RawPortId, PortAttr);
}

HI_S32 DMX_OsiTunerPortGetPacketNum(const HI_U32 PortId, HI_U32 *TsPackCnt, HI_U32 *ErrTsPackCnt)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;
    HI_U32 RawPortId = DMX_INVALID_PORT_ID;

    *TsPackCnt = 0;
    *ErrTsPackCnt = 0;

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        RawPortId = DmxSet->Ops->GetTunerRawId(DmxSet, PortId);
        if (DMX_INVALID_PORT_ID != RawPortId)
        {
            break;
        }
    }

    if (DMX_INVALID_PORT_ID == RawPortId)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        RawPortId = DmxSet->Ops->GetTunerRawId(DmxSet, PortId);
        if (DMX_INVALID_PORT_ID != RawPortId)
        {
            *TsPackCnt += DmxHalDvbPortGetTsPackCount(DmxSet, RawPortId);
            *ErrTsPackCnt += DmxHalDvbPortGetErrTsPackCount(DmxSet, RawPortId);
        }
    }

    ret = HI_SUCCESS;

out:
    return ret;
}

HI_S32 DMX_OsiRamPortGetPacketNum(const HI_U32 PortId, HI_U32 *TsPackCnt)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = HI_NULL;

    DmxSet = GetDmxSetByRamid(PortId);
    if (DmxSet)
    {
        *TsPackCnt = DmxHalIPPortGetTsPackCount(DmxSet, PortId);
        ret = HI_SUCCESS;
    }

    return ret;
}

#ifdef DMX_MMU_SUPPORT
static HI_S32 DmxAllocSecureTsBuf(Dmx_Set_S *DmxSet, DMX_RamPort_Info_S *PortInfo)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 RawPortId = PortInfo - DmxSet->RamPortInfo;
    HI_U32 DescDepth, DescBufSize;
    HI_CHAR BufName[32];
    SMMU_BUFFER_S SecMmuBuf = {0};
    MMZ_BUFFER_S MmzBuf = {0};

    snprintf(BufName, sizeof(BufName), "DMX_TsBuf[%d]", RawPortId);

    ret = HI_DRV_SECSMMU_Alloc(BufName, PortInfo->BufSize, 4, &SecMmuBuf);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("mmu secure memory allocate failed, BufSize=0x%x\n", PortInfo->BufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out0;
    }

    BUG_ON(SecMmuBuf.u32Size != PortInfo->BufSize);

    /* SecMmuBuf.pu8StartVirAddr is NULL, replace with SecMmuBuf.u32StartSmmuAddr. */
    PortInfo->KerAddr       = (HI_U8*)(unsigned long)SecMmuBuf.u32StartSmmuAddr;;
    PortInfo->PhyAddr       = SecMmuBuf.u32StartSmmuAddr;
    PortInfo->BufFlag       = DMX_SECURE_BUF;

    DescDepth = (PortInfo->BufSize / 0x100000) * 0x3ff;
    if (DescDepth < DMX_MIN_IP_DESC_DEPTH)
    {
        DescDepth = DMX_MIN_IP_DESC_DEPTH;
    }
    else if (DescDepth > DMX_MAX_IP_DESC_DEPTH)
    {
        DescDepth = DMX_MAX_IP_DESC_DEPTH;
    }

    DescBufSize = (DescDepth + 1) * DMX_PER_DESC_LEN;

    snprintf(BufName, sizeof(BufName), "DMX_TsDscBuf[%d]", RawPortId);
    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(BufName, HI_NULL, DescBufSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("malloc 0x%x failed\n", DescBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out1;
    }

    PortInfo->DescKerAddr   = MmzBuf.pu8StartVirAddr;
    PortInfo->DescPhyAddr   = MmzBuf.u32StartPhyAddr;
    PortInfo->DescCurrAddr  = (HI_U32*)PortInfo->DescKerAddr;
    PortInfo->DescDepth     = DescDepth;
    PortInfo->DescWrite     = 0;

    memset((HI_VOID*)PortInfo->DescKerAddr, 0, DescBufSize);

    return HI_SUCCESS;

out1:
    HI_DRV_SECSMMU_Release(&SecMmuBuf);
out0:
    return ret;
}

static HI_VOID DmxFreeSecureTsBuf(Dmx_Set_S *DmxSet, DMX_RamPort_Info_S *PortInfo)
{
    SMMU_BUFFER_S SecMmuBuf = {0};
    MMZ_BUFFER_S MmzBuf = {0};

    MmzBuf.pu8StartVirAddr = PortInfo->DescKerAddr;
    MmzBuf.u32StartPhyAddr = PortInfo->DescPhyAddr ;

    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);

    SecMmuBuf.pu8StartVirAddr = (HI_U8*)HI_NULL;
    SecMmuBuf.u32StartSmmuAddr = PortInfo->PhyAddr;

    HI_DRV_SECSMMU_Release(&SecMmuBuf);
}

static HI_S32 DmxAllocNonSecureTsBuf(Dmx_Set_S *DmxSet, DMX_RamPort_Info_S *PortInfo)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 RawPortId = PortInfo - DmxSet->RamPortInfo;
    HI_U32 DescDepth, DescBufSize;
    HI_CHAR BufName[32];
    SMMU_BUFFER_S MmuBuf = {0};
    MMZ_BUFFER_S MmzBuf = {0};

    snprintf(BufName, sizeof(BufName), "DMX_TsBuf[%d]", RawPortId);
    if (HI_SUCCESS != HI_DRV_SMMU_AllocAndMap(BufName, PortInfo->BufSize, 0, &MmuBuf))
    {
        HI_ERR_DEMUX("mmu malloc 0x%x failed\n", PortInfo->BufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out0;
    }

    PortInfo->KerAddr       = MmuBuf.pu8StartVirAddr;
    PortInfo->PhyAddr       = MmuBuf.u32StartSmmuAddr;
    PortInfo->BufFlag       = DMX_MMU_BUF;

    DescDepth = (PortInfo->BufSize / 0x100000) * 0x3ff;
    if (DescDepth < DMX_MIN_IP_DESC_DEPTH)
    {
        DescDepth = DMX_MIN_IP_DESC_DEPTH;
    }
    else if (DescDepth > DMX_MAX_IP_DESC_DEPTH)
    {
        DescDepth = DMX_MAX_IP_DESC_DEPTH;
    }

    DescBufSize = (DescDepth + 1) * DMX_PER_DESC_LEN;

    snprintf(BufName, sizeof(BufName), "DMX_TsDscBuf[%d]", RawPortId);
    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(BufName, HI_NULL, DescBufSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("malloc 0x%x failed\n", DescBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out1;
    }

    PortInfo->DescKerAddr   = MmzBuf.pu8StartVirAddr;
    PortInfo->DescPhyAddr   = MmzBuf.u32StartPhyAddr;
    PortInfo->DescCurrAddr  = (HI_U32*)PortInfo->DescKerAddr;
    PortInfo->DescDepth     = DescDepth;
    PortInfo->DescWrite     = 0;

    memset((HI_VOID*)PortInfo->DescKerAddr, 0, DescBufSize);

    return HI_SUCCESS;

out1:
    MmuBuf.pu8StartVirAddr = PortInfo->KerAddr  ;
    MmuBuf.u32StartSmmuAddr = PortInfo->PhyAddr;

    HI_DRV_SMMU_UnmapAndRelease(&MmuBuf);
out0:
    return ret;
}

static HI_VOID DmxFreeNonSecureTsBuf(Dmx_Set_S *DmxSet, DMX_RamPort_Info_S *PortInfo)
{
    SMMU_BUFFER_S MmuBuf = {0};
    MMZ_BUFFER_S MmzBuf = {0};

    MmzBuf.pu8StartVirAddr = PortInfo->DescKerAddr;
    MmzBuf.u32StartPhyAddr = PortInfo->DescPhyAddr ;

    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);

    MmuBuf.pu8StartVirAddr = PortInfo->KerAddr;
    MmuBuf.u32StartSmmuAddr = PortInfo->PhyAddr;

    HI_DRV_SMMU_UnmapAndRelease(&MmuBuf);
}
#else
static HI_S32 DmxAllocSecureTsBuf(Dmx_Set_S *DmxSet, DMX_RamPort_Info_S *PortInfo)
{
    HI_ERR_DEMUX("not support alloc secure ts buf.\n");
    return HI_ERR_DMX_NOT_SUPPORT;
}

static HI_VOID DmxFreeSecureTsBuf(Dmx_Set_S *DmxSet, DMX_RamPort_Info_S *PortInfo)
{
}

static HI_S32 DmxAllocNonSecureTsBuf(Dmx_Set_S *DmxSet, DMX_RamPort_Info_S *PortInfo)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 RawPortId = PortInfo - DmxSet->RamPortInfo;
    HI_U32 DescDepth, DescBufSize;
    HI_CHAR BufName[32];
    MMZ_BUFFER_S MmzBuf = {0};

    DescDepth = (PortInfo->BufSize / 0x100000) * 0x3ff;
    if (DescDepth < DMX_MIN_IP_DESC_DEPTH)
    {
        DescDepth = DMX_MIN_IP_DESC_DEPTH;
    }
    else if (DescDepth > DMX_MAX_IP_DESC_DEPTH)
    {
        DescDepth = DMX_MAX_IP_DESC_DEPTH;
    }

    DescBufSize = (DescDepth + 1) * DMX_PER_DESC_LEN;

    snprintf(BufName, sizeof(BufName), "DMX_TsBuf[%d]", RawPortId);
    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(BufName, HI_NULL, PortInfo->BufSize + DescBufSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("malloc 0x%x failed\n", PortInfo->BufSize + DescBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out0;
    }

    PortInfo->KerAddr       = MmzBuf.pu8StartVirAddr;
    PortInfo->PhyAddr       = MmzBuf.u32StartPhyAddr;
    PortInfo->BufFlag        = DMX_MMZ_BUF;

    PortInfo->DescKerAddr   = PortInfo->KerAddr + PortInfo->BufSize;
    PortInfo->DescPhyAddr   = PortInfo->PhyAddr + PortInfo->BufSize;
    PortInfo->DescCurrAddr  = (HI_U32*)PortInfo->DescKerAddr;
    PortInfo->DescDepth     = DescDepth;
    PortInfo->DescWrite     = 0;

    memset((HI_VOID*)PortInfo->DescKerAddr, 0, DescBufSize);

    return HI_SUCCESS;

out0:
    return ret;
}

static HI_VOID DmxFreeNonSecureTsBuf(Dmx_Set_S *DmxSet, DMX_RamPort_Info_S *PortInfo)
{
    MMZ_BUFFER_S MmzBuf = {0};

    MmzBuf.pu8StartVirAddr = PortInfo->KerAddr;
    MmzBuf.u32StartPhyAddr = PortInfo->PhyAddr;

    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);
}
#endif

static HI_S32 DmxAllocTsBuf(Dmx_Set_S *DmxSet, DMX_RamPort_Info_S *PortInfo)
{
    HI_S32 ret = HI_FAILURE;

    switch(PortInfo->SecureMode)
    {
        case HI_UNF_DMX_SECURE_MODE_NONE:
            ret = DmxAllocNonSecureTsBuf(DmxSet, PortInfo);
            break;
        case HI_UNF_DMX_SECURE_MODE_TEE:
            ret = DmxAllocSecureTsBuf(DmxSet, PortInfo);
            break;
        default:
            BUG();
    }

    return ret;
}

static HI_VOID DmxFreeTsBuf(Dmx_Set_S *DmxSet, DMX_RamPort_Info_S *PortInfo)
{
    switch(PortInfo->SecureMode)
    {
        case HI_UNF_DMX_SECURE_MODE_NONE:
            DmxFreeNonSecureTsBuf(DmxSet, PortInfo);
            break;
        case HI_UNF_DMX_SECURE_MODE_TEE:
            DmxFreeSecureTsBuf(DmxSet, PortInfo);
            break;
        default:
            BUG();
    }
}

static HI_S32 DmxTrimTsBufAttrs(const HI_UNF_DMX_TSBUF_ATTR_S *InAttr, HI_UNF_DMX_TSBUF_ATTR_S *OutAttr)
{
    if ((InAttr->u32BufSize > DMX_MAX_TS_BUFFER_SIZE) || (InAttr->u32BufSize < DMX_MIN_TS_BUFFER_SIZE))
    {
        HI_ERR_DEMUX("TsBuffer size 0x%x invalid, bigger than 0x%x, or less than 0x%x\n",
            InAttr->u32BufSize, DMX_MAX_TS_BUFFER_SIZE, DMX_MIN_TS_BUFFER_SIZE);

        return HI_ERR_DMX_INVALID_PARA;
    }

    if (InAttr->enSecureMode >= HI_UNF_DMX_SECURE_MODE_BUTT)
    {
        HI_ERR_DEMUX("invalid secure mode(%d).\n", InAttr->enSecureMode);

        return HI_ERR_DMX_INVALID_PARA;
    }

    memcpy(OutAttr, InAttr, sizeof(HI_UNF_DMX_TSBUF_ATTR_S));

#ifdef DMX_TS_BUF_VERSION_2
    OutAttr->u32BufSize = InAttr->u32BufSize / DMX_MAX_BLOCK_SIZE * DMX_MAX_BLOCK_SIZE;
#else
    OutAttr->u32BufSize = InAttr->u32BufSize / 0x1000 * 0x1000;
#endif

    return HI_SUCCESS;
}

static HI_S32 DmxCreateTsBuffer(Dmx_Set_S *DmxSet, DMX_RamPort_Info_S *PortInfo, HI_U32 PortId, const HI_UNF_DMX_TSBUF_ATTR_S *pstTsBufAttr, DMX_MMZ_BUF_S *TsBuf)
{
    HI_S32 ret = HI_FAILURE;

    PortInfo->Owner        = task_tgid_nr(current);
    PortInfo->SecureMode   = pstTsBufAttr->enSecureMode;
    PortInfo->BufSize      = pstTsBufAttr->u32BufSize;

    spin_lock_init(&PortInfo->LockRamPort);

#ifdef DMX_TS_BUF_VERSION_2
    /* create ts buff block map */
    PortInfo->TotalBufBlkNR = PortInfo->BufSize / DMX_MAX_BLOCK_SIZE;
    atomic_set(&PortInfo->FreeBufBlkNR, PortInfo->TotalBufBlkNR);
    PortInfo->pstBufBlkMap = HI_KMALLOC(HI_ID_DEMUX, sizeof(*PortInfo->pstBufBlkMap) * PortInfo->TotalBufBlkNR, GFP_KERNEL);
    if (PortInfo->pstBufBlkMap)
    {
        HI_U32 index;

        memset(PortInfo->pstBufBlkMap, 0, sizeof(*PortInfo->pstBufBlkMap) * PortInfo->TotalBufBlkNR);

        for (index = 0; index < PortInfo->TotalBufBlkNR; index ++)
        {
            PortInfo->pstBufBlkMap[index].HeadBlkIdx = INVALID_HEAD_BLK_IDX;
        }
    }
    else
    {
        HI_ERR_DEMUX("Create ip buffer block map failed.\n");
        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out0;
    }

    memset(&PortInfo->LastGetBufInfo, 0 ,sizeof(PortInfo->LastGetBufInfo));
#else
    PortInfo->Read          = 0;
    PortInfo->Write         = 0;

    PortInfo->ReqAddr       = 0;
    PortInfo->ReqLen        = 0;
#endif

    ret = DmxAllocTsBuf(DmxSet, PortInfo);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    ret = DmxTeecRegisterRamPort(PortId, PortInfo->SecureMode);
    if (HI_SUCCESS != ret)
    {
        goto out2;
    }

    /*config the register */
    TsBufferConfig(DmxSet, PortId, HI_TRUE, PortInfo->DescPhyAddr, PortInfo->DescDepth);

    TsBuf->PhyAddr    = PortInfo->PhyAddr;
    TsBuf->Size       = PortInfo->BufSize;
    TsBuf->Flag       = PortInfo->BufFlag;

    HI_INFO_DEMUX("port %d attach ip buffer success, buffer size=0x%x\n", PortId, PortInfo->BufSize);

    return HI_SUCCESS;

out2:
    DmxFreeTsBuf(DmxSet, PortInfo);
out1:
#ifdef DMX_TS_BUF_VERSION_2
    HI_KFREE(HI_ID_DEMUX, PortInfo->pstBufBlkMap);
    PortInfo->pstBufBlkMap = HI_NULL;
    PortInfo->TotalBufBlkNR = 0;
    atomic_set(&PortInfo->FreeBufBlkNR, 0);
out0:
#endif
    return ret;
}

HI_S32 DMX_OsiTsBufferCreate(const HI_U32 PortId, const HI_UNF_DMX_TSBUF_ATTR_S *pstTsBufAttr, DMX_MMZ_BUF_S *TsBuf)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;
    HI_UNF_DMX_TSBUF_ATTR_S ValidAttr = {0};

    ret = DmxTrimTsBufAttrs(pstTsBufAttr, &ValidAttr);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);
    if (PortInfo->PhyAddr)
    {
        HI_ERR_DEMUX("TSBuffer has been used by other process:PortId=%u\n", PortId);
        ret = HI_ERR_DMX_RECREAT_TSBUFFER;
        goto out;
    }

    ret = DmxCreateTsBuffer(DmxSet, PortInfo, PortId, &ValidAttr, TsBuf);

out:
    return ret;
}

HI_S32 DMX_OsiTsBufferDestroy(const HI_U32 PortId)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        ret =  HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);
    if (PortInfo->PhyAddr)
    {
        TsBufferDeConfig(DmxSet, PortId, HI_FALSE, 0, 0);

        DmxTeecUnregisterRamPort(PortId, PortInfo->SecureMode);

        DmxFreeTsBuf(DmxSet, PortInfo);

#ifdef DMX_TS_BUF_VERSION_2
        if (PortInfo->pstBufBlkMap)
        {
            HI_KFREE(HI_ID_DEMUX, PortInfo->pstBufBlkMap);
            PortInfo->pstBufBlkMap = HI_NULL;
            PortInfo->TotalBufBlkNR = 0;
            atomic_set(&PortInfo->FreeBufBlkNR, 0);
        }
#endif

        PortInfo->PhyAddr       = 0;
        PortInfo->DescPhyAddr   = 0;

        PortInfo->IpRate = DMX_DEFAULT_IP_SPEED;

        RamPortClean(PortId);

        ret = HI_SUCCESS;
    }
    else
    {
        HI_DBG_DEMUX("Invalid ram port id!\n");
    }

out:
    return ret;
}

#ifdef DMX_TS_BUF_VERSION_2
HI_S32 DMX_OsiTsBufferGet(const HI_U32 PortId, const HI_U32 ReqLen, DMX_DATA_BUF_S *Buf, const HI_U32 Timeout)
{
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;
    HI_U32 BufPhyAddr ;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);

    if (0 == PortInfo->PhyAddr)
    {
        HI_ERR_DEMUX("Invalid ram port id(%d).\n", PortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (0 == ReqLen)
    {
        HI_ERR_DEMUX("port %d get buf len is 0\n", PortId);

        return HI_ERR_DMX_INVALID_PARA;
    }

    if (ReqLen > PortInfo->BufSize)
    {
        HI_ERR_DEMUX("error, ReqLen(0x%x) > BufSize(0x%x)\n", ReqLen, PortInfo->BufSize);

        return HI_ERR_DMX_INVALID_PARA;
    }

    ++PortInfo->GetCount;

    /*
        call GetIpFreeDescAndBufLen
        1. check wether TS buffer have enough space for user to get (u32DataSize)
        2. check wether Desc queue have enough free Desc to description the (u32DataSize) TS buffer ,
        each Desc can descript 64K block of buffer.
        3. if this return HI_TRUE,
        the space of PortInfo->ReqAddr and PortInfo->ReqLen will have right value (be writen in this function )
     */
    if (HI_SUCCESS != CheckIPBuf(DmxSet, PortId, ReqLen, &BufPhyAddr))
    {
        HI_S32 ret;

        if (0 == Timeout)
        {
            return HI_ERR_DMX_NOAVAILABLE_BUF;
        }

        /* if have not enough space ,the PortInfo->WaitLen will be give value ,and wait the interupt of
        DmxHalIPPortGetOutIntStatus (new Desc be readed by logic,so there may be new space avaliable),
        every time in DmxHalIPPortGetOutIntStatus ,will call GetIpFreeDescAndBufLen again , if return HI_FALSE,
        will wake up */
        PortInfo->WakeUp    = HI_FALSE;
        PortInfo->WaitLen   = ReqLen;

        ret = wait_event_interruptible_timeout(PortInfo->WaitQueue, PortInfo->WakeUp, msecs_to_jiffies(Timeout));
        if (!PortInfo->WakeUp)
        {
            HI_DBG_DEMUX("timeout\n");

            return HI_ERR_DMX_TIMEOUT;
        }
        else
        {
            if (HI_SUCCESS != CheckIPBuf(DmxSet, PortId, ReqLen, &BufPhyAddr))
            {
                return HI_ERR_DMX_NOAVAILABLE_BUF;
            }
        }
    }

    Buf->BufPhyAddr = BufPhyAddr;
    Buf->BufKerAddr = PortInfo->KerAddr + (BufPhyAddr - PortInfo->PhyAddr);
    Buf->BufLen     = ReqLen;

    ++PortInfo->GetValidCount;

    PortInfo->LastGetBufInfo.BufPhyAddr = Buf->BufPhyAddr;
    PortInfo->LastGetBufInfo.BufKerAddr = Buf->BufKerAddr;
    PortInfo->LastGetBufInfo.BufLen = Buf->BufLen;

    return HI_SUCCESS;
}

HI_VOID DMX_OsrSaveIPTs(DMX_RamPort_Info_S *PortInfo, HI_U8 *buf, HI_U32 len,HI_U32 u32PortID);

HI_S32 DMX_OsiTsBufferPush(const HI_U32 PortId, DMX_DATA_BUF_S *pstData)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;
    HI_U32 WriteLen, BufferPhyAddr;
    HI_U8 *BufferVirAddr;
    HI_SIZE_T LockFlag;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);
    if (0 == PortInfo->PhyAddr)
    {
        HI_ERR_DEMUX("Invalid ram port id(%d).\n", PortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    /* verify buffer valid */
    ret= VerifyPushBuffBlk(PortInfo, pstData);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("Push Ts buffer parameter(buffPhyAddr:0x%x, size:0x%x) is invalid.\n", pstData->BufPhyAddr, pstData->BufLen);
        return HI_ERR_DMX_INVALID_PARA;
    }

    BufferPhyAddr = pstData->BufPhyAddr;
    BufferVirAddr = BufferPhyAddr - PortInfo->PhyAddr + PortInfo->KerAddr ;
    WriteLen = pstData->BufLen;

#ifdef HI_DEMUX_PROC_SUPPORT
    DMX_OsrSaveIPTs(PortInfo, BufferVirAddr, WriteLen, PortId);

    ++PortInfo->PutCount;
#endif

    /* add desc */
    do
    {
        HI_U32 len = DMX_MAX_BLOCK_SIZE - (BufferPhyAddr - PortInfo->PhyAddr) % DMX_MAX_BLOCK_SIZE;
        len = len <= WriteLen ? len : WriteLen;

        if (len > 0)
        {
            HI_U32 BlkIdx = (BufferPhyAddr - PortInfo->PhyAddr) / DMX_MAX_BLOCK_SIZE;

            spin_lock_irqsave(&(PortInfo->LockRamPort), LockFlag);
            PortInfo->pstBufBlkMap[BlkIdx].BlkFlag = ADD_INT_FLAG;
            PortInfo->pstBufBlkMap[BlkIdx].BlkRef ++;
            spin_unlock_irqrestore(&(PortInfo->LockRamPort), LockFlag);
        }

        // set ip descriptor
        *PortInfo->DescCurrAddr++ = BufferPhyAddr;
        *PortInfo->DescCurrAddr++ = len;
        *PortInfo->DescCurrAddr++ = len;
        *PortInfo->DescCurrAddr++ = 0;
        DmxHalIPPortDescAdd(DmxSet, PortId, 1);

        atomic_inc(&PortInfo->LastDescWriteIdx);
        if (atomic_read(&PortInfo->LastDescWriteIdx) >= PortInfo->DescDepth)
        {
            atomic_set(&PortInfo->LastDescWriteIdx, 0);
            PortInfo->DescCurrAddr = (HI_U32*)PortInfo->DescKerAddr;
        }

        if (0 == len)
        {
            break;
        }

        BufferPhyAddr += len;
        WriteLen -= len;
    }while(1);

    return HI_SUCCESS;
}

HI_S32 DMX_OsiTsBufferRelease(const HI_U32 PortId, DMX_DATA_BUF_S *pstData)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);
    if (0 == PortInfo->PhyAddr)
    {
        HI_ERR_DEMUX("Invalid ram port id(%d).\n", PortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    /* verify buffer valid */
    ret = VerifyBuffBlk(PortInfo, pstData);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("Release Ts buffer parameter(buffPhyAddr:0x%x, size:0x%x) is invalid.\n", pstData->BufPhyAddr, pstData->BufLen);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    /* free blk belong to buff */
    ret = FreeBuffBlk(PortInfo, pstData);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("Free alloced but unused buff blk failed.\n");
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    ret = HI_SUCCESS;

out:
    return ret;
}

HI_S32 DMX_OsiTsBufferPut(const HI_U32 PortId, const HI_U32 DataLen, const HI_U32 StartPos)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;
    DMX_DATA_BUF_S stDataInfo;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        ret =  HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);

    stDataInfo.BufPhyAddr = PortInfo->LastGetBufInfo.BufPhyAddr + StartPos;
    stDataInfo.BufKerAddr = PortInfo->LastGetBufInfo.BufKerAddr + StartPos;
    stDataInfo.BufLen = DataLen;
    ret = DMX_OsiTsBufferPush(PortId, &stDataInfo);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    stDataInfo.BufPhyAddr = PortInfo->LastGetBufInfo.BufPhyAddr;
    stDataInfo.BufKerAddr = PortInfo->LastGetBufInfo.BufKerAddr;
    stDataInfo.BufLen = PortInfo->LastGetBufInfo.BufLen;
    ret = DMX_OsiTsBufferRelease(PortId, &stDataInfo);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    ret = HI_SUCCESS;

out:
    return ret;
}

HI_S32 DMX_OsiTsBufferGetStatus(const HI_U32 PortId, HI_UNF_DMX_TSBUF_STATUS_S *Status)
{
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;
    HI_SIZE_T LockFlag;
    HI_U32 index = 0, LastContBlkNum = 0;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);
    if (0 == PortInfo->PhyAddr)
    {
        HI_ERR_DEMUX("Invalid ram port id(%d).\n", PortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    Status->u32BufSize = PortInfo->TotalBufBlkNR * DMX_MAX_BLOCK_SIZE;

    spin_lock_irqsave(&PortInfo->LockRamPort, LockFlag);

    /* locate max free continue space */
    for (index = 0; index < PortInfo->TotalBufBlkNR; index ++)
    {
        if (0 != PortInfo->pstBufBlkMap[index].BlkFlag)
        {
            continue;
        }
        else
        {
            HI_U32 i, max = 1;

            for (i = index + 1; i < PortInfo->TotalBufBlkNR; i++)
            {
                if (0 != PortInfo->pstBufBlkMap[i].BlkFlag)
                {
                    break;
                }
                else
                {
                    max ++;
                }
            }

            if (max > LastContBlkNum)
            {
                LastContBlkNum = max;
            }
        }
    }

    Status->u32UsedSize = ( PortInfo->TotalBufBlkNR - LastContBlkNum) * DMX_MAX_BLOCK_SIZE;

    spin_unlock_irqrestore(&PortInfo->LockRamPort, LockFlag);

    return HI_SUCCESS;
}
#else
HI_S32 DMX_OsiTsBufferGet(const HI_U32 PortId, const HI_U32 ReqLen, DMX_DATA_BUF_S *Buf, const HI_U32 Timeout)
{
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;
    HI_U32 RawPortId;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);

    if (0 == PortInfo->PhyAddr)
    {
        HI_ERR_DEMUX("TSbuffer phy addr is 0, port id=%d.\n", PortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (0 == ReqLen)
    {
        HI_ERR_DEMUX("port %d get buf len is 0\n", PortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (ReqLen > PortInfo->BufSize)
    {
        HI_ERR_DEMUX("error, ReqLen(0x%x) > BufSize(0x%x)\n", ReqLen, PortInfo->BufSize);
        return HI_ERR_DMX_INVALID_PARA;
    }

    ++PortInfo->GetCount;

    RawPortId  = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

    /**
    call GetIpFreeDescAndBufLen
    1. check wether TS buffer have enough space for user to get (u32DataSize)
    2. check wether Desc queue have enough free Desc to description the (u32DataSize) TS buffer ,
    each Desc can descript 64K block of buffer.
    3. if this return HI_TRUE,
    the space of PortInfo->ReqAddr and PortInfo->ReqLen will have right value (be writen in this function )
     */
    if (CheckIPBuf(DmxSet, RawPortId, ReqLen))
    {
        HI_S32 ret;

        if (0 == Timeout)
        {
            return HI_ERR_DMX_NOAVAILABLE_BUF;
        }

        /*if have not enough space ,the PortInfo->WaitLen will be give value ,and wait the interupt of
        DmxHalIPPortGetOutIntStatus (new Desc be readed by logic,so there may be new space avaliable),
        every time in DmxHalIPPortGetOutIntStatus ,will call GetIpFreeDescAndBufLen again , if return HI_FALSE,
        will wake up*/
        PortInfo->WakeUp    = HI_FALSE;
        PortInfo->WaitLen   = ReqLen;

        ret = wait_event_interruptible_timeout(PortInfo->WaitQueue, PortInfo->WakeUp, msecs_to_jiffies(Timeout));
        if (0 == ret || !PortInfo->WakeUp || 0 == PortInfo->ReqAddr)
        {
            HI_WARN_DEMUX("timeout\n");

            return HI_ERR_DMX_TIMEOUT;
        }
    }

    Buf->BufPhyAddr = PortInfo->ReqAddr;
    Buf->BufKerAddr = PortInfo->KerAddr + (PortInfo->ReqAddr - PortInfo->PhyAddr);
    Buf->BufLen     = ReqLen;

    ++PortInfo->GetValidCount;

    return HI_SUCCESS;
}

HI_VOID DMX_OsrSaveIPTs(DMX_RamPort_Info_S *PortInfo, HI_U8 *buf, HI_U32 len,HI_U32 u32PortID);

HI_S32 DMX_OsiTsBufferPut(const HI_U32 PortId, const HI_U32 DataLen, const HI_U32 StartPos)
{
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;
    HI_U32              BufferAddr;
    HI_U32              BufferLen;
    HI_U32              Offset;
    HI_SIZE_T           LockFlag;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);
    if (0 == PortInfo->PhyAddr)
    {
        HI_ERR_DEMUX("Invalid ram port id(%d).\n", PortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (0 == PortInfo->PhyAddr)
    {
        HI_ERR_DEMUX("put phyaddr is 0!");
        return HI_ERR_DMX_INVALID_PARA;
    }

    if ((0 == PortInfo->ReqLen) || (0 == DataLen))
    {
        return HI_SUCCESS;
    }

    if (PortInfo->ReqLen < StartPos + DataLen)
    {
        HI_ERR_DEMUX("port %u put buf len %u, request len %u\n", PortId, DataLen, PortInfo->ReqLen);

        return HI_ERR_DMX_INVALID_PARA;
    }

    spin_lock_irqsave(&PortInfo->LockRamPort, LockFlag);
    Offset = PortInfo->ReqAddr - PortInfo->PhyAddr;

    PortInfo->Write = Offset + DataLen + StartPos;

    BufferAddr  = PortInfo->ReqAddr + StartPos;
    BufferLen   = DataLen;

    PortInfo->ReqLen    = 0;
    PortInfo->ReqAddr   = 0;
    spin_unlock_irqrestore(&PortInfo->LockRamPort, LockFlag);

#ifdef HI_DEMUX_PROC_SUPPORT
    DMX_OsrSaveIPTs(PortInfo, PortInfo->KerAddr + Offset + StartPos, DataLen, PortId);

    ++PortInfo->PutCount;
#endif

    do
    {
        HI_U32 len = (BufferLen >= DMX_MAX_BLOCK_SIZE) ? DMX_MAX_BLOCK_SIZE : BufferLen;

        // set ip descriptor
        *PortInfo->DescCurrAddr++ = BufferAddr;
        *PortInfo->DescCurrAddr++ = len;
        *PortInfo->DescCurrAddr++ = len;
        *PortInfo->DescCurrAddr++ = 0;

        if (++PortInfo->DescWrite >= PortInfo->DescDepth)
        {
            PortInfo->DescWrite     = 0;
            PortInfo->DescCurrAddr  = (HI_U32*)PortInfo->DescKerAddr;
        }
        /*the last time ,BufferLen == 0,will add a desc with len==0, this is OK,we need do it,
        Because
        1.when Desc read pointer updated , may be logic is reading the block ,not finished ,if at that time ,
        DMX_OsiTsBufferGet be called , then buffer may be covered.
        2. if we put a ZERO Desc ,then ,every time when Desc read pointer updated ,the logic must finished read the block*/
        DmxHalIPPortDescAdd(DmxSet, PortId, 1);

        if (0 == len)
        {
            break;
        }

        BufferAddr  += len;
        BufferLen   -= len;
    } while (1);

    return HI_SUCCESS;
}

HI_S32 DMX_OsiTsBufferGetStatus(const HI_U32 PortId, HI_UNF_DMX_TSBUF_STATUS_S *Status)
{
    HI_S32              ret         = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;
    HI_U32              Head;
    HI_U32              Tail;
    HI_SIZE_T           LockFlag;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);

    spin_lock_irqsave(&PortInfo->LockRamPort, LockFlag);
    if (PortInfo->PhyAddr)
    {
        Status->u32BufSize  = PortInfo->BufSize;
        Status->u32UsedSize = PortInfo->BufSize;
        Status->enSecureMode  = PortInfo->SecureMode;

        if (PortInfo->Read < PortInfo->Write)
        {
            Head = PortInfo->Read;
            Tail = PortInfo->BufSize - PortInfo->Write;

            Head = (Head <= DMX_TS_BUFFER_GAP) ? 0 : (Head - DMX_TS_BUFFER_GAP);
            Tail = (Tail <= DMX_TS_BUFFER_GAP) ? 0 : (Tail - DMX_TS_BUFFER_GAP);

            if (Head < Tail)
            {
                Status->u32UsedSize = PortInfo->BufSize - Tail;
            }
            else
            {
                Status->u32UsedSize = PortInfo->BufSize - Head;
            }
        }
        else if (PortInfo->Read > PortInfo->Write)
        {
            Head = PortInfo->Read - PortInfo->Write;
            if (Head > DMX_TS_BUFFER_GAP)
            {
                Status->u32UsedSize = DMX_TS_BUFFER_GAP + PortInfo->BufSize - Head;
            }
        }
        else
        {
            if (0 == PortInfo->Read)
            {
                Status->u32UsedSize = DMX_TS_BUFFER_GAP;
            }
        }

        ret = HI_SUCCESS;
    }
    spin_unlock_irqrestore(&PortInfo->LockRamPort, LockFlag);

    return ret;
}
#endif

HI_S32 DMX_OsiTsBufferReset(const HI_U32 PortId)
{
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);
    if (PortInfo->PhyAddr)
    {
        RamPortClean(PortId);
    }

    return HI_SUCCESS;
}

/*
 * filter acquire and release helper functions.
 */
static HI_S32 DmxFltAcquire(HI_U32 DmxId, HI_U32 *FltId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_FilterInfo_S  *FltInfo = HI_NULL;

    BUG_ON(!mutex_is_locked(&DmxSet->LockAllFlt));

    *FltId = find_first_zero_bit(DmxSet->FilterBitmap, DmxSet->DmxFilterCnt);
    if (!(*FltId < DmxSet->DmxFilterCnt))
    {
        HI_ERR_DEMUX("There is no available filter now!\n");
        ret = HI_ERR_DMX_NOFREE_FILTER;
        goto out;
    }

    FltInfo = &DmxSet->DmxFilterInfo[*FltId];

    mutex_lock(&FltInfo->LockFlt);

    FltInfo->FilterId = *FltId;
    FltInfo->DmxId = DmxId;
    FltInfo->ChanId = DMX_INVALID_CHAN_ID;
    FltInfo->Owner  = task_tgid_nr(current);
    FltInfo->Depth = 0;

    set_bit(*FltId, DmxSet->FilterBitmap);

    mutex_unlock(&FltInfo->LockFlt);

    ret = HI_SUCCESS;

out:
    return ret;
}

static inline HI_VOID DmxFltRelease(HI_U32 DmxId, HI_U32 FltId)
{
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_FilterInfo_S *FltInfo = &DmxSet->DmxFilterInfo[FltId];

    BUG_ON(FltId >= DmxSet->DmxFilterCnt);
    BUG_ON(!mutex_is_locked(&FltInfo->LockFlt));

    FltInfo->FilterId = DMX_INVALID_FILTER_ID;
    FltInfo->DmxId = DMX_INVALID_DEMUX_ID;
    FltInfo->ChanId = DMX_INVALID_CHAN_ID;

    clear_bit(FltId, DmxSet->FilterBitmap);
}

/*
 * exclusively get and put filter helper functions.
 */
HI_S32 DmxGetFltInstance(Dmx_Set_S *DmxSet, HI_U32 FltId)
{
    HI_S32 ret = HI_FAILURE;
    unsigned long mask, *p;
    DMX_FilterInfo_S  *FltInfo = HI_NULL;

    if (unlikely(!DmxSet))
    {
        HI_ERR_DEMUX("Invalid DemuxSet.\n");
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    if (unlikely(FltId >= DmxSet->DmxFilterCnt))
    {
        HI_ERR_DEMUX("Invalid filter id :0x%x\n", FltId);

        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    FltInfo = &DmxSet->DmxFilterInfo[FltId];

    if (unlikely(0 != mutex_lock_interruptible(&FltInfo->LockFlt)))
    {
        HI_WARN_DEMUX("Filter mutex interruptibled.\n");

        ret = HI_ERR_DMX_BUSY_FILTER;
        goto out0;
    }

    mask = BIT_MASK(FltId);
    p = ((unsigned long*)DmxSet->FilterBitmap) + BIT_WORD(FltId);
    if (unlikely(!(*p & mask)))
    {
        HI_ERR_DEMUX("Try to Get invalid Filter(%d) instance.\n", FltId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    return HI_SUCCESS;

out1:
    mutex_unlock(&FltInfo->LockFlt);
out0:
    return ret;
}

HI_VOID DmxPutFltInstance(Dmx_Set_S *DmxSet, HI_U32 FltId)
{
    DMX_FilterInfo_S  *FltInfo = &DmxSet->DmxFilterInfo[FltId];

    BUG_ON(FltId >= DmxSet->DmxFilterCnt);

    mutex_unlock(&FltInfo->LockFlt);
}

/***********************************************************************************
* Function      : DMX_OsiNewFilter
* Description   : apply a new filter
* Input         : DmxId
* Output        : FilterId
* Return        : HI_SUCCESS:     success
*                 HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiNewFilter(const HI_U32 DmxId, HI_U32 *FilterId)
{
    HI_S32 ret = HI_ERR_DMX_NOFREE_FILTER;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    if (!DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    mutex_lock(&DmxSet->LockAllFlt);

    ret = DmxFltAcquire(DmxId, FilterId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

out1:
    mutex_unlock(&DmxSet->LockAllFlt);
out0:
    return ret;
}

/***********************************************************************************
* Function      :  DMX_OsiDeleteFilter
* Description   :  delete a filter
* Input         :  FilterId
* Output        :
* Return        :  HI_SUCCESS:     success
*                  HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiDeleteFilter(HI_U32 DmxId, const HI_U32 FilterId)
{
    HI_S32 ret = HI_SUCCESS;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_FilterInfo_S *Filter = HI_NULL;
    DMX_ChanInfo_S *Chan = HI_NULL;

    ret = DmxGetFltInstance(DmxSet, FilterId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    Filter = &DmxSet->DmxFilterInfo[FilterId];
    Chan = &DmxSet->DmxChanInfo[Filter->ChanId];

    BUG_ON(GetDmxSetByDmxid(Filter->DmxId) != DmxSet);

    if (Filter->ChanId < DmxSet->DmxChanCnt)
    {
        DmxHalDetachFilter(DmxSet, FilterId, Filter->ChanId);

        atomic_dec(&Chan->FilterCount);
    }

    DmxFltRelease(DmxId, FilterId);

    DmxPutFltInstance(DmxSet, FilterId);
out:
    return ret;
}

/***********************************************************************************
* Function      :  DMX_OsiSetFilterAttr
* Description   :  set filter attr
* Input         :  FilterId, FilterAttr
* Output        :
* Return        :  HI_SUCCESS:     success
*                  HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiSetFilterAttr(HI_U32 DmxId, const HI_U32 FilterId, const HI_UNF_DMX_FILTER_ATTR_S *FilterAttr)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_FilterInfo_S *Filter = HI_NULL;
    HI_U32 i;

    ret = DmxGetFltInstance(DmxSet, FilterId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    Filter = &DmxSet->DmxFilterInfo[FilterId];

    BUG_ON(GetDmxSetByDmxid(Filter->DmxId) != DmxSet);

    if (FilterAttr->u32FilterDepth > DMX_FILTER_MAX_DEPTH)
    {
        HI_ERR_DEMUX("filter %u set depth is %u too long\n", FilterId, FilterAttr->u32FilterDepth);

        ret = HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    for (i = 0; i < FilterAttr->u32FilterDepth; i++)
    {
        if (FilterAttr->au8Negate[i] > 1)
        {
            HI_ERR_DEMUX("filter %u set negate is invalid\n", FilterId);

            ret = HI_ERR_DMX_INVALID_PARA;
            goto out1;
        }
    }

    if (DMX_INVALID_FILTER_ID != Filter->FilterId)
    {
        Filter->Depth = FilterGetValidDepth(FilterAttr);

        for (i = 0; i < DMX_FILTER_MAX_DEPTH; i++)
        {
            if (i < FilterAttr->u32FilterDepth)
            {
                HI_U32 Negate = FilterAttr->au8Negate[i];

                if (0xff == FilterAttr->au8Mask[i])
                {
                    Negate = 0;
                }

                Filter->Match[i]   = FilterAttr->au8Match[i];
                Filter->Mask[i]    = FilterAttr->au8Mask[i];
                Filter->Negate[i]  = Negate;

                DmxHalSetFilter(DmxSet, FilterId, i, Filter->Match[i], Negate, Filter->Mask[i]);
            }
            else
            {
                Filter->Match[i]   = 0;
                Filter->Mask[i]    = 0xff;
                Filter->Negate[i]  = 0;

                DmxHalSetFilter(DmxSet, FilterId, i, 0, 0, 0xff);
            }
        }

#ifdef DMX_FILTER_DEPTH_SUPPORT
        DmxHalFilterSetDepth(DmxSet, FilterId, Filter->Depth);
#endif

        ret = HI_SUCCESS;
    }

out1:
    DmxPutFltInstance(DmxSet, FilterId);
out0:
    return ret;
}

/***********************************************************************************
* Function      :  DMX_OsiGetFilterAttr
* Description   :  get filter attr
* Input         :  FilterId
* Output        :  FilterAttr
* Return        :  HI_SUCCESS:     success
*                  HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiGetFilterAttr(HI_U32 DmxId, const HI_U32 FilterId, HI_UNF_DMX_FILTER_ATTR_S *FilterAttr)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_FilterInfo_S *Filter = HI_NULL;

    ret = DmxGetFltInstance(DmxSet, FilterId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    Filter = &DmxSet->DmxFilterInfo[FilterId];

    BUG_ON(GetDmxSetByDmxid(Filter->DmxId) != DmxSet);

    FilterAttr->u32FilterDepth = Filter->Depth;

    memcpy(FilterAttr->au8Match,    Filter->Match,  DMX_FILTER_MAX_DEPTH);
    memcpy(FilterAttr->au8Mask,     Filter->Mask,   DMX_FILTER_MAX_DEPTH);
    memcpy(FilterAttr->au8Negate,   Filter->Negate, DMX_FILTER_MAX_DEPTH);

    DmxPutFltInstance(DmxSet, FilterId);
out:
    return ret;
}

/***********************************************************************************
* Function      :  DMX_OsiAttachFilter
* Description   :  attach filter to channel
* Input         :  FilterId ChanId
* Output        :
* Return        :  HI_SUCCESS:     success
*                  HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiAttachFilter(const HI_U32 DmxId, const HI_U32 FilterId, const HI_U32 ChanId)
{
    HI_S32 ret = HI_SUCCESS;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *Chan = HI_NULL;
    DMX_FilterInfo_S *Filter = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    Chan = &DmxSet->DmxChanInfo[ChanId];

    if (DMX_INVALID_CHAN_ID == Chan->ChanId)
    {
        HI_ERR_DEMUX("ChanId(%d) is invalid!\n", ChanId);
        ret =  HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    if (GetDmxSetByDmxid(Chan->DmxId) != DmxSet)
    {
        HI_ERR_DEMUX("Channel(%d) attached with demux(%d) ,not demux(%d).\n", ChanId, Chan->DmxId, DmxId);
        ret =  HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    if (   (HI_UNF_DMX_CHAN_TYPE_SEC    != Chan->ChanType)
        && (HI_UNF_DMX_CHAN_TYPE_ECM_EMM!= Chan->ChanType)
        && (HI_UNF_DMX_CHAN_TYPE_PES    != Chan->ChanType) )
    {
        HI_ERR_DEMUX("invalid channel type\n");

        ret =  HI_ERR_DMX_NOT_SUPPORT;
        goto out1;
    }

    ret = DmxGetFltInstance(DmxSet, FilterId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    Filter = &DmxSet->DmxFilterInfo[FilterId];

    BUG_ON(GetDmxSetByDmxid(Filter->DmxId) != DmxSet);

    if (DMX_INVALID_CHAN_ID != Filter->ChanId)
    {
        HI_ERR_DEMUX("filter already attached!\n");
        ret = HI_ERR_DMX_ATTACHED_FILTER;
        goto out2;
    }

    if (atomic_read(&Chan->FilterCount) >= DMX_MAX_FILTER_NUM_PER_CHANNEL)
    {
        HI_ERR_DEMUX("channel %u has maximum filters\n", ChanId);

        ret = HI_ERR_DMX_NOT_SUPPORT;
        goto out2;
    }

    atomic_inc(&Chan->FilterCount);

    Filter->ChanId = ChanId;

    DmxHalAttachFilter(DmxSet, FilterId, ChanId);

out2:
    DmxPutFltInstance(DmxSet, FilterId);
out1:
    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

/***********************************************************************************
* Function      : DMX_OsiDetachFilter
* Description   : detach filter from a channel
* Input         : FilterId, ChanId
* Output        :
* Return        : HI_SUCCESS:     success
*                 HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiDetachFilter(const HI_U32 DmxId, const HI_U32 FilterId, const HI_U32 ChanId)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *Chan = HI_NULL;
    DMX_FilterInfo_S *Filter = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    Chan = &DmxSet->DmxChanInfo[ChanId];

    if (GetDmxSetByDmxid(Chan->DmxId) != DmxSet)
    {
        HI_ERR_DEMUX("Channel(%d) attached with demux(%d) ,not demux(%d).\n", ChanId, Chan->DmxId, DmxId);
        ret =  HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    ret = DmxGetFltInstance(DmxSet, FilterId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    Filter = &DmxSet->DmxFilterInfo[FilterId];

    BUG_ON(GetDmxSetByDmxid(Filter->DmxId) != DmxSet);

    if (DMX_INVALID_CHAN_ID != Filter->ChanId)
    {
        if (ChanId == Filter->ChanId)
        {
            DmxHalDetachFilter(DmxSet, FilterId, ChanId);

            Filter->ChanId = DMX_INVALID_CHAN_ID;

            atomic_dec(&Chan->FilterCount);
            ret = HI_SUCCESS;
        }
        else
        {
            ret = HI_ERR_DMX_UNMATCH_FILTER;
        }
    }
    else
    {
        ret = HI_ERR_DMX_NOATTACH_FILTER;
    }

    DmxPutFltInstance(DmxSet, FilterId);
out1:
    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

/***********************************************************************************
* Function      :   DMX_OsiGetFilterChannel
* Description   :  Get  filter attached channel
* Input         : FilterId
* Output        : pChanId
* Return        : HI_SUCCESS:     success
*                 HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiGetFilterChannel(HI_U32 DmxId, const HI_U32 FilterId, HI_U32 *ChanId)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_FilterInfo_S *Filter = HI_NULL;

    ret = DmxGetFltInstance(DmxSet, FilterId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    Filter = &DmxSet->DmxFilterInfo[FilterId];

    BUG_ON(GetDmxSetByDmxid(Filter->DmxId) != DmxSet);

    if (DMX_INVALID_CHAN_ID != Filter->ChanId)
    {
        *ChanId = Filter->ChanId;

        ret = HI_SUCCESS;
    }
    else
    {
        ret = HI_ERR_DMX_NOATTACH_FILTER;
    }

    DmxPutFltInstance(DmxSet, FilterId);
out:
    return ret;
}

/***********************************************************************************
* Function      : DMX_OsiGetFreeFilterNum
* Description   : Get free filter count
* Input         : DmxId
* Output        : FreeCount
* Return        : HI_SUCCESS
*                 HI_FAILURE
* Others:
***********************************************************************************/
HI_S32 DMX_OsiGetFreeFilterNum(const HI_U32 DmxId, HI_U32 *FreeCount)
{
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_FilterInfo_S *FilterInfo  = HI_NULL;
    HI_U32 i;

    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    FilterInfo  = DmxSet->DmxFilterInfo;

    *FreeCount = 0;

    for (i = 0; i < DmxSet->DmxFilterCnt; i++)
    {
        if (DMX_INVALID_FILTER_ID == FilterInfo[i].FilterId)
        {
            ++(*FreeCount);
        }
    }

    return HI_SUCCESS;
}

static HI_S32 DmxTrimChnAttrs(const HI_UNF_DMX_CHAN_ATTR_S *InChanAttr, HI_UNF_DMX_CHAN_ATTR_S *OutChanAttr)
{
    HI_U32 BufSize = InChanAttr->u32BufSize < MIN_MMZ_BUF_SIZE ? MIN_MMZ_BUF_SIZE : InChanAttr->u32BufSize;
    HI_UNF_DMX_CHAN_CRC_MODE_E  CrcMode = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
    HI_UNF_DMX_SECURE_MODE_E SecureMode = HI_UNF_DMX_SECURE_MODE_NONE;

    switch (InChanAttr->enChannelType)
    {
        case HI_UNF_DMX_CHAN_TYPE_SEC :
        case HI_UNF_DMX_CHAN_TYPE_ECM_EMM :
            switch (InChanAttr->enCRCMode)
            {
                case HI_UNF_DMX_CHAN_CRC_MODE_FORBID :
                case HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD :
                case HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_SEND :
                case HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD :
                case HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_SEND :
                    CrcMode = InChanAttr->enCRCMode;
                    break;

                default :
                    HI_ERR_DEMUX("invalid crc mode(0x%x).\n", InChanAttr->enCRCMode);
                    return HI_ERR_DMX_INVALID_PARA;
            }

        break;

        case HI_UNF_DMX_CHAN_TYPE_PES :
        case HI_UNF_DMX_CHAN_TYPE_AUD :
        case HI_UNF_DMX_CHAN_TYPE_VID :
            if (BufSize < DMX_PES_CHANNEL_MIN_SIZE)
            {
                BufSize = DMX_PES_CHANNEL_MIN_SIZE;
            }

            break;

        case HI_UNF_DMX_CHAN_TYPE_POST :
            break;

        default :
            HI_ERR_DEMUX("invalid chan type(0x%x).\n", InChanAttr->enChannelType);

        return HI_ERR_DMX_INVALID_PARA;
    }

    switch (InChanAttr->enOutputMode)
    {
        case HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY :
        case HI_UNF_DMX_CHAN_OUTPUT_MODE_REC :
            break;

        default :
            HI_ERR_DEMUX("Invalid output mode(x%x).\n", InChanAttr->enOutputMode);
            return HI_ERR_DMX_INVALID_PARA;
    }

    switch(InChanAttr->enSecureMode)
    {
        case HI_UNF_DMX_SECURE_MODE_TEE:
            switch(InChanAttr->enOutputMode)
            {
                case HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY:
                    switch (InChanAttr->enChannelType)
                    {
                        case HI_UNF_DMX_CHAN_TYPE_AUD:
                        case HI_UNF_DMX_CHAN_TYPE_VID:
                            break;

                         default:
                            HI_ERR_DEMUX("only av channel support secure play mode.\n");
                            return HI_ERR_DMX_INVALID_PARA;
                    }
                    break;

                case HI_UNF_DMX_CHAN_OUTPUT_MODE_REC:
                    switch (InChanAttr->enChannelType)
                    {
                        case HI_UNF_DMX_CHAN_TYPE_POST:
                            break;

                         default:
                            HI_ERR_DEMUX("only post channel support secure rec mode.\n");
                            return HI_ERR_DMX_INVALID_PARA;
                    }
                    break;

                default:
                    BUG();
            }

            SecureMode = HI_UNF_DMX_SECURE_MODE_TEE;
            break;

        default:
            SecureMode = HI_UNF_DMX_SECURE_MODE_NONE;
    }

    /* duplicate and update channel attrs */
    memcpy(OutChanAttr, InChanAttr, sizeof(HI_UNF_DMX_CHAN_ATTR_S));

    OutChanAttr->u32BufSize = (BufSize + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;
    OutChanAttr->enCRCMode = CrcMode;
    OutChanAttr->enSecureMode = SecureMode;

    return HI_SUCCESS;
}

#if defined(DMX_SECURE_CHANNEL_SUPPORT) && defined(DMX_TEE_SUPPORT)
static HI_S32 DmxTeecRegisterVidSecBuf(DMX_ChanInfo_S *ChanInfo, SMMU_BUFFER_S *MmuBuf)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE);
    operation.params[0].value.a = DMX_CHANHANDLE(ChanInfo->DmxId, ChanInfo->ChanId);
    operation.params[1].value.a = MmuBuf->u32StartSmmuAddr;
    operation.params[1].value.b = MmuBuf->u32Size;

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_REGISTER_VID_SECBUF, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_REGISTER_VID_SECBUF to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

out:
    return ret;
}

static HI_S32 DmxTeecUnregisterVidSecBuf(DMX_ChanInfo_S *ChanInfo, SMMU_BUFFER_S *MmuBuf)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE);
    operation.params[0].value.a = DMX_CHANHANDLE(ChanInfo->DmxId, ChanInfo->ChanId);
    operation.params[1].value.a = MmuBuf->u32StartSmmuAddr;

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_UNREGISTER_VID_SECBUF, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_UNREGISTER_VID_SECBUF to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

out:
    return ret;
}

static HI_S32 DmxTeecRegisterAudSecBuf(DMX_ChanInfo_S *ChanInfo, SMMU_BUFFER_S *MmuBuf, MMZ_BUFFER_S *MmzBuf)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_NONE);
    operation.params[0].value.a = DMX_CHANHANDLE(ChanInfo->DmxId, ChanInfo->ChanId);
    operation.params[1].value.a = MmuBuf->u32StartSmmuAddr;
    operation.params[1].value.b = MmuBuf->u32Size;
    operation.params[2].value.a = MmzBuf->u32StartPhyAddr;
    operation.params[2].value.b = MmzBuf->u32Size;

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_REGISTER_AUD_SECBUF, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_REGISTER_AUD_SECBUF to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

out:
    return ret;
}

static HI_S32 DmxTeecUnregisterAudSecBuf(DMX_ChanInfo_S *ChanInfo, SMMU_BUFFER_S *MmuBuf)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE);
    operation.params[0].value.a = DMX_CHANHANDLE(ChanInfo->DmxId, ChanInfo->ChanId);
    operation.params[1].value.a = MmuBuf->u32StartSmmuAddr;

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_UNREGISTER_AUD_SECBUF, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_UNREGISTER_AUD_SECBUF to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

out:
    return ret;
}

static HI_S32 DmxTeecRegisterAVSecBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo, SMMU_BUFFER_S *MmuBuf)
{
    HI_S32 ret = HI_FAILURE;

    switch(ChanInfo->ChanType)
    {
        case HI_UNF_DMX_CHAN_TYPE_VID:
            ret = DmxTeecRegisterVidSecBuf(ChanInfo, MmuBuf);
            break;

        case HI_UNF_DMX_CHAN_TYPE_AUD:
        {
            DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
            DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];
            HI_CHAR BufName[32];
            MMZ_BUFFER_S MmzBuf = {0};

            snprintf(BufName, sizeof(BufName), "DMX_%sShadowBuf[%u]", "Aud", ChanInfo->ChanId);
            if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(BufName, MMZ_OTHERS, MmuBuf->u32Size, 0, &MmzBuf))
            {
                HI_ERR_DEMUX("aud shadow memory allocate failed, BufSize=0x%x\n", MmuBuf->u32Size);
                break;
            }

            FqInfo->ShadowBufVirAddr = MmzBuf.pu8StartVirAddr;
            FqInfo->ShadowBufPhyAddr = MmzBuf.u32StartPhyAddr;
            FqInfo->ShadowBufSize    = MmuBuf->u32Size;
            FqInfo->ShadowBufFlag    = DMX_MMZ_BUF;

            ret = DmxTeecRegisterAudSecBuf(ChanInfo, MmuBuf, &MmzBuf);
            if (HI_SUCCESS != ret)
            {
                HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);

                FqInfo->ShadowBufVirAddr = HI_NULL;
                FqInfo->ShadowBufPhyAddr = 0;
                FqInfo->ShadowBufSize    = 0;
            }

            break;
        }

        default:
            BUG();
    }

    return ret;
}

static HI_S32 DmxTeecUnregisterAVSecBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo, SMMU_BUFFER_S *MmuBuf)
{
    HI_S32 ret = HI_FAILURE;

    switch(ChanInfo->ChanType)
    {
        case HI_UNF_DMX_CHAN_TYPE_VID:
            ret = DmxTeecUnregisterVidSecBuf(ChanInfo, MmuBuf);
            break;

        case HI_UNF_DMX_CHAN_TYPE_AUD:
        {
            DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
            DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];
            MMZ_BUFFER_S MmzBuf = {0};

            ret = DmxTeecUnregisterAudSecBuf(ChanInfo, MmuBuf);
            if (HI_SUCCESS == ret)
            {
                MmzBuf.pu8StartVirAddr = FqInfo->ShadowBufVirAddr;
                MmzBuf.u32StartPhyAddr = FqInfo->ShadowBufPhyAddr ;
                HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);

                FqInfo->ShadowBufVirAddr = HI_NULL;
                FqInfo->ShadowBufPhyAddr = 0;
                FqInfo->ShadowBufSize    = 0;
            }

            break;
        }

        default:
            BUG();
    }

    return ret;
}

static HI_S32 DmxTeecFixupAudSecBuf(DMX_ChanInfo_S *ChanInfo, HI_U32 ParserAddr, HI_U32 ParserLen)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.params[0].value.a = DMX_CHANHANDLE(ChanInfo->DmxId, ChanInfo->ChanId);
    operation.params[1].value.a = ParserAddr;
    operation.params[1].value.b = ParserLen;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE);

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_FIXUP_AUD_SEC_BUF, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_FIXUP_AUD_SEC_BUF to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

out:
    return ret;
}
#else
static inline HI_S32 DmxTeecRegisterAVSecBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo, SMMU_BUFFER_S *MmuBuf)
{
    HI_ERR_DEMUX("DMX_SECURE_CHANNEL_SUPPORT or DMX_TEE_SUPPORT not configured.\n");
    return HI_ERR_DMX_NOT_SUPPORT;
}

static inline HI_S32 DmxTeecUnregisterAVSecBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo, SMMU_BUFFER_S *MmuBuf)
{
    return HI_ERR_DMX_NOT_SUPPORT;
}

static inline HI_S32 DmxTeecFixupAudSecBuf(DMX_ChanInfo_S *ChanInfo, HI_U32 ParserAddr, HI_U32 ParserLen)
{
    HI_ERR_DEMUX("DMX_SECURE_CHANNEL_SUPPORT or DMX_TEE_SUPPORT not configured.\n");
    return HI_ERR_DMX_NOT_SUPPORT;
}
#endif

#ifdef DMX_MMU_SUPPORT
static HI_S32 DmxAllocSecureAVBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo)
{
    HI_S32 ret = HI_FAILURE;
    DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];
    HI_U32 FqDepth, FqBufSize, OqDepth, OqBufSize, BlockSize;
    HI_CHAR *str;
    HI_CHAR BufName[32];
    MMZ_BUFFER_S MmzBuf = {0};
    SMMU_BUFFER_S SecMmuBuf = {0};

    if (HI_UNF_DMX_SECURE_MODE_TEE != ChanInfo->ChanSecure)
    {
        HI_ERR_DEMUX("only secure channel can alloc secure av buf.\n");
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    /*
    1.if buffer size > 1208*0x3ff (1M),will enter the following if ,and change the BlockSize,then ,the FQ Depth == DMX_MAX_AVFQ_DESC(0x3ff)
    2.if buffer size > 8192*0x3ff (8M),will enter the following if ,and change the BlockSize,then ,the FQ Depth == DMX_MAX_AVFQ_DESC(0x3ff)
    3.usually ,I think will this will not happen.(in StartAVPlay ,we set vedio channel buffer size as 0x300000)
    */
    str = (HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType) ? "Aud" : "Vid";
    BlockSize = (HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType) ? DMX_FQ_AUD_BLOCK_SIZE : DMX_FQ_VID_BLOCK_SIZE;
    if ((ChanInfo->ChanBufSize / DMX_MAX_AVFQ_DESC) > BlockSize)
    {
        BlockSize = (ChanInfo->ChanBufSize/ DMX_MAX_AVFQ_DESC) - ((ChanInfo->ChanBufSize / DMX_MAX_AVFQ_DESC) % 4);
        BlockSize = (BlockSize > DMX_MAX_BLOCK_SIZE) ? DMX_MAX_BLOCK_SIZE : BlockSize;
    }

    snprintf(BufName, sizeof(BufName), "DMX_%sBuf[%u]", str, ChanInfo->ChanId);

    ret = HI_DRV_SECSMMU_Alloc(BufName, ChanInfo->ChanBufSize, 4, &SecMmuBuf);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("smmu secure memory allocate failed, BufSize=0x%x\n", ChanInfo->ChanBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out0;
    }

    BUG_ON(SecMmuBuf.u32Size != ChanInfo->ChanBufSize);

    ret = DmxTeecRegisterAVSecBuf(DmxSet, ChanInfo, &SecMmuBuf);
    if (HI_SUCCESS != ret)
    {
        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out1;
    }

    /* SecMmuBuf.pu8StartVirAddr is NULL, replace with SecMmuBuf.u32StartSmmuAddr. */
    FqInfo->BufVirAddr   = (HI_U8*)(unsigned long)SecMmuBuf.u32StartSmmuAddr;
    FqInfo->BufPhyAddr   = SecMmuBuf.u32StartSmmuAddr;
    FqInfo->BufSize      = SecMmuBuf.u32Size;
    FqInfo->BufFlag      = DMX_SECURE_BUF;
    FqInfo->BlockSize    = BlockSize;

    FqDepth     = FqInfo->BufSize / FqInfo->BlockSize + 1;
    FqBufSize   = (FqDepth * DMX_FQ_DESC_SIZE + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;

    OqDepth     = (FqDepth < DMX_OQ_DEPTH) ? (FqDepth - 1) : DMX_OQ_DEPTH;
    OqBufSize   = OqDepth * DMX_OQ_DESC_SIZE;

    snprintf(BufName, sizeof(BufName), "DMX_%sDscBuf[%u]", str, ChanInfo->ChanId);
    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(BufName, MMZ_OTHERS, FqBufSize + OqBufSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("memory allocate failed, BufSize=0x%x\n", FqBufSize + OqBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out2;
    }

    FqInfo->FqDescSetBase = (FQ_DescInfo_S*)MmzBuf.pu8StartVirAddr;
    FqInfo->FqDescSetPhy  = MmzBuf.u32StartPhyAddr;
    FqInfo->FQDepth       = FqDepth;

    OqInfo->OqDescSetBase = (OQ_DescInfo_S*)((HI_U8*)FqInfo->FqDescSetBase + FqBufSize);
    OqInfo->OqDescSetPhy  = FqInfo->FqDescSetPhy + FqBufSize;
    OqInfo->OQDepth       = OqDepth;

    return HI_SUCCESS;

out2:
    DmxTeecUnregisterAVSecBuf(DmxSet, ChanInfo, &SecMmuBuf);
out1:
    HI_DRV_SECSMMU_Release(&SecMmuBuf);
out0:
    return ret;
}

static HI_VOID DmxFreeSecureAVBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo)
{
    DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];
    MMZ_BUFFER_S MmzBuf = {0};
    SMMU_BUFFER_S SecMmuBuf = {0};

    /* release desc buff. */
    MmzBuf.pu8StartVirAddr = (HI_U8*)FqInfo->FqDescSetBase;
    MmzBuf.u32StartPhyAddr = FqInfo->FqDescSetPhy ;
    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);

    /* release data buff. */
    SecMmuBuf.pu8StartVirAddr = (HI_U8*)HI_NULL;
    SecMmuBuf.u32StartSmmuAddr = FqInfo->BufPhyAddr;
    DmxTeecUnregisterAVSecBuf(DmxSet, ChanInfo, &SecMmuBuf);
    HI_DRV_SECSMMU_Release(&SecMmuBuf);
}

static HI_S32 DmxFixSecureAudBuf(DMX_ChanInfo_S *ChanInfo, DMX_Stream_S *EsData)
{
    return DmxTeecFixupAudSecBuf(ChanInfo, EsData->u32BufPhyAddr, EsData->u32BufLen);
}

static HI_S32 DmxAllocNonSecureAVBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo)
{
    HI_S32 ret = HI_FAILURE;
    DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];
    HI_U32 FqDepth, FqBufSize, OqDepth, OqBufSize, BlockSize;
    HI_CHAR *str;
    HI_CHAR BufName[32];
    SMMU_BUFFER_S MmuBuf = {0};
    MMZ_BUFFER_S MmzBuf = {0};

    /*
    1.if buffer size > 1208*0x3ff (1M),will enter the following if ,and change the BlockSize,then ,the FQ Depth == DMX_MAX_AVFQ_DESC(0x3ff)
    2.if buffer size > 8192*0x3ff (8M),will enter the following if ,and change the BlockSize,then ,the FQ Depth == DMX_MAX_AVFQ_DESC(0x3ff)
    3.usually ,I think will this will not happen.(in StartAVPlay ,we set vedio channel buffer size as 0x300000)
    */
    str = (HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType) ? "Aud" : "Vid";
    BlockSize = (HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType) ? DMX_FQ_AUD_BLOCK_SIZE : DMX_FQ_VID_BLOCK_SIZE;
    if ((ChanInfo->ChanBufSize / DMX_MAX_AVFQ_DESC) > BlockSize)
    {
        BlockSize = (ChanInfo->ChanBufSize/ DMX_MAX_AVFQ_DESC) - ((ChanInfo->ChanBufSize / DMX_MAX_AVFQ_DESC) % 4);
        BlockSize = (BlockSize > DMX_MAX_BLOCK_SIZE) ? DMX_MAX_BLOCK_SIZE : BlockSize;
    }

    snprintf(BufName, sizeof(BufName), "DMX_%sBuf[%u]", str, ChanInfo->ChanId);
    if (HI_SUCCESS != HI_DRV_SMMU_AllocAndMap(BufName, ChanInfo->ChanBufSize , 0, &MmuBuf))
    {
        HI_ERR_DEMUX("mmu memory allocate failed, BufSize=0x%x\n", ChanInfo->ChanBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out0;
    }

    BUG_ON(MmuBuf.u32Size != ChanInfo->ChanBufSize);

    FqInfo->BufVirAddr   = MmuBuf.pu8StartVirAddr;
    FqInfo->BufPhyAddr   = MmuBuf.u32StartSmmuAddr;
    FqInfo->BufSize      = MmuBuf.u32Size;
    FqInfo->BufFlag      = DMX_MMU_BUF;
    FqInfo->BlockSize    = BlockSize;

    FqDepth     = FqInfo->BufSize / FqInfo->BlockSize + 1;
    FqBufSize   = (FqDepth * DMX_FQ_DESC_SIZE + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;

    OqDepth     = (FqDepth < DMX_OQ_DEPTH) ? (FqDepth - 1) : DMX_OQ_DEPTH;
    OqBufSize   = OqDepth * DMX_OQ_DESC_SIZE;

    snprintf(BufName, sizeof(BufName), "DMX_%sDscBuf[%u]", str, ChanInfo->ChanId);
    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(BufName, MMZ_OTHERS, FqBufSize + OqBufSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("memory allocate failed, BufSize=0x%x\n", FqBufSize + OqBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out1;
    }

    FqInfo->FqDescSetBase    = (FQ_DescInfo_S*)MmzBuf.pu8StartVirAddr;
    FqInfo->FqDescSetPhy    = MmzBuf.u32StartPhyAddr;
    FqInfo->FQDepth      = FqDepth;

    OqInfo->OqDescSetBase    = (OQ_DescInfo_S*)((HI_U8*)FqInfo->FqDescSetBase + FqBufSize);
    OqInfo->OqDescSetPhy    = FqInfo->FqDescSetPhy + FqBufSize;
    OqInfo->OQDepth      = OqDepth;

    return HI_SUCCESS;

out1:
    MmuBuf.pu8StartVirAddr = FqInfo->BufVirAddr;
    MmuBuf.u32StartSmmuAddr = FqInfo->BufPhyAddr;

    HI_DRV_SMMU_UnmapAndRelease(&MmuBuf);
out0:
    return ret;
}

static HI_VOID DmxFreeNonSecureAVBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo)
{
    DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];
    SMMU_BUFFER_S MmuBuf = {0};
    MMZ_BUFFER_S MmzBuf = {0};

    /* release desc buff. */
    MmzBuf.pu8StartVirAddr = (HI_U8*)FqInfo->FqDescSetBase;
    MmzBuf.u32StartPhyAddr = FqInfo->FqDescSetPhy ;
    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);

    /* release data buff. */
    MmuBuf.pu8StartVirAddr = FqInfo->BufVirAddr;
    MmuBuf.u32StartSmmuAddr = FqInfo->BufPhyAddr;
    HI_DRV_SMMU_UnmapAndRelease(&MmuBuf);
}

#else

static HI_S32 DmxAllocSecureAVBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo)
{
    HI_ERR_DEMUX("not support alloc secure mmz av buf.\n");
    return HI_ERR_DMX_NOT_SUPPORT;
}

static HI_VOID DmxFreeSecureAVBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo)
{
}

static HI_S32 DmxFixSecureAudBuf(DMX_ChanInfo_S *ChanInfo, DMX_Stream_S *EsData)
{
    return HI_ERR_DMX_NOT_SUPPORT;
}

static HI_S32 DmxAllocNonSecureAVBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo)
{
    HI_S32 ret = HI_FAILURE;
    DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];
    HI_U32 FqDepth, FqBufSize, OqDepth, OqBufSize, BlockSize;
    HI_CHAR *str;
    HI_CHAR BufName[32];
    MMZ_BUFFER_S MmzBuf = {0};

    /*
    1.if buffer size > 1208*0x3ff (1M),will enter the following if ,and change the BlockSize,then ,the FQ Depth == DMX_MAX_AVFQ_DESC(0x3ff)
    2.if buffer size > 8192*0x3ff (8M),will enter the following if ,and change the BlockSize,then ,the FQ Depth == DMX_MAX_AVFQ_DESC(0x3ff)
    3.usually ,I think will this will not happen.(in StartAVPlay ,we set vedio channel buffer size as 0x300000)
    */
    str = (HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType) ? "Aud" : "Vid";
    BlockSize = (HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType) ? DMX_FQ_AUD_BLOCK_SIZE : DMX_FQ_VID_BLOCK_SIZE;
    if ((ChanInfo->ChanBufSize / DMX_MAX_AVFQ_DESC) > BlockSize)
    {
        BlockSize = (ChanInfo->ChanBufSize/ DMX_MAX_AVFQ_DESC) - ((ChanInfo->ChanBufSize / DMX_MAX_AVFQ_DESC) % 4);
        BlockSize = (BlockSize > DMX_MAX_BLOCK_SIZE) ? DMX_MAX_BLOCK_SIZE : BlockSize;
    }

    FqDepth     = ChanInfo->ChanBufSize / BlockSize + 1;
    FqBufSize   = (FqDepth * DMX_FQ_DESC_SIZE + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;

    OqDepth     = (FqDepth < DMX_OQ_DEPTH) ? (FqDepth - 1) : DMX_OQ_DEPTH;
    OqBufSize   = OqDepth * DMX_OQ_DESC_SIZE;

    snprintf(BufName, sizeof(BufName), "DMX_%sBuf[%u]", str, ChanInfo->ChanId);
    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(BufName, MMZ_OTHERS, ChanInfo->ChanBufSize + FqBufSize + OqBufSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("memory allocate failed, BufSize=0x%x\n", ChanInfo->ChanBufSize + FqBufSize + OqBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out0;
    }

    FqInfo->BufVirAddr   = MmzBuf.pu8StartVirAddr;
    FqInfo->BufPhyAddr   = MmzBuf.u32StartPhyAddr;
    FqInfo->BufSize      = ChanInfo->ChanBufSize;
    FqInfo->BufFlag      = DMX_MMZ_BUF;
    FqInfo->BlockSize    = BlockSize;

    FqInfo->FqDescSetBase    = (FQ_DescInfo_S*)(FqInfo->BufVirAddr + FqInfo->BufSize);
    FqInfo->FqDescSetPhy     = FqInfo->BufPhyAddr + FqInfo->BufSize;
    FqInfo->FQDepth          = FqDepth;

    OqInfo->OqDescSetBase    = (OQ_DescInfo_S*)((HI_U8*)FqInfo->FqDescSetBase + FqBufSize);
    OqInfo->OqDescSetPhy     = FqInfo->FqDescSetPhy + FqBufSize;
    OqInfo->OQDepth          = OqDepth;

    ret = HI_SUCCESS;

out0:
    return ret;
}

static HI_VOID DmxFreeNonSecureAVBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo)
{
    DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];
    MMZ_BUFFER_S MmzBuf = {0};

    /* release data buff. */
    MmzBuf.pu8StartVirAddr = FqInfo->BufVirAddr;
    MmzBuf.u32StartPhyAddr = FqInfo->BufPhyAddr;

    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);
}
#endif

static HI_S32 DmxAllocAVBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo)
{
    HI_S32 ret = HI_FAILURE;

    switch(ChanInfo->ChanSecure)
    {
        case HI_UNF_DMX_SECURE_MODE_NONE:
            ret = DmxAllocNonSecureAVBuf(DmxSet, ChanInfo);
            break;
        case HI_UNF_DMX_SECURE_MODE_TEE:
            ret = DmxAllocSecureAVBuf(DmxSet, ChanInfo);
            break;
        default:
            BUG();
    }

    return ret;
}

static HI_VOID DmxFreeAVBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo)
{
    switch(ChanInfo->ChanSecure)
    {
        case HI_UNF_DMX_SECURE_MODE_NONE:
            DmxFreeNonSecureAVBuf(DmxSet, ChanInfo);
            break;
        case HI_UNF_DMX_SECURE_MODE_TEE:
            DmxFreeSecureAVBuf(DmxSet, ChanInfo);
            break;
        default:
            BUG();
    }
}

static HI_S32 DmxAllocOqDscBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo)
{
    HI_S32 ret = HI_FAILURE;
    DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];
    HI_U32 OqDepth, OqBufSize;
    HI_CHAR BufName[32];
    MMZ_BUFFER_S MmzBuf = {0};

    /*OqDepth usually equal FqInfo->FQDepth - 1, except DmxPoolBufSize > DmxBlockSize *  DMX_OQ_DEPTH (5K*0X3ff) > 5M*/
    OqDepth     = FqInfo->FQDepth < DMX_OQ_DEPTH ? (FqInfo->FQDepth - 1) : DMX_OQ_DEPTH;
    OqBufSize   = OqDepth * DMX_OQ_DESC_SIZE;

    snprintf(BufName, sizeof(BufName), "DMX_OqDscBuf[%u]", ChanInfo->ChanId);

    ret = HI_DRV_MMZ_AllocAndMap(BufName, MMZ_OTHERS, OqBufSize, 0, &MmzBuf);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("memory allocate failed, BufSize=0x%x\n", OqBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out;
    }

    OqInfo->OqDescSetPhy    = MmzBuf.u32StartPhyAddr;
    OqInfo->OqDescSetBase    = (OQ_DescInfo_S*)MmzBuf.pu8StartVirAddr;
    OqInfo->OQDepth      = OqDepth;

    ret = HI_SUCCESS;

out:
    return ret;
}

static HI_VOID DmxFreeOqDscBuf(Dmx_Set_S *DmxSet, DMX_ChanInfo_S *ChanInfo)
{
    DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    MMZ_BUFFER_S MmzBuf = {0};

    MmzBuf.pu8StartVirAddr = (HI_U8*)OqInfo->OqDescSetBase;
    MmzBuf.u32StartPhyAddr = OqInfo->OqDescSetPhy;

    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);
}

static HI_S32 DmxConfigurePlyChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr)
{
    HI_S32 ret = HI_FAILURE;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];
    DMX_OQ_Info_S *OqInfo = HI_NULL;
    HI_U32 OqId = DMX_INVALID_OQ_ID;
    HI_UNF_DMX_CHAN_ATTR_S Old = {0};

    if (DMX_INVALID_OQ_ID != ChanInfo->ChanOqId && HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode)
    {
        HI_ERR_DEMUX("this channel(%d) is already a play channel.\n", ChanId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    /* save and update play channel key configs. */
    Old.u32BufSize = ChanInfo->ChanBufSize;
    Old.enChannelType = ChanInfo->ChanType;
    Old.enCRCMode = ChanInfo->ChanCrcMode;
    Old.enOutputMode = ChanInfo->ChanOutMode;
    Old.enSecureMode = ChanInfo->ChanSecure;

    ChanInfo->ChanBufSize   = ChanAttr->u32BufSize;
    ChanInfo->ChanType      = ChanAttr->enChannelType;
    ChanInfo->ChanCrcMode = ChanAttr->enCRCMode;
    ChanInfo->ChanOutMode |= HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;
    ChanInfo->ChanSecure = ChanAttr->enSecureMode;

    ret = DmxOqAcquire(DmxSet, ChanInfo->DmxId, ChanInfo->ChanSecure, &OqId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    OqInfo = &DmxSet->DmxOqInfo[OqId];
    OqInfo->enOQBufMode    = DMX_OQ_MODE_PLAY;
    OqInfo->u32AttachId      = ChanInfo->ChanId;

    ChanInfo->ChanOqId      = OqId;

    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY == (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode))
    {
        /* video and audio channel */
        if ((HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType) || (HI_UNF_DMX_CHAN_TYPE_VID == ChanInfo->ChanType))
        {
            HI_U32 FqId;

            ret = DmxFqAcquire(DmxSet, &FqId);
            if (HI_SUCCESS != ret)
            {
                goto out2;
            }

            DmxFqStop(DmxSet, FqId);

            OqInfo->u32FQId = FqId;

            ret = DmxAllocAVBuf(DmxSet, ChanInfo);
            if (HI_SUCCESS != ret)
            {
                DmxFqRelease(DmxSet, FqId);
                goto out2;
            }
        }
        else /* general channel */
        {

            OqInfo->u32FQId = DMX_FQ_COMMOM;

            ret = DmxAllocOqDscBuf(DmxSet, ChanInfo);
            if (HI_SUCCESS != ret)
            {
                goto out2;
            }
        }
    }

    DmxSetChannel(DmxSet, ChanId);

    return HI_SUCCESS;

out2:
    DmxOqRelease(DmxSet, ChanInfo->DmxId, OqId);
out1:
    ChanInfo->ChanBufSize = Old.u32BufSize;
    ChanInfo->ChanType = Old.enChannelType;
    ChanInfo->ChanCrcMode = Old.enCRCMode;
    ChanInfo->ChanOutMode = Old.enOutputMode;
    ChanInfo->ChanSecure  = Old.enSecureMode;
out0:
    return ret;
}

static HI_S32 DmxConfigureRecChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr)
{
    HI_S32 ret = HI_FAILURE;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if (DMX_INVALID_OQ_ID != ChanInfo->ChanRecOqId && HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & ChanInfo->ChanOutMode)
    {
        HI_ERR_DEMUX("this channel(%d) is already a rec channel.\n", ChanId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    ChanInfo->ChanOutMode |= HI_UNF_DMX_CHAN_OUTPUT_MODE_REC;

    return HI_SUCCESS;

out:
    return ret;
}
/*
 * hold channel instance lock before call this function.
 */
static HI_S32 DmxConfigureChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr)
{
    HI_S32 ret = HI_FAILURE;

    switch (ChanAttr->enOutputMode)
    {
        case HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY:
        {
            ret = DmxConfigurePlyChannel(DmxSet, ChanId, ChanAttr);
            if (HI_SUCCESS != ret)
            {
                goto out;
            }

            break;
        }

        case HI_UNF_DMX_CHAN_OUTPUT_MODE_REC:
        {
            ret = DmxConfigureRecChannel(DmxSet, ChanId, ChanAttr);
            if (HI_SUCCESS != ret)
            {
                goto out;
            }

            break;
        }

        default:
            ret = HI_ERR_DMX_INVALID_PARA;
            goto out;
    }

    return HI_SUCCESS;

out:
    return ret;
}

static HI_S32 DmxCreateChannel(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr)
{
    HI_S32 ret = HI_FAILURE;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    BUG_ON(DmxSet != GetDmxSetByDmxid(DmxId));
    BUG_ON(!mutex_is_locked(&ChanInfo->LockChn));

    ChanInfo->DmxId           = DmxId;
    ChanInfo->ChanId          = ChanId;
    ChanInfo->ChanPid         = DMX_INVALID_PID;
    ChanInfo->Owner           = task_tgid_nr(current);
    ChanInfo->ChanBufSize   = ChanAttr->u32BufSize;
    ChanInfo->ChanType      = ChanAttr->enChannelType;
    ChanInfo->ChanCrcMode   = ChanAttr->enCRCMode;
    ChanInfo->ChanOutMode   = ChanAttr->enOutputMode;
    ChanInfo->ChanStatus    = HI_UNF_DMX_CHAN_CLOSE;
    ChanInfo->ChanSecure      = ChanAttr->enSecureMode;
    ChanInfo->KeyId            = DMX_INVALID_KEY_ID;
    ChanInfo->ChanOqId      = DMX_INVALID_OQ_ID;
    ChanInfo->ChanRecOqId  = DMX_INVALID_OQ_ID;
    atomic_set(&ChanInfo->ChanRecShareCnt, 0);
    ChanInfo->SaveEsHandle = HI_NULL;
    ChanInfo->u32TotalAcq   = 0;
    ChanInfo->u32HitAcq     = 0;
    ChanInfo->u32Release    = 0;
    ChanInfo->LastPts          = INVALID_PTS;
    ChanInfo->ChanEosFlag   = HI_FALSE;
    atomic_set(&ChanInfo->FilterCount, 0);
    memset((HI_U8 *)&ChanInfo->stLastControl, 0, sizeof(Disp_Control_t));
    ChanInfo->u32PesLenErr  = 0;
    ChanInfo->u32CRCErr = 0;
    ChanInfo->u32TEIErr = 0;
    ChanInfo->u32DropCnt = 0;
    ChanInfo->u32CCDiscErr = 0;

    ret = DmxConfigureChannel(DmxSet, ChanId, ChanAttr);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    DmxSetChannel(DmxSet, ChanId);

    return HI_SUCCESS;

out:
    ChanInfo->DmxId = DMX_INVALID_DEMUX_ID;
    return ret;
}

static HI_S32 DmxOpenPlyChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];
    DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    DMX_DmxInfo_S  *DmxInfo = DmxSet->Ops->GetDmxInfo(DmxSet, ChanInfo->DmxId);

    if (unlikely(!(HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode)))
    {
        HI_ERR_DEMUX("this channel(%d) is not an play channel.\n", ChanId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (unlikely(HI_UNF_DMX_CHAN_PLAY_EN & ChanInfo->ChanStatus))
    {
        HI_WARN_DEMUX("play channel(%d) is already opened.\n", ChanId);
        return HI_SUCCESS;
    }

    DmxHalResetChannelCounter(DmxSet, ChanId);

    DmxHalSetChannelPlayDmxid(DmxSet, ChanId, ChanInfo->DmxId);

    /* only SEC & ECM_EMM channel must attach filter */
    if ((ChanInfo->ChanType != HI_UNF_DMX_CHAN_TYPE_SEC) && (ChanInfo->ChanType != HI_UNF_DMX_CHAN_TYPE_ECM_EMM))
    {
        DmxHalSetChannelFltMode(DmxSet, ChanId, DMX_DISABLE);

        if (ChanInfo->ChanType == HI_UNF_DMX_CHAN_TYPE_PES)
        {
            ChanInfo->u32PesBlkCnt = 0;
        }
    }
    else
    {
        DmxHalSetChannelFltMode(DmxSet, ChanId, DMX_ENABLE);
    }

    /* enable bp ram port for av channel */
    if ((ChanInfo->ChanType == HI_UNF_DMX_CHAN_TYPE_AUD) || (ChanInfo->ChanType == HI_UNF_DMX_CHAN_TYPE_VID))
    {
        switch(DmxInfo->PortMode)
        {
            case DMX_PORT_MODE_RAM:
                DmxHalAttachIPBPFQ(DmxSet, DmxInfo->PortId, OqInfo->u32FQId);
                break;
            case DMX_PORT_MODE_RMX:
                RmxEnFqBPIp(DmxSet, DmxInfo->PortId, OqInfo->u32FQId);
                break;
            default:
                break;
        }
    }

    /* start data transfer */
    DmxOqStart(DmxSet, ChanInfo->ChanOqId);

    if ((ChanInfo->ChanType == HI_UNF_DMX_CHAN_TYPE_AUD) || (ChanInfo->ChanType == HI_UNF_DMX_CHAN_TYPE_VID))
    {
        DmxFqStart(DmxSet, OqInfo->u32FQId);
    }

    ChanInfo->ChanStatus |= HI_UNF_DMX_CHAN_PLAY_EN;

    ChanInfo->u32AcqTimeInterval = 0;
    ChanInfo->u32RelTimeInterval = 0;

    HI_DRV_SYS_GetTimeStampMs(&ChanInfo->u32AcqTime);
    ChanInfo->u32RelTime = ChanInfo->u32AcqTime;

    return HI_SUCCESS;
}

static HI_S32 DmxOpenRecChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if (unlikely(!(HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & ChanInfo->ChanOutMode)))
    {
        HI_ERR_DEMUX("this channel(%d) is not an rec channel.\n", ChanId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (unlikely(HI_UNF_DMX_CHAN_REC_EN & ChanInfo->ChanStatus))
    {
        HI_WARN_DEMUX("rec channel(%d) is already opened.\n", ChanId);
        return HI_SUCCESS;
    }

    if (ChanInfo->ChanRecOqId < DmxSet->DmxOqCnt)
    {
        DmxHalSetChannelRecBufId(DmxSet, ChanInfo->ChanId, ChanInfo->ChanRecOqId);

        DmxHalSetChannelRecDmxid(DmxSet, ChanInfo->ChanId, ChanInfo->DmxId);
    }
#ifdef DMX_SCD_VERSION_2
    else
    {
        HI_ERR_DEMUX("Invalid record OQ id(%d).\n", ChanInfo->ChanRecOqId);
        return HI_ERR_DMX_INVALID_REC_CHAN;
    }
#endif

    ChanInfo->ChanStatus |= HI_UNF_DMX_CHAN_REC_EN;

    return HI_SUCCESS;
}

static HI_S32 DmxDeConfigurePlyChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];
    DMX_OQ_Info_S  *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];

    if (unlikely(!(HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode)))
    {
        HI_ERR_DEMUX("this channel(%d) is not an play channel.\n", ChanId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    DmxHalSetChannelPlayBufId(DmxSet, ChanId, DMX_INVALID_OQ_ID);

    if ((HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType) || (HI_UNF_DMX_CHAN_TYPE_VID == ChanInfo->ChanType))
    {
        DmxFreeAVBuf(DmxSet, ChanInfo);

        DmxFqRelease(DmxSet, OqInfo->u32FQId);
    }
    else
    {
        DmxFreeOqDscBuf(DmxSet, ChanInfo);
    }

    DmxOqRelease(DmxSet, ChanInfo->DmxId, ChanInfo->ChanOqId);

    ChanInfo->ChanOutMode &= ~HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;

    return HI_SUCCESS;
}

static HI_S32 DmxDeConfigureRecChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if (unlikely(!(HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & ChanInfo->ChanOutMode)))
    {
        HI_ERR_DEMUX("this channel(%d) is not an rec channel.\n", ChanId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    ChanInfo->ChanOutMode &= ~HI_UNF_DMX_CHAN_OUTPUT_MODE_REC;

    return HI_SUCCESS;

out:
    return ret;
}

static HI_S32 DmxClosePlyChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];
    DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    DMX_FLUSH_TYPE_E FlushType;

    if (unlikely(!(HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode)))
    {
        HI_ERR_DEMUX("this channel(%d) is not an play channel.\n", ChanId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (unlikely(!(HI_UNF_DMX_CHAN_PLAY_EN & ChanInfo->ChanStatus)))
    {
        HI_WARN_DEMUX("play channel(%d) is not opened.\n", ChanId);
        return HI_SUCCESS;
    }

    ChanInfo->ChanStatus    &= ~HI_UNF_DMX_CHAN_PLAY_EN;
    ChanInfo->u32TotalAcq   = 0;
    ChanInfo->u32HitAcq     = 0;
    ChanInfo->u32Release    = 0;
    ChanInfo->u32PesLength  = 0;
    ChanInfo->ChanEosFlag   = HI_FALSE;

    // close channel
    DmxHalSetChannelPlayDmxid(DmxSet, ChanId, DMX_INVALID_DEMUX_ID);

    if ((HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType) || (HI_UNF_DMX_CHAN_TYPE_VID == ChanInfo->ChanType))
    {
        DmxFqStop(DmxSet, OqInfo->u32FQId);
    }

    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY == ChanInfo->ChanOutMode)
    {
        FlushType = DMX_FLUSH_TYPE_REC_PLAY;
    }
    else if (HI_UNF_DMX_CHAN_OUTPUT_MODE_REC == ChanInfo->ChanOutMode)
    {
        FlushType = DMX_FLUSH_TYPE_REC;
    }
    else
    {
        FlushType = DMX_FLUSH_TYPE_REC_PLAY;
    }

    DmxResetChannel(DmxSet, ChanId, FlushType);

    DmxOqStop(DmxSet, ChanInfo->ChanOqId);

    if ((HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType) || (HI_UNF_DMX_CHAN_TYPE_VID == ChanInfo->ChanType))
    {
        DMX_DmxInfo_S *DmxInfo = DmxSet->Ops->GetDmxInfo(DmxSet, ChanInfo->DmxId);

        switch(DmxInfo->PortMode)
        {
            case DMX_PORT_MODE_RAM:
            {
                DmxHalDetachIPBPFQ(DmxSet, DmxInfo->PortId, OqInfo->u32FQId);
                DmxHalClrIPFqBPStatus(DmxSet, DmxInfo->PortId, OqInfo->u32FQId);
                break;
            }

            case DMX_PORT_MODE_RMX:
            {
                RmxDisFqBPIp(DmxSet, DmxInfo->PortId, OqInfo->u32FQId);
                break;
            }

            default:
                break;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 DmxCloseRecChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if (unlikely(!(HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & ChanInfo->ChanOutMode)))
    {
        HI_ERR_DEMUX("this channel(%d) is not an rec channel.\n", ChanId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (unlikely(!(HI_UNF_DMX_CHAN_REC_EN & ChanInfo->ChanStatus)))
    {
        HI_WARN_DEMUX("rec channel(%d) is not opened.\n", ChanId);
        return HI_SUCCESS;
    }

    ChanInfo->ChanStatus &= ~HI_UNF_DMX_CHAN_REC_EN;

    DmxHalSetChannelRecBufId(DmxSet, ChanInfo->ChanId, DMX_INVALID_OQ_ID);

    DmxHalSetChannelRecDmxid(DmxSet, ChanInfo->ChanId, DMX_INVALID_DEMUX_ID);

    return HI_SUCCESS;
}


/*
 * hold channel instance lock before call this function.
 */
static HI_S32 DmxDeConfigureChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_UNF_DMX_CHAN_OUTPUT_MODE_E RmvMode)
{
    HI_S32 ret = HI_FAILURE;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    switch (RmvMode)
    {
        case HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY:
        {
            if (HI_UNF_DMX_CHAN_PLAY_EN & ChanInfo->ChanStatus)
            {
                ret = DmxClosePlyChannel(DmxSet, ChanId);
                if (HI_SUCCESS != ret)
                {
                    goto out;
                }
            }

            ret = DmxDeConfigurePlyChannel(DmxSet, ChanId);
            if (HI_SUCCESS != ret)
            {
                goto out;
            }
            break;
        }

        case HI_UNF_DMX_CHAN_OUTPUT_MODE_REC:
        {
            if (HI_UNF_DMX_CHAN_REC_EN & ChanInfo->ChanStatus)
            {
                ret = DmxCloseRecChannel(DmxSet, ChanId);
                if (HI_SUCCESS != ret)
                {
                    goto out;
                }
            }

            ret = DmxDeConfigureRecChannel(DmxSet, ChanId);
            if (HI_SUCCESS != ret)
            {
                goto out;
            }

            break;
        }

        default:
            ret = HI_ERR_DMX_INVALID_PARA;
            goto out;
    }

    return HI_SUCCESS;

out:
    return ret;
}


static HI_S32 DmxAddChannelFunc(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 Pid, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr, DMX_MMZ_BUF_S *ChanBuf)
{
    HI_S32 ret = HI_FAILURE;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];
    HI_UNF_DMX_CHAN_ATTR_S ValidChnAttr = {0};

    /* double checking */
    if (Pid != ChanInfo->ChanPid)
    {
        goto out0;
    }

    if (ChanInfo->Owner != task_tgid_nr(current))
    {
        HI_ERR_DEMUX("Not allow add channel(%d) func(P&R) at different process.\n", ChanId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    /* verify and tune create channel attrs */
    ret = DmxTrimChnAttrs(ChanAttr, &ValidChnAttr);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    if (ChanInfo->ChanSecure != ValidChnAttr.enSecureMode)
    {
        HI_ERR_DEMUX("Not allow add channel(%d) func(P&R) with different secure mode.\n", ChanId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    ret = DmxConfigureChannel(DmxSet, ChanId, &ValidChnAttr);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    if (ChanBuf)
    {
        DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
        DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];

        ChanBuf->VirAddr      = FqInfo->BufVirAddr;
        ChanBuf->PhyAddr      = FqInfo->BufPhyAddr;
        ChanBuf->Size         = FqInfo->BufSize;
        ChanBuf->Flag         = FqInfo->BufFlag;
    }

    if (unlikely(HI_UNF_DMX_SECURE_MODE_TEE == ChanInfo->ChanSecure))
    {
        if (ChanBuf)
        {
            DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
            DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];

            if (HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType)
            {
                ChanBuf->VirAddr      = FqInfo->ShadowBufVirAddr;
                ChanBuf->PhyAddr      = FqInfo->ShadowBufPhyAddr;
                ChanBuf->Size         = FqInfo->ShadowBufSize;
                ChanBuf->Flag         = FqInfo->ShadowBufFlag;
            }
        }
    }

out0:
    return ret;
}

static HI_S32 DmxDelChannelFunc(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_UNF_DMX_CHAN_OUTPUT_MODE_E RmvMode)
{
    return DmxDeConfigureChannel(DmxSet, ChanId, RmvMode);
}

HI_S32 DmxCloseChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode)
    {
        ret = DmxClosePlyChannel(DmxSet, ChanId);
        if (HI_SUCCESS != ret)
        {
            goto out;
        }
    }

    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & ChanInfo->ChanOutMode)
    {
        ret = DmxCloseRecChannel(DmxSet, ChanId);
        if (HI_SUCCESS != ret)
        {
            goto out;
        }
    }

    ret = HI_SUCCESS;

out:
    return ret;
}

static HI_S32 DmxDestroyChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    HI_U32 i;
    HI_S32 ret = HI_FAILURE;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if (HI_UNF_DMX_CHAN_CLOSE != ChanInfo->ChanStatus)
    {
        ret = DmxCloseChannel(DmxSet, ChanId);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_DEMUX("close chan %d failed 0x%x\n", ChanId, ret);

            return ret;
        }
    }

    // detach all filters attatched to this channel
    if (atomic_read(&ChanInfo->FilterCount))
    {
        for (i = 0; i < DmxSet->DmxFilterCnt; i++)
        {
            DMX_FilterInfo_S *Filter = &DmxSet->DmxFilterInfo[i];

            if (ChanId == Filter->ChanId)
            {
                if (HI_SUCCESS == DmxGetFltInstance(DmxSet, Filter->FilterId))
                {
                    if (ChanId == Filter->ChanId)
                    {
                        DmxHalDetachFilter(DmxSet, Filter->FilterId, ChanId);
                    }

                    Filter->ChanId = DMX_INVALID_CHAN_ID;

                    DmxPutFltInstance(DmxSet, Filter->FilterId);
                }
            }
        }

        atomic_set(&ChanInfo->FilterCount, 0);
    }

#ifdef DMX_DESCRAMBLER_SUPPORT
    //detatch all keys attatched to this channel
    if (DMX_INVALID_KEY_ID != ChanInfo->KeyId)
    {
        __DMX_OsiDescramblerDetach(ChanInfo->DmxId, ChanInfo->KeyId, ChanId);

        ChanInfo->KeyId = DMX_INVALID_KEY_ID;
    }
#endif

    //delete the buffer from the channel
    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_REC == (HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & ChanInfo->ChanOutMode))
    {
        DmxDeConfigureRecChannel(DmxSet, ChanId);

        DmxHalSetShareRecChannel(DmxSet, ChanId, HI_FALSE);
    }

    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY == (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode))
    {
        DmxDeConfigurePlyChannel(DmxSet, ChanId);
    }

    if (HI_UNF_DMX_CHAN_TYPE_POST == ChanInfo->ChanType)
    {
        DmxHalSetChannelTsPostMode(DmxSet, ChanId, DMX_DISABLE);
    }

    DmxHalSetChannelPid(DmxSet, ChanId, DMX_INVALID_PID);

    ChanInfo->DmxId     = DMX_INVALID_DEMUX_ID;
    ChanInfo->ChanId    = DMX_INVALID_CHAN_ID;
    ChanInfo->ChanPid   = DMX_INVALID_PID;

    return HI_SUCCESS;
}

static HI_S32 DmxDestroyPlyChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if (unlikely(HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY_REC == ChanInfo->ChanOutMode))
    {
        ret = DmxDelChannelFunc(DmxSet, ChanId, HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY);
    }
    else
    {
        if (unlikely(!(HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode)))
        {
            HI_ERR_DEMUX("this channel(%d) is not an play channel.\n", ChanId);
            ret = HI_ERR_DMX_INVALID_PARA;
            goto out;
        }

        ret = DmxChnRelease(ChanInfo->DmxId, ChanId);
    }

out:
    return ret;
}

static HI_S32 DmxDestroyRecChannel(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if (unlikely(HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY_REC == ChanInfo->ChanOutMode))
    {
        ret = DmxDelChannelFunc(DmxSet, ChanId, HI_UNF_DMX_CHAN_OUTPUT_MODE_REC);
    }
    else
    {
        if (unlikely(!(HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & ChanInfo->ChanOutMode)))
        {
            HI_ERR_DEMUX("this channel(%d) is not an rec channel.\n", ChanId);
            ret = HI_ERR_DMX_INVALID_PARA;
            goto out;
        }

        ret = DmxChnRelease(ChanInfo->DmxId, ChanId);
    }

out:
    return ret;
}

/***********************************************************************************
* Function      : DMX_OsiCreateChannel
* Description   : create a channel
* Input         : DmxId
* Output        :
* Return        : HI_SUCCESS:     success
*                 HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiCreateChannel(HI_U32 DmxId, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr, DMX_MMZ_BUF_S *ChanBuf, HI_U32 *ChanId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    HI_UNF_DMX_CHAN_ATTR_S ValidChnAttr = {0};

    if (!DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    /* verify and tune create channel attrs */
    ret = DmxTrimChnAttrs(ChanAttr, &ValidChnAttr);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    mutex_lock(&DmxSet->LockAllChn);

    ret = DmxChnAcquire(DmxId, &ValidChnAttr, ChanId, ChanBuf);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

out1:
    mutex_unlock(&DmxSet->LockAllChn);
out0:
    return ret;
}

HI_S32 DMX_OsiAddChannelFunc(HI_U32 DmxId, HI_U32 ChanId, HI_U32 Pid, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr, DMX_MMZ_BUF_S *ChanBuf)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = DmxAddChannelFunc(DmxSet, ChanId, Pid, ChanAttr, ChanBuf);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

out1:
    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_S32 DMX_OsiDelChannelFunc(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_CHAN_OUTPUT_MODE_E RmvMode)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = DmxDelChannelFunc(DmxSet, ChanId, RmvMode);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

out1:
    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

/***********************************************************************************
* Function      : DMX_OsiOpenChannel
* Description   : open a channel and start to work
* Input         : ChanId
* Output        :
* Return        : HI_SUCCESS:     success
*                 HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiOpenPlyChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    HI_S32 ret;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = DmxOpenPlyChannel(DmxSet, ChanId);

    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_S32 DMX_OsiOpenRecChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    HI_S32 ret;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = DmxOpenRecChannel(DmxSet, ChanId);

    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_S32 DMX_OsiOpenChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    HI_S32 ret;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode)
    {
        ret = DmxOpenPlyChannel(DmxSet, ChanId);
        if (HI_SUCCESS != ret)
        {
            goto out1;
        }
    }

    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & ChanInfo->ChanOutMode)
    {
        ret = DmxOpenRecChannel(DmxSet, ChanId);
        if (HI_SUCCESS != ret)
        {
            goto out1;
        }
    }

out1:
    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_S32 DMX_OsiClosePlyChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    HI_S32 ret;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = DmxClosePlyChannel(DmxSet, ChanId);

    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_S32 DMX_OsiCloseRecChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    HI_S32 ret;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = DmxCloseRecChannel(DmxSet, ChanId);

    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}
/***********************************************************************************
* Function      : DMX_OsiCloseChannel
* Description   : close a channel and stop working
* Input         : ChanId
* Output        :
* Return        : HI_SUCCESS:     success
*                 HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiCloseChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    HI_S32 ret;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = DmxCloseChannel(DmxSet, ChanId);

    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_S32 DMX_OsiDestroyPlyChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    HI_S32 ret;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    ret = DmxDestroyPlyChannel(DmxSet, ChanId);

    DmxPutChnInstanceEx(DmxSet, DmxId, ChanId);
out:
    return ret;
}

HI_S32 DMX_OsiDestroyRecChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    HI_S32 ret;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    ret = DmxDestroyRecChannel(DmxSet, ChanId);

    DmxPutChnInstanceEx(DmxSet, DmxId, ChanId);
out:
    return ret;
}

/* notify vdec that the es buffer has been released */
#include "drv_vdec_ext.h"
#include "hi_drv_module.h"

static HI_VOID DMX_ReportEsRelease(HI_HANDLE handle)
{
    HI_S32 ret = HI_FAILURE;
    VDEC_EXPORT_FUNC_S *pVdecFunc = NULL;

    ret = HI_DRV_MODULE_GetFunction(HI_ID_VDEC, (HI_VOID**)&pVdecFunc);
    
    if ((HI_SUCCESS == ret) && (HI_NULL != pVdecFunc->pfnVDEC_ReportEsRels))
    {
        ret = pVdecFunc->pfnVDEC_ReportEsRels(handle);
        if (HI_SUCCESS != ret)
        {
            HI_WARN_DEMUX("Set VDEC_CID_INFORM_DEMUX_RELEASE Failed!\n");
        }
    }
    else
    {
        HI_WARN_DEMUX("Get pVfmwFunc Failed!\n");
    }

    return;
}
/********************************************************/

HI_S32 DMX_OsiDestroyChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    HI_S32 ret;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    /* notify vdec that the es buffer has been released */
    DMX_ReportEsRelease(DMX_CHANHANDLE(DmxId, ChanId));

    ret = DmxChnRelease(DmxId, ChanId);

    DmxPutChnInstanceEx(DmxSet, DmxId, ChanId);
out:
    return ret;
}

/***********************************************************************************
* Function      : DMX_OsiGetChannelAttr
* Description   : Get Channel Attr
* Input         : ChanId, ChanAttr
* Output        :
* Return        : HI_SUCCESS:     success
*                 HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiGetChannelAttr(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr)
{
    HI_S32 ret = HI_SUCCESS;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    ChanAttr->u32BufSize    = ChanInfo->ChanBufSize;
    ChanAttr->enChannelType = ChanInfo->ChanType;
    ChanAttr->enCRCMode     = ChanInfo->ChanCrcMode;
    ChanAttr->enOutputMode  = ChanInfo->ChanOutMode;
    ChanAttr->enSecureMode = ChanInfo->ChanSecure;

    DmxPutChnInstance(DmxSet, ChanId);
out:
    return ret;
}

/***********************************************************************************
* Function      : DMX_OsiSetChannelAttr
* Description   : Set Channel Attr
* Input         : ChanId, ChanAttr
* Output        :
* Return        : HI_SUCCESS:     success
*                 HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiSetChannelAttr(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_CHAN_ATTR_S *ChanAttr)
{
    HI_S32 ret = HI_SUCCESS;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if ((ChanAttr->u32BufSize    != ChanInfo->ChanBufSize)
        || (ChanAttr->enChannelType != ChanInfo->ChanType)
        || (ChanAttr->enOutputMode  != ChanInfo->ChanOutMode)
        || (ChanAttr->enSecureMode != ChanInfo->ChanSecure) )
    {
        HI_ERR_DEMUX("do not support change attr of buffer size/channel type/channel output/secure mode!");

        ret = HI_ERR_DMX_NOT_SUPPORT;
        goto out1;
    }

    switch (ChanAttr->enChannelType)
    {
        case HI_UNF_DMX_CHAN_TYPE_SEC :
        case HI_UNF_DMX_CHAN_TYPE_ECM_EMM :
            switch (ChanAttr->enCRCMode)
            {
                case HI_UNF_DMX_CHAN_CRC_MODE_FORBID :
                case HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD :
                case HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_SEND:
                case HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD :
                case HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_SEND :
                {
                    ChanInfo->ChanCrcMode = ChanAttr->enCRCMode;

                    DmxHalSetChannelCRCMode(DmxSet, ChanInfo->ChanId, ChanInfo->ChanCrcMode);

                    break;
                }

                default :
                    HI_ERR_DEMUX("invalid crc mode %u\n", ChanAttr->enCRCMode);

                    ret = HI_ERR_DMX_INVALID_PARA;
            }
            break;

        case HI_UNF_DMX_CHAN_TYPE_PES :
        case HI_UNF_DMX_CHAN_TYPE_AUD :
        case HI_UNF_DMX_CHAN_TYPE_VID :
        case HI_UNF_DMX_CHAN_TYPE_POST :
            if (ChanAttr->enCRCMode != ChanInfo->ChanCrcMode)
            {
                HI_ERR_DEMUX("PES/VID/AUD/POST channels do not support change their crc mode: %u\n", ChanAttr->enCRCMode);
                ret = HI_ERR_DMX_NOT_SUPPORT;
            }
            break;

        default :
            HI_ERR_DEMUX("invalid chan type %u\n", ChanAttr->enChannelType);

            ret = HI_ERR_DMX_INVALID_PARA;
    }

out1:
    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_S32 DMX_OsiGetChannelPid(HI_U32 DmxId, HI_U32 ChanId, HI_U32 *Pid)
{
    HI_S32 ret = HI_SUCCESS;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    *Pid = ChanInfo->ChanPid;

    DmxPutChnInstance(DmxSet, ChanId);

out:
    return ret;
}

HI_S32 DMX_OsiSetChannelPid(HI_U32 DmxId, HI_U32 ChanId, HI_U32 Pid)
{
    HI_S32 ret;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    if (HI_NULL == DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        HI_ERR_DEMUX("Invalid DmxSet:0x%x\n", DmxSet);
        goto out0;
    }

    if (Pid > DMX_INVALID_PID)
    {
        HI_ERR_DEMUX("invalid param, dmxid:%d,Pid:0x%x\n",DmxId, Pid);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    mutex_lock(&DmxSet->LockAllChn);

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if (HI_UNF_DMX_CHAN_CLOSE != ChanInfo->ChanStatus)
    {
        HI_ERR_DEMUX("close channel(%d) first when set channel pid!\n", ChanId);

        ret = HI_ERR_DMX_OPENING_CHAN;
        goto out2;
    }

#ifndef DMX_DUP_PID_CHANNEL_SUPPORT
    if (Pid < DMX_INVALID_PID)
    {
        HI_U32 index;

        /* try to locate same pid channel and replace it later */
        for_each_set_bit(index, DmxSet->ChnBitmap, DmxSet->DmxChanCnt)
        {
            if (index != ChanId && HI_SUCCESS == DmxGetChnInstance(DmxSet, index))
            {
                if (DmxSet->DmxChanInfo[index].DmxId == ChanInfo->DmxId && DmxSet->DmxChanInfo[index].ChanPid == Pid)
                {
                    DmxHalSetChannelPid(DmxSet, DmxSet->DmxChanInfo[index].ChanId ,DMX_INVALID_PID);
                    DmxSet->DmxChanInfo[index].ChanPid = DMX_INVALID_PID;

                    HI_ERR_DEMUX("Attention: This channel(%d) pid become invalid, update the pid(%d) to channel(%d) of demux(%d). \n",
                                            DmxSet->DmxChanInfo[index].ChanId, Pid, ChanId, DmxId);
                }
                DmxPutChnInstance(DmxSet, index);
            }
        }
    }
#endif

    ChanInfo->ChanPid = Pid;
    DmxHalSetChannelPid(DmxSet, ChanId, Pid);

    ret = HI_SUCCESS;

out2:
    DmxPutChnInstance(DmxSet, ChanId);
out1:
    mutex_unlock(&DmxSet->LockAllChn);
out0:
    return ret;
}

HI_S32 DMX_OsiGetChannelStatus(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_CHAN_STATUS_E *ChanStatus)
{
    HI_S32 ret = HI_SUCCESS;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    *ChanStatus = ChanInfo->ChanStatus;

    DmxPutChnInstance(DmxSet, ChanId);
out:
    return ret;
}

HI_S32 DMX_OsiGetFreeChannelNum(HI_U32 DmxId, HI_U32 *FreeCount)
{
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *ChanInfo = HI_NULL;
    HI_U32 i;

    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    ChanInfo = DmxSet->DmxChanInfo;

    *FreeCount = 0;

    for (i = 0; i < DmxSet->DmxChanCnt; i++)
    {
        if (ChanInfo[i].DmxId >= GetDmxCluster()->Ops->GetDmxNum())
        {
            ++(*FreeCount);
        }
    }

    return HI_SUCCESS;
}

/***********************************************************************************
* Function      :   DMX_OsiGetChannelScrambleFlag
* Description   :  get channel scrambe flag
* Input         :  u32ChannelId ,
* Output        :  penScrambleFlag
* Return        :  HI_SUCCESS:     success
*                  HI_FAILURE:
* Others:
***********************************************************************************/
HI_S32 DMX_OsiGetChannelScrambleFlag(HI_U32 DmxId, HI_U32 u32ChannelId, HI_UNF_DMX_SCRAMBLED_FLAG_E *penScrambleFlag)
{
    HI_S32 ret = HI_SUCCESS;
    HI_BOOL bTSScramble, bPesScramble;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *pChInfo = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, u32ChannelId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    pChInfo = &DmxSet->DmxChanInfo[u32ChannelId];

    if (HI_UNF_DMX_CHAN_CLOSE == pChInfo->ChanStatus)
    {
        HI_ERR_DEMUX("channel %d is not opened!\n", u32ChannelId);
        ret = HI_ERR_DMX_NOT_OPEN_CHAN;
        goto out1;
    }

    DmxHalGetChannelTSScrambleFlag(DmxSet, u32ChannelId, &bTSScramble);
    DmxHalGetChannelPesScrambleFlag(DmxSet, u32ChannelId, &bPesScramble);
    if (HI_TRUE == bTSScramble)
    {
        *penScrambleFlag = HI_UNF_DMX_SCRAMBLED_FLAG_TS;
    }
    else if (HI_TRUE == bPesScramble)
    {
        *penScrambleFlag = HI_UNF_DMX_SCRAMBLED_FLAG_PES;
    }
    else
    {
        *penScrambleFlag = HI_UNF_DMX_SCRAMBLED_FLAG_NO;
    }

out1:
    DmxPutChnInstance(DmxSet, u32ChannelId);
out0:
    return ret;
}

HI_S32 DMX_OsiSetChannelEosFlag(HI_U32 DmxId, HI_U32 ChanId)
{
    HI_S32 ret = HI_SUCCESS;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if (HI_UNF_DMX_CHAN_CLOSE == ChanInfo->ChanStatus)
    {
        HI_ERR_DEMUX("channel %d is not opened!\n", ChanId);

        ret = HI_ERR_DMX_NOT_OPEN_CHAN;
        goto out1;
    }

    ChanInfo->ChanEosFlag = HI_TRUE;

out1:
    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_S32 DMX_OsiGetChannelTsCnt(HI_U32 DmxId, HI_U32 ChanId, HI_U32 *pTsCnt)
{
    HI_S32 ret = HI_SUCCESS;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    *pTsCnt = DmxHalGetChannelCounter(DmxSet, ChanId);

    DmxPutChnInstance(DmxSet, ChanId);
out:
    return ret;
}

HI_S32 DMX_OsiSetChannelCCRepeat(HI_U32 DmxId, HI_U32 ChanId, HI_UNF_DMX_CHAN_CC_REPEAT_SET_S * pstChCCReaptSet)
{
    HI_S32 ret = HI_SUCCESS;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    CHECKPOINTER(pstChCCReaptSet);

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    if (pstChCCReaptSet->enCCRepeatMode == HI_UNF_DMX_CHAN_CC_REPEAT_MODE_DROP)
    {
        DmxHalSetChannelCCRepeatCtrl(DmxSet, ChanId, DMX_DISABLE);
    }
    else
    {
        DmxHalSetChannelCCRepeatCtrl(DmxSet, ChanId, DMX_ENABLE);
    }

    DmxPutChnInstance(DmxSet, ChanId);
out:
    return ret;
}

HI_S32 DMX_OsiGetChannelId(HI_U32 DmxId, HI_U32 Pid, HI_U32 *ChanId)
{
    HI_S32 ret = HI_ERR_DMX_UNMATCH_CHAN;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    HI_U32 idx = 0 , cnt = 0;

    if (!DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    if (Pid >= DMX_INVALID_PID)
    {
        HI_ERR_DEMUX("invalid pid:%d!\n", Pid);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    mutex_lock(&DmxSet->LockAllChn);

    for_each_set_bit(idx, DmxSet->ChnBitmap, DmxSet->DmxChanCnt)
    {
        DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[idx];

        if ((ChanInfo->DmxId == DmxId) && (ChanInfo->ChanPid == Pid))
        {
            if (HI_SUCCESS == DmxGetChnInstance(DmxSet, ChanInfo->ChanId) )
            {
                /* double  check */
                if ((ChanInfo->DmxId == DmxId) && (ChanInfo->ChanPid == Pid))
                {
                    *ChanId = ChanInfo->ChanId;
                    cnt ++;
                }

                DmxPutChnInstance(DmxSet, ChanInfo->ChanId);
            }
        }
    }

    mutex_unlock(&DmxSet->LockAllChn);

    switch(cnt)
    {
        case 0:
            ret = HI_ERR_DMX_UNMATCH_CHAN;
            break;
        case 1:
            ret = HI_SUCCESS;
            break;
        default:
            HI_ERR_DEMUX("multiple channel own this pid(%d) on the same demux(%d).\n", Pid, DmxId);
            ret = HI_ERR_DMX_OCCUPIED_PID;
            break;
    }

out:
    return ret;
}

static HI_S32  __DMX_OsiReadDataRequset(Dmx_Set_S *DmxSet,
                               HI_U32         u32ChId,
                               HI_U32         u32AcqNum,
                               HI_U32 *       pu32AcqedNum,
                               DMX_UserMsg_S* psMsgList,
                               HI_U32         u32TimeOutMs)
{
    DMX_ChanInfo_S *pChanInfo = &DmxSet->DmxChanInfo[u32ChId];
    DMX_OQ_Info_S  *OqInfo = &DmxSet->DmxOqInfo[pChanInfo->ChanOqId];
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 TimeMs= 0;

    pChanInfo->u32TotalAcq++;

    HI_DRV_SYS_GetTimeStampMs(&TimeMs);

    pChanInfo->u32AcqTimeInterval = TimeMs - pChanInfo->u32AcqTime;
    pChanInfo->u32AcqTime = TimeMs;

    if (HI_UNF_DMX_CHAN_PLAY_EN != (HI_UNF_DMX_CHAN_PLAY_EN & pChanInfo->ChanStatus))
    {
        HI_ERR_DEMUX("channel %d is not open yet or not play channel\n", u32ChId);

        s32Ret = HI_ERR_DMX_NOT_OPEN_CHAN;
        goto out;
    }

    if (   (HI_UNF_DMX_CHAN_TYPE_SEC    == pChanInfo->ChanType)
        || (HI_UNF_DMX_CHAN_TYPE_ECM_EMM== pChanInfo->ChanType)
        || (HI_UNF_DMX_CHAN_TYPE_POST   == pChanInfo->ChanType) )
    {
        s32Ret = DMXOsiReadSec(DmxSet, pChanInfo, u32AcqNum, pu32AcqedNum, psMsgList, OqInfo, u32TimeOutMs);

        goto out;
    }
    else if (HI_UNF_DMX_CHAN_TYPE_PES == pChanInfo->ChanType)
    {
        u32AcqNum = DMX_PES_MAX_SIZE;

        s32Ret = DMXOsiReadPes(DmxSet, pChanInfo, u32AcqNum, pu32AcqedNum, psMsgList, u32TimeOutMs);

        goto out;
    }
    else
    {
        HI_ERR_DEMUX("channel %d is wrong channeltype %d!\n", u32ChId, pChanInfo->ChanType);

        s32Ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

out:
    return s32Ret;
}

HI_S32  DMX_OsiReadDataRequset(HI_U32 DmxId,
                               HI_U32         u32ChId,
                               HI_U32         u32AcqNum,
                               HI_U32 *       pu32AcqedNum,
                               DMX_UserMsg_S* psMsgList,
                               HI_U32         u32TimeOutMs)
{
    HI_S32 ret;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    if (0 == u32AcqNum)
    {
        HI_ERR_DEMUX(" channel %d acquire data num = 0!\n", u32ChId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    do
    {
        ret = DmxGetChnInstance(DmxSet, u32ChId);
        if (HI_SUCCESS != ret)
        {
            break;
        }

        ret = __DMX_OsiReadDataRequset(DmxSet, u32ChId, u32AcqNum, pu32AcqedNum, psMsgList, u32TimeOutMs);
        if (HI_SUCCESS == ret)
        {
            DmxPutChnInstance(DmxSet, u32ChId);
            break;
        }
        else
        {
            /* avoid deadlock due to timeout schedule */
            DmxPutChnInstance(DmxSet, u32ChId);

            if (u32TimeOutMs)
            {
                DMX_ChanInfo_S *pChanInfo = &DmxSet->DmxChanInfo[u32ChId];

                ret = DmxChannelWaitTimeOut(DmxSet, pChanInfo, u32TimeOutMs);
                if (0 == ret)
                {
                    ret = HI_ERR_DMX_TIMEOUT;
                    break;
                }
                else if (ret < 0)
                {
                    break;
                }

                /* ret : remain timeout value. */
                u32TimeOutMs = jiffies_to_msecs(ret);

                continue;
            }
            else
            {
                break;
            }
        }

    }while(1);

out:
    return ret;
}

HI_U32 DMX_OsiGetChType(HI_U32 DmxId, HI_U32 u32ChId)
{
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    if (HI_SUCCESS == DmxGetChnInstance(DmxSet, u32ChId))
    {
        DMX_ChanInfo_S *pChanInfo = &DmxSet->DmxChanInfo[u32ChId];
        HI_U32 type = pChanInfo->ChanType;

        DmxPutChnInstance(DmxSet, u32ChId);

        return type;
    }

    return HI_UNF_DMX_CHAN_TYPE_BUTT;
}

HI_S32 __DMX_OsiReleaseReadData(Dmx_Set_S *DmxSet, HI_U32 u32ChId, HI_U32 u32RelNum, DMX_UserMsg_S* psMsgList)
{
    DMX_ChanInfo_S *pChanInfo = &DmxSet->DmxChanInfo[u32ChId];
    DMX_DmxInfo_S *DmxInfo = DmxSet->Ops->GetDmxInfo(DmxSet, pChanInfo->DmxId);
    DMX_OQ_Info_S  *OqInfo;
    HI_U32 u32Fq, u32Blkcnt = 0;
    HI_U32 *pu32ReleaseBlk, u32WriteBlk, u32ReadPtr;
    HI_U32 u32BufPhyAddr, u32BufSize, u32BufLen;
    HI_U32 u32ReadBlkNext, u32BufLenNext = 0;
    HI_U32 u32BufPhyAddrNext = 0;
    HI_U32 u32RealseFlag, u32NextBlkValidFlag;
    HI_U32 *pAddr = HI_NULL;
    HI_U32 i = 0;
    DMX_FQ_Info_S  *FqInfo;
    HI_SIZE_T u32LockFlag;
    HI_U32 TimeMs = 0;
    OQ_DescInfo_S *oq_desc;

    pChanInfo->u32Release++;

    HI_DRV_SYS_GetTimeStampMs(&TimeMs);

    pChanInfo->u32RelTimeInterval = TimeMs - pChanInfo->u32RelTime;
    pChanInfo->u32RelTime = TimeMs;

    OqInfo = &DmxSet->DmxOqInfo[pChanInfo->ChanOqId];

    /*
    first ,get OQ Desc read write pointer from register ,
    OQ read pointer is only update by software,now ,it is the value last time software update it.
    OQ write pointer is only update by logic(hardware   )
    */
    DMXOsiOQGetReadWrite(DmxSet, pChanInfo->ChanOqId, &u32WriteBlk, &u32ReadPtr);

    /*
    malloc pAddr for record the bufferAddress and  buffersize of error buffer block, each error block need 2 words ,
    so ,malloc size is sizeof(HI_U32) * DMX_MAX_ERRLIST_NUM * 2
     */
    pAddr = HI_KMALLOC(HI_ID_DEMUX, sizeof(HI_U32) * DMX_MAX_ERRLIST_NUM * 2, GFP_KERNEL);
    if (HI_NULL == pAddr)
    {
        HI_FATAL_DEMUX("Malloc pAddr failed\n");
        return HI_FAILURE;
    }
     /*the start release position is the u32ReadPtr*/
    OqInfo->u32ReleaseBlk = u32ReadPtr;
    pu32ReleaseBlk = &OqInfo->u32ReleaseBlk;
    /***********************************situition1 ,channel type is common(SEC/POST.ECM/EMM)*******************/
    if (   (HI_UNF_DMX_CHAN_TYPE_SEC    == pChanInfo->ChanType)
        || (HI_UNF_DMX_CHAN_TYPE_ECM_EMM== pChanInfo->ChanType)
        || (HI_UNF_DMX_CHAN_TYPE_POST   == pChanInfo->ChanType) )
    {
        while (*pu32ReleaseBlk != u32WriteBlk)
        {
            u32RealseFlag = 0;
            /*oq_desc is the address of current descriptor (which will be released current) in OQ, not the number */
            oq_desc = OqInfo->OqDescSetBase + *pu32ReleaseBlk;

            u32BufPhyAddr = oq_desc->start_addr;            /*u32BufPhyAddr is current block address,acturally, this first word of Desc is block address*/
            u32BufLen  = oq_desc->pvrctrl_datalen & 0xffff;   /*u32BufLen is current block's data  length*/
            u32BufSize = oq_desc->cactrl_buflen & 0xffff;   /*u32BufSize is current block's size*/

            u32ReadBlkNext = *pu32ReleaseBlk + 1;
            if (u32ReadBlkNext >= OqInfo->OQDepth)
            {
                u32ReadBlkNext = 0;
            }

            /*the following if-else if mainly for find out the next valide block to be released,
            both error block and valide block's information will be recored in pAddr[i].
            so ,the output of this if-else is u32BufPhyAddrNext and u32BufLenNext*/
            if (IsAddrValid(u32ReadPtr, u32WriteBlk, u32ReadBlkNext) != HI_SUCCESS)
            {
                u32NextBlkValidFlag = 0;
            }
            else
            {
                /*
                to find out next valid block ,first ,judge wether the next block is the "last next" ,meets: read == write.
                */
                DmxHalGetOQWORDx(DmxSet, pChanInfo->ChanOqId, DMX_OQ_SADDR_OFFSET, &u32BufPhyAddrNext);
                DmxHalGetOQWORDx(DmxSet, pChanInfo->ChanOqId, DMX_OQ_SZUS_OFFSET, &u32BufLenNext);
                u32BufLenNext >>= 16;
                DMXOsiOQGetReadWrite(DmxSet, pChanInfo->ChanOqId, &u32WriteBlk, HI_NULL);
                if (u32ReadBlkNext == u32WriteBlk)
                {
                    u32NextBlkValidFlag = 1;
                }
                else
                {
                    /*find a valid next block*/
                    while(1)
                    {
                        if (u32ReadBlkNext != u32WriteBlk)
                        {
                            oq_desc = OqInfo->OqDescSetBase + u32ReadBlkNext;

                            u32BufPhyAddrNext = oq_desc->start_addr;
                            u32BufLenNext = oq_desc->cactrl_buflen & 0xffff; //ues buf size instead of buf len,for next block may invalid

                            if(DMXOsiCheckErrMSG(u32BufPhyAddrNext) == 0)
                            {
                                u32NextBlkValidFlag = 1;
                                break;
                            }
                            else
                            {
                                DMXINC(u32ReadBlkNext, OqInfo->OQDepth);
                                continue;
                            }
                        }
                        else
                        {
                            DmxHalGetOQWORDx(DmxSet, pChanInfo->ChanOqId, DMX_OQ_SADDR_OFFSET, &u32BufPhyAddrNext);
                            DmxHalGetOQWORDx(DmxSet, pChanInfo->ChanOqId, DMX_OQ_SZUS_OFFSET, &u32BufLenNext);
                            u32BufLenNext >>= 16;
                            u32NextBlkValidFlag = 1;
                            break;
                        }
                    }
                }
            }

            /*at here ,we release the buffer */
            for (; i < u32RelNum; i++)
            {
                u32RealseFlag = 0;
                /*if psMsgList[i] is in current block buffer*/
                if ((psMsgList[i].u32BufStartAddr >= u32BufPhyAddr)
                   && (psMsgList[i].u32BufStartAddr <= (u32BufPhyAddr + u32BufLen - 1)))
                {
                    /*for section channel ,one block buffer (5K) usually contains some psMsgList[i] .the i is keep increasing ,
                    when psMsgList[i]'s end address large than current block's end address ,
                    it means the whole current block should be released, so set flag u32RealseFlag = 1 .
                    this flag u32RealseFlag is for bolck ,not for psMsgList[i]*/
                    if ((psMsgList[i].u32BufStartAddr + psMsgList[i].u32MsgLen) >= (u32BufPhyAddr + u32BufLen))
                    {
                        u32RealseFlag = 1;
                    }
                }
                else if (u32NextBlkValidFlag)
                {
                    /*if psMsgList[i] is not in  current block buffer, judge wether it is in the next block of buffer*/
                    if ((psMsgList[i].u32BufStartAddr >= u32BufPhyAddrNext)
                       && (psMsgList[i].u32BufStartAddr <= (u32BufPhyAddrNext + u32BufLenNext - 1)))
                    {
                        u32RealseFlag = 1;
                    }
                    /*if psMsgList[i] is not in  current block buffer and next block buffer,
                    may be there are error block between current and next block , or the current block is error block
                    and psMsgList[i] is in it*/
                    else
                    {
                        /*
                        if current block is error block ,use pAddr to record it
                        */
                        if (DMXOsiRelErrMSG(u32BufPhyAddr) == 0)
                        {
                            //u32RealseFlag = 1;
                            if (++ * pu32ReleaseBlk >= OqInfo->OQDepth)
                            {
                                *pu32ReleaseBlk = 0;
                            }

                            pAddr[2 * u32Blkcnt + 0] = u32BufPhyAddr;
                            pAddr[2 * u32Blkcnt + 1] = u32BufSize;
                            u32Blkcnt++;
                            break;
                        }
                        else
                        {
                            HI_ERR_DEMUX("psMsgList[%d].u32BufStartAddr:%x,len:%x,bufferAddr:%x,nextptr:%x,nexlen:%x,OqDescSetPhy:%x,rel:%x,read:%x\n",
                                           i, psMsgList[i].u32BufStartAddr, psMsgList[i].u32MsgLen, u32BufPhyAddr,
                                         u32BufPhyAddrNext, u32BufLenNext, OqInfo->OqDescSetPhy,
                                         *pu32ReleaseBlk,u32ReadPtr);
                            HI_KFREE(HI_ID_DEMUX, (HI_VOID *) pAddr);

                            return HI_ERR_DMX_INVALID_PARA;
                        }
                    }
                }

                if (u32RealseFlag)
                {
                    if (++ * pu32ReleaseBlk >= OqInfo->OQDepth)
                    {
                        *pu32ReleaseBlk = 0;
                    }

                    DMXOsiRelErrMSG(u32BufPhyAddr);
                    pAddr[2 * u32Blkcnt + 0] = u32BufPhyAddr;
                    pAddr[2 * u32Blkcnt + 1] = u32BufSize;
                    u32Blkcnt++;
                    i++;
                    break;
                }
            }

            if (i == u32RelNum)
            {
                break;
            }
        }

        /*at here ,now we finish calc the new read pointer in OQ, the following code is fo update the register to finished the release operation*/
        if (u32Blkcnt)
        {
            u32Fq = OqInfo->u32FQId;

            FqInfo = &DmxSet->DmxFqInfo[u32Fq];
            spin_lock_irqsave(&FqInfo->LockFq, u32LockFlag);

            DmxHalSetOQReadPtr(DmxSet, pChanInfo->ChanOqId, *pu32ReleaseBlk);

            DmxHalGetFQReadWritePtr(DmxSet, u32Fq, HI_NULL, &u32WriteBlk);


            for (i = 0; i < u32Blkcnt; i++)
            {
                FQ_DescInfo_S  *FqDesc = DmxSet->DmxFqInfo[u32Fq].FqDescSetBase + u32WriteBlk;

                FqDesc->start_addr = pAddr[2 * i + 0];
                FqDesc->buflen = pAddr[2 * i + 1];

                if ((++u32WriteBlk) >= DmxSet->DmxFqInfo[u32Fq].FQDepth)
                {
                    u32WriteBlk = 0;
                }
            }

            DmxHalSetFQWritePtr(DmxSet, u32Fq, u32WriteBlk);

            spin_unlock_irqrestore(&FqInfo->LockFq, u32LockFlag);

            DMXCheckFQBPStatus(DmxSet, DmxInfo, u32Fq);

            DmxFqCheckOverflowInt(DmxSet, u32Fq);

            //DMXCheckOQEnableStatus(pChanInfo->stChBuf.u32OQId, pChanInfo->stChBuf.OQDepth);
            if (*pu32ReleaseBlk < u32WriteBlk)
            {
                if (OqInfo->u32ProcsBlk < *pu32ReleaseBlk || OqInfo->u32ProcsBlk > u32WriteBlk)//invalid procs ptr
                {
                    OqInfo->u32ProcsBlk = *pu32ReleaseBlk;
                    OqInfo->u32ProcsOffset = 0;
                }
            }
            else
            {
                if (OqInfo->u32ProcsBlk > u32WriteBlk && OqInfo->u32ProcsBlk < *pu32ReleaseBlk)//invalid procs ptr
                {
                    OqInfo->u32ProcsBlk = *pu32ReleaseBlk;
                    OqInfo->u32ProcsOffset = 0;
                }
            }
        }
    }
    else if (HI_UNF_DMX_CHAN_TYPE_PES == pChanInfo->ChanType)
    {
        for (; i < u32RelNum && u32ReadPtr != u32WriteBlk; i++)
        {
            oq_desc = OqInfo->OqDescSetBase + u32ReadPtr;

            u32BufPhyAddr = oq_desc->start_addr;
            u32BufSize = oq_desc->cactrl_buflen & 0xffff;

            if (psMsgList[i].u32BufStartAddr != u32BufPhyAddr)
            {
                HI_ERR_DEMUX("release addr: %x, oq read phyaddr: %x\n", psMsgList[i].u32BufStartAddr, u32BufPhyAddr);
                HI_KFREE(HI_ID_DEMUX, (HI_VOID *) pAddr);

                return HI_ERR_DMX_INVALID_PARA;
            }

            pAddr[2 * u32Blkcnt + 0] = u32BufPhyAddr;
            pAddr[2 * u32Blkcnt + 1] = u32BufSize;
            u32Blkcnt++;
            DMXINC(u32ReadPtr, OqInfo->OQDepth);
            if (u32ReadPtr == u32WriteBlk)
            {
                break;
            }
        }

        if (u32Blkcnt)
        {
            DmxHalSetOQReadPtr(DmxSet, pChanInfo->ChanOqId, u32ReadPtr);

            u32Fq = OqInfo->u32FQId;

            FqInfo = &DmxSet->DmxFqInfo[u32Fq];
            spin_lock_irqsave(&FqInfo->LockFq, u32LockFlag);

            DmxHalGetFQReadWritePtr(DmxSet, u32Fq, HI_NULL, &u32WriteBlk);

            for (i = 0; i < u32Blkcnt; i++)
            {
                FQ_DescInfo_S  *FqDesc = DmxSet->DmxFqInfo[u32Fq].FqDescSetBase + u32WriteBlk;

                FqDesc->start_addr = pAddr[2 * i + 0];
                FqDesc->buflen =  pAddr[2 * i + 1];

                if ((++u32WriteBlk) >= DmxSet->DmxFqInfo[u32Fq].FQDepth)
                {
                    u32WriteBlk = 0;
                }
            }

            DmxHalSetFQWritePtr(DmxSet, u32Fq, u32WriteBlk);
            spin_unlock_irqrestore(&FqInfo->LockFq, u32LockFlag);

            DMXCheckFQBPStatus(DmxSet, DmxInfo, u32Fq);
            //DMXCheckOQEnableStatus(pChanInfo->stChBuf.u32OQId, pChanInfo->stChBuf.OQDepth);

            DmxFqCheckOverflowInt(DmxSet, u32Fq);
        }
    }

    HI_KFREE(HI_ID_DEMUX, (HI_VOID *) pAddr);

    return HI_SUCCESS;
}

HI_S32 DMX_OsiReleaseReadData(HI_U32 DmxId, HI_U32 u32ChId, HI_U32 u32RelNum, DMX_UserMsg_S* psMsgList)
{
    HI_S32 ret;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *pChanInfo = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, u32ChId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    pChanInfo = &DmxSet->DmxChanInfo[u32ChId];

    if (HI_UNF_DMX_CHAN_PLAY_EN != (HI_UNF_DMX_CHAN_PLAY_EN & pChanInfo->ChanStatus))
    {
        HI_ERR_DEMUX("channel %d is not open yet or not play channel\n", u32ChId);
        ret = HI_ERR_DMX_NOAVAILABLE_DATA;
        goto  out1;
    }

    ret = __DMX_OsiReleaseReadData(DmxSet, u32ChId, u32RelNum, psMsgList);

out1:
    DmxPutChnInstance(DmxSet, u32ChId);
out0:
    return ret;
}

/*
 * This is used to peek the first u32PeekLen of a SEC or PES packet. It does not consume the data in
 * the internal demux section buffer, i.e. it simply copies the requested amount of data
 * but does not move the consumer pointer.
 */
HI_S32 __DMX_OsiPeekDataRequest(Dmx_Set_S *DmxSet, HI_U32 u32ChId,
                              HI_U32         u32PeekLen,
                              DMX_UserMsg_S* psMsgList)
{
    DMX_ChanInfo_S *pChanInfo = &DmxSet->DmxChanInfo[u32ChId];
    DMX_ChanInfo_S stTmpChanInfo;
    DMX_OQ_Info_S  tmpOqInfo;
    HI_U32 u32AcqedNum = 0;
    HI_S32 s32Ret;

    if (HI_SUCCESS != __DMX_OsiGetChnDataFlag(DmxSet, u32ChId, pChanInfo->ChanOqId))//channel have no data
    {
        return HI_ERR_DMX_NOAVAILABLE_DATA;
    }

    if (HI_UNF_DMX_CHAN_TYPE_PES == pChanInfo->ChanType)
    {
        HI_U32 u32CurDropCnt;
        u32CurDropCnt = pChanInfo->u32DropCnt;
        memcpy(&stTmpChanInfo,pChanInfo,sizeof(DMX_ChanInfo_S));

        s32Ret = DMXOsiFindPes(DmxSet, &stTmpChanInfo,psMsgList,16, &u32AcqedNum);
        if (u32CurDropCnt != pChanInfo->u32DropCnt)//drop occurs
        {
            pChanInfo->u32PesBlkCnt   = 0;
            pChanInfo->u32PesLength   = 0;
            pChanInfo->u32ProcsOffset = 0;
        }
    }
    else
    {
        memcpy(&stTmpChanInfo,pChanInfo,sizeof(DMX_ChanInfo_S));
        memcpy(&tmpOqInfo,&DmxSet->DmxOqInfo[pChanInfo->ChanOqId],sizeof(DMX_OQ_Info_S));

        s32Ret = DMXOsiReadSec(DmxSet, &stTmpChanInfo,1,&u32AcqedNum,psMsgList,&tmpOqInfo,0);
    }

    if (HI_SUCCESS == s32Ret && u32AcqedNum)
    {
        return HI_SUCCESS;
    }

    return HI_ERR_DMX_NOAVAILABLE_DATA;
}

HI_S32 DMX_OsiPeekDataRequest(HI_U32  DmxId, HI_U32 u32ChId,
                              HI_U32         u32PeekLen,
                              DMX_UserMsg_S* psMsgList)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *pChanInfo = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, u32ChId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    pChanInfo = &DmxSet->DmxChanInfo[u32ChId];

    if (HI_UNF_DMX_CHAN_PLAY_EN != (HI_UNF_DMX_CHAN_PLAY_EN & pChanInfo->ChanStatus))
    {
        HI_WARN_DEMUX("channel %d is not open yet or not play channel\n", u32ChId);

        ret = HI_ERR_DMX_NOT_SUPPORT;
        goto out1;
    }
    if ((HI_UNF_DMX_CHAN_TYPE_AUD == pChanInfo->ChanType) || (HI_UNF_DMX_CHAN_TYPE_VID == pChanInfo->ChanType))
    {
        ret = HI_ERR_DMX_NOT_SUPPORT;   // do not receive vid and aud channel
        goto out1;
    }

    ret = __DMX_OsiPeekDataRequest(DmxSet, u32ChId, u32PeekLen, psMsgList);

out1:
    DmxPutChnInstance(DmxSet, u32ChId);
out0:
    return ret;
}

static HI_S32 __DmxParsePesHeader(
        HI_U8           *ParserAddr,
        HI_U32          ParserLen,
        HI_U32         *PesHeaderLen,
        HI_U32          *PesPktLen,
        HI_U32          *LastPts,
        Disp_Control_t *pDispController
    )
{
    HI_U32 StreamId, PTSFlag;

    if ((ParserAddr[0] != 0x00) || (ParserAddr[1] != 0x00) || (ParserAddr[2] != 0x01))
    {
        HI_WARN_DEMUX("pes start byte = %x %x %x\n", ParserAddr[2], ParserAddr[1], ParserAddr[0]);
        return HI_FAILURE;
    }

    /* Get StreamId */
    StreamId = ParserAddr[3];

    //del pes header according to 13818-1
    if ((StreamId != 0xbc) //1011 1100  1   program_stream_map
        && (StreamId != 0xbe) //1011 1110       padding_stream
        && (StreamId != 0xbf) //1011 1111   3   private_stream_2
        && (StreamId != 0xf0) //1111 0000   3   ECM_stream
        && (StreamId != 0xf1) //1111 0001   3   EMM_stream
        && (StreamId != 0xff) //1111 1111   4   program_stream_directory
        && (StreamId != 0xf2) //1111 0010   5   ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A or ISO/IEC 13818-6_DSMCC_stream
        && (StreamId != 0xf8)) //1111 1000  6   ITU-T Rec. H.222.1 type E
    {
        /*      if (PTS_DTS_flags =='10' ) {
            '0010'  4   bslbf
            PTS [32..30]    3   bslbf
            marker_bit  1   bslbf
            PTS [29..15]    15  bslbf
            marker_bit  1   bslbf
            PTS [14..0] 15  bslbf
            marker_bit  1   bslbf
        }
                if (PTS_DTS_flags =='11' )  {
            '0011'
            PTS [32..30]
            marker_bit
            PTS [29..15]
            marker_bit
            PTS [14..0]
            marker_bit
            '0001'
            DTS [32..30]
            marker_bit
            DTS [29..15]
            marker_bit
            DTS [14..0]
            marker_bit
        }
         */
        PTSFlag = (ParserAddr[7] & 0x80) >> 7;

        *PesHeaderLen = DMX_PES_HEADER_LENGTH + ParserAddr[8];

        *PesPktLen = ((ParserAddr[4] << 8) | ParserAddr[5]) - *PesHeaderLen + 6;

        if (PTSFlag && (ParserLen > *PesHeaderLen))
        {
            HI_U32 PTSLow = (((ParserAddr[9] & 0x06) >> 1) << 30)
                     | ((ParserAddr[10]) << 22)
                     | (((ParserAddr[11] & 0xfe) >> 1) << 15)
                     | ((ParserAddr[12]) << 7)
                     | (((ParserAddr[13] & 0xfe)) >> 1);
            if (ParserAddr[9] & 0x08)
            {
                *LastPts = (PTSLow / 90) + 0x2D82D83; //(1 << 32) / 90 ,使用科学计算器计算结果是0x2D82D83 (0x2D82D83);
            }
            else
            {
                *LastPts = (PTSLow / 90);
            }
        }
        else
        {
            *LastPts = INVALID_PTS;
        }
        //add by yangchun 201302234
        if ((0xee == StreamId) && ( ParserLen > 24))
        {
            if( (0x70 == ParserAddr[13]) && (0x76 == ParserAddr[14]) && (0x72 == ParserAddr[15]) && (0x63 == ParserAddr[16]))
            {
                if( (0x75 == ParserAddr[17]) && (0x72 == ParserAddr[18]) && (0x74 == ParserAddr[19]) && (0x6d == ParserAddr[20]))
                {
                    pDispController->u32DispTime = *(HI_U32 *)&ParserAddr[21];
                    pDispController->u32DispEnableFlag = *(HI_U32 *)&ParserAddr[25];
                    pDispController->u32DispFrameDistance = *(HI_U32 *)&ParserAddr[29];
                    pDispController->u32DistanceBeforeFirstFrame = *(HI_U32 *)&ParserAddr[33];
                    pDispController->u32GopNum = *(HI_U32 *)&ParserAddr[37];
                    *PesHeaderLen = 184;
                    return -2;
                }
            }
        }

        return HI_SUCCESS;
    }
    else
    {
        HI_ERR_DEMUX("streamID:%x did not del the pes header!\n", StreamId);
        return HI_FAILURE;
    }
}

static HI_S32 __DmxGetPesHeaderLen(HI_U8 *ParserAddr, HI_U32 ParserLen, HI_U32 *PesHeaderLen)
{
    HI_S32 ret = HI_FAILURE;

    *PesHeaderLen = 0;

    if (ParserLen >= DMX_PES_HEADER_LENGTH)
    {
        *PesHeaderLen = DMX_PES_HEADER_LENGTH + ParserAddr[8];

        ret = HI_SUCCESS;
    }

    return ret;
}

#if defined(DMX_SECURE_CHANNEL_SUPPORT) && defined(DMX_TEE_SUPPORT)
static HI_S32 DmxTeecParsePesDispCtrlInfo(DMX_ChanInfo_S *ChanInfo, HI_U32 ParserAddr, HI_U32 ParserLen, HI_U32  *PesHeaderLen, Disp_Control_t *pDispController)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.params[0].value.a = DMX_CHANHANDLE(ChanInfo->DmxId, ChanInfo->ChanId);
    operation.params[1].value.a = ParserAddr;
    operation.params[1].value.b = ParserLen;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INOUT, TEEC_VALUE_OUTPUT, TEEC_VALUE_OUTPUT);

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_GET_DISP_CONTROL, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_GET_DISP_CONTROL to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
    }
    else
    {
        *PesHeaderLen = 184;

        pDispController->u32DispTime = operation.params[1].value.a;
        pDispController->u32DispEnableFlag = operation.params[1].value.b;
        pDispController->u32DispFrameDistance = operation.params[2].value.a;
        pDispController->u32DistanceBeforeFirstFrame = operation.params[2].value.b;
        pDispController->u32GopNum = operation.params[3].value.a;

        ret = HI_SUCCESS;
    }

    return ret;
}
static HI_S32 DmxTeecParsePesHeader(
        DMX_ChanInfo_S  *ChanInfo,
        HI_U32           ParserAddr,
        HI_U32           ParserLen,
        HI_U32          *PesHeaderLen,
        HI_U32          *PesPktLen,
        HI_U32          *LastPts,
        Disp_Control_t *pDispController
    )
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.params[0].value.a = DMX_CHANHANDLE(ChanInfo->DmxId, ChanInfo->ChanId);
    operation.params[1].value.a = ParserAddr;
    operation.params[1].value.b = ParserLen;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT, TEEC_VALUE_OUTPUT);

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_PARSER_PES_HEADER, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_PARSER_PES_HEADER to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

    *PesHeaderLen = operation.params[2].value.a;
    *PesPktLen = operation.params[2].value.b;
    *LastPts = operation.params[3].value.a;

    /* not valid pes pkt. */
    if (0 == *PesHeaderLen && 0 == *PesPktLen && 0 == *LastPts)
    {
        ret = HI_FAILURE;
        goto out;
    }

    /* pvr private extent */
    if (0xee == operation.params[3].value.b /* StreamId */)
    {
        if (HI_SUCCESS == DmxTeecParsePesDispCtrlInfo(ChanInfo, ParserAddr, ParserLen, PesHeaderLen, pDispController))
        {
            ret = -2;
            goto out;
        }
    }

out:
    return ret;
}

static HI_S32 DmxTeecGetPesHeaderLen(DMX_ChanInfo_S *ChanInfo, HI_U32 ParserAddr, HI_U32 ParserLen, HI_U32 *PesHeaderLen)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.params[0].value.a = DMX_CHANHANDLE(ChanInfo->DmxId, ChanInfo->ChanId);
    operation.params[1].value.a = ParserAddr;
    operation.params[1].value.b = ParserLen;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT, TEEC_NONE);

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_GET_PES_HEADER_LEN, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_GET_PES_HEADER_LEN to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

    *PesHeaderLen = operation.params[2].value.a;

out:
    return ret;
}

#else
static inline HI_S32 DmxTeecParsePesHeader(
        DMX_ChanInfo_S *ChanInfo,
        HI_U32          ParserAddr,
        HI_U32          ParserLen,
        HI_U32         *PesHeaderLen,
        HI_U32          *PesPktLen,
        HI_U32          *LastPts,
        Disp_Control_t *pDispController)
{
    HI_ERR_DEMUX("DMX_SECURE_CHANNEL_SUPPORT or DMX_TEE_SUPPORT not configured.\n");
    return HI_ERR_DMX_NOT_SUPPORT;
}

static inline HI_S32 DmxTeecGetPesHeaderLen(DMX_ChanInfo_S *ChanInfo, HI_U32 ParserAddr, HI_U32 ParserLen, HI_U32 *PesHeaderLen)
{
    HI_ERR_DEMUX("DMX_SECURE_CHANNEL_SUPPORT or DMX_TEE_SUPPORT not configured.\n");
    return HI_ERR_DMX_NOT_SUPPORT;
}
#endif

static inline HI_S32 DmxParsePesHeader(DMX_ChanInfo_S *ChanInfo,
        HI_U8           *ParserAddr,
        HI_U32          ParserLen,
        HI_U32         *PesHeaderLen,
        HI_U32          *PesPktLen,
        HI_U32          *LastPts,
        Disp_Control_t *pDispController)
{
    if (HI_UNF_DMX_SECURE_MODE_TEE == ChanInfo->ChanSecure)
    {
        return DmxTeecParsePesHeader(ChanInfo, (HI_U32)(unsigned long)ParserAddr, ParserLen, PesHeaderLen, PesPktLen, LastPts, pDispController);
    }

    return __DmxParsePesHeader(ParserAddr, ParserLen, PesHeaderLen, PesPktLen, LastPts, pDispController);
}

static inline HI_S32 DmxGetPesHeaderLen(DMX_ChanInfo_S *ChanInfo, HI_U8 *ParserAddr, HI_U32 ParserLen, HI_U32 *PesHeaderLen)
{
    if (HI_UNF_DMX_SECURE_MODE_TEE == ChanInfo->ChanSecure)
    {
        return DmxTeecGetPesHeaderLen(ChanInfo, (HI_U32)(unsigned long)ParserAddr, ParserLen, PesHeaderLen);
    }

    return __DmxGetPesHeaderLen(ParserAddr, ParserLen, PesHeaderLen);
}

static HI_S32 __DMX_OsiReadEsRequest(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_Stream_S *EsData)
{
    HI_S32 ret;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];
    DMX_DmxInfo_S  *DmxInfo = DmxSet->Ops->GetDmxInfo(DmxSet, ChanInfo->DmxId);
    DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];

    HI_U32              OqRead      = 0;
    HI_U32              OqWrite     = 0;
    HI_U32              PvrCtrl     = 0;
    HI_U32              DataLen     = 0;
    HI_U32              DataPhyAddr = 0;
    HI_U8               *DataKerAddr = 0;
    HI_U32              StartPhyAddr = 0;
    HI_U32              PesHeadLen  = 0;
    HI_U32              tm;
    Disp_Control_t      stDispController;

    ChanInfo->u32TotalAcq++;

    HI_DRV_SYS_GetTimeStampMs(&tm);

    ChanInfo->u32AcqTimeInterval    = tm - ChanInfo->u32AcqTime;
    ChanInfo->u32AcqTime            = tm;

    OqRead = OqInfo->u32ProcsBlk;

    DMXOsiOQGetReadWrite(DmxSet, OqInfo->u32OQId, &OqWrite, HI_NULL);

    if (OqRead == OqWrite)
    {
        /*Usually if OqRead == OqWrite we directly return , but ChanEosFlag means should put the remains data of channel buf  out ,even not a entire BB(Buffer block)
        1. ChanEosFlag is seted ture by call AVPLAY_SetEosFlag
        2. AVPLAY_SetEosFlag also will set Eosflag of other module such as Vfwm ,VO and so on
        3. these flag is for history customer demands , in order dispaly the last pieces of stream
        4. if ChanInfo->ChanEosFlag is false and we have no a entire BB of data (may be a hale BB of data),will return HI_ERR_DMX_NOAVAILABLE_DATA*/
        if (!ChanInfo->ChanEosFlag)
        {
            return HI_ERR_DMX_NOAVAILABLE_DATA;
        }

        /* only ram port need handling ChanEosFlag. */
        if (DMX_PORT_MODE_RAM != DmxInfo->PortMode)
        {
            return HI_ERR_DMX_NOAVAILABLE_DATA;
        }

        /*pay attention, condition of DMXOsiOQGetBbsAddrSize return success is ((OqWrite == CurrWrite) && (Value & DMX_MASK_BIT_7) && PhyAddr && len)
        so the reason of it's failure possiably can be :
        reason 1 : OqWrite != CurrWrite (from line 5928 to here ,may be it happen ,though it seems no time cost ,
        but logic's speed is more quick,so this may happen with not very low probability)
        reason 2 : len = 0*/
        ret = DMXOsiOQGetBbsAddrSize(DmxSet, OqInfo->u32OQId, OqWrite, &StartPhyAddr, &DataLen, &PvrCtrl);

        /*this if is for err processing:
        err1: "if (0 == DataLen)" means this BB have no data
        err2: a BB with SOP havr not enough data (BB data len < 9 (DMX_PES_HEADER_LENGTH) ||  BB data len < PesHeadLen)
        err3: a BB with datalen = 0
        if not the upper 3 err, this BB is valid and we keep on processing it by jump " if (OqRead != OqWrite)" in line 6025*/
        if (HI_SUCCESS == ret)
        {
            DMX_RamPort_Info_S *PortInfo;
            HI_U32 size;

            PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, DmxInfo->PortId);

            DataPhyAddr = StartPhyAddr + OqInfo->u32ProcsOffset;
            DataKerAddr = FqInfo->BufVirAddr + (DataPhyAddr - FqInfo->BufPhyAddr);
            DataLen     -= OqInfo->u32ProcsOffset;

            /*err1: "if (0 == DataLen)" means this BB have no data*/
            /*if  remain no data is current BB, we should judge the TS buffer's status and return two different err type ,app will use it
            pay attention: the situation channel BB have no data and TS buffer have data is exist,found by x00213242*/
            if (0 == DataLen)
            {

#ifdef DMX_TS_BUF_VERSION_2
                size = GetQueueLenth(atomic_read(&PortInfo->LastDescReadIdx), atomic_read(&PortInfo->LastDescWriteIdx), PortInfo->BufSize);
#else
                size = GetQueueLenth(PortInfo->Read, PortInfo->Write, PortInfo->BufSize);
#endif
                if (size > DMX_TS_BUFFER_EMPTY) /*this is experiential value by x00213242 ,comment by l00188263*/
                {
                    return HI_ERR_DMX_NOAVAILABLE_DATA;
                }

                ChanInfo->ChanEosFlag = HI_FALSE;
                return HI_ERR_DMX_EMPTY_BUFFER;
            }

            /*OqInfo->u32ProcsOffset != 0 meas not the first time process the current BB ,so clear the SOP flag*/
            if (0 != OqInfo->u32ProcsOffset)
            {
                PvrCtrl &= ~DMX_MASK_BIT_2;
            }
            /*err2: a BB with SOP havr not enough data*/
            /*the first time of entering a BB with SOP flag*/
            if (PvrCtrl & DMX_MASK_BIT_2)
            {
                ret = DmxGetPesHeaderLen(ChanInfo, DataKerAddr, DataLen, &PesHeadLen);
                /*BB data len < 9 (DMX_PES_HEADER_LENGTH) ||  BB data len < PesHeadLen*/
                if ((HI_FAILURE == ret) || ((HI_SUCCESS == ret) && (DataLen <= PesHeadLen)))
                {
#ifdef DMX_TS_BUF_VERSION_2
                    size = GetQueueLenth(atomic_read(&PortInfo->LastDescReadIdx), atomic_read(&PortInfo->LastDescWriteIdx), PortInfo->BufSize);
#else
                    size = GetQueueLenth(PortInfo->Read, PortInfo->Write, PortInfo->BufSize);
#endif
                    if (size > DMX_TS_BUFFER_EMPTY)
                    {
                        return HI_ERR_DMX_NOAVAILABLE_DATA;
                    }

                    ChanInfo->ChanEosFlag = HI_FALSE;
                    return HI_ERR_DMX_EMPTY_BUFFER;
                }
            }
            /*get OQ w r pointer again and if still equal ,return HI_ERR_DMX_NOAVAILABLE_DATA
            if not equal ,will enter the outer following if:if (OqRead != OqWrite)*/
            DMXOsiOQGetReadWrite(DmxSet, OqInfo->u32OQId, &OqWrite, HI_NULL);
            /*err3: a BB with datalen = 0*/
            if ((OqRead == OqWrite) && (0 == DataLen))
            {
                return HI_ERR_DMX_NOAVAILABLE_DATA;
            }
        }
        else
        {
            /*get OQ w r pointer again and if still equal ,return HI_ERR_DMX_NOAVAILABLE_DATA
            if not equal ,will enter the outer following if:if (OqRead != OqWrite)*/
            DMXOsiOQGetReadWrite(DmxSet, OqInfo->u32OQId, &OqWrite, HI_NULL);//flush OqWrite again
            if (OqRead == OqWrite)
            {
                /*for  DTS2014012901647 , we add this 'if' */
                if (ChanInfo->ChanEosFlag)
                {
                    ChanInfo->ChanEosFlag = HI_FALSE;
                    return HI_ERR_DMX_EMPTY_BUFFER;
                }

                return HI_ERR_DMX_NOAVAILABLE_DATA;
            }
        }
    }

    if (OqRead != OqWrite)
    {
        OQ_DescInfo_S  *OqDesc      = OqInfo->OqDescSetBase + OqRead ;
        HI_U32          CurrOffset  = OqInfo->u32ProcsOffset;

        StartPhyAddr = OqDesc->start_addr;

        DataPhyAddr = StartPhyAddr;
        DataLen     = (OqDesc->pvrctrl_datalen & 0xffff);
        PvrCtrl     = (OqDesc->pvrctrl_datalen >> 16) & 0xff;

        if (unlikely(!(DataPhyAddr >= FqInfo->BufPhyAddr && DataPhyAddr < FqInfo->BufPhyAddr + FqInfo->BufSize)))
        {
            HI_FATAL_DEMUX("invalid desc[0x%x, %d, %d] of fq[0x%x, %d].\n", DataPhyAddr, DataLen, PvrCtrl, FqInfo->BufPhyAddr, FqInfo->BufSize);
            BUG();
        }

        DMXINC(OqInfo->u32ProcsBlk, OqInfo->OQDepth);

        OqInfo->u32ProcsOffset = 0;

        if (DataLen == CurrOffset)
        {
            return HI_ERR_DMX_NOAVAILABLE_DATA;;
        }

        DataPhyAddr += CurrOffset;
        DataLen     -= CurrOffset;

        if (0 != CurrOffset)
        {
            PvrCtrl &= ~DMX_MASK_BIT_2; // clear sop flag
        }
    }

    DataKerAddr = FqInfo->BufVirAddr + (DataPhyAddr - FqInfo->BufPhyAddr);
    //add by yangchun: defaut value for there is no disp time;
    EsData->u32DispTime = ChanInfo->stLastControl.u32DispTime;
    EsData->u32DispEnableFlag= ChanInfo->stLastControl.u32DispEnableFlag;
    EsData->u32DispFrameDistance= ChanInfo->stLastControl.u32DispFrameDistance;
    EsData->u32DistanceBeforeFirstFrame= ChanInfo->stLastControl.u32DistanceBeforeFirstFrame;
    EsData->u32GopNum = ChanInfo->stLastControl.u32GopNum;

    /*if this BB have SOP flag*/
    if (PvrCtrl & DMX_MASK_BIT_2)
    {
        ChanInfo->u32PesLength = 0;

        ret = DmxParsePesHeader(ChanInfo, DataKerAddr, DataLen, &PesHeadLen, &ChanInfo->u32PesLength, &ChanInfo->LastPts, &stDispController);
        if (HI_SUCCESS == ret)
        {
            DataLen     -= PesHeadLen;/*jump the PES header*/
            DataPhyAddr += PesHeadLen;/*jump the PES header*/
            DataKerAddr += PesHeadLen;/*jump the PES header*/
        }
        else if (-2 == ret)/*this is for PVR smooth ctrl*/
        {
            if (DataLen >= PesHeadLen)
            {
                DataPhyAddr += PesHeadLen;
                DataKerAddr += PesHeadLen;
                DataLen     -= PesHeadLen;
                EsData->u32DispTime = stDispController.u32DispTime;
                EsData->u32DispEnableFlag= stDispController.u32DispEnableFlag;
                EsData->u32DispFrameDistance= stDispController.u32DispFrameDistance;
                EsData->u32DistanceBeforeFirstFrame= stDispController.u32DistanceBeforeFirstFrame;
                EsData->u32GopNum = stDispController.u32GopNum;
                memcpy(&(ChanInfo->stLastControl), &stDispController, sizeof(Disp_Control_t));
            }
            else
            {
                HI_ERR_DEMUX("DMX Unexpect error buflen=%#x, peshead=%#x\n", DataLen, PesHeadLen);
            }
        }
    }

    if (OqRead == OqWrite)
    {
        OqInfo->u32ProcsOffset = DataPhyAddr + DataLen - StartPhyAddr;/*?*/
    }

    if (ChanInfo->ChanType == HI_UNF_DMX_CHAN_TYPE_AUD && ChanInfo->u32PesLength)
    {
        // Attention: for audio channel, del the unuse data, the length may error
        if (ChanInfo->u32PesLength < DataLen)
        {
            DataLen = ChanInfo->u32PesLength;

            ChanInfo->u32PesLength = 0;
            ChanInfo->u32PesLenErr ++;
        }
        else
        {
            ChanInfo->u32PesLength -= DataLen;
        }
    }

    EsData->u32BufPhyAddr   = DataPhyAddr;
    EsData->pu8BufVirAddr   = DataKerAddr;
    EsData->u32BufLen       = DataLen;
    EsData->u32PtsMs        = ChanInfo->LastPts;

    ChanInfo->LastPts       = INVALID_PTS;
    ChanInfo->u32HitAcq++;

    return HI_SUCCESS;
}

HI_S32 DMX_OsiReadEsRequest(HI_U32 DmxId, HI_U32 ChanId, DMX_Stream_S *EsData)
{
    HI_S32 ret = HI_SUCCESS;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if ((HI_UNF_DMX_CHAN_TYPE_VID != ChanInfo->ChanType) && (HI_UNF_DMX_CHAN_TYPE_AUD != ChanInfo->ChanType))
    {
        HI_ERR_DEMUX("channel %d type is not av\n", ChanId);

        ret = HI_ERR_DMX_NOT_SUPPORT;
        goto out1;
    }

    if (HI_UNF_DMX_CHAN_PLAY_EN != (HI_UNF_DMX_CHAN_PLAY_EN & ChanInfo->ChanStatus))
    {
        HI_WARN_DEMUX("channel %d is not open yet or not play channel\n", ChanId);

        ret = HI_ERR_DMX_NOT_OPEN_CHAN;
        goto out1;
    }

    ret = __DMX_OsiReadEsRequest(DmxSet, ChanId, EsData);
    if (HI_SUCCESS == ret)
    {
        if (HI_UNF_DMX_SECURE_MODE_TEE == ChanInfo->ChanSecure)
        {
            if (HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType)
            {
                ret = DmxFixSecureAudBuf(ChanInfo, EsData);
                if (HI_SUCCESS == ret)
                {
                    DMX_OQ_Info_S *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
                    DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];

                    /* fixup nonsecure audio buffer. */
                    EsData->pu8BufVirAddr = EsData->u32BufPhyAddr - FqInfo->BufPhyAddr + FqInfo->ShadowBufVirAddr;
                    EsData->u32BufPhyAddr = EsData->u32BufPhyAddr - FqInfo->BufPhyAddr + FqInfo->ShadowBufPhyAddr;
                }
            }
        }

#ifdef HI_DEMUX_PROC_SUPPORT
        DMX_OsrSaveEs(ChanInfo, EsData);
#endif
    }

out1:
    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

static HI_S32 __DMX_OsiReleaseReadEs(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_Stream_S *EsData)
{
    HI_S32 ret;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];
    DMX_DmxInfo_S *DmxInfo = DmxSet->Ops->GetDmxInfo(DmxSet, ChanInfo->DmxId);
    DMX_OQ_Info_S  *OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    DMX_FQ_Info_S  *FqInfo= &DmxSet->DmxFqInfo[OqInfo->u32FQId];
    HI_U32 TimeMs;

    ChanInfo->u32Release++;

    HI_DRV_SYS_GetTimeStampMs(&TimeMs);

    ChanInfo->u32RelTimeInterval = TimeMs - ChanInfo->u32RelTime;
    ChanInfo->u32RelTime = TimeMs;

    do
    {
        OQ_DescInfo_S  *OqDesc;
        HI_U32          OqRead;
        HI_U32          OqWrite;
        HI_U32          DataPhyAddr;
        HI_U32          DataLen;
        HI_U32          Size;

        DMXOsiOQGetReadWrite(DmxSet, OqInfo->u32OQId, &OqWrite, &OqRead);

        if (OqRead == OqWrite)
        {
            return HI_SUCCESS;
        }

        OqDesc = OqInfo->OqDescSetBase + OqRead;

        DataPhyAddr = OqDesc->start_addr;
        DataLen     = OqDesc->pvrctrl_datalen & 0xffff;

        if (unlikely(!(DataPhyAddr >= FqInfo->BufPhyAddr && DataPhyAddr < FqInfo->BufPhyAddr + FqInfo->BufSize)))
        {
            HI_FATAL_DEMUX("invalid desc[0x%x, %d] of fq[0x%x, %d].\n", DataPhyAddr, DataLen, FqInfo->BufPhyAddr, FqInfo->BufSize);
            BUG();
        }

        if ((EsData->u32BufPhyAddr < DataPhyAddr) || ((EsData->u32BufPhyAddr + EsData->u32BufLen) > (DataPhyAddr + DataLen)))
        {
            ret = DmxOQRelease(DmxSet, OqInfo->u32FQId, ChanInfo->ChanOqId, DataPhyAddr, DataLen);
            if (HI_SUCCESS != ret)
            {
                return ret;
            }

            DMXCheckFQBPStatus(DmxSet, DmxInfo, OqInfo->u32FQId);

            continue;
        }

        if ((EsData->u32BufPhyAddr < DataPhyAddr) || ((EsData->u32BufPhyAddr + EsData->u32BufLen) > (DataPhyAddr + DataLen)))
        {
            HI_ERR_DEMUX("channel %d release error, Rel: Addr=0x%x, len=0x%x, Oq: Addr=0x%x, len=0x%x\n",
                ChanId, EsData->u32BufPhyAddr, EsData->u32BufLen, DataPhyAddr, DataLen);

            return HI_ERR_DMX_INVALID_PARA;
        }

        Size = (EsData->u32BufPhyAddr + EsData->u32BufLen) - DataPhyAddr;
        if (Size < DataLen)
        {
            return HI_SUCCESS;
        }

        ret = DmxOQRelease(DmxSet, OqInfo->u32FQId, ChanInfo->ChanOqId, DataPhyAddr, DataLen);
        if (HI_SUCCESS == ret)
        {
            DMXCheckFQBPStatus(DmxSet, DmxInfo, OqInfo->u32FQId);
        }
    } while (0);

    return HI_SUCCESS;
}

HI_S32 DMX_OsiReleaseReadEs(HI_U32 DmxId, HI_U32 ChanId, DMX_Stream_S *EsData)
{
    HI_S32 ret = HI_SUCCESS;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *ChanInfo = HI_NULL;
    DMX_FQ_Info_S  *FqInfo;
    DMX_OQ_Info_S  *OqInfo;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if ((HI_UNF_DMX_CHAN_TYPE_VID != ChanInfo->ChanType) && (HI_UNF_DMX_CHAN_TYPE_AUD != ChanInfo->ChanType))
    {
        HI_ERR_DEMUX("channel %d type is not av\n", ChanId);

        ret = HI_ERR_DMX_NOT_SUPPORT;
        goto out1;
    }

    if (HI_UNF_DMX_CHAN_PLAY_EN != (HI_UNF_DMX_CHAN_PLAY_EN & ChanInfo->ChanStatus))
    {
        HI_WARN_DEMUX("channel %d is not open yet or not play channel\n", ChanId);

        ret = HI_ERR_DMX_NOT_OPEN_CHAN;
        goto out1;
    }

    OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    FqInfo= &DmxSet->DmxFqInfo[OqInfo->u32FQId];

    if (HI_UNF_DMX_SECURE_MODE_TEE == ChanInfo->ChanSecure)
    {
        if (HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType)
        {
            /* fixup secure audio buffer. */
            EsData->u32BufPhyAddr = EsData->u32BufPhyAddr - FqInfo->ShadowBufPhyAddr + FqInfo->BufPhyAddr;
        }
    }

    if (!(EsData->u32BufPhyAddr >= FqInfo->BufPhyAddr && EsData->u32BufPhyAddr < FqInfo->BufPhyAddr + FqInfo->BufSize))
    {
        HI_ERR_DEMUX("Release es data range[0x%x, 0x%x] is out of buff range[0x%x, 0x%x].\n", EsData->u32BufPhyAddr, EsData->u32BufPhyAddr + EsData->u32BufLen,
                                            FqInfo->BufPhyAddr, FqInfo->BufPhyAddr + FqInfo->BufSize);

        ret = HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    ret = __DMX_OsiReleaseReadEs(DmxSet, ChanId, EsData);

out1:
    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_S32 DMX_OsiGetChanBufStatus(HI_U32 DmxId, HI_U32 ChanId, HI_MPI_DMX_BUF_STATUS_S *BufStatus)
{
    HI_S32 ret = HI_SUCCESS;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S *ChanInfo = HI_NULL;
    DMX_OQ_Info_S  *OqInfo = HI_NULL;
    DMX_FQ_Info_S  *FqInfo = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];
    OqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
    FqInfo = &DmxSet->DmxFqInfo[OqInfo->u32FQId];

    if (OqInfo->enOQBufMode == DMX_OQ_MODE_UNUSED)
    {
        HI_ERR_DEMUX("channel %d OQ status not used.\n", ChanId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    BufStatus->u32BufSize =  FqInfo->BufSize;
    BufStatus->u32UsedSize = 0;
    BufStatus->u32BufRptr = 0;
    BufStatus->u32BufWptr = 0;

    if (HI_UNF_DMX_CHAN_CLOSE != ChanInfo->ChanStatus)
    {
        HI_U32 Read, Write;

        DmxHalGetFQReadWritePtr(DmxSet, OqInfo->u32FQId, &Read, &Write);

        BufStatus->u32BufRptr = Read;
        BufStatus->u32BufWptr = Write;

        if (Read < Write)
        {
            BufStatus->u32UsedSize  = (FqInfo->FQDepth - 1 - Write + Read) * FqInfo->BlockSize;
        }
        else if (Read > Write)
        {
            BufStatus->u32UsedSize  = (Read - Write) * FqInfo->BlockSize;
        }
    }

    ret = HI_SUCCESS;

out1:
    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

static HI_S32 __DmxSelectData(HI_HANDLE *WatchHandleSet, HI_BOOL *ValidFlag, HI_U32 WatchCnt, HI_U32 TimeOutMs)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 index;
    HI_HANDLE ChanHandle;
    HI_U32 DmxId , ChanId, Cnt = 0;

    for (index = 0; index < WatchCnt; index ++)
    {
        ChanHandle = WatchHandleSet[index];

        /* ChanHandle maybe become invalid */
        DmxId = DMX_DMXID(ChanHandle);
        ChanId = DMX_CHANID(ChanHandle);

        ret = DMX_OsiGetChnDataFlag(GetDmxSetByDmxid(DmxId), ChanId);
        if (HI_SUCCESS == ret)
        {
            ValidFlag[index] = HI_TRUE;
            Cnt ++;
        }
    }

    return Cnt ? HI_SUCCESS : HI_ERR_DMX_NOAVAILABLE_DATA;
}

static HI_S32 DmxEnChnEop(Dmx_Set_S *DmxSet, HI_U32 ChanId )
{
    HI_S32 ret = HI_FAILURE;
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    if ((HI_UNF_DMX_CHAN_TYPE_AUD == ChanInfo->ChanType) || (HI_UNF_DMX_CHAN_TYPE_VID == ChanInfo->ChanType))
    {
        ret =  HI_FAILURE;   // do not receive vid and aud channel
        goto out1;
    }

    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY != (ChanInfo->ChanOutMode & HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY))
    {
        ret =  HI_FAILURE;   // do not receive rec channel
        goto out1;
    }

    DmxHalEnableOQEopInt(DmxSet, ChanInfo->ChanOqId);

    ret = HI_SUCCESS;

out1:
    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

static HI_S32 DmxDisChnEop(Dmx_Set_S *DmxSet, HI_U32 OqId)
{
    DmxHalDisableOQEopInt(DmxSet, OqId);

    return HI_SUCCESS;
}

static inline HI_S32 DmxSelectDataTimeout(HI_HANDLE *WatchHandleSet, HI_U32 WatchCnt, HI_U32 TimeOutMs)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 index;
    HI_HANDLE ChanHandle;
    HI_U32 DmxId , ChanId ;
    wait_queue_head_t WaitQueue;
    unsigned long flags;

    mutex_lock(&g_SelectLock);

    BUG_ON(GetDmxCluster()->Ops->GetChnNum() > sizeof(g_EnEopIntMark) /sizeof(g_EnEopIntMark[0]));
    BUG_ON(WatchCnt > sizeof(g_EnEopIntMark) /sizeof(g_EnEopIntMark[0]));
    BUG_ON(g_SelectWaitQueue);

    init_waitqueue_head(&WaitQueue);

    memset(g_EnEopIntMark, 0 , sizeof(g_EnEopIntMark));

    for (index = 0; index < WatchCnt; index ++)
    {
        Dmx_Set_S *DmxSet = HI_NULL;

        ChanHandle = WatchHandleSet[index];

        DmxId = DMX_DMXID(ChanHandle);
        ChanId = DMX_CHANID(ChanHandle);

        DmxSet = GetDmxSetByDmxid(DmxId);

        if (HI_SUCCESS == DmxGetChnInstance(DmxSet, ChanId))
        {
            DMX_ChanInfo_S *ChanInfo  = &DmxSet->DmxChanInfo[ChanId];

            g_EnEopIntMark[index].ValidFlag = HI_TRUE;
            g_EnEopIntMark[index].DmxSet = DmxSet;
            g_EnEopIntMark[index].OQId = ChanInfo->ChanOqId;
            g_EnEopIntMark[index].ChanId = ChanId;
            INIT_WORK(&g_EnEopIntMark[index].work, DmxEopWakeUpWork);

            DmxPutChnInstance(DmxSet, ChanId);
        }
    }

    spin_lock_irqsave(&g_WaitQueueLock, flags);
    g_SelectWaitQueue = &WaitQueue;
    g_SelectCondition = 0;
    spin_unlock_irqrestore(&g_WaitQueueLock, flags);

    for (index = 0; index < WatchCnt; index ++)
    {
        if (HI_TRUE == g_EnEopIntMark[index].ValidFlag)
        {
            if (HI_SUCCESS != DmxEnChnEop(g_EnEopIntMark[index].DmxSet, g_EnEopIntMark[index].ChanId))
            {
                g_EnEopIntMark[index].ValidFlag = HI_FALSE;
            }
        }
    }

    ret = wait_event_interruptible_timeout(*g_SelectWaitQueue, g_SelectCondition, msecs_to_jiffies(TimeOutMs));

    for (index = 0; index < WatchCnt; index ++)
    {
        /* channel maybe changed invalid, but we must disable former responding eop */
        if (HI_TRUE == g_EnEopIntMark[index].ValidFlag)
        {
            DmxDisChnEop(g_EnEopIntMark[index].DmxSet, g_EnEopIntMark[index].OQId);
        }
    }

    spin_lock_irqsave(&g_WaitQueueLock, flags);
    g_SelectCondition = 0;
    g_SelectWaitQueue = HI_NULL;
    spin_unlock_irqrestore(&g_WaitQueueLock, flags);

    flush_workqueue(g_SelectWorkQueue);

    mutex_unlock(&g_SelectLock);

    return ret;
}

static HI_S32 DmxSelectData(HI_HANDLE *WatchHandleSet, HI_U32 WatchCnt, HI_HANDLE *ValidHandleSet, HI_U32 *ValidCnt, HI_U32 TimeOutMs)
{
    HI_S32 ret = HI_FAILURE;
    HI_BOOL ValidFlag[WatchCnt];
    HI_U32 index, cnt = 0;

    do
    {
        memset(ValidFlag, HI_FALSE, sizeof(HI_BOOL) * WatchCnt);

        ret = __DmxSelectData(WatchHandleSet, ValidFlag, WatchCnt, TimeOutMs);
        if (HI_SUCCESS == ret)
        {
            break;
        }

        if (TimeOutMs)
        {
            ret = DmxSelectDataTimeout(WatchHandleSet, WatchCnt, TimeOutMs);
            if (0 == ret)
            {
                ret = HI_ERR_DMX_TIMEOUT;
                break;
            }
            else if (ret < 0)
            {
                break;
            }

            /* ret : remain timeout value. */
            TimeOutMs = jiffies_to_msecs(ret);

            continue;
        }
        else
        {
            break;
        }
    }while(1);

    if (HI_SUCCESS == ret)
    {
        for(index = 0; index < WatchCnt; index++)
        {
            if (HI_TRUE == ValidFlag[index])
            {
                ValidHandleSet[cnt ++] = WatchHandleSet[index];
            }
        }

        *ValidCnt = cnt;
    }

    return ret;
}

HI_S32 DMX_OsiSelectDataFlag(HI_HANDLE *WatchHandleSet, HI_U32 WatchCnt, HI_HANDLE *ValidHandleSet, HI_U32 *ValidCnt, HI_U32 TimeOutMs)
{
    return DmxSelectData(WatchHandleSet, WatchCnt, ValidHandleSet, ValidCnt, TimeOutMs);
}

HI_S32 DMX_OsiGetDataFlag(HI_U32 *ValidHandleSet, HI_U32 *ValidCnt, HI_U32 TimeOutMs)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    HI_U32 TotalChanNum = DmxCluster->Ops->GetChnNum();
    HI_HANDLE WatchHandle[TotalChanNum];
    Dmx_Set_S *DmxSet = HI_NULL;
    HI_U32 index = 0, cnt = 0;

    memset(WatchHandle, 0, sizeof(HI_HANDLE) * TotalChanNum);

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        mutex_lock(&DmxSet->LockAllChn);

        for_each_set_bit(index, DmxSet->ChnBitmap, DmxSet->DmxChanCnt)
        {
            DMX_ChanInfo_S *DmxChanInfo = &DmxSet->DmxChanInfo[index];

            if (HI_SUCCESS == DmxGetChnInstance(DmxSet, DmxChanInfo->ChanId))
            {
                WatchHandle[cnt++] = DMX_CHANHANDLE(DmxChanInfo->DmxId, DmxChanInfo->ChanId);

                DmxPutChnInstance(DmxSet, DmxChanInfo->ChanId);
            }
        }

        mutex_unlock(&DmxSet->LockAllChn);
    }

    return DmxSelectData(WatchHandle, cnt > *ValidCnt ? *ValidCnt : cnt, ValidHandleSet, ValidCnt, TimeOutMs);
}

/*
 * pcr res acquire and release helper functions.
 */
static HI_S32 DmxPcrAcquire(HI_U32 DmxId, HI_U32 *PcrId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_PCR_Info_S  *PcrInfo = HI_NULL;

    BUG_ON(!mutex_is_locked(&DmxSet->LockAllPcr));

    *PcrId = find_first_zero_bit(DmxSet->PcrBitmap, DmxSet->DmxPcrCnt);
    if (!(*PcrId < DmxSet->DmxPcrCnt))
    {
        HI_ERR_DEMUX("There is no available pcr res now!\n");
        ret = HI_ERR_DMX_NOFREE_CHAN;
        goto out;
    }

    PcrInfo = &DmxSet->DmxPcrInfo[*PcrId];

    mutex_lock(&PcrInfo->LockPcr);

    PcrInfo->DmxId      = DmxId;
    PcrInfo->PcrPid     = DMX_INVALID_PID;
    PcrInfo->Owner     = task_tgid_nr(current);;
    PcrInfo->SyncHandle = DMX_INVALID_SYNC_HANDLE;
    PcrInfo->PcrValue   = 0xffffffff;
    PcrInfo->ScrValue   = 0xffffffff;

    DmxHalSetPcrPid(DmxSet, *PcrId, DMX_INVALID_PID);

    set_bit(*PcrId, DmxSet->PcrBitmap);

    mutex_unlock(&PcrInfo->LockPcr);

    ret = HI_SUCCESS;

out:
    return ret;
}

static inline HI_VOID DmxPcrRelease(HI_U32 DmxId, HI_U32 PcrId)
{
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_PCR_Info_S *PcrInfo = &DmxSet->DmxPcrInfo[PcrId];

    BUG_ON(PcrId >= DmxSet->DmxPcrCnt);
    BUG_ON(!mutex_is_locked(&PcrInfo->LockPcr));

    DmxHalSetPcrIntEnable(DmxSet, PcrId, HI_FALSE);

    PcrInfo->DmxId = DMX_INVALID_DEMUX_ID;

    clear_bit(PcrId, DmxSet->PcrBitmap);
}

/*
 * exclusively get and put pcr helper functions.
 */
HI_S32 DmxGetPcrInstance(Dmx_Set_S *DmxSet, HI_U32 PcrId)
{
    HI_S32 ret = HI_FAILURE;
    unsigned long mask, *p;
    DMX_PCR_Info_S  *PcrInfo = HI_NULL;

    if (unlikely(!DmxSet))
    {
        HI_ERR_DEMUX("Invalid DemuxSet.\n");
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    if (unlikely(PcrId >= DmxSet->DmxPcrCnt))
    {
        HI_ERR_DEMUX("Invalid filter id :0x%x\n", PcrId);

        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    PcrInfo = &DmxSet->DmxPcrInfo[PcrId];

    if (unlikely(0 != mutex_lock_interruptible(&PcrInfo->LockPcr)))
    {
        HI_WARN_DEMUX("Pcr mutex interruptibled.\n");

        ret = HI_ERR_DMX_BUSY_FILTER;
        goto out0;
    }

    mask = BIT_MASK(PcrId);
    p = ((unsigned long*)DmxSet->PcrBitmap) + BIT_WORD(PcrId);
    if (unlikely(!(*p & mask)))
    {
        HI_ERR_DEMUX("Try to Get invalid Pcr(%d) instance.\n", PcrId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    return HI_SUCCESS;

out1:
    mutex_unlock(&PcrInfo->LockPcr);
out0:
    return ret;
}

HI_VOID DmxPutPcrInstance(Dmx_Set_S *DmxSet, HI_U32 PcrId)
{
    DMX_PCR_Info_S  *PcrInfo = &DmxSet->DmxPcrInfo[PcrId];

    BUG_ON(PcrId >= DmxSet->DmxPcrCnt);

    mutex_unlock(&PcrInfo->LockPcr);
}

HI_S32 DMX_OsiPcrChannelCreate(const HI_U32 DmxId, HI_U32 *PcrId)
{
    HI_S32 ret = HI_ERR_DMX_NOFREE_CHAN;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    if (!DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    mutex_lock(&DmxSet->LockAllPcr);

    ret = DmxPcrAcquire(DmxId, PcrId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

out1:
    mutex_unlock(&DmxSet->LockAllPcr);
out0:
    return ret;
}

HI_S32 DMX_OsiPcrChannelDestroy(const HI_U32 DmxId, const HI_U32 PcrId)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    ret = DmxGetPcrInstance(DmxSet, PcrId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    DmxPcrRelease(DmxId, PcrId);

    DmxPutPcrInstance(DmxSet, PcrId);
out:
    return ret;
}

HI_S32 DMX_OsiPcrChannelSetPid(const HI_U32 DmxId, const HI_U32 PcrId, const HI_U32 PcrPid)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_PCR_Info_S *PcrInfo = HI_NULL;

    ret = DmxGetPcrInstance(DmxSet, PcrId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    PcrInfo = &DmxSet->DmxPcrInfo[PcrId];

    if (PcrPid > DMX_INVALID_PID)
    {
        HI_ERR_DEMUX("Invalid parameter: PcrPid=0x%x, DmxId=%u\n", PcrPid, PcrInfo->DmxId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    if (PcrPid < DMX_INVALID_PID)
    {
        HI_U32 i;

        for (i = 0; i < DmxSet->DmxPcrCnt; i++)
        {
            if ((DmxSet->DmxPcrInfo[i].DmxId == PcrInfo->DmxId) && (i != PcrId))
            {
                if (DmxSet->DmxPcrInfo[i].PcrPid == PcrPid)
                {
                    PcrInfo->PcrPid = PcrPid;
                    HI_ERR_DEMUX("chanel(%x) pcr pid(%x) already used.\n", PcrId, PcrPid);

                    ret = HI_SUCCESS;
                    goto out1;
                }
            }
        }
    }

    PcrInfo->PcrPid = PcrPid;
    DmxHalSetPcrPid(DmxSet, PcrId, PcrPid);
    DmxHalSetPcrDmxId(DmxSet, PcrId, PcrInfo->DmxId);
    DmxHalSetPcrIntEnable(DmxSet, PcrId, HI_TRUE);

    ret = HI_SUCCESS;

out1:
    DmxPutPcrInstance(DmxSet, PcrId);
out0:
    return ret;
}

HI_S32 DMX_OsiPcrChannelGetPid(const HI_U32 DmxId, const HI_U32 PcrId, HI_U32 *PcrPid)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_PCR_Info_S *PcrInfo = HI_NULL;

    ret = DmxGetPcrInstance(DmxSet, PcrId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    PcrInfo = &DmxSet->DmxPcrInfo[PcrId];

    *PcrPid = PcrInfo->PcrPid;

    ret = HI_SUCCESS;

    DmxPutPcrInstance(DmxSet, PcrId);
out:
    return ret;
}

HI_S32 DMX_OsiPcrChannelGetClock(const HI_U32 DmxId, const HI_U32 PcrId, HI_U64 *PcrValue, HI_U64 *ScrValue)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_PCR_Info_S *PcrInfo = HI_NULL;

    ret = DmxGetPcrInstance(DmxSet, PcrId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    PcrInfo = &DmxSet->DmxPcrInfo[PcrId];

    *PcrValue = PcrInfo->PcrValue;
    *ScrValue = PcrInfo->ScrValue;

    ret = HI_SUCCESS;

    DmxPutPcrInstance(DmxSet, PcrId);
out:
    return ret;
}

HI_S32 DMX_OsiPcrChannelAttachSync(const HI_U32 DmxId, const HI_U32 PcrId, const HI_U32 SyncHadle)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_PCR_Info_S *PcrInfo = HI_NULL;

    ret = DmxGetPcrInstance(DmxSet, PcrId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    PcrInfo = &DmxSet->DmxPcrInfo[PcrId];

    PcrInfo->SyncHandle = SyncHadle;

    ret = HI_SUCCESS;

    DmxPutPcrInstance(DmxSet, PcrId);
out:
    return ret;
}

HI_S32 DMX_OsiPcrChannelDetachSync(const HI_U32 DmxId, const HI_U32 PcrId)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_PCR_Info_S *PcrInfo = HI_NULL;

    ret = DmxGetPcrInstance(DmxSet, PcrId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    PcrInfo = &DmxSet->DmxPcrInfo[PcrId];

    PcrInfo->SyncHandle = DMX_INVALID_SYNC_HANDLE;

    ret = HI_SUCCESS;

    DmxPutPcrInstance(DmxSet, PcrId);
out:
    return ret;
}

static HI_S32 DmxCreateRec(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_UNF_DMX_REC_ATTR_S *RecAttr);
static HI_S32 DmxDestroyRec(Dmx_Set_S *DmxSet, HI_U32 RecId);

#ifndef DMX_SCD_VERSION_2
static HI_S32 DmxRecAcquire(HI_UNF_DMX_REC_ATTR_S *RecAttr, DMX_REC_TIMESTAMP_MODE_E enRecTimeStamp, HI_U32 *RecId, DMX_MMZ_BUF_S *RecBuf)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(RecAttr->u32DmxId);
    DMX_DmxInfo_S *DmxInfo = DmxSet->Ops->GetDmxInfo(DmxSet, RecAttr->u32DmxId);
    DMX_RecInfo_S *RecInfo = HI_NULL;

    BUG_ON(!mutex_is_locked(&DmxSet->LockAllRec));

    if (!(atomic_read(&DmxInfo->RecLimit.RecInstCounter) < DMX_1WAY_REC_CNT))
    {
        HI_ERR_DEMUX("This demux(%d) has %d record instance now!\n", RecAttr->u32DmxId, atomic_read(&DmxInfo->RecLimit.RecInstCounter));
        ret = HI_ERR_DMX_NOFREE_CHAN;
        goto out0;
    }

    *RecId = DmxSet->Ops->GetDmxRawId(DmxSet, RecAttr->u32DmxId);
    RecInfo = &DmxSet->DmxRecInfo[*RecId];

    mutex_lock(&RecInfo->LockRec);

    if (DMX_INVALID_DEMUX_ID != RecInfo->DmxId)
    {
        HI_ERR_DEMUX("Invalid Rec Id(%d)!\n", *RecId);
        ret = HI_ERR_DMX_NOFREE_CHAN;
        goto out1;
    }

    ret = DmxCreateRec(DmxSet, *RecId, RecAttr);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    if (RecBuf)
    {
        DMX_FQ_Info_S  *RecFqInfo = &DmxSet->DmxFqInfo[RecInfo->RecFqId];

        RecBuf->VirAddr = RecFqInfo->BufVirAddr;
        RecBuf->PhyAddr = RecFqInfo->BufPhyAddr;
        RecBuf->Size = RecFqInfo->BufSize;
        RecBuf->Flag = RecFqInfo->BufFlag;
    }

    if (enRecTimeStamp >= DMX_REC_TIMESTAMP_ZERO)
    {
        RecInfo->enRecTimeStamp = enRecTimeStamp;

        DmxHalConfigRecTsTimeStamp(DmxSet, RecInfo->DmxId, RecInfo->enRecTimeStamp);
    }

    set_bit(*RecId, DmxSet->RecBitmap);

    atomic_inc(&DmxInfo->RecLimit.RecInstCounter);

    mutex_unlock(&RecInfo->LockRec);

    return HI_SUCCESS;

out1:
    mutex_unlock(&RecInfo->LockRec);
out0:
    return ret;
}

static HI_S32 DmxRecRelease(HI_U32 DmxId, HI_U32 RecId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_RecInfo_S * RecInfo = &DmxSet->DmxRecInfo[RecId];
    DMX_DmxInfo_S *DmxInfo = DmxSet->Ops->GetDmxInfo(DmxSet, DmxId);

    BUG_ON(RecId >= DmxSet->DmxRecCnt);
    BUG_ON(!mutex_is_locked(&RecInfo->LockRec));

    ret = DmxDestroyRec(DmxSet, RecId);

    clear_bit(RecId, DmxSet->RecBitmap);

    atomic_dec(&DmxInfo->RecLimit.RecInstCounter);

    return ret;
}

static HI_S32 DmxScdAcquire(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 *ScdId)
{
    HI_U32 RawDmxId;

    BUG_ON(DmxSet != GetDmxSetByDmxid(DmxId));

    RawDmxId = DmxSet->Ops->GetDmxRawId(DmxSet, DmxId);

    *ScdId = RawDmxId + 1;

    return HI_SUCCESS;
}

#define DmxScdRelease(DmxSet, ScdId) ({ \
})

static HI_S32 DmxSetRecType(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_UNF_DMX_REC_TYPE_E RecType)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];

    switch(RecType)
    {
        case HI_UNF_DMX_REC_TYPE_ALL_PID:
        {
            DmxHalSetRecType(DmxSet, RecInfo->DmxId, DMX_REC_TYPE_ALL_TS);

#ifdef DMX_REC_EXCLUDE_PID_SUPPORT
            DmxHalEnableAllRecExcludePid(DmxSet, RecInfo->DmxId);
#endif
        }
        break;

        case HI_UNF_DMX_REC_TYPE_SELECT_PID:
        {
            DmxHalSetRecType(DmxSet, RecInfo->DmxId, RecInfo->Descramed ? DMX_REC_TYPE_DESCRAM_TS : DMX_REC_TYPE_SCRAM_TS);
        }
        break;

        case HI_UNF_DMX_REC_TYPE_BUTT:
        {
            DmxHalSetRecType(DmxSet, RecInfo->DmxId, DMX_REC_TYPE_NONE);
        }
        break;

        default:
            BUG();
    }

    return HI_SUCCESS;
}

static HI_VOID DmxSetRecBuf(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 OqId)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];

    DmxHalSetDmxRecBufId(DmxSet, RecInfo->DmxId, OqId);
}

static HI_VOID DmxUnsetRecBuf(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];

    DmxHalSetDmxRecBufId(DmxSet, RecInfo->DmxId, DMX_INVALID_OQ_ID);
}

static HI_S32 DmxStartSCD(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];
    HI_U32 ChnId;
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    /* find scd reference channel */
    for_each_set_bit(ChnId, RecInfo->RecChnBitmap, DmxSet->DmxChanCnt)
    {
        ChanInfo = &DmxSet->DmxChanInfo[ChnId];

        BUG_ON(ChanInfo->DmxId != RecInfo->DmxId);
        BUG_ON(!(HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & ChanInfo->ChanOutMode));

        if (ChanInfo->ChanPid == RecInfo->IndexPid)
        {
            /* get rid of POST mode from channel whatever */
            DmxHalSetChannelTsPostMode(DmxSet, ChanInfo->ChanId, DMX_DISABLE);

            DmxHalSetSCDAttachChannel(DmxSet, RecInfo->ScdId, ChanInfo->ChanId);
            break;
        }
    }

    DmxHalSetSCDBufferId(DmxSet, RecInfo->ScdId, RecInfo->ScdOqId);
    DmxHalClearRecTsCnt(DmxSet, RecId);
    DmxHalEnableEsSCD(DmxSet, RecInfo->ScdId);
    DmxHalEnablePesSCD(DmxSet, RecInfo->ScdId);

    if (HI_UNF_DMX_REC_INDEX_TYPE_VIDEO == RecInfo->IndexType)
    {
        HI_INFO_DEMUX("VCodecType=%d\n", RecInfo->VCodecType);

        if (HI_UNF_VCODEC_TYPE_MPEG2 == RecInfo->VCodecType)
        {
            DmxHalSetScdFilter(DmxSet, 0, 0x00);
            DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 0);

            DmxHalSetScdFilter(DmxSet, 1, 0xb3);
            DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 1);
        }
        else if (HI_UNF_VCODEC_TYPE_AVS == RecInfo->VCodecType)
        {
            DmxHalSetScdFilter(DmxSet, 1, 0xb3);
            DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 1);

            DmxHalSetScdFilter(DmxSet, 2, 0xb6);
            DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 2);

            DmxHalSetScdRangeFilter(DmxSet, 0, 0xb1, 0xb0);
            DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 0);
        }
        else if (HI_UNF_VCODEC_TYPE_MPEG4 == RecInfo->VCodecType)
        {
            DmxHalSetScdFilter(DmxSet, 3, 0xb0);
            DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 3);

            DmxHalSetScdRangeFilter(DmxSet, 1, 0xb6, 0xb5);
            DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 1);

            DmxHalSetScdRangeFilter(DmxSet, 2, 0x2f, 0x00);
            DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 2);
        }
        else if (HI_UNF_VCODEC_TYPE_H264 == RecInfo->VCodecType)
        {
#ifndef DMX_SCD_NEW_FLT_SUPPORT
            DmxHalSetScdFilter(DmxSet, 4, 0x01);
            DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 4);

            DmxHalSetScdFilter(DmxSet, 5, 0x05);
            DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 5);

            DmxHalSetScdRangeFilter(DmxSet, 3, 0x08, 0x07);
            DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 3);

            DmxHalSetScdFilter(DmxSet, 6, 0x11);
            DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 6);

            DmxHalSetScdFilter(DmxSet, 7, 0x15);
            DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 7);

            DmxHalSetScdRangeFilter(DmxSet, 4, 0x18, 0x17);
            DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 4);

            DmxHalSetScdFilter(DmxSet, 8, 0x21);
            DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 8);

            DmxHalSetScdFilter(DmxSet, 9, 0x25);
            DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 9);

            DmxHalSetScdRangeFilter(DmxSet, 5, 0x28, 0x27);
            DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 5);

            DmxHalSetScdRangeFilter(DmxSet, 6, 0x78, 0x31);
            DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 6);
#else
            DmxHalSetScdNewRangeFilter(DmxSet, 0, 0xFF, 0x79, 0xFF, HI_TRUE);
            DmxHalSetScdNewRangeFilter(DmxSet, 1, 0x01, 0x01, 0x1F, HI_FALSE);
            DmxHalSetScdNewRangeFilter(DmxSet, 2, 0x05, 0x05, 0x1F, HI_FALSE);
            DmxHalSetScdNewRangeFilter(DmxSet, 3, 0x08, 0x07, 0x1F, HI_FALSE);

            DmxHalEnScdNewRangeFilter(DmxSet, RecInfo->ScdId, 0);
            DmxHalEnScdNewRangeFilter(DmxSet, RecInfo->ScdId, 1);
            DmxHalEnScdNewRangeFilter(DmxSet, RecInfo->ScdId, 2);
            DmxHalEnScdNewRangeFilter(DmxSet, RecInfo->ScdId, 3);
#endif
        }
        else if (HI_UNF_VCODEC_TYPE_HEVC == RecInfo->VCodecType)
        {
            DmxHalSetScdNewRangeFilter(DmxSet, 4, 0x12, 0x00, 0x7E, HI_FALSE); /* [0x0 ~ 0x9] << 1 */
            DmxHalSetScdNewRangeFilter(DmxSet, 5, 0x2a, 0x20, 0x7E, HI_FALSE); /* [0x10 ~ 0x15] << 1 */
            DmxHalSetScdNewRangeFilter(DmxSet, 6, 0x50, 0x40, 0x7E, HI_FALSE); /* [0x20 ~ 0x28] << 1 */

            DmxHalEnScdNewRangeFilter(DmxSet, RecInfo->ScdId, 4);
            DmxHalEnScdNewRangeFilter(DmxSet, RecInfo->ScdId, 5);
            DmxHalEnScdNewRangeFilter(DmxSet, RecInfo->ScdId, 6);
        }
    }

    DmxFqStart(DmxSet, RecInfo->ScdFqId);
    DmxOqStart(DmxSet, RecInfo->ScdOqId);

    return HI_SUCCESS;
}

static HI_S32 DmxStopSCD(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];
    DMX_OQ_Info_S  *ScdOqInfo = &DmxSet->DmxOqInfo[RecInfo->ScdOqId];

    ScdOqInfo->OqWakeUp = HI_TRUE;

    wake_up_interruptible(&ScdOqInfo->OqWaitQueue);

    DmxFqStop(DmxSet, RecInfo->ScdFqId);
    DmxOqStop(DmxSet, RecInfo->ScdOqId);

    DmxHalDisableEsSCD(DmxSet, RecInfo->ScdId);
    DmxHalDisablePesSCD(DmxSet, RecInfo->ScdId);

    DmxHalClrScdFilter(DmxSet, RecInfo->ScdId);
    DmxHalClrScdRangeFilter(DmxSet, RecInfo->ScdId);
    DmxHalClrScdNewRangeFilter(DmxSet, RecInfo->ScdId);

    DmxHalSetSCDAttachChannel(DmxSet, RecInfo->ScdId, DMX_INVALID_CHAN_ID);

     DmxHalFlushScdChn(DmxSet, RecInfo->ScdId);

    return HI_SUCCESS;
}

static HI_VOID DmxRecFlush(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];
    HI_U32 ChnId;
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    for_each_set_bit(ChnId, RecInfo->RecChnBitmap, DmxSet->DmxChanCnt)
    {
        ChanInfo = &DmxSet->DmxChanInfo[ChnId];

        BUG_ON(RecInfo->DmxId != ChanInfo->DmxId);
        BUG_ON(!(HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & ChanInfo->ChanOutMode));
        BUG_ON(atomic_read(&ChanInfo->ChanRecShareCnt));

        DmxFlushChannel(DmxSet, ChanInfo->ChanId, DMX_FLUSH_TYPE_REC);
    }

    DmxFlushOq(DmxSet, RecInfo->RecOqId, DMX_OQ_CLEAR_TYPE_REC);

    if (HI_UNF_DMX_REC_INDEX_TYPE_NONE != RecInfo->IndexType)
    {
        DmxHalFlushScdBuf(DmxSet, RecInfo->ScdId);
        DmxHalClearRecTsCnt(DmxSet, RecId);
        DmxFlushOq(DmxSet, RecInfo->ScdOqId, DMX_OQ_CLEAR_TYPE_SCD);
    }
}
#else
/*
 * rec res acquire and release helper functions.
 */
static HI_S32 DmxRecAcquire(HI_UNF_DMX_REC_ATTR_S *RecAttr, DMX_REC_TIMESTAMP_MODE_E enRecTimeStamp, HI_U32 *RecId, DMX_MMZ_BUF_S *RecBuf)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(RecAttr->u32DmxId);
    DMX_DmxInfo_S *DmxInfo = DmxSet->Ops->GetDmxInfo(DmxSet, RecAttr->u32DmxId);
    DMX_RecInfo_S *RecInfo = HI_NULL;

    BUG_ON(!mutex_is_locked(&DmxSet->LockAllRec));

    if (!(atomic_read(&DmxInfo->RecLimit.RecInstCounter) < DMX_1WAY_REC_CNT))
    {
        HI_ERR_DEMUX("This demux(%d) has %d record instance now!\n", RecAttr->u32DmxId, atomic_read(&DmxInfo->RecLimit.RecInstCounter));
         ret = HI_ERR_DMX_NOFREE_CHAN;
         goto out0;
    }

    *RecId = find_first_zero_bit(DmxSet->RecBitmap, DmxSet->DmxRecCnt);
    if (!(*RecId < DmxSet->DmxRecCnt))
    {
        HI_ERR_DEMUX("There is no available record res now!\n");
        ret = HI_ERR_DMX_NOFREE_CHAN;
        goto out0;
    }

    RecInfo = &DmxSet->DmxRecInfo[*RecId];

    mutex_lock(&RecInfo->LockRec);

    ret = DmxCreateRec(DmxSet, *RecId, RecAttr);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    if (RecBuf)
    {
        DMX_FQ_Info_S  *RecFqInfo = &DmxSet->DmxFqInfo[RecInfo->RecFqId];

        RecBuf->VirAddr = RecFqInfo->BufVirAddr;
        RecBuf->PhyAddr = RecFqInfo->BufPhyAddr;
        RecBuf->Size = RecFqInfo->BufSize;
        RecBuf->Flag = RecFqInfo->BufFlag;
    }

    if (enRecTimeStamp >= DMX_REC_TIMESTAMP_ZERO)
    {
        RecInfo->enRecTimeStamp = enRecTimeStamp;

        DmxHalConfigRecTsTimeStamp(DmxSet, RecInfo->DmxId, RecInfo->enRecTimeStamp);
    }

    set_bit(*RecId, DmxSet->RecBitmap);

    atomic_inc(&DmxInfo->RecLimit.RecInstCounter);

    mutex_unlock(&RecInfo->LockRec);

    return HI_SUCCESS;

out1:
    mutex_unlock(&RecInfo->LockRec);
out0:
    return ret;
}

static HI_S32 DmxRecRelease(HI_U32 DmxId, HI_U32 RecId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_RecInfo_S * RecInfo = &DmxSet->DmxRecInfo[RecId];
    DMX_DmxInfo_S *DmxInfo = DmxSet->Ops->GetDmxInfo(DmxSet, DmxId);

    BUG_ON(RecId >= DmxSet->DmxRecCnt);
    BUG_ON(!mutex_is_locked(&RecInfo->LockRec));

    ret = DmxDestroyRec(DmxSet, RecId);

    clear_bit(RecId, DmxSet->RecBitmap);

    atomic_dec(&DmxInfo->RecLimit.RecInstCounter);

    return ret;
}

/*
 * scd acquire and release helper functions.
 */
static HI_S32 DmxScdAcquire(Dmx_Set_S *DmxSet, HI_U32 DmxId, HI_U32 *ScdId)
{
    HI_S32 ret = HI_ERR_DMX_NOFREE_CHAN;

    BUG_ON(DmxSet != GetDmxSetByDmxid(DmxId));

    do
    {
        HI_U32 index = find_first_zero_bit(DmxSet->ScdBitmap, DmxSet->DmxScdCnt);

        if (index < DmxSet->DmxScdCnt)
        {
            DMX_SCD_Info_S *ScdInfo = &DmxSet->DmxScdInfo[index];

            spin_lock(&ScdInfo->LockScd);

            /* double check */
            if (index == find_first_zero_bit(DmxSet->ScdBitmap, DmxSet->DmxScdCnt))
            {
                set_bit(index, DmxSet->ScdBitmap);
                *ScdId = index;

                ret = HI_SUCCESS;
            }

            spin_unlock(&ScdInfo->LockScd);
        }
        else
        {
            HI_ERR_DEMUX("There is no available SCD now!\n");
            break;
        }
    }while(HI_SUCCESS != ret);

    return ret;
}

static HI_VOID DmxScdRelease(Dmx_Set_S *DmxSet, HI_U32 ScdId)
{
    DMX_SCD_Info_S *ScdInfo = &DmxSet->DmxScdInfo[ScdId];

    BUG_ON(ScdId >= DmxSet->DmxScdCnt);

    spin_lock(&ScdInfo->LockScd);

    clear_bit(ScdId, DmxSet->ScdBitmap);

    spin_unlock(&ScdInfo->LockScd);
}

static HI_S32 DmxSetRecType(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_UNF_DMX_REC_TYPE_E RecType)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];

    switch(RecType)
    {
        case HI_UNF_DMX_REC_TYPE_ALL_PID:
        {
            DmxHalSetRecType(DmxSet, RecInfo->DmxId, DMX_REC_TYPE_ALL_TS);

#ifdef DMX_REC_EXCLUDE_PID_SUPPORT
            DmxHalEnableAllRecExcludePid(DmxSet, RecInfo->DmxId);
#endif
        }
        break;

        case HI_UNF_DMX_REC_TYPE_SELECT_PID:
        {
            DmxHalSetRecType(DmxSet, RecInfo->DmxId, RecInfo->Descramed ? DMX_REC_TYPE_DESCRAM_TS : DMX_REC_TYPE_SCRAM_TS);
        }
        break;

        case HI_UNF_DMX_REC_TYPE_BUTT:
        {
            DmxHalSetRecType(DmxSet, RecInfo->DmxId, DMX_REC_TYPE_NONE);
        }
        break;

        default:
            BUG();
    }

    return HI_SUCCESS;
}

static HI_VOID DmxSetRecBuf(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 OqId)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];

    DmxHalSetRecBuf(DmxSet, RecId, OqId);
    DmxHalSetDmxRecFlag(DmxSet, RecInfo->DmxId, RecId);

    DmxHalSetDmxRecBufId(DmxSet, RecInfo->DmxId, OqId);
}

static HI_VOID DmxUnsetRecBuf(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];

    DmxHalSetDmxRecBufId(DmxSet, RecInfo->DmxId, DMX_INVALID_OQ_ID);

    DmxHalClearDmxRecFlag(DmxSet, RecInfo->DmxId, RecId);
    DmxHalSetRecBuf(DmxSet, RecId, DMX_INVALID_OQ_ID);
}

static HI_S32 DmxStartSCD(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];
    HI_U32 ChnId;
    DMX_ChanInfo_S *ChanInfo;

    /* find scd reference channel */
    for_each_set_bit(ChnId, RecInfo->RecChnBitmap, DmxSet->DmxChanCnt)
    {
        if (HI_SUCCESS == DmxGetChnInstance(DmxSet, ChnId))
        {
            ChanInfo = &DmxSet->DmxChanInfo[ChnId];

            BUG_ON(ChanInfo->DmxId != RecInfo->DmxId);
            BUG_ON(!(HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & ChanInfo->ChanOutMode));

            if (ChanInfo->ChanPid == RecInfo->IndexPid)
            {
                /* get rid of POST mode from channel whatever */
                DmxHalSetChannelTsPostMode(DmxSet, ChanInfo->ChanId, DMX_DISABLE);

                DmxHalSetSCDAttachChannel(DmxSet, RecInfo->ScdId, ChanInfo->ChanId);

                DmxPutChnInstance(DmxSet, ChnId);
                break;
            }

            DmxPutChnInstance(DmxSet, ChnId);
        }
    }

    DmxHalSetSCDBufferId(DmxSet, RecId, RecInfo->ScdOqId);
    DmxHalClearRecTsCnt(DmxSet, RecId);
    DmxHalEnableEsSCD(DmxSet, RecInfo->ScdId);
    DmxHalEnablePesSCD(DmxSet, RecInfo->ScdId);

    if (HI_UNF_DMX_REC_INDEX_TYPE_VIDEO == RecInfo->IndexType)
    {
        HI_DBG_DEMUX("VCodecType=%d\n", RecInfo->VCodecType);

        switch(RecInfo->VCodecType)
        {
            case HI_UNF_VCODEC_TYPE_MPEG2:
                DmxHalSetScdFilter(DmxSet, 0, 0x00);
                DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 0);

                DmxHalSetScdFilter(DmxSet, 1, 0xb3);
                DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 1);
                break;
            case HI_UNF_VCODEC_TYPE_AVS:
                DmxHalSetScdFilter(DmxSet, 1, 0xb3);
                DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 1);

                DmxHalSetScdFilter(DmxSet, 2, 0xb6);
                DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 2);

                DmxHalSetScdRangeFilter(DmxSet, 0, 0xb1, 0xb0);
                DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 0);
                break;
            case HI_UNF_VCODEC_TYPE_MPEG4:
                DmxHalSetScdFilter(DmxSet, 3, 0xb0);
                DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 3);

                DmxHalSetScdRangeFilter(DmxSet, 1, 0xb6, 0xb5);
                DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 1);

                DmxHalSetScdRangeFilter(DmxSet, 2, 0x2f, 0x00);
                DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 2);
                break;
            case HI_UNF_VCODEC_TYPE_H264:
                DmxHalSetScdFilter(DmxSet, 4, 0x01);
                DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 4);

                DmxHalSetScdFilter(DmxSet, 5, 0x05);
                DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 5);

                DmxHalSetScdRangeFilter(DmxSet, 3, 0x08, 0x07);
                DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 3);

                DmxHalSetScdFilter(DmxSet, 6, 0x11);
                DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 6);

                DmxHalSetScdFilter(DmxSet, 7, 0x15);
                DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 7);

                DmxHalSetScdRangeFilter(DmxSet, 4, 0x18, 0x17);
                DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 4);

                DmxHalSetScdFilter(DmxSet, 8, 0x21);
                DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 8);

                DmxHalSetScdFilter(DmxSet, 9, 0x25);
                DmxHalEnScdFilter(DmxSet, RecInfo->ScdId, 9);

                DmxHalSetScdRangeFilter(DmxSet, 5, 0x28, 0x27);
                DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 5);

                DmxHalSetScdRangeFilter(DmxSet, 6, 0x78, 0x31);
                DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 6);
                break;
            case HI_UNF_VCODEC_TYPE_HEVC:
                DmxHalSetScdRangeFilter(DmxSet, 7, 0x12, 0x0);  /* [0x0 ~ 0x9] << 1 */
                DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 7);

                DmxHalSetScdRangeFilter(DmxSet, 8, 0x2a, 0x20); /* [0x10 ~ 0x15] << 1 */
                DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 8);

                DmxHalSetScdRangeFilter(DmxSet, 9, 0x50, 0x40); /* [0x20 ~ 0x28] << 1 */
                DmxHalEnScdRangeFilter(DmxSet, RecInfo->ScdId, 9);
                break;
            default:
                BUG();
        }
    }

    DmxFqStart(DmxSet, RecInfo->ScdFqId);
    DmxOqStart(DmxSet, RecInfo->ScdOqId);

    return HI_SUCCESS;
}

static HI_S32 DmxStopSCD(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];
    DMX_OQ_Info_S  *ScdOqInfo = &DmxSet->DmxOqInfo[RecInfo->ScdOqId];

    ScdOqInfo->OqWakeUp = HI_TRUE;

    wake_up_interruptible(&ScdOqInfo->OqWaitQueue);

    DmxFqStop(DmxSet, RecInfo->ScdFqId);
    DmxOqStop(DmxSet, RecInfo->ScdOqId);

    DmxHalDisableEsSCD(DmxSet, RecInfo->ScdId);
    DmxHalDisablePesSCD(DmxSet, RecInfo->ScdId);

    DmxHalClrScdFilter(DmxSet, RecInfo->ScdId);
    DmxHalClrScdRangeFilter(DmxSet, RecInfo->ScdId);

    DmxHalSetSCDBufferId(DmxSet, RecId, DMX_INVALID_OQ_ID);
    DmxHalSetSCDAttachChannel(DmxSet, RecInfo->ScdId, DMX_INVALID_CHAN_ID);

    DmxHalFlushScdChn(DmxSet, RecInfo->ScdId);

    return HI_SUCCESS;
}

static HI_VOID DmxRecFlush(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];
    HI_U32 ChnId;
    DMX_ChanInfo_S *ChanInfo = HI_NULL;

    for_each_set_bit(ChnId, RecInfo->RecChnBitmap, DmxSet->DmxChanCnt)
    {
        ChanInfo = &DmxSet->DmxChanInfo[ChnId];

        BUG_ON(RecInfo->DmxId != ChanInfo->DmxId);
        BUG_ON(!(HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & ChanInfo->ChanOutMode));

        /* only flush non shared channel */
        if (0 == atomic_read(&ChanInfo->ChanRecShareCnt))
        {
            DmxFlushChannel(DmxSet, ChanInfo->ChanId, DMX_FLUSH_TYPE_REC);

            HI_DBG_DEMUX("flush rec(%d) non shared channel(%d) done.\n", RecId, ChanInfo->ChanId);
        }
    }

    DmxFlushOq(DmxSet, RecInfo->RecOqId, DMX_OQ_CLEAR_TYPE_REC);

    if (HI_UNF_DMX_REC_INDEX_TYPE_NONE != RecInfo->IndexType)
    {
        DmxHalFlushScdBuf(DmxSet, RecId);
        DmxHalClearRecTsCnt(DmxSet, RecId);
        DmxFlushOq(DmxSet, RecInfo->ScdOqId, DMX_OQ_CLEAR_TYPE_SCD);
    }
}
#endif

/*
 * exclusively get and put rec helper functions.
 */
HI_S32 DmxGetRecInstance(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    HI_S32 ret = HI_FAILURE;
    unsigned long mask, *p;
    DMX_RecInfo_S  *RecInfo = HI_NULL;

    if (unlikely(!DmxSet))
    {
        HI_ERR_DEMUX("Invalid DemuxSet.\n");
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    if (unlikely(RecId >= DmxSet->DmxRecCnt))
    {
        HI_ERR_DEMUX("Invalid rec id :0x%x\n", RecId);

        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    RecInfo = &DmxSet->DmxRecInfo[RecId];

    if (unlikely(0 != mutex_lock_interruptible(&RecInfo->LockRec)))
    {
        HI_WARN_DEMUX("Rec mutex interruptibled.\n");

        ret = HI_ERR_DMX_BUSY_REC;
        goto out0;
    }

    mask = BIT_MASK(RecId);
    p = ((unsigned long*)DmxSet->RecBitmap) + BIT_WORD(RecId);
    if (unlikely(!(*p & mask)))
    {
        HI_ERR_DEMUX("This Rec(%d) instance has not alloced.\n", RecId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    return HI_SUCCESS;

out1:
    mutex_unlock(&RecInfo->LockRec);
out0:
    return ret;
}

HI_VOID DmxPutRecInstance(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];

    BUG_ON(RecId >= DmxSet->DmxRecCnt);

    mutex_unlock(&RecInfo->LockRec);
}

static HI_S32 DmxTrimRecAttrs(const HI_UNF_DMX_REC_ATTR_S *InRecAttr, HI_UNF_DMX_REC_ATTR_S *OutRecAttr, DMX_REC_TIMESTAMP_MODE_E enRecTimeStamp)
{
    HI_U32 RecFqBlockSize = DMX_REC_VID_TS_PACKETS_PER_BLOCK;
    HI_UNF_DMX_SECURE_MODE_E    SecureMode = HI_UNF_DMX_SECURE_MODE_NONE;

    if (InRecAttr->u32RecBufSize < DMX_REC_MIN_BUF_SIZE)
    {
        HI_ERR_DEMUX("invalid RecAttr->u32RecBufSize:0x%x\n",InRecAttr->u32RecBufSize);
        return HI_ERR_DMX_INVALID_PARA;
    }

    memcpy(OutRecAttr, InRecAttr, sizeof(HI_UNF_DMX_REC_ATTR_S));

    switch (InRecAttr->enRecType)
    {
        case HI_UNF_DMX_REC_TYPE_SELECT_PID :
            switch (InRecAttr->enIndexType)
            {
                case HI_UNF_DMX_REC_INDEX_TYPE_VIDEO :
                    if (InRecAttr->u32IndexSrcPid >= DMX_INVALID_PID)
                    {
                        HI_ERR_DEMUX("invalid index pid:0x%x\n",InRecAttr->u32IndexSrcPid);
                        return HI_ERR_DMX_INVALID_PARA;
                    }

                    switch (InRecAttr->enVCodecType)
                    {
                        case HI_UNF_VCODEC_TYPE_MPEG2 :
                        case HI_UNF_VCODEC_TYPE_MPEG4 :
                        case HI_UNF_VCODEC_TYPE_AVS :
                        case HI_UNF_VCODEC_TYPE_H264 :
                        case HI_UNF_VCODEC_TYPE_HEVC:
                            break;

                        default :
                            HI_ERR_DEMUX("invalid vcodec type:%d\n",InRecAttr->enVCodecType);
                            return HI_ERR_DMX_INVALID_PARA;
                    }

                    RecFqBlockSize = enRecTimeStamp >= DMX_REC_TIMESTAMP_ZERO ?
                                                            DMX_REC_VID_TS_WITH_TIMESTAMP_BLOCK_SIZE : DMX_REC_VID_TS_PACKETS_PER_BLOCK;

                    break;

                case HI_UNF_DMX_REC_INDEX_TYPE_AUDIO :
                    if (InRecAttr->u32IndexSrcPid >= DMX_INVALID_PID)
                    {
                        HI_ERR_DEMUX("invalid index pid:0x%x\n",InRecAttr->u32IndexSrcPid);
                        return HI_ERR_DMX_INVALID_PARA;
                    }

                    RecFqBlockSize = enRecTimeStamp >= DMX_REC_TIMESTAMP_ZERO ?
                                                            DMX_REC_AUD_TS_WITH_TIMESTAMP_BLOCK_SIZE : DMX_REC_AUD_TS_PACKETS_PER_BLOCK;

                    break;

                case HI_UNF_DMX_REC_INDEX_TYPE_NONE :
                    break;

                default :
                    return HI_ERR_DMX_INVALID_PARA;
            }

            break;

        case HI_UNF_DMX_REC_TYPE_ALL_PID :
            OutRecAttr->enIndexType = HI_UNF_DMX_REC_INDEX_TYPE_NONE;

            break;

        default :
            HI_ERR_DEMUX("invalid RecAttr->enRecType:%d\n",InRecAttr->enRecType);
            return HI_ERR_DMX_INVALID_PARA;
    }

    switch(InRecAttr->enSecureMode)
    {
        case HI_UNF_DMX_SECURE_MODE_TEE:
            switch (InRecAttr->enRecType)
            {
                case HI_UNF_DMX_REC_TYPE_SELECT_PID:
                    break;

                default:
                    HI_ERR_DEMUX("invalid secure rec type.\n");
                    return HI_ERR_DMX_INVALID_PARA;
            }

            SecureMode = HI_UNF_DMX_SECURE_MODE_TEE;
            break;

        default:
            SecureMode = HI_UNF_DMX_SECURE_MODE_NONE;
    }

    OutRecAttr->u32RecBufSize = (InRecAttr->u32RecBufSize + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;

    /* align with block size. */
    OutRecAttr->u32RecBufSize = OutRecAttr->u32RecBufSize - OutRecAttr->u32RecBufSize % RecFqBlockSize;

    OutRecAttr->enSecureMode = SecureMode;

    return HI_SUCCESS;
}

#if defined(DMX_SECURE_CHANNEL_SUPPORT) && defined(DMX_TEE_SUPPORT)
static HI_S32 DmxTeecRegisterRecSecBuf(DMX_RecInfo_S  *RecInfo, SMMU_BUFFER_S *MmuBuf)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE);
    operation.params[0].value.a = DMX_RECHANDLE(RecInfo->DmxId, RecInfo->RecId);
    operation.params[1].value.a = MmuBuf->u32StartSmmuAddr;
    operation.params[1].value.b = MmuBuf->u32Size;

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_REGISTER_REC_SECBUF, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_REGISTER_REC_SECBUF to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

out:
    return ret;
}

static HI_S32 DmxTeecUnregisterRecSecBuf(DMX_RecInfo_S  *RecInfo, SMMU_BUFFER_S *MmuBuf)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    TEEC_Operation operation;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE);
    operation.params[0].value.a = DMX_RECHANDLE(RecInfo->DmxId, RecInfo->RecId);
    operation.params[1].value.a = MmuBuf->u32StartSmmuAddr;

    ret = DmxCluster->Ops->SendCmdToTA(TEEC_CMD_UNREGISTER_REC_SECBUF, &operation, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("send TEEC_CMD_UNREGISTER_REC_SECBUF to TA failed(0x%x).\n", ret);
        ret = HI_FAILURE;
        goto out;
    }

out:
    return ret;
}
#else
static inline HI_S32 DmxTeecRegisterRecSecBuf(DMX_RecInfo_S  *RecInfo, SMMU_BUFFER_S *MmuBuf)
{
    HI_ERR_DEMUX("DMX_SECURE_CHANNEL_SUPPORT or DMX_TEE_SUPPORT not configured.\n");
    return HI_ERR_DMX_NOT_SUPPORT;
}

static inline HI_S32 DmxTeecUnregisterRecSecBuf(DMX_RecInfo_S  *RecInfo, SMMU_BUFFER_S *MmuBuf)
{
    return HI_ERR_DMX_NOT_SUPPORT;
}
#endif

#ifdef DMX_MMU_SUPPORT
static HI_S32 DmxAllocSecureRecBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
    HI_S32 ret = HI_FAILURE;
    DMX_FQ_Info_S  *RecFqInfo = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    DMX_OQ_Info_S  *RecOqInfo = &DmxSet->DmxOqInfo[RecInfo->RecOqId];
    HI_U32 RecFqBufSize = 0, RecFqBlockSize = 0, RecFqDepth = 0;
    HI_U32 RecOqBufSize = 0, RecOqDepth = 0;
    MMZ_BUFFER_S MmzBuf = {0};
    SMMU_BUFFER_S SecMmuBuf = {0};
    HI_CHAR BufName[32];

    if (HI_UNF_DMX_SECURE_MODE_TEE != RecInfo->SecureMode)
    {
        HI_ERR_DEMUX("only secure rec can alloc secure rec buf.\n");
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    switch(RecInfo->IndexType)
    {
        case HI_UNF_DMX_REC_INDEX_TYPE_VIDEO :
            RecFqBlockSize = RecInfo->enRecTimeStamp >= DMX_REC_TIMESTAMP_ZERO ? DMX_REC_VID_TS_WITH_TIMESTAMP_BLOCK_SIZE : DMX_REC_VID_TS_PACKETS_PER_BLOCK;
            break;
        case HI_UNF_DMX_REC_INDEX_TYPE_AUDIO :
            RecFqBlockSize = RecInfo->enRecTimeStamp >= DMX_REC_TIMESTAMP_ZERO ? DMX_REC_AUD_TS_WITH_TIMESTAMP_BLOCK_SIZE : DMX_REC_AUD_TS_PACKETS_PER_BLOCK;
            break;
        case HI_UNF_DMX_REC_INDEX_TYPE_NONE :
            RecFqBlockSize = DMX_REC_VID_TS_PACKETS_PER_BLOCK;
            break;
        default:
            BUG();
    }

    /* alloc rec data buffer. */
    snprintf(BufName, sizeof(BufName), "DMX_RecBuf[%u]", RecInfo->RecFqId);

    ret = HI_DRV_SECSMMU_Alloc(BufName, RecInfo->RecBufSize, 4, &SecMmuBuf);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("smmu secure memory allocate failed, BufSize=0x%x\n", RecInfo->RecBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out0;
    }

    BUG_ON(SecMmuBuf.u32Size != RecInfo->RecBufSize);

    ret = DmxTeecRegisterRecSecBuf(RecInfo, &SecMmuBuf);
    if (HI_SUCCESS != ret)
    {
        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out1;
    }

    /* SecMmuBuf.pu8StartVirAddr is NULL, replace with SecMmuBuf.u32StartSmmuAddr. */
    RecFqInfo->BufVirAddr   = (HI_U8*)(unsigned long)SecMmuBuf.u32StartSmmuAddr;
    RecFqInfo->BufPhyAddr   = SecMmuBuf.u32StartSmmuAddr;
    RecFqInfo->BufSize      = SecMmuBuf.u32Size;
    RecFqInfo->BufFlag      = DMX_SECURE_BUF;
    RecFqInfo->BlockSize    = RecFqBlockSize;

    /* alloc dsc buffer. */
    RecFqDepth      = RecFqInfo->BufSize / RecFqInfo->BlockSize + 1;
    RecFqBufSize    = (RecFqDepth * DMX_FQ_DESC_SIZE + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;
    RecOqDepth      = RecFqDepth < DMX_OQ_DEPTH ? (RecFqDepth - 1) : DMX_OQ_DEPTH;
    RecOqBufSize    = RecOqDepth * DMX_OQ_DESC_SIZE;

    snprintf(BufName, sizeof(BufName), "DMX_RecDscBuf[%u]", RecInfo->RecFqId);
    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(BufName, MMZ_OTHERS, RecFqBufSize + RecOqBufSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("rec memory allocate failed, BufSize=0x%x\n", RecFqBufSize + RecOqBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out2;
    }

    RecFqInfo->FqDescSetPhy     = MmzBuf.u32StartPhyAddr;
    RecFqInfo->FqDescSetBase     = (FQ_DescInfo_S*)MmzBuf.pu8StartVirAddr;
    RecFqInfo->FQDepth       = RecFqDepth;

    RecOqInfo->OqDescSetPhy     = RecFqInfo->FqDescSetPhy + RecFqBufSize;
    RecOqInfo->OqDescSetBase     = (OQ_DescInfo_S*)((HI_U8*)RecFqInfo->FqDescSetBase + RecFqBufSize);
    RecOqInfo->OQDepth       = RecOqDepth;

    return HI_SUCCESS;
out2:
    DmxTeecUnregisterRecSecBuf(RecInfo, &SecMmuBuf);
out1:
    HI_DRV_SECSMMU_Release(&SecMmuBuf);
out0:
    return ret;
}

static HI_VOID DmxFreeSecureRecBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
    DMX_FQ_Info_S  *RecFqInfo = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    MMZ_BUFFER_S MmzBuf = {0};
    SMMU_BUFFER_S SecMmuBuf = {0};

    MmzBuf.pu8StartVirAddr  = (HI_U8*)RecFqInfo->FqDescSetBase;
    MmzBuf.u32StartPhyAddr  = RecFqInfo->FqDescSetPhy;
    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);

    SecMmuBuf.pu8StartVirAddr = HI_NULL;
    SecMmuBuf.u32StartSmmuAddr = RecFqInfo->BufPhyAddr;
    DmxTeecUnregisterRecSecBuf(RecInfo, &SecMmuBuf);
    HI_DRV_SECSMMU_Release(&SecMmuBuf);
}

static HI_S32 DmxAllocNonSecureRecBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
    HI_S32 ret = HI_FAILURE;
    DMX_FQ_Info_S  *RecFqInfo = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    DMX_OQ_Info_S  *RecOqInfo = &DmxSet->DmxOqInfo[RecInfo->RecOqId];
    HI_U32 RecFqBufSize = 0, RecFqBlockSize = 0, RecFqDepth = 0;
    HI_U32 RecOqBufSize = 0, RecOqDepth = 0;
    SMMU_BUFFER_S MmuBuf = {0};
    MMZ_BUFFER_S MmzBuf = {0};
    HI_CHAR BufName[32];

    switch(RecInfo->IndexType)
    {
        case HI_UNF_DMX_REC_INDEX_TYPE_VIDEO :
            RecFqBlockSize = RecInfo->enRecTimeStamp >= DMX_REC_TIMESTAMP_ZERO ? DMX_REC_VID_TS_WITH_TIMESTAMP_BLOCK_SIZE : DMX_REC_VID_TS_PACKETS_PER_BLOCK;
            break;
        case HI_UNF_DMX_REC_INDEX_TYPE_AUDIO :
            RecFqBlockSize = RecInfo->enRecTimeStamp >= DMX_REC_TIMESTAMP_ZERO ? DMX_REC_AUD_TS_WITH_TIMESTAMP_BLOCK_SIZE : DMX_REC_AUD_TS_PACKETS_PER_BLOCK;
            break;
        case HI_UNF_DMX_REC_INDEX_TYPE_NONE :
            RecFqBlockSize = DMX_REC_VID_TS_PACKETS_PER_BLOCK;
            break;
        default:
            BUG();
    }

    /* alloc rec data buffer. */
    snprintf(BufName, sizeof(BufName), "DMX_RecBuf[%u]", RecInfo->RecFqId);
    if (HI_SUCCESS != HI_DRV_SMMU_AllocAndMap(BufName, RecInfo->RecBufSize , 0, &MmuBuf))
    {
        HI_ERR_DEMUX("mmu memory allocate failed, BufSize=0x%x\n", RecInfo->RecBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out0;
    }

    BUG_ON(MmuBuf.u32Size != RecInfo->RecBufSize);

    RecFqInfo->BufVirAddr   = MmuBuf.pu8StartVirAddr;
    RecFqInfo->BufPhyAddr   = MmuBuf.u32StartSmmuAddr;
    RecFqInfo->BufSize      = MmuBuf.u32Size;
    RecFqInfo->BufFlag      = DMX_MMU_BUF;
    RecFqInfo->BlockSize    = RecFqBlockSize;

    /* alloc dsc buffer. */
    RecFqDepth      = RecFqInfo->BufSize / RecFqInfo->BlockSize + 1;
    RecFqBufSize    = (RecFqDepth * DMX_FQ_DESC_SIZE + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;
    RecOqDepth      = RecFqDepth < DMX_OQ_DEPTH ? (RecFqDepth - 1) : DMX_OQ_DEPTH;
    RecOqBufSize    = RecOqDepth * DMX_OQ_DESC_SIZE;

    snprintf(BufName, sizeof(BufName), "DMX_RecDscBuf[%u]", RecInfo->RecFqId);
    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(BufName, MMZ_OTHERS, RecFqBufSize + RecOqBufSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("rec memory allocate failed, BufSize=0x%x\n", RecFqBufSize + RecOqBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out1;
    }

    RecFqInfo->FqDescSetPhy     = MmzBuf.u32StartPhyAddr;
    RecFqInfo->FqDescSetBase     = (FQ_DescInfo_S*)MmzBuf.pu8StartVirAddr;
    RecFqInfo->FQDepth       = RecFqDepth;

    RecOqInfo->OqDescSetPhy     = RecFqInfo->FqDescSetPhy + RecFqBufSize;
    RecOqInfo->OqDescSetBase     = (OQ_DescInfo_S*)((HI_U8*)RecFqInfo->FqDescSetBase + RecFqBufSize);
    RecOqInfo->OQDepth       = RecOqDepth;

    return HI_SUCCESS;

out1:
    MmuBuf.pu8StartVirAddr = RecFqInfo->BufVirAddr;
    MmuBuf.u32StartSmmuAddr = RecFqInfo->BufPhyAddr;

    HI_DRV_SMMU_UnmapAndRelease(&MmuBuf);
out0:
    return ret;
}

static HI_VOID DmxFreeNonSecureRecBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
    DMX_FQ_Info_S  *RecFqInfo = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    SMMU_BUFFER_S MmuBuf = {0};
    MMZ_BUFFER_S MmzBuf = {0};

    MmzBuf.pu8StartVirAddr  = (HI_U8*)RecFqInfo->FqDescSetBase;
    MmzBuf.u32StartPhyAddr  = RecFqInfo->FqDescSetPhy;

    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);

    MmuBuf.pu8StartVirAddr  = RecFqInfo->BufVirAddr;
    MmuBuf.u32StartSmmuAddr  = RecFqInfo->BufPhyAddr;

    HI_DRV_SMMU_UnmapAndRelease(&MmuBuf);
}

static HI_S32 DmxAllocScdBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
    HI_S32 ret = HI_FAILURE;
    DMX_FQ_Info_S  *ScdFqInfo = &DmxSet->DmxFqInfo[RecInfo->ScdFqId];
    DMX_OQ_Info_S  *ScdOqInfo = &DmxSet->DmxOqInfo[RecInfo->ScdOqId];
    HI_U32 ScdFqBufSize = 0, ScdFqDepth = 0;
    HI_U32 ScdOqBufSize = 0, ScdOqDepth = 0;
    HI_U32 ScdBufSize = 0, ScdFqBlockSize = 0;
    SMMU_BUFFER_S MmuBuf = {0};
    MMZ_BUFFER_S MmzBuf = {0};
    HI_CHAR BufName[32];

    switch(RecInfo->IndexType)
    {
        case HI_UNF_DMX_REC_INDEX_TYPE_VIDEO :
            ScdBufSize = (DMX_REC_VID_SCD_BUF_SIZE + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;
            ScdFqBlockSize = DMX_REC_VID_SCD_NUM_PER_BLOCK;
            break;
        case HI_UNF_DMX_REC_INDEX_TYPE_AUDIO :
            ScdBufSize = (DMX_REC_AUD_SCD_BUF_SIZE + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;
            ScdFqBlockSize = DMX_REC_AUD_SCD_NUM_PER_BLOCK;
            break;
        case HI_UNF_DMX_REC_INDEX_TYPE_NONE :
            ScdBufSize = 0;
            break;
        default:
            BUG();
    }

    /* alloc scd data buffer. */
    snprintf(BufName, sizeof(BufName), "DMX_ScdBuf[%u]", RecInfo->ScdId);
    if (HI_SUCCESS != HI_DRV_SMMU_AllocAndMap(BufName, ScdBufSize , 0, &MmuBuf))
    {
        HI_ERR_DEMUX("mmu memory allocate failed, BufSize=0x%x\n", ScdBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out0;
    }

    BUG_ON(MmuBuf.u32Size != ScdBufSize);

    ScdFqInfo->BufPhyAddr    = MmuBuf.u32StartSmmuAddr;
    ScdFqInfo->BufVirAddr    = MmuBuf.pu8StartVirAddr;
    ScdFqInfo->BufSize       = MmuBuf.u32Size;
    ScdFqInfo->BufFlag       = DMX_MMU_BUF;
    ScdFqInfo->BlockSize     = ScdFqBlockSize;

    /* alloc dsc buffer. */
    ScdFqDepth      = ScdFqInfo->BufSize / ScdFqInfo->BlockSize + 1;
    ScdFqBufSize    = (ScdFqDepth * DMX_FQ_DESC_SIZE + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;
    ScdOqDepth      = ScdFqDepth < DMX_OQ_DEPTH ? (ScdFqDepth - 1) : DMX_OQ_DEPTH;
    ScdOqBufSize    = ScdOqDepth * DMX_OQ_DESC_SIZE;

    snprintf(BufName, sizeof(BufName), "DMX_ScdDscBuf[%u]", RecInfo->ScdId);
    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(BufName, MMZ_OTHERS, ScdFqBufSize + ScdOqBufSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("scd memory allocate failed, BufSize=0x%x\n", ScdFqBufSize + ScdOqBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out1;
    }

    ScdFqInfo->FqDescSetPhy     = MmzBuf.u32StartPhyAddr;
    ScdFqInfo->FqDescSetBase    = (FQ_DescInfo_S*)MmzBuf.pu8StartVirAddr;
    ScdFqInfo->FQDepth          = ScdFqDepth;

    ScdOqInfo->OqDescSetPhy     = ScdFqInfo->FqDescSetPhy + ScdFqBufSize;
    ScdOqInfo->OqDescSetBase    = (OQ_DescInfo_S*)((HI_U8*)ScdFqInfo->FqDescSetBase + ScdFqBufSize);
    ScdOqInfo->OQDepth          = ScdOqDepth;

    return HI_SUCCESS;

out1:
    MmuBuf.pu8StartVirAddr = ScdFqInfo->BufVirAddr;
    MmuBuf.u32StartSmmuAddr = ScdFqInfo->BufPhyAddr;
    HI_DRV_SMMU_UnmapAndRelease(&MmuBuf);
out0:
    return ret;
}

static HI_VOID DmxFreeScdBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
    DMX_FQ_Info_S  *ScdFqInfo = &DmxSet->DmxFqInfo[RecInfo->ScdFqId];
    SMMU_BUFFER_S MmuBuf = {0};
    MMZ_BUFFER_S MmzBuf = {0};

    BUG_ON(HI_UNF_DMX_REC_INDEX_TYPE_NONE == RecInfo->IndexType);

    MmzBuf.pu8StartVirAddr  = (HI_U8*)ScdFqInfo->FqDescSetBase;
    MmzBuf.u32StartPhyAddr  = ScdFqInfo->FqDescSetPhy;
    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);

    MmuBuf.pu8StartVirAddr  = ScdFqInfo->BufVirAddr;
    MmuBuf.u32StartSmmuAddr  = ScdFqInfo->BufPhyAddr;
    HI_DRV_SMMU_UnmapAndRelease(&MmuBuf);
}

#else

static HI_S32 DmxAllocSecureRecBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
    HI_ERR_DEMUX("not support alloc secure mmz rec buf.\n");
    return HI_ERR_DMX_NOT_SUPPORT;
}

static HI_VOID DmxFreeSecureRecBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
}

static HI_S32 DmxAllocNonSecureRecBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
    HI_S32 ret = HI_FAILURE;
    DMX_FQ_Info_S  *RecFqInfo = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    DMX_OQ_Info_S  *RecOqInfo = &DmxSet->DmxOqInfo[RecInfo->RecOqId];
    HI_U32 RecFqBufSize = 0, RecFqBlockSize = 0, RecFqDepth = 0;
    HI_U32 RecOqBufSize = 0, RecOqDepth = 0;
    MMZ_BUFFER_S MmzBuf = {0};
    HI_CHAR BufName[32];

    switch(RecInfo->IndexType)
    {
        case HI_UNF_DMX_REC_INDEX_TYPE_VIDEO :
            RecFqBlockSize = RecInfo->enRecTimeStamp >= DMX_REC_TIMESTAMP_ZERO ? DMX_REC_VID_TS_WITH_TIMESTAMP_BLOCK_SIZE : DMX_REC_VID_TS_PACKETS_PER_BLOCK;
            break;
        case HI_UNF_DMX_REC_INDEX_TYPE_AUDIO :
            RecFqBlockSize = RecInfo->enRecTimeStamp >= DMX_REC_TIMESTAMP_ZERO ? DMX_REC_AUD_TS_WITH_TIMESTAMP_BLOCK_SIZE : DMX_REC_AUD_TS_PACKETS_PER_BLOCK;
            break;
        case HI_UNF_DMX_REC_INDEX_TYPE_NONE :
            RecFqBlockSize = DMX_REC_VID_TS_PACKETS_PER_BLOCK;
            break;
        default:
            BUG();
    }

    RecFqDepth      = RecInfo->RecBufSize / RecFqBlockSize + 1;
    RecFqBufSize    = (RecFqDepth * DMX_FQ_DESC_SIZE + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;
    RecOqDepth      = RecFqDepth < DMX_OQ_DEPTH ? (RecFqDepth - 1) : DMX_OQ_DEPTH;
    RecOqBufSize    = RecOqDepth * DMX_OQ_DESC_SIZE;

    snprintf(BufName, sizeof(BufName), "DMX_RecBuf[%u]", RecInfo->RecFqId);

    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(BufName, MMZ_OTHERS, RecInfo->RecBufSize + RecFqBufSize + RecOqBufSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("rec memory allocate failed, BufSize=0x%x\n", RecInfo->RecBufSize + RecFqBufSize + RecOqBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out;
    }

    RecFqInfo->BufPhyAddr    = MmzBuf.u32StartPhyAddr;
    RecFqInfo->BufVirAddr    = MmzBuf.pu8StartVirAddr;
    RecFqInfo->BufSize       = RecInfo->RecBufSize;
    RecFqInfo->BufFlag       = DMX_MMZ_BUF;
    RecFqInfo->BlockSize     = RecFqBlockSize;

    RecFqInfo->FqDescSetPhy  = RecFqInfo->BufPhyAddr + RecFqInfo->BufSize ;
    RecFqInfo->FqDescSetBase = (FQ_DescInfo_S*)(RecFqInfo->BufVirAddr + RecFqInfo->BufSize);
    RecFqInfo->FQDepth       = RecFqDepth;

    RecOqInfo->OqDescSetPhy  = RecFqInfo->FqDescSetPhy + RecFqBufSize;
    RecOqInfo->OqDescSetBase = (OQ_DescInfo_S*)((HI_U8*)RecFqInfo->FqDescSetBase + RecFqBufSize);
    RecOqInfo->OQDepth       = RecOqDepth;

    return HI_SUCCESS;

out:
    return ret;
}

static HI_VOID DmxFreeNonSecureRecBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
    DMX_FQ_Info_S  *RecFqInfo = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    MMZ_BUFFER_S MmzBuf = {0};

    MmzBuf.pu8StartVirAddr  = RecFqInfo->BufVirAddr;
    MmzBuf.u32StartPhyAddr  = RecFqInfo->BufPhyAddr;

    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);
}

static HI_S32 DmxAllocScdBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
    HI_S32 ret = HI_FAILURE;
    DMX_FQ_Info_S  *ScdFqInfo = &DmxSet->DmxFqInfo[RecInfo->ScdFqId];
    DMX_OQ_Info_S  *ScdOqInfo = &DmxSet->DmxOqInfo[RecInfo->ScdOqId];
    HI_U32 ScdFqBufSize = 0, ScdFqDepth = 0;
    HI_U32 ScdOqBufSize = 0, ScdOqDepth = 0;
    HI_U32 ScdBufSize = 0, ScdFqBlockSize = 0;
    MMZ_BUFFER_S MmzBuf = {0};
    HI_CHAR BufName[32];

    switch(RecInfo->IndexType)
    {
        case HI_UNF_DMX_REC_INDEX_TYPE_VIDEO :
            ScdBufSize = (DMX_REC_VID_SCD_BUF_SIZE + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;
            ScdFqBlockSize = DMX_REC_VID_SCD_NUM_PER_BLOCK;
            break;
        case HI_UNF_DMX_REC_INDEX_TYPE_AUDIO :
            ScdBufSize = (DMX_REC_AUD_SCD_BUF_SIZE + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;
            ScdFqBlockSize = DMX_REC_AUD_SCD_NUM_PER_BLOCK;
            break;
        case HI_UNF_DMX_REC_INDEX_TYPE_NONE :
            ScdBufSize = 0;
            ScdFqBlockSize = 0;
            break;
        default:
            BUG();
    }

    ScdFqDepth      = ScdBufSize / ScdFqBlockSize + 1;
    ScdFqBufSize    = (ScdFqDepth * DMX_FQ_DESC_SIZE + MIN_MMZ_BUF_SIZE - 1) / MIN_MMZ_BUF_SIZE * MIN_MMZ_BUF_SIZE;

    ScdOqDepth      = ScdFqDepth < DMX_OQ_DEPTH ? (ScdFqDepth - 1) : DMX_OQ_DEPTH;
    ScdOqBufSize    = ScdOqDepth * DMX_OQ_DESC_SIZE;

    snprintf(BufName, sizeof(BufName), "DMX_ScdBuf[%u]", RecInfo->ScdId);

    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(BufName, MMZ_OTHERS, ScdBufSize + ScdFqBufSize + ScdOqBufSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("scd memory allocate failed, BufSize=0x%x\n", ScdBufSize + ScdFqBufSize + ScdOqBufSize);

        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out;
    }

    ScdFqInfo->BufPhyAddr    = MmzBuf.u32StartPhyAddr;
    ScdFqInfo->BufVirAddr    = MmzBuf.pu8StartVirAddr;
    ScdFqInfo->BufSize       = ScdBufSize;
    ScdFqInfo->BufFlag       = DMX_MMZ_BUF;
    ScdFqInfo->BlockSize     = ScdFqBlockSize;

    ScdFqInfo->FqDescSetPhy  = ScdFqInfo->BufPhyAddr + ScdFqInfo->BufSize;
    ScdFqInfo->FqDescSetBase = (FQ_DescInfo_S*)(ScdFqInfo->BufVirAddr + ScdFqInfo->BufSize);
    ScdFqInfo->FQDepth       = ScdFqDepth;

    ScdOqInfo->OqDescSetPhy  = ScdFqInfo->FqDescSetPhy + ScdFqBufSize;
    ScdOqInfo->OqDescSetBase = (OQ_DescInfo_S*)((HI_U8*)ScdFqInfo->FqDescSetBase + ScdFqBufSize);
    ScdOqInfo->OQDepth       = ScdOqDepth;

    return HI_SUCCESS;

out:
    return ret;
}

static HI_VOID DmxFreeScdBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
    DMX_FQ_Info_S  *ScdFqInfo = &DmxSet->DmxFqInfo[RecInfo->ScdFqId];
    MMZ_BUFFER_S MmzBuf = {0};

    BUG_ON(HI_UNF_DMX_REC_INDEX_TYPE_NONE == RecInfo->IndexType);

    MmzBuf.pu8StartVirAddr  = ScdFqInfo->BufVirAddr;
    MmzBuf.u32StartPhyAddr  = ScdFqInfo->BufPhyAddr;
    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);
}

#endif

static HI_S32 DmxAllocRecBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
    HI_S32 ret = HI_FAILURE;

    switch(RecInfo->SecureMode)
    {
        case HI_UNF_DMX_SECURE_MODE_NONE:
            ret = DmxAllocNonSecureRecBuf(DmxSet, RecInfo);
            break;
        case HI_UNF_DMX_SECURE_MODE_TEE:
            ret = DmxAllocSecureRecBuf(DmxSet, RecInfo);
            break;
        default:
            BUG();
    }

    return ret;
}

static HI_VOID DmxFreeRecBuf(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo)
{
    switch(RecInfo->SecureMode)
    {
        case HI_UNF_DMX_SECURE_MODE_NONE:
            DmxFreeNonSecureRecBuf(DmxSet, RecInfo);
            break;
        case HI_UNF_DMX_SECURE_MODE_TEE:
            DmxFreeSecureRecBuf(DmxSet, RecInfo);
            break;
        default:
            BUG();
    }
}

static HI_S32 DmxCreateRec(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_UNF_DMX_REC_ATTR_S *RecAttr)
{
    HI_S32 ret = HI_FAILURE;
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];
    HI_U32 RecFqId = DMX_INVALID_FQ_ID, RecOqId = DMX_INVALID_OQ_ID;
    HI_U32 ScdId = DMX_INVALID_SCD_ID, ScdFqId = DMX_INVALID_FQ_ID, ScdOqId = DMX_INVALID_OQ_ID;
    DMX_OQ_Info_S  *RecOqInfo = HI_NULL, *ScdOqInfo = HI_NULL;
    HI_S32 PicParser = -1;

    BUG_ON(DmxSet != GetDmxSetByDmxid(RecAttr->u32DmxId));

    RecInfo->DmxId          = RecAttr->u32DmxId;
    RecInfo->RecId          = RecId;
    RecInfo->RecType        = RecAttr->enRecType;
    RecInfo->Descramed      = RecAttr->bDescramed;
    RecInfo->IndexType      = RecAttr->enIndexType;
    RecInfo->VCodecType     = RecAttr->enVCodecType;
    RecInfo->IndexPid       = RecAttr->u32IndexSrcPid;
    RecInfo->RecBufSize     = RecAttr->u32RecBufSize;
    RecInfo->SecureMode     = RecAttr->enSecureMode;
    RecInfo->RecStatus      = DMX_REC_STATUS_STOP;
    RecInfo->RecFqId        = DMX_INVALID_FQ_ID;
    RecInfo->RecOqId        = DMX_INVALID_OQ_ID;
    RecInfo->ScdFqId        = DMX_INVALID_FQ_ID;
    RecInfo->ScdOqId        = DMX_INVALID_OQ_ID;
    RecInfo->ScdId          = DMX_INVALID_SCD_ID;
    RecInfo->Owner          = task_tgid_nr(current);
    RecInfo->PicParser      = -1;
    RecInfo->enRecTimeStamp = DMX_REC_TIMESTAMP_NONE;
    bitmap_zero(RecInfo->RecChnBitmap, DmxSet->DmxChanCnt);
    memset(&RecInfo->LastFrameInfo, 0, sizeof(HI_MPI_DMX_REC_INDEX_S));

    ret = DmxFqAcquire(DmxSet, &RecFqId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = DmxOqAcquire(DmxSet, RecInfo->DmxId, RecInfo->SecureMode, &RecOqId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    RecOqInfo = &DmxSet->DmxOqInfo[RecOqId];
    RecOqInfo->enOQBufMode    = DMX_OQ_MODE_REC;
    RecOqInfo->u32FQId        = RecFqId;

    RecInfo->RecFqId        = RecFqId;
    RecInfo->RecOqId        = RecOqId;

    ret = DmxAllocRecBuf(DmxSet, RecInfo);
    if (HI_SUCCESS != ret)
    {
        goto out2;
    }

    if (HI_UNF_DMX_REC_INDEX_TYPE_NONE != RecInfo->IndexType)
    {
        if (HI_UNF_DMX_REC_INDEX_TYPE_VIDEO == RecInfo->IndexType)
        {
            VIDSTD_E VidStd = VIDSTD_BUTT;

            switch (RecInfo->VCodecType)
            {
                case HI_UNF_VCODEC_TYPE_MPEG2 :
                    VidStd = VIDSTD_MPEG2;
                    break;
                case HI_UNF_VCODEC_TYPE_MPEG4 :
                    VidStd = VIDSTD_MPEG4;
                    break;
                case HI_UNF_VCODEC_TYPE_AVS :
                    VidStd = VIDSTD_AVS;
                    break;
                case HI_UNF_VCODEC_TYPE_H264 :
                    VidStd = VIDSTD_H264;
                    break;
                case HI_UNF_VCODEC_TYPE_HEVC:
                    VidStd = VIDSTD_HEVC;
                    break;
                default:
                    BUG();
            }

            PicParser = FIDX_OpenInstance(VidStd, STRM_TYPE_ES, (HI_U32*)&RecInfo->LastFrameInfo);
            if (PicParser < 0)
            {
                HI_ERR_DEMUX("PicParser is invlid\n");
                ret = HI_ERR_DMX_NOFREE_CHAN;
                goto out3;
            }
        }

        ret = DmxFqAcquire(DmxSet, &ScdFqId);
        if (HI_SUCCESS != ret)
        {
            goto out4;
        }

        ret = DmxOqAcquire(DmxSet, RecInfo->DmxId, HI_UNF_DMX_SECURE_MODE_NONE, &ScdOqId);
        if (HI_SUCCESS != ret)
        {
            goto out5;
        }

        ScdOqInfo = &DmxSet->DmxOqInfo[ScdOqId];
        ScdOqInfo->enOQBufMode    = DMX_OQ_MODE_SCD;
        ScdOqInfo->u32FQId            = ScdFqId;

        ret = DmxScdAcquire(DmxSet, RecInfo->DmxId, &ScdId);
        if (HI_SUCCESS != ret)
        {
            goto out6;
        }

        RecInfo->ScdFqId        = ScdFqId;
        RecInfo->ScdOqId        = ScdOqId;
        RecInfo->ScdId          = ScdId;
        RecInfo->PicParser      = PicParser;

        ret = DmxAllocScdBuf(DmxSet, RecInfo);
        if (HI_SUCCESS != ret)
        {
            goto out7;
        }
    }

    return HI_SUCCESS;

out7:
    DmxScdRelease(DmxSet, ScdId);
out6:
    DmxOqRelease(DmxSet, RecInfo->DmxId, ScdOqId);
out5:
    DmxFqRelease(DmxSet, ScdFqId);
out4:
    if (PicParser >= 0)
    {
        FIDX_CloseInstance(PicParser);
    }
out3:
    DmxFreeRecBuf(DmxSet, RecInfo);
out2:
    DmxOqRelease(DmxSet, RecInfo->DmxId, RecOqId);
out1:
    DmxFqRelease(DmxSet, RecFqId);
out0:
    RecInfo->DmxId = DMX_INVALID_DEMUX_ID;
    return ret;
}

static HI_S32 DmxDestroyRec(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];

    BUG_ON(DmxSet != GetDmxSetByDmxid(RecInfo->DmxId));

    if (HI_UNF_DMX_REC_INDEX_TYPE_NONE != RecInfo->IndexType)
    {
        DmxFreeScdBuf(DmxSet, RecInfo);

        DmxScdRelease(DmxSet, RecInfo->ScdId);
        DmxOqRelease(DmxSet, RecInfo->DmxId, RecInfo->ScdOqId);
        DmxFqRelease(DmxSet, RecInfo->ScdFqId);

        if (HI_UNF_DMX_REC_INDEX_TYPE_VIDEO == RecInfo->IndexType)
        {
            FIDX_CloseInstance(RecInfo->PicParser);
        }
    }

    DmxFreeRecBuf(DmxSet, RecInfo);

    DmxOqRelease(DmxSet, RecInfo->DmxId, RecInfo->RecOqId);
    DmxFqRelease(DmxSet, RecInfo->RecFqId);

    RecInfo->DmxId = DMX_INVALID_DEMUX_ID;
    RecInfo->RecId = DMX_INVALID_REC_ID;

    return HI_SUCCESS;
}

HI_S32 DMX_DRV_REC_CreateChannel(HI_UNF_DMX_REC_ATTR_S *RecAttr, DMX_REC_TIMESTAMP_MODE_E enRecTimeStamp,HI_U32 *RecId, DMX_MMZ_BUF_S *RecBuf)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;
    HI_UNF_DMX_REC_ATTR_S ValidRecAttr = {0};

    DmxSet = GetDmxSetByDmxid(RecAttr->u32DmxId);
    if (!DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    ret = DmxTrimRecAttrs(RecAttr, &ValidRecAttr, enRecTimeStamp);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    mutex_lock(&DmxSet->LockAllRec);

    ret = DmxRecAcquire(&ValidRecAttr, enRecTimeStamp, RecId, RecBuf);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

out1:
    mutex_unlock(&DmxSet->LockAllRec);
out0:
    return ret;
}

HI_S32 DMX_DRV_REC_DestroyChannel(HI_U32 DmxId, HI_U32 RecId)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_RecInfo_S  *RecInfo     = HI_NULL;

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    RecInfo = &DmxSet->DmxRecInfo[RecId];

    if (DMX_REC_STATUS_STOP != RecInfo->RecStatus)
    {
        HI_ERR_DEMUX("stop and del the rec channel first!\n");
        ret = HI_ERR_DMX_STARTING_REC_CHAN;
        goto out1;
    }

    ret = DmxRecRelease(DmxId, RecId);

out1:
    DmxPutRecInstance(DmxSet, RecId);
out0:
    return ret;
}

/*
 * DMX_SCD_VERSION_2 don't support multi rec same pid channel of same demux in different process.
 * DMX_DUP_PID_CHANNEL_SUPPORT support it but which has bandwidth issues.
 * we choose DMX_SCD_VERSION_2 prefer.
 */

#if defined(DMX_SCD_VERSION_2)
static HI_S32 DmxAddRecSharedPid(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 Pid, HI_U32 *ChanId)
{
    HI_S32 ret = HI_FAILURE;
    DMX_RecInfo_S *RecInfo = &DmxSet->DmxRecInfo[RecId];
    DMX_ChanInfo_S *ChanInfo = HI_NULL;
    HI_BOOL ShareChnIndicator = HI_FALSE;
    HI_U32 index;

    /* look for the pid channel whether already exist */
    for_each_set_bit(index, DmxSet->ChnBitmap, DmxSet->DmxChanCnt)
    {
        ChanInfo = &DmxSet->DmxChanInfo[index];

        if ((HI_UNF_DMX_CHAN_OUTPUT_MODE_REC & ChanInfo->ChanOutMode) &&
                (ChanInfo->DmxId == RecInfo->DmxId) && (Pid == ChanInfo->ChanPid))
        {
            HI_DBG_DEMUX("The Rec PID(0x%x) channel of demux(%d) has existed.\n", Pid, RecInfo->DmxId);

            *ChanId = ChanInfo->ChanId;

            ShareChnIndicator = HI_TRUE;
            break;
        }
    }

    if (HI_TRUE == ShareChnIndicator)
    {
        unsigned long mask = BIT_MASK(*ChanId);
        unsigned long *p = ((unsigned long*)RecInfo->RecChnBitmap) + BIT_WORD(*ChanId);
        ChanInfo = &DmxSet->DmxChanInfo[*ChanId];

        if (*p & mask)
        {
            HI_ERR_DEMUX("Not allow add same pid channel(%d) into same rec instance of same demux.\n", Pid);

            ret = HI_ERR_DMX_INVALID_PARA;
        }
        else if (ChanInfo->Owner != task_tgid_nr(current))
        {
            HI_ERR_DEMUX("Not allow add same pid channel(%d) into different rec instance of same demux at diff process.\n", Pid);

            ret = HI_ERR_DMX_INVALID_PARA;
        }
        else /* allow add same pid channel into different rec instance of same demux at same process */
        {
            atomic_inc(&ChanInfo->ChanRecShareCnt);

            set_bit(*ChanId, RecInfo->RecChnBitmap);

            /* if the channel change to share channel , which will not reset until it destroyed. */
            DmxHalSetShareRecChannel(DmxSet, *ChanId, HI_TRUE);

            ret = HI_SUCCESS;
        }
    }
    else
    {
        ret = HI_FAILURE;
    }

    return ret;
}
#else
static HI_S32 DmxAddRecSharedPid(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 Pid, HI_U32 *ChanId)
{
    return HI_FAILURE;
}
#endif

/*
 * if same pid play channel existed, we can attach rec func to this play channel.
 */
static HI_S32 DmxAttachRecPid(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 Pid, HI_U32 *ChanId)
{
    HI_S32 ret = HI_FAILURE;
    DMX_RecInfo_S *RecInfo = &DmxSet->DmxRecInfo[RecId];
    DMX_ChanInfo_S *ChanInfo = HI_NULL;
    HI_UNF_DMX_CHAN_ATTR_S  ChanAttr;
    HI_U32 ExistChanId;

    ChanAttr.u32BufSize    = 0;
    ChanAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_POST;
    ChanAttr.enCRCMode     = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
    ChanAttr.enOutputMode  = HI_UNF_DMX_CHAN_OUTPUT_MODE_REC;
    ChanAttr.enSecureMode = RecInfo->SecureMode;

    ret = DMX_OsiGetChannelId(RecInfo->DmxId, Pid, &ExistChanId);
    if (HI_SUCCESS != ret)
    {
        ret = HI_ERR_DMX_UNMATCH_CHAN;
        goto out0;
    }

    ret = DmxGetChnInstance(DmxSet, ExistChanId);
    if (HI_SUCCESS != ret)
    {
        ret = HI_ERR_DMX_UNMATCH_CHAN;
        goto out0;
    }

    ret = DmxAddChannelFunc(DmxSet, ExistChanId, Pid, &ChanAttr, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    ChanInfo = &DmxSet->DmxChanInfo[ExistChanId];
    ChanInfo->ChanRecOqId = RecInfo->RecOqId;

    ret = DmxOpenRecChannel(DmxSet, ExistChanId);
    if (HI_SUCCESS != ret)
    {
        ChanInfo->ChanRecOqId = DMX_INVALID_OQ_ID;
        goto out2;
    }

    *ChanId = ExistChanId;

    set_bit(*ChanId, RecInfo->RecChnBitmap);

    DmxPutChnInstance(DmxSet, ExistChanId);

    return HI_SUCCESS;

out2:
    DmxDelChannelFunc(DmxSet, ExistChanId, HI_UNF_DMX_CHAN_OUTPUT_MODE_REC);
out1:
    DmxPutChnInstance(DmxSet, ExistChanId);
out0:
    return ret;
}

static HI_S32 DmxNewRecPid(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 Pid, HI_U32 *ChanId)
{
    HI_S32 ret = HI_FAILURE;
    DMX_RecInfo_S *RecInfo = &DmxSet->DmxRecInfo[RecId];
    DMX_ChanInfo_S *ChanInfo = HI_NULL;
    HI_UNF_DMX_CHAN_ATTR_S  ChanAttr;

    ChanAttr.u32BufSize    = 0;
    ChanAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_POST;
    ChanAttr.enCRCMode     = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
    ChanAttr.enOutputMode  = HI_UNF_DMX_CHAN_OUTPUT_MODE_REC;
    ChanAttr.enSecureMode = RecInfo->SecureMode;

    ret = DMX_OsiCreateChannel(RecInfo->DmxId, &ChanAttr, HI_NULL, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = DMX_OsiSetChannelPid(RecInfo->DmxId, *ChanId, Pid);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    ChanInfo = &DmxSet->DmxChanInfo[*ChanId];
    ChanInfo->ChanRecOqId = RecInfo->RecOqId;

    ret = DMX_OsiOpenRecChannel(RecInfo->DmxId, *ChanId);
    if (HI_SUCCESS != ret)
    {
        ChanInfo->ChanRecOqId = DMX_INVALID_OQ_ID;
        goto out1;
    }

    set_bit(*ChanId, RecInfo->RecChnBitmap);

    return HI_SUCCESS;

out1:
    DMX_OsiDestroyRecChannel(RecInfo->DmxId, *ChanId);
out0:
    return ret;
}

static HI_S32 DmxAddRecPid(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 Pid, HI_U32 *ChanId)
{
    HI_S32 ret = HI_FAILURE;

    ret = DmxAttachRecPid(DmxSet, RecId, Pid, ChanId);
    if (HI_ERR_DMX_UNMATCH_CHAN == ret)
    {
        ret = DmxNewRecPid(DmxSet, RecId, Pid, ChanId);
    }

    return ret;
}

/*
 * This lock protect that multi thread or process judge shared pid channel.
 */
HI_S32 DMX_DRV_REC_AddRecPid(HI_U32 DmxId, HI_U32 RecId, HI_U32 Pid, HI_U32 *ChanId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    if (HI_NULL == DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        HI_ERR_DEMUX("Invalid DmxSet:0x%x\n", DmxSet);
        goto out0;
    }

    if (Pid > DMX_INVALID_PID)
    {
        HI_ERR_DEMUX("Invalid pid:0x%x\n", Pid);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    if (0 != mutex_lock_interruptible(&DmxSet->LockRecPid))
    {
        ret = HI_ERR_DMX_BUSY_REC;
        goto out0;
    }

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    ret = DmxAddRecSharedPid(DmxSet, RecId, Pid, ChanId);
    if (HI_FAILURE == ret)
    {
        ret = DmxAddRecPid(DmxSet, RecId, Pid, ChanId);
    }

    DmxPutRecInstance(DmxSet, RecId);
out1:
    mutex_unlock(&DmxSet->LockRecPid);
out0:
    return ret;
}

#if defined(DMX_SCD_VERSION_2)
static HI_S32 DmxDelRecSharedPid(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 ChanId)
{
    DMX_RecInfo_S *RecInfo = &DmxSet->DmxRecInfo[RecId];
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];
    HI_S32 ret = HI_FAILURE;

    ret = DmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    if (atomic_read(&ChanInfo->ChanRecShareCnt))
    {
        clear_bit(ChanId, RecInfo->RecChnBitmap);

        atomic_dec(&ChanInfo->ChanRecShareCnt);

        HI_DBG_DEMUX("Fake delete the shared Rec PID(0x%x) channel of demux(%d) .\n", ChanInfo->ChanPid, RecInfo->DmxId);

        ret = HI_SUCCESS;
    }
    else
    {
        ret = HI_FAILURE;
    }

    DmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}
#else
static HI_S32 DmxDelRecSharedPid(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 ChanId)
{
    return HI_FAILURE;
}
#endif

static HI_S32 DmxDelRecPid(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    DMX_RecInfo_S *RecInfo = &DmxSet->DmxRecInfo[RecId];

    ret = DMX_OsiDestroyRecChannel(RecInfo->DmxId, ChanId);
    if (HI_SUCCESS == ret)
    {
        clear_bit(ChanId, RecInfo->RecChnBitmap);
    }

    return ret;
}

/*
 * Held lock_Rec before call this function.
 */
static HI_S32 __DMX_DRV_REC_DelRecPid(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    DMX_RecInfo_S *RecInfo = &DmxSet->DmxRecInfo[RecId];
    unsigned long mask = BIT_MASK(ChanId);
    unsigned long *p = ((unsigned long*)RecInfo->RecChnBitmap) + BIT_WORD(ChanId);

    /* ensure this channel belong to this rec instance. */
    if (!(*p & mask))
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    ret = DmxDelRecSharedPid(DmxSet, RecId, ChanId);
    if (HI_FAILURE == ret)
    {
        ret = DmxDelRecPid(DmxSet, RecId, ChanId);
    }

out:
    return ret;
}

HI_S32 DMX_DRV_REC_DelRecPid(HI_U32 DmxId, HI_U32 RecId, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);

    if (HI_NULL == DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        HI_ERR_DEMUX("Invalid DmxSet:0x%x\n", DmxSet);
        goto out0;
    }

    if (0 != mutex_lock_interruptible(&DmxSet->LockRecPid))
    {
        ret = HI_ERR_DMX_BUSY_REC;
        goto out0;
    }

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    ret = __DMX_DRV_REC_DelRecPid(DmxSet, RecId, ChanId);

    DmxPutRecInstance(DmxSet, RecId);
out1:
    mutex_unlock(&DmxSet->LockRecPid);
out0:
    return ret;
}

HI_S32 DMX_DRV_REC_DelAllRecPid(HI_U32 DmxId, HI_U32 RecId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_RecInfo_S  *RecInfo = HI_NULL;
    HI_U32 ChnId;

    if (HI_NULL == DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        HI_ERR_DEMUX("Invalid DmxSet:0x%x\n", DmxSet);
        goto out0;
    }

    if (0 != mutex_lock_interruptible(&DmxSet->LockRecPid))
    {
        ret = HI_ERR_DMX_BUSY_REC;
        goto out0;
    }

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    RecInfo = &DmxSet->DmxRecInfo[RecId];

    if (HI_UNF_DMX_REC_TYPE_ALL_PID == RecInfo->RecType)
    {
        ret = HI_SUCCESS;
    }
    else if (HI_UNF_DMX_REC_TYPE_SELECT_PID == RecInfo->RecType)
    {
        HI_U32 Count = 0;

        for_each_set_bit(ChnId, RecInfo->RecChnBitmap, DmxSet->DmxChanCnt)
        {
            DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChnId];

            Count ++;

            ret = __DMX_DRV_REC_DelRecPid(DmxSet, RecId, ChanInfo->ChanId);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_DEMUX("Del rec channel(%d) failed.\n", ChnId);
                break;
            }
        }

        ret = (0 == Count) ? HI_SUCCESS : ret;
    }
    else
    {
        BUG();
    }

    DmxPutRecInstance(DmxSet, RecId);
out1:
    mutex_unlock(&DmxSet->LockRecPid);
out0:
    return ret;
}

HI_S32 DMX_DRV_REC_GetTsCnt(HI_U32 DmxId, HI_U32 RecId, HI_U32* TSCnt)
{
    HI_S32 ret = HI_SUCCESS;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_RecInfo_S  *RecInfo = HI_NULL;

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto err;
    }

    RecInfo = &DmxSet->DmxRecInfo[RecId];

    *TSCnt = DmxHalGetOqCounter(DmxSet, RecInfo->RecOqId);

    DmxPutRecInstance(DmxSet, RecId);
err:
    return ret;
}

#ifdef DMX_REC_EXCLUDE_PID_SUPPORT
HI_S32 DMX_DRV_REC_AddExcludeRecPid(HI_U32 DmxId, HI_U32 RecId, HI_U32 Pid)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    HI_U32 i;
    HI_U32 tmpRecDmxID,tmpPid;

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    if (Pid > DMX_INVALID_PID)
    {
        HI_ERR_DEMUX("Invalid pid 0x%x\n", Pid);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    for ( i = 0 ; i < DMX_REC_EXCLUDE_PID_NUM ; i++ )
    {
        DmxHalGetAllRecExcludePid(DmxSet, i, &tmpRecDmxID, &tmpPid);
        if ((tmpRecDmxID - 1) == DmxSet->Ops->GetDmxRawId(DmxSet, DmxId) && tmpPid == Pid)
        {
            ret = HI_SUCCESS;//already added the pid
            goto out1;
        }
    }

    for ( i = 0 ; i < DMX_REC_EXCLUDE_PID_NUM ; i++ )
    {
        DmxHalGetAllRecExcludePid(DmxSet, i, &tmpRecDmxID, &tmpPid);
        if (tmpRecDmxID == 0)
        {
            DmxHalSetAllRecExcludePid(DmxSet, i, DmxSet->Ops->GetDmxRawId(DmxSet, DmxId) + 1, Pid);
            ret = HI_SUCCESS;
            goto out1;
        }
    }

    ret = HI_ERR_DMX_NOAVAILABLE_EXCLUDEPID;
    HI_ERR_DEMUX("no available exclude pid resource: 0x%x\n", Pid);

out1:
    DmxPutRecInstance(DmxSet, RecId);
out0:
    return ret;
}

HI_S32 DMX_DRV_REC_DelExcludeRecPid(HI_U32 DmxId, HI_U32 RecId, HI_U32 Pid)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    HI_U32 i;
    HI_U32 tmpRecDmxID,tmpPid;

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    if (Pid > DMX_INVALID_PID)
    {
        HI_ERR_DEMUX("Invalid pid 0x%x\n", Pid);

        ret = HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    for ( i = 0 ; i < DMX_REC_EXCLUDE_PID_NUM ; i++ )
    {
        DmxHalGetAllRecExcludePid(DmxSet, i, &tmpRecDmxID, &tmpPid);
        if ((tmpRecDmxID - 1) == DmxSet->Ops->GetDmxRawId(DmxSet, DmxId) && tmpPid == Pid)
        {
            DmxHalSetAllRecExcludePid(DmxSet, i, 0, DMX_INVALID_PID);
            ret = HI_SUCCESS;
            goto out1;
        }
    }

out1:
    DmxPutRecInstance(DmxSet, RecId);
out0:
    return ret;
}

HI_S32 DMX_DRV_REC_DelAllExcludeRecPid(HI_U32 DmxId, HI_U32 RecId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    HI_U32 i;
    HI_U32 tmpRecDmxID,tmpPid;

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    for ( i = 0 ; i < DMX_REC_EXCLUDE_PID_NUM ; i++ )
    {
        DmxHalGetAllRecExcludePid(DmxSet, i, &tmpRecDmxID, &tmpPid);
        if ((tmpRecDmxID - 1) == DmxSet->Ops->GetDmxRawId(DmxSet, DmxId))
        {
            DmxHalSetAllRecExcludePid(DmxSet, i,0,DMX_INVALID_PID);
        }
    }

    ret = HI_SUCCESS;

    DmxPutRecInstance(DmxSet, RecId);
out:
    return ret;

}
#endif

HI_S32 DMX_DRV_REC_StartRecChn(HI_U32 DmxId, HI_U32 RecId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_RecInfo_S  *RecInfo = HI_NULL;
    DMX_FQ_Info_S  *FqInfo = HI_NULL;

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    RecInfo = &DmxSet->DmxRecInfo[RecId];
    FqInfo = &DmxSet->DmxFqInfo[RecInfo->RecFqId];

    if (unlikely(DMX_REC_STATUS_START == RecInfo->RecStatus))
    {
        ret = HI_SUCCESS;
        goto out1;
    }

    BUG_ON(!list_empty(&RecInfo->head));

    DmxHalSetRecTsCounter(DmxSet, RecId, RecInfo->RecOqId);
    DmxHalSetRecTsCntReplace(DmxSet, RecId);
    DmxHalSetSpsPauseType(DmxSet, RecInfo->DmxId, DMX_ENABLE);
    DmxSetRecType(DmxSet, RecId, RecInfo->RecType);

    DmxSetRecBuf(DmxSet, RecId, RecInfo->RecOqId);

    if (HI_UNF_DMX_REC_INDEX_TYPE_NONE != RecInfo->IndexType)
    {
        if(HI_SUCCESS != DmxStartSCD(DmxSet, RecId))
        {
            HI_WARN_DEMUX("No found SCD reference pid(%d) channel, but continue...\n", RecInfo->IndexPid);
        }
    }

    /* start record data */
    DmxFqStart(DmxSet, RecInfo->RecFqId);
    DmxOqStart(DmxSet, RecInfo->RecOqId);

    RecInfo->PrevScrClk   = 0;
    RecInfo->FirstFrameMs = 0;
    RecInfo->PrevFrameMs = 0;
    RecInfo->Cnt32To63Helper = 0;

    RecInfo->PrevFrameEndAddr = 0;
    RecInfo->BlockStartAddr = FqInfo->BufPhyAddr;
    RecInfo->RemIdxCnt = 0;

    memset(&RecInfo->LastFrameInfo, 0, sizeof(HI_MPI_DMX_REC_INDEX_S));

    RecInfo->RecStatus = DMX_REC_STATUS_START;

    ret = HI_SUCCESS;

out1:
    DmxPutRecInstance(DmxSet, RecId);
out0:
    return ret;
}

HI_S32 DMX_DRV_REC_StopRecChn(HI_U32 DmxId, HI_U32 RecId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_RecInfo_S  *RecInfo = HI_NULL;
    DMX_OQ_Info_S  *RecOqInfo = HI_NULL;
    Dmx_Rec_Data_Index_Helper *Entry, *Tmp;

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    RecInfo = &DmxSet->DmxRecInfo[RecId];
    RecOqInfo = &DmxSet->DmxOqInfo[RecInfo->RecOqId];

    if (unlikely(DMX_REC_STATUS_STOP == RecInfo->RecStatus))
    {
        ret = HI_SUCCESS;
        goto out1;
    }

    RecInfo->RecStatus = DMX_REC_STATUS_STOP;

    RecOqInfo->OqWakeUp = HI_TRUE;
    wake_up_interruptible(&RecOqInfo->OqWaitQueue);

    if (HI_UNF_DMX_REC_INDEX_TYPE_NONE != RecInfo->IndexType)
    {
        DmxStopSCD(DmxSet, RecId);
    }

    DmxFqStop(DmxSet, RecInfo->RecFqId);
    DmxOqStop(DmxSet, RecInfo->RecOqId);

    DmxUnsetRecBuf(DmxSet, RecId);
    DmxSetRecType(DmxSet, RecId, HI_UNF_DMX_REC_TYPE_BUTT);

    DmxRecFlush(DmxSet, RecId);

#ifdef DMX_REC_EXCLUDE_PID_SUPPORT
    if (HI_UNF_DMX_REC_TYPE_ALL_PID == RecInfo->RecType)
    {
        DmxHalDisableAllRecExcludePid(DmxSet, RecInfo->DmxId);
    }
#endif

    /* remove remain rec data&idx */
    RecInfo->RemIdxCnt = 0;
    list_for_each_entry_safe(Entry, Tmp, &RecInfo->head, list)
    {
        list_del(&Entry->list);
        HI_KFREE(HI_ID_DEMUX, Entry);
    }

    ret = HI_SUCCESS;

out1:
    DmxPutRecInstance(DmxSet, RecId);
out0:
    return ret;
}

static HI_S32 DmxAcquireRecData(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_U32 *PhyAddr, HI_U8 **KerAddr, HI_U32 *Len, HI_U32 Timeout)
{
    HI_S32 ret = HI_ERR_DMX_NOAVAILABLE_DATA;
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];
    DMX_FQ_Info_S  *FqInfo  = HI_NULL;
    DMX_OQ_Info_S  *OqInfo  = HI_NULL;
    HI_U32          Read;
    HI_U32          Write;

    FqInfo  = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    OqInfo  = &DmxSet->DmxOqInfo[RecInfo->RecOqId];

    DMXOsiOQGetReadWrite(DmxSet, OqInfo->u32OQId, &Write, &Read);
    if (Read != Write)
    {
        OQ_DescInfo_S  *OqDesc      = OqInfo->OqDescSetBase + Read;
        HI_U32          StartAddr   = OqDesc->start_addr;
        HI_U32          DataLen     = OqDesc->pvrctrl_datalen & 0xffff;

        OqInfo->u32ProcsBlk = Read + 1;
        if (OqInfo->u32ProcsBlk >= OqInfo->OQDepth)
        {
            OqInfo->u32ProcsBlk = 0;
        }

        if (KerAddr)
        {
            *KerAddr    = FqInfo->BufVirAddr + (StartAddr - FqInfo->BufPhyAddr);
        }

        *PhyAddr    = StartAddr;
        *Len        = DataLen;

        ret = HI_SUCCESS;
    }

    return ret;
}

HI_S32 DMX_DRV_REC_AcquireRecData(HI_U32 DmxId, HI_U32 RecId, HI_U32 *PhyAddr, HI_U8 **KerAddr, HI_U32 *Len, HI_U32 Timeout)
{
    HI_S32 ret = HI_ERR_DMX_NOAVAILABLE_DATA;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_RecInfo_S  *RecInfo = HI_NULL;
    DMX_OQ_Info_S  *OqInfo  = HI_NULL;

    do
    {
        ret = DmxGetRecInstance(DmxSet, RecId);
        if (HI_SUCCESS != ret)
        {
            break;
        }

        RecInfo = &DmxSet->DmxRecInfo[RecId];
        OqInfo  = &DmxSet->DmxOqInfo[RecInfo->RecOqId];

        ret = DmxAcquireRecData(DmxSet, RecId, PhyAddr, KerAddr, Len, Timeout);
        if (HI_SUCCESS == ret)
        {
            DmxPutRecInstance(DmxSet, RecId);
            break;
        }
        else
        {
            /* avoid deadlock due to timeout schedule */
            DmxPutRecInstance(DmxSet, RecId);

            if (Timeout)
            {
                OqInfo->OqWakeUp = HI_FALSE;

                DmxHalOQEnableOutputInt(DmxSet, OqInfo->u32OQId, HI_TRUE);
                ret = wait_event_interruptible_timeout(OqInfo->OqWaitQueue, OqInfo->OqWakeUp, msecs_to_jiffies(Timeout));
                DmxHalOQEnableOutputInt(DmxSet, OqInfo->u32OQId, HI_FALSE);

                if (0 == ret)
                {
                    ret = HI_ERR_DMX_TIMEOUT;
                    break;
                }
                else if (ret < 0)
                {
                    break;
                }

                Timeout = jiffies_to_msecs(ret);

                continue;
            }
            else
            {
                break;
            }
        }
    }while(1);

    return ret;
}

HI_S32 DMX_DRV_REC_ReleaseRecData(HI_U32 DmxId, HI_U32 RecId, HI_U32 PhyAddr, HI_U32 Len)
{
    HI_S32 ret = HI_FALSE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_RecInfo_S  *RecInfo = HI_NULL;

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto err;
    }

    RecInfo = &DmxSet->DmxRecInfo[RecId];

    ret = DmxOQRelease(DmxSet, RecInfo->RecFqId, RecInfo->RecOqId, PhyAddr, Len);

    DmxPutRecInstance(DmxSet, RecId);
err:
    return ret;
}

/*
 * reference linux kernel definition(cnt32_to_63.h).
 */
static HI_U32 DmxGetScrMonoMs(DMX_RecInfo_S  *RecInfo, const volatile DMX_IDX_DATA_S *ScData)
{
    union cnt32_to_63 {
        struct {
            HI_U32 lo, hi;
        };
        HI_U64 val;
    } __x;
    HI_U64 CurScrCnt;

    __x.hi = RecInfo->Cnt32To63Helper;
    smp_rmb();
    __x.lo = ScData->u32SrcClk;

    if (unlikely((HI_S32)(__x.hi ^ __x.lo) < 0))
    {
        RecInfo->Cnt32To63Helper = __x.hi = (__x.hi ^ 0x80000000) + (__x.hi >> 31);
    }

    CurScrCnt =  __x.val & 0x7fffffffffffffffULL;

    /*
     * we found that both maybe same when signal is weak.
     */
    if (unlikely(!(RecInfo->PrevScrClk <= CurScrCnt)))
    {
        HI_ERR_DEMUX("PrevScrClk(0x%llx) should less or equal than CurScrCnt(0x%llx).\n", RecInfo->PrevScrClk, CurScrCnt);
        BUG();
    }

    RecInfo->PrevScrClk = CurScrCnt;

    /* trans 90khz count to ms. */
    do_div(CurScrCnt, 90);

    return (HI_U32)CurScrCnt;
}

static HI_S32 DmxFormatSecureHevcIndex(Dmx_Set_S *DmxSet, DMX_RecInfo_S *RecInfo, FINDEX_SCD_S *pstFidx)
{
    HI_S32 ret = HI_FAILURE;
    MMZ_BUFFER_S MmzBuf = {0};
    HI_CHAR BufName[32];

    pstFidx->u32ExtraSCDataSize = sizeof(HI_U8) * HEVC_DUP_DATA_TOTAL_LEN;

    snprintf(BufName, sizeof(BufName), "DMX_h265IdxBuf[%u]", RecInfo->RecId);
    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(BufName, MMZ_OTHERS, pstFidx->u32ExtraSCDataSize, 0, &MmzBuf))
    {
        HI_ERR_DEMUX("create hevc index extra buffer failed\n");
        goto out0;
    }

    pstFidx->pu8ExtraSCData = MmzBuf.pu8StartVirAddr;
    pstFidx->u32ExtraSCDataPhyAddr = MmzBuf.u32StartPhyAddr;

    ret = DmxFixupHevcIndex(DmxSet, RecInfo, pstFidx);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    FIDX_FeedStartCode(RecInfo->PicParser, pstFidx);

out1:
    HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);
    pstFidx->pu8ExtraSCData = HI_NULL;
out0:
    return ret;
}

static HI_S32 DmxFormatHevcIndex(Dmx_Set_S *DmxSet, DMX_RecInfo_S *RecInfo, FINDEX_SCD_S *pstFidx)
{
    HI_S32 ret = HI_FAILURE;

    pstFidx->u32ExtraSCDataSize = sizeof(HI_U8) * HEVC_DUP_DATA_TOTAL_LEN;
    pstFidx->pu8ExtraSCData = HI_KMALLOC(HI_ID_DEMUX, pstFidx->u32ExtraSCDataSize, GFP_KERNEL);
    if (!pstFidx->pu8ExtraSCData)
    {
        HI_ERR_DEMUX("create hevc index extra buffer failed.\n");
        goto out0;
    }

    ret = DmxFixupHevcIndex(DmxSet, RecInfo, pstFidx);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    FIDX_FeedStartCode(RecInfo->PicParser, pstFidx);

out1:
    HI_KFREE(HI_ID_DEMUX, pstFidx->pu8ExtraSCData);
    pstFidx->pu8ExtraSCData = HI_NULL;
out0:
    return ret;
}

static HI_S32 DmxFormatVidIndex(Dmx_Set_S *DmxSet, DMX_RecInfo_S *RecInfo, FINDEX_SCD_S *pstFidx)
{
    HI_S32 ret = HI_FAILURE;
    HI_MPI_DMX_REC_INDEX_S *CurrFrame = &RecInfo->LastFrameInfo;
    HI_U64 u64LastFrameHeadOffset = CurrFrame->u64GlobalOffset;

    mb();

    if (HI_UNF_VCODEC_TYPE_HEVC == RecInfo->VCodecType)
    {
        if (HI_UNF_DMX_SECURE_MODE_TEE == RecInfo->SecureMode)
        {
            ret = DmxFormatSecureHevcIndex(DmxSet, RecInfo, pstFidx);
            if (HI_SUCCESS != ret)
            {
                goto out;
            }
        }
        else
        {
            ret = DmxFormatHevcIndex(DmxSet, RecInfo, pstFidx);
            if (HI_SUCCESS != ret)
            {
                goto out;
            }
        }
    }
    else
    {
        FIDX_FeedStartCode(RecInfo->PicParser, pstFidx);
    }

    /* CurrFrame will be update after FIDX_FeedStartCode */
    ret = u64LastFrameHeadOffset < CurrFrame->u64GlobalOffset ? HI_SUCCESS : HI_FAILURE;

out:
    return ret;
}

static HI_S32 DmxAcquireRecIndex(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_MPI_DMX_REC_INDEX_S *RecIndex, HI_U32 Timeout)
{
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];
    DMX_FQ_Info_S  *FqInfo  = HI_NULL;
    DMX_OQ_Info_S  *OqInfo  = HI_NULL;
    HI_U32          Read, Write;
    HI_U8            *KerAddr;
    HI_BOOL         bUseTimeStamp = HI_FALSE;

    if (unlikely(DMX_REC_STATUS_START != RecInfo->RecStatus))
    {
        HI_ERR_DEMUX("start rec channel first!\n");
        return HI_ERR_DMX_NOT_START_REC_CHAN;
    }

    if (unlikely(RecInfo->enRecTimeStamp >= DMX_REC_TIMESTAMP_ZERO))
    {
        bUseTimeStamp = HI_TRUE;
    }

    FqInfo  = &DmxSet->DmxFqInfo[RecInfo->ScdFqId];
    OqInfo  = &DmxSet->DmxOqInfo[RecInfo->ScdOqId];

    DMXOsiOQGetReadWrite(DmxSet, OqInfo->u32OQId, &Write, &Read);
    if (Read != Write)
    {
        HI_BOOL bGetValidIndex = HI_FALSE;
        HI_MPI_DMX_REC_INDEX_S *CurrFrame   = &RecInfo->LastFrameInfo;
        HI_U32                  CurFrameMs  = 0;
        OQ_DescInfo_S          *OqDesc      = OqInfo->OqDescSetBase + Read;
        HI_U32                  PhyAddr     = OqDesc->start_addr;
        HI_U32                  DataLen     = OqDesc->pvrctrl_datalen & 0xffff;
        FINDEX_SCD_S            EsScd;
        DMX_IDX_DATA_S         *ScData;
        HI_U32                  ScCount;
        HI_U32                  i;

        KerAddr = FqInfo->BufVirAddr + (PhyAddr - FqInfo->BufPhyAddr) + OqInfo->u32ProcsOffset;

        ScData  = (DMX_IDX_DATA_S*)KerAddr;
        ScCount = (DataLen - OqInfo->u32ProcsOffset)/ sizeof(DMX_IDX_DATA_S);

        if (HI_UNF_DMX_REC_INDEX_TYPE_VIDEO == RecInfo->IndexType)
        {
            for (i = 0; i < ScCount; i++)
            {
                if (HI_SUCCESS == DmxScdToVideoIndex(bUseTimeStamp,ScData, &EsScd))
                {
                    if (HI_SUCCESS == DmxFormatVidIndex(DmxSet, RecInfo, &EsScd))
                    {
                        CurFrameMs = DmxGetScrMonoMs(RecInfo, ScData);

                        memcpy(RecIndex, CurrFrame, sizeof(HI_MPI_DMX_REC_INDEX_S));

                        bGetValidIndex = HI_TRUE;
                    }
                }

                ScData ++;

                if (HI_TRUE == bGetValidIndex)
                {
                    break;
                }
            }
        }
        else
        {
            HI_MPI_DMX_REC_INDEX_S ThisFrame = {0};

            for (i = 0; i < ScCount; i++)
            {
                if (HI_SUCCESS == DmxScdToAudioIndex(&ThisFrame, ScData))
                {
                    /*only for the first scd (index) will enter this condition */
                    if (unlikely(RecInfo->LastFrameInfo.u32DataTimeMs == 0))
                    {
                        memcpy(&RecInfo->LastFrameInfo, &ThisFrame, sizeof(HI_MPI_DMX_REC_INDEX_S));
                    }
                    else
                    {
                        CurFrameMs = DmxGetScrMonoMs(RecInfo, ScData);

                        RecInfo->LastFrameInfo.u32FrameSize = ThisFrame.u64GlobalOffset - RecInfo->LastFrameInfo.u64GlobalOffset;
                        memcpy(RecIndex, &RecInfo->LastFrameInfo, sizeof(HI_MPI_DMX_REC_INDEX_S));
                        memcpy(&RecInfo->LastFrameInfo, &ThisFrame, sizeof(HI_MPI_DMX_REC_INDEX_S));

                        bGetValidIndex = HI_TRUE;
                    }
                }

                ScData ++;

                if (HI_TRUE == bGetValidIndex)
                {
                    break;
                }
            }
        }

        OqInfo->u32ProcsOffset += (HI_U32)((HI_U8*)ScData - KerAddr);
        if (OqInfo->u32ProcsOffset == DataLen)
        {
            DmxOQRelease(DmxSet, OqInfo->u32FQId, OqInfo->u32OQId, PhyAddr, DataLen);
        }

        if (bGetValidIndex)
        {
            if (unlikely(0 == RecInfo->FirstFrameMs))
            {
                RecInfo->FirstFrameMs = CurFrameMs;
            }

            /*
             * for some short frame, the scr clock(90khz) count maybe same, the frame time stamp plus 1 for keeping uniqueness of the time stamp.
             */
            if (unlikely(CurFrameMs <= RecInfo->PrevFrameMs))
            {
                HI_DBG_DEMUX("PrevFrameMs:0x%x, CurFrameMs:0x%x, Offset:0x%llx, FrameSize:%d.\n", RecInfo->PrevFrameMs,
                    CurFrameMs, RecIndex->u64GlobalOffset, RecIndex->u32FrameSize);

                CurFrameMs = RecInfo->PrevFrameMs + 1;
            }

            RecInfo->PrevFrameMs = CurFrameMs;

            RecIndex->u32DataTimeMs = CurFrameMs - RecInfo->FirstFrameMs;

            return HI_SUCCESS;
        }
    }

    return HI_ERR_DMX_NOAVAILABLE_DATA;
}

static HI_S32 DMXGetRecDataIndex(Dmx_Set_S *DmxSet, HI_U32 RecId)
{
    HI_S32 ret = HI_FAILURE;
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];
    DMX_FQ_Info_S  *FqInfo  = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    HI_MPI_DMX_REC_INDEX_S *NewRecIndex = HI_NULL;
    HI_MPI_DMX_REC_DATA_S  *NewRecData = HI_NULL;
    HI_U32 NewFrameStartAddr, NewFrameEndAddr;  /* buffer inner addr */
    Dmx_Rec_Data_Index_Helper *helper;

    helper = HI_KMALLOC(HI_ID_DEMUX, sizeof(Dmx_Rec_Data_Index_Helper), GFP_KERNEL);
    if (unlikely(!helper))
    {
        HI_ERR_DEMUX("malloc rec data&idx helper failed.\n");
        ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
        goto out0;
    }

    memset(helper, 0, sizeof(Dmx_Rec_Data_Index_Helper));

    INIT_LIST_HEAD(&helper->list);
    NewRecData = &helper->data[0];
    NewRecIndex = &helper->index;

    ret = DmxAcquireRecIndex(DmxSet, RecId, NewRecIndex, 0);
    if (HI_SUCCESS == ret)
    {
        HI_U64 u64FrameGlobalOffset = NewRecIndex->u64GlobalOffset;

        NewFrameStartAddr = FqInfo->BufPhyAddr + do_div(u64FrameGlobalOffset, (HI_U64)FqInfo->BufSize);
        NewFrameEndAddr = NewFrameStartAddr + NewRecIndex->u32FrameSize;

        /* drop exception frame by simple rule. */
        if (unlikely(NewRecIndex->u32FrameSize >= FqInfo->BufSize || 0 == NewRecIndex->u32FrameSize))
        {
            HI_ERR_DEMUX("Drop exception frame(0x%x, 0x%x), fq(0x%x, 0x%x).\n", NewFrameStartAddr, NewRecIndex->u32FrameSize,
                                        FqInfo->BufPhyAddr, FqInfo->BufSize);

            /* keep ret = success. */
            goto out1;
        }

        /* skip verify first frame because it has no prev frame. */
        if (0 != RecInfo->PrevFrameEndAddr)
        {
            /* in general, the phy address of adjacent frame is continuous, but this rule maybe broken by unstable stream(such as , weak signal). */
            if (unlikely(NewFrameStartAddr != RecInfo->PrevFrameEndAddr))
            {
                HI_U32 Distance __attribute__((unused)) = NewFrameStartAddr > RecInfo->PrevFrameEndAddr ?
                    NewFrameStartAddr - RecInfo->PrevFrameEndAddr : NewFrameStartAddr + FqInfo->BufSize - RecInfo->PrevFrameEndAddr;

                HI_WARN_DEMUX("Found discontinue frame data (%u)!\n", Distance);
            }
        }

        /* rewind frame */
        if ( NewFrameEndAddr > FqInfo->BufPhyAddr + FqInfo->BufSize)
        {
            HI_MPI_DMX_REC_DATA_S *NewRecData_2 = &helper->data[1];

            /* slice 1 */
            NewRecData->u32DataPhyAddr = NewFrameStartAddr;
            NewRecData->pDataAddr = (HI_U8  *)(FqInfo->BufVirAddr + NewRecData->u32DataPhyAddr - FqInfo->BufPhyAddr );
            NewRecData->u32Len = FqInfo->BufPhyAddr + FqInfo->BufSize - NewRecData->u32DataPhyAddr;

            /* slice 2 */
            NewRecData_2->u32DataPhyAddr = FqInfo->BufPhyAddr;
            NewRecData_2->pDataAddr = (HI_U8  *)(FqInfo->BufVirAddr + NewRecData_2->u32DataPhyAddr - FqInfo->BufPhyAddr );
            NewRecData_2->u32Len = NewRecIndex->u32FrameSize - NewRecData->u32Len;

            helper->Rewind = HI_TRUE;
            helper->DataSel = 0;

            /* save and adjust frame end addr */
            RecInfo->PrevFrameEndAddr = NewRecData_2->u32DataPhyAddr + NewRecData_2->u32Len;
            RecInfo->PrevFrameEndAddr = (RecInfo->PrevFrameEndAddr == FqInfo->BufPhyAddr + FqInfo->BufSize) ?
                                                                FqInfo->BufPhyAddr : RecInfo->PrevFrameEndAddr;
        }
        else
        {
            NewRecData->u32DataPhyAddr = NewFrameStartAddr;
            NewRecData->pDataAddr = (HI_U8  *)(FqInfo->BufVirAddr + NewRecData->u32DataPhyAddr - FqInfo->BufPhyAddr );
            NewRecData->u32Len = NewRecIndex->u32FrameSize;

            helper->Rewind = HI_FALSE;
            helper->DataSel = 0;

            /* save and adjust frame end addr */
            RecInfo->PrevFrameEndAddr = NewFrameEndAddr;
            RecInfo->PrevFrameEndAddr = (RecInfo->PrevFrameEndAddr == FqInfo->BufPhyAddr + FqInfo->BufSize) ?
                                                                FqInfo->BufPhyAddr : RecInfo->PrevFrameEndAddr;
        }

        list_add_tail(&helper->list, &RecInfo->head);
    }
    else
    {
        goto out1;
    }

    return HI_SUCCESS;

out1:
    HI_KFREE(HI_ID_DEMUX, helper);
out0:
    return ret;
}

static HI_VOID DMXNewNoRewindFrame(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo, Dmx_Rec_Data_Index_Helper *Entry)
{
    DMX_FQ_Info_S  *FqInfo  = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    HI_MPI_DMX_REC_DATA_S  *RemRecData = &RecInfo->RemRecData;
    HI_MPI_DMX_REC_INDEX_S *RemRecIdx = HI_NULL;

    /* data locate at before block */
    if (unlikely(RecInfo->BlockStartAddr >= Entry->data[0].u32DataPhyAddr + Entry->data[0].u32Len))
    {
        RemRecData->u32DataPhyAddr = RecInfo->BlockStartAddr;
        RemRecData->pDataAddr = (HI_U8  *)(FqInfo->BufVirAddr + RemRecData->u32DataPhyAddr - FqInfo->BufPhyAddr);
        RemRecData->u32Len = FqInfo->BlockSize;
    }
    else /* data locate at after block */
    {
        /* this frame bigger than block */
        if (Entry->data[0].u32DataPhyAddr + Entry->data[0].u32Len - RecInfo->BlockStartAddr > FqInfo->BlockSize)
        {
            RemRecData->u32DataPhyAddr = RecInfo->BlockStartAddr;
            RemRecData->pDataAddr = (HI_U8  *)(FqInfo->BufVirAddr + RemRecData->u32DataPhyAddr - FqInfo->BufPhyAddr);
            RemRecData->u32Len = FqInfo->BlockSize;
        }
        else
        {
            RemRecData->u32DataPhyAddr = RecInfo->BlockStartAddr;
            RemRecData->pDataAddr = (HI_U8  *)(FqInfo->BufVirAddr + RemRecData->u32DataPhyAddr - FqInfo->BufPhyAddr);
            RemRecData->u32Len = Entry->data[0].u32DataPhyAddr + Entry->data[0].u32Len - RecInfo->BlockStartAddr;

            BUG_ON(RemRecData->u32Len > FqInfo->BlockSize);

            RemRecIdx = &RecInfo->RemRecIdx[RecInfo->RemIdxCnt ++];
            memcpy(RemRecIdx, &Entry->index, sizeof(HI_MPI_DMX_REC_INDEX_S));

            list_del(&Entry->list);

            HI_KFREE(HI_ID_DEMUX, Entry);
        }
    }
}

static HI_VOID DMXNewRewindFrame(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo, Dmx_Rec_Data_Index_Helper *Entry)
{
    DMX_FQ_Info_S  *FqInfo  = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    HI_MPI_DMX_REC_DATA_S  *RemRecData = &RecInfo->RemRecData;
    HI_MPI_DMX_REC_INDEX_S *RemRecIdx = HI_NULL;

    if (0 == Entry->DataSel)  /* slice 1*/
    {
        BUG_ON(RecInfo->BlockStartAddr >= Entry->data[0].u32DataPhyAddr + Entry->data[0].u32Len);

        if (Entry->data[0].u32DataPhyAddr + Entry->data[0].u32Len - RecInfo->BlockStartAddr >= FqInfo->BlockSize)
        {
            RemRecData->u32DataPhyAddr = RecInfo->BlockStartAddr;
            RemRecData->pDataAddr = (HI_U8  *)(FqInfo->BufVirAddr + RemRecData->u32DataPhyAddr - FqInfo->BufPhyAddr);
            RemRecData->u32Len = FqInfo->BlockSize;

            /* last block properly */
            if (RemRecData->u32DataPhyAddr + RemRecData->u32Len == FqInfo->BufPhyAddr + FqInfo->BufSize)
            {
                Entry->DataSel = 1;
            }
        }
        else
        {
            BUG();
        }
    }
    else /* slice 2*/
    {
        BUG_ON(RecInfo->BlockStartAddr >= Entry->data[1].u32DataPhyAddr + Entry->data[1].u32Len);

        /* this frame bigger than block */
        if (Entry->data[1].u32DataPhyAddr + Entry->data[1].u32Len - RecInfo->BlockStartAddr > FqInfo->BlockSize)
        {
            RemRecData->u32DataPhyAddr = RecInfo->BlockStartAddr;
            RemRecData->pDataAddr = (HI_U8  *)(FqInfo->BufVirAddr + RemRecData->u32DataPhyAddr - FqInfo->BufPhyAddr);
            RemRecData->u32Len = FqInfo->BlockSize;
        }
        else
        {
            RemRecData->u32DataPhyAddr = RecInfo->BlockStartAddr;
            RemRecData->pDataAddr = (HI_U8  *)(FqInfo->BufVirAddr + RemRecData->u32DataPhyAddr - FqInfo->BufPhyAddr);
            RemRecData->u32Len = Entry->data[1].u32DataPhyAddr + Entry->data[1].u32Len - RecInfo->BlockStartAddr;

            BUG_ON(RemRecData->u32Len > FqInfo->BlockSize);

            RemRecIdx = &RecInfo->RemRecIdx[RecInfo->RemIdxCnt ++];
            memcpy(RemRecIdx, &Entry->index, sizeof(HI_MPI_DMX_REC_INDEX_S));

            list_del(&Entry->list);

            HI_KFREE(HI_ID_DEMUX, Entry);
        }
    }
}

static HI_VOID DMXAppendNoRewindFrame(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo, Dmx_Rec_Data_Index_Helper *Entry)
{
    DMX_FQ_Info_S  *FqInfo  = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    HI_MPI_DMX_REC_DATA_S  *RemRecData = &RecInfo->RemRecData;
    HI_MPI_DMX_REC_INDEX_S *RemRecIdx = HI_NULL;

    /* data locate at before block */
    if (unlikely(RecInfo->BlockStartAddr >= Entry->data[0].u32DataPhyAddr + Entry->data[0].u32Len))
    {
        RemRecData->u32Len = FqInfo->BlockSize;
    }
    else /* data locate at after block */
    {
        /* this frame bigger than block */
        if (Entry->data[0].u32DataPhyAddr + Entry->data[0].u32Len - RecInfo->BlockStartAddr > FqInfo->BlockSize)
        {
            RemRecData->u32Len = FqInfo->BlockSize;
        }
        else
        {
            RemRecData->u32Len = Entry->data[0].u32DataPhyAddr + Entry->data[0].u32Len - RecInfo->BlockStartAddr;

            BUG_ON(RemRecData->u32Len > FqInfo->BlockSize);

            RemRecIdx = &RecInfo->RemRecIdx[RecInfo->RemIdxCnt ++];
            memcpy(RemRecIdx, &Entry->index, sizeof(HI_MPI_DMX_REC_INDEX_S));

            list_del(&Entry->list);

            HI_KFREE(HI_ID_DEMUX, Entry);
        }
    }
}

static HI_VOID DMXAppendRewindFrame(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo, Dmx_Rec_Data_Index_Helper *Entry)
{
    DMX_FQ_Info_S  *FqInfo  = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    HI_MPI_DMX_REC_DATA_S  *RemRecData = &RecInfo->RemRecData;

    if (0 == Entry->DataSel)  /* slice 1*/
    {
        BUG_ON(RecInfo->BlockStartAddr >= Entry->data[0].u32DataPhyAddr + Entry->data[0].u32Len);

        if (Entry->data[0].u32DataPhyAddr + Entry->data[0].u32Len - RecInfo->BlockStartAddr >= FqInfo->BlockSize)
        {
            RemRecData->u32Len = FqInfo->BlockSize;

            /* last block properly */
            if (RemRecData->u32DataPhyAddr + RemRecData->u32Len == FqInfo->BufPhyAddr + FqInfo->BufSize)
            {
                Entry->DataSel = 1;
            }
        }
        else
        {
            BUG();
        }
    }
    else /* slice 2 */
    {
        /* refer to DMX_NewRewindFrame slice 2 */
        BUG();
    }
}

static HI_S32 DMXAcquireRecDataIndex(Dmx_Set_S *DmxSet, HI_U32 RecId, HI_MPI_DMX_REC_DATA_INDEX_S *pstRecDataIdx)
{
    HI_S32 ret = HI_FAILURE;
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];
    DMX_FQ_Info_S  *FqInfo  = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    DMX_OQ_Info_S  *OqInfo  = &DmxSet->DmxOqInfo[RecInfo->RecOqId];
    HI_MPI_DMX_REC_DATA_S  *RemRecData = &RecInfo->RemRecData;

    BUG_ON((RecInfo->BlockStartAddr - FqInfo->BufPhyAddr) % FqInfo->BlockSize);

    if (0 == RecInfo->RemIdxCnt)
    {
        memset(RemRecData, 0, sizeof(HI_MPI_DMX_REC_DATA_S));
    }

    while(RemRecData->u32Len < FqInfo->BlockSize)
    {
        if (!list_empty(&RecInfo->head))
        {
            Dmx_Rec_Data_Index_Helper *Entry = list_first_entry(&RecInfo->head, Dmx_Rec_Data_Index_Helper, list);

            /* new */
            if (0 == RecInfo->RemIdxCnt)
            {
                if (HI_FALSE == Entry->Rewind)
                {
                    DMXNewNoRewindFrame(DmxSet, RecInfo, Entry);
                }
                else /* rewind frame */
                {
                    DMXNewRewindFrame(DmxSet, RecInfo, Entry);
                }
            }
            else if (RecInfo->RemIdxCnt < DMX_MAX_IDX_ACQUIRED_EACH_TIME) /* append */
            {
                if (HI_FALSE == Entry->Rewind)
                {
                    DMXAppendNoRewindFrame(DmxSet, RecInfo, Entry);
                }
                else /* rewind frame */
                {
                    DMXAppendRewindFrame(DmxSet, RecInfo, Entry);
                }
            }
            else
            {
                BUG();
            }
        }
        else /* there is no available index now in list */
        {
            /* almost overflow maybe clear and disabled in ISR, so we always enable it here. */
            DmxFqCheckOverflowInt(DmxSet, RecInfo->RecFqId);
            if (unlikely(FqInfo->FqOverflowCount))
            {
                HI_ERR_DEMUX("Rec Buf(%d) Data overflowed.\n", RecInfo->RecFqId);
                ret = HI_ERR_DMX_OVERFLOW_BUFFER;
                break;
            }

            ret = DMXGetRecDataIndex(DmxSet, RecId);
            if (HI_SUCCESS != ret)
            {
                ret = HI_ERR_DMX_NOAVAILABLE_DATA;
                break;
            }
        }
    }

    /* duplicate rec data and index  */
    if (RemRecData->u32Len == FqInfo->BlockSize)
    {
        pstRecDataIdx->u32RecDataCnt = 1;
        pstRecDataIdx->u32IdxNum = RecInfo->RemIdxCnt;

        memcpy(&pstRecDataIdx->stRecData[0], RemRecData, sizeof(HI_MPI_DMX_REC_DATA_S));

        if (sizeof(HI_MPI_DMX_REC_INDEX_S) * RecInfo->RemIdxCnt <= sizeof(RecInfo->RemRecIdx))
        {
            memcpy(&pstRecDataIdx->stIndex[0], &RecInfo->RemRecIdx[0], sizeof(HI_MPI_DMX_REC_INDEX_S) * RecInfo->RemIdxCnt);
        }
        else
        {
            BUG();
        }

        RecInfo->BlockStartAddr = RemRecData->u32DataPhyAddr + RemRecData->u32Len;

        OqInfo->u32ProcsBlk = (RecInfo->BlockStartAddr - FqInfo->BufPhyAddr)/FqInfo->BlockSize;

        /* rewind buffer if possible */
        RecInfo->BlockStartAddr = (RecInfo->BlockStartAddr == FqInfo->BufPhyAddr + FqInfo->BufSize) ?
                                                FqInfo->BufPhyAddr : RecInfo->BlockStartAddr ;
        RecInfo->RemIdxCnt = 0;

        ret = HI_SUCCESS;
    }

    return ret;
}

HI_S32 DMX_DRV_REC_AcquireRecDataIndex(HI_U32 DmxId, HI_U32 RecId, HI_MPI_DMX_REC_DATA_INDEX_S *pstRecDataIdx)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_RecInfo_S  *RecInfo = HI_NULL;

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    RecInfo = &DmxSet->DmxRecInfo[RecId];

    if (unlikely(DMX_REC_STATUS_START != RecInfo->RecStatus))
    {
        HI_ERR_DEMUX("start rec channel first!");
        ret = HI_ERR_DMX_NOT_START_REC_CHAN;
        goto out1;
    }

    memset(pstRecDataIdx, 0x0, sizeof(HI_MPI_DMX_REC_DATA_INDEX_S));

    if (HI_UNF_DMX_REC_INDEX_TYPE_NONE == RecInfo->IndexType)
    {
        HI_MPI_DMX_REC_DATA_S  *RecData  = &pstRecDataIdx->stRecData[0];

        ret = DmxAcquireRecData(DmxSet, RecId, &RecData->u32DataPhyAddr, HI_NULL, &RecData->u32Len, 0);
        if ( HI_SUCCESS ==  ret)
        {
            pstRecDataIdx->u32IdxNum = 0;
            pstRecDataIdx->u32RecDataCnt = 1;
        }
    }
    else
    {
        ret = DMXAcquireRecDataIndex(DmxSet, RecId, pstRecDataIdx);
    }

out1:
    DmxPutRecInstance(DmxSet, RecId);
out0:
    return ret;
}

HI_S32 DMX_DRV_REC_ReleaseRecDataIndex(HI_U32 DmxId, HI_U32 RecId, HI_MPI_DMX_REC_DATA_INDEX_S *pstRecDataIdx)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_RecInfo_S  *RecInfo = HI_NULL;
    DMX_FQ_Info_S  *FqInfo  = HI_NULL;
    DMX_OQ_Info_S  *OqInfo  = HI_NULL;
    OQ_DescInfo_S  *OqDesc;
    HI_U32 OqRead = 0;
    HI_U32 index;

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    RecInfo = &DmxSet->DmxRecInfo[RecId];

    if (DMX_REC_STATUS_START != RecInfo->RecStatus)
    {
        HI_ERR_DEMUX("start rec channel first!");
        ret =  HI_ERR_DMX_NOT_START_REC_CHAN;
        goto out1;
    }

    FqInfo  = &DmxSet->DmxFqInfo[RecInfo->RecFqId];
    OqInfo  = &DmxSet->DmxOqInfo[RecInfo->RecOqId];

    BUG_ON(pstRecDataIdx->u32RecDataCnt > 2);

    for (index = 0; index < pstRecDataIdx->u32RecDataCnt; index ++)
    {
        HI_MPI_DMX_REC_DATA_S  *RecData = &pstRecDataIdx->stRecData[index];
        HI_U32 RelBlkAddr = RecData->u32DataPhyAddr;

        while(RelBlkAddr < RecData->u32DataPhyAddr + RecData->u32Len)
        {
            DMXOsiOQGetReadWrite(DmxSet, OqInfo->u32OQId, HI_NULL, &OqRead);

            OqDesc = OqInfo->OqDescSetBase + OqRead;

            if ((RelBlkAddr != OqDesc->start_addr) || ((OqDesc->pvrctrl_datalen & 0xffff) != FqInfo->BlockSize))
            {
                HI_ERR_DEMUX("Release block(0x%x) failed, current oq desc(0x%x, 0x%x)!\n", RelBlkAddr, OqDesc->start_addr, (OqDesc->pvrctrl_datalen & 0xffff));

                ret = HI_ERR_DMX_INVALID_PARA;
                goto out1;
            }

            /* only release one blk one time. */

            DmxOQReleaseByBlockCnt(DmxSet, RecInfo->RecFqId, OqInfo->u32OQId, 1);

            RelBlkAddr += FqInfo->BlockSize;
        }
    }

    ret = HI_SUCCESS;

out1:
    DmxPutRecInstance(DmxSet, RecId);
out0:
    return ret;
}

HI_S32 DMX_DRV_REC_AcquireRecIndex(HI_U32 DmxId, HI_U32 RecId, HI_MPI_DMX_REC_INDEX_S *RecIndex, HI_U32 Timeout)
{
    HI_S32 ret = HI_FALSE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_RecInfo_S  *RecInfo = HI_NULL;
    DMX_OQ_Info_S  *OqInfo  = HI_NULL;

    do
    {
        ret = DmxGetRecInstance(DmxSet, RecId);
        if (HI_SUCCESS != ret)
        {
            break;
        }

        RecInfo = &DmxSet->DmxRecInfo[RecId];

        if (unlikely(HI_UNF_DMX_REC_INDEX_TYPE_NONE == RecInfo->IndexType))
        {
            ret = HI_ERR_DMX_NOAVAILABLE_DATA;
            DmxPutRecInstance(DmxSet, RecId);
            break;
        }

        if (unlikely(HI_UNF_VCODEC_TYPE_HEVC == RecInfo->VCodecType))
        {
            ret = HI_ERR_DMX_INVALID_PARA;
            DmxPutRecInstance(DmxSet, RecId);

            HI_ERR_DEMUX("Acquire HEVC(265) index with HI_UNF_DMX_AcquireRecIndex has deprecated by HI_UNF_DMX_AcquireRecDataAndIndex.\n");
            break;
        }

        OqInfo  = &DmxSet->DmxOqInfo[RecInfo->ScdOqId];

        ret = DmxAcquireRecIndex(DmxSet, RecId, RecIndex, Timeout);
        if (HI_SUCCESS == ret)
        {
            DmxPutRecInstance(DmxSet, RecId);
            break;
        }
        else
        {
            /* avoid deadlock due to timeout schedule */
            DmxPutRecInstance(DmxSet, RecId);

            if (Timeout)
            {
                OqInfo->OqWakeUp = HI_FALSE;

                DmxHalOQEnableOutputInt(DmxSet, OqInfo->u32OQId, HI_TRUE);
                ret = wait_event_interruptible_timeout(OqInfo->OqWaitQueue, OqInfo->OqWakeUp, msecs_to_jiffies(Timeout));
                DmxHalOQEnableOutputInt(DmxSet, OqInfo->u32OQId, HI_FALSE);

                if (0 == ret)
                {
                    ret = HI_ERR_DMX_TIMEOUT;
                    break;
                }
                else if (ret < 0)
                {
                    break;
                }

                Timeout = jiffies_to_msecs(ret);

                continue;
            }
            else
            {
                break;
            }
        }
    }while(1);

    return ret;
}

HI_S32 DMX_DRV_REC_GetRecBufferStatus(HI_U32 DmxId, HI_U32 RecId, HI_UNF_DMX_RECBUF_STATUS_S *BufStatus)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_RecInfo_S  *RecInfo = HI_NULL;
    DMX_FQ_Info_S  *FqInfo  = HI_NULL;

    ret = DmxGetRecInstance(DmxSet, RecId);
    if (HI_SUCCESS != ret)
    {
        goto err;
    }

    RecInfo = &DmxSet->DmxRecInfo[RecId];
    FqInfo  = &DmxSet->DmxFqInfo[RecInfo->RecFqId];

    BufStatus->u32BufSize   = FqInfo->BufSize;
    BufStatus->u32UsedSize  = 0;

    if (DMX_REC_STATUS_START == RecInfo->RecStatus)
    {
        HI_U32  Read, Write;

        DmxHalGetFQReadWritePtr(DmxSet, RecInfo->RecFqId, &Read, &Write);

        if (Read < Write)
        {
            BufStatus->u32UsedSize = (FqInfo->FQDepth - 1 - Write + Read) * FqInfo->BlockSize;
        }
        else if (Read > Write)
        {
            BufStatus->u32UsedSize = (Read - Write) * FqInfo->BlockSize;
        }
    }

    ret = HI_SUCCESS;

    DmxPutRecInstance(DmxSet, RecId);
err:
    return ret;
}

HI_S32 DMX_OsiDeviceInit(HI_VOID)
{
    HI_S32 ret;

    ret = DMX_OsiInit();
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    FIDX_Init(DmxRecUpdateFrameInfo);

    return HI_SUCCESS;
}

HI_VOID DMX_OsiDeviceDeInit(HI_VOID)
{
    FIDX_DeInit();

    DMX_OsiDeInit();
}

/*****************************************************************************
 Prototype    : DMX_OsiSuspend
 Description  : Demux
 Input        : None
 Output       : None
 Return Value : None
*****************************************************************************/
HI_S32 DMX_OsiSuspend(PM_BASEDEV_S *himd, pm_message_t state)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;

    if (DMX_CLUSTER_INACTIVED == DmxCluster->Ops->GetClusterState())
    {
        goto out;
    }

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        HI_U32 idx;

        mutex_lock(&DmxSet->LockAllRmx);
        for_each_set_bit(idx, DmxSet->RmxChanBitmap, DmxSet->RmxChanCnt)
        {
           RMX_Chan_Info_S *RmxChanInfo = &DmxSet->RmxChanInfo[idx];

           RMX_OsiStop(RmxChanInfo->RmxId, idx);
        }
        mutex_unlock(&DmxSet->LockAllRmx);

        mutex_lock(&DmxSet->LockAllRec);
        for_each_set_bit(idx, DmxSet->RecBitmap, DmxSet->DmxRecCnt)
        {
            DMX_RecInfo_S *RecInfo = &DmxSet->DmxRecInfo[idx];

            DMX_DRV_REC_StopRecChn(RecInfo->DmxId, idx);
        }
        mutex_unlock(&DmxSet->LockAllRec);

        mutex_lock(&DmxSet->LockAllChn);
        for_each_set_bit(idx, DmxSet->ChnBitmap, DmxSet->DmxChanCnt)
        {
            DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[idx];

            DMX_OsiCloseChannel(ChanInfo->DmxId, idx);
        }
        mutex_unlock(&DmxSet->LockAllChn);
    }

out:
    /* Deconfig the clock */
    DmxHalDeConfigHardware();
    HI_PRINT("DEMUX suspend OK\n");

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : DMX_OsiResume
 Description  : Demux
 Input        : None
 Output       : None
 Return Value : None
 Others       :
*****************************************************************************/
static HI_S32 __DMX_OsiResume(Dmx_Set_S *DmxSet, PM_BASEDEV_S *himd)
{
    HI_UNF_DMX_PORT_ATTR_S  PortAttr;
    HI_UNF_DMX_TSO_PORT_ATTR_S  TSOPortAttr;
    HI_U32 i, j;

    if (HI_SUCCESS != DmxReset(DmxSet))
    {
        return HI_FAILURE;
    }

#ifdef DMX_DESCRAMBLER_SUPPORT
    DescInitHardFlag(DmxSet);
#endif

    DmxFqStart(DmxSet, DMX_FQ_COMMOM);

    for (i = 0; i < DmxSet->TunerPortCnt; i++)
    {
        __DMX_OsiTunerPortGetAttr(DmxSet, i, &PortAttr);
        __DMX_OsiTunerPortSetAttr(DmxSet, i, &PortAttr);

        DmxHalDvbPortSetTsCountCtrl(DmxSet, i, TS_COUNT_CRTL_START);
        DmxHalDvbPortSetErrTsCountCtrl(DmxSet, i, TS_COUNT_CRTL_START);
    }

    for (i = 0; i < DmxSet->RamPortCnt; i++)
    {
        DMX_RamPort_Info_S *PortInfo = &DmxSet->RamPortInfo[i];

        __DMX_OsiRamPortGetAttr(DmxSet, i, &PortAttr);
        __DMX_OsiRamPortSetAttr(DmxSet, i, &PortAttr);

        DmxHalIPPortEnableInt(DmxSet, i);
        DmxHalIPPortSetIntCnt(DmxSet, i, DMX_DEFAULT_INT_CNT);

        DmxHalIPPortSetTsCountCtrl(DmxSet, i, HI_TRUE);

        if (PortInfo->PhyAddr)
        {
            PortInfo->DescCurrAddr  = (HI_U32*)PortInfo->DescKerAddr;
            PortInfo->DescWrite     = 0;
#ifndef DMX_TS_BUF_VERSION_2
            PortInfo->Read          = 0;
            PortInfo->Write         = 0;
            PortInfo->ReqAddr       = 0;
            PortInfo->ReqLen        = 0;
#endif
            PortInfo->WaitLen       = 0;
            PortInfo->WakeUp        = HI_FALSE;

            PortInfo->GetCount      = 0;
            PortInfo->GetValidCount = 0;
            PortInfo->PutCount      = 0;

            /* reenable ram port, refer to TsBufferConfig */
            __DmxHalIPPortDescSet(DmxSet, i, PortInfo->DescPhyAddr, PortInfo->DescDepth);
            __DmxHalIPPortRateSet(DmxSet, i, PortInfo->IpRate);
            __DmxHalIPPortSetOutInt(DmxSet, i, HI_TRUE);
            __DmxHalEnableRamMmu(DmxSet, i);
            __DmxHalIPPortStartStream(DmxSet, i, HI_TRUE);
        }
    }

    for ( i = 0 ; i < DmxSet->TSOPortCnt; i++ )
    {
        DMX_OsiTSOPortGetAttr(i, &TSOPortAttr);
        DMX_OsiTSOPortSetAttr(i, &TSOPortAttr);
    }

#if defined(DMX_TAGPORT_CNT) && (DMX_TAGPORT_CNT > 0)
    DMX_OsiResumeTag();
#endif

    for (i = 0; i < DmxSet->DmxCnt; i++)
    {
        DMX_DmxInfo_S  *DmxInfo = &DmxSet->DmxInfo[i];

        DmxHalSetSpsRefRecCh(DmxSet, i, 0xff);

        if (DMX_INVALID_DEMUX_ID != DmxInfo->DmxId)
        {
            DMX_OsiAttachPort(DmxInfo->DmxId, DmxInfo->PortMode, DmxInfo->PortId);
        }
    }

#ifdef DMX_DESCRAMBLER_SUPPORT
    DmxDescramblerResume(DmxSet);
#endif

    for_each_set_bit(i, DmxSet->ChnBitmap, DmxSet->DmxChanCnt)
    {
        DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[i];

        DmxSetChannel(DmxSet, i);

#ifdef DMX_DESCRAMBLER_SUPPORT
        if (ChanInfo->KeyId < DmxSet->DmxKeyCnt)
        {
            HI_U32 KeyId = ChanInfo->KeyId;

            ChanInfo->KeyId = DMX_INVALID_KEY_ID;

            DMX_OsiDescramblerAttach(ChanInfo->DmxId, KeyId, ChanInfo->ChanId);
        }
#endif
        DMX_OsiOpenChannel(ChanInfo->DmxId, i);
    }

    for_each_set_bit(i, DmxSet->FilterBitmap, DmxSet->DmxFilterCnt)
    {
        DMX_FilterInfo_S *FilterInfo = &DmxSet->DmxFilterInfo[i];
        HI_UNF_DMX_FILTER_ATTR_S FilterAttr;

        FilterAttr.u32FilterDepth = FilterInfo->Depth;

        for (j = 0; j < FilterInfo->Depth; j++)
        {
            FilterAttr.au8Match[j]  = FilterInfo->Match[j];
            FilterAttr.au8Mask[j]   = FilterInfo->Mask[j];
            FilterAttr.au8Negate[j] = FilterInfo->Negate[j];
        }

        DMX_OsiSetFilterAttr(FilterInfo->DmxId, i, &FilterAttr);

        if (DMX_INVALID_CHAN_ID != FilterInfo->ChanId)
        {
            HI_U32 ChanId = FilterInfo->ChanId;

            FilterInfo->ChanId = DMX_INVALID_CHAN_ID;

            DMX_OsiAttachFilter(FilterInfo->DmxId, i, ChanId);
        }
    }

    for_each_set_bit(i, DmxSet->RecBitmap, DmxSet->DmxRecCnt)
    {
        DMX_RecInfo_S *RecInfo = &DmxSet->DmxRecInfo[i];

        DMX_DRV_REC_StartRecChn(RecInfo->DmxId, i);
    }

    for_each_set_bit(i, DmxSet->PcrBitmap, DmxSet->DmxPcrCnt)
    {
        DMX_PCR_Info_S *PcrInfo = &DmxSet->DmxPcrInfo[i];

        DMX_OsiPcrChannelSetPid(PcrInfo->DmxId, i, PcrInfo->PcrPid);
    }

    for_each_set_bit(i, DmxSet->RmxChanBitmap, DmxSet->RmxChanCnt)
    {
        RMX_Chan_Info_S *RmxChanInfo = &DmxSet->RmxChanInfo[i];

        RMX_OsiStart(RmxChanInfo->RmxId, i);
    }

    DmxHalEnableAllDavInt(DmxSet);
    DmxHalEnableAllChEopInt(DmxSet);
    DmxHalEnableAllChEnqueInt(DmxSet);
    DmxHalFQEnableAllOverflowInt(DmxSet);
    DmxHalEnableD2BFOvflInt(DmxSet);
    DmxHalEnableD2BROvflInt(DmxSet);

    DmxHalSetFlushMaxWaitTime(DmxSet, 0x2400);
    DmxHalSetDataFakeMod(DmxSet, DMX_ENABLE);

    DmxHalEnableAllPVRInt(DmxSet);

    return HI_SUCCESS;
}

HI_S32 DMX_OsiResume(PM_BASEDEV_S *himd)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;

    if (DMX_CLUSTER_INACTIVED == DmxCluster->Ops->GetClusterState())
    {
        goto out;
    }

    DmxHalConfigHardware();

    DmxHalEnableGblMmu(&DmxCluster->mmu);

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        __DMX_OsiResume(DmxSet, himd);
    }

    DmxCluster->resumeCnt ++;

out:
    HI_PRINT("DEMUX resume OK\n");

    return HI_SUCCESS;
}

#ifdef HI_DEMUX_PROC_SUPPORT
HI_S32 DMX_OsiRamPortGetBufInfo(HI_U32 PortId, DMX_Proc_RamPort_BufInfo_S *BufInfo)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);

    if (PortInfo->PhyAddr)
    {
        BufInfo->PhyAddr        = PortInfo->PhyAddr;
        BufInfo->BufSize        = PortInfo->BufSize;
#ifdef DMX_TS_BUF_VERSION_2
        BufInfo->Read           = atomic_read(&PortInfo->LastDescReadIdx);
        BufInfo->Write          = atomic_read(&PortInfo->LastDescWriteIdx);
#else
        BufInfo->Read           = PortInfo->Read;
        BufInfo->Write          = PortInfo->Write;
#endif
        BufInfo->UsedSize       = GetQueueLenth(BufInfo->Read, BufInfo->Write, BufInfo->BufSize);
        BufInfo->GetCount       = PortInfo->GetCount;
        BufInfo->GetValidCount  = PortInfo->GetValidCount;
        BufInfo->PutCount       = PortInfo->PutCount;

        ret = HI_SUCCESS;
    }

    return ret;
}


HI_S32 DMX_OsiRamPortGetDescInfo(HI_U32 PortId, DMX_Proc_RamPort_DescInfo_S *DescInfo)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);

    if (PortInfo->PhyAddr)
    {
        HI_U32 RawPortId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

        DmxHalIPPortGetDescInfo(DmxSet, RawPortId, DescInfo);
        return HI_SUCCESS;
    }

    return ret;
}

HI_S32 DMX_OsiRamPortGetBPStatus(HI_U32 PortId, DMX_Proc_RamPort_BPStatus_S *BPStatus)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;
    DMX_RamPort_Info_S *PortInfo = HI_NULL;

    DmxSet = GetDmxSetByRamid(PortId);
    if (!DmxSet)
    {
        return HI_ERR_DMX_INVALID_PARA;
    }

    PortInfo = DmxSet->Ops->GetRamPortInfo(DmxSet, PortId);

    if (PortInfo->PhyAddr)
    {
        HI_U32 RawPortId = DmxSet->Ops->GetRamRawId(DmxSet, PortId);

        DmxHalIPPortGetBPStatus(DmxSet, RawPortId, BPStatus);
        return HI_SUCCESS;
    }

    return ret;
}

HI_S32 DMX_OsiGetDmxRecProc(Dmx_Set_S *DmxSet, HI_U32 RecId, DMX_Proc_Rec_BufInfo_S *BufInfo)
{
    HI_S32 ret = HI_FAILURE;
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];

    switch (RecInfo->RecType)
    {
        case HI_UNF_DMX_REC_TYPE_SELECT_PID :
        case HI_UNF_DMX_REC_TYPE_ALL_PID :
        {
            DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[RecInfo->RecFqId];

            BufInfo->RecType    = RecInfo->RecType;
            BufInfo->Descramed  = RecInfo->Descramed;
            BufInfo->BlockCnt   = FqInfo->FQDepth - 1;
            BufInfo->BlockSize  = FqInfo->BlockSize;
            BufInfo->RecStatus  = (DMX_REC_STATUS_START == RecInfo->RecStatus) ? 1 : 0;
            BufInfo->Overflow   = FqInfo->FqOverflowCount;

            DMXOsiOQGetReadWrite(DmxSet, RecInfo->RecOqId, &BufInfo->BufWrite, &BufInfo->BufRead);

            ret = HI_SUCCESS;
            break;
        }

        default :
            ret = HI_FAILURE;
    }

    return ret;
}

HI_S32 DMX_OsiGetDmxRecScdProc(Dmx_Set_S *DmxSet, HI_U32 RecId, DMX_Proc_RecScd_BufInfo_S *BufInfo)
{
    HI_S32 ret = HI_FAILURE;
    DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];

    memset(BufInfo, 0, sizeof(DMX_Proc_RecScd_BufInfo_S));

    switch (RecInfo->RecType)
    {
        case HI_UNF_DMX_REC_TYPE_SELECT_PID :
        {
            BufInfo->IndexType  = RecInfo->IndexType;
            BufInfo->IndexPid   = DMX_INVALID_PID;

            if (HI_UNF_DMX_REC_INDEX_TYPE_NONE != RecInfo->IndexType)
            {
                DMX_FQ_Info_S *FqInfo = &DmxSet->DmxFqInfo[RecInfo->ScdFqId];

                BufInfo->FqId          = RecInfo->ScdFqId;
                BufInfo->OqId         = RecInfo->ScdOqId;
                BufInfo->ScdId        = RecInfo->ScdId;
                BufInfo->IndexPid   = RecInfo->IndexPid;
                BufInfo->BlockCnt   = FqInfo->FQDepth - 1;
                BufInfo->BlockSize  = FqInfo->BlockSize;
                BufInfo->Overflow   = FqInfo->FqOverflowCount;

                DMXOsiOQGetReadWrite(DmxSet, RecInfo->ScdOqId, &BufInfo->BufWrite, &BufInfo->BufRead);
            }

            ret = HI_SUCCESS;
            break;
        }

        case HI_UNF_DMX_REC_TYPE_ALL_PID :
        {
            BufInfo->IndexType  = HI_UNF_DMX_REC_INDEX_TYPE_NONE;
            BufInfo->IndexPid   = DMX_INVALID_PID;

            ret = HI_SUCCESS;
            break;
        }

        default :
            ret = HI_FAILURE;
    }

    return ret;
}

HI_S32 DMX_OsiGetChannelDescStatus(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_Proc_ChanDesc_S *DescInfo)
{
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    DmxHalGetChannelDescStatus(DmxSet, ChanId, &DescInfo->DoScram, &DescInfo->KeyId);

    if (HI_UNF_DMX_SECURE_MODE_NONE == ChanInfo->ChanSecure && HI_TRUE == DescInfo->DoScram)
    {
        BUG_ON(ChanInfo->KeyId != DescInfo->KeyId);
    }

    return HI_SUCCESS;
}

HI_S32 DMX_OsiGetChanBufProc(Dmx_Set_S *DmxSet, HI_U32 ChanId, DMX_Proc_ChanBuf_S *BufInfo)
{
    HI_S32 ret = HI_FAILURE;
    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];

    memset(BufInfo, 0, sizeof(DMX_Proc_ChanBuf_S));

    if (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY == (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & ChanInfo->ChanOutMode))
    {
        DMX_OQ_Info_S  *OqInfo  = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];
        DMX_FQ_Info_S  *FqInfo  = &DmxSet->DmxFqInfo[OqInfo->u32FQId];

        DMXOsiOQGetReadWrite(DmxSet, ChanInfo->ChanOqId, &BufInfo->DescWrite, &BufInfo->DescRead);

        BufInfo->DescDepth  = OqInfo->OQDepth;
        BufInfo->BlockSize  = FqInfo->BlockSize;
        BufInfo->Overflow   = FqInfo->FqOverflowCount;

        ret = HI_SUCCESS;
    }

    return ret;
}

HI_S32 DMX_OsiGetDmxClkProc(HI_U32 *DmxClk)
{
    *DmxClk = DmxHalGetDmxClk(GetDefDmxSet());

    return HI_SUCCESS;
}

HI_S32 DMX_OsiSaveDmxTs_Start(HI_U32 u32RecId, HI_U32 DmxId, HI_U32 u32RecDmxId)
{
    Dmx_Set_S *DmxSet = GetDmxSetByDmxid(DmxId);
    DMX_ChanInfo_S     *ChanInfo    = DmxSet->DmxChanInfo;
    HI_U32              ChanId = DMX_INVALID_CHAN_ID;
    HI_U32              i;

    for (i = 0; i < DmxSet->DmxChanCnt; i++)
    {
        if ((ChanInfo[i].DmxId == DmxId) && (HI_UNF_DMX_CHAN_CLOSE != ChanInfo[i].ChanStatus))
        {
            DMX_DRV_REC_AddRecPid(u32RecDmxId, u32RecId, ChanInfo[i].ChanPid, &ChanId);

#ifdef DMX_DESCRAMBLER_SUPPORT
            if (DMX_INVALID_KEY_ID != ChanInfo[i].KeyId && DMX_INVALID_CHAN_ID != ChanId)
            {
                DMX_OsiDescramblerAttach(u32RecDmxId, ChanInfo[i].KeyId, ChanId);
            }
#endif
        }
    }

    return 0;
}

/*******************************************************************/
#endif

#if defined(DMX_TAGPORT_CNT) && (DMX_TAGPORT_CNT > 0)
HI_S32  DMX_OsiTagPortGetAttr(const HI_U32 PortId, HI_UNF_DMX_TAG_ATTR_S *pstAttr)
{
    DMX_TagPort_Info_S * TagPortInfo = &g_pDmxDevOsi->TagPortInfo;
    HI_U32 *pu32Low, *pu32Mid, *pu32High, *pu32CurLow, *pu32CurMid, *pu32CurHigh;

    pu32Low = (HI_U32 *)&(pstAttr->au8Tag[0]);                   /* 0~4 bytes */
    pu32Mid = (HI_U32 *)&(pstAttr->au8Tag[4]);                /* 4~8 bytes */
    pu32High = (HI_U32 *)&(pstAttr->au8Tag[4 + 4]);         /* 8~12 bytes*/

    pu32CurLow = (HI_U32 *)&(TagPortInfo->TagPortAttrs[PortId].au8Tag[0]);                   /* 0~4 bytes */
    pu32CurMid = (HI_U32 *)&(TagPortInfo->TagPortAttrs[PortId].au8Tag[4]);                /* 4~8 bytes */
    pu32CurHigh = (HI_U32 *)&(TagPortInfo->TagPortAttrs[PortId].au8Tag[4 + 4]);         /* 8~12 bytes*/

    *pu32Low = *pu32CurLow;
    *pu32Mid = *pu32CurMid;
    *pu32High = *pu32CurHigh;

    pstAttr->enSyncMod = TagPortInfo->enSyncMod;
    pstAttr->u32TagLen = TagPortInfo->u32TagLen;
    pstAttr->enTSSource = TagPortInfo->enTSSource;

    return HI_SUCCESS;

}

HI_S32  DMX_OsiTagPortSetAttr(const HI_U32 PortId, const HI_UNF_DMX_TAG_ATTR_S *pstAttr)
{
    DMX_TagPort_Info_S * TagPortInfo = &g_pDmxDevOsi->TagPortInfo;
    HI_U32 *pu32Low, *pu32Mid, *pu32High, *pu32NewLow, *pu32NewMid, *pu32NewHigh;
    HI_S32 s32Ret = HI_FAILURE;

    if (TagPortInfo->u32TagLen != pstAttr->u32TagLen || TagPortInfo->enSyncMod != pstAttr->enSyncMod ||
        TagPortInfo->enTSSource != pstAttr->enTSSource)
    {
        HI_DBG_DEMUX("Global Tag Port config will change from(TSSource:%d, TagLen:%d, SyncMod:%d) to (TSSource:%d, TagLen:%d, SyncMod:%d).\n",
            TagPortInfo->enTSSource, TagPortInfo->u32TagLen, TagPortInfo->enSyncMod,
            pstAttr->enTSSource, pstAttr->u32TagLen, pstAttr->enSyncMod);
    }

    pu32NewLow = (HI_U32 *)&(pstAttr->au8Tag[0]);                   /* 0~4 bytes */
    pu32NewMid = (HI_U32 *)&(pstAttr->au8Tag[4]);                /* 4~8 bytes */
    pu32NewHigh = (HI_U32 *)&(pstAttr->au8Tag[4 + 4]);         /* 8~12 bytes*/

    switch(pstAttr->u32TagLen)
    {
        case 4:  /* 4 bytes mode */
        {
            if (0 == *pu32NewLow)  /* 0 is reserve unused tag value is */
            {
                s32Ret = HI_ERR_DMX_INVALID_PARA;
                goto out;
            }

            pu32Low = (HI_U32 *)&(TagPortInfo->TagPortAttrs[PortId].au8Tag[0]);                   /* 0~4 bytes */
            pu32Mid = (HI_U32 *)&(TagPortInfo->TagPortAttrs[PortId].au8Tag[4]);                /* 4~8 bytes */
            pu32High = (HI_U32 *)&(TagPortInfo->TagPortAttrs[PortId].au8Tag[4 + 4]);         /* 8~12 bytes*/

            *pu32Low = *pu32NewLow;
            *pu32Mid = 0;
            *pu32High = 0;

            DmxHalSetTagDealAttr(PortId, *pu32Low, *pu32Mid, *pu32High);

            TagPortInfo->u32TagLen = pstAttr->u32TagLen;
            TagPortInfo->enSyncMod = pstAttr->enSyncMod;
            TagPortInfo->enTSSource = pstAttr->enTSSource;

            s32Ret = DmxHalSetTagDealCtl(HI_TRUE, TagPortInfo->enTSSource, TagPortInfo->enSyncMod, TagPortInfo->u32TagLen);
            if (HI_SUCCESS != s32Ret)
            {
                goto out;
            }

            break;
        }

        default:
        {
            HI_DBG_DEMUX("Not support %dbytes mode now.\n", pstAttr->u32TagLen);

            s32Ret = HI_ERR_DMX_INVALID_PARA;
            goto out;
        }
    }

    s32Ret = HI_SUCCESS;

out:
    return s32Ret;
}

HI_S32 DMX_OsiGetTagPortAttr(const HI_U32 TagPortId, HI_UNF_DMX_TAG_ATTR_S *pstAttr)
{
    DMX_TagPort_Info_S * TagPortInfo = &g_pDmxDevOsi ->TagPortInfo;

    if (CHECKTAGPORTID(TagPortId))
    {
        pstAttr->enTSSource = TagPortInfo->enTSSource;
        pstAttr->enSyncMod = TagPortInfo->enSyncMod;
        pstAttr->u32TagLen = TagPortInfo->u32TagLen;
        memcpy(pstAttr->au8Tag, TagPortInfo->TagPortAttrs[TagPortId].au8Tag, sizeof(pstAttr->au8Tag));
    }

    return HI_SUCCESS;
}

HI_VOID DMX_OsiResumeTag(HI_VOID)
{
    HI_UNF_DMX_TAG_ATTR_S attr;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 index;

    for ( index = 0 ; index < DMX_TAGPORT_CNT ; index++ )
    {
        DMX_OsiTagPortGetAttr(index, &attr);

        s32Ret = DMX_OsiTagPortSetAttr(index, &attr);
        if (HI_SUCCESS != s32Ret)
        {
            HI_DBG_DEMUX("Tag port(%d) reset failed(%d) after resume.\n", index, s32Ret);
        }
    }
}

#endif

#ifdef RMX_SUPPORT
static HI_S32 RmxTrimAttrs(const HI_MPI_RMX_ATTR_S *InChanAttr, HI_MPI_RMX_ATTR_S *OutChanAttr)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();

    if (!(InChanAttr->enOutPortId >= HI_MPI_DMX_PORT_RMX_0 && InChanAttr->enOutPortId < HI_MPI_DMX_PORT_RMX_0 + DmxCluster->Ops->GetRmxNum()))
    {
        HI_ERR_DEMUX("invalid remux out port id(%d).\n", InChanAttr->enOutPortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    memcpy(OutChanAttr, InChanAttr, sizeof(HI_MPI_RMX_ATTR_S));

    return HI_SUCCESS;
}

static HI_S32 RmxChanAcquire(HI_U32 RmxId, HI_MPI_RMX_ATTR_S *Attr, HI_U32 *ChanId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByRmxid(RmxId);
    RMX_Chan_Info_S *RmxChanInfo = HI_NULL;
    unsigned long mask, *p;
    HI_U32 index;

    BUG_ON(!mutex_is_locked(&DmxSet->LockAllRmx));

    *ChanId = DmxSet->Ops->GetRmxRawId(DmxSet, RmxId);

    RmxChanInfo = &DmxSet->RmxChanInfo[*ChanId];

    mutex_lock(&RmxChanInfo->LockRmx);

    mask = BIT_MASK(*ChanId);
    p = ((unsigned long*)DmxSet->RmxChanBitmap) + BIT_WORD(*ChanId);
    if (*p & mask)
    {
        ret = HI_ERR_DMX_BUSY;
        goto out;
    }

    RmxChanInfo->RmxId = RmxId;
    RmxChanInfo->Status = RMX_STATUS_STOP;
    RmxChanInfo->Owner = task_tgid_nr(current);
    bitmap_zero(RmxChanInfo->RmxPumpBitmap, DmxSet->RmxPumpCnt);
    RmxChanInfo->RmxPortCnt = sizeof(RmxChanInfo->RmxPortInfo) /sizeof(RMX_Port_Info_S);
    bitmap_zero(RmxChanInfo->RmxPortBitmap, RmxChanInfo->RmxPortCnt);

    for (index = 0; index < RmxChanInfo->RmxPortCnt; index ++)
    {
        RMX_Port_Info_S *RmxPortInfo = &RmxChanInfo->RmxPortInfo[index];

        RmxPortInfo->PortId = HI_UNF_DMX_PORT_BUTT;
        RmxPortInfo->Ref = 0;
        RmxPortInfo->OverflowCount = 0;
    }

    set_bit(*ChanId, DmxSet->RmxChanBitmap);

    ret = HI_SUCCESS;

out:
    mutex_unlock(&RmxChanInfo->LockRmx);
    return ret;
}

static HI_S32 RmxChanRelease(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    RMX_Chan_Info_S * RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];

    BUG_ON(ChanId >= DmxSet->RmxChanCnt);
    BUG_ON(!mutex_is_locked(&RmxChanInfo->LockRmx));

    clear_bit(ChanId, DmxSet->RmxChanBitmap);

    return HI_SUCCESS;
}

HI_S32 RmxGetChnInstance(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    unsigned long mask, *p;
    RMX_Chan_Info_S  *RmxChanInfo = HI_NULL;

    if (unlikely(!DmxSet))
    {
        HI_ERR_DEMUX("Invalid DemuxSet.\n");
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    if (unlikely(ChanId >= DmxSet->RmxChanCnt))
    {
        HI_ERR_DEMUX("Invalid rmx id :0x%x\n", ChanId);

        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];

    if (unlikely(0 != mutex_lock_interruptible(&RmxChanInfo->LockRmx)))
    {
        HI_WARN_DEMUX("Rmx mutex interruptibled.\n");

        ret = HI_ERR_DMX_BUSY_REC;
        goto out0;
    }

    mask = BIT_MASK(ChanId);
    p = ((unsigned long*)DmxSet->RmxChanBitmap) + BIT_WORD(ChanId);
    if (unlikely(!(*p & mask)))
    {
        HI_DBG_DEMUX("This Rmx (%d) instance has not alloced.\n", ChanId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    return HI_SUCCESS;

out1:
    mutex_unlock(&RmxChanInfo->LockRmx);
out0:
    return ret;
}

HI_VOID RmxPutChnInstance(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    RMX_Chan_Info_S  *RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];

    BUG_ON(ChanId >= DmxSet->RmxChanCnt);

    mutex_unlock(&RmxChanInfo->LockRmx);
}

HI_S32 RMX_OsiCreate(HI_MPI_RMX_ATTR_S *Attr, HI_U32 *ChanId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = HI_NULL;
    HI_MPI_RMX_ATTR_S ValidAttr = {0};
    HI_U32 RmxId;

    ret = RmxTrimAttrs(Attr, &ValidAttr);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    RmxId = ValidAttr.enOutPortId - HI_MPI_DMX_PORT_RMX_0;

    DmxSet = GetDmxSetByRmxid(RmxId);
    if (!DmxSet)
    {
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    mutex_lock(&DmxSet->LockAllRmx);

    ret = RmxChanAcquire(RmxId, &ValidAttr, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

out1:
    mutex_unlock(&DmxSet->LockAllRmx);
out0:
    return ret;
}

HI_S32 RMX_OsiDestroy(HI_U32 RmxId, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByRmxid(RmxId);
    RMX_Chan_Info_S  *RmxChanInfo = HI_NULL;
    HI_U32 index;

    ret = RmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = __RMX_OsiStop(RmxId, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];

    for_each_set_bit(index, RmxChanInfo->RmxPumpBitmap, DmxSet->RmxPumpCnt)
    {
        ret = __RMX_OsiDelPump(RmxId, ChanId, index);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_DEMUX("del pump(%d) failed.\n", index);
            goto out1;
        }

        clear_bit(index, RmxChanInfo->RmxPumpBitmap);
    }

    ret = RmxChanRelease(DmxSet, ChanId);

out1:
    RmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_S32 RMX_OsiGetAttr(HI_U32 RmxId, HI_U32 ChanId, HI_MPI_RMX_ATTR_S *Attr)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByRmxid(RmxId);

    ret = RmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    Attr->enOutPortId = RmxId + HI_MPI_DMX_PORT_RMX_0;

    ret = HI_SUCCESS;

    RmxPutChnInstance(DmxSet, ChanId);
out:
    return ret;
}

HI_S32 RMX_OsiSetAttr(HI_U32 RmxId, HI_U32 ChanId, HI_MPI_RMX_ATTR_S *Attr)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByRmxid(RmxId);

    ret = RmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    HI_ERR_DEMUX("dont support modify remux attr now.\n");
    ret = HI_ERR_DMX_NOT_SUPPORT;

    RmxPutChnInstance(DmxSet, ChanId);
out:
    return ret;
}

static HI_S32 RmxStartProc(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    RMX_Chan_Info_S  *RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];
    HI_U32 index;

    for_each_set_bit(index, RmxChanInfo->RmxPumpBitmap, DmxSet->RmxPumpCnt)
    {
        RMX_Pump_Info_S *RmxPumpInfo = &DmxSet->RmxPumpInfo[index];

        BUG_ON(RmxPumpInfo->RmxPortId >= RmxChanInfo->RmxPortCnt);

        switch(RmxPumpInfo->Type)
        {
            case HI_MPI_RMX_PUMP_TYPE_PID:
                DmxHalSetRemuxPidTab(DmxSet, index, RmxPumpInfo->RmxPortId, RmxPumpInfo->Pid);
                break;

            case HI_MPI_RMX_PUMP_TYPE_REMAP_PID:
                DmxHalSetRemuxRemapPidTab(DmxSet, index, RmxPumpInfo->RmxPortId, RmxPumpInfo->Pid, RmxPumpInfo->RemapPid);
                break;

            case HI_MPI_RMX_PUMP_TYPE_ALLPASS_PORT:
                DmxHalSetRemuxPortAllPass(DmxSet, RmxPumpInfo->RmxPortId);
                break;

            default:
                BUG();
        }
    }

    for_each_set_bit(index, RmxChanInfo->RmxPortBitmap, RmxChanInfo->RmxPortCnt)
    {
        RMX_Port_Info_S *RmxPortInfo = &RmxChanInfo->RmxPortInfo[index];

        BUG_ON(0 == RmxPortInfo->Ref);

        DmxHalEnDetectRemuxPort(DmxSet, index);

        if (RmxPortInfo->PortId < HI_UNF_DMX_PORT_IF_0 + DmxCluster->Ops->GetIFPortNum())
        {
            DmxHalSetRemuxPort(DmxSet, ChanId, index, DMX_PORT_MODE_TUNER, RmxPortInfo->PortId - HI_UNF_DMX_PORT_IF_0);
        }
        else if (RmxPortInfo->PortId >= HI_UNF_DMX_PORT_TSI_0 && RmxPortInfo->PortId < HI_UNF_DMX_PORT_TSI_0 + DmxCluster->Ops->GetTSIPortNum())
        {
            DmxHalSetRemuxPort(DmxSet, ChanId, index, DMX_PORT_MODE_TUNER, RmxPortInfo->PortId - HI_UNF_DMX_PORT_TSI_0);
        }
        else if (RmxPortInfo->PortId >= HI_UNF_DMX_PORT_RAM_0 && RmxPortInfo->PortId < HI_UNF_DMX_PORT_RAM_0 + DmxCluster->Ops->GetRamPortNum())
        {
            DmxHalSetRemuxPort(DmxSet, ChanId, index, DMX_PORT_MODE_RAM, RmxPortInfo->PortId - HI_UNF_DMX_PORT_RAM_0);
        }
        else
        {
            BUG();
        }
    }

    return HI_SUCCESS;
}

static HI_S32 RmxStopProc(Dmx_Set_S *DmxSet, HI_U32 ChanId)
{
    RMX_Chan_Info_S  *RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];
    HI_U32 index;

    DmxHalUnsetRemuxPort(DmxSet, ChanId);

    for_each_set_bit(index, RmxChanInfo->RmxPortBitmap, RmxChanInfo->RmxPortCnt)
    {
        DmxHalDisDetectRemuxPort(DmxSet, index);
    }

    for_each_set_bit(index, RmxChanInfo->RmxPumpBitmap, DmxSet->RmxPumpCnt)
    {
        RMX_Pump_Info_S *RmxPumpInfo = &DmxSet->RmxPumpInfo[index];

        BUG_ON(RmxPumpInfo->RmxPortId >= RmxChanInfo->RmxPortCnt);

        switch(RmxPumpInfo->Type)
        {
            case HI_MPI_RMX_PUMP_TYPE_PID:
            case HI_MPI_RMX_PUMP_TYPE_REMAP_PID:
                DmxHalUnSetRemuxPidTab(DmxSet, index);
                break;

            case HI_MPI_RMX_PUMP_TYPE_ALLPASS_PORT:
                DmxHalUnSetRemuxPortAllPass(DmxSet, RmxPumpInfo->RmxPortId);
                break;

            default:
                BUG();
        }
    }

    return HI_SUCCESS;
}

HI_S32 RMX_OsiStart(HI_U32 RmxId, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByRmxid(RmxId);
    RMX_Chan_Info_S  *RmxChanInfo = HI_NULL;

    ret = RmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];

    if (unlikely(RMX_STATUS_START == RmxChanInfo->Status))
    {
        ret = HI_SUCCESS;
        goto out1;
    }

    ret = RmxStartProc(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    RmxChanInfo->Status = RMX_STATUS_START;

out1:
    RmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_S32 __RMX_OsiStop(HI_U32 RmxId, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByRmxid(RmxId);
    RMX_Chan_Info_S  *RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];

    BUG_ON(!mutex_is_locked(&RmxChanInfo->LockRmx));

    if (unlikely(RMX_STATUS_STOP == RmxChanInfo->Status))
    {
        ret = HI_SUCCESS;
        goto out;
    }

    ret = RmxStopProc(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    RmxChanInfo->Status = RMX_STATUS_STOP;

out:
    return ret;
}

HI_S32 RMX_OsiStop(HI_U32 RmxId, HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByRmxid(RmxId);

    ret = RmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret =  __RMX_OsiStop(RmxId, ChanId);

    RmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

static HI_S32 RmxTrimPumpAttrs(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_MPI_RMX_PUMP_ATTR_S *Attr)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    RMX_Chan_Info_S *RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];
    HI_U32 index;

    BUG_ON(!mutex_is_locked(&RmxChanInfo->LockRmx));

    if (Attr->enPumpType >= HI_MPI_RMX_PUMP_TYPE_BUTT)
    {
        HI_ERR_DEMUX("invalid pump type(%d).\n", Attr->enPumpType);
        return HI_ERR_DMX_INVALID_PARA;
    }

    if ( !(Attr->enInPortId < HI_UNF_DMX_PORT_IF_0 + DmxCluster->Ops->GetIFPortNum()) &&
        !(Attr->enInPortId >= HI_UNF_DMX_PORT_TSI_0 && Attr->enInPortId < HI_UNF_DMX_PORT_TSI_0 + DmxCluster->Ops->GetTSIPortNum()) &&
        !(Attr->enInPortId >= HI_UNF_DMX_PORT_RAM_0 && Attr->enInPortId < HI_UNF_DMX_PORT_RAM_0 + DmxCluster->Ops->GetRamPortNum()) )
    {
        HI_ERR_DEMUX("invalid port id(%d).\n", Attr->enInPortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (HI_MPI_RMX_PUMP_TYPE_PID == Attr->enPumpType && Attr->u32Pid > DMX_INVALID_PID)
    {
        HI_ERR_DEMUX("invalid pid(%d).\n", Attr->u32Pid);
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (HI_MPI_RMX_PUMP_TYPE_REMAP_PID == Attr->enPumpType && (Attr->u32Pid > DMX_INVALID_PID ||Attr->u32RemapPid > DMX_INVALID_PID ))
    {
        HI_ERR_DEMUX("invalid pid(%d) or remap pid(%d).\n", Attr->u32Pid, Attr->u32RemapPid);
        return HI_ERR_DMX_INVALID_PARA;
    }

    for_each_set_bit(index, RmxChanInfo->RmxPumpBitmap, DmxSet->RmxPumpCnt)
    {
        RMX_Pump_Info_S *RmxPumpInfo = &DmxSet->RmxPumpInfo[index];
        RMX_Port_Info_S *RmxPortInfo = &RmxChanInfo->RmxPortInfo[RmxPumpInfo->RmxPortId];

        BUG_ON(0 == RmxPortInfo->Ref);

        if (Attr->enInPortId == RmxPortInfo->PortId)
        {
            if (HI_MPI_RMX_PUMP_TYPE_ALLPASS_PORT == RmxPumpInfo->Type || HI_MPI_RMX_PUMP_TYPE_ALLPASS_PORT == Attr->enPumpType)
            {
                HI_ERR_DEMUX("add conflict pump failed.\n");
                return HI_ERR_DMX_INVALID_PARA;
            }

            if (Attr->u32Pid  == RmxPumpInfo->Pid)
            {
                HI_ERR_DEMUX("add repeat pump failed.\n");
                return HI_ERR_DMX_INVALID_PARA;
            }
        }
    }

    return HI_SUCCESS;
}

static HI_S32 RmxPortAcquire(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_MPI_RMX_PUMP_ATTR_S *Attr, HI_U32 *RmxPortId)
{
    HI_S32 ret = HI_ERR_DMX_NOFREE_CHAN;
    RMX_Chan_Info_S *RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];
    HI_U32 index;

    BUG_ON(!mutex_is_locked(&RmxChanInfo->LockRmx));

    /* reused */
    for_each_set_bit(index, RmxChanInfo->RmxPortBitmap, RmxChanInfo->RmxPortCnt)
    {
        RMX_Port_Info_S *RmxPortInfo = &RmxChanInfo->RmxPortInfo[index];

        BUG_ON(0 == RmxPortInfo->Ref);

        if (Attr->enInPortId == RmxPortInfo->PortId)
        {
            RmxPortInfo->Ref ++;

            *RmxPortId = index;

            ret = HI_SUCCESS;
            break;
        }
    }

    /* new */
    if (HI_SUCCESS != ret)
    {
        index = find_first_zero_bit(RmxChanInfo->RmxPortBitmap, RmxChanInfo->RmxPortCnt);
        if (index < RmxChanInfo->RmxPortCnt)
        {
            RMX_Port_Info_S *RmxPortInfo = &RmxChanInfo->RmxPortInfo[index];

            BUG_ON(0 != RmxPortInfo->Ref);

            RmxPortInfo->PortId = Attr->enInPortId;
            RmxPortInfo->Ref = 1;

            *RmxPortId = index;
            set_bit(index, RmxChanInfo->RmxPortBitmap);

            ret = HI_SUCCESS;
        }
    }

    return ret ;
}

static HI_S32 RmxPortRelease(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 RmxPortId)
{
    RMX_Chan_Info_S *RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];
    RMX_Port_Info_S *RmxPortInfo = &RmxChanInfo->RmxPortInfo[RmxPortId];

    BUG_ON(RmxPortId >= RmxChanInfo->RmxPortCnt);
    BUG_ON(!mutex_is_locked(&RmxChanInfo->LockRmx));

    RmxPortInfo->Ref --;

    if (0 == RmxPortInfo->Ref)
    {
        clear_bit(RmxPortId, RmxChanInfo->RmxPortBitmap);
    }

    return HI_SUCCESS;
}

static HI_S32 RmxPumpAcquire(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_MPI_RMX_PUMP_ATTR_S *Attr, HI_U32 *PumpId)
{
    HI_S32 ret = HI_FAILURE;
    RMX_Pump_Info_S *RmxPumpInfo = HI_NULL;

    BUG_ON(!mutex_is_locked(&DmxSet->LockAllPump));

    *PumpId = find_first_zero_bit(DmxSet->RmxPumpBitmap, DmxSet->RmxPumpCnt);
    if (!(*PumpId < DmxSet->RmxPumpCnt))
    {
        HI_ERR_DEMUX("There is no available remux pump now!\n");
        ret = HI_ERR_DMX_NOFREE_CHAN;
        goto out0;
    }

    RmxPumpInfo = &DmxSet->RmxPumpInfo[*PumpId];

    mutex_lock(&RmxPumpInfo->LockPump);

    BUG_ON(HI_MPI_RMX_PUMP_TYPE_BUTT != RmxPumpInfo->Type);

    ret = RmxPortAcquire(DmxSet, ChanId, Attr, &RmxPumpInfo->RmxPortId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("There is no available remux port now!\n");
        goto out1;
    }

    RmxPumpInfo->Type = Attr->enPumpType;
    RmxPumpInfo->Pid = Attr->u32Pid;
    RmxPumpInfo->RemapPid = Attr->u32RemapPid;

    set_bit(*PumpId, DmxSet->RmxPumpBitmap);

out1:
    mutex_unlock(&RmxPumpInfo->LockPump);
out0:
    return ret;
}

static HI_S32 RmxPumpRelease(Dmx_Set_S *DmxSet, HI_U32 ChanId, HI_U32 PumpId)
{
    RMX_Pump_Info_S *RmxPumpInfo = &DmxSet->RmxPumpInfo[PumpId];

    BUG_ON(PumpId >= DmxSet->RmxPumpCnt);
    BUG_ON(!mutex_is_locked(&RmxPumpInfo->LockPump));

    RmxPortRelease(DmxSet, ChanId, RmxPumpInfo->RmxPortId);

    RmxPumpInfo->Type = HI_MPI_RMX_PUMP_TYPE_BUTT;

    clear_bit(PumpId, DmxSet->RmxPumpBitmap);

    return HI_SUCCESS;
}

HI_S32 RmxGetPumpInstance(Dmx_Set_S *DmxSet, HI_U32 PumpId)
{
    HI_S32 ret = HI_FAILURE;
    unsigned long mask, *p;
    RMX_Pump_Info_S *RmxPumpInfo = HI_NULL;

    if (unlikely(!DmxSet))
    {
        HI_ERR_DEMUX("Invalid DemuxSet.\n");
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    if (unlikely(PumpId >= DmxSet->RmxPumpCnt))
    {
        HI_ERR_DEMUX("Invalid channel id :0x%x\n", PumpId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out0;
    }

    RmxPumpInfo = &DmxSet->RmxPumpInfo[PumpId];

    if (unlikely(0 != mutex_lock_interruptible(&RmxPumpInfo->LockPump)))
    {
        HI_WARN_DEMUX("Remux pump mutex interruptibled.\n");

        ret = HI_ERR_DMX_BUSY_CHAN;
        goto out0;
    }

    mask = BIT_MASK(PumpId);
    p = ((unsigned long*)DmxSet->RmxPumpBitmap) + BIT_WORD(PumpId);
    if (unlikely(!(*p & mask)))
    {
        HI_DBG_DEMUX("This remux pump(%d) instance has not alloced.\n", PumpId);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out1;
    }

    return HI_SUCCESS;

out1:
    mutex_unlock(&RmxPumpInfo->LockPump);
out0:
    return ret;
}

HI_VOID RmxPutPumpInstance(Dmx_Set_S *DmxSet, HI_U32 PumpId)
{
    RMX_Pump_Info_S *RmxPumpInfo = &DmxSet->RmxPumpInfo[PumpId];

    BUG_ON(PumpId >= DmxSet->RmxPumpCnt);

    mutex_unlock(&RmxPumpInfo->LockPump);
}

HI_S32 RMX_OsiAddPump(HI_U32 RmxId, HI_U32 ChanId, HI_MPI_RMX_PUMP_ATTR_S *Attr, HI_U32 *PumpId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByRmxid(RmxId);
    RMX_Chan_Info_S  *RmxChanInfo = HI_NULL;

    ret = RmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];

    ret = RmxTrimPumpAttrs(DmxSet, ChanId, Attr);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    if (unlikely(RMX_STATUS_START == RmxChanInfo->Status))
    {
        HI_ERR_DEMUX("add pump failed when remux started.\n");
        ret = HI_ERR_DMX_BUSY;
        goto out1;
    }

    mutex_lock(&DmxSet->LockAllPump);

    ret = RmxPumpAcquire(DmxSet, ChanId, Attr, PumpId);
    if (HI_SUCCESS != ret)
    {
        goto out2;
    }

    set_bit(*PumpId, RmxChanInfo->RmxPumpBitmap);

out2:
    mutex_unlock(&DmxSet->LockAllPump);
out1:
    RmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_S32 __RMX_OsiDelPump(HI_U32 RmxId, HI_U32 ChanId, HI_U32 PumpId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByRmxid(RmxId);
    RMX_Chan_Info_S  *RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];

    BUG_ON(!mutex_is_locked(&RmxChanInfo->LockRmx));

    if (unlikely(RMX_STATUS_START == RmxChanInfo->Status))
    {
        HI_ERR_DEMUX("del pump failed when remux started.\n");
        ret = HI_ERR_DMX_BUSY;
        goto out;
    }

    ret = RmxGetPumpInstance(DmxSet, PumpId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    ret = RmxPumpRelease(DmxSet, ChanId, PumpId);

    RmxPutPumpInstance(DmxSet, PumpId);
out:
    return ret;
}

HI_S32 RMX_OsiDelPump(HI_U32 RmxId, HI_U32 ChanId, HI_U32 PumpId)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByRmxid(RmxId);
    RMX_Chan_Info_S  *RmxChanInfo = HI_NULL;

    ret = RmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];

    ret = __RMX_OsiDelPump(RmxId, ChanId, PumpId);
    if (HI_SUCCESS == ret)
    {
        clear_bit(PumpId, RmxChanInfo->RmxPumpBitmap);
    }

    RmxPutChnInstance(DmxSet, ChanId);
out:
    return ret;
}

HI_S32 RMX_OsiGetPumpDefaultAttr(HI_MPI_RMX_PUMP_ATTR_S *Attr)
{
    Attr->enPumpType = HI_MPI_RMX_PUMP_TYPE_BUTT;
    Attr->enInPortId = HI_UNF_DMX_PORT_BUTT;
    Attr->u32Pid = DMX_INVALID_PID;
    Attr->u32RemapPid = DMX_INVALID_PID;

    return HI_SUCCESS;
}

HI_S32 RMX_OsiGetPumpAttr(HI_U32 RmxId, HI_U32 ChanId, HI_U32 PumpId, HI_MPI_RMX_PUMP_ATTR_S *Attr)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByRmxid(RmxId);
    RMX_Chan_Info_S  *RmxChanInfo = HI_NULL;
    RMX_Pump_Info_S *RmxPumpInfo = HI_NULL;
    RMX_Port_Info_S *RmxPortInfo = HI_NULL;

    ret = RmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = RmxGetPumpInstance(DmxSet, PumpId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    RmxChanInfo = &DmxSet->RmxChanInfo[ChanId];
    RmxPumpInfo = &DmxSet->RmxPumpInfo[PumpId];
    RmxPortInfo = &RmxChanInfo->RmxPortInfo[RmxPumpInfo->RmxPortId];

    Attr->enPumpType = RmxPumpInfo->Type;
    Attr->enInPortId = RmxPortInfo->PortId;
    Attr->u32Pid = RmxPumpInfo->Pid;
    Attr->u32RemapPid = RmxPumpInfo->RemapPid;

    RmxPutPumpInstance(DmxSet, PumpId);
out1:
    RmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_S32 RMX_OsiSetPumpAttr(HI_U32 RmxId, HI_U32 ChanId, HI_U32 PumpId, HI_MPI_RMX_PUMP_ATTR_S *Attr)
{
    HI_S32 ret = HI_FAILURE;
    Dmx_Set_S *DmxSet = GetDmxSetByRmxid(RmxId);

    ret = RmxGetChnInstance(DmxSet, ChanId);
    if (HI_SUCCESS != ret)
    {
        goto out0;
    }

    ret = RmxGetPumpInstance(DmxSet, PumpId);
    if (HI_SUCCESS != ret)
    {
        goto out1;
    }

    HI_ERR_DEMUX("dont support modify remux pump attr now.\n");
    ret = HI_ERR_DMX_NOT_SUPPORT;

    RmxPutPumpInstance(DmxSet, PumpId);
out1:
    RmxPutChnInstance(DmxSet, ChanId);
out0:
    return ret;
}

HI_VOID RmxEnFqBPIp(Dmx_Set_S *DmxSet, HI_U32 RmxId, HI_U32 FqId)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    HI_U32 RmxChanId = DmxSet->Ops->GetRmxRawId(DmxSet, RmxId);

    BUG_ON(RmxId >= DmxCluster->Ops->GetRmxNum());

    if (HI_SUCCESS == RmxGetChnInstance(DmxSet, RmxChanId))
    {
        RMX_Chan_Info_S *RmxChanInfo = &DmxSet->RmxChanInfo[RmxChanId];
        HI_U32 RmxPortId;

        for_each_set_bit(RmxPortId, RmxChanInfo->RmxPortBitmap, RmxChanInfo->RmxPortCnt)
        {
            RMX_Port_Info_S *RmxPortInfo = &RmxChanInfo->RmxPortInfo[RmxPortId];

            if (RmxPortInfo->PortId >= HI_UNF_DMX_PORT_RAM_0 && RmxPortInfo->PortId < HI_UNF_DMX_PORT_RAM_0 + DmxCluster->Ops->GetRamPortNum())
            {
                DmxHalAttachIPBPFQ(DmxSet, RmxPortInfo->PortId - HI_UNF_DMX_PORT_RAM_0, FqId);
            }
        }

        RmxPutChnInstance(DmxSet, RmxChanId);
    }
}

HI_VOID RmxDisFqBPIp(Dmx_Set_S *DmxSet, HI_U32 RmxId, HI_U32 FqId)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    HI_U32 RmxChanId = DmxSet->Ops->GetRmxRawId(DmxSet, RmxId);

    BUG_ON(RmxId >= DmxCluster->Ops->GetRmxNum());

    if (HI_SUCCESS == RmxGetChnInstance(DmxSet, RmxChanId))
    {
        RMX_Chan_Info_S *RmxChanInfo = &DmxSet->RmxChanInfo[RmxChanId];
        HI_U32 RmxPortId;

        for_each_set_bit(RmxPortId, RmxChanInfo->RmxPortBitmap, RmxChanInfo->RmxPortCnt)
        {
            RMX_Port_Info_S *RmxPortInfo = &RmxChanInfo->RmxPortInfo[RmxPortId];

            if (RmxPortInfo->PortId >= HI_UNF_DMX_PORT_RAM_0 && RmxPortInfo->PortId < HI_UNF_DMX_PORT_RAM_0 + DmxCluster->Ops->GetRamPortNum())
            {
                DmxHalDetachIPBPFQ(DmxSet, RmxPortInfo->PortId - HI_UNF_DMX_PORT_RAM_0, FqId);
                DmxHalClrIPFqBPStatus(DmxSet, RmxPortInfo->PortId - HI_UNF_DMX_PORT_RAM_0, FqId);
            }
        }

        RmxPutChnInstance(DmxSet, RmxChanId);
    }
}

HI_VOID RmxChkFqBpIp(Dmx_Set_S *DmxSet, HI_U32 RmxId, HI_U32 FqId)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    HI_U32 RmxChanId = DmxSet->Ops->GetRmxRawId(DmxSet, RmxId);

    BUG_ON(RmxId >= DmxCluster->Ops->GetRmxNum());

    if (HI_SUCCESS == RmxGetChnInstance(DmxSet, RmxChanId))
    {
        RMX_Chan_Info_S *RmxChanInfo = &DmxSet->RmxChanInfo[RmxChanId];
        HI_U32 RmxPortId;

        for_each_set_bit(RmxPortId, RmxChanInfo->RmxPortBitmap, RmxChanInfo->RmxPortCnt)
        {
            RMX_Port_Info_S *RmxPortInfo = &RmxChanInfo->RmxPortInfo[RmxPortId];

            if (RmxPortInfo->PortId >= HI_UNF_DMX_PORT_RAM_0 && RmxPortInfo->PortId < HI_UNF_DMX_PORT_RAM_0 + DmxCluster->Ops->GetRamPortNum())
            {
                __DMXCheckFQBPStatus(DmxSet, RmxPortInfo->PortId - HI_UNF_DMX_PORT_RAM_0, FqId);
            }
        }

        RmxPutChnInstance(DmxSet, RmxChanId);
    }
}

HI_VOID RemuxOverflowMonitor(Dmx_Set_S *DmxSet)
{
    HI_U32 RmxId;

    mutex_lock(&DmxSet->LockAllRmx);

    for_each_set_bit(RmxId, DmxSet->RmxChanBitmap, DmxSet->RmxChanCnt)
    {
        if (HI_SUCCESS == RmxGetChnInstance(DmxSet, RmxId))
        {
            RMX_Chan_Info_S  *RmxChanInfo = &DmxSet->RmxChanInfo[RmxId];
            HI_U32 RmxPortId;

            for_each_set_bit(RmxPortId, RmxChanInfo->RmxPortBitmap, RmxChanInfo->RmxPortCnt)
            {
                RMX_Port_Info_S *RmxPortInfo = &RmxChanInfo->RmxPortInfo[RmxPortId];

                if (RmxPortInfo->OverflowCount != DmxHalGetRemuxPortOverflowCount(DmxSet, RmxPortId))
                {
                    HI_ERR_DEMUX("remux port(%d) detect overflow!!!\n", RmxPortId);

                    RmxPortInfo->OverflowCount = DmxHalGetRemuxPortOverflowCount(DmxSet, RmxPortId);
                }
            }

            RmxPutChnInstance(DmxSet, RmxId);
        }
    }

    mutex_unlock(&DmxSet->LockAllRmx);
}
#endif
