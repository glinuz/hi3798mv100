/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: drv_demux_intf.c
 * Description: demux interface of module.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    2010-01-25   j40671     NULL         init.
 ********************************************************************************/

/***************************** included files ******************************/
#include "linux/compat.h"

#include "hi_type.h"
#include "hi_drv_struct.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_stat.h"
#include "hi_drv_mem.h"
#include "hi_module.h"
#include "hi_kernel_adapt.h"

#include "demux_debug.h"
#include "hi_mpi_demux.h"
#include "hi_drv_demux.h"

#include "drv_demux.h"
#include "drv_demux_ioctl.h"
#include "drv_demux_ext.h"
#include "drv_demux_func.h"

#ifdef DMX_DESCRAMBLER_SUPPORT
#include "drv_descrambler.h"
#include "drv_descrambler_func.h"
#endif

/**************************** global variables ****************************/
static UMAP_DEVICE_S g_stDemuxDev;
#define INVALID_OQ_ID 256
#ifdef HI_DEMUX_PROC_SUPPORT
/****************************** internal function *****************************/
char* SAVE_ES_STR = "save es";
char* SAVE_ALLTS_STR = "save allts";
char* SAVE_IPTS_STR = "save ipts";
char* SAVE_DMXTS_STR = "save dmxts";
char* HELP_STR = "help";
char* START_STR = "start";
char* STOP_STR = "stop";

static HI_VOID DMXDebugShowHelp(HI_VOID)
{
    HI_DRV_PROC_EchoHelper("echo %s %s > /proc/msp/demux_main  -- begin save es\n",SAVE_ES_STR,START_STR);
    HI_DRV_PROC_EchoHelper("echo %s %s > /proc/msp/demux_main  -- stop save es\n",SAVE_ES_STR,STOP_STR);
    HI_DRV_PROC_EchoHelper("echo %s %s x[portid] > /proc/msp/demux_main  -- begin save allts\n",SAVE_ALLTS_STR,START_STR);
    HI_DRV_PROC_EchoHelper("echo %s %s > /proc/msp/demux_main  -- stop save allts\n",SAVE_ALLTS_STR,STOP_STR);
    HI_DRV_PROC_EchoHelper("echo %s %s x[ram portid]> /proc/msp/demux_main  -- begin save ram port ts\n",SAVE_IPTS_STR,START_STR);
    HI_DRV_PROC_EchoHelper("echo %s %s > /proc/msp/demux_main  -- stop save ram port ts\n",SAVE_IPTS_STR,STOP_STR);
    HI_DRV_PROC_EchoHelper("echo %s %s x[dmxid] > /proc/msp/demux_main  -- begin save all channels ts\n",SAVE_DMXTS_STR,START_STR);
    HI_DRV_PROC_EchoHelper("echo %s %s > /proc/msp/demux_main  -- stop save dmx ts\n",SAVE_DMXTS_STR,STOP_STR);
    HI_DRV_PROC_EchoHelper("echo %s > /proc/msp/demux_main  -- show help info\n",HELP_STR);
}

#define STRSKIPBLANK(str)      \
        while (str[0] == ' ')\
        {\
        (str)++;\
        }\

static HI_S32 DMXProcWrite(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    HI_U32 cmd = 0xff;
    HI_U32 param = 0;
    HI_U32 needparam = 0;
    DMX_DEBUG_CMD_CTRl cmdctrl;
    char kbuf[256] = {0};
    char *p1 = kbuf;
    HI_S32 ret;

    if (copy_from_user(kbuf, buf, count < sizeof(kbuf) - 1 ? count : sizeof(kbuf) - 1))
    {
        HI_ERR_DEMUX("copy from user failed\n");
        return -1;
    }

    STRSKIPBLANK(p1);
    if (strstr(p1,SAVE_ES_STR) && p1[0] == 's')
    {
        cmd = DMX_DEBUG_CMD_SAVE_ES;
        p1 += strlen(SAVE_ES_STR);
    }
    else if (strstr(p1,SAVE_ALLTS_STR) && p1[0] == 's')
    {
        cmd = DMX_DEBUG_CMD_SAVE_ALLTS;
        p1 += strlen(SAVE_ALLTS_STR);
        needparam = 1;
    }
    else if (strstr(p1,SAVE_IPTS_STR) && p1[0] == 's')
    {
        cmd = DMX_DEBUG_CMD_SAVE_IPTS;
        p1 += strlen(SAVE_IPTS_STR);
        needparam = 1;
    }
    else if (strstr(p1,SAVE_DMXTS_STR) && p1[0] == 's')
    {
        cmd = DMX_DEBUG_CMD_SAVE_DMXTS;
        p1 += strlen(SAVE_DMXTS_STR);
        needparam = 1;
    }
    else if (strstr(p1,HELP_STR) && p1[0] == 'h')
    {
        DMXDebugShowHelp();
        return count;
    }
    else
    {
        HI_PRINT("unknow command:%s\n",p1);
        DMXDebugShowHelp();
        return (-1);
    }
    STRSKIPBLANK(p1);
    if (strstr(p1,START_STR))
    {
        cmdctrl = DMX_DEBUG_CMD_START;
        p1 += strlen(START_STR);
    }
    else if (strstr(p1,STOP_STR))
    {
        cmdctrl = DMX_DEBUG_CMD_STOP;
        p1 += strlen(STOP_STR);
        needparam = 0;
    }
    else
    {
        HI_PRINT("command is not correct:%s\n", kbuf);
        DMXDebugShowHelp();
        return (-1);
    }
    if (needparam)
    {
        STRSKIPBLANK(p1);
        if (p1[0] == '\n')//do not have param
        {
            HI_PRINT("command is not correct:%s\n", kbuf);
            DMXDebugShowHelp();
            return (-1);
        }
        param = (HI_U32)simple_strtoul(p1, &p1, 10);
    }

    ret = DMX_OsrDebugCtrl(cmd,cmdctrl,param);
    if (ret != HI_SUCCESS)
    {
        HI_PRINT("command is not correct:%s\n", kbuf);
        DMXDebugShowHelp();
        return ret;
    }

    return count;
}

static HI_S32 DMXProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U32 DmxId, DmxClk;
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();

    if (DMX_CLUSTER_INACTIVED == DmxCluster->Ops->GetClusterState())
    {
        HI_WARN_DEMUX("Demux cluster has not started.\n");
        goto out;
    }
    
    PROC_PRINT(p, "DmxId\tPortId\n");

    for (DmxId = 0; DmxId < DmxCluster->Ops->GetDmxNum(); DmxId++)
    {
        HI_S32              ret;
        HI_CHAR             PortStr[8] = "--";
        DMX_PORT_MODE_E     PortMode;
        HI_U32              PortId;

        ret = HI_DRV_DMX_GetPortId(DmxId, &PortMode, &PortId);
        if (HI_SUCCESS == ret)
        {
            switch(PortMode)
            {
                case DMX_PORT_MODE_TUNER:
                {
                    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
					
                    if ( 0 == DmxCluster->Ops->GetIFPortNum())
                    {
                        PortId += HI_UNF_DMX_PORT_TSI_0;
                    }
                    else if(PortId >= DmxCluster->Ops->GetIFPortNum())
                    {
                        PortId += HI_UNF_DMX_PORT_TSI_0 - DmxCluster->Ops->GetIFPortNum();
                    }

                    break;
                }

                case DMX_PORT_MODE_RAM:
                {
                    PortId += HI_UNF_DMX_PORT_RAM_0;
                    
                    break;
                }
#if defined(DMX_TAGPORT_CNT) && (DMX_TAGPORT_CNT > 0)
                case DMX_PORT_MODE_TAG:
                {
                    PortId += HI_UNF_DMX_PORT_TAG_0;
                    break;
                }
#endif
                case DMX_PORT_MODE_RMX:
                {
                    PortId += HI_MPI_DMX_PORT_RMX_0;
                    break;
                }
                
                default:
                    BUG();
            }

            snprintf(PortStr, 8, "%u", PortId);
        }

        PROC_PRINT(p, "  %u\t%s\n", DmxId, PortStr);
    }
    
    PROC_PRINT(p, "---------------\n");

    DMX_OsiGetDmxClkProc(&DmxClk);
    PROC_PRINT(p, "DmxClk\t%d(Mhz)\n\n", DmxClk);

    PROC_PRINT(p, "type \"echo help > /proc/msp/demux_main\" to get help information\n");

out:
    return HI_SUCCESS;
}

static HI_VOID PortTypeToString(HI_CHAR *str, HI_U32 len, HI_U32 Type)

{
    switch (Type)
    {
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_BURST :
            strncpy(str, "PARALLEL_BURST",len);
            break;

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_VALID :
            strncpy(str, "PARALLEL_VALID",len);
            break;

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_204 :
            strncpy(str, "PARALLEL_NOSYNC_204",len);
            break;

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188_204 :
            strncpy(str, "PARALLEL_NOSYNC_188_204",len);
            break;

        case HI_UNF_DMX_PORT_TYPE_SERIAL :
            strncpy(str, "SERIAL",len);
            break;

        case HI_UNF_DMX_PORT_TYPE_SERIAL2BIT :
            strncpy(str, "SERIAL2BIT",len);
            break;

        case HI_UNF_DMX_PORT_TYPE_SERIAL_NOSYNC :
            strncpy(str, "SERIAL_NOSYNC",len);
            break;

        case HI_UNF_DMX_PORT_TYPE_SERIAL2BIT_NOSYNC :
            strncpy(str, "SERIAL2BIT_NOSYNC",len);
            break;

        case HI_UNF_DMX_PORT_TYPE_USER_DEFINED :
            strncpy(str, "USER_DEFINED",len);
            break;

        case HI_UNF_DMX_PORT_TYPE_AUTO :
            strncpy(str, "AUTO",len);
            break;

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188 :
        default :
            strncpy(str, "PARALLEL_NOSYNC_188",len);
    }
}

