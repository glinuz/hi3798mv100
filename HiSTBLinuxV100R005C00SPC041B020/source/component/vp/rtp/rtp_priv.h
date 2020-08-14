#ifndef __RTP_PRIV_H__
#define __RTP_PRIV_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#include "linux_list.h"
#include "hi_common.h"
#include "hi_type.h"
#include "hi_debug.h"
#include "rtp.h"
#include "hi_unf_common.h"

#define HI_ID_RTP ((HI_CHAR)(0x12))

#define RTP_MAX_SESSION_NUM (2)

#define SOCKET_RCVTIMEO  1000   /*socket recvfrom timeout , unit : ms*/

#define HI_ERR_RTP_PTR_NULL (HI_S32)(-1)
#define HI_ERR_RTP_NOT_INIT (HI_S32)(-2)
#define HI_ERR_RTP_INVALID_HANDLE (HI_S32)(-3)
#define HI_ERR_RTP_NOT_SUPPORT (HI_S32)(-4)
#define HI_ERR_RTP_CREATE_ERR (HI_S32)(-5)
#define HI_ERR_RTP_NOT_EXIST (HI_S32)(-6)
#define HI_ERR_RTP_MEM_ERR (HI_S32)(-7)
#define HI_ERR_RTP_NET_ERR (HI_S32)(-8)

#define HI_FATAL_RTP(fmt...) HI_FATAL_PRINT(HI_ID_VP, fmt)
#define HI_ERR_RTP(fmt...) HI_ERR_PRINT(HI_ID_VP, fmt)
#define HI_WARN_RTP(fmt...) HI_WARN_PRINT(HI_ID_VP, fmt)
#define HI_INFO_RTP(fmt...) HI_INFO_PRINT(HI_ID_VP, fmt)
#define HI_DBG_RTP(fmt...) HI_DBG_PRINT(HI_ID_VP, fmt)

#define RTP_VERSION 2
#define RTP_NAL_TYPE_FUA 28
#define RTP_NAL_TYPE_FUB 29
#define RTP_FU_LEN 2
#define RTP_SSRC 0x12345678
#define RTP_NAL_HEADER_SIZE 4

#define RTP_MAX_SORT_DEPTH 10
#define RTP_MAX_PAYLOADS 128
#define RTP_MAX_SPLIT_SIZE 1460 //ip mtu 1500, 20 bype ip header, 8 byte udp header, 12 bype RTP header
#define RTP_MAX_UDP_LEN 1500
#define RTP_MAX_REORDER_BUF_SIZE  1500
#define RTP_MAX_REORDER_BUF_NUM 300

#define RTP_DEFAULT_SORT_DEPTH 5
#define RTP_DEFAULT_SPLIT_SIZE 1450 
#define RTP_DEFAULT_REORDER_BUF_NUM 150

#define RTP_PTS_MS_TO_90MS(PTS_MS) (90 * (PTS_MS))
#define RTP_PTS_90MS_TO_MS(PTS_90MS) ((PTS_90MS) / 90)

/* RTP payload type defination */
typedef enum tagRTP_PAYLOAD_TYPE_E
{
    RTP_PT_ULAW = 0,           /* mu-law */
    RTP_PT_GSM  = 3,           /* GSM */
    RTP_PT_G723 = 4,           /* G.723 */
    RTP_PT_ALAW = 8,           /* a-law */
    RTP_PT_G722 = 9,           /* G.722 */
    RTP_PT_S16BE_STEREO = 10,  /* linear 16, 44.1khz, 2 channel */
    RTP_PT_S16BE_MONO = 11,    /* linear 16, 44.1khz, 1 channel */
    RTP_PT_MPEGAUDIO = 14,     /* mpeg audio */
    RTP_PT_JPEG = 26,          /* jpeg */
    RTP_PT_H261 = 31,          /* h.261 */
    RTP_PT_MPEGVIDEO = 32,     /* mpeg video */
    RTP_PT_MPEG2TS = 33,       /* mpeg2 TS stream */
    RTP_PT_H263  = 34,         /* old H263 encapsulation */
    RTP_PT_H264  = 96,         /* hisilicon define as h.264 */
    RTP_PT_G726  = 97,         /* hisilicon define as G.726 */
    RTP_PT_ADPCM = 98,         /* hisilicon define as ADPCM */
    RTP_PT_AAC  = 99,          /* hisilicon define as AAC */
    RTP_PT_DATA = 100,         /* hisilicon define as md alarm data */
    RTP_PT_ARM     = 101,      /* hisilicon define as AMR */
    RTP_PT_MJPEG   = 102,      /* hisilicon define as MJPEG */
    RTP_PT_H265  = 103,        /* hisilicon define as h.265*/
    RTP_PT_INVALID = 127
} RTP_PAYLOAD_TYPE_E;

