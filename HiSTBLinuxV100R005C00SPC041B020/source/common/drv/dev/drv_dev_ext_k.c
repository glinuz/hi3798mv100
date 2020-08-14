/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : common_ext.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/11/03
  Description   :
  History       :
  1.Date        : 2006/11/03
    Author      : c42025
    Modification: Created file

  1.Date        : 2007/11/16
    Author      : c42025
    Modification: modified definition of UMAP_DEV_NUM
******************************************************************************/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include "hi_module.h"
#include "hi_drv_dev.h"
#include "hi_drv_mem.h"


HI_S32 HI_DRV_UserCopy(struct inode *inode, struct file *file,
           HI_U32 cmd, unsigned long arg,
           HI_S32 (*func)(struct inode *inode, struct file *file,
               HI_U32 cmd, HI_VOID *arg))
{
    HI_CHAR  sbuf[128];
    HI_VOID  *mbuf = NULL;
    HI_VOID  *parg = NULL;
    HI_S32   err  = -EINVAL;

    /*  Copy arguments into temp kernel buffer  */
    switch (_IOC_DIR(cmd))
    {
        case _IOC_NONE:
            parg = NULL;
            break;
        case _IOC_READ:
        case _IOC_WRITE:
        case (_IOC_WRITE | _IOC_READ):
            if (_IOC_SIZE(cmd) <= sizeof(sbuf))
            {
                parg = sbuf;
            }
            else
            {
                HI_U32 buff_size = _IOC_SIZE(cmd);
                /* too big to allocate from stack */
                mbuf = HI_KMALLOC(HI_ID_MEM, buff_size, GFP_KERNEL);
                if (NULL == mbuf)
                {
                    HI_FATAL_DEV("malloc cmd buffer failed\n");
                    return -ENOMEM;

                }
                parg = mbuf;
            }

            err = -EFAULT;
            if (_IOC_DIR(cmd) & _IOC_WRITE)
            {
                if (copy_from_user(parg, (void __user *)arg, _IOC_SIZE(cmd)))
                {
                    HI_FATAL_DEV("copy_from_user failed, when use ioctl, \
                            the para must be a address, cmd=0x%x\n", cmd);
                    goto out;
                }
            }
            break;
    }

    /* call driver */
    err = func(inode, file, cmd, (parg));
    if (err == -ENOIOCTLCMD)
        err = -EINVAL;
    if (err < 0)
        goto out;

    /*  Copy results into user buffer  */
    switch (_IOC_DIR(cmd))
    {
        case _IOC_READ:
        case (_IOC_WRITE | _IOC_READ):
            if (copy_to_user((void __user *)arg, parg, _IOC_SIZE(cmd)))
            {
                HI_FATAL_DEV("copy_to_user failed, when use ioctl, \
                        the para must be a address, cmd=0x%x\n", cmd);
                err = -EFAULT;
            }
        break;
    }

out:
    if (mbuf)
        HI_KFREE(HI_ID_MEM, mbuf);
    return err;
}


static PM_DEVICE_S s_umap_devs[UMAP_DEV_NUM_TOTAL];

/**
 *    HI_DRV_DEV_Register - register umap devices
 *    @umapd:  device structure we want to register
 */
HI_S32 HI_DRV_DEV_Register(UMAP_DEVICE_S *umapd)
{
    HI_U32 i;
    HI_S32 ret;

    for (i = 0; i < UMAP_DEV_NUM_TOTAL; i++)
        if (0 == s_umap_devs[i].minor)
            break;

    if (i == UMAP_DEV_NUM_TOTAL)
    {
        HI_ERR_DEV("too many devices!\n");
        return -1;
    }

    s_umap_devs[i].minor = umapd->minor;
    s_umap_devs[i].name  = umapd->devfs_name;
    s_umap_devs[i].owner  = umapd->owner;
    s_umap_devs[i].app_ops  = umapd->fops;
    s_umap_devs[i].base_ops  = umapd->drvops;

    //HI_INFO_CMPI("try register dev:'%s', minor=%d.\n", umapd->devfs_name, umapd->minor);

    ret = HI_DRV_PM_Register(&s_umap_devs[i]);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_DEV("failed register dev:'%s', minor=%d, ret=%d.\n", umapd->devfs_name, umapd->minor, ret);
    }

    return ret;
}

/**
 *    HI_DRV_DEV_UnRegister - unregister a video4linux device
 *    @umapd: the device to unregister
 *
 *    This unregisters the passed device and deassigns the minor
 *    number. Future open calls will be met with errors.
 */
HI_VOID HI_DRV_DEV_UnRegister(UMAP_DEVICE_S *umapd)
{
    HI_S32 i;

	if (0 == umapd->minor)
	{
		HI_WARN_DEV("try unregister dev:'%s', but minor=%d is invalid.\n", umapd->devfs_name, umapd->minor);
		return ;
	}

    for (i = 0; i < UMAP_DEV_NUM_TOTAL; i++)
        if ( umapd->minor == s_umap_devs[i].minor)
            break;
    if (i == UMAP_DEV_NUM_TOTAL)
        return ;

    //HI_ERR_DEV("try unregister dev:'%s', minor=%d.\n", umapd->devfs_name, umapd->minor);

    HI_DRV_PM_UnRegister(&s_umap_devs[i]);
    s_umap_devs[i].minor = 0;
    return ;
}

HI_S32 HI_DRV_DEV_KInit(HI_VOID)
{
    HI_U32 i;

    for (i = 0; i < UMAP_DEV_NUM_TOTAL; i++)
    {
        s_umap_devs[i].minor = 0;
    }
    return HI_SUCCESS;
}

HI_VOID HI_DRV_DEV_KExit(HI_VOID)
{
    return ;
}


EXPORT_SYMBOL(HI_DRV_DEV_Register);
EXPORT_SYMBOL(HI_DRV_DEV_UnRegister);
EXPORT_SYMBOL(HI_DRV_UserCopy);

