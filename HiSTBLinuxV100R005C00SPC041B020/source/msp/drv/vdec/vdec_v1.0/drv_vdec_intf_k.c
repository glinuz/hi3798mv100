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
#include <mach/hardware.h>

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
//#include "vfmw_ext.h"
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

#ifndef HI_VDEC_DFS_SUPPORT
#define HI_VDEC_DFS_SUPPORT (0)
#endif

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


/*************************** Structure Definition ****************************/

/* Channel entity */
typedef struct tagVDEC_CHAN_ENTITY_S
{
    VDEC_CHANNEL_S     *pstChan;        /* Channel structure pointer for vfmw*/
    VDEC_VPSSCHANNEL_S  stVpssChan;     /* vpss Channel structure */
    HI_U32              u32File;        /* File handle */
    HI_BOOL             bUsed;          /* Busy or free */
    atomic_t            atmUseCnt;      /* Channel use count, support multi user */
    atomic_t            atmRlsFlag;     /* Channel release flag */
    wait_queue_head_t   stRlsQue;       /* Release queue */
    EventCallBack       eCallBack;      /*for opentv5*/
    GetDmxHdlCallBack   DmxHdlCallBack; /*for opentv5*/
    HI_U32              u32DynamicFsEn; /*Dynamic frame store flag*///l00273086
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
    VDEC_REGISTER_PARAM_S  *pstProcParam;   /* VDEC Proc functions */
    DEMUX_EXPORT_FUNC_S    *pDmxFunc;       /* Demux extenal functions */
    VFMW_EXPORT_FUNC_S     *pVfmwFunc;      /* VFMW extenal functions */
    VPSS_EXPORT_FUNC_S      *pVpssFunc;       /*VPSS external functions*/
    FN_VDEC_Watermark       pfnWatermark;   /* Watermark function */
    VDEC_EXPORT_FUNC_S      stExtFunc;      /* VDEC extenal functions */
    struct task_struct     *pVdecTask; //l00273086
} VDEC_GLOBAL_PARAM_S;

/***************************** Global Definition *****************************/

/* VDEC全局屏蔽控制字 bit0: DynamicFs Mask Ctrl; other: reserve*/
HI_U32 MaskCtrlWord = 0;

/*结束码大全*/
HI_CHAR g_EopTab_H264[]={0x00, 0x00, 0x01, 0x1E, 0x48, 0x53, 0x50, 0x49, 0x43, 0x45, 0x4E, 0x44, 0x00, 0x00, 0x01, 0x00};
HI_CHAR g_EopTab_MPEG4[]={0x00, 0x00, 0x01, 0x1E, 0x48, 0x53, 0x50, 0x49, 0x43, 0x45, 0x4E, 0x44, 0x00, 0x00, 0x01};
HI_CHAR g_EopTab_H265[]={0x00, 0x00, 0x01, 0x60, 0x00, 0x48, 0x53, 0x50, 0x49, 0x43, 0x45, 0x4E, 0x44, 0x00, 0x00, 0x01};

#ifdef Hi3716MV410
HI_U32 g_CHAN_FRAME_RATE[HI_VDEC_MAX_INSTANCE_NEW] = {0};

typedef enum
{
    NORMAL_CHAN,
    ALLOC_FRAME_BUF_TO_CHAN,
    DESTORY_CHAN
} VDEC_CHANNEL_CONTROL_STATE_E;

static VDEC_CHANNEL_CONTROL_STATE_E g_ChanControlState[64];

#endif

/***************************** Static Definition *****************************/
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
MMZ_BUFFER_S g_stVDHMMZ = {0, 0, 0};
HI_BOOL g_bVdecPreVDHMMZUsed = HI_FALSE;
HI_U32 g_VdecPreVDHMMZUsedSize = 0;
VDEC_PREMMZ_NODE_S st_VdecChanPreUsedMMZInfo[HI_VDEC_MAX_PRENODE_NUM];
HI_U32 g_VdecPreMMZNodeNum = 1;
#else
#define HI_VDEC_MAX_PRENODE_NUM 100
MMZ_BUFFER_S g_stVDHMMZ = {0, 0, 0};
HI_BOOL g_bVdecPreVDHMMZUsed = HI_FALSE;
HI_U32 g_VdecPreVDHMMZUsedSize = 0;
VDEC_PREMMZ_NODE_S st_VdecChanPreUsedMMZInfo[HI_VDEC_MAX_PRENODE_NUM];
HI_U32 g_VdecPreMMZNodeNum = 0;
#endif


#if (1 == PRE_ALLOC_VDEC_SCD_MMZ)
MMZ_BUFFER_S g_stSCDMMZ = {0, 0, 0};
HI_BOOL g_bVdecPreSCDMMZUsed = HI_FALSE;
#endif

#if (1 == PRE_ALLOC_VDEC_ESBUF_MMZ)
#define ESBUF_MMZ_SIZE (20*1024*1024)
MMZ_BUFFER_S g_stESBUFMMZ = {0, 0, 0};
HI_BOOL g_bVdecPreESBUFMMZUsed = HI_FALSE;
#endif

static HI_S32   VDEC_RegChanProc(HI_S32 s32Num);
static HI_VOID  VDEC_UnRegChanProc(HI_S32 s32Num);
static HI_S32   VDEC_Chan_VpssRecvFrmBuf(VPSS_HANDLE hVpss, HI_DRV_VIDEO_FRAME_S *pstFrm);
static HI_S32   VDEC_Chan_VpssRlsFrmBuf(VPSS_HANDLE hVpss, HI_DRV_VIDEO_FRAME_S  *pstFrm);

static HI_VOID VDEC_ConvertFrm(HI_UNF_VCODEC_TYPE_E enType, VDEC_CHANNEL_S *pstChan,
                               IMAGE *pstImage, HI_DRV_VIDEO_FRAME_S *pstFrame);
static HI_S32 VDEC_FindVdecHandleByVpssHandle(VPSS_HANDLE hVpss, HI_HANDLE *hVdec);
static HI_U32 VDEC_ConverColorSpace(HI_U32 u32ColorSpace);
static HI_UNF_VIDEO_FRAME_PACKING_TYPE_E VDEC_ConverFrameType(HI_DRV_FRAME_TYPE_E  eFrmType);
static HI_VOID VDEC_GetMemConfig(HI_U32 *pu32ScdBufSize, HI_U32 *pu32VdhBufSize);

#ifdef Hi3716MV410
static HI_S32 VDEC_ReleaseAllFrameNode(HI_S32 chanID);
static HI_S32 VDEC_ResetChanFrmBuf(HI_S32 chanID);

static HI_S32 VDEC_AllocPREMem(HI_S32 chanID, HI_S32 frameNum);
static HI_S32 VDEC_AllocMMZMem(HI_S32 chanID, HI_S32 frameNum);

static HI_S32 VDEC_AddFrmBufRec(HI_S32 chanID, MMZ_BUFFER_S *frmBufRec);
static HI_S32 VDEC_DelFrmBufRec(HI_S32 chanID, MMZ_BUFFER_S *frmBufRec);

static HI_S32 VDEC_RefreshConfiguredFrameBuf(HI_S32 chanID, HI_U32 phyAddr);

static HI_S32 VDEC_SetChanControlState(HI_S32 chanID, VDEC_CHANNEL_CONTROL_STATE_E chanControlState);

static HI_S32 VDEC_ReleaseMemForDecoder(HI_HANDLE hHandle);
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
        .pfnVDEC_Suspend     = (HI_VOID *)VDEC_DRV_Suspend,
        .pfnVDEC_Resume      = (HI_VOID *)VDEC_DRV_Resume,
        .pfnVOAcqFrame       = (HI_VOID *)HI_VDEC_Chan_VOAcqFrame,
        .pfnVORlsFrame       = (HI_VOID *)HI_VDEC_Chan_VORlsFrame,
        .pfnVOSendWinInfo    = (HI_VOID *)HI_VDEC_Chan_VOChangeWinInfo,
    },
    .pVdecTask = HI_NULL //l00273086
};

/*********************************** Code ************************************/

#if (VDEC_DEBUG == 1)
static HI_VOID VDEC_PrintImage(IMAGE *pstImg)
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

static HI_VOID VDEC_PrintFrmInfo(HI_UNF_VIDEO_FRAME_INFO_S *pstFrame)
{
    HI_FATAL_VDEC("<0>u32Height = %d\n", pstFrame->u32Height);
    HI_FATAL_VDEC("<0>u32Width = %d\n", pstFrame->u32Width);
    HI_FATAL_VDEC("<0>u32DisplayWidth = %d\n", pstFrame->u32DisplayWidth);
    HI_FATAL_VDEC("<0>u32DisplayHeight = %d\n", pstFrame->u32DisplayHeight);
    HI_FATAL_VDEC("<0>u32DisplayCenterX = %d\n", pstFrame->u32DisplayCenterX);
    HI_FATAL_VDEC("<0>u32DisplayCenterY = %d\n", pstFrame->u32DisplayCenterY);
}
#endif

static inline HI_S32  VDEC_CHAN_TRY_USE_DOWN_HELP(VDEC_CHAN_ENTITY_S *pstEnt)
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

static inline HI_S32  VDEC_CHAN_USE_UP_HELP(VDEC_CHAN_ENTITY_S *pstEnt)
{
    if (atomic_dec_return(&pstEnt->atmUseCnt) < 0)
    {
        HI_ERR_VDEC("Use unlock err\n");
        //while (1)
        //{}
    }

    return HI_SUCCESS;
}

static inline HI_S32  VDEC_CHAN_RLS_DOWN_HELP(VDEC_CHAN_ENTITY_S *pstEnt, HI_U32 time)
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

static inline HI_S32  VDEC_CHAN_RLS_UP_HELP(VDEC_CHAN_ENTITY_S *pstEnt)
{
    if (atomic_dec_return(&pstEnt->atmRlsFlag) < 0)
    {
        //while (1)
        //{}
    }

    return HI_SUCCESS;
}

/* 初始化互斥锁*/
HI_S32  VDEC_InitSpinLock( VDEC_PORT_FRAME_LIST_LOCK_S *pIntrMutex )
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
HI_S32 VDEC_SpinLockIRQ( VDEC_PORT_FRAME_LIST_LOCK_S *pIntrMutex )
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
HI_S32 VDEC_SpinUnLockIRQ( VDEC_PORT_FRAME_LIST_LOCK_S *pIntrMutex )
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

#ifdef Hi3716MV410
HI_S32  VDEC_InitFrmBufRecSpinLock(CHAN_FRAME_BUF_RECORD_LOCK_S *pIntrMutex)
{
    spin_lock_init(&pIntrMutex->irq_lock);
    pIntrMutex->isInit = HI_TRUE;
    return HI_SUCCESS;
}

/* 中断互斥加锁(关中断且加锁)*/
HI_S32 VDEC_FrmBufRecSpinLockIRQ(CHAN_FRAME_BUF_RECORD_LOCK_S *pIntrMutex)
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
HI_S32 VDEC_FrmBufRecSpinUnLockIRQ(CHAN_FRAME_BUF_RECORD_LOCK_S *pIntrMutex)
{
    if (pIntrMutex->isInit == HI_TRUE)
    {
        spin_unlock_irqrestore(&pIntrMutex->irq_lock, pIntrMutex->irq_lockflags);
    }

    return HI_SUCCESS;
}

HI_S32 VDEC_InitChanControlStateLock(CHAN_CONTROL_STATE_LOCK_S *pIntrMutex)
{
    spin_lock_init(&pIntrMutex->irq_lock);
    pIntrMutex->isInit = HI_TRUE;
    return HI_SUCCESS;
}

HI_S32 VDEC_ChanControlStateSpinLockIRQ(CHAN_CONTROL_STATE_LOCK_S *pIntrMutex)
{
    if (pIntrMutex->isInit == HI_FALSE)
    {
        spin_lock_init(&pIntrMutex->irq_lock);
        pIntrMutex->isInit = HI_TRUE;
    }

    spin_lock_irqsave(&pIntrMutex->irq_lock, pIntrMutex->irq_lockflags);

    return HI_SUCCESS;
}

HI_S32 VDEC_ChanControlStateSpinUnlockIRQ(CHAN_CONTROL_STATE_LOCK_S *pIntrMutex)
{
    if (pIntrMutex->isInit == HI_TRUE)
    {
        spin_unlock_irqrestore(&pIntrMutex->irq_lock, pIntrMutex->irq_lockflags);
    }

    return HI_SUCCESS;
}
#endif

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
HI_S32 BUFMNG_SpinLockIRQ( BUFMNG_VPSS_IRQ_LOCK_S *pIntrMutex )
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
HI_S32 BUFMNG_SpinUnLockIRQ( BUFMNG_VPSS_IRQ_LOCK_S *pIntrMutex )
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
static HI_S32 BUFMNG_VPSS_Init(BUFMNG_VPSS_INST_S *pstBufVpssInst)
{
    HI_S32 i;
    BUFMNG_VPSS_NODE_S *pstBufNode;
    BUFMNG_VPSS_NODE_S *pstTarget;
    struct list_head *pos, *n;
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

            s32Ret = HI_DRV_MMZ_AllocAndMap("VDEC_VPSSBuf", "VDEC", pstBufVpssInst->u32FrmBufSize, 0, &pstBufNode->MMZBuf_frm);
            s32Ret |= HI_DRV_MMZ_AllocAndMap("VDEC_VPSSBuf_Meta", "VDEC", pstBufVpssInst->u32MetaBufSize, 0, &pstBufNode->MMZBuf_meta);

            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VDEC("BUFMNG_VPSS_Init Alloc MMZ fail:0x%x.\n", s32Ret);
                list_for_each_safe(pos, n, &(pstBufVpssInst->stVpssBufAvailableList))
                {
                    pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);
                    HI_DRV_MMZ_UnmapAndRelease(&(pstTarget->MMZBuf_frm));
                    HI_DRV_MMZ_UnmapAndRelease(&(pstTarget->MMZBuf_meta));//DRM: meta内存为非安内存 c00277632
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
static HI_S32 BUFMNG_VPSS_DeInit(BUFMNG_VPSS_INST_S *pstBufVpssInst)
{
    HI_S32 s32Ret = HI_SUCCESS;
    struct list_head *pos, *n;
    BUFMNG_VPSS_NODE_S *pstTarget;

    if (HI_DRV_VDEC_BUF_VDEC_ALLOC_MANAGE == pstBufVpssInst->enFrameBuffer)
    {
        list_for_each_safe(pos, n, &(pstBufVpssInst->stVpssBufAvailableList))
        {
            pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);
            HI_DRV_MMZ_UnmapAndRelease(&(pstTarget->MMZBuf_frm));
            HI_DRV_MMZ_UnmapAndRelease(&(pstTarget->MMZBuf_meta));
            list_del_init(pos);
            vfree(pstTarget);
        }

        list_for_each_safe(pos, n, &(pstBufVpssInst->stVpssBufUnAvailableList))
        {
            pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);
            HI_DRV_MMZ_UnmapAndRelease(&(pstTarget->MMZBuf_frm));
            HI_DRV_MMZ_UnmapAndRelease(&(pstTarget->MMZBuf_meta));
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
        pstBufVpssInst->u32MetaBufSize       = 0;
        pstBufVpssInst->enExtBufferState    = VDEC_EXTBUFFER_STATE_START;
        BUFMNG_SpinUnLockIRQ(&pstBufVpssInst->stUnAvailableListLock);
    }

    return s32Ret;
}

