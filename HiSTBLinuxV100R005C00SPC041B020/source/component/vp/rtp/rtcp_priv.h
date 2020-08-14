#ifndef __RTCP_PRIV_H__
#define __RTCP_PRIV_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C"
{
 #endif
#endif

#include "hi_type.h"

#define  RTP_TS_DEFAULT 30              /* default time stamp increase value */
#define  RTP_PT_DEFAULT 120             /* default payload */
#define  RTP_VERSION 2
#define  RTCP_VERSION 2
#define  SESSION_OPEN 1
#define  SESSION_CLOSE 0

#define  RTCP_UPDATEDATA_TIME 1000
#define  SOURCE_LEAVED_DELAY 2000

#define  MAX_SEQUENCE 0xFFFF
#define  MAX_TIMESTAMP 0xFFFFFFFF

#define  MIN_SEQUENTIAL 2               /* min squence packets check */
#define  MAX_DROPOUT 3000               /* max number of packets dropped out */
#define  RTP_SEQ_MOD 65536
#define  MAX_DISORDER 100               /* max number of disordered */
#define  DISORDER_THRESHOLD 10000       /* the threshold of whether SN disorders or reverts */

#define  RTCP_MAX_INTERVAL 5000
#define  RTCP_MIN_INTERVAL 2500

#define  SECS_1900_TO_1970 2208988800u

#define DEFAULT_RTCP_SEND_INTERVAL 5000

#define RTCP_SR 200
#define RTCP_RR 201
#define RTCP_SDES 202
#define RTCP_BYE 203
#define RTCP_APP 204

#define PACK_RTCP_TYPE_SR 1
#define PACK_RTCP_TYPE_RR 2
#define PACK_RTCP_TYPE_SDES 3
#define PACK_RTCP_TYPE_BYE 4
#define PACK_RTCP_TYPE_APP 5

#define SOURCE_COUNT 11                     /* count of supported source */
#define SOURCE_INDEX(ulSSRC) (ulSSRC % SOURCE_COUNT)

#define RTCP_PACKET_DATA_MAX_LEN 2000
#define SDES_ITEM_LENGTH 256
#define RTCP_MAX_APP_DATA_SIZE 1000         /* max size of app package data */
#define RTCP_MAX_APP_SUB_TYPE 31
#define MAX_AUDIO_DEV_NUM 10                /* max number of audio device */
#define RTCP_MAX_CSRC_NUM 15

typedef void* VP_tRTCPSessionHandle;

typedef enum _VP_RTP_RESULT
{
    RTP_ERR_OK = 0,
    RTP_ERR_ERROR = 1,
    RTP_ERR_INVALIDPARAM = 2,
    RTP_ERR_MEMORY = 3,
    RTP_ERR_PACKETILLEGAL   = 4,
    RTP_ERR_NOT_IMPLEMENTED = 5
} VP_RTP_RESULT;

typedef enum _VP_SDES_ITEM_TYPE
{
    RTCP_SDES_ID_CNAME = 1,
    RTCP_SDES_ID_NAME  = 2,
    RTCP_SDES_ID_EMAIL = 3,
    RTCP_SDES_ID_PHONE = 4,
    RTCP_SDES_ID_LOCATION = 5,
    RTCP_SDES_ID_TOOL = 6,
    RTCP_SDES_ID_NOTE = 7,
    RTCP_SDES_ID_PRIVATE = 8,
} VP_SDES_ITEM_TYPE;

typedef struct _VP_RTCP_HEADER
{
    HI_U8  ucVersion;
    HI_U8  ucPadding;
    HI_U8  ucCount;                     /* count of received data */
    HI_U8  ucPacketType;                /* package type, 8 bits, SR: 200 */
    HI_U16 usLength;                    /* RTCP length - 1, 16 bits, including header and padding data */
} VP_RTCP_HEADER;

typedef struct _VP_RTCP_SDESINFO
{
    HI_CHAR szCNAME[SDES_ITEM_LENGTH];
    HI_CHAR szNAME[SDES_ITEM_LENGTH];
    HI_CHAR szEMAIL[SDES_ITEM_LENGTH];
    HI_CHAR szPHONE[SDES_ITEM_LENGTH];
    HI_CHAR szLOC[SDES_ITEM_LENGTH];
    HI_CHAR szTOOL[SDES_ITEM_LENGTH];
    HI_CHAR szNOTE[SDES_ITEM_LENGTH];
    HI_CHAR szPRIV[SDES_ITEM_LENGTH];
} VP_RTCP_SDESINFO;

typedef struct _VP_SDES_ITEM
{
    HI_U8 ucSDESItemType;
    HI_U8 length;
    HI_S8 str[SDES_ITEM_LENGTH];
} VP_SDES_ITEM;

typedef struct _VP_RTCPSENDINFO
{
    HI_U32 ulNTPMSW;
    HI_U32 ulNTPLSW;
    HI_U32 ulTimestamp;
    HI_U32 ulPackets;
    HI_U32 ulBytes;
} VP_RTCPSENDINFO;

typedef struct _VP_RTCP_APP_INFO
{
    VP_RTCP_HEADER     stRtcpHeader;
    HI_U32             ulSSRC;
    HI_S8              pcName[4];
    HI_S8             *pcData;
    HI_S32             iLength;
} VP_RTCP_APP_INFO;

typedef struct _VP_RTCPAPPDATA_STRU
{
    HI_U8       ucType;                 /* only valid in 5 lower bits */
    HI_S8       cName[4];
    HI_U32      uiDataSize;             /* must be 4 aligned */
    HI_U8      *pucData;
} VP_RTCPAPPDATA_STRU;

typedef void (*RTCP_APPDataCallBackProc)(HI_S32 iChannel, const VP_RTCPAPPDATA_STRU *pstAppdata, void *pContext);

typedef struct _VP_RTCP_SDES_CHUNK
{
    HI_U32       ulSSRC;                /* source tag */
    VP_SDES_ITEM stItem[9];             /* source info */
} VP_RTCP_SDES_CHUNK;

typedef struct _VP_RTCPREPORTBLOCK
{
    HI_U32 ulSSRC;
    HI_U32 ulFractLost : 8;
    HI_U32 ulTotalLost : 24;
    HI_U32 ulSequence;
    HI_U32 ulJitter;
    HI_U32 ulLSR;
    HI_U32 ulDLSR;
} VP_RTCPREPORTBLOCK;

typedef struct _VP_RTCPREPORTNODE VP_RTCPREPORTNODE;

struct _VP_RTCPREPORTNODE
{
    VP_RTCPREPORTNODE*    pstNext;
    VP_RTCPREPORTNODE*    pstPrev;
    VP_RTCPREPORTBLOCK    stReportBlock;             /* data of report block */
    HI_U32                ulTime;                    /* received time of the report block */
};

typedef struct _VP_RTPHEADEREX
{
    HI_U16 usHeadExtIdentify;           /* identify of header extension */
    HI_U16 usHeadExtLength;             /* extension length of header */
    HI_U8* pHeadExtData;
} VP_RTPHEADEREX;

typedef struct _VP_RTPRECVINFO
{
    HI_U32 ulSequence;
    HI_U32 ulTimestamp;
    HI_U32 ulPackets;
    HI_U32 ulBytes;
} VP_RTPRECVINFO;

typedef struct _VP_RTP_INFO
{
    HI_U8  data[RTCP_PACKET_DATA_MAX_LEN];
    HI_U32 ulPacketLength;              /* packet length, count as byte */
    HI_U8* pPaload;                     /* pointer of RTP payload data */
    HI_U32 ulPaloadLength;              /* payload data length, count as byte */
    HI_U8  ucVersion;
    HI_U8  ucPadding;
    HI_U8  ucExtension;
    HI_U8  ucCSRCcount;
    HI_U8  ucMarker;
    HI_U8  ucPayloadType;
    HI_U16 usSequenceNumber;
    HI_U32 ulTimestamp;
    HI_U32 ulSSRC;
    HI_U32 ulCSRC[RTCP_MAX_CSRC_NUM];

    /* extension of header */
    HI_U16 usHeadExtIdentify;
    HI_U16 usHeadExtLength;
    HI_U8* pHeadExtData;

    /* application */
    HI_U32 ulInterval;                  /* current timestamp increasement */
    HI_U32 ulJitter;
    HI_U32 ulTime;                      /* send and receive time of RTP packet, count as ms, read only */
    HI_U32 bIncrease;

    /* tag of SN increasement */
    HI_U32 bSnIncrease;
    HI_U32 iRepeat;                     /* count of repeat sending */

    /* debug data */
    HI_S32 lNetDelay;                   /* network delay */
    HI_S32 lEstTotalDelay;              /* target total delay */
    HI_S32 lRealTotalDelay;             /* real total delay */
} VP_RTP_INFO;

typedef struct _VP_REC_REPORT_INFO
{
    HI_U32 SSRC;
    HI_U32 ucLostFraction;
    HI_U32 ulPacketLostCount;
    HI_U32 ulExtendedHighestSn;         /* the highest squence number of extension received */
    HI_U32 ulJitter;                    /* use 16 packets as smooth */
    HI_U32 ulLastSR;                    /* last SR packet, use middle 32 bits of 64 bits NTP from last RTCP SR of SSRC_n */
                                        /* this field will be 0 value when not received SR packet */
    HI_U32 ulDelaySinceLastSR;          /* delay time since last SR packet from SSRC_n, count as 1/65536 second */
                                        /* this field will be 0 value when not received SR packet */
} VP_REC_REPORT_INFO;

typedef struct _VP_RTCP_SR_INFO
{
    VP_RTCP_HEADER     stRtcpHeader;
    HI_U32             ulSSRC;
    HI_U32             ulNTPMSW;
    HI_U32             ulNTPLSW;
    HI_U32             ulRtpTimestamp;
    HI_U32             ulPacketSendCount;
    HI_U32             ulByteSendCount;
    VP_REC_REPORT_INFO stRecReportInfo[31];
} VP_RTCP_SR_INFO;

typedef struct _VP_RTCP_RR_INFO
{
    VP_RTCP_HEADER     stRtcpHeader;
    HI_U32             ulSSRC;
    VP_REC_REPORT_INFO stRecReportInfo[32];
} VP_RTCP_RR_INFO;

typedef  struct _VP_RTCP_SDES_INFO
{
    VP_RTCP_HEADER stRtcpHeader;
    HI_U32         ulSSRC;
    VP_SDES_ITEM   stSdesItem;
} VP_RTCP_SDES_INFO;

typedef struct _VP_RTCP_BYE_INFO
{
    VP_RTCP_HEADER    stRtcpHeader;
    HI_U32            ulCSRC[31];
    HI_U8             ucLength;
    HI_S8*            pcReason;
} VP_RTCP_BYE_INFO;

typedef struct _VP_RTCP_INFO
{
    HI_U8             reserve [RTCP_PACKET_DATA_MAX_LEN];
    VP_RTCP_SR_INFO   stRtcpSRInfo;
    VP_RTCP_RR_INFO   stRtcpRRInfo;
    VP_RTCP_SDES_INFO stRtcpSDESInfo;
    VP_RTCP_BYE_INFO  stRtpBYEInfo;
    VP_RTCP_APP_INFO  stRtcpAppInfo;
    HI_S32            ulRtcpType;       /* RTCP packet type */
} VP_RTCP_INFO;

typedef struct _VP_RTCP_STAT_STRU
{
    HI_U32 uiTotalLostPacket;
    HI_U32 uiLossFraction;
    HI_U32 uiJitter;
    HI_U32 uiDelay;
} VP_RTCP_STAT_STRU;

typedef struct _VP_RTCP_STATINFOR_STRU
{
    HI_U32            uiSendPacketNum;
    HI_U32            uiSendBytes;
    VP_RTCP_STAT_STRU stSendInfo;
    VP_RTCP_STAT_STRU stRecvInfo;
} VP_RTCP_STATINFOR_STRU;

typedef struct _VP_SOURCE VP_SOURCE;
struct _VP_SOURCE
{
    VP_SOURCE*            pstNext;
    VP_SOURCE*            pstPrev;
    VP_RTCP_SDES_CHUNK    stSdesChunk;
    VP_RTCP_SDESINFO      stRtcpSdesInfo;
    VP_RTCPSENDINFO       stSenderInfo;
    VP_RTCPREPORTNODE     stReportBlocks[SOURCE_COUNT];
    VP_RTPHEADEREX        stRTPHeaderEx;
    HI_BOOL               bNeedSDES;                 /* whether SDES is needed */
    HI_BOOL               bIsSender;                 /* whether the RTP data has been sended */
    HI_BOOL               bIsLeaver;                 /* whether the BYE packet has been sended */
    HI_S8                 iProbation;                /* count of continuous checking RTP packets in validity check */
    HI_U32                ulRTPBandwidth;
    HI_U32                ulRTPOrgSequence;          /* the RTP sequence number when source is confirmed as valid */
    HI_U32                ulRTPOrgTimestamp;         /* the RTP time stamp when source is confirmed as valid */
    HI_U32                ulRTPBadSequence;
    HI_U32                ulRTPPayloadType;
    HI_U32                ulRTPSequence;
    HI_U32                ulRTPTimestamp;
    HI_U32                ulSSRC;
    HI_U32                ulRTPInterval;
    HI_U32                uiRTPJitter;
    HI_U32                ulRTPPackets;              /* count of sended or received RTP packets */
    HI_U32                ulRTPBytes;                /* bytes of sended or received RTP packets */
    HI_U32                ulRTPLastPackets;          /* count of real received RTP packets when construct last RR */
    HI_U32                ulRTPLastBytes;            /* bytes of real received RTP packets when construct last RR */
    HI_U32                ulRTPExptPackets;          /* count of expected received RTP packets when construct last RR */
    HI_U32                ulRTPExptBytes;            /* bytes of expected received RTP packets when construct last RR */
    HI_U32                ulRTPDataRate;
    HI_BOOL               bRTPFirstSendPacket;
    HI_BOOL               bRTPFirstRecvPacket;
    HI_U32                ulRTPLastPackTime;
    HI_U32                ulRTCPBandwidth;
    HI_U32                ulRTPCycles;
    HI_U32                ulRTCPSDESCount0;
    HI_U32                ulRTCPSDESCount1;
    HI_U32                ulRTCPSDESCount2;
    HI_U32                ulRTCPInterval;            /* paket interval time of RTCP packets */
    HI_U32                ulRTCPAverageSize;         /* average size of RTCP packet */
    HI_U32                ulRTCPPackets;             /* count of sended or received RTCP packets of source */
    HI_U32                ulRTCPBytes;               /* bytes of sended or received RTCP packets of source */
    HI_U32                ulRTCPDataRate;
    HI_U32                ulRTCPLastSRTime;
    HI_U32                ulRTCPLastSendTime;
    HI_U32                ulRTCPNextSendTime;
    HI_U32                ulTriggerFIR;
    HI_U32                ulLastActiveTime;          /* last active time that contributed to source */
    HI_BOOL               bRTPHasSent;               /* whether the source sends RTP packets in RTCP transport interval */
    HI_BOOL               bRTCPHasSent;
    HI_U32                ulRTPLastSendTime;
    HI_U32                ulLastPacketTs;            /* time stamp of last RTP packet recorded */
    HI_U32                ulLastPacketArriveTime;    /* arrive time of last RTP packet recorded */
    HI_U32                ulRecvDelay;                
    HI_U32                ulRecvJitter;                
    HI_U32                ulSendDelay;                
    HI_U32                ulSendNetDelay;                
    HI_U32                ulSendJitter;                
    HI_S32                ulTimeOffest;              /*发送端标志时钟与接收端标准时钟的偏差 SendTime - RecvTime  ms*/
    HI_S32                ulTimeRatio;              /*Timestamp Send : Recv*/
    HI_BOOL               bIsRemoteRecv;                 /* whether the RTP data has been recv*/
    HI_BOOL               bIsLocalRecv;                 /* whether the RTP data has been recv*/
};

typedef struct _VP_RTCP_SESSION
{
    HI_U32                   ulRTPInvalidPackets;
    HI_U32                   ulRTPInvalidBytes;
    HI_U32                   ulRTCPInvalidPackets;
    HI_U32                   ulRTCPInvalidBytes;
    VP_SOURCE*               pstSources[SOURCE_COUNT + 1];
    VP_RTPRECVINFO           stRecvInfo;
    HI_U32                   ulLastMembers;          /* count of last RTCP member */
    HI_U32                   ulMembers;              /* current count of member */
    HI_U32                   ulSenders;              /* count of sender */
    HI_U32                   ulLeavers;              /* count of members that sended BYT packet */
    HI_U32*                  pulCSRC;                /* pointer of CSRC */
    HI_U8                    ulCSRCs;                /* count of CSRC */
    HI_U32                   ulLastNotifyCSRC;
    HI_U32                   ulLastUpdateTime;
    HI_BOOL                  bRtcpEnable;            /* tag of RTCP enabled */
    HI_U32                   uiRtcpSendInterval;     /* default value: 5000ms */

    //    void*                    RTPLockHandle;
    HI_S32                   iChannel;               /* number of channel */
    RTCP_APPDataCallBackProc pAPPDataCallBackProc;
    void *                   pContext;
    HI_U8                    ucSendAppDataBuf[RTCP_MAX_APP_DATA_SIZE];   /* valid application data that user configs to be sent */
    VP_RTCP_APP_INFO         stSendAppInfo;          /* application data that user configs */
    VP_RTCP_STATINFOR_STRU   stRtcpStatisticInfo;
    HI_S32                   iAppSendFlag;           /* 0: no application sent; 1: application sent */
    HI_U32                   uiState;                /* session state */
#define  LOCAL pstSources[SOURCE_COUNT]
} VP_RTCP_SESSION;

typedef struct _VP_RTPPACKET_STRU
{
    HI_U8  data[RTCP_PACKET_DATA_MAX_LEN];
    HI_U32 ulLen;                       /* total length of RTP packets */
} VP_RTPPACKET_STRU;

typedef enum _VP_PACKET_TYPE_ENUM
{
    VP_RTP  = 0,
    VP_RTCP = 1,
    VP_RTP_DATA  = 2,
    VP_RTCP_DATA = 3,
    VP_OTHER = 4,
} VP_PACKET_TYPE_ENUM;

typedef struct _VP_PACKET_STRU
{
    union
    {
        VP_RTPPACKET_STRU stRtpPack;
        VP_RTP_INFO stRtpInfo;
        VP_RTCP_INFO stRtcpInfo;
    } u;
    VP_PACKET_TYPE_ENUM enPackType;
    HI_U32              u32Time;
    void*               pParentPool;
    struct _VP_PACKET_STRU *pNext;
    struct _VP_PACKET_STRU *pPre;
} VP_PACKET_STRU, *PVP_PACKET_STRU;

/**************************************************************************************************
Function        : VP_RTCP_CreateSession
Description     : Create session
Input Parameter : hHandle -- handle
                  iChID -- channel ID
Output Parameter:
Return Type     : VP_RTP_RESULT
Other           :
**************************************************************************************************/
VP_RTP_RESULT VP_RTCP_CreateSession(VP_tRTCPSessionHandle* hHandle, HI_S32 iChID);

/**************************************************************************************************
Function        : VP_RTCP_DeleteSession
Description     : Delete session
Input Parameter : hHandle -- handle
Output Parameter:
Return Type     : VP_RTP_RESULT
Other           :
**************************************************************************************************/
VP_RTP_RESULT VP_RTCP_DeleteSession(VP_tRTCPSessionHandle hHandle);

/**************************************************************************************************
Function        : VP_RTCP_SendCtrl
Description     : Judge whether it is the time to send RTCP packet, pack RTCP packet when necessary
Input Parameter : hHandle -- handle
Output Parameter: pstPacket -- save packet data in this structure
Return Type     : VP_RTP_RESULT
Other           :
**************************************************************************************************/
VP_RTP_RESULT VP_RTCP_SendCtrl(VP_tRTCPSessionHandle hHandle, VP_PACKET_STRU* pstPacket);

/**************************************************************************************************
Function        : VP_RTCP_ParsePacket
Description     : Parse RTCP packet
Input Parameter : hHandle -- handle
                  pstPacket -- save packet data in this structure
                  pstRtcpInfo -- RTCP info
Output Parameter:
Return Type     : VP_RTP_RESULT
Other           :
**************************************************************************************************/
VP_RTP_RESULT VP_RTCP_ParsePacket(VP_tRTCPSessionHandle hHandle, VP_RTPPACKET_STRU* pstPacket,
                                  VP_RTCP_INFO* pstRtcpInfo);

/**************************************************************************************************
Function        : VP_RTCP_EnableRtcp
Description     : Enable RTCP function
Input Parameter : hHandle -- handle
                  bEnable -- 1 enable
                             0 disable
Output Parameter:
Return Type     : VP_RTP_RESULT
Other           :
**************************************************************************************************/
VP_RTP_RESULT VP_RTCP_EnableRtcp(VP_tRTCPSessionHandle hHandle, HI_BOOL bEnable);

/**************************************************************************************************
Function        : VP_RTCP_CfgRtcpSendInterval
Description     : Config RTCP send interval
Input Parameter : hHandle -- handle
                  iInterval -- interval time(ms)
Output Parameter:
Return Type     : VP_RTP_RESULT
Other           :
**************************************************************************************************/
VP_RTP_RESULT VP_RTCP_CfgRtcpSendInterval(VP_tRTCPSessionHandle hHandle, HI_S32 iInterval);

HI_VOID       VP_RTCP_ParseRtpUpdateSource(VP_RTCP_SESSION *pstSession, HI_U32 ulPacketLen, VP_RTP_INFO *pstRtpInfo);

VP_RTP_RESULT VP_RTCP_RecvCalculateDelayAndJitter(VP_RTCP_SESSION *pstSession, HI_U32 u32Ssrc, HI_U32 u32PtsMs);
VP_RTP_RESULT VP_RTCP_SendCalculateDelayAndJitter(VP_RTCP_SESSION *pstSession, HI_U32 u32PtsMs);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif // __RTCP_PRIV_H__
