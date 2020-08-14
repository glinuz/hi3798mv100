/*
 * Copyright (c) (2014 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    omxvdec.h
 *
 * Purpose: omxvdec main entry header file
 *
 * Author:  yangyichang 00226912
 *
 * Date:    26, 11, 2014
 *
 */

#ifndef __OMXVDEC_H__
#define __OMXVDEC_H__

#include <linux/cdev.h>
#include "hi_drv_mmz.h"       //系统提供内存相关接口
#include "hi_drv_sys.h"       //HI_DRV_SYS_GetTimeStampMs依赖
#include "hi_drv_proc.h"      //proc相关接口依赖
#include "drv_omxvdec.h"
#include "omxvdec_mem.h"
#include "drv_omxvdec_pts_recv.h"
#include "drv_vdec_ext.h"
#include "hi_drv_module.h"  // HI_DRV_MODULE_GetFunction 依赖

#ifdef VFMW_VPSS_BYPASS_EN
#include "hi_kernel_adapt.h"    // add by l00228308
#endif

#define MAX_OPEN_COUNT                    (32)
#define MAX_CHANNEL_NUM                   (MAX_OPEN_COUNT)

#define OMX_ALWS                          (0xFFFFFFFF)
#define OMX_FATAL                         (0)
#define OMX_ERR                           (1)
#define OMX_WARN                          (2)
#define OMX_INFO                          (3)
#define OMX_TRACE                         (4)
#define OMX_INBUF                         (5)
#define OMX_OUTBUF                        (6)
#define OMX_VPSS                          (7)
#define OMX_VER                           (8)
#define OMX_PTS                           (9)

#define OMX_LOWDELAY_REC_NODE_NUM           10
#define OMX_LOWDELAY_REC_ITEM               9
#define OMX_LOWDELAY_REC_USERTAG            0
#define OMX_LOWDELAY_REC_NODE_STATE         1
#define OMX_LOWDELAY_REC_ETB_TIME           2
#define OMX_LOWDELAY_REC_VFMW_RCV_STRM_TIME 3
#define OMX_LOWDELAY_REC_VFMW_RLS_STRM_TIME 4
#define OMX_LOWDELAY_REC_VFMW_RPO_IMG_TIME  5
#define OMX_LOWDELAY_REC_VPSS_RCV_IMG_TIME  6
#define OMX_LOWDELAY_REC_VPSS_RPO_IMG_TIME  7
#define OMX_LOWDELAY_REC_TOTAL_USED_TIME    8
#define OMX_LOWDELAY_REC_NODE_FREE          0
#define OMX_LOWDELAY_REC_NODE_WRITED        1

extern HI_U32 g_TraceOption;