static HI_S32 BUFMNG_VPSS_Reset(BUFMNG_VPSS_INST_S *pstBufVpssInst)
{
    //return HI_SUCCESS;
#if 1
    BUFMNG_VPSS_NODE_S *pstTarget;
    struct list_head *pos, *n;
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
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;

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

static HI_S32 BUFMNG_VPSS_RecBuffer(HI_HANDLE hVpss, HI_HANDLE hPort, BUF_VPSS_S *pstMMZ_Buffer, HI_U32 *pu32Stride)
{
    HI_S32 s32Ret;
    HI_S32 i, j;
    struct list_head *pos;
    BUFMNG_VPSS_NODE_S *pstTarget;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;

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
    if (pstVpssChan->stPort[j].stBufVpssInst.u32FrmBufSize != pstMMZ_Buffer->MMZBuf_frm.u32Size)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        HI_WARN_VDEC("the frame size is change,before change FrmBuf:%d vpss want FrmBuf:%d \n", pstVpssChan->stPort[j].stBufVpssInst.u32FrmBufSize, \
                     pstMMZ_Buffer->MMZBuf_frm.u32Size);
        return HI_FAILURE;
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

    memcpy(&pstMMZ_Buffer->MMZBuf_frm, &pstTarget->MMZBuf_frm, sizeof(MMZ_BUFFER_S));

    //当 地址为-1时，表示上层未分配metabuf,但还是把结构体传给vpss，给vpss做判断
    if (pstTarget->MMZBuf_meta.u32StartPhyAddr == 0xffffffff)
    {
        HI_WARN_VDEC("meta buf not alloc!\n");
    }

    memcpy(&pstMMZ_Buffer->MMZBuf_meta, &pstTarget->MMZBuf_meta, sizeof(MMZ_BUFFER_S));

    *pu32Stride = pstTarget->u32Stride;
    BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
    return HI_SUCCESS;
}
static HI_S32 BUFMNG_VPSS_RelBuffer(HI_HANDLE hVpss, HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstImage)
{
    HI_S32 s32Ret;
    HI_S32 i, j;
    struct list_head *pos, *n;
    BUFMNG_VPSS_NODE_S *pstTarget;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;

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

            if (pstImage->stBufAddr[0].u32PhyAddr_Y == pstTarget->MMZBuf_frm.u32StartPhyAddr) //C00277632
            {
                //pstImage->stBufAddr[0].u32PhyAddr_Y,pstTarget->stMMZBuf.u32StartPhyAddr);
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
    return HI_SUCCESS;
}
static HI_S32 VDEC_VpssNewImageEvent(HI_HANDLE hVpss, HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstImage)
{
    HI_S32 s32Ret;
    HI_S32 i, j;
    struct list_head *pos, *n;
    BUFMNG_VPSS_NODE_S *pstTarget;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;

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

            if (pstTarget->MMZBuf_frm.u32StartPhyAddr == pstImage->stBufAddr[0].u32PhyAddr_Y)
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
static HI_S32 VDEC_FindVdecHandleByVpssHandle(VPSS_HANDLE hVpss, HI_HANDLE *phVdec)
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

HI_S32 VDEC_FindChannelHandleByInstance(HI_HANDLE hHandle, VDEC_CHANNEL_S **pstChan)
{
    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("null point pstChan\n");
        return HI_FAILURE;
    }

    *pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    return HI_SUCCESS;
}


static HI_S32 VDEC_FindVdecHandleByPortHandle(HI_HANDLE hPort, HI_HANDLE *phVdec)
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

HI_S32 VDEC_FindVpssHandleByVdecHandle(HI_HANDLE hVdec, HI_HANDLE *phVpss)
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

HI_S32 VDEC_FindVdecHandleByESBufferHandle(HI_HANDLE hSteeamBufferHandle, HI_HANDLE *phVdec)
{
    HI_U32 i;
    VDEC_CHANNEL_S *pstChan = HI_NULL;

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
static HI_S32 VDEC_CheckVpssPortOutFrameStatus(HI_HANDLE hVdec)
{
    HI_S32 i = 0;
    HI_HANDLE hMASTER = HI_INVALID_HANDLE;
    HI_HANDLE hSLAVE = HI_INVALID_HANDLE;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;
    pstVpssChan = &(s_stVdecDrv.astChanEntity[hVdec].stVpssChan);

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

    if (hMASTER != HI_INVALID_HANDLE || hSLAVE != HI_INVALID_HANDLE)
    {
        HI_DRV_VPSS_PORT_AVAILABLE_S stCanGetFrm;

        if (hMASTER != HI_INVALID_HANDLE)
        {
            stCanGetFrm.hPort = hMASTER;
            stCanGetFrm.bAvailable = HI_FALSE;
            s_stVdecDrv.pVpssFunc->pfnVpssSendCommand(pstVpssChan->hVpss,
                    HI_DRV_VPSS_USER_COMMAND_CHECKAVAILABLE, &(stCanGetFrm));

            if (stCanGetFrm.bAvailable == HI_FALSE)
            {
                return HI_FAILURE;
            }
        }

        if (hSLAVE != HI_INVALID_HANDLE)
        {
            stCanGetFrm.hPort = hSLAVE;
            stCanGetFrm.bAvailable = HI_FALSE;
            s_stVdecDrv.pVpssFunc->pfnVpssSendCommand(pstVpssChan->hVpss,
                    HI_DRV_VPSS_USER_COMMAND_CHECKAVAILABLE, &(stCanGetFrm));

            if (stCanGetFrm.bAvailable == HI_FALSE)
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
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;
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
static HI_S32 VDEC_Chan_RecvVdecPortListFrame(HI_HANDLE hVdec, HI_DRV_VIDEO_FRAME_PACKAGE_S *pstFrm)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 i = 0;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;
    VDEC_PORT_FRAME_LIST_LOCK_S *pListLock = HI_NULL;
    VDEC_PORT_FRAME_LIST_NODE_S *pstListNode = HI_NULL;

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
                    memcpy((void *) & (pstFrm->stFrame[pstFrm->u32FrmNum].stFrameVideo), &(pstListNode->stPortOutFrame), sizeof(HI_DRV_VIDEO_FRAME_S));
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
    struct list_head *pstVdecPortFrameList;
    struct list_head *pnode;
    VDEC_PORT_FRAME_LIST_LOCK_S *pListLock = HI_NULL;
    VDEC_PORT_FRAME_LIST_NODE_S *pstVdecPortFrameListNode = HI_NULL;
    VDEC_PORT_FRAME_LIST_NODE_S *pastFrame[VDEC_MAX_PORT_FRAME] = {0};
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

static HI_S32 VDEC_InsertFrameIntoTmpList(HI_HANDLE hVdec, HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_S32 s32Index = 0;
    //VDEC_PORT_FRAME_LIST_NODE_S *pstVdecPortFrameListNode = HI_NULL;
    VDEC_PORT_FRAME_LIST_LOCK_S *pListLock = HI_NULL;

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

static HI_S32 VDEC_RecvPortFrameFromVpss(HI_HANDLE hVdec, HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstFrame)
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
static HI_S32 VDEC_EventHandle(HI_S32 s32ChanID, HI_S32 s32EventType, HI_VOID *pArgs)
{
    HI_S32 s32Ret;
    HI_HANDLE hHandle;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    USRDAT *pstUsrData = HI_NULL;
    HI_U32 u32WriteID;
    HI_U32 u32ReadID;
    HI_U32 u32IStreamSize = 0;
    UNSUPPORT_SPEC_E e_SpecType;
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
#if (0 == HI_VDEC_CC_FROM_IMAGE)
    HI_U32 u32ID;
    HI_U8 u8Type;
#endif
#endif

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
        HI_ERR_VDEC("chan %d not found, event=%d\n", s32ChanID, s32EventType);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    /* Event handle */
    switch (s32EventType)
    {
        case EVNT_DISP_EREA:
        case EVNT_IMG_SIZE_CHANGE:
        case EVNT_FRMRATE_CHANGE:
        case EVNT_SCAN_CHANGE:
        case EVNT_ASPR_CHANGE:
            break;

        case EVNT_NEW_IMAGE:
            if (pstChan->enCurState == VDEC_CHAN_STATE_RUN)
            {
#if 0
                HI_DRV_VIDEO_FRAME_S *pstLastFrm  = &(pstChan->stLastFrm);
                HI_DRV_VIDEO_FRAME_S stFrameInfo;
                memset(&stFrameInfo, 0, sizeof(HI_DRV_VIDEO_FRAME_S));
                pstImg = (IMAGE *)(*(HI_U32 *)pArgs);

                if (HI_NULL != pstImg)
                {
                    VDEC_ConvertFrm(pstChan->stCurCfg.enType, pstChan, HI_NULL, pstImg, &stFrameInfo);

                    /* Check norm change */
                    if ((stFrameInfo.stDispRect.s32Height != pstLastFrm->stDispRect.s32Height) ||
                        (stFrameInfo.stDispRect.s32Width != pstLastFrm->stDispRect.s32Width) ||
                        (stFrameInfo.bProgressive != pstLastFrm->bProgressive))
                    {
                        pstChan->bNormChange = HI_TRUE;
                        pstChan->stNormChangeParam.enNewFormat    = pstChan->enDisplayNorm;
                        pstChan->stNormChangeParam.u32ImageWidth  = stFrameInfo.u32Width;
                        pstChan->stNormChangeParam.u32ImageHeight = stFrameInfo.u32Height;
                        pstChan->stNormChangeParam.u32FrameRate = stFrameInfo.u32FrameRate;//stFrameRate.u32fpsInteger;
                        //pstChan->stNormChangeParam.stFrameRate.u32fpsDecimal = 0;//stFrameInfo.stFrameRate.u32fpsDecimal;
                        pstChan->stNormChangeParam.bProgressive = stFrameInfo.bProgressive;
                    }

                    //not same l00225186
                    /* Check frame packing */
                    //if (stFrameInfo.enFramePackingType != pstLastFrm->enFramePackingType)
                    if (stFrameInfo.eFrmType != pstLastFrm->eFrmType)
                    {
                        pstChan->bFramePackingChange = HI_TRUE;
                        //pstChan->enFramePackingType = stFrameInfo.enFramePackingType;
                        pstChan->enFramePackingType = stFrameInfo.eFrmType;
                    }

                    /* Save last frame */
                    *pstLastFrm = stFrameInfo;
                    //pstChan->bNewFrame = HI_TRUE;
                }

#endif

                if (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32Speed < 0 || HI_TRUE == s_stVdecDrv.astChanEntity[hHandle].pstChan->bLowdelay)
                {
                    (void)(s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss, HI_DRV_VPSS_USER_COMMAND_IMAGEREADY, NULL);
                }
            }

            break;

        case EVNT_FIND_IFRAME:
            pstChan->stStatInfo.u32TotalVdecParseIFrame++;

            if (1 == pstChan->stStatInfo.u32TotalVdecParseIFrame)
            {
                u32IStreamSize = *(HI_U32 *)pArgs;
                HI_DRV_STAT_Event(STAT_EVENT_ISTREAMGET, u32IStreamSize);
            }

            break;

        case EVNT_USRDAT:

#ifdef HI_TEE_SUPPORT
            if (pstChan->bTvp)
            {
                HI_ERR_VDEC("TVP not support USERDATA!\n");
                break;
            }
            else
#endif
            {
                pstUsrData = (USRDAT *)pArgs;

#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
#if (0 == HI_VDEC_CC_FROM_IMAGE)

                if (pstUsrData->data_size > 5)
                {
                    u32ID = *((HI_U32 *)pstUsrData->data);
                    u8Type = pstUsrData->data[4];

                    if ((VDEC_USERDATA_IDENTIFIER_DVB1 == u32ID) && (VDEC_USERDATA_TYPE_DVB1_CC == u8Type))
                    {
#if (1 == VDEC_USERDATA_NEED_ARRANGE)
                        USRDATA_Arrange(hHandle, pstUsrData);
#endif
                        break;
                    }
                }

#endif
#endif

                if (HI_NULL == pstChan->pstUsrData)
                {
                    pstChan->pstUsrData = HI_KMALLOC_ATOMIC_VDEC(sizeof(VDEC_USRDATA_PARAM_S));

                    if (HI_NULL == pstChan->pstUsrData)
                    {
                        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
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
                    break;
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
                break;
            }

        case EVNT_VDM_ERR:
        case EVNT_SE_ERR:
            pstChan->stStatInfo.u32TotalStreamErrNum++;
            break;

        case EVNT_IFRAME_ERR:
            pstChan->bIFrameErr = HI_TRUE;
            break;

            /* Capture BTL over */
        case EVNT_CAPTURE_BTL_OVER:
        {
            IMAGE *pstImage;

            if ((HI_NULL != pstChan->stBTL.pstFrame) && (1 == atomic_read(&pstChan->stBTL.atmWorking)))
            {
                pstImage = (IMAGE *)(*((HI_U32 *)pArgs));
                VDEC_ConvertFrm(pstChan->stCurCfg.enType, pstChan, pstImage, pstChan->stBTL.pstFrame);
                atomic_dec(&pstChan->stBTL.atmWorking);
                wake_up_interruptible(&(pstChan->stBTL.stWaitQue));
            }

            break;
        }

        /* End frame */
        case EVNT_LAST_FRAME:

            /* *(HI_U32*)pArgs: 0 success, 1 fail,  2 report last frame image id */
            if (1 == *(HI_U32 *)pArgs)
            {
                pstChan->u32EndFrmFlag = 1;
            }
            else if (2 <= *(HI_U32 *)pArgs)
            {
                pstChan->u32EndFrmFlag = 2;
                pstChan->u32LastFrmId = *(HI_U32 *)pArgs - 2;
            }

#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
            USRDATA_SetEosFlag(hHandle);
#endif
            pstChan->bEndOfStrm = HI_TRUE;
            break;

            /* Fake frame for vpss to release all frame to vfmw */
        case EVNT_FAKE_FRAME:
            pstChan->u32EndFrmFlag = 1;
            break;

            /* Resolution change */
        case EVNT_RESOLUTION_CHANGE:
            if (s_stVdecDrv.astChanEntity[hHandle].eCallBack)
            { s_stVdecDrv.astChanEntity[hHandle].eCallBack(hHandle, NULL, VIDDEC_EVT_STREAM_INFO_CHANGE); }

            pstChan->u8ResolutionChange = 1;
            break;

        case EVNT_UNSUPPORT_SPEC:  // UNSUPPORT类型的扩展，特殊处理后然后走EVNT_UNSUPPORT同样分支
            e_SpecType = ((HI_U32 *)pArgs)[0];

            switch (e_SpecType)
            {
                case SPEC_BIT_DEPTH:
                    pstChan->u32LastLumaBitdepth   = ((HI_U32 *)pArgs)[1];
                    pstChan->u32LastChromaBitdepth = ((HI_U32 *)pArgs)[1];
                    break;

                default:
                    break;
            }

            if (s_stVdecDrv.astChanEntity[hHandle].eCallBack)
            { s_stVdecDrv.astChanEntity[hHandle].eCallBack(hHandle, NULL, VIDDEC_EVT_UNSUPPORTED_STREAM_TYPE); }

            pstChan->bUnSupportStream = 1;
            break;

        case EVNT_UNSUPPORT:
            if (s_stVdecDrv.astChanEntity[hHandle].eCallBack)
            { s_stVdecDrv.astChanEntity[hHandle].eCallBack(hHandle, NULL, VIDDEC_EVT_UNSUPPORTED_STREAM_TYPE); }

            pstChan->bUnSupportStream = 1;
            break;

        case EVNT_RATIO_NOTZERO:
            pstChan->u32ErrRatio = *(HI_U32 *)pArgs;
            break;

        case EVNT_NEED_ARRANGE:                             //获取码流信息,为帧存划分做准备.l00273086//
            pstChan->bNeedAlloc = HI_TRUE;
            pstChan->u32RefFrameNum = ((HI_U32 *)pArgs)[0];
            pstChan->u32FrameSize = ((HI_U32 *)pArgs)[1];

            //VPSS在对标清的流做去隔行操作时，会占用3帧
            if (((((HI_U32 *)pArgs)[2]) <= 1920) && ((((HI_U32 *)pArgs)[3]) <= 1088))
            {
                pstChan->u32RefFrameNum += 4;
            }

            if (NULL != s_stVdecDrv.pVdecTask)
            {
                wake_up_process(s_stVdecDrv.pVdecTask);
            }

            break;

        case EVNT_VIDSTD_ERROR:
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

            break;

#ifdef Hi3716MV410

        case EVNT_ARRANGE_FRAME_BUF:
            pstChan->bNeedAlloc = HI_TRUE;
            pstChan->u32RefFrameNum = ((HI_U32 *)pArgs)[0];
            pstChan->u32FrameSize = ((HI_U32 *)pArgs)[1];

            //VPSS在对标清的流做去隔行操作时，会占用3帧
            if (((((HI_U32 *)pArgs)[2]) <= 1920) && ((((HI_U32 *)pArgs)[3]) <= 1088))
            {
                pstChan->u32RefFrameNum += 4;
            }

            if (NULL != s_stVdecDrv.pVdecTask)
            {
                wake_up_process(s_stVdecDrv.pVdecTask);
            }

            //HI_ERR_VDEC("after wake up process\n");

            break;

        case EVNT_POST_MODULE_OCCUPY_ONE_FRAME:

            //HI_ERR_VDEC("FUNCTION: %s\tLINE: %d\n", __FUNCTION__, __LINE__);

            if (((HI_U32 *)pArgs)[3] == 1)
            {
                //HI_ERR_VDEC("FUNCTION: %s\tLINE: %d\n", __FUNCTION__, __LINE__);

                pstChan->specialFrameExist = HI_TRUE;
                pstChan->specialFramePhyAddr = ((HI_U32 *)pArgs)[0];
                pstChan->specialFrameVirAddr = ((HI_U32 *)pArgs)[1];
                pstChan->specialFrameSize    = ((HI_U32 *)pArgs)[2];
            }
            else
            {
                //HI_ERR_VDEC("FUNCTION: %s\tLINE: %d\n", __FUNCTION__, __LINE__);
                pstChan->specialFrameExist = HI_FALSE;
            }

            break;

        case EVNT_REFRESH_CONFIG_FRAME_BUF:
            // 刷新已配置帧存的状态或者释放变分辨率之前动态分配的帧存
            VDEC_RefreshConfiguredFrameBuf(s32ChanID, ((HI_U32 *)pArgs)[0]);

            break;
#endif

        case EVNT_SCD_BUFFER_REMAP:
            pstChan->stSCDMMZBuf.pu8StartVirAddr = (HI_U8*)(HI_SIZE_T)(((HI_U64*)pArgs)[0]);
            pstChan->stEOSBuffer.pu8StartVirAddr = pstChan->stSCDMMZBuf.pu8StartVirAddr + pstChan->stSCDMMZBuf.u32Size - HI_VDEC_EOS_MEM_SIZE;
            break;

        case EVNT_VDH_BUFFER_REMAP:
            pstChan->stVDHMMZBuf.pu8StartVirAddr = (HI_U8*)(HI_SIZE_T)(((HI_U64*)pArgs)[0]);
            break;

        default:
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
            return HI_FAILURE;
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    return HI_SUCCESS;
}

//add by l00225186
/*vpss 回调函数*/
static HI_S32 VDEC_VpssEventHandle(HI_HANDLE hVdec, HI_DRV_VPSS_EVENT_E enEventID, HI_VOID *pstArgs)
{
    HI_S32 s32Ret;
    HI_S32 i;
    HI_HANDLE hHandle;
    HI_HANDLE hPort = HI_INVALID_HANDLE;
    HI_HANDLE hVpss = HI_INVALID_HANDLE;
    BUF_VPSS_S stMMZ_Buffer;
    HI_DRV_VIDEO_FRAME_S *pstFrame = HI_NULL;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;
    HI_VDEC_PRIV_FRAMEINFO_S *pstPrivInfo = HI_NULL;
    HI_DRV_VPSS_PORT_BUFLIST_STATE_S stVpssBufListState;
    HI_U32 u32Stride = 0;

    pstFrame = HI_VMALLOC_BUFMNG(sizeof(HI_DRV_VIDEO_FRAME_S));
    memset(pstFrame, 0, sizeof(HI_DRV_VIDEO_FRAME_S));

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
        HI_VFREE_BUFMNG(pstFrame);
        return HI_FAILURE;
    }

    /* Lock */
    VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[hHandle]);

    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_VDEC("Chan %d lock fail!\n", hHandle);
        HI_VFREE_BUFMNG(pstFrame);
        return HI_FAILURE;
    }

    pstVpssChan = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan);

    /* Event handle */
    switch (enEventID)
    {
        case VPSS_EVENT_CHECK_FRAMEBUFFER:
            hPort = ((HI_DRV_VPSS_BUFINFO_S *)pstArgs)->hPort;
            hVpss = (HI_HANDLE)PORTHANDLE_TO_VPSSID(hPort);
            s32Ret = BUFMNG_VPSS_CheckAvaibleBuffer(hVpss, hPort);

            if (HI_SUCCESS == s32Ret)
            {
                ((HI_DRV_VPSS_BUFINFO_S *)pstArgs)->bAvailable = HI_TRUE;
            }
            else
            {
                ((HI_DRV_VPSS_BUFINFO_S *)pstArgs)->bAvailable = HI_FALSE;
            }

            break;

        case VPSS_EVENT_GET_FRMBUFFER:
            hPort = ((HI_DRV_VPSS_FRMBUF_S *)pstArgs)->hPort;
            hVpss = (HI_HANDLE)PORTHANDLE_TO_VPSSID(hPort);
            memset(&stMMZ_Buffer, 0, sizeof(MMZ_BUFFER_S));
            stMMZ_Buffer.MMZBuf_frm.u32Size = ((HI_DRV_VPSS_FRMBUF_S *)pstArgs)->u32Size;

            s32Ret = BUFMNG_VPSS_RecBuffer(hVpss, hPort, &stMMZ_Buffer, &u32Stride);

            if (HI_SUCCESS != s32Ret)
            {
                VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
                HI_INFO_VDEC("Chan %d BUFMNG_VPSS_RecBuffer fail!\n", hHandle);
                HI_VFREE_BUFMNG(pstFrame);
                return HI_FAILURE;
            }

            ((HI_DRV_VPSS_FRMBUF_S *)pstArgs)->u32StartPhyAddr = stMMZ_Buffer.MMZBuf_frm.u32StartPhyAddr; //c00277632
            ((HI_DRV_VPSS_FRMBUF_S *)pstArgs)->pu8StartVirAddr = stMMZ_Buffer.MMZBuf_frm.pu8StartVirAddr;
            ((HI_DRV_VPSS_FRMBUF_S *)pstArgs)->u32PrivDataPhyAddr = stMMZ_Buffer.MMZBuf_meta.u32StartPhyAddr; //c00277632
            ((HI_DRV_VPSS_FRMBUF_S *)pstArgs)->pu8PrivDataVirAddr = stMMZ_Buffer.MMZBuf_meta.pu8StartVirAddr;
            ((HI_DRV_VPSS_FRMBUF_S *)pstArgs)->u32Stride       = u32Stride;
            break;

        case VPSS_EVENT_REL_FRMBUFFER:
            hPort = ((HI_DRV_VPSS_FRMBUF_S *)pstArgs)->hPort;
            hVpss = (HI_HANDLE)PORTHANDLE_TO_VPSSID(hPort);
            pstFrame->stBufAddr[0].u32PhyAddr_Y = ((HI_DRV_VPSS_FRMBUF_S *)pstArgs)->u32StartPhyAddr;
            BUFMNG_VPSS_RelBuffer(hVpss, hPort, pstFrame);
            break;

        case VPSS_EVENT_NEW_FRAME:
            s_stVdecDrv.astChanEntity[hHandle].pstChan->bNewFrame = HI_TRUE;

            if (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32Speed < 0)
            {
                SINT32 s32VpssHaveFrame = 0;

                s32VpssHaveFrame = VDEC_CheckVpssPortOutFrameStatus(hVdec);

                for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
                {
                    if (HI_SUCCESS == s32VpssHaveFrame)
                    {
                        if (HI_INVALID_HANDLE != s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].hPort
                            && (HI_TRUE == pstVpssChan->stPort[i].bEnable))
                        {
                            memset(pstFrame, 0, sizeof(HI_DRV_VIDEO_FRAME_S));
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
                                pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S *)(((HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv))->u32Reserve);
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
                        if ((HI_FALSE != s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32GetFirstVpssFrameFlag) &&
                            (((0 == pstPrivInfo->s32FrameFormat) && (0 == pstPrivInfo->s32FieldFlag)) ||
                             (((0 == pstPrivInfo->s32BottomFieldFrameFormat) || (0 == pstPrivInfo->s32TopFieldFrameFormat)) && (1 == pstPrivInfo->s32FieldFlag))))
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
                        if ((HI_TRUE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32GetFirstVpssFrameFlag) &&
                            ((pstPrivInfo->s32GopNum) != (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32PortLastFrameGopNum)) &&
                            (0 < s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].s32PortTmpListPos))
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
            }
            else
            {
                if (HI_DRV_VDEC_BUF_USER_ALLOC_MANAGE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.enFrameBuffer ||
                    HI_DRV_VDEC_BUF_VDEC_ALLOC_MANAGE ==  s_stVdecDrv.astChanEntity[hHandle].stVpssChan.enFrameBuffer)
                {
                    if (HI_NULL == pstArgs)
                    {
                        HI_ERR_VDEC("%s %d args err!\n", __func__, __LINE__);
                        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
                        HI_VFREE_BUFMNG(pstFrame);
                        return HI_FAILURE;
                    }

                    hPort = ((HI_DRV_VPSS_FRMINFO_S *)pstArgs)->hPort;

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
                        break;
                    }

                    if (HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].bufferType)
                    {
                        hPort = ((HI_DRV_VPSS_FRMINFO_S *)pstArgs)->hPort;
                        hVpss = (HI_HANDLE)PORTHANDLE_TO_VPSSID(hPort);
                        s32Ret = VDEC_VpssNewImageEvent(hVpss, hPort, &((HI_DRV_VPSS_FRMINFO_S *)pstArgs)->stFrame);

                        if (HI_SUCCESS != s32Ret)
                        {
                            HI_WARN_VDEC("VDEC_VpssNewImageEvent err :%d \n", s32Ret);
                        }
                    }
                }
            }

            break;
#if 1

        case  VPSS_EVENT_BUFLIST_FULL:
            for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
            {
                if (VDEC_PORT_TYPE_MASTER == pstVpssChan->stPort[i].enPortType
                    || VDEC_PORT_TYPE_SLAVE == pstVpssChan->stPort[i].enPortType)
                {
                    HI_DRV_VPSS_PORT_CFG_S stVpssPortCfg;
                    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortCfg)(pstVpssChan->stPort[i].hPort, &stVpssPortCfg);

                    if (HI_SUCCESS == s32Ret)
                    {
                        if (HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == stVpssPortCfg.stBufListCfg.eBufType)
                        {
                            *(HI_DRV_VPSS_BUFFUL_STRATAGY_E *)pstArgs = HI_DRV_VPSS_BUFFUL_PAUSE;
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
                                    *(HI_DRV_VPSS_BUFFUL_STRATAGY_E *)pstArgs = HI_DRV_VPSS_BUFFUL_PAUSE;
                                    break;
                                }
                                else
                                {
                                    /*master_port slave_port not full:HI_DRV_VPSS_BUFFUL_KEEPWORKING*/
                                    *(HI_DRV_VPSS_BUFFUL_STRATAGY_E *)pstArgs = HI_DRV_VPSS_BUFFUL_KEEPWORKING;
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
                VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
                HI_VFREE_BUFMNG(pstFrame);
                return HI_FAILURE;
            }
            else
            {

            }

            break;
#endif
#if 0

        case  VPSS_EVENT_BUFLIST_FULL:
            for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
            {
                if (VDEC_PORT_TYPE_MASTER == pstVpssChan->stPort[i].enPortType
                    || VDEC_PORT_TYPE_SLAVE == pstVpssChan->stPort[i].enPortType)
                {
                    hPort = pstVpssChan->stPort[i].hPort;
                    break;
                }
                else
                {
                    *(HI_DRV_VPSS_BUFFUL_STRATAGY_E *)pstArgs = HI_DRV_VPSS_BUFFUL_KEEPWORKING;
                    break;
                }
            }

            if (i >= VDEC_MAX_PORT_NUM)
            {
                HI_WARN_VDEC("Port not exist!\n");
                VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
                return HI_FAILURE;
            }
            else
            {
                memset(&stVpssBufListState, 0, sizeof(HI_DRV_VPSS_PORT_BUFLIST_STATE_S));
                s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortBufListState)(hPort, &stVpssBufListState);

                if (HI_SUCCESS == s32Ret)
                {
                    /*master_port slave_port full:HI_DRV_VPSS_BUFFUL_PAUSE,master_port slave_port not full:HI_DRV_VPSS_BUFFUL_KEEPWORKING*/
                    if (stVpssBufListState.u32FulBufNumber == stVpssBufListState.u32TotalBufNumber)
                    {
                        /*master_port slave_port full:HI_DRV_VPSS_BUFFUL_PAUSE*/
                        *(HI_DRV_VPSS_BUFFUL_STRATAGY_E *)pstArgs = HI_DRV_VPSS_BUFFUL_PAUSE;
                        break;
                    }
                    else
                    {
                        /*master_port slave_port not full:HI_DRV_VPSS_BUFFUL_KEEPWORKING*/
                        *(HI_DRV_VPSS_BUFFUL_STRATAGY_E *)pstArgs = HI_DRV_VPSS_BUFFUL_KEEPWORKING;
                    }
                }
            }

            break;
#endif

        default:
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
            HI_VFREE_BUFMNG(pstFrame);
            return HI_FAILURE;
    }

    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    HI_VFREE_BUFMNG(pstFrame);
    return HI_SUCCESS;
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

        default:
            return STD_END_RESERVED;
    }
}
HI_VOID VDEC_Chan_GetMaxResoution(HI_S32 *pMaxWidth, HI_S32 *pMaxHeight)
{
    HI_S32                  s32Ret;
    HI_SYS_MEM_CONFIG_S     MemConfig = {0};
    s32Ret = HI_DRV_SYS_GetMemConfig(&MemConfig);

    if (HI_SUCCESS != s32Ret)
    {
        MemConfig.u32TotalSize = 1024; // if get memory config failed, we use 1G config
    }

    switch (MemConfig.u32TotalSize)
    {
        case 256:
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

            break;
        }

        case 512:
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

            break;
        }

        case 768:
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

            break;
        }

        case 2048:
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

            break;
        }

        case 1024:
        default:
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

            break;
        }
    }

    return;

}
static HI_S32 VDEC_SetAttr(VDEC_CHANNEL_S *pstChan)
{
    HI_S32                  s32Ret;
    VDEC_CHAN_CFG_S         stVdecChanCfg;
    HI_UNF_VCODEC_ATTR_S   *pstCfg          = &pstChan->stCurCfg;
    HI_CHIP_TYPE_E          enChipType      = HI_CHIP_TYPE_HI3716C;
    HI_CHIP_VERSION_E       enChipVersion   = HI_CHIP_VERSION_V200;

    memset(&stVdecChanCfg, 0, sizeof(VDEC_CHAN_CFG_S));
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_GET_CHAN_CFG, &stVdecChanCfg);

    if (VDEC_OK != s32Ret)
    {
        HI_ERR_VDEC("VFMW GET_CHAN_CFG err!\n");
        return HI_FAILURE;
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
    stVdecChanCfg.s32DecOrderOutput =  pstCfg->bOrderOutput;

    stVdecChanCfg.eVidStd = VDEC_CodecTypeUnfToFmw(pstCfg->enType);

    if (HI_UNF_VCODEC_TYPE_VC1 == pstCfg->enType)
    {
        if ((1 == pstChan->stOption.u32DynamicFrameStoreAllocEn) && \
            (0 == pstChan->stOption.u32SelfAdaptionDFS))
        {
            pstChan->stOption.s32ExtraFrameStoreNum = 1;
        }

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

    switch (pstCfg->enMode)
    {
        case HI_UNF_VCODEC_MODE_NORMAL:
            stVdecChanCfg.s32DecMode = IPB_MODE;
            break;

        case HI_UNF_VCODEC_MODE_IP:
            stVdecChanCfg.s32DecMode = IP_MODE;
            break;

        case HI_UNF_VCODEC_MODE_I:
            stVdecChanCfg.s32DecMode = I_MODE;
            break;

        case HI_UNF_VCODEC_MODE_DROP_INVALID_B:
            stVdecChanCfg.s32DecMode = DISCARD_B_BF_P_MODE;
            break;

        case HI_UNF_VCODEC_MODE_BUTT:
        default:
            stVdecChanCfg.s32DecMode = IPB_MODE;
            break;
    }

    if (pstChan->bLowdelay)
    {
        stVdecChanCfg.s32DecMode = IP_MODE;
        pstCfg->enMode = HI_UNF_VCODEC_MODE_IP;
    }

    if (HI_TRUE == pstCfg->bOrderOutput)  // normal/simple dpb mode
    {
        stVdecChanCfg.s32DecOrderOutput = pstCfg->bOrderOutput + pstCfg->s32CtrlOptions;
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

        EnVcmp  = 0;
    }

    /* Config decode compress attr */
    stVdecChanCfg.s32VcmpEn = EnVcmp;

    stVdecChanCfg.s32VcmpWmStartLine = 0;
    stVdecChanCfg.s32VcmpWmEndLine = 0;

    stVdecChanCfg.s32Btl1Dt2DEnable = pstChan->stOption.s32Btl1Dt2DEnable;
    stVdecChanCfg.s32BtlDbdrEnable = pstChan->stOption.s32BtlDbdrEnable;


    /* Only if pstCfg->orderOutput is 1 we do the judge */
    if (pstCfg->bOrderOutput)
    {
        if (pstCfg->s32CtrlOptions & HI_UNF_VCODEC_CTRL_OPTION_SIMPLE_DPB || pstChan->bLowdelay)
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

    /* Set to VFMW */
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_CFG_CHAN, &stVdecChanCfg);

    if (VDEC_OK != s32Ret)
    {
        HI_ERR_VDEC("VFMW CFG_CHAN err!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 VDEC_SeekPTS(HI_HANDLE hHandle, HI_U32 *pu32SeekPts, HI_U32 u32Gap)
{
    HI_S32 s32Ret;
    HI_S32 s32RetStart;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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

static HI_S32 VDEC_GetCap(VDEC_CAP_S *pstCap)
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

static HI_S32 VDEC_CreateStrmBuf(HI_DRV_VDEC_STREAM_BUF_S *pstBuf)
{
    HI_S32 s32Ret;
    BUFMNG_INST_CONFIG_S stBufInstCfg;
#ifdef HI_TEE_SUPPORT
    memset(&stBufInstCfg, 0, sizeof(BUFMNG_INST_CONFIG_S));
#endif

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

static HI_S32 VDEC_StrmBuf_SetUserAddr(HI_HANDLE hHandle, HI_U32 u32Addr)
{
    return BUFMNG_SetUserAddr(hHandle, u32Addr);
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
    VDEC_CHANNEL_S *pstChan = HI_NULL;

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
    VDEC_CHANNEL_S *pstChan = HI_NULL;

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
    VDEC_CHANNEL_S *pstChan = HI_NULL;

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

HI_S32 HI_DRV_VDEC_DiscardFrm(HI_HANDLE hHandle, VDEC_DISCARD_FRAME_S *pstParam)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;

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

static HI_S32 VDEC_GetStrmBuf(HI_HANDLE hHandle, VDEC_ES_BUF_PRI_S *pstEsBuf, HI_BOOL bUserSpace)
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

static HI_S32 VDEC_PutStrmBuf(HI_HANDLE hHandle, VDEC_ES_BUF_PRI_S *pstEsBuf, HI_BOOL bUserSpace)
{
    HI_S32 s32Ret;
    BUFMNG_BUF_S stElem;
    HI_HANDLE hVdec = HI_INVALID_HANDLE;
#ifdef 	__VDEC_ADD_EXT_EOP__
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
    }


    if ((pstEsBuf->bEndOfFrame) && (flag))
    {
        /* Get buffer */

		switch(s_stVdecDrv.astChanEntity[hVdec].pstChan->stCurCfg.enType)
		{
		    case HI_UNF_VCODEC_TYPE_MPEG4:
				stElem.u32Size = sizeof(g_EopTab_MPEG4)/sizeof(HI_CHAR);
				break;
			case HI_UNF_VCODEC_TYPE_HEVC:
				stElem.u32Size = sizeof(g_EopTab_H265)/sizeof(HI_CHAR);
				break;
			case HI_UNF_VCODEC_TYPE_H264:
				stElem.u32Size = sizeof(g_EopTab_H264)/sizeof(HI_CHAR);
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
    		switch(s_stVdecDrv.astChanEntity[hVdec].pstChan->stCurCfg.enType)
    		{
    		    case HI_UNF_VCODEC_TYPE_MPEG4:
    				memcpy(stElem.pu8KnlVirAddr,g_EopTab_MPEG4,sizeof(HI_CHAR)*stElem.u32Size);
    				break;
    			case HI_UNF_VCODEC_TYPE_HEVC:
    				memcpy(stElem.pu8KnlVirAddr,g_EopTab_H265,sizeof(HI_CHAR)*stElem.u32Size);
    				break;
    			case HI_UNF_VCODEC_TYPE_H264:
    				memcpy(stElem.pu8KnlVirAddr,g_EopTab_H264,sizeof(HI_CHAR)*stElem.u32Size);
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
		    HI_INFO_VDEC("BUFMNG_GetWriteBuffer (extern size = %d,pu8KnlVirAddr = %p) failed\n",stElem.u32Size,stElem.pu8KnlVirAddr);
		}
    }
#endif

    return HI_SUCCESS;
}

static HI_S32 VDEC_RecvStrmBuf(HI_HANDLE hHandle, STREAM_DATA_S *pstPacket, HI_BOOL bUserSpace)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    BUFMNG_BUF_S stEsBuf;

    memset(&stEsBuf, 0, sizeof(BUFMNG_BUF_S));

    /* Check parameter */
    if (HI_NULL == pstPacket)
    {
        HI_ERR_VDEC("bad param!\n");
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
        DMX_Stream_S vidEsBuf = {0};

        pstChan->stStatInfo.u32VdecAcqBufTry++;

        s32Ret = (s_stVdecDrv.pDmxFunc->pfnDmxAcquireEs)(pstChan->hDmxVidChn, &vidEsBuf);

        if (HI_SUCCESS != s32Ret)
        {
            goto OUT;
        }

        pstPacket->PhyAddr = vidEsBuf.u32BufPhyAddr;
        pstPacket->VirAddr = vidEsBuf.pu8BufVirAddr;
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
    }
    /* Get ES data from BM */
    else
    {
        s32Ret = BUFMNG_AcqReadBuffer(pstChan->hStrmBuf, &stEsBuf);

        if (HI_SUCCESS != s32Ret)
        {
            goto OUT;
        }

        pstPacket->PhyAddr = stEsBuf.u32PhyAddr;

        /* If get from user space, pass user virtual address */
        if (bUserSpace)
        {
            pstPacket->VirAddr = stEsBuf.pu8UsrVirAddr;
        }
        /* If get from kernel space, pass kernel virtual address */
        else
        {
            pstPacket->VirAddr = stEsBuf.pu8KnlVirAddr;
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

		pstPacket->is_extern_eop_flag = (stEsBuf.u32Marker & BUFMNG_EXTERN_EOP_BIT)?1:0;
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
    }

OUT:

    /* Added for set eos flag */
    /* Must be end of stream */
    if ((pstChan->bSetEosFlag) &&
        /* Get ES buffer fail */
        (((pstChan->hStrmBuf != HI_INVALID_HANDLE) && (HI_SUCCESS != s32Ret)) ||
         /* Get Demux buffer HI_ERR_DMX_EMPTY_BUFFER */
         ((pstChan->hDmxVidChn != HI_INVALID_HANDLE) && (HI_ERR_DMX_EMPTY_BUFFER == s32Ret))))
    {
        pstPacket->PhyAddr = pstChan->stEOSBuffer.u32StartPhyAddr;
        pstPacket->VirAddr = pstChan->stEOSBuffer.pu8StartVirAddr;
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

static HI_S32 VDEC_RlsStrmBuf(HI_HANDLE hHandle, STREAM_DATA_S *pstPacket, HI_BOOL bUserSpace)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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
        vidEsBuf.pu8BufVirAddr = pstPacket->VirAddr;
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
            pstChan->stStatInfo.u32TotalVdecHoldByte -= pstPacket->Length;
            pstChan->stStatInfo.u32VdecRlsBufOK++;
        }

        return s32Ret;
    }
    /* Put BM buffer */
    else
    {
        stBuf.u32PhyAddr = 0;

        if (bUserSpace)
        {
            stBuf.pu8UsrVirAddr = pstPacket->VirAddr;
            stBuf.pu8KnlVirAddr = HI_NULL;
        }
        else
        {
            stBuf.pu8KnlVirAddr = pstPacket->VirAddr;
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

        pstChan->stStatInfo.u32TotalVdecHoldByte -= pstPacket->Length;
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    }

    return HI_SUCCESS;
}

static HI_S32 VDEC_Chan_RecvStrmBuf(HI_S32 hHandle, STREAM_DATA_S *pstPacket)
{
    return VDEC_RecvStrmBuf((HI_HANDLE)hHandle, pstPacket, HI_FALSE);
}

static HI_S32 VDEC_Chan_RlsStrmBuf(HI_S32 hHandle, STREAM_DATA_S *pstPacket)
{
    return VDEC_RlsStrmBuf((HI_HANDLE)hHandle, pstPacket, HI_FALSE);
}
//add by l00225186
/*提供给vo收帧的函数*/
static HI_S32 VDEC_Chan_VOAcqFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame)
{
    HI_S32 s32Ret;

    if ((HI_INVALID_HANDLE == hPort) || (HI_NULL == pstVpssFrame))
    {
        HI_ERR_VDEC("VDEC_Chan_VOAcqFrame Bad param!\n");
        return HI_FAILURE;
    }

    /*调用vpss的获取帧存函数,要确定vpss释放函数成功返回的是HI_SUCCESS*/
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortFrame)(hPort, pstVpssFrame);
    return s32Ret;
}
HI_S32 HI_VDEC_Chan_VOAcqFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstVpssFrame)
{
    HI_S32 s32Ret;
    s32Ret = HI_SUCCESS;
    if ((HI_INVALID_HANDLE == hPort) || (HI_NULL == pstVpssFrame))
    {
        HI_ERR_VDEC("VDEC_Chan_VOAcqFrame Bad param!\n");
        return HI_FAILURE;
    }
    s32Ret = VDEC_Chan_VOAcqFrame(hPort,pstVpssFrame);
	if(HI_SUCCESS != s32Ret)
	{
	    HI_ERR_VDEC("VDEC_Chan_VOAcqFrame err!\n");
		return HI_FAILURE;
	}
    return s32Ret;
}

/*提供给vo释放帧的函数*/
static HI_S32 VDEC_Chan_VORlsFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame)
{
    HI_S32 s32Ret;
    HI_DRV_VPSS_PORT_CFG_S stVpssPortCfg;
    HI_HANDLE hVdec = HI_INVALID_HANDLE;
    VDEC_CHANNEL_S *pstChan;
    s32Ret = VDEC_FindVdecHandleByPortHandle(hPort, &hVdec);

    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
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
            return HI_FAILURE;
        }
    }

    if ((HI_INVALID_HANDLE == hPort) || (HI_NULL == pstVpssFrame))
    {
        HI_ERR_VDEC("VDEC_Chan_VORlsFrame Bad param!\n");
        return HI_FAILURE;
    }

    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortCfg)(hPort, &stVpssPortCfg);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("HI_DRV_VPSS_GetDefaultPortCfg err!\n");
        return HI_FAILURE;
    }

    if (HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE == stVpssPortCfg.stBufListCfg.eBufType)
    {
        s32Ret = BUFMNG_VPSS_RelBuffer((HI_HANDLE)PORTHANDLE_TO_VPSSID(hPort), hPort, pstVpssFrame);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("BUFMNG_VPSS_RelBuffer err!\n");
            return HI_FAILURE;
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
    }

    return s32Ret;
}

