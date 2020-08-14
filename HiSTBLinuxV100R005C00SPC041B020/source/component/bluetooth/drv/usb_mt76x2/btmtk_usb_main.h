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
#ifndef __BTMTK_USB_MAIN_H__
#define __BTMTK_USB_MAIN_H__

#include "btmtk_define.h"

enum {
    BTMTK_USB_STATE_UNKNOWN,
    BTMTK_USB_STATE_INIT,
    BTMTK_USB_STATE_DISCONNECT,
    BTMTK_USB_STATE_PROBE,
    BTMTK_USB_STATE_WORKING,
    BTMTK_USB_STATE_EARLY_SUSPEND,
    BTMTK_USB_STATE_SUSPEND,
    BTMTK_USB_STATE_RESUME,
    BTMTK_USB_STATE_LATE_RESUME,
    BTMTK_USB_STATE_FW_DUMP,
    BTMTK_USB_STATE_SUSPEND_DISCONNECT,
    BTMTK_USB_STATE_SUSPEND_PROBE,
    BTMTK_USB_STATE_RESUME_DISCONNECT,
    BTMTK_USB_STATE_RESUME_PROBE,
    BTMTK_USB_STATE_RESUME_FW_DUMP,
};

typedef struct _OSAL_UNSLEEPABLE_LOCK_ {
    spinlock_t      lock;
    unsigned long   flag;
} OSAL_UNSLEEPABLE_LOCK, *P_OSAL_UNSLEEPABLE_LOCK;

typedef struct {
    OSAL_UNSLEEPABLE_LOCK   spin_lock;
    unsigned char           buffer[META_BUFFER_SIZE];   /* MTKSTP_BUFFER_SIZE:1024 */
    unsigned int            read_p;                     /* indicate the current read index */
    unsigned int            write_p;                    /* indicate the current write index */
} ring_buffer_struct;

struct btmtk_usb_data {
    struct usb_device       *udev;    /* store the usb device informaiton */
    struct usb_interface    *intf;    /* current interface */
    struct usb_interface    *isoc;    /* isochronous interface */
    struct work_struct      waker;
    struct semaphore        wr_mtx;
    struct semaphore        rd_mtx;
    struct usb_anchor       tx_anchor;
    struct usb_anchor       intr_anchor;
    struct usb_anchor       bulk_anchor;
    int                     meta_tx;
    spinlock_t              txlock;

    /* USB endpoint */
    struct usb_endpoint_descriptor  *intr_ep;
    struct usb_endpoint_descriptor  *bulk_tx_ep;
    struct usb_endpoint_descriptor  *bulk_rx_ep;

    int                 suspend_count;

    /* request for different io operation */
    unsigned char       w_request;
    unsigned char       r_request;

    /* io buffer for usb control transfer */
    unsigned char       *io_buf;

    unsigned char       *rom_patch;
    unsigned char       *rom_patch_bin_file_name;
    unsigned char       *rom_patch_image;
    unsigned int        rom_patch_image_len;
    unsigned int        chip_id;
    unsigned int        rom_patch_offset;
    unsigned int        rom_patch_len;

    unsigned char       *i_buf;
    unsigned char       *o_buf;
    ring_buffer_struct  *metabuffer;

    unsigned char       interrupt_urb_submitted;
    unsigned char       bulk_urb_submitted;

    void                *bt_fifo;
#if SUPPORT_MT7668
    unsigned char       is_mt7668_dongle_state;
#endif
};

/**
 * Inline functions
 */
#if SUPPORT_MT7662
static inline int is_mt7662(struct btmtk_usb_data *data)
{
    return (((data->chip_id & 0xffff0000) == 0x76620000) || ((data->chip_id & 0xffff0000) == 0x76320000));
}

static inline int is_mt7662T(struct btmtk_usb_data *data)
{
    return (((data->chip_id & 0xffffffff) == 0x76620100) || ((data->chip_id & 0xffffffff) == 0x76320100));
}
#endif

#if SUPPORT_MT7668
static inline int is_mt7668(struct btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffff) == 0x7668);
}

enum {
    BTMTK_USB_7668_DONGLE_STATE_UNKNOWN,
    BTMTK_USB_7668_DONGLE_STATE_POWERING_ON,
    BTMTK_USB_7668_DONGLE_STATE_POWER_ON,
    BTMTK_USB_7668_DONGLE_STATE_WOBLE,
    BTMTK_USB_7668_DONGLE_STATE_POWERING_OFF,
    BTMTK_USB_7668_DONGLE_STATE_POWER_OFF,
    BTMTK_USB_7668_DONGLE_STATE_ERROR
};
#endif

/**
 * Extern functions
 */
int btmtk_usb_send_data(const unsigned char *buffer, const unsigned int length);
int btmtk_usb_meta_send_data(const unsigned char *buffer,
        const unsigned int length);
void btmtk_usb_toggle_rst_pin(void);

#endif /* __BTMTK_USB_MAIN_H__ */
