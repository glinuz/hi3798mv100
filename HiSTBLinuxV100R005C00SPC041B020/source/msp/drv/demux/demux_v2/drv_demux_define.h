/*******************************************************************************
*             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: drv_demux_define.h
* Description: Register define for HiPVRV200
*
* History:
* Version   Date              Author    DefectNum       Description
* main\1    2009-09-27    y00106256     NULL            create this file.
*******************************************************************************/
#ifndef __DRV_DEMUX_DEFINE_H__
#define __DRV_DEMUX_DEFINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <linux/list.h>
#include <linux/wait.h>

#include "hi_type.h"

#include "drv_demux_config.h"
#include "drv_demux_ext.h"

#include "hi_drv_mmz.h"
#include "hi_drv_proc.h"

#include "hi_unf_common.h"
#include "hi_unf_demux.h"
#include "hi_mpi_demux.h"

#define DMX_FQ_COM_BLKSIZE              (8 * 1024)
#define DMX_PES_MAX_SIZE                ((64 * 1024) / DMX_FQ_COM_BLKSIZE)

#define DMX_MAX_FILTER_NUM_PER_CHANNEL  32      //the FILTER number in per channel

#define DMX_PER_DESC_LEN                0x10
#define DMX_MAX_ERRLIST_NUM             0x100

#define DMX_CHANNEL_REGION_NUM          3
#define DMX_CHANNEL_NUM_PER_REGION      32

#define DMX_FQ_REGION_NUM               2
#define DMX_FQ_NUM_PER_REGION           32

#define DMX_OQ_REGION_NUM               4
#define DMX_OQ_NUM_PER_REGION           32

#define DMX_OQ_RSV_OFFSET   0
#define DMX_OQ_CTRL_OFFSET  1
#define DMX_OQ_EOPWR_OFFSET 2
#define DMX_OQ_SZUS_OFFSET  3
#define DMX_OQ_SADDR_OFFSET 4
#define DMX_OQ_RDWR_OFFSET  5
#define DMX_OQ_CFG_OFFSET   6
#define DMX_OQ_START_OFFSET 7

#define DMX_FQ_CTRL_OFFSET  0
#define DMX_FQ_RDVD_OFFSET  1
#define DMX_FQ_SZWR_OFFSET  2
#define DMX_FQ_START_OFFSET 3
#define DMX_FQ_ALOVF_CNT    4

#define DMX_FQ_COMMOM       0x0
#define DMX_FQ_AV_BASE      8
#define DMX_FQ_REC_BASE     2
#define DMX_FQ_SCD_BASE     5
#define DMX_OQ_DEPTH        0x3ff
#define DMX_OQ_ALOVF_CNT    0x4//almost overflow level
#define DMX_OQ_OUTINT_CNT   0x1//OQ queue continues output buf block to this number will cuase break
#define DMX_OQ_START_CNT    0x30//OQ queue continues output minimal free block count
#define DMX_OQ_STOP_PERSENT  80 //channel pid disable persent,if oq use dscs more the 80% driver will stop it
#define DMX_OQ_START_PERSENT 60//channel pid enable persent,if oq use dscs less the 60% driver will restart it
#define DMX_OQ_SOFT_ALOVF   (DMX_OQ_ALOVF_CNT*2)//software almost overflow level
#define DMX_MAX_AVFQ_DESC   0x3f0

#define DMX_SHIFT_16BIT     16
#define DMX_SHIFT_8BIT      8

#define DMX_INVALID_PORT_ID             0xffffffff

#define DMX_DEFAULT_TAG_LENGTH      (4)

#define DMX_INVALID_DEMUX_ID            0xffffffff
#define DMX_INVALID_REC_ID            0xffffffff
#define DMX_INVALID_SYNC_HANDLE         0xffffffff
#define DMX_INVALID_CHAN_ID             0xffff
#define DMX_INVALID_FILTER_ID           0xffff
#define DMX_INVALID_KEY_ID              0xffff
#define DMX_INVALID_PID                 0x1fff
#define DMX_INVALID_OQ_ID              0xffff
#define DMX_INVALID_FQ_ID               0xffff
#define DMX_INVALID_SCD_ID             0xffff
#define DMX_INVALID_REMUX_ID        0xffffffff

#define DMX_MIN_TS_BUFFER_SIZE          0x1000
#define DMX_MAX_TS_BUFFER_SIZE          0x1000000
#define DMX_TS_BUFFER_GAP               0x100

#define DMX_DISABLE                     0x0
#define DMX_ENABLE                      0x1

#define DMX_MAX_SEC_LEN                 0x1000

#define SECTION_LENGTH_FIELD_SIZE       2

#define DMX_MAX_IP_DESC_DEPTH           0xffff
#define DMX_MIN_IP_DESC_DEPTH           0x3ff
#define DMX_MAX_BLOCK_SIZE              (0xf000) /* < 0xffff */

#define DMX_MAX_LOST_TH                 0x3
#define DMX_MAX_LOCK_TH                 0x7
#define DMX_MAX_FLUSH_WAIT              0x1000

#define DMX_RAM_PORT_MIN_LEN            188
#define DMX_RAM_PORT_MAX_LEN            255

#define DMX_TS_PACKET_LEN               188
#define DMX_TS_PACKET_LEN_204           204
#define DMX_SCD_PACKET_LEN              28

#define DMX_PORT_OFFSET                 1
#define DMX_DEFAULT_INT_CNT             1

#define DMX_RAM_PORT_OFFSET             15

#define DMX_TAG_PORT_OFFSET 0x20

#define RMX_PORT_OFFSET 0x40

#define DMX_PES_HEADER_LENGTH           9
#define DMX_PES_HEADER_AND_PTS_LENGTH   14

#define DMX_DEFAULT_POST_TH             0

/*
 * disable dynamic tune dmx clk: crg64.pvr_dmx_clk_sel * 8 / (DMX_DEFAULT_IP_SPEED + 1 ) = iprate(mbps)
 * enable dynamic tune dmx clk: actual_dmx_clk(offset:0x2320) * 8 / (DMX_DEFAULT_IP_SPEED + 1 ) = iprate(mbps)
*/
#define DMX_DEFAULT_IP_SPEED              9


#define DMX_KEY_HARDONLY_FLAG           0xffffffff