HI_S32 HI_VDEC_Chan_VORlsFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstVpssFrame)
{
    HI_S32 s32Ret;
	if(HI_NULL == pstVpssFrame)
        {
	    HI_ERR_VDEC("invalid param !\n");
        return HI_FAILURE;
    }
    s32Ret = VDEC_Chan_VORlsFrame(hPort, pstVpssFrame);
    if (HI_SUCCESS != s32Ret)
    {
	     HI_ERR_VDEC("VDEC_Chan_VORlsFrame err !\n");
        return HI_FAILURE;
    }
    return s32Ret;
}

static HI_S32 VDEC_ConvertWinInfo(HI_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg, HI_DRV_WIN_PRIV_INFO_S *pstWinInfo)
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
    pstVpssPortCfg->enOutBitWidth				 = pstWinInfo->enBitWidth;
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
static HI_S32 VDEC_Chan_VOChangeWinInfo(HI_HANDLE hPort, HI_DRV_WIN_PRIV_INFO_S *pstWinInfo)
{
    HI_S32 s32Ret;
    HI_DRV_VPSS_PORT_CFG_S stVpssPortCfg;
    HI_DRV_VPSS_CFG_S stVpssCfg;
    HI_HANDLE hVpss;
    HI_HANDLE hVdec = HI_INVALID_HANDLE;

    if ((HI_INVALID_HANDLE == hPort) || (HI_NULL == pstWinInfo))
    {
        HI_ERR_VDEC("VDEC_Chan_VORlsFrame Bad param!\n");
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
	s32Ret = VDEC_Chan_VOChangeWinInfo(hPort,pstWinInfo);
    if (HI_SUCCESS != s32Ret)
    {
	    HI_ERR_VDEC("call VDEC_Chan_VOChangeWinInfo err!\n");
    }
    return s32Ret;
}

//add by l00225185
/*创建vpss*/
static HI_S32 VDEC_Chan_CreateVpss(HI_HANDLE hVdec, HI_HANDLE *phVpss)
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

    s32Ret = down_interruptible(&s_stVdecDrv.stSem);

    if (0 != s32Ret)
    {
        HI_WARN_VDEC("call down_interruptible err!\n");
    }

    /*保存vpss句柄*/
    s_stVdecDrv.astChanEntity[hVdec].stVpssChan.hVpss = *phVpss;
    up(&s_stVdecDrv.stSem);


    s_stVdecDrv.astChanEntity[hVdec].stVpssChan.eFramePackType = HI_UNF_FRAME_PACKING_TYPE_BUTT;
    s_stVdecDrv.astChanEntity[hVdec].stVpssChan.enFrameBuffer = HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE;//HI_DRV_VDEC_BUF_USER_ALLOC_MANAGE;//;
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
            up(&s_stVdecDrv.stSem);
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

static HI_S32 VDEC_Chan_CreatePort(HI_HANDLE hVpss, HI_HANDLE *phPort, VDEC_PORT_ABILITY_E ePortAbility )
{
    HI_S32 s32Ret;
    HI_DRV_VPSS_PORT_CFG_S stVpssPortCfg;
    HI_S32 i, j;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;

#ifdef HI_TEE_SUPPORT
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;

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
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;

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
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;

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
    VDEC_PORT_FRAME_LIST_LOCK_S *pListLock = HI_NULL;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;
    VDEC_PORT_FRAME_LIST_NODE_S *pstListNode = HI_NULL;

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
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;
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

static HI_S32 VDEC_Chan_SetVpssAttr(HI_HANDLE hVpss, HI_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstPortCfg)
{
    HI_S32                  s32Ret;
    HI_S32                  i, j;
    VDEC_VPSSCHANNEL_S      *pstVpssChan = HI_NULL;

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

static HI_S32 VDEC_Chan_GetVpssAttr(HI_HANDLE hVpss, HI_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstPortCfg)
{
    HI_S32                  s32Ret;
    HI_S32                  i, j;
    VDEC_VPSSCHANNEL_S      *pstVpssChan = HI_NULL;

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

static HI_S32 VDEC_Chan_SetExtBuffer(HI_HANDLE hVpss, VDEC_BUFFER_ATTR_PRI_S *pstBufferAttr)
{
    HI_S32 i;
    BUFMNG_VPSS_NODE_S *pstBufNode;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;
    BUFMNG_VPSS_INST_S *pstBufVpssInst;
    BUFMNG_VPSS_NODE_S *pstTarget;
    struct list_head *pos, *n;
    HI_S32 s32Ret = HI_SUCCESS;

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

            pstBufNode->MMZBuf_frm.u32StartPhyAddr = pstBufferAttr->u32PhyAddr[i];
            pstBufNode->MMZBuf_frm.pu8StartVirAddr = (HI_U8*)(HI_SIZE_T)pstBufferAttr->u64UsrVirAddr[i];
            pstBufNode->MMZBuf_meta.u32StartPhyAddr = pstBufferAttr->u32PhyAddr_meta[i];
            pstBufNode->MMZBuf_meta.pu8StartVirAddr = (HI_U8*)(HI_SIZE_T)pstBufferAttr->u64UsrVirAddr_meta[i];

            pstBufNode->MMZBuf_frm.u32Size         = pstBufferAttr->u32BufSize;
            pstBufNode->MMZBuf_meta.u32Size         = pstBufferAttr->u32MetaBufSize;
            pstBufNode->enFrameBufferState       = HI_DRV_VDEC_BUF_STATE_IN_VDEC_EMPTY;
            pstBufNode->u32Stride                = pstBufferAttr->u32Stride;
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
static HI_S32 VDEC_Chan_CheckAndDelBuffer(HI_HANDLE hVpss, VDEC_BUFFER_INFO_S *pstBufInfo)
{
    HI_S32 i;
    struct list_head *pos, *n;
    BUFMNG_VPSS_NODE_S *pstTarget;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;
    BUFMNG_VPSS_INST_S *pstBufVpssInst;

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

        if (pstBufInfo->u32PhyAddr == pstTarget->MMZBuf_frm.u32StartPhyAddr)
        {
            pstBufInfo->enBufState = VDEC_BUF_STATE_EMPTY;
            list_del_init(pos);
            pstBufVpssInst->u32BufNum--;
             HI_VFREE_BUFMNG(pstTarget);
            BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[0].stBufVpssInst.stUnAvailableListLock);
            return HI_SUCCESS;
        }
    }
    list_for_each_safe(pos, n, &(pstVpssChan->stPort[0].stBufVpssInst.stVpssBufUnAvailableList))
    {
        pstTarget = list_entry(pos, BUFMNG_VPSS_NODE_S, node);

        if ( pstBufInfo->u32PhyAddr == pstTarget->MMZBuf_frm.u32StartPhyAddr)
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
                 HI_VFREE_BUFMNG(pstTarget);
                BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[0].stBufVpssInst.stUnAvailableListLock);
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
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;
    BUFMNG_VPSS_INST_S *pstBufVpssInst;

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
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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
static HI_S32 VDEC_Chan_GetFrmPackingType(HI_HANDLE hVpss, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E *penFramePackType)
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

HI_S32 HI_DRV_VDEC_GetFrmPackingType(HI_HANDLE hVdec, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E *penFramePackType)
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
HI_S32 HI_DRV_VDEC_GetVideoFrameInfo(HI_HANDLE hVdec, HI_UNF_AVPLAY_VIDEO_FRAME_INFO_S *pstVideoFrameInfo)
{
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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
static HI_S32 VDEC_Chan_GetPortParam(HI_HANDLE hVpss, HI_HANDLE hPort, VDEC_PORT_PARAM_S *pstPortParam)
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

static HI_S32 VDEC_Chan_GetPortState(HI_HANDLE hHandle, HI_BOOL *bAllPortComplete)
{
    HI_S32 s32Ret;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;

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
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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
    ((HI_VDEC_PRIV_FRAMEINFO_S *)(((HI_DRV_VIDEO_PRIVATE_S *)(pstChan->stLastFrm.u32Priv))->u32Reserve))->u8EndFrame = 1;
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hVdec]);
    return HI_SUCCESS;

}

static HI_S32 VDEC_Chan_GetFrmStatusInfo(HI_HANDLE hVdec, HI_HANDLE hPort, VDEC_FRMSTATUSINFO_S *pstVdecFrmStatusInfo)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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
    BUFMNG_VPSS_NODE_S *pstTarget;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;

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
    BUFMNG_VPSS_NODE_S *pstTarget;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;

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

//add by l00225186
/*Avplay收帧，vdec直接在vpss中取*/
static HI_S32 VDEC_Chan_RecvVpssFrmBuf(HI_HANDLE hVpss, HI_DRV_VIDEO_FRAME_PACKAGE_S *pstFrm)
{
    HI_S32 s32Ret;
    HI_S32 i, j = 0;
    BUFMNG_VPSS_NODE_S *pstTarget = HI_NULL;
    HI_HANDLE hMASTER = HI_INVALID_HANDLE;
    HI_HANDLE hSLAVE = HI_INVALID_HANDLE;

    VDEC_CHANNEL_S *pstChan = HI_NULL;
    VDEC_CHAN_STATINFO_S *pstStatInfo = HI_NULL;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    if ((HI_INVALID_HANDLE == hVpss) || (HI_NULL == pstFrm))
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_FAILURE;
    }

    s32Ret = VDEC_FindVdecHandleByVpssHandle(hVpss, &hHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("ERROR:func:%s line:%d!\n", __func__, __LINE__);
        return HI_FAILURE;
    }

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

    pstStatInfo->u32AvplayRcvFrameTry ++;
    pstVpssChan = &(s_stVdecDrv.astChanEntity[i].stVpssChan);

    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if ((HI_INVALID_HANDLE != pstVpssChan->stPort[j].hPort)
            && (HI_TRUE == pstVpssChan->stPort[j].bEnable))
        {
            if ((HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE == pstVpssChan->stPort[j].bufferType)
                && ((VDEC_PORT_TYPE_MASTER == pstVpssChan->stPort[j].enPortType)
                    || (VDEC_PORT_TYPE_SLAVE == pstVpssChan->stPort[j].enPortType)))
            {
                if (VDEC_PORT_TYPE_MASTER == pstVpssChan->stPort[j].enPortType)
                {
                    hMASTER = pstVpssChan->stPort[j].hPort;
                }

                if (VDEC_PORT_TYPE_SLAVE == pstVpssChan->stPort[j].enPortType)
                {
                    hSLAVE = pstVpssChan->stPort[j].hPort;
                }
            }
        }
    }

    if (hMASTER != HI_INVALID_HANDLE || hSLAVE != HI_INVALID_HANDLE)
    {
        HI_DRV_VPSS_PORT_AVAILABLE_S stCanGetFrm;

        if (hMASTER != HI_INVALID_HANDLE)
        {
            stCanGetFrm.hPort = hMASTER;
            stCanGetFrm.bAvailable = HI_FALSE;
            s_stVdecDrv.pVpssFunc->pfnVpssSendCommand(pstVpssChan->hVpss,
                    HI_DRV_VPSS_USER_COMMAND_CHECKAVAILABLE, &(stCanGetFrm));

            if (stCanGetFrm.bAvailable == HI_FALSE)
            {
                VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
                return HI_FAILURE;
            }
        }

        if (hSLAVE != HI_INVALID_HANDLE)
        {
            stCanGetFrm.hPort = hSLAVE;
            stCanGetFrm.bAvailable = HI_FALSE;
            s_stVdecDrv.pVpssFunc->pfnVpssSendCommand(pstVpssChan->hVpss,
                    HI_DRV_VPSS_USER_COMMAND_CHECKAVAILABLE, &(stCanGetFrm));

            if (stCanGetFrm.bAvailable == HI_FALSE)
            {
                VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
                return HI_FAILURE;
            }
        }
    }

    /*调用vpss接口获取主port数据*/
    if (hMASTER != HI_INVALID_HANDLE)
    {
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortFrame)(hMASTER, &pstFrm->stFrame[pstFrm->u32FrmNum].stFrameVideo);

        if (HI_SUCCESS != s32Ret)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
            HI_ERR_VDEC("Get MainPort Frame err!\n");
            return HI_FAILURE;
        }

        pstFrm->stFrame[pstFrm->u32FrmNum].hport = hMASTER;
        memcpy(&(pstChan->stLastVpssFrm), &pstFrm->stFrame[pstFrm->u32FrmNum].stFrameVideo, sizeof(HI_DRV_VIDEO_FRAME_S));
        pstFrm->u32FrmNum++;
    }

    if (hSLAVE != HI_INVALID_HANDLE)
    {
        s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortFrame)(hSLAVE, &pstFrm->stFrame[pstFrm->u32FrmNum].stFrameVideo);

        if (HI_SUCCESS != s32Ret)
        {
            VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
            HI_ERR_VDEC("Get SlavePort Frame err!\n");
            return HI_FAILURE;
        }

        pstFrm->stFrame[pstFrm->u32FrmNum].hport = hSLAVE;
        pstFrm->u32FrmNum++;
    }

    for (j = 0; j < VDEC_MAX_PORT_NUM; j++)
    {
        if ((HI_INVALID_HANDLE != pstVpssChan->stPort[j].hPort) && (1 == pstVpssChan->stPort[j].bEnable))
        {
            if ((HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE == pstVpssChan->stPort[j].bufferType) && (VDEC_PORT_TYPE_VIRTUAL == pstVpssChan->stPort[j].enPortType))
            {
                /*调用vpss接口获取从port数据*/
                s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssGetPortFrame)(pstVpssChan->stPort[j].hPort, &pstFrm->stFrame[pstFrm->u32FrmNum].stFrameVideo);

                if (HI_SUCCESS == s32Ret)
                {
                    pstFrm->stFrame[pstFrm->u32FrmNum].hport = pstVpssChan->stPort[j].hPort;
                    pstFrm->u32FrmNum++;
                }
            }
        }
    }

    if (HI_SUCCESS != VDEC_Chan_AllPortHaveDate(hVpss))
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        return HI_FAILURE;
    }


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
                    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
                    return HI_FAILURE;
                }

                if ((pstVpssChan->stPort[j].stBufVpssInst.pstUnAvailableListPos->next) == &(pstVpssChan->stPort[j].stBufVpssInst.stVpssBufAvailableList))
                {
                    BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
                    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
                    return HI_FAILURE;
                }

                pstTarget = list_entry(pstVpssChan->stPort[j].stBufVpssInst.pstUnAvailableListPos->next, BUFMNG_VPSS_NODE_S, node);

                if (HI_NULL == pstTarget)
                {
                    BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
                    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
                    return HI_FAILURE;
                }

                if (HI_DRV_VDEC_BUF_STATE_IN_USER == pstTarget->enFrameBufferState)
                {
                    BUFMNG_SpinUnLockIRQ(&pstVpssChan->stPort[j].stBufVpssInst.stUnAvailableListLock);
                    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
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

    if (0 == pstFrm->u32FrmNum)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        return HI_FAILURE;
    }

    pstStatInfo->u32AvplayRcvFrameOK++;
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_GetEsBuf(HI_HANDLE hHandle, VDEC_ES_BUF_S *pstEsBuf)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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

HI_S32 HI_DRV_VDEC_PutEsBuf(HI_HANDLE hHandle, VDEC_ES_BUF_S *pstEsBuf)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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

static HI_VOID VDEC_ConvertFrm(HI_UNF_VCODEC_TYPE_E enType, VDEC_CHANNEL_S *pstChan,
                               IMAGE *pstImage, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    UINT32 u32fpsInteger, u32fpsDecimal;
    HI_VDEC_PRIV_FRAMEINFO_S *pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S *)(((HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv))->u32Reserve);
    HI_DRV_VIDEO_PRIVATE_S *pstVideoPriv = (HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv);

    if ((pstImage->format & 0x3000) != 0)
    {
        pstFrame->bTopFieldFirst = HI_TRUE;
    }
    else
    {
        pstFrame->bTopFieldFirst = HI_FALSE;
    }

    /* Don't use 0x1C000 bits. */

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

    if (HI_UNF_VCODEC_TYPE_H263 == enType || HI_UNF_VCODEC_TYPE_SORENSON == enType)
    {
        pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21;
    }
    else if (HI_UNF_VCODEC_TYPE_MJPEG == enType)
    {
        switch (pstImage->BTLInfo.YUVFormat)
        {
            case SPYCbCr400:
                pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV08;
                break;

            case SPYCbCr411:
                pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV12_411;
                break;

            case SPYCbCr422_1X2:
                pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV16;
                break;

            case SPYCbCr422_2X1:
                pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV61_2X1;
                break;

            case SPYCbCr444:
                pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV24;
                break;

            case PLNYCbCr400:
                pstFrame->ePixFormat = HI_DRV_PIX_FMT_YUV400;
                break;

            case PLNYCbCr411:
                pstFrame->ePixFormat = HI_DRV_PIX_FMT_YUV411;
                break;

            case PLNYCbCr420:
                pstFrame->ePixFormat = HI_DRV_PIX_FMT_YUV420p;
                break;

            case PLNYCbCr422_1X2:
                pstFrame->ePixFormat = HI_DRV_PIX_FMT_YUV422_1X2;
                break;

            case PLNYCbCr422_2X1:
                pstFrame->ePixFormat = HI_DRV_PIX_FMT_YUV422_2X1;
                break;

            case PLNYCbCr444:
                pstFrame->ePixFormat = HI_DRV_PIX_FMT_YUV_444;
                break;

            case PLNYCbCr410:
                pstFrame->ePixFormat = HI_DRV_PIX_FMT_YUV410p;
                break;

            case SPYCbCr420:
            default:
                pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21;
                break;
        }
    }
    else
    {
        if (0 == EnVcmp)
        {
            switch ((pstImage->format >> 2) & 7)
            {
                case 0:
                    pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21_TILE;
                    break;

                case 1:
                    pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21_TILE;
                    break;

                default:
                    pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21_TILE;
                    break;
            }
        }
        else
        {
            switch ((pstImage->format >> 2) & 7)
            {
                case 0:
                    pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21_TILE_CMP;
                    break;

                case 1:
                    pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21_TILE_CMP;
                    break;

                default:
                    pstFrame->ePixFormat = HI_DRV_PIX_FMT_NV21_TILE_CMP;
                    break;
            }
        }
    }

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

    if (enType == HI_UNF_VCODEC_TYPE_HEVC)
    {
        pstFrame->enBufValidMode = HI_DRV_FIELD_ALL;
    }
    else if (enType == HI_UNF_VCODEC_TYPE_H264)
    {
        pstFrame->enBufValidMode = pstFrame->enFieldMode;
    }

    if (HI_UNF_VCODEC_TYPE_HEVC == enType)
    {
        if ((pstImage->bit_depth_luma > 8) || (pstImage->bit_depth_chroma > 8))
        {
            pstFrame->enBitWidth =  HI_DRV_PIXEL_BITWIDTH_10BIT;
            //HI_ERR_VDEC("========VPSS Receive Frame ,This Frame is  10Bit\n");
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


    if (HI_UNF_VCODEC_TYPE_MJPEG == enType || HI_UNF_VCODEC_TYPE_SORENSON == enType || HI_UNF_VCODEC_TYPE_H263 == enType)
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
    }
    else
    {
        if (0 == EnVcmp)
        {
            pstFrame->stBufAddr[0].u32PhyAddr_Y        = pstImage->top_luma_phy_addr;
            pstFrame->stBufAddr[0].u32Stride_Y         = pstImage->image_stride / 16;
            pstFrame->stBufAddr[0].u32PhyAddr_C        = pstImage->top_chrom_phy_addr;
            pstFrame->stBufAddr[0].u32Stride_C         = pstFrame->stBufAddr[0].u32Stride_Y;

            if ((HI_UNF_VCODEC_TYPE_HEVC == enType) && (HI_DRV_PIXEL_BITWIDTH_10BIT == pstFrame->enBitWidth))
            {
                pstFrame->stBufAddr_LB[0].u32PhyAddr_Y = pstImage->luma_2bit_phy_addr;
                pstFrame->stBufAddr_LB[0].u32Stride_Y = pstImage->image_stride_2bit;
                pstFrame->stBufAddr_LB[0].u32PhyAddr_C = pstImage->chrom_2bit_phy_addr;
                pstFrame->stBufAddr_LB[0].u32Stride_C = pstImage->image_stride_2bit;
            }
        }
        else
        {
            pstFrame->stBufAddr[0].u32PhyAddr_YHead    = pstImage->top_luma_phy_addr;
            pstFrame->stBufAddr[0].u32PhyAddr_Y        = pstImage->top_luma_phy_addr + (pstImage->image_height + 31) / 32 * 32 * 16;
            pstFrame->stBufAddr[0].u32Stride_Y         = pstImage->image_stride / 16;
            pstFrame->stBufAddr[0].u32PhyAddr_CHead    = pstImage->top_chrom_phy_addr;
            pstFrame->stBufAddr[0].u32PhyAddr_C        = pstImage->top_chrom_phy_addr + (pstImage->image_height + 31) / 32 * 32 * 8;
            pstFrame->stBufAddr[0].u32Stride_C         = pstFrame->stBufAddr[0].u32Stride_Y;
        }
    }

    //FOR MVC DEBUG
    if (pstImage->is_3D)
    {
        pstFrame->stBufAddr[1].u32PhyAddr_YHead       = pstImage->top_luma_phy_addr_1;
        pstFrame->stBufAddr[1].u32PhyAddr_Y           = pstImage->top_luma_phy_addr_1 + (pstImage->image_height + 31) / 32 * 32 * 16;
        pstFrame->stBufAddr[1].u32Stride_Y            = pstImage->image_stride / 16;

        pstFrame->stBufAddr[1].u32PhyAddr_CHead       = pstImage->top_chrom_phy_addr_1;
        pstFrame->stBufAddr[1].u32PhyAddr_C           = pstImage->top_chrom_phy_addr_1 + (pstImage->image_height + 31) / 32 * 32 * 8;
        pstFrame->stBufAddr[1].u32Stride_C            = pstFrame->stBufAddr[1].u32Stride_Y;

        if (0 == EnVcmp)
        {
            pstFrame->stBufAddr[1].u32PhyAddr_Y       = pstImage->top_luma_phy_addr_1;
            pstFrame->stBufAddr[1].u32PhyAddr_C       = pstImage->top_chrom_phy_addr_1;
        }
    }

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
    }
    else
    {
        pstFrame->u32Circumrotate = 0;
    }

    pstFrame->u32AspectWidth                   = pstImage->u32AspectWidth;
    pstFrame->u32AspectHeight                  = pstImage->u32AspectHeight;
    pstFrame->u32Width                         = (HI_U32)pstImage->disp_width;
    pstFrame->u32Height                        = (HI_U32)pstImage->disp_height;
    pstFrame->stDispRect.s32Width              = (HI_S32)pstImage->disp_width;
    pstFrame->stDispRect.s32Height             = (HI_S32)pstImage->disp_height;
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

    if (pstFrame->u32Height <= 288 && enType != HI_UNF_VCODEC_TYPE_HEVC)
    {
        pstFrame->bProgressive = HI_TRUE;
    }

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

    if (pstFrame->u32FrameRate < 1000)
    {
        pstFrame->u32FrameRate = 25000;
    }

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
#endif
    }
    else
    {
        pstFrame->u32TunnelPhyAddr = 0;
    }

    pstPrivInfo->entype = enType;
#ifdef HI_TEE_SUPPORT

    /* Check if is secure frame */
    if (pstImage->is_SecureFrame != 0)
    {
        pstFrame->bSecure = HI_TRUE;
    }
    else
    {
        pstFrame->bSecure = HI_FALSE;
    }

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

    ((HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv))->u32PrivDispTime = pstImage->DispTime;
    ((HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv))->u32PlayTime = pstImage->u32RepeatCnt;
    ((HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv))->eColorSpace  = HI_DRV_CS_BT601_YUV_LIMITED;
    pstVideoPriv->stVideoOriginalInfo.enSource = HI_DRV_SOURCE_DTV;
    pstVideoPriv->stVideoOriginalInfo.u32Width = pstImage->disp_width;
    pstVideoPriv->stVideoOriginalInfo.u32Height = pstImage->disp_height;
    pstVideoPriv->stVideoOriginalInfo.u32FrmRate = pstFrame->u32FrameRate;
    pstVideoPriv->stVideoOriginalInfo.en3dType = pstFrame->eFrmType;
    pstVideoPriv->stVideoOriginalInfo.enSrcColorSpace = HI_DRV_CS_BT601_YUV_LIMITED;
    pstVideoPriv->stVideoOriginalInfo.enColorSys = HI_DRV_COLOR_SYS_AUTO;
    pstVideoPriv->stVideoOriginalInfo.bGraphicMode = HI_FALSE;
    pstVideoPriv->stVideoOriginalInfo.bInterlace = pstFrame->bProgressive;
}

#ifdef Hi3716MV410
HI_S32 VDEC_AllocMMZMem(HI_S32 chanID, HI_S32 frameNum)
{
    HI_S32 ret = HI_SUCCESS;
    HI_S32 i = 0;
    HI_S32 singleFrameSize = 0;
    HI_S32 allocatedFrameNum = 0;

    VDEC_CHANNEL_S *pstChan = NULL;

    MMZ_BUFFER_S *memRecord = NULL;

    pstChan = s_stVdecDrv.astChanEntity[chanID].pstChan;

    singleFrameSize = pstChan->u32FrameSize;

    //HI_ERR_VDEC("MMZ_FRAME_NUM: %d\n", frameNum);

    //for (i = 0; i < MAX_FRAME_NUM; i++)
    for (i = 0; i < 64; i++)
    {
        //HI_ERR_VDEC("allocatedFrameNum = %d\n", allocatedFrameNum);
        //HI_ERR_VDEC("bufferNodeStatus = %d\n", pstChan->frmBufArr[i].bufferNodeStatus);

        if ((0xff == pstChan->frmBufArr[i].bufferNodeStatus) && (allocatedFrameNum < frameNum))
        {
            memRecord = &(pstChan->frmBufArr[i].frameBuffer);
            ret = HI_DRV_MMZ_AllocAndMap("VFMW_FRM_BUF", "VFMW", singleFrameSize, 0, memRecord);

            if (HI_SUCCESS != ret)
            {
                break; // 分配失败
            }
            else
            {
                pstChan->frmBufArr[i].isAvailable = 1;
                pstChan->frmBufArr[i].bufferNodeStatus = 1;

                VDEC_AddFrmBufRec(chanID, memRecord);
            }

            allocatedFrameNum++;
        }
        else if (allocatedFrameNum >= frameNum)
        {
            break;
        }

        //HI_ERR_VDEC("bufferNodeStatus = %d\n", pstChan->frmBufArr[i].bufferNodeStatus);
    }

    if (allocatedFrameNum < frameNum)
    {
        HI_ERR_VDEC("DFS, memory is not enough!\n");

        if (HI_SUCCESS != VDEC_ReleaseAllFrameNode(chanID))
        {
            HI_ERR_VDEC("DFS, release all frame node failed!\n");
        }
    }

    return ret;
}

