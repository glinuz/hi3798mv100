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
#ifndef __BTMTK_USB_DBGFS_H__
#define __BTMTK_USB_DBGFS_H__

#include "btmtk_define.h"

#define DBG_TAG_MAX_LEN         32
#define DBG_FW_MAX_CMD_COUNT    2

typedef struct mtkdbg_data_s {
    /**
     * kref_get(&kref);
     * kref_put(&kref, btusb_delete);
     * kref_init(&kref);
     */
    struct kref         kref;
    struct dentry       *btmtk_root_entery;
    struct dentry       *btmtk_bt_dev_entry;
    struct dentry       *btmtk_trace_entry;
    struct dentry       *btmtk_fifo_entry;
    struct semaphore    wr_lock;
    void        *drv_data;
    u8          *w_buf;
    u8          *hci_buf;
} mtkdbg_data_t;

/**
 * Extern functions
 */
void btmtk_dbgfs_set_drvdata(void *drv_data);
void btmtk_dbgfs_remove_drvdata(void);
int btmtk_dbgfs_init(void);
void btmtk_dbgfs_exit(void);

#endif /* __BTMTK_USB_DBGFS_H__ */
