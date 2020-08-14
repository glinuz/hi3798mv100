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
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/completion.h>
#include <linux/usb.h>
#include <linux/version.h>
#include <linux/firmware.h>
#include <linux/skbuff.h>
#include <linux/cdev.h>
#include <linux/spinlock.h>
#include <linux/kallsyms.h>
#include <linux/fs.h>
#include <linux/kthread.h>
#include <asm/uaccess.h>
#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>

#include "btmtk_usb_main.h"
#include "btmtk_usb_dbgfs.h"
#include "btmtk_usb_fifo.h"

/*============================================================================*/
/* Local Configuration */
/*============================================================================*/
#define VERSION "6.0.16081101"

/*============================================================================*/
/* Function Prototype */
/*============================================================================*/
#define ___________________________________________________Function_Prototype
static int btmtk_usb_standby(void);
static void btmtk_usb_cap_init(void);
static int btmtk_usb_BT_init(void);
static void btmtk_usb_BT_exit(void);
static int btmtk_usb_send_assert_cmd(void);
static int btmtk_usb_send_assert_cmd_ctrl(void);
static int btmtk_usb_send_assert_cmd_bulk(void);
static int btmtk_usb_submit_intr_urb(void);
static int btmtk_usb_submit_bulk_in_urb(void);
static int btmtk_usb_send_hci_reset_cmd(void);
static int btmtk_usb_send_woble_suspend_cmd(void);
static void btmtk_usb_load_rom_patch_complete(struct urb *urb);
static void btmtk_usb_bulk_in_complete(struct urb *urb);

static void btmtk_usb_hci_snoop_print_to_log(void);
static int btmtk_usb_get_rom_patch_result(void);
static void btmtk_usb_tx_complete_meta(struct urb *urb);
static int btmtk_usb_load_rom_patch(void);
static int btmtk_usb_send_wmt_reset_cmd(void);
static void btmtk_usb_early_suspend(void);
static void btmtk_usb_late_resume(void);
static void btmtk_usb_intr_complete(struct urb *urb);
static int btmtk_usb_send_wmt_cmd(const u8 *cmd, const int cmd_len, const u8 *event,
        const int event_len, u32 delay);
static int btmtk_usb_send_hci_cmd(const u8 *cmd, int cmd_len, const u8 *event, int event_len);
/** file_operations: stpbt */
static int btmtk_usb_fops_open(struct inode *inode, struct file *file);
static int btmtk_usb_fops_close(struct inode *inode, struct file *file);
static ssize_t btmtk_usb_fops_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t btmtk_usb_fops_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos);
static unsigned int btmtk_usb_fops_poll(struct file *file, poll_table *wait);
static long btmtk_usb_fops_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
#if FASYNC_BT_RECOVER
static int btmtk_usb_fops_fasync(int fd, struct file *file, int on);
#endif /* FASYNC_BT_RECOVER */

#if SUPPORT_MT7662
static int btmtk_usb_load_rom_patch_7662(void);
static int btmtk_usb_io_read32_7662(u32 reg, u32 *val);
static int btmtk_usb_io_write32_7662(u32 reg, u32 val);
static int btmtk_usb_switch_iobase_7662(int base);
static int btmtk_usb_chk_crc_7662(u32 checksum_len);
static u16 btmtk_usb_get_crc_7662(void);
static u16 btmtk_usb_checksum16_7662(u8 *pData, int len);
static int btmtk_usb_send_hci_set_tx_power_cmd_7662(void);
static int btmtk_usb_send_hci_radio_on_cmd_7662(void);
static int btmtk_usb_send_hci_radio_off_cmd_7662(void);
static int btmtk_usb_send_hci_set_ce_cmd_7662(void);
static int btmtk_usb_send_hci_check_rom_patch_result_cmd_7662(void);
static int btmtk_usb_send_hci_low_power_cmd_7662(BOOL enable);
static void btmtk_usb_send_dummy_bulk_out_packet_7662(void);
#endif /* SUPPORT_MT7662 */

#if SUPPORT_MT7668
static int btmtk_usb_load_rom_patch_7668(void);
static int btmtk_usb_io_read32_7668(u32 reg, u32 *val);
static int btmtk_usb_io_read32_7668(u32 reg, u32 *val);
static int btmtk_usb_send_hci_tci_set_sleep_cmd_7668(void);
static int btmtk_usb_send_wmt_power_on_cmd_7668(void);
static int btmtk_usb_send_wmt_power_off_cmd_7668(void);
#endif /* SUPPORT_MT7668 */

/*============================================================================*/
/* Global Variable */
/*============================================================================*/
#define ___________________________________________________Global_Variables
/* stpbt character device for meta */
#ifdef FIXED_STPBT_MAJOR_DEV_ID
static int BT_major = FIXED_STPBT_MAJOR_DEV_ID;
#else
static int BT_major;
#endif
static struct class *pBTClass;
static struct device *pBTDev;
static struct cdev BT_cdev;
static struct usb_driver btmtk_usb_driver;
static wait_queue_head_t inq;
static struct btmtk_usb_data *g_data;
static int probe_counter;
static int need_reset_stack;
static int leftHciEventSize;
static int is_fops_close_ongoing;
static int leftACLSize;
static int btmtk_usb_state = BTMTK_USB_STATE_UNKNOWN;
static DECLARE_WAIT_QUEUE_HEAD(BT_wq);
static DEFINE_MUTEX(btmtk_usb_state_mutex);
#if FASYNC_BT_RECOVER
static struct fasync_struct *fasync;
#else
static u8 reset_event[] = { 0x04, 0xFF, 0x04, 0x00, 0x01, 0xFF, 0xFF };
#endif

typedef void (*register_early_suspend) (void (*f) (void));
typedef void (*register_late_resume) (void (*f) (void));
register_early_suspend register_early_suspend_func;
register_late_resume register_late_resume_func;

/* Hci Snoop */
static u8 hci_cmd_snoop_buf[HCI_SNOOP_ENTRY_NUM][HCI_SNOOP_BUF_SIZE];
static u8 hci_cmd_snoop_len[HCI_SNOOP_ENTRY_NUM] = { 0 };

static unsigned int hci_cmd_snoop_timestamp[HCI_SNOOP_ENTRY_NUM];
static u8 hci_event_snoop_buf[HCI_SNOOP_ENTRY_NUM][HCI_SNOOP_BUF_SIZE];
static u8 hci_event_snoop_len[HCI_SNOOP_ENTRY_NUM] = { 0 };

static unsigned int hci_event_snoop_timestamp[HCI_SNOOP_ENTRY_NUM];
static u8 hci_acl_snoop_buf[HCI_SNOOP_ENTRY_NUM][HCI_SNOOP_BUF_SIZE];
static u8 hci_acl_snoop_len[HCI_SNOOP_ENTRY_NUM] = { 0 };

static unsigned int hci_acl_snoop_timestamp[HCI_SNOOP_ENTRY_NUM];
static int hci_cmd_snoop_index = HCI_SNOOP_ENTRY_NUM - 1;
static int hci_event_snoop_index = HCI_SNOOP_ENTRY_NUM - 1;
static int hci_acl_snoop_index = HCI_SNOOP_ENTRY_NUM - 1;

const struct file_operations BT_fops = {
    .open = btmtk_usb_fops_open,
    .release = btmtk_usb_fops_close,
    .read = btmtk_usb_fops_read,
    .write = btmtk_usb_fops_write,
    .poll = btmtk_usb_fops_poll,
    .unlocked_ioctl = btmtk_usb_fops_unlocked_ioctl,
#if FASYNC_BT_RECOVER
    .fasync = btmtk_usb_fops_fasync,
#endif /* FASYNC_BT_RECOVER */
};

/*============================================================================*/
/* Internal Functions */
/*============================================================================*/
#define ___________________________________________________Internal_Functions
static void *btmtk_usb_osal_memcpy(void *dst, const void *src, u32 len)
{
    return memcpy(dst, src, len);
}

static int btmtk_usb_init_memory(void)
{
    if (g_data) {
        g_data->chip_id = 0;
        g_data->suspend_count = 0;
    }

    if (g_data->metabuffer)
        memset(g_data->metabuffer->buffer, 0, META_BUFFER_SIZE);

    if (g_data->io_buf)
        memset(g_data->io_buf, 0, USB_IO_BUF_SIZE);

    if (g_data->rom_patch_bin_file_name)
        memset(g_data->rom_patch_bin_file_name, 0, 32);

    if (g_data->i_buf)
        memset(g_data->i_buf, 0, BUFFER_SIZE);

    if (g_data->o_buf)
        memset(g_data->o_buf, 0, BUFFER_SIZE);
    BTUSB_INFO("%s: Success", __func__);
    return 1;
}

static int btmtk_usb_allocate_memory(void)
{
    if (NULL == g_data) {
        g_data = kzalloc(sizeof(*g_data), GFP_KERNEL);
        if (!g_data) {
            BTUSB_ERR("%s: alloc memory fail (g_data)", __func__);
            return -1;
        }
    }

    if (NULL == g_data->metabuffer) {
        g_data->metabuffer =
            kmalloc(sizeof(ring_buffer_struct), GFP_KERNEL);
        if (!g_data->metabuffer) {
            BTUSB_ERR("%s: alloc memory fail (g_data->metabuffer)",
                  __func__);
            return -1;
        }
        memset(g_data->metabuffer->buffer, 0, META_BUFFER_SIZE);
    }

    if (NULL == g_data->io_buf) {
        g_data->io_buf = kmalloc(USB_IO_BUF_SIZE, GFP_KERNEL);
        if (!g_data->io_buf) {
            BTUSB_ERR("%s: alloc memory fail (g_data->io_buf)",
                  __func__);
            return -1;
        }
        memset(g_data->io_buf, 0, USB_IO_BUF_SIZE);
    }

    if (NULL == g_data->rom_patch_bin_file_name) {
        g_data->rom_patch_bin_file_name = kmalloc(32, GFP_KERNEL);
        if (!g_data->rom_patch_bin_file_name) {
            BTUSB_ERR
                ("%s: alloc memory fail (g_data->rom_patch_bin_file_name)",
                 __func__);
            return -1;
        }
        memset(g_data->rom_patch_bin_file_name, 0, 32);
    }

    if (NULL == g_data->i_buf) {
        g_data->i_buf = kmalloc(BUFFER_SIZE, GFP_KERNEL);
        if (!g_data->i_buf) {
            BTUSB_ERR("%s: alloc memory fail (g_data->)", __func__);
            return -1;
        }
        memset(g_data->i_buf, 0, BUFFER_SIZE);
    }

    if (NULL == g_data->o_buf) {
        g_data->o_buf = kmalloc(BUFFER_SIZE, GFP_KERNEL);
        if (!g_data->o_buf) {
            BTUSB_ERR("%s: alloc memory fail (g_data->o_buf)",
                  __func__);
            return -1;
        }
        memset(g_data->o_buf, 0, BUFFER_SIZE);
    }
    BTUSB_INFO("%s: Success", __func__);
    return 1;
}

static void btmtk_usb_free_memory(void)
{
    kfree(g_data->metabuffer);
    g_data->metabuffer = NULL;

    kfree(g_data->i_buf);
    g_data->i_buf = NULL;

    kfree(g_data->o_buf);
    g_data->o_buf = NULL;

    kfree(g_data->rom_patch_bin_file_name);
    g_data->rom_patch_bin_file_name = NULL;

    kfree(g_data->io_buf);
    g_data->io_buf = NULL;

    kfree(g_data->rom_patch_image);
    g_data->rom_patch_image = NULL;

    kfree(g_data);
    g_data = NULL;

    BTUSB_INFO("%s: Success", __func__);
}

static int btmtk_usb_get_state(void)
{
    return btmtk_usb_state;
}

static void btmtk_usb_set_state(int new_state)
{
    BTUSB_INFO("%s: %s(%d) -> %s(%d)", __func__,
           btmtk_usb_state == BTMTK_USB_STATE_INIT ? "INIT" :
           btmtk_usb_state == BTMTK_USB_STATE_DISCONNECT ? "DISCONNECT" :
           btmtk_usb_state == BTMTK_USB_STATE_PROBE ? "PROBE" :
           btmtk_usb_state == BTMTK_USB_STATE_WORKING ? "WORKING" :
           btmtk_usb_state == BTMTK_USB_STATE_EARLY_SUSPEND ? "EARLY_SUSPEND" :
           btmtk_usb_state == BTMTK_USB_STATE_SUSPEND ? "SUSPEND" :
           btmtk_usb_state == BTMTK_USB_STATE_RESUME ? "RESUME" :
           btmtk_usb_state == BTMTK_USB_STATE_LATE_RESUME ? "LATE_RESUME" :
           btmtk_usb_state == BTMTK_USB_STATE_FW_DUMP ? "FW_DUMP" :
           btmtk_usb_state == BTMTK_USB_STATE_SUSPEND_DISCONNECT ? "SUSPEND_DISCONNECT" :
           btmtk_usb_state == BTMTK_USB_STATE_SUSPEND_PROBE ? "SUSPEND_PROBE" :
           btmtk_usb_state == BTMTK_USB_STATE_RESUME_DISCONNECT ? "RESUME_DISCONNECT" :
           btmtk_usb_state == BTMTK_USB_STATE_RESUME_PROBE ? "RESUME_PROBE" :
           btmtk_usb_state == BTMTK_USB_STATE_RESUME_FW_DUMP ? "RESUME_FW_DUMP" : "UNKNOWN", btmtk_usb_state,
           new_state == BTMTK_USB_STATE_INIT ? "INIT" :
           new_state == BTMTK_USB_STATE_DISCONNECT ? "DISCONNECT" :
           new_state == BTMTK_USB_STATE_PROBE ? "PROBE" :
           new_state == BTMTK_USB_STATE_WORKING ? "WORKING" :
           new_state == BTMTK_USB_STATE_EARLY_SUSPEND ? "EARLY_SUSPEND" :
           new_state == BTMTK_USB_STATE_SUSPEND ? "SUSPEND" :
           new_state == BTMTK_USB_STATE_RESUME ? "RESUME" :
           new_state == BTMTK_USB_STATE_LATE_RESUME ? "LATE_RESUME" :
           new_state == BTMTK_USB_STATE_FW_DUMP ? "FW_DUMP" :
           new_state == BTMTK_USB_STATE_SUSPEND_DISCONNECT ? "SUSPEND_DISCONNECT" :
           new_state == BTMTK_USB_STATE_SUSPEND_PROBE ? "SUSPEND_PROBE" :
           new_state == BTMTK_USB_STATE_RESUME_DISCONNECT ? "RESUME_DISCONNECT" :
           new_state == BTMTK_USB_STATE_RESUME_PROBE ? "RESUME_PROBE" :
           new_state == BTMTK_USB_STATE_RESUME_FW_DUMP ? "RESUME_FW_DUMP" : "UNKNOWN", new_state);
    btmtk_usb_state = new_state;
}

static unsigned int btmtk_usb_hci_snoop_get_microseconds(void)
{
    struct timeval now;

    do_gettimeofday(&now);
    return (now.tv_sec * 1000000 + now.tv_usec);
}

static void btmtk_usb_hci_snoop_init(void)
{
    int i;

    hci_cmd_snoop_index = HCI_SNOOP_ENTRY_NUM - 1;
    hci_event_snoop_index = HCI_SNOOP_ENTRY_NUM - 1;
    hci_acl_snoop_index = HCI_SNOOP_ENTRY_NUM - 1;
    for (i = 0; i < HCI_SNOOP_ENTRY_NUM; i++) {
        hci_cmd_snoop_len[i] = 0;
        hci_event_snoop_len[i] = 0;
        hci_acl_snoop_len[i] = 0;
    }
}

static void btmtk_usb_hci_snoop_save_cmd(u32 len, u8 *buf)
{
    u32 copy_len = HCI_SNOOP_BUF_SIZE;

    if (buf) {
        if (len < HCI_SNOOP_BUF_SIZE)
            copy_len = len;
        hci_cmd_snoop_len[hci_cmd_snoop_index] = copy_len & 0xff;
        memset(hci_cmd_snoop_buf[hci_cmd_snoop_index], 0,
               HCI_SNOOP_BUF_SIZE);
        btmtk_usb_osal_memcpy(hci_cmd_snoop_buf[hci_cmd_snoop_index],
                      buf, copy_len & 0xff);
        hci_cmd_snoop_timestamp[hci_cmd_snoop_index] =
            btmtk_usb_hci_snoop_get_microseconds();

        hci_cmd_snoop_index--;
        if (hci_cmd_snoop_index < 0)
            hci_cmd_snoop_index = HCI_SNOOP_ENTRY_NUM - 1;
    }
}