HI_S32 VDEC_AddFrmBufRec(HI_S32 chanID, MMZ_BUFFER_S *frmBufRec)
{
    UINT32 i = 0;

    VDEC_CHANNEL_S *pstChan = NULL;

    if (frmBufRec == NULL)
    {
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[chanID].pstChan;

    VDEC_FrmBufRecSpinLockIRQ(&(pstChan->frameBufRecLock));

    for (i = 0; i < 64; i++)
    {
        if (pstChan->frmBufRec[i].u32Size == 0)
        {
            break;
        }
    }

    if (i < 64)
    {
        pstChan->frmBufRec[i].u32StartPhyAddr = frmBufRec->u32StartPhyAddr;
        pstChan->frmBufRec[i].pu8StartVirAddr = frmBufRec->pu8StartVirAddr;
        pstChan->frmBufRec[i].u32Size         = frmBufRec->u32Size;

        VDEC_FrmBufRecSpinUnLockIRQ(&(pstChan->frameBufRecLock));

        return HI_SUCCESS;
    }
    else
    {
        VDEC_FrmBufRecSpinUnLockIRQ(&(pstChan->frameBufRecLock));

        return HI_FAILURE;
    }
}

HI_S32 VDEC_DelFrmBufRec(HI_S32 chanID, MMZ_BUFFER_S *frmBufRec)
{
    HI_U32 i = 0;
    VDEC_CHANNEL_S *pstChan = NULL;

    if (frmBufRec == NULL)
    {
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[chanID].pstChan;

    VDEC_FrmBufRecSpinLockIRQ(&(pstChan->frameBufRecLock));

    for (i = 0; i < 64; i++)
    {
        if ((pstChan->frmBufRec[i].u32StartPhyAddr == frmBufRec->u32StartPhyAddr)
            && (pstChan->frmBufRec[i].pu8StartVirAddr == frmBufRec->pu8StartVirAddr)
            && (pstChan->frmBufRec[i].u32Size == frmBufRec->u32Size))
        {
            pstChan->frmBufRec[i].u32StartPhyAddr = 0;
            pstChan->frmBufRec[i].pu8StartVirAddr = 0;
            pstChan->frmBufRec[i].u32Size         = 0;

            VDEC_FrmBufRecSpinUnLockIRQ(&(pstChan->frameBufRecLock));

            return HI_SUCCESS;
        }
    }

    VDEC_FrmBufRecSpinUnLockIRQ(&(pstChan->frameBufRecLock));

    return HI_FAILURE;

}

static HI_S32 VDEC_RefreshConfiguredFrameBuf(HI_S32 chanID, HI_U32 phyAddr)
{
    HI_U32 i = 0;

    VDEC_CHANNEL_S *pstChan = NULL;

    FRAME_BUF_S stFrameBuf;

    SINT32 ret = HI_SUCCESS;

    UINT32 u32NeededFrameNum = 0;

    pstChan = s_stVdecDrv.astChanEntity[chanID].pstChan;

    if (pstChan == NULL)
    {
        return HI_FAILURE;
    }

    //HI_ERR_VDEC("FUNCTION: %s\tLINE: %d\n", __FUNCTION__, __LINE__);

    u32NeededFrameNum = (pstChan->stOption.u32CfgFrameNum + pstChan->stOption.s32ExtraFrameStoreNum);

    for (i = 0; i < 64; i++)
    {
        if ((pstChan->specialFrameExist)
            && (pstChan->specialFramePhyAddr >= pstChan->frmBufRec[i].u32StartPhyAddr)
            && (pstChan->specialFramePhyAddr < pstChan->frmBufRec[i].u32StartPhyAddr + pstChan->frmBufRec[i].u32Size))
        {
            //HI_ERR_VDEC("Release specical frame buffer\n", __FUNCTION__, __LINE__);
            //HI_ERR_VDEC("pstChan->frmBufRec[i].u32StartPhyAddr = 0x%x\n", pstChan->frmBufRec[i].u32StartPhyAddr);
            //HI_ERR_VDEC("pstChan->frmBufRec[i].u32StartVirAddr = 0x%x\n", pstChan->frmBufRec[i].u32StartVirAddr);
            //HI_ERR_VDEC("pstChan->frmBufRec[i].u32Size         = 0x%x\n", pstChan->frmBufRec[i].u32Size);

            HI_DRV_MMZ_UnmapAndRelease(&(pstChan->frmBufRec[i]));

            VDEC_DelFrmBufRec(chanID, &(pstChan->frmBufRec[i]));

            break;
        }
    }

    if (i == 64)
    {
        for (i = 0; i < 64; i++)
        {
            if (!(((pstChan->frmBufArr[i].frameBuffer.u32StartPhyAddr + pstChan->frmBufArr[i].frameBuffer.u32Size) <= pstChan->specialFramePhyAddr)
                  || ((pstChan->specialFramePhyAddr + pstChan->specialFrameSize) <= pstChan->frmBufArr[i].frameBuffer.u32StartPhyAddr)))
            {
                if ((pstChan->frmBufArr[i].bufferNodeStatus != 0xff) && (pstChan->frmBufArr[i].isAvailable == 0))
                {
                    // 将该帧配置下去，并标记为已配置、可用
                    stFrameBuf.u32StartPhyAddr = pstChan->frmBufArr[i].frameBuffer.u32StartPhyAddr;
                    stFrameBuf.u32StartVirAddr = (HI_U32)pstChan->frmBufArr[i].frameBuffer.pu8StartVirAddr;
                    stFrameBuf.u32Size         = pstChan->u32FrameSize;
                    stFrameBuf.bufNodeStatus   = pstChan->frmBufArr[i].bufferNodeStatus;
                    stFrameBuf.frameNum        = u32NeededFrameNum;

                    //ret = VDEC_Control(pstChan->hChan, VDEC_CID_ALLOC_MEM_TO_DECODER, (HI_VOID *)&stFrameBuf);
                    ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_ALLOC_MEM_TO_DECODER, (HI_VOID *)&stFrameBuf);

                    if (ret == HI_SUCCESS)
                    {
                        pstChan->frmBufArr[i].isConfigured = 1;
                        pstChan->frmBufArr[i].isAvailable = 1;
                        pstChan->configuredFrameNum++;
                    }
                }
            }
        }

    }

#if 0

    if (pstChan->configuredFrameNum < pstChan->needFrameNum) // 之前预分配帧存中有被后级占用导致无法配置下去的情况
    {
        HI_ERR_VDEC("FUNCTION: %s\tLINE: %d\n", __FUNCTION__, __LINE__);
        HI_ERR_VDEC("pstChan->configuredFrameNum = %d \t pstChan->needFrameNum = %d\n", pstChan->configuredFrameNum, pstChan->needFrameNum);

        for (i = 0; i < 64; i++)
        {
            HI_ERR_VDEC("FUNCTION: %s\tLINE: %d\n", __FUNCTION__, __LINE__);
            HI_ERR_VDEC("pstChan->frmBufArr[%d].bufferNodeStatus = %d\n", i, pstChan->frmBufArr[i].bufferNodeStatus);
            HI_ERR_VDEC("pstChan->frmBufArr[%d].isAvailable = %d\n", i, pstChan->frmBufArr[i].isAvailable);

            if ((pstChan->frmBufArr[i].bufferNodeStatus != 0xff) && (pstChan->frmBufArr[i].isAvailable == 0))
            {
                HI_ERR_VDEC("FUNCTION: %s\tLINE: %d\n", __FUNCTION__, __LINE__);
                // 将该帧配置下去，并标记为已配置、可用
                stFrameBuf.u32StartPhyAddr = pstChan->frmBufArr[i].frameBuffer.u32StartPhyAddr;
                stFrameBuf.u32StartVirAddr = pstChan->frmBufArr[i].frameBuffer.u32StartVirAddr;
                stFrameBuf.u32Size         = pstChan->u32FrameSize;
                stFrameBuf.bufNodeStatus   = pstChan->frmBufArr[i].bufferNodeStatus;
                stFrameBuf.frameNum        = u32NeededFrameNum;

                ret = VDEC_Control(pstChan->hChan, VDEC_CID_ALLOC_MEM_TO_DECODER, (HI_VOID *)&stFrameBuf);

                if (ret == HI_SUCCESS)
                {
                    HI_ERR_VDEC("FUNCTION: %s\tLINE: %d\n", __FUNCTION__, __LINE__);
                    pstChan->frmBufArr[i].isConfigured = 1;
                    pstChan->frmBufArr[i].isAvailable = 1;
                    pstChan->configuredFrameNum++;
                }
            }
        }
    }
    else // 后级占用的变分辨率前的一帧是动态分配的帧存，在这里释放
    {
        HI_ERR_VDEC("FUNCTION: %s\tLINE: %d\n", __FUNCTION__, __LINE__);

        for (i = 0; i < 64; i++)
        {
            HI_ERR_VDEC("FUNCTION: %s\tLINE: %d\n", __FUNCTION__, __LINE__);
            HI_ERR_VDEC("@@@ Special_Frame PhyAddr = 0x%x\n", phyAddr);

            if ((phyAddr >= pstChan->frmBufRec[i].u32StartPhyAddr)
                && (phyAddr < pstChan->frmBufRec[i].u32StartPhyAddr + pstChan->frmBufRec[i].u32Size))
            {
                HI_ERR_VDEC("FUNCTION: %s\tLINE: %d\n", __FUNCTION__, __LINE__);
                HI_ERR_VDEC("pstChan->frmBufRec[i].u32StartPhyAddr = 0x%x\n", pstChan->frmBufRec[i].u32StartPhyAddr);
                HI_ERR_VDEC("pstChan->frmBufRec[i].u32StartVirAddr = 0x%x\n", pstChan->frmBufRec[i].u32StartVirAddr);
                HI_ERR_VDEC("pstChan->frmBufRec[i].u32Size         = 0x%x\n", pstChan->frmBufRec[i].u32Size);

                HI_DRV_MMZ_UnmapAndRelease(&(pstChan->frmBufRec[i]));

                VDEC_DelFrmBufRec(chanID, &(pstChan->frmBufRec[i]));

                break;
            }
        }
    }

#endif

    //HI_ERR_VDEC("FUNCTION: %s\tLINE: %d\n", __FUNCTION__, __LINE__);

    pstChan->specialFrameExist = HI_FAILURE;

    return ret;

}

HI_S32 VDEC_ReleaseAllFrameNode(HI_S32 chanID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_S32 i   = 0;

    MMZ_BUFFER_S *memRecord = NULL;

    VDEC_CHANNEL_S *pstChan = NULL;

    pstChan = s_stVdecDrv.astChanEntity[chanID].pstChan;

    //for (i = 0; i < MAX_FRAME_NUM; i++)
    for (i = 0; i < 64; i++)
    {
        if (0xff != pstChan->frmBufArr[i].bufferNodeStatus)
        {
            memRecord = &(pstChan->frmBufArr[i].frameBuffer);

            if (1 == pstChan->frmBufArr[i].bufferNodeStatus)
            {
                HI_DRV_MMZ_UnmapAndRelease(memRecord);
            }

            pstChan->frmBufArr[i].bufferNodeStatus = 0xff;
            pstChan->frmBufArr[i].isAvailable = 0;
            pstChan->frmBufArr[i].isConfigured = 0;
        }
    }

    return ret;

}

HI_S32 VDEC_ResetChanFrmBuf(HI_S32 chanID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 i = 0;
    VDEC_CHANNEL_S *pstChan = NULL;

    pstChan = s_stVdecDrv.astChanEntity[chanID].pstChan;

    memset(pstChan->frmBufArr, 0, sizeof(CHAN_FRAME_BUFFER_S) * 64);

    for (i = 0; i < 64; i++)
    {
        pstChan->frmBufArr[i].bufferNodeStatus = 0xff;
    }

    return ret;
}

HI_S32 VDEC_AllocPREMem(HI_S32 chanID, HI_S32 frameNum)
{
    HI_S32 i = 0;
    HI_S32 allocatedFrameNum = 0;
    HI_S32 ret = HI_SUCCESS;
    HI_S32 startPhyAddr = 0;
    HI_S32 startVirAddr = 0;
    VDEC_CHANNEL_S *pstChan = NULL;

    pstChan = s_stVdecDrv.astChanEntity[chanID].pstChan;

    //HI_ERR_VDEC("FUNCTION: %s\tLINE: %d\tframeNum = %d\tspecialFrameExist = %d\n", __FUNCTION__, __LINE__, frameNum, pstChan->specialFrameExist);

    if (pstChan->specialFrameExist) // 存在后级占用一帧的情况
    {
        // 如果占用的一帧是在预分配的开头部分并且占用的那一帧的大小大于等于新分辨率的总帧存大小
        if ((pstChan->specialFramePhyAddr == pstChan->stVDHMMZBuf.u32StartPhyAddr)
            && (pstChan->specialFrameSize >= (pstChan->needFrameNum * pstChan->u32FrameSize)))
        {
            //for (i = 0; i < MAX_FRAME_NUM; i++)
            for (i = 0; i < 64; i++)
            {
                if ((0xff == pstChan->frmBufArr[i].bufferNodeStatus) && (allocatedFrameNum < frameNum))
                {
                    pstChan->frmBufArr[i].frameBuffer.u32StartPhyAddr = pstChan->specialFramePhyAddr;
                    pstChan->frmBufArr[i].frameBuffer.pu8StartVirAddr = (HI_U8*)pstChan->specialFrameVirAddr;
                    pstChan->frmBufArr[i].frameBuffer.u32Size = pstChan->u32FrameSize;

                    pstChan->frmBufArr[i].isAvailable = 0; // 不可用
                    pstChan->frmBufArr[i].bufferNodeStatus = 0; // 预分配
                    pstChan->frmBufArr[i].isConfigured = 0;
                    allocatedFrameNum++;
                    break;
                }
            }

            //if (i == MAX_FRAME_NUM)
            if (i == 64)
            {
                HI_ERR_VDEC("Not find free node!\n");
            }

            startPhyAddr = pstChan->specialFramePhyAddr + pstChan->specialFrameSize;
            startVirAddr = pstChan->specialFrameVirAddr + pstChan->specialFrameSize;

            //for (i = 0; i < MAX_FRAME_NUM; i++)
            for (i = 0; i < 64; i++)
            {
                if ((0xff == pstChan->frmBufArr[i].bufferNodeStatus) && (allocatedFrameNum < frameNum))
                {
                    pstChan->frmBufArr[i].frameBuffer.u32StartPhyAddr = startPhyAddr + (allocatedFrameNum - 1) * pstChan->u32FrameSize;
                    pstChan->frmBufArr[i].frameBuffer.pu8StartVirAddr = (HI_U8*)(startVirAddr + (allocatedFrameNum - 1) * pstChan->u32FrameSize);
                    pstChan->frmBufArr[i].frameBuffer.u32Size         = pstChan->u32FrameSize;

                    pstChan->frmBufArr[i].isAvailable = 1;
                    pstChan->frmBufArr[i].bufferNodeStatus = 0;
                    pstChan->frmBufArr[i].isConfigured = 0;

                    allocatedFrameNum++;
                }
                else if (allocatedFrameNum == frameNum)
                {
                    break;
                }
            }

            //if ((i == MAX_FRAME_NUM) && (allocatedFrameNum < frameNum))
            if ((i == 64) && (allocatedFrameNum < frameNum))
            {
                HI_ERR_VDEC("Not enough frame node, release all node!\n");

                if (HI_SUCCESS != VDEC_ReleaseAllFrameNode(chanID))
                {
                    HI_ERR_VDEC("Release all node failed!\n");
                }

                return HI_FAILURE;
            }
            else
            {
                return HI_SUCCESS;
            }
        }
        else
        {
            startPhyAddr = pstChan->stVDHMMZBuf.u32StartPhyAddr;
            startVirAddr = (HI_S32)pstChan->stVDHMMZBuf.pu8StartVirAddr;

            //for (i = 0; i < MAX_FRAME_NUM; i++)
            for (i = 0; i < 64; i++)
            {
                if ((0xff == pstChan->frmBufArr[i].bufferNodeStatus) && (allocatedFrameNum < frameNum))
                {
                    pstChan->frmBufArr[i].frameBuffer.u32StartPhyAddr = startPhyAddr + (allocatedFrameNum * pstChan->u32FrameSize);
                    pstChan->frmBufArr[i].frameBuffer.pu8StartVirAddr = (HI_U8*)(startVirAddr + (allocatedFrameNum * pstChan->u32FrameSize));
                    pstChan->frmBufArr[i].frameBuffer.u32Size         = pstChan->u32FrameSize;

                    if (((pstChan->frmBufArr[i].frameBuffer.u32StartPhyAddr + pstChan->frmBufArr[i].frameBuffer.u32Size) <= pstChan->specialFramePhyAddr)
                        || ((pstChan->specialFramePhyAddr + pstChan->specialFrameSize) <= pstChan->frmBufArr[i].frameBuffer.u32StartPhyAddr))
                    {
                        pstChan->frmBufArr[i].isAvailable = 1; // 该帧存没有被后级持有的帧占用，可以配置给解码器使用
                    }
                    else
                    {
                        pstChan->frmBufArr[i].isAvailable = 0; // 该帧存被后级持有的帧占用，不可以配置给解码器使用
                    }

                    pstChan->frmBufArr[i].bufferNodeStatus = 0;
                    pstChan->frmBufArr[i].isConfigured = 0;

                    allocatedFrameNum++;

                }
                else if (allocatedFrameNum == frameNum)
                {
                    break;
                }
            }

            //if ((i == MAX_FRAME_NUM) && (allocatedFrameNum < frameNum))
            if ((i == 64) && (allocatedFrameNum < frameNum))
            {
                HI_ERR_VDEC("Not enough frame node, release all node!\n");

                if (HI_SUCCESS != VDEC_ReleaseAllFrameNode(chanID))
                {
                    HI_ERR_VDEC("Release all node failed!\n");
                }

                return HI_FAILURE;
            }
            else
            {
                return HI_SUCCESS;
            }

        }

    }
    else // 不存在后级占用一帧的情况
    {
        startPhyAddr = pstChan->stVDHMMZBuf.u32StartPhyAddr;
        startVirAddr = (HI_S32)pstChan->stVDHMMZBuf.pu8StartVirAddr;

        //HI_ERR_VDEC("PRE_FRAME_NUM = %d\n", frameNum);

        //for (i = 0; i < MAX_FRAME_NUM; i++)
        for (i = 0; i < 64; i++)
        {
            //HI_ERR_VDEC("allocatedFrameNum = %d\n", allocatedFrameNum);
            //HI_ERR_VDEC("bufferNodeStatus = %d\n", pstChan->frmBufArr[i].bufferNodeStatus);

            if ((0xff == pstChan->frmBufArr[i].bufferNodeStatus) && (allocatedFrameNum < frameNum))
            {
                pstChan->frmBufArr[i].frameBuffer.u32StartPhyAddr = startPhyAddr + (allocatedFrameNum * pstChan->u32FrameSize);
                pstChan->frmBufArr[i].frameBuffer.pu8StartVirAddr = (HI_U8*)(startVirAddr + (allocatedFrameNum * pstChan->u32FrameSize));
                pstChan->frmBufArr[i].frameBuffer.u32Size         = pstChan->u32FrameSize;

                //HI_ERR_VDEC("u32StartPhyAddr = 0x%x\n", pstChan->frmBufArr[i].frameBuffer.u32StartPhyAddr);
                //HI_ERR_VDEC("u32StartVirAddr = 0x%x\n", pstChan->frmBufArr[i].frameBuffer.u32StartVirAddr);
                //HI_ERR_VDEC("u32Size         = 0x%x\n", pstChan->frmBufArr[i].frameBuffer.u32Size);

                pstChan->frmBufArr[i].isAvailable = 1; // 该帧存没有被后级持有的帧占用，可以配置给解码器使用
                pstChan->frmBufArr[i].bufferNodeStatus = 0;
                pstChan->frmBufArr[i].isConfigured = 0;

                allocatedFrameNum++;
            }
            else if (allocatedFrameNum == frameNum)
            {
                break;
            }
        }

        //if ((i == MAX_FRAME_NUM) && (allocatedFrameNum < frameNum))
        if ((i == 64) && (allocatedFrameNum < frameNum))
        {
            HI_ERR_VDEC("Not enough frame node, release all node!\n");

            if (HI_SUCCESS != VDEC_ReleaseAllFrameNode(chanID))
            {
                HI_ERR_VDEC("Release all node failed!\n");
            }

            return HI_FAILURE;
        }
        else
        {
            return HI_SUCCESS;
        }
    }

    return ret;
}

HI_S32 VDEC_SetChanControlState(HI_S32 chanID, VDEC_CHANNEL_CONTROL_STATE_E chanControlState)
{
    HI_S32 ret = HI_SUCCESS;

    VDEC_CHANNEL_S *pstChan = NULL;

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
#endif

static HI_S32 VDEC_RlsFrm(VDEC_CHANNEL_S *pstChan, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_S32 s32Ret;
    IMAGE stImage;
    IMAGE_INTF_S *pstImgInft = &pstChan->stImageIntf;
    VDEC_CHAN_STATINFO_S *pstStatInfo = &pstChan->stStatInfo;
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
}


static HI_S32 VDEC_CheckConvertFrm(VDEC_CHANNEL_S *pstChan, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    if (0 == pstFrame->u32Height || 0 == pstFrame->u32Width)
    {
        (HI_VOID)VDEC_RlsFrm(pstChan, pstFrame);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


static HI_S32 VDEC_RecvFastBackFrm(HI_HANDLE hHandle, IMAGE_INTF_S *pstImgInft, IMAGE *pstImage)
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
                if (((pstImage->is_fld_save == 0) && (0 == (pstImage->format & 0x03))) ||
                    ((pstImage->is_fld_save == 1) && ((0 == (pstImage->top_fld_type & 0x03)) || (0 == (pstImage->bottom_fld_type & 0x03)))))
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
static HI_S32 VDEC_RecvFrm(HI_HANDLE hHandle, VDEC_CHANNEL_S *pstChan, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32UserdataId;
    IMAGE stImage;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;
    VDEC_CHAN_STATE_S stChanState = {0};
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
#if (1 == HI_VDEC_CC_FROM_IMAGE)
    HI_U32 u32ID;
    HI_U8 u8Type;
    HI_U32 u32Index;
    USRDAT *pstUsrData = HI_NULL;
#endif
#endif
    IMAGE_INTF_S *pstImgInft = &pstChan->stImageIntf;
    VDEC_CHAN_STATINFO_S *pstStatInfo = &pstChan->stStatInfo;
    HI_VDEC_PRIV_FRAMEINFO_S *pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S *)(((HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv))->u32Reserve);
    HI_VDEC_PRIV_FRAMEINFO_S *pstLastFrmPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S *)(((HI_DRV_VIDEO_PRIVATE_S *)(pstChan->stLastDispFrameInfo.u32Priv))->u32Reserve);
    HI_DRV_VIDEO_PRIVATE_S *pstVideoPriv = (HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv);
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
    pstStatInfo->u32VdecRcvFrameTry++;

    if (s_stVdecDrv.astChanEntity[hHandle].stVpssChan.s32Speed < 0)
    {
        s32Ret = VDEC_RecvFastBackFrm(hHandle, pstImgInft, &stImage);

        if (HI_SUCCESS != s32Ret)
        {
            VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;
            pstVpssChan = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan);
            //(void)(s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(pstVpssChan->hVpss,HI_DRV_VPSS_USER_COMMAND_IMAGEREADY, NULL);
        }
    }
    else
    {
        s32Ret = pstImgInft->read_image(pstImgInft->image_provider_inst_id, &stImage);
    }

    if (VDEC_OK != s32Ret)
    {
        /* If last frame decode fail, retry 5 times */
        if (((pstChan->u32EndFrmFlag == 1) && (pstChan->u32LastFrmTryTimes++ >= 4)) ||
            /* If report last frame id after this frame had been outputed, check last frame id */
            ((pstChan->u32EndFrmFlag == 2) && (pstLastFrmPrivInfo->image_id % 100 == pstChan->u32LastFrmId)) || //(HI_VDEC_PRIV_FRAMEINFO_S*)pstFrame->u32Priv
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

            if (HI_TRUE == pstChan->bTvp)
            {
                pstFrame->bSecure = HI_TRUE;
            }
            else
            {
                pstFrame->bSecure = HI_FALSE;
            }

#endif
            pstVideoPriv->u32LastFlag = DEF_HI_DRV_VPSS_LAST_ERROR_FLAG;

            return HI_SUCCESS;
        }

        return HI_FAILURE;
    }

    pstChan->u32LastFrmTryTimes = 0;

    pstStatInfo->u32VdecRcvFrameOK++;
    pstStatInfo->u32TotalVdecOutFrame++;

    /* Calculate PTS */
    PTSREC_CalcStamp(hHandle, pstChan->stCurCfg.enType, &stImage);

#ifdef Hi3716MV410

    if (g_CHAN_FRAME_RATE[hHandle] != 0)
    {
        stImage.frame_rate = g_CHAN_FRAME_RATE[hHandle] * 1024;
    }

#endif

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
        pstChan->pu8UsrDataForWaterMark[u32UserdataId] = stImage.p_usrdat[u32UserdataId];
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
        USRDATA_Arrange(hHandle, stImage.p_usrdat[0]);
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

#if 1

    if ((pstFrame->stDispRect.s32Height != pstChan->stLastFrm.stDispRect.s32Height) ||
        (pstFrame->stDispRect.s32Width != pstChan->stLastFrm.stDispRect.s32Width))
    {
        pstChan->bNormChange                      = HI_TRUE;
        pstChan->stNormChangeParam.enNewFormat    = pstChan->enDisplayNorm;
        pstChan->stNormChangeParam.u32ImageWidth  = pstFrame->stDispRect.s32Width & 0xfffffffe;
        pstChan->stNormChangeParam.u32ImageHeight = pstFrame->stDispRect.s32Height & 0xfffffffc;
        pstChan->stNormChangeParam.u32FrameRate   = pstFrame->u32FrameRate;
        pstChan->stNormChangeParam.bProgressive   = pstFrame->bProgressive;
    }

    /*set framePackingType for pstFrame*/
    pstVpssChan = &(s_stVdecDrv.astChanEntity[hHandle].stVpssChan);

    if (HI_UNF_FRAME_PACKING_TYPE_BUTT != pstVpssChan->eFramePackType)
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

                if (stImage.disp_width / 2 > stImage.disp_height)
                {
                    pstFrame->u32AspectWidth = stImage.disp_width / 2;
                    pstFrame->u32AspectHeight = stImage.disp_height;
                }
                else
                {
                    pstFrame->u32AspectWidth = stImage.disp_width;
                    pstFrame->u32AspectHeight = stImage.disp_height;
                }

                s_stVdecDrv.astChanEntity[hHandle].pstChan->u32UserSetAspectWidth = pstFrame->u32AspectWidth;
                s_stVdecDrv.astChanEntity[hHandle].pstChan->u32UserSetAspectHeight = pstFrame->u32AspectHeight;
                break;

            case HI_UNF_FRAME_PACKING_TYPE_TOP_AND_BOTTOM:   /**< Top and bottom */
                pstFrame->eFrmType = HI_DRV_FT_TAB;

                if (stImage.disp_width < stImage.disp_height)
                {
                    pstFrame->u32AspectWidth = stImage.disp_width;
                    pstFrame->u32AspectHeight = stImage.disp_height / 2;
                }
                else
                {
                    pstFrame->u32AspectWidth = stImage.disp_width;
                    pstFrame->u32AspectHeight = stImage.disp_height;
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
    }

    if (pstFrame->eFrmType != pstChan->stLastFrm.eFrmType)
    {
        pstChan->bFramePackingChange = HI_TRUE;
        pstChan->enFramePackingType = VDEC_ConverFrameType(pstFrame->eFrmType);
    }

    memcpy(&(pstChan->stLastFrm), pstFrame, sizeof(HI_DRV_VIDEO_FRAME_S));
#endif
    pstFrame->hTunnelSrc = (HI_ID_VDEC << 16) | hHandle;

    if (stImage.image_id % 100 == pstChan->u32LastFrmId)
    {
        pstPrivInfo->u8EndFrame = 1;
        pstVideoPriv->u32LastFlag = DEF_HI_DRV_VPSS_LAST_FRAME_FLAG;
    }

    /* Count err frame */
    pstStatInfo->u32VdecErrFrame = stChanState.error_frame_num;
    //w00271806  vdec proc ErrFrame count
    pstStatInfo->u32VdecDecErrFrame = stChanState.dec_error_frame_num;

    pstFrame->bIsFirstIFrame = HI_FALSE;

    /*Record the interval of I frames and the output time of the first I frame*/
    /*CNcomment: 记录I帧间隔和换台后第一个I帧解码输出时间 */
    if ((0 == (stImage.format & 0x3)) || ((1 == stImage.is_fld_save) && \
                                          ((1 == (stImage.format & 0x3)) && ((0 == stImage.top_fld_type) || \
                                                  (0 == stImage.bottom_fld_type))))) /* I frame */
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

    /* TVP not support userdata*/
    if (stImage.is_SecureFrame != HI_TRUE)
    {
        for (u32Index = 0 ; u32Index < 4; u32Index++)
        {

            pstUsrData     = ((stImage.p_usrdat))[u32Index];

            if (HI_NULL != pstUsrData)
            {
                if (pstUsrData->data_size > 5)
                {
                    pstUsrData->PTS = stImage.PTS;
                    u32ID = *((HI_U32 *)pstUsrData->data);
                    u8Type = pstUsrData->data[4];

                    if ((VDEC_USERDATA_IDENTIFIER_DVB1 == u32ID) && (VDEC_USERDATA_TYPE_DVB1_CC == u8Type))
                    {
                        USRDATA_Put(hHandle, pstUsrData, HI_UNF_VIDEO_USERDATA_DVB1_CC);
                    }
                }
            }
        }
    }

#endif
#endif

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_GetFrmBuf(HI_HANDLE hHandle, HI_DRV_VDEC_FRAME_BUF_S *pstFrm)
{
    HI_S32 s32Ret;
    HI_S32 as8TmpBuf[16];
    VDEC_CHANNEL_S *pstChan = HI_NULL;

    if (HI_NULL == pstFrm)
    {
        HI_ERR_VDEC("Bad param!\n");
        return HI_ERR_VDEC_INVALID_PARA;
    }

    memset(as8TmpBuf, 0, sizeof(as8TmpBuf));

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
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_GET_USRDEC_FRAME, as8TmpBuf);

    if (VDEC_OK != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_WARN_VDEC("Chan %d GET_USRDEC_FRAME err!\n", pstChan->hChan);
        return HI_FAILURE;
    }

    pstFrm->u32PhyAddr = (HI_U32)(as8TmpBuf[0]);
    pstFrm->u32Size = (HI_U32)(as8TmpBuf[1]);
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_PutFrmBuf(HI_HANDLE hHandle, HI_DRV_VDEC_USR_FRAME_S *pstFrm)
{
    HI_S32 s32Ret;
    USRDEC_FRAME_DESC_S stFrameDesc;
    VDEC_CHANNEL_S       *pstChan = HI_NULL;

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

HI_S32 HI_DRV_VDEC_RecvFrmBuf(HI_HANDLE hHandle, HI_DRV_VIDEO_FRAME_S *pstFrm)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_RlsFrmBuf(HI_HANDLE hHandle, HI_DRV_VIDEO_FRAME_S  *pstFrm)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;

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

    s32Ret = VDEC_RlsFrm(pstChan, pstFrm);

    if (HI_SUCCESS != s32Ret)
    {
        VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
        HI_WARN_VDEC("VDEC_RlsFrm err!\n");
        return HI_FAILURE;
    }

    pstChan->stStatInfo.u32UserRlsFrameOK++;
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    return HI_SUCCESS;
}
static HI_S32 VDEC_Chan_VpssRecvFrmBuf(VPSS_HANDLE hVpss, HI_DRV_VIDEO_FRAME_S *pstFrm)
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
static HI_S32 VDEC_Chan_VpssRlsFrmBuf(VPSS_HANDLE hVpss, HI_DRV_VIDEO_FRAME_S  *pstFrm)
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

    s32Ret = HI_DRV_VDEC_RlsFrmBuf(hVdec, pstFrm);
    return s32Ret;
}
HI_S32 HI_DRV_VDEC_RlsFrmBufWithoutHandle(HI_DRV_VIDEO_FRAME_S *pstFrm)
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
static inline HI_VOID VDEC_YUVFormat_UNF2VFMW(HI_UNF_VIDEO_FORMAT_E enUNF, YUV_FORMAT_E *penVFMW)
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

static inline HI_VOID VDEC_YUVFormat_UNF2VFMW(HI_DRV_PIX_FORMAT_E enVideo, YUV_FORMAT_E *penVFMW)
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
HI_S32 HI_DRV_VDEC_BlockToLine(HI_S32 hHandle, HI_DRV_VDEC_BTL_S *pstBTL)
{
    /* Only hi3712 support this interface now. */
#if 0
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    struct
    {
        IMAGE astImage[2];
        HI_U32 u32Size;
    } stBTLParam;
    HI_VDEC_PRIV_FRAMEINFO_S *pstPrivInfo = HI_NULL;

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
    pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S *)pstBTL->pstInFrame->u32Priv;

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
static VDEC_CHAN_CAP_LEVEL_E VDEC_CapLevelUnfToFmw(HI_UNF_AVPLAY_OPEN_OPT_S *pstVdecCapParam)
{

    if (HI_UNF_VCODEC_DEC_TYPE_ISINGLE == pstVdecCapParam->enDecType)
    {
        return CAP_LEVEL_SINGLE_IFRAME_FHD;
    }
    else if (HI_UNF_VCODEC_DEC_TYPE_NORMAL == pstVdecCapParam->enDecType)
    {
        if (HI_UNF_VCODEC_PRTCL_LEVEL_H264 == pstVdecCapParam->enProtocolLevel)
        {
            switch (pstVdecCapParam->enCapLevel)
            {
                case HI_UNF_VCODEC_CAP_LEVEL_QCIF:
                    return CAP_LEVEL_H264_QCIF;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_CIF:
                    return CAP_LEVEL_H264_CIF;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_D1:
                    return CAP_LEVEL_H264_D1;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_720P:
                    return CAP_LEVEL_H264_720;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_FULLHD:
                    return CAP_LEVEL_H264_FHD;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_1280x800:
                    return CAP_LEVEL_1280x800;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_800x1280:
                    return CAP_LEVEL_800x1280;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_1488x1280:
                    return CAP_LEVEL_1488x1280;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_1280x1488:
                    return CAP_LEVEL_1280x1488;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_2160x1280:
                    return CAP_LEVEL_2160x1280;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_1280x2160:
                    return CAP_LEVEL_1280x2160;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_2160x2160:
                    return CAP_LEVEL_2160x2160;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_4096x2160:
                    return CAP_LEVEL_4096x2160;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_2160x4096:
                    return CAP_LEVEL_2160x4096;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_4096x4096:
                    return CAP_LEVEL_4096x4096;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_8192x4096:
                    return CAP_LEVEL_8192x4096;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_4096x8192:
                    return CAP_LEVEL_4096x8192;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_8192x8192:
                    return CAP_LEVEL_8192x8192;
                    break;

                default:
                    return CAP_LEVEL_H264_FHD;
                    break;
            }
        }
        else if (HI_UNF_VCODEC_PRTCL_LEVEL_MVC == pstVdecCapParam->enProtocolLevel)
        {
            return CAP_LEVEL_MVC_FHD;
        }
        else
        {
            switch (pstVdecCapParam->enCapLevel)
            {
                case HI_UNF_VCODEC_CAP_LEVEL_QCIF:
                    return CAP_LEVEL_MPEG_QCIF;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_CIF:
                    return CAP_LEVEL_MPEG_CIF;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_D1:
                    return CAP_LEVEL_MPEG_D1;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_720P:
                    return CAP_LEVEL_MPEG_720;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_FULLHD:
                    return CAP_LEVEL_MPEG_FHD;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_1280x800:
                    return CAP_LEVEL_1280x800;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_800x1280:
                    return CAP_LEVEL_800x1280;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_1488x1280:
                    return CAP_LEVEL_1488x1280;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_1280x1488:
                    return CAP_LEVEL_1280x1488;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_2160x1280:
                    return CAP_LEVEL_2160x1280;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_1280x2160:
                    return CAP_LEVEL_1280x2160;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_2160x2160:
                    return CAP_LEVEL_2160x2160;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_4096x2160:
                    return CAP_LEVEL_4096x2160;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_2160x4096:
                    return CAP_LEVEL_2160x4096;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_4096x4096:
                    return CAP_LEVEL_4096x4096;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_8192x4096:
                    return CAP_LEVEL_8192x4096;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_4096x8192:
                    return CAP_LEVEL_4096x8192;
                    break;

                case HI_UNF_VCODEC_CAP_LEVEL_8192x8192:
                    return CAP_LEVEL_8192x8192;
                    break;

                default:
                    return CAP_LEVEL_MPEG_FHD;
                    break;
            }
        }
    }
    else
    {
        return CAP_LEVEL_BUTT;
    }
}

static HI_S32 VDEC_Chan_InitParam(HI_HANDLE hHandle, HI_UNF_AVPLAY_OPEN_OPT_S *pstCapParam)
{
#ifdef Hi3716MV410
    HI_U32 i = 0;
#endif

    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    VDEC_CHAN_CAP_LEVEL_E enCapToFmw;

#ifndef Hi3716MV410
    HI_SYS_MEM_CONFIG_S stMemConfig = {0};
#endif

    UINT32 dfs_support = 0;

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
    /* Get proper buffer size */
    enCapToFmw = VDEC_CapLevelUnfToFmw(pstCapParam);
    pstChan->enCapToFmw = enCapToFmw;
    pstChan->stOption.eAdapterType = ADAPTER_TYPE_VDEC;
    pstChan->stOption.Purpose = PURPOSE_DECODE;
    pstChan->stOption.MemAllocMode = MODE_PART_BY_SDK;
    pstChan->stProbeStreamInfo.bProbeCodecTypeChangeFlag = HI_FALSE;

    if ((MaskCtrlWord & 0x1) == 1)
    {
        dfs_support = 0;
    }
    else if (HI_VDEC_DFS_SUPPORT == 1)
    {
        dfs_support = 1;
    }
    else
    {
        dfs_support = 0;
    }

    pstChan->stOption.u32DynamicFrameStoreAllocEn = dfs_support;
    pstChan->stOption.s32ExtraFrameStoreNum = PLUS_FS_NUM + 1;//l00273086 PLUS_FS_NUM + 1:流畅播放防止阻塞
    pstChan->stOption.s32DelayTime = 0;//l00273086 动态分配帧存时，若分配不到内存vfmw等待的时间
    pstChan->stOption.u32SelfAdaptionDFS = 1;//l00273086 自适应帧存
    pstChan->stOption.u32CfgFrameNum = 0;//l00273086 配置帧数
    pstChan->stOption.u32NeedMMZ = 1;//l00273086是否使用MMZ做为预分配不足时的补充
    pstChan->stOption.u32MaxMemUse = -1;//l00273086最大可用帧存
    pstChan->bNeedAlloc = HI_FALSE;//l00273086 记录该通道是否已经分配过帧存了

    pstChan->stOption.s32MaxSliceNum = 136;
    pstChan->stOption.s32MaxSpsNum = 32;
    pstChan->stOption.s32MaxPpsNum = 256;
    pstChan->stOption.s32SupportBFrame = 1;
    pstChan->stOption.s32SupportH264 = 1;
    pstChan->stOption.s32ReRangeEn = 1;     /* Support rerange frame buffer when definition change */
    pstChan->stOption.s32SlotWidth = 0;
    pstChan->stOption.s32SlotHeight = 0;

#ifdef Hi3716MV410
    pstChan->specialFrameExist   = 0;
    pstChan->specialFramePhyAddr = 0;
    pstChan->specialFrameVirAddr = 0;
    pstChan->specialFrameSize    = 0;


    VDEC_InitFrmBufRecSpinLock(&(pstChan->frameBufRecLock));
    VDEC_InitChanControlStateLock(&(pstChan->chanControlStateLock));

    memset(&(pstChan->frmBufRec), 0, sizeof(MMZ_BUFFER_S) * 64);

    memset(pstChan->frmBufArr, 0 , sizeof(CHAN_FRAME_BUFFER_S) * 64);

    for (i = 0; i < 64; i++)
    {
        pstChan->frmBufArr[i].bufferNodeStatus = 0xff;
        pstChan->frmBufArr[i].isAvailable = 0;
    }

#endif

    /*calculate max width and height*/
    switch (enCapToFmw)
    {
        case CAP_LEVEL_MPEG_QCIF:
        case CAP_LEVEL_H264_QCIF:
            pstChan->stOption.s32MaxWidth  = 176;
            pstChan->stOption.s32MaxHeight = 144;
            pstChan->stOption.s32SCDBufSize = 512 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_MPEG_CIF:
        case CAP_LEVEL_H264_CIF:
            pstChan->stOption.s32MaxWidth  = 352;
            pstChan->stOption.s32MaxHeight = 288;
            pstChan->stOption.s32SCDBufSize = 768 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_MPEG_D1:
        case CAP_LEVEL_H264_D1:
            pstChan->stOption.s32MaxWidth  = 720;
            pstChan->stOption.s32MaxHeight = 576;
            pstChan->stOption.s32SCDBufSize = 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_MPEG_720:
        case CAP_LEVEL_H264_720:
            pstChan->stOption.s32MaxWidth  = 1280;
            pstChan->stOption.s32MaxHeight = 736;
            pstChan->stOption.s32SCDBufSize = 2 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_MPEG_FHD:
        case CAP_LEVEL_H264_FHD:
        case CAP_LEVEL_MVC_FHD:
            pstChan->stOption.s32MaxWidth  = 1920;
            pstChan->stOption.s32MaxHeight = 1088;
            pstChan->stOption.s32SCDBufSize = 3 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_H264_BYDHD:
            pstChan->stOption.s32MaxWidth  = 5632;
            pstChan->stOption.s32MaxHeight = 4224;
            pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_SINGLE_IFRAME_FHD:
            pstChan->stOption.s32MaxWidth  = 1920;
            pstChan->stOption.s32MaxHeight = 1088;
            pstChan->stOption.s32SCDBufSize = 2 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_1280x800:
            pstChan->stOption.s32MaxWidth  = 1280;
            pstChan->stOption.s32MaxHeight = 800;
            pstChan->stOption.s32SCDBufSize = 2 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_800x1280:
            pstChan->stOption.s32MaxWidth  = 800;
            pstChan->stOption.s32MaxHeight = 1280;
            pstChan->stOption.s32SCDBufSize = 2 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_1488x1280:
            pstChan->stOption.s32MaxWidth  = 1488;
            pstChan->stOption.s32MaxHeight = 1280;
            pstChan->stOption.s32SCDBufSize = 3 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_1280x1488:
            pstChan->stOption.s32MaxWidth  = 1280;
            pstChan->stOption.s32MaxHeight = 1488;
            pstChan->stOption.s32SCDBufSize = 3 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_2160x1280:
            pstChan->stOption.s32MaxWidth  = 2160;
            pstChan->stOption.s32MaxHeight = 1280;
            pstChan->stOption.s32SCDBufSize = 3 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_1280x2160:
            pstChan->stOption.s32MaxWidth  = 1280;
            pstChan->stOption.s32MaxHeight = 2176;
            pstChan->stOption.s32SCDBufSize = 3 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_2160x2160:
            pstChan->stOption.s32MaxWidth  = 2160;
            pstChan->stOption.s32MaxHeight = 2176;
            pstChan->stOption.s32SCDBufSize = 4 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_4096x2160:
            pstChan->stOption.s32MaxWidth  = 4096;
            pstChan->stOption.s32MaxHeight = 2304;
            pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_2160x4096:
            pstChan->stOption.s32MaxWidth  = 2304;
            pstChan->stOption.s32MaxHeight = 4096;
            pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_4096x4096:
            pstChan->stOption.s32MaxWidth  = 4096;
            pstChan->stOption.s32MaxHeight = 4096;
            pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_8192x4096:
            pstChan->stOption.s32MaxWidth  = 8192;
            pstChan->stOption.s32MaxHeight = 4096;
            pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_4096x8192:
            pstChan->stOption.s32MaxWidth  = 4096;
            pstChan->stOption.s32MaxHeight = 8192;
            pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        case CAP_LEVEL_8192x8192:
            pstChan->stOption.s32MaxWidth  = 8192;
            pstChan->stOption.s32MaxHeight = 8192;
            pstChan->stOption.s32SCDBufSize = 16 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;

        default:
            pstChan->stOption.s32MaxWidth  = 1920;
            pstChan->stOption.s32MaxHeight = 1088;
            pstChan->stOption.s32SCDBufSize = 3 * 1024 * 1024 - (HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE);
            break;
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

    if (stMemConfig.u32TotalSize % 256 > 0)
    {
        stMemConfig.u32TotalSize = stMemConfig.u32TotalSize / 256 * 256 + 256;
    }

    switch (stMemConfig.u32TotalSize)
    {
        case 256:
            pstChan->stOption.s32MaxRefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_256;
            pstChan->stOption.s32DisplayFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_256 - HI_VDEC_BUFFER_FRAME;
            RefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_256;
            DispFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_256;
            pstChan->stOption.s32ExtraFrameStoreNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_256;
            pstChan->stOption.u32MaxMemUse = HI_VIDEO_MAX_VDH_BUF_IN_256 * 1024 * 1024;
            pstChan->stOption.u32DynamicFrameStoreAllocEn = 1;
            break;

        case 512:
            pstChan->stOption.s32MaxRefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_512;
            pstChan->stOption.s32DisplayFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_512 - HI_VDEC_BUFFER_FRAME;
            RefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_512;
            DispFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_512;
            pstChan->stOption.s32ExtraFrameStoreNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_512;
            pstChan->stOption.u32MaxMemUse = HI_VIDEO_MAX_VDH_BUF_IN_512 * 1024 * 1024;
            pstChan->stOption.u32DynamicFrameStoreAllocEn = 1;
            break;

        case 768:
            pstChan->stOption.s32MaxRefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_768;
            pstChan->stOption.s32DisplayFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_768 - HI_VDEC_BUFFER_FRAME;
            RefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_768;
            DispFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_768;
            pstChan->stOption.s32ExtraFrameStoreNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_768;
            pstChan->stOption.u32MaxMemUse = HI_VIDEO_MAX_VDH_BUF_IN_768 * 1024 * 1024;
            break;

        case 2048:
            pstChan->stOption.s32MaxRefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_2048;
            pstChan->stOption.s32DisplayFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_2048 - HI_VDEC_BUFFER_FRAME;
            RefFrameNum = HI_VIDEO_MAX_REF_FRAME_NUM_IN_2048;
            DispFrameNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_2048;
            pstChan->stOption.s32ExtraFrameStoreNum = HI_VIDEO_MAX_DISP_FRAME_NUM_IN_2048;
            pstChan->stOption.u32MaxMemUse = HI_VIDEO_MAX_VDH_BUF_IN_2048 * 1024 * 1024;
            break;

        case 1024:
        default:
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


static HI_S32 VDEC_Chan_AllocHandle(HI_HANDLE *phHandle, struct file *pstFile)
{
    HI_U32 i, j;
    HI_S32 s32Ret;
    HI_UNF_AVPLAY_OPEN_OPT_S stCapParam;
    VDEC_CHANNEL_S  *pstChan = HI_NULL;

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
    if (down_interruptible(&s_stVdecDrv.stSem))
    {
        HI_ERR_VDEC("Global lock err!\n");
        return HI_FAILURE;
    }

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
            s_stVdecDrv.astChanEntity[i].u32File = (HI_U32)HI_NULL;
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
    s_stVdecDrv.astChanEntity[i].u32File = (HI_U32)pstFile;

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
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        HI_ERR_VDEC("Invalid handle :%d\n", hHandle);
        return HI_FAILURE;
    }

    /* Clear global parameter */
    s32Ret = down_interruptible(&s_stVdecDrv.stSem);

    if (0 != s32Ret)
    {
        up(&s_stVdecDrv.stSem);
        HI_WARN_VDEC("down_interruptible return %d\n", s32Ret);
        return HI_FAILURE;
    }

    if (s_stVdecDrv.u32ChanNum > 0)
    {
        s_stVdecDrv.u32ChanNum--;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    s_stVdecDrv.astChanEntity[hHandle].bUsed = HI_FALSE;
    s_stVdecDrv.astChanEntity[hHandle].pstChan = HI_NULL;
    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVdec = HI_INVALID_HANDLE;
    s_stVdecDrv.astChanEntity[hHandle].u32File = (HI_U32)HI_NULL;
    HI_VFREE_VDEC(pstChan);
    up(&s_stVdecDrv.stSem);
    return HI_SUCCESS;
}

HI_S32 VDEC_Chan_FindPreMMZ(MMZ_BUFFER_S *pstMMZBuffer)
{
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U32 u32Size = 0;

    for (i = 0; i < g_VdecPreMMZNodeNum; i++)
    {
        if (st_VdecChanPreUsedMMZInfo[i].u32Size >= pstMMZBuffer->u32Size && 0 == st_VdecChanPreUsedMMZInfo[i].u32NodeState)
        {
            break;
        }
    }

    if (i < g_VdecPreMMZNodeNum)
    {
        if (pstMMZBuffer->u32Size <= st_VdecChanPreUsedMMZInfo[i].u32Size)
        {

            if (0 == i)
            {
                if (st_VdecChanPreUsedMMZInfo[i + 1].u32Size > 0)
                {
                    if (1 == st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState)
                    {
                        u32Size = st_VdecChanPreUsedMMZInfo[i].u32Size - pstMMZBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i].u32Size           = pstMMZBuffer->u32Size;

                        if (u32Size > 0)
                        {
                            for (j = g_VdecPreMMZNodeNum; j > (i + 1); j--)
                            {
                                memcpy(&st_VdecChanPreUsedMMZInfo[j], &st_VdecChanPreUsedMMZInfo[j - 1], sizeof(VDEC_PREMMZ_NODE_S));
                            }

                            st_VdecChanPreUsedMMZInfo[i + 1].u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr + pstMMZBuffer->u32Size;
                            st_VdecChanPreUsedMMZInfo[i + 1].u32StartVirAddr = st_VdecChanPreUsedMMZInfo[i].u32StartVirAddr + pstMMZBuffer->u32Size;
                            st_VdecChanPreUsedMMZInfo[i + 1].u32Size         = u32Size;
                            st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState    = 0;
                            g_VdecPreMMZNodeNum ++;
                        }
                    }
                    else
                    {
                        u32Size = st_VdecChanPreUsedMMZInfo[i].u32Size - pstMMZBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i].u32Size           = pstMMZBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr + pstMMZBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].u32StartVirAddr = st_VdecChanPreUsedMMZInfo[i].u32StartVirAddr + pstMMZBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].u32Size         = st_VdecChanPreUsedMMZInfo[i + 1].u32Size + u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState    = 0;

                    }
                }
                else
                {
                    g_VdecPreMMZNodeNum ++;
                    u32Size = st_VdecChanPreUsedMMZInfo[i].u32Size - pstMMZBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i].u32Size           = pstMMZBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i + 1].u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr + pstMMZBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i + 1].u32StartVirAddr = st_VdecChanPreUsedMMZInfo[i].u32StartVirAddr + pstMMZBuffer->u32Size;
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

                u32Size += pstMMZBuffer->u32Size;
                st_VdecChanPreUsedMMZInfo[i].u32Size           = pstMMZBuffer->u32Size;
                st_VdecChanPreUsedMMZInfo[i + 1].u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr + pstMMZBuffer->u32Size;
                st_VdecChanPreUsedMMZInfo[i + 1].u32StartVirAddr = st_VdecChanPreUsedMMZInfo[i].u32StartVirAddr + pstMMZBuffer->u32Size;
                st_VdecChanPreUsedMMZInfo[i + 1].u32Size         = g_stVDHMMZ.u32Size - u32Size;
                st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState    = 0;
                g_VdecPreMMZNodeNum ++;
            }
            else
            {
                u32Size = st_VdecChanPreUsedMMZInfo[i].u32Size - pstMMZBuffer->u32Size;

                if (st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState == 0)
                {
                    st_VdecChanPreUsedMMZInfo[i].u32Size           = pstMMZBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i + 1].u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr + pstMMZBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i + 1].u32StartVirAddr = st_VdecChanPreUsedMMZInfo[i].u32StartVirAddr + pstMMZBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i + 1].u32Size         = st_VdecChanPreUsedMMZInfo[i + 1].u32Size + u32Size;
                }
                else
                {
                    u32Size = st_VdecChanPreUsedMMZInfo[i].u32Size - pstMMZBuffer->u32Size;
                    st_VdecChanPreUsedMMZInfo[i].u32Size           = pstMMZBuffer->u32Size;

                    if (u32Size > 0)
                    {
                        for (j = g_VdecPreMMZNodeNum; j > (i + 1); j--)
                        {
                            memcpy(&st_VdecChanPreUsedMMZInfo[j], &st_VdecChanPreUsedMMZInfo[j - 1], sizeof(VDEC_PREMMZ_NODE_S));
                        }

                        st_VdecChanPreUsedMMZInfo[i + 1].u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr + pstMMZBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].u32StartVirAddr = st_VdecChanPreUsedMMZInfo[i].u32StartVirAddr + pstMMZBuffer->u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].u32Size         = u32Size;
                        st_VdecChanPreUsedMMZInfo[i + 1].u32NodeState    = 0;
                        g_VdecPreMMZNodeNum ++;
                    }
                }
            }

        st_VdecChanPreUsedMMZInfo[i].u32NodeState = 1;
        pstMMZBuffer->u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr;
        pstMMZBuffer->pu8StartVirAddr = (HI_U8*)(st_VdecChanPreUsedMMZInfo[i].u32StartVirAddr);
        return HI_SUCCESS;
       }
    }
    else
    {
        return HI_FAILURE;
    }
    return HI_FAILURE;
}

