/******************************************************************************

Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_demux_ioctl.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2009/12/14
Last Modified :
Description   : demux
Function List :
History       :
* main\1    2009-12-14   jianglei     init.
******************************************************************************/
#ifndef __DRV_DEMUX_IOCTL_H__
#define __DRV_DEMUX_IOCTL_H__

#include "hi_type.h"

#include "hi_mpi_demux.h"
#include "drv_demux_config.h"
#include "hi_drv_demux.h"


/*************************** Structure Definition ****************************/
typedef struct hiDMX_PoolBuf_Attr_S
{
    HI_U32 BufPhyAddr;
    HI_U32 BufSize;
    DMX_BUF_FLAG_E BufFlag;
}DMX_PoolBuf_Attr_S;

typedef struct hiDMX_Port_GetAttr_S
{
    DMX_PORT_MODE_E         PortMode;
    HI_U32                  PortId;
    HI_UNF_DMX_PORT_ATTR_S  PortAttr;
} DMX_Port_GetAttr_S;

typedef DMX_Port_GetAttr_S DMX_Port_SetAttr_S ;

typedef struct hiDMX_Tag_GetAttr_S
{
    HI_U32 DmxId;
    HI_UNF_DMX_TAG_ATTR_S TagAttr;
}DMX_Tag_GetAttr_S;

typedef DMX_Tag_GetAttr_S DMX_Tag_SetAttr_S;

typedef struct hiDMX_TSO_Port_GetAttr_S
{
    HI_U32                  PortId;
    HI_UNF_DMX_TSO_PORT_ATTR_S  PortAttr;
} DMX_TSO_Port_Attr_S;

typedef struct hiDMX_Port_Attach_S
{
    DMX_PORT_MODE_E PortMode;
    HI_U32          PortId;
    HI_U32          DmxId;
} DMX_Port_Attach_S;

typedef struct hiDMX_PortPacketNum_S
{
    DMX_PORT_MODE_E PortMode;
    HI_U32          PortId;
    HI_U32          TsPackCnt;
    HI_U32          ErrTsPackCnt;
} DMX_PortPacketNum_S;

typedef DMX_Port_Attach_S DMX_Port_GetId_S;

typedef struct hiDMX_TsBufInit_S
{
    HI_U32          PortId;        /* [in] */
    HI_UNF_DMX_TSBUF_ATTR_S Attr;  /* [in] */
    HI_U32          BufPhyAddr;    /* [out] */
    HI_U32          BufSize;       /* [out] */
    DMX_BUF_FLAG_E  BufFlag;       /* [out] */
}DMX_TsBufInit_S;

typedef struct hiDMX_TsBufGet_S
{
    HI_U32          PortId;
    HI_U32          ReqLen;
    HI_U32          BufPhyAddr;
    HI_U32          BufSize;
    HI_U32          TimeoutMs;
} DMX_TsBufGet_S;

typedef struct hiDMX_TsBufPush_S
{
    HI_U32          PortId;
    HI_U32          BufPhyAddr;
    HI_U32          BufSize;
}DMX_TsBufPush_S;

typedef struct hiDMX_TsBufRel_S
{
    HI_U32          PortId;
    HI_U32          BufPhyAddr;
    HI_U32          BufSize;
}DMX_TsBufRel_S;

typedef struct hiDMX_TsBufPut_S
{
    HI_U32  PortId;
    HI_U32  ValidDataLen;
    HI_U32  StartPos;
} DMX_TsBufPut_S;

typedef struct hiDMX_TsBufStaGet_S
{
    HI_U32                      PortId;
    HI_UNF_DMX_TSBUF_STATUS_S   Status;
} DMX_TsBufStaGet_S;

typedef struct hiDMX_ChanNew_S
{
    HI_U32 u32DemuxId;
    HI_U32 u32Pid;
    HI_UNF_DMX_CHAN_ATTR_S stChAttr;
    HI_HANDLE hChannel;
    HI_U32 BufPhyAddr;
    HI_U32 BufSize;
    DMX_BUF_FLAG_E BufFlag;
}DMX_ChanNew_S;