#define DMX_CHECKCHN_TIMEOUT        1000    /* 1s */
#define DMX_RESETCHN_TIME1          2000    /* 2s */
#define DMX_RESETCHN_TIME2          5000    /* 2s */
#define DMX_CHECK_START_PERSENT     80      /* 80% */
#define DMX_CHN_RESET_PERSENT       20      /* 20% */

#define INVALID_PTS                 0xFFFFFFFFL

#define DMX_CSA2_TABLE_ID           0
#define DMX_SPE_TABLE_ID            1
#define DMX_OTHER_TABLE_ID          2

#define DMX_KEY_MIN_LEN             8
#define DMX_KEY_MAX_LEN             16

#define CHECKTAGPORTID(TagPortId) ({                      \
    HI_BOOL bRet = HI_FALSE;                                    \
    if (TagPortId != DMX_INVALID_PORT_ID                  \
        && TagPortId < DMX_TAGPORT_CNT)    \
    {                                                                        \
        bRet = HI_TRUE;                                               \
    }                                                                        \
    bRet;                                                                  \
})

#define CHECKPOINTER(ptr)                                   \
    do                                                      \
    {                                                       \
        if (!(ptr))                                         \
        {                                                   \
            HI_ERR_DEMUX("pointer is null\n");             \
            return HI_ERR_DMX_NULL_PTR;                     \
        }                                                   \
    } while (0)

#define CHECKTSOPORTID(Id)                                \
    do                                                      \
    {                                                       \
        if ((Id) >= DMX_TSOPORT_CNT)                      \
        {                                                   \
            HI_ERR_DEMUX("invalid TSO port %u\n", Id);   \
            return HI_ERR_DMX_INVALID_PARA;                 \
        }                                                   \
    } while (0)

#define DMXINC(a, size) \
    if ((++a) >= (size)) { \
        (a) = 0;\
    } \

typedef enum hiDMX_Ch_AFMode_E
{
    DMX_AF_SEND                   = 0,                         /*send AF data*/
    DMX_AF_DISCARD                = 1,                         /*discard AF data*/
    DMX_UNMEANING_AF_DISCARD      = 2                          /*send meaningful AF data only*/
}DMX_Ch_AFMode_E;

typedef enum hiDMX_Ch_ATTR_E
{
    DMX_CH_GENERAL = 0,
    DMX_CH_GENERAL_NOPL,
    DMX_CH_PES_NOCHECK = 2,
    DMX_CH_PES = 3,
    DMX_CH_AUDIO = 6,
    DMX_CH_VIDEO,
    DMX_CH_UNDEF
}DMX_Ch_ATTR_E;

typedef enum
{
    DMX_FLUSH_TYPE_REC_PLAY = 0x0,
    DMX_FLUSH_TYPE_PLAY     = 0x1,
    DMX_FLUSH_TYPE_REC      = 0x2,
    DMX_FLUSH_TYPE_UNDEF    = 0x3
} DMX_FLUSH_TYPE_E;

typedef enum
{
    DMX_OQ_CLEAR_TYPE_PLAY  = 0,
    DMX_OQ_CLEAR_TYPE_REC   = 1,
    DMX_OQ_CLEAR_TYPE_SCD   = 2,
    DMX_OQ_CLEAR_TYPE_UNDEF
} DMX_OQ_CLEAR_TYPE_E;

typedef enum
{
    DMX_REC_STATUS_STOP,
    DMX_REC_STATUS_START
} DMX_REC_STATUS_E;

typedef enum
{
    RMX_STATUS_STOP,
    RMX_STATUS_START
}RMX_STATUS_E;

typedef enum
{
    DMX_OQ_MODE_UNUSED,
    DMX_OQ_MODE_PLAY,
    DMX_OQ_MODE_REC,
    DMX_OQ_MODE_SCD,
    DMX_OQ_MODE_UNDEF
} DMX_OQ_MODE_E;

/****************************************************************************/

typedef struct
{
    HI_UNF_DMX_PORT_TYPE_E  PortType;
    HI_U32                  SyncLockTh;
    HI_U32                  SyncLostTh;

    /*
    ** whether Tuner input clock inverting or not.
    ** 0: in-phase(default)
    ** 1: inverting
    */
    HI_U32  TunerInClk;
    /* port-line sequence select:
    ** parallel:
    **    0: mean cdata[7] is the significant bit(default)
    **    1: mean cdata[0] is the significant bit
    ** serial:
    **    0: mean cdata[0] is the data line (default)
    **    1: mean cdata[7] is the data line
    */
    HI_U32  BitSelector;
} DMX_TunerPort_Info_S;

typedef struct
{
    HI_U32 u32UsedFlag;
    DMX_UserMsg_S psMsg;
} DMX_ERRMSG_S;

typedef struct
{
    HI_U32         DmxId;
    HI_U32         FilterId;
    HI_U32         ChanId;
    pid_t            Owner;
    struct mutex LockFlt;
    HI_U32         Depth;
    HI_U8       Match[DMX_FILTER_MAX_DEPTH];
    HI_U8       Mask[DMX_FILTER_MAX_DEPTH];
    HI_U8       Negate[DMX_FILTER_MAX_DEPTH];
} DMX_FilterInfo_S;

typedef struct
{
    HI_U32         DmxId;
    HI_U32         KeyId;
    pid_t            Owner;
    HI_U32         CaType;
    struct mutex LockKey;
    HI_U32         CaEntropy;
    HI_U32         DescType;
    HI_U32         KeyLen;

#ifdef HI_DEMUX_PROC_SUPPORT
    HI_U32  ChanCount;
    HI_U32  EvenKey[DMX_KEY_MAX_LEN / sizeof(HI_U32)];
    HI_U32  OddKey[DMX_KEY_MAX_LEN / sizeof(HI_U32)];
    HI_U32  AttachDescCnt;
    HI_U32  DetachDescCnt;
    HI_U32  SetOddKeyCnt;
    HI_U32  SetEvenKeyCnt;
#endif
} DMX_KeyInfo_S;

/*
OQ QUEUE DESCRIBTOR
*/
typedef struct hi_OQ_Desc_t
{
    HI_U32 start_addr;
    HI_U32 cactrl_buflen;/*cactrl(16)+buflen(16)*/
    HI_U32 pvrctrl_datalen;/*rsv(8)+pvrctrl(8)+datalen(16)*/
    HI_U32 carsv;
} OQ_DescInfo_S;