HI_S32 VDEC_Chan_ReleasePreMMZ(MMZ_BUFFER_S *pstMMZBuffer)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 i = 0;
    HI_U32 j = 0;

    if (!pstMMZBuffer)
    {
        HI_ERR_VDEC(" parameter NULL!\n");
        return HI_FAILURE;
    }

    if (pstMMZBuffer->u32Size > 0)
    {
        if (pstMMZBuffer->u32StartPhyAddr >= g_stVDHMMZ.u32StartPhyAddr && pstMMZBuffer->u32StartPhyAddr < (g_stVDHMMZ.u32StartPhyAddr + g_stVDHMMZ.u32Size))
        {
            for (i = 0; i < g_VdecPreMMZNodeNum; i++)
            {
                if ( st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr == pstMMZBuffer->u32StartPhyAddr)
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

        g_VdecPreVDHMMZUsedSize -= pstMMZBuffer->u32Size;
    }


    return s32Ret;
}



static HI_S32 VDEC_Chan_GetMemSize(HI_HANDLE hHandle)
{
    HI_S32 s32Ret;
	VDEC_CHANNEL_S *pstChan = HI_NULL;
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
        pstChan->enCapToFmw = CAP_LEVEL_H264_720;
        pstChan->stOption.s32MaxWidth  = 1280;
        pstChan->stOption.s32MaxHeight = 736;
    }

    ((HI_S32 *)as8TmpBuf)[0] = (HI_S32)pstChan->enCapToFmw;
    ((HI_S32 *)as8TmpBuf)[1] = (HI_S32)&pstChan->stOption;
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(HI_INVALID_HANDLE, VDEC_CID_GET_CHAN_DETAIL_MEMSIZE_WITH_OPTION, as8TmpBuf);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_Chan_GetMemSize failed!\n");
        return HI_FAILURE;
    }

    pstChan->stMemSize = *(DETAIL_MEM_SIZE *)as8TmpBuf;

    return HI_SUCCESS;
}

HI_S32 VDEC_Chan_AllocVFMWMem(HI_HANDLE hHandle)
{
    HI_S32 s32Ret;
    HI_U32 u32VDHSize __attribute__((unused))	= 0;
    HI_U32 ScdBufSize __attribute__((unused))	= 0;
    HI_U32 VdhBufSize __attribute__((unused))	= 0;

#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
    MMZ_BUFFER_S stMMZBuffer;
#endif
    VDEC_CHANNEL_S *pstChan = HI_NULL;

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("VDEC_Chan_Create err , chan %d invalid!\n", hHandle);
        return HI_FAILURE;
    }
    /*1:获取通道所需内存*/
    s32Ret = VDEC_Chan_GetMemSize(hHandle);
	if(HI_SUCCESS != s32Ret)
	{
	    HI_ERR_VDEC("VDEC_Chan_GetMemSize err! return :%#x\n",s32Ret);
		return HI_FAILURE;
	}

	/*2:分配通道所需内存*/
