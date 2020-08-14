/******************************************************************************

  Copyright (C), 2013-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_memdev.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/01
  Description   : Support /dev/hi_mem device.
  History       :
  1.Date        : 2013/07/01
    Author      : l00185424
    Modification: Created file

******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#include <linux/mman.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/slab.h>

/* Unf headers */

/* Drv headers */

/* Local headers */
#include "hi_drv_module.h"
#include "hi_drv_memdev.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/


/***************************** Global Definition *****************************/


/***************************** Static Definition *****************************/

static HI_S32 MEMDEV_DRV_Open(struct inode *inode, struct file *file);
static HI_S32 MEMDEV_DRV_Close(struct inode *inode, struct file *file);
static HI_S32 MEMDEV_DRV_Mmap(struct file *file, struct vm_area_struct *vm);


static struct file_operations g_stMemdevFops =
{
    .open    = MEMDEV_DRV_Open,
    .release   = MEMDEV_DRV_Close,
    .mmap    = MEMDEV_DRV_Mmap
};

static struct miscdevice g_stMemdevDevice =
{
    MISC_DYNAMIC_MINOR,
    UMAP_DEVNAME_MEMDEV,
    &g_stMemdevFops
};

/*********************************** Code ************************************/


static HI_S32 MEMDEV_DRV_Open(struct inode *inode, struct file *file)
{
    HI_INFO_MEMDEV("Open hi_mem:%s,%d\n", current->comm, current->pid);
    return HI_SUCCESS;
}

static HI_S32 MEMDEV_DRV_Close(struct inode *inode, struct file *file)
{
    HI_INFO_MEMDEV("Close hi_mem:%s,%d\n", current->comm, current->pid);
    return HI_SUCCESS;
}

static HI_S32 MEMDEV_DRV_Mmap(struct file *file, struct vm_area_struct *vma)
{
    size_t size = vma->vm_end - vma->vm_start;

    vma->vm_page_prot = phys_mem_access_prot(file, vma->vm_pgoff, size, vma->vm_page_prot);

    if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, size, vma->vm_page_prot))
    {
        HI_FATAL_MEMDEV("remap_pfn_range fail.\n");
        return -EAGAIN;
    }

    return HI_SUCCESS;
}


HI_S32 MEMDEV_DRV_ModInit(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = HI_DRV_MODULE_Register(HI_ID_MEMDEV, "HI_MEMDEV", HI_NULL);
    if(HI_SUCCESS != s32Ret)
    {
        HI_FATAL_MEMDEV("register memdev modules failed.\n");
        goto out1;
    }
    
    s32Ret = misc_register(&g_stMemdevDevice);
    if (s32Ret)
    {
        HI_FATAL_MEMDEV("hi_memdevice_dev device register failed\n");
        goto out2;
    }

    return HI_SUCCESS;
    
out2:
    HI_DRV_MODULE_UnRegister(HI_ID_MEMDEV);
out1:
    return s32Ret;
}

HI_S32 MEMDEV_DRV_ModExit(HI_VOID)
{
    misc_deregister(&g_stMemdevDevice);
    HI_DRV_MODULE_UnRegister(HI_ID_MEMDEV);
    return HI_SUCCESS;
}

MODULE_AUTHOR("Hisilicon");
MODULE_DESCRIPTION("Hisilicon Mem Device Driver");
MODULE_LICENSE("GPL");

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */
