/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_vdec_intf_k.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/05/17
  Description   :
  History       :
  1.Date        : 2006/05/17
    Author      : g45345
    Modification: Created file
  2.Date        : 2012/08/16
    Author      : l00185424
    Modification: Reconstruction

******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>  //l00273086
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <asm/io.h>
//#include <mach/hardware.h>

/* Unf headers */
#include "hi_unf_avplay.h"
#include "hi_error_mpi.h"
#include "hi_unf_common.h"
/* Drv headers */
#include "hi_kernel_adapt.h"
#include "hi_drv_demux.h"
#include "drv_demux_ext.h"
#include "drv_vdec_ext.h"
#include "vfmw.h"
#include "vfmw_ext.h"
#include "drv_vpss_ext.h"
#include "hi_drv_vpss.h"
/* Local headers */
#include "drv_vdec_private.h"
#include "drv_vdec_pts_recv.h"
#include "drv_vdec_buf_mng.h"
#include "drv_vdec_usrdata.h"
#include "hi_drv_vdec.h"
#include "hi_mpi_vdec.h"
#include "hi_drv_stat.h"

#include <linux/dma-buf.h>    // add by l00228308

#include "drv_omxvdec.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/
#define VDH_IRQ_NUM (95 + 32)            /*interrupt vdector*/
#define MCE_INVALID_FILP (0xffffffff)
#define VDEC_NAME "HI_VDEC"
#define VDEC_IFRAME_MAX_READTIMES 2

#define HI_VMALLOC_BUFMNG(size)     HI_VMALLOC(HI_ID_VDEC, size)
#define HI_VFREE_BUFMNG(addr)       HI_VFREE(HI_ID_VDEC, addr)
#define HI_KMALLOC_ATOMIC_BUFMNG(size)     HI_KMALLOC(HI_ID_VDEC, size, GFP_ATOMIC)
#define HI_KFREE_BUFMNG(addr)       HI_KFREE(HI_ID_VDEC, addr)

//#define __VDEC_ADD_EXT_EOP__   //add by l00228308

#define VDEC_CHAN_STRMBUF_ATTACHED(pstChan) \
    (((HI_INVALID_HANDLE != pstChan->hStrmBuf) && (HI_INVALID_HANDLE == pstChan->hDmxVidChn)) \
     || ((HI_INVALID_HANDLE == pstChan->hStrmBuf) && (HI_INVALID_HANDLE != pstChan->hDmxVidChn)))

#define VDEC_CHAN_TRY_USE_DOWN(pstEnt) \
    s32Ret = VDEC_CHAN_TRY_USE_DOWN_HELP((pstEnt));

#define VDEC_CHAN_USE_UP(pstEnt) \
    VDEC_CHAN_USE_UP_HELP((pstEnt));

#define VDEC_CHAN_RLS_DOWN(pstEnt, time) \
    s32Ret = VDEC_CHAN_RLS_DOWN_HELP((pstEnt), (time));

#define VDEC_CHAN_RLS_UP(pstEnt) \
    VDEC_CHAN_RLS_UP_HELP((pstEnt));

#define HI_VDEC_SCD_EXT_MEM     (79 * 1024)
#define HI_VDEC_EOS_MEM_SIZE    (1024)

#define HI_VDEC_SVDEC_VDH_MEM   (45 * 1024 * 1024)
#define HI_VDEC_REF_FRAME_MIN   (4)
#define HI_VDEC_REF_FRAME_MAX   (16)
#define HI_VDEC_DISP_FRAME_MIN  (3)
#define HI_VDEC_DISP_FRAME_MAX  (18)
#define HI_VDEC_SVDEC_FRAME_MIN (6)

#if (1 == HI_VDEC_HD_SIMPLE)
#define HI_VDEC_BUFFER_FRAME (1)
#else
#define HI_VDEC_BUFFER_FRAME (2)
#endif

#define HI_VDEC_TREEBUFFER_MIN (11)

#define HI_VDEC_RESOCHANGE_MASK (0x1)
#define HI_VDEC_CLOSEDEI_MASK   (0x2) /* Close deinterlace */

#define HI_VDEC_CC_FROM_IMAGE (1)

#define HI_VDEC_FRAME_SIZE_1080P    (3600 * 1024)
#define HI_VDEC_FRAME_SIZE_2160P    (8100 * 1024)
#define HI_VDEC_FRAME_SIZE_4K       (15400 * 1024)

#define SMPTE_ST_2084 (16)
#define BT2020 (9)
#define BT2020_NON_CONSTANT_LUMA_SYS (9)
#define BT2020_CONSTANT_LUMA_SYS (10)


#define VDEC_InitEvent(WaitQueue) \
    do\
    {\
        init_waitqueue_head(&(WaitQueue));\
    }while(0)

#define VDEC_WaitEvent(WaitQueue, Condition, TimeOut)\
    do\
    {   HI_U32 ret;\
        ret = wait_event_interruptible_timeout(WaitQueue, Condition, TimeOut);\
        if (ret == 0)\
        {\
            HI_WARN_VDEC("[%s %d] ret=%d\n",__func__,__LINE__, ret);\
        }\
    }while(0)

#define VDEC_GiveEvent(WaitQueue)                                 \
    do\
    {\
        wake_up_interruptible(&(WaitQueue));\
    }while(0)

#define VDEC_ASSERT_RETURN(Condition, RetValue)                        \
    do                                                                 \
    {                                                                  \
        if (!Condition)                                                \
        {                                                              \
            HI_ERR_VDEC("[%s %d]assert warning\n",__func__,__LINE__);  \
            return RetValue;                                           \
        }                                                              \
    }while(0)

/*************************** Structure Definition ****************************/

/* Channel entity */
typedef struct tagVDEC_CHAN_ENTITY_S
{
    VDEC_CHANNEL_S*     pstChan;        /* Channel structure pointer for vfmw*/
    VDEC_VPSSCHANNEL_S  stVpssChan;     /* vpss Channel structure */
    HI_U64              u64File;        /* File handle*/
    HI_BOOL             bUsed;          /* Busy or free */
    atomic_t            atmUseCnt;      /* Channel use count, support multi user */
    atomic_t            atmRlsFlag;     /* Channel release flag */
    wait_queue_head_t   stRlsQue;       /* Release queue */
    EventCallBack       eCallBack;      /*for opentv5*/
    GetDmxHdlCallBack   DmxHdlCallBack; /*for opentv5*/
    HI_U32              u32DynamicFsEn; /*Dynamic frame store flag*///l00273086
    HI_U32              u32TunnelModeEn;
} VDEC_CHAN_ENTITY_S;

/* Global parameter */
typedef struct
{
    HI_U32                  u32ChanNum;     /* Record vfmw channel num */
    VDEC_CAP_S              stVdecCap;      /* Vfmw capability */
    VDEC_CHAN_ENTITY_S      astChanEntity[HI_VDEC_MAX_INSTANCE_NEW];   /* Channel parameter */
    struct semaphore        stSem;          /* Mutex */
    struct timer_list       stTimer;
    atomic_t                atmOpenCnt;     /* Open times */
    HI_BOOL                 bReady;         /* Init flag */
    HI_UNF_VCODEC_ATTR_S    stDefCfg;       /* Default channel config */
    VDEC_REGISTER_PARAM_S*  pstProcParam;   /* VDEC Proc functions */
    DEMUX_EXPORT_FUNC_S*    pDmxFunc;       /* Demux extenal functions */
    VFMW_EXPORT_FUNC_S*     pVfmwFunc;      /* VFMW extenal functions */
    VPSS_EXPORT_FUNC_S*      pVpssFunc;       /*VPSS external functions*/
    FN_VDEC_Watermark       pfnWatermark;   /* Watermark function */
    VDEC_EXPORT_FUNC_S      stExtFunc;      /* VDEC extenal functions */
    struct task_struct*     pVdecTask; //l00273086
#ifdef VFMW_VPSS_BYPASS_EN
    VDEC_List_S             stRemainFrmList;
#endif
	OMX_EXPORT_FUNC_S*      pOmxFunc;
} VDEC_GLOBAL_PARAM_S;

/***************************** Global Definition *****************************/

/* VDEC全局屏蔽控制字 bit0: DynamicFs Mask Ctrl; other: reserve*/
HI_U32 MaskCtrlWord = 0;

/*结束码大全*/
HI_CHAR g_EopTab_H264[] = {0x00, 0x00, 0x01, 0x1E, 0x48, 0x53, 0x50, 0x49, 0x43, 0x45, 0x4E, 0x44, 0x00, 0x00, 0x01, 0x00};
HI_CHAR g_EopTab_MPEG4[] = {0x00, 0x00, 0x01, 0x1E, 0x48, 0x53, 0x50, 0x49, 0x43, 0x45, 0x4E, 0x44, 0x00, 0x00, 0x01};
HI_CHAR g_EopTab_H265[] = {0x00, 0x00, 0x01, 0x60, 0x00, 0x48, 0x53, 0x50, 0x49, 0x43, 0x45, 0x4E, 0x44, 0x00, 0x00, 0x01};

HI_U32 g_CHAN_FRAME_RATE[HI_VDEC_MAX_INSTANCE_NEW] = {0};

typedef enum
{
    NORMAL_CHAN,
    ALLOC_FRAME_BUF_TO_CHAN,
    DESTORY_CHAN
} VDEC_CHANNEL_CONTROL_STATE_E;

static VDEC_CHANNEL_CONTROL_STATE_E g_ChanControlState[64];


/***************************** Static Definition *****************************/
typedef struct
{
    HI_U32        u32NeededFrameNum ;
    HI_U32        u32NeededPmvNum ;
    HI_U32        u32NeededHdrMetadataNum ;
    HI_BOOL       bAllocFrameBuffer;
    HI_BOOL       bAllocPmvBuffer;
    HI_BOOL       bAllocHdrMetadataBuffer;
} VDEC_AllocMem_Info_S;

typedef HI_S32 (*FN_IOCTL_HANDLER)(struct file*  filp, unsigned int cmd, void* arg);
typedef struct
{
    HI_U32 Code;
    FN_IOCTL_HANDLER pHandler;
}IOCTL_COMMAND_NODE;

typedef HI_S32 (*FN_VDEC_EVENT_HANDLER)(VDEC_CHANNEL_S* pstChan,
                                       HI_HANDLE hHandle,
                                       HI_VOID* pArgs);
typedef struct
{
    HI_U32 Code;
    FN_VDEC_EVENT_HANDLER pHandler;
}VDECEVENT_COMMAND_NODE;

typedef HI_VOID (*FN_CapLEVEL_HANDLER)(VDEC_CHANNEL_S* pstChan);
typedef struct
{
    HI_U32 Code;
    FN_CapLEVEL_HANDLER pHandler;
}CapLEVEL_NODE;

typedef HI_S32 (*FN_VPSSEVENT_HANDLER)(VDEC_VPSSCHANNEL_S *pstVpssChan,
                                       HI_HANDLE hVdec,
                                       HI_HANDLE hHandle,
                                       HI_VOID* pstArgs);
typedef struct
{
    HI_U32 Code;
    FN_VPSSEVENT_HANDLER pHandler;
}VPSSEVENT_COMMAND_NODE;


#if defined(HI_VIDEO_MAX_REF_FRAME_NUM) && defined(HI_VIDEO_MAX_DISP_FRAME_NUM)
static HI_S32 RefFrameNum  = HI_VIDEO_MAX_REF_FRAME_NUM;
static HI_S32 DispFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM;
#else
static HI_S32 RefFrameNum  = HI_VIDEO_MAX_REF_FRAME_NUM_IN_1024;
static HI_S32 DispFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_1024;
#endif

static HI_S32 EnVcmp = 1;
static HI_S32 En2d = 1;

#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
#define HI_VDEC_MAX_PRENODE_NUM 100
VDEC_BUFFER_S g_stVDHMMZ = {0, 0, 0};
HI_BOOL g_bVdecPreVDHMMZUsed = HI_FALSE;
HI_U32 g_VdecPreVDHMMZUsedSize = 0;
VDEC_PREMMZ_NODE_S st_VdecChanPreUsedMMZInfo[HI_VDEC_MAX_PRENODE_NUM];
HI_U32 g_VdecPreMMZNodeNum = 1;
#else
#define HI_VDEC_MAX_PRENODE_NUM 100
VDEC_BUFFER_S g_stVDHMMZ = {0, 0, 0};
HI_BOOL g_bVdecPreVDHMMZUsed = HI_FALSE;
HI_U32 g_VdecPreVDHMMZUsedSize = 0;
VDEC_PREMMZ_NODE_S st_VdecChanPreUsedMMZInfo[HI_VDEC_MAX_PRENODE_NUM];
HI_U32 g_VdecPreMMZNodeNum = 0;
#endif


#if (1 == PRE_ALLOC_VDEC_SCD_MMZ)
VDEC_BUFFER_S g_stSCDMMZ = {0, 0, 0};
HI_BOOL g_bVdecPreSCDMMZUsed = HI_FALSE;
#endif

#if (1 == PRE_ALLOC_VDEC_ESBUF_MMZ)
#define ESBUF_MMZ_SIZE (20*1024*1024)
VDEC_BUFFER_S g_stESBUFMMZ = {0, 0, 0};
HI_BOOL g_bVdecPreESBUFMMZUsed = HI_FALSE;
#endif

static HI_S32   VDEC_RegChanProc(HI_S32 s32Num);
static HI_VOID  VDEC_UnRegChanProc(HI_S32 s32Num);
static HI_S32   VDEC_Chan_VpssRecvFrmBuf(VPSS_HANDLE hVpss, HI_DRV_VIDEO_FRAME_S* pstFrm);
static HI_S32   VDEC_Chan_VpssRlsFrmBuf(VPSS_HANDLE hVpss, HI_DRV_VIDEO_FRAME_S*  pstFrm);

static HI_VOID VDEC_ConvertFrm(HI_UNF_VCODEC_TYPE_E enType, VDEC_CHANNEL_S* pstChan,
                               IMAGE* pstImage, HI_DRV_VIDEO_FRAME_S* pstFrame);
static HI_S32 VDEC_FindVdecHandleByVpssHandle(VPSS_HANDLE hVpss, HI_HANDLE* hVdec);
static HI_U32 VDEC_ConverColorSpace(HI_U32 u32ColorSpace);
static HI_UNF_VIDEO_FRAME_PACKING_TYPE_E VDEC_ConverFrameType(HI_DRV_FRAME_TYPE_E  eFrmType);
static HI_VOID VDEC_GetMemConfig(HI_U32* pu32ScdBufSize, HI_U32* pu32VdhBufSize);

static HI_S32 VDEC_ResetChanFrmBuf(HI_S32 chanID);

static HI_S32 VDEC_AddFrmBufRec(HI_S32 chanID, FRAME_BUF_S* frmBufRec, UINT32* pu32BufIndex);
static HI_S32 VDEC_DelFrmBufRec(HI_S32 chanID, UINT32 u32BufIndex);

static HI_S32 VDEC_SetChanControlState(HI_S32 chanID, VDEC_CHANNEL_CONTROL_STATE_E chanControlState);

static HI_S32 VDEC_ReleaseMemForDecoder(HI_HANDLE hHandle);

#ifdef VFMW_VPSS_BYPASS_EN
#define D_VDEC_CHECK_PTR_RET(ptr) \
    do {\
        if (HI_NULL == ptr)\
        { \
            HI_ERR_VDEC("PTR '%s' is NULL.\n", # ptr); \
            return HI_ERR_VDEC_NULL_PTR;           \
        }  \
    } while (0)

#define D_VDEC_CHECK_PTR(ptr) \
    do {\
        if (HI_NULL == ptr)\
        { \
            HI_ERR_VDEC("PTR '%s' is NULL.\n", # ptr); \
            return;           \
        }  \
    } while (0)


static HI_VOID BUFMNG_VDEC_List_Init(VDEC_List_S* pList);
static HI_VOID BUFMNG_VDEC_List_Deinit(VDEC_List_S* pList);
static HI_S32 BUFMNG_VDEC_List_Add(VDEC_List_S* pList, VDEC_SPECIAL_FRM_INFO_S* pSpecialFrmRec);
static HI_S32 BUFMNG_VDEC_List_Del(VDEC_List_S* pList, HI_U32 u32Index);
static HI_S32 BUFMNG_VDEC_List_FindNodeIndex(VDEC_List_S* pList, HI_U32 u32TargetPhyAddr, HI_U32* pIndex);
static HI_S32 BUFMNG_VDEC_List_FindNodeIndexCanRls(VDEC_List_S* pList, HI_U32* pIndex);

HI_S32 HI_DRV_VDEC_GlobalRelFrm(HI_DRV_VIDEO_FRAME_S* pstFrame);
HI_S32 decoder_global_release_frame(HI_DRV_VIDEO_FRAME_S* pstFrame);
HI_S32 HI_DRV_VDEC_GetVideoBypassInfo(HI_HANDLE hHandle, HI_BOOL* pbVideoBypass);

static HI_BOOL VDEC_IsSpecialFrm(VDEC_BUFFER_S* pstInputFrmRec);
static HI_S32 VDEC_MarkSpecialFrmCanRls(HI_DRV_VIDEO_FRAME_S* pstFrame);

HI_S32 VDEC_DRV_GlobalRelFrmInter(HI_U32 u32Index);

#endif

static VDEC_GLOBAL_PARAM_S s_stVdecDrv =
{
    .atmOpenCnt = ATOMIC_INIT(0),
    .bReady = HI_FALSE,
    .stDefCfg =
    {
        .enType         = HI_UNF_VCODEC_TYPE_H264,
        .enMode         = HI_UNF_VCODEC_MODE_NORMAL,
        .u32ErrCover    = 100,
        .bOrderOutput   = 0,
        .u32Priority    = 15
    },
    .pstProcParam = HI_NULL,
    .pDmxFunc = HI_NULL,
    .pVfmwFunc = HI_NULL,
    .pVpssFunc = HI_NULL,
    .pfnWatermark = HI_NULL,
    .stExtFunc =
    {
        .pfnVDEC_Suspend     = (HI_VOID*)VDEC_DRV_Suspend,
        .pfnVDEC_Resume      = (HI_VOID*)VDEC_DRV_Resume,
        .pfnVOAcqFrame       = (HI_VOID*)HI_VDEC_Chan_VOAcqFrame,
        .pfnVORlsFrame       = (HI_VOID*)HI_VDEC_Chan_VORlsFrame,
        .pfnVOSendWinInfo    = (HI_VOID*)HI_VDEC_Chan_VOChangeWinInfo,
        .pfnVDEC_SetOmxCallBacks = (HI_VOID*)HI_DRV_VDEC_SetOmxCallBacks,
        .pfnVDEC_ReportEsRels = (HI_VOID*)HI_DRV_VDEC_ReportEsRels
    },
    .pVdecTask = HI_NULL, //l00273086

#ifdef VFMW_VPSS_BYPASS_EN
    .stRemainFrmList.bInit      = 0,
    .stRemainFrmList.s32Num     = 0,
#endif
    .pOmxFunc = HI_NULL,
};

/*********************************** Code ************************************/

#if (VDEC_DEBUG == 1)
static HI_VOID VDEC_PrintImage(IMAGE* pstImg)
{
    HI_FATAL_VDEC("<0>top_luma_phy_addr = 0x%08x \n", pstImg->top_luma_phy_addr);
    HI_FATAL_VDEC("<0>top_chrom_phy_addr = 0x%08x \n", pstImg->top_chrom_phy_addr);
    HI_FATAL_VDEC("<0>btm_luma_phy_addr = 0x%08x \n", pstImg->btm_luma_phy_addr);
    HI_FATAL_VDEC("<0>btm_chrom_phy_addr = 0x%08x \n", pstImg->btm_chrom_phy_addr);
    HI_FATAL_VDEC("<0>disp_width = %d \n", pstImg->disp_width);
    HI_FATAL_VDEC("<0>disp_height = %d \n", pstImg->disp_height);
    HI_FATAL_VDEC("<0>disp_center_x = %d \n", pstImg->disp_center_x);
    HI_FATAL_VDEC("<0>disp_center_y = %d \n", pstImg->disp_center_y);
    HI_FATAL_VDEC("<0>error_level = %d \n", pstImg->error_level);
    HI_FATAL_VDEC("<0>seq_cnt = %d \n", pstImg->seq_cnt);
    HI_FATAL_VDEC("<0>seq_img_cnt = %d \n", pstImg->seq_img_cnt);
    HI_FATAL_VDEC("<0>PTS = %lld \n", pstImg->PTS);
}

static HI_VOID VDEC_PrintFrmInfo(HI_UNF_VIDEO_FRAME_INFO_S* pstFrame)
{
    HI_FATAL_VDEC("<0>u32Height = %d\n", pstFrame->u32Height);
    HI_FATAL_VDEC("<0>u32Width = %d\n", pstFrame->u32Width);
    HI_FATAL_VDEC("<0>u32DisplayWidth = %d\n", pstFrame->u32DisplayWidth);
    HI_FATAL_VDEC("<0>u32DisplayHeight = %d\n", pstFrame->u32DisplayHeight);
    HI_FATAL_VDEC("<0>u32DisplayCenterX = %d\n", pstFrame->u32DisplayCenterX);
    HI_FATAL_VDEC("<0>u32DisplayCenterY = %d\n", pstFrame->u32DisplayCenterY);
}
#endif

static inline HI_S32  VDEC_CHAN_TRY_USE_DOWN_HELP(VDEC_CHAN_ENTITY_S* pstEnt)
{
    atomic_inc(&pstEnt->atmUseCnt);

    if (atomic_read(&pstEnt->atmRlsFlag) != 0)
    {
        atomic_dec(&pstEnt->atmUseCnt);

        if (atomic_read(&pstEnt->atmRlsFlag) != 1)
        {
            HI_ERR_VDEC("Use lock err\n");
            //while (1)
            //{}
        }

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static inline HI_S32  VDEC_CHAN_USE_UP_HELP(VDEC_CHAN_ENTITY_S* pstEnt)
{
    if (atomic_dec_return(&pstEnt->atmUseCnt) < 0)
    {
        HI_ERR_VDEC("Use unlock err\n");
        //while (1)
        //{}
    }

    return HI_SUCCESS;
}

static inline HI_S32  VDEC_CHAN_RLS_DOWN_HELP(VDEC_CHAN_ENTITY_S* pstEnt, HI_U32 time)
{
    HI_S32 s32Ret;

    /* Realse all */
    /* CNcomment:多个进行释放 */
    if (atomic_inc_return(&pstEnt->atmRlsFlag) != 1)
    {
        atomic_dec(&pstEnt->atmRlsFlag);
        return HI_FAILURE;
    }

    if (atomic_read(&pstEnt->atmUseCnt) != 0)
    {
        if (HI_INVALID_TIME == time)
        {
            s32Ret = wait_event_interruptible(pstEnt->stRlsQue, (atomic_read(&pstEnt->atmUseCnt) == 0));
        }
        else
        {
            s32Ret = wait_event_interruptible_timeout(pstEnt->stRlsQue, (atomic_read(&pstEnt->atmUseCnt) == 0), time);
        }

        if (s32Ret == 0)
        {
            return HI_SUCCESS;
        }
        else
        {
            if (s32Ret < 0)
            {
                return HI_SUCCESS;
            }

            atomic_dec(&pstEnt->atmRlsFlag);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

static inline HI_S32  VDEC_CHAN_RLS_UP_HELP(VDEC_CHAN_ENTITY_S* pstEnt)
{
    if (atomic_dec_return(&pstEnt->atmRlsFlag) < 0)
    {
        //while (1)
        //{}
    }

    return HI_SUCCESS;
}
/* 初始化互斥锁*/
HI_S32  VDEC_InitSpinLock( VDEC_PORT_FRAME_LIST_LOCK_S* pIntrMutex )
{
    if (HI_NULL == pIntrMutex)
    {
        return HI_FAILURE;
    }

    spin_lock_init(&pIntrMutex->irq_lock);
    pIntrMutex->isInit = HI_TRUE;
    return HI_SUCCESS;
}
/* 中断互斥加锁(关中断且加锁)*/
HI_S32 VDEC_SpinLockIRQ( VDEC_PORT_FRAME_LIST_LOCK_S* pIntrMutex )
{
    if (HI_NULL == pIntrMutex)
    {
        return HI_FAILURE;
    }

    if (pIntrMutex->isInit == HI_FALSE)
    {
        spin_lock_init(&pIntrMutex->irq_lock);
        pIntrMutex->isInit = HI_TRUE;
    }

    spin_lock_irqsave(&pIntrMutex->irq_lock, pIntrMutex->irq_lockflags);

    return HI_SUCCESS;
}
/* 中断互斥解锁(开中断且去锁)*/
HI_S32 VDEC_SpinUnLockIRQ( VDEC_PORT_FRAME_LIST_LOCK_S* pIntrMutex )
{
    if (HI_NULL == pIntrMutex)
    {
        return HI_FAILURE;
    }

    if (pIntrMutex->isInit == HI_TRUE)
    {
        spin_unlock_irqrestore(&pIntrMutex->irq_lock, pIntrMutex->irq_lockflags);
    }

    return HI_SUCCESS;
}

HI_S32  VDEC_InitFrmBufRecSpinLock(CHAN_FRAME_BUF_RECORD_LOCK_S* pIntrMutex)
{
    spin_lock_init(&pIntrMutex->irq_lock);
    pIntrMutex->isInit = HI_TRUE;
    return HI_SUCCESS;
}

/* 中断互斥加锁(关中断且加锁)*/
HI_S32 VDEC_FrmBufRecSpinLockIRQ(CHAN_FRAME_BUF_RECORD_LOCK_S* pIntrMutex)
{
    if (pIntrMutex->isInit == HI_FALSE)
    {
        spin_lock_init(&pIntrMutex->irq_lock);
        pIntrMutex->isInit = HI_TRUE;
    }

    spin_lock_irqsave(&pIntrMutex->irq_lock, pIntrMutex->irq_lockflags);

    return HI_SUCCESS;
}

/* 中断互斥解锁(开中断且去锁)*/
HI_S32 VDEC_FrmBufRecSpinUnLockIRQ(CHAN_FRAME_BUF_RECORD_LOCK_S* pIntrMutex)
{
    if (pIntrMutex->isInit == HI_TRUE)
    {
        spin_unlock_irqrestore(&pIntrMutex->irq_lock, pIntrMutex->irq_lockflags);
    }

    return HI_SUCCESS;
}


#ifdef VFMW_VPSS_BYPASS_EN
static HI_S32 VDEC_FindSpecialFrmToNormalIndex(VDEC_CHANNEL_S* pstChan, HI_U32 u32Phyaddr, HI_U32* pIndex)
{
    HI_U32 index;
    D_VDEC_CHECK_PTR_RET(pstChan);

    if ((u32Phyaddr == 0) || (u32Phyaddr == 0xffffffff))
    {
        return HI_FAILURE;
    }

    for (index = 0; index < VDEC_MAX_BUFFER_RECORD; index++)
    {
        if (pstChan->stFrameBufferRecord[index].stFrameBuffer.u32StartPhyAddr == u32Phyaddr)
        {
            *pIndex = index;
            return HI_SUCCESS;
        }
    }
    HI_ERR_VDEC("find normal frame buffer index error!Phyaddr = 0x%x\n", u32Phyaddr);
    return HI_FAILURE;
}
#endif


HI_S32 VDEC_InitChanControlStateLock(CHAN_CONTROL_STATE_LOCK_S* pIntrMutex)
{
    spin_lock_init(&pIntrMutex->irq_lock);
    pIntrMutex->isInit = HI_TRUE;
    return HI_SUCCESS;
}

HI_S32 VDEC_ChanControlStateSpinLockIRQ(CHAN_CONTROL_STATE_LOCK_S* pIntrMutex)
{
    if (pIntrMutex->isInit == HI_FALSE)
    {
        spin_lock_init(&pIntrMutex->irq_lock);
        pIntrMutex->isInit = HI_TRUE;
    }

    spin_lock_irqsave(&pIntrMutex->irq_lock, pIntrMutex->irq_lockflags);

    return HI_SUCCESS;
}

HI_S32 VDEC_ChanControlStateSpinUnlockIRQ(CHAN_CONTROL_STATE_LOCK_S* pIntrMutex)
{
    if (pIntrMutex->isInit == HI_TRUE)
    {
        spin_unlock_irqrestore(&pIntrMutex->irq_lock, pIntrMutex->irq_lockflags);
    }

    return HI_SUCCESS;
}

HI_S32  BUFMNG_InitSpinLock( BUFMNG_VPSS_IRQ_LOCK_S *pIntrMutex )
{
    if (HI_NULL == pIntrMutex)
    {
        return HI_FAILURE;
    }

    spin_lock_init(&pIntrMutex->irq_lock);
    pIntrMutex->isInit = HI_TRUE;
    return HI_SUCCESS;
}
/* 中断互斥加锁(关中断且加锁)*/
HI_S32 BUFMNG_SpinLockIRQ( BUFMNG_VPSS_IRQ_LOCK_S* pIntrMutex )
{
    if (HI_NULL == pIntrMutex)
    {
        return HI_FAILURE;
    }

    if (pIntrMutex->isInit == HI_FALSE)
    {
        spin_lock_init(&pIntrMutex->irq_lock);
        pIntrMutex->isInit = HI_TRUE;
    }

    spin_lock_irqsave(&pIntrMutex->irq_lock, pIntrMutex->irq_lockflags);

    return HI_SUCCESS;
}
/* 中断互斥解锁(开中断且去锁)*/
HI_S32 BUFMNG_SpinUnLockIRQ( BUFMNG_VPSS_IRQ_LOCK_S* pIntrMutex )
{
    if (HI_NULL == pIntrMutex)
    {
        return HI_FAILURE;
    }

    if (pIntrMutex->isInit == HI_TRUE)
    {
        spin_unlock_irqrestore(&pIntrMutex->irq_lock, pIntrMutex->irq_lockflags);
    }

    return HI_SUCCESS;
}
static HI_S32 BUFMNG_VPSS_Init(BUFMNG_VPSS_INST_S* pstBufVpssInst)
{
    HI_S32 i;
    BUFMNG_VPSS_NODE_S* pstBufNode;
    BUFMNG_VPSS_NODE_S* pstTarget;
    struct list_head* pos, *n;
    HI_S32 s32Ret = HI_SUCCESS;
    //s32Ret  = BUFMNG_InitSpinLock(&pstBufVpssInst->stAvailableListLock);
    memset(&pstBufVpssInst->stUnAvailableListLock, 0, sizeof(BUFMNG_VPSS_IRQ_LOCK_S));
    s32Ret  = BUFMNG_InitSpinLock(&pstBufVpssInst->stUnAvailableListLock);
    INIT_LIST_HEAD(&pstBufVpssInst->stVpssBufAvailableList);
    INIT_LIST_HEAD(&pstBufVpssInst->stVpssBufUnAvailableList);

    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (HI_DRV_VDEC_BUF_VDEC_ALLOC_MANAGE == pstBufVpssInst->enFrameBuffer)
    {
        for (i = 0; i < pstBufVpssInst->u32BufNum; i++)
        {
            pstBufNode = HI_VMALLOC_BUFMNG(sizeof(BUFMNG_VPSS_NODE_S));

            if (HI_NULL == pstBufNode)
            {
                HI_VFREE_BUFMNG(pstBufNode);
                HI_ERR_VDEC("BUFMNG_VPSS_Init No memory.\n");
                return HI_ERR_BM_NO_MEMORY;
            }

            s32Ret = HI_DRV_VDEC_AllocAndMap("VDEC_VPSSBuf", "VDEC", pstBufVpssInst->u32FrmBufSize, 0, &pstBufNode->VDECBuf_frm);
            s32Ret |= HI_DRV_VDEC_AllocAndMap("VDEC_VPSSBuf_Meta", "VDEC", pstBufVpssInst->u32MetaBufSize, 0, &pstBufNode->VDECBuf_meta);

            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VDEC("BUFMNG_VPSS_Init Alloc MMZ fail:0x%x.\n", s32Ret);
                list_for_each_safe(pos, n, &(pstBufVpssInst->stVpssBufAvailableList))
                {
                    pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);
                    HI_DRV_VDEC_UnmapAndRelease(&(pstTarget->VDECBuf_frm));
                    HI_DRV_VDEC_UnmapAndRelease(&(pstTarget->VDECBuf_meta));//DRM: meta内存为非安内存 c00277632
                    list_del_init(pos);
                    HI_VFREE_BUFMNG(pstTarget);
                }
                HI_VFREE_BUFMNG(pstBufNode);
                return HI_ERR_BM_NO_MEMORY;
            }

            BUFMNG_SpinLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
            pstBufNode->bBufUsed = HI_FALSE;
            pstBufVpssInst->u32AvaiableFrameCnt = 0;
            list_add_tail(&(pstBufNode->node), &(pstBufVpssInst->stVpssBufAvailableList));
            BUFMNG_SpinUnLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
        }
    }
    else if (HI_DRV_VDEC_BUF_USER_ALLOC_MANAGE == pstBufVpssInst->enFrameBuffer)
    {
#if 0
        pstBufVpssInst->stBufferAttr.u32BufNum = 10;

        for (i = 0; i < pstBufVpssInst->stBufferAttr.u32BufNum; i++)
        {
            pstBufNode = HI_VMALLOC_BUFMNG(sizeof(BUFMNG_VPSS_NODE_S));

            if (HI_NULL == pstBufNode)
            {
                HI_VFREE_BUFMNG(pstBufNode);
                HI_ERR_VDEC("BUFMNG_VPSS_Init No memory.\n");
                return HI_ERR_BM_NO_MEMORY;
            }

#if 1
            s32Ret = HI_DRV_MMZ_AllocAndMap("VDEC_ALLOC_VPSSBuf", "VDEC", 3 * 1024 * 1024, 0, &pstBufNode->stMMZBuf);
#else
            pstBufNode->stMMZBuf.u32StartPhyAddr = pstBufVpssInst->stBufferAttr.u32PhyAddr[i];
            pstBufNode->stMMZBuf.u32StartVirAddr = pstBufVpssInst->stBufferAttr.u32UsrVirAddr[i];
            pstBufNode->stMMZBuf.u32Size         = pstBufVpssInst->stBufferAttr.u32BufSize;
#endif
            BUFMNG_SpinLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
            pstBufNode->bBufUsed = HI_FALSE;
            pstBufVpssInst->u32AvaiableFrameCnt = 0;
            list_add_tail(&(pstBufNode->node), &(pstBufVpssInst->stVpssBufAvailableList));
            BUFMNG_SpinUnLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
        }

#endif
#if 0
        VDEC_BUFFER_ATTR_S stBufferAttr;
        VDEC_Chan_SetExtBuffer(0, &stBufferAttr);
#endif
        BUFMNG_SpinLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
        pstBufVpssInst->u32AvaiableFrameCnt = 0;
        pstBufVpssInst->u32BufNum           = 0;
        pstBufVpssInst->enExtBufferState    = VDEC_EXTBUFFER_STATE_START;
        BUFMNG_SpinUnLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
    }

    pstBufVpssInst->pstUnAvailableListPos = &pstBufVpssInst->stVpssBufUnAvailableList;
    return s32Ret;
}

static HI_S32 BUFMNG_VPSS_DeInit(BUFMNG_VPSS_INST_S* pstBufVpssInst)
{
    HI_S32 s32Ret = HI_SUCCESS;
    struct list_head *pos, *n;
    BUFMNG_VPSS_NODE_S *pstTarget;

    if (HI_DRV_VDEC_BUF_VDEC_ALLOC_MANAGE == pstBufVpssInst->enFrameBuffer)
    {
        list_for_each_safe(pos, n, &(pstBufVpssInst->stVpssBufAvailableList))
        {
            pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);
            HI_DRV_VDEC_UnmapAndRelease(&(pstTarget->VDECBuf_frm));
            HI_DRV_VDEC_UnmapAndRelease(&(pstTarget->VDECBuf_meta));
            list_del_init(pos);
            vfree(pstTarget);
        }

        list_for_each_safe(pos, n, &(pstBufVpssInst->stVpssBufUnAvailableList))
        {
            pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);
            HI_DRV_VDEC_UnmapAndRelease(&(pstTarget->VDECBuf_frm));
            HI_DRV_VDEC_UnmapAndRelease(&(pstTarget->VDECBuf_meta));
            list_del_init(pos);
            vfree(pstTarget);
        }
    }

    if (HI_DRV_VDEC_BUF_USER_ALLOC_MANAGE == pstBufVpssInst->enFrameBuffer)
    {
        BUFMNG_SpinLockIRQ(&pstBufVpssInst->stAvailableListLock);
        list_for_each_safe(pos, n, &(pstBufVpssInst->stVpssBufAvailableList))
        {
            pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);
            list_del_init(pos);
            HI_VFREE_BUFMNG(pstTarget);
        }
        BUFMNG_SpinUnLockIRQ(&pstBufVpssInst->stAvailableListLock);
        BUFMNG_SpinLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
        list_for_each_safe(pos, n, &(pstBufVpssInst->stVpssBufUnAvailableList))
        {
            pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);
            list_del_init(pos);
            HI_VFREE_BUFMNG(pstTarget);
        }
        pstBufVpssInst->u32AvaiableFrameCnt = 0;
        pstBufVpssInst->u32BufNum           = 0;
        pstBufVpssInst->u32FrmBufSize       = 0;
        pstBufVpssInst->u32MetaBufSize      = 0;
        pstBufVpssInst->enExtBufferState    = VDEC_EXTBUFFER_STATE_START;
        BUFMNG_SpinUnLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
    }

    return s32Ret;
}

static HI_S32 BUFMNG_VPSS_Reset(BUFMNG_VPSS_INST_S* pstBufVpssInst)
{
    //return HI_SUCCESS;
#if 1
    BUFMNG_VPSS_NODE_S* pstTarget;
    struct list_head* pos, *n;
    HI_S32 s32Ret = HI_SUCCESS;

    if (HI_DRV_VDEC_BUF_VDEC_ALLOC_MANAGE == pstBufVpssInst->enFrameBuffer)
    {
        BUFMNG_SpinLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
        list_for_each_safe(pos, n, &(pstBufVpssInst->stVpssBufUnAvailableList))
        {
            pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);
            list_del_init(pos);
            pstTarget->bBufUsed = HI_FALSE;
            list_add_tail(&(pstTarget->node), &(pstBufVpssInst->stVpssBufAvailableList));
        }
        pstBufVpssInst->u32AvaiableFrameCnt = 0;
        pstBufVpssInst->pstUnAvailableListPos = &pstBufVpssInst->stVpssBufUnAvailableList;
        BUFMNG_SpinUnLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
    }

    if (HI_DRV_VDEC_BUF_USER_ALLOC_MANAGE == pstBufVpssInst->enFrameBuffer)
    {
        BUFMNG_SpinLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
        list_for_each_safe(pos, n, &(pstBufVpssInst->stVpssBufUnAvailableList))
        {
            pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);

            if (pstTarget->enFrameBufferState != HI_DRV_VDEC_BUF_STATE_IN_USER)
            {
                pstTarget->bBufUsed = HI_FALSE;
                pstTarget->enFrameBufferState = HI_DRV_VDEC_BUF_STATE_IN_VDEC_EMPTY;

                if (pstBufVpssInst->pstUnAvailableListPos == pos)
                {
                    pstBufVpssInst->pstUnAvailableListPos = pos->prev;
                }

                list_del_init(pos);
                list_add_tail(&(pstTarget->node), &(pstBufVpssInst->stVpssBufAvailableList));
            }
            else
            {
                pstBufVpssInst->pstUnAvailableListPos = pos;
            }
        }
        pstBufVpssInst->u32AvaiableFrameCnt = 0;
        BUFMNG_SpinUnLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
    }

    return s32Ret;
#endif
}
static HI_S32 BUFMNG_VPSS_CheckAvaibleBuffer(HI_HANDLE hVpss, HI_HANDLE hPort)
{
    HI_S32 s32Ret;
    HI_S32 i, j;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;

    if (HI_INVALID_HANDLE == hPort || HI_INVALID_HANDLE == hVpss)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Too many chans!\n");
        return HI_FAILURE;
    }

    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", i);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if (hPort == pstVpssChan->stPort[j].hPort)
        {
            break;
        }
    }

    if (j >= VDEC_MAX_PORT_NUM)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        HI_ERR_VDEC("Port %d not exist!\n", hPort);
        return HI_FAILURE;
    }
    else
    {
#ifdef VFMW_VPSS_BYPASS_EN
        /*first check the buffer state*/
        if (VDEC_EXTBUFFER_STATE_START != pstVpssChan->stPort[0].stBufVpssInst.enExtBufferState)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
            return HI_FAILURE;
        }
#endif

        BUFMNG_SpinLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);

        if (list_empty(&pstVpssChan->stPort[j].stBufVpssInst.stVpssBufAvailableList))
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
            BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
            return HI_FAILURE;
        }

        BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
    return HI_SUCCESS;
}

static HI_S32 BUFMNG_VPSS_RecBuffer(HI_HANDLE hVpss, HI_HANDLE hPort, BUF_VPSS_S* pstMMZ_Buffer, HI_U32* pu32Stride)
{
    HI_S32 s32Ret;
    HI_S32 i, j;
    struct list_head* pos;
    BUFMNG_VPSS_NODE_S* pstTarget;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;

    if (HI_INVALID_HANDLE == hPort || HI_INVALID_HANDLE == hVpss)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    /*首先应该找到hVpss对应的是第几个astChanEntity*/
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Too many chans!\n");
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", i);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

    /*查找port是否存在*/
    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if (hPort == pstVpssChan->stPort[j].hPort)
        {
            break;
        }
    }

    if (j >= VDEC_MAX_PORT_NUM)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        HI_WARN_VDEC("Port %d not exist!\n", hPort);
        return HI_FAILURE;
    }

    /*此状态有上层控制当设置为STOP状态的时候意味不允许VPSS使用外部帧存*/
    if (VDEC_EXTBUFFER_STATE_START != pstVpssChan->stPort[0].stBufVpssInst.enExtBufferState)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        return HI_FAILURE;
    }

    /*获取帧存*/
    if (VDEC_VPSS_BYPASSMODE_ENABLE != pstVpssChan->enVideoBypass)
    {
        if (pstVpssChan->stPort[j].stBufVpssInst.u32FrmBufSize < pstMMZ_Buffer->VDECBuf_frm.u32Size)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
            HI_WARN_VDEC("the frame size is change,before change FrmBuf:%d vpss want FrmBuf:%d \n", pstVpssChan->stPort[j].stBufVpssInst.u32FrmBufSize, \
                         pstMMZ_Buffer->VDECBuf_frm.u32Size);
            return HI_FAILURE;
        }
    }

    BUFMNG_SpinLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);

    if ((&pstVpssChan->stPort[j].stBufVpssInst.stVpssBufAvailableList) == pstVpssChan->stPort[j].stBufVpssInst.stVpssBufAvailableList.next)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
        return HI_FAILURE;
    }

    pos = pstVpssChan->stPort[j].stBufVpssInst.stVpssBufAvailableList.next;
    pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);
    pstTarget->enFrameBufferState = HI_DRV_VDEC_BUF_STATE_IN_VPSS;
    /*删除stVpssBufAvailableList上的pos节点*/
    list_del_init(pos);
    pstTarget->bBufUsed = HI_TRUE;
    /*将pos节点添加到stVpssBufUnAvailableList上去*/
    list_add_tail(pos, &(pstVpssChan->stPort[j].stBufVpssInst.stVpssBufUnAvailableList));

    memcpy(&pstMMZ_Buffer->VDECBuf_frm, &pstTarget->VDECBuf_frm, sizeof(VDEC_BUFFER_S));

    //当 地址为-1时，表示上层未分配metabuf,但还是把结构体传给vpss，给vpss做判断
    if (pstTarget->VDECBuf_meta.u32StartPhyAddr == 0xffffffff)
    {
        HI_WARN_VDEC("meta buf not alloc!\n");
    }

    memcpy(&pstMMZ_Buffer->VDECBuf_meta, &pstTarget->VDECBuf_meta, sizeof(VDEC_BUFFER_S));

    *pu32Stride = pstTarget->u32Stride;
    BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
    return HI_SUCCESS;
}
static HI_S32 BUFMNG_VPSS_RelBuffer(HI_HANDLE hVpss, HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstImage)
{
    HI_S32 s32Ret;
    HI_S32 i, j;
    struct list_head* pos, *n;
    BUFMNG_VPSS_NODE_S* pstTarget;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
#ifdef VFMW_VPSS_BYPASS_EN
    HI_BOOL need_release_frm = HI_FALSE;
    HI_DRV_VIDEO_FRAME_S stPrivFrmInfo = {0};
    HI_DRV_VIDEO_FRAME_S* pstPrivFrame = HI_NULL;
    memset(&stPrivFrmInfo, 0, sizeof(HI_DRV_VIDEO_FRAME_S));
#endif

    if (HI_INVALID_HANDLE == hPort || HI_INVALID_HANDLE == hVpss)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Too many chans!\n");
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", i);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

    /*查找port是否存在*/
    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if (hPort == pstVpssChan->stPort[j].hPort)
        {
            break;
        }
    }

    if (j >= VDEC_MAX_PORT_NUM)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        HI_WARN_VDEC("Port %d not exist!\n", hPort);
        return HI_FAILURE;
    }
    else
    {
        /*释放帧存*/
        //pos从stVpssBufUnAvailableList的next开始
        BUFMNG_SpinLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
        list_for_each_safe(pos, n, &(pstVpssChan->stPort[j].stBufVpssInst.stVpssBufUnAvailableList))
        {
            pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);

            if (pstImage->stBufAddr[0].u32PhyAddr_Y == pstTarget->VDECBuf_frm.u32StartPhyAddr) //C00277632
            {
                //pstImage->stBufAddr[0].u32PhyAddr_Y,pstTarget->stMMZBuf.u32StartPhyAddr);
#ifdef VFMW_VPSS_BYPASS_EN
                if (pstVpssChan->enVideoBypass == VDEC_VPSS_BYPASSMODE_ENABLE)
                {
                    //如果是VPSS bypass 通路找到metadata对应的物理帧存地址来进行还帧
                    //pstPrivFrame = (HI_DRV_VIDEO_FRAME_S *)phys_to_virt(pstTarget->VDECBuf_meta.u32StartPhyAddr);
                    pstPrivFrame = (HI_DRV_VIDEO_FRAME_S*)pstTarget->VDECBuf_meta.pu8StartVirAddr;
                    if (pstPrivFrame)
                    {
                        /*暂时存储metadata中的真正VFMW传出的帧信息结构体，以备下面还帧之用，此处不能有锁，不能直接在此还帧*/
                        memcpy(&stPrivFrmInfo, pstPrivFrame, sizeof(HI_DRV_VIDEO_FRAME_S));

                        /*及时清除metadata里面的帧信息，避免外部误判断*/
                        memset(pstPrivFrame, 0 , sizeof(HI_DRV_VIDEO_FRAME_S));
                        need_release_frm = HI_TRUE;
                    }
                    else
                    {
                        HI_ERR_VDEC("convert the metadata FrmInfo error!\n");
                    }
                }
#endif
                pstTarget->bBufUsed = HI_FALSE;
                pstTarget->enFrameBufferState = HI_DRV_VDEC_BUF_STATE_IN_VDEC_EMPTY;

                /*删除stVpssBufUnAvailableList上的pos节点*/
                if (pstVpssChan->stPort[j].stBufVpssInst.pstUnAvailableListPos == pos)
                {
                    pstVpssChan->stPort[j].stBufVpssInst.pstUnAvailableListPos = pos->prev;
                }

                list_del_init(pos);
                /*将pos节点添加到stVpssBufAvailableList上去*/
                list_add_tail(&(pstTarget->node), &(pstVpssChan->stPort[j].stBufVpssInst.stVpssBufAvailableList));
                break;
            }
        }
        BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);

#ifdef VFMW_VPSS_BYPASS_EN
     if (pstVpssChan->enVideoBypass == VDEC_VPSS_BYPASSMODE_ENABLE && need_release_frm)
    {
        /*此处向VFMW还帧 add by l00228308*/
        s32Ret = VDEC_Chan_VpssRlsFrmBuf(hVpss, &stPrivFrmInfo);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("VDEC_Chan_VpssRlsFrmBuf failed!return 0x%x,PhyAddr = 0x%x \n", s32Ret, stPrivFrmInfo.stBufAddr[0].u32PhyAddr_Y);
            return s32Ret;
        }
    }
#endif
    return HI_SUCCESS;
}
static VOID HI_DRV_VDEC_REPORT_TO_OMX(HI_S32 chan_id, HI_U32 EventID, HI_S32 result, HI_VOID* PrivData)
{
    if ((NULL != (s_stVdecDrv.pOmxFunc)) && (NULL != (s_stVdecDrv.pOmxFunc->pfnOmxVdecEventReport)))
    {
        (s_stVdecDrv.pOmxFunc->pfnOmxVdecEventReport)(chan_id, EventID, result, PrivData);
    }
}

static HI_S32 VDEC_VpssNewImageEvent(HI_HANDLE hVpss, HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstImage)
{
    HI_S32 s32Ret;
    HI_S32 i, j;
    struct list_head* pos, *n;
    BUFMNG_VPSS_NODE_S* pstTarget;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;

    if (HI_INVALID_HANDLE == hPort || HI_INVALID_HANDLE == hVpss)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Too many chans!\n");
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", i);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

    /*查找port是否存在*/
    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if (hPort == pstVpssChan->stPort[j].hPort)
        {
            break;
        }
    }

    if (j >= VDEC_MAX_PORT_NUM)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        HI_WARN_VDEC("Port %d not exist!\n", hPort);
        return HI_FAILURE;
    }
    else
    {
        BUFMNG_SpinLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
        list_for_each_safe(pos, n, &(pstVpssChan->stPort[j].stBufVpssInst.stVpssBufUnAvailableList))
        {
            pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);

            if (pstTarget->VDECBuf_frm.u32StartPhyAddr == pstImage->stBufAddr[0].u32PhyAddr_Y)
            {
                memcpy(&pstTarget->stVpssOutFrame, pstImage, sizeof(HI_DRV_VIDEO_FRAME_S));
                //pstTarget->bBufUsed = HI_FALSE;
                pstTarget->enFrameBufferState = HI_DRV_VDEC_BUF_STATE_IN_VDEC_FULL;
                pstVpssChan->stPort[j].stBufVpssInst.u32AvaiableFrameCnt++;
                break;
            }

        }
        BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
    return HI_SUCCESS;
}
static HI_S32 VDEC_FindVdecHandleByVpssHandle(VPSS_HANDLE hVpss, HI_HANDLE* phVdec)
{
    HI_U32 i;

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }
    else
    {
        if (HI_INVALID_HANDLE == s_stVdecDrv.astChanEntity[i].stVpssChan.hVdec)
        {
            /*HI_ERR_VDEC("Invalid hVdec!\n");*/
            return HI_FAILURE;
        }

        *phVdec = s_stVdecDrv.astChanEntity[i].stVpssChan.hVdec;
        return HI_SUCCESS;
    }
}

HI_S32 VDEC_FindChannelHandleByInstance(HI_HANDLE hHandle, VDEC_CHANNEL_S** pstChan)
{
    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("null point pstChan\n");
        return HI_FAILURE;
    }

    *pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    return HI_SUCCESS;
}


static HI_S32 VDEC_FindVdecHandleByPortHandle(HI_HANDLE hPort, HI_HANDLE* phVdec)
{
    HI_U32 i, j;

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
        {
            if (hPort == s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].hPort &&
                HI_TRUE == s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].bEnable)
            {
                break;
            }
        }

        if (j < VDEC_MAX_PORT_NUM)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_FAILURE;
    }
    else
    {
        if (HI_INVALID_HANDLE == s_stVdecDrv.astChanEntity[i].stVpssChan.hVdec)
        {
            /*HI_ERR_VDEC("Invalid hVdec!\n");*/
            return HI_FAILURE;
        }

        *phVdec = s_stVdecDrv.astChanEntity[i].stVpssChan.hVdec;
        return HI_SUCCESS;
    }
}

HI_S32 VDEC_FindVpssHandleByVdecHandle(HI_HANDLE hVdec, HI_HANDLE* phVpss)
{
    HI_U32 i;

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVdec == s_stVdecDrv.astChanEntity[i].stVpssChan.hVdec)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Too many chans!\n");
        return HI_FAILURE;
    }
    else
    {
        *phVpss = s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss;
        return HI_SUCCESS;
    }
}

HI_S32 VDEC_FindVdecHandleByESBufferHandle(HI_HANDLE hSteeamBufferHandle, HI_HANDLE* phVdec)
{
    HI_U32 i;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        pstChan = s_stVdecDrv.astChanEntity[i].pstChan;

        if (HI_NULL == pstChan)
        {
            continue;
        }

        if (hSteeamBufferHandle == pstChan->hStrmBuf)
        {
            *phVdec = i;
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
static HI_S32 VDEC_CheckVpssPortOutFrameStatus(HI_HANDLE hVdec, HI_VOID* pstArgs)
{
    HI_S32 i = 0;
    HI_HANDLE hMASTER = HI_INVALID_HANDLE;
    HI_HANDLE hSLAVE = HI_INVALID_HANDLE;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
    HI_DRV_VPSS_PORT_AVAILABLE_S* pstCanGetFrm = HI_NULL;

    pstVpssChan = &(s_stVdecDrv.astChanEntity[hVdec].stVpssChan);
    pstCanGetFrm = (HI_DRV_VPSS_PORT_AVAILABLE_S*)pstArgs;

    for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
    {
        if ((HI_INVALID_HANDLE != pstVpssChan->stPort[i].hPort)
            && (HI_TRUE == pstVpssChan->stPort[i].bEnable))
        {
            if ((HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE == pstVpssChan->stPort[i].bufferType)
                && ((VDEC_PORT_TYPE_MASTER == pstVpssChan->stPort[i].enPortType)
                    || (VDEC_PORT_TYPE_SLAVE == pstVpssChan->stPort[i].enPortType)))
            {
                if (VDEC_PORT_TYPE_MASTER == pstVpssChan->stPort[i].enPortType)
                {
                    hMASTER = pstVpssChan->stPort[i].hPort;
                }

                if (VDEC_PORT_TYPE_SLAVE == pstVpssChan->stPort[i].enPortType)
                {
                    hSLAVE = pstVpssChan->stPort[i].hPort;
                }
            }
        }
    }

    if ((hMASTER != HI_INVALID_HANDLE) || (hSLAVE != HI_INVALID_HANDLE))
    {
        for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
        {
            if ((hMASTER != HI_INVALID_HANDLE) && (hMASTER == pstCanGetFrm[i].hPort) && (pstCanGetFrm[i].bAvailable == HI_FALSE))
            {
                return HI_FAILURE;
            }

            if ((hSLAVE != HI_INVALID_HANDLE) && (hSLAVE == pstCanGetFrm[i].hPort) && (pstCanGetFrm[i].bAvailable == HI_FALSE))
            {
                return HI_FAILURE;
            }
        }
    }

    return HI_SUCCESS;
}
static HI_S32 VDEC_CheckPortFrameListStatus(HI_HANDLE hVdec)
{
    HI_S32 i = 0;
    HI_HANDLE hPort = HI_INVALID_HANDLE;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
    pstVpssChan = &(s_stVdecDrv.astChanEntity[hVdec].stVpssChan);

    /*check list frame status, if the master port and slave port enable then must ensure there exists data in the framelist*/
    for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
    {
        if ((HI_INVALID_HANDLE != pstVpssChan->stPort[i].hPort)
            && (HI_TRUE == pstVpssChan->stPort[i].bEnable))
        {
            if (HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE == pstVpssChan->stPort[i].bufferType)
            {
                if ((HI_TRUE == pstVpssChan->stPort[i].bEnable) &&
                    ((VDEC_PORT_TYPE_MASTER == pstVpssChan->stPort[i].enPortType) ||
                     (VDEC_PORT_TYPE_SLAVE == pstVpssChan->stPort[i].enPortType)))
                {
                    hPort = pstVpssChan->stPort[i].hPort;

                    if (HI_INVALID_HANDLE != hPort)
                    {
                        if ((&(pstVpssChan->stPort[i].stPortList.stVdecPortFrameList)) ==
                            (pstVpssChan->stPort[i].stPortList.stVdecPortFrameList.next))
                        {
                            return HI_FAILURE;
                        }
                    }
                }
                else
                {
                    if ((HI_TRUE == pstVpssChan->stPort[i].bEnable) && (VDEC_PORT_TYPE_VIRTUAL == pstVpssChan->stPort[i].enPortType))
                    {
                        if ((&pstVpssChan->stPort[i].stPortList.stVdecPortFrameList) !=
                            (pstVpssChan->stPort[i].stPortList.stVdecPortFrameList.next))
                        {
                            return HI_SUCCESS;
                        }
                    }
                }
            }
        }
    }

    return HI_SUCCESS;
}
static HI_S32 VDEC_Chan_RecvVdecPortListFrame(HI_HANDLE hVdec, HI_DRV_VIDEO_FRAME_PACKAGE_S* pstFrm)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 i = 0;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
    VDEC_PORT_FRAME_LIST_LOCK_S* pListLock = HI_NULL;
    VDEC_PORT_FRAME_LIST_NODE_S* pstListNode = HI_NULL;

    if ((HI_INVALID_HANDLE == hVdec) || (HI_NULL == pstFrm))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    pstFrm->u32FrmNum = 0;
    s32Ret = VDEC_CheckPortFrameListStatus(hVdec);

    if (HI_SUCCESS == s32Ret)
    {
        pstVpssChan = &(s_stVdecDrv.astChanEntity[hVdec].stVpssChan);

        for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
        {
            pListLock = &pstVpssChan->stPort[i].stPortList.stPortFrameListLock;

            if ((HI_INVALID_HANDLE != pstVpssChan->stPort[i].hPort) && (HI_TRUE == pstVpssChan->stPort[i].bEnable))
            {
                VDEC_SpinLockIRQ(pListLock);

                if ((&pstVpssChan->stPort[i].stPortList.stVdecPortFrameList) != (pstVpssChan->stPort[i].stPortList.stVdecPortFrameList.next))
                {
                    pstListNode = list_entry(pstVpssChan->stPort[i].stPortList.stVdecPortFrameList.next, VDEC_PORT_FRAME_LIST_NODE_S, node);
                    memcpy((void*) & (pstFrm->stFrame[pstFrm->u32FrmNum].stFrameVideo), &(pstListNode->stPortOutFrame), sizeof(HI_DRV_VIDEO_FRAME_S));
                    pstFrm->stFrame[pstFrm->u32FrmNum].hport = pstVpssChan->stPort[i].hPort;
                    pstFrm->u32FrmNum++;
                    list_del_init(&(pstListNode->node));
                    HI_KFREE_BUFMNG(pstListNode);
                    VDEC_SpinUnLockIRQ(pListLock);
                }
                else
                {
                    VDEC_SpinUnLockIRQ(pListLock);
                    continue;
                }
            }
        }
    }
    else
    {
        return HI_FAILURE;
    }

    if (pstFrm->u32FrmNum > 0)
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}
static HI_S32 VDEC_InsertTmpListIntoPortList(HI_HANDLE hVdec, HI_HANDLE hPort)
{
    HI_S32 i = 0;
    struct list_head* pstVdecPortFrameList;
    struct list_head* pnode;
    VDEC_PORT_FRAME_LIST_LOCK_S* pListLock = HI_NULL;
    VDEC_PORT_FRAME_LIST_NODE_S* pstVdecPortFrameListNode = HI_NULL;
    VDEC_PORT_FRAME_LIST_NODE_S* pastFrame[VDEC_MAX_PORT_FRAME] = {0};
    HI_U32 u32pastFrameIndex  = 0;
    pListLock = &(s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[hPort].stPortList.stPortFrameListLock);

    if ((HI_INVALID_HANDLE == hVdec) || (HI_INVALID_HANDLE == hPort))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    VDEC_SpinLockIRQ(pListLock);

    i = s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[hPort].s32PortTmpListPos - 1;

    if (i < 0)
    {
        VDEC_SpinUnLockIRQ(pListLock);
        return HI_FAILURE;
    }

    /*s32LastValidPos = s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[hPort].s32PortTmpListPos;  */
    pstVdecPortFrameList = &(s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[hPort].stPortList.stVdecPortFrameList);

    for (; i >= 0; i--)
    {
        pstVdecPortFrameListNode = HI_KMALLOC_ATOMIC_BUFMNG(sizeof(VDEC_PORT_FRAME_LIST_NODE_S));

        if (HI_NULL == pstVdecPortFrameListNode)
        {
            HI_ERR_VDEC("VDEC_InsertTmpListIntoPortList No memory.\n");

            for (i = 0; i < u32pastFrameIndex; i++)
            {
                if (HI_NULL != pastFrame[i])
                {
                    HI_KFREE_BUFMNG(pastFrame[i]);
                }
            }

            return HI_ERR_BM_NO_MEMORY;
        }
        else
        {
            if (u32pastFrameIndex < VDEC_MAX_PORT_FRAME)
            {
                pastFrame[u32pastFrameIndex] = pstVdecPortFrameListNode;
                u32pastFrameIndex ++;
            }
        }

        memcpy(&(pstVdecPortFrameListNode->stPortOutFrame), &(s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[hPort].astPortTmpList[i].stPortOutFrame), sizeof(HI_DRV_VIDEO_FRAME_S));
        pnode = &(pstVdecPortFrameListNode->node);
        list_add_tail(pnode, pstVdecPortFrameList);
    }

    /*memcpy(&(s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[hPort].astPortTmpList[0]),
           &(s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[hPort].astPortTmpList[s32LastValidPos]),
           sizeof(VDEC_PORT_FRAME_LIST_NODE_S));*/
    s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[hPort].s32PortTmpListPos = 0;

    VDEC_SpinUnLockIRQ(pListLock);

    return HI_SUCCESS;
}

static HI_S32 VDEC_InsertFrameIntoTmpList(HI_HANDLE hVdec, HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    HI_S32 s32Index = 0;
    //VDEC_PORT_FRAME_LIST_NODE_S *pstVdecPortFrameListNode = HI_NULL;
    VDEC_PORT_FRAME_LIST_LOCK_S* pListLock = HI_NULL;

    pListLock = &(s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[hPort].stPortList.stPortFrameListLock);

    VDEC_SpinLockIRQ(pListLock);
    //pstVdecPortFrameListNode = HI_KMALLOC_ATOMIC_BUFMNG(sizeof(VDEC_PORT_FRAME_LIST_NODE_S));
    //if(HI_NULL == pstVdecPortFrameListNode)
    //{
    //    HI_ERR_VDEC("HI_KMALLOC_ATOMIC_BUFMNG err!\n");
    //    return HI_FAILURE;
    //}
    s32Index = s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[hPort].s32PortTmpListPos;

    if (s32Index >= VDEC_MAX_PORT_FRAME)
    {
        HI_ERR_VDEC("Invalid List Index :%d!\n", s32Index);
        VDEC_SpinUnLockIRQ(pListLock);
        return HI_FAILURE;
    }


    memcpy(&(s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[hPort].astPortTmpList[s32Index].stPortOutFrame),
           pstFrame, sizeof(HI_DRV_VIDEO_FRAME_S));

    s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[hPort].s32PortTmpListPos ++;

    VDEC_SpinUnLockIRQ(pListLock);

    return HI_SUCCESS;
}

static HI_S32 VDEC_RecvPortFrameFromVpss(HI_HANDLE hVdec, HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_HANDLE hPortTmp = 0;

    if ((HI_INVALID_HANDLE == hVdec) || (HI_INVALID_HANDLE == hPort))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    hPortTmp = s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[hPort].hPort;

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortFrame)(hPortTmp, pstFrame);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Get Port %d Frame err!\n", hPort);
        return HI_FAILURE;
    }

    return s32Ret;
}


static HI_S32 VDEC_Event_NewImage(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    IMAGE* pstImage;

    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);
    pstImage = (IMAGE*)(pArgs);

    if ((pstImage->disp_height != pstChan->stNormChangeParam.u32ImageHeight)
        || (pstImage->disp_width != pstChan->stNormChangeParam.u32ImageWidth))
    {
        switch (pstImage->format & 0x300)
        {
            case 0x0: /* PROGRESSIVE */
            case 0x200: /* INFERED_PROGRESSIVE */
                pstChan->stNormChangeParam.bProgressive = HI_TRUE;
                break;

            case 0x100: /* INTERLACE */
            case 0x300: /* INFERED_INTERLACE */
            default:
                pstChan->stNormChangeParam.bProgressive = HI_FALSE;
                break;
        }

        if (pstImage->disp_height <= 288)
        {
            pstChan->stNormChangeParam.bProgressive = HI_TRUE;
        }

        pstChan->stNormChangeParam.enNewFormat    = pstChan->enDisplayNorm;
        pstChan->stNormChangeParam.u32ImageWidth  = pstImage->disp_width  & 0xfffffffe;
        pstChan->stNormChangeParam.u32ImageHeight = pstImage->disp_height & 0xfffffffc;
        pstChan->stNormChangeParam.u32FrameRate   = 25000;
        pstChan->bNormChange                      = HI_TRUE;

        if ((pstChan->enCurState == VDEC_CHAN_STATE_RUN)
             && (1 == pstChan->OmxTunnelMode)
             && (pstChan->hOmxChn != HI_INVALID_HANDLE))
        {
            HI_DRV_VDEC_REPORT_TO_OMX((HI_S32)(pstChan->hOmxChn),
                           VDEC_EVT_REPORT_IMG_SIZE_CHG, HI_SUCCESS, pstImage);
        }
    }


    if (pstChan->enCurState == VDEC_CHAN_STATE_RUN)
    {
        if (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32Speed < 0
            || HI_TRUE == s_stVdecDrv.astChanEntity[hHandle].pstChan->bLowdelay)
        {
            (void)(s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)
                (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss,
                HI_DRV_VPSS_USER_COMMAND_IMAGEREADY, NULL);
        }
    }

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_FindIFrame(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    HI_U32 u32IStreamSize = 0;
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);

    pstChan->stStatInfo.u32TotalVdecParseIFrame++;

    if (1 == pstChan->stStatInfo.u32TotalVdecParseIFrame)
    {
        u32IStreamSize = *(HI_U32*)pArgs;
        HI_DRV_STAT_Event(STAT_EVENT_ISTREAMGET, u32IStreamSize);
    }

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_UsrData(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    USRDAT* pstUsrData = HI_NULL;
    HI_U32 u32WriteID;
    HI_U32 u32ReadID;

#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
#if (0 == HI_VDEC_CC_FROM_IMAGE)
    HI_U32 u32ID;
    HI_U8 u8Type;
#endif
#endif

    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);

#ifdef HI_TEE_SUPPORT
    if (pstChan->bTvp)
    {
        HI_ERR_VDEC("TVP not support USERDATA!\n");
        return HI_FAILURE;
    }
    else
#endif
    {
        pstUsrData = (USRDAT*)(pArgs);

#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
#if (0 == HI_VDEC_CC_FROM_IMAGE)

        if (pstUsrData->data_size > 5)
        {
            u32ID = *((HI_U32*)pstUsrData->data);
            u8Type = pstUsrData->data[4];

            if ((VDEC_USERDATA_IDENTIFIER_DVB1 == u32ID) && (VDEC_USERDATA_TYPE_DVB1_CC == u8Type))
            {
#if (1 == VDEC_USERDATA_NEED_ARRANGE)
                USRDATA_Arrange(hHandle, pstUsrData);
#endif
                return HI_SUCCESS;
            }
        }

#endif
#endif

        if (HI_NULL == pstChan->pstUsrData)
        {
            pstChan->pstUsrData = HI_KMALLOC_ATOMIC_VDEC(sizeof(VDEC_USRDATA_PARAM_S));

            if (HI_NULL == pstChan->pstUsrData)
            {
                HI_ERR_VDEC("No memory\n");
                return HI_FAILURE;
            }

            memset(pstChan->pstUsrData, 0, sizeof(VDEC_USRDATA_PARAM_S));
        }

        /* Discard if the buffer of user data full */
        /* CNcomment: 如果用户数据buffer满就直接丢弃 */
        u32WriteID = pstChan->pstUsrData->u32WriteID;
        u32ReadID = pstChan->pstUsrData->u32ReadID;

        if ((u32WriteID + 1) % VDEC_UDC_MAX_NUM == u32ReadID)
        {
            HI_INFO_VDEC("Chan %d drop user data\n", hHandle);
            return HI_FAILURE;
        }

        pstChan->pstUsrData->stAttr[u32WriteID].enBroadcastProfile = HI_UNF_VIDEO_BROADCAST_DVB;
        pstChan->pstUsrData->stAttr[u32WriteID].enPositionInStream = pstUsrData->from;
        pstChan->pstUsrData->stAttr[u32WriteID].u32Pts = (HI_U32)pstUsrData->PTS;
        pstChan->pstUsrData->stAttr[u32WriteID].u32SeqCnt = pstUsrData->seq_cnt;
        pstChan->pstUsrData->stAttr[u32WriteID].u32SeqFrameCnt  = pstUsrData->seq_img_cnt;
        pstChan->pstUsrData->stAttr[u32WriteID].bBufferOverflow = (pstUsrData->data_size > VDEC_KUD_MAX_LEN);
        pstChan->pstUsrData->stAttr[u32WriteID].pu8Buffer = pstChan->pstUsrData->au8Buf[u32WriteID];
        pstChan->pstUsrData->stAttr[u32WriteID].u32Length =
            (pstUsrData->data_size > VDEC_KUD_MAX_LEN) ? MAX_USER_DATA_LEN : pstUsrData->data_size;

        memcpy(pstChan->pstUsrData->stAttr[u32WriteID].pu8Buffer, pstUsrData->data,
               pstChan->pstUsrData->stAttr[u32WriteID].u32Length);

        pstChan->pstUsrData->u32WriteID = (u32WriteID + 1) % VDEC_UDC_MAX_NUM;
        HI_INFO_VDEC("Chan: %d get user data\n", hHandle);
        pstChan->bNewUserData = HI_TRUE;
        return HI_SUCCESS;
    }

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_StreamErr(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);
    pstChan->stStatInfo.u32TotalStreamErrNum++;

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_IFrameErr(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);
    pstChan->bIFrameErr = HI_TRUE;

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_ErrFrame(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);
    pstChan->bErrorFrame = HI_TRUE;

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_LastFrame(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);

    /* *(HI_U32*)pArgs: 0 success, 1 fail,  2 report last frame image id */
    if (1 == *(HI_U32*)pArgs)
    {
        pstChan->u32EndFrmFlag = 1;
    }
    else if (2 <= *(HI_U32*)pArgs)
    {
        pstChan->u32EndFrmFlag = 2;
        pstChan->u32LastFrmId = *(HI_U32*)pArgs - 2;
    }

#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    USRDATA_SetEosFlag(hHandle);
#endif
    pstChan->bEndOfStrm = HI_TRUE;

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_FakeFrame(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);
    pstChan->u32EndFrmFlag = 4;

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_ResolutionChange(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);

    if (s_stVdecDrv.astChanEntity[hHandle].eCallBack)
    {
        s_stVdecDrv.astChanEntity[hHandle].eCallBack(hHandle, NULL, VIDDEC_EVT_STREAM_INFO_CHANGE);
    }

    pstChan->u8ResolutionChange = 1;

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_UnsupportSpec(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    UNSUPPORT_SPEC_E e_SpecType;

    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);
    e_SpecType = ((HI_U32*)pArgs)[0];

    switch (e_SpecType)
    {
        case SPEC_BIT_DEPTH:
            pstChan->u32LastLumaBitdepth   = ((HI_U32*)pArgs)[1];
            pstChan->u32LastChromaBitdepth = ((HI_U32*)pArgs)[1];
            break;

        default:
            break;
    }

    if (s_stVdecDrv.astChanEntity[hHandle].eCallBack)
    { s_stVdecDrv.astChanEntity[hHandle].eCallBack(hHandle, NULL, VIDDEC_EVT_UNSUPPORTED_STREAM_TYPE); }

    pstChan->bUnSupportStream = 1;

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_Unsupport(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);

    if (s_stVdecDrv.astChanEntity[hHandle].eCallBack)
    {
        s_stVdecDrv.astChanEntity[hHandle].eCallBack(hHandle, NULL, VIDDEC_EVT_UNSUPPORTED_STREAM_TYPE);
    }

    pstChan->bUnSupportStream = 1;

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_RatioNotZero(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);

    pstChan->u32ErrRatio = *(HI_U32*)pArgs;
    pstChan->stStatInfo.u32VdecDecErrFrame++;//统计错误帧数

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_VideoStdError(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);

    if (HI_UNF_VCODEC_TYPE_MPEG2 == pstChan->stCurCfg.enType)
    {
        pstChan->stProbeStreamInfo.bProbeCodecTypeChangeFlag = HI_TRUE;
        pstChan->stProbeStreamInfo.enCodecType = HI_UNF_VCODEC_TYPE_H264;
    }

    if (HI_UNF_VCODEC_TYPE_H264 == pstChan->stCurCfg.enType)
    {
        pstChan->stProbeStreamInfo.bProbeCodecTypeChangeFlag = HI_TRUE;
        pstChan->stProbeStreamInfo.enCodecType = HI_UNF_VCODEC_TYPE_MPEG2;
    }

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_ArrangeFrameBuf(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VDEC_SPECIAL_FRM_S stReportFrmRec;
#ifdef VFMW_VPSS_BYPASS_EN
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
#endif

    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);
    memset(&stReportFrmRec, 0, sizeof(VDEC_SPECIAL_FRM_S));

    BUFMNG_SpinLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);

    /*
    // when the "EVNT_ARRANGE_FRAME_BUF" event report. vfmw already make sure that VPSS had receive all the new frames of old resulution,
    // so don't need to set s32ImgOutputEn = false
    // change by l00228308
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)( pstChan->hChan, VDEC_CID_SET_IMG_OUTPUT_INFO, &s32ImgOutputEn);
    if (VDEC_OK != s32Ret)
    {
        HI_INFO_VDEC("Chan %d VDEC_CID_SET_IMG_OUTPUT_INFO (enable = %d) err!\n", hHandle,s32ImgOutputEn);
    }*/
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)( pstChan->hChan, VDEC_CID_REPORT_OCCUQIED_FRM, &stReportFrmRec);
    if (s32Ret != VDEC_OK)
    {
        HI_ERR_VDEC("Chan %d VDEC_CID_REPORT_OCCUQIED_FRM err!\n", pstChan->hChan);
    }
    else
    {
        HI_INFO_VDEC("Chan %d VDEC_CID_REPORT_OCCUQIED_FRM occupied frame = %d!\n", hHandle, stReportFrmRec.specialFrameNum);
        {
            HI_U32 i;
            HI_U32 index = 0;
            VDEC_SPECIAL_FRM_INFO_S stSpecialFrmInfo;

            for (i = 0; i < stReportFrmRec.specialFrameNum ; i++)
            {
                if (HI_SUCCESS == VDEC_FindSpecialFrmToNormalIndex(pstChan, stReportFrmRec.specialFrmRec[i].PhyAddr, &index))
                {
                    stSpecialFrmInfo.bCanRls                 = HI_FALSE;
                    stSpecialFrmInfo.frmBufRec.u32StartPhyAddr = pstChan->stFrameBufferRecord[index].stFrameBuffer.u32StartPhyAddr;
                    stSpecialFrmInfo.frmBufRec.pu8StartVirAddr = pstChan->stFrameBufferRecord[index].stFrameBuffer.pu8StartVirAddr;
                    stSpecialFrmInfo.frmBufRec.u32Size         = pstChan->stFrameBufferRecord[index].stFrameBuffer.u32Size;
                    stSpecialFrmInfo.enbufferNodeStatus      = pstChan->stFrameBufferRecord[index].enFrameBufferNodeStatus;
                    stSpecialFrmInfo.hdrBufRec.u32StartPhyAddr = pstChan->stFrameBufferRecord[index].stHdrBuffer.u32StartPhyAddr;
                    stSpecialFrmInfo.hdrBufRec.pu8StartVirAddr = pstChan->stFrameBufferRecord[index].stHdrBuffer.pu8StartVirAddr;
                    stSpecialFrmInfo.hdrBufRec.u32Size         = pstChan->stFrameBufferRecord[index].stHdrBuffer.u32Size;
                    stSpecialFrmInfo.enhdrNodeStatus           = pstChan->stFrameBufferRecord[index].enHdrBufferNodeStatus;
                    pstChan->stFrameBufferRecord[index].enFrameBufferNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
                    pstChan->stFrameBufferRecord[index].enHdrBufferNodeStatus   = DRV_VDEC_BUFFER_STATUS_BUTT;
#ifdef HI_TEE_SUPPORT

                    if (HI_TRUE == pstChan->bTvp)
                    {
                        stSpecialFrmInfo.bIsSecFrm   = HI_TRUE;
                    }
                    else
                    {
                        stSpecialFrmInfo.bIsSecFrm   = HI_FALSE;
                    }

#endif
                    HI_INFO_VDEC("add list  pstChan->specialFrmRec[%d],u32StartPhyAddr= 0x%x[0x%x],u32StartVirAddr = 0x%x,u32Size = %d\n",
                                 i, stSpecialFrmInfo.frmBufRec.u32StartPhyAddr, stReportFrmRec.specialFrmRec[i].PhyAddr, stSpecialFrmInfo.frmBufRec.pu8StartVirAddr, stSpecialFrmInfo.frmBufRec.u32Size);
                    s32Ret = BUFMNG_VDEC_List_Add(&s_stVdecDrv.stRemainFrmList, &stSpecialFrmInfo);
                }
                else
                {
                    HI_INFO_VDEC("special Frame 0x%x,can't find match single buffer node\n", stReportFrmRec.specialFrmRec[i].PhyAddr);
                }
            }
        }
    }

    BUFMNG_SpinUnLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);

    VDEC_ChanControlStateSpinLockIRQ(&(pstChan->stAllocMemLock));
    pstChan->stVFMWReportFrameInfo.u32ImageNum           = ((HI_U32*)pArgs)[0];
    pstChan->stVFMWReportFrameInfo.u32ImageSize          = ((HI_U32*)pArgs)[1];
    pstChan->stVFMWReportFrameInfo.u32PmvNum             = ((HI_U32*)pArgs)[2];
    pstChan->stVFMWReportFrameInfo.u32PmvSize            = ((HI_U32*)pArgs)[3];
    pstChan->stVFMWReportFrameInfo.u32HdrMetadataSize    = ((HI_U32*)pArgs)[4];
    pstChan->stVFMWReportFrameInfo.u32AllocFrameOnly    = 0;

    pstChan->stVFMWConfiguredBufferNum.u32ImageNum       = 0;
    pstChan->stVFMWConfiguredBufferNum.u32PmvNum         = 0;
#if 0

    //VPSS在对标清的流做去隔行操作时，会占用3帧
    if (((((HI_U32*)pArgs)[5]) <= 1920) && ((((HI_U32*)pArgs)[6]) <= 1088))
    {
        pstChan->stVFMWReportFrameInfo.u32ImageNum += 4;
    }

    u32TotalFrameNum = pstChan->stVFMWReportFrameInfo.u32ImageNum + pstChan->stOption.s32ExtraFrameStoreNum;
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_SET_FRAME_BUFFER_NUM, (HI_VOID*)&u32TotalFrameNum);

    if (VDEC_OK != s32Ret)
    {
        HI_ERR_VDEC("call vfmw command VDEC_CID_SET_FRAME_BUFFER_NUM err!\n");
    }

#endif
    VDEC_ChanControlStateSpinUnlockIRQ(&(pstChan->stAllocMemLock));

#ifdef VFMW_VPSS_BYPASS_EN
    pstVpssChan = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan);
    pstVpssChan->u32InputWidth  = (((HI_U32*)pArgs)[5]);
    pstVpssChan->u32InputHeight = (((HI_U32*)pArgs)[6]);
#endif

    if (NULL != s_stVdecDrv.pVdecTask)
    {
        wake_up_process(s_stVdecDrv.pVdecTask);
    }

    return s32Ret;
}

static HI_S32 VDEC_Event_AllocFrameBufOnly(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);

    VDEC_ChanControlStateSpinLockIRQ(&(pstChan->stAllocMemLock));
    pstChan->stVFMWReportFrameInfo.u32ImageNumOnly = ((HI_U32*)pArgs)[0];
    pstChan->stVFMWReportFrameInfo.u32ImageSizeOnly      = ((HI_U32*)pArgs)[1];
    //pstChan->stVFMWReportFrameInfo.u32PmvNum             = ((HI_U32*)pArgs)[2];
    //pstChan->stVFMWReportFrameInfo.u32PmvSize            = ((HI_U32*)pArgs)[3];
    //pstChan->stVFMWReportFrameInfo.u32HdrMetadataSize    = ((HI_U32*)pArgs)[4];
    pstChan->stVFMWReportFrameInfo.u32AllocFrameOnly    = 1;
    VDEC_ChanControlStateSpinUnlockIRQ(&(pstChan->stAllocMemLock));

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_ReleaseFrameBufOnly(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    HI_U32 u32Index = 0;
    HI_U32 RelsePhyAddr = 0;
    HI_U8* RelseVirAddr = HI_NULL;

    RelsePhyAddr = (HI_U32)((HI_U64*)pArgs)[0];
    RelseVirAddr  = (HI_U8*)(HI_SIZE_T)((HI_U64*)pArgs)[1];

    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);
    VDEC_ChanControlStateSpinLockIRQ(&(pstChan->stAllocMemLock));

    for (u32Index = 0; u32Index < VDEC_MAX_BUFFER_RECORD; u32Index++)
    {
        if (pstChan->stFrameBufferRecord[u32Index].enFrameBufferNodeStatus != DRV_VDEC_BUFFER_STATUS_BUTT &&
            pstChan->stFrameBufferRecord[u32Index].stFrameBuffer.u32StartPhyAddr == RelsePhyAddr &&
            pstChan->stFrameBufferRecord[u32Index].stFrameBuffer.pu8StartVirAddr == RelseVirAddr)
        {
            pstChan->stFrameBufferRecord[u32Index].bWaitRelease = HI_TRUE;    //节点帧存等待释放
            break;
        }
    }

    if (u32Index >= VDEC_MAX_BUFFER_RECORD)
    {
        HI_ERR_VDEC("EVNT_RELSE_ONLY  PhyAddr=%x, VirAddr=%x not exist !!!\n", RelsePhyAddr, RelseVirAddr);
    }

    pstChan->stVFMWReportFrameInfo.u32AllocFrameOnly    = 1;
    VDEC_ChanControlStateSpinUnlockIRQ(&(pstChan->stAllocMemLock));

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_ScdBufRemap(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);

    pstChan->stSCDMMZBuf.pu8StartVirAddr = (HI_U8*)(HI_SIZE_T)(((HI_U64*)pArgs)[0]);
    pstChan->stEOSBuffer.pu8StartVirAddr = pstChan->stSCDMMZBuf.pu8StartVirAddr
                        + pstChan->stSCDMMZBuf.u32Size - HI_VDEC_EOS_MEM_SIZE;

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_VdhBufRemap(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);

    pstChan->stVDHMMZBuf.pu8StartVirAddr = (HI_U8*)(HI_SIZE_T)(((HI_U64*)pArgs)[0]);

    return HI_SUCCESS;
}

static HI_S32 VDEC_Event_MeetNewFrame(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, HI_VOID* pArgs)
{
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);

    pstChan->stStatInfo.u32TotalVfmwOutFrame++;

    return HI_SUCCESS;
}


static const VDECEVENT_COMMAND_NODE g_EVENT_CommandTable[] =
{
    {EVNT_NEW_IMAGE,                        VDEC_Event_NewImage},
    {EVNT_FIND_IFRAME,                      VDEC_Event_FindIFrame},
    {EVNT_USRDAT,                           VDEC_Event_UsrData},
    {EVNT_VDM_ERR,                          VDEC_Event_StreamErr},
    {EVNT_SE_ERR,                           VDEC_Event_StreamErr},
    {EVNT_IFRAME_ERR,                       VDEC_Event_IFrameErr},
    {EVNT_ERR_FRAME,                        VDEC_Event_ErrFrame},
    {EVNT_LAST_FRAME,                       VDEC_Event_LastFrame},
    {EVNT_FAKE_FRAME,                       VDEC_Event_FakeFrame},
    {EVNT_RESOLUTION_CHANGE,                VDEC_Event_ResolutionChange},
    {EVNT_UNSUPPORT_SPEC,                   VDEC_Event_UnsupportSpec},
    {EVNT_UNSUPPORT,                        VDEC_Event_Unsupport},
    {EVNT_RATIO_NOTZERO,                    VDEC_Event_RatioNotZero},
    {EVNT_VIDSTD_ERROR,                     VDEC_Event_VideoStdError},
    {EVNT_ARRANGE_FRAME_BUF,                VDEC_Event_ArrangeFrameBuf},
    {EVNT_ALLOC_FRAME_BUF_ONLY,             VDEC_Event_AllocFrameBufOnly},
    {EVNT_RELSE_FRAME_BUF_ONLY,             VDEC_Event_ReleaseFrameBufOnly},
    {EVNT_SCD_BUFFER_REMAP,                 VDEC_Event_ScdBufRemap},
    {EVNT_VDH_BUFFER_REMAP,                 VDEC_Event_VdhBufRemap},
    {EVNT_MEET_NEWFRM,                      VDEC_Event_MeetNewFrame},

    {EVNT_BUTT,        HI_NULL}, //terminal element
};

FN_VDEC_EVENT_HANDLER VDEC_Event_Get_Handler(HI_U32 Code)
{
    HI_U32 Index = 0;
    FN_VDEC_EVENT_HANDLER pTargetHandler = HI_NULL;

    while(1)
    {
        if (g_EVENT_CommandTable[Index].Code == EVNT_BUTT || g_EVENT_CommandTable[Index].pHandler == HI_NULL)
        {
            break;
        }

        if (Code == g_EVENT_CommandTable[Index].Code)
        {
            pTargetHandler = g_EVENT_CommandTable[Index].pHandler;
            break;
        }

        Index++;
    }

    return pTargetHandler;
}

static HI_S32 VDEC_EventHandle(HI_S32 s32ChanID, HI_S32 s32EventType, HI_VOID* pArgs)
{
    HI_S32 s32Ret;
    HI_HANDLE hHandle;
    FN_VDEC_EVENT_HANDLER pEvent_Handler = HI_NULL;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    /* Find channel number */
    for (hHandle = 0; hHandle < HI_VDEC_MAX_INSTANCE_NEW; hHandle++)
    {
        if (s_stVdecDrv.astChanEntity[hHandle].pstChan)
        {
            if (s_stVdecDrv.astChanEntity[hHandle].pstChan->hChan == s32ChanID)
            {
                break;
            }
        }
    }

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("bad handle %d!\n", hHandle);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);
    if (s32Ret != HI_SUCCESS)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    pEvent_Handler = VDEC_Event_Get_Handler(s32EventType);
    if (pEvent_Handler == HI_NULL)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return HI_FAILURE;
    }

    s32Ret = pEvent_Handler(pstChan, hHandle, pArgs);
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    return s32Ret;
}

/**************************************************************/
/**************快退时VDEC从VPSS收到的帧做倒序处理**************/
/**************************************************************/
HI_S32 FastbackwardCheckForNewIFrame(HI_VDEC_PRIV_FRAMEINFO_S* pstPrivInfo, HI_HANDLE hHandle)
{
    /*if get a new I frame then insert the tmpFrames to portFrameList from tail to head*/
    if ((HI_FALSE != s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32GetFirstVpssFrameFlag)
        && (((0 == pstPrivInfo->s32FrameFormat) && (0 == pstPrivInfo->s32FieldFlag))
           ||(((0 == pstPrivInfo->s32BottomFieldFrameFormat) || (0 == pstPrivInfo->s32TopFieldFrameFormat))
              && (1 == pstPrivInfo->s32FieldFlag))
           )
       )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

HI_S32 FastbackwardCheckForNewGop(HI_VDEC_PRIV_FRAMEINFO_S* pstPrivInfo, HI_HANDLE hHandle, HI_S32 i)
{
    /*if get a new Gop_Num then insert the tmpFrames to portFrameList from tail to head*/
    if ((HI_TRUE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32GetFirstVpssFrameFlag)
        && ((pstPrivInfo->s32GopNum) != (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32PortLastFrameGopNum))
        &&(0 < s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32PortTmpListPos)
        )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


static HI_VOID VDEC_FastbackwardHandle(HI_HANDLE hVdec, HI_HANDLE hHandle, VDEC_VPSSCHANNEL_S* pstVpssChan, HI_VOID* pstArgs)
{
    HI_S32 i;
    HI_S32 s32Ret;
    HI_S32 s32VpssHaveFrame = 0;
    HI_DRV_VIDEO_FRAME_S *pstFrame = HI_NULL;
    HI_VDEC_PRIV_FRAMEINFO_S* pstPrivInfo = HI_NULL;

    pstFrame = HI_VMALLOC_BUFMNG(sizeof(*pstFrame));
    if (HI_NULL == pstFrame)
    {
       HI_ERR_VDEC("VDEC_FastbackwardHandle Alloc Mem failed!\n");
       return;
    }

    s32VpssHaveFrame = VDEC_CheckVpssPortOutFrameStatus(hVdec, pstArgs);
    for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
    {
        if (HI_SUCCESS == s32VpssHaveFrame)
        {
            if (HI_INVALID_HANDLE != s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].hPort
                && (HI_TRUE == pstVpssChan->stPort[i].bEnable))
            {
                memset(pstFrame, 0, sizeof(*pstFrame));
                s32Ret = VDEC_RecvPortFrameFromVpss(hVdec, i, pstFrame);

                if (HI_SUCCESS == s32Ret)
                {
                    if (HI_TRUE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stControlInfo.u32DispOptimizeFlag)
                    {
                        if (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].u32LastFrameIndex == pstFrame->u32FrameIndex)
                        {
                            s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssRelPortFrame)(s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].hPort, pstFrame);

                            if (HI_SUCCESS != s32Ret)
                            {
                                HI_ERR_VDEC("vdec RelPortFrame err!\n");
                            }

                            continue;
                        }
                        else
                        {
                            pstFrame->u32FrameRate /= 2;
                        }
                    }

                    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].u32LastFrameIndex = pstFrame->u32FrameIndex;
                    pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S*)(((HI_DRV_VIDEO_PRIVATE_S*)(pstFrame->u32Priv))->u32Reserve);
                    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32RecvNewFrame = 1;
                }
                else
                {
                    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32RecvNewFrame = 0;
                    continue;
                }
            }
            else
            {
                continue;
            }
        }
        else
        {
            continue;
        }

        if (HI_FALSE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stControlInfo.u32BackwardOptimizeFlag)
        {
            /*if get a new I frame then insert the tmpFrames to portFrameList from tail to head*/
            if (FastbackwardCheckForNewIFrame(pstPrivInfo, hHandle))
            {
                VDEC_InsertTmpListIntoPortList(hHandle, i);
            }
            else
            {
                if (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32RecvNewFrame)
                {
                    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32GetFirstVpssFrameFlag = HI_TRUE;
                    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.eLastFrameFormat = pstPrivInfo->s32FrameFormat;
                }
            }
        }
        else
        {
            hHandle &= 0x000000ff;

            /*if get a new Gop_Num then insert the tmpFrames to portFrameList from tail to head*/
            if (FastbackwardCheckForNewGop(pstPrivInfo, hHandle, i))
            {
                VDEC_InsertTmpListIntoPortList(hHandle, i);
            }

            /*insert the new frame into tmpFrames*/
            //else
            {
                if (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32RecvNewFrame)
                {
                    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32GetFirstVpssFrameFlag = HI_TRUE;
                    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32PortLastFrameGopNum = pstPrivInfo->s32GopNum;
                }

            }
        }

        if (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32RecvNewFrame)
        {
            VDEC_InsertFrameIntoTmpList(hHandle, i, pstFrame);
            s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32RecvNewFrame = 0;
        }
    }
    HI_VFREE_BUFMNG(pstFrame);

    return;
}

static HI_S32 VpssEventCheckBuffer(VDEC_VPSSCHANNEL_S *pstVpssChan,
                                   HI_HANDLE hVdec,
                                   HI_HANDLE hHandle,
                                   HI_VOID* pstArgs)
{
    HI_S32 s32Ret;
    HI_HANDLE hPort = HI_INVALID_HANDLE;
    HI_HANDLE hVpss = HI_INVALID_HANDLE;

    VDEC_ASSERT_RETURN(pstArgs != HI_NULL, HI_FAILURE);

    hPort = ((HI_DRV_VPSS_BUFINFO_S*)pstArgs)->hPort;
    hVpss = (HI_HANDLE)PORTHANDLE_TO_VPSSID(hPort);
    s32Ret = BUFMNG_VPSS_CheckAvaibleBuffer(hVpss, hPort);

    if (HI_SUCCESS == s32Ret)
    {
        ((HI_DRV_VPSS_BUFINFO_S*)pstArgs)->bAvailable = HI_TRUE;
    }
    else
    {
        ((HI_DRV_VPSS_BUFINFO_S*)pstArgs)->bAvailable = HI_FALSE;
    }

    return HI_SUCCESS;
}

static HI_S32 VpssEventGetFrameBuffer(VDEC_VPSSCHANNEL_S *pstVpssChan,
                                      HI_HANDLE hVdec,
                                      HI_HANDLE hHandle,
                                      HI_VOID* pstArgs)
{
    HI_S32 s32Ret;
    HI_HANDLE hPort = HI_INVALID_HANDLE;
    HI_HANDLE hVpss = HI_INVALID_HANDLE;
    BUF_VPSS_S stMMZ_Buffer;
    HI_U32 u32Stride = 0;

    VDEC_ASSERT_RETURN(pstArgs != HI_NULL, HI_FAILURE);

    hPort = ((HI_DRV_VPSS_FRMBUF_S*)pstArgs)->hPort;
    hVpss = (HI_HANDLE)PORTHANDLE_TO_VPSSID(hPort);
    memset(&stMMZ_Buffer, 0, sizeof(BUF_VPSS_S));
    stMMZ_Buffer.VDECBuf_frm.u32Size = ((HI_DRV_VPSS_FRMBUF_S*)pstArgs)->u32Size;

    s32Ret = BUFMNG_VPSS_RecBuffer(hVpss, hPort, &stMMZ_Buffer, &u32Stride);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d BUFMNG_VPSS_RecBuffer fail!\n", hHandle);
        return HI_FAILURE;
    }

    ((HI_DRV_VPSS_FRMBUF_S*)pstArgs)->u32StartPhyAddr = stMMZ_Buffer.VDECBuf_frm.u32StartPhyAddr;  //c00277632
    ((HI_DRV_VPSS_FRMBUF_S*)pstArgs)->pu8StartVirAddr = stMMZ_Buffer.VDECBuf_frm.pu8StartVirAddr;
    ((HI_DRV_VPSS_FRMBUF_S*)pstArgs)->u32PrivDataPhyAddr = stMMZ_Buffer.VDECBuf_meta.u32StartPhyAddr;  //c00277632
    ((HI_DRV_VPSS_FRMBUF_S*)pstArgs)->pu8PrivDataVirAddr = stMMZ_Buffer.VDECBuf_meta.pu8StartVirAddr;
    ((HI_DRV_VPSS_FRMBUF_S*)pstArgs)->u32Stride       = u32Stride;

    return HI_SUCCESS;
}

static HI_S32 VpssEventReleaseFrameBuffer(VDEC_VPSSCHANNEL_S *pstVpssChan,
                                          HI_HANDLE hVdec,
                                          HI_HANDLE hHandle,
                                          HI_VOID* pstArgs)
{
    HI_HANDLE hPort = HI_INVALID_HANDLE;
    HI_HANDLE hVpss = HI_INVALID_HANDLE;
    HI_DRV_VIDEO_FRAME_S *pstFrame = HI_NULL;

    VDEC_ASSERT_RETURN(pstArgs != HI_NULL, HI_FAILURE);

    if (HI_NULL != (pstFrame = HI_VMALLOC_BUFMNG(sizeof(HI_DRV_VIDEO_FRAME_S))))
    {
        hPort = ((HI_DRV_VPSS_FRMBUF_S*)pstArgs)->hPort;
        hVpss = (HI_HANDLE)PORTHANDLE_TO_VPSSID(hPort);
        pstFrame->stBufAddr[0].u32PhyAddr_Y = ((HI_DRV_VPSS_FRMBUF_S*)pstArgs)->u32StartPhyAddr;
        BUFMNG_VPSS_RelBuffer(hVpss, hPort, pstFrame);
        HI_VFREE_BUFMNG(pstFrame);
    }

    return HI_SUCCESS;
}

static HI_S32 VpssEventNewFrame(VDEC_VPSSCHANNEL_S *pstVpssChan,
                                HI_HANDLE hVdec,
                                HI_HANDLE hHandle,
                                HI_VOID* pstArgs)
{
    HI_S32 s32Ret;
    HI_HANDLE hPort = HI_INVALID_HANDLE;
    HI_HANDLE hVpss = HI_INVALID_HANDLE;
    HI_S32 i = 0;

    //VDEC_ASSERT_RETURN(pstArgs != HI_NULL, HI_FAILURE);

    s_stVdecDrv.astChanEntity[hHandle].pstChan->bNewFrame = HI_TRUE;

    if (HI_DRV_VDEC_BUF_USER_ALLOC_MANAGE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.enFrameBuffer
      ||HI_DRV_VDEC_BUF_VDEC_ALLOC_MANAGE ==  s_stVdecDrv.astChanEntity[hHandle].stVpssChan.enFrameBuffer)
    {
        if (pstArgs == HI_NULL)
        {
            HI_ERR_VDEC("%s %d args err!\n", __func__, __LINE__);
            return HI_FAILURE;
        }

        hPort = ((HI_DRV_VPSS_FRMINFO_S*)pstArgs)->hPort;

        for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
        {
            if (hPort == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].hPort)
            {
                break;
            }
        }

        if (i >= VDEC_MAX_PORT_NUM)
        {
            HI_ERR_VDEC("====ERR port handle\n");
            return HI_FAILURE;
        }

        if (HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].bufferType)
        {
            hPort = ((HI_DRV_VPSS_FRMINFO_S*)pstArgs)->hPort;
            hVpss = (HI_HANDLE)PORTHANDLE_TO_VPSSID(hPort);
            s32Ret = VDEC_VpssNewImageEvent(hVpss, hPort, &((HI_DRV_VPSS_FRMINFO_S*)pstArgs)->stFrame);
            if (s32Ret != HI_SUCCESS)
            {
                HI_WARN_VDEC("VDEC_VpssNewImageEvent err :%d \n", s32Ret);
            }
        }
    }

    return HI_SUCCESS;
}

static HI_S32 VpssEventTaskComplete(VDEC_VPSSCHANNEL_S *pstVpssChan,
                                    HI_HANDLE hVdec,
                                    HI_HANDLE hHandle,
                                    HI_VOID* pstArgs)
{
    HI_S32 i = 0;
    HI_HANDLE hMaster = HI_INVALID_HANDLE;
    HI_HANDLE hSlave = HI_INVALID_HANDLE;
    HI_HANDLE hVirtual = HI_INVALID_HANDLE;
     HI_DRV_VPSS_PORT_AVAILABLE_S* pstCanGetFrm = HI_NULL;

    VDEC_ASSERT_RETURN(pstArgs != HI_NULL, HI_FAILURE);

    pstCanGetFrm = (HI_DRV_VPSS_PORT_AVAILABLE_S*)pstArgs;

    if (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32Speed < 0)
    {
        VDEC_FastbackwardHandle(hVdec, hHandle, pstVpssChan, pstArgs);
    }

    for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
    {
        if (   (HI_TRUE == pstVpssChan->stPort[i].bEnable)
               && (HI_INVALID_HANDLE != pstVpssChan->stPort[i].hPort)
               && (HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE == pstVpssChan->stPort[i].bufferType) )
        {
            if (VDEC_PORT_TYPE_MASTER == pstVpssChan->stPort[i].enPortType)
            {
                hMaster =  pstVpssChan->stPort[i].hPort;
            }
            else if (VDEC_PORT_TYPE_SLAVE == pstVpssChan->stPort[i].enPortType)
            {
                hSlave = pstVpssChan->stPort[i].hPort;
            }
            else if (VDEC_PORT_TYPE_VIRTUAL == pstVpssChan->stPort[i].enPortType)
            {
                hVirtual = pstVpssChan->stPort[i].hPort;
            }
        }
    }

    for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
    {
        if ( (hMaster == pstCanGetFrm[i].hPort) && (HI_TRUE == pstCanGetFrm[i].bAvailable) )
        {
            s_stVdecDrv.astChanEntity[hHandle].pstChan->bMasterWakeUp = HI_TRUE;
            VDEC_GiveEvent(s_stVdecDrv.astChanEntity[hHandle].pstChan->WaitQueue);
        }
        else if (hSlave == pstCanGetFrm[i].hPort && (HI_TRUE == pstCanGetFrm[i].bAvailable) )
        {
            s_stVdecDrv.astChanEntity[hHandle].pstChan->bSlaveWakeUp = HI_TRUE;
            VDEC_GiveEvent(s_stVdecDrv.astChanEntity[hHandle].pstChan->WaitQueue);
        }
        else if (hVirtual == pstCanGetFrm[i].hPort && (HI_TRUE == pstCanGetFrm[i].bAvailable) )
        {
            s_stVdecDrv.astChanEntity[hHandle].pstChan->bVirtualWakeUp = HI_TRUE;
            VDEC_GiveEvent(s_stVdecDrv.astChanEntity[hHandle].pstChan->WaitQueue);
        }
    }

    return HI_SUCCESS;
}

static HI_S32 VpssEventBufferListFull(VDEC_VPSSCHANNEL_S *pstVpssChan,
                                      HI_HANDLE hVdec,
                                      HI_HANDLE hHandle,
                                      HI_VOID* pstArgs)
{
    HI_S32 s32Ret;
    HI_HANDLE hPort = HI_INVALID_HANDLE;
    HI_S32 i = 0;
    HI_DRV_VPSS_PORT_CFG_S stVpssPortCfg = {0};
    HI_DRV_VPSS_PORT_BUFLIST_STATE_S stVpssBufListState = {0};

    VDEC_ASSERT_RETURN(pstArgs != HI_NULL, HI_FAILURE);

    for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
    {
        if (VDEC_PORT_TYPE_MASTER == pstVpssChan->stPort[i].enPortType
            || VDEC_PORT_TYPE_SLAVE == pstVpssChan->stPort[i].enPortType)
        {

            s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortCfg)(pstVpssChan->stPort[i].hPort, &stVpssPortCfg);

            if (HI_SUCCESS == s32Ret)
            {
                if (HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == stVpssPortCfg.stBufListCfg.eBufType)
                {
                    *(HI_DRV_VPSS_BUFFUL_STRATAGY_E*)pstArgs = HI_DRV_VPSS_BUFFUL_PAUSE;
                    break;
                }
                else
                {
                    hPort = pstVpssChan->stPort[i].hPort;
                    memset(&stVpssBufListState, 0, sizeof(HI_DRV_VPSS_PORT_BUFLIST_STATE_S));
                    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortBufListState)(hPort, &stVpssBufListState);

                    if (HI_SUCCESS == s32Ret)
                    {
                        /*master_port slave_port full:HI_DRV_VPSS_BUFFUL_PAUSE,master_port slave_port not full:HI_DRV_VPSS_BUFFUL_KEEPWORKING*/
                        if (stVpssBufListState.u32FulBufNumber > stVpssBufListState.u32TotalBufNumber - 2)
                        {
                            /*master_port slave_port full:HI_DRV_VPSS_BUFFUL_PAUSE*/
                            *(HI_DRV_VPSS_BUFFUL_STRATAGY_E*)pstArgs = HI_DRV_VPSS_BUFFUL_PAUSE;
                            break;
                        }
                        else
                        {
                            /*master_port slave_port not full:HI_DRV_VPSS_BUFFUL_KEEPWORKING*/
                            *(HI_DRV_VPSS_BUFFUL_STRATAGY_E*)pstArgs = HI_DRV_VPSS_BUFFUL_KEEPWORKING;
                        }
                    }
                }
            }
        }
        else
        {
            //*(HI_DRV_VPSS_BUFFUL_STRATAGY_E *)pstArgs = HI_DRV_VPSS_BUFFUL_KEEPWORKING;
        }
    }

    if (i >= VDEC_MAX_PORT_NUM)
    {
        HI_WARN_VDEC("Port not exist!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}



static const VPSSEVENT_COMMAND_NODE g_VpssEvent_CommandTable[] =
{
    {VPSS_EVENT_CHECK_FRAMEBUFFER,            VpssEventCheckBuffer},
    {VPSS_EVENT_GET_FRMBUFFER,                VpssEventGetFrameBuffer},
    {VPSS_EVENT_REL_FRMBUFFER,                VpssEventReleaseFrameBuffer},
    {VPSS_EVENT_NEW_FRAME,                    VpssEventNewFrame},
    {VPSS_EVENT_TASK_COMPLETE,                VpssEventTaskComplete},
    {VPSS_EVENT_BUFLIST_FULL,                 VpssEventBufferListFull},

    {VPSS_EVENT_BUTT,        HI_NULL}, //terminal element
};

FN_VPSSEVENT_HANDLER VpssEvent_Get_Handler(HI_U32 Code)
{
    HI_U32 Index = 0;
    FN_VPSSEVENT_HANDLER pTargetHandler = HI_NULL;

    while(1)
    {
        if (g_VpssEvent_CommandTable[Index].Code == VPSS_EVENT_BUTT || g_VpssEvent_CommandTable[Index].pHandler == HI_NULL)
        {
            break;
        }

        if (Code == g_VpssEvent_CommandTable[Index].Code)
        {
            pTargetHandler = g_VpssEvent_CommandTable[Index].pHandler;
            break;
        }

        Index++;
    }

    return pTargetHandler;
}

//add by l00225186
/*vpss 回调函数*/
static HI_S32 VDEC_VpssEventHandle(HI_HANDLE hVdec, HI_DRV_VPSS_EVENT_E enEventID, HI_VOID* pstArgs)
{
    HI_S32 s32Ret;
    HI_HANDLE hHandle;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
    FN_VPSSEVENT_HANDLER pEvent_Handler = HI_NULL;

    /* Find channel number */
    for (hHandle = 0; hHandle < HI_VDEC_MAX_INSTANCE_NEW - 1; hHandle++)
    {
        if (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVdec == hVdec)
        {
            break;
        }
    }

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("bad handle %d!\n", hHandle);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan);

    pEvent_Handler = VpssEvent_Get_Handler(enEventID);
    if (pEvent_Handler == HI_NULL)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return HI_FAILURE;
    }

    s32Ret = pEvent_Handler(pstVpssChan, hVdec, hHandle, pstArgs);

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    return s32Ret;
}

static VID_STD_E VDEC_CodecTypeUnfToFmw(HI_UNF_VCODEC_TYPE_E unfType)
{
    switch (unfType)
    {
        case HI_UNF_VCODEC_TYPE_MPEG2:
            return STD_MPEG2;

        case HI_UNF_VCODEC_TYPE_MPEG4:
            return STD_MPEG4;

        case HI_UNF_VCODEC_TYPE_AVS:
            return STD_AVS;

        case HI_UNF_VCODEC_TYPE_H263:
            return STD_H263;

        case HI_UNF_VCODEC_TYPE_VP6:
            return STD_VP6;

        case HI_UNF_VCODEC_TYPE_VP6F:
            return STD_VP6F;

        case HI_UNF_VCODEC_TYPE_VP6A:
            return STD_VP6A;

        case HI_UNF_VCODEC_TYPE_VP8:
            return STD_VP8;

        case HI_UNF_VCODEC_TYPE_SORENSON:
            return STD_SORENSON;

        case HI_UNF_VCODEC_TYPE_H264:
            return STD_H264;

        case HI_UNF_VCODEC_TYPE_HEVC:
            return STD_HEVC;

        case HI_UNF_VCODEC_TYPE_REAL9:
            return STD_REAL9;

        case HI_UNF_VCODEC_TYPE_REAL8:
            return STD_REAL8;

        case HI_UNF_VCODEC_TYPE_VC1:
            return STD_VC1;

        case HI_UNF_VCODEC_TYPE_DIVX3:
            return STD_DIVX3;

        case HI_UNF_VCODEC_TYPE_MVC:
            return STD_MVC;

        case HI_UNF_VCODEC_TYPE_RAW:
            return STD_RAW;

        case HI_UNF_VCODEC_TYPE_MJPEG:
            return STD_USER;
        case HI_UNF_VCODEC_TYPE_VP9:
            return STD_VP9;

        default:
            return STD_END_RESERVED;
    }
}

HI_VOID GetMaxResoutionFor256M(HI_S32* pMaxWidth, HI_S32* pMaxHeight)
{
    if (HI_VIDEO_PRE_CAP_1080P_IN_256 == 1)
    {
        *pMaxWidth = 1920;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_MVC_IN_256 == 1)
    {
        *pMaxWidth = 1920;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_2160P_IN_256 == 1)
    {
        *pMaxWidth = 3840;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_4K_IN_256 == 1)
    {
        *pMaxWidth = 4096;
        *pMaxHeight = 2304;
    }
    else
    {
        *pMaxWidth = 4096;
        *pMaxHeight = 2304;
    }

    return;
}

HI_VOID GetMaxResoutionFor512M(HI_S32* pMaxWidth, HI_S32* pMaxHeight)
{
    if (HI_VIDEO_PRE_CAP_1080P_IN_512 == 1)
    {
        *pMaxWidth = 1920;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_MVC_IN_512 == 1)
    {
        *pMaxWidth = 1920;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_2160P_IN_512 == 1)
    {
        *pMaxWidth = 3840;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_4K_IN_512 == 1)
    {
        *pMaxWidth = 4096;
        *pMaxHeight = 2304;
    }
    else
    {
        *pMaxWidth = 4096;
        *pMaxHeight = 2304;
    }

    return;
}

HI_VOID GetMaxResoutionFor768M(HI_S32* pMaxWidth, HI_S32* pMaxHeight)
{
    if (HI_VIDEO_PRE_CAP_1080P_IN_768 == 1)
    {
        *pMaxWidth = 1920;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_MVC_IN_768 == 1)
    {
        *pMaxWidth = 1920;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_2160P_IN_768 == 1)
    {
        *pMaxWidth = 3840;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_4K_IN_768 == 1)
    {
        *pMaxWidth = 4096;
        *pMaxHeight = 2304;
    }
    else
    {
        *pMaxWidth = 4096;
        *pMaxHeight = 2304;
    }

    return;
}

HI_VOID GetMaxResoutionFor1024M(HI_S32* pMaxWidth, HI_S32* pMaxHeight)
{
    if (HI_VIDEO_PRE_CAP_1080P_IN_1024 == 1)
    {
        *pMaxWidth = 1920;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_MVC_IN_1024 == 1)
    {
        *pMaxWidth = 1920;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_2160P_IN_1024 == 1)
    {
        *pMaxWidth = 3840;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_4K_IN_1024 == 1)
    {
        *pMaxWidth = 4096;
        *pMaxHeight = 2304;
    }
    else
    {
        *pMaxWidth = 4096;
        *pMaxHeight = 2304;
    }

    return;
}

HI_VOID GetMaxResoutionFor2048M(HI_S32* pMaxWidth, HI_S32* pMaxHeight)
{
    if (HI_VIDEO_PRE_CAP_1080P_IN_2048 == 1)
    {
        *pMaxWidth = 1920;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_MVC_IN_2048 == 1)
    {
        *pMaxWidth = 1920;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_2160P_IN_2048 == 1)
    {
        *pMaxWidth = 3840;
        *pMaxHeight = 1088;
    }
    else if (HI_VIDEO_PRE_CAP_4K_IN_2048 == 1)
    {
        *pMaxWidth = 4096;
        *pMaxHeight = 2304;
    }
    else
    {
        *pMaxWidth = 4096;
        *pMaxHeight = 2304;
    }

    return;
}

HI_VOID VDEC_Chan_GetMaxResoution(HI_S32* pMaxWidth, HI_S32* pMaxHeight)
{
    HI_S32                  s32Ret;
    HI_SYS_MEM_CONFIG_S     MemConfig = {0};
    s32Ret = HI_DRV_SYS_GetMemConfig(&MemConfig);

    if (HI_SUCCESS != s32Ret)
    {
        MemConfig.u32TotalSize = 1024; // if get memory config failed, we use 1G config
    }

    //set the chan max resouion
    switch (MemConfig.u32TotalSize)
    {
        case 256:
        {
            GetMaxResoutionFor256M(pMaxWidth, pMaxHeight);

            break;
        }

        case 512:
        {
            GetMaxResoutionFor512M(pMaxWidth, pMaxHeight);

            break;
        }

        case 768:
        {
            GetMaxResoutionFor768M(pMaxWidth, pMaxHeight);

            break;
        }

        case 2048:
        {
            GetMaxResoutionFor2048M(pMaxWidth, pMaxHeight);

            break;
        }

        case 1024:
        default:
        {
            GetMaxResoutionFor1024M(pMaxWidth, pMaxHeight);

            break;
        }
    }

    return;

}

static HI_VOID GetDecMode(HI_UNF_VCODEC_MODE_E enMode, VDEC_CHAN_CFG_S *pstVdecChanCfg)
{
    switch (enMode)
    {
        case HI_UNF_VCODEC_MODE_NORMAL:
            pstVdecChanCfg->s32DecMode = IPB_MODE;
            break;

        case HI_UNF_VCODEC_MODE_IP:
            pstVdecChanCfg->s32DecMode = IP_MODE;
            break;

        case HI_UNF_VCODEC_MODE_I:
            pstVdecChanCfg->s32DecMode = I_MODE;
            break;

        case HI_UNF_VCODEC_MODE_DROP_INVALID_B:
            pstVdecChanCfg->s32DecMode = DISCARD_B_BF_P_MODE;
            break;

        case HI_UNF_VCODEC_MODE_BUTT:
        default:
            pstVdecChanCfg->s32DecMode = IPB_MODE;
            break;
    }

    return;
}

static HI_VOID GetCMPInfo(VDEC_CHANNEL_S *pstChan, HI_CHIP_TYPE_E enChipType, HI_CHIP_VERSION_E enChipVersion)
{
    if (((HI_CHIP_TYPE_HI3719M   == enChipType) && (HI_CHIP_VERSION_V100 == enChipVersion))
        || ((HI_CHIP_TYPE_HI3718M   == enChipType) && (HI_CHIP_VERSION_V100 == enChipVersion)) )
    {
        if (pstChan->stOption.s32MaxWidth > 1920 || pstChan->stOption.s32MaxHeight > 1088)
        {
            EnVcmp  = 0;
        }
        else
        {
            EnVcmp  = 1;
        }
    }
    else
    {

        EnVcmp  = 1;
    }

    /*根据MaskCtrlWord的第3比特位决定是否关闭压缩功能*/
    if (MaskCtrlWord & 0x4)
    {
        EnVcmp = 0;
    }

}

#ifdef HI_HDR_SUPPORT
static HI_VOID SetHDRAttr(VDEC_CHANNEL_S *pstChan, VDEC_CHAN_CFG_S *pstVdecChanCfg)
{
/* set hdr info*/
   if (pstChan->stVdecHdrAttr.enVdecHDRType != HI_VDEC_HDR_CHAN_TYPE_BUTT)
   {
       pstVdecChanCfg->s8BeHDRChan = 1;
       if (HI_VDEC_HDR_CHAN_TYPE_BL == pstChan->stVdecHdrAttr.enVdecHDRType)
       {
           pstVdecChanCfg->dv_codec_capacity = DV_BL_DECODING_SUPPORT;
       }
       else if (HI_VDEC_HDR_CHAN_TYPE_EL == pstChan->stVdecHdrAttr.enVdecHDRType)
       {
           pstVdecChanCfg->dv_codec_capacity = DV_EL_DECODING_SUPPORT;
       }
       else
       {
           HI_ERR_VDEC("hdr info error!");
       }
       //赋值单流，双流信息
       if (HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_SINGLE_VES == pstChan->stVdecHdrAttr.AvplayHDRAttr.enHDRStreamType || HI_UNF_AVPLAY_HDR_STREAM_TYPE_SL_VES == pstChan->stVdecHdrAttr.AvplayHDRAttr.enHDRStreamType)
       {
           pstVdecChanCfg->ves_imp = DV_SINGLE_VES_IMP;
       }
       else if (HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_DUAL_VES == pstChan->stVdecHdrAttr.AvplayHDRAttr.enHDRStreamType)
       {
           pstVdecChanCfg->ves_imp = DV_DUAL_VES_IMP;
       }
       else
       {
           pstVdecChanCfg->ves_imp = DV_MAX_VES_IMP;
       }
       //赋值单层，双层信息
       if (HI_UNF_AVPLAY_HDR_STREAM_TYPE_SL_VES == pstChan->stVdecHdrAttr.AvplayHDRAttr.enHDRStreamType)
       {
           pstVdecChanCfg->layer_imp = DV_SINGLE_LAYER_IMP;
       }
       else if (HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_SINGLE_VES == pstChan->stVdecHdrAttr.AvplayHDRAttr.enHDRStreamType || HI_UNF_AVPLAY_HDR_STREAM_TYPE_DL_DUAL_VES == pstChan->stVdecHdrAttr.AvplayHDRAttr.enHDRStreamType)
       {
           pstVdecChanCfg->layer_imp = DV_DUAL_LAYER_IMP;
       }
       else
       {
           pstVdecChanCfg->layer_imp = DV_MAX_LAYER_IMP;
       }

   }
   else
   {
       pstVdecChanCfg->s8BeHDRChan = 0;
   }

   return;
}
#endif



static HI_S32 VDEC_SetAttr(VDEC_CHANNEL_S* pstChan)
{
    HI_S32                  s32Ret;
    VDEC_CHAN_CFG_S         stVdecChanCfg;
    HI_UNF_VCODEC_ATTR_S*   pstCfg          = &pstChan->stCurCfg;
    HI_CHIP_TYPE_E          enChipType      = HI_CHIP_TYPE_HI3716C;
    HI_CHIP_VERSION_E       enChipVersion   = HI_CHIP_VERSION_V200;
    HI_U32                  CtrlOptions     = pstCfg->s32CtrlOptions;

    memset(&stVdecChanCfg, 0, sizeof(VDEC_CHAN_CFG_S));
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_GET_CHAN_CFG, &stVdecChanCfg);

    if (VDEC_OK != s32Ret)
    {
        HI_ERR_VDEC("VFMW GET_CHAN_CFG err!\n");
        return HI_FAILURE;
    }

	if (((CtrlOptions >> 1) & 0x1) == 1)
    {
        pstChan->bLowdelay= 0;
        stVdecChanCfg.isMiracastChan = 1;
    }
    else
    {
        stVdecChanCfg.isMiracastChan = 0;
    }

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)

    if (HI_UNF_VCODEC_TYPE_HEVC == pstCfg->enType)
    {
        stVdecChanCfg.s32ModuleLowlyEnable = 0;
    }
    else
    {
        stVdecChanCfg.s32ModuleLowlyEnable = pstChan->bLowdelay;
    }

#else
    stVdecChanCfg.s32ModuleLowlyEnable = pstChan->bLowdelay;
#endif

    stVdecChanCfg.s32LowdlyEnable = 0;//pstChan->bLowdelay;

#if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    stVdecChanCfg.s32SedOnlyEnable = 1;
#else
    stVdecChanCfg.s32SedOnlyEnable = 0;
#endif

    stVdecChanCfg.s32DecOrderOutput =  pstCfg->bOrderOutput;

    stVdecChanCfg.eVidStd = VDEC_CodecTypeUnfToFmw(pstCfg->enType);

    if (HI_UNF_VCODEC_TYPE_VC1 == pstCfg->enType)
    {
        stVdecChanCfg.StdExt.Vc1Ext.IsAdvProfile = pstCfg->unExtAttr.stVC1Attr.bAdvancedProfile;
        stVdecChanCfg.StdExt.Vc1Ext.CodecVersion = (HI_S32)(pstCfg->unExtAttr.stVC1Attr.u32CodecVersion);
    }
    else if ((HI_UNF_VCODEC_TYPE_VP6 == pstCfg->enType) ||
             (HI_UNF_VCODEC_TYPE_VP6F == pstCfg->enType) ||
             (HI_UNF_VCODEC_TYPE_VP6A == pstCfg->enType))
    {
        stVdecChanCfg.StdExt.Vp6Ext.bReversed = pstCfg->unExtAttr.stVP6Attr.bReversed;
    }
    else if ((HI_UNF_VCODEC_TYPE_SORENSON == pstCfg->enType) || (HI_UNF_VCODEC_TYPE_H263 == pstCfg->enType) || \
             (HI_UNF_VCODEC_TYPE_MJPEG == pstCfg->enType))
    {
        pstChan->stOption.u32DynamicFrameStoreAllocEn = 0;
    }

    stVdecChanCfg.s32ExtraFrameStoreNum = pstChan->stOption.s32ExtraFrameStoreNum;

    stVdecChanCfg.s32ChanPriority = pstCfg->u32Priority;
    stVdecChanCfg.s32ChanErrThr = pstCfg->u32ErrCover;

    GetDecMode(pstCfg->enMode, &stVdecChanCfg);

    if (pstChan->bLowdelay)
    {
        stVdecChanCfg.s32DecMode = IP_MODE;
        pstCfg->enMode = HI_UNF_VCODEC_MODE_IP;
    }

    if (HI_TRUE == pstCfg->bOrderOutput)  // normal/simple dpb mode
    {
        stVdecChanCfg.s32DecOrderOutput = pstCfg->bOrderOutput + (CtrlOptions & 0x1);
    }
    else  // display mode
    {
        stVdecChanCfg.s32DecOrderOutput = 0;
    }

    stVdecChanCfg.s32ChanStrmOFThr = (pstChan->u32DmxBufSize * 95) / 100;
    stVdecChanCfg.s32DnrTfEnable = 0;
    stVdecChanCfg.s32DnrDispOutEnable = 0;

    /* MV300 COMPRESS PATCH */
    HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);
    GetCMPInfo(pstChan, enChipType, enChipVersion);
    /* Config decode compress attr */
    stVdecChanCfg.s32VcmpEn = EnVcmp;

    stVdecChanCfg.s32VcmpWmStartLine = 0;
    stVdecChanCfg.s32VcmpWmEndLine = 0;

    stVdecChanCfg.s32Btl1Dt2DEnable = pstChan->stOption.s32Btl1Dt2DEnable;
    stVdecChanCfg.s32BtlDbdrEnable = pstChan->stOption.s32BtlDbdrEnable;


    /* Only if pstCfg->orderOutput is 1 we do the judge */
    if (pstCfg->bOrderOutput)
    {
        if ((CtrlOptions & 0x1) == HI_UNF_VCODEC_CTRL_OPTION_NORMAL_DPB)
        {
            /*just want to set output by decode order*/
            stVdecChanCfg.s32DecOrderOutput = 1;
        }
        else if (((CtrlOptions & 0x1) & HI_UNF_VCODEC_CTRL_OPTION_SIMPLE_DPB) || pstChan->bLowdelay)
        {
            /* set to 2 means both bOrderoutput and SIMPLE_DPB */
            stVdecChanCfg.s32DecOrderOutput = 2;
        }
    }

    if (pstChan->bDPBFullCtrl)
    {
        stVdecChanCfg.s32DecOrderOutput = 3;
    }

    VDEC_Chan_GetMaxResoution(&stVdecChanCfg.s32MaxWidth, &stVdecChanCfg.s32MaxHeight);
#ifdef HI_HDR_SUPPORT
    /* set hdr info*/
    SetHDRAttr(pstChan, &stVdecChanCfg);
#endif
    /* Set to VFMW */
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_CFG_CHAN, &stVdecChanCfg);

    if (VDEC_OK != s32Ret)
    {
        HI_ERR_VDEC("VFMW CFG_CHAN err!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 VDEC_SeekPTS(HI_HANDLE hHandle, HI_U32* pu32SeekPts, HI_U32 u32Gap)
{
    HI_S32 s32Ret;
    HI_S32 s32RetStart;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    HI_U64 pTmpUserData[3] = {0};
    BUFMNG_STATUS_S stBMStatus = {0};
    HI_U32 u32RawBufferNum = 0;
    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    /*TS mode not support SeekPts*/
    if (pstChan->hDmxVidChn != HI_INVALID_HANDLE)
    {
        HI_ERR_VDEC("ERR: %d  VDEC do not support ts mode Now! \n", hHandle);
        return  HI_FAILURE;
    }

    /* stop vpss and vfmw */
    if (pstChan->enCurState != VDEC_CHAN_STATE_STOP)
    {
        if (HI_INVALID_HANDLE != s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss)
        {
            /*Stop VPSS*/
            s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss, HI_DRV_VPSS_USER_COMMAND_STOP, HI_NULL);

            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VDEC("%s HI_DRV_VPSS_USER_COMMAND_STOP err!\n", __FUNCTION__);
                return HI_FAILURE;
            }

            /*Reset VPSS*/
            s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss, HI_DRV_VPSS_USER_COMMAND_RESET, HI_NULL);

            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VDEC("%s HI_DRV_VPSS_USER_COMMAND_STOP err!\n", __FUNCTION__);
                return HI_FAILURE;
            }
        }

        /* Stop VFMW */
        if (HI_INVALID_HANDLE != pstChan->hChan)
        {
            s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_STOP_CHAN, HI_NULL);

            if (VDEC_OK != s32Ret)
            {
                HI_ERR_VDEC("Chan %d STOP_CHAN err!\n", pstChan->hChan);
                return HI_FAILURE;
            }
        }

        /* Save state */
        pstChan->enCurState = VDEC_CHAN_STATE_STOP;
    }

    /*calc rawbuffer num*/
    s32Ret = BUFMNG_GetStatus(pstChan->hStrmBuf, &stBMStatus);

    if (HI_SUCCESS == s32Ret)
    {
        u32RawBufferNum = stBMStatus.u32PutOK - stBMStatus.u32RlsOK;
    }

    /*call vfmw seek pts*/
    pTmpUserData[0] = *(pu32SeekPts);
    pTmpUserData[1] = u32Gap;
    pTmpUserData[2] = u32RawBufferNum;
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_SET_PTS_TO_SEEK, &pTmpUserData[0]);

    if (s32Ret != VDEC_OK)
    {
        HI_ERR_VDEC("VDEC_CID_SET_PTS_TO_SEEK err!\n");
        return HI_FAILURE;
    }

    *(pu32SeekPts) = pTmpUserData[0];

    /*start VFMW*/
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_START_CHAN, HI_NULL);

    if (VDEC_OK != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VDEC_CID_START_CHAN err!\n", pstChan->hChan);
        return HI_FAILURE;
    }

    /*start VPSS*/
    s32RetStart = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss, HI_DRV_VPSS_USER_COMMAND_START, HI_NULL);

    if (HI_SUCCESS != s32RetStart)
    {
        HI_ERR_VDEC("%s HI_DRV_VPSS_USER_COMMAND_START err!\n", __FUNCTION__);
        return HI_FAILURE;
    }

    /* Save state */
    pstChan->enCurState = VDEC_CHAN_STATE_RUN;

    return  s32Ret;
}

static HI_S32 VDEC_GetCap(VDEC_CAP_S* pstCap)
{
    HI_S32 s32Ret;

    if (HI_NULL == pstCap)
    {
        return HI_FAILURE;
    }

    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(HI_INVALID_HANDLE, VDEC_CID_GET_CAPABILITY, pstCap);

    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_VDEC("VFMW GET_CAPABILITY err:%d!\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 VDEC_CreateStrmBuf(HI_DRV_VDEC_STREAM_BUF_S* pstBuf)
{
    HI_S32 s32Ret;
    BUFMNG_INST_CONFIG_S stBufInstCfg;

    if (HI_NULL == pstBuf)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    /* Create buffer manager instance */
    stBufInstCfg.enAllocType = BUFMNG_ALLOC_INNER;
    stBufInstCfg.u32PhyAddr = 0;
    stBufInstCfg.pu8UsrVirAddr = HI_NULL;
    stBufInstCfg.pu8KnlVirAddr = HI_NULL;
    stBufInstCfg.u32Size = pstBuf->u32Size;
#ifdef HI_TEE_SUPPORT
    stBufInstCfg.bTvp = pstBuf->bTvp;
#endif
    strncpy(stBufInstCfg.aszName, "VDEC_ESBuf", 16);
    s32Ret = BUFMNG_Create(&(pstBuf->hHandle), &stBufInstCfg);

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VDEC("BUFMNG_Create err!\n");
        return HI_FAILURE;
    }

    pstBuf->u32PhyAddr = stBufInstCfg.u32PhyAddr;
    return HI_SUCCESS;
}

static HI_S32 VDEC_StrmBuf_SetUserAddr(HI_HANDLE hHandle, HI_U64 u64Addr)
{
    return BUFMNG_SetUserAddr(hHandle, u64Addr);
}

static HI_S32 VDEC_DestroyStrmBuf(HI_HANDLE hHandle)
{
    /* Destroy instance */
    if (HI_SUCCESS != BUFMNG_Destroy(hHandle))
    {
        HI_ERR_VDEC("Destroy buf %d err!\n", hHandle);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_AttachStrmBuf(HI_HANDLE hHandle, HI_U32 u32BufSize, HI_HANDLE hDmxVidChn, HI_HANDLE hStrmBuf)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_ERR_VDEC_INVALID_CHANID;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    /* Must attach buffer before start */
    if (pstChan->enCurState != VDEC_CHAN_STATE_STOP)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d state err:%d!\n", hHandle, pstChan->enCurState);
        return HI_ERR_VDEC_INVALID_STATE;
    }

    if (VDEC_CHAN_STRMBUF_ATTACHED(pstChan))
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d has strm buf:%d!\n", hHandle, pstChan->hStrmBuf);
        return HI_ERR_VDEC_BUFFER_ATTACHED;
    }

    if (HI_INVALID_HANDLE != hDmxVidChn)
    {
        pstChan->hDmxVidChn = hDmxVidChn;
        pstChan->u32DmxBufSize = u32BufSize;
        pstChan->hStrmBuf = HI_INVALID_HANDLE;
        pstChan->u32StrmBufSize = 0;
    }
    else
    {
        pstChan->hStrmBuf = hStrmBuf;
        pstChan->u32StrmBufSize = u32BufSize;
        pstChan->hDmxVidChn = HI_INVALID_HANDLE;
        pstChan->u32DmxBufSize = 0;
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_DetachStrmBuf(HI_HANDLE hHandle)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    /* Must stop channel first */
    if (pstChan->enCurState != VDEC_CHAN_STATE_STOP)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d state err:%d!\n", hHandle, pstChan->enCurState);
        return HI_FAILURE;
    }

    /* Clear handles */
    pstChan->hStrmBuf = HI_INVALID_HANDLE;
    pstChan->u32StrmBufSize = 0;
    pstChan->hDmxVidChn = HI_INVALID_HANDLE;
    pstChan->u32DmxBufSize = 0;
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_SetEosFlag(HI_HANDLE hHandle)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    pstChan->bSetEosFlag = HI_TRUE;

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    HI_INFO_VDEC("Chan %d STREAM_END OK\n", hHandle);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_DiscardFrm(HI_HANDLE hHandle, VDEC_DISCARD_FRAME_S* pstParam)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    if (HI_NULL == pstParam)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    /* Call vfmw */
    if (HI_INVALID_HANDLE != pstChan->hChan)
    {
        s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_SET_DISCARDPICS_PARAM, pstParam);

        if (VDEC_OK != s32Ret)
        {
            HI_ERR_VDEC("Chan %d DISCARDPICS err!\n", pstChan->hChan);
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
            return HI_FAILURE;
        }
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    HI_INFO_VDEC("Chan %d DiscardFrm mode %d OK\n", hHandle, pstParam->enMode);
    return HI_SUCCESS;
}

static HI_S32 VDEC_GetStrmBuf(HI_HANDLE hHandle, VDEC_ES_BUF_PRI_S* pstEsBuf, HI_BOOL bUserSpace)
{
    HI_S32 s32Ret;
    BUFMNG_BUF_S stElem;

    if (HI_NULL == pstEsBuf)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Get buffer */
    stElem.u32Size = pstEsBuf->u32BufSize;
    s32Ret = BUFMNG_GetWriteBuffer(hHandle, &stElem);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    pstEsBuf->u32BufSize = stElem.u32Size;

    /* If invoked by user space, return user virtual address */
    if (bUserSpace)
    {
        pstEsBuf->u64Addr = (HI_U64)(HI_SIZE_T)stElem.pu8UsrVirAddr;
    }
    /* else, invoked by kernel space, return kernel virtual address */
    else
    {
        pstEsBuf->u64Addr = (HI_U64)(HI_SIZE_T)stElem.pu8KnlVirAddr;
    }

    pstEsBuf->u32PhyAddr = stElem.u32PhyAddr;

    return HI_SUCCESS;
}

static HI_S32 VDEC_PutStrmBuf(HI_HANDLE hHandle, VDEC_ES_BUF_PRI_S* pstEsBuf, HI_BOOL bUserSpace)
{
    HI_S32 s32Ret;
    BUFMNG_BUF_S stElem;
    HI_HANDLE hVdec = HI_INVALID_HANDLE;
#ifdef  __VDEC_ADD_EXT_EOP__
    HI_S32 flag = 0;
#endif
    /* Check parameter */
    if (HI_NULL == pstEsBuf)
    {
        HI_INFO_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    /* If user sapce, put by pu8UsrVirAddr */
    if (bUserSpace)
    {
        stElem.pu8UsrVirAddr = (HI_U8*)(HI_SIZE_T)pstEsBuf->u64Addr;
        stElem.pu8KnlVirAddr = 0;
    }
    /* If kernek sapce, put by pu8KnlVirAddr */
    else
    {
        stElem.pu8KnlVirAddr = (HI_U8*)(HI_SIZE_T)pstEsBuf->u64Addr;
        stElem.pu8UsrVirAddr = 0;
    }

    stElem.u64Pts = pstEsBuf->u64Pts;
    stElem.u32Marker = 0;

    if (!pstEsBuf->bEndOfFrame)
    {
        stElem.u32Marker |= BUFMNG_NOT_END_FRAME_BIT;
    }

    if (pstEsBuf->bDiscontinuous)
    {
        stElem.u32Marker |= BUFMNG_DISCONTINUOUS_BIT;
    }

    stElem.u32Size = pstEsBuf->u32BufSize;
    s32Ret = BUFMNG_PutWriteBuffer(hHandle, &stElem);

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VDEC("Buf %d put err!\n", hHandle);
        return HI_FAILURE;
    }

    /* save raw stream */
    (VOID)VDEC_FindVdecHandleByESBufferHandle(hHandle, &hVdec);
    BUFMNG_SaveRaw((hVdec & 0xff), stElem.pu8KnlVirAddr, stElem.u32Size);

#ifdef __VDEC_ADD_EXT_EOP__ //add by l00228308
    if (hVdec != HI_INVALID_HANDLE)
    {
        //flag |= (s_stVdecDrv.astChanEntity[hVdec].pstChan->stCurCfg.enType == HI_UNF_VCODEC_TYPE_MPEG2);
        //flag |= (s_stVdecDrv.astChanEntity[hVdec].pstChan->stCurCfg.enType == HI_UNF_VCODEC_TYPE_MPEG4);
        flag |= (s_stVdecDrv.astChanEntity[hVdec].pstChan->stCurCfg.enType == HI_UNF_VCODEC_TYPE_HEVC);
        flag |= (s_stVdecDrv.astChanEntity[hVdec].pstChan->stCurCfg.enType == HI_UNF_VCODEC_TYPE_H264);
	    flag &= (s_stVdecDrv.astChanEntity[hVdec].pstChan->stVdecHdrAttr.enVdecHDRType != HI_VDEC_HDR_CHAN_TYPE_BL);
	    flag &= (s_stVdecDrv.astChanEntity[hVdec].pstChan->stVdecHdrAttr.enVdecHDRType != HI_VDEC_HDR_CHAN_TYPE_EL);
    }


    if ((pstEsBuf->bEndOfFrame) && (flag))
    {
        /* Get buffer */

        switch (s_stVdecDrv.astChanEntity[hVdec].pstChan->stCurCfg.enType)
        {
            case HI_UNF_VCODEC_TYPE_MPEG4:
                stElem.u32Size = sizeof(g_EopTab_MPEG4) / sizeof(HI_CHAR);
                break;
            case HI_UNF_VCODEC_TYPE_HEVC:
                stElem.u32Size = sizeof(g_EopTab_H265) / sizeof(HI_CHAR);
                break;
            case HI_UNF_VCODEC_TYPE_H264:
                stElem.u32Size = sizeof(g_EopTab_H264) / sizeof(HI_CHAR);
                break;
        }
        s32Ret = BUFMNG_GetWriteBuffer(hHandle, &stElem);

        if ((s32Ret == HI_SUCCESS) && (stElem.pu8KnlVirAddr))
        {
            stElem.u64Pts = pstEsBuf->u64Pts;
            stElem.u32Marker = 0;
            if (!pstEsBuf->bEndOfFrame)
            {
                stElem.u32Marker |= BUFMNG_NOT_END_FRAME_BIT;
            }
            if (pstEsBuf->bDiscontinuous)
            {
                stElem.u32Marker |= BUFMNG_DISCONTINUOUS_BIT;
            }
            stElem.u32Marker |= BUFMNG_EXTERN_EOP_BIT;
            switch (s_stVdecDrv.astChanEntity[hVdec].pstChan->stCurCfg.enType)
            {
                case HI_UNF_VCODEC_TYPE_MPEG4:
                    memcpy(stElem.pu8KnlVirAddr, g_EopTab_MPEG4, sizeof(HI_CHAR)*stElem.u32Size);
                    break;
                case HI_UNF_VCODEC_TYPE_HEVC:
                    memcpy(stElem.pu8KnlVirAddr, g_EopTab_H265, sizeof(HI_CHAR)*stElem.u32Size);
                    break;
                case HI_UNF_VCODEC_TYPE_H264:
                    memcpy(stElem.pu8KnlVirAddr, g_EopTab_H264, sizeof(HI_CHAR)*stElem.u32Size);
                    break;
            }
            s32Ret = BUFMNG_PutWriteBuffer(hHandle, &stElem);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_VDEC("Buf %d put err!\n", hHandle);
            }
        }
        else
        {
            HI_INFO_VDEC("BUFMNG_GetWriteBuffer (extern size = %d,pu8KnlVirAddr = %p) failed\n", stElem.u32Size, stElem.pu8KnlVirAddr);
        }
    }
#endif

    return HI_SUCCESS;
}

HI_S32 VDEC_GetEsFromDmx(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle, STREAM_DATA_S* pstPacket)
{
    HI_S32 s32Ret = HI_FAILURE;
    DMX_Stream_S vidEsBuf = {0};

    pstChan->stStatInfo.u32VdecAcqBufTry++;
    s32Ret = (s_stVdecDrv.pDmxFunc->pfnDmxAcquireEs)(pstChan->hDmxVidChn, &vidEsBuf);
    if (s32Ret != HI_SUCCESS)
    {
       return s32Ret;
    }

    pstPacket->PhyAddr = vidEsBuf.u32BufPhyAddr;
    pstPacket->VirAddr = (HI_U64)(HI_SIZE_T)(vidEsBuf.pu8BufVirAddr);
    pstPacket->Length = vidEsBuf.u32BufLen;
    pstPacket->Pts   = vidEsBuf.u32PtsMs;
    pstPacket->Index = vidEsBuf.u32Index;
    pstPacket->DispTime = (HI_U64)vidEsBuf.u32DispTime;
    pstPacket->DispEnableFlag = vidEsBuf.u32DispEnableFlag;
    pstPacket->DispFrameDistance = vidEsBuf.u32DispFrameDistance;
    pstPacket->DistanceBeforeFirstFrame = vidEsBuf.u32DistanceBeforeFirstFrame;
    pstPacket->GopNum = vidEsBuf.u32GopNum;
    pstPacket->is_not_last_packet_flag = 0;
    pstPacket->UserTag = 0;
    //pstPacket->discontinue_count = 0;
    pstPacket->is_stream_end_flag = 0;
    pstPacket->is_extern_eop_flag = 0;

    /*get first valid pts*/
    if (0 == pstChan->u32ValidPtsFlag && -1 != vidEsBuf.u32PtsMs)
    {
        pstChan->bFirstValidPts = HI_TRUE;
        pstChan->u32FirstValidPts = vidEsBuf.u32PtsMs;
        pstChan->u32ValidPtsFlag = 1;
    }
    /*get second valid pts*/
    else if (1 == pstChan->u32ValidPtsFlag && -1 != vidEsBuf.u32PtsMs)
    {
        pstChan->bSecondValidPts = HI_TRUE;
        pstChan->u32SecondValidPts = vidEsBuf.u32PtsMs;
        pstChan->u32ValidPtsFlag = 2;
    }

    if (0 == pstChan->stStatInfo.u32TotalVdecInByte)
    {
        HI_DRV_STAT_Event(STAT_EVENT_STREAMIN, 0);
    }

    pstChan->stStatInfo.u32TotalVdecInByte   += pstPacket->Length;
    pstChan->stStatInfo.u32TotalVdecHoldByte += pstPacket->Length;
    pstChan->stStatInfo.u32VdecAcqBufOK++;

#ifdef TEST_VDEC_SAVEFILE
    VDEC_Dbg_RecSaveFile(hHandle, pstPacket->VirAddr, pstPacket->Length);
#endif

    return s32Ret;
}

HI_S32 VDEC_GetEsFromBM(VDEC_CHANNEL_S* pstChan,
                         HI_HANDLE hHandle,
                         STREAM_DATA_S* pstPacket,
                         HI_BOOL bUserSpace)
{
    HI_S32 s32Ret = HI_FAILURE;
    BUFMNG_BUF_S stEsBuf = {0};

    s32Ret = BUFMNG_AcqReadBuffer(pstChan->hStrmBuf, &stEsBuf);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    pstPacket->PhyAddr = stEsBuf.u32PhyAddr;

    /* If get from user space, pass user virtual address */
    if (bUserSpace)
    {
        pstPacket->VirAddr = (HI_U64)(HI_SIZE_T)(stEsBuf.pu8UsrVirAddr);
    }
    /* If get from kernel space, pass kernel virtual address */
    else
    {
        pstPacket->VirAddr = (HI_U64)(HI_SIZE_T)(stEsBuf.pu8KnlVirAddr);
    }

    pstPacket->Length = stEsBuf.u32Size;
    pstPacket->Pts = stEsBuf.u64Pts;
    pstPacket->Index = stEsBuf.u32Index;
    pstPacket->is_not_last_packet_flag = stEsBuf.u32Marker & BUFMNG_NOT_END_FRAME_BIT;

#if 0

    if (stEsBuf.u32Marker & BUFMNG_DISCONTINUOUS_BIT)
    {
        pstChan->u32DiscontinueCount++;
    }

#endif

    pstPacket->is_extern_eop_flag = (stEsBuf.u32Marker & BUFMNG_EXTERN_EOP_BIT) ? 1 : 0;
    pstPacket->UserTag = 0;

    //pstPacket->discontinue_count = pstChan->u32DiscontinueCount;

    pstPacket->is_stream_end_flag = (stEsBuf.u32Marker & BUFMNG_END_OF_STREAM_BIT) ? 1 : 0;

    /*get first valid pts*/
    if (0 == pstChan->u32ValidPtsFlag && -1 != (HI_U32)stEsBuf.u64Pts)
    {
        pstChan->bFirstValidPts = HI_TRUE;
        pstChan->u32FirstValidPts = (HI_U32)stEsBuf.u64Pts;
        pstChan->u32ValidPtsFlag = 1;
    }
    /*get second valid pts*/
    else if (1 == pstChan->u32ValidPtsFlag && -1 != (HI_U32)stEsBuf.u64Pts)
    {
        pstChan->bSecondValidPts = HI_TRUE;
        pstChan->u32SecondValidPts = (HI_U32)stEsBuf.u64Pts;
        pstChan->u32ValidPtsFlag = 2;
    }

    pstChan->stStatInfo.u32TotalVdecInByte   += pstPacket->Length;
    pstChan->stStatInfo.u32TotalVdecHoldByte += pstPacket->Length;

    return s32Ret;
}

static HI_S32 VDEC_RecvStrmBuf(HI_HANDLE hHandle, STREAM_DATA_S* pstPacket, HI_BOOL bUserSpace)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    BUFMNG_BUF_S stEsBuf;

    memset(&stEsBuf, 0, sizeof(BUFMNG_BUF_S));
    VDEC_ASSERT_RETURN(pstPacket != HI_NULL, HI_FAILURE);

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);
    VDEC_ASSERT_RETURN(s32Ret == HI_SUCCESS, HI_FAILURE);

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        s32Ret = HI_FAILURE;
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return s32Ret;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (pstChan->hDmxVidChn == HI_INVALID_HANDLE && s_stVdecDrv.astChanEntity[hHandle].DmxHdlCallBack)
    {
        s_stVdecDrv.astChanEntity[hHandle].DmxHdlCallBack(pstChan->u32DmxID, 2, &(pstChan->hDmxVidChn));
    }

    /* Get ES data from demux directly */
    if ((pstChan->hDmxVidChn != HI_INVALID_HANDLE) && s_stVdecDrv.pDmxFunc && s_stVdecDrv.pDmxFunc->pfnDmxAcquireEs)
    {
        s32Ret = VDEC_GetEsFromDmx(pstChan, hHandle, pstPacket);
        if (s32Ret != HI_SUCCESS)
        {
            goto OUT;
        }
    }
    else if((pstChan->OmxTunnelMode) && (pstChan->hOmxChn != HI_INVALID_HANDLE) &&\
            (s_stVdecDrv.pOmxFunc) && (s_stVdecDrv.pOmxFunc->pfnOmxAcquireStream))
    {

        pstChan->stStatInfo.u32VdecAcqBufTry++;

        s32Ret = (s_stVdecDrv.pOmxFunc->pfnOmxAcquireStream)(pstChan->hOmxChn, (HI_VOID*)pstPacket);
        if (HI_SUCCESS != s32Ret)
        {
            goto OUT;
        }

        pstChan->stStatInfo.u32TotalVdecInByte   += pstPacket->Length;
        pstChan->stStatInfo.u32TotalVdecHoldByte += pstPacket->Length;
        pstChan->stStatInfo.u32VdecAcqBufOK++;
    }
    /* Get ES data from BM */
    else
    {
        s32Ret = VDEC_GetEsFromBM(pstChan, hHandle, pstPacket, bUserSpace);
        if (s32Ret != HI_SUCCESS)
        {
            goto OUT;
        }
    }

OUT:

    /* Added for set eos flag */
    /* Must be end of stream */
    if ((pstChan->bSetEosFlag) &&
        /* Get ES buffer fail */
        (((pstChan->hStrmBuf != HI_INVALID_HANDLE) && (HI_SUCCESS != s32Ret)) ||
         /* Get Demux buffer HI_ERR_DMX_EMPTY_BUFFER */
         ((pstChan->hDmxVidChn != HI_INVALID_HANDLE) && (HI_ERR_DMX_EMPTY_BUFFER == s32Ret))) && \
         (pstChan->OmxTunnelMode != 1))
    {
        pstPacket->PhyAddr = pstChan->stEOSBuffer.u32StartPhyAddr;
        pstPacket->VirAddr = (HI_U64)(HI_SIZE_T)(pstChan->stEOSBuffer.pu8StartVirAddr);
        pstPacket->Length = pstChan->stEOSBuffer.u32Size;
        pstPacket->Pts = HI_INVALID_PTS;
        pstPacket->Index = 0;
        pstPacket->UserTag = 0;

#if 0

        if (pstChan->hDmxVidChn != HI_INVALID_HANDLE)
        {
            pstPacket->discontinue_count = 0;
        }
        else
        {
            pstPacket->discontinue_count = pstChan->u32DiscontinueCount;
        }

#endif

        pstPacket->is_not_last_packet_flag = 0;
        pstPacket->is_stream_end_flag = 1;
        pstChan->bSetEosFlag = HI_FALSE;
        s32Ret = HI_SUCCESS;
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    return s32Ret;
}

static HI_S32 VDEC_RlsStrmBuf(HI_HANDLE hHandle, STREAM_DATA_S* pstPacket, HI_BOOL bUserSpace)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    BUFMNG_BUF_S stBuf;

    if (HI_NULL == pstPacket)
    {
        HI_INFO_VDEC("INFO: %d pstPacket == HI_NULL!\n", hHandle);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    /* Release EOS MMZ buffer */
    if (pstPacket->PhyAddr == pstChan->stEOSBuffer.u32StartPhyAddr)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return HI_SUCCESS;
    }

    /* Put ES buffer of demux */
    if ((pstChan->hDmxVidChn != HI_INVALID_HANDLE) && s_stVdecDrv.pDmxFunc && s_stVdecDrv.pDmxFunc->pfnDmxReleaseEs)
    {
        DMX_Stream_S vidEsBuf;

        pstChan->stStatInfo.u32VdecRlsBufTry++;
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

        vidEsBuf.u32BufPhyAddr = pstPacket->PhyAddr;
        vidEsBuf.pu8BufVirAddr = (HI_U8*)(HI_SIZE_T)(pstPacket->VirAddr);
        vidEsBuf.u32BufLen = pstPacket->Length;
        vidEsBuf.u32PtsMs = pstPacket->Pts;
        vidEsBuf.u32Index = pstPacket->Index;

        s32Ret = (s_stVdecDrv.pDmxFunc->pfnDmxReleaseEs)(pstChan->hDmxVidChn, &vidEsBuf);

        if (HI_SUCCESS != s32Ret)
        {
            HI_WARN_VDEC("VDEC ReleaseBuf(%#x) to Dmx err:%#x.\n", pstPacket->PhyAddr, s32Ret);
        }
        else
        {
            pstChan->stStatInfo.u32TotalVdecHoldByte -= vidEsBuf.u32BufLen;
            pstChan->stStatInfo.u32VdecRlsBufOK++;
        }

        return s32Ret;
    }
    else if((pstChan->OmxTunnelMode) && (pstChan->hOmxChn != HI_INVALID_HANDLE) &&\
            (s_stVdecDrv.pOmxFunc) && (s_stVdecDrv.pOmxFunc->pfnOmxReleaseStream))
    {
        pstChan->stStatInfo.u32VdecRlsBufTry++;

        s32Ret = (s_stVdecDrv.pOmxFunc->pfnOmxReleaseStream)(pstChan->hOmxChn, (HI_VOID*)pstPacket);

        if (HI_SUCCESS != s32Ret)
        {
            HI_WARN_VDEC("VDEC ReleaseBuf(%#x) to Dmx err:%#x.\n", pstPacket->PhyAddr, s32Ret);
        }
        else
        {
            pstChan->stStatInfo.u32TotalVdecHoldByte -= pstPacket->Length;
            pstChan->stStatInfo.u32VdecRlsBufOK++;
        }

        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return s32Ret;

    }
    /* Put BM buffer */
    else
    {
        stBuf.u32PhyAddr = 0;

        if (bUserSpace)
        {
            stBuf.pu8UsrVirAddr = (HI_U8*)(HI_SIZE_T)(pstPacket->VirAddr);
            stBuf.pu8KnlVirAddr = HI_NULL;
        }
        else
        {
            stBuf.pu8KnlVirAddr = (HI_U8*)(HI_SIZE_T)(pstPacket->VirAddr);
            stBuf.pu8UsrVirAddr = HI_NULL;
        }

        stBuf.u32Size  = pstPacket->Length;
        stBuf.u32Index = pstPacket->Index;
        stBuf.u64Pts = pstPacket->Pts;
        /* Don't care stBuf.u32Marker here. */

        /* Put */
        s32Ret = BUFMNG_RlsReadBuffer(pstChan->hStrmBuf, &stBuf);

        if (HI_SUCCESS != s32Ret)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
            return HI_FAILURE;
        }

        pstChan->stStatInfo.u32TotalVdecHoldByte -= stBuf.u32Size;
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    }

    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_RecvStrmBuf(HI_S32 hHandle, STREAM_DATA_S* pstPacket)
{
    return VDEC_RecvStrmBuf((HI_HANDLE)hHandle, pstPacket, HI_FALSE);
}

static HI_S32 VDEC_Chan_RlsStrmBuf(HI_S32 hHandle, STREAM_DATA_S* pstPacket)
{
    return VDEC_RlsStrmBuf((HI_HANDLE)hHandle, pstPacket, HI_FALSE);
}
//add by l00225186
/*提供给vo收帧的函数*/
static HI_S32 VDEC_Chan_VOAcqFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstVpssFrame)
{
    HI_S32 s32Ret;

    if ((HI_INVALID_HANDLE == hPort) || (HI_NULL == pstVpssFrame))
    {
        HI_ERR_VDEC("VDEC_Chan_VOAcqFrame Bad param!\n");
        return HI_FAILURE;
    }

    /*调用vpss的获取帧存函数,要确定vpss释放函数成功返回的是HI_SUCCESS*/
#ifdef VFMW_VPSS_BYPASS_EN
    BUFMNG_SpinLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);
#endif
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortFrame)(hPort, pstVpssFrame);

#ifdef VFMW_VPSS_BYPASS_EN
    BUFMNG_SpinUnLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);
#endif
    return s32Ret;
}

HI_S32 HI_VDEC_Chan_VOAcqFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstVpssFrame)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if ((HI_INVALID_HANDLE == hPort) || (HI_NULL == pstVpssFrame))
    {
        HI_ERR_VDEC("VDEC_Chan_VOAcqFrame Bad param!\n");
        return HI_FAILURE;
    }
    s32Ret = VDEC_Chan_VOAcqFrame(hPort, pstVpssFrame);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_Chan_VOAcqFrame err!\n");
        return HI_FAILURE;
    }
    return s32Ret;
}
/*提供给vo释放帧的函数*/
static HI_S32 VDEC_Chan_VORlsFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstVpssFrame)
{
    HI_S32 s32Ret;
    HI_DRV_VPSS_PORT_CFG_S stVpssPortCfg;
    HI_HANDLE hVdec = HI_INVALID_HANDLE;
    VDEC_CHANNEL_S* pstChan;
#ifdef VFMW_VPSS_BYPASS_EN
    HI_S32 s32RetSpecialRls = HI_FAILURE;
#endif

    s32Ret = VDEC_FindVdecHandleByPortHandle(hPort, &hVdec);

    if (HI_SUCCESS != s32Ret)
    {
        goto error;
    }
    else
    {
        pstChan = VDEC_DRV_GetChan(hVdec);

        if (HI_NULL != pstChan)
        {
            pstChan->stStatInfo.u32AvplayRlsFrameTry ++;
        }
        else
        {
            HI_ERR_VDEC("VDEC_Chan_VORlsFrame call VDEC_DRV_GetChan ERROR!\n");
            goto error;
        }
    }

    if ((HI_INVALID_HANDLE == hPort) || (HI_NULL == pstVpssFrame))
    {
        HI_ERR_VDEC("VDEC_Chan_VORlsFrame Bad param!\n");
        goto error;
    }

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortCfg)(hPort, &stVpssPortCfg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("HI_DRV_VPSS_GetDefaultPortCfg err!\n");
        goto error;
    }

    if (HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == stVpssPortCfg.stBufListCfg.eBufType)
    {
        s32Ret = BUFMNG_VPSS_RelBuffer((HI_HANDLE)PORTHANDLE_TO_VPSSID(hPort), hPort, pstVpssFrame);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("BUFMNG_VPSS_RelBuffer err!\n");
             goto error;
        }
    }
    else
    {
        /*调用vpss的释放帧存函数*/
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssRelPortFrame)(hPort, pstVpssFrame);
    }


    if (HI_SUCCESS == s32Ret)
    {
        pstChan->stStatInfo.u32AvplayRlsFrameOK++;
        return s32Ret;
    }

error:
#ifdef VFMW_VPSS_BYPASS_EN
    if (pstVpssFrame != NULL)
    {
        BUFMNG_SpinLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);
        if (s_stVdecDrv.stRemainFrmList.s32Num != 0)
        {

            HI_S32 s32Ret = HI_FAILURE;
            HI_U32 u32Index = 0;

            s32Ret = BUFMNG_VDEC_List_FindNodeIndex(&s_stVdecDrv.stRemainFrmList, pstVpssFrame->stBufAddr[0].u32PhyAddr_YHead, &u32Index);
            if ( HI_SUCCESS != s32Ret )
            {
                HI_INFO_VDEC("Can't find the remain Frm node (Phy = 0x%x,bSecureMem = %d), Remain num = %d\n",
                             pstVpssFrame->stBufAddr[0].u32PhyAddr_Y, pstVpssFrame->bSecure, s_stVdecDrv.stRemainFrmList.s32Num);
            }
            else
            {
                /*this frame will be release in the thread*/
                s_stVdecDrv.stRemainFrmList.stSpecialFrmRec[u32Index].bCanRls = HI_TRUE;
                s32RetSpecialRls = HI_SUCCESS;
                HI_INFO_VDEC("set the remain Frm node (Phy = 0x%x,bSecureMem = %d) can be release, Remain num = %d\n",
                             pstVpssFrame->stBufAddr[0].u32PhyAddr_Y, pstVpssFrame->bSecure, s_stVdecDrv.stRemainFrmList.s32Num);
            }
        }
        BUFMNG_SpinUnLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);
    }
    return s32RetSpecialRls;
#else
    return HI_FAILURE;
#endif
}
HI_S32 HI_VDEC_Chan_VORlsFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstVpssFrame)
{
    HI_S32 s32Ret;
    if (HI_NULL == pstVpssFrame)
    {
        HI_ERR_VDEC("invalid param !\n");
        return HI_FAILURE;
    }
    s32Ret = VDEC_Chan_VORlsFrame(hPort, pstVpssFrame);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_Chan_VORlsFrame err ! Phyaddr = 0x%x\n", pstVpssFrame->stBufAddr[0].u32PhyAddr_Y);
        return HI_FAILURE;
    }
    return s32Ret;
}
static HI_S32 VDEC_ConvertWinInfo(HI_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S* pstVpssPortCfg, HI_DRV_WIN_PRIV_INFO_S* pstWinInfo)
{
    HI_HANDLE hVdec = HI_INVALID_HANDLE;
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 i;
    pstVpssPortCfg->s32OutputWidth               = pstWinInfo->stOutRect.s32Width;
    pstVpssPortCfg->s32OutputHeight              = pstWinInfo->stOutRect.s32Height;
    s32Ret = VDEC_FindVdecHandleByPortHandle(hPort, &hVdec);

    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    hVdec = hVdec & 0xff;

    for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
    {
        if (s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[i].hPort == hPort)
        {
            break;
        }
    }

    if (i >= VDEC_MAX_PORT_NUM)
    {
        return HI_FAILURE;
    }

    if ( HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stPort[i].bufferType)
    {
        pstVpssPortCfg->s32OutputWidth           = 0;
        pstVpssPortCfg->s32OutputHeight          = 0;
    }

    pstVpssPortCfg->u32MaxFrameRate              = pstWinInfo->u32MaxRate;
    pstVpssPortCfg->eDstCS                       = HI_DRV_CS_BT709_YUV_LIMITED;
    pstVpssPortCfg->eFormat                      = pstWinInfo->ePixFmt;
    pstVpssPortCfg->enOutBitWidth                = pstWinInfo->enBitWidth;
    pstVpssPortCfg->eAspMode                     = pstWinInfo->enARCvrs;
    pstVpssPortCfg->stCustmAR                    = pstWinInfo->stCustmAR;


    pstVpssPortCfg->stDispPixAR                 = pstWinInfo->stScreenAR;

    pstVpssPortCfg->stScreen = pstWinInfo->stScreen;
    pstVpssPortCfg->b3Dsupport = pstWinInfo->bIn3DMode;

    switch (pstWinInfo->enRotation)
    {
        case HI_DRV_ROT_ANGLE_0:
            pstVpssPortCfg->enRotation = HI_DRV_VPSS_ROTATION_DISABLE;
            break;

        case HI_DRV_ROT_ANGLE_90:
            pstVpssPortCfg->enRotation = HI_DRV_VPSS_ROTATION_90;
            break;

        case HI_DRV_ROT_ANGLE_180:
            pstVpssPortCfg->enRotation = HI_DRV_VPSS_ROTATION_180;
            break;

        case HI_DRV_ROT_ANGLE_270:
            pstVpssPortCfg->enRotation = HI_DRV_VPSS_ROTATION_270;
            break;

        default:
            HI_ERR_VDEC("Invalid Rotation param %d !\n", pstWinInfo->enRotation);
            pstVpssPortCfg->enRotation = HI_DRV_VPSS_ROTATION_DISABLE;
            break;
    }

    pstVpssPortCfg->bHoriFlip = pstWinInfo->bHoriFlip;
    pstVpssPortCfg->bVertFlip = pstWinInfo->bVertFlip;
    pstVpssPortCfg->bTunnelEnable = pstWinInfo->bTunnelSupport;
    pstVpssPortCfg->bPassThrough = pstWinInfo->bPassThrough;
    memcpy(&(pstVpssPortCfg->stInRect), &(pstWinInfo->stInRect), sizeof(HI_RECT_S));
    pstVpssPortCfg->bUseCropRect = pstWinInfo->bUseCropRect;
    memcpy(&(pstVpssPortCfg->stCropRect), &(pstWinInfo->stCropRect), sizeof(HI_DRV_CROP_RECT_S));
    /*
    pstVpssPortCfg->stBufListCfg.eBufType        = 0;
    pstVpssPortCfg->stBufListCfg.u32BufNumber    = 6;
    pstVpssPortCfg->stBufListCfg.u32BufSize      = 2*(pstWinInfo->stOutRect.s32Width)*(pstWinInfo->stOutRect.s32Height);
    pstVpssPortCfg->stBufListCfg.u32BufStride    = pstVpssPortCfg->s32OutputWidth;
    */
    return HI_SUCCESS;
}
//add by l00225186
static HI_S32 VDEC_Chan_VOChangeWinInfo(HI_HANDLE hPort, HI_DRV_WIN_PRIV_INFO_S* pstWinInfo)
{
    HI_S32 s32Ret;
    HI_DRV_VPSS_PORT_CFG_S stVpssPortCfg;
    HI_DRV_VPSS_CFG_S stVpssCfg;
    HI_HANDLE hVpss;
    HI_HANDLE hVdec = HI_INVALID_HANDLE;

    if ((HI_INVALID_HANDLE == hPort) || (HI_NULL == pstWinInfo))
    {
        HI_ERR_VDEC("VDEC_Chan_VOChangeWinInfo Bad param!\n");
        return HI_FAILURE;
    }

    s32Ret = VDEC_FindVdecHandleByPortHandle(hPort, &hVdec);

    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    /*when decode i frame,user set vpss config*/
    hVdec = hVdec & 0xff;

    if (s_stVdecDrv.astChanEntity[hVdec].pstChan->bIsIFrameDec == HI_TRUE)
    {
        return HI_SUCCESS;
    }

    hVpss = PORTHANDLE_TO_VPSSID(hPort);
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortCfg)(hPort, &stVpssPortCfg);

    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetVpssCfg)(hVpss, &stVpssCfg);

    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (pstWinInfo->bUseCropRect )
    {
        stVpssCfg.stProcCtrl.bUseCropRect = HI_TRUE;
        stVpssCfg.stProcCtrl.stCropRect.u32LeftOffset = pstWinInfo->stCropRect.u32LeftOffset;
        stVpssCfg.stProcCtrl.stCropRect.u32RightOffset = pstWinInfo->stCropRect.u32RightOffset;
        stVpssCfg.stProcCtrl.stCropRect.u32BottomOffset = pstWinInfo->stCropRect.u32BottomOffset;
        stVpssCfg.stProcCtrl.stCropRect.u32TopOffset = pstWinInfo->stCropRect.u32TopOffset;
    }
    else
    {
        stVpssCfg.stProcCtrl.bUseCropRect = HI_FALSE;
        stVpssCfg.stProcCtrl.stInRect.s32Height = pstWinInfo->stInRect.s32Height;
        stVpssCfg.stProcCtrl.stInRect.s32Width  = pstWinInfo->stInRect.s32Width;
        stVpssCfg.stProcCtrl.stInRect.s32X      = pstWinInfo->stInRect.s32X;
        stVpssCfg.stProcCtrl.stInRect.s32Y      = pstWinInfo->stInRect.s32Y;
    }

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSetVpssCfg)(hVpss, &stVpssCfg);

    if (0 != s32Ret)
    {
        HI_ERR_VDEC("call VpssSetVpssCfg err!\n");
    }

    VDEC_ConvertWinInfo(hPort, &stVpssPortCfg, pstWinInfo);
    /*重新设置PORT属性*/
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSetPortCfg)(hPort, &stVpssPortCfg);
    return s32Ret;
}
HI_S32 HI_VDEC_Chan_VOChangeWinInfo(HI_HANDLE hPort, HI_DRV_WIN_PRIV_INFO_S* pstWinInfo)
{
    HI_S32 s32Ret;
    if ((HI_INVALID_HANDLE == hPort) || (HI_NULL == pstWinInfo))
    {
        HI_ERR_VDEC("HI_VDEC_Chan_VOChangeWinInfo Bad param!\n");
        return HI_FAILURE;
    }
    s32Ret = VDEC_Chan_VOChangeWinInfo(hPort, pstWinInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("call VDEC_Chan_VOChangeWinInfo err!\n");
    }
    return s32Ret;
}
//add by l00225185
/*创建vpss*/
static HI_S32 VDEC_Chan_CreateVpss(HI_HANDLE hVdec, HI_HANDLE* phVpss)
{
    HI_S32 s32Ret;
    HI_DRV_VPSS_CFG_S stVpssCfg;
    HI_DRV_VPSS_SOURCE_FUNC_S stRegistSrcFunc;

    if ((HI_INVALID_HANDLE == hVdec) || (HI_NULL == phVpss))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    hVdec &= 0xFF;

    if (hVdec >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("VDEC_Chan_CreateVpss err hvdec:%d too large!\n", hVdec);
        return HI_FAILURE;
    }

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetDefaultCfg)(&stVpssCfg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("HI_DRV_VPSS_GetDefaultCfg err!\n");
        return HI_FAILURE;
    }

    /*向上返回创建的vpss句柄*/
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssCreateVpss)(&stVpssCfg, phVpss);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("HI_DRV_VPSS_CreateVpss err!\n");
        return HI_FAILURE;
    }

    down(&s_stVdecDrv.stSem);
    //s32Ret = down(&s_stVdecDrv.stSem);

    //if (0 != s32Ret)
    //{
        //HI_WARN_VDEC("call down_interruptible err!\n");
    //}

    /*保存vpss句柄*/
    s_stVdecDrv.astChanEntity[hVdec].stVpssChan.hVpss = *phVpss;
    up(&s_stVdecDrv.stSem);


    s_stVdecDrv.astChanEntity[hVdec].stVpssChan.eFramePackType = HI_UNF_FRAME_PACKING_TYPE_BUTT;
    s_stVdecDrv.astChanEntity[hVdec].stVpssChan.enFrameBuffer = HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE;//HI_DRV_VDEC_BUF_USER_ALLOC_MANAGE;//;

    s_stVdecDrv.astChanEntity[hVdec].stVpssChan.enVideoBypass = VDEC_VPSS_BYPASSMODE_BUTT;

    /*注册vpss回调函数*/
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssRegistHook)(*phVpss, hVdec, VDEC_VpssEventHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("HI_DRV_VPSS_RegistHook err!\n");
        return HI_FAILURE;
    }

    stRegistSrcFunc.VPSS_GET_SRCIMAGE = VDEC_Chan_VpssRecvFrmBuf;
    stRegistSrcFunc.VPSS_REL_SRCIMAGE = VDEC_Chan_VpssRlsFrmBuf;
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSetSourceMode)(*phVpss, VPSS_SOURCE_MODE_VPSSACTIVE, &stRegistSrcFunc);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("HI_DRV_VPSS_SetSourceMode err!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 VDEC_Chan_DestroyVpss(HI_HANDLE hVdec)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hVpss;

    //s32Ret = down_interruptible(&s_stVdecDrv.stSem);
    hVpss = s_stVdecDrv.astChanEntity[hVdec].stVpssChan.hVpss;

    if (HI_INVALID_HANDLE != hVpss)
    {
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(s_stVdecDrv.astChanEntity[hVdec].stVpssChan.hVpss, HI_DRV_VPSS_USER_COMMAND_STOP, NULL);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("HI_DRV_VPSS_USER_COMMAND_STOP err!\n");
            return HI_FAILURE;
        }

        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssDestroyVpss)(hVpss);

        if (HI_SUCCESS != s32Ret)
        {
            //up(&s_stVdecDrv.stSem);
            return s32Ret;
        }

        s_stVdecDrv.astChanEntity[hVdec].stVpssChan.hVpss = HI_INVALID_HANDLE;
        s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stControlInfo.u32BackwardOptimizeFlag = HI_FALSE;
        s_stVdecDrv.astChanEntity[hVdec].stVpssChan.stControlInfo.u32DispOptimizeFlag = HI_FALSE;
        s_stVdecDrv.astChanEntity[hVdec].stVpssChan.s32GetFirstIFrameFlag = HI_FALSE;
        s_stVdecDrv.astChanEntity[hVdec].stVpssChan.s32GetFirstVpssFrameFlag = HI_FALSE;
        s_stVdecDrv.astChanEntity[hVdec].stVpssChan.eLastFrameFormat = VDEC_FRAME_BUTT;
        //s_stVdecDrv.astChanEntity[hVdec].stVpssChan.s32LastFrameGopNum = -1;
        s_stVdecDrv.astChanEntity[hVdec].stVpssChan.s32ImageDistance = 0;
    }

    //up(&s_stVdecDrv.stSem);

    return s32Ret;
}

static HI_S32 VDEC_Chan_CreatePort(HI_HANDLE hVpss, HI_HANDLE* phPort, VDEC_PORT_ABILITY_E ePortAbility )
{
    HI_S32 s32Ret;
    HI_DRV_VPSS_PORT_CFG_S stVpssPortCfg;
    HI_S32 i, j;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;

#ifdef HI_TEE_SUPPORT
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    HI_DRV_VPSS_CFG_S stVpssCfg;
#endif


    if ((HI_INVALID_HANDLE == hVpss) || (HI_NULL == phPort))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    /*find astChanEntity by vpss handle*/
    /*CNcomment:首先应该找到hVpss对应的是第几个astChanEntity*/
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d lock fail!\n", i);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

#ifdef HI_TEE_SUPPORT

    pstChan = s_stVdecDrv.astChanEntity[i].pstChan;

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetVpssCfg)(hVpss, &stVpssCfg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("vdec call Vpss %d VpssGetVpssCfg err\n", hVpss);
        return HI_FAILURE;
    }

    stVpssCfg.bSecure = pstChan->bTvp;
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSetVpssCfg)(hVpss, &stVpssCfg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("vdec call Vpss %d VpssSetVpssCfg err\n", hVpss);
        return HI_FAILURE;
    }

#endif

    /*get defualt port cfg*/
    /*CNcomment:创建port操作，调用vpss创建port函数，并在vdec端做记录*/
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetDefaultPortCfg)(&stVpssPortCfg);

    if (HI_SUCCESS != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        HI_ERR_VDEC("HI_DRV_VPSS_GetDefaultPortCfg err!\n");
        return HI_FAILURE;
    }

    if (HI_DRV_VDEC_BUF_USER_ALLOC_MANAGE == pstVpssChan->enFrameBuffer || HI_DRV_VDEC_BUF_VDEC_ALLOC_MANAGE == pstVpssChan->enFrameBuffer )
    {
        stVpssPortCfg.stBufListCfg.eBufType = HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE;
    }
    /*create port*/
    /*CNcomment:创建port*/
    else if ( HI_DRV_VDEC_BUF_VPSS_ALLOC_MANAGE == pstVpssChan->enFrameBuffer)
    {
        stVpssPortCfg.stBufListCfg.eBufType = HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE;
    }

    switch (ePortAbility)
    {
        case VDEC_PORT_HD:
            stVpssPortCfg.s32OutputHeight = 1080;
            stVpssPortCfg.s32OutputWidth  = 1920;
            break;

        case VDEC_PORT_SD:
            stVpssPortCfg.s32OutputHeight = 576;
            stVpssPortCfg.s32OutputWidth  = 720;
            break;

        case VDEC_PORT_STR:
            stVpssPortCfg.s32OutputHeight = 720;
            stVpssPortCfg.s32OutputWidth  = 1280;
            break;

        default:
            break;
    }

    stVpssPortCfg.stBufListCfg.u32BufSize = stVpssPortCfg.s32OutputHeight * stVpssPortCfg.s32OutputWidth * 3 / 2;
    stVpssPortCfg.stBufListCfg.u32BufStride = stVpssPortCfg.s32OutputWidth;

    if (HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == stVpssPortCfg.stBufListCfg.eBufType)
    {
        stVpssPortCfg.s32OutputWidth = 0;    // 宽高设为0表示根据输入自适应配置
        stVpssPortCfg.s32OutputHeight = 0;
    }

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssCreatePort)(hVpss, &stVpssPortCfg, phPort);

    if (HI_SUCCESS != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        HI_ERR_VDEC("HI_DRV_VPSS_CreatePort err!\n");
        return HI_FAILURE;
    }

    /*save port handle*/
    /*CNcomment:vdec端记录port句柄*/
    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if (HI_INVALID_HANDLE == pstVpssChan->stPort[j].hPort)
        {
            break;
        }
    }

    if (j >= VDEC_MAX_PORT_NUM)
    {
        HI_ERR_VDEC("Too many ports!\n");
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        return HI_FAILURE;
    }
    else
    {
        pstVpssChan->stPort[j].hPort = *phPort;
        pstVpssChan->stPort[j].bEnable = HI_TRUE;
        pstVpssChan->stPort[j].enPortType = VDEC_PORT_TYPE_BUTT;
        pstVpssChan->stPort[j].bufferType = stVpssPortCfg.stBufListCfg.eBufType;

        /*init the vpss buffer*/
        /*CNcomment:如果由vdec来管理vpss的帧存则进行帧存的初始化*/
        if (HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == stVpssPortCfg.stBufListCfg.eBufType)
        {
            pstVpssChan->stPort[j].stBufVpssInst.enFrameBuffer = pstVpssChan->enFrameBuffer;
        }
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_DestroyPort(HI_HANDLE hVpss, HI_HANDLE hPort)
{
    HI_S32 s32Ret;
    HI_S32 i, j;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;

    if ((HI_INVALID_HANDLE == hVpss) || (HI_INVALID_HANDLE == hPort))
    {
        HI_ERR_VDEC("Bad param! hVpss:%#x hPort:%#x\n", hVpss, hPort);
        return HI_FAILURE;
    }

    /*find astChanEntity by vpss handle*/
    /*CNcomment:首先应该找到hVpss对应的是第几个astChanEntity*/
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", i);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

    /*find port*/
    /*CNcomment:查找port是否存在*/
    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if (hPort == pstVpssChan->stPort[j].hPort)
        {
            break;
        }
    }

    if (j >= VDEC_MAX_PORT_NUM)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        HI_WARN_VDEC("Port %d not exist!\n", hPort);
        return HI_FAILURE;
    }
    else
    {
        pstVpssChan->stPort[j].hPort = HI_INVALID_HANDLE;
        pstVpssChan->stPort[j].bEnable = HI_FALSE;
        pstVpssChan->stPort[j].bufferType = HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE;
        pstVpssChan->stPort[j].enPortType = VDEC_PORT_TYPE_BUTT;
        pstVpssChan->stPort[j].s32PortTmpListPos = 0;

        /*call vpss funtion to destory port*/
        /*CNcomment:调用vpss接口销毁port*/
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssDestroyPort)(hPort);

        if (HI_SUCCESS != s32Ret)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
            HI_WARN_VDEC("HI_DRV_VPSS_DestroyPort err!\n");
            return HI_FAILURE;
        }

        if (HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == pstVpssChan->stPort[j].bufferType)
        {
            BUFMNG_VPSS_DeInit(&pstVpssChan->stPort[j].stBufVpssInst);
        }
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);

    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_EnablePort(HI_HANDLE hVpss, HI_HANDLE hPort )
{
    HI_S32 s32Ret;
    HI_S32 i, j;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;

    if ((HI_INVALID_HANDLE == hVpss) || (HI_INVALID_HANDLE == hPort))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    /*find astChanEntity by vpss handle*/
    /*CNcomment:首先应该找到hVpss对应的是第几个astChanEntity*/
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    if (HI_SUCCESS != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        HI_INFO_VDEC("Chan %d lock fail!\n", i);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

    /*enable port*/
    /*CNcomment:查找port是否存在*/
    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if (hPort == pstVpssChan->stPort[j].hPort)
        {
            break;
        }
    }

    if (j >= VDEC_MAX_PORT_NUM)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        HI_WARN_VDEC("Port %d not exist!\n", hPort);
        return HI_FAILURE;
    }
    else
    {
        pstVpssChan->stPort[j].bEnable = HI_TRUE;
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssEnablePort)(hPort, HI_TRUE);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("call VpssEnablePort err!\n");
        }
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_DisablePort(HI_HANDLE hVpss, HI_HANDLE hPort )
{
    HI_S32 s32Ret;
    HI_S32 i, j;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;

    if ((HI_INVALID_HANDLE == hVpss) || (HI_INVALID_HANDLE == hPort))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    /*find astChanEntity by vpss handle*/
    /*CNcomment:首先应该找到hVpss对应的是第几个astChanEntity*/
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", i);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

    /*disenable port,find port*/
    /*CNcomment:查找port是否存在*/
    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if (hPort == pstVpssChan->stPort[j].hPort)
        {
            break;
        }
    }

    if (j >= VDEC_MAX_PORT_NUM)
    {
        HI_WARN_VDEC("Port %d not exist!\n", hPort);
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        return HI_FAILURE;
    }
    else
    {
        pstVpssChan->stPort[j].bEnable = HI_FALSE;
        pstVpssChan->stPort[j].s32PortLastFrameGopNum = -1;
        pstVpssChan->stPort[j].u32LastFrameIndex = -1;
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssEnablePort)(hPort, HI_FALSE);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("call pfnVpssEnablePort err!\n");
        }
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_ResetVpss(HI_HANDLE hVpss)
{
    HI_S32 i = 0;
    HI_S32 s32Ret;
    HI_U32 u32Index;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    VDEC_PORT_FRAME_LIST_LOCK_S* pListLock = HI_NULL;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
    VDEC_PORT_FRAME_LIST_NODE_S* pstListNode = HI_NULL;

    if (HI_INVALID_HANDLE == hVpss)
    {
        HI_ERR_VDEC("VDEC_Chan_ResetVpss Bad param!\n");
        return HI_FAILURE;
    }

    s32Ret = VDEC_FindVdecHandleByVpssHandle(hVpss, &hHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_FindVdecHandleByVpssHandle err!\n");
        return HI_FAILURE;
    }

    /*Reset pvr tmplist and portlist data*/
    for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
    {
        pListLock = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].stPortList.stPortFrameListLock);
        pstVpssChan = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan);

        if ((HI_INVALID_HANDLE != pstVpssChan->stPort[i].hPort) && (HI_TRUE == pstVpssChan->stPort[i].bEnable))
        {
            VDEC_SpinLockIRQ(pListLock);

            if (0 < pstVpssChan->stPort[i].s32PortTmpListPos)
            {
                for (u32Index = 0; u32Index < pstVpssChan->stPort[i].s32PortTmpListPos; u32Index++)
                {
                    s32Ret = VDEC_Chan_VORlsFrame(pstVpssChan->stPort[i].hPort, &(pstVpssChan->stPort[i].astPortTmpList[u32Index].stPortOutFrame));

                    if (s32Ret != HI_SUCCESS)
                    {
                        HI_ERR_VDEC("VDEC pvr mode rls vpss port %d frame error.\n", pstVpssChan->stPort[i].hPort);
                    }
                }
            }

            pstVpssChan->stPort[i].s32PortTmpListPos = 0;

            while ((&pstVpssChan->stPort[i].stPortList.stVdecPortFrameList) != (pstVpssChan->stPort[i].stPortList.stVdecPortFrameList.next))
            {
                pstListNode = list_entry(pstVpssChan->stPort[i].stPortList.stVdecPortFrameList.next, VDEC_PORT_FRAME_LIST_NODE_S, node);
                s32Ret = VDEC_Chan_VORlsFrame(pstVpssChan->stPort[i].hPort, &(pstListNode->stPortOutFrame));

                if (s32Ret != HI_SUCCESS)
                {
                    HI_ERR_VDEC("VDEC pvr mode rls vpss port %d frame error.\n", pstVpssChan->stPort[i].hPort);
                }

                list_del_init(&(pstListNode->node));
                HI_KFREE_BUFMNG(pstListNode);
            }

            VDEC_SpinUnLockIRQ(pListLock);
        }
    }

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(hVpss, HI_DRV_VPSS_USER_COMMAND_RESET, HI_NULL);
    return s32Ret;
}

static HI_S32 VDEC_Chan_SetPortType(HI_HANDLE hVpss, HI_HANDLE hPort, VDEC_PORT_TYPE_E enPortType )
{
    HI_S32 s32Ret;
    HI_S32 i, j;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
    //HI_DRV_VPSS_PORT_CFG_S stVpssPortCfg;

    if ((HI_INVALID_HANDLE == hVpss) || (HI_INVALID_HANDLE == hPort))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    /*find astChanEntity by vpss handle*/
    /*CNcomment:首先应该找到hVpss对应的是第几个astChanEntity*/
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", i);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

    /*set main port,find port*/
    /*CNcomment:查找port是否存在*/
    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if (hPort == pstVpssChan->stPort[j].hPort)
        {
            break;
        }
    }

    if (j >= VDEC_MAX_PORT_NUM)
    {
        HI_WARN_VDEC("Port %d not exist!\n", hPort);
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        return HI_FAILURE;
    }
    else
    {
        pstVpssChan->stPort[j].enPortType = enPortType;

        /*set MaxFrameRate 30pfs when port is virtual port*/
        if (VDEC_PORT_TYPE_VIRTUAL == enPortType)
        {
#if 0
            s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortCfg)(hPort, &stVpssPortCfg);

            if (HI_SUCCESS != s32Ret)
            {
                VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
                HI_ERR_VDEC("pfnVpssGetPortCfg err!\n");
                return HI_FAILURE;
            }

            stVpssPortCfg.u32MaxFrameRate  = 30;
            s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSetPortCfg)(hPort, &stVpssPortCfg);

            if (HI_SUCCESS != s32Ret)
            {
                VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
                HI_ERR_VDEC("pfnVpssSetPortCfg err!\n");
                return HI_FAILURE;
            }

#endif
        }
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_SetVpssAttr(HI_HANDLE hVpss, HI_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S* pstPortCfg)
{
    HI_S32                  s32Ret;
    HI_S32                  i, j;
    VDEC_VPSSCHANNEL_S*      pstVpssChan = HI_NULL;

    if (HI_INVALID_HANDLE == hPort)
    {
        HI_ERR_VDEC("hPort is invalid!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Too many chans!\n");
        return HI_FAILURE;
    }

    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if (hPort == pstVpssChan->stPort[j].hPort)
        {
            break;
        }
    }

    if (j >= VDEC_MAX_PORT_NUM)
    {
        HI_WARN_VDEC("Port %d not exist!\n", hPort);
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        return HI_FAILURE;
    }
    else
    {
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSetPortCfg)(hPort, pstPortCfg);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("pfnVpssSetPortCfg ERR, Ret=%#x\n", s32Ret);
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
            return s32Ret;
        }

    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);

    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_GetVpssAttr(HI_HANDLE hVpss, HI_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S* pstPortCfg)
{
    HI_S32                  s32Ret;
    HI_S32                  i, j;
    VDEC_VPSSCHANNEL_S*      pstVpssChan = HI_NULL;

    if (HI_INVALID_HANDLE == hPort)
    {
        HI_ERR_VDEC("hPort is invalid!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Too many chans!\n");
        return HI_FAILURE;
    }

    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if (hPort == pstVpssChan->stPort[j].hPort)
        {
            break;
        }
    }

    if (j >= VDEC_MAX_PORT_NUM)
    {
        HI_WARN_VDEC("Port %d not exist!\n", hPort);
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        return HI_FAILURE;
    }
    else
    {
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortCfg)(hPort, pstPortCfg);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("pfnVpssGetPortCfg ERR, Ret=%#x\n", s32Ret);
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
            return HI_FAILURE;
        }

    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);

    return HI_SUCCESS;
}



static HI_S32 VDEC_DRV_ION_Mmap(VDEC_ION_BUF_S* pIonBuf)
{
    HI_U8* pu8VirAddr = HI_NULL;
    struct dma_buf* bufHhd = HI_NULL;
    if (pIonBuf == HI_NULL)
    {
        HI_ERR_VDEC("input null parm\n");
        return HI_FAILURE;
    }

    bufHhd = dma_buf_get(pIonBuf->BufFd);

    if ( HI_NULL == bufHhd)
    {
        HI_ERR_VDEC("%s dma_buf_get failed!\n", __func__);
        return HI_FAILURE;
    }
    if (HI_SUCCESS != dma_buf_begin_cpu_access(bufHhd, 0, 0, 0))
    {
        HI_ERR_VDEC("dmabuf map failed!\n", __func__);
        goto err;
    }

    pu8VirAddr = dma_buf_kmap(bufHhd, 0 /*Offset*/);
    if (pu8VirAddr == HI_NULL)
    {
        HI_ERR_VDEC("dma_buf_get return HI_NULL!! fd = 0x%x\n", pIonBuf->BufFd);
        goto err1;
    }

    pIonBuf->pu8DmaBuf = (HI_U8*)bufHhd;
    pIonBuf->pu8StartVirAddr = pu8VirAddr;

    HI_INFO_VDEC("map success!!! dmabuf = %p, pu8VirAddr = %p\n", bufHhd, pu8VirAddr);

    return HI_SUCCESS;

err1:
    dma_buf_end_cpu_access(bufHhd, 0, 0, 0);
err:
    dma_buf_put(bufHhd);
    return HI_FAILURE;
}

static HI_VOID VDEC_DRV_ION_Unmap(VDEC_ION_BUF_S* pIonBuf)
{
    struct dma_buf* bufHhd = HI_NULL;

    if (pIonBuf == HI_NULL)
    {
        HI_ERR_VDEC("input null parm\n");
        return ;
    }

    bufHhd = (struct dma_buf*)pIonBuf->pu8DmaBuf;

    dma_buf_kunmap(bufHhd, 0, 0);

    dma_buf_end_cpu_access(bufHhd, 0, 0, 0);

    dma_buf_put(bufHhd);

    return ;
}

static HI_S32 VDEC_Chan_SetExtBuffer(HI_HANDLE hVpss, VDEC_BUFFER_ATTR_PRI_S* pstBufferAttr)
{
    HI_S32 i;
    BUFMNG_VPSS_NODE_S* pstBufNode;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
    BUFMNG_VPSS_INST_S* pstBufVpssInst;
    BUFMNG_VPSS_NODE_S* pstTarget;
    struct list_head* pos, *n;
    HI_S32 s32Ret = HI_SUCCESS;
    VDEC_ION_BUF_S stVdecMetaPriv = {0};
#ifdef VFMW_VPSS_BYPASS_EN
    HI_DRV_VPSS_PORT_CFG_S stVpssPortCfg;
#endif

    if ((HI_INVALID_HANDLE == hVpss) || (HI_NULL == pstBufferAttr))
    {
        HI_ERR_VDEC("Bad param eFramePackType!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);
    pstBufVpssInst = &(pstVpssChan->stPort[0].stBufVpssInst);

#ifdef VFMW_VPSS_BYPASS_EN
    HI_INFO_VDEC("set bypass = %d\n", pstBufferAttr->bVideoBypass);

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortCfg)(pstVpssChan->stPort[0].hPort, &stVpssPortCfg);
    if (s32Ret == HI_SUCCESS)
    {
        stVpssPortCfg.bPassThrough = pstBufferAttr->bVideoBypass;
        pstVpssChan->enVideoBypass = (pstBufferAttr->bVideoBypass == HI_FALSE) ? VDEC_VPSS_BYPASSMODE_DISABLE : VDEC_VPSS_BYPASSMODE_ENABLE;
        if (HI_SUCCESS != (s_stVdecDrv.pVpssFunc->pfnVpssSetPortCfg)(pstVpssChan->stPort[0].hPort, &stVpssPortCfg))
        {
            HI_ERR_VDEC("pfnVpssSetPortCfg set vpssbypass = %d failed!\n", stVpssPortCfg.bPassThrough);
        }
    }
    else
    {
        HI_ERR_VDEC("ERROR: pfnVpssGetPortCfg return %d\n", s32Ret);
    }
#endif

    /*网络播放场景，bufferType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE 时,start vpss 的时机需要设置在配置了bypass策略之后 add by l00228308 */
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(pstVpssChan->hVpss, HI_DRV_VPSS_USER_COMMAND_START, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("HI_DRV_VPSS_USER_COMMAND_START err!\n");
        return HI_FAILURE;
    }

    if (HI_DRV_VDEC_BUF_USER_ALLOC_MANAGE == s_stVdecDrv.astChanEntity[i].stVpssChan.enFrameBuffer)
    {
        //只支持每次配的buf大小一样
        if (0 != pstBufVpssInst->u32BufNum && pstBufferAttr->u32BufSize != pstBufVpssInst->u32FrmBufSize)
        {
            HI_ERR_VDEC("the buffer size if not same! the size should FrmBuf:%d MetaBuf:%d but you put FrmBuf:%d MetaBuf:%d u32BufNum:%d\n", \
                        pstBufVpssInst->u32FrmBufSize, pstBufVpssInst->u32MetaBufSize, pstBufferAttr->u32BufSize, pstBufferAttr->u32MetaBufSize, pstBufVpssInst->u32BufNum);
            return HI_FAILURE;
        }

        //第一次外部把buf设置进来时，把大小记录下来
        if (0 == pstBufVpssInst->u32BufNum)
        {
            pstBufVpssInst->u32FrmBufSize = pstBufferAttr->u32BufSize;
            pstBufVpssInst->u32MetaBufSize = pstBufferAttr->u32MetaBufSize;
        }

        if (pstBufferAttr->u32PhyAddr_meta[0] == 0xffffffff || pstBufferAttr->u32PhyAddr_meta[0] == 0)
        {
            HI_BOOL bStore = HI_FALSE;
            s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(hVpss, HI_DRV_VPSS_USER_COMMAND_STOREPRIV, &bStore);
        }
        else
        {
            HI_BOOL bStore = HI_TRUE;
            s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(hVpss, HI_DRV_VPSS_USER_COMMAND_STOREPRIV, &bStore);
        }

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("Vpss:%d call VpssSendCommand error!\n", pstVpssChan->hVpss);
            return HI_FAILURE;
        }

        for (i = 0; i < pstBufferAttr->u32BufNum; i++)
        {
            pstBufNode = HI_VMALLOC_BUFMNG(sizeof(BUFMNG_VPSS_NODE_S));

            if (HI_NULL == pstBufNode)
            {
                list_for_each_safe(pos, n, &(pstBufVpssInst->stVpssBufUnAvailableList))
                {
                    pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);
                    list_del_init(pos);
                    HI_VFREE_BUFMNG(pstTarget);
                }
                HI_ERR_VDEC("BUFMNG_VPSS_Init No memory.\n");
                return HI_ERR_BM_NO_MEMORY;
            }

#if 1
            memset(&stVdecMetaPriv, 0, sizeof(VDEC_ION_BUF_S));

            if (pstBufferAttr->u32PhyAddr_meta[i] != 0xffffffff  &&  pstBufferAttr->u32PhyAddr_meta[i] != 0)
            {
                HI_S32 s32Ret = HI_FAILURE;

                stVdecMetaPriv.BufFd = pstBufferAttr->s32MetadataBufFd[i];
                stVdecMetaPriv.u32Offset = pstBufferAttr->u32PrivOffset_meta;

                s32Ret = VDEC_DRV_ION_Mmap(&stVdecMetaPriv);
                if (s32Ret != HI_SUCCESS)
                {
                    HI_ERR_VDEC("VDEC_DRV_ION_Mmap fd = 0x%x, phyaddr = 0x%x,failed!\n", pstBufferAttr->s32MetadataBufFd[i], pstBufferAttr->u32PhyAddr_meta[i]);
                }
            }
#endif
            pstBufNode->VDECBuf_frm.u32StartPhyAddr = pstBufferAttr->u32PhyAddr[i];
            pstBufNode->VDECBuf_frm.pu8StartVirAddr = (HI_U8*)(HI_SIZE_T)pstBufferAttr->u64UsrVirAddr[i];
            pstBufNode->VDECBuf_meta.u32StartPhyAddr = pstBufferAttr->u32PhyAddr_meta[i];
            pstBufNode->VDECBuf_meta.pu8StartVirAddr = stVdecMetaPriv.pu8StartVirAddr + stVdecMetaPriv.u32Offset;
            pstBufNode->VDECBuf_frm.u32Size         = pstBufferAttr->u32BufSize;
            pstBufNode->VDECBuf_meta.u32Size         = pstBufferAttr->u32MetaBufSize;
            pstBufNode->enFrameBufferState       = HI_DRV_VDEC_BUF_STATE_IN_VDEC_EMPTY;
            pstBufNode->u32Stride                = pstBufferAttr->u32Stride;

            memcpy(&pstBufNode->VDECBuf_meta_priv, &stVdecMetaPriv, sizeof(VDEC_ION_BUF_S));

            BUFMNG_SpinLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
            pstBufNode->bBufUsed = HI_FALSE;
            pstBufVpssInst->u32BufNum ++;
            list_add_tail(&(pstBufNode->node), &(pstBufVpssInst->stVpssBufAvailableList));
            BUFMNG_SpinUnLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
        }
    }

    return HI_SUCCESS;
}
static HI_S32 VDEC_Chan_SetFrameBufferMode(HI_HANDLE hVpss, VDEC_FRAMEBUFFER_MODE_E enFrameBufferMode)
{
    HI_S32 i;

    if ((HI_INVALID_HANDLE == hVpss) || (VDEC_BUF_TYPE_BUTT == enFrameBufferMode))
    {
        HI_ERR_VDEC("Bad param eFramePackType!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    if (VDEC_BUF_VPSS_ALLOC_MANAGE == enFrameBufferMode)
    {
        s_stVdecDrv.astChanEntity[i].stVpssChan.enFrameBuffer = HI_DRV_VDEC_BUF_VPSS_ALLOC_MANAGE;
    }

    if (VDEC_BUF_USER_ALLOC_MANAGE == enFrameBufferMode)
    {
        s_stVdecDrv.astChanEntity[i].stVpssChan.enFrameBuffer = HI_DRV_VDEC_BUF_USER_ALLOC_MANAGE;
        s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[0].bufferType = HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE;
        BUFMNG_VPSS_Init(&(s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[0].stBufVpssInst));
    }

    return HI_SUCCESS;
}
static HI_S32 VDEC_Chan_CheckAndDelBuffer(HI_HANDLE hVpss, VDEC_BUFFER_INFO_S* pstBufInfo)
{
    HI_S32 i;
    struct list_head* pos, *n;
    BUFMNG_VPSS_NODE_S* pstTarget;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
    BUFMNG_VPSS_INST_S* pstBufVpssInst;

    if ((HI_INVALID_HANDLE == hVpss) || (HI_NULL == pstBufInfo))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);
    BUFMNG_SpinLockIRQ(&pstVpssChan->stPort[0].stBufVpssInst.stUnAvailableListLock);
    pstBufVpssInst = &(pstVpssChan->stPort[0].stBufVpssInst);

    list_for_each_safe(pos, n, &(pstVpssChan->stPort[0].stBufVpssInst.stVpssBufAvailableList))
    {
        pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);

        if (pstBufInfo->u32PhyAddr == pstTarget->VDECBuf_frm.u32StartPhyAddr)
        {
            pstBufInfo->enBufState = VDEC_BUF_STATE_EMPTY;
            list_del_init(pos);
            pstBufVpssInst->u32BufNum--;

            BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[0].stBufVpssInst.stUnAvailableListLock);
#if 1
            if (pstTarget->VDECBuf_meta.pu8StartVirAddr != 0)
            {
                VDEC_DRV_ION_Unmap(&pstTarget->VDECBuf_meta_priv);
            }
#endif
            HI_VFREE_BUFMNG(pstTarget);

            return HI_SUCCESS;
        }
    }
    list_for_each_safe(pos, n, &(pstVpssChan->stPort[0].stBufVpssInst.stVpssBufUnAvailableList))
    {
        pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);

        if (pstBufInfo->u32PhyAddr == pstTarget->VDECBuf_frm.u32StartPhyAddr)
        {
            if (pstTarget->enFrameBufferState == HI_DRV_VDEC_BUF_STATE_IN_VPSS)
            {
                pstBufInfo->enBufState = VDEC_BUF_STATE_IN_USE;
                BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[0].stBufVpssInst.stUnAvailableListLock);
                return HI_FAILURE;
            }
            else
            {
                pstBufInfo->enBufState = VDEC_BUF_STATE_FULL;

                if (pstVpssChan->stPort[0].stBufVpssInst.pstUnAvailableListPos == pos)
                {
                    pstVpssChan->stPort[0].stBufVpssInst.pstUnAvailableListPos = pos->prev;
                }

                if (pstTarget->enFrameBufferState == HI_DRV_VDEC_BUF_STATE_IN_VDEC_FULL)
                {
                    pstBufVpssInst->u32AvaiableFrameCnt --;
                }

                list_del_init(pos);
                pstBufVpssInst->u32BufNum--;
                BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[0].stBufVpssInst.stUnAvailableListLock);
#if 1
                if (pstTarget->VDECBuf_meta.pu8StartVirAddr != 0)
                {
                    VDEC_DRV_ION_Unmap(&pstTarget->VDECBuf_meta_priv);
                }
#endif
                HI_VFREE_BUFMNG(pstTarget);
                return HI_SUCCESS;
            }
        }
    }
    BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[0].stBufVpssInst.stUnAvailableListLock);
    return HI_FAILURE;
}

static HI_S32 VDEC_Chan_SetExtBufferState(HI_HANDLE hVpss, VDEC_EXTBUFFER_STATE_E enExtBufferState)
{
    HI_S32 i;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
    BUFMNG_VPSS_INST_S* pstBufVpssInst;

    if ((HI_INVALID_HANDLE == hVpss) || (enExtBufferState >= VDEC_EXTBUFFER_STATE_BUTT))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);
    pstBufVpssInst = &(pstVpssChan->stPort[0].stBufVpssInst);
    pstBufVpssInst->enExtBufferState = enExtBufferState;
    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_SetResolution(HI_HANDLE hVpss, VDEC_RESOLUTION_ATTR_S  stResolution)
{
    HI_S32 s32Ret;
    HI_S32 i, j;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    HI_DRV_VPSS_PORT_CFG_S stPortCfg;
    HI_HANDLE hPort;

    if (HI_INVALID_HANDLE == hVpss)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[i].pstChan;

    //0表示将vdec记录的分辨率信息清0，其余情况表示设置vpss的输出宽高
    if (0 == stResolution.s32Height && 0 == stResolution.s32Width)
    {
        pstChan->stLastFrm.stDispRect.s32Height = stResolution.s32Height;
        pstChan->stLastFrm.stDispRect.s32Width = stResolution.s32Width;
    }
    else
    {
        for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
        {
            if (HI_INVALID_HANDLE != s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].hPort)
            {
                break;
            }
        }

        if (j >= VDEC_MAX_PORT_NUM)
        {
            HI_ERR_VDEC("Too many ports!\n");
            return HI_FAILURE;
        }

        hPort = s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].hPort;
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortCfg)(hPort, &stPortCfg);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("pfnVpssGetPortCfg ERR, Ret=%#x\n", s32Ret);
            return HI_FAILURE;
        }

        stPortCfg.s32OutputHeight = stResolution.s32Height;
        stPortCfg.s32OutputWidth = stResolution.s32Width;
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSetPortCfg)(hPort, &stPortCfg);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("pfnVpssSetPortCfg ERR, Ret=%#x\n", s32Ret);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_GetLeftStreamFrm(HI_HANDLE hHandle, HI_U32* pLeftFrm)
{
    HI_S32 s32Ret = HI_SUCCESS;
    BUFMNG_STATUS_S stStatus;
    VDEC_CHANNEL_S *pstChan = HI_NULL;

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (HI_INVALID_HANDLE != pstChan->hStrmBuf)
    {
        s32Ret = BUFMNG_GetStatus(pstChan->hStrmBuf, &stStatus);
        if (HI_SUCCESS != s32Ret)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
            HI_ERR_VDEC("Chan %d get strm buf status err!\n", hHandle);
            return HI_FAILURE;
        }

        *pLeftFrm = (stStatus.u32PutOK > pstChan->stStatInfo.u32TotalVfmwOutFrame) ? \
                    (stStatus.u32PutOK - pstChan->stStatInfo.u32TotalVfmwOutFrame) : \
                     0;//仅仅用于按帧送流并每帧后增加帧结束码的场景
    }
    else
    {
        *pLeftFrm = 0;
        HI_ERR_VDEC("Chan %d strm buf handle = NULL!\n", hHandle);
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return HI_FAILURE;
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_SetFrmPackingType(HI_HANDLE hVpss, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E eFramePackType)
{
    HI_S32 s32Ret;
    HI_S32 i;

    if ((HI_INVALID_HANDLE == hVpss) || (HI_UNF_FRAME_PACKING_TYPE_BUTT == eFramePackType))
    {
        HI_ERR_VDEC("Bad param eFramePackType!\n");
        return HI_FAILURE;
    }

    /*find astChanEntity by vpss handle*/
    /*CNcomment:首先应该找到hVpss对应的是第几个astChanEntity*/
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", i);
        return HI_FAILURE;
    }

    s_stVdecDrv.astChanEntity[i].stVpssChan.eFramePackType = eFramePackType;

    if (HI_NULL != s_stVdecDrv.astChanEntity[i].pstChan)
    {
        s_stVdecDrv.astChanEntity[i].pstChan->eFramePackType = eFramePackType;
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
    return HI_SUCCESS;
}
HI_S32 HI_DRV_VDEC_SetFrmPackingType(HI_HANDLE hVdec, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E eFramePackType)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss = HI_INVALID_HANDLE;
    hVdec = hVdec & 0xff;
    s32Ret = VDEC_FindVpssHandleByVdecHandle(hVdec, &hVpss);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_FindVpssHandleByVdecHandle in function:%s %d\n", __func__, __LINE__);
        return HI_FAILURE;
    }

    s32Ret = VDEC_Chan_SetFrmPackingType(hVpss, eFramePackType);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_Chan_SetFrmPackingType in function:%s %d\n", __func__, __LINE__);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
static HI_S32 VDEC_Chan_GetFrmPackingType(HI_HANDLE hVpss, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E* penFramePackType)
{
    HI_S32 s32Ret;
    HI_S32 i;

    if ((HI_INVALID_HANDLE == hVpss) || (HI_NULL == penFramePackType))
    {
        HI_ERR_VDEC("Bad param penFramePackType!\n");
        return HI_FAILURE;
    }

    /*find astChanEntity by vpss handle*/
    /*CNcomment:首先应该找到hVpss对应的是第几个astChanEntity*/
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", i);
        return HI_FAILURE;
    }

    *penFramePackType = s_stVdecDrv.astChanEntity[i].stVpssChan.eFramePackType;

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_GetFrmPackingType(HI_HANDLE hVdec, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E* penFramePackType)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss = HI_INVALID_HANDLE;
    hVdec = hVdec & 0xff;
    s32Ret = VDEC_FindVpssHandleByVdecHandle(hVdec, &hVpss);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_FindVpssHandleByVdecHandle in function:%s %d\n", __func__, __LINE__);
        return HI_FAILURE;
    }

    s32Ret = VDEC_Chan_GetFrmPackingType(hVpss, penFramePackType);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_Chan_SetFrmPackingType in function:%s %d\n", __func__, __LINE__);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
static HI_UNF_VIDEO_FRAME_PACKING_TYPE_E VDEC_ConverFrameType(HI_DRV_FRAME_TYPE_E  eFrmType)
{
    switch (eFrmType)
    {
        case HI_DRV_FT_NOT_STEREO:
            return HI_UNF_FRAME_PACKING_TYPE_NONE;

        case HI_DRV_FT_SBS:
            return HI_UNF_FRAME_PACKING_TYPE_SIDE_BY_SIDE;

        case HI_DRV_FT_TAB:
            return HI_UNF_FRAME_PACKING_TYPE_TOP_AND_BOTTOM;

        case HI_DRV_FT_FPK:
            return HI_UNF_FRAME_PACKING_TYPE_FRAME_PACKING;

        case HI_DRV_FT_TILE:
            return HI_UNF_FRAME_PACKING_TYPE_3D_TILE;

        default:
            return HI_UNF_FRAME_PACKING_TYPE_BUTT;
    }
}
HI_S32 HI_DRV_VDEC_GetVideoFrameInfo(HI_HANDLE hVdec, HI_UNF_AVPLAY_VIDEO_FRAME_INFO_S* pstVideoFrameInfo)
{
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    hVdec = hVdec & 0xff;

    if (hVdec >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    if (HI_NULL == s_stVdecDrv.astChanEntity[hVdec].pstChan)
    {
        HI_ERR_VDEC("Chan %d not init!\n", hVdec);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hVdec].pstChan;
    pstVideoFrameInfo->u32Width           = pstChan->stLastFrm.u32Width;
    pstVideoFrameInfo->u32Height          = pstChan->stLastFrm.u32Height;
    pstVideoFrameInfo->u32AspectWidth     = pstChan->stLastFrm.u32AspectWidth;
    pstVideoFrameInfo->u32AspectHeight    = pstChan->stLastFrm.u32AspectHeight;
    pstVideoFrameInfo->u32fpsInteger      = pstChan->stLastVpssFrm.u32FrameRate / 1000;
    pstVideoFrameInfo->u32fpsDecimal      = 0;
    pstVideoFrameInfo->bProgressive       = pstChan->stLastVpssFrm.bProgressive;
    pstVideoFrameInfo->enFramePackingType = VDEC_ConverFrameType(pstChan->stLastVpssFrm.eFrmType);
    return HI_SUCCESS;
}
static HI_S32 VDEC_Chan_GetPortParam(HI_HANDLE hVpss, HI_HANDLE hPort, VDEC_PORT_PARAM_S* pstPortParam)
{
    HI_S32 s32Ret;
    HI_U32 i;

    if ((HI_INVALID_HANDLE == hVpss) || (HI_INVALID_HANDLE == hPort) || (HI_NULL == pstPortParam))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    /*find astChanEntity by vpss handle*/
    /*CNcomment:首先应该找到hVpss对应的是第几个astChanEntity*/
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", i);
        return HI_FAILURE;
    }

    /*主要是给vo提供获取/释放帧存的函数和wininfo改变的处理函数,获取帧存的函数只是为了和释放配对*/
    pstPortParam->pfVOAcqFrame = VDEC_Chan_VOAcqFrame;
    pstPortParam->pfVORlsFrame = VDEC_Chan_VORlsFrame;
    pstPortParam->pfVOSendWinInfo = VDEC_Chan_VOChangeWinInfo;
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_GetPortState(HI_HANDLE hHandle, HI_BOOL* bAllPortComplete)
{
    HI_S32 s32Ret;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;

    /* check input parameters */
    if ((HI_INVALID_HANDLE == hHandle) || (HI_NULL == bAllPortComplete))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan);

    /*get vpss status*/
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(pstVpssChan->hVpss, HI_DRV_VPSS_USER_COMMAND_CHECKALLDONE, bAllPortComplete);

    if (HI_SUCCESS != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Vpss:%d HI_DRV_VPSS_USER_COMMAND_CHECKALLDONE error!\n", pstVpssChan->hVpss);
        return HI_FAILURE;
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_SendEos(HI_HANDLE hVdec)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    hVdec = hVdec & 0xff;

    if (hVdec >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("%s %d hVdec:%d\n", __func__, __LINE__, hVdec);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hVdec]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d lock fail!\n", hVdec);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hVdec].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hVdec]);
        HI_WARN_VDEC("Chan %d not init!\n", hVdec);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hVdec].pstChan;
    ((HI_VDEC_PRIV_FRAMEINFO_S*)(((HI_DRV_VIDEO_PRIVATE_S*)(pstChan->stLastFrm.u32Priv))->u32Reserve))->u8EndFrame = 1;
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hVdec]);
    return HI_SUCCESS;

}

static HI_S32 VDEC_Chan_GetFrmStatusInfo(HI_HANDLE hVdec, HI_HANDLE hPort, VDEC_FRMSTATUSINFO_S* pstVdecFrmStatusInfo)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    HI_DRV_VPSS_PORT_BUFLIST_STATE_S stVpssBufListState;
    //VDEC_CHAN_FRMSTATUSINFO_S stVdecChanFrmStatusInfo;
    VDEC_CHAN_STATE_S stChanState;

    /* check input parameters */
    if (HI_NULL == pstVdecFrmStatusInfo)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    memset(&stChanState, 0, sizeof(VDEC_CHAN_STATE_S));
    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hVdec]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hVdec);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hVdec].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hVdec]);
        HI_ERR_VDEC("Chan %d not init!\n", hVdec);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hVdec].pstChan;

    /*get vfmw frame status*/
    //TODO: add function in vfmw VDEC_CID_GET_CHAN_STATE
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_GET_CHAN_STATE, &stChanState);

    if (VDEC_OK != s32Ret)
    {
        HI_FATAL_VDEC("Chan %d GET_CHAN_STATE err\n", pstChan->hChan);
    }
    else
    {
        pstVdecFrmStatusInfo->u32DecodedFrmNum = stChanState.decoded_1d_frame_num + stChanState.wait_disp_frame_num;
        pstVdecFrmStatusInfo->u32StrmSize = stChanState.buffered_stream_size;//u32StrmSize;
    }

    /*get vpss port frame status*/
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortBufListState)(hPort, &stVpssBufListState);

    if (HI_SUCCESS != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hVdec]);
        HI_ERR_VDEC("Get Port %d status error!\n", hPort);

        return HI_FAILURE;
    }

    /*port used_buffer number*/
    pstVdecFrmStatusInfo->u32OutBufFrmNum = stVpssBufListState.u32FulBufNumber;

    /*copy from VDEC_Chan_GetStatusInfo*/
    pstVdecFrmStatusInfo->u32StrmInBps = pstChan->stStatInfo.u32AvrgVdecInBps;

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hVdec]);

    return s32Ret;
}

static HI_S32 VDEC_Chan_AllPortHaveDate(HI_HANDLE hVpss)
{
    HI_S32 i, j = 0;
    BUFMNG_VPSS_NODE_S* pstTarget;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;

    /*find astChanEntity by vpss handle*/
    /*CNcomment:首先应该找到hVpss对应的是第几个astChanEntity*/
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if ((HI_INVALID_HANDLE != pstVpssChan->stPort[j].hPort) && (HI_TRUE == pstVpssChan->stPort[j].bEnable))
        {
            if (HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == pstVpssChan->stPort[j].bufferType)
            {
                BUFMNG_SpinLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);

                /*首先判断是不是所有的有效port队列上都有数据，只要有一个没有则退出*/
                if (pstVpssChan->stPort[j].stBufVpssInst.pstUnAvailableListPos->next
                    != &pstVpssChan->stPort[j].stBufVpssInst.stVpssBufUnAvailableList && (pstVpssChan->stPort[j].stBufVpssInst.u32AvaiableFrameCnt > 0))
                {
                    pstTarget = list_entry(pstVpssChan->stPort[j].stBufVpssInst.pstUnAvailableListPos->next, BUFMNG_VPSS_NODE_S, node);

                    if ((NULL == pstTarget) || (HI_DRV_VDEC_BUF_STATE_IN_USER == pstTarget->enFrameBufferState))
                    {
                        BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
                        return HI_FAILURE;
                    }

                    if (HI_TRUE == pstTarget->bBufUsed)
                    {
                        BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
                        continue;
                    }
                    else
                    {
                        if (VDEC_PORT_TYPE_VIRTUAL != pstVpssChan->stPort[j].enPortType)
                        {
                            BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
                            return HI_FAILURE;
                        }
                        else
                        {
                            BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
                            continue;
                        }
                    }
                }
                else
                {
                    BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
                    return HI_FAILURE;
                }
            }
        }
    }

    return HI_SUCCESS;
}

#if 0
static HI_S32 VDEC_Chan_OnePortHaveDate(HI_HANDLE hVpss, HI_HANDLE hPort)
{
    HI_S32 i = 0;
    BUFMNG_VPSS_NODE_S* pstTarget;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);
    {
        if ((HI_INVALID_HANDLE != pstVpssChan->stPort[hPort].hPort) && (HI_TRUE == pstVpssChan->stPort[hPort].bEnable))
        {
            if (HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == pstVpssChan->stPort[hPort].bufferType)
            {
                if (pstVpssChan->stPort[hPort].stBufVpssInst.pstUnAvailableListPos->next
                    != &pstVpssChan->stPort[hPort].stBufVpssInst.stVpssBufUnAvailableList)
                {
                    pstTarget = list_entry(pstVpssChan->stPort[hPort].stBufVpssInst.pstUnAvailableListPos->next, BUFMNG_VPSS_NODE_S, node);

                    if (NULL == pstTarget)
                    {
                        return HI_FAILURE;
                    }
                    else if (HI_TRUE == pstTarget->bBufUsed)
                    {
                        return HI_SUCCESS;
                    }
                }
                else
                {
                    return HI_FAILURE;
                }
            }
        }
    }
    return HI_SUCCESS;
}
#endif

static HI_VOID RecvVpssGetPortInfo(VDEC_VPSSCHANNEL_S *pstVpssChan,
                                   HI_HANDLE          *pMASTER,
                                   HI_HANDLE          *pSLAVE,
                                   HI_HANDLE          *pVIRTUAL)
{
    HI_S32  j = 0;

    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if (   (HI_TRUE == pstVpssChan->stPort[j].bEnable)
               && (HI_INVALID_HANDLE != pstVpssChan->stPort[j].hPort)
               && (HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE == pstVpssChan->stPort[j].bufferType) )
        {
            if (VDEC_PORT_TYPE_MASTER == pstVpssChan->stPort[j].enPortType)
            {
                *pMASTER = pstVpssChan->stPort[j].hPort;
            }

            if (VDEC_PORT_TYPE_SLAVE == pstVpssChan->stPort[j].enPortType)
            {
                *pSLAVE = pstVpssChan->stPort[j].hPort;
            }

            if (VDEC_PORT_TYPE_VIRTUAL == pstVpssChan->stPort[j].enPortType)
            {
                *pVIRTUAL = pstVpssChan->stPort[j].hPort;
            }
        }
    }

    return;
}

static HI_S32 RecvVpssMasterProcess( VDEC_VPSSCHANNEL_S *pstVpssChan,
                                     VDEC_CHANNEL_S *pstChan,
                                     HI_DRV_VIDEO_FRAME_PACKAGE_S  *pstFrm,
                                     HI_HANDLE hMASTER,
                                     HI_S32 i,
                                     HI_U32 TimeOut)

{
    HI_S32  s32Ret;
    HI_DRV_VPSS_PORT_AVAILABLE_S stCanGetFrm;

    stCanGetFrm.hPort = hMASTER;
    stCanGetFrm.bAvailable = HI_FALSE;
    s_stVdecDrv.pVpssFunc->pfnVpssSendCommand(pstVpssChan->hVpss,
            HI_DRV_VPSS_USER_COMMAND_CHECKAVAILABLE, &(stCanGetFrm));

    if (stCanGetFrm.bAvailable == HI_FALSE)
    {
        s_stVdecDrv.astChanEntity[i].pstChan->bWakeUp       = HI_FALSE;
        s_stVdecDrv.astChanEntity[i].pstChan->bMasterWakeUp = HI_FALSE;

        VDEC_WaitEvent(s_stVdecDrv.astChanEntity[i].pstChan->WaitQueue,
                       s_stVdecDrv.astChanEntity[i].pstChan->bMasterWakeUp || s_stVdecDrv.astChanEntity[i].pstChan->bWakeUp,
                       msecs_to_jiffies(TimeOut));

        if (HI_FALSE == s_stVdecDrv.astChanEntity[i].pstChan->bMasterWakeUp)
        {
            HI_INFO_VDEC("get master frame fail, timeout!\n");
            return HI_FAILURE;
        }
    }

    /*调用vpss接口获取主port数据*/
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortFrame)(hMASTER, &pstFrm->stFrame[pstFrm->u32FrmNum].stFrameVideo);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Get MainPort Frame err!\n");
        return HI_FAILURE;
    }

    pstFrm->stFrame[pstFrm->u32FrmNum].hport = hMASTER;
    memcpy(&(pstChan->stLastVpssFrm), &pstFrm->stFrame[pstFrm->u32FrmNum].stFrameVideo, sizeof(HI_DRV_VIDEO_FRAME_S));
    pstFrm->u32FrmNum++;

    return HI_SUCCESS;
}

static HI_S32 RecvVpssSlaveProcess(VDEC_VPSSCHANNEL_S *pstVpssChan,
                                    HI_DRV_VIDEO_FRAME_PACKAGE_S  *pstFrm,
                                    HI_HANDLE hSLAVE,
                                    HI_S32 i,
                                    HI_U32 TimeOut)

{
    HI_S32  s32Ret;
    HI_DRV_VPSS_PORT_AVAILABLE_S stCanGetFrm;

    stCanGetFrm.hPort = hSLAVE;
    stCanGetFrm.bAvailable = HI_FALSE;
    s_stVdecDrv.pVpssFunc->pfnVpssSendCommand(pstVpssChan->hVpss,
            HI_DRV_VPSS_USER_COMMAND_CHECKAVAILABLE, &(stCanGetFrm));

    if (stCanGetFrm.bAvailable == HI_FALSE)
    {
        s_stVdecDrv.astChanEntity[i].pstChan->bWakeUp      = HI_FALSE;
        s_stVdecDrv.astChanEntity[i].pstChan->bSlaveWakeUp = HI_FALSE;

        VDEC_WaitEvent(s_stVdecDrv.astChanEntity[i].pstChan->WaitQueue,
                       s_stVdecDrv.astChanEntity[i].pstChan->bSlaveWakeUp || s_stVdecDrv.astChanEntity[i].pstChan->bWakeUp,
                       msecs_to_jiffies(TimeOut));

        if (HI_FALSE == s_stVdecDrv.astChanEntity[i].pstChan->bSlaveWakeUp)
        {
            HI_INFO_VDEC("get slave frame fail, timeout!\n");
            return HI_FAILURE;
        }
    }

    /*调用vpss接口获取从port数据*/
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortFrame)(hSLAVE, &pstFrm->stFrame[pstFrm->u32FrmNum].stFrameVideo);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Get SlavePort Frame err!\n");
        return HI_FAILURE;
    }

    pstFrm->stFrame[pstFrm->u32FrmNum].hport = hSLAVE;
    pstFrm->u32FrmNum++;

    return HI_SUCCESS;
}

static HI_S32 RecvVpssOnlyVitureProcess(VDEC_VPSSCHANNEL_S *pstVpssChan,
                                         HI_DRV_VIDEO_FRAME_PACKAGE_S  *pstFrm,
                                         HI_HANDLE hVIRTUAL,
                                         HI_S32 i,
                                         HI_U32 TimeOut)

{
    HI_S32  s32Ret;
    HI_DRV_VPSS_PORT_AVAILABLE_S stCanGetFrm;

    stCanGetFrm.hPort = hVIRTUAL;
    stCanGetFrm.bAvailable = HI_FALSE;
    s_stVdecDrv.pVpssFunc->pfnVpssSendCommand(pstVpssChan->hVpss,
            HI_DRV_VPSS_USER_COMMAND_CHECKAVAILABLE, &(stCanGetFrm));

    if (stCanGetFrm.bAvailable == HI_FALSE)
    {
        s_stVdecDrv.astChanEntity[i].pstChan->bWakeUp        = HI_FALSE;
        s_stVdecDrv.astChanEntity[i].pstChan->bVirtualWakeUp = HI_FALSE;

        VDEC_WaitEvent(s_stVdecDrv.astChanEntity[i].pstChan->WaitQueue,
                       s_stVdecDrv.astChanEntity[i].pstChan->bVirtualWakeUp || s_stVdecDrv.astChanEntity[i].pstChan->bWakeUp,
                       msecs_to_jiffies(TimeOut));

        if (HI_FALSE == s_stVdecDrv.astChanEntity[i].pstChan->bVirtualWakeUp)
        {
            HI_INFO_VDEC("get virtual frame fail, timeout!\n");
            return HI_FAILURE;
        }
    }

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortFrame)(hVIRTUAL, &pstFrm->stFrame[pstFrm->u32FrmNum].stFrameVideo);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Get VirtualPort Frame err!\n");
        return HI_FAILURE;
    }

    pstFrm->stFrame[pstFrm->u32FrmNum].hport = hVIRTUAL;
    pstFrm->u32FrmNum++;

    return HI_SUCCESS;
}

static HI_S32 RecvVpssGetData(VDEC_VPSSCHANNEL_S *pstVpssChan,
                              HI_DRV_VIDEO_FRAME_PACKAGE_S  *pstFrm)
{
    HI_S32  j = 0;
    BUFMNG_VPSS_NODE_S *pstTarget = HI_NULL;

    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if ((HI_INVALID_HANDLE != pstVpssChan->stPort[j].hPort) && (HI_TRUE == pstVpssChan->stPort[j].bEnable))
        {
            if (HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == pstVpssChan->stPort[j].bufferType)
            {
                /*从vdec维护的队列中获取数据*/
                BUFMNG_SpinLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);

                if (pstVpssChan->stPort[j].stBufVpssInst.u32AvaiableFrameCnt <= 0)
                {
                    BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
                    return HI_FAILURE;
                }

                if ((pstVpssChan->stPort[j].stBufVpssInst.pstUnAvailableListPos->next) == &(pstVpssChan->stPort[j].stBufVpssInst.stVpssBufAvailableList))
                {
                    BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
                    return HI_FAILURE;
                }

                pstTarget = list_entry(pstVpssChan->stPort[j].stBufVpssInst.pstUnAvailableListPos->next, BUFMNG_VPSS_NODE_S, node);


                if (HI_DRV_VDEC_BUF_STATE_IN_USER == pstTarget->enFrameBufferState)
                {
                    BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
                    return HI_FAILURE;
                }

                pstTarget->enFrameBufferState = HI_DRV_VDEC_BUF_STATE_IN_USER;
                memcpy(&(pstFrm->stFrame[pstFrm->u32FrmNum].stFrameVideo), &(pstTarget->stVpssOutFrame), sizeof(HI_DRV_VIDEO_FRAME_S));
                pstVpssChan->stPort[j].stBufVpssInst.u32AvaiableFrameCnt --;
                //if(pstVpssChan->stPort[j].stBufVpssInst.pstUnAvailableListPos->next
                //    != &pstVpssChan->stPort[j].stBufVpssInst.stVpssBufUnAvailableList)
                {
                    pstVpssChan->stPort[j].stBufVpssInst.pstUnAvailableListPos = pstVpssChan->stPort[j].stBufVpssInst.pstUnAvailableListPos->next;
                }
                pstFrm->stFrame[pstFrm->u32FrmNum].hport = pstVpssChan->stPort[pstFrm->u32FrmNum].hPort;
                pstFrm->u32FrmNum++;
                BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
            }
        }
    }

    return HI_SUCCESS;
}
//add by l00225186
/*Avplay收帧，vdec直接在vpss中取*/
static HI_S32 VDEC_Chan_RecvVpssFrmBuf(HI_HANDLE hVpss, HI_DRV_VIDEO_FRAME_PACKAGE_S* pstFrm, HI_U32 TimeOut)
{
    HI_S32                      s32Ret;
    HI_S32                      i = 0;
    HI_HANDLE                   hMASTER  = HI_INVALID_HANDLE;
    HI_HANDLE                   hSLAVE   = HI_INVALID_HANDLE;
    HI_HANDLE                   hVIRTUAL = HI_INVALID_HANDLE;

    VDEC_CHANNEL_S*              pstChan = HI_NULL;
    VDEC_CHAN_STATINFO_S*        pstStatInfo = HI_NULL;
    VDEC_VPSSCHANNEL_S*          pstVpssChan = HI_NULL;
    HI_HANDLE                    hHandle = HI_INVALID_HANDLE;
    HI_DRV_VPSS_PORT_AVAILABLE_S stCanGetFrm;

    if ((HI_INVALID_HANDLE == hVpss) || (HI_NULL == pstFrm))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    s32Ret = VDEC_FindVdecHandleByVpssHandle(hVpss, &hHandle);
    VDEC_ASSERT_RETURN(s32Ret == HI_SUCCESS, HI_FAILURE);

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    pstStatInfo = &pstChan->stStatInfo;
    pstFrm->u32FrmNum = 0;

    /*find astChanEntity by vpss handle*/
    /*CNcomment:首先应该找到hVpss对应的是第几个astChanEntity*/
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (hVpss == s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss)
        {
            break;
        }
    }

    //VDEC_ASSERT_RETURN(i < HI_VDEC_MAX_INSTANCE_NEW, HI_FAILURE);
    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Can't find vpss :%s %d!\n", __FUNCTION__, __LINE__);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);
    VDEC_ASSERT_RETURN(s32Ret == HI_SUCCESS, HI_FAILURE);

    pstStatInfo->u32AvplayRcvFrameTry ++;
    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

    RecvVpssGetPortInfo(pstVpssChan, &hMASTER, &hSLAVE, &hVIRTUAL);

    if (hMASTER != HI_INVALID_HANDLE)
    {
        s32Ret = RecvVpssMasterProcess(pstVpssChan, pstChan, pstFrm, hMASTER, i, TimeOut);
        if (s32Ret != HI_SUCCESS)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
            return HI_FAILURE;
        }
    }

    if (hSLAVE != HI_INVALID_HANDLE)
    {
        s32Ret = RecvVpssSlaveProcess(pstVpssChan, pstFrm, hSLAVE, i, TimeOut);
        if (s32Ret != HI_SUCCESS)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
            return HI_FAILURE;
        }
    }

    /* Transcode scene, APP attach master and virtaul window */
    if ( (hMASTER != HI_INVALID_HANDLE) && (hVIRTUAL != HI_INVALID_HANDLE) )
    {
        stCanGetFrm.hPort = hVIRTUAL;
        stCanGetFrm.bAvailable = HI_FALSE;
        s_stVdecDrv.pVpssFunc->pfnVpssSendCommand(pstVpssChan->hVpss,
                HI_DRV_VPSS_USER_COMMAND_CHECKAVAILABLE, &(stCanGetFrm));

        if (HI_TRUE == stCanGetFrm.bAvailable)
        {
            s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortFrame)(hVIRTUAL, &pstFrm->stFrame[pstFrm->u32FrmNum].stFrameVideo);
            if (HI_SUCCESS == s32Ret)
            {
                pstFrm->stFrame[pstFrm->u32FrmNum].hport = hVIRTUAL;
                pstFrm->u32FrmNum++;
            }
        }
    }
    /*APP only attach virtual window*/
    else if ( (hMASTER == HI_INVALID_HANDLE) && (hVIRTUAL != HI_INVALID_HANDLE) )
    {
        s32Ret = RecvVpssOnlyVitureProcess(pstVpssChan, pstFrm, hVIRTUAL, i, TimeOut);
        if (s32Ret != HI_SUCCESS)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
            return HI_FAILURE;
        }
    }

    if (HI_SUCCESS != VDEC_Chan_AllPortHaveDate(hVpss))
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        return HI_FAILURE;
    }

    s32Ret = RecvVpssGetData(pstVpssChan, pstFrm);
    if (s32Ret != HI_SUCCESS)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        return HI_FAILURE;
    }

    if (0 == pstFrm->u32FrmNum)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        return HI_FAILURE;
    }

    pstStatInfo->u32AvplayRcvFrameOK++;
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_GetEsBuf(HI_HANDLE hHandle, VDEC_ES_BUF_S* pstEsBuf)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    VDEC_ES_BUF_PRI_S stEsBufPri = {0};

    if (HI_NULL == pstEsBuf)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    /* Get */
    memset(&stEsBufPri, 0, sizeof(stEsBufPri));
    stEsBufPri.u32BufSize = pstEsBuf->u32BufSize;
    s32Ret = VDEC_GetStrmBuf(pstChan->hStrmBuf, &stEsBufPri, HI_FALSE);

    if (HI_SUCCESS != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return s32Ret;
    }
    pstEsBuf->pu8Addr          = (HI_U8*)(HI_SIZE_T)stEsBufPri.u64Addr;
    pstEsBuf->u32PhyAddr       = stEsBufPri.u32PhyAddr;
    pstEsBuf->u32BufSize       = stEsBufPri.u32BufSize;
    pstEsBuf->u64Pts           = stEsBufPri.u64Pts;
    pstEsBuf->bEndOfFrame      = stEsBufPri.bEndOfFrame;
    pstEsBuf->bDiscontinuous   = stEsBufPri.bDiscontinuous;

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_PutEsBuf(HI_HANDLE hHandle, VDEC_ES_BUF_S* pstEsBuf)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    VDEC_ES_BUF_PRI_S stEsBufPri;

    if (HI_NULL == pstEsBuf)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    stEsBufPri.u64Addr         = (HI_U64)(HI_SIZE_T)pstEsBuf->pu8Addr;
    stEsBufPri.u32PhyAddr      = pstEsBuf->u32PhyAddr;
    stEsBufPri.u32BufSize      = pstEsBuf->u32BufSize;
    stEsBufPri.u64Pts          = pstEsBuf->u64Pts;
    stEsBufPri.bEndOfFrame     = pstEsBuf->bEndOfFrame;
    stEsBufPri.bDiscontinuous  = pstEsBuf->bDiscontinuous;
    s32Ret = VDEC_PutStrmBuf(pstChan->hStrmBuf, &stEsBufPri, HI_FALSE);

    if (HI_SUCCESS != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return s32Ret;
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}

static HI_VOID VDEC_GetColourInfo(IMAGE* pstImage, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    pstFrame->stColourInfo.u8FullRangeFlag   = pstImage->ColourInfo.FullRangeFlag;
    pstFrame->stColourInfo.u8ColourPrimaries = pstImage->ColourInfo.ColourPrimaries;
    pstFrame->stColourInfo.u8MatrixCoeffs    = pstImage->ColourInfo.MatrixCoeffs;
}

#if 0 //when AVS2.0 support, open this function
HI_DRV_VIDEO_FRAME_TYPE_E VDEC_CheckAVS2HDRType(UINT32 TransferCharacteristics)
{
    HI_DRV_VIDEO_FRAME_TYPE_E   enSrcFrameType;

    switch (TransferCharacteristics)
    {
        case 12:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_HDR10;
            break;

        case 13:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_SLF;
            break;

        case 14:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_HLG;
            break;

        default:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_SDR;
            break;
    }

    return enSrcFrameType;
}

static HI_VOID VDEC_GetAVS2HDRInfo(IMAGE* pstImage, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    pstFrame->unHDRInfo.stSLFInfo.ColourPrimaries = pstImage->ColourPrimaries;
    pstFrame->unHDRInfo.stSLFInfo.MatrixCoeffs    = pstImage->MatrixCoeffs;

    return;
}
#endif

HI_DRV_VIDEO_FRAME_TYPE_E VDEC_CheckHEVCHDRType(IMAGE* pstImage, HI_U8 *pCompatible)
{
    HI_DRV_VIDEO_FRAME_TYPE_E   enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_SDR;

    if (pstImage->TechnicolorInfo.IsValid == 1)
    {
        enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_TECHNICOLOR;

        return enSrcFrameType;
    }

    switch (pstImage->TransferCharacteristics)
    {
        case 14:
            if (pstImage->ComapitibilityInfo.HDRTransferCharacteristicIdc == 18)
            {
                enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_HLG;
                *pCompatible = 1;
            }
            else
            {
                enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_SDR;
            }

            break;

        case 16:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_HDR10;
            break;

        case 18:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_HLG;
            *pCompatible = 0;
            break;

        default:
            enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_SDR;
            break;

    }

    return enSrcFrameType;
}

static HI_VOID VDEC_GetHEVCHDRInfo(IMAGE* pstImage, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    if (pstFrame->enSrcFrameType == HI_DRV_VIDEO_FRAME_TYPE_HDR10)
    {
        VDEC_GetColourInfo(pstImage, pstFrame);
        pstFrame->unHDRInfo.stHDR10Info.u8TransferCharacteristics = pstImage->TransferCharacteristics;

        if (pstImage->MasteringAvailable == 1)
        {
            pstFrame->unHDRInfo.stHDR10Info.u8MasteringAvailable = pstImage->MasteringAvailable;
            memcpy(&(pstFrame->unHDRInfo.stHDR10Info.stMasteringInfo),
                   &(pstImage->MasteringDisplayColourVolume),
                   sizeof(MASTERING_DISPLAY_COLOUR_VOLUME_S));
        }

        if (pstImage->ContentAvailable == 1)
        {
            pstFrame->unHDRInfo.stHDR10Info.u8ContentAvailable = pstImage->ContentAvailable;
            memcpy(&(pstFrame->unHDRInfo.stHDR10Info.stContentInfo),
                   &(pstImage->ContentLightLevel),
                   sizeof(CONTENT_LIGHT_LEVEL_INFO_S));
        }
    }
    else if (pstFrame->enSrcFrameType == HI_DRV_VIDEO_FRAME_TYPE_HLG)
    {
        VDEC_GetColourInfo(pstImage, pstFrame);
    }
    else if (pstFrame->enSrcFrameType == HI_DRV_VIDEO_FRAME_TYPE_TECHNICOLOR)
    {
        if (pstImage->TechnicolorInfo.IsValid == 1)
        {
            memcpy(&(pstFrame->unHDRInfo.stTechnicolorInfo.stHDR_StaticParms),
                   &(pstImage->TechnicolorInfo.TCH_HDR_StaticParms),
                   sizeof(TECHNICOLOR_HDR_SEQ_PARAMS));
            memcpy(&(pstFrame->unHDRInfo.stTechnicolorInfo.stHDR_DanamicParms),
                   &(pstImage->TechnicolorInfo.TCH_HDR_DanamicParms),
                   sizeof(TECHNICOLOR_HDR_FRM_PARAMS));
        }
    }

    return;
}

static HI_VOID VDEC_SetHDRInfo(HI_UNF_VCODEC_TYPE_E enType, IMAGE* pstImage, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    HI_U8 IsBackwardsCompatible = 0;

    switch (enType)
    {
#if 0 //when AVS2.0 support, open this function
        case HI_UNF_VCODEC_TYPE_AVS:
            pstFrame->enCodecType = HI_DRV_STD_AVS2;
            pstFrame->enSrcFrameType = VDEC_CheckAVS2HDRType(pstImage->TransferCharacteristics);
            VDEC_GetAVS2HDRInfo(pstImage, pstFrame);

            break;
#endif

        case HI_UNF_VCODEC_TYPE_VP9:
            pstFrame->enCodecType = HI_DRV_STD_VP9;
            //todo

            break;

        case HI_UNF_VCODEC_TYPE_HEVC:
            pstFrame->enCodecType = HI_DRV_STD_HEVC;
            pstFrame->enSrcFrameType = VDEC_CheckHEVCHDRType(pstImage, &IsBackwardsCompatible);
            pstFrame->unHDRInfo.stHLGInfo.IsBackwardsCompatible = IsBackwardsCompatible;
            VDEC_GetHEVCHDRInfo(pstImage, pstFrame);

            break;

        default :
            pstFrame->enCodecType = HI_DRV_STD_START_RESERVED;
            //do nothing

            break;
    }

    return;
}

HI_DRV_COLOR_SPACE_E VDEC_GetColorSpace(HI_UNF_VCODEC_TYPE_E enType, IMAGE* pstImage)
{
    HI_DRV_COLOR_SPACE_E ColorSpace = HI_DRV_CS_UNKNOWN;

    switch (enType)
    {
        case HI_UNF_VCODEC_TYPE_HEVC:
            if (pstImage->ColourInfo.ColourPrimaries == 9)
            {
                ColorSpace = HI_DRV_CS_BT2020_YUV_LIMITED;
            }

            break;

        default:

            break;
    }

    return ColorSpace;
}

static HI_VOID ConvertFrm_GetCmpInfo(HI_VDEC_PRIV_FRAMEINFO_S* pstPrivInfo, IMAGE* pstImage)
{
    /* Image compress flag to frame flag */
#if 1
    pstPrivInfo->stCompressInfo.u32CompressFlag = (1 == pstImage->BTLInfo.u32IsCompress) ? 1 : 0;
#else
    pstPrivInfo->stCompressInfo.u32CompressFlag = (1 == pstImage->ImageDnr.s32VcmpEn) ? 1 : 0;
#endif

    if (0 == (pstImage->ImageDnr.s32VcmpFrameHeight % 16))
    {
        pstPrivInfo->stCompressInfo.s32CompFrameHeight = pstImage->ImageDnr.s32VcmpFrameHeight;
    }
    else
    {
        HI_WARN_VDEC("s32CompFrameHeight err!\n");
        pstPrivInfo->stCompressInfo.s32CompFrameHeight = 0;
    }

    if (0 == (pstImage->ImageDnr.s32VcmpFrameWidth % 16))
    {
        pstPrivInfo->stCompressInfo.s32CompFrameWidth = pstImage->ImageDnr.s32VcmpFrameWidth;
    }
    else
    {
        HI_WARN_VDEC("s32CompFrameWidth err!\n");
        pstPrivInfo->stCompressInfo.s32CompFrameWidth = 0;
    }

    return;
}

static const HI_U32 g_TypeMjpg_Table[][2] =
{
    {SPYCbCr400,          HI_DRV_PIX_FMT_NV08},
    {SPYCbCr411,          HI_DRV_PIX_FMT_NV12_411},
    {SPYCbCr422_1X2,      HI_DRV_PIX_FMT_NV16},
    {SPYCbCr422_2X1,      HI_DRV_PIX_FMT_NV61_2X1},
    {SPYCbCr444,          HI_DRV_PIX_FMT_NV24},
    {PLNYCbCr400,         HI_DRV_PIX_FMT_YUV400},
    {PLNYCbCr411,         HI_DRV_PIX_FMT_YUV411},
    {PLNYCbCr420,         HI_DRV_PIX_FMT_YUV420p},
    {PLNYCbCr422_1X2,     HI_DRV_PIX_FMT_YUV422_1X2},
    {PLNYCbCr422_2X1,     HI_DRV_PIX_FMT_YUV422_2X1},
    {PLNYCbCr444,         HI_DRV_PIX_FMT_YUV_444},
    {PLNYCbCr410,         HI_DRV_PIX_FMT_YUV410p},
    {SPYCbCr420,          HI_DRV_PIX_FMT_NV21},

    {YCbCrBUTT,           HI_DRV_PIX_BUTT}, //terminal element n
};

HI_U32 TypeMjpgConvert(HI_U32 Code)
{
    HI_U32 Index = 0;
    HI_U32 ConvertedCode = HI_DRV_PIX_FMT_NV21;

    while(1)
    {
        if (g_TypeMjpg_Table[Index][0] == YCbCrBUTT || g_TypeMjpg_Table[Index][1] == HI_DRV_PIX_BUTT)
        {
            break;
        }

        if (Code == g_TypeMjpg_Table[Index][0])
        {
            ConvertedCode = g_TypeMjpg_Table[Index][1];
            break;
        }

        Index++;
    }

    return ConvertedCode;
}

static HI_VOID ConvertFrm_GetPixFormat(HI_UNF_VCODEC_TYPE_E enType,
                                       IMAGE* pstImage,
                                       HI_DRV_VIDEO_FRAME_S* pstFrame)
{

    if (HI_UNF_VCODEC_TYPE_H263 == enType || HI_UNF_VCODEC_TYPE_SORENSON == enType)
    {
        pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21;
    }
    else if (HI_UNF_VCODEC_TYPE_MJPEG == enType)
    {
        pstFrame->ePixFormat = TypeMjpgConvert(pstImage->BTLInfo.YUVFormat);
    }
    else
    {
        if (0 == EnVcmp)
        {
            switch ((pstImage->format >> 17) & 0x1)  /*1d  or 2d*/
            {
                case 0:
                    pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21_TILE;
                    break;

                case 1:
                    pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21;
                    break;

                default:
                    pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21_TILE;
                    break;
            }
        }
        else
        {
            switch ((pstImage->format >> 17) & 0x1)  /*1d  or 2d*/
            {
                case 0:
                    if ((pstImage->format >> 18) & 0x1) /* raw or cmp*/
                    {
                        pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21_TILE_CMP;
                    }
                    else
                    {
                        pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21_TILE;
                    }

                    break;

                case 1:
                    if ((pstImage->format >> 18) & 0x1) /* raw or cmp*/
                    {
                        pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21_CMP;
                    }
                    else
                    {
                        pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21;
                    }

                    break;

                default:
                    pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21_TILE_CMP;
                    break;
            }
        }
    }

    return;
}

static HI_VOID ConvertFrm_GetDisplayNorm(VDEC_CHANNEL_S* pstChan, IMAGE* pstImage)
{
    switch (pstImage->format & 0xE0)
    {
        case 0x20:
            pstChan->enDisplayNorm = HI_UNF_ENC_FMT_PAL;
            break;

        case 0x40:
            pstChan->enDisplayNorm = HI_UNF_ENC_FMT_NTSC;
            break;

        default:
            pstChan->enDisplayNorm = HI_UNF_ENC_FMT_BUTT;
            break;
    }

    return;
}

static HI_VOID ConvertFrm_GetProgressiveInfo(VDEC_CHANNEL_S* pstChan,
                                             IMAGE* pstImage,
                                             HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    switch (pstImage->format & 0x300)
    {
        case 0x0: /* PROGRESSIVE */
            pstFrame->bProgressive = HI_TRUE;
            pstChan->stStatInfo.u32FrameType[1]++;
            break;

        case 0x100: /* INTERLACE */
        case 0x200: /* INFERED_PROGRESSIVE */
        case 0x300: /* INFERED_INTERLACE */
        default:
            pstFrame->bProgressive = HI_FALSE;
            pstChan->stStatInfo.u32FrameType[0]++;
            break;
    }

    return;
}

static HI_VOID ConvertFrm_GetSampleType(HI_DRV_VIDEO_PRIVATE_S* pstVideoPriv,
                                        IMAGE* pstImage)
{
    switch (pstImage->format & 0x300)
    {
        case 0x0: /* PROGRESSIVE */
            pstVideoPriv->eSampleType = HI_DRV_SAMPLE_TYPE_PROGRESSIVE;
            break;

        case 0x100: /* INTERLACE */
            pstVideoPriv->eSampleType = HI_DRV_SAMPLE_TYPE_INTERLACE;
            break;

        case 0x200: /* INFERED_PROGRESSIVE */
        case 0x300: /* INFERED_INTERLACE */
        default:
            pstVideoPriv->eSampleType = HI_DRV_SAMPLE_TYPE_UNKNOWN;

            break;
    }

    return;
}

static HI_VOID ConvertFrm_GetFieldMode(IMAGE* pstImage,
                                        HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    switch (pstImage->format & 0xC00)
    {
        case 0x400:
            pstFrame->enFieldMode = HI_DRV_FIELD_TOP;
            break;

        case 0x800:
            pstFrame->enFieldMode = HI_DRV_FIELD_BOTTOM;
            break;

        case 0xC00:
            pstFrame->enFieldMode = HI_DRV_FIELD_ALL;
            break;

        default:
            pstFrame->enFieldMode = HI_DRV_FIELD_BUTT;
            break;
    }

    return;
}

static HI_VOID ConvertFrm_GetBitWidth(HI_UNF_VCODEC_TYPE_E enType,
                                      IMAGE* pstImage,
                                      HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    if (HI_UNF_VCODEC_TYPE_HEVC == enType || HI_UNF_VCODEC_TYPE_VP9 == enType)
    {
        if ((pstImage->bit_depth_luma > 8) || (pstImage->bit_depth_chroma > 8))
        {
            pstFrame->enBitWidth =  HI_DRV_PIXEL_BITWIDTH_10BIT;
        }
        else
        {
            pstFrame->enBitWidth =  HI_DRV_PIXEL_BITWIDTH_8BIT;
        }

    }
    else
    {
        pstFrame->enBitWidth =  HI_DRV_PIXEL_BITWIDTH_8BIT;
    }


    return;
}

static HI_VOID ConvertFrm_GetBufAddr(HI_UNF_VCODEC_TYPE_E enType,
                                      IMAGE* pstImage,
                                      HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    if (HI_UNF_VCODEC_TYPE_MJPEG == enType
        || HI_UNF_VCODEC_TYPE_SORENSON == enType
        || HI_UNF_VCODEC_TYPE_H263 == enType)
    {
        pstFrame->stBufAddr[0].u32PhyAddr_Y        = pstImage->top_luma_phy_addr;
        pstFrame->stBufAddr[0].u32Stride_Y         = pstImage->image_stride;
        pstFrame->stBufAddr[0].u32PhyAddr_YHead    = pstImage->BTLInfo.u32YHeadAddr;
        pstFrame->stBufAddr[0].u32PhyAddr_C        = pstImage->top_chrom_phy_addr;
        pstFrame->stBufAddr[0].u32Stride_C         = pstFrame->stBufAddr[0].u32Stride_Y;
        pstFrame->stBufAddr[0].u32PhyAddr_CHead    = pstImage->BTLInfo.u32CHeadAddr;
        pstFrame->stBufAddr[0].u32PhyAddr_Cr       = pstImage->BTLInfo.u32CrAddr;
        pstFrame->stBufAddr[0].u32Stride_Cr        = pstImage->BTLInfo.u32CrStride;
        pstFrame->stBufAddr[0].u32PhyAddr_CrHead   = pstImage->BTLInfo.u32CHeadAddr;
        pstFrame->stBufAddr[0].vir_addr_y          = pstImage->luma_vir_addr;
        pstFrame->stBufAddr[0].vir_addr_c          = pstImage->chrom_vir_addr;
    }
    else
    {
        if ((0 == EnVcmp) || (((pstImage->format >> 18) & 0x1) == 0))
        {
            pstFrame->stBufAddr[0].u32PhyAddr_YHead    = pstImage->top_luma_phy_addr;
            pstFrame->stBufAddr[0].u32PhyAddr_Y        = pstImage->top_luma_phy_addr;
            pstFrame->stBufAddr[0].vir_addr_y          = pstImage->luma_vir_addr;

            pstFrame->stBufAddr[0].u32Stride_Y         = pstImage->image_stride / 16;

            pstFrame->stBufAddr[0].u32PhyAddr_CHead    = pstImage->top_chrom_phy_addr;
            pstFrame->stBufAddr[0].u32PhyAddr_C        = pstImage->top_chrom_phy_addr;
            pstFrame->stBufAddr[0].vir_addr_c          = pstImage->chrom_vir_addr;

            pstFrame->stBufAddr[0].u32Stride_C         = pstFrame->stBufAddr[0].u32Stride_Y;
        }
        else
        {
            pstFrame->stBufAddr[0].u32PhyAddr_YHead    = pstImage->top_luma_phy_addr;
            pstFrame->stBufAddr[0].vir_addr_y          = pstImage->luma_vir_addr;
            pstFrame->stBufAddr[0].u32PhyAddr_Y        = pstImage->top_luma_phy_addr + pstImage->head_info_size;
            pstFrame->stBufAddr[0].u32Stride_Y         = pstImage->image_stride / 16;
            pstFrame->stBufAddr[0].u32PhyAddr_CHead    = pstImage->top_chrom_phy_addr;
            pstFrame->stBufAddr[0].vir_addr_c          = pstImage->chrom_vir_addr;
            pstFrame->stBufAddr[0].u32PhyAddr_C        = pstImage->top_chrom_phy_addr + (pstImage->head_info_size / 2);
            pstFrame->stBufAddr[0].u32Stride_C         = pstFrame->stBufAddr[0].u32Stride_Y;
            pstFrame->stBufAddr[0].u32Head_Stride      = pstImage->head_stride;
            pstFrame->stBufAddr[0].u32Head_Size        = pstImage->head_info_size;
        }

        if (HI_DRV_PIXEL_BITWIDTH_10BIT == pstFrame->enBitWidth)
        {
            pstFrame->stBufAddr_LB[0].u32PhyAddr_Y = pstImage->luma_2bit_phy_addr;
            pstFrame->stBufAddr_LB[0].u32PhyAddr_C = pstImage->chrom_2bit_phy_addr;

#if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
            pstFrame->stBufAddr_LB[0].u32Stride_Y  = pstImage->image_stride_2bit / 16;
            pstFrame->stBufAddr_LB[0].u32Stride_C  = pstImage->image_stride_2bit / 16;
#else
            pstFrame->stBufAddr_LB[0].u32Stride_Y  = pstImage->image_stride_2bit / 32;
            pstFrame->stBufAddr_LB[0].u32Stride_C  = pstImage->image_stride_2bit / 32;
#endif
        }
    }

    //FOR MVC DEBUG
    if (pstImage->is_3D)
    {
        pstFrame->stBufAddr[1].u32PhyAddr_YHead       = pstImage->top_luma_phy_addr_1;
        pstFrame->stBufAddr[1].u32PhyAddr_Y           = pstImage->top_luma_phy_addr_1 + pstImage->head_info_size;
        pstFrame->stBufAddr[1].u32Stride_Y            = pstImage->image_stride / 16;

        pstFrame->stBufAddr[1].u32PhyAddr_CHead       = pstImage->top_chrom_phy_addr_1;
        pstFrame->stBufAddr[1].u32PhyAddr_C           = pstImage->top_chrom_phy_addr_1 + (pstImage->head_info_size / 2);
        pstFrame->stBufAddr[1].u32Stride_C            = pstFrame->stBufAddr[1].u32Stride_Y;

        pstFrame->stBufAddr[1].u32Head_Stride         = pstImage->head_stride;
        pstFrame->stBufAddr[1].u32Head_Size           = pstImage->head_info_size;

        if ((0 == EnVcmp) || (((pstImage->format >> 18) & 0x1) == 0))
        {
            pstFrame->stBufAddr[1].u32PhyAddr_Y       = pstImage->top_luma_phy_addr_1;
            pstFrame->stBufAddr[1].u32PhyAddr_C       = pstImage->top_chrom_phy_addr_1;
        }
    }

    return;
}

static HI_VOID ConvertFrm_GetVP6ExtAttr(HI_UNF_VCODEC_TYPE_E enType,
                                          VDEC_CHANNEL_S* pstChan,
                                          HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    if ((HI_UNF_VCODEC_TYPE_VP6 == enType) || (HI_UNF_VCODEC_TYPE_VP6F == enType) || (HI_UNF_VCODEC_TYPE_VP6A == enType))
    {
        if (HI_UNF_VCODEC_TYPE_VP6A == enType)
        {
            pstFrame->u32Circumrotate = pstChan->stCurCfg.unExtAttr.stVP6Attr.bReversed & 0x1;
        }
        else
        {
            pstFrame->u32Circumrotate = !(pstChan->stCurCfg.unExtAttr.stVP6Attr.bReversed & 0x1);
        }

        if ((pstChan->stCurCfg.unExtAttr.stVP6Attr.u32DispWidth != 0)
            && (pstChan->stCurCfg.unExtAttr.stVP6Attr.u32DispHeight!= 0))
        {
            pstFrame->u32Width = pstChan->stCurCfg.unExtAttr.stVP6Attr.u32DispWidth;
            pstFrame->u32Height = pstChan->stCurCfg.unExtAttr.stVP6Attr.u32DispHeight;
            pstFrame->stDispRect.s32Width = pstChan->stCurCfg.unExtAttr.stVP6Attr.u32DispWidth;
            pstFrame->stDispRect.s32Height = pstChan->stCurCfg.unExtAttr.stVP6Attr.u32DispHeight;
        }
    }
    else
    {
        pstFrame->u32Circumrotate = 0;
    }

    return;
}

static HI_VOID ConvertFrm_GetFrmType(HI_UNF_VCODEC_TYPE_E enType,
                                     IMAGE* pstImage,
                                     HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    /* vfmw解码出来的FP信息不外透，统一由上层设置
       MVC传递的是两帧地址，还是保留 */
    if (HI_UNF_VCODEC_TYPE_MVC == enType)
    {
        switch (pstImage->eFramePackingType)
        {
            case FRAME_PACKING_TYPE_NONE:
                pstFrame->eFrmType = HI_DRV_FT_NOT_STEREO;
                break;

            case FRAME_PACKING_TYPE_SIDE_BY_SIDE:
                pstFrame->eFrmType = HI_DRV_FT_SBS;
                break;

            case FRAME_PACKING_TYPE_TOP_BOTTOM:
                pstFrame->eFrmType = HI_DRV_FT_TAB;
                break;

            case FRAME_PACKING_TYPE_TIME_INTERLACED:
                pstFrame->eFrmType = HI_DRV_FT_FPK;
                break;

            default:
                pstFrame->eFrmType = FRAME_PACKING_TYPE_BUTT;
                break;
        }
    }

    return;
}

static HI_VOID ConvertFrm_GetFrameRate(VDEC_CHANNEL_S* pstChan,
                                       IMAGE* pstImage,
                                       HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    UINT32 u32fpsInteger, u32fpsDecimal;

    switch (pstChan->stFrameRateParam.enFrmRateType)
    {
        case HI_UNF_AVPLAY_FRMRATE_TYPE_USER:
            u32fpsInteger = pstChan->stFrameRateParam.stSetFrmRate.u32fpsInteger;
            u32fpsDecimal = pstChan->stFrameRateParam.stSetFrmRate.u32fpsDecimal;
            //pstFrame->stFrameRate.u32fpsInteger = pstChan->stFrameRateParam.stSetFrmRate.u32fpsInteger;
            //pstFrame->stFrameRate.u32fpsDecimal = pstChan->stFrameRateParam.stSetFrmRate.u32fpsDecimal;stSetFrmRate.u32fpsDecimal;

            break;

        case HI_UNF_AVPLAY_FRMRATE_TYPE_PTS:
        case HI_UNF_AVPLAY_FRMRATE_TYPE_USER_PTS:
        case HI_UNF_AVPLAY_FRMRATE_TYPE_STREAM:
        default:
            u32fpsInteger = pstImage->frame_rate / 1024;
            u32fpsDecimal = pstImage->frame_rate % 1024;
            //pstFrame->stFrameRate.u32fpsInteger = pstImage->frame_rate/1024;
            //pstFrame->stFrameRate.u32fpsDecimal = (pstImage->frame_rate*1000/1024)%1000;

            break;
    }

    pstFrame->u32FrameRate = u32fpsInteger * 1000 + u32fpsDecimal;

    /*60.0~60.5fps is setted to 60fps just for CES*/
    if ((pstFrame->u32FrameRate >= 60000) && (pstFrame->u32FrameRate < 60500))
    {
        pstFrame->u32FrameRate = 60000;
    }

    if (pstFrame->u32FrameRate < 1000)
    {
        pstFrame->u32FrameRate = 25000;
    }

    return;
}

static HI_VOID ConvertFrm_GetTunnelPhyAddr(VDEC_CHANNEL_S* pstChan,
                                           IMAGE* pstImage,
                                           HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    if (HI_TRUE == pstChan->bLowdelay)
    {
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)

        if (HI_UNF_VCODEC_TYPE_HEVC == pstChan->stCurCfg.enType)
        {
            pstFrame->u32TunnelPhyAddr = 0;
        }
        else
        {
            pstFrame->u32TunnelPhyAddr = pstImage->line_num_phy_addr;
        }

#else
        pstFrame->u32TunnelPhyAddr = pstImage->line_num_phy_addr;
        pstFrame->u64TunnelVirAddr = pstImage->line_num_vir_addr;
#endif
    }
    else
    {
        pstFrame->u32TunnelPhyAddr = 0;
    }

    return;
}



static HI_VOID VDEC_ConvertFrm(HI_UNF_VCODEC_TYPE_E enType, VDEC_CHANNEL_S* pstChan,
                               IMAGE* pstImage, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    HI_VDEC_PRIV_FRAMEINFO_S* pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S*)(((HI_DRV_VIDEO_PRIVATE_S*)(pstFrame->u32Priv))->u32Reserve);
    HI_DRV_VIDEO_PRIVATE_S* pstVideoPriv = (HI_DRV_VIDEO_PRIVATE_S*)(pstFrame->u32Priv);

    pstFrame->bTopFieldFirst = (pstImage->format & 0x3000) != 0 ? HI_TRUE : HI_FALSE;

    ConvertFrm_GetCmpInfo(pstPrivInfo, pstImage);
    ConvertFrm_GetPixFormat(enType, pstImage, pstFrame);
    ConvertFrm_GetDisplayNorm(pstChan, pstImage);
    ConvertFrm_GetProgressiveInfo(pstChan, pstImage, pstFrame);
    ConvertFrm_GetSampleType(pstVideoPriv, pstImage);
    ConvertFrm_GetFieldMode(pstImage, pstFrame);

    if (enType == HI_UNF_VCODEC_TYPE_HEVC)
    {
        pstFrame->enBufValidMode = HI_DRV_FIELD_ALL;
    }
    else if (enType == HI_UNF_VCODEC_TYPE_H264)
    {
        pstFrame->enBufValidMode = pstFrame->enFieldMode;
    }

    pstFrame->u32Width                         = (HI_U32)pstImage->disp_width;
    pstFrame->u32Height                        = (HI_U32)pstImage->disp_height;
    pstFrame->stDispRect.s32Width              = (HI_S32)pstImage->disp_width;
    pstFrame->stDispRect.s32Height             = (HI_S32)pstImage->disp_height;

    ConvertFrm_GetBitWidth(enType, pstImage, pstFrame);
    ConvertFrm_GetBufAddr(enType, pstImage, pstFrame);
    ConvertFrm_GetVP6ExtAttr(enType, pstChan, pstFrame);

    pstFrame->u32AspectWidth                   = pstImage->u32AspectWidth;
    pstFrame->u32AspectHeight                  = pstImage->u32AspectHeight;
    pstFrame->stDispRect.s32X                  = 0;
    pstFrame->stDispRect.s32Y                  = 0;
    pstFrame->u32ErrorLevel                    = pstImage->error_level;
    pstFrame->u32SrcPts                        = (HI_U32)pstImage->SrcPts;
    pstFrame->u32Pts                           = (HI_U32)pstImage->PTS;
    //pstFrame->u32FrmCnt                        = pstImage->seq_img_cnt;
    pstFrame->u32FrameIndex                    = pstImage->seq_img_cnt;
    pstChan->u32DecodeAspectWidth              = pstImage->u32AspectWidth;
    pstChan->u32DecodeAspectHeight             = pstImage->u32AspectHeight;
    pstChan->u32LastLumaBitdepth               = pstImage->bit_depth_luma;
    pstChan->u32LastChromaBitdepth             = pstImage->bit_depth_chroma;

    ConvertFrm_GetFrmType(enType, pstImage, pstFrame);

    pstPrivInfo->image_id       = pstImage->image_id;
    pstPrivInfo->image_id_1     = pstImage->image_id_1;
    pstPrivInfo->u32SeqFrameCnt = pstImage->seq_img_cnt;
    pstPrivInfo->u8Repeat       = !(pstImage->format & 0x80000);  /* control vo discard frame bit19 */
    pstPrivInfo->u8TestFlag     = pstImage->optm_inf.Rwzb;
    pstPrivInfo->u8EndFrame     = pstImage->last_frame;
    pstPrivInfo->s32FrameFormat = pstImage->format & 0x3;
    pstPrivInfo->s32TopFieldFrameFormat = pstImage->top_fld_type & 0x03;
    pstPrivInfo->s32BottomFieldFrameFormat = pstImage->bottom_fld_type & 0x03;
    pstPrivInfo->s32FieldFlag      = pstImage->is_fld_save;

    pstPrivInfo->s32GopNum      = pstImage->GopNum;

    /* For VC1 */
    if (HI_UNF_VCODEC_TYPE_VC1 == enType)
    {
        pstPrivInfo->u32BeVC1 = HI_TRUE;
        pstPrivInfo->stVC1RangeInfo.u8PicStructure = pstImage->ImageDnr.pic_structure;
        pstPrivInfo->stVC1RangeInfo.u8PicQPEnable = pstImage->ImageDnr.use_pic_qp_en;
        pstPrivInfo->stVC1RangeInfo.s32QPY = pstImage->ImageDnr.QP_Y;
        pstPrivInfo->stVC1RangeInfo.s32QPU = pstImage->ImageDnr.QP_U;
        pstPrivInfo->stVC1RangeInfo.s32QPV = pstImage->ImageDnr.QP_V;
        pstPrivInfo->stVC1RangeInfo.u8ChromaFormatIdc = pstImage->ImageDnr.chroma_format_idc;
        pstPrivInfo->stVC1RangeInfo.u8VC1Profile = pstImage->ImageDnr.vc1_profile;
        pstPrivInfo->stVC1RangeInfo.s32RangedFrm = pstImage->ImageDnr.Rangedfrm;
        pstPrivInfo->stVC1RangeInfo.u8RangeMapYFlag = pstImage->ImageDnr.Range_mapy_flag;
        pstPrivInfo->stVC1RangeInfo.u8RangeMapY = pstImage->ImageDnr.Range_mapy;
        pstPrivInfo->stVC1RangeInfo.u8RangeMapUVFlag = pstImage->ImageDnr.Range_mapuv_flag;
        pstPrivInfo->stVC1RangeInfo.u8RangeMapUV = pstImage->ImageDnr.Range_mapuv;
        pstPrivInfo->stVC1RangeInfo.u8BtmRangeMapYFlag = pstImage->ImageDnr.bottom_Range_mapy_flag;
        pstPrivInfo->stVC1RangeInfo.u8BtmRangeMapY = pstImage->ImageDnr.bottom_Range_mapy;
        pstPrivInfo->stVC1RangeInfo.u8BtmRangeMapUVFlag = pstImage->ImageDnr.bottom_Range_mapuv_flag;
        pstPrivInfo->stVC1RangeInfo.u8BtmRangeMapUV = pstImage->ImageDnr.bottom_Range_mapuv;
    }
    else
    {
        pstPrivInfo->u32BeVC1 = HI_FALSE;
    }

    //pstFrame->stVideoFrameAddr[0].u32CrAddr = pstImage->BTLInfo.u32CrAddr;
    //pstFrame->stVideoFrameAddr[0].u32CrStride = pstImage->BTLInfo.u32CrStride;
    pstPrivInfo->stCompressInfo.u32HeadOffset = pstImage->BTLInfo.u32HeadOffset;
    pstPrivInfo->stCompressInfo.u32YHeadAddr = pstImage->BTLInfo.u32YHeadAddr;
    pstPrivInfo->stCompressInfo.u32CHeadAddr = pstImage->BTLInfo.u32CHeadAddr;
    pstPrivInfo->stCompressInfo.u32HeadStride = pstImage->BTLInfo.u32HeadStride;
    pstPrivInfo->stBTLInfo.u32BTLImageID = pstImage->BTLInfo.btl_imageid;
    pstPrivInfo->stBTLInfo.u32Is1D = pstImage->BTLInfo.u32Is1D;
    pstPrivInfo->stBTLInfo.u32IsCompress = pstImage->BTLInfo.u32IsCompress;
    pstPrivInfo->stBTLInfo.u32DNROpen = pstImage->BTLInfo.u32DNROpen;
    pstPrivInfo->stBTLInfo.u32DNRInfoAddr = pstImage->BTLInfo.u32DNRInfoAddr;
    pstPrivInfo->stBTLInfo.u32DNRInfoStride = pstImage->BTLInfo.u32DNRInfoStride;

    if ((pstFrame->u32Height <= 288) && (enType != HI_UNF_VCODEC_TYPE_HEVC))
    {
        pstFrame->bProgressive = HI_TRUE;
    }

    ConvertFrm_GetFrameRate(pstChan, pstImage, pstFrame);
    ConvertFrm_GetTunnelPhyAddr(pstChan, pstImage, pstFrame);

#ifdef HI_HDR_SUPPORT
    /* HDR info*/
    if (pstChan->stVdecHdrAttr.AvplayHDRAttr.bEnable)
    {
        if (HI_VDEC_HDR_CHAN_TYPE_BL == pstChan->stVdecHdrAttr.enVdecHDRType)
        {
            pstFrame->enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_DOLBY_BL;
        }
        else if (HI_VDEC_HDR_CHAN_TYPE_EL == pstChan->stVdecHdrAttr.enVdecHDRType)
        {
            pstFrame->enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_DOLBY_EL;
        }

        if (pstImage->HDR_Metadata_size > 0)
        {
            pstFrame->unHDRInfo.stDolbyInfo.bMetadataValid = 1;
        }
        else
        {
            pstFrame->unHDRInfo.stDolbyInfo.bMetadataValid = 0;
        }
        pstFrame->unHDRInfo.stDolbyInfo.stMetadata.u32Addr = pstImage->HDR_Metadata_phy_addr;
        pstFrame->unHDRInfo.stDolbyInfo.stMetadata.u32Length = pstImage->HDR_Metadata_size;

    }
    else
    {
        pstFrame->enSrcFrameType = HI_DRV_VIDEO_FRAME_TYPE_SDR;
        pstFrame->unHDRInfo.stDolbyInfo.bMetadataValid = 0;
    }

    pstFrame->unHDRInfo.stDolbyInfo.bCompatible = pstChan->stVdecHdrAttr.AvplayHDRAttr.bCompatible;
#endif

    if ((pstFrame->enSrcFrameType != HI_DRV_VIDEO_FRAME_TYPE_DOLBY_BL) && (pstFrame->enSrcFrameType != HI_DRV_VIDEO_FRAME_TYPE_DOLBY_EL))
    {
        VDEC_SetHDRInfo(enType, pstImage, pstFrame);
    }

    pstPrivInfo->entype = enType;
#ifdef HI_TEE_SUPPORT
    /* Check if is secure frame */
    pstFrame->bSecure = (pstImage->is_SecureFrame != 0) ? HI_TRUE : HI_FALSE;
#endif

    if (0 == pstImage->is_fld_save)
    {
        pstVideoPriv->ePictureMode = HI_DRV_PICTURE_FRAME;
    }
    else if (1 == pstImage->is_fld_save)
    {
        pstVideoPriv->ePictureMode = HI_DRV_PICTURE_FIELD;
    }
    else
    {
        pstVideoPriv->ePictureMode = HI_DRV_PICTURE_BUTT;
    }

    ((HI_DRV_VIDEO_PRIVATE_S*)(pstFrame->u32Priv))->u32PrivDispTime = pstImage->DispTime;
    ((HI_DRV_VIDEO_PRIVATE_S*)(pstFrame->u32Priv))->u32PlayTime = pstImage->u32RepeatCnt;
    ((HI_DRV_VIDEO_PRIVATE_S*)(pstFrame->u32Priv))->eColorSpace  = VDEC_GetColorSpace(enType, pstImage);
    pstVideoPriv->stVideoOriginalInfo.enSource = HI_DRV_SOURCE_DTV;
    pstVideoPriv->stVideoOriginalInfo.u32Width = pstImage->disp_width;
    pstVideoPriv->stVideoOriginalInfo.u32Height = pstImage->disp_height;
    pstVideoPriv->stVideoOriginalInfo.u32FrmRate = pstFrame->u32FrameRate;
    pstVideoPriv->stVideoOriginalInfo.en3dType = pstFrame->eFrmType;
    pstVideoPriv->stVideoOriginalInfo.enSrcColorSpace = HI_DRV_CS_BT601_YUV_LIMITED;
    pstVideoPriv->stVideoOriginalInfo.enColorSys = HI_DRV_COLOR_SYS_AUTO;
    pstVideoPriv->stVideoOriginalInfo.bGraphicMode = HI_FALSE;
    pstVideoPriv->stVideoOriginalInfo.bInterlace = pstFrame->bProgressive;

    //DBM Info
    pstVideoPriv->stDNRInfo.bValid = pstImage->dbm_vaild;
    pstVideoPriv->stDNRInfo.u32ImageAvgQP = pstImage->dbm_qp_avg;
    pstVideoPriv->stDNRInfo.u32VdhInfoAddr = pstImage->dbm_info_phy_addr;
    pstVideoPriv->stDNRInfo.u32Size  = pstImage->dbm_info_size;
    pstVideoPriv->stDNRInfo.u32VdhInfoStride  = pstImage->dbm_info_stride;
}


HI_S32 VDEC_AddFrmBufRec(HI_S32 chanID, FRAME_BUF_S* pstFrameBuf, UINT32* pu32BufIndex)
{
    HI_U32 i = 0;
    VDEC_CHANNEL_S* pstChan = NULL;

    if (pstFrameBuf == NULL)
    {
        HI_ERR_VDEC("null pointer\n");
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[chanID].pstChan;

    VDEC_FrmBufRecSpinLockIRQ(&(pstChan->frameBufRecLock));

    for (i = 0; i < VDEC_MAX_BUFFER_RECORD; i++)
    {
        if (HI_FALSE == pstChan->stFrameBufferRecord[i].bConfigured && HI_FALSE == pstChan->stFrameBufferRecord[i].bAvailable)
        {
            pstChan->stFrameBufferRecord[i].enFrameBufferNodeStatus = pstFrameBuf->enBufNodeStatus;
            pstChan->stFrameBufferRecord[i].enHdrBufferNodeStatus   = pstFrameBuf->enBufHdrMetadataNodeStatus;
            pstChan->stFrameBufferRecord[i].enPmvBufferNodeStatus   = pstFrameBuf->enBufPmvNodeStatus;

            pstChan->stFrameBufferRecord[i].stFrameBuffer.pu8StartVirAddr  = (HI_U8*)(HI_SIZE_T)pstFrameBuf->u64StartVirAddr;
            pstChan->stFrameBufferRecord[i].stFrameBuffer.u32StartPhyAddr  = pstFrameBuf->u32StartPhyAddr;
            pstChan->stFrameBufferRecord[i].stFrameBuffer.u32Size          = pstFrameBuf->u32Size;

            pstChan->stFrameBufferRecord[i].stHdrBuffer.pu8StartVirAddr    = (HI_U8*)(HI_SIZE_T)pstFrameBuf->u64HdrMetadataStartVirAddr;
            pstChan->stFrameBufferRecord[i].stHdrBuffer.u32StartPhyAddr    = pstFrameBuf->u32HdrMetadataStartPhyAddr;
            pstChan->stFrameBufferRecord[i].stHdrBuffer.u32Size            = pstFrameBuf->u32HdrMetadataSize;

            pstChan->stFrameBufferRecord[i].stPmvBuffer.pu8StartVirAddr    = (HI_U8*)(HI_SIZE_T)pstFrameBuf->u64PmvStartVirAddr;
            pstChan->stFrameBufferRecord[i].stPmvBuffer.u32StartPhyAddr    = pstFrameBuf->u32PmvStartPhyAddr;
            pstChan->stFrameBufferRecord[i].stPmvBuffer.u32Size            = pstFrameBuf->u32PmvSize;
            *pu32BufIndex = i;
            VDEC_FrmBufRecSpinUnLockIRQ(&(pstChan->frameBufRecLock));
            return HI_SUCCESS;
        }


    }

        *pu32BufIndex = i;
        VDEC_FrmBufRecSpinUnLockIRQ(&(pstChan->frameBufRecLock));
        HI_ERR_VDEC("no node find!\n");
        return HI_FAILURE;

}

HI_S32 VDEC_DelFrmBufRec(HI_S32 chanID, UINT32 u32BufIndex)
{
    VDEC_CHANNEL_S* pstChan = NULL;

    pstChan = s_stVdecDrv.astChanEntity[chanID].pstChan;
    if (HI_NULL == pstChan || u32BufIndex >= VDEC_MAX_BUFFER_RECORD)
    {
        HI_ERR_VDEC("invalid param!\n");
        return HI_FAILURE;
    }

    VDEC_FrmBufRecSpinLockIRQ(&(pstChan->frameBufRecLock));


    memset(&pstChan->stFrameBufferRecord[u32BufIndex], 0, sizeof(CHAN_FRAME_BUFFER_S));
    pstChan->stFrameBufferRecord[u32BufIndex].enFrameBufferNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
    pstChan->stFrameBufferRecord[u32BufIndex].enHdrBufferNodeStatus   = DRV_VDEC_BUFFER_STATUS_BUTT;
    pstChan->stFrameBufferRecord[u32BufIndex].enPmvBufferNodeStatus   = DRV_VDEC_BUFFER_STATUS_BUTT;
    pstChan->stFrameBufferRecord[u32BufIndex].bAvailable                 = HI_FALSE;
    pstChan->stFrameBufferRecord[u32BufIndex].bConfigured            = HI_FALSE;
    pstChan->stFrameBufferRecord[u32BufIndex].bWaitRelease         = HI_FALSE;


    VDEC_FrmBufRecSpinUnLockIRQ(&(pstChan->frameBufRecLock));

    return HI_SUCCESS;

}


HI_S32 VDEC_ResetChanFrmBuf(HI_S32 hHandle)
{
    HI_U32 i = 0;
    VDEC_CHANNEL_S* pstChan = NULL;
    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }
    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("Chan %d VDEC_ResetChanFrmBuf err , not init!\n", hHandle);
        return HI_FAILURE;
    }


    for (i = 0; i < VDEC_MAX_BUFFER_RECORD; i++)
    {
        /*此处无需关心是否有special帧，因为此前都已经把special帧都记录到全局的数组里面了*/
        memset(&pstChan->stFrameBufferRecord[i], 0, sizeof(CHAN_FRAME_BUFFER_S));
        pstChan->stFrameBufferRecord[i].enFrameBufferNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
        pstChan->stFrameBufferRecord[i].enHdrBufferNodeStatus   = DRV_VDEC_BUFFER_STATUS_BUTT;
        pstChan->stFrameBufferRecord[i].enPmvBufferNodeStatus   = DRV_VDEC_BUFFER_STATUS_BUTT;
        pstChan->stFrameBufferRecord[i].bAvailable              = HI_FALSE;
        pstChan->stFrameBufferRecord[i].bConfigured             = HI_FALSE;
        pstChan->stFrameBufferRecord[i].bWaitRelease          = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 VDEC_SetChanControlState(HI_S32 chanID, VDEC_CHANNEL_CONTROL_STATE_E chanControlState)
{
    HI_S32 ret = HI_SUCCESS;

    VDEC_CHANNEL_S* pstChan = NULL;

    pstChan = s_stVdecDrv.astChanEntity[chanID].pstChan;

    if (pstChan == NULL)
    {
        return HI_FAILURE;
    }

    VDEC_ChanControlStateSpinLockIRQ(&(pstChan->chanControlStateLock));

    switch (chanControlState)
    {
        case NORMAL_CHAN:
            g_ChanControlState[chanID] = chanControlState;

            ret = HI_SUCCESS;

            break;

        case ALLOC_FRAME_BUF_TO_CHAN:
            if (NORMAL_CHAN != g_ChanControlState[chanID])
            {
                ret = HI_FAILURE;
            }
            else
            {
                g_ChanControlState[chanID] = chanControlState;
                ret = HI_SUCCESS;
            }

            break;

        case DESTORY_CHAN:
            if (ALLOC_FRAME_BUF_TO_CHAN == g_ChanControlState[chanID])
            {
                ret = HI_FAILURE;
            }
            else
            {
                g_ChanControlState[chanID] = chanControlState;
                ret = HI_SUCCESS;
            }

            break;

        default:
            break;
    }

    VDEC_ChanControlStateSpinUnlockIRQ(&(pstChan->chanControlStateLock));

    return ret;
}

HI_S32 VDEC_Chan_ReleasePreMem(VDEC_BUFFER_S* pstMemBuffer)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 i = 0;
    HI_U32 j = 0;

    VDEC_ASSERT_RETURN(pstMemBuffer != HI_NULL, HI_FAILURE);

    if (pstMemBuffer->u32Size > 0)
    {
        if (pstMemBuffer->u32StartPhyAddr >= g_stVDHMMZ.u32StartPhyAddr && pstMemBuffer->u32StartPhyAddr < (g_stVDHMMZ.u32StartPhyAddr + g_stVDHMMZ.u32Size))
        {
            for (i = 0; i < g_VdecPreMMZNodeNum; i++)
            {
                if ( st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr == pstMemBuffer->u32StartPhyAddr)
                {

                    if (0 == i)
                    {
                        if (0 == st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState && st_VdecChanPreUsedMMZInfo[i + 1].u32Size > 0)
                        {
                            st_VdecChanPreUsedMMZInfo[i].u32Size = st_VdecChanPreUsedMMZInfo[i].u32Size + st_VdecChanPreUsedMMZInfo[i + 1].u32Size;
                            st_VdecChanPreUsedMMZInfo[i].u32NodeState = 0;
                            st_VdecChanPreUsedMMZInfo[i + 1].u32Size = 0;

                            for (j = i + 1; j < g_VdecPreMMZNodeNum; j++)
                            {
                                memcpy(&st_VdecChanPreUsedMMZInfo[j], &st_VdecChanPreUsedMMZInfo[j + 1], sizeof(VDEC_PREMMZ_NODE_S));
                            }

                            g_VdecPreMMZNodeNum --;
                        }
                        else
                        {
                            st_VdecChanPreUsedMMZInfo[i].u32NodeState = 0;
                        }
                    }
                    else if (g_VdecPreMMZNodeNum - 1 == i)
                    {
                        if (0 == st_VdecChanPreUsedMMZInfo[i - 1].u32NodeState && st_VdecChanPreUsedMMZInfo[i - 1].u32Size > 0)
                        {
                            st_VdecChanPreUsedMMZInfo[i - 1].u32Size = st_VdecChanPreUsedMMZInfo[i].u32Size + st_VdecChanPreUsedMMZInfo[i - 1].u32Size;
                            st_VdecChanPreUsedMMZInfo[i].u32Size = 0;
                            g_VdecPreMMZNodeNum --;
                        }
                        else
                        {
                            st_VdecChanPreUsedMMZInfo[i].u32NodeState = 0;
                        }
                    }
                    else
                    {
                        if (0 == st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState && st_VdecChanPreUsedMMZInfo[i + 1].u32Size > 0)
                        {
                            st_VdecChanPreUsedMMZInfo[i].u32Size = st_VdecChanPreUsedMMZInfo[i].u32Size + st_VdecChanPreUsedMMZInfo[i + 1].u32Size;
                            st_VdecChanPreUsedMMZInfo[i].u32NodeState = 0;
                            st_VdecChanPreUsedMMZInfo[i + 1].u32Size = 0;

                            for (j = i + 1; j < g_VdecPreMMZNodeNum; j++)
                            {
                                memcpy(&st_VdecChanPreUsedMMZInfo[j], &st_VdecChanPreUsedMMZInfo[j + 1], sizeof(VDEC_PREMMZ_NODE_S));
                            }

                            g_VdecPreMMZNodeNum --;
                        }

                        if ( 0 == st_VdecChanPreUsedMMZInfo[i - 1].u32NodeState && st_VdecChanPreUsedMMZInfo[i - 1].u32Size > 0)
                        {
                            st_VdecChanPreUsedMMZInfo[i - 1].u32Size = st_VdecChanPreUsedMMZInfo[i - 1].u32Size + st_VdecChanPreUsedMMZInfo[i].u32Size;

                            for (j = i; j < g_VdecPreMMZNodeNum; j++)
                            {
                                memcpy(&st_VdecChanPreUsedMMZInfo[j], &st_VdecChanPreUsedMMZInfo[j + 1], sizeof(VDEC_PREMMZ_NODE_S));
                            }

                            g_VdecPreMMZNodeNum --;
                        }

                        if (1 == st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState && 1 == st_VdecChanPreUsedMMZInfo[i - 1].u32NodeState)
                        {
                            st_VdecChanPreUsedMMZInfo[i].u32NodeState = 0;
                        }
                    }
                }
            }
        }

        g_VdecPreVDHMMZUsedSize -= pstMemBuffer->u32Size;
    }


    return s32Ret;
}
#ifdef VFMW_VPSS_BYPASS_EN     //specialFrameExist->specialFrameNum

static HI_S32 VDEC_MarkSpecialFrmCanRls(HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Index = 0;

    D_VDEC_CHECK_PTR_RET(pstFrame);

    //BUFMNG_SpinLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);
    if (s_stVdecDrv.stRemainFrmList.s32Num != 0)
    {
        s32Ret = BUFMNG_VDEC_List_FindNodeIndex(&s_stVdecDrv.stRemainFrmList, pstFrame->stBufAddr[0].u32PhyAddr_YHead, &u32Index);
        if ( HI_SUCCESS != s32Ret )
        {
            HI_INFO_VDEC("Can't find the remain Frm node (Phy = 0x%x,bSecureMem = %d), Remain num = %d\n",
                         pstFrame->stBufAddr[0].u32PhyAddr_Y, pstFrame->bSecure, s_stVdecDrv.stRemainFrmList.s32Num);
        }
        else
        {
            /*this frame will be release in the thread*/
            s_stVdecDrv.stRemainFrmList.stSpecialFrmRec[u32Index].bCanRls = HI_TRUE;
            HI_INFO_VDEC("set the remain Frm node (Phy = 0x%x,bSecureMem = %d) can be release, Remain num = %d\n",
                         pstFrame->stBufAddr[0].u32PhyAddr_Y, pstFrame->bSecure, s_stVdecDrv.stRemainFrmList.s32Num);
        }
    }
    //BUFMNG_SpinUnLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);

    return s32Ret;
}

static HI_BOOL VDEC_IsSpecialFrm(VDEC_BUFFER_S* pstInputFrmRec)
{
    HI_U32 specialFrameIndex;
    HI_BOOL bSpecialFrm = HI_FALSE;
    VDEC_SPECIAL_FRM_INFO_S* pSpecialFrmInfo;

    if (!pstInputFrmRec)
    {
        HI_ERR_VDEC("Error:input param is null!\n");
        return HI_FALSE;
	}

    if ((pstInputFrmRec->u32StartPhyAddr == 0) || (pstInputFrmRec->u32StartPhyAddr == 0xffffffff))
    {
        return HI_FALSE;
    }

    BUFMNG_SpinLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);

    for (specialFrameIndex = 0; specialFrameIndex < VDEC_MAX_REMAIN_FRM_NUM; specialFrameIndex++)
    {
        pSpecialFrmInfo = &s_stVdecDrv.stRemainFrmList.stSpecialFrmRec[specialFrameIndex];
        if (pSpecialFrmInfo->frmBufRec.u32StartPhyAddr ==  pstInputFrmRec->u32StartPhyAddr)
        {
            bSpecialFrm = HI_TRUE;
            break;
        }
    }
    BUFMNG_SpinUnLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);

    return bSpecialFrm;
}


#else
static HI_S32 VDEC_RlsSpecialFrame(VDEC_CHANNEL_S* pstChan)
{
    HI_S32 s32Ret;
    HI_U32 i;
    VDEC_BUFFER_S stMemFrameBuffer;
    VDEC_BUFFER_S stMemHdrBuffer;

    if (HI_TRUE == pstChan->specialFrameExist)
    {

        for (i = 0; i < VDEC_MAX_BUFFER_RECORD; i++)
        {
            if (pstChan->specialFramePhyAddr == pstChan->stFrameBufferRecord[i].stFrameBuffer.u32StartPhyAddr)
            {
                memcpy(&stMemFrameBuffer, &(pstChan->stFrameBufferRecord[i].stFrameBuffer), sizeof(VDEC_BUFFER_S));
                memcpy(&stMemHdrBuffer, &(pstChan->stFrameBufferRecord[i].stHdrBuffer), sizeof(VDEC_BUFFER_S));
                break;
            }
        }
        if (DRV_VDEC_BUFFER_STATUS_PREALLOC == pstChan->stFrameBufferRecord[i].enFrameBufferNodeStatus) //预分配的节点，通过预分配函数释放
        {
            s32Ret = VDEC_Chan_ReleasePreMem(&stMemFrameBuffer);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VDEC("call VDEC_Chan_ReleasePreMem err, return :%#x \n", s32Ret);
                return HI_FAILURE;
            }

        }
        else if (DRV_VDEC_BUFFER_STATUS_SYSALLOC == pstChan->stFrameBufferRecord[i].enFrameBufferNodeStatus)
        {
            HI_DRV_VDEC_Release(&stMemFrameBuffer);
        }

        if (DRV_VDEC_BUFFER_STATUS_PREALLOC == pstChan->stFrameBufferRecord[i].enHdrBufferNodeStatus) //预分配的节点，通过预分配函数释放
        {
            s32Ret = VDEC_Chan_ReleasePreMem(&stMemHdrBuffer);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VDEC("call VDEC_Chan_ReleasePreMem err, return :%#x \n", s32Ret);
                return HI_FAILURE;
            }
        }
        else if (DRV_VDEC_BUFFER_STATUS_SYSALLOC == pstChan->stFrameBufferRecord[i].enHdrBufferNodeStatus)
        {
            HI_DRV_VDEC_UnmapAndRelease(&stMemHdrBuffer);
        }

        //刷新此记录节点的状态为初始状态
        memset(&pstChan->stFrameBufferRecord[i], 0, sizeof(CHAN_FRAME_BUFFER_S));
        pstChan->stFrameBufferRecord[i].enFrameBufferNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
        pstChan->stFrameBufferRecord[i].enHdrBufferNodeStatus   = DRV_VDEC_BUFFER_STATUS_BUTT;
        pstChan->stFrameBufferRecord[i].bAvailable              = HI_FALSE;
        pstChan->stFrameBufferRecord[i].bConfigured             = HI_FALSE;
        pstChan->stFrameBufferRecord[i].bCanRelse                               = HI_FALSE;
        pstChan->specialFrameExist                              = HI_FALSE;
    }
    return HI_SUCCESS;
}

#endif

static HI_S32 VDEC_RlsFrm(VDEC_CHANNEL_S* pstChan, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    HI_S32 s32Ret;
#ifdef VFMW_VPSS_BYPASS_EN
    HI_S32 s32Ret2 = HI_FAILURE;
#endif
    IMAGE stImage;
    IMAGE_INTF_S *pstImgInft = &pstChan->stImageIntf;
    VDEC_CHAN_STATINFO_S *pstStatInfo = &pstChan->stStatInfo;
    HI_DRV_VIDEO_PRIVATE_S *pstVideoPrivate = NULL;
    HI_VDEC_PRIV_FRAMEINFO_S *pstPrivInfo = NULL;

    pstVideoPrivate = (HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv);
    if (pstVideoPrivate == NULL)
    {
        return HI_FAILURE;
    }

    pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S *)(pstVideoPrivate->u32Reserve);
    if (pstPrivInfo == NULL)
    {
        return HI_FAILURE;
    }

    if (pstImgInft->release_image == NULL)
    {
        return HI_FAILURE;
    }

    memset(&stImage, 0, sizeof(stImage));
    stImage.image_stride = pstFrame->stBufAddr[0].u32Stride_Y;
    stImage.image_height = pstFrame->u32Height;
    stImage.image_width   = pstFrame->u32Width;
    stImage.luma_phy_addr = pstFrame->stBufAddr[0].u32PhyAddr_Y;
    stImage.top_luma_phy_addr = pstFrame->stBufAddr[0].u32PhyAddr_YHead;
    stImage.image_id             = pstPrivInfo->image_id;
    stImage.image_id_1           = pstPrivInfo->image_id_1;
    stImage.BTLInfo.btl_imageid  = pstPrivInfo->stBTLInfo.u32BTLImageID;
    stImage.BTLInfo.u32Is1D      = pstPrivInfo->stBTLInfo.u32Is1D;

    pstStatInfo->u32VdecRlsFrameTry++;

    s32Ret = pstImgInft->release_image(pstImgInft->image_provider_inst_id, &stImage);

    if (VDEC_OK != s32Ret)
    {
#ifdef VFMW_VPSS_BYPASS_EN
        s32Ret2 = VDEC_MarkSpecialFrmCanRls(pstFrame);
        if (HI_SUCCESS == s32Ret2)
        {
            pstStatInfo->u32VdecRlsFrameOK++;
            return HI_SUCCESS;
        }
        else
#endif
        {
            pstStatInfo->u32VdecRlsFrameFail++;
            return HI_FAILURE;
        }
    }
    else
    {
        pstStatInfo->u32VdecRlsFrameOK++;
        return HI_SUCCESS;
    }
    return HI_SUCCESS;
}


static HI_S32 VDEC_CheckConvertFrm(VDEC_CHANNEL_S* pstChan, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    if (0 == pstFrame->u32Height || 0 == pstFrame->u32Width)
    {
        (HI_VOID)VDEC_RlsFrm(pstChan, pstFrame);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 VDEC_CheckGetFirstIFrameFlag(IMAGE* pstImage)
{
    if (((pstImage->is_fld_save == 0) && (0 == (pstImage->format & 0x03))) ||
        ((pstImage->is_fld_save == 1)
        && ((0 == (pstImage->top_fld_type & 0x03)) || (0 == (pstImage->bottom_fld_type & 0x03))))
        )
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}
static HI_S32 VDEC_RecvFastBackFrm(HI_HANDLE hHandle, IMAGE_INTF_S* pstImgInft, IMAGE* pstImage)
{
    HI_S32 s32Ret;

    /*receive an effective image from vfmw*/
    s32Ret = pstImgInft->read_image(pstImgInft->image_provider_inst_id, pstImage);

    if (VDEC_OK == s32Ret)
    {
        if (HI_FALSE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stControlInfo.u32BackwardOptimizeFlag)
        {
            if (HI_FALSE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32GetFirstIFrameFlag)
            {
                if (VDEC_CheckGetFirstIFrameFlag(pstImage) == HI_TRUE)
                {
                    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32GetFirstIFrameFlag = HI_TRUE;
                    return HI_SUCCESS;
                }
                else
                {
                    s32Ret = pstImgInft->release_image(pstImgInft->image_provider_inst_id, pstImage);

                    if (VDEC_OK != s32Ret)
                    {
                        HI_WARN_VDEC("release_image failure\n");
                    }

                    return HI_FAILURE;
                }
            }
            else
            {
                return HI_SUCCESS;
            }
        }
        else
        {
            if (HI_TRUE == pstImage->DispEnableFlag)
            {
                if (HI_FALSE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32GetFirstIFrameFlag)
                {
                    //if (pstImage->GopNum !=  s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32LastFrameGopNum)
                    {
                        s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32GetFirstIFrameFlag = HI_TRUE;
                        //s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32LastFrameGopNum = pstImage->GopNum;
                        s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32ImageDistance = pstImage->DistanceBeforeFirstFrame;

                        if (0 != s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32ImageDistance)
                        {
                            s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32ImageDistance --;
                            s32Ret = pstImgInft->release_image(pstImgInft->image_provider_inst_id, pstImage);

                            if (VDEC_OK != s32Ret)
                            {
                                HI_ERR_VDEC("release_image failure\n");
                            }

                            return HI_FAILURE;
                        }
                        else
                        {
                            return HI_SUCCESS;
                        }
                    }
                    /*else
                    {
                        can go here?
                        s32Ret = pstImgInft->release_image(pstImgInft->image_provider_inst_id, pstImage);
                        return HI_FAILURE;
                    }*/
                }
                else
                {
                    if (0 != s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32ImageDistance)
                    {
                        s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32ImageDistance --;
                        s32Ret = pstImgInft->release_image(pstImgInft->image_provider_inst_id, pstImage);

                        if (VDEC_OK != s32Ret)
                        {
                            HI_ERR_VDEC("release_image failure\n");
                        }

                        return HI_FAILURE;
                    }
                    else
                    {
                        s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32ImageDistance = pstImage->DispFrameDistance;
                        return HI_SUCCESS;
                    }
                }
            }
            else
            {
                s32Ret = pstImgInft->release_image(pstImgInft->image_provider_inst_id, pstImage);

                if (VDEC_OK != s32Ret)
                {
                    HI_ERR_VDEC("release_image failure\n");
                }

                return HI_FAILURE;
            }
        }
    }
    else
    {
        return HI_FAILURE;
    }

    return HI_FAILURE;
}

static HI_S32 RecvFrm_ReadFailProcess(VDEC_CHANNEL_S* pstChan,
                                      HI_VDEC_PRIV_FRAMEINFO_S *pstPrivInfo,
                                      HI_VDEC_PRIV_FRAMEINFO_S *pstLastFrmPrivInfo,
                                      HI_DRV_VIDEO_PRIVATE_S *pstVideoPriv,
                                      HI_DRV_VIDEO_FRAME_S* pstFrame)
{
        if ((pstChan->u32EndFrmFlag == 4)
          || ((pstChan->u32EndFrmFlag == 1) && (pstChan->u32LastFrmTryTimes++ >= 4))
          || ((pstChan->u32EndFrmFlag == 2) && (pstLastFrmPrivInfo->image_id % 100 == pstChan->u32LastFrmId))
          || //(HI_VDEC_PRIV_FRAMEINFO_S*)pstFrame->u32Priv
            /* For user space decode mode, the first fail means receive over. */
            (pstChan->u32EndFrmFlag == 3))
        {
            /* Last frame is the end frame */
            pstPrivInfo->u8EndFrame = 2;
            pstChan->u32EndFrmFlag = 0;
            pstChan->u32LastFrmTryTimes = 0;
            pstChan->u32LastFrmId = -1;
            pstFrame->bProgressive = HI_FALSE;
            pstFrame->enFieldMode = HI_DRV_FIELD_ALL;
#ifdef HI_TEE_SUPPORT
            pstFrame->bSecure = pstChan->bTvp == HI_TRUE ? HI_TRUE : HI_FALSE;
#endif
            pstVideoPriv->u32LastFlag = DEF_HI_DRV_VPSS_LAST_ERROR_FLAG;

            return HI_SUCCESS;
        }

        return HI_FAILURE;
    }


static HI_VOID RecvFrm_FramePackTypeProcess(HI_HANDLE hHandle,
                                            IMAGE *pstImage,
                                            VDEC_VPSSCHANNEL_S* pstVpssChan,
                                            HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    switch (pstVpssChan->eFramePackType)
    {
        case HI_UNF_FRAME_PACKING_TYPE_NONE:            /**< Normal frame, not a 3D frame */
            pstFrame->eFrmType = HI_DRV_FT_NOT_STEREO;
            break;

        case HI_UNF_FRAME_PACKING_TYPE_3D_TILE:            /**< Tile frame, for vpss to convert 3D */
            pstFrame->eFrmType = HI_DRV_FT_TILE;
            break;

        case HI_UNF_FRAME_PACKING_TYPE_SIDE_BY_SIDE:     /**< Side by side */
            pstFrame->eFrmType = HI_DRV_FT_SBS;

            if (pstImage->disp_width / 2 > pstImage->disp_height)
            {
                pstFrame->u32AspectWidth = pstImage->disp_width / 2;
                pstFrame->u32AspectHeight = pstImage->disp_height;
            }
            else
            {
                pstFrame->u32AspectWidth = pstImage->disp_width;
                pstFrame->u32AspectHeight = pstImage->disp_height;
            }

            s_stVdecDrv.astChanEntity[hHandle].pstChan->u32UserSetAspectWidth = pstFrame->u32AspectWidth;
            s_stVdecDrv.astChanEntity[hHandle].pstChan->u32UserSetAspectHeight = pstFrame->u32AspectHeight;
            break;

        case HI_UNF_FRAME_PACKING_TYPE_TOP_AND_BOTTOM:   /**< Top and bottom */
            pstFrame->eFrmType = HI_DRV_FT_TAB;

            if (pstImage->disp_width < pstImage->disp_height)
            {
                pstFrame->u32AspectWidth = pstImage->disp_width;
                pstFrame->u32AspectHeight = pstImage->disp_height / 2;
            }
            else
            {
                pstFrame->u32AspectWidth = pstImage->disp_width;
                pstFrame->u32AspectHeight = pstImage->disp_height;
            }

            s_stVdecDrv.astChanEntity[hHandle].pstChan->u32UserSetAspectWidth = pstFrame->u32AspectWidth;
            s_stVdecDrv.astChanEntity[hHandle].pstChan->u32UserSetAspectHeight = pstFrame->u32AspectHeight;
            break;

        case HI_UNF_FRAME_PACKING_TYPE_TIME_INTERLACED:  /**< Time interlaced: one frame for left eye, the next frame for right eye */
            //pstFrame->eFrmType = HI_DRV_FT_FPK;
            break;

        case HI_UNF_FRAME_PACKING_TYPE_FRAME_PACKING:
            break;

        default:
            pstFrame->eFrmType = HI_DRV_FT_BUTT;
            break;
    }

    return;
}

static HI_S32 RecvFrm_IFrameCheck(IMAGE *pstImage)
{

    if ((0 == (pstImage->format & 0x3))
        || ((1 == pstImage->is_fld_save)
           && ((1 == (pstImage->format & 0x3))
               && ((0 == pstImage->top_fld_type)
                  || (0 == pstImage->bottom_fld_type))
              )
            )
       )/* I frame */
    {
        return 1;
    }

    return 0;
}

#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
#if (1 == HI_VDEC_CC_FROM_IMAGE)
static HI_VOID RecvFrm_UserDataProcess(HI_HANDLE hHandle, IMAGE *pstImage)
{
    HI_U32 u32ID;
    HI_U8 u8Type;
    HI_U32 u32Index;
    USRDAT* pstUsrData = HI_NULL;

    /* TVP not support userdata*/
    if (pstImage->is_SecureFrame != HI_TRUE)
    {
        for (u32Index = 0 ; u32Index < 4; u32Index++)
        {

            pstUsrData     = (VOID*)(HI_SIZE_T)(pstImage->p_usrdat[u32Index]);

            if (HI_NULL != pstUsrData)
            {
                if (pstUsrData->data_size > 5)
                {
                    pstUsrData->PTS = pstImage->PTS;
                    u32ID = *((HI_U32*)pstUsrData->data);
                    u8Type = pstUsrData->data[4];

                    if ((VDEC_USERDATA_IDENTIFIER_DVB1 == u32ID) && (VDEC_USERDATA_TYPE_DVB1_CC == u8Type))
                    {
                        USRDATA_Put(hHandle, pstUsrData, HI_UNF_VIDEO_USERDATA_DVB1_CC);
                    }
                }
            }
        }
    }

    return;
}
#endif
#endif


static HI_S32 VDEC_RecvFrm(HI_HANDLE hHandle, VDEC_CHANNEL_S* pstChan, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32UserdataId;
    IMAGE stImage = {0};
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
    VDEC_CHAN_STATE_S stChanState = {0};

    IMAGE_INTF_S* pstImgInft = &pstChan->stImageIntf;
    VDEC_CHAN_STATINFO_S* pstStatInfo = &pstChan->stStatInfo;
    HI_VDEC_PRIV_FRAMEINFO_S* pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S*)(((HI_DRV_VIDEO_PRIVATE_S*)(pstFrame->u32Priv))->u32Reserve);
    HI_VDEC_PRIV_FRAMEINFO_S* pstLastFrmPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S*)(((HI_DRV_VIDEO_PRIVATE_S*)(pstChan->stLastDispFrameInfo.u32Priv))->u32Reserve);
    HI_DRV_VIDEO_PRIVATE_S* pstVideoPriv = (HI_DRV_VIDEO_PRIVATE_S*)(pstFrame->u32Priv);
    HI_S32 s32FrameRate;
    memset(&stImage, 0, sizeof(stImage));

    if (pstLastFrmPrivInfo->u8EndFrame == 1)
    {
        pstPrivInfo->u8EndFrame = 2;
        pstChan->u32EndFrmFlag = 0;
        pstChan->u32LastFrmTryTimes = 0;
        pstChan->u32LastFrmId = -1;
        pstFrame->bProgressive = HI_FALSE;
        pstFrame->enFieldMode = HI_DRV_FIELD_ALL;
        pstVideoPriv->u32LastFlag = DEF_HI_DRV_VPSS_LAST_ERROR_FLAG;

        return HI_SUCCESS;
    }

    /*VDEC Receive frame from VFMW */
#ifdef VFMW_VPSS_BYPASS_EN
    BUFMNG_SpinLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);
#endif
    pstStatInfo->u32VdecRcvFrameTry++;

    if (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32Speed < 0)
    {
        s32Ret = VDEC_RecvFastBackFrm(hHandle, pstImgInft, &stImage);

        if (HI_SUCCESS != s32Ret)
        {
            VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
            pstVpssChan = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan);
            //(void)(s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(pstVpssChan->hVpss,HI_DRV_VPSS_USER_COMMAND_IMAGEREADY, NULL);
        }
    }
    else
    {
        s32Ret = pstImgInft->read_image(pstImgInft->image_provider_inst_id, &stImage);
    }

#ifdef VFMW_VPSS_BYPASS_EN
    BUFMNG_SpinUnLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);
#endif
    if (s32Ret != VDEC_OK)
    {
        return RecvFrm_ReadFailProcess(pstChan, pstPrivInfo, pstLastFrmPrivInfo, pstVideoPriv, pstFrame);
    }

    pstChan->u32LastFrmTryTimes = 0;

    pstStatInfo->u32VdecRcvFrameOK++;
    pstStatInfo->u32TotalVdecOutFrame++;

    /* Calculate PTS */
    PTSREC_CalcStamp(hHandle, pstChan, &stImage);


    if (g_CHAN_FRAME_RATE[hHandle] != 0)
    {
        stImage.frame_rate = g_CHAN_FRAME_RATE[hHandle] * 1024;
    }


    s32FrameRate = stImage.frame_rate / 1024;

    if (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32Speed < 0)
    {
        s32FrameRate = 60;
        s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_SET_FRAME_RATE, &s32FrameRate);

        if (VDEC_OK != s32Ret)
        {
            HI_ERR_VDEC("VDEC_CID_SET_FRAME_RATE failure\n");
        }
    }

    if (s32FrameRate > 30 && s32FrameRate > (pstChan->stLastDispFrameInfo.u32FrameRate / 1000))
    {
        s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_SET_FRAME_RATE, &s32FrameRate);

        if (VDEC_OK != s32Ret)
        {
            HI_ERR_VDEC("VDEC_CID_SET_FRAME_RATE failure\n");
        }
    }

    /*interleaved source, VPSS module swtich field to frame, need to adjust pts*/
    pstPrivInfo->s32InterPtsDelta = PTSREC_GetInterPtsDelta(hHandle);

    /* Save user data for watermark */
    for (u32UserdataId = 0; u32UserdataId < 4; u32UserdataId++)
    {
        pstChan->pu8UsrDataForWaterMark[u32UserdataId] = (VOID*)(HI_SIZE_T)(stImage.p_usrdat[u32UserdataId]);
    }

    /* Get channel state */
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_GET_CHAN_STATE, &stChanState);

    if (VDEC_OK != s32Ret)
    {
        HI_FATAL_VDEC("Chan %d GET_CHAN_STATE err\n", pstChan->hChan);
    }

#if (0 == VDEC_USERDATA_NEED_ARRANGE)

    if (HI_NULL != stImage.p_usrdat)
    {
        USRDATA_Arrange(hHandle, (VOID*)(HI_SIZE_T)(stImage.p_usrdat[0]));
    }

#endif

    /* Convert VFMW-IMAGE to VO-HI_UNF_VIDEO_FRAME_INFO_S */
    VDEC_ConvertFrm(pstChan->stCurCfg.enType, pstChan, &stImage, pstFrame);
    s32Ret = VDEC_CheckConvertFrm(pstChan, pstFrame);

    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    pstChan->bAvsPlus = stImage.optm_inf.bAvsPlus;

    /*set framePackingType for pstFrame*/
    pstVpssChan = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan);

    if (HI_UNF_FRAME_PACKING_TYPE_BUTT != pstVpssChan->eFramePackType)
    {
         RecvFrm_FramePackTypeProcess(hHandle, &stImage, pstVpssChan, pstFrame);
    }

    if (pstFrame->eFrmType != pstChan->stLastFrm.eFrmType)
    {
        pstChan->bFramePackingChange = HI_TRUE;
        pstChan->enFramePackingType = VDEC_ConverFrameType(pstFrame->eFrmType);
    }

    memcpy(&(pstChan->stLastFrm), pstFrame, sizeof(HI_DRV_VIDEO_FRAME_S));

    pstFrame->hTunnelSrc = (HI_ID_VDEC << 16) | hHandle;

    if (stImage.image_id % 100 == pstChan->u32LastFrmId)
    {
        pstPrivInfo->u8EndFrame = 1;
        pstVideoPriv->u32LastFlag = DEF_HI_DRV_VPSS_LAST_FRAME_FLAG;
    }

    /* Count err frame */
    pstStatInfo->u32VdecErrFrame = stChanState.error_frame_num;
    //w00271806  vdec proc ErrFrame count
    //pstStatInfo->u32VdecDecErrFrame = stChanState.dec_error_frame_num;

    pstFrame->bIsFirstIFrame = HI_FALSE;

    /*Record the interval of I frames and the output time of the first I frame*/
    /*CNcomment: 记录I帧间隔和换台后第一个I帧解码输出时间 */
    if (RecvFrm_IFrameCheck(&stImage))
    {
        HI_DRV_STAT_Event(STAT_EVENT_IFRAMEINTER, pstFrame->u32Pts);

        if (1 == pstStatInfo->u32TotalVdecOutFrame)
        {
            pstFrame->bIsFirstIFrame = HI_TRUE;
            HI_DRV_STAT_Event(STAT_EVENT_IFRAMEOUT, 0);
        }
    }

    if (pstChan->bIsIFrameDec)
    {
        //pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21;
        pstFrame->bProgressive = HI_TRUE;
        pstPrivInfo->u8Marker |= 0x2;
    }
    else
    {
        pstPrivInfo->u8Marker &= 0xfd;
    }

#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
#if (1 == HI_VDEC_CC_FROM_IMAGE)
    RecvFrm_UserDataProcess(hHandle, &stImage);
#endif
#endif

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_GetFrmBuf(HI_HANDLE hHandle, HI_DRV_VDEC_FRAME_BUF_S* pstFrm)
{
    HI_S32 s32Ret;
    USRDEC_FRAME_DESC_S stFrameDesc;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    if (HI_NULL == pstFrm)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    memset(&stFrameDesc, 0, sizeof(USRDEC_FRAME_DESC_S));

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_WARN_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    /* Get from VFMW */
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_GET_USRDEC_FRAME, &stFrameDesc);

    if (VDEC_OK != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_WARN_VDEC("Chan %d GET_USRDEC_FRAME err!\n", pstChan->hChan);
        return HI_FAILURE;
    }

    pstFrm->u32PhyAddr = stFrameDesc.s32LumaPhyAddr;
    pstFrm->u32Size = stFrameDesc.s32FrameSize;
    if (HI_TRUE == pstChan->bLowdelay)
    {
        pstFrm->u32LineNumPhyAddr = stFrameDesc.s32LineNumPhyAddr;
        pstFrm->u64LineNumVirAddr = stFrameDesc.u64LineNumVirAddr;
    }
    else
    {
        pstFrm->u32LineNumPhyAddr = 0;
        pstFrm->u64LineNumVirAddr = 0;
    }
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_PutFrmBuf(HI_HANDLE hHandle, HI_DRV_VDEC_USR_FRAME_S* pstFrm)
{
    HI_S32 s32Ret;
    USRDEC_FRAME_DESC_S stFrameDesc;
    VDEC_CHANNEL_S*       pstChan = HI_NULL;

    if (HI_NULL == pstFrm)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_WARN_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    /* Convert color format */
    switch (pstFrm->enFormat)
    {
        case HI_UNF_FORMAT_YUV_SEMIPLANAR_422:
            stFrameDesc.enFmt = COLOR_FMT_422_2x1;
            stFrameDesc.s32IsSemiPlanar = HI_TRUE;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_420:
            stFrameDesc.enFmt = COLOR_FMT_420;
            stFrameDesc.s32IsSemiPlanar = HI_TRUE;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_400:
            stFrameDesc.enFmt = COLOR_FMT_400;
            stFrameDesc.s32IsSemiPlanar = HI_TRUE;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_411:
            stFrameDesc.enFmt = COLOR_FMT_411;
            stFrameDesc.s32IsSemiPlanar = HI_TRUE;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_422_1X2:
            stFrameDesc.enFmt = COLOR_FMT_422_1x2;
            stFrameDesc.s32IsSemiPlanar = HI_TRUE;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_444:
            stFrameDesc.enFmt = COLOR_FMT_444;
            stFrameDesc.s32IsSemiPlanar = HI_TRUE;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_400:
            stFrameDesc.enFmt = COLOR_FMT_400;
            stFrameDesc.s32IsSemiPlanar = HI_FALSE;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_411:
            stFrameDesc.enFmt = COLOR_FMT_411;
            stFrameDesc.s32IsSemiPlanar = HI_FALSE;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_420:
            stFrameDesc.enFmt = COLOR_FMT_420;
            stFrameDesc.s32IsSemiPlanar = HI_FALSE;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_422_1X2:
            stFrameDesc.enFmt = COLOR_FMT_422_1x2;
            stFrameDesc.s32IsSemiPlanar = HI_FALSE;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_422_2X1:
            stFrameDesc.enFmt = COLOR_FMT_422_2x1;
            stFrameDesc.s32IsSemiPlanar = HI_FALSE;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_444:
            stFrameDesc.enFmt = COLOR_FMT_444;
            stFrameDesc.s32IsSemiPlanar = HI_FALSE;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_410:
            stFrameDesc.enFmt = COLOR_FMT_410;
            stFrameDesc.s32IsSemiPlanar = HI_FALSE;
            break;

        case HI_UNF_FORMAT_YUV_PACKAGE_UYVY:
        case HI_UNF_FORMAT_YUV_PACKAGE_YUYV:
        case HI_UNF_FORMAT_YUV_PACKAGE_YVYU:
        default:
            stFrameDesc.enFmt = COLOR_FMT_BUTT;
            stFrameDesc.s32IsSemiPlanar = HI_FALSE;
            break;
    }

    stFrameDesc.Pts = pstFrm->u32Pts;
    stFrameDesc.s32YWidth  = pstFrm->s32YWidth;
    stFrameDesc.s32YHeight = pstFrm->s32YHeight;
    stFrameDesc.s32LumaPhyAddr = pstFrm->s32LumaPhyAddr;
    stFrameDesc.s32LumaStride = pstFrm->s32LumaStride;
    stFrameDesc.s32CbPhyAddr    = pstFrm->s32CbPhyAddr;
    stFrameDesc.s32CrPhyAddr    = pstFrm->s32CrPhyAddr;
    stFrameDesc.s32ChromStride  = pstFrm->s32ChromStride;
    stFrameDesc.s32ChromCrStride  = pstFrm->s32ChromCrStride;
    stFrameDesc.s32IsFrameValid = pstFrm->bFrameValid;
    stFrameDesc.s32LineNumPhyAddr = pstFrm->s32LineNumPhyAddr;
    stFrameDesc.u64LineNumVirAddr = pstFrm->u64LineNumVirAddr;

    /* Last frame is the end frame */
    if (pstFrm->bEndOfStream)
    {
        pstChan->u32EndFrmFlag = 3;
    }

    /* Put */
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_PUT_USRDEC_FRAME, &stFrameDesc);

    if (VDEC_OK != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d PUT_USRDEC_FRAME err!\n", pstChan->hChan);
        return HI_FAILURE;
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_RecvFrmBuf(HI_HANDLE hHandle, HI_DRV_VIDEO_FRAME_S* pstFrm)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    //HI_DRV_VIDEO_FRAME_S* pstLastFrm = HI_NULL;

    if ((HI_NULL == pstFrm) || (HI_INVALID_HANDLE == hHandle))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    memset(pstFrm, 0, sizeof(HI_DRV_VIDEO_FRAME_S));

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_WARN_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;


    if (VDEC_CHAN_STATE_RUN != pstChan->enCurState)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_WARN_VDEC("Chan %d isn't runnig!\n", hHandle);
        return HI_FAILURE;
    }

    /*VPSS Read a frame from VDEC */
    pstChan->stStatInfo.u32UserAcqFrameTry++;

    s32Ret = VDEC_RecvFrm(hHandle, pstChan, pstFrm);
    if (HI_SUCCESS != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return HI_FAILURE;
    }

    pstChan->stStatInfo.u32UserAcqFrameOK++;

    pstChan->stLastDispFrameInfo = *pstFrm;

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    /* save yuv data */
    BUFMNG_SaveYuv(hHandle, pstFrm, pstChan->stCurCfg.enType);

    /* save rpu stream */
	if (BUFMNG_SaveRPU(hHandle, pstFrm) < 0)
    {
        HI_WARN_VDEC("Chan %d save rpu failed!!!\n", hHandle);
    }

    if (MaskCtrlWord & 0x2)
    {
        VDEC_RlsFrm(pstChan, pstFrm);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_RlsFrmBuf(HI_HANDLE hHandle, HI_DRV_VIDEO_FRAME_S*  pstFrm)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    if ((HI_NULL == pstFrm) || (HI_INVALID_HANDLE == hHandle))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_WARN_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    pstChan->stStatInfo.u32UserRlsFrameTry++;

#ifdef VFMW_VPSS_BYPASS_EN
    BUFMNG_SpinLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);
#endif
    s32Ret = VDEC_RlsFrm(pstChan, pstFrm);
#ifdef VFMW_VPSS_BYPASS_EN
    BUFMNG_SpinUnLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);
#endif

    if (HI_SUCCESS != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("VDEC_RlsFrm err!==>Phyaddr = 0x%x\n", pstFrm->stBufAddr[0].u32PhyAddr_Y);
        return HI_FAILURE;
    }

    pstChan->stStatInfo.u32UserRlsFrameOK++;
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}
static HI_S32 VDEC_Chan_VpssRecvFrmBuf(VPSS_HANDLE hVpss, HI_DRV_VIDEO_FRAME_S* pstFrm)
{
    HI_S32 s32Ret;
    HI_HANDLE hVdec;

    if (HI_NULL == pstFrm)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_FindVdecHandleByVpssHandle(hVpss, &hVdec);

    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    s32Ret = HI_DRV_VDEC_RecvFrmBuf(hVdec, pstFrm);

    return s32Ret;
}
static HI_S32 VDEC_Chan_VpssRlsFrmBuf(VPSS_HANDLE hVpss, HI_DRV_VIDEO_FRAME_S*  pstFrm)
{
    HI_S32 s32Ret;
    HI_S32 s32RetSpecialRls = HI_FAILURE;
    HI_DRV_VIDEO_PRIVATE_S* pstVideoPriv;
    HI_HANDLE hVdec;

    if (HI_NULL == pstFrm)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    pstVideoPriv = (HI_DRV_VIDEO_PRIVATE_S*)(pstFrm->u32Priv);

    if (pstVideoPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_ERROR_FLAG)
    {
        HI_INFO_VDEC("VPSS release Fake frame! index = 0x%x\n", pstFrm->u32FrameIndex);
        return HI_SUCCESS;
    }

#ifdef VFMW_VPSS_BYPASS_EN
    BUFMNG_SpinLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);

    if (s_stVdecDrv.stRemainFrmList.s32Num != 0)
    {
        HI_S32 s32Ret = HI_FAILURE;
        HI_U32 u32Index = 0;

        s32Ret = BUFMNG_VDEC_List_FindNodeIndex(&s_stVdecDrv.stRemainFrmList, pstFrm->stBufAddr[0].u32PhyAddr_YHead, &u32Index);
        if ( HI_SUCCESS != s32Ret )
        {
            HI_INFO_VDEC("VPSS::Can't find the remain Frm node (Phy = 0x%x,bSecureMem = %d), Remain num = %d\n",
                         pstFrm->stBufAddr[0].u32PhyAddr_Y, pstFrm->bSecure, s_stVdecDrv.stRemainFrmList.s32Num);
        }
        else
        {
            /*this frame will be release in the thread*/
            s_stVdecDrv.stRemainFrmList.stSpecialFrmRec[u32Index].bCanRls = HI_TRUE;
            HI_INFO_VDEC("vpss::set the remain Frm node (Phy = 0x%x,bSecureMem = %d) can be release, Remain num = %d\n",
                         pstFrm->stBufAddr[0].u32PhyAddr_Y, pstFrm->bSecure, s_stVdecDrv.stRemainFrmList.s32Num);
            BUFMNG_SpinUnLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);

            /*updata the proc info*/
            s32Ret = VDEC_FindVdecHandleByVpssHandle(hVpss, &hVdec);

            if ((HI_SUCCESS != s32Ret))
            {
                return HI_SUCCESS;  //此时有可能通道已经被释放
            }

            {
                VDEC_CHANNEL_S* pstChan = s_stVdecDrv.astChanEntity[hVdec].pstChan;
                pstChan->stStatInfo.u32VdecRlsFrameTry++;
                pstChan->stStatInfo.u32VdecRlsFrameOK++;
            }

            return s32Ret;
        }
    }
    BUFMNG_SpinUnLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);
#endif

    s32Ret = VDEC_FindVdecHandleByVpssHandle(hVpss, &hVdec);

    if ((HI_SUCCESS != s32Ret))
    {
        return s32RetSpecialRls;   // if s32RetSpecialRls = success ,mean this frame will be release success
    }


    s32Ret = HI_DRV_VDEC_RlsFrmBuf(hVdec, pstFrm);
    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("normal release PhyAddr 0x%x, failed! return 0x%x\n", pstFrm->stBufAddr[0].u32PhyAddr_Y, s32Ret);
        return s32RetSpecialRls;   // if s32RetSpecialRls = success ,mean this frame will be release success
    }

    return s32Ret;
}
HI_S32 HI_DRV_VDEC_RlsFrmBufWithoutHandle(HI_DRV_VIDEO_FRAME_S* pstFrm)
{
    HI_S32 s32Ret;
    HI_U32 u32Yaddr;

    if (HI_NULL == pstFrm)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Get handle value */
    u32Yaddr = pstFrm->stBufAddr[0].u32PhyAddr_Y;
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(HI_INVALID_HANDLE, VDEC_CID_GET_CHAN_ID_BY_MEM, &u32Yaddr);

    if (VDEC_OK != s32Ret)
    {
        HI_ERR_VDEC("VMFW GET_CHAN_ID_BY_MEM err!\n");
        return HI_FAILURE;
    }

    return HI_DRV_VDEC_RlsFrmBuf(u32Yaddr & 0xff, pstFrm);
}

#if 0
static inline HI_VOID VDEC_YUVFormat_UNF2VFMW(HI_UNF_VIDEO_FORMAT_E enUNF, YUV_FORMAT_E* penVFMW)
{
    switch (enUNF)
    {
        case HI_UNF_FORMAT_YUV_SEMIPLANAR_422:
            *penVFMW = SPYCbCr422_2X1;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_420:
            *penVFMW = SPYCbCr420;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_400:
            *penVFMW = SPYCbCr400;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_411:
            *penVFMW = SPYCbCr411;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_422_1X2:
            *penVFMW = SPYCbCr422_1X2;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_444:
            *penVFMW = SPYCbCr444;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_400:
            *penVFMW = PLNYCbCr400;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_411:
            *penVFMW = PLNYCbCr411;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_420:
            *penVFMW = PLNYCbCr420;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_422_1X2:
            *penVFMW = PLNYCbCr422_1X2;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_422_2X1:
            *penVFMW = PLNYCbCr422_2X1;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_444:
            *penVFMW = PLNYCbCr444;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_410:
            *penVFMW = PLNYCbCr410;
            break;

        case HI_UNF_FORMAT_YUV_PACKAGE_UYVY:
        case HI_UNF_FORMAT_YUV_PACKAGE_YUYV:
        case HI_UNF_FORMAT_YUV_PACKAGE_YVYU:
        default:
            *penVFMW = SPYCbCr420;
            break;
    }
}
#endif

static inline HI_VOID VDEC_YUVFormat_UNF2VFMW(HI_DRV_PIX_FORMAT_E enVideo, YUV_FORMAT_E* penVFMW)
{
    switch (enVideo)
    {
        case HI_DRV_PIX_FMT_NV08:
            *penVFMW = SPYCbCr400;
            break;

        case HI_DRV_PIX_FMT_NV12_411:
            *penVFMW = SPYCbCr411;
            break;

        case HI_DRV_PIX_FMT_NV16:
            *penVFMW = SPYCbCr422_1X2;
            break;

        case HI_DRV_PIX_FMT_NV16_2X1:
            *penVFMW = SPYCbCr422_2X1;
            break;

        case HI_DRV_PIX_FMT_NV24:
            *penVFMW = SPYCbCr444;
            break;

        case HI_DRV_PIX_FMT_YUV400:
            *penVFMW = PLNYCbCr400;
            break;

        case HI_DRV_PIX_FMT_YUV411:
            *penVFMW = PLNYCbCr411;
            break;

        case HI_DRV_PIX_FMT_YUV420p:
            *penVFMW = PLNYCbCr420;
            break;

        case HI_DRV_PIX_FMT_YUV422_1X2:
            *penVFMW = PLNYCbCr422_1X2;
            break;

        case HI_DRV_PIX_FMT_YUV422_2X1:
            *penVFMW = PLNYCbCr422_2X1;
            break;

        case HI_DRV_PIX_FMT_YUV_444:
            *penVFMW = PLNYCbCr444;
            break;

        case HI_DRV_PIX_FMT_YUV410p:
            *penVFMW = PLNYCbCr410;
            break;

        case HI_DRV_PIX_FMT_NV21:
        default:
            *penVFMW = SPYCbCr420;
            break;
    }
}
//此函数已经不用，编译有警告,直接干掉,避免其他模块可能还可能有调用此函数的地方为避免编译报错，先将函数内存实现干掉
//warning: the frame size of 1040 bytes is larger than 1024 bytes [-Wframe-larger-than=]
#if 1
HI_S32 HI_DRV_VDEC_BlockToLine(HI_S32 hHandle, HI_DRV_VDEC_BTL_S* pstBTL)
{
    /* Only hi3712 support this interface now. */
#if 0
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    struct
    {
        IMAGE astImage[2];
        HI_U32 u32Size;
    } stBTLParam;
    HI_VDEC_PRIV_FRAMEINFO_S* pstPrivInfo = HI_NULL;

    if ((HI_NULL == pstBTL) || (HI_NULL == pstBTL->pstInFrame) ||
        (HI_NULL == pstBTL->pstOutFrame) || (HI_INVALID_HANDLE == hHandle))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_WARN_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S*)pstBTL->pstInFrame->u32Priv;

    /* Set input image data */
    stBTLParam.astImage[0].top_luma_phy_addr = pstBTL->pstInFrame->stBufAddr[0].u32PhyAddr_Y;
    stBTLParam.astImage[0].top_chrom_phy_addr = pstBTL->pstInFrame->stBufAddr[0].u32PhyAddr_C;
    stBTLParam.astImage[0].luma_phy_addr = pstBTL->pstInFrame->stBufAddr[0].u32PhyAddr_Y;
    stBTLParam.astImage[0].chrom_phy_addr = pstBTL->pstInFrame->stBufAddr[0].u32PhyAddr_C;
    stBTLParam.astImage[0].image_width = pstBTL->pstInFrame->u32Width;
    stBTLParam.astImage[0].image_height = pstBTL->pstInFrame->u32Height;
    stBTLParam.astImage[0].image_stride = pstBTL->pstInFrame->stBufAddr[0].u32Stride_Y;
    //stBTLParam.astImage[0].image_id = pstBTL->pstInFrame->u32FrameIndex;
    stBTLParam.astImage[0].BTLInfo.u32Is1D = pstPrivInfo->stBTLInfo.u32Is1D;
    stBTLParam.astImage[0].BTLInfo.u32IsCompress = pstPrivInfo->stBTLInfo.u32IsCompress;
    stBTLParam.astImage[0].BTLInfo.u32HeadStride = pstPrivInfo->stCompressInfo.u32HeadStride;
    stBTLParam.astImage[0].BTLInfo.u32HeadOffset = pstPrivInfo->stCompressInfo.u32HeadOffset;
    stBTLParam.astImage[0].BTLInfo.u32YHeadAddr = pstPrivInfo->stCompressInfo.u32YHeadAddr;
    stBTLParam.astImage[0].BTLInfo.u32CHeadAddr = pstPrivInfo->stCompressInfo.u32CHeadAddr;
    stBTLParam.astImage[0].BTLInfo.u32CrStride = pstBTL->pstInFrame->stBufAddr[0].u32Stride_Cr;
    stBTLParam.astImage[0].BTLInfo.u32CrAddr = pstBTL->pstInFrame->stBufAddr[0].u32PhyAddr_Cr;
    //stBTLParam.astImage[0].BTLInfo.u32Reversed = pstBTL->pstInFrame->u32Circumrotate;
    VDEC_YUVFormat_UNF2VFMW(pstBTL->pstInFrame->ePixFormat, &(stBTLParam.astImage[0].BTLInfo.YUVFormat));

    /* Set output image data */
    stBTLParam.astImage[1].luma_2d_phy_addr = pstBTL->u32PhyAddr;

    /* Set buffer size */
    stBTLParam.u32Size = pstBTL->u32Size;

    pstChan->stBTL.pstFrame = pstBTL->pstOutFrame;
    atomic_inc(&pstChan->stBTL.atmWorking);

    /* VDEC_CID_FRAME_BTL */
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_FRAME_BTL, &stBTLParam);

    if (VDEC_OK != s32Ret)
    {
        HI_ERR_VDEC("Chan %d VDEC_CID_FRAME_BTL err!\n", pstChan->hChan);
        goto err;
    }

    /* Wait for over */
    if (0 == wait_event_interruptible_timeout(pstChan->stBTL.stWaitQue,
            (atomic_read(&pstChan->stBTL.atmWorking) == 0), msecs_to_jiffies(pstBTL->u32TimeOutMs)))
    {
        HI_ERR_VDEC("Chan %d BlockToLine time out!\n", pstChan->hChan);
        goto err;
    }

    pstChan->stBTL.pstFrame = HI_NULL;
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;

err:
    pstChan->stBTL.pstFrame = HI_NULL;
    atomic_set(&pstChan->stBTL.atmWorking, 0);
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_FAILURE;
#else
    return HI_FAILURE;
#endif
}
#endif

static const HI_U32 g_CapLevelConvertTable[][3] =
{
    {HI_UNF_VCODEC_CAP_LEVEL_QCIF,          CAP_LEVEL_H264_QCIF,    CAP_LEVEL_MPEG_QCIF},
    {HI_UNF_VCODEC_CAP_LEVEL_CIF,           CAP_LEVEL_H264_CIF,     CAP_LEVEL_MPEG_CIF},
    {HI_UNF_VCODEC_CAP_LEVEL_D1,            CAP_LEVEL_H264_D1,      CAP_LEVEL_MPEG_D1},
    {HI_UNF_VCODEC_CAP_LEVEL_720P,          CAP_LEVEL_H264_720,     CAP_LEVEL_MPEG_720},
    {HI_UNF_VCODEC_CAP_LEVEL_FULLHD,        CAP_LEVEL_H264_FHD,     CAP_LEVEL_MPEG_FHD},
    {HI_UNF_VCODEC_CAP_LEVEL_1280x800,      CAP_LEVEL_1280x800,     CAP_LEVEL_1280x800},
    {HI_UNF_VCODEC_CAP_LEVEL_800x1280,      CAP_LEVEL_800x1280,     CAP_LEVEL_800x1280},
    {HI_UNF_VCODEC_CAP_LEVEL_1488x1280,     CAP_LEVEL_1488x1280,    CAP_LEVEL_1488x1280},
    {HI_UNF_VCODEC_CAP_LEVEL_1280x1488,     CAP_LEVEL_1280x1488,    CAP_LEVEL_1280x1488},
    {HI_UNF_VCODEC_CAP_LEVEL_2160x1280,     CAP_LEVEL_2160x1280,    CAP_LEVEL_2160x1280},
    {HI_UNF_VCODEC_CAP_LEVEL_1280x2160,     CAP_LEVEL_1280x2160,    CAP_LEVEL_1280x2160},
    {HI_UNF_VCODEC_CAP_LEVEL_2160x2160,     CAP_LEVEL_2160x2160,    CAP_LEVEL_2160x2160},
    {HI_UNF_VCODEC_CAP_LEVEL_4096x2160,     CAP_LEVEL_4096x2160,    CAP_LEVEL_4096x2160},
    {HI_UNF_VCODEC_CAP_LEVEL_2160x4096,     CAP_LEVEL_2160x4096,    CAP_LEVEL_2160x4096},
    {HI_UNF_VCODEC_CAP_LEVEL_4096x4096,     CAP_LEVEL_4096x4096,    CAP_LEVEL_4096x4096},
    {HI_UNF_VCODEC_CAP_LEVEL_8192x4096,     CAP_LEVEL_8192x4096,    CAP_LEVEL_8192x4096},
    {HI_UNF_VCODEC_CAP_LEVEL_4096x8192,     CAP_LEVEL_4096x8192,    CAP_LEVEL_4096x8192},
    {HI_UNF_VCODEC_CAP_LEVEL_8192x8192,     CAP_LEVEL_8192x8192,    CAP_LEVEL_8192x8192},

    {HI_UNF_VCODEC_CAP_LEVEL_BUTT,          CAP_LEVEL_BUTT,         CAP_LEVEL_BUTT}, //terminal element
};

HI_U32 VDEC_CapLevelConvert(HI_U32 Code, HI_UNF_VCODEC_PRTCL_LEVEL_E PrctlLevel)
{
    HI_U32 Index = 0;
    HI_U32 TargetIndex = 0;
    HI_U32 ConvertedCode = CAP_LEVEL_BUTT;

    TargetIndex = (PrctlLevel == HI_UNF_VCODEC_PRTCL_LEVEL_H264) ? 1 : 2;
    ConvertedCode = (PrctlLevel == HI_UNF_VCODEC_PRTCL_LEVEL_H264) ? CAP_LEVEL_H264_FHD : CAP_LEVEL_MPEG_FHD;

    while(1)
    {
        if (g_CapLevelConvertTable[Index][0] == HI_UNF_VCODEC_CAP_LEVEL_BUTT
         || g_CapLevelConvertTable[Index][TargetIndex] == CAP_LEVEL_BUTT)
        {
            break;
        }

        if (Code == g_CapLevelConvertTable[Index][0])
        {
            ConvertedCode = g_CapLevelConvertTable[Index][TargetIndex];
            break;
        }

        Index++;
    }

    return ConvertedCode;
}

static VDEC_CHAN_CAP_LEVEL_E VDEC_CapLevelUnfToFmw(HI_UNF_AVPLAY_OPEN_OPT_S* pstVdecCapParam)
{
    if (HI_UNF_VCODEC_DEC_TYPE_ISINGLE == pstVdecCapParam->enDecType)
    {
        return CAP_LEVEL_SINGLE_IFRAME_FHD;
    }
    else if (HI_UNF_VCODEC_DEC_TYPE_NORMAL == pstVdecCapParam->enDecType)
    {
        if (HI_UNF_VCODEC_PRTCL_LEVEL_MVC == pstVdecCapParam->enProtocolLevel)
        {
            return CAP_LEVEL_MVC_FHD;
        }

        return VDEC_CapLevelConvert(pstVdecCapParam->enCapLevel, pstVdecCapParam->enProtocolLevel);
    }
    else
    {
        return CAP_LEVEL_BUTT;
    }
}

static HI_VOID VDEC_InitCapLevel_QCIF(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 176;
    pstChan->stOption.s32MaxHeight = 144;
    pstChan->stOption.s32SCDBufSize = 512 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_CIF(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 352;
    pstChan->stOption.s32MaxHeight = 288;
    pstChan->stOption.s32SCDBufSize = 768 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_D1(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 720;
    pstChan->stOption.s32MaxHeight = 576;
    pstChan->stOption.s32SCDBufSize = 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_720(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 1280;
    pstChan->stOption.s32MaxHeight = 736;
    pstChan->stOption.s32SCDBufSize = 2 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_FHD(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 1920;
    pstChan->stOption.s32MaxHeight = 1088;
    pstChan->stOption.s32SCDBufSize = 3 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_BYDHD(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 5632;
    pstChan->stOption.s32MaxHeight = 4224;
    pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_SINGLE_IFRAME_FHD(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 1920;
    pstChan->stOption.s32MaxHeight = 1088;
    pstChan->stOption.s32SCDBufSize = 2 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_1280x800(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 1280;
    pstChan->stOption.s32MaxHeight = 800;
    pstChan->stOption.s32SCDBufSize = 2 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_800x1280(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 800;
    pstChan->stOption.s32MaxHeight = 1280;
    pstChan->stOption.s32SCDBufSize = 2 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_1488x1280(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 1488;
    pstChan->stOption.s32MaxHeight = 1280;
    pstChan->stOption.s32SCDBufSize = 3 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_1280x1488(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 1280;
    pstChan->stOption.s32MaxHeight = 1488;
    pstChan->stOption.s32SCDBufSize = 3 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_2160x1280(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 2160;
    pstChan->stOption.s32MaxHeight = 1280;
    pstChan->stOption.s32SCDBufSize = 3 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_1280x2160(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 1280;
    pstChan->stOption.s32MaxHeight = 2176;
    pstChan->stOption.s32SCDBufSize = 3 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_2160x2160(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 2160;
    pstChan->stOption.s32MaxHeight = 2176;
    pstChan->stOption.s32SCDBufSize = 4 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_4096x2160(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 4096;
    pstChan->stOption.s32MaxHeight = 2304;
    pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_2160x4096(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 2304;
    pstChan->stOption.s32MaxHeight = 4096;
    pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_4096x4096(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 4096;
    pstChan->stOption.s32MaxHeight = 4096;
    pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_8192x4096(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 8192;
    pstChan->stOption.s32MaxHeight = 4096;
    pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_4096x8192(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 4096;
    pstChan->stOption.s32MaxHeight = 8192;
    pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_8192x8192(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 8192;
    pstChan->stOption.s32MaxHeight = 8192;
    pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}

static HI_VOID VDEC_InitCapLevel_Default(VDEC_CHANNEL_S* pstChan)
{
    pstChan->stOption.s32MaxWidth  = 1920;
    pstChan->stOption.s32MaxHeight = 1088;
    pstChan->stOption.s32SCDBufSize = 3 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);

    return ;
}


static const CapLEVEL_NODE g_CapLEVEL_Table[] =
{
    {CAP_LEVEL_MPEG_QCIF,                   VDEC_InitCapLevel_QCIF},
    {CAP_LEVEL_H264_QCIF,                   VDEC_InitCapLevel_QCIF},
    {CAP_LEVEL_MPEG_CIF,                    VDEC_InitCapLevel_CIF},
    {CAP_LEVEL_H264_CIF,                    VDEC_InitCapLevel_CIF},
    {CAP_LEVEL_MPEG_D1,                     VDEC_InitCapLevel_D1},
    {CAP_LEVEL_H264_D1,                     VDEC_InitCapLevel_D1},
    {CAP_LEVEL_MPEG_720,                    VDEC_InitCapLevel_720},
    {CAP_LEVEL_H264_720,                    VDEC_InitCapLevel_720},
    {CAP_LEVEL_MPEG_FHD,                    VDEC_InitCapLevel_FHD},
    {CAP_LEVEL_H264_FHD,                    VDEC_InitCapLevel_FHD},
    {CAP_LEVEL_MVC_FHD,                     VDEC_InitCapLevel_FHD},
    {CAP_LEVEL_H264_BYDHD,                  VDEC_InitCapLevel_BYDHD},
    {CAP_LEVEL_SINGLE_IFRAME_FHD,           VDEC_InitCapLevel_SINGLE_IFRAME_FHD},
    {CAP_LEVEL_1280x800,                    VDEC_InitCapLevel_1280x800},
    {CAP_LEVEL_800x1280,                    VDEC_InitCapLevel_800x1280},
    {CAP_LEVEL_1488x1280,                   VDEC_InitCapLevel_1488x1280},
    {CAP_LEVEL_1280x1488,                   VDEC_InitCapLevel_1280x1488},
    {CAP_LEVEL_2160x1280,                   VDEC_InitCapLevel_2160x1280},
    {CAP_LEVEL_1280x2160,                   VDEC_InitCapLevel_1280x2160},
    {CAP_LEVEL_2160x2160,                   VDEC_InitCapLevel_2160x2160},
    {CAP_LEVEL_4096x2160,                   VDEC_InitCapLevel_4096x2160},
    {CAP_LEVEL_2160x4096,                   VDEC_InitCapLevel_2160x4096},
    {CAP_LEVEL_4096x4096,                   VDEC_InitCapLevel_4096x4096},
    {CAP_LEVEL_8192x4096,                   VDEC_InitCapLevel_8192x4096},
    {CAP_LEVEL_4096x8192,                   VDEC_InitCapLevel_4096x8192},
    {CAP_LEVEL_8192x8192,                   VDEC_InitCapLevel_8192x8192},

    {CAP_LEVEL_BUTT,        HI_NULL}, //terminal element
};

FN_CapLEVEL_HANDLER VDEC_CapLEVEL_Handler(HI_U32 Code)
{
    HI_U32 Index = 0;
    FN_CapLEVEL_HANDLER pTargetHandler = HI_NULL;

    while(1)
    {
        if (g_CapLEVEL_Table[Index].Code == CAP_LEVEL_BUTT || g_CapLEVEL_Table[Index].pHandler == HI_NULL)
        {
            break;
        }

        if (Code == g_CapLEVEL_Table[Index].Code)
        {
            pTargetHandler = g_CapLEVEL_Table[Index].pHandler;
            break;
        }

        Index++;
    }

    return pTargetHandler;
}

static HI_S32 VDEC_Chan_InitParam(HI_HANDLE hHandle, HI_UNF_AVPLAY_OPEN_OPT_S* pstCapParam)
{
    HI_U32 i = 0;

    HI_S32                      s32Ret;
    VDEC_CHANNEL_S*              pstChan = HI_NULL;
    VDEC_CHAN_CAP_LEVEL_E       enCapToFmw;
    HI_SYS_MEM_CONFIG_S         stMemConfig = {0};
    FN_CapLEVEL_HANDLER         pCapLevel_Handler = HI_NULL;

    /* check input parameters */
    if (HI_NULL == pstCapParam)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Allocate resource */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        pstChan = HI_VMALLOC_VDEC(sizeof(VDEC_CHANNEL_S));

        if (HI_NULL == pstChan)
        {
            HI_ERR_VDEC("No memory\n");
            goto err0;
        }
        else
        {
            /* Initialize the channel attributes */
            memset(pstChan, 0, sizeof(VDEC_CHANNEL_S));
            s_stVdecDrv.astChanEntity[hHandle].pstChan = pstChan;
        }
    }
    else
    {
        pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    }

    pstChan->hVdec = hHandle;
    pstChan->hChan = HI_INVALID_HANDLE;
    pstChan->hStrmBuf = HI_INVALID_HANDLE;
    pstChan->u32StrmBufSize = 0;
    pstChan->hDmxVidChn = HI_INVALID_HANDLE;
    pstChan->u32DmxBufSize = 0;
    pstChan->bNormChange = HI_FALSE;
    pstChan->stNormChangeParam.enNewFormat    = HI_UNF_ENC_FMT_BUTT;
    pstChan->stNormChangeParam.u32ImageWidth  = 0;
    pstChan->stNormChangeParam.u32ImageHeight = 0;
    pstChan->stNormChangeParam.u32FrameRate   = 0;
    pstChan->stNormChangeParam.bProgressive = HI_FALSE;
    pstChan->stIFrame.st2dBuf.u32Size = 0;
    pstChan->bNewFrame = HI_FALSE;
    pstChan->bFramePackingChange = HI_FALSE;
    pstChan->bNewSeq = HI_FALSE;
    pstChan->bNewUserData = HI_FALSE;
    pstChan->bIFrameErr = HI_FALSE;
    pstChan->bErrorFrame = HI_FALSE;
    pstChan->bUnSupportStream = HI_FALSE;
    pstChan->pstUsrData = HI_NULL;
    pstChan->stFrameRateParam.enFrmRateType = HI_UNF_AVPLAY_FRMRATE_TYPE_PTS;
    pstChan->stFrameRateParam.stSetFrmRate.u32fpsInteger = 25;
    pstChan->stFrameRateParam.stSetFrmRate.u32fpsDecimal = 0;
    pstChan->bSetEosFlag = HI_FALSE;
    pstChan->bProcRegister = HI_FALSE;
    pstChan->u8ResolutionChange = 0;

    //pstChan->u32DiscontinueCount = 0;

    pstChan->s32Speed = 1024;
    pstChan->u32FrameCnt = 0;
    pstChan->s32FindStartCodeCnt = 0;
    pstChan->s32Mpeg2Score = 0;
    pstChan->s32H264Score = 0;
    atomic_set(&pstChan->stBTL.atmWorking, 0);
    init_waitqueue_head(&pstChan->stBTL.stWaitQue);
    pstChan->stBTL.pstFrame = HI_NULL;

    pstChan->bIsIFrameDec = HI_FALSE;
    pstChan->u32ErrRatio = 0;
    pstChan->u32LastFrmId = -1;
    pstChan->OmxTunnelMode = s_stVdecDrv.astChanEntity[hHandle].u32TunnelModeEn;
    pstChan->hOmxChn = HI_INVALID_HANDLE;
    /* Get proper buffer size */
    enCapToFmw = VDEC_CapLevelUnfToFmw(pstCapParam);
    pstChan->enCapToFmw = enCapToFmw;
    pstChan->stOption.eAdapterType = ADAPTER_TYPE_VDEC;
    pstChan->stOption.Purpose = PURPOSE_DECODE;
    pstChan->stOption.MemAllocMode = MODE_PART_BY_SDK;
    pstChan->stProbeStreamInfo.bProbeCodecTypeChangeFlag = HI_FALSE;
    pstChan->stOption.u32DynamicFrameStoreAllocEn = ((MaskCtrlWord & 0x1) == 1) ? 0 : s_stVdecDrv.astChanEntity[hHandle].u32DynamicFsEn; //l00273086 使能动态分配功能
    pstChan->stOption.s32ExtraFrameStoreNum = PLUS_FS_NUM + 1;//l00273086 PLUS_FS_NUM + 1:流畅播放防止阻塞
    pstChan->stOption.s32DelayTime = 0;//l00273086 动态分配帧存时，若分配不到内存vfmw等待的时间
    pstChan->stOption.u32CfgFrameNum = 0;//l00273086 配置帧数
    pstChan->stOption.u32MaxMemUse = -1;//l00273086最大可用帧存

    pstChan->stOption.s32MaxSliceNum = 136;
    pstChan->stOption.s32MaxSpsNum = 32;
    pstChan->stOption.s32MaxPpsNum = 256;
    pstChan->stOption.s32SupportBFrame = 1;
    pstChan->stOption.s32SupportH264 = 1;
    pstChan->stOption.s32ReRangeEn = 1;     /* Support rerange frame buffer when definition change */
    pstChan->stOption.s32SlotWidth = 0;
    pstChan->stOption.s32SlotHeight = 0;

    //init hdr param
#ifdef HI_HDR_SUPPORT
    pstChan->stVdecHdrAttr.enVdecHDRType = HI_VDEC_HDR_CHAN_TYPE_BUTT;
    pstChan->stVdecHdrAttr.AvplayHDRAttr.enHDRStreamType = HI_UNF_AVPLAY_HDR_STREAM_TYPE_BUTT;
#endif
    VDEC_InitFrmBufRecSpinLock(&(pstChan->frameBufRecLock));
    VDEC_InitChanControlStateLock(&(pstChan->chanControlStateLock));
    VDEC_InitChanControlStateLock(&(pstChan->stAllocMemLock));

    memset(pstChan->stFrameBufferRecord, 0, sizeof(CHAN_FRAME_BUFFER_S) * VDEC_MAX_BUFFER_RECORD);


    for (i = 0; i < VDEC_MAX_BUFFER_RECORD; i++)
    {
        pstChan->stFrameBufferRecord[i].enFrameBufferNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
        pstChan->stFrameBufferRecord[i].enHdrBufferNodeStatus   = DRV_VDEC_BUFFER_STATUS_BUTT;
        pstChan->stFrameBufferRecord[i].enPmvBufferNodeStatus   = DRV_VDEC_BUFFER_STATUS_BUTT;
        pstChan->stFrameBufferRecord[i].bAvailable              = HI_FALSE;
        pstChan->stFrameBufferRecord[i].bConfigured             = HI_FALSE;
        pstChan->stFrameBufferRecord[i].bWaitRelease          = HI_FALSE;
    }

    pCapLevel_Handler = VDEC_CapLEVEL_Handler(enCapToFmw);
    if (pCapLevel_Handler == HI_NULL)
    {
        VDEC_InitCapLevel_Default(pstChan);
    }
    else
    {
        pCapLevel_Handler(pstChan);
    }

#if defined(HI_VIDEO_MAX_REF_FRAME_NUM) && defined(HI_VIDEO_MAX_DISP_FRAME_NUM) && defined(HI_VIDEO_MAX_VDH_BUF)
    pstChan->stOption.s32MaxRefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM;
    pstChan->stOption.s32DisplayFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM - HI_VDEC_BUFFER_FRAME;
    RefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM;
    DispFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM;
    pstChan->stOption.s32ExtraFrameStoreNum = HI_VIDEO_MAX_DISP_FRAME_NUM;
    pstChan->stOption.u32MaxMemUse = HI_VIDEO_MAX_VDH_BUF * 1024 * 1024;
    pstChan->stOption.u32DynamicFrameStoreAllocEn = 1;

#else

    /* calculate refrence frame number and display frame number */
    s32Ret = HI_DRV_SYS_GetMemConfig(&stMemConfig);

    if (HI_SUCCESS != s32Ret)
    {
        stMemConfig.u32TotalSize = 1024;
    }

    if (stMemConfig.u32TotalSize > 1536 && stMemConfig.u32TotalSize <= 2048)//2G内存下解码参数配置
    {
        pstChan->stOption.s32MaxRefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_2048;
        pstChan->stOption.s32DisplayFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_2048 - HI_VDEC_BUFFER_FRAME;
        RefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_2048;
        DispFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_2048;
        pstChan->stOption.s32ExtraFrameStoreNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_2048;
        pstChan->stOption.u32MaxMemUse = HI_VIDEO_MAX_VDH_BUF_IN_2048 * 1024 * 1024;
    }
    else if (stMemConfig.u32TotalSize > 768 && stMemConfig.u32TotalSize <= 1024)//1G内存下解码参数配置
    {
        pstChan->stOption.s32MaxRefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_1024;
        pstChan->stOption.s32DisplayFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_1024 - HI_VDEC_BUFFER_FRAME;
        RefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_1024;
        DispFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_1024;
        pstChan->stOption.s32ExtraFrameStoreNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_1024;
        pstChan->stOption.u32MaxMemUse = HI_VIDEO_MAX_VDH_BUF_IN_1024 * 1024 * 1024;
    }
    else if (stMemConfig.u32TotalSize > 512 && stMemConfig.u32TotalSize <= 768)//768M内存下解码参数配置
    {
        pstChan->stOption.s32MaxRefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_768;
        pstChan->stOption.s32DisplayFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_768 - HI_VDEC_BUFFER_FRAME;
        RefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_768;
        DispFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_768;
        pstChan->stOption.s32ExtraFrameStoreNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_768;
        pstChan->stOption.u32MaxMemUse = HI_VIDEO_MAX_VDH_BUF_IN_768 * 1024 * 1024;
        if ((enCapToFmw >= CAP_LEVEL_4096x2160) && (enCapToFmw <= CAP_LEVEL_4096x4096))
        {
           pstChan->stOption.s32SCDBufSize = 10 * 1024 * 1024;
        }
    }
    else if (stMemConfig.u32TotalSize > 256 && stMemConfig.u32TotalSize <= 512)//512M内存下解码参数配置
    {
        pstChan->stOption.s32MaxRefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_512;
        pstChan->stOption.s32DisplayFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_512 - HI_VDEC_BUFFER_FRAME;
        RefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_512;
        DispFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_512;
        pstChan->stOption.s32ExtraFrameStoreNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_512;
        pstChan->stOption.u32MaxMemUse = HI_VIDEO_MAX_VDH_BUF_IN_512 * 1024 * 1024;
        pstChan->stOption.u32DynamicFrameStoreAllocEn = 1;
        if ((enCapToFmw >= CAP_LEVEL_4096x2160) && (enCapToFmw <= CAP_LEVEL_4096x4096))
        {
            pstChan->stOption.s32SCDBufSize = 5 * 1024 * 1024;
        }
    }
    else if (stMemConfig.u32TotalSize <= 256)//256M内存解码参数配置
    {
        pstChan->stOption.s32MaxRefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_256;
        pstChan->stOption.s32DisplayFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_256 - HI_VDEC_BUFFER_FRAME;
        RefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_256;
        DispFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_256;
        pstChan->stOption.s32ExtraFrameStoreNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_256;
        pstChan->stOption.u32MaxMemUse = HI_VIDEO_MAX_VDH_BUF_IN_256 * 1024 * 1024;
        pstChan->stOption.u32DynamicFrameStoreAllocEn = 1;
    }
    else //默认使用1G内存下解码参数配置
    {
        pstChan->stOption.s32MaxRefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_1024;
        pstChan->stOption.s32DisplayFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_1024 - HI_VDEC_BUFFER_FRAME;
        RefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_1024;
        DispFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_1024;
        pstChan->stOption.s32ExtraFrameStoreNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_1024;
        pstChan->stOption.u32MaxMemUse = HI_VIDEO_MAX_VDH_BUF_IN_1024 * 1024 * 1024;
    }
#endif

    //在没打开DFS的时候，对于MVC的流直接使用menuconfig中的帧存配置选项，会导致有些MVC的流因帧存不够而不能播放。如果在menuconfig中更改，又会导致每个通道都分配过多帧存，而出现帧存不够的情况。按照产品建议未打开DFS时在底层写死。
    if ((1 != pstChan->stOption.u32DynamicFrameStoreAllocEn) && (CAP_LEVEL_MVC_FHD == enCapToFmw))
    {
        pstChan->stOption.s32MaxRefFrameNum = 16;
        pstChan->stOption.s32DisplayFrameNum = HI_VDEC_BUFFER_FRAME;
    }

    /* 1: Enable 1D to 2D, the data got by VOU is 2D; 0: Disable */
    /* Can't support 1D->2D switching. Because 2D must allocate memory when VDEC_CID_CREATE_CHAN. */
    pstChan->stOption.s32Btl1Dt2DEnable = En2d;     /* Default 1D */
    /* 1: Enable DbDr info calculation, used by DNR in VOU; 0: Disable */
    pstChan->stOption.s32BtlDbdrEnable = 1;         /* DNR Enable */

    /* Register proc file of this chan */
    if (HI_FALSE == pstChan->bProcRegister)
    {
        s32Ret = VDEC_RegChanProc(hHandle);

        if (HI_SUCCESS == s32Ret)
        {
            pstChan->bProcRegister = HI_TRUE;
        }
        else
        {
            pstChan->bProcRegister = HI_FALSE;
        }
    }

    /* Set default config */
    pstChan->stCurCfg = s_stVdecDrv.stDefCfg;

    pstChan->enDisplayNorm = HI_UNF_ENC_FMT_BUTT;
    pstChan->stLastFrm.eFrmType = HI_DRV_FT_BUTT;
    pstChan->u32ValidPtsFlag = 0;

    /* Alloc pts recover channel */
    PTSREC_Alloc(hHandle);

    /* Update information of VDEC device */
    // s_stVdecDrv.astChanEntity[hHandle].pstChan = pstChan;
    pstChan->stUserCfgCap = *pstCapParam;

    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVdec = hHandle;

#ifdef TEST_VDEC_SAVEFILE
    VDEC_Dbg_OpenSaveFile(hHandle);
#endif

    HI_INFO_VDEC("Chan %d alloc OK!\n", hHandle);
    return HI_SUCCESS;

err0:
    return HI_FAILURE;
}


static HI_S32 VDEC_Chan_AllocHandle(HI_HANDLE* phHandle, struct file* pstFile)
{
    HI_U32 i, j;
    HI_S32 s32Ret;
    HI_UNF_AVPLAY_OPEN_OPT_S stCapParam;
    VDEC_CHANNEL_S*  pstChan = HI_NULL;

    if (HI_NULL == phHandle)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    memset(&stCapParam, 0, sizeof(HI_UNF_AVPLAY_OPEN_OPT_S));

    /* Check ready flag */
    if (s_stVdecDrv.bReady != HI_TRUE)
    {
        HI_ERR_VDEC("Need open first!\n");
        return HI_ERR_VDEC_NOT_OPEN;
    }

    /* Lock */
    down(&s_stVdecDrv.stSem);
    //if (down(&s_stVdecDrv.stSem))
    //{
        //HI_ERR_VDEC("Global lock err!\n");
        //return HI_FAILURE;
    //}

    /* Check channel number */
    if ((s_stVdecDrv.u32ChanNum >= HI_VDEC_MAX_INSTANCE_NEW)
        || (s_stVdecDrv.u32ChanNum >= s_stVdecDrv.stVdecCap.s32MaxChanNum))
    {
        HI_ERR_VDEC("Too many chans:%d!\n", s_stVdecDrv.u32ChanNum);
        goto err0;
    }

    /* Allocate new channel */
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (HI_FALSE == s_stVdecDrv.astChanEntity[i].bUsed)
        {
            s_stVdecDrv.astChanEntity[i].bUsed = HI_TRUE;
            s_stVdecDrv.astChanEntity[i].pstChan = HI_NULL;
            s_stVdecDrv.astChanEntity[i].eCallBack = 0;
            s_stVdecDrv.astChanEntity[i].DmxHdlCallBack = 0;
            s_stVdecDrv.astChanEntity[i].u64File = (HI_U64)HI_NULL;
            atomic_set(&s_stVdecDrv.astChanEntity[i].atmUseCnt, 0);
            atomic_set(&s_stVdecDrv.astChanEntity[i].atmRlsFlag, 0);
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Too many chans!\n");
        goto err0;
    }

    s_stVdecDrv.astChanEntity[i].pstChan = HI_NULL;
    s_stVdecDrv.astChanEntity[i].u64File = (HI_U64)(HI_SIZE_T)pstFile;

    s_stVdecDrv.astChanEntity[i].stVpssChan.hVdec = i;
    s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss = HI_INVALID_HANDLE;
    s_stVdecDrv.astChanEntity[i].stVpssChan.stControlInfo.u32BackwardOptimizeFlag = HI_FALSE;
    s_stVdecDrv.astChanEntity[i].stVpssChan.stControlInfo.u32DispOptimizeFlag = HI_FALSE;
    s_stVdecDrv.astChanEntity[i].stVpssChan.s32GetFirstIFrameFlag = HI_FALSE;
    s_stVdecDrv.astChanEntity[i].stVpssChan.eLastFrameFormat = VDEC_FRAME_BUTT;
    //s_stVdecDrv.astChanEntity[i].stVpssChan.s32LastFrameGopNum = -1;
    s_stVdecDrv.astChanEntity[i].stVpssChan.s32Speed = 1024;
    s_stVdecDrv.astChanEntity[i].stVpssChan.s32ImageDistance = 0;

    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].hPort = HI_INVALID_HANDLE;
        s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].bEnable = HI_FALSE;
        s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].enPortType = VDEC_PORT_TYPE_BUTT;
        s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].s32PortTmpListPos = 0;
        s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].s32GetFirstVpssFrameFlag = HI_FALSE;
        s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].s32RecvNewFrame = HI_FALSE;
        s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].s32PortLastFrameGopNum = -1;
        s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].u32LastFrameIndex = -1;
        INIT_LIST_HEAD(&s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].stPortList.stVdecPortFrameList);
        s32Ret = VDEC_InitSpinLock(&s_stVdecDrv.astChanEntity[i].stVpssChan.stPort[j].stPortList.stPortFrameListLock);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("VDEC_InitSpinLock Err:%d\n", s32Ret);
        }

    }

    s_stVdecDrv.u32ChanNum++;
    *phHandle = (HI_ID_VDEC << 16) | i;


    if ( HI_NULL == s_stVdecDrv.astChanEntity[i].pstChan)
    {
        pstChan = HI_VMALLOC_VDEC(sizeof(VDEC_CHANNEL_S));

        if (HI_NULL == pstChan)
        {
            HI_ERR_VDEC("No memory\n");
            goto err0;
        }
        else
        {
            /* Initialize the channel attributes */
            memset(pstChan, 0, sizeof(VDEC_CHANNEL_S));
            pstChan->hVdec = HI_INVALID_HANDLE;
            pstChan->hChan = HI_INVALID_HANDLE;
            pstChan->bTvp  = HI_FALSE;
            /* Initalize the kernel event */
            pstChan->bWakeUp        = HI_FALSE;
            pstChan->bMasterWakeUp  = HI_FALSE;
            pstChan->bSlaveWakeUp   = HI_FALSE;
            pstChan->bVirtualWakeUp = HI_FALSE;
            VDEC_InitEvent(pstChan->WaitQueue);
        }
    }

    s_stVdecDrv.astChanEntity[i].pstChan = pstChan;
    up(&s_stVdecDrv.stSem);
    return HI_SUCCESS;

err0:
    up(&s_stVdecDrv.stSem);
    return HI_FAILURE;
}

static HI_S32 VDEC_Chan_FreeHandle(HI_HANDLE hHandle)
{
    //HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Invalid handle :%d\n", hHandle);
        return HI_FAILURE;
    }

    /* Clear global parameter */
    down(&s_stVdecDrv.stSem);
    //s32Ret = down(&s_stVdecDrv.stSem);

    //if (0 != s32Ret)
    //{
       // HI_WARN_VDEC("down_interruptible return %d\n", s32Ret);
    //}

    if (s_stVdecDrv.u32ChanNum > 0)
    {
        s_stVdecDrv.u32ChanNum--;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    s_stVdecDrv.astChanEntity[hHandle].bUsed = HI_FALSE;
    s_stVdecDrv.astChanEntity[hHandle].pstChan = HI_NULL;
    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVdec = HI_INVALID_HANDLE;
    s_stVdecDrv.astChanEntity[hHandle].u64File = (HI_U64)HI_NULL;
    HI_VFREE_VDEC(pstChan);
    up(&s_stVdecDrv.stSem);
    return HI_SUCCESS;
}

HI_S32 VDEC_Chan_FindPreMem(VDEC_BUFFER_S* pstMemBuffer)
{
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U32 u32Size = 0;

    for (i = 0; i < g_VdecPreMMZNodeNum; i++)
    {
        if (st_VdecChanPreUsedMMZInfo[i].u32Size >= pstMemBuffer->u32Size && 0 == st_VdecChanPreUsedMMZInfo[i].u32NodeState)
        {
            break;
        }
    }

    if (i < g_VdecPreMMZNodeNum)
    {
        if (pstMemBuffer->u32Size <= st_VdecChanPreUsedMMZInfo[i].u32Size)
        {

            if (0 == i)
            {
                if (st_VdecChanPreUsedMMZInfo[i + 1].u32Size > 0)
                {
                    if (1 == st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState)
                    {
                        u32Size = st_VdecChanPreUsedMMZInfo[i].u32Size - pstMemBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i].u32Size           = pstMemBuffer->u32Size;

                        if (u32Size > 0)
                        {
                            for (j = g_VdecPreMMZNodeNum; j > (i + 1); j--)
                            {
                                memcpy(&st_VdecChanPreUsedMMZInfo[j], &st_VdecChanPreUsedMMZInfo[j - 1], sizeof(VDEC_PREMMZ_NODE_S));
                            }

                            st_VdecChanPreUsedMMZInfo[i + 1].u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr + pstMemBuffer->u32Size;
                            st_VdecChanPreUsedMMZInfo[i + 1].pu8StartVirAddr = st_VdecChanPreUsedMMZInfo[i].pu8StartVirAddr + pstMemBuffer->u32Size;
                            st_VdecChanPreUsedMMZInfo[i + 1].u32Size         = u32Size;
                            st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState    = 0;
                            g_VdecPreMMZNodeNum ++;
                        }
                    }
                    else
                    {
                        u32Size = st_VdecChanPreUsedMMZInfo[i].u32Size - pstMemBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i].u32Size           = pstMemBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr + pstMemBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].pu8StartVirAddr = st_VdecChanPreUsedMMZInfo[i].pu8StartVirAddr + pstMemBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].u32Size         = st_VdecChanPreUsedMMZInfo[i + 1].u32Size + u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState    = 0;

                    }
                }
                else
                {
                    g_VdecPreMMZNodeNum ++;
                    u32Size = st_VdecChanPreUsedMMZInfo[i].u32Size - pstMemBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i].u32Size           = pstMemBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i + 1].u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr + pstMemBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i + 1].pu8StartVirAddr = st_VdecChanPreUsedMMZInfo[i].pu8StartVirAddr + pstMemBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i + 1].u32Size         = st_VdecChanPreUsedMMZInfo[i + 1].u32Size + u32Size;
                    st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState    = 0;
                }
            }
            else if (i == g_VdecPreMMZNodeNum - 1)
            {
                for (j = 0; j < i; j++)
                {
                    u32Size += st_VdecChanPreUsedMMZInfo[j].u32Size;
                }

                u32Size += pstMemBuffer->u32Size;
                st_VdecChanPreUsedMMZInfo[i].u32Size           = pstMemBuffer->u32Size;
                st_VdecChanPreUsedMMZInfo[i + 1].u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr + pstMemBuffer->u32Size;
                st_VdecChanPreUsedMMZInfo[i + 1].pu8StartVirAddr = st_VdecChanPreUsedMMZInfo[i].pu8StartVirAddr + pstMemBuffer->u32Size;
                st_VdecChanPreUsedMMZInfo[i + 1].u32Size         = g_stVDHMMZ.u32Size - u32Size;
                st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState    = 0;
                g_VdecPreMMZNodeNum ++;
            }
            else
            {
                u32Size = st_VdecChanPreUsedMMZInfo[i].u32Size - pstMemBuffer->u32Size;

                if (st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState == 0)
                {
                    st_VdecChanPreUsedMMZInfo[i].u32Size           = pstMemBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i + 1].u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr + pstMemBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i + 1].pu8StartVirAddr = st_VdecChanPreUsedMMZInfo[i].pu8StartVirAddr + pstMemBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i + 1].u32Size         = st_VdecChanPreUsedMMZInfo[i + 1].u32Size + u32Size;
                }
                else
                {
                    u32Size = st_VdecChanPreUsedMMZInfo[i].u32Size - pstMemBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i].u32Size           = pstMemBuffer->u32Size;

                    if (u32Size > 0)
                    {
                        for (j = g_VdecPreMMZNodeNum; j > (i + 1); j--)
                        {
                            memcpy(&st_VdecChanPreUsedMMZInfo[j], &st_VdecChanPreUsedMMZInfo[j - 1], sizeof(VDEC_PREMMZ_NODE_S));
                        }

                        st_VdecChanPreUsedMMZInfo[i + 1].u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr + pstMemBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].pu8StartVirAddr = st_VdecChanPreUsedMMZInfo[i].pu8StartVirAddr + pstMemBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].u32Size         = u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState    = 0;
                        g_VdecPreMMZNodeNum ++;
                    }
                }
            }

            st_VdecChanPreUsedMMZInfo[i].u32NodeState = 1;
            pstMemBuffer->u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr;
            pstMemBuffer->pu8StartVirAddr = st_VdecChanPreUsedMMZInfo[i].pu8StartVirAddr;
            return HI_SUCCESS;
        }
    }
    else
    {
        return HI_FAILURE;
    }
    return HI_FAILURE;
}





static HI_S32 VDEC_Chan_GetMemSize(HI_HANDLE hHandle)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    HI_S8 as8TmpBuf[128];
    HI_U32 FrmNum = 0;

    memset(as8TmpBuf, 0, sizeof(as8TmpBuf));
    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("VDEC_Chan_GetMemSize err , chan handle:%d invalid!\n", hHandle);
        return HI_FAILURE;
    }

    /* Hevc计算内存公式不一样，在GetMemSize调用前，通过传递一个HEVC的能力集类型来识别 */
    if (HI_UNF_VCODEC_TYPE_HEVC == pstChan->stCurCfg.enType)
    {
        switch (pstChan->enCapToFmw)
        {
            case CAP_LEVEL_MPEG_QCIF:
            case CAP_LEVEL_H264_QCIF:
            case CAP_LEVEL_HEVC_QCIF:
                pstChan->stOption.s32MaxWidth  = 192;
                pstChan->stOption.s32MaxHeight = 192;
                pstChan->enCapToFmw            = CAP_LEVEL_HEVC_QCIF;
                break;

            case CAP_LEVEL_MPEG_CIF:
            case CAP_LEVEL_H264_CIF:
            case CAP_LEVEL_HEVC_CIF:
                pstChan->stOption.s32MaxWidth  = 384;
                pstChan->stOption.s32MaxHeight = 320;
                pstChan->enCapToFmw            = CAP_LEVEL_HEVC_CIF;
                break;

            case CAP_LEVEL_MPEG_D1:
            case CAP_LEVEL_H264_D1:
            case CAP_LEVEL_HEVC_D1:
                pstChan->stOption.s32MaxWidth  = 768;
                pstChan->stOption.s32MaxHeight = 576;
                pstChan->enCapToFmw            = CAP_LEVEL_HEVC_D1;
                break;

            case CAP_LEVEL_MPEG_720:
            case CAP_LEVEL_H264_720:
            case CAP_LEVEL_HEVC_720:
                pstChan->stOption.s32MaxWidth  = 1280;
                pstChan->stOption.s32MaxHeight = 768;
                pstChan->enCapToFmw            = CAP_LEVEL_HEVC_720;
                break;

            case CAP_LEVEL_MPEG_FHD:
            case CAP_LEVEL_H264_FHD:
            case CAP_LEVEL_MVC_FHD:
            case CAP_LEVEL_HEVC_FHD:
                pstChan->stOption.s32MaxWidth  = 1920;
                pstChan->stOption.s32MaxHeight = 1088;
                pstChan->enCapToFmw            = CAP_LEVEL_HEVC_FHD;
                break;

            default:
                pstChan->stOption.s32MaxWidth  = 4096;
                pstChan->stOption.s32MaxHeight = 2304;
                pstChan->enCapToFmw            = CAP_LEVEL_HEVC_UHD;
                break;
        }
    }
    else if (HI_UNF_VCODEC_TYPE_MJPEG == pstChan->stCurCfg.enType)
    {
        pstChan->enCapToFmw            = CAP_LEVEL_H264_FHD;
        pstChan->stOption.s32MaxWidth  = 1920;
        pstChan->stOption.s32MaxHeight = 1088;
        pstChan->stOption.s32MaxRefFrameNum = RefFrameNum - (PLUS_FS_NUM / 2);
        FrmNum = pstChan->stOption.s32MaxRefFrameNum + pstChan->stOption.s32DisplayFrameNum;

        if (FrmNum < HI_VDEC_SVDEC_FRAME_MIN)
        {
            pstChan->stOption.s32MaxRefFrameNum = HI_VDEC_SVDEC_FRAME_MIN;
            pstChan->stOption.s32DisplayFrameNum = 0;
        }

    }
    else if (HI_UNF_VCODEC_TYPE_H263 == pstChan->stCurCfg.enType || HI_UNF_VCODEC_TYPE_SORENSON == pstChan->stCurCfg.enType)
    {
        pstChan->enCapToFmw = CAP_LEVEL_H264_FHD;
        pstChan->stOption.s32MaxWidth  = 1920;
        pstChan->stOption.s32MaxHeight = 1088;
    }

    pstChan->stOption.s32IsSecMode = pstChan->bTvp;

    ((HI_U64*)as8TmpBuf)[0] = (HI_U64)pstChan->enCapToFmw;
    ((HI_U64*)as8TmpBuf)[1] = (HI_U64)(HI_SIZE_T)(&pstChan->stOption);  //@l00284814 64bit
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(HI_INVALID_HANDLE, VDEC_CID_GET_CHAN_DETAIL_MEMSIZE_WITH_OPTION, as8TmpBuf);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_Chan_GetMemSize failed!\n");
        return HI_FAILURE;
    }

    pstChan->stMemSize = *(DETAIL_MEM_SIZE*)as8TmpBuf;

    return HI_SUCCESS;
}

#ifdef HI_TEE_SUPPORT
HI_S32 VDEC_Chan_AllocMemForTEE(VDEC_CHANNEL_S* pstChan, HI_HANDLE hHandle)
{
    if (pstChan->stMemSize.ScdDetailMem > 0)
    {
        if (HI_SUCCESS != HI_DRV_VDEC_Alloc_TVP("VDEC_Sec_Eos", HI_NULL, HI_VDEC_EOS_MEM_SIZE, 0, &pstChan->stEOSBuffer))
        {
            HI_ERR_VDEC("Chan %d alloc SEC Eos Mem err!\n", hHandle);
            return HI_FAILURE;;
        }
    }

    if (1 != pstChan->stOption.u32DynamicFrameStoreAllocEn)
    {
        if (pstChan->stMemSize.VdhDetailMem > 0)
        {
            if (HI_SUCCESS != HI_DRV_VDEC_Alloc_TVP("VDEC_Sec_Vdh", HI_NULL, pstChan->stMemSize.VdhDetailMem, 0, &pstChan->stVDHMMZBuf))
            {
                HI_ERR_VDEC("Chan %d alloc SEC vdh Mem err!\n", hHandle);
                HI_DRV_VDEC_Release_TVP(&pstChan->stEOSBuffer);
                return HI_FAILURE;;
            }
        }
    }
    else
    {
        pstChan->stVDHMMZBuf.u32Size = 0;
        pstChan->stVDHMMZBuf.u32StartPhyAddr = HI_NULL;
        pstChan->stVDHMMZBuf.pu8StartVirAddr = HI_NULL;
    }

    pstChan->stOption.Purpose = PURPOSE_DECODE;
    pstChan->stOption.MemDetail.ChanMemCtx.Length  = 0;
    pstChan->stOption.MemDetail.ChanMemCtx.PhyAddr = 0;
    pstChan->stOption.MemDetail.ChanMemCtx.VirAddr = HI_NULL;
    pstChan->stOption.MemDetail.ChanMemScd.Length  = 0;
    pstChan->stOption.MemDetail.ChanMemScd.PhyAddr = 0;
    pstChan->stOption.MemDetail.ChanMemScd.VirAddr = HI_NULL;
    pstChan->stOption.MemDetail.ChanMemVdh.Length  = pstChan->stVDHMMZBuf.u32Size;
    pstChan->stOption.MemDetail.ChanMemVdh.PhyAddr = pstChan->stVDHMMZBuf.u32StartPhyAddr;
    pstChan->stOption.MemDetail.ChanMemVdh.VirAddr = HI_NULL;

    return HI_SUCCESS;
}
#endif

HI_VOID AllocVFMWMemGetVDHSize(VDEC_CHANNEL_S* pstChan, HI_U32 *p32VDHSize)
{
#if (1 == HI_VDEC_SVDEC_SUPPORT)

    if (pstChan->stCurCfg.enType == HI_UNF_VCODEC_TYPE_H263 || pstChan->stCurCfg.enType == HI_UNF_VCODEC_TYPE_SORENSON)
    {
        HI_U32 ScdBufSize = 0;
        HI_U32 VdhBufSize = 0;

        VDEC_GetMemConfig(&ScdBufSize, &VdhBufSize);

        *p32VDHSize = (pstChan->stMemSize.VdhDetailMem >= HI_VDEC_SVDEC_VDH_MEM) ? pstChan->stMemSize.VdhDetailMem : HI_VDEC_SVDEC_VDH_MEM;

        if (VdhBufSize > 0)
        {
            *p32VDHSize = (VdhBufSize >= (*p32VDHSize)) ? (*p32VDHSize) : (VdhBufSize);
        }
    }
    else
#endif
    {
        *p32VDHSize = pstChan->stMemSize.VdhDetailMem;
    }

    return;
}

HI_S32 VDEC_Chan_AllocVFMWMem(HI_HANDLE hHandle)
{
    HI_S32 s32Ret;
    HI_U32 u32VDHSize = 0;

#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
    VDEC_BUFFER_S stMemBuffer;
#endif
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);

    /*1:获取通道所需内存*/
    s32Ret = VDEC_Chan_GetMemSize(hHandle);
    VDEC_ASSERT_RETURN(s32Ret == HI_SUCCESS, HI_FAILURE);

    /*2:分配通道所需内存*/
#ifdef HI_TEE_SUPPORT
    pstChan->stOption.s32IsSecMode = (HI_TRUE == pstChan->bTvp) ? 1 : 0;
    if (1 == pstChan->stOption.s32IsSecMode)
    {
       s32Ret = VDEC_Chan_AllocMemForTEE(pstChan, hHandle);
       VDEC_ASSERT_RETURN(s32Ret == HI_SUCCESS, HI_FAILURE);
    }
    else
#endif
    {
        /* Alloc SCD buffer */
        if (pstChan->stMemSize.ScdDetailMem > 0)
        {
#if (1 == PRE_ALLOC_VDEC_SCD_MMZ)

            /* Only first handle use g_stVDHMMZ */
            if ((HI_FALSE == g_bVdecPreSCDMMZUsed) && (pstChan->stMemSize.ScdDetailMem <= g_stSCDMMZ.u32Size))
            {
                g_bVdecPreSCDMMZUsed = HI_TRUE;
                s32Ret = HI_DRV_VDEC_Map(&g_stSCDMMZ);

                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_VDEC("Chan %d HI_DRV_VDEC_Map SCD MMZ err!\n", hHandle);
                }

                pstChan->stSCDMMZBuf.u32Size = pstChan->stMemSize.ScdDetailMem;
                pstChan->stSCDMMZBuf.u32StartPhyAddr = g_stSCDMMZ.u32StartPhyAddr;
                pstChan->stSCDMMZBuf.pu8StartVirAddr = g_stSCDMMZ.pu8StartVirAddr;

                pstChan->stEOSBuffer.u32StartPhyAddr = pstChan->stSCDMMZBuf.u32StartPhyAddr + g_stSCDMMZ.u32Size - HI_VDEC_EOS_MEM_SIZE;
                pstChan->stEOSBuffer.pu8StartVirAddr = pstChan->stSCDMMZBuf.pu8StartVirAddr + g_stSCDMMZ.u32Size - HI_VDEC_EOS_MEM_SIZE;
                pstChan->stEOSBuffer.u32Size = HI_VDEC_EOS_MEM_SIZE;
            }
            else
#endif
            {
                HI_U32  BufSize = ((pstChan->stMemSize.ScdDetailMem + 0x3ff) & 0xfffffc00) + HI_VDEC_EOS_MEM_SIZE;
                s32Ret = HI_DRV_VDEC_AllocAndMap("VFMW_SCD", HI_NULL, BufSize, 0, &pstChan->stSCDMMZBuf);
                VDEC_ASSERT_RETURN(s32Ret == HI_SUCCESS, HI_FAILURE);

                pstChan->stEOSBuffer.u32StartPhyAddr = pstChan->stSCDMMZBuf.u32StartPhyAddr + pstChan->stSCDMMZBuf.u32Size - HI_VDEC_EOS_MEM_SIZE;
                pstChan->stEOSBuffer.pu8StartVirAddr = pstChan->stSCDMMZBuf.pu8StartVirAddr + pstChan->stSCDMMZBuf.u32Size - HI_VDEC_EOS_MEM_SIZE;
                pstChan->stEOSBuffer.u32Size = HI_VDEC_EOS_MEM_SIZE;
            }

            /*pstChan->stSCDMMZBuf.u32SizeD的大小就是从vfmw获取的大小:pstChan->stMemSize.ScdDetailMem*/
            pstChan->stOption.MemDetail.ChanMemScd.Length  = pstChan->stSCDMMZBuf.u32Size;
            pstChan->stOption.MemDetail.ChanMemScd.PhyAddr = pstChan->stSCDMMZBuf.u32StartPhyAddr;
            pstChan->stOption.MemDetail.ChanMemScd.VirAddr = (HI_U64)(HI_SIZE_T)(pstChan->stSCDMMZBuf.pu8StartVirAddr);
            HI_INFO_VDEC("<0>SCD Buffer allocate %d!\n", pstChan->stSCDMMZBuf.u32Size);
        }

        /* Context memory allocated by VFMW */
        /* CNcomment: 这部分由vfmw自己进行分配，scd和vdh的内存由vdec进行分配*/
        pstChan->stOption.MemDetail.ChanMemCtx.Length  = 0;
        pstChan->stOption.MemDetail.ChanMemCtx.PhyAddr = 0;
        pstChan->stOption.MemDetail.ChanMemCtx.VirAddr = HI_NULL;

        AllocVFMWMemGetVDHSize(pstChan, &u32VDHSize);

        if (u32VDHSize > 0)
        {
            pstChan->stMemSize.VdhDetailMem = u32VDHSize;
#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
            memset(&stMemBuffer, 0, sizeof(stMemBuffer));
            stMemBuffer.u32Size = u32VDHSize;

            if (1 != pstChan->stOption.u32DynamicFrameStoreAllocEn) //l00273086 采用动态分配的方案，预分配在这里由于不知道真正需要的帧存大小，所以不能去预分节点。
            {
                s32Ret = VDEC_Chan_FindPreMem(&stMemBuffer);
            }
            else
            {
                s32Ret = HI_FAILURE;
            }

            if (HI_SUCCESS == s32Ret)
            {
                /* CNcomment:解码采用预分配内存的方式，所以将预分配内存的大小分配给解码，而不是把其需要的大小赋值*/
                pstChan->stVDHMMZBuf.u32Size  = u32VDHSize;
                pstChan->stVDHMMZBuf.u32StartPhyAddr = stMemBuffer.u32StartPhyAddr;
                pstChan->stVDHMMZBuf.pu8StartVirAddr = stMemBuffer.pu8StartVirAddr;
            }
            else
#endif
            {
                if (1 == pstChan->stOption.u32DynamicFrameStoreAllocEn) //如果采用动态帧存的方案,则在这里不需要了整块申请了 l00273086//
                {
                    pstChan->stVDHMMZBuf.u32Size = 0;
                    pstChan->stVDHMMZBuf.u32StartPhyAddr = HI_NULL;
                    pstChan->stVDHMMZBuf.pu8StartVirAddr = HI_NULL;
                }
                else
                {
                    s32Ret = HI_DRV_VDEC_AllocAndMap("VFMW_VDH", HI_NULL, u32VDHSize, 0, &pstChan->stVDHMMZBuf);

                    if (HI_SUCCESS != s32Ret)
                    {
                        HI_ERR_VDEC("Chan %d alloc VDH MMZ err!\n", hHandle);
                        goto err2;
                    }
                }
            }

            pstChan->stOption.MemDetail.ChanMemVdh.Length  = pstChan->stVDHMMZBuf.u32Size;
            pstChan->stOption.MemDetail.ChanMemVdh.PhyAddr = pstChan->stVDHMMZBuf.u32StartPhyAddr;
            pstChan->stOption.MemDetail.ChanMemVdh.VirAddr = (HI_U64)(HI_SIZE_T)(pstChan->stVDHMMZBuf.pu8StartVirAddr);
            HI_INFO_VDEC("VDH Buffer allocate %d!\n", pstChan->stVDHMMZBuf.u32Size);
        }
    }

    return HI_SUCCESS;

err2:
#if (1 == PRE_ALLOC_VDEC_SCD_MMZ)

    if (g_bVdecPreSCDMMZUsed && (pstChan->stSCDMMZBuf.u32StartPhyAddr == g_stSCDMMZ.u32StartPhyAddr))
    {
        g_bVdecPreSCDMMZUsed = HI_FALSE;
        HI_DRV_VDEC_Unmap(&g_stSCDMMZ);
    }
    else
#endif
    {
        HI_DRV_VDEC_UnmapAndRelease(&pstChan->stSCDMMZBuf);
    }

    return HI_FAILURE;
}


HI_S32 VDEC_Chan_FreeVFMWMem(HI_HANDLE hHandle)
{
    HI_S32 s32Ret = HI_FAILURE;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("VDEC_Chan_Create err , chan %d invalid!\n", hHandle);
        return HI_FAILURE;
    }

    /* Free vfmw memory */
#ifdef HI_TEE_SUPPORT
    if (HI_TRUE == pstChan->bTvp)
    {
        if (0 == pstChan->stOption.u32DynamicFrameStoreAllocEn)
        {
            if ((pstChan->stVDHMMZBuf.u32StartPhyAddr != 0) && (pstChan->stVDHMMZBuf.u32Size != 0))
            {
                HI_DRV_VDEC_Release_TVP(&pstChan->stVDHMMZBuf);
            }
        }

        if ((pstChan->stEOSBuffer.u32StartPhyAddr!= 0) && (pstChan->stEOSBuffer.u32Size != 0))
        {
            HI_DRV_VDEC_Release_TVP(&pstChan->stEOSBuffer);
        }
    }
    else
#endif
    {
#if (1 == PRE_ALLOC_VDEC_SCD_MMZ)
        if (pstChan->stSCDMMZBuf.u32StartPhyAddr == g_stSCDMMZ.u32StartPhyAddr)
        {
            g_bVdecPreSCDMMZUsed = HI_FALSE;
            HI_DRV_VDEC_Unmap(&g_stSCDMMZ);
        }
        else
#endif
        {
            HI_DRV_VDEC_UnmapAndRelease(&pstChan->stSCDMMZBuf);
        }
    }
    pstChan->stEOSBuffer.u32StartPhyAddr = 0;
    pstChan->stEOSBuffer.pu8StartVirAddr = 0;
    pstChan->stEOSBuffer.u32Size = 0;
    /*release the memory alloc by vdec*/
    /*CN:释放动态申请的内存,410动态内存是在vdec里面申请的，所以在vdec里面释放*/
    s32Ret = VDEC_ReleaseMemForDecoder(hHandle);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_ReleaseMemForDecoder err!\n");
    }

    return HI_SUCCESS;
}
static HI_S32 VDEC_Chan_Create(HI_HANDLE hHandle)
{


    HI_S32 s32Ret;
    HI_S8 as8TmpBuf[128];
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    IMAGE_INTF_S*           pstImageIntf  = HI_NULL;
    STREAM_INTF_S*          pstStreamIntf = HI_NULL;

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("VDEC_Chan_Create err , chan %d invalid!\n", hHandle);
        return HI_FAILURE;
    }

    /*1:alloc memory for vfmw*/
    s32Ret = VDEC_Chan_AllocVFMWMem(hHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_Chan_AllocMem err , no memory!\n");
        return HI_FAILURE;
    }
    //vc1 have resolution 2048*1536,if donnot set this vdh will write out of buffer
    if (HI_UNF_VCODEC_TYPE_VC1 == pstChan->stCurCfg.enType)
    {
        pstChan->stOption.s32MaxWidth  = 2048;
        pstChan->stOption.s32MaxHeight = 1536;
    }

    ((HI_U64*)as8TmpBuf)[0] = (HI_U64)pstChan->enCapToFmw;
    ((HI_U64*)as8TmpBuf)[1] = (HI_U64)(HI_SIZE_T)(&pstChan->stOption);
    /*2:create vfmw channel*/
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(HI_INVALID_HANDLE, VDEC_CID_CREATE_CHAN_WITH_OPTION, as8TmpBuf);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VFMW CREATE_CHAN_WITH_OPTION err!\n");
        goto err1;
    }

    /* Record hHandle */
    pstChan->hChan = *(HI_U32*)as8TmpBuf;
    pstChan->enCurState = VDEC_CHAN_STATE_STOP;
    HI_INFO_VDEC("Create channel success:%d!\n", pstChan->hChan);

    /* 3:Set interface of read/release stream buffer */
    pstStreamIntf = &pstChan->stStrmIntf;
    pstStreamIntf->stream_provider_inst_id = hHandle;
    pstStreamIntf->read_stream = VDEC_Chan_RecvStrmBuf;
    pstStreamIntf->release_stream = VDEC_Chan_RlsStrmBuf;
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_SET_STREAM_INTF, pstStreamIntf);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d SET_STREAM_INTF err!\n", hHandle);
        goto err1;
    }

    /* 4:Get interface of read/release image */
    pstImageIntf = &pstChan->stImageIntf;
    pstImageIntf->image_provider_inst_id = pstChan->hChan;
    pstImageIntf->read_image = s_stVdecDrv.pVfmwFunc->pfnVfmwGetImage;
    pstImageIntf->release_image = s_stVdecDrv.pVfmwFunc->pfnVfmwReleaseImage;

    return HI_SUCCESS;

err1:
    (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)( pstChan->hChan, VDEC_CID_DESTROY_CHAN_WITH_OPTION, HI_NULL);
    pstChan->hChan = HI_INVALID_HANDLE;

    return HI_FAILURE;
}

#ifdef VFMW_VPSS_BYPASS_EN

static HI_S32 VDEC_RlsFrm_bypass(VDEC_CHANNEL_S *pstChan, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_S32 s32Ret;
    IMAGE stImage;
    IMAGE_INTF_S *pstImgInft = &pstChan->stImageIntf;
    VDEC_CHAN_STATINFO_S *pstStatInfo = &(pstChan->stStatInfo);
    HI_VDEC_PRIV_FRAMEINFO_S *pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S *)(((HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv))->u32Reserve);

    memset(&stImage, 0, sizeof(stImage));
    stImage.image_stride = pstFrame->stBufAddr[0].u32Stride_Y;

    stImage.image_height = pstFrame->u32Height;
    stImage.image_width   = pstFrame->u32Width;
    stImage.luma_phy_addr = pstFrame->stBufAddr[0].u32PhyAddr_Y;
    stImage.top_luma_phy_addr = pstFrame->stBufAddr[0].u32PhyAddr_Y;

    stImage.image_id             = pstPrivInfo->image_id;
    stImage.image_id_1           = pstPrivInfo->image_id_1;
    stImage.BTLInfo.btl_imageid  = pstPrivInfo->stBTLInfo.u32BTLImageID;
    stImage.BTLInfo.u32Is1D      = pstPrivInfo->stBTLInfo.u32Is1D;

    pstStatInfo->u32VdecRlsFrameTry++;

    s32Ret = pstImgInft->release_image(pstImgInft->image_provider_inst_id, &stImage);
    if (VDEC_OK != s32Ret)
    {
        pstStatInfo->u32VdecRlsFrameFail++;
        return HI_FAILURE;
    }
    else
    {
        pstStatInfo->u32VdecRlsFrameOK++;
        return HI_SUCCESS;
    }

    return HI_SUCCESS;
}

//归还由VDEC所持有的帧存
static HI_S32 VDEC_Rls_Frm_Occupied_by_Vdec(HI_HANDLE hHandle)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;

    struct list_head* pos, *n;
    BUFMNG_VPSS_NODE_S* pstTarget;

    HI_DRV_VIDEO_FRAME_S stPrivFrmInfo = {0};
    HI_DRV_VIDEO_FRAME_S* pstPrivFrame = HI_NULL;
    memset(&stPrivFrmInfo, 0, sizeof(HI_DRV_VIDEO_FRAME_S));

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan);
    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;



    /*释放帧存*/
    //pos从stVpssBufUnAvailableList的next开始
    BUFMNG_SpinLockIRQ(&pstVpssChan->stPort[0].stBufVpssInst.stUnAvailableListLock);

    if (list_empty(&pstVpssChan->stPort[0].stBufVpssInst.stVpssBufUnAvailableList)
      || pstVpssChan->stPort[0].stBufVpssInst.stVpssBufUnAvailableList.next == HI_NULL)
    {
        BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[0].stBufVpssInst.stUnAvailableListLock);
        return HI_FAILURE;
    }

    list_for_each_safe(pos, n, &(pstVpssChan->stPort[0].stBufVpssInst.stVpssBufUnAvailableList))
    {
        pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);

        //如果该buf由VDEC持有，则包含了帧存，seek时需要释放
        if (pstTarget->enFrameBufferState == HI_DRV_VDEC_BUF_STATE_IN_VDEC_FULL
            && pstVpssChan->enVideoBypass == VDEC_VPSS_BYPASSMODE_ENABLE)
        {
            //如果是VPSS bypass 通路找到metadata对应的物理帧存地址来进行还帧
            pstPrivFrame = (HI_DRV_VIDEO_FRAME_S*)pstTarget->VDECBuf_meta.pu8StartVirAddr;
            if (pstPrivFrame)
            {
                memcpy(&stPrivFrmInfo,pstPrivFrame,sizeof(HI_DRV_VIDEO_FRAME_S));
                memset(pstPrivFrame, 0 , sizeof(HI_DRV_VIDEO_FRAME_S));

                s32Ret |= VDEC_RlsFrm_bypass(pstChan, &stPrivFrmInfo);

                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_VDEC("VDEC_RlsFrm err!\n");

                    continue;
                }
            }
            else
            {
                HI_ERR_VDEC("convert the metadata FrmInfo error!\n");
            }

            pstTarget->bBufUsed = HI_FALSE;
            pstTarget->enFrameBufferState = HI_DRV_VDEC_BUF_STATE_IN_VDEC_EMPTY;

            /*删除stVpssBufUnAvailableList上的pos节点*/
            if (pstVpssChan->stPort[0].stBufVpssInst.pstUnAvailableListPos == pos)
            {
                pstVpssChan->stPort[0].stBufVpssInst.pstUnAvailableListPos = pos->prev;
            }

            list_del_init(pos);
            /*将pos节点添加到stVpssBufAvailableList上去*/
            list_add_tail(&(pstTarget->node), &(pstVpssChan->stPort[0].stBufVpssInst.stVpssBufAvailableList));
        }
    }
    BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[0].stBufVpssInst.stUnAvailableListLock);

    return s32Ret;
}
#endif


static HI_S32 VDEC_Chan_Destroy(HI_HANDLE hHandle)
{
    HI_S32 s32Ret;

    VDEC_CHANNEL_S* pstChan = HI_NULL;

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    /* Destroy VFMW decode channel */
    if (HI_INVALID_HANDLE != pstChan->hChan)
    {
#ifdef VFMW_VPSS_BYPASS_EN   //specialFrameExist->specialFrameNum
        VDEC_SPECIAL_FRM_S stReportFrmRec;
        SINT32 s32ImgOutputEn;


        s32ImgOutputEn = 0;
        VDEC_Rls_Frm_Occupied_by_Vdec(hHandle);
        BUFMNG_SpinLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);
        memset(&stReportFrmRec, 0, sizeof(VDEC_SPECIAL_FRM_S));

        s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)( pstChan->hChan, VDEC_CID_SET_IMG_OUTPUT_INFO, &s32ImgOutputEn);
        if (VDEC_OK != s32Ret)
        {
            HI_INFO_VDEC("Chan %d VDEC_CID_SET_IMG_OUTPUT_INFO (enable = %d) err!\n", hHandle, s32ImgOutputEn);
        }
        s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)( pstChan->hChan, VDEC_CID_REPORT_OCCUQIED_FRM, &stReportFrmRec);
        if (VDEC_OK != s32Ret)
        {
            HI_WARN_VDEC("Chan %d VDEC_CID_REPORT_OCCUQIED_FRM err!\n", hHandle);
        }
        else
        {
            HI_INFO_VDEC("Chan %d VDEC_CID_REPORT_OCCUQIED_FRM occupied frame = %d!\n", hHandle, stReportFrmRec.specialFrameNum);
            {
                HI_U32 i;
                HI_U32 index = 0;
                VDEC_SPECIAL_FRM_INFO_S stSpecialFrmInfo;


                for (i = 0; i < stReportFrmRec.specialFrameNum ; i++)
                {
                    if (HI_SUCCESS == VDEC_FindSpecialFrmToNormalIndex(pstChan, stReportFrmRec.specialFrmRec[i].PhyAddr, &index))
                    {
                        stSpecialFrmInfo.bCanRls                 = HI_FALSE;
                        stSpecialFrmInfo.frmBufRec.u32StartPhyAddr = pstChan->stFrameBufferRecord[index].stFrameBuffer.u32StartPhyAddr;
                        stSpecialFrmInfo.frmBufRec.pu8StartVirAddr = pstChan->stFrameBufferRecord[index].stFrameBuffer.pu8StartVirAddr;
                        stSpecialFrmInfo.frmBufRec.u32Size         = pstChan->stFrameBufferRecord[index].stFrameBuffer.u32Size;
                        stSpecialFrmInfo.enbufferNodeStatus      = pstChan->stFrameBufferRecord[index].enFrameBufferNodeStatus;
                        stSpecialFrmInfo.hdrBufRec.u32StartPhyAddr = pstChan->stFrameBufferRecord[index].stHdrBuffer.u32StartPhyAddr;
                        stSpecialFrmInfo.hdrBufRec.pu8StartVirAddr = pstChan->stFrameBufferRecord[index].stHdrBuffer.pu8StartVirAddr;
                        stSpecialFrmInfo.hdrBufRec.u32Size         = pstChan->stFrameBufferRecord[index].stHdrBuffer.u32Size;
                        stSpecialFrmInfo.enhdrNodeStatus           = pstChan->stFrameBufferRecord[index].enHdrBufferNodeStatus;
                        pstChan->stFrameBufferRecord[index].enFrameBufferNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
                        pstChan->stFrameBufferRecord[index].enHdrBufferNodeStatus   = DRV_VDEC_BUFFER_STATUS_BUTT;
#ifdef HI_TEE_SUPPORT
                        if (HI_TRUE == pstChan->bTvp)
                        {
                            stSpecialFrmInfo.bIsSecFrm   = HI_TRUE;
                        }
                        else
                        {
                            stSpecialFrmInfo.bIsSecFrm   = HI_FALSE;
                        }
#endif
                        HI_INFO_VDEC("add list  pstChan->specialFrmRec[%d],u32StartPhyAddr= 0x%x,u32StartVirAddr = 0x%x,u32Size = %d\n",
                                     i, stSpecialFrmInfo.frmBufRec.u32StartPhyAddr , stSpecialFrmInfo.frmBufRec.pu8StartVirAddr, stSpecialFrmInfo.frmBufRec.u32Size);
                        s32Ret = BUFMNG_VDEC_List_Add(&s_stVdecDrv.stRemainFrmList, &stSpecialFrmInfo);
                        if (s32Ret != HI_SUCCESS)
                        {
                            HI_ERR_VDEC("[%s %d], s32Ret=%d\n", __func__, __LINE__, s32Ret);
                        }
                    }
                    else
                    {
                        //HI_ERR_VDEC("special Frame 0x%x,can't find match single buffer node\n", stSpecialFrmInfo.frmBufRec.u32StartPhyAddr);
                        HI_ERR_VDEC("special Frame 0x%x,can't find match single buffer node\n", stReportFrmRec.specialFrmRec[i].PhyAddr);
                        stSpecialFrmInfo.enbufferNodeStatus       = DRV_VDEC_BUFFER_STATUS_BUTT;
                    }
                }
            }
        }

        BUFMNG_SpinUnLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);
#endif
        s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)( pstChan->hChan, VDEC_CID_DESTROY_CHAN_WITH_OPTION, HI_NULL);
        if (VDEC_OK != s32Ret)
        {
            HI_ERR_VDEC("Chan %d DESTROY_CHAN err!\n", hHandle);
        }

        pstChan->hChan = HI_INVALID_HANDLE;
        /*free vfmw channel mem*/
        s32Ret = VDEC_Chan_FreeVFMWMem(hHandle);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("VDEC_Chan_FreeVFMWMem err:%d \n", s32Ret);
        }
    }

    return HI_SUCCESS;
}



static HI_S32 VDEC_Chan_Free(HI_HANDLE hHandle)
{
    HI_S32 s32Ret = HI_FAILURE;

    VDEC_CHANNEL_S* pstChan = HI_NULL;
    /* Stop channel first */
    s32Ret = HI_DRV_VDEC_ChanStop(hHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("INFO: VDEC_Chan_Free call HI_DRV_VDEC_ChanStop %d err !\n", hHandle);
        return HI_FAILURE;
    }

    VDEC_CHAN_RLS_DOWN(&s_stVdecDrv.astChanEntity[hHandle], HI_INVALID_TIME);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("INFO: %d use too long !\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_RLS_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    s32Ret = VDEC_Chan_Destroy(hHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_Chan_Destroy err! s32Ret:%#x\n", s32Ret);
    }

    /* Remove proc interface */
    if (HI_TRUE == pstChan->bProcRegister)
    {
        VDEC_UnRegChanProc(hHandle);
        pstChan->bProcRegister = HI_FALSE;
    }

#ifdef TEST_VDEC_SAVEFILE
    VDEC_Dbg_CloseSaveFile(hHandle);
#endif

    /* Free I frame 2d buffer */
    if (0 != pstChan->stIFrame.st2dBuf.u32Size)
    {
        HI_DRV_VDEC_UnmapAndRelease(&pstChan->stIFrame.st2dBuf);
        pstChan->stIFrame.st2dBuf.u32Size = 0;
    }

    /* Free pts recover channel */
    PTSREC_Free(hHandle);

    /* Free user data */
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    USRDATA_Free(hHandle);
#endif


    /* Free resource */
    //if (pstChan)
    //{
        if (pstChan->pstUsrData)
        {
            HI_KFREE_VDEC(pstChan->pstUsrData);
        }

#if 0
        HI_VFREE_VDEC(pstChan);
#endif
    //}

    VDEC_CHAN_RLS_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    HI_INFO_VDEC("Chan %d free OK!\n", hHandle);
    return HI_SUCCESS;
}


HI_S32 HI_DRV_VDEC_ChanStart(HI_HANDLE hHandle)
{
    HI_S32 s32Ret = HI_FAILURE;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
    MEM_DESC_S  stChanMemVdh;
    MMZ_BUFFER_S stMMZBuffer;

    memset(&stChanMemVdh, 0, sizeof(MEM_DESC_S));
    memset(&stMMZBuffer, 0, sizeof(MMZ_BUFFER_S));
    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_DRV_VDEC_REPORT_TO_OMX(-1, VDEC_MSG_RESP_START_DONE, s32Ret, NULL);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        HI_ERR_VDEC("Chan %d Start err , not init!\n", hHandle);
        HI_DRV_VDEC_REPORT_TO_OMX(-1, VDEC_MSG_RESP_START_DONE, s32Ret, NULL);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    pstVpssChan = &s_stVdecDrv.astChanEntity[hHandle].stVpssChan;

    if (HI_INVALID_HANDLE == s_stVdecDrv.astChanEntity[hHandle].pstChan->hChan)
    {
        pstChan->enCurState = VDEC_CHAN_STATE_STOP;
        s32Ret = VDEC_Chan_Create(hHandle);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("Chan %d Start err ,VDEC_Chan_Create err! HI_DRV_VPSS_USER_COMMAND_STOP:%d\n", hHandle, s32Ret);
            HI_DRV_VDEC_REPORT_TO_OMX((HI_S32)(pstChan->hOmxChn), VDEC_MSG_RESP_START_DONE, s32Ret, NULL);
            return HI_FAILURE;
        }
    }

    s32Ret = VDEC_SetAttr(pstChan);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d SetAttr err!\n", hHandle);
        goto error_1;   // 释放已经申请的内存
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Lock %d err!\n", hHandle);
        goto error_1;   // 释放已经申请的内存
    }

    /* Check and get pstChan pointer */
    if ((HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan) ||
        (HI_INVALID_HANDLE == s_stVdecDrv.astChanEntity[hHandle].pstChan->hChan))
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        goto error_1;
    }

    /* Already running, retrun HI_SUCCESS */
    if (pstChan->enCurState == VDEC_CHAN_STATE_RUN)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return HI_SUCCESS;
    }

    /* Initialize status information*/
    memset(&(pstChan->stStatInfo), 0, sizeof(VDEC_CHAN_STATINFO_S));
    pstChan->bEndOfStrm = HI_FALSE;
    pstChan->u32EndFrmFlag = 0;
    pstChan->u32LastFrmId = -1;
    pstChan->u32LastFrmTryTimes = 0;
    pstChan->u8ResolutionChange = 0;

    g_ChanControlState[hHandle] = NORMAL_CHAN;

    //pstChan->u32DiscontinueCount = 0;

    pstChan->s32Speed = 1024;
    pstChan->bSetEosFlag = HI_FALSE;

    /* Start VFMW channel */
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_START_CHAN, &stChanMemVdh);

    if (VDEC_OK != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d VDEC_CID_START_CHAN err!\n", pstChan->hChan);
        goto error_1;
    }


    /* Start pts recover channel */
    PTSREC_Start(hHandle);

    /* Start user data channel */
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    USRDATA_Start(hHandle);
#endif

    /*网络播放场景，bufferType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE 时,start vpss 的时机需要设置在配置了bypass策略之后 add by l00228308 */
    if ((pstVpssChan->stPort[0].bufferType != HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE) || (pstVpssChan->enVideoBypass != VDEC_VPSS_BYPASSMODE_BUTT))
    {
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss, HI_DRV_VPSS_USER_COMMAND_START, NULL);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("HI_DRV_VPSS_USER_COMMAND_START err!\n");
            goto error_1;
        }
    }

    /* Save state */
    pstChan->enCurState = VDEC_CHAN_STATE_RUN;
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    HI_INFO_VDEC("Chan %d start OK\n", hHandle);
    HI_DRV_VDEC_REPORT_TO_OMX((HI_S32)(pstChan->hOmxChn), VDEC_MSG_RESP_START_DONE, s32Ret, &(pstChan->hChan));
    return HI_SUCCESS;

error_1:
    s32Ret = HI_DRV_VDEC_ChanStop(hHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("inter HI_DRV_VDEC_ChanStop err!\n");
    }

    HI_DRV_VDEC_REPORT_TO_OMX((HI_S32)(pstChan->hOmxChn), VDEC_MSG_RESP_START_DONE, s32Ret, NULL);
    return HI_FAILURE;
}

HI_S32 HI_DRV_VDEC_ChanStop(HI_HANDLE hHandle)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i = 0;

    VDEC_CHANNEL_S* pstChan = HI_NULL;
    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_DRV_VDEC_REPORT_TO_OMX(-1, VDEC_MSG_RESP_STOP_DONE, s32Ret, NULL);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        s32Ret = HI_FAILURE;
        HI_DRV_VDEC_REPORT_TO_OMX(-1, VDEC_MSG_RESP_STOP_DONE, s32Ret, NULL);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    g_CHAN_FRAME_RATE[hHandle] = 0;

    /* Already stop, retrun HI_SUCCESS */
    if (pstChan->enCurState == VDEC_CHAN_STATE_STOP)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return HI_SUCCESS;
    }

    if (HI_INVALID_HANDLE != s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss)
    {
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss, HI_DRV_VPSS_USER_COMMAND_STOP, NULL);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("HI_DRV_VPSS_USER_COMMAND_STOP err!\n");
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
            HI_DRV_VDEC_REPORT_TO_OMX((HI_S32)(pstChan->hOmxChn), VDEC_MSG_RESP_STOP_DONE, s32Ret, NULL);
            return HI_FAILURE;
        }
    }

    /* Stop VFMW */
    if (HI_INVALID_HANDLE != pstChan->hChan)
    {
        for (i = 0; i < 50; i++)
        {
            s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_STOP_CHAN, HI_NULL);

            if (VDEC_OK != s32Ret)
            {
                msleep(10);
                HI_WARN_VDEC("VDEC_CID_STOP_CHAN err!\n");
                continue;
            }
            else
            {
                break;
            }
        }
    }

    /* Lock */
    down(&s_stVdecDrv.stSem);
    //if (down(&s_stVdecDrv.stSem))
    //{
        //HI_ERR_VDEC("Global lock err!\n");
        //return HI_FAILURE;
    //}
    /*reset vpss before VFMW decode channel destroy(reset VDH) change by l00228308*/
    if (HI_INVALID_HANDLE != s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss)
    {
        VDEC_Chan_ResetVpss(s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss);
    }

    /* Destroy VFMW decode channel */
    s32Ret = VDEC_Chan_Destroy(hHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_Chan_Destroy err! s32Ret:%#x\n", s32Ret);
    }

    memset(&pstChan->stVFMWReportFrameInfo, 0, sizeof(VFMW_REPORT_FRAME_BUFFER_INFO_S));
    pstChan->enCurState = VDEC_CHAN_STATE_STOP;
    up(&s_stVdecDrv.stSem);

    /* Stop user data channel */
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    USRDATA_Stop(hHandle);
#endif

    /* Stop pts recover channel */
    PTSREC_Stop(hHandle);

    /* Save state */
    pstChan->bEndOfStrm = HI_FALSE;
    pstChan->u32EndFrmFlag = 0;
    pstChan->u32LastFrmId = -1;
    pstChan->u32LastFrmTryTimes = 0;
    pstChan->u32ValidPtsFlag = 0;
    pstChan->u8ResolutionChange = 0;

    //pstChan->u32DiscontinueCount = 0;

    pstChan->s32Speed = 1024;
    pstChan->bSetEosFlag = HI_FALSE;


    //wake up wait queue
    pstChan->bWakeUp = HI_TRUE;
    VDEC_GiveEvent(pstChan->WaitQueue);
    HI_DRV_LD_Stop_Statistics();
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    HI_INFO_VDEC("Chan %d stop ret:%x\n", hHandle, s32Ret);
    HI_DRV_VDEC_REPORT_TO_OMX((HI_S32)(pstChan->hOmxChn), VDEC_MSG_RESP_STOP_DONE, s32Ret, NULL);
    return s32Ret;
}

static HI_S32 VDEC_Chan_Reset(HI_HANDLE hHandle, HI_DRV_VDEC_RESET_TYPE_E enType)
{
    HI_S32 s32Ret, i;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    pstVpssChan = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan);

    /* Must stop channel before reset */
    if (pstChan->enCurState != VDEC_CHAN_STATE_STOP)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d state err:%d!\n", hHandle, pstChan->enCurState);
        return HI_FAILURE;
    }

    if (HI_INVALID_HANDLE != pstChan->hStrmBuf)
    {
        s32Ret = BUFMNG_Reset(pstChan->hStrmBuf);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("Chan %d strm buf reset err!\n", hHandle);
        }
    }

    /* Reset vfmw */
    if (HI_INVALID_HANDLE != pstChan->hChan)
    {
        s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_RESET_CHAN, HI_NULL);

        if (VDEC_OK != s32Ret)
        {
            HI_ERR_VDEC("Chan %d RESET_CHAN err!\n", pstChan->hChan);
        }
    }

    if (HI_DRV_VDEC_BUF_USER_ALLOC_MANAGE == pstVpssChan->enFrameBuffer)
    {
        for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
        {
            if ((HI_INVALID_HANDLE != pstVpssChan->stPort[i].hPort) && (HI_TRUE == pstVpssChan->stPort[i].bEnable))
            {
                (HI_VOID)BUFMNG_VPSS_Reset(&(pstVpssChan->stPort[i].stBufVpssInst));
            }
        }
    }

    /* Reset end frame flag */
    pstChan->bEndOfStrm = HI_FALSE;
    pstChan->u32EndFrmFlag = 0;
    pstChan->u32LastFrmId = -1;
    pstChan->u32LastFrmTryTimes = 0;

    pstChan->u32ValidPtsFlag = 0;
    pstChan->u8ResolutionChange = 0;

    //pstChan->u32DiscontinueCount = 0;

    pstChan->s32Speed = 1024;
    pstChan->bSetEosFlag = HI_FALSE;
    pstChan->bUnSupportStream = HI_FALSE;
    pstChan->u32FrameCnt = 0;
    pstChan->s32FindStartCodeCnt = 0;
    pstChan->s32Mpeg2Score = 0;
    pstChan->s32H264Score = 0;
    /* Reset pts recover channel */
    PTSREC_Reset(hHandle);

    /* Reset user data channel */
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    USRDATA_Reset(hHandle);
#endif

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    HI_INFO_VDEC("Chan %d reset OK\n", hHandle);
    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_CheckCfg(VDEC_CHANNEL_S* pstChan, VDEC_VCODEC_ATTR_S* pstCfgParam)
{
    HI_S32 s32Level = 0;
    HI_UNF_VCODEC_ATTR_S* pstCfg = &pstChan->stCurCfg;

    if (pstCfgParam->enType >= HI_UNF_VCODEC_TYPE_BUTT)
    {
        HI_ERR_VDEC("Bad type:%d!\n", pstCfgParam->enType);
        return HI_FAILURE;
    }

    if (pstCfgParam->enMode >= HI_UNF_VCODEC_MODE_BUTT)
    {
        HI_ERR_VDEC("Bad mode:%d!\n", pstCfgParam->enMode);
        return HI_FAILURE;
    }

    if (pstCfgParam->u32ErrCover > 100)
    {
        HI_ERR_VDEC("Bad err_cover:%d!\n", pstCfgParam->u32ErrCover);
        return HI_FAILURE;
    }

    if (pstCfgParam->u32Priority > HI_UNF_VCODEC_MAX_PRIORITY)
    {
        HI_ERR_VDEC("Bad priority:%d!\n", pstCfgParam->u32Priority);
        return HI_FAILURE;
    }

    /* enVdecType can't be set dynamically */
    if (pstCfg->enType != pstCfgParam->enType)
    {
        s32Level |= 1;
    }
    else if ((HI_UNF_VCODEC_TYPE_VC1 == pstCfg->enType)
             && ((pstCfg->unExtAttr.stVC1Attr.bAdvancedProfile != pstCfgParam->unExtAttr.stVC1Attr.bAdvancedProfile)
                 || (pstCfg->unExtAttr.stVC1Attr.u32CodecVersion != pstCfgParam->unExtAttr.stVC1Attr.u32CodecVersion)))
    {
        s32Level |= 1;
    }

    /* priority can't be set dynamically */
    if (pstCfg->u32Priority != pstCfgParam->u32Priority)
    {
        s32Level |= 1;
    }

    return s32Level;
}

HI_S32 DRV_VDEC_SetChanAttr(HI_HANDLE hHandle, VDEC_VCODEC_ATTR_S* pstCfgParam)
{
    HI_S32 s32Ret;
    HI_S32 s32Level;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    OMXVDEC_DRV_CONTEXT* pstOmxCtx = HI_NULL;

    /* check input parameters */
    if (HI_NULL == pstCfgParam)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if ((HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan))
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if((pstChan->OmxTunnelMode) && (pstChan->hOmxChn == HI_INVALID_HANDLE))
    {
        pstOmxCtx = (OMXVDEC_DRV_CONTEXT*)((HI_SIZE_T)(pstCfgParam->CodecContext));
        pstChan->hOmxChn = pstOmxCtx->chan_handle;
    }
    /* Check parameter */
    s32Level = VDEC_Chan_CheckCfg(pstChan, pstCfgParam);

    if (s32Level < 0)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return HI_FAILURE;
    }

    /* Some parameter can't be set when channel is running */
    if ((pstChan->enCurState != VDEC_CHAN_STATE_STOP) && (s32Level))
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d state err:%d!\n", hHandle, pstChan->enCurState);
        return HI_FAILURE;
    }

    //    pstChan->stCurCfg = *pstCfgParam;
    //@f00241306 fot 64 bit
    pstChan->stCurCfg.bOrderOutput   = pstCfgParam->bOrderOutput;
    pstChan->stCurCfg.enMode         = pstCfgParam->enMode;
    pstChan->stCurCfg.enType         = pstCfgParam->enType;
    pstChan->stCurCfg.s32CtrlOptions = pstCfgParam->s32CtrlOptions;
    pstChan->stCurCfg.u32ErrCover    = pstCfgParam->u32ErrCover;
    pstChan->stCurCfg.u32Priority    = pstCfgParam->u32Priority;
    pstChan->stCurCfg.unExtAttr      = pstCfgParam->unExtAttr;
    pstChan->stCurCfg.pCodecContext  = (HI_VOID*)(HI_SIZE_T)pstCfgParam->CodecContext;

    if ((HI_UNF_VCODEC_TYPE_SORENSON == pstChan->stCurCfg.enType)
        || (HI_UNF_VCODEC_TYPE_H263 == pstChan->stCurCfg.enType)
        || (HI_UNF_VCODEC_TYPE_MJPEG == pstChan->stCurCfg.enType))
    {
        pstChan->stOption.u32DynamicFrameStoreAllocEn = 0;
    }

    if (pstChan->enCurState == VDEC_CHAN_STATE_RUN)
    {
        s32Ret = VDEC_SetAttr(pstChan);

        if (HI_SUCCESS != s32Ret)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
            HI_ERR_VDEC("Chan %d SetAttr err!\n", hHandle);
            return HI_FAILURE;
        }
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    HI_INFO_VDEC("Chan %d SetAttr OK\n", hHandle);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_SetChanAttr(HI_HANDLE hHandle, HI_UNF_VCODEC_ATTR_S* pstCfgParam)
{
    HI_S32 s32Ret;
    VDEC_VCODEC_ATTR_S stVdecVcodecAttr;
    /* check input parameters */
    if (HI_NULL == pstCfgParam)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }
    stVdecVcodecAttr.bOrderOutput      = pstCfgParam->bOrderOutput;
    stVdecVcodecAttr.CodecContext      = (HI_SIZE_T)pstCfgParam->pCodecContext;
    stVdecVcodecAttr.enMode            = pstCfgParam->enMode;
    stVdecVcodecAttr.enType            = pstCfgParam->enType;
    stVdecVcodecAttr.s32CtrlOptions    = pstCfgParam->s32CtrlOptions;
    stVdecVcodecAttr.u32ErrCover       = pstCfgParam->u32ErrCover;
    stVdecVcodecAttr.u32Priority       = pstCfgParam->u32Priority;
    stVdecVcodecAttr.unExtAttr         = pstCfgParam->unExtAttr;
    s32Ret = DRV_VDEC_SetChanAttr(hHandle, &stVdecVcodecAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("chan %d DRV_VDEC_SetChanAttr fail!\n", hHandle);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

#ifdef HI_HDR_SUPPORT
HI_S32 HI_DRV_VDEC_SetChanHDRAttr(HI_HANDLE hHandle, VDEC_HDR_ATTR_S* vdec_hdr_attr)
{
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    /* check input parameters */
    if (HI_NULL == vdec_hdr_attr)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = hHandle & 0xff;
    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Check and get pstChan pointer */
    if ((HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan))
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    /* Set config */
    pstChan->stVdecHdrAttr = *vdec_hdr_attr;
    HI_INFO_VDEC("Chan %d SetHDRAttr OK\n", hHandle);
    return HI_SUCCESS;
}
#endif

HI_S32 DRV_VDEC_GetChanAttr(HI_HANDLE hHandle, VDEC_VCODEC_ATTR_S* pstCfgParam)
{
    HI_S32 s32Ret;

    /* check input parameters */
    if (HI_NULL == pstCfgParam)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    //   *pstCfgParam = s_stVdecDrv.astChanEntity[hHandle].pstChan->stCurCfg;  //@f00241306 for 64bit
    pstCfgParam->bOrderOutput = s_stVdecDrv.astChanEntity[hHandle].pstChan->stCurCfg.bOrderOutput;
    pstCfgParam->enMode       = s_stVdecDrv.astChanEntity[hHandle].pstChan->stCurCfg.enMode;
    pstCfgParam->enType       = s_stVdecDrv.astChanEntity[hHandle].pstChan->stCurCfg.enType;
    pstCfgParam->s32CtrlOptions = s_stVdecDrv.astChanEntity[hHandle].pstChan->stCurCfg.s32CtrlOptions;
    pstCfgParam->u32ErrCover    = s_stVdecDrv.astChanEntity[hHandle].pstChan->stCurCfg.u32ErrCover;
    pstCfgParam->u32Priority    = s_stVdecDrv.astChanEntity[hHandle].pstChan->stCurCfg.u32Priority;
    pstCfgParam->unExtAttr      = s_stVdecDrv.astChanEntity[hHandle].pstChan->stCurCfg.unExtAttr;
    pstCfgParam->CodecContext   = (HI_U64)(HI_SIZE_T)(s_stVdecDrv.astChanEntity[hHandle].pstChan->stCurCfg.pCodecContext);
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    HI_INFO_VDEC("Chan %d GetAttr OK\n", hHandle);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_GetChanAttr(HI_HANDLE hHandle, HI_UNF_VCODEC_ATTR_S* pstCfgParam)
{
    /* check input parameters */
    if (HI_NULL == pstCfgParam)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    *pstCfgParam = s_stVdecDrv.astChanEntity[hHandle].pstChan->stCurCfg;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_GetChanStatusInfo(HI_HANDLE hHandle, VDEC_STATUSINFO_S* pstStatus)
{
    HI_S32 i;
    HI_U32 freeSize;
    HI_U32 busySize;
    HI_S32 s32Ret;
    HI_HANDLE hMasterPortHandle = HI_INVALID_HANDLE;
    HI_HANDLE hVirPortHandle = HI_INVALID_HANDLE;
    HI_HANDLE hPortHandle = HI_INVALID_HANDLE;
    HI_BOOL bAllPortCompleteFrm = HI_FALSE;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;
    VDEC_CHAN_STATE_S stChanState = {0};
    BUFMNG_STATUS_S stStatus = {0};
    HI_DRV_VPSS_PORT_BUFLIST_STATE_S stVpssBufListState = {0};

    /* check input parameters */
    VDEC_ASSERT_RETURN(pstStatus != HI_NULL, HI_ERR_VDEC_INVALID_PARA);

    hHandle = hHandle & 0xff;
    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    memset(&stChanState, 0, sizeof(VDEC_CHAN_STATE_S));
    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);
    VDEC_ASSERT_RETURN(s32Ret == HI_SUCCESS, HI_FAILURE);

    /* Check and get pstChan pointer */
    if (s_stVdecDrv.astChanEntity[hHandle].pstChan == HI_NULL)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    pstVpssChan = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan);

    if (HI_INVALID_HANDLE != pstChan->hStrmBuf)
    {
        s32Ret = BUFMNG_GetStatus(pstChan->hStrmBuf, &stStatus);

        if (HI_SUCCESS != s32Ret)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
            HI_ERR_VDEC("Chan %d get strm buf status err!\n", hHandle);
            return HI_FAILURE;
        }

        freeSize = stStatus.u32Free;
        busySize = stStatus.u32Used;
        pstStatus->u32BufferSize = pstChan->u32StrmBufSize;
        pstStatus->u32BufferUsed = busySize;
        pstStatus->u32BufferAvailable = freeSize;
    }

    pstStatus->u32StrmInBps = pstChan->stStatInfo.u32AvrgVdecInBps;
    pstStatus->u32TotalDecFrmNum = pstChan->stStatInfo.u32TotalVdecOutFrame;
    pstStatus->u32TotalErrFrmNum = pstChan->stStatInfo.u32VdecErrFrame;
    pstStatus->u32TotalErrStrmNum = pstChan->stStatInfo.u32TotalStreamErrNum;

    /* judge if reach end of stream */
    if (HI_INVALID_HANDLE != pstChan->hChan)
    {
        s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_GET_CHAN_STATE, &stChanState);
        if (VDEC_OK != s32Ret)
        {
            HI_FATAL_VDEC("Chan %d GET_CHAN_STATE err\n", pstChan->hChan);
        }
    }

    pstStatus->u32FrameBufNum = stChanState.wait_disp_frame_num;

    /* Get frame num and stream size vfmw holded */
    pstStatus->u32VfmwFrmNum  = stChanState.decoded_1d_frame_num;
    pstStatus->u32VfmwStrmSize = stChanState.buffered_stream_size;

    pstStatus->stVfmwFrameRate.u32fpsInteger = stChanState.frame_rate / 10;
    pstStatus->stVfmwFrameRate.u32fpsDecimal = stChanState.frame_rate % 10 * 100;
    pstStatus->u32VfmwStrmNum = stChanState.buffered_stream_num;
    //pstStatus->u32VfmwTotalDispFrmNum = stChanState.total_disp_frame_num;
    pstStatus->u32FieldFlag = stChanState.is_field_flg;

    pstStatus->bEndOfStream = (pstChan->bEndOfStrm == HI_TRUE) ? HI_TRUE : HI_FALSE;

    /*get vpss status*/
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(pstVpssChan->hVpss, HI_DRV_VPSS_USER_COMMAND_CHECKALLDONE, &bAllPortCompleteFrm);

    if (HI_SUCCESS != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Vpss:%d HI_DRV_VPSS_USER_COMMAND_CHECKALLDONE error!\n", pstVpssChan->hVpss);
        return HI_FAILURE;
    }

    pstStatus->bAllPortCompleteFrm = bAllPortCompleteFrm;

    for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
    {
        if (VDEC_PORT_TYPE_MASTER == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].enPortType)
        {
            hMasterPortHandle = s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].hPort;
            break;
        }
        else if (VDEC_PORT_TYPE_VIRTUAL == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].enPortType)
        {
            hVirPortHandle = s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].hPort;
        }
    }

    if (HI_INVALID_HANDLE != hMasterPortHandle)
    {
        hPortHandle = hMasterPortHandle;
    }
    else if (HI_INVALID_HANDLE != hVirPortHandle)
    {
        hPortHandle = hVirPortHandle;
    }
    else
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("VDEC_GetChanStatusInfo get INVALID Porthandle\n");
        return HI_FAILURE;
    }

    if (HI_INVALID_HANDLE != hPortHandle)
    {
        if (i == VDEC_MAX_PORT_NUM)
        {
            i = VDEC_MAX_PORT_NUM - 1;
        }

        if (HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].bufferType)
        {
            s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortBufListState)(hPortHandle, &stVpssBufListState);

            if (HI_SUCCESS == s32Ret)
            {
                pstStatus->u32VfmwTotalDispFrmNum = stVpssBufListState.u32TotalBufNumber;
            }
            else
            {
                VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
                return HI_FAILURE;
            }
        }
        else if (HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].bufferType)
        {
            pstStatus->u32VfmwTotalDispFrmNum = s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].stBufVpssInst.u32AvaiableFrameCnt;
        }
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_GetChanStreamInfo(HI_HANDLE hHandle, HI_UNF_VCODEC_STREAMINFO_S* pstStreamInfo)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    VDEC_CHAN_STATE_S stChanState;

    /* check input parameters */
    if (HI_NULL == pstStreamInfo)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    memset(&stChanState, 0, sizeof(VDEC_CHAN_STATE_S));
    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_GET_CHAN_STATE, &stChanState);

    if (VDEC_OK != s32Ret)
    {
        HI_ERR_VDEC("Chan %d GET_CHAN_STATE err\n", pstChan->hChan);
    }

    pstStreamInfo->enVCodecType  = pstChan->stCurCfg.enType;
    pstStreamInfo->enSubStandard = HI_UNF_VIDEO_SUB_STANDARD_UNKNOWN;
    pstStreamInfo->u32SubVersion = 0;
    pstStreamInfo->u32Profile = stChanState.profile;
    pstStreamInfo->u32Level = stChanState.level;
    pstStreamInfo->enDisplayNorm = pstChan->enDisplayNorm;
    pstStreamInfo->bProgressive = (pstChan->stLastFrm.bProgressive);
    pstStreamInfo->u32AspectWidth = pstChan->stLastFrm.u32AspectWidth;
    pstStreamInfo->u32AspectHeight = pstChan->stLastFrm.u32AspectHeight;
    pstStreamInfo->u32bps = stChanState.bit_rate;
    //pstStreamInfo->u32fpsInteger = pstChan->stLastFrm.stFrameRate.u32fpsInteger;
    //pstStreamInfo->u32fpsDecimal = pstChan->stLastFrm.stFrameRate.u32fpsDecimal;
    pstStreamInfo->u32fpsInteger = pstChan->stLastFrm.u32FrameRate / 1000;
    pstStreamInfo->u32fpsDecimal = pstChan->stLastFrm.u32FrameRate % 1000;
    pstStreamInfo->u32Width  = pstChan->stLastFrm.u32Width;
    pstStreamInfo->u32Height = pstChan->stLastFrm.u32Height;
    pstStreamInfo->u32DisplayWidth   = pstChan->stLastFrm.stDispRect.s32Width;
    pstStreamInfo->u32DisplayHeight  = pstChan->stLastFrm.stDispRect.s32Height;
    pstStreamInfo->u32DisplayCenterX = pstChan->stLastFrm.stDispRect.s32Width / 2;
    pstStreamInfo->u32DisplayCenterY = pstChan->stLastFrm.stDispRect.s32Height / 2;

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_CheckNewEvent(HI_HANDLE hHandle, VDEC_EVENT_S* pstEvent)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    /* check input parameters */
    VDEC_ASSERT_RETURN(pstEvent != HI_NULL, HI_ERR_VDEC_INVALID_PARA);

    hHandle = hHandle & 0xff;
    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);
    VDEC_ASSERT_RETURN(s32Ret == HI_SUCCESS, HI_FAILURE);

    /* Check and get pstChan pointer */
    if (s_stVdecDrv.astChanEntity[hHandle].pstChan == HI_NULL)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    /* Check norm change event */
    if (pstChan->bNormChange)
    {
        pstChan->bNormChange = HI_FALSE;
        pstEvent->bNormChange = HI_TRUE;
        pstEvent->stNormChangeParam = pstChan->stNormChangeParam;
    }
    else
    {
        pstEvent->bNormChange = HI_FALSE;
    }

    /* Check error frame event */
    pstEvent->bErrorFrame = pstChan->bErrorFrame;
    pstChan->bErrorFrame = HI_FALSE;

    /* Check frame packing event */
    if (pstChan->bFramePackingChange)
    {
        pstChan->bFramePackingChange = HI_FALSE;
        pstEvent->bFramePackingChange = HI_TRUE;
        pstEvent->enFramePackingType = pstChan->enFramePackingType;
    }
    else
    {
        pstEvent->bFramePackingChange = HI_FALSE;
        pstEvent->enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_NONE;
    }

    /* Check new frame event */
    if (pstChan->bNewFrame)
    {
        pstChan->bNewFrame = HI_FALSE;
        pstEvent->bNewFrame = HI_TRUE;
    }
    else
    {
        pstEvent->bNewFrame = HI_FALSE;
    }

    /* Check new seq event */
    if (pstChan->bNewSeq)
    {
        pstChan->bNewSeq = HI_FALSE;
        pstEvent->bNewSeq = HI_TRUE;
    }
    else
    {
        pstEvent->bNewSeq = HI_FALSE;
    }

    /* Check new user data event */
    if (pstChan->bNewUserData)
    {
        pstChan->bNewUserData = HI_FALSE;
        pstEvent->bNewUserData = HI_TRUE;
    }
    else
    {
        pstEvent->bNewUserData = HI_FALSE;
    }

    /* Check I frame err event */
    if (pstChan->bIFrameErr)
    {
        pstChan->bIFrameErr = HI_FALSE;
        pstEvent->bIFrameErr = HI_TRUE;
    }
    else
    {
        pstEvent->bIFrameErr = HI_FALSE;
    }

    if (pstChan->bFirstValidPts)
    {
        pstChan->bFirstValidPts = HI_FALSE;
        pstEvent->bFirstValidPts = HI_TRUE;
        pstEvent->u32FirstValidPts = pstChan->u32FirstValidPts;
    }

    if (pstChan->bSecondValidPts)
    {
        pstChan->bSecondValidPts = HI_FALSE;
        pstEvent->bSecondValidPts = HI_TRUE;
        pstEvent->u32SecondValidPts = pstChan->u32SecondValidPts;
    }

    if (pstChan->bUnSupportStream)
    {
        pstChan->bUnSupportStream = HI_FALSE;
        pstEvent->bUnSupportStream = HI_TRUE;
    }
    else
    {
        pstEvent->bUnSupportStream = HI_FALSE;
    }

    if (0 != pstChan->u32ErrRatio)
    {
        pstEvent->u32ErrRatio = pstChan->u32ErrRatio;
        pstChan->u32ErrRatio = 0;
    }
    else
    {
        pstEvent->u32ErrRatio = 0;
    }

    if (pstChan->stProbeStreamInfo.bProbeCodecTypeChangeFlag)
    {
        pstChan->stProbeStreamInfo.bProbeCodecTypeChangeFlag = HI_FALSE;
        pstEvent->stProbeStreamInfo.bProbeCodecTypeChangeFlag = HI_TRUE;
        pstEvent->stProbeStreamInfo.enCodecType = pstChan->stProbeStreamInfo.enCodecType;
    }
    else
    {
        pstEvent->stProbeStreamInfo.bProbeCodecTypeChangeFlag = HI_FALSE;
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}

HI_S32 DRV_VDEC_GetUsrData(HI_HANDLE hHandle, VDEC_VIDEO_USERDATA_S* pstUsrData)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    /* check input parameters */
    if (HI_NULL == pstUsrData)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    /* If pstUsrData is null, it must be none user data */
    if (HI_NULL == pstChan->pstUsrData)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d none user data!\n", hHandle);
        return HI_FAILURE;
    }

    /* Copy usrdata data */
    s32Ret = copy_to_user((HI_U8*)(HI_SIZE_T)pstUsrData->pu8Buffer,
                          pstChan->pstUsrData->au8Buf[pstChan->pstUsrData->u32ReadID],
                          pstChan->pstUsrData->stAttr[pstChan->pstUsrData->u32ReadID].u32Length);

    if (s32Ret != 0)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d copy_to_user err!\n", hHandle);
        return HI_FAILURE;
    }

    /* copy usrdata attribute */
    memcpy(pstUsrData,
           &pstChan->pstUsrData->stAttr[pstChan->pstUsrData->u32ReadID],
           sizeof(HI_UNF_VIDEO_USERDATA_S));
    pstChan->pstUsrData->u32ReadID = (pstChan->pstUsrData->u32ReadID + 1) % VDEC_UDC_MAX_NUM;
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_GetUsrData(HI_HANDLE hHandle, HI_UNF_VIDEO_USERDATA_S* pstUsrData)
{
    HI_S32 s32Ret;
    VDEC_VIDEO_USERDATA_S stVdecUsrData = {0};

    /* check input parameters */
    if (HI_NULL == pstUsrData)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    memset(&stVdecUsrData, 0, sizeof(VDEC_VIDEO_USERDATA_S));

    s32Ret = DRV_VDEC_GetUsrData(hHandle, &stVdecUsrData);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("vdec call HI_DRV_VDEC_GetUsrData err\n");
        return HI_FAILURE;
    }
    pstUsrData->bBufferOverflow      = stVdecUsrData.bBufferOverflow;
    pstUsrData->bTopFieldFirst       = stVdecUsrData.bTopFieldFirst;
    pstUsrData->enBroadcastProfile   = stVdecUsrData.enBroadcastProfile;
    pstUsrData->enPositionInStream   = stVdecUsrData.enPositionInStream;
    pstUsrData->pu8Buffer            = (HI_U8*)(HI_SIZE_T)stVdecUsrData.pu8Buffer;
    pstUsrData->u32Length            = stVdecUsrData.u32Length;
    pstUsrData->u32Pts               = stVdecUsrData.u32Pts;
    pstUsrData->u32SeqCnt            = stVdecUsrData.u32SeqCnt;
    pstUsrData->u32SeqFrameCnt       = stVdecUsrData.u32SeqFrameCnt;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_SetProgressive(HI_HANDLE hHandle, HI_BOOL pProgressive)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss;
    HI_DRV_VPSS_CFG_S stVpssCfg;
    /* check input parameters
    if (HI_NULL == pProgressive)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }*/
    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hVpss = s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss;

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetVpssCfg)(hVpss, &stVpssCfg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("vdec call Vpss %d VpssGetVpssCfg err\n", hVpss);
        return HI_FAILURE;
    }

    if (pProgressive)
    {
        stVpssCfg.bProgRevise = HI_FALSE;
    }
    else
    {
        stVpssCfg.enProgInfo = HI_DRV_VPSS_PRODETECT_AUTO;
        stVpssCfg.bProgRevise = HI_TRUE;
    }

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSetVpssCfg)(hVpss, &stVpssCfg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("vdec call Vpss %d VpssSetVpssCfg err\n", hVpss);
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}

HI_S32 HI_DRV_VDEC_SetSceneMode(HI_HANDLE hHandle, HI_UNF_AVPLAY_SCENE_MODE_E eSceneMode)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss;
    HI_DRV_VPSS_CFG_S stVpssCfg;
    /* check input parameters
    if (HI_NULL == pProgressive)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }*/
    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hVpss = s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss;

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetVpssCfg)(hVpss, &stVpssCfg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("vdec call Vpss %d VpssGetVpssCfg err\n", hVpss);
        return HI_FAILURE;
    }

    stVpssCfg.enSceneMode = (HI_DRV_VPSS_SCENEMODE_E)eSceneMode;

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSetVpssCfg)(hVpss, &stVpssCfg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("vdec call Vpss %d VpssSetVpssCfg err\n", hVpss);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_SetDPBFullCtrl(HI_HANDLE hHandle, HI_BOOL bDPBFullCtrl)
{
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    if (HI_INVALID_HANDLE == hHandle)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    hHandle &= 0xFF;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("HI_DRV_VDEC_SetLowdelay err hvdec:%d too large!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        HI_ERR_VDEC("Chan not create!\n");
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    pstChan->bDPBFullCtrl = bDPBFullCtrl;
    return HI_SUCCESS;
}

static HI_U32 VDEC_ConverColorSpace(HI_U32 u32ColorSpace)
{
    switch (u32ColorSpace)
    {
        case HI_UNF_COLOR_SPACE_UNKNOWN:
            return HI_DRV_CS_UNKNOWN;

        case HI_UNF_COLOR_SPACE_BT601_YUV_LIMITED:
            return HI_DRV_CS_BT601_YUV_LIMITED;

        case HI_UNF_COLOR_SPACE_BT601_YUV_FULL:
            return HI_DRV_CS_BT601_YUV_FULL;

        case HI_UNF_COLOR_SPACE_BT601_RGB_LIMITED:
            return HI_DRV_CS_BT601_RGB_LIMITED;

        case HI_UNF_COLOR_SPACE_BT601_RGB_FULL:
            return HI_DRV_CS_BT601_RGB_FULL;

        case HI_UNF_COLOR_SPACE_NTSC1953:
            return HI_DRV_CS_NTSC1953;

        case HI_UNF_COLOR_SPACE_BT470_SYSTEM_M:
            return HI_DRV_CS_BT470_SYSTEM_M;

        case HI_UNF_COLOR_SPACE_BT470_SYSTEM_BG:
            return HI_DRV_CS_BT470_SYSTEM_BG;

        case HI_UNF_COLOR_SPACE_BT709_YUV_LIMITED:
            return HI_DRV_CS_BT709_YUV_LIMITED;

        case HI_UNF_COLOR_SPACE_BT709_YUV_FULL:
            return HI_DRV_CS_BT709_YUV_FULL;

        case HI_UNF_COLOR_SPACE_BT709_RGB_LIMITED:
            return HI_DRV_CS_BT709_RGB_LIMITED;

        case HI_UNF_COLOR_SPACE_BT709_RGB_FULL:
            return HI_DRV_CS_BT709_RGB_FULL;

        case HI_UNF_COLOR_SPACE_REC709:
            return HI_DRV_CS_REC709;

        case HI_UNF_COLOR_SPACE_SMPT170M:
            return HI_DRV_CS_SMPT170M;

        case HI_UNF_COLOR_SPACE_SMPT240M:
            return HI_DRV_CS_SMPT240M;

        case HI_UNF_COLOR_SPACE_BT878:
            return HI_DRV_CS_BT878;

        case HI_UNF_COLOR_SPACE_XVYCC:
            return HI_DRV_CS_XVYCC;

        case HI_UNF_COLOR_SPACE_JPEG:
            return HI_DRV_CS_JPEG;

        default:
            HI_ERR_VDEC("HI_DRV_VDEC_SetColorSpace Unknow type %d,use default!\n");
            return HI_DRV_CS_DEFAULT;
    }

    return HI_SUCCESS;
}
HI_S32 HI_DRV_VDEC_SetColorSpace(HI_HANDLE hHandle, HI_U32 u32UsrColorSpace)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss;
    HI_DRV_VPSS_CFG_S stVpssCfg;

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("HI_DRV_VDEC_SetColorSpace Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hVpss = s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss;

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetVpssCfg)(hVpss, &stVpssCfg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("vdec SetColorSpace call Vpss %d VpssGetVpssCfg err\n", hVpss);
        return HI_FAILURE;
    }

    stVpssCfg.enSrcCS = VDEC_ConverColorSpace(u32UsrColorSpace);
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSetVpssCfg)(hVpss, &stVpssCfg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("vdec SetColorSpace call Vpss %d VpssSetVpssCfg err\n", hVpss);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_GetUserBuffer(HI_HANDLE hHandle, VDEC_CMD_USER_BUFFER_S* stUserBuffer)
{
    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("HI_DRV_VDEC_SetColorSpace Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }
    stUserBuffer->u32PhyAddr = s_stVdecDrv.astChanEntity[hHandle].pstChan->stVDHMMZBuf.u32StartPhyAddr;
    stUserBuffer->u32Size    = s_stVdecDrv.astChanEntity[hHandle].pstChan->stVDHMMZBuf.u32Size;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_SetDEI(HI_HANDLE hHandle, HI_BOOL bDEI)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss;
    HI_DRV_VPSS_CFG_S stVpssCfg;
    /* check input parameters
    if (HI_NULL == pProgressive)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }*/
    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hVpss = s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss;

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetVpssCfg)(hVpss, &stVpssCfg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("vdec call Vpss %d VpssGetVpssCfg err\n", hVpss);
        return HI_FAILURE;
    }

    if (bDEI)
    {
        stVpssCfg.enProgInfo = HI_DRV_VPSS_PRODETECT_PROGRESSIVE;
    }
    else
    {
        stVpssCfg.enProgInfo = HI_DRV_VPSS_PRODETECT_AUTO;
    }

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSetVpssCfg)(hVpss, &stVpssCfg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("vdec call Vpss %d VpssSetVpssCfg err\n", hVpss);
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}


HI_S32 HI_DRV_VDEC_SetTrickMode(HI_HANDLE hHandle, HI_UNF_AVPLAY_TPLAY_OPT_S* pstOpt)
{
    HI_S32 i = 0;
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    HI_S32 s32Speed;
    HI_HANDLE hVpss;
    HI_DRV_VPSS_CFG_S stVpssCfg;

    HI_BOOL bProgressive = HI_FALSE;

    /* check input parameters */
    if (HI_NULL == pstOpt)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (VDEC_CHAN_STATE_RUN != pstChan->enCurState)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_WARN_VDEC("Chan %d isn't runnig!\n", hHandle);
        return HI_FAILURE;
    }

    s32Speed = (pstOpt->u32SpeedInteger * 1000 + pstOpt->u32SpeedDecimal) * 1024 / 1000;

    if (HI_UNF_AVPLAY_TPLAY_DIRECT_BACKWARD == pstOpt->enTplayDirect)
    {
        s32Speed = -s32Speed;
    }

    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_SET_TRICK_MODE, &s32Speed);

    if (VDEC_OK != s32Ret)
    {
        HI_ERR_VDEC("VFMW Chan %d set trick mode err\n", pstChan->hChan);
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return HI_FAILURE;
    }

    if (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32Speed != s32Speed)
    {
        for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
        {
            s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32RecvNewFrame = HI_FALSE;
            //s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32PortTmpListPos = 0;
            s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32GetFirstVpssFrameFlag = 0;
            s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32PortLastFrameGopNum = -1;
            s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].u32LastFrameIndex = -1;
        }
    }

    if (1024 == s32Speed)
    {
        hVpss = s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss;
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetVpssCfg)(hVpss, &stVpssCfg);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("vdec call Vpss %d VpssGetVpssCfg err\n", hVpss);
            return HI_FAILURE;
        }

        stVpssCfg.enProgInfo = HI_DRV_VPSS_PRODETECT_AUTO;
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSetVpssCfg)(hVpss, &stVpssCfg);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("vdec call Vpss %d VpssSetVpssCfg err\n", hVpss);
            return HI_FAILURE;
        }

    }

    pstChan->s32Speed = s32Speed;
    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32Speed = s32Speed;
#if 1

    if ((s32Speed > 1024) && (HI_TRUE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stControlInfo.u32DispOptimizeFlag))
    {
        bProgressive = HI_FALSE;
        s32Ret = HI_DRV_VDEC_SetDEI(hHandle, bProgressive);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("VDEC %d HI_DRV_VDEC_SetDEI err\n", pstChan->hChan);
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
            return HI_FAILURE;
        }
    }

#endif
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_SetCtrlInfo(HI_HANDLE hHandle, HI_UNF_AVPLAY_CONTROL_INFO_S* pstCtrlInfo)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    VFMW_CONTROLINFO_S stInfo;
    HI_BOOL bProgressive = HI_FALSE;

    /* check input parameters */
    if (HI_NULL == pstCtrlInfo)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (VDEC_CHAN_STATE_RUN != pstChan->enCurState)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_WARN_VDEC("Chan %d isn't runnig!\n", hHandle);
        return HI_FAILURE;
    }

    stInfo.u32IDRFlag = pstCtrlInfo->u32IDRFlag;
    stInfo.u32BFrmRefFlag = pstCtrlInfo->u32BFrmRefFlag;
    stInfo.u32ContinuousFlag = pstCtrlInfo->u32ContinuousFlag;
    stInfo.u32BackwardOptimizeFlag = pstCtrlInfo->u32BackwardOptimizeFlag;
    stInfo.u32DispOptimizeFlag = pstCtrlInfo->u32DispOptimizeFlag;
    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stControlInfo.u32BackwardOptimizeFlag = pstCtrlInfo->u32BackwardOptimizeFlag;
    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stControlInfo.u32DispOptimizeFlag = pstCtrlInfo->u32DispOptimizeFlag;
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_SET_CTRL_INFO, &stInfo);

    if (VDEC_OK != s32Ret)
    {
        HI_ERR_VDEC("VFMW Chan %d set ctrl info err\n", pstChan->hChan);
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return HI_FAILURE;
    }

    if (!pstCtrlInfo->u32DispOptimizeFlag)
    {
        bProgressive = HI_TRUE;
    }
    else
    {
        bProgressive = HI_FALSE;
    }

    //该改动为临时版本，和zhouzhong(z00111416)沟通后，后续PVR会在上层配置u32DispOptimizeFlag
    //那时需要删除此处代码
    if ((pstChan->s32Speed) > 1024)
    {
        bProgressive = HI_FALSE;
    }

    s32Ret = HI_DRV_VDEC_SetDEI(hHandle, bProgressive);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC %d HI_DRV_VDEC_SetDEI err\n", pstChan->hChan);
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return HI_FAILURE;
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}


static HI_S32 VDEC_IFrame_GetStrm(HI_S32 hHandle, STREAM_DATA_S* pstPacket)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    if (HI_NULL == pstPacket)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("bad handle %d!\n", hHandle);
        return HI_FAILURE;
    }

    s32Ret = VDEC_CHAN_TRY_USE_DOWN_HELP(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (pstChan->stIFrame.u32ReadTimes >= VDEC_IFRAME_MAX_READTIMES)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        return HI_FAILURE;
    }

    pstPacket->PhyAddr = pstChan->stIFrame.stMMZBuf.u32StartPhyAddr;
    pstPacket->VirAddr = (HI_U64)(HI_SIZE_T)(pstChan->stIFrame.stMMZBuf.pu8StartVirAddr);
    pstPacket->Length = pstChan->stIFrame.stMMZBuf.u32Size;
    pstPacket->Pts = 0;
    pstPacket->Index = 0;
    pstPacket->is_not_last_packet_flag = 0;
    pstPacket->UserTag = 0;
    //pstPacket->discontinue_count = 0;
    pstPacket->is_stream_end_flag = 0;

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    pstChan->stIFrame.u32ReadTimes++;
    return HI_SUCCESS;
}

static HI_S32 VDEC_IFrame_PutStrm(HI_S32 hHandle, STREAM_DATA_S* pstPacket)
{
    return HI_SUCCESS;
}

static HI_S32 VDEC_IFrame_SetAttr(VDEC_CHANNEL_S* pstChan, HI_UNF_VCODEC_TYPE_E type)
{
    HI_S32 s32Ret;
    VDEC_CHAN_CFG_S     stVdecChanCfg   = {0};
    HI_CHIP_TYPE_E      enChipType      = HI_CHIP_TYPE_HI3716C;
    HI_CHIP_VERSION_E   enChipVersion   = HI_CHIP_VERSION_V200;

    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("Invalid parameter!\n");
        return HI_FAILURE;
    }

    if (HI_INVALID_HANDLE == pstChan->hChan)
    {
        HI_ERR_VDEC("Invalid parameter!\n");
        return HI_FAILURE;
    }
    stVdecChanCfg.s32ExtraFrameStoreNum = 0;
    stVdecChanCfg.eVidStd = VDEC_CodecTypeUnfToFmw(type);
    stVdecChanCfg.s32ChanPriority = 18;
    stVdecChanCfg.s32ChanErrThr = 100;
    stVdecChanCfg.s32DecMode = I_MODE;
    stVdecChanCfg.s32DecOrderOutput = 1;
    stVdecChanCfg.s32MaxWidth = 4096;
    stVdecChanCfg.s32MaxHeight = 2304;

#if 1
    stVdecChanCfg.s32Btl1Dt2DEnable = pstChan->stOption.s32Btl1Dt2DEnable;
    stVdecChanCfg.s32BtlDbdrEnable = pstChan->stOption.s32BtlDbdrEnable;
#endif

    HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);

    if (((HI_CHIP_TYPE_HI3716M == enChipType) && (HI_CHIP_VERSION_V300 == enChipVersion)) ||
        ((HI_CHIP_TYPE_HI3712 == enChipType) && (HI_CHIP_VERSION_V100 == enChipVersion)))
    {
        /* do nothing */
    }
    else
    {
        if (HI_UNF_VCODEC_TYPE_VP8 == type)
        {
            HI_ERR_VDEC("Unsupport protocol:%d!\n", type);
            return HI_FAILURE;
        }
    }

    /* Do not use compress in i frame decode mode*/
    EnVcmp = 0;
    stVdecChanCfg.s32VcmpEn = EnVcmp;
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_CFG_CHAN, &stVdecChanCfg);

    if (VDEC_OK != s32Ret)
    {
        HI_ERR_VDEC("VFMW CFG_CHAN err!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 VDEC_IFrame_BufInit(HI_U32 u32BufSize, VDEC_CHANNEL_S* pstChan)
{
    VDEC_BUFFER_S* pstMMZBuf = &pstChan->stIFrame.stMMZBuf;
#ifdef HI_TEE_SUPPORT
    if (HI_TRUE == pstChan->bTvp)
    {
        return HI_DRV_VDEC_Alloc_TVP("VDEC_Sec_IFrm", HI_NULL, u32BufSize, 0, pstMMZBuf);
    }
    else
#endif
    {
        return HI_DRV_VDEC_AllocAndMap("VDEC_IFrame", HI_NULL, u32BufSize, 0, pstMMZBuf);
    }
}

static HI_VOID VDEC_IFrame_BufDeInit(VDEC_CHANNEL_S* pstChan)
{
    VDEC_BUFFER_S* pstMMZBuf = &pstChan->stIFrame.stMMZBuf;

#ifdef HI_TEE_SUPPORT
    if (HI_TRUE == pstChan->bTvp)
    {
        HI_DRV_VDEC_Release_TVP(pstMMZBuf);
    }
    else
#endif
    {
        HI_DRV_VDEC_UnmapAndRelease(pstMMZBuf);
    }

    memset(pstMMZBuf, 0, sizeof(MMZ_BUFFER_S));
    return;
}

HI_S32 HI_DRV_VDEC_DecodeIFrame(HI_HANDLE hHandle, HI_UNF_AVPLAY_I_FRAME_S* pstStreamInfo,HI_BOOL bCapture, HI_BOOL bUserSpace)
{
    HI_S32 s32Ret, i;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    STREAM_INTF_S stSteamIntf;
#if 0
    HI_S32 s322DbufferSize;
    HI_DRV_VDEC_BTL_S stBtl;
    HI_DRV_VIDEO_FRAME_S st2dFrame;
#endif

    /*parameter check*/
    if (HI_NULL == pstStreamInfo)
    {
        HI_ERR_VDEC("bad param\n");
        return HI_FAILURE;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("bad handle %d!\n", hHandle);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (pstChan->enCurState != VDEC_CHAN_STATE_STOP)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d state err:%d!\n", hHandle, pstChan->enCurState);
        return HI_FAILURE;
    }

    if (!VDEC_CHAN_STRMBUF_ATTACHED(pstChan))
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d bad strm buf!\n", hHandle);
        return HI_FAILURE;
    }
    g_ChanControlState[hHandle] = NORMAL_CHAN;

    if (HI_INVALID_HANDLE == pstChan->hChan)
    {
        s32Ret = VDEC_Chan_Create(hHandle);

        if (HI_SUCCESS != s32Ret)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
            HI_ERR_VDEC("IFrame VDEC_Chan_Create err\n");
            return HI_FAILURE;
        }
    }

    /* Modify the decoder's attributes */
    s32Ret = VDEC_IFrame_SetAttr(pstChan, pstStreamInfo->enType);

    if (HI_SUCCESS != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("IFrame_SetAttr err\n");
        return HI_FAILURE;
    }

    pstChan->bIsIFrameDec = HI_TRUE;

    /* Init I frame buffer */
    pstChan->stIFrame.u32ReadTimes = 0;
    /* malloc the memory to save the stream from user,unmalloc below (OUT1) until read the frame from vfmw successly */
    s32Ret = VDEC_IFrame_BufInit(pstStreamInfo->u32BufSize, pstChan);

    if (HI_SUCCESS != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Alloc IFrame buf err\n");
        goto OUT0;
    }

#ifdef HI_TEE_SUPPORT
    if (HI_TRUE == pstChan->bTvp)
    {
        /*需要实现一个安全拷贝安全安全的接口*/
        //memcpy((HI_U8 *)pstChan->stIFrame.stMMZBuf.pu8StartVirAddr, pstStreamInfo->pu8Addr, pstStreamInfo->u32BufSize);
    }
    else
#endif
    {
        if (!bUserSpace)
        {
            /*not from user space : from kernel ddr ,use memcpy is ok*/
            memcpy((HI_U8*)pstChan->stIFrame.stMMZBuf.pu8StartVirAddr,
                   pstStreamInfo->pu8Addr, pstStreamInfo->u32BufSize);
        }
        else
        {
            /*I MODE : the stream from user is one IFrame stream, copy the stream from user space , should use function : copy_from_user*/
            if (0 != copy_from_user((HI_U8*)pstChan->stIFrame.stMMZBuf.pu8StartVirAddr,
                                    pstStreamInfo->pu8Addr, pstStreamInfo->u32BufSize))
            {
                VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
                HI_ERR_VDEC("Chan %d IFrame copy %dB %p->%p err!\n",
                            hHandle, pstStreamInfo->u32BufSize, pstStreamInfo->pu8Addr,
                            (HI_U8*)pstChan->stIFrame.stMMZBuf.pu8StartVirAddr);
                goto OUT1;
            }

            HI_INFO_VDEC("Chan %d IFrame copy %dB %p->%p success!\n",
                         hHandle, pstStreamInfo->u32BufSize, pstStreamInfo->pu8Addr,
                         (HI_U8*)pstChan->stIFrame.stMMZBuf.pu8StartVirAddr);
        }
    }

    /* Set IFrame stream read functions */
    stSteamIntf.stream_provider_inst_id = hHandle;
    stSteamIntf.read_stream = VDEC_IFrame_GetStrm;
    stSteamIntf.release_stream = VDEC_IFrame_PutStrm;
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_SET_STREAM_INTF, &stSteamIntf);

    if (VDEC_OK != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d SET_STREAM_INTF err!\n", hHandle);
        goto OUT1;
    }

    /* Start decode */
    memset(&pstChan->stStatInfo, 0, sizeof(VDEC_CHAN_STATINFO_S));
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_START_CHAN, HI_NULL);

    if (VDEC_OK != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d START_CHAN err!\n", hHandle);
        goto OUT2;
    }

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss, HI_DRV_VPSS_USER_COMMAND_START, NULL);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("HI_DRV_VPSS_USER_COMMAND_START err in DecodeIFrame!\n");
        return HI_FAILURE;
    }

    pstChan->enCurState = VDEC_CHAN_STATE_RUN;

    /* Start PTS recover channel */
    PTSREC_Start(hHandle);

    /* Here we invoke USE_UP function to release the atomic number, so that the VFMW can decode
      by invoking the interface of get stream, the VDEC_Event_Handle function can also hHandle
      EVNT_NEW_IMAGE */
    /*CNcomment: 此处调用USE_UP释放锁 以使 VFMW 可以调用获取码流接口 进行解码
      VDEC_Event_Handle 可以处理 EVNT_NEW_IMAGE 事件 */
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    /* wait for vdec process complete */
    for (i = 0; i < 10; i++)
    {
        msleep(10);
    }

    /* Stop channel */
    PTSREC_Stop(hHandle);
#if 1
    (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_STOP_CHAN, HI_NULL);
    /*设置通道状态与动态帧存线程互斥*/
    down(&s_stVdecDrv.stSem);
    pstChan->enCurState = VDEC_CHAN_STATE_STOP;
    memset(&pstChan->stVFMWReportFrameInfo, 0, sizeof(VFMW_REPORT_FRAME_BUFFER_INFO_S));
    up(&s_stVdecDrv.stSem);
#endif
OUT2:
OUT1:

    /* Free MMZ buffer */
    VDEC_IFrame_BufDeInit(pstChan);
    pstChan->stIFrame.u32ReadTimes = 0;

OUT0:
    /* Resume channel attribute */
    s32Ret = VDEC_SetAttr(pstChan);
    s32Ret |= VDEC_Chan_Destroy(hHandle);

    pstChan->bIsIFrameDec = HI_FALSE;

    return s32Ret;
}

HI_S32 HI_DRV_VDEC_ReleaseIFrame(HI_HANDLE hHandle, HI_DRV_VIDEO_FRAME_S* pstFrameInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    // VDEC_CHANNEL_S *pstChan = HI_NULL;
    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    return s32Ret;
#if 0

    if (HI_NULL == pstFrameInfo)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_ERR_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (0 != pstChan->stIFrame.st2dBuf.u32Size)
    {
        HI_DRV_MMZ_UnmapAndRelease(&pstChan->stIFrame.st2dBuf);
        pstChan->stIFrame.st2dBuf.u32Size = 0;
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
#endif
}

HI_S32 HI_DRV_VDEC_SetLowdelay(HI_HANDLE hVdec, HI_BOOL bLowdelay)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    HI_HANDLE hVpss;
    HI_DRV_VPSS_CFG_S stVpssCfg;

    if (HI_INVALID_HANDLE == hVdec)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    hVdec &= 0xFF;

    if (hVdec >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("HI_DRV_VDEC_SetLowdelay err hvdec:%d too large!\n", hVdec);
        return HI_FAILURE;
    }

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hVdec].pstChan)
    {
        HI_ERR_VDEC("Chan not create!\n");
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hVdec].pstChan;
    pstChan->bLowdelay = bLowdelay;

    if (bLowdelay)
    {
        pstChan->stCurCfg.enMode = HI_UNF_VCODEC_MODE_IP;
        pstChan->stCurCfg.bOrderOutput = 1;
        pstChan->stCurCfg.s32CtrlOptions = (pstChan->stCurCfg.s32CtrlOptions & 0xfffffffe) + 1;
    }
    else
    {
        pstChan->stCurCfg.enMode = HI_UNF_VCODEC_MODE_NORMAL;
        pstChan->stCurCfg.bOrderOutput = 0;
        pstChan->stCurCfg.s32CtrlOptions = (pstChan->stCurCfg.s32CtrlOptions & 0xfffffffe);
    }

    s32Ret = VDEC_FindVpssHandleByVdecHandle(hVdec, &hVpss);

    if (HI_SUCCESS == s32Ret)
    {
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetVpssCfg)(hVpss, &stVpssCfg);

        if (HI_SUCCESS == s32Ret)
        {
            stVpssCfg.bAlwaysFlushSrc = bLowdelay;
        }
        else
        {
            HI_ERR_VDEC("HI_DRV_VDEC_SetLowdelay call pfnVpssGetVpssCfg err.\n");
            return HI_FAILURE;
        }

        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSetVpssCfg)(hVpss, &stVpssCfg);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("HI_DRV_VDEC_SetLowdelay call pfnVpssSetVpssCfg err.\n");
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_VDEC("HI_DRV_VDEC_SetLowdelay call VDEC_FindVpssHandleByVdecHandle err.\n");
        return HI_FAILURE;
    }

    s32Ret = HI_DRV_VDEC_SetDEI(hVdec, HI_TRUE);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("HI_DRV_VDEC_SetLowdelay call HI_DRV_VDEC_SetDEI err.\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#ifdef HI_TEE_SUPPORT
HI_S32 HI_DRV_VDEC_SetTvp(HI_HANDLE hVdec, HI_BOOL bTvp)
{

    VDEC_OPERATION_S stOpt;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    if (HI_INVALID_HANDLE == hVdec)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    hVdec &= 0xFF;

    if (hVdec >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("HI_DRV_VDEC_SetTvp err hvdec:%d too large!\n", hVdec);
        return HI_FAILURE;
    }

    if (HI_NULL == s_stVdecDrv.astChanEntity[hVdec].pstChan)
    {
        HI_ERR_VDEC("Chan not create!\n");
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hVdec].pstChan;

    pstChan->bTvp = bTvp;

    if (HI_TRUE == pstChan->bTvp)
    {
        memset(&stOpt, 0, sizeof(VDEC_OPERATION_S));
        stOpt.eAdapterType = ADAPTER_TYPE_VDEC;
        stOpt.VdecCallback = VDEC_EventHandle;
        stOpt.mem_malloc = HI_NULL;
        stOpt.mem_free = HI_NULL;
        stOpt.is_secure = HI_TRUE;

        if (HI_SUCCESS != (s_stVdecDrv.pVfmwFunc->pfnVfmwInit)(&stOpt))
        {
            HI_ERR_VDEC("Init trusted vfmw failed\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}
#endif

static HI_S32 VDEC_Ioctl_GetAndCheckHandle(unsigned int cmd, void* arg, HI_HANDLE *pHandle)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    *pHandle = (*((HI_HANDLE*)arg)) & 0xff;

    if ((*pHandle) >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("CMD %d bad handle:%d!\n", cmd, *pHandle);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetUsrData(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = DRV_VDEC_GetUsrData(hHandle, &(((VDEC_CMD_USERDATA_S*)arg)->stUserData));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("DRV_VDEC_GetUsrData err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_IFRMDECODE(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    VDEC_CMD_IFRAME_DEC_S tmp;
    VDEC_CMD_IFRAME_DEC_S* pstIFrameDec = HI_NULL;
    memset(&tmp, 0 , sizeof(VDEC_CMD_IFRAME_DEC_S));

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    pstIFrameDec = &tmp;
    pstIFrameDec->hHandle = ((VDEC_CMD_IFRAME_DEC_64BIT_S*)arg)->hHandle;
    pstIFrameDec->bCapture = ((VDEC_CMD_IFRAME_DEC_64BIT_S*)arg)->bCapture;
    pstIFrameDec->stIFrame.enType = ((VDEC_CMD_IFRAME_DEC_64BIT_S*)arg)->stIFrame.enType;
    pstIFrameDec->stIFrame.pu8Addr = (HI_U8*)(HI_SIZE_T)(((VDEC_CMD_IFRAME_DEC_64BIT_S*)arg)->stIFrame.Addr);
    pstIFrameDec->stIFrame.u32BufSize = ((VDEC_CMD_IFRAME_DEC_64BIT_S*)arg)->stIFrame.u32BufSize;

    s32Ret  = HI_DRV_VDEC_DecodeIFrame((pstIFrameDec->hHandle) & 0xff, &pstIFrameDec->stIFrame , pstIFrameDec->bCapture, HI_TRUE);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_DecodeIFrame err!:%d\n", s32Ret);
    }

    s32Ret |= VDEC_Chan_Reset((pstIFrameDec->hHandle) & 0xff, VDEC_RESET_TYPE_ALL);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_Reset err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_ReleaseIFrame(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_ReleaseIFrame(hHandle, &(((VDEC_CMD_IFRAME_RLS_S*)arg)->stVoFrameInfo));

    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_ReleaseIFrame err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_ChanAlloc(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    //s32Ret = VDEC_Chan_Alloc(hHandle, &(((VDEC_CMD_ALLOC_S*)arg)->stOpenOpt));
    if (((VDEC_CMD_ALLOC_S*)arg)->u32DFSEnable > 1)  //l00273086
    {
        ((VDEC_CMD_ALLOC_S*)arg)->u32DFSEnable = 0;
    }
    if (((VDEC_CMD_ALLOC_S*)arg)->u32TunnelModeEnable > 1) //l00273086
    {
        ((VDEC_CMD_ALLOC_S*)arg)->u32TunnelModeEnable = 0;
    }
    s_stVdecDrv.astChanEntity[hHandle].u32DynamicFsEn = ((VDEC_CMD_ALLOC_S*)arg)->u32DFSEnable;  //l00273086
    s_stVdecDrv.astChanEntity[hHandle].u32TunnelModeEn = ((VDEC_CMD_ALLOC_S*)arg)->u32TunnelModeEnable;
    s32Ret = VDEC_Chan_InitParam(hHandle, &(((VDEC_CMD_ALLOC_S*)arg)->stOpenOpt));

    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_InitParam err!:%d\n", s32Ret);
    }

    *((HI_HANDLE*)arg) = hHandle;

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_ChanFree(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_Chan_Free(hHandle);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_Free err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_ChanStart(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_ChanStart(hHandle);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_ChanStart err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_ChanStop(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    HI_S32 stopLoop = 500;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    while ((HI_FAILURE == VDEC_SetChanControlState(hHandle, DESTORY_CHAN)) && (stopLoop > 0))
    {
        msleep(1);
        stopLoop--;
    }

    if (stopLoop == 0)
    {
        HI_ERR_VDEC("VDEC_SetChanControlState(hHandle, DESTORY_CHAN) failed\n");
        s32Ret = HI_FAILURE;
        return s32Ret;
    }

    s32Ret = HI_DRV_VDEC_ChanStop(hHandle);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_ChanStop err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_ChanReset(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_Chan_Reset(hHandle, ((VDEC_CMD_RESET_S*)arg)->enType);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_Reset err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_ChanSetAttr(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = DRV_VDEC_SetChanAttr(hHandle, &(((VDEC_CMD_ATTR_S*)arg)->stAttr));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("DRV_VDEC_SetChanAttr err!:%d\n", s32Ret);
    }

    return s32Ret;
}

#ifdef HI_HDR_SUPPORT
static HI_S32 VDEC_Ioctl_ChanSetHDRAttr(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_SetChanHDRAttr(hHandle, &(((VDEC_CMD_HDR_ATTR_S*)arg)->vdec_hdr_attr));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_SetChanHDRAttr err!: %d\n", s32Ret);
    }

    return s32Ret;
}
#endif

static HI_S32 VDEC_Ioctl_ChanGetAttr(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = DRV_VDEC_GetChanAttr(hHandle, &(((VDEC_CMD_ATTR_S*)arg)->stAttr));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("DRV_VDEC_GetChanAttr err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_CreatEsBuffer(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hVdec;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hVdec = ((HI_DRV_VDEC_STREAM_BUF_S*)arg)->hVdec & 0xff;
    if (hVdec >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("CMD %d bad handle:%d!\n", cmd, hVdec);

        return HI_ERR_VDEC_INVALID_PARA;
    }

    if (s_stVdecDrv.astChanEntity[hVdec].pstChan == NULL)
    {
        HI_INFO_VDEC("pstChan is null!\n");

        return HI_FAILURE;
    }

    s32Ret = VDEC_CreateStrmBuf((HI_DRV_VDEC_STREAM_BUF_S*)arg);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_CreateStrmBuf err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_DestroyEsBuffer(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_DestroyStrmBuf(*((HI_HANDLE*)arg));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_DestroyStrmBuf err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetEsBuffer(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VDEC_CMD_BUF_S* pstBuf = (VDEC_CMD_BUF_S*)arg;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_GetStrmBuf(pstBuf->hHandle, &(pstBuf->stBuf), HI_TRUE);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_GetStrmBuf err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_PutEsBuffer(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VDEC_CMD_BUF_S* pstBuf = (VDEC_CMD_BUF_S*)arg;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_PutStrmBuf(pstBuf->hHandle, &(pstBuf->stBuf), HI_TRUE);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_PutStrmBuf err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetUserAddr(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VDEC_CMD_BUF_USERADDR_S* pstUserAddr = (VDEC_CMD_BUF_USERADDR_S*)arg;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_StrmBuf_SetUserAddr(pstUserAddr->hHandle, pstUserAddr->u64UserAddr);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_StrmBuf_SetUserAddr err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_RcveiveBuffer(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    BUFMNG_BUF_S stEsBuf;
    VDEC_CMD_BUF_S* pstBuf = (VDEC_CMD_BUF_S*)arg;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = BUFMNG_AcqReadBuffer(pstBuf->hHandle, &stEsBuf);
    if (s32Ret == HI_SUCCESS)
    {
        pstBuf->stBuf.u64Addr = (HI_U64)(HI_SIZE_T)stEsBuf.pu8UsrVirAddr;
        pstBuf->stBuf.u32PhyAddr = stEsBuf.u32PhyAddr;
        pstBuf->stBuf.u32BufSize = stEsBuf.u32Size;
        pstBuf->stBuf.u64Pts = stEsBuf.u64Pts;
        pstBuf->stBuf.bEndOfFrame = !(stEsBuf.u32Marker & BUFMNG_NOT_END_FRAME_BIT);
        pstBuf->stBuf.bDiscontinuous = (stEsBuf.u32Marker & BUFMNG_DISCONTINUOUS_BIT) ? 1 : 0;
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_RleaseBuffer(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    BUFMNG_BUF_S stEsBuf;
    VDEC_CMD_BUF_S* pstBuf = (VDEC_CMD_BUF_S*)arg;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    stEsBuf.u32PhyAddr = 0;
    stEsBuf.pu8UsrVirAddr = (HI_U8*)(HI_SIZE_T)pstBuf->stBuf.u64Addr;
    stEsBuf.pu8KnlVirAddr = HI_NULL;
    stEsBuf.u32Size = pstBuf->stBuf.u32BufSize;
    stEsBuf.u64Pts = pstBuf->stBuf.u64Pts;
    /* Don't care stEsBuf.u32Marker here. */
    s32Ret = BUFMNG_RlsReadBuffer(pstBuf->hHandle, &stEsBuf);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("BUFMNG_RlsReadBuffer err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_ResetEsBuffer(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = BUFMNG_Reset((*((HI_HANDLE*)arg)));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("BUFMNG_Reset err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetStatus(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    BUFMNG_STATUS_S stBMStatus;
    VDEC_CMD_BUF_STATUS_S* pstStatus = (VDEC_CMD_BUF_STATUS_S*)arg;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = BUFMNG_GetStatus(pstStatus->hHandle, &stBMStatus);
    if (s32Ret == HI_SUCCESS)
    {
        pstStatus->stStatus.u32Size = stBMStatus.u32Used + stBMStatus.u32Free;
        pstStatus->stStatus.u32Available = stBMStatus.u32Free;
        pstStatus->stStatus.u32Used = stBMStatus.u32Used;
        pstStatus->stStatus.u32DataNum = stBMStatus.u32DataNum;
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_RlsFrmBuf(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_RlsFrmBuf(hHandle, &(((VDEC_CMD_VO_FRAME_S*)arg)->stFrame));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_RlsFrmBuf err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_RecvFrmBuf(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_RecvFrmBuf(hHandle, &(((VDEC_CMD_VO_FRAME_S*)arg)->stFrame));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_RecvFrmBuf err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetChanStatusInfo(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_GetChanStatusInfo(hHandle, &(((VDEC_CMD_STATUS_S*)arg)->stStatus));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_GetChanStatusInfo err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetChanStreamInfo(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_GetChanStreamInfo(hHandle, &(((VDEC_CMD_STREAM_INFO_S*)arg)->stInfo));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_GetChanStreamInfo err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_AttachStrmBuf(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    VDEC_CMD_ATTACH_BUF_S stParam = *((VDEC_CMD_ATTACH_BUF_S*)arg);

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_Chan_AttachStrmBuf(hHandle, stParam.u32BufSize, stParam.hDmxVidChn, stParam.hStrmBuf);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_AttachStrmBuf err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_DetachStrmBuf(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_Chan_DetachStrmBuf(hHandle);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_DetachStrmBuf err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetEosFlag(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_SetEosFlag(hHandle);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_SetEosFlag err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_DiscardFrm(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_DiscardFrm(hHandle, &(((VDEC_CMD_DISCARD_FRAME_S*)arg)->stDiscardOpt));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_DiscardFrm err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_CheckNewEvent(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_CheckNewEvent(hHandle, &(((VDEC_CMD_EVENT_S*)arg)->stEvent));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_CheckNewEvent err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_EventNewFrm(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    VDEC_CMD_FRAME_S* pstFrameCmd = (VDEC_CMD_FRAME_S*)arg;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    if (s_stVdecDrv.astChanEntity[hHandle].pstChan == NULL)
    {
        HI_INFO_VDEC("pstChan is null!\n");
        return HI_FAILURE;
    }

    pstFrameCmd->stFrame = s_stVdecDrv.astChanEntity[hHandle].pstChan->stLastFrm;
    s32Ret = HI_SUCCESS;
    HI_INFO_VDEC("UMAPC_VDEC_CHAN_EVNET_NEWFRAME return:%d\n", s32Ret);

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetFrmBuf(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_GetFrmBuf(hHandle, &(((VDEC_CMD_GET_FRAME_S*)arg)->stFrame));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_GetFrmBuf err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_PutFrmBuf(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_PutFrmBuf(hHandle, &(((VDEC_CMD_PUT_FRAME_S*)arg)->stFrame));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_PutFrmBuf err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetFrmRate(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    HI_UNF_AVPLAY_FRMRATE_PARAM_S* pstParam = &(((VDEC_CMD_FRAME_RATE_S*)arg)->stFrameRate);

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    if (HI_NULL != s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        s_stVdecDrv.astChanEntity[hHandle].pstChan->stFrameRateParam = *pstParam;
        s32Ret = PTSREC_SetFrmRate(hHandle, pstParam);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_VDEC("PTSREC_SetFrmRate err:%d\n", s32Ret);
        }
    }
    else
    {
        s32Ret = HI_FAILURE;
        HI_ERR_VDEC("UMAPC_VDEC_CHAN_SETFRMRATE return :%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetFrmRate(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    HI_UNF_AVPLAY_FRMRATE_PARAM_S* pstParam = &(((VDEC_CMD_FRAME_RATE_S*)arg)->stFrameRate);

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    if (HI_NULL != s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        *pstParam = s_stVdecDrv.astChanEntity[hHandle].pstChan->stFrameRateParam;
        s32Ret = HI_SUCCESS;
        HI_INFO_VDEC("UMAPC_VDEC_CHAN_GETFRMRATE return :%d\n", s32Ret);
    }
    else
    {
        s32Ret = HI_FAILURE;
        HI_ERR_VDEC("UMAPC_VDEC_CHAN_GETFRMRATE return :%d\n", s32Ret);
    }

    return s32Ret;
}

#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
static HI_S32 VDEC_Ioctl_UserDataInitBuf(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    if (s_stVdecDrv.astChanEntity[hHandle].pstChan == NULL)
    {
        HI_INFO_VDEC("pstChan is null!\n");

        return HI_FAILURE;
    }

    s32Ret = USRDATA_Alloc(hHandle, &(((VDEC_CMD_USERDATABUF_S*)arg)->stBuf));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("USRDATA_Alloc err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_UserDataSetBufAddr(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = USRDATA_SetUserAddr(hHandle, ((VDEC_CMD_BUF_USERADDR_S*)arg)->u64UserAddr);
    if (s32Ret != HI_SUCCESS)
    {
         HI_WARN_VDEC("USRDATA_SetUserAddr err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_AcqueUserData(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = USRDATA_Acq(hHandle, &(((VDEC_CMD_USERDATA_ACQMODE_S*)arg)->stUserData),
                                 &(((VDEC_CMD_USERDATA_ACQMODE_S*)arg)->enType));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("USRDATA_Acq err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_ReleaseUserData(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = USRDATA_Rls(hHandle, &(((VDEC_CMD_USERDATA_S*)arg)->stUserData));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("USRDATA_Rls err!:%d\n", s32Ret);
    }

    return s32Ret;
}
#endif

static HI_S32 VDEC_Ioctl_SeekPts(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_SeekPTS(hHandle, &(((VDEC_CMD_SEEK_PTS_S*)(arg))->u32SeekPts),
                          ((VDEC_CMD_SEEK_PTS_S*)(arg))->u32Gap);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_SeekPTS err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetCap(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_GetCap((VDEC_CAP_S*)arg);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_GetCap err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_AllocHandle(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_Chan_AllocHandle((HI_HANDLE*)arg, filp);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_AllocHandle err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_FreeHandle(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_Chan_FreeHandle(hHandle);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_FreeHandle err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetTrickMode(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_SetTrickMode(hHandle, &(((VDEC_CMD_TRICKMODE_OPT_S*)arg)->stTPlayOpt));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_SetTrickMode err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetCtrlInfo(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_SetCtrlInfo(hHandle, &(((VDEC_CMD_SET_CTRL_INFO_S*)arg)->stCtrlInfo));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_SetCtrlInfo err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetProgressive(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_SetProgressive(hHandle, ((VDEC_CMD_SET_PROGRESSIVE_S*)arg)->bProgressive);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_SetProgressive err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetSceneMode(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_SetSceneMode(hHandle, ((VDEC_CMD_SET_SCENE_MODE_S*)arg)->eSceneMode);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_SetSceneMode err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetDPBFullCtrl(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_SetDPBFullCtrl(hHandle, ((VDEC_CMD_SET_DPBFULL_CTRL_S*)arg)->bDPBFullCtrl);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_SetDPBFullCtrl err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetLowdelay(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_SetLowdelay(hHandle, ((VDEC_CMD_SET_LOWDELAY_S*)arg)->bLowdelay);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_SetLowdelay err!:%d\n", s32Ret);
    }

    return s32Ret;
}

#ifdef HI_TEE_SUPPORT
static HI_S32 VDEC_Ioctl_SetTvp(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_SetTvp(hHandle, ((VDEC_CMD_SET_TVP_S*)arg)->bTVP);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_SetTvp err!:%d\n", s32Ret);
    }

    return s32Ret;
}
#endif

static HI_S32 VDEC_Ioctl_SetColorSpace(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_SetColorSpace(hHandle, ((VDEC_CMD_SET_COLORSPACE_S*)arg)->u32ColorSpace);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_SetColorSpace err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetUserBuffer(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_GetUserBuffer(hHandle, ((VDEC_CMD_USER_BUFFER_S*)arg));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("HI_DRV_VDEC_SetColorSpace err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_RcvVpssFrm(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    HI_HANDLE hVdec = HI_INVALID_HANDLE;
    VDEC_CMD_VPSS_FRAME_S* pstVpssFrmTmp = (VDEC_CMD_VPSS_FRAME_S*)arg;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_FindVdecHandleByVpssHandle(hHandle, &hVdec);
    if ( s32Ret == HI_SUCCESS)
    {
        if (s_stVdecDrv.astChanEntity[hVdec].stVpssChan.s32Speed < 0)
        {
            s32Ret = VDEC_Chan_RecvVdecPortListFrame(hVdec, &(pstVpssFrmTmp->stFrame));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_RecvVdecPortListFrame err:%d\n", s32Ret);
            }
        }
        else
        {
            s32Ret = VDEC_Chan_RecvVpssFrmBuf(hHandle, &(pstVpssFrmTmp->stFrame), pstVpssFrmTmp->TimeOut);

            if (HI_SUCCESS != s32Ret)
            {
                HI_INFO_VDEC("VDEC_Chan_RecvVpssFrmBuf err:%d\n", s32Ret);
            }
        }
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_ReleasePortFrm(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_VORlsFrame(hHandle, &(((VDEC_CMD_VPSS_FRAME_S*)arg)->VideoFrame));
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC release port %d frame ERR:%x!\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_CreateVpss(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;
    HI_U32 hOuthandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = (*((HI_HANDLE*)arg)) & 0xff;

    s32Ret = VDEC_Chan_CreateVpss(hHandle, &hOuthandle);
    if (s32Ret == HI_SUCCESS)
    {
        *((HI_HANDLE*)arg) = hOuthandle;  //将vpss的句柄返回，供用户态代码中使用
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_DestroyVpss(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = (*((HI_HANDLE*)arg)) & 0xff;

    s32Ret = VDEC_Chan_DestroyVpss(hHandle);
    if ( s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_DestroyVpss err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_CreatePort(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_CreatePort(hHandle, &((VDEC_CMD_VPSS_FRAME_S*)arg)->hPort,
                                 ((VDEC_CMD_VPSS_FRAME_S*)arg)->ePortAbility);
    if ( s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_CreatePort err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_DestroyPort(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_DestroyPort(hHandle, ((VDEC_CMD_VPSS_FRAME_S*)arg)->hPort);
    if ( s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_DestroyPort err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_EnablePort(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_EnablePort(hHandle, ((VDEC_CMD_VPSS_FRAME_S*)arg)->hPort);
    if ( s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_EnablePort err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_DisablePort(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_DisablePort(hHandle, ((VDEC_CMD_VPSS_FRAME_S*)arg)->hPort);
    if ( s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_DisablePort err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetPortType(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_SetPortType(hHandle, ((VDEC_CMD_VPSS_FRAME_S*)arg)->hPort,
                                   ((VDEC_CMD_VPSS_FRAME_S*)arg)->enPortType);
    if ( s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_SetPortType err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_CancleMainPort(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetFrmPackingType(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_SetFrmPackingType(hHandle, ((VDEC_CMD_VPSS_FRAME_S*)arg)->eFramePackType);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_SetFrmPackingType err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetFrmPackingType(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_GetFrmPackingType(hHandle, &((VDEC_CMD_VPSS_FRAME_S*)arg)->eFramePackType);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_GetFrmPackingType err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_ResetVpss(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_ResetVpss(hHandle);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_ResetVpss err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SendEos(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_Chan_SendEos(hHandle);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_SendEos err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetPortState(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_Chan_GetPortState(hHandle, &((VDEC_CMD_VPSS_FRAME_S*)arg)->bAllPortComplete);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_GetPortState err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetFrmStatusInfo(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_Chan_GetFrmStatusInfo(hHandle, ((VDEC_CMD_VPSS_FRAME_S*)arg)->hPort,
                 &((VDEC_CMD_VPSS_FRAME_S*)arg)->stVdecFrmStatusInfo);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_GetFrmStatusInfo err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetPortAttr(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_SetVpssAttr(hHandle, ((VDEC_CMD_VPSS_FRAME_S*)arg)->hPort,
                                  &((VDEC_CMD_VPSS_FRAME_S*)arg)->stPortCfg);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_SetVpssAttr err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetPortAttr(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_GetVpssAttr(hHandle, ((VDEC_CMD_VPSS_FRAME_S*)arg)->hPort,
                                   &((VDEC_CMD_VPSS_FRAME_S*)arg)->stPortCfg);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_GetVpssAttr err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetExtBuffer(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_SetExtBuffer(hHandle, &((VDEC_CMD_VPSS_FRAME_S*)arg)->stBufferAttr);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_SetExtBuffer err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetFrameBufferMode(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_SetFrameBufferMode(hHandle, ((VDEC_CMD_SET_BUFFERMODE_S*)arg)->enFrameBufferMode);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_SetFrameBufferMode err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_CheckAndDelBuffer(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_CheckAndDelBuffer(hHandle, &(((VDEC_CMD_CHECKANDDELBUFFER_S*)arg)->stBufInfo));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_SetExtBufferState err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetExtBufferState(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_SetExtBufferState(hHandle, ((VDEC_CMD_SETEXTBUFFERTATE_S*)arg)->enExtBufferState);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_SetExtBufferState err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_SetResolution(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hHandle = *((HI_HANDLE*)arg);

    s32Ret = VDEC_Chan_SetResolution(hHandle, ((VDEC_CMD_SETRESOLUTION_S*)arg)->stResolution);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_SetResolution err!:%d\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetLeftStreamFrm(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = VDEC_Chan_GetLeftStreamFrm(hHandle, &(((VDEC_CMD_GETLEFTSTREAMFRAME_S*)arg)->u32LeftFrm));
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VDEC("VDEC_Chan_GetLeftStreamFrm err!:%d\n", s32Ret);
    }

    return s32Ret;
}

#ifdef VFMW_VPSS_BYPASS_EN
static HI_S32 VDEC_Ioctl_GolbaRelease(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (arg == HI_NULL)
    {
        HI_ERR_VDEC("CMD %d Bad arg!\n", cmd);
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_GlobalRelFrm(&((VDEC_CMD_GLOBAL_REL_FRM_S*)arg)->FrmInfo);
    if (s32Ret != HI_SUCCESS)
    {
#ifdef ANDROID
        s32Ret = decoder_global_release_frame(&((VDEC_CMD_GLOBAL_REL_FRM_S*)arg)->FrmInfo);

        if (s32Ret != HI_SUCCESS)
#endif
        {
            HI_ERR_VDEC("HI_DRV_VDEC_GlobalRelFrm err!:%d\n", s32Ret);
        }
    }

    return s32Ret;
}

static HI_S32 VDEC_Ioctl_GetVidoByPass(struct file*  filp, unsigned int cmd, void* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if (VDEC_Ioctl_GetAndCheckHandle(cmd, arg, &hHandle) != HI_SUCCESS)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VDEC_GetVideoBypassInfo(hHandle, &((VDEC_CMD_GET_VIDEO_BYPASS_S*)arg)->bVideoBypass);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VDEC("VDEC_Chan_GetVideoBypassInfo err!:%d\n", s32Ret);
    }

    return s32Ret;
}

#endif

static const IOCTL_COMMAND_NODE g_IOCTL_CommandTable[] =
{
    {UMAPC_VDEC_CHAN_USRDATA,               VDEC_Ioctl_GetUsrData},
    {UMAPC_VDEC_CHAN_IFRMDECODE,            VDEC_Ioctl_IFRMDECODE},
    {UMAPC_VDEC_CHAN_IFRMRELEASE,           VDEC_Ioctl_ReleaseIFrame},
    {UMAPC_VDEC_CHAN_ALLOC,                 VDEC_Ioctl_ChanAlloc},
    {UMAPC_VDEC_CHAN_FREE,                  VDEC_Ioctl_ChanFree},
    {UMAPC_VDEC_CHAN_START,                 VDEC_Ioctl_ChanStart},
    {UMAPC_VDEC_CHAN_STOP,                  VDEC_Ioctl_ChanStop},
    {UMAPC_VDEC_CHAN_RESET,                 VDEC_Ioctl_ChanReset},
    {UMAPC_VDEC_CHAN_SETATTR,               VDEC_Ioctl_ChanSetAttr},
#ifdef HI_HDR_SUPPORT
    {UMAPC_VDEC_CHAN_SET_HDR_ATTR,          VDEC_Ioctl_ChanSetHDRAttr},
#endif
    {UMAPC_VDEC_CHAN_GETATTR,               VDEC_Ioctl_ChanGetAttr},
    {UMAPC_VDEC_CREATE_ESBUF,               VDEC_Ioctl_CreatEsBuffer},
    {UMAPC_VDEC_DESTROY_ESBUF,              VDEC_Ioctl_DestroyEsBuffer},
    {UMAPC_VDEC_GETBUF,                     VDEC_Ioctl_GetEsBuffer},
    {UMAPC_VDEC_PUTBUF,                     VDEC_Ioctl_PutEsBuffer},
    {UMAPC_VDEC_SETUSERADDR,                VDEC_Ioctl_SetUserAddr},
    {UMAPC_VDEC_RCVBUF,                     VDEC_Ioctl_RcveiveBuffer},
    {UMAPC_VDEC_RLSBUF,                     VDEC_Ioctl_RleaseBuffer},
    {UMAPC_VDEC_RESET_ESBUF,                VDEC_Ioctl_ResetEsBuffer},
    {UMAPC_VDEC_GET_ESBUF_STATUS,           VDEC_Ioctl_GetStatus},
    {UMAPC_VDEC_CHAN_RLSFRM,                VDEC_Ioctl_RlsFrmBuf},
    {UMAPC_VDEC_CHAN_RCVFRM,                VDEC_Ioctl_RecvFrmBuf},
    {UMAPC_VDEC_CHAN_STATUSINFO,            VDEC_Ioctl_GetChanStatusInfo},
    {UMAPC_VDEC_CHAN_STREAMINFO,            VDEC_Ioctl_GetChanStreamInfo},
    {UMAPC_VDEC_CHAN_ATTACHBUF,             VDEC_Ioctl_AttachStrmBuf},
    {UMAPC_VDEC_CHAN_DETACHBUF,             VDEC_Ioctl_DetachStrmBuf},
    {UMAPC_VDEC_CHAN_SETEOSFLAG,            VDEC_Ioctl_SetEosFlag},
    {UMAPC_VDEC_CHAN_DISCARDFRM,            VDEC_Ioctl_DiscardFrm},
    {UMAPC_VDEC_CHAN_CHECKEVT,              VDEC_Ioctl_CheckNewEvent},
    {UMAPC_VDEC_CHAN_EVNET_NEWFRAME,        VDEC_Ioctl_EventNewFrm},
    {UMAPC_VDEC_CHAN_GETFRM,                VDEC_Ioctl_GetFrmBuf},
    {UMAPC_VDEC_CHAN_PUTFRM,                VDEC_Ioctl_PutFrmBuf},
    {UMAPC_VDEC_CHAN_SETFRMRATE,            VDEC_Ioctl_SetFrmRate},
    {UMAPC_VDEC_CHAN_GETFRMRATE,            VDEC_Ioctl_GetFrmRate},
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    {UMAPC_VDEC_CHAN_USERDATAINITBUF,       VDEC_Ioctl_UserDataInitBuf},
    {UMAPC_VDEC_CHAN_USERDATASETBUFADDR,    VDEC_Ioctl_UserDataSetBufAddr},
    {UMAPC_VDEC_CHAN_ACQUSERDATA,           VDEC_Ioctl_AcqueUserData},
    {UMAPC_VDEC_CHAN_RLSUSERDATA,           VDEC_Ioctl_ReleaseUserData},
#endif
    {UMAPC_VDEC_CHAN_SEEKPTS,               VDEC_Ioctl_SeekPts},
    {UMAPC_VDEC_GETCAP,                     VDEC_Ioctl_GetCap},
    {UMAPC_VDEC_ALLOCHANDLE,                VDEC_Ioctl_AllocHandle},
    {UMAPC_VDEC_FREEHANDLE,                 VDEC_Ioctl_FreeHandle},
    {UMAPC_VDEC_CHAN_SETTRICKMODE,          VDEC_Ioctl_SetTrickMode},
    {UMAPC_VDEC_CHAN_SETCTRLINFO,           VDEC_Ioctl_SetCtrlInfo},
    {UMAPC_VDEC_CHAN_PROGRSSIVE,            VDEC_Ioctl_SetProgressive},
    {UMAPC_VDEC_CHAN_DPBFULL,               VDEC_Ioctl_SetDPBFullCtrl},
    {UMAPC_VDEC_CHAN_LOWDELAY,              VDEC_Ioctl_SetLowdelay},
#ifdef HI_TEE_SUPPORT
    {UMAPC_VDEC_CHAN_TVP,                   VDEC_Ioctl_SetTvp},
#endif
    {UMAPC_VDEC_CHAN_SETCOLORSPACE,         VDEC_Ioctl_SetColorSpace},
    {UMAPC_VDEC_GETUSERBUFFER,              VDEC_Ioctl_GetUserBuffer},
    {UMAPC_VDEC_CHAN_RCVVPSSFRM,            VDEC_Ioctl_RcvVpssFrm},
    {UMAPC_VDEC_CHAN_RLSPORTFRM,            VDEC_Ioctl_ReleasePortFrm},
    {UMAPC_VDEC_CHAN_CREATEVPSS,            VDEC_Ioctl_CreateVpss},
    {UMAPC_VDEC_CHAN_DESTORYVPSS,           VDEC_Ioctl_DestroyVpss},
    {UMAPC_VDEC_CHAN_CREATEPORT,            VDEC_Ioctl_CreatePort},
    {UMAPC_VDEC_CHAN_DESTROYPORT,           VDEC_Ioctl_DestroyPort},
    {UMAPC_VDEC_CHAN_ENABLEPORT,            VDEC_Ioctl_EnablePort},
    {UMAPC_VDEC_CHAN_DISABLEPORT,           VDEC_Ioctl_DisablePort},
    {UMAPC_VDEC_CHAN_SETPORTTYPE,           VDEC_Ioctl_SetPortType},
    {UMAPC_VDEC_CHAN_CANCLEMAINPORT,        VDEC_Ioctl_CancleMainPort},
    {UMAPC_VDEC_CHAN_SETFRMPACKTYPE,        VDEC_Ioctl_SetFrmPackingType},
    {UMAPC_VDEC_CHAN_GETFRMPACKTYPE,        VDEC_Ioctl_GetFrmPackingType},
    {UMAPC_VDEC_CHAN_RESETVPSS,             VDEC_Ioctl_ResetVpss},
    {UMAPC_VDEC_CHAN_SENDEOS,               VDEC_Ioctl_SendEos},
    {UMAPC_VDEC_CHAN_GETPORTSTATE,          VDEC_Ioctl_GetPortState},
    {UMAPC_VDEC_CHAN_GETFRMSTATUSINFO,      VDEC_Ioctl_GetFrmStatusInfo},
    {UMAPC_VDEC_CHAN_SETPORTATTR,           VDEC_Ioctl_SetPortAttr},
    {UMAPC_VDEC_CHAN_GETPORTATTR,           VDEC_Ioctl_GetPortAttr},
    {UMAPC_VDEC_CHAN_SETEXTBUFFER,          VDEC_Ioctl_SetExtBuffer},
    {UMAPC_VDEC_CHAN_SETBUFFERMODE,         VDEC_Ioctl_SetFrameBufferMode},
    {UMAPC_VDEC_CHAN_CHECKANDDELBUFFER,     VDEC_Ioctl_CheckAndDelBuffer},
    {UMAPC_VDEC_CHAN_SETEXTBUFFERSTATE,     VDEC_Ioctl_SetExtBufferState},
    {UMAPC_VDEC_CHAN_SETRESOLUTION,         VDEC_Ioctl_SetResolution},
{UMAPC_VDEC_CHAN_GETLEFTSTREAMFRAME,        VDEC_Ioctl_GetLeftStreamFrm},
#ifdef VFMW_VPSS_BYPASS_EN
    {UMAPC_VDEC_CHAN_GLOBALREL,             VDEC_Ioctl_GolbaRelease},
    {UMAPC_VDEC_CHAN_GETVIDEOBYPASS,        VDEC_Ioctl_GetVidoByPass},
#endif
    {UMAPC_VDEC_CHAN_SCENE_MODE,            VDEC_Ioctl_SetSceneMode},

    {0,        HI_NULL}, //terminal element
};

FN_IOCTL_HANDLER VDEC_Ioctl_Get_Handler(HI_U32 Code)
{
    HI_U32 Index = 0;
    FN_IOCTL_HANDLER pTargetHandler = HI_NULL;

    while(1)
    {
        if (g_IOCTL_CommandTable[Index].Code == 0 || g_IOCTL_CommandTable[Index].pHandler == HI_NULL)
        {
            break;
        }

        if (Code == g_IOCTL_CommandTable[Index].Code)
        {
            pTargetHandler = g_IOCTL_CommandTable[Index].pHandler;
            break;
        }

        Index++;
    }

    return pTargetHandler;
}

HI_S32 VDEC_Ioctl(struct inode* inode, struct file*  filp, unsigned int cmd, void* arg)
{
    FN_IOCTL_HANDLER pIOCTL_Handler = HI_NULL;

    pIOCTL_Handler = VDEC_Ioctl_Get_Handler(cmd);
    if (pIOCTL_Handler == HI_NULL)
    {
        HI_ERR_VDEC("%s: ERROR cmd %d is not supported!\n", __func__, cmd);

        return HI_FAILURE;
    }

    return pIOCTL_Handler(filp, cmd, arg);
}

static HI_S32 VDEC_RegChanProc(HI_S32 s32Num)
{
    HI_CHAR aszBuf[16];
    DRV_PROC_ITEM_S* pstItem;

    /* Check parameters */
    if (HI_NULL == s_stVdecDrv.pstProcParam)
    {
        return HI_FAILURE;
    }

    /* Create proc */
    if (-1 == s32Num)
    {
        snprintf(aszBuf, sizeof(aszBuf), "vdec_ctrl");
    }
    else
    {
        snprintf(aszBuf, sizeof(aszBuf), "vdec%02d", s32Num);
    }

    pstItem = HI_DRV_PROC_AddModule(aszBuf, HI_NULL, HI_NULL);

    if (!pstItem)
    {
        HI_FATAL_VDEC("Create vdec proc entry fail!\n");
        return HI_FAILURE;
    }

    /* Set functions */
    if (-1 == s32Num)
    {
        pstItem->read  = s_stVdecDrv.pstProcParam->pfnCtrlReadProc;
        pstItem->write = s_stVdecDrv.pstProcParam->pfnCtrlWriteProc;
    }
    else
    {
        pstItem->read  = s_stVdecDrv.pstProcParam->pfnReadProc;
        pstItem->write = s_stVdecDrv.pstProcParam->pfnWriteProc;
    }

    HI_INFO_VDEC("Create proc entry for vdec%d OK!\n", s32Num);
    return HI_SUCCESS;
}

static HI_VOID VDEC_UnRegChanProc(HI_S32 s32Num)
{
    HI_CHAR aszBuf[16];

    if (-1 == s32Num)
    {
        snprintf(aszBuf, sizeof(aszBuf), "vdec_ctrl");
    }
    else
    {
        snprintf(aszBuf, sizeof(aszBuf), "vdec%02d", s32Num);
    }

    HI_DRV_PROC_RemoveModule(aszBuf);
    return;
}

static HI_VOID VDEC_TimerFunc(HI_LENGTH_T value)
{
    HI_HANDLE hHandle;
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    VDEC_CHAN_STATINFO_S* pstStatInfo = HI_NULL;

    s_stVdecDrv.stTimer.expires  = jiffies + (HZ);
    s_stVdecDrv.stTimer.function = VDEC_TimerFunc;

    for (hHandle = 0; hHandle < HI_VDEC_MAX_INSTANCE_NEW; hHandle++)
    {
        /* Lock */
        VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

        if (HI_SUCCESS != s32Ret)
        {
            continue;
        }

        /* Check and get pstChan pointer */
        if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
            continue;
        }

        pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

        if (pstChan->enCurState != VDEC_CHAN_STATE_RUN)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
            continue;
        }

        pstStatInfo = &pstChan->stStatInfo;
        pstStatInfo->u32TotalVdecTime++;
        pstStatInfo->u32CalcBpsVdecTime++;
        pstStatInfo->u32AvrgVdecFpsLittle = (HI_U32)((pstStatInfo->u32TotalVdecOutFrame
                                            * 100) / pstStatInfo->u32TotalVdecTime);

        pstStatInfo->u32AvrgVdecFps = (HI_U32)(pstStatInfo->u32TotalVdecOutFrame / pstStatInfo->u32TotalVdecTime);
        pstStatInfo->u32AvrgVdecFpsLittle -= (pstStatInfo->u32AvrgVdecFps * 100);
        pstStatInfo->u32AvrgVdecInBps = (HI_U32)(pstStatInfo->u32TotalVdecInByte / pstStatInfo->u32CalcBpsVdecTime * 8);

        //pstStatInfo->u32AvrgVdecInBps = (HI_U32)(pstStatInfo->u32TotalVdecInByte / 1024);
        //pstStatInfo->u32TotalVdecInByte = 0;
        if (pstStatInfo->u32TotalVdecInByte > 0xFEFFFFFFUL)
        {
            pstStatInfo->u32TotalVdecInByte = 0;
            pstStatInfo->u32CalcBpsVdecTime = 0;
        }

        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    }

    add_timer(&s_stVdecDrv.stTimer);
    return;
}
HI_S32 VDEC_VPSS_Init(HI_VOID)
{
    //HI_U32 i,j;
    HI_S32 s32Ret;
    /*init vpss*/
    /*CNcomment:对vpss初始化*/
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGlobalInit)();
    return s32Ret;
}
//l00273086 start//
static UINT32 s_VdecTaskSleepTimems = 2;
static UINT32 u32TaskRunFlag = 0;

static HI_S32 VDEC_ReleaseMemForDecoder(HI_HANDLE hHandle)
{
    HI_U32 i = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    VDEC_CHANNEL_S* pstChan = NULL;

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("null pointer!\n");
        return HI_FAILURE;
    }

    if (1 != pstChan->stOption.u32DynamicFrameStoreAllocEn)
    {
        HI_DRV_VDEC_UnmapAndRelease(&pstChan->stVDHMMZBuf);
        return HI_SUCCESS;
    }

    for (i = 0; i < VDEC_MAX_BUFFER_RECORD; i++)
    {
        //节点记录的帧存没有被后级占用，直接释放
#ifdef VFMW_VPSS_BYPASS_EN   //specialFrameExist->specialFrameNum    bSpecialFrameCanRls
        if (!((s_stVdecDrv.stRemainFrmList.s32Num != 0) && (VDEC_IsSpecialFrm(&pstChan->stFrameBufferRecord[i].stFrameBuffer))))
#else
        if (!((pstChan->specialFrameExist == 1) && (pstChan->specialFramePhyAddr == pstChan->stFrameBufferRecord[i].stFrameBuffer.u32StartPhyAddr)))
#endif
        {
            //1:释放帧存节点
            if (DRV_VDEC_BUFFER_STATUS_PREALLOC == pstChan->stFrameBufferRecord[i].enFrameBufferNodeStatus)
            {
                s32Ret = VDEC_Chan_ReleasePreMem(&pstChan->stFrameBufferRecord[i].stFrameBuffer);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_VDEC("call VDEC_Chan_ReleasePreMem err, return :%#x \n", s32Ret);
                    return HI_FAILURE;
                }
            }
            if (DRV_VDEC_BUFFER_STATUS_SYSALLOC == pstChan->stFrameBufferRecord[i].enFrameBufferNodeStatus)
            {
                    HI_DRV_VDEC_ReleaseMem(&pstChan->stFrameBufferRecord[i].stFrameBuffer, pstChan->bTvp, HI_FALSE);
            }

        #ifdef HI_HDR_SUPPORT
            //2:释放hdr节点
            if (pstChan->stVdecHdrAttr.enVdecHDRType != HI_VDEC_HDR_CHAN_TYPE_BUTT)
            {
                if (DRV_VDEC_BUFFER_STATUS_PREALLOC == pstChan->stFrameBufferRecord[i].enHdrBufferNodeStatus)
                {
                    s32Ret = VDEC_Chan_ReleasePreMem(&pstChan->stFrameBufferRecord[i].stHdrBuffer);
                    if (HI_SUCCESS != s32Ret)
                    {
                        HI_ERR_VDEC("call VDEC_Chan_ReleasePreMem err, return :%#x \n", s32Ret);
                        return HI_FAILURE;
                    }
                }
                if (DRV_VDEC_BUFFER_STATUS_SYSALLOC == pstChan->stFrameBufferRecord[i].enHdrBufferNodeStatus)
                {
                    HI_DRV_VDEC_ReleaseMem(&pstChan->stFrameBufferRecord[i].stHdrBuffer, HI_FALSE, HI_TRUE);
                }
            }
        #endif

            //3:释放pmv节点
            if (DRV_VDEC_BUFFER_STATUS_PREALLOC == pstChan->stFrameBufferRecord[i].enPmvBufferNodeStatus)
            {
                s32Ret = VDEC_Chan_ReleasePreMem(&pstChan->stFrameBufferRecord[i].stPmvBuffer);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_VDEC("call VDEC_Chan_ReleasePreMem err, return :%#x \n", s32Ret);
                    return HI_FAILURE;
                }
            }
            if (DRV_VDEC_BUFFER_STATUS_SYSALLOC == pstChan->stFrameBufferRecord[i].enPmvBufferNodeStatus)
            {
                HI_DRV_VDEC_ReleaseMem(&pstChan->stFrameBufferRecord[i].stPmvBuffer, pstChan->bTvp, HI_FALSE);
            }

            //4:复位总帧存状态
            s32Ret = VDEC_DelFrmBufRec(hHandle, i);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VDEC("call VDEC_DelFrmBufRec err , return:%d\n", s32Ret);
                return HI_FAILURE;
            }
        }
        else
        {
            //后级不使用pmv数据，直接释放
            if (DRV_VDEC_BUFFER_STATUS_PREALLOC == pstChan->stFrameBufferRecord[i].enPmvBufferNodeStatus)
            {
                s32Ret = VDEC_Chan_ReleasePreMem(&pstChan->stFrameBufferRecord[i].stPmvBuffer);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_VDEC("call VDEC_Chan_ReleasePreMem err, return :%#x \n", s32Ret);
                    return HI_FAILURE;
                }
            }
            if (DRV_VDEC_BUFFER_STATUS_SYSALLOC == pstChan->stFrameBufferRecord[i].enPmvBufferNodeStatus)
            {
                HI_DRV_VDEC_ReleaseMem(&pstChan->stFrameBufferRecord[i].stPmvBuffer, pstChan->bTvp, HI_FALSE);
            }
            pstChan->stFrameBufferRecord[i].enPmvBufferNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
            pstChan->stFrameBufferRecord[i].enFrameBufferNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
        }
    }


    pstChan->stVFMWConfiguredBufferNum.u32ImageNum       = 0;
    pstChan->stVFMWConfiguredBufferNum.u32PmvNum         = 0;
    return HI_SUCCESS;

}

static HI_S32 VDEC_SetUseableBufferToDecoder(HI_HANDLE hHandle)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i = 0;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    FRAME_BUF_S   stFrameBuf;
    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("%s, pstChan = NULL\n", __FUNCTION__);
        return HI_FAILURE;
    }
    for (i = 0; i < VDEC_MAX_BUFFER_RECORD; i++)
    {
        if (HI_FALSE == pstChan->stFrameBufferRecord[i].bConfigured && HI_TRUE == pstChan->stFrameBufferRecord[i].bAvailable)
        {
            stFrameBuf.enBufHdrMetadataNodeStatus   =  pstChan->stFrameBufferRecord[i].enHdrBufferNodeStatus;
            stFrameBuf.enBufNodeStatus              =  pstChan->stFrameBufferRecord[i].enFrameBufferNodeStatus;
            stFrameBuf.enBufPmvNodeStatus           =  pstChan->stFrameBufferRecord[i].enPmvBufferNodeStatus;
            stFrameBuf.u32StartPhyAddr              = pstChan->stFrameBufferRecord[i].stFrameBuffer.u32StartPhyAddr;
            stFrameBuf.u64StartVirAddr              = (HI_U64)(HI_SIZE_T)pstChan->stFrameBufferRecord[i].stFrameBuffer.pu8StartVirAddr;
            stFrameBuf.u32Size                      = pstChan->stFrameBufferRecord[i].stFrameBuffer.u32Size;
            stFrameBuf.u32HdrMetadataStartPhyAddr   = pstChan->stFrameBufferRecord[i].stHdrBuffer.u32StartPhyAddr;
            stFrameBuf.u64HdrMetadataStartVirAddr   = (HI_U64)(HI_SIZE_T)pstChan->stFrameBufferRecord[i].stHdrBuffer.pu8StartVirAddr;
            stFrameBuf.u32HdrMetadataSize           = pstChan->stFrameBufferRecord[i].stHdrBuffer.u32Size;
            stFrameBuf.u32PmvStartPhyAddr           = pstChan->stFrameBufferRecord[i].stPmvBuffer.u32StartPhyAddr;
            stFrameBuf.u64PmvStartVirAddr           = (HI_U64)(HI_SIZE_T)pstChan->stFrameBufferRecord[i].stPmvBuffer.pu8StartVirAddr;
            stFrameBuf.u32PmvSize                   = pstChan->stFrameBufferRecord[i].stPmvBuffer.u32Size;
            s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_ALLOC_MEM_TO_DECODER, (HI_VOID*)&stFrameBuf);
            if (s32Ret == HI_SUCCESS)
            {
                pstChan->stFrameBufferRecord[i].bConfigured = HI_TRUE;
                pstChan->stFrameBufferRecord[i].bAvailable  = HI_FALSE;
                pstChan->stVFMWConfiguredBufferNum.u32ImageNum++;
                pstChan->stVFMWConfiguredBufferNum.u32PmvNum++;
                return HI_SUCCESS;
            }
        }
    }
    return HI_FAILURE;
}

static HI_VOID GetAllocMemInfo(VDEC_CHANNEL_S* pstChan, VDEC_AllocMem_Info_S *pstAllocMemInfo)
{
    HI_U32 u32Index = 0;
    HI_U32 u32CfgCnt = 0;

    //计算此通道还需要配置的帧存，pmv，hdr buffer的个数
    if(pstChan->stVFMWReportFrameInfo.u32AllocFrameOnly == 0)
    {
        pstAllocMemInfo->u32NeededFrameNum = pstChan->stVFMWReportFrameInfo.u32ImageNum +
            pstChan->stOption.s32ExtraFrameStoreNum - pstChan->stVFMWConfiguredBufferNum.u32ImageNum;
    }
    else
    {
        //VP9 动态帧存时VFMW 上报总帧存数
        if(pstChan->stVFMWReportFrameInfo.u32ImageNumOnly > 0)
        {
            if(pstChan->stVFMWReportFrameInfo.u32ImageSizeOnly == pstChan->stVFMWReportFrameInfo.u32ImageSize)
            {
                pstChan->stVFMWReportFrameInfo.u32ImageNum = pstChan->stVFMWReportFrameInfo.u32ImageNumOnly;
            }
            else
            {
                for(u32Index=0; u32Index<VDEC_MAX_BUFFER_RECORD; u32Index++)
                {
                    //查找该分辨率已经配置的帧存
                    if(pstChan->stFrameBufferRecord[u32Index].enFrameBufferNodeStatus != DRV_VDEC_BUFFER_STATUS_BUTT &&
                       pstChan->stFrameBufferRecord[u32Index].stFrameBuffer.u32Size == pstChan->stVFMWReportFrameInfo.u32ImageSizeOnly &&
                       pstChan->stFrameBufferRecord[u32Index].bConfigured == HI_TRUE &&
                       pstChan->stFrameBufferRecord[u32Index].bWaitRelease == HI_FALSE)
                    {
                        u32CfgCnt ++;
                    }
                }

                pstChan->stVFMWConfiguredBufferNum.u32ImageNum = u32CfgCnt;
                pstChan->stVFMWReportFrameInfo.u32ImageNum = pstChan->stVFMWReportFrameInfo.u32ImageNumOnly;
                pstChan->stVFMWReportFrameInfo.u32ImageSize = pstChan->stVFMWReportFrameInfo.u32ImageSizeOnly;
            }
            pstChan->stVFMWReportFrameInfo.u32ImageNumOnly = 0;
        }
        if(pstChan->stVFMWReportFrameInfo.u32ImageNum > pstChan->stVFMWConfiguredBufferNum.u32ImageNum)
        {
            pstAllocMemInfo->u32NeededFrameNum = pstChan->stVFMWReportFrameInfo.u32ImageNum - pstChan->stVFMWConfiguredBufferNum.u32ImageNum;
        }
        else
        {
            pstAllocMemInfo->u32NeededFrameNum = 0;
        }
    }

    pstAllocMemInfo->u32NeededPmvNum  = pstChan->stVFMWReportFrameInfo.u32PmvNum - pstChan->stVFMWConfiguredBufferNum.u32PmvNum;

#ifdef HI_HDR_SUPPORT
    pstAllocMemInfo->u32NeededHdrMetadataNum = (pstChan->stVdecHdrAttr.enVdecHDRType != HI_VDEC_HDR_CHAN_TYPE_BUTT) ?
                                               pstAllocMemInfo->u32NeededFrameNum : 0;
#else
    pstAllocMemInfo->u32NeededHdrMetadataNum = 0;
#endif

    return;
}

static HI_S32 CheckIsNeedAllocMem(VDEC_CHANNEL_S* pstChan, VDEC_AllocMem_Info_S *pstAllocMemInfo)
{
    if ((1 == pstChan->stOption.u32DynamicFrameStoreAllocEn)
        && (pstAllocMemInfo->u32NeededFrameNum > 0)
        && (pstChan->stVFMWReportFrameInfo.u32ImageNum > 0))
    {
        return 1;
    }

    return 0;
}

static HI_VOID AllocFrameMem(VDEC_CHANNEL_S* pstChan,
                             VDEC_AllocMem_Info_S *pstAllocMemInfo,
                             VDEC_BUFFER_S *pstMemBuffer,
                             FRAME_BUF_S   *pstFrameBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;

    pstMemBuffer->u32Size = pstChan->stVFMWReportFrameInfo.u32ImageSize;
    //有预分配的话先从预分配里面拿帧存数据，没有的话采用动态分配
#ifdef HI_TEE_SUPPORT

    if (HI_TRUE == pstChan->bTvp)
    {
        s32Ret = HI_FAILURE;
    }
    else
#endif
    {

        s32Ret = VDEC_Chan_FindPreMem(pstMemBuffer);
    }

    if (HI_SUCCESS != s32Ret)
    {
        //预分配分配不到的话直接从系统里面分配
        s32Ret = HI_DRV_VDEC_AllocMem("VFMW_FRM_BUF", NULL, pstMemBuffer->u32Size, 0, pstMemBuffer, pstChan->bTvp, HI_FALSE);

        if (HI_SUCCESS != s32Ret)
        {
            HI_WARN_VDEC("no memory !\n");
            //状态为BUTT表示此节点记录的帧存信息无效
            pstFrameBuf->enBufNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
            pstAllocMemInfo->bAllocFrameBuffer = HI_FALSE;
            return; //防止Frame分配失败，继续分配metadata与pmv，而该内存又不会配置下去，导致metadata与pmv无法释放。
        }
        else
        {
            //状态为SYSALLOC表示此节点记录的帧存信息有效，且内存来自动态分配
            pstFrameBuf->enBufNodeStatus = DRV_VDEC_BUFFER_STATUS_SYSALLOC;
            pstAllocMemInfo->bAllocFrameBuffer = HI_TRUE;
        }
    }
    else
    {
        //状态为PREALLOC表示节点记录的帧存信息有效，且内存来自预分配
        pstFrameBuf->enBufNodeStatus = DRV_VDEC_BUFFER_STATUS_PREALLOC;
        pstAllocMemInfo->bAllocFrameBuffer = HI_TRUE;
    }

    //记录帧存信息到局部变量stFrameBuf中，配置给vfmw
    pstFrameBuf->u32StartPhyAddr = pstMemBuffer->u32StartPhyAddr;
    pstFrameBuf->u64StartVirAddr = (HI_U64)(HI_SIZE_T)pstMemBuffer->pu8StartVirAddr;
    pstFrameBuf->u32Size         = pstMemBuffer->u32Size;

    return;
}


static HI_VOID AllocMetadataMem(VDEC_CHANNEL_S* pstChan,
                                VDEC_AllocMem_Info_S *pstAllocMemInfo,
                                VDEC_BUFFER_S *pstMemBuffer,
                                FRAME_BUF_S   *pstFrameBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;

    //2:分配hdr metadata buffer
    if (pstAllocMemInfo->u32NeededHdrMetadataNum != 0)
    {

        memset(pstMemBuffer, 0, sizeof(VDEC_BUFFER_S));
        pstMemBuffer->u32Size = pstChan->stVFMWReportFrameInfo.u32HdrMetadataSize;
        //hdr metadata信息直接从系统里面动态分配，因为vo要用此片buffer映射用户态虚拟地址，如果传递的物理地址不是直接从系统里面分配的话，会映射失败
        s32Ret = HI_DRV_VDEC_AllocMem("VFMW_HDR_BUF", NULL, pstMemBuffer->u32Size, 0, pstMemBuffer, HI_FALSE, HI_TRUE);

        if (HI_SUCCESS != s32Ret)
        {
            HI_WARN_VDEC("no memory !\n");
            pstFrameBuf->enBufHdrMetadataNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
            pstAllocMemInfo->bAllocHdrMetadataBuffer = HI_FALSE;

            if ( pstFrameBuf->enBufNodeStatus == DRV_VDEC_BUFFER_STATUS_SYSALLOC
                 && pstAllocMemInfo->bAllocFrameBuffer == HI_TRUE)
            {
                pstMemBuffer->u32StartPhyAddr = pstFrameBuf->u32StartPhyAddr;
                pstMemBuffer->pu8StartVirAddr = (HI_U8*)(HI_SIZE_T)pstFrameBuf->u64StartVirAddr;
                pstMemBuffer->u32Size = pstFrameBuf->u32Size;
                HI_DRV_VDEC_ReleaseMem(pstMemBuffer, pstChan->bTvp, HI_FALSE);
            }

            return;//防止metadata分配失败，继续分配pmv，而该内存又不会配置下去，导致frame与pmv无法释放。
        }
        else
        {
            pstFrameBuf->enBufHdrMetadataNodeStatus = DRV_VDEC_BUFFER_STATUS_SYSALLOC;
            pstAllocMemInfo->bAllocHdrMetadataBuffer = HI_TRUE;
        }

        //记录metadata信息到局部变量stFrameBuf中，配置给vfmw
        pstFrameBuf->u32HdrMetadataStartPhyAddr = pstMemBuffer->u32StartPhyAddr;
        pstFrameBuf->u64HdrMetadataStartVirAddr = (HI_U64)(HI_SIZE_T)pstMemBuffer->pu8StartVirAddr;
        pstFrameBuf->u32HdrMetadataSize         = pstMemBuffer->u32Size;

    }
    else
    {
        pstFrameBuf->enBufHdrMetadataNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
    }

    return;
}

static HI_VOID AllocPmvMem(VDEC_CHANNEL_S* pstChan,
                                VDEC_AllocMem_Info_S *pstAllocMemInfo,
                                VDEC_BUFFER_S *pstMemBuffer,
                                FRAME_BUF_S   *pstFrameBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;

    //如果pmv的buffer已经配置完成则不继续申请，pmv的buffer个数为参考帧+1，比帧存buffer的个数少
    if (pstAllocMemInfo->u32NeededPmvNum != 0)
    {
        memset(pstMemBuffer, 0, sizeof(VDEC_BUFFER_S));
        pstMemBuffer->u32Size = pstChan->stVFMWReportFrameInfo.u32PmvSize;
        //有预分配的话先从预分配里面拿帧存数据，没有的话采用动态分配
#ifdef HI_TEE_SUPPORT
        if (HI_TRUE == pstChan->bTvp)
        {
            s32Ret = HI_FAILURE;
        }
        else
#endif
        {
            s32Ret = VDEC_Chan_FindPreMem(pstMemBuffer);
        }

        if (HI_SUCCESS != s32Ret)
        {
            //预分配分配不到的话直接从系统里面分配
            s32Ret = HI_DRV_VDEC_AllocMem("VFMW_PMV_BUF", NULL, pstMemBuffer->u32Size, 0, pstMemBuffer, pstChan->bTvp, HI_FALSE);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("no memory !\n");
                //无效节点
                pstFrameBuf->enBufPmvNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
                pstAllocMemInfo->bAllocPmvBuffer = HI_FALSE;

                if (pstFrameBuf->enBufNodeStatus == DRV_VDEC_BUFFER_STATUS_SYSALLOC
                    && pstAllocMemInfo->bAllocFrameBuffer == HI_TRUE)
                {
                    pstMemBuffer->u32StartPhyAddr = pstFrameBuf->u32StartPhyAddr;
                    pstMemBuffer->pu8StartVirAddr = (HI_U8*)(HI_SIZE_T)pstFrameBuf->u64StartVirAddr;
                    pstMemBuffer->u32Size = pstFrameBuf->u32Size;
                    HI_DRV_VDEC_ReleaseMem(pstMemBuffer, pstChan->bTvp, HI_FALSE);
                }

                if (pstFrameBuf->enBufHdrMetadataNodeStatus == DRV_VDEC_BUFFER_STATUS_SYSALLOC
                    && pstAllocMemInfo->bAllocHdrMetadataBuffer == HI_TRUE)
                {
                    pstMemBuffer->u32StartPhyAddr = pstFrameBuf->u32HdrMetadataStartPhyAddr;
                    pstMemBuffer->pu8StartVirAddr = (HI_U8*)(HI_SIZE_T)pstFrameBuf->u64HdrMetadataStartVirAddr;
                    pstMemBuffer->u32Size = pstFrameBuf->u32HdrMetadataSize;
                    HI_DRV_VDEC_ReleaseMem(pstMemBuffer, pstChan->bTvp, HI_TRUE);
                }
            }
            else
            {
                //有效节点，内存来自系统动态分配
                pstFrameBuf->enBufPmvNodeStatus = DRV_VDEC_BUFFER_STATUS_SYSALLOC;
                pstAllocMemInfo->bAllocPmvBuffer = HI_TRUE;
            }
        }
        else
        {
            //有效节点，内存来自预分配
            pstFrameBuf->enBufPmvNodeStatus = DRV_VDEC_BUFFER_STATUS_PREALLOC;
            pstAllocMemInfo->bAllocPmvBuffer = HI_TRUE;
        }

        //记录pmv信息到局部变量stFrameBuf中，配置给vfmw
        pstFrameBuf->u32PmvStartPhyAddr = pstMemBuffer->u32StartPhyAddr;
        pstFrameBuf->u64PmvStartVirAddr = (HI_U64)(HI_SIZE_T)pstMemBuffer->pu8StartVirAddr;
        pstFrameBuf->u32PmvSize         = pstMemBuffer->u32Size;
    }
    else
    {
        pstFrameBuf->enBufPmvNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
    }

    return;
}

static HI_S32 CheckIsAllMemReady(VDEC_AllocMem_Info_S *pstAllocMemInfo)
{
    if ((pstAllocMemInfo->bAllocFrameBuffer == HI_TRUE)
        && ((pstAllocMemInfo->u32NeededHdrMetadataNum > 0
        && pstAllocMemInfo->bAllocHdrMetadataBuffer == HI_TRUE)
        || (pstAllocMemInfo->u32NeededHdrMetadataNum == 0))
        && ((pstAllocMemInfo->u32NeededPmvNum > 0
        && HI_TRUE == pstAllocMemInfo->bAllocPmvBuffer)
        || (pstAllocMemInfo->u32NeededPmvNum == 0)))
    {
        return 1;
    }

    return 0;
}

static HI_VOID SetAndRecodrMem(HI_HANDLE hHandle,
                               VDEC_CHANNEL_S* pstChan,
                               VDEC_AllocMem_Info_S *pstAllocMemInfo,
                               FRAME_BUF_S   *pstFrameBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Index = 0;

    //同时分配到frame,pmv,hdr节点才就配置给vfmw,但是如果pmv节点已经配置够了，此次配置则不需要配置pmv节点
    if (CheckIsAllMemReady(pstAllocMemInfo))
    {
        s32Ret = VDEC_AddFrmBufRec(hHandle, pstFrameBuf, &u32Index);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("VDEC_AddFrmBufRec err!\n");
        }

        s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_ALLOC_MEM_TO_DECODER, (HI_VOID*)pstFrameBuf);
        if (s32Ret == HI_SUCCESS)
        {
            if (u32Index < VDEC_MAX_BUFFER_RECORD)
            {
                pstChan->stFrameBufferRecord[u32Index].bConfigured = HI_TRUE;
                pstChan->stFrameBufferRecord[u32Index].bAvailable   = HI_FALSE;
            }

            pstChan->stVFMWConfiguredBufferNum.u32ImageNum++;

            if (pstAllocMemInfo->u32NeededPmvNum > 0)
            {
                pstChan->stVFMWConfiguredBufferNum.u32PmvNum++;
                pstAllocMemInfo->u32NeededPmvNum--;
            }
        }
        else
        {
            //记录此节点信息信息可用，下次再配置给vfmw
            if (u32Index < VDEC_MAX_BUFFER_RECORD)
            {
                pstChan->stFrameBufferRecord[u32Index].bConfigured = HI_FALSE;
                pstChan->stFrameBufferRecord[u32Index].bAvailable   = HI_TRUE;
            }
        }
    }

    return;
}

static HI_S32 VDEC_CheckFrameResize(HI_U32 OldSize, VDEC_CHANNEL_S* pstChan)
{
    HI_S32 Resize = 0;

    if (((pstChan->stVFMWReportFrameInfo.u32AllocFrameOnly == 0)
        && (OldSize != pstChan->stVFMWReportFrameInfo.u32ImageSize))
        || ((pstChan->stVFMWReportFrameInfo.u32AllocFrameOnly == 1)
        && (OldSize != pstChan->stVFMWReportFrameInfo.u32ImageSizeOnly)))
    {
        Resize = 1;
    }

    return Resize;
}

static HI_S32 VDEC_AllocMemForDecoder(HI_HANDLE hHandle)
{
    HI_U32 u32Index = 0;
    HI_S32 i = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 OldImageSize = 0;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    VDEC_CHAN_FRAME_STORE_PARAM_S stFsParam;
    FRAME_BUF_S   stFrameBuf;
    VDEC_AllocMem_Info_S stAllocMemInfo = {0};
    VDEC_BUFFER_S stMemBuffer = {0};
    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    memset(&stFsParam, 0, sizeof(stFsParam));
    memset(&stAllocMemInfo, 0, sizeof(stAllocMemInfo));

    VDEC_ASSERT_RETURN(pstChan != HI_NULL, HI_FAILURE);

    VDEC_ChanControlStateSpinLockIRQ(&(pstChan->stAllocMemLock));
    GetAllocMemInfo(pstChan, &stAllocMemInfo);
    VDEC_ChanControlStateSpinUnlockIRQ(&(pstChan->stAllocMemLock));

    if (pstChan->stVFMWReportFrameInfo.u32AllocFrameOnly == 1)
    {
        //动态释放VP9 帧存
        for (u32Index = 0; u32Index < VDEC_MAX_BUFFER_RECORD; u32Index++)
        {
            if (pstChan->stFrameBufferRecord[u32Index].enFrameBufferNodeStatus != DRV_VDEC_BUFFER_STATUS_BUTT &&
                pstChan->stFrameBufferRecord[u32Index].bWaitRelease == HI_TRUE)
            {
                HI_DRV_VDEC_ReleaseMem(&pstChan->stFrameBufferRecord[u32Index].stFrameBuffer, pstChan->bTvp, HI_FALSE);

                pstChan->stFrameBufferRecord[u32Index].stFrameBuffer.u32Size = 0;
                pstChan->stFrameBufferRecord[u32Index].stFrameBuffer.u32StartPhyAddr = 0;
                pstChan->stFrameBufferRecord[u32Index].enFrameBufferNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;

                if (pstChan->stFrameBufferRecord[u32Index].enPmvBufferNodeStatus == DRV_VDEC_BUFFER_STATUS_BUTT &&
                    pstChan->stFrameBufferRecord[u32Index].enHdrBufferNodeStatus == DRV_VDEC_BUFFER_STATUS_BUTT)
                {
                    pstChan->stFrameBufferRecord[u32Index].bAvailable = HI_FALSE;
                    pstChan->stFrameBufferRecord[u32Index].bConfigured = HI_FALSE;
                }

                pstChan->stFrameBufferRecord[u32Index].bWaitRelease = HI_FALSE;
            }
        }
    }

    if (CheckIsNeedAllocMem(pstChan, &stAllocMemInfo))
    {
        s32Ret = VDEC_SetChanControlState(hHandle, ALLOC_FRAME_BUF_TO_CHAN);
        VDEC_ASSERT_RETURN(s32Ret == HI_SUCCESS, s32Ret);

        //如果通道第一次或者需要重新配置帧存，则先清空节点信息，并释放上一次配置的帧存
        if (0 == pstChan->stVFMWConfiguredBufferNum.u32ImageNum && pstChan->stVFMWReportFrameInfo.u32AllocFrameOnly == 0)
        {
            //通道配置新帧存数据之前释放上一个分辨率分配下来的帧存
            s32Ret = VDEC_ReleaseMemForDecoder(hHandle);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VDEC("VDEC_ReleaseMemForDecoder err! return:%#x .\n", s32Ret);
            }
            // 清空通道里帧存分配的记录,但是不清除变分辨率之前被后级占用的帧
            s32Ret = VDEC_ResetChanFrmBuf(hHandle);

            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VDEC("VDEC_ResetChanFrmBuf err! return:%#x .\n", s32Ret);
                VDEC_SetChanControlState(hHandle, NORMAL_CHAN);

                return s32Ret;
            }
        }

        OldImageSize = pstChan->stVFMWReportFrameInfo.u32ImageSize;
        for (i = 0; i < stAllocMemInfo.u32NeededFrameNum; i++)
        {
            if (VDEC_CheckFrameResize(OldImageSize, pstChan) == 1)
            {
                break;
            }

            //先检查一下有没有可以直接配置给vfmw的节点，有的话直接配置
            s32Ret = VDEC_SetUseableBufferToDecoder(hHandle);
            if (s32Ret == HI_SUCCESS)
            {
                continue;
            }

            //如果没有可以直接配置给vfmw的节点，则新分配
            //1:分配帧存buffer
            memset(&stMemBuffer, 0, sizeof(VDEC_BUFFER_S));
            memset(&stFrameBuf, 0, sizeof(FRAME_BUF_S));

            stFrameBuf.enBufNodeStatus            = DRV_VDEC_BUFFER_STATUS_BUTT;
            stFrameBuf.enBufHdrMetadataNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
            stFrameBuf.enBufPmvNodeStatus         = DRV_VDEC_BUFFER_STATUS_BUTT;

            AllocFrameMem(pstChan, &stAllocMemInfo, &stMemBuffer, &stFrameBuf);
            AllocMetadataMem(pstChan, &stAllocMemInfo, &stMemBuffer, &stFrameBuf);
            AllocPmvMem(pstChan, &stAllocMemInfo, &stMemBuffer, &stFrameBuf);

            SetAndRecodrMem(hHandle, pstChan, &stAllocMemInfo, &stFrameBuf);
        }

        s32Ret = VDEC_SetChanControlState(hHandle, NORMAL_CHAN);
    }

    return s32Ret;
}

static SINT32 VdecTaskFunc(VOID* param)   //l00273086
{
    UINT32 i = 0;
    SINT32 s32Ret = HI_SUCCESS;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    u32TaskRunFlag = 1;//标识Task开始运转，防止task还没有开始run就被强制close导致挂住

    while (1)
    {
        /* Lock */
        down(&s_stVdecDrv.stSem);
        //if (down_interruptible(&s_stVdecDrv.stSem))
        //{
            //HI_ERR_VDEC("Global lock err!\n");
            //continue;
        //}

        for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
        {
            pstChan = s_stVdecDrv.astChanEntity[i].pstChan;

            if ((HI_NULL != pstChan) && (VDEC_CHAN_STATE_RUN == pstChan->enCurState))
            {
                //To do
                VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

                if (HI_SUCCESS != s32Ret)
                {
                    continue;
                }

                s32Ret = VDEC_AllocMemForDecoder(i);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_INFO_VDEC("VDEC_AllocMemForDecoder chan %d  err\n", i);
                }

                VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
            }
            else
            {
                //To do
            }
        }

#ifdef VFMW_VPSS_BYPASS_EN   //specialFrameExist->specialFrameNum    bSpecialFrameCanRls
        //BUFMNG_LOCK(s_stVdecDrv.stRemainFrmList.stSem);
        if (s_stVdecDrv.stRemainFrmList.s32Num != 0)
        {
            HI_U32 u32Index = 0;
            HI_U32 j = 0;
            HI_U32 num = 0;

            num = s_stVdecDrv.stRemainFrmList.s32Num;
            for (j = 0; j<num && num>0; j++)
            {
               BUFMNG_SpinLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);
               s32Ret = BUFMNG_VDEC_List_FindNodeIndexCanRls(&s_stVdecDrv.stRemainFrmList, &u32Index);
               BUFMNG_SpinUnLockIRQ(&s_stVdecDrv.stRemainFrmList.stIrq);

               if ( HI_SUCCESS == s32Ret )
               {
                   /*find the node which can be release now! release the frameBuffer and del the Note */
                   HI_U32 u32StartPhyAddr = s_stVdecDrv.stRemainFrmList.stSpecialFrmRec[u32Index].frmBufRec.u32StartPhyAddr;

                   if (HI_SUCCESS != VDEC_DRV_GlobalRelFrmInter(u32Index))
                   {
                       HI_ERR_VDEC("release the special frameBuffer and del Note failed!\n");
                   }
                   else
                   {
                        HI_INFO_VDEC("release the special frameBuffer success! Phyaddr = 0x%x\n", u32StartPhyAddr);
                   }
               }
            }
        }
        //BUFMNG_UNLOCK(s_stVdecDrv.stRemainFrmList.stSem);
#endif
        up(&s_stVdecDrv.stSem);

        if (i >= HI_VDEC_MAX_INSTANCE_NEW)
        {
            msleep(s_VdecTaskSleepTimems);
        }
        if (kthread_should_stop())
        {
            HI_INFO_VDEC("%s, thread stop!\n", __FUNCTION__);
            break;
        }
    }

    u32TaskRunFlag = 0;

    HI_INFO_VDEC("%s,task stop, flag: %d\n", __FUNCTION__, u32TaskRunFlag);

    return s32Ret;
}
//l00273086 end//

HI_S32 HI_DRV_VDEC_GetPreAllocVideoBuffer(VDEC_MMZ_BUFFER_S* pstMMZBuf)
{
    if (HI_NULL == pstMMZBuf)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
    memcpy(pstMMZBuf, &g_stVDHMMZ, sizeof(VDEC_MMZ_BUFFER_S));
    return HI_TRUE;
#endif

    return HI_FAILURE;
}

HI_S32 VDEC_DRV_FreeAll(HI_HANDLE hHandle)
{
    HI_U32 i;
    HI_S32 s32Ret;
    HI_HANDLE hVpss;
    HI_HANDLE hPort;

    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    hVpss = s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss;

    down(&s_stVdecDrv.stSem);

    /*1:destroy all port*/
    for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
    {
        hPort = s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].hPort;

        if ((hPort != HI_INVALID_HANDLE) && (hVpss != HI_INVALID_HANDLE))
        {
            VDEC_Chan_DestroyPort(hVpss, hPort);
        }
    }

    /*2:destroy vpss*/
    if (HI_INVALID_HANDLE != hVpss)
    {
        VDEC_Chan_DestroyVpss(hHandle);
    }

    up(&s_stVdecDrv.stSem);

    /*3:destroy vfmw and vdec*/
    if (s_stVdecDrv.astChanEntity[hHandle].bUsed)
    {
        if (s_stVdecDrv.astChanEntity[hHandle].pstChan)
        {
            VDEC_Chan_Free(hHandle);
        }

        /*4:destory stream buf*/
        if (HI_INVALID_HANDLE == s_stVdecDrv.astChanEntity[hHandle].pstChan->hDmxVidChn)
        {
            s32Ret = VDEC_DestroyStrmBuf(s_stVdecDrv.astChanEntity[hHandle].pstChan->hStrmBuf);

            if (s32Ret != HI_SUCCESS)
            {
                HI_WARN_VDEC("VDEC_DestroyStrmBuf err!:%d\n", s32Ret);
            }
        }
        VDEC_Chan_FreeHandle(hHandle);
    }

    return HI_SUCCESS;
}

HI_VOID HI_DRV_VDEC_SetOmxCallBacks(OMX_EXPORT_FUNC_S* OmxCallbacks)
{
    s_stVdecDrv.pOmxFunc = OmxCallbacks;
}
static HI_S32 VDEC_OpenDev(HI_VOID)
{
    HI_U32 i;
    HI_S32 s32Ret;
    VDEC_OPERATION_S stOpt;

    /* Init global parameter */
    init_timer(&s_stVdecDrv.stTimer);

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        atomic_set(&s_stVdecDrv.astChanEntity[i].atmUseCnt, 0);
        atomic_set(&s_stVdecDrv.astChanEntity[i].atmRlsFlag, 0);
        init_waitqueue_head(&s_stVdecDrv.astChanEntity[i].stRlsQue);
        s_stVdecDrv.astChanEntity[i].stVpssChan.hVdec = HI_INVALID_HANDLE;
        s_stVdecDrv.astChanEntity[i].stVpssChan.hVpss = HI_INVALID_HANDLE;
        s_stVdecDrv.astChanEntity[i].pstChan = HI_NULL; //l00273086
        s_stVdecDrv.astChanEntity[i].u32DynamicFsEn = 0;//l00273086
    }

    /* Init buffer manager */
    BUFMNG_Init();

    /* Init vfmw */
    memset(&stOpt, 0, sizeof(VDEC_OPERATION_S));
    stOpt.eAdapterType = ADAPTER_TYPE_VDEC;
    stOpt.VdecCallback = VDEC_EventHandle;
    stOpt.mem_malloc = HI_NULL;
    stOpt.mem_free = HI_NULL;
    stOpt.is_secure = HI_FALSE;

    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwInit)(&stOpt);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Init vfmw err:%d!\n", s32Ret);
        goto err1;
    }

    /* Get vfmw capabilite */
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(HI_INVALID_HANDLE, VDEC_CID_GET_CAPABILITY, &s_stVdecDrv.stVdecCap);

    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_VDEC("VFMW GET_CAPABILITY err:%d!\n", s32Ret);
        goto err2;
    }

    /* Init pts recover function */
    PTSREC_Init();

    /* Init CC user data function */
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    USRDATA_Init();
#endif

    VDEC_VPSS_Init();
    /* Set global timer */
    s_stVdecDrv.stTimer.expires  = jiffies + (HZ);
    s_stVdecDrv.stTimer.function = VDEC_TimerFunc;
    add_timer(&s_stVdecDrv.stTimer);

    /* Set ready flag */
    s_stVdecDrv.bReady = HI_TRUE;

    HI_INFO_VDEC("VDEC_OpenDev OK.\n");

    return HI_SUCCESS;

err2:
    (s_stVdecDrv.pVfmwFunc->pfnVfmwExit)(HI_FALSE);
err1:
    BUFMNG_DeInit();
    return HI_FAILURE;
}

static HI_S32 VDEC_CloseDev(HI_VOID)
{
    HI_U32 i;

    /* Reentrant */
    if (s_stVdecDrv.bReady == HI_FALSE)
    {
        return HI_SUCCESS;
    }

    /* Set ready flag */
    s_stVdecDrv.bReady = HI_FALSE;

    /* Delete timer */
    del_timer(&s_stVdecDrv.stTimer);

    /* Free all channels */
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (HI_NULL != s_stVdecDrv.astChanEntity[i].pstChan)
        {
            VDEC_DRV_FreeAll(i);
        }
    }

    /* Vfmw exit */
#ifdef HI_TEE_SUPPORT
    (s_stVdecDrv.pVfmwFunc->pfnVfmwExit)(HI_TRUE);
#endif
    (s_stVdecDrv.pVfmwFunc->pfnVfmwExit)(HI_FALSE);


    /* Buffer manager exit  */
    BUFMNG_DeInit();

    /* Pts recover exit */
    PTSREC_DeInit();
    (s_stVdecDrv.pVpssFunc->pfnVpssGlobalDeInit)();

    /* CC user data exit */
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    USRDATA_DeInit();
#endif

    return HI_SUCCESS;
}

HI_S32 VDEC_DRV_CreateTask(HI_VOID)
{
    if (s_stVdecDrv.pVdecTask == HI_NULL)
    {
        HI_INIT_MUTEX(&s_stVdecDrv.stSem);

        s_stVdecDrv.pVdecTask = kthread_create(VdecTaskFunc, (VOID*)NULL, "HI_VDEC_Task");

        if (IS_ERR(s_stVdecDrv.pVdecTask))
        {
            HI_FATAL_VDEC("VDEC can not create thread!\n");

            return HI_FAILURE;
        }

        wake_up_process(s_stVdecDrv.pVdecTask);
    }

    return HI_SUCCESS;
}

HI_VOID VDEC_DRV_DestoryTask(HI_VOID)
{
    if ((s_stVdecDrv.pVdecTask != HI_NULL) && (u32TaskRunFlag == 1))
    {
        kthread_stop(s_stVdecDrv.pVdecTask);
        msleep(1);
        s_stVdecDrv.pVdecTask = HI_NULL;
    }

    return;
}

HI_S32 VDEC_DRV_Open(struct inode* inode, struct file*  filp)
{
    HI_S32 s32Ret;

    if (atomic_inc_return(&s_stVdecDrv.atmOpenCnt) == 1)
    {
        s_stVdecDrv.pDmxFunc  = HI_NULL;
        s_stVdecDrv.pVfmwFunc = HI_NULL;
        s_stVdecDrv.pVpssFunc = HI_NULL;

        /* Get demux functions */
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_DEMUX, (HI_VOID**)&s_stVdecDrv.pDmxFunc);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("Get demux function err:%#x!\n", s32Ret);
        }

        /* Get vfmw functions */
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_VFMW, (HI_VOID**)&s_stVdecDrv.pVfmwFunc);

        /* Check vfmw functions */
        if ((HI_SUCCESS != s32Ret)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc->pfnVfmwInit)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc->pfnVfmwExit)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc->pfnVfmwControl)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc->pfnVfmwSuspend)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc->pfnVfmwResume))
        {
            HI_FATAL_VDEC("Get vfmw function err!\n");
            goto err;
        }

        /*Get vpss functions*/
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_VPSS, (HI_VOID**)&s_stVdecDrv.pVpssFunc);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("Get vpss function err:%#x!\n", s32Ret);
            goto err;
        }

        /* Init device */
        if (HI_SUCCESS != VDEC_OpenDev())
        {
            HI_FATAL_VDEC("VDEC_OpenDev err!\n" );
            goto err;
        }
    }

    return HI_SUCCESS;

err:
    atomic_dec(&s_stVdecDrv.atmOpenCnt);
    return HI_FAILURE;
}

HI_S32 VDEC_DRV_Release(struct inode* inode, struct file* filp)
{
    HI_S32 i;
    HI_S32 s32Ret;

    /* Not the last close, only close the channel match with the 'filp' */
    if (0 != atomic_dec_return(&s_stVdecDrv.atmOpenCnt))
    {
        for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
        {
            VDEC_CHAN_ENTITY_S* Chan = &s_stVdecDrv.astChanEntity[i];

            if (Chan->u64File != ((HI_U64)(HI_SIZE_T)filp))
            {
                continue;
            }

            s32Ret = VDEC_DRV_FreeAll(i);

            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VDEC("VDEC_DRV_FreeAll err:%#x\n", s32Ret);
            }
        }
    }
    /* Last close */
    else
    {
        VDEC_CloseDev();
    }

    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (HI_TRUE == s_stVdecDrv.astChanEntity[i].bUsed)
        {
            break;
        }
    }

    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        g_VdecPreMMZNodeNum = 1;
        memset(st_VdecChanPreUsedMMZInfo, 0, sizeof(st_VdecChanPreUsedMMZInfo));
        st_VdecChanPreUsedMMZInfo[0].u32Size         = g_stVDHMMZ.u32Size;
        st_VdecChanPreUsedMMZInfo[0].u32StartPhyAddr = g_stVDHMMZ.u32StartPhyAddr;
        st_VdecChanPreUsedMMZInfo[0].pu8StartVirAddr = g_stVDHMMZ.pu8StartVirAddr;
        st_VdecChanPreUsedMMZInfo[0].u32NodeState    = 0;
    }

    return HI_SUCCESS;
}

HI_S32 VDEC_DRV_RegWatermarkFunc(FN_VDEC_Watermark pfnFunc)
{
    /* Check parameters */
    if (HI_NULL == pfnFunc)
    {
        return HI_FAILURE;
    }

    s_stVdecDrv.pfnWatermark = pfnFunc;
    return HI_SUCCESS;
}

HI_VOID VDEC_DRV_UnRegWatermarkFunc(HI_VOID)
{
    s_stVdecDrv.pfnWatermark = HI_NULL;
    return;
}

HI_S32 VDEC_DRV_RegisterProc(VDEC_REGISTER_PARAM_S* pstParam)
{
    HI_S32 i;
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_NULL == pstParam)
    {
        HI_ERR_VDEC("null pointer pstParam\n");
        return HI_FAILURE;
    }

    s_stVdecDrv.pstProcParam = pstParam;

    /* Create ctrl proc */
    s32Ret = VDEC_RegChanProc(-1);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("VDEC_RegChanProc Err!\n");
    }

    /* Create proc */
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (s_stVdecDrv.astChanEntity[i].pstChan)
        {
            if (HI_FALSE == s_stVdecDrv.astChanEntity[i].pstChan->bProcRegister)
            {
                s32Ret  = VDEC_RegChanProc(i);

                if (HI_SUCCESS == s32Ret)
                {
                    s_stVdecDrv.astChanEntity[i].pstChan->bProcRegister = HI_TRUE;
                }
                else
                {
                    s_stVdecDrv.astChanEntity[i].pstChan->bProcRegister = HI_FALSE;
                }
            }
        }
    }

    return HI_SUCCESS;
}

HI_VOID VDEC_DRV_UnregisterProc(HI_VOID)
{
    HI_S32 i;

    /* Unregister ctrl proc*/
    VDEC_UnRegChanProc(-1);

    /* Unregister */
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (s_stVdecDrv.astChanEntity[i].pstChan)
        {
            if (HI_TRUE == s_stVdecDrv.astChanEntity[i].pstChan->bProcRegister)
            {
                VDEC_UnRegChanProc(i);
                s_stVdecDrv.astChanEntity[i].pstChan->bProcRegister = HI_FALSE;
            }
        }
    }

    /* Clear param */
    s_stVdecDrv.pstProcParam = HI_NULL;
    return;
}

HI_S32 VDEC_DRV_Suspend(PM_BASEDEV_S* pdev, pm_message_t state)
{
    if (s_stVdecDrv.pVfmwFunc && s_stVdecDrv.pVfmwFunc->pfnVfmwSuspend)
    {
        if ((s_stVdecDrv.pVfmwFunc->pfnVfmwSuspend)())
        {
            HI_FATAL_VDEC("Suspend err!\n");
            return HI_FAILURE;
        }
    }

    HI_PRINT("VDEC suspend OK\n");
    return HI_SUCCESS;
}

HI_S32 VDEC_DRV_Resume(PM_BASEDEV_S* pdev)
{
    if (s_stVdecDrv.pVfmwFunc && s_stVdecDrv.pVfmwFunc->pfnVfmwResume)
    {
        if ((s_stVdecDrv.pVfmwFunc->pfnVfmwResume)())
        {
            HI_FATAL_VDEC("Resume err!\n");
            return HI_FAILURE;
        }
    }

    HI_PRINT("VDEC resume OK\n");
    return HI_SUCCESS;
}


VDEC_CHANNEL_S* VDEC_DRV_GetChan(HI_HANDLE hHandle)
{
    if (s_stVdecDrv.bReady)
    {
        if (hHandle < HI_VDEC_MAX_INSTANCE_NEW)
        {
            if (s_stVdecDrv.astChanEntity[hHandle].pstChan)
            {
                return s_stVdecDrv.astChanEntity[hHandle].pstChan;
            }
        }
    }

    return HI_NULL;
}

VFMW_EXPORT_FUNC_S* VDEC_DRV_GetVfmwExportFunc(HI_VOID)
{
    return s_stVdecDrv.pVfmwFunc;
}

static HI_VOID VDEC_GetMemConfig(HI_U32* pu32ScdBufSize, HI_U32* pu32VdhBufSize)
{
    HI_S32              Ret;
    HI_SYS_MEM_CONFIG_S MemConfig = {0};
    HI_U32              TotalFrmNum;
    HI_CHIP_TYPE_E      ChipType    = HI_CHIP_TYPE_HI3798M;
    HI_CHIP_VERSION_E   ChipVersion = HI_CHIP_VERSION_V100;

    HI_DRV_SYS_GetChipVersion(&ChipType, &ChipVersion);

#if defined(HI_VIDEO_MAX_REF_FRAME_NUM) && defined(HI_VIDEO_MAX_DISP_FRAME_NUM)
    TotalFrmNum = HI_VIDEO_MAX_REF_FRAME_NUM + HI_VIDEO_MAX_DISP_FRAME_NUM;

    if (HI_VIDEO_PRE_CAP_1080P == 1)
    {
        *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P;
    }
    else if (HI_VIDEO_PRE_CAP_MVC == 1)
    {
        *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P * 2;
    }
    else
    {
        *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P * 2;
    }

#else

    Ret = HI_DRV_SYS_GetMemConfig(&MemConfig);

    if (HI_SUCCESS != Ret)
    {
        MemConfig.u32TotalSize = 1024; // if get memory config failed, we use 1G config
    }

    switch (MemConfig.u32TotalSize)
    {
        case 256:
        {
            TotalFrmNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_256 + HI_VIDEO_MAX_DISP_FRAME_NUM_IN_256;

            if (HI_VIDEO_PRE_CAP_1080P_IN_256 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P;
            }
            else if (HI_VIDEO_PRE_CAP_MVC_IN_256 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P * 2;
            }
            else if (HI_VIDEO_PRE_CAP_2160P_IN_256 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_2160P;
            }
            else if (HI_VIDEO_PRE_CAP_4K_IN_256 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_4K;
            }
            else
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P * 2;
            }

            break;
        }

        case 512:
        {

            TotalFrmNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_512 + HI_VIDEO_MAX_DISP_FRAME_NUM_IN_512;

            if (HI_VIDEO_PRE_CAP_1080P_IN_512 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P;
            }
            else if (HI_VIDEO_PRE_CAP_MVC_IN_512 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P * 2;
            }
            else if (HI_VIDEO_PRE_CAP_2160P_IN_512 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_2160P;
            }
            else if (HI_VIDEO_PRE_CAP_4K_IN_512 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_4K;
            }
            else
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P * 2;
            }

            break;
        }

        case 768:
        {

            TotalFrmNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_768 + HI_VIDEO_MAX_DISP_FRAME_NUM_IN_768;

            if (HI_VIDEO_PRE_CAP_1080P_IN_768 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P;
            }
            else if (HI_VIDEO_PRE_CAP_MVC_IN_768 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P * 2;
            }
            else if (HI_VIDEO_PRE_CAP_2160P_IN_768 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_2160P;
            }
            else if (HI_VIDEO_PRE_CAP_4K_IN_768 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_4K;
            }
            else
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_4K;
            }

            break;
        }

        case 2048:
        {

            TotalFrmNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_2048 + HI_VIDEO_MAX_DISP_FRAME_NUM_IN_2048;

            if (HI_VIDEO_PRE_CAP_1080P_IN_2048 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P;
            }
            else if (HI_VIDEO_PRE_CAP_MVC_IN_2048 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P * 2;
            }
            else if (HI_VIDEO_PRE_CAP_2160P_IN_2048 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_2160P;
            }
            else if (HI_VIDEO_PRE_CAP_4K_IN_2048 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_4K;
            }
            else
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_4K;
            }

            break;
        }

        case 1024:
        default:
        {

            TotalFrmNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_1024 + HI_VIDEO_MAX_DISP_FRAME_NUM_IN_1024;

            if (HI_VIDEO_PRE_CAP_1080P_IN_1024 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P;
            }
            else if (HI_VIDEO_PRE_CAP_MVC_IN_1024 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_1080P * 2;
            }
            else if (HI_VIDEO_PRE_CAP_2160P_IN_1024 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_2160P;
            }
            else if (HI_VIDEO_PRE_CAP_4K_IN_1024 == 1)
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_4K;
            }
            else
            {
                *pu32VdhBufSize = TotalFrmNum * HI_VDEC_FRAME_SIZE_4K;
            }

            break;
        }
    }

#endif

    if (((HI_CHIP_TYPE_HI3798C_A == ChipType && HI_CHIP_VERSION_V200 == ChipVersion)))
    {
        *pu32VdhBufSize = (*pu32VdhBufSize) * 5 / 4;
    }

    *pu32ScdBufSize = 0;
}

HI_S32 VDEC_AllocPreBuffer(HI_VOID)
{
#if (1 == PRE_ALLOC_VDEC_SCD_MMZ) || (1 == PRE_ALLOC_VDEC_VDH_MMZ) || (1 == PRE_ALLOC_VDEC_ESBUF_MMZ)
    HI_S32              ret;
#endif
    HI_U32              ScdBufSize = 0;
    HI_U32              VdhBufSize = 0;

    /*1:calc the scd and vdh pre alloc memory size*/
    VDEC_GetMemConfig(&ScdBufSize, &VdhBufSize);

    /*2:alloc pre scd memory*/
#if (1 == PRE_ALLOC_VDEC_SCD_MMZ)

    if (ScdBufSize > 0)
    {
        ret = HI_DRV_VDEC_Alloc("VFMW_SCD_PRE", "VFMW", ScdBufSize + HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE, 0, &g_stSCDMMZ);

        if (HI_SUCCESS != ret)
        {
            HI_ERR_VDEC("SCD Alloc 0x%x failed\n", ScdBufSize);
            return HI_FAILURE;
        }
    }

#endif

    /*3:alloc pre vdh memory*/
#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)

    if (VdhBufSize > 0)
    {
        ret = HI_DRV_VDEC_Alloc("VFMW_VDH_PRE", "VFMW", VdhBufSize, 0, &g_stVDHMMZ);

        if (HI_SUCCESS != ret)
        {
            HI_ERR_VDEC("VDH Alloc 0x%x failed,ScdBufSize:0x%x\n", VdhBufSize, ScdBufSize);
            goto err1;
        }

        ret = HI_DRV_VDEC_Map(&g_stVDHMMZ);

        if (HI_SUCCESS != ret)
        {
            HI_ERR_VDEC("VDH Map failed\n");
            goto err2;
        }

        st_VdecChanPreUsedMMZInfo[0].u32Size         = g_stVDHMMZ.u32Size;
        st_VdecChanPreUsedMMZInfo[0].u32StartPhyAddr = g_stVDHMMZ.u32StartPhyAddr;
        st_VdecChanPreUsedMMZInfo[0].pu8StartVirAddr = g_stVDHMMZ.pu8StartVirAddr;
        st_VdecChanPreUsedMMZInfo[0].u32NodeState    = 0;
    }

#endif

    /*4:alloc pre es_buffer memory*/
#if (1 == PRE_ALLOC_VDEC_ESBUF_MMZ)
    ret = HI_DRV_VDEC_AllocAndMap("VDEC_ESBuf_PRE", "VDEC", ESBUF_MMZ_SIZE, 0, &g_stESBUFMMZ);

    if (HI_SUCCESS != ret)
    {
        HI_ERR_VDEC("ESBUF Alloc 0x%x failed\n", ESBUF_MMZ_SIZE);
        goto err3;
    }

#endif

    return HI_SUCCESS;

#if (1 == PRE_ALLOC_VDEC_ESBUF_MMZ)
err3:
#endif
#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
    HI_DRV_VDEC_Unmap(&g_stVDHMMZ);
    memset(&st_VdecChanPreUsedMMZInfo[0], 0, sizeof(VDEC_PREMMZ_NODE_S));
#endif
#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
err2:
    HI_DRV_VDEC_Release(&g_stVDHMMZ);
    memset(&g_stVDHMMZ, 0, sizeof(MMZ_BUFFER_S));
#endif
#if (1 == PRE_ALLOC_VDEC_SCD_MMZ)
err1:
    HI_DRV_VDEC_Release(&g_stSCDMMZ);
    memset(&g_stSCDMMZ, 0, sizeof(MMZ_BUFFER_S));
#endif
#if (1 == PRE_ALLOC_VDEC_SCD_MMZ) || (1 == PRE_ALLOC_VDEC_VDH_MMZ) || (1 == PRE_ALLOC_VDEC_ESBUF_MMZ)
    HI_ERR_VDEC("ScdBufSize:%d,VdhBufSize:%d\n", ScdBufSize, VdhBufSize);
    return HI_FAILURE;
#endif
}
HI_S32 VDEC_DRV_Init(HI_VOID)
{
    HI_S32 ret;

    BUFMNG_SaveInit();

    ret = HI_DRV_MODULE_Register(HI_ID_VDEC, VDEC_NAME, (HI_VOID*)&s_stVdecDrv.stExtFunc);

    if (HI_SUCCESS != ret)
    {
        HI_FATAL_VDEC("Reg module fail:%#x!\n", ret);
        return ret;
    }

    /*alloc pre buffer*/
    ret = VDEC_AllocPreBuffer();

    if (HI_SUCCESS != ret)
    {
        HI_FATAL_VDEC("alloc pre buffer fail:%#x!\n", ret);
        return ret;
    }

    /*MCE流程，在这里初始化bypass相关变量*/
#ifdef VFMW_VPSS_BYPASS_EN
    VDEC_DRV_SpecialFrmListInit();
#endif

    ret = VDEC_DRV_CreateTask();
    if (ret != HI_SUCCESS)
    {
        HI_FATAL_VDEC("CreateVdecTask fail!,ret=%d\n",ret);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID VDEC_DRV_Exit(HI_VOID)
{
#if (1 == PRE_ALLOC_VDEC_ESBUF_MMZ)

    if (0 != g_stESBUFMMZ.u32Size)
    {
        HI_DRV_VDEC_Release(&g_stESBUFMMZ);
        g_stESBUFMMZ.u32Size = 0;
    }

#endif

#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)

    if (0 != g_stVDHMMZ.u32Size)
    {
        HI_DRV_VDEC_Unmap(&g_stVDHMMZ);
        HI_DRV_VDEC_Release(&g_stVDHMMZ);
        g_stVDHMMZ.u32Size = 0;
    }

#endif

#if (1 == PRE_ALLOC_VDEC_SCD_MMZ)

    if (0 != g_stSCDMMZ.u32Size)
    {
        HI_DRV_VDEC_UnmapAndRelease(&g_stSCDMMZ);
        g_stSCDMMZ.u32Size = 0;
    }

#endif

    HI_DRV_MODULE_UnRegister(HI_ID_VDEC);

    return;
}

HI_S32 HI_DRV_VDEC_Init(HI_VOID)
{
    return VDEC_DRV_Init();
}

HI_VOID HI_DRV_VDEC_DeInit(HI_VOID)
{
    return VDEC_DRV_Exit();
}

HI_S32 HI_DRV_VDEC_Open(HI_VOID)
{
    return VDEC_DRV_Open(HI_NULL, HI_NULL);
}

HI_S32 HI_DRV_VDEC_Close(HI_VOID)
{
    return VDEC_DRV_Release(HI_NULL, HI_NULL);
}

HI_S32 HI_DRV_VDEC_AllocChan(HI_HANDLE* phHandle, HI_UNF_AVPLAY_OPEN_OPT_S* pstCapParam)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss = HI_INVALID_HANDLE;

    s32Ret = VDEC_Chan_AllocHandle(phHandle, (struct file*)MCE_INVALID_FILP);

    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    s32Ret = VDEC_Chan_CreateVpss((*phHandle) & 0xff, &hVpss);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_Chan_CreateVpss err!\n");
        return HI_FAILURE;
    }

    //return VDEC_Chan_Alloc((*phHandle)&0xff, pstCapParam);
    return VDEC_Chan_InitParam((*phHandle) & 0xff, pstCapParam);
}

HI_S32 HI_DRV_VDEC_FreeChan(HI_HANDLE hHandle)
{
    HI_S32 s32Ret;

    s32Ret = VDEC_Chan_Free(hHandle & 0xff);
    s32Ret |= VDEC_Chan_FreeHandle(hHandle & 0xff);
    s32Ret |= VDEC_Chan_DestroyVpss(hHandle & 0xff);

    return s32Ret;
}
HI_S32 HI_DRV_VDEC_CreateStrmBuf(HI_HANDLE hVdec, HI_DRV_VDEC_STREAM_BUF_S* pstBuf)
{
    HI_S32 s32Ret;
    s32Ret = VDEC_CreateStrmBuf(pstBuf);
    VDEC_Chan_AttachStrmBuf(hVdec, pstBuf->u32Size, HI_INVALID_HANDLE, pstBuf->hHandle);
    return s32Ret;
}
HI_S32 HI_DRV_VDEC_ChanBufferInit(HI_HANDLE hHandle, HI_U32 u32BufSize, HI_HANDLE hDmxVidChn)
{
    HI_S32 s32Ret;
    HI_DRV_VDEC_STREAM_BUF_S stBuf;

    if (HI_INVALID_HANDLE == hDmxVidChn)
    {
        stBuf.u32Size = u32BufSize;
        s32Ret = HI_DRV_VDEC_CreateStrmBuf(hHandle & 0xff, &stBuf);

        if (HI_SUCCESS != s32Ret)
        {
            return HI_FAILURE;
        }
    }
    else
    {
        s32Ret =  VDEC_Chan_AttachStrmBuf(hHandle & 0xff, u32BufSize, hDmxVidChn, HI_INVALID_HANDLE);
    }

    return s32Ret;
}

HI_S32 HI_DRV_VDEC_ChanBufferDeInit(HI_HANDLE hHandle)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_HANDLE hDemuxHandle;

    if ((hHandle & 0xff) > HI_VDEC_MAX_INSTANCE_NEW - 1)
    {
        HI_ERR_VDEC("HI_DRV_VDEC_ChanBufferDeInit error hHandle:%d wrong!\n", hHandle);
        return s32Ret;
    }

    hDemuxHandle = s_stVdecDrv.astChanEntity[hHandle & 0xff].pstChan->hDmxVidChn;

    if (HI_INVALID_HANDLE == hDemuxHandle)
    {
        s32Ret = VDEC_DestroyStrmBuf(s_stVdecDrv.astChanEntity[hHandle & 0xff].pstChan->hStrmBuf);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("VDEC_DestroyStrmBuf error!\n");
            return s32Ret;
        }
    }

    s32Ret = VDEC_Chan_DetachStrmBuf(hHandle & 0xff);
    return s32Ret;
}

HI_S32 HI_DRV_VDEC_ResetChan(HI_HANDLE hHandle)
{
    return VDEC_Chan_Reset(hHandle & 0xff, VDEC_RESET_TYPE_ALL);
}

HI_S32 HI_DRV_VDEC_SetPortType(HI_HANDLE hVdec, HI_HANDLE hPort, VDEC_PORT_TYPE_E enPortType )
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss;
    s32Ret = VDEC_FindVpssHandleByVdecHandle(hVdec & 0xff, &hVpss);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_FindVpssHandleByVdecHandle err!\n");
        return HI_FAILURE;
    }

    return VDEC_Chan_SetPortType(hVpss, hPort, enPortType );
}

HI_S32 HI_DRV_VDEC_EnablePort(HI_HANDLE hVdec, HI_HANDLE hPort)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss;
    s32Ret = VDEC_FindVpssHandleByVdecHandle(hVdec & 0xff, &hVpss);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_FindVpssHandleByVdecHandle err!\n");
        return HI_FAILURE;
    }

    return VDEC_Chan_EnablePort(hVpss, hPort);
}

HI_S32 HI_DRV_VDEC_CreatePort(HI_HANDLE hVdec, HI_HANDLE* phPort, VDEC_PORT_ABILITY_E ePortAbility)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss;
    s32Ret = VDEC_FindVpssHandleByVdecHandle(hVdec & 0xff, &hVpss);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_FindVpssHandleByVdecHandle err!\n");
        return HI_FAILURE;
    }

    return VDEC_Chan_CreatePort(hVpss, phPort, ePortAbility );
}

HI_S32 HI_DRV_VDEC_DestroyPort(HI_HANDLE hVdec, HI_HANDLE hPort)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss;
    s32Ret = VDEC_FindVpssHandleByVdecHandle(hVdec & 0xff, &hVpss);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_FindVpssHandleByVdecHandle err!\n");
        return HI_FAILURE;
    }

    return VDEC_Chan_DestroyPort(hVpss, hPort);
}

HI_S32 HI_DRV_VDEC_GetPortParam(HI_HANDLE hVdec, HI_HANDLE hPort, VDEC_PORT_PARAM_S* pstPortParam)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss;
    s32Ret = VDEC_FindVpssHandleByVdecHandle(hVdec & 0xff, &hVpss);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_FindVpssHandleByVdecHandle err!\n");
        return HI_FAILURE;
    }

    return VDEC_Chan_GetPortParam(hVpss, hPort, pstPortParam);
}
HI_S32 HI_DRV_VDEC_Chan_RecvVpssFrmBuf(HI_HANDLE hVdec, HI_DRV_VIDEO_FRAME_PACKAGE_S* pstFrm)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss;
    s32Ret = VDEC_FindVpssHandleByVdecHandle(hVdec & 0xff, &hVpss);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_FindVpssHandleByVdecHandle err!\n");
        return HI_FAILURE;
    }

    return VDEC_Chan_RecvVpssFrmBuf(hVpss, pstFrm, 0);
}
HI_S32 HI_DRV_VDEC_SetChanFrmRate(HI_HANDLE hHandle, HI_UNF_AVPLAY_FRMRATE_PARAM_S* pstFrmRate)
{
    return PTSREC_SetFrmRate(hHandle, pstFrmRate);
}
HI_S32 HI_DRV_VDEC_GetChanFrmRate(HI_HANDLE hHandle, HI_UNF_AVPLAY_FRMRATE_PARAM_S* pstFrmRate)
{
    return PTSREC_GetFrmRate(hHandle, pstFrmRate);
}
HI_VOID HI_DRV_VDEC_GetVcmpFlag(HI_BOOL* pbVcmpFlag)
{
    if (1 == EnVcmp)
    {
        *pbVcmpFlag = HI_TRUE;
    }
    else
    {
        *pbVcmpFlag = HI_FALSE;
    }
}

HI_S32 HI_DRV_VDEC_DestroyStrmBuf(HI_HANDLE hHandle)
{
    HI_S32 s32Ret;
    s32Ret = VDEC_DestroyStrmBuf(hHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_DestroyStrmBuf err!\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_Chan_AttachStrmBuf(HI_HANDLE hHandle, HI_U32 u32BufSize, HI_HANDLE hDmxVidChn, HI_HANDLE hStrmBuf)
{
    HI_S32 s32Ret;
    s32Ret = VDEC_Chan_AttachStrmBuf(hHandle, u32BufSize, hDmxVidChn, hStrmBuf);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_Chan_AttachStrmBuf err!\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_Chan_DetachStrmBuf(HI_HANDLE hHandle)
{
    HI_S32 s32Ret;
    s32Ret = VDEC_Chan_DetachStrmBuf(hHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_Chan_DetachStrmBuf err!\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_GetCap(VDEC_CAP_S* pstCap)
{
    HI_S32 s32Ret;
    s32Ret = VDEC_GetCap(pstCap);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_GetCap err!\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

VDEC_CHANNEL_S* HI_DRV_VDEC_DRV_GetChan(HI_HANDLE hHandle)
{
    VDEC_CHANNEL_S* stVdecChn = HI_NULL;
    stVdecChn = VDEC_DRV_GetChan(hHandle);

    if (HI_NULL == stVdecChn)
    {
        HI_ERR_VDEC("VDEC_DRV_GetChan err!\n");
        return HI_NULL;
    }

    return stVdecChn;
}
HI_S32 HI_DRV_VDEC_RegisterEventCallback(HI_HANDLE hHandle, EventCallBack fCallback)
{
    HI_HANDLE hHnd = hHandle & 0xff;

    if (hHnd >= HI_VDEC_MAX_INSTANCE)
    {
        HI_ERR_VDEC("bad handle %d!\n", hHnd);
        return HI_FAILURE;
    }

    s_stVdecDrv.astChanEntity[hHnd].eCallBack = fCallback;
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_RegisterDmxHdlCallback(HI_U32  hHandle, GetDmxHdlCallBack fCallback, HI_U32 dmxID)
{
    HI_HANDLE hHnd = hHandle & 0xff;

    if (hHnd >= HI_VDEC_MAX_INSTANCE)
    {
        HI_ERR_VDEC("bad handle %d!\n", hHnd);
        return HI_FAILURE;
    }

    s_stVdecDrv.astChanEntity[hHnd].pstChan->u32DmxID = dmxID;
    s_stVdecDrv.astChanEntity[hHnd].DmxHdlCallBack = fCallback;
    return HI_SUCCESS;
}

#ifdef VFMW_VPSS_BYPASS_EN
static HI_VOID BUFMNG_VDEC_List_Init(VDEC_List_S* pList)
{
    HI_S32 i = 0;
    D_VDEC_CHECK_PTR(pList);

    if (HI_FALSE == pList->bInit)
    {
        HI_S32 s32Ret;

        memset(&pList->stSpecialFrmRec[0], 0, sizeof(VDEC_SPECIAL_FRM_INFO_S)*VDEC_MAX_REMAIN_FRM_NUM);
        for (i = 0; i < VDEC_MAX_REMAIN_FRM_NUM; i++)
        {
            pList->stSpecialFrmRec[i].enbufferNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
            pList->stSpecialFrmRec[i].enhdrNodeStatus    = DRV_VDEC_BUFFER_STATUS_BUTT;
        }
        s32Ret =  BUFMNG_InitSpinLock(&pList->stIrq);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("Error!!!!BUFMNG_InitSpinLock return failed!\n");
        }
        pList->bInit = HI_TRUE;
        pList->s32Num = 0;
    }
    return;
}

static HI_VOID BUFMNG_VDEC_List_Deinit(VDEC_List_S* pList)
{
    HI_U32 i = 0;
    VDEC_SPECIAL_FRM_INFO_S* pSpecialFrmRec;

    D_VDEC_CHECK_PTR(pList);

    for (i = 0; i < VDEC_MAX_REMAIN_FRM_NUM; i++)
    {
        pSpecialFrmRec = &pList->stSpecialFrmRec[i];
        if ((DRV_VDEC_BUFFER_STATUS_SYSALLOC == pSpecialFrmRec->enbufferNodeStatus)
            && (pSpecialFrmRec->frmBufRec.u32StartPhyAddr != 0)
            && (pSpecialFrmRec->frmBufRec.u32StartPhyAddr != 0xffffffff))
        {
            HI_DRV_VDEC_Release(&pSpecialFrmRec->frmBufRec);
        }
        //清空该节点内容，视为空闲节点
        memset(pSpecialFrmRec, 0, sizeof(VDEC_SPECIAL_FRM_INFO_S));
        pSpecialFrmRec->enbufferNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
        pSpecialFrmRec->enhdrNodeStatus   = DRV_VDEC_BUFFER_STATUS_BUTT;

    }
    pList->s32Num = 0;
    pList->bInit = HI_FALSE;
    return ;
}

HI_S32 BUFMNG_VDEC_List_FindNodeIndex(VDEC_List_S* pList, HI_U32 u32TargetPhyAddr, HI_U32* pIndex)
{
    HI_U32 index = 0;
    VDEC_SPECIAL_FRM_INFO_S* pSpecialFrmRec;

    D_VDEC_CHECK_PTR_RET(pList);
    D_VDEC_CHECK_PTR_RET(pIndex);

    for (index = 0; index < VDEC_MAX_REMAIN_FRM_NUM; index++)
    {
        pSpecialFrmRec = &pList->stSpecialFrmRec[index];
       if ( pSpecialFrmRec->frmBufRec.u32StartPhyAddr == u32TargetPhyAddr && u32TargetPhyAddr != 0 && u32TargetPhyAddr != 0xffffffff)
        {
            *pIndex = index;
            break;
        }
    }

    if (index >= VDEC_MAX_REMAIN_FRM_NUM)
    {
        return HI_FAILURE;
    }
    else
    {
        return HI_SUCCESS;
    }
}

HI_S32 BUFMNG_VDEC_List_FindNodeIndexCanRls(VDEC_List_S* pList, HI_U32* pIndex)
{
    HI_U32 index = 0;
    VDEC_SPECIAL_FRM_INFO_S* pSpecialFrmRec;

    D_VDEC_CHECK_PTR_RET(pList);

    for (index = 0; index < VDEC_MAX_REMAIN_FRM_NUM; index++)
    {
        pSpecialFrmRec = &pList->stSpecialFrmRec[index];
        if ( pSpecialFrmRec->bCanRls == HI_TRUE )
        {
            *pIndex = index;
            break;
        }
    }

    if (index >= VDEC_MAX_REMAIN_FRM_NUM)
    {
        return HI_FAILURE;
    }
    else
    {
        return HI_SUCCESS;
    }
}

static HI_S32 BUFMNG_VDEC_List_Add(VDEC_List_S* pList, VDEC_SPECIAL_FRM_INFO_S* pSpecialFrmRec)
{

    HI_U32 index;
    VDEC_SPECIAL_FRM_INFO_S* pListRec;
    D_VDEC_CHECK_PTR_RET(pList);
    D_VDEC_CHECK_PTR_RET(pSpecialFrmRec);

    //BUFMNG_SpinLockIRQ(&pList->stIrq); move to outside
    for (index = 0; index < VDEC_MAX_REMAIN_FRM_NUM; index++)
    {
        pListRec = &pList->stSpecialFrmRec[index];
        if (pListRec->frmBufRec.u32StartPhyAddr == 0)
        {
            memcpy(pListRec, pSpecialFrmRec, sizeof(VDEC_SPECIAL_FRM_INFO_S));
            pList->s32Num++;
            if (pList->s32Num > VDEC_MAX_REMAIN_FRM_NUM)
            {
                HI_ERR_VDEC("Remain Frame num = %d larger than Max(%d),force to be %d\n", pList->s32Num, VDEC_MAX_REMAIN_FRM_NUM, VDEC_MAX_REMAIN_FRM_NUM);
                pList->s32Num = VDEC_MAX_REMAIN_FRM_NUM;
            }
            break;
        }
    }
    //BUFMNG_SpinUnLockIRQ(&pList->stIrq);

    if (index >= VDEC_MAX_REMAIN_FRM_NUM)
    {
        HI_ERR_VDEC("can't find the idle node of the global frame list! Remain Frame num = %d\n", pList->s32Num);
    }
    return HI_SUCCESS;
}

static HI_S32 BUFMNG_VDEC_List_Del(VDEC_List_S* pList, HI_U32 u32Index)
{
    VDEC_SPECIAL_FRM_INFO_S* pSpecialFrmRec;

    D_VDEC_CHECK_PTR_RET(pList);

    if (u32Index >= VDEC_MAX_REMAIN_FRM_NUM)
    {
        HI_ERR_VDEC("want to delete index(%d) >= %d(max), error!!!\n", u32Index, VDEC_MAX_REMAIN_FRM_NUM);
        return HI_FAILURE;
    }

    BUFMNG_SpinLockIRQ(&pList->stIrq);
    pSpecialFrmRec = &pList->stSpecialFrmRec[u32Index];

    //清空该节点内容，视为空闲节点
    memset(pSpecialFrmRec, 0, sizeof(VDEC_SPECIAL_FRM_INFO_S));
    pSpecialFrmRec->enbufferNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
    pSpecialFrmRec->enhdrNodeStatus    = DRV_VDEC_BUFFER_STATUS_BUTT;
    pList->s32Num--;
    if (pList->s32Num < 0)
    {
        HI_ERR_VDEC("the vdec global remain frame < 0 ??!  force to be 0!\n");
        pList->s32Num = 0;
    }
    BUFMNG_SpinUnLockIRQ(&pList->stIrq);
    return HI_SUCCESS;
}

/******************************************************/
/***         内部全局还帧接口                       ***/
/** 该函数是内部vdec线程调用                        ***/
/******************************************************/
HI_S32 VDEC_DRV_GlobalRelFrmInter(HI_U32 u32Index)
{
    HI_S32 s32Ret = 0;
    VDEC_SPECIAL_FRM_INFO_S* pSpecialFrmRec;

    if (u32Index >= VDEC_MAX_REMAIN_FRM_NUM)
    {
        HI_ERR_VDEC("want to release index(%d) >= %d(max), error!!!\n", u32Index, VDEC_MAX_REMAIN_FRM_NUM);
        return HI_FAILURE;
    }

    /* should del lock before release the memory*/
    pSpecialFrmRec = &s_stVdecDrv.stRemainFrmList.stSpecialFrmRec[u32Index];

    if (pSpecialFrmRec->frmBufRec.u32StartPhyAddr == 0 || pSpecialFrmRec->frmBufRec.u32StartPhyAddr == 0xffffffff)
    {
        HI_INFO_VDEC("phy is invalid!\n");
        return HI_FAILURE;
    }
#ifdef HI_TEE_SUPPORT
    if (HI_TRUE == pSpecialFrmRec->bIsSecFrm)
    {
        if (DRV_VDEC_BUFFER_STATUS_SYSALLOC == pSpecialFrmRec->enbufferNodeStatus)   /*动态申请*/
        {
            HI_DRV_VDEC_Release_TVP(&pSpecialFrmRec->frmBufRec);
        }
    }
    else
#endif
    {
        if (DRV_VDEC_BUFFER_STATUS_SYSALLOC == pSpecialFrmRec->enbufferNodeStatus)   /*动态申请*/
        {
            HI_DRV_VDEC_Release(&pSpecialFrmRec->frmBufRec);
        }
        else
        {
            /*预分配*/
            VDEC_Chan_ReleasePreMem(&pSpecialFrmRec->frmBufRec);
        }
    }

    if (DRV_VDEC_BUFFER_STATUS_SYSALLOC == pSpecialFrmRec->enhdrNodeStatus)  /*动态申请*/
    {
        HI_DRV_VDEC_UnmapAndRelease(&pSpecialFrmRec->hdrBufRec);
    }
    else if (DRV_VDEC_BUFFER_STATUS_PREALLOC == pSpecialFrmRec->enhdrNodeStatus)
    {
        VDEC_Chan_ReleasePreMem(&pSpecialFrmRec->hdrBufRec);
    }

    s32Ret = BUFMNG_VDEC_List_Del(&s_stVdecDrv.stRemainFrmList, u32Index);
    return s32Ret;
}

/******************************************************/
/***             全局还帧接口                       ***/
/** 此处输入的 pstFrame 应对应metadata强转中的信息  ***/
/******************************************************/
HI_S32 HI_DRV_VDEC_GlobalRelFrm(HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Index = 0;
    VDEC_SPECIAL_FRM_INFO_S* pSpecialFrmInfo;
    D_VDEC_CHECK_PTR_RET(pstFrame);

    s32Ret = BUFMNG_VDEC_List_FindNodeIndex(&s_stVdecDrv.stRemainFrmList, pstFrame->stBufAddr[0].u32PhyAddr_YHead, &u32Index);
    if ( HI_SUCCESS != s32Ret )
    {
        HI_INFO_VDEC("Can't find the remain Frm node (Phy = 0x%x,bSecureMem = %d), Remain num = %d\n",
                     pstFrame->stBufAddr[0].u32PhyAddr_Y, pstFrame->bSecure, s_stVdecDrv.stRemainFrmList.s32Num);
        return HI_FAILURE;
    }

    pSpecialFrmInfo = &s_stVdecDrv.stRemainFrmList.stSpecialFrmRec[u32Index];
#ifdef HI_TEE_SUPPORT
    if (HI_TRUE == pSpecialFrmInfo->bIsSecFrm)
    {
        if (DRV_VDEC_BUFFER_STATUS_SYSALLOC == pSpecialFrmInfo->enbufferNodeStatus)   /*动态申请*/
        {
            HI_DRV_VDEC_Release_TVP(&pSpecialFrmInfo->frmBufRec);
        }
    }
    else
#endif
    {
        if (DRV_VDEC_BUFFER_STATUS_SYSALLOC == pSpecialFrmInfo->enbufferNodeStatus)   /*动态申请*/
        {
            HI_DRV_VDEC_Release(&pSpecialFrmInfo->frmBufRec);
        }
        else
        {
            /*预分配*/
            VDEC_Chan_ReleasePreMem(&pSpecialFrmInfo->frmBufRec);
        }
    }

    if (DRV_VDEC_BUFFER_STATUS_SYSALLOC == pSpecialFrmInfo->enhdrNodeStatus)     /*动态申请*/
    {
        HI_DRV_VDEC_UnmapAndRelease(&pSpecialFrmInfo->hdrBufRec);
    }
    else if (DRV_VDEC_BUFFER_STATUS_PREALLOC == pSpecialFrmInfo->enhdrNodeStatus)
    {
        /*预分配*/
        VDEC_Chan_ReleasePreMem(&pSpecialFrmInfo->hdrBufRec);
    }

    s32Ret = BUFMNG_VDEC_List_Del(&s_stVdecDrv.stRemainFrmList, u32Index);

    return s32Ret;
}



/******************************************************/
/***            查询是否bypass                      ***/
/**                                                 ***/
/******************************************************/
HI_S32 HI_DRV_VDEC_GetVideoBypassInfo(HI_HANDLE hHandle, HI_BOOL* pbVideoBypass)
{
    HI_DRV_VPSS_BYPASSATTR_S stVpssBypassInfo;
    HI_S32 s32Ret = HI_FAILURE;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    VDEC_VPSSCHANNEL_S* pstVpssChan = HI_NULL;

    if (HI_INVALID_HANDLE == hHandle)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    D_VDEC_CHECK_PTR_RET(pbVideoBypass);

    /* Check and get pstChan pointer */
    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        HI_WARN_VDEC("Chan %d not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    if (HI_UNF_VCODEC_TYPE_VC1 == pstChan->stCurCfg.enType)
    {
        // if is vc1, no bypass
        *pbVideoBypass = 0;
    }
    else
    {
        pstVpssChan = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan);

        memset(&stVpssBypassInfo, 0 , sizeof(HI_DRV_VPSS_BYPASSATTR_S));

        stVpssBypassInfo.u32InputWidth  = pstVpssChan->u32InputWidth;
        stVpssBypassInfo.u32InputHeight = pstVpssChan->u32InputHeight;
        stVpssBypassInfo.u32InputFrameRate = 0;
        stVpssBypassInfo.enInputPixFormat  = 0;

        if (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)
        {
            s32Ret = s_stVdecDrv.pVpssFunc->pfnVpssSendCommand(pstVpssChan->hVpss,
                     HI_DRV_VPSS_USER_COMMAND_CHECKBYPASS, &(stVpssBypassInfo));
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VDEC("VPSS Check bypass failed!return 0x%x\n", s32Ret);
                return HI_FAILURE;
            }

            *pbVideoBypass = stVpssBypassInfo.bVpssBypass;
        }
        else
        {
            HI_ERR_VDEC("pVpssFunc->pfnVpssSendCommand= NULL!\n");
        }
    }

    return s32Ret;
}


/******************************************************/
/***            获取special帧信息                   ***/
/** 提供给 vdec_ctrl 查看proc信息的函数             ***/
/******************************************************/
HI_S32 VDEC_DRV_GetSpecialFrmInfo(VDEC_SPECIAL_FRM_PROC_S* pstSpecialFrmInfo)
{
    HI_U32 u32Index = 0;
    HI_U32 SpecialFrmCnt = 0;
    VDEC_SPECIAL_FRM_INFO_S* pSpecialFrmInfo;

    D_VDEC_CHECK_PTR_RET(pstSpecialFrmInfo);

    pstSpecialFrmInfo->u32SpecialFrmNum = s_stVdecDrv.stRemainFrmList.s32Num;

    if (pstSpecialFrmInfo->u32SpecialFrmNum != 0)
    {
        for (u32Index = 0; u32Index < VDEC_MAX_REMAIN_FRM_NUM; u32Index++)
        {
            pSpecialFrmInfo = &s_stVdecDrv.stRemainFrmList.stSpecialFrmRec[u32Index];
            if (pSpecialFrmInfo->frmBufRec.u32StartPhyAddr != 0)
            {
                memcpy(&pstSpecialFrmInfo->frmBufRec[SpecialFrmCnt], &pSpecialFrmInfo->frmBufRec, sizeof(VDEC_BUFFER_S));
                SpecialFrmCnt++;
            }
        }
    }

    if (SpecialFrmCnt != pstSpecialFrmInfo->u32SpecialFrmNum)
    {
        HI_ERR_VDEC("the SpecialFrmCnt not the same! list record = %d, real record = %d\n", pstSpecialFrmInfo->u32SpecialFrmNum , SpecialFrmCnt);
    }
    return HI_SUCCESS;
}

/******************************************************/
/***            初始化specialFrame 链表             ***/
/** 插ko的时候初始化                                ***/
/******************************************************/
HI_VOID VDEC_DRV_SpecialFrmListInit(HI_VOID)
{
    /* Init the remain buffer list*/
    BUFMNG_VDEC_List_Init(&s_stVdecDrv.stRemainFrmList);

    return ;
}

/******************************************************/
/***          去初始化specialFrame 链表             ***/
/** 卸载ko的时候去初始化                            ***/
/******************************************************/
HI_VOID VDEC_DRV_SpecialFrmListDeinit(HI_VOID)
{
    /* deinit the remain buffer list*/
    BUFMNG_VDEC_List_Deinit(&s_stVdecDrv.stRemainFrmList);
    return ;
}
#endif

HI_VOID HI_DRV_VDEC_ReportEsRels(HI_HANDLE hDmxVidChn)
{
    HI_S32 s32Ret = VDEC_ERR;
    HI_HANDLE hVdec;
    VDEC_CHANNEL_S* pstChan = HI_NULL;

    if (hDmxVidChn == HI_INVALID_HANDLE)
    {
        return;
    }

    for (hVdec = 0; hVdec < HI_VDEC_MAX_INSTANCE_NEW; hVdec++)
    {
        VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hVdec]);

        if (s32Ret != HI_SUCCESS)
        {
            continue;
        }

        pstChan = s_stVdecDrv.astChanEntity[hVdec].pstChan;
        if (pstChan == HI_NULL)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hVdec]);
            continue;
        }

        if (pstChan->hDmxVidChn == hDmxVidChn)
        {
            if (pstChan->hChan != HI_INVALID_HANDLE)
            {
                s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_REPORT_DEMUX_RELEASE, HI_NULL);

                if (s32Ret != VDEC_OK)
                {
                    HI_ERR_VDEC("Chan %d VDEC_CID_REPORT_DEMUX_RELEASE err!\n", pstChan->hChan);
                }
            }

            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hVdec]);
            break;
        }

        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hVdec]);
    }

    if (hVdec >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_WARN_VDEC("hDmxVidChn = %x ERROR!\n", hDmxVidChn);
    }
}


module_param(RefFrameNum, int, S_IRUGO);
module_param(DispFrameNum, int, S_IRUGO);
module_param(EnVcmp, int, S_IRUGO);
module_param(En2d, int, S_IRUGO);

EXPORT_SYMBOL(HI_DRV_VDEC_GetChanStatusInfo);
EXPORT_SYMBOL(HI_DRV_VDEC_Open);
EXPORT_SYMBOL(HI_DRV_VDEC_ResetChan);
EXPORT_SYMBOL(HI_DRV_VDEC_GetEsBuf);
EXPORT_SYMBOL(HI_DRV_VDEC_PutEsBuf);
EXPORT_SYMBOL(HI_DRV_VDEC_SetEosFlag);
EXPORT_SYMBOL(HI_DRV_VDEC_AllocChan);
EXPORT_SYMBOL(HI_DRV_VDEC_SetChanAttr);
EXPORT_SYMBOL(HI_DRV_VDEC_DestroyPort);
EXPORT_SYMBOL(HI_DRV_VDEC_GetPortParam);
EXPORT_SYMBOL(HI_DRV_VDEC_CreatePort);
EXPORT_SYMBOL(HI_DRV_VDEC_Chan_RecvVpssFrmBuf);
EXPORT_SYMBOL(HI_DRV_VDEC_GetChanStreamInfo);
EXPORT_SYMBOL(HI_DRV_VDEC_ChanStop);
EXPORT_SYMBOL(HI_DRV_VDEC_ChanStart);
EXPORT_SYMBOL(HI_DRV_VDEC_FreeChan);
EXPORT_SYMBOL(HI_DRV_VDEC_RecvFrmBuf);
EXPORT_SYMBOL(HI_DRV_VDEC_GetChanAttr);
EXPORT_SYMBOL(HI_DRV_VDEC_SetPortType);
EXPORT_SYMBOL(HI_DRV_VDEC_EnablePort);
EXPORT_SYMBOL(HI_DRV_VDEC_Close);
EXPORT_SYMBOL(HI_DRV_VDEC_CreateStrmBuf);
EXPORT_SYMBOL(HI_DRV_VDEC_ChanBufferInit);
EXPORT_SYMBOL(HI_DRV_VDEC_ChanBufferDeInit);
EXPORT_SYMBOL(HI_DRV_VDEC_DestroyStrmBuf);
EXPORT_SYMBOL(HI_DRV_VDEC_Chan_AttachStrmBuf);
EXPORT_SYMBOL(HI_DRV_VDEC_Chan_DetachStrmBuf);
EXPORT_SYMBOL(HI_DRV_VDEC_GetCap);
EXPORT_SYMBOL(HI_DRV_VDEC_DRV_GetChan);
EXPORT_SYMBOL(HI_DRV_VDEC_RegisterEventCallback);
EXPORT_SYMBOL(HI_DRV_VDEC_RegisterDmxHdlCallback);
EXPORT_SYMBOL(VDEC_Chan_FindPreMem);
EXPORT_SYMBOL(VDEC_Chan_ReleasePreMem);
EXPORT_SYMBOL(HI_DRV_VDEC_SetOmxCallBacks);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
