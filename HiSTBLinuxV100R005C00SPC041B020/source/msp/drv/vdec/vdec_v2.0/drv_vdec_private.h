#ifndef __HI_VDEC_PRIVATE_H__
#define __HI_VDEC_PRIVATE_H__

#include "hi_type.h"
#include "hi_module.h"
#include "hi_drv_sys.h"
#include "hi_drv_dev.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"
#include "hi_drv_proc.h"
#include "hi_drv_stat.h"
#include "hi_drv_module.h"
#include "drv_vdec_buf_mng.h"
#include "hi_unf_avplay.h"
#include "hi_drv_video.h"
#include "hi_drv_vdec.h"
#include "vfmw_ext.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


/* VDEC VERSION */
#define VDEC_VERSION                (2016102800)
#define VDEC_DBG_MODULE_VDI         (0x0)
#define VDEC_DEBUG                  (0)

#define VDEC_KUD_MAX_NUM 4      /*max user data num of each frame*/
#define VDEC_KUD_MAX_LEN 256UL  /*max user data length*/
#define VDEC_UDC_MAX_NUM 16
#define VDEC_MAX_BUFFER_RECORD 64
#define VDEC_MAX_PMV_RECORD 20

//add by l00225186
#define VDEC_MAX_PROC_ARGS_SIZE 30
#define VDEC_MAX_PORT_NUM 3
#define VDEC_MAX_PORT_FRAME 20
#define HI_KMALLOC_VDEC(size)           HI_KMALLOC(HI_ID_VDEC, size, GFP_KERNEL)
#define HI_KMALLOC_ATOMIC_VDEC(size)    HI_KMALLOC(HI_ID_VDEC, size, GFP_ATOMIC)
#define HI_KFREE_VDEC(addr)             HI_KFREE(HI_ID_VDEC, addr)
#define HI_VMALLOC_VDEC(size)           HI_VMALLOC(HI_ID_VDEC, size)
#define HI_VFREE_VDEC(addr)             HI_VFREE(HI_ID_VDEC, addr)

typedef enum tagVDEC_CHAN_STATE_E
{
    VDEC_CHAN_STATE_STOP = 0,
    VDEC_CHAN_STATE_RUN,
    VDEC_CHAN_STATE_INVALID
} VDEC_CHAN_STATE_E;

typedef enum tagVDEC_FRAME_FORMAT_E
{
    VDEC_I_FRAME = 0,
    VDEC_P_FRAME,
    VDEC_B_FRAME,
    VDEC_FRAME_BUTT
} VDEC_FRAME_FORMAT_E;

typedef enum hiDRV_VDEC_FRAMEBUFFER_TYPE_E
{
    HI_DRV_VDEC_BUF_VPSS_ALLOC_MANAGE = 0,
    HI_DRV_VDEC_BUF_USER_ALLOC_MANAGE,
    HI_DRV_VDEC_BUF_VDEC_ALLOC_MANAGE,
    HI_DRV_VDEC_BUF_USER_ALLOC_VPSS_MANAGE,
    HI_DRV_VDEC_BUF_TYPE_BUTT
} HI_DRV_VDEC_FRAMEBUFFER_TYPE_E;
typedef enum hiDRV_VDEC_FRAMEBUFFER_STATE_E
{
    HI_DRV_VDEC_BUF_STATE_IN_VDEC_EMPTY = 0,
    HI_DRV_VDEC_BUF_STATE_IN_VDEC_FULL,
    HI_DRV_VDEC_BUF_STATE_IN_VPSS,
    HI_DRV_VDEC_BUF_STATE_IN_USER,
    HI_DRV_VDEC_BUF_STATE_BUTT
} HI_DRV_VDEC_FRAMEBUFFER_STATE_E;
typedef struct tagVDEC_CHAN_STATINFO_S
{
    HI_U32 u32TotalVdecOutFrame;    /*the number of total output frames*/
    HI_U32 u32TotalVdecParseIFrame; /*the number of total I frames decoded from stream*/
    HI_U32 u32TotalVdecInByte;      /*total bytes of the input stream*/
    HI_U32 u32TotalVdecHoldByte;    /*bytes of stream in VDEC_Frimware buffer*/
    HI_U32 u32TotalVdecTime;        /*total run time of the vdec channel*/
    HI_U32 u32CalcBpsVdecTime;      /*run time of the vdec channel, can be reset, used by calculate bps*/
    HI_U32 u32AvrgVdecFps;          /*the integer part of average output frame rate*/
    HI_U32 u32AvrgVdecFpsLittle;    /*the decimal part of average output frame rate*/
    HI_U32 u32AvrgVdecInBps;        /*the average stream input bit rate(bps)*/
    HI_U32 u32TotalStreamErrNum;    /*the totoal error number of stream*/

    HI_U32 u32VdecRcvFrameTry;      /*the frame number try to acquire*/
    HI_U32 u32VdecRcvFrameOK;       /*the frame number acquire success*/
    HI_U32 u32VdecRlsFrameTry;      /*the frame number try to release*/
    HI_U32 u32VdecRlsFrameOK;       /*the frame number release success*/
    HI_U32 u32VdecRlsFrameFail;     /*the frame number release fail*/
    HI_U32 u32VdecErrFrame;         /*the error frame number*/
    HI_U32 u32VdecDecErrFrame;         /*the number of Frame which pErrRatio is not 0*/

    HI_U32 u32VdecAcqBufTry;
    HI_U32 u32VdecAcqBufOK;
    HI_U32 u32VdecRlsBufTry;
    HI_U32 u32VdecRlsBufOK;

    HI_U32 u32UserAcqFrameTry;
    HI_U32 u32UserAcqFrameOK;
    HI_U32 u32UserRlsFrameTry;
    HI_U32 u32UserRlsFrameOK;

    HI_U32 u32AvplayRcvFrameTry;
    HI_U32 u32AvplayRcvFrameOK;
    HI_U32 u32AvplayRlsFrameTry;
    HI_U32 u32AvplayRlsFrameOK;
    HI_U32 u32TotalVfmwOutFrame;

    HI_U32 u32FrameType[2];
} VDEC_CHAN_STATINFO_S;

typedef struct
{
    VDEC_BUFFER_S    stMMZBuf;
    VDEC_BUFFER_S    st2dBuf;
    HI_U32          u32ReadTimes;
} VDEC_IFRAME_PARAM_S;

typedef struct
{
    HI_U32                  u32ReadID;
    HI_U32                  u32WriteID;
    HI_U8                   au8Buf[VDEC_UDC_MAX_NUM][MAX_USER_DATA_LEN];
    HI_UNF_VIDEO_USERDATA_S stAttr[VDEC_UDC_MAX_NUM];
} VDEC_USRDATA_PARAM_S;

typedef struct
{
    atomic_t                atmWorking;
    wait_queue_head_t       stWaitQue;
    HI_DRV_VIDEO_FRAME_S*  pstFrame;
} VDEC_BTL_PARAM_S;

typedef struct
{
    HI_DRV_VIDEO_FRAME_S stVpssOutFrame;
    HI_BOOL bBufUsed;
    struct list_head node;
    VDEC_BUFFER_S VDECBuf_frm;
    VDEC_BUFFER_S VDECBuf_meta;
    VDEC_ION_BUF_S VDECBuf_meta_priv;
    HI_DRV_VDEC_FRAMEBUFFER_STATE_E enFrameBufferState;
    HI_U32 u32Stride;                          /**<Stride of external frame buffer *//**<CNcomment:外部帧存的stride*/
} BUFMNG_VPSS_NODE_S;
typedef struct
{
    struct list_head stVpssBufAvailableList;
    struct list_head stVpssBufUnAvailableList;
    struct list_head* pstUnAvailableListPos;
    HI_U32 u32BufNum;
    HI_U32 u32FrmBufSize;
    HI_U32 u32MetaBufSize;
    BUFMNG_VPSS_IRQ_LOCK_S stAvailableListLock;
    BUFMNG_VPSS_IRQ_LOCK_S stUnAvailableListLock;
    VDEC_BUFFER_ATTR_S stBufferAttr;
    HI_DRV_VDEC_FRAMEBUFFER_TYPE_E enFrameBuffer;
    HI_U32 u32AvaiableFrameCnt;
    VDEC_EXTBUFFER_STATE_E enExtBufferState;
} BUFMNG_VPSS_INST_S;

