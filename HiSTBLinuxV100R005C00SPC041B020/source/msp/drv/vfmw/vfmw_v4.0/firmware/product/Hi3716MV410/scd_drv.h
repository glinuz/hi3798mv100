
#ifndef __SCD_DRV_H__
#define __SCD_DRV_H__


//#include "basedef.h"
#include "mem_manage.h"
#include "vfmw.h"
#include "sysconfig.h"

#define SCDDRV_OK       (0)
#define SCDDRV_ERR      (-1)

#define SCD_IDLE        (0)
#define SCD_BUSY        (1)

#define HEADER_BUFFER_SIZE         (128)

#ifdef CFG_SCD_TIME_OUT
#define SCD_TIMEOUT     (CFG_SCD_TIME_OUT)
#else
#define SCD_TIMEOUT     (2000)
#endif

#ifdef CFG_SCD_BUF
#define SCD_MSG_BUFFER  (CFG_SCD_BUF)
#else
#define SCD_MSG_BUFFER  (700*1024)  // 低延迟所需部分空间在这里一起分配
#endif

#ifndef MUTI_SCD
#ifndef SM_RUN_ON_SOFTSCD

/* register operator */
#ifdef MULTI_VDH

#define  WR_SCDREG(reg, dat)   MEM_WritePhyWord((SCD_REG_PHY_ADDR + reg),(dat));
#define  WR_SCDREG_1(reg, dat) MEM_WritePhyWord((SCD_REG_PHY_ADDR_1 + reg),(dat));
#define  WR_SCDREG_2(reg, dat) MEM_WritePhyWord((SCD_REG_PHY_ADDR_2 + reg),(dat));
#define  RD_SCDREG(reg)        MEM_ReadPhyWord((SCD_REG_PHY_ADDR + reg))
#define  RD_SCDREG_1(reg)      MEM_ReadPhyWord((SCD_REG_PHY_ADDR_1 + reg))
#define  RD_SCDREG_2(reg)      MEM_ReadPhyWord((SCD_REG_PHY_ADDR_2 + reg))

#else

#define  WR_SCDREG(reg, dat)                          \
    do{                                       \
        MEM_WritePhyWord((s_RegPhyBaseAddr + reg),(dat));                   \
    }while(0)
#define  WR_SCDREG_1(reg, dat)                          \
    do{                                       \
        MEM_WritePhyWord((s_RegPhyBaseAddr_1 + reg),(dat));                   \
    }while(0)

#define  RD_SCDREG(reg)    MEM_ReadPhyWord((SCD_REG_PHY_ADDR + reg))
#define  RD_SCDREG_1(reg)    MEM_ReadPhyWord((SCD_REG_PHY_ADDR_1 + reg))

#endif
#endif

#else//for muti scd
extern UADDR  s_ScdRegPhyBaseAddr[MAX_SCD_NUM];
extern UADDR  s_ScdRegResetPhyAddr[MAX_SCD_NUM];
extern UADDR  s_ScdRegResetOKPhyAddr[MAX_SCD_NUM];

#define  WR_SCDREG(id,reg,dat)   MEM_WritePhyWord((s_ScdRegPhyBaseAddr[id] + reg),(dat))
#define  RD_SCDREG(id,reg)       MEM_ReadPhyWord((s_ScdRegPhyBaseAddr[id] + reg))

#endif


typedef enum
{
    FMW_OK          = 0,
    FMW_ERR_PARAM   = -1,
    FMW_ERR_NOMEM   = -2,
    FMW_ERR_NOTRDY  = -3,
    FMW_ERR_BUSY    = -4,
    FMW_ERR_RAWNULL = -5, // Number or Size is 0
    FMW_ERR_SEGFULL = -6, // Number or Size is 0
    FMW_ERR_SCD     = -7
} FMW_RETVAL_E;

typedef enum
{
    SCDDRV_SLEEP_STAGE_NONE = 0,      // 未休眠
    SCDDRV_SLEEP_STAGE_PREPARE,       // 收到休眠命令，但还未完成休眠
    SCDDRV_SLEEP_STAGE_SLEEP          // 已休眠
} SCDDRV_SLEEP_STAGE_E;

/* control registers */
#if 1
#define  REG_SCD_START        0x800
#define  REG_LIST_ADDRESS     0x804
#define  REG_UP_ADDRESS       0x808
#define  REG_UP_LEN           0x80c
#define  REG_BUFFER_FIRST     0x810
#define  REG_BUFFER_LAST      0x814
#define  REG_BUFFER_INI       0x818
#define  REG_SCD_INT_MASK     0x81c
#define  REG_SCD_PROTOCOL     0x820
#define  REG_SCD_INI_CLR      0x824
/* state registers */
#define  REG_SCD_OVER         0x840
#define  REG_SCD_INT          0x844
#define  REG_SCD_NUM          0x84c
#define  REG_ROLL_ADDR        0x850
#define  REG_SRC_EATEN        0x854
#else
#define  REG_SCD_START        0x000
#define  REG_LIST_ADDRESS     0x004
#define  REG_UP_ADDRESS       0x008
#define  REG_UP_LEN           0x00c
#define  REG_BUFFER_FIRST     0x010
#define  REG_BUFFER_LAST      0x014
#define  REG_BUFFER_INI       0x018
#define  REG_SCD_INT_MASK     0x01c
#define  REG_SCD_PROTOCOL     0x020
#define  REG_SCD_INI_CLR      0x024
/* state registers */
#define  REG_SCD_OVER         0x040
#define  REG_SCD_INT          0x044
#define  REG_SCD_NUM          0x04c
#define  REG_ROLL_ADDR        0x050
#define  REG_SRC_EATEN        0x054
#endif
//add by l00225186 2013-03-21 3716cv200
#define  REG_DSP_SPS_MSG_ADDRESS      0x828
#define  REG_DSP_PPS_MSG_ADDRESS      0x82c
#define  REG_DVM_MSG_ADDRESS           0x830
#define  REG_SED_TOP_ADDRESS           0x834
#define  REG_CA_MN_ADDRESS             0x838
//end add

#define  REG_AVS_FLAG       0x0000
#define  REG_EMAR_ID        0x0004
#define  REG_VDH_SELRST        0x0008
#define  REG_VDH_ARBIT_CTRL_STATE             0X0010
#define  REG_VDH_CK_GT       0x000c
#define  REG_DSP_WATCH_DOG             0X0018
/*######################################################
       macro & constants
 ######################################################*/
// 实例状态
#define SM_INST_MODE_IDLE   0
#define SM_INST_MODE_WORK   1
#define SM_INST_MODE_WAIT   2


// 极限常数
#ifdef CFG_MAX_RAW_NUM
#define MAX_STREAM_RAW_NUM  CFG_MAX_RAW_NUM
#else
#define MAX_STREAM_RAW_NUM  (1024)   ///?????? for test
#endif
#ifdef CFG_MAX_SEG_NUM
#define MAX_STREAM_SEG_NUM  CFG_MAX_SEG_NUM
#else
#define MAX_STREAM_SEG_NUM  (1024 + 128)
#endif
#define MAX_STREAM_SEG_ALLOW    (100000)
#define MIN_STREAM_SEG_NUM      4//最小启动SCD的消息池的个数
#define SM_MAX_SMID             MAX_CHAN_NUM
#define SM_SCD_UP_INFO_NUM      2
#ifdef SCD_MP4_SLICE_ENABLE
#define MAX_SM_SCD_UP_INFO_NUM  3  //MPEG4上行消息区使用3个word，其他协议使用2个word，取最大的
#else
#define MAX_SM_SCD_UP_INFO_NUM  SM_SCD_UP_INFO_NUM
#endif
#define SM_MAX_DOWNMSG_SIZE     (3*MAX_STREAM_RAW_NUM*sizeof(SINT32))
#define SM_MAX_UPMSG_SIZE       (SM_SCD_UP_INFO_NUM*MAX_STREAM_SEG_NUM*sizeof(SINT32))

//add by l00225186 2013-03-22 3716cv200
#define DSP_SPS_MSG_SIZE    (32*8*4)
#define DSP_PPS_MSG_SIZE    (256*8*4)
#define DVM_SIZE             (2*64*4)
#define DSP_SED_TOP_SIZE    (64*4*1088)
#define CA_MN_SIZE           (64*4*1088)
//end add

#define SM_SEGWASTE_BUF_SIZE  64     //Seg剩余空间浪费的大小

//#define MIN_BTM_SEG_LEN  (2*1024)  // 要求WORD整数倍, 用来拷贝卷绕的,现在用FPGA_SCD_SEG_BLANK_AHB_LEN
#define MIN_TOP_SEG_LEN  (1*1024)    // 要求WORD整数倍，可能解冒的
#define SM_H263_THRESHOLD_NUM 12     //连续多少个H263头出现后就算成H263模式

#define SM_RAW_DISCARD_SIZE    (1024*1024)

#define WAIT_SEG_REL_MAX_TIME   1000
#define WAIT_SCD_RDY_MAX_TIME   1000

#define SM_VIDOBJLAY_START_CODE        0x00000120
#define SM_VOP_START_CODE              0x000001b6
#define SM_SHORT_VIDEO_START_CODE      0x00008000



#define SMSEG_STATE_INVALID   0
#define SMSEG_STATE_FRESH     1
#define SMSEG_STATE_READOUT   2

#define SMSEG_IS_READ( seg )     (((seg).SegState == SMSEG_STATE_READOUT) ? 1 : 0)
#define SMSEG_IS_RELEASED( seg ) (((seg).SegState == SMSEG_STATE_INVALID) ? 1 : 0)
#define SMSEG_IS_FRESH( seg )    (((seg).SegState == SMSEG_STATE_FRESH) ? 1 : 0)

#define SMSEG_SET_ISREAD( seg )                       \
    do{                                     \
        (seg).SegState = SMSEG_STATE_READOUT;                             \
    } while (0)

#define SMSEG_SET_ISRELEASED( seg )                     \
    do{                                     \
        (seg).SegState = SMSEG_STATE_INVALID;                             \
    } while (0)

#define SMSEG_SET_ISFRESH( seg )                      \
    do{                                     \
        (seg).SegState = SMSEG_STATE_FRESH;                             \
    } while (0)



#define FMW_ASSERT( cond )                          \
    do{                                     \
        if( !(cond) )                             \
        {                                   \
            return;                               \
        }                                   \
    } while (0)

#define FMW_ASSERT_RET( cond, ret )                     \
    do{                                     \
        if( !(cond) )                             \
        {                                   \
            return (ret);                           \
        }                                   \
    } while (0)

#define SMSEG_PHY2VIR( SegArrayPtr, phy, vir )                \
    do{                                     \
        (vir) = SegArrayPtr->pSegBufVirAddr+((phy)-SegArrayPtr->SegBufPhyAddr); \
    } while (0)



/*Check Val is between MinValue and MaxValue*/
#define SM_CHECK_VALUE(Val,MinValue,MaxValue,RetAction) \
    do \
    {\
        if (((Val) < (MinValue)) || ((Val) > (MaxValue)))\
        {\
            RetAction;\
        }\
    }while(0)



/*######################################################
       struct defs.
 ######################################################*/
typedef struct
{
    STREAM_DATA_S RawPacket[MAX_STREAM_RAW_NUM];
    SINT32      Head;
    SINT32      Tail;
    SINT32      History;
    SINT32      FirstPacketOffset;
    SINT32      CurShowIndex;
    SINT32      RawTotalSize;
} RAW_ARRAY_S;

typedef struct
{
    SINT8    ScdIntMask;
    SINT8    SliceCheckFlag;
    SINT8    ScdStart;//是否启动Scd
    UADDR    DownMsgPhyAddr;//就是下行消息池的地址
    SINT32   *pDownMsgVirAddr;
    UADDR    UpMsgPhyAddr;
    SINT32   *pUpMsgVirAddr;
    SINT32   UpLen;
    UADDR    BufferFirst;
    UADDR    BufferLast;
    UADDR    BufferIni;
    SINT32   ScdProtocol;
    SINT32   ScdIniClr;

    //add by l00225186 2013-04-15  3716cv200
    UADDR    DspSpsMsgMemAddr;
    SINT32   *pDspSpsMsgMemVirAddr;
    SINT32   DspSpsMsgMemSize;

    UADDR    DspPpsMsgMemAddr;
    SINT32   *pDspPpsMsgMemVirAddr;
    SINT32   DspPpsMsgMemSize;

    UADDR    DvmMemAddr;
    SINT32   *pDvmMemVirAddr;
    SINT32   DvmMemSize;

    UADDR    DspSedTopMemAddr;
    SINT32   *pDspSedTopMemVirAddr;
    SINT32   DspSedTopMemSize;

    UADDR    DspCaMnMemAddr;
    SINT32   *pDspCaMnMemVirAddr;
    SINT32   DspCaMnMemSize;

    SINT32   ScdLowdlyEnable;
    SINT32   reg_avs_flag;
    //end add
} SM_CTRLREG_S;
//add by l00225186 3716cv200 2013-04-15
typedef struct
{
    SINT32  Avs_Flag;
    SINT32  EMAR_ID;
    SINT32  VDH_SEL_RST_EN;
} GLB_CTRLREG_S;
typedef struct
{
    SINT32  Scdover;
    SINT32  ScdInt;
    SINT32  ShortScdNum;
    SINT32  ScdNum;
    UADDR   ScdRollAddr;
    SINT32  SrcEaten;
} SM_STATEREG_S;

/* MEPG4 特有的有效上行消息 */
typedef struct
{
    USIGN  IsShStreamFlag:        1;
    USIGN  SliceFlag:             1;
    USIGN  IsShortHeader:         1;
    USIGN  StartCodeBitOffset:    4;
    USIGN  StartCodeLength:       4;
    USIGN  Reserved:              21;
} MP4_SCD_UPMSG;

/********************************************************************************
PhyAddr   码流片的物理地址
VirAddr   码流片的虚拟地址
LenInByte 码流片的长度，以字节为单位
StreamID  码流片的ID。每片位于缓冲区的码流都有一个唯一的ID。
SegState  码流片的状态
        0：  空闲，此码流片无效
        1：  SCD已分割，但未被读走
        2：  已读走，但未释放
        其它：reserved
IsLastSeg 标识这片码流是否是一个解码单元的最后一片码流
        0：    本片码流不是最后一片
        1：    本片码流是最后一片
        other：reserved
        注：一个解码单元是指协议层一次解析的启动单元，对于H.264协议而言，
            解码单元是NAL；MPEG系列协议的解码单元则是picture
Pts         码流的时间戳
*********************************************************************************/
typedef struct
{
    UINT8     *VirAddr;
    SINT8     SegState;
    UINT8     IsLastSeg;
    UINT8     IsStreamEnd;
    UINT8     DspCheckPicOverFlag;
    UADDR     PhyAddr;
    SINT32    LenInByte;
    SINT32    StreamID;
    UINT64    Pts;
    UINT64    RawPts;
    UINT64    Usertag;
    UINT64    DispTime;
    UINT32    DispEnableFlag;
    UINT32    DispFrameDistance;
    UINT32    DistanceBeforeFirstFrame;
    UINT32    GopNum;
    /* MEPG4 特有的有效上行消息 */
    MP4_SCD_UPMSG stMp4UpMsg;
} STREAM_SEG_S;


