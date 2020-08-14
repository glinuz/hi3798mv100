/******************************************************************************

Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_mpi_demux.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2009-12-14
Last Modified :
Description   : MPI Layer function declaration for linux
Function List :
History       :
* main\1    2009-12-14   jianglei     init.
******************************************************************************/

#ifndef __HI_MPI_DEMUX_H__
#define __HI_MPI_DEMUX_H__

#include "hi_type.h"

#include "hi_unf_demux.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/***************************** Macro Definition ******************************/

#define DMX_DMXID(ChanHandle)  ( ((ChanHandle) & 0x0000ff00) >> 8)

#define DMX_CHANHANDLE_MAGIC      (0x1)
#define DMX_CHANID(ChanHandle)      ((ChanHandle) & 0x000000ff)
#define DMX_CHANHANDLE(DmxId, ChanId)    ((HI_ID_DEMUX << 28) | (DMX_CHANHANDLE_MAGIC << 24) | \
                    ((0 << 16) & 0xff0000) | ((DmxId << 8) & 0x0000ff00) | (ChanId & 0x000000ff))
/* 
 * compatible and extend(bit16 ~ bit24) 
 */
#define DMX_CHANHANDLE2(DmxId, ChanId, Mode) ((HI_ID_DEMUX << 28) | (DMX_CHANHANDLE_MAGIC << 24) | \
                    ((Mode << 16) & 0xff0000) | ((DmxId << 8) & 0x0000ff00) | (ChanId & 0x000000ff))

#define __DMX_CHECK_CHANHANDLE(ChanHandle) ({\
    HI_S32 ret = HI_SUCCESS; \
    if (((ChanHandle >> 28) & 0x0000000f) != HI_ID_DEMUX || ((ChanHandle >> 24) & 0x0000000f) != DMX_CHANHANDLE_MAGIC) \
    {\
        ret = HI_ERR_DMX_INVALID_PARA;\
    }\
    ret; \
})

#define DMX_CHECK_CHANHANDLE(ChanHandle)                                \
    do                                                                  \
    {                                                                   \
        if (HI_SUCCESS != __DMX_CHECK_CHANHANDLE(ChanHandle)) \
        { \
            HI_ERR_DEMUX("Invalid Channel handle(0x%x)\n", ChanHandle); \
            return HI_ERR_DMX_INVALID_PARA; \
        }\
    } while (0)

#define DMX_FLTHANDLE_MAGIC      (0x2)
#define DMX_FLTID(FilterHandle)     ((FilterHandle) & 0x000000ff)
#define DMX_FLTHANDLE(DmxId, FilterId)     ((HI_ID_DEMUX << 24) | (DMX_FLTHANDLE_MAGIC << 16) | ((DmxId << 8) & 0x0000ff00) | (FilterId & 0x000000ff))
#define DMX_CHECK_FLTHANDLE(FilterHandle)                               \
    do                                                                  \
    {                                                                   \
        if (((FilterHandle >> 24) & 0x000000ff) != HI_ID_DEMUX || ((FilterHandle >> 16) & 0x000000ff) != DMX_FLTHANDLE_MAGIC)\
        {                                                               \
            HI_ERR_DEMUX("Invalid Filter Handle(0x%x)\n", FilterHandle); \
            return HI_ERR_DMX_INVALID_PARA;                             \
        }                                                               \
    } while (0)

#define DMX_RECHANDLE_MAGIC      (0x3)
#define DMX_RECID(RecHandle)    ((RecHandle) & 0x000000ff)
#define DMX_RECHANDLE(DmxId, RecId)    ((HI_ID_DEMUX << 24) | (DMX_RECHANDLE_MAGIC << 16) | ((DmxId << 8) & 0x0000ff00) | (RecId & 0x000000ff))
#define DMX_CHECK_RECHANDLE(RecHandle)                                  \
    do                                                                  \
    {                                                                   \
        if (((RecHandle >> 24) & 0x000000ff) != HI_ID_DEMUX || ((RecHandle >> 16) & 0x000000ff) != DMX_RECHANDLE_MAGIC) \
        {                                                               \
            HI_ERR_DEMUX("Invalid Rec Handle 0x%x\n", RecHandle);          \
            return HI_ERR_DMX_INVALID_PARA;                             \
        }                                                               \
    } while (0)