static void btmtk_usb_hci_snoop_save_event(u32 len, u8 *buf)
{
    u32 copy_len = HCI_SNOOP_BUF_SIZE;

    if (buf) {
        if (len < HCI_SNOOP_BUF_SIZE)
            copy_len = len;
        hci_event_snoop_len[hci_event_snoop_index] = copy_len;
        memset(hci_event_snoop_buf[hci_event_snoop_index], 0,
               HCI_SNOOP_BUF_SIZE);
        btmtk_usb_osal_memcpy(hci_event_snoop_buf
                      [hci_event_snoop_index], buf, copy_len);
        hci_event_snoop_timestamp[hci_event_snoop_index] =
            btmtk_usb_hci_snoop_get_microseconds();

        hci_event_snoop_index--;
        if (hci_event_snoop_index < 0)
            hci_event_snoop_index = HCI_SNOOP_ENTRY_NUM - 1;
    }
}

static void btmtk_usb_hci_snoop_save_acl(u32 len, u8 *buf)
{
    u32 copy_len = HCI_SNOOP_BUF_SIZE;

    if (buf) {
        if (len < HCI_SNOOP_BUF_SIZE)
            copy_len = len;
        hci_acl_snoop_len[hci_acl_snoop_index] = copy_len & 0xff;
        memset(hci_acl_snoop_buf[hci_acl_snoop_index], 0,
               HCI_SNOOP_BUF_SIZE);
        btmtk_usb_osal_memcpy(hci_acl_snoop_buf[hci_acl_snoop_index],
                      buf, copy_len & 0xff);
        hci_acl_snoop_timestamp[hci_acl_snoop_index] =
            btmtk_usb_hci_snoop_get_microseconds();

        hci_acl_snoop_index--;
        if (hci_acl_snoop_index < 0)
            hci_acl_snoop_index = HCI_SNOOP_ENTRY_NUM - 1;
    }
}

static void btmtk_usb_hci_snoop_print_to_log(void)
{
    int counter, index, j;

    BTUSB_INFO("HCI Command Dump");
    BTUSB_INFO("  index(len)(timestamp:us) :HCI Command");
    index = hci_cmd_snoop_index + 1;
    if (index >= HCI_SNOOP_ENTRY_NUM)
        index = 0;
    for (counter = 0; counter < HCI_SNOOP_ENTRY_NUM; counter++) {
        if (hci_cmd_snoop_len[index] > 0) {
            pr_cont("    %d(%02d)(%u) :", counter,
                   hci_cmd_snoop_len[index],
                   hci_cmd_snoop_timestamp[index]);
            for (j = 0; j < hci_cmd_snoop_len[index]; j++)
                pr_cont("%02X ", hci_cmd_snoop_buf[index][j]);
            pr_cont("\n");
        }
        index++;
        if (index >= HCI_SNOOP_ENTRY_NUM)
            index = 0;
    }

    BTUSB_INFO("HCI Event Dump");
    BTUSB_INFO("  index(len)(timestamp:us) :HCI Event");
    index = hci_event_snoop_index + 1;
    if (index >= HCI_SNOOP_ENTRY_NUM)
        index = 0;
    for (counter = 0; counter < HCI_SNOOP_ENTRY_NUM; counter++) {
        if (hci_event_snoop_len[index] > 0) {
            pr_cont("    %d(%02d)(%u) :", counter,
                   hci_event_snoop_len[index],
                   hci_event_snoop_timestamp[index]);
            for (j = 0; j < hci_event_snoop_len[index]; j++)
                pr_cont("%02X ", hci_event_snoop_buf[index][j]);
            pr_cont("\n");
        }
        index++;
        if (index >= HCI_SNOOP_ENTRY_NUM)
            index = 0;
    }

    BTUSB_INFO("HCI ACL Dump");
    BTUSB_INFO("  index(len)(timestamp:us) :ACL");
    index = hci_acl_snoop_index + 1;
    if (index >= HCI_SNOOP_ENTRY_NUM)
        index = 0;
    for (counter = 0; counter < HCI_SNOOP_ENTRY_NUM; counter++) {
        if (hci_acl_snoop_len[index] > 0) {
            pr_cont("    %d(%02d)(%u) :", counter,
                   hci_acl_snoop_len[index],
                   hci_acl_snoop_timestamp[index]);
            for (j = 0; j < hci_acl_snoop_len[index]; j++)
                pr_cont("%02X ", hci_acl_snoop_buf[index][j]);
            pr_cont("\n");
        }
        index++;
        if (index >= HCI_SNOOP_ENTRY_NUM)
            index = 0;
    }
}

static int btmtk_usb_send_assert_cmd(void)
{
    int ret = 0;

    if (g_data->interrupt_urb_submitted == 0) {
        ret = btmtk_usb_submit_bulk_in_urb();
        if (ret < 0) {
            BTUSB_ERR("%s: failed", __func__);
            return -1;
        }
    }

    ret = btmtk_usb_send_assert_cmd_ctrl();
    if (ret < 0)
        ret = btmtk_usb_send_assert_cmd_bulk();
    if (ret < 0) {
        BTUSB_ERR("%s: send assert cmd fail,tigger hw reset only",
              __func__);
        btmtk_usb_toggle_rst_pin();
    }
    return ret;
}

void btmtk_usb_toggle_rst_pin(void)
{
    BTUSB_INFO("%s: begig", __func__);
    need_reset_stack = 1;

    do {
        typedef void (*pdwnc_func) (u8 fgReset);
        char *pdwnc_func_name = "PDWNC_SetBTInResetState";
        pdwnc_func pdwndFunc =
            (pdwnc_func) kallsyms_lookup_name(pdwnc_func_name);

        if (pdwndFunc) {
            BTUSB_INFO("%s: Invoke %s(%d)", __func__,
                   pdwnc_func_name, 1);
            pdwndFunc(1);
        } else
            BTUSB_INFO("%s: No Exported Func Found [%s]", __func__,
                   pdwnc_func_name);
    } while (0);

    btmtk_usb_hci_snoop_print_to_log();

    BTUSB_INFO("%s: toggle GPIO %d", __func__, BT_DONGLE_RESET_GPIO_PIN);
    do {
        typedef void (*func_ptr) (unsigned gpio, int init_value);
        char *func_name = "mtk_gpio_direction_output";
        func_ptr pFunc = (func_ptr) kallsyms_lookup_name(func_name);

        if (pFunc) {
            BTUSB_INFO("%s: Invoke %s(%d)", __func__, func_name, 1);
            pFunc(BT_DONGLE_RESET_GPIO_PIN, 0);
            mdelay(500);
            pFunc(BT_DONGLE_RESET_GPIO_PIN, 1);
        } else
            BTUSB_INFO("%s: No Exported Func Found [%s]", __func__,
                   func_name);

    } while (0);
    BTUSB_INFO("%s: end", __func__);
}

static void btmtk_usb_lock_unsleepable_lock(P_OSAL_UNSLEEPABLE_LOCK pUSL)
{
    spin_lock_irqsave(&(pUSL->lock), pUSL->flag);
}

static void btmtk_usb_unlock_unsleepable_lock(P_OSAL_UNSLEEPABLE_LOCK pUSL)
{
    spin_unlock_irqrestore(&(pUSL->lock), pUSL->flag);
}

static void btmtk_usb_load_code_from_bin(u8 **image, char *bin_name,
                     struct device *dev, u32 *code_len)
{
    const struct firmware *fw_entry;
    int err = 0;

    if (g_data->rom_patch_image && g_data->rom_patch_image_len) {
        /* o need to request firmware again. */
        *image = g_data->rom_patch_image;
        *code_len = g_data->rom_patch_image_len;
        return;
    }

    err = request_firmware(&fw_entry, bin_name, dev);
    if (err != 0) {
        *image = NULL;
        BTUSB_ERR("%s: request_firmware fail!! error code = %d",
              __func__, err);
        return;
    }

    *image = kmalloc(fw_entry->size, GFP_ATOMIC);
    btmtk_usb_osal_memcpy(*image, fw_entry->data, fw_entry->size);
    *code_len = fw_entry->size;

    g_data->rom_patch_image = *image;
    g_data->rom_patch_image_len = *code_len;

    release_firmware(fw_entry);
}

static void btmtk_usb_stop_traffic(void)
{
    if ((g_data->bulk_urb_submitted == 0) &&
        (g_data->interrupt_urb_submitted == 0)) {
        return;
    }

    BTUSB_INFO("%s: start", __func__);
    usb_kill_anchored_urbs(&g_data->intr_anchor);
    usb_kill_anchored_urbs(&g_data->bulk_anchor);
    g_data->bulk_urb_submitted = 0;
    g_data->interrupt_urb_submitted = 0;

    BTUSB_INFO("%s: end", __func__);
}

static void btmtk_usb_waker(struct work_struct *work)
{
    int err;

    err = usb_autopm_get_interface(g_data->intf);

    if (err < 0)
        return;

    usb_autopm_put_interface(g_data->intf);
}

static int btmtk_usb_submit_intr_urb(void)
{
    struct urb *urb;
    u8 *buf;
    unsigned int pipe;
    int err, size;

    BTUSB_INFO("%s: begin", __func__);

    if (g_data->interrupt_urb_submitted) {
        BTUSB_WARN("%s: already submitted", __func__);
        return 0;
    }
    g_data->interrupt_urb_submitted = 0;

    if (!g_data->intr_ep) {
        BTUSB_ERR("%s: error 1", __func__);
        return -ENODEV;
    }

    urb = usb_alloc_urb(0, GFP_KERNEL);
    if (!urb) {
        BTUSB_ERR("%s: error 2", __func__);
        return -ENOMEM;
    }
    /* size = le16_to_cpu(g_data->intr_ep->wMaxPacketSize); */
    size = le16_to_cpu(HCI_MAX_EVENT_SIZE);
    BTUSB_INFO("%s: maximum packet size:%d", __func__, size);

    buf = kmalloc(size, GFP_KERNEL);
    if (!buf) {
        usb_free_urb(urb);
        BTUSB_ERR("%s: error 3", __func__);
        return -ENOMEM;
    }

    pipe = usb_rcvintpipe(g_data->udev, g_data->intr_ep->bEndpointAddress);

    usb_fill_int_urb(urb, g_data->udev, pipe, buf, size,
             btmtk_usb_intr_complete, g_data,
             g_data->intr_ep->bInterval);

    urb->transfer_flags |= URB_FREE_BUFFER;

    usb_anchor_urb(urb, &g_data->intr_anchor);

    err = usb_submit_urb(urb, GFP_KERNEL);
    if (err < 0) {
        if (err != -EPERM && err != -ENODEV)
            BTUSB_ERR("%s: urb %p submission failed (%d)", __func__,
                  urb, -err);
        usb_unanchor_urb(urb);
    } else {
        g_data->interrupt_urb_submitted = 1;
    }

    usb_free_urb(urb);
    BTUSB_INFO("%s: end", __func__);
    return err;
}

static int btmtk_usb_submit_bulk_in_urb(void)
{
    struct urb *urb = NULL;
    u8 *buf = NULL;
    unsigned int pipe = 0;
    int err = 0;
    int size = HCI_MAX_FRAME_SIZE;

    BTUSB_INFO("%s: begin", __func__);

    if (g_data->bulk_urb_submitted) {
        BTUSB_WARN("%s: already submitted", __func__);
        return 0;
    }
    g_data->bulk_urb_submitted = 0;

    if (!g_data->bulk_rx_ep) {
        BTUSB_ERR("%s: end error 1", __func__);
        return -ENODEV;
    }

    urb = usb_alloc_urb(0, GFP_KERNEL);
    if (!urb) {
        BTUSB_ERR("%s: end error 2", __func__);
        return -ENOMEM;
    }

    buf = kmalloc(size, GFP_KERNEL);
    if (!buf) {
        usb_free_urb(urb);
        BTUSB_ERR("%s: end error 3", __func__);
        return -ENOMEM;
    }
    pipe =
        usb_rcvbulkpipe(g_data->udev, g_data->bulk_rx_ep->bEndpointAddress);
    usb_fill_bulk_urb(urb, g_data->udev, pipe, buf, size,
              btmtk_usb_bulk_in_complete, g_data);

    urb->transfer_flags |= URB_FREE_BUFFER;

    usb_mark_last_busy(g_data->udev);
    usb_anchor_urb(urb, &g_data->bulk_anchor);

    err = usb_submit_urb(urb, GFP_KERNEL);
    if (err < 0) {
        if (err != -EPERM && err != -ENODEV)
            BTUSB_ERR("%s: urb %p submission failed (%d)", __func__,
                  urb, -err);
        usb_unanchor_urb(urb);
    } else {
        g_data->bulk_urb_submitted = 1;
    }

    usb_free_urb(urb);
    BTUSB_INFO("%s: end", __func__);
    return err;
}

static int btmtk_usb_send_wmt_cmd(const u8 *cmd, const int cmd_len,
        const u8 *event, const int event_len, u32 delay)
{
    int ret = -1;                       /* if successful, read length */
    int i = 0;
    BOOL check = FALSE;

    if (g_data == NULL || g_data->udev == NULL || g_data->io_buf == NULL
            || cmd == NULL) {
        BTUSB_ERR("%s: incorrect cmd pointer", __func__);
        return ret;
    }
    if (event != NULL && event_len > 0)
        check = TRUE;

    /* send WMT command */
    ret = usb_control_msg(g_data->udev, usb_sndctrlpipe(g_data->udev, 0),
            0x01, DEVICE_CLASS_REQUEST_OUT, 0x30, 0x00, (void *)cmd, cmd_len,
            USB_CTRL_IO_TIMO);
    if (ret < 0) {
        BTUSB_ERR("%s: command send failed(%d)", __func__, ret);
        return ret;
    }

    /* ms delay */
    mdelay(delay);

    if (event_len == -1) {
        /**
         * If event_len == -1, DO NOT read event, since FW wouldn't feedback
         */
        return 0;
    }

    /* check WMT event */
    memset(g_data->io_buf, 0, USB_IO_BUF_SIZE);
    ret = usb_control_msg(g_data->udev, usb_rcvctrlpipe(g_data->udev, 0),
                0x01, DEVICE_VENDOR_REQUEST_IN, 0x30, 0x00, g_data->io_buf,
                USB_IO_BUF_SIZE, USB_CTRL_IO_TIMO);
    if (ret < 0) {
        BTUSB_ERR("%s: event get failed(%d)", __func__, ret);
        if (check == TRUE)
            return ret;
        else
            return 0;   /* Do not ask read so return 0 */
    }

    if (check == TRUE) {
        /* maybe don't care some returned parameters */
        if (ret >= event_len) {
            for (i = 0; i < event_len; i++) {
                if (event[i] != g_data->io_buf[i])
                    break;
            }
        } else {
            BTUSB_ERR("%s: event length is not match(%d/%d)", __func__, ret, event_len);
        }
        if (i != event_len) {
            BTUSB_ERR("%s: got unknown event:(%d/%d)", __func__, i, event_len);
            pr_cont("\t");
            for (i = 0; i < ret && i < 64; i++)
                pr_cont("%02X ", g_data->io_buf[i]);
                pr_cont("\n");
            } else {
                return ret; /* return read length */
            }
    }
    return -1;
}

static int btmtk_usb_send_hci_cmd(const u8 *cmd, const int cmd_len,
        const u8 *event, const int event_len)
{
    int ret = -1;                               /* if successful, read length */
    int len = 0;
    int i = 0;
    unsigned long timo = 0;
    BOOL check = FALSE;

    /* parameter check */
    if (g_data == NULL || g_data->udev == NULL || g_data->io_buf == NULL
            || cmd == NULL) {
        BTUSB_ERR("%s: incorrect cmd pointer", __func__);
        return ret;
    }
    if (event != NULL && event_len > 0)
        check = TRUE;

    /* need get event by interrupt, stop traffic before cmd send */
    btmtk_usb_stop_traffic();

    /* send HCI command */
    ret = usb_control_msg(g_data->udev, usb_sndctrlpipe(g_data->udev, 0),
            0, DEVICE_CLASS_REQUEST_OUT, 0, 0,
            (void *)cmd, cmd_len, USB_CTRL_IO_TIMO);
    if (ret < 0) {
        BTUSB_ERR("%s: command send failed(%d)", __func__, ret);
        return ret;
    }

    if (event_len == -1) {
        /**
         * If event_len == -1, DO NOT read event, since FW wouldn't feedback
         */
        return 0;
    }

    /* check HCI event */
    timo = jiffies + msecs_to_jiffies(USB_INTR_MSG_TIMO);
    do {
        memset(g_data->io_buf, 0, USB_IO_BUF_SIZE);
        ret = usb_interrupt_msg(g_data->udev, usb_rcvintpipe(g_data->udev, 1),
                g_data->io_buf, USB_IO_BUF_SIZE, &len, USB_INTR_MSG_TIMO);
        if (ret < 0) {
            BTUSB_ERR("%s: event get failed(%d)", __func__, ret);
            if (check == TRUE)
                return ret;
            else
                return 0;   /* Do not ask read so return 0 */
        }

        if (check == TRUE) {
            /* maybe don't care some returned parameters */
            if (len >= event_len) {
                for (i = 0; i < event_len; i++) {
                    if (event[i] != g_data->io_buf[i])
                        break;
                }
            } else {
                BTUSB_ERR("%s: event length is not match(%d/%d)", __func__, len, event_len);
            }
            if (i != event_len) {
                BTUSB_ERR("%s: got unknown event:(%d/%d)", __func__, i, event_len);
                pr_cont("\t");
                for (i = 0; i < len && i < 64; i++)
                    pr_cont("%02X ", g_data->io_buf[i]);
                pr_cont("\n");
            } else {
                return len; /* actually read length */
            }
        }
    } while (time_before(jiffies, timo));

    BTUSB_ERR("%s: error, got event timeout, jiffies = %lu", __func__, jiffies);
    return -1;
}

