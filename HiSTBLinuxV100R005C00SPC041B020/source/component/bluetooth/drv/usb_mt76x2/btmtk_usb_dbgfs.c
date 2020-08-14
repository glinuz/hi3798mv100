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
#if defined(CONFIG_DEBUG_FS) && (CONFIG_DEBUG_FS == 1)

#include <linux/debugfs.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/kref.h>
#include <linux/semaphore.h>
#include <linux/compiler.h>

#include "btmtk_usb_dbgfs.h"
#include "btmtk_usb_main.h"

/**
 * Directory define.,
 * If this functionality enabled you should find specific file in
 *  /sys/kernel/debug/BTDBG_ROOT/
 */
#define BTDBG_ROOT  "mtkbt"
#define BTDBG_DEV   "bt_dev"
#define BTDBG_TRACE "log_lvl"

#define FS_BUFFER_SIZE 2048
#define MTKBT_MAX_HCI_FRAME_SIZE 1028   /* (BT_MAX_ACL_SIZE + 4) */

typedef struct BTDbg_Func_s {
    const char *name;
    int (*fp)(const char *value, void *dbg_filp);
    const char *doc;
    const char *help;
} BTDbg_Func_t;

typedef struct BTDbg_FW_Func_s {
    const char *major;
    const char *minor[DBG_FW_MAX_CMD_COUNT];
    const char *cmd[DBG_FW_MAX_CMD_COUNT];
    const char *doc[DBG_FW_MAX_CMD_COUNT];
} BTDbg_FW_Func_t;

/**
 * Global variable
 */
mtkdbg_data_t *g_mtkbt_dbg = NULL;
u8 btmtk_log_lvl = BTMTK_LOG_LEVEL_DEFAULT;

/**
 * Function prototype
 */
static int btmtk_dbgfs_data_alloc(void *dbg_filp);
static void btmtk_dbgfs_data_free(void *dbg_filp);
static int btmtk_dbgfs_fw_func_main(const char *value, void *dbg_filp);
static int btmtk_dbgfs_tx_frame(void *drv_data, const unsigned char *buffer, const unsigned int length);

static int strtol_access_ok(char ch)
{
    if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F')
        || (ch >= 'a' && ch <= 'f'))
        return 0;

    BTUSB_ERR("wrong char_%c_", ch);

    return -1;
}

static int str_to_u8(const char *ptr)
{
    u8 ret = 0;
    u8 temp_str[3] = { 0 };
    long res = 0;

    temp_str[0] = *ptr;
    temp_str[1] = *(ptr + 1);

    ret = (u8) (kstrtol((char *)temp_str, 16, &res) & 0xff);

    return res;
}

static int str_to_bytes(u8 *cmd_buf, const char *ptr)
{
    int i = 0, j = 0, len = 0;

    len = strlen(ptr);

    while (i < len) {
        if (ptr[i] == ' ' || ptr[i] == '\t' || ptr[i] == '\r'
            || ptr[i] == '\n') {
            i++;
            continue;
        }

        if ((ptr[i] == '0' && ptr[i + 1] == 'x')
            || (ptr[0] == '0' && ptr[i + 1] == 'X')) {
            i += 2;
            continue;
        }

        if (strtol_access_ok(ptr[i]) != 0)
            break;

        cmd_buf[j++] = str_to_u8(&ptr[i]);
        i += 2;
    }

    return j;
}

static int btmtk_dbgfs_tx_frame(void *drv_data, const unsigned char *buffer, const unsigned int length)
{
    int retval = -1;

    BTUSB_DBG("%s: hci cmd", __func__);

    if (unlikely(buffer == NULL || length <= 0)) {
        BTUSB_ERR("%s: [hci_buf = %p][length = 0x%x]", __func__, buffer, length);
        return -EFAULT;
    }

    if (drv_data != NULL)
        BTUSB_DBG("%s: [drv_data=%p]", __func__, drv_data);


    switch (buffer[0]) {
    case MTK_HCI_COMMAND_PKT:
        retval = btmtk_usb_meta_send_data(buffer, length);
        break;

    case MTK_HCI_ACLDATA_PKT:
    case MTK_HCI_SCODATA_PKT:
        retval = btmtk_usb_send_data(buffer, length);
        break;

    default:
        BTUSB_ERR("%s: [INVALID PACKAGE TYPE = %d]", __func__, buffer[0]);
        return -EILSEQ;
    }

    BTUSB_DBG("%s: retval = %d", __func__, retval);

    return retval;
}