typedef struct
{
    HI_U32               u32OQId;
    DMX_OQ_MODE_E enOQBufMode;
    HI_UNF_DMX_SECURE_MODE_E SecureMode;
    HI_U32               u32AttachId;
    OQ_DescInfo_S    *OqDescSetBase;
    HI_U32               OqDescSetPhy;
    HI_U32               OQDepth;
    HI_U32               u32FQId;
    HI_U32               u32ProcsBlk; /*the current OQ Desc now be processing ,in anther words: OQ read pointer keeped in software.(software is reading the data of the correlative buffer block)*/
    HI_U32               u32ProcsOffset;/*offset of the current OQ BB(buffer block: u32ProcsBlk) now be processing */
    HI_U32               u32ReleaseBlk;
    HI_U32               u32ReleaseOffset;
    atomic_t             EopIntActiveCount;
    HI_U32               OqWakeUp;
    wait_queue_head_t   OqWaitQueue;
    HI_U32               OqWaitCondition;
    struct mutex       LockOq;
} DMX_OQ_Info_S;

/*
Passing decode parameters
*/
typedef struct hi_Disp_Control_t
{
    HI_U32          u32DispTime;
    HI_U32          u32DispEnableFlag;
    HI_U32          u32DispFrameDistance;
    HI_U32          u32DistanceBeforeFirstFrame;
    HI_U32          u32GopNum;
} Disp_Control_t;

typedef struct
{
    HI_U32                          DmxId;
    HI_U32                          ChanId;
    struct mutex                    LockChn;
    HI_U32                          ChanPid;
    pid_t                           Owner;
    atomic_t                        FilterCount;
    HI_U32                          ChanBufSize;
    HI_UNF_DMX_CHAN_TYPE_E          ChanType;
    HI_UNF_DMX_CHAN_CRC_MODE_E      ChanCrcMode;
    HI_UNF_DMX_CHAN_OUTPUT_MODE_E   ChanOutMode;
    HI_UNF_DMX_CHAN_STATUS_E        ChanStatus;
    HI_UNF_DMX_SECURE_MODE_E        ChanSecure;
    HI_U32                          KeyId;
    HI_U32                          ChanOqId;
    HI_U32                          ChanRecOqId;
    atomic_t                        ChanRecShareCnt;   /* main for different rec instance share one same rec channel. */
    struct file                     *SaveEsHandle;
    HI_U32                          u32TotalAcq;
    HI_U32                          u32HitAcq;
    HI_U32                          u32Release;
    HI_U32                          u32PesBlkCnt;           /*Count of PES block alreadly be acquired by user,after finishing acquire a whole PES packet,it will be set 0*/
    HI_U32                          u32PesLength;           /*Current PES packet's len,The value been set when each time a new PES header come and parser it get the length. The value been set zeor when initialize or after inishing acquire a whole PES packet*/
    HI_U32                          u32ProcsOffset;         /* only for pes, The offset of Current PES packet alreadly been acquired, each time after DMXOsiFindPes return ,it will recorded the added datalen . it will be set 0 after finishing acquire a whole PES packet*/
    HI_U32                          LastPts;                /* the last parsed PTS*/
    HI_U32                          u32AcqTime;             /* the last get time */
    HI_U32                          u32AcqTimeInterval;     /* the interval from this time to previous time, in millisecond */
    HI_U32                          u32RelTime;             /* the last release time */
    HI_U32                          u32RelTimeInterval;     /* the interval from this time to previous time in milliseond */
    HI_BOOL                         ChanEosFlag;
    Disp_Control_t                  stLastControl;
    HI_U32                          u32PesLenErr;
    HI_U32                          u32CRCErr;
    HI_U32                          u32TEIErr;
    HI_U32                          u32DropCnt;
    HI_U32                          u32CCDiscErr;
} DMX_ChanInfo_S;

typedef struct
{
    atomic_t RecInstCounter;   /* < DMX_1WAY_REC_CNT */
}DMX_Rec_Limit_S;

typedef struct
{
    HI_U32                      DmxId;
    HI_U32                      PortId;
    DMX_PORT_MODE_E             PortMode;
    DMX_Rec_Limit_S             RecLimit;
} DMX_DmxInfo_S;

typedef struct
{
    HI_UNF_DMX_TAG_SYNC_MODE_E enSyncMod;
    HI_U32  u32TagLen;
    HI_UNF_DMX_PORT_E  enTSSource;  /* dvb port id */
    HI_UNF_DMX_TAG_ATTR_S  TagPortAttrs[DMX_TAGPORT_CNT];
}DMX_TagPort_Info_S;


#define FREE_BLK_FLAG  (0x0)
#define ALLOC_BLK_FLAG (0xab)
#define ADD_INT_FLAG (0x17)
#define ADD_RELCAIM_INT_FLAG (0x27)
#define INVALID_HEAD_BLK_IDX (0xffff)

typedef struct
{
    HI_UNF_DMX_PORT_TYPE_E   PortType;
    pid_t                    Owner;
    HI_U32                   SyncLockTh;
    HI_U32                   SyncLostTh;
    HI_U32                   MinLen;
    HI_U32                   MaxLen;
    HI_UNF_DMX_SECURE_MODE_E SecureMode;

#ifdef DMX_TS_BUF_VERSION_2
    HI_U32     TotalBufBlkNR;
    atomic_t   FreeBufBlkNR;
    atomic_t   LastDescWriteIdx;
    atomic_t   LastDescReadIdx;
    struct {
        HI_U8   BlkFlag;  /* include alloc flag and debug flag ... */
        HI_U8   BlkRef;   /* blk put reference */
        HI_U16 HeadBlkIdx; /* point to the buff's head blk idx which this blk belong to */
        HI_U32 ReqLen;
    } *pstBufBlkMap;
    DMX_DATA_BUF_S   LastGetBufInfo;   /* save last geted buf info for Compatible with HI_DRV_DMX_PutTSBuffer */
#else
    HI_U32                  Read;       /*Ts buffer read (offset )pointer, update in DMXOsiIsr, whtn get IP desc out int*/
    HI_U32                  Write;      /*Ts buffer write pointer,update in DMX_OsiTsBufferPut*/

    HI_U32                  ReqAddr;    // physical address
    HI_U32                  ReqLen;
#endif

    HI_U32                  DescPhyAddr;
    HI_U8                   *DescKerAddr;
    HI_U32                  DescDepth;
    HI_U32                  DescWrite;
    HI_U32                 *DescCurrAddr;

    HI_U32                  PhyAddr;
    HI_U8                    *KerAddr;
    HI_U32                  BufSize;
    DMX_BUF_FLAG_E  BufFlag;

    HI_U32                  WaitLen;
    HI_BOOL                 WakeUp;
    wait_queue_head_t       WaitQueue;

    spinlock_t              LockRamPort;

    HI_U32                  GetCount;
    HI_U32                  GetValidCount;
    HI_U32                  PutCount;

    HI_U32                  IpRate;
} DMX_RamPort_Info_S;

typedef struct
{
    HI_U32 start_addr;
    HI_U32 buflen;/*rsv(16)+buflen(16)*/
} FQ_DescInfo_S;

typedef struct
{
    HI_U8               *BufVirAddr;
    HI_U32              BufPhyAddr;
    HI_U32              BufSize;
    DMX_BUF_FLAG_E      BufFlag;
    HI_U8               *ShadowBufVirAddr;
    HI_U32              ShadowBufPhyAddr;
    HI_U32              ShadowBufSize;
    DMX_BUF_FLAG_E      ShadowBufFlag;
    HI_U32              BlockSize;
    FQ_DescInfo_S       *FqDescSetBase;
    HI_U32              FqDescSetPhy;
    HI_U32              FQDepth;
    HI_U32              FqOverflowCount;
    spinlock_t          LockFq;
} DMX_FQ_Info_S;

typedef struct
{
    HI_U32             Reserved;   /* solve division by zero warnning */
    spinlock_t         LockScd;
}DMX_SCD_Info_S;

typedef struct
{
    HI_U32  DmxId;
    HI_U32  PcrPid;
    pid_t     Owner;
    HI_U32  SyncHandle;
    HI_U64  PcrValue;
    HI_U64  ScrValue;

    struct mutex LockPcr;
} DMX_PCR_Info_S;

/**recorded Ts time stamp mode*/
/**CNcomment: 录制TS包时间戳添加模式*/
typedef enum hiDMX_REC_TIMESTAMP_MODE_E
{
    DMX_REC_TIMESTAMP_NONE,               /**<No time stamp added before each recoreded  ts packet*/  /**<CNcomment: 不在每个录制的TS 包前加时间戳 */
    DMX_REC_TIMESTAMP_ZERO,                 /**<Use 4 byte 0  added before each recoreded  ts packet*/  /**<CNcomment: 在每个录制的TS 包前加4字节时间戳，内容为0 */
    DMX_REC_TIMESTAMP_HIGH32BIT_SCR,      /**<Use high 32 bit of SCR_base (4 byte)  added before each recoreded  ts packet*/  /**<CNcomment: 在每个录制的TS 包前加4字节时间戳，内容为SCR_BASE 的高32bit */
    DMX_REC_TIMESTAMP_LOW32BIT_SCR,         /**<Use low 32 bit of SCR_base (4 byte)  added before each recoreded  ts packet*/  /**<CNcomment: 在每个录制的TS 包前加4字节时间戳，内容为SCR_BASE 的低32bit */
} DMX_REC_TIMESTAMP_MODE_E;

typedef struct
{
    HI_MPI_DMX_REC_INDEX_S index;
    HI_MPI_DMX_REC_DATA_S data[2];    /* 2 for save rewind frame */
    struct list_head list;
    HI_BOOL Rewind;
    HI_U32   DataSel;  /* data selector for rewind frame */
}Dmx_Rec_Data_Index_Helper;

typedef struct
{
    HI_U32                      DmxId;
    HI_U32                      RecId;
    HI_UNF_DMX_REC_TYPE_E       RecType;
    HI_BOOL                     Descramed;
    HI_UNF_DMX_REC_INDEX_TYPE_E IndexType;
    HI_UNF_VCODEC_TYPE_E        VCodecType;
    HI_U32                      IndexPid;
    HI_U32                      IndexChanID;
    HI_U32                      RecBufSize;
    HI_UNF_DMX_SECURE_MODE_E    SecureMode;
    DMX_REC_STATUS_E            RecStatus;
    HI_U32                      RecFqId;
    HI_U32                      ScdFqId;
    HI_U32                      RecOqId;
    HI_U32                      ScdOqId;
    HI_U32                      ScdId;
    pid_t                       Owner;
    HI_U32                      PicParser;

    HI_U64                      PrevScrClk;
    HI_U32                      FirstFrameMs;
    HI_U32                      PrevFrameMs;
    HI_U32                      Cnt32To63Helper;

    struct mutex                LockRec;
    atomic_t                    RefCount;
    DECLARE_BITMAP(RecChnBitmap, DMX_CHANNEL_CNT);

    HI_MPI_DMX_REC_INDEX_S      LastFrameInfo;
    DMX_REC_TIMESTAMP_MODE_E    enRecTimeStamp;

    struct list_head       head;
    HI_U32                 PrevFrameEndAddr;
    HI_U32                 BlockStartAddr;
    HI_MPI_DMX_REC_DATA_S  RemRecData;
    HI_U32                 RemIdxCnt;
    HI_MPI_DMX_REC_INDEX_S RemRecIdx[DMX_MAX_IDX_ACQUIRED_EACH_TIME];
} DMX_RecInfo_S;

typedef struct
{
    HI_MPI_RMX_PUMP_TYPE_E Type;
    HI_U32 RmxPortId;
    HI_U32 Pid;
    HI_U32 RemapPid;
    struct mutex LockPump;
}RMX_Pump_Info_S;

typedef struct
{
    HI_UNF_DMX_PORT_E PortId;
    HI_U32 Ref;
    HI_U32 OverflowCount;
}RMX_Port_Info_S;

#ifndef RMX_SUPPORT
#define RMX_CHAN_CNT 0
#define RMX_PUMP_CNT 0
#define RMX_PORT_CNT 0
#endif

typedef struct
{
    HI_U32 RmxId;
    RMX_STATUS_E Status;
    pid_t Owner;

    DECLARE_BITMAP(RmxPumpBitmap, RMX_PUMP_CNT);

    HI_U32 RmxPortCnt;
    RMX_Port_Info_S RmxPortInfo[RMX_PORT_CNT];
    DECLARE_BITMAP(RmxPortBitmap, RMX_PORT_CNT);

    struct mutex LockRmx;
}RMX_Chan_Info_S;

typedef struct {
    HI_VOID      *IoBase;
    HI_U32       CbTtbr;
}Dmx_Mmu_S;

struct Dmx_Set_Operations;
typedef struct hiDmx_Set_S
{
    HI_U32                         SetId;
    HI_VOID                        *IoBase;
    Dmx_Mmu_S                      mmu;

    struct Dmx_Set_Operations      *Ops;

    DMX_TunerPort_Info_S           *TunerPortInfo;
    HI_U32                         TunerPortCnt;
    DMX_RamPort_Info_S             *RamPortInfo;
    HI_U32                         RamPortCnt;
    DMX_TagPort_Info_S             *TagPortInfo;
    HI_U32                         TagPortCnt;
    HI_UNF_DMX_TSO_PORT_ATTR_S     *TSOPortInfo;
    HI_U32                         TSOPortCnt;
    DMX_DmxInfo_S                  *DmxInfo;
    HI_U32                         DmxCnt;
    DMX_RecInfo_S                  *DmxRecInfo;
    HI_U32                         DmxRecCnt;
    struct mutex                   LockAllRec;
    struct mutex                   LockRecPid;
    DECLARE_BITMAP(RecBitmap, DMX_REC_CNT);
    DMX_ChanInfo_S                 *DmxChanInfo;
    HI_U32                         DmxChanCnt;
    struct mutex                   LockAllChn;
    DECLARE_BITMAP(ChnBitmap, DMX_CHANNEL_CNT);
    DMX_FilterInfo_S               *DmxFilterInfo;
    HI_U32                         DmxFilterCnt;
    struct mutex                   LockAllFlt;
    DECLARE_BITMAP(FilterBitmap, DMX_FILTER_CNT);
    DMX_KeyInfo_S                  *DmxKeyInfo;
    HI_U32                         DmxKeyCnt;
    HI_U32                         KeyCsa2HardFlag;
    HI_U32                         KeyCsa3HardFlag;
    HI_U32                         KeySpeHardFlag;
    HI_U32                         KeyAesCbcHardFlag;
    HI_U32                         KeyOtherHardFlag;
    struct mutex                   LockAllKey;
    DECLARE_BITMAP(KeyBitmap, DMX_KEY_CNT);
    DMX_PCR_Info_S                 *DmxPcrInfo;
    HI_U32                         DmxPcrCnt;
    struct mutex                   LockAllPcr;
    DECLARE_BITMAP(PcrBitmap, DMX_PCR_CHANNEL_CNT);
    DMX_FQ_Info_S                  *DmxFqInfo;
    HI_U32                         DmxFqCnt;
    DECLARE_BITMAP(FqBitmap, DMX_FQ_CNT);
    DMX_OQ_Info_S                  *DmxOqInfo;
    HI_U32                         DmxOqCnt;
    DECLARE_BITMAP(OqBitmap, DMX_OQ_CNT);
    DMX_SCD_Info_S                 *DmxScdInfo;
    HI_U32                         DmxScdCnt;
    DECLARE_BITMAP(ScdBitmap, DMX_SCD_CNT);

    RMX_Chan_Info_S                *RmxChanInfo;
    HI_U32                         RmxChanCnt;
    struct mutex                   LockAllRmx;
    DECLARE_BITMAP(RmxChanBitmap, RMX_CHAN_CNT);
    RMX_Pump_Info_S                *RmxPumpInfo;
    HI_U32                         RmxPumpCnt;
    struct mutex                   LockAllPump;
    DECLARE_BITMAP(RmxPumpBitmap, RMX_PUMP_CNT);

    HI_U32                         Irq;

    struct list_head               list;
}Dmx_Set_S;

struct Dmx_Set_Operations
{
    DMX_TunerPort_Info_S* (*GetTunerPortInfo)(const Dmx_Set_S *DmxSet, const HI_U32 TunerPortId);   /* TunerPortId is logical id */
    DMX_TunerPort_Info_S* (*GetTunerPortInfoByRawId)(const Dmx_Set_S *DmxSet, const HI_U32 TunerRawPortId);   /* TunerRawPortId is Raw id */
    HI_U32 (*GetTunerRawId)(const Dmx_Set_S *DmxSet, const HI_U32 TunerPortId);  /* TunerPortId is logical id */
    DMX_RamPort_Info_S* (*GetRamPortInfo)(const Dmx_Set_S *DmxSet, const HI_U32 RamPortId);  /* RamPortId is logical id */
    DMX_RamPort_Info_S* (*GetRamPortInfoByRawId)(const Dmx_Set_S *DmxSet, const HI_U32 RamRawPortId); /* RamRawPortId is Raw id */
    HI_U32 (*GetRamRawId)(const Dmx_Set_S *DmxSet, const HI_U32 RamPortId);  /* RamPortId is logical id */
    DMX_DmxInfo_S* (*GetDmxInfo)(const Dmx_Set_S *DmxSet, const HI_U32 DmxId);  /* DmxId is logical id */
    HI_U32 (*GetDmxRawId)(const Dmx_Set_S *DmxSet, const HI_U32 DmxId);  /* DmxId is logical id */
    HI_U32 (*GetRmxRawId)(const Dmx_Set_S *DmxSet, const HI_U32 RmxId);  /* RmxId is logical id */
};

typedef struct hiDmx_Set_1_Cfg_S
{
    Dmx_Set_S DmxSet;

    DMX_TunerPort_Info_S        TunerPortInfo[DMX_TUNERPORT_CNT];
    DMX_RamPort_Info_S          RamPortInfo[DMX_RAMPORT_CNT];
    DMX_TagPort_Info_S          TagPortInfo;
    HI_UNF_DMX_TSO_PORT_ATTR_S  TSOPortInfo[DMX_TSOPORT_CNT];
    DMX_DmxInfo_S               DmxInfo[DMX_CNT];
    DMX_RecInfo_S               DmxRecInfo[DMX_REC_CNT];
    DMX_ChanInfo_S              DmxChanInfo[DMX_CHANNEL_CNT];
    DMX_FilterInfo_S            DmxFilterInfo[DMX_FILTER_CNT];
    DMX_KeyInfo_S               DmxKeyInfo[DMX_KEY_CNT];
    DMX_PCR_Info_S              DmxPcrInfo[DMX_PCR_CHANNEL_CNT];
    DMX_FQ_Info_S               DmxFqInfo[DMX_FQ_CNT];
    DMX_OQ_Info_S               DmxOqInfo[DMX_OQ_CNT];
    DMX_SCD_Info_S              DmxScdInfo[DMX_SCD_CNT];
    RMX_Chan_Info_S             RmxChanInfo[RMX_CHAN_CNT];
    RMX_Pump_Info_S             RmxPumpInfo[RMX_PUMP_CNT];
}Dmx_Set_1_Cfg_S;

typedef struct hiDmx_Set_2_Cfg_S
{
    Dmx_Set_S DmxSet;

    DMX_TunerPort_Info_S        TunerPortInfo[DMX_TUNERPORT_CNT];
    DMX_RamPort_Info_S          RamPortInfo[DMX_RAMPORT_CNT];
    DMX_TagPort_Info_S          TagPortInfo;
    HI_UNF_DMX_TSO_PORT_ATTR_S  TSOPortInfo[0];    /* only dmx_set_1 export TSO port */
    DMX_DmxInfo_S               DmxInfo[DMX_CNT];
    DMX_RecInfo_S               DmxRecInfo[DMX_REC_CNT];
    DMX_ChanInfo_S              DmxChanInfo[DMX_CHANNEL_CNT];
    DMX_FilterInfo_S            DmxFilterInfo[DMX_FILTER_CNT];
    DMX_KeyInfo_S               DmxKeyInfo[DMX_KEY_CNT];
    DMX_PCR_Info_S              DmxPcrInfo[DMX_PCR_CHANNEL_CNT];
    DMX_FQ_Info_S               DmxFqInfo[DMX_FQ_CNT];
    DMX_OQ_Info_S               DmxOqInfo[DMX_OQ_CNT];
    DMX_SCD_Info_S              DmxScdInfo[DMX_SCD_CNT];
}Dmx_Set_2_Cfg_S;

typedef enum
{
    DMX_CLUSTER_INACTIVED = 0x0,
    DMX_CLUSTER_ACTIVED = 0x1,
} DMX_Cluster_State_E;

#ifdef DMX_TEE_SUPPORT
#include "teek_client_api.h"

#define TEEC_CMD_REGISTER_CHANNEL       0
#define TEEC_CMD_UNREGISTER_CHANNEL     1
#define TEEC_CMD_REGISTER_OQ            2
#define TEEC_CMD_UNREGISTER_OQ          3
#define TEEC_CMD_REGISTER_RAM_PORT      4
#define TEEC_CMD_UNREGISTER_RAM_PORT    5
#define TEEC_CMD_LOCK_CHANNEL           6
#define TEEC_CMD_UNLOCK_CHANNEL         7
#define TEEC_CMD_REGISTER_VID_SECBUF    8
#define TEEC_CMD_UNREGISTER_VID_SECBUF  9
#define TEEC_CMD_REGISTER_AUD_SECBUF    10
#define TEEC_CMD_UNREGISTER_AUD_SECBUF  11
#define TEEC_CMD_REGISTER_REC_SECBUF    12
#define TEEC_CMD_UNREGISTER_REC_SECBUF  13
#define TEEC_CMD_PARSER_PES_HEADER      14
#define TEEC_CMD_GET_DISP_CONTROL       15
#define TEEC_CMD_GET_PES_HEADER_LEN     16
#define TEEC_CMD_FIXUP_AUD_SEC_BUF      17
#define TEEC_CMD_PARSER_SC_DATA         18
#define TEEC_CMD_PARSER_FILTER_SC_DATA  19

typedef struct hiDmx_Teec_S
{
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Session session_lock; /* only for lock command. */
    TEEC_Session session_unlock; /* only for unlock command. */
    HI_BOOL  connected;
    struct mutex lock;
}Dmx_Teec_S;
#else
typedef struct {} TEEC_Operation;
typedef struct {
    struct mutex lock;
} Dmx_Teec_S;
#endif

struct Dmx_Cluster_Operations;
typedef struct hiDmx_Cluster_S
{
    DMX_Cluster_State_E state;
    spinlock_t     lock;

    Dmx_Mmu_S mmu;

    Dmx_Teec_S teec;

    struct list_head head; /* dmx set list */

    struct Dmx_Cluster_Operations *Ops;

    HI_U32 resumeCnt;   /* suspend & resume helper. */
}Dmx_Cluster_S;

struct Dmx_Cluster_Operations
{
    DMX_Cluster_State_E (*GetClusterState)(HI_VOID);
    HI_S32 (*StartCluster)(HI_VOID);
    HI_S32 (*StopCluster)(HI_VOID);
    HI_S32 (*ReleaseCluster)(struct file * file);

    HI_S32 (*GetDmxSet)(const HI_U32 DmxSetId, Dmx_Set_S **DmxSet);
    HI_S32 (*AddDmxSet)(Dmx_Set_S *DmxSet);
    HI_S32 (*DelDmxSet)(const HI_U32 DmxSetId);

    HI_S32 (*SendCmdToTA)(HI_U32 CmdId, TEEC_Operation *Operation, HI_U32 *RetOrigin);

    HI_U32 (*GetIFPortNum)(HI_VOID);
    HI_U32 (*GetTSIPortNum)(HI_VOID);
    HI_U32 (*GetTagPortNum)(HI_VOID);
    HI_U32 (*GetTsoPortNum)(HI_VOID);
    HI_U32 (*GetRamPortNum)(HI_VOID);
    HI_U32 (*GetDmxNum)(HI_VOID);
    HI_U32 (*GetRecNum)(HI_VOID);
    HI_U32 (*GetChnNum)(HI_VOID);
    HI_U32 (*GetAVChnNum)(HI_VOID);
    HI_U32 (*GetFilterNum)(HI_VOID);
    HI_U32 (*GetKeyNum)(HI_VOID);
    HI_U32 (*GetRmxNum)(HI_VOID);
};