/********************************************************************************

RawPacket 原始码流信息的记录空间，最大可记录MAX_RAW_PACKET_NUM个码流包的信息。
Head    第一个有效记录的位置
Tail    最后一个有效记录的位置
SegBufPhyAddr 码流片段缓冲区物理地址
pSegBufVirAddr  码流片段缓冲区虚拟地址
SegBufSize    码流片段缓冲区大小，单位：字节
SegBufReadAddr  码流片段缓冲区的读地址
SegBufWriteAddr 码流片段缓冲区的写地址
*********************************************************************************/
typedef struct
{
    UINT8         *pSegBufVirAddr;
    STREAM_SEG_S  StreamSeg[ MAX_STREAM_SEG_NUM ];
    SINT32        Head;
    SINT32        Tail;
    SINT32        History;
    SINT32        Current;
    UADDR         SegBufPhyAddr;
    SINT32        SegBufSize;
    UADDR         SegBufReadAddr;
    UADDR         SegBufWriteAddr;

    SINT32        SegTotalSize;    /* SegTotalSize:  Insert加, Release减 */
    SINT32        SegFreshSize;    /* SegFreshSize:  Insert加, Read减 */
    SINT32        SegFreshNum;
} SEG_ARRAY_S;


/********************************************************************************
Mode  指定配置后SM实例的工作模式。
        0：非工作模式
        1：工作模式
        如果Mode＝0表明外部模块希望这个SM实例停止工作，这种情况下只需关闭此实例，其它成员的取值被忽略。
Priority    优先级
            优先级通常是一个大于零的整数，数值越大优先级越高。
            0代表"无权限"，即无优先级，任何时候都不会被调度。
VidStd  VID_STD_E 视频协议类型。由于视频协议的多样性，在码流切分和管理中可能存在与协议相关的操作，这个参数指定协议类型。
DownMsgAddr 下行消息（码流链）的物理地址
DownMsgSize 下行消息的大小（字节数）
UpMsgAddr   上行消息（起始码地址集）的物理地址
UpMsgSize   上行消息的大小（字节数）
SegBufAddr  码流缓冲区的物理地址。这个缓冲区中存放的是被SCD（或其它等效模块）切割好的码流。缓冲区地址分为物理地址和虚拟地址，是为了适应某些操作系统（比如Linux）的特点，在其它操作系统中，虚拟地址和物理地址被置成同一个值即可。
SegBufSize  缓冲区的大小，以字节为单位。
*********************************************************************************/
typedef struct
{
    //SINT32        Mode;
    UINT8        *pBufVirAddr;
    UINT8         Priority;
    VID_STD_E     VidStd;

    UADDR         BufPhyAddr;
    SINT32        BufSize;

    /*寄存器物理基地址*/
    UADDR         RegPhyBaseAddr;
    STD_EXTENSION_U    StdExt;              /* 扩展信息，VC1相关的是码流是否为AP 和版本信息*/
    //UINT32        LastRawDisContinueCount;  /* 用来判断是否发生raw包DisContinueCount变化 ，初始化为-1 */
    //SINT32        DisContinueRawPacketFlag; /* 0: 未发生；1: 发生；2: 发生后重新开始，需被再次回到0状态 */
    SINT32        ScdLowdlyEnable;          /* 0: 低延迟未使能; 1: 低延迟使能 */
    SINT32        s32ChanLowlyEnable;
    SINT32        IsOmxPath;             /* 0: 低缓存未使能; 1: 低缓存使能 */
    //STREAM_DATA_S DisContinueRawPacket;     /* 记录发生raw包discontinue时的最新一包，用以后续从此包开始解码 */

    SINT32        MaxRawPacketNum;
    SINT32        MaxRawPacketSize;
} SM_CFG_S;


typedef struct
{
    UINT8        *pSegBufVirAddr;
    UADDR         DownMsgPhyAddr;
    SINT32       *pDownMsgVirAddr;
    SINT32        DownMsgSize;

    UADDR         UpMsgPhyAddr;
    SINT32       *pUpMsgVirAddr;
    SINT32        UpMsgSize;
    SINT32        UpMsgNum;  // Num of UpMsg, Unit: 2 * WORD
    //add by l00225186 2013-04-15 3716cv200
    UADDR         DspSpsMsgMemAddr;
    SINT32         *pDspSpsMsgMemVirAddr;
    SINT32         DspSpsMsgMemSize;

    UADDR         DspPpsMsgMemAddr;
    SINT32         *pDspPpsMsgMemVirAddr;
    SINT32         DspPpsMsgMemSize;

    UADDR         DvmMemAddr;
    SINT32         *pDvmMemVirAddr;
    SINT32         DvmMemSize;

    UADDR         DspSedTopMemAddr;
    SINT32         *pDspSedTopMemVirAddr;
    SINT32         DspSedTopMemSize;

    UADDR         DspCaMnMemAddr;
    SINT32         *pDspCaMnMemVirAddr;
    SINT32         DspCaMnMemSize;
    //end add
    UADDR         SegBufPhyAddr;
    SINT32        SegBufSize;
} SM_BUFFERCFG_S;


/********************************************************************************
 SM实例的信息结构，用于对外传递内部信息，在GetInfo()接口中使用
*********************************************************************************/
typedef struct
{
    SINT8         InstMode;      /* 0: 非工作模式,  1: 工作模式   */
    SINT32        RawNum;        /* 原始码流包数目                */
    SINT32        SegNum;        /* 原始码流数据总量              */
    SINT32        TotalRawSize;  /* 已切割的码流片段数目          */
    SINT32        TotalSegSize;  /* 已切割的码流片段数据总量      */

    SINT32        numReadSegFail;  /* 连续读不到seg的计数 */
} SM_INST_INFO_S;

/********************************************************************************
 每次送入Scd的Raw码流的属性，需要后面SegStream打Pts使用
*********************************************************************************/
typedef struct
{
    UINT8    *pCurrStartVirAddr;
    UINT8    *pCurrEndVirAddr;
    UADDR     CurrStartPhyAddr;
    UADDR     CurrEndPhyAddr;
    SINT32    Length;
    SINT32    TotalLen;//从第0包到当前包的累计长度
    UINT64    Pts;
} SM_PUSHRAWPACKET_S;

typedef struct
{
    SINT32             PushRawNum;
    SINT32             PushRawTotalLen;
    SINT32             PushRawOver; // 0:初始状态；1:正在启动SCD处理；2:完成SCD处理；3:本次启动由于seekPTS被置为无效
} SM_PUSHRAW_S;

typedef enum
{
    SM_NOT_MPEG4 = 0,
    SM_MPEG4_NOTH263,
    SM_MPEG4_H263
} MPEG4_SUB_STD;

typedef struct
{
    UINT8    *pSegBufVirAddr;//Seg Buffer的虚拟地址,取数据时使用
    SINT32    *pScdUpMsg;      //被过滤的上行消息的地址
    SINT32    ProtocolType;
    SINT32    ModeFlag;       //0:在非Mpeg4模式; 1:上一次不是H263模式, 2:上一次是H263模式
    SINT32    LastH263Num;    //上一次解码连续为H263头的数量
    UADDR     SegBufPhyAddr;
    SINT32    SegBufSize;
    UADDR     SegBufRollAddr;
    SINT32    SegNum;         //被过滤的SegStream共有多少包
    SINT32    SMInstID;       //add by l0022808 当前包的实例ID
} SM_SEGSTREAMFILTER_S;