static int btmtk_dbgfs_tx_hci_data(const char *value, void *dbg_filp)
{
    int length = 0, ret = 0;
    u8 *hci_cmd = NULL;
    mtkdbg_data_t *mtkdbg_filp = (mtkdbg_data_t *)dbg_filp;

    if (!mtkdbg_filp->hci_buf) {

        BTUSB_ERR("%s: hci buff is null", __func__);
        btmtk_dbgfs_data_alloc(mtkdbg_filp);

        if (!mtkdbg_filp->hci_buf)
            return -1;
    }

    hci_cmd = mtkdbg_filp->hci_buf;

    length = str_to_bytes(hci_cmd, value);

    BTUSB_DBG_RAW(hci_cmd, length, "%s: dump cmd:", __func__);

    if (mtkdbg_filp->drv_data)
        BTUSB_DBG("%s: [mtkdbg_filp->drv_data = %p]", __func__,
              mtkdbg_filp->drv_data);

    ret = btmtk_dbgfs_tx_frame(mtkdbg_filp->drv_data, hci_cmd, length);

    BTUSB_DBG("%s: ret = %d", __func__, ret);

    return length;
}

static BTDbg_Func_t dbgfs_func_list[] = {
    {"HCI", btmtk_dbgfs_tx_hci_data, "tx hci data with type only",
        "01 6F FC 05 01 02 01 00 08"},
    {"FW", btmtk_dbgfs_fw_func_main, "set fw to do something", "DUMP"},
    {"", NULL, NULL, NULL}
};

static BTDbg_FW_Func_t dbgfs_fw_func_list[] = {
    { "DUMP", {"1", "2"},
        {"01 6F FC 05 01 02 01 00 08", "02 6F FC 05 01 02 01 00 08"},
        {"tx fw assert cmd from endpoint 0", "tx fw assert cmd from endpoint 2"} },
    { NULL, {NULL, NULL}, {NULL, NULL}, {NULL, NULL} }
};

static void btmtk_dbgfs_func_usage(BTDbg_Func_t *funclp)
{
    int i;

    BTUSB_DBG("Usage:\n\t <command>= vaule01 value02 ... ");
    for (i = 0; funclp[i].fp; i++)
            BTUSB_DBG("[%s]example:\n \t \t <%s>=<%s>", funclp[i].doc, funclp[i].name, funclp[i].help);

    BTUSB_DBG("<command> should be less than DBG_TAG_MAX_LEN(0x%x) bytes",
          DBG_TAG_MAX_LEN);
    BTUSB_DBG("<value> should be less than FS_BUFFER_SIZE(0x%x) bytes",
          FS_BUFFER_SIZE);
}

static void btmtk_dbgfs_fw_func_usage(BTDbg_FW_Func_t *funclp)
{
    int i, j;

    BTUSB_DBG("Usage:\n\t <FW>= vaule01 value02 ...");
    for (i = 0; funclp[i].major; i++) {
        for (j = 0; j < DBG_FW_MAX_CMD_COUNT; j++) {
            BTUSB_DBG("%s: example:\n \t \t \t \t <FW>=<%s><%s>",
                  __func__, funclp[i].major,
                  funclp[i].minor[j]);
            BTUSB_DBG("%s: means:[%s]", __func__,
                  funclp[i].doc[j]);
            BTUSB_DBG("%s: tx fw cmd [%s]", __func__,
                  funclp[i].cmd[j]);
        }
    }
}