int btmtk_usb_meta_send_data(const u8 *buffer, const unsigned int length)
{
    int ret = 0;

    if (buffer[0] != 0x01) {
        BTUSB_WARN("the data from meta isn't HCI command, value: 0x%X",
               buffer[0]);
    } else {
        u8 *buf = (u8 *)buffer;

        ret =
            usb_control_msg(g_data->udev,
                    usb_sndctrlpipe(g_data->udev, 0), 0x0,
                    DEVICE_CLASS_REQUEST_OUT, 0x00, 0x00,
                    buf + 1, length - 1,
                    USB_CTRL_IO_TIMO);
    }

    if (ret < 0) {
        BTUSB_ERR("%s: error1(%d)", __func__, ret);
        return ret;
    }

    return length;
}

int btmtk_usb_send_data(const u8 *buffer, const unsigned int length)
{
    struct urb *urb;
    unsigned int pipe;
    int err;

    if (!g_data->bulk_tx_ep) {
        BTUSB_ERR("%s: No bulk_tx_ep", __func__);
        return -ENODEV;
    }

    urb = usb_alloc_urb(0, GFP_KERNEL);

    if (!urb) {
        BTUSB_ERR("%s: No memory", __func__);
        return -ENOMEM;
    }

    if (buffer[0] == 0x02) {

        while (g_data->meta_tx != 0)
            udelay(500);

        g_data->meta_tx = 1;

        pipe =
            usb_sndbulkpipe(g_data->udev,
                    g_data->bulk_tx_ep->bEndpointAddress);

        usb_fill_bulk_urb(urb, g_data->udev, pipe, (void *)buffer + 1,
                  length - 1, btmtk_usb_tx_complete_meta,
                  g_data);

        usb_anchor_urb(urb, &g_data->tx_anchor);

        err = usb_submit_urb(urb, GFP_KERNEL);
        if (err == 0)
            err = length;
        else
            BTUSB_ERR("send ACL data fail, error:%d", err);
        mdelay(1);
    } else if (buffer[0] == 0x03) {
        BTUSB_WARN("%s: Iso not supported", __func__);
        err = -1;
    } else {
        BTUSB_WARN("%s: unknown data", __func__);
        err = -1;
    }

    if (err < 0) {
        BTUSB_ERR("%s: urb %p submission failed (%d)", __func__, urb,
              -err);

        kfree(urb->setup_packet);
        usb_unanchor_urb(urb);
    } else {
        usb_mark_last_busy(g_data->udev);
    }

    usb_free_urb(urb);
    return err;

}

static int btmtk_usb_BT_init(void)
{
    dev_t devID = MKDEV(BT_major, 0);
    int ret = 0;
    int cdevErr = 0;
    int major = 0;

    BTUSB_INFO("%s: begin", __func__);
    btmtk_usb_set_state(BTMTK_USB_STATE_INIT);

    ret = alloc_chrdev_region(&devID, 0, 1, "BT_chrdev");
    if (ret) {
        BTUSB_ERR("fail to allocate chrdev");
        return ret;
    }

    BT_major = major = MAJOR(devID);
    BTUSB_INFO("%s: major number: %d", __func__, BT_major);

    cdev_init(&BT_cdev, &BT_fops);
    BT_cdev.owner = THIS_MODULE;

    cdevErr = cdev_add(&BT_cdev, devID, 1);
    if (cdevErr)
        goto error;

    BTUSB_INFO("%s: %s driver(major %d) installed.", __func__, "BT_chrdev",
           BT_major);

    pBTClass = class_create(THIS_MODULE, "BT_chrdev");
    if (IS_ERR(pBTClass)) {
        BTUSB_ERR("class create fail, error code(%ld)",
              PTR_ERR(pBTClass));
        goto err1;
    }

    pBTDev = device_create(pBTClass, NULL, devID, NULL, "stpbt");
    if (IS_ERR(pBTDev)) {
        BTUSB_ERR("device create fail, error code(%ld)",
              PTR_ERR(pBTDev));
        goto err2;
    }
    /* init wait queue */
    init_waitqueue_head(&(inq));

    /* register system power off callback function. */
    do {
        typedef void (*func_ptr) (int (*f) (void));
        char *func_name = "RegisterPdwncCallback";
        func_ptr pFunc = (func_ptr) kallsyms_lookup_name(func_name);

        if (pFunc) {
            BTUSB_INFO("%s: Register Pdwnc callback success.",
                   __func__);
            pFunc(&btmtk_usb_standby);
        } else
            BTUSB_WARN
                ("%s: No Exported Func Found [%s], just skip!",
                 __func__, func_name);
    } while (0);

    /* register early_suspend / late_reasume callback function. */
    do {
        char *register_early_suspend_func_name =
            "RegisterEarlySuspendNotification";
        char *register_late_resume_func_name =
            "RegisterLateResumeNotification";

        register_early_suspend_func = (register_early_suspend)
            kallsyms_lookup_name(register_early_suspend_func_name);
        register_late_resume_func = (register_late_resume)
            kallsyms_lookup_name(register_late_resume_func_name);

        if (register_early_suspend_func && register_late_resume_func) {
            BTUSB_INFO
                ("%s: Register early suspend/late resume nitofication success.",
                 __func__);
            register_early_suspend_func(&btmtk_usb_early_suspend);
            register_late_resume_func(&btmtk_usb_late_resume);
        } else {
            BTUSB_WARN
                ("%s: Failed to register early suspend/late resume notification",
                 __func__);
        }
    } while (0);

    /* allocate buffers. */
    if (btmtk_usb_allocate_memory() < 0) {
        BTUSB_ERR("%s: allocate memory failed!", __func__);
        goto err2;
    }

    btmtk_usb_set_state(BTMTK_USB_STATE_DISCONNECT);
    BTUSB_INFO("%s: end", __func__);
    return 0;

err2:
    if (pBTClass) {
        class_destroy(pBTClass);
        pBTClass = NULL;
    }

    btmtk_usb_free_memory();

err1:
error:
    if (cdevErr == 0)
        cdev_del(&BT_cdev);

    if (ret == 0)
        unregister_chrdev_region(devID, 1);
    BTUSB_ERR("%s: error", __func__);
    return -1;
}

static void btmtk_usb_BT_exit(void)
{
    dev_t dev = MKDEV(BT_major, 0);

    if (pBTDev) {
        device_destroy(pBTClass, dev);
        pBTDev = NULL;
    }

    if (pBTClass) {
        class_destroy(pBTClass);
        pBTClass = NULL;
    }

    cdev_del(&BT_cdev);
    unregister_chrdev_region(dev, 1);

    btmtk_usb_free_memory();

    btmtk_usb_set_state(BTMTK_USB_STATE_UNKNOWN);
    BTUSB_INFO("%s: BT_chrdev driver removed.", __func__);
}

static void btmtk_usb_handle_resume(void)
{
    if (btmtk_usb_submit_intr_urb() < 0)
        btmtk_usb_send_assert_cmd();
    if (btmtk_usb_submit_bulk_in_urb() < 0)
        btmtk_usb_send_assert_cmd();

    btmtk_usb_set_state(BTMTK_USB_STATE_WORKING);
    wake_up_interruptible(&inq);
}

static void btmtk_usb_handle_entering_WoBLE_state(void)
{
    BTUSB_INFO("%s: begin", __func__);
#if SUPPORT_MT7662
    if (is_mt7662(g_data)) {
        btmtk_usb_send_woble_suspend_cmd();
        BTUSB_INFO("%s: end", __func__);
        return;
    }
#endif
#if SUPPORT_MT7668
    if (is_mt7668(g_data)) {
        while (g_data->is_mt7668_dongle_state == BTMTK_USB_7668_DONGLE_STATE_POWERING_ON ||
               g_data->is_mt7668_dongle_state == BTMTK_USB_7668_DONGLE_STATE_POWERING_OFF) {
            BTUSB_INFO("%s: dongle state is POWERING ON or OFF.", __func__);
            mdelay(100);
        }

        if (g_data->is_mt7668_dongle_state == BTMTK_USB_7668_DONGLE_STATE_POWER_OFF) {
            if (btmtk_usb_send_wmt_power_on_cmd_7668() < 0) {
                g_data->is_mt7668_dongle_state = BTMTK_USB_7668_DONGLE_STATE_ERROR;
                BTUSB_INFO("%s: end", __func__);
                return;
            }
            btmtk_usb_send_hci_tci_set_sleep_cmd_7668();
            btmtk_usb_send_hci_reset_cmd();
            g_data->is_mt7668_dongle_state = BTMTK_USB_7668_DONGLE_STATE_POWER_ON;
        }

        if (g_data->is_mt7668_dongle_state == BTMTK_USB_7668_DONGLE_STATE_POWER_ON) {
            if (btmtk_usb_send_woble_suspend_cmd() < 0) {
                g_data->is_mt7668_dongle_state = BTMTK_USB_7668_DONGLE_STATE_ERROR;
                BTUSB_INFO("%s: end", __func__);
                return;
            }
            g_data->is_mt7668_dongle_state = BTMTK_USB_7668_DONGLE_STATE_WOBLE;
        }

        if (g_data->is_mt7668_dongle_state == BTMTK_USB_7668_DONGLE_STATE_WOBLE)
            BTUSB_INFO("%s: Dongle is in WoBLE state.", __func__);
        else
            BTUSB_INFO("%s: Dongle state error (%d)", __func__, g_data->is_mt7668_dongle_state);

        BTUSB_INFO("%s: end", __func__);
        return;
    }
#endif
    BTUSB_WARN("%s: No matched dongle!", __func__);
    return;
}

/*============================================================================*/
/* Internal Functions : Chip Related */
/*============================================================================*/
#define ______________________________________Internal_Functions_Chip_Related
/**
 * Only for load rom patch function, tmp_str[15] is '\n'
 */
#define SHOW_FW_DETAILS(s)                                                  \
    BTUSB_INFO("%s: %s = %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", __func__, s,      \
            tmp_str[0], tmp_str[1], tmp_str[2], tmp_str[3],                 \
            tmp_str[4], tmp_str[5], tmp_str[6], tmp_str[7],                 \
            tmp_str[8], tmp_str[9], tmp_str[10], tmp_str[11],               \
            tmp_str[12], tmp_str[13], tmp_str[14]/*, tmp_str[15]*/)

#if SUPPORT_MT7662
static int btmtk_usb_load_rom_patch_7662(void)
{
    u32 loop = 0;
    u32 value;
    s32 sent_len;
    int ret = 0;
    u16 total_checksum = 0;
    struct urb *urb;
    u32 patch_len = 0;
    u32 cur_len = 0;
    dma_addr_t data_dma;
    struct completion sent_to_mcu_done;
    int first_block = 1;
    u8 phase;
	int retValue;
    void *buf;
    u8 *pos;
    char *tmp_str;
    unsigned int pipe = usb_sndbulkpipe(g_data->udev,
                        g_data->
                        bulk_tx_ep->bEndpointAddress);

load_patch_protect:
    btmtk_usb_switch_iobase_7662(WLAN);
    btmtk_usb_io_read32_7662(SEMAPHORE_03, &value);
    loop++;

    if ((value & 0x01) == 0x00) {
        if (loop < 1000) {
            mdelay(1);
            goto load_patch_protect;
        } else {
            BTUSB_WARN("%s: WARNING! Can't get semaphore! Continue",
                   __func__);
        }
    }

    btmtk_usb_switch_iobase_7662(SYSCTL);

    btmtk_usb_io_write32_7662(0x1c, 0x30);

    btmtk_usb_switch_iobase_7662(WLAN);

    /* check ROM patch if upgrade */
    btmtk_usb_io_read32_7662(CLOCK_CTL, &value);
    if ((value & 0x01) == 0x01) {
        BTUSB_INFO("%s: no need to load rom patch", __func__);
        if (!is_mt7662T(g_data))
            btmtk_usb_send_dummy_bulk_out_packet_7662();
        goto error0;
    }

    urb = usb_alloc_urb(0, GFP_ATOMIC);

    if (!urb) {
        ret = -ENOMEM;
        goto error0;
    }

    buf =
        usb_alloc_coherent(g_data->udev, UPLOAD_PATCH_UNIT, GFP_ATOMIC,
                   &data_dma);
    if (!buf) {
        ret = -ENOMEM;
        goto error1;
    }

    pos = buf;
    btmtk_usb_load_code_from_bin(&g_data->rom_patch,
                     g_data->rom_patch_bin_file_name,
                     &g_data->udev->dev,
                     &g_data->rom_patch_len);

    if (!g_data->rom_patch) {
        BTUSB_ERR
            ("%s: please assign a rom patch(/etc/firmware/%s)or(/lib/firmware/%s)",
             __func__, g_data->rom_patch_bin_file_name,
             g_data->rom_patch_bin_file_name);

        ret = -1;
        goto error2;
    }

    tmp_str = g_data->rom_patch;
    SHOW_FW_DETAILS("FW Version");
    SHOW_FW_DETAILS("build Time");

    tmp_str = g_data->rom_patch + 16;
    BTUSB_INFO("%s: platform = %c%c%c%c", __func__, tmp_str[0], tmp_str[1],
           tmp_str[2], tmp_str[3]);

    tmp_str = g_data->rom_patch + 20;
    BTUSB_INFO("%s: HW/SW version = %c%c%c%c", __func__, tmp_str[0],
           tmp_str[1], tmp_str[2], tmp_str[3]);

    tmp_str = g_data->rom_patch + 24;
    BTUSB_INFO("%s: Patch version = %c%c%c%c", __func__, tmp_str[0],
           tmp_str[1], tmp_str[2], tmp_str[3]);

    do {
        typedef void (*pdwnc_func) (u8 fgReset);
        char *pdwnc_func_name = "PDWNC_SetBTInResetState";
        pdwnc_func pdwndFunc = NULL;

        pdwndFunc = (pdwnc_func) kallsyms_lookup_name(pdwnc_func_name);

        if (pdwndFunc) {
            BTUSB_INFO("%s: Invoke %s(%d)", __func__,
                   pdwnc_func_name, 1);
            pdwndFunc(1);
        } else {
            BTUSB_WARN("%s: No Exported Func Found [%s]", __func__,
                   pdwnc_func_name);
        }
    } while (0);

    BTUSB_INFO("%s: loading rom patch...", __func__);

    init_completion(&sent_to_mcu_done);

    cur_len = 0x00;
    patch_len = g_data->rom_patch_len - PATCH_INFO_SIZE;
    BTUSB_INFO("%s: patch_len = %d", __func__, patch_len);

    /* loading rom patch */
    while (1) {
        s32 sent_len_max = UPLOAD_PATCH_UNIT - PATCH_HEADER_SIZE;

        sent_len =
            (patch_len - cur_len) >=
            sent_len_max ? sent_len_max : (patch_len - cur_len);

        if (sent_len > 0) {
            if (first_block == 1) {
                if (sent_len < sent_len_max)
                    phase = PATCH_PHASE3;
                else
                    phase = PATCH_PHASE1;
                first_block = 0;
            } else if (sent_len == sent_len_max) {
                phase = PATCH_PHASE2;
            } else {
                phase = PATCH_PHASE3;
            }

            /* prepare HCI header */
            pos[0] = 0x6F;
            pos[1] = 0xFC;
            pos[2] = (sent_len + 5) & 0xFF;
            pos[3] = ((sent_len + 5) >> 8) & 0xFF;

            /* prepare WMT header */
            pos[4] = 0x01;
            pos[5] = 0x01;
            pos[6] = (sent_len + 1) & 0xFF;
            pos[7] = ((sent_len + 1) >> 8) & 0xFF;

            pos[8] = phase;

            btmtk_usb_osal_memcpy(&pos[9],
                          g_data->rom_patch +
                          PATCH_INFO_SIZE + cur_len,
                          sent_len);

            BTUSB_INFO
                ("%s: sent_len = %d, cur_len = %d, phase = %d",
                 __func__, sent_len, cur_len, phase);

            usb_fill_bulk_urb(urb,
                      g_data->udev,
                      pipe,
                      buf,
                      sent_len + PATCH_HEADER_SIZE,
                      btmtk_usb_load_rom_patch_complete,
                      &sent_to_mcu_done);

            urb->transfer_dma = data_dma;
            urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

            ret = usb_submit_urb(urb, GFP_ATOMIC);
			
			BTUSB_INFO
                ("usb_submit_urb ret:%d",ret);
			

            if (ret)
                goto error2;
			
			retValue = wait_for_completion_timeout
                (&sent_to_mcu_done, msecs_to_jiffies(5000));
				
			BTUSB_INFO
                ("wait_for_completion_timeout ret:%d",retValue);

            if (!retValue) {
                usb_kill_urb(urb);
                BTUSB_ERR("%s: upload rom_patch timeout",
                      __func__);
                goto error2;
            }

            cur_len += sent_len;
            mdelay(1);

        } else {
            BTUSB_INFO("%s: loading rom patch... Done", __func__);
            break;
        }
    }

    mdelay(1);
    ret = btmtk_usb_get_rom_patch_result();
    mdelay(1);

    /* Send Checksum request */
    total_checksum =
        btmtk_usb_checksum16_7662(g_data->rom_patch + PATCH_INFO_SIZE,
                       patch_len);
    btmtk_usb_chk_crc_7662(patch_len);

    mdelay(1);

    if (total_checksum != btmtk_usb_get_crc_7662()) {
        BTUSB_ERR("checksum fail!, local(0x%x) <> fw(0x%x)",
              total_checksum, btmtk_usb_get_crc_7662());
        ret = -1;
        goto error2;
    }

    /* send check rom patch result request */
    mdelay(1);
    btmtk_usb_send_hci_check_rom_patch_result_cmd_7662();

    /* CHIP_RESET */
    mdelay(1);
    ret = btmtk_usb_send_wmt_reset_cmd();

    /* BT_RESET */
    mdelay(1);
    btmtk_usb_send_hci_reset_cmd();

    mdelay(1);
    btmtk_usb_send_hci_low_power_cmd_7662(TRUE);

    /* for WoBLE/WoW low power */
    mdelay(1);
    btmtk_usb_send_hci_set_ce_cmd_7662();

    mdelay(1);
    btmtk_usb_send_hci_set_tx_power_cmd_7662();

error2:
    usb_free_coherent(g_data->udev, UPLOAD_PATCH_UNIT, buf, data_dma);
error1:
    usb_free_urb(urb);
error0:
    btmtk_usb_io_write32_7662(SEMAPHORE_03, 0x1);

    do {
        typedef void (*pdwnc_func) (u8 fgReset);
        char *pdwnc_func_name = "PDWNC_SetBTInResetState";
        pdwnc_func pdwndFunc = NULL;

        pdwndFunc = (pdwnc_func) kallsyms_lookup_name(pdwnc_func_name);

        if (pdwndFunc) {
            BTUSB_INFO("%s: Invoke %s(%d)", __func__,
                   pdwnc_func_name, 0);
            pdwndFunc(0);
        } else
            BTUSB_WARN("%s: No Exported Func Found [%s]", __func__,
                   pdwnc_func_name);
    } while (0);

    return ret;
}

