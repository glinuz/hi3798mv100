#ifndef __RTP_H__
#define __RTP_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#include <sys/socket.h>
#include <netinet/in.h>
#include "hi_type.h"

typedef enum tagRTP_CONTROL_MODE_E
{
    RTP_ONLY_SEND = 0,
    RTP_ONLY_RECV,
    RTP_SEND_AND_RECV
} RTP_CONTROL_MODE_E;

typedef union tagRTP_NET_ATTR_S
{
    struct sockaddr_in stIpv4Attr;              /* IPV4 attribute */
    struct sockaddr_in6 stIpv6Attr;             /* IPV6 attribute */
} RTP_NET_ATTR_S;

typedef struct tagRTP_ATTR_S
{
    RTP_CONTROL_MODE_E enCtrlMod;              /* send and receive mode */

    HI_BOOL        bIsIpv4;                    /* whether use IPV4 attribute */
    RTP_NET_ATTR_S stLocalNet;                 /* Local network attribute */
    RTP_NET_ATTR_S stRemoteNet;                /* Remote network attribute */

    HI_BOOL bEnableRtcp;                       /* whether to enable RTCP */

    HI_U32 u32SplitSize;                       /* split size, should be less than MTU size */
    HI_U32 u32PayLoadType;                     /* pay load type */
    HI_U32 u32SortDepth;                       /* sort in order when receive RTP packets, this para indicates sort depth */
    HI_U32 u32RecvBufSize;                     /* buffer size, usually 1500 bytes */
    HI_U32 u32RecvBufNum;                      /* buffer number (total_size = u32BufSize * u32BufNum) */
} RTP_ATTR_S;

typedef struct tagRTP_NET_STATIC_S
{
    HI_U32 u32Delay;
    HI_U32 u32Jitter;
    HI_U32 u32LostPackets;
    HI_U32 u32LossPercent;
} RTP_NET_STATIC_S;

typedef struct tagRTP_RTCP_REPORT_S
{
    HI_U32           u32SendBytes;
    HI_U32           u32SendPackets;
    HI_U32           u32RecvPacketNum;
    HI_U32           u32RecvBytes;
    RTP_NET_STATIC_S stSendInfo;
    RTP_NET_STATIC_S stRecvInfo;
} RTP_RTCP_REPORT_S;

HI_S32 RTP_Init(HI_VOID);
HI_S32 RTP_DeInit(HI_VOID);

HI_S32 RTP_GetDefaultAttr(RTP_ATTR_S *pstRtpAttr);
HI_S32 RTP_Create(const RTP_ATTR_S *pstRtpAttr, HI_HANDLE *phRTP);
HI_S32 RTP_Destroy(HI_HANDLE hRTP);

HI_S32 RTP_Send(HI_HANDLE hRTP, HI_U8 *pu8BufAddr, HI_U32 u32Len, HI_U32 u32PtsMs);
HI_S32 RTP_AcquireES(HI_HANDLE hRTP, size_t *u32BufAddr, HI_U32 *u32Len, HI_U32 *u32PtsMs);
HI_S32 RTP_ReleaseES(HI_HANDLE hRTP);

HI_S32 RTP_GetAttr(HI_HANDLE hRTP, RTP_ATTR_S *pstRtpAttr);
HI_S32 RTP_SetAttr(HI_HANDLE hRTP, const RTP_ATTR_S *pstRtpAttr);

HI_S32 RTP_GetRtcpReport(HI_HANDLE hRTP, RTP_RTCP_REPORT_S *pstReport);
HI_S32 RTP_SetRtcpAttr(HI_HANDLE hRTP, HI_BOOL bEnable, HI_U32 u32SendTime);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif //__RTP_H__