/*
[127:96]:FQStartAddr(32bit);
[96:64]:{FQSize(16bit),FQWPtr(16bit)};
[63:32]:{FQVal(16bit),FQRPtr(16bit)}
[31:0]:{FQAlovfl_TH(8bit),FQIntCfg(4bit),FQIntCnt(4bit),FQUse(16bit)}
*/
//not used now
#if 0
typedef struct hi_FQ_Header_t
{
    HI_SIZE_T FQStartAddr:32;
    HI_SIZE_T FQSize:16;
    HI_SIZE_T FQWPtr:16;
    HI_SIZE_T FQVal:16;
    HI_SIZE_T FQRPtr:16;
    HI_SIZE_T FQAlovfl_TH:16;
    HI_SIZE_T FQIntCfg:16;
    HI_SIZE_T FQIntCnt:16;
    HI_SIZE_T FQUse:16;
} FQ_HeaderInfor_t;
#else
typedef struct hi_FQ_Header_t
{
    HI_U32 FQUse;
    HI_U32 FQRPtr;
    HI_U32 FQVal;
    HI_U32 FQWPtr;
    HI_U32 FQSize;
    HI_U32 FQStartAddr;
    HI_BOOL Valid;
} FQ_HeaderInfor_t;
#endif

/*
[255:224]:BQSAddr(32bit)
[223:192]:{Rsv(2bit),BQIntCfg(4bit),BQSize(10bit),BQAlovfl_TH(8bit),BQCfg(8bit)};
[191:160]:{Rsv(6bit),BQRPtr(10bit),BQIntCnt(4bit),Rsv(2bit),BQWPtr(10bit)}
[159:128]:BBSAddr(32bit)
[127:96]:{BBSize(16bit),BQUse(16bit)}
[96:64]:{BBEopAddr(16bit),BBWaddr(16bit)}
[63:32]:{WResByte(24bit),PVRCtrl(8bit)}
[31:0]:{EopResByte(24bit),Rsv(8bit)}

*/
//not used now
#if 0
typedef struct hi_OQ_Header_t
{
    HI_SIZE_T OQSAddr:32;
    HI_SIZE_T Rsv1:2;
    HI_SIZE_T OQIntCfg:4;
    HI_SIZE_T OQSize:10;
    HI_SIZE_T OQAlovfl_TH:8;
    HI_SIZE_T FQCfg:8;
    HI_SIZE_T Rsv2:6;
    HI_SIZE_T OQRPtr:10;
    HI_SIZE_T OQIntCnt:4;
    HI_SIZE_T Rsv3:2;
    HI_SIZE_T OQWPtr:10;
    HI_SIZE_T BBSAddr:32;
    HI_SIZE_T BBSize:16;
    HI_SIZE_T OQUse:16;
    HI_SIZE_T BBEopAddr:16;
    HI_SIZE_T BBWaddr:16;
    HI_SIZE_T WResByte:24;
    HI_SIZE_T PVRCtrl:8;
    HI_SIZE_T EopResByte:24;
    HI_SIZE_T Rsv4:8;
} OQ_HeaderInfor_t;
#else
typedef struct hi_OQ_Header_t
{
    HI_U32 OQID;
	HI_U32 BBWAddr;
	HI_U32 BBEopAddr;
	HI_U32 OQUse;
	HI_U32 BBSize;
	HI_U32 BBSAddr;
	HI_U32 OQWPtr;
	HI_U32 OQRPtr;
    HI_U32 OQWAddr;
    HI_U32 OQRAddr;
	HI_U32 FQCfg;
	HI_U32 OQSize;
	HI_U32 OQSAddr;
    HI_BOOL Valid;
} OQ_HeaderInfor_t;
#endif

typedef struct hi_ChannelDataFlow_info_t
{
    HI_U32 PIDTsPacket;             /*0xda00 + 4*n*/
    HI_U32 BufferTsPacket;          /*0xd800 + 4*n */
    HI_U32 OQEn;
    HI_U32 FQEn;
} ChannelDataFlow_info_t;

typedef struct
{
    HI_U32  DescPhyAddr;
    HI_U32  DescDepth;
    HI_U32  DescWPtr ;
    HI_U32  DescRPtr ;
    HI_U32  ValidDescNum;       /*The number of desc logic can read */
    HI_U32  AddDescNum;         /**/
    HI_U32  DescWAddr;
    HI_U32  DescRAddr;
    HI_U32  DescSize;             /*the desc->buffer block size */
} DMX_Proc_RamPort_DescInfo_S;

typedef struct
{

    HI_U32   FQLow32BPEn;           /*IP_BP_FQ_CFG ,default is 0x0, ,each bit means a FQ (0~31),for example: 0x6 means FQ1,FQ2 BR enable */
    HI_U32   FQHigh8BPEn;           /*IP_BP_FQ_CFG ,default is 0x0, ,each bit means a FQ (32~39),for example: 0x6 means FQ33,FQ34 BR enable */
    HI_BOOL  FQBP;                  /*0xc340 + t*0x8 ,FQ BackPressure*/
    //HI_BOOL  OQBP;                /*0xc300 + t*0x10, OQ BackPressure, As driver default disable the OQ BP ,so we do not need watch here*/
    HI_BOOL  SwitchBufferBp;        /*0xc204*/
    HI_BOOL  OverflowBp;            /*0xc204, IP is in BP status (caused by channel Desc queue overflow (FQ/OQ))*/
    HI_U32   Rate;                  /*0xc830 + t*0x100 ,IP rate*/
    //HI_BOOL  SwitchBufferOverflow;  /*(0x0140)*/
    //HI_BOOL  PortLogicOverflow;     /*0x3a28*/
    HI_BOOL  DebugEn;               /*0xC890+0x100*t , defalut is enable ,the following filds is controled by this fild*/
    HI_U32   OutByte;               /*0xC894+0x100*t ,the number of IP port ouput bytes*/
    HI_U32   OutTSNum;        /*0xC898+0x100*t ,the number of IP port ouput TS packets*/
    HI_U32   BPCount;               /*0xC89c+0x100*t ,the BackPressure count*/

} DMX_Proc_RamPort_BPStatus_S;

