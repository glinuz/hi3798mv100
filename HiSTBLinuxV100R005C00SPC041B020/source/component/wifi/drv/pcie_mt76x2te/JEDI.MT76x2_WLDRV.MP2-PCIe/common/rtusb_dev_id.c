/*
  * Copyright (c) 2016 MediaTek Inc.  All rights reserved.
  *
  * This software is available to you under a choice of one of two
  * licenses.  You may choose to be licensed under the terms of the GNU
  * General Public License (GPL) Version 2, available from the file
  * COPYING in the main directory of this source tree, or the
  * BSD license below:
  *
  *     Redistribution and use in source and binary forms, with or
  *     without modification, are permitted provided that the following
  *     conditions are met:
  *
  *      - Redistributions of source code must retain the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer.
  *
  *      - Redistributions in binary form must reproduce the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer in the documentation and/or other materials
  *        provided with the distribution.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  */


#define RTMP_MODULE_OS

#include "rtmp_comm.h"
#include "rt_os_util.h"
#include "rt_os_net.h"


/* module table */
USB_DEVICE_ID rtusb_dev_id[] = {
#ifdef MT76x2
{USB_DEVICE(0x0E8D, 0x7612), .driver_info = RLT_MAC_BASE},
{USB_DEVICE_AND_INTERFACE_INFO(0x0E8D, 0x7632, 0xff, 0xff, 0xff), .driver_info = RLT_MAC_BASE},
{USB_DEVICE_AND_INTERFACE_INFO(0x0E8D, 0x7662, 0xff, 0xff, 0xff), .driver_info = RLT_MAC_BASE},
{USB_DEVICE_AND_INTERFACE_INFO(0x0E8D, 0x7600, 0xff, 0xff, 0xff), .driver_info = RLT_MAC_BASE},
{USB_DEVICE_AND_INTERFACE_INFO(0x0E8D, 0x76a0, 0xff, 0xff, 0xff), .driver_info = RLT_MAC_BASE},
{USB_DEVICE_AND_INTERFACE_INFO(0x0E8D, 0x76a1, 0xff, 0xff, 0xff), .driver_info = RLT_MAC_BASE},
{USB_DEVICE_AND_INTERFACE_INFO(0x0E8D, 0x76a2, 0xff, 0xff, 0xff), .driver_info = RLT_MAC_BASE},
{USB_DEVICE_AND_INTERFACE_INFO(0x0E8D, 0x76a3, 0xff, 0xff, 0xff), .driver_info = RLT_MAC_BASE},
{USB_DEVICE_AND_INTERFACE_INFO(0x04DD, 0x76a0, 0xff, 0xff, 0xff), .driver_info = RLT_MAC_BASE},
{USB_DEVICE_AND_INTERFACE_INFO(0x04DD, 0x9a51, 0xff, 0xff, 0xff), .driver_info = RLT_MAC_BASE},
{USB_DEVICE_AND_INTERFACE_INFO(0x04DD, 0x9a50, 0xff, 0xff, 0xff), .driver_info = RLT_MAC_BASE},
{USB_DEVICE_AND_INTERFACE_INFO(0x0471, 0x7600, 0xff, 0xff, 0xff), .driver_info = RLT_MAC_BASE},
{USB_DEVICE_AND_INTERFACE_INFO(0x0471, 0x7632, 0xff, 0xff, 0xff), .driver_info = RLT_MAC_BASE},
#endif /* endif */
{} /* Terminating entry */
};

INT const rtusb_usb_id_len = sizeof(rtusb_dev_id) / sizeof(USB_DEVICE_ID);
MODULE_DEVICE_TABLE(usb, rtusb_dev_id);