typedef struct
{
    SINT8          SegFullFlag;
    SINT8          IsCounting;
    SINT8          LastCfgAllRawFlag; // 1 : 该实例上次启动SCD时对所有的RAW码流都进行了配置处理，该标志位在每次SCD启动时进行判断赋值
    SINT8          InsertNewSegFlag; // 1 : 该实例SCD处理后有新的SEG码流，该标志位会在每次SCD中断中进行更新，SCD启动后清0
    SINT8          FillNewRawFlag; // 1 : 该实例上次SCD处理后又有新的RAW码流，该标志位会在每次SCD启动后清0

    SM_SEGSTREAMFILTER_S SegFilter;//只在Mpeg4的情况下使用
    SM_CFG_S       Config;
    RAW_ARRAY_S    RawPacketArray;
    SEG_ARRAY_S    StreamSegArray;
    SM_PUSHRAW_S   SmPushRaw;
    SM_BUFFERCFG_S BufAddrCfg;
    SINT32         InstID;
    SINT32         ScdID;         //记录当前通道被处理的SCD id号，用于SM_Stop等函数中,在启动SCD切割钱要重新赋值
    SINT32         CutingFlag;    //记录当前通道是否正在被SCD切割，在SM_SCDIntServeProc中断处理函数中清0，在CutStreamWithSCD中置1
    SINT32         MatchScdIdPlus1;  //记录当前通道匹配的SCDid +1; -1->该通道DSP处理部分未就绪，不能处理；0->随机使用SCD切割；1->固定选用SCD0切割；2->固定选用SCD1切割...;
    SINT32         Mode;
    SINT32         TransFormSeg[MAX_STREAM_SEG_NUM *MAX_SM_SCD_UP_INFO_NUM];  //变换后的Seg片断,只在Mpeg4的情况下可能与上行消息不一样,其它协议都是一样的
    UINT32         BlockTimeInMs;  // 被阻塞的时间，单位ms
    UINT32         LastBlockTime;  // 上一次被阻塞的时间(系统时间)，用于后续刷新阻塞时间
    UINT64         LastPts;//如果本次的Pts与上次相同则打上-1
    SINT32         PrevCfgRawSize;
    SINT32         PrevCfgSegSize;

    /* 统计码率 */
    SINT32         TotalRawSize;
    UINT32         LastStatTime; //用于统计码率的时间
    UINT32         BitRate;  // 码率, 单位Kbps

    /* 其它统计信息 */
    SINT32         numReadSegFail;
    SINT32         LastSCDStartTime; //上次启动SCD处理的时间，单位 毫秒

    UINT32         UpMsgStepNum;
    UINT32         u32RawSizeCount;
    UINT32         u32RawNumCount;
    SINT32         s32ChanLowlyEnable;    /* chan lowdly enable */

    UINT32         HeaderLen;
    UINT8          HeaderBuf[HEADER_BUFFER_SIZE];

} SM_INSTANCE_S;


typedef struct hiSCD_DRV_MEM_S
{
    UADDR          HwMemAddr;
    SINT32         HwMemSize;

    UADDR          DownMsgMemAddr;
    UINT32        *pDownMsgMemVirAddr;
    SINT32         DownMsgMemSize;

    UADDR          UpMsgMemAddr;
    SINT32        *pUpMsgMemVirAddr;
    SINT32         UpMsgMemSize;
    //add by l00225186 2013-04-15  3716cv200
    //sps pps 需要与通道相关
    /*
    	SINT32         DspSpsMsgMemAddr;
    	SINT32         *pDspSpsMsgMemVirAddr;
    	SINT32         DspSpsMsgMemSize;

    	SINT32         DspPpsMsgMemAddr;
    	SINT32         *pDspPpsMsgMemVirAddr;
    	SINT32         DspPpsMsgMemSize;
    */
    UADDR          DvmMemAddr;
    SINT32         *pDvmMemVirAddr;
    SINT32         DvmMemSize;

    UADDR          DspSedTopMemAddr;
    SINT32         *pDspSedTopMemVirAddr;
    SINT32         DspSedTopMemSize;

    UADDR          DspCaMnMemAddr;
    SINT32         *pDspCaMnMemVirAddr;
    SINT32         DspCaMnMemSize;
    //end add
    UADDR          ScdRegAddr;
    UADDR          ScdResetRegAddr;
} SCD_DRV_MEM_S;

/*
    低延迟所需要维护SPS,PPS信息的DDT MEM 描述
*/
typedef struct
{
    MEM_RECORD_S   stBaseMemInfo;

    UADDR          DspSpsMsgMemAddr;
    SINT32         *pDspSpsMsgMemVirAddr;
    SINT32         DspSpsMsgMemSize;

    UADDR          DspPpsMsgMemAddr;
    SINT32         *pDspPpsMsgMemVirAddr;
    SINT32         DspPpsMsgMemSize;
} DSP_CTX_MEM_S;

/********************************************************************************
ThisInstID    当前SCD正在处理的实例编号
ScdState    SCD的工作状态：
          0：空闲（未启动）
          1：运行（正在运行中）
*********************************************************************************/
typedef struct
{
    SCD_DRV_MEM_S       ScdDrvMem;
    //    SM_INSTANCE_S       SmInstArray[MAX_CHAN_NUM];
    SINT32              IsScdDrvOpen;
    SINT32              ThisInstID;
    SINT32              SCDState;
    UINT32              LastProcessTime;
    SINT32              DspReloadFlag;
} SMDRV_PARAM_S;

/********************************************************************************
SmInstArray     SM实例数组，记录SCD实例的信息的指针
DspCtxMem       SM实例对应分配的DSP资源描述符
SmInstPriority  SM实例的优先级
          0代表最高优先级，1次之，依次类推
*********************************************************************************/
typedef struct
{
    DSP_CTX_MEM_S       DspCtxMem;
    SM_INSTANCE_S      *pSmInstArray;
    SINT32              SmInstPriority;
} SM_IIS_S;



/*######################################################
       interface functions.
 ######################################################*/
SINT32 SCDDRV_PrepareSleep(SINT32 ScdId);
SCDDRV_SLEEP_STAGE_E SCDDRV_GetSleepStage(SINT32 ScdId);
VOID SCDDRV_ForceSleep(SINT32 ScdId);
VOID SCDDRV_ExitSleep(SINT32 ScdId);


/*========================================================================
    part1.    raw stream management module
========================================================================*/
/*********************************************************************************
    函数原型
      SINT32 ResetSCD(VOID)
    功能描述
      复位SCD硬件
    参数说明
      无
    返回值
      无
    算法描述
      略
*********************************************************************************/
SINT32 ResetSCD(SINT32 ScdId);

/************************************************************************
  原型  VOID ResetRawStreamArray( RAW_ARRAY_S *pRawStreamArray )
  功能  将一组原始码流集合复位，清除其中所有记录
  参数  pRawStreamArray 指向原始码流集合
  返回值  无
************************************************************************/
VOID ResetRawStreamArray( RAW_ARRAY_S *pRawStreamArray );