static HI_S32 DMXPortProcRead(struct seq_file *p, HI_VOID *v)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    HI_U32                      i;
    HI_U32                      TsPacks = 0, ErrTsPacks = 0;
    HI_UNF_DMX_PORT_ATTR_S      PortAttr;
    HI_UNF_DMX_TSO_PORT_ATTR_S  TSOPortAttr;
    HI_U32                      PortID;

    if (DMX_CLUSTER_INACTIVED == DmxCluster->Ops->GetClusterState())
    {
        HI_WARN_DEMUX("Demux cluster has not started.\n");
        goto out;
    }

    PROC_PRINT(p, " --------------------------------IF port--------------------------\n");
    PROC_PRINT(p, " Id  AllTsCnt   ErrTsCnt  Lock/lost  ClkReverse    BitSel   Type\n");
    for (i = 0; i < DmxCluster->Ops->GetIFPortNum(); i++)
    {
        HI_CHAR str[32] = "";
        HI_CHAR BitSel[16]  = "";
        HI_DRV_DMX_TunerPortGetAttr(i, &PortAttr);

        HI_DRV_DMX_TunerPortGetPacketNum(i, &TsPacks, &ErrTsPacks);

        PortTypeToString(str, sizeof(str),PortAttr.enPortType);
        if ( PortAttr.u32SerialBitSelector == 0x0 )
        {
            strncpy(BitSel, "D7",sizeof(BitSel));
        }
        else
        {
            strncpy(BitSel, "D0",sizeof(BitSel));
        }

        PROC_PRINT(p, "%3u    0x%-8x 0x%-4x     %u/%u        %d          %s       %s\n",
            i, TsPacks, ErrTsPacks, PortAttr.u32SyncLockTh, PortAttr.u32SyncLostTh, PortAttr.u32TunerInClk,BitSel,str);
    }

    PROC_PRINT(p, "\n-------------------------------TSI port-----------------------------------\n");
    PROC_PRINT(p, " Id  AllTsCnt   ErrTsCnt  Lock/lost  ClkReverse    BitSel   Type\n");

    for (i = DmxCluster->Ops->GetIFPortNum(); i < DmxCluster->Ops->GetIFPortNum() + DmxCluster->Ops->GetTSIPortNum(); i++)
    {
        HI_CHAR str[32] = "";
        HI_CHAR BitSel[16]  = "";
        
        if ( DmxCluster->Ops->GetIFPortNum() != 0 )
        {
            PortID = i + HI_UNF_DMX_PORT_TSI_0 - DmxCluster->Ops->GetIFPortNum();
        }
        else
        {
            PortID = i + HI_UNF_DMX_PORT_TSI_0;
        }
        
        HI_DRV_DMX_TunerPortGetAttr(i, &PortAttr);

        HI_DRV_DMX_TunerPortGetPacketNum(i, &TsPacks, &ErrTsPacks);

        PortTypeToString(str, sizeof(str),PortAttr.enPortType);
        if ( PortAttr.u32SerialBitSelector == 0x0 )
        {
            strncpy(BitSel, "D7",sizeof(BitSel));
        }
        else
        {
            strncpy(BitSel, "D0",sizeof(BitSel));
        }

        PROC_PRINT(p, "%3u    0x%-8x 0x%-4x     %u/%u        %d          %s       %s\n",
            PortID, TsPacks, ErrTsPacks, PortAttr.u32SyncLockTh, PortAttr.u32SyncLostTh, PortAttr.u32TunerInClk,BitSel,str);
    }
    
    PROC_PRINT(p, "\n------------------------------------------TSO port--------------------------------------------------\n");
    PROC_PRINT(p, " Id   Enable  ClkReverse  TSPortID    ClkMode     VldMode   Sync    Serial  BitSel  LSB   Clk   ClkDiv\n");

    for (i = 0; i < DmxCluster->Ops->GetTsoPortNum(); i++)
    {
        HI_CHAR ClkMode[16] = "";
        HI_CHAR Sync[16]    = "";
        HI_CHAR BitSel[16]  = "";
        HI_CHAR Clk[16]  = "";


        HI_DRV_DMX_TSOPortGetAttr(i, &TSOPortAttr);

        if ( TSOPortAttr.enClkMode ==  HI_UNF_DMX_TSO_CLK_MODE_JITTER)
        {
            strncpy(ClkMode, "JITTER",sizeof(ClkMode));
        }
        else
        {
            strncpy(ClkMode, "NORMAL",sizeof(ClkMode));
        }

        if ( TSOPortAttr.bBitSync ==  HI_TRUE)
        {
            strncpy(Sync, "Bit ",sizeof(Sync));
        }
        else
        {
            strncpy(Sync, "Byte",sizeof(Sync));
        }

        if ( TSOPortAttr.enBitSelector == 0x0 )
        {
            strncpy(BitSel, "D0",sizeof(BitSel));
        }
        else
        {
            strncpy(BitSel, "D7",sizeof(BitSel));
        }

        switch(TSOPortAttr.enClk)
        {
            case HI_UNF_DMX_TSO_CLK_100M:
                strncpy(Clk, " 100M",sizeof(Clk));
                break;
            case HI_UNF_DMX_TSO_CLK_150M:
                strncpy(Clk, " 150M",sizeof(Clk));
                break;
            case HI_UNF_DMX_TSO_CLK_1200M:
                strncpy(Clk, "1200M",sizeof(Clk));
                break;
            case HI_UNF_DMX_TSO_CLK_1500M:
                strncpy(Clk, "1500M",sizeof(Clk));
                break;
            default:
                strncpy(Clk, "  Err",sizeof(Clk));
                break;
        }


        PROC_PRINT(p, " %d      %d         %d          %-3d      %s        %d       %s      %d      %s      %d   %s     %d\n",
        i,
        TSOPortAttr.bEnable,
        TSOPortAttr.bClkReverse,
        TSOPortAttr.enTSSource,
        ClkMode,
        TSOPortAttr.enValidMode,
        Sync,
        TSOPortAttr.bSerial,
        BitSel,
        TSOPortAttr.bLSB,
        Clk,
        TSOPortAttr.u32ClkDiv
        );

    }

    PROC_PRINT(p, "\n------------------------------------------------------------------RAM port-----------------------------------------------------\n");
    PROC_PRINT(p, " Id  AllTsCnt  TsChkRange Lock/lost   BufAddr      BufSize      BufUsed       Read       Write      Get(Try/Ok)     Put      Type\n");

    for (i = 0; i < DmxCluster->Ops->GetRamPortNum(); i++)
    {
        HI_CHAR Type[32] = "";
        HI_CHAR TsChkRange[16] = "";
        HI_CHAR BufUsed[20] = "";
        HI_U32  BufUsedRatio = 0;

        DMX_Proc_RamPort_BufInfo_S BufInfo;

        HI_DRV_DMX_RamPortGetAttr(i, &PortAttr);

        HI_DRV_DMX_RamPortGetPacketNum(i, &TsPacks);

        PortTypeToString(Type, sizeof(Type),PortAttr.enPortType);
        if ( PortAttr.enPortType !=  HI_UNF_DMX_PORT_TYPE_USER_DEFINED)
        {
            snprintf(TsChkRange, sizeof(TsChkRange),"  none   ");
        }
        else
        {
            snprintf(TsChkRange, sizeof(TsChkRange),"%d ~ %d",PortAttr.u32UserDefLen1,PortAttr.u32UserDefLen2);
        }

        if (HI_SUCCESS == DMX_OsiRamPortGetBufInfo(i, &BufInfo))
        {
            BufUsedRatio = (BufInfo.UsedSize/1024 * 100)/(BufInfo.BufSize/1024);
            snprintf(BufUsed,sizeof(BufUsed),"0x%x(%d%%)",BufInfo.UsedSize,BufUsedRatio);

            PROC_PRINT(p, "%3u    0x%-8x%s   %u/%u     0x%-8x    0x%-8x   %-10s    0x%-8x 0x%-8x%6u/%-10u%-5u    %s \n",
            HI_UNF_DMX_PORT_RAM_0 + i,
            TsPacks,
            TsChkRange,
            PortAttr.u32SyncLockTh,
            PortAttr.u32SyncLostTh,
            BufInfo.PhyAddr,
            BufInfo.BufSize,
            BufUsed,
            BufInfo.Read,
            BufInfo.Write,
            BufInfo.GetCount,
            BufInfo.GetValidCount,
            BufInfo.PutCount,
            Type);
        }
        else
        {
            PROC_PRINT(p, "%3u    0x%-8x%s   %u/%u                                                                                             %s\n",
            HI_UNF_DMX_PORT_RAM_0 + i, TsPacks, TsChkRange,PortAttr.u32SyncLockTh, PortAttr.u32SyncLostTh, Type);
        }
    }

#if defined(DMX_TAGPORT_CNT) && (DMX_TAGPORT_CNT > 0)
    {
        PROC_PRINT(p, "\n----------------------Tag port-------------------------------\n");
        PROC_PRINT(p, " Id  SyncMode TagLen              Tag               \n");

        for (i = 0; i < DMX_TAGPORT_CNT; i ++)
        {
            HI_UNF_DMX_TAG_ATTR_S TagPortAttrs;
            HI_U32 * pu32Low = (HI_U32 *)&(TagPortAttrs.au8Tag[0]);                   /* 0~4 bytes */
            HI_U32 * pu32Mid = (HI_U32 *)&(TagPortAttrs.au8Tag[4]);                /* 4~8 bytes */
            HI_U32 * pu32High = (HI_U32 *)&(TagPortAttrs.au8Tag[4 + 4]);         /* 8~12 bytes*/

            DMX_OsiGetTagPortAttr(i, &TagPortAttrs);

            PROC_PRINT(p, "  %u       %u       %u     0x%08x%08x%08x\n", i, TagPortAttrs.enSyncMod, TagPortAttrs.u32TagLen,
                *pu32Low, *pu32Mid, *pu32High);
        }
    }
#endif

out:
    return HI_SUCCESS;
}

static HI_S32 DMXChanProcRead(struct seq_file *p, HI_VOID *v)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;

    if (DMX_CLUSTER_INACTIVED == DmxCluster->Ops->GetClusterState())
    {
        HI_WARN_DEMUX("Demux cluster has not started.\n");
        goto out;
    }

    PROC_PRINT(p, "DmxId  ChnId  PID\tType Mod Stat  Secure  KeyId    Acquire(Try/Ok)    Release\n");

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        HI_U32 ChanId = 0;

        mutex_lock(&DmxSet->LockAllChn);
        
        for_each_set_bit(ChanId, DmxSet->ChnBitmap, DmxSet->DmxChanCnt)
        {     
            if (HI_SUCCESS == DmxGetChnInstance(DmxSet, ChanId))
            {
                HI_CHAR         ChanType[][8]   = {"SEC", "PES", "AUD", "VID", "PST", "ECM"};
                HI_CHAR         OutMode[][8]    = {"Reserve", "PLY", "REC", "P&R", "REC.S"};
                HI_CHAR         KeyStr[8]       = "--";
                HI_CHAR         ModeStr[8];
                DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[ChanId];
                DMX_Proc_ChanDesc_S DescInfo = {0}; 

                if (HI_SUCCESS == DMX_OsiGetChannelDescStatus(DmxSet, ChanId, &DescInfo))
                {
                    if (HI_TRUE == DescInfo.DoScram)
                    {
                        snprintf(KeyStr, 8,"%-2u", DescInfo.KeyId);
                    }
                }

                snprintf(ModeStr, sizeof(ModeStr), "%s", (ChanInfo->ChanOutMode == HI_UNF_DMX_CHAN_OUTPUT_MODE_BUTT)?"UNO":OutMode[ChanInfo->ChanOutMode]);

                if (!strncmp(ModeStr, "REC", sizeof("REC") - 1) && atomic_read(&ChanInfo->ChanRecShareCnt) > 0)
                {
                    snprintf(ModeStr, sizeof(ModeStr), "%s", OutMode[4]);
                }

                PROC_PRINT(p, "  %u      %u    0x%x\t%s  %s %s\t%s\t%s  %10u/%-10u    %u\n",
                            ChanInfo->DmxId,
                            ChanId,
                            ChanInfo->ChanPid,
                            ChanType[ChanInfo->ChanType],
                            ModeStr,
                            (HI_UNF_DMX_CHAN_CLOSE == ChanInfo->ChanStatus)  ? "CLOSE" : "OPEN",
                            (HI_UNF_DMX_SECURE_MODE_TEE == ChanInfo->ChanSecure) ? "*" : "--",
                            KeyStr,
                            ChanInfo->u32TotalAcq,
                            ChanInfo->u32HitAcq,
                            ChanInfo->u32Release
                 );

                DmxPutChnInstance(DmxSet, ChanId);
            }
        }

        mutex_unlock(&DmxSet->LockAllChn);
    }

out:    
    return HI_SUCCESS;
}