typedef struct hiDMX_GetChan_Attr_S
{
    HI_HANDLE hChannel;
    HI_UNF_DMX_CHAN_ATTR_S stChAttr;
}DMX_GetChan_Attr_S;

typedef DMX_GetChan_Attr_S DMX_SetChan_Attr_S ;


typedef struct hiDMX_ChanPIDSet_S
{
    HI_HANDLE hChannel;
    HI_U32 u32Pid;
}DMX_ChanPIDSet_S;

typedef struct hiDMX_ChanPIDGet_S
{
    HI_HANDLE hChannel;
    HI_U32  u32Pid;
}DMX_ChanPIDGet_S;

typedef struct hiDMX_ChanStatusGet_S
{
    HI_HANDLE hChannel;
    HI_UNF_DMX_CHAN_STATUS_S stStatus;
}DMX_ChanStatusGet_S;

typedef struct hiDMX_ChannelIdGet_S
{
    HI_U32  u32DmxId;
    HI_U32  u32Pid;
    HI_HANDLE hChannel;
}DMX_ChannelIdGet_S;

typedef struct hiDMX_FreeChanGet_S
{
    HI_U32 u32DmxId;
    HI_U32 u32FreeCount;
}DMX_FreeChanGet_S;

typedef struct hiDMX_ScrambledFlagGet_S
{
    HI_HANDLE hChannel;
    HI_UNF_DMX_SCRAMBLED_FLAG_E enScrambleFlag;
}DMX_ScrambledFlagGet_S;

typedef struct
{
    HI_U32                      DmxId;
    HI_UNF_DMX_FILTER_ATTR_S    FilterAttr;
    HI_HANDLE                   Filter;
} DMX_NewFilter_S;

typedef struct
{
    HI_HANDLE                   Filter;
    HI_UNF_DMX_FILTER_ATTR_S    FilterAttr;
} DMX_FilterSet_S;

typedef DMX_FilterSet_S DMX_FilterGet_S;

typedef struct
{
    HI_HANDLE   Filter;
    HI_HANDLE   Channel;
} DMX_FilterAttach_S;

typedef DMX_FilterAttach_S DMX_FilterDetach_S;
typedef DMX_FilterAttach_S DMX_FilterChannelIDGet_S;

typedef struct
{
    HI_U32  DmxId;
    HI_U32  FreeCount;
} DMX_FreeFilterGet_S;

typedef struct hiDMX_GetDataFlag_S
{
    HI_U32 WatchChannelNum;      /* [in] */
    HI_HANDLE *ValidChannel;     /* [out] channel has data ready */
    HI_U32 *ValidChannelNum;     /* [out] channel has data number */
    HI_U32 u32TimeOutMs;
}DMX_GetDataFlag_S;

typedef struct hiDMX_Compat_GetDataFlag_S
{
    HI_U32 WatchChannelNum;      /* [in] */
    HI_U32 ValidChannel;         /* [out] channel has data ready */
    HI_U32 ValidChannelNum;      /* [out] channel has data number */
    HI_U32 u32TimeOutMs;
}DMX_Compat_GetDataFlag_S;

typedef struct hiDMX_SelectDataFlag_S
{
    HI_HANDLE *WatchChannel;     /* [in] channel handles to check*/
    HI_U32 WatchChannelNum;      /* [in] channel number to check*/
    HI_HANDLE *ValidChannel;     /* [out] channel has data ready */
    HI_U32 *ValidChannelNum;     /* [out] channel has data number */
    HI_U32 u32TimeOutMs;         /* [in] timeout time in MS*/
}DMX_SelectDataFlag_S;

typedef struct hiDMX_Compat_SelectDataFlag_S
{
    HI_U32 WatchChannel;         /* [in] channel handles to check*/
    HI_U32 WatchChannelNum;      /* [in] channel number to check*/
    HI_U32 ValidChannel;         /* [out] channel has data ready */
    HI_U32 ValidChannelNum;      /* [out] channel has data number */
    HI_U32 u32TimeOutMs;         /* [in] timeout time in MS*/
}DMX_Compat_SelectDataFlag_S;

