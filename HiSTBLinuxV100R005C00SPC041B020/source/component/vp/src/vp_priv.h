#ifndef __VP_PRIV_H__
#define __VP_PRIV_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#include "hi_debug.h"
#include "hi_unf_venc.h"

#define HI_FATAL_VP(fmt...) HI_FATAL_PRINT(HI_ID_VP, fmt)
#define HI_ERR_VP(fmt...) HI_ERR_PRINT(HI_ID_VP, fmt)
#define HI_WARN_VP(fmt...) HI_WARN_PRINT(HI_ID_VP, fmt)
#define HI_INFO_VP(fmt...) HI_INFO_PRINT(HI_ID_VP, fmt)
#define HI_DBG_VP(fmt...) HI_DBG_PRINT(HI_ID_VP, fmt)

#ifndef HI_ADVCA_FUNCTION_RELEASE
#define VP_PROC_SUPPORT
#endif
#ifdef VP_PROC_SUPPORT
#define VP_USR_PROC_DIR "vp"
#define VP_USR_PROC_ENTRY_NAME "vp_info"
#endif

#define MAX_USBCAM_BUFFER_NUM 16
#define MAX_VENC_ONE_FRAME_SIZE (500 * 1024)
#define NAL_AND_FU_PACKET_LEN (12 + 4)
#define MAX_VP_CHANNEL_NUM 2
#define NET_SEND_EMPTY_PACKAGE_INTEVAL  1000  /*unit : ms*/

typedef enum hiVP_ENGINE_STATE_E
{
    VP_ENGINE_SRCATTED = 0x1,
    VP_ENGINE_PREVIEW = 0x2,
    VP_ENGINE_SENDED = 0x4,
    VP_ENGINE_RECVED = 0x8,
    VP_ENGINE_BUTT = 0x0
} VP_ENGINE_STATE_E;

typedef struct hiVP_USBCAM_PARAM_S
{
    HI_U32       u32CapFmt;                             /* capability format, such as YUV, MJPEG, etc */
    HI_MMZ_BUF_S stMmzFrm[MAX_USBCAM_BUFFER_NUM];       /* MMZ buffer, malloc->MMZ->VI */
    HI_VOID     *pMmapAddr[MAX_USBCAM_BUFFER_NUM];      /* virtual mapped address*/
    HI_U32       u32MmapLength[MAX_USBCAM_BUFFER_NUM];  /* mapped length */
} VP_USBCAM_PARAM_S;

typedef struct hiVP_RTCP_ATTR_S
{
    HI_BOOL bEnable;               /* enable RTCP session */
    HI_U32  u32SendInterval;       /* RTCP report send interval */
} VP_RTCP_ATTR_S;

typedef struct hiVP_ATTR_S
{
    HI_HANDLE hVP;
    HI_HANDLE hVI;
    HI_HANDLE hVENC;
    HI_HANDLE hAVPLAY;
    HI_HANDLE hLocalAVPLAY;     /* only used in capture MJPEG mode */
    HI_HANDLE hRTP;
    HI_HANDLE hLocalWin;
    HI_HANDLE hRemoteWin;
    HI_HANDLE hVirWin;          /* only used in capture MJPEG mode */

    HI_UNF_VP_SOURCE_ATTR_S stSrcAttr;
    HI_UNF_VENC_CHN_ATTR_S  stVencAttr;
    HI_UNF_VCODEC_TYPE_E    enDecType;
    HI_UNF_VP_NET_ATTR_U    stLocalNetAttr;
    HI_UNF_VP_NET_ATTR_U    stRemoteNetAttr;
    HI_BOOL                 bIsIpv4;
    VP_RTCP_ATTR_S          stRtcpAttr;          /* RTCP session attributes */
    HI_UNF_VP_RTP_ATTR_S    stRtpAttr;         /**<RTP attribute*//**<CNcomment: RTPÊôÐÔ*/

    HI_U32            u32VpState;                /* machine state */
    VP_USBCAM_PARAM_S stUsbcamPara;              /* private usb camera parameters */
    HI_U8             *pu8VencStream;            /*store VENC stream */

    volatile HI_BOOL bUSBCamStop;                /* usb camera thread */
    pthread_t        threadUSBCam;

    volatile HI_BOOL bNetSendStop;               /* net send thread */
    pthread_t        threadNetSend;
    HI_BOOL          bSendEmptyPackage;          /*  net send empty package or not*/

    volatile HI_BOOL bNetRecvStop;               /* net recv thread */
    pthread_t        threadNetRecv;
} VP_ATTR_S;

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif //__UNF_VP_PRIV_H__
