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

#include "btmtk_define.h"

#if ENABLE_BT_FIFO_THREAD
#include <linux/kthread.h>
#include <asm/uaccess.h>

#include "btmtk_usb_fifo.h"
#include "btmtk_usb_main.h"

/**
 * Definition
 */
#define FW_DUMP_SIZE (1024 * 512)
#define SYS_LOG_SIZE (1024 * 512)

#define KFIO_D(pData, t)    (((btmtk_fifo_data_t *)(pData))->fifo_l[t].kfifo.data)
#define pKFIO(pData, t)     (&((btmtk_fifo_data_t *)(pData))->fifo_l[t].kfifo)
#define pFIO(pData, t)      (&((btmtk_fifo_data_t *)(pData))->fifo_l[t])

static btmtk_fifo_data_t g_fifo_data;

static btmtk_fifo_t btmtk_fifo_list[] = {
    { FIFO_SYSLOG, SYS_LOG_SIZE, SYS_LOG_FILE_NAME, {0}, NULL, {0} },
    { FIFO_COREDUMP, FW_DUMP_SIZE, FW_DUMP_FILE_NAME, {0}, NULL, {0} },
    { FIFO_END, 0, NULL, {0}, NULL, {0} },
    { FIFO_BTBUF, 0, NULL, {0}, NULL, {0} }
};

static int fifo_alloc(btmtk_fifo_t *bt_fifo)
{
    int ret = 0;
    struct __kfifo *kfio = &bt_fifo->kfifo;

    ret = __kfifo_alloc(kfio, bt_fifo->size, 1, GFP_KERNEL);
    if (ret == 0)
        bt_fifo->size = kfio->mask + 1;

    return ret;
}

static int fifo_init(btmtk_fifo_t *bt_fifo)
{
    int ret = 0;
    struct __kfifo *kfio = &bt_fifo->kfifo;

    BTUSB_INFO("%s: %d", __func__, bt_fifo->type);

    ret = __kfifo_init(kfio, kfio->data, bt_fifo->size, kfio->esize);

    if (bt_fifo->filp) {
        BTUSB_INFO("%s: call filp_close", __func__);
        vfs_fsync(bt_fifo->filp, 0);
        filp_close(bt_fifo->filp, NULL);
        bt_fifo->filp = NULL;
        BTUSB_WARN("%s: FW dump file closed, rx=0x%X, tx =0x%x",
                __func__, bt_fifo->stat.rx, bt_fifo->stat.tx);
    }

    bt_fifo->stat.rx = 0;
    bt_fifo->stat.tx = 0;
    return ret;
}

static void fifo_out_info(btmtk_fifo_data_t *data_p)
{
    struct __kfifo *fifo = NULL;

    if (data_p == NULL) {
        BTUSB_ERR("%s: data_p == NULL return", __func__);
        return;
    }

    fifo = pKFIO(data_p, FIFO_SYSLOG);

    if (fifo->data != NULL) {
        if (fifo->in > fifo->out + fifo->mask)
            BTUSB_WARN("sys_log buffer full");

        if (fifo->in == fifo->out + fifo->mask)
            BTUSB_WARN("sys_log buffer empty");

        BTUSB_DBG("[syslog][fifo in - fifo out = 0x%x]",
              fifo->in - fifo->out);
    }

    fifo = pKFIO(data_p, FIFO_COREDUMP);

    if (fifo->data != NULL) {
        if (fifo->in > fifo->out + fifo->mask)
            BTUSB_WARN("coredump buffer full");

        if (fifo->in == fifo->out + fifo->mask)
            BTUSB_WARN("coredump buffer empty");
    }
}

static u32 fifo_out_accessible(btmtk_fifo_data_t *data_p)
{
    int i;
    btmtk_fifo_t *fio_p = NULL;

    if (data_p == NULL)
        return 0;

    if (test_bit(TSK_SHOULD_STOP, &data_p->tsk_flag)) {
        BTUSB_WARN("%s: task should stop", __func__);
        return 1;
    }

    for (i = 0; i < FIFO_END; i++) {

        fio_p = &data_p->fifo_l[i];

        if (fio_p->type != i)
            continue;

        if (fio_p->kfifo.data != NULL) {
            if (fio_p->kfifo.in != fio_p->kfifo.out)
                return 1;
        }
    }

    return 0;
}