/************************************************************************
  原型  SINT32 InsertRawPacket( RAW_ARRAY_S *pRawStreamArray, RAW_PACKET_S *pRawPacket )
  功能  将一个原始码流包插入到指定的码流集合
  参数  pRawStreamArray 指向原始码流集合
        pRawPacket  指向原始码流包
  返回值  成功返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 InsertRawPacket( RAW_ARRAY_S *pRawStreamArray, STREAM_DATA_S *pRawPacket );

/************************************************************************
  原型  SINT32 GetRawIsFull(SINT32 SmID)
  功能  得到Raw Buffer能否插入的状态
  参数  SmID  码流管理模块的实例的标识
  返回值  可以插入Raw Packet就返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 GetRawState(SINT32 SmID);

/************************************************************************
  原型  SINT32 SetFirstOffset( RAW_ARRAY_S *pRawStreamArray, SINT32 Offset )
  功能  设置第一个码流包的有效字节偏移
  参数  pRawStreamArray 指向原始码流集合
    Offset  第一个数据包的有效字节偏移量
  返回值  成功返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 SetFirstOffset( RAW_ARRAY_S *pRawStreamArray, SINT32 Offset );

/************************************************************************
  原型  SINT32 GetFirstOffset( RAW_ARRAY_S *pRawStreamArray, SINT32 *pOffset )
  功能  获取第一个码流包的有效字节偏移
  参数  pRawStreamArray 指向原始码流集合
        pOffset 第一个数据包的有效字节偏移量
  返回值  成功返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 GetFirstOffset( RAW_ARRAY_S *pRawStreamArray, SINT32 *pOffset );

/************************************************************************
  原型  SINT32 ShowFirstRawPacket( RAW_ARRAY_S *pRawStreamArray, RAW_PACKET_S *pRawPacket )
  功能  查看指定的码流集合中的第一个码流包信息。
  参数  pRawStreamArray 指向原始码流集合
        pRawPacket  指向原始码流包的结构，用于接收码流包信息
  返回值  成功返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 ShowFirstRawPacket( RAW_ARRAY_S *pRawStreamArray, STREAM_DATA_S *pRawPacket );

/************************************************************************
  原型  SINT32 ShowNextRawPacket( RAW_ARRAY_S *pRawStreamArray, RAW_PACKET_S *pRawPacket )
  功能  "Next"是相对于上一次调用Show…函数而言的。本函数与ShowFirstRawPacket()结合可查看所有码流包的信息。
  参数  pRawStreamArray 指向原始码流集合
        pRawPacket  指向原始码流包的结构，用于接收码流包信息
  返回值  成功返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 ShowNextRawPacket( RAW_ARRAY_S *pRawStreamArray, STREAM_DATA_S *pRawPacket );

/************************************************************************
  原型  SINT32 DeleteRawPacket( RAW_ARRAY_S *pRawStreamArray, UINT32 DelNum )
  功能  从第一码流包开始，连续删除DelNum个码流包。
          由于有码流片段缓冲区的存在，原始码流无需乱序释放，所以原始码流
      管理模块可以只支持按插入顺序释放。码流包从原始码流集中删除的同时，
      也会调用MPP的接口通知外部组建释放对应的码流空间。
  参数  pRawStreamArray 指向原始码流集合
        pRawPacket  指向原始码流包的结构，用于接收码流包信息
  返回值  成功返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 DeleteRawPacket( RAW_ARRAY_S *pRawStreamArray, UINT32 DelNum );

/************************************************************************
原型    SINT32 DeleteRawPacketInBuffer(SINT32 SmID, SINT32 ResetFlag)
功能    从history开始，连续删除到head
参数    pRawStreamArray 指向原始码流集合
返回值  成功返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 DeleteRawPacketInBuffer(SINT32 SmID, SINT32 ResetFlag);

/************************************************************************
  原型  SINT32 GetRawNumOffsert( RAW_ARRAY_S *pRawStreamArray, SINT32 Len, SINT32 *pNum, SINT32 *pOffset, UINT64 *pPts, UINT64 *pUsertag, UINT64 *pDisptime, UINT32 *pDispEnableFlag, UINT32 *pDispFrameDistance, UINT32 *pDistanceBeforeFirstFrame, UINT32 *pGopNum, UINT64 *pRawPts);
  功能  从长度Len计算从第一码流包开始，共有需要有多个的码流包的大小等于Len，如果有半包则长度余数为Offset。
  参数  pRawStreamArray 指向原始码流集合
        Len             长度
  返回值  成功返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 GetRawNumOffsert( RAW_ARRAY_S *pRawStreamArray, SINT32 Len, SINT32 *pNum, SINT32 *pOffset, UINT64 *pPts, UINT64 *pUsertag, UINT64 *pDisptime, UINT32 *pDispEnableFlag, UINT32 *pDispFrameDistance, UINT32 *pDistanceBeforeFirstFrame, UINT32 *pGopNum, UINT64 *pRawPts);

/************************************************************************
  原型  SINT32 DeleteRawLen( RAW_ARRAY_S *pRawStreamArray, SINT32 DelLen )
  功能  从第一码流包开始，连续删除DelLen的码流包如果删除的长度不够，则要置偏移。
  参数  pRawStreamArray 指向原始码流集合
        DelLen  总共需要删除的长度
  返回值  成功返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 DeleteRawLen( RAW_ARRAY_S *pRawStreamArray, SINT32 DelLen );

/************************************************************************
  原型  SINT32 DeleteLastSendRaw( UINT32 SmID  )
  功能  删除SmID所指向的通道，上次所送入SCD的码流
  参数  SmID通道号
  返回值  无
************************************************************************/
VOID DeleteLastSendRaw( UINT32 SmID );

/************************************************************************
  原型  SINT32 GetRawStreamSize( RAW_ARRAY_S *pRawStreamArray, SINT32 *pStreamSize)
  功能  将原始码流集合中所有包的长度累加起来即为码流总量，目的是获得目前的码流占用情况。
  参数  pRawStreamArray 指向原始码流集合
  返回值  成功返回码流长度，否则返回错误码（负整数）
************************************************************************/
SINT32 GetRawStreamSize( RAW_ARRAY_S *pRawStreamArray, SINT32 *pStreamSize);

/************************************************************************
    原型  SINT32 GetRawStreamNum( RAW_ARRAY_S *pRawStreamArray, SINT32 *pStreamNum )
    功能  将原始码流集合中已被推送但尚未被切割的码流包数，目的是获得目前的码流占用情况。
    参数  pRawStreamArray 指向原始码流集合
    返回值  成功返回pStreamNum码流包数，并返回FMW_OK,失败则返回错误码（负整数）
************************************************************************/
SINT32 GetRawStreamNum( RAW_ARRAY_S *pRawStreamArray, SINT32 *pStreamNum );

/************************************************************************
原型    SINT32 GetRawStreamNumInBuffer( RAW_ARRAY_S *pRawStreamArray, SINT32 *pStreamNum )
功能    将原始码流集合中已被推送但尚未被切割的码流包数和已切割但未被从buffer中释放的码流包数，
        目的是获得目前的码流占用情况。
参数    pRawStreamArray 指向原始码流集合
返回值  成功返回pStreamNum码流包数，并返回FMW_OK,失败则返回错误码（负整数）
************************************************************************/
SINT32 GetRawStreamNumInBuffer(RAW_ARRAY_S *pRawStreamArray, SINT32 *pStreamNum);

/*========================================================================
    part2.    stream segment management module
========================================================================*/

