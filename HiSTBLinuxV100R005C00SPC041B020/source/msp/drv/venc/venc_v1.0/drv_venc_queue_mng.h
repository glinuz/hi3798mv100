/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

#ifndef __DRV_VENC_QUEUE_MNG_H__
#define __DRV_VENC_QUEUE_MNG_H__

#include <linux/list.h>
#include <linux/interrupt.h>

#include "hi_drv_venc.h"      //for omxvenc
#include "hi_drv_video.h"
#include "hi_type.h"


typedef struct queue_data
{
    struct list_head list;
    /*msg payload*/
    HI_BOOL  bToOMX;
    union
    {
       HI_DRV_VIDEO_FRAME_S queue_info;        
       venc_msginfo msg_info_omx;              //for omxvenc
    } unFrmInfo;	
} queue_data_s;

typedef struct queue_info
{
    spinlock_t msg_lock;
    wait_queue_head_t wait;

    HI_VOID* alloc_data;              //实际为  queue_data_s* 类型
    HI_U32   queue_num;
    HI_S32   stop;
    struct list_head head;
    struct list_head free;

    volatile HI_BOOL bSleep;
	volatile HI_BOOL bStrmLock;       // 线程与stop处理流程互斥 w00283384

    HI_BOOL bToOMX;                   //标志该队列是不是for omxvenc
} queue_info_s;

queue_info_s* VENC_DRV_MngQueueInit( HI_U32 depth, HI_BOOL bOMX);

HI_S32 VENC_DRV_MngQueueDeinit(queue_info_s* queue);

HI_S32 VENC_DRV_MngDequeue(queue_info_s* queue, queue_data_s* pQueue_data);

HI_S32 VENC_DRV_MngQueue(queue_info_s* queue, queue_data_s* pQueue_data, HI_U32 msgcode, HI_U32 status);

HI_S32 VENC_DRV_MngQueueEmpty(queue_info_s* queue);

HI_S32 VENC_DRV_MngQueueFull(queue_info_s* queue);

HI_S32 VENC_DRV_MngQueueRefresh(queue_info_s* queue);


#endif /* __DRV_VENC_QUEUE_MNG_H__ */