static int btmtk_dbgfs_fw_func_main(const char *value, void *dbg_filp)
{
    int i;
    u32 index = 0;
    const char *s2 = value;

    BTUSB_DBG("%s: [FW=%s]", __func__, value);

    while (1) {
        if (dbgfs_fw_func_list[index].major == NULL || value == NULL)
            goto fw_exit;

        s2 = strstr(value, dbgfs_fw_func_list[index].major);

        if (s2 != NULL) {
            for (i = 0; i < DBG_FW_MAX_CMD_COUNT; i++) {
                if (dbgfs_fw_func_list[index].minor[i] ==
                    NULL)
                    continue;

                if (strstr(s2, dbgfs_fw_func_list[index].minor[i]) != NULL)
                    break;
            }

            if (i < DBG_FW_MAX_CMD_COUNT)
                break;
        }
        index++;
    }

    BTUSB_DBG("%s: match [cmd =%s]", __func__,
          dbgfs_fw_func_list[index].cmd[i]);
    btmtk_dbgfs_tx_hci_data(dbgfs_fw_func_list[index].cmd[i], dbg_filp);

    return 0;

fw_exit:
    BTUSB_DBG("%s: [FW=Wrong cmd!] Fail", __func__);
    btmtk_dbgfs_fw_func_usage(dbgfs_fw_func_list);
    return -1;
}

static int btmtk_dbgfs_get_option(char *option)
{
    u32 index = 0;

    while (1) {
        if (strcmp(option, dbgfs_func_list[index].name) == 0)
            return index;

        if (dbgfs_func_list[index].fp == NULL)
            return -1;

        index++;
    }
}

int BTDbg_Func_Entry(void *dbg_filp, u8 *tag, const char *value)
{
    int index;
    int ret = 0;

    if (tag == NULL)
        goto exit;

    index = btmtk_dbgfs_get_option(tag);

    if (index == -1)
        goto exit;

    ret = dbgfs_func_list[index].fp(value, dbg_filp);
    if (ret < 0) {
        BTUSB_ERR("[tag = %s][value = %s], fail", tag, value);
        return -1;
    }

    return 0;
exit:
    btmtk_dbgfs_func_usage(dbgfs_func_list);
    return -1;
}

int btmtk_dbgfs_func_main(const char *value, void *dbg_filp)
{
    static int count_f;
    int val_len = 0, i;
    u8 Tag_param[DBG_TAG_MAX_LEN] = { 0 };
    const char *val_param = NULL;

    if (unlikely(dbg_filp == NULL || value == NULL)) {
        BTUSB_ERR("%s: [flp =0x%p][value = 0x%p] fail", __func__,
              dbg_filp, value);
        return -1;
    }

    count_f++;

    BTUSB_DBG("%s: in count = %d", __func__, count_f);

    val_len = strlen(value);

    val_len = val_len < DBG_TAG_MAX_LEN ? val_len : DBG_TAG_MAX_LEN;

    for (i = 0; i < val_len; i++) {
        if (value[i] != '=')
            Tag_param[i] = value[i];
        else
            break;
    }

    if (i < val_len) {
        val_param = &value[i + 1];
        BTDbg_Func_Entry(dbg_filp, Tag_param, val_param);

    } else {
        btmtk_dbgfs_func_usage(dbgfs_func_list);
    }

    BTUSB_DBG("%s: out = %d", __func__, count_f);
    return 0;
}

static int BTDbg_dev_open(struct inode *inode, struct file *file)
{
    mtkdbg_data_t *mtkdbg_filp = NULL;

    BTUSB_DBG("%s: %s/%s open", __func__, BTDBG_ROOT, BTDBG_DEV);

    file->private_data = inode->i_private;

    mtkdbg_filp = (mtkdbg_data_t *)file->private_data;

    if (mtkdbg_filp)
        sema_init(&mtkdbg_filp->wr_lock, 1);

    return 0;
}

static int BTDbg_dev_close(struct inode *inode, struct file *filp)
{
    BTUSB_DBG("[%s][%s]close", BTDBG_ROOT, BTDBG_DEV);

    filp->private_data = NULL;

    return 0;
}

