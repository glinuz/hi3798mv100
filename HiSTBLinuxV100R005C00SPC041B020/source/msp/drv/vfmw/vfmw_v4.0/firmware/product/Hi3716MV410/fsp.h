
#ifndef __FSP_HEADER__
#define __FSP_HEADER__

#ifdef __cplusplus
extern "C" {
#endif

#include "public.h"
#include "vfmw.h"
#include "syn_cmn.h"

#define USE_FSP  1


#define FSP_OK     0
#define FSP_ERR   -1
#define FSP_WAIT_ARRANGE 2 //l00273086

#define FS_DISP_STATE_NOT_USED        0
#define FS_DISP_STATE_ALLOCED         1
#define FS_DISP_STATE_DEC_COMPLETE    2
#define FS_DISP_STATE_WAIT_DISP       3
#define FS_DISP_STATE_OCCUPIED     4

#define MAX_PMV_NUM          MAX_PMV_SLOT_NUM


#define  FSP_ASSERT_RET( cond, else_print )								\
    do {																	\
        if( !(cond) )														\
        {																	\
            dprint(PRN_FATAL,"fsp.c,L%d: %s\n", __LINE__, else_print );		\
            return FSP_ERR;													\
        }																	\
    }while(0)

#define  FSP_ASSERT( cond, else_print )									\
    do {																	\
        if( !(cond) )														\
        {																	\
            dprint(PRN_FATAL,"fsp.c,L%d: %s\n", __LINE__, else_print );		\
            return;															\
        }																	\
    }while(0)

/* 物理帧存的类型 */
typedef enum
{
    FSP_PHY_FS_TYPE_DECODE,
    FSP_PHY_FS_TYPE_DISPLAY,
    FSP_PHY_FS_TYPE_BUTT
} FSP_PHY_FS_TYPE_E;

/* 物理帧存的存储方式 */
typedef enum
{
    FSP_PHY_FS_STORE_TYPE_FRAME,
    FSP_PHY_FS_STORE_TYPE_FIELD,
    FSP_PHY_FS_STORE_TYPE_BUTT
} FSP_PHY_FS_STORE_TYPE_E;

typedef enum
{
    LFS_PMV_STATE_NOT_USE,
    LFS_PMV_STATE_ALLOCATED,
    LFS_PMV_STATE_VALID,
    LFS_PMV_STATE_BUTT
} LFS_PMV_STATE_E;

/* FSP实例配置信息 */
typedef struct hiFSP_INST_CFG_S
{
    SINT8     s32TfEnable;
    SINT8     s32RprEnable;
    SINT8     s8SubLevelEnable;
    SINT8     s32UserDec;

    SINT8     s32ExpectedDecFsNum;
    SINT8     s32ExpectedTfFsNum;
    SINT8     s32ExpectedDispFsNum;
    SINT8     s32ExpectedPmvNum;

    SINT32    s32DecFsWidth;
    SINT32    s32DecFsHeight;

    SINT32    s32TfFsWidth;
    SINT32    s32TfFsHeight;

    SINT32    s32DispFsWidth;
    SINT32    s32DispFsHeight;
} FSP_INST_CFG_S;


/* 物理帧存描述子 */
typedef struct hiFSP_PHY_FS_S
{
    SINT8     IsDecRef;
    SINT8     IsTfRef;
    SINT8     DispState;

    SINT32    PhyFsID;
    FSP_PHY_FS_STORE_TYPE_E  eStoreType;
    UADDR     PhyAddr;
    UADDR     LineNumAddr;
    SINT32    Stride;
    //HEVC 10bit
    SINT32    Stride_2bit;
    SINT32    LumaOffset_2bit;
    SINT32    ChromOffset_2bit;
    SINT32    FsWidth;
    SINT32    FsHeight;
    SINT32    ChromOffset;
    SINT32    LumaPixelOffset;
    SINT32    ChromaPixelOffset;
    UINT32    u32Tick;

    UINT8     isAvailable;
} FSP_PHY_FS_S;

typedef struct
{
    FSP_PHY_FS_S phyFS;
    UINT32 isValid;
} FSP_PHY_FS_RECORD_S;


/* 逻辑帧存描述子 */
typedef struct hiFSP_LOGIC_FS_S
{
    SINT8             s32IsRef;
    SINT8             s32DispState;
    SINT8             IsDummyFs;
    SINT8             s32LogicFsID;
    SINT8             PmvIdc;
    IMAGE             stDispImg;

    UINT32            u32Tick;
    LFS_PMV_STATE_E   eLfsPmvState;

    FSP_PHY_FS_S      *pstDecodeFs;
    FSP_PHY_FS_S      *pstDispOutFs;
    FSP_PHY_FS_S      *pstTfOutFs;

    UADDR             PmvAddr;
    SINT32            HalfPmvOffset;

    UINT32            TR;
} FSP_LOGIC_FS_S;

/* 帧存备份信息结构体 */
typedef struct hiFSP_PRE_INF_S
{
    SINT32           s32PreWidth;      // 记录前一次的宽高及解码帧存数，用于 4+64 平滑切换。
    SINT32           s32PreHeight;
    SINT32           s32PreDecNum;   // 解码帧存大小由宽高决定，同时要记录个数，才能判断解码帧存地址是否发生变化
    SINT32           s32PreDecSlotLen;

} FSP_PRE_INF_S;

/* FSP实例状态描述 */
typedef enum hiFSP_INST_STATE_E
{
    FSP_INST_STATE_NULL,
    FSP_INST_STATE_INUSE,
    FSP_INST_STATE_BUTT
} FSP_INST_STATE_E;


/*****************帧存划分状态描述 START l00273086***************/
typedef enum hiFSP_PARTITION_STATE
{
    FSP_PARTITION_STATE_FIRST = 0, /*第一次划分*/
    FSP_PARTITION_STATE_WAIT,      /*等待划分*/
    FSP_PARTITION_STATE_SUCCESS,   /*划分成功*/
    FSP_PARTITION_STATE_FAIL,      /*划分失败*/
} FSP_PARTITION_STATE_E;
/*****************帧存划分状态描述 END   l00273086***************/

/* 帧存池实例 */
typedef struct hiFSP_INST_S
{
    /* 物理帧存实体 */
    SINT8             s32InstID;
    SINT8             s8LowBufEnable;
    FSP_INST_STATE_E  eFspInstState;

    SINT8             s8SizeChangeFlag;

    SINT8             s8TotalValidFsNum;
    SINT8             s32DecFsNum;
    SINT8             s32DispFsNum;
    SINT8             s32TfFsNum;

    SINT8             s32PmvNum;

#ifndef Hi3716MV410
    SINT8             IsPmvInUse[MAX_PMV_NUM];
#endif

    UINT32            u32InstTick;

    FSP_PRE_INF_S     stPreInfo;
    FSP_INST_CFG_S    stInstCfg;

    FSP_LOGIC_FS_S    stLogicFs[MAX_FRAME_NUM];// 64
#ifndef Hi3716MV410
    FSP_PHY_FS_S      stDummyPhyFs;

    FSP_PHY_FS_S      stDecFs[MAX_FRAME_NUM]; //64
    FSP_PHY_FS_S      stDispFs[MAX_FRAME_NUM]; //64
    FSP_PHY_FS_S      stTfFs[2];
#else
    FSP_PHY_FS_RECORD_S stDummyPhyFsRec;

    FSP_PHY_FS_RECORD_S stDecFsRec[MAX_FRAME_NUM]; //64
    FSP_PHY_FS_RECORD_S stDispFsRec[MAX_FRAME_NUM]; //64
    FSP_PHY_FS_RECORD_S stTfFsRec[2];
#endif

    /* PMV存储实体 */
    SINT32            s32HalfPmvOffset;

#ifndef Hi3716MV410
    UADDR             PmvAddr[MAX_PMV_NUM]; //20
#else
    PMV_RECORD_S pmvRec[MAX_PMV_NUM];
#endif
    /*帧存划分状态*/ //l00273086
    FSP_PARTITION_STATE_E eFspPartitionState;

    UINT32 configuredFrameNum;
    UINT32 configuredPmvNum;

} FSP_INST_S;

typedef struct hiFSP_ATTR_S
{
    SINT8             s8LowBufEnable;
} FSP_ATTR_S;

/* 帧存池实例常用信息 */
typedef struct FSP_INST_INFO_S
{
    /* PMV存储实体 */
    SINT8             s32PmvNum;
    /* 物理帧存实体 */
    SINT8             s32DecFsNum;
    SINT8             s32DispFsNum;
    SINT8             s32TfFsNum;
    SINT8             s32InstID;

#ifndef Hi3716MV410
    FSP_PHY_FS_S      stDecFs[MAX_FRAME_NUM];
#else
    FSP_PHY_FS_RECORD_S stDecFsRec[MAX_FRAME_NUM];
#endif

    SINT32            s32DecFsSize;
} FSP_INST_INFO_S;


/***********************************************************************
    全局初始化
 ***********************************************************************/
VOID FSP_Init(VOID);

/***********************************************************************
    全局退出
 ***********************************************************************/
VOID  FSP_Exit(VOID);

/***********************************************************************
    帧存实例特性配置
 ***********************************************************************/
SINT32 FSP_ConfigAttribute(SINT32 InstID, FSP_ATTR_S *pstAttr);

/***********************************************************************
    使能实例
    成功返回实例FSP_OK，失败返回FSP_ERR
 ***********************************************************************/
SINT32  FSP_EnableInstance(SINT32  InstID);

/***********************************************************************
    销毁实例
 ***********************************************************************/
VOID  FSP_DisableInstance(SINT32 InstID);

/***********************************************************************
    复位实例
    成功返回实例FSP_OK，失败返回FSP_ERR
 ***********************************************************************/
SINT32  FSP_ResetInstance(SINT32  InstID);

/***********************************************************************
    清空实例
    在极端情况下，如果解码出现严重错误，协议层可能会要求清除所有解码上下
    文，其中包括清空FSP实例，将所有帧存都标记为不参考，不显示，等待新一轮
    解码使用。
    与FSP_ResetInstance()不同的是，本函数不清除配置信息，也不清除帧存的分配
    信息，仅仅清除所有帧的占用信息。
    成功返回实例FSP_OK，失败返回FSP_ERR
 ***********************************************************************/
SINT32  FSP_EmptyInstance(SINT32  InstID);

SINT32 FSP_GetUsedBufferNum(SINT32 InstID);
/***********************************************************************
    清空没有被放到VO QUEUE 的逻辑帧
 ***********************************************************************/
SINT32 FSP_ClearNotInVoQueue(SINT32 InstID, IMAGE_VO_QUEUE *pstVoQue);

/***********************************************************************
    配置实例
 ***********************************************************************/
SINT32  FSP_ConfigInstance( SINT32 InstID,  FSP_INST_CFG_S *pstFspInstCfg );

/***********************************************************************
    帧存分割
 ***********************************************************************/
SINT32  FSP_PartitionFsMemory ( SINT32 InstID, UADDR MemPhyAddr, SINT32 MemSize, SINT32 *UsedMemSize );

#ifdef Hi3716MV410
SINT32 FSP_ConfigFrameBuf(SINT32 InstID, UADDR MemPhyAddr, SINT32 MemSize, SINT32 *UsedMemSize);
#endif

/***********************************************************************
    获取解码帧地址表
    VDH配置解码帧地址时，要求按"解码帧地址表 + 帧ID"的方式配
 ***********************************************************************/
VOID  FSP_GetDecodeFrameTable(SINT32 InstID, UADDR PhyAddr[], SINT32 *pFsNum);

/***********************************************************************
    获取解码PMV地址表
    VDH配置PMV地址时，要求按"PMV地址表 + PMV ID"的方式配
 ***********************************************************************/
VOID  FSP_GetDecodePmvTable(SINT32 InstID, UADDR PhyAddr[], SINT32 *pPmvNum);

/***********************************************************************
    获取新的逻辑帧
    InstID: 实例ID
    NeedPhyFs: 标志是否需要分配物理帧存。H.264在解码gap时也要分配帧存，
               但这种帧存不需要给真实的物理帧，以此区分.

    成功返回逻辑帧的ID，失败返回-1
 ***********************************************************************/
SINT32  FSP_NewLogicFs ( SINT32 InstID, SINT32 NeedPhyFs );

/***********************************************************************
    清除逻辑帧
    如果要刻意丢弃一个已经申请到的逻辑帧，可以分别把它设置为不解码参考，
    不待显示，不作TF参考等，但最好调用此函数，使用简单且能保证把这个逻辑
    帧上关联的资源完全释放掉
    HoldDispFs: 指示是否需要保留待显示的显示帧存，1表示如果逻辑帧内的显示
                帧存已插入显示队列则保留，否则把整个逻辑帧都清除掉。
                无论HoldDispFs为何值，当前逻辑帧的参考特性一定会被清除.
 ***********************************************************************/
VOID FSP_ClearLogicFs( SINT32 InstID, SINT32 LogicFrameID, SINT32 HoldDispFs);

/***********************************************************************
    设置逻辑帧的参考属性, 包括解码帧和TF参考帧
    IsRef = 1标记为作参考，反之该逻辑帧标记为不参考
 ***********************************************************************/
VOID  FSP_SetRef ( SINT32 InstID, SINT32 LogicFrameID, SINT32 IsRef );

/***********************************************************************
    获取逻辑帧的解码参考属性
 ***********************************************************************/
SINT32  FSP_GetRef ( SINT32 InstID, SINT32 LogicFrameID );

/***********************************************************************
    设置逻辑帧是否等待显示
 ***********************************************************************/
VOID  FSP_SetDisplay ( SINT32 InstID, SINT32 LogicFrameID, SINT32 IsWaitDisp );

/***********************************************************************
    获取逻辑帧是否等待显示
 ***********************************************************************/
SINT32  FSP_GetDisplay ( SINT32 InstID, SINT32 LogicFrameID );

/***********************************************************************
    设置物理帧存的存储方式(按帧存储/按场存储)
 ***********************************************************************/
SINT32  FSP_SetStoreType( SINT32 InstID, FSP_PHY_FS_TYPE_E ePhyFsType, SINT32 PhyFsID, FSP_PHY_FS_STORE_TYPE_E eStoreType);

/***********************************************************************
    获取物理帧存的存储方式(按帧存储/按场存储)
 ***********************************************************************/
FSP_PHY_FS_STORE_TYPE_E  FSP_GetStoreType( SINT32 InstID, FSP_PHY_FS_TYPE_E ePhyFsType, SINT32 PhyFsID);

/***********************************************************************
    删除指定逻辑帧的PMV块
    PMV块与每一个解码帧绑定，在将一帧标记为参考时，它的PMV块也被锁定。但
    不是每一个参考帧都需要PMV块(前向参考帧的PMV无需保存)，所以在一个参考
    帧的PMV块不需要保存时将它删掉，可保证最大程度地节省内存。
    如果PMV块足够(例如MPEG2协议下3个PMV块)，本函数不使用也可以正常运行.
 ***********************************************************************/
VOID  FSP_RemovePmv ( SINT32 InstID, SINT32 LogicFrameID );


/***********************************************************************
    获取逻辑帧
 ***********************************************************************/
FSP_LOGIC_FS_S  *FSP_GetLogicFs( SINT32 InstID, SINT32 LogicFrameID);

/***********************************************************************
    获取逻辑帧的输出图像(指向IMAGE结构体的指针)
 ***********************************************************************/
IMAGE  *FSP_GetFsImagePtr( SINT32 InstID, SINT32 LogicFrameID);

/***********************************************************************
    获取解码物理帧存
 ***********************************************************************/
FSP_PHY_FS_S  *FSP_GetDecPhyFs( SINT32 InstID, SINT32 LogicFrameID);

/***********************************************************************
    获取输出物理帧存
 ***********************************************************************/
FSP_PHY_FS_S  *FSP_GetDispPhyFs( SINT32 InstID, SINT32 LogicFrameID);

/***********************************************************************
    获取TF参考物理帧存
 ***********************************************************************/
FSP_PHY_FS_S  *FSP_GetTfPhyFs( SINT32 InstID, SINT32 LogicFrameID);

/***********************************************************************
    获取PMV地址
 ***********************************************************************/
UADDR FSP_GetPmvAddr( SINT32 InstID, SINT32 LogicFrameID );

/***********************************************************************
    判断是否还能申请到新帧
    如果能申请到，返回1，否则返回0
 ***********************************************************************/
SINT32  FSP_IsNewFsAvalible(SINT32 InstID);

/***********************************************************************
 检测是否发生了帧存(主要是显示帧存)泄漏
 ***********************************************************************/
SINT32  FSP_IsFsLeak(SINT32 InstID);

/***********************************************************************
    获取DecFs地址表
    264协议用地址表+ID的方式给逻辑配置地址信息，所以需要获取解码帧存地址表
 ***********************************************************************/
VOID  FSP_GetDecFsAddrTab( SINT32 InstID, SINT32 *DecFsNum, SINT32 *DecFsAddrTab , SINT32 *LineNumAddrTab);

/***********************************************************************
    获取PMV地址表
    264协议用地址表+ID的方式给逻辑配置PMV地址信息，所以需要获取PMV地址表
 ***********************************************************************/
VOID  FSP_GetPmvAddrTab( SINT32 InstID, SINT32 *PmvNum, SINT32 *PmvAddrTab );

/***********************************************************************
    打印FSP内部状态
    用于实时运行时，利用特定命令探测FSP内部状态
 ***********************************************************************/
VOID FSP_PrintContext(SINT32 InstID);

/***********************************************************************
    获取当前帧存实例相关信息
 ***********************************************************************/
VOID  FSP_GetInstInfo( SINT32 InstID, FSP_INST_INFO_S *pFspInstInfo );

/***********************************************************************
    通过物理地址获取逻辑帧存ID
 ***********************************************************************/
SINT32  FSP_GetLogicIdByPhyAddr( SINT32 InstID, UADDR PhyAddr);

/***********************************************************************
    获取清空标记
 ***********************************************************************/
UINT8 FSP_GetQueSwitchFlag(SINT32 InstID);

/***********************************************************************
    获取设置清空标记
 ***********************************************************************/
VOID FSP_SetQueSwitchFlag(SINT32 InstID);

/***********************************************************************
    获取空闲逻辑帧存ID
    参考帧复位时用
 ***********************************************************************/
SINT32 FSP_GetNullLogicFs(SINT32 InstID);

/***********************************************************************
    帧存队列处理函数
    当VO 取走一帧时，记录下该帧物理地址
    当VO 释放一帧时，清楚队列中该帧物理地址记录
 ***********************************************************************/
VOID  FSP_ProcFspQue ( SINT32 InstID, IMAGE *p_image, SINT32 Flag );

/***********************************************************************
    换台时，设置标记及保存FSP 队列信息
 ***********************************************************************/
VOID  FSP_SwitchNotify (SINT32 InstID);

/***********************************************************************
    获取当前帧存使用方式
    1:树状帧存，0:原帧存，-1:均未使用
 ***********************************************************************/
SINT32 FSP_GetFspType(SINT32 InstID);

/***********************************************************************
    查询是否有帧存上限递增空间
 ***********************************************************************/
SINT32 FSP_GetTotalValidFsNum(SINT32 InstID);


/***********************************************************************
	外部帧存设置接口
 ***********************************************************************/
#ifdef VFMW_BVT_SUPPORT
SINT32 FSP_SetFrameInterface(FSP_FRAME_INTF_S *pFspFrameIntf);
#endif

/***********************************************************************
	更新逻辑帧存stride等相关信息设置接口
 ***********************************************************************/
#ifndef Hi3716MV410
SINT32 FSP_UdateInfo(SINT32 InstID, UINT32 Width, UINT32 Height);
#endif


#ifdef __cplusplus
}
#endif

#endif