static int btmtk_usb_io_read32_7662(u32 reg, u32 *val)
{
    int ret = -1;
    u8 request = g_data->r_request;

    memset(g_data->io_buf, 0, USB_IO_BUF_SIZE);
    ret = usb_control_msg(g_data->udev, usb_rcvctrlpipe(g_data->udev, 0),
            request, DEVICE_VENDOR_REQUEST_IN, 0x0, reg,
            g_data->io_buf, sizeof(u32), USB_CTRL_IO_TIMO);

    if (ret < 0) {
        *val = 0xffffffff;
        BTUSB_ERR("%s: error(%d), reg=%x, value=%x", __func__, ret, reg,
              *val);
        return ret;
    }

    memmove(val, g_data->io_buf, sizeof(u32));
    *val = le32_to_cpu(*val);

    return 0;
}

static int btmtk_usb_io_write32_7662(u32 reg, u32 val)
{
    int ret;
    u16 value, index;
    u8 request = g_data->w_request;

    index = (u16) reg;
    value = val & 0x0000ffff;

    ret =
        usb_control_msg(g_data->udev, usb_sndctrlpipe(g_data->udev, 0),
                request, DEVICE_VENDOR_REQUEST_OUT, value, index,
                NULL, 0, USB_CTRL_IO_TIMO);

    if (ret < 0) {
        BTUSB_ERR("%s: error(%d), reg=%x, value=%x", __func__, ret, reg,
              val);
        return ret;
    }

    index = (u16) (reg + 2);
    value = (val & 0xffff0000) >> 16;

    ret =
        usb_control_msg(g_data->udev, usb_sndctrlpipe(g_data->udev, 0),
                request, DEVICE_VENDOR_REQUEST_OUT, value, index,
                NULL, 0, USB_CTRL_IO_TIMO);

    if (ret < 0) {
        BTUSB_ERR("%s: error(%d), reg=%x, value=%x", __func__, ret, reg,
              val);
        return ret;
    }

    if (ret > 0)
        ret = 0;

    return ret;
}

static int btmtk_usb_switch_iobase_7662(int base)
{
    int ret = 0;

    switch (base) {
    case SYSCTL:
        g_data->w_request = 0x42;
        g_data->r_request = 0x47;
        break;
    case WLAN:
        g_data->w_request = 0x02;
        g_data->r_request = 0x07;
        break;

    default:
        return -EINVAL;
    }

    return ret;
}

static u16 btmtk_usb_checksum16_7662(u8 *pData, int len)
{
    u32 sum = 0;

    while (len > 1) {
        sum += *((u16 *) pData);

        pData = pData + 2;

        if (sum & 0x80000000)
            sum = (sum & 0xFFFF) + (sum >> 16);

        len -= 2;
    }

    if (len)
        sum += *((u8 *) pData);

    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return (u16)(~sum);
}

static int btmtk_usb_chk_crc_7662(u32 checksum_len)
{
    int ret = 0;

    BT_DBG("%s", __func__);

    memmove(g_data->io_buf, &g_data->rom_patch_offset, 4);
    memmove(&g_data->io_buf[4], &checksum_len, 4);

    ret =
        usb_control_msg(g_data->udev, usb_sndctrlpipe(g_data->udev, 0), 0x1,
                DEVICE_VENDOR_REQUEST_OUT, 0x20, 0x00,
                g_data->io_buf, 8, USB_CTRL_IO_TIMO);

    if (ret < 0)
        BTUSB_ERR("%s: error(%d)", __func__, ret);

    return ret;
}

static u16 btmtk_usb_get_crc_7662(void)
{
    int ret = 0;
    u16 crc, count = 0;

    while (1) {
        ret =
            usb_control_msg(g_data->udev,
                    usb_rcvctrlpipe(g_data->udev, 0), 0x01,
                    DEVICE_VENDOR_REQUEST_IN, 0x21, 0x00,
                    g_data->io_buf, USB_IO_BUF_SIZE,
                    USB_CTRL_IO_TIMO);

        if (ret < 0) {
            crc = 0xFFFF;
            BTUSB_ERR("%s: error(%d)", __func__, ret);
        }

        memmove(&crc, g_data->io_buf, 2);

        crc = le16_to_cpu(crc);

        if (crc != 0xFFFF)
            break;

        mdelay(1);

        if (count++ > 100) {
            BTUSB_WARN("Query CRC over %d times", count);
            break;
        }
    }

    return crc;
}

static int btmtk_usb_send_hci_set_tx_power_cmd_7662(void)
{
    u8 cmd[] = { 0x79, 0xFC, 0x06, 0x07, 0x80, 0x00, 0x06, 0x07, 0x07 };
    u8 event[] = { 0x0E, 0x04, 0x01, 0x79, 0xFC, 0x00 };
    int ret = -1;                           /* if successful, 0 */

    ret = btmtk_usb_send_hci_cmd(cmd, sizeof(cmd), event, sizeof(event));
    if (ret < 0) {
        BTUSB_ERR("%s: failed(%d)", __func__, ret);
    } else {
        BTUSB_INFO("%s: OK", __func__);
        ret = 0;
    }

    return ret;
}

static int btmtk_usb_send_hci_check_rom_patch_result_cmd_7662(void)
{
    u8 cmd[] = { 0xD1, 0xFC, 0x04, 0x00, 0xE2, 0x40, 0x00 };
    u8 event[] = { 0x0E, 0x08, 0x01, 0xD1, 0xFC, 0x00 };
    int ret = -1;                           /* if successful, 0 */

    ret = btmtk_usb_send_hci_cmd(cmd, sizeof(cmd), event, sizeof(event));
    if (ret < 0) {
        BTUSB_ERR("%s: failed(%d)", __func__, ret);

    } else if (ret == sizeof(event) + 4) {
        if (g_data->io_buf[6] == 0 && g_data->io_buf[7] == 0
                && g_data->io_buf[8] == 0 && g_data->io_buf[9] == 0) {
            BTUSB_WARN("Check rom patch result: NG");
            return -1;
        } else {
            BTUSB_INFO("Check rom patch result: OK");
            ret = 0;
        }

    } else {
        BTUSB_ERR("%s: failed, incorrect response length(%d)", __func__, ret);
        return -1;
    }

    return ret;
}

static int btmtk_usb_send_hci_radio_on_cmd_7662(void)
{
    u8 cmd[] = { 0xC9, 0xFC, 0x02, 0x01, 0x01 };
    u8 event[] = { 0xE6, 0x02, 0x08, 0x01 };
    int ret = -1;                           /* if successful, 0 */

    ret = btmtk_usb_send_hci_cmd(cmd, sizeof(cmd), event, sizeof(event));
    if (ret < 0) {
        BTUSB_ERR("%s: failed(%d)", __func__, ret);
    } else {
        BTUSB_INFO("%s: OK", __func__);
        ret = 0;
    }

    return ret;
}

static int btmtk_usb_send_hci_set_ce_cmd_7662(void)
{
    u8 cmd[] = { 0xD1, 0xFC, 0x04, 0x0c, 0x07, 0x41, 0x00 };
    u8 event[] = { 0x0E, 0x08, 0x01, 0xD1, 0xFC, 0x00 };
    int ret = -1;                           /* if successful, 0 */

    ret = btmtk_usb_send_hci_cmd(cmd, sizeof(cmd), event, sizeof(event));
    if (ret < 0) {
        BTUSB_ERR("%s: failed(%d)", __func__, ret);

    } else if (ret == sizeof(event) + 4) {
        if (g_data->io_buf[6] & 0x01) {
            BTUSB_WARN("warning, 0x41070c[0] is 1!");
            ret = 0;
        } else {
            /* write 0x41070C[0] to 1 */
            u8 cmd2[11] = { 0xD0, 0xFC, 0x08, 0x0C, 0x07, 0x41, 0x00 };

            cmd2[7] = g_data->io_buf[6] | 0x01;
            cmd2[8] = g_data->io_buf[7];
            cmd2[9] = g_data->io_buf[8];
            cmd2[10] = g_data->io_buf[9];

            ret = btmtk_usb_send_hci_cmd(cmd, sizeof(cmd), NULL, 0);
            if (ret < 0) {
                BTUSB_ERR("%s: write 0x41070C failed(%d)", __func__, ret);
            } else {
                BTUSB_INFO("%s: OK", __func__);
                ret = 0;
            }
        }
    } else {
        BTUSB_ERR("%s: failed, incorrect response length(%d)", __func__, ret);
        return -1;
    }

    return ret;
}

static int btmtk_usb_send_hci_radio_off_cmd_7662(void)
{
    u8 cmd[] = { 0xC9, 0xFC, 0x02, 0x01, 0x00 };
    u8 event[] = { 0xE6, 0x02, 0x08, 0x00 };    /* unexpected opcode */
    int ret = -1;                               /* if successful, 0 */

    ret = btmtk_usb_send_hci_cmd(cmd, sizeof(cmd), event, sizeof(event));
    if (ret < 0) {
        BTUSB_ERR("%s: failed(%d)", __func__, ret);
    } else {
        BTUSB_INFO("%s: OK", __func__);
        ret = 0;
    }

    return ret;
}

static int btmtk_usb_send_hci_low_power_cmd_7662(BOOL enable)
{
    /* default for disable */
    u8 cmd[] = { 0x40, 0xFC, 0x00 };
    u8 event[] = { 0x0E, 0x04, 0x01, 0x40, 0xFC, 0x00 };
    int ret = -1;                           /* if successful, 0 */

    /* for enable */
    if (enable == TRUE) {
        cmd[0] = 0x41;
        event[3] = 0x41;
    }
    ret = btmtk_usb_send_hci_cmd(cmd, sizeof(cmd), event, sizeof(event));
    if (ret < 0) {
        BTUSB_ERR("%s: failed(%d)", __func__, ret);
    } else {
        BTUSB_INFO("%s: OK", __func__);
        ret = 0;
    }

    return ret;
}

static void btmtk_usb_send_dummy_bulk_out_packet_7662(void)
{
    int ret = 0;
    int actual_len;
    unsigned int pipe;
    u8 dummy_bulk_out_fuffer[8] = { 0 };

    pipe =
        usb_sndbulkpipe(g_data->udev, g_data->bulk_tx_ep->bEndpointAddress);
    ret =
        usb_bulk_msg(g_data->udev, pipe, dummy_bulk_out_fuffer, 8,
             &actual_len, 100);
    if (ret)
        BTUSB_ERR("%s: submit dummy bulk out failed!", __func__);
    else
        BTUSB_INFO("%s: 1. OK", __func__);

    ret =
        usb_bulk_msg(g_data->udev, pipe, dummy_bulk_out_fuffer, 8,
             &actual_len, 100);
    if (ret)
        BTUSB_ERR("%s: submit dummy bulk out failed!", __func__);
    else
        BTUSB_INFO("%s: 2. OK", __func__);
}
#endif /* SUPPORT_MT7662 */

#if SUPPORT_MT7668
static int btmtk_usb_io_read32_7668(u32 reg, u32 *val)
{
    int ret = -1;
    __le16 reg_high;
    __le16 reg_low;

    reg_high = ((reg >> 16) & 0xffff);
    reg_low = (reg & 0xffff);

    memset(g_data->io_buf, 0, USB_IO_BUF_SIZE);
    ret = usb_control_msg(g_data->udev, usb_rcvctrlpipe(g_data->udev, 0),
            0x63,                       /* bRequest */
            DEVICE_VENDOR_REQUEST_IN,   /* bRequestType */
            reg_high,                   /* wValue */
            reg_low,                    /* wIndex */
            g_data->io_buf,
            sizeof(u32), USB_CTRL_IO_TIMO);

    if (ret < 0) {
        *val = 0xffffffff;
        BTUSB_ERR("%s: error(%d), reg=%x, value=%x", __func__, ret, reg, *val);
        return ret;
    }

    memmove(val, g_data->io_buf, sizeof(u32));
    *val = le32_to_cpu(*val);

    return 0;
}

static BOOL btmtk_usb_check_need_load_rom_patch_7668(void)
{
    /* TRUE: need load patch., FALSE: do not need */
    u8 cmd[] = { 0x6F, 0xFC, 0x05, 0x01, 0x17, 0x01, 0x00, 0x01 };
    u8 event[] = { 0xE4, 0x05, 0x02, 0x17, 0x01, 0x00, /* 0x02 */ };    /* event[6] is key */
    int ret = -1;

    BTUSB_DBG_RAW(cmd, sizeof(cmd), "%s: Send CMD:", __func__);
    ret = btmtk_usb_send_wmt_cmd(cmd, sizeof(cmd), event, sizeof(event), 20);
    /* can't get correct event, need load patch */
    if (ret < 0)
        return TRUE;

    if (ret == sizeof(event) + 1 && (g_data->io_buf[6] == 0x00 || g_data->io_buf[6] == 0x01))
        return FALSE;   /* Do not need */

    return TRUE;
}