typedef struct hiDMX_AcqMsg_S
{
    HI_HANDLE hChannel;
    HI_U32 u32AcquireNum;
    HI_U32 u32AcquiredNum;
    HI_UNF_DMX_DATA_S *pstBuf;
    HI_U32 u32TimeOutMs;
}DMX_AcqMsg_S;

typedef struct hiDMX_Compat_AcqMsg_S
{
    HI_HANDLE hChannel;
    HI_U32 u32AcquireNum;
    HI_U32 u32AcquiredNum;
    HI_U32 pstBuf;
    HI_U32 u32TimeOutMs;
}DMX_Compat_AcqMsg_S;

typedef struct hiDMX_RelMsg_S
{
    HI_HANDLE hChannel;
    HI_U32 u32ReleaseNum;
    HI_UNF_DMX_DATA_S *pstBuf;
}DMX_RelMsg_S;

typedef struct hiDMX_Compat_RelMsg_S
{
    HI_HANDLE hChannel;
    HI_U32 u32ReleaseNum;
    HI_U32 pstBuf;
}DMX_Compat_RelMsg_S;

typedef struct hiDMX_NewPcr_S
{
    HI_U32 u32DmxId;
    HI_U32 u32PcrId;
}DMX_NewPcr_S;

typedef struct hiDMX_PcrPidSet_S
{
    HI_U32 pu32PcrChId;
    HI_U32 u32Pid;
}DMX_PcrPidSet_S;

typedef DMX_PcrPidSet_S  DMX_PcrPidGet_S;

typedef struct hiDMX_PcrScrGet_S
{
    HI_U32 pu32PcrChId;
    HI_U32 reserve;
    HI_U64 u64PcrValue;
    HI_U64 u64ScrValue;
}DMX_PcrScrGet_S;

typedef struct hiDMX_PcrValGet_S
{
    HI_U32 pu32PcrChId;
    HI_U32 u32PcrMs;
}DMX_PcrValGet_S;

typedef struct
{
    HI_U32 u32PcrChId;
    HI_U32 u32SyncHandle;
} DMX_PCRSYNC_S;

typedef struct hiDMX_PesBufAttach_S
{
    HI_HANDLE hChannel;
    HI_MMZ_BUF_S stPesBuf;
}DMX_PesBufAttach_S;

typedef struct hiDMX_PesBufStaGet_S
{
    HI_HANDLE hChannel;
    HI_MPI_DMX_BUF_STATUS_S stBufStat;
}DMX_PesBufStaGet_S;

typedef struct hiDMX_PesBufGet_S
{
    HI_HANDLE hChannel;
    HI_U32 BufPhyAddr;
    HI_U32 BufSize;
    HI_U32 PtsMs;
}DMX_PesBufGet_S;

typedef struct
{
    HI_HANDLE               RecHandle;
    HI_UNF_DMX_REC_ATTR_S   RecAttr;
    HI_U32                  RecBufPhyAddr;
    HI_U32                  RecBufSize;
    DMX_BUF_FLAG_E                  RecBufFlag;
} DMX_Rec_CreateChan_S;

typedef struct
{
    HI_HANDLE   RecHandle;
    HI_HANDLE   ChanHandle;
    HI_U32      Pid;
} DMX_Rec_AddPid_S;

typedef struct
{
    HI_HANDLE   RecHandle;
    HI_HANDLE   ChanHandle;
} DMX_Rec_DelPid_S;

typedef struct
{
    HI_HANDLE   RecHandle;
    HI_U32      Pid;
} DMX_Rec_ExcludePid_S;

typedef struct
{
    HI_HANDLE               RecHandle;
    HI_U32                  BufPhyAddr;
    HI_U32                  BufSize;
    HI_U32                  TimeoutMs;
} DMX_Rec_AcquireData_S;

typedef struct
{
    HI_HANDLE               RecHandle;
    HI_U32                  BufPhyAddr;
    HI_U32                  BufSize;
} DMX_Rec_ReleaseData_S;

typedef struct
{
    HI_HANDLE               RecHandle;
    HI_MPI_DMX_REC_INDEX_S  IndexData;
    HI_U32                  TimeoutMs;
} DMX_Rec_AcquireIndex_S;

