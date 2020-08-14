/*
*  Copyright (c) 2016 MediaTek Inc.
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License version 2 as
*  published by the Free Software Foundation.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU General Public License for more details.
*/
#ifndef __BTMTK_USB_FIFO_H__
#define __BTMTK_USB_FIFO_H__

#include <linux/kfifo.h>    /* Used for kfifo */

#define FW_DUMP_END_EVENT "coredump end"

typedef enum {
    FIFO_SYSLOG = 0,
    FIFO_COREDUMP,
    FIFO_BTBUF,
    FIFO_END
} fifo_t;

typedef enum TSK_BIT {
    TSK_INIT = 0,
    TSK_START,
    TSK_RUNNING,
    TSK_SHOULD_STOP,
    TSK_EXIT,
} tsk_bit_t;

typedef struct btmtk_fio_stats_s {
    unsigned int rx;
    unsigned int tx;
} btfio_stats;

typedef struct btmtk_fifo_s {
    fifo_t          type;
    unsigned int    size;
    const char      *folder_name;
    struct __kfifo  kfifo;
    struct file     *filp;
    btfio_stats     stat;
} btmtk_fifo_t;

typedef struct btmtk_fifo_data_s {
    struct task_struct  *fifo_tsk;
    btmtk_fifo_t        *fifo_l;
    wait_queue_head_t   rx_wait_q;
    unsigned long       tsk_flag;
} btmtk_fifo_data_t;

/**
 * Extern functions
 */
void *btmtk_fifo_init(void);
int btmtk_fifo_start(void *fio_d);
u32 btmtk_fifo_in(fifo_t type, void *fifo_d, const void *buf,
        unsigned int length);

#endif /* __BTMTK_USB_FIFO_H__ */