#ifdef HI_TEE_SUPPORT
    pstChan->stOption.s32IsSecMode = (HI_TRUE == pstChan->bTvp) ? 1 : 0;

    if (1 == pstChan->stOption.s32IsSecMode)
    {
        pstChan->stOption.Purpose = PURPOSE_DECODE;
        pstChan->stOption.MemAllocMode = MODE_ALL_BY_MYSELF;
        pstChan->stOption.MemDetail.ChanMemScd.Length  = 0;
        pstChan->stOption.MemDetail.ChanMemScd.PhyAddr = 0;
        pstChan->stOption.MemDetail.ChanMemScd.VirAddr = HI_NULL;
        pstChan->stOption.MemDetail.ChanMemCtx.Length  = 0;
        pstChan->stOption.MemDetail.ChanMemCtx.PhyAddr = 0;
        pstChan->stOption.MemDetail.ChanMemCtx.VirAddr = HI_NULL;
        pstChan->stOption.MemDetail.ChanMemScd.Length  = 0;
        pstChan->stOption.MemDetail.ChanMemScd.PhyAddr = 0;
        pstChan->stOption.MemDetail.ChanMemScd.VirAddr = HI_NULL;

        pstChan->stEOSBuffer.u32StartPhyAddr = (HI_U32)HI_SEC_MMZ_New(HI_VDEC_EOS_MEM_SIZE, BUFMNG_SEC_ZONE, "SEC_VDEC_LAST_FRAME");

        if (0 == pstChan->stEOSBuffer.u32StartPhyAddr)
        {
            HI_ERR_VDEC("Chan %d alloc SEC LAST FRAME MMZ err!\n", hHandle);
            goto err1;
        }
        //cwm
        pstChan->stEOSBuffer.pu8StartVirAddr = (HI_U8*)pstChan->stEOSBuffer.u32StartPhyAddr;
        pstChan->stEOSBuffer.u32Size = HI_VDEC_EOS_MEM_SIZE;
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
                s32Ret = HI_DRV_MMZ_Map(&g_stSCDMMZ);

                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_VDEC("Chan %d HI_DRV_MMZ_Map SCD MMZ err!\n", hHandle);
                }

                pstChan->stSCDMMZBuf.u32Size = pstChan->stMemSize.ScdDetailMem;
                pstChan->stSCDMMZBuf.u32StartPhyAddr = g_stSCDMMZ.u32StartPhyAddr;
                pstChan->stSCDMMZBuf.u32StartVirAddr = g_stSCDMMZ.u32StartVirAddr;

                pstChan->stEOSBuffer.u32StartPhyAddr = pstChan->stSCDMMZBuf.u32StartPhyAddr + g_stSCDMMZ.u32Size - HI_VDEC_EOS_MEM_SIZE;
                pstChan->stEOSBuffer.u32StartVirAddr = pstChan->stSCDMMZBuf.u32StartVirAddr + g_stSCDMMZ.u32Size - HI_VDEC_EOS_MEM_SIZE;
                pstChan->stEOSBuffer.u32Size = HI_VDEC_EOS_MEM_SIZE;
            }
            else
#endif
            {
                HI_U32  BufSize = ((pstChan->stMemSize.ScdDetailMem + 0x3ff) & 0xfffffc00) + HI_VDEC_EOS_MEM_SIZE;

                s32Ret = HI_DRV_MMZ_AllocAndMap("VFMW_SCD", HI_NULL, BufSize, 0, &pstChan->stSCDMMZBuf);

                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_VDEC("Chan %d alloc SCD MMZ err!\n", hHandle);
                    goto err1;
                }

                pstChan->stEOSBuffer.u32StartPhyAddr = pstChan->stSCDMMZBuf.u32StartPhyAddr + pstChan->stSCDMMZBuf.u32Size - HI_VDEC_EOS_MEM_SIZE;
                pstChan->stEOSBuffer.pu8StartVirAddr = pstChan->stSCDMMZBuf.pu8StartVirAddr + pstChan->stSCDMMZBuf.u32Size - HI_VDEC_EOS_MEM_SIZE;
                pstChan->stEOSBuffer.u32Size = HI_VDEC_EOS_MEM_SIZE;
            }

            /*pstChan->stSCDMMZBuf.u32SizeD的大小就是从vfmw获取的大小:pstChan->stMemSize.ScdDetailMem*/
            pstChan->stOption.MemDetail.ChanMemScd.Length  = pstChan->stSCDMMZBuf.u32Size;
            pstChan->stOption.MemDetail.ChanMemScd.PhyAddr = pstChan->stSCDMMZBuf.u32StartPhyAddr;
            pstChan->stOption.MemDetail.ChanMemScd.VirAddr = (HI_VOID *)pstChan->stSCDMMZBuf.pu8StartVirAddr;
            HI_INFO_VDEC("<0>SCD Buffer allocate %d!\n", pstChan->stSCDMMZBuf.u32Size);
        }

        /* Context memory allocated by VFMW */
        /* CNcomment: 这部分由vfmw自己进行分配，scd和vdh的内存由vdec进行分配*/
        pstChan->stOption.MemDetail.ChanMemCtx.Length  = 0;
        pstChan->stOption.MemDetail.ChanMemCtx.PhyAddr = 0;
        pstChan->stOption.MemDetail.ChanMemCtx.VirAddr = HI_NULL;

#if (1 == HI_VDEC_SVDEC_SUPPORT)

        if (pstChan->stCurCfg.enType == HI_UNF_VCODEC_TYPE_H263 || pstChan->stCurCfg.enType == HI_UNF_VCODEC_TYPE_SORENSON)
        {
            VDEC_GetMemConfig(&ScdBufSize, &VdhBufSize);
            u32VDHSize = (pstChan->stMemSize.VdhDetailMem >= HI_VDEC_SVDEC_VDH_MEM) ? pstChan->stMemSize.VdhDetailMem : HI_VDEC_SVDEC_VDH_MEM;

            if (VdhBufSize > 0)
            {
                u32VDHSize = (VdhBufSize >= u32VDHSize) ? (u32VDHSize) : (VdhBufSize);
            }
        }
        else
#endif
        {
            u32VDHSize = pstChan->stMemSize.VdhDetailMem;
        }

        if (u32VDHSize > 0)
        {
            pstChan->stMemSize.VdhDetailMem = u32VDHSize;
#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
            memset(&stMMZBuffer, 0, sizeof(stMMZBuffer));
            stMMZBuffer.u32Size = u32VDHSize;

            if (1 != pstChan->stOption.u32DynamicFrameStoreAllocEn) //l00273086 采用动态分配的方案，预分配在这里由于不知道真正需要的帧存大小，所以不能去预分节点。
            {
                s32Ret = VDEC_Chan_FindPreMMZ(&stMMZBuffer);
            }
            else
            {
                s32Ret = ~HI_SUCCESS;
            }

            if (HI_SUCCESS == s32Ret)
            {
                /* CNcomment:解码采用预分配内存的方式，所以将预分配内存的大小分配给解码，而不是把其需要的大小赋值*/
                pstChan->stVDHMMZBuf.u32Size  = u32VDHSize;
                pstChan->stVDHMMZBuf.u32StartPhyAddr = stMMZBuffer.u32StartPhyAddr;
                pstChan->stVDHMMZBuf.pu8StartVirAddr = stMMZBuffer.pu8StartVirAddr;
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
                    s32Ret = HI_DRV_MMZ_AllocAndMap("VFMW_VDH", HI_NULL, u32VDHSize, 0, &pstChan->stVDHMMZBuf);

                    if (HI_SUCCESS != s32Ret)
                    {
                        HI_ERR_VDEC("Chan %d alloc VDH MMZ err!\n", hHandle);
                        goto err2;
                    }
                }
            }

            pstChan->stOption.MemDetail.ChanMemVdh.Length  = pstChan->stVDHMMZBuf.u32Size;
            pstChan->stOption.MemDetail.ChanMemVdh.PhyAddr = pstChan->stVDHMMZBuf.u32StartPhyAddr;
            pstChan->stOption.MemDetail.ChanMemVdh.VirAddr = (HI_VOID *)pstChan->stVDHMMZBuf.pu8StartVirAddr;
            HI_INFO_VDEC("VDH Buffer allocate %d!\n", pstChan->stVDHMMZBuf.u32Size);
        }
    }

    return HI_SUCCESS;

err2:
#if (1 == PRE_ALLOC_VDEC_SCD_MMZ)

    if (g_bVdecPreSCDMMZUsed && (pstChan->stSCDMMZBuf.u32StartPhyAddr == g_stSCDMMZ.u32StartPhyAddr))
    {
        g_bVdecPreSCDMMZUsed = HI_FALSE;
        HI_DRV_MMZ_Unmap(&g_stSCDMMZ);
    }
    else
#endif
    {
        HI_DRV_MMZ_UnmapAndRelease(&pstChan->stSCDMMZBuf);
    }

#ifdef HI_TEE_SUPPORT

    if ( (HI_TRUE == pstChan->bTvp) && (0 != pstChan->stEOSBuffer.u32StartPhyAddr))
    {
        HI_SEC_MMZ_Delete(pstChan->stEOSBuffer.u32StartPhyAddr);
    }

#endif

err1:
    return HI_FAILURE;
}


HI_S32 VDEC_Chan_FreeVFMWMem(HI_HANDLE hHandle)
{
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("VDEC_Chan_Create err , chan %d invalid!\n", hHandle);
        return HI_FAILURE;
    }

    /* Free vfmw memory */
#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)

    if (pstChan->stVDHMMZBuf.u32StartPhyAddr >= g_stVDHMMZ.u32StartPhyAddr && pstChan->stVDHMMZBuf.u32StartPhyAddr < (g_stVDHMMZ.u32StartPhyAddr + g_stVDHMMZ.u32Size))
    {
        VDEC_Chan_ReleasePreMMZ(&pstChan->stVDHMMZBuf);
    }
    else
#endif
    {
        if (1 != pstChan->stOption.u32DynamicFrameStoreAllocEn) //l00273086
        {
            HI_DRV_MMZ_UnmapAndRelease(&pstChan->stVDHMMZBuf);
        }
    }

#if (1 == PRE_ALLOC_VDEC_SCD_MMZ)

    if (pstChan->stSCDMMZBuf.u32StartPhyAddr == g_stSCDMMZ.u32StartPhyAddr)
    {
        g_bVdecPreSCDMMZUsed = HI_FALSE;
        HI_DRV_MMZ_Unmap(&g_stSCDMMZ);
    }
    else
#endif
    {
        HI_DRV_MMZ_UnmapAndRelease(&pstChan->stSCDMMZBuf);
    }

#ifdef HI_TEE_SUPPORT

    if ( (HI_TRUE == pstChan->bTvp) && (0 != pstChan->stEOSBuffer.u32StartPhyAddr))
    {
        HI_SEC_MMZ_Delete(pstChan->stEOSBuffer.u32StartPhyAddr);
    }

#endif
    pstChan->stEOSBuffer.u32StartPhyAddr = 0;
    pstChan->stEOSBuffer.pu8StartVirAddr = 0;
    pstChan->stEOSBuffer.u32Size = 0;
    /*release the memory alloc by vdec*/
    /*CN:释放动态申请的内存,410动态内存是在vdec里面申请的，所以在vdec里面释放*/
#ifdef Hi3716MV410
    VDEC_ReleaseMemForDecoder(hHandle);
#endif

    return HI_SUCCESS;
}
static HI_S32 VDEC_Chan_Create(HI_HANDLE hHandle)
{


    HI_S32 s32Ret;
    HI_S8 as8TmpBuf[128];
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    IMAGE_INTF_S           *pstImageIntf  = HI_NULL;
    STREAM_INTF_S          *pstStreamIntf = HI_NULL;

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
    if(HI_UNF_VCODEC_TYPE_VC1 == pstChan->stCurCfg.enType)
    {
        pstChan->stOption.s32MaxWidth  = 2048;
        pstChan->stOption.s32MaxHeight = 1536;
    }

    ((HI_S32 *)as8TmpBuf)[0] = (HI_S32)pstChan->enCapToFmw;
    ((HI_S32 *)as8TmpBuf)[1] = (HI_S32)&pstChan->stOption;
    /*2:create vfmw channel*/
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(HI_INVALID_HANDLE, VDEC_CID_CREATE_CHAN_WITH_OPTION, as8TmpBuf);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VFMW CREATE_CHAN_WITH_OPTION err!\n");
        goto err1;
    }

    /* Record hHandle */
    pstChan->hChan = *(HI_U32 *)as8TmpBuf;
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
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_GET_IMAGE_INTF, pstImageIntf);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("Chan %d GET_IMAGE_INTF err!\n", hHandle);
        goto err1;
    }

    return HI_SUCCESS;

err1:
    (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)( pstChan->hChan, VDEC_CID_DESTROY_CHAN_WITH_OPTION, HI_NULL);
    pstChan->hChan = HI_INVALID_HANDLE;

    return HI_FAILURE;
}

static HI_S32 VDEC_Chan_Destroy(HI_HANDLE hHandle)
{
    HI_S32 s32Ret;

    VDEC_CHANNEL_S *pstChan = HI_NULL;

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

    VDEC_CHANNEL_S *pstChan = HI_NULL;
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
        HI_DRV_MMZ_UnmapAndRelease(&pstChan->stIFrame.st2dBuf);
        pstChan->stIFrame.st2dBuf.u32Size = 0;
    }

    /* Free pts recover channel */
    PTSREC_Free(hHandle);

    /* Free user data */
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    USRDATA_Free(hHandle);
#endif

    /* Clear global parameter */
#if 0
    s32Ret = down_interruptible(&s_stVdecDrv.stSem);
    s_stVdecDrv.astChanEntity[hHandle].pstChan = HI_NULL;
    s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVdec = HI_INVALID_HANDLE;
    s_stVdecDrv.astChanEntity[hHandle].u32File = (HI_U32)HI_NULL;
    up(&s_stVdecDrv.stSem);
#endif

    /* Free resource */
    if (pstChan)
    {
        if (pstChan->pstUsrData)
        {
            HI_KFREE_VDEC(pstChan->pstUsrData);
        }

#if 0
        HI_VFREE_VDEC(pstChan);
#endif
    }

    VDEC_CHAN_RLS_UP(&s_stVdecDrv.astChanEntity[hHandle]);
    HI_INFO_VDEC("Chan %d free OK!\n", hHandle);
    return HI_SUCCESS;
}


HI_S32 HI_DRV_VDEC_ChanStart(HI_HANDLE hHandle)
{
    HI_S32 s32Ret = HI_FAILURE;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    MEM_DESC_S  stChanMemVdh;
    MMZ_BUFFER_S stMMZBuffer;

    memset(&stChanMemVdh, 0, sizeof(MEM_DESC_S));
    memset(&stMMZBuffer, 0, sizeof(MMZ_BUFFER_S));
    hHandle = hHandle & 0xff;

    if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        return HI_ERR_VDEC_INVALID_PARA;
    }

    if (HI_NULL == s_stVdecDrv.astChanEntity[hHandle].pstChan)
    {
        HI_ERR_VDEC("Chan %d Start err , not init!\n", hHandle);
        return HI_FAILURE;
    }

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (HI_INVALID_HANDLE == s_stVdecDrv.astChanEntity[hHandle].pstChan->hChan)
    {
        pstChan->enCurState = VDEC_CHAN_STATE_STOP;
        s32Ret = VDEC_Chan_Create(hHandle);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("Chan %d Start err ,VDEC_Chan_Create err! HI_DRV_VPSS_USER_COMMAND_STOP:%d\n", hHandle, s32Ret);
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
	#ifdef Hi3716MV410
    g_ChanControlState[hHandle] = NORMAL_CHAN;
	#endif
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
    s32Ret = (s_stVdecDrv.pVpssFunc->pfnVpssSendCommand)(s_stVdecDrv.astChanEntity[hHandle].stVpssChan.hVpss, HI_DRV_VPSS_USER_COMMAND_START, NULL);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("HI_DRV_VPSS_USER_COMMAND_START err!\n");
        goto error_1;
    }

    /* Save state */
    pstChan->enCurState = VDEC_CHAN_STATE_RUN;
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    HI_INFO_VDEC("Chan %d start OK\n", hHandle);
    return HI_SUCCESS;

error_1:
    s32Ret = HI_DRV_VDEC_ChanStop(hHandle);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("inter HI_DRV_VDEC_ChanStop err!\n");
    }

    return HI_FAILURE;
}

HI_S32 HI_DRV_VDEC_ChanStop(HI_HANDLE hHandle)
{
    HI_S32 s32Ret;
    HI_U32 i = 0;

    VDEC_CHANNEL_S *pstChan = HI_NULL;
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

#ifdef Hi3716MV410
    g_CHAN_FRAME_RATE[hHandle] = 0;
#endif

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
    if (down_interruptible(&s_stVdecDrv.stSem))
    {
        HI_ERR_VDEC("Global lock err!\n");
        return HI_FAILURE;
    }

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

    up(&s_stVdecDrv.stSem);

    /* Stop user data channel */
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
    USRDATA_Stop(hHandle);
#endif

    /* Stop pts recover channel */
    PTSREC_Stop(hHandle);

    /* Save state */
    pstChan->enCurState = VDEC_CHAN_STATE_STOP;
    pstChan->bEndOfStrm = HI_FALSE;
    pstChan->u32EndFrmFlag = 0;
    pstChan->u32LastFrmId = -1;
    pstChan->u32LastFrmTryTimes = 0;
    pstChan->u32ValidPtsFlag = 0;
    pstChan->u8ResolutionChange = 0;

    //pstChan->u32DiscontinueCount = 0;

    pstChan->s32Speed = 1024;
    pstChan->bSetEosFlag = HI_FALSE;

    HI_DRV_LD_Stop_Statistics();
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    HI_INFO_VDEC("Chan %d stop ret:%x\n", hHandle, s32Ret);
    return s32Ret;
}