typedef struct tagVDEC_PORT_FRAME_LIST_NODE_S
{
    HI_DRV_VIDEO_FRAME_S stPortOutFrame;
    struct list_head node;
} VDEC_PORT_FRAME_LIST_NODE_S;

typedef struct tagVDEC_PORT_FRAME_LIST_LOCK_S
{
    spinlock_t     irq_lock;
    unsigned long  irq_lockflags;
    int            isInit;
} VDEC_PORT_FRAME_LIST_LOCK_S;

typedef struct tagVDEC_PORT_LIST_S
{
    struct list_head stVdecPortFrameList;
    VDEC_PORT_FRAME_LIST_LOCK_S stPortFrameListLock;
} VDEC_PORT_LIST_S;

typedef struct
{
    HI_HANDLE           hPort;
    HI_BOOL             bMainPort; /*0:SlavePort 1:MainPort*/
    VDEC_PORT_TYPE_E    enPortType;
    HI_BOOL             bEnable;/*0:enable 1:disable*/
    BUFMNG_VPSS_INST_S  stBufVpssInst;
    HI_DRV_VPSS_BUFFER_TYPE_E bufferType;
    VDEC_PORT_LIST_S    stPortList;
    VDEC_PORT_FRAME_LIST_NODE_S astPortTmpList[VDEC_MAX_PORT_FRAME];
    HI_S32 s32PortTmpListPos;
    HI_S32 s32GetFirstVpssFrameFlag;
    HI_S32 s32RecvNewFrame;
    HI_S32 s32PortLastFrameGopNum;
    HI_U32 u32LastFrameIndex;
} VDEC_VPSS_PORT_PARAM_S;


typedef struct tagCHAN_FRAME_BUFFER_S
{
    VDEC_BUFFER_S              stFrameBuffer;
    VDEC_BUFFER_S              stHdrBuffer;
    VDEC_BUFFER_S              stPmvBuffer;
    BUFFER_NODE_STATUS         enFrameBufferNodeStatus;
    BUFFER_NODE_STATUS         enHdrBufferNodeStatus;
    BUFFER_NODE_STATUS         enPmvBufferNodeStatus;
    HI_BOOL                    bAvailable;//表示是不是可以配置给vfmw，空闲的帧存才能配置给vfmw
    HI_BOOL                    bConfigured;
    HI_BOOL                    bWaitRelease;
} CHAN_FRAME_BUFFER_S;

typedef struct tagCHAN_FRAME_BUF_RECORD_LOCK_S
{
    spinlock_t     irq_lock;
    unsigned long  irq_lockflags;
    int            isInit;
} CHAN_FRAME_BUF_RECORD_LOCK_S;

typedef struct tagCHAN_CONTROL_STATE_LOCK_S
{
    spinlock_t    irq_lock;
    unsigned long irq_lockflags;
    int           isInit;
} CHAN_CONTROL_STATE_LOCK_S;

/*记录vfmw上报的需要申请的内存信息大小，包括帧存信息，pmv信息，hdr的metadata信息*/
typedef struct tagVFMW_REPORT_FRAME_BUFFER_INFO_S
{
    UINT32  u32OneFrameTotalSize;/*所有数据的总大小*/
    UINT32  u32ImageSize;/*帧存信息大小*/
    UINT32  u32ImageNum;/*需要申请的帧存个数*/
    UINT32  u32PmvSize;/*pmv信息大小*/
    UINT32  u32PmvNum;/*需要申请的pmv个数*/
    UINT32  u32HdrMetadataSize;/*hdr metadata信息大小*/
    UINT32  u32AllocFrameOnly;/*仅需要申请帧存*/
    UINT32  u32ImageSizeOnly;
    UINT32  u32ImageNumOnly;
} VFMW_REPORT_FRAME_BUFFER_INFO_S;