static int btmtk_usb_load_rom_patch_7668(void)
{
    s32 sent_len;
    int ret = 0;
    struct urb *urb;
    u32 patch_len = 0;
    u32 cur_len = 0;
    dma_addr_t data_dma;
    struct completion sent_to_mcu_done;
    int first_block = 1;
    u8 phase;
    void *buf;
    u8 *pos;
    char *tmp_str;
    unsigned int pipe = usb_sndbulkpipe(g_data->udev,
            g_data->bulk_tx_ep->bEndpointAddress);

    BTUSB_INFO("%s: begin", __func__);

    if (btmtk_usb_check_need_load_rom_patch_7668() == FALSE) {
        BTUSB_INFO("%s: no need to load rom patch", __func__);
        ret = btmtk_usb_send_wmt_reset_cmd();
        goto error0;
    }

    urb = usb_alloc_urb(0, GFP_ATOMIC);

    if (!urb) {
        ret = -ENOMEM;
        goto error0;
    }

    buf = usb_alloc_coherent(g_data->udev, UPLOAD_PATCH_UNIT, GFP_ATOMIC,
            &data_dma);

    if (!buf) {
        ret = -ENOMEM;
        goto error1;
    }

    pos = buf;

    btmtk_usb_load_code_from_bin(&g_data->rom_patch,
            g_data->rom_patch_bin_file_name, &g_data->udev->dev,
            &g_data->rom_patch_len);

    if (!g_data->rom_patch) {
        BTUSB_ERR("%s: please assign a rom patch(/etc/firmware/%s)or(/lib/firmware/%s)",
             __func__, g_data->rom_patch_bin_file_name,
             g_data->rom_patch_bin_file_name);

        ret = -1;
        goto error2;
    }

    tmp_str = g_data->rom_patch;
    SHOW_FW_DETAILS("FW Version");
    SHOW_FW_DETAILS("build Time");

    tmp_str = g_data->rom_patch + 16;
    BTUSB_INFO("%s: platform = %c%c%c%c", __func__, tmp_str[0], tmp_str[1],
           tmp_str[2], tmp_str[3]);

    tmp_str = g_data->rom_patch + 20;
    BTUSB_INFO("%s: HW/SW version = %c%c%c%c", __func__, tmp_str[0],
           tmp_str[1], tmp_str[2], tmp_str[3]);

    tmp_str = g_data->rom_patch + 24;

    BTUSB_INFO("loading rom patch...");

    init_completion(&sent_to_mcu_done);

    cur_len = 0x00;
    patch_len = g_data->rom_patch_len - PATCH_INFO_SIZE;
    BTUSB_INFO("%s: patch_len = %d", __func__, patch_len);

    BTUSB_INFO("%s: loading rom patch...", __func__);
    /* loading rom patch */
    while (1) {
        s32 sent_len_max = UPLOAD_PATCH_UNIT - PATCH_HEADER_SIZE;

        sent_len =
            (patch_len - cur_len) >=
            sent_len_max ? sent_len_max : (patch_len - cur_len);

        if (sent_len > 0) {
            if (first_block == 1) {
                if (sent_len < sent_len_max)
                    phase = PATCH_PHASE3;
                else
                    phase = PATCH_PHASE1;
                first_block = 0;
            } else if (sent_len == sent_len_max) {
                phase = PATCH_PHASE2;
            } else {
                phase = PATCH_PHASE3;
            }

            /* prepare HCI header */
            pos[0] = 0x6F;
            pos[1] = 0xFC;
            pos[2] = (sent_len + 5) & 0xFF;
            pos[3] = ((sent_len + 5) >> 8) & 0xFF;

            /* prepare WMT header */
            pos[4] = 0x01;
            pos[5] = 0x01;
            pos[6] = (sent_len + 1) & 0xFF;
            pos[7] = ((sent_len + 1) >> 8) & 0xFF;

            pos[8] = phase;

            memcpy(&pos[9],
                   g_data->rom_patch + PATCH_INFO_SIZE + cur_len,
                   sent_len);

            BTUSB_INFO
                ("%s: sent_len = %d, cur_len = %d, phase = %d",
                 __func__, sent_len, cur_len, phase);

            usb_fill_bulk_urb(urb,
                      g_data->udev,
                      pipe,
                      buf,
                      sent_len + PATCH_HEADER_SIZE,
                      btmtk_usb_load_rom_patch_complete,
                      &sent_to_mcu_done);

            urb->transfer_dma = data_dma;
            urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

            ret = usb_submit_urb(urb, GFP_ATOMIC);		

            if (ret) {
                BTUSB_ERR("%s: submit urb failed (%d)",
                      __func__, ret);
                goto error2;
            }

            if (!wait_for_completion_timeout
                (&sent_to_mcu_done, msecs_to_jiffies(1000))) {
                usb_kill_urb(urb);
                BTUSB_ERR("%s: upload rom_patch timeout",
                      __func__);
                goto error2;
            }

            cur_len += sent_len;

            mdelay(1);
            ret = btmtk_usb_get_rom_patch_result();
            mdelay(1);

        } else {
            BTUSB_INFO("%s: loading rom patch... Done", __func__);
            break;
        }
    }

    /* CHIP_RESET */
    ret = btmtk_usb_send_wmt_reset_cmd();
    g_data->is_mt7668_dongle_state = BTMTK_USB_7668_DONGLE_STATE_POWER_OFF;
error2:
    usb_free_coherent(g_data->udev, UPLOAD_PATCH_UNIT, buf, data_dma);
error1:
    usb_free_urb(urb);
error0:
    BTUSB_INFO("btmtk_usb_load_rom_patch end");
    return ret;
}

static int btmtk_usb_send_hci_tci_set_sleep_cmd_7668(void)
{
    u8 cmd[] = { 0x7A, 0xFC, 0x07, 0x05, 0x40, 0x06, 0x40, 0x06, 0x00, 0x00 };
    u8 event[] = { 0x0E, 0x04, 0x01, 0x7A, 0xFC, 0x00 };
    int ret = -1;                                   /* if successful, 0 */

    ret = btmtk_usb_send_hci_cmd(cmd, sizeof(cmd), event, sizeof(event));
    if (ret < 0) {
        BTUSB_ERR("%s: failed(%d)", __func__, ret);
    } else {
        BTUSB_INFO("%s: OK", __func__);
        ret = 0;
    }

    return ret;
}

static int btmtk_usb_send_wmt_power_on_cmd_7668(void)
{
    u8 count = 0;                                           /* retry 3 times */
    u8 cmd[] = { 0x6F, 0xFC, 0x06, 0x01, 0x06, 0x02, 0x00, 0x00, 0x01 };
    u8 event[] = { 0xE4, 0x05, 0x02, 0x06, 0x01, 0x00 };    /* event[6] is key */
    int ret = -1;                                           /* if successful, 0 */

    g_data->is_mt7668_dongle_state = BTMTK_USB_7668_DONGLE_STATE_POWERING_ON;
    do {
        ret = btmtk_usb_send_wmt_cmd(cmd, sizeof(cmd), event, sizeof(event), 20);
        if (ret < 0) {
            BTUSB_ERR("%s: failed(%d)", __func__, ret);
            g_data->is_mt7668_dongle_state = BTMTK_USB_7668_DONGLE_STATE_ERROR;

        } else if (ret == sizeof(event) + 1) {
            switch (g_data->io_buf[6]) {
            case 0:             /* successful */
                BTUSB_INFO("%s: OK", __func__);
                g_data->is_mt7668_dongle_state = BTMTK_USB_7668_DONGLE_STATE_POWER_ON;
                ret = 0;
                break;
            case 2:             /* retry */
                BTUSB_INFO("%s: Try again", __func__);
                continue;
            default:
                BTUSB_INFO("%s: Unknown result: %02X", __func__, g_data->io_buf[6]);
                g_data->is_mt7668_dongle_state = BTMTK_USB_7668_DONGLE_STATE_ERROR;
                return -1;
            }

        } else {
            BTUSB_ERR("%s: failed, incorrect response length(%d)", __func__, ret);
            g_data->is_mt7668_dongle_state = BTMTK_USB_7668_DONGLE_STATE_ERROR;
            return -1;
        }
    } while (++count < 3 && ret > 0);

    return ret;
}

static int btmtk_usb_send_wmt_power_off_cmd_7668(void)
{
    u8 cmd[] = { 0x6F, 0xFC, 0x06, 0x01, 0x06, 0x02, 0x00, 0x00, 0x00 };
    u8 event[] = { 0xE4, 0x05, 0x02, 0x06, 0x01, 0x00, 0x00 };
    int ret = -1;                                           /* if successful, 0 */

    g_data->is_mt7668_dongle_state = BTMTK_USB_7668_DONGLE_STATE_POWERING_OFF;
    ret = btmtk_usb_send_wmt_cmd(cmd, sizeof(cmd), event, sizeof(event), 20);
    if (ret < 0) {
        BTUSB_ERR("%s: failed(%d)", __func__, ret);
        g_data->is_mt7668_dongle_state = BTMTK_USB_7668_DONGLE_STATE_ERROR;
        return ret;
    }

    BTUSB_INFO("%s: OK", __func__);
    g_data->is_mt7668_dongle_state = BTMTK_USB_7668_DONGLE_STATE_POWER_OFF;
    return 0;
}
#endif /* SUPPORT_MT7668 */

static void btmtk_usb_cap_init(void)
{
#if SUPPORT_MT7662
    if (g_data->chip_id == 0) {
        btmtk_usb_switch_iobase_7662(WLAN);
        btmtk_usb_io_read32_7662(0x00, &g_data->chip_id);
    }
#endif

#if SUPPORT_MT7668
    if (g_data->chip_id == 0)
        btmtk_usb_io_read32_7668(0x80021008, &g_data->chip_id);
#endif

    BTUSB_INFO("%s: chip id = %x", __func__, g_data->chip_id);

#if SUPPORT_MT7662
    if (is_mt7662T(g_data)) {
        BTUSB_INFO("%s: This is MT7662T chip", __func__);
        if (g_data->rom_patch_bin_file_name) {
            memset(g_data->rom_patch_bin_file_name, 0, 32);
            btmtk_usb_osal_memcpy(g_data->rom_patch_bin_file_name,
                          "mt7662t_patch_e1_hdr.bin", 24);
        }
        g_data->rom_patch_offset = 0xBC000;
        g_data->rom_patch_len = 0;
        return;
    } else if (is_mt7662(g_data)) {
        BTUSB_INFO("%s: This is MT7662U chip", __func__);
        if (g_data->rom_patch_bin_file_name) {
            memset(g_data->rom_patch_bin_file_name, 0, 32);
            btmtk_usb_osal_memcpy(g_data->rom_patch_bin_file_name,
                          "mt7662_patch_e3_hdr.bin", 23);
        }
        g_data->rom_patch_offset = 0x90000;
        g_data->rom_patch_len = 0;
        return;
    }
#endif /* SUPPORT_MT7662 */

#if SUPPORT_MT7668
    if (is_mt7668(g_data)) {
        BTUSB_INFO("%s: This is MT7668 chip", __func__);
        if (g_data->rom_patch_bin_file_name) {
            memset(g_data->rom_patch_bin_file_name, 0, 32);
            memcpy(g_data->rom_patch_bin_file_name,
                   "mt7668_patch_e1_hdr.bin", 23);
        }
        g_data->rom_patch_offset = 0x2000000;
        g_data->rom_patch_len = 0;
        return;
    }
#endif /* SUPPORT_MT7668 */

    BTUSB_WARN("Unknown chip(%x)", g_data->chip_id);

}

static int btmtk_usb_load_rom_patch(void)
{
    int err = -1;

    if (g_data == NULL)
        BTUSB_ERR("%s: g_data is NULL !", __func__);
#if SUPPORT_MT7662
    if (is_mt7662(g_data))
        return btmtk_usb_load_rom_patch_7662();
#endif

#if SUPPORT_MT7668
    if (is_mt7668(g_data))
        return btmtk_usb_load_rom_patch_7668();
#endif

    BTUSB_WARN("%s: unknown chip id (%d)", __func__, g_data->chip_id);
    return err;
}

/*============================================================================*/
/* Internal Functions : Send HCI/WMT */
/*============================================================================*/
#define ______________________________________Internal_Functions_Send_HCI_WMT
static int btmtk_usb_send_woble_suspend_cmd(void)
{
    int ret = -1;                       /* if successful, 0 */
#if defined(BT_RC_VENDOR_T1)
    do {
        /* yellow: gpio5, blue: gpio19 */
        u8 wmt[] = { 0x6F, 0xFC, 0x38,  /* HCI header */
            0x01, 0x18, 0x34, 0x00, 0x00, 0x01, 0x00, 0x00,
            0x05, 0x01, 0x00, 0x00, 0x01, 0x80, 0x64, 0x00,
            0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
            0x01, 0x80, 0x64, 0x00, 0x01, 0x80, 0x00, 0x00,
            0x13, 0x01, 0x00, 0x00, 0x01, 0x80, 0x01, 0x80,
            0x01, 0x80, 0x01, 0x80, 0x64, 0x00, 0x01, 0x80,
            0x01, 0x80, 0x64, 0x00, 0x01, 0x80, 0x00, 0x00 };
        u8 hci[] = { 0xC9, 0xFC, 0x02, 0x01, 0x0C };

        BTUSB_INFO("%s: BT_RC_VENDOR_T1", __func__);
        ret = btmtk_usb_send_wmt_cmd(wmt, sizeof(wmt), NULL, -1, 0);
        if (ret < 0) {
            BTUSB_ERR("%s: Send WMT failed(%d)", __func__, ret);
            return ret;
        }
        BTUSB_INFO("%s: sent WMT OK", __func__);

        ret = btmtk_usb_send_hci_cmd(hci, sizeof(hci), NULL, -1);
    } while (0);

#elif defined(BT_RC_VENDOR_N0)
    do {
        u8 cmd[] = { 0xC9, 0xFC, 0x02, 0x01, 0x0A };

        BTUSB_INFO("%s: BT_RC_VENDOR_N0", __func__);
        ret = btmtk_usb_send_hci_cmd(cmd, sizeof(cmd), NULL, -1);
   } while (0);

#elif defined(BT_RC_VENDOR_S0)
    do {
        u8 cmd[] = { 0xC9, 0xFC, 0x02, 0x01, 0x0B };

        BTUSB_INFO("%s: BT_RC_VENDOR_S0", __func__);
        ret = btmtk_usb_send_hci_cmd(cmd, sizeof(cmd), NULL, -1);
    } while (0);

#else /* BT_RC_VENDOR_T0 */
    do {
        u8 cmd[] = { 0xC9, 0xFC, 0x0D, 0x01, 0x0E, 0x00, 0x05, 0x43,
            0x52, 0x4B, 0x54, 0x4D, 0x20, 0x04, 0x32, 0x00 };

        BTUSB_INFO("%s: BT_RC_VENDOR_T0 or Default", __func__);
        ret = btmtk_usb_send_hci_cmd(cmd, sizeof(cmd), NULL, -1);
    } while (0);
#endif

    if (ret < 0) {
        BTUSB_ERR("%s: failed(%d)", __func__, ret);
    } else {
        BTUSB_INFO("%s: OK", __func__);
        ret = 0;
    }

    return ret;
}

static int btmtk_usb_send_wmt_reset_cmd(void)
{
    u8 cmd[] = { 0x6F, 0xFC, 0x05, 0x01, 0x07, 0x01, 0x00, 0x04 };
    u8 event[] = { 0xE4, 0x05, 0x02, 0x07, 0x01, 0x00, 0x00 };
    int ret = -1;                   /* if successful, 0 */

    BTUSB_DBG_RAW(cmd, sizeof(cmd), "%s: Send CMD:", __func__);
    ret = btmtk_usb_send_wmt_cmd(cmd, sizeof(cmd), event, sizeof(event), 20);
    if (ret < 0) {
        BTUSB_ERR("Check reset wmt result: NG");
    } else {
        BTUSB_INFO("Check reset wmt result: OK");
        ret = 0;
    }

    return ret;
}

static int btmtk_usb_get_rom_patch_result(void)
{
    u8 event[] = { 0xE4, 0x05, 0x02, 0x01, 0x01, 0x00, 0x00 };
    int ret = -1;                       /* if successful, 0 */

    if (g_data == NULL) {
        BTUSB_ERR("%s: g_data == NULL!", __func__);
        return -1;
    }
    if (g_data->udev == NULL) {
        BTUSB_ERR("%s: g_data->udev == NULL!", __func__);
        return -1;
    }
    if (g_data->io_buf == NULL) {
        BTUSB_ERR("%s: g_data->io_buf == NULL!", __func__);
        return -1;
    }

    memset(g_data->io_buf, 0, USB_IO_BUF_SIZE);
    ret =
        usb_control_msg(g_data->udev, usb_rcvctrlpipe(g_data->udev, 0),
                0x01, DEVICE_VENDOR_REQUEST_IN, 0x30, 0x00,
                g_data->io_buf, USB_IO_BUF_SIZE,
                USB_CTRL_IO_TIMO);

    if (ret < 0)
        BTUSB_ERR("%s: error(%d)", __func__, ret);

    /* ret should be 16 bytes */
    if (ret >= sizeof(event) && !memcmp(g_data->io_buf, event, sizeof(event))) {
        BTUSB_INFO("Get rom patch result: OK");
        return 0;
    } else {
        BTUSB_WARN("Get rom patch result: NG");
        return -1;
    }

    return ret;
}

