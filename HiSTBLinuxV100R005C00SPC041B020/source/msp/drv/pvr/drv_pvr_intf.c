/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_pvr_intf.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/04/27
  Description   :
  History       :
  1.Date        : 2010/04/27
    Author      : jianglei 40671
    Modification: Created file

*******************************************************************************/
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/errno.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_module.h"
#include "hi_drv_module.h"
#include "hi_drv_dev.h"
#include "hi_kernel_adapt.h"

#include "pvr_debug.h"
#include "drv_pvr_ext.h"
#include "hi_drv_pvr.h"
#include "hi_error_mpi.h"

#define PVR_NAME                "HI_PVR"

static UMAP_DEVICE_S            PvrDev;
static struct file_operations   PvrFileOps;
static HI_U32                   PvrPlayChan[PVR_PLAY_MAX_CHN_NUM];
static HI_U32                   PvrRecChan[PVR_REC_MAX_CHN_NUM];
static HI_U32                   PvrDecodeAbility;

HI_DECLARE_MUTEX(PvrMutex);

static HI_S32 PVR_CreatePlay(HI_U32 *pChanId)
{
    HI_S32 ret = HI_ERR_PVR_NO_CHN_LEFT;
    HI_U32 i;

    for (i = 0; i < PVR_PLAY_MAX_CHN_NUM; i++)
    {
        if (0 == PvrPlayChan[i])
        {
            PvrPlayChan[i] = 1;

            *pChanId = i;

            ret = HI_SUCCESS;

            break;
        }
    }

    return ret;
}

static HI_S32 PVR_DestroyPlay(const HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;

    if (ChanId < PVR_PLAY_MAX_CHN_NUM)
    {
        if (1 == PvrPlayChan[ChanId])
        {
            PvrPlayChan[ChanId] = 0;

            ret = HI_SUCCESS;
        }
    }

    return ret;
}

static HI_S32 PVR_CreateRec(HI_U32 *pChanId)
{
    HI_S32 ret = HI_ERR_PVR_NO_CHN_LEFT;
    HI_U32 i;

    for (i = 0; i < PVR_REC_MAX_CHN_NUM; i++)
    {
        if (0 == PvrRecChan[i])
        {
            PvrRecChan[i] = 1;

            *pChanId = i;

            ret = HI_SUCCESS;

            break;
        }
    }

    return ret;
}

static HI_S32 PVR_DestroyRec(const HI_U32 ChanId)
{
    HI_S32 ret = HI_FAILURE;

    if (ChanId < PVR_REC_MAX_CHN_NUM)
    {
        if (1 == PvrRecChan[ChanId])
        {
            PvrRecChan[ChanId] = 0;

            ret = HI_SUCCESS;
        }
    }

    return ret;
}

static HI_S32 PVR_Ioctl(struct inode *inode, struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret;

    ret = down_interruptible(&PvrMutex);

    switch (cmd)
    {
        case CMD_PVR_CREATE_PLAY_CHN:
        {
            ret = PVR_CreatePlay((HI_U32 *)arg);

            break;
        }

        case CMD_PVR_DESTROY_PLAY_CHN:
        {
            ret = PVR_DestroyPlay(*((HI_U32 *)arg));

            break;
        }

        case CMD_PVR_CREATE_REC_CHN:
        {
            ret = PVR_CreateRec((HI_U32*)arg);

            break;
        }

        case CMD_PVR_DESTROY_REC_CHN:
        {
            ret = PVR_DestroyRec(*((HI_U32 *)arg));

            break;
        }

        case CMD_PVR_ACQUIRE_DECODE_ABILITY:
        {
            if (PvrDecodeAbility > *((HI_U32 *)arg))
            {
                PvrDecodeAbility -= *((HI_U32 *)arg);
            }
            else
            {
                ret = HI_FAILURE;
            }
            break;
        }

        case CMD_PVR_RELEASE_DECODE_ABILITY:
        {
            if (PvrDecodeAbility + *((HI_U32 *)arg) > PVR_PLAY_DECODE_ABILITY)
            {
                ret = HI_FAILURE;
            }
            else
            {
                PvrDecodeAbility += *((HI_U32 *)arg);
            }
            break;
        }

        case CMD_PVR_GET_DECODE_ABILITY:
        {
            *((HI_U32 *)arg) = PvrDecodeAbility;
            break;
        }
        
        default:
            ret = -ENOIOCTLCMD;
    }

    up(&PvrMutex);

    return ret;
}

static int PVR_DRV_Open(struct inode *inode, struct file *file)
{
    return 0;
}

static int PVR_DRV_Close(struct inode *inode, struct file *file)
{
    HI_S32 ret;
    HI_U32 i;

    ret = down_interruptible(&PvrMutex);

    for (i = 0; i < PVR_PLAY_MAX_CHN_NUM; i++)
    {
        PVR_DestroyPlay(i);
    }

    PvrDecodeAbility = PVR_PLAY_DECODE_ABILITY;

    for (i = 0; i < PVR_REC_MAX_CHN_NUM; i++)
    {
        PVR_DestroyRec(i);
    }

    up(&PvrMutex);

    return 0;
}

static long PVR_DRV_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    return (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, PVR_Ioctl);
}

HI_S32 PVR_DRV_ModInit(HI_VOID)
{
    HI_S32 ret;
    HI_U32 i;

    ret = HI_DRV_MODULE_Register(HI_ID_PVR, PVR_NAME, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    PvrFileOps.owner            = THIS_MODULE;
    PvrFileOps.open             = PVR_DRV_Open;
    PvrFileOps.release          = PVR_DRV_Close;
    PvrFileOps.unlocked_ioctl   = PVR_DRV_Ioctl;
#ifdef CONFIG_COMPAT    
    PvrFileOps.compat_ioctl     = PVR_DRV_Ioctl;
#endif 

    strncpy(PvrDev.devfs_name, UMAP_DEVNAME_PVR, sizeof(UMAP_DEVNAME_PVR));
    PvrDev.minor  = UMAP_MIN_MINOR_PVR;
    PvrDev.owner  = THIS_MODULE;
    PvrDev.fops   = &PvrFileOps;
    PvrDev.drvops = HI_NULL;

    if (HI_DRV_DEV_Register(&PvrDev) < 0)
    {
        HI_FATAL_PVR("register failed\n");

        HI_DRV_MODULE_UnRegister(HI_ID_PVR);

        return HI_FAILURE;
    }

    for (i = 0 ; i < PVR_PLAY_MAX_CHN_NUM; i++)
    {
        PvrPlayChan[i] = 0;
    }

    for (i = 0 ; i < PVR_REC_MAX_CHN_NUM; i++)
    {
        PvrRecChan[i] = 0;
    }

    PvrDecodeAbility = PVR_PLAY_DECODE_ABILITY;

#ifdef MODULE
    HI_PRINT("Load hi_pvr.ko success.  \t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;
}

HI_VOID PVR_DRV_ModExit(HI_VOID)
{
    HI_DRV_DEV_UnRegister(&PvrDev);

    HI_DRV_MODULE_UnRegister(HI_ID_PVR);
}

#ifdef MODULE
module_init(PVR_DRV_ModInit);
module_exit(PVR_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");