typedef struct
{
    HI_HANDLE               RecHandle;
    HI_MPI_DMX_REC_DATA_INDEX_S  *RecDataIdx;
} DMX_Rec_ProcessDataIndex_S;

/* compat define for HI_MPI_DMX_REC_DATA_INDEX_S. */
typedef struct 
{
    HI_U32 u32IdxNum;                                   
    HI_U32 u32RecDataCnt;                               
    HI_MPI_DMX_REC_INDEX_S stIndex[DMX_MAX_IDX_ACQUIRED_EACH_TIME];
    struct {
        HI_U32 Reserved;
        HI_U32 u32DataPhyAddr;
        HI_U32 u32Len;
    }stRecData[2];
} DMX_Compat_REC_DATA_INDEX_S;

typedef struct
{
    HI_HANDLE               RecHandle;
    HI_U32                  RecDataIdx;
} DMX_Compat_Rec_ProcessDataIndex_S;

typedef struct
{
    HI_HANDLE                   RecHandle;
    HI_UNF_DMX_RECBUF_STATUS_S  BufStatus;
} DMX_Rec_BufStatus_S;

#ifdef DMX_USE_ECM
typedef struct
{
    HI_HANDLE   hChannel;
    HI_U32      u32SwFlag;
} DMX_ChanSwGet_S;

typedef struct
{
    HI_HANDLE       hChannel;
    DMX_MMZ_BUF_S   stChnBuf;
} DMX_ChanSwBufGet_S;
#endif

typedef struct
{
    HI_HANDLE   hChannel;
    HI_U32      u32ChanTsCnt;
} DMX_ChanChanTsCnt_S;

typedef struct hiDMX_SetChan_ExtAttr_S
{
    HI_UNF_DMX_CHAN_CC_REPEAT_SET_S stChCCRepeatSet;
}DMX_SetChan_CC_REPEAT_S;

typedef struct {
    HI_MPI_RMX_ATTR_S Attr; /* [in] */
    HI_HANDLE Handle; /* [out] */
}RMX_Create_S;

typedef struct {
    HI_HANDLE Handle; /*[in] */
    HI_MPI_RMX_ATTR_S Attr; /* [in/out] */
}RMX_Attr_S;

typedef struct {
    HI_HANDLE RmxHandle; /* [in] */
    HI_MPI_RMX_PUMP_ATTR_S Attr; /* [in] */
    HI_HANDLE PumpHandle; /* [out] */
}RMX_Add_Pump_S;

typedef struct {
    HI_HANDLE   PumpHandle; /* [in] */
    HI_MPI_RMX_PUMP_ATTR_S Attr; /* [out] */
}RMX_Pump_Attr_S;
    

/********************************************************
  DEMUX command code definition
 *********************************************************/
/*DEMUX base code of command code*/
#define DEMUX_CMD_MASK              (0xF0)
#define DEMUX_GLOBAL_CMD            (0x00)
#define DEMUX_PORT_CMD              (0x10)
#define DEMUX_TSBUFFER_CMD          (0x20)
#define DEMUX_CHAN_CMD              (0x30)
#define DEMUX_FILT_CMD              (0x40)
#define DEMUX_KSET_CMD              (0x50)
#define DEMUX_RECV_CMD              (0x60)
#define DEMUX_PCR_CMD               (0x70)
#define DEMUX_AV_CMD                (0x80)
#define DEMUX_REC_CMD               (0x90)
#define DEMUX_RMX_CMD               (0xA0)


/* global */
#define CMD_DEMUX_GET_POOLBUF_ADDR          _IOR (HI_ID_DEMUX, 0x00, DMX_PoolBuf_Attr_S)
#define CMD_DEMUX_GET_CAPABILITY            _IOR (HI_ID_DEMUX, 0x01, HI_UNF_DMX_CAPABILITY_S)
#define CMD_DEMUX_SET_PUSI                  _IOW (HI_ID_DEMUX, 0x02, HI_UNF_DMX_PUSI_SET_S)
#define CMD_DEMUX_SET_TEI                   _IOW (HI_ID_DEMUX, 0x03, HI_UNF_DMX_TEI_SET_S)
#define CMD_DEMUX_TSI_ATTACH_TSO            _IOW (HI_ID_DEMUX, 0x04, HI_UNF_DMX_TSI_ATTACH_TSO_S)
#define CMD_DEMUX_GET_RESUME_COUNT          _IOWR (HI_ID_DEMUX, 0x05, HI_U32)