static int btmtk_usb_send_hci_reset_cmd(void)
{
    u8 cmd[] = { 0x03, 0x0C };
    u8 event[] = { 0x0E, 0x04, 0x01, 0x03, 0x0C, 0x00 };
    int ret = -1;                           /* if successful, 0 */

    ret = btmtk_usb_send_hci_cmd(cmd, sizeof(cmd), event, sizeof(event));
    if (ret < 0) {
        BTUSB_ERR("%s: failed(%d)", __func__, ret);
    } else {
        BTUSB_INFO("%s: OK", __func__);
        ret = 0;
    }

    return ret;
}

static int btmtk_usb_send_assert_cmd_ctrl(void)
{
    u8 cmd[] = { 0x6F, 0xFC, 0x05, 0x01, 0x02, 0x01, 0x00, 0x08 };
    int ret = -1;                           /* if successful, 0 */

    BTUSB_DBG_RAW(cmd, sizeof(cmd), "%s: Send CMD:", __func__);
    /* Ask DO NOT read event */
    ret = btmtk_usb_send_hci_cmd(cmd, sizeof(cmd), NULL, -1);
    if (ret < 0) {
        BTUSB_ERR("%s: error(%d)", __func__, ret);
    } else {
        BTUSB_INFO("%s: OK", __func__);
        ret = 0;
    }

    return ret;
}

static int btmtk_usb_send_assert_cmd_bulk(void)
{
    int ret = 0;
    int actual_length = 9;
    u8 buf[] = { 0x6F, 0xFC, 0x05, 0x00, 0x01, 0x02, 0x01, 0x00, 0x08 };

    BTUSB_DBG_RAW(buf, sizeof(buf), "%s: Send CMD:", __func__);
    ret = usb_bulk_msg(g_data->udev, usb_sndbulkpipe(g_data->udev, 2), buf,
             sizeof(buf), &actual_length, 100);

    if (ret < 0) {
        BTUSB_ERR("%s: error(%d)", __func__, ret);
        return ret;
    }
    BTUSB_INFO("%s: OK", __func__);
    return 0;
}

/*============================================================================*/
/* Callback Functions */
/*============================================================================*/
#define ___________________________________________________Callback_Functions
static void btmtk_usb_early_suspend(void)
{
    if (g_data == NULL) {
        BTUSB_ERR("%s: ERROR, g_data is NULL!", __func__);
        return;
    }

    BTUSB_INFO("%s: begin", __func__);
    BTUSB_WARN("%s: skip this state", __func__);
    /* btmtk_usb_set_state(BTMTK_USB_STATE_EARLY_SUSPEND); */
    BTUSB_INFO("%s: end", __func__);
}

static void btmtk_usb_late_resume(void)
{
    int state = BTMTK_USB_STATE_UNKNOWN;

    BTUSB_INFO("%s: begin", __func__);
    if (g_data == NULL) {
        BTUSB_ERR("%s: ERROR, g_data is NULL!", __func__);
        return;
    }

    mutex_lock(&btmtk_usb_state_mutex);
    state = btmtk_usb_get_state();
    if (state == BTMTK_USB_STATE_EARLY_SUSPEND || state == BTMTK_USB_STATE_WORKING) {
        BTUSB_WARN
            ("%s: invoked immediately after early suspend, ignore.",
             __func__);
        btmtk_usb_set_state(BTMTK_USB_STATE_WORKING);
        BTUSB_INFO("%s: end", __func__);
        mutex_unlock(&btmtk_usb_state_mutex);
        return;
    } else if (state == BTMTK_USB_STATE_DISCONNECT || state == BTMTK_USB_STATE_SUSPEND_DISCONNECT
           || state == BTMTK_USB_STATE_RESUME_DISCONNECT) {
        BTUSB_WARN
            ("%s: previous state is error disconnect(%d), ignore and set to disconnect state.",
             __func__, state);
        btmtk_usb_set_state(BTMTK_USB_STATE_DISCONNECT);
        BTUSB_INFO("%s: end", __func__);
        mutex_unlock(&btmtk_usb_state_mutex);
        return;
    } else if (state == BTMTK_USB_STATE_FW_DUMP || state == BTMTK_USB_STATE_RESUME_FW_DUMP) {
        BTUSB_WARN
            ("%s: previous state is error fw dump(%d), ignore and set to fw dump state.",
             __func__, state);
        btmtk_usb_set_state(BTMTK_USB_STATE_FW_DUMP);
        BTUSB_INFO("%s: end", __func__);
        mutex_unlock(&btmtk_usb_state_mutex);
        return;
    } else if (state == BTMTK_USB_STATE_SUSPEND_PROBE || state == BTMTK_USB_STATE_RESUME_PROBE) {
        BTUSB_WARN
            ("%s: previous state is error probe(%d), ignore and set to probe state.",
             __func__, state);
        btmtk_usb_set_state(BTMTK_USB_STATE_PROBE);
        BTUSB_INFO("%s: end", __func__);
        mutex_unlock(&btmtk_usb_state_mutex);
        return;
    } else if (state != BTMTK_USB_STATE_RESUME && state != BTMTK_USB_STATE_SUSPEND) {
        BTUSB_WARN
            ("%s: previous state is not suspend/resume(%d), ignore.",
             __func__, state);
        BTUSB_INFO("%s: end", __func__);
        mutex_unlock(&btmtk_usb_state_mutex);
        return;
    }
    btmtk_usb_set_state(BTMTK_USB_STATE_LATE_RESUME);
    mutex_unlock(&btmtk_usb_state_mutex);

    btmtk_usb_handle_resume();
    BTUSB_INFO("%s: end", __func__);
}

static void btmtk_usb_load_rom_patch_complete(struct urb *urb)
{
    struct completion *sent_to_mcu_done = (struct completion *)urb->context;

    complete(sent_to_mcu_done);
}

static void btmtk_usb_intr_complete(struct urb *urb)
{
    u8 *event_buf;
    u32 roomLeft, last_len, length;
    int err;

    if (g_data == NULL) {
        BTUSB_ERR("%s: ERROR, g_data is NULL!", __func__);
        return;
    }

    if (urb->status != 0)
        BTUSB_WARN("%s: urb %p urb->status %d count %d", __func__,
              urb, urb->status, urb->actual_length);

    if (urb->status == 0 && urb->actual_length != 0) {
        event_buf = urb->transfer_buffer;

        btmtk_usb_hci_snoop_save_event(urb->actual_length,
                           urb->transfer_buffer);

        if (urb->actual_length >= 3) {
            if ((urb->actual_length == 2 + event_buf[1]) && event_buf[0] == 0xFF && event_buf[2] == 0x50) {
                btmtk_fifo_in(FIFO_SYSLOG, g_data->bt_fifo, (const void *)event_buf, urb->actual_length);
                goto intr_resub;
            }
        }

        /* In this condition, need to add header */
        if (leftHciEventSize == 0)
            length = urb->actual_length + 1;
        else
            length = urb->actual_length;

        btmtk_usb_lock_unsleepable_lock(&(g_data->metabuffer->spin_lock));

        /* roomleft means the usable space */
        if (g_data->metabuffer->read_p <= g_data->metabuffer->write_p) {
            roomLeft = META_BUFFER_SIZE - g_data->metabuffer->write_p
                + g_data->metabuffer->read_p - 1;
        } else {
            roomLeft = g_data->metabuffer->read_p - g_data->metabuffer->write_p - 1;
        }

        /* no enough space to store the received data */
        if (roomLeft < length)
            BTUSB_WARN("%s: Queue is full !!", __func__);

        if (length + g_data->metabuffer->write_p < META_BUFFER_SIZE) {
            /* only one interrupt event, not be splited */
            if (leftHciEventSize == 0) {
                /* copy event header: 0x04 */
                g_data->metabuffer->buffer[g_data->metabuffer->write_p] = 0x04;
                g_data->metabuffer->write_p += 1;
            }
            /* copy event data */
            btmtk_usb_osal_memcpy(g_data->metabuffer->buffer + g_data->metabuffer->write_p,
                    event_buf, urb->actual_length);
            g_data->metabuffer->write_p += urb->actual_length;
        } else {
            BTUSB_DBG("%s: back to meta buffer head", __func__);
            last_len = META_BUFFER_SIZE - g_data->metabuffer->write_p;

            /* only one interrupt event, not be splited */
            if (leftHciEventSize == 0) {
                if (last_len != 0) {
                    /* copy evnet header: 0x04 */
                    g_data->metabuffer->buffer[g_data->metabuffer->write_p] = 0x04;
                    g_data->metabuffer->write_p += 1;
                    last_len--;
                    /* copy event data */
                    btmtk_usb_osal_memcpy(g_data->metabuffer->buffer +
                         g_data->metabuffer->write_p, event_buf, last_len);
                    btmtk_usb_osal_memcpy(g_data->metabuffer->buffer,
                         event_buf + last_len, urb->actual_length - last_len);
                    g_data->metabuffer->write_p = urb->actual_length - last_len;
                } else {
                    g_data->metabuffer->buffer[0] = 0x04;
                    g_data->metabuffer->write_p = 1;
                    /* copy event data */
                    btmtk_usb_osal_memcpy
                        (g_data->metabuffer->buffer + g_data->metabuffer->write_p,
                         event_buf, urb->actual_length);
                    g_data->metabuffer->write_p += urb->actual_length;
                }
            /* leftHciEventSize != 0 */
            } else {
                /* copy event data */
                btmtk_usb_osal_memcpy(g_data->metabuffer->buffer
                        + g_data->metabuffer->write_p, event_buf,
                        last_len);
                btmtk_usb_osal_memcpy(g_data->metabuffer->buffer,
                        event_buf + last_len, urb->actual_length - last_len);
                g_data->metabuffer->write_p = urb->actual_length - last_len;
            }
        }

        btmtk_usb_unlock_unsleepable_lock(&(g_data->metabuffer->spin_lock));

        /* means there is some data in next event */
        if (leftHciEventSize == 0
            && event_buf[1] >= (HCI_MAX_EVENT_SIZE - 1)) {
            leftHciEventSize = event_buf[1] + 2 - HCI_MAX_EVENT_SIZE;
            BTUSB_DBG("there is left event, size: %d",
                  leftHciEventSize);
        } else if (leftHciEventSize != 0) {
            leftHciEventSize -= urb->actual_length;

            /* error handling. Length wrong, drop some bytes to recovery counter!! */
            if (leftHciEventSize < 0) {
                BTUSB_WARN
                    ("%s: *size wrong(%d), this event may be wrong!!",
                     __func__, leftHciEventSize);
                leftHciEventSize = 0;    /* reset count */
            }

            if (leftHciEventSize == 0) {
                if (btmtk_usb_get_state() ==
                    BTMTK_USB_STATE_WORKING) {
                    wake_up(&BT_wq);
                    wake_up_interruptible(&inq);
                } else {
                    BTUSB_WARN
                        ("%s: current is in suspend/resume (%d), Don't wake-up wait queue",
                         __func__, btmtk_usb_get_state());
                }
            }
        } else if (leftHciEventSize == 0
               && event_buf[1] < (HCI_MAX_EVENT_SIZE - 1)) {
            if (btmtk_usb_get_state() == BTMTK_USB_STATE_WORKING) {
                wake_up(&BT_wq);
                wake_up_interruptible(&inq);
            } else {
                BTUSB_WARN
                    ("%s: current is in suspend/resume (%d), Don't wake-up wait queue",
                     __func__, btmtk_usb_get_state());
            }
        }
    } else {
        BTUSB_INFO("%s: urb->status:%d, urb->length:%d", __func__,
               urb->status, urb->actual_length);
    }
intr_resub:
    usb_mark_last_busy(g_data->udev);
    usb_anchor_urb(urb, &g_data->intr_anchor);

    err = usb_submit_urb(urb, GFP_ATOMIC);
    if (err < 0) {
        /* -EPERM: urb is being killed;
         * -ENODEV: device got disconnected */
        if (err != -EPERM && err != -ENODEV)
            BTUSB_ERR("%s: urb %p failed to resubmit intr_in_urb(%d)",
                 __func__, urb, -err);
        usb_unanchor_urb(urb);
    }
}