#define DMX_PCRHANDLE_MAGIC      (0x4)
#define DMX_PCRID(PcrHandle)    ((PcrHandle) & 0x000000ff)
#define DMX_PCRHANDLE(DmxId, PcrId)    ((HI_ID_DEMUX << 24) | (DMX_PCRHANDLE_MAGIC << 16) | ((DmxId << 8) & 0x0000ff00) | (PcrId & 0x000000ff))
#define DMX_CHECK_PCRHANDLE(PcrHandle)                                  \
    do                                                                  \
    {                                                                   \
        if (((PcrHandle >> 24) & 0x000000ff) != HI_ID_DEMUX || ((PcrHandle >> 16) & 0x000000ff) != DMX_PCRHANDLE_MAGIC) \
        {                                                               \
            HI_ERR_DEMUX("Invalid Pcr Handle 0x%x\n", PcrHandle);          \
            return HI_ERR_DMX_INVALID_PARA;                             \
        }                                                               \
    } while (0)


#define RMX_HANDLE_MAGIC         (0x5)
#define RMX_RMXID(Handle)   ( ((Handle) & 0x0000ff00) >> 8)
#define RMX_ID(Handle)    ((Handle) & 0x000000ff)
#define RMX_HANDLE(RmxId, ChanId) ((HI_ID_DEMUX << 24) | (RMX_HANDLE_MAGIC << 16) | ((RmxId << 8) & 0x0000ff00) | (ChanId & 0x000000ff))
#define RMX_CHECK_HANDLE(Handle)                                \
    do                                                                  \
    {                                                                   \
        if (((Handle >> 24) & 0x0000000f) != HI_ID_DEMUX || ((Handle >> 16) & 0x0000000f) != RMX_HANDLE_MAGIC) \
        {\
            HI_ERR_DEMUX("Invalid remux handle(0x%x)\n", Handle); \
            return HI_ERR_DMX_INVALID_PARA;\
        }\
    } while (0)

#define RMX_PUMP_HANDLE_MAGIC         (0x6)
#define RMX_PUMPID(Handle)    ((Handle) & 0x00ff0000)
#define RMX_PUMPHANDLE(RmxId, ChanId, RmxPumpId) ((HI_ID_DEMUX << 28) | (RMX_PUMP_HANDLE_MAGIC << 24) |((RmxPumpId << 16) & 0xff0000) | ((RmxId << 8) & 0x0000ff00) | (ChanId & 0x000000ff))
#define RMX_CHECK_PUMPHANDLE(Handle)                                \
    do                                                                  \
    {                                                                   \
        if (((Handle >> 28) & 0x0000000f) != HI_ID_DEMUX || ((Handle >> 24) & 0x0000000f) != RMX_PUMP_HANDLE_MAGIC) \
        {\
            HI_ERR_DEMUX("Invalid remux pump handle(0x%x)\n", Handle); \
            return HI_ERR_DMX_INVALID_PARA;\
        }\
    } while (0)
    
/*************************** Struct Definition ****************************/
typedef struct hiMPI_DMX_BUF_STATUS_S
{
    HI_U32 u32BufSize;  /*buffer size */
    HI_U32 u32UsedSize; /* buffer used size *//*CNcomment:缓冲区已使用大小 */
    HI_U32 u32BufRptr;  /*buffer read pointer *//*CNcomment:缓冲区读指针*/
    HI_U32 u32BufWptr;/*buffer written pointer *//*CNcomment:缓冲区写指针*/
} HI_MPI_DMX_BUF_STATUS_S;

typedef enum
{
    DMX_MMZ_BUF = 0,
    DMX_MMU_BUF,
    DMX_SECURE_BUF
}DMX_BUF_FLAG_E;

typedef struct hiDMX_BUF_S
{
    HI_U8   *VirAddr;         /* Virtual address of a buffer. */
    HI_U32  PhyAddr;        /* Physical address of a buffer. */
    HI_U32  Size;               /* Buffer size, in the unit of byte. */
    DMX_BUF_FLAG_E Flag;
} DMX_BUF_S;

/******************************* API declaration *****************************/
HI_S32 HI_MPI_DMX_Init(HI_VOID);
HI_S32 HI_MPI_DMX_DeInit(HI_VOID);
HI_S32 HI_MPI_DMX_GetCapability(HI_UNF_DMX_CAPABILITY_S *pstCap);