/* TS PORT */
#define CMD_DEMUX_PORT_GET_ATTR             _IOWR(HI_ID_DEMUX, 0x10, DMX_Port_GetAttr_S)        /* get port attr */
#define CMD_DEMUX_PORT_SET_ATTR             _IOW (HI_ID_DEMUX, 0x11, DMX_Port_SetAttr_S)        /* set port attr */
#define CMD_DEMUX_PORT_ATTACH               _IOW (HI_ID_DEMUX, 0x12, DMX_Port_Attach_S)         /* attach ts port to demux */
#define CMD_DEMUX_PORT_DETACH               _IOW (HI_ID_DEMUX, 0x13, HI_U32)                    /* detach ts port from demux */
#define CMD_DEMUX_PORT_GETID                _IOWR(HI_ID_DEMUX, 0x14, DMX_Port_GetId_S)          /* get ts port id of demux */
#define CMD_DEMUX_PORT_GETPACKETNUM         _IOWR(HI_ID_DEMUX, 0x15, DMX_PortPacketNum_S)       /* get ts pack counter */
#define CMD_DEMUX_TSO_PORT_GET_ATTR         _IOWR(HI_ID_DEMUX, 0x16, DMX_TSO_Port_Attr_S)       /* get TSO port attr */
#define CMD_DEMUX_TSO_PORT_SET_ATTR         _IOW(HI_ID_DEMUX, 0x17, DMX_TSO_Port_Attr_S)        /* Set TSO port attr */
#define CMD_DEMUX_DMX_GET_TAG_ATTR          _IOWR(HI_ID_DEMUX, 0x18, DMX_Tag_GetAttr_S)         /* get port tag attrs */
#define CMD_DEMUX_DMX_SET_TAG_ATTR          _IOW(HI_ID_DEMUX, 0x19, DMX_Tag_SetAttr_S)          /* set port tag attrs */

/* Ts Buffer */
#define CMD_DEMUX_TS_BUFFER_INIT            _IOWR(HI_ID_DEMUX, 0x20, DMX_TsBufInit_S)           /* TS Buffer init */
#define CMD_DEMUX_TS_BUFFER_DEINIT          _IOW (HI_ID_DEMUX, 0x21, HI_U32)                    /* TS Buffer deinit */
#define CMD_DEMUX_TS_BUFFER_GET             _IOWR(HI_ID_DEMUX, 0x22, DMX_TsBufGet_S)            /* Get TS Buffer */
#define CMD_DEMUX_TS_BUFFER_PUT             _IOW (HI_ID_DEMUX, 0x23, DMX_TsBufPut_S)            /* Put TS Buffer */
#define CMD_DEMUX_TS_BUFFER_RESET           _IOW (HI_ID_DEMUX, 0x24, HI_U32)                    /* Reset TS Buffer */
#define CMD_DEMUX_TS_BUFFER_GET_STATUS      _IOWR(HI_ID_DEMUX, 0x25, DMX_TsBufStaGet_S)         /* Get TS Buffer status */
#define CMD_DEMUX_TS_BUFFER_PUSH            _IOWR(HI_ID_DEMUX, 0x26, DMX_TsBufPush_S)           /* Push TS Buffer status*/
#define CMD_DEMUX_TS_BUFFER_RELEASE         _IOWR(HI_ID_DEMUX, 0x27, DMX_TsBufRel_S)            /* Release TS Buffer status*/