static void btmtk_usb_bulk_in_complete(struct urb *urb)
{
    /* actual_length: the ACL data size (doesn't contain header) */
    u32 roomLeft, last_len, length, index, actual_length;
    u8 *event_buf;
    int state = btmtk_usb_get_state();
    int err;
    u8 *buf;
    u16 len;

    if (g_data == NULL) {
        BTUSB_ERR("%s: ERROR, g_data is NULL!", __func__);
        return;
    }

    roomLeft = 0;
    last_len = 0;
    length = 0;
    index = 0;
    actual_length = 0;

    if (urb->status != 0)
        BTUSB_INFO("%s: urb %p urb->status %d count %d", __func__,
               urb, urb->status, urb->actual_length);

    /* Handle FW Dump Data */
    buf = urb->transfer_buffer;
    len = 0;
    if (urb->actual_length > 4) {
        len = buf[2] + ((buf[3] << 8) & 0xff00);
        if (buf[0] == 0x6f && buf[1] == 0xfc && len + 4 == urb->actual_length) {
            if (state != BTMTK_USB_STATE_FW_DUMP && state != BTMTK_USB_STATE_RESUME_FW_DUMP) {
                /* This is the first BULK_IN packet of FW dump. */
                if (state == BTMTK_USB_STATE_RESUME)
                    btmtk_usb_set_state(BTMTK_USB_STATE_RESUME_FW_DUMP);
                else
                    btmtk_usb_set_state(BTMTK_USB_STATE_FW_DUMP);
                btmtk_usb_hci_snoop_print_to_log();
            }

            btmtk_fifo_in(FIFO_COREDUMP, g_data->bt_fifo, (const void *)&buf[4], len);
            /* set to 0xff to avoide BlueDroid dropping this ACL packet. */
            buf[0] = 0xff;
            buf[1] = 0xff;
        }
    }

    if (urb->status == 0) {
        event_buf = urb->transfer_buffer;
        len = buf[2] + ((buf[3] << 8) & 0xff00);

        btmtk_usb_hci_snoop_save_acl(urb->actual_length,
                         urb->transfer_buffer);

        if (urb->actual_length > 4 && event_buf[0] == 0x6f
            && event_buf[1] == 0xfc && len + 4 == urb->actual_length) {
            BTUSB_DBG("Coredump message");
        } else {
            length = urb->actual_length + 1;

            actual_length =
                1 * (event_buf[2] & 0x0f) +
                16 * ((event_buf[2] & 0xf0) >> 4)
                + 256 * ((event_buf[3] & 0x0f)) +
                4096 * ((event_buf[3] & 0xf0) >> 4);

            btmtk_usb_lock_unsleepable_lock(&
                            (g_data->
                             metabuffer->spin_lock));

            /* roomleft means the usable space */
            if (g_data->metabuffer->read_p <=
                g_data->metabuffer->write_p)
                roomLeft =
                    META_BUFFER_SIZE -
                    g_data->metabuffer->write_p +
                    g_data->metabuffer->read_p - 1;
            else
                roomLeft =
                    g_data->metabuffer->read_p -
                    g_data->metabuffer->write_p - 1;

            /* no enough space to store the received data */
            if (roomLeft < length)
                BTUSB_WARN("%s: Queue is full !!", __func__);

            if (length + g_data->metabuffer->write_p <
                META_BUFFER_SIZE) {

                if (leftACLSize == 0) {
                    /* copy ACL data header: 0x02 */
                    g_data->metabuffer->
                        buffer[g_data->metabuffer->write_p]
                        = 0x02;
                    g_data->metabuffer->write_p += 1;
                }

                /* copy event data */
                btmtk_usb_osal_memcpy(g_data->
                              metabuffer->buffer +
                              g_data->
                              metabuffer->write_p,
                              event_buf,
                              urb->actual_length);
                g_data->metabuffer->write_p +=
                    urb->actual_length;
            } else {
                last_len =
                    META_BUFFER_SIZE -
                    g_data->metabuffer->write_p;
                if (leftACLSize == 0) {
                    if (last_len != 0) {
                        /* copy ACL data header: 0x02 */
                        g_data->
                            metabuffer->buffer[g_data->
                                       metabuffer->write_p]
                            = 0x02;
                        g_data->metabuffer->write_p +=
                            1;
                        last_len--;
                        /* copy event data */
                        btmtk_usb_osal_memcpy
                            (g_data->metabuffer->buffer
                             +
                             g_data->metabuffer->write_p,
                             event_buf, last_len);
                        btmtk_usb_osal_memcpy
                            (g_data->metabuffer->buffer,
                             event_buf + last_len,
                             urb->actual_length -
                             last_len);
                        g_data->metabuffer->write_p =
                            urb->actual_length -
                            last_len;
                    } else {
                        g_data->metabuffer->buffer[0] =
                            0x02;
                        g_data->metabuffer->write_p = 1;
                        /* copy event data */
                        btmtk_usb_osal_memcpy
                            (g_data->metabuffer->buffer
                             +
                             g_data->metabuffer->write_p,
                             event_buf,
                             urb->actual_length);
                        g_data->metabuffer->write_p +=
                            urb->actual_length;
                    }
                } else {    /* leftACLSize !=0 */

                    /* copy event data */
                    btmtk_usb_osal_memcpy
                        (g_data->metabuffer->buffer +
                         g_data->metabuffer->write_p,
                         event_buf, last_len);
                    btmtk_usb_osal_memcpy
                        (g_data->metabuffer->buffer,
                         event_buf + last_len,
                         urb->actual_length - last_len);
                    g_data->metabuffer->write_p =
                        urb->actual_length - last_len;
                }
            }

            btmtk_usb_unlock_unsleepable_lock(&
                              (g_data->
                               metabuffer->spin_lock));

            /* the maximize bulk in ACL data packet size is 512 (4byte header + 508 byte data) */
            /* maximum receieved data size of one packet is 1025 (4byte header + 1021 byte data) */
            if (leftACLSize == 0 && actual_length > 1017) {
                /* the data in next interrupt event */
                leftACLSize =
                    actual_length + 4 - urb->actual_length;
                BTUSB_DBG("there is left ACL event, size: %d",
                      leftACLSize);
            } else if (leftACLSize > 0) {
                leftACLSize -= urb->actual_length;

                /* error handling. Length wrong, drop some bytes to recovery counter!! */
                if (leftACLSize < 0) {
                    BTUSB_WARN
                        ("* size wrong(%d), this acl data may be wrong!!",
                         leftACLSize);
                    leftACLSize = 0;    /* reset count */
                }

                if (leftACLSize == 0) {
                    if (state == BTMTK_USB_STATE_WORKING || state == BTMTK_USB_STATE_FW_DUMP ||
                        state == BTMTK_USB_STATE_RESUME_FW_DUMP) {
                        wake_up(&BT_wq);
                        wake_up_interruptible(&inq);
                    } else {
                        BTUSB_DBG
                            ("%s: current is in suspend/resume (%d), Don't wake-up wait queue",
                             __func__,
                             state);
                    }
                }
            } else if (leftACLSize == 0 && actual_length <= 1017) {
                if (state == BTMTK_USB_STATE_WORKING || state == BTMTK_USB_STATE_FW_DUMP ||
                    state == BTMTK_USB_STATE_RESUME_FW_DUMP) {
                    wake_up(&BT_wq);
                    wake_up_interruptible(&inq);
                } else {
                    BTUSB_DBG
                        ("%s: current is in suspend/resume (%d), Don't wake-up wait queue",
                         __func__, state);
                }
            } else {
                BTUSB_WARN
                    ("ACL data count fail, leftACLSize:%d",
                     leftACLSize);
            }
        }

    } else {
        BTUSB_INFO("%s: urb->status:%d", __func__, urb->status);
    }

    usb_anchor_urb(urb, &g_data->bulk_anchor);
    usb_mark_last_busy(g_data->udev);

    err = usb_submit_urb(urb, GFP_ATOMIC);

    if (err != 0) {
        /* -EPERM: urb is being killed;
         * -ENODEV: device got disconnected */
        if (err != -EPERM && err != -ENODEV)
            BTUSB_ERR
                ("%s: urb %p failed to resubmit bulk_in_urb(%d)",
                 __func__, urb, -err);
        usb_unanchor_urb(urb);
    }
}

static void btmtk_usb_tx_complete_meta(struct urb *urb)
{
    if (g_data == NULL) {
        BTUSB_ERR("%s: ERROR, g_data is NULL!", __func__);
        return;
    }

    g_data->meta_tx = 0;
    kfree(urb->setup_packet);
}

static int btmtk_usb_standby(void)
{
    if (g_data == NULL) {
        BTUSB_ERR("%s: ERROR, g_data is NULL!", __func__);
        return -ENODEV;
    }

    BTUSB_INFO("%s: begin", __func__);

    btmtk_usb_handle_entering_WoBLE_state();

    BTUSB_INFO("%s: end", __func__);
    BTUSB_WARN("%s: Add 500ms delay", __func__);
    msleep(500); /* Add 500ms delay to avoid log lost. */
    return 0;
}

void btmtk_usb_trigger_core_dump(void)
{
    int state = BTMTK_USB_STATE_UNKNOWN;

    BTUSB_WARN("%s: Invoked by other module (WiFi).", __func__);

    mutex_lock(&btmtk_usb_state_mutex);
    state = btmtk_usb_get_state();
    if (state == BTMTK_USB_STATE_FW_DUMP || state == BTMTK_USB_STATE_RESUME_FW_DUMP) {
        BTUSB_WARN("%s: current in dump state, skip.", __func__);
        mutex_unlock(&btmtk_usb_state_mutex);
        return;
    }
    mutex_unlock(&btmtk_usb_state_mutex);

    btmtk_usb_toggle_rst_pin();
}
EXPORT_SYMBOL(btmtk_usb_trigger_core_dump);

/*============================================================================*/
/* Interface Functions : BT Stack */
/*============================================================================*/
#define ______________________________________Interface_Function_for_BT_Stack
static ssize_t btmtk_usb_fops_write(struct file *file, const char __user *buf,
                    size_t count, loff_t *f_pos)
{
    int retval = 0;
    int state = BTMTK_USB_STATE_UNKNOWN;

    if (g_data == NULL) {
        BTUSB_ERR("%s: ERROR, g_data is NULL!", __func__);
        return -ENODEV;
    }

    mutex_lock(&btmtk_usb_state_mutex);
    state = btmtk_usb_get_state();
    if (state != BTMTK_USB_STATE_WORKING) {
        BTUSB_ERR("%s: current is in suspend/resume (%d).", __func__,
              state);
        mutex_unlock(&btmtk_usb_state_mutex);
        /* avoid platform in retry loop when controller doesn't exist*/
         if (state == BTMTK_USB_STATE_DISCONNECT)
         	return -EIO;
        msleep(3000);
        return -EAGAIN;
    }
    mutex_unlock(&btmtk_usb_state_mutex);

    /* semaphore mechanism, the waited process will sleep */
    down(&g_data->wr_mtx);

    if (count > 0) {
        u32 copy_size = (count < BUFFER_SIZE) ? count : BUFFER_SIZE;

        if (copy_from_user(&g_data->o_buf[0], &buf[0], copy_size)) {
            retval = -EFAULT;
            BTUSB_ERR("%s: copy data from user fail", __func__);
            goto OUT;
        }
        BTUSB_DBG_RAW(buf, 16, "%s: (len=%2d)", __func__, copy_size);

        /* command */
        if (g_data->o_buf[0] == 0x01) {
            if (copy_size == 9 &&
                g_data->o_buf[0] == 0x01 &&
                g_data->o_buf[1] == 0x6f &&
                g_data->o_buf[2] == 0xfc &&
                g_data->o_buf[3] == 0x05 &&
                g_data->o_buf[4] == 0x01 &&
                g_data->o_buf[5] == 0x02 &&
                g_data->o_buf[6] == 0x01 &&
                g_data->o_buf[7] == 0x00 &&
                g_data->o_buf[8] == 0x08) {
                BTUSB_INFO
                    ("%s: Donge FW Assert Triggered by BT Stack!",
                     __func__);
                btmtk_usb_hci_snoop_print_to_log();
            } else if (copy_size == 4 &&
                   g_data->o_buf[0] == 0x01 &&
                   g_data->o_buf[1] == 0x03 &&
                   g_data->o_buf[2] == 0x0c &&
                   g_data->o_buf[3] == 0x00) {
                BTUSB_INFO
                    ("%s: got command: 0x03 0c 00 (HCI_RESET)",
                     __func__);
            } else if (copy_size == 4 && g_data->o_buf[0] == 0x01
                   && g_data->o_buf[1] == 0x01
                   && g_data->o_buf[2] == 0x10
                   && g_data->o_buf[3] == 0x00) {
                BTUSB_INFO
                    ("%s: got command: 0x01 10 00 (READ_LOCAL_VERSION)",
                     __func__);
            }

            btmtk_usb_hci_snoop_save_cmd(copy_size, &g_data->o_buf[0]);
            retval = btmtk_usb_meta_send_data(&g_data->o_buf[0], copy_size);
        } else if (g_data->o_buf[0] == 0x02) {
            retval = btmtk_usb_send_data(&g_data->o_buf[0], copy_size);
        } else if (g_data->o_buf[0] == 0x03) {
            retval = btmtk_usb_send_data(&g_data->o_buf[0], copy_size);
        } else {
            BTUSB_WARN("%s: this is unknown bt data:0x%02x",
                   __func__, g_data->o_buf[0]);
        }
    } else {
        retval = -EFAULT;
        BTUSB_ERR
            ("%s: target packet length:%zu is not allowed, retval = %d",
             __func__, count, retval);
    }

OUT:
    up(&g_data->wr_mtx);
    return retval;
}

static ssize_t btmtk_usb_fops_read(struct file *file, char __user *buf,
                   size_t count, loff_t *f_pos)
{
    int retval = 0;
    int copyLen = 0;
    int state = BTMTK_USB_STATE_UNKNOWN;
    unsigned short tailLen = 0;
    u8 *buffer = NULL;

    if (g_data == NULL) {
        BTUSB_ERR("%s: ERROR, g_data is NULL!", __func__);
        return -ENODEV;
    }
    buffer = g_data->i_buf;

    down(&g_data->rd_mtx);

    if (count > BUFFER_SIZE) {
        count = BUFFER_SIZE;
        BTUSB_WARN("read size is bigger than 1024");
    }

    mutex_lock(&btmtk_usb_state_mutex);
    state = btmtk_usb_get_state();
    if (state == BTMTK_USB_STATE_FW_DUMP || state == BTMTK_USB_STATE_RESUME_FW_DUMP) {
        /* do nothing */

    } else if (state != BTMTK_USB_STATE_WORKING) {
        BTUSB_ERR("%s: current is in suspend/resume (%d).", __func__, state);
        mutex_unlock(&btmtk_usb_state_mutex);
        return -EAGAIN;

#if !FASYNC_BT_RECOVER
    } else if (need_reset_stack == 1) {
        BTUSB_WARN("%s: Reset BT stack", __func__);
        mutex_unlock(&btmtk_usb_state_mutex);
        if (copy_to_user(buf, reset_event, sizeof(reset_event))) {
            BTUSB_ERR("%s: Copy reset event to stack fail", __func__);
            up(&g_data->rd_mtx);
            return -EFAULT;
        }
        need_reset_stack = 0;
        up(&g_data->rd_mtx);
        return sizeof(reset_event);
#endif /* !FASYNC_BT_RECOVER */
    }
    mutex_unlock(&btmtk_usb_state_mutex);

    btmtk_usb_lock_unsleepable_lock(&(g_data->metabuffer->spin_lock));

    /* means the buffer is empty */
    while (g_data->metabuffer->read_p == g_data->metabuffer->write_p) {

        /*  unlock the buffer to let other process write data to buffer */
        btmtk_usb_unlock_unsleepable_lock(&
                          (g_data->
                           metabuffer->spin_lock));

        /* If nonblocking mode, return directly O_NONBLOCK is specified during open() */
        if (file->f_flags & O_NONBLOCK) {
            /* BTUSB_DBG("Non-blocking btmtk_usb_fops_read()"); */
            retval = -EAGAIN;
            goto OUT;
        }
        wait_event(BT_wq,
               g_data->metabuffer->read_p !=
               g_data->metabuffer->write_p);
        btmtk_usb_lock_unsleepable_lock(&
                        (g_data->
                         metabuffer->spin_lock));
    }

    while (g_data->metabuffer->read_p != g_data->metabuffer->write_p) {
        if (g_data->metabuffer->write_p > g_data->metabuffer->read_p) {
            copyLen =
                g_data->metabuffer->write_p -
                g_data->metabuffer->read_p;
            if (copyLen > count)
                copyLen = count;
            btmtk_usb_osal_memcpy(g_data->i_buf,
                          g_data->metabuffer->buffer +
                          g_data->metabuffer->read_p,
                          copyLen);
            g_data->metabuffer->read_p += copyLen;
            break;
        } else {
            tailLen = META_BUFFER_SIZE - g_data->metabuffer->read_p;
            if (tailLen > count) {    /* exclude equal case to skip wrap check */
                copyLen = count;
                btmtk_usb_osal_memcpy(g_data->i_buf,
                              g_data->
                              metabuffer->buffer +
                              g_data->
                              metabuffer->read_p,
                              copyLen);
                g_data->metabuffer->read_p += copyLen;
            } else {
                /* part 1: copy tailLen */
                btmtk_usb_osal_memcpy(g_data->i_buf,
                              g_data->
                              metabuffer->buffer +
                              g_data->
                              metabuffer->read_p,
                              tailLen);

                buffer += tailLen;    /* update buffer offset */

                /* part 2: check if head length is enough */
                copyLen = count - tailLen;

                /* if write_p < copyLen: means we can copy all data until write_p;
                            else: we can only copy data for copyLen */
                copyLen =
                    (g_data->metabuffer->write_p <
                     copyLen) ? g_data->
                    metabuffer->write_p : copyLen;

                /* if copylen not 0, copy data to buffer */
                if (copyLen) {
                    btmtk_usb_osal_memcpy(buffer,
                                  g_data->metabuffer->buffer
                                  + 0, copyLen);
                }
                /* Update read_p final position */
                g_data->metabuffer->read_p = copyLen;

                /* update return length: head + tail */
                copyLen += tailLen;
            }
            break;
        }
    }

    btmtk_usb_unlock_unsleepable_lock(&(g_data->metabuffer->spin_lock));

    BTUSB_DBG_RAW(g_data->i_buf, 16, "%s:  (len=%2d)", __func__, copyLen);
    if (copy_to_user(buf, g_data->i_buf, copyLen)) {
        BTUSB_ERR("copy to user fail");
        copyLen = -EFAULT;
        goto OUT;
    }
OUT:
    up(&g_data->rd_mtx);
    return copyLen;
}

static int btmtk_usb_fops_open(struct inode *inode, struct file *file)
{
    int state = BTMTK_USB_STATE_UNKNOWN;

    if (g_data == NULL) {
        BTUSB_ERR("%s: ERROR, g_data is NULL!", __func__);
        return -ENODEV;
    }

    mutex_lock(&btmtk_usb_state_mutex);
    state = btmtk_usb_get_state();
    if (state == BTMTK_USB_STATE_INIT || state == BTMTK_USB_STATE_DISCONNECT) {
        mutex_unlock(&btmtk_usb_state_mutex);
        return -EAGAIN;
    }
    mutex_unlock(&btmtk_usb_state_mutex);

    BTUSB_INFO("%s: Mediatek Bluetooth USB driver ver %s", __func__,
           VERSION);

    if (is_fops_close_ongoing) {
        BTUSB_WARN("%s: fops close is on-going !", __func__);
        return -ENODEV;
    }

    BTUSB_INFO("%s: major %d minor %d (pid %d), probe counter: %d",
           __func__, imajor(inode), iminor(inode), current->pid,
           probe_counter);

    if (current->pid == 1) {
        BTUSB_WARN("%s: return 0", __func__);
        return 0;
    }

    mutex_lock(&btmtk_usb_state_mutex);
    state = btmtk_usb_get_state();
    if (state != BTMTK_USB_STATE_WORKING) {
        BTUSB_WARN("%s: current is in suspend/resume (%d).", __func__,
               state);
        mutex_unlock(&btmtk_usb_state_mutex);
        return 0;
    }
    mutex_unlock(&btmtk_usb_state_mutex);

    /* init meta buffer */
    spin_lock_init(&(g_data->metabuffer->spin_lock.lock));

    sema_init(&g_data->wr_mtx, 1);
    sema_init(&g_data->rd_mtx, 1);

    /* init wait queue */
    init_waitqueue_head(&(inq));

    /* Init Hci Snoop */
    btmtk_usb_hci_snoop_init();

#if SUPPORT_MT7662
    if (is_mt7662(g_data)) {
        btmtk_usb_send_hci_reset_cmd();
        btmtk_usb_send_hci_low_power_cmd_7662(TRUE);
        btmtk_usb_send_hci_set_ce_cmd_7662();
        btmtk_usb_send_hci_set_tx_power_cmd_7662();
        btmtk_usb_send_hci_radio_on_cmd_7662();
    }
#endif
#if SUPPORT_MT7668
    if (is_mt7668(g_data)) {
        btmtk_usb_send_wmt_power_on_cmd_7668();
        btmtk_usb_send_hci_tci_set_sleep_cmd_7668();
        btmtk_usb_send_hci_reset_cmd();
    }
#endif

    btmtk_usb_lock_unsleepable_lock(&(g_data->metabuffer->spin_lock));
    g_data->metabuffer->read_p = 0;
    g_data->metabuffer->write_p = 0;
    btmtk_usb_unlock_unsleepable_lock(&(g_data->metabuffer->spin_lock));

    BTUSB_INFO("enable interrupt and bulk in urb");
    btmtk_usb_submit_intr_urb();
    btmtk_usb_submit_bulk_in_urb();
    BTUSB_INFO("%s: OK", __func__);

    return 0;
}