static struct file *fifo_filp_open(const char *file, int flags, umode_t mode,
        fifo_t type)
{
    struct file *f = NULL;
    /* Retry to open following path */
    static const char * const sys[] = {
        "/sdcard/"SYSLOG_FNAME, "/data/misc/bluedroid/"SYSLOG_FNAME, "/tmp/"SYSLOG_FNAME};
    static const char * const fw[] = {
        "/sdcard/"FWDUMP_FNAME, "/data/misc/bluedroid/"FWDUMP_FNAME, "/tmp/"SYSLOG_FNAME};
    u8 i = 0;

    f = filp_open(file, flags, mode);
    if (IS_ERR(f)) {
        const char *p = NULL;
        u8 count = sizeof(sys) / sizeof(sys[0]);

        f = NULL;
        BTUSB_ERR("%s: open file error: %s, try to open others default path",
                __func__, file);

        if (type != FIFO_SYSLOG && type != FIFO_COREDUMP) {
            BTUSB_ERR("%s: Incorrect type: %d", __func__, type);
            return NULL;
        }

        for (i = 0; i < count; ++i) {
            if (type == FIFO_SYSLOG)
                p = sys[i];
            else
                p = fw[i];

            if (memcmp(file, p, MIN(strlen(file), strlen(p))) != 0) {
                BTUSB_INFO("%s: Try to open %s", __func__, p);
                f = filp_open(p, flags, mode);
                if (IS_ERR(f)) {
                    f = NULL;
                    continue;
                }
            } else {
                continue;
            }
            BTUSB_INFO("%s: %s opened", __func__, p);
            break;
        }
    }
    return f;
}

static u32 fifo_out_to_file(btmtk_fifo_t *fio, u32 len, u32 off, u8 *end)
{
    struct __kfifo *fifo = &fio->kfifo;
    unsigned int size = fifo->mask + 1;
    unsigned int esize = fifo->esize;
    unsigned int l;
    unsigned char *dump_end = NULL;
    mm_segment_t old_fs;

    off &= fifo->mask;

    if (esize != 1) {
        off *= esize;
        size *= esize;
        len *= esize;
    }
    l = min(len, size - off);

    if (l > 0 && fifo->data != NULL) {
        old_fs = get_fs();
        set_fs(KERNEL_DS);

        if (fio->filp == NULL) {
            BTUSB_WARN("%s: FW Dump started, open file %s", __func__,
                   fio->folder_name);

            fio->filp = fifo_filp_open(fio->folder_name, O_RDWR | O_CREAT, 0644, fio->type);
            if (IS_ERR(fio->filp)) {
                set_fs(old_fs);
                fio->filp = NULL;
                return 0;
            }
        }

        if (fio->filp != NULL) {
            dump_end = strstr(fifo->data, FW_DUMP_END_EVENT);
            fio->filp->f_op->write(fio->filp, fifo->data + off, l,
                    &fio->filp->f_pos);
            fio->filp->f_op->write(fio->filp, fifo->data, len - l,
                    &fio->filp->f_pos);
            fio->stat.tx += len;

            if (dump_end && end) {
                *end = 1;
                BTUSB_INFO("%s: FW Dump finished(success)", __func__);
            }
        }
        set_fs(old_fs);
    }

    /*
     * make sure that the data is copied before
     * incrementing the fifo->out index counter
     */
    smp_wmb();
    return len;
}

static u32 fifo_out_peek_fs(btmtk_fifo_t *fio, u32 len, u8 *end)
{
    unsigned int l;
    struct __kfifo *fifo = &fio->kfifo;

    l = fifo->in - fifo->out;
    if (len > l)
        len = l;

    fifo_out_to_file(fio, len, fifo->out, end);

    return len;
}

static u32 fifo_out_fs(void *fifo_d)
{
    u8 end = 0;
    u32 len = 0;
    btmtk_fifo_t *syslog_p = NULL;
    btmtk_fifo_t *coredump_p = NULL;
    btmtk_fifo_data_t *data_p = (void *)fifo_d;

    if (fifo_d == NULL) {
        BTUSB_ERR("[%s: fifo data is null]", __func__);
        return len;
    }

    if (KFIO_D(fifo_d, FIFO_SYSLOG) != NULL) {
        syslog_p = pFIO(fifo_d, FIFO_SYSLOG);
        len = fifo_out_peek_fs(syslog_p, SYS_LOG_SIZE, &end);
        syslog_p->kfifo.out += len;
    }

    if (KFIO_D(fifo_d, FIFO_COREDUMP) != NULL) {
        coredump_p = pFIO(fifo_d, FIFO_COREDUMP);
        len = fifo_out_peek_fs(coredump_p, FW_DUMP_SIZE, &end);
        coredump_p->kfifo.out += len;
    }

    if (end) {
        set_bit(TSK_SHOULD_STOP, &data_p->tsk_flag);
        BTUSB_INFO("%s: [call btmtk_usb_toggle_rst_pinl]", __func__);
        btmtk_usb_toggle_rst_pin();
    }

    return len;
}