#ifndef HI_ADVCA_FUNCTION_RELEASE
#define OmxPrint(flag, format,arg...) \
    do { \
        if (OMX_ALWS == flag || (0 != (g_TraceOption & (1 << flag)))) \
            printk("<OMXVDEC:%d> " format, __LINE__, ## arg); \
    } while (0)
#else
#define OmxPrint(flag, format,arg...)    ({do{}while(0);0;})
#endif


#define OMXVDEC_ASSERT_RETURN(cond, else_print)                        \
do {                                                            \
    if( !(cond) )                                               \
    {                                                           \
        OmxPrint(OMX_ALWS,"ASSERT: %s at L%d:%s\n", __func__, __LINE__, else_print ); \
        return HI_FAILURE;                                       \
    }                                                           \
}while(0)

#define OMXVDEC_ASSERT_RETURN_NULL(cond, else_print)                        \
do {                                                            \
    if( !(cond) )                                               \
    {                                                           \
        OmxPrint(OMX_ALWS,"ASSERT: %s at L%d:%s\n", __func__, __LINE__, else_print ); \
        return;                                       \
    }                                                           \
}while(0)

#ifdef VFMW_VPSS_BYPASS_EN
#define D_OMXVDEC_CHECK_PTR_RET(ptr) \
    do {\
        if (HI_NULL == ptr)\
        { \
            OmxPrint(OMX_ERR, "PTR '%s' is NULL.\n", # ptr); \
            return HI_FAILURE;           \
        }  \
    } while (0)

#define D_OMXVDEC_CHECK_PTR(ptr) \
    do {\
        if (HI_NULL == ptr)\
        { \
            OmxPrint(OMX_ERR, "PTR '%s' is NULL.\n", # ptr); \
            return;           \
        }  \
    } while (0)
#endif

/*
   g_TraceOption 常用值

   1:      OMX_FATAL
   2:      OMX_ERR
   4:      OMX_WARN
   8:      OMX_INFO      (常用于查看创建配置项及跟踪最后一帧情况)
   16:     OMX_TRACE
   32:     OMX_INBUF
   64:     OMX_OUTBUF
   128:    OMX_VPSS      (常用于跟踪图像不输出原因)
   256:    OMX_RAWCTRL   (原用于码流输入控制，现已废弃)
   512:    OMX_PTS       (常用于查看输入输出PTS值)

   3:      OMX_FATAL & OMX_ERR
   7:      OMX_FATAL & OMX_ERR & OMX_WARN
   11:     OMX_FATAL & OMX_ERR & OMX_INFO
   19:     OMX_FATAL & OMX_ERR & OMX_TRACE
   96:     OMX_INBUF & OMX_OUTBUF
   35:     OMX_FATAL & OMX_ERR & OMX_INBUF
   67:     OMX_FATAL & OMX_ERR & OMX_OUTBUF
   99:     OMX_FATAL & OMX_ERR & OMX_INBUF & OMX_OUTBUF

*/

typedef enum {
    TASK_STATE_INVALID = 0,
    TASK_STATE_SLEEPING,
    TASK_STATE_ONCALL,
    TASK_STATE_RUNNING,
}eTASK_STATE;

typedef struct {
    eTASK_STATE         task_state;
    wait_queue_head_t   task_wait;
    struct task_struct *task_thread;
}OMXVDEC_TASK;

typedef enum {
    ALLOC_INVALID = 0,
    ALLOC_BY_MMZ,
    ALLOC_BY_MMZ_UNMAP,
    ALLOC_BY_PRE,
    ALLOC_BY_SEC,
}eMEM_ALLOC;

typedef enum {
    SAVE_FLIE_RAW = 0,
    SAVE_FLIE_YUV,
    SAVE_FLIE_IMG,
}eSAVE_FLIE;

#ifdef VFMW_VPSS_BYPASS_EN
/* vdec remain frame List */
//add by l00225186
/* vdec remain frame List */
#define OMXVDEC_MAX_REMAIN_FRM_NUM (32)
typedef struct BUFMNG_VPSS_LOCK_S
{
    spinlock_t     irq_lock;
    unsigned long  irq_lockflags;
    int            isInit;
} OMXVDEC_IRQ_LOCK_S;

typedef struct tagVDEC_SPECIAL_INFO_S
{
    MMZ_BUFFER_S        frmBufRec;
    eMEM_ALLOC          enbufferNodeStatus;
    HI_BOOL             bCanRls;
}OMXVDEC_FRM_INFO_S;

typedef struct tagVDEC_LIST_S
{
    HI_BOOL              bInit;
    HI_S32               s32Num;
    spinlock_t           bypass_lock;
    OMXVDEC_FRM_INFO_S   stSpecialFrmRec[OMXVDEC_MAX_REMAIN_FRM_NUM];
}OMXVDEC_List_S;

typedef struct
{
    HI_U32  occoupy_frame_num;
    MMZ_BUFFER_S frmBufRec[VFMW_MAX_RESERVE_NUM];
} OMXVDEC_PROC_OCCOUPY_FRAME_INFO;

#endif

typedef struct {
    HI_U32              open_count;
    HI_U32              total_chan_num;
    OMXVDEC_TASK        task;
    unsigned long       chan_bitmap;
    struct list_head    chan_list;
    spinlock_t          lock;
    spinlock_t          channel_lock;
    struct cdev         cdev;
    struct device      *device;
#ifdef VFMW_VPSS_BYPASS_EN
    OMXVDEC_List_S      stRemainFrmList;
#endif
}OMXVDEC_ENTRY;

typedef struct {
    VOID*      pDecoderFunc;        /*decoder external functions*/
    VOID*      pProcessorFunc;      /*processor external functions*/
    VOID*      pVdecFunc;
}OMXVDEC_FUNC;

typedef struct
{
    HI_U32 usrtag_start_time;
    HI_U32 interval;
    HI_U32 current_tag;
    HI_U32 time_record[OMX_LOWDELAY_REC_NODE_NUM + 1][OMX_LOWDELAY_REC_ITEM];
    HI_U32 time_cost_sum;
    HI_U32 average_time_cost;
    HI_U32 rec_index;
}OMXVDEC_LOWDELAY_PROC_RECORD;


HI_VOID omxvdec_release_mem(HI_VOID *pMMZ_Buf, eMEM_ALLOC eMemAlloc);

UINT32 OMX_GetTimeInMs(VOID);

#ifdef VFMW_VPSS_BYPASS_EN

HI_S32 OMXVDEC_List_FindNode(OMXVDEC_List_S *pList,HI_U32 u32TargetPhyAddr,HI_U32 *pIndex);
//HI_S32 OMXVDEC_List_FindNodeCanRls(OMXVDEC_List_S *pList, HI_U32 *pIndex);
HI_S32 OMXVDEC_List_Add(OMXVDEC_List_S *pList,OMXVDEC_FRM_INFO_S *pSpecialFrmRec);
HI_S32 OMXVDEC_List_Del(OMXVDEC_List_S *pList,HI_U32 u32Index);
HI_S32 OMXVDEC_Frame_in_List(OMXVDEC_List_S *pList,OMXVDEC_FRM_INFO_S *pSpecialFrmRec);
#endif

#endif
