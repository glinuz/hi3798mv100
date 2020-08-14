/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_win_intf.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/09/20
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

#include "hi_module.h"
#include "hi_drv_module.h"
#include "hi_kernel_adapt.h"


//#include "mpi_priv_vi.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"

#include "hi_drv_win.h"
#include "drv_disp.h"
#include "drv_disp_osal.h"

#include "drv_window.h"
#include "drv_win_ext.h"
#include "hi_osal.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

extern HI_S32 WIN_DRV_Open(struct inode *finode, struct file  *ffile);
extern HI_S32 WIN_DRV_Close(struct inode *finode, struct file  *ffile);
extern HI_S32 DRV_WIN_Ioctl(struct inode *inode, struct file  *file, unsigned int cmd, HI_VOID *arg);
extern HI_S32 DRV_WIN_Suspend(PM_BASEDEV_S *pdev, pm_message_t state);
extern HI_S32 DRV_WIN_Resume(PM_BASEDEV_S *pdev);
extern HI_S32 DRV_WIN_SetSftwareStage(HI_VOID);


static UMAP_DEVICE_S g_WinRegisterData;


static long VO_DRV_Ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    long Ret;

    Ret = (long)HI_DRV_UserCopy(ffile->f_dentry->d_inode,ffile, cmd, arg, DRV_WIN_Ioctl);

    return Ret;
}



static struct file_operations s_WinFileOps =
{
    .owner          = THIS_MODULE,
    .open           = WIN_DRV_Open,
    .unlocked_ioctl = VO_DRV_Ioctl,
    .release        = WIN_DRV_Close,
};


static PM_BASEOPS_S  s_WinBasicOps = {
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = DRV_WIN_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = DRV_WIN_Resume,
};

HI_VOID WIN_ModExit(HI_VOID)
{
#ifndef HI_MCE_SUPPORT
    DRV_WIN_UnRegister();
#endif

    HI_DRV_DEV_UnRegister(&g_WinRegisterData);

#ifdef MODULE
    HI_PRINT("remove hi_vou.ko success.     \t(%s)\n", VERSION_STRING);
#endif

    return;
}

HI_S32 WIN_ModInit(HI_VOID)
{
    //HI_S32  Ret;

    HI_OSAL_Snprintf(g_WinRegisterData.devfs_name, HIMEDIA_DEVICE_NAME_MAX_LEN, "%s", UMAP_DEVNAME_VO);
    g_WinRegisterData.fops   = &s_WinFileOps;
    g_WinRegisterData.minor  = UMAP_MIN_MINOR_VO;
    g_WinRegisterData.owner  = THIS_MODULE;
    g_WinRegisterData.drvops = &s_WinBasicOps;
    if (HI_DRV_DEV_Register(&g_WinRegisterData) < 0)
    {
        HI_FATAL_WIN("register VO failed.\n");
        return HI_FAILURE;
    }

#ifndef HI_MCE_SUPPORT
    if (HI_SUCCESS != DRV_WIN_Register())
    {
        HI_DRV_DEV_UnRegister(&g_WinRegisterData);
        HI_FATAL_WIN("DRV_WIN_Register failed.\n");
        return HI_FAILURE;
    }
#endif

    /*here we set the sftware to normal stage. not kernel boot stage.*/
    DRV_WIN_SetSftwareStage();

#ifdef MODULE
    HI_PRINT("Load hi_vou.ko success.     \t(%s)\n", VERSION_STRING);
#endif

    return  0;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