/************************************************************************
  原型  SINT32 ConfigStreamSegArray( SEG_ARRAY_S *pStreamSegArray, UINT32 BufPhyAddr, UINT8 *pBufVirAddr, UINT32 BufSize )
  功能  为码流片段集合配置必要的信息，如码流片段缓冲区的首地址、长度等。
  参数  pStreamSegArray 指向切割码流片段集合
        BufPhyAddr  码流片段缓冲区物理地址
        BufSize 码流片段缓冲区大小，单位：字节
  返回值  成功返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 ConfigStreamSegArray( SEG_ARRAY_S *pStreamSegArray, UADDR BufPhyAddr, UINT8 *pBufVirAddr, UINT32 BufSize );

/************************************************************************
  原型  VOID ResetStreamSegArray( SEG_ARRAY_S *pStreamSegArray )
  功能  将一组切割码流片段集合复位，清除其中所有记录，释放全部切割码流缓冲空间。
  参数  pStreamSegArray 指向切割码流片段集合
  返回值  无
************************************************************************/
VOID ResetStreamSegArray( SEG_ARRAY_S *pStreamSegArray );

/************************************************************************
  原型  VOID GetSegBuf( SEG_ARRAY_S *pStreamSegArray, UINT32 *pBufAddr, UINT32 *pBufLength )
  功能  获取码流片段缓冲区的地址和长度。
          SCD切割码流时需要配置缓冲区的顶底地址。
  参数  pStreamSegArray 指向切割码流片段集合
        pAddr 接收码流片段缓冲区首地址（物理地址）变量的指针
        pLength 接收码流片段缓冲区长度变量的指针
  返回值  无
************************************************************************/
VOID GetSegBuf( SEG_ARRAY_S *pStreamSegArray, UADDR *pBufAddr, UINT32 *pBufLength );

/************************************************************************
  原型  VOID GetFreeSegBuf( SEG_ARRAY_S *pStreamSegArray, UINT32 *pAddr, UINT32 *pLength )
  功能  获取空闲的码流片段缓冲空间，所有介于写地址和读地址只见的空间都被视为空闲空间。
          由于码流片段缓冲区是一个循环buffer，所以空闲区域可能卷绕，从而使得地址不连续。
      判断地址是否卷绕的依据是pAddr+pLength>buffer底部。
  参数  pStreamSegArray 指向切割码流片段集合
        pAddr 接收空闲空间首地址（物理地址）变量的指针
        pLength 接收空闲空间长度变量的指针
  返回值  无
************************************************************************/
VOID GetFreeSegBuf( SEG_ARRAY_S *pStreamSegArray, SINT32 *pAddr, SINT32 *pLength );

/************************************************************************
  原型  SINT32 InsertStreamSeg( SEG_ARRAY_S *pStreamSegArray, STREAM_SEG_S *pStreamSeg )
  功能  向特定的码流片段集中插入一个码流片段。
      本函数的任务有两个，第一是将待插入码流片段的描述信息加入到队列中，第二是更新码流片段缓冲区的写地址。
  参数  pStreamSegArray 指向切割码流片段集合
      pStreamSeg  待插入码流片段的描述信息
  返回值  成功返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 InsertStreamSeg( SEG_ARRAY_S *pStreamSegArray, STREAM_SEG_S *pStreamSeg , SINT32 InstID);

/************************************************************************
  原型  SINT32 GetStreamSeg( SEG_ARRAY_S *pStreamSegArray, STREAM_SEG_S *pStreamSeg )
  功能  从特定的码流片段集中取出一个码流片段：将处于队列头的码流片段信息取出。
  参数  pStreamSegArray 指向流片段集合
        pStreamSeg  存储码流片段的描述信息的结构指针
  返回值  成功返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 GetStreamSeg( SEG_ARRAY_S *pStreamSegArray, STREAM_SEG_S *pStreamSeg );

/************************************************************************
  原型  SINT32 ReleaseStreamSeg( SEG_ ARRAY_S *pStreamSegArray, UINT32 StreamID)
  功能  将ID为StreamID的码流片段释放，如果这个码流片段是历史上已读取但未释放的第一段码流，则同步更新码流片段缓冲区的读地址。
  参数  pStreamSegArray 指向流片段集合
      StreamID  待释放码流片段的ID
        如果StreamID取值在0~ MAX_STREAM_SEG_NUM-1之间则表示要释放索引号为StreamID的码流片段；
        如果StreamID = 0xffffffff，则表示释放第一个码流片段。这个参数的使用场景是，如果码流
      解析模块长时间不释放码流包，出于容错考虑SM模块需要强制释放第一包，以腾出空间来继续切割码流，
      避免死锁。这种情况下，无论第一个码流片段是否被读走，都强制释放。
  返回值  成功返回FMW_OK，否则返回错误码
************************************************************************/
SINT32 ReleaseStreamSeg( SEG_ARRAY_S *pStreamSegArray, UINT32 StreamID );

/************************************************************************
    原型  SINT32 GetSegStreamSize( SEG_ARRAY_S *pSegStreamArray, SINT32 *pStreamSize)
    功能  将码流片断集合中所有包的长度累加起来即为码流总量，目的是获得目前的码流占用情况。
    参数  pSegStreamArray 指向码流片断集合
    返回值  成功返回码流长度，否则返回错误码（负整数）
************************************************************************/
SINT32 GetSegStreamSize( SEG_ARRAY_S *pSegStreamArray, SINT32 *pStreamSize);

/************************************************************************
    原型  GetSegStreamNum(SEG_ARRAY_S *pSegStreamArray, SINT32 *pStreamNum);
    功能  将码流片断集合中已被推送但尚未被切割的码流包数，目的是获得目前的码流占用情况。
    参数  pSegStreamArray 指向码流片断集合
    返回值  成功返回码流包数，否则返回错误码（负整数）
************************************************************************/
SINT32 GetSegStreamNum(SEG_ARRAY_S *pSegStreamArray, SINT32 *pStreamNum);

VOID GetFreshSegStream( SEG_ARRAY_S *pSegStreamArray, SINT32 *pFreshNum, SINT32 *pFreshSize);


/*========================================================================
    part3.    top level
========================================================================*/
SINT32 SM_OpenSCDDrv(UADDR MemAddr, SINT32 MemSize, SINT32 ScdId);
SINT32 SM_CloseSCDDrv(SINT32 ScdId);
VOID SM_GiveThreadEvent(SINT32 SmID);
VOID SCDDRV_MaskInt(SINT32 ScdId);
VOID SCDDRV_EnableInt(SINT32 ScdId);
//SINT32 SM_GetInstanceRawDiscontinue(SINT32 InstID);
//SINT32 SM_SetRawDiscontinueFlag(SINT32 InstID, SINT32 Flag);
//SINT32 SM_GetInstanceStreamEmptyFlag(SINT32 InstID);
VOID SM_InstInit(VOID);
VOID SCDDRV_ResetSCD(SINT32 ChanID);

/************************************************************************
  原型  SINT32 SM_Reset ( SINT32 SmID )
  功能  本函数用以复位一个码流管理模块的实例，
      本函数使得ID号为SmID的码流管理模块实例的所有状态都回到初始值。
  参数  SmID  标识码流管理模块的上下文信息。在多路解码中，每一路解码器都有一个码流管理模块的
      实例，SmID指向某个特定的实例。
  返回值  成功返回FMW_OK，否则返回特定的错误码
************************************************************************/
SINT32 SM_ClearInst (SINT32 SmID);
SINT32 SM_Reset ( SINT32 SmID );
SINT32 SM_Start (SINT32 SmID);
SINT32 SM_Stop (SINT32 SmID);

