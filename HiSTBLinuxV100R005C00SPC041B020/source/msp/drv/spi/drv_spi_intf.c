/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/slab.h>

#include "hi_drv_spi.h"
#include "hi_common.h"
#include "hi_drv_dev.h"
#include "drv_spi_ioctl.h"
#include "hi_drv_proc.h"
#include "hi_kernel_adapt.h"
#include "hi_error_mpi.h"
#include "hi_drv_mem.h"
#include "hi_drv_mem.h"

#define MAX_SPI_CHANNEL        2

static UMAP_DEVICE_S g_SpiRegisterData;
static struct semaphore sem_spi;
extern HI_S32 HI_DRV_SPI_Ioctl(struct file *file, HI_U32 cmd, HI_SIZE_T arg);
#ifdef CONFIG_COMPAT
extern HI_S32 HI_DRV_SPI_Compat_Ioctl(struct file *file, HI_U32 cmd, HI_SIZE_T arg);
#endif


/*
 * initializes SPI interface routine.
 *
 * @return value:0--success.
 *
 */
#if (1 == HI_PROC_SUPPORT)
HI_S32 SPI_ProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U8 fform,spo,sph,dss,blend;

    PROC_PRINT(p, "---------Hisilicon SPI Info---------\n");
    PROC_PRINT(p, "---------------SPI0-----------------\n");

    HI_DRV_SPI_GetFrom(0,&fform,&spo,&sph,&dss);
    if(fform==0) {
        PROC_PRINT(p,"--Motorola SPI--\n");
        PROC_PRINT(p,"--spo=%d--\n",spo);
        PROC_PRINT(p,"--sph=%d--\n",sph);
    } else if(fform==1) {
        PROC_PRINT(p,"--T1 SPI--\n");
    } else {
        PROC_PRINT(p,"-- Microwire SPI--\n");
    }
    
    blend=HI_DRV_SPI_GetBlend(0);
    if(blend) {
        PROC_PRINT(p,"--big-end--\n");
    } else {
        PROC_PRINT(p,"--little-end--\n");
    }
    PROC_PRINT(p, "\n");

#if  defined(CHIP_TYPE_hi3718mv100)   \
            || defined(CHIP_TYPE_hi3719mv100)
        return HI_SUCCESS;
#endif

    PROC_PRINT(p, "---------------SPI1-----------------\n");
    HI_DRV_SPI_GetFrom(1,&fform,&spo,&sph,&dss);
    if(fform==0) {
        PROC_PRINT(p,"--Motorola SPI--\n");
        PROC_PRINT(p,"--spo=%d--\n",spo);
        PROC_PRINT(p,"--sph=%d--\n",sph);
    } else if(fform==1) {
        PROC_PRINT(p,"--T1 SPI--\n");
    } else {
        PROC_PRINT(p,"-- Microwire SPI--\n");
    }

    blend=HI_DRV_SPI_GetBlend(1);
    if(blend) {
        PROC_PRINT(p,"--big-end--\n");
    } else {
        PROC_PRINT(p,"--little-end--\n");
    }
    PROC_PRINT(p, "\n");

    return HI_SUCCESS;
}
#endif

static long HI_SSP_INTF_Ioctl(struct file *file, HI_U32 cmd, HI_SIZE_T arg)
{
    long ret = HI_FAILURE;
    if (down_interruptible(&sem_spi)) {
        HI_FATAL_SPI("ssp ioctl Semaphore lock is error. \n");
        return HI_FAILURE;
    }

    switch (cmd)
    {
    case CMD_SPI_OPEN:
    case CMD_SPI_CLOSE:
    case CMD_SPI_WRITE:
    case CMD_SPI_READ:
    case CMD_SPI_SET_ATTR:
    case CMD_SPI_GET_ATTR:
    case CMD_SPI_SET_BLEND:
    case CMD_SPI_GET_BLEND:
    case CMD_SPI_READEX:
    case CMD_SPI_SET_LOOP:
    case CMD_SPI_RW_LOOP:
        ret = HI_DRV_SPI_Ioctl(file, cmd, arg);
        break;

    default:
        ret = -ENOIOCTLCMD;
        break;
    }
    up(&sem_spi);
    return ret;
}

