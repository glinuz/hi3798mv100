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
#ifndef __BTMTK_CONFIG_H__
#define __BTMTK_CONFIG_H__

#include <linux/usb.h>
#include <linux/version.h>

/**
 * Kernel verion and configuration check
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)
    /**
     * After 3.6.0:  { USB_DEVICE_AND_INTERFACE_INFO(0x0e8d, 0x7662, 0xe0, 0x01, 0x01), .bInterfaceNumber = 0 },
     * Before 3.6.0: { USB_DEVICE_AND_INTERFACE_INFO(0x0e8d, 0x76a1, 0xe0, 0x01, 0x01)},
     */
    #error "ERROR : MTK BT Driver does NOT support kernel version earlier than 3.6.0."
#endif

#ifndef CONFIG_PM
    #error "ERROR : CONFIG_PM should be turn on."
#endif

/**
 * Support IC configuration
 */
#define SUPPORT_MT7662 1
#define SUPPORT_MT7668 1

/**
 * Debug Level Configureation
 */
#define ENABLE_BT_FIFO_THREAD   1

/**
 * ASYNC method to ask vendor lib restart BT stack, for Jeci.M0
 */
#define FASYNC_BT_RECOVER 0

/**
 * BTMTK LOG location, last char must be '/'
 */
/* #define BTMTK_LOG_PATH  "/data/misc/bluedroid/" */

/**
 * USB device ID configureation
 */
static struct usb_device_id btmtk_usb_table[] = {
#if SUPPORT_MT7662
    {USB_DEVICE_AND_INTERFACE_INFO(0x0e8d, 0x7662, 0xe0, 0x01, 0x01), .bInterfaceNumber = 0},
    {USB_DEVICE_AND_INTERFACE_INFO(0x0e8d, 0x76a0, 0xe0, 0x01, 0x01), .bInterfaceNumber = 0},
    {USB_DEVICE_AND_INTERFACE_INFO(0x0e8d, 0x76a1, 0xe0, 0x01, 0x01), .bInterfaceNumber = 0},
    {USB_DEVICE_AND_INTERFACE_INFO(0x0e8d, 0x7632, 0xe0, 0x01, 0x01), .bInterfaceNumber = 0},
#endif

#if SUPPORT_MT7668
    {USB_DEVICE_AND_INTERFACE_INFO(0x0e8d, 0x7668, 0xe0, 0x01, 0x01), .bInterfaceNumber = 0},
#endif
    {}
};

/**
 * Fixed STPBT Major Device Id
 */
/* #define FIXED_STPBT_MAJOR_DEV_ID 253 */

#endif /* __BTMTK_CONFIG_H__ */