ssize_t BTDbg_dev_write(struct file *filp, const char __user *buf,
            size_t count, loff_t *f_pos)
{
    mtkdbg_data_t *mtkdbg_filp = (mtkdbg_data_t *)filp->private_data;

    if (mtkdbg_filp == NULL) {

        BTUSB_ERR("%s: no mtkbt dgb data", __func__);
        return -ENODEV;
    }

    down(&mtkdbg_filp->wr_lock);

    if (mtkdbg_filp->w_buf == NULL) {
        BTUSB_ERR("%s: no w_buf", __func__);

        btmtk_dbgfs_data_alloc((void *)mtkdbg_filp);

        if (mtkdbg_filp->w_buf == NULL)
            goto wt_exit;
    }

    memset(mtkdbg_filp->w_buf, 0, FS_BUFFER_SIZE);

    if (unlikely(count > FS_BUFFER_SIZE)) {
        BTUSB_WARN("%s: w count(%zu) > FS_BUFFER_SIZE(%d)", __func__, count, FS_BUFFER_SIZE);
        count = FS_BUFFER_SIZE - 1;
    }

    if (copy_from_user(mtkdbg_filp->w_buf, buf, count) != 0) {
        BTUSB_ERR("[%s][%s]copy_from_user fail", BTDBG_ROOT,
              BTDBG_DEV);
        goto wt_exit;
    }

    BTUSB_DBG("%s: w_buf: %s", __func__, mtkdbg_filp->w_buf);

#if 1
    btmtk_dbgfs_func_main(mtkdbg_filp->w_buf, mtkdbg_filp);
#else

    io_len = strlen(mtkdbg_filp->w_buf);

    if (io_len != count)
        BTUSB_WARN("WARN:io_len != count");

    target_len = io_len < 512 ? io_len : 512;

    for (i = 0; i < 512; i++) {
        if (mtkdbg_filp->w_buf[i] != '=')
            parm_tag[i] = mtkdbg_filp->w_buf[i];
        else
            break;
    }
    if (i < 512) {
        pValue = &mtkdbg_filp->w_buf[i + 1];
        BTDbg_Func_Entry(parm_tag, pValue);
    } else {
        BTUSB_WARN("tag len should be less than 512");
        goto wt_exit;
    }
#endif
wt_exit:
    up(&mtkdbg_filp->wr_lock);

    return count;
}

const struct file_operations BTDbg_dev_fops = {
    .open = BTDbg_dev_open,
    .release = BTDbg_dev_close,
    .write = BTDbg_dev_write
};

static int btmtk_dbgfs_data_alloc(void *dbg_filp)
{
    mtkdbg_data_t *mtkdbg_filp = (mtkdbg_data_t *)dbg_filp;

    if (!mtkdbg_filp) {
        BTUSB_ERR("%s: faill to alloc data for dbg file is null",
              __func__);
        return -1;
    }

    if (!mtkdbg_filp->btmtk_root_entery) {

        BTUSB_ERR("%s: btmtk_root_entery is null", __func__);
        return -1;
    }

    if (!mtkdbg_filp->btmtk_bt_dev_entry)
        mtkdbg_filp->btmtk_bt_dev_entry =
            debugfs_create_file(BTDBG_DEV, 0644,
                    mtkdbg_filp->btmtk_root_entery,
                    (void *)mtkdbg_filp, &BTDbg_dev_fops);

    if (!mtkdbg_filp->btmtk_trace_entry)
        mtkdbg_filp->btmtk_trace_entry =
            debugfs_create_u8(BTDBG_TRACE, 0644,
                      mtkdbg_filp->btmtk_root_entery,
                      &btmtk_log_lvl);

    if (mtkdbg_filp->btmtk_bt_dev_entry == NULL
        || mtkdbg_filp->btmtk_trace_entry == NULL) {

        BTUSB_WARN("[%s = %p], [%s = %p]", BTDBG_DEV,
               mtkdbg_filp->btmtk_bt_dev_entry, BTDBG_TRACE,
               mtkdbg_filp->btmtk_trace_entry);

    }
    if (!mtkdbg_filp->w_buf)
        mtkdbg_filp->w_buf = kmalloc(FS_BUFFER_SIZE, GFP_KERNEL);

    if (!mtkdbg_filp->hci_buf)
        mtkdbg_filp->hci_buf =
            kmalloc(MTKBT_MAX_HCI_FRAME_SIZE, GFP_KERNEL);

    if (mtkdbg_filp->w_buf == NULL || mtkdbg_filp->hci_buf == NULL) {
        BTUSB_WARN("%s: [w_buf = %p][hci_buf = %p]", __func__,
                mtkdbg_filp->w_buf, mtkdbg_filp->hci_buf);
        return -1;
    }

    return 0;
}