/* Channel */
#define CMD_DEMUX_CHAN_NEW                  _IOWR(HI_ID_DEMUX, 0x30, DMX_ChanNew_S)             /* apply for a free channel */
#define CMD_DEMUX_CHAN_NEW2                 _IOWR(HI_ID_DEMUX, 0x31, DMX_ChanNew_S)             /* apply for a free channel */
#define CMD_DEMUX_CHAN_DEL                  _IOW (HI_ID_DEMUX, 0x32, HI_HANDLE)                 /* delete an allocated channel */
#define CMD_DEMUX_CHAN_OPEN                 _IOW (HI_ID_DEMUX, 0x33, HI_HANDLE)                 /* open channel */
#define CMD_DEMUX_CHAN_CLOSE                _IOW (HI_ID_DEMUX, 0x34, HI_HANDLE)                 /* close channel */
#define CMD_DEMUX_CHAN_ATTR_GET             _IOWR(HI_ID_DEMUX, 0x35, DMX_GetChan_Attr_S)
#define CMD_DEMUX_CHAN_ATTR_SET             _IOW (HI_ID_DEMUX, 0x36, DMX_SetChan_Attr_S)
#define CMD_DEMUX_GET_CHAN_STATUS           _IOWR(HI_ID_DEMUX, 0x37, DMX_ChanStatusGet_S)       /* get channel open/close status */
#define CMD_DEMUX_PID_SET                   _IOW (HI_ID_DEMUX, 0x38, DMX_ChanPIDSet_S)          /* set pid of channel */
#define CMD_DEMUX_PID_GET                   _IOWR(HI_ID_DEMUX, 0x39, DMX_ChanPIDGet_S)          /* get pid of channel */
#define CMD_DEMUX_CHANID_GET                _IOWR(HI_ID_DEMUX, 0x3A, DMX_ChannelIdGet_S)        /* get channel id with the designated pid */
#define CMD_DEMUX_FREECHAN_GET              _IOWR(HI_ID_DEMUX, 0x3B, DMX_FreeChanGet_S)         /* get free channel counter */
#define CMD_DEMUX_SCRAMBLEFLAG_GET          _IOWR(HI_ID_DEMUX, 0x3C, DMX_ScrambledFlagGet_S)    /* get scrambed flag of audio channel */
#define CMD_DEMUX_CHAN_SET_EOS_FLAG         _IOWR(HI_ID_DEMUX, 0x3D, HI_HANDLE)
#define CMD_DEMUX_CHAN_CC_REPEAT_SET        _IOW (HI_ID_DEMUX, 0x3E, DMX_SetChan_CC_REPEAT_S)   /* set channel CC repeat attr*/


#ifdef DMX_USE_ECM
#define CMD_DEMUX_GET_CHAN_SWFLAG           _IOWR(HI_ID_DEMUX, 0x3D, DMX_ChanSwGet_S)
#define CMD_DEMUX_GET_CHAN_SWBUF_ADDR       _IOWR(HI_ID_DEMUX, 0x3E, DMX_ChanSwBufGet_S)        /* get sw buffer addr */
#endif

#define CMD_DEMUX_GET_CHAN_TSCNT            _IOWR(HI_ID_DEMUX, 0x3F, DMX_ChanChanTsCnt_S)       /* get channel ts count */

/* Filter */
#define CMD_DEMUX_FLT_NEW                   _IOWR(HI_ID_DEMUX, 0x40, DMX_NewFilter_S)           /* apply for a free filter */
#define CMD_DEMUX_FLT_DEL                   _IOW (HI_ID_DEMUX, 0x41, HI_HANDLE)                 /* delete an allocated filter */
#define CMD_DEMUX_FLT_SET                   _IOW (HI_ID_DEMUX, 0x42, DMX_FilterSet_S)           /* set fiter parameter */
#define CMD_DEMUX_FLT_GET                   _IOWR(HI_ID_DEMUX, 0x43, DMX_FilterGet_S)           /* get fiter parameter */
#define CMD_DEMUX_FLT_ATTACH                _IOW (HI_ID_DEMUX, 0x44, DMX_FilterAttach_S)        /* attach a filter to a channel */
#define CMD_DEMUX_FLT_DETACH                _IOW (HI_ID_DEMUX, 0x45, DMX_FilterDetach_S)        /* detach a filter from a channel */
#define CMD_DEMUX_FREEFLT_GET               _IOWR(HI_ID_DEMUX, 0x46, DMX_FreeFilterGet_S)       /* get free filter coute */
#define CMD_DEMUX_FLT_DELALL                _IOW (HI_ID_DEMUX, 0x47, HI_HANDLE)                 /* delete all filters on a channel */
#define CMD_DEMUX_FLT_CHANID_GET            _IOWR(HI_ID_DEMUX, 0x48, DMX_FilterChannelIDGet_S)