#ifdef CONFIG_COMPAT
static long HI_SSP_INTF_Compat_Ioctl(struct file *file, HI_U32 cmd, HI_SIZE_T arg)
{
    long ret = HI_FAILURE;
    if (down_interruptible(&sem_spi)) {
        HI_FATAL_SPI("ssp ioctl Semaphore lock is error. \n");
        return HI_FAILURE;
    }

    switch (cmd)
    {
    case CMD_SPI_OPEN:
    case CMD_SPI_CLOSE:
    case CMD_SPI_WRITE:
    case CMD_SPI_READ:
    case CMD_SPI_SET_ATTR:
    case CMD_SPI_GET_ATTR:
    case CMD_SPI_SET_BLEND:
    case CMD_SPI_GET_BLEND:
    case CMD_SPI_READEX:
    case CMD_SPI_SET_LOOP:
    case CMD_SPI_RW_LOOP:
        ret = HI_DRV_SPI_Compat_Ioctl(file, cmd, arg);
        break;

    default:
        ret = -ENOIOCTLCMD;
        break;
    }
    up(&sem_spi);
    return ret;
}
#endif

HI_S32 HI_SPI_DEV_Open(struct inode *inode, struct file *filp)
{
    unsigned long *pDevId = HI_KMALLOC(HI_ID_SPI,
        sizeof(unsigned long)*MAX_SPI_CHANNEL, GFP_KERNEL);
    if (!pDevId) {
        HI_ERR_SPI("spi kmalloc failed\n");
        return HI_FAILURE;
    }

    memset(pDevId, 0, sizeof(unsigned long)*MAX_SPI_CHANNEL);
    filp->private_data = pDevId;
    return HI_SUCCESS;
}


HI_S32 HI_SPI_DEV_Close(struct inode *inode, struct file *filp)
{
    int i;
    unsigned long *pDevId = filp->private_data;

    for (i=0; i<MAX_SPI_CHANNEL; i++) {
        if (pDevId && pDevId[i] == 1) {
            HI_DRV_SPI_Close(pDevId[i]);
        }
    }

    HI_KFREE(HI_ID_SPI, pDevId);
    filp->private_data = NULL;
    return HI_SUCCESS;
}

static struct file_operations SPI_FOPS =
{
    .owner = THIS_MODULE,
    .open = HI_SPI_DEV_Open,
    .unlocked_ioctl = HI_SSP_INTF_Ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = HI_SSP_INTF_Compat_Ioctl,
#endif
    .release = HI_SPI_DEV_Close,
};


HI_S32 SPI_DRV_ModInit(HI_VOID)
{
#if (1 == HI_PROC_SUPPORT)
    DRV_PROC_ITEM_S  *pProcItem;
#endif

    HI_INIT_MUTEX(&sem_spi);
    HI_DRV_SPI_Init();

    /* SSP driver register */
    snprintf(g_SpiRegisterData.devfs_name, sizeof(g_SpiRegisterData.devfs_name), UMAP_DEVNAME_SPI);
    g_SpiRegisterData.minor = UMAP_MIN_MINOR_SPI;
    g_SpiRegisterData.owner = THIS_MODULE;
    g_SpiRegisterData.fops   = &SPI_FOPS;

    if (HI_DRV_DEV_Register(&g_SpiRegisterData) < 0) {
        HI_FATAL_SPI("register SSP failed.\n");
        return HI_FAILURE;
    }
#if (1 == HI_PROC_SUPPORT)
    /* register PROC funtion*/
    pProcItem = HI_DRV_PROC_AddModule(HI_MOD_SPI, HI_NULL, HI_NULL);
    if (!pProcItem) {
        HI_INFO_SPI("add SPI proc failed.\n");
        HI_DRV_DEV_UnRegister(&g_SpiRegisterData);
        return HI_FAILURE;
    }

    pProcItem->read  = SPI_ProcRead;
#endif

#ifdef MODULE
    HI_PRINT("Load hi_spi.ko success.  \t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;
}

HI_VOID  SPI_DRV_ModExit(HI_VOID)
{
#if (1 == HI_PROC_SUPPORT)
    HI_DRV_PROC_RemoveModule(HI_MOD_SPI);
#endif

    HI_DRV_DEV_UnRegister(&g_SpiRegisterData);

    HI_DRV_SPI_DeInit();
}

#ifdef MODULE
module_init(SPI_DRV_ModInit);
module_exit(SPI_DRV_ModExit);
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HISILICON");