/********************************************************
the common bit mask definition
*********************************************************/
#define DMX_MASK_BIT_0      0x00000001UL        /* bit0 */
#define DMX_MASK_BIT_1      0x00000002UL        /* bit1 */
#define DMX_MASK_BIT_2      0x00000004UL        /* bit2 */
#define DMX_MASK_BIT_3      0x00000008UL        /* bit3 */
#define DMX_MASK_BIT_4      0x00000010UL        /* bit4 */
#define DMX_MASK_BIT_5      0x00000020UL        /* bit5 */
#define DMX_MASK_BIT_6      0x00000040UL        /* bit6 */
#define DMX_MASK_BIT_7      0x00000080UL        /* bit7 */
#define DMX_MASK_BIT_8      0x00000100UL        /* bit8 */
#define DMX_MASK_BIT_9      0x00000200UL        /* bit9 */
#define DMX_MASK_BIT_10     0x00000400UL        /* bit10 */
#define DMX_MASK_BIT_11     0x00000800UL        /* bit11 */
#define DMX_MASK_BIT_12     0x00001000UL        /* bit12 */
#define DMX_MASK_BIT_13     0x00002000UL        /* bit13 */
#define DMX_MASK_BIT_14     0x00004000UL        /* bit14 */
#define DMX_MASK_BIT_15     0x00008000UL        /* bit15 */
#define DMX_MASK_BIT_16     0x00010000UL        /* bit16 */
#define DMX_MASK_BIT_17     0x00020000UL        /* bit17 */
#define DMX_MASK_BIT_18     0x00040000UL        /* bit18 */
#define DMX_MASK_BIT_19     0x00080000UL        /* bit19 */
#define DMX_MASK_BIT_20     0x00100000UL        /* bit20 */
#define DMX_MASK_BIT_21     0x00200000UL        /* bit21 */
#define DMX_MASK_BIT_22     0x00400000UL        /* bit22 */
#define DMX_MASK_BIT_23     0x00800000UL        /* bit23 */
#define DMX_MASK_BIT_24     0x01000000UL        /* bit24 */
#define DMX_MASK_BIT_25     0x02000000UL        /* bit25 */
#define DMX_MASK_BIT_26     0x04000000UL        /* bit26 */
#define DMX_MASK_BIT_27     0x08000000UL        /* bit27 */
#define DMX_MASK_BIT_28     0x10000000UL        /* bit28 */
#define DMX_MASK_BIT_29     0x20000000UL        /* bit29 */
#define DMX_MASK_BIT_30     0x40000000UL        /* bit30 */
#define DMX_MASK_BIT_31     0x80000000UL        /* bit31 */

#define DMX_MASK_INT_ALL    0xffffffff

#define DMX_DEFAULT_BUF_NUM     16

/*
 * global cluster and set helpers.
 */
extern Dmx_Cluster_S *g_DmxClusterCtl;
static inline Dmx_Cluster_S *GetDmxClusterHelper(HI_VOID)
{
    BUG_ON(!g_DmxClusterCtl);
    BUG_ON(!g_DmxClusterCtl->Ops);

    return g_DmxClusterCtl;
}

#define GetDmxCluster() (GetDmxClusterHelper())

static inline Dmx_Set_S *GetDmxSetByDmxidHelper(HI_U32 DmxId)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    HI_U32 DmxSetId = DmxId / DMX_CNT + DMX_SET_1; /* Dmx Set ID start from DMX_SET_1 */
    Dmx_Set_S *tmp= HI_NULL;

    if (unlikely(HI_SUCCESS != DmxCluster->Ops->GetDmxSet(DmxSetId, &tmp)))
    {
        HI_ERR_DEMUX("Get dmxset(%d) from dmxid(%d) failed.\n", DmxSetId, DmxId);
    }

    return tmp;
}

static inline Dmx_Set_S *GetDmxSetByRamidHelper(HI_U32 RamPortId)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    HI_U32 DmxSetId = RamPortId / DMX_RAMPORT_CNT + DMX_SET_1; /* Dmx Set ID start from DMX_SET_1 */
    Dmx_Set_S *tmp= HI_NULL;

    if (unlikely(HI_SUCCESS != DmxCluster->Ops->GetDmxSet(DmxSetId, &tmp)))
    {
        HI_ERR_DEMUX("Get dmxset(%d) from ramid(%d) failed.\n", DmxSetId, RamPortId);
    }

    return tmp;
}

static inline Dmx_Set_S *GetDmxSetByTuneridHelper(HI_U32 TunerPortId)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    HI_U32 DmxSetId = TunerPortId / DMX_TUNERPORT_CNT + DMX_SET_1; /* Dmx Set ID start from DMX_SET_1 */
    Dmx_Set_S *tmp= HI_NULL;

    if (unlikely(HI_SUCCESS != DmxCluster->Ops->GetDmxSet(DmxSetId, &tmp)))
    {
        HI_ERR_DEMUX("Get dmxset(%d) from tunerid(%d) failed.\n", DmxSetId, TunerPortId);
    }

    return tmp;
}

#define GetDefDmxSet() (GetDmxSetByDmxidHelper(0))   /* The default id must  belong to HI_DMX_SET_1_ID set */
#define GetDmxSetByDmxid(DmxId) (GetDmxSetByDmxidHelper(DmxId))
#define GetDmxSetByRamid(RamPortId) (GetDmxSetByRamidHelper(RamPortId))

static inline Dmx_Set_S *GetDmxSetByRmxidHelper(HI_U32 RmxId)
{
    Dmx_Cluster_S *DmxCluster = GetDmxCluster();
    Dmx_Set_S *tmp= HI_NULL;
    HI_U32 DmxSetId = RMX_CHAN_CNT ? RmxId / RMX_CHAN_CNT + DMX_SET_1 : -1; /* Dmx Set ID start from DMX_SET_1 */

    if (unlikely(HI_SUCCESS != DmxCluster->Ops->GetDmxSet(DmxSetId, &tmp)))
    {
        HI_ERR_DEMUX("Get dmxset(%d) from rmx id(%d) failed.\n", DmxSetId, RmxId);
    }

    return tmp;
}

#define GetDmxSetByRmxid(RmxId) (GetDmxSetByRmxidHelper(RmxId))

#define TraverseForEachDmxSet(pos, head) \
    for (pos = list_entry((head)->next, typeof(*pos), list);    \
         prefetch(pos->list.next), &pos->list != (head);    \
         pos = list_entry(pos->list.next, typeof(*pos), list))


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  // __DRV_DEMUX_DEFINE_H__