static int btmtk_fifo_thread(void *ptr)
{
    btmtk_fifo_data_t *data_p = (btmtk_fifo_data_t *)ptr;

    if (data_p == NULL) {
        BTUSB_ERR("%s: [FIFO Data is null]", __func__);
        return -1;
    }

    while (!kthread_should_stop() || test_bit(TSK_START, &data_p->tsk_flag)) {
        wait_event_interruptible(data_p->rx_wait_q,
                     fifo_out_accessible(data_p));
        if (test_bit(TSK_SHOULD_STOP, &data_p->tsk_flag))
            break;

        fifo_out_info(data_p);
        fifo_out_fs(data_p);
        fifo_out_info(data_p);
    }

    set_bit(TSK_EXIT, &data_p->tsk_flag);
    BTUSB_INFO("%s: end: down != 0", __func__);

    return 0;
}

void *btmtk_fifo_init(void)
{
    int i;
    btmtk_fifo_data_t *data_p = &g_fifo_data;
    btmtk_fifo_t *fio_p = NULL;

    UNUSED(btmtk_usb_table);    /* clear warning */
    data_p->fifo_l = btmtk_fifo_list;

    while (test_bit(TSK_RUNNING, &data_p->tsk_flag)) {
        if (test_bit(TSK_EXIT, &data_p->tsk_flag))
            break;

        set_bit(TSK_SHOULD_STOP, &data_p->tsk_flag);
        msleep(100);
    }

    data_p->tsk_flag = 0;

    for (i = 0; i < FIFO_END; i++) {
        fio_p = &(data_p->fifo_l[i]);
        if (fio_p->type != i)
            continue;

        if (fio_p->kfifo.data == NULL)
            fifo_alloc(fio_p);

        fifo_init(fio_p);
    }

    init_waitqueue_head(&data_p->rx_wait_q);
    set_bit(TSK_INIT, &data_p->tsk_flag);

    return (void *)(data_p);
}

u32 btmtk_fifo_in(fifo_t type, void *fifo_d, const void *buf,
             unsigned int length)
{
    u32 len = 0;
    u8 hci_pkt = MTK_HCI_EVENT_PKT;
    btmtk_fifo_data_t *data_p = (btmtk_fifo_data_t *)fifo_d;
    btmtk_fifo_t *fio_p = NULL;
    struct __kfifo *fifo = NULL;

    if (fifo_d == NULL) {
        BTUSB_ERR("%s: [fifo data is null]", __func__);
        return len;
    }

    if (!test_bit(TSK_START, &data_p->tsk_flag)) {
        BTUSB_ERR("%s: [fail task not start ]", __func__);
        return 0;
    }

    fio_p = pFIO(fifo_d, type);
    fifo = pKFIO(fifo_d, type);

    if (fifo->data != NULL) {
        if (type == FIFO_SYSLOG) {
            BTUSB_DBG("%s: [type == FIFO_SYSLOG ]", __func__);
            len = __kfifo_in(pKFIO(fifo_d, type), (const void *)&hci_pkt,
                    sizeof(hci_pkt));

            if (len != sizeof(hci_pkt))
                return len;
        }

        len = __kfifo_in(pKFIO(fifo_d, type), buf, length);
        fio_p->stat.rx += len;
    }

    if (len != 0)
        wake_up_interruptible(&data_p->rx_wait_q);

    return len;
}

int btmtk_fifo_start(void *fio_d)
{
    btmtk_fifo_data_t *data_p = (btmtk_fifo_data_t *)fio_d;
    int err;

    if (data_p == NULL) {
        BTUSB_ERR("%s: [fail][fifo data is null]", __func__);
        return -1;
    }

    if (!test_bit(TSK_INIT, &data_p->tsk_flag)) {
        BTUSB_ERR("%s: [fail task not init ]", __func__);
        return -1;
    }

    if (!KFIO_D(data_p, FIFO_SYSLOG) && !KFIO_D(data_p, FIFO_COREDUMP))
        return -1;

    data_p->fifo_tsk = kthread_create(btmtk_fifo_thread, fio_d,
            "btmtk_fifo_thread");
    if (IS_ERR(data_p->fifo_tsk)) {
        BTUSB_ERR("%s: create FIFO thread failed!", __func__);
        err = PTR_ERR(data_p->fifo_tsk);
        data_p->fifo_tsk = NULL;
        return -1;
    }

    set_bit(TSK_START, &data_p->tsk_flag);
    BTUSB_INFO("%s: set TSK_START", __func__);
    wake_up_process(data_p->fifo_tsk);
    BTUSB_INFO("%s: [ok]", __func__);

    return 0;
}

#else /* ENABLE_BT_FIFO_THREAD */
void *btmtk_fifo_init(void)
{
    int *p = 0;

    UNUSED(btmtk_usb_table);    /* clear warning */
    return NULL;
}

int btmtk_fifo_start(void *fio_d)
{
    return 0;
}

u32 btmtk_fifo_in(fifo_t type, void *fifo_d, const void *buf,
             unsigned int length)
{
    return 0;
}

#endif /* ENABLE_BT_FIFO_THREAD */