/************************************************************************
  原型  SINT32 SM_Config ( SINT32 SmID, SM_CFG_S *pSmCfg )
  功能  本函数用以配置一个码流管理模块的实例，在这个实例开始工作之前，必须调用此函数给它配置必要的信息。
  参数  SmID  标识码流管理模块的上下文信息。在多路解码中，每一路解码器都有一个码流管理模块的实例，
      SmID指向某个特定的实例。
      pSmCfg  SM实例的配置信息
  返回值  成功返回FMW_OK，否则返回特定的错误码
************************************************************************/
SINT32 SM_Config ( SINT32 SmID, SM_CFG_S *pSmCfg );
SINT32 SM_CfgBuf ( SINT32 SmID, SINT32 ScdId );

/************************************************************************
    原型    SINT32 SM_GetInfo( SINT32 SmID, SM_INST_INFO_S *pSmInstInfo )
    功能  本函数查询码流管理模块中原始码流已累计了多少包，已切割待解码的码流有多少段。在多路解码环境下，上层调度程序可能需要根据每一路的这些信息来决定将SCD分配给哪一路去切割码流。
    参数  SmID  码流管理模块的实例的标识
            pSmInstInfo  存放实例信息的结构
    返回值  FMW_ERR_PARAM or FMW_OK
************************************************************************/
SINT32 SM_GetInfo( SINT32 SmID, SM_INST_INFO_S *pSmInstInfo );

/************************************************************************
  原型  SINT32 SM_PushRaw(SINT32 SmID, RAW_PACKET_S *pRawPacket)
  功能  外部程序调用本函数向码流管理模块推送一段码流，推送过程只是将码流的相关信息记录到SM模块的内部结构中，并不一定立即执行码流的切分工作。
  参数  SmID  码流管理模块的实例的标识
    PhyAddr 原始码流的物理地址
    VirAddr 原始码流的虚拟地址
    Length  原始码流的长度，以字节为单位
    Pts 原始码流的节目时间戳(PTS)
  返回值  成功返回FMW_OK，否则返回特定的错误码
************************************************************************/
SINT32 SM_PushRaw(SINT32 SmID, STREAM_DATA_S *pRawPacket);

/************************************************************************
原型  SINT32 SM_ReadStreamSeg(SINT32 SmID, STREAM_SEG_S *pStreamSeg )
功能  本函数从码流管理模块获取一段已分割完毕的码流
参数  SmID  码流管理模块的实例的标识
      pStreamSeg  接收码流段信息的结构指针
返回值  如果读取成功返回FMW_OK，否则返回相应的错误码
************************************************************************/
SINT32 SM_ReadStreamSeg(SINT32 SmID, STREAM_SEG_S *pStreamSeg);

/************************************************************************
  原型  SINT32 SM_ReleaseStreamSeg( SINT32 SmID, SINT32 StreamID )
  功能  本函数将协议层已经解码完毕的码流片归还给码流管理模块
  参数  SmID  码流管理模块的实例的标识
        StreamID  码流片的ID
  返回值  无
************************************************************************/
SINT32 SM_ReleaseStreamSeg( SINT32 SmID, SINT32 StreamID );

/************************************************************************
  原型  SINT32 CalcInstBlockTime( SINT32 SmID )
  功能  计算指定实例被阻塞的时间，将上次阻塞时间与当前时间之差累加到
          总阻塞时间上去，并且更新"上次阻塞时间"
  参数  SmID  码流管理模块的实例的标识
          FirstCalc  1: 第一次计算，仅记录"上次阻塞时间"
                     0: 累计计算，在上述工作之外还要累加阻塞时间
  返回值  累计阻塞时间
************************************************************************/
SINT32 CalcInstBlockTime( SINT32 SmID, SINT32 FirstCalc );

/************************************************************************
  原型  SINT32 SetInstMode( SINT32 SmID, SINT32 Mode )
  功能  计算指定实例被阻塞的时间，将上次阻塞时间与当前时间之差累加到
          总阻塞时间上去，并且更新"上次阻塞时间"
  参数  SmID  码流管理模块的实例的标识
          Mode    SM_INST_MODE_WORK  工作模式
                  SM_INST_MODE_WAIT  阻塞模式
  返回值  成功返回FMW_OK， 否则返回错误码
************************************************************************/
SINT32 SetInstMode( SINT32 SmID, SINT32 Mode );


/*========================================================================
    part4.   SCD level 码流切割和控制子模块
========================================================================*/

/************************************************************************
  原型  SINT32 CutStreamWithSCD(RAW_ARRAY_S *pRawStreamArray, SEG_ARRAY_S *pStreamSegArray, UINT32 StdType, SINT32 *pCutRawNum)
    功能  驱动SCD切割码流。
            本函数从原始码流集中取出若干个码流包配置给SCD，然后启动SCD切割。一次切割的原始码流包个数取决于两个条件：
            1.  码流片段缓冲区的空闲空间
            2.  原始码流的PTS分布
            亦即，可切割的码流包总长度不得大于码流片段缓冲区的空闲空间总量，一次切割的码流包PTS不能变化（对于MPEG系列可能无须受此限制）。
            本函数调用原始码流管理子模块的ShowFirstRawPacket() / ShowNextRawPacket()函数组依次扫描原始码流，得到可切割的码流包数。
    参数  pRawStreamArray 原始码流集
          pStreamSegArray 切割码流片段集
          StdType 码流的协议类型。
            SCD同步头检测依赖于协议类型。
            pCutRawNum 可切割的码流包数目（大于0的整数）
    返回值  FMW_OK或错误码
************************************************************************/
SINT32 CutStreamWithSCD(SM_INSTANCE_S *pScdInstance, SINT32 ScdId);

/************************************************************************
  原型  VOID ScdDriver(SM_CTRLREG_S *pSmCtrlReg, UINT32 *pRegUpAddress)
    功能  配置寄存器函数
    参数
    返回值
************************************************************************/
SINT32 ScdDriver(SM_CTRLREG_S *pSmCtrlReg, SM_PUSHRAW_S *pSmPushRaw, SINT32 SegStreamSize, RAW_ARRAY_S *pRawStreamArray,
            SINT32 *pDownMsgVirAddr, UADDR DownMsgPhyAddr,SINT32 ScdId);

/************************************************************************
  原型  VOID WriteScdVtrlReg()
    功能  配置Scd状态寄存器函数
    参数
    返回值
************************************************************************/
VOID WriteScdVtrlReg(SM_CTRLREG_S *pSmCtrlReg, SINT32 ScdId);

/************************************************************************
  原型  SINT32 WriteScdMsg(SM_PUSHRAW_S *pSmPushRaw, SINT32 *pDownMsgVirAddr, SINT32 DownMsgPhyAddr)
    功能  配置Scd下行消息池
    参数
    返回值
************************************************************************/
SINT32 WriteScdMsg(SM_PUSHRAW_S *pSmPushRaw, SINT32 SegStreamSize, RAW_ARRAY_S *pRawStreamArray,
           SINT32 *pDownMsgVirAddr, UADDR DownMsgPhyAddr, SINT32 LowdlyFlag,SINT32 ScdId);

/************************************************************************
  原型  VOID ReadScdStateReg(SM_STATEREG_S *pSmStateReg)
    功能  读取Scd状态寄存器函数
    参数
    返回值
************************************************************************/
VOID ReadScdStateReg(SM_STATEREG_S *pSmStateReg, SINT32 ScdId);

/************************************************************************
  原型  SINT32 CheckScdStateReg(SM_STATEREG_S *pSmStateReg, SINT32 StdType)
    功能  检查Scd状态寄存器函数
    参数
    返回值
************************************************************************/
SINT32 CheckScdStateReg(SM_STATEREG_S *pSmStateReg, SINT32 StdType);

