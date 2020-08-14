/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-02-07
 *
******************************************************************************/
#ifndef HINFC610_OSH
#define HINFC610_OSH
/******************************************************************************/

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <asm/io.h>
#include <asm/setup.h>
#include <asm/errno.h>
#include <linux/platform_device.h>
#include <linux/mtd/partitions.h>
#include <linux/clk.h>
#include <linux/debugfs.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 5))
#  include "../../mtdcore.h"
#endif

/******************************************************************************/
#endif /* HINFC610_OSH */