static HI_S32 DMXChanBufProcRead(struct seq_file *p, HI_VOID *v)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;

    if (DMX_CLUSTER_INACTIVED == DmxCluster->Ops->GetClusterState())
    {
        HI_WARN_DEMUX("Demux cluster has not started.\n");
        goto out;
    }

    PROC_PRINT(p, "DmxId  ChnId  FqId  OqId  Size  BlkCnt  BlkSize  Read  Write  Used  Overflow\n");

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        HI_U32 ChanId = 0;

        mutex_lock(&DmxSet->LockAllChn);
        
        for_each_set_bit(ChanId, DmxSet->ChnBitmap, DmxSet->DmxChanCnt)
        {  
            if (HI_SUCCESS == DmxGetChnInstance(DmxSet, ChanId))
            {
                DMX_Proc_ChanBuf_S  BufInfo;
                HI_U32 UsedPercent = 0;
                HI_U32 Size;
                DMX_ChanInfo_S *ChanInfo = HI_NULL;
                DMX_OQ_Info_S  *DmxOqInfo = HI_NULL;

                if (HI_SUCCESS != DMX_OsiGetChanBufProc(DmxSet, ChanId, &BufInfo))
                {
                    DmxPutChnInstance(DmxSet, ChanId);
                    continue;
                }

                ChanInfo = &DmxSet->DmxChanInfo[ChanId];
                DmxOqInfo = &DmxSet->DmxOqInfo[ChanInfo->ChanOqId];

                BUG_ON(ChanInfo->ChanOqId != DmxOqInfo->u32OQId);

                if (BufInfo.DescDepth)
                {
                    if (BufInfo.DescRead <= BufInfo.DescWrite)
                    {
                        UsedPercent = (BufInfo.DescWrite - BufInfo.DescRead) * 100 / BufInfo.DescDepth;
                    }
                    else
                    {
                        UsedPercent = (BufInfo.DescDepth + BufInfo.DescWrite - BufInfo.DescRead) * 100 / BufInfo.DescDepth;
                    }
                }

                Size = BufInfo.DescDepth * BufInfo.BlockSize / 1024;

                PROC_PRINT(p, "  %u     %u      %u     %u   %uK   %u    %uK     %u     %u     %u%%     %u\n",
                    ChanInfo->DmxId, ChanId, DmxOqInfo->u32FQId, ChanInfo->ChanOqId, Size, BufInfo.DescDepth, BufInfo.BlockSize/1024, BufInfo.DescRead, BufInfo.DescWrite, UsedPercent, BufInfo.Overflow);

                DmxPutChnInstance(DmxSet, ChanId);
            }
        }

        mutex_unlock(&DmxSet->LockAllChn);
    }

out:
    return HI_SUCCESS;
}

static HI_S32 DMXFilterProcRead(struct seq_file *p, HI_VOID *v)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;

    if (DMX_CLUSTER_INACTIVED == DmxCluster->Ops->GetClusterState())
    {
        HI_WARN_DEMUX("Demux cluster has not started.\n");
        goto out;
    }

    PROC_PRINT(p, "DmxId ChnId FltId Depth Param\n");

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        HI_U32 FltId = 0;

        mutex_lock(&DmxSet->LockAllFlt);
        
        for_each_set_bit(FltId, DmxSet->FilterBitmap, DmxSet->DmxFilterCnt)
        {
            if (HI_SUCCESS == DmxGetFltInstance(DmxSet, FltId))
            {
                DMX_FilterInfo_S   *FilterInfo = &DmxSet->DmxFilterInfo[FltId];
                HI_CHAR             str[16] = "--";
                HI_U32 i;

                if (FilterInfo->ChanId < DmxSet->DmxChanCnt)
                {
                    DMX_ChanInfo_S *ChanInfo = &DmxSet->DmxChanInfo[FilterInfo->ChanId];

                    snprintf(str, 16, "%2u    %2u", ChanInfo->DmxId, FilterInfo->ChanId);
                }

                PROC_PRINT(p, " %s     %u   %3u   Match :", str, FltId, FilterInfo->Depth);

                for (i = 0; i < FilterInfo->Depth; i++)
                {
                    PROC_PRINT(p, " %02x", FilterInfo->Match[i]);
                }

                PROC_PRINT(p, "\n\t\t\t Mask  :");
                for (i = 0; i < FilterInfo->Depth; i++)
                {
                    PROC_PRINT(p, " %02x", FilterInfo->Mask[i]);
                }

                PROC_PRINT(p, "\n\t\t\t Negate:");
                for (i = 0; i < FilterInfo->Depth; i++)
                {
                    PROC_PRINT(p, " %02x", FilterInfo->Negate[i]);
                }

                PROC_PRINT(p, "\n");

                DmxPutFltInstance(DmxSet, FltId);
            }
        }

        mutex_unlock(&DmxSet->LockAllFlt);
    }

out:
    return 0;
}

static HI_S32 DMXPcrProcRead(struct seq_file *p, HI_VOID *v)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;

    if (DMX_CLUSTER_INACTIVED == DmxCluster->Ops->GetClusterState())
    {
        HI_WARN_DEMUX("Demux cluster has not started.\n");
        goto out;
    }

    PROC_PRINT(p, "DmxId PcrId   PID    CurrPcr     CurrScr\n");

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        HI_U32 PcrId = 0;

        mutex_lock(&DmxSet->LockAllPcr);
        
        for_each_set_bit(PcrId, DmxSet->PcrBitmap, DmxSet->DmxPcrCnt)
        {
            if (HI_SUCCESS == DmxGetPcrInstance(DmxSet, PcrId))
            {
                DMX_PCR_Info_S *PcrInfo = &DmxSet->DmxPcrInfo[PcrId];

                PROC_PRINT(p, "  %u    %u      0x%-4x 0x%-9llx 0x%-9llx\n",
                    PcrInfo->DmxId, PcrId, PcrInfo->PcrPid, PcrInfo->PcrValue, PcrInfo->ScrValue);

                DmxPutPcrInstance(DmxSet, PcrId);
            }
        }

        mutex_unlock(&DmxSet->LockAllPcr);
    }

out:
    return HI_SUCCESS;
}

static HI_S32 DMXRecProcRead(struct seq_file *p, HI_VOID *v)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;

    if (DMX_CLUSTER_INACTIVED == DmxCluster->Ops->GetClusterState())
    {
        HI_WARN_DEMUX("Demux cluster has not started.\n");
        goto out;
    }

    PROC_PRINT(p, "DmxId RecId FqId OqId Type Descramed Status  Size  BlkCnt BlkSize Read Write Overflow\n");

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        HI_U32 RecId = 0;

        mutex_lock(&DmxSet->LockAllRec);
        
        for_each_set_bit(RecId, DmxSet->RecBitmap, DmxSet->DmxRecCnt)
        {
            if (HI_SUCCESS == DmxGetRecInstance(DmxSet, RecId))
            {
                DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];
                DMX_Proc_Rec_BufInfo_S  RecBufInfo;
                HI_CHAR                *Type;
                HI_CHAR                *Status;
                HI_U32                  Size;

                if (HI_SUCCESS != DMX_OsiGetDmxRecProc(DmxSet, RecId, &RecBufInfo))
                {
                    DmxPutRecInstance(DmxSet, RecId);
                    continue;
                }

                switch (RecBufInfo.RecType)
                {
                    case HI_UNF_DMX_REC_TYPE_SELECT_PID :
                        Type = "pid";
                        break;

                    case HI_UNF_DMX_REC_TYPE_ALL_PID :
                        Type = "all";
                        break;

                    default :
                        DmxPutRecInstance(DmxSet, RecId);
                        continue;
                }

                Size    = RecBufInfo.BlockCnt * RecBufInfo.BlockSize / 1024;
                Status  = RecBufInfo.RecStatus ? "start" : "stop ";

                PROC_PRINT(p, "%3u   %3u   %2u   %2u   %s      %u     %s  %5uK %-6u %-7u %-4u %-5u %u\n",
                    RecInfo->DmxId, RecId, RecInfo->RecFqId, RecInfo->RecOqId, Type, RecBufInfo.Descramed, Status, Size, RecBufInfo.BlockCnt,
                    RecBufInfo.BlockSize, RecBufInfo.BufRead, RecBufInfo.BufWrite, RecBufInfo.Overflow);

                DmxPutRecInstance(DmxSet, RecId);
            }
        }

        mutex_unlock(&DmxSet->LockAllRec);
    }

out:
    return HI_SUCCESS;
}

static HI_S32 DMXRecScdProcRead(struct seq_file *p, HI_VOID *v)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;

    if (DMX_CLUSTER_INACTIVED == DmxCluster->Ops->GetClusterState())
    {
        HI_WARN_DEMUX("Demux cluster has not started.\n");
        goto out;
    }

    PROC_PRINT(p, "DmxId RecId FqId OqId ScdId Type  Pid    Size BlkCnt BlkSize Read Write Overflow\n");

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        HI_U32 RecId = 0;

        mutex_lock(&DmxSet->LockAllRec);
        
        for_each_set_bit(RecId, DmxSet->RecBitmap, DmxSet->DmxRecCnt)
        {
            if (HI_SUCCESS == DmxGetRecInstance(DmxSet, RecId))
            {
                DMX_RecInfo_S  *RecInfo = &DmxSet->DmxRecInfo[RecId];
                DMX_Proc_RecScd_BufInfo_S   ScdBufInfo;
                HI_CHAR *Type;
                HI_U32 Size;

                if (HI_SUCCESS != DMX_OsiGetDmxRecScdProc(DmxSet, RecId, &ScdBufInfo))
                {
                    DmxPutRecInstance(DmxSet, RecId);
                    continue;
                }

                switch (ScdBufInfo.IndexType)
                {
                    case HI_UNF_DMX_REC_INDEX_TYPE_VIDEO :
                        Type = "video";
                        break;

                    case HI_UNF_DMX_REC_INDEX_TYPE_AUDIO :
                        Type = "audio";
                        break;

                    case HI_UNF_DMX_REC_INDEX_TYPE_NONE :
                    default :
                        Type = "none ";
                }

                Size = ScdBufInfo.BlockCnt * ScdBufInfo.BlockSize / 1024;

                PROC_PRINT(p, "%3u   %3u   %2u   %2u   %2u    %s 0x%-4x %3uK %-6u %-7u %-4u %-5u %u\n",
                    RecInfo->DmxId, RecId, ScdBufInfo.FqId, ScdBufInfo.OqId, ScdBufInfo.ScdId, Type, ScdBufInfo.IndexPid, Size, ScdBufInfo.BlockCnt,
                    ScdBufInfo.BlockSize, ScdBufInfo.BufRead, ScdBufInfo.BufWrite, ScdBufInfo.Overflow);

                DmxPutRecInstance(DmxSet, RecId);
            }
        }

        mutex_unlock(&DmxSet->LockAllRec);
    }

out:   
    return HI_SUCCESS;
}

