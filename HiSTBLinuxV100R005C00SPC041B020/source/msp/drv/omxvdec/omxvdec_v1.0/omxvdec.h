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
#include "drv_omxvdec_pts_recv.h"

#define BPP_MODE_ENABLE                   (0)

#define MAX_OPEN_COUNT                      (32)
#define MAX_CHANNEL_NUM                   (MAX_OPEN_COUNT)

#define OMX_ALWS                          (0xFFFFFFFF)
#define OMX_FATAL                          (0)
#define OMX_ERR                              (1)
#define OMX_WARN                          (2)
#define OMX_INFO                          (3)
#define OMX_TRACE                          (4)
#define OMX_INBUF                          (5)
#define OMX_OUTBUF                              (6)
#define OMX_VPSS                           (7)
#define OMX_VER                                (8)
#define OMX_PTS                           (9)

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
}OMXVDEC_ENTRY;

typedef struct {
  VOID*      pDecoderFunc;           /*decoder external functions*/
  VOID*      pProcessorFunc;       /*processor external functions*/
}OMXVDEC_FUNC;

typedef enum {
    ALLOC_BY_MMZ = 0,
    ALLOC_BY_PRE,
    ALLOC_BY_SEC,
}eMEM_ALLOC;

HI_VOID omxvdec_release_mem(MMZ_BUFFER_S *pMMZ_Buf, eMEM_ALLOC eMemAlloc);

#endif