/*记录已经配置给vfmw的内存节点的个数，包括帧存，pmv内存，hdr metadata内存*/
typedef struct tagVFMW_CONFIGURED_BUFFER_NUM_S
{
    UINT32  u32ImageNum;/*已经配置给vfmw的帧存个数*/
    UINT32  u32PmvNum;/*已经配置给vfmw的pmv个数*/
} VFMW_CONFIGURED_BUFFER_NUM_S;

#ifdef VFMW_VPSS_BYPASS_EN   //specialFrameExist->specialFrameNum
typedef struct tagVDEC_SPECIAL_PROC_S
{
    HI_U32  u32SpecialFrmNum;
    VDEC_BUFFER_S frmBufRec[VFMW_MAX_RESERVE_NUM];
} VDEC_SPECIAL_FRM_PROC_S;
#endif

typedef struct tagVDEC_CHANNEL_S
{
    /* For configurate */
    HI_UNF_VCODEC_ATTR_S    stCurCfg;       /* Configurate */
    HI_UNF_AVPLAY_OPEN_OPT_S stUserCfgCap;

    /* State */
    VDEC_CHAN_STATE_E       enCurState;

    /* Parameters from or to VFMW */
    VDEC_CHAN_CAP_LEVEL_E   enCapToFmw;
    DETAIL_MEM_SIZE         stMemSize;
    VDEC_CHAN_OPTION_S      stOption;
    VDEC_BUFFER_S            stSCDMMZBuf;
    VDEC_BUFFER_S            stVDHMMZBuf;
    STREAM_INTF_S           stStrmIntf ;
    IMAGE_INTF_S            stImageIntf;
    VDEC_USRDAT_S*          pu8UsrDataForWaterMark[4];


    HI_DRV_VIDEO_FRAME_S    stLastFrm;
    HI_DRV_VIDEO_FRAME_S    stLastVpssFrm;
    HI_U32                  u32UserSetAspectWidth;
    HI_U32                  u32UserSetAspectHeight;
    HI_U32                  u32DecodeAspectWidth;
    HI_U32                  u32DecodeAspectHeight;
    HI_U32                  u32LastFrmId;
    HI_U32                  u32LastFrmTryTimes;
    HI_U32                  u32EndFrmFlag;
    HI_BOOL                 bEndOfStrm;
    HI_UNF_ENC_FMT_E        enDisplayNorm;
    HI_UNF_VIDEO_FRAME_PACKING_TYPE_E eFramePackType;
    /* Last display frame info */
    HI_DRV_VIDEO_FRAME_S stLastDispFrameInfo;////////123

    HI_U32                  u32BitRate;
    HI_U32                  u32Profile;
    HI_U32                  u32LastLumaBitdepth;     // proc 10bit信息
    HI_U32                  u32LastChromaBitdepth;   // proc 10bit信息
    HI_U32                  u32Level;

    /* Channel status */
    VDEC_CHAN_STATINFO_S    stStatInfo;

    /* For stream buffer */
    HI_HANDLE               hChan;          /* VFMW handle */
    HI_HANDLE               hVdec;          /*VDEC handle*/
    HI_HANDLE               hDmxVidChn;     /* Dmx handle */
    HI_U32                  u32DmxID;
    HI_U32                  u32DmxBufSize;  /* Dmx buffer size */
    HI_HANDLE               hStrmBuf;       /* Stream buffer handle */
    HI_U32                  u32StrmBufSize; /* Stream buffer size */

    /* For event */
    HI_BOOL                             bNormChange;
    HI_UNF_NORMCHANGE_PARAM_S           stNormChangeParam;
    HI_BOOL                             bFramePackingChange;
    HI_UNF_VIDEO_FRAME_PACKING_TYPE_E   enFramePackingType;
    HI_BOOL                             bNewSeq;
    HI_BOOL                         bNewFrame;      /* use tlastFrm */
    HI_BOOL                         bNewUserData;
    HI_BOOL                         bIFrameErr;
    HI_BOOL                         bErrorFrame;
    HI_U8                           bAvsPlus;

    /*0: 1st and 2nd not get; 1: 1st get 2nd not get; 2: 1st and 2nd both get*/
    HI_U32                          u32ValidPtsFlag;

    HI_BOOL                         bFirstValidPts;
    HI_U32                          u32FirstValidPts;
    HI_BOOL                         bSecondValidPts;
    HI_U32                          u32SecondValidPts;

    /* For I frame decode */
    VDEC_IFRAME_PARAM_S             stIFrame;

    /* For user data */
    VDEC_USRDATA_PARAM_S*           pstUsrData;

    /* For capture BTL */
    VDEC_BTL_PARAM_S                stBTL;

    /* For frame rate */
    HI_UNF_AVPLAY_FRMRATE_PARAM_S   stFrameRateParam;

    /* For trick mode */
    HI_S32                          s32Speed;

    /* For end of stream flag */
    VDEC_BUFFER_S                   stEOSBuffer;
    HI_S32                          s32FindStartCodeCnt;
    HI_S32                          s32Mpeg2Score;
    HI_S32                          s32H264Score;
    HI_S32                          s32LastTwoStartCode;
    HI_S32                          s32LastOneStartCode;
    HI_S32                          s32CurrentStartCode;
    HI_BOOL                         bSetEosFlag;

    /* For resolution change */
    HI_U8                           u8ResolutionChange;

    HI_BOOL                         bIsIFrameDec;
    HI_BOOL                         bUnSupportStream;

    HI_BOOL                         bLowdelay;
    HI_BOOL                         bProcRegister;
    HI_BOOL                         bTvp;
    HI_U32                          u32FrameCnt;
    HI_U32                          u32ErrRatio;

    HI_U32                          u32FrameSize;//l00273086
    HI_U32                          u32RefFrameNum; //l00273086
    VFMW_REPORT_FRAME_BUFFER_INFO_S stVFMWReportFrameInfo;//记录vfmw上报的需要vdec申请的帧存大小和个数，pmv和hdr metadata分开上报。
    VFMW_CONFIGURED_BUFFER_NUM_S    stVFMWConfiguredBufferNum;//记录已经配置给vfmw的内存(帧存,pmv,hdr metadata)的个数，通过此信息与vfmw上报的信息对比，来计算还需要配置的对应的内存的个数
    HI_BOOL                         bDPBFullCtrl;
    VDEC_PROBE_STREAM_INFO_S        stProbeStreamInfo;

    CHAN_FRAME_BUFFER_S stFrameBufferRecord[VDEC_MAX_BUFFER_RECORD];//所有帧存节点，包括预分配和动态分配

    CHAN_FRAME_BUF_RECORD_LOCK_S frameBufRecLock;
    CHAN_CONTROL_STATE_LOCK_S    chanControlStateLock;

    CHAN_CONTROL_STATE_LOCK_S stAllocMemLock;

    HI_S32 preAllocFrameNum;
    HI_S32 mmzAllocFrameNum;

#ifdef HI_HDR_SUPPORT
    VDEC_HDR_ATTR_S stVdecHdrAttr;
#endif
    HI_U32 OmxTunnelMode;
    HI_HANDLE  hOmxChn;
    HI_BOOL             bWakeUp;
    HI_BOOL             bMasterWakeUp;
    HI_BOOL             bSlaveWakeUp;
    HI_BOOL             bVirtualWakeUp;
    wait_queue_head_t   WaitQueue;
} VDEC_CHANNEL_S;