/* Port */
HI_S32 HI_MPI_DMX_GetTSPortAttr(HI_UNF_DMX_PORT_E enPortId, HI_UNF_DMX_PORT_ATTR_S *pstAttr);
HI_S32 HI_MPI_DMX_SetTSPortAttr(HI_UNF_DMX_PORT_E enPortId, const HI_UNF_DMX_PORT_ATTR_S *pstAttr);
HI_S32 HI_MPI_DMX_GetTSOPortAttr(HI_UNF_DMX_TSO_PORT_E enPortId, HI_UNF_DMX_TSO_PORT_ATTR_S *pstAttr);
HI_S32 HI_MPI_DMX_SetTSOPortAttr(HI_UNF_DMX_TSO_PORT_E enPortId, const HI_UNF_DMX_TSO_PORT_ATTR_S *pstAttr);
HI_S32 HI_MPI_DMX_GetDmxTagAttr(HI_U32 u32DmxId, HI_UNF_DMX_TAG_ATTR_S *pstAttr);
HI_S32 HI_MPI_DMX_SetDmxTagAttr(HI_U32 u32DmxId, const HI_UNF_DMX_TAG_ATTR_S *pstAttr);
HI_S32 HI_MPI_DMX_AttachTSPort(HI_U32 u32DmxId, HI_UNF_DMX_PORT_E enPortId);
HI_S32 HI_MPI_DMX_DetachTSPort(HI_U32 u32DmxId);
HI_S32 HI_MPI_DMX_GetTSPortId(HI_U32 u32DmxId, HI_UNF_DMX_PORT_E *penPortId);
HI_S32 HI_MPI_DMX_GetTSPortPacketNum(HI_UNF_DMX_PORT_E enPortId, HI_UNF_DMX_PORT_PACKETNUM_S *sPortStat);

/* TS Buffer */
HI_S32 HI_MPI_DMX_GetTSBufferDefaultAttr(HI_UNF_DMX_TSBUF_ATTR_S *pstTsBufAttr);
HI_S32 HI_MPI_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_E enPortId, const HI_UNF_DMX_TSBUF_ATTR_S *pstTsBufAttr, HI_HANDLE *phTsBuffer);
HI_S32 HI_MPI_DMX_DestroyTSBuffer(HI_HANDLE hTsBuffer);
HI_S32 HI_MPI_DMX_GetTSBuffer(HI_HANDLE hTsBuffer, HI_U32 u32ReqLen, HI_UNF_STREAM_BUF_S *pstData, HI_U32 *pu32PhyAddr, HI_U32 u32TimeOutMs);
HI_S32 HI_MPI_DMX_PushTSBuffer(HI_HANDLE hTsBuffer, const HI_UNF_STREAM_BUF_S *pstData);
HI_S32 HI_MPI_DMX_ReleaseTSBuffer(HI_HANDLE hTsBuffer, const HI_UNF_STREAM_BUF_S *pstData);
HI_S32 HI_MPI_DMX_PutTSBuffer(HI_HANDLE hTsBuffer, HI_U32 u32ValidDataLen, HI_U32 u32StartPos);
HI_S32 HI_MPI_DMX_ResetTSBuffer(HI_HANDLE hTsBuffer);
HI_S32 HI_MPI_DMX_GetTSBufferStatus(HI_HANDLE hTsBuffer, HI_UNF_DMX_TSBUF_STATUS_S *pStatus);
HI_S32 HI_MPI_DMX_GetTSBufferPortId(HI_HANDLE hTsBuffer, HI_UNF_DMX_PORT_E *penPortId);
HI_S32 HI_MPI_DMX_GetTSBufferHandle(HI_UNF_DMX_PORT_E enPortId, HI_HANDLE *phTsBuffer);