/* total 12 bytes */
typedef struct tagRTP_HDR_S
{
    /* byte 0 */
    HI_U32 u32CsrcCnt          : 4;   /* CSRC count */
    HI_U32 u32HeaderExt        : 1;   /* header extension flag */
    HI_U32 u32Padding          : 1;   /* padding flag */
    HI_U32 u32Version          : 2;   /* protocol version */

    /* byte 1 */
    HI_U32 u32Payload          : 7;   /* payload type */
    HI_U32 u32Marker           : 1;   /* marker bit */

    /* bytes 2, 3 */
    HI_U32 u32SeqNum           : 16;  /* sequence number */

    /* bytes 4-7 */
    HI_U32 u32PtsMs;                  /* timestamp in ms */

    /* bytes 8-11 */
    HI_U32 u32Ssrc;                   /* synchronization source */
} RTP_HDR_S;

typedef struct list_head HI_LIST;

typedef struct tagRTP_RECV_BUF_NODE_S
{
    HI_LIST          listhead;
    HI_U8           *pBlockAddr;
    HI_U32           u32BlockSize;
    HI_U32           u32SeqNumber;
    HI_U32           u32Pts;
    HI_U32           u32Offset;
} RTP_RECV_BUF_NODE_S;

typedef struct tagRTP_RECV_BUF_S
{
    RTP_RECV_BUF_NODE_S stRtpBufNode[RTP_MAX_REORDER_BUF_NUM];
    HI_LIST             RtpFullList;
    HI_LIST             RtpEmptyList;
    HI_U8               *pu8BufPool;
    HI_U32              u32LastOutSeq;
} RTP_RECV_BUF_S;

#define RTP_HDR_LEN sizeof(RTP_HDR_S)
#define RTP_SET_CSRCCNT(pHDR, val) ((pHDR)->u32CsrcCnt = (val))
#define RTP_SET_HDREXT(pHDR, val) ((pHDR)->u32HeaderExt = (val))
#define RTP_SET_PADDING(pHDR, val) ((pHDR)->u32Padding = (val))
#define RTP_SET_VERSION(pHDR, val) ((pHDR)->u32Version = (val))
#define RTP_SET_PAYLOAD(pHDR, val) ((pHDR)->u32Payload = (val))
#define RTP_SET_MARKER(pHDR, val) ((pHDR)->u32Marker = (val))
#define RTP_SET_SEQNUM(pHDR, val) ((pHDR)->u32SeqNum = (val))
#define RTP_SET_PTS(pHDR, val) ((pHDR)->u32PtsMs = (val))
#define RTP_SET_SSRC(pHDR, val) ((pHDR)->u32Ssrc = (val))

HI_U32   RTCP_EnableNonblock(HI_S32 fd);
HI_S32   RTCP_Create(HI_U32 u32Chn);
HI_VOID  RTCP_Destroy(HI_U32 u32Chn);
HI_VOID* RTP_ScheduleRtcpSend(HI_VOID* args);
HI_VOID* RTP_ScheduleRtcpRecv(HI_VOID* args);
HI_S32   RTP_CheckUserConfig(const RTP_ATTR_S *pstRtpAttr);
HI_S32   RTP_FindValidChannel(const HI_HANDLE *phRTP, HI_U32 *hId);
HI_VOID  RTP_HeaderPacket(HI_U8* pPackAddr, HI_U32 u32TimeStamp, HI_U32 u32Marker, RTP_PAYLOAD_TYPE_E enPayload,
                                 HI_U32 u32Ssrc, HI_U16 u16LastSn);
HI_S32   RTP_RecvBufInit(HI_U32 u32Chn);
HI_VOID  RTP_RecvBufDeInit(HI_U32 u32Chn);
HI_S32   RTP_JudgeSeqNum(HI_U16 u16RefNum, HI_U16 u16JudgeNum);
HI_S32   RTP_RecvBufQueueES(HI_U32 u32Chn, HI_U32 u32SeqNum, RTP_RECV_BUF_NODE_S *pstTmpBufNode);
HI_VOID* RTP_ScheduleRecv(HI_VOID* args);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif //__RTP_PRIV_H__