/* data receive */
#define CMD_DEMUX_GET_DATA_FLAG             _IOWR(HI_ID_DEMUX, 0x60, DMX_GetDataFlag_S)         /* get data flag of dma buffer */
#define CMD_DEMUX_COMPAT_GET_DATA_FLAG      _IOWR(HI_ID_DEMUX, 0x60, DMX_Compat_GetDataFlag_S)  /* get data flag of dma buffer */
#define CMD_DEMUX_ACQUIRE_MSG               _IOWR(HI_ID_DEMUX, 0x61, DMX_AcqMsg_S)
#define CMD_DEMUX_COMPAT_ACQUIRE_MSG        _IOWR(HI_ID_DEMUX, 0x61, DMX_Compat_AcqMsg_S)
#define CMD_DEMUX_RELEASE_MSG               _IOW (HI_ID_DEMUX, 0x62, DMX_RelMsg_S)
#define CMD_DEMUX_COMPAT_RELEASE_MSG        _IOW (HI_ID_DEMUX, 0x62, DMX_Compat_RelMsg_S)
#define CMD_DEMUX_SELECT_DATA_FLAG          _IOWR(HI_ID_DEMUX, 0x63, DMX_SelectDataFlag_S)
#define CMD_DEMUX_COMPAT_SELECT_DATA_FLAG   _IOWR(HI_ID_DEMUX, 0x63, DMX_Compat_SelectDataFlag_S)

/* PCR */
#define CMD_DEMUX_PCR_NEW                   _IOWR(HI_ID_DEMUX, 0x70, DMX_NewPcr_S)              /* set pcr pid */
#define CMD_DEMUX_PCR_DEL                   _IOW (HI_ID_DEMUX, 0x71, HI_U32)                    /* set pcr pid */
#define CMD_DEMUX_PCRPID_SET                _IOW (HI_ID_DEMUX, 0x72, DMX_PcrPidSet_S)           /* set pcr pid */
#define CMD_DEMUX_PCRPID_GET                _IOWR(HI_ID_DEMUX, 0x73, DMX_PcrPidGet_S)           /* get pcr pid */
#define CMD_DEMUX_CURPCR_GET                _IOWR(HI_ID_DEMUX, 0x74, DMX_PcrScrGet_S)           /* get pcr count */
#define CMD_DEMUX_PCRSYN_ATTACH             _IOWR(HI_ID_DEMUX, 0x75, DMX_PCRSYNC_S)             /* attach pcr channel and sync handle */
#define CMD_DEMUX_PCRSYN_DETACH             _IOWR(HI_ID_DEMUX, 0x76, DMX_PCRSYNC_S)             /* detach pcr channel and sync handle */

/* AV */
#define CMD_DEMUX_PES_BUFFER_GETSTAT        _IOWR(HI_ID_DEMUX, 0x80, DMX_PesBufStaGet_S)        /* Get PES Buffer status */
#define CMD_DEMUX_ES_BUFFER_GET             _IOWR(HI_ID_DEMUX, 0x81, DMX_PesBufGet_S)           /* Get ES Buffer */
#define CMD_DEMUX_ES_BUFFER_PUT             _IOW (HI_ID_DEMUX, 0x82, DMX_PesBufGet_S)           /* Put ES Buffer */