typedef struct tagVDEC_CONTROLINFO_S
{
    HI_S32  u32BackwardOptimizeFlag;  /*Backward optimize flag, 1 means optimize the backward fast play performance*/
    HI_S32  u32DispOptimizeFlag;      /*Display optimize flag, 1 means optimize the VO display performance*/
} VDEC_CONTROLINFO_S;


typedef enum
{
    VDEC_VPSS_BYPASSMODE_AUTO = 0,    /*透传与否由VPSS决策*/
    VDEC_VPSS_BYPASSMODE_ENABLE,      /*PORT透传使能*/
    VDEC_VPSS_BYPASSMODE_DISABLE,     /*PORT不透传禁止*/
    VDEC_VPSS_BYPASSMODE_BUTT         /*状态不明*/
} VDEC_BUF_BYPASSMODE_E;


typedef struct tagVDEC_VPSSCHANNEL_S
{
    HI_HANDLE hVdec;
    HI_HANDLE hVpss;
    HI_BOOL bUsed;
    HI_UNF_VIDEO_FRAME_PACKING_TYPE_E eFramePackType;
    VDEC_VPSS_PORT_PARAM_S       stPort[VDEC_MAX_PORT_NUM];
    VDEC_CONTROLINFO_S stControlInfo;
    HI_S32 s32Speed;
    HI_S32 s32GetFirstIFrameFlag;
    HI_S32 s32ImageDistance;
    HI_S32 s32GetFirstVpssFrameFlag;
    VDEC_FRAME_FORMAT_E eLastFrameFormat;
    HI_S32 s32LastFrameGopNum;
    HI_DRV_VDEC_FRAMEBUFFER_TYPE_E enFrameBuffer;
    VDEC_BUFFER_ATTR_S stBufferAttr;
    VDEC_BUF_BYPASSMODE_E enVideoBypass;
    HI_U32 u32InputWidth;
    HI_U32 u32InputHeight;
} VDEC_VPSSCHANNEL_S;