/* Channel */
/*Buffer for video and audio channel should be attached,it's meaningless to config when apply them *//*CNcomment: 音视频通道的buffer需要绑定，在申请时配置没有意义*/
HI_S32 HI_MPI_DMX_GetPortMode(HI_U32 u32DmxId, HI_UNF_DMX_PORT_MODE_E *penPortMod);
HI_S32 HI_MPI_DMX_GetChannelDefaultAttr(HI_UNF_DMX_CHAN_ATTR_S *pstChAttr);
HI_S32 HI_MPI_DMX_CreateChannel(HI_U32 u32DmxId, const HI_UNF_DMX_CHAN_ATTR_S *pstChAttr, HI_HANDLE *phChannel);
HI_S32 HI_MPI_DMX_CreateChannelWithPID(HI_U32 u32DmxId, HI_U32 u32Pid, const HI_UNF_DMX_CHAN_ATTR_S *pstChAttr, HI_HANDLE *phChannel);
HI_S32 HI_MPI_DMX_DestroyChannel(HI_HANDLE hChannel);
HI_S32 HI_MPI_DMX_GetChannelAttr(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_ATTR_S *pstChAttr);
HI_S32 HI_MPI_DMX_SetChannelAttr(HI_HANDLE hChannel, const HI_UNF_DMX_CHAN_ATTR_S *pstChAttr);
HI_S32 HI_MPI_DMX_SetChannelPID(HI_HANDLE hChannel, HI_U32 u32Pid);
HI_S32 HI_MPI_DMX_GetChannelPID(HI_HANDLE hChannel, HI_U32 *pu32Pid);
HI_S32 HI_MPI_DMX_OpenChannel(HI_HANDLE hChannel);
HI_S32 HI_MPI_DMX_CloseChannel(HI_HANDLE hChannel);
HI_S32 HI_MPI_DMX_GetChannelStatus(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_STATUS_S *pstStatus);
HI_S32 HI_MPI_DMX_GetChannelHandle(HI_U32 u32DmxId, HI_U32 u32Pid, HI_HANDLE *phChannel);
HI_S32 HI_MPI_DMX_GetFreeChannelCount(HI_U32 u32DmxId, HI_U32 *pu32FreeCount);
HI_S32 HI_MPI_DMX_GetScrambledFlag(HI_HANDLE hChannel, HI_UNF_DMX_SCRAMBLED_FLAG_E *penScrambleFlag);
HI_S32 HI_MPI_DMX_SetChannelEosFlag(HI_HANDLE hChannel);
HI_S32 HI_MPI_DMX_GetChannelTsCount(HI_HANDLE hChannel, HI_U32 *pu32TsCount);


/* Filter */
HI_S32 HI_MPI_DMX_CreateFilter(HI_U32   u32DmxId, const HI_UNF_DMX_FILTER_ATTR_S  *pstFilterAttr,
            HI_HANDLE *phFilter);
HI_S32 HI_MPI_DMX_DestroyFilter(HI_HANDLE hFilter);
HI_S32 HI_MPI_DMX_DeleteAllFilter(HI_HANDLE hChannel);
HI_S32 HI_MPI_DMX_SetFilterAttr(HI_HANDLE hFilter, const HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr);
HI_S32 HI_MPI_DMX_GetFilterAttr(HI_HANDLE hFilter, HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr );
HI_S32 HI_MPI_DMX_AttachFilter(HI_HANDLE hFilter, HI_HANDLE hChannel);
HI_S32 HI_MPI_DMX_DetachFilter(HI_HANDLE hFilter, HI_HANDLE hChannel);
HI_S32 HI_MPI_DMX_GetFilterChannelHandle(HI_HANDLE hFilter, HI_HANDLE *phChannel);
HI_S32 HI_MPI_DMX_GetFreeFilterCount(HI_U32 u32DmxId ,  HI_U32 * pu32FreeCount);


/* Data receive */
HI_S32  HI_MPI_DMX_GetDataHandle(HI_HANDLE *phChannel, HI_U32 *pu32ChNum,
            HI_U32 u32TimeOutMs);

HI_S32  HI_MPI_DMX_SelectDataHandle(HI_HANDLE *phWatchChannel, HI_U32 u32WatchNum,
            HI_HANDLE *phDataChannel, HI_U32 *pu32ChNum, HI_U32 u32TimeOutMs);

HI_S32  HI_MPI_DMX_AcquireBuf(HI_HANDLE hChannel, HI_U32 u32AcquireNum,
            HI_U32 * pu32AcquiredNum, HI_UNF_DMX_DATA_S *pstBuf,
            HI_U32 u32TimeOutMs);
HI_S32  HI_MPI_DMX_ReleaseBuf(HI_HANDLE hChannel, HI_U32 u32ReleaseNum,
            HI_UNF_DMX_DATA_S *pstBuf);

/* PCR */
HI_S32 HI_MPI_DMX_CreatePcrChannel(HI_U32 u32DmxId, HI_U32 *pu32PcrChId);
HI_S32 HI_MPI_DMX_DestroyPcrChannel(HI_U32 u32PcrChId);
HI_S32 HI_MPI_DMX_PcrPidSet(HI_U32 pu32PcrChId, HI_U32 u32Pid);
HI_S32 HI_MPI_DMX_PcrPidGet(HI_U32 pu32PcrChId, HI_U32 *pu32Pid);
HI_S32 HI_MPI_DMX_PcrScrGet(HI_U32 pu32PcrChId, HI_U64 *pu64PcrMs,HI_U64 *pu64ScrMs);
HI_S32 HI_MPI_DMX_PcrSyncAttach(HI_U32 u32PcrChId, HI_U32 u32SyncHandle);
HI_S32 HI_MPI_DMX_PcrSyncDetach(HI_U32 u32PcrChId);