static int btmtk_usb_fops_close(struct inode *inode, struct file *file)
{
    int state = BTMTK_USB_STATE_UNKNOWN;

    if (g_data == NULL) {
        BTUSB_ERR("%s: ERROR, g_data is NULL!", __func__);
        return -ENODEV;
    }

    is_fops_close_ongoing = 1;
    BTUSB_INFO("%s: major %d minor %d (pid %d), probe:%d", __func__,
           imajor(inode), iminor(inode), current->pid, probe_counter);

#if FASYNC_BT_RECOVER
    btmtk_usb_fops_fasync(-1, file, 0);
#endif /* FASYNC_BT_RECOVER */

    mutex_lock(&btmtk_usb_state_mutex);
    state = btmtk_usb_get_state();
    if (state != BTMTK_USB_STATE_WORKING) {
        BTUSB_WARN("%s: current is in suspend/resume (%d).", __func__, state);
        is_fops_close_ongoing = 0;
        mutex_unlock(&btmtk_usb_state_mutex);
        return 0;
    }
    mutex_unlock(&btmtk_usb_state_mutex);

    btmtk_usb_stop_traffic();
    btmtk_usb_send_hci_reset_cmd();

#if SUPPORT_MT7662
    if (is_mt7662(g_data))
        btmtk_usb_send_hci_radio_off_cmd_7662();
#endif

#if SUPPORT_MT7668
    if (is_mt7668(g_data))
        btmtk_usb_send_wmt_power_off_cmd_7668();
#endif

    btmtk_usb_lock_unsleepable_lock(&(g_data->metabuffer->spin_lock));
    g_data->metabuffer->read_p = 0;
    g_data->metabuffer->write_p = 0;
    btmtk_usb_unlock_unsleepable_lock(&(g_data->metabuffer->spin_lock));

    is_fops_close_ongoing = 0;

    BTUSB_INFO("%s: OK", __func__);
    return 0;
}

static long btmtk_usb_fops_unlocked_ioctl(struct file *file, unsigned int cmd,
                      unsigned long arg)
{
    long retval = 0;

    if (g_data == NULL) {
        BTUSB_ERR("%s: ERROR, g_data is NULL!", __func__);
        return -ENODEV;
    }

    switch (cmd) {
    case IOCTL_FW_ASSERT:
        /* BT trigger fw assert for debug */
        BTUSB_INFO("BT Set fw assert..., reason:%lu", arg);
        break;
    default:
        retval = -EFAULT;
        BTUSB_WARN("BT_ioctl(): unknown cmd (%d)", cmd);
        break;
    }

    return retval;
}

#if FASYNC_BT_RECOVER
static int btmtk_usb_fops_fasync(int fd, struct file *file, int on)
{
    BTUSB_INFO("%s: fd = 0x%X, flag = 0x%X", __func__, fd, on);
    return fasync_helper(fd, file, on, &fasync);
}
#endif /* FASYNC_BT_RECOVER */

static unsigned int btmtk_usb_fops_poll(struct file *file, poll_table *wait)
{
    unsigned int mask = 0;
    int state = BTMTK_USB_STATE_UNKNOWN;

    if (g_data == NULL) {
        BTUSB_ERR("%s: ERROR, g_data is NULL!", __func__);
        return -ENODEV;
    }

    if (g_data->metabuffer->read_p == g_data->metabuffer->write_p) {
        poll_wait(file, &inq, wait);

        /* empty let select sleep */
        if ((g_data->metabuffer->read_p != g_data->metabuffer->write_p)
            || need_reset_stack == 1) {
            mask |= POLLIN | POLLRDNORM;        /* readable */
        }
    } else {
        mask |= POLLIN | POLLRDNORM;            /* readable */
    }

    /* do we need condition? */
    mask |= POLLOUT | POLLWRNORM;               /* writable */

    mutex_lock(&btmtk_usb_state_mutex);
    state = btmtk_usb_get_state();
    if (state == BTMTK_USB_STATE_FW_DUMP || state == BTMTK_USB_STATE_RESUME_FW_DUMP)
        mask |= POLLIN | POLLRDNORM;            /* readable */
    else if (state != BTMTK_USB_STATE_WORKING)  /* BTMTK_USB_STATE_WORKING: do nothing */
        mask = 0;
    mutex_unlock(&btmtk_usb_state_mutex);

    return mask;
}

/*============================================================================*/
/* Interface Functions : Kernel */
/*============================================================================*/
#define ________________________________________Interface_Function_for_Kernel
static int btmtk_usb_probe(struct usb_interface *intf,
               const struct usb_device_id *id)
{
    struct usb_endpoint_descriptor *ep_desc;
    int i;
    int state = BTMTK_USB_STATE_UNKNOWN;
    int err = -1;

    probe_counter++;

    BTUSB_INFO("%s: begin", __func__);
    BTUSB_INFO("========================================================");
    BTUSB_INFO("btmtk_usb Mediatek Bluetooth USB driver ver %s", VERSION);
    BTUSB_INFO("========================================================");
    BTUSB_INFO("probe_counter = %d", probe_counter);

    mutex_lock(&btmtk_usb_state_mutex);
    state = btmtk_usb_get_state();
    if (state == BTMTK_USB_STATE_SUSPEND_DISCONNECT)
        btmtk_usb_set_state(BTMTK_USB_STATE_SUSPEND_PROBE);
    else if (state == BTMTK_USB_STATE_RESUME_DISCONNECT)
        btmtk_usb_set_state(BTMTK_USB_STATE_RESUME_PROBE);
    else
        btmtk_usb_set_state(BTMTK_USB_STATE_PROBE);
    mutex_unlock(&btmtk_usb_state_mutex);

    /* interface numbers are hardcoded in the spec */
    if (intf->cur_altsetting->desc.bInterfaceNumber != 0) {
        BTUSB_ERR("[ERR] interface number != 0 (%d)",
              intf->cur_altsetting->desc.bInterfaceNumber);
        btmtk_usb_set_state(BTMTK_USB_STATE_DISCONNECT);
        BTUSB_ERR("btmtk_usb_probe end Error 1");
        return -ENODEV;
    }

    if (!g_data) {
        btmtk_usb_set_state(BTMTK_USB_STATE_DISCONNECT);
        BTUSB_ERR("btmtk_usb_probe end Error 2");
        return -ENOMEM;
    }

    btmtk_usb_init_memory();

    /* set the endpoint type of the interface to btmtk_usb_data */
    for (i = 0; i < intf->cur_altsetting->desc.bNumEndpoints; i++) {
        ep_desc = &intf->cur_altsetting->endpoint[i].desc;

        if (usb_endpoint_is_int_in(ep_desc)) {
            g_data->intr_ep = ep_desc;
            continue;
        }

        if (usb_endpoint_is_bulk_out(ep_desc)) {
            g_data->bulk_tx_ep = ep_desc;
            continue;
        }

        if (usb_endpoint_is_bulk_in(ep_desc)) {
            g_data->bulk_rx_ep = ep_desc;
            continue;
        }
    }

    if (!g_data->intr_ep || !g_data->bulk_tx_ep || !g_data->bulk_rx_ep) {
        btmtk_usb_set_state(BTMTK_USB_STATE_DISCONNECT);
        BTUSB_ERR("btmtk_usb_probe end Error 3");
        return -ENODEV;
    }

    g_data->udev = interface_to_usbdev(intf);
    g_data->intf = intf;

    spin_lock_init(&g_data->txlock);
    INIT_WORK(&g_data->waker, btmtk_usb_waker);

    g_data->meta_tx = 0;

    /* init all usb anchor */
    init_usb_anchor(&g_data->tx_anchor);
    init_usb_anchor(&g_data->intr_anchor);
    init_usb_anchor(&g_data->bulk_anchor);

    g_data->metabuffer->read_p = 0;
    g_data->metabuffer->write_p = 0;
    memset(g_data->metabuffer->buffer, 0, META_BUFFER_SIZE);

    btmtk_usb_cap_init();

    err = btmtk_usb_load_rom_patch();
    if (err < 0) {
        btmtk_usb_set_state(BTMTK_USB_STATE_DISCONNECT);
        BTUSB_ERR("btmtk_usb_probe end Error 4");
        return err;
    }

    /* Interface numbers are hardcoded in the specification */
    g_data->isoc = usb_ifnum_to_if(g_data->udev, 1);

    /* bind isoc interface to usb driver */
    if (g_data->isoc) {
        err =
            usb_driver_claim_interface(&btmtk_usb_driver, g_data->isoc,
                           g_data);
        if (err < 0) {
            btmtk_usb_set_state(BTMTK_USB_STATE_DISCONNECT);
            BTUSB_ERR("btmtk_usb_probe end Error 7");
            return err;
        }
    }

    usb_set_intfdata(intf, g_data);

    mutex_lock(&btmtk_usb_state_mutex);
    state = btmtk_usb_get_state();
    if (state == BTMTK_USB_STATE_SUSPEND_PROBE)
        btmtk_usb_set_state(BTMTK_USB_STATE_SUSPEND);
    else if (state == BTMTK_USB_STATE_RESUME_PROBE)
        btmtk_usb_set_state(BTMTK_USB_STATE_RESUME);
    else
        btmtk_usb_set_state(BTMTK_USB_STATE_WORKING);
    mutex_unlock(&btmtk_usb_state_mutex);

    if (need_reset_stack) {
    #if FASYNC_BT_RECOVER
        BTUSB_INFO("%s: Notify BT vendor-lib to restart stack", __func__);
        kill_fasync(&fasync, SIGIO, POLL_IN);
        need_reset_stack = 0;
    #else
        wake_up_interruptible(&inq);
    #endif
    }

    g_data->bt_fifo = btmtk_fifo_init();
    btmtk_fifo_start(g_data->bt_fifo);

    BTUSB_INFO("%s: end", __func__);
    return 0;
}

static void btmtk_usb_disconnect(struct usb_interface *intf)
{
    int state = BTMTK_USB_STATE_UNKNOWN;

    if (!usb_get_intfdata(intf))
        return;

    BTUSB_INFO("%s: begin", __func__);

    mutex_lock(&btmtk_usb_state_mutex);
    state = btmtk_usb_get_state();
    if (state == BTMTK_USB_STATE_SUSPEND || state == BTMTK_USB_STATE_SUSPEND_DISCONNECT) {
        if (register_late_resume_func) {
            BTUSB_WARN
                ("%s: disconnect happens in suspend state --> should stay in suspend state later !",
                 __func__);
            btmtk_usb_set_state(BTMTK_USB_STATE_SUSPEND_DISCONNECT);
        } else {
            BTUSB_WARN
                ("%s: disconnect hapens in suspend state --> go to disconnect state !",
                 __func__);
            btmtk_usb_set_state(BTMTK_USB_STATE_DISCONNECT);
        }
    } else if (state == BTMTK_USB_STATE_RESUME || state == BTMTK_USB_STATE_RESUME_FW_DUMP ||
           state == BTMTK_USB_STATE_RESUME_DISCONNECT) {
        BTUSB_WARN
            ("%s: disconnect hapens when driver is in resume state, should stay in resume state later !",
             __func__);
        btmtk_usb_set_state(BTMTK_USB_STATE_RESUME_DISCONNECT);
    } else
        btmtk_usb_set_state(BTMTK_USB_STATE_DISCONNECT);
    mutex_unlock(&btmtk_usb_state_mutex);

#if FASYNC_BT_RECOVER
    /* Once platform use disconn -> probe to replace suspend -> resume */
    need_reset_stack = 1;
#endif /* FASYNC_BT_RECOVER */

    if (!g_data)
        return;

    usb_set_intfdata(g_data->intf, NULL);

    if (g_data->isoc)
        usb_set_intfdata(g_data->isoc, NULL);

    if (intf == g_data->isoc)
        usb_driver_release_interface(&btmtk_usb_driver, g_data->intf);
    else if (g_data->isoc)
        usb_driver_release_interface(&btmtk_usb_driver, g_data->isoc);

    g_data->metabuffer->read_p = 0;
    g_data->metabuffer->write_p = 0;

    cancel_work_sync(&g_data->waker);

    btmtk_dbgfs_remove_drvdata();

    btmtk_fifo_init();
    g_data->bt_fifo = NULL;

    BTUSB_INFO("%s: end", __func__);
}

static int btmtk_usb_suspend(struct usb_interface *intf, pm_message_t message)
{
    BTUSB_INFO("%s: begin", __func__);

    btmtk_usb_set_state(BTMTK_USB_STATE_SUSPEND);

    if ((g_data->suspend_count++)) {
        BTUSB_WARN("%s: Has suspended. suspend_count: %d", __func__,
               g_data->suspend_count);
        return 0;
    }

    btmtk_usb_handle_entering_WoBLE_state();

    btmtk_usb_stop_traffic();
    usb_kill_anchored_urbs(&g_data->tx_anchor);

    need_reset_stack = 1;
    BTUSB_INFO("%s: end", __func__);
    return 0;
}

static int btmtk_usb_resume(struct usb_interface *intf)
{
    BTUSB_INFO("%s: begin", __func__);

    btmtk_usb_set_state(BTMTK_USB_STATE_RESUME);

    g_data->suspend_count--;
    if (g_data->suspend_count) {
        BTUSB_WARN("%s: data->suspend_count %d, return 0", __func__,
               g_data->suspend_count);
        return 0;
    }

#if FASYNC_BT_RECOVER
    if (need_reset_stack) {
        BTUSB_INFO("%s: Notify BT vendor-lib to restart stack", __func__);
        kill_fasync(&fasync, SIGIO, POLL_IN);
        need_reset_stack = 0;
    }
#endif /* FASYNC_BT_RECOVER */

    if (register_late_resume_func == NULL)
        btmtk_usb_handle_resume();
    BTUSB_INFO("%s: end", __func__);
    return 0;
}

static int btmtk_usb_reset_resume(struct usb_interface *intf)
{
    BTUSB_INFO("%s, call resume directly", __func__);
    return btmtk_usb_resume(intf);
}

static struct usb_driver btmtk_usb_driver = {
    .name = "btmtk_usb",
    .probe = btmtk_usb_probe,
    .disconnect = btmtk_usb_disconnect,
    .suspend = btmtk_usb_suspend,
    .resume = btmtk_usb_resume,
    .reset_resume = btmtk_usb_reset_resume,
    .id_table = btmtk_usb_table,
    .supports_autosuspend = 1,
    .disable_hub_initiated_lpm = 1,
};

static int __init btmtk_usb_init(void)
{
    BTUSB_INFO("%s: btmtk usb driver ver %s", __func__, VERSION);

    btmtk_dbgfs_init();
    BTUSB_DBG("debugfs is enable");

    btmtk_usb_BT_init();

    return usb_register(&btmtk_usb_driver);
}

static void __exit btmtk_usb_exit(void)
{
    BTUSB_INFO("%s: btmtk usb driver ver %s", __func__, VERSION);

    btmtk_dbgfs_exit();

    usb_deregister(&btmtk_usb_driver);
    btmtk_usb_BT_exit();
}

module_init(btmtk_usb_init);
module_exit(btmtk_usb_exit);

/**
 * Module information
 */
MODULE_DESCRIPTION("Mediatek Bluetooth USB driver ver " VERSION);
MODULE_VERSION(VERSION);
MODULE_LICENSE("GPL");