#ifdef RMX_SUPPORT
static HI_S32 DMXRmxProcRead(struct seq_file *p, HI_VOID *v)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *DmxSet = HI_NULL;

    if (DMX_CLUSTER_INACTIVED == DmxCluster->Ops->GetClusterState())
    {
        HI_WARN_DEMUX("Demux cluster has not started.\n");
        goto out;
    }

    PROC_PRINT(p, "RmxId   PumpId   Type       Pid   NewPid    Port        Overflow\n");

    TraverseForEachDmxSet(DmxSet, &DmxCluster->head)
    {
        HI_U32 RmxId;

        mutex_lock(&DmxSet->LockAllRmx);
        
        for_each_set_bit(RmxId, DmxSet->RmxChanBitmap, DmxSet->RmxChanCnt)
        {
            if (HI_SUCCESS == RmxGetChnInstance(DmxSet, RmxId))
            {
                RMX_Chan_Info_S  *RmxChanInfo = &DmxSet->RmxChanInfo[RmxId];
                HI_U32 PumpId;

                for_each_set_bit(PumpId, RmxChanInfo->RmxPumpBitmap, DmxSet->RmxPumpCnt)
                {
                    if (HI_SUCCESS == RmxGetPumpInstance(DmxSet, PumpId))
                    {
                        RMX_Pump_Info_S *RmxPumpInfo = &DmxSet->RmxPumpInfo[PumpId];
                        RMX_Port_Info_S *RmxPortInfo = &RmxChanInfo->RmxPortInfo[RmxPumpInfo->RmxPortId];

                        switch(RmxPumpInfo->Type)
                        {
                            case HI_MPI_RMX_PUMP_TYPE_PID:
                                PROC_PRINT(p, "  %d       %d      %s        0x%x   %s        %d/%d          %d\n", RmxId, PumpId, "Pid", RmxPumpInfo->Pid, "--", 
                                        RmxPortInfo->PortId, RmxPumpInfo->RmxPortId, RmxPortInfo->OverflowCount);
                                break;
                            case HI_MPI_RMX_PUMP_TYPE_REMAP_PID:
                                PROC_PRINT(p, "  %d       %d      %s   0x%x   0x%x       %d/%d          %d\n", RmxId, PumpId, "RemapPid", RmxPumpInfo->Pid, RmxPumpInfo->RemapPid, 
                                        RmxPortInfo->PortId, RmxPumpInfo->RmxPortId, RmxPortInfo->OverflowCount);
                                break;
                            case HI_MPI_RMX_PUMP_TYPE_ALLPASS_PORT:
                                PROC_PRINT(p, "  %d       %d      %s    %s    %s        %d/%d          %d\n", RmxId, PumpId, "AllPass", "--", "--", 
                                        RmxPortInfo->PortId, RmxPumpInfo->RmxPortId, RmxPortInfo->OverflowCount);
                                break;
                            default:
                                BUG();
                        }
                                            
                        RmxPutPumpInstance(DmxSet, PumpId);
                    }
                }

                RmxPutChnInstance(DmxSet, RmxId);
            }
        }

        mutex_unlock(&DmxSet->LockAllRmx);
    }

out:   
    return HI_SUCCESS;
}
#endif
#endif

/**************************** external functions ******************************/