static HI_S32 VDEC_Chan_Reset(HI_HANDLE hHandle, HI_DRV_VDEC_RESET_TYPE_E enType)
{
    HI_S32 s32Ret, i;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;

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

static HI_S32 VDEC_Chan_CheckCfg(VDEC_CHANNEL_S *pstChan, HI_UNF_VCODEC_ATTR_S *pstCfgParam)
{
    HI_S32 s32Level = 0;
    HI_UNF_VCODEC_ATTR_S *pstCfg = &pstChan->stCurCfg;

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

HI_S32 HI_DRV_VDEC_SetChanAttr(HI_HANDLE hHandle, HI_UNF_VCODEC_ATTR_S *pstCfgParam)
{
    HI_S32 s32Ret;
    HI_S32 s32Level;
    VDEC_CHANNEL_S *pstChan = HI_NULL;

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

    /* Set config */
    pstChan->stCurCfg = *pstCfgParam;

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

HI_S32 HI_DRV_VDEC_GetChanAttr(HI_HANDLE hHandle, HI_UNF_VCODEC_ATTR_S *pstCfgParam)
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

    *pstCfgParam = s_stVdecDrv.astChanEntity[hHandle].pstChan->stCurCfg;
    VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);

    HI_INFO_VDEC("Chan %d GetAttr OK\n", hHandle);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VDEC_GetChanStatusInfo(HI_HANDLE hHandle, VDEC_STATUSINFO_S *pstStatus)
{
    HI_S32 i;
    HI_U32 freeSize;
    HI_U32 busySize;
    HI_S32 s32Ret;
    HI_HANDLE hMasterPortHandle = HI_INVALID_HANDLE;
    HI_HANDLE hVirPortHandle = HI_INVALID_HANDLE;
    HI_HANDLE hPortHandle = HI_INVALID_HANDLE;
    HI_BOOL bAllPortCompleteFrm = HI_FALSE;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    VDEC_VPSSCHANNEL_S *pstVpssChan = HI_NULL;
    VDEC_CHAN_STATE_S stChanState;
    BUFMNG_STATUS_S stStatus;
    HI_DRV_VPSS_PORT_BUFLIST_STATE_S stVpssBufListState;

    /* check input parameters */
    if (HI_NULL == pstStatus)
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
        //HI_ERR_VDEC("Chan %d not init!\n", hHandle);
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

    if (pstChan->bEndOfStrm)
    {
        pstStatus->bEndOfStream = HI_TRUE;
    }
    else
    {
        pstStatus->bEndOfStream = HI_FALSE;
    }

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

HI_S32 HI_DRV_VDEC_GetChanStreamInfo(HI_HANDLE hHandle, HI_UNF_VCODEC_STREAMINFO_S *pstStreamInfo)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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

HI_S32 HI_DRV_VDEC_CheckNewEvent(HI_HANDLE hHandle, VDEC_EVENT_S *pstEvent)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;

    /* check input parameters */
    if (HI_NULL == pstEvent)
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

HI_S32 HI_DRV_VDEC_GetUsrData(HI_HANDLE hHandle, HI_UNF_VIDEO_USERDATA_S *pstUsrData)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;

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
    s32Ret = copy_to_user(pstUsrData->pu8Buffer,
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

HI_S32 HI_DRV_VDEC_SetDPBFullCtrl(HI_HANDLE hHandle, HI_BOOL bDPBFullCtrl)
{
    VDEC_CHANNEL_S *pstChan = HI_NULL;

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


HI_S32 HI_DRV_VDEC_SetTrickMode(HI_HANDLE hHandle, HI_UNF_AVPLAY_TPLAY_OPT_S *pstOpt)
{
    HI_S32 i = 0;
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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

HI_S32 HI_DRV_VDEC_SetCtrlInfo(HI_HANDLE hHandle, HI_UNF_AVPLAY_CONTROL_INFO_S *pstCtrlInfo)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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


static HI_S32 VDEC_IFrame_GetStrm(HI_S32 hHandle, STREAM_DATA_S *pstPacket)
{
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;

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
    pstPacket->VirAddr = pstChan->stIFrame.stMMZBuf.pu8StartVirAddr;
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

static HI_S32 VDEC_IFrame_PutStrm(HI_S32 hHandle, STREAM_DATA_S *pstPacket)
{
    return HI_SUCCESS;
}

static HI_S32 VDEC_IFrame_SetAttr(VDEC_CHANNEL_S *pstChan, HI_UNF_VCODEC_TYPE_E type)
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
#ifndef HI_TEE_SUPPORT
static HI_S32 VDEC_IFrame_BufInit(HI_U32 u32BufSize, MMZ_BUFFER_S *pstMMZBuf)
#else
static HI_S32 VDEC_IFrame_BufInit(HI_U32 u32BufSize, MMZ_BUFFER_S *pstMMZBuf, HI_BOOL bTvp)
#endif
{
#ifdef HI_TEE_SUPPORT

    if (HI_TRUE == bTvp)
    {
        pstMMZBuf->u32StartPhyAddr = (HI_U32)HI_SEC_MMZ_New(u32BufSize, BUFMNG_SEC_ZONE, "SEC_VDEC_IFrame");

        if (0 == pstMMZBuf->u32StartPhyAddr)
        {
            HI_ERR_VDEC("%s: Alloc SEC MMZ(%s) failed.\n", __func__, "VDEC_IFrame");
            return HI_FAILURE;
        }

        pstMMZBuf->pu8StartVirAddr = (HI_U8 *)pstMMZBuf->u32StartPhyAddr;
        pstMMZBuf->u32Size = u32BufSize;

        return HI_SUCCESS;
    }
    else
#endif
    {
        return HI_DRV_MMZ_AllocAndMap("VDEC_IFrame", HI_NULL, u32BufSize, 0, pstMMZBuf);
    }
}

static HI_VOID VDEC_IFrame_BufDeInit(VDEC_CHANNEL_S* pstChan)
{
    MMZ_BUFFER_S* pstMMZBuf = &pstChan->stIFrame.stMMZBuf;

#ifdef HI_TEE_SUPPORT

    if (HI_TRUE == pstChan->bTvp)
    {
        HI_SEC_MMZ_Delete(pstMMZBuf->u32StartPhyAddr);
    }
    else
#endif
    {
        HI_DRV_MMZ_UnmapAndRelease(pstMMZBuf);
    }

    memset(pstMMZBuf, 0, sizeof(MMZ_BUFFER_S));
    return;
}

HI_S32 HI_DRV_VDEC_DecodeIFrame(HI_HANDLE hHandle, HI_UNF_AVPLAY_I_FRAME_S *pstStreamInfo, HI_BOOL bCapture, HI_BOOL bUserSpace)
{
    HI_S32 s32Ret, i;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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
#ifndef HI_TEE_SUPPORT
    s32Ret = VDEC_IFrame_BufInit(pstStreamInfo->u32BufSize, &(pstChan->stIFrame.stMMZBuf));
#else
    s32Ret = VDEC_IFrame_BufInit(pstStreamInfo->u32BufSize, &(pstChan->stIFrame.stMMZBuf), pstChan->bTvp);
#endif

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
            memcpy(pstChan->stIFrame.stMMZBuf.pu8StartVirAddr,
                   pstStreamInfo->pu8Addr, pstStreamInfo->u32BufSize);
        }
        else
        {
            /*I MODE : the stream from user is one IFrame stream, copy the stream from user space , should use function : copy_from_user*/
            if (0 != copy_from_user(pstChan->stIFrame.stMMZBuf.pu8StartVirAddr,
                                    pstStreamInfo->pu8Addr, pstStreamInfo->u32BufSize))
            {
                VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[hHandle]);
                HI_ERR_VDEC("Chan %d IFrame copy %dB %p->%p err!\n",
                            hHandle, pstStreamInfo->u32BufSize, pstStreamInfo->pu8Addr,
                            pstChan->stIFrame.stMMZBuf.pu8StartVirAddr);
                goto OUT1;
            }

            HI_INFO_VDEC("Chan %d IFrame copy %dB %p->%p success!\n",
                         hHandle, pstStreamInfo->u32BufSize, pstStreamInfo->pu8Addr,
                         pstChan->stIFrame.stMMZBuf.pu8StartVirAddr);
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

#ifdef Hi3716MV410
    g_ChanControlState[hHandle] = NORMAL_CHAN;
#endif

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
    pstChan->enCurState = VDEC_CHAN_STATE_STOP;
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

HI_S32 HI_DRV_VDEC_ReleaseIFrame(HI_HANDLE hHandle, HI_DRV_VIDEO_FRAME_S *pstFrameInfo)
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
    VDEC_CHANNEL_S *pstChan = HI_NULL;
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
        pstChan->stCurCfg.s32CtrlOptions = 1;
    }
    else
    {
        pstChan->stCurCfg.enMode = HI_UNF_VCODEC_MODE_NORMAL;
        pstChan->stCurCfg.bOrderOutput = 0;
        pstChan->stCurCfg.s32CtrlOptions = 0;
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
    VDEC_CHANNEL_S *pstChan = HI_NULL;

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

        if (HI_SUCCESS != (s_stVdecDrv.pVfmwFunc->pfnVfmwTrustDecoderInit)(&stOpt))
        {
            HI_ERR_VDEC("Init trusted vfmw failed\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}
#endif

HI_S32 VDEC_Ioctl(struct inode *inode, struct file  *filp, unsigned int cmd, void *arg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hHandle = HI_INVALID_HANDLE;

    /* Check parameter in this switch */
    switch (cmd)
    {

        case UMAPC_VDEC_GETCAP:
        case UMAPC_VDEC_ALLOCHANDLE:
        case UMAPC_VDEC_CREATE_ESBUF:
        case UMAPC_VDEC_DESTROY_ESBUF:
        case UMAPC_VDEC_GETBUF:
        case UMAPC_VDEC_PUTBUF:
        case UMAPC_VDEC_SETUSERADDR_OLD:
        case UMAPC_VDEC_RCVBUF:
        case UMAPC_VDEC_RLSBUF:
        case UMAPC_VDEC_RESET_ESBUF:
        case UMAPC_VDEC_GET_ESBUF_STATUS:
            if (HI_NULL == arg)
            {
                HI_ERR_VDEC("CMD %p Bad arg!\n", (HI_VOID *)cmd);
                return HI_ERR_VDEC_INVALID_PARA;
            }

            break;

        case UMAPC_VDEC_FREEHANDLE:
        case UMAPC_VDEC_CHAN_ALLOC:
        case UMAPC_VDEC_CHAN_FREE:
        case UMAPC_VDEC_CHAN_START:
        case UMAPC_VDEC_CHAN_STOP:
        case UMAPC_VDEC_CHAN_RESET:
        case UMAPC_VDEC_CHAN_SETATTR_OLD:
        case UMAPC_VDEC_CHAN_GETATTR_OLD:
        case UMAPC_VDEC_CHAN_ATTACHBUF:
        case UMAPC_VDEC_CHAN_DETACHBUF:
        case UMAPC_VDEC_CHAN_SETEOSFLAG:
        case UMAPC_VDEC_CHAN_DISCARDFRM:
        case UMAPC_VDEC_CHAN_USRDATA_OLD:
        case UMAPC_VDEC_CHAN_STATUSINFO:
        case UMAPC_VDEC_CHAN_STREAMINFO:
        case UMAPC_VDEC_CHAN_CHECKEVT:
        case UMAPC_VDEC_CHAN_EVNET_NEWFRAME:
        case UMAPC_VDEC_CHAN_IFRMDECODE_OLD:
        case UMAPC_VDEC_CHAN_IFRMRELEASE:
        case UMAPC_VDEC_CHAN_SETFRMRATE:
        case UMAPC_VDEC_CHAN_GETFRMRATE:
        case UMAPC_VDEC_CHAN_GETFRM:
        case UMAPC_VDEC_CHAN_PUTFRM:
        case UMAPC_VDEC_CHAN_RLSFRM:
        case UMAPC_VDEC_CHAN_RCVFRM:
        case UMAPC_VDEC_CHAN_SETTRICKMODE:
#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)
        case UMAPC_VDEC_CHAN_USERDATAINITBUF:
        case UMAPC_VDEC_CHAN_USERDATASETBUFADDR_OLD:
        case UMAPC_VDEC_CHAN_ACQUSERDATA_OLD:
        case UMAPC_VDEC_CHAN_RLSUSERDATA_OLD:
#endif
        case UMAPC_VDEC_CHAN_SEEKPTS:
        case UMAPC_VDEC_CHAN_SETCTRLINFO:
        case UMAPC_VDEC_CHAN_GETFRMSTATUSINFO:
        case UMAPC_VDEC_CHAN_SENDEOS:
        case UMAPC_VDEC_CHAN_GETPORTSTATE:
        case UMAPC_VDEC_CHAN_PROGRSSIVE:
        case UMAPC_VDEC_CHAN_LOWDELAY:
#ifdef HI_TEE_SUPPORT
        case UMAPC_VDEC_CHAN_TVP:
#endif
        case UMAPC_VDEC_CHAN_DPBFULL:
        case UMAPC_VDEC_CHAN_SETCOLORSPACE:
            if (HI_NULL == arg)
            {
                HI_ERR_VDEC("CMD %p Bad arg!\n", (HI_VOID *)cmd);
                return HI_ERR_VDEC_INVALID_PARA;
            }

            hHandle = (*((HI_HANDLE *)arg)) & 0xff;

            if (hHandle >= HI_VDEC_MAX_INSTANCE_NEW)
            {
                HI_ERR_VDEC("CMD %p bad handle:%d!\n", (HI_VOID *)cmd, hHandle);
                return HI_ERR_VDEC_INVALID_PARA;
            }

            break;

        case UMAPC_VDEC_CHAN_CREATEVPSS:
        case UMAPC_VDEC_CHAN_DESTORYVPSS:
            if (HI_NULL == arg)
            {
                HI_ERR_VDEC("CMD %p Bad arg!\n", (HI_VOID *)cmd);
                return HI_ERR_VDEC_INVALID_PARA;
            }

            hHandle = (*((HI_HANDLE *)arg)) & 0xff;
            break;

            //add by l00225186
            /*调用vpss接口的时候，使用的是vpss的句柄，因此需要将句柄参数传递过来，并提取*/
        case UMAPC_VDEC_CHAN_RCVVPSSFRM:
        case UMAPC_VDEC_CHAN_CREATEPORT:
        case UMAPC_VDEC_CHAN_DESTROYPORT:
        case UMAPC_VDEC_CHAN_ENABLEPORT:
        case UMAPC_VDEC_CHAN_DISABLEPORT:
        case UMAPC_VDEC_CHAN_SETPORTTYPE:
        case UMAPC_VDEC_CHAN_CANCLEMAINPORT:
        case UMAPC_VDEC_CHAN_SETFRMPACKTYPE:
        case UMAPC_VDEC_CHAN_GETFRMPACKTYPE:
        case UMAPC_VDEC_CHAN_RESETVPSS:
        case UMAPC_VDEC_CHAN_SETPORTATTR:
        case UMAPC_VDEC_CHAN_GETPORTATTR:
        case UMAPC_VDEC_CHAN_RLSPORTFRM:
        case UMAPC_VDEC_CHAN_SETEXTBUFFER:
        case UMAPC_VDEC_CHAN_SETBUFFERMODE:
        case UMAPC_VDEC_CHAN_CHECKANDDELBUFFER:
        case UMAPC_VDEC_CHAN_SETEXTBUFFERSTATE:
        case UMAPC_VDEC_CHAN_SETRESOLUTION:
            if (HI_NULL == arg)
            {
                HI_ERR_VDEC("CMD %p Bad arg!\n", (HI_VOID *)cmd);
                return HI_ERR_VDEC_INVALID_PARA;
            }

            hHandle = *((HI_HANDLE *)arg); //vpss的句柄是什么生成规则未知，不做判断
            break;

        default:
            HI_ERR_VDEC("CMD %p unsupport now!\n", (HI_VOID *)cmd);
            return HI_ERR_VDEC_NOT_SUPPORT;

    }

    /* Call function in this switch */
    switch (cmd)
    {
        case UMAPC_VDEC_CHAN_USRDATA_OLD:
        {
            s32Ret = HI_DRV_VDEC_GetUsrData(hHandle, &(((VDEC_CMD_USERDATA_OLD_S *)arg)->stUserData));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_GetUsrData err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_IFRMDECODE_OLD:
        {
            VDEC_CMD_IFRAME_DEC_OLD_S *pstIFrameDec = (VDEC_CMD_IFRAME_DEC_OLD_S *)arg;
            s32Ret  = HI_DRV_VDEC_DecodeIFrame((pstIFrameDec->hHandle) & 0xff, &pstIFrameDec->stIFrame,pstIFrameDec->bCapture, HI_TRUE);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_DecodeIFrame err!:%d\n", s32Ret);
            }

            s32Ret |= VDEC_Chan_Reset((pstIFrameDec->hHandle) & 0xff, VDEC_RESET_TYPE_ALL);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_Reset err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_IFRMRELEASE:
        {
            s32Ret = HI_DRV_VDEC_ReleaseIFrame(hHandle, &(((VDEC_CMD_IFRAME_RLS_S *)arg)->stVoFrameInfo));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_ReleaseIFrame err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_ALLOC:
        {
            //s32Ret = VDEC_Chan_Alloc(hHandle, &(((VDEC_CMD_ALLOC_S*)arg)->stOpenOpt));
            if (((VDEC_CMD_ALLOC_S *)arg)->u32DFSEnable > 1) //l00273086
            {
                ((VDEC_CMD_ALLOC_S *)arg)->u32DFSEnable = 0;
            }

            s_stVdecDrv.astChanEntity[hHandle].u32DynamicFsEn = ((VDEC_CMD_ALLOC_S *)arg)->u32DFSEnable; //l00273086
            s32Ret = VDEC_Chan_InitParam(hHandle, &(((VDEC_CMD_ALLOC_S *)arg)->stOpenOpt));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_InitParam err!:%d\n", s32Ret);
            }

            *((HI_HANDLE *)arg) = hHandle;
            break;
        }

        case UMAPC_VDEC_CHAN_FREE:
        {
            s32Ret = VDEC_Chan_Free(hHandle);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_Free err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_START:
        {
            s32Ret = HI_DRV_VDEC_ChanStart(hHandle);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_ChanStart err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_STOP:
        {
#ifdef Hi3716MV410
            HI_S32 stopLoop = 500;

            while ((HI_FAILURE == VDEC_SetChanControlState(hHandle, DESTORY_CHAN)) && (stopLoop > 0))
            {
                msleep(1);
                stopLoop--;
            }

            if (stopLoop == 0)
            {
                HI_ERR_VDEC("VDEC_SetChanControlState(hHandle, DESTORY_CHAN) failed\n");
                s32Ret = HI_FAILURE;
                break;
            }

#endif

            s32Ret = HI_DRV_VDEC_ChanStop(hHandle);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_ChanStop err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_RESET:
        {
            s32Ret = VDEC_Chan_Reset(hHandle, ((VDEC_CMD_RESET_S *)arg)->enType);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_Reset err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_SETATTR_OLD:
        {
            s32Ret = HI_DRV_VDEC_SetChanAttr(hHandle, &(((VDEC_CMD_ATTR_OLD_S *)arg)->stAttr));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_SetChanAttr err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_GETATTR_OLD:
        {
            s32Ret = HI_DRV_VDEC_GetChanAttr(hHandle, &(((VDEC_CMD_ATTR_OLD_S *)arg)->stAttr));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_GetChanAttr err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CREATE_ESBUF:
        {
            s32Ret = VDEC_CreateStrmBuf((HI_DRV_VDEC_STREAM_BUF_S *)arg);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_CreateStrmBuf err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_DESTROY_ESBUF:
        {
            s32Ret = VDEC_DestroyStrmBuf(*((HI_HANDLE *)arg));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_DestroyStrmBuf err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_GETBUF:
        {
            VDEC_CMD_BUF_S *pstBuf = (VDEC_CMD_BUF_S *)arg;
            s32Ret = VDEC_GetStrmBuf(pstBuf->hHandle, &(pstBuf->stBuf), HI_TRUE);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_GetStrmBuf err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_PUTBUF:
        {
            VDEC_CMD_BUF_S *pstBuf = (VDEC_CMD_BUF_S *)arg;
            s32Ret = VDEC_PutStrmBuf(pstBuf->hHandle, &(pstBuf->stBuf), HI_TRUE);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_PutStrmBuf err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_SETUSERADDR_OLD:
        {
            VDEC_CMD_BUF_USERADDR_OLD_S *pstUserAddr = (VDEC_CMD_BUF_USERADDR_OLD_S *)arg;
            s32Ret = VDEC_StrmBuf_SetUserAddr(pstUserAddr->hHandle, pstUserAddr->u32UserAddr);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_StrmBuf_SetUserAddr err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_RCVBUF:
        {
            BUFMNG_BUF_S stEsBuf;
            VDEC_CMD_BUF_S *pstBuf = (VDEC_CMD_BUF_S *)arg;

            s32Ret = BUFMNG_AcqReadBuffer(pstBuf->hHandle, &stEsBuf);

            if (HI_SUCCESS == s32Ret)
            {
                pstBuf->stBuf.u64Addr= (HI_U64)(HI_SIZE_T)stEsBuf.pu8UsrVirAddr;
                pstBuf->stBuf.u32PhyAddr = stEsBuf.u32PhyAddr;
                pstBuf->stBuf.u32BufSize = stEsBuf.u32Size;
                pstBuf->stBuf.u64Pts = stEsBuf.u64Pts;
                pstBuf->stBuf.bEndOfFrame = !(stEsBuf.u32Marker & BUFMNG_NOT_END_FRAME_BIT);
                pstBuf->stBuf.bDiscontinuous = (stEsBuf.u32Marker & BUFMNG_DISCONTINUOUS_BIT) ? 1 : 0;
            }

            break;
        }

        case UMAPC_VDEC_RLSBUF:
        {
            BUFMNG_BUF_S stEsBuf;
            VDEC_CMD_BUF_S *pstBuf = (VDEC_CMD_BUF_S *)arg;

            stEsBuf.u32PhyAddr = 0;
             stEsBuf.pu8UsrVirAddr = (HI_U8*)(HI_SIZE_T)pstBuf->stBuf.u64Addr;
            stEsBuf.pu8KnlVirAddr = HI_NULL;
            stEsBuf.u32Size = pstBuf->stBuf.u32BufSize;
            stEsBuf.u64Pts = pstBuf->stBuf.u64Pts;
            /* Don't care stEsBuf.u32Marker here. */
            s32Ret = BUFMNG_RlsReadBuffer(pstBuf->hHandle, &stEsBuf);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("BUFMNG_RlsReadBuffer err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_RESET_ESBUF:
        {
            s32Ret = BUFMNG_Reset((*((HI_HANDLE *)arg)));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("BUFMNG_Reset err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_GET_ESBUF_STATUS:
        {
            BUFMNG_STATUS_S stBMStatus;
            VDEC_CMD_BUF_STATUS_S *pstStatus = (VDEC_CMD_BUF_STATUS_S *)arg;
            s32Ret = BUFMNG_GetStatus(pstStatus->hHandle, &stBMStatus);

            if (HI_SUCCESS == s32Ret)
            {
                pstStatus->stStatus.u32Size = stBMStatus.u32Used + stBMStatus.u32Free;
                pstStatus->stStatus.u32Available = stBMStatus.u32Free;
                pstStatus->stStatus.u32Used = stBMStatus.u32Used;
                pstStatus->stStatus.u32DataNum = stBMStatus.u32DataNum;
            }

            break;
        }

        case UMAPC_VDEC_CHAN_RLSFRM:
        {
            s32Ret = HI_DRV_VDEC_RlsFrmBuf(hHandle, &(((VDEC_CMD_VO_FRAME_S *)arg)->stFrame));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_RlsFrmBuf err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_RCVFRM:
        {
            s32Ret = HI_DRV_VDEC_RecvFrmBuf(hHandle, &(((VDEC_CMD_VO_FRAME_S *)arg)->stFrame));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_RecvFrmBuf err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_STATUSINFO:
        {
            s32Ret = HI_DRV_VDEC_GetChanStatusInfo(hHandle, &(((VDEC_CMD_STATUS_S *)arg)->stStatus));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_GetChanStatusInfo err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_STREAMINFO:
        {
            s32Ret = HI_DRV_VDEC_GetChanStreamInfo(hHandle, &(((VDEC_CMD_STREAM_INFO_S *)arg)->stInfo));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_GetChanStreamInfo err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_ATTACHBUF:
        {
            VDEC_CMD_ATTACH_BUF_S stParam = *((VDEC_CMD_ATTACH_BUF_S *)arg);

            s32Ret = VDEC_Chan_AttachStrmBuf(hHandle, stParam.u32BufSize, stParam.hDmxVidChn, stParam.hStrmBuf);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_AttachStrmBuf err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_DETACHBUF:
        {
            s32Ret = VDEC_Chan_DetachStrmBuf(hHandle);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_DetachStrmBuf err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_SETEOSFLAG:
        {
            s32Ret = HI_DRV_VDEC_SetEosFlag(hHandle);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_SetEosFlag err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_DISCARDFRM:
        {
            s32Ret = HI_DRV_VDEC_DiscardFrm(hHandle, &(((VDEC_CMD_DISCARD_FRAME_S *)arg)->stDiscardOpt));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_DiscardFrm err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_CHECKEVT:
        {
            s32Ret = HI_DRV_VDEC_CheckNewEvent(hHandle, &(((VDEC_CMD_EVENT_S *)arg)->stEvent));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_CheckNewEvent err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_EVNET_NEWFRAME:
        {
            VDEC_CMD_FRAME_S *pstFrameCmd = (VDEC_CMD_FRAME_S *)arg;
            pstFrameCmd->stFrame = s_stVdecDrv.astChanEntity[pstFrameCmd->hHandle & 0xff].pstChan->stLastFrm;
            s32Ret = HI_SUCCESS;
            HI_INFO_VDEC("UMAPC_VDEC_CHAN_EVNET_NEWFRAME return:%d\n", s32Ret);
            break;
        }

        case UMAPC_VDEC_CHAN_GETFRM:
        {
            s32Ret = HI_DRV_VDEC_GetFrmBuf(hHandle, &(((VDEC_CMD_GET_FRAME_S *)arg)->stFrame));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_GetFrmBuf err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_PUTFRM:
        {
            s32Ret = HI_DRV_VDEC_PutFrmBuf(hHandle, &(((VDEC_CMD_PUT_FRAME_S *)arg)->stFrame));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_PutFrmBuf err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_SETFRMRATE:
        {
            HI_UNF_AVPLAY_FRMRATE_PARAM_S *pstParam = &(((VDEC_CMD_FRAME_RATE_S *)arg)->stFrameRate);

            if (HI_NULL != s_stVdecDrv.astChanEntity[hHandle].pstChan)
            {
                s_stVdecDrv.astChanEntity[hHandle].pstChan->stFrameRateParam = *pstParam;
                s32Ret = PTSREC_SetFrmRate(hHandle, pstParam);

                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_VDEC("PTSREC_SetFrmRate err:%d\n", s32Ret);
                }
            }
            else
            {
                s32Ret = HI_FAILURE;
                HI_ERR_VDEC("UMAPC_VDEC_CHAN_SETFRMRATE return :%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_GETFRMRATE:
        {
            HI_UNF_AVPLAY_FRMRATE_PARAM_S *pstParam = &(((VDEC_CMD_FRAME_RATE_S *)arg)->stFrameRate);

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

            break;
        }

#if (1 == HI_VDEC_USERDATA_CC_SUPPORT)

        case UMAPC_VDEC_CHAN_USERDATAINITBUF:
        {
            s32Ret = USRDATA_Alloc(hHandle, &(((VDEC_CMD_USERDATABUF_S *)arg)->stBuf));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("USRDATA_Alloc err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_USERDATASETBUFADDR_OLD:
        {
            s32Ret = USRDATA_SetUserAddr(hHandle, ((VDEC_CMD_BUF_USERADDR_OLD_S *)arg)->u32UserAddr);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("USRDATA_SetUserAddr err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_ACQUSERDATA_OLD:
        {
            s32Ret = USRDATA_Acq(hHandle, &(((VDEC_CMD_USERDATA_ACQMODE_OLD_S *)arg)->stUserData),
                                 &(((VDEC_CMD_USERDATA_ACQMODE_OLD_S *)arg)->enType));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("USRDATA_Acq err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_RLSUSERDATA_OLD:
        {
            s32Ret = USRDATA_Rls(hHandle, &(((VDEC_CMD_USERDATA_OLD_S *)arg)->stUserData));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("USRDATA_Rls err!:%d\n", s32Ret);
            }

            break;
        }

#endif

        case UMAPC_VDEC_CHAN_SEEKPTS:
        {
            s32Ret = VDEC_SeekPTS(hHandle, &(((VDEC_CMD_SEEK_PTS_S *)(arg))->u32SeekPts), ((VDEC_CMD_SEEK_PTS_S *)(arg))->u32Gap);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_SeekPTS err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_GETCAP:
        {
            s32Ret = VDEC_GetCap((VDEC_CAP_S *)arg);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_GetCap err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_ALLOCHANDLE:
        {
            s32Ret = VDEC_Chan_AllocHandle((HI_HANDLE *)arg, filp);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_AllocHandle err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_FREEHANDLE:
        {
            s32Ret = VDEC_Chan_FreeHandle(hHandle);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_FreeHandle err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_SETTRICKMODE:
        {
            s32Ret = HI_DRV_VDEC_SetTrickMode(hHandle, &(((VDEC_CMD_TRICKMODE_OPT_S *)arg)->stTPlayOpt));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_SetTrickMode err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_SETCTRLINFO:
        {
            s32Ret = HI_DRV_VDEC_SetCtrlInfo(hHandle, &(((VDEC_CMD_SET_CTRL_INFO_S *)arg)->stCtrlInfo));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_SetCtrlInfo err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_PROGRSSIVE:
        {
            s32Ret = HI_DRV_VDEC_SetProgressive(hHandle, ((VDEC_CMD_SET_PROGRESSIVE_S *)arg)->bProgressive);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_SetProgressive err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_DPBFULL:
        {
            s32Ret = HI_DRV_VDEC_SetDPBFullCtrl(hHandle, ((VDEC_CMD_SET_DPBFULL_CTRL_S *)arg)->bDPBFullCtrl);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_SetDPBFullCtrl err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_LOWDELAY:
        {
            s32Ret = HI_DRV_VDEC_SetLowdelay(hHandle, ((VDEC_CMD_SET_LOWDELAY_S *)arg)->bLowdelay);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_SetLowdelay err!:%d\n", s32Ret);
            }

            break;
        }

#ifdef HI_TEE_SUPPORT

        case UMAPC_VDEC_CHAN_TVP:
        {
            s32Ret = HI_DRV_VDEC_SetTvp(hHandle, ((VDEC_CMD_SET_TVP_S *)arg)->bTVP);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_SetTvp err!:%d\n", s32Ret);
            }

            break;
        }

#endif

        case UMAPC_VDEC_CHAN_SETCOLORSPACE:
        {
            s32Ret = HI_DRV_VDEC_SetColorSpace(hHandle, ((VDEC_CMD_SET_COLORSPACE_S *)arg)->u32ColorSpace);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("HI_DRV_VDEC_SetColorSpace err!:%d\n", s32Ret);
            }

            break;
        }

        //add by l00225186
        case UMAPC_VDEC_CHAN_RCVVPSSFRM:
        {
            //hHandle表示vpss的handle
            HI_HANDLE hVdec = HI_INVALID_HANDLE;
            VDEC_CMD_VPSS_FRAME_S *pstVpssFrmTmp = (VDEC_CMD_VPSS_FRAME_S *)arg;

            s32Ret = VDEC_FindVdecHandleByVpssHandle(hHandle, &hVdec);

            if (HI_SUCCESS == s32Ret)
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
                    s32Ret = VDEC_Chan_RecvVpssFrmBuf(hHandle, &(pstVpssFrmTmp->stFrame));

                    if (HI_SUCCESS != s32Ret)
                    {
                        HI_INFO_VDEC("VDEC_Chan_RecvVpssFrmBuf err:%d\n", s32Ret);
                    }
                }
            }

            break;
        }

        case UMAPC_VDEC_CHAN_RLSPORTFRM:
            s32Ret = VDEC_Chan_VORlsFrame(hHandle, &(((VDEC_CMD_VPSS_FRAME_S *)arg)->VideoFrame));

            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VDEC("VDEC release port %d frame ERR:%x!\n", s32Ret);
            }

            break;

        case UMAPC_VDEC_CHAN_CREATEVPSS:
        {
            HI_U32 hOuthandle = HI_INVALID_HANDLE;
            s32Ret = VDEC_Chan_CreateVpss(hHandle, &hOuthandle);

            if (s32Ret == HI_SUCCESS)
            {
                *((HI_HANDLE *)arg) = hOuthandle; //将vpss的句柄返回，供用户态代码中使用
            }

            break;
        }

        case UMAPC_VDEC_CHAN_DESTORYVPSS:
        {
            s32Ret = VDEC_Chan_DestroyVpss(hHandle);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_DestroyVpss err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_CREATEPORT:
        {
            s32Ret = VDEC_Chan_CreatePort(hHandle, &((VDEC_CMD_VPSS_FRAME_S *)arg)->hPort, ((VDEC_CMD_VPSS_FRAME_S *)arg)->ePortAbility);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_CreatePort err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_DESTROYPORT:
        {
            s32Ret = VDEC_Chan_DestroyPort(hHandle, ((VDEC_CMD_VPSS_FRAME_S *)arg)->hPort);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_DestroyPort err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_ENABLEPORT:
        {
            s32Ret = VDEC_Chan_EnablePort(hHandle, ((VDEC_CMD_VPSS_FRAME_S *)arg)->hPort);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_EnablePort err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_DISABLEPORT:
        {
            s32Ret = VDEC_Chan_DisablePort(hHandle, ((VDEC_CMD_VPSS_FRAME_S *)arg)->hPort);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_DisablePort err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_SETPORTTYPE:
        {
            s32Ret = VDEC_Chan_SetPortType(hHandle, ((VDEC_CMD_VPSS_FRAME_S *)arg)->hPort, ((VDEC_CMD_VPSS_FRAME_S *)arg)->enPortType);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_SetPortType err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_CANCLEMAINPORT:
        {
            break;
        }

        case UMAPC_VDEC_CHAN_SETFRMPACKTYPE:
        {
            s32Ret = VDEC_Chan_SetFrmPackingType(hHandle, ((VDEC_CMD_VPSS_FRAME_S *)arg)->eFramePackType);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_SetFrmPackingType err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_GETFRMPACKTYPE:
        {
            s32Ret = VDEC_Chan_GetFrmPackingType(hHandle, &((VDEC_CMD_VPSS_FRAME_S *)arg)->eFramePackType);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_GetFrmPackingType err!:%d\n", s32Ret);
            }

            break;

        }

        case UMAPC_VDEC_CHAN_RESETVPSS:
        {
            s32Ret = VDEC_Chan_ResetVpss(hHandle);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_ResetVpss err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_SENDEOS:
        {
            s32Ret = VDEC_Chan_SendEos(hHandle);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_SendEos err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_GETPORTSTATE:
        {
            s32Ret = VDEC_Chan_GetPortState(hHandle, &((VDEC_CMD_VPSS_FRAME_S *)arg)->bAllPortComplete);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_GetPortState err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_GETFRMSTATUSINFO:
        {
            s32Ret = VDEC_Chan_GetFrmStatusInfo(hHandle, ((VDEC_CMD_VPSS_FRAME_S *)arg)->hPort, &((VDEC_CMD_VPSS_FRAME_S *)arg)->stVdecFrmStatusInfo);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_GetFrmStatusInfo err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_SETPORTATTR:
        {
            s32Ret = VDEC_Chan_SetVpssAttr(hHandle, ((VDEC_CMD_VPSS_FRAME_S *)arg)->hPort, &((VDEC_CMD_VPSS_FRAME_S *)arg)->stPortCfg);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_SetVpssAttr err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_GETPORTATTR:
        {
            s32Ret = VDEC_Chan_GetVpssAttr(hHandle, ((VDEC_CMD_VPSS_FRAME_S *)arg)->hPort, &((VDEC_CMD_VPSS_FRAME_S *)arg)->stPortCfg);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_GetVpssAttr err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_SETEXTBUFFER:
        {
            s32Ret = VDEC_Chan_SetExtBuffer(hHandle, &((VDEC_CMD_VPSS_FRAME_S *)arg)->stBufferAttr);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_SetExtBuffer err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_SETBUFFERMODE:
        {
            s32Ret = VDEC_Chan_SetFrameBufferMode(hHandle, ((VDEC_CMD_SET_BUFFERMODE_S *)arg)->enFrameBufferMode);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_SetFrameBufferMode err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_CHECKANDDELBUFFER:
        {
            s32Ret = VDEC_Chan_CheckAndDelBuffer(hHandle, &(((VDEC_CMD_CHECKANDDELBUFFER_S *)arg)->stBufInfo));

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_SetExtBufferState err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_SETEXTBUFFERSTATE:
        {
            s32Ret = VDEC_Chan_SetExtBufferState(hHandle, ((VDEC_CMD_SETEXTBUFFERTATE_S *)arg)->enExtBufferState);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_SetExtBufferState err!:%d\n", s32Ret);
            }

            break;
        }

        case UMAPC_VDEC_CHAN_SETRESOLUTION:
        {
            s32Ret = VDEC_Chan_SetResolution(hHandle, ((VDEC_CMD_SETRESOLUTION_S *)arg)->stResolution);

            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VDEC("VDEC_Chan_SetResolution err!:%d\n", s32Ret);
            }

            break;
        }

        default:
            s32Ret = HI_FAILURE;
            break;
    }

    return s32Ret;
}

static HI_S32 VDEC_RegChanProc(HI_S32 s32Num)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
#if (1 == HI_PROC_SUPPORT)
    HI_CHAR aszBuf[16];
    DRV_PROC_ITEM_S *pstItem;

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
#endif
#endif

    return HI_SUCCESS;
}

static HI_VOID VDEC_UnRegChanProc(HI_S32 s32Num)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
#if (1 == HI_PROC_SUPPORT)
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
#endif
#endif

    return;
}

static HI_VOID VDEC_TimerFunc(HI_LENGTH_T value)
{
    HI_HANDLE hHandle;
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    VDEC_CHAN_STATINFO_S *pstStatInfo = HI_NULL;

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

static SINT32 VDEC_RlsPreMMZNode(HI_HANDLE hHandle)
{
    SINT32 s32Ret = HI_SUCCESS;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    UINT32 i = 0;

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("%s, pstChan = NULL\n", __FUNCTION__);
        return HI_FAILURE;
    }

    for (i = 0; i < g_VdecPreMMZNodeNum; i++)
    {
        if ((1 == st_VdecChanPreUsedMMZInfo[i].u32NodeState)
            && (pstChan->stVDHMMZBuf.u32StartPhyAddr == st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr))
        {
            st_VdecChanPreUsedMMZInfo[i].u32NodeState = 0;
        }

        if (0 == st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr)
        {
            g_VdecPreMMZNodeNum--;
        }
    }

    return s32Ret;
}
static SINT32 VDEC_FindPreMMZNode(HI_HANDLE hHandle, MMZ_BUFFER_S *pstMMZbuffer)
{
    SINT32 s32Ret = HI_SUCCESS;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    UINT32 i = 0;

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("%s, pstChan = NULL\n", __FUNCTION__);
        return HI_FAILURE;
    }

    //在预分配中寻找可用的节点
    for (i = 0; i < g_VdecPreMMZNodeNum; i++)
    {
        if ((0 == st_VdecChanPreUsedMMZInfo[i].u32NodeState) && \
            (st_VdecChanPreUsedMMZInfo[i].u32Size >= pstMMZbuffer->u32Size))
        {
            if (HI_SUCCESS == VDEC_Chan_FindPreMMZ(pstMMZbuffer))
            {
                pstChan->stVDHMMZBuf.u32Size         = pstMMZbuffer->u32Size;
                pstChan->stVDHMMZBuf.u32StartPhyAddr = pstMMZbuffer->u32StartPhyAddr;
                pstChan->stVDHMMZBuf.pu8StartVirAddr = pstMMZbuffer->pu8StartVirAddr;
            }
            else
            {
                HI_ERR_VDEC("%s, Can't find pre mmz node!\n", __FUNCTION__);
            }

            break;
        }
    }

    if (i >= g_VdecPreMMZNodeNum)
    {
        for (i = 0; i < g_VdecPreMMZNodeNum; i++)
        {
            if ((0 == st_VdecChanPreUsedMMZInfo[i].u32NodeState) && (0 != st_VdecChanPreUsedMMZInfo[i].u32Size))
            {
                pstChan->stVDHMMZBuf.u32Size         = st_VdecChanPreUsedMMZInfo[i].u32Size;
                pstChan->stVDHMMZBuf.u32StartPhyAddr = st_VdecChanPreUsedMMZInfo[i].u32StartPhyAddr;
                pstChan->stVDHMMZBuf.pu8StartVirAddr = (HI_U8*)(st_VdecChanPreUsedMMZInfo[i].u32StartVirAddr);
                st_VdecChanPreUsedMMZInfo[i].u32NodeState = 1;
                g_VdecPreMMZNodeNum++;
                break;
            }
        }
    }

    if (i < g_VdecPreMMZNodeNum) //找到了合适的预分配节点
    {
        s32Ret = HI_SUCCESS;
    }
    else
    {
        s32Ret = HI_FAILURE;
    }

    return s32Ret;

}

#ifndef Hi3716MV410
static SINT32 VDEC_AllocMemForVfmw(HI_HANDLE hHandle)
{
    SINT32 s32Ret = HI_SUCCESS;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    VDEC_CHAN_FRAME_STORE_PARAM_S stFsParam;
    MMZ_BUFFER_S stMMZBuffer;
    UINT32 u32NeededFrameNum = 0;;

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    memset(&stMMZBuffer, 0, sizeof(stMMZBuffer));
    memset(&stFsParam, 0, sizeof(stFsParam));

    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("%s, pstChan = NULL\n", __FUNCTION__);
        return HI_FAILURE;
    }

    if ((1 == pstChan->stOption.u32DynamicFrameStoreAllocEn) && (HI_TRUE == pstChan->bNeedAlloc))
    {
        s32Ret = VDEC_RlsPreMMZNode(hHandle);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("VDEC_RlsPreMMZNode err!ret:%d\n", s32Ret);
        }

        //如果使用动态帧存自适应功能，则用流本身的信息来分配帧存
        if (1 == pstChan->stOption.u32SelfAdaptionDFS)
        {
            pstChan->stOption.u32CfgFrameNum = pstChan->u32RefFrameNum;
        }

        u32NeededFrameNum = (pstChan->stOption.u32CfgFrameNum + pstChan->stOption.s32ExtraFrameStoreNum);
        stMMZBuffer.u32Size = (pstChan->u32FrameSize) * (u32NeededFrameNum);//计算需要的帧存大小

#ifdef HI_TEE_SUPPORT

        if (pstChan->bTvp)
        {
            stFsParam.PhyAddr = HI_NULL;
            stFsParam.VirAddr = HI_NULL;
            stFsParam.Length  = 0;
            stFsParam.FrameNum = u32NeededFrameNum;
            stFsParam.NeedMMZ = pstChan->stOption.u32NeedMMZ;
        }
        else
#endif
        {
            if (stMMZBuffer.u32Size > pstChan->stOption.u32MaxMemUse) //超过最大可用帧存大小
            {
                //pstChan->stOption.u32NeedMMZ = 0;
                stFsParam.PhyAddr = HI_NULL;
                stFsParam.VirAddr = HI_NULL;
                stFsParam.Length  = 0;
                stFsParam.FrameNum = u32NeededFrameNum;
                stFsParam.NeedMMZ = 0;//pstChan->stOption.u32NeedMMZ;
            }
            else
            {
                if (HI_SUCCESS == VDEC_FindPreMMZNode(hHandle, &stMMZBuffer)) //找到了合适的预分配节点
                {
                    stFsParam.PhyAddr = pstChan->stVDHMMZBuf.u32StartPhyAddr;
                    stFsParam.VirAddr = (HI_U32)pstChan->stVDHMMZBuf.pu8StartVirAddr;
                    stFsParam.Length  = pstChan->stVDHMMZBuf.u32Size;
                    stFsParam.FrameNum = u32NeededFrameNum;
                    stFsParam.NeedMMZ = pstChan->stOption.u32NeedMMZ;
                }
                else
                {
                    stFsParam.PhyAddr = HI_NULL;
                    stFsParam.VirAddr = HI_NULL;
                    stFsParam.Length  = 0;
                    stFsParam.FrameNum = u32NeededFrameNum;
                    stFsParam.NeedMMZ = pstChan->stOption.u32NeedMMZ;
                }
            }
        }

        pstChan->u8NeedMMZ = stFsParam.NeedMMZ;
        s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_ALLOC_MEM_TO_CHANNEL, (HI_VOID *)&stFsParam);
        pstChan->bNeedAlloc = HI_FALSE;
    }

    return s32Ret;
}
#endif

#ifdef Hi3716MV410
static HI_S32 VDEC_ReleaseMemForDecoder(HI_HANDLE hHandle)
{
    HI_U32 i = 0;

    HI_S32 ret = HI_SUCCESS;

    VDEC_CHANNEL_S *pstChan = NULL;

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;

    //HI_ERR_VDEC("FUNCTION: %s \t LINE: %d \t pstChan->mmzAllocFrameNum = %d\n", __FUNCTION__, __LINE__, pstChan->mmzAllocFrameNum);

    //for (i = 0; i < pstChan->mmzAllocFrameNum; i++)
    // 这个地方可能会存在内存泄露
    for (i = 0; i < 64; i++)
    {
        //HI_ERR_VDEC("FUNCTION: %s \t LINE: %d \t pstChan->frmBufRec[%d].u32StartPhyAddr = 0x%x\n", __FUNCTION__, __LINE__, i, pstChan->frmBufRec[i].u32StartPhyAddr);
        //HI_ERR_VDEC("FUNCTION: %s \t LINE: %d \t pstChan->frmBufRec[%d].u32StartVirAddr = 0x%x\n", __FUNCTION__, __LINE__, i, pstChan->frmBufRec[i].u32StartVirAddr);

        if (pstChan->specialFrameExist == 1)
        {
            if (!((pstChan->specialFramePhyAddr >= pstChan->frmBufRec[i].u32StartPhyAddr)
                  && (pstChan->specialFramePhyAddr < pstChan->frmBufRec[i].u32StartPhyAddr + pstChan->frmBufRec[i].u32Size)))
            {
                //HI_ERR_VDEC("FUNCTION: %s \t LINE: %d\n", __FUNCTION__, __LINE__);
                HI_DRV_MMZ_UnmapAndRelease(&(pstChan->frmBufRec[i]));

                VDEC_DelFrmBufRec(hHandle, &(pstChan->frmBufRec[i]));
            }
        }
        else
        {
            //HI_ERR_VDEC("FUNCTION: %s \t LINE: %d\n", __FUNCTION__, __LINE__);
            HI_DRV_MMZ_UnmapAndRelease(&(pstChan->frmBufRec[i]));

            VDEC_DelFrmBufRec(hHandle, &(pstChan->frmBufRec[i]));
        }
    }

    pstChan->configuredFrameNum = 0;

    return ret;

}


static SINT32 VDEC_AllocMemForDecoder(HI_HANDLE hHandle)
{
    SINT32 s32Ret = HI_SUCCESS;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    VDEC_CHAN_FRAME_STORE_PARAM_S stFsParam;
    MMZ_BUFFER_S stMMZBuffer;
    FRAME_BUF_S  stFrameBuf;
    UINT32 u32NeededFrameNum = 0;

    HI_S32 i = 0;

    pstChan = s_stVdecDrv.astChanEntity[hHandle].pstChan;
    memset(&stMMZBuffer, 0, sizeof(stMMZBuffer));
    memset(&stFsParam, 0, sizeof(stFsParam));

    if (HI_NULL == pstChan)
    {
        HI_ERR_VDEC("%s, pstChan = NULL\n", __FUNCTION__);
        return HI_FAILURE;
    }

    if ((1 == pstChan->stOption.u32DynamicFrameStoreAllocEn) && (HI_TRUE == pstChan->bNeedAlloc))
    {
#ifdef Hi3716MV410
        s32Ret = VDEC_SetChanControlState(hHandle, ALLOC_FRAME_BUF_TO_CHAN);

        if (s32Ret == HI_FAILURE)
        {
            return s32Ret;
        }

#endif
        // 清空通道里帧存分配的记录
        VDEC_ResetChanFrmBuf(hHandle);

        s32Ret = VDEC_RlsPreMMZNode(hHandle);
#ifdef Hi3716MV410
        s32Ret = VDEC_ReleaseMemForDecoder(hHandle);
#endif

        // 如果使用动态帧存自适应功能，则用流本身的信息来分配帧存
        if (1 == pstChan->stOption.u32SelfAdaptionDFS)
        {
            pstChan->stOption.u32CfgFrameNum = pstChan->u32RefFrameNum;
        }


        u32NeededFrameNum = (pstChan->stOption.u32CfgFrameNum + pstChan->stOption.s32ExtraFrameStoreNum);
        pstChan->needFrameNum = u32NeededFrameNum;
        stMMZBuffer.u32Size = (pstChan->u32FrameSize) * (u32NeededFrameNum);//计算需要的帧存大小


        if (stMMZBuffer.u32Size > pstChan->stOption.u32MaxMemUse) //超过最大可用帧存大小
        {
            //pstChan->stOption.u32NeedMMZ = 0;
            stFsParam.PhyAddr = HI_NULL;
            stFsParam.VirAddr = HI_NULL;
            stFsParam.Length  = 0;
            stFsParam.FrameNum = u32NeededFrameNum;
            stFsParam.NeedMMZ = 0;//pstChan->stOption.u32NeedMMZ;
        }
        else
        {
            if (HI_SUCCESS == VDEC_FindPreMMZNode(hHandle, &stMMZBuffer)) //找到了合适的预分配节点
            {
                stFsParam.PhyAddr = pstChan->stVDHMMZBuf.u32StartPhyAddr;
                stFsParam.VirAddr = (HI_U32)pstChan->stVDHMMZBuf.pu8StartVirAddr;
                stFsParam.Length  = pstChan->stVDHMMZBuf.u32Size;
                stFsParam.FrameNum = u32NeededFrameNum;
                stFsParam.NeedMMZ = pstChan->stOption.u32NeedMMZ;
            }
            else
            {
                stFsParam.PhyAddr = HI_NULL;
                stFsParam.VirAddr = HI_NULL;
                stFsParam.Length  = 0;
                stFsParam.FrameNum = u32NeededFrameNum;
                stFsParam.NeedMMZ = pstChan->stOption.u32NeedMMZ;
            }
        }

        pstChan->u8NeedMMZ = stFsParam.NeedMMZ;

        pstChan->preAllocFrameNum = stFsParam.Length / pstChan->u32FrameSize;

        pstChan->mmzAllocFrameNum = (pstChan->needFrameNum > pstChan->preAllocFrameNum)
                                    ? (pstChan->needFrameNum - pstChan->preAllocFrameNum) : 0;

        if (HI_SUCCESS != VDEC_AllocPREMem(hHandle, pstChan->preAllocFrameNum))
        {

            return HI_FAILURE;
        }

        // 先将预分配帧存配置下去, 再配置动态分配帧存, 配置后将isConfigured标识置起
        for (i = 0; i < 64; i++)
        {
            if ((0 == pstChan->frmBufArr[i].isConfigured) && (1 == pstChan->frmBufArr[i].isAvailable))
            {
                stFrameBuf.u32StartPhyAddr = pstChan->frmBufArr[i].frameBuffer.u32StartPhyAddr;
                stFrameBuf.u32StartVirAddr = (HI_U32)pstChan->frmBufArr[i].frameBuffer.pu8StartVirAddr;
                stFrameBuf.u32Size         = pstChan->u32FrameSize;
                stFrameBuf.bufNodeStatus   = pstChan->frmBufArr[i].bufferNodeStatus;
                stFrameBuf.frameNum        = u32NeededFrameNum;

                s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_ALLOC_MEM_TO_DECODER, (HI_VOID *)&stFrameBuf);

                if (s32Ret == HI_SUCCESS)
                {
                    pstChan->frmBufArr[i].isConfigured = 1;
                    pstChan->configuredFrameNum++;
                }
            }
        }

        // 这里暂时将动态分配帧存也分配出来并且配置下去,
        // 后面将改成如果有预分配帧存，就先配置下去，并且将帧存划分成功标志置起
        // 再去按照需要动态分配的帧数，一帧一帧地申请并且配置下去
        VDEC_AllocMMZMem(hHandle, pstChan->mmzAllocFrameNum);

        for (i = 0; i < 64; i++)
        {
            if ((0 == pstChan->frmBufArr[i].isConfigured) && (1 == pstChan->frmBufArr[i].isAvailable))
            {
                stFrameBuf.u32StartPhyAddr = pstChan->frmBufArr[i].frameBuffer.u32StartPhyAddr;
                stFrameBuf.u32StartVirAddr = (HI_U32)pstChan->frmBufArr[i].frameBuffer.pu8StartVirAddr;
                stFrameBuf.u32Size         = pstChan->u32FrameSize;
                stFrameBuf.bufNodeStatus   = pstChan->frmBufArr[i].bufferNodeStatus;
                stFrameBuf.frameNum        = u32NeededFrameNum;

                s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_ALLOC_MEM_TO_DECODER, (HI_VOID *)&stFrameBuf);

                if (s32Ret == HI_SUCCESS)
                {
                    pstChan->frmBufArr[i].isConfigured = 1;
                    pstChan->configuredFrameNum++;
                }
            }
        }

        pstChan->bNeedAlloc = HI_FAILURE;
#ifdef Hi3716MV410
        s32Ret = VDEC_SetChanControlState(hHandle, NORMAL_CHAN);
#endif
    }

    return s32Ret;
}
#endif

static SINT32 VdecTaskFunc(VOID *param)   //l00273086
{
    UINT32 i = 0;
    SINT32 s32Ret = HI_SUCCESS;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    u32TaskRunFlag = 1;//标识Task开始运转，防止task还没有开始run就被强制close导致挂住

    while (1)
    {
        /* Lock */
        if (down_interruptible(&s_stVdecDrv.stSem))
        {
            HI_ERR_VDEC("Global lock err!\n");
            continue;
        }

        for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
        {
            pstChan = s_stVdecDrv.astChanEntity[i].pstChan;

            if ((HI_NULL != pstChan) && (VDEC_CHAN_STATE_RUN == pstChan->enCurState))
            {
                //To do
                VDEC_CHAN_TRY_USE_DOWN(&s_stVdecDrv.astChanEntity[i]);

                if (HI_SUCCESS != s32Ret)
                {
                    // HI_ERR_VDEC("%s Lock %d err!\n", __FUNCTION__, i);
                    continue;
                }

#ifndef Hi3716MV410
                s32Ret = VDEC_AllocMemForVfmw(i);
#else
                s32Ret = VDEC_AllocMemForDecoder(i);
#endif
                VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
            }
            else
            {
                //To do
            }

            //VDEC_CHAN_USE_UP(&s_stVdecDrv.astChanEntity[i]);
        }

        up(&s_stVdecDrv.stSem);

        if (i >= HI_VDEC_MAX_INSTANCE_NEW)
        {
            msleep(s_VdecTaskSleepTimems);
        }

        /*
        else
        {
            HI_INFO_VDEC("%s wrong Chan! i: %d\n", __FUNCTION__, i);
        }
        */
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

HI_S32 HI_DRV_VDEC_GetPreAllocVideoBuffer(VDEC_MMZ_BUFFER_S *pstMMZBuf)
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

    s32Ret = down_interruptible(&s_stVdecDrv.stSem);

    /*1:destroy all port*/
    for (i = 0; i < VDEC_MAX_PORT_NUM; i++)
    {
        hPort = s_stVdecDrv.astChanEntity[hHandle].stVpssChan.stPort[i].hPort;

        if (hPort != HI_INVALID_HANDLE)
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

        VDEC_Chan_FreeHandle(hHandle);
    }

    return HI_SUCCESS;
}

static HI_S32 VDEC_OpenDev(HI_VOID)
{
    HI_U32 i;
    HI_S32 s32Ret;
    VDEC_OPERATION_S stOpt;

    /* Init global parameter */
    HI_INIT_MUTEX(&s_stVdecDrv.stSem);
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
    stOpt.eAdapterType = ADAPTER_TYPE_VDEC;
    stOpt.VdecCallback = VDEC_EventHandle;
    stOpt.mem_malloc = HI_NULL;
    stOpt.mem_free = HI_NULL;
    s32Ret = (s_stVdecDrv.pVfmwFunc->pfnVfmwInitWithOperation)(&stOpt);

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

    /**********************Creat Vdec task //l00273086 start*************************/
    if (HI_NULL == s_stVdecDrv.pVdecTask) //l00273086
    {
        s_stVdecDrv.pVdecTask = kthread_create(VdecTaskFunc, (VOID *)NULL, "VdecKernelThread");

        if (IS_ERR(s_stVdecDrv.pVdecTask))
        {
            HI_FATAL_VDEC("VDEC can not create thread!\n");
            goto err2;
        }
    }

    /**********************Creat Vdec task //l00273086 end**************************/
    HI_INFO_VDEC("VDEC_OpenDev OK.\n");
    return HI_SUCCESS;

err2:
    (s_stVdecDrv.pVfmwFunc->pfnVfmwExit)();
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
    del_timer_sync(&s_stVdecDrv.stTimer);

    /*stop task*/
    if ((HI_NULL != s_stVdecDrv.pVdecTask) && (1 == u32TaskRunFlag))
    {
        kthread_stop(s_stVdecDrv.pVdecTask);
        msleep(1);
        s_stVdecDrv.pVdecTask = HI_NULL;
    }//l00273086

    /* Free all channels */
    for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
    {
        if (HI_NULL != s_stVdecDrv.astChanEntity[i].pstChan)
        {
            VDEC_DRV_FreeAll(i);
        }
    }

    /* Vfmw exit */
    (s_stVdecDrv.pVfmwFunc->pfnVfmwExit)();
#ifdef HI_TEE_SUPPORT
    (s_stVdecDrv.pVfmwFunc->pfnVfmwTrustDecoderExit)();
#endif

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

HI_S32 VDEC_DRV_Open(struct inode *inode, struct file  *filp)
{
    HI_S32 s32Ret;

    if (atomic_inc_return(&s_stVdecDrv.atmOpenCnt) == 1)
    {
        s_stVdecDrv.pDmxFunc  = HI_NULL;
        s_stVdecDrv.pVfmwFunc = HI_NULL;
        s_stVdecDrv.pVpssFunc = HI_NULL;

        /* Get demux functions */
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_DEMUX, (HI_VOID **)&s_stVdecDrv.pDmxFunc);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VDEC("Get demux function err:%#x!\n", s32Ret);
        }

        /* Get vfmw functions */
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_VFMW, (HI_VOID **)&s_stVdecDrv.pVfmwFunc);

        /* Check vfmw functions */
        if ((HI_SUCCESS != s32Ret)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc->pfnVfmwInitWithOperation)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc->pfnVfmwExit)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc->pfnVfmwControl)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc->pfnVfmwSuspend)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc->pfnVfmwResume)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc->pfnVfmwSetDbgOption))
        {
            HI_FATAL_VDEC("Get vfmw function err!\n");
            goto err;
        }

#ifdef HI_TEE_SUPPORT

        if ((HI_NULL == s_stVdecDrv.pVfmwFunc->pfnVfmwTrustDecoderInit)
            || (HI_NULL == s_stVdecDrv.pVfmwFunc->pfnVfmwTrustDecoderExit))
        {
            HI_FATAL_VDEC("Get trusted vfmw function err!\n");
            goto err;
        }

#endif
        /*Get vpss functions*/
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_VPSS, (HI_VOID **)&s_stVdecDrv.pVpssFunc);

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

HI_S32 VDEC_DRV_Release(struct inode *inode, struct file *filp)
{
    HI_S32 i;
    HI_S32 s32Ret;

    /* Not the last close, only close the channel match with the 'filp' */
    if (0 != atomic_dec_return(&s_stVdecDrv.atmOpenCnt))
    {
        for (i = 0; i < HI_VDEC_MAX_INSTANCE_NEW; i++)
        {
            VDEC_CHAN_ENTITY_S *Chan = &s_stVdecDrv.astChanEntity[i];

            if (Chan->u32File != ((HI_U32)filp))
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
        st_VdecChanPreUsedMMZInfo[0].u32StartVirAddr = (HI_U32)g_stVDHMMZ.pu8StartVirAddr;
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

HI_S32 VDEC_DRV_RegisterProc(VDEC_REGISTER_PARAM_S *pstParam)
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

HI_S32 VDEC_DRV_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
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

HI_S32 VDEC_DRV_Resume(PM_BASEDEV_S *pdev)
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

/*this function is the interface of controlling by proc file system*/
/*CNcomment: 通过proc文件系统进行控制的函数入口*/
HI_S32 VDEC_DRV_DebugCtrl(HI_U32 u32Para1, HI_U32 u32Para2)
{
    HI_INFO_VDEC("Para1=0x%x, Para2=0x%x\n", u32Para1, u32Para2);
    (s_stVdecDrv.pVfmwFunc->pfnVfmwSetDbgOption)(u32Para1, (HI_U8 *)&u32Para2);

    return HI_SUCCESS;
}

VDEC_CHANNEL_S *VDEC_DRV_GetChan(HI_HANDLE hHandle)
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

VFMW_EXPORT_FUNC_S *VDEC_DRV_GetVfmwExportFunc(HI_VOID)
{
    return s_stVdecDrv.pVfmwFunc;
}

static HI_VOID VDEC_GetMemConfig(HI_U32 *pu32ScdBufSize, HI_U32 *pu32VdhBufSize)
{
#ifndef Hi3716MV410
    HI_S32              Ret;
    HI_SYS_MEM_CONFIG_S MemConfig = {0};
#endif

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

    if (((HI_CHIP_TYPE_HI3798C_A == ChipType && HI_CHIP_VERSION_V200 == ChipVersion))
		||((HI_CHIP_TYPE_HI3798C_B == ChipType && HI_CHIP_VERSION_V200 == ChipVersion)))
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
        ret = HI_DRV_MMZ_Alloc("VFMW_SCD_PRE", HI_NULL, ScdBufSize + HI_VDEC_SCD_EXT_MEM + HI_VDEC_EOS_MEM_SIZE, 0, &g_stSCDMMZ);

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
        ret = HI_DRV_MMZ_Alloc("VFMW_VDH_PRE", HI_NULL, VdhBufSize, 0, &g_stVDHMMZ);

        if (HI_SUCCESS != ret)
        {
            HI_ERR_VDEC("VDH Alloc 0x%x failed,ScdBufSize:0x%x\n", VdhBufSize, ScdBufSize);
			#if (1 == PRE_ALLOC_VDEC_SCD_MMZ)
            goto err1;
			#endif
        }

        ret = HI_DRV_MMZ_Map(&g_stVDHMMZ);

        if (HI_SUCCESS != ret)
        {
            HI_ERR_VDEC("VDH Map failed\n");
            goto err2;
        }

        st_VdecChanPreUsedMMZInfo[0].u32Size         = g_stVDHMMZ.u32Size;
        st_VdecChanPreUsedMMZInfo[0].u32StartPhyAddr = g_stVDHMMZ.u32StartPhyAddr;
        st_VdecChanPreUsedMMZInfo[0].u32StartVirAddr = (HI_U32)g_stVDHMMZ.pu8StartVirAddr;
        st_VdecChanPreUsedMMZInfo[0].u32NodeState    = 0;
    }

#endif

    /*4:alloc pre es_buffer memory*/
#if (1 == PRE_ALLOC_VDEC_ESBUF_MMZ)
    ret = HI_DRV_MMZ_AllocAndMap("VDEC_ESBuf_PRE", HI_NULL, ESBUF_MMZ_SIZE, 0, &g_stESBUFMMZ);

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
    HI_DRV_MMZ_Unmap(&g_stVDHMMZ);
    memset(&st_VdecChanPreUsedMMZInfo[0], 0, sizeof(VDEC_PREMMZ_NODE_S));
#endif
#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
err2:
    HI_DRV_MMZ_Release(&g_stVDHMMZ);
    memset(&g_stVDHMMZ, 0, sizeof(MMZ_BUFFER_S));
#endif
#if (1 == PRE_ALLOC_VDEC_SCD_MMZ)
err1:
    HI_DRV_MMZ_Release(&g_stSCDMMZ);
    memset(&g_stSCDMMZ, 0, sizeof(MMZ_BUFFER_S));
#endif
    HI_ERR_VDEC("ScdBufSize:%d,VdhBufSize:%d\n", ScdBufSize, VdhBufSize);
    return HI_FAILURE;

}
HI_S32 VDEC_DRV_Init(HI_VOID)
{
    HI_S32              ret;

    BUFMNG_SaveInit();

    ret = HI_DRV_MODULE_Register(HI_ID_VDEC, VDEC_NAME, (HI_VOID *)&s_stVdecDrv.stExtFunc);

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

    return HI_SUCCESS;
}

HI_VOID VDEC_DRV_Exit(HI_VOID)
{
#if (1 == PRE_ALLOC_VDEC_ESBUF_MMZ)

    if (0 != g_stESBUFMMZ.u32Size)
    {
        HI_DRV_MMZ_Release(&g_stESBUFMMZ);
        g_stESBUFMMZ.u32Size = 0;
    }

#endif

#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)

    if (0 != g_stVDHMMZ.u32Size)
    {
        HI_DRV_MMZ_Unmap(&g_stVDHMMZ);
        HI_DRV_MMZ_Release(&g_stVDHMMZ);
        g_stVDHMMZ.u32Size = 0;
    }

#endif

#if (1 == PRE_ALLOC_VDEC_SCD_MMZ)

    if (0 != g_stSCDMMZ.u32Size)
    {
        HI_DRV_MMZ_UnmapAndRelease(&g_stSCDMMZ);
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

HI_S32 HI_DRV_VDEC_AllocChan(HI_HANDLE *phHandle, HI_UNF_AVPLAY_OPEN_OPT_S *pstCapParam)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss = HI_INVALID_HANDLE;

    s32Ret = VDEC_Chan_AllocHandle(phHandle, (struct file *)MCE_INVALID_FILP);

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
HI_S32 HI_DRV_VDEC_CreateStrmBuf(HI_HANDLE hVdec, HI_DRV_VDEC_STREAM_BUF_S *pstBuf)
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

HI_S32 HI_DRV_VDEC_CreatePort(HI_HANDLE hVdec, HI_HANDLE *phPort, VDEC_PORT_ABILITY_E ePortAbility)
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

HI_S32 HI_DRV_VDEC_GetPortParam(HI_HANDLE hVdec, HI_HANDLE hPort, VDEC_PORT_PARAM_S *pstPortParam)
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
HI_S32 HI_DRV_VDEC_Chan_RecvVpssFrmBuf(HI_HANDLE hVdec, HI_DRV_VIDEO_FRAME_PACKAGE_S *pstFrm)
{
    HI_S32 s32Ret;
    HI_HANDLE hVpss;
    s32Ret = VDEC_FindVpssHandleByVdecHandle(hVdec & 0xff, &hVpss);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VDEC("VDEC_FindVpssHandleByVdecHandle err!\n");
        return HI_FAILURE;
    }

    return VDEC_Chan_RecvVpssFrmBuf(hVpss, pstFrm);
}
HI_S32 HI_DRV_VDEC_SetChanFrmRate(HI_HANDLE hHandle, HI_UNF_AVPLAY_FRMRATE_PARAM_S *pstFrmRate)
{
    return PTSREC_GetFrmRate(hHandle, pstFrmRate);
}
HI_S32 HI_DRV_VDEC_GetChanFrmRate(HI_HANDLE hHandle, HI_UNF_AVPLAY_FRMRATE_PARAM_S *pstFrmRate)
{
    return PTSREC_SetFrmRate(hHandle, pstFrmRate);
}
HI_VOID HI_DRV_VDEC_GetVcmpFlag(HI_BOOL *pbVcmpFlag)
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

HI_S32 HI_DRV_VDEC_GetCap(VDEC_CAP_S *pstCap)
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

VDEC_CHANNEL_S *HI_DRV_VDEC_DRV_GetChan(HI_HANDLE hHandle)
{
    VDEC_CHANNEL_S *stVdecChn = HI_NULL;
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
//EXPORT_SYMBOL(VDEC_CreateStrmBuf);
EXPORT_SYMBOL(HI_DRV_VDEC_Chan_AttachStrmBuf);
EXPORT_SYMBOL(HI_DRV_VDEC_Chan_DetachStrmBuf);
EXPORT_SYMBOL(HI_DRV_VDEC_GetCap);
EXPORT_SYMBOL(HI_DRV_VDEC_DRV_GetChan);
EXPORT_SYMBOL(HI_DRV_VDEC_RegisterEventCallback);
EXPORT_SYMBOL(HI_DRV_VDEC_RegisterDmxHdlCallback);
EXPORT_SYMBOL(VDEC_Chan_FindPreMMZ);
EXPORT_SYMBOL(VDEC_Chan_ReleasePreMMZ);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