/*Only video and audio channel are enable to attach PES Buffer *//*CNcomment:   只有音视频通道允许绑定和解绑定PES Buffer*/
HI_S32 HI_MPI_DMX_GetPESBufferStatus(HI_HANDLE hChannel, HI_MPI_DMX_BUF_STATUS_S *pBufStat);

/*Be used to send stream to user status decoder *//*CNcomment:   用于给用户态的音频解码送码流*/
HI_S32 HI_MPI_DMX_AcquireEs(HI_HANDLE hChannel, HI_UNF_ES_BUF_S *pAudioEsBuf);
HI_S32 HI_MPI_DMX_ReleaseEs(HI_HANDLE hChannel,const HI_UNF_ES_BUF_S *pAudioEsBuf);

/***********************recoder type***************/
/* Sequence of DMX_IDX_DATA_S 's member can not change,must match the sequence defined by hardware*/
/* CNcomment:DMX_IDX_DATA_S 中各成员的顺序不能改变，必须与硬件规定的顺序保持一致 */
typedef struct hiDMX_IDX_DATA_S
{
    HI_U32 u32Chn_Ovflag_IdxType_Flags;
    HI_U32 u32ScType_Byte12AfterSc_OffsetInTs;
    HI_U32 u32TsCntLo32;
    HI_U32 u32TsCntHi8_Byte345AfterSc;
    HI_U32 u32ScCode_Byte678AfterSc;
    HI_U32 u32SrcClk;
    HI_U32 u32BackPacetNum;/*Back package number*//* CNcomment:回退包计数*/
} DMX_IDX_DATA_S;

typedef struct hiMPI_DMX_REC_INDEX_S
{
    HI_UNF_VIDEO_FRAME_TYPE_E   enFrameType;
    HI_U32                      u32PtsMs;
    HI_U64                      u64GlobalOffset;
    HI_U32                      u32FrameSize;      
    HI_U32                      u32DataTimeMs;

    /* hevc private */
    HI_S16                      s16CurPoc;
    HI_U16                      u16RefPocCnt;
    HI_S16                      as16RefPoc[16]; /* according to hevc protocol , max reference poc is 16. */
} HI_MPI_DMX_REC_INDEX_S;

typedef HI_UNF_DMX_REC_DATA_S HI_MPI_DMX_REC_DATA_S;

typedef struct hiMPI_DMX_REC_DATA_INDEX_S
{          
    HI_U32 u32IdxNum; 
    HI_U32 u32RecDataCnt;
    HI_MPI_DMX_REC_INDEX_S stIndex[DMX_MAX_IDX_ACQUIRED_EACH_TIME];
    HI_MPI_DMX_REC_DATA_S  stRecData[2];
} HI_MPI_DMX_REC_DATA_INDEX_S;

HI_S32 HI_MPI_DMX_CreateRecChn(HI_UNF_DMX_REC_ATTR_S *pstRecAttr, HI_HANDLE *phRecChn);
HI_S32 HI_MPI_DMX_DestroyRecChn(HI_HANDLE hRecChn);

HI_S32 HI_MPI_DMX_AddRecPid(HI_HANDLE hRecChn, HI_U32 u32Pid, HI_HANDLE *phChannel);
HI_S32 HI_MPI_DMX_DelRecPid(HI_HANDLE hRecChn, HI_HANDLE hChannel);
HI_S32 HI_MPI_DMX_DelAllRecPid(HI_HANDLE hRecChn);

HI_S32 HI_MPI_DMX_AddExcludeRecPid(HI_HANDLE hRecChn, HI_U32 u32Pid);
HI_S32 HI_MPI_DMX_DelExcludeRecPid(HI_HANDLE hRecChn, HI_U32 u32Pid);
HI_S32 HI_MPI_DMX_DelAllExcludeRecPid(HI_HANDLE hRecChn);

HI_S32 HI_MPI_DMX_StartRecChn(HI_HANDLE hRecChn);
HI_S32 HI_MPI_DMX_StopRecChn(HI_HANDLE hRecChn);

HI_S32 HI_MPI_DMX_AcquireRecData(HI_HANDLE hRecChn, HI_MPI_DMX_REC_DATA_S *pstRecData, HI_U32 u32TimeoutMs);
HI_S32 HI_MPI_DMX_ReleaseRecData(HI_HANDLE hRecChn, const HI_MPI_DMX_REC_DATA_S *pstRecData);