static HI_S32 DMXGlobalIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_GET_POOLBUF_ADDR:
        {
            DMX_PoolBuf_Attr_S *Param = (DMX_PoolBuf_Attr_S*)arg;
            DMX_MMZ_BUF_S PoolBuf = {0};

            ret = HI_DRV_DMX_GetPoolBufAddr(&PoolBuf);

            Param->BufPhyAddr = PoolBuf.PhyAddr;
            Param->BufSize = PoolBuf.Size;
            Param->BufFlag = PoolBuf.Flag;

            break;
        }

        case CMD_DEMUX_GET_CAPABILITY:
        {
            HI_UNF_DMX_CAPABILITY_S *Param = (HI_UNF_DMX_CAPABILITY_S*)arg;

            ret = HI_DRV_DMX_GetCapability(Param);

            break;
        }
        case CMD_DEMUX_SET_PUSI:
        {
            HI_UNF_DMX_PUSI_SET_S *pPara = (HI_UNF_DMX_PUSI_SET_S *)arg;
            ret = HI_DRV_DMX_SetPusi(pPara->bPusi);
            break;
        }
        case CMD_DEMUX_SET_TEI:
        {
            HI_UNF_DMX_TEI_SET_S *pPara = (HI_UNF_DMX_TEI_SET_S *)arg;
            ret = HI_DRV_DMX_SetTei(pPara);
            break;
        }

        case CMD_DEMUX_GET_RESUME_COUNT:
        {
            HI_U32 *pCount = (HI_U32 *)arg;
            ret = HI_DRV_DMX_GetResumeCount(pCount);
            break;
        }

        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXPortIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_PORT_GET_ATTR:
        {
            DMX_Port_GetAttr_S *Param = (DMX_Port_GetAttr_S*)arg;

            if (DMX_PORT_MODE_RAM == Param->PortMode)
            {
                ret = HI_DRV_DMX_RamPortGetAttr(Param->PortId, &Param->PortAttr);
            }
            else
            {
                ret = HI_DRV_DMX_TunerPortGetAttr(Param->PortId, &Param->PortAttr);
            }

            break;
        }

        case CMD_DEMUX_PORT_SET_ATTR:
        {
            DMX_Port_SetAttr_S *Param = (DMX_Port_SetAttr_S*)arg;

            if (DMX_PORT_MODE_RAM == Param->PortMode)
            {
                ret = HI_DRV_DMX_RamPortSetAttr(Param->PortId, &Param->PortAttr);
            }
            else
            {
                ret = HI_DRV_DMX_TunerPortSetAttr(Param->PortId, &Param->PortAttr);
            }

            break;
        }

        case CMD_DEMUX_TSO_PORT_GET_ATTR:
        {
            DMX_TSO_Port_Attr_S *Param = (DMX_TSO_Port_Attr_S*)arg;
            
            ret = HI_DRV_DMX_TSOPortGetAttr(Param->PortId, &Param->PortAttr);
            
            break;
        }

        case CMD_DEMUX_TSO_PORT_SET_ATTR:
        {
            DMX_TSO_Port_Attr_S *Param = (DMX_TSO_Port_Attr_S*)arg;
            
            ret = HI_DRV_DMX_TSOPortSetAttr(Param->PortId, &Param->PortAttr);
            
            break;
        }

        case CMD_DEMUX_DMX_GET_TAG_ATTR:
        {
#if defined(DMX_TAGPORT_CNT) && (DMX_TAGPORT_CNT > 0)
            DMX_Tag_GetAttr_S *Param = (DMX_Tag_GetAttr_S*)arg;

            ret = HI_DRV_DMX_TagPortGetAttr(Param->PortId, &Param->TagAttr);
#else
            ret = HI_ERR_NOT_SUPPORT_TAGDEAL;
#endif
            break;
        }

        case CMD_DEMUX_DMX_SET_TAG_ATTR:
        {
#if defined(DMX_TAGPORT_CNT) && (DMX_TAGPORT_CNT > 0)
            DMX_Tag_SetAttr_S *Param = (DMX_Tag_GetAttr_S*)arg;

            ret = HI_DRV_DMX_TagPortSetAttr(Param->PortId, &Param->TagAttr);
#else
            ret = HI_ERR_NOT_SUPPORT_TAGDEAL;
#endif
            break;
        }

        case CMD_DEMUX_PORT_ATTACH:
        {
            DMX_Port_Attach_S *Param = (DMX_Port_Attach_S*)arg;

            ret = HI_DRV_DMX_AttachPort(Param->DmxId, Param->PortMode, Param->PortId);

            break;
        }

        case CMD_DEMUX_PORT_DETACH:
        {
            ret = HI_DRV_DMX_DetachPort(*(HI_U32*)arg);

            break;
        }

        case CMD_DEMUX_PORT_GETID:
        {
            DMX_Port_GetId_S *Param = (DMX_Port_GetId_S*)arg;

            ret = HI_DRV_DMX_GetPortId(Param->DmxId, &Param->PortMode, &Param->PortId);

            break;
        }

        case CMD_DEMUX_PORT_GETPACKETNUM:
        {
            DMX_PortPacketNum_S *Param = (DMX_PortPacketNum_S*)arg;

            if (DMX_PORT_MODE_RAM == Param->PortMode)
            {
                Param->ErrTsPackCnt = 0;

                ret = HI_DRV_DMX_RamPortGetPacketNum(Param->PortId, &Param->TsPackCnt);
            }
            else
            {
                ret = HI_DRV_DMX_TunerPortGetPacketNum(Param->PortId, &Param->TsPackCnt, &Param->ErrTsPackCnt);
            }

            break;
        }

        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXTsBufferIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_TS_BUFFER_INIT:
        {
            DMX_TsBufInit_S *Param = (DMX_TsBufInit_S*)arg;
            DMX_MMZ_BUF_S TsBuf = {0};

            ret = HI_DRV_DMX_CreateTSBuffer(Param->PortId, &Param->Attr, &TsBuf, (HI_VOID *)file);

            Param->BufPhyAddr = TsBuf.PhyAddr;
            Param->BufSize = TsBuf.Size;
            Param->BufFlag = TsBuf.Flag;

            break;
        }

        case CMD_DEMUX_TS_BUFFER_DEINIT:
        {
            ret = HI_DRV_DMX_DestroyTSBuffer(*(HI_U32*)arg);
            
            break;
        }

        case CMD_DEMUX_TS_BUFFER_GET:
        {
            DMX_TsBufGet_S *Param = (DMX_TsBufGet_S*)arg;
            DMX_DATA_BUF_S TsBuf = {0};

            ret = HI_DRV_DMX_GetTSBuffer(Param->PortId, Param->ReqLen, &TsBuf, Param->TimeoutMs);

            Param->BufPhyAddr = TsBuf.BufPhyAddr;
            Param->BufSize = TsBuf.BufLen;

            break;
        }

        case CMD_DEMUX_TS_BUFFER_PUSH:
        {
            DMX_TsBufPush_S *pPara = (DMX_TsBufPush_S *)arg;
            DMX_DATA_BUF_S TsBuf = {0};

            TsBuf.BufPhyAddr = pPara->BufPhyAddr;
            TsBuf.BufLen = pPara->BufSize;
            
            ret = HI_DRV_DMX_PushTSBuffer(pPara->PortId, &TsBuf);
            
            break;
        }
        
        case CMD_DEMUX_TS_BUFFER_RELEASE:
        {
            DMX_TsBufRel_S *pPara = (DMX_TsBufRel_S *)arg;
            DMX_DATA_BUF_S TsBuf = {0};

            TsBuf.BufPhyAddr = pPara->BufPhyAddr;
            TsBuf.BufLen = pPara->BufSize;
            
            ret = HI_DRV_DMX_ReleaseTSBuffer(pPara->PortId, &TsBuf);
            
            break;
        }

        case CMD_DEMUX_TS_BUFFER_PUT:
        {
            DMX_TsBufPut_S *Param = (DMX_TsBufPut_S*)arg;

            ret = HI_DRV_DMX_PutTSBuffer(Param->PortId, Param->ValidDataLen, Param->StartPos);

            break;
        }

        case CMD_DEMUX_TS_BUFFER_RESET:
        {
            ret = HI_DRV_DMX_ResetTSBuffer(*(HI_U32*)arg);

            break;
        }

        case CMD_DEMUX_TS_BUFFER_GET_STATUS:
        {
            DMX_TsBufStaGet_S *Param = (DMX_TsBufStaGet_S*)arg;

            ret = HI_DRV_DMX_GetTSBufferStatus(Param->PortId, &Param->Status);

            break;
        }

        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXChanIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_CHAN_NEW:
        {
            DMX_ChanNew_S *pPara = (DMX_ChanNew_S *)arg;
            DMX_MMZ_BUF_S ChBuf = {0};
            
            ret = HI_DRV_DMX_CreateChannel(pPara->u32DemuxId, &pPara->stChAttr,
                                    &pPara->hChannel, &ChBuf, (HI_VOID *)file);

            pPara->BufPhyAddr = ChBuf.PhyAddr;
            pPara->BufSize = ChBuf.Size;
            pPara->BufFlag = ChBuf.Flag;

            break;
        }

        case CMD_DEMUX_CHAN_NEW2:
        {
            DMX_ChanNew_S *pPara = (DMX_ChanNew_S *)arg;
            DMX_MMZ_BUF_S ChBuf = {0};
            
            ret = HI_DRV_DMX_CreateChannelWithPID(pPara->u32DemuxId, pPara->u32Pid, &pPara->stChAttr,
                                    &pPara->hChannel, &ChBuf, (HI_VOID *)file);

            pPara->BufPhyAddr = ChBuf.PhyAddr;
            pPara->BufSize = ChBuf.Size;
            pPara->BufFlag = ChBuf.Flag;
            
            break;
        }

        case CMD_DEMUX_CHAN_DEL:
        {
            ret = HI_DRV_DMX_DestroyChannel(*(HI_HANDLE*)arg);
            
            break;
        }

        case CMD_DEMUX_CHAN_OPEN:
        {
            ret = HI_DRV_DMX_OpenChannel(*(HI_HANDLE*)arg);
            
            break;
        }

        case CMD_DEMUX_CHAN_CLOSE:
        {
            ret = HI_DRV_DMX_CloseChannel(*(HI_HANDLE*)arg);
            
            break;
        }

        case CMD_DEMUX_CHAN_ATTR_GET:
        {
            DMX_GetChan_Attr_S *pPara = (DMX_GetChan_Attr_S *)arg;
            
            ret = HI_DRV_DMX_GetChannelAttr(pPara->hChannel, &pPara->stChAttr);
            
            break;
        }

        case CMD_DEMUX_CHAN_ATTR_SET:
        {
            DMX_SetChan_Attr_S *pPara = (DMX_SetChan_Attr_S *)arg;
            
            ret = HI_DRV_DMX_SetChannelAttr(pPara->hChannel, &pPara->stChAttr);
            
            break;
        }

        case CMD_DEMUX_GET_CHAN_STATUS:
        {
            DMX_ChanStatusGet_S *pPara = (DMX_ChanStatusGet_S *)arg;
            
            ret = HI_DRV_DMX_GetChannelStatus(pPara->hChannel, &pPara->stStatus);
            
            break;
        }

        case CMD_DEMUX_PID_SET:
        {
            DMX_ChanPIDSet_S *pPara = (DMX_ChanPIDSet_S *)arg;
            
            ret = HI_DRV_DMX_SetChannelPID(pPara->hChannel, pPara->u32Pid);
            
            break;
        }

        case CMD_DEMUX_PID_GET:
        {
            DMX_ChanPIDGet_S *pPara = (DMX_ChanPIDGet_S *)arg;

            ret = HI_DRV_DMX_GetChannelPID(pPara->hChannel, &pPara->u32Pid);

            break;
        }

        case CMD_DEMUX_CHANID_GET:
        {
            DMX_ChannelIdGet_S *pPara = (DMX_ChannelIdGet_S *)arg;

            ret = HI_DRV_DMX_GetChannelHandle(pPara->u32DmxId, pPara->u32Pid, &pPara->hChannel);

            break;
        }

        case CMD_DEMUX_FREECHAN_GET:
        {
            DMX_FreeChanGet_S *pPara = (DMX_FreeChanGet_S *)arg;

            ret = HI_DRV_DMX_GetFreeChannelCount(pPara->u32DmxId, &pPara->u32FreeCount);

            break;
        }

        case CMD_DEMUX_SCRAMBLEFLAG_GET:
        {
            DMX_ScrambledFlagGet_S *pPara = (DMX_ScrambledFlagGet_S *)arg;

            ret = HI_DRV_DMX_GetScrambledFlag(pPara->hChannel, &pPara->enScrambleFlag);

            break;
        }

        case CMD_DEMUX_CHAN_SET_EOS_FLAG:
        {
            ret = HI_DRV_DMX_SetChannelEosFlag(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_GET_CHAN_TSCNT:
        {
            DMX_ChanChanTsCnt_S *pPara = (DMX_ChanChanTsCnt_S *)arg;

            ret = HI_DRV_DMX_GetChannelTsCount(pPara->hChannel, &(pPara->u32ChanTsCnt));

            break;
        }
        
        case CMD_DEMUX_CHAN_CC_REPEAT_SET:
        {
            DMX_SetChan_CC_REPEAT_S *pPara = (DMX_SetChan_CC_REPEAT_S *)arg;

            ret = HI_DRV_DMX_SetChannelCCRepeat(pPara->stChCCRepeatSet.hChannel, &pPara->stChCCRepeatSet);

            break;
        }

        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXFiltIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_FLT_NEW:
        {
            DMX_NewFilter_S *Param = (DMX_NewFilter_S*)arg;

            ret = HI_DRV_DMX_CreateFilter(Param->DmxId, &Param->FilterAttr, &Param->Filter, (HI_VOID *)file);

            break;
        }

        case CMD_DEMUX_FLT_DEL:
        {
            ret = HI_DRV_DMX_DestroyFilter(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_FLT_SET:
        {
            DMX_FilterSet_S *Param = (DMX_FilterSet_S*)arg;

            ret = HI_DRV_DMX_SetFilterAttr(Param->Filter, &Param->FilterAttr);

            break;
        }

        case CMD_DEMUX_FLT_GET:
        {
            DMX_FilterGet_S *Param = (DMX_FilterGet_S*)arg;

            ret = HI_DRV_DMX_GetFilterAttr(Param->Filter, &Param->FilterAttr);

            break;
        }

        case CMD_DEMUX_FLT_ATTACH:
        {
            DMX_FilterAttach_S *Param = (DMX_FilterAttach_S*)arg;

            ret = HI_DRV_DMX_AttachFilter(Param->Filter, Param->Channel);

            break;
        }

        case CMD_DEMUX_FLT_DETACH:
        {
            DMX_FilterDetach_S *Param = (DMX_FilterDetach_S*)arg;

            ret = HI_DRV_DMX_DetachFilter(Param->Filter, Param->Channel);

            break;
        }

        case CMD_DEMUX_FREEFLT_GET:
        {
            DMX_FreeFilterGet_S *Param = (DMX_FreeFilterGet_S*)arg;

            ret = HI_DRV_DMX_GetFreeFilterCount(Param->DmxId, &Param->FreeCount);

            break;
        }

        case CMD_DEMUX_FLT_DELALL:
        {
            ret = HI_DRV_DMX_DestroyAllFilter(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_FLT_CHANID_GET:
        {
            DMX_FilterChannelIDGet_S *Param = (DMX_FilterChannelIDGet_S*)arg;

            ret = HI_DRV_DMX_GetFilterChannelHandle(Param->Filter, &Param->Channel);

            break;
        }

        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXRecvIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_GET_DATA_FLAG:
        {
            DMX_GetDataFlag_S *pPara = (DMX_GetDataFlag_S *)arg;

            if (pPara->WatchChannelNum <= GetDmxCluster()->Ops->GetChnNum())
            {
                HI_U32 ValidChannelNum = pPara->WatchChannelNum;
                HI_HANDLE ValidChannel[pPara->WatchChannelNum];
                
                ret = HI_DRV_DMX_GetDataHandle(ValidChannel, &ValidChannelNum, pPara->u32TimeOutMs);
                if (HI_SUCCESS == ret)
                {
                    put_user(ValidChannelNum, pPara->ValidChannelNum);
                    
                    if (copy_to_user(pPara->ValidChannel, ValidChannel, sizeof(HI_HANDLE) * ValidChannelNum))
                    {
                        HI_ERR_DEMUX("copy_to_user failed.\n");
                        ret = HI_FAILURE;
                    }
                }
            }
            else
            {
                HI_ERR_DEMUX("WatchChannelNum is too big.\n");
            }
            
            break;
        }

        case CMD_DEMUX_SELECT_DATA_FLAG:
        {
            DMX_SelectDataFlag_S *pPara = (DMX_SelectDataFlag_S*)arg;

            if (pPara->WatchChannelNum <= GetDmxCluster()->Ops->GetChnNum())
            {
                HI_U32 WatchChannelNum = pPara->WatchChannelNum;
                HI_U32 ValidChannelNum;
                HI_HANDLE WatchChannel[WatchChannelNum];
                HI_HANDLE ValidChannel[WatchChannelNum];
            
                if (copy_from_user(WatchChannel, pPara->WatchChannel, sizeof(HI_HANDLE) * WatchChannelNum))
                {
                    HI_ERR_DEMUX("copy_from_user failed.\n");
                    ret = HI_FAILURE;
                }
                else
                {
                    ret = HI_DRV_DMX_SelectDataHandle(WatchChannel, WatchChannelNum, ValidChannel, &ValidChannelNum, pPara->u32TimeOutMs);
                    if (HI_SUCCESS == ret)
                    {
                        put_user(ValidChannelNum, pPara->ValidChannelNum);

                        if (copy_to_user(pPara->ValidChannel, ValidChannel, sizeof(HI_HANDLE) * ValidChannelNum))
                        {
                            HI_ERR_DEMUX("copy_to_user failed.\n");
                            ret = HI_FAILURE; 
                        }
                    }
                }
            }
            else
            {
                HI_ERR_DEMUX("WatchChannelNum is too big.\n");
            }
            
            break;
        }

        case CMD_DEMUX_ACQUIRE_MSG:
        {
            DMX_AcqMsg_S *pPara = (DMX_AcqMsg_S *)arg;
            DMX_UserMsg_S stReqBufTmp[DMX_DEFAULT_BUF_NUM];
            DMX_UserMsg_S* pstBufTmp;
            HI_U32 u32AcquireNum = pPara->u32AcquireNum;
            HI_UNF_DMX_DATA_S *pstBuf;
            
            if (u32AcquireNum > DMX_DEFAULT_BUF_NUM)
            {
                pstBufTmp = HI_KMALLOC(HI_ID_DEMUX, sizeof(DMX_UserMsg_S) * u32AcquireNum, GFP_KERNEL);
                if (NULL == pstBufTmp)
                {
                    HI_FATAL_DEMUX("malloc failed.\n");
                    ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
                    break;
                }
            }
            else
            {
                pstBufTmp = stReqBufTmp;
            }

            ret = HI_DRV_DMX_AcquireBuf(pPara->hChannel, pPara->u32AcquireNum,
                                    &pPara->u32AcquiredNum, pstBufTmp, pPara->u32TimeOutMs);

            if (ret == HI_SUCCESS && pPara->u32AcquiredNum)
            {
                HI_UNF_DMX_DATA_S stUsrBuf;
                HI_U32 i;

                pstBuf = pPara->pstBuf;
                for (i = 0; i < pPara->u32AcquiredNum; i++)
                {
                    stUsrBuf.pu8Data = (HI_U8*)(unsigned long)pstBufTmp[i].u32BufStartAddr;
                    stUsrBuf.u32Size = pstBufTmp[i].u32MsgLen;
                    stUsrBuf.enDataType = pstBufTmp[i].enDataType;

                    if (copy_to_user(&pstBuf[i], &stUsrBuf, sizeof(HI_UNF_DMX_DATA_S)))
                    {
                        HI_ERR_DEMUX("copy data buf to usr failed\n");
                        ret = HI_FAILURE;
                        break;
                    }
                }
            }
            if (u32AcquireNum > DMX_DEFAULT_BUF_NUM)
            {
                HI_KFREE(HI_ID_DEMUX, pstBufTmp);
            }

            break;
        }

        case CMD_DEMUX_RELEASE_MSG:
        {
            DMX_RelMsg_S *pPara = (DMX_RelMsg_S *)arg;
            DMX_UserMsg_S *pstBufTmp;
            DMX_UserMsg_S stReqBufTmp[DMX_DEFAULT_BUF_NUM];
            HI_U32 u32ReleaseNum;
            HI_UNF_DMX_DATA_S* pstBuf;
            HI_UNF_DMX_DATA_S stUsrBuf;
            HI_U32 i;

            u32ReleaseNum = pPara->u32ReleaseNum;
            if (u32ReleaseNum > DMX_DEFAULT_BUF_NUM)
            {
                pstBufTmp = HI_KMALLOC(HI_ID_DEMUX, sizeof(DMX_UserMsg_S) * u32ReleaseNum, GFP_KERNEL);
            }
            else
            {
                pstBufTmp = stReqBufTmp;
            }

            if (NULL == pstBufTmp)
            {
                HI_FATAL_DEMUX("malloc failed u32ReleaseNum:%x.\n", u32ReleaseNum);
                ret =  HI_ERR_DMX_ALLOC_MEM_FAILED;
                break;
            }
            pstBuf = pPara->pstBuf;
            for (i = 0; i < u32ReleaseNum; i++)
            {
                if (copy_from_user(&stUsrBuf, &pstBuf[i], sizeof(HI_UNF_DMX_DATA_S)))
                {
                    HI_ERR_DEMUX("copy data buf to usr failed\n");
                    ret = HI_FAILURE;
                    break;
                }

                pstBufTmp[i].u32BufStartAddr = (HI_U32)(unsigned long)stUsrBuf.pu8Data;
                pstBufTmp[i].u32MsgLen = stUsrBuf.u32Size;
            }
            ret = HI_DRV_DMX_ReleaseBuf(pPara->hChannel, u32ReleaseNum, pstBufTmp);
            if (u32ReleaseNum > DMX_DEFAULT_BUF_NUM)
            {
                HI_KFREE(HI_ID_DEMUX, pstBufTmp);
            }

            break;
        }

        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

#ifdef CONFIG_COMPAT  
static HI_S32 Compat_DMXRecvIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_COMPAT_GET_DATA_FLAG:
        {
            DMX_Compat_GetDataFlag_S *pPara = (DMX_Compat_GetDataFlag_S *)arg;

            if (pPara->WatchChannelNum <= GetDmxCluster()->Ops->GetChnNum())
            {
                HI_U32 ValidChannelNum = pPara->WatchChannelNum;
                HI_HANDLE ValidChannel[pPara->WatchChannelNum];
                
                ret = HI_DRV_DMX_GetDataHandle(ValidChannel, &ValidChannelNum, pPara->u32TimeOutMs);
                if (HI_SUCCESS == ret)
                {
                    put_user(ValidChannelNum, (HI_U32*)compat_ptr(pPara->ValidChannelNum));
                    
                    if (copy_to_user(compat_ptr(pPara->ValidChannel), ValidChannel, sizeof(HI_HANDLE) * ValidChannelNum))
                    {
                        HI_ERR_DEMUX("copy_to_user failed.\n");
                        ret = HI_FAILURE;
                    }
                }
            }
            else
            {
                HI_ERR_DEMUX("WatchChannelNum is too big.\n");
            }
            
            break;
        }

        case CMD_DEMUX_COMPAT_SELECT_DATA_FLAG:
        {
            DMX_Compat_SelectDataFlag_S *pPara = (DMX_Compat_SelectDataFlag_S*)arg;

            if (pPara->WatchChannelNum <= GetDmxCluster()->Ops->GetChnNum())
            {
                HI_U32 WatchChannelNum = pPara->WatchChannelNum;
                HI_U32 ValidChannelNum;
                HI_HANDLE WatchChannel[WatchChannelNum];
                HI_HANDLE ValidChannel[WatchChannelNum];

                if (copy_from_user(WatchChannel, compat_ptr(pPara->WatchChannel), sizeof(HI_HANDLE) * WatchChannelNum))
                {
                    HI_ERR_DEMUX("copy_from_user failed.\n");
                    ret = HI_FAILURE;
                }
                else
                {
                    ret = HI_DRV_DMX_SelectDataHandle(WatchChannel, WatchChannelNum, ValidChannel, &ValidChannelNum, pPara->u32TimeOutMs);
                    if (HI_SUCCESS == ret)
                    {
                        put_user(ValidChannelNum, (HI_U32*)compat_ptr(pPara->ValidChannelNum));

                        if (copy_to_user(compat_ptr(pPara->ValidChannel), ValidChannel, sizeof(HI_HANDLE) * ValidChannelNum))
                        {
                            HI_ERR_DEMUX("copy_to_user failed.\n");
                            ret = HI_FAILURE;
                        }
                    }
                }
            }
            else
            {
                HI_ERR_DEMUX("WatchChannelNum is too big.\n");
            }
            
            break;
        }

        case CMD_DEMUX_COMPAT_ACQUIRE_MSG:
        {
            DMX_Compat_AcqMsg_S *pPara = (DMX_Compat_AcqMsg_S *)arg;
            DMX_UserMsg_S stReqBufTmp[DMX_DEFAULT_BUF_NUM];
            DMX_UserMsg_S  *pstBufTmp, *pstCompatBuf;
            HI_U32 u32AcquireNum = pPara->u32AcquireNum;
            
            if (u32AcquireNum > DMX_DEFAULT_BUF_NUM)
            {
                pstBufTmp = HI_KMALLOC(HI_ID_DEMUX, sizeof(DMX_UserMsg_S) * u32AcquireNum, GFP_KERNEL);
                if (NULL == pstBufTmp)
                {
                    HI_FATAL_DEMUX("malloc failed.\n");
                    ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
                    break;
                }
            }
            else
            {
                pstBufTmp = stReqBufTmp;
            }

            ret = HI_DRV_DMX_AcquireBuf(pPara->hChannel, pPara->u32AcquireNum,
                                    &pPara->u32AcquiredNum, pstBufTmp, pPara->u32TimeOutMs);

            if (ret == HI_SUCCESS && pPara->u32AcquiredNum)
            {
                HI_U32 i;

                /* resue DMX_UserMsg_S for which has same binary layout with HI_UNF_DMX_DATA_S in compat mode. */
                pstCompatBuf = (DMX_UserMsg_S*)compat_ptr(pPara->pstBuf);
                
                for (i = 0; i < pPara->u32AcquiredNum; i++)
                {
                    put_user(pstBufTmp[i].u32BufStartAddr, &pstCompatBuf[i].u32BufStartAddr);
                    put_user(pstBufTmp[i].u32MsgLen, &pstCompatBuf[i].u32MsgLen);
                    put_user(pstBufTmp[i].enDataType, &pstCompatBuf[i].enDataType);
                }
            }
            
            if (u32AcquireNum > DMX_DEFAULT_BUF_NUM)
            {
                HI_KFREE(HI_ID_DEMUX, pstBufTmp);
            }

            break;
        }

        case CMD_DEMUX_COMPAT_RELEASE_MSG:
        {
            DMX_Compat_RelMsg_S *pPara = (DMX_Compat_RelMsg_S *)arg;
            DMX_UserMsg_S *pstBufTmp, *pstCompatBuf;
            DMX_UserMsg_S stReqBufTmp[DMX_DEFAULT_BUF_NUM];
            HI_U32 u32ReleaseNum;
            HI_U32 i;

            u32ReleaseNum = pPara->u32ReleaseNum;
            if (u32ReleaseNum > DMX_DEFAULT_BUF_NUM)
            {
                pstBufTmp = HI_KMALLOC(HI_ID_DEMUX, sizeof(DMX_UserMsg_S) * u32ReleaseNum, GFP_KERNEL);
            }
            else
            {
                pstBufTmp = stReqBufTmp;
            }

            if (NULL == pstBufTmp)
            {
                HI_FATAL_DEMUX("malloc failed u32ReleaseNum:%x.\n", u32ReleaseNum);
                ret =  HI_ERR_DMX_ALLOC_MEM_FAILED;
                break;
            }

            /* resue DMX_UserMsg_S for which has same binary layout with HI_UNF_DMX_DATA_S in compat mode. */
            pstCompatBuf = (DMX_UserMsg_S*)compat_ptr(pPara->pstBuf);
            for (i = 0; i < u32ReleaseNum; i++)
            {
                get_user(pstBufTmp[i].u32BufStartAddr, &pstCompatBuf[i].u32BufStartAddr);
                get_user(pstBufTmp[i].u32MsgLen, &pstCompatBuf[i].u32MsgLen);
            }
            
            ret = HI_DRV_DMX_ReleaseBuf(pPara->hChannel, u32ReleaseNum, pstBufTmp);
            
            if (u32ReleaseNum > DMX_DEFAULT_BUF_NUM)
            {
                HI_KFREE(HI_ID_DEMUX, pstBufTmp);
            }

            break;
        }

        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }

    }

    return ret;
}
#endif

static HI_S32 DMXPcrIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_PCR_NEW:
        {
            DMX_NewPcr_S *pPara = (DMX_NewPcr_S *)arg;
            
            ret = HI_DRV_DMX_CreatePcrChannel(pPara->u32DmxId, &pPara->u32PcrId, (HI_VOID *)file);
            
            break;
        }

        case CMD_DEMUX_PCR_DEL:
        {
            ret = HI_DRV_DMX_DestroyPcrChannel(*(HI_U32*)arg);
            
            break;
        }

        case CMD_DEMUX_PCRPID_SET:
        {
            DMX_PcrPidSet_S *pPara = (DMX_PcrPidSet_S *)arg;
            
            ret = HI_DRV_DMX_PcrPidSet(pPara->pu32PcrChId, pPara->u32Pid);
            
            break;
        }

        case CMD_DEMUX_PCRPID_GET:
        {
            DMX_PcrPidGet_S *pPara = (DMX_PcrPidGet_S *)arg;
            
            ret = HI_DRV_DMX_PcrPidGet(pPara->pu32PcrChId, &pPara->u32Pid);
            
            break;
        }

        /* not support PCR user mode get, presently */
        case CMD_DEMUX_CURPCR_GET:
        {
            DMX_PcrScrGet_S *pPara = (DMX_PcrScrGet_S *)arg;
            
            ret = HI_DRV_DMX_PcrScrGet(pPara->pu32PcrChId, &pPara->u64PcrValue, &pPara->u64ScrValue);
            
            break;
        }

        case CMD_DEMUX_PCRSYN_ATTACH:
        {
            DMX_PCRSYNC_S *pPara = (DMX_PCRSYNC_S *)arg;
            
            ret = HI_DRV_DMX_PcrSyncAttach(pPara->u32PcrChId, pPara->u32SyncHandle);
            
            break;
        }

        case CMD_DEMUX_PCRSYN_DETACH:
        {
            DMX_PCRSYNC_S *pPara = (DMX_PCRSYNC_S *)arg;
            
            ret = HI_DRV_DMX_PcrSyncDetach(pPara->u32PcrChId);
            
            break;
        }

        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXAvIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_PES_BUFFER_GETSTAT:
        {
            DMX_PesBufStaGet_S *pPara = (DMX_PesBufStaGet_S *)arg;
            
            ret = HI_DRV_DMX_GetPESBufferStatus(pPara->hChannel, &pPara->stBufStat);
            
            break;
        }

        case CMD_DEMUX_ES_BUFFER_GET:
        {
            DMX_PesBufGet_S *pPara = (DMX_PesBufGet_S *)arg;
            DMX_Stream_S stEsBuf = {0};
            stEsBuf.u32BufLen = 0;
            stEsBuf.u32PtsMs = INVALID_PTS;
            
            ret = HI_DRV_DMX_AcquireEs(pPara->hChannel, &stEsBuf);
            if (HI_SUCCESS == ret)
            {
                pPara->BufPhyAddr = stEsBuf.u32BufPhyAddr;
                pPara->BufSize = stEsBuf.u32BufLen;
                pPara->PtsMs = stEsBuf.u32PtsMs;
            }

            break;
        }

        case CMD_DEMUX_ES_BUFFER_PUT:
        {
            DMX_PesBufGet_S *pPara = (DMX_PesBufGet_S *)arg;
            DMX_Stream_S stEsBuf = {0};

            stEsBuf.u32BufPhyAddr = pPara->BufPhyAddr;
            stEsBuf.u32BufLen = pPara->BufSize;
            stEsBuf.u32PtsMs = pPara->PtsMs;

            ret = HI_DRV_DMX_ReleaseEs(pPara->hChannel, &stEsBuf);
            break;
        }

        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXRecIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_REC_CHAN_CREATE :
        {
            DMX_Rec_CreateChan_S *Param = (DMX_Rec_CreateChan_S*)arg;
            DMX_MMZ_BUF_S RecBuf = {0};

            ret = HI_DRV_DMX_CreateRecChn(
                    &Param->RecAttr,
                    &Param->RecHandle,
                    &RecBuf,
                    (HI_VOID *)file
                );

            Param->RecBufPhyAddr = RecBuf.PhyAddr;
            Param->RecBufSize = RecBuf.Size;
            Param->RecBufFlag  = RecBuf.Flag;
            
            break;
        }

        case CMD_DEMUX_REC_CHAN_DESTROY :
        {
            ret = HI_DRV_DMX_DestroyRecChn(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_REC_CHAN_ADD_PID :
        {
            DMX_Rec_AddPid_S *Param = (DMX_Rec_AddPid_S*)arg;

            ret = HI_DRV_DMX_AddRecPid(Param->RecHandle, Param->Pid, &Param->ChanHandle, (HI_VOID *)file);

            break;
        }

        case CMD_DEMUX_REC_CHAN_DEL_PID :
        {
            DMX_Rec_DelPid_S *Param = (DMX_Rec_DelPid_S*)arg;

            ret = HI_DRV_DMX_DelRecPid(Param->RecHandle, Param->ChanHandle);

            break;
        }

        case CMD_DEMUX_REC_CHAN_DEL_ALL_PID :
        {
            ret = HI_DRV_DMX_DelAllRecPid(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_REC_CHAN_ADD_EXCLUDE_PID :
        {
            DMX_Rec_ExcludePid_S *Param = (DMX_Rec_ExcludePid_S*)arg;

            ret = HI_DRV_DMX_AddExcludeRecPid(Param->RecHandle, Param->Pid);

            break;
        }

        case CMD_DEMUX_REC_CHAN_DEL_EXCLUDE_PID :
        {
            DMX_Rec_ExcludePid_S *Param = (DMX_Rec_ExcludePid_S*)arg;

            ret = HI_DRV_DMX_DelExcludeRecPid(Param->RecHandle, Param->Pid);

            break;
        }

        case CMD_DEMUX_REC_CHAN_CANCEL_EXCLUDE :
        {
            ret = HI_DRV_DMX_DelAllExcludeRecPid(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_REC_CHAN_START :
        {
            ret = HI_DRV_DMX_StartRecChn(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_REC_CHAN_STOP :
        {
            ret = HI_DRV_DMX_StopRecChn(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_REC_CHAN_ACQUIRE_DATA :
        {
            DMX_Rec_AcquireData_S *Param = (DMX_Rec_AcquireData_S*)arg;
            HI_MPI_DMX_REC_DATA_S RecData = {0};

            ret = HI_DRV_DMX_AcquireRecData(Param->RecHandle, &RecData, Param->TimeoutMs);

            Param->BufPhyAddr = RecData.u32DataPhyAddr;
            Param->BufSize = RecData.u32Len;

            break;
        }

        case CMD_DEMUX_REC_CHAN_RELEASE_DATA :
        {
            DMX_Rec_ReleaseData_S *Param = (DMX_Rec_ReleaseData_S*)arg;
            HI_MPI_DMX_REC_DATA_S RecData = {0};

            RecData.u32DataPhyAddr = Param->BufPhyAddr;
            RecData.u32Len = Param->BufSize;

            ret = HI_DRV_DMX_ReleaseRecData(Param->RecHandle, &RecData);

            break;
        }

        case CMD_DEMUX_REC_CHAN_ACQUIRE_INDEX :
        {
            DMX_Rec_AcquireIndex_S *Param = (DMX_Rec_AcquireIndex_S*)arg;

            ret = HI_DRV_DMX_AcquireRecIndex(Param->RecHandle, &Param->IndexData, Param->TimeoutMs);

            break;
        }

        case CMD_DEMUX_REC_CHAN_ACQUIRE_DATA_INDEX :
        {
            DMX_Rec_ProcessDataIndex_S *Param = (DMX_Rec_ProcessDataIndex_S*)arg;
            HI_MPI_DMX_REC_DATA_INDEX_S *RecDataIdx = HI_NULL;

            BUG_ON(!Param->RecDataIdx);

            RecDataIdx = HI_KMALLOC(HI_ID_DEMUX, sizeof(HI_MPI_DMX_REC_DATA_INDEX_S), GFP_KERNEL);
            if (!RecDataIdx)
            {
                ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
                break;
            }

            ret = HI_DRV_DMX_AcquireRecDataIndex(Param->RecHandle, RecDataIdx);
            if (HI_SUCCESS == ret)
            {
                if (copy_to_user(Param->RecDataIdx, RecDataIdx, sizeof(HI_MPI_DMX_REC_DATA_INDEX_S)))
                {
                    BUG();
                }
            }

            HI_KFREE(HI_ID_DEMUX, RecDataIdx);
            
            break;
        }
        
        case CMD_DEMUX_REC_CHAN_RELEASE_DATA_INDEX :
        {
            DMX_Rec_ProcessDataIndex_S *Param = (DMX_Rec_ProcessDataIndex_S*)arg;
            HI_MPI_DMX_REC_DATA_INDEX_S *RecDataIdx = HI_NULL;

            BUG_ON(!Param->RecDataIdx);

            RecDataIdx = HI_KMALLOC(HI_ID_DEMUX, sizeof(HI_MPI_DMX_REC_DATA_INDEX_S), GFP_KERNEL);
            if (!RecDataIdx)
            {
                ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
                break;
            }

            if (copy_from_user(RecDataIdx, Param->RecDataIdx, sizeof(HI_MPI_DMX_REC_DATA_INDEX_S)))
            {
                BUG();
            }
            
            ret = HI_DRV_DMX_ReleaseRecDataIndex(Param->RecHandle, RecDataIdx);

            HI_KFREE(HI_ID_DEMUX, RecDataIdx);

            break;
        }

        case CMD_DEMUX_REC_CHAN_GET_BUF_STATUS :
        {
            DMX_Rec_BufStatus_S *Param = (DMX_Rec_BufStatus_S*)arg;

            ret = HI_DRV_DMX_GetRecBufferStatus(Param->RecHandle, &Param->BufStatus);

            break;
        }

        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXRmxIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch(cmd)
    {
        case CMD_REMUX_CREATE:
        {
            RMX_Create_S *Param = (RMX_Create_S *)arg;

            ret = HI_DRV_RMX_Create(&Param->Attr, &Param->Handle);

            break;
        }

        case CMD_REMUX_DESTROY:
        {
            ret = HI_DRV_RMX_Destroy(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_REMUX_GET_ATTR:
        {
            RMX_Attr_S *Param = (RMX_Attr_S *)arg;

            ret = HI_DRV_RMX_GetAttr(Param->Handle, &Param->Attr);
            
            break;
        }

        case CMD_REMUX_SET_ATTR:
        {
            RMX_Attr_S *Param = (RMX_Attr_S *)arg;

            ret = HI_DRV_RMX_SetAttr(Param->Handle, &Param->Attr);
            
            break;
        }

        case CMD_REMUX_START:
        {
            ret = HI_DRV_RMX_Start(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_REMUX_STOP:
        {
            ret = HI_DRV_RMX_Stop(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_REMUX_ADD_PUMP:
        {
            RMX_Add_Pump_S *Param = (RMX_Add_Pump_S *)arg;

            ret = HI_DRV_RMX_AddPump(Param->RmxHandle, &Param->Attr, &Param->PumpHandle);

            break;
        }

        case CMD_REMUX_DEL_PUMP:
        {
            ret = HI_DRV_RMX_DelPump(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_REMUX_GET_PUMP_DEFAULT_ATTR:
        {
            RMX_Pump_Attr_S *Param = (RMX_Pump_Attr_S *)arg;

            ret = HI_DRV_RMX_GetPumpDefaultAttr(&Param->Attr);

            break;
        }

        case CMD_REMUX_GET_PUMP_ATTR:
        {
            RMX_Pump_Attr_S *Param = (RMX_Pump_Attr_S *)arg;

            ret = HI_DRV_RMX_GetPumpAttr(Param->PumpHandle, &Param->Attr);
            
            break;
        }

        case CMD_REMUX_SET_PUMP_ATTR:
        {
            RMX_Pump_Attr_S *Param = (RMX_Pump_Attr_S *)arg;

            ret = HI_DRV_RMX_SetPumpAttr(Param->PumpHandle, &Param->Attr);
            
            break;
        }
        
        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

#ifdef CONFIG_COMPAT  
static HI_S32 Compat_DMXRecIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_COMPAT_REC_CHAN_ACQUIRE_DATA_INDEX :
        {
            DMX_Compat_Rec_ProcessDataIndex_S *Param = (DMX_Compat_Rec_ProcessDataIndex_S*)arg;
            DMX_Compat_REC_DATA_INDEX_S *CompatRecDataidx = (DMX_Compat_REC_DATA_INDEX_S *)compat_ptr(Param->RecDataIdx);
            HI_MPI_DMX_REC_DATA_INDEX_S *RecDataIdx = HI_NULL;
            HI_U32 index;
                        
            RecDataIdx = HI_KMALLOC(HI_ID_DEMUX, sizeof(HI_MPI_DMX_REC_DATA_INDEX_S), GFP_KERNEL);
            if (!RecDataIdx)
            {
                ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
                break;
            }

            ret = HI_DRV_DMX_AcquireRecDataIndex(Param->RecHandle, RecDataIdx);
            if (HI_SUCCESS == ret)
            {
                put_user(RecDataIdx->u32IdxNum, &CompatRecDataidx->u32IdxNum);
                put_user(RecDataIdx->u32RecDataCnt, &CompatRecDataidx->u32RecDataCnt);
                
                if (copy_to_user(CompatRecDataidx->stIndex, RecDataIdx->stIndex, RecDataIdx->u32IdxNum * sizeof(HI_MPI_DMX_REC_INDEX_S)))
                {
                    BUG();
                }
                
                for (index = 0; index < RecDataIdx->u32RecDataCnt; index ++)
                {
                    put_user(RecDataIdx->stRecData[index].u32DataPhyAddr, &CompatRecDataidx->stRecData[index].u32DataPhyAddr);
                    put_user(RecDataIdx->stRecData[index].u32Len, &CompatRecDataidx->stRecData[index].u32Len);
                }
            }

            HI_KFREE(HI_ID_DEMUX, RecDataIdx);
            
            break;
        }
        
        case CMD_DEMUX_COMPAT_REC_CHAN_RELEASE_DATA_INDEX :
        {
            DMX_Compat_Rec_ProcessDataIndex_S *Param = (DMX_Compat_Rec_ProcessDataIndex_S*)arg;
            DMX_Compat_REC_DATA_INDEX_S *CompatRecDataidx = (DMX_Compat_REC_DATA_INDEX_S *)compat_ptr(Param->RecDataIdx);
            HI_MPI_DMX_REC_DATA_INDEX_S *RecDataIdx = HI_NULL;
            HI_U32 index;

            RecDataIdx = HI_KMALLOC(HI_ID_DEMUX, sizeof(HI_MPI_DMX_REC_DATA_INDEX_S), GFP_KERNEL);
            if (!RecDataIdx)
            {
                ret = HI_ERR_DMX_ALLOC_MEM_FAILED;
                break;
            }

            get_user(RecDataIdx->u32IdxNum, &CompatRecDataidx->u32IdxNum);
            get_user(RecDataIdx->u32RecDataCnt, &CompatRecDataidx->u32RecDataCnt);
            
            if (copy_from_user(RecDataIdx->stIndex, CompatRecDataidx->stIndex, RecDataIdx->u32IdxNum * sizeof(HI_MPI_DMX_REC_INDEX_S)))
            {
                BUG();
            }

            for (index = 0; index < RecDataIdx->u32RecDataCnt; index ++)
            {
                 get_user(RecDataIdx->stRecData[index].u32DataPhyAddr, &CompatRecDataidx->stRecData[index].u32DataPhyAddr);
                 get_user(RecDataIdx->stRecData[index].u32Len, &CompatRecDataidx->stRecData[index].u32Len);
            }

            ret = HI_DRV_DMX_ReleaseRecDataIndex(Param->RecHandle, RecDataIdx);

            HI_KFREE(HI_ID_DEMUX, RecDataIdx);
            
            break;
        }
        
        default:
            ret = DMXRecIoctl(file, cmd, arg);
    }

    return ret;
}
#endif

HI_S32 DMX_OsrIoctl(struct inode *inode, struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32  ret     = HI_FAILURE;
    HI_U32  CmdType = cmd & DEMUX_CMD_MASK;

    switch (CmdType)
    {
        case DEMUX_GLOBAL_CMD:
        {
            ret = DMXGlobalIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_PORT_CMD:
        {
            ret = DMXPortIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_TSBUFFER_CMD:
        {
            ret = DMXTsBufferIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_CHAN_CMD:
        {
            ret = DMXChanIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_FILT_CMD:
        {
            ret = DMXFiltIoctl(file, cmd, arg);

            break;
        }

#ifdef DMX_DESCRAMBLER_SUPPORT
        case DEMUX_KSET_CMD:
        {
            ret = DMXKeyIoctl(file, cmd, arg);

            break;
        }
#endif

        case DEMUX_RECV_CMD:
        {
            ret = DMXRecvIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_PCR_CMD:
        {
            ret = DMXPcrIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_AV_CMD:
        {
            ret = DMXAvIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_REC_CMD:
        {
            ret = DMXRecIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_RMX_CMD:
        {
            ret = DMXRmxIoctl(file, cmd, arg);

            break;
        }

        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

#ifdef CONFIG_COMPAT  
HI_S32 DMX_Compat_OsrIoctl(struct inode *inode, struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32  ret     = HI_FAILURE;
    HI_U32  CmdType = cmd & DEMUX_CMD_MASK;

    switch (CmdType)
    {
        case DEMUX_GLOBAL_CMD:
        {
            ret = DMXGlobalIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_PORT_CMD:
        {
            ret = DMXPortIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_TSBUFFER_CMD:
        {
            ret = DMXTsBufferIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_CHAN_CMD:
        {
            ret = DMXChanIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_FILT_CMD:
        {
            ret = DMXFiltIoctl(file, cmd, arg);

            break;
        }

#ifdef DMX_DESCRAMBLER_SUPPORT
        case DEMUX_KSET_CMD:
        {
            ret = DMXKeyIoctl(file, cmd, arg);

            break;
        }
#endif

        case DEMUX_RECV_CMD:
        {
            ret = Compat_DMXRecvIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_PCR_CMD:
        {
            ret = DMXPcrIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_AV_CMD:
        {
            ret = DMXAvIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_REC_CMD:
        {
            ret = Compat_DMXRecIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_RMX_CMD:
        {
            ret = DMXRmxIoctl(file, cmd, arg);

            break;
        }

        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}
#endif

/*****************************************************************************
 Prototype    : DMX_DRV_Open
 Description  : open function in DEMUX module
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
static HI_S32 DMX_DRV_Open(struct inode * inode, struct file * file)
{  
    HI_DRV_DMX_Open((HI_VOID*)file);

    return 0;
}

/*****************************************************************************
 Prototype    : DMX_DRV_Release
 Description  : release function in DEMUX module
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
static HI_S32 DMX_DRV_Release(struct inode * inode, struct file * file)
{
    HI_DRV_DMX_Close(file);

    return 0;
}

/*****************************************************************************
 Prototype    : DMX_DRV_Ioctl
 Description  : Ioctl function in DEMUX module
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
static long DMX_DRV_Ioctl(struct file *file, HI_U32 cmd, unsigned long arg)
{
    return (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, DMX_OsrIoctl);
}

#ifdef CONFIG_COMPAT  
static long DMX_DRV_Compat_Ioctl(struct file *file, HI_U32 cmd, unsigned long arg)
{
    return (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, DMX_Compat_OsrIoctl);
}
#endif

static struct file_operations DMX_DRV_Fops =
{
    .owner          = THIS_MODULE,
    .open           = DMX_DRV_Open,
    .unlocked_ioctl = DMX_DRV_Ioctl,
#ifdef CONFIG_COMPAT    
    .compat_ioctl = DMX_DRV_Compat_Ioctl,
#endif    
    .release        = DMX_DRV_Release,
};

static PM_BASEOPS_S dmx_drvops =
{
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = DMX_OsiSuspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = DMX_OsiResume,
};

/*****************************************************************************
 Prototype    : DMX_DRV_ModInit
 Description  : initialize function in DEMUX module
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
HI_S32 DMX_DRV_ModInit(HI_VOID)
{
#ifdef HI_DEMUX_PROC_SUPPORT
    DRV_PROC_ITEM_S *item;
    DRV_PROC_EX_S stFnOpt = {0};
#endif

#ifndef HI_MCE_SUPPORT
    if (HI_SUCCESS != HI_DRV_DMX_BasicInit())
    {
        goto out;
    }
#endif

    strncpy(g_stDemuxDev.devfs_name, UMAP_DEVNAME_DEMUX, strlen(UMAP_DEVNAME_DEMUX));
    g_stDemuxDev.devfs_name[strlen(UMAP_DEVNAME_DEMUX)] = '\0';
    g_stDemuxDev.fops   = &DMX_DRV_Fops;
    g_stDemuxDev.minor  = UMAP_MIN_MINOR_DEMUX;
    g_stDemuxDev.owner  = THIS_MODULE;
    g_stDemuxDev.drvops = &dmx_drvops;

    if (HI_DRV_DEV_Register(&g_stDemuxDev) < 0)
    {
        HI_FATAL_DEMUX("Unable to register demux dev\n");
        goto out;
    }

#ifdef HI_DEMUX_PROC_SUPPORT
    item = HI_DRV_PROC_AddModule("demux_main", NULL, NULL);
    if (!item)
    {
        HI_ERR_DEMUX("add proc demux_main failed\n");
    }
    else
    {
        item->read  = DMXProcRead;
        item->write = DMXProcWrite;
    }

    stFnOpt.fnRead = DMXPortProcRead;
    item = HI_DRV_PROC_AddModule("demux_port", &stFnOpt, NULL);
    if (!item)
    {
        HI_ERR_DEMUX("add proc demux_port failed\n");
    }

    stFnOpt.fnRead = DMXChanProcRead;
    item = HI_DRV_PROC_AddModule("demux_chan", &stFnOpt, NULL);
    if (!item)
    {
        HI_ERR_DEMUX("add proc demux_chan failed\n");
    }

    stFnOpt.fnRead = DMXChanBufProcRead;
    item = HI_DRV_PROC_AddModule("demux_chanbuf", &stFnOpt, NULL);
    if (!item)
    {
        HI_ERR_DEMUX("add proc demux_chanbuf failed\n");
    }

    stFnOpt.fnRead = DMXFilterProcRead;
    item = HI_DRV_PROC_AddModule("demux_filter", &stFnOpt, NULL);
    if (!item)
    {
        HI_ERR_DEMUX("add proc demux_filter failed\n");
    }


    stFnOpt.fnRead = DMXPcrProcRead;
    item = HI_DRV_PROC_AddModule("demux_pcr", &stFnOpt, NULL);
    if (!item)
    {
        HI_ERR_DEMUX("add proc demux_pcr failed\n");
    }

    stFnOpt.fnRead = DMXRecProcRead;
    item = HI_DRV_PROC_AddModule("demux_rec", &stFnOpt, NULL);
    if (!item)
    {
        HI_ERR_DEMUX("add proc demux_rec failed\n");
    }

    stFnOpt.fnRead = DMXRecScdProcRead;
    item = HI_DRV_PROC_AddModule("demux_rec_index", &stFnOpt, NULL);
    if (!item)
    {
        HI_ERR_DEMUX("add proc demux_rec_index failed\n");
    }

#ifdef RMX_SUPPORT
    stFnOpt.fnRead = DMXRmxProcRead;
    item = HI_DRV_PROC_AddModule("demux_rmx", &stFnOpt, NULL);
    if (!item)
    {
        HI_ERR_DEMUX("add proc demux_rmx failed\n");
    }
#endif

#endif

#ifdef MODULE
    HI_PRINT("Load hi_demux.ko success.  \t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;

out :
    DMX_DRV_ModExit();

    return HI_FAILURE;
}

/*****************************************************************************
 Prototype    : DMX_DRV_ModExit
 Description  : exit function in DEMUX module
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
HI_VOID DMX_DRV_ModExit(HI_VOID)
{
#ifdef HI_DEMUX_PROC_SUPPORT
#ifdef RMX_SUPPORT
    HI_DRV_PROC_RemoveModule("demux_rmx");
#endif
    HI_DRV_PROC_RemoveModule("demux_rec_index");
    HI_DRV_PROC_RemoveModule("demux_rec");
    HI_DRV_PROC_RemoveModule("demux_pcr");


    HI_DRV_PROC_RemoveModule("demux_filter");
    HI_DRV_PROC_RemoveModule("demux_chanbuf");
    HI_DRV_PROC_RemoveModule("demux_chan");
    HI_DRV_PROC_RemoveModule("demux_port");
    HI_DRV_PROC_RemoveModule("demux_main");
#endif

    HI_DRV_DEV_UnRegister(&g_stDemuxDev);

#ifndef HI_MCE_SUPPORT
    HI_DRV_DMX_DeInit();
#endif
}

#ifdef MODULE
module_init(DMX_DRV_ModInit);
module_exit(DMX_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");