static void btmtk_dbgfs_data_free(void *dbg_filp)
{
    mtkdbg_data_t *mtkdbg_filp = (mtkdbg_data_t *)dbg_filp;

    if (!mtkdbg_filp) {
        BTUSB_ERR("%s: faill to free data for dbg file is null",
              __func__);
        return;
    }

    if (!mtkdbg_filp->btmtk_root_entery) {
        BTUSB_ERR("%s: fail to free for root entry is null", __func__);
        return;
    }

    debugfs_remove(mtkdbg_filp->btmtk_bt_dev_entry);
    mtkdbg_filp->btmtk_bt_dev_entry = NULL;

    debugfs_remove(mtkdbg_filp->btmtk_trace_entry);
    mtkdbg_filp->btmtk_trace_entry = NULL;

    kfree(mtkdbg_filp->w_buf);
    mtkdbg_filp->w_buf = NULL;

    kfree(mtkdbg_filp->hci_buf);
    mtkdbg_filp->hci_buf = NULL;
}

void btmtk_dbgfs_set_drvdata(void *drv_data)
{
    mtkdbg_data_t *mtkdbg_filp = g_mtkbt_dbg;

    BTUSB_DBG("%s: enter", __func__);

    if (unlikely(drv_data == NULL)) {

        BTUSB_ERR("%s: fail for drv_data is null", __func__);
        return;
    }

    if (unlikely(mtkdbg_filp == NULL)) {
        BTUSB_ERR("%s: no mtkbt dgb data", __func__);
        return;
    }

    mtkdbg_filp->drv_data = drv_data;
}

void btmtk_dbgfs_remove_drvdata(void)
{
    mtkdbg_data_t *mtkdbg_filp = g_mtkbt_dbg;

    BTUSB_DBG("%s: enter", __func__);

    if (unlikely(mtkdbg_filp == NULL)) {
        BTUSB_ERR("%s: no mtkbt dgb data", __func__);
        return;
    }

    mtkdbg_filp->drv_data = NULL;
}

int btmtk_dbgfs_init(void)
{
    mtkdbg_data_t *mtkdbg_filp;

    UNUSED(btmtk_usb_table);    /* clear warning */
    BTUSB_DBG("%s", __func__);

    if (g_mtkbt_dbg != NULL)
            BTUSB_WARN("%s: mtkbt dbg is not null, memory leak !", __func__);

    mtkdbg_filp = kzalloc(sizeof(mtkdbg_data_t), GFP_KERNEL);

    if (!mtkdbg_filp) {
            BTUSB_ERR("%s: alloc mtk dbg file fail!", __func__);
            return -1;
    }

    mtkdbg_filp->btmtk_root_entery = debugfs_create_dir(BTDBG_ROOT, NULL);

    if (mtkdbg_filp->btmtk_root_entery == NULL) {
        BTUSB_ERR("%s: fail to create dbg root dir !", __func__);
        return -1;
    }

    sema_init(&mtkdbg_filp->wr_lock, 1);

    g_mtkbt_dbg = mtkdbg_filp;

    btmtk_dbgfs_data_alloc((void *)g_mtkbt_dbg);

    BTUSB_DBG("%s: success", __func__);
    return 0;
}

void btmtk_dbgfs_exit(void)
{

    if (!g_mtkbt_dbg) {
        BTUSB_ERR("%s: faill to exit dbg file is null", __func__);
        return;
    }

    btmtk_dbgfs_data_free((void *)g_mtkbt_dbg);

    debugfs_remove(g_mtkbt_dbg->btmtk_root_entery);    /*(bt_dev_entry); */
    g_mtkbt_dbg->btmtk_root_entery = NULL;

    kfree(g_mtkbt_dbg);
    g_mtkbt_dbg = NULL;

}

#else /* CONFIG_DEBUG_FS */
void btmtk_dbgfs_set_drvdata(void *drv_data)
{
    return;
}

void btmtk_dbgfs_remove_drvdata(void)
{
    return;
}

int btmtk_dbgfs_init(void)
{
    BTUSB_INFO("%s: debugfs is not enable now", __func__);
    return 0;
}

void btmtk_dbgfs_exit(void)
{
    return;
}

#endif /* CONFIG_DEBUG_FS */