HI_S32 HI_MPI_DMX_AcquireRecIndex(HI_HANDLE hRecChn, HI_MPI_DMX_REC_INDEX_S *pstRecIndex, HI_U32 u32TimeoutMs);
HI_S32 HI_MPI_DMX_AcquireRecDataAndIndex(HI_HANDLE hRecChn, HI_MPI_DMX_REC_DATA_INDEX_S* pstRecDataIdx);
HI_S32 HI_MPI_DMX_ReleaseRecDataAndIndex(HI_HANDLE hRecChn, HI_MPI_DMX_REC_DATA_INDEX_S* pstRecDataIdx);

HI_S32 HI_MPI_DMX_GetRecBufferStatus(HI_HANDLE hRecChn, HI_UNF_DMX_RECBUF_STATUS_S *pstBufStatus);

HI_S32 HI_MPI_DMX_Invoke(HI_UNF_DMX_INVOKE_TYPE_E enCmd, const HI_VOID *pCmdPara);

HI_S32 HI_MPI_DMX_GetCallback(HI_HANDLE hChannel, HI_UNF_DMX_CB_DESC_S *pstCbDesc);
HI_S32 HI_MPI_DMX_RegisterCallback(HI_HANDLE hChannel, HI_UNF_DMX_CB_DESC_S *pstCbDesc);
HI_S32 HI_MPI_DMX_UnRegisterCallback(HI_HANDLE hChannel);

HI_S32 HI_MPI_DMX_GetResumeCount(HI_U32 *pCount);


typedef enum hiMPI_DMX_PORT_E
{
    HI_MPI_DMX_PORT_RMX_0 = 0xa0,

    HI_MPI_DMX_PORT_BUTT
}HI_MPI_DMX_PORT_E;
    
typedef struct hiMPI_RMX_ATTR_S
{
    HI_MPI_DMX_PORT_E enOutPortId;
} HI_MPI_RMX_ATTR_S;

typedef enum hiMPI_RMX_PUMP_TYPE_E
{
    HI_MPI_RMX_PUMP_TYPE_PID,      
    HI_MPI_RMX_PUMP_TYPE_REMAP_PID,
    HI_MPI_RMX_PUMP_TYPE_ALLPASS_PORT,   

    HI_MPI_RMX_PUMP_TYPE_BUTT
} HI_MPI_RMX_PUMP_TYPE_E;

typedef struct hiMPI_RMX_PUMP_ATTR_S
{
    HI_MPI_RMX_PUMP_TYPE_E enPumpType;
    HI_UNF_DMX_PORT_E enInPortId;
    HI_U32 u32Pid;
    HI_U32 u32RemapPid;
} HI_MPI_RMX_PUMP_ATTR_S;

HI_S32 HI_MPI_RMX_GetDefaultAttr(HI_MPI_RMX_ATTR_S * pstAttr);
HI_S32 HI_MPI_RMX_Create(HI_MPI_RMX_ATTR_S * pstAttr, HI_HANDLE *phRmx);
HI_S32 HI_MPI_RMX_Destroy(HI_HANDLE hRmx);
HI_S32 HI_MPI_RMX_GetAttr(HI_HANDLE hRmx, HI_MPI_RMX_ATTR_S * pstAttr);
HI_S32 HI_MPI_RMX_SetAttr(HI_HANDLE hRmx, HI_MPI_RMX_ATTR_S * pstAttr);
HI_S32 HI_MPI_RMX_Start(HI_HANDLE hRmx);
HI_S32 HI_MPI_RMX_Stop(HI_HANDLE hRmx);
HI_S32 HI_MPI_RMX_GetPumpDefaultAttr(HI_MPI_RMX_PUMP_ATTR_S * pstAttr);
HI_S32 HI_MPI_RMX_AddPump(HI_HANDLE hRmx, HI_MPI_RMX_PUMP_ATTR_S *pstAttr, HI_HANDLE *phRmxPump);
HI_S32 HI_MPI_RMX_DelPump(HI_HANDLE hRmxPump);
HI_S32 HI_MPI_RMX_GetPumpAttr(HI_HANDLE hRmxPump, HI_MPI_RMX_PUMP_ATTR_S *pstAttr);
HI_S32 HI_MPI_RMX_SetPumpAttr(HI_HANDLE hRmxPump, HI_MPI_RMX_PUMP_ATTR_S *pstAttr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  // __HI_MPI_DEMUX_H__