/************************************************************************
原型  SINT32 ScdReturnSegFilter(SM_SEGSTREAMFILTER_S *pSegStreamFilter, SINT32 *pTransFormSeg, SINT32 *pSegNum, UINT32 UpMsgStepNum)
功能
参数    pSegNum(输入/输出)
返回值  FMW_OK或错误码
************************************************************************/
SINT32 ScdReturnSegFilter(SM_SEGSTREAMFILTER_S *pSegStreamFilter, SINT32 *pTransFormSeg, SINT32 *pExtraSrcBack, SINT32 *pSegNum, UINT32 UpMsgStepNum);

/************************************************************************
原型  SINT32 CheckSegValid(STREAM_SEG_S *pSegPacket, SINT32 SendSegTotalSize, SINT32 SegFreeBufLen)
功能
        PreSegTotalSize:本包之前先送入的总长度
        SegFreeBufLen:Seg Stream Buffer中还剩余多少长度
返回值  FMW_OK或错误码

************************************************************************/
SINT32 CheckSegValid(STREAM_SEG_S *pSegPacket, SINT32 SegFreeBufLen);

/************************************************************************
原型  SM_PUSHRAW_S *pPushRaw, SINT32 CurrSegTotalSize, SINT32 *pCurrRawId,UINT32 *pPts)
功能
参数
返回值  Pts
************************************************************************/
//VOID GetSegPts(SM_PUSHRAW_S *pPushRaw, SINT32 CurrSegTotalSize, SINT32 *pCurrRawId, SINT64 *pLastPts, SINT64 *pPts);
//  VOID GetSegPts(RAW_ARRAY_S *pRawStreamArray, SINT32 Len, UINT64 *pLastPts, UINT64 *pPts, UINT64 *pUsertag, UINT64 *pDisptime);
VOID GetSegPts(RAW_ARRAY_S *pRawStreamArray, SINT32 Len, UINT64 *pLastPts, STREAM_SEG_S *pSegPacket);

/************************************************************************
原型  SINT32 ProcessSCDReturn(RAW_ ARRAY_S *pRawStreamArray, SEG_ARRAY_S *pStreamSegArray, SINT32 *pCutRawNum)
功能  处理SCD的返回信息。
        本函数从SCD的寄存器和上、下行消息池中读取上一次的码流切割信息，完成下列两种操作：
        1.  根据上行消息将各帧（或NAL）码流按STREAM_SEG_S格式组织并且插入到码流片段集pStreamSegArray中管理起来
        2.  分析此次切割消耗了多少原始码流包，并将这些码流包释放。
        参数
        pRawStreamArray 原始码流集
        pStreamSegArray 切割码流片段集
返回值  FMW_OK或错误码
************************************************************************/
SINT32 ProcessScdReturn(SM_INSTANCE_S *pScdInstance, SINT32 ScdId);

/************************************************************************
  原型  VOID SM_SCDIntServeProc ( VOID )
  功能  本函数为SCD的中断服务程序，响应SCD的中断信号并处理其返回的信息。
  参数  无
  返回值  无
************************************************************************/
VOID SM_SCDIntServeProc ( SINT32 ScdId);

/************************************************************************
  原型  VOID SM_Wakeup ( VOID )
  功能  本函数用于唤醒SM模块，避免SM停工后无法恢复运行。
      如果采用线程监控SCD，则解码线程在每解完一帧调用此函数。
      如果采用定时中断监控SCD，则在定时中断中调用此函数。
  参数  无
  返回值  无
************************************************************************/
VOID SM_Wakeup ( VOID );
VOID  SM_SimpleEntry(VOID);

/************************************************************************
  原型  VOID SM_GetBasePhyVir(SINT32 SmID, SINT32 *pBasePhyAddr, UINT8 **pBaseVirAddr)
  功能  得以基物理地址与基虚拟地址
  参数  无
  返回值  输出的虚拟地址
************************************************************************/
VOID SM_GetBasePhyVir(SINT32 SmID, SINT32 *pBasePhyAddr, UINT8 **pBaseVirAddr);

/************************************************************************
  原型  UINT8 *SM_PhyToVir(SINT32 BasePhyAddr, UINT8 *pBaseVirAddr, SINT32 PhyAddr)
  功能  物理地址转成虚拟地址
  参数  无
  返回值  输出的虚拟地址
************************************************************************/
UINT8 *SM_PhyToVir(UADDR PhyAddr);

/************************************************************************
原型  UINT8 *SM_ScdPhyToVir(SINT32 BasePhyAddr, UINT8 *pBaseVirAddr, SINT32 PhyAddr)
功能  物理地址转成虚拟地址
参数  无
返回值  输出的虚拟地址
************************************************************************/
UINT8 *SM_ScdPhyToVir(UADDR BasePhyAddr, UINT8 *pBaseVirAddr, UADDR PhyAddr);

/************************************************************************
原型  VOID PrintScdRawState()
功能  打印Raw队列的详细情况
参数
返回值  无
************************************************************************/
VOID PrintScdRawState(SINT32 SmID, SINT32 PrintDetail);

/************************************************************************
原型  VOID PrintScdSegState()
功能  打印Seg队列的详细情况
参数
返回值  无
************************************************************************/
VOID PrintScdSegState(SINT32 SmID, SINT32 PrintDetail);
/************************************************************************
原型  VOID PrintScdVtrlReg()
功能  打印Scd控制寄存器函数
参数
返回值  无
************************************************************************/
VOID PrintScdVtrlReg(SINT32 ScdId);

/************************************************************************
原型  VOID PrintScdVtrlReg()
功能  打印下行消息池
参数
返回值  无
************************************************************************/
VOID PrintDownMsg(UADDR DownMsgPhyAddr, SINT32 * pDownMsgVirAddr, SINT32 PushRawNum,SINT32 ScdId);

/************************************************************************
原型  VOID PrintScdStateReg()
功能  打印Scd状态寄存器函数
参数
返回值  无
************************************************************************/
VOID PrintScdStateReg(SM_STATEREG_S *pSmStateReg, SINT32 ScdId);

/************************************************************************
原型  VOID PrintScdInputRaw()
功能  打印Raw码流
参数
返回值  无
************************************************************************/
VOID PrintScdInputRawAfterStart(UINT8 *pDownMsgVirBaseAddr, UADDR DownMsgPhyBaseAddr,
                                UINT8 *pRawBufVirBaseAddr,  UADDR RawBufPhyBaseAddr,
                                VOID *fpStream);

VOID PrintSmInfo(SINT32 SmID);

SINT32 SM_SeekPts(SINT32 ChanID, UINT64 *pArgs);
SINT32 SM_AllocDSPCtxMem(SINT32 SmID);
VOID SM_DeletDSPCtxMem(MEM_RECORD_S *pstDSPMem);
VOID WriteHexFile(SINT32 eVidStd, SINT32 ScdId);

/***********************************************************************
SCDDRV_MarkReloadDsp: 该函数用以标记当前DSP代码需要重载，
                      而重载时机SCD中断
SmID : 1)当其=-1时，表示无需对实例相关的变量进行标识
备注:需要在SCD中断重载DSP代码，是源于当前DSP中加载的是Lowdelay处理代码，
     需要等SCD闲时才能重载，否则会出现死机
     add by l00228308
 ***********************************************************************/
VOID SCDDRV_MarkReloadDsp(SINT32 ScdId, SINT32 SmID);

VOID SM_UpdateMatchMode(SINT32 ScdId, SINT32 SmID);

#endif  // __SCD_DRV_H__