/* REC */
#define CMD_DEMUX_REC_CHAN_CREATE           _IOWR(HI_ID_DEMUX, 0x90, DMX_Rec_CreateChan_S)
#define CMD_DEMUX_REC_CHAN_DESTROY          _IOW (HI_ID_DEMUX, 0x91, HI_HANDLE)
#define CMD_DEMUX_REC_CHAN_ADD_PID          _IOWR(HI_ID_DEMUX, 0x92, DMX_Rec_AddPid_S)
#define CMD_DEMUX_REC_CHAN_DEL_PID          _IOW (HI_ID_DEMUX, 0x93, DMX_Rec_DelPid_S)
#define CMD_DEMUX_REC_CHAN_DEL_ALL_PID      _IOW (HI_ID_DEMUX, 0x94, HI_HANDLE)
#define CMD_DEMUX_REC_CHAN_ADD_EXCLUDE_PID  _IOW (HI_ID_DEMUX, 0x95, DMX_Rec_ExcludePid_S)
#define CMD_DEMUX_REC_CHAN_DEL_EXCLUDE_PID  _IOW (HI_ID_DEMUX, 0x96, DMX_Rec_ExcludePid_S)
#define CMD_DEMUX_REC_CHAN_CANCEL_EXCLUDE   _IOW (HI_ID_DEMUX, 0x97, HI_HANDLE)
#define CMD_DEMUX_REC_CHAN_START            _IOW (HI_ID_DEMUX, 0x98, HI_HANDLE)
#define CMD_DEMUX_REC_CHAN_STOP             _IOW (HI_ID_DEMUX, 0x99, HI_HANDLE)
#define CMD_DEMUX_REC_CHAN_ACQUIRE_DATA     _IOWR(HI_ID_DEMUX, 0x9A, DMX_Rec_AcquireData_S)
#define CMD_DEMUX_REC_CHAN_RELEASE_DATA     _IOW (HI_ID_DEMUX, 0x9B, DMX_Rec_ReleaseData_S)
#define CMD_DEMUX_REC_CHAN_ACQUIRE_INDEX    _IOWR(HI_ID_DEMUX, 0x9C, DMX_Rec_AcquireIndex_S)
#define CMD_DEMUX_REC_CHAN_GET_BUF_STATUS   _IOWR(HI_ID_DEMUX, 0x9D, DMX_Rec_BufStatus_S)
#define CMD_DEMUX_REC_CHAN_ACQUIRE_DATA_INDEX           _IOWR(HI_ID_DEMUX, 0x9E, DMX_Rec_ProcessDataIndex_S)
#define CMD_DEMUX_COMPAT_REC_CHAN_ACQUIRE_DATA_INDEX    _IOWR(HI_ID_DEMUX, 0x9E, DMX_Compat_Rec_ProcessDataIndex_S)
#define CMD_DEMUX_REC_CHAN_RELEASE_DATA_INDEX           _IOWR(HI_ID_DEMUX, 0x9F, DMX_Rec_ProcessDataIndex_S)
#define CMD_DEMUX_COMPAT_REC_CHAN_RELEASE_DATA_INDEX    _IOWR(HI_ID_DEMUX, 0x9F, DMX_Compat_Rec_ProcessDataIndex_S)

/* RMX */
#define CMD_REMUX_CREATE                    _IOWR(HI_ID_DEMUX, 0xA0, RMX_Create_S)
#define CMD_REMUX_DESTROY                   _IOW(HI_ID_DEMUX, 0xA1, HI_HANDLE) 
#define CMD_REMUX_GET_ATTR                  _IOWR(HI_ID_DEMUX, 0xA2, RMX_Attr_S)
#define CMD_REMUX_SET_ATTR                  _IOWR(HI_ID_DEMUX, 0xA3, RMX_Attr_S)
#define CMD_REMUX_START                     _IOW(HI_ID_DEMUX, 0xA4, HI_HANDLE)
#define CMD_REMUX_STOP                      _IOW(HI_ID_DEMUX, 0xA5, HI_HANDLE)
#define CMD_REMUX_ADD_PUMP                  _IOWR(HI_ID_DEMUX, 0xA6, RMX_Add_Pump_S)
#define CMD_REMUX_DEL_PUMP                  _IOWR(HI_ID_DEMUX, 0xA7, HI_HANDLE)
#define CMD_REMUX_GET_PUMP_DEFAULT_ATTR     _IOWR(HI_ID_DEMUX, 0xA8, RMX_Pump_Attr_S)
#define CMD_REMUX_GET_PUMP_ATTR             _IOWR(HI_ID_DEMUX, 0xA9, RMX_Pump_Attr_S)
#define CMD_REMUX_SET_PUMP_ATTR             _IOWR(HI_ID_DEMUX, 0xAA, RMX_Pump_Attr_S)

#endif  // __DRV_DEMUX_IOCTL_H__