typedef struct tagVDEC_REGISTER_PARAM_S
{
    DRV_PROC_READ_FN  pfnCtrlReadProc;
    DRV_PROC_WRITE_FN pfnCtrlWriteProc;
    DRV_PROC_READ_FN  pfnReadProc;
    DRV_PROC_WRITE_FN pfnWriteProc;
    DRV_PROC_READ_FN  pfnVpuReadProc;
    DRV_PROC_WRITE_FN pfnVpuWriteProc;
} VDEC_REGISTER_PARAM_S;

typedef struct tagVDEC_PREMMZ_NODE_S
{
    HI_U8*  pu8StartVirAddr;
    HI_U32  u32StartPhyAddr;
    HI_U32  u32Size;
    HI_U32  u32NodeState;/*0:have MMZ not used,1:have MMZ but used 2:invalid*/
} VDEC_PREMMZ_NODE_S;

typedef int (*FN_VDEC_Watermark)(HI_DRV_VIDEO_FRAME_S*, VDEC_USRDAT_S* pu8UsrDataForWaterMark[4]);

HI_S32 VDEC_DRV_RegWatermarkFunc(FN_VDEC_Watermark pfnFunc);
HI_VOID VDEC_DRV_UnRegWatermarkFunc(HI_VOID);

HI_S32 VDEC_DRV_RegisterProc(VDEC_REGISTER_PARAM_S* pstParam);
HI_VOID VDEC_DRV_UnregisterProc(HI_VOID);
HI_S32 VDEC_DRV_Suspend(PM_BASEDEV_S* pdev, pm_message_t state);
HI_S32 VDEC_DRV_Resume(PM_BASEDEV_S* pdev);
VDEC_CHANNEL_S* VDEC_DRV_GetChan(HI_HANDLE hHandle);
VFMW_EXPORT_FUNC_S* VDEC_DRV_GetVfmwExportFunc(HI_VOID);

HI_S32 VDEC_DRV_Init(HI_VOID);
HI_VOID VDEC_DRV_Exit(HI_VOID);
HI_S32 VDEC_DRV_Open(struct inode* inode,  struct file*  filp);
HI_S32 VDEC_DRV_Release(struct inode* inode,  struct file*  filp);
HI_S32 VDEC_Ioctl(struct inode* inode,  struct file*  filp,  unsigned int  cmd,  void* arg);
HI_S32 VDEC_FindVpssHandleByVdecHandle(HI_HANDLE hVdec, HI_HANDLE* phVpss);
#ifdef VFMW_VPSS_BYPASS_EN   //specialFrameExist->specialFrameNum
HI_S32 VDEC_DRV_GetSpecialFrmInfo(VDEC_SPECIAL_FRM_PROC_S* pstSpecialFrmInfo);
HI_VOID VDEC_DRV_SpecialFrmListInit(HI_VOID);
HI_VOID VDEC_DRV_SpecialFrmListDeinit(HI_VOID);
#endif
HI_S32 VDEC_DRV_CreateTask(HI_VOID);
HI_VOID VDEC_DRV_DestoryTask(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_VDEC_KER_TEST_H__ */

