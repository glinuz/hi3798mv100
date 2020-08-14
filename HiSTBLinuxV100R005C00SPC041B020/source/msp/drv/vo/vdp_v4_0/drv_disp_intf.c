/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_disp_intf.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/15
  Description   :
  History       :
  1.Date        : 2012/09/20
  Author        :
  Modification  : Created file

*******************************************************************************/
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36))
#include <linux/smp_lock.h>
#endif
#include <linux/seq_file.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
#include <asm/system.h>
#endif
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>

#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_module.h"
#include "hi_drv_module.h"
#include "hi_kernel_adapt.h"

#include "hi_drv_video.h"
#include "hi_drv_disp.h"

#include "hi_drv_disp.h"
#include "drv_disp_ext.h"
#include "drv_display.h"
#include "drv_disp_debug.h"
#include "drv_disp.h"
#include "drv_disp_osal.h"
#include "hi_osal.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

extern HI_S32  DISP_FileOpen(struct inode *finode, struct file  *ffile);
extern HI_S32  DISP_FileClose(struct inode *finode, struct file  *ffile);
extern HI_S32  DRV_DISP_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg);
extern HI_S32  DRV_DISP_Compat_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg);

extern HI_S32  DRV_DISP_Init(HI_VOID);
extern HI_S32  DRV_DISP_DeInit(HI_VOID);
extern HI_S32  DRV_DISP_Suspend(PM_BASEDEV_S *pdev, pm_message_t state);
extern HI_S32  DRV_DISP_Resume(PM_BASEDEV_S *pdev);

extern HI_VOID DRV_WIN_UnRegister(HI_VOID);
extern HI_S32  DRV_WIN_Register(HI_VOID);
extern HI_VOID WIN_ModExit(HI_VOID);
extern HI_S32  WIN_ModInit(HI_VOID);

static UMAP_DEVICE_S g_DispRegisterData;

static long DISP_FileIoctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    long Ret;

    Ret = (long)HI_DRV_UserCopy(ffile->f_dentry->d_inode,ffile, cmd, arg, DRV_DISP_Ioctl);

    return Ret;
}
static long DISP_Compat_FileIoctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    long Ret;

    Ret = (long)HI_DRV_UserCopy(ffile->f_dentry->d_inode,ffile, cmd, arg, DRV_DISP_Compat_Ioctl);

    return Ret;
}
static struct file_operations s_DispFileOps =
{
    .owner = THIS_MODULE,
    .open           = DISP_FileOpen,
    .unlocked_ioctl = DISP_FileIoctl,
    .compat_ioctl   = DISP_Compat_FileIoctl,
    .release        = DISP_FileClose,
};

static PM_BASEOPS_S s_DispDasicOps =
{
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = DRV_DISP_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = DRV_DISP_Resume,
};

HI_VOID DISP_ModExit(HI_VOID)
{
    DRV_DISP_UnRegister();

    HI_DRV_DEV_UnRegister(&g_DispRegisterData);

    return;
}

HI_S32 DISP_ModInit(HI_VOID)
{
    /* Regist Disp device */
    HI_OSAL_Snprintf(g_DispRegisterData.devfs_name,HIMEDIA_DEVICE_NAME_MAX_LEN,"%s", UMAP_DEVNAME_DISP);
    g_DispRegisterData.fops   = &s_DispFileOps;
    g_DispRegisterData.minor  = UMAP_MIN_MINOR_DISP;
    g_DispRegisterData.owner  = THIS_MODULE;
    g_DispRegisterData.drvops = &s_DispDasicOps;

    if (HI_DRV_DEV_Register(&g_DispRegisterData) < 0)
    {
        HI_FATAL_DISP("register DISP failed.\n");
        return HI_FAILURE;
    }

#ifndef HI_MCE_SUPPORT
    if (HI_SUCCESS != DRV_DISP_Register())
    {
        //if DISP init failed, exit
        HI_DRV_DEV_UnRegister(&g_DispRegisterData);

        HI_FATAL_DISP("DRV_DISP_Init failed.\n");
        return HI_FAILURE;
    }
#endif

    return HI_SUCCESS;
}

extern HI_S32 DRV_DISP_Init2(HI_VOID);
extern HI_S32 DRV_DISP_DeInit2(HI_VOID);

HI_S32 VDP_DRV_ModInit(HI_VOID)
{
    if(HI_SUCCESS != DISP_ModInit())
    {
        HI_FATAL_DISP("DISP_ModInit failed.\n");
        return HI_FAILURE;
    }

    if(HI_SUCCESS != WIN_ModInit())
    {
        HI_FATAL_DISP("WIN_ModInit failed.\n");
        DISP_ModExit();
        return HI_FAILURE;
    }

    DRV_DISP_Init2();

#ifdef MODULE
    HI_PRINT("Load hi_vou.ko success.     \t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;
}

HI_VOID VDP_DRV_ModExit(HI_VOID)
{
    DRV_DISP_DeInit2();

    WIN_ModExit();

    DISP_ModExit();

#ifdef MODULE
    HI_PRINT("remove hi_vou.ko success.     \t(%s)\n", VERSION_STRING);
#endif

    return;
}

#ifdef MODULE
module_init(VDP_DRV_ModInit);
module_exit(VDP_DRV_ModExit);
#else
EXPORT_SYMBOL(VDP_DRV_ModInit);
EXPORT_SYMBOL(VDP_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
