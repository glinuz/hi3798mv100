/*
 * Copyright (c) (2014 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    message.h
 *
 * Purpose: omxvdec message queue interface
 *
 * Author:  yangyichang 00226912
 *
 * Date:    26, 11, 2014
 *
 */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "omxvdec.h"


#define  QUEUE_DEFAULT_DEPTH         (50)

typedef struct msg_data {
    struct list_head list;
    /*msg payload*/
    OMXVDEC_MSG_INFO msg_info;
} msg_data_s;

typedef struct msg_queue {
    spinlock_t lock;
    wait_queue_head_t wait;

    HI_U8  quit;
    HI_VOID *alloc;
    HI_U32 msg_num;
    HI_U32 stop:1;
    struct list_head head;
    struct list_head free;
} msg_queue_s;


HI_S32 message_queue(msg_queue_s *queue, HI_U32 msgcode, HI_U32 status, HI_VOID *priv);

HI_S32 message_dequeue(msg_queue_s *queue, OMXVDEC_MSG_INFO *pmsg_info);

msg_queue_s* message_queue_init(HI_U32 max_msg_num);

HI_VOID message_queue_deinit(msg_queue_s *queue);

#endif
